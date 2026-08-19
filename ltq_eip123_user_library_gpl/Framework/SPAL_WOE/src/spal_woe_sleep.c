/* spal_woe_sleep.c
 *
 * Description: Win32 specific implementation of SPAL Sleep API
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

#include "spal_thread.h"
#include "implementation_defs.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


/*----------------------------------------------------------------------------
 * SPAL_SleepMS
 */
void
SPAL_SleepMS(
        unsigned int Milliseconds)
{
    Sleep(Milliseconds);
}


/* end of file spal_woe_sleep.c */
