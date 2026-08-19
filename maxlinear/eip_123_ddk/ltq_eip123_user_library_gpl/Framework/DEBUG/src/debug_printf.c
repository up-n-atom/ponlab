/* debug_printf.c
 *
 * Description: Implementation of DEBUG_printf.
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

#include <stdarg.h>
#include <stdio.h>

#ifdef DEBUG_CF_USE_STDOUT
#define DEBUG_fd  stdout
#else
#define DEBUG_fd  stderr
#endif

int DEBUG_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);

    (void)vfprintf(DEBUG_fd, format, ap);

#ifdef DEBUG_CF_USE_FLUSH
    (void)fflush(DEBUG_fd);
#endif

    va_end(ap);

    return 0;
}

/* end of file debug_printf.c */
