/* c_cal_hw.h
 *
 * Configuration options for CAL_HW module
 * The project-specific configuration file are included,
 * whereafter defaults are provided for missing parameters.
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

/*----------------------------------------------------------------
 * Defines that can be used or replaced in the cs_xxx.h file
 */

/*----------------------------------------------------------------
 * inclusion of product configuration file
 */
#include "cs_cal_hw.h"

#ifdef CAL_HW_CMv1
#define CALHW_DMACONFIG_RUNPARAMS_ADDR 0xE078
#else
#define CALHW_DMACONFIG_RUNPARAMS_ADDR 0xF878
#endif


// backup values

#ifndef CALHW_CM_WAIT_LIMIT_MS
#define CALHW_CM_WAIT_LIMIT_MS   (30 * 1000)
#endif

#ifndef CALHW_PKA_WAIT_LIMIT_MS
#define CALHW_PKA_WAIT_LIMIT_MS  (30 * 1000)
#endif

#ifndef CALHW_POLLING_DELAY_MS
#define CALHW_POLLING_DELAY_MS 1
#endif


#define CALHW_CM_POLLING_MAXLOOPS \
    (CALHW_CM_WAIT_LIMIT_MS / CALHW_POLLING_DELAY_MS)

#define CALHW_PKA_POLLING_MAXLOOPS \
    (CALHW_PKA_WAIT_LIMIT_MS / CALHW_POLLING_DELAY_MS)


#ifndef CALHW_CM_MAILBOX_NR
#define CALHW_CM_MAILBOX_NR  1
#endif

#ifndef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX  LOG_SEVERITY_WARN
#endif

/* end of file c_cal_hw.h */
