/* clib.h
 *
 * C Library Abstraction
 *
 * This header function guarantees the availability of a select list of
 * Standard C APIs. This makes the user of this API compiler independent.
 * It also gives a single customization point for these functions.
 *
 * THIS IMPLEMENTATION IS FOR SPALv2
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

#ifndef INCLUDE_GUARD_CLIB_H
#define INCLUDE_GUARD_CLIB_H

/* guaranteed APIs:

    memcpy
    memmove
    memset
    memcmp
    offsetof
    strcmp
*/

#include "c_lib.h"

#define memcpy c_memcpy
#define memmove c_memmove
#define memset c_memset
#define memcmp c_memcmp
#define strcmp c_strcmp


/* Zero-init macro
 *
 *   _x (input)
 *              Name of the variable that must be zeroed
 *
 */
#define ZEROINIT(_x)  memset(&_x, 0, sizeof(_x))


#endif /* Inclusion Guard */

/* end of file clib.h */
