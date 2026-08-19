/* umdevxs_device.c
 *
 * Device API for the Linux UMDevXS driver.
 *
 * It allows searching for a named device resource, which returns a handle.
 * The handle can be used to map the device memory into the application
 * memory map (in user mode).
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

#include "c_umdevxs.h"              // config options

#ifndef UMDEVXS_REMOVE_DEVICE

#include "umdevxs_internal.h"

#include <linux/string.h>           // strcmp
#include <linux/mm.h>               // remap_pfn_range

#include "basic_defs.h"             // IDENTIFIER_NOT_USED

// Device administration structure
typedef struct
{
    // name string used in Device_Find
    const char * DeviceName_p;

    // device offset range memory map
    unsigned int StartByteOffset;
    unsigned int LastByteOffset;

    int Bar;        // only for PCI devices
    int Flags;      // 0 = Normal, 1 = PCI, 2 = Simulation, 3 = Open Firmware
} UMDevXS_DeviceDef_t;

// macro used in cs_umdevxs.h
#define UMDEVXS_DEVICE_ADD(_name, _start, _last) { _name, _start, _last, 0, 0 }

#ifndef UMDEVXS_REMOVE_PCI
#define UMDEVXS_DEVICE_ADD_PCI(_name, _bar, _start, _size) \
            { _name, _start, (_start + _size -1), _bar, 1/*=PCI*/ }
#else
/* empty due to comma! */
#define UMDEVXS_DEVICE_ADD_PCI(_name, _bar, _start, _size) { 0 }
#endif /* UMDEVXS_REMOVE_PCI */

#ifndef UMDEVXS_REMOVE_SIMULATION
#define UMDEVXS_DEVICE_ADD_SIM(_name, _size) { _name, 0, (_size-1), 0, 2/*=Simulation*/ }
#else
/* empty due to comma! */
#define UMDEVXS_DEVICE_ADD_SIM(_name, _size) { 0 }
#endif /* UMDEVXS_REMOVE_SIMULATION */

#ifndef UMDEVXS_REMOVE_DEVICE_OF
#define UMDEVXS_DEVICE_ADD_OF(_name, _start, _last) \
                                { _name, _start, _last, 0, 3 }
#else
#define UMDEVXS_DEVICE_ADD_OF(_name, _start, _last) { 0 }
#endif


static const UMDevXS_DeviceDef_t UMDevXS_Devices[] =
{
#ifndef UMDEVXS_REMOVE_PCI
    // support for up to 4 memory windows
    UMDEVXS_DEVICE_ADD_PCI(
        "PCI.0", 0,
        UMDEVXS_PCI_BAR0_SUBSET_START,
        UMDEVXS_PCI_BAR0_SUBSET_SIZE),
    UMDEVXS_DEVICE_ADD_PCI(
        "PCI.1", 1,
        UMDEVXS_PCI_BAR1_SUBSET_START,
        UMDEVXS_PCI_BAR1_SUBSET_SIZE),
    UMDEVXS_DEVICE_ADD_PCI(
        "PCI.2", 2,
        UMDEVXS_PCI_BAR2_SUBSET_START,
        UMDEVXS_PCI_BAR2_SUBSET_SIZE),
    UMDEVXS_DEVICE_ADD_PCI(
        "PCI.3", 3,
        UMDEVXS_PCI_BAR3_SUBSET_START,
        UMDEVXS_PCI_BAR3_SUBSET_SIZE),
#endif /* UMDEVXS_REMOVE_PCI */
    UMDEVXS_DEVICES
};


// number of devices supported calculated on HWPAL_DEVICES defined
// in cs_linux_pci_x86.h
#define UMDEVXS_DEVICE_COUNT \
        (sizeof(UMDevXS_Devices) \
         / sizeof(UMDevXS_DeviceDef_t))


/*----------------------------------------------------------------------------
 * UMDevXSLib_Device_IsValidNameField
 *
 * Check that the Name field is zero-terminated within the required limit and
 * only contains ascii characters.
 *
 * Returns true when the name is valid.
 */
static bool
UMDevXSLib_Device_IsValidNameField(
        const char * Name_p)
{
    int i;

    for (i = 0; i <= UMDEVXS_CMDRSP_MAXLEN_NAME; i++)
    {
        const uint8_t c = (uint8_t)Name_p[i];

        // check for zero-termination
        if (c == 0)
            return true;        // ## RETURN ##

        // check for invalid characters
        if (c < 32 || c > 127)
            return false;
    } // for

    // did not find zero-terminator
    return false;
}


/*----------------------------------------------------------------------------
 * UMDevXS_Device_HandleCmd_Find
 */
void
UMDevXS_Device_HandleCmd_Find(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    int i;

    // sanity-check the name
    // it must be zero-terminated
    if (!UMDevXSLib_Device_IsValidNameField(CmdRsp_p->szName))
    {
        CmdRsp_p->Error = 1;
        return;
    }

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_Device_HandleCmd_Find: "
        "Name=%s\n",
        CmdRsp_p->szName);

    for (i = 0; i < UMDEVXS_DEVICE_COUNT; i++)
    {
        const UMDevXS_DeviceDef_t * const p = UMDevXS_Devices + i;

        // protect again potential empty records
        // caused by incomplete initializers
        if (p->DeviceName_p == NULL)
            continue;

        if (strcmp(CmdRsp_p->szName, p->DeviceName_p) == 0)
        {
            // found it!

            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_Device_HandleCmd_Find: "
                "Match on device %d [flag=%d]\n",
                i, p->Flags);

            // fill in the result fields
            CmdRsp_p->Handle = UMDevXS_Handle_Make(
                                        UMDEVXS_HANDLECLASS_DEVICE,
                                        i);

            CmdRsp_p->uint1 = p->LastByteOffset - p->StartByteOffset + 1;

            // forward PCI devices requests
            if (p->Flags == 1)
            {
#ifndef UMDEVXS_REMOVE_PCI
                UMDevXS_PCIDev_HandleCmd_Find(
                            CmdRsp_p,
                            p->Bar,
                            p->StartByteOffset);
#else
                CmdRsp_p->Handle = 0;
                CmdRsp_p->uint1 = 0;
                CmdRsp_p->Error = 6;
#endif /* UMDEVXS_REMOVE_PCI */
            }

            if (p->Flags == 2)
            {
#ifndef UMDEVXS_REMOVE_SIMULATION
                UMDevXS_SimDev_HandleCmd_Find(
                            CmdRsp_p,
                            (void *)p,
                            p->LastByteOffset);
#else
                CmdRsp_p->Handle = 0;
                CmdRsp_p->uint1 = 0;
                CmdRsp_p->Error = 6;
#endif /* UMDEVXS_REMOVE_SIMULATION */
            }

            return; // ## RETURN ##
        }
    } // for

    // not found
    LOG_WARN(
        UMDEVXS_LOG_PREFIX
        "Failed to locate device with name '%s'\n",
        CmdRsp_p->szName);

    CmdRsp_p->Error = 1;
}


/*----------------------------------------------------------------------------
 * UMDevXS_Device_HandleCmd_Enum
 */
void
UMDevXS_Device_HandleCmd_Enum(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    unsigned int DeviceNr = CmdRsp_p->uint1;

    if (DeviceNr >= UMDEVXS_DEVICE_COUNT)
    {
        // unsupported device number
        CmdRsp_p->Error = 1;
        return;
    }

    {
        const UMDevXS_DeviceDef_t * const p = UMDevXS_Devices + DeviceNr;

        if (p->DeviceName_p == NULL)
        {
            // accidentally empty entry
            // return empty name, but no error
            CmdRsp_p->szName[0] = 0;
        }
        else
        {
            memcpy(
                CmdRsp_p->szName,
                p->DeviceName_p,
                UMDEVXS_CMDRSP_MAXLEN_NAME);
        }
    }
}


/*----------------------------------------------------------------------------
 * UMDevXS_Device_Map
 */
int
UMDevXS_Device_Map(
        unsigned int DeviceNr,
        unsigned int Length,
        struct vm_area_struct * vma_p)
{
    const UMDevXS_DeviceDef_t * p;
    unsigned long address = 0;
    int res;

    if (DeviceNr < 0 || DeviceNr >= UMDEVXS_DEVICE_COUNT)
        return -1;

    p = UMDevXS_Devices + DeviceNr;

    // for mapping requests for PCI resources
    if (p->Flags == 1)
    {
#ifndef UMDEVXS_REMOVE_PCI
        int res;

        res = UMDevXS_PCIDev_Map(
                    p->Bar,
                    p->StartByteOffset,
                    p->LastByteOffset - p->StartByteOffset + 1,
                    Length,
                    vma_p);

        if (res != 0)
        {
            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_Device_Map: "
                "UMDevXS_PCIDev_Map returned %d\n",
                res);
        }

        return res;
#else
        return -2;
#endif /* UMDEVXS_REMOVE_PCI */
    }

    if (p->Flags == 2)
    {
#ifndef UMDEVXS_REMOVE_SIMULATION
        int res;

        res = UMDevXS_SimDev_Map((void *)p, &address);
        if (res != 0)
        {
            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_Device_Map: "
                "UMDevXS_SimDev_Map returned %d\n",
                res);

            return res;
        }

        // address is filled in; mapping is done below
#else
        return -2;
#endif /* UMDEVXS_REMOVE_SIMULATION */
    }

    if (p->Flags == 3)
    {
#ifndef UMDEVXS_REMOVE_DEVICE_OF
        int res;

        res = UMDevXS_OFDev_Map(
                            p->StartByteOffset,
                            p->LastByteOffset - p->StartByteOffset + 1,
                            Length,
                            vma_p);

        if (res != 0)
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_Device_Map: "
                "UMDevXS_OFDev_Map returned %d\n",
                res);
        }

        return res;
#else
        return -2;
#endif /* UMDEVXS_REMOVE_DEVICE_OF */
    }
    else
    {
        // normal case (not PCI, Simulation device or OF)
        address = p->StartByteOffset;

        // honor (or reject) length request
        if (Length > p->LastByteOffset - p->StartByteOffset + 1)
        {
            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_Device_Map: "
                "length rejected (%u)\n",
                Length);

            return -1;
        }
    }

    // avoid caching and buffering
    vma_p->vm_page_prot = pgprot_noncached(vma_p->vm_page_prot);
    vma_p->vm_flags |= VM_IO;

    // map the range into application space
    res = remap_pfn_range(
                    vma_p,
                    vma_p->vm_start,
                    address >> PAGE_SHIFT,
                    Length,
                    vma_p->vm_page_prot);

    if (res < 0)
    {
        LOG_INFO(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_Device_Map: "
            "remap_pfn_range failed (%d)\n",
            res);

        return res;
    }

    // return success
    return 0;
}

#endif /* UMDEVXS_REMOVE_DEVICE */

/* end of file umdevxs_device.c */
