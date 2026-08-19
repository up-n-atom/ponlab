/* spal_posix_sleep.c
 *
 * Description: POSIX specific implementation of SPAL Sleep API
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L /* Request IEEE 1003.1-2004 support. */
#endif /* _POSIX_C_SOURCE */

#include "spal_thread.h"
#include "implementation_defs.h"

#include <semaphore.h>
#include <time.h>
#include <errno.h>

/*----------------------------------------------------------------------------
 * SPAL_SleepMS
 */
void
SPAL_SleepMS(
        unsigned int Milliseconds)
{
    struct timespec WaitTime;
    sem_t Sem;
    int rval;

    // create a private semaphore that we can wait on until the timeout
    rval = sem_init(
            &Sem,
            /*pshared:*/0,
            /*Initial:*/0);     // blocks forever
    ASSERT(rval == 0);

    rval = clock_gettime(CLOCK_REALTIME, &WaitTime);
    ASSERT(rval == 0);

#define THOUSAND 1000
#define MILLION  1000000
#define BILLION  1000000000
    WaitTime.tv_sec += Milliseconds / THOUSAND;
    WaitTime.tv_nsec += (Milliseconds % THOUSAND) * MILLION;
    if (WaitTime.tv_nsec >= BILLION)
    {
        WaitTime.tv_sec += 1;
        WaitTime.tv_nsec -= BILLION;
    }
#undef BILLION
#undef MILLION
#undef THOUSAND

    // wait can be interrupted by certain (debug) signals
    do
    {
        rval = sem_timedwait(&Sem, &WaitTime);
    }
    while (rval != 0 && errno == EINTR);

    sem_destroy(&Sem);
}

/* end of file spal_posix_sleep.c */
