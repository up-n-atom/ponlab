/* cal_cm-v2_hmac.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the HMAC functions.
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

#include "c_cal_cm-v2.h"

#ifdef SFZCRYPTO_CF_HMAC_DATA__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement + sfzcrypto_cm_hash_data

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_mac.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_hmac_data
 */
SfzCryptoStatus
sfzcrypto_cm_hmac_data(
        SfzCryptoHmacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    uint8_t MacAlgo = 0;
    uint8_t DigestNBytes = 0;
    bool loadDigestFromAsset = false;
    bool saveDigestInAsset = false;
    int res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

#ifdef CALCM_STRICT_ARGS
    if (p_ctxt == NULL ||
        /*p_key == NULL ||  key is optional except for init and final operations! */
        p_data == NULL)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

#ifdef CALCM_TRACE_sfzcrypto_cm_hmac_data
    if (p_key)
    {
        Log_FormattedMessageINFO(
            "sfzcrypto_cm_hmac_data:"
            " algo=%d"
            " length=%d"
            " keylen=%d"
            " keytype=%d"
            " key_assetid=0x%x"
            " init=%d"
            " final=%d"
            " mac_asset_id=0x%x"
            " mac_loc=%d"
            "\n",
            p_ctxt->hashCtx.algo,
            length,
            p_key->length,
            p_key->type,
            p_key->asset_id,
            init,
            final,
            p_ctxt->mac_asset_id,
            p_ctxt->mac_loc);
    }
    else
    {
        Log_FormattedMessageINFO(
            "sfzcrypto_cm_hmac_data:"
            " algo=%d"
            " length=%d"
            " keylen=(no key)"
            " init=%d"
            " final=%d"
            " mac_asset_id=0x%x"
            " mac_loc=%d"
            "\n",
            p_ctxt->hashCtx.algo,
            length,
            init,
            final,
            p_ctxt->mac_asset_id,
            p_ctxt->mac_loc);
    }
#endif /* CALCM_TRACE_sfzcrypto_cm_hmac_data */

    switch (p_ctxt->mac_loc)
    {
        case SFZ_IN_CONTEXT:
            break;
        case SFZ_IN_ASSET:
            loadDigestFromAsset = true;
            // fall through
        case SFZ_TO_ASSET:
            saveDigestInAsset = true;
            break;
        case SFZ_FROM_ASSET:
            loadDigestFromAsset = true;
            break;
        default:
            return SFZCRYPTO_INVALID_PARAMETER;
    } // switch
    p_ctxt->mac_loc &= BIT_0;

#ifdef CALCM_STRICT_ARGS
    if (p_ctxt->mac_asset_id == SFZCRYPTO_ASSETID_INVALID)
    {
        if (loadDigestFromAsset || saveDigestInAsset)
            return SFZCRYPTO_INVALID_PARAMETER;
    }

    // inconsistent input
    if ((init && loadDigestFromAsset) ||
        (final && saveDigestInAsset))
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    // key is mandatory except for a continuation
    if (init || final)
    {
        // ensure key is present
        if (p_key == NULL)
            return SFZCRYPTO_INVALID_PARAMETER;

        if (p_key->type != SFZCRYPTO_KEY_HMAC)
        {
            LOG_WARN("Not an HMAC key\n");
            return SFZCRYPTO_INVALID_PARAMETER;
        }
    }
#endif /* CALCM_STRICT_ARGS */

    // note: zero-length data is a valid case

    switch (p_ctxt->hashCtx.algo)
    {
        case SFZCRYPTO_ALGO_HASH_MD5:
            MacAlgo = CMTOKENS_MAC_ALGORITHM_HMAC_MD5;
            DigestNBytes = 128 / 8;
            break;

        case SFZCRYPTO_ALGO_HASH_SHA160:
            MacAlgo = CMTOKENS_MAC_ALGORITHM_HMAC_SHA160;
            DigestNBytes = 160 / 8;
            break;

        case SFZCRYPTO_ALGO_HASH_SHA224:
            MacAlgo = CMTOKENS_MAC_ALGORITHM_HMAC_SHA224;
            DigestNBytes = 256 / 8;
            // note: intermediate digest is always 256 bits, final is 224 bits
            break;

        case SFZCRYPTO_ALGO_HASH_SHA256:
            MacAlgo = CMTOKENS_MAC_ALGORITHM_HMAC_SHA256;
            DigestNBytes = 256 / 8;
            break;

        default:
            return SFZCRYPTO_INVALID_ALGORITHM;     // ## RETURN ##
    } // switch

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    CMTokens_MakeCommand_Mac_SetLengthAlgoMode(
                                    &t_cmd,
                                    length,
                                    MacAlgo,
                                    init,
                                    final);

    // copy the Digest into the token, or set the Asset Store reference
    if (!loadDigestFromAsset && !init)
    {
        // copy intermediate MAC from hashCtx.digest to token
        CMTokens_MakeCommand_Mac_CopyDigest(&t_cmd, DigestNBytes,
                                            p_ctxt->hashCtx.digest);
    }
    else
    {
        if (!init)
        {
            // IV from asset store
            CMTokens_MakeCommand_Mac_SetASLoadDigest(&t_cmd, p_ctxt->mac_asset_id);
        }
    }

    if (saveDigestInAsset)
    {
        // digest to asset store
        CMTokens_MakeCommand_Mac_SetASSaveDigest(&t_cmd, p_ctxt->mac_asset_id);
    }

    // copy the key into the token, or set the Asset Store reference
    // key is optional, except for Initial and Final operations
    if (init || final)
    {
        if (p_key->asset_id == SFZCRYPTO_ASSETID_INVALID)
        {
            // key in token
            if (p_key->length <= 64)
            {
                CMTokens_MakeCommand_Mac_CopyKey(&t_cmd, p_key->length, p_key->key);
            }
            else
            {
                // key is too long, we need to hash it first
                SfzCryptoHashContext HashCtx = { 0 };

                HashCtx.algo = p_ctxt->hashCtx.algo;

                funcres = sfzcrypto_cm_hash_data(
                                    &HashCtx,
                                    p_key->key,
                                    p_key->length,
                                    /*init:*/true,
                                    /*final:*/true);

                if (funcres != SFZCRYPTO_SUCCESS)
                {
                    LOG_WARN(
                        "sfzcrypto_cm_hmac_data: "
                        "Hash long key failed with error %d\n",
                        funcres);

                    CALCM_DMA_Free(Task_p);
                    return SFZCRYPTO_INTERNAL_ERROR;
                }

                // copy the key digest into the token
                {
                    unsigned int DigestLen = DigestNBytes;

                    if (MacAlgo == CMTOKENS_MAC_ALGORITHM_HMAC_SHA224)
                        DigestLen = 224 / 8;

                    CMTokens_MakeCommand_Mac_CopyKey(&t_cmd, DigestLen, HashCtx.digest);
                }
            }
        }
        else
        {
            // key from asset store
            CMTokens_MakeCommand_Mac_SetASLoadKey(&t_cmd, p_key->asset_id, p_key->length);
        }
    }

    // handle the total message length counter
    if (init)
    {
        p_ctxt->hashCtx.count[0] = 0;
        p_ctxt->hashCtx.count[1] = 0;
    }

    // increment total message length
    // (important to do _before_ operation when final)
    p_ctxt->hashCtx.count[0] += length;
    if (p_ctxt->hashCtx.count[0] < length)
        p_ctxt->hashCtx.count[1]++;

    CMTokens_MakeCommand_Mac_SetTotalMessageLength(
                                        &t_cmd,
                                        p_ctxt->hashCtx.count[0],
                                        p_ctxt->hashCtx.count[1]);

    // prepare the input data (common function with Hash)
    if (length > 0)
    {
        // this handles unaligned data, creates the input DMA descriptor
        // and handles memory coherency (commit from cache to system memory)
        funcres = CALAdapter_HashHmacLoad_PrepareInputData(
                        Task_p,
                        p_data,
                        length);

        if (funcres != SFZCRYPTO_SUCCESS)
        {
            // there was a problem with the input data
            LOG_INFO("sfzcrypto_cm_hmac_data: Abort after prepare");
            CALCM_DMA_Free(Task_p);
            return funcres;     // ## RETURN ##
        }

        CMTokens_MakeCommand_Mac_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);
    }

    // exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

    // if a bounce buffer was used, release it
    CALAdapter_PostDMA(Task_p);
    CALCM_DMA_Free(Task_p);
    Task_p = NULL;

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "sfzcrypto_cm_hmac_data: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        // map CM-specific errors to CAL errors
        if (res == CMTOKENS_RESULT_SEQ_INVALID_LENGTH)
            return SFZCRYPTO_INVALID_LENGTH;

        if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
            return SFZCRYPTO_INVALID_PARAMETER;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // copy digest from response token
    if (!saveDigestInAsset)
    {
        // SHA224 has 256 bit intermediate digest, but 224 bit final digest
        if (final)
            if (p_ctxt->hashCtx.algo == SFZCRYPTO_ALGO_HASH_SHA224)
                DigestNBytes = 224 / 8;

        CMTokens_ParseResponse_Mac_CopyDigest(
                                        &t_res,
                                        DigestNBytes,
                                        p_ctxt->hashCtx.digest);

        p_ctxt->mac_loc = SFZ_IN_CONTEXT;
    }
    else
    {
        p_ctxt->mac_loc = SFZ_IN_ASSET;
    }

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_HMAC_DATA__CM */

/* end of file cal_cm-v2_hmac.c */
