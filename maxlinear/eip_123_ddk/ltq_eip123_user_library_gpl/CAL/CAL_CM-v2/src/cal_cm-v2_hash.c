/* cal_cm-v2_hash.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the Hash functions.
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

#ifdef SFZCRYPTO_CF_HASH_DATA__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                 // the API to implement

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_hash.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_hash_data
 */
SfzCryptoStatus
sfzcrypto_cm_hash_data(
        SfzCryptoHashContext * const p_ctxt,
        uint8_t * p_data,
        uint32_t length,
        bool init_with_default,
        bool final)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    uint8_t HashAlgo = 0;
    uint8_t DigestNBytes = 0;
    int res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

#ifdef CALCM_TRACE_sfzcrypto_cm_hash_data
    Log_FormattedMessageINFO(
        "sfzcrypto_cm_hash_data:"
        " algo=%d"
        " init=%d"
        " final=%d"
        " length=%d"
        "\n",
        p_ctxt->algo,
        init_with_default,
        final,
        length);
#endif /* CALCM_TRACE_sfzcrypto_cm_hash_data */

#ifdef CALCM_STRICT_ARGS
    // note: zero-length data is a valid case
    if (p_ctxt == NULL ||
        p_data == NULL)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    switch (p_ctxt->algo)
    {
        case SFZCRYPTO_ALGO_HASH_MD5:
            HashAlgo = CMTOKENS_HASH_ALGORITHM_MD5;
            DigestNBytes = 128 / 8;
            break;

        case SFZCRYPTO_ALGO_HASH_SHA160:
            HashAlgo = CMTOKENS_HASH_ALGORITHM_SHA160;
            DigestNBytes = 160 / 8;
            break;

        case SFZCRYPTO_ALGO_HASH_SHA224:
            HashAlgo = CMTOKENS_HASH_ALGORITHM_SHA224;
            DigestNBytes = 256 / 8;
            // note: intermediate digest is always 256 bits, final is 224 bits
            break;

        case SFZCRYPTO_ALGO_HASH_SHA256:
            HashAlgo = CMTOKENS_HASH_ALGORITHM_SHA256;
            DigestNBytes = 256 / 8;
            break;

        default:
            return SFZCRYPTO_INVALID_ALGORITHM;     // ## RETURN ##
    } // switch

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    CMTokens_MakeCommand_Hash_SetLengthAlgoMode(
                                    &t_cmd,
                                    length,
                                    HashAlgo,
                                    init_with_default,
                                    final);

    // prepare the input data (common function with HMAC)
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
            LOG_INFO("sfzcrypto_cm_hash_data: Abort after prepare");
            CALCM_DMA_Free(Task_p);
            return funcres;     // ## RETURN ##
        }

        CMTokens_MakeCommand_Hash_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);
    }

    // copy the Digest into the token
    CMTokens_MakeCommand_Hash_CopyDigest(&t_cmd, DigestNBytes, p_ctxt->digest);

    // handle the total message length counter
    if (init_with_default)
    {
        p_ctxt->count[0] = 0;
        p_ctxt->count[1] = 0;
    }

    // increment total message length
    // (important to do _before_ operation when final)
    p_ctxt->count[0] += length;
    if (p_ctxt->count[0] < length)
        p_ctxt->count[1]++;

    CMTokens_MakeCommand_Hash_SetTotalMessageLength(
                                        &t_cmd,
                                        p_ctxt->count[0],
                                        p_ctxt->count[1]);

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
    Task_p = NULL;

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "sfzcrypto_cm_hash_data: "
            "Failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        // special return value mapping for hash
        if (res == CMTOKENS_RESULT_SEQ_INVALID_LENGTH)
            return SFZCRYPTO_INVALID_LENGTH;

        if (res == CMTOKENS_RESULT_SEQ_ENGINE_NOT_AVAILABLE)
            return SFZCRYPTO_UNSUPPORTED;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // copy digest from response token
    if (final)
    {
        // SHA224 has 256 bit intermediate digest, but 224 bit final digest
        if (p_ctxt->algo == SFZCRYPTO_ALGO_HASH_SHA224)
            DigestNBytes = 224 / 8;
    }

    CMTokens_ParseResponse_Hash_CopyDigest(
                                    &t_res,
                                    DigestNBytes,
                                    p_ctxt->digest);

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_HASH_DATA__CM */

/* end of file cal_cm-v2_hash.c */
