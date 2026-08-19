/* cal_cm-v2_dma.c
 *
 * Implementation of the CAL API for Crypto Module.
 *
 * This file contains the DMA-safe buffer functionality:
 * - alloc/free
 * - pre/post DMA
 * - data bounce
 * - DMA descriptor chaining
 * - Synchronization: TokenID support
 */

/*****************************************************************************
* Copyright (c) 2007-2015 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_cal_cm-v2.h"        // configuration

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm-v2_dma.h"      // the API to implement

#include "dmares_buf.h"         // DMAResource_Alloc/Release/CheckAndRegister
#include "dmares_addr.h"        // DMAResource_Translate
#include "dmares_rw.h"          // DMAResource_Read/Write32, Pre/PostDMA

#include "cm_tokens_random.h"
#include "cm_tokens_misc.h"

#include "spal_sleep.h"         // SPAL_SleepMS
#include "spal_memory.h"

// Is pointer `p' aligned at `a', i.e. are its log2(a) low bits zero?
#define IS_ALIGNED(p, a)  (0 == ((((char *)(p)) - (char *)0) & ((a)-1)))

// size of a descriptor chain
// this must match the value returned by EIP122_Get_DC_DMAResource_Size()
#define CALCM_DMA_STD_SIZE_DC      256

#define CALCM_DMA_STD_OFS_TOKENID  0
#define CALCM_DMA_STD_OFS_DC_IN    (CALCM_DMA_STD_OFS_TOKENID + 4)
#define CALCM_DMA_STD_OFS_DC_OUT   (CALCM_DMA_STD_OFS_DC_IN   + CALCM_DMA_STD_SIZE_DC)
#define CALCM_DMA_STD_OFS_ARC4     (CALCM_DMA_STD_OFS_DC_OUT  + CALCM_DMA_STD_SIZE_DC)
#define CALCM_DMA_STD_SIZE         (CALCM_DMA_STD_OFS_ARC4    + EIP123_ARC4_STATE_BUF_SIZE)

#define LTQ_EIP123_TMP_HACK

/*----------------------------------------------------------------------------
 * CALCM_DMA_Alloc
 *
 * This routine allocates a DMA administration block in which all the handles,
 * pointers and offsets related to a DMA transaction are tracked.
 *
 * One fixed-size DMA-safe buffer is allocated for a few standard items
 * (DMA descriptor chains, TokenID Word and ARC4 state).
 *
 * Returns a pointer to the dynamically allocated instance that must be freed
 * by calling CALCM_DMA_Free(), or NULL in case of an error.
 */
CALCM_DMA_Admin_t *
CALCM_DMA_Alloc(void)
{
    int result;
    DMAResource_Handle_t DMAResHandle;
    DMAResource_AddrPair_t DMAResAddrPair;
    DMAResource_Properties_t DMAResProp = {0};
    CALCM_DMA_Admin_t * Task_p;
    unsigned int AllocCase = 0;

    if (CALCM_DMA_STD_SIZE_DC <= EIP123_Get_DC_DMAResource_Size())
    {
        LOG_CRIT(
            "CALCM_DMA_Alloc: "
            "Configuration error! (%d < %d)\n",
            CALCM_DMA_STD_SIZE_DC,
            EIP123_Get_DC_DMAResource_Size());

        return NULL;
    }

    // allocate the administration structure
    Task_p = SPAL_Memory_Calloc(1, sizeof(CALCM_DMA_Admin_t));
    if (Task_p == NULL)
        return NULL;

    // allocate the standard DMA buffer
    AllocCase = 1;
    DMAResProp.Size = CALCM_DMA_STD_SIZE;
    DMAResProp.Alignment = 4;
    DMAResProp.Bank = CALCM_DMA_BANK;

    result = DMAResource_Alloc(
                        DMAResProp,
                        &DMAResAddrPair,
                        &DMAResHandle);

    Task_p->Std_DMAHandle = DMAResHandle;
    Task_p->StdBuffer_p = DMAResAddrPair.Address_p;

    if (result == 0)
    {
        // Translate standard DMA buffer address
        AllocCase = 2;
        result = DMAResource_Translate(DMAResHandle,
                                       DMARES_DOMAIN_EIP12xDMA,
                                       &DMAResAddrPair);

        Task_p->Std_Addr = (uint32_t)(uintptr_t)DMAResAddrPair.Address_p;
    }

    if (result == 0)
    {
        // Register a subset of the standard buffer as Input Descriptor Chain
        AllocCase = 3;

        DMAResProp.Size = CALCM_DMA_STD_SIZE_DC;
        DMAResProp.Alignment = CALCM_DMA_ALIGNMENT;

        DMAResAddrPair.Address_p = Task_p->StdBuffer_p + CALCM_DMA_STD_OFS_DC_IN;
        DMAResAddrPair.Domain = DMARES_DOMAIN_HOST;

        result = DMAResource_CheckAndRegister(
                                DMAResProp,
                                DMAResAddrPair,
                                'R',
                                &DMAResHandle);

        Task_p->InDCDMAHandle = DMAResHandle;
        Task_p->InDCAddr_p = (void *)(uintptr_t)(Task_p->Std_Addr + CALCM_DMA_STD_OFS_DC_IN);
    }

    if (result == 0)
    {
        // Register a subset of the standard buffer as Output Descriptor Chain
        AllocCase = 3;

        DMAResProp.Size = CALCM_DMA_STD_SIZE_DC;
        DMAResProp.Alignment = CALCM_DMA_ALIGNMENT;

        DMAResAddrPair.Address_p = Task_p->StdBuffer_p + CALCM_DMA_STD_OFS_DC_OUT;
        DMAResAddrPair.Domain = DMARES_DOMAIN_HOST;

        result = DMAResource_CheckAndRegister(
                                DMAResProp,
                                DMAResAddrPair,
                                'R',
                                &DMAResHandle);

        Task_p->OutDCDMAHandle = DMAResHandle;
        Task_p->OutDCAddr_p = (void *)(uintptr_t)(Task_p->Std_Addr + CALCM_DMA_STD_OFS_DC_OUT);
    }

    if (result == 0)
    {
        // set up the ARC4 state buffer
        Task_p->ARC4StateBuffer_p = Task_p->StdBuffer_p + CALCM_DMA_STD_OFS_ARC4;
        Task_p->ARC4BufAddr = Task_p->Std_Addr + CALCM_DMA_STD_OFS_ARC4;
    }

    if (result == 0)
    {
        // setup the Token ID word
        Task_p->TokenID_DMAHandle = Task_p->Std_DMAHandle;
        Task_p->TokenID_p = (uint32_t *)(Task_p->StdBuffer_p + CALCM_DMA_STD_OFS_TOKENID);
        Task_p->TokenID_Addr = Task_p->Std_Addr + CALCM_DMA_STD_OFS_TOKENID;
    }

    if (result == 0)
        return Task_p;

    LOG_CRIT(
        "CALCM_DMA_Alloc:"
        " Failure in case %d"
        " (error %d)\n",
        AllocCase,
        result);
    IDENTIFIER_NOT_USED(AllocCase);     // avoids warning when LOG_CRIT is off

    CALCM_DMA_Free(Task_p);

    return NULL;
}


/*----------------------------------------------------------------------------
 * CALCM_DMA_Free
 */
void
CALCM_DMA_Free(
        CALCM_DMA_Admin_t * Task_p)
{
    // free the resources allocated by CALCM_DMA_Alloc
    if (Task_p->InDCDMAHandle)
        DMAResource_Release(Task_p->InDCDMAHandle);

    if (Task_p->OutDCDMAHandle)
        DMAResource_Release(Task_p->OutDCDMAHandle);

    if (Task_p->Std_DMAHandle)
        DMAResource_Release(Task_p->Std_DMAHandle);

    SPAL_Memory_Free(Task_p);
}


/*----------------------------------------------------------------------------
 * CALAdapter_InputBufferPreDMA
 *
 * This function prepares the Input Buffer for EIP123 DMA operation.
 */
bool
CALAdapter_InputBufferPreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        EIP123_Fragment_t * const Fragment_p,
        const unsigned int InputByteCount,
        const uint8_t * InputBuffer_p,
        const uint8_t * LastBlock_p)
{
    EIP123_Status_t res12x;
    int result;
    DMAResource_AddrPair_t DMAResAddrPair;
    DMAResource_Handle_t DMAHandle = {0};
    DMAResource_Properties_t DMAResProp = {0};

    if (0 == InputByteCount)
    {
        // note that a (continued) hash function might be called with
        // zero-length input data.
        return true;
    }

    if (Task_p->InBufDMAHandle != NULL)
    {
        LOG_WARN("CALAdapter_InputBufferPreDMA: InBufDMAHandle not NULL\n");
    }

    if (Task_p->BounceInputBuffer_p != NULL)
    {
        LOG_WARN("CALAdapter_InputBufferPreDMA: BounceInputBuffer_p not NULL\n");
    }

    // Prepare Input Buffer for DMA operation
    Task_p->InBufDMAHandle = NULL;
    Task_p->BounceInputBuffer_p = NULL;

    DMAResProp.Size = (InputByteCount + 3) & (~3);
    DMAResProp.Alignment = CALCM_DMA_ALIGNMENT;
    DMAResProp.Bank = CALCM_DMA_BANK;

    DMAResAddrPair.Address_p = (void *)InputBuffer_p;
    DMAResAddrPair.Domain = DMARES_DOMAIN_HOST;

    // Always use a bounce buffer if the input buffer does not
    // have the minimum required alignment or LastBlock_p is non-NULL.
    result = -1;
#ifndef CALCM_DMA_BOUNCE_ALWAYS
    if (IS_ALIGNED(InputBuffer_p, CALCM_DMA_ALIGNMENT) &&
        (LastBlock_p == NULL))
    {
        // Try to register the buffer hoping it is already DMA-safe
        result = DMAResource_CheckAndRegister(
                                        DMAResProp,
                                        DMAResAddrPair,
                                        'R',
                                        &DMAHandle);
    }
#endif /* CALCM_DMA_BOUNCE_ALWAYS */
    if (result < 0)
    {
        // Buffer is badly aligned or not registered, bounce it
        result = DMAResource_Alloc(
                            DMAResProp,
                            &DMAResAddrPair,
                            &DMAHandle);
        if (result < 0)
        {
            LOG_WARN(
                "CALAdapter_InputBufferPreDMA: "
                "Bouncing Input Buffer failed: %d (Size=0x%x)\n",
                result,
                DMAResProp.Size);

            goto fail;
        }

        // Copy original buffer to the bounce buffer;
        // Input is seen as a raw byte stream, hence
        // DMAResource_WriteArray is not applicable.
        Task_p->BounceInputBuffer_p = DMAResAddrPair.Address_p;
        if (LastBlock_p)
        {
            unsigned int Size1 = InputByteCount - AlgorithmicBlockSize;

            // LastBlock_p is used in case some original input
            // needed to be padded to make it block-sized.

            // copy main input to bounce buffer
            memcpy(
                Task_p->BounceInputBuffer_p,
                InputBuffer_p,
                Size1);

            // append last block
            memcpy(
                Task_p->BounceInputBuffer_p + Size1,
                LastBlock_p,
                AlgorithmicBlockSize);
        }
        else
        {
            memcpy(
                Task_p->BounceInputBuffer_p,
                InputBuffer_p,
                InputByteCount);
        }
    }

    Task_p->InBufDMAHandle = DMAHandle;
    Task_p->LastOutputByteCount = InputByteCount;

    // Translate Input Buffer address
    result = DMAResource_Translate(
                            Task_p->InBufDMAHandle,
                            DMARES_DOMAIN_EIP12xDMA,
                            &DMAResAddrPair);

    if (result < 0)
    {
        // Address could not be translated, abort
        LOG_INFO(
            "CALAdapter_InputBufferPreDMA: "
            "Input Buffer Address Translation failed: %d\n",
            result);

        goto fail;
    }

    Fragment_p->StartAddress = (uint32_t)(uintptr_t)DMAResAddrPair.Address_p;
    Fragment_p->Length = InputByteCount;

    res12x = EIP123_DescriptorChain_Populate(
                    &Task_p->InDescriptor,
                    Task_p->InDCDMAHandle,
                    (uint32_t)(uintptr_t)Task_p->InDCAddr_p,
                    /*Input:*/true,
                    /*Fragment count:*/1,
                    Fragment_p,
                    AlgorithmicBlockSize,
                    /*TokenID Address, not used:*/0);

    if (res12x != EIP123_STATUS_SUCCESS)
    {
        LOG_WARN("Populate Input Descriptor Chain failed: %d\n", res12x);
        goto fail;
    }

    // Ensure data coherence for the Input Buffer
    DMAResource_PreDMA(Task_p->InBufDMAHandle, 0, 0);

    return true;

fail:

    if (Task_p->InBufDMAHandle)
    {
        DMAResource_Release(Task_p->InBufDMAHandle);
        Task_p->InBufDMAHandle = NULL;
    }

    Task_p->BounceInputBuffer_p = NULL;

    return false;
}


/*----------------------------------------------------------------------------
 * CALAdapter_OutputBufferPreDMA
 *
 * This function prepares the Output Buffer for EIP123 DMA operation.
 *
 * Token ID buffer will also be prepared for for EIP123 DMA operation.
 *
 * In place DMA operation (Input Buffer = Output Buffer) is supported.
 */
bool
CALAdapter_OutputBufferPreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        EIP123_Fragment_t * const Fragment_p,
        const unsigned int OutputByteCount,
        uint8_t * OutputBuffer_p,
        const uint8_t * InputBuffer_p)
{
    EIP123_Status_t res12x;
    int result;
    DMAResource_AddrPair_t DMAResAddrPair;
    DMAResource_Handle_t DMAHandle = {0};
    DMAResource_Properties_t DMAResProp = {0};

    if (Task_p->OutBufDMAHandle != NULL)
    {
        LOG_WARN(
            "CALAdapter_OutputBufferPreDMA: "
            "OutBufDMAHandle not NULL\n");
    }

    if (Task_p->BounceOutputBuffer_p != NULL)
    {
        LOG_WARN(
            "CALAdapter_OutputBufferPreDMA: "
            "BounceOutputBuffer_p not NULL\n");
    }

    // Prepare the Output Buffer for DMA operation
    Task_p->OutBufDMAHandle = NULL;
    Task_p->LastOutputBuffer_p = (uint8_t *)OutputBuffer_p;
    Task_p->BounceOutputBuffer_p = NULL;
    #ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
    Task_p->LastTokenID_ByteOfs = 0; /* wadever... i am just trying to hack this  */
    #else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    Task_p->LastTokenID_ByteOfs = 1; /* invalid */
    #endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */

    // Prepare the Output Descriptor Chain
    if (OutputBuffer_p)
    {
        // Check if in place DMA operation is possible
        if (OutputBuffer_p == InputBuffer_p)
        {
            // Check that fragment data is already prepared and sane
            if (Fragment_p->StartAddress == 0 ||
                Fragment_p->Length != OutputByteCount)
            {
                LOG_WARN(
                    "CALAdapter_OutputBufferPreDMA: "
                    "In place DMA operation Pre DMA failed\n");
                goto fail;
            }

            // In place DMA operation. the same DMA resource is used
            // for DMA input and output
            Task_p->OutBufDMAHandle = Task_p->InBufDMAHandle;

            // Check if the Input Buffer was bounced
            if (Task_p->BounceInputBuffer_p)
                Task_p->BounceOutputBuffer_p = Task_p->BounceInputBuffer_p;
        }
        else
        {
            DMAResAddrPair.Address_p = (void *)OutputBuffer_p;
            DMAResAddrPair.Domain = DMARES_DOMAIN_HOST;

            DMAResProp.Size = (OutputByteCount + 3) & (~3);
            DMAResProp.Alignment = CALCM_DMA_ALIGNMENT;
            DMAResProp.Bank = CALCM_DMA_BANK;

            result = -1;
#ifndef CALCM_DMA_BOUNCE_ALWAYS
            if (IS_ALIGNED(OutputBuffer_p, CALCM_DMA_ALIGNMENT))
            {
                // Try to register the buffer hoping it is already DMA-safe
                result = DMAResource_CheckAndRegister(DMAResProp,
                                                      DMAResAddrPair,
                                                      'R',
                                                      &DMAHandle);
            }
#endif /* CALCM_DMA_BOUNCE_ALWAYS */
            if (result < 0)
            {
                // Buffer badly aligned or not registered, bounce it
                result = DMAResource_Alloc(DMAResProp,
                                           &DMAResAddrPair,
                                           &DMAHandle);
                if (result < 0)
                {
                    LOG_WARN(
                        "CALAdapter_OutputBufferPreDMA: "
                        "Bouncing Output Buffer failed: %d (Size=0x%x)\n",
                        result,
                        DMAResProp.Size);
                    goto fail;
                }

                // Copy original buffer to the bounce buffer
                Task_p->BounceOutputBuffer_p = DMAResAddrPair.Address_p;
            }

            Task_p->OutBufDMAHandle = DMAHandle;

            // Translate Output Buffer address
            result = DMAResource_Translate(
                                Task_p->OutBufDMAHandle,
                                DMARES_DOMAIN_EIP12xDMA,
                                &DMAResAddrPair);
            if (result < 0)
            {
                // Address could not be translated, abort
                LOG_WARN(
                    "CALAdapter_OutputBufferPreDMA: "
                    "Output Buffer Address Translation failed: %d\n",
                    result);
                goto fail;
            }

            Fragment_p->StartAddress = (uint32_t)(uintptr_t)DMAResAddrPair.Address_p;
            Fragment_p->Length = OutputByteCount;
        }

        res12x = EIP123_DescriptorChain_Populate(
                      &Task_p->OutDescriptor,
                      Task_p->OutDCDMAHandle,
                      (uint32_t)(uintptr_t)Task_p->OutDCAddr_p,
                      /*Input:*/false,
                      /*Fragment count:*/1,
                      Fragment_p,
                      AlgorithmicBlockSize,
                      #ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
                      /*TokenID Address:*/0);
                      #else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
                      /*TokenID Address:*/Task_p->TokenID_Addr);
                      #endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */

        if (res12x != EIP123_STATUS_SUCCESS)
        {
            LOG_WARN(
                "CALAdapter_OutputBufferPreDMA: "
                "Populate Output Descriptor Chain failed: %d\n",
                res12x);
            goto fail;
        }

        // Check if in place DMA operation is requested
        if (Task_p->OutBufDMAHandle != Task_p->InBufDMAHandle)
        {
            // Ensure data coherence for the Output Buffer,
            // this is already done for in place DMA operation
            DMAResource_PreDMA(Task_p->OutBufDMAHandle, 0, 0);
        }

        #ifndef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK

        // set the initial TokenID value
        {
            Task_p->LastTokenID_ByteOfs = 1;  // 1 == separate TokenID buffer

            DMAResource_Write32(
                    Task_p->TokenID_DMAHandle,
                    0,
                    (uint32_t)~CAL_TOKENID_VALUE);

            // Ensure data coherence for Token ID
            DMAResource_PreDMA(Task_p->TokenID_DMAHandle, 0, 4);
        }

        #endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */

    }

    return true;

fail:

    // Check if the is in place DMA operation
    if (Task_p->OutBufDMAHandle &&
        Task_p->OutBufDMAHandle != Task_p->InBufDMAHandle)
    {
        DMAResource_Release(Task_p->OutBufDMAHandle);
        Task_p->OutBufDMAHandle = NULL;
    }

    Task_p->BounceOutputBuffer_p = NULL;
    Task_p->LastOutputBuffer_p = NULL;

    return false;
}


/*----------------------------------------------------------------------------
 * CALAdapter_PreDMA
 *
 * This function allows the EIP122 DMA to operate on the input data
 * Provided Input and Output Buffers will be checked for DMA-safeness and if
 * safe they will be used for EIP122 DMA operations, otherwise bounce buffers
 * will be allocated
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_PreDMA(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        const unsigned int InputOutputByteCount,
        const uint8_t * InputBuffer_p,
        uint8_t * OutputBuffer_p)
{
    EIP123_Fragment_t Frag;

    Frag.StartAddress = 0;
    Frag.Length = 0;

    // Prepare the Input Buffer for DMA operation
    if (!CALAdapter_InputBufferPreDMA(
                                Task_p,
                                AlgorithmicBlockSize,
                                &Frag,
                                InputOutputByteCount,
                                InputBuffer_p,
                                NULL))
    {
        goto fail;
    }

    // Prepare the Output Buffer for DMA operation
    if (!CALAdapter_OutputBufferPreDMA(
                                Task_p,
                                AlgorithmicBlockSize,
                                &Frag,
                                InputOutputByteCount,
                                OutputBuffer_p,
                                InputBuffer_p))
    {
        goto fail;
    }

    return SFZCRYPTO_SUCCESS;

fail:

    if (Task_p->InBufDMAHandle)
    {
        DMAResource_Release(Task_p->InBufDMAHandle);
        // Check if in place DMA operation was requested
        if (Task_p->InBufDMAHandle == Task_p->OutBufDMAHandle)
        {
            Task_p->OutBufDMAHandle = NULL;
        }
        Task_p->InBufDMAHandle = NULL;
    }

    if (Task_p->OutBufDMAHandle)
    {
        DMAResource_Release(Task_p->OutBufDMAHandle);
        Task_p->OutBufDMAHandle = NULL;
    }

    Task_p->BounceInputBuffer_p = NULL;
    Task_p->LastOutputBuffer_p = NULL;
    Task_p->BounceOutputBuffer_p = NULL;
    Task_p->LastARC4State_p = NULL;

    return SFZCRYPTO_INTERNAL_ERROR;
}


/*----------------------------------------------------------------------------
 * CALAdapter_PreDMA_ARC4
 *
 * Same as CALAdapter_PreDMA but also supports ARC4 state buffer.
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_PreDMA_ARC4(
        CALCM_DMA_Admin_t * const Task_p,
        unsigned int AlgorithmicBlockSize,
        const unsigned int InputOutputByteCount,
        const uint8_t * InputBuffer_p,
        uint8_t * OutputBuffer_p,
        uint8_t * ARC4State_p,
        uint32_t * const ARC4BufAddr_p)
{
    EIP123_Fragment_t Frag;

    Frag.StartAddress = 0;
    Frag.Length = 0;

    if (ARC4State_p == NULL)
        goto fail;

    // Prepare the Input Buffer for DMA operation
    if (!CALAdapter_InputBufferPreDMA(
                                Task_p,
                                AlgorithmicBlockSize,
                                &Frag,
                                InputOutputByteCount,
                                InputBuffer_p,
                                NULL))
    {
        goto fail;
    }

    // Prepare the Output Buffer for DMA operation
    if (!CALAdapter_OutputBufferPreDMA(
                                Task_p,
                                AlgorithmicBlockSize,
                                &Frag,
                                InputOutputByteCount,
                                OutputBuffer_p,
                                InputBuffer_p))
    {
        goto fail;
    }

    // Prepare the ARC4 State Buffer for DMA operation
    Task_p->LastARC4State_p = ARC4State_p;
    *ARC4BufAddr_p = Task_p->ARC4BufAddr;

    // The ARC4 state data is seen as a raw byte stream, hence
    // DMAResource_WriteArray() is not applicable.
    memcpy(
        Task_p->ARC4StateBuffer_p,
        ARC4State_p,
        EIP123_ARC4_STATE_BUF_SIZE);

    // Ensure data coherence for the ARC4 State Buffer
    DMAResource_PreDMA(
            Task_p->Std_DMAHandle,
            CALCM_DMA_STD_OFS_ARC4,
            EIP123_ARC4_STATE_BUF_SIZE);

    return SFZCRYPTO_SUCCESS;

fail:

    if (Task_p->InBufDMAHandle)
    {
        DMAResource_Release(Task_p->InBufDMAHandle);
        // Check if in place DMA operation was requested
        if (Task_p->InBufDMAHandle == Task_p->OutBufDMAHandle)
        {
            Task_p->OutBufDMAHandle = NULL;
        }
        Task_p->InBufDMAHandle = NULL;
    }

    if (Task_p->OutBufDMAHandle)
    {
        DMAResource_Release(Task_p->OutBufDMAHandle);
        Task_p->OutBufDMAHandle = NULL;
    }

    Task_p->BounceInputBuffer_p = NULL;
    Task_p->LastOutputBuffer_p = NULL;
    Task_p->BounceOutputBuffer_p = NULL;
    Task_p->LastARC4State_p = NULL;

    return SFZCRYPTO_INTERNAL_ERROR;
}


/*----------------------------------------------------------------------------
 * CALAdapter_PostDMA
 *
 * This function ensures that the EIP123 DMA operation results are available
 * for the host CPU.
 *
 * If bounce buffers were used (in CALAdapter_PostDMA() function)
 * then this function will update the original buffers
 * and free the bounce buffers
 *
 * This function work on the Input, Output and ARC4 State Buffer DMA resources
 *
 * NOTE: This function does not work on the TokenID DMA resource!
 */
void
CALAdapter_PostDMA(
        CALCM_DMA_Admin_t * const Task_p)
{
    // DMA resource for Input Buffer
    if (Task_p->InBufDMAHandle)
    {
        // check it is not in place DMA operation
        if (Task_p->InBufDMAHandle != Task_p->OutBufDMAHandle)
        {
            // Release DMA resource for Input Buffer
            DMAResource_Release(Task_p->InBufDMAHandle);
        }
        Task_p->InBufDMAHandle = NULL;
    }

    // DMA resource for Output Buffer
    if (Task_p->OutBufDMAHandle)
    {
        // Ensure data coherence for the Output Buffer
        DMAResource_PostDMA(Task_p->OutBufDMAHandle, 0, 0);

        // Check if the original buffer was bounced
        if (Task_p->LastOutputBuffer_p &&
            Task_p->BounceOutputBuffer_p &&
            Task_p->LastOutputBuffer_p !=
            Task_p->BounceOutputBuffer_p)
        {
            // Update the original buffer from the bounce buffer
            memcpy(
                Task_p->LastOutputBuffer_p,
                Task_p->BounceOutputBuffer_p,
                Task_p->LastOutputByteCount);
        }

        // Release DMA resource for Output Buffer,
        DMAResource_Release(Task_p->OutBufDMAHandle);
        Task_p->OutBufDMAHandle = NULL;
    }

    // DMA resource for ARC4 State Buffer
    if (Task_p->LastARC4State_p)
    {
        // Ensure data coherence for the ARC4 State Buffer
        DMAResource_PostDMA(
                Task_p->Std_DMAHandle,
                CALCM_DMA_STD_OFS_ARC4,
                EIP123_ARC4_STATE_BUF_SIZE);

        // Update the original buffer from the bounce buffer
        memcpy(
            Task_p->LastARC4State_p,
            Task_p->ARC4StateBuffer_p,
            EIP123_ARC4_STATE_BUF_SIZE);
    }

    Task_p->BounceInputBuffer_p = NULL;
    Task_p->LastOutputBuffer_p = NULL;
    Task_p->BounceOutputBuffer_p = NULL;
    Task_p->LastARC4State_p = NULL;
}


/*----------------------------------------------------------------------------
 * CALAdapter_HashHmacLoad_PrepareInputData
 *
 * This routine is used by the Hash and HMAC services to perform all actions
 * required on the input data (these operations do not have output data).
 *
 * The EIP123 DMA Input Descriptor is populated.
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_HashHmacLoad_PrepareInputData(
        CALCM_DMA_Admin_t * const Task_p,
        const uint8_t * InputBuffer_p,
        const unsigned int InputByteCount)
{
    unsigned int AlgorithmicBlockSize = EIP123_ALGOBLOCKSIZE_HASH;
    EIP123_Fragment_t Frag;

    Frag.StartAddress = 0;
    Frag.Length = 0;

    // Prepare the Input Buffer for DMA operation
    if (!CALAdapter_InputBufferPreDMA(
                                Task_p,
                                AlgorithmicBlockSize,
                                &Frag,
                                InputByteCount,
                                InputBuffer_p,
                                NULL))
    {
        return SFZCRYPTO_INTERNAL_ERROR;
    }

    return SFZCRYPTO_SUCCESS;
}

/*----------------------------------------------------------------------------
 * CALAdapter_Mac_PrepareInputData
 *
 * This routine is a variant on CALAdapter_HashHmacLoad_PrepareInputData.
 * It is used by input-only operations that sometimes need to pad some
 * given input data, see the cipher_mac algorithms for example.
 * 'InputByteCount' is always a multiple 'BlockSize'.
 * If 'LastBlock_p' is non-NULL, the input data consists of the first
 * ('InputByteCount' - 'BlockSize') bytes from 'InputBuffer_p' followed by
 * 'BlockSize' bytes from 'LastBlock_p'.
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_Mac_PrepareInputData(
        CALCM_DMA_Admin_t * const Task_p,
        const uint8_t * InputBuffer_p,
        const unsigned int InputByteCount,
        const uint8_t * LastBlock_p,
        unsigned int BlockSize)
{
    // Ensure data coherency before handing input buffer off to
    // the HW and fill the input buffer descriptor

    EIP123_Fragment_t Frag;

    Frag.StartAddress = 0;
    Frag.Length = 0;

    // Sanity check input
    if ((BlockSize == 0) ||
        (0 != (BlockSize & (BlockSize-1))) || // BlockSize is power-of-two?
        (InputByteCount & (BlockSize-1)))     // Count is N*BlockSize?
    {
        return SFZCRYPTO_INVALID_LENGTH;
    }

    // Prepare the Input Buffer for DMA operation
    if (!CALAdapter_InputBufferPreDMA(
                                Task_p,
                                BlockSize,
                                &Frag,
                                InputByteCount,
                                InputBuffer_p,
                                LastBlock_p))
    {
        return SFZCRYPTO_INTERNAL_ERROR;
    }

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * CALAdapter_CryptoNopWrap_FinalizeOutput
 *
 * This routine is used by symm_crypt and nop services (that both have input
 * and output buffers) to finalize the output after the engine has completed
 * the operation and the DMA has written the output.
 *
 * First, the WriteTokenID word is polled until it is written. This proves
 * that all DMA-written data has actually reached system memory. This step is
 * protected with a timeout.
 *
 * Memory coherency actions are initiated to make sure the output buffers are
 * not cached somewhere.
 *
 * If the output is unaligned, the data is copied from the bounce buffer to
 * the final output buffer.
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_CryptoNopWrap_FinalizeOutput(
        CALCM_DMA_Admin_t * Task_p)
{
    uint32_t value = 0; // Must not be equal to EIP123_TOKENID_VALUE
    int LoopsLimiter = CALCM_POLLING_MAXLOOPS;

    IDENTIFIER_NOT_USED(Task_p);

#ifdef LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK
    // not yet arrived; sleep a bit
    SPAL_SleepMS(CALCM_POLLING_DELAY_MS);
    CALAdapter_PostDMA(Task_p);
    return SFZCRYPTO_SUCCESS;
#else /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
    if (Task_p->TokenID_DMAHandle == NULL)
        return SFZCRYPTO_INVALID_PARAMETER;

    // wait for the TokenID value to "arrive", in case DMA is delayed
    do
    {
        // Ensure data coherence for Token ID before reading it
        if (Task_p->LastTokenID_ByteOfs == 1)
        {
            // separate TokenID buffer
            DMAResource_PostDMA(Task_p->TokenID_DMAHandle, 0, 4);
            value = DMAResource_Read32(Task_p->TokenID_DMAHandle, 0);
        }

        if (value == CAL_TOKENID_VALUE)
            break;  // from the while

        // not yet arrived; sleep a bit
        SPAL_SleepMS(CALCM_POLLING_DELAY_MS);
        LOG_INFO("CAL Adapter: Waiting for TokenID\n");
    }
    while(--LoopsLimiter > 0);

    if (value == CAL_TOKENID_VALUE)
    {
        CALAdapter_PostDMA(Task_p);
        return SFZCRYPTO_SUCCESS;
    }
    else
    {
        return SFZCRYPTO_INTERNAL_ERROR;
    }
#endif /* LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK */
}


/*----------------------------------------------------------------------------
 * CALAdapter_RandomWrapNvm_PrepareOutput
 *
 * This routine is used by random_generate to prepare the output buffer for
 * use by DMA. In order to support WriteTokenID to a single DMA buffer (as
 * required by this class of tokens) we allocate an oversized bounce buffer.
 *
 * Later, when we add zero-copy support, the buffers can be allocated
 * slightly oversized (max +7 bytes) and bouncing can be made selective.
 */
SfzCryptoStatus
CALAdapter_RandomWrapNvm_PrepareOutput(
        CALCM_DMA_Admin_t * const Task_p,
        const unsigned int OutputByteCount,
        uint8_t * OutputBuffer_p,
        bool fOutputByteCount_Includes_TokenId)
{
    unsigned int OutBufSize_Aligned;
    int result;
    DMAResource_AddrPair_t DMAResAddrPair;

    // round up to whole number of 32bit words
    OutBufSize_Aligned = (OutputByteCount + 3) & (~3);
    if (fOutputByteCount_Includes_TokenId == false)
        OutBufSize_Aligned += 4;

    Task_p->OutBufDMAHandle = NULL;
    Task_p->LastOutputBuffer_p = (uint8_t *)OutputBuffer_p;
    Task_p->LastOutputByteCount = OutputByteCount;
    Task_p->BounceOutputBuffer_p = NULL;

    {
        DMAResource_Properties_t DMAResProp = {0};
        DMAResource_Handle_t DMAHandle = {0};

        DMAResProp.Size = OutBufSize_Aligned;
        DMAResProp.Alignment = 4;   // expected alignment = 32bit
        DMAResProp.Bank = CALCM_DMA_BANK;

        // create bounce buffer
        result = DMAResource_Alloc(
                            DMAResProp,
                            &DMAResAddrPair,
                            &DMAHandle);

        if (result < 0)
        {
            LOG_INFO(
                "CALAdapter_Random_PrepareOutput: "
                "Failed to allocate output bounce buffer: %d (Size=0x%x)\n",
                result,
                DMAResProp.Size);

            Task_p->LastOutputBuffer_p = NULL;

            return SFZCRYPTO_INTERNAL_ERROR;
        }

        if (!IS_ALIGNED(DMAResAddrPair.Address_p, 4))
        {
            LOG_INFO(
                "CALAdapter_Random_PrepareOutput: "
                "Bounce buffer address is not aligned!\n");

            DMAResource_Release(DMAHandle);
            return SFZCRYPTO_INTERNAL_ERROR;
        }

        Task_p->BounceOutputBuffer_p = DMAResAddrPair.Address_p;
        Task_p->OutBufDMAHandle = DMAHandle;
    }

    // translate Output Buffer address
    result = DMAResource_Translate(
                            Task_p->OutBufDMAHandle,
                            DMARES_DOMAIN_EIP12xDMA,
                            &DMAResAddrPair);
    if (result < 0)
    {
        // address could not be translated; abort
        LOG_INFO(
            "CALAdapter_Random_PrepareOutput: "
            "Address translation failed: %d\n",
            result);

        DMAResource_Release(Task_p->OutBufDMAHandle);

        Task_p->OutBufDMAHandle = NULL;
        Task_p->BounceOutputBuffer_p = NULL;
        Task_p->LastOutputBuffer_p = NULL;

        return SFZCRYPTO_INTERNAL_ERROR;
    }

    {
        EIP123_Fragment_t Frag = { 0 };
        EIP123_Status_t res12x;

        Frag.StartAddress = (uint32_t)(uintptr_t)DMAResAddrPair.Address_p;
        Frag.Length = OutBufSize_Aligned;

        res12x = EIP123_DescriptorChain_Populate(
                          &Task_p->OutDescriptor,
                          Task_p->OutDCDMAHandle,
                          (uint32_t)(uintptr_t)Task_p->OutDCAddr_p,
                          /*Input:*/false,
                          /*Fragment count:*/1,
                          &Frag,
                          /*AlgorithmicBlockSize:*/4,
                          /*TokenID Address:*/0);  // only output address needed

        if (res12x != EIP123_STATUS_SUCCESS)
        {
            DMAResource_Release(Task_p->OutBufDMAHandle);

            Task_p->OutBufDMAHandle = NULL;
            Task_p->BounceOutputBuffer_p = NULL;
            Task_p->LastOutputBuffer_p = NULL;

            LOG_INFO(
                "CALAdapter_Random_PrepareOutput: "
                "Populate output descriptor chain failed: %d\n",
                res12x);

            return SFZCRYPTO_INTERNAL_ERROR;
        }
    }

    // set the initial TokenID value
    {
        // TokenID will be written to last 32bit word in the buffer
        Task_p->LastTokenID_ByteOfs = OutBufSize_Aligned - 4;

        DMAResource_Write32(
                Task_p->OutBufDMAHandle,
                (Task_p->LastTokenID_ByteOfs / 4),
                (uint32_t)~CAL_TOKENID_VALUE);

        // PreDMA handled as part of output buffer below
    }

    // Ensure data coherence for the Output Buffer,
    DMAResource_PreDMA(Task_p->OutBufDMAHandle, 0, 0);

    return SFZCRYPTO_SUCCESS;
}


/*----------------------------------------------------------------------------
 * CALAdapter_RandomWrapNvm_FinalizeOutput
 *
 * This routine is used by symm_crypt and nop services (that both have input
 * and output buffers) to finalize the output after the engine has completed
 * the operation and the DMA has written the output.
 *
 * First, the WriteTokenID word is polled until it is written. This proves
 * that all DMA-written data has actually reached system memory. This step is
 * protected with a timeout.
 *
 * Memory coherency actions are initiated to make sure the output buffers are
 * not cached somewhere.
 *
 * If the output is unaligned, the data is copied from the bounce buffer to
 * the final output buffer.
 *
 * Returns SFZCRYPTO_SUCCESS upon success, otherwise one of the appropriate
 * error codes.
 */
SfzCryptoStatus
CALAdapter_RandomWrapNvm_FinalizeOutput(
        CALCM_DMA_Admin_t * const Task_p)
{
    uint32_t value = 0; // Must not be equal to EIP123_TOKENID_VALUE
    int LoopsLimiter = CALCM_POLLING_MAXLOOPS;

    IDENTIFIER_NOT_USED(Task_p);

    // wait for the TokenID value to "arrive", in case DMA is delayed
    do
    {
        // check TokenID (part of output buffer)
        DMAResource_PostDMA(
                    Task_p->OutBufDMAHandle,
                    Task_p->LastTokenID_ByteOfs,
                    4);

        value = DMAResource_Read32(
                    Task_p->OutBufDMAHandle,
                    (Task_p->LastTokenID_ByteOfs / 4));

        // see if the tokenID has arrived
        #ifdef LTQ_EIP123_TMP_HACK
        if (value == 0xFE5A0000)
            break;
        #else /* LTQ_EIP123_TMP_HACK */
        if (value == CAL_TOKENID_VALUE)
            break;  // from the while
        #endif /* LTQ_EIP123_TMP_HACK */

        // not yet arrived; sleep a bit
        SPAL_SleepMS(CALCM_POLLING_DELAY_MS);
        LOG_INFO("CAL Adapter: Waiting for TokenID\n");
    }
    while(--LoopsLimiter > 0);

    #ifdef LTQ_EIP123_TMP_HACK
    if (value != 0xFE5A0000)
    #else /* LTQ_EIP123_TMP_HACK */
    if (value != CAL_TOKENID_VALUE)
    #endif /* LTQ_EIP123_TMP_HACK */
        return SFZCRYPTO_INTERNAL_ERROR;

    CALAdapter_PostDMA(Task_p);

    return SFZCRYPTO_SUCCESS;
}


/* end of file cal_cm-v2_dma.c */
