/* cs_umdevxs_pci.h
 *
 * Configuration Switches for UMDevXS Kernel driver for
 *  PCI driver for Versatile FPGA
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

// PCI device Vendor ID and Device ID
// used to match the driver to the appropriate device

// FPGA device on ARM Versatile
#define UMDEVXS_PCI_VENDOR_ID  0x10EE
#define UMDEVXS_PCI_DEVICE_ID  0x0300

// logging level (choose one)
#define LOG_SEVERITY_MAX LOG_SEVERITY_CRIT
//#define LOG_SEVERITY_MAX LOG_SEVERITY_WARN
//#define LOG_SEVERITY_MAX LOG_SEVERITY_INFO

// uncomment to enable device logging when an error occurs
//#define UMDEVXS_CHRDEV_LOG_ERRORS

// uncomment to enable Pre- and Post-DMA logging
//#define HWPAL_TRACE_DMARESOURCE_PREPOSTDMA

#define UMDEVXS_LOG_PREFIX "UMDevXS_PCI: "

#define UMDEVXS_MODULENAME "umdevxs"

// uncomment to remove selected functionality
//#define UMDEVXS_REMOVE_DEVICE
//#define UMDEVXS_REMOVE_SMBUF
//#define UMDEVXS_REMOVE_PCI
#define UMDEVXS_REMOVE_SIMULATION
//#define UMDEVXS_REMOVE_INTERRUPT
#define UMDEVXS_REMOVE_DEVICE_PCICFG
#define UMDEVXS_REMOVE_DEVICE_OF

// Definition of device resources
// UMDEVXS_DEVICE_ADD      Name               Start  Last
// UMDEVXS_DEVICE_ADD_PCI  Name               Bar    Start       Size
// UMDEVXS_DEVICE_ADD_SIM  Name               Size
#define UMDEVXS_DEVICES \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST0",    1,     0x00000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST1",    1,     0x10000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST2",    1,     0x20000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST3",    1,     0x30000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST4",    1,     0x40000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST5",    1,     0x50000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST6",    1,     0x60000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_HOST7",    1,     0x70000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP150",          1,    0x100000,   0x10000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP150_PKA",      1,    0x104000,    0x4000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP150_PKA_PRG",  1,    0x106000,    0x2000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP150_AIC",      1,    0x108000,     0x100), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_FPGA",     2,     0x00000,    0x1000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_FPGA_NVM", 2,     0x01000,    0x1000), \
    UMDEVXS_DEVICE_ADD_PCI("EIP123_FPGA_PRG", 2,     0x10000,    0x10000)

// in addition to the above devices, the memory windows of the
// PCI devices can be retrieved as PCI.<bar> with <bar> being
// a single digit and in the valid range for the device (0..n).
// To more easily support large windows, we allow a subset to
// used using the following switches
#define UMDEVXS_PCI_BAR0_SUBSET_START  0
#define UMDEVXS_PCI_BAR0_SUBSET_SIZE   4*1024

// BAR 1 is the design (EIP-123 / EIP-150)
#define UMDEVXS_PCI_BAR1_SUBSET_START  0
#define UMDEVXS_PCI_BAR1_SUBSET_SIZE   2*1024*1024

// BAR 2 is the EIP-123 FPGA glue around the design
#define UMDEVXS_PCI_BAR2_SUBSET_START  0
#define UMDEVXS_PCI_BAR2_SUBSET_SIZE   128*1024

#define UMDEVXS_PCI_BAR3_SUBSET_START  0
#define UMDEVXS_PCI_BAR3_SUBSET_SIZE   4*1024

// Enable when using MSI interrupts on PCI
//#define UMDEVXS_USE_MSI

/* end of file cs_umdevxs_pci.h */
