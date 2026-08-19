/* cal_pk.h
 *
 * Function provides the CAL API with specialized names for the CAL_PK
 * implementation. Details area in the real CAL API header files.
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

#include "sfzcryptoapi.h"

SfzCryptoStatus
sfzcrypto_pk_init(void);

void
CALPK_FeatureMatrix_Amend(
        SfzCryptoFeatureMatrix * const p_features);

uint32_t
sfzcrypto_pk_read_version(
        char * version_p);

SfzCryptoStatus
sfzcrypto_pk_ecdsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_ecdsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_dsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_dsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_rsa_encrypt(
        SfzCryptoAsymKey * const p_enctx,
        SfzCryptoBigInt * const p_plaintext,
        SfzCryptoBigInt * const p_ciphertext);

SfzCryptoStatus
sfzcrypto_pk_rsa_decrypt(
        SfzCryptoAsymKey * const p_dectx,
        SfzCryptoBigInt  * const p_ciphertext,
        SfzCryptoBigInt  * const p_plaintext);

SfzCryptoStatus
sfzcrypto_pk_rsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_rsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_pk_ecdh_publicpart_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_mypubpart);

SfzCryptoStatus
sfzcrypto_pk_ecdh_sharedsecret_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

SfzCryptoStatus
sfzcrypto_pk_dh_publicpart_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_mypubpart);

SfzCryptoStatus
sfzcrypto_pk_dh_sharedsecret_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

SfzCryptoStatus
sfzcrypto_pk_gen_dh_domain_param(
        SfzCryptoDHDomainParam * const p_dh_dom_param,
        uint32_t primeBits);

SfzCryptoStatus
sfzcrypto_pk_gen_dsa_domain_param(
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        uint32_t primeBits,
        uint32_t subPrimeBits);

SfzCryptoStatus
sfzcrypto_pk_gen_dsa_key_pair(
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        SfzCryptoBigInt * const p_dsa_pubkey,
        SfzCryptoBigInt * const p_dsa_priv_key);

SfzCryptoStatus
sfzcrypto_pk_gen_ecdsa_key_pair(
        SfzCryptoECPDomainParam * const p_ec_dom_param,
        SfzCryptoECCPoint * const p_ecdsa_pub_key,
        SfzCryptoBigInt * const p_ecdsa_priv_key,
        uint32_t ec_bits_key_len);

SfzCryptoStatus
sfzcrypto_pk_gen_rsa_key_pair(
        SfzCryptoAsymKey * const p_rsa_pub_key,
        SfzCryptoAsymKey * const p_rsa_priv_key,
        uint32_t rsa_mod_bits);

SfzCryptoStatus
sfzcrypto_pk_rand_data(
        uint32_t rand_num_size_bytes,
        uint8_t * rand_num_p);

SfzCryptoStatus
sfzcrypto_pk_random_selftest(
        uint32_t control_flags,
        uint32_t * const result_flags_p);

SfzCryptoStatus
sfzcrypto_pk_random_reseed(void);

/* end of file cal_pk.h */
