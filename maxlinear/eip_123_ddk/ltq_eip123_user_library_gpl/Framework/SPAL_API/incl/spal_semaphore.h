/* spal_semaphore.h
 *
 * Description: Semaphore APIs
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

#ifndef INCLUDE_GUARD_SPAL_SEMAPHORE_H_
#define INCLUDE_GUARD_SPAL_SEMAPHORE_H_

#include "public_defs.h"
#include "spal_result.h"

#include "cfg_spal.h"

struct SPAL_Semaphore
{
    union
    {
#ifdef SPAL_CFG_SEMAPHORE_ALIGN_TYPE
        SPAL_CFG_SEMAPHORE_ALIGN_TYPE Alignment;
#endif
        uint8_t Size[SPAL_CFG_SEMAPHORE_SIZE];
    } Union;
};

typedef struct SPAL_Semaphore SPAL_Semaphore_t;


SPAL_Result_t
SPAL_Semaphore_Init(
        SPAL_Semaphore_t * const Semaphore_p,
        const unsigned int InitialCount);


void
SPAL_Semaphore_Wait(
        SPAL_Semaphore_t * const Semaphore_p);


SPAL_Result_t
SPAL_Semaphore_TryWait(
        SPAL_Semaphore_t * const Semaphore_p);


SPAL_Result_t
SPAL_Semaphore_TimedWait(
        SPAL_Semaphore_t * const Semaphore_p,
        const unsigned int TimeoutMilliSeconds);


void
SPAL_Semaphore_Post(
        SPAL_Semaphore_t * const Semaphore_p);


void
SPAL_Semaphore_Destroy(
        SPAL_Semaphore_t * const Semaphore_p);

#endif /* Include guard */

/* end of file spal_semaphore.h */
