/* umdevxs_lkm.c
 *
 * Loadable Kernel Module (LKM) support the Linux UMPCI driver.
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
#include "umdevxs_internal.h"

#include <linux/errno.h>
#include <linux/module.h>

#include "tool_eip123.h"

MODULE_LICENSE(UMDEVXS_LICENSE);


/*----------------------------------------------------------------------------
 * UMDevXS_module_init
 */
static int
UMDevXS_module_init(void)
{
    int Status;
#if (!defined(UMDEVXS_REMOVE_PCI) || \
     !defined(UMDEVXS_REMOVE_INTERRUPT) || \
     !defined(UMDEVXS_REMOVE_DEVICE_OF))
    int nIRQ = UMDEVXS_INTERRUPT_STATIC_IRQ;
#endif

    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "loading driver\n");

    /* !<WW: temp put here until decided */
    eip123_tool_hw_reset();
    //eip123_tool_set_axi_slave_endian(0); // little endian

#ifndef UMDEVXS_REMOVE_PCI
    Status = UMDevXS_PCIDev_Init();
    if (Status < 0)
        return Status;
    nIRQ = Status;
#endif

#ifndef UMDEVXS_REMOVE_DEVICE_OF
    Status = UMDevXS_OFDev_Init();
    if (Status < 0)
        return Status;
    nIRQ = Status;
#endif

#ifndef UMDEVXS_REMOVE_INTERRUPT
    UMDevXS_Interrupt_Init(nIRQ);
#endif

    Status = UMDevXS_ChrDev_Init();
    if (Status < 0)
        return Status;

#ifndef UMDEVXS_REMOVE_DMABUF
    if (!BufAdmin_Init(UMDEVXS_DMARESOURCE_HANDLES_MAX, NULL, 0))
        return -ENOMEM;
#endif

#ifndef UMDEVXS_REMOVE_SMBUF
    Status = UMDevXS_SMBuf_Init();
    if (Status < 0)
        return Status;
#endif

    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_module_exit
 */
static void
UMDevXS_module_exit(void)
{
    LOG_INFO(
        UMDEVXS_LOG_PREFIX
        "unloading driver\n");

#ifndef UMDEVXS_REMOVE_SMBUF
    UMDevXS_SMBuf_UnInit();
#endif

#ifndef UMDEVXS_REMOVE_INTERRUPT
    UMDevXS_Interrupt_UnInit();
#endif

#ifndef UMDEVXS_REMOVE_PCI
    UMDevXS_PCIDev_UnInit();
#endif

#ifndef UMDEVXS_REMOVE_DEVICE_OF
    UMDevXS_OFDev_UnInit();
#endif

    UMDevXS_ChrDev_UnInit();

#ifndef UMDEVXS_REMOVE_DMABUF
    BufAdmin_UnInit();
#endif
}

module_init(UMDevXS_module_init);
module_exit(UMDevXS_module_exit);

/* end of file umdevxs_lkm.c */
