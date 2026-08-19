/* cal_cm-v2_asset.c
 *
 * Implementation of the CAL API.
 *
 * This file implements the Asset Store services, invoking the Crypto Module.
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

#include "c_cal_cm-v2.h"

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                 // the API to implement

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_asset.h"
#include "cm_tokens_errdetails.h"

#define CALCM_KEYBLOB_EXTRA \
    (SFZCRYPTO_KEYBLOB_MAX_DATA_SIZE - SFZCRYPTO_ASSET_SIZE_MAX)

#if defined(SFZCRYPTO_CF_ASSET_IMPORT__CM) || \
    defined(SFZCRYPTO_CF_ASSET_DERIVE__CM) || \
    defined(SFZCRYPTO_CF_ASSET_LOAD_KEY__CM) || \
    defined(SFZCRYPTO_CF_ASSET_GEN_KEY__CM) || \
    defined(SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__CM) || \
    defined(SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__CM)
#define CALCMLIB_ENABLE_ASSETLOAD_COMMON
#endif


/*----------------------------------------------------------------------------
 * CALCMLib_AssetLoad_Common
 *
 * This function is called by all sfzcrypto_cm_asset_* functions that setup
 * the content of an asset. The AssetLoad token pointed to by t_cmd_p is
 * almost completely setup except for the Input and/or Output DMA addresses
 * (and the application identity).
 *
 * If non-NULL, the input (plaintext key or key blob) is prepared for input
 * via DMA. Also, the output of a key blob via DMA is prepared if KeyBlob_p
 * is non-NULL. The relevant DMA addresses are added to the AssetLoad token.
 * Next, a token exchange with the CM is done. The response token is checked
 * for errors and the necessary post-DMA processing, if any, is performed.
 * Finally, the status indicating success or error is returned, together
 * with key blob output, if any.
 */
#ifdef CALCMLIB_ENABLE_ASSETLOAD_COMMON
static SfzCryptoStatus
CALCMLib_AssetLoad_Common(
        CMTokens_Command_t * const t_cmd_p,
        SfzCryptoOctetsIn * Input_p,
        SfzCryptoSize InputSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p)
{
    CALCM_DMA_Admin_t * Task_p;
    CMTokens_Response_t t_rsp;
    SfzCryptoStatus status;

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    // prepare data for input via DMA and setup DMA InputAddress in token.
    if (Input_p != NULL)
    {
        EIP123_Fragment_t Fragment;

        if (!CALAdapter_InputBufferPreDMA(
                    Task_p,
                    4 /*AlgorithmicBlockSize*/,
                    &Fragment,
                    InputSize,
                    Input_p,
                    NULL /* unused LastBlock_p */))
        {
            CALCM_DMA_Free(Task_p);
            return SFZCRYPTO_INTERNAL_ERROR;
        }

        // add the DMA address of the (bounced) input buffer to the token.
        CMTokens_MakeCommand_AssetLoad_WriteInDescriptor(t_cmd_p, &Task_p->InDescriptor);
    }

    if (KeyBlob_p != NULL)
    {
        // prepare for output of the key blob via DMA.
        // re-use code that that always creates a bounce buffer, with an
        // extra word for the TokenID.
        status = CALAdapter_RandomWrapNvm_PrepareOutput(
                        Task_p,
                        *KeyBlobSize_p,
                        KeyBlob_p,
                        /*fOutputByteCount_Includes_TokenId:*/false);

        if (status != SFZCRYPTO_SUCCESS)
        {
            CALCM_DMA_Free(Task_p);
            return status;
        }

        // request the 'WrTokenID' option and add the output DMA address
        // to the AssetLoad token.
        CMTokens_MakeCommand_SetTokenID(t_cmd_p, CAL_TOKENID_VALUE, true);
        CMTokens_MakeCommand_AssetLoad_WriteOutDescriptor(t_cmd_p, &Task_p->OutDescriptor);
    }

    // perform a token exchange with the CM
    status = CAL_CM_ExchangeToken(t_cmd_p, &t_rsp);
    if (status != SFZCRYPTO_SUCCESS)
    {
        // free the bounce buffers, if any, before returning with an error status
        CALAdapter_PostDMA(Task_p);
        CALCM_DMA_Free(Task_p);

        return status;
    }

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_rsp);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_rsp, &ErrMsg_p);

            LOG_WARN(
                "CALCMLib_AssetLoad_Common: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            // free the bounce buffers, if any, before returning with an error status
            CALAdapter_PostDMA(Task_p);
            CALCM_DMA_Free(Task_p);

            // return specific error code
            if (res == CMTOKENS_RESULT_SEQ_UNWRAP_ERROR)
                return SFZCRYPTO_SIGNATURE_CHECK_FAILED;

            return SFZCRYPTO_INTERNAL_ERROR;
        }

        // perform post-DMA processing on key blob output if applicable
        if (KeyBlobSize_p != NULL)
        {
            *KeyBlobSize_p = 0;
            status = CALAdapter_RandomWrapNvm_FinalizeOutput(Task_p);

            if (status == SFZCRYPTO_SUCCESS)
                // return actual key blob size
                CMTokens_ParseResponse_AssetLoad_Export(&t_rsp, (uint32_t *)KeyBlobSize_p);
            else
                CALAdapter_PostDMA(Task_p);
        }
        else
            CALAdapter_PostDMA(Task_p);
    }

    CALCM_DMA_Free(Task_p);

    return status;
}
#endif /* CALCMLIB_ENABLE_ASSETLOAD_COMMON */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_alloc
 *
 * Allocate an asset and set its policy. Its content is setup later.
 */
#ifdef SFZCRYPTO_CF_ASSET_ALLOC__CM
SfzCryptoStatus
sfzcrypto_cm_asset_alloc(
        SfzCryptoPolicyMask DesiredPolicy,
        SfzCryptoSize AssetSize,
        SfzCryptoAssetId * const NewAssetId_p)
{
    CMTokens_Response_t t_rsp;
    CMTokens_Command_t t_cmd;
    SfzCryptoStatus status;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_alloc: "
        "Policy=0x%x; Size=%d\n",
        DesiredPolicy,
        AssetSize);
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (DesiredPolicy == 0 ||
        AssetSize > SFZCRYPTO_ASSET_SIZE_MAX ||
        NewAssetId_p == NULL)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    CMTokens_MakeCommand_AssetCreate(&t_cmd, DesiredPolicy, AssetSize);

    // exchange a message with the CM
    status = CAL_CM_ExchangeToken(&t_cmd, &t_rsp);
    if (status != SFZCRYPTO_SUCCESS)
    {
        return status;
    }

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_rsp);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_rsp, &ErrMsg_p);

            LOG_WARN(
                "sfzcrypto_cm_asset_alloc: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            if (res == CMTOKENS_RESULT_SEQ_INVALID_LENGTH)
                return SFZCRYPTO_INVALID_KEYSIZE;

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    CMTokens_ParseResponse_AssetCreate(&t_rsp, NewAssetId_p);

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_alloc: "
        "AssetId=0x%x\n",
        *NewAssetId_p);
#endif


    return SFZCRYPTO_SUCCESS;
}
#endif /* SFZCRYPTO_CF_ASSET_ALLOC__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_alloc_temporary
 */
#ifdef SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__CM
SfzCryptoStatus
sfzcrypto_cm_asset_alloc_temporary(
        SfzCryptoSymKeyType KeyType,
        SfzCryptoModeType FbMode,
        SfzCryptoHashAlgo HashAlgo,
        SfzCryptoAssetId KeyAssetId,
        SfzCryptoAssetId * const NewTempAssetId_p)
{
    SfzCryptoPolicyMask policy;
    uint32_t assetLen;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_alloc_temporary: "
        "KeyType=%u, FbMode=%u, HashAlgo=%u, KeyAssetId=0x%x\n",
        KeyType, FbMode, HashAlgo, KeyAssetId);
#endif

    if (KeyAssetId == SFZCRYPTO_ASSETID_INVALID)
        return SFZCRYPTO_INVALID_PARAMETER;

    if (KeyType == SFZCRYPTO_KEY_HMAC)
    {
        if (HashAlgo == SFZCRYPTO_ALGO_HASH_SHA160)
        {
            assetLen = (160 / 8);
        }
        else
        {
            // SHA-224 and SHA-256
            // both have 256bit temporary MAC
            assetLen = (256 / 8);
        }

        return sfzcrypto_cm_asset_alloc(
                           SFZCRYPTO_POLICY_ASSET_TEMP_MAC,
                           assetLen,
                           NewTempAssetId_p);
    }

    switch(KeyType)
    {
        case SFZCRYPTO_KEY_AES:
            assetLen = SFZCRYPTO_AES_BLOCK_LEN;
            break;

        case SFZCRYPTO_KEY_CAMELLIA:
            assetLen = SFZCRYPTO_CAMELLIA_BLOCK_LEN;
            break;

        case SFZCRYPTO_KEY_TRIPLE_DES:
            assetLen = SFZCRYPTO_DES_BLOCK_LEN;
            break;

        case SFZCRYPTO_KEY_C2:
            assetLen = SFZCRYPTO_C2_BLOCK_LEN;
            break;

        default:
            return SFZCRYPTO_INVALID_PARAMETER;
    } // switch

    switch(FbMode)
    {
        case SFZCRYPTO_MODE_CBC:
        case SFZCRYPTO_MODE_CBCMAC:
            policy = SFZCRYPTO_POLICY_ASSET_IV;
            break;

        case SFZCRYPTO_MODE_CTR:
        case SFZCRYPTO_MODE_ICM:
            policy = SFZCRYPTO_POLICY_ASSET_COUNTER;
            break;

        case SFZCRYPTO_MODE_CMAC:
            policy = SFZCRYPTO_POLICY_ASSET_TEMP_MAC;
            break;

        case SFZCRYPTO_MODE_C_CBC:
            policy = SFZCRYPTO_POLICY_ASSET_C_CBC_STATE |
                     SFZCRYPTO_POLICY_ALGO_CIPHER_C2;
            break;

        case SFZCRYPTO_MODE_C2_H:
            policy = SFZCRYPTO_POLICY_ASSET_TEMP_MAC |
                     SFZCRYPTO_POLICY_ALGO_CIPHER_C2;
            break;

        default:
            /* This mode does not support or need temporary assets. */
            return SFZCRYPTO_INVALID_PARAMETER;
    }   // switch

    return sfzcrypto_cm_asset_alloc(
                       policy,
                       assetLen,
                       NewTempAssetId_p);
}
#endif /* SFZCRYPTO_CF_ASSET_ALLOC_TEMPORARY__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_free
 *
 * Free the asset referenced by AssetId.
 */
#ifdef SFZCRYPTO_CF_ASSET_FREE__CM
SfzCryptoStatus
sfzcrypto_cm_asset_free(
        SfzCryptoAssetId AssetId)
{
    CMTokens_Response_t t_rsp;
    CMTokens_Command_t t_cmd;
    SfzCryptoStatus status;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_free: "
        "AssetId=0x%x\n",
        AssetId);
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    CMTokens_MakeCommand_AssetDelete(&t_cmd, AssetId);

    // exchange a message with the CM
    status = CAL_CM_ExchangeToken(&t_cmd, &t_rsp);
    if (status != SFZCRYPTO_SUCCESS)
    {
        return status;
    }

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_rsp);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_rsp, &ErrMsg_p);

            LOG_WARN(
                "sfzcrypto_cm_asset_free: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            // specific return code for attempt to free static asset
            if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
                return SFZCRYPTO_OPERATION_FAILED;

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    return SFZCRYPTO_SUCCESS;
}
#endif /* SFZCRYPTO_CF_ASSET_FREE__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_search
 *
 * Get the id for the asset identified by the given StaticAssetNumber.
 */
#ifdef SFZCRYPTO_CF_ASSET_SEARCH__CM
SfzCryptoStatus
sfzcrypto_cm_asset_search(
        uint32_t StaticAssetNumber,
        SfzCryptoAssetId * const NewAssetId_p)
{
    CMTokens_Response_t t_rsp;
    CMTokens_Command_t t_cmd;
    SfzCryptoStatus status;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (NewAssetId_p == NULL)
        return SFZCRYPTO_BAD_ARGUMENT;

    if (StaticAssetNumber > CMTOKENS_STATIC_ASSET_NUMBER_MAX)
        return SFZCRYPTO_INVALID_PARAMETER;
#endif

    CMTokens_MakeCommand_AssetSearch(&t_cmd, StaticAssetNumber);

    // exchange a message with the CM
    status = CAL_CM_ExchangeToken(&t_cmd, &t_rsp);
    if (status != SFZCRYPTO_SUCCESS)
    {
        return status;
    }

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_rsp);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_rsp, &ErrMsg_p);

            LOG_WARN(
                "sfzcrypto_cm_asset_search: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            // map 'not found' by FW to invalid parameter
            if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
                return SFZCRYPTO_INVALID_PARAMETER;

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    CMTokens_ParseResponse_AssetSearch(&t_rsp, NewAssetId_p, NULL);

    return SFZCRYPTO_SUCCESS;
}
#endif /* SFZCRYPTO_CF_ASSET_SEARCH__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_get_root_key
 *
 * Request the asset id associated with the root key.
 */
#ifdef SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__CM
SfzCryptoAssetId
sfzcrypto_cm_asset_get_root_key(void)
{
    SfzCryptoStatus status;
    SfzCryptoAssetId RootKeyAssetId = 0;

    status = sfzcrypto_cm_asset_search(
                        CALCM_ROOT_KEY_INDEX,
                        &RootKeyAssetId);

    if (status != SFZCRYPTO_SUCCESS)
    {
        LOG_WARN(
            "sfzcrypto_cm_asset_get_root_key: "
            "Failed with status %d\n",
            status);

        return SFZCRYPTO_ASSETID_INVALID;
    }

    return RootKeyAssetId;
}
#endif /* SFZCRYPTO_CF_ASSET_GET_ROOT_KEY__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_import
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given key blob. Use the given KEK and AD to unwrap the key blob.
 */
#ifdef SFZCRYPTO_CF_ASSET_IMPORT__CM
SfzCryptoStatus
sfzcrypto_cm_asset_import(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        KekAssetId == SFZCRYPTO_ASSETID_INVALID ||
        AdditionalData_p == NULL ||
        AdditionalDataSize == 0 ||
        AdditionalDataSize > SFZCRYPTO_KEYBLOB_AAD_MAX_SIZE ||
        KeyBlob_p == NULL ||
        KeyBlobSize - CALCM_KEYBLOB_EXTRA > SFZCRYPTO_ASSET_SIZE_MAX)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif /* CALCM_STRICT_ARGS */

    CMTokens_MakeCommand_AssetLoad_Import(&t_cmd, TargetAssetId, KekAssetId, KeyBlobSize);
    CMTokens_MakeCommand_AssetLoad_SetAad(&t_cmd, AdditionalData_p, AdditionalDataSize);

    return CALCMLib_AssetLoad_Common(&t_cmd, KeyBlob_p, KeyBlobSize, NULL, NULL);
}
#endif /* SFZCRYPTO_CF_ASSET_IMPORT__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_derive
 *
 * Setup the content of the asset referenced by TargetAssetId by deriving it
 * from the given KDK and label info.
 */
#ifdef SFZCRYPTO_CF_ASSET_DERIVE__CM
SfzCryptoStatus
sfzcrypto_cm_asset_derive(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KdkAssetId,
        SfzCryptoOctetsIn * Label_p,
        SfzCryptoSize LabelLen)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_derive: "
        "TargetAssetId=0x%x; KdkAssetId=0x%x, LabelLen=%d\n",
        TargetAssetId,
        KdkAssetId,
        LabelLen);
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        KdkAssetId == SFZCRYPTO_ASSETID_INVALID ||
        Label_p == NULL ||
        LabelLen == 0 ||
        LabelLen > SFZCRYPTO_KDF_LABEL_MAX_SIZE)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    CMTokens_MakeCommand_AssetLoad_Derive(&t_cmd, TargetAssetId, KdkAssetId, 0);
    CMTokens_MakeCommand_AssetLoad_SetAad(&t_cmd, Label_p, LabelLen);

    return CALCMLib_AssetLoad_Common(&t_cmd, NULL, 0, NULL, NULL);
}
#endif /* SFZCRYPTO_CF_ASSET_DERIVE__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_load_key
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given plain data.
 */
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY__CM
SfzCryptoStatus
sfzcrypto_cm_asset_load_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_load_key: "
        "TargetAssetId=0x%x; AssetSize=%d\n",
        TargetAssetId,
        AssetSize);
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        Data_p == NULL ||
        AssetSize == 0 ||
        AssetSize > SFZCRYPTO_ASSET_SIZE_MAX)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif /* CALCM_STRICT_ARGS */

    CMTokens_MakeCommand_AssetLoad_Plaintext(&t_cmd, TargetAssetId, AssetSize);

    return CALCMLib_AssetLoad_Common(&t_cmd, Data_p, AssetSize, NULL, NULL);
}
#endif /* SFZCRYPTO_CF_ASSET_LOAD_KEY__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_gen_key
 *
 * Setup the content of the asset referenced by TargetAssetId with random
 * data obtained from the RNG.
 */
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY__CM
SfzCryptoStatus
sfzcrypto_cm_asset_gen_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_TRACE_ASSETSTORE
    LOG_WARN(
        "%%AS%% "
        "sfzcrypto_cm_asset_gen_key: "
        "TargetAssetId=0x%x; AssetSize=%d\n",
        TargetAssetId,
        AssetSize);
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        AssetSize == 0 ||
        AssetSize > SFZCRYPTO_ASSET_SIZE_MAX)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    CMTokens_MakeCommand_AssetLoad_Random(&t_cmd, TargetAssetId);

    return CALCMLib_AssetLoad_Common(&t_cmd, NULL, 0, NULL, NULL);
}
#endif /* SFZCRYPTO_CF_ASSET_GEN_KEY__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_load_key_and_wrap
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given plain data and export the resulting asset as a key blob.
 * Use the given KEK and AD to create the key blob.
 */
#ifdef SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__CM
SfzCryptoStatus
sfzcrypto_cm_asset_load_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        Data_p == NULL ||
        AssetSize == 0 ||
        AssetSize > SFZCRYPTO_ASSET_SIZE_MAX ||
        KekAssetId == SFZCRYPTO_ASSETID_INVALID ||
        AdditionalData_p == NULL ||
        AdditionalDataSize == 0 ||
        AdditionalDataSize > SFZCRYPTO_KEYBLOB_AAD_MAX_SIZE ||
        KeyBlob_p == NULL ||
        KeyBlobSize_p == NULL ||
        *KeyBlobSize_p < AssetSize + CALCM_KEYBLOB_EXTRA)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    CMTokens_MakeCommand_AssetLoad_Plaintext(&t_cmd, TargetAssetId, AssetSize);
    CMTokens_MakeCommand_AssetLoad_Export(&t_cmd, KekAssetId);
    CMTokens_MakeCommand_AssetLoad_SetAad(&t_cmd, AdditionalData_p, AdditionalDataSize);

    return CALCMLib_AssetLoad_Common(&t_cmd, Data_p, AssetSize, KeyBlob_p, KeyBlobSize_p);
}
#endif /* SFZCRYPTO_CF_ASSET_LOAD_KEY_AND_WRAP__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_gen_key_and_wrap
 *
 * Setup the content of the asset referenced by TargetAssetId with random
 * data obtained from the RNG and export the resulting asset as a key blob.
 * Use the given KEK and AD to create the key blob.
 */
#ifdef SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__CM
SfzCryptoStatus
sfzcrypto_cm_asset_gen_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        AssetSize == 0 ||
        AssetSize > SFZCRYPTO_ASSET_SIZE_MAX ||
        KekAssetId == SFZCRYPTO_ASSETID_INVALID ||
        AdditionalData_p == NULL ||
        AdditionalDataSize == 0 ||
        AdditionalDataSize > SFZCRYPTO_KEYBLOB_AAD_MAX_SIZE ||
        KeyBlob_p == NULL ||
        KeyBlobSize_p == NULL ||
        *KeyBlobSize_p < AssetSize + CALCM_KEYBLOB_EXTRA)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    CMTokens_MakeCommand_AssetLoad_Random(&t_cmd, TargetAssetId);
    CMTokens_MakeCommand_AssetLoad_Export(&t_cmd, KekAssetId);
    CMTokens_MakeCommand_AssetLoad_SetAad(&t_cmd, AdditionalData_p, AdditionalDataSize);

    return CALCMLib_AssetLoad_Common(&t_cmd, NULL, 0, KeyBlob_p, KeyBlobSize_p);
}
#endif /* SFZCRYPTO_CF_ASSET_GEN_KEY_AND_WRAP__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_asset_aesunwrap
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given AES wrapped key blob. Use the given KEK to unwrap the key blob.
 */
#ifdef SFZCRYPTO_CF_ASSET_IMPORT__CM
SfzCryptoStatus
sfzcrypto_cm_asset_aesunwrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize)
{
    CMTokens_Command_t t_cmd;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (TargetAssetId == SFZCRYPTO_ASSETID_INVALID ||
        KekAssetId == SFZCRYPTO_ASSETID_INVALID ||
        KeyBlob_p == NULL ||
        KeyBlobSize > (SFZCRYPTO_ASSET_SIZE_MAX + 64/8))
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif /* CALCM_STRICT_ARGS */

    CMTokens_MakeCommand_AssetLoad_AesUnwrap(&t_cmd, TargetAssetId, KekAssetId, KeyBlobSize);

    return CALCMLib_AssetLoad_Common(&t_cmd, KeyBlob_p, KeyBlobSize, NULL, NULL);
}
#endif /* SFZCRYPTO_CF_ASSET_IMPORT__CM */


/* compile-time verify that the policy bit definitions are compatible */
/* compares CAL-API to CM-Tokens interface */
#define COMPARE_POLICY(_p) \
        COMPILE_GLOBAL_ASSERT( SFZCRYPTO_POLICY_##_p == \
                          CMTOKENS_ASSET_POLICY_##_p)

COMPARE_POLICY(ALGO_CIPHER_AES);
COMPARE_POLICY(ALGO_CIPHER_TRIPLE_DES);
COMPARE_POLICY(ALGO_HMAC_SHA1);
COMPARE_POLICY(ALGO_HMAC_SHA224);
COMPARE_POLICY(ALGO_HMAC_SHA256);
COMPARE_POLICY(ASSET_IV);
COMPARE_POLICY(ASSET_COUNTER);
COMPARE_POLICY(ASSET_TEMP_MAC);
COMPARE_POLICY(FUNCTION_ENCRYPT);
COMPARE_POLICY(FUNCTION_DECRYPT);
COMPARE_POLICY(FUNCTION_MAC);
COMPARE_POLICY(TRUSTED_DERIVE);
COMPARE_POLICY(SECURE_DERIVE);
COMPARE_POLICY(SECURE_WRAP);
COMPARE_POLICY(SECURE_UNWRAP);

/* end of file cal_cm-v2_asset.c */
