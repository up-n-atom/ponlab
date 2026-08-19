/* umdevxs_pcidev.c
 *
 * PCI device support for the Linux UMDevXS driver.
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

#include "c_umdevxs.h"          // config options

#ifndef UMDEVXS_REMOVE_PCI

#include "umdevxs_internal.h"

#include <linux/module.h>       // MODULE_DEVICE_TABLE
#include <linux/pci.h>          // pci_*
#include <linux/mm.h>           // remap_pfn_range

#include "basic_defs.h"         // uint8_t, MASK_xx_BITS, etc.

// PCI device related
static char UMDevXS_PCIDev_module_name[] = UMDEVXS_MODULENAME;
static struct pci_dev * UMDevXS_PCIDev_PCIDevice_p = NULL;


/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_Map
 *
 * This function is called to request a PCI memory resource to be mapped into
 * the memory space of an application. Configured start-offset and maximum
 * size and provided, together with the size requested by the application.
 * These parameters can select a subset of the PCI memory resource, but must
 * always provide a resource as large as requested by the application.
 */
int
UMDevXS_PCIDev_Map(
        unsigned int BAR,
        unsigned int SubsetStart,       // defined
        unsigned int SubsetSize,        // defined
        unsigned int Length,            // requested
        struct vm_area_struct * vma_p)
{
    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_PCIDev_Map: "
        "BAR=%u, Subset=%u,%u, Length=%u\n",
        BAR,
        SubsetStart,
        SubsetSize,
        Length);

    // honour application limiter
    if (SubsetSize > Length)
        SubsetSize = Length;

    // was the PCI side of the driver enabled by the OS
    // this only happens when a compatible PCI device is inserted
    if (UMDevXS_PCIDev_PCIDevice_p == NULL)
        return -1;

    // is the BAR valid?
    if (BAR >= DEVICE_COUNT_RESOURCE)
        return -2;

    // is this BAR a memory mapped resource?
    if ((pci_resource_flags(UMDevXS_PCIDev_PCIDevice_p, BAR) & IORESOURCE_MEM) == 0)
        return -3;

    // limit size
    {
        unsigned int Len;

        Len = (unsigned int)pci_resource_len(
                                 UMDevXS_PCIDev_PCIDevice_p,
                                 BAR);

#ifndef UMDEVXS_REMOVE_SMALL_PCIWINDOW_SUPPORT
        // special handling of Versatile Board with 1MB memory windows
        if ((SubsetStart > Len) && (Len == 1 * 1024 * 1024))
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "Redirecting map for BAR=%u, SubsetStart=0x%08x,"
                " Len=0x%x, to StartOffset=0\n",
                BAR,
                SubsetStart,
                Len);

            SubsetStart = 0;
        }
#endif /* UMDEVXS_REMOVE_SMALL_PCIWINDOW_SUPPORT */

        // start beyond end of resource?
        if (SubsetStart > Len)
            return -4;

        // limit inside resource
        if (SubsetStart + SubsetSize > Len)
            SubsetSize = Len - SubsetStart;
    }

    // now map the region into the application memory space
    {
        long StartOfs;
        int ret;

        StartOfs = (unsigned int)pci_resource_start(UMDevXS_PCIDev_PCIDevice_p, BAR);
        StartOfs += SubsetStart;

        LOG_INFO(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_PCIDev_Map: "
            "Start=0x%lx, Size=%d\n",
            StartOfs,
            SubsetSize);

        // map the whole physically contiguous area in one piece
        ret = remap_pfn_range(
                    vma_p,
                    vma_p->vm_start,
                    StartOfs >> PAGE_SHIFT,
                    SubsetSize,
                    vma_p->vm_page_prot);

        if (ret < 0)
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_PCIDev_Map: "
                "remap result: %d\n",
                ret);

            return -10;
        }
    }

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_Probe
 */
static int
UMDevXS_PCIDev_Probe(
        struct pci_dev * PCI_Device_p,
        const struct pci_device_id * id)
{
    if (PCI_Device_p == NULL)
        return -ENODEV;

    // remember the device reference
    // we need when access the configuration space
    UMDevXS_PCIDev_PCIDevice_p = PCI_Device_p;

    {
        // enable the device
        // this also looks up the IRQ
        int res = pci_enable_device(PCI_Device_p);

        if (res)
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "Failed to enable PCI device %s (err: %d)\n",
                pci_name(PCI_Device_p),
                res);

            return res;
        }
#ifdef UMDEVXS_USE_MSI
        pci_set_master(PCI_Device_p);
#endif
    }

    LOG_CRIT(
        UMDEVXS_LOG_PREFIX
        "installed\n");

    IDENTIFIER_NOT_USED(id);

    // return 0 to indicate "we decided to take ownership"
    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_Remove
 */
static void
UMDevXS_PCIDev_Remove(
        struct pci_dev * PCI_Device_p)
{
    if (PCI_Device_p == NULL)
        return;

    pci_disable_device(PCI_Device_p);

    // side-effect of pci_disable_device is clearing of the Master bit
    // if the device tries DMA, it can hang
    // to prevent this, re-enable the Master capability of the device
    pci_set_master(PCI_Device_p);

    LOG_CRIT(
        UMDEVXS_LOG_PREFIX
        "removed\n");
}


/*----------------------------------------------------------------------------
 * Declarations related to the PCI driver
 */

static const struct pci_device_id UMDevXS_PCIDev_DeviceIDs[] =
{
    {PCI_DEVICE(UMDEVXS_PCI_VENDOR_ID, UMDEVXS_PCI_DEVICE_ID), },
    {0, }
};

static struct pci_driver UMDevXS_PCIDev_PCI_Driver =
{
#ifdef _MSC_VER
    // microsoft compiler does not support partial initializers
    // NOTE: struct must have fields in this order
    UMDevXS_PCIDev_module_name,
    UMDevXS_PCIDev_DeviceIDs,
    UMDevXS_PCIDev_Probe,
    UMDevXS_PCIDev_Remove,
#else
    .name = UMDevXS_PCIDev_module_name,
    .id_table = UMDevXS_PCIDev_DeviceIDs,
    .probe = UMDevXS_PCIDev_Probe,
    .remove = UMDevXS_PCIDev_Remove,
#endif
};



/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_HandleCmd_Find
 *
 * This function is called when the the Device_Find function matched a PCI
 * placeholder device. This function is called with the related BAR and must
 * check it is valid and fill in a few CmdRsp fields.
 * The BAR must be supported by this PCI device.
 * The Size and Handle must be returned.
 * CmdRsp_p->uint1 = configured size of device to map
 */
void
UMDevXS_PCIDev_HandleCmd_Find(
        UMDevXS_CmdRsp_t * const CmdRsp_p,
        unsigned int BAR,
        unsigned int SubsetStart)
{
    if (CmdRsp_p == NULL)
        return;

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_PCIDev_HandleCmd: "
        "BAR=%u\n",
        BAR);

    // was the PCI side of the driver enabled by the OS
    // this only happens when a compatible PCI device is inserted
    if (UMDevXS_PCIDev_PCIDevice_p == NULL)
    {
        CmdRsp_p->Error = 2;
        return;
    }

    // is the BAR valid?
    if (BAR >= DEVICE_COUNT_RESOURCE)
    {
        CmdRsp_p->Error = 3;
        return;
    }

    // is this BAR a memory mapped resource?
    if ((pci_resource_flags(UMDevXS_PCIDev_PCIDevice_p, BAR) & IORESOURCE_MEM) == 0)
    {
        CmdRsp_p->Error = 4;
        return;
    }

    // limit size
    {
        unsigned int Len;

        Len = (unsigned int)pci_resource_len(
                                 UMDevXS_PCIDev_PCIDevice_p,
                                 BAR);

#ifndef UMDEVXS_REMOVE_SMALL_PCIWINDOW_SUPPORT
        // special handling of Versatile Board with 1MB memory windows
        if ((SubsetStart > Len) &&
            (Len == 1 * 1024 * 1024))
        {
            unsigned int PCIAddr;

            // address bits 31:28 are remapped by Versatile board
            // address bits 27:20 are taken from PCI address (assigned by BIOS!)
            // address bits 19:0 are taken from PCI
            // thus: bits 27:20 must equal the wanted start offset

            // example: 256MB window, SMAP=3, StartOffset=0x08000000, size=2MB
            //          ==> offset 0..0x1FFFFF maps to 0x38000000..0x03801FFFF
            //          1MB window, PCI-BIOS assigned address = 0xD8000000
            //          ==> offset 0..0x1FFFFF maps to 0x38000000..0x03801FFFF

            PCIAddr = pci_resource_start(
                                 UMDevXS_PCIDev_PCIDevice_p,
                                 BAR);

            if ((PCIAddr & MASK_28_BITS) == SubsetStart)
            {
                // possible to support!

                // do not map more than requested
                if (Len > CmdRsp_p->uint1)
                    Len = CmdRsp_p->uint1;

                LOG_CRIT(
                    UMDEVXS_LOG_PREFIX
                    "Redirecting request for BAR=%u, StartOffset=0x%08x,"
                    " Len=0x%x, PCIAddr=0x%08x to StartOffset=0, Len=0x%x\n",
                    BAR,
                    SubsetStart,
                    CmdRsp_p->uint1,
                    PCIAddr,
                    Len);

                SubsetStart = 0;
                CmdRsp_p->uint1 = Len;
            }
        }
#endif /* UMDEVXS_REMOVE_SMALL_PCIWINDOW_SUPPORT */

        // start beyond end of resource?
        if (SubsetStart > Len)
        {
            CmdRsp_p->Error = 5;
            return;
        }

        // limit inside resource
        if (SubsetStart + CmdRsp_p->uint1 > Len)
            CmdRsp_p->uint1 = Len - SubsetStart;
    }
}


/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_Init
 *
 * Returns <0 on error.
 * Returns >=0 on success. The number is the interrupt number associated
 * with this PCI device.
 */
int
UMDevXS_PCIDev_Init(void)
{
    int Status;

    Status = pci_register_driver(&UMDevXS_PCIDev_PCI_Driver);
    if (Status < 0)
    {
        LOG_CRIT(
            UMDEVXS_LOG_PREFIX
            "Failed to register the PCI device (%d)\n",
            Status);

        return Status;
    }

    // UMDevXS_PCIDev_PCIDevice_p is filled in by Probe function
    // but only when invoked!
    if (UMDevXS_PCIDev_PCIDevice_p)
    {
#ifdef UMDEVXS_USE_MSI
        pci_enable_msi(UMDevXS_PCIDev_PCIDevice_p);
#endif // UMDEVXS_USE_MSI
        return UMDevXS_PCIDev_PCIDevice_p->irq;
    }

    return -1;
}


/*----------------------------------------------------------------------------
 * UMDevXS_PCIDev_UnInit
 */
void
UMDevXS_PCIDev_UnInit(void)
{
#ifdef UMDEVXS_USE_MSI
    if (UMDevXS_PCIDev_PCIDevice_p)
    {
        pci_disable_msi(UMDevXS_PCIDev_PCIDevice_p);
    }
#endif
    pci_unregister_driver(&UMDevXS_PCIDev_PCI_Driver);
}


/*----------------------------------------------------------------------------
 * UMDevXS_Device_HandleCmd_Read32
 */
#ifndef UMDEVXS_REMOVE_DEVICE_PCICFG
void
UMDevXS_PCIDev_HandleCmd_Read32(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    CmdRsp_p->Error = -1;

    if (CmdRsp_p == NULL)
        return;

    // was the PCI side of the driver enabled by the OS
    // this only happens when a compatible PCI device is inserted
    if (UMDevXS_PCIDev_PCIDevice_p == NULL)
        return;

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_PCIDev_HandleCmd_Read32: "
        "read offset (32-bit integers)=%u\n",
        CmdRsp_p->uint1);

    pci_read_config_dword(UMDevXS_PCIDev_PCIDevice_p,
                           CmdRsp_p->uint1,
                           &CmdRsp_p->uint2);

    CmdRsp_p->Error = 0; // Success
}
#endif // UMDEVXS_REMOVE_DEVICE_PCICFG


/*----------------------------------------------------------------------------
 * UMDevXS_Device_HandleCmd_Write32
 */
#ifndef UMDEVXS_REMOVE_DEVICE_PCICFG
void
UMDevXS_PCIDev_HandleCmd_Write32(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    CmdRsp_p->Error = -1;

    if (CmdRsp_p == NULL)
        return;

    // was the PCI side of the driver enabled by the OS
    // this only happens when a compatible PCI device is inserted
    if (UMDevXS_PCIDev_PCIDevice_p == NULL)
        return;

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_PCIDev_HandleCmd_Write32: "
        "write offset (32-bit integers)=%u, "
        "value=%u\n",
        CmdRsp_p->uint1,
        CmdRsp_p->uint2);

    pci_write_config_dword(UMDevXS_PCIDev_PCIDevice_p,
                           CmdRsp_p->uint1,
                           CmdRsp_p->uint2);

    CmdRsp_p->Error = 0; // Success
}
#endif // UMDEVXS_REMOVE_DEVICE_PCICFG


MODULE_DEVICE_TABLE(pci, UMDevXS_PCIDev_DeviceIDs);

#endif /* UMDEVXS_REMOVE_PCI */

/* end of file umdevxs_pcidev.c */
