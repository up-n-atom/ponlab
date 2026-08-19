
/***********************************************************************************
File:				B0PhyTxtdAnt2Regs.h
Module:				b0PhyTxtdAnt2
SOC Revision:		latest
Generated at:       2022-05-04 10:57:24
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PHY_TXTD_ANT2_REGS_H_
#define _B0_PHY_TXTD_ANT2_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PHY_TXTD_ANT2_BASE_ADDRESS                             MEMORY_MAP_UNIT_232_BASE_ADDRESS
#define	REG_B0_PHY_TXTD_ANT2_TX_MODE_11B_EN                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x0)
#define	REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_0                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x8)
#define	REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_1                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xC)
#define	REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_2                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x10)
#define	REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_3                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x14)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_TEST_PLUG_MUX_CONTROL        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x18)
#define	REG_B0_PHY_TXTD_ANT2_TX_ANT_RAM_RM                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x1C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_ANT_SPARE                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x20)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_ANT_BYPASS_REG               (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x24)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_WINDOWING                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x28)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_DUP_SWAP_FLIP                (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_MIX11B_FREQ_OFFSET           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x30)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_0                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x34)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_1                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x38)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_2                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x3C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_3                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x40)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_4                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x44)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_5                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x48)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_6                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x4C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_7                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x50)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_MEM_GLOBAL_RM                (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x54)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_MEM_GLOBAL_TEST_MODE         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x58)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_SPARE                        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x5C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_1ST_PHASE           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x60)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_2ND_PHASE           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x64)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_BIST                         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x68)
#define	REG_B0_PHY_TXTD_ANT2_TX_IQ_RIPPLE                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x6C)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_IFFT_IN_OUT_SHIFT            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x70)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_END_ZEROS                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x74)
#define	REG_B0_PHY_TXTD_ANT2_TX_ABB_30                         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x78)
#define	REG_B0_PHY_TXTD_ANT2_TX_ANT_REG_31                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x7C)
#define	REG_B0_PHY_TXTD_ANT2_TX_BF_CAL_GAIN_INDEX_ANT          (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x80)
#define	REG_B0_PHY_TXTD_ANT2_TPC_ANT_RAM                       (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x84)
#define	REG_B0_PHY_TXTD_ANT2_TPC_ACCELERATOR                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x88)
#define	REG_B0_PHY_TXTD_ANT2_TPC_TSSI_A_B                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x8C)
#define	REG_B0_PHY_TXTD_ANT2_TPC_TSSI_C                        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x90)
#define	REG_B0_PHY_TXTD_ANT2_TPC_SHORT                         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x94)
#define	REG_B0_PHY_TXTD_ANT2_RF_POWER                          (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x98)
#define	REG_B0_PHY_TXTD_ANT2_TX_DIGITAL_GAIN                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x9C)
#define	REG_B0_PHY_TXTD_ANT2_TX_PGC2_GAIN_SELECT_OUT           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xA0)
#define	REG_B0_PHY_TXTD_ANT2_RF_POWER_OFFSET                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xA4)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_IQ_0                         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xA8)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_IQ_1                         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xAC)
#define	REG_B0_PHY_TXTD_ANT2_STREAMER                          (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xB0)
#define	REG_B0_PHY_TXTD_ANT2_TEST_PLUG_LEN                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xB4)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS_COEFF_0                  (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xB8)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPS_COEFF_2                  (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xBC)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_UPSRF_COEFF_0                (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xC0)
#define	REG_B0_PHY_TXTD_ANT2_PM_ANALOG_LPBK_EN                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xC4)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_GCLK_EN                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xC8)
#define	REG_B0_PHY_TXTD_ANT2_IDLE_MODE_EN                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xCC)
#define	REG_B0_PHY_TXTD_ANT2_TX_SC2FC_TXON                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xD0)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_BYPASS_SEL                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xD4)
#define	REG_B0_PHY_TXTD_ANT2_TX_PPM_FREQ_IN_CFG                (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xD8)
#define	REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_UP_LIM_CFG          (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xE0)
#define	REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_INCR_MAX_CFG        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xE4)
#define	REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_INCR_CFG            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xE8)
#define	REG_B0_PHY_TXTD_ANT2_DPD_CFG                           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xEC)
#define	REG_B0_PHY_TXTD_ANT2_DPD_STATUS                        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xF0)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_CTL                       (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xF4)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_0                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xF8)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_1                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0xFC)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_2                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x100)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_3                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x104)
#define	REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_4                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x108)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_0                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x10C)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_1                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x110)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_2                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x114)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_3                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x118)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_4                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x11C)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_5                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x120)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_6                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x124)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_7                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x128)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_8                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x12C)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_0                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x130)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_1                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x134)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_2                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x138)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_3                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x13C)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_4                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x140)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_5                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x144)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_6                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x148)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_7                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x14C)
#define	REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_8                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x150)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_ANT_RAM_POWER_SAVE           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x29C)
#define	REG_B0_PHY_TXTD_ANT2_TPC_DPD                           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2A0)
#define	REG_B0_PHY_TXTD_ANT2_CLIPPER_CONF                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2A4)
#define	REG_B0_PHY_TXTD_ANT2_CLIPPER_THRESHOLD                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2A8)
#define	REG_B0_PHY_TXTD_ANT2_LOOPBACK_PYPASS                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2AC)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_GCLK_BYPASS                  (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2B0)
#define	REG_B0_PHY_TXTD_ANT2_PM_COMB_BYPASS                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2B8)
#define	REG_B0_PHY_TXTD_ANT2_STAT_COMB_VALID_MISMATCH          (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2BC)
#define	REG_B0_PHY_TXTD_ANT2_STAT_COMB_OUT_SAT                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2C0)
#define	REG_B0_PHY_TXTD_ANT2_STAT_COMB_VALID_MISMATCH_CLEAR    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2C8)
#define	REG_B0_PHY_TXTD_ANT2_STAT_COMB_OUT_SAT_CLEAR           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2CC)
#define	REG_B0_PHY_TXTD_ANT2_PM_CNF_BYPASS                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2D0)
#define	REG_B0_PHY_TXTD_ANT2_PM_CNF_AUTOMATIC_BYPASS_BY_BW     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2D8)
#define	REG_B0_PHY_TXTD_ANT2_PM_SYMBOL_TYPE_CLIPPER_LAT        (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2DC)
#define	REG_B0_PHY_TXTD_ANT2_RF_POWER_OFFSET_320               (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2E0)
#define	REG_B0_PHY_TXTD_ANT2_PM_TX_DIG_LPBK_ENABLE             (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2E8)
#define	REG_B0_PHY_TXTD_ANT2_PM_TX_IQ_COMP_RES                 (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2EC)
#define	REG_B0_PHY_TXTD_ANT2_PM_TX_IQ_TPC                      (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2F0)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC1_COEF_0            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2F4)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC1_COEF_1            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2F8)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC2_COEF_0            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x2FC)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC2_COEF_1            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x300)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC3_COEF_0            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x304)
#define	REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC3_COEF_1            (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x308)
#define	REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_3RD_PHASE           (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x30C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_0                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x310)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_1                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x314)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_2                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x318)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_3                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x31C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_4                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x320)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_5                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x324)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_6                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x328)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_7                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x32C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_8                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x330)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_9                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x334)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_10                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x338)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_11                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x33C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_12                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x340)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_13                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x344)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_14                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x348)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_15                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x34C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_16                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x350)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_17                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x354)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_18                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x358)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_19                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x35C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_20                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x360)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_21                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x364)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_22                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x368)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_23                    (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x36C)
#define	REG_B0_PHY_TXTD_ANT2_TX_SPUR_CONTROL                   (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x370)
#define	REG_B0_PHY_TXTD_ANT2_TX_IQ_EQU_SAT                     (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x374)
#define	REG_B0_PHY_TXTD_ANT2_PM_LOOP_FORCE_TX_BANDEDGE         (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x378)
#define	REG_B0_PHY_TXTD_ANT2_PM_NCO_TX_MASTER                  (B0_PHY_TXTD_ANT2_BASE_ADDRESS + 0x37C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PHY_TXTD_ANT2_TX_MODE_11B_EN 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMode11BEn : 1; //Enable TX 11B Antenna , reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2TxMode11BEn_u;

/*REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_0 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAfeOffsetI0 : 12; //pm_afe_offset_i0, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 pmAfeOffsetQ0 : 12; //pm_afe_offset_q0, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB0PhyTxtdAnt2TxDcOffset0_u;

/*REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_1 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAfeOffsetQ1 : 12; //AFE DC Offset Q0, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 pmAfeOffsetI1 : 12; //AFE DC Offset I0, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB0PhyTxtdAnt2TxDcOffset1_u;

/*REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_2 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAfeOffsetQ2 : 12; //AFE DC Offset Q1, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 pmAfeOffsetI2 : 12; //AFE DC Offset I1, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB0PhyTxtdAnt2TxDcOffset2_u;

/*REG_B0_PHY_TXTD_ANT2_TX_DC_OFFSET_3 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAfeOffsetQ3 : 12; //AFE DC Offset Q2, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 pmAfeOffsetI3 : 12; //AFE DC Offset I2, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB0PhyTxtdAnt2TxDcOffset3_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_TEST_PLUG_MUX_CONTROL 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxtdTestPlugMuxControl : 3; //Tx TD Test Plug Mux Control, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdTestPlugMuxControl_u;

/*REG_B0_PHY_TXTD_ANT2_TX_ANT_RAM_RM 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxwinRamRm : 3; //txwin_ram_rm, reset value: 0x3, access type: RW
		uint32 pmDpdRamRm : 3; //dpd_ram_rm, reset value: 0x3, access type: RW
		uint32 pmRippleRamRm : 3; //ripple_ram_rm, reset value: 0x3, access type: RW
		uint32 pmIqRamRm : 3; //iq_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxtdAnt2TxAntRamRm_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_ANT_SPARE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxtdAntSpare : 32; //Spare register, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAntSpare_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_ANT_BYPASS_REG 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUpsBypass : 1; //UPS 640MHz Up Sampler Bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmUpsrfBypass : 1; //pm_upsrf_bypass, reset value: 0x0, access type: RW
		uint32 pmNoishBypass : 1; //pm_noish_bypass, reset value: 0x0, access type: RW
		uint32 reserved1 : 28;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAntBypassReg_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_WINDOWING 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxwinTailDis : 1; //Disable TX window last Roll Off, reset value: 0x0, access type: RW
		uint32 pmTxwinEn : 1; //pm_txwin_en, reset value: 0x1, access type: RW
		uint32 pmCddOffsetEn : 1; //pm_cdd_offset_en, reset value: 0x1, access type: RW
		uint32 pmTxwinCp4 : 3; //pm_txwin_cp_4, reset value: 0x1, access type: RW
		uint32 pmTxwinCp8Ss : 3; //pm_txwin_cp_8_ss, reset value: 0x1, access type: RW
		uint32 pmTxwinCp8Ls : 3; //pm_txwin_cp_8_ls, reset value: 0x1, access type: RW
		uint32 pmTxwinCp16 : 3; //pm_txwin_cp_16, reset value: 0x1, access type: RW
		uint32 pmTxwinCp32 : 3; //pm_txwin_cp_32, reset value: 0x1, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdWindowing_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_DUP_SWAP_FLIP 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxDuplicateI : 1; //Enable CDD offset, reset value: 0x0, access type: RW
		uint32 pmTxDuplicateQ : 1; //pm_tx_duplicate_q, reset value: 0x0, access type: RW
		uint32 pmTxFlipI : 1; //pm_tx_flip_i, reset value: 0x0, access type: RW
		uint32 pmTxFlipQ : 1; //pm_tx_flip_q, reset value: 0x0, access type: RW
		uint32 pmTxIqSwap : 1; //pm_tx_iq_swap, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdDupSwapFlip_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_MIX11B_FREQ_OFFSET 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMix11BFreqOffset : 3; //11B Mixer Frequency shift: , 0: 0 , 1: +10MHz , 2: +30MHz , 3: -10MHz , 4: -30MHz, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdMix11BFreqOffset_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_0 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff0 : 24; //UPS8 coeff 0 of 8 x 24 bit , reset value: 0x34697F, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff0_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_1 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff1 : 24; //UPS8 coeff 1 of 8 x 24 bit , reset value: 0xB4719D, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff1_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_2 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff2 : 24; //UPS8 coeff 2 of 8 x 24 bit , reset value: 0x4F66FD, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff2_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_3 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff3 : 24; //UPS8 coeff 3 of 8 x 24 bit , reset value: 0xC4218D, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff3_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_4 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff4 : 24; //UPS8 coeff 4 of 8 x 24 bit , reset value: 0x6BE5FB, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff4_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_5 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff5 : 24; //UPS8 coeff 5 of 8 x 24 bit , reset value: 0xD5E36E, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff5_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_6 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff6 : 24; //UPS8 coeff 6 of 8 x 24 bit , reset value: 0x886877, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff6_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS8_COEFF_7 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUps8Coeff7 : 24; //UPS8 coeff 7 of 8 x 24 bit , reset value: 0xE3B943, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUps8Coeff7_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_MEM_GLOBAL_RM 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMemGlobalRm : 2; //Global RM value for rams, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdMemGlobalRm_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_MEM_GLOBAL_TEST_MODE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMemGlobalTestMode : 2; //Global test mode for rams, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdMemGlobalTestMode_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_SPARE 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxtdSpare : 32; //Spare register, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2TxtdSpare_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_1ST_PHASE 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdAfeIdle1StPhaseI : 13; //AFE idle I data, reset value: 0x0, access type: RW
		uint32 txtdAfeIdle1StPhaseQ : 13; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAfeIdle1StPhase_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_2ND_PHASE 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdAfeIdle2NdPhaseI : 13; //AFE idle Q data, reset value: 0x0, access type: RW
		uint32 txtdAfeIdle2NdPhaseQ : 13; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAfeIdle2NdPhase_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_BIST 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSwBistStart : 1; //sw_bist_start, reset value: 0x0, access type: RW
		uint32 pmClearRamMode : 1; //clear_ram_mode, reset value: 0x0, access type: RW
		uint32 pmBistScrBypass : 1; //bist_scr_bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdBist_u;

/*REG_B0_PHY_TXTD_ANT2_TX_IQ_RIPPLE 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmIqEquBypass : 1; //iq_equ_bypass, reset value: 0x0, access type: RW
		uint32 pmRippleEquBypass : 1; //ripple_equ_bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxIqRipple_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_IFFT_IN_OUT_SHIFT 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmIfftInShift20 : 2; //ifft shift in for bw 20, reset value: 0x2, access type: RW
		uint32 pmIfftInShift40 : 2; //ifft shift in for bw 40, reset value: 0x1, access type: RW
		uint32 pmIfftInShift80 : 2; //ifft shift in for bw 80, reset value: 0x0, access type: RW
		uint32 pmIfftInShift160 : 2; //ifft shift in for bw 160, reset value: 0x0, access type: RW
		uint32 pmIfftOutShift20 : 3; //ifft shift out for bw 20, reset value: 0x0, access type: RW
		uint32 pmIfftOutShift40 : 3; //ifft shift out for bw 40, reset value: 0x0, access type: RW
		uint32 pmIfftOutShift80 : 3; //ifft shift out for bw 80, reset value: 0x1, access type: RW
		uint32 pmIfftOutShift160 : 3; //ifft shift out for bw 160, reset value: 0x1, access type: RW
		uint32 pmIfftInShiftToneGen : 2; //pm_ifft_in_shift_tone_gen, reset value: 0x2, access type: RW
		uint32 pmIfftOutShiftToneGen : 3; //pm_ifft_out_shift_tone_gen, reset value: 0x6, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdIfftInOutShift_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_END_ZEROS 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEndZeros : 8; //pm_end_zeros , reset value: 0x96, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdEndZeros_u;

/*REG_B0_PHY_TXTD_ANT2_TX_ABB_30 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSc2FcPhaseRegfile : 2; //tx_sc2fc_phase_regfile, reset value: 0x0, access type: RW
		uint32 txSc2FcTxData : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxtdAnt2TxAbb30_u;

/*REG_B0_PHY_TXTD_ANT2_TX_ANT_REG_31 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txRippleInSsbMode : 1; //tx_ripple_in_ssb_mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 txUseSecRippleInHbMode : 1; //tx_use_sec_ripple_in_hb_mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegB0PhyTxtdAnt2TxAntReg31_u;

/*REG_B0_PHY_TXTD_ANT2_TX_BF_CAL_GAIN_INDEX_ANT 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfCalGainIndexAnt : 3; //bf_cal_gain_index_ant, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 bfCalRm : 3; //bf_cal_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB0PhyTxtdAnt2TxBfCalGainIndexAnt_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_ANT_RAM 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpcRamRm : 3; //tpc_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxtdAnt2TpcAntRam_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_ACCELERATOR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpcaccenable : 1; //tpcaccenable, reset value: 0x0, access type: RW
		uint32 txTpcAccDone : 1; //tx_tpc_acc_done, reset value: 0x0, access type: RO
		uint32 pmTxPgc2GainSelect : 2; //pm_tx_pgc2_gain_select, reset value: 0x0, access type: RW
		uint32 tpcBoostEnMask : 1; //tpc_boost_en_mask, reset value: 0x0, access type: RW
		uint32 txDigitalGainOut : 10; //tx_digital_gain_out, reset value: 0x0, access type: RO
		uint32 pmTxPgc2GainOverride : 1; //pm_tx_pgc2_gain_override, reset value: 0x0, access type: RW
		uint32 tpcGainIndexAnt : 8; //tpc_gain_index_ant, reset value: 0x0, access type: RO
		uint32 phyTxPowerAntLimit : 7; //phy_tx_power_ant_limit, reset value: 0x0, access type: RO
		uint32 tssiOpenLoopEn : 1; //tssi_open_loop_en, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2TpcAccelerator_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_TSSI_A_B 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTssiA : 16; //tx_tssi_A, reset value: 0x0, access type: RO
		uint32 txTssiB : 16; //tx_tssi_B, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2TpcTssiAB_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_TSSI_C 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTssiC : 16; //tx_tssi_C, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxtdAnt2TpcTssiC_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_SHORT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPgcGain : 2; //tx_pgc_gain, reset value: 0x0, access type: RO
		uint32 txPaDrv : 4; //tx_pa_drv, reset value: 0x0, access type: RO
		uint32 txS2DIndex : 4; //tx_S2D_index, reset value: 0x0, access type: RO
		uint32 txFectl : 5; //tx_fectl, reset value: 0x0, access type: RW
		uint32 txRfOn : 1; //tx_rf_on, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxtdAnt2TpcShort_u;

/*REG_B0_PHY_TXTD_ANT2_RF_POWER 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrRfPower : 8; //tcr_rf_power, reset value: 0x0, access type: RO
		uint32 tcrPhyPowerBoost : 2; //tcr_phy_power_boost, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 phyTxPowerAntBoost : 9; //tx_power from tcr + boost factor, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegB0PhyTxtdAnt2RfPower_u;

/*REG_B0_PHY_TXTD_ANT2_TX_DIGITAL_GAIN 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxDigitalGain : 10; //pm_tx_digital_gain, reset value: 0x143, access type: RW
		uint32 pmTxGainEn : 1; //pm_tx_gain_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxtdAnt2TxDigitalGain_u;

/*REG_B0_PHY_TXTD_ANT2_TX_PGC2_GAIN_SELECT_OUT 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPgc2GainSelectOut : 2; //tx_pgc2_gain_select_out, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxPgc2GainSelectOut_u;

/*REG_B0_PHY_TXTD_ANT2_RF_POWER_OFFSET 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfPowerOffset20 : 7; //rf_power_offset_20, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rfPowerOffset40 : 7; //rf_power_offset_40, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 rfPowerOffset80 : 7; //rf_power_offset_80, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 rfPowerOffset16011B : 7; //rf_power_offset_160_11b, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB0PhyTxtdAnt2RfPowerOffset_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_IQ_0 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF1 : 12; //pm_tx_iq_td_f1, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF2 : 12; //pm_tx_iq_td_f2, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdIq0_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_IQ_1 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF3 : 12; //pm_tx_iq_td_f4, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF4 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdIq1_u;

/*REG_B0_PHY_TXTD_ANT2_STREAMER 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 streamerEnable : 1; //streamer_enable, reset value: 0x0, access type: RW
		uint32 streamerRate : 1; //0 = 640MHz , 1 = 320MHz, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2Streamer_u;

/*REG_B0_PHY_TXTD_ANT2_TEST_PLUG_LEN 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugLen : 6; //test_plug_len, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxtdAnt2TestPlugLen_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS_COEFF_0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUpsCoef0 : 7; //UPS 640Msps coeff 0  , reset value: 0x43, access type: RW
		uint32 reserved0 : 9;
		uint32 pmUpsCoef1 : 10; //UPS 640Msps coeff 1  , reset value: 0x151, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUpsCoeff0_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPS_COEFF_2 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUpsCoef2 : 12; //UPS 640Msps coeff 2  , reset value: 0xB5F, access type: RW
		uint32 reserved0 : 4;
		uint32 pmUpsCoef3 : 13; //UPS 640Msps coeff 3  , reset value: 0x138B, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUpsCoeff2_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_UPSRF_COEFF_0 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmUpsrfCoef0 : 2; //UPSRF 1280Msps coeff 0  , reset value: 0x2, access type: RW
		uint32 reserved0 : 14;
		uint32 pmUpsrfCoef1 : 4; //UPSRF 1280Msps coeff 1  , reset value: 0x9, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdUpsrfCoeff0_u;

/*REG_B0_PHY_TXTD_ANT2_PM_ANALOG_LPBK_EN 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAnalogLpbkEn : 2; //0-no loop, 1-loop same ant, 2-loop from 5th ant (relevant only to ant0), reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2PmAnalogLpbkEn_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_GCLK_EN 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdPmGclkEn : 6; //txtd_pm_gclk_en, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdGclkEn_u;

/*REG_B0_PHY_TXTD_ANT2_IDLE_MODE_EN 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIdleMode : 1; //pm_tone_gen_bw, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2IdleModeEn_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SC2FC_TXON 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSc2FcTxon : 2; //tx_sc2fc_txon, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxSc2FcTxon_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_BYPASS_SEL 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxtdBypassSel : 1; //pm_txtd_bypass_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdBypassSel_u;

/*REG_B0_PHY_TXTD_ANT2_TX_PPM_FREQ_IN_CFG 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPpmFreqInCfg : 20; //ppm frequency offset calculated in last reception. This value is read by genrisc from RXTD and written here. It used for NCO phase address calculation., reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB0PhyTxtdAnt2TxPpmFreqInCfg_u;

/*REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_UP_LIM_CFG 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPmFdlNcoUpLimCfg : 30; //nco counter uper limit initial value used when PPM >0, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB0PhyTxtdAnt2TxPmFdlNcoUpLimCfg_u;

/*REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_INCR_MAX_CFG 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPmFdlNcoIncrMaxCfg : 32; //fdl nco incr max value. Above it saturation bit is asserted, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2TxPmFdlNcoIncrMaxCfg_u;

/*REG_B0_PHY_TXTD_ANT2_TX_PM_FDL_NCO_INCR_CFG 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPmFdlNcoIncrCfg : 32; //fdl nco incr value. Calculated in last reception. Genrisc read this value from Rx and writes it here., reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2TxPmFdlNcoIncrCfg_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_CFG 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdCfgSelDpdApi : 6; //no description, reset value: 0x0, access type: RW
		uint32 dpdCfgPhyCtl : 1; //no description, reset value: 0x0, access type: RW
		uint32 dpdCfgGlobalEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 dpdCfgEqEnable : 1; //no description, reset value: 0x0, access type: RW
		uint32 dpdCfgEqSelTap : 1; //no description, reset value: 0x0, access type: RW
		uint32 dpdCfgEqSelMem : 3; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegB0PhyTxtdAnt2DpdCfg_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_STATUS 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdStatusDbgRdy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2DpdStatus_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_CTL 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgCtlRun : 1; //no description, reset value: 0x0, access type: WO
		uint32 dpdDbgCtlCmd : 3; //no description, reset value: 0x0, access type: RW
		uint32 dpdDbgCtlArgs : 28; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgCtl_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_0 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgRes0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgRes0_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_1 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgRes1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgRes1_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_2 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgRes2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgRes2_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_3 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgRes3 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgRes3_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_DBG_RES_4 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdDbgRes4 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2DpdDbgRes4_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_0 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI0 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ0 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA0_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_1 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA1_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_2 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA2_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_3 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA3_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_4 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI4 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ4 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA4_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_5 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI5 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ5 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA5_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_6 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI6 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ6 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA6_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_7 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI7 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ7 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA7_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_A_8 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqAI8 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqAQ8 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqA8_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_0 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI0 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ0 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB0_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_1 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB1_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_2 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB2_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_3 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB3_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_4 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI4 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ4 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB4_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_5 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI5 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ5 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB5_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_6 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI6 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ6 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB6_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_7 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI7 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ7 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB7_u;

/*REG_B0_PHY_TXTD_ANT2_DPD_MEM_EQ_B_8 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdMemEqBI8 : 12; //no description, reset value: 0x0, access type: RW
		uint32 dpdMemEqBQ8 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2DpdMemEqB8_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_ANT_RAM_POWER_SAVE 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRamLs : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
		uint32 pmRamPsHwEn : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAntRamPowerSave_u;

/*REG_B0_PHY_TXTD_ANT2_TPC_DPD 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdSelIndex : 8; //dpd_sel_index -  , bit 7:6 is tx BW from TCR , bits 5:2 tx_pa_drv from tpc table , bits 1:0 tx pgc gain select. , , reset value: 0x0, access type: RO
		uint32 dpdSelPhy : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyTxtdAnt2TpcDpd_u;

/*REG_B0_PHY_TXTD_ANT2_CLIPPER_CONF 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmPreambleClipperThreshold : 10; //When HIGH clipper is bypasseed, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmPreambleClipperThresholdForce : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 pmDataClipperThreshold : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmDataClipperThresholdForce : 1; //no description, reset value: 0x0, access type: RW
		uint32 clipperBypass : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegB0PhyTxtdAnt2ClipperConf_u;

/*REG_B0_PHY_TXTD_ANT2_CLIPPER_THRESHOLD 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preambleClipperThreshold : 10; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dataClipperThreshold : 10; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 txClipperThresholdIndex : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2ClipperThreshold_u;

/*REG_B0_PHY_TXTD_ANT2_LOOPBACK_PYPASS 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLoopBandedgeBypass : 1; //pm_loop_bandedge_bypass, reset value: 0x0, access type: RW
		uint32 pmLoopFdlBypass : 1; //pm_loop_fdl_bypass, reset value: 0x0, access type: RW
		uint32 pmLoopForceRxPrefilter : 1; //pm_loop_force_rx_prefilter, reset value: 0x0, access type: RW
		uint32 pmLoopForceRxFdl : 1; //pm_loop_force_rx_fdl, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxtdAnt2LoopbackPypass_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_GCLK_BYPASS 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdGclkBypassEn : 13; //txtd_gclk_bypass_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdGclkBypass_u;

/*REG_B0_PHY_TXTD_ANT2_PM_COMB_BYPASS 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCombBypass : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2PmCombBypass_u;

/*REG_B0_PHY_TXTD_ANT2_STAT_COMB_VALID_MISMATCH 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statCombValidMismatch : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2StatCombValidMismatch_u;

/*REG_B0_PHY_TXTD_ANT2_STAT_COMB_OUT_SAT 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statCombOutSat : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxtdAnt2StatCombOutSat_u;

/*REG_B0_PHY_TXTD_ANT2_STAT_COMB_VALID_MISMATCH_CLEAR 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statCombValidMismatchClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2StatCombValidMismatchClear_u;

/*REG_B0_PHY_TXTD_ANT2_STAT_COMB_OUT_SAT_CLEAR 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statCombOutSatClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2StatCombOutSatClear_u;

/*REG_B0_PHY_TXTD_ANT2_PM_CNF_BYPASS 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCnfBypass : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2PmCnfBypass_u;

/*REG_B0_PHY_TXTD_ANT2_PM_CNF_AUTOMATIC_BYPASS_BY_BW 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCnfAutomaticBypassByBw : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxtdAnt2PmCnfAutomaticBypassByBw_u;

/*REG_B0_PHY_TXTD_ANT2_PM_SYMBOL_TYPE_CLIPPER_LAT 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSymbolTypeLatencyConfig : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PhyTxtdAnt2PmSymbolTypeClipperLat_u;

/*REG_B0_PHY_TXTD_ANT2_RF_POWER_OFFSET_320 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfPowerOffset320 : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB0PhyTxtdAnt2RfPowerOffset320_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TX_DIG_LPBK_ENABLE 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxDigLpbkEnable : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxDigLpbkEnable_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TX_IQ_COMP_RES 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqCompRes : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxIqCompRes_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TX_IQ_TPC 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTpcEnable : 2; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTpcIndex : 2; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTpcIndexFromProgmodel : 1; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdTpcIndexFromProgmodel : 1; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdTpcIndex : 2; //no description, reset value: 0x0, access type: RW
		uint32 pmTxCalTpcIndexFromProgmodel : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxIqTpc_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC1_COEF_0 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF1Tpc1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF2Tpc1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc1Coef0_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC1_COEF_1 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF3Tpc1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF4Tpc1 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc1Coef1_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC2_COEF_0 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF1Tpc2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF2Tpc2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc2Coef0_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC2_COEF_1 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF3Tpc2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF4Tpc2 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc2Coef1_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC3_COEF_0 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF1Tpc3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF2Tpc3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc3Coef0_u;

/*REG_B0_PHY_TXTD_ANT2_PM_TXTD_IQ_TPC3_COEF_1 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTxIqTdF3Tpc3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 pmTxIqTdF4Tpc3 : 12; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PhyTxtdAnt2PmTxtdIqTpc3Coef1_u;

/*REG_B0_PHY_TXTD_ANT2_TXTD_AFE_IDLE_3RD_PHASE 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txtdAfeIdle3RdPhaseI : 13; //no description, reset value: 0x0, access type: RW
		uint32 txtdAfeIdle3RdPhaseQ : 13; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxtdAfeIdle3RdPhase_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_0 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLutEnable : 1; //no description, reset value: 0x0, access type: RW
		uint32 txSpurLut0 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut0_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_1 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut1 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut1_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_2 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut2 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut2_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_3 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut3 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut3_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_4 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut4 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut4_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_5 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut5 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut5_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_6 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut6 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut6_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_7 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut7 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut7_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_8 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut8 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut8_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_9 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut9 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut9_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_10 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut10 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut10_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_11 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut11 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut11_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_12 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut12 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut12_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_13 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut13 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut13_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_14 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut14 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut14_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_15 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut15 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut15_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_16 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut16 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut16_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_17 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut17 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut17_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_18 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut18 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut18_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_19 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut19 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut19_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_20 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut20 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut20_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_21 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut21 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut21_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_22 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut22 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut22_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_LUT_23 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLut23 : 26; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurLut23_u;

/*REG_B0_PHY_TXTD_ANT2_TX_SPUR_CONTROL 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSpurLutBypass : 1; //no description, reset value: 0x1, access type: RW
		uint32 txSpurLutWaitForFirstValid : 1; //no description, reset value: 0x1, access type: RW
		uint32 txSpurLutTestCounterInit : 3; //no description, reset value: 0x0, access type: RW
		uint32 txSpurLutTestCounterEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 txSpurLutSamplingPhase : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxtdAnt2TxSpurControl_u;

/*REG_B0_PHY_TXTD_ANT2_TX_IQ_EQU_SAT 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 iqFdISat : 1; //no description, reset value: 0x0, access type: RO
		uint32 iqFdQSat : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxtdAnt2TxIqEquSat_u;

/*REG_B0_PHY_TXTD_ANT2_PM_LOOP_FORCE_TX_BANDEDGE 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLoopForceTxBandedge : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2PmLoopForceTxBandedge_u;

/*REG_B0_PHY_TXTD_ANT2_PM_NCO_TX_MASTER 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmNcoTxMaster : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxtdAnt2PmNcoTxMaster_u;



#endif // _B0_PHY_TXTD_ANT2_REGS_H_
