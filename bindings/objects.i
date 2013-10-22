/*
 * Copyright (c) 2013 Frank Morgner
 *
 * This file is part of OpenPACE.
 *
 * OpenPACE is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * OpenPACE is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * OpenPACE.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include <eac/objects.h>

const int id_PACE_DH_GM_3DES_CBC_CBC = NID_id_PACE_DH_GM_3DES_CBC_CBC;
const int id_PACE_DH_GM_AES_CBC_CMAC_128 = NID_id_PACE_DH_GM_AES_CBC_CMAC_128;
const int id_PACE_DH_GM_AES_CBC_CMAC_192 = NID_id_PACE_DH_GM_AES_CBC_CMAC_192;
const int id_PACE_DH_GM_AES_CBC_CMAC_256 = NID_id_PACE_DH_GM_AES_CBC_CMAC_256;
const int id_PACE_ECDH_GM_3DES_CBC_CBC = NID_id_PACE_ECDH_GM_3DES_CBC_CBC;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_128 = NID_id_PACE_ECDH_GM_AES_CBC_CMAC_128;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_192 = NID_id_PACE_ECDH_GM_AES_CBC_CMAC_192;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_256 = NID_id_PACE_ECDH_GM_AES_CBC_CMAC_256;
const int id_PACE_DH_IM_3DES_CBC_CBC = NID_id_PACE_DH_IM_3DES_CBC_CBC;
const int id_PACE_DH_IM_AES_CBC_CMAC_128 = NID_id_PACE_DH_IM_AES_CBC_CMAC_128;
const int id_PACE_DH_IM_AES_CBC_CMAC_192 = NID_id_PACE_DH_IM_AES_CBC_CMAC_192;
const int id_PACE_DH_IM_AES_CBC_CMAC_256 = NID_id_PACE_DH_IM_AES_CBC_CMAC_256;
const int id_PACE_ECDH_IM_3DES_CBC_CBC = NID_id_PACE_ECDH_IM_3DES_CBC_CBC;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_128 = NID_id_PACE_ECDH_IM_AES_CBC_CMAC_128;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_192 = NID_id_PACE_ECDH_IM_AES_CBC_CMAC_192;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_256 = NID_id_PACE_ECDH_IM_AES_CBC_CMAC_256;

const int id_CA_DH_3DES_CBC_CBC       = NID_id_CA_DH_3DES_CBC_CBC;
const int id_CA_DH_AES_CBC_CMAC_128   = NID_id_CA_DH_AES_CBC_CMAC_128;
const int id_CA_DH_AES_CBC_CMAC_192   = NID_id_CA_DH_AES_CBC_CMAC_192;
const int id_CA_DH_AES_CBC_CMAC_256   = NID_id_CA_DH_AES_CBC_CMAC_256;
const int id_CA_ECDH_3DES_CBC_CBC     = NID_id_CA_ECDH_3DES_CBC_CBC;
const int id_CA_ECDH_AES_CBC_CMAC_128 = NID_id_CA_ECDH_AES_CBC_CMAC_128;
const int id_CA_ECDH_AES_CBC_CMAC_192 = NID_id_CA_ECDH_AES_CBC_CMAC_192;
const int id_CA_ECDH_AES_CBC_CMAC_256 = NID_id_CA_ECDH_AES_CBC_CMAC_256;

const int id_RI_DH_SHA_1 = NID_id_RI_DH_SHA_1;
const int id_RI_DH_SHA_224 = NID_id_RI_DH_SHA_224;
const int id_RI_DH_SHA_256 = NID_id_RI_DH_SHA_256;
const int id_RI_DH_SHA_384 = NID_id_RI_DH_SHA_384;
const int id_RI_DH_SHA_512 = NID_id_RI_DH_SHA_512;
const int id_RI_ECDH_SHA_1 = NID_id_RI_ECDH_SHA_1;
const int id_RI_ECDH_SHA_224 = NID_id_RI_ECDH_SHA_224;
const int id_RI_ECDH_SHA_256 = NID_id_RI_ECDH_SHA_256;
const int id_RI_ECDH_SHA_384 = NID_id_RI_ECDH_SHA_384;
const int id_RI_ECDH_SHA_512 = NID_id_RI_ECDH_SHA_512;

%}

const int id_PACE_DH_GM_3DES_CBC_CBC;
const int id_PACE_DH_GM_AES_CBC_CMAC_128;
const int id_PACE_DH_GM_AES_CBC_CMAC_192;
const int id_PACE_DH_GM_AES_CBC_CMAC_256;
const int id_PACE_ECDH_GM_3DES_CBC_CBC;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_128;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_192;
const int id_PACE_ECDH_GM_AES_CBC_CMAC_256;
const int id_PACE_DH_IM_3DES_CBC_CBC;
const int id_PACE_DH_IM_AES_CBC_CMAC_128;
const int id_PACE_DH_IM_AES_CBC_CMAC_192;
const int id_PACE_DH_IM_AES_CBC_CMAC_256;
const int id_PACE_ECDH_IM_3DES_CBC_CBC;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_128;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_192;
const int id_PACE_ECDH_IM_AES_CBC_CMAC_256;

const int id_CA_DH_3DES_CBC_CBC;
const int id_CA_DH_AES_CBC_CMAC_128;
const int id_CA_DH_AES_CBC_CMAC_192;
const int id_CA_DH_AES_CBC_CMAC_256;
const int id_CA_ECDH_3DES_CBC_CBC;
const int id_CA_ECDH_AES_CBC_CMAC_128;
const int id_CA_ECDH_AES_CBC_CMAC_192;
const int id_CA_ECDH_AES_CBC_CMAC_256;

const int id_RI_DH_SHA_1;
const int id_RI_DH_SHA_224;
const int id_RI_DH_SHA_256;
const int id_RI_DH_SHA_384;
const int id_RI_DH_SHA_512;
const int id_RI_ECDH_SHA_1;
const int id_RI_ECDH_SHA_224;
const int id_RI_ECDH_SHA_256;
const int id_RI_ECDH_SHA_384;
const int id_RI_ECDH_SHA_512;
