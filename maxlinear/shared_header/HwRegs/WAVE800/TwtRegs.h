
/***********************************************************************************
File:				TwtRegs.h
Module:				twt
SOC Revision:		latest
Generated at:       2024-06-26 12:55:22
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TWT_REGS_H_
#define _TWT_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TWT_BASE_ADDRESS                               MEMORY_MAP_UNIT_56_BASE_ADDRESS
#define	REG_TWT_TX_TWT_SW_HALT                   (TWT_BASE_ADDRESS + 0x0)
#define	REG_TWT_TX_TWT_GEN_CFG                   (TWT_BASE_ADDRESS + 0x4)
#define	REG_TWT_TX_TWT_SP_GROUP_UPDATE           (TWT_BASE_ADDRESS + 0x8)
#define	REG_TWT_TX_TWT_SP_GROUP_START_TSF        (TWT_BASE_ADDRESS + 0xC)
#define	REG_TWT_TX_TWT_SP_GROUP_LOW_PHASE        (TWT_BASE_ADDRESS + 0x10)
#define	REG_TWT_TX_TWT_SP_STA_UPDATE             (TWT_BASE_ADDRESS + 0x14)
#define	REG_TWT_TX_TWT_SP_GROUP_DEACTIVATE       (TWT_BASE_ADDRESS + 0x1C)
#define	REG_TWT_TX_TWT_FSM_STATUS                (TWT_BASE_ADDRESS + 0x20)
#define	REG_TWT_TX_TWT_STA_EOSP_SENT             (TWT_BASE_ADDRESS + 0x2C)
#define	REG_TWT_TX_TWT_SP_GROUP_VALID0           (TWT_BASE_ADDRESS + 0x30)
#define	REG_TWT_TX_TWT_SP_GROUP_VALID1           (TWT_BASE_ADDRESS + 0x34)
#define	REG_TWT_TX_TWT_SP_GROUP_VALID2           (TWT_BASE_ADDRESS + 0x38)
#define	REG_TWT_TX_TWT_SP_GROUP_VALID3           (TWT_BASE_ADDRESS + 0x3C)
#define	REG_TWT_TX_STA_SEL_CTRL                  (TWT_BASE_ADDRESS + 0x40)
#define	REG_TWT_TWT_FIFO_BASE_ADDR               (TWT_BASE_ADDRESS + 0x50)
#define	REG_TWT_TWT_FIFO_DEPTH_MINUS_ONE         (TWT_BASE_ADDRESS + 0x54)
#define	REG_TWT_TWT_FIFO_CLEAR_STRB              (TWT_BASE_ADDRESS + 0x58)
#define	REG_TWT_TWT_FIFO_RD_ENTRIES_NUM          (TWT_BASE_ADDRESS + 0x5C)
#define	REG_TWT_TWT_FIFO_NUM_ENTRIES_COUNT       (TWT_BASE_ADDRESS + 0x60)
#define	REG_TWT_TWT_FIFO_DEBUG                   (TWT_BASE_ADDRESS + 0x64)
#define	REG_TWT_TWT_FIFO_EN                      (TWT_BASE_ADDRESS + 0x68)
#define	REG_TWT_TX_TWT_ISR                       (TWT_BASE_ADDRESS + 0x70)
#define	REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS       (TWT_BASE_ADDRESS + 0x80)
#define	REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO    (TWT_BASE_ADDRESS + 0x84)
#define	REG_TWT_TX_TWT_NEXT_ACTIVE_SP_RO         (TWT_BASE_ADDRESS + 0x88)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TWT_TX_TWT_SW_HALT 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtHalt : 1; //Enables update of twt_sp field, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegTwtTxTwtSwHalt_u;

/*REG_TWT_TX_TWT_GEN_CFG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMuEospMinCalc : 1; //Select logic to calculate EOSP in TWT. 1'b0 - W600-2 logic. 1'b1 - minimum between all active groups , , reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 swEospAlgorithmSel : 2; //select between eosp calc methodology: , 0 - TBD , 1 - max between unannounce and announce , 2 - max between unannounce and announce , 3 - TBD , , reset value: 0x1, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegTwtTxTwtGenCfg_u;

/*REG_TWT_TX_TWT_SP_GROUP_UPDATE 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpHighLimit : 14; //High phase of the SP. Values are in 128uS resolution., reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
		uint32 swTwtSpGroupIdx : 7; //SP Group index, reset value: 0x0, access type: RW
		uint32 twtSpUpdateDone : 1; //Instruction done indication, reset value: 0x1, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupUpdate_u;

/*REG_TWT_TX_TWT_SP_GROUP_START_TSF 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpStartTsf : 32; //Start TSF value. 32 LSB bit only (Out of the full 64 bits), reset value: 0x0, access type: RW
	} bitFields;
} RegTwtTxTwtSpGroupStartTsf_u;

/*REG_TWT_TX_TWT_SP_GROUP_LOW_PHASE 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpLowLimit : 22; //Low phase of the SP. Values are in 128uS resolution., reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 swTsfStartTimeHigh : 1; //Keep this 1'b1 for proper functionality, reset value: 0x1, access type: RW
		uint32 swTwtSpGroupUpdateActSwFifo : 1; //1'b0 - don't update the software fifo when this group goes to active , 1'b1 - update the software fifo when this group goes to active, reset value: 0x0, access type: RW
		uint32 swTwtSpGroupUpdateActSelector : 1; //1'b0 - don't update the selector bit plan_selection_without_data when this group goes to active , 1'b1 - update the selector bit plan_selection_without_data when this group goes to active, reset value: 0x0, access type: RW
		uint32 swTwtAnnounceValue : 1; //Set if this group is Announced or unannounced , 0b1 - this group is announced , 0b0 - this group is unannounced , , reset value: 0x0, access type: RW
	} bitFields;
} RegTwtTxTwtSpGroupLowPhase_u;

/*REG_TWT_TX_TWT_SP_STA_UPDATE 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpStaGroupIdx : 7; //SP Group index, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 swTwtSpStaIdx : 8; //STA index, reset value: 0x0, access type: RW
		uint32 swTwtSpStaValue : 1; //Connectd/Disconnect STA from group, reset value: 0x0, access type: RW
		uint32 reserved1 : 14;
		uint32 twtSpStaUpdateDone : 1; //Instruction done indication, reset value: 0x1, access type: RO
	} bitFields;
} RegTwtTxTwtSpStaUpdate_u;

/*REG_TWT_TX_TWT_SP_GROUP_DEACTIVATE 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpDeactivateGroupIdx : 7; //SP group index, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
		uint32 twtSpDeactivateDone : 1; //Instruction done indication, reset value: 0x1, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupDeactivate_u;

/*REG_TWT_TX_TWT_FSM_STATUS 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpMainSm : 5; //Main SM state, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 groupClearPendingRequest : 1; //sticky event to tell that there is a pending request to clear a group, reset value: 0x0, access type: RO
		uint32 groupInitPendingRequest : 1; //sticky event to tell that there is a pending request to init a group, reset value: 0x0, access type: RO
		uint32 periodicUpdatePendingRequest : 1; //sticky event to tell that there is a pending request to perform periodic SP update, reset value: 0x0, access type: RO
		uint32 reserved1 : 21;
	} bitFields;
} RegTwtTxTwtFsmStatus_u;

/*REG_TWT_TX_TWT_STA_EOSP_SENT 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swStaEospSent : 8; //Fix WLANVLSIIP-4926, SW should write the STA ID to this field whenever we transmit EOSP to a STA that eventually cause BAA to clear the SP bit in selector, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegTwtTxTwtStaEospSent_u;

/*REG_TWT_TX_TWT_SP_GROUP_VALID0 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid0 : 32; //Enabled SP groups - group number 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupValid0_u;

/*REG_TWT_TX_TWT_SP_GROUP_VALID1 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid1 : 32; //Enabled SP groups - group number 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupValid1_u;

/*REG_TWT_TX_TWT_SP_GROUP_VALID2 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid2 : 32; //Enabled SP groups - group number 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupValid2_u;

/*REG_TWT_TX_TWT_SP_GROUP_VALID3 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid3 : 32; //Enabled SP groups - group number 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegTwtTxTwtSpGroupValid3_u;

/*REG_TWT_TX_STA_SEL_CTRL 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swStaSelUpdateSelectorDbAnnounced : 1; //This bit selects whether to update the selector DB bit STA SEL in case a group that marked as "Update selector DB" and is Announced is set for this station, reset value: 0x0, access type: RW
		uint32 swStaSelUpdateSelectorDbUnannounced : 1; //This bit selects whether to update the selector DB bit STA SEL in case a group that marked as "Update selector DB" and is Unannounced is set for this station, reset value: 0x0, access type: RW
		uint32 swStaSelUpdateSwFifoAnnounced : 1; //This bit selects whether to update the External SW Fifo in case a group that marked as "Update SW Fifo" and is Announced is set for this station, reset value: 0x0, access type: RW
		uint32 swStaSelUpdateSwFifoUnannounced : 1; //This bit selects whether to update the External SW Fifo in case a group that marked as "Update SW Fifo" and is Unannounced is set for this station, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegTwtTxStaSelCtrl_u;

/*REG_TWT_TWT_FIFO_BASE_ADDR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoBaseAddr : 22; //TWT FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegTwtTwtFifoBaseAddr_u;

/*REG_TWT_TWT_FIFO_DEPTH_MINUS_ONE 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoDepthMinusOne : 8; //TWT FIFO depth minus one, reset value: 0x7, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegTwtTwtFifoDepthMinusOne_u;

/*REG_TWT_TWT_FIFO_CLEAR_STRB 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoClearStrb : 1; //Clear TWT FIFO. , By write '1'., reset value: 0x0, access type: WO
		uint32 swTwtFifoClearFullDropCtrStrb : 1; //Clear TWT FIFO full drop counter. , By write '1'., reset value: 0x0, access type: WO
		uint32 swTwtFifoClearDecLessThanZeroStrb : 1; //Clear TWT FIFO decrement amount less than zero indication. , By write '1'.  , , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegTwtTwtFifoClearStrb_u;

/*REG_TWT_TWT_FIFO_RD_ENTRIES_NUM 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoRdEntriesNum : 9; //TWT FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegTwtTwtFifoRdEntriesNum_u;

/*REG_TWT_TWT_FIFO_NUM_ENTRIES_COUNT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoNumEntriesCount : 9; //TWT FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegTwtTwtFifoNumEntriesCount_u;

/*REG_TWT_TWT_FIFO_DEBUG 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoWrPtr : 8; //TWT FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 twtSwFifoNotEmpty : 1; //TWT FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 swTwtFifoFull : 1; //TWT FIFO full indication, reset value: 0x0, access type: RO
		uint32 swTwtFifoDecrementLessThanZero : 1; //TWT setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 swTwtFifoFullDropCtr : 8; //TWT setting FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegTwtTwtFifoDebug_u;

/*REG_TWT_TWT_FIFO_EN 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtEnSwFifo : 1; //Enable the external SW fifo, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegTwtTwtFifoEn_u;

/*REG_TWT_TX_TWT_ISR 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSwFifoNotEmptyMasked : 1; //masked external SW fifo interrupt to tell that the fifo isn't empty, reset value: 0x0, access type: RO
		uint32 twtSwFifoDropIrqMasked : 1; //Masked external SW fifo interrupt to tell that the fifo had dropped data, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 twtSwFifoNotEmptyEn : 1; //Enable the external SW fifo interrupt to tell that the fifo isn't empty, reset value: 0x1, access type: RW
		uint32 twtSwFifoDropIrqEn : 1; //Enable the external SW fifo interrupt to tell that the fifo had dropped data, reset value: 0x1, access type: RW
		uint32 twtSwFifoLessThanZeroIrqEn : 1; //Enable the external SW fifo interrupt to tell that the fifo SW handshake caused less than 0 pointer, reset value: 0x1, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegTwtTxTwtIsr_u;

/*REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtNextActiveSpThrs : 14; //Threshold for the next SP activation. Values are in 128uS resolution. All groups that their high time is less than this threshold will be part of calculating twt_next_active_sp_thrs, reset value: 0x8, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegTwtTxTwtNextActiveSpThrs_u;

/*REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtNextActiveSpThrs : 32; //Next SP activation,between all groups that their high time is less than sw_twt_next_active_sp_thrs. Values are in 1uS., reset value: 0x1FFFFFC0, access type: RO
	} bitFields;
} RegTwtTxTwtNextActiveSpThrsRo_u;

/*REG_TWT_TX_TWT_NEXT_ACTIVE_SP_RO 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtNextActiveSp : 32; //Next SP activation,between all groups. Values are in 1uS., reset value: 0x1FFFFFC0, access type: RO
	} bitFields;
} RegTwtTxTwtNextActiveSpRo_u;

//========================================
/* REG_TWT_TX_TWT_SW_HALT 0x0 */
#define B0_TWT_TX_TWT_SW_HALT_FIELD_SW_TWT_HALT_MASK                                                              0x00000001
//========================================
/* REG_TWT_TX_TWT_GEN_CFG 0x4 */
#define B0_TWT_TX_TWT_GEN_CFG_FIELD_SW_MU_EOSP_MIN_CALC_MASK                                                      0x00000001
#define B0_TWT_TX_TWT_GEN_CFG_FIELD_SW_EOSP_ALGORITHM_SEL_MASK                                                    0x00000030
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_UPDATE 0x8 */
#define B0_TWT_TX_TWT_SP_GROUP_UPDATE_FIELD_SW_TWT_SP_HIGH_LIMIT_MASK                                             0x00003FFF
#define B0_TWT_TX_TWT_SP_GROUP_UPDATE_FIELD_SW_TWT_SP_GROUP_IDX_MASK                                              0x7F000000
#define B0_TWT_TX_TWT_SP_GROUP_UPDATE_FIELD_TWT_SP_UPDATE_DONE_MASK                                               0x80000000
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_START_TSF 0xC */
#define B0_TWT_TX_TWT_SP_GROUP_START_TSF_FIELD_SW_TWT_SP_START_TSF_MASK                                           0xFFFFFFFF
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_LOW_PHASE 0x10 */
#define B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE_FIELD_SW_TWT_SP_LOW_LIMIT_MASK                                           0x003FFFFF
#define B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE_FIELD_SW_TSF_START_TIME_HIGH_MASK                                        0x10000000
#define B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE_FIELD_SW_TWT_SP_GROUP_UPDATE_ACT_SW_FIFO_MASK                            0x20000000
#define B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE_FIELD_SW_TWT_SP_GROUP_UPDATE_ACT_SELECTOR_MASK                           0x40000000
#define B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE_FIELD_SW_TWT_ANNOUNCE_VALUE_MASK                                         0x80000000
//========================================
/* REG_TWT_TX_TWT_SP_STA_UPDATE 0x14 */
#define B0_TWT_TX_TWT_SP_STA_UPDATE_FIELD_SW_TWT_SP_STA_GROUP_IDX_MASK                                            0x0000007F
#define B0_TWT_TX_TWT_SP_STA_UPDATE_FIELD_SW_TWT_SP_STA_IDX_MASK                                                  0x0000FF00
#define B0_TWT_TX_TWT_SP_STA_UPDATE_FIELD_SW_TWT_SP_STA_VALUE_MASK                                                0x00010000
#define B0_TWT_TX_TWT_SP_STA_UPDATE_FIELD_TWT_SP_STA_UPDATE_DONE_MASK                                             0x80000000
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_DEACTIVATE 0x1C */
#define B0_TWT_TX_TWT_SP_GROUP_DEACTIVATE_FIELD_SW_TWT_SP_DEACTIVATE_GROUP_IDX_MASK                               0x0000007F
#define B0_TWT_TX_TWT_SP_GROUP_DEACTIVATE_FIELD_TWT_SP_DEACTIVATE_DONE_MASK                                       0x80000000
//========================================
/* REG_TWT_TX_TWT_FSM_STATUS 0x20 */
#define B0_TWT_TX_TWT_FSM_STATUS_FIELD_TWT_SP_MAIN_SM_MASK                                                        0x0000001F
#define B0_TWT_TX_TWT_FSM_STATUS_FIELD_GROUP_CLEAR_PENDING_REQUEST_MASK                                           0x00000100
#define B0_TWT_TX_TWT_FSM_STATUS_FIELD_GROUP_INIT_PENDING_REQUEST_MASK                                            0x00000200
#define B0_TWT_TX_TWT_FSM_STATUS_FIELD_PERIODIC_UPDATE_PENDING_REQUEST_MASK                                       0x00000400
//========================================
/* REG_TWT_TX_TWT_STA_EOSP_SENT 0x2C */
#define B0_TWT_TX_TWT_STA_EOSP_SENT_FIELD_SW_STA_EOSP_SENT_MASK                                                   0x000000FF
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_VALID0 0x30 */
#define B0_TWT_TX_TWT_SP_GROUP_VALID0_FIELD_TWT_SP_GROUP_VALID0_MASK                                              0xFFFFFFFF
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_VALID1 0x34 */
#define B0_TWT_TX_TWT_SP_GROUP_VALID1_FIELD_TWT_SP_GROUP_VALID1_MASK                                              0xFFFFFFFF
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_VALID2 0x38 */
#define B0_TWT_TX_TWT_SP_GROUP_VALID2_FIELD_TWT_SP_GROUP_VALID2_MASK                                              0xFFFFFFFF
//========================================
/* REG_TWT_TX_TWT_SP_GROUP_VALID3 0x3C */
#define B0_TWT_TX_TWT_SP_GROUP_VALID3_FIELD_TWT_SP_GROUP_VALID3_MASK                                              0xFFFFFFFF
//========================================
/* REG_TWT_TX_STA_SEL_CTRL 0x40 */
#define B0_TWT_TX_STA_SEL_CTRL_FIELD_SW_STA_SEL_UPDATE_SELECTOR_DB_ANNOUNCED_MASK                                 0x00000001
#define B0_TWT_TX_STA_SEL_CTRL_FIELD_SW_STA_SEL_UPDATE_SELECTOR_DB_UNANNOUNCED_MASK                               0x00000002
#define B0_TWT_TX_STA_SEL_CTRL_FIELD_SW_STA_SEL_UPDATE_SW_FIFO_ANNOUNCED_MASK                                     0x00000004
#define B0_TWT_TX_STA_SEL_CTRL_FIELD_SW_STA_SEL_UPDATE_SW_FIFO_UNANNOUNCED_MASK                                   0x00000008
//========================================
/* REG_TWT_TWT_FIFO_BASE_ADDR 0x50 */
#define B0_TWT_TWT_FIFO_BASE_ADDR_FIELD_SW_TWT_FIFO_BASE_ADDR_MASK                                                0x003FFFFF
//========================================
/* REG_TWT_TWT_FIFO_DEPTH_MINUS_ONE 0x54 */
#define B0_TWT_TWT_FIFO_DEPTH_MINUS_ONE_FIELD_SW_TWT_FIFO_DEPTH_MINUS_ONE_MASK                                    0x000000FF
//========================================
/* REG_TWT_TWT_FIFO_CLEAR_STRB 0x58 */
#define B0_TWT_TWT_FIFO_CLEAR_STRB_FIELD_SW_TWT_FIFO_CLEAR_STRB_MASK                                              0x00000001
#define B0_TWT_TWT_FIFO_CLEAR_STRB_FIELD_SW_TWT_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK                                0x00000002
#define B0_TWT_TWT_FIFO_CLEAR_STRB_FIELD_SW_TWT_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK                           0x00000004
//========================================
/* REG_TWT_TWT_FIFO_RD_ENTRIES_NUM 0x5C */
#define B0_TWT_TWT_FIFO_RD_ENTRIES_NUM_FIELD_SW_TWT_FIFO_RD_ENTRIES_NUM_MASK                                      0x000001FF
//========================================
/* REG_TWT_TWT_FIFO_NUM_ENTRIES_COUNT 0x60 */
#define B0_TWT_TWT_FIFO_NUM_ENTRIES_COUNT_FIELD_SW_TWT_FIFO_NUM_ENTRIES_COUNT_MASK                                0x000001FF
//========================================
/* REG_TWT_TWT_FIFO_DEBUG 0x64 */
#define B0_TWT_TWT_FIFO_DEBUG_FIELD_SW_TWT_FIFO_WR_PTR_MASK                                                       0x000000FF
#define B0_TWT_TWT_FIFO_DEBUG_FIELD_TWT_SW_FIFO_NOT_EMPTY_MASK                                                    0x00010000
#define B0_TWT_TWT_FIFO_DEBUG_FIELD_SW_TWT_FIFO_FULL_MASK                                                         0x00020000
#define B0_TWT_TWT_FIFO_DEBUG_FIELD_SW_TWT_FIFO_DECREMENT_LESS_THAN_ZERO_MASK                                     0x00040000
#define B0_TWT_TWT_FIFO_DEBUG_FIELD_SW_TWT_FIFO_FULL_DROP_CTR_MASK                                                0xFF000000
//========================================
/* REG_TWT_TWT_FIFO_EN 0x68 */
#define B0_TWT_TWT_FIFO_EN_FIELD_TWT_EN_SW_FIFO_MASK                                                              0x00000001
//========================================
/* REG_TWT_TX_TWT_ISR 0x70 */
#define B0_TWT_TX_TWT_ISR_FIELD_TWT_SW_FIFO_NOT_EMPTY_MASKED_MASK                                                 0x00000001
#define B0_TWT_TX_TWT_ISR_FIELD_TWT_SW_FIFO_DROP_IRQ_MASKED_MASK                                                  0x00000002
#define B0_TWT_TX_TWT_ISR_FIELD_TWT_SW_FIFO_NOT_EMPTY_EN_MASK                                                     0x00000010
#define B0_TWT_TX_TWT_ISR_FIELD_TWT_SW_FIFO_DROP_IRQ_EN_MASK                                                      0x00000020
#define B0_TWT_TX_TWT_ISR_FIELD_TWT_SW_FIFO_LESS_THAN_ZERO_IRQ_EN_MASK                                            0x00000040
//========================================
/* REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS 0x80 */
#define B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_FIELD_SW_TWT_NEXT_ACTIVE_SP_THRS_MASK                                   0x00003FFF
//========================================
/* REG_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO 0x84 */
#define B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO_FIELD_TWT_NEXT_ACTIVE_SP_THRS_MASK                                   0xFFFFFFFF
//========================================
/* REG_TWT_TX_TWT_NEXT_ACTIVE_SP_RO 0x88 */
#define B0_TWT_TX_TWT_NEXT_ACTIVE_SP_RO_FIELD_TWT_NEXT_ACTIVE_SP_MASK                                             0xFFFFFFFF


#endif // _TWT_REGS_H_
