
/***********************************************************************************
File:				B2RxhRdAccRegs.h
Module:				b2RxhRdAcc
SOC Revision:		latest
Generated at:       2024-06-26 12:54:59
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_RXH_RD_ACC_REGS_H_
#define _B2_RXH_RD_ACC_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_RXH_RD_ACC_BASE_ADDRESS                 MEMORY_MAP_UNIT_20063_BASE_ADDRESS
#define	REG_B2_RXH_RD_ACC_CONFIG_INIT                  (B2_RXH_RD_ACC_BASE_ADDRESS + 0x0)
#define	REG_B2_RXH_RD_ACC_STATUS                       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x4)
#define	REG_B2_RXH_RD_ACC_RD_PREFETCH_INTR_MASK        (B2_RXH_RD_ACC_BASE_ADDRESS + 0x8)
#define	REG_B2_RXH_RD_ACC_RD_PREFETCH_INTR_CLEAR       (B2_RXH_RD_ACC_BASE_ADDRESS + 0xC)
#define	REG_B2_RXH_RD_ACC_C0_INCREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x10)
#define	REG_B2_RXH_RD_ACC_C0_DECREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x14)
#define	REG_B2_RXH_RD_ACC_C1_INCREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x18)
#define	REG_B2_RXH_RD_ACC_C1_DECREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x1C)
#define	REG_B2_RXH_RD_ACC_C2_INCREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x20)
#define	REG_B2_RXH_RD_ACC_C2_DECREMENT                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x24)
#define	REG_B2_RXH_RD_ACC_RTD_COUNTER                  (B2_RXH_RD_ACC_BASE_ADDRESS + 0x28)
#define	REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT0          (B2_RXH_RD_ACC_BASE_ADDRESS + 0x2C)
#define	REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT1          (B2_RXH_RD_ACC_BASE_ADDRESS + 0x30)
#define	REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT2          (B2_RXH_RD_ACC_BASE_ADDRESS + 0x34)
#define	REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT3          (B2_RXH_RD_ACC_BASE_ADDRESS + 0x38)
#define	REG_B2_RXH_RD_ACC_LOGGER_CTRL                  (B2_RXH_RD_ACC_BASE_ADDRESS + 0x3C)
#define	REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT_CLR       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x40)
#define	REG_B2_RXH_RD_ACC_DBG_STATE_MACHINES           (B2_RXH_RD_ACC_BASE_ADDRESS + 0x44)
#define	REG_B2_RXH_RD_ACC_DBG_ERRORS                   (B2_RXH_RD_ACC_BASE_ADDRESS + 0x48)
#define	REG_B2_RXH_RD_ACC_DBG_STATUSES                 (B2_RXH_RD_ACC_BASE_ADDRESS + 0x4C)
#define	REG_B2_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG         (B2_RXH_RD_ACC_BASE_ADDRESS + 0x50)
#define	REG_B2_RXH_RD_ACC_DA_SA_CHECK_EN               (B2_RXH_RD_ACC_BASE_ADDRESS + 0x54)
#define	REG_B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_MSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x58)
#define	REG_B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_LSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x5C)
#define	REG_B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_MSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x60)
#define	REG_B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_LSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x64)
#define	REG_B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_MSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x68)
#define	REG_B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_LSB       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x6C)
#define	REG_B2_RXH_RD_ACC_LOGGER_C0_STALLED_LIM_CFG    (B2_RXH_RD_ACC_BASE_ADDRESS + 0x70)
#define	REG_B2_RXH_RD_ACC_DBG_C0_STALLED_MAX_CNT       (B2_RXH_RD_ACC_BASE_ADDRESS + 0x74)
#define	REG_B2_RXH_RD_ACC_RELEASED_RTD_COUNTER         (B2_RXH_RD_ACC_BASE_ADDRESS + 0x78)
#define	REG_B2_RXH_RD_ACC_DBG_MPDU_PTR_RTD_GEN         (B2_RXH_RD_ACC_BASE_ADDRESS + 0x7C)
#define	REG_B2_RXH_RD_ACC_DBG_ETH_PTR_RTD_GEN          (B2_RXH_RD_ACC_BASE_ADDRESS + 0x80)
#define	REG_B2_RXH_RD_ACC_DBG_MPDU_PTR_DESC_UPD        (B2_RXH_RD_ACC_BASE_ADDRESS + 0x84)
#define	REG_B2_RXH_RD_ACC_DBG_ETH_PTR_DESC_UPD         (B2_RXH_RD_ACC_BASE_ADDRESS + 0x88)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_RXH_RD_ACC_CONFIG_INIT 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdAccEna : 1; //Enable RxH RD-Accelerator. When disabled: legacy (bypass) processing performed., reset value: 0x0, access type: RW
		uint32 alignSourceAdrEn : 1; //enable DMA address alignment to 16 bytes.(RxCB pointer), reset value: 0x0, access type: RW
		uint32 ddrAddrCheckEna : 1; //enable checking if DDR address falls by mistake onto WLAN address space (comparing 6 MS bits to the BAR), reset value: 0x0, access type: RW
		uint32 rdPrefetchEnable : 3; //Each bit enables corresponding Prefetch FIFO fill with free-RDs. , In ------recovery------&return process - disable the prefetch. , 0 - Host Data prefetch enable. , 1 - MGMT Host. , 2 - MGMT FW., reset value: 0x0, access type: RW
		uint32 maxRtdIdx : 3; //Defines RTD Ring final entry index (=> Ring size)., reset value: 0x3, access type: RW
		uint32 rtdGenEn : 1; //Enable filling new RTD words 3..24 (above MSDU), reset value: 0x1, access type: RW
		uint32 descUpdEn : 1; //Enable writing to MPDU desc. and RD memories., reset value: 0x1, access type: RW
		uint32 freeRdCntDecrEn : 1; //enable controlling HostIF freeRD counter (decrement) and busyRD (increment) upon POP of the "data RD" (prefetch #0) - instead of RxH., reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB2RxhRdAccConfigInit_u;

/*REG_B2_RXH_RD_ACC_STATUS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c0NumOfRtd : 3; //Accumulator[0] value: number of RTDs in progress in the Ring, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 c1PendForCpu : 3; //Accumulator[1] value: number of RTDs in the Ring pending for the RxH processing., reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 c2PendForDescUpd : 3; //Accumulator[2 value: number of RTDs in the Ring pending for the Descriptor Update processing (after RxH processing done)., reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 rdPrefetchIntrStatus : 3; //Each bit relates to the corresponding Prefetch core - if it got "FFFFFF" response to POP request from the Qmanager (meaning "list empty")., reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
		uint32 rdPrefetchFifoEmpty : 3; //Each bit displays corresponding Prefetch FIFO "empty" status., reset value: 0x7, access type: RO
		uint32 rdPrefetchFifoFull : 3; //bits[i] - FIFO full flag of Prefetch core #i., reset value: 0x0, access type: RO
		uint32 reserved4 : 9;
		uint32 rdAccActive : 1; //"0" means all internal state-machines are IDLE and it is safe to re-congiure the block (when disabled!), reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccStatus_u;

/*REG_B2_RXH_RD_ACC_RD_PREFETCH_INTR_MASK 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdPrefetchIntrMask : 3; //Bit[i] = ---1--- meaning do NOT produce ---empty_list--- IRQ from Prefetch[i], reset value: 0x7, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxhRdAccRdPrefetchIntrMask_u;

/*REG_B2_RXH_RD_ACC_RD_PREFETCH_INTR_CLEAR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdPrefetchIntrClear : 3; //set '1' to the bit corresponding to the Prefetch core, whose interrupt to be cleared., reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxhRdAccRdPrefetchIntrClear_u;

/*REG_B2_RXH_RD_ACC_C0_INCREMENT 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c0Increment : 1; //[for debug] Increments Accumulator[0] - Number of RTDs in progress in Ring., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC0Increment_u;

/*REG_B2_RXH_RD_ACC_C0_DECREMENT 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c0Decrement : 1; //[for debug] Decrements Accumulator[1] - Number of RTDs in progress in Ring., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC0Decrement_u;

/*REG_B2_RXH_RD_ACC_C1_INCREMENT 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c1Increment : 1; //[for debug] Increments Accumulator[1] - Number of RTDs in Ring, pending for CPU., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC1Increment_u;

/*REG_B2_RXH_RD_ACC_C1_DECREMENT 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c1Decrement : 1; //Write by RxH: Decrements Accumulator[1] - Number of RTDs in Ring, pending for CPU., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC1Decrement_u;

/*REG_B2_RXH_RD_ACC_C2_INCREMENT 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c2Increment : 1; //Write by RxH: Increments Accumulator[2] - Number of RTDs in Ring, pending for RD_update (already processed by the RxH)., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC2Increment_u;

/*REG_B2_RXH_RD_ACC_C2_DECREMENT 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 c2Decrement : 1; //[for debug] Decrements Accumulator[2] - Number of RTDs in Ring, pending for RD_update (already processed by the RxH)., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccC2Decrement_u;

/*REG_B2_RXH_RD_ACC_RTD_COUNTER 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rtdCounter : 32; //Number of RTDs generated by the RD Acc, wrap around., reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccRtdCounter_u;

/*REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT0 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 msduDescPendCnt0 : 32; //Counts MSDU desc FIFO push events, when the FIFO was below 25% full., reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccMsduDescPendCnt0_u;

/*REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT1 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 msduDescPendCnt1 : 32; //Counts MSDU desc FIFO push events, when the FIFO was 25%..50% full., reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccMsduDescPendCnt1_u;

/*REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT2 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 msduDescPendCnt2 : 32; //Counts MSDU desc FIFO push events, when the FIFO was 50%..75% full., reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccMsduDescPendCnt2_u;

/*REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT3 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 msduDescPendCnt3 : 32; //Counts MSDU desc FIFO push events, when the FIFO was above 75% full., reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccMsduDescPendCnt3_u;

/*REG_B2_RXH_RD_ACC_LOGGER_CTRL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerEna : 1; //no description, reset value: 0x0, access type: RW
		uint32 loggerPrio : 2; //no description, reset value: 0x0, access type: RW
		uint32 loggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 loggerMsgEna : 5; //Specific messages enable bits: , Bit #0 ------ enable MSDU desc FIFO Logger message , Bit #1 ------ enable RTD_Gen Logger message , Bit #2 ------ enable Prefetchers Logger message , Bit #3 ------ enable Desc. Upd. Logger message , Bit #4 ------ enable SMC Logger message , , reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2RxhRdAccLoggerCtrl_u;

/*REG_B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT_CLR 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 msduDescPendCntClr : 1; //Clears all MSDU desc FIFO statistics counters., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxhRdAccMsduDescPendCntClr_u;

/*REG_B2_RXH_RD_ACC_DBG_STATE_MACHINES 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgRtdGenSm : 3; //RTD Generator sub-block State Machine., reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dbgDescUpdSm : 4; //Descriptor Update sub-block State Machine., reset value: 0x0, access type: RO
		uint32 dbgPrefetchSm : 6; //bits[1:0] - Prefetch core 0 State Machine. , bits[3:2] - Prefetch core 1 State Machine. , bits[5:4] - Prefetch core 2 State Machine., reset value: 0x0, access type: RO
		uint32 reserved1 : 18;
	} bitFields;
} RegB2RxhRdAccDbgStateMachines_u;

/*REG_B2_RXH_RD_ACC_DBG_ERRORS 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgAccumErr : 3; //Bit per Accumulator. '1' = width exceeded error (underflow or overflow)., reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dbgMsduFifoWasFull : 1; //Sticky. Cleared on rd_acc_ena rise., reset value: 0x0, access type: RO
		uint32 dbgCoordFifoWasFull : 1; //Sticky. Cleared on rd_acc_ena rise. , '1' = RxCoordinator reported its FIFO full when Desc. Update tried to send new MPDU ptr. , reset value: 0x0, access type: RO
		uint32 dbgWasDaSaErr : 1; //sticky bit - will be 1 after the first addr check error, reset value: 0x0, access type: RO
		uint32 dbgWasNotValidMld : 1; //Sticky bit. Goes to '1' if MLD_DB returned valid bit ==0., reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegB2RxhRdAccDbgErrors_u;

/*REG_B2_RXH_RD_ACC_DBG_STATUSES 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPrefWordType : 3; //Bit per Prefetch FIFO: , ------0------ = ------next word is RD ptr------; ------1------ = ------next word is RD.W0mod------, reset value: 0x0, access type: RO
		uint32 dbgRxCoordFifoFull : 1; //RxCoordinator reports its FIFO full, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 dbgRingPtrRtdGen : 8; //Current RTD Ring entry start pointer (in words) - from the RTD Generator sub-block., reset value: 0x0, access type: RO
		uint32 dbgRingPtrDescUpd : 8; //Current RTD Ring entry start pointer (in words) - from the Descriptor Update sub-block., reset value: 0x0, access type: RO
		uint32 dbgMsduFifoUsedw : 8; //Current number of MSDU descriptors in the FIFO (0..130), reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccDbgStatuses_u;

/*REG_B2_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 indexOfRdList0 : 7; //Free RD list index ("Host data") to pop from., reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 indexOfRdList1 : 7; //Free RD list index ("MGMT Host") to pop from., reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 indexOfRdList2 : 7; //Free RD list index ("MGMT FW") to pop from., reset value: 0x0, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegB2RxhRdAccFreeRdListIdxCfg_u;

/*REG_B2_RXH_RD_ACC_DA_SA_CHECK_EN 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 daSaCheckEn : 32; //each bit enables the address check for the corresponding RD_type (refer to the descriptors XLS enum). Default value enables addr. check for Unicast Data frames. Set zero to disable the check completely., reset value: 0x00060009, access type: RW
	} bitFields;
} RegB2RxhRdAccDaSaCheckEn_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_MSB 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref0NxtDataMsb : 32; //Prefetch #0 FIFO next data (MS) = rd_desc_w0[31:3], reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccDbgPref0NxtDataMsb_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_LSB 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref0NxtDataLsb : 26; //Prefetch #0 FIFO next data (LS) = {rd_desc_ptr, 2'b00}, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2RxhRdAccDbgPref0NxtDataLsb_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_MSB 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref1NxtDataMsb : 32; //Prefetch #1 FIFO next data (MS) = {rd_desc_w0[31:3], rd_desc_w3[26:24]}, reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccDbgPref1NxtDataMsb_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_LSB 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref1NxtDataLsb : 26; //Prefetch #1 FIFO next data (LS) = {rd_desc_ptr, 2'b00}, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2RxhRdAccDbgPref1NxtDataLsb_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_MSB 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref2NxtDataMsb : 32; //Prefetch #2 FIFO next data (MS) = {rd_desc_w0[31:3], rd_desc_w3[26:24]}, reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccDbgPref2NxtDataMsb_u;

/*REG_B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_LSB 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgPref2NxtDataLsb : 26; //Prefetch #2 FIFO next data (LS) = {rd_desc_ptr, 2'b00}, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2RxhRdAccDbgPref2NxtDataLsb_u;

/*REG_B2_RXH_RD_ACC_LOGGER_C0_STALLED_LIM_CFG 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerC0StalledLimCfg : 16; //Configures number of clocks of C0 being at MAX value (stalled) to produce Logger message (#1). Next counting will start over when C0 reaches MAX next time., reset value: 0x200, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxhRdAccLoggerC0StalledLimCfg_u;

/*REG_B2_RXH_RD_ACC_DBG_C0_STALLED_MAX_CNT 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgC0StalledMaxCnt : 16; //Debug reg: max value of c0_stalled counter till now. Sampled each c0_max_pulse., reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxhRdAccDbgC0StalledMaxCnt_u;

/*REG_B2_RXH_RD_ACC_RELEASED_RTD_COUNTER 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 releasedRtdCounter : 32; //Counts globally all released RTDs (after Desc, Upd. finish), reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxhRdAccReleasedRtdCounter_u;

/*REG_B2_RXH_RD_ACC_DBG_MPDU_PTR_RTD_GEN 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgMpduPtrRtdGen : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxhRdAccDbgMpduPtrRtdGen_u;

/*REG_B2_RXH_RD_ACC_DBG_ETH_PTR_RTD_GEN 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgEthPtrRtdGen : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2RxhRdAccDbgEthPtrRtdGen_u;

/*REG_B2_RXH_RD_ACC_DBG_MPDU_PTR_DESC_UPD 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgMpduPtrDescUpd : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxhRdAccDbgMpduPtrDescUpd_u;

/*REG_B2_RXH_RD_ACC_DBG_ETH_PTR_DESC_UPD 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgEthPtrDescUpd : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2RxhRdAccDbgEthPtrDescUpd_u;

//========================================
/* REG_RXH_RD_ACC_CONFIG_INIT 0x0 */
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_RD_ACC_ENA_MASK                                                           0x00000001
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_ALIGN_SOURCE_ADR_EN_MASK                                                  0x00000002
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_DDR_ADDR_CHECK_ENA_MASK                                                   0x00000004
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_RD_PREFETCH_ENABLE_MASK                                                   0x00000038
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_MAX_RTD_IDX_MASK                                                          0x000001C0
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_RTD_GEN_EN_MASK                                                           0x00000200
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_DESC_UPD_EN_MASK                                                          0x00000400
#define B2_RXH_RD_ACC_CONFIG_INIT_FIELD_FREE_RD_CNT_DECR_EN_MASK                                                  0x00000800
//========================================
/* REG_RXH_RD_ACC_STATUS 0x4 */
#define B2_RXH_RD_ACC_STATUS_FIELD_C0_NUM_OF_RTD_MASK                                                             0x00000007
#define B2_RXH_RD_ACC_STATUS_FIELD_C1_PEND_FOR_CPU_MASK                                                           0x00000070
#define B2_RXH_RD_ACC_STATUS_FIELD_C2_PEND_FOR_DESC_UPD_MASK                                                      0x00000700
#define B2_RXH_RD_ACC_STATUS_FIELD_RD_PREFETCH_INTR_STATUS_MASK                                                   0x00007000
#define B2_RXH_RD_ACC_STATUS_FIELD_RD_PREFETCH_FIFO_EMPTY_MASK                                                    0x00070000
#define B2_RXH_RD_ACC_STATUS_FIELD_RD_PREFETCH_FIFO_FULL_MASK                                                     0x00380000
#define B2_RXH_RD_ACC_STATUS_FIELD_RD_ACC_ACTIVE_MASK                                                             0x80000000
//========================================
/* REG_RXH_RD_ACC_RD_PREFETCH_INTR_MASK 0x8 */
#define B2_RXH_RD_ACC_RD_PREFETCH_INTR_MASK_FIELD_RD_PREFETCH_INTR_MASK_MASK                                      0x00000007
//========================================
/* REG_RXH_RD_ACC_RD_PREFETCH_INTR_CLEAR 0xC */
#define B2_RXH_RD_ACC_RD_PREFETCH_INTR_CLEAR_FIELD_RD_PREFETCH_INTR_CLEAR_MASK                                    0x00000007
//========================================
/* REG_RXH_RD_ACC_C0_INCREMENT 0x10 */
#define B2_RXH_RD_ACC_C0_INCREMENT_FIELD_C0_INCREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_C0_DECREMENT 0x14 */
#define B2_RXH_RD_ACC_C0_DECREMENT_FIELD_C0_DECREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_C1_INCREMENT 0x18 */
#define B2_RXH_RD_ACC_C1_INCREMENT_FIELD_C1_INCREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_C1_DECREMENT 0x1C */
#define B2_RXH_RD_ACC_C1_DECREMENT_FIELD_C1_DECREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_C2_INCREMENT 0x20 */
#define B2_RXH_RD_ACC_C2_INCREMENT_FIELD_C2_INCREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_C2_DECREMENT 0x24 */
#define B2_RXH_RD_ACC_C2_DECREMENT_FIELD_C2_DECREMENT_MASK                                                        0x00000001
//========================================
/* REG_RXH_RD_ACC_RTD_COUNTER 0x28 */
#define B2_RXH_RD_ACC_RTD_COUNTER_FIELD_RTD_COUNTER_MASK                                                          0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_MSDU_DESC_PEND_CNT0 0x2C */
#define B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT0_FIELD_MSDU_DESC_PEND_CNT0_MASK                                          0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_MSDU_DESC_PEND_CNT1 0x30 */
#define B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT1_FIELD_MSDU_DESC_PEND_CNT1_MASK                                          0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_MSDU_DESC_PEND_CNT2 0x34 */
#define B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT2_FIELD_MSDU_DESC_PEND_CNT2_MASK                                          0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_MSDU_DESC_PEND_CNT3 0x38 */
#define B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT3_FIELD_MSDU_DESC_PEND_CNT3_MASK                                          0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_LOGGER_CTRL 0x3C */
#define B2_RXH_RD_ACC_LOGGER_CTRL_FIELD_LOGGER_ENA_MASK                                                           0x00000001
#define B2_RXH_RD_ACC_LOGGER_CTRL_FIELD_LOGGER_PRIO_MASK                                                          0x00000006
#define B2_RXH_RD_ACC_LOGGER_CTRL_FIELD_LOGGER_BUSY_MASK                                                          0x00000008
#define B2_RXH_RD_ACC_LOGGER_CTRL_FIELD_LOGGER_MSG_ENA_MASK                                                       0x000001F0
//========================================
/* REG_RXH_RD_ACC_MSDU_DESC_PEND_CNT_CLR 0x40 */
#define B2_RXH_RD_ACC_MSDU_DESC_PEND_CNT_CLR_FIELD_MSDU_DESC_PEND_CNT_CLR_MASK                                    0x00000001
//========================================
/* REG_RXH_RD_ACC_DBG_STATE_MACHINES 0x44 */
#define B2_RXH_RD_ACC_DBG_STATE_MACHINES_FIELD_DBG_RTD_GEN_SM_MASK                                                0x00000007
#define B2_RXH_RD_ACC_DBG_STATE_MACHINES_FIELD_DBG_DESC_UPD_SM_MASK                                               0x000000F0
#define B2_RXH_RD_ACC_DBG_STATE_MACHINES_FIELD_DBG_PREFETCH_SM_MASK                                               0x00003F00
//========================================
/* REG_RXH_RD_ACC_DBG_ERRORS 0x48 */
#define B2_RXH_RD_ACC_DBG_ERRORS_FIELD_DBG_ACCUM_ERR_MASK                                                         0x00000007
#define B2_RXH_RD_ACC_DBG_ERRORS_FIELD_DBG_MSDU_FIFO_WAS_FULL_MASK                                                0x00000010
#define B2_RXH_RD_ACC_DBG_ERRORS_FIELD_DBG_COORD_FIFO_WAS_FULL_MASK                                               0x00000020
#define B2_RXH_RD_ACC_DBG_ERRORS_FIELD_DBG_WAS_DA_SA_ERR_MASK                                                     0x00000040
#define B2_RXH_RD_ACC_DBG_ERRORS_FIELD_DBG_WAS_NOT_VALID_MLD_MASK                                                 0x00000080
//========================================
/* REG_RXH_RD_ACC_DBG_STATUSES 0x4C */
#define B2_RXH_RD_ACC_DBG_STATUSES_FIELD_DBG_PREF_WORD_TYPE_MASK                                                  0x00000007
#define B2_RXH_RD_ACC_DBG_STATUSES_FIELD_DBG_RX_COORD_FIFO_FULL_MASK                                              0x00000008
#define B2_RXH_RD_ACC_DBG_STATUSES_FIELD_DBG_RING_PTR_RTD_GEN_MASK                                                0x0000FF00
#define B2_RXH_RD_ACC_DBG_STATUSES_FIELD_DBG_RING_PTR_DESC_UPD_MASK                                               0x00FF0000
#define B2_RXH_RD_ACC_DBG_STATUSES_FIELD_DBG_MSDU_FIFO_USEDW_MASK                                                 0xFF000000
//========================================
/* REG_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG 0x50 */
#define B2_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG_FIELD_INDEX_OF_RD_LIST0_MASK                                           0x0000007F
#define B2_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG_FIELD_INDEX_OF_RD_LIST1_MASK                                           0x00007F00
#define B2_RXH_RD_ACC_FREE_RD_LIST_IDX_CFG_FIELD_INDEX_OF_RD_LIST2_MASK                                           0x007F0000
//========================================
/* REG_RXH_RD_ACC_DA_SA_CHECK_EN 0x54 */
#define B2_RXH_RD_ACC_DA_SA_CHECK_EN_FIELD_DA_SA_CHECK_EN_MASK                                                    0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF0_NXT_DATA_MSB 0x58 */
#define B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_MSB_FIELD_DBG_PREF0_NXT_DATA_MSB_MASK                                    0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF0_NXT_DATA_LSB 0x5C */
#define B2_RXH_RD_ACC_DBG_PREF0_NXT_DATA_LSB_FIELD_DBG_PREF0_NXT_DATA_LSB_MASK                                    0x03FFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF1_NXT_DATA_MSB 0x60 */
#define B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_MSB_FIELD_DBG_PREF1_NXT_DATA_MSB_MASK                                    0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF1_NXT_DATA_LSB 0x64 */
#define B2_RXH_RD_ACC_DBG_PREF1_NXT_DATA_LSB_FIELD_DBG_PREF1_NXT_DATA_LSB_MASK                                    0x03FFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF2_NXT_DATA_MSB 0x68 */
#define B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_MSB_FIELD_DBG_PREF2_NXT_DATA_MSB_MASK                                    0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_PREF2_NXT_DATA_LSB 0x6C */
#define B2_RXH_RD_ACC_DBG_PREF2_NXT_DATA_LSB_FIELD_DBG_PREF2_NXT_DATA_LSB_MASK                                    0x03FFFFFF
//========================================
/* REG_RXH_RD_ACC_LOGGER_C0_STALLED_LIM_CFG 0x70 */
#define B2_RXH_RD_ACC_LOGGER_C0_STALLED_LIM_CFG_FIELD_LOGGER_C0_STALLED_LIM_CFG_MASK                              0x0000FFFF
//========================================
/* REG_RXH_RD_ACC_DBG_C0_STALLED_MAX_CNT 0x74 */
#define B2_RXH_RD_ACC_DBG_C0_STALLED_MAX_CNT_FIELD_DBG_C0_STALLED_MAX_CNT_MASK                                    0x0000FFFF
//========================================
/* REG_RXH_RD_ACC_RELEASED_RTD_COUNTER 0x78 */
#define B2_RXH_RD_ACC_RELEASED_RTD_COUNTER_FIELD_RELEASED_RTD_COUNTER_MASK                                        0xFFFFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_MPDU_PTR_RTD_GEN 0x7C */
#define B2_RXH_RD_ACC_DBG_MPDU_PTR_RTD_GEN_FIELD_DBG_MPDU_PTR_RTD_GEN_MASK                                        0x0000FFFF
//========================================
/* REG_RXH_RD_ACC_DBG_ETH_PTR_RTD_GEN 0x80 */
#define B2_RXH_RD_ACC_DBG_ETH_PTR_RTD_GEN_FIELD_DBG_ETH_PTR_RTD_GEN_MASK                                          0x03FFFFFF
//========================================
/* REG_RXH_RD_ACC_DBG_MPDU_PTR_DESC_UPD 0x84 */
#define B2_RXH_RD_ACC_DBG_MPDU_PTR_DESC_UPD_FIELD_DBG_MPDU_PTR_DESC_UPD_MASK                                      0x0000FFFF
//========================================
/* REG_RXH_RD_ACC_DBG_ETH_PTR_DESC_UPD 0x88 */
#define B2_RXH_RD_ACC_DBG_ETH_PTR_DESC_UPD_FIELD_DBG_ETH_PTR_DESC_UPD_MASK                                        0x03FFFFFF


#endif // _RXH_RD_ACC_REGS_H_
