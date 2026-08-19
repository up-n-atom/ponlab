/* sfzcryptoapi_enum.h
 *
 * The Cryptographic Abstraction Layer API: Shared Enumerations.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_ENUM_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_ENUM_H

#include "public_defs.h"            // uint8_t, uint32_t, etc.

typedef enum
{
    /* Hash: MD5. */
    SFZCRYPTO_ALGO_HASH_MD5 = 0,
    /* Hash: SHA1. */
    SFZCRYPTO_ALGO_HASH_SHA160,
    /* Hash: SHA-224. */
    SFZCRYPTO_ALGO_HASH_SHA224,
    /* Hash: SHA-256. */
    SFZCRYPTO_ALGO_HASH_SHA256,
    /* Hash: SHA-384. */
    SFZCRYPTO_ALGO_HASH_SHA384,
    /* Hash: SHA-512. */
    SFZCRYPTO_ALGO_HASH_SHA512,

    SFZCRYPTO_NUM_ALGOS_HASH        // must be last
} SfzCryptoHashAlgo;


typedef enum
{
    SFZCRYPTO_KEY_AES = 0,
    SFZCRYPTO_KEY_DES,
    SFZCRYPTO_KEY_TRIPLE_DES,
    SFZCRYPTO_KEY_ARCFOUR,
    SFZCRYPTO_KEY_AES_SIV,
    SFZCRYPTO_KEY_HMAC,
    SFZCRYPTO_KEY_CAMELLIA,
    SFZCRYPTO_KEY_C2,
    SFZCRYPTO_KEY_MULTI2,

    SFZCRYPTO_NUM_SYM_KEY_TYPES      // must be last
} SfzCryptoSymKeyType;


typedef enum
{
    /* Encryption Scheme : RSAES-PKCS1-V1_5. */
    SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1 = 0,
    /* Encryption Scheme : RSAES-OAEP(SHA1). */
    SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA1,
    /* Encryption Scheme : RSAES-OAEP(SHA2-256). */
    SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA256,
    /* Signature Scheme : RSASSA-PSS(undefined-hash) */
    SFZCRYPTO_ALGO_ASYMM_RSA_PSS,
    /* Encryption Primitive : RSAEP / RSADP. */
    SFZCRYPTO_ALGO_ASYMM_RSA_RAW,
    /* Signature Scheme : RSASSA-PKCS1-V1_5(MD5). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_MD5,
    /* Signature Scheme : RSASSA-PSS(MD5). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PSS_MD5,
    /* Signature Scheme : RSASSA-PKCS1-V1_5(SHA1).*/
    SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA1,
    /* Signature Scheme : RSASSA-PKCS1-V1_5(SHA2-224). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA224,
    /* Signature Scheme : RSASSA-PKCS1-V1_5(SHA2-256). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA256,
    /* Signature Scheme : RSASSA-PSS(SHA1). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA1,
    /* Signature Scheme : RSASSA-PSS(SHA2-224). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA224,
    /* Signature Scheme : RSASSA-PSS(SHA2-256). */
    SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA256,
    /* Signature Scheme : DSA(SHA1). */
    SFZCRYPTO_ALGO_ASYMM_DSA_WITH_SHA1,
    /* Signature Scheme : ECDSA(SHA1). */
    SFZCRYPTO_ALGO_ASYMM_ECDSA_WITH_SHA1,
    /* Signature Scheme : ECDSA(SHA224). */
    SFZCRYPTO_ALGO_ASYMM_ECDSA_WITH_SHA224,
    /* Signature Scheme : ECDSA(SHA256). */
    SFZCRYPTO_ALGO_ASYMM_ECDSA_WITH_SHA256,
    /* Key Exchange Scheme : DH. */
    SFZCRYPTO_ALGO_ASYMM_DH,
    /* Key Exchange Scheme : ECDH. */
    SFZCRYPTO_ALGO_ASYMM_ECDH,

    SFZCRYPTO_NUM_ALGOS_ASYMCRYPTO      // must be last
} SfzCryptoAlgoAsym;

/* Block cipher modes for multi-block data. */
typedef enum
{
    /* Mode: ECB. */
    SFZCRYPTO_MODE_ECB = 0,
    /* Mode: CBC. */
    SFZCRYPTO_MODE_CBC,
    /* Mode: CTR. */
    SFZCRYPTO_MODE_CTR,
    /* Mode: ICM. */
    SFZCRYPTO_MODE_ICM,
    /* Mode: AESF8. */
    SFZCRYPTO_MODE_F8,
    /* Mode: CFB. */
    SFZCRYPTO_MODE_CFB,
    /* Mode: OFB. */
    SFZCRYPTO_MODE_OFB,
    /* Mode: C-CBC. */
    SFZCRYPTO_MODE_C_CBC,

    /* Mode/MAC: CMAC. */
    SFZCRYPTO_MODE_CMAC,
    /* Mode/MAC: CBC-MAC. */
    SFZCRYPTO_MODE_CBCMAC,
    /* Mode/MAC: S2V-CMAC. */
    SFZCRYPTO_MODE_S2V_CMAC,
    /* Mode: C2_H */
    SFZCRYPTO_MODE_C2_H,

    SFZCRYPTO_NUM_MODES_SYMCRYPTO       // must be last
}
SfzCryptoModeType;


/**
  Values to indicate where an item is actually stored (between operations)
  or to request a change of location (prior to an operation). In the latter
  case, the value is updated to one of the former during the operation.

  note: BIT_0 indicates current location
        BIT_1 is set to request a switch to the other location

  Example: when calling a function with the SfzCryptoLocation set to
           SFZ_TO_ASSET, it will be set to SFZ_IN_ASSET when the operation
           succeeded, otherwise it will be SFZ_IN_CONTEXT.
*****************************************************************************/
typedef enum
{
    SFZ_IN_CONTEXT = 0,
    SFZ_IN_ASSET = 1,
    SFZ_TO_ASSET = 2,
    SFZ_FROM_ASSET = 3
} SfzCryptoLocation;


/* Cipher operation. */
typedef enum _SfzCipherOp
{
    SFZ_DECRYPT = 0,    /* Decrypt. */
    SFZ_ENCRYPT = 1,    /* Encrypt. */
    SFZ_UNWRAP = 0,     /* Unwrap. */
    SFZ_WRAP = 1        /* Wrap. */
} SfzCipherOp;


// Command types
typedef enum
{
    /* Asymmetric crypto: sig generation. */
    SFZCRYPTO_CMD_SIG_GEN,
    /* Asymmetric crypto: sig verification. */
    SFZCRYPTO_CMD_SIG_VERIFY,
    /* Asymmetric crypto: RSA encryption. */
    SFZCRYPTO_CMD_RSA_ENCRYPT,
    /* Asymmetric crypto: RSA decryption. */
    SFZCRYPTO_CMD_RSA_DECRYPT,
    /* Asymmetric crypto: key generation. */
    SFZCRYPTO_CMD_KEY_GEN
}
SfzCryptoCmdType;

#endif /* Include Guard */

/* end of file sfzcryptoapi_enum.h */
