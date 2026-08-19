/* cal_cm-v2_c2.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements functions associated with the C2 crypto algorithm.
 */

/*****************************************************************************
* Copyright (c) 2011-2015 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef CALCM_REMOVE_ALGO_C2

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                // the API to implement
#include "cal_cm-v2_internal.h"    // CAL_CM_ExchangeToken
#include "cal_cm-v2_dma.h"

#include "cm_tokens_mac.h"
#include "cm_tokens_crypto.h"
#include "cm_tokens_cprm.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * CAL_CM_C2
 */
SfzCryptoStatus
CAL_CM_C2(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;
    unsigned int data_len = src_len;
    unsigned int block_size = SFZCRYPTO_C2_BLOCK_LEN;
    bool fEncrypt = false;
    uint8_t Mode = 0;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    switch (p_ctxt->iv_loc)
    {
        case SFZ_IN_CONTEXT:
            if (p_ctxt->iv_asset_id != SFZCRYPTO_ASSETID_INVALID)
            {
                return SFZCRYPTO_BAD_ARGUMENT;
            }
            break;
        case SFZ_IN_ASSET:
        case SFZ_TO_ASSET:
            if (p_ctxt->fbmode != SFZCRYPTO_MODE_C_CBC ||
                p_ctxt->iv_asset_id == SFZCRYPTO_ASSETID_INVALID)
            {
                return SFZCRYPTO_BAD_ARGUMENT;
            }
            break;
        case SFZ_FROM_ASSET:
            /* Although the CM Firmware supports the return of C-CBC state data
               through the token, CAL never uses that option... */
        default:
            return SFZCRYPTO_INVALID_PARAMETER;
    } // switch

    if (direction == SFZ_ENCRYPT)
        fEncrypt = true;

    // check key length
    if (p_key->length != 56/8)
    {
        return SFZCRYPTO_INVALID_KEYSIZE;
    }

    if (data_len & (SFZCRYPTO_C2_BLOCK_LEN - 1))
    {
        return SFZCRYPTO_INVALID_LENGTH;
    }

    switch(p_ctxt->fbmode)
    {
        case SFZCRYPTO_MODE_ECB:
            Mode = CMTOKENS_CRYPTO_MODE_ECB;
            break;

        case SFZCRYPTO_MODE_C_CBC:
            if (p_ctxt->iv_loc == SFZ_IN_CONTEXT)
            {
                Mode = CMTOKENS_CRYPTO_MODE_C2_C_CBC;
            }
            else
            {
                // when an asset is used, we support C-CBC continuation
                Mode = (p_ctxt->iv_loc == SFZ_TO_ASSET) ?
                        CMTOKENS_CRYPTO_MODE_C2_C_CBC_INIT :
                        CMTOKENS_CRYPTO_MODE_C2_C_CBC_CONT;
            }
            break;

        default:
            return SFZCRYPTO_INVALID_MODE;
    } // switch

    // check size of output buffer
    if (data_len > *p_dst_len)
    {
        *p_dst_len = data_len;
        return SFZCRYPTO_BUFFER_TOO_SMALL;
    }

    *p_dst_len = data_len;

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    // start filling the token
    CMTokens_MakeCommand_Crypto_C2(&t_cmd, fEncrypt, Mode, data_len);

    // key
    if (p_key->asset_id == SFZCRYPTO_ASSETID_INVALID)
    {
        uint8_t aux_keybuf[8];

        c_memcpy(aux_keybuf+1, p_key->key, p_key->length);
        aux_keybuf[0] = 0;
        // put the key into the token
        CMTokens_MakeCommand_Crypto_CopyKey(&t_cmd, p_key->length + 1, aux_keybuf);
        c_memset(aux_keybuf, 0, sizeof(aux_keybuf));
    }
    else
    {
        // key will be taken from asset store
        CMTokens_MakeCommand_Crypto_SetASLoadKey(&t_cmd, p_key->asset_id);
    }

    // IV (actually C-CBC state) handling for C2
    if (p_ctxt->iv_loc == SFZ_IN_ASSET)
    {
        // Let CM load C-CBC state from a asset
        CMTokens_MakeCommand_Crypto_SetASLoadIV(&t_cmd, p_ctxt->iv_asset_id);
    }
    if (p_ctxt->iv_loc != SFZ_IN_CONTEXT)
    {
        CMTokens_MakeCommand_Crypto_SetASSaveIV(&t_cmd, p_ctxt->iv_asset_id);
    }

    // ensure data coherency of the input DMA buffers
    // and fill the input buffer descriptor
    // also bounces input and output, if required
    funcres = CALAdapter_PreDMA(
                        Task_p,
                        block_size,
                        data_len,
                        p_src,
                        p_dst);

    if (funcres != SFZCRYPTO_SUCCESS)
    {
        CALCM_DMA_Free(Task_p);
        return funcres;
    }

    #ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, /*WriteTokenID:*/false);
    #else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, /*WriteTokenID:*/true);
    #endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_Crypto_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);
    CMTokens_MakeCommand_Crypto_WriteOutDescriptor(&t_cmd, &Task_p->OutDescriptor);

    // exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        // free the bounce buffers
        CALAdapter_PostDMA(Task_p);
        CALCM_DMA_Free(Task_p);

        return funcres;
    }

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_res);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

            LOG_WARN(
                "CAL_CM_C2: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            // free the bounce buffers
            CALAdapter_PostDMA(Task_p);
            CALCM_DMA_Free(Task_p);

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    // handle post-DMA memory coherency (also WriteTokenID)
    funcres = CALAdapter_CryptoNopWrap_FinalizeOutput(Task_p);
    if (funcres == SFZCRYPTO_SUCCESS)
    {
        // IV (actually C-CBC state) handling
        if (p_ctxt->iv_loc == SFZ_TO_ASSET)
        {
            p_ctxt->iv_loc = SFZ_IN_ASSET;
        }
    }

    CALCM_DMA_Free(Task_p);

    return funcres;
}


/*----------------------------------------------------------------------------
 * CM_CAL_C2_H
 */
SfzCryptoStatus
CM_CAL_C2_H(
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    uint8_t LastBlock[SFZCRYPTO_C2_BLOCK_LEN];
    int npadbytes = 0;
    uint32_t xlength;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    bool loadIvFromAsset = false;
    bool saveIvInAsset = false;
    int res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    if (p_ctxt->fbmode != SFZCRYPTO_MODE_C2_H)
    {
        return SFZCRYPTO_INVALID_ALGORITHM;     // ## RETURN ##
    }

    // prepare the input data
    c_memset(LastBlock, 0, sizeof(LastBlock));
    if (length == 0)
    {
        // special handling for zero-length message
        LastBlock[0] = 0x80;
        npadbytes = sizeof(LastBlock);
    }
    else // length > 0
    {
        npadbytes = (0-length) & (SFZCRYPTO_C2_BLOCK_LEN-1);
        if (npadbytes != 0)
        {
            if (!final)
            {
                return SFZCRYPTO_INVALID_LENGTH;        // ## RETURN ##
            }
            // split input in N initial blocks followed by one pad block.
            c_memcpy(LastBlock,
                     p_data + length + npadbytes - SFZCRYPTO_C2_BLOCK_LEN,
                     SFZCRYPTO_C2_BLOCK_LEN - npadbytes);
            LastBlock[SFZCRYPTO_C2_BLOCK_LEN - npadbytes] = 0x80;
            length -= (SFZCRYPTO_C2_BLOCK_LEN - npadbytes);
        }
    }

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    // prepare input data for processing by the CM
    {
        uint8_t * lastblock_p = NULL;

        xlength = length;
        if (npadbytes)
        {
            xlength += SFZCRYPTO_C2_BLOCK_LEN;
            lastblock_p = LastBlock;
        }

        // this handles unaligned data, creates the input DMA descriptor
        // and handles memory coherency (commit from cache to system memory)
        funcres = CALAdapter_Mac_PrepareInputData(
                        Task_p,
                        p_data,
                        xlength,
                        lastblock_p,
                        SFZCRYPTO_C2_BLOCK_LEN);

        if (funcres != SFZCRYPTO_SUCCESS)
        {
            // there was a problem with the input data
            LOG_INFO("CAL_CM_C2_H: Abort after prepare");
            CALCM_DMA_Free(Task_p);
            return funcres;     // ## RETURN ##
        }
    }

    // start creation of MAC token
    CMTokens_MakeCommand_Mac_SetLengthAlgoMode(
                                    &t_cmd,
                                    xlength,
                                    CMTOKENS_HASH_ALGORITHM_C2_H,
                                    init,
                                    final);

    // setup input buffer info in MAC token
    CMTokens_MakeCommand_Mac_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);

    // copy mac-to-start-from into the token, or set the Asset Store reference
    if (!loadIvFromAsset)
    {
        // mac-to-start-from in token
        CMTokens_MakeCommand_Mac_CopyDigest(&t_cmd, SFZCRYPTO_C2_BLOCK_LEN, p_ctxt->iv);
    }
    else
    {
        // mac-to-start-from from asset store
        CMTokens_MakeCommand_Mac_SetASLoadDigest(&t_cmd, p_ctxt->iv_asset_id);
    }

    if (saveIvInAsset)
    {
        // digest to asset store
        CMTokens_MakeCommand_Mac_SetASSaveDigest(&t_cmd, p_ctxt->iv_asset_id);
    }

    // copy the key into the token, or set the Asset Store reference
    if (p_key->asset_id == SFZCRYPTO_ASSETID_INVALID)
    {
        // key in token
        CMTokens_MakeCommand_Mac_CopyKey(&t_cmd, p_key->length, p_key->key);
    }
    else
    {
        // key from asset store
        CMTokens_MakeCommand_Mac_SetASLoadKey(&t_cmd,
                                              p_key->asset_id,
                                              p_key->length);
    }

    // use total message length to indicate the number of bytes in the last block
    // (note: this is where MAC for C2 is slightly different from MAC for CMAC!)
    {
        uint32_t lastblocklen = ((SFZCRYPTO_C2_BLOCK_LEN - npadbytes) & (SFZCRYPTO_C2_BLOCK_LEN-1));

        CMTokens_MakeCommand_Mac_SetTotalMessageLength(
                                            &t_cmd,
                                            lastblocklen,
                                            0);
    }

    // exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        CALCM_DMA_Free(Task_p);
        return funcres;
    }

    // if a bounce buffer was used, release it
    CALAdapter_PostDMA(Task_p);
    CALCM_DMA_Free(Task_p);

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "CAL_CM_C2_H: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        // map CM error code to CAL error code
        if (res == CMTOKENS_RESULT_SEQ_INVALID_LENGTH)
            return SFZCRYPTO_INVALID_LENGTH;

        // map CM error code to CAL error code
        if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
            return SFZCRYPTO_OPERATION_FAILED;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // copy digest from response token
    if (!saveIvInAsset)
    {
        CMTokens_ParseResponse_Mac_CopyDigest(
                                        &t_res,
                                        SFZCRYPTO_C2_BLOCK_LEN,
                                        p_ctxt->iv);
        p_ctxt->iv_loc = SFZ_IN_CONTEXT;
    }
    else
    {
        p_ctxt->iv_loc = SFZ_IN_ASSET;
    }

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_cprm_c2_derive
 */
SfzCryptoStatus
sfzcrypto_cm_cprm_c2_derive(
        SfzCryptoCprmC2KeyDeriveFunction  FunctionSelect,
        SfzCryptoAssetId                  AssetIn,
        SfzCryptoAssetId                  AssetIn2,
        SfzCryptoAssetId                  AssetOut,
        SfzCryptoOctetsIn *               InputData_p,
        SfzCryptoSize                     InputDataSize,
        SfzCryptoOctetsOut *              OutputData_p,
        SfzCryptoSize *                   const OutputDataSize_p)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;
    uint32_t OutLenInBytes;
    int FuncSel = (int)FunctionSelect;
    int res;

#ifdef CALCM_TRACE_sfzcrypto_cm_cprm_c2_derive
    Log_FormattedMessageINFO(
        "sfzcrypto_cm_cprm_c2_derive:"
        " FunctionSelect=%u,"
        " AssetIn=%u, AssetIn2=%u, AssetOut=%u,"
        " InputDataSize=%u\n",
        (unsigned int)FunctionSelect,
        (unsigned int)AssetIn,
        (unsigned int)AssetIn2,
        (unsigned int)AssetOut,
        (unsigned int)InputDataSize);
#endif

    if (FuncSel < SFZCRYPTO_CPRM_C2_KZ_DERIVE ||
        FuncSel > SFZCRYPTO_CPRM_C2_KM_DERIVE)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    // make command token
    CMTokens_MakeCommand_C2KeyDeriveCommon(
        &t_cmd,
        FuncSel,
        (uint32_t)AssetIn,
        (uint32_t)AssetOut,
        (uint8_t *)InputData_p,
        InputDataSize);

    if (AssetIn2 != SFZCRYPTO_ASSETID_INVALID)
    {
        CMTokens_MakeCommand_C2KeyDerive_SetAssetIn2(&t_cmd, AssetIn2);
    }

    // exchange tokens with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "sfzcrypto_cprm_c2_derive: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        // map CM error code to CAL error code
        if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
            return SFZCRYPTO_OPERATION_FAILED;
        if (res == CMTOKENS_RESULT_SEQ_VERIFY_ERROR)
            return SFZCRYPTO_VERIFY_FAILED;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // extract OutputData from response token, if any
    CMTokens_ParseResponse_C2KeyDerive(&t_res, &OutLenInBytes);
    if (OutLenInBytes == 0)
    {
        if (OutputDataSize_p != NULL)
            *OutputDataSize_p = 0;

        return SFZCRYPTO_SUCCESS;
    }

    if ((NULL == OutputDataSize_p) || (*OutputDataSize_p < OutLenInBytes))
    {
        return SFZCRYPTO_BUFFER_TOO_SMALL;
    }

    CMTokens_ParseResponse_C2KeyDerive_CopyOutput(
            &t_res,
            OutLenInBytes,
            OutputData_p);

    *OutputDataSize_p = OutLenInBytes;

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_cprm_c2_devicekeyobject_rownr_get
 */
SfzCryptoStatus
sfzcrypto_cm_cprm_c2_devicekeyobject_rownr_get(
        SfzCryptoAssetId DeviceKeyAssetId,
        uint16_t * const RowNumber_p)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;
    int res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);

    if (RowNumber_p == NULL)
        return SFZCRYPTO_INVALID_PARAMETER;
#endif

    // make command token
    CMTokens_MakeCommand_C2KeyInfo(&t_cmd, DeviceKeyAssetId);

    // exchange tokens with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "sfzcrypto_cprm_c2_rownr_get: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        // map CM error code to CAL error code
        if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
            return SFZCRYPTO_OPERATION_FAILED;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // extract Row number from response token
    CMTokens_ParseResponse_C2KeyInfo(&t_res, RowNumber_p);

    return SFZCRYPTO_SUCCESS;
}


#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* CALCM_REMOVE_ALGO_C2 */

/* end of file cal_cm-v2_c2.c */
