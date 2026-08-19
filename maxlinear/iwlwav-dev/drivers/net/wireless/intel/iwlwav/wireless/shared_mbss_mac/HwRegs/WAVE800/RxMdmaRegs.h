
/***********************************************************************************
File:				RxMdmaRegs.h
Module:				rxMdma
SOC Revision:		latest
Generated at:       2024-06-26 12:55:12
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _RX_MDMA_REGS_H_
#define _RX_MDMA_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define RX_MDMA_BASE_ADDRESS                     MEMORY_MAP_UNIT_51_BASE_ADDRESS
#define	REG_RX_MDMA_DMAE_VER_REG               (RX_MDMA_BASE_ADDRESS + 0x0)
#define	REG_RX_MDMA_DMA_CONFIG_REG             (RX_MDMA_BASE_ADDRESS + 0x4)
#define	REG_RX_MDMA_DESTINATION_ADDRESS_REG    (RX_MDMA_BASE_ADDRESS + 0x8)
#define	REG_RX_MDMA_SOURCE_ADDRESS_REG         (RX_MDMA_BASE_ADDRESS + 0xC)
#define	REG_RX_MDMA_LEN_REG                    (RX_MDMA_BASE_ADDRESS + 0x10)
#define	REG_RX_MDMA_CFG_DIRECT_ACCESS_REG      (RX_MDMA_BASE_ADDRESS + 0x14)
#define	REG_RX_MDMA_CMD_DIRECT_COUNTER_REG     (RX_MDMA_BASE_ADDRESS + 0x18)
#define	REG_RX_MDMA_COUNTER_REG                (RX_MDMA_BASE_ADDRESS + 0x1C)
#define	REG_RX_MDMA_TIMEOUT_CFG_REG            (RX_MDMA_BASE_ADDRESS + 0x20)
#define	REG_RX_MDMA_TIMEOUT_ERROR_REG          (RX_MDMA_BASE_ADDRESS + 0x24)
#define	REG_RX_MDMA_PENDING_JOBS_REG           (RX_MDMA_BASE_ADDRESS + 0x28)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_RX_MDMA_DMAE_VER_REG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaVerI : 32; //dma_ver, reset value: 0x2, access type: RW
	} bitFields;
} RegRxMdmaDmaeVerReg_u;

/*REG_RX_MDMA_DMA_CONFIG_REG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxBurst : 16; //max_burst_-_the_DMA_will_deivide_the_transaction_to_several_burst_with_mac_burst_size, reset value: 0x10, access type: RW
		uint32 ddrAlignEnable : 1; //ddr_align_enable-_the_DMA_will_deivide_the_transaction_to_burst_which_align_to_DDR_line_, reset value: 0x0, access type: RW
		uint32 outstanding : 8; //number_outstanding_-_max_outstanding_accordng_the_DMA_define_, reset value: 0x20, access type: RW
		uint32 reserved0 : 3;
		uint32 ddrLine : 4; //ddr_line-_to_be_use_in_DDR_align_access__4_->_16B__5->_32B__6->64__, reset value: 0x6, access type: RW
	} bitFields;
} RegRxMdmaDmaConfigReg_u;

/*REG_RX_MDMA_DESTINATION_ADDRESS_REG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 destinationAddressI : 32; //DMA_destination_address, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaDestinationAddressReg_u;

/*REG_RX_MDMA_SOURCE_ADDRESS_REG 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sourceAddressI : 32; //DMA_source_address, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaSourceAddressReg_u;

/*REG_RX_MDMA_LEN_REG 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sizeI : 32; //size, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaLenReg_u;

/*REG_RX_MDMA_CFG_DIRECT_ACCESS_REG 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 directWnpEnableI : 1; //enable_WNP_transaction, reset value: 0x0, access type: RW
		uint32 directWordSwapI : 1; //enable_word_swap_for_direct_access, reset value: 0x0, access type: RW
		uint32 directByteSwapI : 1; //enable_byte_swap_for_direct_access, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
		uint32 directDoneAccessI : 1; //done_with_job_requested_from_direct_access_clear_befor_sending_job, reset value: 0x0, access type: RO
	} bitFields;
} RegRxMdmaCfgDirectAccessReg_u;

/*REG_RX_MDMA_CMD_DIRECT_COUNTER_REG 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cmdCounterI : 32; //Number_of_command_request_passed_wrap_around__RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaCmdDirectCounterReg_u;

/*REG_RX_MDMA_COUNTER_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cmdCounterI : 32; //Number_of_command_request_passed_wrap_around__RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaCounterReg_u;

/*REG_RX_MDMA_TIMEOUT_CFG_REG 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timeoutCfgI : 32; //timeout, reset value: 0x0, access type: RW
	} bitFields;
} RegRxMdmaTimeoutCfgReg_u;

/*REG_RX_MDMA_TIMEOUT_ERROR_REG 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timeoutErrorI : 1; //error, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegRxMdmaTimeoutErrorReg_u;

/*REG_RX_MDMA_PENDING_JOBS_REG 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wnpPendingI : 8; //WNP_pending, reset value: 0x0, access type: RO
		uint32 wpPendingI : 8; //WP_pending, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegRxMdmaPendingJobsReg_u;

//========================================
/* REG_RX_MDMA_DMAE_VER_REG 0x0 */
#define B01_RX_MDMA_DMAE_VER_REG_FIELD_DMA_VER_I_MASK                                                             0xFFFFFFFF
//========================================
/* REG_RX_MDMA_DMA_CONFIG_REG 0x4 */
#define B01_RX_MDMA_DMA_CONFIG_REG_FIELD_MAX_BURST_MASK                                                           0x0000FFFF
#define B01_RX_MDMA_DMA_CONFIG_REG_FIELD_DDR_ALIGN_ENABLE_MASK                                                    0x00010000
#define B01_RX_MDMA_DMA_CONFIG_REG_FIELD_OUTSTANDING_MASK                                                         0x01FE0000
#define B01_RX_MDMA_DMA_CONFIG_REG_FIELD_DDR_LINE_MASK                                                            0xF0000000
//========================================
/* REG_RX_MDMA_DESTINATION_ADDRESS_REG 0x8 */
#define B01_RX_MDMA_DESTINATION_ADDRESS_REG_FIELD_DESTINATION_ADDRESS_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_RX_MDMA_SOURCE_ADDRESS_REG 0xC */
#define B01_RX_MDMA_SOURCE_ADDRESS_REG_FIELD_SOURCE_ADDRESS_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_RX_MDMA_LEN_REG 0x10 */
#define B01_RX_MDMA_LEN_REG_FIELD_SIZE_I_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_RX_MDMA_CFG_DIRECT_ACCESS_REG 0x14 */
#define B01_RX_MDMA_CFG_DIRECT_ACCESS_REG_FIELD_DIRECT_WNP_ENABLE_I_MASK                                          0x00000001
#define B01_RX_MDMA_CFG_DIRECT_ACCESS_REG_FIELD_DIRECT_WORD_SWAP_I_MASK                                           0x00000002
#define B01_RX_MDMA_CFG_DIRECT_ACCESS_REG_FIELD_DIRECT_BYTE_SWAP_I_MASK                                           0x00000004
#define B01_RX_MDMA_CFG_DIRECT_ACCESS_REG_FIELD_DIRECT_DONE_ACCESS_I_MASK                                         0x80000000
//========================================
/* REG_RX_MDMA_CMD_DIRECT_COUNTER_REG 0x18 */
#define B01_RX_MDMA_CMD_DIRECT_COUNTER_REG_FIELD_CMD_COUNTER_I_MASK                                               0xFFFFFFFF
//========================================
/* REG_RX_MDMA_COUNTER_REG 0x1C */
#define B01_RX_MDMA_COUNTER_REG_FIELD_CMD_COUNTER_I_MASK                                                          0xFFFFFFFF
//========================================
/* REG_RX_MDMA_TIMEOUT_CFG_REG 0x20 */
#define B01_RX_MDMA_TIMEOUT_CFG_REG_FIELD_TIMEOUT_CFG_I_MASK                                                      0xFFFFFFFF
//========================================
/* REG_RX_MDMA_TIMEOUT_ERROR_REG 0x24 */
#define B01_RX_MDMA_TIMEOUT_ERROR_REG_FIELD_TIMEOUT_ERROR_I_MASK                                                  0x00000001
//========================================
/* REG_RX_MDMA_PENDING_JOBS_REG 0x28 */
#define B01_RX_MDMA_PENDING_JOBS_REG_FIELD_WNP_PENDING_I_MASK                                                     0x000000FF
#define B01_RX_MDMA_PENDING_JOBS_REG_FIELD_WP_PENDING_I_MASK                                                      0x0000FF00


#endif // _RX_MDMA_REGS_H_
