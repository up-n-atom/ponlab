
/***********************************************************************************
File:				B0AutoRespRegs.h
Module:				b0AutoResp
SOC Revision:		latest
Generated at:       2024-06-26 12:54:23
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_AUTO_RESP_REGS_H_
#define _B0_AUTO_RESP_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_AUTO_RESP_BASE_ADDRESS                                     MEMORY_MAP_UNIT_25_BASE_ADDRESS
#define	REG_B0_AUTO_RESP_AUTO_RESP_OUTOF_IDLE_EN                         (B0_AUTO_RESP_BASE_ADDRESS + 0x0)
#define	REG_B0_AUTO_RESP_AUTO_RESP_SW_CLR                                (B0_AUTO_RESP_BASE_ADDRESS + 0x4)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG                           (B0_AUTO_RESP_BASE_ADDRESS + 0x8)
#define	REG_B0_AUTO_RESP_AUTO_RESP_LOGGER                                (B0_AUTO_RESP_BASE_ADDRESS + 0xC)
#define	REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT                         (B0_AUTO_RESP_BASE_ADDRESS + 0x10)
#define	REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR                    (B0_AUTO_RESP_BASE_ADDRESS + 0x14)
#define	REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR_CLR                (B0_AUTO_RESP_BASE_ADDRESS + 0x18)
#define	REG_B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG                         (B0_AUTO_RESP_BASE_ADDRESS + 0x1C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_RATE_ADPTV_EN                         (B0_AUTO_RESP_BASE_ADDRESS + 0x24)
#define	REG_B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN                        (B0_AUTO_RESP_BASE_ADDRESS + 0x28)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN                        (B0_AUTO_RESP_BASE_ADDRESS + 0x2C)
#define	REG_B0_AUTO_RESP_CONTROL_ACK_CONFIG_TABLE                        (B0_AUTO_RESP_BASE_ADDRESS + 0x30)
#define	REG_B0_AUTO_RESP_CONTROL_BA_CONFIG_TABLE                         (B0_AUTO_RESP_BASE_ADDRESS + 0x34)
#define	REG_B0_AUTO_RESP_MANGMNT_ACK_CONFIG_TABLE                        (B0_AUTO_RESP_BASE_ADDRESS + 0x38)
#define	REG_B0_AUTO_RESP_MANAGMNT_BA_CONFIG_TABLE                        (B0_AUTO_RESP_BASE_ADDRESS + 0x3C)
#define	REG_B0_AUTO_RESP_STA_DB_MULI_TID_EN_OFFSET                       (B0_AUTO_RESP_BASE_ADDRESS + 0x40)
#define	REG_B0_AUTO_RESP_BAG_UPDT_FIFO_STAT                              (B0_AUTO_RESP_BASE_ADDRESS + 0x44)
#define	REG_B0_AUTO_RESP_AUTO_RESP_OT_CFG                                (B0_AUTO_RESP_BASE_ADDRESS + 0x60)
#define	REG_B0_AUTO_RESP_AUTO_RESP_CTR                                   (B0_AUTO_RESP_BASE_ADDRESS + 0x70)
#define	REG_B0_AUTO_RESP_MPDU_DESC_DLM_IDX                               (B0_AUTO_RESP_BASE_ADDRESS + 0x74)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_BW_TABLE                             (B0_AUTO_RESP_BASE_ADDRESS + 0x78)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_MCS_DEGRADATION_VAL                  (B0_AUTO_RESP_BASE_ADDRESS + 0x7C)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP0TO9          (B0_AUTO_RESP_BASE_ADDRESS + 0x80)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP10TO19        (B0_AUTO_RESP_BASE_ADDRESS + 0x84)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP20TO29        (B0_AUTO_RESP_BASE_ADDRESS + 0x8C)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP30TO31        (B0_AUTO_RESP_BASE_ADDRESS + 0x90)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA0TO31                    (B0_AUTO_RESP_BASE_ADDRESS + 0x94)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA32TO63                   (B0_AUTO_RESP_BASE_ADDRESS + 0x98)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA64TO95                   (B0_AUTO_RESP_BASE_ADDRESS + 0x9C)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA96TO127                  (B0_AUTO_RESP_BASE_ADDRESS + 0x100)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA128TO159                 (B0_AUTO_RESP_BASE_ADDRESS + 0x104)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA160TO191                 (B0_AUTO_RESP_BASE_ADDRESS + 0x108)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA192TO223                 (B0_AUTO_RESP_BASE_ADDRESS + 0x10C)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA224TO255                 (B0_AUTO_RESP_BASE_ADDRESS + 0x110)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1                 (B0_AUTO_RESP_BASE_ADDRESS + 0x114)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_MAX_CFG                     (B0_AUTO_RESP_BASE_ADDRESS + 0x118)
#define	REG_B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG                  (B0_AUTO_RESP_BASE_ADDRESS + 0x11C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCR_BASE_ADDR                         (B0_AUTO_RESP_BASE_ADDRESS + 0x120)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT                    (B0_AUTO_RESP_BASE_ADDRESS + 0x124)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TID_VAL_FOR_ACK_FRAME                 (B0_AUTO_RESP_BASE_ADDRESS + 0x128)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_BASE_ADDR                    (B0_AUTO_RESP_BASE_ADDRESS + 0x12C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEPTH_MINUS_ONE              (B0_AUTO_RESP_BASE_ADDRESS + 0x130)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB                   (B0_AUTO_RESP_BASE_ADDRESS + 0x134)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_RD_ENTRIES_NUM               (B0_AUTO_RESP_BASE_ADDRESS + 0x138)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_NUM_ENTRIES_COUNT            (B0_AUTO_RESP_BASE_ADDRESS + 0x13C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG                        (B0_AUTO_RESP_BASE_ADDRESS + 0x140)
#define	REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN                           (B0_AUTO_RESP_BASE_ADDRESS + 0x144)
#define	REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG                          (B0_AUTO_RESP_BASE_ADDRESS + 0x150)
#define	REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_BW_RU_TABLE                  (B0_AUTO_RESP_BASE_ADDRESS + 0x158)
#define	REG_B0_AUTO_RESP_AUTO_RESP_SPARE_REG                             (B0_AUTO_RESP_BASE_ADDRESS + 0x15C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_HE_DURATION_UPDATE_MODE               (B0_AUTO_RESP_BASE_ADDRESS + 0x160)
#define	REG_B0_AUTO_RESP_AUTO_RESP_HE_MU_TCR_BASE_ADDR                   (B0_AUTO_RESP_BASE_ADDRESS + 0x164)
#define	REG_B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS0                      (B0_AUTO_RESP_BASE_ADDRESS + 0x168)
#define	REG_B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS1                      (B0_AUTO_RESP_BASE_ADDRESS + 0x16C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0                   (B0_AUTO_RESP_BASE_ADDRESS + 0x170)
#define	REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1                   (B0_AUTO_RESP_BASE_ADDRESS + 0x174)
#define	REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR                (B0_AUTO_RESP_BASE_ADDRESS + 0x178)
#define	REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_TID_AGG_LIMIT                   (B0_AUTO_RESP_BASE_ADDRESS + 0x17C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_CTR_CONT                              (B0_AUTO_RESP_BASE_ADDRESS + 0x180)
#define	REG_B0_AUTO_RESP_INT_ERR_STATUS                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x184)
#define	REG_B0_AUTO_RESP_INT_ERR_EN                                      (B0_AUTO_RESP_BASE_ADDRESS + 0x188)
#define	REG_B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL                          (B0_AUTO_RESP_BASE_ADDRESS + 0x190)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS0                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x200)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS1                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x204)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS2                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x208)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS3                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x20C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS4                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x210)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS5                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x214)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS6                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x218)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS7                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x21C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS8                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x220)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS9                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x224)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS10                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x228)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS11                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x22C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS12                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x230)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS13                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x234)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS14                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x238)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TCS15                                 (B0_AUTO_RESP_BASE_ADDRESS + 0x23C)
#define	REG_B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1                           (B0_AUTO_RESP_BASE_ADDRESS + 0x280)
#define	REG_B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2                           (B0_AUTO_RESP_BASE_ADDRESS + 0x284)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TF_CFG                                (B0_AUTO_RESP_BASE_ADDRESS + 0x290)
#define	REG_B0_AUTO_RESP_AUTO_RESP_TF_DEBUG                              (B0_AUTO_RESP_BASE_ADDRESS + 0x29C)
#define	REG_B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG2                 (B0_AUTO_RESP_BASE_ADDRESS + 0x2A0)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_BASE_ADDR            (B0_AUTO_RESP_BASE_ADDRESS + 0x2B0)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEPTH_MINUS_ONE      (B0_AUTO_RESP_BASE_ADDRESS + 0x2B4)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB           (B0_AUTO_RESP_BASE_ADDRESS + 0x2B8)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_RD_ENTRIES_NUM       (B0_AUTO_RESP_BASE_ADDRESS + 0x2BC)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_NUM_ENTRIES_COUNT    (B0_AUTO_RESP_BASE_ADDRESS + 0x2C0)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG                (B0_AUTO_RESP_BASE_ADDRESS + 0x2C4)
#define	REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_EN                   (B0_AUTO_RESP_BASE_ADDRESS + 0x2C8)
#define	REG_B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX0                          (B0_AUTO_RESP_BASE_ADDRESS + 0x2D0)
#define	REG_B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX1                          (B0_AUTO_RESP_BASE_ADDRESS + 0x2D4)
#define	REG_B0_AUTO_RESP_AUTO_RESP_AID_FOR_MLD                           (B0_AUTO_RESP_BASE_ADDRESS + 0x2E0)
#define	REG_B0_AUTO_RESP_STD_DB_GENERAL                                  (B0_AUTO_RESP_BASE_ADDRESS + 0x300)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_AUTO_RESP_AUTO_RESP_OUTOF_IDLE_EN 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 outOfIdleEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0AutoRespAutoRespOutofIdleEn_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_SW_CLR 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcFifoClrReq : 1; //SW writes 1'b1 in order to clear the fifo between RXC and auto response module , reset value: 0x0, access type: WO
		uint32 bagUpdtFifoClrReq : 1; //SW writes 1'b1 in order to clear the fifo between auto response top and BAG bitmap update logic , reset value: 0x0, access type: WO
		uint32 userDbClrPulse : 1; //SW writes 1'b1 in order to clear auto resp user DB. Should be used each time sender writes the tx user ID and tid aggregation limit to th DB , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0AutoRespAutoRespSwClr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dynamicBitmapDisable : 1; //enable/disable of bitmap resizing: , 0 - use dynamic bitmap sizing. i.e.- change the window size to values less or equal to negotiate window bitmap size , 1 - use only the negotiate window bitmap size, reset value: 0x0, access type: RW
		uint32 barNoBaAgrAckPolicy : 1; //BAR BA agreement ACK policy --- defined if to response with ACK to BAR with no BA Agreement: , '0' - do not respond with ACK to BAR with no BA Agreement  , '1' - respond with ACK to BAR with no BA Agreement  , , reset value: 0x0, access type: RW
		uint32 barNumBitsCfg : 1; //0 - Use the fragments bits as in BA , 1 - use the window size of BA Agreement i.e. in case window size = 120 use 128 , , reset value: 0x1, access type: RW
		uint32 trafficBaAgreementRecipientEn : 1; //enable BA agreement recipient I/F. 0 - disable, 1 - enable (default) , , reset value: 0x1, access type: RW
		uint32 swUsrDbClr : 1; //sw writes 1'b1 in order to force user DB clear at the start of the next RX session , , reset value: 0x0, access type: RW
		uint32 acceptFirstSnOutOfBoundsInKeepHist : 1; //'0' - ignore first SN out of bounds in a new session with keep history. '1' - accept first SN out of bounds in a new session with keep history, reset value: 0x1, access type: RW
		uint32 heSuUseMbaInstdAck : 1; //0' - do not replace ACK with M-BA TID ACK in HE SU, '1' - replace ACK with M-BA TID ACK  in HE SU, reset value: 0x0, access type: RW
		uint32 heMuUseMbaInstdAck : 1; //0' - do not replace ACK with M-BA TID ACK in HE MU, '1' - replace ACK with M-BA TID ACK  in HE MU, reset value: 0x1, access type: RW
		uint32 supportMultiTidBar : 1; //When set to '1' multi TID BAR is supported. Otherwise ignored., reset value: 0x1, access type: RW
		uint32 supportTbMuBar : 1; //When set to '1' TB MU BAR is supported. Otherwise ignored., reset value: 0x1, access type: RW
		uint32 psAlsoOnNonMuSta : 1; //decide which PHY mode disable PS read: , 0 - phy_mode_he_eht_mu_ap , 1- phy_mode_he_eht_mu_ap OR phy_mode_he_eht_mu_sta, reset value: 0x0, access type: RW
		uint32 allowEhtMcs14Resp : 1; //1 - allow to respond in EHT MCS14 , 0 - move to 11A/G mcs0, reset value: 0x1, access type: RW
		uint32 allowEhtMcs15Resp : 1; //1 - allow to respond in EHT MCS15 , 0 - move to 11A/G mcs0, reset value: 0x1, access type: RW
		uint32 swAllowEhtLdpc : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB0AutoRespAutoRespMiscConfig_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_LOGGER 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespLoggerEn : 1; //Auto response logger enable bit, reset value: 0x0, access type: RW
		uint32 autoRespLoggerPrio : 2; //Auto response logger priority bits, reset value: 0x0, access type: RW
		uint32 loggerAllCfg : 1; //1 bit indication if to send all to logger - '1' - send all, '0' - send only rx start/end & tx, reset value: 0x1, access type: RW
		uint32 loggerCounterLimitCfg : 8; //limit of number of words sent to logger before done indication (default 'd100), reset value: 0x64, access type: RW
		uint32 loggerActive : 1; //logger active status. '1'- active, '0' - not active , reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegB0AutoRespAutoRespLogger_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcMainFifoByteCount : 3; //no description, reset value: 0x0, access type: RO
		uint32 rxcMainFifoFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcMainFifoEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcMainFifoPushIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcMainFifoPopIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
		uint32 rxcBufFifoByteCount : 2; //no description, reset value: 0x0, access type: RO
		uint32 rxcBufFifoFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcBufFifoEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcBufFifoPushIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcBufFifoPopIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 10;
	} bitFields;
} RegB0AutoRespAutoRespRxcFifoStat_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcFifoFullCntr : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0AutoRespAutoRespRxcFifoFullCntr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR_CLR 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcFifoFullCntrClrReq : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0AutoRespAutoRespRxcFifoFullCntrClr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMainFsm : 5; //no description, reset value: 0x0, access type: RO
		uint32 baAgrRdFsm : 3; //no description, reset value: 0x0, access type: RO
		uint32 usrStaRdFsm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 tidBmapFsm : 4; //no description, reset value: 0x0, access type: RO
		uint32 arespMtidSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 arespMuBarSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 10;
	} bitFields;
} RegB0AutoRespAutoRespSmStatDebug_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_RATE_ADPTV_EN 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rateAdptvRptEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0AutoRespAutoRespRateAdptvEn_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cbaBitmapLen64 : 4; //fragment number subfield bits setting in case of bitmap length 64 bit, reset value: 0x0, access type: RW
		uint32 cbaBitmapLen128 : 4; //fragment number subfield bits setting in case of bitmap length 128 bit, reset value: 0x2, access type: RW
		uint32 cbaBitmapLen256 : 4; //fragment number subfield bits setting in case of bitmap length 128 bit, reset value: 0x4, access type: RW
		uint32 cbaBitmapLen32 : 4; //fragment number subfield bits setting in case of bitmap length 32 bit, reset value: 0x6, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespAutoRespCbaBitmapLen_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mbaBitmapLen64 : 4; //fragment number subfield bits setting in case of bitmap length 64 bit, reset value: 0x0, access type: RW
		uint32 mbaBitmapLen128 : 4; //fragment number subfield bits setting in case of bitmap length 128 bit, reset value: 0x2, access type: RW
		uint32 mbaBitmapLen256 : 4; //fragment number subfield bits setting in case of bitmap length 256 bit, reset value: 0x4, access type: RW
		uint32 mbaBitmapLen32 : 4; //fragment number subfield bits setting in case of bitmap length 32 bit, reset value: 0x6, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespAutoRespMbaBitmapLen_u;

/*REG_B0_AUTO_RESP_CONTROL_ACK_CONFIG_TABLE 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlAckConfig : 16; //no description, reset value: 0x400, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespControlAckConfigTable_u;

/*REG_B0_AUTO_RESP_CONTROL_BA_CONFIG_TABLE 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlBaConfig : 16; //no description, reset value: 0x400, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespControlBaConfigTable_u;

/*REG_B0_AUTO_RESP_MANGMNT_ACK_CONFIG_TABLE 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mangmntAckConfig : 16; //no description, reset value: 0xBFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespMangmntAckConfigTable_u;

/*REG_B0_AUTO_RESP_MANAGMNT_BA_CONFIG_TABLE 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mangmntBaConfig : 16; //no description, reset value: 0xBFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespManagmntBaConfigTable_u;

/*REG_B0_AUTO_RESP_STA_DB_MULI_TID_EN_OFFSET 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staDbMultiTidEn4BOffset : 8; //sta tx multi tid enalbed field address offset inside sta db in 4 bytes alignment, reset value: 0xA, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0AutoRespStaDbMuliTidEnOffset_u;

/*REG_B0_AUTO_RESP_BAG_UPDT_FIFO_STAT 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bagUpdtFifoByteCount : 4; //no description, reset value: 0x0, access type: RO
		uint32 bagUpdtFifoFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagUpdtFifoEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagUpdtFifoPushIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagUpdtFifoPopIgnored : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0AutoRespBagUpdtFifoStat_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_OT_CFG 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableInternalBaAgrInd : 1; //This bit is used for fixing WLANVLSIIP-4954. Set this bit to 1 to override the fix made for this Jira, reset value: 0x1, access type: RW
		uint32 swUsePhyModeNonTfForDur : 1; //define if phy mode 6 or 7 are used for special dur calculations. , 1'b0: take phy mode 7 , 1'b1: take phy mode 6 , , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0AutoRespAutoRespOtCfg_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_CTR 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespTxCtr : 8; //Auto response Tx counter, reset value: 0x0, access type: RO
		uint32 autoRespRxNavAbortCtr : 8; //Auto response Rx NAV abort counter, reset value: 0x0, access type: RO
		uint32 autoRespTxNavAbortCtr : 8; //Auto response Tx NAV abort counter, reset value: 0x0, access type: RO
		uint32 autoRespStaticBwAbortCtr : 8; //Auto response Static BW abort counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespCtr_u;

/*REG_B0_AUTO_RESP_MPDU_DESC_DLM_IDX 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduDescFreeListIdx : 8; //sets the MPDU descriptor free list index for Auto response, reset value: 0x0, access type: RW
		uint32 perUserTxBaseDlmListIdx : 8; //sets the MPDU per user descriptor base index for Auto response , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0AutoRespMpduDescDlmIdx_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_BW_TABLE 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyBwTable : 15; //Auto reply BW table , 3 bits per input BW, reset value: 0x7688, access type: RW
		uint32 reserved0 : 9;
		uint32 autoReplyDisallowedChannelBhvEht : 2; //Behavior in case of a collision with a disallowed channel, one or more. For EHT phy mode in RX (not in case of MCS14 or MCS15) , 0 - Reduce to RU and transmit , 1 - Abort transmission , 2 - Ignore and transmit on those disallowed channels , 3 - Static behavior, full CBW minus forbidden channel, reset value: 0x3, access type: RW
		uint32 autoReplyDisallowedChannelBhvNonEht : 2; //Behavior in case of a collision with a disallowed channel, one or more. For non EHT phy mode in RX , 0 - Reduce to RU and transmit , 1 - Abort transmission , 2 - Ignore and transmit on those disallowed channels , 3 - Static behavior, full CBW minus forbidden channel, reset value: 0x0, access type: RW
		uint32 autoReplyDisallowedChannelBhvEhtLowMcs : 2; //Behavior in case of a collision with a disallowed channel, one or more. For EHT phy mode in RX with MCS14 or MCS15 , 0 - Reduce to RU and transmit , 1 - Abort transmission , 2 - Ignore and transmit on those disallowed channels , 3 - Static behavior, full CBW minus forbidden channel, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoReplyBwTable_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_MCS_DEGRADATION_VAL 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyMcsDegradationVal : 4; //Auto reply degradation value for MCS in, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespAutoReplyMcsDegradationVal_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP0TO9 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrDegradationValVap0To9 : 30; //Auto reply degradation value for power, 3 bit per VAP, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB0AutoRespAutoReplyPwrDegradationValVap0To9_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP10TO19 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrDegradationValVap10To19 : 30; //Auto reply degradation value for power, 3 bit per VAP, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB0AutoRespAutoReplyPwrDegradationValVap10To19_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP20TO29 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrDegradationValVap20To29 : 30; //Auto reply degradation value for power, 3 bit per VAP, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB0AutoRespAutoReplyPwrDegradationValVap20To29_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP30TO31 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrDegradationValVap30To31 : 6; //Auto reply degradation value for power, 3 bit per VAP, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0AutoRespAutoReplyPwrDegradationValVap30To31_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA0TO31 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta0To31 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta0To31_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA32TO63 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta32To63 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta32To63_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA64TO95 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta64To95 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta64To95_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA96TO127 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta96To127 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta96To127_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA128TO159 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta128To159 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta128To159_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA160TO191 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta160To191 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta160To191_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA192TO223 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta192To223 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta192To223_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA224TO255 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPwrNearSta224To255 : 32; //Auto reply STA is near indication, one bit per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoReplyPwrNearSta224To255_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPhyMode : 4; //auto reply phy mode, reset value: 0x0, access type: RO
		uint32 autoReplyCbw : 3; //auto reply bw, reset value: 0x0, access type: RO
		uint32 autoReplyMcs : 8; //auto_ reply mcs, reset value: 0x0, access type: RO
		uint32 autoReplyTxPower : 8; //auto reply tx power, reset value: 0x0, access type: RO
		uint32 autoReplyRuSize : 4; //auto reply ru size, reset value: 0x0, access type: RO
		uint32 autoReplyLdpc : 1; //auto reply ldpc, reset value: 0x0, access type: RO
		uint32 autoReplyMru : 4; //auto reply mru, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoReplyOverrideParamsDbg1_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_MAX_CFG 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyBwTableMaxCfg : 3; //auto reply BW select max val (deafult 2'd3 --> 160 MHz). Represents the Maximum BW that Auto response can use for response frame. it is compared to the BW calculation logic and used only if calculated value is grater then it., reset value: 0x4, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0AutoRespAutoReplyBwTableMaxCfg_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bag1StStageSmOut : 3; //no description, reset value: 0x0, access type: RO
		uint32 bag2NdStageSmOut : 4; //no description, reset value: 0x0, access type: RO
		uint32 bagFrameGenClkEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 selectedUser : 8; //no description, reset value: 0x0, access type: RO
		uint32 selectedTid : 4; //no description, reset value: 0x0, access type: RO
		uint32 selectedAckType : 3; //no description, reset value: 0x0, access type: RO
		uint32 dbWrEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 dbDataOutAvailable : 1; //no description, reset value: 0x0, access type: RO
		uint32 readRaTaCount : 2; //no description, reset value: 0x0, access type: RO
		uint32 bagFrameGenToRxcSmcSmcReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagFrameGenToUpMpduDlmValid : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagFrameGenToMpduDescRamSmcSmcReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 bagFrameGenToTxCircBufSmcSmcReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB0AutoRespAutoRespFrameGenStatDebug_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCR_BASE_ADDR 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespTcrBaseAddr : 26; //Base address of the first common TCR in SU TXOP (byte aligned)), reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0AutoRespAutoRespTcrBaseAddr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 nullMpduCnt : 8; //mpdu pointer null counter. Counts up each time received a null pointer rom q_manager when requesting a free mpdu pointer, reset value: 0x0, access type: RO
		uint32 nullMpduCntOvflw : 1; //mpdu pointer null counter overflow. Asserts if counter reaches saturation, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 nullMpduCntClr : 1; //SW writes 1'b1 in order to clear the mpdu null ptr counter status register , reset value: 0x0, access type: WO
		uint32 reserved1 : 15;
	} bitFields;
} RegB0AutoRespAutoRespMpduNullPtrStat_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TID_VAL_FOR_ACK_FRAME 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduTidFieldVal : 4; //This is the TID val that is put in the TID field of the ACK frame sent, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespAutoRespTidValForAckFrame_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_BASE_ADDR 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoBaseAddr : 22; //AUTO RESP ERROR FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoBaseAddr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEPTH_MINUS_ONE 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoDepthMinusOne : 8; //AUTO RESP ERROR FIFO depth minus one, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoDepthMinusOne_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoClearStrb : 1; //Clear AUTO RESP ERROR FIFO. , By write '1'., reset value: 0x0, access type: WO
		uint32 autoRespErrFifoClearFullDropCtrStrb : 1; //Clear AUTO RESP ERROR FIFO full drop counter. , By write '1'., reset value: 0x0, access type: WO
		uint32 autoRespErrFifoClearDecLessThanZeroStrb : 1; //Clear AUTO RESP ERROR FIFO decrement amount less than zero indication. , By write '1'.  , , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoClearStrb_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_RD_ENTRIES_NUM 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoRdEntriesNum : 9; //AUTO RESP ERROR FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoRdEntriesNum_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_NUM_ENTRIES_COUNT 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoNumEntriesCount : 9; //AUTO RESP ERROR FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoNumEntriesCount_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoWrPtr : 8; //AUTO RESP ERROR FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 autoRespErrFifoNotEmpty : 1; //AUTO RESP ERROR FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 autoRespErrFifoFull : 1; //AUTO RESP ERROR FIFO full indication, reset value: 0x0, access type: RO
		uint32 autoRespErrFifoDecrementLessThanZero : 1; //AUTO RESP ERROR FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 autoRespErrFifoFullDropCtr : 8; //AUTO RESP ERROR FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespErrFifoDebug_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoMltiTidEn : 1; //AUTO RESP ERROR FIFO multi TID error enable., reset value: 0x0, access type: RW
		uint32 autoRespErrFifoBarErrEn : 1; //AUTO RESP ERROR FIFO BAR error enable., reset value: 0x0, access type: RW
		uint32 autoRespErrFifoSnOutBoundEn : 1; //AUTO RESP ERROR FIFO NUM SN out of bounds error enable., reset value: 0x0, access type: RW
		uint32 autoRespErrFifoMultiTidBarLengthEn : 1; //AUTO RESP ERROR FIFO MULTI TID BAR LENGTH error enable., reset value: 0x0, access type: RW
		uint32 autoRespErrFifoImmToNonImmEn : 1; //AUTO RESP ERROR FIFO Immediate to Non-Immediate error enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0AutoRespAutoRespErrFifoEn_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSuErTxRuSizeRespForHeSu : 1; //Defines the transmitted HE SU ER RU Size when received HE SU PPDU with BW = 20MHz. , 0 - RU242 , 1 - RU106, reset value: 0x0, access type: RW
		uint32 heSuErTxRuSizeRespForHeSuErRu242 : 1; //Defines the transmitted HE SU ER RU Size when received HE SU ER PPDU with RU=242. , 0 - RU242 , 1 - RU106, reset value: 0x0, access type: RW
		uint32 heSuErTxRuSizeRespForHeSuErRu106 : 1; //Defines the transmitted HE SU ER RU Size when received HE SU ER PPDU with RU=106. , 0 - RU242 , 1 - RU106, reset value: 0x1, access type: RW
		uint32 heSuErTxDcmRespForHeSuOrHeSuErDcm1 : 1; //Defines how to set the HE SU ER DCM when the received PPDU is HE SU BW20Mhz or HE SU ER with DCM. , 0 - response without DCM , 1 - response with DCM, reset value: 0x1, access type: RW
		uint32 heSuErTxDcmRespForHeSuDcm0 : 1; //Defines how to set the HE SU ER DCM when the received PPDU is HE SU BW20Mhz without DCM. , 0 - response without DCM , 1 - response with DCM, reset value: 0x1, access type: RW
		uint32 heSuErTxDcmRespForHeSuErDcm0 : 1; //Defines how to set the HE SU ER DCM when the received PPDU is HE SU ER without DCM. , 0 - response without DCM , 1 - response with DCM, reset value: 0x1, access type: RW
		uint32 heSuErUnknownStaRespType : 1; //Defines the ACK PHY Mode respond when received HE SU ER PPDU from Unknown STA. , 0 --- 11A/G , 1 --- HE SU ER, RU Size = 242, reset value: 0x1, access type: RW
		uint32 heSuErTxDcmRespForUnknownStaDcm0 : 1; //Defines how to set the HE SU ER DCM when the received PPDU is from HE SU ER Unknown STA without DCM. , 0 - response without DCM , 1 - response with DCM, reset value: 0x1, access type: RW
		uint32 heSuErTxDcmRespForUnknownStaDcm1 : 1; //Defines how to set the HE SU ER DCM when the received PPDU is from HE SU ER Unknown STA with DCM. , 0 - response without DCM , 1 - response with DCM, reset value: 0x1, access type: RW
		uint32 readOmiHeErDisableBitFromSelector : 1; //Read OMI HE ER Disable Bit from Selector bitmap, defines if to use the OMI HE ER Disable bit from the Selector to disable HE SU ER. , 0 - don't read OMI HE ER Disable Bit , 1 - read OMI HE ER Disable bit and use it for enable/disable HE SU ER, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0AutoRespAutoRespHeSuErCfg_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_BW_RU_TABLE 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSuErRuSize4Bw10 : 3; //Auto Resp HE SU ER RU size value for received BW of 10 MHz. default = 3'd2 - 106 , reset value: 0x2, access type: RW
		uint32 heSuErRuSize4Bw20 : 3; //Auto Resp HE SU ER RU size value for received BW of 20 MHz. default = 3'd3 - 242 , reset value: 0x3, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0AutoRespAutoRespHeSuErBwRuTable_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_SPARE_REG 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespSpareReg : 32; //Auto resp spare register, reset value: 0x88888888, access type: RW
	} bitFields;
} RegB0AutoRespAutoRespSpareReg_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_HE_DURATION_UPDATE_MODE 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespHeDurationUpdateMode : 1; //one bit en/disable. '1' -full TXOP.  '0' - partial duration. In this case auto resp in HE MU selects partial duration base mode for deila duration source , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0AutoRespAutoRespHeDurationUpdateMode_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_HE_MU_TCR_BASE_ADDR 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespHeMuTcrBaseAddr : 26; //Base address of the first common TCR in HE MU TXOP (byte aligned)), reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0AutoRespAutoRespHeMuTcrBaseAddr_u;

/*REG_B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS0 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user310WasActive : 32; //user31 - user0 tx was active bus reflected to debug status register , reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespUserTxWasActiveStatus0_u;

/*REG_B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS1 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user3532WasActive : 4; //user35 - user32 tx was active bus reflected to debug status register , reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespUserTxWasActiveStatus1_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyErrNoRespDebugCntr : 8; //No response due to phy error indcation debug counter. Counter is incremented by one for each RX session that ended with no Auto resp due to phy error, reset value: 0x0, access type: RO
		uint32 seqAbortNoRespDebugCntr : 8; //No response due to sequencer abort indcation debug counter. Counter is incremented by one for each RX session that ended with no Auto resp due to sequencer abort, reset value: 0x0, access type: RO
		uint32 validStaNoRespDebugCntr : 8; //No response due to no valid station debug counter. Counter is incremented by one for each RX session that ended with no auto resp due to no valid station indication in RX session, reset value: 0x0, access type: RO
		uint32 unicastBitNoRespDebugCntr : 8; //No response due to unicast bit not set debug counter. Counter is incremented by one for each RX session that ended with no auto resp due to unicast bit never set in RX session, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespNoRespDebugCntr0_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ntdAbortNoRespDebugCntr : 8; //No response due to NTD abort debug counter. Counter is incremented by one for each RX session that ended with no Auto resp due to NTD abort, reset value: 0x0, access type: RO
		uint32 bagPushNoRespDebugCntr : 8; //No response due to no ackable mpdu debug counter. Counter is incremented by one for each RX session that ended with no Auto resp due to no ackable MPDU received from RXC, reset value: 0x0, access type: RO
		uint32 noRespStatus : 6; //No response status bus {no_push_to_bag_fifo,ntd_decision_is_fail,unicast_bit_was_never_set,no_valid_sta_in_rx,seq_abort_no_resp,phy_err_no_resp} , reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 rxcPhyErr : 1; //indication phy error was asserted, reset value: 0x0, access type: RO
		uint32 rxNavZero : 1; //rx nav zero indicaiton reflected from module input, reset value: 0x0, access type: RO
		uint32 txNavZero : 1; //tx nav zero indicaiton reflected from module input, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
	} bitFields;
} RegB0AutoRespAutoRespNoRespDebugCntr1_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyErrDebugCntrClr : 1; //SW writes 1'b1 in order to clear phy error no resp debug counter , reset value: 0x0, access type: WO
		uint32 seqAbortDebugCntrClr : 1; //SW writes 1'b1 in order to clear sequencer abort no resp debug counter , reset value: 0x0, access type: WO
		uint32 validStaDebugCntrClr : 1; //SW writes 1'b1 in order to clear no valid STA no resp debug counter , reset value: 0x0, access type: WO
		uint32 unicastBitDebugCntrClr : 1; //SW writes 1'b1 in order to clear unicast bit no resp debug counter , reset value: 0x0, access type: WO
		uint32 ntdAbortDebugCntrClr : 1; //SW writes 1'b1 in order to clear ntd abort no resp debug counter , reset value: 0x0, access type: WO
		uint32 bagPushDebugCntrClr : 1; //SW writes 1'b1 in order to clear bag push no resp debug counter , reset value: 0x0, access type: WO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0AutoRespAutoRespNoRespDebugCntrClr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_HE_SU_TID_AGG_LIMIT 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSuTidAggLimit : 3; //TID aggregation limit in HE SU. This is the max num if TIDs that can be received and answered with ACK/BA. The value set is considered as N-1, reset value: 0x7, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0AutoRespAutoRespHeSuTidAggLimit_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_CTR_CONT 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespBfAbortCtr : 8; //Auto response BF abort counter, reset value: 0x0, access type: RO
		uint32 autoRespTfCcaAbortCtr : 8; //Auto response CCA abort in TB frames, reset value: 0x0, access type: RO
		uint32 autoRespTxhFreezeAbortCtr : 8; //Auto response TxH Freeze abort, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0AutoRespAutoRespCtrCont_u;

/*REG_B0_AUTO_RESP_INT_ERR_STATUS 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespErrFifoFullDropIrq : 1; //Auto response Error FIFO drop while full IRQ, reset value: 0x0, access type: RO
		uint32 autoRespErrFifoDecrementLessThanZeroIrq : 1; //Auto response Error FIFO decrement less than zero IRQ, reset value: 0x0, access type: RO
		uint32 autoRespMubarHwSwFifoFullDropIrq : 1; //Auto response MU-BAR Hw-SW FIFO drop while full IRQ, reset value: 0x0, access type: RO
		uint32 autoRespMubarHwSwFifoDecrementLessThanZeroIrqAgain : 1; //Auto response MU-BAR Hw-SW FIFO decrement less than zero IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespIntErrStatus_u;

/*REG_B0_AUTO_RESP_INT_ERR_EN 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enAutoRespErrFifoFullDropIrq : 1; //Enable auto_resp_err_fifo_full_drop_irq., reset value: 0x1, access type: RW
		uint32 enAutoRespErrFifoDecrementLessThanZeroIrq : 1; //enable auto_resp_err_fifo_decrement_less_than_zero_irq., reset value: 0x1, access type: RW
		uint32 enAutoRespMubarHwSwFifoFullDropIrq : 1; //Enable auto_resp_mubar_hw_sw_fifo_full_drop_irq., reset value: 0x1, access type: RW
		uint32 enAutoRespMubarHwSwFifoDecrementLessThanZeroIrq : 1; //enable auto_resp_mubar_hw_sw_fifo_decrement_less_than_zero_irq., reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespIntErrEn_u;

/*REG_B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearHistoryPerTidEn : 1; //configuration if to clear history per TID or not: , 0 - clear history is done to all TIDs together , 1 - clear history is done per TID  , reset value: 0x1, access type: RW
		uint32 keepHistoryToTidCntInPerUser : 1; //configuration if to keep history in per user TID count or not: , 0 - clear history is done to all counters , 1 - clear history is not done, reset value: 0x1, access type: RW
		uint32 keepHistoryEnabled : 1; //Enable keep history feature , 0 - keep history disabled , 1 - keep history enabled, reset value: 0x1, access type: RW
		uint32 keepHistoryOnlyBar : 1; //1'b0 - keep history behaves as in W600 project , 1'b1 - keep history is only in case a BAR was received, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0AutoRespClearHistoryTidCtrl_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS0 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp0RecipePtr : 13; //Auto response 0 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp0RxNavAbort : 1; //Auto response 0 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp0TxNavAbort : 1; //Auto response 0 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp0StaticBwAbort : 1; //Auto response 0 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp0BfAbort : 1; //Auto response 0 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp0TxhFreezeAbort : 1; //Auto response 0 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs0_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS1 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp1RecipePtr : 13; //Auto response 1 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp1RxNavAbort : 1; //Auto response 1 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp1TxNavAbort : 1; //Auto response 1 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp1StaticBwAbort : 1; //Auto response 1 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp1BfAbort : 1; //Auto response 1 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp1TxhFreezeAbort : 1; //Auto response 1 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs1_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS2 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp2RecipePtr : 13; //Auto response 2 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp2RxNavAbort : 1; //Auto response 2 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp2TxNavAbort : 1; //Auto response 2 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp2StaticBwAbort : 1; //Auto response 2 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp2BfAbort : 1; //Auto response 2 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp2TxhFreezeAbort : 1; //Auto response 2 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs2_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS3 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp3RecipePtr : 13; //Auto response 3 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp3RxNavAbort : 1; //Auto response 3 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp3TxNavAbort : 1; //Auto response 3 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp3StaticBwAbort : 1; //Auto response 3 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp3BfAbort : 1; //Auto response 3 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp3TxhFreezeAbort : 1; //Auto response 3 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs3_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS4 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp4RecipePtr : 13; //Auto response 4 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp4RxNavAbort : 1; //Auto response 4 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp4TxNavAbort : 1; //Auto response 4 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp4StaticBwAbort : 1; //Auto response 4 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp4BfAbort : 1; //Auto response 4 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp4TxhFreezeAbort : 1; //Auto response 4 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs4_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS5 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp5RecipePtr : 13; //Auto response 5 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp5RxNavAbort : 1; //Auto response 5 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp5TxNavAbort : 1; //Auto response 5 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp5StaticBwAbort : 1; //Auto response 5 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp5BfAbort : 1; //Auto response 5 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp5TxhFreezeAbort : 1; //Auto response 5 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs5_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS6 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp6RecipePtr : 13; //Auto response 6 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp6RxNavAbort : 1; //Auto response 6 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp6TxNavAbort : 1; //Auto response 6 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp6StaticBwAbort : 1; //Auto response 6 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp6BfAbort : 1; //Auto response 6 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp6TxhFreezeAbort : 1; //Auto response 6 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs6_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS7 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp7RecipePtr : 13; //Auto response 7 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp7RxNavAbort : 1; //Auto response 7 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp7TxNavAbort : 1; //Auto response 7 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp7StaticBwAbort : 1; //Auto response 7 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp7BfAbort : 1; //Auto response 7 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp7TxhFreezeAbort : 1; //Auto response 7 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs7_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS8 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp8RecipePtr : 13; //Auto response 8 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp8RxNavAbort : 1; //Auto response 8 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp8TxNavAbort : 1; //Auto response 8 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp8StaticBwAbort : 1; //Auto response 8 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp8BfAbort : 1; //Auto response 8 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp8TxhFreezeAbort : 1; //Auto response 8 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs8_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS9 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp9RecipePtr : 13; //Auto response 9 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp9RxNavAbort : 1; //Auto response 9 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp9TxNavAbort : 1; //Auto response 9 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp9StaticBwAbort : 1; //Auto response 9 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp9BfAbort : 1; //Auto response 9 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp9TxhFreezeAbort : 1; //Auto response 9 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs9_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS10 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp10RecipePtr : 13; //Auto response 10 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp10RxNavAbort : 1; //Auto response 10 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp10TxNavAbort : 1; //Auto response 10 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp10StaticBwAbort : 1; //Auto response 10 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp10BfAbort : 1; //Auto response 10 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp10TxhFreezeAbort : 1; //Auto response 10 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs10_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS11 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp11RecipePtr : 13; //Auto response 11 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp11RxNavAbort : 1; //Auto response 11 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp11TxNavAbort : 1; //Auto response 11 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp11StaticBwAbort : 1; //Auto response 11 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp11BfAbort : 1; //Auto response 11 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp11TxhFreezeAbort : 1; //Auto response 11 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs11_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS12 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp12RecipePtr : 13; //Auto response 12 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp12RxNavAbort : 1; //Auto response 12 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp12TxNavAbort : 1; //Auto response 12 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp12StaticBwAbort : 1; //Auto response 12 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp12BfAbort : 1; //Auto response 12 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp12TxhFreezeAbort : 1; //Auto response 12 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs12_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS13 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp13RecipePtr : 13; //Auto response 13 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp13RxNavAbort : 1; //Auto response 13 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp13TxNavAbort : 1; //Auto response 13 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp13StaticBwAbort : 1; //Auto response 13 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp13BfAbort : 1; //Auto response 13 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp13TxhFreezeAbort : 1; //Auto response 13 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs13_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS14 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp14RecipePtr : 13; //Auto response 14 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp14RxNavAbort : 1; //Auto response 14 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp14TxNavAbort : 1; //Auto response 14 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp14StaticBwAbort : 1; //Auto response 14 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp14BfAbort : 1; //Auto response 14 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp14TxhFreezeAbort : 1; //Auto response 14 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs14_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TCS15 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoResp15RecipePtr : 13; //Auto response 15 recipe pointer in TX Recipe area. Auto resp will automatically add the TX Recipe base address to this value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 autoResp15RxNavAbort : 1; //Auto response 15 RX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp15TxNavAbort : 1; //Auto response 15 TX NAV abort, reset value: 0x0, access type: RW
		uint32 autoResp15StaticBwAbort : 1; //Auto response 15 Static BW abort, reset value: 0x0, access type: RW
		uint32 autoResp15BfAbort : 1; //Auto response 15 BF abort, reset value: 0x0, access type: RW
		uint32 autoResp15TxhFreezeAbort : 1; //Auto response 15 TxH Freeze abort, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0AutoRespAutoRespTcs15_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrLengthNoFcsNoDelToHob : 12; //no description, reset value: 0x0, access type: RO
		uint32 autoRespTypeSmp : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 keepHistoryInd : 1; //no description, reset value: 0x0, access type: RO
		uint32 selectRecipeType : 4; //no description, reset value: 0x0, access type: RO
		uint32 ntdTransmitSuccessLvl : 1; //no description, reset value: 0x0, access type: RO
		uint32 sequencerAutoRespAbortSmp : 1; //no description, reset value: 0x0, access type: RO
		uint32 txhNtdAutoRespTxActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 rxcRxDoneLvl : 1; //no description, reset value: 0x0, access type: RO
		uint32 phyMode : 4; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespStatDebug1_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 arespUserTxWasActvieValid : 1; //Valid indication for tx was active bus, reset value: 0x0, access type: RO
		uint32 deliaTcrLenReady : 1; //TCR length ready indication to Delia, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
		uint32 ntdRecipeType : 5; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
		uint32 deliaFrameReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 deliaTbFrameReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 baAgrRdSmcReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 staDbSmcReq : 1; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespStatDebug2_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TF_CFG 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespQosNullLen : 7; //Length of qos-null frame without delimiter. This is the minimal limit check for TZ in case of TB TX., reset value: 0x22, access type: RW
		uint32 reserved0 : 9;
		uint32 swNegateHePuncturingNav : 1; //negate all he_puncturing_nav bits coming from TF decoder module. Only set this field in case there is a bug in polarity between TF decoder and Auto resp, reset value: 0x0, access type: RW
		uint32 swNegateCcaBandsFree : 1; //negate all cca_bands_free bits coming from PHY. Only set this field in case there is a bug in polarity between PHY and Auto resp, reset value: 0x0, access type: RW
		uint32 swIgnoreTfCsRequired : 1; //Ignore CS required check by the Auto resp module, reset value: 0x0, access type: RW
		uint32 reserved1 : 13;
	} bitFields;
} RegB0AutoRespAutoRespTfCfg_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_TF_DEBUG 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tbVapIndex : 5; //VAP ID captured on last TB ackable frame, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 tbDecisionType : 2; //TB decision made according to latest recieved TF , reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 ackableTf : 1; //Ackable TF detected, reset value: 0x0, access type: RO
		uint32 tfLengthCheckValid : 1; //static indication after TF reception if the TF length check was valid or not, reset value: 0x0, access type: RO
		uint32 reserved2 : 14;
	} bitFields;
} RegB0AutoRespAutoRespTfDebug_u;

/*REG_B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG2 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoReplyPuncturingMap : 16; //Puncturing map to TX, reset value: 0x0, access type: RO
		uint32 autoReplySubBand : 4; //Sub band to TX, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0AutoRespAutoReplyOverrideParamsDbg2_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_BASE_ADDR 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoBaseAddr : 22; //AUTO RESP MUBAR HW-SW FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoBaseAddr_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEPTH_MINUS_ONE 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoDepthMinusOne : 8; //AUTO RESP MUBAR HW-SW FIFO depth minus one, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoDepthMinusOne_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoClearStrb : 1; //Clear AUTO RESP MUBAR HW-SW FIFO. , By write '1'., reset value: 0x0, access type: WO
		uint32 autoRespMubarHwSwFifoClearFullDropCtrStrb : 1; //Clear AUTO RESP MUBAR HW-SW FIFO full drop counter. , By write '1'., reset value: 0x0, access type: WO
		uint32 autoRespMubarHwSwFifoClearDecLessThanZeroStrb : 1; //Clear AUTO RESP MUBAR HW-SW FIFO decrement amount less than zero indication. , By write '1'.  , , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoClearStrb_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_RD_ENTRIES_NUM 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoRdEntriesNum : 9; //AUTO RESP MUBAR HW-SW FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoRdEntriesNum_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_NUM_ENTRIES_COUNT 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoNumEntriesCount : 9; //AUTO RESP MUBAR HW-SW FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoNumEntriesCount_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoWrPtr : 8; //AUTO RESP MUBAR HW-SW FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 autoRespMubarHwSwFifoNotEmpty : 1; //AUTO RESP MUBAR HW-SW FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 autoRespMubarHwSwFifoFull : 1; //AUTO RESP MUBAR HW-SW FIFO full indication, reset value: 0x0, access type: RO
		uint32 autoRespMubarHwSwFifoDecrementLessThanZero : 1; //AUTO RESP MUBAR HW-SW FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 autoRespMubarHwSwFifoFullDropCtr : 8; //AUTO RESP MUBAR HW-SW FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoDebug_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_EN 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespMubarHwSwFifoEn : 1; //AUTO RESP MUBAR HW-SW FIFO enabled., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0AutoRespAutoRespMubarHwSwFifoEn_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX0 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespVapDurIdx0 : 32; //VAPs0-15 index in dur memory. 2 bits per vap, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoRespVapDurIdx0_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX1 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoRespVapDurIdx1 : 32; //VAPs16-31 index in dur memory. 2 bits per vap, reset value: 0x0, access type: RW
	} bitFields;
} RegB0AutoRespAutoRespVapDurIdx1_u;

/*REG_B0_AUTO_RESP_AUTO_RESP_AID_FOR_MLD 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 myAidForMld : 12; //MY AID on the VAP that W700 is a STA, reset value: 0x65, access type: RW
		uint32 reserved0 : 4;
		uint32 useMyAidForMld : 1; //1- use my_aid_for_mld in case of a station , 0 - use W600 logic to calculate my AID in the VAP in case of a station, reset value: 0x0, access type: RW
		uint32 nonStaModeOldBehave : 1; //In non STA mode: , 0 - take AID from RXC interface (unlike W600 versions) , 1 - calculate AID from STA ID (like W600 versions), reset value: 0x0, access type: RW
		uint32 reserved1 : 14;
	} bitFields;
} RegB0AutoRespAutoRespAidForMld_u;

/*REG_B0_AUTO_RESP_STD_DB_GENERAL 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 unicastBroadcastN : 1; //unicast/broadcast format - sets transmission format in HE MU to be Unicast(=1) or Broadcast (=0 default), reset value: 0x0, access type: RW
		uint32 primaryTransmittedVapid : 5; //Primary transmitted BSSID VAP ID, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0AutoRespStdDbGeneral_u;

//========================================
/* REG_AUTO_RESP_AUTO_RESP_OUTOF_IDLE_EN 0x0 */
#define B0_AUTO_RESP_AUTO_RESP_OUTOF_IDLE_EN_FIELD_OUT_OF_IDLE_EN_MASK                                            0x00000001
//========================================
/* REG_AUTO_RESP_AUTO_RESP_SW_CLR 0x4 */
#define B0_AUTO_RESP_AUTO_RESP_SW_CLR_FIELD_RXC_FIFO_CLR_REQ_MASK                                                 0x00000001
#define B0_AUTO_RESP_AUTO_RESP_SW_CLR_FIELD_BAG_UPDT_FIFO_CLR_REQ_MASK                                            0x00000002
#define B0_AUTO_RESP_AUTO_RESP_SW_CLR_FIELD_USER_DB_CLR_PULSE_MASK                                                0x00000004
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MISC_CONFIG 0x8 */
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_DYNAMIC_BITMAP_DISABLE_MASK                                      0x00000001
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_BAR_NO_BA_AGR_ACK_POLICY_MASK                                    0x00000002
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_BAR_NUM_BITS_CFG_MASK                                            0x00000004
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_TRAFFIC_BA_AGREEMENT_RECIPIENT_EN_MASK                           0x00000008
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_SW_USR_DB_CLR_MASK                                               0x00000010
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_ACCEPT_FIRST_SN_OUT_OF_BOUNDS_IN_KEEP_HIST_MASK                  0x00000020
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_HE_SU_USE_MBA_INSTD_ACK_MASK                                     0x00000040
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_HE_MU_USE_MBA_INSTD_ACK_MASK                                     0x00000080
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_SUPPORT_MULTI_TID_BAR_MASK                                       0x00000100
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_SUPPORT_TB_MU_BAR_MASK                                           0x00000200
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_PS_ALSO_ON_NON_MU_STA_MASK                                       0x00000400
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_ALLOW_EHT_MCS14_RESP_MASK                                        0x00000800
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_ALLOW_EHT_MCS15_RESP_MASK                                        0x00001000
#define B0_AUTO_RESP_AUTO_RESP_MISC_CONFIG_FIELD_SW_ALLOW_EHT_LDPC_MASK                                           0x00002000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_LOGGER 0xC */
#define B0_AUTO_RESP_AUTO_RESP_LOGGER_FIELD_AUTO_RESP_LOGGER_EN_MASK                                              0x00000001
#define B0_AUTO_RESP_AUTO_RESP_LOGGER_FIELD_AUTO_RESP_LOGGER_PRIO_MASK                                            0x00000006
#define B0_AUTO_RESP_AUTO_RESP_LOGGER_FIELD_LOGGER_ALL_CFG_MASK                                                   0x00000008
#define B0_AUTO_RESP_AUTO_RESP_LOGGER_FIELD_LOGGER_COUNTER_LIMIT_CFG_MASK                                         0x00000FF0
#define B0_AUTO_RESP_AUTO_RESP_LOGGER_FIELD_LOGGER_ACTIVE_MASK                                                    0x00001000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT 0x10 */
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_MAIN_FIFO_BYTE_COUNT_MASK                                  0x00000007
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_MAIN_FIFO_FULL_MASK                                        0x00000008
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_MAIN_FIFO_EMPTY_MASK                                       0x00000010
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_MAIN_FIFO_PUSH_IGNORED_MASK                                0x00000020
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_MAIN_FIFO_POP_IGNORED_MASK                                 0x00000040
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_BUF_FIFO_BYTE_COUNT_MASK                                   0x00030000
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_BUF_FIFO_FULL_MASK                                         0x00040000
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_BUF_FIFO_EMPTY_MASK                                        0x00080000
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_BUF_FIFO_PUSH_IGNORED_MASK                                 0x00100000
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_STAT_FIELD_RXC_BUF_FIFO_POP_IGNORED_MASK                                  0x00200000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR 0x14 */
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR_FIELD_RXC_FIFO_FULL_CNTR_MASK                                   0x000000FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR_CLR 0x18 */
#define B0_AUTO_RESP_AUTO_RESP_RXC_FIFO_FULL_CNTR_CLR_FIELD_RXC_FIFO_FULL_CNTR_CLR_REQ_MASK                       0x00000001
//========================================
/* REG_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG 0x1C */
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_AUTO_RESP_MAIN_FSM_MASK                                        0x0000001F
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_BA_AGR_RD_FSM_MASK                                             0x000000E0
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_USR_STA_RD_FSM_MASK                                            0x00000300
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_TID_BMAP_FSM_MASK                                              0x0000F000
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_ARESP_MTID_SM_MASK                                             0x00030000
#define B0_AUTO_RESP_AUTO_RESP_SM_STAT_DEBUG_FIELD_ARESP_MU_BAR_SM_MASK                                           0x00300000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_RATE_ADPTV_EN 0x24 */
#define B0_AUTO_RESP_AUTO_RESP_RATE_ADPTV_EN_FIELD_RATE_ADPTV_RPT_EN_MASK                                         0x00000001
//========================================
/* REG_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN 0x28 */
#define B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN_FIELD_CBA_BITMAP_LEN_64_MASK                                        0x0000000F
#define B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN_FIELD_CBA_BITMAP_LEN_128_MASK                                       0x000000F0
#define B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN_FIELD_CBA_BITMAP_LEN_256_MASK                                       0x00000F00
#define B0_AUTO_RESP_AUTO_RESP_CBA_BITMAP_LEN_FIELD_CBA_BITMAP_LEN_32_MASK                                        0x0000F000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN 0x2C */
#define B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN_FIELD_MBA_BITMAP_LEN_64_MASK                                        0x0000000F
#define B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN_FIELD_MBA_BITMAP_LEN_128_MASK                                       0x000000F0
#define B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN_FIELD_MBA_BITMAP_LEN_256_MASK                                       0x00000F00
#define B0_AUTO_RESP_AUTO_RESP_MBA_BITMAP_LEN_FIELD_MBA_BITMAP_LEN_32_MASK                                        0x0000F000
//========================================
/* REG_AUTO_RESP_CONTROL_ACK_CONFIG_TABLE 0x30 */
#define B0_AUTO_RESP_CONTROL_ACK_CONFIG_TABLE_FIELD_CONTROL_ACK_CONFIG_MASK                                       0x0000FFFF
//========================================
/* REG_AUTO_RESP_CONTROL_BA_CONFIG_TABLE 0x34 */
#define B0_AUTO_RESP_CONTROL_BA_CONFIG_TABLE_FIELD_CONTROL_BA_CONFIG_MASK                                         0x0000FFFF
//========================================
/* REG_AUTO_RESP_MANGMNT_ACK_CONFIG_TABLE 0x38 */
#define B0_AUTO_RESP_MANGMNT_ACK_CONFIG_TABLE_FIELD_MANGMNT_ACK_CONFIG_MASK                                       0x0000FFFF
//========================================
/* REG_AUTO_RESP_MANAGMNT_BA_CONFIG_TABLE 0x3C */
#define B0_AUTO_RESP_MANAGMNT_BA_CONFIG_TABLE_FIELD_MANGMNT_BA_CONFIG_MASK                                        0x0000FFFF
//========================================
/* REG_AUTO_RESP_STA_DB_MULI_TID_EN_OFFSET 0x40 */
#define B0_AUTO_RESP_STA_DB_MULI_TID_EN_OFFSET_FIELD_STA_DB_MULTI_TID_EN_4B_OFFSET_MASK                           0x000000FF
//========================================
/* REG_AUTO_RESP_BAG_UPDT_FIFO_STAT 0x44 */
#define B0_AUTO_RESP_BAG_UPDT_FIFO_STAT_FIELD_BAG_UPDT_FIFO_BYTE_COUNT_MASK                                       0x0000000F
#define B0_AUTO_RESP_BAG_UPDT_FIFO_STAT_FIELD_BAG_UPDT_FIFO_FULL_MASK                                             0x00000010
#define B0_AUTO_RESP_BAG_UPDT_FIFO_STAT_FIELD_BAG_UPDT_FIFO_EMPTY_MASK                                            0x00000020
#define B0_AUTO_RESP_BAG_UPDT_FIFO_STAT_FIELD_BAG_UPDT_FIFO_PUSH_IGNORED_MASK                                     0x00000040
#define B0_AUTO_RESP_BAG_UPDT_FIFO_STAT_FIELD_BAG_UPDT_FIFO_POP_IGNORED_MASK                                      0x00000080
//========================================
/* REG_AUTO_RESP_AUTO_RESP_OT_CFG 0x60 */
#define B0_AUTO_RESP_AUTO_RESP_OT_CFG_FIELD_ENABLE_INTERNAL_BA_AGR_IND_MASK                                       0x00000001
#define B0_AUTO_RESP_AUTO_RESP_OT_CFG_FIELD_SW_USE_PHY_MODE_NON_TF_FOR_DUR_MASK                                   0x00000002
//========================================
/* REG_AUTO_RESP_AUTO_RESP_CTR 0x70 */
#define B0_AUTO_RESP_AUTO_RESP_CTR_FIELD_AUTO_RESP_TX_CTR_MASK                                                    0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_CTR_FIELD_AUTO_RESP_RX_NAV_ABORT_CTR_MASK                                          0x0000FF00
#define B0_AUTO_RESP_AUTO_RESP_CTR_FIELD_AUTO_RESP_TX_NAV_ABORT_CTR_MASK                                          0x00FF0000
#define B0_AUTO_RESP_AUTO_RESP_CTR_FIELD_AUTO_RESP_STATIC_BW_ABORT_CTR_MASK                                       0xFF000000
//========================================
/* REG_AUTO_RESP_MPDU_DESC_DLM_IDX 0x74 */
#define B0_AUTO_RESP_MPDU_DESC_DLM_IDX_FIELD_MPDU_DESC_FREE_LIST_IDX_MASK                                         0x000000FF
#define B0_AUTO_RESP_MPDU_DESC_DLM_IDX_FIELD_PER_USER_TX_BASE_DLM_LIST_IDX_MASK                                   0x0000FF00
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_BW_TABLE 0x78 */
#define B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_FIELD_AUTO_REPLY_BW_TABLE_MASK                                           0x00007FFF
#define B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_FIELD_AUTO_REPLY_DISALLOWED_CHANNEL_BHV_EHT_MASK                         0x03000000
#define B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_FIELD_AUTO_REPLY_DISALLOWED_CHANNEL_BHV_NON_EHT_MASK                     0x0C000000
#define B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_FIELD_AUTO_REPLY_DISALLOWED_CHANNEL_BHV_EHT_LOW_MCS_MASK                 0x30000000
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_MCS_DEGRADATION_VAL 0x7C */
#define B0_AUTO_RESP_AUTO_REPLY_MCS_DEGRADATION_VAL_FIELD_AUTO_REPLY_MCS_DEGRADATION_VAL_MASK                     0x0000000F
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP0TO9 0x80 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP0TO9_FIELD_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP0TO9_MASK     0x3FFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP10TO19 0x84 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP10TO19_FIELD_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP10TO19_MASK 0x3FFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP20TO29 0x8C */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP20TO29_FIELD_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP20TO29_MASK 0x3FFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP30TO31 0x90 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP30TO31_FIELD_AUTO_REPLY_PWR_DEGRADATION_VAL_VAP30TO31_MASK 0x0000003F
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA0TO31 0x94 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA0TO31_FIELD_AUTO_REPLY_PWR_NEAR_STA0TO31_MASK                         0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA32TO63 0x98 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA32TO63_FIELD_AUTO_REPLY_PWR_NEAR_STA32TO63_MASK                       0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA64TO95 0x9C */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA64TO95_FIELD_AUTO_REPLY_PWR_NEAR_STA64TO95_MASK                       0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA96TO127 0x100 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA96TO127_FIELD_AUTO_REPLY_PWR_NEAR_STA96TO127_MASK                     0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA128TO159 0x104 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA128TO159_FIELD_AUTO_REPLY_PWR_NEAR_STA128TO159_MASK                   0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA160TO191 0x108 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA160TO191_FIELD_AUTO_REPLY_PWR_NEAR_STA160TO191_MASK                   0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA192TO223 0x10C */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA192TO223_FIELD_AUTO_REPLY_PWR_NEAR_STA192TO223_MASK                   0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA224TO255 0x110 */
#define B0_AUTO_RESP_AUTO_REPLY_PWR_NEAR_STA224TO255_FIELD_AUTO_REPLY_PWR_NEAR_STA224TO255_MASK                   0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1 0x114 */
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_PHY_MODE_MASK                               0x0000000F
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_CBW_MASK                                    0x00000070
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_MCS_MASK                                    0x00007F80
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_TX_POWER_MASK                               0x007F8000
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_RU_SIZE_MASK                                0x07800000
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_LDPC_MASK                                   0x08000000
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG1_FIELD_AUTO_REPLY_MRU_MASK                                    0xF0000000
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_BW_TABLE_MAX_CFG 0x118 */
#define B0_AUTO_RESP_AUTO_REPLY_BW_TABLE_MAX_CFG_FIELD_AUTO_REPLY_BW_TABLE_MAX_CFG_MASK                           0x00000007
//========================================
/* REG_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG 0x11C */
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_1ST_STAGE_SM_OUT_MASK                               0x00000007
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_2ND_STAGE_SM_OUT_MASK                               0x00000078
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_FRAME_GEN_CLK_EN_MASK                               0x00000080
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_SELECTED_USER_MASK                                      0x0000FF00
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_SELECTED_TID_MASK                                       0x000F0000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_SELECTED_ACK_TYPE_MASK                                  0x00700000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_DB_WR_EN_MASK                                           0x00800000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_DB_DATA_OUT_AVAILABLE_MASK                              0x01000000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_READ_RA_TA_COUNT_MASK                                   0x06000000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_FRAME_GEN_TO_RXC_SMC_SMC_REQ_MASK                   0x08000000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_FRAME_GEN_TO_UP_MPDU_DLM_VALID_MASK                 0x10000000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_FRAME_GEN_TO_MPDU_DESC_RAM_SMC_SMC_REQ_MASK         0x20000000
#define B0_AUTO_RESP_AUTO_RESP_FRAME_GEN_STAT_DEBUG_FIELD_BAG_FRAME_GEN_TO_TX_CIRC_BUF_SMC_SMC_REQ_MASK           0x40000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCR_BASE_ADDR 0x120 */
#define B0_AUTO_RESP_AUTO_RESP_TCR_BASE_ADDR_FIELD_AUTO_RESP_TCR_BASE_ADDR_MASK                                   0x03FFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT 0x124 */
#define B0_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT_FIELD_NULL_MPDU_CNT_MASK                                        0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT_FIELD_NULL_MPDU_CNT_OVFLW_MASK                                  0x00000100
#define B0_AUTO_RESP_AUTO_RESP_MPDU_NULL_PTR_STAT_FIELD_NULL_MPDU_CNT_CLR_MASK                                    0x00010000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TID_VAL_FOR_ACK_FRAME 0x128 */
#define B0_AUTO_RESP_AUTO_RESP_TID_VAL_FOR_ACK_FRAME_FIELD_MPDU_TID_FIELD_VAL_MASK                                0x0000000F
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_BASE_ADDR 0x12C */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_BASE_ADDR_FIELD_AUTO_RESP_ERR_FIFO_BASE_ADDR_MASK                         0x003FFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEPTH_MINUS_ONE 0x130 */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEPTH_MINUS_ONE_FIELD_AUTO_RESP_ERR_FIFO_DEPTH_MINUS_ONE_MASK             0x000000FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB 0x134 */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_ERR_FIFO_CLEAR_STRB_MASK                       0x00000001
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_ERR_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK         0x00000002
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_ERR_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK    0x00000004
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_RD_ENTRIES_NUM 0x138 */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_RD_ENTRIES_NUM_FIELD_AUTO_RESP_ERR_FIFO_RD_ENTRIES_NUM_MASK               0x000001FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_NUM_ENTRIES_COUNT 0x13C */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_NUM_ENTRIES_COUNT_FIELD_AUTO_RESP_ERR_FIFO_NUM_ENTRIES_COUNT_MASK         0x000001FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG 0x140 */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG_FIELD_AUTO_RESP_ERR_FIFO_WR_PTR_MASK                                0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG_FIELD_AUTO_RESP_ERR_FIFO_NOT_EMPTY_MASK                             0x00010000
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG_FIELD_AUTO_RESP_ERR_FIFO_FULL_MASK                                  0x00020000
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG_FIELD_AUTO_RESP_ERR_FIFO_DECREMENT_LESS_THAN_ZERO_MASK              0x00040000
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_DEBUG_FIELD_AUTO_RESP_ERR_FIFO_FULL_DROP_CTR_MASK                         0xFF000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN 0x144 */
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN_FIELD_AUTO_RESP_ERR_FIFO_MLTI_TID_EN_MASK                              0x00000001
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN_FIELD_AUTO_RESP_ERR_FIFO_BAR_ERR_EN_MASK                               0x00000002
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN_FIELD_AUTO_RESP_ERR_FIFO_SN_OUT_BOUND_EN_MASK                          0x00000004
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN_FIELD_AUTO_RESP_ERR_FIFO_MULTI_TID_BAR_LENGTH_EN_MASK                  0x00000008
#define B0_AUTO_RESP_AUTO_RESP_ERR_FIFO_EN_FIELD_AUTO_RESP_ERR_FIFO_IMM_TO_NON_IMM_EN_MASK                        0x00000010
//========================================
/* REG_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG 0x150 */
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_RU_SIZE_RESP_FOR_HE_SU_MASK                         0x00000001
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_RU_SIZE_RESP_FOR_HE_SU_ER_RU242_MASK                0x00000002
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_RU_SIZE_RESP_FOR_HE_SU_ER_RU106_MASK                0x00000004
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_DCM_RESP_FOR_HE_SU_OR_HE_SU_ER_DCM1_MASK            0x00000008
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_DCM_RESP_FOR_HE_SU_DCM0_MASK                        0x00000010
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_DCM_RESP_FOR_HE_SU_ER_DCM0_MASK                     0x00000020
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_UNKNOWN_STA_RESP_TYPE_MASK                             0x00000040
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_DCM_RESP_FOR_UNKNOWN_STA_DCM0_MASK                  0x00000080
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_HE_SU_ER_TX_DCM_RESP_FOR_UNKNOWN_STA_DCM1_MASK                  0x00000100
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_CFG_FIELD_READ_OMI_HE_ER_DISABLE_BIT_FROM_SELECTOR_MASK                   0x00000200
//========================================
/* REG_AUTO_RESP_AUTO_RESP_HE_SU_ER_BW_RU_TABLE 0x158 */
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_BW_RU_TABLE_FIELD_HE_SU_ER_RU_SIZE_4BW_10_MASK                            0x00000007
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_ER_BW_RU_TABLE_FIELD_HE_SU_ER_RU_SIZE_4BW_20_MASK                            0x00000038
//========================================
/* REG_AUTO_RESP_AUTO_RESP_SPARE_REG 0x15C */
#define B0_AUTO_RESP_AUTO_RESP_SPARE_REG_FIELD_AUTO_RESP_SPARE_REG_MASK                                           0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_HE_DURATION_UPDATE_MODE 0x160 */
#define B0_AUTO_RESP_AUTO_RESP_HE_DURATION_UPDATE_MODE_FIELD_AUTO_RESP_HE_DURATION_UPDATE_MODE_MASK               0x00000001
//========================================
/* REG_AUTO_RESP_AUTO_RESP_HE_MU_TCR_BASE_ADDR 0x164 */
#define B0_AUTO_RESP_AUTO_RESP_HE_MU_TCR_BASE_ADDR_FIELD_AUTO_RESP_HE_MU_TCR_BASE_ADDR_MASK                       0x03FFFFFF
//========================================
/* REG_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS0 0x168 */
#define B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS0_FIELD_USER31_0_WAS_ACTIVE_MASK                                    0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS1 0x16C */
#define B0_AUTO_RESP_USER_TX_WAS_ACTIVE_STATUS1_FIELD_USER35_32_WAS_ACTIVE_MASK                                   0x0000000F
//========================================
/* REG_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0 0x170 */
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0_FIELD_PHY_ERR_NO_RESP_DEBUG_CNTR_MASK                          0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0_FIELD_SEQ_ABORT_NO_RESP_DEBUG_CNTR_MASK                        0x0000FF00
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0_FIELD_VALID_STA_NO_RESP_DEBUG_CNTR_MASK                        0x00FF0000
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR0_FIELD_UNICAST_BIT_NO_RESP_DEBUG_CNTR_MASK                      0xFF000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1 0x174 */
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_NTD_ABORT_NO_RESP_DEBUG_CNTR_MASK                        0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_BAG_PUSH_NO_RESP_DEBUG_CNTR_MASK                         0x0000FF00
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_NO_RESP_STATUS_MASK                                      0x003F0000
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_RXC_PHY_ERR_MASK                                         0x01000000
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_RX_NAV_ZERO_MASK                                         0x02000000
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR1_FIELD_TX_NAV_ZERO_MASK                                         0x04000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR 0x178 */
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_PHY_ERR_DEBUG_CNTR_CLR_MASK                           0x00000001
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_SEQ_ABORT_DEBUG_CNTR_CLR_MASK                         0x00000002
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_VALID_STA_DEBUG_CNTR_CLR_MASK                         0x00000004
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_UNICAST_BIT_DEBUG_CNTR_CLR_MASK                       0x00000008
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_NTD_ABORT_DEBUG_CNTR_CLR_MASK                         0x00000010
#define B0_AUTO_RESP_AUTO_RESP_NO_RESP_DEBUG_CNTR_CLR_FIELD_BAG_PUSH_DEBUG_CNTR_CLR_MASK                          0x00000020
//========================================
/* REG_AUTO_RESP_AUTO_RESP_HE_SU_TID_AGG_LIMIT 0x17C */
#define B0_AUTO_RESP_AUTO_RESP_HE_SU_TID_AGG_LIMIT_FIELD_HE_SU_TID_AGG_LIMIT_MASK                                 0x00000007
//========================================
/* REG_AUTO_RESP_AUTO_RESP_CTR_CONT 0x180 */
#define B0_AUTO_RESP_AUTO_RESP_CTR_CONT_FIELD_AUTO_RESP_BF_ABORT_CTR_MASK                                         0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_CTR_CONT_FIELD_AUTO_RESP_TF_CCA_ABORT_CTR_MASK                                     0x0000FF00
#define B0_AUTO_RESP_AUTO_RESP_CTR_CONT_FIELD_AUTO_RESP_TXH_FREEZE_ABORT_CTR_MASK                                 0x00FF0000
//========================================
/* REG_AUTO_RESP_INT_ERR_STATUS 0x184 */
#define B0_AUTO_RESP_INT_ERR_STATUS_FIELD_AUTO_RESP_ERR_FIFO_FULL_DROP_IRQ_MASK                                   0x00000001
#define B0_AUTO_RESP_INT_ERR_STATUS_FIELD_AUTO_RESP_ERR_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_MASK                    0x00000002
#define B0_AUTO_RESP_INT_ERR_STATUS_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_FULL_DROP_IRQ_MASK                           0x00000004
#define B0_AUTO_RESP_INT_ERR_STATUS_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_AGAIN_MASK      0x00000008
//========================================
/* REG_AUTO_RESP_INT_ERR_EN 0x188 */
#define B0_AUTO_RESP_INT_ERR_EN_FIELD_EN_AUTO_RESP_ERR_FIFO_FULL_DROP_IRQ_MASK                                    0x00000001
#define B0_AUTO_RESP_INT_ERR_EN_FIELD_EN_AUTO_RESP_ERR_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_MASK                     0x00000002
#define B0_AUTO_RESP_INT_ERR_EN_FIELD_EN_AUTO_RESP_MUBAR_HW_SW_FIFO_FULL_DROP_IRQ_MASK                            0x00000004
#define B0_AUTO_RESP_INT_ERR_EN_FIELD_EN_AUTO_RESP_MUBAR_HW_SW_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_MASK             0x00000008
//========================================
/* REG_AUTO_RESP_CLEAR_HISTORY_TID_CTRL 0x190 */
#define B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL_FIELD_CLEAR_HISTORY_PER_TID_EN_MASK                                   0x00000001
#define B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL_FIELD_KEEP_HISTORY_TO_TID_CNT_IN_PER_USER_MASK                        0x00000002
#define B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL_FIELD_KEEP_HISTORY_ENABLED_MASK                                       0x00000004
#define B0_AUTO_RESP_CLEAR_HISTORY_TID_CTRL_FIELD_KEEP_HISTORY_ONLY_BAR_MASK                                      0x00000008
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS0 0x200 */
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS0_FIELD_AUTO_RESP0_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS1 0x204 */
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS1_FIELD_AUTO_RESP1_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS2 0x208 */
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS2_FIELD_AUTO_RESP2_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS3 0x20C */
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS3_FIELD_AUTO_RESP3_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS4 0x210 */
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS4_FIELD_AUTO_RESP4_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS5 0x214 */
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS5_FIELD_AUTO_RESP5_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS6 0x218 */
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS6_FIELD_AUTO_RESP6_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS7 0x21C */
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS7_FIELD_AUTO_RESP7_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS8 0x220 */
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS8_FIELD_AUTO_RESP8_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS9 0x224 */
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_RECIPE_PTR_MASK                                              0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_RX_NAV_ABORT_MASK                                            0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_TX_NAV_ABORT_MASK                                            0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_STATIC_BW_ABORT_MASK                                         0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_BF_ABORT_MASK                                                0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS9_FIELD_AUTO_RESP9_TXH_FREEZE_ABORT_MASK                                        0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS10 0x228 */
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS10_FIELD_AUTO_RESP10_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS11 0x22C */
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS11_FIELD_AUTO_RESP11_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS12 0x230 */
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS12_FIELD_AUTO_RESP12_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS13 0x234 */
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS13_FIELD_AUTO_RESP13_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS14 0x238 */
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS14_FIELD_AUTO_RESP14_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TCS15 0x23C */
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_RECIPE_PTR_MASK                                            0x00001FFF
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_RX_NAV_ABORT_MASK                                          0x02000000
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_TX_NAV_ABORT_MASK                                          0x04000000
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_STATIC_BW_ABORT_MASK                                       0x08000000
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_BF_ABORT_MASK                                              0x10000000
#define B0_AUTO_RESP_AUTO_RESP_TCS15_FIELD_AUTO_RESP15_TXH_FREEZE_ABORT_MASK                                      0x20000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_STAT_DEBUG1 0x280 */
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_TCR_LENGTH_NO_FCS_NO_DEL_TO_HOB_MASK                             0x00000FFF
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_AUTO_RESP_TYPE_SMP_MASK                                          0x00007000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_KEEP_HISTORY_IND_MASK                                            0x00080000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_SELECT_RECIPE_TYPE_MASK                                          0x00F00000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_NTD_TRANSMIT_SUCCESS_LVL_MASK                                    0x01000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_SEQUENCER_AUTO_RESP_ABORT_SMP_MASK                               0x02000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_TXH_NTD_AUTO_RESP_TX_ACTIVE_MASK                                 0x04000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_RXC_RX_DONE_LVL_MASK                                             0x08000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG1_FIELD_PHY_MODE_MASK                                                    0xF0000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_STAT_DEBUG2 0x284 */
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_ARESP_USER_TX_WAS_ACTVIE_VALID_MASK                              0x00000001
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_DELIA_TCR_LEN_READY_MASK                                         0x00000002
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_NTD_RECIPE_TYPE_MASK                                             0x000F8000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_DELIA_FRAME_READY_MASK                                           0x10000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_DELIA_TB_FRAME_READY_MASK                                        0x20000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_BA_AGR_RD_SMC_REQ_MASK                                           0x40000000
#define B0_AUTO_RESP_AUTO_RESP_STAT_DEBUG2_FIELD_STA_DB_SMC_REQ_MASK                                              0x80000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TF_CFG 0x290 */
#define B0_AUTO_RESP_AUTO_RESP_TF_CFG_FIELD_AUTO_RESP_QOS_NULL_LEN_MASK                                           0x0000007F
#define B0_AUTO_RESP_AUTO_RESP_TF_CFG_FIELD_SW_NEGATE_HE_PUNCTURING_NAV_MASK                                      0x00010000
#define B0_AUTO_RESP_AUTO_RESP_TF_CFG_FIELD_SW_NEGATE_CCA_BANDS_FREE_MASK                                         0x00020000
#define B0_AUTO_RESP_AUTO_RESP_TF_CFG_FIELD_SW_IGNORE_TF_CS_REQUIRED_MASK                                         0x00040000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_TF_DEBUG 0x29C */
#define B0_AUTO_RESP_AUTO_RESP_TF_DEBUG_FIELD_TB_VAP_INDEX_MASK                                                   0x0000001F
#define B0_AUTO_RESP_AUTO_RESP_TF_DEBUG_FIELD_TB_DECISION_TYPE_MASK                                               0x00000300
#define B0_AUTO_RESP_AUTO_RESP_TF_DEBUG_FIELD_ACKABLE_TF_MASK                                                     0x00010000
#define B0_AUTO_RESP_AUTO_RESP_TF_DEBUG_FIELD_TF_LENGTH_CHECK_VALID_MASK                                          0x00020000
//========================================
/* REG_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG2 0x2A0 */
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG2_FIELD_AUTO_REPLY_PUNCTURING_MAP_MASK                         0x0000FFFF
#define B0_AUTO_RESP_AUTO_REPLY_OVERRIDE_PARAMS_DBG2_FIELD_AUTO_REPLY_SUB_BAND_MASK                               0x000F0000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_BASE_ADDR 0x2B0 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_BASE_ADDR_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_BASE_ADDR_MASK         0x003FFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEPTH_MINUS_ONE 0x2B4 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEPTH_MINUS_ONE_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_DEPTH_MINUS_ONE_MASK 0x000000FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB 0x2B8 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB_MASK       0x00000001
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK 0x00000002
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_STRB_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK 0x00000004
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_RD_ENTRIES_NUM 0x2BC */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_RD_ENTRIES_NUM_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_RD_ENTRIES_NUM_MASK 0x000001FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_NUM_ENTRIES_COUNT 0x2C0 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_NUM_ENTRIES_COUNT_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_NUM_ENTRIES_COUNT_MASK 0x000001FF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG 0x2C4 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_WR_PTR_MASK                0x000000FF
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_NOT_EMPTY_MASK             0x00010000
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_FULL_MASK                  0x00020000
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_DECREMENT_LESS_THAN_ZERO_MASK 0x00040000
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_DEBUG_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_FULL_DROP_CTR_MASK         0xFF000000
//========================================
/* REG_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_EN 0x2C8 */
#define B0_AUTO_RESP_AUTO_RESP_MUBAR_HW_SW_FIFO_EN_FIELD_AUTO_RESP_MUBAR_HW_SW_FIFO_EN_MASK                       0x00000001
//========================================
/* REG_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX0 0x2D0 */
#define B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX0_FIELD_AUTO_RESP_VAP_DUR_IDX0_MASK                                     0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX1 0x2D4 */
#define B0_AUTO_RESP_AUTO_RESP_VAP_DUR_IDX1_FIELD_AUTO_RESP_VAP_DUR_IDX1_MASK                                     0xFFFFFFFF
//========================================
/* REG_AUTO_RESP_AUTO_RESP_AID_FOR_MLD 0x2E0 */
#define B0_AUTO_RESP_AUTO_RESP_AID_FOR_MLD_FIELD_MY_AID_FOR_MLD_MASK                                              0x00000FFF
#define B0_AUTO_RESP_AUTO_RESP_AID_FOR_MLD_FIELD_USE_MY_AID_FOR_MLD_MASK                                          0x00010000
#define B0_AUTO_RESP_AUTO_RESP_AID_FOR_MLD_FIELD_NON_STA_MODE_OLD_BEHAVE_MASK                                     0x00020000
//========================================
/* REG_AUTO_RESP_STD_DB_GENERAL 0x300 */
#define B0_AUTO_RESP_STD_DB_GENERAL_FIELD_UNICAST_BROADCAST_N_MASK                                                0x00000001
#define B0_AUTO_RESP_STD_DB_GENERAL_FIELD_PRIMARY_TRANSMITTED_VAPID_MASK                                          0x0000003E


#endif // _AUTO_RESP_REGS_H_
