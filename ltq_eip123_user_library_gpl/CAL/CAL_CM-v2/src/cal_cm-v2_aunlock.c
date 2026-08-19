/* cal_cm-v2_aunlock.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the authenticated unlock functions.
 */

/*****************************************************************************
* Copyright (c) 2015 INSIDE Secure B.V. All Rights Reserved.
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

#ifdef SFZCRYPTO_CF_AUNLOCK__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                 // the API to implement

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_aunlock.h"
#include "cm_tokens_errdetails.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_authenticated_unlock_start
 */
SfzCryptoStatus
sfzcrypto_cm_authenticated_unlock_start(
        const uint16_t AuthKeyNumber,
        SfzCryptoAssetId * p_AuthStateASId,
        uint8_t * p_Nonce,
        uint32_t * p_NonceLength)
{
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoAssetId KeyAssetId;
    int res;

#ifdef CALCM_STRICT_ARGS
    // note: zero-length data is a valid case
    if (p_AuthStateASId == NULL ||
        p_Nonce == NULL ||
        p_NonceLength  == NULL ||
        *p_NonceLength < 16 ||
        AuthKeyNumber == 0 ||
        AuthKeyNumber >= 63)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    // Get AssetId of specified key Asset Number
    funcres = sfzcrypto_cm_asset_search(AuthKeyNumber, &KeyAssetId);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        return funcres;
    }

    // Allocate AuthState Asset
    funcres = sfzcrypto_cm_asset_alloc(SFZCRYPTO_POLICY_ASSET_AUTHSTATE,
                                       (160/8),
                                       p_AuthStateASId);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        return funcres;
    }

    // Create and send Authenticated Unlock start request
    CMTokens_Command_AUnlock_Start(&t_cmd, *p_AuthStateASId, KeyAssetId);

    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        return funcres;
    }

    // Check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN("sfzcrypto_cm_authenticated_unlock_start: "
                 "Failed with error %d (%s)\n",
                 res, ErrMsg_p);

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    // Copy Nonce from response token
    *p_NonceLength = 16;
    CMTokens_ParseResponse_AUnlock_CopyNonce(&t_res, p_Nonce);

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_authenticated_unlock_verify
 */
SfzCryptoStatus
sfzcrypto_cm_authenticated_unlock_verify(
        const SfzCryptoAssetId AuthStateASId,
        const uint8_t * p_Nonce,
        const uint32_t NonceLength,
        const uint8_t * p_Signature,
        const uint32_t SignatureLength)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    int res;

#ifdef CALCM_STRICT_ARGS
    // note: zero-length data is a valid case
    if (AuthStateASId == SFZCRYPTO_ASSETID_INVALID ||
        p_Nonce == NULL ||
        p_Signature == NULL ||
        NonceLength != 16 ||
        SignatureLength == 0)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
    {
        return SFZCRYPTO_NO_MEMORY;
    }

    // Create and send Authenticated Unlock verify request
    CMTokens_Command_AUnlock_Verify(&t_cmd, AuthStateASId, p_Nonce, SignatureLength);

    // Prepare the input data (signature)
    funcres = CALAdapter_PreDMA(Task_p, 4, SignatureLength, p_Signature, NULL);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        // there was a problem with the input data
        LOG_INFO("sfzcrypto_cm_nop: Abort after prepare");
        CALCM_DMA_Free(Task_p);
        return funcres;
    }
    CMTokens_Command_AUnlock_Verify_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);

    // Exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    CALCM_DMA_Free(Task_p);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        return funcres;
    }

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        // Special return value mapping
        if (res == CMTOKENS_RESULT_SEQ_VERIFY_ERROR)
        {
            return SFZCRYPTO_VERIFY_FAILED;
        }

        LOG_WARN("sfzcrypto_cm_authenticated_unlock_verify: "
                 "Failed with error %d (%s)\n",
                 res, ErrMsg_p);

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_secure_debug
 */
SfzCryptoStatus
sfzcrypto_cm_secure_debug(
        const SfzCryptoAssetId AuthStateASId,
        const bool bSet)
{
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    int res;

#ifdef CALCM_STRICT_ARGS
    // note: zero-length data is a valid case
    if (AuthStateASId == SFZCRYPTO_ASSETID_INVALID)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    // Create and send Authenticated Unlock start request
    CMTokens_Command_SetSecureDebug(&t_cmd, AuthStateASId, bSet);

    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
    {
        return funcres;
    }

    // Check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);
    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        // Special return value mapping
        if (res == CMTOKENS_RESULT_SEQ_VERIFY_ERROR)
        {
            return SFZCRYPTO_VERIFY_FAILED;
        }

        LOG_WARN("sfzcrypto_cm_secure_debug: Failed with error %d (%s)\n",
                 res, ErrMsg_p);

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_AUNLOCK__CM */

/* end of file cal_cm-v2_aunlock.c */
