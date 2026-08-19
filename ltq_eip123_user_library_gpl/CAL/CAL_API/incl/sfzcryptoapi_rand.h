/* sfzcryptoapi_rand.h
 *
 * The Cryptographic Abstraction Layer APIs: Random Number functions.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_RAND_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_RAND_H

#include "public_defs.h"                // uint8_t, uint32_t, etc.
#include "sfzcryptoapi_result.h"        // SfzCryptoStatus
#include "sfzcryptoapi_init.h"          // SfzCryptoContext


/*----------------------------------------------------------------------------
 * sfzcrypto_rand_data
 *
 * Generate a random number.
 *
 * Request a number of bytes from the random number generator.
 *
 * Reads num bytes of random data from the True Random Number Generator
 * (TRNG) into the buffer at rand_num. The implementation is expected to
 * perform post processing as well.
 *
 * @pre p_rand_num != NULL
 *
 * @pre rand_num points to a valid pre-allocated buffer with sufficient space
 *      to hold rand_num_size_bytes bytes.
 *
 * @pre rand_num_size_bytes <= 65535.
 *
 * sfzcryptoctx_p
 *     Pointer to a pre-allocated and setup SfzCryptoContext object..
 *
 * rand_num_size_bytes
 *     Number of bytes to read.
 *
 * p_rand_num
 *     Pointer to buffer where random numbers will be stored.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_rand_data(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t rand_num_size_bytes,
        uint8_t * p_rand_num);


/*----------------------------------------------------------------------------
 * sfzcrypto_random_reseed
 *
 * This function triggers an internal re-seed of the Random Number Generator.
 * Use this function to guarantee fresh seed and key material for the X9.31
 * post-processor.
 */
SfzCryptoStatus
sfzcrypto_random_reseed(
            SfzCryptoContext * const sfzcryptoctx_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_random_selftest
 *
 * Test the Random Number Generator (PRNG/TRNG).
 * Request the RNG to test itself (or query which tests are available).
 * The RNG generally provides multiple tests. See the flag descriptions
 * below for an overview.
 *
 * control_flags
 *    Flags that indicate which specific test(s) to run.
 *    See SFZCRYPTO_RANDOM_SELFTEST_FLAG_* below.
 *    If all-zero, no tests are run but the set of supported tests is
 *    returned via p_result_flags.
 *
 * p_result_flags
 *    Pointer to the memory location in which to store the result bits.
 *    Upon return, '1' bits indicate which tests passed (or are available).
 *    See SFZCRYPTO_RANDOM_SELFTEST_FLAG_* below.
 *
 * Return Value:
 *     SFZCRYPTO_SUCCESS, if no errors occurred
 *     SFZCRYPTO_OPERATION_FAILED, if not all requested tests passed
 *     some other value, like SFZCRYPTO_INVALID_PARAMETERS.
 */
SfzCryptoStatus
sfzcrypto_random_selftest(
        SfzCryptoContext * const sfzcryptoctx_p,
        uint32_t control_flags,
        uint32_t * const p_result_flags);


/*----------------------------------------------------------------------------
 * SFZCRYPTO_RANDOM_SELFTEST_FLAG_*
 *
 * These flags are used in the sfzcrypto_random_selftest function.
 *
 * SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALGORITHMS:
 *    Test the implementation of (deterministic) crypto algorithms
 *    (like SHA1, AES, etc) used by the [P]RNG.
 *
 * SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALARMS:
 *    Test the alarm circuitry (e.g. bit pattern detectors)
 *    built into the [P]RNG.
 *
 * SFZCRYPTO_RANDOM_SELFTEST_FLAG_ENTROPY:
 *    Test the health of the entropy source(s) used by the [P]RNG.
 */

#define SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALGORITHMS  BIT_0
#define SFZCRYPTO_RANDOM_SELFTEST_FLAG_ALARMS      BIT_1
#define SFZCRYPTO_RANDOM_SELFTEST_FLAG_ENTROPY     BIT_2


#endif /* Include Guard */

/* end of file sfzcryptoapi_rand.h */
