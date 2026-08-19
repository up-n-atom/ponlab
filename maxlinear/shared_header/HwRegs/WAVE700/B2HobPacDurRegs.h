
/***********************************************************************************
File:				B2HobPacDurRegs.h
Module:				b2HobPacDur
SOC Revision:		latest
Generated at:       2022-05-04 10:58:06
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_HOB_PAC_DUR_REGS_H_
#define _B2_HOB_PAC_DUR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_HOB_PAC_DUR_BASE_ADDRESS                             MEMORY_MAP_UNIT_20500_BASE_ADDRESS
#define	REG_B2_HOB_PAC_DUR_CF_ACK_SFR                                     (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x10)
#define	REG_B2_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW                       (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x14)
#define	REG_B2_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH                      (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x18)
#define	REG_B2_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE                           (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x20)
#define	REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR                                 (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x50)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR_LOW                              (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x54)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x58)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x5C)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x60)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x64)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x68)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x6C)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x70)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xB0)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xB4)
#define	REG_B2_HOB_PAC_DUR_TSF_OFFSET_INDEX                               (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xB8)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xBC)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xC0)
#define	REG_B2_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR                       (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xC4)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW                             (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xC8)
#define	REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH                            (B2_HOB_PAC_DUR_BASE_ADDRESS + 0xCC)
#define	REG_B2_HOB_PAC_DUR_RX_ADDR1_IDX                                   (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x104)
#define	REG_B2_HOB_PAC_DUR_TX_MPDU_DUR_FIELD                              (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x108)
#define	REG_B2_HOB_PAC_DUR_TX_ADDR3_SELECT                                (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x10C)
#define	REG_B2_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS              (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x110)
#define	REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS              (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x114)
#define	REG_B2_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER                           (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x118)
#define	REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS    (B2_HOB_PAC_DUR_BASE_ADDRESS + 0x11C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_HOB_PAC_DUR_CF_ACK_SFR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 pmSfr : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 19;
	} bitFields;
} RegB2HobPacDurCfAckSfr_u;

/*REG_B2_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurLegacyTsfFieldSfrLow_u;

/*REG_B2_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurLegacyTsfFieldSfrHigh_u;

/*REG_B2_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 6;
		uint32 txopJumpTableBase : 18; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB2HobPacDurTxopJumpTableBase_u;

/*REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDur : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2HobPacDurAutoReplyDur_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR_LOW 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfrLow_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfrHigh_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr1Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr1Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr1High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr1High_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr2Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr2Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr2High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr2High_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr3Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr3Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr3High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr3High_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr4Low : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB2HobPacDurTsfFieldSfr4Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr4High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr4High_u;

/*REG_B2_HOB_PAC_DUR_TSF_OFFSET_INDEX 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfOffsetIndex : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB2HobPacDurTsfOffsetIndex_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr5Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr5Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr5High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr5High_u;

/*REG_B2_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyBfReportDur : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2HobPacDurAutoReplyBfReportDur_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr6Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr6Low_u;

/*REG_B2_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr6High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2HobPacDurTsfFieldSfr6High_u;

/*REG_B2_HOB_PAC_DUR_RX_ADDR1_IDX 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxAddr1Idx : 5; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB2HobPacDurRxAddr1Idx_u;

/*REG_B2_HOB_PAC_DUR_TX_MPDU_DUR_FIELD 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDurField : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2HobPacDurTxMpduDurField_u;

/*REG_B2_HOB_PAC_DUR_TX_ADDR3_SELECT 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr3Select : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2HobPacDurTxAddr3Select_u;

/*REG_B2_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerAtTxEnRise15Bits : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB2HobPacDurTxNavTimerAtTxEnRise15Bits_u;

/*REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDurNoAirTime15Bits : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB2HobPacDurAutoReplyDurNoAirTime15Bits_u;

/*REG_B2_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxPhyStatusBufferId : 1; //Rx PHY status buffer ID of the last Rx session, reset value: 0x0, access type: RO
		uint32 rxPhyStatusBufferValid : 1; //Rx PHY status buffer valid of the last Rx session, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2HobPacDurRxPhyStatusBuffer_u;

/*REG_B2_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDurBfReportNoAirTime15Bits : 15; //Auto-reply BF report duration without the Air time of Tx BF report - used for TCR TXOP duration field, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB2HobPacDurAutoReplyDurBfReportNoAirTime15Bits_u;



#endif // _B2_HOB_PAC_DUR_REGS_H_
