/* debug_abort.c
 *
 * Description: Implementation of DEBUG_abort.
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

#include "implementation_defs.h"

#include <stdlib.h>
#include <stdio.h>      // fflush, stderr


/* This logic is to make it possible to get coverage reports on
   software runs that end-up (intentionally) to abort. */
#ifdef DEBUG_CF_ABORT_WRITE_PROFILE
void __gcov_flush();    /* Function to write profiles on disk. */
#define DEBUG_ABORT_WRITE_PROFILE __gcov_flush()
#else
#define DEBUG_ABORT_WRITE_PROFILE do { /* Not written. */ } while(0)
#endif


void DEBUG_abort(void)
{
#ifdef WIN32
    // avoid the "report to microsoft?" dialog and the
    // "your program seems to have stopped abnormally" message
    _set_abort_behavior(0, _WRITE_ABORT_MSG + _CALL_REPORTFAULT);
#endif

    /* flush stderr before calling abort() to make sure
       out is not cut off due to buffering. */
    fflush(stderr);

    DEBUG_ABORT_WRITE_PROFILE;

    abort();
}

/* end of file debug_abort.c */
