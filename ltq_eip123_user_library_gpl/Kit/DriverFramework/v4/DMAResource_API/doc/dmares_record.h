/* dmares_record.h
 *
 * Driver Framework, DMAResource Record Definition (example)
 *
 * This file is an customizable declaration of the DMAResource_Record_t type
 * as part DMA Resource API.
 * Do not include this file directly; include dmares_types.h instead.
 *
 * The document "Driver Framework Porting Guide" contains the detailed
 * specification of this API. The information contained in this header file
 * is for reference only.
 */

/*****************************************************************************
* Copyright (c) 2008-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_DMARES_TYPES_H
#error "Please include dmares_types.h instead of dmares_record.h"
#endif

/*----------------------------------------------------------------------------
 * Address Domains
 *
 * This is a list of domains that are supported by the implementation.
 * The domain describes the applicability of the address.
 */
enum
{
    DMARES_DOMAIN_DEVICE_CM,
    DMARES_DOMAIN_DEVICE_PE,
    DMARES_DOMAIN_DEVICE_PKA,
    DMARES_DOMAIN_PHYSICAL,
    DMARES_DOMAIN_BUS,
    DMARES_DOMAIN_INTERHOST,
    DMARES_DOMAIN_USER,
    DMARES_DOMAIN_KERNEL,
    DMARES_DOMAIN_DRIVER,
    DMARES_DOMAIN_APPLICATION,
    DMARES_DOMAIN_UNKNOWN
};

#define DMARES_ADDRPAIRS_CAPACITY 4

typedef struct
{
    uint32_t Magic;     // signature used to validate handles

    struct
    {
        // for freeing the buffer
        void * AllocatedAddr_p;
        unsigned int AllocatedSize;     // in bytes

        char AllocatorRef;
    } alloc;

    DMAResource_Properties_t Props;

    // list of applicable addresses for this resource
    DMAResource_AddrPair_t AddPairs[DMARES_ADDRPAIRS_CAPACITY];

    struct
    {
        // aligned start-address, data starts here
        void * HostAddr_p;
    } host;

    struct
    {
        // used by Read32/Write32[Array]
        bool fSwapEndianess;
    } swap;

} DMAResource_Record_t;


/* end of file dmares_record.h */
