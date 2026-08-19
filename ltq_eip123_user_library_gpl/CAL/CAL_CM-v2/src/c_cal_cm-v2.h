/* c_cal_cm-v2.h
 *
 * Configuration options for CAL_CM module
 * The project-specific cs_cal_cm.h file is included,
 * whereafter defaults are provided for missing parameters.
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

/*----------------------------------------------------------------
 * Defines that can be used in the cs_xxx.h file
 */

/* currently none */


/*----------------------------------------------------------------
 * inclusion of cs_cal_cm.h
 */
#include "cs_cal_cm-v2.h"
#include "cf_cal.h"             // expected implementation
#include "cf_impldefs.h"        // IMPLDEFS_CF_DISABLE_L_DEBUG

// backup value for wait limiter (resource lock timeout)
#ifndef CALCM_WAIT_LIMIT_MS
#define CALCM_WAIT_LIMIT_MS 5000
#endif


#ifndef CALCM_POLLING_DELAY_MS
#define CALCM_POLLING_DELAY_MS 10
#endif


#define CALCM_POLLING_MAXLOOPS \
    (CALCM_WAIT_LIMIT_MS / CALCM_POLLING_DELAY_MS)


#ifndef CALCM_DMA_ALIGNMENT
#define CALCM_DMA_ALIGNMENT   4
#endif

#ifndef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX  LOG_SEVERITY_WARN
#endif

/* end of file cs_cal_cm-v2.h */
