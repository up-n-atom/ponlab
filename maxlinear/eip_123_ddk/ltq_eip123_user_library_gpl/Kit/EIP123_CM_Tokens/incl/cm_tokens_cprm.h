/* cm_tokens_cprm.h
 *
 * Crypto Module Tokens Parser/Generator - CPRM Token
 *
 * This module can converts a set of parameters into a Crypto Module Command
 * token, or parses a set of parameters from a Crypto Module Response token.
 * This module handles the CPRM token that is used to either:
 * - request a C2 Key Derivation
 * - request C2 (Device) Key Info
 * - request configuration of the Multi2 engine (not really related to CPRM?!)
 */

/*****************************************************************************
* Copyright (c) 2011-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_CM_TOKENS_CPRM_H
#define INCLUDE_GUARD_CM_TOKENS_CPRM_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_C2KeyDeriveCommon
 *
 * Setup the given command token with a C2 Key Derive request. Upon return,
 * the command token is completely setup, except for the Identity field
 * (setup by driver). The AS_IDin2 field is set to zero. To change this,
 * call CMTokens_MakeCommand_C2KeyDerive_SetAssetIn2 next if necessary.
 */
static inline void
CMTokens_MakeCommand_C2KeyDeriveCommon(
        CMTokens_Command_t * const CommandToken_p,
        const int FuncSel,
        const uint32_t AssetInRef,
        const uint32_t AssetOutRef,
        const uint8_t * const InputData_p,
        const uint32_t InputLenInBytes)
{
    CommandToken_p->W[0] = (13 << 24) |  // Opcode = 13 = CPRM Asset Management
                           (0 << 28);    // Subcode = 0 = C2 Key Asset Derive

    CommandToken_p->W[2] = AssetInRef;
    CommandToken_p->W[3] = 0;
    CommandToken_p->W[4] = AssetOutRef;
    CommandToken_p->W[5] = (1 << (FuncSel + 24)) |
                           InputLenInBytes;
    CMTokens_MakeCommand_ReadByteArray(
                            InputData_p,
                            InputLenInBytes,
                            CommandToken_p,
                            /*StartWord:*/6);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_C2KeyDerive_SetAssetIn2
 *
 * Setup the AS_IDin2 field of the given C2 Key Derive command token.
 */
static inline void
CMTokens_MakeCommand_C2KeyDerive_SetAssetIn2(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetIn2Ref)
{
    CommandToken_p->W[3] = AssetIn2Ref;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_C2KeyDerive
 *
 * Extract the output length from a C2 Key Derive response token.
 */
static inline void
CMTokens_ParseResponse_C2KeyDerive(
        const CMTokens_Response_t * const ResponseToken_p,
        uint32_t * const OutputLenInBytes_p)
{
    *OutputLenInBytes_p = ResponseToken_p->W[1] & 0xFF;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_C2KeyDerive_CopyOutput
 *
 * Extract the output bytes from a C2 Key Derive response token.
 */
static inline void
CMTokens_ParseResponse_C2KeyDerive_CopyOutput(
        const CMTokens_Response_t * const ResponseToken_p,
        const unsigned int OutputLenInBytes,
        uint8_t * Dest_p)
{
    CMTokens_ParseResponse_WriteByteArray(
                            ResponseToken_p,
                            /*StartWord:*/2,
                            OutputLenInBytes,
                            Dest_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_C2KeyInfo
 *
 * Setup the given command token with a C2 Key Info request.
 */
static inline void
CMTokens_MakeCommand_C2KeyInfo(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t DeviceKeyAssetRef)
{
    CommandToken_p->W[0] = (13 << 24) |  // Opcode = 13 = CPRM Asset Management
                           (1 << 28);    // Subcode = 1 = C2 Key Asset Information

    CommandToken_p->W[2] = DeviceKeyAssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_C2KeyInfo
 *
 * Extract the Row number from a C2 Key Info response token.
 */
static inline void
CMTokens_ParseResponse_C2KeyInfo(
        const CMTokens_Response_t * const ResponseToken_p,
        uint16_t * const RowNumber_p)
{
    *RowNumber_p = (uint16_t)(ResponseToken_p->W[1] & 0xFFFF);
}

#endif /* Include Guard */

/* end of file cm_tokens_cprm.h */
