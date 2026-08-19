/* log_safezone.c
 *
 * Log implementation for specific environment
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

#define LOG_SEVERITY_MAX  LOG_SEVERITY_NO_OUTPUT

#include "log.h"            // the API to implement


/*----------------------------------------------------------------------------
 * Log_HexDump
 *
 * This function logs Hex Dump of a Buffer
 *
 * szPrefix
 *     Prefix to be printed on every row.
 *
 * PrintOffset
 *     Offset value that is printed at the start of every row. Can be used
 *     when the byte printed are located at some offset in another buffer.
 *
 * Buffer_p
 *     Pointer to the start of the array of bytes to hex dump.
 *
 * ByteCount
 *     Number of bytes to include in the hex dump from Buffer_p.
 *
 * Return Value
 *     None.
 */
void
Log_HexDump_SafeZone(
        const char * szPrefix_p,
        const unsigned int PrintOffset,
        const uint8_t * Buffer_p,
        const unsigned int ByteCount,
        const char * FileLineStr_p)
{
    int BytesInThisLine = 16;
    unsigned int i;

    for(i = 0; i < ByteCount; i += 16)
    {
        // if we do not have enough data for a full line
        // then modify the line to cut off the printout

        // work-around due to the SafeZone macros
        // we have also dropped the newline
        if (i + 16 > ByteCount)
            BytesInThisLine = ByteCount - i;

        {
            char LineBuf[(3 * 16) + 1];
            char * p = LineBuf;
            int lp;

            for (lp = 0; lp < BytesInThisLine; lp++)
            {
                uint8_t b = Buffer_p[i + lp];
                char c;

                *p++ = ' ';

                c = (b >> 4) + '0';
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;

                c = (b & MASK_4_BITS) + '0';
                if (c > '9')
                    c = c - '0' - 10 + 'A';
                *p++ = c;
            } // for

            *p = 0;     // zero-terminate the string

            DEBUG_printf(
                "LL_DEBUG, LF_LOG, %s: "
                "%s %08d:%s\n",
                FileLineStr_p,
                szPrefix_p,
                PrintOffset + i,
                LineBuf);
        }
    } // for
}


/* end of file log_safezone.c */
