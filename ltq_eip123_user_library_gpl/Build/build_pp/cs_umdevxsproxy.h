/* cs_umdevxsproxy.h
 *
 * Configuration Switches for the UMDevXS Proxy Library.
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

#define UMDEVXSPROXY_LOG_PREFIX "UMDevXSProxy: "

#define UMDEVXSPROXY_NODE_NAME "//dev//umdevxs_c"

// uncomment to remove selected functionality
//#define UMDEVXSPROXY_REMOVE_DEVICE
//#define UMDEVXSPROXY_REMOVE_SMBUF
//#define UMDEVXSPROXY_REMOVE_INTERRUPT
#define UMDEVXSPROXY_REMOVE_PCICFG

/* end of file cs_umdevxsproxy.h */
