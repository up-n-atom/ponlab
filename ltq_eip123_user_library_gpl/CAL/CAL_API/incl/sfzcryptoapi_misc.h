/* sfzcryptoapi_misc.h
 *
 * The Cryptographic Abstraction Layer APIs: Non-crypto functions.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_MISC_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_MISC_H

#include "public_defs.h"                // uint8_t, uint32_t, etc.
#include "sfzcryptoapi_result.h"        // SfzCryptoStatus
#include "sfzcryptoapi_init.h"          // SfzCryptoContext
#include "sfzcryptoapi_buffers.h"       // SfzCryptoOctet*Ptr
#include "sfzcryptoapi_enum.h"          // _ALGO enums
#include "sfzcryptoapi_asset.h"         // SfzCryptoAssetId

/*----------------------------------------------------------------------------
 * Feature Matrix
 *
 * The feature matrix is a data structure that shows the available features
 * in the CAL implementation.
 * The table is populated when calling sfzcrypto_get_featurematrix().
 *
 * All fields starting with f_ are flag fields. These are set to true when the
 * feature is available.
 *
 * f_rand
 *     When set, random number generation is supported.
 *     See sfzcrypto_rand_data().
 *
 * f_algos_hash[]
 *     Indexed using SFZCRYPTO_ALGO_HASH_*
 *     When set, the specific hash algorithm is supported by
 *     sfzcrypto_hash_data().
 *
 * f_keytypes[]
 *     Indexed using SFZCRYPTO_KEY_*
 *     When set, the key type is supported.
 *     Two of the key types have additional meaning:
 *         SFZCRYPTO_KEY_ARCFOUR: ARCFOUR symmetric crypto is supported
 *                                by sfzcrypto_symm_crypto().
 *         SFZCRYPTO_KEY_HMAC: HMAC is supported (sfzcrypto_hmac_data) using
 *                             any supported Hash algo (see f_algos_hash).
 *
 * f_symm_crypto_modes[key][mode]
 *     Indexed by SFZCRYPTO_KEY_* and SFZCRYPTO_MODE_*
 *     When set, the specific crypto operation is supported by
 *     sfzcrypto_symm_crypto(), for example AES-CBC, 3DES-ECB, AES-F8.
 *     Note: ARC4 support is not reflected by this data structure.
 *
 * f_cipher_mac_modes[key][mode]
 *     Indexed by SFZCRYPTO_KEY_* and SFZCRYPTO_MODE_*
 *     When set, the specific cipher mac operation is supported by
 *     sfzcrypto_cipher_mac(), for example AES-CMAC and AES-S2V_CMAC.
 *
 * f_authcrypt_AES_CCM
 * f_authcrypt_AES_SIV
 *     When set, the specific algorithm is supported by the
 *     sfzcrypto_auth_crypt() function.
 *
 * f_wrap_AES_WRAP
 *     When set, AES-WRAP Key Wrapping algorithm is supported.
 *     The AES-WRAP algorithm is supported for the AES Algorithm if
 *     f_keytypes[SFZCRYPTO_KEY_AES] is true and for
 *     the Camellia Encryption Algorithm if f_keytypes[SFZCRYPTO_KEY_CAMELLIA]
 *     is true.
 *     See sfzcrypto_aes_wrap_unwrap().
 *
 * f_keygen_asym
 *     When set, generation of domain parameters and keypairs is supported for
 *     asymmetric key operations.
 *     See sfzcrypto_gen_*_key_pair and sfzcrypto_gen_dsa_domain_param().
 *
 * f_algos_asymcrypto[]
 *     Indexed by SFZCRYPTO_ALGO_ASYMM_*
 *     When set, the support for specific algorithm is present and can be used
 *     with sfzcrypto_rsa_encrypt() and sfzcrypto_rsa_decrypt().
 *
 * f_algos_sign[]
 *     Indexed by SFZCRYPTO_ALGO_ASYMM_*
 *     When set, the sign/verify operation for the specific asymmetric
 *     algorithm is supported.
 *     See sfzcrypto_*_sign() and sfzcrypto_*_verify().
 *
 * f_algos_key_exchange[]
 *     Indexed by SFZCRYPTO_ALGO_ASYMM_*
 *     When set, key exchange is supported for the specific asymmetric
 *     algorithm. See sfzcrypto_*_gen().
 *
 * f_algos_pad[]
 *     Indexed by SFZCRYPTO_PAD_*.
 *     When set, the specific padding is supported by sfzcrypto_symm_crypto().
 *
 * keyrange_sym[key][range]
 *     Indexed by SFZCRYPTO_KEY_* and SFZCRYPTO_KEYRANGE_INDEX_*
 *     Length range for each supported key type.
 *     Minimum, Maximum and Step size are provided, all in bits.
 *     Example:
 *     AES keys range 128, 192 and 256bits are reported like this:
 *     keyrange_sym[SFZCRYPTO_KEY_AES][SFZCRYPTO_KEYRANGE_INDEX_MIN] = 128
 *     keyrange_sym[SFZCRYPTO_KEY_AES][SFZCRYPTO_KEYRANGE_INDEX_MAX] = 256
 *     keyrange_sym[SFZCRYPTO_KEY_AES][SFZCRYPTO_KEYRANGE_INDEX_STEP] = 64
 *
 * keyrange_asym[algo][range]
 *     Indexed by SFZCRYPTO_ALGO_ASYMM_* and SFZCRYPTO_KEYRANGE_INDEX_*
 *     Length range for each supported key type.
 *     Minimum, Maximum and Step size are provided, all in bits.
 */

// following three are used in the keyrange fields of the feature matrix
/* Index for minimum size. */
#define SFZCRYPTO_KEYRANGE_INDEX_MIN      0
/* Index for maximum size. */
#define SFZCRYPTO_KEYRANGE_INDEX_MAX      1
/* Index for size step. */
#define SFZCRYPTO_KEYRANGE_INDEX_STEP     2

typedef struct
{
    /* Checklist of available random number generation.*/
    bool f_rand;

    /* Checklist of available hash algorithms (SFZCRYPTO_ALGO_HASH_*). */
    bool f_algos_hash[SFZCRYPTO_NUM_ALGOS_HASH];

    /* supported keytypes (SFZCRYPTO_KEY_*) */
    bool f_keytypes[SFZCRYPTO_NUM_SYM_KEY_TYPES];

    /* HMAC is supported when HMAC key type is supported */
    /* HMAC is supported for all Hash algorithms where algos_hash[] == true */

    /* Check table of key types and modes supported by different API functions. */
    /* note: not applicable for ARCFOUR */
    bool f_symm_crypto_modes[SFZCRYPTO_NUM_SYM_KEY_TYPES][SFZCRYPTO_NUM_MODES_SYMCRYPTO];
    bool f_cipher_mac_modes[SFZCRYPTO_NUM_SYM_KEY_TYPES][SFZCRYPTO_NUM_MODES_SYMCRYPTO];

    /* Authenticated Crypto: AES-CCM, AES-SIV */
    bool f_authcrypt_AES_CCM;
    bool f_authcrypt_AES_SIV;

    /* AES-WRAP */
    bool f_wrap_AES_WRAP;

    /* True if key generation supported for DH, ECDH, DSA, ECDSA & RSA */
    bool f_keygen_asym;

    /* Checklist of available asymmetric crypto algorithms. */
    bool f_algos_asymcrypto[SFZCRYPTO_NUM_ALGOS_ASYMCRYPTO];

    /* Checklist of available sign/verify algorithms. */
    bool f_algos_sign[SFZCRYPTO_NUM_ALGOS_ASYMCRYPTO];

    /* Checklist of available key exchange algorithms. */
    bool f_algos_key_exchange[SFZCRYPTO_NUM_ALGOS_ASYMCRYPTO];

    /* Minimum and maximum symmetric key size supported (bits). */
    uint32_t keyrange_sym[SFZCRYPTO_NUM_SYM_KEY_TYPES][3];

    /* Minimum and maximum asymmetric key size supported (bits). */
    uint32_t keyrange_asym[SFZCRYPTO_NUM_ALGOS_ASYMCRYPTO][3];

    /* Checklist of authenticated unlock and Secure Debug */
    bool f_aunlock;
}
SfzCryptoFeatureMatrix;


/*----------------------------------------------------------------------------
 * sfzcrypto_nop
 *
 * This function allows data to be copied with the DMA engine.
 *
 * dst_p
 *     Pointer to the destination memory of at least "len" bytes.
 *
 * src_p
 *     Pointer to the source memory holding "len" bytes.
 *
 * len
 *     Number of bytes to copy from src_p to dst_p.
 *
 * Return Value:
 *     One of SfzCryptoStatus.
 */
SfzCryptoStatus
sfzcrypto_nop(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoOctetsOut * dst_p,
        SfzCryptoOctetsIn * src_p,
        uint32_t len);


/*----------------------------------------------------------------------------
 * sfzcrypto_nvm_publicdata_read
 *
 * Read Public Data Object from non-volatile memory (NVM).
 *
 * The data available depends on the type of hardware and actual data
 * written to NVM in production. This memory is organized in Objects, some of
 * which are private the hardware module and not accessible through this API.
 * Only Public Data Objects can be retrieved via this API.
 *
 * ObjectNr
 *     The identity number for the Public Data object to retrieve.
 *
 * Data_p
 *     Pointer to the output buffer where the object data will be copied to,
 *     or NULL just to query the length of the object.
 *     Size of this this buffer must be provided via *DataLen_p.
 *
 * DataLen_p
 *     In: size, in bytes, of the buffer pointed to by Data_p.
 *     Out: length of the actual object, in bytes.
 *
 * It is possible to query the length of the object by calling this function
 * with Data_p = NULL. In that case the object data is not returned, but the
 * length will be returned via *DataLen_p.
 *
 * Special Return Values:
 *     SFZCRYPTO_INVALID_PARAMETER = ObjectID not found
 */
SfzCryptoStatus
sfzcrypto_nvm_publicdata_read(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t ObjectNr,
        SfzCryptoOctetsOut * Data_p,
        uint32_t * const DataLen_p);


/*
 Read the version of sfzcrypto provider.

 Depending on provider, the exact format of the string and details of
 the information provided are implementation dependent. The resulting string
 is always zero-terminated and this termination character is included in
 the size of the buffer returned via p_len.

 If the provided buffer is not long enough, no data will be written to the
 buffer pointed to by p_version.

  @pre  The p_version value points to a valid pre-allocated
        string buffer of at least *p_len bytes.
  @pre  p_len parameter is not NULL.
  @post retval != SFZCRYPTO_SUCCESS ||
        strlen(p_version) + 1 == *p_len

 @param sfzcryptoctx_p
 Pointer to a pre-allocated and setup SfzCryptoContext object.

 @param p_version
 Pointer to the destination buffer where the provider version
 will be written into, or NULL just to determine length of the buffer.

 @param p_len
 Input output parameter presenting size of p_version writeable storage on
 input and length of the version string written on output.
 When p_version is NULL, the version string is not actually written into
 the buffer, but the update effect on p_len still applies.

 @return
 One of the SfzCryptoStatus values.
 When p_version is NULL, the function always returns SFZCRYPTO_SUCCESS.

*****************************************************************************/
SfzCryptoStatus
sfzcrypto_read_version(
        SfzCryptoContext * const sfzcryptoctx_p,
        char * p_version,
        uint32_t * const p_len);


/*
   Get the list of available features.

   Returns the table of features and combinations thereof supported
   by the implementation of sfzcrypto under question. The format of the table
   is dictated by the SfzCryptoFeatureMatrix data type. Please refer to that
   structure for the details of what each field means and how to use the
   returned feature table.

   @param sfzcryptoctx_p
   Pointer to a pre-allocated and setup SfzCryptoContext object..

   @param p_features
   Pointer to a valid pre-allocated object of type SfzCryptoFeatureMatrix
   that will be filled up by the implementation to reflect available features.

   @return
   One of the SfzCryptoStatus values.

   SFZCRYPTO_SUCCESS if there was no error and the feature matrix has been
   successfully filled with valid values.

*/
SfzCryptoStatus
sfzcrypto_get_featurematrix(
        SfzCryptoFeatureMatrix * const p_features);


/*
   Configure the Multi2 engine.

   Sets the NumberOfRounds and SystemKey parameters to be used for subsequent
   operations with the Multi2 algorithm.

   @param NumberOfRounds
   Desired number of rounds, between 32..128.

   @param SystemKeyAssetId
   Reference to the SystemKey asset. Use SFZCRYPTO_ASSETID_INVALID in case
   the next parameter is used to pass an explicit SystemKey value.

   @param SystemKey_p
   Explicit 256-bit SystemKey value.

   @return
   One of the SfzCryptoStatus values.

*/
SfzCryptoStatus
sfzcrypto_multi2_configure(
        uint8_t          NumberOfRounds,
        SfzCryptoAssetId SystemKeyAssetId,
        const uint8_t *  SystemKey_p);

#endif /* Include Guard */

/* end of file sfzcryptoapi_misc.h */
