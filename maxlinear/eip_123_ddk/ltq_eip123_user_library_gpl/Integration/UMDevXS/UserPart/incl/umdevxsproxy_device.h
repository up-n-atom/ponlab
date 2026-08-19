/* umdevxsproxy_device.h
 *
 * This user-mode library handles the communication with the
 * Linux User Mode Device Access driver. Using the API it is possible to
 * access memory mapped devices and to enable access to foreign-allocated
 * shared memory buffers.
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

#ifndef INCLUDE_GUARD_UMDEVXSPROXY_DEVICE_H
#define INCLUDE_GUARD_UMDEVXSPROXY_DEVICE_H


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_Find
 *
 * This function must be used to locate a named device resource. The returned
 * DeviceID can be used to map the device into user space memory using
 * UMDevXSProxy_Device_Map.
 *
 * Name_p (input)
 *     Pointer to the name of the device resource.
 *     A list of supported device resources is implementation specific.
 *
 * DeviceID_p (output)
 *     Returns the DeviceID that can be used in other API functions.
 *
 * DeviceMemorySize_p (output)
 *     Returns the size of the device memory window. This is the maximum size
 *     of the region that can be mapped into user space.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to locate device with this name
 */
int
UMDevXSProxy_Device_Find(
        const char * Name_p,
        int * const DeviceID_p,
        unsigned int * const DeviceMemorySize_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_Enum
 *
 * This function can be used to get a list of device names supported by the
 * implementation.
 *
 * DeviceNr
 *     Device number to enumerate. Start at 0 and stop upon error.
 *
 * NameSize
 *     Size of the memory region pointer to by Name_p.
 *
 * Name_p (output)
 *     Pointer to the memory range where the device name will be copied to.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to execute the request, or invalid DeviceNr
 */
int
UMDevXSProxy_Device_Enum(
        const unsigned int DeviceNr,
        const unsigned int NameSize,
        char * Name_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_Map
 *
 * DeviceID
 *    Device identifier as returned by UMDevXSProxy_Device_Find.
 *
 * DeviceMemorySize
 *     Size of the memory block to map in (device-dependent).
 *
 * Return Value
 *     NULL   Failed to map this memory region (might be non-existing).
 *     Other  Pointer application can use to access this region.
 */
void *
UMDevXSProxy_Device_Map(
        const int DeviceID,
        const unsigned int DeviceMemorySize);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_Device_Unmap
 *
 * DeviceID
 *    Device identifier as returned by UMDevXSProxy_Device_Find.
 *
 * DeviceMemory_p
 *    Pointer as returned by UMDevXSProxy_Device_Map.
 *
 * DeviceMemorySize
 *    Size of the device memory block as used in the UMDevXSProxy_Device_Map
 *    call.
 *
 * Return Value
 *     0   Success
 *    -1   Error code
 */
int
UMDevXSProxy_Device_Unmap(
        const int DeviceID,
        void * DeviceMemory_p,
        const unsigned int DeviceMemorySize);


#endif /* INCLUDE_GUARD_UMDEVXSPROXY_DEVICE_H */

/* umdevxsproxy_device.h */
