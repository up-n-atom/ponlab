/* intdispatch_mgmt.h
 *
 * Interrupt Dispatcher API - Management
 * This part of the API must be used by one "client" only.
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

#ifndef INCLUDE_GUARD_INTDISPATCH_MGMT_H
#define INCLUDE_GUARD_INTDISPATCH_MGMT_H

/*----------------------------------------------------------------------------
 * IntDispatch_Shutdown
 *
 * This function performs the reverse operation of IntDispatchMgmt_Initialize.
 * Once this function returns, no more interrupts will be dispatched.
 * Care should be taken that only one software module makes this call and the
 * remainder of the API is not used during or after this call.
 *
 * Return Value
 *     0    Success
 *     <0   Error code
 */
int
IntDispatch_Shutdown(void);

#endif /* Include Guard */

/* end of file intdispatch_mgmt.h */
