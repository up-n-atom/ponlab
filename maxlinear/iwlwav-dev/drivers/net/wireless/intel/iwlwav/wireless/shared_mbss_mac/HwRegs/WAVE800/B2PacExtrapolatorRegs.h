
/***********************************************************************************
File:				B2PacExtrapolatorRegs.h
Module:				b2PacExtrapolator
SOC Revision:		latest
Generated at:       2024-06-26 12:54:51
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_PAC_EXTRAPOLATOR_REGS_H_
#define _B2_PAC_EXTRAPOLATOR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_PAC_EXTRAPOLATOR_BASE_ADDRESS                  MEMORY_MAP_UNIT_20062_BASE_ADDRESS
#define	REG_B2_PAC_EXTRAPOLATOR_EXTRAP_CONTROL_CFG                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x0)
#define	REG_B2_PAC_EXTRAPOLATOR_SU_STA_DB_CFG                       (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x4)
#define	REG_B2_PAC_EXTRAPOLATOR_MU_DB_CFG                           (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x8)
#define	REG_B2_PAC_EXTRAPOLATOR_MU_USER_REINIT_VEC                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xC)
#define	REG_B2_PAC_EXTRAPOLATOR_LN_BETA_LUT_BASE_ADDR               (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x10)
#define	REG_B2_PAC_EXTRAPOLATOR_WORD_UPD_EN_VEC                     (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x14)
#define	REG_B2_PAC_EXTRAPOLATOR_WORD_METHOD_VEC                     (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x18)
#define	REG_B2_PAC_EXTRAPOLATOR_TSF_RESOLUTION_CFG                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x1C)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x24)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x28)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x2C)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15                (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x30)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x34)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x38)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x3C)
#define	REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15                (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x40)
#define	REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5                   (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x44)
#define	REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x48)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_0_31                         (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x4C)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_32_63                        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x50)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_64_95                        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x54)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_96_127                       (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x58)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_128_159                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x5C)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_160_191                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x60)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_192_223                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x64)
#define	REG_B2_PAC_EXTRAPOLATOR_STA_EN_224_255                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x68)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_REQ                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x6C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x70)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_USER_VALID_VEC               (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x74)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_REQ             (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x78)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_STATUS                       (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x7C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG             (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x80)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_MIRROR_ADDR                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x84)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_DB_ADDR                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x88)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_API_WORD_UPD_ENA_VEC             (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x8C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_REQ                          (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x90)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_ALFA                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x94)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER            (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x98)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x9C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_TSF                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xA0)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_TSF                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xA4)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_ABS_LN_BETA                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xA8)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_AVG_DELTA_T                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xAC)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xB0)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_ALFA                     (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xB4)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED             (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xB8)
#define	REG_B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL                  (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xBC)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_REG                           (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xC0)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_CURRENT_TSF                   (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xC4)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN                       (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xC8)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_NEW_ALFA                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xCC)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW                      (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xE0)
#define	REG_B2_PAC_EXTRAPOLATOR_DEBUG_LAST_SMC_ADDR                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xE4)
#define	REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_DBG                        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xE8)
#define	REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_DB_BASE                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xEC)
#define	REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG                        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xF0)
#define	REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_IDLE_DUR                   (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xF4)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI                   (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xF8)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP              (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0xFC)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF                     (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x100)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_USERS_DONE             (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x104)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_REINIT_DONE            (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x108)
#define	REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x10C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_TSF                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x120)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_USER_REINIT_VEC              (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x124)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_INPUT_BUFFER                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x128)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_DB_PTR                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x12C)
#define	REG_B2_PAC_EXTRAPOLATOR_MU_DB_ENTRY_SIZE                    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x130)
#define	REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15                 (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x134)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER_5TH        (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x138)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED_5TH    (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x13C)
#define	REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED_5TH         (B2_PAC_EXTRAPOLATOR_BASE_ADDRESS + 0x140)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_PAC_EXTRAPOLATOR_EXTRAP_CONTROL_CFG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extrapEn : 1; //Phy AF Block enable bit, Default is Disabled , When disabled @run-time, the block will finish current process and ignore next HW triggers., reset value: 0x0, access type: RW
		uint32 extrapBusy : 1; //Phy AF Block busy bit, Default is Free, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2PacExtrapolatorExtrapControlCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_SU_STA_DB_CFG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suStaDbBaseAddr : 22; //Alfa factors wlan DB base address at SHRAM, word aligned , reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 suDbEntrySize : 6; //in units of 32b words; max: 63 words., reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacExtrapolatorSuStaDbCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_MU_DB_CFG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muDbBaseAddr : 22; //no description, reset value: 0x0, access type: RW
		uint32 muDbUpdateEna : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
	} bitFields;
} RegB2PacExtrapolatorMuDbCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_MU_USER_REINIT_VEC 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUserReinitVec : 32; //Bit per user. "1" means treat/overwrite old DB value as "0". Active even if the user isn't "valid"., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorMuUserReinitVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_LN_BETA_LUT_BASE_ADDR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lnBetaLutBaseAddr : 22; //Ln Beta LUT base address at SHRAM, word aligned, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorLnBetaLutBaseAddr_u;

/*REG_B2_PAC_EXTRAPOLATOR_WORD_UPD_EN_VEC 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wordUpdEnVec : 16; //bit N is an enable of WordN update, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacExtrapolatorWordUpdEnVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_WORD_METHOD_VEC 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wordMethodVec : 32; //each two bits are method of the corresponding word. [1:0]->word0, [3:2]-> word1, etc., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorWordMethodVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_TSF_RESOLUTION_CFG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suTsfResolution : 4; //TSF resolution to determine time units, reset value: 0x7, access type: RW
		uint32 muTsfResolution : 4; //TSF resolution to determine time units, reset value: 0x7, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacExtrapolatorTsfResolutionCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetSuWord0 : 5; //Word0 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetSuWord1 : 5; //Word1 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetSuWord2 : 5; //Word2 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetSuWord3 : 5; //Word3 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetSuWord03_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetSuWord4 : 5; //Word4 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetSuWord5 : 5; //Word5 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetSuWord6 : 5; //Word6 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetSuWord7 : 5; //Word7 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetSuWord47_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetSuWord8 : 5; //Word8 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetSuWord9 : 5; //Word9 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetSuWord10 : 5; //Word10 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetSuWord11 : 5; //Word11 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetSuWord811_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetSuWord12 : 5; //Word12 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetSuWord13 : 5; //Word13 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetSuWord14 : 5; //Word14 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetSuWord15 : 5; //Word15 offset in the SU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetSuWord1215_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetMuWord0 : 5; //Word0 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetMuWord1 : 5; //Word1 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetMuWord2 : 5; //Word2 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetMuWord3 : 5; //Word3 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetMuWord03_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetMuWord4 : 5; //Word4 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetMuWord5 : 5; //Word5 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetMuWord6 : 5; //Word6 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetMuWord7 : 5; //Word7 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetMuWord47_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetMuWord8 : 5; //Word8 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetMuWord9 : 5; //Word9 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetMuWord10 : 5; //Word10 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetMuWord11 : 5; //Word11 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetMuWord811_u;

/*REG_B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 offsetMuWord12 : 5; //Word12 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 offsetMuWord13 : 5; //Word13 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 offsetMuWord14 : 5; //Word14 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 offsetMuWord15 : 5; //Word15 offset in the MU entry, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB2PacExtrapolatorOffsetMuWord1215_u;

/*REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 signExtWord0 : 5; //each bit controls sign-extension of the corresponding field of the relevant word., reset value: 0x1f, access type: RW
		uint32 signExtWord1 : 5; //each bit controls sign-extension of the corresponding field of the relevant word., reset value: 0x1f, access type: RW
		uint32 signExtWord2 : 5; //each bit controls sign-extension of the corresponding field of the relevant word., reset value: 0x1f, access type: RW
		uint32 signExtWord3 : 5; //each bit controls sign-extension of the corresponding field of the relevant word., reset value: 0x1f, access type: RW
		uint32 signExtWord4 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord5 : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2PacExtrapolatorSignExtWord05_u;

/*REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 signExtWord6 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord7 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord8 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord9 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord10 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord11 : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2PacExtrapolatorSignExtWord611_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_0_31 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn031 : 32; //STA enable bits 0 to 31. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn031_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_32_63 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn3263 : 32; //STA enable bits 32 to 63. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn3263_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_64_95 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn6495 : 32; //STA enable bits 64 to 95. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn6495_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_96_127 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn96127 : 32; //STA enable bits 96 to 127. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn96127_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_128_159 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn128159 : 32; //STA enable bits 128 to 159. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn128159_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_160_191 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn160191 : 32; //STA enable bits 160 to 191. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn160191_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_192_223 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn192223 : 32; //STA enable bits 192 to 223. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn192223_u;

/*REG_B2_PAC_EXTRAPOLATOR_STA_EN_224_255 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staEn224255 : 32; //STA enable bits 224 to 255. Default is Disabled, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorStaEn224255_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_REQ 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacMuAfReq : 1; //Send MAC AF start request, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacExtrapolatorSwMacMuAfReq_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacInpEntrySize : 2; //number of 32bit words per Input entry (1 or 2), reset value: 0x0, access type: RW
		uint32 swMacTsfRes : 4; //MAC AF tsf resolution , reset value: 0x0, access type: RW
		uint32 swMacAfDone : 1; //Processing done indication, reset value: 0x0, access type: RO
		uint32 swMacAfReqError : 1; //Entry access conflict - collides with Entry Update API. Error IRQ is also asserted., reset value: 0x0, access type: RO
		uint32 swMacErrIrqClear : 1; //clear error IRQ, reset value: 0x0, access type: WO
		uint32 swMacErrIrqEna : 1; //Enable Error IRQ assertion is case of entry access conflict., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2PacExtrapolatorSwMacMuAfCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_USER_VALID_VEC 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacUserValidVec : 32; //Enable bit per user for MAC AF., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwMacUserValidVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_REQ 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swEntryUpdateApiReq : 1; //Set entry update API request., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacExtrapolatorSwApiEntryUpdateReq_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_STATUS 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swApiUpdActive : 1; //Update is ongoing, reset value: 0x0, access type: RO
		uint32 swApiUpdPending : 1; //The API waits for Phy AF to finish, reset value: 0x0, access type: RO
		uint32 swApiDbUpdError : 1; //When asserted = the requested entry is corrupted due to collision. IRQ is also asserted., reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2PacExtrapolatorSwApiStatus_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swApiEntryId : 8; //Entry ID can be either STA_ID or USER_ID., reset value: 0x0, access type: RW
		uint32 swApiEntrySize : 6; //Entry size of the requested DB to update., reset value: 0x0, access type: RW
		uint32 swApiLastWordNum : 5; //Last word in mirror buffer = last word of an array to copy from., reset value: 0x0, access type: RW
		uint32 swApiErrIrqClear : 1; //Clear API error IRQ., reset value: 0x0, access type: WO
		uint32 swApiErrIrqEna : 1; //Enable API Error IRQ (entry access conflict)., reset value: 0x0, access type: RW
		uint32 reserved0 : 11;
	} bitFields;
} RegB2PacExtrapolatorSwApiEntryUpdateCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_MIRROR_ADDR 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swApiMirrorAddr : 22; //Address of 32 words array (or less) to copy words from., reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorSwApiMirrorAddr_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_DB_ADDR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swApiDbAddr : 22; //Target DB pointer to copy there words from mirror buffer., reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorSwApiDbAddr_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_API_WORD_UPD_ENA_VEC 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swApiWordUpdEnaVec : 32; //Each bit enables overwriting the word at the corresponding position in the target DB entry by the relevant word of mirror buffer., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwApiWordUpdEnaVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_REQ 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmReqSet : 1; //Set Immediate calc SW request, reset value: 0x0, access type: WO
		uint32 swImmBusy : 1; //Clear interrupt indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2PacExtrapolatorSwImmReq_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_ALFA 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmCurrentAlfa : 21; //Set SW current alfa, reset value: 0x0, access type: RW
		uint32 reserved0 : 11;
	} bitFields;
} RegB2PacExtrapolatorSwImmCurrentAlfa_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmCurrentParameter : 32; //Set SW current parameter, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwImmCurrentParameter_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmPreviousAvgWeighted : 32; //Set SW previous avg weighted, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwImmPreviousAvgWeighted_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_TSF 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmCurrentTsf : 32; //Set SW current TSF, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwImmCurrentTsf_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_TSF 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmPreviousTsf : 32; //Set SW previous TSF, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwImmPreviousTsf_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_ABS_LN_BETA 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmAbsLnBeta : 12; //Set SW abs ln beta, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB2PacExtrapolatorSwImmAbsLnBeta_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_AVG_DELTA_T 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmAvgDeltaT : 15; //Set SW average delta T, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB2PacExtrapolatorSwImmAvgDeltaT_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmNumOfValidFields : 3; //Set SW number of valid fields: , 0x0: one field (1 Byte) , 0x1: two fields (2 Byte) , 0x2: three fields (3 Byte) , 0x3: four fields (4 Byte) , 0x4: five fields (5 Byte), reset value: 0x0, access type: RW
		uint32 swImmHalfWordEn : 1; //Set SW half word enable, if enable number of fields ignored, reset value: 0x0, access type: RW
		uint32 swImmSignExt : 5; //SW sign extension indication per word field, reset value: 0x0, access type: RW
		uint32 swImmTsfRes : 4; //Set SW tsf resolution, reset value: 0x7, access type: RW
		uint32 swImmReqAlfaMode : 1; //Set SW request alfa mode: , 0x0: Basic alfa mode , 0x1: Alfa Beta mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB2PacExtrapolatorSwImmReqCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_ALFA 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmNewAlfa : 21; //SW calculated new alfa parameter, reset value: 0x0, access type: RO
		uint32 reserved0 : 11;
	} bitFields;
} RegB2PacExtrapolatorSwImmNewAlfa_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmNewAvgWeighted : 32; //SW new avg weighted, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorSwImmNewAvgWeighted_u;

/*REG_B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerPhyEn : 1; //Phy AF Logger enable bit, Default is Disabled, reset value: 0x0, access type: RW
		uint32 loggerMacEn : 1; //MAC AF Logger enable, default: Disabled, reset value: 0x0, access type: RW
		uint32 loggerPhyActive : 1; //Phy AF Logger active indication, reset value: 0x0, access type: RO
		uint32 loggerMacActive : 1; //MAC AF Logger active indication, reset value: 0x0, access type: RO
		uint32 loggerPhyMsgEna : 8; //One enable bit per Phy AF message type. Upto 8 diff. msg_ids., reset value: 0x0, access type: RW
		uint32 loggerMacMsgEna : 8; //One enable bit per MAC AF message type. Upto 8 diff. msg_ids., reset value: 0x0, access type: RW
		uint32 loggerPhyPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 loggerMacPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2PacExtrapolatorExtrapLoggerCtrl_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_REG 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugExtrapHwActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 debugExtrapSwImmActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 debugExtrapSwMacActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 debugWordIdx : 4; //no description, reset value: 0x0, access type: RO
		uint32 debugFieldIdx : 3; //no description, reset value: 0x0, access type: RO
		uint32 debugExtrapMainSm : 4; //no description, reset value: 0x0, access type: RO
		uint32 debugPhyModeSmp : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB2PacExtrapolatorDebugReg_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_CURRENT_TSF 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugCurrentTsf : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorDebugCurrentTsf_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugDataInField0 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataInField1 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataInField2 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataInField3 : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorDebugDataIn_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_NEW_ALFA 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugNewAlfa : 21; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 11;
	} bitFields;
} RegB2PacExtrapolatorDebugNewAlfa_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugDataNewField0 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataNewField1 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataNewField2 : 8; //no description, reset value: 0x0, access type: RO
		uint32 debugDataNewField3 : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorDebugDataNew_u;

/*REG_B2_PAC_EXTRAPOLATOR_DEBUG_LAST_SMC_ADDR 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugLastSmcAddr : 22; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorDebugLastSmcAddr_u;

/*REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_DBG 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minRssiDbgValue : 8; //no description, reset value: 0x81, access type: RW
		uint32 minRssiDbgStb : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2PacExtrapolatorMinRssiDbg_u;

/*REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_DB_BASE 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minRssiDbBase : 22; //Base address of "Minimal RSSI per STA" database, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorMinRssiDbBase_u;

/*REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minRssiTrig : 1; //Trigger immediate RSSI DB update when sta dropped + set 80h value to the dropped entry., reset value: 0x0, access type: RW
		uint32 minRssiCdb : 1; //Single Band (0) or CDB mode (1), reset value: 0x0, access type: RW
		uint32 minRssiEna : 1; //Enables automatic min_rssi_search triggering. , Disable only at idle state of both engines!!! (min_rssi & Phy AF)., reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 minRssiStaId : 8; //STA ID to overwrite with 80h (=dropped/invalid) @ min_rssi_trig, reset value: 0x0, access type: RW
		uint32 minRssiTrsh : 8; //Min RSSI threshold - value to provide to Phy when all STA's are 80h, reset value: 0x81, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegB2PacExtrapolatorMinRssiCfg_u;

/*REG_B2_PAC_EXTRAPOLATOR_MIN_RSSI_IDLE_DUR 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minRssiIdleDur : 32; //Number of microseconds at idle state - when the internal timer reaches this value, the min_RSSI DB is auto-initialized (set 80h to all STAs entries)., reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegB2PacExtrapolatorMinRssiIdleDur_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minRssiByteCnt : 4; //no description, reset value: 0x0, access type: RO
		uint32 minRssiAddrCnt : 4; //no description, reset value: 0x0, access type: RO
		uint32 minRssiSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 minRssiStaIdx : 9; //no description, reset value: 0x1ff, access type: RO
		uint32 minRssiSwUpdPend : 1; //no description, reset value: 0x0, access type: RO
		uint32 minRssiHwUpdPend : 1; //no description, reset value: 0x0, access type: RO
		uint32 minRssiReinitPend : 1; //no description, reset value: 0x0, access type: RO
		uint32 minRssiHwSrchPend : 1; //no description, reset value: 0x0, access type: RO
		uint32 minRssiSelMaster : 2; //0: HW_UPD, 1: SW_UPD, 2: HW_SRCH, 3: RE_INIT, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB2PacExtrapolatorDbgInfoMinRssi_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fetchSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 prepCurrSta : 8; //no description, reset value: 0x0, access type: RO
		uint32 prepCurrUser : 6; //no description, reset value: 0x0, access type: RO
		uint32 prepNoMoreUsers : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB2PacExtrapolatorDbgInfoAfFetchPrep_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAfSm : 4; //no description, reset value: 0x0, access type: RO
		uint32 macAfWordCnt : 4; //no description, reset value: 0x0, access type: RO
		uint32 macAfCurrUser : 5; //no description, reset value: 0x0, access type: RO
		uint32 macAfWordsReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 macAfNoReinit : 1; //no description, reset value: 0x1, access type: RO
		uint32 macAfNoUsers : 1; //no description, reset value: 0x1, access type: RO
		uint32 macAfGatorEna : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB2PacExtrapolatorDbgInfoMacAf_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_USERS_DONE 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAfUserDoneVec : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorDbgInfoMacUsersDone_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_REINIT_DONE 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAfUserReinitDoneVec : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacExtrapolatorDbgInfoMacReinitDone_u;

/*REG_B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 updApiSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 updApiWordCnt : 5; //no description, reset value: 0x0, access type: RO
		uint32 updApiGatorEna : 1; //no description, reset value: 0x0, access type: RO
		uint32 updApiRdAddr : 8; //no description, reset value: 0x0, access type: RO
		uint32 updApiWrAddr : 8; //no description, reset value: 0x0, access type: RO
		uint32 updApiPhyAfCollision : 1; //High when Phy AF works on the same user group (API will be pending), reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
	} bitFields;
} RegB2PacExtrapolatorDbgInfoUpdApi_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_TSF 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacMuAfTsf : 32; //TSF value of the MAC words sampling moment., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwMacMuAfTsf_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_USER_REINIT_VEC 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacUserReinitVec : 32; //Bit per user. "1" means treat/overwrite old DB value as "0".  , Active even if the user isn't "valid"., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacExtrapolatorSwMacUserReinitVec_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_INPUT_BUFFER 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacInputBuffer : 22; //MAC words input buffer pointer (ShRAM), reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorSwMacInputBuffer_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_MAC_MU_DB_PTR 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMacMuDbPtr : 22; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacExtrapolatorSwMacMuDbPtr_u;

/*REG_B2_PAC_EXTRAPOLATOR_MU_DB_ENTRY_SIZE 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muDbEntrySize : 6; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB2PacExtrapolatorMuDbEntrySize_u;

/*REG_B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 signExtWord12 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord13 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord14 : 5; //no description, reset value: 0x0, access type: RW
		uint32 signExtWord15 : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB2PacExtrapolatorSignExtWord1215_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER_5TH 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmCurrentParameter5Th : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacExtrapolatorSwImmCurrentParameter5Th_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED_5TH 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmPreviousAvgWeighted5Th : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacExtrapolatorSwImmPreviousAvgWeighted5Th_u;

/*REG_B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED_5TH 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swImmNewAvgWeighted5Th : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacExtrapolatorSwImmNewAvgWeighted5Th_u;

//========================================
/* REG_PAC_EXTRAPOLATOR_EXTRAP_CONTROL_CFG 0x0 */
#define B2_PAC_EXTRAPOLATOR_EXTRAP_CONTROL_CFG_FIELD_EXTRAP_EN_MASK                                               0x00000001
#define B2_PAC_EXTRAPOLATOR_EXTRAP_CONTROL_CFG_FIELD_EXTRAP_BUSY_MASK                                             0x00000002
//========================================
/* REG_PAC_EXTRAPOLATOR_SU_STA_DB_CFG 0x4 */
#define B2_PAC_EXTRAPOLATOR_SU_STA_DB_CFG_FIELD_SU_STA_DB_BASE_ADDR_MASK                                          0x003FFFFF
#define B2_PAC_EXTRAPOLATOR_SU_STA_DB_CFG_FIELD_SU_DB_ENTRY_SIZE_MASK                                             0x3F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_MU_DB_CFG 0x8 */
#define B2_PAC_EXTRAPOLATOR_MU_DB_CFG_FIELD_MU_DB_BASE_ADDR_MASK                                                  0x003FFFFF
#define B2_PAC_EXTRAPOLATOR_MU_DB_CFG_FIELD_MU_DB_UPDATE_ENA_MASK                                                 0x00400000
//========================================
/* REG_PAC_EXTRAPOLATOR_MU_USER_REINIT_VEC 0xC */
#define B2_PAC_EXTRAPOLATOR_MU_USER_REINIT_VEC_FIELD_MU_USER_REINIT_VEC_MASK                                      0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_LN_BETA_LUT_BASE_ADDR 0x10 */
#define B2_PAC_EXTRAPOLATOR_LN_BETA_LUT_BASE_ADDR_FIELD_LN_BETA_LUT_BASE_ADDR_MASK                                0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_WORD_UPD_EN_VEC 0x14 */
#define B2_PAC_EXTRAPOLATOR_WORD_UPD_EN_VEC_FIELD_WORD_UPD_EN_VEC_MASK                                            0x0000FFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_WORD_METHOD_VEC 0x18 */
#define B2_PAC_EXTRAPOLATOR_WORD_METHOD_VEC_FIELD_WORD_METHOD_VEC_MASK                                            0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_TSF_RESOLUTION_CFG 0x1C */
#define B2_PAC_EXTRAPOLATOR_TSF_RESOLUTION_CFG_FIELD_SU_TSF_RESOLUTION_MASK                                       0x0000000F
#define B2_PAC_EXTRAPOLATOR_TSF_RESOLUTION_CFG_FIELD_MU_TSF_RESOLUTION_MASK                                       0x000000F0
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3 0x24 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3_FIELD_OFFSET_SU_WORD0_MASK                                         0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3_FIELD_OFFSET_SU_WORD1_MASK                                         0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3_FIELD_OFFSET_SU_WORD2_MASK                                         0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_0_3_FIELD_OFFSET_SU_WORD3_MASK                                         0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7 0x28 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7_FIELD_OFFSET_SU_WORD4_MASK                                         0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7_FIELD_OFFSET_SU_WORD5_MASK                                         0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7_FIELD_OFFSET_SU_WORD6_MASK                                         0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_4_7_FIELD_OFFSET_SU_WORD7_MASK                                         0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11 0x2C */
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11_FIELD_OFFSET_SU_WORD8_MASK                                        0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11_FIELD_OFFSET_SU_WORD9_MASK                                        0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11_FIELD_OFFSET_SU_WORD10_MASK                                       0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_8_11_FIELD_OFFSET_SU_WORD11_MASK                                       0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15 0x30 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15_FIELD_OFFSET_SU_WORD12_MASK                                      0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15_FIELD_OFFSET_SU_WORD13_MASK                                      0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15_FIELD_OFFSET_SU_WORD14_MASK                                      0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_SU_WORD_12_15_FIELD_OFFSET_SU_WORD15_MASK                                      0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3 0x34 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3_FIELD_OFFSET_MU_WORD0_MASK                                         0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3_FIELD_OFFSET_MU_WORD1_MASK                                         0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3_FIELD_OFFSET_MU_WORD2_MASK                                         0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_0_3_FIELD_OFFSET_MU_WORD3_MASK                                         0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7 0x38 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7_FIELD_OFFSET_MU_WORD4_MASK                                         0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7_FIELD_OFFSET_MU_WORD5_MASK                                         0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7_FIELD_OFFSET_MU_WORD6_MASK                                         0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_4_7_FIELD_OFFSET_MU_WORD7_MASK                                         0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11 0x3C */
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11_FIELD_OFFSET_MU_WORD8_MASK                                        0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11_FIELD_OFFSET_MU_WORD9_MASK                                        0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11_FIELD_OFFSET_MU_WORD10_MASK                                       0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_8_11_FIELD_OFFSET_MU_WORD11_MASK                                       0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15 0x40 */
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15_FIELD_OFFSET_MU_WORD12_MASK                                      0x0000001F
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15_FIELD_OFFSET_MU_WORD13_MASK                                      0x00001F00
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15_FIELD_OFFSET_MU_WORD14_MASK                                      0x001F0000
#define B2_PAC_EXTRAPOLATOR_OFFSET_MU_WORD_12_15_FIELD_OFFSET_MU_WORD15_MASK                                      0x1F000000
//========================================
/* REG_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5 0x44 */
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD0_MASK                                           0x0000001F
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD1_MASK                                           0x000003E0
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD2_MASK                                           0x00007C00
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD3_MASK                                           0x000F8000
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD4_MASK                                           0x01F00000
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_0_5_FIELD_SIGN_EXT_WORD5_MASK                                           0x3E000000
//========================================
/* REG_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11 0x48 */
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD6_MASK                                          0x0000001F
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD7_MASK                                          0x000003E0
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD8_MASK                                          0x00007C00
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD9_MASK                                          0x000F8000
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD10_MASK                                         0x01F00000
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_6_11_FIELD_SIGN_EXT_WORD11_MASK                                         0x3E000000
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_0_31 0x4C */
#define B2_PAC_EXTRAPOLATOR_STA_EN_0_31_FIELD_STA_EN_0_31_MASK                                                    0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_32_63 0x50 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_32_63_FIELD_STA_EN_32_63_MASK                                                  0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_64_95 0x54 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_64_95_FIELD_STA_EN_64_95_MASK                                                  0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_96_127 0x58 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_96_127_FIELD_STA_EN_96_127_MASK                                                0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_128_159 0x5C */
#define B2_PAC_EXTRAPOLATOR_STA_EN_128_159_FIELD_STA_EN_128_159_MASK                                              0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_160_191 0x60 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_160_191_FIELD_STA_EN_160_191_MASK                                              0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_192_223 0x64 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_192_223_FIELD_STA_EN_192_223_MASK                                              0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_STA_EN_224_255 0x68 */
#define B2_PAC_EXTRAPOLATOR_STA_EN_224_255_FIELD_STA_EN_224_255_MASK                                              0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_REQ 0x6C */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_REQ_FIELD_SW_MAC_MU_AF_REQ_MASK                                          0x00000001
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG 0x70 */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_INP_ENTRY_SIZE_MASK                                     0x00000003
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_TSF_RES_MASK                                            0x0000003C
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_AF_DONE_MASK                                            0x00000040
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_AF_REQ_ERROR_MASK                                       0x00000080
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_ERR_IRQ_CLEAR_MASK                                      0x00000100
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_CFG_FIELD_SW_MAC_ERR_IRQ_ENA_MASK                                        0x00000200
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_USER_VALID_VEC 0x74 */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_USER_VALID_VEC_FIELD_SW_MAC_USER_VALID_VEC_MASK                                0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_REQ 0x78 */
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_REQ_FIELD_SW_ENTRY_UPDATE_API_REQ_MASK                            0x00000001
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_STATUS 0x7C */
#define B2_PAC_EXTRAPOLATOR_SW_API_STATUS_FIELD_SW_API_UPD_ACTIVE_MASK                                            0x00000001
#define B2_PAC_EXTRAPOLATOR_SW_API_STATUS_FIELD_SW_API_UPD_PENDING_MASK                                           0x00000002
#define B2_PAC_EXTRAPOLATOR_SW_API_STATUS_FIELD_SW_API_DB_UPD_ERROR_MASK                                          0x00000004
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG 0x80 */
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG_FIELD_SW_API_ENTRY_ID_MASK                                    0x000000FF
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG_FIELD_SW_API_ENTRY_SIZE_MASK                                  0x00003F00
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG_FIELD_SW_API_LAST_WORD_NUM_MASK                               0x0007C000
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG_FIELD_SW_API_ERR_IRQ_CLEAR_MASK                               0x00080000
#define B2_PAC_EXTRAPOLATOR_SW_API_ENTRY_UPDATE_CFG_FIELD_SW_API_ERR_IRQ_ENA_MASK                                 0x00100000
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_MIRROR_ADDR 0x84 */
#define B2_PAC_EXTRAPOLATOR_SW_API_MIRROR_ADDR_FIELD_SW_API_MIRROR_ADDR_MASK                                      0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_DB_ADDR 0x88 */
#define B2_PAC_EXTRAPOLATOR_SW_API_DB_ADDR_FIELD_SW_API_DB_ADDR_MASK                                              0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_API_WORD_UPD_ENA_VEC 0x8C */
#define B2_PAC_EXTRAPOLATOR_SW_API_WORD_UPD_ENA_VEC_FIELD_SW_API_WORD_UPD_ENA_VEC_MASK                            0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_REQ 0x90 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_FIELD_SW_IMM_REQ_SET_MASK                                                  0x00000001
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_FIELD_SW_IMM_BUSY_MASK                                                     0x00000002
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_ALFA 0x94 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_ALFA_FIELD_SW_IMM_CURRENT_ALFA_MASK                                    0x001FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER 0x98 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER_FIELD_SW_IMM_CURRENT_PARAMETER_MASK                          0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED 0x9C */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED_FIELD_SW_IMM_PREVIOUS_AVG_WEIGHTED_MASK                  0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_TSF 0xA0 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_TSF_FIELD_SW_IMM_CURRENT_TSF_MASK                                      0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_TSF 0xA4 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_TSF_FIELD_SW_IMM_PREVIOUS_TSF_MASK                                    0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_ABS_LN_BETA 0xA8 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_ABS_LN_BETA_FIELD_SW_IMM_ABS_LN_BETA_MASK                                      0x00000FFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_AVG_DELTA_T 0xAC */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_AVG_DELTA_T_FIELD_SW_IMM_AVG_DELTA_T_MASK                                      0x00007FFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG 0xB0 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG_FIELD_SW_IMM_NUM_OF_VALID_FIELDS_MASK                                  0x00000007
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG_FIELD_SW_IMM_HALF_WORD_EN_MASK                                         0x00000008
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG_FIELD_SW_IMM_SIGN_EXT_MASK                                             0x000001F0
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG_FIELD_SW_IMM_TSF_RES_MASK                                              0x00001E00
#define B2_PAC_EXTRAPOLATOR_SW_IMM_REQ_CFG_FIELD_SW_IMM_REQ_ALFA_MODE_MASK                                        0x00002000
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_NEW_ALFA 0xB4 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_ALFA_FIELD_SW_IMM_NEW_ALFA_MASK                                            0x001FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED 0xB8 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED_FIELD_SW_IMM_NEW_AVG_WEIGHTED_MASK                            0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL 0xBC */
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_PHY_EN_MASK                                           0x00000001
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_MAC_EN_MASK                                           0x00000002
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_PHY_ACTIVE_MASK                                       0x00000004
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_MAC_ACTIVE_MASK                                       0x00000008
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_PHY_MSG_ENA_MASK                                      0x00000FF0
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_MAC_MSG_ENA_MASK                                      0x000FF000
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_PHY_PRIORITY_MASK                                     0x00300000
#define B2_PAC_EXTRAPOLATOR_EXTRAP_LOGGER_CTRL_FIELD_LOGGER_MAC_PRIORITY_MASK                                     0x00C00000
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_REG 0xC0 */
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_EXTRAP_HW_ACTIVE_MASK                                           0x00000001
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_EXTRAP_SW_IMM_ACTIVE_MASK                                       0x00000002
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_EXTRAP_SW_MAC_ACTIVE_MASK                                       0x00000004
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_WORD_IDX_MASK                                                   0x00000078
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_FIELD_IDX_MASK                                                  0x00000380
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_EXTRAP_MAIN_SM_MASK                                             0x00003C00
#define B2_PAC_EXTRAPOLATOR_DEBUG_REG_FIELD_DEBUG_PHY_MODE_SMP_MASK                                               0x0001C000
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_CURRENT_TSF 0xC4 */
#define B2_PAC_EXTRAPOLATOR_DEBUG_CURRENT_TSF_FIELD_DEBUG_CURRENT_TSF_MASK                                        0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_DATA_IN 0xC8 */
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN_FIELD_DEBUG_DATA_IN_FIELD0_MASK                                         0x000000FF
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN_FIELD_DEBUG_DATA_IN_FIELD1_MASK                                         0x0000FF00
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN_FIELD_DEBUG_DATA_IN_FIELD2_MASK                                         0x00FF0000
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_IN_FIELD_DEBUG_DATA_IN_FIELD3_MASK                                         0xFF000000
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_NEW_ALFA 0xCC */
#define B2_PAC_EXTRAPOLATOR_DEBUG_NEW_ALFA_FIELD_DEBUG_NEW_ALFA_MASK                                              0x001FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW 0xE0 */
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW_FIELD_DEBUG_DATA_NEW_FIELD0_MASK                                       0x000000FF
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW_FIELD_DEBUG_DATA_NEW_FIELD1_MASK                                       0x0000FF00
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW_FIELD_DEBUG_DATA_NEW_FIELD2_MASK                                       0x00FF0000
#define B2_PAC_EXTRAPOLATOR_DEBUG_DATA_NEW_FIELD_DEBUG_DATA_NEW_FIELD3_MASK                                       0xFF000000
//========================================
/* REG_PAC_EXTRAPOLATOR_DEBUG_LAST_SMC_ADDR 0xE4 */
#define B2_PAC_EXTRAPOLATOR_DEBUG_LAST_SMC_ADDR_FIELD_DEBUG_LAST_SMC_ADDR_MASK                                    0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_MIN_RSSI_DBG 0xE8 */
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_DBG_FIELD_MIN_RSSI_DBG_VALUE_MASK                                            0x000000FF
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_DBG_FIELD_MIN_RSSI_DBG_STB_MASK                                              0x00000100
//========================================
/* REG_PAC_EXTRAPOLATOR_MIN_RSSI_DB_BASE 0xEC */
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_DB_BASE_FIELD_MIN_RSSI_DB_BASE_MASK                                          0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_MIN_RSSI_CFG 0xF0 */
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG_FIELD_MIN_RSSI_TRIG_MASK                                                 0x00000001
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG_FIELD_MIN_RSSI_CDB_MASK                                                  0x00000002
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG_FIELD_MIN_RSSI_ENA_MASK                                                  0x00000004
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG_FIELD_MIN_RSSI_STA_ID_MASK                                               0x0000FF00
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_CFG_FIELD_MIN_RSSI_TRSH_MASK                                                 0x00FF0000
//========================================
/* REG_PAC_EXTRAPOLATOR_MIN_RSSI_IDLE_DUR 0xF4 */
#define B2_PAC_EXTRAPOLATOR_MIN_RSSI_IDLE_DUR_FIELD_MIN_RSSI_IDLE_DUR_MASK                                        0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI 0xF8 */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_BYTE_CNT_MASK                                        0x0000000F
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_ADDR_CNT_MASK                                        0x000000F0
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_SM_MASK                                              0x00000700
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_STA_IDX_MASK                                         0x000FF800
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_SW_UPD_PEND_MASK                                     0x00100000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_HW_UPD_PEND_MASK                                     0x00200000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_REINIT_PEND_MASK                                     0x00400000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_HW_SRCH_PEND_MASK                                    0x00800000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MIN_RSSI_FIELD_MIN_RSSI_SEL_MASTER_MASK                                      0x03000000
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP 0xFC */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP_FIELD_FETCH_SM_MASK                                            0x00000007
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP_FIELD_PREP_CURR_STA_MASK                                       0x000007F8
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP_FIELD_PREP_CURR_USER_MASK                                      0x0001F800
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_AF_FETCH_PREP_FIELD_PREP_NO_MORE_USERS_MASK                                  0x00020000
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF 0x100 */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_SM_MASK                                                  0x0000000F
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_WORD_CNT_MASK                                            0x000000F0
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_CURR_USER_MASK                                           0x00001F00
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_WORDS_READY_MASK                                         0x00002000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_NO_REINIT_MASK                                           0x00004000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_NO_USERS_MASK                                            0x00008000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_AF_FIELD_MAC_AF_GATOR_ENA_MASK                                           0x00010000
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_MAC_USERS_DONE 0x104 */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_USERS_DONE_FIELD_MAC_AF_USER_DONE_VEC_MASK                               0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_MAC_REINIT_DONE 0x108 */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_MAC_REINIT_DONE_FIELD_MAC_AF_USER_REINIT_DONE_VEC_MASK                       0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API 0x10C */
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_SM_MASK                                                0x00000003
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_WORD_CNT_MASK                                          0x0000007C
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_GATOR_ENA_MASK                                         0x00000080
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_RD_ADDR_MASK                                           0x0000FF00
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_WR_ADDR_MASK                                           0x00FF0000
#define B2_PAC_EXTRAPOLATOR_DBG_INFO_UPD_API_FIELD_UPD_API_PHY_AF_COLLISION_MASK                                  0x01000000
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_TSF 0x120 */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_AF_TSF_FIELD_SW_MAC_MU_AF_TSF_MASK                                          0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_USER_REINIT_VEC 0x124 */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_USER_REINIT_VEC_FIELD_SW_MAC_USER_REINIT_VEC_MASK                              0xFFFFFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_INPUT_BUFFER 0x128 */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_INPUT_BUFFER_FIELD_SW_MAC_INPUT_BUFFER_MASK                                    0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_MAC_MU_DB_PTR 0x12C */
#define B2_PAC_EXTRAPOLATOR_SW_MAC_MU_DB_PTR_FIELD_SW_MAC_MU_DB_PTR_MASK                                          0x003FFFFF
//========================================
/* REG_PAC_EXTRAPOLATOR_MU_DB_ENTRY_SIZE 0x130 */
#define B2_PAC_EXTRAPOLATOR_MU_DB_ENTRY_SIZE_FIELD_MU_DB_ENTRY_SIZE_MASK                                          0x0000003F
//========================================
/* REG_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15 0x134 */
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15_FIELD_SIGN_EXT_WORD12_MASK                                        0x0000001F
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15_FIELD_SIGN_EXT_WORD13_MASK                                        0x000003E0
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15_FIELD_SIGN_EXT_WORD14_MASK                                        0x00007C00
#define B2_PAC_EXTRAPOLATOR_SIGN_EXT_WORD_12_15_FIELD_SIGN_EXT_WORD15_MASK                                        0x000F8000
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER_5TH 0x138 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_CURRENT_PARAMETER_5TH_FIELD_SW_IMM_CURRENT_PARAMETER_5TH_MASK                  0x000000FF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED_5TH 0x13C */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_PREVIOUS_AVG_WEIGHTED_5TH_FIELD_SW_IMM_PREVIOUS_AVG_WEIGHTED_5TH_MASK          0x000000FF
//========================================
/* REG_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED_5TH 0x140 */
#define B2_PAC_EXTRAPOLATOR_SW_IMM_NEW_AVG_WEIGHTED_5TH_FIELD_SW_IMM_NEW_AVG_WEIGHTED_5TH_MASK                    0x000000FF


#endif // _PAC_EXTRAPOLATOR_REGS_H_
