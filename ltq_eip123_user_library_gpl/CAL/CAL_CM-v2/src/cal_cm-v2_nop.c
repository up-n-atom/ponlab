/* cal_cm-v2_nop.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the NOP crypto aka Data Copy.
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

#ifdef SFZCRYPTO_CF_NOP__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_nop.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_nop
 *
 * Uses the NOP token to copy data from SRC to DST.
 * Caller should be aware that a 32bit multiple will be copied:
 * - The length will be rounded up
 * - A few more bytes can be read from SRC (typically OK)
 * - A few more bytes can be written to DST
 */
SfzCryptoStatus
sfzcrypto_cm_nop(
        SfzCryptoOctetsOut * p_dst,
        SfzCryptoOctetsIn * p_src,
        uint32_t len)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    uint32_t DataLength;

#ifdef CALCM_STRICT_ARGS
    if (p_dst == NULL ||
        p_src == NULL ||
        len == 0)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }
#endif

    // the DMA can only transfer 32-bit multiples
    // so round up "len" to next multiple of 32bits
    DataLength = len + (3 & (4 - (len & 3)));

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    CMTokens_MakeCommand_Nop(&t_cmd, DataLength);

    // perform the pre-DMA tasks, then write the IN token
    funcres = CALAdapter_PreDMA(
                        Task_p,
                        /*AlgorithmicBlockSize:*/4,
                        len,
                        p_src,
                        p_dst);

    if (funcres != SFZCRYPTO_SUCCESS)
    {
        // there was a problem with the input data
        LOG_INFO("sfzcrypto_cm_nop: Abort after prepare");
        CALCM_DMA_Free(Task_p);
        return funcres;     // ## RETURN ##
    }

    #ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, /*WriteTokenID:*/false);
    #else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, true);
    #endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_Nop_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);
    CMTokens_MakeCommand_Nop_WriteOutDescriptor(&t_cmd, &Task_p->OutDescriptor);

    // exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
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
                "sfzcrypto_cm_nop: "
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

    CALCM_DMA_Free(Task_p);

    return funcres;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_NOP__CM */

/* end of file cal_cm-v2_nop.c */
