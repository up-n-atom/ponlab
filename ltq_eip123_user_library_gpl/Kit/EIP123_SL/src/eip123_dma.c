/* eip123_dma.c
 *
 * Driver for the EIP-123 HW2.x Crypto Module.
 *
 * This file contains the functionality to create a DMA Linked List, verify
 * such a list and to convert the copy the first fragment to the command
 * token.
 */

/*****************************************************************************
* Copyright (c) 2007-2013 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_eip123.h"           // configuration

#include "basic_defs.h"         // uint8_t, IDENTIFIER_NOT_USED, etc.
#include "clib.h"               // memset, memcpy
#include "dmares_types.h"       // HW access DMA API
#include "dmares_rw.h"          // HW access DMA API

#include "eip123_dma.h"         // the API to implement

#define EIP123_DMA_MAXLEN_DATA         0x001FFFFF // 2 MB - 1 byte

typedef struct
{
    uint32_t DMAC_SrcAddr;
    uint32_t DMAC_DestAddr;
    uint32_t DMAC_LLIAddr;
    uint32_t DMAC_DmaLength;
} EIP123_DMA_Descriptor_t;


// Error codes for EIP123_DescriptorChain_Fragment() function
enum
{
    EIP123_DC_FRAGMENT_SUCCESS,
    EIP123_DC_FRAGMENT_INVALID_ARGUMENT,
    EIP123_DC_FRAGMENT_TOO_FEW_FRAGMENTS_ERROR,
    EIP123_DC_FRAGMENT_ALIGNMENT_ERROR
};


/*----------------------------------------------------------------------------
 * EIP123Lib_RoundUp_DMALen_to_BusWidth
 *
 * Returns true when the validation failed.
 */
static inline uint32_t
EIP123Lib_RoundUp_DMALen_to_BusWidth(
        const uint32_t DMALEN)
{
    // we currently only support a 32bit bus
    // so bits 0 and 1 must be zero
    if ((DMALEN & 3) > 0)
        return DMALEN - (DMALEN & 3) + 4;
    else
        return DMALEN;
}


/*----------------------------------------------------------------------------
 * EIP123_Get_DC_DMAResource_Size
 */
uint32_t
EIP123_Get_DC_DMAResource_Size(void)
{
    // -1 because first fragment is in mailbox
    return ((EIP123_MAX_PHYSICAL_FRAGMENTS - 1) * EIP123_FRAGMENT_SIZE * sizeof(uint32_t));
}


/*----------------------------------------------------------------------------
 * EIP123_DescriptorChain_Fragment
 *
 * This function will attempt to fragment the provided fragments into smaller
 * ones to satisfy the requirement of the EIP-122 DMA engine that the DMA
 * blocks should not be larger than 2MB-1 bytes
 *
 * Input:
 *
 * FragmentCount
 *      Number of original fragments
 *
 * Fragments_p
 *      List (linear array) of original fragments
 *
 * AlgorithmicBlockSize
 *      Algorithm specific data block size, required for fragment size
 *      calculation, used for non-last fragment size alignment check,
 *      alignment of last fragment is not checked
 *
 * Input/Output:
 *
 * FragmentListCount_p
 *      Number of elements available in the provided fragment list
 *      where new fragments can be stored after fragmentation
 *      On return contains the number of copied fragments
 *
 * Output:
 *
 *  FragmentList_p
 *      List for storing new fragments after fragmentation
 *
 * Returns:
 *      EIP123_DC_FRAGMENT_SUCCESS
 *      Fragmentation is successful or provided fragments (Fragments_p)
 *      successfully copied to the new fragment list (FragmentList_p)
 *
 *      EIP123_DC_FRAGMENT_TOO_FEW_FRAGMENTS_ERROR
 *      Not enough fragments provided for fragmentation
 *
 *      EIP123_DC_FRAGMENT_INVALID_ARGUMENT
 *      One of provided parameters is invalid
 *
 *      EIP123_DC_FRAGMENT_ALIGNMENT_ERROR
 *      Non-last fragment length is not aligned for
 *      AlgorithmicBlockSize, see above
 *
 */
static int
EIP123_DescriptorChain_Fragment(
        const unsigned int FragmentCount,
        const EIP123_Fragment_t * const Fragments_p,
        const unsigned int AlgorithmicBlockSize,
        unsigned int * FragmentListCount_p,
        EIP123_Fragment_t * FragmentList_p)
{
    uint32_t i, MaxFragmentSize, MaxNonLastFragmentSize, MaxLastFragmentSize;
    uint32_t NewFragCount = 1;
    const EIP123_Fragment_t * Frag_p = Fragments_p;

    MaxFragmentSize = MaxNonLastFragmentSize =
            EIP123Lib_RoundUp_DMALen_to_BusWidth(EIP123_DMA_MAXLEN_DATA) -
            AlgorithmicBlockSize;

    MaxLastFragmentSize = EIP123_DMA_MAXLEN_DATA;

    if (MaxNonLastFragmentSize == 0 ||
        MaxNonLastFragmentSize % AlgorithmicBlockSize)
    {
        *FragmentListCount_p = NewFragCount - 1;
        return EIP123_DC_FRAGMENT_ALIGNMENT_ERROR;
    }

    // Start fragmentation
    for(i = 0; i < FragmentCount; i++)
    {
        // Fragment with length=0 should be rejected
        // avoids writing zero-length dma descriptors
        if (Frag_p->Length == 0)
        {
            *FragmentListCount_p = NewFragCount - 1;
            return EIP123_DC_FRAGMENT_INVALID_ARGUMENT;
        }

        if (i == FragmentCount - 1) // Check if Last Fragment
        {   // This is Last Fragment
            MaxFragmentSize = MaxLastFragmentSize;
        }

        // Check if this fragment requires further fragmentation
        if (Frag_p->Length > MaxFragmentSize)
        {
            // Fragmentation is required
            uint32_t count = 0, j = 0;

            MaxFragmentSize = MaxNonLastFragmentSize;

            // Split the fragment
            do
            {
                // Check if required fragments are available
                if (NewFragCount > (*FragmentListCount_p))
                {
                    // Fragmentation is required but not possible
                    *FragmentListCount_p = NewFragCount - 1;
                    return EIP123_DC_FRAGMENT_TOO_FEW_FRAGMENTS_ERROR;
                }

                // This fragment start address
                FragmentList_p->StartAddress = Frag_p->StartAddress +
                                                j * MaxFragmentSize;
                count += MaxFragmentSize;

                // This fragment size
                if (count >= Frag_p->Length) // Last fragment
                {
                    // This fragment size may be less than MaxFragmentSize
                    FragmentList_p->Length =
                            MaxFragmentSize - (count - Frag_p->Length);
                }
                else // Not last fragment
                {
                    // This fragment size is always MaxFragmentSize long
                    FragmentList_p->Length = MaxFragmentSize;

                    // Go to next new fragment
                    NewFragCount++; FragmentList_p++;
                }

                j++;
            }
            while(count < Frag_p->Length);
        }
        else // No fragmentation is required
        {
            // Non-Last Fragment Size must be a multiple of
            // AlgorithmicBlockSize
            if (i != (FragmentCount - 1) &&
                Frag_p->Length % AlgorithmicBlockSize)
            {
                *FragmentListCount_p = NewFragCount - 1;
                return EIP123_DC_FRAGMENT_ALIGNMENT_ERROR;
            }

            // Check if required fragments are available
            if (NewFragCount > (*FragmentListCount_p))
            {
                // Fragmentation is required but not possible
                *FragmentListCount_p = NewFragCount - 1;
                return EIP123_DC_FRAGMENT_TOO_FEW_FRAGMENTS_ERROR;
            }

            // No, fragmentation is not required, just copy the fragment
            FragmentList_p->StartAddress = Frag_p->StartAddress;
            FragmentList_p->Length = Frag_p->Length;
        }

        Frag_p++;                         // Go to next original fragment
        NewFragCount++; FragmentList_p++; // Go to next new fragment
    }

    // Fragmentation is successful!
    *FragmentListCount_p = NewFragCount - 1;
    return EIP123_DC_FRAGMENT_SUCCESS;
}


/*----------------------------------------------------------------------------
 * DescriptorChain_Populate
 *
 * Set fIsInput to 'true' for Input Descriptors.
 * Set fIsInput to 'false' for Output and State Descriptors.
 * Set DMAHandle to the obtained DMA resource handle,
 * otherwise set to NULL if DMAHandle is not used when only one descriptor
 * fragment is provided
 * Set DescriptorPhysAddr to a physical address of the 2nd
 * fragment in descriptor chain (DMA resource) or 0 if not present
 * Set AlgorithmicBlockSize according to the used algorithm
 * Set TokenIDPhysAddr to a physical address of the TokenID
 */
EIP123_Status_t
EIP123_DescriptorChain_Populate(
        EIP123_DescriptorChain_t * const Descriptor_p,
        const DMAResource_Handle_t DMAHandle,
        const uint32_t DescriptorPhysAddr,
        const bool fIsInput,
        const unsigned int FragmentCount,
        const EIP123_Fragment_t * const Fragments_p,
        const unsigned int AlgorithmicBlockSize,
        const uint32_t TokenIDPhysAddr)
{
    const EIP123_Fragment_t * Frag_p;
    EIP123_Fragment_t FragmentList[EIP123_MAX_PHYSICAL_FRAGMENTS];
    unsigned int FragmentListCount = 0;

    // Sanity checks for input parameters
    if (Fragments_p == NULL ||
        Descriptor_p == NULL ||
        FragmentCount == 0 ||
        AlgorithmicBlockSize == 0 ||
        FragmentCount > EIP123_MAX_PHYSICAL_FRAGMENTS)
    {
        return EIP123_STATUS_INVALID_ARGUMENT;
    }

    // Check if one fragment should be used for Token ID,
    // result is a number of fragments available for fragmentation
    FragmentListCount =
            (TokenIDPhysAddr == 0) ? EIP123_MAX_PHYSICAL_FRAGMENTS :
                                     (EIP123_MAX_PHYSICAL_FRAGMENTS - 1);

    // Check if all provided fragments can be processed
    if (FragmentCount > FragmentListCount)
    {
        // Too many fragments provided (possibly including Token ID)
        return EIP123_STATUS_INVALID_ARGUMENT;
    }

    // TokenIDPhysAddr>0 should be rejected for fIsInput==true
    if (TokenIDPhysAddr != 0 && fIsInput)
    {
        // Avoids writing Token ID output-dma descriptor
        // at end of input descriptor chain
        return EIP123_STATUS_INVALID_ARGUMENT;
    }

    switch ( EIP123_DescriptorChain_Fragment(FragmentCount,
                                  Fragments_p,
                                  AlgorithmicBlockSize,
                                  &FragmentListCount,
                                  FragmentList) )
    {
        case EIP123_DC_FRAGMENT_SUCCESS:
            // Fragmentation is done
            Frag_p = FragmentList;
            break;

        default:
            // Fragmentation failed
            return EIP123_STATUS_INVALID_ARGUMENT;
    }

    // Other post-fragmentation sanity checks for input parameters
    if (DMAHandle == NULL &&
        (FragmentListCount > 1 ||
        (FragmentListCount == 1 && TokenIDPhysAddr != 0)))
    {
        return EIP123_STATUS_INVALID_ARGUMENT;
    }

    // Sanity checks for input parameters passed, can start the function
    {
        uint32_t PhysAddr = DescriptorPhysAddr;
        EIP123_DMA_Descriptor_t * const p =
            (EIP123_DMA_Descriptor_t *)Descriptor_p->not_exposed;

        Descriptor_p->EntriesUsed = 0;
        Descriptor_p->fHasWriteTokenID = false;
        Descriptor_p->DMAHandle = NULL;

        // Start with the first fragment
        // First fragment is copied to buffer descriptor
        p->DMAC_DmaLength = EIP123Lib_RoundUp_DMALen_to_BusWidth(Frag_p->Length);
        p->DMAC_LLIAddr = PhysAddr;

        if (fIsInput)
        {
            // this is for channel 0 that transfers data into the EIP123
            // set up a gather descriptor
            p->DMAC_SrcAddr = Frag_p->StartAddress;
            p->DMAC_DestAddr = 0;
        }
        else
        {
            // this is for channel 1 that transfers data from the EIP123
            // set up a scatter descriptor
            p->DMAC_DestAddr = Frag_p->StartAddress;
            p->DMAC_SrcAddr = 0;
        }

        Descriptor_p->EntriesUsed++;

        // Go to the next fragment or terminate the fragment chain
        if (Descriptor_p->EntriesUsed == FragmentListCount &&
            TokenIDPhysAddr == 0)
        {
            // This is the last fragment in the chain
            p->DMAC_LLIAddr = 0;       // next pointer = NULL
        }
        else
        {
            uint32_t n;

            Descriptor_p->DMAHandle = DMAHandle;

            // Continue with the fragment chain
            while(Descriptor_p->EntriesUsed < FragmentListCount)
            {
                // Calculate offset in DMA resource,
                // First fragment is copied to buffer descriptor
                // Second and following fragments are copied to DMA resource
                n = (Descriptor_p->EntriesUsed - 1) * EIP123_FRAGMENT_SIZE;

                Frag_p++;

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + (fIsInput ? 0 : 1),  // Offset in words
                        Frag_p->StartAddress);

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + (fIsInput ? 1 : 0),  // Offset in words
                        0);

                // Check if we are filling in the last fragment
                if ((Descriptor_p->EntriesUsed + 1) ==
                    (unsigned char)FragmentListCount &&
                   TokenIDPhysAddr == 0)
                {
                    // Last fragment, no TokenID is requested,
                    // terminate descriptor chain
                    DMAResource_Write32(Descriptor_p->DMAHandle,
                                        n + 2,  // Offset in words
                                        0); // Physical address 0,
                                            // no next pointer
                }
                else
                {
                    PhysAddr += (sizeof(uint32_t) * EIP123_FRAGMENT_SIZE);

                    DMAResource_Write32(Descriptor_p->DMAHandle,
                                        n + 2,  // Offset in words
                                        PhysAddr);
                }

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + 3,  // Offset in words
                        EIP123Lib_RoundUp_DMALen_to_BusWidth(Frag_p->Length));

                Descriptor_p->EntriesUsed++;
            }

            // Finish off with the fragment for the TokenID if requested
            if (TokenIDPhysAddr != 0)
            {
                // TokenID is copied to DMA resource
                n = (Descriptor_p->EntriesUsed - 1) * EIP123_FRAGMENT_SIZE;

                // This is for channel 1 that transfers data from the EIP123
                // Set up a scatter fragment
                // Set dma descriptor srcaddr=0 for tokenid
                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + 0,  // Offset in words
                        0); // TokenID fragment phys. address

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + 1,  // Offset in words
                        TokenIDPhysAddr); // TokenID fragment phys. address

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + 2,  // Offset in words
                        0); // Physical address 0, no next pointer

                DMAResource_Write32(
                        Descriptor_p->DMAHandle,
                        n + 3,  // Offset in words
                        EIP123_TOKEN_ID_SIZE);

                Descriptor_p->EntriesUsed++;
                Descriptor_p->fHasWriteTokenID = true;
            }
        }
    }

    return EIP123_STATUS_SUCCESS;
}


/*----------------------------------------------------------------------------
 * EIP123_DescriptorChain_GetFirst
 */
void
EIP123_DescriptorChain_GetFirst(
        const EIP123_DescriptorChain_t * const Descriptor_p,
        uint32_t * const SrcAddr_p,
        uint32_t * const DstAddr_p,
        uint32_t * const LLIAddr_p,
        uint32_t * const Length_p)
{
    const EIP123_DMA_Descriptor_t * p =
            (EIP123_DMA_Descriptor_t *)Descriptor_p->not_exposed;

    if (SrcAddr_p)
        *SrcAddr_p = p->DMAC_SrcAddr;

    if (DstAddr_p)
        *DstAddr_p = p->DMAC_DestAddr;

    if (LLIAddr_p)
        *LLIAddr_p = p->DMAC_LLIAddr;

    if (Length_p)
        *Length_p = p->DMAC_DmaLength;
}


/* end of file eip123_dma.c */
