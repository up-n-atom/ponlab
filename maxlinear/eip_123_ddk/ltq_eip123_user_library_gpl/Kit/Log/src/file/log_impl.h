/* log_impl.h
 *
 * Log Module, implementation for redirecting to a file.
 */

/*****************************************************************************
* Copyright (c) 2008-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_LOG_IMPL_H
#define INCLUDE_GUARD_LOG_IMPL_H

#include <stdio.h>      // fprintf

// this file must be opened and assigned to Log_FILE before
// any of the Log API functions may be used.
extern FILE * Log_FILE;

// following implementation requires Variadic Macro support
#define Log_Message(...)          fprintf(Log_FILE, __VA_ARGS__)
#define Log_FormattedMessage      Log_Message

// backwards compatible implementation
#define Log_FormattedMessageINFO  Log_FormattedMessage
#define Log_FormattedMessageWARN  Log_FormattedMessage
#define Log_FormattedMessageCRIT  Log_FormattedMessage

#endif /* Include Guard */

/* end of file log_impl.h */
