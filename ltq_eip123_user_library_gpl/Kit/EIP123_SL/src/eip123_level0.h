/* eip123_level0.h
 *
 * This file contains all the macros and inline functions that allow
 * access to the EIP123 registers and to build the values read or
 * written to the registers as well as the tokens.
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

#ifndef INCLUDE_GUARD_EIP123_LEVEL0_H
#define INCLUDE_GUARD_EIP123_LEVEL0_H

#include "basic_defs.h"         // uint32_t, bool, inline, BIT_* etc.
#include "device_types.h"       // Device_Handle_t
#include "device_rw.h"          // Read32, Write32


/* EIP123 mailbox memory locations as offset from a base address */
/* note: IN-mailbox is write-only, OUT-mailbox is read-only */
#define EIP123_MAILBOX_IN_BASE        0
#define EIP123_MAILBOX_OUT_BASE       0
#define EIP123_MAILBOX_SPACING_BYTES  0x400

/* EIP123 mailbox register locations as offset from a base address */
#define EIP123_REGISTEROFFSET_MAILBOX_CTRL    0x3F00
#define EIP123_REGISTEROFFSET_MAILBOX_STAT    0x3F00
#define EIP123_REGISTEROFFSET_MAILBOX_LOCKOUT 0x3F10
#define EIP123_REGISTEROFFSET_EIP_OPTIONS     0x3FF8
#define EIP123_REGISTEROFFSET_EIP_VERSION     0x3FFC

static uint32_t
EIP123Lib_ReadReg_MailboxStat(
        Device_Handle_t Device)
{
    return Device_Read32(Device, EIP123_REGISTEROFFSET_MAILBOX_STAT);
}

static void
EIP123Lib_WriteReg_MailboxCtrl(
        Device_Handle_t Device,
        uint32_t Value)
{
    Device_Write32(Device, EIP123_REGISTEROFFSET_MAILBOX_CTRL, Value);
}

static uint32_t
EIP123Lib_ReadReg_Options(
        Device_Handle_t Device)
{
    return Device_Read32(Device, EIP123_REGISTEROFFSET_EIP_OPTIONS);
}


static uint32_t
EIP123Lib_ReadReg_Version(
        Device_Handle_t Device)
{
    return Device_Read32(Device, EIP123_REGISTEROFFSET_EIP_VERSION);
}

#ifndef EIP123_REMOVE_MAILBOXACCESSCONTROL
static uint32_t
EIP123Lib_ReadReg_Lockout(
        Device_Handle_t Device)
{
    return Device_Read32(Device, EIP123_REGISTEROFFSET_MAILBOX_LOCKOUT);
}
#endif

#ifndef EIP123_REMOVE_MAILBOXACCESSCONTROL
static void
EIP123Lib_WriteReg_Lockout(
        Device_Handle_t Device,
        uint32_t Value)
{
    Device_Write32(Device, EIP123_REGISTEROFFSET_MAILBOX_LOCKOUT, Value);
}
#endif

#endif /* Include Guard */

/* end of file eip123_level0.h */
