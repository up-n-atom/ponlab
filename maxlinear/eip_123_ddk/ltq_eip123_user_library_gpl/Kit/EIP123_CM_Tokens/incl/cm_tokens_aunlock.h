/* cm_tokens_aunlock.h
 *
 * Crypto Module Token helper functions
 * - Authenticated unlock tokens related functions and definitions
 *
 * This module can converts a set of parameters into a Crypto Module Command
 * token, or parses a set of parameters from a Crypto Module Response token.
 */

/*****************************************************************************
* Copyright (c) 2015 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_SM_TOKEN_AUNLOCK_H
#define INCLUDE_GUARD_SM_TOKEN_AUNLOCK_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t


/*----------------------------------------------------------------------------
 * CMTokens_Command_AUnlock_Start
 *
 * CommandToken_p
 *      Pointer to the command token buffer.
 *
 * StateAssetID
 *      ID of Asset of Authenticated Unlock State.
 *
 * KeyAssetID
 *      ID of Asset of Authenticated Unlock Key.
 */
static inline void
CMTokens_Command_AUnlock_Start(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t StateAssetID,
        const uint32_t KeyAssetID)
{
    CommandToken_p->W[0] = (8 << 24) | (0 << 28);
    CommandToken_p->W[2] = StateAssetID;
    CommandToken_p->W[3] = KeyAssetID;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_AUnlock_CopyNonce
 *
 * This function copies the Nonce from the result token to the buffer
 * provided by the caller. The Nonce length is always 16 bytes.
 *
 * ResultToken_p
 *     Pointer to the result token buffer.
 *
 * NonceData_p
 *      Pointer to the buffer to copy the nonce to.
 */
static inline void
CMTokens_ParseResponse_AUnlock_CopyNonce(
        const CMTokens_Response_t * const ResultToken_p,
        uint8_t * NonceData_p)
{
    CMTokens_ParseResponse_WriteByteArray(ResultToken_p, 2, 16, NonceData_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_Command_AUnlock_Verify
 *
 * CommandToken_p
 *      Pointer to the command token buffer.
 *
 * StateAssetID
 *      ID of Asset of Authenticated Unlock State.
 *
 * NonceData_p
 *      Pointer to the buffer that holds the nonce.
 *
 * DataAddress
 *      DMA Address of the signature.
 *
 * DataLengthInBytes
 *      Size of the signature.
 */
static inline void
CMTokens_Command_AUnlock_Verify(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t StateAssetID,
        const uint8_t * const NonceData_p,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (8 << 24) | (1 << 28);
    CommandToken_p->W[2] = StateAssetID;
    CommandToken_p->W[3] = (DataLengthInBytes & MASK_10_BITS);

    CMTokens_MakeCommand_ReadByteArray(NonceData_p, 16, CommandToken_p, 6);
}


/*----------------------------------------------------------------------------
 * CMTokens_Command_AUnlock_Verify_WriteInDescriptor
 */
static inline void
CMTokens_Command_AUnlock_Verify_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 4, 1, Descriptor_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_Command_SetSecureDebug
 *
 * CommandToken_p
 *      Pointer to the command token buffer.
 *
 * StateAssetID
 *      ID of Asset of Authenticated Unlock State.
 *
 * Set
 *      Indication to set the port bits, if not set the port bits are cleared.
 */
static inline void
CMTokens_Command_SetSecureDebug(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t StateAssetID,
        const bool Set)
{
    CommandToken_p->W[0] = (8 << 24) | (2 << 28);
    CommandToken_p->W[2] = StateAssetID;
    CommandToken_p->W[3] = Set ? BIT_31 : 0;
}


#endif /* Include Guard */

/* end of file cm_tokens_aunlock.h */
