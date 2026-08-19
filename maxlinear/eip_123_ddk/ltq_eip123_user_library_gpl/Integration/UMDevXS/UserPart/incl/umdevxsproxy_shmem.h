/* umdevxsproxy_shmem.h
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

#ifndef INCLUDE_GUARD_UMDEVXSPROXY_SHMEM_H
#define INCLUDE_GUARD_UMDEVXSPROXY_SHMEM_H

/*----------------------------------------------------------------------------
 * Types used by SHMem API
 */
typedef struct
{
    void * p;
} UMDevXSProxy_SHMem_Handle_t;


typedef struct
{
    void * p;
} UMDevXSProxy_SHMem_BufPtr_t;

typedef struct
{
    void * p;
} UMDevXSProxy_SHMem_DevAddr_t;


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Alloc
 *
 * This routine allocates a block of memory that can be shared with another
 * host. The memory is allocated in kernel space. A handle is returned that
 * can be passed to the SMBUF_MAP service to map the memory in the user's
 * address space.
 * Also, a device (aka physical or bus) address is returned that can be used
 * by DMA devices.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to ...
 */
int
UMDevXSProxy_SHMem_Alloc(
        const unsigned int Size,
        const unsigned int Bank,
        const unsigned int Alignment,
        UMDevXSProxy_SHMem_Handle_t * const Handle_p,
        UMDevXSProxy_SHMem_BufPtr_t * const BufPtr_p,
        UMDevXSProxy_SHMem_DevAddr_t * const DevAddr_p,
        unsigned int * const ActualSize_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Register
 *
 * This routine registers a block of memory in order to obtain a handle
 * to be used in subsequent Pre/PostDMA calls.
 * The passed-in handle must refer to an already allocated/attached block
 * that encompasses the registered block.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to ...
 */
int
UMDevXSProxy_SHMem_Register(
        const unsigned int Size,
        const UMDevXSProxy_SHMem_BufPtr_t BufPtr,
        const UMDevXSProxy_SHMem_Handle_t Handle,
        UMDevXSProxy_SHMem_Handle_t * const RetHandle_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Free
 *
 * This function frees a memory block previously allocated using
 * UMDevXSProxy_SHMem_Alloc. The memory block is removed from the caller's
 * memory map and then freed. The caller must make sure the buffer is not
 * accessed anymore (also by the remote host!) before calling this function.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to ...
 */
int
UMDevXSProxy_SHMem_Free(
        const UMDevXSProxy_SHMem_Handle_t Handle);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Attach
 *
 * This function registers a block of memory, typically allocated by another
 * host to exchange data through shared memory.
 * A handle is returned that can be passed to the SHMEM_MAP service to map
 * the memory into the user's address space.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to ...
 */
int
UMDevXSProxy_SHMem_Attach(
        const UMDevXSProxy_SHMem_DevAddr_t DevAddr,
        const unsigned int Size,
        const unsigned int Bank,
        UMDevXSProxy_SHMem_Handle_t * const Handle_p,
        UMDevXSProxy_SHMem_BufPtr_t * const BufPtr_p,
        unsigned int * const Size_p);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Detach
 *
 * This function detaches from a block of shared memory.
 * The memory block is removed from the caller's memory map.
 *
 * Return Value
 *     0  Success
 *    -1  Failed to ...
 */
int
UMDevXSProxy_SHMem_Detach(
        const UMDevXSProxy_SHMem_Handle_t Handle);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Commit
 *
 * This function ensures the [subset of the] buffer is committed from a cache
 * (if any) to system memory, to ensure the other host can read it.
 */
void
UMDevXSProxy_SHMem_Commit(
        const UMDevXSProxy_SHMem_Handle_t Handle,
        const unsigned int SubsetStart,
        const unsigned int SubsetLength);


/*----------------------------------------------------------------------------
 * UMDevXSProxy_SHMem_Refresh
 *
 * This function ensures the [subset of the] buffer is refreshed from system
 * memory into the cache (if any), to ensure we are not looking at old data
 * that has been replaced by the other host.
 */
void
UMDevXSProxy_SHMem_Refresh(
        const UMDevXSProxy_SHMem_Handle_t Handle,
        const unsigned int SubsetStart,
        const unsigned int SubsetLength);


#endif /* INCLUDE_GUARD_UMDEVXSPROXY_SHMEM_H */

/* umdevxsproxy_shmem.h */
