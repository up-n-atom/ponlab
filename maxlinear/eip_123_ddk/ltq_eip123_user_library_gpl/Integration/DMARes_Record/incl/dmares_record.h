/* dmares_record.h
 *
 * DMAResource Record Definition.
 *
 * The document "Driver Framework Porting Guide" contains the detailed
 * specification of this API.
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

#include "umdevxsproxy_shmem.h" // UMDevXSProxy_SHMem_Handle_t

/*----------------------------------------------------------------------------
 * AddrTrans_Domain_t
 *
 * This is a list of domains that can be supported by the implementation. The
 * exact meaning can be different for different EIP devices and different
 * environments.
 */
enum
{
    DMARES_DOMAIN_UNKNOWN = 0,
    DMARES_DOMAIN_HOST,
    DMARES_DOMAIN_BUS,
    DMARES_DOMAIN_INTERHOST,
    DMARES_DOMAIN_EIP12xDMA,
    DMARES_DOMAIN_ALTERNATIVE
} AddrTrans_Domain_t;

// Maximum number of address/domain pairs stored per DMA resource.
#define DMARES_ADDRPAIRS_CAPACITY 3

typedef struct
{
    // signature used to validate handles
    uint32_t Magic;

    // DMA resource properties: Size, Alignment, Bank & fCached
    DMAResource_Properties_t Props;

    // Storage for upto N address/domain pairs.
    DMAResource_AddrPair_t AddrPairs[DMARES_ADDRPAIRS_CAPACITY];

    // if true, 32-bit words are swapped when transferred to/from
    // the DMA resource
    bool fSwapEndianess;

    // this implementation supports the following allocator references:
    // 'A' -> this DMA resource has been obtained through DMAResource_Alloc
    // 'R' -> this DMA resource has been obtained through DMAResource_Register
    // 'T' -> this DMA resource has been obtained through DMAResource_Attach
    char AllocatorRef;

    // kernel driver handle for this DMA resource
    UMDevXSProxy_SHMem_Handle_t DriverHandle;

} DMAResource_Record_t;

#define DMARES_RECORD_MAGIC 0xde42b5e7

/* end of file dmares_record.h */
