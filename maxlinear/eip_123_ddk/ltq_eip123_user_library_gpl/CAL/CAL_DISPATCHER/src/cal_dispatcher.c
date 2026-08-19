/* cal_dispatcher.c
 *
 * Function module provides CAL API stubs or routes the call to the configured
 * implementation.
 */

/*****************************************************************************
* Copyright (c) 2010-2015 INSIDE Secure B.V. All Rights Reserved.
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

#include "cf_cal.h"             // configuration flags

#include "sfzcryptoapi.h"       // the API to implement

#include "implementation_defs.h"
#include "c_lib.h"              // c_memset

#include "cal_sw.h"
#include "cal_cm.h"
#include "cal_pk.h"

/*---------------------------------------------------------------------------*/
// this function cannot be removed or stubbed
SfzCryptoStatus
sfzcrypto_init(
        SfzCryptoContext * const sfzcryptoctx_p)
{
    SfzCryptoStatus res = SFZCRYPTO_UNSUPPORTED;

    IDENTIFIER_NOT_USED(sfzcryptoctx_p);

#ifdef SFZCRYPTO_CF_USE__SW
    res = sfzcrypto_sw_init();
    if (res != SFZCRYPTO_SUCCESS)
        return res;
#endif

#ifdef SFZCRYPTO_CF_USE__CM
    res = sfzcrypto_cm_init();
    if (res != SFZCRYPTO_SUCCESS)
        return res;
#endif

#ifdef SFZCRYPTO_CF_USE__PK
    res = sfzcrypto_pk_init();
    if (res != SFZCRYPTO_SUCCESS)
        return res;
#endif

    return res;
}


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_FEATURE_MATRIX__REMOVE
SfzCryptoStatus
sfzcrypto_get_featurematrix(
        SfzCryptoFeatureMatrix * const p_features)
{
#ifdef SFZCRYPTO_CF_FEATURE_MATRIX__STUB
    IDENTIFIER_NOT_USED(p_features);
    return SFZCRYPTO_UNSUPPORTED;
#else
    if (p_features == NULL)
        return SFZCRYPTO_INVALID_PARAMETER;

    c_memset(p_features, 0, sizeof(SfzCryptoFeatureMatrix));

#ifdef SFZCRYPTO_CF_USE__PK
    CALPK_FeatureMatrix_Amend(p_features);
#endif
#ifdef SFZCRYPTO_CF_USE__CM
    CALCM_FeatureMatrix_Amend(p_features);
#endif
#ifdef SFZCRYPTO_CF_USE__SW
    CALSW_FeatureMatrix_Amend(p_features);
#endif

    return SFZCRYPTO_SUCCESS;
#endif /* SFZCRYPTO_CF_FEATURE_MATRIX__STUB */
}
#endif /* !SFZCRYPTO_CF_FEATURE_MATRIX__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_READ_VERSION__REMOVE
SfzCryptoStatus
sfzcrypto_read_version(
        SfzCryptoContext * const sfzcryptoctx_p,
        char * p_version,
        uint32_t * const p_len)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);

#ifdef SFZCRYPTO_CF_READ_VERSION__STUB
    IDENTIFIER_NOT_USED(p_version);
    IDENTIFIER_NOT_USED(p_len);
    return SFZCRYPTO_UNSUPPORTED;
#else

    if (p_len == NULL)
        return SFZCRYPTO_BAD_ARGUMENT;

    {
        uint32_t Len = 0;

#ifdef SFZCRYPTO_CF_USE__CM
        Len += sfzcrypto_cm_read_version(NULL);
#endif
#ifdef SFZCRYPTO_CF_USE__PK
        Len += sfzcrypto_pk_read_version(NULL);
#endif
#ifdef SFZCRYPTO_CF_USE__SW
        Len += sfzcrypto_sw_read_version(NULL);
#endif

        // handle query for length
        if (p_version == NULL)
        {
            *p_len = Len;
            return SFZCRYPTO_SUCCESS;
        }

        // if the output buffer is too small,
        // only the length is returned
        if (Len > *p_len)
        {
            *p_len = Len;
            return SFZCRYPTO_BUFFER_TOO_SMALL;
        }

        *p_len = Len;

#ifdef SFZCRYPTO_CF_USE__CM
        Len = sfzcrypto_cm_read_version(p_version);
        p_version[Len - 1] = ' ';
        p_version += Len;
#endif

#ifdef SFZCRYPTO_CF_USE__PK
        Len = sfzcrypto_pk_read_version(p_version);
        p_version[Len - 1] = ' ';
        p_version += Len;
#endif

#ifdef SFZCRYPTO_CF_USE__SW
        Len = sfzcrypto_sw_read_version(p_version);
        p_version[Len - 1] = ' ';
        p_version += Len;
#endif

        // terminate the string
        p_version[-1] = 0;
    }

    return SFZCRYPTO_SUCCESS;
#endif /* SFZCRYPTO_CF_READ_VERSION__STUB */
}
#endif /* !SFZCRYPTO_CF_READ_VERSION__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_HASH_DATA__REMOVE
SfzCryptoStatus
sfzcrypto_hash_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoHashContext * const p_ctxt,
        uint8_t * p_data,
        uint32_t length,
        bool init_with_default,
        bool final)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_HASH_DATA__STUB
    IDENTIFIER_NOT_USED(p_ctxt);
    IDENTIFIER_NOT_USED(p_data);
    IDENTIFIER_NOT_USED(length);
    IDENTIFIER_NOT_USED(init_with_default);
    IDENTIFIER_NOT_USED(final);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_HASH_DATA__SW
    return sfzcrypto_sw_hash_data(
                p_ctxt,
                p_data, length,
                init_with_default, final);
#endif
#ifdef SFZCRYPTO_CF_HASH_DATA__CM
    return sfzcrypto_cm_hash_data(
                p_ctxt,
                p_data, length,
                init_with_default, final);
#endif
}
#endif /* !SFZCRYPTO_CF_HASH_DATA__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_HMAC_DATA__REMOVE
SfzCryptoStatus
sfzcrypto_hmac_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoHmacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_HMAC_DATA__STUB
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
    IDENTIFIER_NOT_USED(p_ctxt);
    IDENTIFIER_NOT_USED(p_key);
    IDENTIFIER_NOT_USED(p_data);
    IDENTIFIER_NOT_USED(length);
    IDENTIFIER_NOT_USED(init);
    IDENTIFIER_NOT_USED(final);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_HMAC_DATA__SW
    return sfzcrypto_sw_hmac_data(
                p_ctxt, p_key,
                p_data, length,
                init, final);
#endif
#ifdef SFZCRYPTO_CF_HMAC_DATA__CM
    return sfzcrypto_cm_hmac_data(
                p_ctxt, p_key,
                p_data, length,
                init, final);
#endif
}
#endif /* !SFZCRYPTO_CF_HMAC_DATA__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_SYMM_CRYPT__REMOVE
SfzCryptoStatus
sfzcrypto_symm_crypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoCipherContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_SYMM_CRYPT__STUB
    IDENTIFIER_NOT_USED(p_ctxt);
    IDENTIFIER_NOT_USED(p_key);
    IDENTIFIER_NOT_USED(p_src);
    IDENTIFIER_NOT_USED(src_len);
    IDENTIFIER_NOT_USED(p_dst);
    IDENTIFIER_NOT_USED(p_dst_len);
    IDENTIFIER_NOT_USED(direction);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_SYMM_CRYPT__SW
    return sfzcrypto_sw_symm_crypt(
                p_ctxt, p_key,
                p_src, src_len,
                p_dst, p_dst_len,
                direction);
#endif
#ifdef SFZCRYPTO_CF_SYMM_CRYPT__CM
    return sfzcrypto_cm_symm_crypt(
                p_ctxt, p_key,
                p_src, src_len,
                p_dst, p_dst_len,
                direction);
#endif
}
#endif /* !SFZCRYPTO_CF_SYMM_CRYPT__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_CIPHER_MAC_DATA__REMOVE
SfzCryptoStatus
sfzcrypto_cipher_mac_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_CIPHER_MAC_DATA__STUB
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
    IDENTIFIER_NOT_USED(p_ctxt);
    IDENTIFIER_NOT_USED(p_key);
    IDENTIFIER_NOT_USED(p_data);
    IDENTIFIER_NOT_USED(length);
    IDENTIFIER_NOT_USED(init);
    IDENTIFIER_NOT_USED(final);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_CIPHER_MAC_DATA__SW
    return sfzcrypto_sw_cipher_mac_data(
                    p_ctxt, p_key,
                    p_data, length,
                    init, final);
#endif
#ifdef SFZCRYPTO_CF_CIPHER_MAC_DATA__CM
    return sfzcrypto_cm_cipher_mac_data(
                    p_ctxt, p_key,
                    p_data, length,
                    init, final);
#endif
}
#endif /* !SFZCRYPTO_CF_CIPHER_MAC_DATA__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_AUTH_CRYPT__REMOVE
SfzCryptoStatus
sfzcrypto_auth_crypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAuthCryptContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_nonce,
        uint32_t nonce_len,
        uint8_t * p_aad,
        uint32_t aad_len,
        uint32_t mac_len,
        uint32_t data_len,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction,
        bool init,
        bool finish)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_AUTH_CRYPT__STUB
    IDENTIFIER_NOT_USED(p_ctxt);
    IDENTIFIER_NOT_USED(p_key);
    IDENTIFIER_NOT_USED(p_nonce);
    IDENTIFIER_NOT_USED(nonce_len);
    IDENTIFIER_NOT_USED(p_aad);
    IDENTIFIER_NOT_USED(aad_len);
    IDENTIFIER_NOT_USED(mac_len);
    IDENTIFIER_NOT_USED(data_len);
    IDENTIFIER_NOT_USED(p_src);
    IDENTIFIER_NOT_USED(src_len);
    IDENTIFIER_NOT_USED(p_dst);
    IDENTIFIER_NOT_USED(p_dst_len);
    IDENTIFIER_NOT_USED(direction);
    IDENTIFIER_NOT_USED(init);
    IDENTIFIER_NOT_USED(finish);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AUTH_CRYPT__SW
    return sfzcrypto_sw_auth_crypt(
                    p_ctxt, p_key,
                    p_nonce, nonce_len,
                    p_aad, aad_len,
                    mac_len,
                    data_len,
                    p_src, src_len,
                    p_dst, p_dst_len,
                    direction,
                    init, finish);
#endif
#ifdef SFZCRYPTO_CF_AUTH_CRYPT__CM
    return sfzcrypto_cm_auth_crypt(
                    p_ctxt, p_key,
                    p_nonce, nonce_len,
                    p_aad, aad_len,
                    mac_len,
                    data_len,
                    p_src, src_len,
                    p_dst, p_dst_len,
                    direction,
                    init, finish);
#endif
}
#endif /* !SFZCRYPTO_CF_AUTH_CRYPT__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_AES_WRAP_UNWRAP__REMOVE
SfzCryptoStatus
sfzcrypto_aes_wrap_unwrap(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoCipherContext * const ctxt_p,
        SfzCryptoCipherKey * const kek_p,
        const uint8_t * src_p,
        uint32_t src_len,
        uint8_t * dst_p,
        uint32_t * const dst_len_p,
        SfzCipherOp direction,
        const uint8_t * initial_value_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_AES_WRAP_UNWRAP__STUB
    IDENTIFIER_NOT_USED(ctxt_p);
    IDENTIFIER_NOT_USED(kek_p);
    IDENTIFIER_NOT_USED(src_p);
    IDENTIFIER_NOT_USED(src_len);
    IDENTIFIER_NOT_USED(dst_p);
    IDENTIFIER_NOT_USED(dst_len_p);
    IDENTIFIER_NOT_USED(direction);
    IDENTIFIER_NOT_USED(initial_value_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AES_WRAP_UNWRAP__SW
    return sfzcrypto_sw_aes_wrap_unwrap(
                    ctxt_p,
                    kek_p,
                    src_p, src_len,
                    dst_p, dst_len_p,
                    direction,
                    initial_value_p);
#endif
}
#endif /* !SFZCRYPTO_CF_AES_WRAP_UNWRAP__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RAND_DATA__REMOVE
SfzCryptoStatus
sfzcrypto_rand_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t rand_num_size_bytes,
        uint8_t * p_rand_num)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RAND_DATA__STUB
    IDENTIFIER_NOT_USED(rand_num_size_bytes);
    IDENTIFIER_NOT_USED(p_rand_num);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RAND_DATA__SW
    return sfzcrypto_sw_rand_data(
                rand_num_size_bytes,
                p_rand_num);
#endif
#ifdef SFZCRYPTO_CF_RAND_DATA__CM
    return sfzcrypto_cm_rand_data(
                rand_num_size_bytes,
                p_rand_num);
#endif
#ifdef SFZCRYPTO_CF_RAND_DATA__PK
    return sfzcrypto_pk_rand_data(
                rand_num_size_bytes,
                p_rand_num);
#endif
}
#endif /* !SFZCRYPTO_CF_RAND_DATA__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ECDSA_SIGN__REMOVE
SfzCryptoStatus
sfzcrypto_ecdsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ECDSA_SIGN__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(p_hash_msg);
    IDENTIFIER_NOT_USED(hash_msglen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ECDSA_SIGN__SW
    return sfzcrypto_sw_ecdsa_sign(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
#ifdef SFZCRYPTO_CF_ECDSA_SIGN__PK
    return sfzcrypto_pk_ecdsa_sign(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
}
#endif /* !SFZCRYPTO_CF_ECDSA_SIGN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ECDSA_VERIFY__REMOVE
SfzCryptoStatus
sfzcrypto_ecdsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ECDSA_VERIFY__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(p_hash_msg);
    IDENTIFIER_NOT_USED(hash_msglen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ECDSA_VERIFY__SW
    return sfzcrypto_sw_ecdsa_verify(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
#ifdef SFZCRYPTO_CF_ECDSA_VERIFY__PK
    return sfzcrypto_pk_ecdsa_verify(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
}
#endif /* !SFZCRYPTO_CF_ECDSA_VERIFY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_DSA_SIGN__REMOVE
SfzCryptoStatus
sfzcrypto_dsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_DSA_SIGN__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(p_hash_msg);
    IDENTIFIER_NOT_USED(hash_msglen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_DSA_SIGN__SW
    return sfzcrypto_sw_dsa_sign(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
#ifdef SFZCRYPTO_CF_DSA_SIGN__PK
    return sfzcrypto_pk_dsa_sign(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
}
#endif /* !SFZCRYPTO_CF_DSA_SIGN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_DSA_VERIFY__REMOVE
SfzCryptoStatus
sfzcrypto_dsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_DSA_VERIFY__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(p_hash_msg);
    IDENTIFIER_NOT_USED(hash_msglen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_DSA_VERIFY__SW
    return sfzcrypto_sw_dsa_verify(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
#ifdef SFZCRYPTO_CF_DSA_VERIFY__PK
    return sfzcrypto_pk_dsa_verify(
                    p_sigctx,
                    p_signature,
                    p_hash_msg, hash_msglen);
#endif
}
#endif /* !SFZCRYPTO_CF_DSA_VERIFY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RSA_ENCRYPT__REMOVE
SfzCryptoStatus
sfzcrypto_rsa_encrypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_enctx,
        SfzCryptoBigInt * const p_plaintext,
        SfzCryptoBigInt * const p_ciphertext)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RSA_ENCRYPT__STUB
    IDENTIFIER_NOT_USED(p_enctx);
    IDENTIFIER_NOT_USED(p_plaintext);
    IDENTIFIER_NOT_USED(p_ciphertext);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RSA_ENCRYPT__SW
    return sfzcrypto_sw_rsa_encrypt(
                p_enctx,
                p_plaintext, p_ciphertext);
#endif
#ifdef SFZCRYPTO_CF_RSA_ENCRYPT__PK
    return sfzcrypto_pk_rsa_encrypt(
                p_enctx,
                p_plaintext, p_ciphertext);
#endif
}
#endif /* !SFZCRYPTO_CF_RSA_ENCRYPT__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RSA_DECRYPT__REMOVE
SfzCryptoStatus
sfzcrypto_rsa_decrypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dectx,
        SfzCryptoBigInt * const p_ciphertext,
        SfzCryptoBigInt * const p_plaintext)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RSA_DECRYPT__STUB
    IDENTIFIER_NOT_USED(p_dectx);
    IDENTIFIER_NOT_USED(p_ciphertext);
    IDENTIFIER_NOT_USED(p_plaintext);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RSA_DECRYPT__SW
    return sfzcrypto_sw_rsa_decrypt(
                    p_dectx,
                    p_ciphertext, p_plaintext);
#endif
#ifdef SFZCRYPTO_CF_RSA_DECRYPT__PK
    return sfzcrypto_pk_rsa_decrypt(
                    p_dectx,
                    p_ciphertext, p_plaintext);
#endif
}
#endif /* !SFZCRYPTO_CF_RSA_DECRYPT__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RSA_SIGN__REMOVE
SfzCryptoStatus
sfzcrypto_rsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * digest,
        uint32_t digest_len)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RSA_SIGN__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(digest);
    IDENTIFIER_NOT_USED(digest_len);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RSA_SIGN__SW
    return sfzcrypto_sw_rsa_sign(
                p_sigctx, p_signature,
                digest, digest_len);
#endif
#ifdef SFZCRYPTO_CF_RSA_SIGN__PK
    return sfzcrypto_pk_rsa_sign(
                p_sigctx, p_signature,
                digest, digest_len);
#endif
}
#endif /* !SFZCRYPTO_CF_RSA_SIGN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RSA_VERIFY__REMOVE
SfzCryptoStatus
sfzcrypto_rsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * digest,
        uint32_t digest_len)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RSA_VERIFY__STUB
    IDENTIFIER_NOT_USED(p_sigctx);
    IDENTIFIER_NOT_USED(p_signature);
    IDENTIFIER_NOT_USED(digest);
    IDENTIFIER_NOT_USED(digest_len);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RSA_VERIFY__SW
    return sfzcrypto_sw_rsa_verify(
                    p_sigctx, p_signature,
                    digest, digest_len);
#endif
#ifdef SFZCRYPTO_CF_RSA_VERIFY__PK
    return sfzcrypto_pk_rsa_verify(
                    p_sigctx, p_signature,
                    digest, digest_len);
#endif
}
#endif /* !SFZCRYPTO_CF_RSA_VERIFY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__REMOVE
SfzCryptoStatus
sfzcrypto_ecdh_publicpart_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_mypubpart)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__STUB
    IDENTIFIER_NOT_USED(p_dhctx);
    IDENTIFIER_NOT_USED(p_mypubpart);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__SW
    return sfzcrypto_sw_ecdh_publicpart_gen(
                        p_dhctx, p_mypubpart);
#endif
#ifdef SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__PK
    return sfzcrypto_pk_ecdh_publicpart_gen(p_dhctx, p_mypubpart);
#endif
}
#endif /* !SFZCRYPTO_CF_ECDH_PUBLICPART_GEN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__REMOVE
SfzCryptoStatus
sfzcrypto_ecdh_sharedsecret_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__STUB
    IDENTIFIER_NOT_USED(p_dhctx);
    IDENTIFIER_NOT_USED(p_otherpubpart);
    IDENTIFIER_NOT_USED(p_sharedsecret);
    IDENTIFIER_NOT_USED(p_sharedsecretlen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__SW
    return sfzcrypto_sw_ecdh_sharedsecret_gen(
                    p_dhctx, p_otherpubpart,
                    p_sharedsecret, p_sharedsecretlen);
#endif
#ifdef SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__PK
    return sfzcrypto_pk_ecdh_sharedsecret_gen(
                    p_dhctx, p_otherpubpart,
                    p_sharedsecret, p_sharedsecretlen);
#endif
}
#endif /* !SFZCRYPTO_CF_ECDH_SHAREDSECRET_GEN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_DH_PUBLICPART_GEN__REMOVE
SfzCryptoStatus
sfzcrypto_dh_publicpart_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_mypubpart)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_DH_PUBLICPART_GEN__STUB
    IDENTIFIER_NOT_USED(p_dhctx);
    IDENTIFIER_NOT_USED(p_mypubpart);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_DH_PUBLICPART_GEN__SW
    return sfzcrypto_sw_dh_publicpart_gen(
                        p_dhctx, p_mypubpart);
#endif
#ifdef SFZCRYPTO_CF_DH_PUBLICPART_GEN__PK
    return sfzcrypto_pk_dh_publicpart_gen(p_dhctx, p_mypubpart);
#endif
}
#endif /* !SFZCRYPTO_CF_DH_PUBLICPART_GEN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__REMOVE
SfzCryptoStatus
sfzcrypto_dh_sharedsecret_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__STUB
    IDENTIFIER_NOT_USED(p_dhctx);
    IDENTIFIER_NOT_USED(p_otherpubpart);
    IDENTIFIER_NOT_USED(p_sharedsecret);
    IDENTIFIER_NOT_USED(p_sharedsecretlen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__SW
    return sfzcrypto_sw_dh_sharedsecret_gen(
                    p_dhctx, p_otherpubpart,
                    p_sharedsecret, p_sharedsecretlen);
#endif
#ifdef SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__PK
    return sfzcrypto_pk_dh_sharedsecret_gen(
                    p_dhctx, p_otherpubpart,
                    p_sharedsecret, p_sharedsecretlen);
#endif
}
#endif /* !SFZCRYPTO_CF_DH_SHAREDSECRET_GEN__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__REMOVE
SfzCryptoStatus
sfzcrypto_gen_dh_domain_param(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoDHDomainParam * const p_dh_dom_param,
        uint32_t prime_bits)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__STUB
    IDENTIFIER_NOT_USED(p_dh_dom_param);
    IDENTIFIER_NOT_USED(prime_bits);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__SW
    return sfzcrypto_sw_gen_dh_domain_param(
                        p_dh_dom_param, prime_bits);
#endif
#ifdef SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__PK
    return sfzcrypto_pk_gen_dh_domain_param(
                        p_dh_dom_param, prime_bits);
#endif
}
#endif /* !SFZCRYPTO_CF_GEN_DH_DOMAIN_PARAM__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__REMOVE
SfzCryptoStatus
sfzcrypto_gen_dsa_domain_param(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        uint32_t prime_bits,
        uint32_t sub_prime_bits)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__STUB
    IDENTIFIER_NOT_USED(p_dsa_dom_param);
    IDENTIFIER_NOT_USED(prime_bits);
    IDENTIFIER_NOT_USED(sub_prime_bits);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__SW
    return sfzcrypto_sw_gen_dsa_domain_param(
                        p_dsa_dom_param, prime_bits, sub_prime_bits);
#endif
#ifdef SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__PK
    return sfzcrypto_pk_gen_dsa_domain_param(
                        p_dsa_dom_param, prime_bits, sub_prime_bits);
#endif
}
#endif /* !SFZCRYPTO_CF_GEN_DSA_DOMAIN_PARAM__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__REMOVE
SfzCryptoStatus
sfzcrypto_gen_dsa_key_pair(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        SfzCryptoBigInt * const p_dsa_pubkey,
        SfzCryptoBigInt * const p_dsa_priv_key)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__STUB
    IDENTIFIER_NOT_USED(p_dsa_dom_param);
    IDENTIFIER_NOT_USED(p_dsa_pubkey);
    IDENTIFIER_NOT_USED(p_dsa_priv_key);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__SW
    return sfzcrypto_sw_gen_dsa_key_pair(
                        p_dsa_dom_param, p_dsa_pubkey, p_dsa_priv_key);
#endif
#ifdef SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__PK
    return sfzcrypto_pk_gen_dsa_key_pair(
                        p_dsa_dom_param, p_dsa_pubkey, p_dsa_priv_key);
#endif
}
#endif /* !SFZCRYPTO_CF_GEN_DSA_KEY_PAIR__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__REMOVE
SfzCryptoStatus
sfzcrypto_gen_ecdsa_key_pair(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoECPDomainParam * const p_ec_dom_param,
        SfzCryptoECCPoint *const  p_ecdsa_pub_key,
        SfzCryptoBigInt * const p_ecdsa_priv_key,
        uint32_t ec_bits_key_len)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__STUB
    IDENTIFIER_NOT_USED(p_ec_dom_param);
    IDENTIFIER_NOT_USED(p_ecdsa_pub_key);
    IDENTIFIER_NOT_USED(p_ecdsa_priv_key);
    IDENTIFIER_NOT_USED(ec_bits_key_len);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__SW
    return sfzcrypto_sw_gen_ecdsa_key_pair(
                        p_ec_dom_param,
                        p_ecdsa_pub_key,
                        p_ecdsa_priv_key,
                        ec_bits_key_len);
#endif
#ifdef SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__PK
    return sfzcrypto_pk_gen_ecdsa_key_pair(
                        p_ec_dom_param,
                        p_ecdsa_pub_key,
                        p_ecdsa_priv_key,
                        ec_bits_key_len);
#endif
}
#endif /* !SFZCRYPTO_CF_GEN_ECDSA_KEY_PAIR__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__REMOVE
SfzCryptoStatus
sfzcrypto_gen_rsa_key_pair(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_rsa_pub_key,
        SfzCryptoAsymKey * const p_rsa_priv_key,
        uint32_t rsa_mod_bits)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__STUB
    IDENTIFIER_NOT_USED(p_rsa_pub_key);
    IDENTIFIER_NOT_USED(p_rsa_priv_key);
    IDENTIFIER_NOT_USED(rsa_mod_bits);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__SW
    return sfzcrypto_sw_gen_rsa_key_pair(
                    p_rsa_pub_key, p_rsa_priv_key, rsa_mod_bits);
#endif
#ifdef SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__PK
    return sfzcrypto_pk_gen_rsa_key_pair(
                    p_rsa_pub_key, p_rsa_priv_key, rsa_mod_bits);
#endif
}
#endif /* !SFZCRYPTO_CF_GEN_RSA_KEY_PAIR__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RANDOM_RESEED__REMOVE
SfzCryptoStatus
sfzcrypto_random_reseed(
        SfzCryptoContext * const sfzcryptoctx_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RANDOM_RESEED__STUB
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RANDOM_RESEED__SW
    return sfzcrypto_sw_random_reseed();
#endif
#ifdef SFZCRYPTO_CF_RANDOM_RESEED__CM
    return sfzcrypto_cm_random_reseed();
#endif
#ifdef SFZCRYPTO_CF_RANDOM_RESEED__PK
    return sfzcrypto_pk_random_reseed();
#endif
}
#endif /* !SFZCRYPTO_CF_RANDOM_RESEED__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_RANDOM_SELFTEST__REMOVE
SfzCryptoStatus
sfzcrypto_random_selftest(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t control_flags,
        uint32_t * const result_flags_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_RANDOM_SELFTEST__STUB
    if (result_flags_p)
        *result_flags_p = 0;

    if (control_flags == 0)
    {
        // upon query, return that no tests supported
        return SFZCRYPTO_SUCCESS;
    }
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_RANDOM_SELFTEST__CM
    return sfzcrypto_cm_random_selftest(control_flags, result_flags_p);
#endif
#ifdef SFZCRYPTO_CF_RANDOM_SELFTEST__PK
    return sfzcrypto_pk_random_selftest(control_flags, result_flags_p);
#endif
}
#endif /* !SFZCRYPTO_CF_RANDOM_SELFTEST__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_NOP__REMOVE
SfzCryptoStatus
sfzcrypto_nop(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoOctetsOut * dst_p,
        SfzCryptoOctetsIn * src_p,
        uint32_t len)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_NOP__STUB
    IDENTIFIER_NOT_USED(dst_p);
    IDENTIFIER_NOT_USED(src_p);
    IDENTIFIER_NOT_USED(len);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_NOP__SW
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
    return sfzcrypto_sw_nop(dst_p, src_p, len);
#endif
#ifdef SFZCRYPTO_CF_NOP__CM
    return sfzcrypto_cm_nop(dst_p, src_p, len);
#endif
}
#endif /* !SFZCRYPTO_CF_NOP__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_NVM_PUBLICDATA_READ__REMOVE
SfzCryptoStatus
sfzcrypto_nvm_publicdata_read(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t ObjectNr,
        SfzCryptoOctetsOut * Data_p,
        uint32_t * const DataLen_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_NVM_PUBLICDATA_READ__STUB
    IDENTIFIER_NOT_USED(ObjectNr);
    IDENTIFIER_NOT_USED(Data_p);
    IDENTIFIER_NOT_USED(DataLen_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_NVM_PUBLICDATA_READ__SW
    return sfzcrypto_sw_nvm_publicdata_read(ObjectNr, Data_p, DataLen_p);
#endif
#ifdef SFZCRYPTO_CF_NVM_PUBLICDATA_READ__CM
    return sfzcrypto_cm_nvm_publicdata_read(ObjectNr, Data_p, DataLen_p);
#endif
}
#endif /* !SFZCRYPTO_CF_NVM_PUBLICDATA_READ__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_ALLOC__REMOVE
SfzCryptoStatus
sfzcrypto_asset_alloc(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoPolicyMask DesiredPolicy,
        SfzCryptoSize AssetSize,
        SfzCryptoAssetId * const NewAssetId_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_ALLOC__STUB
    IDENTIFIER_NOT_USED(DesiredPolicy);
    IDENTIFIER_NOT_USED(AssetSize);
    IDENTIFIER_NOT_USED(NewAssetId_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_ALLOC__SW
    return sfzcrypto_sw_asset_alloc(
                    DesiredPolicy,
                    AssetSize,
                    NewAssetId_p);
#endif
#ifdef SFZCRYPTO_CF_ASSET_ALLOC__CM
    return sfzcrypto_cm_asset_alloc(
                    DesiredPolicy,
                    AssetSize,
                    NewAssetId_p);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_ALLOC__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__REMOVE
SfzCryptoStatus
sfzcrypto_asset_alloc_temporary(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoSymKeyType KeyType,
        SfzCryptoModeType FbMode,
        SfzCryptoHashAlgo HashAlgo,
        SfzCryptoAssetId KeyAssetId,
        SfzCryptoAssetId * const NewTempAssetId_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__STUB
    IDENTIFIER_NOT_USED(KeyType);
    IDENTIFIER_NOT_USED(FbMode);
    IDENTIFIER_NOT_USED(HashAlgo);
    IDENTIFIER_NOT_USED(KeyAssetId);
    IDENTIFIER_NOT_USED(NewTempAssetId_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__SW
    return sfzcrypto_sw_asset_alloc_temporary(
                KeyType, FbMode, HashAlgo,
                KeyAssetId,
                NewTempAssetId_p);
#endif
#ifdef SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__CM
    return sfzcrypto_cm_asset_alloc_temporary(
                KeyType, FbMode, HashAlgo,
                KeyAssetId,
                NewTempAssetId_p);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_FREE__REMOVE
SfzCryptoStatus
sfzcrypto_asset_free(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId AssetId)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_FREE__STUB
    IDENTIFIER_NOT_USED(AssetId);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_FREE__SW
    return sfzcrypto_sw_asset_free(AssetId);
#endif
#ifdef SFZCRYPTO_CF_ASSET_FREE__CM
    return sfzcrypto_cm_asset_free(AssetId);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_FREE__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_SEARCH__REMOVE
SfzCryptoStatus
sfzcrypto_asset_search(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t StaticAssetNumber,
        SfzCryptoAssetId * const NewAssetId_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_SEARCH__STUB
    IDENTIFIER_NOT_USED(StaticAssetNumber);
    IDENTIFIER_NOT_USED(NewAssetId_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_SEARCH__SW
    return sfzcrypto_sw_asset_search(
                    StaticAssetNumber,
                    NewAssetId_p);
#endif
#ifdef SFZCRYPTO_CF_ASSET_SEARCH__CM
    return sfzcrypto_cm_asset_search(
                    StaticAssetNumber,
                    NewAssetId_p);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_SEARCH__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__REMOVE
SfzCryptoAssetId
sfzcrypto_asset_get_root_key(void)
{
#ifdef SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__STUB
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__SW
    return sfzcrypto_sw_asset_get_root_key();
#endif
#ifdef SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__CM
    return sfzcrypto_cm_asset_get_root_key();
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_IMPORT__REMOVE
SfzCryptoStatus
sfzcrypto_asset_import(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_IMPORT__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(KekAssetId);
    IDENTIFIER_NOT_USED(AdditionalData_p);
    IDENTIFIER_NOT_USED(AdditionalDataSize);
    IDENTIFIER_NOT_USED(KeyBlob_p);
    IDENTIFIER_NOT_USED(KeyBlobSize);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_IMPORT__SW
    return sfzcrypto_sw_asset_import(
                TargetAssetId,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize);
#endif
#ifdef SFZCRYPTO_CF_ASSET_IMPORT__CM
    return sfzcrypto_cm_asset_import(
                TargetAssetId,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_IMPORT__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_DERIVE__REMOVE
SfzCryptoStatus
sfzcrypto_asset_derive(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KdkAssetId,
        SfzCryptoOctetsIn * Label_p,
        SfzCryptoSize LabelLen)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_DERIVE__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(KdkAssetId);
    IDENTIFIER_NOT_USED(Label_p);
    IDENTIFIER_NOT_USED(LabelLen);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_DERIVE__SW
    return sfzcrypto_sw_asset_derive(
                TargetAssetId,
                KdkAssetId,
                Label_p,
                LabelLen);
#endif
#ifdef SFZCRYPTO_CF_ASSET_DERIVE__CM
    return sfzcrypto_cm_asset_derive(
                TargetAssetId,
                KdkAssetId,
                Label_p,
                LabelLen);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_DERIVE__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_LOAD_KEY__REMOVE
SfzCryptoStatus
sfzcrypto_asset_load_key(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize DataSize)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(Data_p);
    IDENTIFIER_NOT_USED(DataSize);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY__SW
    return sfzcrypto_sw_asset_load_key(
                TargetAssetId,
                Data_p,
                DataSize);
#endif
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY__CM
    return sfzcrypto_cm_asset_load_key(
                TargetAssetId,
                Data_p,
                DataSize);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_LOAD_KEY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_GEN_KEY__REMOVE
SfzCryptoStatus
sfzcrypto_asset_gen_key(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize DataSize)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(DataSize);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY__SW
    return sfzcrypto_sw_asset_gen_key(
                TargetAssetId,
                DataSize);
#endif
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY__CM
    return sfzcrypto_cm_asset_gen_key(
                TargetAssetId,
                DataSize);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_GEN_KEY__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__REMOVE
SfzCryptoStatus
sfzcrypto_asset_load_key_and_wrap(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(Data_p);
    IDENTIFIER_NOT_USED(AssetSize);
    IDENTIFIER_NOT_USED(KekAssetId);
    IDENTIFIER_NOT_USED(AdditionalData_p);
    IDENTIFIER_NOT_USED(AdditionalDataSize);
    IDENTIFIER_NOT_USED(KeyBlob_p);
    IDENTIFIER_NOT_USED(KeyBlobSize_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__SW
    return sfzcrypto_sw_asset_load_key_and_wrap(
                TargetAssetId,
                Data_p,
                AssetSize,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize_p);
#endif
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__CM
    return sfzcrypto_cm_asset_load_key_and_wrap(
                TargetAssetId,
                Data_p,
                AssetSize,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize_p);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__REMOVE
SfzCryptoStatus
sfzcrypto_asset_gen_key_and_wrap(
        SfzCryptoContext *  const sfzcryptoctx_p,
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p)
{
    IDENTIFIER_NOT_USED(sfzcryptoctx_p);
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__STUB
    IDENTIFIER_NOT_USED(TargetAssetId);
    IDENTIFIER_NOT_USED(AssetSize);
    IDENTIFIER_NOT_USED(KekAssetId);
    IDENTIFIER_NOT_USED(AdditionalData_p);
    IDENTIFIER_NOT_USED(AdditionalDataSize);
    IDENTIFIER_NOT_USED(KeyBlob_p);
    IDENTIFIER_NOT_USED(KeyBlobSize_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__SW
    return sfzcrypto_sw_asset_gen_key_and_wrap(
                TargetAssetId,
                AssetSize,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize_p);
#endif
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__CM
    return sfzcrypto_cm_asset_gen_key_and_wrap(
                TargetAssetId,
                AssetSize,
                KekAssetId,
                AdditionalData_p,
                AdditionalDataSize,
                KeyBlob_p,
                KeyBlobSize_p);
#endif
}
#endif /* !SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_CPRM_C2_DERIVE__REMOVE
SfzCryptoStatus
sfzcrypto_cprm_c2_derive(
        SfzCryptoCprmC2KeyDeriveFunction FunctionSelect,
        SfzCryptoAssetId AssetIn,
        SfzCryptoAssetId AssetIn2,
        SfzCryptoAssetId AssetOut,
        SfzCryptoOctetsIn * InputData_p,
        SfzCryptoSize InputDataSize,
        SfzCryptoOctetsOut * OutputData_p,
        SfzCryptoSize * const OutputDataSize_p)
{
#ifdef SFZCRYPTO_CF_CPRM_C2_DERIVE__STUB
    IDENTIFIER_NOT_USED(FunctionSelect);
    IDENTIFIER_NOT_USED(AssetIn);
    IDENTIFIER_NOT_USED(AssetIn2);
    IDENTIFIER_NOT_USED(AssetOut);
    IDENTIFIER_NOT_USED(InputData_p);
    IDENTIFIER_NOT_USED(InputDataSize);
    IDENTIFIER_NOT_USED(OutputData_p);
    IDENTIFIER_NOT_USED(OutputDataSize_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_CPRM_C2_DERIVE__SW
    return sfzcrypto_sw_cprm_c2_derive(
                FunctionSelect,
                AssetIn,
                AssetIn2,
                AssetOut,
                InputData_p,
                InputDataSize,
                OutputData_p,
                OutputDataSize_p);
#endif
#ifdef SFZCRYPTO_CF_CPRM_C2_DERIVE__CM
    return sfzcrypto_cm_cprm_c2_derive(
                FunctionSelect,
                AssetIn,
                AssetIn2,
                AssetOut,
                InputData_p,
                InputDataSize,
                OutputData_p,
                OutputDataSize_p);
#endif
}
#endif /* !SFZCRYPTO_CF_CPRM_C2_DERIVE__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__REMOVE
SfzCryptoStatus
sfzcrypto_cprm_c2_devicekeyobject_rownr_get(
        SfzCryptoAssetId DeviceKeyAssetId,
        uint16_t * const RowNumber_p)
{
#ifdef SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__STUB
    IDENTIFIER_NOT_USED(DeviceKeyAssetId);
    IDENTIFIER_NOT_USED(RowNumber_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__SW
    return sfzcrypto_sw_cprm_c2_devicekeyobject_rownr_get(
                DeviceKeyAssetId,
                RowNumber_p);
#endif
#ifdef SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__CM
    return sfzcrypto_cm_cprm_c2_devicekeyobject_rownr_get(
                DeviceKeyAssetId,
                RowNumber_p);
#endif
}
#endif /* !SFZCRYPTO_CF_CPRM_C2_DEVICEKEYOBJECT_ROWNR_GET__REMOVE */


/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_MULTI2_CONFIGURE__REMOVE
SfzCryptoStatus
sfzcrypto_multi2_configure(
        uint8_t NumberOfRounds,
        SfzCryptoAssetId SystemKeyAssetId,
        const uint8_t * SystemKey_p)
{
#ifdef SFZCRYPTO_CF_MULTI2_CONFIGURE__STUB
    IDENTIFIER_NOT_USED(NumberOfRounds);
    IDENTIFIER_NOT_USED(SystemKeyAssetId);
    IDENTIFIER_NOT_USED(SystemKey_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_MULTI2_CONFIGURE__SW
    return sfzcrypto_sw_multi2_configure(
                NumberOfRounds,
                SystemKeyAssetId,
                SystemKey_p);
#endif
#ifdef SFZCRYPTO_CF_MULTI2_CONFIGURE__CM
    return sfzcrypto_cm_multi2_configure(
                NumberOfRounds,
                SystemKeyAssetId,
                SystemKey_p);
#endif
}
#endif /* !SFZCRYPTO_CF_MULTI2_CONFIGURE__REMOVE */

/*---------------------------------------------------------------------------*/
#ifndef SFZCRYPTO_CF_AUNLOCK__REMOVE

SfzCryptoStatus
sfzcrypto_authenticated_unlock_start(
        const uint16_t AuthKeyNumber,
        SfzCryptoAssetId * AuthStateASId_p,
        uint8_t * Nonce_p,
        uint32_t * NonceLength_p)
{
#ifdef SFZCRYPTO_CF_AUNLOCK__STUB
    IDENTIFIER_NOT_USED(AuthKeyNumber);
    IDENTIFIER_NOT_USED(AuthStateASId_p);
    IDENTIFIER_NOT_USED(Nonce_p);
    IDENTIFIER_NOT_USED(NonceLength_p);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AUNLOCK__CM
    return sfzcrypto_cm_authenticated_unlock_start(AuthKeyNumber,
                                                   AuthStateASId_p,
                                                   Nonce_p, NonceLength_p);
#endif
}

SfzCryptoStatus
sfzcrypto_authenticated_unlock_verify(
        const SfzCryptoAssetId AuthStateASId,
        SfzCryptoBigInt * const Signature_p,
        const uint8_t * Nonce_p,
        const uint32_t NonceLength)
{
#ifdef SFZCRYPTO_CF_AUNLOCK__STUB
    IDENTIFIER_NOT_USED(AuthStateASId);
    IDENTIFIER_NOT_USED(Signature_p);
    IDENTIFIER_NOT_USED(Nonce_p);
    IDENTIFIER_NOT_USED(NonceLength);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AUNLOCK__CM
    return sfzcrypto_cm_authenticated_unlock_verify(AuthStateASId,
                                                    Nonce_p,
                                                    NonceLength,
                                                    Signature_p->p_num,
                                                    Signature_p->byteLen);
#endif
}


SfzCryptoStatus
sfzcrypto_authenticated_unlock_release(
        const SfzCryptoAssetId AuthStateASId)
{
#ifdef SFZCRYPTO_CF_AUNLOCK__STUB
    IDENTIFIER_NOT_USED(AuthStateASId);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AUNLOCK__CM
    return sfzcrypto_cm_asset_free(AuthStateASId);
#endif
}


SfzCryptoStatus
sfzcrypto_secure_debug(
        const SfzCryptoAssetId AuthStateASId,
        const bool bSet)
{
#ifdef SFZCRYPTO_CF_AUNLOCK__STUB
    IDENTIFIER_NOT_USED(AuthStateASId);
    IDENTIFIER_NOT_USED(bSet);
    return SFZCRYPTO_UNSUPPORTED;
#endif
#ifdef SFZCRYPTO_CF_AUNLOCK__CM
    return sfzcrypto_cm_secure_debug(AuthStateASId, bSet);
#endif
}

#endif /* !SFZCRYPTO_CF_AUNLOCK__REMOVE */


/* end of file cal_dispatcher.c */
