/* eip123_dma.h
 *
 * EIP-123 HW2.x Crypto Module Driver Library API
 *
 * DMA support functions and knowledge of DMA fields in the tokens.
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

#ifndef INCLUDE_GUARD_EIP123_DMA_H
#define INCLUDE_GUARD_EIP123_DMA_H

#include "basic_defs.h"         // uint32_t, bool, inline, etc.
#include "dmares_types.h"       // DMAResource_Handle_t

typedef enum
{
    EIP123_STATUS_SUCCESS = 0,
    EIP123_STATUS_INVALID_ARGUMENT,
} EIP123_Status_t;

// data block must be an integer multiple of a work block size (in bytes)
#define EIP123_ALGOBLOCKSIZE_HASH       64
#define EIP123_ALGOBLOCKSIZE_AES        16
#define EIP123_ALGOBLOCKSIZE_CAMELLIA   16
#define EIP123_ALGOBLOCKSIZE_DES        8
#define EIP123_ALGOBLOCKSIZE_MULTI2     8
#define EIP123_ALGOBLOCKSIZE_C2         8
#define EIP123_ALGOBLOCKSIZE_ARC4       4
#define EIP123_ALGOBLOCKSIZE_NOP        4

#define EIP123_TOKEN_ID_SIZE           4          // In bytes
#define EIP123_ARC4_STATE_BUF_SIZE     256        // In bytes
#define EIP123_LIMIT_RANDOM_GENERATE   65535      // In bytes
#define EIP123_FRAGMENT_SIZE           4          // In 32-bit words

typedef struct
{
    unsigned char EntriesUsed;
    bool fHasWriteTokenID;
    DMAResource_Handle_t DMAHandle;
    uint32_t not_exposed[EIP123_FRAGMENT_SIZE];
} EIP123_DescriptorChain_t;

typedef struct
{
    uint32_t StartAddress;
    uint32_t Length;        // in bytes
} EIP123_Fragment_t;


/*----------------------------------------------------------------------------
 * EIP123_Get_DC_DMAResource_Size
 *
 * Helper function for DMA resource size calculation in bytes for descriptor
 * chains.
 */
uint32_t
EIP123_Get_DC_DMAResource_Size(void);


/*----------------------------------------------------------------------------
 * EIP123_DescriptorChain_Populate
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
        const uint32_t TokenIDPhysAddr);


/*----------------------------------------------------------------------------
 * EIP123_DescriptorChain_GetFirst
 *
 * This function returns the four elements of the first DMA descriptor chain.
 * This information must be put in the Input Token. This function can be used
 * after EIP123_DescriptorChain_Populate().
 */
void
EIP123_DescriptorChain_GetFirst(
        const EIP123_DescriptorChain_t * const Descriptor_p,
        uint32_t * const SrcAddr_p,
        uint32_t * const DstAddr_p,
        uint32_t * const LLIAddr_p,
        uint32_t * const Length_p);

#endif /* Include Guard */

/* end of file eip123_dma.h */
