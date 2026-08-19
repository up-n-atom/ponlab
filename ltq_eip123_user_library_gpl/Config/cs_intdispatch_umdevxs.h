/* cs_intdispatch_umdevxs.h
 *
 * Configuration Settings for the Interrupt Dispatcher for the SafeZone FPGA.
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

// logging level for Interrupt Dispatcher
// Choose from LOG_SEVERITY_INFO, LOG_SEVERITY_WARN, LOG_SEVERITY_CRIT
//#define INTDISPATCH_LOG_SEVERITY  LOG_SEVERITY_INFO
#define INTDISPATCH_LOG_SEVERITY  LOG_SEVERITY_WARN
//#define INTDISPATCH_LOG_SEVERITY  LOG_SEVERITY_CRIT

// device to request from Driver Framework (Device)
// second interrupt controller is optional - undefine to deactivate
#define INTDISPATCH_DEVICE_EIP201_0 "EIP123_AIC"
#define INTDISPATCH_DEVICE_EIP201_1 "EIP150_AIC"

// definition of interrupt resources for the first AIC (EIP201_0)
// the interrupt sources is a bitmask of interrupt sources
// Refer to the data sheet of device for the correct values
// Config: Choose from RISING_EDGE, FALLING_EDGE, ACTIVE_HIGH, ACTIVE_LOW
//                            Name                         Inputs,                         Config
#define INTDISPATCH_RESOURCES_0 \
    INTDISPATCH_RESOURCE_ADD("EIP123_MAILBOXES_OUT-FULL",  BIT_1 + BIT_3 + BIT_5 + BIT_7,  RISING_EDGE)

// definition of interrupt resources for the optional second AIC (EIP201_1)
// the interrupt sources is a bitmask of interrupt sources
// Refer to the data sheet of device for the correct values
// Config: Choose from RISING_EDGE, FALLING_EDGE, ACTIVE_HIGH, ACTIVE_LOW
//                            Name                         Inputs,                         Config
#define INTDISPATCH_RESOURCES_1 \
    INTDISPATCH_RESOURCE_ADD("EIP28_READY",                BIT_1,                          RISING_EDGE)

// select which interrupts to trace
// comment-out or set to zero to disable tracing
//#define INTDISPATCH_TRACE_FILTER_0 0xFFFFFFFF
//#define INTDISPATCH_TRACE_FILTER_1 0xFFFFFFFF

/* end of file cs_intdispatch_umdevxs.h */
