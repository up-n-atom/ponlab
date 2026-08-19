/* cal_cm-v2_aesdes.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the AES and DES crypto.
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

#include "c_cal_cm-v2.h"

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm-v2_internal.h"        // CAL_CM_ExchangeToken + the API to implement
#include "cal_cm-v2_dma.h"

#include "cm_tokens_crypto.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * CAL_CM_AESDES
 */
SfzCryptoStatus
CAL_CM_AESDES(
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
    unsigned int block_size = SFZCRYPTO_DES_BLOCK_LEN;
    bool fEncrypt = false;
    bool loadIvFromAsset = false;
    bool saveIvInAsset = false;
    uint8_t Mode = 0;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    switch (p_ctxt->iv_loc)
    {
        case SFZ_IN_CONTEXT:
            break;
        case SFZ_IN_ASSET:
            loadIvFromAsset = true;
            // fall through
        case SFZ_TO_ASSET:
            saveIvInAsset = true;
            break;
        case SFZ_FROM_ASSET:
            loadIvFromAsset = true;
            break;
        default:
            return SFZCRYPTO_INVALID_PARAMETER;
    } // switch
    p_ctxt->iv_loc &= BIT_0;

    if (p_key->type == SFZCRYPTO_KEY_AES)
        block_size = SFZCRYPTO_AES_BLOCK_LEN;

    if (direction == SFZ_ENCRYPT)
        fEncrypt = true;

    // key size errors to be reported after checking KR entry
    switch(p_key->type)
    {
        case SFZCRYPTO_KEY_DES:
            // DES supports 56-bit keys stored in 64-bits only
            if (p_key->length != 64/8)
                return SFZCRYPTO_INVALID_KEYSIZE;
            break;

        case SFZCRYPTO_KEY_TRIPLE_DES:
            // 3DES keys must be 3xDES keys
            if (p_key->length != 3*64/8)
                return SFZCRYPTO_INVALID_KEYSIZE;
            break;

        case SFZCRYPTO_KEY_AES:
            // AES requires keys of length 128, 192 or 256 bits
            if ((p_key->length != 128/8) &&
                (p_key->length != 192/8) &&
                (p_key->length != 256/8))
            {
                return SFZCRYPTO_INVALID_KEYSIZE;
            }

            if (SFZCRYPTO_MODE_CTR == p_ctxt->fbmode ||
                SFZCRYPTO_MODE_ICM == p_ctxt->fbmode)
            {
                // the HW AES engine accepts only block length data
                // even for modes like CTR and ICM. So lets round up the length
                // the caller must of course provide the extra space (up to 15 bytes)
                if ((data_len % 16) != 0)
                {
                    data_len = ((data_len + 15) / 16) * 16;
                    LOG_INFO("sfzcrypto_hw_symm_crypt: "
                             "padding to block length %d -> %d\n",
                             src_len, data_len);
                }

                // this length will be used in the buffer descriptors and for the token
            }
            break;

        default:
            return SFZCRYPTO_INVALID_ALGORITHM;

    } // switch

    if (data_len % block_size)
        return SFZCRYPTO_INVALID_LENGTH;

    if (p_key->type == SFZCRYPTO_KEY_DES ||
        p_key->type == SFZCRYPTO_KEY_TRIPLE_DES)
    {
        if (p_ctxt->fbmode != SFZCRYPTO_MODE_ECB &&
            p_ctxt->fbmode != SFZCRYPTO_MODE_CBC)
        {
            return SFZCRYPTO_INVALID_MODE;
        }
    }

    // check size of output buffer
    if (data_len > *p_dst_len)
    {
        *p_dst_len = data_len;
        return SFZCRYPTO_BUFFER_TOO_SMALL;
    }

    *p_dst_len = data_len;

    switch(p_ctxt->fbmode)
    {
        case SFZCRYPTO_MODE_ECB:
            Mode = CMTOKENS_CRYPTO_MODE_ECB;
            break;

        case SFZCRYPTO_MODE_CBC:
            Mode = CMTOKENS_CRYPTO_MODE_CBC;
            break;

        case SFZCRYPTO_MODE_CTR:
            Mode = CMTOKENS_CRYPTO_MODE_CTR;
            break;

        case SFZCRYPTO_MODE_ICM:
            Mode = CMTOKENS_CRYPTO_MODE_AES_ICM;
            break;

        default:
            return SFZCRYPTO_INVALID_MODE;
    } // switch

    // start filling the token
    if (p_key->type == SFZCRYPTO_KEY_AES)
    {
        CMTokens_MakeCommand_Crypto_AES(&t_cmd, fEncrypt, Mode, data_len);
        CMTokens_MakeCommand_Crypto_AES_SetKeyLength(&t_cmd, p_key->length);
    }
    else
    {
        bool fDES = false;

        if (p_key->type == SFZCRYPTO_KEY_DES)
            fDES = true;

        CMTokens_MakeCommand_Crypto_3DES(&t_cmd, fDES, fEncrypt, Mode, data_len);
    }

    // key
    if (p_key->asset_id == SFZCRYPTO_ASSETID_INVALID)
    {
        // put the key into the token
        CMTokens_MakeCommand_Crypto_CopyKey(&t_cmd, p_key->length, p_key->key);
    }
    else
    {
        // key will be taken from asset store
        CMTokens_MakeCommand_Crypto_SetASLoadKey(&t_cmd, p_key->asset_id);
    }

    // IV
    if (Mode != CMTOKENS_CRYPTO_MODE_ECB)
    {
        if (!loadIvFromAsset)
        {
            // IV in token
            CMTokens_MakeCommand_Crypto_CopyIV(&t_cmd, p_ctxt->iv);
        }
        else
        {
            // IV from Asset Store
            CMTokens_MakeCommand_Crypto_SetASLoadIV(&t_cmd, p_ctxt->iv_asset_id);
        }

        if (saveIvInAsset)
            CMTokens_MakeCommand_Crypto_SetASSaveIV(&t_cmd, p_ctxt->iv_asset_id);
    }

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

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
                "CAL_CM_AESDES: "
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
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        CALCM_DMA_Free(Task_p);
        return funcres;
    }

    if (!saveIvInAsset)
    {
        CMTokens_ParseResponse_Crypto_CopyIV(&t_res, p_ctxt->iv);
        p_ctxt->iv_loc = SFZ_IN_CONTEXT;
    }
    else
    {
        p_ctxt->iv_loc = SFZ_IN_ASSET;
    }

    CALCM_DMA_Free(Task_p);

    return SFZCRYPTO_SUCCESS;
}


/* end of file cal_cm-v2_aesdes.c */
