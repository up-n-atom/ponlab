/* umdevxsproxy_device_pcicfg.h
 *
 * UMDevXS Proxy interface for reading and writing the PCI Configuration Space.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef UMDEVXSPROXY_DEVICE_PCICFG_H_
#define UMDEVXSPROXY_DEVICE_PCICFG_H_

/*----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */

#include "basic_defs.h"



/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_PciCfg_Read32
 *
 * This function can be used to read a 32-bit integer at the specified
 * byte offset from the PCI Configuration Space
 *
 * ByteOffset
 *     Byte offset to read at.
 *
 * Int32_p (output)
 *     Pointer to memory location where the 32-bit integer will be stored.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to execute the request
 */
int
UMDevXSProxy_Device_PciCfg_Read32(
        const unsigned int ByteOffset,
        uint32_t * const Int32_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_PciCfg_Write32
 *
 * This function can be used to write a 32-bit integer at the specified
 * byte offset from the PCI Configuration Space
 *
 * ByteOffset
 *     Byte offset to write at.
 *
 * Int32
 *     32-bit integer value to write.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to execute the request
 */
int
UMDevXSProxy_Device_PciCfg_Write32(
        const unsigned int ByteOffset,
        const uint32_t Int32);


#endif /* UMDEVXSPROXY_DEVICE_PCICFG_H_ */

/* end of file umdevxsproxy_device_pcicfg.h */
