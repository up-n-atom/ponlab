/* log_impl.h
 *
 * Log Module, implementation for SafeZone Framework
 */

/*****************************************************************************
* Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "implementation_defs.h"        // original L_DEBUG and helpers

void
Log_HexDump_SafeZone(
        const char * szPrefix_p,
        const unsigned int PrintOffset,
        const uint8_t * Buffer_p,
        const unsigned int ByteCount,
        const char * FileLineStr_p);

#define Log_HexDump(_szPrefix_p, _PrintOffset, _Buffer_p, _ByteCount) \
    Log_HexDump_SafeZone(_szPrefix_p, _PrintOffset, _Buffer_p, _ByteCount, __FILELINE__)

#ifndef IMPLDEFS_CF_DISABLE_L_DEBUG

#define Log_Message(_str) \
    DEBUG_printf("LL_DEBUG, LF_LOG, " __FILELINE__ ": " _str)

#define Log_FormattedMessage(...) \
    DEBUG_printf("LL_DEBUG, LF_LOG, " __FILELINE__ ": " __VA_ARGS__)

#define Log_FormattedMessageINFO(...) \
    DEBUG_printf("LL_DEBUG, LF_LOG_INFO, " __FILELINE__ ": " __VA_ARGS__)

#define Log_FormattedMessageWARN(...) \
    DEBUG_printf("LL_DEBUG, LF_LOG_WARN, " __FILELINE__ ": " __VA_ARGS__)

#define Log_FormattedMessageCRIT(...) \
    DEBUG_printf("LL_DEBUG, LF_LOG_CRIT, " __FILELINE__ ": " __VA_ARGS__)

#else

// debug logs are disabled
#define Log_Message(_str)
#define Log_FormattedMessage(...)
#define Log_FormattedMessageINFO(...)
#define Log_FormattedMessageWARN(...)
#define Log_FormattedMessageCRIT(...)

#endif /* IMPLDEFS_CF_DISABLE_DEBUG_L_PRINTF */

#endif /* Include Guard */

/* end of file log_impl.h */
