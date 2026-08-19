/* cal_hw_api.h
 *
 * CAL_HW module interface.
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

#ifndef INCLUDE_GUARD_CAL_HW_API_H
#define INCLUDE_GUARD_CAL_HW_API_H

#include "cs_cal_hw.h"
#ifdef CAL_HW_CMv1
#include "cm1_tokens_common.h"       // CMTokens_Command/Response_t
#else
#include "cm_tokens_common.h"        // CMTokens_Command/Response_t
#endif

/*----------------------------------------------------------------------------
 * CAL_HW_Init
 *
 * This function initializes the CAL_HW implementation. It must be called once
 * before the other API function may be used. It is not an error to call this
 * function more than once.
 *
 * NOTE: This function is not reentrant!
 *
 * Return Value:
 *     0    Success
 *     <0   Error code
 *     >0   Reserved
 */
int
CAL_HW_Init(void);


/*----------------------------------------------------------------------------
 * CAL_HW_ExchangeToken
 *
 * This function exchanges a message with the Crypto Module. The Command Token
 * is written to the IN mailbox and handed off to the CM. It then wait for an
 * OUT-mailbox-full event, copies the Response Token to the provided buffer
 * before handing back the OUT mailbox to the CM.
 *
 * NOTE: This function is not reentrant!
 *
 * Return Value:
 *   >=0    Length of received message
 *    <0    Error code
 */
int
CAL_HW_ExchangeToken(
        const CMTokens_Command_t * const CmdToken_p,
        CMTokens_Response_t * const ResponseToken_p);


/*----------------------------------------------------------------------------
 * CAL_HW_WaitForPKADone_WithTimeout
 *
 * This function waits until the Public Key Accelerator HW module finishes the
 * operation started. This operation could have finished already.
 * The implementation handles the differences between Polling and Interrupt
 * mode. The timeout prevents endless waiting.
 *
 * Return Value:
 *     0    Success
 *    <0    Error code.
 */
int
CAL_HW_WaitForPKADone_WithTimeout(
        void * const EIP28_IOArea_p);


#endif /* Include Guard */

/* end of file cal_hw_api.h */
