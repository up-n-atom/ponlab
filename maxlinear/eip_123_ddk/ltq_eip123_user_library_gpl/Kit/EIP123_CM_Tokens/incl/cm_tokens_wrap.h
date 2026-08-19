/* cm_tokens_wrap.h
 *
 * Crypto Module Tokens Parser/Generator - Wrap/Unwrap Token
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

#ifndef INCLUDE_GUARD_CM_TOKENS_WRAP_H
#define INCLUDE_GUARD_CM_TOKENS_WRAP_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WrapUnwrap
 *
 * fWrap
 *     true = Wrap
 *     false = Unwrap
 *
 * InputDataLength
 *     Number of bytes to process.
 *     Must be at least 16, at most 1024 and always a multiple of 8.
 */
static inline void
CMTokens_MakeCommand_WrapUnwrap(
        CMTokens_Command_t * const CommandToken_p,
        const bool fWrap,
        const uint16_t InputDataLengthInBytes)
{
    CommandToken_p->W[0] = (6 << 24);   // Opcode = 6 = Wrap/Unwrap
    CommandToken_p->W[2] = (MASK_11_BITS & InputDataLengthInBytes);

    if (fWrap)
        CommandToken_p->W[5] = BIT_15;
    else
        CommandToken_p->W[5] = 0;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WrapUnwrap_SetKeyLength
 *
 * This function sets the coded length of the AES key.
 */
static inline void
CMTokens_MakeCommand_WrapUnwrap_SetKeyLength(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int KeyLengthInBytes)
{
    uint8_t CodedKeyLen = 0;

    // key length only needed for AES
    switch(KeyLengthInBytes)
    {
        case 128/8:
            CodedKeyLen = 1;
            break;

        case 192/8:
            CodedKeyLen = 2;
            break;

        case 256/8:
            CodedKeyLen = 3;
            break;
    } // switch

    CommandToken_p->W[5] |= (CodedKeyLen << 16);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WrapUnwrap_SetASLoadKey
 *
 * This function sets the Asset Store Load location for the key and activates
 * its use. This also disables the use of the key via the token.
 */
static inline void
CMTokens_MakeCommand_WrapUnwrap_SetASLoadKey(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[5] |= BIT_8;
    CommandToken_p->W[6] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WrapUnwrap_CopyKey
 *
 * This function copies the key from the buffer provided by the caller into
 * the command token.
 */
static inline void
CMTokens_MakeCommand_WrapUnwrap_CopyKey(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int KeyLengthInBytes,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            KeyLengthInBytes,
                            CommandToken_p,
                            /*StartWord:*/6);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_WrapUnwrap_GetDataLength
 *
 * This function returns the length of the result object from the response
 * token.
 */
static inline void
CMTokens_ParseResponse_WrapUnwrap_GetDataLength(
        const CMTokens_Response_t * const ResponseToken_p,
        unsigned int * const DataLength_p)
{
    *DataLength_p = (MASK_11_BITS & ResponseToken_p->W[1]);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WrapUnwrap_WriteIn/OutDescriptor
 */
static inline void
CMTokens_MakeCommand_WrapUnwrap_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 3, 1, Descriptor_p);
}


static inline void
CMTokens_MakeCommand_WrapUnwrap_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteOutDescriptor(CommandToken_p, 4, 1, Descriptor_p);
}


#endif /* Include Guard */

/* end of file cm_tokens_wrap.h */
