/**
*  File: spal_posix_mutex.c
*
*  Description : Posix Mutex  APIs
*
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
*/

#include "spal_mutex.h"
#include "implementation_defs.h"

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define SPAL_MAGIC_MUTEX 0x55555555

struct SPAL_MutexPosix
{
    uint32_t Magic;
    pthread_mutex_t Mutex;
};

typedef struct SPAL_MutexPosix SPAL_MutexPosix_t;

COMPILE_GLOBAL_ASSERT(sizeof(SPAL_MutexPosix_t) <= sizeof(SPAL_Mutex_t));

SPAL_Result_t
SPAL_Mutex_Init(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;

    PRECONDITION(m_p != NULL);

    if (pthread_mutex_init(&m_p->Mutex, NULL) != 0)
    {
        m_p->Magic = 0;

        POSTCONDITION(m_p->Magic != SPAL_MAGIC_MUTEX);

        return SPAL_RESULT_NORESOURCE;
    }

    m_p->Magic = SPAL_MAGIC_MUTEX;

    POSTCONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    return SPAL_SUCCESS;
}


void
SPAL_Mutex_Lock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;
    int rval;

    PRECONDITION(m_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    rval = pthread_mutex_lock(&m_p->Mutex);

    ASSERT(rval == 0);
}


void
SPAL_Mutex_UnLock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;
    int rval;

    PRECONDITION(m_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    rval = pthread_mutex_unlock(&m_p->Mutex);

    ASSERT(rval == 0);
}


void
SPAL_Mutex_Destroy(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;
    int rval;

    PRECONDITION(m_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    rval = pthread_mutex_destroy(&m_p->Mutex);
    m_p->Magic = 0;

    ASSERT(rval == 0);

    POSTCONDITION(m_p->Magic != SPAL_MAGIC_MUTEX);
}


bool
SPAL_Mutex_IsLocked(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;
    int rval;

    PRECONDITION(m_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    rval = pthread_mutex_trylock(&m_p->Mutex);

    ASSERT(rval == EBUSY || rval == 0);

    if (rval == EBUSY)
    {
        return true;
    }

    rval = pthread_mutex_unlock(&m_p->Mutex);

    ASSERT(rval == 0);

    return false;
}


SPAL_Result_t
SPAL_Mutex_TryLock(
        SPAL_Mutex_t * const Mutex_p)
{
    SPAL_MutexPosix_t * const m_p = (SPAL_MutexPosix_t * const) Mutex_p;
    int rval;

    PRECONDITION(m_p != NULL);
    PRECONDITION(m_p->Magic == SPAL_MAGIC_MUTEX);

    rval = pthread_mutex_trylock(&m_p->Mutex);

    ASSERT(rval == EBUSY || rval == 0);

    if (rval == EBUSY)
    {
        return SPAL_RESULT_LOCKED;
    }

    return SPAL_SUCCESS;
}

/* end of file spal_posix_mutex.c */
