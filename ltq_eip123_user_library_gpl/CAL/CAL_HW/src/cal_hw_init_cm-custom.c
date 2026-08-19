/* cal_hw_init_cm-custom.c
 *
 * CAL_HW, (custom) HW initialization.
 *
 * This is a stub implementation for the SafeXcel-IP-123, which needs to be
 * filled in by the customer. Please use the cal_hw_init_cm-fpga.c as example.
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

#include "c_cal_hw.h"               // configuration

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "device_mgmt.h"
#include "device_rw.h"

/*----------------------------------------------------------------------------
 * CAL_HW_ClockAndReset
 *
 * This function is called from CAL_HW_Init to initialize the hardware modules
 * into a known and usable state. A typical implementation should reset the HW
 * blocks. When this is not possible, a check should be made to ensure the HW
 * is not in a state that can trigger problems when the SW tries to use it.
 *
 * Return 0 for succes and <0 upon error.
 */
int
CAL_HW_ClockAndReset(void)
{
    return -1;
}


/* end of file cal_hw_init_cm-custom.c */
