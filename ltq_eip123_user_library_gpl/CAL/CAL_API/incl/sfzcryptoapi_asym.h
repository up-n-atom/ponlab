/* sfzcryptoapi_asym.h
 *
 * The Cryptographic Abstraction Layer API: Asymmetric Cryptography.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_ASYM_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_ASYM_H

#include "public_defs.h"
#include "sfzcryptoapi_result.h"        // SfzCryptoStatus
#include "sfzcryptoapi_init.h"          // SfzCryptoContext
#include "sfzcryptoapi_enum.h"

// for ECP operations for the minimum allowed modulus length
#define SFZCRYPTO_ECP_MIN_BITS          128

// for ECP operations for the maximum size of buffer in bits
#define SFZCRYPTO_ECP_MAX_BITS          521

// for RSA operations for the minimum allowed modulus length
#define SFZCRYPTO_RSA_MIN_BITS          512

// for RSA operations for the maximum size of buffer in bits
#define SFZCRYPTO_RSA_MAX_BITS          4096

// for DH operations for the minimum allowed prime length
#define SFZCRYPTO_DH_MIN_BITS           512

// for DH operations for the maximum size of buffer in bits
#define SFZCRYPTO_DH_MAX_BITS           4096

// for DSA operations for the minimum allowed prime length
#define SFZCRYPTO_DSA_MIN_BITS          512

// for DSA operations for the maximum size of buffer in bits
#define SFZCRYPTO_DSA_MAX_BITS          3072

// for DSA operations for the minimum allowed prime length
#define SFZCRYPTO_DSA_SUBPRIME_MIN_BITS 160

// for DSA operations for the maximum size of buffer in bits
#define SFZCRYPTO_DSA_SUBPRIME_MAX_BITS 256


// for ECP operations for the maximum size of buffer in bytes
#define SFZCRYPTO_ECP_BYTES             ((SFZCRYPTO_ECP_MAX_BITS + 7)/8)

// for ECP operations for the maximum size of buffer in words
#define SFZCRYPTO_ECP_WORDS             ((SFZCRYPTO_ECP_BYTES + 3)/4)

// for ECDH operations for the maximum size of buffer in bytes
// (same as the ECP maximum)
#define SFZCRYPTO_ECDH_BYTES            SFZCRYPTO_ECP_BYTES

// for ECDH operations for the maximum size of buffer in words
// (same as the ECP maximum).
#define SFZCRYPTO_ECDH_WORDS            SFZCRYPTO_ECP_WORDS

// for RSA operations for the maximum size of buffer in bytes
#define SFZCRYPTO_RSA_BYTES             ((SFZCRYPTO_RSA_MAX_BITS + 7)/8)

// for RSA operations for the maximum size of buffer in words
#define SFZCRYPTO_RSA_WORDS             ((SFZCRYPTO_RSA_BYTES + 3)/4)

// for DH operations for the maximum size of buffer in bytes
#define SFZCRYPTO_DH_BYTES              ((SFZCRYPTO_DH_MAX_BITS + 7)/8)

// for DH operations for the maximum size of buffer in words
#define SFZCRYPTO_DH_WORDS              ((SFZCRYPTO_DH_BYTES + 3)/4)

// for DSA operations for the maximum size of buffer in bytes
#define SFZCRYPTO_DSA_BYTES             ((SFZCRYPTO_DSA_MAX_BITS + 7)/8)

// for DSA operations for the maximum size of buffer in words
#define SFZCRYPTO_DSA_WORDS             ((SFZCRYPTO_DSA_BYTES + 3)/4)

// for DSA operations for the maximum size of buffer in bytes
#define SFZCRYPTO_DSA_SUBPRIME_BYTES    ((SFZCRYPTO_DSA_SUBPRIME_MAX_BITS+7)/8)

// for DSA operations for the maximum size of buffer in words
#define SFZCRYPTO_DSA_SUBPRIME_WORDS    ((SFZCRYPTO_DSA_SUBPRIME_BYTES + 3)/4)

// maximum buffer size for HASH operation in bytes
// MD5 utilizes only 16 bytes out of this buffer
// SHA2 operations will utilize the maximum buffer size
#define SFZCRYPTO_MAX_DIGEST_LENGTH          64

// salt length is decided based on the hash algorithm utilized
// so in case of MD5, the salt length will be only 16 bytes
// the 64 bytes in case of sha-512
#define SFZCRYPTO_MAX_SALT_LENGTH            64

// counter size in bytes utilized in PSS operation
#define SFZCRYPTO_PSS_CTR_SIZE               4

// size in bytes used for tag and padding in PKCS #1 padding operation
#define SFZCRYPTO_PKCS1_FIX_PAD              11

// value of tag used in PKCS #1 padding when done for sign/verification
#define SFZCRYPTO_PKCS1_SIGN_VERIFY_TAG      1

// value of tag used in PKCS #1 padding when done for encryption/decryption
#define SFZCRYPTO_PKCS1_ENCRYPT_DECRYPT_TAG  2

/* Big number structure. */
typedef struct
{
    uint8_t *p_num;                     /* Data part. */
    uint32_t byteLen;                   /* Data length in bytes. */
} SfzCryptoBigInt;

/* Curve point structure. */
typedef struct
{
    SfzCryptoBigInt x_cord;             /* Represents the X coordinate. */
    SfzCryptoBigInt y_cord;             /* Represents the Y coordinate. */
} SfzCryptoECCPoint;                    /* ECC point. */

/* ECP domain. */
typedef struct
{
    SfzCryptoBigInt modulus;            /* Modulus. */
    SfzCryptoBigInt a;                  /* Constant a. */
    SfzCryptoBigInt b;                  /* Constant b. */
    SfzCryptoBigInt g_order;            /* Curve order. */
    SfzCryptoECCPoint G;                /* Base point of the curve. */
} SfzCryptoECPDomainParam;

/* DH domain parameters */
typedef struct
{
    SfzCryptoBigInt prime_p;            /* Prime p. */
    SfzCryptoBigInt base_g;             /* Base g. */
} SfzCryptoDHDomainParam;

/* DSA domain parameters */
typedef struct
{
    SfzCryptoBigInt prime_p;
    SfzCryptoBigInt sub_prime_q;
    SfzCryptoBigInt base_g;
    //uint32_t prime_len_in_bits;
} SfzCryptoDSADomainParam;

// Signature
typedef struct
{
    SfzCryptoBigInt r;                  /* Signature component r. */
    SfzCryptoBigInt s;                  /* Signature component s. */
} SfzCryptoSign;


/* Structure used for all public-key algorithms to pass the
    public and private key part in the specified algorithm. */
typedef struct
{
    SfzCryptoCmdType cmd_type;          /* CMD type. */
    SfzCryptoAlgoAsym algo_type;        /* Algorithm type. */
    uint32_t mod_bits;                  /* Modulus length in bits. */

    union
    {
        struct
        {
            SfzCryptoBigInt prime_p;        /* p */
            SfzCryptoBigInt subPrime_q;     /* q */
            SfzCryptoBigInt base_g;         /* g */
            SfzCryptoBigInt pubkey_y;       /* y */
        }
        dsaPubKey;

        struct
        {
            SfzCryptoBigInt prime_p;        /* p */
            SfzCryptoBigInt subPrime_q;     /* q */
            SfzCryptoBigInt base_g;         /* g */
            SfzCryptoBigInt privkey_x;      /* x */
        }
        dsaPrivKey;

        struct
        {
            SfzCryptoECPDomainParam domainParam;
            SfzCryptoECCPoint Q;
        }
        ecPubKey;

        struct
        {
            SfzCryptoECPDomainParam domainParam;
            SfzCryptoBigInt privKey;
        }
        ecPrivKey;

        struct
        {
            SfzCryptoBigInt modulus;        /* m */
            SfzCryptoBigInt pubexp;         /* e */
            SfzCryptoBigInt privexp;        /* d */
            SfzCryptoBigInt primeP;         /* P */
            SfzCryptoBigInt primeQ;         /* Q */
            SfzCryptoBigInt dmodP;          /* d % P */
            SfzCryptoBigInt dmodQ;          /* d % Q */
            SfzCryptoBigInt cofQinv;        /* cofQinv *p_Q mod m = 1 */
        }
        rsaPrivKey;

        struct
        {
            SfzCryptoBigInt modulus;        /* m */
            SfzCryptoBigInt pubexp;         /* e */
        }
        rsaPubKey;

        struct
        {
            SfzCryptoBigInt prime_p;        /* p */
            SfzCryptoBigInt base_g;         /* g */
            SfzCryptoBigInt pubkey;         /* y */
        }
        dhPubKey;

        struct
        {
            SfzCryptoBigInt prime_p;        /* p */
            SfzCryptoBigInt base_g;         /* g */
            SfzCryptoBigInt privkey;        /* x  */
        }
        dhPrivKey;
    }
    Key;
}
SfzCryptoAsymKey;

#define SFZCRYPTO_COMP_A_EQ_B         0x00000001
#define SFZCRYPTO_COMP_A_LT_B         0x00000002
#define SFZCRYPTO_COMP_A_GT_B         0x00000004

/*
   Generate signature using the ECDSA algorithm.

   This function is used for generating the signature on the supplied hashed
   message of specified length (SHA1 hash length is 20 bytes). The
   signature is generated using the ECDSA algorithm. The current
   implementation supports ECDSA with SHA1 only. The bits range  are
   supported buy ECDSA varies from 128 bits to 521 bits. The input/output
   data format is octet string and the first byte at lowest address i.e. at
   index '0' zero in the string would be the most significant byte in the
   string. The current implementation supports ECDSA with SHA1 and
   curves over prime fields only. The bits range are supported by ECDSA
   varies from 128 bits to 521 bits.

   <TABLE>
   INPUT/OUTPUT to ECDSA-Sign      input                     output
   ------------------------------  ------------------------  ---------------
   ECDSA                           sha-hash length(20bytes)  2*curve modulus
   </TABLE>

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the private key of the
   signer.

   @param p_signature
   The generated signature on the message.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_ecdsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);


/*
   Verify the ECDSA signature.

   This function is used for verifying the signature on the supplied hashed
   of given length. The signature verification is done using the ECDSA
   algorithm. The input/output data format is octet string and the first
   byte at lowest address i.e. at index '0' zero in the string would be
   the most significant byte in the string. The current implementation
   supports ECDSA with SHA1 curves over prime fields only. The bits range
   are supported by ECDSA varies from 128 bits to 521 bits.

   <TABLE>
   INPUT/OUTPUT to ECDSA-Verify  input            output
   ----------------------------  ---------------  ------
   ECDSA                         2*curve modulus  N/A
   </TABLE>

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the public key of signer.

   @param p_signature
   Signature for verification.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_ecdsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);


/*
   Generate a signature using the DSA algorithm.

   This function is used for generating the signature on the supplied
   hashed message of given length. The signature is generated using the
   DSA algorithm. The input/output data format is octet string and the first
   byte at lowest address i.e. at index '0' zero in the string would be
   the most significant byte in the string.

   1) The standard followed is "FIPS PUB 186-2, 2000 January 27"
   2) Implements DSA with SHA1
   3) Size of the prime modulus range from 512 bits to 1024 bits in multiple
   of 64 bits.

   <TABLE>
   INPUT/OUTPUT to DSA-Sign  input                      output
   ------------------------  -------------------------  --------------------
   DSA                       sha1-hash length(20bytes)  2*(sha1-hash length)
   </TABLE>

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the private key of the signer.

   @param p_signature
   The generated signature on the message.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.


   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_dsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

/*
   Verify the DSA signature.

   This functions used for verifying the signature on the supplied
   hashed message of specified length. The signature verification is
   done using the DSA algorithm. The input/output data format is
   octet string and the first byte at lowest address i.e. at index '0'
   zero in the string would be the most significant byte in the
   string.

   1) The standard followed is "FIPS PUB 186-2, 2000 January 27"
   2) Implements DSA with SHA1
   3) Size of the prime modulus range from 512 bits to 1024 bits in multiple
   of 64 bits.

   <TABLE>
   INPUT/OUTPUT to DSA-Verify   input                      output
   ---------------------------  -------------------------  ------
   DSA                          sha1-hash length(20bytes)  N/A
   </TABLE>

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the public key of signer.

   @param p_signature
   Signature for verification.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.


   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_dsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);


/*
   Encrypt plaintext using various RSA mechanisms.

   This function is used for encrypting plaintext of specified length. The
   encryption is done as per the mechanism supplied. Currently this
   supports raw RSA encryption, and RSA encryption with PKCS #1 padding
   scheme. The padding schemes can be selected with the algo_type
   value defined in the sfzcryptotypes.h header file. The pkcs#1
   standard has been followed to implement the algorithm. RSA support upto
   4096 bits operations. The plain text data length can be as follow
   and depends on padding schemes used.

   <TABLE>
   Padding Scheme             Input                     Output
   -------------------------  ------------------------  -----------------
   OAEP                       <= m-2-2hLen                m
   PKCS#1                     <= m-11                     m
   RSA Raw                       m                        m
   </TABLE>

   Where m is modulus length in bytes. hLen is the hash length used in the
   operation.


   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_enctx.

   @pre the result (output) buffers should be minimum of one byte and maximum
   (4096/8) bytes for output, it can vary based on modulus length.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_enctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_enctx
   Pointer to SfzCryptoAsymKey to pass the public key of receiver.

   The algorithm can be as follow:

   <CODE>
   p_enctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1
   p_enctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA1
   p_enctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA256,
   p_enctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_RAW
   </CODE>

   The command field can be populated as:

   <CODE>
   p_enctx->cmd_type  = SFZCRYPTO_CMD_RSA_ENCRYPT

   p_sigctx->mod_bits  = 1024 (need to initialize this with modulus length
                               in bits).
   </CODE>

   @param p_plaintext
   With this parameter the user can pass the plaintext string which is
   to be encrypted using RSA mechanisms.

   @param p_ciphertext
   Generated ciphertext of the plaintext.

   @return
   One of the SfzCryptoStatus values.

*/

SfzCryptoStatus
sfzcrypto_rsa_encrypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_enctx,
        SfzCryptoBigInt * const p_plaintext,
        SfzCryptoBigInt * const p_ciphertext);

/*
   Decrypt ciphertext using various RSA mechanisms.

   This API is used for decrypting ciphertext of specified length. The
   decryption is done as per the mechanism supplied. Currently this
   supports raw RSA decryption, and RSA encryption with PKCS #1 padding
   scheme. The padding schemes can be selected with the algo_type
   value defined in the sfzcryptotypes.h header file. The input/output
   data format is octet string and the first byte at lowest address i.e.
   at index '0' zero in the string would be the most significant byte in
   the string. The pkcs#1 standard has been followed to implement the
   algorithm. We support upto 4096 bits RSA operations.

   <TABLE>
   Padding Scheme              Input (Bytes)          Output (Bytes)
   --------------------------  ---------------------  -------------------
   OAEP                            m                  <= m-2-2hLen
   PKCS#1                          m                  <= m-11
   RSA Raw                         m                     m
   </TABLE>

   Where m is modulus length in bytes. hLen is the hash length used in the
   operation.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_dectx.

   @pre the result (output) buffers should be minimum of one byte and maximum
   (4096/8) bytes for output, it can vary based on modulus length.

   The algorithm and command could be as follow:

   <CODE>
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA1
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA256,
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_RAW

   p_dectx->cmd_type  = SFZCRYPTO_CMD_RSA_DECRYPT
   p_sigctx->mod_bits = 1024 (need to initialize this with modulus length
                              in bits).
   </CODE>

   Note: Internally this API uses CRT and Non-CRT operation the
   decision is taken based on the parameters passed and is as given
   below:

   <CODE>
   ----------------------------------------------------------------------------
   Parameters:                                    |      Operation (CRT)
   ----------------------------------------------------------------------------
                                                  |
   p_dectx->Key.rsaPrivKey.modulus.p_num = p_xxx  |
   p_dectx->Key.rsaPrivKey.modulus.byteLen = xxx; |
                                                  |
   p_dectx->Key.rsaPrivKey.pubexp.p_num = p_xxx;  | (Not needed, don't care)
   p_dectx->Key.rsaPrivKey.pubexp.byteLen = xxx;  |
                                                  |
   p_dectx->Key.rsaPrivKey.privexp.p_num = p_xxx; |
   p_dectx->Key.rsaPrivKey.privexp.byteLen = xxx; |
   ---------------------------------------------------
   p_dectx->Key.rsaPrivKey.primeP.p_num = p_xxx;  | CRT
   p_dectx->Key.rsaPrivKey.primeP.byteLen = xxx;  |
                                                  |(Detecting that all
   p_dectx->Key.rsaPrivKey.primeQ.p_num = p_xxx;  |parameters to perform
   p_dectx->Key.rsaPrivKey.primeQ.byteLen = xxx;  |the CRT operation are
                                                  |available so internall
   p_dectx->Key.rsaPrivKey.dmodP.p_num = p_xxx;   |operation will performed
   p_dectx->Key.rsaPrivKey.dmodP.byteLen = xxx;   |using CRT operation)
                                                  |
   p_dectx->Key.rsaPrivKey.dmodQ.p_num = p_xxx;   |
   p_dectx->Key.rsaPrivKey.dmodQ.byteLen = xxx;   |
                                                  |
   p_dectx->Key.rsaPrivKey.cofQinv.p_num = p_xxx; |
   p_dectx->Key.rsaPrivKey.cofQinv.byteLen = xxx; |
   ----------------------------------------------------------------------------

   ----------------------------------------------------------------------------
   Parameters:                                    |  Operation ( non-CRT)
   ----------------------------------------------------------------------------
                                                  |
   p_dectx->Key.rsaPrivKey.modulus.p_num = p_xxx  |
   p_dectx->Key.rsaPrivKey.modulus.byteLen = xxx; |
                                                  |Operation will use these
   p_dectx->Key.rsaPrivKey.pubexp.p_num = p_xxx;  |parameters as to use CRT
   p_dectx->Key.rsaPrivKey.pubexp.byteLen = xxx;  |operation internally there
                                                  |is no sufficient input
                                                  |so will switch tonon-CRT
   p_dectx->Key.rsaPrivKey.privexp.p_num = p_xxx; |operation.
   p_dectx->Key.rsaPrivKey.privexp.byteLen = xxx; |
                                                  |
   ---------------------------------------------------
   (If below args any one is set to NULL)         |
                                                  |
   p_dectx->Key.rsaPrivKey.primeP.p_num = NULL;   |Detecting NULL it will
   p_dectx->Key.rsaPrivKey.primeP.byteLen = xxx;  |switch to non-CRT operation.
                                                  |
   p_dectx->Key.rsaPrivKey.primeQ.p_num = p_xxx;  |
   p_dectx->Key.rsaPrivKey.primeQ.byteLen = xxx;  |
                                                  |
   p_dectx->Key.rsaPrivKey.dmodP.p_num = p_xxx;   |
   p_dectx->Key.rsaPrivKey.dmodP.byteLen = xxx;   |
                                                  |
   p_dectx->Key.rsaPrivKey.dmodQ.p_num = p_xxx;   |
   p_dectx->Key.rsaPrivKey.dmodQ.byteLen = xxx;   |
                                                  |
   p_dectx->Key.rsaPrivKey.cofQinv.p_num = p_xxx; |
   p_dectx->Key.rsaPrivKey.cofQinv.byteLen = xxx; |
   ----------------------------------------------------------------------------
   </CODE>

    Where: p_xxx is valid buffer pointer and xxx is valid length in bytes.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dectx
   Pointer to SfzCryptoAsymKey to pass the private key of the receiver.

   The algorithm can be as follow:

   <CODE>
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA1
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_OAEP_WITH_MGF1_SHA256,
   p_dectx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_RAW
   </CODE>

   The command field should be populated as:

   <CODE>
   p_dectx->cmd_type  = SFZCRYPTO_CMD_RSA_DECRYPT

   p_dectx->mod_bits = 1024 (need to initialize this with modulus length
                             in bits).
   </CODE>

   @param p_ciphertext
   Using this parameter the user can pass the ciphertext which is to
   be decrypted using RSA mechanisms. The caller need not to set the
   ciphertext length field, it will be populated internally.

   @param p_plaintext
   Decrypted plaintext.

   @return
   One of the SfzCryptoStatus values.

*/

SfzCryptoStatus
sfzcrypto_rsa_decrypt(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dectx,
        SfzCryptoBigInt  * const p_ciphertext,
        SfzCryptoBigInt  * const p_plaintext);


/*
   Generate a signature using the RSA algorithm.

   This API is used for generating the signature on the supplied
   message of specified length. The signature is generated using the
   RSA algorithm and supports the PKCS #1 and PSS padding schemes. The
   padding schemes can be selected with the algo_type value defined in
   the sfzcryptotypes.h header file. The input/output data format is octet
   string and the first byte at lowest address i.e. at index '0' zero in the
   string would be the most significant byte in the string. The pkcs#1
   standard has been followed to implement the algorithm.

   <TABLE>
   Padding Scheme                Input                  Output
   ----------------------------  ---------------------  -----------------
   PSS                            hLen                  m
   PKCS#1                        <= m-11                m
   </TABLE>

   Where m is modulus length in bytes. hLen is the hash length used in the
   operation.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @pre the result (output) buffers should be minimum of one byte and maximum
   (4096/8) bytes for output, it can vary based on modulus length. The
   output will not contain any padding zero on left side.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the private key of the signer.

   The algorithm can be any one as follow:

   <CODE>
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA256
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA1
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA224
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_MD5

   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA1
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA256
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA224
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_MD5
   </CODE>

   The command field should be populated as:

   <CODE>
   p_sigctx->cmd_type  = SFZCRYPTO_CMD_SIG_GEN

   p_sigctx->mod_bits  = 1024 (need to initialize this with modulus length
                               in bits).
   </CODE>

   Note: Internally this API uses CRT and Non-CRT operation the decision is
   taken based on the parameters passed and is as given below:

   <CODE>
   ----------------------------------------------------------------------------
   Parameters:                                     |      Operations
   ----------------------------------------------------------------------------
   |
   p_sigctx->Key.rsaPrivKey.modulus.p_num = p_xxx  |
   p_sigctx->Key.rsaPrivKey.modulus.byteLen = xxx; |
   |
   p_sigctx->Key.rsaPrivKey.pubexp.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.pubexp.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.privexp.p_num = p_xxx; |
   p_sigctx->Key.rsaPrivKey.privexp.byteLen = xxx; |
   |
   p_sigctx->Key.rsaPrivKey.primeP.p_num = p_xxx;  |      CRT
   p_sigctx->Key.rsaPrivKey.primeP.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.primeQ.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.primeQ.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.dmodP.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodP.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.dmodQ.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodQ.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.cofQinv.p_num = p_xxx; |
   p_sigctx->Key.rsaPrivKey.cofQinv.byteLen = xxx; |
   ----------------------------------------------------------------------------

   ----------------------------------------------------------------------------
   Parameters:                                     |      Operations
   ----------------------------------------------------------------------------
   |
   p_sigctx->Key.rsaPrivKey.modulus.p_num = NULL   |
   p_sigctx->Key.rsaPrivKey.modulus.byteLen = 0;   |
   |
   p_sigctx->Key.rsaPrivKey.pubexp.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.pubexp.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.privexp.p_num = NULL;  |
   p_sigctx->Key.rsaPrivKey.privexp.byteLen = 0;   |
   |
   p_sigctx->Key.rsaPrivKey.primeP.p_num = p_xxx;  |      CRT
   p_sigctx->Key.rsaPrivKey.primeP.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.primeQ.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.primeQ.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.dmodP.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodP.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.dmodQ.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodQ.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.cofQinv.p_num = p_xxx; |
   p_sigctx->Key.rsaPrivKey.cofQinv.byteLen = xxx; |
   ----------------------------------------------------------------------------

   ----------------------------------------------------------------------------
   Parameters:                                     |      Operations
   ----------------------------------------------------------------------------
   |
   p_sigctx->Key.rsaPrivKey.modulus.p_num = p_xxx  |
   p_sigctx->Key.rsaPrivKey.modulus.byteLen = xxx; |
   |
   p_sigctx->Key.rsaPrivKey.pubexp.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.pubexp.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.privexp.p_num = p_xxx; |
   p_sigctx->Key.rsaPrivKey.privexp.byteLen = xxx; |
   |
   (If below args any one is set to NULL)         |
   |
   p_sigctx->Key.rsaPrivKey.primeP.p_num = NULL;   |      non-CRT
   p_sigctx->Key.rsaPrivKey.primeP.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.primeQ.p_num = p_xxx;  |
   p_sigctx->Key.rsaPrivKey.primeQ.byteLen = xxx;  |
   |
   p_sigctx->Key.rsaPrivKey.dmodP.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodP.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.dmodQ.p_num = p_xxx;   |
   p_sigctx->Key.rsaPrivKey.dmodQ.byteLen = xxx;   |
   |
   p_sigctx->Key.rsaPrivKey.cofQinv.p_num = p_xxx; |
   p_sigctx->Key.rsaPrivKey.cofQinv.byteLen = xxx; |
   ----------------------------------------------------------------------------
   </CODE>

   Where: p_xxx is valid buffer pointer and xxx is valid length in bytes.

   @param p_signature
   The generated signature on the message.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_rsa_sign(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);


/*
   Verify the RSA signature.

   This API is used for verifying the signature of the supplied message
   of specified length. The signature verification is done using the
   RSA algorithm and supports the PKCS #1 and PSS padding schemes. The
   padding scheme can be selected with the algo_type value defined in
   the sfzcryptotypes.h header file. The input/output data format is octet
   string and the first byte at lowest address i.e. at index '0' zero in the
   string would be the most significant byte in the string. The pkcs#1
   standard has been followed to implement the algorithm.

   <TABLE>
   Padding Scheme                Input                  Output
   ----------------------------  ---------------------  -----------------
   PSS                             m                    N/A
   PKCS#1                          m                    N/A
   </TABLE>

   Where m is modulus length in bytes. hLen is the hash length used in the
   operation.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_sigctx.

   @pre the result (output) buffers should be minimum of one byte and maximum
   (4096/8) bytes for output, it can vary based on modulus length.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_sigctx
   Pointer to SfzCryptoAsymKey to pass the public key of signer.

   The algorithm can be any one as follow:

   <CODE>
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA256
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA1
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_SHA224
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PKCS1_MD5

   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA1
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA256
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_SHA224
   p_sigctx->algo_type = SFZCRYPTO_ALGO_ASYMM_RSA_PSS_MD5
   </CODE>

   The command field should be populated as:

   <CODE>
   p_sigctx->cmd_type = SFZCRYPTO_CMD_SIG_VERIFY
   p_sigctx->mod_bits = 1024 (need to initialize this with modulus length
                              in bits).
   </CODE>

   @param p_signature
   The signature to be verified.

   @param p_hash_msg
   Digest of the message on which the signature need to calculate.

   @param hash_msglen
   The length of the supplied message's digest in bytes.

   @return
   One of SfzCryptoStatus.

*/
SfzCryptoStatus
sfzcrypto_rsa_verify(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

/*
   Generate the public part of the sender (my) using the ECDH scheme.

   Generates the public part of the sender (my) using the sender's
   private key. This public part is sent to the other party to generate
   the shared secret using this public part. The input/output data format is
   octet string and the first byte at lowest address i.e. at index '0' zero
   in the string would be the most significant byte in the string.

   <TABLE>
   INPUT/OUTPUT to ECDH-Pubkey  Input                  Output
   ---------------------------  ---------------------  ------------------
   ECDH                         Curve Mod length keys  2*Curve Mod length
   </TABLE>

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_dhctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dhctx
   Pointer to SfzCryptoAsymKey to pass the ECDH parameters and valid
   private key pointer to hold the generated private key. The private
   key will be generated inside the sfzcrypto_ecdh_publicpart_gen() function.

   The private key should follow the following condition:
   Choose an integer s (1<= x < p) randomly. Where p is curve modulus.

   @param p_mypubpart
   Using this parameter the user gets the generated public part of the
   sender which is used to generate the shared secret.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_ecdh_publicpart_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_mypubpart);

/*
   Generate the shared secret using other party's public part using the
   ECDH scheme.

   Generates the shared secret using the other party's public part and
   the receiver's private key. The input/output data format is octet string
   and the first byte at lowest address i.e. at index '0' zero in the string
   would be the most significant byte in the string.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_dhctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dhctx
   Pointer to SfzCryptoAsymKey to pass the private key of receiver.

   @param p_otherpubpart
   Using this parameter the user passes the other party's public part
   which is used to generate the this shared secret.

   @param p_sharedsecret
   Using this parameter the user gets the generated shared secret.

   @param p_sharedsecretlen
   The length of the generated shared secret (expressed in bytes).

   @return
   One of the SfzCryptoStatus values.

   <TABLE>
   INPUT/OUTPUT to ECDH-SharedKKey  Input               Output
   -------------------------------  ------------------  ----------
   ECDH                             2*Curve Mod length  Curve Mod length keys
   </TABLE>

*/
SfzCryptoStatus
sfzcrypto_ecdh_sharedsecret_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

/*
   Generate the public part of the sender (my) using the DH scheme.

   Generates the public part of the sender (my) using sender's private
   key. This public part is sent to other party which generates the
   shared secret using this public part. The input/output data format is octet
   string and the first byte at lowest address i.e. at index '0' zero in the
   string would be the most significant byte in the string.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_dhctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dhctx
   Pointer to SfzCryptoAsymKey to pass the DH parameters and valid
   private key pointer to hold the generated private key. The private
   key will be generated inside the sfzcrypto_dh_publicpart_gen() function.

   The private key should follow the following condition:

   <CODE>
   7.1 Private-value generation

   An integer x, the private value, shall be generated privately and
   randomly. This integer shall satisfy 0 < x < p-1, unless the central
   authority specifies a private-value length l, in which case the
   integer shall satisfy 2^(l-1) <= x < 2^l.
   </CODE>

   (Source: "PKCS #3: Diffie-Hellman Key-Agreement Standard". An RSA
   Laboratories Technical Note Version 1.4 Revised November 1, 1993.)

   @param p_mypubpart
   By using this parameter, the user gets the generated public part of
   the sender which is used to generate the shared secret.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_dh_publicpart_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_mypubpart);


/*
   Generate the shared secret using the peer's public part and the DH
   scheme.

   Generates the shared secret using the peer's public part and the
   receiver's private key. The input/output data format is octet
   string and the first byte at lowest address i.e. at index '0' zero
   in the string would be the most significant byte in the string. The
   pkcs#3 standard has been followed to implement the algorithm.

   @pre all the pointers passed in argument must be valid pointer.

   @pre The key value points to a valid populated in p_dhctx.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dhctx
   Pointer to SfzCryptoAsymKey to pass the private key of the receiver.

   @param p_otherpubpart
   By using this parameter the user passes the other party's public
   part which is used to generate the shared secret.

   @param p_sharedsecret
   Using this parameter the user gets the generated shared secret.

   @param p_sharedsecretlen
   The length of the generated shared secret (expressed in bytes).

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_dh_sharedsecret_gen(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

/*

   Generate Diffie-Hellman domain parameters.

   Diffie-Hellman parameter generation is done as follows:

   - An odd prime p is selected, as well as an integer g, the base,
   which satisfies 0 < g < p.

   - The length of the prime p in octets is the integer k satisfying
   2^(8*(k-1)) <= p < 2^(8*k).

   - The {p, g} values are the DH domain parameter.

   The generated DH parameters are populated in the
   sfzcrypto_dh_domain_param structure.

   The maximum value of primeBits value can be 4096 bits.

   The pkcs#3 standard has been followed to implement the algorithm.

   The input/output data format is octet string and the first byte at lowest
   address i.e. at index '0' zero in the string would be the most significant
   byte in the string.

   @param p_sfzcrypto_ctx
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dh_dom_param
   Pointer to sfzcrypto_dh_domain_param to collect the generated DH
   Domain Parameters.

   @param primeBits
   Used to generate the prime with the given length (in bits).

   @return
   One of SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_gen_dh_domain_param(
        SfzCryptoContext * const p_sfzcrypto_ctx,
        SfzCryptoDHDomainParam * const p_dh_dom_param,
        uint32_t primeBits);


/*
   Generate the DSA domain parameters.

   The DSA makes use of the following parameters:

   1)   p = a prime modulus, where 2^(L-1) <p < 2^L
   for 512 <= L <= 1024 and L is a multiple of 64

   2)   q = a prime divisor of (p - 1), where 2^159 <q < 2^160

   3)   g = h^(p-1)/q mod p, where h is any integer with 1 < h < (p - 1)
   such that h^(p-1)/q mod p > 1 (g has order q mod p)

   Where {p, q, g} are the DSA domain parameters. Here p denotes a
   prime and q denotes a sub-prime.

   @pre all the pointers passed in argument must be valid pointer.

   The input/output data format is octet string and the first byte at lowest
   address i.e. at index '0' zero in the string would be the most significant
   byte in the string.

   @param p_sfzcrypto_ctx
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dsa_dom_param
   Pointer to sfzcrypto_dsa_domain_param. The generated DSA parameters
   are populated in this DSA Domain parameter structure.

   @param primeBits
   Used to generate the prime of the given bit length.

   @param subPrimeBits
   Used to generate the sub-prime of the given sub-prime bit length.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_gen_dsa_domain_param(
        SfzCryptoContext * const p_sfzcrypto_ctx,
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        uint32_t primeBits,
        uint32_t subPrimeBits);


/*
   Generate a DSA key pair.

   The DSA key pair is generated as follows :

   1. x = a randomly or pseudo randomly generated integer
   with 0 < x < q

   2. y = g^x mod p

   Where {x, p, q, & g} comprises the private key of DSA, and {y, p, q, & g}
   comprises the public key of DSA. p,q,g are the dsa domain parameter and
   already generated. The generated keys are populated in sfzcrypto_dsa_pubkey
   & sfzcrypto_dsa_privkey structures.

   The input/output data format is octet string and the first byte at lowest
   address i.e. at index '0' zero in the string would be the most significant
   byte in the string.

   The primeBits value could be any 512 to 1024 bits and multiple of 64.
   The subPrimeBits value will be 160 bits to pass.

   @param p_sfzcrypto_ctx
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_dsa_dom_param
   Pointer to sfzcrypto_dsa_domain_param to pass the DSA domain
   parameters, which are used to generate the DSA key pair.

   @param p_dsa_pubkey
   The generated public key will be populated in this parameter.

   @param p_dsa_priv_key
   The generated private key will be populated in this parameter

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_gen_dsa_key_pair(
        SfzCryptoContext * const p_sfzcrypto_ctx,
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        SfzCryptoBigInt * const p_dsa_pubkey,
        SfzCryptoBigInt * const p_dsa_priv_key);


/*
   Generate an ECDSA key pair.

   The ECDSA key pair is generated as follows:

   An entity 'A's public and private key pair is associated with a
   particular set of elliptic curve domain parameters,
   T = (p; a; b; G; n; h).

   To generate a key pair, entity 'A' does the following:

   1. A random or pseudo-random integer d is selected in the interval
   [1, n-1].

   2. Q = dG is computed.

   3. A's public key is Q; A's private key is d.

   The maximum value of ec_bits_key_len can be 521.

   The input/output data format is octet string and the first byte at lowest
   address i.e. at index '0' zero in the string would be the most significant
   byte in the string.

   @param p_sfzcrypto_ctx
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_ec_dom_param
   Pointer to sfzcrypto_ecdsa_domain_param to pass the ECDSA domain
   parameters, which are used to generate the ECDSA key pair.

   @param p_ecdsa_pub_key
   The generated public key will be populated in this parameter.

   @param p_ecdsa_priv_key
   The generated private key will be populated in this parameter.

   @param ec_bits_key_len
   Used to generate public and private keys.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_gen_ecdsa_key_pair(
        SfzCryptoContext * const p_sfzcrypto_ctx,
        SfzCryptoECPDomainParam * const p_ec_dom_param,
        SfzCryptoECCPoint * const p_ecdsa_pub_key,
        SfzCryptoBigInt * const p_ecdsa_priv_key,
        uint32_t ec_bits_key_len);

/*
   Generate an RSA key pair of given bits mode length.

   The RSA key pair is generated as follows:

   1) Two large random (and distinct) primes p and q are generated,
   each roughly the same size (in range of (rsa_mod_bits + 1)/2 bits.

   2) n = pq and xi = (p - 1)(q - 1) is computed.

   3) A random integer e, 1 < e < xi is selected, so that gcd (e; xi) =
   1.

   4) The extended Euclidean algorithm is used to compute the unique
   integer d, 1 < d < xi, such that e*d = 1 (mod xi).

   5) A's public key is (n, e); A's private key is (n, d).

   The maximum value of rsa_mod_bits can be 4096.

   @pre all the pointers passed in argument must be valid pointer.

   The input/output data format is octet string and the first byte at lowest
   address i.e. at index '0' zero in the string would be the most significant
   byte in the string.

   @param p_sfzcrypto_ctx
   Pointer to a pre-allocated and setup SfzCryptoContext object.

   @param p_rsa_pub_key
   The generated public key will be populated in this parameter.

   @param p_rsa_priv_key
   The generated private key will be populated in this parameter.

   @param rsa_mod_bits
   Used to generate public and private keys.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_gen_rsa_key_pair(
        SfzCryptoContext * const p_sfzcrypto_ctx,
        SfzCryptoAsymKey * const p_rsa_pub_key,
        SfzCryptoAsymKey * const p_rsa_priv_key,
        uint32_t rsa_mod_bits);

#endif /* Include Guard */

/* end of file sfzcryptoapi_asym.h */
