/* cal_cm-v2_random.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the Get Random Number and Random Configure functions.
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

#include "cal_cm.h"             // the API to implement

#include "cal_cm-v2_internal.h" // CAL_CM_ExchangeToken
#include "cal_cm-v2_dma.h"

#include "cm_tokens_random.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
* sfzcrypto_cm_random_reseed
*
* This function requests the CM to re-seed the post-processor.
*/
#ifdef SFZCRYPTO_CF_RANDOM_RESEED__CM
SfzCryptoStatus
sfzcrypto_cm_random_reseed(void)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    CMTokens_MakeCommand_PRNG_ReseedNow(&t_cmd);

    // exchange a message with the CM
    {
        SfzCryptoStatus funcres;

        funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
        if (funcres != SFZCRYPTO_SUCCESS)
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
                "sfzcrypto_cm_random_reseed: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    return SFZCRYPTO_SUCCESS;
}
#endif /* SFZCRYPTO_CF_RANDOM_RESEED__CM */


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_rand_data
 *
 * p_rand_num required.
 */
#ifdef SFZCRYPTO_CF_RAND_DATA__CM
SfzCryptoStatus
sfzcrypto_cm_rand_data(
        uint32_t rand_num_size_bytes,
        uint8_t * p_rand_num)
{
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CALCM_DMA_Admin_t * Task_p = NULL;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;

#ifdef CALCM_STRICT_ARGS
    if (p_rand_num == NULL)
        return SFZCRYPTO_BAD_ARGUMENT;

    if (rand_num_size_bytes == 0 ||
        rand_num_size_bytes > EIP123_LIMIT_RANDOM_GENERATE)
    {
        return SFZCRYPTO_INVALID_LENGTH;
    }
#endif

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    CMTokens_MakeCommand_RandomNumber_Generate(
                                &t_cmd,
                                (uint16_t)rand_num_size_bytes);

    funcres = CALAdapter_RandomWrapNvm_PrepareOutput(
                                Task_p,
                                (unsigned int)rand_num_size_bytes,
                                (uint8_t *)p_rand_num,
                                /*fOutputByteCount_Includes_TokenId:*/false);

    if (funcres != SFZCRYPTO_SUCCESS)
    {
        // there was a problem with the output buffer
        LOG_INFO("sfzcrypto_cm_rand_data: Abort after prepare");
        CALCM_DMA_Free(Task_p);
        return funcres;                // ## RETURN ##
    }

    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, true);
    CMTokens_MakeCommand_RandomNumber_Generator_WriteOutDescriptor(
                                        &t_cmd,
                                        &Task_p->OutDescriptor);

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
                "sfzcrypto_cm_rand_data: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            // free the bounce buffers
            CALAdapter_PostDMA(Task_p);

            CALCM_DMA_Free(Task_p);
            return SFZCRYPTO_INTERNAL_ERROR;
        }

        res = CMTokens_ParseResponse_RandomNumber_Generate(&t_res);
        if (res > 0)
        {
            LOG_WARN(
                "sfzcrypto_cm_rand_data: "
                "quality warning=%d \n",
                res);
        }

    }

    // handle post-DMA memory coherency and bounce buffer
    funcres = CALAdapter_RandomWrapNvm_FinalizeOutput(Task_p);

    CALCM_DMA_Free(Task_p);

    return funcres;
}
#endif /* SFZCRYPTO_CF_RAND_DATA__CM */

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

/* end of file cal_cm_random.c */
