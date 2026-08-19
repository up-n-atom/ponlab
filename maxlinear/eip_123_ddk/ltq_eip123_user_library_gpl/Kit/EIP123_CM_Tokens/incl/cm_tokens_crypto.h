/* cm_tokens_crypto.h
 *
 * Crypto Module Tokens Parser/Generator - Crypto Token
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

#ifndef INCLUDE_GUARD_CM_TOKENS_CRYPTO_H
#define INCLUDE_GUARD_CM_TOKENS_CRYPTO_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t
#include "eip123_dma.h"         // EIP123_DescriptorChain_t
#include "cs_cm_tokens.h"       // CMTOKENS_REMOVE_CRYPTO_*

enum
{
    CMTOKENS_CRYPTO_MODE_ECB = 0,
    CMTOKENS_CRYPTO_MODE_CBC = 1,
    CMTOKENS_CRYPTO_MODE_CTR = 2,
    CMTOKENS_CRYPTO_MODE_AES_ICM = 3,
    CMTOKENS_CRYPTO_MODE_AES_f8 = 4,
    CMTOKENS_CRYPTO_MODE_C2_C_CBC = 1,
    CMTOKENS_CRYPTO_MODE_C2_C_CBC_INIT = 2,
    CMTOKENS_CRYPTO_MODE_C2_C_CBC_CONT = 3,
    CMTOKENS_CRYPTO_MODE_MULTI2_OFB = 2,
    CMTOKENS_CRYPTO_MODE_MULTI2_CFB = 3
};


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_AES
 *
 * fEncrypt
 *     true = Encrypt
 *     false = Decrypt
 *
 * Mode
 *     AES Mode. Must be one of CMTOKENS_CRYPTO_MODE_AES*
 *
 * DataLength
 *     Number of bytes to process.
 *     Must be a multiple of 16.
 */
static inline void
CMTokens_MakeCommand_Crypto_AES(
        CMTokens_Command_t * const CommandToken_p,
        const bool fEncrypt,
        const uint8_t Mode,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    CommandToken_p->W[10] = 0;  // Algorithm = AES

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    CommandToken_p->W[10] |= (MASK_4_BITS & Mode) << 4;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_3DES
 *
 * fDES
 *     true = DES
 *     false = 3DES
 *
 * fEncrypt
 *     true = Encrypt
 *     false = Decrypt
 *
 * Mode
 *     DES Mode. Must be one of CMTOKENS_CRYPTO_MODE_AESDES*
 *
 * DataLength
 *     Number of bytes to process.
 *     Must be a multiple of 16.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_3DES
static inline void
CMTokens_MakeCommand_Crypto_3DES(
        CMTokens_Command_t * const CommandToken_p,
        const bool fDES,
        const bool fEncrypt,
        const uint8_t Mode,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    if (fDES)
        CommandToken_p->W[10] = 1;  // Algorithm = DES
    else
        CommandToken_p->W[10] = 2;  // Algorithm = 3DES

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    CommandToken_p->W[10] |= (MASK_1_BIT & Mode) << 4;

}
#endif /* !CMTOKENS_REMOVE_CRYPTO_3DES */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_ARC4
 *
 * DataLength
 *     Number of bytes to process.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_ARC4
static inline void
CMTokens_MakeCommand_Crypto_ARC4(
        CMTokens_Command_t * const CommandToken_p,
        const bool fEncrypt,
        const uint32_t DataLengthInBytes,
        const uint8_t i,
        const uint8_t j)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    CommandToken_p->W[10] = (3 << 4) |      // Mode = Stateful-to-Stateful
                            3;              // Algorithm = ARC4

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    // no mode: we always use stateful-to-stateful

    CommandToken_p->W[12] = (j << 8) | i;
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_ARC4 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_AES_SetKeyLength
 *
 * This function sets the coded length of the AES key.
 */
static inline void
CMTokens_MakeCommand_Crypto_AES_SetKeyLength(
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

    CommandToken_p->W[10] |= (CodedKeyLen << 16);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_CAMELLIA
 *
 * fEncrypt
 *     true = Encrypt
 *     false = Decrypt
 *
 * Mode
 *     Feedback mode. Must be one of CMTOKENS_CRYPTO_MODE_{ECB,CBC,AES_CTR}
 *
 * DataLength
 *     Number of bytes to process.
 *     Must be a multiple of SFZCRYPTO_CAMELLIA_BLOCK_LEN (16).
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_CAMELLIA
static inline void
CMTokens_MakeCommand_Crypto_CAMELLIA(
        CMTokens_Command_t * const CommandToken_p,
        const bool fEncrypt,
        const uint8_t Mode,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    CommandToken_p->W[10] = 4;  // Algorithm = CAMELLIA

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    CommandToken_p->W[10] |= (MASK_4_BITS & Mode) << 4;
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_CAMELLIA */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_C2
 *
 * fEncrypt
 *     true = Encrypt
 *     false = Decrypt
 *
 * Mode
 *     Feedback mode. Must be one of CMTOKENS_CRYPTO_MODE_{ECB,C2_C_CBC}
 *
 * DataLength
 *     Number of bytes to process.
 *     Must be a multiple of SFZCRYPTO_C2_BLOCK_LEN (8).
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_C2
static inline void
CMTokens_MakeCommand_Crypto_C2(
        CMTokens_Command_t * const CommandToken_p,
        const bool fEncrypt,
        const uint8_t Mode,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    CommandToken_p->W[10] = 5;  // Algorithm = C2

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    CommandToken_p->W[10] |= (MASK_2_BITS & Mode) << 4;
    // W[10][19:16] (encoded KeyLength) is ignored for C2
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_C2 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_MULTI2
 *
 * fEncrypt
 *     true = Encrypt
 *     false = Decrypt
 *
 * Mode
 *     Feedback mode. Must be one of CMTOKENS_CRYPTO_MODE_{ECB,CBC,
 *     MULTI2_OFB,MULTI2_CFB}
 *
 * DataLength
 *     Number of bytes to process.
 *     Must be a multiple of SFZCRYPTO_MULTI2_BLOCK_LEN (8).
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_MULTI2
static inline void
CMTokens_MakeCommand_Crypto_MULTI2(
        CMTokens_Command_t * const CommandToken_p,
        const bool fEncrypt,
        const uint8_t Mode,
        const uint32_t DataLengthInBytes)
{
    CommandToken_p->W[0] = (1 << 24);   // Opcode = 1 = Crypto
    CommandToken_p->W[2] = DataLengthInBytes;

    CommandToken_p->W[10] = 6;  // Algorithm = MULTI2

    if (fEncrypt)
        CommandToken_p->W[10] |= BIT_15;

    CommandToken_p->W[10] |= (MASK_4_BITS & Mode) << 4;
    // W[10][19:16] (encoded KeyLength) is ignored for MULTI2
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_MULTI2 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_SetASLoadKey
 *
 * This function sets the Asset Store Load location for the key and activates
 * its use. This also disables the use of the key via the token.
 */
static inline void
CMTokens_MakeCommand_Crypto_SetASLoadKey(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[10] |= BIT_8;
    CommandToken_p->W[16] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_SetASLoadIV
 *
 * This function sets the Asset Store Load location for the IV and
 * activates its use. This also disables the use of the IV via the token.
 */
static inline void
CMTokens_MakeCommand_Crypto_SetASLoadIV(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[10] |= BIT_9;
    CommandToken_p->W[12] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_SetASSaveIV
 *
 * This function sets the Asset Store Save location for the IV and
 * activates its use.
 */
static inline void
CMTokens_MakeCommand_Crypto_SetASSaveIV(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t AssetRef)
{
    CommandToken_p->W[10] |= BIT_12;
    CommandToken_p->W[11] = AssetRef;
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_CopyKey
 *
 * This function copies the key from the buffer provided by the caller into
 * the command token.
 */
static inline void
CMTokens_MakeCommand_Crypto_CopyKey(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int KeyLengthInBytes,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            KeyLengthInBytes,
                            CommandToken_p,
                            /*StartWord:*/16);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_Copyf8SaltKey
 *
 * This function copies the f8 salt key from the buffer provided by the caller
 * into the command token.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_AES_F8
static inline void
CMTokens_MakeCommand_Crypto_Copyf8SaltKey(
        CMTokens_Command_t * const CommandToken_p,
        const unsigned int KeyLengthInBytes,
        const uint8_t * Source_p)
{
    CommandToken_p->W[10] |= ((MASK_4_BITS & KeyLengthInBytes) << 24);

    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            KeyLengthInBytes,
                            CommandToken_p,
                            /*StartWord:*/28);
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_AES_F8 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_CopyIV
 *
 * This function copies the IV from the buffer provided by the caller into
 * the command token. The IV length is always 16 bytes.
 */
static inline void
CMTokens_MakeCommand_Crypto_CopyIV(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            16,
                            CommandToken_p,
                            /*StartWord:*/12);
}


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_Copyf8IV
 *
 * This function copies the f8 IV from the buffer provided by the caller into
 * the command token. The f8 IV length is always 16 bytes.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_AES_F8
static inline void
CMTokens_MakeCommand_Crypto_Copyf8IV(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            16,
                            CommandToken_p,
                            /*StartWord:*/24);
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_AES_F8 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_Copyf8Keystream
 *
 * This function copies the f8 Keystream from the buffer provided by the
 * caller into the command token. The f8 Keystream length is always 16 bytes.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_AES_F8
static inline void
CMTokens_MakeCommand_Crypto_Copyf8Keystream(
        CMTokens_Command_t * const CommandToken_p,
        const uint8_t * Source_p)
{
    CMTokens_MakeCommand_ReadByteArray(
                            Source_p,
                            16,
                            CommandToken_p,
                            /*StartWord:*/32);
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_AES_F8 */


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_Crypto_CopyIV
 *
 * This function copies the IV from the response token to the buffer
 * provided by the caller. The IV length is always 16 bytes.
 */
static inline void
CMTokens_ParseResponse_Crypto_CopyIV(
        const CMTokens_Response_t * const ResponseToken_p,
        uint8_t * Dest_p)
{
    CMTokens_ParseResponse_WriteByteArray(
                            ResponseToken_p,
                            /*StartWord:*/2,
                            16,
                            Dest_p);
}


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_Crypto_Copyf8Keysteam
 *
 * This function copies the f8 keystream from the response token to the buffer
 * provided by the caller. The f8 keystream length is always 16 bytes.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_AES_F8
static inline void
CMTokens_ParseResponse_Crypto_Copyf8Keysteam(
        const CMTokens_Response_t * const ResponseToken_p,
        uint8_t * Dest_p)
{
    CMTokens_ParseResponse_WriteByteArray(
                            ResponseToken_p,
                            /*StartWord:*/6,
                            16,
                            Dest_p);
}
#endif /* CMTOKENS_REMOVE_CRYPTO_AES_F8 */


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_Crypto_ARC4_ij
 *
 * This function reads the ARC4 pointers i and j from the response buffer.
 */
#ifndef CMTOKENS_REMOVE_CRYPTO_ARC4
static inline void
CMTokens_ParseResponse_Crypto_ARC4_ij(
        const CMTokens_Response_t * const ResponseToken_p,
        uint8_t * const i_p,
        uint8_t * const j_p)
{
    *i_p = MASK_8_BITS & ResponseToken_p->W[1];
    *j_p = MASK_8_BITS & (ResponseToken_p->W[1] >> 8);
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_ARC4 */


/*----------------------------------------------------------------------------
 * CMTokens_MakeCommand_Crypto_WriteIn/Out/StateDescriptor
 */
static inline void
CMTokens_MakeCommand_Crypto_WriteInDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteInDescriptor(CommandToken_p, 3, 3, Descriptor_p);
}


static inline void
CMTokens_MakeCommand_Crypto_WriteOutDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const EIP123_DescriptorChain_t * const Descriptor_p)
{
    CMTokens_MakeCommand_WriteOutDescriptor(CommandToken_p, 6, 3, Descriptor_p);
}


#ifndef CMTOKENS_REMOVE_CRYPTO_ARC4
static inline void
CMTokens_MakeCommand_Crypto_WriteStateDescriptor(
        CMTokens_Command_t * const CommandToken_p,
        const uint32_t ARC4State_Addr)
{
    CommandToken_p->W[9] = ARC4State_Addr;
}
#endif /* !CMTOKENS_REMOVE_CRYPTO_ARC4 */


#endif /* Include Guard */

/* end of file cm_tokens_crypto.h */
