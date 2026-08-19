/* c_umdevxsproxy.h
 *
 * Configuration options for UMDevXS Proxy Library.
 *
 * This file includes cs_umdevxsproxy.h (from the product-level) and then
 * provides defaults for missing configuration switches.
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

#ifndef INCLUDE_GUARD_C_UMDEVXSPROXY_H
#define INCLUDE_GUARD_C_UMDEVXSPROXY_H

// get the product-level configuration
#include "cs_umdevxsproxy.h"

#ifndef UMDEVXSPROXY_LOG_PREFIX
#define UMDEVXSPROXY_LOG_PREFIX "UMDevXSProxy: "
#endif

#ifndef UMDEVXSPROXY_NODENAME
#define UMDEVXSPROXY_NODENAME "//dev//umpci_c"
#endif

#endif /* INCLUDE_GUARD_C_UMDEVXSPROXY_H */

/* end of file c_umdevxsproxy.h */
