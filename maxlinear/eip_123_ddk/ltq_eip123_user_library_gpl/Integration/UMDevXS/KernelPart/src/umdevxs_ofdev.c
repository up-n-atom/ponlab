/* umdevxs_ofdev.c
 *
 * Open Firmware device support for the Linux UMDevXS driver.
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


/*----------------------------------------------------------------------------
 * This module implements (provides) the following interface(s):
 */

#include "umdevxs_internal.h"


/*----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */

// Default configuration
#include "c_umdevxs.h"

#ifndef UMDEVXS_REMOVE_DEVICE_OF

// Driver Framework Basic Definitions API
#include "basic_defs.h"         // uint8_t, MASK_xx_BITS, etc.

// Linux Kernel API
#include <linux/of_platform.h>      // of_*,
#include <asm/of_device.h>          // struct of_device
#include <asm/io.h>                 // ioread32, iowrite32
#include <asm/system.h>             // smp_rmb, smp_wmb
#include <linux/version.h>          // LINUX_VERSION_CODE, KERNEL_VERSION
#include <linux/module.h>           // MODULE_DEVICE_TABLE
#include <linux/mm.h>               // remap_pfn_range


/*----------------------------------------------------------------------------
 * Definitions and macros
 */

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif /* KERNEL_VERSION */

/* 2.6.35 is the highest version currently supported */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
#error "Kernel versions after 2.6.35 are not supported"
#endif /* LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35) */


/*----------------------------------------------------------------------------
 * Forward declarations
 */

static int
UMDevXS_OFDev_Probe(
        struct of_device *ofdev,
        const struct of_device_id *match);

static int
UMDevXS_OFDev_Remove(
        struct of_device *ofdev);


/*----------------------------------------------------------------------------
 * Local variables
 */

// OF device related
static char UMDevXS_OFDev_module_name[] = UMDEVXS_MODULENAME;
static struct of_device*  UMDevXS_OFDev_Device_p = NULL;

// virtual address returned by of_iomap()
static uint32_t * UMDevXS_MappedBaseAddr_p = NULL;

#ifndef UMDEVXS_REMOVE_INTERRUPT
// Device virtual IRQ number
static unsigned int UMDEVXS_OF_Virtual_IRQ = 0;
#endif // UMDEVXS_REMOVE_INTERRUPT

static struct of_device_id UMDevXS_OFDev_DeviceIDs[] =
  {
    { .compatible      = UMDEVXS_PLATFORM_DEVICE_NAME,},
    { },
  };

static struct of_platform_driver UMDevXS_OFDev_Driver =
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34)
    .driver = {
        .name = UMDevXS_OFDev_module_name,
        .owner = THIS_MODULE,
        .of_match_table = UMDevXS_OFDev_DeviceIDs,
    },
#else
    .name         = UMDevXS_OFDev_module_name,
    .match_table  = UMDevXS_OFDev_DeviceIDs,
#endif
    .probe        = UMDevXS_OFDev_Probe,
    .remove       = UMDevXS_OFDev_Remove,
};


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_Probe
 */
static int
UMDevXS_OFDev_Probe(
        struct of_device * OF_Device_p,
        const struct of_device_id * id)
{
    if (OF_Device_p == NULL)
    {
        LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_OFDev_Probe: "
                "Failed to probe the device 0x%p\n", OF_Device_p);
        return -EINVAL;
    }

    if (of_find_compatible_node(NULL,NULL,UMDEVXS_PLATFORM_DEVICE_COMPATIBLE))
    {
        LOG_INFO(
                 UMDEVXS_LOG_PREFIX
                 "UMDevXS_OFDev_Probe: found compatible device %s "
                 "for requested device %s\n",
                 UMDEVXS_PLATFORM_DEVICE_COMPATIBLE,
                 UMDEVXS_PLATFORM_DEVICE_NAME);
    }
    else
    {
        LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_OFDev_Probe: device not supported\n");
    }

    if (UMDevXS_OFDev_Device_p != NULL)
    {
        LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_OFDev_Probe: device is already initialized\n");
        return -EINVAL;
    }

    // Native for OS device reference
    UMDevXS_OFDev_Device_p = OF_Device_p;

    // Get the OF virtual interrupt number, it may not coincide with
    // the hardware interrupt number
#ifndef UMDEVXS_REMOVE_INTERRUPT
    // Exported under GPL
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34)
    UMDEVXS_OF_Virtual_IRQ =
            irq_of_parse_and_map(UMDevXS_OFDev_Device_p->dev.of_node, 0);
#else
    UMDEVXS_OF_Virtual_IRQ =
            irq_of_parse_and_map(UMDevXS_OFDev_Device_p->node, 0);
#endif // LINUX_VERSION_CODE
    LOG_INFO(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_OFDev_Probe: "
            "virtual IRQ %d\n",
            UMDEVXS_OF_Virtual_IRQ);
#endif // UMDEVXS_REMOVE_INTERRUPT

    LOG_CRIT(
        UMDEVXS_LOG_PREFIX
        "installed\n");

    IDENTIFIER_NOT_USED(id);

    // return 0 to indicate "we decided to take ownership"
    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_Remove
 */
static int
UMDevXS_OFDev_Remove(
        struct of_device * OF_Device_p)
{
    if (UMDevXS_OFDev_Device_p != OF_Device_p)
    {
        LOG_CRIT(
                 UMDEVXS_LOG_PREFIX
                 "UMDevXS_OFDev_Remove: "
                 "Failed, device 0x%p not initialized\n", OF_Device_p);
        return -EINVAL;
    }

#ifndef UMDEVXS_REMOVE_INTERRUPT
    if (UMDEVXS_OF_Virtual_IRQ)
    {
        // Exported under GPL
        irq_dispose_mapping(UMDEVXS_OF_Virtual_IRQ);

        UMDEVXS_OF_Virtual_IRQ = 0;
    }
#endif // UMDEVXS_REMOVE_INTERRUPT

    UMDevXS_MappedBaseAddr_p = NULL;

    LOG_CRIT(
        UMDEVXS_LOG_PREFIX
        "removed\n");

    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_Init
 *
 * Returns <0 on error.
 * Returns >=0 on success. The number is the interrupt number associated
 * with this OF device.
 */
int
UMDevXS_OFDev_Init(void)
{
    int Status;

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_OFDev_Init invoked\n");

    UMDevXS_OFDev_Device_p = NULL;

    Status = of_register_platform_driver(&UMDevXS_OFDev_Driver);
    if (Status)
    {
        LOG_CRIT(
            UMDEVXS_LOG_PREFIX
            "Failed to register the OF device (%d)\n",
            Status);

        return -1;
    }

    // UMDevXS_OFDev_Device_p is filled in by Probe function
    // but only when invoked!
    if (UMDevXS_OFDev_Device_p)
    {
#ifndef UMDEVXS_REMOVE_INTERRUPT
        return (int)UMDEVXS_OF_Virtual_IRQ;
#else
        return 0;
#endif // UMDEVXS_REMOVE_INTERRUPT
    }

    return -2;
}


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_UnInit
 */
void
UMDevXS_OFDev_UnInit(void)
{
    LOG_INFO(
             UMDEVXS_LOG_PREFIX
             "UMDevXS_OFDev_UnInit: "
             "calling of_unregister_platform_driver\n");

    of_unregister_platform_driver(&UMDevXS_OFDev_Driver);
}


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_Map
 */
int
UMDevXS_OFDev_Map(
        unsigned int SubsetStart,       // defined
        unsigned int SubsetSize,        // defined
        unsigned int Length,            // requested
        struct vm_area_struct * vma_p)
{
    unsigned long address = 0;
    int res;

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "UMDevXS_OFDev_Map: "
        "Subset=%u,%u, Length=%u\n",
        SubsetStart,
        SubsetSize,
        Length);

    // honor requested length if less than defined
    if (SubsetSize > Length)
        SubsetSize = Length;

    // reject requested length request if greater than defined
    if (Length > SubsetSize)
    {
        LOG_CRIT(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_OFDev_Map: "
            "length rejected (%u)\n",
            Length);

        return -1;
    }

    // was the OF device enabled by the OS
    // this only happens when a compatible OF device is found
    if (UMDevXS_OFDev_Device_p == NULL)
        return -1;

    // get OF device physical address
    {
        u32 * prop = NULL;
        struct device_node *node;
        int size, addr_cells, size_cells;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34)
        node = UMDevXS_OFDev_Device_p->dev.of_node;
#else
        node = UMDevXS_OFDev_Device_p->node;
#endif // LINUX_VERSION_CODE

        addr_cells = of_n_addr_cells(node);
        size_cells = of_n_size_cells(node);

        prop = (u32*)of_get_property(node, "reg", &size);

        // device tree specific for this OF device
        // only 32-bit physical addresses are supported
        if(prop &&
           size       == UMDEVXS_DEVICE_OF_ADDR_SIZE &&
           addr_cells == UMDEVXS_DEVICE_OF_ADDR_CELLS &&
           size_cells == UMDEVXS_DEVICE_OF_SIZE_CELLS)
        {
            LOG_INFO(
                    UMDEVXS_LOG_PREFIX
                    "UMDevXS_OFDev_Map: reg property addr %08x, size %08x, "
                    "size %d, addr cells = %d, size cells = %d\n",
                    prop[UMDEVXS_DEVICE_OF_ADDR_IDX],
                    prop[UMDEVXS_DEVICE_OF_SIZE_IDX],
                    size,
                    addr_cells,
                    size_cells);

            address = prop[UMDEVXS_DEVICE_OF_ADDR_IDX] + SubsetStart;
        }
        else
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_OFDev_Map: "
                "failed: prop = %p, "
                "size = %d, addr cells = %d, size cells = %d\n",
                prop, size, addr_cells, size_cells);

            return -2;
        }
    }

    // avoid caching and buffering
    vma_p->vm_page_prot = pgprot_noncached(vma_p->vm_page_prot);
    vma_p->vm_flags |= VM_IO | VM_RESERVED;

    // map the range into application space
    res = remap_pfn_range(
                    vma_p,
                    vma_p->vm_start,
                    address >> PAGE_SHIFT,
                    Length,
                    vma_p->vm_page_prot);

    if (res < 0)
    {
        LOG_CRIT(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_OFDev_Map: "
            "remap_pfn_range failed (%d), addr = %08x, len = %x\n",
            res, (unsigned int)address, Length);

        return res;
    }

    // return success
    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_OFDev_GetReference
 */
void*
UMDevXS_OFDev_GetReference(void)
{
    return (&UMDevXS_OFDev_Device_p->dev);
}


#endif // UMDEVXS_REMOVE_DEVICE_OF


/* end of file umdevxs_ofdev.c */
