/* cal_cm-v2_read_version.c
 *
 * Implementation of the CAL API.
 *
 * This file implements the Read Version function.
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

#ifdef SFZCRYPTO_CF_USE__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                 // the API to implement

#include "cal_cm-v2_internal.h"     // CAL_CM_ExchangeToken + CAL_CM_SysInfo_Get

#include "cm_tokens_systeminfo.h"
#include "cm_tokens_errdetails.h"

// we only support single-digit major/minor/patch numbers
static const char CALCM_version_template[] = "CAL_CM v2.0 CM{HWv0.0p0 FWv0.0p0}";
//                                            01234567890123456789012345678901234


/*----------------------------------------------------------------------------
 * CALCM_ReadVersion_GetLen
 */
uint32_t
CALCM_ReadVersion_GetLen(void)
{
    return sizeof(CALCM_version_template);  // includes terminating zero
}


/*----------------------------------------------------------------------------
 * CAL_CM_SysInfo_Get
 */
int
CAL_CM_SysInfo_Get(
        CMTokens_SystemInfo_t * const SysInfo_p)
{
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;

#ifdef CALCM_STRICT_ARGS
    CMTokens_MakeToken_Clear(&t_cmd);
#endif

    CMTokens_MakeCommand_ReadSystemInfo(&t_cmd);

    // exchange a message with the CM
    {
        SfzCryptoStatus funcres;

        funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
        if (funcres != SFZCRYPTO_SUCCESS)
            return -1;
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
                "CAL_CM_SysInfo_Get: "
                "Failed with error %d (%s)\n",
                res,
                ErrMsg_p);

            return -2;
        }
    }

    // extract the version info from the response
    CMTokens_ParseResponse_ReadSystemInfo(&t_res, SysInfo_p);

    return 0;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_read_version
 *
 * This function retrieves the version information from the Crypto Module and
 * returns this in a string, including the length.
 */
uint32_t
sfzcrypto_cm_read_version(
        char * version_p)
{
    if (version_p)
    {
        CMTokens_SystemInfo_t SysInfo;
        int res;

        res = CAL_CM_SysInfo_Get(&SysInfo);
        if (res)
            return SFZCRYPTO_INTERNAL_ERROR;

#ifdef CALCM_STRICT_ARGS
        // sanity-check single-digit version numbers
        if (SysInfo.Firmware.Major > 9 ||
            SysInfo.Firmware.Minor > 9 ||
            SysInfo.Firmware.Patch > 9 ||
            SysInfo.Hardware.Major > 9 ||
            SysInfo.Hardware.Minor > 9 ||
            SysInfo.Hardware.Patch > 9)
        {
            LOG_WARN(
                "sfzcrypto_cm_read_version: "
                "Detected invalid version component\n");
        }
#endif /* CALCM_STRICT_ARGS */

        // copy the template
        memcpy(
            version_p,
            CALCM_version_template,
            sizeof(CALCM_version_template));

        // add the HW/FW version details
        version_p[18] += SysInfo.Hardware.Major;
        version_p[20] += SysInfo.Hardware.Minor;
        version_p[22] += SysInfo.Hardware.Patch;

        version_p[27] += SysInfo.Firmware.Major;
        version_p[29] += SysInfo.Firmware.Minor;
        version_p[31] += SysInfo.Firmware.Patch;
    }

    return sizeof(CALCM_version_template);
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_USE__CM */

/* end of file cal_cm-v2_read_version.c */
