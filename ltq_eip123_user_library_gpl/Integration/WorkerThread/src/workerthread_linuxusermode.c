/* workerthread_linuxusermode.c
 *
 * Worker Thread implementation for Linux User Mode Applications.
 */

/*****************************************************************************
* Copyright (c) 2008-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "basic_defs.h"
#include "workerthread.h"

#include <semaphore.h>          // sem_t, sem_init, destroy, wait
#include <pthread.h>            // pthread_t, pthread_create

typedef struct
{
    WorkerThread_HandlerFuncPtr_t HandlerFuncPtr;
    void * HandlerParam_p;
    sem_t * ThreadSem_p;
    sem_t * AckSem_p;          // used during creation
} WorkerThread_ThreadArgs_t;


/*----------------------------------------------------------------------------
 * WorkerThread_MainLoop
 *
 * This is the worker thread main loop that is invoked from the worker thread
 * function.
 */
static void
WorkerThread_MainLoop(
        WorkerThread_ThreadArgs_t * const p)
{
    while(p)
    {
        // acquire the worker thread semaphore
        // note: not using timeout version
        sem_wait(p->ThreadSem_p);

        // call the handler function
        p->HandlerFuncPtr(p->HandlerParam_p);

    } // (endless) while
}


/*----------------------------------------------------------------------------
 * WorkerThread_pthread_Main
 *
 * This is the main function for a worker thread as required by SPAL.
 * The argument is a pointer to a set of arguments that we copy. We must then
 * signal
 */
static void *
WorkerThread_pthread_Main(
        void * arg)
{
    WorkerThread_ThreadArgs_t Args;
    sem_t Sem;

    {
        WorkerThread_ThreadArgs_t * const InOutArgs_p =
                            (WorkerThread_ThreadArgs_t *)arg;

        // create the worker-thread semaphore
        if (sem_init(&Sem, /*shared:*/0, /*initCount:*/0) != 0)
            return NULL;       // ## RETURN ##

        // copy the worker thread arguments onto our local stack
        Args = *InOutArgs_p;

        // return a  pointer to our local copy of the semaphore
        InOutArgs_p->ThreadSem_p = &Sem;

        // signal the creator that we have copied the arguments
        sem_post(InOutArgs_p->AckSem_p);
    }

    Args.ThreadSem_p = &Sem;

    // invoke the handler function
    WorkerThread_MainLoop(&Args);

    // above function never returns, but we must return something
    return NULL;
}


/*----------------------------------------------------------------------------
 * WorkerThread_Start
 */
bool
WorkerThread_Start(
        const char * Name_p,                            // input
        WorkerThread_HandlerFuncPtr_t HandlerFuncPtr,   // input
        void * const HandlerParam_p,                    // input
        WorkerThreadRef_t * const WorkerThreadRef_p)    // output
{
    WorkerThread_ThreadArgs_t Args;
    sem_t AckSem;
    pthread_t ThreadId;

    IDENTIFIER_NOT_USED(Name_p);

    // create the thread-creation acknowledgement semaphore
    if (sem_init(&AckSem, /*shared:*/0, /*initCount:*/0) != 0)
        return false;       // ## RETURN ##

    // fill in the arguments block
    Args.HandlerFuncPtr = HandlerFuncPtr;
    Args.HandlerParam_p = HandlerParam_p;
    Args.ThreadSem_p = NULL;    // will be filled in by thread
    Args.AckSem_p = &AckSem;

    // create the worker thread and provide the arguments (by reference)
    if (pthread_create(
                &ThreadId,
                /*attr:*/ NULL,
                WorkerThread_pthread_Main,
                &Args) != 0)
    {
        // destroy the semaphore again
        sem_destroy(&AckSem);
        return false;       // ## RETURN ##
    }

    // sleep on the Acknowledgement Semaphore to make sure that the worker thread
    // has started and copied the arguments.
    // this avoids us from reusing the stack on which the startup parameters
    // are placed before the worker thread has copied them.
    sem_wait(&AckSem);

    // the worker thread reference we return is actually the semaphore pointer
    *WorkerThreadRef_p = Args.ThreadSem_p;

    // it is now OK to release the memory for Args

    // destroy the temporary semaphore again
    sem_destroy(&AckSem);

    // check that worker thread reference is actually invalid
    if (Args.ThreadSem_p == NULL)
        return false;       // ## RETURN ##

    return true;
}


/*----------------------------------------------------------------------------
 * WorkerThread_Signal
 *
 * Signals the worker thread. The pointer IS the handler to the semaphore
 * (WorkerThreadRef_t is a void *).
 * It is not possible to validate the pointer.
 */
void
WorkerThread_Signal(
        WorkerThreadRef_t WorkerThreadRef)    // input
{
    if (WorkerThreadRef)
    {
        // increase the semaphore
        sem_post(WorkerThreadRef);
    }
}

/* end of file workerthread_linuxusermode.c */
