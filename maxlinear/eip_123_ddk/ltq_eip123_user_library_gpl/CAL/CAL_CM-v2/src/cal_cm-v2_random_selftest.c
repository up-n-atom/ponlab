/* cal_cm-v2_random_selftest.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file implements the Random SelfTest functions.
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

#ifdef SFZCRYPTO_CF_RANDOM_SELFTEST__CM

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm-v2_internal.h"
#include "cal_cm-v2_dma.h"

#include "cm_tokens_random.h"
#include "cm_tokens_misc.h"
#include "cm_tokens_errdetails.h"

#include "cal_cm.h"             // the API to implement
#include "sfzcryptoapi_misc.h"  // SFZCRYPTO_RANDOM_SELFTEST_FLAG_*

#include "dmares_rw.h"          // DMAResource_Write32Array
#include "dmares_buf.h"         // DMAResource_{Alloc,Release}
                                // DMAResource_Xxx types

static const uint8_t AES_PRNG_KAT[] =
{
    // Seed (V)
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // Key
    0xa5, 0x3e, 0x05, 0x2e, 0xa8, 0x0a, 0x62, 0x13,
    0x3e, 0xb2, 0x6b, 0x33, 0x72, 0xa7, 0x1e, 0xa2,
    0xcc, 0xdd, 0xf1, 0x0e, 0x1a, 0x7c, 0xb8, 0x0b,
    0x6f, 0xd9, 0xc8, 0xa8, 0x6d, 0xa4, 0x5d, 0x46,

    // TestData (DT)
    0x97, 0xb4, 0x5f, 0x0e, 0xe0, 0xfb, 0x64, 0xac,
    0x3b, 0x8a, 0x1b, 0x9b, 0x75, 0x06, 0x66, 0xec,

    // Expected result
    0x69, 0xf6, 0xa4, 0x09, 0x47, 0x05, 0x73, 0xa5,
    0xde, 0xb2, 0x37, 0x44, 0xa9, 0xfe, 0x8c, 0x26
};


/*----------------------------------------------------------------------------
 * CAL_CM_RNG_Test_Algorithms
 *
 * Run one KAT on the PRNG's AES-based Post-Processor.
 *
 * Returns:
 *   0 on success
 *  <0 on error
 *  >0 reserved
 */
static int
CAL_CM_RNG_Test_Algorithms(void)
{
    CMTokens_Command_t t_cmd;
    SfzCryptoStatus funcres;
    CMTokens_Response_t t_res;
    uint8_t ResultBytes[16];
    int res;

    // setup PRNG_Test token with KAT input data
    CMTokens_MakeToken_Clear(&t_cmd);
    CMTokens_MakeCommand_PRNG_PP_Test(
            &t_cmd,
            AES_PRNG_KAT,
            AES_PRNG_KAT+16,
            AES_PRNG_KAT+16+32);

    // do token exchange with the CM
    funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);
    if (funcres != SFZCRYPTO_SUCCESS)
        return -funcres;

    // check for errors
    res = CMTokens_ParseResponse_Generic(&t_res);

    if (res != 0)
    {
        const char * ErrMsg_p;

        res = CMTokens_ParseResponse_ErrorDetails(&t_res, &ErrMsg_p);

        LOG_WARN(
            "CAL_CM_RNG_Test_Algorithms: "
            "Token exchange failed with error %d (%s)\n",
            res,
            ErrMsg_p);

        return -SFZCRYPTO_INTERNAL_ERROR;
    }

    res = CMTokens_ParseRespone_PRNG_PP_Test(&t_res, ResultBytes);
    if (res < 0)
    {
        LOG_WARN("CAL_CM_RNG_Test_Algorithms: ParseResponse failed\n");
        return -1001;
    }

    if (0 != c_memcmp(ResultBytes, AES_PRNG_KAT+16+32+16, 16))
    {
        LOG_WARN("CAL_CM_RNG_Test_Algorithms: Unexpected result\n");
        return -1002;
    }

    return 0;
}

/* The following functions assume that a bit sequence starts with bit 31
 * of the first word because that's how the EIP76-TRNG handles bit
 * streams passed to it as a sequence of words.
 */

/*----------------------------------------------------------------------------
 * CAL_CM_Force_Monobit_Fail
 *
 * Force every 29'th bit of pseudo-random bit sequence 'prdata_p' to one
 * in order to let the resulting sequence cause a "monobit_fail" alarm.
 * Note that 20000/690 ~ 29, see the EIP76 manual for details.
 */
static void
CAL_CM_Force_Monobit_Fail(
        uint32_t * const prdata_p,
        unsigned int nwords)
{
    unsigned int i = 0;

    while (i < 32*nwords)
    {
        unsigned int index = i >> 5;
        unsigned int shift = 31 - (i & 31);

        prdata_p[index] |= 1 << shift;
        i += 29;
    }
}

/*----------------------------------------------------------------------------
 * CAL_CM_Force_Poker_Fail
 *
 * Change most of the 0100 and 1011 nibbles from the pseudo-random bit
 * sequence 'prdata_p' in 0010 respectively 1101 in order to let the resulting
 * sequence cause a "poker_fail" alarm.
 */
static void
CAL_CM_Force_Poker_Fail(
        uint32_t * const prdata_p,
        unsigned int nwords)
{
    unsigned int i = 0;

    while (i < 32*nwords)
    {
        unsigned int index = i >> 5;
        unsigned int shift = 28 - (i & 31);
        unsigned int nibble = (prdata_p[index] >> shift) & 15;

        if ((nibble == 4) || (nibble == 11))
        {
            prdata_p[index] ^= 6 << shift;
            i += 76;
        }

        i += 4;
    }
}

/*----------------------------------------------------------------------------
 * CAL_CM_Force_LongRun_Fail
 *
 * Clear 64 bits near the end of the pseudo-random bit sequence 'prdata_p'
 * in order to let the resulting sequence cause a "longrun_fail" alarm.
 */
static void
CAL_CM_Force_LongRun_Fail(
        uint32_t * const prdata_p,
        unsigned int nwords)
{
    prdata_p[nwords - 3] = 0;
    prdata_p[nwords - 2] = 0;
}

/*----------------------------------------------------------------------------
 * CAL_CM_SetupPseudoRandomData
 *
 * Use an LFSR with taps described by 'polynomial' to generate a pseudo-
 * random bit sequence of 32 * 'nwords' bits.
 */
static void CAL_CM_SetupPseudoRandomData(
        uint32_t * const prdata_p,
        unsigned int nwords,
        unsigned int polynomial)
{
    unsigned int i, lfsr = 1;

    c_memset(prdata_p, 0, nwords*sizeof(uint32_t));
    for (i = 0; i < 32*nwords; i++)
    {
        unsigned int index = i >> 5;
        unsigned int shift = 31 - (i & 31);

        lfsr = (lfsr >> 1) ^ ((0 - (lfsr & 1)) & polynomial);
        prdata_p[index] |= (lfsr & 1) << shift;
    }
}

/* Some values for 'polynomial' that cause the generation of
 * a maximum-length bit sequence of length 63 respectively 65535 bits.
 */
#define LFSR_MAXLEN_6STAGE  0x30
#define LFSR_MAXLEN_16STAGE 0xB400


typedef struct
{
    unsigned int polynomial;
    void (*modifyfunc)(uint32_t *, unsigned int);
    int expected_result;
} RngAlarmsTestVector_t;

static const RngAlarmsTestVector_t
CAL_CM_RNG_ALARMS_TESTS[] = {
    {LFSR_MAXLEN_6STAGE, NULL, TRNG_RESULT_RUN_FAIL},
    {LFSR_MAXLEN_16STAGE, CAL_CM_Force_LongRun_Fail, TRNG_RESULT_LONGRUN_FAIL},
    {LFSR_MAXLEN_16STAGE, CAL_CM_Force_Poker_Fail, TRNG_RESULT_POKER_FAIL},
    {LFSR_MAXLEN_16STAGE, CAL_CM_Force_Monobit_Fail, TRNG_RESULT_MONOBIT_FAIL}
};

#define NELEMENTS(table) \
    sizeof(table) / sizeof(table[0])

/* Test patterns are 20000 bits, i.e. 625 words. */
#define PR_NWORDS (20000/32)
static uint32_t prdata_buffer[PR_NWORDS];

/*----------------------------------------------------------------------------
 * CAL_CM_RNG_Test_Alarms
 *
 * Run several KATs on the TRNG's bit pattern test circuits.
 *
 * Return Value:
 *    <0  Error
 *     0  Success
 *    >0  Reserved
 */
static int
CAL_CM_RNG_Test_Alarms(void)
{
    CALCM_DMA_Admin_t * Task_p = NULL;
    DMAResource_Handle_t Handle;
    uint32_t * PseudoRandomData;
    CMTokens_Command_t t_cmd;
    SfzCryptoStatus funcres;
    int retval = 0;
    unsigned int i;

    Task_p = CALCM_DMA_Alloc();
    if (!Task_p)
        return -999;

    {
        DMAResource_Properties_t Props = {0};
        DMAResource_AddrPair_t AddrPair;
        int rv;

        Props.Size = PR_NWORDS * sizeof(uint32_t);
        Props.Alignment = 4;
        rv = DMAResource_Alloc(
                Props,
                &AddrPair,
                &Handle);
        if (rv < 0)
        {
            LOG_INFO(
                "CAL_CM_RNG_Test_Alarms: "
                "DMA buffer allocation failed (error=%d)\n",
                rv);
            return -1001;
        }

        PseudoRandomData = AddrPair.Address_p;
    }

    for (i = 0; i < NELEMENTS(CAL_CM_RNG_ALARMS_TESTS); i++)
    {
        const RngAlarmsTestVector_t * tv;

        tv = &CAL_CM_RNG_ALARMS_TESTS[i];
        CAL_CM_SetupPseudoRandomData(
                prdata_buffer,
                PR_NWORDS,
                tv->polynomial);
        if (tv->modifyfunc)
        {
            tv->modifyfunc(prdata_buffer, PR_NWORDS);
        }

        DMAResource_Write32Array(Handle, 0, PR_NWORDS, prdata_buffer);

        // prepare pattern for input via DMA
        funcres = CALAdapter_PreDMA(
                        Task_p,
                        /*AlgorithmicBlockSize:*/4,
                        PR_NWORDS * sizeof(uint32_t),
                        (uint8_t *)PseudoRandomData,
                        NULL/*no output*/);

        if (funcres != SFZCRYPTO_SUCCESS)
        {
            // there was a problem with the input data
            LOG_INFO(
                "CAL_CM_RNG_Test_Alarms: "
                "PreDMA failed (i=%u)\n",
                i);

            retval = -funcres;
            break;
        }

        {
            CMTokens_Response_t t_res;
            uint8_t result;
            int res;

            // setup TRNG_Test token
            CMTokens_MakeToken_Clear(&t_cmd);
            CMTokens_MakeCommand_TRNG_HW_SelfTest(&t_cmd, PR_NWORDS * sizeof(uint32_t));
            CMTokens_MakeCommand_TRNG_HW_SelfTest_WriteInDescriptor(&t_cmd, &Task_p->InDescriptor);

            // do token exchange with the CM
            funcres = CAL_CM_ExchangeToken(&t_cmd, &t_res);

            // assuming PreDMA didn't create a bounce buffer,
            // there is no need to call PostDMA here.

            if (funcres != SFZCRYPTO_SUCCESS)
            {
                LOG_INFO(
                    "CAL_CM_RNG_Test_Alarms: "
                    "Token exchange failed (i=%u)\n",
                    i);
                retval = -funcres;
                break;
            }

            res = CMTokens_ParseResponse_TRNG_HW_SelfTest(
                            &t_res, &result);
            if ((res < 0) || (result != tv->expected_result))
            {
                LOG_INFO(
                    "CAL_CM_RNG_Test_Alarms: "
                    "unexpected result: res=%d, "
                    "result=%d, expected_result=%d\n",
                    res,
                    result,
                    tv->expected_result);
                retval = -1003;
                break;
            }
        }

        CALAdapter_PostDMA(Task_p);
    } // for

    DMAResource_Release(Handle);
    CALCM_DMA_Free(Task_p);
    return retval;
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_random_selftest
 */
SfzCryptoStatus
sfzcrypto_cm_random_selftest(
        uint32_t control_flags,
        uint32_t * const p_result_flags)
{
    uint32_t flags;

    if (p_result_flags == NULL)
        return SFZCRYPTO_INVALID_PARAMETER;

    if (0 == control_flags)
    {
        // return the supported tests
        *p_result_flags = (SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALGORITHMS |
                           SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALARMS);

        return SFZCRYPTO_SUCCESS;
    }

    *p_result_flags = 0;
    flags = control_flags;
    while (0 != flags)
    {
        int res = 0;
        uint32_t testid;

        testid = flags & (0 - flags);     // isolate right-most 1 in flags
        switch(testid)
        {
            case SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALGORITHMS:
                res = CAL_CM_RNG_Test_Algorithms();
                break;

            case SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALARMS:
                res = CAL_CM_RNG_Test_Alarms();
                break;

            default:
                LOG_WARN(
                    "sfzcrypto_cm_random_selftest: "
                    "Unsupported test request (0x%08x)\n",
                    control_flags);

                return SFZCRYPTO_INVALID_PARAMETER;
        } // switch

        if (0 == res)
        {
            *p_result_flags |= testid;
        }
        else
        {
            LOG_INFO(
                "sfzcrypto_cm_random_selftest: "
                "unexpected result: res=%d\n",
                res);
        }
        flags ^= testid;    // clear bit
    } // while

    if (control_flags != *p_result_flags)
        return SFZCRYPTO_OPERATION_FAILED;

    return SFZCRYPTO_SUCCESS;
}

#else

// avoid the "empty translation unit" warning
extern const int _avoid_empty_translation_unit;

#endif /* SFZCRYPTO_CF_RANDOM_SELFTEST__CM */

/* end of file cal_cm-v2_random_selftest.c */
