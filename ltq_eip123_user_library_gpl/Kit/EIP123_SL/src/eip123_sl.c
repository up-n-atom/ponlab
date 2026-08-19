/* eip123_sl.c
 *
 * Driver for the EIP-123 HW2.x Crypto Module
 */

/*****************************************************************************
* Copyright (c) 2007-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_eip123.h"           // configuration

#include "basic_defs.h"         // uint8_t, IDENTIFIER_NOT_USED, etc.
#include "clib.h"               // memset, memcpy
#include "device_types.h"       // Device_Handle_t
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t

#include "eip123_level0.h"

#include "eip123.h"             // the API we will implement


/*----------------------------------------------------------------------------
 * EIP123_VerifyDeviceComms
 */
#ifndef EIP123_REMOVE_VERIFYDEVICECOMMS
int
EIP123_VerifyDeviceComms(
        Device_Handle_t Device,
        const uint8_t MailboxNr)
{
#ifdef EIP123_STRICT_ARGS
    if (MailboxNr < 1 || MailboxNr > 4)
        return -1;
#endif

    // check the hardware revision
    {
        uint32_t Value;
        Value = EIP123Lib_ReadReg_Version(Device);
        Value &= 0x0FF0FFFF;
        if (Value != 0x0200847B &&      /* HW 2.0 */
            Value != 0x0210847B &&      /* HW 2.1 */
            Value != 0x0220847B)        /* HW 2.2 */
        {
            // not a support SafeXcel-IP-123 HW2.x
            return -2;
        }
    }

    // check the mailbox number is valid and accessible
    {
        const uint32_t Options = EIP123Lib_ReadReg_Options(Device);

        // check the mailbox number is valid
        if (MailboxNr > (MASK_4_BITS & Options))
        {
            return -3;
        }

        // intentionally no lock-out check here
    }

    return 0;       // 0 = success
}
#endif /* !EIP123_REMOVE_VERIFYDEVICECOMMS */


/*----------------------------------------------------------------------------
 * EIP123_GetOptions
 */
#ifndef EIP123_REMOVE_GETOPTIONS
int
EIP123_GetOptions(
        Device_Handle_t Device,
        uint8_t * const MyHostID_p,
        uint8_t * const MasterID_p,
        uint8_t * const NrOfMailboxes_p)
{
    const uint32_t Options = EIP123Lib_ReadReg_Options(Device);

    if (MyHostID_p)
        *MyHostID_p = (uint8_t)(MASK_3_BITS & (Options >> 20));

    if (MasterID_p)
        *MasterID_p = (uint8_t)(MASK_3_BITS & (Options >> 16));

    if (NrOfMailboxes_p)
        *NrOfMailboxes_p = (uint8_t)(MASK_4_BITS & Options);

    return 0;       // no error
}
#endif /* !EIP123_REMOVE_GETOPTIONS */


/*----------------------------------------------------------------------------
 * EIP123_MailboxAccessControl
 *
 * MailboxNr
 *     The number of the mailbox (1..M) for which to control the access.
 *
 * HostNr
 *     The number of the host (0..H) for which to control the access to
 *     MailboxNr.
 *
 * fAccessAllowed
 *     true   Allow HostNr to link and use MailboxNr.
 *     false  Do not allow HostNr to link and use MailboxNr.
 */
#ifndef EIP123_REMOVE_MAILBOXACCESSCONTROL
int
EIP123_MailboxAccessControl(
        Device_Handle_t Device,
        uint8_t MailboxNr,
        uint8_t HostNr,
        bool fAccessAllowed)
{
#ifdef EIP123_STRICT_ARGS
    if (MailboxNr < 1 || MailboxNr > 4)
        return -1;

    if (HostNr > 7)
        return -2;
#endif

    {
        uint32_t LockOut = EIP123Lib_ReadReg_Lockout(Device);
        uint32_t BitMask = BIT_0;

        {
            int BitNr = (MailboxNr - 1) * 8 + HostNr;

            if (BitNr > 0)
                BitMask <<= BitNr;
        }

        if (fAccessAllowed)
        {
            // clear a bit grant access
            LockOut &= ~BitMask;
        }
        else
        {
            // set a bit to lock out access
            LockOut |= BitMask;
        }

        EIP123Lib_WriteReg_Lockout(Device, LockOut);
    }

    return 0;       // 0 = success
}
#endif /* !EIP123_REMOVE_MAILBOXACCESSCONTROL */


/*----------------------------------------------------------------------------
 * EIP123_Link
 */
#ifndef EIP123_REMOVE_LINK
int
EIP123_Link(
        Device_Handle_t Device,
        const uint8_t MailboxNr)
{
    uint32_t SetValue = BIT_2 << ((MailboxNr - 1) * 4);
    uint32_t GetValue;

    EIP123Lib_WriteReg_MailboxCtrl(Device, SetValue);

    GetValue = EIP123Lib_ReadReg_MailboxStat(Device);

    if ((GetValue & SetValue) != SetValue)
        return -1;

    return 0;   // 0 = success
}
#endif /* !EIP123_REMOVE_LINK */


/*----------------------------------------------------------------------------
 * EIP123_Unlink
 */
#ifndef EIP123_REMOVE_UNLINK
int
EIP123_Unlink(
        Device_Handle_t Device,
        const uint8_t MailboxNr)
{
    uint32_t SetValue = BIT_3 << ((MailboxNr - 1) * 4);
    uint32_t GetValue;

    EIP123Lib_WriteReg_MailboxCtrl(Device, SetValue);

    // check if mailbox still linked
    GetValue = EIP123Lib_ReadReg_MailboxStat(Device);
    SetValue >>= 1;

    if ((GetValue & SetValue) != 0)
        return -1;      // still linked

    return 0;   // 0 = success
}
#endif /* !EIP123_REMOVE_UNLINK */


/*----------------------------------------------------------------------------
 * EIP123_CanWriteToken
 *
 * This routine checks if the IN mailbox is empty, allowing a new token to be
 * written to it.
 */
#ifndef EIP123_REMOVE_CANWRITETOKEN
bool
EIP123_CanWriteToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr)
{
    uint32_t MailboxBit = BIT_0 << ((MailboxNr - 1) * 4);
    uint32_t Status;

    Status = EIP123Lib_ReadReg_MailboxStat(Device);

    if ((Status & MailboxBit) == 0)
        return true;

    return false;
}
#endif /* !EIP123_REMOVE_CANWRITETOKEN */


/*----------------------------------------------------------------------------
 * EIP123_CanReadToken
 */
bool
EIP123_CanReadToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr)
{
    uint32_t MailboxBit = BIT_1 << ((MailboxNr - 1) * 4);
    uint32_t Status;

    Status = EIP123Lib_ReadReg_MailboxStat(Device);

    if ((Status & MailboxBit) != 0)
        return true;

    return false;
}


/*----------------------------------------------------------------------------
 * EIP123_WriteAndSubmitToken
 */
int
EIP123_WriteAndSubmitToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr,
        CMTokens_Command_t * const CommandToken_p)
{
#ifdef EIP123_STRICT_ARGS
    if (CommandToken_p == NULL)
        return -1;
#endif

    if (!EIP123_CanWriteToken(Device, MailboxNr))
        return -2;

    // copy the token to the IN mailbox
    {
        unsigned int MailboxAddr = EIP123_MAILBOX_IN_BASE;

        MailboxAddr += EIP123_MAILBOX_SPACING_BYTES * (MailboxNr - 1);

        Device_Write32Array(
                    Device,
                    MailboxAddr,
                    CommandToken_p->W,
                    CMTOKENS_COMMAND_WORDS);
    }

    // hand over the IN mailbox (containing the token) to the CM
    {
        uint32_t MailboxBit = BIT_0 << ((MailboxNr - 1) * 4);

        EIP123Lib_WriteReg_MailboxCtrl(Device, MailboxBit);
    }

    return 0;   // success
}


/*----------------------------------------------------------------------------
 * EIP123_ReadToken
 */
int
EIP123_ReadToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr,
        CMTokens_Response_t * const ResponseToken_p)
{
#ifdef EIP123_STRICT_ARGS
    if (ResponseToken_p == NULL)
        return -1;
#endif

    if (!EIP123_CanReadToken(Device, MailboxNr))
        return -2;

    // copy the token from the OUT mailbox
    {
        unsigned int MailboxAddr = EIP123_MAILBOX_IN_BASE;

        MailboxAddr += EIP123_MAILBOX_SPACING_BYTES * (MailboxNr - 1);

        Device_Read32Array(
                    Device,
                    MailboxAddr,
                    ResponseToken_p->W,
                    CMTOKENS_RESPONSE_WORDS);
    }

    // hand back the OUT mailbox to the CM
    {
        uint32_t MailboxBit = BIT_1 << ((MailboxNr - 1) * 4);

        EIP123Lib_WriteReg_MailboxCtrl(Device, MailboxBit);
    }

    return 0;   // success
}


/* end of file eip123_sl.c */
