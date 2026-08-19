/* cm_tokens_random.h
 *
 * Crypto Module Tokens Parser/Generator - Random Tokens
 *
 * This module can converts a set of parameters into a Crypto Module Command
 * token, or parses a set of parameters from a Crypto Module Response token.
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

#ifndef INCLUDE_GUARD_CM_TOKENS_RANDOM_H
#define INCLUDE_GUARD_CM_TOKENS_RANDOM_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

typedef struct
{
    uint16_t StartTime;
    uint8_t  AutoSeed;
    uint16_t MaxRefillTime;
    uint8_t  SampleDiv;
    uint8_t  MinRefillTime;
} CMTokens_TRNGConfig_t;


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_TRNG_Configure
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * TrngConfig_p
 *     TRNG configuration data.
 */
static inline void
CMTokens_MakeCommand_TRNG_Configure(
        CMTokens_Command_t * const CommandToken_p,
        const CMTokens_TRNGConfig_t TrngConfig)
{
    CommandToken_p->W[0] =
            (4 << 24) |             // Opcode = 4
            (1 << 28);              // Subcode = 1

    CommandToken_p->W[2] =
            (TrngConfig.StartTime << 16) |
            (TrngConfig.AutoSeed << 8) |
            BIT_0;                  // LST

    CommandToken_p->W[3] =
            (TrngConfig.MaxRefillTime << 16) |
            (TrngConfig.SampleDiv << 8) |
            TrngConfig.MinRefillTime;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_RandomNumber_Generate
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * NumberLengthInBytes
 *     The number of random bytes to generate.
 *
 * OutputDataAddress
 *     DMA address of the buffer where the random number bytes will be written
 *     to. Note that only one contiguous buffer is supported.
 *     The size of the buffer must be an integer number of 32bit words, equal
 *     or larger than NumberLengthInBytes. When WriteTokenID is used, one more
 *     32bit word will be written.
 */
static inline void
CMTokens_MakeCommand_RandomNumber_Generate(
        CMTokens_Command_t * const CommandToken_p,
        const uint16_t NumberLengthInBytes)
{
    CommandToken_p->W[0] =
            (4 << 24) |             // Opcode = 4
            (0 << 28);              // Subcode = 0

    CommandToken_p->W[2] = NumberLengthInBytes;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_RandomNumber_Generator_WriteOutDescriptor
 */
static inline void
CMTokens_MakeCommand_RandomNumber_Generator_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteOutDescriptor(CommandToken_p, 3, 1, Descriptor_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_RandomNumber_Generate
 *
 * This function extracts 'quality warning' details from a TRNG random number
 * generate response token.
 *
 * ResultToken_p
 *     Pointer to the result token buffer this function will read from.
 *
 * Return Value
 *     0    no warnings
 *     <0   Error code
 *     >0   the 5-bit Result code indicating some statistic anomaly in
 *          the generated random data.
 */
static inline int
CMTokens_ParseResponse_RandomNumber_Generate(
        CMTokens_Response_t * const ResponseToken_p)
{
    int rv;

    rv = ResponseToken_p->W[0] >> 24;
    if (rv != 0)
    {
        if (rv & BIT_7)
            return -rv;                       // ## error RETURN
        if ((rv & (BIT_6 | BIT_5)) != BIT_6)
            return -rv;                       // ## error RETURN
        rv = rv & MASK_5_BITS;
    }

    return rv;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_PRNG_ReseedNow
 *
 * This function creates a command token that requests to reseed the TRNG.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 */
static inline void
CMTokens_MakeCommand_PRNG_ReseedNow(
        CMTokens_Command_t * const CommandToken_p)
{
    CommandToken_p->W[0] =
            (4 << 24) |             // Opcode = 4
            (1 << 28);              // Subcode = 1

    CommandToken_p->W[2] = BIT_1;   // RRD = Reseed post-processor
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_PRNG_PP_Test
 *
 * This function creates a TRNG post-processor (=PRNG) verify command token
 * with given 'seed', 'key' and 'testdata' bytes.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * SeedBytes_p
 *     Pointer to the 16 seed bytes, LSB first.
 *
 * KeyBytes_p
 *     Pointer to the 32 key bytes, LSB first.
 *
 * TestDataBytes_p
 *     Pointer to the 16 test data bytes, LSB first.
 */
static inline void
CMTokens_MakeCommand_PRNG_PP_Test(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * SeedBytes_p,
        const uint8_t * KeyBytes_p,
        const uint8_t * TestDataBytes_p)
{
    CommandToken_p->W[0] =
            (4 << 24) |             // Opcode = 4
            (2 << 28);              // Subcode = 2

    CMTokens_MakeCommand_ReadByteArray(
            SeedBytes_p, 16, CommandToken_p, 2);

    CMTokens_MakeCommand_ReadByteArray(
            KeyBytes_p, 32, CommandToken_p, 6);

    CMTokens_MakeCommand_ReadByteArray(
            TestDataBytes_p, 16, CommandToken_p, 14);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_PRNG_PP_Test
 *
 * This function extracts the test result bytes from a PRNG verify
 * response token.
 *
 * ResultToken_p
 *     Pointer to the result token buffer this function will read from.
 *
 * ResultBytes_p
 *     Pointer to where to store the 16 result bytes, LSB first.
 *
 * Return Value
 *     0    Success
 *     <0   Error code
 *     >0   Reserved
 */
static inline int
CMTokens_ParseRespone_PRNG_PP_Test(
        CMTokens_Response_t * const ResponseToken_p,
        uint8_t * const ResultBytes_p)
{
    if (NULL == ResponseToken_p ||
        NULL == ResultBytes_p)
    {
        return -1;
    }

    CMTokens_ParseResponse_WriteByteArray(
        ResponseToken_p,
        2,
        16,
        ResultBytes_p);

    return 0;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_TRNG_HW_SelfTest
 *
 * This function populates a TRNG HW selftest command token, except for the
 * InputDataAddress field.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * TestDataLengthInBytes
 *     Length of the test data set. Must be an integer multiple of 4.
 */
static inline void
CMTokens_MakeCommand_TRNG_HW_SelfTest(
        CMTokens_Command_t * const CommandToken_p,
        const uint16_t TestDataLengthInBytes)
{
    CommandToken_p->W[0] =
            (4 << 24) |             // Opcode = 4
            (3 << 28);              // Subcode = 3

    CommandToken_p->W[2] = TestDataLengthInBytes;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_TRNG_HW_SelfTest_WriteInDescriptor
 */
static inline void
CMTokens_MakeCommand_TRNG_HW_SelfTest_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 3, 1, Descriptor_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_TRNG_HW_SelfTest
 *
 * This function extracts the test result details from a TRNG HW SelfTest
 * response token.
 *
 * ResultToken_p
 *     Pointer to the result token buffer this function will read from.
 *
 * ResultBytes_p
 *     Pointer to where to store the 16 result bytes, LSB first.
 *
 * Return Value
 *     0    Success
 *     <0   Error code
 *     >0   Reserved
 */
static inline int
CMTokens_ParseResponse_TRNG_HW_SelfTest(
        CMTokens_Response_t * const ResponseToken_p,
        uint8_t * const Result_p)
{
    if (NULL == ResponseToken_p ||
        NULL == Result_p)
    {
        return -1;
    }

    *Result_p = ResponseToken_p->W[0] >> 24;
    return 0;
}

/* Typical 'Result' values returned for a TRNG HW selftest. */
#define TRNG_RESULT_RUN_FAIL     0x41
#define TRNG_RESULT_LONGRUN_FAIL 0x42
#define TRNG_RESULT_POKER_FAIL   0x44
#define TRNG_RESULT_MONOBIT_FAIL 0x48

#endif /* Include Guard */

/* end of file cm_tokens_random.h */
