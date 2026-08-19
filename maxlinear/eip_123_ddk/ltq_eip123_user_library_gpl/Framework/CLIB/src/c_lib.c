/* c_lib.c
 *
 * Description: Wrappers for C Library functions
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
#include "c_lib.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void *
c_memcpy(
        void * restrict s1,
        const void * restrict s2,
        size_t n)
{
    return memcpy(s1, s2, n);
}


void *
c_memmove(
        void * s1,
        const void * s2,
        size_t n)
{
    return memmove(s1, s2, n);
}

void *
c_memset(
        void * s,
        int c,
        size_t n)
{
    return memset(s, c, n);
}


int
c_memcmp(
        const void * s1,
        const void * s2,
        size_t n)
{
    return memcmp(s1, s2, n);
}

int
c_strcmp(
        const char * s1,
        const char * s2)
{
    return strcmp(s1, s2);
}

int
c_strncmp(
        const char *s1,
        const char *s2,
        size_t n)
{
    return strncmp(s1, s2, n);
}

char *
c_strcpy(
        char * restrict s1,
        const char * restrict s2)
{
    return strcpy(s1, s2);
}

char *
c_strcat(
        char * dest,
        const char * src)
{
    return strcat(dest,src);
}

char *
c_strncpy(
        char * dest,
        const char * src,
        size_t n)
{
    return strncpy(dest, src, n);
}

size_t
c_strlen(
        const char *s)
{
    return strlen(s);
}

char *
c_strstr(
        const char * str1,
        const char * str2)
{
    return strstr(str1, str2);
}

long
c_strtol(
        const char * str,
        char ** endptr,
        int16_t radix)
{
    return strtol(str, endptr, radix);
}

char *
c_strchr (const char * str, int32_t c)
{
    return strchr(str, c);
}

int
c_tolower(int c)
{
    return tolower(c);
}

int
c_toupper(int c)
{
    return toupper(c);
}

int *
c_memchr(
        const void * buf,
        int32_t ch,
        size_t num)
{
    return memchr(buf, ch, num);
}

/* end of file c_lib.c */
