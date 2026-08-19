/* cal_cm-v2_nvm.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the NVM read API.
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

#ifdef SFZCRYPTO_CF_NVM_PUBLICDATA_READ__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"             // the API to implement

#include "cal_cm-v2_internal.h"    // CAL_CM_ExchangeToken
#include "cal_cm-v2_dma.h"

#include "cm_tokens_misc.h"
#include "cm_tokens_asset.h"
#include "cm_tokens_errdetails.h"

#define CAL_CM_NVM_DATASIZE_MAX 512   // must be a multiple of 4
static uint8_t CAL_CM_nvm_data[CAL_CM_NVM_DATASIZE_MAX];

/*----------------------------------------------------------------------------
 * CAL_CM_get_hw_nvmdata
 *
 * This function reads all NVM data from the CM into the global buffer named
 * CAL_CM_nvm_data and returns the length of the NVM data.
 */
static SfzCryptoStatus
CAL_CM_get_hw_nvmdata(
        SfzCryptoAssetId StaticAssetId,
        uint32_t * const p_len)
{
#define DUMMY_ASSETID 0xa55e71d

    uint32_t datalen = sizeof(CAL_CM_nvm_data);
    CALCM_DMA_Admin_t * Task_p = NULL;
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;

    *p_len = 0;

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return SFZCRYPTO_NO_MEMORY;

    CMTokens_MakeToken_Clear(&t_cmd);
    CMTokens_MakeCommand_NVM_Read(&t_cmd, StaticAssetId, datalen);
    // FW will write TokenID at last 4 words of buffer

    // share code with "random_generate".
    // this function allocates a bounce buffer for DataLength+4
    // bytes, so that it also can hold the tokenID.
    funcres = CALAdapter_RandomWrapNvm_PrepareOutput(
                        Task_p,
                        datalen,
                        CAL_CM_nvm_data,
                        /*fOutputByteCount_Includes_TokenId:*/true);

    if (funcres != SFZCRYPTO_SUCCESS)
    {
        // there was a problem with the input data
        LOG_INFO("CAL_CM_get_hw_nvmdata: Abort after prepare");
        CALCM_DMA_Free(Task_p);
        return funcres;     // ## RETURN ##
    }

    CMTokens_MakeCommand_SetTokenID(&t_cmd, CAL_TOKENID_VALUE, true);
    CMTokens_MakeCommand_NVM_Read_WriteOutDescriptor(&t_cmd, &Task_p->OutDescriptor);

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

            // free the bounce buffers
            CALAdapter_PostDMA(Task_p);
            CALCM_DMA_Free(Task_p);

            // special situation: NVM object is not Public Data
            if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
                return SFZCRYPTO_OPERATION_FAILED;

            LOG_WARN(
                "CAL_CM_get_hw_nvmdata: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    // handle post-DMA memory coherency (also WriteTokenID)
    funcres = CALAdapter_RandomWrapNvm_FinalizeOutput(Task_p);

    if (funcres == SFZCRYPTO_SUCCESS)
        CMTokens_ParseResponse_NVM_Read(&t_res, p_len);

    CALCM_DMA_Free(Task_p);

    return funcres;

#undef DUMMY_ASSETID
}


/*----------------------------------------------------------------------------
 * CAL_CM_find_nvm_object
 */
static SfzCryptoStatus
CAL_CM_find_nvm_object(
        uint8_t ObjectNr,
        uint32_t * const AssetId_p,
        uint32_t * const DataLen_p)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    SfzCryptoStatus funcres;

    *AssetId_p = SFZCRYPTO_ASSETID_INVALID;

    CMTokens_MakeToken_Clear(&t_cmd);
    CMTokens_MakeCommand_AssetSearch(&t_cmd, (uint8_t)ObjectNr);

    // exchange a message with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

    // check for errors
    {
        int res;

        res = CMTokens_ParseResponse_Generic(&t_res);

        if (res != 0)
        {
            const char * ErrMsg_p;

            res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

            // special situation: object not found
            if (res == CMTOKENS_RESULT_SEQ_INVALID_ASSET)
                return SFZCRYPTO_INVALID_PARAMETER;

            LOG_WARN(
                "CAL_CM_find_nvm_object: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    CMTokens_ParseResponse_AssetSearch(&t_res, AssetId_p, DataLen_p);

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_nvm_publicdata_read
 *
 * Uses the NVM_Read token to retrieve NVM data from the CM.
 */
SfzCryptoStatus
sfzcrypto_cm_nvm_publicdata_read(
        uint32_t ObjectNr,
        SfzCryptoOctetsOut * Data_p,
        uint32_t * const DataLen_p)
{
    SfzCryptoStatus funcres = SFZCRYPTO_SUCCESS;
    SfzCryptoAssetId AssetId = SFZCRYPTO_ASSETID_INVALID;
    uint32_t FoundLen = 0;
    uint32_t datalen;

#ifdef CALCM_STRICT_ARGS
    if (DataLen_p == NULL)
        return SFZCRYPTO_INVALID_PARAMETER;

    if (ObjectNr > 255)
        return SFZCRYPTO_INVALID_PARAMETER;
#endif /* CALCM_STRICT_ARGS */

    funcres = CAL_CM_find_nvm_object((uint8_t)ObjectNr, &AssetId, &FoundLen);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

    // return error code when NVM object was not found
    if (AssetId == SFZCRYPTO_ASSETID_INVALID)
        return SFZCRYPTO_INVALID_PARAMETER;

    if (Data_p == NULL)
    {
        // only update the length
        *DataLen_p = FoundLen;
        return SFZCRYPTO_SUCCESS;
    }

    funcres = CAL_CM_get_hw_nvmdata(AssetId, &datalen);
    if (funcres != SFZCRYPTO_SUCCESS)
        return funcres;

#ifdef CALCM_STRICT_ARGS
    if (*DataLen_p < 1)
        return SFZCRYPTO_INVALID_PARAMETER;
#endif /* CALCM_STRICT_ARGS */

    /* return requested amount, but never more then is available. */
    datalen = MIN(*DataLen_p, datalen);

    c_memcpy(Data_p, CAL_CM_nvm_data, datalen);
    *DataLen_p = datalen;

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_NVM_PUBLICDATA_READ__CM */

/* end of file cal_cm-v2_nvm.c */
