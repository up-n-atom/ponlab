/* hwpal_dmares_umdevxs.c
 *
 * Linux User-Mode implementation of the Driver Framework v4 DMAResource API
 * using the UMDevXS services.
 * The implementation of the address translation API is in a separate file,
 * to make it simpler to specialize it for a specific environment. That file
 * also describes the various address domains in more detail. See for example
 * "hwpal_dmares_addr_tw_fpga.c" or "hwpal_dmares_addr_cm_fpga.c".
 *
 * Implementation-specific details:
 *
 * - Alloc requests are passed to the kernel driver that uses __get_dma_pages
 *   to get page-aligned memory suitable for DMA. Page alignment is required
 *   since the memory is also mapped into user space (via remap_pfn_range).
 *   Both the user space address and device (aka physical, bus) address for
 *   the memory is returned so that subsequent address translation between
 *   DMARES_DOMAIN_HOST and DMARES_DOMAIN_BUS can be supported.
 *
 * - Attach requests are also passed to the kernel driver to obtain a user
 *   space address for the attached memory.
 *
 * - Pre/PostDMA requests are passed to the kernel driver, unless the fCached
 *   flag for the given DMAResource is FALSE. The kernel driver uses
 *   "flush_cache_range" to trigger the platform-specific (ARM) cache clean/
 *   invalidate functionality.
 *   The fCached flag for a DMAResource is always set to TRUE, unless
 *   HWPAL_ARCH_COHERENT is #defined (in "cs_hwpal.h").
 *   Maybe the kernel driver should pass the fact whether allocated memory
 *   is actually cached or not back to the requesting client, but this
 *   has not been implemented (yet).
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

#include "c_hwpal_dmares_umdevxs.h" // get the configuration options

#include "basic_defs.h"
#include "clib.h"           // memset

#include "dmares_mgmt.h"    // the API to implement
#include "dmares_buf.h"
#include "dmares_addr.h"
#include "dmares_rw.h"
#include "device_swap.h"    // Device_SwapEndian32

#undef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX  HWPAL_LOG_SEVERITY
#include "log.h"

#include "umdevxsproxy.h"           // UMDevXSProxy_Init
#include "umdevxsproxy_shmem.h"

#include <pthread.h>        // pthread_mutex_*
#include <stdlib.h>         // malloc, free
#include <unistd.h>         // getpagesize

/*

 Requirements on the records:
  - pre-allocated array of records
  - valid between Create and Destroy
  - re-use on a least-recently-used basis to make sure accidental continued
    use after destroy does not cause crashes, allowing us to detect the
    situation instead of crashing quickly.

 Requirements on the handles:
  - one handle per record
  - valid between Create and Destroy
  - quickly find the ptr-to-record belonging to the handle
  - detect continued use of a handle after Destroy
  - caller-hidden admin/status, thus not inside the record
  - report leaking handles upon exit

 Solution:
  - handle cannot be a record number (no post-destroy use detection possible)
  - recnr/destroyed in separate memory location for each handle: Handles_p
  - Array of records: Records_p
  - free locations in Array1: Freelist1 (FreeHandles)
  - free record numbers list: Freelist2 (FreeRecords)
 */

typedef struct
{
    int ReadIndex;
    int WriteIndex;
    int * Nrs_p;
} DMAResourceLib_FreeList_t;

typedef struct
{
    int CurIndex;
} DMAResourceLib_InUseHandles_Iterator_t;

static int HandlesCount = 0; // remainder are valid only when this is != 0
static int * Handles_p;
static DMAResource_Record_t * Records_p;
static DMAResourceLib_FreeList_t FreeHandles;
static DMAResourceLib_FreeList_t FreeRecords;

static pthread_mutex_t HWPAL_Mutex;
#define ENTER_CRITICAL_SECTION \
    pthread_mutex_lock(&HWPAL_Mutex)
#define LEAVE_CRITICAL_SECTION \
    pthread_mutex_unlock(&HWPAL_Mutex)

#define HWPAL_RECNR_DESTROYED  -1


/*----------------------------------------------------------------------------
 * DMAResourceLib_FreeList_Get
 *
 * Gets the next entry from the freelist. Returns -1 when the list is empty.
 */
static inline int
DMAResourceLib_FreeList_Get(
        DMAResourceLib_FreeList_t * const List_p)
{
    int Nr = -1;
    int ReadIndex_Updated = List_p->ReadIndex + 1;

    if (ReadIndex_Updated >= HandlesCount)
        ReadIndex_Updated = 0;

    // if post-increment ReadIndex == WriteIndex, the list is empty
    if (ReadIndex_Updated != List_p->WriteIndex)
    {
        // grab the next number
        Nr = List_p->Nrs_p[List_p->ReadIndex];
        List_p->ReadIndex = ReadIndex_Updated;
    }

    return Nr;
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_FreeList_Add
 *
 * Adds an entry to the freelist.
 */
static inline void
DMAResourceLib_FreeList_Add(
        DMAResourceLib_FreeList_t * const List_p,
        int Nr)
{
    if (List_p->WriteIndex == List_p->ReadIndex)
    {
        LOG_WARN(
            "DMAResourceLib_FreeList_Add: "
            "Attempt to add value %d to full list\n",
            Nr);
        return;
    }

    if (Nr < 0 || Nr >= HandlesCount)
    {
        LOG_WARN(
            "DMAResourceLib_FreeList_Add: "
            "Attempt to put invalid value: %d\n",
            Nr);
        return;
    }

    {
        int WriteIndex_Updated = List_p->WriteIndex + 1;
        if (WriteIndex_Updated >= HandlesCount)
            WriteIndex_Updated = 0;

        // store the number
        List_p->Nrs_p[List_p->WriteIndex] = Nr;
        List_p->WriteIndex = WriteIndex_Updated;
    }
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_InUseHandles_*
 *
 * Helper functions to iterate over all currently in-use handles.
 *
 * Usage:
 *     DMAResourceLib_InUseHandles_Iterator_t it;
 *     for (Handle = DMAResourceLib_InUseHandles_First(&it);
 *          Handle != NULL;
 *          Handle = DMAResourceLib_InUseHandles_Next(&it))
 *     { ...
 *
 */
static inline DMAResource_Record_t *
DMAResourceLib_InUseHandles_Get(
        DMAResourceLib_InUseHandles_Iterator_t * const it)
{
    DMAResource_Record_t * Rec_p;

    do
    {
        if (it->CurIndex >= HandlesCount)
            return NULL;

        Rec_p = Records_p + it->CurIndex++;

        if (Rec_p->Magic != DMARES_RECORD_MAGIC)
            Rec_p = NULL;
    }
    while(Rec_p == NULL);

    return Rec_p;
}


static inline DMAResource_Record_t *
DMAResourceLib_InUseHandles_First(
        DMAResourceLib_InUseHandles_Iterator_t * const it)
{
    it->CurIndex = 0;
    return DMAResourceLib_InUseHandles_Get(it);
}


static inline DMAResource_Record_t *
DMAResourceLib_InUseHandles_Next(
        DMAResourceLib_InUseHandles_Iterator_t * const it)
{
    return DMAResourceLib_InUseHandles_Get(it);
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_LookupDomain
 *
 * Lookup given domain in Rec_p->AddrPairs array.
 */

/* static */ DMAResource_AddrPair_t *
DMAResourceLib_LookupDomain(
        const DMAResource_Record_t * Rec_p,
        const DMAResource_AddrDomain_t Domain)
{
    const DMAResource_AddrPair_t * res = Rec_p->AddrPairs;

    while (res->Domain != Domain)
    {
        if (res->Domain == 0)
        {
            return NULL;
        }

        if (++res == Rec_p->AddrPairs + DMARES_ADDRPAIRS_CAPACITY)
        {
            return NULL;
        }
    }

    return (DMAResource_AddrPair_t *)res;
}

// except when x is zero,
// (x & (-x)) returns a value where all bits of `x' have
// been cleared except the right-most '1'
#define IS_POWER_OF_TWO(_x) (((_x) & (0 - (_x))) == (_x))


/*----------------------------------------------------------------------------
 * DMAResourceLib_IsSaneInput
 *
 * Return true iff the DMAResource defined by the given address pair
 * and properties appears to be valid.
 *
 */
/* static */ bool
DMAResourceLib_IsSaneInput(
        const DMAResource_AddrPair_t * AddrPair_p,
        const DMAResource_Properties_t * Props_p)
{
    unsigned int Alignment = Props_p->Alignment;

    if ((Alignment < 1) ||
        (Alignment > (unsigned int)getpagesize()) ||
        !IS_POWER_OF_TWO(Alignment))
    {
        LOG_WARN(
            "DMAResourceLib_IsSaneInput: "
            "Bad alignment value: 0x%x\n",
            Alignment);
        return false;
    }

    if (Props_p->Size == 0 ||
        Props_p->Size >= BIT_30)     // 1GB
    {
        LOG_WARN(
            "DMAResourceLib_IsSaneInput: "
            "Bad size value: 0x%x\n",
            Props_p->Size);
        return false;
    }

    if (AddrPair_p != NULL)
    {
        unsigned int Address = (unsigned int)(uintptr_t)AddrPair_p->Address_p;

        // Reject NULL as address
        if (Address == 0)
        {
            LOG_WARN(
                "DMAResourceLib_IsSaneInput: "
                "Bad address: %p\n",
                AddrPair_p->Address_p);
            return false;
        }

        // Verify if address is consistent with alignment
        if ((Address & (Alignment-1)) != 0)
        {
            LOG_WARN(
                "DMAResourceLib_IsSaneInput: "
                "Address and alignment not consistent: %p & %x\n",
                AddrPair_p->Address_p,
                Alignment);
            return false;
        }
    }

    return true;
}

/*----------------------------------------------------------------------------
 * DMAResourceLib_IsSubRangeOf
 *
 * Return true iff the address range defined by `AddrPair1' and `Size1' is
 * within the address range defined by `AddrPair2' and `Size2'.
 */
static bool
DMAResourceLib_IsSubRangeOf(
        const DMAResource_AddrPair_t * const AddrPair1,
        const unsigned int Size1,
        const DMAResource_AddrPair_t * const AddrPair2,
        const unsigned int Size2)
{
    if (AddrPair1->Domain == AddrPair2->Domain)
    {
        const uint8_t * Addr1 = AddrPair1->Address_p;
        const uint8_t * Addr2 = AddrPair2->Address_p;

        if ((Size1 <= Size2) &&
            (Addr2 <= Addr1) &&
            ((Addr1 + Size1) <= (Addr2 + Size2)))
        {
            return true;
        }
    }

    return false;
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_Find_Matching_DMAResource
 *
 * Return a pointer to the DMAResource record for a currently allocated or
 * attached DMA buffer that matches the given `Properties' and `AddrPair'.
 * The match can be either exact or indicate that the buffer defined by
 * `Properties and `AddrPair' is a proper sub section of the allocated or
 * attached buffer.
 */
static DMAResource_Record_t *
DMAResourceLib_Find_Matching_DMAResource(
        const DMAResource_Properties_t * const Properties,
        const DMAResource_AddrPair_t AddrPair)
{
    DMAResourceLib_InUseHandles_Iterator_t it;
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * Rec_p;
    unsigned int Size;

    for (Rec_p = DMAResourceLib_InUseHandles_First(&it);
         Rec_p != NULL;
         Rec_p = DMAResourceLib_InUseHandles_Next(&it))
    {
        if (Rec_p->AllocatorRef == 'R')
        {
            // skip registered buffers when looking for a match,
            // i.e. only consider allocated or attached buffers.
            continue;
        }

        if (Properties->Bank != Rec_p->Props.Bank  ||
            Properties->Size > Rec_p->Props.Size ||
            Properties->Alignment > Rec_p->Props.Alignment)
        {
            // obvious mismatch in properties
            continue;
        }

        Size = Properties->Size;
        Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_HOST);
        if (Pair_p != NULL &&
            DMAResourceLib_IsSubRangeOf(&AddrPair, Size, Pair_p, Rec_p->Props.Size))
        {
            return Rec_p;
        }

        Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_BUS);
        if (Pair_p != NULL &&
            DMAResourceLib_IsSubRangeOf(&AddrPair, Size, Pair_p, Rec_p->Props.Size))
        {
            return Rec_p;
        }
    } // for

    return NULL;
}



/*----------------------------------------------------------------------------
 * DMAResourceLib_Setup_Record
 *
 * Setup most fields of a given DMAResource record, except for the
 * AddrPairs array.
 */
static void
DMAResourceLib_Setup_Record(
        const DMAResource_Properties_t * const Props_p,
        const UMDevXSProxy_SHMem_Handle_t DriverHandle,
        const char AllocatorRef,
        DMAResource_Record_t * const Rec_p)
{
    Rec_p->Magic = DMARES_RECORD_MAGIC;
    Rec_p->Props = *Props_p;
    Rec_p->DriverHandle = DriverHandle;
    Rec_p->AllocatorRef = AllocatorRef;
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_Handle2RecordPtr
 *
 * Return a pointer to the DMAResource record referenced by Handle and check
 * that the record's "magic value" field has the expected value.
 * Return NULL on error.
 */
/* static */ DMAResource_Record_t *
DMAResourceLib_Handle2RecordPtr(
        const DMAResource_Handle_t Handle)
{
    DMAResource_Record_t * Rec_p;

    Rec_p = DMAResource_Handle2RecordPtr(Handle);
    if (NULL != Rec_p)
    {
        if (Rec_p->Magic != DMARES_RECORD_MAGIC)
        {
            LOG_CRIT(
                "DMAResource record: "
                "Bad magic value %08x\n",
                Rec_p->Magic);
            Rec_p = NULL;
        }
    }

    return Rec_p;
}


/*----------------------------------------------------------------------------
 * DMAResourceLib_ChildBusAddress
 *
 * Assuming the given ChildHostAddress is within the host address range
 * for the DMAResource defined by ParentRec_p, return the bus address
 * for the child buffer.
 */
static void *
DMAResourceLib_ChildBusAddress(
        const DMAResource_Record_t * ParentRec_p,
        const void * ChildHostAddr)
{
    ptrdiff_t DevAddrOffset;
    DMAResource_AddrPair_t * Pair_p;

    Pair_p = DMAResourceLib_LookupDomain(ParentRec_p, DMARES_DOMAIN_HOST);
    if (Pair_p == NULL)
        return NULL;

    DevAddrOffset = (uint8_t *)ChildHostAddr - (uint8_t *)Pair_p->Address_p;

    Pair_p = DMAResourceLib_LookupDomain(ParentRec_p, DMARES_DOMAIN_BUS);
    if (Pair_p == NULL)
        return NULL;

    return  ((uint8_t *)Pair_p->Address_p) + DevAddrOffset;
}


/*----------------------------------------------------------------------------
 * DMAResource_Init
 *
 * This function must be used to initialize the DMAResource administration.
 * It must be called before any of the other DMAResource_* functions may be
 * called. It may be called anew only after DMAResource_UnInit has been called.
 *
 * Return Value
 *     true   Initialization successfully, rest of the API may now be used.
 *     false  Initialization failed.
 */
bool
DMAResource_Init(void)
{
    unsigned int MaxHandles = HWPAL_DMA_NRESOURCES;
    int res;

    // already initialized?
    if (HandlesCount != 0)
        return false;

    // this implementation only supports MaxHandles != 0
    if (MaxHandles == 0)
        return false;

    res = UMDevXSProxy_Init();
    if (res < 0)
    {
        LOG_CRIT(
            "DMAResource_Init: "
            "UMDevXSProxy_Init returned %d. "
            "Has the driver been loaded?\n",
            res);

        return false;       // ## RETURN ##
    }

    pthread_mutex_init(&HWPAL_Mutex, NULL);

    Records_p = malloc(MaxHandles * sizeof(DMAResource_Record_t));
    Handles_p = malloc(MaxHandles * sizeof(int));
    FreeHandles.Nrs_p = malloc(MaxHandles * sizeof(int));
    FreeRecords.Nrs_p = malloc(MaxHandles * sizeof(int));

    // if any allocation failed, free the whole lot
    if (Records_p == NULL ||
        Handles_p == NULL ||
        FreeHandles.Nrs_p == NULL ||
        FreeRecords.Nrs_p == NULL)
    {
        if (Records_p)
            free(Records_p);

        if (Handles_p)
            free(Handles_p);

        if (FreeHandles.Nrs_p)
            free(FreeHandles.Nrs_p);

        if (FreeRecords.Nrs_p)
            free(FreeRecords.Nrs_p);

        Records_p = NULL;
        Handles_p = NULL;
        FreeHandles.Nrs_p = NULL;
        FreeRecords.Nrs_p = NULL;

        return false;
    }

    // initialize the record numbers freelist
    // initialize the handle numbers freelist
    // initialize the handles array
    {
        unsigned int i;

        for (i = 0; i < MaxHandles; i++)
        {
            Handles_p[i] = HWPAL_RECNR_DESTROYED;
            FreeHandles.Nrs_p[i] = MaxHandles - 1 - i;
            FreeRecords.Nrs_p[i] = i;
            Records_p[i].Magic = 0;
        }

        FreeHandles.ReadIndex = 0;
        FreeHandles.WriteIndex = 0;

        FreeRecords.ReadIndex = 0;
        FreeRecords.WriteIndex = 0;
    }

    HandlesCount = MaxHandles;

    return true;
}


/*----------------------------------------------------------------------------
 * DMAResource_UnInit
 *
 * This function can be used to uninitialize the DMAResource administration.
 * The caller must make sure that handles will not be used after this function
 * returns.
 * If memory was allocated by DMAResource_Init, this function will free it.
 */
void
DMAResource_UnInit(void)
{
    // exit if not initialized
    if (HandlesCount == 0)
        return;

#ifdef HWPAL_TRACE_DMARESOURCE_LEAKS
    // find resource leaks
    {
        int i;
        bool fFirstPrint = true;

        for (i = 0; i < HandlesCount; i++)
        {
            int RecNr = Handles_p[i];

            if (RecNr >= 0)
            {
                if (fFirstPrint)
                {
                    fFirstPrint = false;
                    Log_FormattedMessage(
                        "DMAResource_UnInit found leaking handles:\n");
                }

                Log_FormattedMessage(
                    "Handle %p => "
                    "Record %d\n",
                    Handles_p + i,
                    RecNr);

                {
                    DMAResource_Record_t * Rec_p = Records_p + RecNr;

                    Log_FormattedMessage(
                        "  BufferSize = %d\n"
                        "  Alignment = %d\n"
                        "  Address = %p (Domain = %d)\n",
                        Rec_p->Props.Size,
                        Rec_p->Props.Alignment,
                        Rec_p->AddrPairs[0].Address_p,
                        Rec_p->AddrPairs[0].Domain);
                }
            } // if
        } // for

        if (fFirstPrint)
            Log_FormattedMessage(
                "DMAResource_UnInit: no leaks found\n");
    }
#endif /* HWPAL_TRACE_DMARESOURCE_LEAKS */

    HandlesCount = 0;

    free(FreeHandles.Nrs_p);
    free(FreeRecords.Nrs_p);
    free(Handles_p);
    free(Records_p);

    FreeHandles.Nrs_p = NULL;
    FreeRecords.Nrs_p = NULL;
    Handles_p = NULL;
    Records_p = NULL;
}


/*----------------------------------------------------------------------------
 * DMAResource_CreateRecord
 *
 * This function can be used to create a record. The function returns a handle
 * for the record. Use DMAResource_Handle2RecordPtr to access the record.
 * Destroy the record when no longer required, see DMAResource_Destroy.
 * This function initializes the record to all zeros.
 *
 * Return Values
 *     Handle for the DMA Resource.
 *     NULL is returned when the creation failed.
 */
DMAResource_Handle_t
DMAResource_CreateRecord(void)
{
    int HandleNr;
    int RecNr = 0;

    // return NULL when not initialized
    if (HandlesCount == 0)
        return NULL;

    ENTER_CRITICAL_SECTION;

    HandleNr = DMAResourceLib_FreeList_Get(&FreeHandles);
    if (HandleNr != -1)
    {
        RecNr = DMAResourceLib_FreeList_Get(&FreeRecords);
        if (RecNr == -1)
        {
            DMAResourceLib_FreeList_Add(&FreeHandles, HandleNr);
            HandleNr = -1;
        }
    }

    LEAVE_CRITICAL_SECTION;

    // return NULL when reservation failed
    if (HandleNr == -1)
        return NULL;

    // initialize the record
    {
        DMAResource_Record_t * Rec_p = Records_p + RecNr;
        memset(Rec_p, 0, sizeof(DMAResource_Record_t));
    }

    // initialize the handle
    Handles_p[HandleNr] = RecNr;

    // fill in the handle position
    return Handles_p + HandleNr;
}


/*----------------------------------------------------------------------------
 * DMAResource_DestroyRecord
 *
 * This function invalidates the handle and the record instance.
 *
 * Handle
 *     A valid handle that was once returned by DMAResource_CreateRecord or
 *     one of the DMA Buffer Management functions (Alloc/Register/Attach).
 *
 * Return Values
 *     None
 */
void
DMAResource_DestroyRecord(
        const DMAResource_Handle_t Handle)
{
    if (DMAResource_IsValidHandle(Handle))
    {
        int * p = (int *)Handle;
        int RecNr = *p;

        if (RecNr >= 0 &&
            RecNr < HandlesCount)
        {
            int HandleNr = p - Handles_p;

            // unset record field with magic value
            Records_p[RecNr].Magic = 0;

            // note handle is no longer value
            *p = HWPAL_RECNR_DESTROYED;

            ENTER_CRITICAL_SECTION;

            // add the HandleNr and RecNr to respective LRU lists
            DMAResourceLib_FreeList_Add(&FreeHandles, HandleNr);
            DMAResourceLib_FreeList_Add(&FreeRecords, RecNr);

            LEAVE_CRITICAL_SECTION;
        }
        else
        {
            LOG_WARN(
                "DMAResource_Destroy: "
                "Handle %p was already destroyed\n",
                Handle);
        }
    }
    else
    {
        LOG_WARN(
            "DMAResource_Destroy: "
            "Invalid handle %p\n",
            Handle);
    }
}


/*----------------------------------------------------------------------------
 * DMAResource_IsValidHandle
 *
 * This function tells whether a handle is valid.
 *
 * Handle
 *     A valid handle that was once returned by DMAResource_CreateRecord or
 *     one of the DMA Buffer Management functions (Alloc/Register/Attach).
 *
 * Return Value
 *     true   The handle is valid
 *     false  The handle is NOT valid
 */
bool
DMAResource_IsValidHandle(
        const DMAResource_Handle_t Handle)
{
    int * p = (int *)Handle;

    if (p < Handles_p ||
        p >= Handles_p + HandlesCount)
    {
        return false;
    }

    // check that the handle has not been destroyed yet
    if (*p < 0 ||
        *p >= HandlesCount)
    {
        return false;
    }

    return true;
}


/*----------------------------------------------------------------------------
 * DMAResource_Handle2RecordPtr
 *
 * This function can be used to get a pointer to the DMA resource record
 * (DMAResource_Record_t) for the provided handle. The pointer is valid until
 * the record and handle are destroyed.
 *
 * Handle
 *     A valid handle that was once returned by DMAResource_CreateRecord or
 *     one of the DMA Buffer Management functions (Alloc/Register/Attach).
 *
 * Return Value
 *     Pointer to the DMAResource_Record_t memory for this handle.
 *     NULL is returned if the handle is invalid.
 */
DMAResource_Record_t *
DMAResource_Handle2RecordPtr(
        const DMAResource_Handle_t Handle)
{
    if (HandlesCount < HWPAL_DMA_NRESOURCES)
    {
        LOG_WARN(
            "DMAResource: "
            "Module not initialized? (HandlesCount=%d)\n",
            HandlesCount);
    }

    if (DMAResource_IsValidHandle(Handle))
    {
        int * p = (int *)Handle;
        int RecNr = *p;

        if (RecNr >= 0 &&
            RecNr < HandlesCount)
        {
            return Records_p + RecNr;           // ## RETURN ##
        }
    }

    return NULL;
}


/*----------------------------------------------------------------------------
 * DMAResource_Read32
 */
uint32_t
DMAResource_Read32(
        const DMAResource_Handle_t Handle,
        const unsigned int WordOffset)
{
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * Rec_p;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Read32: "
            "Invalid handle %p\n",
            Handle);

        return 0;
    }

    if (WordOffset * 4 >= Rec_p->Props.Size)
    {
        LOG_WARN(
            "DMAResource_Read32: "
            "Invalid WordOffset %u for Handle %p\n",
            WordOffset,
            Handle);

        return 0;
    }

    Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_HOST);
    if (Pair_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Read32: "
            "No host address found for Handle %p?\n",
            Handle);

        return 0;
    }

    {
        uint32_t * Address_p = Pair_p->Address_p;
        uint32_t Value = Address_p[WordOffset];

        // swap endianness, if required
        if (Rec_p->fSwapEndianess)
            Value = Device_SwapEndian32(Value);

#ifdef HWPAL_TRACE_DMARESOURCE_READ
        Log_FormattedMessage(
            "DMAResource_Read32:  "
            "(handle %p) "
            "0x%08x = [%u] "
            "(swap=%d)\n",
            Handle,
            Value,
            WordOffset,
            Rec_p->fSwapEndianess);
#endif

        return Value;
    }
}


/*----------------------------------------------------------------------------
 * DMAResource_Write32
 */
void
DMAResource_Write32(
        const DMAResource_Handle_t Handle,
        const unsigned int WordOffset,
        const uint32_t Value)
{
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * Rec_p;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Write32: "
            "Invalid handle %p\n",
            Handle);

        return;
    }

    if (WordOffset * 4 >= Rec_p->Props.Size)
    {
        LOG_WARN(
            "DMAResource_Write32: "
            "Invalid WordOffset %u for Handle %p\n",
            WordOffset,
            Handle);

        return;
    }

    Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_HOST);
    if (Pair_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Write32: "
            "No host address found for Handle %p?\n",
            Handle);

        return;
    }

#ifdef HWPAL_TRACE_DMARESOURCE_WRITE
    Log_FormattedMessage(
        "DMAResource_Write32: "
        "(handle %p) "
        "[%u] = 0x%08x "
        "(swap=%d)\n",
        Handle,
        WordOffset,
        Value,
        Rec_p->fSwapEndianess);
#endif

    {
        uint32_t * Address_p = Pair_p->Address_p;
        uint32_t WriteValue = Value;

        // swap endianness, if required
        if (Rec_p->fSwapEndianess)
            WriteValue = Device_SwapEndian32(WriteValue);

        Address_p[WordOffset] = WriteValue;
    }
}


/*----------------------------------------------------------------------------
 * DMAResource_Read32Array
 */
void
DMAResource_Read32Array(
        const DMAResource_Handle_t Handle,
        const unsigned int StartWordOffset,
        const unsigned int WordCount,
        uint32_t * Values_p)
{
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * Rec_p;

    if (WordCount == 0)
        return;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Read32Array: "
            "Invalid handle %p\n",
            Handle);
        return;
    }

    if ((StartWordOffset + WordCount - 1) * 4 >= Rec_p->Props.Size)
    {
        LOG_WARN(
            "DMAResource_Read32Array: "
            "Invalid range: %u - %u\n",
            StartWordOffset,
            StartWordOffset + WordCount - 1);
        return;
    }

    Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_HOST);
    if (Pair_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Read32Array: "
            "No host address found for Handle %p?\n",
            Handle);

        return;
    }

    {
        uint32_t * Address_p = Pair_p->Address_p;
        unsigned int i;

        for (i = 0; i < WordCount; i++)
        {
            uint32_t Value = Address_p[StartWordOffset + i];

            // swap endianness, if required
            if (Rec_p->fSwapEndianess)
                Value = Device_SwapEndian32(Value);

            Values_p[i] = Value;
        } // for
    }

#ifdef HWPAL_TRACE_DMARESOURCE_READ
    {
        uint32_t * Address_p = Rec_p->Host.Address_p;
        if (Values_p == Address_p + StartWordOffset)
        {
            Log_FormattedMessage(
                "DMAResource_Read32Array: "
                "(handle %p) "
                "[%u..%u] IN-PLACE "
                "(swap=%d)\n",
                Handle,
                WordOffset,
                WordOffset + WordCount - 1,
                Rec_p->fSwapEndianess);
        }
        else
        {
            Log_FormattedMessage(
                "DMAResource_Read32Array: "
                "(handle %p) "
                "[%u..%u] "
                "(swap=%d)\n",
                Handle,
                StartWordOffset,
                StartWordOffset + WordCount - 1,
                Rec_p->fSwapEndianess);
        }
    }
#endif
}


/*----------------------------------------------------------------------------
 * DMAResource_Write32Array
 */
void
DMAResource_Write32Array(
        const DMAResource_Handle_t Handle,
        const unsigned int StartWordOffset,
        const unsigned int WordCount,
        const uint32_t * Values_p)
{
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * Rec_p;

    if (WordCount == 0)
        return;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Write32Array: "
            "Invalid handle %p\n",
            Handle);
        return;
    }

    if ((StartWordOffset + WordCount - 1) * 4 >= Rec_p->Props.Size)
    {
        LOG_WARN(
            "DMAResource_Write32Array: "
            "Invalid range: %u - %u\n",
            StartWordOffset,
            StartWordOffset + WordCount - 1);
        return;
    }

    Pair_p = DMAResourceLib_LookupDomain(Rec_p, DMARES_DOMAIN_HOST);
    if (Pair_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Write32Array: "
            "No host address found for Handle %p?\n",
            Handle);

        return;
    }

    {
        uint32_t * Address_p = Pair_p->Address_p;
        unsigned int i;

        for (i = 0; i < WordCount; i++)
        {
            uint32_t Value = Values_p[i];

            // swap endianness, if required
            if (Rec_p->fSwapEndianess)
                Value = Device_SwapEndian32(Value);

            Address_p[StartWordOffset + i] = Value;
        } // for
    }

#ifdef HWPAL_TRACE_DMARESOURCE_WRITE
    {
        uint32_t * Address_p = Rec_p->Host.Address_p;
        if (Values_p == Address_p + StartWordOffset)
        {
            Log_FormattedMessage(
                "DMAResource_Write32Array: "
                "(handle %p) "
                "[%u..%u] IN-PLACE "
                "(swap=%d)\n",
                Handle,
                StartWordOffset,
                StartWordOffset + WordCount - 1,
                Rec_p->fSwapEndianess);
        }
        else
        {
            Log_FormattedMessage(
                "DMAResource_Write32Array: "
                "(handle %p) "
                "[%u..%u] "
                "(swap=%d)\n",
                Handle,
                StartWordOffset,
                StartWordOffset + WordCount - 1,
                Rec_p->fSwapEndianess);
        }
    }
#endif /* HWPAL_TRACE_DMARESOURCE_WRITE */
}


/*----------------------------------------------------------------------------
 * DMAResource_PreDMA
 */
void
DMAResource_PreDMA(
        const DMAResource_Handle_t Handle,
        const unsigned int ByteOffset,
        const unsigned int ByteCount)
{
    DMAResource_Record_t *Rec_p;
    unsigned int NBytes = ByteCount;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_PreDMA: "
            "Invalid handle %p\n",
            Handle);
        return;
    }

    if (NBytes == 0)
    {
        NBytes = Rec_p->Props.Size;
    }

    if ((ByteOffset >= Rec_p->Props.Size) ||
        (NBytes > Rec_p->Props.Size) ||
        (ByteOffset + NBytes > Rec_p->Props.Size))
    {
        LOG_WARN(
            "DMAResource_PreDMA: "
            "Invalid range 0x%08x-0x%08x (not in 0x0-0x%08x)\n",
            ByteOffset,
            ByteOffset + NBytes,
            Rec_p->Props.Size);
        return;
    }

    if (Rec_p->Props.fCached)
    {
        // Send "cache clean" request to driver via driver proxy
        UMDevXSProxy_SHMem_Commit(
                Rec_p->DriverHandle,
                ByteOffset,
                NBytes);
    }
}


/*----------------------------------------------------------------------------
 * DMAResource_PostDMA
 */
void
DMAResource_PostDMA(
        const DMAResource_Handle_t Handle,
        const unsigned int ByteOffset,
        const unsigned int ByteCount)
{
    DMAResource_Record_t *Rec_p;
    unsigned int NBytes = ByteCount;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_PostDMA: "
            "Invalid handle %p\n",
            Handle);
        return;
    }

    if (NBytes == 0)
    {
        NBytes = Rec_p->Props.Size;
    }

    if ((ByteOffset >= Rec_p->Props.Size) ||
        (NBytes > Rec_p->Props.Size) ||
        (ByteOffset + NBytes > Rec_p->Props.Size))
    {
        LOG_WARN(
            "DMAResource_PostDMA: "
            "Invalid range 0x%08x-0x%08x (not in 0x0-0x%08x)\n",
            ByteOffset,
            ByteOffset + NBytes,
            Rec_p->Props.Size);
        return;
    }

    if (Rec_p->Props.fCached)
    {
        // Send "cache invalidate" request to driver via driver proxy
        UMDevXSProxy_SHMem_Refresh(
                Rec_p->DriverHandle,
                ByteOffset,
                NBytes);
    }
}


/*----------------------------------------------------------------------------
 * DMAResource_Alloc
 */
int
DMAResource_Alloc(
        const DMAResource_Properties_t RequestedProperties,
        DMAResource_AddrPair_t * const AddrPair_p,
        DMAResource_Handle_t * const Handle_p)
{
    DMAResource_Properties_t ActualProperties = {0};
    UMDevXSProxy_SHMem_Handle_t BufHandle;
    UMDevXSProxy_SHMem_DevAddr_t DevAddr;
    UMDevXSProxy_SHMem_BufPtr_t BufPtr;
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Handle_t Handle;
    DMAResource_Record_t * Rec_p = NULL;
    unsigned int ActualSize;
    int rv;

    if ((NULL == AddrPair_p) || (NULL == Handle_p))
    {
        return -1;
    }

    if (!DMAResourceLib_IsSaneInput(NULL, &RequestedProperties))
    {
        return -1;
    }

    // Allocate record
    Handle = DMAResource_CreateRecord();
    if (NULL != Handle)
    {
        Rec_p = DMAResource_Handle2RecordPtr(Handle);
    }
    if ((NULL == Handle) || (NULL == Rec_p))
    {
        return -1;
    }

    // Request driver (via the driver proxy) to alloc a buffer
    // with the requested properties.
    rv = UMDevXSProxy_SHMem_Alloc(
             RequestedProperties.Size,
             RequestedProperties.Bank,
             RequestedProperties.Alignment,
             &BufHandle,
             &BufPtr,
             &DevAddr,
             &ActualSize);

    if (rv != 0)
    {
        LOG_WARN(
            "DMAResource_Alloc: "
            "Driver returned with error: %d"
            " (Size/Bank/Align=0x%x/%d/0x%x)\n",
            rv,
            RequestedProperties.Size,
            RequestedProperties.Bank,
            RequestedProperties.Alignment);
        DMAResource_DestroyRecord(Handle);
        return -1;
    }

    ActualProperties.Alignment = RequestedProperties.Alignment;
    ActualProperties.Bank = RequestedProperties.Bank;
#ifndef HWPAL_ARCH_COHERENT
    ActualProperties.fCached = true;
#endif
    // Hide the actual size from the caller, since (s)he is not
    // supposed to access/use any space beyond what was requested
    ActualProperties.Size = RequestedProperties.Size;

    DMAResourceLib_Setup_Record(
            &ActualProperties,
            BufHandle,
            'A',
            Rec_p);

    // put the bus address first, presumably being the most
    // frequently looked-up domain.
    Pair_p = Rec_p->AddrPairs;
    Pair_p->Address_p = DevAddr.p;
    Pair_p->Domain = DMARES_DOMAIN_BUS;
    ++Pair_p;
    Pair_p->Address_p = BufPtr.p;
    Pair_p->Domain = DMARES_DOMAIN_HOST;

    // return results
    *AddrPair_p = *Pair_p;
    *Handle_p = Handle;
    return 0;
}


/*----------------------------------------------------------------------------
 * DMAResource_CheckAndRegister
 */
int
DMAResource_CheckAndRegister(
        const DMAResource_Properties_t ActualProperties,
        const DMAResource_AddrPair_t AddrPair,
        const char AllocatorRef,
        DMAResource_Handle_t * const Handle_p)
{
    UMDevXSProxy_SHMem_Handle_t DriverHandle;
    UMDevXSProxy_SHMem_BufPtr_t BufPtr;
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Record_t * ParentRec_p;
    DMAResource_Record_t * Rec_p;
    DMAResource_Handle_t Handle;
    int rv;

    if (NULL == Handle_p)
    {
        return -1;
    }

    if (!DMAResourceLib_IsSaneInput(&AddrPair, &ActualProperties))
    {
        return -2;
    }

    // This implementation only accepts an AddrPair with an address
    // from domain DMARES_DOMAIN_HOST and 'R' as AllocatorRef.
    // Also, the given address and properties must match an already
    // allocated or attached DMA buffer so that the buffer is known
    // to be DMA-safe.

    if (AddrPair.Domain != DMARES_DOMAIN_HOST)
    {
        LOG_INFO(
            "DMAResource_Register: "
            "Unsupported domain: %u\n",
            AddrPair.Domain);
        return -3;
    }

    if (AllocatorRef != 'R')
    {
        LOG_INFO(
            "DMAResource_Register: "
            "Unsupported AllocatorRef: %c\n",
            AllocatorRef);

        return -4;
    }

    ParentRec_p = DMAResourceLib_Find_Matching_DMAResource(
                        &ActualProperties,
                        AddrPair);

    if (ParentRec_p == NULL)
    {
        LOG_INFO(
            "DMAResource_Register: "
            "Rejecting register request\n");

        return -6;
    }

    // ask kernel driver to register the buffer and obtain a driver handle
    BufPtr.p = AddrPair.Address_p;
    rv = UMDevXSProxy_SHMem_Register(
                ActualProperties.Size,
                BufPtr,
                ParentRec_p->DriverHandle,
                &DriverHandle);
    if (rv != 0)
    {
        LOG_WARN(
            "DMAResource_Register: "
            "Driver register request failed (%d)\n",
            rv);
        return -7;
    }

    // allocate record -> Handle & Rec_p
    Handle = DMAResource_CreateRecord();
    if (Handle == NULL)
        return -8;

    Rec_p = DMAResource_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
        return -9;

    DMAResourceLib_Setup_Record(
            &ActualProperties,
            DriverHandle,
            'R',
            Rec_p);

    Pair_p = Rec_p->AddrPairs;
    Pair_p->Address_p = DMAResourceLib_ChildBusAddress(
                                            ParentRec_p,
                                            AddrPair.Address_p);
    Pair_p->Domain = DMARES_DOMAIN_BUS;
    if (Pair_p->Address_p != NULL)
        Pair_p++;
    Pair_p->Address_p = AddrPair.Address_p;
    Pair_p->Domain = DMARES_DOMAIN_HOST;

    *Handle_p = Handle;
    return 0;
}


/*----------------------------------------------------------------------------
 * DMAResource_Attach
 */
int
DMAResource_Attach(
        const DMAResource_Properties_t ActualProperties,
        const DMAResource_AddrPair_t AddrPair,
        DMAResource_Handle_t * const Handle_p)
{
    UMDevXSProxy_SHMem_Handle_t DriverHandle = {0};
    UMDevXSProxy_SHMem_DevAddr_t DevAddr;
    UMDevXSProxy_SHMem_BufPtr_t BufPtr;
    DMAResource_AddrPair_t BusAddrPair;
    DMAResource_AddrPair_t * Pair_p;
    DMAResource_Handle_t Handle;
    DMAResource_Record_t *Rec_p;
    unsigned int ActualSize;
    int rv;

    if (Handle_p == NULL)
    {
        return -1;
    }
    *Handle_p = NULL;

    if (!DMAResourceLib_IsSaneInput(&AddrPair, &ActualProperties))
    {
        return -1;
    }

    // allocate record -> Handle & Rec_p
    Handle = DMAResource_CreateRecord();
    if (Handle == NULL)
        return -1;

    Rec_p = DMAResource_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
        return -1;

    // setup DMAResource record, note that so far we don't have a
    // valid DriverHandle yet.
    DMAResourceLib_Setup_Record(
            &ActualProperties,
            DriverHandle,
            'T',
            Rec_p);

    // setup DMAResource record with supplied address info so that
    // subsequent Translate call can (hopefully) use it to derive
    // the buffer's bus address.
    Pair_p = Rec_p->AddrPairs;
    Pair_p->Address_p = AddrPair.Address_p;
    Pair_p->Domain = AddrPair.Domain;

    rv = DMAResource_Translate(
                Handle,
                DMARES_DOMAIN_BUS,
                &BusAddrPair);
    if (rv != 0)
    {
        DMAResource_DestroyRecord(Handle);
        return -1;
    }
    ++Pair_p;
    *Pair_p = BusAddrPair;

    // request the driver to map the buffer described by Size, Bank
    // and its Device (aka physical) address in user space.
    DevAddr.p = BusAddrPair.Address_p;
    rv = UMDevXSProxy_SHMem_Attach(
                DevAddr,
                ActualProperties.Size,
                ActualProperties.Bank,
                &DriverHandle,
                &BufPtr,
                &ActualSize);

    if (rv < 0)
    {
        LOG_WARN(
            "DMAResource_Attach: "
            "Driver returned with error: %u\n",
            rv);
        DMAResource_DestroyRecord(Handle);
        return -1;
    }

    // update the DriverHandle in the record
    Rec_p->DriverHandle = DriverHandle;

    ++Pair_p;
    Pair_p->Address_p = BufPtr.p;
    Pair_p->Domain = DMARES_DOMAIN_HOST;

    *Handle_p = Handle;
    return 0;
}


/*----------------------------------------------------------------------------
 * DMAResource_Release
 */
int
DMAResource_Release(
        const DMAResource_Handle_t Handle)
{
    DMAResource_Record_t * Rec_p;
    int rv = 0;

    Rec_p = DMAResource_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_Release: "
            "Invalid handle %p\n",
            Handle);
        return -1;
    }

    if (Rec_p->AllocatorRef != 'T')
    {
        // Let the kernel driver free an _Alloc'd or _Registered DMA resource.
        // For an _Alloc'd resource, unmap and free the memory and forget about
        // the resource. For a _Registered resource, only forget about it.
        rv = UMDevXSProxy_SHMem_Free(Rec_p->DriverHandle);
    }
    else
    {
        // Let the kernel driver free an _Attached DMA resource, i.e. unmap
        // the memory and forget about the resource.
        rv = UMDevXSProxy_SHMem_Detach(Rec_p->DriverHandle);
    }

    if (rv < 0)
    {
        LOG_WARN(
            "DMAResource_Release: "
            "Driver returned with error: %u\n",
            rv);
        // no return here, e.g. destroy/free record anyway
    }
    // free administration resources
    DMAResource_DestroyRecord(Handle);

    return rv;
}


/*----------------------------------------------------------------------------
 * DMAResource_SwapEndianess_Set
 */
int
DMAResource_SwapEndianess_Set(
        const DMAResource_Handle_t Handle,
        const bool fSwapEndianess)
{
    DMAResource_Record_t * Rec_p;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_SwapEndianess_Set: "
            "Invalid handle %p\n",
            Handle);
        return -1;
    }

    Rec_p->fSwapEndianess = fSwapEndianess;
    return 0;
}


/*----------------------------------------------------------------------------
 * DMAResource_SwapEndianess_Get
 */
int
DMAResource_SwapEndianess_Get(
        const DMAResource_Handle_t Handle)
{
    DMAResource_Record_t * Rec_p;

    Rec_p = DMAResourceLib_Handle2RecordPtr(Handle);
    if (Rec_p == NULL)
    {
        LOG_WARN(
            "DMAResource_SwapEndianess_Get: "
            "Invalid handle %p\n",
            Handle);
        return -1;
    }

    if (Rec_p->fSwapEndianess)
    {
        return 1;
    }
    return 0;
}

/*
 * The DMAResource address translation API is implemented in a separate file
 */

/* end of file hwpal_dmares_umdevxs.c */
