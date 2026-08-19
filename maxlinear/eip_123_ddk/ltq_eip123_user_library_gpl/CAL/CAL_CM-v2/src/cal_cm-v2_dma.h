/* cal_cm-v2_dma.h
 *
 * CAL module internal interfaces and definitions.
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

#ifndef INCLUDE_GUARD_CAL_CM_DMA_H
#define INCLUDE_GUARD_CAL_CM_DMA_H

#include "eip123_dma.h"
#include "cm_tokens_common.h"       // CMTokens_*

#include "sfzcryptoapi.h"           // SfzCryptoStatus

typedef struct
{
    EIP123_DescriptorChain_t InDescriptor;
    EIP123_DescriptorChain_t OutDescriptor;

    // DMA resources
    DMAResource_Handle_t Std_DMAHandle;
    DMAResource_Handle_t InDCDMAHandle;
    DMAResource_Handle_t OutDCDMAHandle;
    DMAResource_Handle_t InBufDMAHandle;
    DMAResource_Handle_t OutBufDMAHandle;
    DMAResource_Handle_t TokenID_DMAHandle;

    // host addresses
    uint8_t * StdBuffer_p;
    uint8_t * BounceInputBuffer_p;
    uint8_t * LastOutputBuffer_p;
    uint8_t * BounceOutputBuffer_p;
    uint8_t * LastARC4State_p;
    uint8_t * ARC4StateBuffer_p;
    uint32_t * TokenID_p;

    // Address for device DMA
    void * InDCAddr_p;
    void * OutDCAddr_p;
    uint32_t ARC4BufAddr;
    uint32_t TokenID_Addr;
    uint32_t Std_Addr;

    // for switching between separte TokenID buffer and part of output buffer
    // 1 = use TokenID_DMAHandle, else use OutBufDMAHandle with this offset
    unsigned int LastTokenID_ByteOfs;

    unsigned int LastOutputByteCount;

} CALCM_DMA_Admin_t;


CALCM_DMA_Admin_t *
CALCM_DMA_Alloc(void);

void
CALCM_DMA_Free(
        CALCM_DMA_Admin_t * Task_p);

bool
CALAdapter_InputBufferPreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        EIP123_Fragment_t * const Fragment_p,
        const unsigned int InputByteCount,
        const uint8_t * InputBuffer_p,
        const uint8_t * LastBlock_p);

SfzCryptoStatus
CALAdapter_PreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        const unsigned int InputOutputByteCount,
        const uint8_t * InputBuffer_p,
        uint8_t * OutputBuffer_p);

SfzCryptoStatus
CALAdapter_PreDMA_ARC4(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        const unsigned int InputOutputByteCount,
        const uint8_t * InputBuffer_p,
        uint8_t * OutputBuffer_p,
        uint8_t * ARC4State_p,
        uint32_t * const ARC4BufAddr_p);

void
CALAdapter_PostDMA(
        CALCM_DMA_Admin_t * const Task_p);

SfzCryptoStatus
CALAdapter_HashHmacLoad_PrepareInputData(
        CALCM_DMA_Admin_t * const Task_p,
        const uint8_t * InputBuffer_p,
        const unsigned int InputByteCount);

SfzCryptoStatus
CALAdapter_Mac_PrepareInputData(
        CALCM_DMA_Admin_t * const Task_p,
        const uint8_t * InputBuffer_p,
        const unsigned int InputByteCount,
        const uint8_t * LastBlock_p,
        unsigned int BlockSize);

SfzCryptoStatus
CALAdapter_CryptoNopWrap_FinalizeOutput(
        CALCM_DMA_Admin_t * Task_p);

bool
CALAdapter_OutputBufferPreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        EIP123_Fragment_t * const Fragment_p,
        const unsigned int OutputByteCount,
        uint8_t * OutputBuffer_p,
        const uint8_t * InputBuffer_p);

SfzCryptoStatus
CALAdapter_RandomWrapNvm_PrepareOutput(
        CALCM_DMA_Admin_t * const Task_p,
        const unsigned int OutputByteCount,
        uint8_t * OutputBuffer_p,
        bool fOutputByteCount_Includes_TokenId);

SfzCryptoStatus
CALAdapter_RandomWrapNvm_FinalizeOutput(
        CALCM_DMA_Admin_t * const Task_p);

// TokenID value is given to EIP-123 for writing to the
// TokenID memory location pointed out when calling
// EIP123_DescriptorChain_Populate
// Same memory location must be initialized with a different value
#define CAL_TOKENID_VALUE   0x5AFE   /* "SAFE" */


#endif /* Include Guard */

/* end of file cal_cm-v2_dma.h */
