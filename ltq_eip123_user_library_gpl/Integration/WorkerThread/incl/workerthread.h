/* workerthread.h
 *
 * Worker Thread API specification.
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

#ifndef INCLUDE_GUARD_WORKERTHREAD_H
#define INCLUDE_GUARD_WORKERTHREAD_H

#include "basic_defs.h"

/*
 * A Worker Thread is an execution context that can sleep when there is
 * nothing to do. It can be signalled, which will cause it to call a function
 * that performs work. When the function returns, the thread will sleep
 * again.
 *
 * The implementation of the worker threads is environment-specific and can
 * be used from an adapter layer to defer work to another execution context,
 * including interrupt context.
 *
 * The worker thread implementation should not use any global storage and
 * instead keep all data on the stack. It must be possible to create multiple
 * worker threads and signal a specific instance.
 *
 * The implementation must be able to create the execution context including
 * its stack, or get it from somewhere predefined. The method for sleeping
 * the worker thread must allow multiple outstanding requests as typically
 * provided by a semaphore.
 *
 * The worker thread never exists / completes / destroys.
 */


/*----------------------------------------------------------------------------
 * WorkerThread_HandlerFuncPtr_t
 *
 * Pointer to the caller-provided handler function that will be invoked by
 * the worker thread execution context.
 *
 * HandlerParam_p
 *     Pointer to parameter block as provided to WorkerThread_Start.
 *
 * There is no result code.
 */

typedef void (* WorkerThread_HandlerFuncPtr_t)(void * const HandlerParam_p);

typedef void * WorkerThreadRef_t;


/*----------------------------------------------------------------------------
 * WorkerThread_Start
 *
 * This function is called to start the worker thread. It accepts the pointer
 * to the handler function. The implementation must create the semaphore and
 * thread and then loop forever doing this:
 *  - sleep on the semaphore
 *  - call the handler function
 *
 * Name_p
 *    Name of this worker thread, allowing the implementation to select
 *    implicit configuration decisions based on the name - this is a contract
 *    between the user of the API and the implementation.
 *
 * HandlerFuncPtr
 *    Pointer to the function that will be invoked when the worker thread
 *    is signalled (wakes up).
 *
 * HandlerParam_p
 *    Pointer that will be provided as-is when invoking the handler function.
 *
 * WorkerThreadRef_p
 *    Output parameter: reference to the worker thread to be used when calling
 *    the Signal function.
 *
 * Returns true on success, false on failure to start the worker thread.
 */
bool
WorkerThread_Start(
        const char * Name_p,                            // input
        WorkerThread_HandlerFuncPtr_t HandlerFuncPtr,   // input
        void * const HandlerParam_p,                    // input
        WorkerThreadRef_t * const WorkerThreadRef_p);   // output


/*----------------------------------------------------------------------------
 * WorkerThread_Signal
 *
 * This function must be used by the caller when it wants the handler function
 * to be invoked. In response to this call the worker thread will wake up and
 * do its task as described above.
 *
 * Exactly when the worker thread calls the handler function is implementation
 * specific and typically depends on the OS scheduler behaviour and the
 * priority of the worker thread in relation to the caller. If the caller is
 * an interrupt context, the worker thread is typically invoked first when the
 * interrupt handler has completed.
 *
 * The number of invocations to the handler function must equal the number of
 * times the handler function is invoked. The handler function is guaranteed
 * to be invoked when this function has been called. If it was already running
 * it will be called once more.
 */
void
WorkerThread_Signal(
        WorkerThreadRef_t WorkerThreadRef);   // input


#endif /* Include Guard */

/* end of file workerthread.h */
