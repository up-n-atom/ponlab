/* umdevxsproxy.h
 *
 * This user-mode library handles the communication with the kernel driver.
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

#ifndef INCLUDE_GUARD_UMDEVXSPROXY_H
#define INCLUDE_GUARD_UMDEVXSPROXY_H


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Init
 *
 * Must be called once before any of the other functions.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to communicate with kernel driver
 */
int
UMDevXSProxy_Init(void);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Shutdown
 *
 * Must be called last, as clean-up step before stopping the application.
 */
void
UMDevXSProxy_Shutdown(void);


#endif /* INCLUDE_GUARD_UMDEVXSPROXY_H */

/* end of file umdevxsproxy.h */
