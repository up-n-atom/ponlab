/* cf_cal.h
 *
 * This configuration file controls which CAL API functions are included in
 * the build and which CAL implementation provides it: stub or hw.
 *
 * Based on pre-processor defines a specific configuration file is included.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

// Initially stub all functionality
#define SFZCRYPTO_CF_FEATURE_MATRIX__STUB
#define SFZCRYPTO_CF_READ_VERSION__STUB
#define SFZCRYPTO_CF_HASH_DATA__STUB
#define SFZCRYPTO_CF_HMAC_DATA__STUB
#define SFZCRYPTO_CF_SYMM_CRYPT__STUB
#define SFZCRYPTO_CF_CIPHER_MAC_DATA__STUB
#define SFZCRYPTO_CF_AUTH_CRYPT__STUB
#define SFZCRYPTO_CF_AES_WRAP_UNWRAP__STUB
#define SFZCRYPTO_CF_CPRM_C2_DERIVE__STUB
#define SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__STUB
#define SFZCRYPTO_CF_MULTI2_CONFIGURE__STUB
#define SFZCRYPTO_CF_RANDOM_RESEED__STUB
#define SFZCRYPTO_CF_RAND_DATA__STUB
#define SFZCRYPTO_CF_RANDOM_SELFTEST__STUB
#define SFZCRYPTO_CF_NOP__STUB
#define SFZCRYPTO_CF_NVM_PUBLICDATA_READ__STUB
#define SFZCRYPTO_CF_ECDSA_SIGN__STUB
#define SFZCRYPTO_CF_ECDSA_VERIFY__STUB
#define SFZCRYPTO_CF_DSA_SIGN__STUB
#define SFZCRYPTO_CF_DSA_VERIFY__STUB
#define SFZCRYPTO_CF_RSA_ENCRYPT__STUB
#define SFZCRYPTO_CF_RSA_DECRYPT__STUB
#define SFZCRYPTO_CF_RSA_SIGN__STUB
#define SFZCRYPTO_CF_RSA_VERIFY__STUB
#define SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__STUB
#define SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__STUB
#define SFZCRYPTO_CF_DH_PUBLICPART_GEN__STUB
#define SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__STUB
#define SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__STUB
#define SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__STUB
#define SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__STUB
#define SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__STUB
#define SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__STUB
#define SFZCRYPTO_CF_ASSET_ALLOC__STUB
#define SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__STUB
#define SFZCRYPTO_CF_ASSET_FREE__STUB
#define SFZCRYPTO_CF_ASSET_SEARCH__STUB
#define SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__STUB
#define SFZCRYPTO_CF_ASSET_LOAD_GENERIC__STUB
#define SFZCRYPTO_CF_ASSET_IMPORT__STUB
#define SFZCRYPTO_CF_ASSET_DERIVE__STUB
#define SFZCRYPTO_CF_ASSET_LOAD_KEY__STUB
#define SFZCRYPTO_CF_ASSET_GEN_KEY__STUB
#define SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__STUB
#define SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__STUB
#define SFZCRYPTO_CF_AUNLOCK__STUB

#ifdef CFG_ENABLE_CM_HW1
#include "cf_cal_cm-v1.h"
#endif

#ifdef CFG_ENABLE_CM_HW2
#include "cf_cal_cm-v2.h"
#endif

/* end of file cf_cal.h */
