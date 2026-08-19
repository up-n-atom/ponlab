/* cm_tokens_misc.h
 *
 * Crypto Module Tokens Parser/Generator - Misceleaneous Tokens
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

#ifndef INCLUDE_GUARD_CM_TOKENS_MISC_H
#define INCLUDE_GUARD_CM_TOKENS_MISC_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command/Response
#include "eip123_dma.h"         // EIP123_DescriptorChain_t


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_RegisterWrite
 *
 * This token can be used to write the DMA controller configuration register.
 */
static inline void
CMTokens_MakeCommand_RegisterWrite(
        CMTokens_Command_t * const CommandToken_p,
        const uint16_t Address,
        const uint32_t Value)
{
    // Word 0: Opcode and Subcode
    CommandToken_p->W[0] =
            (14 << 24) |        // Opcode
            (1 << 28);          // Subcode

    // Word 2: Mode (don't care) and Number of words to write
    CommandToken_p->W[2] = 1;

    // Word 3: Mask = 0 = Write all 32 bits
    CommandToken_p->W[3] = 0;

    // Word 4: Address to write to
    CommandToken_p->W[4] = Address;

    // Word 5: Data to write
    CommandToken_p->W[5] = Value;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_NVM_Read
 *
 * DataLength
 *     Output data length
 *     Must be a multiple of 4.
 */
static inline void
CMTokens_MakeCommand_NVM_Read(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetID,
        const uint32_t DataLength)
{
    CommandToken_p->W[0] =
            (7 << 24) |             // Opcode = 7 (Asset Mgmt)
            (2 << 28);              // Subcode = 2

    CommandToken_p->W[2] = AssetID;
    CommandToken_p->W[3] = DataLength;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_NVM_Read
 */
static inline void
CMTokens_ParseResponse_NVM_Read(
        const CMTokens_Response_t * const ResponseToken_p,
        uint32_t * const DataLength_p)
{
    *DataLength_p = ResponseToken_p->W[1] & MASK_10_BITS;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_NVM_Read_WriteOutDescriptor
 */
static inline void
CMTokens_MakeCommand_NVM_Read_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteOutDescriptor(CommandToken_p, 4, 1, Descriptor_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Multi2Config
 *
 * Setup the given command token with a Multi2 Config request.
 * If AssetRef is 0, SystemKey must be non-NULL. If AssetRef is non-zero,
 * SystemKey may be NULL.
 */
static inline void
CMTokens_MakeCommand_Multi2Config(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t NumberOfRounds,
        const uint32_t SystemKeyAssetRef,
        const uint8_t * const SystemKey_p)
{
    CommandToken_p->W[0] = (13 << 24) |  // Opcode = 13 = CPRM Asset Management
                           (2 << 28);    // Subcode = 2 = Multi2 Configuration
    CommandToken_p->W[2] = SystemKeyAssetRef;
    CommandToken_p->W[3] = (uint32_t)NumberOfRounds;
    if (SystemKey_p != NULL)
    {
        CMTokens_MakeCommand_ReadByteArray(
                                SystemKey_p,
                                32,
                                CommandToken_p,
                                /*StartWord:*/4);
    }
}

#endif /* Include Guard */

/* end of file cm_tokens_misc.h */
