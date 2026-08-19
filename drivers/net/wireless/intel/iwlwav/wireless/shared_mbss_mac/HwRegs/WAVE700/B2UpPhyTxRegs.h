
/***********************************************************************************
File:				B2UpPhyTxRegs.h
Module:				b2UpPhyTx
SOC Revision:		latest
Generated at:       2022-05-04 10:58:38
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_UP_PHY_TX_REGS_H_
#define _B2_UP_PHY_TX_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_UP_PHY_TX_BASE_ADDRESS                             MEMORY_MAP_UNIT_20205_BASE_ADDRESS
#define	REG_B2_UP_PHY_TX_TX_SW_RESET                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x0)
#define	REG_B2_UP_PHY_TX_TX_BLOCK_ENABLE                                  (B2_UP_PHY_TX_BASE_ADDRESS + 0x4)
#define	REG_B2_UP_PHY_TX_TX_SW_RESET_GENERATE                             (B2_UP_PHY_TX_BASE_ADDRESS + 0x8)
#define	REG_B2_UP_PHY_TX_TX_SW_RESET_MASK                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0xC)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_04                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x10)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_05                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x14)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_07                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1C)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_08                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x20)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_09                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x24)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_0A                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x28)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_0B                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x2C)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_0C                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x30)
#define	REG_B2_UP_PHY_TX_GCLK_CONTROL                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x3C)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_11                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x44)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_12                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x48)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_19                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x64)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_1A                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x68)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_1B                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x6C)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_1C                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x70)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_1D                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x74)
#define	REG_B2_UP_PHY_TX_PSTART_DELAY_CNTRL                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x78)
#define	REG_B2_UP_PHY_TX_TX_START_PPM_DRIFT                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x7C)
#define	REG_B2_UP_PHY_TX_TX_NCO_FDL_BYPASS_CNTRL                          (B2_UP_PHY_TX_BASE_ADDRESS + 0x80)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_21                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x84)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_29                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xA4)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_32                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xC8)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_33                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xCC)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_34                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xD0)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_35                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xD4)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_36                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xD8)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_37                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xDC)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_3E                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0xF8)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_48                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x120)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_50                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x140)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_58                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x160)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_5A                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x168)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_5B                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x16C)
#define	REG_B2_UP_PHY_TX_TX_BE_REG_61                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x184)
#define	REG_B2_UP_PHY_TX_TX_BE_RAM_RM_REG                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x188)
#define	REG_B2_UP_PHY_TX_TX_GLBL_RAM_RM_REG                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x18C)
#define	REG_B2_UP_PHY_TX_TX2_CONTROL                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x190)
#define	REG_B2_UP_PHY_TX_DEBUG                                            (B2_UP_PHY_TX_BASE_ADDRESS + 0x194)
#define	REG_B2_UP_PHY_TX_BEAM_FORMING0                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x198)
#define	REG_B2_UP_PHY_TX_TX_BE_RAM_RM_REG1                                (B2_UP_PHY_TX_BASE_ADDRESS + 0x19C)
#define	REG_B2_UP_PHY_TX_MEMORY_CONTROL                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x1A0)
#define	REG_B2_UP_PHY_TX_SPARE_REGS_0                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1A4)
#define	REG_B2_UP_PHY_TX_SPARE_REGS_1                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1A8)
#define	REG_B2_UP_PHY_TX_SPARE_REGS_2                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1AC)
#define	REG_B2_UP_PHY_TX_SPARE_REGS_3                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1B0)
#define	REG_B2_UP_PHY_TX_CUSTOM1_SM_REG_0                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x1B4)
#define	REG_B2_UP_PHY_TX_CUSTOM1_SM_REG_1                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x1B8)
#define	REG_B2_UP_PHY_TX_CYCLIC_INTERPOLATION_DPD0                        (B2_UP_PHY_TX_BASE_ADDRESS + 0x1BC)
#define	REG_B2_UP_PHY_TX_VHT_HT_CDD_0                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1C0)
#define	REG_B2_UP_PHY_TX_VHT_HT_CDD_1                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1C4)
#define	REG_B2_UP_PHY_TX_VHT_HT_CDD_2                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1C8)
#define	REG_B2_UP_PHY_TX_MU_PILOT_DIRECT                                  (B2_UP_PHY_TX_BASE_ADDRESS + 0x1CC)
#define	REG_B2_UP_PHY_TX_TX_CONTROL_ERROR_EN_MASK                         (B2_UP_PHY_TX_BASE_ADDRESS + 0x1D0)
#define	REG_B2_UP_PHY_TX_CONST_MAP_0                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x1D4)
#define	REG_B2_UP_PHY_TX_CONST_MAP_1                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x1D8)
#define	REG_B2_UP_PHY_TX_CONST_MAP_2                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x1DC)
#define	REG_B2_UP_PHY_TX_CONST_MAP_3                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x1E0)
#define	REG_B2_UP_PHY_TX_CONST_MAP_L_SIG                                  (B2_UP_PHY_TX_BASE_ADDRESS + 0x1E4)
#define	REG_B2_UP_PHY_TX_HE_STF_SCALE_0                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x1E8)
#define	REG_B2_UP_PHY_TX_HE_STF_SCALE_1                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x1EC)
#define	REG_B2_UP_PHY_TX_HE_STF_SCALE_2                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x1F0)
#define	REG_B2_UP_PHY_TX_HE_STF_SCALE_3                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x1F4)
#define	REG_B2_UP_PHY_TX_TONE_GEN                                         (B2_UP_PHY_TX_BASE_ADDRESS + 0x1F8)
#define	REG_B2_UP_PHY_TX_LEGACY_CDD_0                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x1FC)
#define	REG_B2_UP_PHY_TX_LEGACY_CDD_1                                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x200)
#define	REG_B2_UP_PHY_TX_CUSTOM2_SM_REG_0                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x204)
#define	REG_B2_UP_PHY_TX_CUSTOM2_SM_REG_1                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x208)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_0_1                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x20C)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_2_3                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x210)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_4_5                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x214)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_6_7                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x218)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_8_9                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x21C)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_10_11                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x220)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_12_13                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x224)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_14_15                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x228)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_16_17                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x22C)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_18_19                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x230)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_20_21                     (B2_UP_PHY_TX_BASE_ADDRESS + 0x234)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_22                        (B2_UP_PHY_TX_BASE_ADDRESS + 0x238)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_23                        (B2_UP_PHY_TX_BASE_ADDRESS + 0x23C)
#define	REG_B2_UP_PHY_TX_TXTD_SCALE_11B                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x240)
#define	REG_B2_UP_PHY_TX_TONE_GEN_SCALE                                   (B2_UP_PHY_TX_BASE_ADDRESS + 0x244)
#define	REG_B2_UP_PHY_TX_CDD_PER_RU                                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x248)
#define	REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_0                                (B2_UP_PHY_TX_BASE_ADDRESS + 0x24C)
#define	REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_1                                (B2_UP_PHY_TX_BASE_ADDRESS + 0x250)
#define	REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_2                                (B2_UP_PHY_TX_BASE_ADDRESS + 0x254)
#define	REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_3                                (B2_UP_PHY_TX_BASE_ADDRESS + 0x258)
#define	REG_B2_UP_PHY_TX_TXTD_BE_FILTER_CONTROL                           (B2_UP_PHY_TX_BASE_ADDRESS + 0x25C)
#define	REG_B2_UP_PHY_TX_MU_TRAINING_DONE                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x260)
#define	REG_B2_UP_PHY_TX_TX_ANTENNA_ON                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x268)
#define	REG_B2_UP_PHY_TX_TX_RAMS_LS                                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x26C)
#define	REG_B2_UP_PHY_TX_TX_BF_CB                                         (B2_UP_PHY_TX_BASE_ADDRESS + 0x270)
#define	REG_B2_UP_PHY_TX_EHT_SCRAMBLER_ENABLE                             (B2_UP_PHY_TX_BASE_ADDRESS + 0x274)
#define	REG_B2_UP_PHY_TX_TX_DIGITAL_BW                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x278)
#define	REG_B2_UP_PHY_TX_CTRL_COMB_SC_FORCE                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x27C)
#define	REG_B2_UP_PHY_TX_CTRL_COMB_SC_VALUE                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x280)
#define	REG_B2_UP_PHY_TX_CTRL_SYMB_BYPASS_MODE                            (B2_UP_PHY_TX_BASE_ADDRESS + 0x284)
#define	REG_B2_UP_PHY_TX_CTRL_SYMB_BYPASS                                 (B2_UP_PHY_TX_BASE_ADDRESS + 0x288)
#define	REG_B2_UP_PHY_TX_CTRL_CURRENT_COMB_SC_VALUE                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x28C)
#define	REG_B2_UP_PHY_TX_CTRL_CURRENT_SYMB_SC_VALUE                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x290)
#define	REG_B2_UP_PHY_TX_SPARE_FROM_FD                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x294)
#define	REG_B2_UP_PHY_TX_SPARE_FROM_TD                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x298)
#define	REG_B2_UP_PHY_TX_SPARE_FROM_BE                                    (B2_UP_PHY_TX_BASE_ADDRESS + 0x29C)
#define	REG_B2_UP_PHY_TX_SPARE_TO_FD                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x2A0)
#define	REG_B2_UP_PHY_TX_SPARE_TO_TD                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x2A4)
#define	REG_B2_UP_PHY_TX_SPARE_TO_BE                                      (B2_UP_PHY_TX_BASE_ADDRESS + 0x2A8)
#define	REG_B2_UP_PHY_TX_ENABLE_EHT_FILLERS                               (B2_UP_PHY_TX_BASE_ADDRESS + 0x2AC)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_24                        (B2_UP_PHY_TX_BASE_ADDRESS + 0x2B0)
#define	REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_25                        (B2_UP_PHY_TX_BASE_ADDRESS + 0x2B4)
#define	REG_B2_UP_PHY_TX_EHT_STF_SCALE_4                                  (B2_UP_PHY_TX_BASE_ADDRESS + 0x2B8)
#define	REG_B2_UP_PHY_TX_EHT_STF_SCALE_5                                  (B2_UP_PHY_TX_BASE_ADDRESS + 0x2BC)
#define	REG_B2_UP_PHY_TX_TONE_GEN_INVERT_BIN                              (B2_UP_PHY_TX_BASE_ADDRESS + 0x2C0)
#define	REG_B2_UP_PHY_TX_SCALE_DATA_WITH_PUNCTURING                       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2C4)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_DEFAULT_VAL    (B2_UP_PHY_TX_BASE_ADDRESS + 0x2C8)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_0          (B2_UP_PHY_TX_BASE_ADDRESS + 0x2CC)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_1          (B2_UP_PHY_TX_BASE_ADDRESS + 0x2D0)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_2          (B2_UP_PHY_TX_BASE_ADDRESS + 0x2D4)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_0       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2D8)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_1       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2DC)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_2       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2E0)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_3       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2E4)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_4       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2E8)
#define	REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_5       (B2_UP_PHY_TX_BASE_ADDRESS + 0x2EC)
#define	REG_B2_UP_PHY_TX_TX_TEST_BUS_GCLK_EN                              (B2_UP_PHY_TX_BASE_ADDRESS + 0x2F0)
#define	REG_B2_UP_PHY_TX_CTRL_MU_TD_GAIN_FORCE                            (B2_UP_PHY_TX_BASE_ADDRESS + 0x2F4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_UP_PHY_TX_TX_SW_RESET 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg : 32; //Sub blocks SW Reset ,  , [0] - tx phy mac IF , [1] - air_time_calc , [4] - tx0_long_preamble , [6] - Tx BE , [8] - tx0_tx_mapper , [9] - tx0_bin_ch , [10] - tx1_tx_mapper , [11] - tx1_bin_ch , [12] - tx2_tx_mapper , [13] - tx2_bin_ch , [14] - tx3_tx_mapper , [15] - tx3_bin_ch , [21] - tx_control , [22] - tx_signal , [30:24] - Tx TD resets , , reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxSwReset_u;

/*REG_B2_UP_PHY_TX_TX_BLOCK_ENABLE 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSublockEnable : 32; //Sub blocks Enable ,  , [0] - tx phy mac IF , [1] - air_time_calc , [4] - tx0_long_preamble , [6] - Tx BE , [8] - tx0_tx_mapper , [9] - tx0_bin_ch , [10] - tx1_tx_mapper , [11] - tx1_bin_ch , [12] - tx2_tx_mapper , [13] - tx2_bin_ch , [14] - tx3_tx_mapper , [15] - tx3_bin_ch , [21] - tx_control , [22] - tx_signal , [30:24] - Tx TD  , , reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBlockEnable_u;

/*REG_B2_UP_PHY_TX_TX_SW_RESET_GENERATE 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetGenerate : 32; //SW Reset Generate, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxSwResetGenerate_u;

/*REG_B2_UP_PHY_TX_TX_SW_RESET_MASK 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macSwResetMask : 32; //MAC SW Reset mask, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxSwResetMask_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_04 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 macEmuModeEn : 1; //MAC Emulator Enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 analogLpbkEn : 1; //Analog loop back enable, reset value: 0x0, access type: RW
		uint32 analogLpbkClkEn : 1; //Analog loop back clock enable, reset value: 0x0, access type: RW
		uint32 reserved2 : 25;
	} bitFields;
} RegB2UpPhyTxTxBeReg04_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_05 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength8 : 16; //Symbol Length Set 0, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength9 : 16; //Symbol Length Set 1, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg05_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_07 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx0Enable : 1; //TX0 enable, reset value: 0x0, access type: RW
		uint32 tx1Enable : 1; //TX1 enable, reset value: 0x0, access type: RW
		uint32 tx2Enable : 1; //TX2 enable, reset value: 0x0, access type: RW
		uint32 tx3Enable : 1; //TX3 enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 txIsHalfBand20160 : 1; //TX is in Half Band 20Mhz out of 80Mhz, reset value: 0x0, access type: RW
		uint32 txIsHalfBand40160 : 1; //TX is in Half Band 40Mhz out of 80Mhz, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 r1PilotsMode : 1; //Enable Rank 1 Pilots mode, reset value: 0x0, access type: RW
		uint32 afeTxDebugModeEn : 1; //Enable AFE TX Debug mode, reset value: 0x0, access type: RW
		uint32 afeTxDebugModePStart : 1; //AFE TX Debug mode p_start, reset value: 0x0, access type: WO
		uint32 tx0FeEnable : 1; //tx0 frontend enable , reset value: 0x1, access type: RW
		uint32 tx1FeEnable : 1; //tx1 frontend enable , reset value: 0x1, access type: RW
		uint32 tx2FeEnable : 1; //tx2 frontend enable , reset value: 0x1, access type: RW
		uint32 tx3FeEnable : 1; //tx3 frontend enable , reset value: 0x1, access type: RW
		uint32 txIsHalfBand80160 : 1; //half band 20 40 mode, reset value: 0x1, access type: RW
		uint32 reserved2 : 13;
	} bitFields;
} RegB2UpPhyTxTxBeReg07_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_08 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength0 : 16; //Symbol Length Set 0, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength1 : 16; //Symbol Length Set 1, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg08_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_09 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength2 : 16; //Symbol Length Set 2, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength3 : 16; //Symbol Length Set 3, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg09_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_0A 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength4 : 16; //Symbol Length Set 4, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength5 : 16; //Symbol Length Set 5, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg0A_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_0B 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength6 : 16; //Symbol Length Set 6, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength7 : 16; //Symbol Length Set 7, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg0B_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_0C 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlPstartToSymStrbDelay : 16; //1st Symbol Strobe Delay, reset value: 0x60C, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxTxBeReg0C_u;

/*REG_B2_UP_PHY_TX_GCLK_CONTROL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gclkEnBypass : 32; //Sub blocks Gated clock bypass ,  , [0] - tx phy mac IF , [1] - air_time_calc , [4] - tx0_long_preamble , [6] - Tx BE , [8] - tx0_tx_mapper , [9] - tx0_bin_ch , [10] - tx1_tx_mapper , [11] - tx1_bin_ch , [12] - tx2_tx_mapper , [13] - tx2_bin_ch , [14] - tx3_tx_mapper , [15] - tx3_bin_ch , [21] - tx_control , [22] - tx_signal , [31:24] - Tx TD  , , reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxGclkControl_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_11 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 scrInitSel : 1; //Scrambler init bypass mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
		uint32 txEndlessMode : 1; //tx_endless_mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 18;
	} bitFields;
} RegB2UpPhyTxTxBeReg11_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_12 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swScrInit0 : 7; //Scrambler new mode init user0, reset value: 0x7f, access type: RW
		uint32 reserved0 : 1;
		uint32 swScrEhtInit : 11; //Scrambler init for eht packet, reset value: 0x7ff, access type: RW
		uint32 reserved1 : 13;
	} bitFields;
} RegB2UpPhyTxTxBeReg12_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_19 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength10 : 16; //Symbol Length Set 0, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength11 : 16; //Symbol Length Set 1, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg19_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_1A 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength12 : 16; //Symbol Length Set 0, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength13 : 16; //Symbol Length Set 1, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg1A_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_1B 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength14 : 16; //Symbol Length Set 0, reset value: 0x1, access type: RW
		uint32 txCntrlSymbolLength15 : 16; //Symbol Length Set 1, reset value: 0x1, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg1B_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_1C 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlSymbolLength16 : 16; //no description, reset value: 0x31FF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxTxBeReg1C_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_1D 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 signalConvTypeA : 9; //Signal Convolutional Encoder Polynomial A, reset value: 0x0, access type: RW
		uint32 signalConvTypeB : 9; //Signal Convolutional Encoder Polynomial B, reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB2UpPhyTxTxBeReg1D_u;

/*REG_B2_UP_PHY_TX_PSTART_DELAY_CNTRL 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pstartToSymStrbDelayWithFdlNcoCfg : 16; //1st Symbol Strobe Delay with FDL & NCO active. Adding the FDL block with output buffer, will require a new calculation for the number of clk's: , Tx Clk^' s=1600-!fdl_bypass*2*72= 1456 clks = 0x5B0, reset value: 0x5B0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxPstartDelayCntrl_u;

/*REG_B2_UP_PHY_TX_TX_START_PPM_DRIFT 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txStartPpmDriftCfg : 16; //ppm drift compesation. Need to shorten the time between p_start and air time to compensate on RX drift due to PPM offset. This number is subtructed from tx_cntrl_pstart_to_sym_strb_delay_with_fdl_nco_cfg in case that DFL & NCO are active. p_sart_delay = p, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxTxStartPpmDrift_u;

/*REG_B2_UP_PHY_TX_TX_NCO_FDL_BYPASS_CNTRL 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNcoFdlBypassCfg : 1; //forces NCO & FDL in bypass mode, reset value: 0x0, access type: RW
		uint32 txNcoFdlForceNoBypassCfg : 1; //if tx_nco_fdl_bypass_cfg is low this bit is taken into account: '0' - normal action. FDL & NCO are bypassesd if phy mode in NOT HE TB. '1' - force active. FDL & NCO are forced active even if phy mode is NOT HE TB, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2UpPhyTxTxNcoFdlBypassCntrl_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_21 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txFdControlSm : 4; //ppm drift compesation. Need to shorten the time between p_start and air time to compensate on RX drift due to PPM offset. This number is subtructed from tx_cntrl_pstart_to_sym_strb_delay_with_fdl_nco_cfg in case that DFL & NCO are active. p_sart_delay = pstart_to_sym_strb_delay_with_fdl_nco_cfg -  tx_start_ppm_drift_cfg, reset value: 0x0, access type: RO
		uint32 txBeControlSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 txControlSm : 5; //tx_control_sm, reset value: 0x0, access type: RO
		uint32 txCtrlDataDoneLong : 1; //TX OFDM Control Data Done, reset value: 0x0, access type: RO
		uint32 reserved1 : 17;
	} bitFields;
} RegB2UpPhyTxTxBeReg21_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_29 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swServiceData : 16; //SW Service Data, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxTxBeReg29_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_32 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilot0Map : 10; //Pilot value for zero, reset value: 0x120, access type: RW
		uint32 pilot1Map : 10; //Pilot value for one, reset value: 0x2E0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB2UpPhyTxTxBeReg32_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_33 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 rotateTxBypass : 1; //Rotate TX bypass, reset value: 0x0, access type: RW
		uint32 rotHtSigEn : 1; //Rotate HT Signals, reset value: 0x0, access type: RW
		uint32 longPreCalibrateMode : 1; //Long Preamble Calibration mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 sbRotateSelect : 1; //Band rotate select, reset value: 0x0, access type: RW
		uint32 reserved2 : 10;
		uint32 longPreBpskVal : 5; //Long Preamble BPSK value, reset value: 0x0, access type: RW
		uint32 longCalQpskVal : 5; //Long Preamble Calibration mode QPSK value, reset value: 0x0, access type: RW
		uint32 reserved3 : 6;
	} bitFields;
} RegB2UpPhyTxTxBeReg33_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_34 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilotIniCb0R1Ists1 : 4; //HT pilot init for no CB and Rank1, reset value: 0x8, access type: RW
		uint32 pilotIniCb0R2Ists1 : 4; //HT pilot init for no CB and Rank2 for antenna 0, reset value: 0x9, access type: RW
		uint32 pilotIniCb0R2Ists2 : 4; //HT pilot init for no CB and Rank2 for antenna 1, reset value: 0x3, access type: RW
		uint32 pilotIniCb0R3Ists3 : 4; //HT pilot init for CB and Rank1, reset value: 0xc, access type: RW
		uint32 pilotIniCb0R4Ists4 : 4; //HT pilot init for CB and Rank2 for antenna 0, reset value: 0x4, access type: RW
		uint32 pilotIniCb1R1Ists1 : 6; //HT pilot init for CB and Rank1 for antenna 1, reset value: 0x3, access type: RW
		uint32 pilotIniCb1R2Ists1 : 6; //HT pilot init for CB and Rank2 for antenna 1, reset value: 0x27, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg34_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_35 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilotSqFcb : 18; //HT Pilot sequence for Full CB, reset value: 0x1a845, access type: RW
		uint32 pilotIniCb1R2Ists2 : 6; //HT pilot init for CB and Rank2 for antenna 2, reset value: 0x2, access type: RW
		uint32 pilotIniCb1R3Ists3 : 6; //HT pilot init for CB and Rank3 for antenna 3, reset value: 0xb, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2UpPhyTxTxBeReg35_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_36 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilotSqNcb : 8; //HT Pilot sequence for Non CB and ssb, reset value: 0x0, access type: RW
		uint32 pilotSqDup : 8; //HT Pilot sequence for Non CB and duplicate mode, reset value: 0x0, access type: RW
		uint32 pilotIniCb1R4Ists4 : 6; //HT Pilot sequence for CB and duplicate mode, reset value: 0x11, access type: RW
		uint32 pilotIniCb2 : 8; //HT Pilot sequence for CB80, reset value: 0xc, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2UpPhyTxTxBeReg36_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_37 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilotSqFcb80 : 24; //HT Pilot sequence for Full CB, reset value: 0xe31aa3, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxTxBeReg37_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_3E 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pilotInitValPStart : 7; //Pilot Init value, reset value: 0x3f, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2UpPhyTxTxBeReg3E_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_48 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSsb40 : 1; //TX Filter select USB/LSB 40Mhz, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 txSsb20 : 1; //TX Filter select USB/LSB 20Mhz, reset value: 0x0, access type: RW
		uint32 txLongPreambleRamEn : 1; //long preamble from ram and not rom. For tone generator, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 txSsb80 : 1; //tx_ssb_80, reset value: 0x0, access type: RW
		uint32 txSsb160 : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 is320UpperChannel : 1; //320 mode 2, reset value: 0x0, access type: RW
		uint32 reserved3 : 23;
	} bitFields;
} RegB2UpPhyTxTxBeReg48_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_50 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 pilotIniCb0R3Ists1 : 4; //rank3 ists1 pilots, reset value: 0x9, access type: RW
		uint32 pilotIniCb0R3Ists2 : 4; //rank3 ists2 pilots, reset value: 0xa, access type: RW
		uint32 pilotIniCb0R4Ists1 : 4; //rank4 ists1 pilots, reset value: 0x8, access type: RW
		uint32 pilotIniCb0R4Ists2 : 4; //rank4 ists1 pilots, reset value: 0x1, access type: RW
		uint32 pilotIniCb0R4Ists3 : 4; //rank4 ists1 pilots, reset value: 0x2, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegB2UpPhyTxTxBeReg50_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_58 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 modem11BForceTransmitAnt : 1; //Modem 802.11B Force transmit on ant0,1,2, reset value: 0x0, access type: RW
		uint32 modem11BForceTransmitAntMask : 4; //Modem 802.11B Force transmit antenna mask, reset value: 0xf, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB2UpPhyTxTxBeReg58_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_5A 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugLocalEn00 : 1; //Test Plug 00 Enable, reset value: 0x0, access type: RW
		uint32 testPlugLocalEn01 : 1; //Test Plug 00 Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
		uint32 txBfSm2TbRate : 4; //tx_bf_sm_2_tb_rate, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxBeReg5A_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_5B 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusLowOutSelect : 2; //Test Plug Mux Out Low Selector, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 testBusMidOutSelect : 2; //Test Plug Mux Out Mid Selector, reset value: 0x1, access type: RW
		uint32 reserved1 : 2;
		uint32 testBusHighOutSelect : 2; //Test Plug Mux Out High Selector, reset value: 0x2, access type: RW
		uint32 reserved2 : 2;
		uint32 testBusFourOutSelect : 2; //Test Plug Mux Out Four Selector, reset value: 0x3, access type: RW
		uint32 reserved3 : 2;
		uint32 testBusFiveOutSelect : 3; //Test Plug Mux Out Five Selector, reset value: 0x4, access type: RW
		uint32 reserved4 : 13;
	} bitFields;
} RegB2UpPhyTxTxBeReg5B_u;

/*REG_B2_UP_PHY_TX_TX_BE_REG_61 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 r1PilotsLongNcbTbl : 2; //Map table for R1 pilots Long Pre Non CB, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 r1PilotsLongFcbTbl : 2; //Map table for R1 pilots Long Pre Full CB, reset value: 0x0, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegB2UpPhyTxTxBeReg61_u;

/*REG_B2_UP_PHY_TX_TX_BE_RAM_RM_REG 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpcRamRm : 3; //LDPC PRAM rm reg, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 intlvRam0RmValue : 3; //Interleaver 0 RAM 0 rm reg, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 intlvRam1RmValue : 3; //Interleaver 0 RAM 1 rm reg, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 intlvRam2RmValue : 3; //Interleaver 0 RAM 2 rm reg, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 intlvRam3RmValue : 3; //Interleaver 0 RAM 3 rm reg, reset value: 0x3, access type: RW
		uint32 tx0BinChTpmRm : 3; //tx0_bin_ch_tpm_rm, reset value: 0x3, access type: RW
		uint32 tx0BinChRuIdRm : 3; //tx0_bin_ch_ru_id_rm, reset value: 0x3, access type: RW
		uint32 txtdScaleRamRm : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 4;
	} bitFields;
} RegB2UpPhyTxTxBeRamRmReg_u;

/*REG_B2_UP_PHY_TX_TX_GLBL_RAM_RM_REG 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 longPreambleRamRm : 3; //Long preamble ram rm reg, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 emuExtDataRm : 3; //mac emulator external data ram, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 emuExtControlRm : 3; //mac emulator external control rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 emuExtParityRm : 3; //mac emulator external parity ram, reset value: 0x3, access type: RW
		uint32 reserved3 : 17;
	} bitFields;
} RegB2UpPhyTxTxGlblRamRmReg_u;

/*REG_B2_UP_PHY_TX_TX2_CONTROL 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 flatBypass : 1; //flat bypass in 3ss and 3antennas or 2ss and 2antenna using default matrix, reset value: 0x0, access type: RW
		uint32 csdBypass : 1; //csd bypass, reset value: 0x0, access type: RW
		uint32 antennaSelectionEn : 1; //antenna_selection_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 htShortEqualizer : 12; //ht short energy equalizer, reset value: 0x516, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegB2UpPhyTxTx2Control_u;

/*REG_B2_UP_PHY_TX_DEBUG 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 afeTxStreamerMode : 2; //use streamer for 1/2/3 ants, reset value: 0x0, access type: RW
		uint32 afeTxStreamerDuplicate : 1; //afe_tx_streamer_duplicate, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB2UpPhyTxDebug_u;

/*REG_B2_UP_PHY_TX_BEAM_FORMING0 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 txPowerLoadGainReady : 1; //tx_power_load_gain_ready, reset value: 0x0, access type: RW
		uint32 reserved1 : 20;
		uint32 txSpCalibEn : 1; //calibration enable, reset value: 0x0, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegB2UpPhyTxBeamForming0_u;

/*REG_B2_UP_PHY_TX_TX_BE_RAM_RM_REG1 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qRamRmValue : 3; //Standard LDPC ERAM1 rm reg, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2UpPhyTxTxBeRamRmReg1_u;

/*REG_B2_UP_PHY_TX_MEMORY_CONTROL 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memGlobalRm : 2; //Global RM value for rams, reset value: 0x0, access type: RW
		uint32 memGlobalTestMode : 2; //Global test mode for rams, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2UpPhyTxMemoryControl_u;

/*REG_B2_UP_PHY_TX_SPARE_REGS_0 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr00 : 1; //Spare registers, reset value: 0x0, access type: WO
		uint32 spareGpr01 : 1; //no description, reset value: 0x0, access type: WO
		uint32 spareGpr0231 : 30; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxSpareRegs0_u;

/*REG_B2_UP_PHY_TX_SPARE_REGS_1 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr1 : 32; //Spare registers, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxSpareRegs1_u;

/*REG_B2_UP_PHY_TX_SPARE_REGS_2 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr2 : 32; //Spare registers, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxSpareRegs2_u;

/*REG_B2_UP_PHY_TX_SPARE_REGS_3 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr3 : 32; //Spare registers, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxSpareRegs3_u;

/*REG_B2_UP_PHY_TX_CUSTOM1_SM_REG_0 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cddCustom12SsAnt1 : 8; //custom1 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 cddCustom12SsAnt2 : 8; //custom1 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 cddCustom12SsAnt3 : 8; //custom1 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxCustom1SmReg0_u;

/*REG_B2_UP_PHY_TX_CUSTOM1_SM_REG_1 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cddCustom13SsAnt1 : 8; //custom1 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 cddCustom13SsAnt2 : 8; //custom1 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 cddCustom13SsAnt3 : 8; //custom1 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxCustom1SmReg1_u;

/*REG_B2_UP_PHY_TX_CYCLIC_INTERPOLATION_DPD0 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 calibrationInExplicit : 1; //The rules to operate the Calibration unit are: , not((Explicit or NDP) and calibrationInExplicit==0), reset value: 0x0, access type: RW
		uint32 reserved1 : 30;
	} bitFields;
} RegB2UpPhyTxCyclicInterpolationDpd0_u;

/*REG_B2_UP_PHY_TX_VHT_HT_CDD_0 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cdd2StsAnt1 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd2StsAnt2 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd2StsAnt3 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd3StsAnt1 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxVhtHtCdd0_u;

/*REG_B2_UP_PHY_TX_VHT_HT_CDD_1 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cdd3StsAnt2 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd3StsAnt3 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd4StsAnt1 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 cdd4StsAnt2 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxVhtHtCdd1_u;

/*REG_B2_UP_PHY_TX_VHT_HT_CDD_2 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cdd4StsAnt3 : 8; //cdd for flat mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2UpPhyTxVhtHtCdd2_u;

/*REG_B2_UP_PHY_TX_MU_PILOT_DIRECT 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muPilotDirect2SsEn : 1; //mu_pilot_direct_2ss_en, reset value: 0x0, access type: RW
		uint32 muPilotDirect3SsEn : 1; //mu_pilot_direct_3ss_en, reset value: 0x0, access type: RW
		uint32 muPilotDirect4SsEn : 1; //mu_pilot_direct_4ss_en, reset value: 0x0, access type: RW
		uint32 createMuAsDirectMap : 1; //create_mu_as_direct_map, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2UpPhyTxMuPilotDirect_u;

/*REG_B2_UP_PHY_TX_TX_CONTROL_ERROR_EN_MASK 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txControlErrorEnMask : 3; //[0]-mu packet , [1]-legacy bf , [2]-tx_enable while rx_ready, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2UpPhyTxTxControlErrorEnMask_u;

/*REG_B2_UP_PHY_TX_CONST_MAP_0 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataEquSet0 : 12; //BPSK constellation mapping, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 dataEquSet1 : 12; //QPSK constellation mapping, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB2UpPhyTxConstMap0_u;

/*REG_B2_UP_PHY_TX_CONST_MAP_1 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataEquSet2 : 12; //constellation mapping QAM16, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 dataEquSet3 : 12; //constellation mapping QAM64, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB2UpPhyTxConstMap1_u;

/*REG_B2_UP_PHY_TX_CONST_MAP_2 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataEquSet4 : 12; //constellation mapping QAM256, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 dataEquSet5 : 12; //constellation mapping QAM1024, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB2UpPhyTxConstMap2_u;

/*REG_B2_UP_PHY_TX_CONST_MAP_3 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataEquSet6 : 12; //constellation mapping QAM4096, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB2UpPhyTxConstMap3_u;

/*REG_B2_UP_PHY_TX_CONST_MAP_L_SIG 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lSigBpskVal : 12; //SU/MU BSLK, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 lSigBpskBoostVal : 12; //l_sig_bpsk_boost_val, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB2UpPhyTxConstMapLSig_u;

/*REG_B2_UP_PHY_TX_HE_STF_SCALE_0 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal0 : 11; //he_stf_scale_val0, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal1 : 11; //he_stf_scale_val1, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfScale0_u;

/*REG_B2_UP_PHY_TX_HE_STF_SCALE_1 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal2 : 11; //he_stf_scale_val2, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal3 : 11; //he_stf_scale_val3, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfScale1_u;

/*REG_B2_UP_PHY_TX_HE_STF_SCALE_2 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal4 : 11; //he_stf_scale_val4, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal5 : 11; //he_stf_scale_val5, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfScale2_u;

/*REG_B2_UP_PHY_TX_HE_STF_SCALE_3 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal6 : 11; //he_stf_scale_val6, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal7 : 11; //he_stf_scale_val7, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfScale3_u;

/*REG_B2_UP_PHY_TX_TONE_GEN 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 toneGenStart : 1; //tone_gen_start, reset value: 0x0, access type: RW
		uint32 toneGenOn : 1; //tone_gen_on, reset value: 0x0, access type: RW
		uint32 toneGenLstfMode : 1; //tone_gen_lstf_mode, reset value: 0x0, access type: RW
		uint32 toneGenCycles : 27; //tone_gen_cycles, reset value: 0x0, access type: RW
		uint32 toneGenActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2UpPhyTxToneGen_u;

/*REG_B2_UP_PHY_TX_LEGACY_CDD_0 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyCdd2AntsOfst1 : 8; //phy_cdd_2_ants_ofst_1, reset value: 0x0, access type: RW
		uint32 phyCdd3AntsOfst1 : 8; //phy_cdd_3_ants_ofst_1, reset value: 0x0, access type: RW
		uint32 phyCdd3AntsOfst2 : 8; //phy_cdd_3_ants_ofst_2, reset value: 0x0, access type: RW
		uint32 phyCdd4AntsOfst1 : 8; //phy_cdd_4_ants_ofst_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxLegacyCdd0_u;

/*REG_B2_UP_PHY_TX_LEGACY_CDD_1 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyCdd4AntsOfst2 : 8; //phy_cdd_4_ants_ofst_2, reset value: 0x0, access type: RW
		uint32 phyCdd4AntsOfst3 : 8; //phy_cdd_4_ants_ofst_3, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxLegacyCdd1_u;

/*REG_B2_UP_PHY_TX_CUSTOM2_SM_REG_0 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cddCustom22SsAnt1 : 8; //custom2 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 cddCustom22SsAnt2 : 8; //custom2 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 cddCustom22SsAnt3 : 8; //custom2 cdd register for 2ss , reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxCustom2SmReg0_u;

/*REG_B2_UP_PHY_TX_CUSTOM2_SM_REG_1 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cddCustom23SsAnt1 : 8; //custom2 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 cddCustom23SsAnt2 : 8; //custom2 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 cddCustom23SsAnt3 : 8; //custom2 cdd register for 3ss , reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxCustom2SmReg1_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_0_1 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength0 : 16; //tx_cntrl_td_symbol_delay_length_0, reset value: 0x9ff, access type: RW
		uint32 txCntrlTdSymbolDelayLength1 : 16; //tx_cntrl_td_symbol_delay_length_1, reset value: 0x1dff, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength01_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_2_3 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength2 : 16; //tx_cntrl_td_symbol_delay_length_2, reset value: 0x31FF, access type: RW
		uint32 txCntrlTdSymbolDelayLength3 : 16; //tx_cntrl_td_symbol_delay_length_3, reset value: 0x3BFF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength2_3_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_4_5 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength4 : 16; //tx_cntrl_td_symbol_delay_length_4, reset value: 0x59FF, access type: RW
		uint32 txCntrlTdSymbolDelayLength5 : 16; //tx_cntrl_td_symbol_delay_length_5, reset value: 0x67FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength45_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_6_7 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength6 : 16; //tx_cntrl_td_symbol_delay_length_6, reset value: 0x2BFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength7 : 16; //tx_cntrl_td_symbol_delay_length_7, reset value: 0x3FFF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength67_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_8_9 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength8 : 16; //tx_cntrl_td_symbol_delay_length_8, reset value: 0x67FF, access type: RW
		uint32 txCntrlTdSymbolDelayLength9 : 16; //tx_cntrl_td_symbol_delay_length_9, reset value: 0x3BFF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength89_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_10_11 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength10 : 16; //tx_cntrl_td_symbol_delay_length_10, reset value: 0x5DFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength11 : 16; //tx_cntrl_td_symbol_delay_length_11, reset value: 0xA1FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength1011_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_12_13 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength12 : 16; //tx_cntrl_td_symbol_delay_length_12, reset value: 0x3BFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength13 : 16; //tx_cntrl_td_symbol_delay_length_13, reset value: 0x63FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength1213_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_14_15 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength14 : 16; //tx_cntrl_td_symbol_delay_length_14, reset value: 0xB3FF, access type: RW
		uint32 txCntrlTdSymbolDelayLength15 : 16; //no description, reset value: 0x23FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength1415_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_16_17 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength16 : 16; //tx_cntrl_td_symbol_delay_length_14, reset value: 0x2FFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength17 : 16; //no description, reset value: 0x47FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength1617_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_18_19 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength18 : 16; //tx_cntrl_td_symbol_delay_length_14, reset value: 0x5FFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength19 : 16; //no description, reset value: 0x77FF, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength1819_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_20_21 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength20 : 16; //tx_cntrl_td_symbol_delay_length_14, reset value: 0x7BFF, access type: RW
		uint32 txCntrlTdSymbolDelayLength21 : 16; //no description, reset value: 0x8D7F, access type: RW
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength2021_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_22 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength22 : 17; //tx_cntrl_td_symbol_delay_length_14, reset value: 0x103FF, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength22_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_23 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength23 : 17; //tx_cntrl_td_symbol_delay_length_14, reset value: 0x153FF, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength23_u;

/*REG_B2_UP_PHY_TX_TXTD_SCALE_11B 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdScale11B : 12; //txtd_scale_11b, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB2UpPhyTxTxtdScale11B_u;

/*REG_B2_UP_PHY_TX_TONE_GEN_SCALE 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 toneGenBpskVal : 12; //no description, reset value: 0x400, access type: RW
		uint32 reserved0 : 4;
		uint32 fdScaleShift : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB2UpPhyTxToneGenScale_u;

/*REG_B2_UP_PHY_TX_CDD_PER_RU 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cddPerRuBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxCddPerRu_u;

/*REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_0 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfTbScaleVal0 : 11; //he_stf_scale_val0, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfTbScaleVal1 : 11; //he_stf_scale_val1, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfTbScale0_u;

/*REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_1 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfTbScaleVal2 : 11; //he_stf_scale_val2, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfTbScaleVal3 : 11; //he_stf_scale_val3, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfTbScale1_u;

/*REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_2 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfTbScaleVal4 : 11; //he_stf_scale_val4, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfTbScaleVal5 : 11; //he_stf_scale_val5, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxHeStfTbScale2_u;

/*REG_B2_UP_PHY_TX_HE_STF_TB_SCALE_3 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfTbScaleVal6 : 11; //he_stf_scale_val6, reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB2UpPhyTxHeStfTbScale3_u;

/*REG_B2_UP_PHY_TX_TXTD_BE_FILTER_CONTROL 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdBeFilterEnMask : 24; //no description, reset value: 0x0, access type: RW
		uint32 txStrbEarlyStart : 6; //no description, reset value: 0x0, access type: RW
		uint32 txtdBeFilterForceBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 txtdBeFilterForceEnable : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB2UpPhyTxTxtdBeFilterControl_u;

/*REG_B2_UP_PHY_TX_MU_TRAINING_DONE 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTrainingDone : 1; //done pulse trigger for mu training sequence, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxMuTrainingDone_u;

/*REG_B2_UP_PHY_TX_TX_ANTENNA_ON 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAntennaOn : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB2UpPhyTxTxAntennaOn_u;

/*REG_B2_UP_PHY_TX_TX_RAMS_LS 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intlvRamLs : 1; //LS for TXBE intlv ram, reset value: 0x0, access type: RW
		uint32 qRamLs : 1; //LS for TXBE q ram, reset value: 0x0, access type: RW
		uint32 txfdRamsLs : 1; //no description, reset value: 0x0, access type: RW
		uint32 macEmuLs : 1; //mac emulator rams ls, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2UpPhyTxTxRamsLs_u;

/*REG_B2_UP_PHY_TX_TX_BF_CB 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBfCbSu : 1; //chicken bit for SU case: , when SU packet is BFmed, report is bad and 1ss, in this case stream should be duplicated on all antennas, reset value: 0x1, access type: RW
		uint32 txBfCbOfdma : 1; //chicken bit for OFDMA case: , when OFDMA packet is BFmed, report is bad and 1ss, in this case stream should be duplicated on all antennas, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2UpPhyTxTxBfCb_u;

/*REG_B2_UP_PHY_TX_EHT_SCRAMBLER_ENABLE 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtScramblerEnable : 1; //select between 7bits to 11bits scrambler, '0' - 7bits. '1' - 11bits, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxEhtScramblerEnable_u;

/*REG_B2_UP_PHY_TX_TX_DIGITAL_BW 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDigitalBw : 3; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2UpPhyTxTxDigitalBw_u;

/*REG_B2_UP_PHY_TX_CTRL_COMB_SC_FORCE 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlCombScForce : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxCtrlCombScForce_u;

/*REG_B2_UP_PHY_TX_CTRL_COMB_SC_VALUE 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlCombScValue : 9; //no description, reset value: 0x5b, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2UpPhyTxCtrlCombScValue_u;

/*REG_B2_UP_PHY_TX_CTRL_SYMB_BYPASS_MODE 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlSymbBypassMode : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2UpPhyTxCtrlSymbBypassMode_u;

/*REG_B2_UP_PHY_TX_CTRL_SYMB_BYPASS 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlSymbBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxCtrlSymbBypass_u;

/*REG_B2_UP_PHY_TX_CTRL_CURRENT_COMB_SC_VALUE 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlCurrentCombScValue : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2UpPhyTxCtrlCurrentCombScValue_u;

/*REG_B2_UP_PHY_TX_CTRL_CURRENT_SYMB_SC_VALUE 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlCurrentSymbScValue : 24; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB2UpPhyTxCtrlCurrentSymbScValue_u;

/*REG_B2_UP_PHY_TX_SPARE_FROM_FD 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd : 16; //spare connectivity from FD, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareFromFd_u;

/*REG_B2_UP_PHY_TX_SPARE_FROM_TD 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTd : 16; //spare connectivity from TD, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareFromTd_u;

/*REG_B2_UP_PHY_TX_SPARE_FROM_BE 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromBe : 16; //spare connectivity from BE, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareFromBe_u;

/*REG_B2_UP_PHY_TX_SPARE_TO_FD 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd : 16; //spare connectivity to FD, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareToFd_u;

/*REG_B2_UP_PHY_TX_SPARE_TO_TD 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTd : 16; //spare connectivity to TD, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareToTd_u;

/*REG_B2_UP_PHY_TX_SPARE_TO_BE 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToBe : 16; //spare connectivity to BE, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2UpPhyTxSpareToBe_u;

/*REG_B2_UP_PHY_TX_ENABLE_EHT_FILLERS 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableEhtFillers : 1; //support phy fillers for users>=20M in EHT, reset value: 0x0, access type: RW
		uint32 enableEhtFillersSmallRu : 1; //support phy fillers for users<20M in EHT, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2UpPhyTxEnableEhtFillers_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_24 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength24 : 17; //no description, reset value: 0xA9FF, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength24_u;

/*REG_B2_UP_PHY_TX_TD_SYMBOL_DELAY_LENGTH_25 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCntrlTdSymbolDelayLength25 : 17; //no description, reset value: 0x129FF, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2UpPhyTxTdSymbolDelayLength25_u;

/*REG_B2_UP_PHY_TX_EHT_STF_SCALE_4 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal8 : 11; //no description, reset value: 0x3d4, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal9 : 11; //no description, reset value: 0x36d, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxEhtStfScale4_u;

/*REG_B2_UP_PHY_TX_EHT_STF_SCALE_5 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfScaleVal10 : 11; //no description, reset value: 0x352, access type: RW
		uint32 reserved0 : 5;
		uint32 heStfScaleVal11 : 11; //no description, reset value: 0x386, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB2UpPhyTxEhtStfScale5_u;

/*REG_B2_UP_PHY_TX_TONE_GEN_INVERT_BIN 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 toneGenInvertBin : 11; //no description, reset value: 0x7FF, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB2UpPhyTxToneGenInvertBin_u;

/*REG_B2_UP_PHY_TX_SCALE_DATA_WITH_PUNCTURING 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 scaleDataWithPuncturing : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxScaleDataWithPuncturing_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_DEFAULT_VAL 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320DefaultVal : 3; //no description, reset value: 0x6, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320DefaultVal_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_0 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Mru0 : 30; //no description, reset value: 0x1ff39e66, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Mru0_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_1 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Mru1 : 30; //no description, reset value: 0x108c99a6, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Mru1_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_MRU_2 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Mru2 : 15; //no description, reset value: 0x7c1f, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Mru2_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_0 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru0 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru0_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_1 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru1 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru1_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_2 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru2 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru2_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_3 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru3 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru3_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_4 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru4 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru4_u;

/*REG_B2_UP_PHY_TX_PM_EHT_PREAMBLE_TONE_ROTATION_320_NONMRU_5 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtPreambleToneRotation320Nonmru5 : 19; //no description, reset value: 0x60000, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegB2UpPhyTxPmEhtPreambleToneRotation320Nonmru5_u;

/*REG_B2_UP_PHY_TX_TX_TEST_BUS_GCLK_EN 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTestBusGclkEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2UpPhyTxTxTestBusGclkEn_u;

/*REG_B2_UP_PHY_TX_CTRL_MU_TD_GAIN_FORCE 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlMuTdGainValue : 8; //no description, reset value: 0x0, access type: RW
		uint32 ctrlMuTdGainForce : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2UpPhyTxCtrlMuTdGainForce_u;



#endif // _B2_UP_PHY_TX_REGS_H_
