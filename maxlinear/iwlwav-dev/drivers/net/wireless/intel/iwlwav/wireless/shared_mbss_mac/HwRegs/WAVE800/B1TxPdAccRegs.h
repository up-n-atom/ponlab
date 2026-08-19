
/***********************************************************************************
File:				B1TxPdAccRegs.h
Module:				b1TxPdAcc
SOC Revision:		latest
Generated at:       2024-06-26 12:54:46
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_TX_PD_ACC_REGS_H_
#define _B1_TX_PD_ACC_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_TX_PD_ACC_BASE_ADDRESS                            MEMORY_MAP_UNIT_10042_BASE_ADDRESS
#define	REG_B1_TX_PD_ACC_TXPD_ACTIVATION_CTRL                   (B1_TX_PD_ACC_BASE_ADDRESS + 0x0)
#define	REG_B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS                  (B1_TX_PD_ACC_BASE_ADDRESS + 0x4)
#define	REG_B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD                 (B1_TX_PD_ACC_BASE_ADDRESS + 0x8)
#define	REG_B1_TX_PD_ACC_TXPD_DMA_MODE                          (B1_TX_PD_ACC_BASE_ADDRESS + 0xC)
#define	REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS            (B1_TX_PD_ACC_BASE_ADDRESS + 0x10)
#define	REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS            (B1_TX_PD_ACC_BASE_ADDRESS + 0x14)
#define	REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_RD_ENTRIES_NUM         (B1_TX_PD_ACC_BASE_ADDRESS + 0x18)
#define	REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_RELEASE_ENTRIES_NUM    (B1_TX_PD_ACC_BASE_ADDRESS + 0x1C)
#define	REG_B1_TX_PD_ACC_TXPD_ABORT_MODE                        (B1_TX_PD_ACC_BASE_ADDRESS + 0x20)
#define	REG_B1_TX_PD_ACC_TXPD_CONTROL_BITS                      (B1_TX_PD_ACC_BASE_ADDRESS + 0x24)
#define	REG_B1_TX_PD_ACC_TXPD_STATUS_0                          (B1_TX_PD_ACC_BASE_ADDRESS + 0x28)
#define	REG_B1_TX_PD_ACC_TXPD_STATUS_1                          (B1_TX_PD_ACC_BASE_ADDRESS + 0x2C)
#define	REG_B1_TX_PD_ACC_SPP_PER_PD_BITMAP                      (B1_TX_PD_ACC_BASE_ADDRESS + 0x30)
#define	REG_B1_TX_PD_ACC_SKIP_DMA_PER_PD_BITMAP                 (B1_TX_PD_ACC_BASE_ADDRESS + 0x34)
#define	REG_B1_TX_PD_ACC_ADD_MPDU_PER_PD_BITMAP                 (B1_TX_PD_ACC_BASE_ADDRESS + 0x38)
#define	REG_B1_TX_PD_ACC_SPP_FRAGMENT_CONTROL                   (B1_TX_PD_ACC_BASE_ADDRESS + 0x3C)
#define	REG_B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL               (B1_TX_PD_ACC_BASE_ADDRESS + 0x40)
#define	REG_B1_TX_PD_ACC_DBG_DLM_MPDU_COUNT                     (B1_TX_PD_ACC_BASE_ADDRESS + 0x44)
#define	REG_B1_TX_PD_ACC_DBG_DLM_PD_COUNT                       (B1_TX_PD_ACC_BASE_ADDRESS + 0x48)
#define	REG_B1_TX_PD_ACC_TXPD_LOGGER_CONTROL                    (B1_TX_PD_ACC_BASE_ADDRESS + 0x50)
#define	REG_B1_TX_PD_ACC_TXPD_LOGGER_STATUS                     (B1_TX_PD_ACC_BASE_ADDRESS + 0x54)
#define	REG_B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS                  (B1_TX_PD_ACC_BASE_ADDRESS + 0x60)
#define	REG_B1_TX_PD_ACC_TXPD_INT_ERROR_EN                      (B1_TX_PD_ACC_BASE_ADDRESS + 0x64)
#define	REG_B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR                   (B1_TX_PD_ACC_BASE_ADDRESS + 0x68)
#define	REG_B1_TX_PD_ACC_TXPD_SPARE_REGISTER                    (B1_TX_PD_ACC_BASE_ADDRESS + 0x70)
#define	REG_B1_TX_PD_ACC_TXPD_SM_DEBUG                          (B1_TX_PD_ACC_BASE_ADDRESS + 0x74)
#define	REG_B1_TX_PD_ACC_STATISTICS_COUNTERS_EN                 (B1_TX_PD_ACC_BASE_ADDRESS + 0x78)
#define	REG_B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_2                (B1_TX_PD_ACC_BASE_ADDRESS + 0x7C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_TX_PD_ACC_TXPD_ACTIVATION_CTRL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdGo : 1; //Writing to this field a value of '1' serves as a Go command. The TX_PD Accelerator shall start processing the PPDU upon a Go command , It is forbidden to issue a Go command before the previous one had ended. , , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1TxPdAccTxpdActivationCtrl_u;

/*REG_B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 manipulateRetryFieldsEn : 1; //Enable manipulate retry bit and retry count, reset value: 0x1, access type: RW
		uint32 skipMiddlePdInMpdu : 1; //Enable Skip middle PDs in MPDU. This field is used only during Abort Mode. It designates whether only the first and the last PDs within an MPDU are passed to the Sender during Abort., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
		uint32 longMpduThreshold : 14; //Long MPDU threshold (in bytes). An MPDU which is greater or equal than this threshold shall be considered as long. Otherwise it shall be considered as short. This definition is used in the calculation of the retry limit., reset value: 0x300, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB1TxPdAccTxpdOpeartionParams_u;

/*REG_B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ethType0Overhead : 6; //Eth. Type0 (ETH2) overhead. Value is signed, reset value: 0x3a, access type: RW
		uint32 reserved0 : 2;
		uint32 ethType1Overhead : 6; //Eth. Type1 (Rsvd) overhead. Value is signed, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 ethType2Overhead : 6; //Eth. Type2 (SNAP) overhead. Value is signed, reset value: 0x32, access type: RW
		uint32 reserved2 : 2;
		uint32 ethType3Overhead : 6; //Eth. Type3 (EAPOL) overhead. Value is signed, reset value: 0x3a, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegB1TxPdAccTxpdEthTypeOverhead_u;

/*REG_B1_TX_PD_ACC_TXPD_DMA_MODE 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaLastCtrl : 1; //Affects the DMA_Last field within the DMA Job (within the PTD). If DMA_LAST_CTRL configuration bit is set, then DMA Last shall be set for the last PD in the PPDU. Otherwise, DMA Last shall be written as 0., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1TxPdAccTxpdDmaMode_u;

/*REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdSenderPtdFifoBaseAddr : 11; //Base Address of the TXPD_SENDER_PTD_FIFO within the RAM. This is a DW address. Note: Address must be even !!, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 txpdSenderPtdFifoDepthMinusOne : 7; //Depth of the FIFO minus one. Designates the number of PTDs (minus one), which can fit in the FIFO. This number is derived from PTD_WORD_SIZE, which is the actual size which is allocated for each PTD, reset value: 0x1f, access type: RW
		uint32 reserved1 : 1;
		uint32 ptdWordSize : 5; //PTD size in DW units. This field designates the number of DWs which is allocated for each PTD within the memory. This size must not be less than the actual size of the PTD. Note: Size must be even !!, reset value: 0xc, access type: RW
		uint32 reserved2 : 3;
	} bitFields;
} RegB1TxPdAccTxpdSenderPtdFifoParams_u;

/*REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdSenderPtdNumEntriesCount : 7; //Number of entries available for read in the FIFO. Each unit designates a PTD, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txpdSenderPtdFifoWrPtr : 7; //FIFO Write pointer of pending PTDs. Each unit designates a PTD, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 txpdSenderPtdFifoRdPtr : 7; //FIFO Read pointer of released PTDs. Each unit designates a PTD, reset value: 0x0, access type: RO
		uint32 reserved2 : 5;
		uint32 txpdSenderPtdFifoDecrementLessThanZero : 1; //Number of PTDs decremented to less than zero (error indication). , Locked until cleared by txpd_sender_ptd_fifo_clear_dec_less_than_zero, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
	} bitFields;
} RegB1TxPdAccTxpdSenderPtdFifoStatus_u;

/*REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_RD_ENTRIES_NUM 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdSenderPtdRdEntriesNum : 7; //Number of PTDs to decrement. Each unit designates a PTD. The number of pending PTDs shall be decremented (with the amount that was written) when writing to this register, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB1TxPdAccTxpdSenderPtdRdEntriesNum_u;

/*REG_B1_TX_PD_ACC_TXPD_SENDER_PTD_RELEASE_ENTRIES_NUM 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdSenderPtdReleaseEntriesNum : 7; //Number of PTDs to release. Each unit designates a PTD. The number of released PTDs shall be incremented (with the amount that was written) when writing to this register, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB1TxPdAccTxpdSenderPtdReleaseEntriesNum_u;

/*REG_B1_TX_PD_ACC_TXPD_ABORT_MODE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdAbortMode : 1; //When this field is active, the TX_PD Accelerator operates in Abort mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1TxPdAccTxpdAbortMode_u;

/*REG_B1_TX_PD_ACC_TXPD_CONTROL_BITS 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 txpdPendingPtdClear : 1; //Write 1 to this field in order to clear the pending PTDs related pointers and accumulators, reset value: 0x0, access type: WO
		uint32 txpdOccupiedPtdClear : 1; //Write 1 to this field in order to clear the occupied PTDs related pointers and accumulators, reset value: 0x0, access type: WO
		uint32 txpdReleasedPtdClear : 1; //Write 1 to this field in order to clear the released PTDs related pointers and accumulators, reset value: 0x0, access type: WO
		uint32 txpdSenderPtdFifoClearDecLessThanZero : 1; //Write 1 to this field in order to clear txpd_sender_ptd_fifo_decrement_less_than_zero error, reset value: 0x0, access type: WO
		uint32 txpdPtdOccupancyMaxClear : 1; //Write 1 to this field in order to clear all max occupancy indications, reset value: 0x0, access type: WO
		uint32 reserved1 : 23;
	} bitFields;
} RegB1TxPdAccTxpdControlBits_u;

/*REG_B1_TX_PD_ACC_TXPD_STATUS_0 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdPendingPtdOccupancyMax : 7; //Maximal number of pending PTDs (Cleared by writing to txpd_ptd_occupancy_max_clear), reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txpdOccupiedPtdOccupancyMax : 7; //Maximal number of occupied PTDs (Cleared by writing to txpd_ptd_occupancy_max_clear), reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 txpdReleasedPtdOccupancyMax : 7; //Maximal number of released PTDs (Cleared by writing to txpd_ptd_occupancy_max_clear), reset value: 0x0, access type: RO
		uint32 reserved2 : 9;
	} bitFields;
} RegB1TxPdAccTxpdStatus0_u;

/*REG_B1_TX_PD_ACC_TXPD_STATUS_1 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdOccupiedPtdOccupancyLive : 7; //Live number of occupied PTDs, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txpdReleasedPtdOccupancyLive : 7; //Live number of released PTDs, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 txpdIsIdle : 1; //Indication that TX_PD block is Idle. May be used as an indication for end of abort process, reset value: 0x1, access type: RO
		uint32 reserved2 : 15;
	} bitFields;
} RegB1TxPdAccTxpdStatus1_u;

/*REG_B1_TX_PD_ACC_SPP_PER_PD_BITMAP 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sppPerPdBitmap : 32; //Sender PD Processing (SPP) bitmap. This bitmap is indexed by PD Type. The SPP bit is written to the PTD, and shall be passed to the Sender through the DMA and the TX_HC, reset value: 0x8038, access type: RW
	} bitFields;
} RegB1TxPdAccSppPerPdBitmap_u;

/*REG_B1_TX_PD_ACC_SKIP_DMA_PER_PD_BITMAP 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 skipDmaPerPdBitmap : 32; //Skip DMA bitmap. This bitmap is indexed by PD Type. The Skip DMA bit is written to the PTD, and shall be passed to the DMA, reset value: 0x78100004, access type: RW
	} bitFields;
} RegB1TxPdAccSkipDmaPerPdBitmap_u;

/*REG_B1_TX_PD_ACC_ADD_MPDU_PER_PD_BITMAP 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addMpduPerPdBitmap : 32; //Add MPDU bitmap. This bitmap is indexed by PD Type. The ADD MPDU bit is written to the PTD, and shall be passed to the TX_HC through the DMA, reset value: 0x9ffffff, access type: RW
	} bitFields;
} RegB1TxPdAccAddMpduPerPdBitmap_u;

/*REG_B1_TX_PD_ACC_SPP_FRAGMENT_CONTROL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sppForFirstFragPtd : 1; //Sender PD Processing (SPP) control for first fragments. When this field is set, SPP shall be set for every PTD of a first fragment (regardless of the SPP_PER_PD_BITMAP), reset value: 0x0, access type: RW
		uint32 sppForNonFirstFragFirstPtd : 1; //Sender PD Processing (SPP) control for non-first fragments (first PTD). When this field is set, SPP shall be set for the first PTD of any non-first fragment (regardless of the SPP_PER_PD_BITMAP), reset value: 0x0, access type: RW
		uint32 sppForNonFirstFragSecondPtd : 1; //Sender PD Processing (SPP) control for non-first fragments (second PTD). When this field is set, SPP shall be set for the second PTD of any non-first fragment (regardless of the SPP_PER_PD_BITMAP), reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1TxPdAccSppFragmentControl_u;

/*REG_B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmMpduCountUserToMonitor : 7; //User ID to monitor for dlm_mpdu_count. If the value is set to all 1's (width of all 1's is band dependent), then all UIDs shall be counted, reset value: 0x7f, access type: RW
		uint32 reserved0 : 1;
		uint32 dlmPdCountUserToMonitor : 7; //User ID to monitor for dlm_pd_count. If the value is set to all 1's (width of all 1's is band dependent), then all UIDs shall be counted, reset value: 0x7f, access type: RW
		uint32 reserved1 : 1;
		uint32 dlmMpduCountClear : 1; //Write 1 to this field in order to clear dlm_mpdu counter, reset value: 0x0, access type: WO
		uint32 dlmPdCountClear : 1; //Write 1 to this field in order to clear dlm_pd counter, reset value: 0x0, access type: WO
		uint32 reserved2 : 14;
	} bitFields;
} RegB1TxPdAccDbgDlmCountersControl_u;

/*REG_B1_TX_PD_ACC_DBG_DLM_MPDU_COUNT 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmMpduCount : 16; //Number of MPDUs that were extracted from the DLM queues. The MPDUs can be counted per specific user or globally (based on dlm_mpdu_count_user_to_monitor), reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1TxPdAccDbgDlmMpduCount_u;

/*REG_B1_TX_PD_ACC_DBG_DLM_PD_COUNT 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmPdCount : 20; //Number of PDs that were extracted from the DLM queues. The PDs can be counted per specific user or globally (based on dlm_pd_count_user_to_monitor), reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1TxPdAccDbgDlmPdCount_u;

/*REG_B1_TX_PD_ACC_TXPD_LOGGER_CONTROL 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdLoggerEn : 1; //Logger Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 txpdLoggerPriority : 2; //TXPD Logger priority, reset value: 0x0, access type: RW
		uint32 txpdLoggerNumOfMsgsMinusOne : 4; //The maximal number of messages which will comprise a logger packet. If timeout (or logger disable) occurs before reaching this threshold, then the number of messages within a logger packet may be less than this threshold, reset value: 0x8, access type: RW
		uint32 reserved1 : 4;
		uint32 txpdLoggerMiddlePdEnable : 1; //When enabled, middle PDs (not first and not last) within an MPDU shall generate a logger message (assuming logger is enabled) , , reset value: 0x0, access type: RW
		uint32 txpdLoggerLastPdEnable : 1; //When enabled, any last PD within an MPDU shall generate a logger message (assuming logger is enabled) , , reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 txpdLoggerMessageDropClear : 1; //Write 1 to this field in order to clear the status indication (txpd_logger_message_drop_sticky) that a logger message was dropped, reset value: 0x0, access type: WO
		uint32 reserved3 : 15;
	} bitFields;
} RegB1TxPdAccTxpdLoggerControl_u;

/*REG_B1_TX_PD_ACC_TXPD_LOGGER_STATUS 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdLoggerIdle : 1; //Indication that the logger is IDLE. Use this indication after the logger is disabled, in order to verify that it has finished any work in progress., reset value: 0x1, access type: RO
		uint32 txpdLoggerSm : 3; //State of the logger state machine, reset value: 0x0, access type: RO
		uint32 txpdLoggerPacketOpen : 1; //Indication whether or not the logger holds an open packet. An open packet designates that some messages were already output by the logger, but the packet was not closed and sent yet., reset value: 0x0, access type: RO
		uint32 txpdLoggerMessageDropSticky : 1; //Sticky Indication (cleared by writing to txpd_logger_message_drop_clear) - designates that a logger message was dropped, since a new message was initiated before the previuos one ended, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1TxPdAccTxpdLoggerStatus_u;

/*REG_B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdGoWhileActiveMismatchStatus : 1; //Go command is issued while TX_PD is not idle, reset value: 0x0, access type: RO
		uint32 txpdSymbolOverheadOverflowStatus : 1; //Accumulated symbol length to transmit exceeds the allowed maximum, reset value: 0x0, access type: RO
		uint32 txpdLastInStaDlmMismatchStatus : 1; //Last is STA indication is set (within MPDU Descriptor) but DLM queue of the respective user is not yet empty, reset value: 0x0, access type: RO
		uint32 txpdOccupiedPtdUnderflowStatus : 1; //Number of occupied PTDs has underflowed, reset value: 0x0, access type: RO
		uint32 txpdSnSsnDiffOorStatus : 1; //Difference between SN and SSN is Out of Range (greater or equal than 256), reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1TxPdAccTxpdIntErrorStatus_u;

/*REG_B1_TX_PD_ACC_TXPD_INT_ERROR_EN 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdGoWhileActiveMismatchEn : 1; //Go while active mismatch interrupt enable, reset value: 0x0, access type: RW
		uint32 txpdSymbolOverheadOverflowEn : 1; //Symbol Overhead Overflow interrupt enable, reset value: 0x0, access type: RW
		uint32 txpdLastInStaDlmMismatchEn : 1; //Last in STA DLM Mismatch interrupt enable, reset value: 0x0, access type: RW
		uint32 txpdOccupiedPtdUnderflowEn : 1; //Occupied PTD Underflow interrupt enable, reset value: 0x0, access type: RW
		uint32 txpdSnSsnDiffOorEn : 1; //SN SSN diff OOR interrupt enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1TxPdAccTxpdIntErrorEn_u;

/*REG_B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdGoWhileActiveMismatchClr : 1; //Go while active mismatch interrupt clear, reset value: 0x0, access type: WO
		uint32 txpdSymbolOverheadOverflowClr : 1; //Symbol Overhead Overflow interrupt clear, reset value: 0x0, access type: WO
		uint32 txpdLastInStaDlmMismatchClr : 1; //Last in STA DLM Mismatch interrupt clear, reset value: 0x0, access type: WO
		uint32 txpdOccupiedPtdUnderflowClr : 1; //Occupied PTD Underflow interrupt clear, reset value: 0x0, access type: WO
		uint32 txpdSnSsnDiffOorClr : 1; //SN SSN diff OOR interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1TxPdAccTxpdIntErrorClear_u;

/*REG_B1_TX_PD_ACC_TXPD_SPARE_REGISTER 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegister : 16; //Spare Register, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1TxPdAccTxpdSpareRegister_u;

/*REG_B1_TX_PD_ACC_TXPD_SM_DEBUG 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txpdCoreSm : 4; //txpd_core_sm, reset value: 0x0, access type: RO
		uint32 txpdPtdRdSm : 3; //txpd_ptd_rd_sm, reset value: 0x0, access type: RO
		uint32 txpdPdWrSm : 3; //txpd_pd_wr_sm, reset value: 0x0, access type: RO
		uint32 txpdRdbmSm : 4; //txpd_rdbm_sm, reset value: 0x0, access type: RO
		uint32 txpdStdmSm : 4; //txpd_stdm_sm, reset value: 0x0, access type: RO
		uint32 txpdVapmSm : 2; //txpd_vapm_sm, reset value: 0x0, access type: RO
		uint32 txpdDbitCalcSm : 2; //txpd_dbit_calc_sm, reset value: 0x0, access type: RO
		uint32 txpdInitSm : 2; //txpd_init_sm, reset value: 0x0, access type: RO
		uint32 txpdPtdWrSm : 2; //txpd_ptd_wr_sm, reset value: 0x0, access type: RO
		uint32 txpdPtdWrWrapSm : 2; //txpd_ptd_wr_wrap_sm, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1TxPdAccTxpdSmDebug_u;

/*REG_B1_TX_PD_ACC_STATISTICS_COUNTERS_EN 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 retryCountEn : 1; //Retry counter enable., reset value: 0x0, access type: RW
		uint32 multipleRetryCountEn : 1; //Multiple retry counter enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1TxPdAccStatisticsCountersEn_u;

/*REG_B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_2 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 discardAffectedByBaSessionEn : 1; //When this field is active, the TX_PD Accelerator shall not set the Discard bit (within the Retry DB) if the MPDU belongs to a TID with BA aggreement. Retry Counter will continue to advance regardless of this configuration, reset value: 0x0, access type: RW
		uint32 senderNotEmptyIrqNullifyEn : 1; //This field enables the nullify IRQ mechanism. It ensures that txpd_sender_ptd_fifo_not_empty IRQ is negated after each txpd_sender_ptd_rd_entries_num_pulse_wr and before being asserted again (if needed). , It is used to generate a rising edge to the IRQ (opposed to level IRQ) after the sender writes the number of read PTD entries. , , reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1TxPdAccTxpdOpeartionParams2_u;

//========================================
/* REG_TX_PD_ACC_TXPD_ACTIVATION_CTRL 0x0 */
#define B1_TX_PD_ACC_TXPD_ACTIVATION_CTRL_FIELD_TXPD_GO_MASK                                                      0x00000001
//========================================
/* REG_TX_PD_ACC_TXPD_OPEARTION_PARAMS 0x4 */
#define B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_FIELD_MANIPULATE_RETRY_FIELDS_EN_MASK                                  0x00000001
#define B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_FIELD_SKIP_MIDDLE_PD_IN_MPDU_MASK                                      0x00000002
#define B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_FIELD_LONG_MPDU_THRESHOLD_MASK                                         0x3FFF0000
//========================================
/* REG_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD 0x8 */
#define B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD_FIELD_ETH_TYPE0_OVERHEAD_MASK                                         0x0000003F
#define B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD_FIELD_ETH_TYPE1_OVERHEAD_MASK                                         0x00003F00
#define B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD_FIELD_ETH_TYPE2_OVERHEAD_MASK                                         0x003F0000
#define B1_TX_PD_ACC_TXPD_ETH_TYPE_OVERHEAD_FIELD_ETH_TYPE3_OVERHEAD_MASK                                         0x3F000000
//========================================
/* REG_TX_PD_ACC_TXPD_DMA_MODE 0xC */
#define B1_TX_PD_ACC_TXPD_DMA_MODE_FIELD_DMA_LAST_CTRL_MASK                                                       0x00000001
//========================================
/* REG_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS 0x10 */
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS_FIELD_TXPD_SENDER_PTD_FIFO_BASE_ADDR_MASK                        0x000007FF
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS_FIELD_TXPD_SENDER_PTD_FIFO_DEPTH_MINUS_ONE_MASK                  0x007F0000
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_PARAMS_FIELD_PTD_WORD_SIZE_MASK                                         0x1F000000
//========================================
/* REG_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS 0x14 */
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS_FIELD_TXPD_SENDER_PTD_NUM_ENTRIES_COUNT_MASK                     0x0000007F
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS_FIELD_TXPD_SENDER_PTD_FIFO_WR_PTR_MASK                           0x00007F00
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS_FIELD_TXPD_SENDER_PTD_FIFO_RD_PTR_MASK                           0x007F0000
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_FIFO_STATUS_FIELD_TXPD_SENDER_PTD_FIFO_DECREMENT_LESS_THAN_ZERO_MASK         0x10000000
//========================================
/* REG_TX_PD_ACC_TXPD_SENDER_PTD_RD_ENTRIES_NUM 0x18 */
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_RD_ENTRIES_NUM_FIELD_TXPD_SENDER_PTD_RD_ENTRIES_NUM_MASK                     0x0000007F
//========================================
/* REG_TX_PD_ACC_TXPD_SENDER_PTD_RELEASE_ENTRIES_NUM 0x1C */
#define B1_TX_PD_ACC_TXPD_SENDER_PTD_RELEASE_ENTRIES_NUM_FIELD_TXPD_SENDER_PTD_RELEASE_ENTRIES_NUM_MASK           0x0000007F
//========================================
/* REG_TX_PD_ACC_TXPD_ABORT_MODE 0x20 */
#define B1_TX_PD_ACC_TXPD_ABORT_MODE_FIELD_TXPD_ABORT_MODE_MASK                                                   0x00000001
//========================================
/* REG_TX_PD_ACC_TXPD_CONTROL_BITS 0x24 */
#define B1_TX_PD_ACC_TXPD_CONTROL_BITS_FIELD_TXPD_PENDING_PTD_CLEAR_MASK                                          0x00000010
#define B1_TX_PD_ACC_TXPD_CONTROL_BITS_FIELD_TXPD_OCCUPIED_PTD_CLEAR_MASK                                         0x00000020
#define B1_TX_PD_ACC_TXPD_CONTROL_BITS_FIELD_TXPD_RELEASED_PTD_CLEAR_MASK                                         0x00000040
#define B1_TX_PD_ACC_TXPD_CONTROL_BITS_FIELD_TXPD_SENDER_PTD_FIFO_CLEAR_DEC_LESS_THAN_ZERO_MASK                   0x00000080
#define B1_TX_PD_ACC_TXPD_CONTROL_BITS_FIELD_TXPD_PTD_OCCUPANCY_MAX_CLEAR_MASK                                    0x00000100
//========================================
/* REG_TX_PD_ACC_TXPD_STATUS_0 0x28 */
#define B1_TX_PD_ACC_TXPD_STATUS_0_FIELD_TXPD_PENDING_PTD_OCCUPANCY_MAX_MASK                                      0x0000007F
#define B1_TX_PD_ACC_TXPD_STATUS_0_FIELD_TXPD_OCCUPIED_PTD_OCCUPANCY_MAX_MASK                                     0x00007F00
#define B1_TX_PD_ACC_TXPD_STATUS_0_FIELD_TXPD_RELEASED_PTD_OCCUPANCY_MAX_MASK                                     0x007F0000
//========================================
/* REG_TX_PD_ACC_TXPD_STATUS_1 0x2C */
#define B1_TX_PD_ACC_TXPD_STATUS_1_FIELD_TXPD_OCCUPIED_PTD_OCCUPANCY_LIVE_MASK                                    0x0000007F
#define B1_TX_PD_ACC_TXPD_STATUS_1_FIELD_TXPD_RELEASED_PTD_OCCUPANCY_LIVE_MASK                                    0x00007F00
#define B1_TX_PD_ACC_TXPD_STATUS_1_FIELD_TXPD_IS_IDLE_MASK                                                        0x00010000
//========================================
/* REG_TX_PD_ACC_SPP_PER_PD_BITMAP 0x30 */
#define B1_TX_PD_ACC_SPP_PER_PD_BITMAP_FIELD_SPP_PER_PD_BITMAP_MASK                                               0xFFFFFFFF
//========================================
/* REG_TX_PD_ACC_SKIP_DMA_PER_PD_BITMAP 0x34 */
#define B1_TX_PD_ACC_SKIP_DMA_PER_PD_BITMAP_FIELD_SKIP_DMA_PER_PD_BITMAP_MASK                                     0xFFFFFFFF
//========================================
/* REG_TX_PD_ACC_ADD_MPDU_PER_PD_BITMAP 0x38 */
#define B1_TX_PD_ACC_ADD_MPDU_PER_PD_BITMAP_FIELD_ADD_MPDU_PER_PD_BITMAP_MASK                                     0xFFFFFFFF
//========================================
/* REG_TX_PD_ACC_SPP_FRAGMENT_CONTROL 0x3C */
#define B1_TX_PD_ACC_SPP_FRAGMENT_CONTROL_FIELD_SPP_FOR_FIRST_FRAG_PTD_MASK                                       0x00000001
#define B1_TX_PD_ACC_SPP_FRAGMENT_CONTROL_FIELD_SPP_FOR_NON_FIRST_FRAG_FIRST_PTD_MASK                             0x00000002
#define B1_TX_PD_ACC_SPP_FRAGMENT_CONTROL_FIELD_SPP_FOR_NON_FIRST_FRAG_SECOND_PTD_MASK                            0x00000004
//========================================
/* REG_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL 0x40 */
#define B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL_FIELD_DLM_MPDU_COUNT_USER_TO_MONITOR_MASK                           0x0000007F
#define B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL_FIELD_DLM_PD_COUNT_USER_TO_MONITOR_MASK                             0x00007F00
#define B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL_FIELD_DLM_MPDU_COUNT_CLEAR_MASK                                     0x00010000
#define B1_TX_PD_ACC_DBG_DLM_COUNTERS_CONTROL_FIELD_DLM_PD_COUNT_CLEAR_MASK                                       0x00020000
//========================================
/* REG_TX_PD_ACC_DBG_DLM_MPDU_COUNT 0x44 */
#define B1_TX_PD_ACC_DBG_DLM_MPDU_COUNT_FIELD_DLM_MPDU_COUNT_MASK                                                 0x0000FFFF
//========================================
/* REG_TX_PD_ACC_DBG_DLM_PD_COUNT 0x48 */
#define B1_TX_PD_ACC_DBG_DLM_PD_COUNT_FIELD_DLM_PD_COUNT_MASK                                                     0x000FFFFF
//========================================
/* REG_TX_PD_ACC_TXPD_LOGGER_CONTROL 0x50 */
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_EN_MASK                                                0x00000001
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_PRIORITY_MASK                                          0x0000000C
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_NUM_OF_MSGS_MINUS_ONE_MASK                             0x000000F0
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_MIDDLE_PD_ENABLE_MASK                                  0x00001000
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_LAST_PD_ENABLE_MASK                                    0x00002000
#define B1_TX_PD_ACC_TXPD_LOGGER_CONTROL_FIELD_TXPD_LOGGER_MESSAGE_DROP_CLEAR_MASK                                0x00010000
//========================================
/* REG_TX_PD_ACC_TXPD_LOGGER_STATUS 0x54 */
#define B1_TX_PD_ACC_TXPD_LOGGER_STATUS_FIELD_TXPD_LOGGER_IDLE_MASK                                               0x00000001
#define B1_TX_PD_ACC_TXPD_LOGGER_STATUS_FIELD_TXPD_LOGGER_SM_MASK                                                 0x0000000E
#define B1_TX_PD_ACC_TXPD_LOGGER_STATUS_FIELD_TXPD_LOGGER_PACKET_OPEN_MASK                                        0x00000010
#define B1_TX_PD_ACC_TXPD_LOGGER_STATUS_FIELD_TXPD_LOGGER_MESSAGE_DROP_STICKY_MASK                                0x00000020
//========================================
/* REG_TX_PD_ACC_TXPD_INT_ERROR_STATUS 0x60 */
#define B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS_FIELD_TXPD_GO_WHILE_ACTIVE_MISMATCH_STATUS_MASK                        0x00000001
#define B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS_FIELD_TXPD_SYMBOL_OVERHEAD_OVERFLOW_STATUS_MASK                        0x00000002
#define B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS_FIELD_TXPD_LAST_IN_STA_DLM_MISMATCH_STATUS_MASK                        0x00000004
#define B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS_FIELD_TXPD_OCCUPIED_PTD_UNDERFLOW_STATUS_MASK                          0x00000008
#define B1_TX_PD_ACC_TXPD_INT_ERROR_STATUS_FIELD_TXPD_SN_SSN_DIFF_OOR_STATUS_MASK                                 0x00000010
//========================================
/* REG_TX_PD_ACC_TXPD_INT_ERROR_EN 0x64 */
#define B1_TX_PD_ACC_TXPD_INT_ERROR_EN_FIELD_TXPD_GO_WHILE_ACTIVE_MISMATCH_EN_MASK                                0x00000001
#define B1_TX_PD_ACC_TXPD_INT_ERROR_EN_FIELD_TXPD_SYMBOL_OVERHEAD_OVERFLOW_EN_MASK                                0x00000002
#define B1_TX_PD_ACC_TXPD_INT_ERROR_EN_FIELD_TXPD_LAST_IN_STA_DLM_MISMATCH_EN_MASK                                0x00000004
#define B1_TX_PD_ACC_TXPD_INT_ERROR_EN_FIELD_TXPD_OCCUPIED_PTD_UNDERFLOW_EN_MASK                                  0x00000008
#define B1_TX_PD_ACC_TXPD_INT_ERROR_EN_FIELD_TXPD_SN_SSN_DIFF_OOR_EN_MASK                                         0x00000010
//========================================
/* REG_TX_PD_ACC_TXPD_INT_ERROR_CLEAR 0x68 */
#define B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR_FIELD_TXPD_GO_WHILE_ACTIVE_MISMATCH_CLR_MASK                            0x00000001
#define B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR_FIELD_TXPD_SYMBOL_OVERHEAD_OVERFLOW_CLR_MASK                            0x00000002
#define B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR_FIELD_TXPD_LAST_IN_STA_DLM_MISMATCH_CLR_MASK                            0x00000004
#define B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR_FIELD_TXPD_OCCUPIED_PTD_UNDERFLOW_CLR_MASK                              0x00000008
#define B1_TX_PD_ACC_TXPD_INT_ERROR_CLEAR_FIELD_TXPD_SN_SSN_DIFF_OOR_CLR_MASK                                     0x00000010
//========================================
/* REG_TX_PD_ACC_TXPD_SPARE_REGISTER 0x70 */
#define B1_TX_PD_ACC_TXPD_SPARE_REGISTER_FIELD_SPARE_REGISTER_MASK                                                0x0000FFFF
//========================================
/* REG_TX_PD_ACC_TXPD_SM_DEBUG 0x74 */
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_CORE_SM_MASK                                                        0x0000000F
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_PTD_RD_SM_MASK                                                      0x00000070
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_PD_WR_SM_MASK                                                       0x00000380
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_RDBM_SM_MASK                                                        0x00003C00
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_STDM_SM_MASK                                                        0x0003C000
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_VAPM_SM_MASK                                                        0x000C0000
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_DBIT_CALC_SM_MASK                                                   0x00300000
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_INIT_SM_MASK                                                        0x00C00000
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_PTD_WR_SM_MASK                                                      0x03000000
#define B1_TX_PD_ACC_TXPD_SM_DEBUG_FIELD_TXPD_PTD_WR_WRAP_SM_MASK                                                 0x0C000000
//========================================
/* REG_TX_PD_ACC_STATISTICS_COUNTERS_EN 0x78 */
#define B1_TX_PD_ACC_STATISTICS_COUNTERS_EN_FIELD_RETRY_COUNT_EN_MASK                                             0x00000001
#define B1_TX_PD_ACC_STATISTICS_COUNTERS_EN_FIELD_MULTIPLE_RETRY_COUNT_EN_MASK                                    0x00000002
//========================================
/* REG_TX_PD_ACC_TXPD_OPEARTION_PARAMS_2 0x7C */
#define B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_2_FIELD_DISCARD_AFFECTED_BY_BA_SESSION_EN_MASK                         0x00000001
#define B1_TX_PD_ACC_TXPD_OPEARTION_PARAMS_2_FIELD_SENDER_NOT_EMPTY_IRQ_NULLIFY_EN_MASK                           0x00000002


#endif // _TX_PD_ACC_REGS_H_
