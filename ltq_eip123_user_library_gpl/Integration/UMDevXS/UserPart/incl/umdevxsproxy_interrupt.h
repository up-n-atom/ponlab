/* umdevxsproxy_interrupt.h
 *
 * This user-mode library handles the communication with the
 * Linux User Mode Device Access (UMDevXS) driver.
 * Using this part of the API it is possible to wait for an interrupt event.
 */

/*****************************************************************************
* Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_UMDEVXSPROXY_INTERRUPT_H
#define INCLUDE_GUARD_UMDEVXSPROXY_INTERRUPT_H


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Interrupt_WaitWithTimeout
 *
 * This function must be used to wait for an interrupt even to occur. As soon
 * as the interrupt has been reported (by the OS), the function call returns.
 * The timeout value requests a maximum wait duration, in milliseconds.
 *
 * Timeout_ms (input)
 *     Maximum time to wait for the interrupt. If the interrupt does not occur
 *     when this amount of time has elapsed, the function returns and reports
 *     the timeout instead of interrupt occurance.
 *
 * Return Value
 *     0  Return due to interrupt
 *     1  Return due to timeout
 *    <0  Error code
 */
int
UMDevXSProxy_Interrupt_WaitWithTimeout(
        const unsigned int Timeout_ms);


#endif /* INCLUDE_GUARD_UMDEVXSPROXY_INTERRUPT_H */

/* umdevxsproxy_interrupt.h */
