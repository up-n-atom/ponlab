/* hwpal_device_umdevxs.c
 *
 * This is is the Linux User-mode Driver Framework v4 Device API
 * implementation for UMDevXS. The implementation is device-agnostic and
 * receives configuration details from the cs_hwpal_umdevxs.h file.
 * This implementation supports mapping two UMDevXS devices, to for example
 * allow control of the FPGA via another device.
 *
 * This implementation uses the UMDevXS Proxy and Kernel Driver to map a
 * memory region with the device registers into user space memory.
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

#include "c_hwpal_device_umdevxs.h" // get the configuration options

#include "basic_defs.h"             // uint32_t, NULL, inline, etc.
#include "clib.h"                   // memcmp

#include "device_mgmt.h"            // API to implement
#include "device_rw.h"              // API to implement
#include "device_swap.h"

#include "umdevxsproxy.h"           // UMDevXSProxy_Init
#include "umdevxsproxy_device.h"    // UMDevXSProxy_Device{Find,Map,Unmap}
#ifndef HWPAL_REMOVE_DEVICE_PCICONFIGSPACE
#include "umdevxsproxy_device_pcicfg.h" // UMDevXSProxy_Device_PciCfg_Read32,
                                        // UMDevXSProxy_Device_PciCfg_Write32
#endif

#undef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX  HWPAL_LOG_SEVERITY
#include "log.h"

#ifndef HWPAL_REMOVE_DEVICE_PCICONFIGSPACE
#define IOMEM32_MAGIC_PCICONFIGSPACE  0xFF434647   // 43 46 47 = C F G
#endif

typedef struct
{
    const char * DevName;
    unsigned int DeviceNr;
    unsigned int FirstOfs;
    unsigned int LastOfs;
    unsigned int Flags;              // swapping, tracing

#ifdef HWPAL_DEVICE_MAGIC
    unsigned int Magic;
#endif

} HWPALLib_DeviceAdmin_t;


#ifdef HWPAL_DEVICE_MAGIC
#define HWPAL_DEVICE_ADD(_name, _devnr, _firstofs, _lastofs, _flags) \
                       { _name, _devnr, _firstofs, _lastofs, _flags, \
                                                        HWPAL_DEVICE_MAGIC }
#else
#define HWPAL_DEVICE_ADD(_name, _devnr, _firstofs, _lastofs, _flags) \
                       { _name, _devnr, _firstofs, _lastofs, _flags }
#endif

static HWPALLib_DeviceAdmin_t HWPALLib_Devices[] =
{
    HWPAL_DEVICES
};

#define HWPALLIB_DEVICES_COUNT  \
            (sizeof(HWPALLib_Devices) / sizeof(HWPALLib_DeviceAdmin_t))

// info from UMDevXS Proxy
static struct
{
    uint32_t * Mem32_p;
    unsigned int Size;
} HWPALLib_UMDevXS_Devices[3];

// definition of the Flags
#define HWPAL_FLAGS_READ   BIT_0
#define HWPAL_FLAGS_WRITE  BIT_1
#define HWPAL_FLAGS_SWAP   BIT_2


/*----------------------------------------------------------------------------
 * Device_RemapDeviceAddress
 *
 * This function remaps certain device addresses (relative within the whole
 * device address map) to other addresses. This is needed when the integration
 * has remapped some EIP device registers to other addresses. The EIP Driver
 * Libraries assume the devices always have the same internal layout.
 */

// the cs_hwpal_device_lkm_pci.h file defines a HWPAL_REMAP_ADDRESSES that
// depends on the following HWPAL_REMAP_ONE

#define HWPAL_REMAP_ONE(_old, _new) \
    case _old: \
        DeviceByteOffset = _new; \
        break;

static inline unsigned int
Device_RemapDeviceAddress(
        unsigned int DeviceByteOffset)
{
    switch(DeviceByteOffset)
    {
        // include the remap statements
        HWPAL_REMAP_ADDRESSES

        default:
            break;
    }

    return DeviceByteOffset;
}


/*----------------------------------------------------------------------------
 * HWPALLib_Device2RecPtr
 *
 * This function converts an Device_Handle_t received via one of the
 * Device API functions into a HWPALLib_Devices record pointer, if it is
 * valid.
 *
 * Return Value
 *     NULL    Provided Device Handle was not valid
 *     other   Pointer to a HWPALLib_DeviceAdmin_t record
 */
static inline HWPALLib_DeviceAdmin_t *
HWPALLib_Device2RecordPtr(
        Device_Handle_t Device)
{
    // since we have so few records, we simply enumerate them
    HWPALLib_DeviceAdmin_t * p = (void *)Device;

    if (p < HWPALLib_Devices)
        return NULL;

    if (p >= HWPALLib_Devices + HWPALLIB_DEVICES_COUNT)
        return NULL;

#ifdef HWPAL_DEVICE_MAGIC
    if (p->Magic != HWPAL_DEVICE_MAGIC)
        return NULL;
#endif

    return p;
}


/*----------------------------------------------------------------------------
 * HWPALLib_IsValid
 *
 * This function checks that the parameters are valid to make the access.
 *
 * Device_p is valid
 * ByteOffset is inside device memory range
 * ByteOffset is 32-bit aligned
 */
static inline bool
HWPALLib_IsValid(
        const HWPALLib_DeviceAdmin_t * const Device_p,
        const unsigned int ByteOffset)
{
    if (Device_p == NULL)
        return false;

    if (ByteOffset & 3)
        return false;

    if (Device_p->FirstOfs + ByteOffset > Device_p->LastOfs)
        return false;

    return true;
}

/*------------------------------------------------------------------------------
 * device_mgmt API
 *
 * These functions support finding a device given its name.
 * A handle is returned that is needed in the device_rw API
 * to read or write the device
 */

/*------------------------------------------------------------------------------
 * HWPALLib_MapDevice
 */
static int
HWPALLib_MapDevice(
        unsigned int DeviceNr,
        const char * DeviceName_p)
{
    int res;
    int DeviceID;
    unsigned int Size;
    void * p;

    res = UMDevXSProxy_Device_Find(
                DeviceName_p,
                &DeviceID,
                &Size);

    if (res < 0)
    {
        LOG_CRIT(
            "Device_Initialize: "
            "Failed to find UMDevXS resource %d ('%s')\n",
            DeviceNr,
            DeviceName_p);

        return -1;
    }

    // add the device memory to our memory map
    p = UMDevXSProxy_Device_Map(DeviceID, Size);

    if (p == NULL)
    {
        LOG_CRIT(
            "Device_Initialize: "
            "Failed to map UMDevXS resource %d ('%s'), size %d\n",
            DeviceNr,
            DeviceName_p,
            Size);

        return -1;
    }

    HWPALLib_UMDevXS_Devices[DeviceNr].Mem32_p = p;
    HWPALLib_UMDevXS_Devices[DeviceNr].Size = Size;

    LOG_INFO(
        "Device_Initialize: "
        "Successfully mapped UMDevXS resource %d ('%s') of size=0x%08x\n",
        DeviceName_p,
        Size);

    return 0;       // 0 = success
}


/*------------------------------------------------------------------------------
 * Device_Initialize
 */
int
Device_Initialize(
        void * CustomInitData_p)
{
    int res;

    IDENTIFIER_NOT_USED(CustomInitData_p);

    res = UMDevXSProxy_Init();
    if (res < 0)
    {
        LOG_CRIT(
            "Device_Initialize: "
            "UMDevXSProxy_Init returned %d. "
            "Has the driver been loaded?\n",
            res);

        return -1;       // ## RETURN ##
    }

    // map the devices
    res = HWPALLib_MapDevice(0, HWPAL_DEVICE0_UMDEVXS);
    if (res < 0)
        return res - 100;

#ifdef HWPAL_DEVICE1_UMDEVXS
    HWPALLib_MapDevice(1, HWPAL_DEVICE1_UMDEVXS);
    if (res < 0)
        return res - 200;
#endif

#ifdef HWPAL_DEVICE2_UMDEVXS
    HWPALLib_MapDevice(2, HWPAL_DEVICE2_UMDEVXS);
    if (res < 0)
        return res - 300;
#endif

    return 0;   // 0 = success
}


void
Device_UnInitialize(void)
{
    UMDevXSProxy_Shutdown();

    HWPALLib_UMDevXS_Devices[0].Mem32_p = NULL;
    HWPALLib_UMDevXS_Devices[0].Size = 0;

    HWPALLib_UMDevXS_Devices[1].Mem32_p = NULL;
    HWPALLib_UMDevXS_Devices[1].Size = 0;

    HWPALLib_UMDevXS_Devices[2].Mem32_p = NULL;
    HWPALLib_UMDevXS_Devices[2].Size = 0;
}


/*-----------------------------------------------------------------------------
 * Device_Find
 */
Device_Handle_t
Device_Find(
        const char * DeviceName_p)
{
    int i;
    unsigned int NameLen;

    if (DeviceName_p == NULL)
    {
        // not supported, thus not found
        return NULL;
    }

    if (HWPALLib_UMDevXS_Devices[0].Mem32_p == NULL)
    {
        // if Device_Initialize failed (or wasn't called yet),
        // fail to find any device
        return NULL;
    }

    // count the device name length, including the terminating zero
    NameLen = 0;
    while (DeviceName_p[NameLen++])
    {
        if (NameLen == HWPAL_MAX_DEVICE_NAME_LENGTH)
        {
            break;
        }
    }

    // walk through the defined devices and compare the name
    for (i = 0; i < HWPALLIB_DEVICES_COUNT; i++)
    {
        if (memcmp(
                DeviceName_p,
                HWPALLib_Devices[i].DevName,
                NameLen) == 0)
        {
            // check that the device is valid
            unsigned int DeviceNr = HWPALLib_Devices[i].DeviceNr;
            if (DeviceNr > 2)
                return NULL;

            // is the device mapped?
            if (HWPALLib_UMDevXS_Devices[DeviceNr].Mem32_p == NULL)
                return NULL;

            // Return the device handle
            return (Device_Handle_t)(HWPALLib_Devices + i);
        }
    }

    LOG_WARN(
        "Device_Find: "
        "Could not find device '%s'\n",
        DeviceName_p);

    return NULL;
}


/*------------------------------------------------------------------------------
 * device_rw API
 *
 * These functions can be used to transfer a single 32bit word or an array of
 * 32bit words to or from a device.
 * Endianess swapping is performed on the fly based on the configuration for
 * this device.
 *
 */

/*------------------------------------------------------------------------------
 * Device_Read32
 */
uint32_t
Device_Read32(
        const Device_Handle_t Device,
        const unsigned int ByteOffset)
{
    HWPALLib_DeviceAdmin_t * Device_p;
    uint32_t WordRead;
    unsigned int Idx;

    Device_p = HWPALLib_Device2RecordPtr(Device);

    if (!HWPALLib_IsValid(Device_p, ByteOffset))
    {
        LOG_WARN(
            "Device_Read32: "
            "Invalid Device (%p) or ByteOffset (%u)\n",
            Device,
            ByteOffset);

        return 0xEEEEEEEE;
    }

#ifndef HWPAL_REMOVE_DEVICE_PCICONFIGSPACE
    if (Device_p->FirstOfs == IOMEM32_MAGIC_PCICONFIGSPACE)
    {
        UMDevXSProxy_Device_PciCfg_Read32(ByteOffset, &WordRead);
    }
    else
#endif
    {
        unsigned int DeviceByteOffset = Device_p->FirstOfs + ByteOffset;

        DeviceByteOffset = Device_RemapDeviceAddress(DeviceByteOffset);

        Idx = DeviceByteOffset >> 2;
        WordRead = HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p[Idx];

#ifdef HWPAL_DEVICE_ENABLE_SWAP
        if (Device_p->Flags & HWPAL_FLAGS_SWAP)
            WordRead = Device_SwapEndian32(WordRead);
#endif
    }

#ifdef HWPAL_TRACE_DEVICE_READ
    if (Device_p->Flags & HWPAL_FLAGS_READ)
    {
        unsigned int DeviceByteOffset = Device_p->FirstOfs + ByteOffset;
        unsigned int DeviceByteOffset2 =
                Device_RemapDeviceAddress(DeviceByteOffset);

        if (DeviceByteOffset2 != DeviceByteOffset)
        {
            DeviceByteOffset2 -= Device_p->FirstOfs;
            Log_FormattedMessage(
                    "Device_Read32: "
                    "0x%x(was 0x%x) = 0x%08x (%s)\n",
                    DeviceByteOffset2,
                    ByteOffset,
                    WordRead,
                    Device_p->DevName);
        }
        else
        {
            Log_FormattedMessage(
                "Device_Read32: %s@0x%08x => 0x%08x, dev nr = %d, "
                "addr = %p, offset = 0x%08x\n",
                Device_p->DevName,
                ByteOffset,
                WordRead,
                Device_p->DeviceNr,
                (void *)HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p,
                Device_p->FirstOfs + ByteOffset);
        }
    }
#endif

    return WordRead;
}


/*------------------------------------------------------------------------------
 * Device_Write32
 */
void
Device_Write32(
        const Device_Handle_t Device,
        const unsigned int ByteOffset,
        const uint32_t Value)
{
    HWPALLib_DeviceAdmin_t * Device_p;
    uint32_t WordWrite = Value;
    unsigned int Idx;

    Device_p = HWPALLib_Device2RecordPtr(Device);

    if (!HWPALLib_IsValid(Device_p, ByteOffset))
    {
        LOG_WARN(
            "Device_Write32: "
            "Invalid Device (%p) or ByteOffset (%u)\n",
            Device,
            ByteOffset);

        return;
    }

#ifdef HWPAL_TRACE_DEVICE_WRITE
    if (Device_p->Flags & HWPAL_FLAGS_WRITE)
    {
        unsigned int DeviceByteOffset = Device_p->FirstOfs + ByteOffset;
        unsigned int DeviceByteOffset2 =
                Device_RemapDeviceAddress(DeviceByteOffset);

        if (DeviceByteOffset2 != DeviceByteOffset)
        {
            DeviceByteOffset2 -= Device_p->FirstOfs;
            Log_FormattedMessage(
                    "Device_Write32: "
                    "0x%x(was 0x%x) = 0x%08x (%s)\n",
                    DeviceByteOffset2,
                    ByteOffset,
                    Value,
                    Device_p->DevName);
        }
        else
        {
            Log_FormattedMessage(
                "Device_Write32: %s@0x%08x = 0x%08x, dev nr = %d, "
                "addr = %p, offset = 0x%08x\n",
                Device_p->DevName,
                ByteOffset,
                Value,
                Device_p->DeviceNr,
                (void *)HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p,
                Device_p->FirstOfs + ByteOffset);
        }
    }
#endif

#ifndef HWPAL_REMOVE_DEVICE_PCICONFIGSPACE
    if (Device_p->FirstOfs == IOMEM32_MAGIC_PCICONFIGSPACE)
    {
        UMDevXSProxy_Device_PciCfg_Write32(ByteOffset, WordWrite);
    }
    else
#endif
    {
        uint32_t DeviceByteOffset = Device_p->FirstOfs + ByteOffset;

        DeviceByteOffset = Device_RemapDeviceAddress(DeviceByteOffset);

#ifdef HWPAL_DEVICE_ENABLE_SWAP
        if (Device_p->Flags & HWPAL_FLAGS_SWAP)
            WordWrite = Device_SwapEndian32(WordWrite);
#endif

        Idx = DeviceByteOffset >> 2;
        HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p[Idx] = WordWrite;

    }

    return;
}

void
Device_Read32Array(
        const Device_Handle_t Device,
        const unsigned int StartByteOffset,
        uint32_t * MemoryDst_p,
        const int Count)
{
    HWPALLib_DeviceAdmin_t * Device_p;
    uint32_t WordRead;
    unsigned int Idx;
    int Nwords;

    Device_p = HWPALLib_Device2RecordPtr(Device);

    if (Count == 0)
    {
        // avoid that `Count-1' goes negative in test below
        return;
    }

    if ((Count < 0) ||
        !HWPALLib_IsValid(Device_p, StartByteOffset) ||
        !HWPALLib_IsValid(Device_p, StartByteOffset + (Count - 1) * 4))
    {
        LOG_WARN(
            "Device_Read32Array: "
            "Invalid Device (%p) or read area (%u-%u)\n",
            Device,
            StartByteOffset,
            (unsigned int)(StartByteOffset + (Count - 1) * sizeof(uint32_t)));

        return;
    }

    Idx = (Device_p->FirstOfs + StartByteOffset) >> 2;
    for (Nwords = 0; Nwords < Count; ++Nwords, ++Idx)
    {
        WordRead = HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p[Idx];

#ifdef HWPAL_DEVICE_ENABLE_SWAP
        if (Device_p->Flags & HWPAL_FLAGS_SWAP)
            WordRead = Device_SwapEndian32(WordRead);
#endif

        MemoryDst_p[Nwords] = WordRead;

#ifdef HWPAL_TRACE_DEVICE_READ
        if (Device_p->Flags & HWPAL_FLAGS_READ)
        {
            Log_FormattedMessage(
                "Device_Read32Array: rd %s@0x%08x => 0x%08x\n",
                Device_p->DevName,
                (Nwords << 2) + Device_p->FirstOfs,
                WordRead);
        }
#endif
    }
}

void
Device_Write32Array(
        const Device_Handle_t Device,
        const unsigned int StartByteOffset,
        const uint32_t * MemorySrc_p,
        const int Count)
{
    HWPALLib_DeviceAdmin_t * Device_p;
    uint32_t WordWrite;
    unsigned int Idx;
    int Nwords;

    Device_p = HWPALLib_Device2RecordPtr(Device);

    if (Count == 0)
    {
        // avoid that `Count-1' goes negative in test below
        return;
    }

    if ((Count < 0) ||
        !HWPALLib_IsValid(Device_p, StartByteOffset) ||
        !HWPALLib_IsValid(Device_p, StartByteOffset + (Count - 1) * 4))
    {
        LOG_WARN(
            "Device_Write32Array: "
            "Invalid Device (%p) or write area (%u-%u)\n",
            Device,
            StartByteOffset,
            (unsigned int)(StartByteOffset + (Count - 1) * sizeof(uint32_t)));

        return;
    }

    Idx = (Device_p->FirstOfs + StartByteOffset) >> 2;
    for (Nwords = 0; Nwords < Count; ++Nwords, ++Idx)
    {
        WordWrite = MemorySrc_p[Nwords];

#ifdef HWPAL_DEVICE_ENABLE_SWAP
        if (Device_p->Flags & HWPAL_FLAGS_SWAP)
            WordWrite = Device_SwapEndian32(WordWrite);
#endif

        HWPALLib_UMDevXS_Devices[Device_p->DeviceNr].Mem32_p[Idx] = WordWrite;

#ifdef HWPAL_TRACE_DEVICE_WRITE
        if (Device_p->Flags & HWPAL_FLAGS_WRITE)
        {
            Log_FormattedMessage(
                "Device_Write32Array: wr %s@0x%08x = 0x%08x\n",
                Device_p->DevName,
                (Nwords << 2) + Device_p->FirstOfs,
                WordWrite);
        }
#endif
    }
}


/* end of file hwpal_device_umdevxs.c */
