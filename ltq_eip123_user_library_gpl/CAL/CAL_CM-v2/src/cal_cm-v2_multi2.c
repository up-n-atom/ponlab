/* cal_cm-v2_multi2.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements functions associated with the Multi2 crypto algorithm.
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

#ifndef CALCM_REMOVE_ALGO_MULTI2

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement

#include "cal_cm-v2_internal.h"    // CAL_CM_ExchangeToken
#include "cal_cm-v2_dma.h"

#include "cm_tokens_crypto.h"
#include "cm_tokens_misc.h"     // CMTokens_MakeCommand_Multi2Config
#include "cm_tokens_errdetails.h"

/*----------------------------------------------------------------------------
 * sfzcrypto_cm_multi2_configure
 */
SfzCryptoStatus
sfzcrypto_cm_multi2_configure(
        uint8_t NumberOfRounds,
        SfzCryptoAssetId SystemKeyAssetId,
        const uint8_t * SystemKey_p)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;
    int res;

#ifdef CALCM_STRICT_ARGS
    {
        bool fNoAsset, fNoSysKey;

        //if (NumberOfRounds > 255)
        //{
        //    return SFZCRYPTO_INVALID_PARAMETER;
        //}

        // either an asset or an explicit SystemKey should be specified.
        fNoAsset = (SystemKeyAssetId == SFZCRYPTO_ASSETID_INVALID);
        fNoSysKey = (SystemKey_p == NULL);
        if ((fNoAsset ^ fNoSysKey) == 0)
        {
            return SFZCRYPTO_INVALID_PARAMETER;
        }

        CMTokens_MakeToken_Clear(&t_cmd);
    }
#endif

    // make command token
    CMTokens_MakeCommand_Multi2Config(
        &t_cmd,
        NumberOfRounds,
        SystemKeyAssetId,
        SystemKey_p);

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
            "sfzcrypto_multi2_configure: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        return SFZCRYPTO_OPERATION_FAILED;
    }

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * CAL_CM_MULTI2
 */
SfzCryptoStatus
CAL_CM_MULTI2(
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
    unsigned int block_size = SFZCRYPTO_MULTI2_BLOCK_LEN;
    bool fEncrypt = false;
    uint8_t Mode = 0;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    // Assets are not supported for Multi2, except for the System Key
    if ((p_key->asset_id != SFZCRYPTO_ASSETID_INVALID) ||
        (p_ctxt->iv_loc != SFZ_IN_CONTEXT))
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    if (direction == SFZ_ENCRYPT)
        fEncrypt = true;

    // check key length
    if (p_key->length != 64/8)
    {
        return SFZCRYPTO_INVALID_KEYSIZE;
    }

    if (data_len & (SFZCRYPTO_MULTI2_BLOCK_LEN - 1))
    {
        return SFZCRYPTO_INVALID_LENGTH;
    }

    switch(p_ctxt->fbmode)
    {
        case SFZCRYPTO_MODE_ECB:
            Mode = CMTOKENS_CRYPTO_MODE_ECB;
            break;

        case SFZCRYPTO_MODE_CBC:
            Mode = CMTOKENS_CRYPTO_MODE_CBC;
            break;

        case SFZCRYPTO_MODE_OFB:
            Mode = CMTOKENS_CRYPTO_MODE_MULTI2_OFB;
            break;

        case SFZCRYPTO_MODE_CFB:
            Mode = CMTOKENS_CRYPTO_MODE_MULTI2_CFB;
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
    CMTokens_MakeCommand_Crypto_MULTI2(&t_cmd, fEncrypt, Mode, data_len);

    // put the key into the token
    CMTokens_MakeCommand_Crypto_CopyKey(&t_cmd, p_key->length, p_key->key);

    // IV
    if (Mode != CMTOKENS_CRYPTO_MODE_ECB)
    {
        // IV in token
        CMTokens_MakeCommand_Crypto_CopyIV(&t_cmd, p_ctxt->iv);
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
                "CAL_CM_MULTI2: "
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
        // Save IV unless ECB Mode
        if (Mode != CMTOKENS_CRYPTO_MODE_ECB)
        {
            CMTokens_ParseResponse_Crypto_CopyIV(&t_res, p_ctxt->iv);
            p_ctxt->iv_loc = SFZ_IN_CONTEXT;
        }
    }

    CALCM_DMA_Free(Task_p);

    return funcres;
}


#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* CALCM_REMOVE_ALGO_MULTI2 */

/* end of file cal_cm-v2_multi2.c */
