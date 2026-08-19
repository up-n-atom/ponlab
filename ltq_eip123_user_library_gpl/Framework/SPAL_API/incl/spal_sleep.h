/* spal_sleep.h
 *
 * Description: Sleep APIs
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

#ifndef INCLUDE_GUARD_SPAL_SLEEP_H
#define INCLUDE_GUARD_SPAL_SLEEP_H

/*----------------------------------------------------------------------------
 * SPAL_SleepMS
 *
 * This function blocks the caller for the specified number of milliseconds.
 * The typical implementation will sleep the execution context, allowing other
 * execution contexts to be scheduled. This function must be called from a
 * schedulable execution context.
 *
 * Milliseconds
 *     During in milliseconds to sleep before returning.
 */
void
SPAL_SleepMS(
        unsigned int Milliseconds);


#endif /* Include guard */

/* end of file spal_sleep.h */
