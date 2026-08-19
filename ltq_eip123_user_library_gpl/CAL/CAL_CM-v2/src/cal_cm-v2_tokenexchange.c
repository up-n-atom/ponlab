/* cal_cm-v2_tokenexchange.c
 *
 * Implementation of the CAL API for Crypto Module.
 * This file contains the token exchange with the EIP-123 Crypto Module.
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
#include "log.h"

#include "cal_cm-v2_internal.h"     // the API to implement

#include "spal_semaphore.h"         // SPAL_Semaphore*
#include "cal_hw_api.h"             // CAL_HW_*

// we use a semaphore because API has timeout option
static SPAL_Semaphore_t CAL_CM_TokenExchange_ExclusiveLock;


/*----------------------------------------------------------------------------
 * CALCMLib_PrintToken
 *
 * This function prints the contents of a token, for debug purposes.
 */
#ifdef CALCM_TRACE_TOKENS
static void
CALCMLib_PrintToken(
        const char * TitleStr_p,
        uint32_t * Words_p,
        unsigned int WordCount)
{
    int WordsInThisLine = 4;
    unsigned int i;

    for (i = 0; i < WordCount; i += 4)
    {
        // if we do not have enough data for a full line
        // then modify the line to cut off the printout

        // work-around due to the SafeZone macros
        // we have also dropped the newline
        if (i + 4 > WordCount)
            WordsInThisLine = WordCount - i;

        {
            char LineBuf[(3 * 16) + 1];
            char * p = LineBuf;
            int lp;

            for (lp = 0; lp < WordsInThisLine; lp++)
            {
                uint32_t w = Words_p[i + lp];
                char c;

                *p++ = ' ';

                c = '0' + (MASK_4_BITS & (w >> 28));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 24));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 20));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 16));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 12));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 8));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & (w >> 4));
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = '0' + (MASK_4_BITS & w);
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

            } // for

            *p = 0;     // zero-terminate the string

            Log_FormattedMessage(
                "%s %02d:%s\n",
                TitleStr_p,
                i,
                LineBuf);
        }
    } // for
}
#endif /* CALCM_TRACE_TOKENS */


/*----------------------------------------------------------------------------
 * CALCMLib_DecodeOpcode
 */
#ifdef CALCM_TRACE_TOKENS
static void
CALCMLib_DecodeOpcode(
        uint32_t Word0,
        const char ** OpcodeStr_pp,
        const char ** SubcodeStr_pp)
{
    unsigned int Opcode = MASK_4_BITS & (Word0 >> 24);
    unsigned int Subcode = MASK_2_BITS & (Word0 >> 28);

    *OpcodeStr_pp = "Reserved";
    *SubcodeStr_pp = "n/a";

    switch(Opcode)
    {
        case 0:
            *OpcodeStr_pp = "NOP";
            return;

        case 1:
            *OpcodeStr_pp = "Crypto";
            return;

        case 2:
            *OpcodeStr_pp = "Hash";
            return;

        case 3:
            *OpcodeStr_pp = "MAC";
            return;

        case 4:
            *OpcodeStr_pp = "TRNG";
            switch(Subcode)
            {
                case 0:
                    *SubcodeStr_pp = "GetRandomNumber";
                    return;

                case 1:
                    *SubcodeStr_pp = "Configure";
                    return;

                case 2:
                    *SubcodeStr_pp = "Test PRNG";
                    return;

                case 3:
                    *SubcodeStr_pp = "Test TRNG";
                    return;

            } // switch
            return;

        case 6:
            *OpcodeStr_pp = "AES-Wrap";
            return;

        case 7:
            *OpcodeStr_pp = "AssetMgmt";
            switch(Subcode)
            {
                case 0:
                    *SubcodeStr_pp = "Create Asset";
                    return;

                case 1:
                    *SubcodeStr_pp = "Load Asset";
                    return;

                case 2:
                    *SubcodeStr_pp = "NVM Read";
                    return;

                case 3:
                    *SubcodeStr_pp = "Delete Asset";
                    return;

            } // switch
            return;

        case 14:
            *OpcodeStr_pp = "Service";
            switch(Subcode)
            {
                case 0:
                    *SubcodeStr_pp = "Read Register";
                    return;

                case 1:
                    *SubcodeStr_pp = "Write Register";
                    return;

                case 2:
                    *SubcodeStr_pp = "Clock Switch";
                    return;

            } // switch
            return;

        case 15:
            *OpcodeStr_pp = "System Info";
            return;

        default:
            break;
    } // switch
}
#endif /* CALCM_TRACE_TOKENS */


/*----------------------------------------------------------------------------
 * CAL_CM_ExchangeToken
 *
 * This function exchanges a token with the EIP-123 Crypto Module using the
 * following steps, using a single statically linked mailbox.
 *  1.  Get exclusive access to CM
 *  2a. Check that the IN mailbox is empty
 *  2b. Write command token to IN mailbox
 *  2c. Hand over IN mailbox to CM
 *  3.  Wait for result token in OUT mailbox
 *  4a. Copy result token from OUT mailbox
 *  4b. Release OUT mailbox.
 *  5.  Release the exclusive lock
 */
SfzCryptoStatus
CAL_CM_ExchangeToken(
        CMTokens_Command_t * const CommandToken_p,
        CMTokens_Response_t * const ResponseToken_p)
{
    int res;

    if (CommandToken_p == NULL ||
        ResponseToken_p == NULL)
    {
        return SFZCRYPTO_INTERNAL_ERROR;
    }

#ifdef CALCM_TRACE_TOKENS
    {
        const char * p1 = "?";
        const char * p2 = p1;
        CALCMLib_DecodeOpcode(CommandToken_p->W[0], &p1, &p2);
        LOG_INFO("IN: Opcode=%s, Subcode=%s\n", p1, p2);

        CALCMLib_PrintToken("IN: ", CommandToken_p->W, CMTOKENS_COMMAND_WORDS);
    }
#endif

    if (SPAL_Semaphore_TimedWait(
                &CAL_CM_TokenExchange_ExclusiveLock,
                CALCM_WAIT_LIMIT_MS) != SPAL_SUCCESS)
    {
        LOG_CRIT(
            "CAL_CM_ExchangeToken: "
            "Failed to acquire lock\n");

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    res = CAL_HW_ExchangeToken(CommandToken_p, ResponseToken_p);

    SPAL_Semaphore_Post(&CAL_CM_TokenExchange_ExclusiveLock);

    if (res != 0)
    {
        LOG_WARN(
            "CAL_CM_ExchangeToken: "
            "Failed to exchange token (error %d)\n",
            res);

        return SFZCRYPTO_INTERNAL_ERROR;
    }
    else
    {
#ifdef CALCM_TRACE_TOKENS
        CALCMLib_PrintToken("OUT: ", ResponseToken_p->W, CMTOKENS_RESPONSE_WORDS);
#endif
    }

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * CAL_CM_PrintSystemInfo
 */
static int
CAL_CM_PrintSystemInfo(void)
{
    CMTokens_SystemInfo_t SysInfo;

    // get the system info (using a token exchange)
    {
        int res = CAL_CM_SysInfo_Get(&SysInfo);
        if (res != 0)
            return res;
    }

    Log_FormattedMessageINFO(
        "CM SysInfo: HW%u.%u.%u FW%u.%u.%u Mem:0x%04X Self:%u.0x%08X\n",
        SysInfo.Hardware.Major,
        SysInfo.Hardware.Minor,
        SysInfo.Hardware.Patch,
        SysInfo.Firmware.Major,
        SysInfo.Firmware.Minor,
        SysInfo.Firmware.Patch,
        SysInfo.Hardware.MemorySizeInBytes,
        SysInfo.SelfIdentity.HostID,
        SysInfo.SelfIdentity.Identity);

    if (SysInfo.Firmware.fIsTestFW)
    {
        Log_FormattedMessageCRIT(
            "CM SysInfo: "
            "Detected TEST firmware!\n");
    }

    if (SysInfo.NVM.ErrorCode != CMTOKENS_SYSINFO_NVM_OK)
    {
        Log_FormattedMessageCRIT(
            "CM SysInfo: "
            "Detected NVM issue (%d at 0x%04x)\n",
            SysInfo.NVM.ErrorCode,
            SysInfo.NVM.ErrorLocation);
    }

    return 0;
}


/*----------------------------------------------------------------------------
 * CAL_CM_Init
 *
 * This function sets up the communication with the EIP-123 device.
 */
int
CAL_CM_Init(void)
{
    int res;

    // create the synchronization lock used in this file
    if (SPAL_Semaphore_Init(&CAL_CM_TokenExchange_ExclusiveLock, 1) != SPAL_SUCCESS)
    {
        LOG_WARN(
            "CAL_CM_Init: "
            "Failed to create lock\n");
        return -1;
    }

    res = CAL_HW_Init();
    if (res != 0)
    {
        LOG_WARN(
            "CAL_CM_Init: "
            "Failed to initialize CAL_HW (error %d)\n",
            res);

        return -2;
    }

    res = CAL_CM_PrintSystemInfo();
    if (res != 0)
    {
        LOG_WARN(
            "CAL_CM_Init: "
            "Failed to report system info (error %d)\n",
            res);

        return -3;
    }

    return 0;   // 0 = success
}


/* end of file cal_cm-v2_tokenexchange.c */
