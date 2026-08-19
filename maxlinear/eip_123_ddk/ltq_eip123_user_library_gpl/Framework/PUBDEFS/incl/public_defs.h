/* public_defs.h
 *
 * Description: See below.
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

/*
  This header provides public definitions required by the SafeZone
  Software Modules. The definitions are considered public, because
  they are required by the public API headers.

  - C99 exact-width integer types, e.g. uint32_t
  - C99 Boolean

  This particular header should is used for building on basic
  development platforms that are Linux and Win32.

*/

#ifndef INCLUDE_GUARD_PUBLIC_DEFS_H
#define INCLUDE_GUARD_PUBLIC_DEFS_H

#if defined(WIN32)

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint32_t size_t;
typedef uint32_t uintptr_t;

typedef uint8_t bool;

#define true 1
#define false 0

#define INT8_MIN (-128)
#define INT8_MAX  (127)

#define INT16_MIN (-32768)
#define INT16_MAX (32767)

#define INT32_MIN (-2147483647-1)
#define INT32_MAX (2147483647)

#define INT64_MIN  (-9223372036854775808LL)
#define INT64_MAX  (9223372036854775807LL)
#define UINT64_MAX (18446744073709551615ULL)

#define UINT8_MAX  (255)

#define UINT16_MAX (65535)

#define UINT32_MAX (4294967295)

#define restrict

#elif (defined(__GNUC__) || defined(__CC_ARM))

#include <stdint.h>        // exact width integers
#include <stdbool.h>       // Boolean
#include <stddef.h>        // offsetof, size_t

#else
#error Unsupported platform
#endif

#endif /* Include guard */

/* end of file public_defs.h */
