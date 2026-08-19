
/***********************************************************************************
File:				B0TxHandlerRegs.h
Module:				b0TxHandler
SOC Revision:		latest
Generated at:       2024-06-26 12:54:33
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TX_HANDLER_REGS_H_
#define _B0_TX_HANDLER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TX_HANDLER_BASE_ADDRESS                        MEMORY_MAP_UNIT_23_BASE_ADDRESS
#define	REG_B0_TX_HANDLER_ASPM_CONTROL                        (B0_TX_HANDLER_BASE_ADDRESS + 0x14)
#define	REG_B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3           (B0_TX_HANDLER_BASE_ADDRESS + 0x18)
#define	REG_B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7           (B0_TX_HANDLER_BASE_ADDRESS + 0x1C)
#define	REG_B0_TX_HANDLER_AC_ID_PRIORITY                      (B0_TX_HANDLER_BASE_ADDRESS + 0x20)
#define	REG_B0_TX_HANDLER_WLAN_RETRY_CTR_LIMIT                (B0_TX_HANDLER_BASE_ADDRESS + 0x24)
#define	REG_B0_TX_HANDLER_AC_EXT_CW_REMAPPING                 (B0_TX_HANDLER_BASE_ADDRESS + 0x28)
#define	REG_B0_TX_HANDLER_LCG_RAND_FUNC_SEED                  (B0_TX_HANDLER_BASE_ADDRESS + 0x2C)
#define	REG_B0_TX_HANDLER_LCG_RAND_FUNC_SHIFT_NUM             (B0_TX_HANDLER_BASE_ADDRESS + 0x30)
#define	REG_B0_TX_HANDLER_TXH_MAP_CONTROL                     (B0_TX_HANDLER_BASE_ADDRESS + 0x34)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BE                    (B0_TX_HANDLER_BASE_ADDRESS + 0x40)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BK                    (B0_TX_HANDLER_BASE_ADDRESS + 0x44)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_VI                    (B0_TX_HANDLER_BASE_ADDRESS + 0x48)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_VO                    (B0_TX_HANDLER_BASE_ADDRESS + 0x4C)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_GPLP                  (B0_TX_HANDLER_BASE_ADDRESS + 0x50)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_GPHP                  (B0_TX_HANDLER_BASE_ADDRESS + 0x54)
#define	REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BEACON                (B0_TX_HANDLER_BASE_ADDRESS + 0x58)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BE                 (B0_TX_HANDLER_BASE_ADDRESS + 0x5C)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BK                 (B0_TX_HANDLER_BASE_ADDRESS + 0x60)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_VI                 (B0_TX_HANDLER_BASE_ADDRESS + 0x64)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_VO                 (B0_TX_HANDLER_BASE_ADDRESS + 0x68)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPLP               (B0_TX_HANDLER_BASE_ADDRESS + 0x6C)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPHP               (B0_TX_HANDLER_BASE_ADDRESS + 0x70)
#define	REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BEACON             (B0_TX_HANDLER_BASE_ADDRESS + 0x74)
#define	REG_B0_TX_HANDLER_WINNER_RESULT                       (B0_TX_HANDLER_BASE_ADDRESS + 0x7C)
#define	REG_B0_TX_HANDLER_UNFREEZE_REPORT_SET                 (B0_TX_HANDLER_BASE_ADDRESS + 0x80)
#define	REG_B0_TX_HANDLER_UNFREEZE_REPORT                     (B0_TX_HANDLER_BASE_ADDRESS + 0x84)
#define	REG_B0_TX_HANDLER_TXH_MAP_DBG                         (B0_TX_HANDLER_BASE_ADDRESS + 0x88)
#define	REG_B0_TX_HANDLER_RECIPE_PTR_AC_ID0TO3                (B0_TX_HANDLER_BASE_ADDRESS + 0x8C)
#define	REG_B0_TX_HANDLER_RECIPE_PTR_AC_GPLP                  (B0_TX_HANDLER_BASE_ADDRESS + 0x90)
#define	REG_B0_TX_HANDLER_RECIPE_PTR_AC_GPHP                  (B0_TX_HANDLER_BASE_ADDRESS + 0x94)
#define	REG_B0_TX_HANDLER_RECIPE_PTR_AC_BEACON                (B0_TX_HANDLER_BASE_ADDRESS + 0x98)
#define	REG_B0_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS               (B0_TX_HANDLER_BASE_ADDRESS + 0x9C)
#define	REG_B0_TX_HANDLER_TXH_EPSILON_MICRO_PRESCALER_NUM     (B0_TX_HANDLER_BASE_ADDRESS + 0xA0)
#define	REG_B0_TX_HANDLER_TXH_EPSILON_MICRO_NUM               (B0_TX_HANDLER_BASE_ADDRESS + 0xA4)
#define	REG_B0_TX_HANDLER_TXH_LOGGER                          (B0_TX_HANDLER_BASE_ADDRESS + 0xA8)
#define	REG_B0_TX_HANDLER_TXH_LOGGER_ACTIVE                   (B0_TX_HANDLER_BASE_ADDRESS + 0xAC)
#define	REG_B0_TX_HANDLER_DEBUG_MU_EDCA_TIMER_RESOLUTION      (B0_TX_HANDLER_BASE_ADDRESS + 0xB0)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_REP_CYCLE_COUNT          (B0_TX_HANDLER_BASE_ADDRESS + 0x104)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_REP_DURATION             (B0_TX_HANDLER_BASE_ADDRESS + 0x108)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_STATUS                   (B0_TX_HANDLER_BASE_ADDRESS + 0x10C)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BE                (B0_TX_HANDLER_BASE_ADDRESS + 0x110)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BK                (B0_TX_HANDLER_BASE_ADDRESS + 0x114)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VI                (B0_TX_HANDLER_BASE_ADDRESS + 0x118)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VO                (B0_TX_HANDLER_BASE_ADDRESS + 0x11C)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPLP              (B0_TX_HANDLER_BASE_ADDRESS + 0x120)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPHP              (B0_TX_HANDLER_BASE_ADDRESS + 0x124)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BEACON            (B0_TX_HANDLER_BASE_ADDRESS + 0x128)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_DBG                      (B0_TX_HANDLER_BASE_ADDRESS + 0x12C)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_IRQ                      (B0_TX_HANDLER_BASE_ADDRESS + 0x130)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_CLEAR_IRQ                (B0_TX_HANDLER_BASE_ADDRESS + 0x134)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_GENERATE_IRQ_COMMANDS    (B0_TX_HANDLER_BASE_ADDRESS + 0x138)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_COMMAND_VAP_VECTOR       (B0_TX_HANDLER_BASE_ADDRESS + 0x13C)
#define	REG_B0_TX_HANDLER_TXH_PAUSER_COMMAND                  (B0_TX_HANDLER_BASE_ADDRESS + 0x140)
#define	REG_B0_TX_HANDLER_SPARE_REGISTERS                     (B0_TX_HANDLER_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TX_HANDLER_ASPM_CONTROL 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMinAspmSlot : 4; //no description, reset value: 0x8, access type: RW
		uint32 pmEnableBackoffAspm : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 11;
		uint32 statusMinBackoffPlusIfsn : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 12;
	} bitFields;
} RegB0TxHandlerAspmControl_u;

/*REG_B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastVapWinnerAcBe : 5; //Last VAP winner for AC_BE, reset value: 0x0, access type: RO
		uint32 lastVapWinnerAcBk : 5; //Last VAP winner for AC_BK, reset value: 0x0, access type: RO
		uint32 lastVapWinnerAcVi : 5; //Last VAP winner for AC_VI, reset value: 0x0, access type: RO
		uint32 lastVapWinnerAcVo : 5; //Last VAP winner for AC_VO, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TxHandlerLastVapWinnerAcId0To3_u;

/*REG_B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastVapWinnerAcGplp : 5; //Last VAP winner for AC_GPLP, reset value: 0x0, access type: RO
		uint32 lastVapWinnerAcGphp : 5; //Last VAP winner for AC_GPHP, reset value: 0x0, access type: RO
		uint32 lastVapWinnerAcBeacon : 5; //Last VAP winner for AC_BEACON, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0TxHandlerLastVapWinnerAcId4To7_u;

/*REG_B0_TX_HANDLER_AC_ID_PRIORITY 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 acIdPriority0 : 3; //AC_ID priority0, reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 acIdPriority1 : 3; //AC_ID priority1, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 acIdPriority2 : 3; //AC_ID priority2, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 acIdPriority3 : 3; //AC_ID priority3, reset value: 0x2, access type: RW
		uint32 reserved3 : 1;
		uint32 acIdPriority4 : 3; //AC_ID priority4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 acIdPriority5 : 3; //AC_ID priority5, reset value: 0x6, access type: RW
		uint32 reserved5 : 1;
		uint32 acIdPriority6 : 3; //AC_ID priority6, reset value: 0x7, access type: RW
		uint32 reserved6 : 5;
	} bitFields;
} RegB0TxHandlerAcIdPriority_u;

/*REG_B0_TX_HANDLER_WLAN_RETRY_CTR_LIMIT 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wlanShortRetryCtrLimit : 8; //WLAN Short Retry limit, reset value: 0x7, access type: RW
		uint32 wlanLongRetryCtrLimit : 8; //WLAN Long Retry limit, reset value: 0x4, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxHandlerWlanRetryCtrLimit_u;

/*REG_B0_TX_HANDLER_AC_EXT_CW_REMAPPING 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 acGplpCwRemappingAp : 3; //AC_GPLP CW remapping in AP mode, reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 acGphpCwRemappingAp : 3; //AC_GPHP CW remapping in AP mode, reset value: 0x6, access type: RW
		uint32 reserved1 : 1;
		uint32 acBeaconCwRemappingAp : 3; //AC_BEACON CW remapping in AP mode, reset value: 0x7, access type: RW
		uint32 reserved2 : 5;
		uint32 acGplpCwRemappingSta : 3; //AC_GPLP CW remapping in STA mode, reset value: 0x5, access type: RW
		uint32 reserved3 : 1;
		uint32 acGphpCwRemappingSta : 3; //AC_GPHP CW remapping in STA mode, reset value: 0x6, access type: RW
		uint32 reserved4 : 1;
		uint32 acBeaconCwRemappingSta : 3; //AC_BEACON CW remapping in STA mode, reset value: 0x7, access type: RW
		uint32 reserved5 : 5;
	} bitFields;
} RegB0TxHandlerAcExtCwRemapping_u;

/*REG_B0_TX_HANDLER_LCG_RAND_FUNC_SEED 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcgRandFuncSeed : 32; //LCG random function seed, reset value: 0x12345678, access type: RW
	} bitFields;
} RegB0TxHandlerLcgRandFuncSeed_u;

/*REG_B0_TX_HANDLER_LCG_RAND_FUNC_SHIFT_NUM 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcgRandFuncShiftNum : 5; //LCG random function shift number, reset value: 0x10, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0TxHandlerLcgRandFuncShiftNum_u;

/*REG_B0_TX_HANDLER_TXH_MAP_CONTROL 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhMapEnable : 1; //Tx Handler MAP enable, reset value: 0x1, access type: RW
		uint32 freezeInAggBuilderErrorEn : 1; //freeze in AGG builder error enable, reset value: 0x0, access type: RW
		uint32 punishOnInternalCollisionEn : 1; //Punish on internal collision enable, reset value: 0x1, access type: RW
		uint32 increaseOnIntColSsrcSlrcN : 1; //Determined which retry counter to increment in case of internal collision: , 0 - SLRC, 1 - SSRC, reset value: 0x1, access type: RW
		uint32 txGroupAddressedSetEcwminEn : 1; //Tx group addressed set ECWmin enable, reset value: 0x1, access type: RW
		uint32 retryCtrLimitZeroedCtrEn : 1; //Retry ctr limit zeroed ctr enable, reset value: 0x1, access type: RW
		uint32 punishWinnerOnCollisionEn : 1; //Punish Winner on external collision enable, reset value: 0x1, access type: RW
		uint32 ignoreMuEdcaTimerExpired : 1; //Ignore MU EDCA timer expired event. it'll cause not to move back from MU EDCA to regular EDCA, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxHandlerTxhMapControl_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcBe : 32; //Tx request VAP AC_BE vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcBe_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BK 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcBk : 32; //Tx request VAP AC_BK vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcBk_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_VI 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcVi : 32; //Tx request VAP AC_VI vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcVi_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_VO 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcVo : 32; //Tx request VAP AC_VO vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcVo_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_GPLP 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcGplp : 32; //Tx request VAP AC_GPLP vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcGplp_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_GPHP 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcGphp : 32; //Tx request VAP AC_GPHP vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcGphp_u;

/*REG_B0_TX_HANDLER_TX_REQ_VAP_AC_BEACON 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txReqVapAcBeacon : 32; //Tx request VAP AC_BEACON vector internal, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerTxReqVapAcBeacon_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BE 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcBe : 32; //Last slot VAP AC_BE vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcBe_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BK 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcBk : 32; //Last slot VAP AC_BK vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcBk_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_VI 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcVi : 32; //Last slot VAP AC_VI vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcVi_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_VO 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcVo : 32; //Last slot VAP AC_VO vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcVo_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPLP 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcGplp : 32; //Last slot VAP AC_GPLP vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcGplp_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPHP 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcGphp : 32; //Last slot VAP AC_GPHP vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcGphp_u;

/*REG_B0_TX_HANDLER_LAST_SLOT_VAP_AC_BEACON 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSlotVapAcBeacon : 32; //Last slot VAP AC_BEACON vector, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxHandlerLastSlotVapAcBeacon_u;

/*REG_B0_TX_HANDLER_WINNER_RESULT 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 winnerVapId : 5; //winner VAP ID, reset value: 0x0, access type: RO
		uint32 winnerAcId : 3; //winner AC ID, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxHandlerWinnerResult_u;

/*REG_B0_TX_HANDLER_UNFREEZE_REPORT_SET 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 unfreezeCommand : 1; //Set Unfreeze command, reset value: 0x0, access type: WO
		uint32 ctsRxInfoUpdate : 1; //CTS Rx information update, reset value: 0x0, access type: WO
		uint32 txResultUpdate : 1; //Tx result update, reset value: 0x0, access type: WO
		uint32 reserved0 : 5;
		uint32 ctsRxInfo : 2; //CTS Rx information, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
		uint32 txResult : 2; //Tx result, reset value: 0x0, access type: RW
		uint32 txFrameLength : 1; //Tx frame length, reset value: 0x0, access type: RW
		uint32 txHeUlTbSuccessAcBitmap : 4; //Success AC IDs 0-3 bitmap - used for HE UL TB STA mode, reset value: 0x0, access type: RW
		uint32 txHeUlTbVapid : 5; //VAP ID - used for HE UL TB STA mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB0TxHandlerUnfreezeReportSet_u;

/*REG_B0_TX_HANDLER_UNFREEZE_REPORT 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 ctsRxInfoRd : 2; //CTS Rx information read, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 txResultRd : 2; //Tx result read, reset value: 0x0, access type: RO
		uint32 txFrameLengthRd : 1; //Tx frame length read, reset value: 0x0, access type: RO
		uint32 txHeUlTbSuccessAcBitmapRd : 4; //Success AC IDs 0-3 bitmap read - used for HE UL TB STA mode, reset value: 0x0, access type: RO
		uint32 txHeUlTbVapidRd : 5; //VAP ID read - used for HE UL TB STA mode, reset value: 0x0, access type: RO
		uint32 reserved2 : 4;
	} bitFields;
} RegB0TxHandlerUnfreezeReport_u;

/*REG_B0_TX_HANDLER_TXH_MAP_DBG 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhMapSm : 5; //TXH MAP state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 pauseFdbkSm : 2; //pause fdbk state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 txhMapHandlerSm : 4; //TXH MAP handler state machine, reset value: 0x0, access type: RO
		uint32 txhMapInFreeze : 1; //TXH MAP  in freeze, reset value: 0x0, access type: RO
		uint32 lastSlotMaskedVapAc : 1; //last slot masked vap_ac, reset value: 0x0, access type: RO
		uint32 txReqVapAc : 1; //Tx req vap_ac, reset value: 0x0, access type: RO
		uint32 txSelTxhdVapAc : 1; //tx_sel txhd vap_ac, reset value: 0x0, access type: RO
		uint32 mapHandlerOperation : 4; //map handler operation, reset value: 0x0, access type: RO
		uint32 txhMapHeUlTbMode : 1; //TXH MAP in HE UL TB STA mode, reset value: 0x0, access type: RO
		uint32 reserved2 : 7;
	} bitFields;
} RegB0TxHandlerTxhMapDbg_u;

/*REG_B0_TX_HANDLER_RECIPE_PTR_AC_ID0TO3 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipePtrAcId0To3 : 26; //Recipe pointer AC_ID 0to3, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0TxHandlerRecipePtrAcId0To3_u;

/*REG_B0_TX_HANDLER_RECIPE_PTR_AC_GPLP 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipePtrAcGplp : 26; //Recipe pointer AC_ID GPLP, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0TxHandlerRecipePtrAcGplp_u;

/*REG_B0_TX_HANDLER_RECIPE_PTR_AC_GPHP 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipePtrAcGphp : 26; //Recipe pointer AC_ID GPHP, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0TxHandlerRecipePtrAcGphp_u;

/*REG_B0_TX_HANDLER_RECIPE_PTR_AC_BEACON 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipePtrAcBeacon : 26; //Recipe pointer AC_ID Beacon, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0TxHandlerRecipePtrAcBeacon_u;

/*REG_B0_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhNtdCcaSensitive : 1; //TXH to NTD CCA sensitive, reset value: 0x1, access type: RW
		uint32 txhNtdRxNavSensitive : 1; //TXH to NTD Rx NAV sensitive, reset value: 0x1, access type: RW
		uint32 txhNtdTxNavSensitive : 1; //TXH to NTD Tx NAV sensitive, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TxHandlerTxhNtdTxReqParams_u;

/*REG_B0_TX_HANDLER_TXH_EPSILON_MICRO_PRESCALER_NUM 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhEpsilonMicroPrescalerNum : 10; //TxH HW epsilon micro prescaler num, reset value: 0x9f, access type: RW
		uint32 reserved0 : 6;
		uint32 txhEpsilonMicroPrescalerNumFreqReduced : 10; //TxH HW epsilon micro prescaler num freq reduced, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegB0TxHandlerTxhEpsilonMicroPrescalerNum_u;

/*REG_B0_TX_HANDLER_TXH_EPSILON_MICRO_NUM 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhEpsilonMicroNum : 8; //TxH HW epsilon micro num, reset value: 0x5, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxHandlerTxhEpsilonMicroNum_u;

/*REG_B0_TX_HANDLER_TXH_LOGGER 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhLoggerEn : 1; //Tx Handler logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 txhLoggerPriority : 2; //Tx Handler logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB0TxHandlerTxhLogger_u;

/*REG_B0_TX_HANDLER_TXH_LOGGER_ACTIVE 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhLoggerActive : 1; //Tx Handler logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxHandlerTxhLoggerActive_u;

/*REG_B0_TX_HANDLER_DEBUG_MU_EDCA_TIMER_RESOLUTION 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugMuEdcaTimerResolution : 3; //for debug, change resolution of MU EDCA timer by (8*1024[us] shift right of this register value), reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TxHandlerDebugMuEdcaTimerResolution_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_REP_CYCLE_COUNT 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserRepCycleCount : 8; //TXH Pauser repetition cycle count, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxHandlerTxhPauserRepCycleCount_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_REP_DURATION 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserRepPauseDur : 16; //TXH Pauser repetition pause duration, reset value: 0x0, access type: RW
		uint32 txhPauserRepResumeDur : 16; //TXH Pauser repetition resume duration, reset value: 0x0, access type: RW
	} bitFields;
} RegB0TxHandlerTxhPauserRepDuration_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_STATUS 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcInUsed : 1; //TXH Pauser vap ac in used, reset value: 0x0, access type: RO
		uint32 txhPauserWaitForConfirm : 1; //TXH Pauser wait for confirm, reset value: 0x0, access type: RO
		uint32 txhPauserRepModeSeq : 1; //TXH pauser repetition mode seq, reset value: 0x0, access type: RO
		uint32 txhPauserRepLimitedInfiniteN : 1; //TXH Pauser repetition limited infinite_n, reset value: 0x0, access type: RO
		uint32 txhPauserRepPausePeriod : 1; //TXH Pauser repetition pause period, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 txhPauserRepCounter : 8; //TXH Pauser repetition counter, reset value: 0x0, access type: RO
		uint32 reserved1 : 16;
	} bitFields;
} RegB0TxHandlerTxhPauserStatus_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BE 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcBe : 32; //TXH Pauser VAP AC_BE, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcBe_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BK 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcBk : 32; //TXH Pauser VAP AC_BK, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcBk_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VI 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcVi : 32; //TXH Pauser VAP AC_VI, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcVi_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VO 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcVo : 32; //TXH Pauser VAP AC_VO, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcVo_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPLP 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcGplp : 32; //TXH Pauser VAP AC_GPLP, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcGplp_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPHP 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcGphp : 32; //TXH Pauser VAP AC_GPHP, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcGphp_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BEACON 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapAcBeacon : 32; //TXH Pauser VAP AC_BEACON, reset value: 0xffff, access type: RO
	} bitFields;
} RegB0TxHandlerTxhPauserVapAcBeacon_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_DBG 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserSm : 4; //TXH Pauser state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxHandlerTxhPauserDbg_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_IRQ 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserIrq : 1; //TXH Pauser IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxHandlerTxhPauserIrq_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_CLEAR_IRQ 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserClearIrq : 1; //TXH Pauser clear IRQ, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxHandlerTxhPauserClearIrq_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_GENERATE_IRQ_COMMANDS 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserGenerateIrqCommands : 16; //TXH Pauser generate IRQ per command , reset value: 0xffff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxHandlerTxhPauserGenerateIrqCommands_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_COMMAND_VAP_VECTOR 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserVapVector : 32; //TXH Pauser VAP vector, reset value: 0x0, access type: RW
	} bitFields;
} RegB0TxHandlerTxhPauserCommandVapVector_u;

/*REG_B0_TX_HANDLER_TXH_PAUSER_COMMAND 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserCommand : 4; //TXH Pauser command, reset value: 0x0, access type: RW
		uint32 txhPauserRepetitionType : 1; //TXH Pauser repetition type, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 txhPauserAcVector : 8; //TXH Pauser AC vector, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegB0TxHandlerTxhPauserCommand_u;

/*REG_B0_TX_HANDLER_SPARE_REGISTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegisters : 16; //Spare registers, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxHandlerSpareRegisters_u;

//========================================
/* REG_TX_HANDLER_ASPM_CONTROL 0x14 */
#define B0_TX_HANDLER_ASPM_CONTROL_FIELD_PM_MIN_ASPM_SLOT_MASK                                                    0x0000000F
#define B0_TX_HANDLER_ASPM_CONTROL_FIELD_PM_ENABLE_BACKOFF_ASPM_MASK                                              0x00000010
#define B0_TX_HANDLER_ASPM_CONTROL_FIELD_STATUS_MIN_BACKOFF_PLUS_IFSN_MASK                                        0x000F0000
//========================================
/* REG_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3 0x18 */
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3_FIELD_LAST_VAP_WINNER_AC_BE_MASK                                  0x0000001F
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3_FIELD_LAST_VAP_WINNER_AC_BK_MASK                                  0x000003E0
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3_FIELD_LAST_VAP_WINNER_AC_VI_MASK                                  0x00007C00
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID0TO3_FIELD_LAST_VAP_WINNER_AC_VO_MASK                                  0x000F8000
//========================================
/* REG_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7 0x1C */
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7_FIELD_LAST_VAP_WINNER_AC_GPLP_MASK                                0x0000001F
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7_FIELD_LAST_VAP_WINNER_AC_GPHP_MASK                                0x000003E0
#define B0_TX_HANDLER_LAST_VAP_WINNER_AC_ID4TO7_FIELD_LAST_VAP_WINNER_AC_BEACON_MASK                              0x00007C00
//========================================
/* REG_TX_HANDLER_AC_ID_PRIORITY 0x20 */
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY0_MASK                                                   0x00000007
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY1_MASK                                                   0x00000070
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY2_MASK                                                   0x00000700
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY3_MASK                                                   0x00007000
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY4_MASK                                                   0x00070000
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY5_MASK                                                   0x00700000
#define B0_TX_HANDLER_AC_ID_PRIORITY_FIELD_AC_ID_PRIORITY6_MASK                                                   0x07000000
//========================================
/* REG_TX_HANDLER_WLAN_RETRY_CTR_LIMIT 0x24 */
#define B0_TX_HANDLER_WLAN_RETRY_CTR_LIMIT_FIELD_WLAN_SHORT_RETRY_CTR_LIMIT_MASK                                  0x000000FF
#define B0_TX_HANDLER_WLAN_RETRY_CTR_LIMIT_FIELD_WLAN_LONG_RETRY_CTR_LIMIT_MASK                                   0x0000FF00
//========================================
/* REG_TX_HANDLER_AC_EXT_CW_REMAPPING 0x28 */
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_GPLP_CW_REMAPPING_AP_MASK                                      0x00000007
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_GPHP_CW_REMAPPING_AP_MASK                                      0x00000070
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_BEACON_CW_REMAPPING_AP_MASK                                    0x00000700
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_GPLP_CW_REMAPPING_STA_MASK                                     0x00070000
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_GPHP_CW_REMAPPING_STA_MASK                                     0x00700000
#define B0_TX_HANDLER_AC_EXT_CW_REMAPPING_FIELD_AC_BEACON_CW_REMAPPING_STA_MASK                                   0x07000000
//========================================
/* REG_TX_HANDLER_LCG_RAND_FUNC_SEED 0x2C */
#define B0_TX_HANDLER_LCG_RAND_FUNC_SEED_FIELD_LCG_RAND_FUNC_SEED_MASK                                            0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LCG_RAND_FUNC_SHIFT_NUM 0x30 */
#define B0_TX_HANDLER_LCG_RAND_FUNC_SHIFT_NUM_FIELD_LCG_RAND_FUNC_SHIFT_NUM_MASK                                  0x0000001F
//========================================
/* REG_TX_HANDLER_TXH_MAP_CONTROL 0x34 */
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_TXH_MAP_ENABLE_MASK                                                   0x00000001
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_FREEZE_IN_AGG_BUILDER_ERROR_EN_MASK                                   0x00000002
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_PUNISH_ON_INTERNAL_COLLISION_EN_MASK                                  0x00000004
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_INCREASE_ON_INT_COL_SSRC_SLRC_N_MASK                                  0x00000008
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_TX_GROUP_ADDRESSED_SET_ECWMIN_EN_MASK                                 0x00000010
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_RETRY_CTR_LIMIT_ZEROED_CTR_EN_MASK                                    0x00000020
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_PUNISH_WINNER_ON_COLLISION_EN_MASK                                    0x00000040
#define B0_TX_HANDLER_TXH_MAP_CONTROL_FIELD_IGNORE_MU_EDCA_TIMER_EXPIRED_MASK                                     0x00000080
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_BE 0x40 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_BE_FIELD_TX_REQ_VAP_AC_BE_MASK                                                0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_BK 0x44 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_BK_FIELD_TX_REQ_VAP_AC_BK_MASK                                                0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_VI 0x48 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_VI_FIELD_TX_REQ_VAP_AC_VI_MASK                                                0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_VO 0x4C */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_VO_FIELD_TX_REQ_VAP_AC_VO_MASK                                                0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_GPLP 0x50 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_GPLP_FIELD_TX_REQ_VAP_AC_GPLP_MASK                                            0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_GPHP 0x54 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_GPHP_FIELD_TX_REQ_VAP_AC_GPHP_MASK                                            0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TX_REQ_VAP_AC_BEACON 0x58 */
#define B0_TX_HANDLER_TX_REQ_VAP_AC_BEACON_FIELD_TX_REQ_VAP_AC_BEACON_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_BE 0x5C */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_BE_FIELD_LAST_SLOT_VAP_AC_BE_MASK                                          0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_BK 0x60 */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_BK_FIELD_LAST_SLOT_VAP_AC_BK_MASK                                          0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_VI 0x64 */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_VI_FIELD_LAST_SLOT_VAP_AC_VI_MASK                                          0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_VO 0x68 */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_VO_FIELD_LAST_SLOT_VAP_AC_VO_MASK                                          0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_GPLP 0x6C */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPLP_FIELD_LAST_SLOT_VAP_AC_GPLP_MASK                                      0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_GPHP 0x70 */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_GPHP_FIELD_LAST_SLOT_VAP_AC_GPHP_MASK                                      0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_LAST_SLOT_VAP_AC_BEACON 0x74 */
#define B0_TX_HANDLER_LAST_SLOT_VAP_AC_BEACON_FIELD_LAST_SLOT_VAP_AC_BEACON_MASK                                  0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_WINNER_RESULT 0x7C */
#define B0_TX_HANDLER_WINNER_RESULT_FIELD_WINNER_VAP_ID_MASK                                                      0x0000001F
#define B0_TX_HANDLER_WINNER_RESULT_FIELD_WINNER_AC_ID_MASK                                                       0x000000E0
//========================================
/* REG_TX_HANDLER_UNFREEZE_REPORT_SET 0x80 */
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_UNFREEZE_COMMAND_MASK                                             0x00000001
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_CTS_RX_INFO_UPDATE_MASK                                           0x00000002
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_TX_RESULT_UPDATE_MASK                                             0x00000004
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_CTS_RX_INFO_MASK                                                  0x00000300
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_TX_RESULT_MASK                                                    0x00030000
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_TX_FRAME_LENGTH_MASK                                              0x00040000
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_TX_HE_UL_TB_SUCCESS_AC_BITMAP_MASK                                0x00780000
#define B0_TX_HANDLER_UNFREEZE_REPORT_SET_FIELD_TX_HE_UL_TB_VAPID_MASK                                            0x0F800000
//========================================
/* REG_TX_HANDLER_UNFREEZE_REPORT 0x84 */
#define B0_TX_HANDLER_UNFREEZE_REPORT_FIELD_CTS_RX_INFO_RD_MASK                                                   0x00000300
#define B0_TX_HANDLER_UNFREEZE_REPORT_FIELD_TX_RESULT_RD_MASK                                                     0x00030000
#define B0_TX_HANDLER_UNFREEZE_REPORT_FIELD_TX_FRAME_LENGTH_RD_MASK                                               0x00040000
#define B0_TX_HANDLER_UNFREEZE_REPORT_FIELD_TX_HE_UL_TB_SUCCESS_AC_BITMAP_RD_MASK                                 0x00780000
#define B0_TX_HANDLER_UNFREEZE_REPORT_FIELD_TX_HE_UL_TB_VAPID_RD_MASK                                             0x0F800000
//========================================
/* REG_TX_HANDLER_TXH_MAP_DBG 0x88 */
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TXH_MAP_SM_MASK                                                           0x0000001F
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_PAUSE_FDBK_SM_MASK                                                        0x00000300
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TXH_MAP_HANDLER_SM_MASK                                                   0x0000F000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TXH_MAP_IN_FREEZE_MASK                                                    0x00010000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_LAST_SLOT_MASKED_VAP_AC_MASK                                              0x00020000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TX_REQ_VAP_AC_MASK                                                        0x00040000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TX_SEL_TXHD_VAP_AC_MASK                                                   0x00080000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_MAP_HANDLER_OPERATION_MASK                                                0x00F00000
#define B0_TX_HANDLER_TXH_MAP_DBG_FIELD_TXH_MAP_HE_UL_TB_MODE_MASK                                                0x01000000
//========================================
/* REG_TX_HANDLER_RECIPE_PTR_AC_ID0TO3 0x8C */
#define B0_TX_HANDLER_RECIPE_PTR_AC_ID0TO3_FIELD_RECIPE_PTR_AC_ID0TO3_MASK                                        0x03FFFFFF
//========================================
/* REG_TX_HANDLER_RECIPE_PTR_AC_GPLP 0x90 */
#define B0_TX_HANDLER_RECIPE_PTR_AC_GPLP_FIELD_RECIPE_PTR_AC_GPLP_MASK                                            0x03FFFFFF
//========================================
/* REG_TX_HANDLER_RECIPE_PTR_AC_GPHP 0x94 */
#define B0_TX_HANDLER_RECIPE_PTR_AC_GPHP_FIELD_RECIPE_PTR_AC_GPHP_MASK                                            0x03FFFFFF
//========================================
/* REG_TX_HANDLER_RECIPE_PTR_AC_BEACON 0x98 */
#define B0_TX_HANDLER_RECIPE_PTR_AC_BEACON_FIELD_RECIPE_PTR_AC_BEACON_MASK                                        0x03FFFFFF
//========================================
/* REG_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS 0x9C */
#define B0_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS_FIELD_TXH_NTD_CCA_SENSITIVE_MASK                                      0x00000001
#define B0_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS_FIELD_TXH_NTD_RX_NAV_SENSITIVE_MASK                                   0x00000002
#define B0_TX_HANDLER_TXH_NTD_TX_REQ_PARAMS_FIELD_TXH_NTD_TX_NAV_SENSITIVE_MASK                                   0x00000004
//========================================
/* REG_TX_HANDLER_TXH_EPSILON_MICRO_PRESCALER_NUM 0xA0 */
#define B0_TX_HANDLER_TXH_EPSILON_MICRO_PRESCALER_NUM_FIELD_TXH_EPSILON_MICRO_PRESCALER_NUM_MASK                  0x000003FF
#define B0_TX_HANDLER_TXH_EPSILON_MICRO_PRESCALER_NUM_FIELD_TXH_EPSILON_MICRO_PRESCALER_NUM_FREQ_REDUCED_MASK     0x03FF0000
//========================================
/* REG_TX_HANDLER_TXH_EPSILON_MICRO_NUM 0xA4 */
#define B0_TX_HANDLER_TXH_EPSILON_MICRO_NUM_FIELD_TXH_EPSILON_MICRO_NUM_MASK                                      0x000000FF
//========================================
/* REG_TX_HANDLER_TXH_LOGGER 0xA8 */
#define B0_TX_HANDLER_TXH_LOGGER_FIELD_TXH_LOGGER_EN_MASK                                                         0x00000001
#define B0_TX_HANDLER_TXH_LOGGER_FIELD_TXH_LOGGER_PRIORITY_MASK                                                   0x00000300
//========================================
/* REG_TX_HANDLER_TXH_LOGGER_ACTIVE 0xAC */
#define B0_TX_HANDLER_TXH_LOGGER_ACTIVE_FIELD_TXH_LOGGER_ACTIVE_MASK                                              0x00000001
//========================================
/* REG_TX_HANDLER_DEBUG_MU_EDCA_TIMER_RESOLUTION 0xB0 */
#define B0_TX_HANDLER_DEBUG_MU_EDCA_TIMER_RESOLUTION_FIELD_DEBUG_MU_EDCA_TIMER_RESOLUTION_MASK                    0x00000007
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_REP_CYCLE_COUNT 0x104 */
#define B0_TX_HANDLER_TXH_PAUSER_REP_CYCLE_COUNT_FIELD_TXH_PAUSER_REP_CYCLE_COUNT_MASK                            0x000000FF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_REP_DURATION 0x108 */
#define B0_TX_HANDLER_TXH_PAUSER_REP_DURATION_FIELD_TXH_PAUSER_REP_PAUSE_DUR_MASK                                 0x0000FFFF
#define B0_TX_HANDLER_TXH_PAUSER_REP_DURATION_FIELD_TXH_PAUSER_REP_RESUME_DUR_MASK                                0xFFFF0000
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_STATUS 0x10C */
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_VAP_AC_IN_USED_MASK                                      0x00000001
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_WAIT_FOR_CONFIRM_MASK                                    0x00000002
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_REP_MODE_SEQ_MASK                                        0x00000004
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_REP_LIMITED_INFINITE_N_MASK                              0x00000008
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_REP_PAUSE_PERIOD_MASK                                    0x00000010
#define B0_TX_HANDLER_TXH_PAUSER_STATUS_FIELD_TXH_PAUSER_REP_COUNTER_MASK                                         0x0000FF00
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_BE 0x110 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BE_FIELD_TXH_PAUSER_VAP_AC_BE_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_BK 0x114 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BK_FIELD_TXH_PAUSER_VAP_AC_BK_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_VI 0x118 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VI_FIELD_TXH_PAUSER_VAP_AC_VI_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_VO 0x11C */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_VO_FIELD_TXH_PAUSER_VAP_AC_VO_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_GPLP 0x120 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPLP_FIELD_TXH_PAUSER_VAP_AC_GPLP_MASK                                    0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_GPHP 0x124 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_GPHP_FIELD_TXH_PAUSER_VAP_AC_GPHP_MASK                                    0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_VAP_AC_BEACON 0x128 */
#define B0_TX_HANDLER_TXH_PAUSER_VAP_AC_BEACON_FIELD_TXH_PAUSER_VAP_AC_BEACON_MASK                                0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_DBG 0x12C */
#define B0_TX_HANDLER_TXH_PAUSER_DBG_FIELD_TXH_PAUSER_SM_MASK                                                     0x0000000F
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_IRQ 0x130 */
#define B0_TX_HANDLER_TXH_PAUSER_IRQ_FIELD_TXH_PAUSER_IRQ_MASK                                                    0x00000001
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_CLEAR_IRQ 0x134 */
#define B0_TX_HANDLER_TXH_PAUSER_CLEAR_IRQ_FIELD_TXH_PAUSER_CLEAR_IRQ_MASK                                        0x00000001
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_GENERATE_IRQ_COMMANDS 0x138 */
#define B0_TX_HANDLER_TXH_PAUSER_GENERATE_IRQ_COMMANDS_FIELD_TXH_PAUSER_GENERATE_IRQ_COMMANDS_MASK                0x0000FFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_COMMAND_VAP_VECTOR 0x13C */
#define B0_TX_HANDLER_TXH_PAUSER_COMMAND_VAP_VECTOR_FIELD_TXH_PAUSER_VAP_VECTOR_MASK                              0xFFFFFFFF
//========================================
/* REG_TX_HANDLER_TXH_PAUSER_COMMAND 0x140 */
#define B0_TX_HANDLER_TXH_PAUSER_COMMAND_FIELD_TXH_PAUSER_COMMAND_MASK                                            0x0000000F
#define B0_TX_HANDLER_TXH_PAUSER_COMMAND_FIELD_TXH_PAUSER_REPETITION_TYPE_MASK                                    0x00000010
#define B0_TX_HANDLER_TXH_PAUSER_COMMAND_FIELD_TXH_PAUSER_AC_VECTOR_MASK                                          0x0000FF00
//========================================
/* REG_TX_HANDLER_SPARE_REGISTERS 0x1FC */
#define B0_TX_HANDLER_SPARE_REGISTERS_FIELD_SPARE_REGISTERS_MASK                                                  0x0000FFFF


#endif // _TX_HANDLER_REGS_H_
