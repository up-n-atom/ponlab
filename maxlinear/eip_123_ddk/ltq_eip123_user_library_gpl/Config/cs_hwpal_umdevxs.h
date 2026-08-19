/* cs_hwpal_umdevxs.h
 *
 * Configuration Settings for Driver Framework Implementation
 * for the SafeXcel-IP-123 HW2.0 Crypto Module.
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

// logging level for HWPAL Device
// Choose from LOG_SEVERITY_INFO, LOG_SEVERITY_WARN, LOG_SEVERITY_CRIT
#define HWPAL_LOG_SEVERITY  LOG_SEVERITY_WARN

// maximum allowed length for a device name
#define HWPAL_MAX_DEVICE_NAME_LENGTH 64

// debug checking and trace code
#define HWPAL_STRICT_ARGS_CHECK
#define HWPAL_DEVICE_MAGIC        54333
//#define HWPAL_TRACE_DEVICE_READ
//#define HWPAL_TRACE_DEVICE_WRITE

// device to request from UMDevXS driver
#define HWPAL_DEVICE0_UMDEVXS  "EIP123_HOST0"
#define HWPAL_DEVICE1_UMDEVXS  "EIP150"
#define HWPAL_DEVICE2_UMDEVXS  "EIP123_FPGA"

// definition of static resources inside the above device
// Refer to the data sheet of device for the correct values
//                   Name            DeviceNr   Start    Last     Flags (see below)
#define HWPAL_DEVICES \
    HWPAL_DEVICE_ADD("EIP123",       0,         0x00000, 0x03FFF, 0), \
    HWPAL_DEVICE_ADD("EIP123_AIC",   0,         0x03E00, 0x03E1F, 0), \
    HWPAL_DEVICE_ADD("EIP150",       1,         0x00000, 0x0FFFF, 0), \
    HWPAL_DEVICE_ADD("EIP150_TRNG",  1,         0x00000, 0x00080, 0), \
    HWPAL_DEVICE_ADD("EIP150_PKA",   1,         0x04000, 0x07FFF, 0), \
    HWPAL_DEVICE_ADD("EIP150_AIC",   1,         0x08000, 0x0801F, 0), \
    HWPAL_DEVICE_ADD("SZFPGA_CTRL",  2,         0x00000, 0x00FFF, 0), \
    HWPAL_DEVICE_ADD("SZFPGA_AIC",   2,         0x00100, 0x0011F, 0)
// Flags:
// (binary OR of the following)
//   0 = Disable trace
//   1 = Trace reads  (requires HWPAL_TRACE_DEVICE_READ)
//   2 = Trace writes (requires HWPAL_TRACE_DEVICE_WRITE)
//   4 = Swap word endianess before write / after read

// no remapping required
#define HWPAL_REMAP_ADDRESSES
/* device address remapping is done like this:
#define HWPAL_REMAP_ADDRESS \
      HWPAL_REMAP_ONE(_old, _new) \
      HWPAL_REMAP_ONE(_old, _new)
*/

// #of supported DMA resources
#define HWPAL_DMA_NRESOURCES 128

// only define this if the platform hardware guarantees cache coherency of
// DMA buffers, i.e. when SW does not need to do coherency management.
#undef HWPAL_ARCH_COHERENT

// disable support for PCI config space devices
#define HWPAL_REMOVE_DEVICE_PCICONFIGSPACE

/* end of file cs_hwpal_umdevxs.h */
