/* c_lib.h
 *
 * Description: Wrappers for C Library functions.
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

#ifndef INCLUDE_GUARD_C_LIB_H
#define INCLUDE_GUARD_C_LIB_H

#include "public_defs.h"

void *
c_memcpy(
        void * restrict s1,
        const void * restrict s2,
        size_t n);

void *
c_memmove(
        void * s1,
        const void * s2,
        size_t n);


void *
c_memset(
        void *s,
        int c,
        size_t n);


int
c_memcmp(
        const void * s1,
        const void * s2,
        size_t n);


int
c_strcmp(
        const char * s1,
        const char * s2);


char *
c_strcpy(
        char * restrict s1,
        const char * restrict s2);

char *
c_strcat(
        char * dest,
        const char * src);

char *
c_strncpy(
        char * dest,
        const char * src,
        size_t n);

int
c_strncmp(
        const char * s1,
        const char * s2,
        size_t n);

size_t
c_strlen(
        const char * s);

char *
c_strstr(
        const char * str1,
        const char * str2);


long
c_strtol(
        const char * str,
        char ** endptr,
        int16_t radix);

char *
c_strchr(
        const char * str,
        int32_t c);

int
c_tolower(
        int c);

int
c_toupper(
        int c);


int *
c_memchr(
        const void * buf,
        int32_t ch,
        size_t num);

#endif /* Include guard */

/* end of file c_lib.h */
