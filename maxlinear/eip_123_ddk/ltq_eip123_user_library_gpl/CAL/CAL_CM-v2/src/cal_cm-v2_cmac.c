/* cal_cm-v2_cmac.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the cipher MAC functions.
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

#ifdef SFZCRYPTO_CF_CIPHER_MAC_DATA__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement
#include "cal_sw.h"             // sfzcrypto_sw_cipher_mac_data_s2v

#include "cal_cm-v2_internal.h" // CAL_CM_ExchangeToken
#include "cal_cm-v2_dma.h"

#include "cm_tokens_mac.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_cipher_mac_data
 */
SfzCryptoStatus
sfzcrypto_cm_cipher_mac_data(
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    uint8_t LastBlock[16];
    int npadbytes = 0;
    uint32_t xlength;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    uint8_t MacAlgo = 0;
    uint8_t IV_Bytes = 0;
    bool loadIvFromAsset = false;
    bool saveIvInAsset = false;
    int res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

#ifdef CALCM_STRICT_ARGS
    if (p_ctxt == NULL ||
        p_key == NULL ||
        ((p_data == NULL) && (length > 0)))
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

#ifdef CALCM_TRACE_sfzcrypto_cipher_mac_data
    Log_FormattedMessageINFO(
        "sfzcrypto_cipher_mac_data:"
        " mode=%d"
        " length=%d"
        " keylen=%d"
        " init=%d"
        " final=%d"
        " iv_asset_id=%d"
        " iv_loc=%d"
        "\n",
        p_ctxt->fbmode,
        length,
        p_key->length,
        init,
        final,
        p_ctxt->iv_asset_id,
        p_ctxt->iv_loc);
#endif /* CALCM_TRACE_sfzcrypto_cipher_mac_data */

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

#ifdef CALCM_STRICT_ARGS
    // reject insane input combinations
    if (p_ctxt->iv_asset_id == SFZCRYPTO_ASSETID_INVALID)
    {
        if (loadIvFromAsset || saveIvInAsset)
            return SFZCRYPTO_INVALID_PARAMETER;
    }

    if ((init && loadIvFromAsset) ||
        (final && saveIvInAsset))
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif /* CALCM_STRICT_ARGS */

    // reject zero-length data, unless the goal is to MAC the
    // NULL string in one go.
    if (length == 0 &&
        (!init || !final))
    {
        return SFZCRYPTO_INVALID_LENGTH;
    }

#ifndef CALCM_REMOVE_ALGO_C2
    if (p_key->type == SFZCRYPTO_KEY_C2)
    {
        return CM_CAL_C2_H(
                p_ctxt,
                p_key,
                p_data,
                length,
                init, final);
    }
#endif /* CALCM_REMOVE_ALGO_C2 */

#ifndef CALCM_REMOVE_ALGO_CAMELLIA
    if (p_key->type == SFZCRYPTO_KEY_CAMELLIA)
    {
        switch (p_ctxt->fbmode)
        {
            case SFZCRYPTO_MODE_CBCMAC:
                MacAlgo = CMTOKENS_MAC_ALGORITHM_CAMELLIA_CBCMAC;
                IV_Bytes = 128 / 8;
                break;

            case SFZCRYPTO_MODE_CMAC:
                MacAlgo = CMTOKENS_MAC_ALGORITHM_CAMELLIA_CMAC;
                IV_Bytes = 128 / 8;
                break;

            default:
                return SFZCRYPTO_INVALID_ALGORITHM;     // ## RETURN ##
        } // switch
    } // if KEY_CAMELLIA
#endif /* !CALCM_REMOVE_ALGO_CAMELLIA */

    if (p_key->type == SFZCRYPTO_KEY_AES)
    {
        switch (p_ctxt->fbmode)
        {
            case SFZCRYPTO_MODE_S2V_CMAC:
                return sfzcrypto_sw_cipher_mac_data_s2v(
                                p_ctxt,
                                p_key,
                                p_data,
                                length,
                                init,
                                final);                 // ## RETURN ##

            case SFZCRYPTO_MODE_CBCMAC:
                MacAlgo = CMTOKENS_MAC_ALGORITHM_AES_CBCMAC;
                IV_Bytes = 128 / 8;
                break;

            case SFZCRYPTO_MODE_CMAC:
                MacAlgo = CMTOKENS_MAC_ALGORITHM_AES_CMAC;
                IV_Bytes = 128 / 8;
                break;

            default:
                return SFZCRYPTO_INVALID_ALGORITHM;     // ## RETURN ##
        } // switch
    } // if KEY_AES

    if (MacAlgo == 0)
    {
        return SFZCRYPTO_INVALID_ALGORITHM;         // ## RETURN ##
    }

    // prepare the input data
    c_memset(LastBlock, 0, IV_Bytes);
    if (length == 0)
    {
        if (p_ctxt->fbmode == SFZCRYPTO_MODE_CBCMAC)
        {
            c_memset(p_ctxt->iv, 0, sizeof(p_ctxt->iv));
            return SFZCRYPTO_SUCCESS;               // ## RETURN ##
        }
        LastBlock[0] = 0x80;
        npadbytes = 16;
    }
    else
    {
        // length > 0
        npadbytes = (0-length) & (IV_Bytes-1);
        if (npadbytes != 0)
        {
            if (!final)
            {
                return SFZCRYPTO_INVALID_LENGTH;        // ## RETURN ##
            }
            // split input in N initial blocks followed by one pad block.
            c_memcpy(LastBlock,
                     p_data + length + npadbytes - IV_Bytes,
                     IV_Bytes - npadbytes);
            if (p_ctxt->fbmode == SFZCRYPTO_MODE_CMAC)
                LastBlock[IV_Bytes - npadbytes] = 0x80;
            length -= (IV_Bytes - npadbytes);
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
            xlength += IV_Bytes;
            lastblock_p = LastBlock;
        }

        // this handles unaligned data, creates the input DMA descriptor
        // and handles memory coherency (commit from cache to system memory)
        funcres = CALAdapter_Mac_PrepareInputData(
                        Task_p,
                        p_data,
                        xlength,
                        lastblock_p,
                        IV_Bytes);

        if (funcres != SFZCRYPTO_SUCCESS)
        {
            // there was a problem with the input data
            LOG_INFO("sfzcrypto_cipher_mac_data: Abort after prepare");
            CALCM_DMA_Free(Task_p);
            return funcres;     // ## RETURN ##
        }
    }

    // start creation of MAC token
    CMTokens_MakeCommand_Mac_SetLengthAlgoMode(
                                    &t_cmd,
                                    xlength,
                                    MacAlgo,
                                    init,
                                    final);

    // setup input buffer info in MAC token
    CMTokens_MakeCommand_Mac_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);

    // copy mac-to-start-from into the token, or set the Asset Store reference
    if (!loadIvFromAsset)
    {
        // mac-to-start-from in token
        CMTokens_MakeCommand_Mac_CopyDigest(&t_cmd, IV_Bytes, p_ctxt->iv);
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

    // use total message length to indicate wether padding was used
    if (npadbytes == 16)
        npadbytes -= 1; // NULL-string MAC is special case
    CMTokens_MakeCommand_Mac_SetTotalMessageLength(
                                        &t_cmd,
                                        npadbytes,
                                        0);

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
            "sfzcrypto_cipher_mac_data: "
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
                                        IV_Bytes,
                                        p_ctxt->iv);
        p_ctxt->iv_loc = SFZ_IN_CONTEXT;
    }
    else
    {
        p_ctxt->iv_loc = SFZ_IN_ASSET;
    }

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_CIPHER_MAC_DATA__CM */

/* end of file cal_cm-v2_cmac.c */
