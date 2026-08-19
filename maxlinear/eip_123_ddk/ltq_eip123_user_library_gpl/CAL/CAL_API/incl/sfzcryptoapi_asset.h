/* sfzcryptoapi_asset.h
 *
 * The Cryptographic Abstraction Layer API: Asset Management.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_ASSET_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_ASSET_H

#include "public_defs.h"
#include "sfzcryptoapi_enum.h"
#include "sfzcryptoapi_init.h"       // SfzCryptoContext
#include "sfzcryptoapi_buffers.h"    // SfzCryptoOctet*Ptr*


/*----------------------------------------------------------------------------
 * SfzCryptoAssetId
 *
 * Asset Identifier. Refers to an asset in the Asset Store.
 *
 * The AssetId is decided by the implementation and its value should not be
 * used directly, except for comparing against SFZCRYPTO_ASSETID_INVALID.
 */
typedef uint32_t SfzCryptoAssetId;

// guaranteed unused AssetId value
#define SFZCRYPTO_ASSETID_INVALID  0


/*----------------------------------------------------------------------------
 * SfzCryptoTrustedAssetId
 *
 * This is an alias for SfzCryptoAssetId. It is used in API to stress the fact
 * that the key must be a trusted key (this is defined by its policy).
 */
typedef SfzCryptoAssetId SfzCryptoTrustedAssetId;


/*----------------------------------------------------------------------------
 * SFZCRYPTO_ASSET_SIZE_MAX
 *
 * The maximum value for the AssetSize parameter used in this API.
 */
#define SFZCRYPTO_ASSET_SIZE_MAX   (512 / 8)


/*----------------------------------------------------------------------------
 * SfzCryptoPolicyMask, SFZCRYPTO_POLICY_*
 *
 * This type is used to hold the asset policy. The policy is a bitmask where
 * five groups of bits define different aspects of the policy as shown below.
 *
 * NOTE: The limitations on the policy are decided by the implementation.
 *       Please check the documentation.
 *
 * CIPHER_MASK:
 *    These policy bits define the symmetric cipher algorithm the asset may
 *    be used in. Currently supported are AES and 3DES.
 *
 * HMAC_MASK:
 *    These policy bits define the HMAC algorithm the asset may be used in.
 *    Currently supported are HMAC-SHA1, HMAC-SHA224 and HMAC-SHA256.
 *
 * ASSET_MASK:
 *    These policy bits define special types of assets:
 *       IV: for AES and 3DES.
 *       Counter: for AES counter modes (ICM, CTR)
 *       Temporary (intermediate) MAC: for HMAC and CMAC.
 *
 * FUNCTION_MASK:
 *    These policy bits define the operation that can be performed with this
 *    asset. Currently support are Encrypt, Decrypt and MAC.
 *
 * TRUSTED_MASK:
 *    These policy bits define enable the four possible trusted operations
 *    using the asset:
 *        TRUSTED_DERIVE: Asset may be used in Trusted Derive operation to
 *                        create a KDK or KEK.
 *        SECURE_DERIVE:  Asset may be used in Secure Derive operation to
 *                        create a normal key.
 *        SECURE_WRAP:    Asset is a KEK and may be used to export a keyblob.
 *        SECURE_UNWRAP:  Asset is a KEK and may be used to import a keyblob.
 */

/* 32-bit mask representing access policy of the asset. */
typedef uint32_t SfzCryptoPolicyMask;

#define SFZCRYPTO_POLICY_ALGO_CIPHER_MASK         0x0000001FU
#define SFZCRYPTO_POLICY_ALGO_CIPHER_AES          0x00000001U
#define SFZCRYPTO_POLICY_ALGO_CIPHER_CAMELLIA     0x00000002U
#define SFZCRYPTO_POLICY_ALGO_CIPHER_TRIPLE_DES   0x00000004U
#define SFZCRYPTO_POLICY_ALGO_CIPHER_MULTI2       0x00000008U
#define SFZCRYPTO_POLICY_ALGO_CIPHER_C2           0x00000010U

#define SFZCRYPTO_POLICY_ALGO_HMAC_MASK           0x000000E0U
#define SFZCRYPTO_POLICY_ALGO_HMAC_SHA1           0x00000020U
#define SFZCRYPTO_POLICY_ALGO_HMAC_SHA224         0x00000040U
#define SFZCRYPTO_POLICY_ALGO_HMAC_SHA256         0x00000080U

#define SFZCRYPTO_POLICY_ASSET_MASK               0x00000F00U
#define SFZCRYPTO_POLICY_ASSET_IV                 0x00000100U
#define SFZCRYPTO_POLICY_ASSET_COUNTER            0x00000200U
#define SFZCRYPTO_POLICY_ASSET_TEMP_MAC           0x00000400U
#define SFZCRYPTO_POLICY_ASSET_C_CBC_STATE        0x00000800U
#define SFZCRYPTO_POLICY_ASSET_AUTHSTATE          0x10000000U

#define SFZCRYPTO_POLICY_FUNCTION_MASK            0x000F7000U
#define SFZCRYPTO_POLICY_FUNCTION_ENCRYPT         0x00001000U
#define SFZCRYPTO_POLICY_FUNCTION_DECRYPT         0x00002000U
#define SFZCRYPTO_POLICY_FUNCTION_MAC             0x00004000U
// Policy (function) bits unique C2 key assets
#define SFZCRYPTO_POLICY_C2_DERIVE_MASK           0x000F0000U
#define SFZCRYPTO_POLICY_C2_KZ_DERIVE             0x00010000U
#define SFZCRYPTO_POLICY_C2_KS_DERIVE             0x00020000U
#define SFZCRYPTO_POLICY_C2_KMU_DERIVE            0x00040000U
#define SFZCRYPTO_POLICY_C2_KM_DERIVE             0x00080000U

#define SFZCRYPTO_POLICY_TRUSTED_MASK             0x01E00000U
#define SFZCRYPTO_POLICY_TRUSTED_DERIVE           0x01000000U
#define SFZCRYPTO_POLICY_SECURE_DERIVE            0x00200000U
#define SFZCRYPTO_POLICY_SECURE_WRAP              0x00400000U
#define SFZCRYPTO_POLICY_SECURE_UNWRAP            0x00800000U

/* Mask of used bits. */
#define SFZCRYPTO_POLICY_FULL_MASK        \
    (SFZCRYPTO_POLICY_TRUSTED_MASK |      \
     SFZCRYPTO_POLICY_C2_DERIVE_MASK |    \
     SFZCRYPTO_POLICY_FUNCTION_MASK |     \
     SFZCRYPTO_POLICY_ASSET_MASK |        \
     SFZCRYPTO_POLICY_ALGO_HMAC_MASK |    \
     SFZCRYPTO_POLICY_ALGO_CIPHER_MASK)

#define SFZCRYPTO_POLICY_UNUSED_MASK      \
    (~SFZCRYPTO_POLICY_FULL_MASK)

/* Extract algorithm, asset, unused and trusted bits.
   These form type of asset.
   Unused bits are included, because the type must be regarded
   as invalid if there are unused bits detected -
   thus unused bits affect the type.
 */
#define SFZCRYPTO_POLICY_TYPE_MASK        \
    (SFZCRYPTO_POLICY_TRUSTED_MASK |      \
     SFZCRYPTO_POLICY_ASSET_MASK |        \
     SFZCRYPTO_POLICY_ALGO_HMAC_MASK |    \
     SFZCRYPTO_POLICY_ALGO_CIPHER_MASK |  \
     SFZCRYPTO_POLICY_UNUSED_MASK)


/*----------------------------------------------------------------------------
 * Typical Key Policies
 *
 * AES-CBC Key:
 *    CIPHER_MASK = AES
 *    FUNCTION_MASK = Encrypt + Decrypt
 *
 * HMAC-SHA224 Key:
 *    HMAC_MASK = HMAC-SHA224
 *    FUNCTION_MASK = MAC
 *
 * HMAC-SHA256 MAC:
 *    HMAC_MASK = HMAC-SHA256
 *    ASSET_MASK = TempMAC
 *
 * AES-CBCMAC Key:
 *    CIPHER_MASK = AES
 *    FUNCTION_MASK = Encrypt
 *
 * AES-CBCMAC MAC:
 *    CIPHER_MASK = AES
 *    FUNCTION_MASK = MAC
 *    FUNCTION_MASK = Encrypt
 *
 * Temporary IV's: use sfzcrypto_asset_alloc_temp()
 */


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_alloc
 *
 * This function allocates an entry in the Asset Store, allowing it to be
 * filled with a cryptographic secret (typically a key). The use-policy, size
 * and ownership are set and cannot be changed afterwards.
 *
 * DesiredPolicy
 *     Bitmask (based on SFZCRYPTO_POLICY_*) that defines the use-policy for
 *     the asset.
 *
 * AssetSize
 *     The requested exact size of the asset, in bytes.
 *
 * NewAssetId_p
 *     Pointer to the memory location where the AssetId will be written when
 *     the entry was successfully allocated. The AssetId is needed to use the
 *     asset, or free it using sfzcrypto_asset_free.
 *
 * NOTE: To avoid running out of space in the Asset Store, implementations can
 *       choose to discard assets from the Asset Store when an application
 *       has terminated.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_alloc(
        SfzCryptoContext *       const sfzcryptoctx_p,
        SfzCryptoPolicyMask      DesiredPolicy,
        SfzCryptoSize            AssetSize,
        SfzCryptoAssetId * const NewAssetId_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_alloc_temporary
 *
 * This function allocates an entry in the Asset Store for use as
 * temporary asset in a specific cryptographic operation. The cryptographic
 * operation is specified by the type of the key used in that operation, plus
 * a secondary parameter that depends on the operation: either the hash
 * algorithm or the cipher feedback mode.
 *
 * KeyType
 *     The type of the key for the operation.
 *     Use one of SFZCRYPTO_KEY_*
 *
 * FbMode
 *     The cipher feedback mode. Use one of SFZCRYPTO_MODE_*
 *     This parameter is ignored when KeyType == SFZCRYPTO_KEY_HMAC.
 *
 * HashAlgo
 *     The hash algorithm. Use one of SFZCRYPTO_HASH_ALGO_*
 *     This parameter is ignored unless KeyType == SFZCRYPTO_KEY_HMAC.
 *
 * KeyAssetId
 *     The asset ID of the key this temporary asset will be used with.
 *
 * NewTempAssetId_p
 *     Pointer to the location where the AssetId of the temporary asset
 *     will be written, when it was allocated successfully.
 *     The TempAssetId can be used as iv_asset_id or mac_asset_id field of
 *     the context structure for the corresponding operation.
 *     The TempAssetId must be freed using sfzcrypto_asset_free.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 *
 * Example for allocating a temporary IV for use with AES-CBC:
 *   sfzcrypto_asset_alloc_temp(
 *                    context_p,
 *                    SFZCRYPTO_KEY_AES,
 *                    SFZCRYPTO_MODE_CBC,
 *                    0,
 *                    KeyAssetId,
 *                    &TempAssetId);
 *
 * Example for allocating a temporary MAC for use with HMAC-SHA-256:
 *   sfzcrypto_asset_alloc_temp(
 *                    context_p,
 *                    SFZCRYPTO_KEY_HMAC,
 *                    0,
 *                    SFZCRYPTO_ALGO_HASH_SHA256,
 *                    KeyAssetId,
 *                    &TempAssetId);
 *
 * Example for allocating a temporary MAC for use with AES-CMAC:
 *   sfzcrypto_asset_alloc_temp(
 *                    context_p,
 *                    SFZCRYPTO_KEY_AES,
 *                    SFZCRYPTO_MODE_CMAC,
 *                    0,
 *                    KeyAssetId,
 *                    &TempAssetId);
 */
SfzCryptoStatus
sfzcrypto_asset_alloc_temporary(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoSymKeyType KeyType,
        SfzCryptoModeType FbMode,       // only when KeyType != SFZCRYPTO_KEY_HMAC
        SfzCryptoHashAlgo HashAlgo,     // only when KeyType == SFZCRYPTO_KEY_HMAC
        SfzCryptoAssetId KeyAssetId,
        SfzCryptoAssetId * const NewTempAssetId_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_free
 *
 * This function removes an asset from the Asset Store, freeing up the space
 * it occupied and invalidating the corresponding AssetId.
 *
 * AssetId
 *     Reference to the asset to remove from the Asset Store
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_free(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId AssetId);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_search
 *
 * This function can be used to get an AssetId for static assets that were
 * typically burned into the chip during production.
 *
 * StaticAssetNumber
 *     The number to search for in the available static assets. The actual
 *     numbers supported depends on the contents of the NVM.
 *
 * NewAssetId_p
 *     Pointer to the memory location where the AssetId will be written when
 *     the static asset was found. The AssetId is needed to use the asset.
 *     It is not required to free the AssetId using sfzcrypto_asset_free.
 *
 * NOTE: The supported values for StaticAssetNumber are defined by the device
 *       manufacturer and can differ from product to product.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_search(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t StaticAssetNumber,
        SfzCryptoAssetId * const NewAssetId_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_get_root_key
 *
 * This helper function returns the AssetId of the RootKey available to this
 * application. The RootKey can be used in Trusted Derive operations.
 * This function cannot fail. The AssetId for the RootKey is constant and
 * cached by the implementation.
 */
SfzCryptoAssetId
sfzcrypto_asset_get_root_key(void);


/*----------------------------------------------------------------------------
 * Key Blob
 *
 * This followed defines help process a Key Blob, which is a cryptographically
 * protected block of data with an Asset inside. It is generated and returned
 * immediately after an asset was loaded, derived or generated. This is the
 * only possiblity to get a keyblob as it cannot be generated on request.
 * The keyblob can be imported into the Asset Store at a later time, for
 * example after a power cycle.
 *
 * The functions that can generate a keyblob require the following five
 * parameters:
 *
 * KeyAssetId (input)
 *     AssetId of the KEK to be used when generating the keyblob.
 *
 * AdditionalData_p (input)
 * AdditionalDataSize (input)
 *     Pointer and number of bytes for the AAD used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * KeyBlob_p (output)
 *     Pointer to the caller's memory block where the keyblob is stored.
 *
 * KeyBlobSize_p (input, output)
 *     Size of the memory block, in bytes, pointed to by KeyBlob_p.
 *     When generating a keyblob, the size will be updated with the actual
 *     size when this function returns.
 *     When important a keyblob, the size must be the exact size of the
 *     keyblob, in bytes.
 */

/* Maximum sizes. */
/* For AAD used in construction of keyblob. */
#define SFZCRYPTO_KEYBLOB_AAD_MAX_SIZE     224
/* For KDF label. */
#define SFZCRYPTO_KDF_LABEL_MAX_SIZE       (224 - 20)

/* KeyBlob uses AES-SIV algorithm, without nonce. */
#define SFZCRYPTO_KEYBLOB_SIZE(keysize)    ((keysize) + 16)
/* Max KeyBlob size, based on 512-bit key, i.e. 512 / 8 */
#define SFZCRYPTO_KEYBLOB_MAX_DATA_SIZE    (512 / 8 + 16)


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_import
 *
 * This function imports an asset previously exported in a Keyblob.
 * The following parameters must be identical to when the Keyblob was
 * generated, otherwise it will not be imported:
 *   - Asset Ownership
 *   - Asset Policy
 *   - Asset Size
 *   - KEK used
 *   - Additional Data
 * The first three were set when sfzcrypto_asset_alloc was invoked. The other
 * two paremeters to this function.
 *
 * TargetAssetId
 *     The asset to import the keyblob into.
 *     This number must have been returned by sfzcrypto_asset_alloc.
 *
 * KeyAssetId
 * AdditionalData_p
 * AdditionalDataSize
 * KeyBlob_p
 * KeyBlobSize_p
 *     See section "Key Blob" above for a description of these parameters.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_import(
        SfzCryptoContext *       const sfzcryptoctx_p,
        SfzCryptoAssetId         TargetAssetId,
        SfzCryptoTrustedAssetId  KekAssetId,
        SfzCryptoOctetsIn *      AdditionalData_p,
        SfzCryptoSize            AdditionalDataSize,
        SfzCryptoOctetsIn *      KeyBlob_p,
        SfzCryptoSize            KeyBlobSize);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_derive
 *
 * This function allows one asset to be derived from another asset using the
 * built-in Key Derivation Function (KDF). The target asset must have been
 * allocated, with ownership, policy and length defined, but not yet filled
 * with data.
 *
 * It is not possible nor required to get a keyblob since the derive operation
 * is repeatable: it produces in the same key after every power cycle.
 *
 * TargetAssetId
 *     This number references the asset to store the derived key into.
 *     This number must have been returned by sfzcrypto_asset_alloc.
 *     The asset also contains the desired length of the key.
 *
 * Label_p
 * LabelLen
 *     Pointer-to and length-in-bytes of the label to use in the KDF.
 *     The label, together with other properties of the asset, helps ensuring
 *     the derived asset is unique.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_derive(
        SfzCryptoContext *       const sfzcryptoctx_p,
        SfzCryptoAssetId         TargetAssetId,
        SfzCryptoTrustedAssetId  KdkAssetId,
        SfzCryptoOctetsIn *      Label_p,
        SfzCryptoSize            LabelLen);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_load_key
 *
 * This function can be used to load a plaintext secret into the Asset Store.
 * The asset, with ownership, policy and size, must have been allocated using
 * sfzcrypto_asset_alloc.
 *
 * TargetAssetId
 *     This number references the asset to load the data into.
 *     This number must have been returned by sfzcrypto_asset_alloc.
 *
 * Data_p
 *     Pointer to the data to be loaded into the asset.
 *
 * DataSize
 *     Number of data bytes pointed to by Data_p.
 *     This value must match exactly the size of the asset as provided to
 *     sfzcrypto_asset_alloc.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_load_key(
        SfzCryptoContext *   const sfzcryptoctx_p,
        SfzCryptoAssetId     TargetAssetId,
        SfzCryptoOctetsIn *  Data_p,
        SfzCryptoSize        DataSize);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_gen_key
 *
 * This function allows an asset to be filled with random data. Since the
 * Random Number Generator is directly accessible to the Asset Store, this
 * operation allows generating a truely secret key that is only known inside
 * the Asset Store.
 *
 * TargetAssetId
 *     This number references the asset to store the generated key into.
 *     This number must have been returned by sfzcrypto_asset_alloc.
 *     The asset also contains the desired length of the asset.
 *
 * DataSize
 *     Number of bytes to take from RNG and put into asset.
 *     This value must match exactly the size of the asset as provided to
 *     sfzcrypto_asset_alloc.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_gen_key(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId   TargetAssetId,
        SfzCryptoSize      DataSize);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_load_key_and_wrap
 *
 * KeyAssetId
 * AdditionalData_p
 * AdditionalDataSize
 * KeyBlob_p
 * KeyBlobSize_p
 *     See section "Key Blob" above for a description of these parameters.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_load_key_and_wrap(
        SfzCryptoContext * const sfzcryptoctx_p,
        SfzCryptoAssetId         TargetAssetId,
        SfzCryptoOctetsIn *      Data_p,
        SfzCryptoSize            AssetSize,
        SfzCryptoTrustedAssetId  KekAssetId,
        SfzCryptoOctetsIn *      AdditionalData_p,
        SfzCryptoSize            AdditionalDataSize,
        SfzCryptoOctetsOut *     KeyBlob_p,
        SfzCryptoSize * const    KeyBlobSize_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_asset_gen_key_and_wrap
 *
 * This function is a variant of sfzcrypto_asset_gen_key - see the
 * description elsewhere in this file. Apart from performing that function,
 * it also generates and exports a Keyblob.
 *
 * KeyAssetId
 * AdditionalData_p
 * AdditionalDataSize
 * KeyBlob_p
 * KeyBlobSize_p
 *     See section "Key Blob" above for a description of these parameters.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_asset_gen_key_and_wrap(
        SfzCryptoContext *       const sfzcryptoctx_p,
        SfzCryptoAssetId         TargetAssetId,
        SfzCryptoSize            AssetSize,
        SfzCryptoTrustedAssetId  KekAssetId,
        SfzCryptoOctetsIn *      AdditionalData_p,
        SfzCryptoSize            AdditionalDataSize,
        SfzCryptoOctetsOut *     KeyBlob_p,
        SfzCryptoSize * const    KeyBlobSize_p);


#endif /* Include Guard */

/* end of file sfzcryptoapi_asset.h */
