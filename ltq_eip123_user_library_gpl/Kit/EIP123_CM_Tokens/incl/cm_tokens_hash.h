/* cm_tokens_hash.h
 *
 * Crypto Module Tokens Parser/Generator - Hash Token
 *
 * This module can converts a set of parameters into a Crypto Module Command
 * token, or parses a set of parameters from a Crypto Module Result token.
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

#ifndef INCLUDE_GUARD_CM_TOKENS_HASH_H
#define INCLUDE_GUARD_CM_TOKENS_HASH_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

enum
{
    CMTOKENS_HASH_ALGORITHM_MD5 = 0,
    CMTOKENS_HASH_ALGORITHM_SHA160 = 1,
    CMTOKENS_HASH_ALGORITHM_SHA224 = 2,
    CMTOKENS_HASH_ALGORITHM_SHA256 = 3,
};


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Hash_SetLengthAlgoMode
 *
 * DataLength
 *     Number of bytes to process in this request.
 *     For non-final hash (fFinalize == false) this must be a multiple of 64
 *     bytes, otherwise the request will be rejected.
 *     For final hash, this can be any value.
 *
 * HashAlgo
 *     Hash algorithm selection. Use one of CMTOKENS_HASH_ALGORITHM_*
 *
 * fInitWithDefault
 *     Set to true to have the digest initialized with the default value
 *     according to the specification for the selected hash algorithm.
 *
 * fFinalize
 *     Set to true to have the hash finalized.
 */
static inline void
CMTokens_MakeCommand_Hash_SetLengthAlgoMode(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t DataLength,
        const uint8_t HashAlgo,
        const bool fInitWithDefault,
        const bool fFinalize)
{
    CommandToken_p->W[0] = (2 << 24);   // Opcode = 2 = Hash

    CommandToken_p->W[2] = DataLength;

    // note: following also clears AS enable flags
    CommandToken_p->W[6] = (MASK_4_BITS & HashAlgo);

    if (!fInitWithDefault)
        CommandToken_p->W[6] |= BIT_4;

    if (!fFinalize)
        CommandToken_p->W[6] |= BIT_5;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Hash_SetTotalMessageLength
 *
 * TotalMessageLength_LSW = Bits 31:0
 * TotalMessageLength_MSW = Bits 60:32
 *     This is the total message length required for finalizing the hash.
 */
static inline void
CMTokens_MakeCommand_Hash_SetTotalMessageLength(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TotalMessageLength_LSW,
        const uint32_t TotalMessageLength_MSW)
{
    CommandToken_p->W[16] = TotalMessageLength_LSW;
    CommandToken_p->W[17] = TotalMessageLength_MSW;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Hash_CopyDigest
 *
 * This function copies the digest from the buffer provided by the caller into
 * the command token. The requested number of bytes are copied (length depends
 * on the algorithm that will be used).
 */
static inline void
CMTokens_MakeCommand_Hash_CopyDigest(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int DigestLenInBytes,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            DigestLenInBytes,
                            CommandToken_p,
                            /*StartWord:*/8);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_Hash_CopyDigest
 *
 * This function copies the digest from the response token to the buffer
 * provided by the caller. The requested number of bytes are copied (length
 * depends on the algorithm that was used).
 *
 * The digest is written to the destination buffer, Byte 0 first.
 */
static inline void
CMTokens_ParseResponse_Hash_CopyDigest(
        CMTokens_Response_t * const ResponseToken_p,
        const unsigned int DigestLenInBytes,
        uint8_t * Dest_p)
{
    CMTokens_ParseResponse_WriteByteArray(
                            ResponseToken_p,
                            /*StartWord:*/2,
                            DigestLenInBytes,
                            Dest_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Hash_WriteInDescriptor
 */
static inline void
CMTokens_MakeCommand_Hash_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 3, 3, Descriptor_p);
}


#endif /* Include Guard */

/* end of file cm_tokens_hash.h */
