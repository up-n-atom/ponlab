/* cm_tokens_common.h
 *
 * Crypto Module Tokens Parser/Generator - Common Data Types
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

#ifndef INCLUDE_GUARD_CM_TOKENS_COMMON_H
#define INCLUDE_GUARD_CM_TOKENS_COMMON_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

// an array of 64 32bit words can represent any Crypto Token
// two types are used, for clarity and for future trimming

#define CMTOKENS_COMMAND_WORDS   64
#define CMTOKENS_RESPONSE_WORDS  64

typedef struct
{
    uint32_t W[CMTOKENS_COMMAND_WORDS];
} CMTokens_Command_t;

typedef struct
{
    uint32_t W[CMTOKENS_RESPONSE_WORDS];
} CMTokens_Response_t;


/*----------------------------------------------------------------------------
 * CMTokens_MakeToken_Clear
 *
 * This function initializes a CMTokens_Command_t array with a known pattern.
 * This helps debugging.
 */
static inline void
CMTokens_MakeToken_Clear(
        CMTokens_Command_t * const CommandToken_p)
{
    unsigned int i;

    for (i = 0; i < CMTOKENS_COMMAND_WORDS; i++)
        CommandToken_p->W[i] = 0xAAAAAAAA;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeToken_Identity
 *
 * This function sets the token identity.
 */
static inline void
CMTokens_MakeToken_Identity(
        CMTokens_Command_t * const CommandToken_p,
        uint32_t Identity)
{
    CommandToken_p->W[1] = Identity;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_SetTokenID
 *
 * This function sets the TokenID related field in the Command Token. The CM
 * will copy this value to the Result Token.
 * This can also be used for data stream synchronization: the TokenID is
 * appended to the end of the DMA stream. This word must be initialized to a
 * different value and then polled until the expected TokenID value/pattern
 * shows up.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * TokenIDValue
 *     The 16 bit TokenID value that the CM will write to the Result Token
 *     and optionally appends to the end of the output DMA data stream.
 *
 * fWriteTokenID
 *     false  do not append TokenID to DMA data stream.
 *     true   write TokenID at end of DMA data stream. The stream will first
 *            be padded with zeros until a complete 32bit word before an extra
 *            32bit word is output with the TokenID in it.
 */
static inline void
CMTokens_MakeCommand_SetTokenID(
        CMTokens_Command_t * const CommandToken_p,
        uint16_t TokenIDValue,
        bool fWriteTokenID)
{
    // replace TokenID field (word 0, lowest 16 bits) with TokenIDValue
    CommandToken_p->W[0] &= ((MASK_16_BITS << 16) - BIT_18);
    CommandToken_p->W[0] |= TokenIDValue;

    // set Write Token ID field (word 0, but 18)
    if (fWriteTokenID)
        CommandToken_p->W[0] |= BIT_18;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WriteInDescriptor
 *
 * Internal helper function - not to be used directly due to knowledge of
 * token word offsets.
 */
void
CMTokens_MakeCommand_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int StartWord,
        const unsigned int WordWriteCount,
        const EIP123_DescriptorChain_t * const Descriptor_p);


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WriteOutDescriptor
 *
 * Internal helper function - not to be used directly due to knowledge of
 * token word offsets.
 */
void
CMTokens_MakeCommand_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int StartWord,
        const unsigned int WordWriteCount,
        const EIP123_DescriptorChain_t * const Descriptor_p);


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_Generic
 *
 * This token checks the initial field of a response token and checks it for
 * errors. This function can be used on any response token.
 *
 * Return Value
 *      1    Found an error result - call CMTokens_ParseResponse_ErrorDetails
 *      0    Success (no error)
 */
static inline int
CMTokens_ParseResponse_Generic(
        CMTokens_Response_t * const ResponseToken_p)
{
    if (ResponseToken_p->W[0] & BIT_31)
        return 1;

    return 0;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_ReadByteArray
 *
 * This function fills a consecutive number of words in the command token with
 * bytes from an array. Four consecutive bytes form a 32bit word, LSB-first.
 */
void
CMTokens_MakeCommand_ReadByteArray(
        const uint8_t * Source_p,
        unsigned int SrcLenInBytes,
        CMTokens_Command_t * const CommandToken_p,
        unsigned int StartWord);


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_WriteByteArray
 *
 * This function reads a number of consecutive words from the response token
 * and writes these to a byte array, breaking down each word into bytes, LSB
 * first.
 */
void
CMTokens_ParseResponse_WriteByteArray(
        const CMTokens_Response_t * const ResponseToken_p,
        unsigned int StartWord,
        unsigned int DestLenInBytes,
        uint8_t * Dest_p);


#endif /* Include Guard */

/* end of file cm_tokens_common.h */
