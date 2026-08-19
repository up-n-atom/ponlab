/* cm_tokens_asset.h
 *
 * Crypto Module Tokens Parser/Generator - Asset Tokens
 *
 * This module can converts a set of parameters into a Crypto Module Command
 * token, or parses a set of parameters from a Crypto Module Response token.
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

#ifndef INCLUDE_GUARD_CM_TOKENS_ASSET_H
#define INCLUDE_GUARD_CM_TOKENS_ASSET_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "clib.h"               // memset, memcpy
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

// CM accepts a 6-bit number when searching for a static asset.
#define CMTOKENS_STATIC_ASSET_NUMBER_MAX (MASK_6_BITS)

#define CMTOKENS_ASSET_ADD_MINIMUM_LENGTH  33
#define CMTOKENS_ASSET_ADD_PADDING_VALUE   0

/*----------------------------------------------------------------------------
 * CMTOKENS_ASSET_POLICY_*
 *
 * The defines below define the asset policy bits. The policy is a bitmask
 * where five groups of bits define different aspects of the policy as shown
 * below.
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

#define CMTOKENS_ASSET_POLICY_ALGO_CIPHER_MASK          0x00000005U
#define CMTOKENS_ASSET_POLICY_ALGO_CIPHER_AES           0x00000001U
#define CMTOKENS_ASSET_POLICY_ALGO_CIPHER_TRIPLE_DES    0x00000004U

#define CMTOKENS_ASSET_POLICY_ALGO_HMAC_MASK            0x000000E0U
#define CMTOKENS_ASSET_POLICY_ALGO_HMAC_SHA1            0x00000020U
#define CMTOKENS_ASSET_POLICY_ALGO_HMAC_SHA224          0x00000040U
#define CMTOKENS_ASSET_POLICY_ALGO_HMAC_SHA256          0x00000080U

#define CMTOKENS_ASSET_POLICY_ASSET_MASK                0x00000700U
#define CMTOKENS_ASSET_POLICY_ASSET_IV                  0x00000100U
#define CMTOKENS_ASSET_POLICY_ASSET_COUNTER             0x00000200U
#define CMTOKENS_ASSET_POLICY_ASSET_TEMP_MAC            0x00000400U

#define CMTOKENS_ASSET_POLICY_FUNCTION_MASK             0x00007000U
#define CMTOKENS_ASSET_POLICY_FUNCTION_ENCRYPT          0x00001000U
#define CMTOKENS_ASSET_POLICY_FUNCTION_DECRYPT          0x00002000U
#define CMTOKENS_ASSET_POLICY_FUNCTION_MAC              0x00004000U

#define CMTOKENS_ASSET_POLICY_TRUSTED_MASK              0x01E00000U
#define CMTOKENS_ASSET_POLICY_TRUSTED_DERIVE            0x01000000U
#define CMTOKENS_ASSET_POLICY_SECURE_DERIVE             0x00200000U
#define CMTOKENS_ASSET_POLICY_SECURE_WRAP               0x00400000U
#define CMTOKENS_ASSET_POLICY_SECURE_UNWRAP             0x00800000U


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetCreate
 *
 * Request to create an asset with the given policy and length.
 */
static inline void
CMTokens_MakeCommand_AssetCreate(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t Policy,
        const uint32_t LengthInBytes)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (0 << 28);   // Subcode = 0 = Asset Create / Search

    CommandToken_p->W[2] = Policy;

    CommandToken_p->W[3] = (LengthInBytes & MASK_10_BITS);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_AssetCreate
 */
static inline void
CMTokens_ParseResponse_AssetCreate(
        const CMTokens_Response_t * const ResponseToken_p,
        uint32_t * const AssetRef_p)
{
    *AssetRef_p = ResponseToken_p->W[1];
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetSearch
 *
 * Request to return the ID for a static asset with the given index.
 */
static inline void
CMTokens_MakeCommand_AssetSearch(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t Index)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (0 << 28);   // Subcode = 0 = Asset Create / Search

    CommandToken_p->W[3] = (MASK_6_BITS & Index) << 16;

    CommandToken_p->W[3] |= BIT_15;  // search
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_AssetSearch
 */
static inline void
CMTokens_ParseResponse_AssetSearch(
        const CMTokens_Response_t * const ResponseToken_p,
        uint32_t * const AssetRef_p,
        uint32_t * const DataLen_p)     // optional
{
    *AssetRef_p = ResponseToken_p->W[1];

    if (DataLen_p)
    {
        *DataLen_p = (ResponseToken_p->W[2] & MASK_10_BITS);
    }
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetDelete
 */
static inline void
CMTokens_MakeCommand_AssetDelete(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (3 << 28);   // Subcode = 3 = Asset Delete

    CommandToken_p->W[2] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_Derive
 *
 * Request to setup the target asset content by derivation.
 * Use CMTokens_MakeCommand_AssetLoad_SetAad to setup the additional data
 * to be used as input for the key derivation proces.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_Derive(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TargetAssetRef,
        const uint32_t KdkAssetRef,
        const uint32_t SaltLengthInBytes)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (1 << 28);   // Subcode = 1 = Asset Load

    CommandToken_p->W[2] = TargetAssetRef;

    CommandToken_p->W[3] = (SaltLengthInBytes & MASK_10_BITS);
    CommandToken_p->W[3] = BIT_24;      // Derive

    CommandToken_p->W[7] = KdkAssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_Random
 *
 * Request to setup the target asset content with random data. The asset
 * size was already specified when the asset was created. Use CMTokens_
 * MakeCommand_AssetLoad_Export to also request the export of the asset as
 * key blob.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_Random(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TargetAssetRef)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (1 << 28);   // Subcode = 1 = Asset Load

    CommandToken_p->W[2] = TargetAssetRef;

    CommandToken_p->W[3] = BIT_25;      // Random
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_Import
 *
 * Request to setup the target asset content from a key blob.
 * Use CMTokens_MakeCommand_AssetLoad_SetAad to setup the additional data
 * to be used when unwrapping the key blob.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_Import(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TargetAssetRef,
        const uint32_t KekAssetRef,
        const uint32_t KeyBlobSizeInBytes)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (1 << 28);   // Subcode = 1 = Asset Load

    CommandToken_p->W[2] = TargetAssetRef;

    CommandToken_p->W[3] = (KeyBlobSizeInBytes & MASK_10_BITS);
    CommandToken_p->W[3] |= BIT_26;     // Unwrap

    CommandToken_p->W[7] = KekAssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_PlainText
 *
 * Request to setup the target asset content from plain text.
 * Use CMTokens_MakeCommand_AssetLoad_Export to also request the export of the
 * asset as key blob.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_Plaintext(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TargetAssetRef,
        const uint32_t PlaintextLengthInBytes)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (1 << 28);   // Subcode = 1 = Asset Load

    CommandToken_p->W[2] = TargetAssetRef;

    CommandToken_p->W[3] = (PlaintextLengthInBytes & MASK_10_BITS);
    CommandToken_p->W[3] |= BIT_27;     // Plaintext
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_AesUnwrap
 *
 * Request to setup the target asset content from an AES key wrapped key blob.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_AesUnwrap(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TargetAssetRef,
        const uint32_t KekAssetRef,
        const uint32_t KeyBlobSizeInBytes)
{
    CommandToken_p->W[0] = (7 << 24) |  // Opcode = 7 = Asset Management
                           (1 << 28);   // Subcode = 1 = Asset Load

    CommandToken_p->W[2] = TargetAssetRef;

    CommandToken_p->W[3] = (KeyBlobSizeInBytes & MASK_10_BITS);
    CommandToken_p->W[3] |= BIT_28;     // Unwrap

    CommandToken_p->W[7] = KekAssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_SetAad
 *
 * Setup additional data for
 * - AssetLoad Derive
 * - AssetLoad Unwrap / Import
 * - AssetLoad Plain/Generate with request to produce a keyblob ('Wrap')
 *
 * Minimum AAD length is enforced by this function by padding with zero bytes.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_SetAad(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * const AdditionalData_p,
        uint32_t AdditionalDataSizeInBytes)
{
    uint8_t AAD[CMTOKENS_ASSET_ADD_MINIMUM_LENGTH + 4];  // +4 for ReadByteArray
    const uint8_t * AAD_p = AdditionalData_p;

    if (AdditionalDataSizeInBytes < CMTOKENS_ASSET_ADD_MINIMUM_LENGTH)
    {
        // add padding
        memset(
            AAD,
            CMTOKENS_ASSET_ADD_PADDING_VALUE,
            CMTOKENS_ASSET_ADD_MINIMUM_LENGTH);

        memcpy(
            AAD,
            AdditionalData_p,
            AdditionalDataSizeInBytes);

        AdditionalDataSizeInBytes = CMTOKENS_ASSET_ADD_MINIMUM_LENGTH;
        AAD_p = AAD;
    }

    CommandToken_p->W[3] |= (AdditionalDataSizeInBytes << 16);

    CMTokens_MakeCommand_ReadByteArray(
            AAD_p,
            AdditionalDataSizeInBytes,
            CommandToken_p,
            /*StartWord:*/8);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_Export
 *
 * Request to export an asset as key blob after its contents have been setup
 * with either random or plain text data.
 * Use CMTokens_MakeCommand_AssetLoad_SetAad to setup the additional data
 * to be used when generating the key blob.
 */
static inline void
CMTokens_MakeCommand_AssetLoad_Export(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t KekAssetRef)
{
    CommandToken_p->W[3] |= BIT_31;      // KeyBlob

    CommandToken_p->W[7] = KekAssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_AssetLoad_Export
 */
static inline void
CMTokens_ParseResponse_AssetLoad_Export(
        const CMTokens_Response_t * const ResponseToken_p,
        uint32_t * const KeyBlobSizeInBytes_p)
{
    *KeyBlobSizeInBytes_p = ResponseToken_p->W[1] & MASK_10_BITS;
}


/*----------------------------------------------------------------------------
 * CMTokens_CommandNeedsAppID
 *
 * Returns true when the provided token is an Asset Load token with the
 * Derive flag set.
 */
static inline bool
CMTokens_CommandNeedsAppID(
        const CMTokens_Command_t * const CommandToken_p)
{
    if (((CommandToken_p->W[0] >> 24) & MASK_6_BITS) == 0x17)
    {
        // Token = Asset Management; Asset Load
        if (CommandToken_p->W[3] & BIT_24)
        {
            // Derive command
            return true;
        }
    }

    return false;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_InsertAppID
 *
 * This function is called the insert the AppID at the start of the AAD area
 * in the AssetLoad command. This is needed when CMToken_Command_NeedsAppID
 * returns true.
 */
static inline void
CMTokens_MakeCommand_InsertAppID(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * const AdditionalData_p,
        uint32_t AdditionalDataSizeInBytes)
{
    unsigned int AADMax = (CMTOKENS_COMMAND_WORDS - 8) * 4; // Max AAD bytes in token
    unsigned int AADLen = (CommandToken_p->W[3] >> 16) & MASK_8_BITS;

    // ensure additional data by itself fits
    if (AdditionalDataSizeInBytes > AADMax)
        AdditionalDataSizeInBytes = AADMax;

    // calculate how much of the current AAD data can remain
    if (AADLen + AdditionalDataSizeInBytes > AADMax)
        AADLen = AADMax - AdditionalDataSizeInBytes;

    // move the current AAD data to make space for the new data
    // move is done on byte-array, assuming LSB-first
    {
        uint8_t * AAD_Src_p = (uint8_t *)(CommandToken_p->W + 8);
        uint8_t * AAD_Dst_p = AAD_Src_p + AdditionalDataSizeInBytes;
        unsigned int i;

        for (i = AADLen; i > 0; i--)
            AAD_Dst_p[i - 1] = AAD_Src_p[i - 1];
    }

    // now write the new AAD data
    CMTokens_MakeCommand_ReadByteArray(
            AdditionalData_p,
            AdditionalDataSizeInBytes,
            CommandToken_p,
            8);

    // overwrite the length field
    // (do not try to update it due to size limiters above)
    AADLen += AdditionalDataSizeInBytes;
    CommandToken_p->W[3] &= ~(MASK_8_BITS << 16);
    CommandToken_p->W[3] |= (AADLen << 16);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_WriteInDescriptor
 */
static inline void
CMTokens_MakeCommand_AssetLoad_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 4, 1, Descriptor_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_AssetLoad_WriteOutDescriptor
 */
static inline void
CMTokens_MakeCommand_AssetLoad_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteOutDescriptor(CommandToken_p, 5, 2, Descriptor_p);
}


#endif /* Include Guard */

/* end of file cm_tokens_asset.h */
