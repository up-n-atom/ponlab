
/***********************************************************************************
File:				B1PacDurRegs.h
Module:				b1PacDur
SOC Revision:		latest
Generated at:       2022-05-04 10:57:43
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_PAC_DUR_REGS_H_
#define _B1_PAC_DUR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_PAC_DUR_BASE_ADDRESS                             MEMORY_MAP_UNIT_10010_BASE_ADDRESS
#define	REG_B1_PAC_DUR_MISC                           (B1_PAC_DUR_BASE_ADDRESS + 0xC)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME                 (B1_PAC_DUR_BASE_ADDRESS + 0x1C)
#define	REG_B1_PAC_DUR_TXOP_JUMP_TABLE_BASE           (B1_PAC_DUR_BASE_ADDRESS + 0x50)
#define	REG_B1_PAC_DUR_TX_MPDU_DUR_FIELD              (B1_PAC_DUR_BASE_ADDRESS + 0x54)
#define	REG_B1_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE     (B1_PAC_DUR_BASE_ADDRESS + 0x58)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME6P               (B1_PAC_DUR_BASE_ADDRESS + 0x60)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME6M               (B1_PAC_DUR_BASE_ADDRESS + 0x64)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME1                (B1_PAC_DUR_BASE_ADDRESS + 0x84)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME2                (B1_PAC_DUR_BASE_ADDRESS + 0x88)
#define	REG_B1_PAC_DUR_TSF_FIELD_TIME3                (B1_PAC_DUR_BASE_ADDRESS + 0x8C)
#define	REG_B1_PAC_DUR_PHY_PAC_TX_AIR_TIME_RESULT     (B1_PAC_DUR_BASE_ADDRESS + 0x94)
#define	REG_B1_PAC_DUR_DURATION_CALCULATION_RESULT    (B1_PAC_DUR_BASE_ADDRESS + 0x9C)
#define	REG_B1_PAC_DUR_TSF_ADJUST_DATA_IN             (B1_PAC_DUR_BASE_ADDRESS + 0xA0)
#define	REG_B1_PAC_DUR_TX_AIR_TIME_CTR_WR_VALUE       (B1_PAC_DUR_BASE_ADDRESS + 0xA4)
#define	REG_B1_PAC_DUR_TX_AIR_TIME_CTR                (B1_PAC_DUR_BASE_ADDRESS + 0xA8)
#define	REG_B1_PAC_DUR_TSF_OFFSET_INDEX               (B1_PAC_DUR_BASE_ADDRESS + 0xF4)
#define	REG_B1_PAC_DUR_DUR_TIMER_COMPENSATION         (B1_PAC_DUR_BASE_ADDRESS + 0xF8)
#define	REG_B1_PAC_DUR_TX_FRAME_ADDR3_SELECT_VAP      (B1_PAC_DUR_BASE_ADDRESS + 0xFC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_PAC_DUR_MISC 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerMgmt : 1; //DUR  power mgmt, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDurMisc_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime : 9; //DUR  tsf field time, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1PacDurTsfFieldTime_u;

/*REG_B1_PAC_DUR_TXOP_JUMP_TABLE_BASE 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 6;
		uint32 txopJumpTableBase : 18; //TXOP jump table base, reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegB1PacDurTxopJumpTableBase_u;

/*REG_B1_PAC_DUR_TX_MPDU_DUR_FIELD 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDurField : 16; //Tx MPDU duration field, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PacDurTxMpduDurField_u;

/*REG_B1_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerAtTxEnRise : 18; //Sample Tx NAV at the rise of MAC-PHY Tx enable, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PacDurTxNavTimerAtTxEnRise_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME6P 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime6P : 28; //constant offset to add to calculated tsf_field_time6 , reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB1PacDurTsfFieldTime6P_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME6M 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime6M : 28; //constant offset to substract from calculated tsf_field_time6 , reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB1PacDurTsfFieldTime6M_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime1 : 25; //TSF field time 1 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1PacDurTsfFieldTime1_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime2 : 25; //TSF field time 2 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1PacDurTsfFieldTime2_u;

/*REG_B1_PAC_DUR_TSF_FIELD_TIME3 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldTime3 : 25; //TSF field time 3 (multiple BSSID), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1PacDurTsfFieldTime3_u;

/*REG_B1_PAC_DUR_PHY_PAC_TX_AIR_TIME_RESULT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacTxAirTimeResult : 17; //PHY-MAC Tx air time result, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1PacDurPhyPacTxAirTimeResult_u;

/*REG_B1_PAC_DUR_DURATION_CALCULATION_RESULT 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDur : 16; //duration calculation result, reset value: 0x0, access type: RO
		uint32 autoReplyBfReportDur : 16; //BF report duration calculation result, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDurDurationCalculationResult_u;

/*REG_B1_PAC_DUR_TSF_ADJUST_DATA_IN 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfAdjustDataIn : 24; //tsf adjust data in, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB1PacDurTsfAdjustDataIn_u;

/*REG_B1_PAC_DUR_TX_AIR_TIME_CTR_WR_VALUE 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAirTimeCtrWrValue : 28; //Tx air time write value, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB1PacDurTxAirTimeCtrWrValue_u;

/*REG_B1_PAC_DUR_TX_AIR_TIME_CTR 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAirTimeCtr : 28; //Tx air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1PacDurTxAirTimeCtr_u;

/*REG_B1_PAC_DUR_TSF_OFFSET_INDEX 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfOffsetIndex : 5; //TSF offset index from HOB, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1PacDurTsfOffsetIndex_u;

/*REG_B1_PAC_DUR_DUR_TIMER_COMPENSATION 0xF8 */
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
} RegB1PacDurDurTimerCompensation_u;

/*REG_B1_PAC_DUR_TX_FRAME_ADDR3_SELECT_VAP 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txFrameAddr3SelectVap : 32; //Tx frame address 3 selection, 1bit per VAP: , 0: the same as address2 (i.e. AP mode). , 1: the same as address1 (i.e. STA mode). , , reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDurTxFrameAddr3SelectVap_u;



#endif // _B1_PAC_DUR_REGS_H_
