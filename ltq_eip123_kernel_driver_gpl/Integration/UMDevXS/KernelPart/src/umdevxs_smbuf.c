/* umdevxs_smbuf.c
 *
 * Shared Memory Buffer - Obtainer & Provider role.
 *
 * The provider allocates shared memory and hands it out to another host.
 * The obtainer can use memory allocated by another host.
 */

/*****************************************************************************
* Copyright (c) 2009-2015 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef UMDEVXS_REMOVE_SMBUF

#include "umdevxs_internal.h"
#include "log.h"

#include <linux/mm.h>           // remap_pfn_range & find_vma
#include <linux/sched.h>        // task_struct
#include <linux/types.h>        // uintptr_t
#include <asm/io.h>             // virt_to_phys
#include <asm/current.h>        // current
#include <asm/cacheflush.h>     // flush_cache_range


/*----------------------------------------------------------------------------
 * DMABuf_Alloc
 *
 * This implementation supports ignores the Bank field in RequestedProperties
 * and always uses __get_dma_pages to allocate memory.
 * compatible with "IPC Shared Memory". This function always allocates on a
 * 4kB page boundary, so the size must be a multiple of 4kB.
 */
static DMABuf_Status_t
DMABuf_Alloc(
        const DMABuf_Properties_t RequestedProperties,
        DMABuf_DevAddress_t * const DevAddr_p,
        BufAdmin_Handle_t * const Handle_p)
{
    BufAdmin_Handle_t Handle;
    BufAdmin_Record_t * Rec_p;
    int order;

    if (Handle_p == NULL ||
        DevAddr_p == NULL)
    {
        return DMABUF_ERROR_BAD_ARGUMENT;
    }

    // initialize the output parameters
    *Handle_p = BUFADMIN_HANDLE_NULL;
    DevAddr_p->p = NULL;

    // validate the properties
    if (RequestedProperties.Size == 0)
        return DMABUF_ERROR_BAD_ARGUMENT;

    if ((RequestedProperties.Size & (PAGE_SIZE - 1)) != 0)
        return DMABUF_ERROR_BAD_ARGUMENT;

    // we support up to 32 megabyte buffers
    if (RequestedProperties.Size >= 32*1024*1024)
        return DMABUF_ERROR_BAD_ARGUMENT;

    // let order=n, where n is smallest integer for which:
    // PAGE_SIZE * (2^n) >= RequestedProperties.Size
    order = get_order(RequestedProperties.Size);

    // create a record
    Handle = BufAdmin_Record_Create();
    if (Handle == BUFADMIN_HANDLE_NULL)
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "DMABuf_Alloc: "
            "Failed to create a handle\n");

        return DMABUF_ERROR_OUT_OF_MEMORY;
    }

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        // panic...
        goto DESTROY_HANDLE;
    }

    // allocate the memory
    {
        void * p;
				if(RequestedProperties.phyaddr == NULL){
        // Use __get_dma_pages() to ensure we get page aligned, DMA-capable
        // memory (i.e. from start of system RAM). Assume this function is
        // called from sleepable context, so no need to pass the GPF_ATOMIC flag.

        p = (void *)__get_dma_pages(GFP_KERNEL, order);

        if (p == NULL)
        {
            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "DMABuf_Alloc: "
                "__get_dma_pages(%u) failed\n",
                order);

            goto DESTROY_HANDLE;
        }

        // sanity-check the alignment
        // perhaps a bit too paranoid, but it does not hurt
        {
            unsigned int n = (unsigned int)(uintptr_t)p;

            if ((n & (PAGE_SIZE - 1)) != 0)
            {
                LOG_INFO(
                    UMDEVXS_LOG_PREFIX
                    "DMABuf_Alloc: "
                    "Unaligned start! %p not aligned to 0x%x\n",
                    p,
                    (unsigned int)(PAGE_SIZE - 1));

                free_pages((unsigned long)p, order);
                goto DESTROY_HANDLE;
            }
        }
					Rec_p->alloc.Nofree = 0;	
				}
				else
				{
					p = RequestedProperties.phyaddr;
					Rec_p->alloc.Nofree = 1;	
				}
        // fill in the record fields
        Rec_p->Magic = UMDEVXS_DMARESOURCE_MAGIC;

        Rec_p->alloc.AllocatedAddr_p = p;
        Rec_p->alloc.AllocatedSize = PAGE_SIZE << order;

        Rec_p->host.Alignment = PAGE_SIZE;
        Rec_p->host.HostAddr_p = p;
        Rec_p->host.BufferSize = RequestedProperties.Size;  // note: not the allocated "n"
    }

    // set the output parameters
    *Handle_p = Handle;
    DevAddr_p->p = (void *)virt_to_phys(Rec_p->alloc.AllocatedAddr_p);

    return DMABUF_STATUS_OK;

DESTROY_HANDLE:
    BufAdmin_Record_Destroy(Handle);
    return DMABUF_ERROR_OUT_OF_MEMORY;
}


/*----------------------------------------------------------------------------
 * DMABuf_Register
 *
 * This function must be used to register an "alien" buffer that was allocated
 * somewhere else. The caller guarantees that this buffer can be used for DMA.
 *
 * ActualProperties (input)
 *     Properties that describe the buffer that is being registered.
 *
 * Buffer_p (input)
 *     Pointer to the buffer. This pointer must be valid to use on the host
 *     in the domain of the driver.
 *
 * Alternative_p (input)
 *     Some allocators return two addresses. This parameter can be used to
 *     pass this second address to the driver. The type is pointer to ensure
 *     it is always large enough to hold a system address, also in LP64
 *     architecture. Set to NULL if not used.
 *
 * AllocatorRef (input)
 *     Number to describe the source of this buffer. The exact numbers
 *     supported is implementation specitic. This provides some flexibility
 *     for a specific implementation to support a number of "alien" buffers
 *     from different allocator and propertly interpret and use the
 *     Alternative_p parameter when translating the address to the device
 *     memory map. Set to zero if not used.
 *
 * Handle_p (output)
 *     Pointer to the memory location when the handle will be returned.
 *
 * Return Values
 *     DMABUF_STATUS_OK: Success, Handle_p was written.
 *     DMABUF_ERROR_BAD_ARGUMENT
 */
static DMABuf_Status_t
DMABuf_Register(
        const DMABuf_Properties_t ActualProperties,
        void * Buffer_p,
        void * Alternative_p,
        const char AllocatorRef,
        BufAdmin_Handle_t * const Handle_p)
{
    BufAdmin_Handle_t Handle;
    BufAdmin_Record_t * Rec_p;

    if (Handle_p == NULL)
        return DMABUF_ERROR_BAD_ARGUMENT;

    // initialize the output parameter
    *Handle_p = BUFADMIN_HANDLE_NULL;

    // validate the properties
    if (ActualProperties.Size == 0)
        return DMABUF_ERROR_BAD_ARGUMENT;

    // we support up to 32 megabyte buffers
    if (ActualProperties.Size >= 32*1024*1024)
        return DMABUF_ERROR_BAD_ARGUMENT;

    // create a record
    Handle = BufAdmin_Record_Create();
    if (Handle == BUFADMIN_HANDLE_NULL)
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "DMABuf_Alloc: "
            "Failed to create a handle\n");

        return DMABUF_ERROR_OUT_OF_MEMORY;
    }

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        // panic...
        goto DESTROY_HANDLE;
    }

    // register the memory
    {
        // fill in the record fields
        Rec_p->Magic = UMDEVXS_DMARESOURCE_MAGIC;

        Rec_p->alloc.AllocatedAddr_p = Buffer_p;
        Rec_p->alloc.AllocatedSize = ActualProperties.Size;
        Rec_p->alloc.Alternative_p = Alternative_p;
        //Rec_p->alloc.AllocatorRef = AllocatorRef;
        IDENTIFIER_NOT_USED(AllocatorRef);

        //Rec_p->host.fCached = ActualProperties.fCached;

        //Consider: check the current alignment
        Rec_p->host.Alignment = ActualProperties.Alignment;
        Rec_p->host.HostAddr_p = Rec_p->alloc.AllocatedAddr_p;
        Rec_p->host.BufferSize = Rec_p->alloc.AllocatedSize;
    }

    // set the output parameters
    *Handle_p = Handle;

    return DMABUF_STATUS_OK;

DESTROY_HANDLE:
    BufAdmin_Record_Destroy(Handle);
    return DMABUF_ERROR_OUT_OF_MEMORY;
}


/*----------------------------------------------------------------------------
 * DMABuf_Release
 *
 * Free the DMA resource (unless not allocated locally) and the record used
 * to describe it.
 *
 */
static DMABuf_Status_t
DMABuf_Release(
        BufAdmin_Handle_t Handle)
{

    if (BufAdmin_IsValidHandle(Handle))
    {
        BufAdmin_Record_t * Rec_p;

        Rec_p = BufAdmin_Handle2RecordPtr(Handle);
        if (Rec_p == NULL)
            return DMABUF_ERROR_INVALID_HANDLE;

        if (Rec_p->alloc.AllocatedAddr_p != NULL && Rec_p->alloc.Nofree == 0)
        {
            free_pages(
                (unsigned long)Rec_p->alloc.AllocatedAddr_p,
                get_order(Rec_p->alloc.AllocatedSize));
            Rec_p->alloc.AllocatedAddr_p = NULL;
        }

        Rec_p->Magic = 0;

        BufAdmin_Record_Destroy(Handle);

        return DMABUF_STATUS_OK;
    }

    return DMABUF_ERROR_INVALID_HANDLE;
}


/*----------------------------------------------------------------------------
 * HWPAL_DMAResource_PreDMA
 */
static void
HWPAL_DMAResource_PreDMA(
        BufAdmin_Handle_t Handle,
        const unsigned int ByteOffset,
        const unsigned int ByteCount)
{
    BufAdmin_Record_t * Rec_p;
    char * cpu_addr;
    size_t size;

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "HWPAL_DMAResource_PreDMA: "
            "Invalid handle %d\n",
            Handle);

        return;
    }

    // flush_cache_range needs virtual address
    cpu_addr = ((char *)Rec_p->user.Addr) + ByteOffset;
    if (ByteCount == 0)
    {
        size = Rec_p->host.BufferSize;
#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
        Log_FormattedMessage(
            "HWPAL_DMAResource_PreDMA: "
            "Handle=%d, "
            "Range=ALL (%u-%u)\n",
            Handle,
            ByteOffset,
            ByteOffset + (unsigned int)size - 1);
#endif
    }
    else
    {
        size = ByteCount;
#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
        Log_FormattedMessage(
                "HWPAL_DMAResource_PreDMA: "
                "Handle=%d, "
                "Range=%u-%u\n",
                Handle,
                ByteOffset,
                ByteOffset + (unsigned int)size - 1);
#endif
    }

#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
    LOG_INFO(
        "HWPAL_DMAResource_PreDMA: "
        "addr=%p, size=0x%08x\n",
        cpu_addr, (unsigned int)size);
#endif


    {
        struct vm_area_struct * vma;
        vma = find_vma(current->mm, (unsigned long)cpu_addr);
        if (vma)
        {
             flush_cache_range(
                     vma,
                     (unsigned long)cpu_addr,
                     ((unsigned long)cpu_addr)+size);
        }
        else
        {
             LOG_WARN(
                 "HWPAL_DMAResource_PreDMA: "
                 "find_vma failed for VM address %p", cpu_addr);
        }
    }
}


/*----------------------------------------------------------------------------
 * UMDevXS_DMABuf_SetAppID
 */
static void
UMDevXS_DMABuf_SetAppID(
        BufAdmin_Handle_t Handle,
        void * AppID)
{
    BufAdmin_Record_t * Rec_p;

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p)
        Rec_p->AppID = AppID;
}


/*----------------------------------------------------------------------------
 * UMDevXS_DMABufCleanUp_EnumFunc
 *
 * This function is called for each registered DMA buffer. We check the
 * AppID and release the buffer (and free the record) if no longer required.
 */
static void
UMDevXS_DMABufCleanUp_EnumFunc(
        BufAdmin_Handle_t Handle,
        BufAdmin_Record_t * const Rec_p,
        void * AppID)
{
    if (Rec_p->AppID == AppID)
    {
        LOG_WARN(
            "Cleaning up Handle=%d (0x%x)\n",
                Handle,
                UMDevXS_Handle_Make(UMDEVXS_HANDLECLASS_SMBUF,  (int)Handle));

        DMABuf_Release(Handle);
    }
}


/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_CleanUp
 *
 * This function looks up all SMBuf handles with the given AppID and frees
 * these by calling DMABuf_Release for each handle.
 */
void
UMDevXS_SMBuf_CleanUp(
        void * AppID)
{
    BufAdmin_Enumerate(UMDevXS_DMABufCleanUp_EnumFunc, AppID);
}


/*----------------------------------------------------------------------------
 * HWPAL_DMAResource_PostDMA
 */
static void
HWPAL_DMAResource_PostDMA(
        BufAdmin_Handle_t Handle,
        const unsigned int ByteOffset,
        const unsigned int ByteCount)
{
    BufAdmin_Record_t * Rec_p;
    char * cpu_addr;
    size_t size;

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "HWPAL_DMAResource_PostDMA: "
            "Invalid handle %d\n",
            Handle);

        return;
    }


    // dma_sync_single_for_cpu wants the bus address
    cpu_addr = ((char *)Rec_p->user.Addr) + ByteOffset;
    if (ByteCount == 0)
    {
        size = Rec_p->user.Size;
#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
        Log_FormattedMessage(
                "HWPAL_DMAResource_PostDMA: "
                "Handle=%d, "
                "Range=ALL (%u-%u)\n",
                Handle,
                ByteOffset,
                ByteOffset + (unsigned int)size - 1);
#endif
    }
    else
    {
         size = ByteCount;
#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
        Log_FormattedMessage(
                "HWPAL_DMAResource_PostDMA: "
                "Handle=%d, "
                "Range=%u-%u\n",
                Handle,
                ByteOffset,
                ByteOffset + (unsigned int)size - 1);
#endif
    }

#ifdef HWPAL_TRACE_DMARESOURCE_PREPOSTDMA
    LOG_INFO(
        "HWPAL_DMAResource_PostDMA: "
        "addr=%p, size=0x%08x\n",
        cpu_addr, (unsigned int)size);
#endif

    {
        struct vm_area_struct * vma;
        vma = find_vma(current->mm, (unsigned long)cpu_addr);
        if (vma)
        {
             flush_cache_range(
                     vma,
                     (unsigned long)cpu_addr,
                     ((unsigned long)cpu_addr)+size);
        }
        else
        {
             LOG_WARN(
                 "HWPAL_DMAResource_PostDMA: "
                 "find_vma failed for VM address %p", cpu_addr);
        }
    }
}


/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_Map
 */
int
UMDevXS_SMBuf_Map(
        int HandleIndex,
        unsigned int Length,
        struct vm_area_struct * vma_p)
{
    BufAdmin_Handle_t Handle = HandleIndex;
    BufAdmin_Record_t * Rec_p;

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
        return -1;

    // reject oversize mapping request
    if (Length > Rec_p->alloc.AllocatedSize)
        return -2;

    // now map the region into the application memory space
    {
        long StartOfs;
        int ret;

        // if the memory was allocated locally, derive the physical address
        // from Rec_p->alloc.AllocatedAddr_p, the kernel virtual address
        // returned by __get_dma_pages.
        // if the memory was foreign allocated, use the device address
        // that was passed via the SMBUF_ATTACH service.
        if (NULL != Rec_p->alloc.AllocatedAddr_p)
        {
             StartOfs = virt_to_phys(Rec_p->alloc.AllocatedAddr_p);
        }
        else
        {
             StartOfs = (long)Rec_p->alloc.Alternative_p;
        }

        LOG_INFO(
            UMDEVXS_LOG_PREFIX
            "UMDevXS_SMBuf_Map: "
            "Start=0x%lx, Size=%d\n",
            StartOfs,
            Length);

        if ((StartOfs & (PAGE_SIZE - 1)) != 0)
            return -4;

        // map the whole physically contiguous area in one piece
        ret = remap_pfn_range(
                    vma_p,
                    vma_p->vm_start,
                    StartOfs >> PAGE_SHIFT,
                    Length,
                    vma_p->vm_page_prot);

        if (ret < 0)
        {
            LOG_CRIT(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_SMBuf_Map: "
                "remap result: %d\n",
                ret);

            return -5;
        }

    }

    return 0;       // 0 = success
}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_Alloc
 *
 * This function handles the ALLOC command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_Alloc(
        void * AppID,
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;
    DMABuf_DevAddress_t DevAddr;
    BufAdmin_Record_t * Rec_p;
    unsigned int Size;
    unsigned int Alignment;

    if (CmdRsp_p == NULL)
        return;

    // get the size and round it up to a multiple of PAGE_SIZE
    Size = CmdRsp_p->uint1;
    Size = (Size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1);

    // this implementation ignores the Bank argument

    // get alignment and reject it if > PAGE_SIZE or not a power of 2
    Alignment = CmdRsp_p->uint3;
    if ((Alignment < 1) ||
        (Alignment > PAGE_SIZE) ||
        (Alignment != (Alignment & (-Alignment))))
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "UMDevXSLib_SMBuf_HandleCmd_Alloc: "
            "Invalid alignment (%d)\n",
            Alignment);

        CmdRsp_p->Error = 1;
        return;
    }


    {
        DMABuf_Properties_t Props = { 0 };
        DMABuf_Status_t dmares;

        Props.Size = Size;
        Props.Alignment = Alignment;
				if(CmdRsp_p->ptr1 != 0)
					Props.phyaddr = CmdRsp_p->ptr1;
        dmares = DMABuf_Alloc(Props, &DevAddr, &Handle);

        if (dmares != DMABUF_STATUS_OK)
        {
            LOG_WARN(
                UMDEVXS_LOG_PREFIX
                "UMDevXSLib_SMBuf_HandleCmd_Alloc: "
                "DMABuf_Alloc returned %d\n",
                dmares);

            CmdRsp_p->Error = 1;
            return;
        }
    }


    // link it to this application
    UMDevXS_DMABuf_SetAppID(Handle, AppID);

    // populate the output parameters
    CmdRsp_p->Handle = UMDevXS_Handle_Make(
                                UMDEVXS_HANDLECLASS_SMBUF,
                                (int)Handle);

    // get actually allocated size
    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p)
    {
        CmdRsp_p->uint1 = Rec_p->host.BufferSize;
        // alternatively, the possibly bigger Rec_p->alloc.ActualSize
        // could be used here; in that case, the Length check at the start
        // of UMDevXS_SMBuf_Map must be changed accordingly.
    }
    // In the extremely unlikely event that Rec_p is NULL here, the
    // requested size is returned as actual size

    CmdRsp_p->ptr1 = DevAddr.p;

}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_Register
 *
 * This function handles the REGISTER command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_Register(
        void * AppID,
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;
    BufAdmin_Handle_t RetHandle;
    BufAdmin_Record_t * Rec_p = NULL;
    unsigned int Size;
    void * BufPtr;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);
    Size = CmdRsp_p->uint1;
    BufPtr = CmdRsp_p->ptr1;

    if (BufAdmin_IsValidHandle(Handle))
    {
        Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    }

    if (Rec_p == NULL)
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "UMDevXSLib_SMBuf_HandleCmd_Register: "
            "Invalid handle %d\n",
            Handle);

        CmdRsp_p->Error = 1;
        return;
    }

    // check if Handle refers to a buffer that is already allocated or
    // attached by this application:
    if ((Rec_p->AppID != AppID) ||
        ((Rec_p->alloc.AllocatedAddr_p == NULL) &&
         (Rec_p->alloc.Alternative_p == NULL)) ||
        (Rec_p->user.Addr == NULL))
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "UMDevXSLib_SMBuf_HandleCmd_Register: "
            "Register denied, bad parent buffer %p\n",
            Rec_p);

        CmdRsp_p->Error = 1;
        return;
    }

    if ((Size > Rec_p->user.Size) ||
        (BufPtr < Rec_p->user.Addr) ||
        (BufPtr + Size > Rec_p->user.Addr + Rec_p->user.Size))
    {
        LOG_WARN(
            UMDEVXS_LOG_PREFIX
            "UMDevXSLib_SMBuf_HandleCmd_Register: "
            "Out-of-range address (%p) or size (%u)\n",
            BufPtr,
            Size);

        CmdRsp_p->Error = 1;
        return;
    }


    // register buffer and get the handle
    {
        DMABuf_Properties_t Props = { 0 };
        DMABuf_Status_t dmares;

        Props.Size = Size;
        Props.Bank = Rec_p->alloc.MemoryBank;

        // Pass NULL addresses to indicate this record is for a registered
        // buffer
        dmares = DMABuf_Register(
                            Props,
                            NULL,
                            /*Alternative_p:*/NULL,
                            /*AllocatorRef:*/0,
                            &RetHandle);

        if (dmares != DMABUF_STATUS_OK)
        {
            LOG_WARN(
                UMDEVXS_LOG_PREFIX
                "UMDevXSLib_SMBuf_HandleCmd_Register: "
                "DMABuf_Register returned %d\n",
                dmares);

            CmdRsp_p->Error = 9;
            return;
        }
    }

    // populate the record's .user fields
    Rec_p = BufAdmin_Handle2RecordPtr(RetHandle);
    if (Rec_p == NULL)
    {
        // highly unlikely...
        CmdRsp_p->Error = 2;
        return;
    }

    // Keep these fields zero so that UMDevXSProxy_SHMem_Free
    // can decide not to unmap when freeing registered memory!
    // This implies that registered memory cannot be sub-registered,
    // but this is also already forbidden in DMAResource_CheckAndRegister,
    // see DMAResourceLib_Find_Matching_DMAResource.
    // Rec_p->user.Addr = BufPtr;
    // Rec_p->user.Size = Size;

    // link it to this application
    UMDevXS_DMABuf_SetAppID(RetHandle, AppID);

    // populate the output parameters
    CmdRsp_p->Handle = UMDevXS_Handle_Make(
                                UMDEVXS_HANDLECLASS_SMBUF,
                                (int)RetHandle);

}

/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_Free
 *
 * This function handles the FREE command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_Free(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);

    DMABuf_Release(Handle);

    CmdRsp_p->Handle = 0;
}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_Attach
 *
 * This function handles the ATTACH command.
 * It creates a new record to store the passed info and returns a handle
 * for that record.
 * Passed info is: (phys) address (CmdRsp_p->ptr1)
 *                 size           (CmdRsp_p->uint1)
 *                 bank           (CmdRsp_p->uint2)
 * The info is typically used to implement a subsequent mmap request that
 * receives the handle through the `MapOffset' argument of the mmap call...
 */
static void
UMDevXSLib_SMBuf_HandleCmd_Attach(
        void * AppID,
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;
    void * BufAddr;
    unsigned int BufSize;

    if (CmdRsp_p == NULL)
        return;

    BufAddr = CmdRsp_p->ptr1;
    BufSize = CmdRsp_p->uint1;

    // register the buffer to get a handle
    {
        DMABuf_Properties_t Props = { 0 };
        DMABuf_Status_t dmares;

        Props.Size = BufSize;
        Props.Bank = CmdRsp_p->uint2;

        // Pass BufAddr as Alternative_p so created resource record
        // is reckognized as referring to foreign-allocated memory.
        dmares = DMABuf_Register(
                            Props,
                            NULL,
                            /*Alternative_p:*/BufAddr,
                            /*AllocatorRef:*/0,
                            &Handle);

        if (dmares != DMABUF_STATUS_OK)
        {
            LOG_WARN(
                UMDEVXS_LOG_PREFIX
                "UMDevXSLib_SMBuf_HandleCmd_Attach: "
                "DMABuf_Register returned %d\n",
                dmares);

            CmdRsp_p->Error = 9;
            return;
        }
    }

    // link this resource to this application
    UMDevXS_DMABuf_SetAppID(Handle, AppID);

    // populate the output parameters
    CmdRsp_p->Handle = UMDevXS_Handle_Make(
                                UMDEVXS_HANDLECLASS_SMBUF,
                                (int)Handle);

    CmdRsp_p->uint1 = BufSize;
}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_Detach
 *
 * This function handles the DETACH command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_Detach(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);

    DMABuf_Release(Handle);

    CmdRsp_p->Handle = 0;
}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_SetBufInfo
 *
 * This function handles the SETBUFINFO command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_SetBufInfo(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;
    BufAdmin_Record_t * Rec_p;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        CmdRsp_p->Error = 2;
        return;
    }

    Rec_p->user.Addr = CmdRsp_p->ptr1;
    Rec_p->user.Size = CmdRsp_p->uint1;
}


/*----------------------------------------------------------------------------
 * UMDevXSLib_SMBuf_HandleCmd_GetBufInfo
 *
 * This function handles the GETBUFINFO command.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_GetBufInfo(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;
    BufAdmin_Record_t * Rec_p;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);

    Rec_p = BufAdmin_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        CmdRsp_p->Error = 2;
        return;
    }

    CmdRsp_p->ptr1 = Rec_p->user.Addr;
    CmdRsp_p->uint1 = Rec_p->user.Size;
}



/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_HandleCmd_CommitRefresh
 *
 * This function handles COMMIT and REFRESH.
 */
static void
UMDevXSLib_SMBuf_HandleCmd_CommitRefresh(
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    BufAdmin_Handle_t Handle;

    if (CmdRsp_p == NULL)
        return;

    Handle = UMDevXS_Handle_GetIndex(CmdRsp_p->Handle);

    if (BufAdmin_IsValidHandle(Handle))
    {
        if (CmdRsp_p->Opcode == UMDEVXS_OPCODE_SMBUF_COMMIT)
        {
            HWPAL_DMAResource_PreDMA(Handle, CmdRsp_p->uint1, CmdRsp_p->uint2);
            return;
        }

        if (CmdRsp_p->Opcode == UMDEVXS_OPCODE_SMBUF_REFRESH)
        {
            HWPAL_DMAResource_PostDMA(Handle, CmdRsp_p->uint1, CmdRsp_p->uint2);
            return;
        }
    }

    CmdRsp_p->Error = 3;
}


/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_HandleCmd
 *
 * This function handles the ALLOC, REGISTER, FREE, ATTACH, DETACH,
 * GET/SETBUFINFO, COMMIT and REFRESH commands.
 */
void
UMDevXS_SMBuf_HandleCmd(
        void * AppID,
        UMDevXS_CmdRsp_t * const CmdRsp_p)
{
    if (CmdRsp_p == NULL)
        return;

    switch(CmdRsp_p->Opcode)
    {
        case UMDEVXS_OPCODE_SMBUF_ALLOC:
            // allocate an appropriate buffer using DMABuf
            UMDevXSLib_SMBuf_HandleCmd_Alloc(AppID, CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_REGISTER:
            // register an already allocated/attached DMA buffer
            UMDevXSLib_SMBuf_HandleCmd_Register(AppID, CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_FREE:
            // free an allocated or registered DMA buffer
            UMDevXSLib_SMBuf_HandleCmd_Free(CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_ATTACH:
            // attached to a shared memory buffer
            UMDevXSLib_SMBuf_HandleCmd_Attach(AppID, CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_DETACH:
            UMDevXSLib_SMBuf_HandleCmd_Detach(CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_SETBUFINFO:
            UMDevXSLib_SMBuf_HandleCmd_SetBufInfo(CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_GETBUFINFO:
            UMDevXSLib_SMBuf_HandleCmd_GetBufInfo(CmdRsp_p);
            break;

        case UMDEVXS_OPCODE_SMBUF_COMMIT:
        case UMDEVXS_OPCODE_SMBUF_REFRESH:
            UMDevXSLib_SMBuf_HandleCmd_CommitRefresh(CmdRsp_p);
            break;

        default:
            // unsupported command
            LOG_INFO(
                UMDEVXS_LOG_PREFIX
                "UMDevXS_SMBuf_HandleCmd: "
                "Unsupported opcode: %u\n",
                CmdRsp_p->Opcode);

            CmdRsp_p->Error = 123;
            break;
    } // switch
}


/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_Init
 */
int
UMDevXS_SMBuf_Init(void)
{
    // no initialization required, currently
    return 0;
}


/*----------------------------------------------------------------------------
 * UMDevXS_SMBuf_UnInit
 */
void
UMDevXS_SMBuf_UnInit(void)
{
}

#endif /* UMDEVXS_REMOVE_SMBUF */

/* end of file umdevxs_smbuf.c */
