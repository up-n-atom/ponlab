/* cm_tokens_mac.h
 *
 * Crypto Module Tokens Parser/Generator - MAC Tokens
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

#ifndef INCLUDE_GUARD_CM_TOKENS_MAC_H
#define INCLUDE_GUARD_CM_TOKENS_MAC_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

enum
{
    CMTOKENS_MAC_ALGORITHM_HMAC_MD5 = 0,
    CMTOKENS_MAC_ALGORITHM_HMAC_SHA160 = 1,
    CMTOKENS_MAC_ALGORITHM_HMAC_SHA224 = 2,
    CMTOKENS_MAC_ALGORITHM_HMAC_SHA256 = 3,
    CMTOKENS_MAC_ALGORITHM_AES_CMAC = 4,
    CMTOKENS_MAC_ALGORITHM_AES_CBCMAC = 5,
    CMTOKENS_MAC_ALGORITHM_CAMELLIA_CMAC = 6,
    CMTOKENS_MAC_ALGORITHM_CAMELLIA_CBCMAC = 7,
    CMTOKENS_HASH_ALGORITHM_C2_H = 8
};

/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_SetLengthAlgoMode
 *
 * DataLength
 *     Number of bytes to process in this request.
 *     For non-final hash (fFinalize == false) this must be a multiple of 64
 *     bytes, otherwise the request will be rejected.
 *     For final hash, this can be any value.
 *
 * MacAlgo
 *     Mac algorithm selection. Use one of CMTOKENS_MAC_ALGORITHM_*
 *
 * fInit
 *     Set to true to have the digest initialized with the default value
 *     according to the specification for the selected algorithm.
 *
 * fFinalize
 *     Set to true to have the mac finalized.
 */
static inline void
CMTokens_MakeCommand_Mac_SetLengthAlgoMode(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t DataLength,
        const uint8_t MacAlgo,
        const bool fInit,
        const bool fFinalize)
{
    CommandToken_p->W[0] = (3 << 24);   // Opcode = 3 = Mac

    CommandToken_p->W[2] = DataLength;

    // note: following also clears AS enable flags
    CommandToken_p->W[6] = (MASK_4_BITS & MacAlgo);

    if (!fInit)
        CommandToken_p->W[6] |= BIT_4;

    if (!fFinalize)
        CommandToken_p->W[6] |= BIT_5;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_SetTotalMessageLength
 *
 * TotalMessageLength_LSW = Bits 31:0
 * TotalMessageLength_MSW = Bits 60:32
 *     This is the total message length required for finalizing the hash.
 */
static inline void
CMTokens_MakeCommand_Mac_SetTotalMessageLength(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t TotalMessageLength_LSW,
        const uint32_t TotalMessageLength_MSW)
{
    CommandToken_p->W[16] = TotalMessageLength_LSW;
    CommandToken_p->W[17] = TotalMessageLength_MSW;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_SetASLoadKey
 *
 * This function sets the Asset Store Load location for the key and activates
 * its use. This also disables the use of the key via the token.
 */
static inline void
CMTokens_MakeCommand_Mac_SetASLoadKey(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef,
        const unsigned int KeyLengthInBytes)
{
    CommandToken_p->W[6] |= ((MASK_7_BITS & KeyLengthInBytes) << 16);
    CommandToken_p->W[6] |= BIT_8;
    CommandToken_p->W[18] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_SetASLoadDigest
 *
 * This function sets the Asset Store Load location for the digest and
 * activates its use. This also disables the use of the digest via the token.
 */
static inline void
CMTokens_MakeCommand_Mac_SetASLoadDigest(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[8] = AssetRef;
    CommandToken_p->W[6] |= BIT_9;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_SetASSaveDigest
 *
 * This function sets the Asset Store Save location for the digest and
 * activates its use.
 */
static inline void
CMTokens_MakeCommand_Mac_SetASSaveDigest(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[7] = AssetRef;
    CommandToken_p->W[6] |= BIT_12;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_CopyKey
 *
 * This function copies the key from the buffer provided by the caller into
 * the command token.
 */
static inline void
CMTokens_MakeCommand_Mac_CopyKey(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int KeyLengthInBytes,
        const uint8_t * Source_p)
{
    CommandToken_p->W[6] |= ((MASK_7_BITS & KeyLengthInBytes) << 16);

    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            KeyLengthInBytes,
                            CommandToken_p,
                            /*StartWord:*/18);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Mac_CopyDigest
 *
 * This function copies the digest from the buffer provided by the caller into
 * the command token. The requested number of bytes are copied (length depends
 * on the algorithm that will be used).
 */
static inline void
CMTokens_MakeCommand_Mac_CopyDigest(
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
 * CMTokens_ParseResponse_Mac_CopyDigest
 *
 * This function copies the digest from the response token to the buffer
 * provided by the caller. The requested number of bytes are copied (length
 * depends on the algorithm that was used).
 */
static inline void
CMTokens_ParseResponse_Mac_CopyDigest(
        const CMTokens_Response_t * const ResponseToken_p,
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
 * CMTokens_MakeCommand_Mac_WriteInDescriptor
 */
static inline void
CMTokens_MakeCommand_Mac_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 3, 3, Descriptor_p);
}


#endif /* Include Guard */

/* end of file cm_tokens_mac.h */
