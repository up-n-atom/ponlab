/* workerthread_spalv2.c
 *
 * Worker Thread implementation, using SPALv2.
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

#include "spal_result.h"
#include "spal_semaphore.h"
#include "spal_thread.h"

typedef struct
{
    WorkerThread_HandlerFuncPtr_t HandlerFuncPtr;
    void * HandlerParam_p;
    SPAL_Semaphore_t * ThreadSem_p;
    SPAL_Semaphore_t * AckSem_p;          // used during creation
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
        SPAL_Semaphore_Wait(p->ThreadSem_p);

        // call the handler function
        p->HandlerFuncPtr(p->HandlerParam_p);

    } // (endless) while
}


/*----------------------------------------------------------------------------
 * WorkerThread_SPAL_Main
 *
 * This is the main function for a worker thread as required by SPAL.
 * The argument is a pointer to a set of arguments that we copy. We must then
 * signal
 */
static void *
WorkerThread_SPAL_Main(
        void * arg)
{
    WorkerThread_ThreadArgs_t Args;
    SPAL_Semaphore_t Sem;

    {
        SPAL_Result_t res;
        WorkerThread_ThreadArgs_t * const InOutArgs_p =
                            (WorkerThread_ThreadArgs_t *)arg;

        // create the worker-thread semaphore
        res = SPAL_Semaphore_Init(&Sem, /*initCount:*/0);

        if (res != SPAL_SUCCESS)
            return NULL;       // ## RETURN ##

        // copy the worker thread arguments onto our local stack
        Args = *InOutArgs_p;

        // return a  pointer to our local copy of the semaphore
        InOutArgs_p->ThreadSem_p = &Sem;

        // signal the creator that we have copied the arguments
        SPAL_Semaphore_Post(InOutArgs_p->AckSem_p);
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
    SPAL_Result_t res;
    SPAL_Thread_t ThreadId;
    WorkerThread_ThreadArgs_t Args;
    SPAL_Semaphore_t AckSem;

    IDENTIFIER_NOT_USED(Name_p);

    // create the thread-creation acknowledgement semaphore
    res = SPAL_Semaphore_Init(&AckSem, /*initCount:*/0);

    if (res != SPAL_SUCCESS)
        return false;       // ## RETURN ##

    // fill in the arguments block
    Args.HandlerFuncPtr = HandlerFuncPtr;
    Args.HandlerParam_p = HandlerParam_p;
    Args.ThreadSem_p = NULL;    // will be filled in by thread
    Args.AckSem_p = &AckSem;

    // create the worker thread and provide the arguments (by reference)
    res = SPAL_Thread_Create(
                &ThreadId,
                /*Reserved_p:*/NULL,
                WorkerThread_SPAL_Main,
                /*Arg:*/(void *)&Args);

    if (res != SPAL_SUCCESS)
    {
        // destroy the semaphore again
        SPAL_Semaphore_Destroy(&AckSem);
        return false;       // ## RETURN ##
    }

    // sleep on the Acknowledgement Semaphore to make sure that the worker thread
    // has started and copied the arguments.
    // this avoids us from reusing the stack on which the startup parameters
    // are placed before the worker thread has copied them.
    SPAL_Semaphore_Wait(&AckSem);

    // the worker thread reference we return is actually the semaphore pointer
    *WorkerThreadRef_p = Args.ThreadSem_p;

    // it is now OK to release the memory for Args

    // destroy the temporary semaphore again
    SPAL_Semaphore_Destroy(&AckSem);

    // check that worker thread reference is actually invalid
    if (Args.ThreadSem_p == NULL)
        return false;       // ## RETURN ##

    // let the thread run freely (this avoids valgrind warnings)
    SPAL_Thread_Detach(ThreadId);

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
        SPAL_Semaphore_Post(WorkerThreadRef);
    }
}

/* end of file workerthread_spalv2.c */
