/* eip123.h
 *
 * EIP-123 HW2.x Crypto Module Driver Library API
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

#ifndef INCLUDE_GUARD_EIP123_H
#define INCLUDE_GUARD_EIP123_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "device_types.h"       // Device_Handle_t
#include "cm_tokens_common.h"   // CMTokens_Command_t, CMTokens_Response_t


/*----------------------------------------------------------------------------
 * EIP123_VerifyDeviceComms
 *
 * This function can be used to verify that the Device Handle really provides
 * communication to an EIP-123 hardware device.
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 */
int
EIP123_VerifyDeviceComms(
        Device_Handle_t Device,
        const uint8_t MailboxNr);


/*----------------------------------------------------------------------------
 * EIP123_GetOptions
 *
 * This function can be used to verify that the Device Handle really provides
 * communication to an EIP-123 hardware device.
 *
 * MyHostID_p (optional)
 *     Pointer to the memory location where the HostID for this CPU will be
 *     written.
 *
 * MasterID_p (optional)
 *     Pointer to the memory location where the HostID of the Master CPU will
 *     be written. The Master CPU can use the EIP123_Mailbox_ControlAccess
 *     function.
 *
 * NrOfMailboxes_p (optional)
 *     Pointer to the memory location where the number of mailboxes available
 *     on this EIP-123 will be written.
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 */
int
EIP123_GetOptions(
        Device_Handle_t Device,
        uint8_t * const MyHostID_p,
        uint8_t * const MasterID_p,
        uint8_t * const NrOfMailboxes_p);


/*----------------------------------------------------------------------------
 * EIP123_MailboxAccessControl
 *
 * This function is only available to the Master CPU and allows it to control
 * the maibox sharing by allowing only selected hosts to use selected
 * mailboxes. After reset, no host is allowed to access any mailbox.
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
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 */
int
EIP123_MailboxAccessControl(
        Device_Handle_t Device,
        uint8_t MailboxNr,
        uint8_t HostNr,
        bool fAccessAllowed);


/*----------------------------------------------------------------------------
 * EIP123_Link
 *
 * This function tries to link the requested mailbox. Upon success the mailbox
 * can be used until unlinked. A mailbox can only be linked by this host when
 * access has been granted by the Master CPU.
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 */
int
EIP123_Link(
        Device_Handle_t Device,
        const uint8_t MailboxNr);


/*----------------------------------------------------------------------------
 * EIP123_Unlink
 *
 * This function unlinks a previously linked mailbox.
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 */
int
EIP123_Unlink(
        Device_Handle_t Device,
        const uint8_t MailboxNr);


/*----------------------------------------------------------------------------
 * CanReadToken/CanWriteToken
 *
 * CanWriteToken returns 'true' when a new command token may be written to the
 * mailbox of the EIP123 Crypto Module using one of the WriteToken functions.
 * CanReadToken returns 'true' when a result token can be read from the
 * mailbox of the EIP123 Crypto Module using one of the ReadToken functions.
 * This functions are fully re-entrant and thread-safe.
 */
bool
EIP123_CanWriteToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr);

bool
EIP123_CanReadToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr);


/*----------------------------------------------------------------------------
 * EIP123_WriteAndSubmitToken
 *
 * This function writes the token to the IN mailbox and then hands off the
 * mailbox to the CM to start processing the token. The request fails when the
 * mailbox is full or not linked to thist host.
 *
 * Device
 *     The Driver Framework Device Handle for the EIP-123.
 *
 * MailboxNr
 *     The mailbox number to write this token to (1..4).
 *     The mailbox must be linked to this host.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * Return Value
 *     0    Success
 *     <0   Error code
 *     >0   Reserved
 */
int
EIP123_WriteAndSubmitToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr,
        CMTokens_Command_t * const CommandToken_p);


/*----------------------------------------------------------------------------
 * EIP123_ReadToken
 *
 * This function reads the token from the OUT mailbox and then frees the
 * mailbox, allowing the CM to write another response in it.
 * The request fails when the mailbox is empty or the token is not for this
 * host.
 *
 * Device
 *     The Driver Framework Device Handle for the EIP-123.
 *
 * MailboxNr
 *     The mailbox number to write this token to (1..4).
 *     The mailbox must be linked to this host.
 *
 * CommandToken_p
 *     Pointer to the command token buffer this function will write to.
 *
 * Return Value
 *     0    Success
 *     <0   Error code
 *     >0   Reserved
 */
int
EIP123_ReadToken(
        Device_Handle_t Device,
        const uint8_t MailboxNr,
        CMTokens_Response_t * const ResponseToken_p);

#endif /* Include Guard */

/* end of file eip123.h */
