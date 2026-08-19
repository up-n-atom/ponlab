/* cm_tokens_common.c
 *
 * Crypto Module Tokens Parser/Generator.
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

#include "c_cm_tokens.h"        // configuration options

#include "basic_defs.h"         // uint8_t, IDENTIFIER_NOT_USED, etc.
#include "clib.h"               // memset, memcpy
#include "eip123_dma.h"         // EIP123_DescriptorChain_t

#include "cm_tokens_common.h"   // the API to implement


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_ReadByteArray
 */
void
CMTokens_MakeCommand_ReadByteArray(
        const uint8_t * Source_p,
        unsigned int SrcLenInBytes,
        CMTokens_Command_t * const CommandToken_p,
        unsigned int StartWord)
{
    const uint8_t * const Stop_p = Source_p + SrcLenInBytes;

    if (CommandToken_p == NULL ||
        Source_p == NULL)
    {
        return;
    }

    while (Source_p < Stop_p)
    {
        uint32_t W;

        // LSB-first
        W = *Source_p++;
        W |= (*Source_p++) << 8;
        W |= (*Source_p++) << 16;
        W |= (*Source_p++) << 24;

        if (StartWord >= CMTOKENS_COMMAND_WORDS)
            return;

        CommandToken_p->W[StartWord++] = W;

    } // while
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_WriteByteArray
 */
void
CMTokens_ParseResponse_WriteByteArray(
        const CMTokens_Response_t * const ResponseToken_p,
        unsigned int StartWord,
        unsigned int DestLenInBytes,
        uint8_t * Dest_p)
{
    uint8_t * const Stop_p = Dest_p + DestLenInBytes;

    if (ResponseToken_p == NULL ||
        Dest_p == NULL)
    {
        return;
    }

    while (Dest_p < Stop_p)
    {
        uint32_t W;

        if (StartWord >= CMTOKENS_RESPONSE_WORDS)
            return;

        W = ResponseToken_p->W[StartWord++];

        // LSB-first
        if (Dest_p < Stop_p)
        {
            *Dest_p++ = (uint8_t)W;
            W >>= 8;
        }

        if (Dest_p < Stop_p)
        {
            *Dest_p++ = (uint8_t)W;
            W >>= 8;
        }

        if (Dest_p < Stop_p)
        {
            *Dest_p++ = (uint8_t)W;
            W >>= 8;
        }

        if (Dest_p < Stop_p)
        {
            *Dest_p++ = (uint8_t)W;
        }
    } // while
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WriteInDescriptor
 */
void
CMTokens_MakeCommand_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int StartWord,
        const unsigned int WordWriteCount,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    uint32_t src = 0;
    uint32_t lli = 0;
    uint32_t len = 0;

    EIP123_DescriptorChain_GetFirst(
                Descriptor_p,
                &src, /*dst:*/NULL, &lli, &len);

    // Write InputDataAddress
    if (WordWriteCount > 0)
        CommandToken_p->W[StartWord + 0] = src;

    // Write InputDataLength
    if (WordWriteCount > 1)
        CommandToken_p->W[StartWord + 1] = len;

    // Write InputGatherAddress
    if (WordWriteCount > 2)
        CommandToken_p->W[StartWord + 2] = lli;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_WriteOutDescriptor
 */
void
CMTokens_MakeCommand_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int StartWord,
        const unsigned int WordWriteCount,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    uint32_t dst = 0;
    uint32_t lli = 0;
    uint32_t len = 0;

    EIP123_DescriptorChain_GetFirst(
                Descriptor_p,
                /*src:*/NULL, &dst, &lli, &len);

    // Write OutputDataAddress
    if (WordWriteCount > 0)
        CommandToken_p->W[StartWord + 0] = dst;

    // Write OutputDataLength
    if (WordWriteCount > 1)
        CommandToken_p->W[StartWord + 1] = len;

    // Write OutputScatterAddress
    if (WordWriteCount > 2)
        CommandToken_p->W[StartWord + 2] = lli;
}


/* end of file cm_tokens_common.c */
