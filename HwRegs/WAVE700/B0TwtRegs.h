
/***********************************************************************************
File:				B0TwtRegs.h
Module:				b0Twt
SOC Revision:		latest
Generated at:       2022-05-04 10:57:28
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TWT_REGS_H_
#define _B0_TWT_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TWT_BASE_ADDRESS                             MEMORY_MAP_UNIT_56_BASE_ADDRESS
#define	REG_B0_TWT_TX_TWT_SW_HALT                   (B0_TWT_BASE_ADDRESS + 0x0)
#define	REG_B0_TWT_TX_TWT_GEN_CFG                   (B0_TWT_BASE_ADDRESS + 0x4)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_UPDATE           (B0_TWT_BASE_ADDRESS + 0x8)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_START_TSF        (B0_TWT_BASE_ADDRESS + 0xC)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE        (B0_TWT_BASE_ADDRESS + 0x10)
#define	REG_B0_TWT_TX_TWT_SP_STA_UPDATE             (B0_TWT_BASE_ADDRESS + 0x14)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_DEACTIVATE       (B0_TWT_BASE_ADDRESS + 0x1C)
#define	REG_B0_TWT_TX_TWT_FSM_STATUS                (B0_TWT_BASE_ADDRESS + 0x20)
#define	REG_B0_TWT_TX_TWT_STA_EOSP_SENT             (B0_TWT_BASE_ADDRESS + 0x2C)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_VALID0           (B0_TWT_BASE_ADDRESS + 0x30)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_VALID1           (B0_TWT_BASE_ADDRESS + 0x34)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_VALID2           (B0_TWT_BASE_ADDRESS + 0x38)
#define	REG_B0_TWT_TX_TWT_SP_GROUP_VALID3           (B0_TWT_BASE_ADDRESS + 0x3C)
#define	REG_B0_TWT_TX_STA_SEL_CTRL                  (B0_TWT_BASE_ADDRESS + 0x40)
#define	REG_B0_TWT_TWT_FIFO_BASE_ADDR               (B0_TWT_BASE_ADDRESS + 0x50)
#define	REG_B0_TWT_TWT_FIFO_DEPTH_MINUS_ONE         (B0_TWT_BASE_ADDRESS + 0x54)
#define	REG_B0_TWT_TWT_FIFO_CLEAR_STRB              (B0_TWT_BASE_ADDRESS + 0x58)
#define	REG_B0_TWT_TWT_FIFO_RD_ENTRIES_NUM          (B0_TWT_BASE_ADDRESS + 0x5C)
#define	REG_B0_TWT_TWT_FIFO_NUM_ENTRIES_COUNT       (B0_TWT_BASE_ADDRESS + 0x60)
#define	REG_B0_TWT_TWT_FIFO_DEBUG                   (B0_TWT_BASE_ADDRESS + 0x64)
#define	REG_B0_TWT_TWT_FIFO_EN                      (B0_TWT_BASE_ADDRESS + 0x68)
#define	REG_B0_TWT_TX_TWT_ISR                       (B0_TWT_BASE_ADDRESS + 0x70)
#define	REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS       (B0_TWT_BASE_ADDRESS + 0x80)
#define	REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO    (B0_TWT_BASE_ADDRESS + 0x84)
#define	REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_RO         (B0_TWT_BASE_ADDRESS + 0x88)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TWT_TX_TWT_SW_HALT 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtHalt : 1; //Enables update of twt_sp field, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TwtTxTwtSwHalt_u;

/*REG_B0_TWT_TX_TWT_GEN_CFG 0x4 */
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
} RegB0TwtTxTwtGenCfg_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_UPDATE 0x8 */
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
} RegB0TwtTxTwtSpGroupUpdate_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_START_TSF 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpStartTsf : 32; //Start TSF value. 32 LSB bit only (Out of the full 64 bits), reset value: 0x0, access type: RW
	} bitFields;
} RegB0TwtTxTwtSpGroupStartTsf_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_LOW_PHASE 0x10 */
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
} RegB0TwtTxTwtSpGroupLowPhase_u;

/*REG_B0_TWT_TX_TWT_SP_STA_UPDATE 0x14 */
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
} RegB0TwtTxTwtSpStaUpdate_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_DEACTIVATE 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtSpDeactivateGroupIdx : 7; //SP group index, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
		uint32 twtSpDeactivateDone : 1; //Instruction done indication, reset value: 0x1, access type: RO
	} bitFields;
} RegB0TwtTxTwtSpGroupDeactivate_u;

/*REG_B0_TWT_TX_TWT_FSM_STATUS 0x20 */
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
} RegB0TwtTxTwtFsmStatus_u;

/*REG_B0_TWT_TX_TWT_STA_EOSP_SENT 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swStaEospSent : 8; //Fix WLANVLSIIP-4926, SW should write the STA ID to this field whenever we transmit EOSP to a STA that eventually cause BAA to clear the SP bit in selector, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TwtTxTwtStaEospSent_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_VALID0 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid0 : 32; //Enabled SP groups - group number 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TwtTxTwtSpGroupValid0_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_VALID1 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid1 : 32; //Enabled SP groups - group number 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TwtTxTwtSpGroupValid1_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_VALID2 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid2 : 32; //Enabled SP groups - group number 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TwtTxTwtSpGroupValid2_u;

/*REG_B0_TWT_TX_TWT_SP_GROUP_VALID3 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtSpGroupValid3 : 32; //Enabled SP groups - group number 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TwtTxTwtSpGroupValid3_u;

/*REG_B0_TWT_TX_STA_SEL_CTRL 0x40 */
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
} RegB0TwtTxStaSelCtrl_u;

/*REG_B0_TWT_TWT_FIFO_BASE_ADDR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoBaseAddr : 22; //TWT FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB0TwtTwtFifoBaseAddr_u;

/*REG_B0_TWT_TWT_FIFO_DEPTH_MINUS_ONE 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoDepthMinusOne : 8; //TWT FIFO depth minus one, reset value: 0x7, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TwtTwtFifoDepthMinusOne_u;

/*REG_B0_TWT_TWT_FIFO_CLEAR_STRB 0x58 */
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
} RegB0TwtTwtFifoClearStrb_u;

/*REG_B0_TWT_TWT_FIFO_RD_ENTRIES_NUM 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoRdEntriesNum : 9; //TWT FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0TwtTwtFifoRdEntriesNum_u;

/*REG_B0_TWT_TWT_FIFO_NUM_ENTRIES_COUNT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtFifoNumEntriesCount : 9; //TWT FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB0TwtTwtFifoNumEntriesCount_u;

/*REG_B0_TWT_TWT_FIFO_DEBUG 0x64 */
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
} RegB0TwtTwtFifoDebug_u;

/*REG_B0_TWT_TWT_FIFO_EN 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtEnSwFifo : 1; //Enable the external SW fifo, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TwtTwtFifoEn_u;

/*REG_B0_TWT_TX_TWT_ISR 0x70 */
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
} RegB0TwtTxTwtIsr_u;

/*REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTwtNextActiveSpThrs : 14; //Threshold for the next SP activation. Values are in 128uS resolution. All groups that their high time is less than this threshold will be part of calculating twt_next_active_sp_thrs, reset value: 0x8, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TwtTxTwtNextActiveSpThrs_u;

/*REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_THRS_RO 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtNextActiveSpThrs : 32; //Next SP activation,between all groups that their high time is less than sw_twt_next_active_sp_thrs. Values are in 1uS., reset value: 0x1FFFFFC0, access type: RO
	} bitFields;
} RegB0TwtTxTwtNextActiveSpThrsRo_u;

/*REG_B0_TWT_TX_TWT_NEXT_ACTIVE_SP_RO 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twtNextActiveSp : 32; //Next SP activation,between all groups. Values are in 1uS., reset value: 0x1FFFFFC0, access type: RO
	} bitFields;
} RegB0TwtTxTwtNextActiveSpRo_u;



#endif // _B0_TWT_REGS_H_
