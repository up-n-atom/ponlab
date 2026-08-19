/* c_intdispatch_umdevxs.h
 *
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

// get the product configuration settings
#include "cs_intdispatch_umdevxs.h"


// provide backup values
#ifndef INTDISPATCH_LOG_SEVERITY
#define INTDISPATCH_LOG_SEVERITY LOG_SEVERITY_INFO
#endif

#ifndef INTDISPATCH_DEVICE_EIP201_0
#error "Missing configuration option: INTDISPATCH_DEVICE_EIP201_0"
#endif

#ifndef INTDISPATCH_RESOURCES_0
#error "Missing configuration option: INTDISPATCH_RESOURCES_0"
#endif

#ifndef INTDISPATCH_TRACE_FILTER_0
#define INTDISPATCH_TRACE_FILTER_0 0
#endif


#ifdef INTDISPATCH_DEVICE_EIP201_1

// second interrupt controller support is enabled

#ifndef INTDISPATCH_RESOURCES_1
#error "Missing configuration option: INTDISPATCH_RESOURCES_1"
#endif

#ifndef INTDISPATCH_TRACE_FILTER_1
#define INTDISPATCH_TRACE_FILTER_1 0
#endif

#else

// second interrupt controller support is disabled

#undef INTDISPATCH_RESOURCES_1
#undef INTDISPATCH_TRACE_FILTER_1

#endif /* !INTDISPATCH_DEVICE_EIP201_1 */


// sanity checks
// (none)

/* end of file c_intdispatch_umdevxs.h */
