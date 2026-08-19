/* spal_woe_semaphore.c
 *
 * Description: Semaphore API implementation for Win32
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

#include "spal_semaphore.h"
#include "implementation_defs.h"

#include <windows.h>

#define SPAL_MAGIC_SEMAPHORE 0x34523624U

struct SPAL_SemaphoreWoe
{
    uint32_t Magic;
    HANDLE Sem;
};

typedef struct SPAL_SemaphoreWoe SPAL_SemaphoreWoe_t;

COMPILE_GLOBAL_ASSERT(sizeof(SPAL_SemaphoreWoe_t) <= sizeof(SPAL_Semaphore_t));


SPAL_Result_t
SPAL_Semaphore_Init(
        SPAL_Semaphore_t * const Semaphore_p,
        const unsigned int InitialCount)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    HANDLE Sem;

    PRECONDITION(Semaphore_p != NULL);

    Sem =
        CreateSemaphore(
                /* lpSemaphoreAttributes: */ NULL,
                InitialCount,
                /* lMaximumCount: */ 100,
                /* lpName: */ NULL);
    if (Sem == NULL)
    {
        Sem_p->Magic = ~SPAL_MAGIC_SEMAPHORE;

        return SPAL_RESULT_NORESOURCE;
    }

    Sem_p->Sem = Sem;
    Sem_p->Magic = SPAL_MAGIC_SEMAPHORE;

    return SPAL_SUCCESS;
}


void
SPAL_Semaphore_Wait(
        SPAL_Semaphore_t * const Semaphore_p)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    DWORD rval;

    PRECONDITION(Semaphore_p != NULL);
    PRECONDITION(Sem_p->Magic == SPAL_MAGIC_SEMAPHORE);

    rval =
        WaitForSingleObject(
                Sem_p->Sem,
                /* dwMilliseconds : */ INFINITE);

    ASSERT(rval == WAIT_OBJECT_0);
}


SPAL_Result_t
SPAL_Semaphore_TryWait(
        SPAL_Semaphore_t * const Semaphore_p)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    DWORD rval;

    PRECONDITION(Semaphore_p != NULL);
    PRECONDITION(Sem_p->Magic == SPAL_MAGIC_SEMAPHORE);

    rval =
        WaitForSingleObject(
                Sem_p->Sem,
                /* dwMilliseconds : */ 0);

    if (rval == WAIT_TIMEOUT)
    {
        return SPAL_RESULT_LOCKED;
    }

    ASSERT(rval == WAIT_OBJECT_0);

    return SPAL_SUCCESS;
}



SPAL_Result_t
SPAL_Semaphore_TimedWait(
        SPAL_Semaphore_t * const Semaphore_p,
        const unsigned int TimeoutMilliSeconds)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    DWORD rval;

    PRECONDITION(Semaphore_p != NULL);
    PRECONDITION(Sem_p->Magic == SPAL_MAGIC_SEMAPHORE);

    rval =
        WaitForSingleObject(
                Sem_p->Sem,
                TimeoutMilliSeconds);

    if (rval == WAIT_TIMEOUT)
    {
        return SPAL_RESULT_TIMEOUT;
    }

    ASSERT(rval == WAIT_OBJECT_0);

    return SPAL_SUCCESS;
}



void
SPAL_Semaphore_Post(
        SPAL_Semaphore_t * const Semaphore_p)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    BOOL rbool;

    PRECONDITION(Semaphore_p != NULL);
    PRECONDITION(Sem_p->Magic == SPAL_MAGIC_SEMAPHORE);

    rbool =
        ReleaseSemaphore(
                Sem_p->Sem,
                /* lReleaseCount: */ 1,
                /* lpPreviousCount: */ NULL);

    ASSERT(rbool != 0);
}



void
SPAL_Semaphore_Destroy(
        SPAL_Semaphore_t * const Semaphore_p)
{
    SPAL_SemaphoreWoe_t * const Sem_p =
            (SPAL_SemaphoreWoe_t * const) Semaphore_p;
    BOOL rbool;

    PRECONDITION(Semaphore_p != NULL);
    PRECONDITION(Sem_p->Magic == SPAL_MAGIC_SEMAPHORE);

    rbool =
        CloseHandle(
                Sem_p->Sem);

    Sem_p->Magic = ~SPAL_MAGIC_SEMAPHORE;

    ASSERT(rbool != 0);
}

/* end of file spal_woe_semaphore.c */
