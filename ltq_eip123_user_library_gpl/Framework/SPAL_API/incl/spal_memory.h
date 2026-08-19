/* spal_memory.h
 *
 * Description: Memory management routines
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

#ifndef INCLUDE_GUARD_SPAL_MEMORY_H_
#define INCLUDE_GUARD_SPAL_MEMORY_H_

#include "public_defs.h"

void *
SPAL_Memory_Alloc(
        const size_t Size);


void
SPAL_Memory_Free(
        void * const Memory_p);


void *
SPAL_Memory_Calloc(
        const size_t MemberCount,
        const size_t MemberSize);


void *
SPAL_Memory_ReAlloc(
        void * const Mem_p,
        size_t NewSize);

#endif /* Include guard */

/* end of file spal_memory.h */
