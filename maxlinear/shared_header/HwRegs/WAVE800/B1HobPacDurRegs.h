
/***********************************************************************************
File:				B1HobPacDurRegs.h
Module:				b1HobPacDur
SOC Revision:		latest
Generated at:       2024-06-26 12:54:37
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_HOB_PAC_DUR_REGS_H_
#define _B1_HOB_PAC_DUR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_HOB_PAC_DUR_BASE_ADDRESS                                  MEMORY_MAP_UNIT_10500_BASE_ADDRESS
#define	REG_B1_HOB_PAC_DUR_CF_ACK_SFR                                     (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x10)
#define	REG_B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW                       (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x14)
#define	REG_B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH                      (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x18)
#define	REG_B1_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE                           (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x20)
#define	REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR                                 (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x50)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR_LOW                              (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x54)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x58)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x5C)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x60)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x64)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x68)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x6C)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x70)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xB0)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xB4)
#define	REG_B1_HOB_PAC_DUR_TSF_OFFSET_INDEX                               (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xB8)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xBC)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xC0)
#define	REG_B1_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR                       (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xC4)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW                             (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xC8)
#define	REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH                            (B1_HOB_PAC_DUR_BASE_ADDRESS + 0xCC)
#define	REG_B1_HOB_PAC_DUR_RX_ADDR1_IDX                                   (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x104)
#define	REG_B1_HOB_PAC_DUR_TX_MPDU_DUR_FIELD                              (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x108)
#define	REG_B1_HOB_PAC_DUR_TX_ADDR3_SELECT                                (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x10C)
#define	REG_B1_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS              (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x110)
#define	REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS              (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x114)
#define	REG_B1_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER                           (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x118)
#define	REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS    (B1_HOB_PAC_DUR_BASE_ADDRESS + 0x11C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_HOB_PAC_DUR_CF_ACK_SFR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 pmSfr : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 19;
	} bitFields;
} RegB1HobPacDurCfAckSfr_u;

/*REG_B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurLegacyTsfFieldSfrLow_u;

/*REG_B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurLegacyTsfFieldSfrHigh_u;

/*REG_B1_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 6;
		uint32 txopJumpTableBase : 18; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1HobPacDurTxopJumpTableBase_u;

/*REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDur : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1HobPacDurAutoReplyDur_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR_LOW 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfrLow_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfrHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfrHigh_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr1Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr1Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr1High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr1High_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr2Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr2Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr2High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr2High_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr3Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr3Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr3High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr3High_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr4Low : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB1HobPacDurTsfFieldSfr4Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr4High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr4High_u;

/*REG_B1_HOB_PAC_DUR_TSF_OFFSET_INDEX 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfOffsetIndex : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1HobPacDurTsfOffsetIndex_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr5Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr5Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr5High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr5High_u;

/*REG_B1_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyBfReportDur : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1HobPacDurAutoReplyBfReportDur_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr6Low : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr6Low_u;

/*REG_B1_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfFieldSfr6High : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobPacDurTsfFieldSfr6High_u;

/*REG_B1_HOB_PAC_DUR_RX_ADDR1_IDX 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxAddr1Idx : 5; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1HobPacDurRxAddr1Idx_u;

/*REG_B1_HOB_PAC_DUR_TX_MPDU_DUR_FIELD 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDurField : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1HobPacDurTxMpduDurField_u;

/*REG_B1_HOB_PAC_DUR_TX_ADDR3_SELECT 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr3Select : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1HobPacDurTxAddr3Select_u;

/*REG_B1_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerAtTxEnRise15Bits : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1HobPacDurTxNavTimerAtTxEnRise15Bits_u;

/*REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDurNoAirTime15Bits : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1HobPacDurAutoReplyDurNoAirTime15Bits_u;

/*REG_B1_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxPhyStatusBufferId : 1; //Rx PHY status buffer ID of the last Rx session, reset value: 0x0, access type: RO
		uint32 rxPhyStatusBufferValid : 1; //Rx PHY status buffer valid of the last Rx session, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1HobPacDurRxPhyStatusBuffer_u;

/*REG_B1_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyDurBfReportNoAirTime15Bits : 15; //Auto-reply BF report duration without the Air time of Tx BF report - used for TCR TXOP duration field, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1HobPacDurAutoReplyDurBfReportNoAirTime15Bits_u;

//========================================
/* REG_HOB_PAC_DUR_CF_ACK_SFR 0x10 */
#define B1_HOB_PAC_DUR_CF_ACK_SFR_FIELD_PM_SFR_MASK                                                               0x00001000
//========================================
/* REG_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW 0x14 */
#define B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_LOW_FIELD_TSF_FIELD_SFR_LOW_MASK                                      0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH 0x18 */
#define B1_HOB_PAC_DUR_LEGACY_TSF_FIELD_SFR_HIGH_FIELD_TSF_FIELD_SFR_HIGH_MASK                                    0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE 0x20 */
#define B1_HOB_PAC_DUR_TXOP_JUMP_TABLE_BASE_FIELD_TXOP_JUMP_TABLE_BASE_MASK                                       0x00FFFFC0
//========================================
/* REG_HOB_PAC_DUR_AUTO_REPLY_DUR 0x50 */
#define B1_HOB_PAC_DUR_AUTO_REPLY_DUR_FIELD_AUTO_REPLY_DUR_MASK                                                   0x0000FFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR_LOW 0x54 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR_LOW_FIELD_TSF_FIELD_SFR_LOW_MASK                                             0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH 0x58 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR_HIGH_FIELD_TSF_FIELD_SFR_HIGH_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW 0x5C */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR1_LOW_FIELD_TSF_FIELD_SFR1_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH 0x60 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR1_HIGH_FIELD_TSF_FIELD_SFR1_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW 0x64 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR2_LOW_FIELD_TSF_FIELD_SFR2_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH 0x68 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR2_HIGH_FIELD_TSF_FIELD_SFR2_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW 0x6C */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR3_LOW_FIELD_TSF_FIELD_SFR3_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH 0x70 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR3_HIGH_FIELD_TSF_FIELD_SFR3_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW 0xB0 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR4_LOW_FIELD_TSF_FIELD_SFR4_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH 0xB4 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR4_HIGH_FIELD_TSF_FIELD_SFR4_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_OFFSET_INDEX 0xB8 */
#define B1_HOB_PAC_DUR_TSF_OFFSET_INDEX_FIELD_TSF_OFFSET_INDEX_MASK                                               0x0000001F
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW 0xBC */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR5_LOW_FIELD_TSF_FIELD_SFR5_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH 0xC0 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR5_HIGH_FIELD_TSF_FIELD_SFR5_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR 0xC4 */
#define B1_HOB_PAC_DUR_AUTO_REPLY_BF_REPORT_DUR_FIELD_AUTO_REPLY_BF_REPORT_DUR_MASK                               0x0000FFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW 0xC8 */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR6_LOW_FIELD_TSF_FIELD_SFR6_LOW_MASK                                           0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH 0xCC */
#define B1_HOB_PAC_DUR_TSF_FIELD_SFR6_HIGH_FIELD_TSF_FIELD_SFR6_HIGH_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_PAC_DUR_RX_ADDR1_IDX 0x104 */
#define B1_HOB_PAC_DUR_RX_ADDR1_IDX_FIELD_RX_ADDR1_IDX_MASK                                                       0x0000001F
//========================================
/* REG_HOB_PAC_DUR_TX_MPDU_DUR_FIELD 0x108 */
#define B1_HOB_PAC_DUR_TX_MPDU_DUR_FIELD_FIELD_TX_MPDU_DUR_FIELD_MASK                                             0x0000FFFF
//========================================
/* REG_HOB_PAC_DUR_TX_ADDR3_SELECT 0x10C */
#define B1_HOB_PAC_DUR_TX_ADDR3_SELECT_FIELD_TX_ADDR3_SELECT_MASK                                                 0x00000001
//========================================
/* REG_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS 0x110 */
#define B1_HOB_PAC_DUR_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS_FIELD_TX_NAV_TIMER_AT_TX_EN_RISE_15BITS_MASK             0x00007FFF
//========================================
/* REG_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS 0x114 */
#define B1_HOB_PAC_DUR_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS_FIELD_AUTO_REPLY_DUR_NO_AIR_TIME_15BITS_MASK             0x00007FFF
//========================================
/* REG_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER 0x118 */
#define B1_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER_FIELD_RX_PHY_STATUS_BUFFER_ID_MASK                                    0x00000001
#define B1_HOB_PAC_DUR_RX_PHY_STATUS_BUFFER_FIELD_RX_PHY_STATUS_BUFFER_VALID_MASK                                 0x00000002
//========================================
/* REG_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS 0x11C */
#define B1_HOB_PAC_DUR_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS_FIELD_AUTO_REPLY_DUR_BF_REPORT_NO_AIR_TIME_15BITS_MASK 0x00007FFF


#endif // _HOB_PAC_DUR_REGS_H_
