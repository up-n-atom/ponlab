/* intdispatch_umdevxs.c
 *
 * This API enables callers to register interest in one or more interrupts.
 * The Interrupt Dispatcher SW module handles the top-level interrupt
 * controllers (one or two), decodes the interrupts and dispatches these to
 * the interested parties.
 */

/*****************************************************************************
* Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_intdispatch_umdevxs.h"   // mapping of names to sources

#include "intdispatch_mgmt.h"        // the API to implement, part 1
#include "intdispatch.h"             // the API to implement, part 2

#include "umdevxsproxy_interrupt.h"  // interrupt support
#include "umdevxsproxy_device.h"     // for EIP-201 access

// for EIP-201 concurrency protection:
#include <time.h>                    // time_t, time
#include <errno.h>                   // errno, EINTR
#include <semaphore.h>               // sem_t, sem_post, sem_timedwait, etc.

#include "workerthread.h"

#define LOG_SEVERITY_MAX  INTDISPATCH_LOG_SEVERITY
#include "log.h"

#include "basic_defs.h"              // NULL, inline
#include "clib.h"                    // strcmp
#include "device_mgmt.h"             // Device_Initialize, Device_Find
#include "eip201.h"                  // Advanced Interrupt Controller

#define ELEMENTS_COUNT(_x) (sizeof(_x) / sizeof(_x[0]))


/*-----------------------------------------------------------------------------
 * IntDispatchLib_AIC#_IntSettings[]
 *
 * Initialization data for each AIC.
 */

#define INTDISPATCH_RESOURCE_ADD(_name, _sources, _config) \
            { _sources, EIP201_CONFIG_##_config, false }
/*
typedef struct
{
    EIP201_Source_t Source;
    EIP201_Config_t Config;
    bool fEnable;
} EIP201_SourceSettings_t;
*/

static const EIP201_SourceSettings_t IntDispatchLib_AIC0_IntSettings[] =
{
    INTDISPATCH_RESOURCES_0
};

#ifdef INTDISPATCH_DEVICE_EIP201_1
static const EIP201_SourceSettings_t IntDispatchLib_AIC1_IntSettings[] =
{
    INTDISPATCH_RESOURCES_1
};
#endif /* INTDISPATCH_DEVICE_EIP201_1 */

#undef INTDISPATCH_RESOURCE_ADD


/*-----------------------------------------------------------------------------
 * IntDispatchLib_InterruptInfo[]
 *
 * Constant information about each interrupt source.
 * The relation to IntDispatchLib_HookAdmin[] is 1:1 (same index).
 * IntDispatchLib_IntCount reflects the number of entries.
 */
typedef struct
{
    const char * szIntName_p;
    uint8_t AIC_Nr;
    EIP201_Source_t Sources;
} IntDispatchLib_InterruptInfo_t;

static const IntDispatchLib_InterruptInfo_t IntDispatchLib_InterruptInfo[] =
{

#undef  INTDISPATCH_RESOURCE_ADD
#define INTDISPATCH_RESOURCE_ADD(_name, _sources, _config) \
    { _name, /*AIC_Nr:*/0, _sources }

    INTDISPATCH_RESOURCES_0

#undef  INTDISPATCH_RESOURCE_ADD
#define INTDISPATCH_RESOURCE_ADD(_name, _sources, _config) \
    { _name, /*AIC_Nr:*/1, _sources }

#ifdef INTDISPATCH_DEVICE_EIP201_1
    ,
    INTDISPATCH_RESOURCES_1
#endif /* INTDISPATCH_DEVICE_EIP201_1 */
};


/*-----------------------------------------------------------------------------
 * IntDispatchLib_HookAdmin[]
 *
 * Administration data for each hookable interrupt source.
 * The relation to IntDispatchLib_InterruptInfo[] is 1:1 (same index).
 * IntDispatchLib_IntCount reflects the number of entries.
 */

typedef struct
{
    bool fIsHooked;
    void * Arg_p;
    IntDispatch_Callback_t CBFunc_p;
} IntDispatchLib_HookAdmin_t;

#undef  INTDISPATCH_RESOURCE_ADD
#define INTDISPATCH_RESOURCE_ADD(_name, _sources, _config) { false }

static IntDispatchLib_HookAdmin_t IntDispatchLib_HookAdmin[] =
{
    INTDISPATCH_RESOURCES_0
#ifdef INTDISPATCH_DEVICE_EIP201_1
    ,
    INTDISPATCH_RESOURCES_1
#endif /* INTDISPATCH_DEVICE_EIP201_1 */
};


static const int IntDispatchLib_IntCount = ELEMENTS_COUNT(IntDispatchLib_HookAdmin);

static WorkerThreadRef_t IntDispatchLib_WorkerThreadRef = NULL;
static Device_Handle_t IntDispatchLib_Devices[2];

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
static sem_t IntDispatchLib_MutexEIP201;
#endif


/*----------------------------------------------------------------------------
 * IntDispatchLib_NotInitialized
 *
 * Evaluates to 'true' when the Interrupt Dispatcher is not initialized.
 */
static inline bool
IntDispatchLib_NotInitialized(void)
{
    return (IntDispatchLib_WorkerThreadRef == NULL) ? true : false;
}


/*----------------------------------------------------------------------------
 * IntDispatchLib_AdminIndex2Handle
 */
static inline IntDispatch_Handle_t
IntDispatchLib_AdminIndex2Handle(
        int AdminIndex)
{
    return (IntDispatch_Handle_t)(BIT_16 + AdminIndex);
}


/*----------------------------------------------------------------------------
 * IntDispatchLib_Handle2AdminIndex
 *
 * Return the index or <0 on error.
 */
static inline int
IntDispatchLib_Handle2AdminIndex(
        IntDispatch_Handle_t Handle)
{
    int AdminIndex = (int)Handle;

    if (AdminIndex & BIT_16)
    {
        AdminIndex ^= BIT_16;

        if (AdminIndex >= 0 && AdminIndex < IntDispatchLib_IntCount)
            return AdminIndex;
    }

    return -1;
}


/*----------------------------------------------------------------------------
 * IntDispatchLib_MutexEIP201_TryLock
 *
 * This function tries to obtain the mutex for EIP-201 concurrency protection
 * and returns whether this was successful. A timeout between 1 and 2 seconds
 * (1 second granularity) prevents against dead-locks.
 *
 * 0 = success, <0 = failed (includes timeout)
 */
#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
static inline int
IntDispatchLib_MutexEIP201_TryLock(void)
{
    int res;
    struct timespec ts = { 0 };

    // ensure we are initialized
    if (IntDispatchLib_NotInitialized())
        return -99;

    // get current clock
    {
        time_t t;
        t = time(NULL);

        if (t == -1)
            return -1;

        // set up ts based on t
        ts.tv_sec = t;
    }

    // set timeout moment 1 second into the future
    // due to rounding (we could be close to 1 second), we use 2
    // effective wait = 1,0..2,0 seconds; average 1,5 seconds
    ts.tv_sec += 2;

    // wait until we get the mutex
    do
    {
        res = sem_timedwait(
                        &IntDispatchLib_MutexEIP201,
                        &ts);

        // success?
        if (res == 0)
            return 0;     // ## RETURN ##

        // loop if signal handler interruption
    }
    while (res != 0 && errno == EINTR);

    LOG_INFO(
        "IntDispatchLib_MutexEIP201_TryLock: "
        "sem_timedwait failed: "
        "res=%d, "
        "errno=%d\n",
        res,
        errno);

    // timeout or other error
    return -2;
}
#endif /* INTDISPATCH_EIP201_NOT_REENTRANT */


/*----------------------------------------------------------------------------
 * IntDispatchLib_MutexEIP201_Unlock
 */
#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
static inline void
IntDispatchLib_MutexEIP201_Unlock(void)
{
    sem_post(&IntDispatchLib_MutexEIP201);
}
#endif /* INTDISPATCH_EIP201_NOT_REENTRANT */


/*----------------------------------------------------------------------------
 * IntDispatchLib_CheckAndDispatchNow
 *
 * This function is called when an interrupt was received. The source is read
 * from EIP-201 and acknowledged. We then decode the active interrupts and
 * invoked the registered callback functions.
 */
static void
IntDispatchLib_CheckAndDispatchNow(
        const unsigned int AIC_Nr)
{
    EIP201_SourceBitmap_t Sources;
    int i;

    // read the active interrupts from EIP201
    Sources = EIP201_SourceStatus_ReadAllEnabled(
                           IntDispatchLib_Devices[AIC_Nr]);

    // allow early finish
    if (Sources == 0)
        return;         // ## RETURN ##

    // acknowledge these interrupts
    (void)EIP201_Acknowledge(
                IntDispatchLib_Devices[AIC_Nr],
                Sources);

    // now decode the source(s) and call the appropriate hook function
    for(i = 0; i < IntDispatchLib_IntCount; i++)
    {
        const IntDispatchLib_InterruptInfo_t * Info_p;
        Info_p = IntDispatchLib_InterruptInfo + i;

        if (Info_p->AIC_Nr == AIC_Nr)
        {
            EIP201_SourceBitmap_t Active;

            Active = Info_p->Sources & Sources;

            if (Active)
            {
                const IntDispatchLib_HookAdmin_t * p;
                const EIP201_SourceBitmap_t TraceFilter =
                                    AIC_Nr == 0 ? INTDISPATCH_TRACE_FILTER_0
                                                : INTDISPATCH_TRACE_FILTER_1;

                // clear these bits from Sources
                Sources &= ~Active;

                if (Active & TraceFilter)
                {
                    Log_FormattedMessage(
                        "IntDispatcher: "
                        "Invoking for AIC %u sources 0x%08x\n",
                        AIC_Nr,
                        Active);
                }

                p = IntDispatchLib_HookAdmin + i;

                if (p->fIsHooked)
                {
                    p->CBFunc_p(p->Arg_p);
                }
                else
                {
                    LOG_WARN(
                        "IntDispatcher: "
                        "Cannot dispatch for AIC %d sources 0x%08x\n",
                        AIC_Nr,
                        Active);

                    // we should disable these sources
                    // to prevent this from happening again
                    // but this requires concurrency protection with other callers
                    // (until we support EIP-201 HW1.1)
                }
            }

            // allow early finish
            if (Sources == 0)
                return;         // ## RETURN ##

        } // if correct AIC
    } // for
}


/*----------------------------------------------------------------------------
 * IntDispatchLib_HandlerFunc
 *
 * This function is called by the worker thread library every time it is
 * signalled. We never return from this function and instead use its
 * execution context.
 *
 * We wait for interrupts reported by the kernel driver by calling the
 * UMDevXS Proxy to wait for such an event. The proxy call will block until
 * the interrupt or the timeout occurs.
 *
 * This function then calls the interrupt dispatcher to check the EIP-201's
 * for interrupts and call the registered callback functions.
 */
static void
IntDispatchLib_HandlerFunc(
        void * const HandlerParam_p)
{
    int res;

    do
    {
        res = UMDevXSProxy_Interrupt_WaitWithTimeout(/*timeout_ms:*/500);

        if (res == 0)
        {
            // interrupt (not timeout)

            // check first AIC
            IntDispatchLib_CheckAndDispatchNow(0);

            // check second AIC (optional)
#ifdef INTDISPATCH_DEVICE_EIP201_1
            IntDispatchLib_CheckAndDispatchNow(1);
#endif
        }
    }
    while(res >= 0);

    IDENTIFIER_NOT_USED(HandlerParam_p);
}


/*----------------------------------------------------------------------------
 * IntDispatch_Initialize
 *
 * This function creates the interrupt dispatcher context, which will call
 * the UMDevXS Proxy to receive interrupt events from the kernel domain.
 */
int
IntDispatch_Initialize(void)
{
    if (!IntDispatchLib_NotInitialized())
    {
        // already initialized
        // silently allow
        return 0;
    }

    LOG_INFO(
        "IntDispatch_Initialize: "
        "Initializing\n");

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
    // initialize the EIP-201 concurrency protection semaphore
    sem_init(
            &IntDispatchLib_MutexEIP201,
            /*shared:*/0,
            /*initial value:*/1);
#endif

    // find the first EIP-201 device
    IntDispatchLib_Devices[0] = Device_Find(INTDISPATCH_DEVICE_EIP201_0);
    if (IntDispatchLib_Devices[0] == NULL)
    {
        // failed to find EIP-201
        LOG_WARN(
            "IntDispatch_Initialize: "
            "Failed to find Device named %s\n",
            INTDISPATCH_DEVICE_EIP201_0);

        return -20;
    }

#ifdef INTDISPATCH_DEVICE_EIP201_1
    // find the second EIP-201 device
    IntDispatchLib_Devices[1] = Device_Find(INTDISPATCH_DEVICE_EIP201_1);
    if (IntDispatchLib_Devices[1] == NULL)
    {
        // failed to find EIP-201
        LOG_WARN(
            "IntDispatch_Initialize: "
            "Failed to find Device named %s\n",
            INTDISPATCH_DEVICE_EIP201_0);

        return -21;
    }
#endif /* INTDISPATCH_DEVICE_EIP201_1 */

    // configure the EIP-201
    {
        EIP201_Status_t res201;

        res201 = EIP201_Initialize(
                        IntDispatchLib_Devices[0],
                        IntDispatchLib_AIC0_IntSettings,
                        ELEMENTS_COUNT(IntDispatchLib_AIC0_IntSettings));

        if (res201 != EIP201_STATUS_SUCCESS)
        {
            LOG_INFO(
                "IntDispatch_Initialize: "
                "EIP201_Initialize returned %d (AIC %d)\n",
                res201,
                0);

            return -30;
        }

#ifdef INTDISPATCH_DEVICE_EIP201_1
        res201 = EIP201_Initialize(
                        IntDispatchLib_Devices[1],
                        IntDispatchLib_AIC1_IntSettings,
                        ELEMENTS_COUNT(IntDispatchLib_AIC1_IntSettings));

        if (res201 != EIP201_STATUS_SUCCESS)
        {
            LOG_INFO(
                "IntDispatch_Initialize: "
                "EIP201_Initialize returned %d (AIC %d)\n",
                res201,
                1);

            return -31;
        }
#endif /* INTDISPATCH_DEVICE_EIP201_1 */
    }

    // create the worker thread context
    // (it will be used to wait for kernel events using UMDevXS Proxy)
    if (!WorkerThread_Start(
                    "Interrupt Dispatcher",
                    IntDispatchLib_HandlerFunc,
                    /*HandlerParam_p:*/NULL,
                    &IntDispatchLib_WorkerThreadRef))
    {
        LOG_WARN(
            "IntDispatch_Initialize: "
            "Failed to create the worker thread\n");
        return -4;
    }

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
    // test the semaphore
    // requires that IntDispatchLib_MutexEIP201 is initialized
    // (is tested by IntDispatchLib_NotInitialized)
    {
        int res = IntDispatchLib_MutexEIP201_TryLock();
        if (res != 0)
        {
            LOG_WARN(
                "IntDispatch_Initialize: "
                "EIP-201 mutex is broken (%d)\n",
                res);

            return -5;
        }

        IntDispatchLib_MutexEIP201_Unlock();
    }
#endif /* INTDISPATCH_EIP201_NOT_REENTRANT */

    // immediately get the worker thread handler function invoked
    WorkerThread_Signal(IntDispatchLib_WorkerThreadRef);

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * IntDispatch_Shutdown
 */
int
IntDispatch_Shutdown(void)
{
    // we leave the thread and semaphore alive for now
    // no actions required for static resource usage (Device API)
    return 0;
}


/*----------------------------------------------------------------------------
 * IntDispatch_Hook
 */
int
IntDispatch_Hook(
        const char * const szIntName_p,
        IntDispatch_Callback_t CBFunc,
        void * Arg_p,
        IntDispatch_Handle_t * const Handle_p)
{
    int i;

    if (szIntName_p == NULL ||
        CBFunc == NULL ||
        Handle_p == NULL)
    {
        return -1;
    }

    // ensure we are initialized
    if (IntDispatchLib_NotInitialized())
        return -99;

    // initialize the output parameters
    *Handle_p = 0;

    for (i = 0; i < IntDispatchLib_IntCount; i++)
    {
        if (strcmp(IntDispatchLib_InterruptInfo[i].szIntName_p, szIntName_p) == 0)
        {
            // found a match!
            IntDispatchLib_HookAdmin_t * const p = IntDispatchLib_HookAdmin + i;
            const IntDispatchLib_InterruptInfo_t * const Info_p = IntDispatchLib_InterruptInfo + i;

            if (p->fIsHooked)
            {
                LOG_WARN(
                    "IntDispatch_Hook: "
                    "Interrupt %s is already hooked!\n",
                    szIntName_p);

                return -2;
            }

            LOG_INFO(
                "IntDispatch_Hook: "
                "Hooking interrupt %s\n",
                szIntName_p);

            // store the configuration
            p->Arg_p = Arg_p;
            p->CBFunc_p = CBFunc;
            p->fIsHooked = true;

            // leave the interrupt masked for now
            // but reset the edge history
            (void)EIP201_Acknowledge(
                            IntDispatchLib_Devices[Info_p->AIC_Nr],
                            Info_p->Sources);

            // fill in the output parameter
            *Handle_p = IntDispatchLib_AdminIndex2Handle(i);

            return 0;   // 0 = success
        }
    } // for

    // not found
    LOG_WARN(
        "IntDispatch_Hook: "
        "Attempt was made to hook interrupt named %s\n",
        szIntName_p);

    return -3;
}


/*----------------------------------------------------------------------------
 * IntDispatch_Unhook
 */
int
IntDispatch_Unhook(
        const IntDispatch_Handle_t Handle)
{
    const int AdminIndex = IntDispatchLib_Handle2AdminIndex(Handle);

    if (AdminIndex < 0)
        return -1;

    LOG_INFO(
        "IntDispatch_Unhook: "
        "Unhooking interrupt %s\n",
        IntDispatchLib_InterruptInfo[AdminIndex].szIntName_p);

    {
        IntDispatchLib_HookAdmin_t * const p = IntDispatchLib_HookAdmin + AdminIndex;

        if (p->fIsHooked == false)
            return -2;

        // disable the interrupt source(s)
        if (IntDispatch_Mask(Handle) < 0)
            return -3;

        // remove the callback details
        p->Arg_p = NULL;
        p->CBFunc_p = NULL;

        p->fIsHooked = false;
    }

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * IntDispatch_Mask
 */
int
IntDispatch_Mask(
        const IntDispatch_Handle_t Handle)
{
    const int AdminIndex = IntDispatchLib_Handle2AdminIndex(Handle);
    const IntDispatchLib_InterruptInfo_t * Info_p;

    if (AdminIndex < 0)
        return -1;

    Info_p = IntDispatchLib_InterruptInfo + AdminIndex;

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
    {
        int res;

        res = IntDispatchLib_MutexEIP201_TryLock();

        if (res != 0)
        {
            LOG_WARN(
                "IntDispatch_Mask: "
                "Failed to acquire EIP-201 mutex (%d)\n",
                res);

            return -2;
        }
    }
#endif /* INTDISPATCH_EIP201_NOT_REENTRANT */

    {
        EIP201_Status_t res201;

        // disable the interrupt source(s)
        res201 = EIP201_SourceMask_DisableSource(
                            IntDispatchLib_Devices[Info_p->AIC_Nr],
                            Info_p->Sources);

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
        IntDispatchLib_MutexEIP201_Unlock();
#endif

        if (res201 != EIP201_STATUS_SUCCESS)
        {
            LOG_INFO(
                "IntDispatch_Mask: "
                "EIP201_SourceMask_DisableSource returned %d\n",
                res201);
        }
    }

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * IntDispatch_Unmask
 */
int
IntDispatch_Unmask(
        const IntDispatch_Handle_t Handle)
{
    const int AdminIndex = IntDispatchLib_Handle2AdminIndex(Handle);
    const IntDispatchLib_InterruptInfo_t * Info_p;

    if (AdminIndex < 0)
        return -1;

    Info_p = IntDispatchLib_InterruptInfo + AdminIndex;

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
    {
        int res;

        res = IntDispatchLib_MutexEIP201_TryLock();
        if (res != 0)
        {
            LOG_WARN(
                "IntDispatch_Unmask: "
                "Failed to acquire EIP-201 mutex (%d)\n",
                res);

            return -2;
        }
    }
#endif /* INTDISPATCH_EIP201_NOT_REENTRANT */

    {
        EIP201_Status_t res201;

        // enable the interrupt source(s)
        res201 = EIP201_SourceMask_EnableSource(
                            IntDispatchLib_Devices[Info_p->AIC_Nr],
                            Info_p->Sources);

#ifdef INTDISPATCH_EIP201_NOT_REENTRANT
        IntDispatchLib_MutexEIP201_Unlock();
#endif

        if (res201 != EIP201_STATUS_SUCCESS)
        {
            LOG_INFO(
                "IntDispatch_Unmask: "
                "EIP201_SourceMask_EnableSource returned %d\n",
                res201);
        }
    }

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * IntDispatch_EdgeReset
 */
int
IntDispatch_EdgeReset(
        const IntDispatch_Handle_t Handle)
{
    EIP201_Status_t res201;

    const int AdminIndex = IntDispatchLib_Handle2AdminIndex(Handle);
    const IntDispatchLib_InterruptInfo_t * Info_p;

    if (AdminIndex < 0)
        return -1;

    Info_p = IntDispatchLib_InterruptInfo + AdminIndex;

    // acknowledge these interrupts to reset the edge-detection history
    res201 = EIP201_Acknowledge(
                            IntDispatchLib_Devices[Info_p->AIC_Nr],
                            Info_p->Sources);

    if (res201 != EIP201_STATUS_SUCCESS)
    {
        LOG_INFO(
            "IntDispatch_EdgeReset: "
            "EIP201_Acknowledge (%u) returned %d\n",
            Info_p->Sources,
            res201);

        return -2;
    }

    return 0;       // 0 = success
}


/* end of file intdispatch_umdevxs.c */
