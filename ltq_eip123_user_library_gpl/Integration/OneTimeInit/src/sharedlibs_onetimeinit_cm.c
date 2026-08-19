/* sharedlibs_onetimeinit_cm.c
 *
 * Implementation of API for the one-time-only initialization
 * for CM environment.
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

#include "sharedlibs_onetimeinit.h"  // API to implement
#include "dmares_mgmt.h"             // DMAResource_Init
#include "device_mgmt.h"             // Device_Initialize

#define LOG_SEVERITY_MAX  LOG_SEVERITY_WARN
#include "log.h"

static int OneTimeInitLib_InitState = 0;

/*----------------------------------------------------------------------------
 * SharedLibs_OneTimeInit
 *
 * Initialize all shared libraries, unless this has already been done.
 * This function is not fully re-entrant nor thread-safe and should be
 * invoked by the application while it is still single-threaded.
 */
int
SharedLibs_OneTimeInit(void)
{
    // run initialization only once
    if (OneTimeInitLib_InitState == 0)
    {
        if (0 != Device_Initialize(NULL))
            OneTimeInitLib_InitState |= BIT_0;

        if (!DMAResource_Init())
            OneTimeInitLib_InitState |= BIT_1;

        if (OneTimeInitLib_InitState == 0)
        {
            // success
            OneTimeInitLib_InitState = 99;
        }
        else
        {
            LOG_WARN(
                "SharedLibs_OneTimeInit: "
                "Initialization failed: %d\n",
                OneTimeInitLib_InitState);
        }
    }

    if (OneTimeInitLib_InitState == 99/*success*/)
        return 0;

    // return error code
    return (0 - OneTimeInitLib_InitState);
}


/* end of file sharedlibs_onetimeinit_cm.c */
