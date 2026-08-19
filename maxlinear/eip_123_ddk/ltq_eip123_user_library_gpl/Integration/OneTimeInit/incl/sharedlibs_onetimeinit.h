/* sharedlibs_onetimeinit.h
 *
 * API for the one-time-only initialization of shared libraries.
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

#ifndef INCLUDE_GUARD_SHAREDLIBS_ONETIMEINIT_H
#define INCLUDE_GUARD_SHAREDLIBS_ONETIMEINIT_H

/*----------------------------------------------------------------------------
 * SharedLibs_OneTimeInit
 *
 * Initialize all shared libraries, unless this has already been done.
 * This function is not fully re-entrant nor thread-safe and should be
 * invoked by the application while it is still single-threaded.
 *
 * Returns 0 on success or a negative value otherwise.
 */
int
SharedLibs_OneTimeInit(void);

#endif /* INCLUDE_GUARD_SHAREDLIBS_ONETIMEINIT_H */

/* end of file sharedlibs_onetimeinit.h */
