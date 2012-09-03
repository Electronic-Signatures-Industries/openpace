/*
 * Copyright (c) 2010-2012 Dominik Oepen and Frank Morgner
 *
 * This file is part of OpenPACE.
 *
 * OpenPACE is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * OpenPACE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file eac.c
 * @brief OpenEAC implementation
 *
 * @author Frank Morgner <morgner@informatik.hu-berlin.de>
 * @author Dominik Oepen <oepen@informatik.hu-berlin.de>
 */

#include "eac_err.h"
#include "eac_kdf.h"
#include "eac_lib.h"
#include "eac_util.h"
#include "misc.h"
#include <eac/eac.h>
#include <eac/pace.h>
#include <openssl/err.h>
#include <string.h>

BUF_MEM *
EAC_add_iso_pad(const BUF_MEM * m, int block_size)
{
    BUF_MEM * out = NULL;
    int p_len;

    check(m, "Invalid arguments");

    /* calculate length of padded message */
    p_len = (m->length / block_size) * block_size + block_size;

    out = BUF_MEM_create(p_len);
    if (!out)
        goto err;

    /* Flawfinder: ignore */
    memcpy(out->data, m->data, m->length);

    /* now add iso padding */
    memset(out->data + m->length, 0x80, 1);
    memset(out->data + m->length + 1, 0, p_len - m->length - 1);

    return out;

err:
    if (out)
        BUF_MEM_free(out);

    return NULL;
}

BUF_MEM *
EAC_encrypt(const EAC_CTX *ctx, const BIGNUM *ssc, const BUF_MEM *data)
{
    check_return((ctx && ctx->key_ctx), "Invalid arguments");

    if (!update_iv(ctx->key_ctx, ctx->cipher_ctx, ssc))
        return NULL;

    return cipher_no_pad(ctx->key_ctx, ctx->cipher_ctx, ctx->key_ctx->k_enc, data, 1);
}

BUF_MEM *
EAC_decrypt(const EAC_CTX *ctx, const BIGNUM *ssc, const BUF_MEM *data)
{
    check_return((ctx && ctx->key_ctx), "Invalid arguments");

    if (!update_iv(ctx->key_ctx, ctx->cipher_ctx, ssc))
        return NULL;

    return cipher_no_pad(ctx->key_ctx, ctx->cipher_ctx, ctx->key_ctx->k_enc, data, 0);
}

BUF_MEM *
EAC_authenticate(const EAC_CTX *ctx, const BIGNUM *ssc, const BUF_MEM *data)
{
    int l;
    BUF_MEM *out = NULL, *to_authenticate = NULL;
    unsigned char *ssc_buf = NULL;

    check((ctx && data), "invalid arguments");

    l = encode_ssc(ssc, ctx->key_ctx, &ssc_buf);
    if (l < 0) {
        goto err;
    }

    to_authenticate = BUF_MEM_create(l + data->length);
    if (!to_authenticate) {
        goto err;
    }
    /* Flawfinder: ignore */
    memcpy(to_authenticate->data, ssc_buf, l);
    /* Flawfinder: ignore */
    memcpy(to_authenticate->data + l, data->data, data->length);
    to_authenticate->length = l + data->length;

    out = authenticate(ctx->key_ctx, to_authenticate);

err:
    if (ssc_buf)
        free(ssc_buf);
    /* TR-03110 uses Encrypt then authenticate, so no need to wipe the memory
     * from the authenticated data */
    if (to_authenticate)
        BUF_MEM_free(to_authenticate);

    return out;
}

BUF_MEM *
EAC_Comp(const EAC_CTX *ctx, int id, const BUF_MEM *pub)
{
    switch (id) {
        case EAC_ID_PACE:
            if (!ctx || !ctx->pace_ctx || !ctx->pace_ctx->ka_ctx) {
                log_err("Invalid arguments");
                return 0;
            }
            return Comp(ctx->pace_ctx->ka_ctx->key, pub, ctx->bn_ctx, ctx->md_ctx);

        case EAC_ID_TA:
            if (!ctx || !ctx->ta_ctx) {
                log_err("Invalid arguments");
                return 0;
            }
            if (ctx->ta_ctx->priv_key)
                return Comp(ctx->ta_ctx->priv_key, pub, ctx->bn_ctx, ctx->md_ctx);
            else
                return Comp(ctx->ta_ctx->pub_key, pub, ctx->bn_ctx, ctx->md_ctx);

        case EAC_ID_CA:
            if (!ctx || !ctx->ca_ctx || !ctx->ca_ctx->ka_ctx) {
                log_err("Invalid arguments");
                return 0;
            }
            return Comp(ctx->ca_ctx->ka_ctx->key, pub, ctx->bn_ctx, ctx->md_ctx);

        default:
            log_err("Invalid arguments");
            return NULL;
    }
}

BUF_MEM *
EAC_hash_certificate_description(const unsigned char *cert_desc,
        size_t cert_desc_len)
{
    BUF_MEM *cd, *out;

    cd = BUF_MEM_create_init(cert_desc, cert_desc_len);
    out = hash(EVP_sha256(), NULL, NULL, cd);
    if (cd)
        BUF_MEM_free(cd);

    return out;
}

int
EAC_CTX_set_encryption_ctx(EAC_CTX *ctx, int id)
{
    const KA_CTX *new;

    switch (id) {
        case EAC_ID_PACE:
            if (!ctx || !ctx->pace_ctx || !ctx->pace_ctx->ka_ctx ||
                    !ctx->pace_ctx->ka_ctx->k_enc || !ctx->pace_ctx->ka_ctx->k_mac) {
                log_err("Invalid arguments");
                return 0;
            }
            new = ctx->pace_ctx->ka_ctx;
            break;

        case EAC_ID_CA:
            if (!ctx || !ctx->ca_ctx || !ctx->ca_ctx->ka_ctx ||
                    !ctx->ca_ctx->ka_ctx->k_enc || !ctx->ca_ctx->ka_ctx->k_mac) {
                log_err("Invalid arguments");
                return 0;
            }
            new = ctx->ca_ctx->ka_ctx;
            break;

        case EAC_ID_EAC:
            if (!ctx || !ctx->key_ctx || !ctx->key_ctx->k_enc || !ctx->key_ctx->k_mac) {
                log_err("Invalid arguments");
                return 0;
            }
            return 1;
            break;

        default:
            log_err("Invalid arguments");
            return 0;
    }

    KA_CTX_clear_free(ctx->key_ctx);
    ctx->key_ctx = KA_CTX_dup(new);
    if (!ctx->key_ctx)
        return 0;

    return 1;
}

BUF_MEM *
KA_CTX_generate_key(const KA_CTX *ctx, BN_CTX *bn_ctx)
{
    check_return((ctx && ctx->generate_key), "Invalid arguments");

    return ctx->generate_key(ctx->key, bn_ctx);
}

int
KA_CTX_compute_key(KA_CTX *ctx, const BUF_MEM *in, BN_CTX *bn_ctx)
{
    if (!ctx || !ctx->compute_key) {
        log_err("Invalid arguments");
        return 0;
    }

    BUF_MEM_clear_free(ctx->shared_secret);
    ctx->shared_secret = ctx->compute_key(ctx->key, in, bn_ctx);
    if (!ctx->shared_secret)
        return 0;

    return 1;
}

int
KA_CTX_derive_keys(KA_CTX *ctx, const BUF_MEM *nonce, EVP_MD_CTX *md_ctx)
{
    if (!ctx) {
        log_err("Invalid arguments");
        return 0;
    }

    BUF_MEM_clear_free(ctx->k_mac);
    ctx->k_mac = kdf_mac(nonce, ctx, md_ctx);

    BUF_MEM_clear_free(ctx->k_enc);
    ctx->k_enc = kdf_enc(nonce, ctx, md_ctx);

    if (!ctx->k_mac || !ctx->k_enc)
        return 0;

    return 1;
}