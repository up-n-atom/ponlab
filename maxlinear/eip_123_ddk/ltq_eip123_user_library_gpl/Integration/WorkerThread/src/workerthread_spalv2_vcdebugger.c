/* workerthread_spalv2_vcdebugger.c
 *
 * Worker Thread implementation, using SPALv2.
 * Additional intergration with the Microsoft Visual C Debugger,
 * specifically thread naming.
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

#include <windows.h>

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType;        // Must be 0x1000.
   LPCSTR szName;       // Pointer to name (in user addr space).
   DWORD dwThreadID;    // Thread ID (-1=caller thread).
   DWORD dwFlags;       // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

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
 * WorkerThread_SetThreadName
 *
 * This function tries to tell the Microsoft Visual C Debugger the name of the
 * thread by raising an exception to the debugger.
 */
static void
WorkerThread_SetThreadName(
        const uint32_t ThreadId,
        const char * Name_p)
{
    THREADNAME_INFO info;

    info.dwType = 0x1000;
    info.szName = Name_p;
    info.dwThreadID = (DWORD)ThreadId;
    info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
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
    // SPAL_Thread_Create() returns the thread handle, not the ThreadId that we need
    {
        HANDLE ThreadHandle;
        DWORD RealThreadId = 0;

        ThreadHandle = CreateThread(
                            /*lpThreadAttributes:*/NULL,
                            /*dwStackSize:*/0,
                            (LPTHREAD_START_ROUTINE)WorkerThread_SPAL_Main,
                            (void *)&Args,
                            /*dwCreationFlags:*/0,
                            &RealThreadId);     // part not supported by SPAL_Thread_Create

        if (ThreadHandle == NULL)
        {
            ThreadId = 0;
            res = SPAL_RESULT_NORESOURCE;
        }
        else
        {
            ThreadId = (SPAL_Thread_t)ThreadHandle;
            res = SPAL_SUCCESS;

            WorkerThread_SetThreadName(RealThreadId, Name_p);
        }
    }

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

/* end of file workerthread_spalv2_vcdebugger.c */
