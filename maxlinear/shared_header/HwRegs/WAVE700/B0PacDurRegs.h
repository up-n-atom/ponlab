
/***********************************************************************************
File:				B0PacDurRegs.h
Module:				b0PacDur
SOC Revision:		latest
Generated at:       2022-05-04 10:57:13
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PAC_DUR_REGS_H_
#define _B0_PAC_DUR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PAC_DUR_BASE_ADDRESS                             MEMORY_MAP_UNIT_10_BASE_ADDRESS
#define	REG_B0_PAC_DUR_MISC                           (B0_PAC_DUR_BASE_ADDRESS + 0xC)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME                 (B0_PAC_DUR_BASE_ADDRESS + 0x1C)
#define	REG_B0_PAC_DUR_TXOP_JUMP_TABLE_BASE           (B0_PAC_DUR_BASE_ADDRESS + 0x50)
#define	REG_B0_PAC_DUR_TX_MPDU_DUR_FIELD              (B0_PAC_DUR_BASE_ADDRESS + 0x54)
#define	REG_B0_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE     (B0_PAC_DUR_BASE_ADDRESS + 0x58)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME6P               (B0_PAC_DUR_BASE_ADDRESS + 0x60)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME6M               (B0_PAC_DUR_BASE_ADDRESS + 0x64)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME1                (B0_PAC_DUR_BASE_ADDRESS + 0x84)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME2                (B0_PAC_DUR_BASE_ADDRESS + 0x88)
#define	REG_B0_PAC_DUR_TSF_FIELD_TIME3                (B0_PAC_DUR_BASE_ADDRESS + 0x8C)
#define	REG_B0_PAC_DUR_PHY_PAC_TX_AIR_TIME_RESULT     (B0_PAC_DUR_BASE_ADDRESS + 0x94)
#define	REG_B0_PAC_DUR_DURATION_CALCULATION_RESULT    (B0_PAC_DUR_BASE_ADDRESS + 0x9C)
#define	REG_B0_PAC_DUR_TSF_ADJUST_DATA_IN             (B0_PAC_DUR_BASE_ADDRESS + 0xA0)
#define	REG_B0_PAC_DUR_TX_AIR_TIME_CTR_WR_VALUE       (B0_PAC_DUR_BASE_ADDRESS + 0xA4)
#define	REG_B0_PAC_DUR_TX_AIR_TIME_CTR                (B0_PAC_DUR_BASE_ADDRESS + 0xA8)
#define	REG_B0_PAC_DUR_TSF_OFFSET_INDEX               (B0_PAC_DUR_BASE_ADDRESS + 0xF4)
#define	REG_B0_PAC_DUR_DUR_TIMER_COMPENSATION         (B0_PAC_DUR_BASE_ADDRESS + 0xF8)
#define	REG_B0_PAC_DUR_TX_FRAME_ADDR3_SELECT_VAP      (B0_PAC_DUR_BASE_ADDRESS + 0xFC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PAC_DUR_MISC 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerMgmt : 1; //DUR  power mgmt, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDurMisc_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime : 9; //DUR  tsf field time, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PacDurTsfFieldTime_u;

/*REG_B0_PAC_DUR_TXOP_JUMP_TABLE_BASE 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 6;
		uint32 txopJumpTableBase : 18; //TXOP jump table base, reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegB0PacDurTxopJumpTableBase_u;

/*REG_B0_PAC_DUR_TX_MPDU_DUR_FIELD 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDurField : 16; //Tx MPDU duration field, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PacDurTxMpduDurField_u;

/*REG_B0_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerAtTxEnRise : 18; //Sample Tx NAV at the rise of MAC-PHY Tx enable, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PacDurTxNavTimerAtTxEnRise_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME6P 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime6P : 28; //constant offset to add to calculated tsf_field_time6 , reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB0PacDurTsfFieldTime6P_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME6M 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime6M : 28; //constant offset to substract from calculated tsf_field_time6 , reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB0PacDurTsfFieldTime6M_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime1 : 25; //TSF field time 1 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB0PacDurTsfFieldTime1_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime2 : 25; //TSF field time 2 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB0PacDurTsfFieldTime2_u;

/*REG_B0_PAC_DUR_TSF_FIELD_TIME3 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime3 : 25; //TSF field time 3 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB0PacDurTsfFieldTime3_u;

/*REG_B0_PAC_DUR_PHY_PAC_TX_AIR_TIME_RESULT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacTxAirTimeResult : 17; //PHY-MAC Tx air time result, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB0PacDurPhyPacTxAirTimeResult_u;

/*REG_B0_PAC_DUR_DURATION_CALCULATION_RESULT 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDur : 16; //duration calculation result, reset value: 0x0, access type: RO
		uint32 autoReplyBfReportDur : 16; //BF report duration calculation result, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDurDurationCalculationResult_u;

/*REG_B0_PAC_DUR_TSF_ADJUST_DATA_IN 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfAdjustDataIn : 24; //tsf adjust data in, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PacDurTsfAdjustDataIn_u;

/*REG_B0_PAC_DUR_TX_AIR_TIME_CTR_WR_VALUE 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAirTimeCtrWrValue : 28; //Tx air time write value, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB0PacDurTxAirTimeCtrWrValue_u;

/*REG_B0_PAC_DUR_TX_AIR_TIME_CTR 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAirTimeCtr : 28; //Tx air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB0PacDurTxAirTimeCtr_u;

/*REG_B0_PAC_DUR_TSF_OFFSET_INDEX 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfOffsetIndex : 5; //TSF offset index from HOB, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PacDurTsfOffsetIndex_u;

/*REG_B0_PAC_DUR_DUR_TIMER_COMPENSATION 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 durTimerRxrdyCompensation : 5; //Dur timer RxRdy compensation, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 durTimerTxenCompensation : 5; //Dur timer TxEn compensation, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegB0PacDurDurTimerCompensation_u;

/*REG_B0_PAC_DUR_TX_FRAME_ADDR3_SELECT_VAP 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txFrameAddr3SelectVap : 32; //Tx frame address 3 selection, 1bit per VAP: , 0: the same as address2 (i.e. AP mode). , 1: the same as address1 (i.e. STA mode). , , reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDurTxFrameAddr3SelectVap_u;



#endif // _B0_PAC_DUR_REGS_H_
