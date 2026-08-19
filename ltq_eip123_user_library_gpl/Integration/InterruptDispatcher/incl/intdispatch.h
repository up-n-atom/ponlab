/* intdispatch.h
 *
 * This API enables callers to register interest in one or more interrupts.
 * The Interrupt Dispatcher SW module handles the top-level interrupt
 * controller, decodes the interrupts and dispatches these to the interested
 * parties.
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

#ifndef INCLUDE_GUARD_INTDISPATCH_H
#define INCLUDE_GUARD_INTDISPATCH_H

/*----------------------------------------------------------------------------
 * IntDispatch_Callback_t
 *
 * This is the prototype of the callback function that can be registered via
 * this API. The callback is invoked to indicate an interrupt was detected
 * and should be serviced.
 */
typedef void (* IntDispatch_Callback_t)(void * Arg_p);


/*----------------------------------------------------------------------------
 * IntDispatch_Handle_t
 *
 * This handle is used in the API to refer to a hooked interrupt. The handle
 * can be used to unhook the interrupt and to mask and unmask the interrupt.
 */
typedef int IntDispatch_Handle_t;


/*----------------------------------------------------------------------------
 * IntDispatch_Initialize
 *
 * This function must be called by every user of this API prior to using
 * any of the other functions. This allows the implementation to perform
 * first-time initialization actions, which typically means telling the OS
 * about the interrupt(s) we want to service.
 *
 * Return Value
 *      0    Success
 *     <0    Error code
 */
int
IntDispatch_Initialize(void);


/*----------------------------------------------------------------------------
 * IntDispatch_Hook
 *
 * This function registers a callback function that can service an interrupt.
 * The callback function will be invoked when the interrupt has been decoded.
 * A handle is returned that can be used to mask and unmask the interrupt (to
 * create a critical section) and to unhook the interrupt. The interrupt is
 * left masked by this function.
 *
 * szIntName_p
 *     Zero-terminated name that describes the interrupt to be hooked.
 *
 * Arg_p (input)
 *     Opaque pointer that will be passed to the callback function.
 *
 * Handle_p (output)
 *     Pointer to the memory location that will be written with the handle
 *     that must be used to mask, unmask or unhook an interrupt.
 *
 * Return Value
 *      0    Success
 *     <0    Error code. Specifically:
 *     -1    Bad parameter
 *     -2    No interupt with that name
 *     -3    Interrupt is already hooked
 */
int
IntDispatch_Hook(
        const char * const szIntName_p,
        IntDispatch_Callback_t CBFunc,
        void * Arg_p,
        IntDispatch_Handle_t * const Handle_p);


/*----------------------------------------------------------------------------
 * IntDispatch_Unhook
 *
 * This function unhooks an interrupt. When this function returns, the
 * previously registered callback function will not be invoked anymore.
 * The interrupt source will be masked in the process.
 *
 * Return Value
 *     0    Success
 *     <0   Error code, specifically:
 *     -1   Bad parameter
 */
int
IntDispatch_Unhook(
        const IntDispatch_Handle_t Handle);


/*----------------------------------------------------------------------------
 * IntDispatch_Mask
 *
 * This function masks the interrupt source related to the handle, which
 * prevents the callback function from being invoked. Interrupt masking is not
 * "stackable", so multiple calls to this function serve no purpose.
 * If an interrupt configured for edge-detection occured during the masking
 * period, this event is remembered and the callback will be invoked
 * immediately when Unmask is called.
 *
 * Return Value
 *     0    Success
 *     <0   Error code, specifically:
 *     -1   Bad parameter
 */
int
IntDispatch_Mask(
        const IntDispatch_Handle_t Handle);


/*----------------------------------------------------------------------------
 * IntDispatch_Unmask
 *
 * This function unmasks the interrupt source related to the handle, which
 * allows the callback function to be invoked again. Interrupt masking is not
 * "stackable", so the first call to Unmask enables the callback.
 * If an interrupt configured for edge-detection occured during the masking
 * period, this event was remembered and the callback will be invoked during
 * this function call, or shortly after this function returns.
 * Use IntDispatch_EdgeReset to forget such old events.
 *
 * Return Value
 *     0    Success
 *     <0   Error code, specifically:
 *     -1   Bad parameter
 */
int
IntDispatch_Unmask(
        const IntDispatch_Handle_t Handle);


/*----------------------------------------------------------------------------
 * IntDispatch_EdgeReset
 *
 * This function can be used to state of for interrupts configured for edge-
 * detection. This is also done automatically before dispatching an interrupt
 * (invoke the callback function). This function can be used to clear the
 * history built up during a masking period - see IntDispatch_Mask/Unmask.
 *
 * Return Value
 *     0    Success
 *     <0   Error code, specifically:
 *     -1   Bad parameter
 */
int
IntDispatch_EdgeReset(
        const IntDispatch_Handle_t Handle);

#endif /* Include Guard */

/* end of file intdispatch.h */
