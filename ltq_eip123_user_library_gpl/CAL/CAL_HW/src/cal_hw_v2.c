/* cal_hw_v2.c
 *
 * Implementation of the CAL_HW module.
 *
 * This implementation supports Token Exchange with the Crypto Module HW2.x.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_cal_hw.h"            // configuration options

#ifdef CAL_HW_CMv2

#include "basic_defs.h"             // uint8_t, etc.
#include "clib.h"                   // memcpy, memset
#include "log.h"                    // LOG_*
#include "device_mgmt.h"            // Device_Handle_t, Device_Find
#include "device_swap.h"            // Device_SwapEndian32

#include "eip123.h"
#include "cm_tokens_misc.h"
#include "cm_tokens_random.h"
#include "cm_tokens_errdetails.h"
#include "cm_tokens_asset.h"

#ifdef CALHW_USE_INTERRUPTS
#include "spal_semaphore.h"         // SPAL_Semaphore_*
#include "intdispatch.h"            // IntDispatch_*
#else
#include "spal_sleep.h"             // SPAL_Sleep*
#endif

#include "cal_hw_api.h"             // the API to implement
#include "identities.h"             // Identities_*_Get
#include "sharedlibs_onetimeinit.h" // SharedLib_OneTimeInit

#ifndef CALHW_REMOVE_PKA_SUPPORT
#include "eip28.h"                  // EIP28_CheckIfDone
#endif

#define LTQ_FORCE_NO_IDENTITY

static struct
{
    bool fIsInitialized;

    struct
    {
        Device_Handle_t Device123;
#ifdef CALHW_USE_INTERRUPTS
        SPAL_Semaphore_t WaitInterruptSem;
        IntDispatch_Handle_t IntDispatch_Handle;
#endif
    } CM;

#ifndef CALHW_REMOVE_PKA_SUPPORT
    struct
    {
        Device_Handle_t Device28;
#ifdef CALHW_USE_INTERRUPTS
        SPAL_Semaphore_t WaitInterruptSem;
        IntDispatch_Handle_t IntDispatch_Handle;
#endif
    } PKA;
#endif /* !CALHW_REMOVE_PKA_SUPPORT */

} CAL_HW;


/*----------------------------------------------------------------------------
 * CAL_HW_ClockAndReset
 *
 * This function ensures the modules are clocked and not in reset.
 * It should reset the modules to make sure they are in a known state.
 *
 * This function implemented in a separate source file to simplify
 * customization.
 */
extern int CAL_HW_ClockAndReset(void);


/*----------------------------------------------------------------------------
 * CALHWLib_InterruptHandler_EIP123
 *
 * This function is invoked by the Interrupt Dispatcher when the EIP-123 OUT
 * Mailbox Full interrupt has been activated. We increment the wait semaphore
 * on which CALHWLib_WaitForOutToken_Init is waiting.
 */
#ifdef CALHW_USE_INTERRUPTS
static void
CALHWLib_InterruptHandler_EIP123(
        void * Context)
{
    IDENTIFIER_NOT_USED(Context);

    LOG_INFO("CAL_HW: Signalling CM waiter thread\n");

    SPAL_Semaphore_Post(&CAL_HW.CM.WaitInterruptSem);
}
#endif /* CALHW_USE_INTERRUPTS */


/*----------------------------------------------------------------------------
 * CALHWLib_WaitForOutToken_Init
 */
static int
CALHWLib_WaitForOutToken_Init(void)
{
#ifdef CALHW_USE_INTERRUPTS
    // hook the interrupts

    int res;

    // create the semaphores used to signal the application / worker thread
    if (SPAL_Semaphore_Init(
            &CAL_HW.CM.WaitInterruptSem,
            /*Initial value:*/0) != SPAL_SUCCESS)
    {
        return -50;
    }

    // Hook the EIP-123 Interrupt
    res = IntDispatch_Initialize();
    if (res < 0)
    {
        res -= 100;
        return res;
    }

    res = IntDispatch_Hook(
                "EIP123_MAILBOXES_OUT-FULL",
                CALHWLib_InterruptHandler_EIP123,
                NULL,
                &CAL_HW.CM.IntDispatch_Handle);
    if (res < 0)
    {
        res -= 200;
        return res;
    }

    // Unmask interrupts
    res = IntDispatch_Unmask(CAL_HW.CM.IntDispatch_Handle);
    if (res < 0)
    {
        (void)IntDispatch_Unhook(CAL_HW.CM.IntDispatch_Handle);

        res -= 300;
        return res;
    }
#endif /* CALHW_USE_INTERRUPTS */

    // success
    return 0;
}


/*----------------------------------------------------------------------------
 * CALHWLib_WaitForOutToken_Interrupt
 *
 * This helper function waits for the OUT token. When this function returns
 * with no error code, the OUT token is available in the OUT mailbox.
 *
 * Returns <0 in case of error.
 */
#ifdef CALHW_USE_INTERRUPTS
static int
CALHWLib_WaitForOutToken_Interrupt(void)
{
    LOG_INFO("CAL_HW: Wait for OUT token START\n");

    // wait for interrupt
    // this is signalled with the semaphore
    if (SPAL_Semaphore_TimedWait(
                &CAL_HW.CM.WaitInterruptSem,
                CALHW_CM_WAIT_LIMIT_MS) == SPAL_SUCCESS)
    {
        LOG_INFO("CAL_HW: Wait for OUT token PASS\n");
        return 0;
    }

    LOG_WARN(
        "CAL_HW: "
        "Wait for OUT token TIMEOUT!\n");

    return -1;
}
#endif /* CALHW_USE_INTERRUPTS */


/*----------------------------------------------------------------------------
 * CALHWLib_WaitForOutToken_Polling
 *
 * This helper function waits for the OUT token. When this function returns
 * with no error code, the OUT token is available in the OUT mailbox.
 *
 * Returns <0 in case of error.
 */
#ifndef CALHW_USE_INTERRUPTS
static int
CALHWLib_WaitForOutToken_Polling(void)
{
    int SkipSleep = 50;
    unsigned int LoopsLeft = CALHW_CM_POLLING_MAXLOOPS + SkipSleep;

    LOG_INFO("CAL_HW: Wait for OUT token START\n");

    // poll for device completion with sleep
    while (LoopsLeft > 0)
    {
        if (EIP123_CanReadToken(CAL_HW.CM.Device123, CALHW_CM_MAILBOX_NR))
        {
            // OUT token is available!
            LOG_INFO("CAL_HW: Wait for OUT token PASS\n");
            return 0;       // ## RETURN ##
        }

        if (SkipSleep > 0)
        {
            // first few rounds are without sleep
            // this avoids sleeping unnecessarily for fast tokens
            SkipSleep--;
        }
        else
        {
            // sleep a bit
            SPAL_SleepMS(CALHW_POLLING_DELAY_MS);
        }

        LoopsLeft--;
    } // while

    // reached the polling limit
    LOG_CRIT(
        "CAL_HW: "
        "Wait for OUT token reached limit after after %u ms\n",
        CALHW_POLLING_DELAY_MS * CALHW_CM_POLLING_MAXLOOPS);

    return -1;
}
#endif /* !CALHW_USE_INTERRUPTS */


/*----------------------------------------------------------------------------
 * CALHWLib_ExchangeToken_Sub
 *
 * Inner steps of token exchange.
 */
static int
CALHWLib_ExchangeToken_Sub(
        CMTokens_Command_t * const CommandToken_p,
        CMTokens_Response_t * const ResponseToken_p)
{
    int res;

    // write the command token to the IN mailbox
    // also checks that it is empty
    res = EIP123_WriteAndSubmitToken(
                    CAL_HW.CM.Device123,
                    CALHW_CM_MAILBOX_NR,
                    CommandToken_p);
    if (res != 0)
        return -1;

    // wait for the result token to be available
#ifdef CALHW_USE_INTERRUPTS
    res = CALHWLib_WaitForOutToken_Interrupt();
#else
    res = CALHWLib_WaitForOutToken_Polling();
#endif
    if (res != 0)
        return -2;

    // copy the OUT token
    res = EIP123_ReadToken(
                CAL_HW.CM.Device123,
                CALHW_CM_MAILBOX_NR,
                ResponseToken_p);
    if (res != 0)
        return -3;

    return 0;   // success
}


/*----------------------------------------------------------------------------
 * CALHWLib_PrintToken
 *
 * This function prints the contents of a token, for debug purposes.
 */
#ifdef CALHW_TRACE_TOKENS
static void
CALHWLib_PrintToken(
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
#endif /* CALHW_TRACE_TOKENS */


/*----------------------------------------------------------------------------
 * CALHWLib_DecodeOpcode
 */
#ifdef CALHW_TRACE_TOKENS
static void
CALHWLib_DecodeOpcode(
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
#endif /* CALHW_TRACE_TOKENS */


/*----------------------------------------------------------------------------
 * CALHWLib_ExchangeToken
 *
 * This function exchanges a token with the EIP-123 Crypto Module. The token
 * is not modified in any way. That is, the identity must have been filled in
 * already - see CAL_HW_ExchangeToken.
 */
static int
CALHWLib_ExchangeToken(
        CMTokens_Command_t * const CommandToken_p,
        CMTokens_Response_t * const ResponseToken_p)
{
    int res;

#ifdef CALHW_TRACE_TOKENS
    {
        const char * p1 = "?";
        const char * p2 = p1;

        CALHWLib_DecodeOpcode(CommandToken_p->W[0], &p1, &p2);

        Log_FormattedMessage("IN: Opcode=%s, Subcode=%s\n", p1, p2);

        CALHWLib_PrintToken(
                "IN: ",
                CommandToken_p->W,
                CMTOKENS_COMMAND_WORDS);
    }
#endif /* CALHW_TRACE_TOKENS */

    res = CALHWLib_ExchangeToken_Sub(CommandToken_p, ResponseToken_p);

#ifdef CALHW_TRACE_TOKENS
    if (res == 0)
    {
        CALHWLib_PrintToken(
                "OUT: ",
                ResponseToken_p->W,
                CMTOKENS_RESPONSE_WORDS);
    }
#endif /* CALHW_TRACE_TOKENS */

    return res;
}


/*----------------------------------------------------------------------------
 * CALHWLib_CM_Init
 *
 * Initialize the communication with the Crypto Module.
 * Returns <0 on error.
 */
static int
CALHWLib_CM_Init(void)
{
    int res;

    // find the EIP123 device
    CAL_HW.CM.Device123 = Device_Find("EIP123");
    if (CAL_HW.CM.Device123 == NULL)
        return -1;

    if (EIP123_VerifyDeviceComms(CAL_HW.CM.Device123, CALHW_CM_MAILBOX_NR) != 0)
        return -2;

    // get exclusive access to the requested mailbox
    if (EIP123_Link(CAL_HW.CM.Device123, CALHW_CM_MAILBOX_NR) != 0)
        return -3;

    Log_FormattedMessageINFO(
            "CAL_HW: "
            "Using mailbox %d\n",
            CALHW_CM_MAILBOX_NR);

    // OUT mailbox is unexpectedly FULL?
    if (EIP123_CanReadToken(CAL_HW.CM.Device123, CALHW_CM_MAILBOX_NR))
        return -4;

    // IN mailbox is unexpectedly FULL?
    if (!EIP123_CanWriteToken(CAL_HW.CM.Device123, CALHW_CM_MAILBOX_NR))
        return -5;

    res = CALHWLib_WaitForOutToken_Init();
    if (res < 0)
        return res;

    // intentional use of swap function for code-coverage
    return Device_SwapEndian32(0); // 0 = success
}


/*----------------------------------------------------------------------------
 * CALHWLib_Configure_DMA
 */
static int
CALHWLib_Configure_DMA(void)
{
#ifdef CALHW_DMACONFIG_RUNPARAMS
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    int res;

    Log_FormattedMessageINFO(
        "CAL_HW: "
        "Configuring DMA: 0x%04X = 0x%08X\n",
        CALHW_DMACONFIG_RUNPARAMS_ADDR,
        CALHW_DMACONFIG_RUNPARAMS);

    CMTokens_MakeToken_Clear(&t_cmd);
    CMTokens_MakeCommand_RegisterWrite(
                            &t_cmd,
                            CALHW_DMACONFIG_RUNPARAMS_ADDR,
                            CALHW_DMACONFIG_RUNPARAMS);

    res = CALHWLib_ExchangeToken(&t_cmd, &t_res);
    if (res != 0)
        return -1;

    if (CMTokens_ParseResponse_Generic(&t_res))
    {
        res = CMTokens_ParseResponse_ErrorDetails(&t_res, NULL);
        res -= 2000;
        return res;
    }
#endif /* CALHW_DMACONFIG_RUNPARAMS */

    // success
    return 0;
}


/*----------------------------------------------------------------------------
 * CALHWLib_Configure_TRNG
 *
 * This function exchanges a TRNG Configuration token with the CM with the
 * necessary start-up parameters. This starts the TRNG and allows random
 * numbers to be generated.
 */
static int
CALHWLib_Configure_TRNG(void)
{
#ifdef CALHW_ENABLE_TRNGCONFIG
    CMTokens_TRNGConfig_t cfg = { 0 };
    CMTokens_Command_t t_cmd;
    CMTokens_Response_t t_res;
    int res;

    CMTokens_MakeToken_Clear(&t_cmd);

#ifdef CALHW_TRNGCONFIG_STARTTIME
    // when not provided, zero is used = maximum time
    cfg.StartTime = CALHW_TRNGCONFIG_STARTTIME;
#endif

#ifdef CALHW_TRNGCONFIG_AUTOSEED
    cfg.AutoSeed = CALHW_TRNGCONFIG_AUTOSEED;
#endif

#ifdef CALHW_TRNGCONFIG_MAXREFILLTIME
    cfg.MaxRefillTime = CALHW_TRNGCONFIG_MAXREFILLTIME;
#endif

#ifdef CALHW_TRNGCONFIG_SAMPLEDIV
    cfg.SampleDiv = CALHW_TRNGCONFIG_SAMPLEDIV;
#endif

#ifdef CALHW_TRNGCONFIG_MINREFILLTIME
    cfg.MinRefillTime = CALHW_TRNGCONFIG_MINREFILLTIME;
#endif

    Log_FormattedMessageINFO(
        "CAL_HW: "
        "Configuring TRNG: %u, %u, %u, %u, %u\n",
        cfg.StartTime,
        cfg.MaxRefillTime,
        cfg.MinRefillTime,
        cfg.SampleDiv,
        cfg.AutoSeed);

    CMTokens_MakeCommand_TRNG_Configure(&t_cmd, cfg);

    res = CALHWLib_ExchangeToken(&t_cmd, &t_res);
    if (res != 0)
        return -1;

    // check and report errors
    if (CMTokens_ParseResponse_Generic(&t_res))
    {
        res = CMTokens_ParseResponse_ErrorDetails(&t_res, NULL);
        res -= 2000;
        return res;
    }
#endif /* CALHW_ENABLE_TRNGCONFIG */

    // success
    return 0;
}


/*----------------------------------------------------------------------------
 * CALHWLib_CM_Configure
 *
 * This function sends two tokens to the CM. The first initializes the ExtDMA
 * and the second starts the TRNG.
 */
static int
CALHWLib_CM_Configure(void)
{
    int res;

    res = CALHWLib_Configure_DMA();
    if (res < 0)
    {
        LOG_WARN(
            "CAL_HW: "
            "Configure DMA failed (error %d)\n",
            res);

        return -1;
    }

    res = CALHWLib_Configure_TRNG();
    if (res < 0)
    {
        LOG_WARN(
            "CAL_HW: "
            "Configure TRNG failed (error %d)\n",
            res);

        return -2;
    }

    // success
    return 0;
}


/*----------------------------------------------------------------------------
 * CALHWLib_SetIdentityFields
 */
static void
CALHWLib_SetIdentityFields(
        CMTokens_Command_t * const Cmd_p)
{
    // insert short-lived ID
    {
        uint32_t ID = 0;
        int res;

        res = Identities_ShortLivedID_Get(&ID);
        if (res < 0)
        {
            LOG_WARN("CAL_HW: Error retrieving identity (%d)\n", res);
            // no reason to abort
            ID = 0xEEEEEEEE;
        }

        // all tokens require the 32bit Identity in the second word
        CMTokens_MakeToken_Identity(Cmd_p, ID);
    }

    // insert long-lived ID in appropriate token
    if (CMTokens_CommandNeedsAppID(Cmd_p))
    {
        // Token = Asset Management; Asset Load
        uint8_t LL_ID[IDENTITIES_LONGLIVEDID_BYTECOUNT];
        int res;

        res = Identities_LongLivedID_Get(LL_ID);
        if (res != 0)
        {
            LOG_WARN("CAL_HW: Error retrieving long-lived AppID (%d)\n", res);
            memset(LL_ID, 0xBA, IDENTITIES_LONGLIVEDID_BYTECOUNT);
        }

        // insert the long-lived ID at the start of the AAD block
        CMTokens_MakeCommand_InsertAppID(
                        Cmd_p,
                        LL_ID,
                        IDENTITIES_LONGLIVEDID_BYTECOUNT);
    }
}


/*----------------------------------------------------------------------------
 * CAL_HW_ExchangeToken
 *
 * This function exchanging a message with the Crypto Module, after filling in
 * the appropriate identity fields.
 *
 * Return Value:
 *   >=0    Length of received message
 *    <0    Error code
 */
int
CAL_HW_ExchangeToken(
        const CMTokens_Command_t * const CmdToken_p,
        CMTokens_Response_t * const ResponseToken_p)
{
    CMTokens_Command_t t_cmd;

    if (CmdToken_p == NULL || ResponseToken_p == NULL)
        return -1;

    if (CAL_HW.fIsInitialized == false)
        return -2;

    // make a copy of the token, to prevent the caller from modifying it
    memcpy(&t_cmd, CmdToken_p, sizeof(CMTokens_Command_t));

    // add the identities
    #ifndef LTQ_FORCE_NO_IDENTITY
    CALHWLib_SetIdentityFields(&t_cmd);
    #endif /* LTQ_FORCE_NO_IDENTITY */

    // exchange the token
    return CALHWLib_ExchangeToken(&t_cmd, ResponseToken_p);
}


/*----------------------------------------------------------------------------
 * CALHWLib_InterruptHandler_EIP28
 *
 * This function is invoked by the Interrupt Dispatcher when the EIP-28
 * interrupt has been activated. We increment the wait semaphore on which
 * CALHWLib_WaitPKA_Interrupt is waiting.
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT
#ifdef CALHW_USE_INTERRUPTS
static void
CALHWLib_InterruptHandler_EIP28(
        void * Context)
{
    IDENTIFIER_NOT_USED(Context);

    LOG_INFO("CAL_HW: Signalling PKA waiter thread\n");

    SPAL_Semaphore_Post(&CAL_HW.PKA.WaitInterruptSem);
}
#endif /* CALHW_USE_INTERRUPTS */
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CALHWLib_WaitForPKA_Init
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT
static int
CALHWLib_WaitForPKA_Init(void)
{
#ifdef CALHW_USE_INTERRUPTS
    // hook the interrupts

    int res;

    // create the semaphores used to signal the application / worker thread
    if (SPAL_Semaphore_Init(
            &CAL_HW.PKA.WaitInterruptSem,
            /*Initial value:*/0) != SPAL_SUCCESS)
    {
        return -50;
    }

    // Hook the EIP-28 Interrupt
    res = IntDispatch_Initialize();
    if (res < 0)
    {
        res -= 100;
        return res;
    }

    res = IntDispatch_Hook(
                "EIP28_READY",
                CALHWLib_InterruptHandler_EIP28,
                NULL,
                &CAL_HW.PKA.IntDispatch_Handle);
    if (res < 0)
    {
        res -= 200;
        return res;
    }

    // Unmask interrupts
    res = IntDispatch_Unmask(CAL_HW.PKA.IntDispatch_Handle);
    if (res < 0)
    {
        (void)IntDispatch_Unhook(CAL_HW.PKA.IntDispatch_Handle);

        res -= 300;
        return res;
    }
#endif /* CALHW_USE_INTERRUPTS */

    // success
    return 0;
}
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CALHWLib_WaitPKA_Interrupt
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT
#ifdef CALHW_USE_INTERRUPTS
static int
CALHWLib_WaitPKA_Interrupt(
        void * const EIP28_IOArea_p)
{
    LOG_INFO("CAL_HW: Wait for PKA ready START\n");

    // wait for interrupt
    // this is signalled with the semaphore
    if (SPAL_Semaphore_TimedWait(
                &CAL_HW.PKA.WaitInterruptSem,
                CALHW_PKA_WAIT_LIMIT_MS) == SPAL_SUCCESS)
    {
        LOG_INFO("CAL_HW: Wait for PKA ready PASS\n");
        return 0;
    }

    LOG_WARN(
        "CAL_HW: "
        "Wait for PKA ready TIMEOUT!\n");

    IDENTIFIER_NOT_USED(EIP28_IOArea_p);

    return -1;
}
#endif /* CALHW_USE_INTERRUPTS */
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CALHWLib_WaitPKA_Polling
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT
#ifndef CALHW_USE_INTERRUPTS
static int
CALHWLib_WaitPKA_Polling(
        void * const EIP28_IOArea_p)
{
    unsigned int LoopsLeft = CALHW_PKA_POLLING_MAXLOOPS;

    LOG_INFO("CAL_HW: Wait for PKA ready START\n");

    // poll for device completion
    while (LoopsLeft > 0)
    {
        bool fIsDone = false;

        EIP28_CheckIfDone_CALLATOMIC(EIP28_IOArea_p, &fIsDone);

        if (fIsDone)
        {
            LOG_INFO("CAL_HW: Wait for PKA ready PASS\n");
            return 0;        // ## RETURN ##
        }

        // sleep a bit
        SPAL_SleepMS(CALHW_POLLING_DELAY_MS);

        LoopsLeft--;
    } // while

    LOG_CRIT(
        "CAL_HW: "
        "Wait for PKA ready TIMEOUT after %u ms\n",
        CALHW_POLLING_DELAY_MS * CALHW_PKA_POLLING_MAXLOOPS);

    // reached the polling limit
    return -1;   // failed
}
#endif /* !CALHW_USE_INTERRUPTS */
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CAL_HW_WaitForPKADone_WithTimeout
 *
 * This function waits until the Public Key Accelerator HW module finishes the
 * operation started. This operation could have finished already.
 * The implementation handles the differences between Polling and Interrupt
 * mode. The timeout prevents endless waiting.
 *
 * Return Value:
 *     0    Success
 *    <0    Error code.
 */
int
CAL_HW_WaitForPKADone_WithTimeout(
        void * const EIP28_IOArea_p)
{
#ifdef CALHW_REMOVE_PKA_SUPPORT
    IDENTIFIER_NOT_USED(EIP28_IOArea_p);
    return -9;
#else

    if (CAL_HW.fIsInitialized == false)
        return -2;

#ifdef CALHW_USE_INTERRUPTS
    return CALHWLib_WaitPKA_Interrupt(EIP28_IOArea_p);
#else
    return CALHWLib_WaitPKA_Polling(EIP28_IOArea_p);
#endif

#endif /* !CALHW_REMOVE_PKA_SUPPORT */
}


/*----------------------------------------------------------------------------
 * CALHWLib_PKA_Init
 *
 * Initialize the communication with the Public Key Accelerator.
 * Returns <0 on error.
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT
static int
CALHWLib_PKA_Init(void)
{
    int res;

    // find the EIP28 device
    CAL_HW.PKA.Device28 = Device_Find("EIP150_PKA");
    if (CAL_HW.PKA.Device28 == NULL)
        return -1;

    res = CALHWLib_WaitForPKA_Init();
    if (res < 0)
        return res;

    // intentional use of swap function for code-coverage
    return Device_SwapEndian32(0); // 0 = success
}
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CAL_HW_Init
 *
 * This function initializes this implementation.
 */
int
CAL_HW_Init(void)
{
    int res;

    // already initialized?
    if (CAL_HW.fIsInitialized)
        return 0;

#ifdef CALHW_USE_INTERRUPTS
    Log_FormattedMessageINFO("CAL_HW: Interrupt mode\n");
#else
    Log_FormattedMessageINFO("CAL_HW: Polling mode\n");
#endif /* CALHW_USE_INTERRUPTS */

    // Initialize (user mode) Device, DMA-Resource and interrupt access
    // layer, unless already done.
    res = SharedLibs_OneTimeInit();
    if (res != 0)
        return -1;

    res = CAL_HW_ClockAndReset();
    if (res < 0)
    {
        LOG_CRIT(
            "CAL_HW: Clock and Reset error %d\n",
            res);

        return -2;
    }

    res = CALHWLib_CM_Init();
    if (res < 0)
    {
        LOG_CRIT(
            "CAL_HW: CM initialize error %d\n",
            res);

        return -3;
    }

    res = CALHWLib_CM_Configure();
    if (res < 0)
    {
        LOG_CRIT(
            "CAL_HW: CM configure error %d\n",
            res);

        return -4;
    }

#ifndef CALHW_REMOVE_PKA_SUPPORT
    res = CALHWLib_PKA_Init();
    if (res < 0)
    {
        LOG_CRIT(
            "CAL_HW: PKA initialize error %d\n",
            res);

        return -5;
    }
#endif /* !CALHW_REMOVE_PKA_SUPPORT */

    CAL_HW.fIsInitialized = true;

    // success
    return 0;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* !CAL_HW_CMv2 */

/* end of file cal_hw_v2.c */
