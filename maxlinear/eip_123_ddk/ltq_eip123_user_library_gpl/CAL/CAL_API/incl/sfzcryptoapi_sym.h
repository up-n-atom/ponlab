/* sfzcryptoapi_sym.h
 *
 * The Cryptographic Abstraction Layer API: Symmetric Cryptography.
 */

/*****************************************************************************
* Copyright (c) 2007-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_SYM_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_SYM_H

#include "public_defs.h"            // uint8_t, uint32_t, etc.

#include "sfzcryptoapi_enum.h"
#include "sfzcryptoapi_asset.h"     // SfzCryptoAssetId type

#define SFZCRYPTO_ARCFOUR_STATE_DATA_LEN 256

#define SFZCRYPTO_AES_IV_LEN 16
#define SFZCRYPTO_DES_IV_LEN 8

#define SFZCRYPTO_AES_BLOCK_LEN      16
#define SFZCRYPTO_CAMELLIA_BLOCK_LEN 16
#define SFZCRYPTO_DES_BLOCK_LEN      8
#define SFZCRYPTO_C2_BLOCK_LEN       8
#define SFZCRYPTO_MULTI2_BLOCK_LEN   8

#define SFZCRYPTO_MAX_IVLEN     16    /* Maximum IV length. */
#define SFZCRYPTO_MAX_KEYLEN    256   /* Maximum key length. */


/* Context structure to maintain the intermediate/final digest for a hash.

*****************************************************************************/
typedef struct
{
    SfzCryptoHashAlgo algo;
    /* Algorithm. */
    uint32_t count[2];
    /* 64-bit bit count, not to be filled by user. */
    uint8_t digest[32];
    /* At least big enough to hold a SHA2 digest. */
} SfzCryptoHashContext;

/*
   Common structure to maintain the intermediate/final digest for HMAC.

   The *_loc values must be one of LOC_* values.

   mac_asset_id
   Asset Identifier of the temporary asset
   used to hold the intermediate MAC value.

   mac_loc
   Location of the temporary MAC.
   See SfzCryptoLocation for details.

*****************************************************************************/
typedef struct
{
    /* Embedded hashing context */
    SfzCryptoHashContext  hashCtx;
    /* Reference to (temp) MAC asset */
    SfzCryptoAssetId   mac_asset_id;
    /* Actual MAC location */
    SfzCryptoLocation  mac_loc;
} SfzCryptoHmacContext;     /* HMAC context. */

/*
   Structure to hold a symmetric crypto key in plain.
*/
typedef struct
{
    /* Type of the key (AES, DES, TDES, HMAC, etc). */
    SfzCryptoSymKeyType type;
    /* Reference to the key. Set to SFZCRYPTO_ASSETID_INVALID if none */
    SfzCryptoAssetId asset_id;
    /* Length in bytes. */
    uint32_t       length;
    /* The key buffer, at least as big as the
        largest possible key (256 byte ARCFOUR key).*/
    uint8_t        key[SFZCRYPTO_MAX_KEYLEN];
    /* Salt key for AES f8 mode. */
    uint8_t        f8_salt_key[16];
    /* Length of salt key for AES f8 mode. */
    uint32_t       f8_salt_keyLen;
}
SfzCryptoCipherKey;


/*
   Structure to maintain the IV and other miscellaneous functionality
   during a cryptographic operation.

   iv_asset_id
   Asset Identifier of the temporary asset used to hold the IV value.

   iv_loc
   Location of the temporary IV.
   See SfzCryptoLocation for details.
*/
typedef struct
{
    /* FeedBack mode (ECB, CBC etc). */
    SfzCryptoModeType fbmode;
    /* The IV. */
    uint8_t         iv[SFZCRYPTO_MAX_IVLEN];
    /* Reference to IV asset  */
    SfzCryptoAssetId  iv_asset_id;
    /** Actual location of the IV. */
    SfzCryptoLocation iv_loc;

    struct            /* State for ARCFOUR when algorithm is ARCFOUR. */
    {
        /* Keystream. */
        uint8_t keystream[SFZCRYPTO_ARCFOUR_STATE_DATA_LEN];
        /* Position i. */
        uint8_t i;
        /* Position j. */
        uint8_t j;
    } ARCFOUR_state;

    uint8_t f8_iv[16]; /* IV for AES f8. */
    uint8_t f8_keystream[16]; /* Needed for AES f8 continuation case. */
}
SfzCryptoCipherContext;

// following can be used in SfzCryptoCipherContext.fbmode for ARC4 operations
// (when SfzCryptoCipherKey.type == SFZCRYPTO_KEY_ARCFOUR)
enum
{
    /* Mode: ARC4 oneshot (stateless -> stateless) */
    SFZCRYPTO_MODE_ARC4_STATELESS = 0,
    /* Mode: ARC4 finalize (stateful -> stateless) */
    SFZCRYPTO_MODE_ARC4_FINAL = 1,
    /* Mode: ARC4 initial (stateless -> stateful) */
    SFZCRYPTO_MODE_ARC4_INITIAL = 2,
    /* Mode: ARC4 continue (stateful -> stateful) */
    SFZCRYPTO_MODE_ARC4_STATEFUL = 3
};


/* Context structure for authenticated encryption. */
typedef struct
{
    SfzCryptoCipherContext ctxt;
    uint8_t iv[SFZCRYPTO_MAX_IVLEN];      /* For integrity sum. */
    uint8_t counter[SFZCRYPTO_MAX_IVLEN]; /* For CTR-mode. */
} SfzCryptoAuthCryptContext;

/* Cipher MAC shares structure with encryption operations. */
typedef SfzCryptoCipherContext SfzCryptoCipherMacContext;


/* ------------------------------------------------------------------------- */
/* MANAGING KEY MATERIAL                                                     */

/* Initializes everything in key including the key data.
   The macro is incompatible with AES-f8. */
#define SFZCRYPTO_CIPHER_KEY_INIT(sfzcryptokey_p, keytype,                 \
                                  keymat_ptr, keylength_bytes)             \
    do { (sfzcryptokey_p)->type = (keytype);                               \
         (sfzcryptokey_p)->asset_id = (SFZCRYPTO_ASSETID_INVALID);         \
         (sfzcryptokey_p)->length = (keylength_bytes);                     \
         c_memcpy((sfzcryptokey_p)->key, (keymat_ptr), (keylength_bytes)); \
    } while(0)

/* Initializes everything in key except the actual key data.
   The macro is incompatible with AES-f8. */
#define SFZCRYPTO_CIPHER_KEY_INIT_WITHOUT_KEYDATA(sfzcryptokey_p, keytype, \
                                                  keylength_bytes)         \
    do { (sfzcryptokey_p)->type = (keytype);                               \
         (sfzcryptokey_p)->asset_id = (SFZCRYPTO_ASSETID_INVALID);         \
         (sfzcryptokey_p)->length = (keylength_bytes);                     \
    } while(0)


/*
   Hash data.

   Hashes data using the algorithm specified in the ctxt parameter. The
   ctxt parameter also holds the final or intermediate digest in the
   appropriate field see the declaration of SfzCryptoHashContext for
   more details.

   @pre p_ctxt != NULL

   @pre p_data != NULL points to a valid pre-allocated buffer of length bytes

   @pre (length % 64) == 0 if (final == FALSE)

   @pre When calling this API in sequence for a single stream of data, the
   caller must make sure that one call has finished completely before
   the next call is made because the API will update data in p_ctxt and
   the start of one call before the end of previous one can start the
   operation with inconsistent values.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_ctxt
   Context to maintain the intermediate digest.

   @param p_data
   Pointer to the input data to be hashed.

   @param length
   Length of the data to hash (expressed in bytes).

   @param init
   TRUE if this is the first chunk of data to be hashed.

   @param final
   TRUE if this is the last chunk of data to be hashed.

   @return
   One of the SfzCryptoStatus values.

*****************************************************************************/
SfzCryptoStatus
sfzcrypto_hash_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoHashContext * const p_ctxt,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

/*
   Calculate HMAC of given data.

   Calculates the HMAC of the data using the algorithm specified
   in the ctxt. The ctxt value holds the final or intermediate
   digest in the appropriate field. see the declaration of
   SfzCryptoHmacContext for more details.

   @pre p_ctxt != NULL

   @pre p_data != NULL points to a valid pre-allocated buffer of >= length
   bytes.

   @pre p_key points to a valid populated SfzCryptoCipherKey.

   @pre key->type == SFZCRYPTO_ALGO_HMAC

   @pre (length % 64) == 0 if (final == FALSE)

   @pre p_key->length <= 256 bytes

   @pre When calling this API in sequence for a single stream of data, the
   caller must make sure that one call has finished completely before
   the next call is made because the API will update data in p_ctxt and
   the start of one call before the end of previous one can start the
   operation with inconsistent values.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_ctxt
   Context to maintain the intermediate digest.

   @param p_key
   Contains the key to use. See the SfzCryptoCipherKey declaration.

   @param p_data
   Pointer to the input data to be hashed.

   @param length
   Length in bytes of the data to HMAC.

   @param init
   TRUE if this is the first chunk of data to be hmaced.

   @param final
   TRUE if this is the last chunk of data to be hmaced.

   @return
   One of the SfzCryptoStatus values.

*****************************************************************************/
SfzCryptoStatus
sfzcrypto_hmac_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoHmacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

/*
   Calculate a cipher MAC of given data.

   Calculates the cipher MAC of the data using the algorithm implied
   by p_ctxt->fbmode (e.g. CMAC or CBCMAC) and p_key->type (e.g. AES)

   @pre p_ctxt != NULL

   @pre p_data != NULL points to a valid pre-allocated buffer of >= length
   bytes.

   @pre p_key points to a valid populated SfzCryptoCipherKey.

   @pre key->type == SFZCRYPTO_KEY_AES ||
        key->type == SFZCRYPTO_KEY_CAMELLIA ||
        key->type == SFZCRYPTO_KEY_C2

   @pre (length % blocklen) == 0 if (final == FALSE),
        where blocklen == 16 for AES/Camellia, and 8 for C2.

   @pre p_key->length is valid key length for the algorithm.

   @pre When calling this API in sequence for a single stream of data, the
   caller must make sure that one call has finished completely before
   the next call is made because the API will update data in p_ctxt and
   the start of one call before the end of previous one can start the
   operation with inconsistent values.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_ctxt
   Context to maintain the intermediate MAC.

   @param p_key
   Contains the key to use. See the SfzCryptoCipherKey declaration.

   @param p_data
   Pointer to the input data to be mac'ed.

   @param length
   Length in bytes of the data to process.

   @param init
   TRUE if this is the first chunk of data to be processed.

   @param final
   TRUE if this is the last chunk of data to be processed.

   @return
   One of the SfzCryptoStatus values.

*****************************************************************************/
SfzCryptoStatus
sfzcrypto_cipher_mac_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

/*
   Encrypt or decrypt data using a symmetric crypto algorithm.

   This specification deprecates the handling of non-block length data for modes
   that do not require the input data length to be a multiple of the block length.
   Even for modes which do require a modulo block-size length, it will be handled
   if the user specifies a padding scheme. Presently only the PKCS#7 padding
   scheme is allowed. When padding is in effect in an encryption operation, the
   output buffer is usually required to be bigger than the input data. While in
   the case of decryption, the output will be shorter than the input data since
   the padding would have been required. In either case, the implementation will
   check if the output buffer is big enough for the operation. The exact behavior
   is described below in the description of the parameter p_dst_len.

   @note This specification deprecates the support for padding.

   @pre p_ctxt != NULL && key != NULL

   @pre p_src and p_dst point to valid pre-allocated buffers.

   @pre p_key points to a valid populated SfzCryptoCipherKey.

   @pre When using this API in sequence on a stream of data, each data chunk
   must be a multiple of the block-size for that algo unless it is the
   last chunk. Morover, the last chunk can be non-block size multiple
   if and only if either a padding scheme has been selected or the mode
   itself allows data to be non-block size. Such modes are AES-CTR and AES-ICM.
   The block lengths of AES, DES, 3DES and ARC4 are 16, 8, 8 and 1 respectively.
   This specification however deprecates the support for padding.

   @pre When calling this API in sequence for a single stream of data, the
   caller must make sure that one call has finished completely before
   the next call is made because the API will update data in p_ctxt and
   the start of one call before the end of previous one can start the
   operation with inconsistent values.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_ctxt
   Context to maintain the IV. See SfzCryptoCipherContext.

   @param p_key
   Pointer to the key to use. See SfzCryptoCipherKey.

   @param p_src
   Pointer to the input data to be (en/de)crypted.

   @param src_len
   Length in bytes of the data to be (en/de)crypted.

   @param p_dst
   Pointer to the buffer that receives the resulting text.

   @param p_dst_len
   Pointer to the length of the destination buffer reserved to receive the result
   of the (en/de)cryption. This is an input output parameter. If the value it
   points to is less than what is required by the implementation, then the API
   fails with return code SFZCRYPTO_BUFFER_TOO_SMALL and sets the value to the
   minimum required dst buffer length to succeed. On a successfull return, the
   value is set to the exact length of the returned output.

   For most algos and modes, the required destination length is usually the same
   as the input data length. There are exceptions though and the p_dst_len param
   helps the implementation to communicate to the user what a better length for
   the destination buffer would be. For example, in case of encryption, when
   padding is requested, the dst buffer must be bigger by at least one extra
   block. This specification however deprecates the support for padding.

   @param direction
   TRUE for encryption and FALSE for decryption.

   @return
   One of the SfzCryptoStatus values.

*****************************************************************************/
SfzCryptoStatus
sfzcrypto_symm_crypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoCipherContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

/*
   Encrypt or decrypt data using a authenticating encryption algorithm.

   Non-block length data is allowed, but processing is single part processing.

   @pre p_ctxt != NULL && p_key != NULL

   @pre p_src and p_dst point to valid pre-allocated buffers.

   @pre p_key points to a valid populated SfzCryptoCipherKey.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_ctxt
   Used to maintain the state of a multipart authenticated crypto operation.

   @param p_key
   Pointer to the key to use. See SfzCryptoCipherKey.
   AES-CCM will be used when p_key->type == SFZCRYPTO_KEY_AES
   AES-SIV will be used when p_key->type == SFZCRYPTO_KEY_AES_SIV

   @param p_nonce
   Nonce to use in cryption.

   @param nonce_len
   Length of nonce. Note: this parameter is used to calculate width
   of counter to use.

   @param p_aad
   Additional authenticated data. This data is authenticated but not encrypted.

   @param aad_len
   Length of additional authenticated data.

   @param mac_len
   Length of authentication tag (in bytes).

   @param data_len
   This parameter provides total length of encrypted / decrypted data.

   @param p_src
   Pointer to the input data to be (en/de)crypted.

   @param src_len
   Length in bytes of the data to be (en/de)crypted.

   @param p_dst
   Pointer to the buffer that receives the resulting text.
   Unlike in other functions, it is not allowed for p_dst to be p_src.
   This is because src and dst are of different size.

   @param p_dst_len
   Pointer to the length of the destination buffer reserved to receive the
   result of the (en/de)cryption. This is an input-output parameter. If the
   value it points to is less than what is required by the implementation,
   then the API fails with return code SFZCRYPTO_BUFFER_TOO_SMALL and sets
   the value to the minimum required dst buffer length to succeed. On a
   successfull return, the value is set to the exact length of the returned
   output.

   For AES-CCM, the ciphertext_len == plaintext_len + mac_len.

   @param direction
   TRUE for encryption and FALSE for decryption.

   @param init
   Initialize: indicate this is the first segment.

   @param finish
   Finish: indicate that this is the last segment.

   @return
   One of the SfzCryptoStatus values.
   SFZCRYPTO_SIGNATURE_CHECK_FAILED will be returned when there is mac
   mismatch in decryption. In this case, the contents of p_dst and p_dst_len
   are undefined.

*****************************************************************************/
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
        bool finish);

/*
    AES Key Wrap Algorithm for wrapping and unwrapping data.

    AES Key Wrap plaindata to produce the result at wrappeddata, OR
    AES Key Wrap unwraps wrappeddata to produce the result at plaindata.
    The AES Wrap algorithm used is specified in the RFC 3394.
    The AES Key Wrap algorithm can be used with the AES algorithm or
    if with the Camellia Encryption Algorithm (as per RFC 3657).

    @pre p_ctxt != NULL && p_kek != NULL

    @pre p_kek points to a valid, populated SfzCryptoCipherKey.

    @pre p_wrappeddata points to valid pre-allocated buffer >=
    plain_len+8 bytes.

    @pre p_kek->type == SFZCRYPTO_KEY_AES ||
         p_kek->type == SFZCRYPTO_KEY_CAMELLIA

    @pre (plain_len >= 16) and (plain_len%8 == 0)

    *Note:* wrappeddata is 8 bytes longer in the front than plaindata so
    as to contain the integrity value.

    @param sfzcryptoctx_p
    Pointer to a pre-allocated and setup SfzCryptoContext object..

    @param ctxt_p
    Cipher Context for AES/Camellia processing.

    @param kek_p
    Contains the kek to use. See SfzCryptoCipherKey.

    @param src_p
    Pointer to the data to wrap/unwrap from.

    @param src_len
    The length of source in bytes.

    @param dst_p
    Pointer to the data to wrap/unwrap to.

    @param dst_len_p
    Pointed value indicates size of target buffer.
    The size will be updated to express how many bytes were actually used.

    @param direction
    TRUE for wrap, FALSE for unwrap.

    @param initial_value_p
    Pointer to eight bytes of character data.
    These need to be provided if alternative initial values is needed.
    If not supplied (left NULL), default from RFC 3394 will be used.

    @return
    One of the SfzCryptoStatus values.

*****************************************************************************/
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
        const uint8_t * initial_value_p);


#endif /* Include Guard */

/* end of file sfzcryptoapi_sym.h */
