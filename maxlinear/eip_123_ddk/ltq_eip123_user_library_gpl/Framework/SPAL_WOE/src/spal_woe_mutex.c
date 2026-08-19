/* spal_woe_mutex.c
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

#include "spal_mutex.h"
#include "implementation_defs.h"

#include <windows.h>

#define SPAL_MAGIC_MUTEX 0x34523623U
#define NO_OWNER (DWORD)(-1)

struct SPAL_MutexWoe
{
    uint32_t Magic;
    bool Locked;
    DWORD Owner;
    CRITICAL_SECTION CriticalSection;
};

typedef struct SPAL_MutexWoe SPAL_MutexWoe_t;

COMPILE_GLOBAL_ASSERT(sizeof(SPAL_MutexWoe_t) <= sizeof(SPAL_Mutex_t));

SPAL_Result_t
SPAL_Mutex_Init(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;

    PRECONDITION(Mutex_p != NULL);

    InitializeCriticalSection(&m_p->CriticalSection);

    m_p->Owner = NO_OWNER;
    m_p->Magic = SPAL_MAGIC_MUTEX;
    m_p->Locked = false;

    return SPAL_SUCCESS;
}


void
SPAL_Mutex_Lock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;

    PRECONDITION(Mutex_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);
    PRECONDITION(m_p->Owner != GetCurrentThreadId());

    EnterCriticalSection(&m_p->CriticalSection);

    ASSERT(m_p->Locked == false);
    m_p->Locked = true;
    m_p->Owner = GetCurrentThreadId();
}


void
SPAL_Mutex_UnLock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;

    PRECONDITION(Mutex_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);
    PRECONDITION(m_p->Locked == true);
    PRECONDITION(m_p->Owner == GetCurrentThreadId());

    m_p->Locked = false;
    m_p->Owner = NO_OWNER;

    LeaveCriticalSection(&m_p->CriticalSection);
}

void
SPAL_Mutex_Destroy(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;

    PRECONDITION(Mutex_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);
    PRECONDITION(m_p->Locked == false);

    DeleteCriticalSection(&m_p->CriticalSection);

    m_p->Magic = ~SPAL_MAGIC_MUTEX;
}

bool
SPAL_Mutex_IsLocked(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;

    PRECONDITION(Mutex_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    return m_p->Locked;
}


SPAL_Result_t
SPAL_Mutex_TryLock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexWoe_t * const m_p = (SPAL_MutexWoe_t * const) Mutex_p;
    BOOL rbool;

    PRECONDITION(Mutex_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);
    PRECONDITION(m_p->Owner != GetCurrentThreadId());

    if (m_p->Locked)
    {
        return SPAL_RESULT_LOCKED;
    }

    rbool =
        TryEnterCriticalSection(
                &m_p->CriticalSection);

    if (rbool != 0)
    {
        m_p->Locked = true;
        m_p->Owner = GetCurrentThreadId();

        return SPAL_SUCCESS;
    }

    return SPAL_RESULT_LOCKED;
}

/* end of file spal_woe_mutex.c */
