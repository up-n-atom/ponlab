/* cal_cm-v2_init.c
 *
 * Implementation of the CAL API for the Crypto Module (CAL_CM).
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

#include "c_cal_cm-v2.h"           // configuration

#include "cal_cm.h"             // the API to implement

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                     // the API to implement

#include "cal_cm-v2_internal.h"         // CAL_CM_Init
#include "cal_cm-v2_dma.h"              // CALCM_DMA_Alloc

#define CALCM_ISINITIALIZED_SIGNATURE (uint32_t)0xCA1CA1CA
#define CALCM_INIT_ONGOING_SIGNATURE  (uint32_t)0xCA1DD1CA

static uint32_t CAL_CM_IsInitialized;


/*----------------------------------------------------------------------------
 * CALCMLib_BasicDMATest
 *
 * This function uses Hash to verify the CM DMA operation.
 *
 * Returns true on success, false on error.
 */
static bool
CALCMLib_BasicDMATest(void)
{
    // try a hash known-answer-test to ensure DMA addresses are correct
    SfzCryptoStatus res;
    SfzCryptoHashContext hc = { 0, {0}, {0} };
    uint8_t Msg[57] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
#ifdef CALCM_BASICDMATEST_USE_SHA1
    static const uint8_t Digest[20] =
    {
        0x84, 0x98, 0x3e, 0x44,
        0x1c, 0x3b, 0xd2, 0x6e,
        0xba, 0xae, 0x4a, 0xa1,
        0xf9, 0x51, 0x29, 0xe5,
        0xe5, 0x46, 0x70, 0xf1
    };
#else
    static const uint8_t Digest[32] =
    {
        0x24, 0x8d, 0x6a, 0x61,
        0xd2, 0x06, 0x38, 0xb8,
        0xe5, 0xc0, 0x26, 0x93,
        0x0c, 0x3e, 0x60, 0x39,
        0xa3, 0x3c, 0xe4, 0x59,
        0x64, 0xff, 0x21, 0x67,
        0xf6, 0xec, 0xed, 0xd4,
        0x19, 0xdb, 0x06, 0xc1
    };
#endif

#ifdef CALCM_BASICDMATEST_USE_SHA1
    hc.algo = SFZCRYPTO_ALGO_HASH_SHA160;
#else
    hc.algo = SFZCRYPTO_ALGO_HASH_SHA256;
#endif

    res = sfzcrypto_cm_hash_data(
                &hc,
                Msg,
                56,
                /*init_with_default:*/true,
                /*final:*/true);

    if (res != SFZCRYPTO_SUCCESS)
    {
        LOG_WARN(
            "Basic DMA Test Failed: "
            "sfzcrypto_cm_hash_data returned %d\n",
            res);

        return false;
    }

    // check the digest
    if (memcmp(hc.digest, Digest, sizeof(Digest)) != 0)
    {
        // print this critical message
        LOG_WARN(
            "Basic DMA Test Failed: "
            "Wrong digest\n");

        return false;
    }

    return true;        // success
}


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_init
 *
 * This routine is called for every SfzCryptoContext that has been allocated.
 * We currently detect whether this is the first-ever call to this function
 * and then trigger first-time driver initialization.
 *
 * NOTE: This function is not reentrant during the first call.
 */
SfzCryptoStatus
sfzcrypto_cm_init(void)
{
    int res;

    if (CAL_CM_IsInitialized == CALCM_ISINITIALIZED_SIGNATURE)
    {
        // already initialized
        return SFZCRYPTO_ALREADY_INITIALIZED;
    }

    if (CAL_CM_IsInitialized == CALCM_INIT_ONGOING_SIGNATURE)
    {
        // this happens when initialization is interrupted and another thread
        // calls sfzcrypto_cm_init. This means the non-reentrance rule was
        // not obeyed by the application, so we return an error to avoid the
        // caller from starting to use the not yet initialized CAL API.
        return SFZCRYPTO_INTERNAL_ERROR;
    }

    CAL_CM_IsInitialized = CALCM_INIT_ONGOING_SIGNATURE;

    // there is a theoretical situation where two applications end up here

    res = CAL_CM_Init();
    if (res != 0)
    {
        LOG_INFO(
            "sfzcrypto_cm_init: "
            "CAL_CM_Init returned %d\n",
            res);

        goto fail;
    }

    if (!CALCMLib_BasicDMATest())
    {
        LOG_CRIT(
            "sfzcrypto_cm_init: "
            "Basic DMA test failed!\n");

        goto fail;
    }

    CAL_CM_IsInitialized = CALCM_ISINITIALIZED_SIGNATURE;

    return SFZCRYPTO_SUCCESS;

fail:
    // initialization failed
    CAL_CM_IsInitialized = SFZCRYPTO_NOT_INITIALISED;
    return SFZCRYPTO_INTERNAL_ERROR;
}


/* end of file cal_cm-v2_init.c */
