/* cal_cm-v2_featurematrix_amend.c
 *
 * Implementation of the CAL API.
 *
 * This file implements the Feature Matrix function.
 */

/*****************************************************************************
* Copyright (c) 2007-2015 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_cal_cm-v2.h"        // configuration

#ifdef SFZCRYPTO_CF_USE__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement


#define SET_TRUE(_element) p_features->_element = true

#define SET_KEYRANGE(_min, _max, _step, _elem, _index)  \
    (p_features->_elem[_index][SFZCRYPTO_KEYRANGE_INDEX_MIN]) = _min;\
    (p_features->_elem[_index][SFZCRYPTO_KEYRANGE_INDEX_MAX]) =  _max;\
    (p_features->_elem[_index][SFZCRYPTO_KEYRANGE_INDEX_STEP]) =  _step


/*----------------------------------------------------------------------------
 * CALCM_FeatureMatrix_Amend
 *
 * This routine fills up the feature matrix structure will all the H/W
 * supported crypto operations, their key ranges and modes.
 */
void
CALCM_FeatureMatrix_Amend(
        SfzCryptoFeatureMatrix * const p_features)
{
    /* Supported HASH Algorithms */
#ifdef SFZCRYPTO_CF_HASH_DATA__CM
#ifndef CALCM_REMOVE_ALGO_MD5
    SET_TRUE(f_algos_hash[SFZCRYPTO_ALGO_HASH_MD5]);
#endif
#ifndef CALCM_REMOVE_ALGO_SHA160
    SET_TRUE(f_algos_hash[SFZCRYPTO_ALGO_HASH_SHA160]);
#endif
    SET_TRUE(f_algos_hash[SFZCRYPTO_ALGO_HASH_SHA224]);
    SET_TRUE(f_algos_hash[SFZCRYPTO_ALGO_HASH_SHA256]);
#endif

    /* Supported HMAC Algorithms */
#ifdef SFZCRYPTO_CF_HMAC_DATA__CM
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_HMAC]);
    SET_KEYRANGE(0,  (uint32_t)-1,  8, keyrange_sym, SFZCRYPTO_KEY_HMAC);
#endif

#ifdef SFZCRYPTO_CF_SYMM_CRYPT__CM
    /* AES is not configurable */
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_AES]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_CBC]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_CTR]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_ICM]);
    SET_KEYRANGE(128,  256, 64, keyrange_sym, SFZCRYPTO_KEY_AES);

    /* Camellia algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_CAMELLIA
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_CAMELLIA]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_CAMELLIA][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_CAMELLIA][SFZCRYPTO_MODE_CBC]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_CAMELLIA][SFZCRYPTO_MODE_CTR]);
    SET_KEYRANGE(128,  256, 64, keyrange_sym, SFZCRYPTO_KEY_CAMELLIA);
#endif

    /* DES and 3DES are optional */
#ifndef CALCM_REMOVE_ALGO_DES_3DES
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_DES]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_DES][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_DES][SFZCRYPTO_MODE_CBC]);
    SET_KEYRANGE( 64,   64,  0, keyrange_sym, SFZCRYPTO_KEY_DES);

    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_TRIPLE_DES]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_TRIPLE_DES][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_TRIPLE_DES][SFZCRYPTO_MODE_CBC]);
    SET_KEYRANGE(192,  192,  0, keyrange_sym, SFZCRYPTO_KEY_TRIPLE_DES);
#endif

    /* ARC4 is optional */
#ifndef CALCM_REMOVE_ALGO_ARCFOUR
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_ARCFOUR]);
    SET_KEYRANGE(  8, 2048,  8, keyrange_sym, SFZCRYPTO_KEY_ARCFOUR);
#endif

#ifndef CALCM_REMOVE_ALGO_AES_F8
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_F8]);
#endif

    /* C2 algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_C2
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_C2][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_C2][SFZCRYPTO_MODE_C_CBC]);
#endif

    /* MULTI2 algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_MULTI2
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_MULTI2]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_MULTI2][SFZCRYPTO_MODE_ECB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_MULTI2][SFZCRYPTO_MODE_CBC]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_MULTI2][SFZCRYPTO_MODE_CFB]);
    SET_TRUE(f_symm_crypto_modes[SFZCRYPTO_KEY_MULTI2][SFZCRYPTO_MODE_OFB]);
    SET_KEYRANGE( 64,   64,  0, keyrange_sym, SFZCRYPTO_KEY_MULTI2);
#endif

#endif /* SFZCRYPTO_CF_SYMM_CRYPT__CM */

#ifdef SFZCRYPTO_CF_CIPHER_MAC_DATA__CM
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_AES]);
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_CMAC]);
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_CBCMAC]);
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_AES][SFZCRYPTO_MODE_S2V_CMAC]);
    SET_KEYRANGE(128,  256, 64, keyrange_sym, SFZCRYPTO_KEY_AES);

    /* Camellia algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_CAMELLIA
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_CAMELLIA]);
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_CAMELLIA][SFZCRYPTO_MODE_CMAC]);
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_CAMELLIA][SFZCRYPTO_MODE_CBCMAC]);
    SET_KEYRANGE(128,  256, 64, keyrange_sym, SFZCRYPTO_KEY_CAMELLIA);
#endif

    /* C2 algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_C2
    SET_TRUE(f_cipher_mac_modes[SFZCRYPTO_KEY_C2][SFZCRYPTO_MODE_C2_H]);
#endif
#endif /* SFZCRYPTO_CF_CIPHER_MAC_DATA__CM */

    /* C2 algorithm is optional. */
#ifndef CALCM_REMOVE_ALGO_C2
    /* Set global flags for SFZCRYPTO_KEY_C2 if any function is available. */
#if defined SFZCRYPTO_CF_CPRM_C2_DERIVE__CM ||                          \
    defined SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__CM ||       \
    defined SFZCRYPTO_CF_CIPHER_MAC_DATA__CM ||                         \
    defined SFZCRYPTO_CF_SYMM_CRYPT__CM
    SET_TRUE(f_keytypes[SFZCRYPTO_KEY_C2]);
    SET_KEYRANGE( 56,   56,  0, keyrange_sym, SFZCRYPTO_KEY_C2);
#endif
#endif

#ifdef SFZCRYPTO_CF_RAND_DATA__CM
    /* the TRNG is optional */
    SET_TRUE(f_rand);
#endif

#ifdef SFZCRYPTO_CF_AUNLOCK__CM
    /* Authenticated unlock and Secure Debug is optional */
    SET_TRUE(f_aunlock);
#endif
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_USE__CM */

/* end of file cal_cm-v2_featurematrix_amend.c */
