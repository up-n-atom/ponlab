/* cal_cm-v2_arc4.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements ARC4.
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

#ifndef CALCM_REMOVE_ALGO_ARCFOUR

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm-v2_internal.h"        // CAL_CM_ExchangeToken + the API to implement
#include "cal_cm-v2_dma.h"

#include "cm_tokens_crypto.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * CAL_ARC4_CreateState
 */
static void
CAL_ARC4_CreateState(
        unsigned int KeyLength,
        uint8_t * KeyBytes_p,
        uint8_t * keystream)
{
    unsigned int x, y, k;

    for (x = 0; x <= 255; x++)
        keystream[x] = (uint8_t)x;

    k = 0;
    y = 0;
    for (x = 0; x <= 255; x++)
    {
        uint8_t m = keystream[x];
        y = y + m + KeyBytes_p[k];
        y &= 255;
        keystream[x] = keystream[y];
        keystream[y] = m;

        k++;
        if (k >= KeyLength)
            k = 0;
    } // for
}


/*----------------------------------------------------------------------------
 * CAL_CM_ARC4
 */
SfzCryptoStatus
CAL_CM_ARC4(
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
    bool fEncrypt = false;
    uint32_t ARC4BufAddr = 0;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    if (direction == SFZ_ENCRYPT)
        fEncrypt = true;

    // ARC4 supports keys between 8 and 2048 bits, in steps of 8
    if ((p_key->length < 8/8) || (p_key->length > 2048/8))
        return SFZCRYPTO_INVALID_KEYSIZE;

    // check size of output buffer
    if (src_len > *p_dst_len)
    {
        *p_dst_len = src_len;
        return SFZCRYPTO_BUFFER_TOO_SMALL;
    }

    *p_dst_len = src_len;

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    // create the ARC4 state from the key
    if (p_ctxt->fbmode == (SfzCryptoModeType)SFZCRYPTO_MODE_ARC4_STATELESS ||
        p_ctxt->fbmode == (SfzCryptoModeType)SFZCRYPTO_MODE_ARC4_INITIAL)
    {
        CAL_ARC4_CreateState(
                    p_key->length,
                    p_key->key,
                    p_ctxt->ARCFOUR_state.keystream);

        // note: pre-incremented i as required by CM hardware
        p_ctxt->ARCFOUR_state.i = 1;
        p_ctxt->ARCFOUR_state.j = 0;
    }

    // start filling the token
    CMTokens_MakeCommand_Crypto_ARC4(
                        &t_cmd,
                        fEncrypt,
                        src_len,
                        p_ctxt->ARCFOUR_state.i,
                        p_ctxt->ARCFOUR_state.j);

    // ensure data coherency of the input DMA buffers
    // and fill the input buffer descriptor
    // also bounces input and output, if required
    funcres = CALAdapter_PreDMA_ARC4(
                        Task_p,
                        /*Algo block size:*/EIP123_ALGOBLOCKSIZE_ARC4,
                        src_len,
                        p_src,
                        p_dst,
                        p_ctxt->ARCFOUR_state.keystream,
                        &ARC4BufAddr);

    if (funcres != SFZCRYPTO_SUCCESS)
    {
        CALCM_DMA_Free(Task_p);
        return funcres;
    }

    #ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, /*WriteTokenID:*/false);
    #else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, /*WriteTokenID:*/true);
    #endif  /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    CMTokens_MakeCommand_Crypto_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);
    CMTokens_MakeCommand_Crypto_WriteOutDescriptor(&t_cmd, &Task_p->OutDescriptor);
    CMTokens_MakeCommand_Crypto_WriteStateDescriptor(&t_cmd, ARC4BufAddr);

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
                "CAL_CM_ARC4: "
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

    CMTokens_ParseResponse_Crypto_ARC4_ij(
                        &t_res,
                        &p_ctxt->ARCFOUR_state.i,
                        &p_ctxt->ARCFOUR_state.j);

    CALCM_DMA_Free(Task_p);

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* CALCM_REMOVE_ALGO_ARCFOUR */

/* end of file cal_cm-v2_arc4.c */
