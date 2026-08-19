
/***********************************************************************************
File:				B1DfsPhyAgcAntRegs.h
Module:				b1DfsPhyAgcAnt
SOC Revision:		latest
Generated at:       2022-05-04 10:57:34
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_DFS_PHY_AGC_ANT_REGS_H_
#define _B1_DFS_PHY_AGC_ANT_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_DFS_PHY_AGC_ANT_BASE_ADDRESS                             MEMORY_MAP_UNIT_10215_BASE_ADDRESS
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_PARAMS_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x0)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_OFFSET2_REG                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SYSTEM_GAIN_REG                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x8)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SYSTEM_GAIN_SELECT_REG           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xC)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SAT_THR                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x10)
#define	REG_B1_DFS_PHY_AGC_ANT_INB_RSSI_PARAMS_REG                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x14)
#define	REG_B1_DFS_PHY_AGC_ANT_INB_RSSI_OFFSET2_REG                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x18)
#define	REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SYSTEM_GAIN_REG              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1C)
#define	REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x20)
#define	REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SAT_THR                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x24)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_PARAMS_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x28)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_OFFSET2_REG                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2C)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SYSTEM_GAIN_REG                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x30)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SYSTEM_GAIN_SELECT_REG           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x34)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SAT_THR                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x38)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_PARAMS_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3C)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_OFFSET2_REG                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x40)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SYSTEM_GAIN_REG                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x44)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SYSTEM_GAIN_SELECT_REG           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x48)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SAT_THR                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_PARAMS_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x50)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_RX_RANGE0                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x54)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_RX_RANGE1                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x58)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_TX                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x5C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET2_RX_REG                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x60)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET2_TX_REG                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x64)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SYSTEM_GAIN_REG                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x68)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SYSTEM_GAIN_SELECT_REG           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x6C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SAT_THR_RANGE0                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x70)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SAT_THR_RANGE1                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x74)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A_RANGE0_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x78)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A_RANGE1_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x7C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A2_RANGE0_REG               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x80)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A2_RANGE1_REG               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x84)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_B_RANGE0_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x88)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_B_RANGE1_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x8C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_C_RANGE0_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x90)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_C_RANGE1_REG                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x94)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_NLOG_RANGE0_REG             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x98)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_NLOG_RANGE1_REG             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x9C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_A_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xA0)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_A2_REG                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xA4)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_B_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xA8)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_C_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xAC)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_NLOG_REG                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xB0)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_TX_RX_REG                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xB4)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_REG                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xB8)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_POWER_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xBC)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_SRSSI_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xC0)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_SPOWER_REG                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xC4)
#define	REG_B1_DFS_PHY_AGC_ANT_BB_PWR_DIFF_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xC8)
#define	REG_B1_DFS_PHY_AGC_ANT_IB_RSSI_REG                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xCC)
#define	REG_B1_DFS_PHY_AGC_ANT_IB_POWER_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xD0)
#define	REG_B1_DFS_PHY_AGC_ANT_IB_SRSSI_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xD4)
#define	REG_B1_DFS_PHY_AGC_ANT_IB_SPOWER_REG                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xD8)
#define	REG_B1_DFS_PHY_AGC_ANT_IB_PWR_DIFF_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xDC)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_REG                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xE0)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_POWER_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xE4)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_SRSSI_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xE8)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_SPOWER_REG                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xEC)
#define	REG_B1_DFS_PHY_AGC_ANT_FB_PWR_DIFF_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xF0)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_REG                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xF4)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_POWER_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xF8)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_SRSSI_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0xFC)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_SPOWER_REG                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x100)
#define	REG_B1_DFS_PHY_AGC_ANT_HB_PWR_DIFF_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x104)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_REG                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x108)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_POWER_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x10C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_SRSSI_REG                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x110)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_SPOWER_REG                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x114)
#define	REG_B1_DFS_PHY_AGC_ANT_SATURATION_RESULT                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x118)
#define	REG_B1_DFS_PHY_AGC_ANT_RSSI_BLOCKS_GCLK_EN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x11C)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_DEC_OUT_STS                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x120)
#define	REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_HIGH_RES_REG                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x124)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_RF_POWER                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x140)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_FB_POWER                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x144)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x148)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_HB_POWER                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x14C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x150)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACI_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x154)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_RF_OFFSET                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x158)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_FB_OFFSET                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x15C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_OFFSET                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x160)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_HB_OFFSET                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x164)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_OFFSET                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x168)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACI_OFFSET                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x16C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_SATURATION_REG                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x170)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_DELAY                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x174)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SOURCES                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x178)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_MAX_POWER_LEVEL              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x17C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_MIN_POWER_LEVEL              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x180)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SIGNAL_MAX_LEVEL             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x184)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_ENV_MIN_LEVEL                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x188)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SIGNAL_ENV_DIFF_VALUES       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x18C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_GSP_TABLE_CONTROL            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x190)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_LNA_CONTROL                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x194)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_SEL_S2D_RX_LUT                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1B4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_OFFSET                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1B8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_LIMITS                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1BC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_SHIFT                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1C0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_RX_FILTER_CONTROL                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1C4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_RF_SYSTEM_GAIN_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1C8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_FB_SYSTEM_GAIN_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1CC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_SYSTEM_GAIN_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1D0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_POST_BB_SYSTEM_GAIN_OFFSET            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1D4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_HB_SYSTEM_GAIN_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1D8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_SYSTEM_GAIN_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1DC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACI_SYSTEM_GAIN_OFFSET                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1E0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_CCA_SYSTEM_GAIN_OFFSET                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1E4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_DET_SYSTEM_GAIN_OFFSET                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1E8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_BOF_OFFSET                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1EC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_HB_BOF_OFFSET                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1F0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_RSSI_BOF_OFFSET                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1F4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_ACI_BOF_OFFSET                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1F8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_DET_BOF_OFFSET                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x1FC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_BOF_TARGET                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x200)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_HB_BOF_TARGET                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x204)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_BOF_TARGET                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x208)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_DET_BOF_TARGET                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x20C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_DIFI1_DB_GAIN                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x210)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PRESET_LNA_PGC1                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x214)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_BB_DIGITAL_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x218)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_BB_DIGITAL_GAIN                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x21C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_BB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x220)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_BB_DIGITAL_GAIN      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x224)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_HB_DIGITAL_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x228)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_HB_DIGITAL_GAIN                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x22C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_HB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x230)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_HB_DIGITAL_GAIN      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x234)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_IB_DIGITAL_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x238)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_IB_DIGITAL_GAIN                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x23C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_IB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x240)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_IB_DIGITAL_GAIN      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x244)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_DET_DIGITAL_GAIN                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x248)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_DET_DIGITAL_GAIN                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x24C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_DET_DIGITAL_GAIN        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x250)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_DET_DIGITAL_GAIN     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x254)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_RF_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x258)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_FB_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x25C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x260)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x264)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x268)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_ACI_POWER                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x26C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_SIGNAL_POWER                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x270)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_ENV_POWER                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x274)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_STATUS                               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x278)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_RF_SYSTEM_GAIN                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x27C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_FB_SYSTEM_GAIN                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x280)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_SYSTEM_GAIN                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x284)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_POST_BB_SYSTEM_GAIN                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x288)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_SYSTEM_GAIN                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x28C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_SYSTEM_GAIN                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x290)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_ACI_SYSTEM_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x294)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_CCA_SYSTEM_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x298)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DET_SYSTEM_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x29C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_DIGITAL_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2A0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_DIGITAL_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2A4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_DIGITAL_GAIN                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2A8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DET_DIGITAL_GAIN                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2AC)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_BOF                               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2B0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_TOTAL_DIGITAL_GAIN                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2B4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_DIGITAL_BOF                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2B8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_BOF                               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2BC)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_TOTAL_DIGITAL_GAIN                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2C0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_HB_DIGITAL_BOF                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2C4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_BOF                               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2C8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_TOTAL_DIGITAL_GAIN                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2CC)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_DIGITAL_BOF                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2D0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DET_BOF                              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2D4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DET_TOTAL_DIGITAL_GAIN               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2D8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DET_DIGITAL_BOF                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2DC)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DC_OFFSET_I                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2E0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_DC_OFFSET_Q                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2E4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_AGC_RAMS_RM                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2E8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_GAIN_VALUES                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2EC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_FCSI_CONTROL                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2F0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_FCSI_VALUES                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2F8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_GAIN_ACC_CONTROL                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x2FC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_CODE_ACC_THR                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x300)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_CODE_ACC_THR                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x304)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LONGTERM_RELEASE_THR                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x308)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LONGTERM_THR                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x30C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_FAST_LOW_THR                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x310)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_DELTA_CODE_ACC_THR                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x314)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_UP_THR                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x318)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_SLOW_LOW_THR                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x31C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_UP_THR                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x320)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_SLOW_LOW_THR                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x324)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_SET_CODE_ACC_THR_CTRL                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x328)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ALGO_CTRL                             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x32C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_EVENT_STATUS                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x330)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_SET_INC_GAP_DB                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x334)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_SET_DEC_GAP_DB                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x338)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_UP_EXTRA_GAP_DB                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x33C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_UP_DEF_THR                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x340)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_SLOW_LOW_MIN_THR             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x344)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_SET_INC_GAP_DB                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x348)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_SET_DEC_GAP_DB                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x34C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_UP_EXTRA_GAP_DB                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x350)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_UP_DEF_THR                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x354)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_SLOW_LOW_MIN_THR             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x358)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER_UP_THR                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x35C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER_SLOW_LOW_THR                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x360)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER_UP_THR                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x364)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER_SLOW_LOW_THR                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x368)
#define	REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_GAIN_WORD                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x36C)
#define	REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_STM_CLEAR_CONTROL               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x370)
#define	REG_B1_DFS_PHY_AGC_ANT_CALC_ACC_AUTO_CLOSE_MODE                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x374)
#define	REG_B1_DFS_PHY_AGC_ANT_CALC_POWER_THR_ACC_GO                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x378)
#define	REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_GAIN_CANCEL                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x37C)
#define	REG_B1_DFS_PHY_AGC_ANT_DISABLE_PERIPHERALS                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x380)
#define	REG_B1_DFS_PHY_AGC_ANT_AGC_STM_RDBACK                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x384)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_BB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x388)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_BB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x38C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_HB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x390)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_HB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x394)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_IB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x398)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_IB_DIGITAL_GAIN         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x39C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_DET_DIGITAL_GAIN        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3A0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_DET_DIGITAL_GAIN        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3A4)
#define	REG_B1_DFS_PHY_AGC_ANT_BLOCKER_CONTROL                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3C0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BLOCKER_TH                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3C4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ALGO_CTRL_SEL                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3D0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_BLOCKER_ADDR                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x3D4)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_INB_RSSI_PARAMS_REG                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x400)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_INB_RSSI_OFFSET2_REG                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x404)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SYSTEM_GAIN_REG           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x408)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x40C)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SAT_THR                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x410)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_PARAMS_REG                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x414)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_OFFSET2_REG                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x418)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SYSTEM_GAIN_REG               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x41C)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SYSTEM_GAIN_SELECT_REG        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x420)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SAT_THR                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x424)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_IB_RSSI_REG                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x428)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_IB_POWER_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x42C)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_IB_SRSSI_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x430)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_IB_SPOWER_REG                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x434)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_IB_PWR_DIFF_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x438)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_REG                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x43C)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_POWER_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x440)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_SRSSI_REG                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x444)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_SPOWER_REG                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x448)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_HB_PWR_DIFF_REG                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x44C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_POWER                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x450)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_POWER                           (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x454)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_ACI_POWER                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x458)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_POWER                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x45C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_POWER                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x460)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_ACI_POWER                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x464)
#define	REG_B1_DFS_PHY_AGC_ANT_UP_CONTROL                               (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x468)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_SYSTEM_GAIN_OFFSET              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x46C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_SYSTEM_GAIN_OFFSET              (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x470)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_ACI_SYSTEM_GAIN_OFFSET             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x474)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_CCA_SYSTEM_GAIN_OFFSET             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x478)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_BOF_OFFSET                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x47C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_RSSI_BOF_OFFSET                 (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x480)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_ACI_BOF_OFFSET                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x484)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_BOF_TARGET                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x488)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_BOF_TARGET                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x48C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_SYSTEM_GAIN                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x490)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_SYSTEM_GAIN                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x494)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_ACI_SYSTEM_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x498)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_CCA_SYSTEM_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x49C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_DIGITAL_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4A0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_DIGITAL_GAIN                   (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4A4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_BOF                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4A8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_TOTAL_DIGITAL_GAIN             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4AC)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_DIGITAL_BOF                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4B0)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_BOF                            (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4B4)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_TOTAL_DIGITAL_GAIN             (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4B8)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_DIGITAL_BOF                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4BC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SAT_SOURCES                  (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4C0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_0                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4C4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_1                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4C8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_2                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4CC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_3                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4D0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_4                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4D4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_5                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4D8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_6                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4DC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_7                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4E0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_8                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4E4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_9                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4E8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_10                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4EC)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_11                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4F0)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_12                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4F4)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_13                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x4F8)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B0_B4                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x500)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B5_B7                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x504)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B8_B11                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x508)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B0_B4                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x50C)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B5_B7                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x510)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B8_B11                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x514)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS0_B4                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x518)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS4_B7                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x51C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS8_B11                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x520)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_ABS_BLOCKER_MAP                      (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x524)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_TH_BLOCKER_MAP                       (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x528)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_MAX_BLOCKER_LSB_POWER                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x52C)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_MAX_BLOCKER_USB_POWER                (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x530)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_LNA_INTERNAL_GAIN                     (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x534)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PRI_ACI_POWER                         (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x538)
#define	REG_B1_DFS_PHY_AGC_ANT_ACC_PRI_ACI_POWER                        (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x53C)
#define	REG_B1_DFS_PHY_AGC_ANT_ULTRA_WIDE_BAND                          (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x540)
#define	REG_B1_DFS_PHY_AGC_ANT_PM_PRESET_CALIBRATION                    (B1_DFS_PHY_AGC_ANT_BASE_ADDRESS + 0x544)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_PARAMS_REG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbRssiBlkSize : 3; //bb rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 bbRssiIirPole : 4; //bb rssi iir pole, reset value: 0x0, access type: RW
		uint32 bbRssiIirPoleMin : 3; //bb rssi iir shift, reset value: 0x0, access type: RW
		uint32 bbRssiOffset1 : 16; //bb rssi offset1, reset value: 0x0, access type: RW
		uint32 bbRssiCancelDc : 1; //bb rssi cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_OFFSET2_REG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiOffset2 : 9; //bb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SYSTEM_GAIN_REG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiSystemGain : 9; //bb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SYSTEM_GAIN_SELECT_REG 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbRssiSystemGainHwSelect : 1; //bb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_SAT_THR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiSatThr : 9; //bb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_INB_RSSI_PARAMS_REG 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 inbandRssiBlkSize : 3; //inband rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 inbandRssiIirPole : 4; //inband rssi iir pole, reset value: 0x0, access type: RW
		uint32 inbandRssiIirPoleMin : 3; //inband rssi iir shift, reset value: 0x0, access type: RW
		uint32 inbandRssiOffset1 : 16; //inband rssi offset1, reset value: 0x0, access type: RW
		uint32 inbandRssiCancelDc : 1; //inband cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntInbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_INB_RSSI_OFFSET2_REG 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiOffset2 : 9; //inband rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntInbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SYSTEM_GAIN_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiSystemGain : 9; //inband rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntInbandRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 inbandRssiSystemGainHwSelect : 1; //inband rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntInbandRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_INBAND_RSSI_SAT_THR 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiSatThr : 9; //inband rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntInbandRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_PARAMS_REG 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fbRssiBlkSize : 3; //full band rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 fbRssiIirPole : 4; //full band rssi iir pole, reset value: 0x0, access type: RW
		uint32 fbRssiIirPoleMin : 3; //full band rssi iir shift, reset value: 0x0, access type: RW
		uint32 fbRssiOffset1 : 16; //full band rssi offset1, reset value: 0x0, access type: RW
		uint32 fbRssiCancelDc : 1; //full band cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_OFFSET2_REG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiOffset2 : 9; //full band rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SYSTEM_GAIN_REG 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiSystemGain : 9; //full band rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SYSTEM_GAIN_SELECT_REG 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fbRssiSystemGainHwSelect : 1; //full band rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_SAT_THR 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiSatThr : 9; //full band rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_PARAMS_REG 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbRssiBlkSize : 3; //hb rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 hbRssiIirPole : 4; //hb rssi iir pole, reset value: 0x0, access type: RW
		uint32 hbRssiIirPoleMin : 3; //hb rssi iir shift, reset value: 0x0, access type: RW
		uint32 hbRssiOffset1 : 16; //hb rssi offset1, reset value: 0x0, access type: RW
		uint32 hbRssiCancelDc : 1; //hb rssi cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_OFFSET2_REG 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiOffset2 : 9; //hb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SYSTEM_GAIN_REG 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiSystemGain : 9; //hb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SYSTEM_GAIN_SELECT_REG 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbRssiSystemGainHwSelect : 1; //hb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_SAT_THR 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiSatThr : 9; //hb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_PARAMS_REG 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiBlkSizeRxMode : 3; //rf rssi decimatiion, 2^(block size), in RX mode, reset value: 0x0, access type: RW
		uint32 rfRssiBlkSizeTxMode : 3; //rf rssi decimatiion, 2^(block size), in TX mode, reset value: 0x0, access type: RW
		uint32 rfRssiBlkSizeUserMode : 3; //rf rssi decimatiion, 2^(block size), in user mode (override TX/RX), reset value: 0x0, access type: RW
		uint32 rfRssiIirPole : 4; //rf rssi iir pole, reset value: 0x0, access type: RW
		uint32 rfRssiIirPoleMin : 3; //rf rssi iir shift, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_RX_RANGE0 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1RxRange0 : 14; //rf rssi offset1, used for dc cancellation in rx range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiOffset1RxRange0_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_RX_RANGE1 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1RxRange1 : 14; //rf rssi offset1, used for dc cancellation in rx range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiOffset1RxRange1_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET1_TX 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1Tx : 14; //rf rssi offset1, used for dc cancellation in tx, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiOffset1Tx_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET2_RX_REG 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset2Rx : 8; //rf rssi offset2, used for digital top power dBm conversion, RX mode, reset value: 0x0, access type: RWS
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiOffset2RxReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_OFFSET2_TX_REG 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset2Tx : 8; //rf rssi offset2, used for digital top power dBm conversion, TX mode, reset value: 0x0, access type: RWS
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiOffset2TxReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SYSTEM_GAIN_REG 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSystemGain : 9; //rf rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SYSTEM_GAIN_SELECT_REG 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiSystemGainHwSelect : 1; //rf  rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SAT_THR_RANGE0 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSatThrRange0 : 9; //rf rssi saturation threshold for range 0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiSatThrRange0_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_SAT_THR_RANGE1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSatThrRange1 : 9; //rf rssi saturation threshold for range 1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiSatThrRange1_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A_RANGE0_REG 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefARange0 : 16; //rf rssi coef A range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefARange0Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A_RANGE1_REG 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefARange1 : 16; //rf rssi coef A range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefARange1Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A2_RANGE0_REG 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefA2Range0 : 16; //rf rssi coef A2 range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefA2Range0Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_A2_RANGE1_REG 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefA2Range1 : 16; //rf rssi coef A2 range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefA2Range1Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_B_RANGE0_REG 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefBRange0 : 16; //rf rssi coef B range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefBRange0Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_B_RANGE1_REG 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefBRange1 : 16; //rf rssi coef B range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefBRange1Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_C_RANGE0_REG 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefCRange0 : 16; //rf rssi coef C range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefCRange0Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_C_RANGE1_REG 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefCRange1 : 16; //rf rssi coef C range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefCRange1Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_NLOG_RANGE0_REG 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefNlogRange0 : 16; //rf rssi coef Nlog range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefNlogRange0Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_COEF_NLOG_RANGE1_REG 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefNlogRange1 : 16; //rf rssi coef Nlog range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiCoefNlogRange1Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_A_REG 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefA : 16; //rf_tssi_coef_A, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfTssiCoefAReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_A2_REG 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefA2 : 16; //rf_tssi_coef_A2, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfTssiCoefA2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_B_REG 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefB : 16; //rf_tssi_coef_B, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfTssiCoefBReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_C_REG 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefC : 16; //rf_tssi_coef_C, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfTssiCoefCReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_TSSI_COEF_NLOG_REG 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefNlog : 16; //rf_tssi_coef_Nlog, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntRfTssiCoefNlogReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_TX_RX_REG 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiTxMode : 1; //rf rssi user programable mode : 1 tx mode, 0 rx mode, reset value: 0x0, access type: RW
		uint32 rfRssiRxRange : 1; //rf rssi user programable range, reset value: 0x0, access type: RW
		uint32 rfRssiModeOverride : 1; //rf rssi  mode override  - 1 use from pm, 0 use hw mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rfRssiRangeOverride : 1; //rf rssi  range override  - 1 use from pm, 0 use hw mode, reset value: 0x0, access type: RW
		uint32 rfTssiCoefOverride : 1; //rf tssi coef override  - 1 tssi coef from pm, 0 tssi coef from TPC  hw , reset value: 0x0, access type: RW
		uint32 rfRssiBlkSizeOverride : 1; //rf rssi blk size override: 1 decimation from pm, 0 - decimation according to TX/RX mode, reset value: 0x0, access type: RW
		uint32 rfTssiCoefMode : 2; //rf_tssi_coef_mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiTxRxReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_RSSI_REG 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssi : 9; //BB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_POWER_REG 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbPower : 9; //BB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_SRSSI_REG 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbSrssi : 9; //BB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_SPOWER_REG 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbSpower : 9; //BB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntBbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_BB_PWR_DIFF_REG 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbB1 : 9; //BB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 bbB2 : 9; //BB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntBbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_IB_RSSI_REG 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibRssi : 9; //IB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntIbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_IB_POWER_REG 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibPower : 9; //IB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntIbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_IB_SRSSI_REG 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibSrssi : 9; //IB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntIbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_IB_SPOWER_REG 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibSpower : 9; //IB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntIbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_IB_PWR_DIFF_REG 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ibB1 : 9; //IB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 ibB2 : 9; //IB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntIbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_RSSI_REG 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssi : 9; //FB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_POWER_REG 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbPower : 9; //FB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_SRSSI_REG 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbSrssi : 9; //FB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_SPOWER_REG 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbSpower : 9; //FB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntFbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_FB_PWR_DIFF_REG 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fbB1 : 9; //FB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 fbB2 : 9; //FB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntFbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_RSSI_REG 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssi : 9; //HB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_POWER_REG 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbPower : 9; //HB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_SRSSI_REG 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbSrssi : 9; //HB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_SPOWER_REG 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbSpower : 9; //HB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntHbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_HB_PWR_DIFF_REG 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbB1 : 9; //HB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 hbB2 : 9; //HB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntHbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_REG 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssi : 9; //RF RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_POWER_REG 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfPower : 9; //RF power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_SRSSI_REG 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfSrssi : 9; //RF RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_SPOWER_REG 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfSpower : 9; //RF Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntRfSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_SATURATION_RESULT 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiSatFlag : 1; //rf rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 fbRssiSatFlag : 1; //fb rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 bbRssiSatFlag : 1; //bb rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 hbRssiSatFlag : 1; //hb rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 ibRssiSatFlag : 1; //ib rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 aciRssiSatFlag : 1; //aci rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 upHbRssiSatFlag : 1; //up_hb rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 upIbRssiSatFlag : 1; //up_ib rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 upAciRssiSatFlag : 1; //up_aci rssi is saturated above threshold, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntSaturationResult_u;

/*REG_B1_DFS_PHY_AGC_ANT_RSSI_BLOCKS_GCLK_EN 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiGclkEn : 1; //enable rf rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 fbRssiGclkEn : 1; //enable fb rssi peripheral gator, reset value: 0x0, access type: RW
		uint32 bbRssiGclkEn : 1; //enable bb rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 hbRssiGclkEn : 1; //enable hb rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 ibRssiGclkEn : 1; //enable ib rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 upHbRssiGclkEn : 1; //enable up hb rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 upIbRssiGclkEn : 1; //enable up ib rssi peripheral gator, reset value: 0x1, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB1DfsPhyAgcAntRssiBlocksGclkEn_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_DEC_OUT_STS 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiDecOutSts : 14; //rf rssi decimator output for dc measurement, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiDecOutSts_u;

/*REG_B1_DFS_PHY_AGC_ANT_RF_RSSI_HIGH_RES_REG 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiHighRes : 10; //RF RSSI output in 1/4 db resolution , reset value: 0x0, access type: ROS
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyAgcAntRfRssiHighResReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_RF_POWER 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfPower : 9; //programmble rf power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmRfPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_FB_POWER 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbPower : 9; //programmble fb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmFbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPower : 9; //programmble bb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_HB_POWER 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbPower : 9; //programmble hb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmHbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPower : 9; //programmble ib power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACI_POWER 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciPower : 9; //programmble aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_RF_OFFSET 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfOffset : 9; //offset for rf power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmRfOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_FB_OFFSET 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbOffset : 9; //offset for fb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmFbOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_OFFSET 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbOffset : 9; //offset for bb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_HB_OFFSET 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbOffset : 9; //offset for hb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmHbOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_OFFSET 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbOffset : 9; //offset for ib power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACI_OFFSET 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciOffset : 9; //offset for aci power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAciOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_SATURATION_REG 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRfRssiSatFlag : 1; //rf rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmFbRssiSatFlag : 1; //fb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmBbRssiSatFlag : 1; //bb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmHbRssiSatFlag : 1; //hb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmIbRssiSatFlag : 1; //ib rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAciRssiSatFlag : 1; //aci rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmUpHbRssiSatFlag : 1; //up hb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmUpIbRssiSatFlag : 1; //up ib rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmUpAciRssiSatFlag : 1; //up aci rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmSaturationReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_DELAY 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainDelay : 16; //delay between table go to start acceleration, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainDelay_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SOURCES 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainRfSource : 1; //rf power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainFbSource : 1; //fb power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainBbSource : 1; //bb power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainHbSource : 1; //hb power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainIbSource : 1; //ib power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainAciSource : 1; //aci power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainUpHbSource : 1; //up hb power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainUpIbSource : 1; //up ib power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainUpAciSource : 1; //up aci power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 pmAccGainRfSatSource : 1; //rf rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainFbSatSource : 1; //fb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainBbSatSource : 1; //bb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainHbSatSource : 1; //hb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainIbSatSource : 1; //ib rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainAciSatSource : 1; //aci rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainUpHbSatSource : 1; //up hb rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainUpIbSatSource : 1; //up ib rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainUpAciSatSource : 1; //up aci rssi is saturated above threshold from pm, reset value: 0x0, access type: RW
		uint32 pmAccGainPriAciSource : 1; //primary aci power from pm or peripherals: 1- from pm, 0-from peripherals, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmAccGainSignalSource : 2; //acc_gain_signal_source: 0-from BB, 1-from IB, 2-from Aci, 3- from HB, reset value: 0x0, access type: RW
		uint32 pmAccGainEnvSource : 2; //acc_gain_env_source: 0- from rf, 1-from fb, 2 - from bb, reset value: 0x0, access type: RW
		uint32 pmAccGainPgc1Source : 2; //acc_gain_pgc1_source: 0- from fb, 1-from bb,2-rf, reset value: 0x0, access type: RW
		uint32 pmAccGainIbGainSource : 1; //acc_gain_ib_gain calculation source: 0-from ib rssi, 1- from aci, reset value: 0x0, access type: RW
		uint32 pmAccGainBlockerTypeSource : 1; //blocker type source, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainSources_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_MAX_POWER_LEVEL 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainMaxPowerLevel : 9; //acc gain HW limit max power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainMaxPowerLevel_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_MIN_POWER_LEVEL 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainMinPowerLevel : 9; //acc gain HW limit min power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainMinPowerLevel_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SIGNAL_MAX_LEVEL 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainSignalMaxLevel : 9; //acc gain HW limit for BB is RF  in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainSignalMaxLevel_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_ENV_MIN_LEVEL 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainEnvMinLevel : 9; //acc gain HW limit for RF is BB in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainEnvMinLevel_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SIGNAL_ENV_DIFF_VALUES 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainSignalEnvMaxDiff : 9; //maximal difference beween signal and environment for table access in 1/2 dB, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 pmAccGainSignalEnvMinDiff : 9; //minimal difference beween signal and environment for table access, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainSignalEnvDiffValues_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_GSP_TABLE_CONTROL 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 pmAccGainGspTableShift : 3; //shift for delta calculated for table resolution, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
		uint32 pmAccGainGspTableMode : 1; //mode for table:0-per bw, 1-full band, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainGspTableControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_LNA_CONTROL 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainLnaIndexMode : 1; //lna_target or switching point, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainLnaControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_SEL_S2D_RX_LUT 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSelS2DRxLut0 : 1; //control bit to select rf rssi range and s2d pair for lna index 0, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut1 : 1; //control bit to select rf rssi range and s2d pair for lna index 1, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut2 : 1; //control bit to select rf rssi range and s2d pair for lna index 2, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut3 : 1; //control bit to select rf rssi range and s2d pair for lna index 3, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut4 : 1; //control bit to select rf rssi range and s2d pair for lna index 4, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut5 : 1; //control bit to select rf rssi range and s2d pair for lna index 5, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut6 : 1; //control bit to select rf rssi range and s2d pair for lna index 6, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut7 : 1; //control bit to select rf rssi range and s2d pair for lna index 7, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut8 : 1; //control bit to select rf rssi range and s2d pair for lna index 8, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut9 : 1; //control bit to select rf rssi range and s2d pair for lna index 9, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut10 : 1; //control bit to select rf rssi range and s2d pair for lna index 10, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut11 : 1; //control bit to select rf rssi range and s2d pair for lna index 11, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut12 : 1; //control bit to select rf rssi range and s2d pair for lna index 12, reset value: 0x0, access type: RW
		uint32 pmSelS2DRxLut13 : 1; //control bit to select rf rssi range and s2d pair for lna index 13, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntPmSelS2DRxLut_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_OFFSET 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPgc1GainOffset : 9; //pgc1 target power offset in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmPgc1GainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_LIMITS 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmPgc1GainPlusOffsetMaxLimit : 9; //pgc1 gain plus offset max limit in 1/2 dB, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 pmPgc1GainPlusOffsetMinLimit : 9; //pgc1 gain plus offset min limit in 1/2 dB, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegB1DfsPhyAgcAntPmPgc1GainLimits_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PGC1_GAIN_SHIFT 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmPgc1GainShift : 3; //pgc1 index shift to convert from gain to index, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyAgcAntPmPgc1GainShift_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_RX_FILTER_CONTROL 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pmRxFilterBwInterferenceMode : 1; //rx_filter_bw interference_mode: 0-no interference, 1-w/ interference. Different filter bw for int, reset value: 0x0, access type: RW
		uint32 reserved1 : 29;
	} bitFields;
} RegB1DfsPhyAgcAntPmRxFilterControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_RF_SYSTEM_GAIN_OFFSET 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfSystemGainOffset : 9; //offset for rf gain acceleration (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmRfSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_FB_SYSTEM_GAIN_OFFSET 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbSystemGainOffset : 9; //offset for fb gain acceleration (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmFbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_SYSTEM_GAIN_OFFSET 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbSystemGainOffset : 9; //offset for bb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_POST_BB_SYSTEM_GAIN_OFFSET 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPostBbSystemGainOffset : 9; //offset for pre bb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmPostBbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_HB_SYSTEM_GAIN_OFFSET 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbSystemGainOffset : 9; //offset for hb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmHbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_SYSTEM_GAIN_OFFSET 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbSystemGainOffset : 9; //offset for ib gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACI_SYSTEM_GAIN_OFFSET 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciSystemGainOffset : 9; //offset for aci gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmAciSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_CCA_SYSTEM_GAIN_OFFSET 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmCcaSystemGainOffset : 9; //offset for cca gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmCcaSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_DET_SYSTEM_GAIN_OFFSET 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetSystemGainOffset : 9; //offset for det gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmDetSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_BOF_OFFSET 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbBofOffset : 9; //convert bb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_HB_BOF_OFFSET 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbBofOffset : 9; //convert hb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmHbBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_RSSI_BOF_OFFSET 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbRssiBofOffset : 9; //convert ib rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbRssiBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_ACI_BOF_OFFSET 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbAciBofOffset : 9; //convert ib aci power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbAciBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_DET_BOF_OFFSET 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetBofOffset : 9; //convert det power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmDetBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_BOF_TARGET 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbBofTarget : 9; //target backoff from full scale at bb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_HB_BOF_TARGET 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbBofTarget : 9; //target backoff from full scale at hb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmHbBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_BOF_TARGET 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbBofTarget : 9; //target backoff from full scale at ib rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_DET_BOF_TARGET 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetBofTarget : 9; //target backoff from full scale at det rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmDetBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_DIFI1_DB_GAIN 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDifi1DbGain : 9; //difi1 gain in 1/2 dB must be equal to the linear gain of pm_difi1_gain , reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmDifi1DbGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PRESET_LNA_PGC1 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLnaIndexMaxGain : 4; //max gain lna index, reset value: 0x0, access type: RW
		uint32 pmLnaIndexStep1Gain : 4; //step1 env & signal are saturated, gain lna index, reset value: 0x0, access type: RW
		uint32 pmLnaIndexStep1EnvSatGain : 4; //step1 env only is saturated, gain lna index, reset value: 0x0, access type: RW
		uint32 pmLnaIndexStep1SignalSatGain : 4; //step1 signal only is saturated, gain lna index, reset value: 0x0, access type: RW
		uint32 pmPgc1IndexMaxGain : 3; //max gain pgc1 index, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmPgc1IndexStep1Gain : 3; //step1 env & signal are satuarted gain pgc1 index, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmPgc1IndexStep1EnvSatGain : 3; //step1 env only is saturated gain pgc1 index, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmPgc1IndexStep1SignalSatGain : 3; //step1 signal only is saturated gain pgc1 index, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntPmPresetLnaPgc1_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_BB_DIGITAL_GAIN 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxBbDigitalGain : 9; //max gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_BB_DIGITAL_GAIN 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1BbDigitalGain : 9; //step1 env & signal are saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1BbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_BB_DIGITAL_GAIN 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatBbDigitalGain : 9; //step1 env only is saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1EnvSatBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_BB_DIGITAL_GAIN 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatBbDigitalGain : 9; //step1 signal only is saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1SignalSatBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_HB_DIGITAL_GAIN 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxHbDigitalGain : 9; //max gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_HB_DIGITAL_GAIN 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1HbDigitalGain : 9; //step1 env & signal are saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1HbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_HB_DIGITAL_GAIN 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatHbDigitalGain : 9; //step1 env only is saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1EnvSatHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_HB_DIGITAL_GAIN 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatHbDigitalGain : 9; //step1 signal only is saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1SignalSatHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_IB_DIGITAL_GAIN 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxIbDigitalGain : 9; //max gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_IB_DIGITAL_GAIN 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1IbDigitalGain : 9; //step1 env & signal are saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1IbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_IB_DIGITAL_GAIN 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatIbDigitalGain : 9; //step1 env only is saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1EnvSatIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_IB_DIGITAL_GAIN 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatIbDigitalGain : 9; //step1 signal only is saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1SignalSatIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_DET_DIGITAL_GAIN 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxDetDigitalGain : 9; //max gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_DET_DIGITAL_GAIN 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1DetDigitalGain : 9; //step1 env & signal are saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1DetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_ENV_SAT_DET_DIGITAL_GAIN 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatDetDigitalGain : 9; //step1 env only is saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1EnvSatDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_STEP1_SIGNAL_SAT_DET_DIGITAL_GAIN 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatDetDigitalGain : 9; //step1 signal only is saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmStep1SignalSatDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_RF_POWER 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accRfPower : 9; //rf power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccRfPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_FB_POWER 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accFbPower : 9; //fb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccFbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPower : 9; //bb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_POWER 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbPower : 9; //hb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPower : 9; //ib power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_ACI_POWER 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accAciPower : 9; //aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_SIGNAL_POWER 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accSignalPower : 9; //signal power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccSignalPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_ENV_POWER 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accEnvPower : 9; //env power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccEnvPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_STATUS 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accRfSat : 1; //rf rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accFbSat : 1; //fb rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accBbSat : 1; //bb rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accHbSat : 1; //hb rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accIbSat : 1; //ib rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accAciSat : 1; //aci rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accUpHbSat : 1; //up_hb rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accUpIbSat : 1; //up ib rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accUpAciSat : 1; //up aci rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accSignalLimitMax : 1; //signal power is bigger than pm_max_power_level, reset value: 0x0, access type: RO
		uint32 accSignalLimitMin : 1; //signal power is less than pm_min_power_level, reset value: 0x0, access type: RO
		uint32 accSignalSat : 1; //signal rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accEnvLimitMax : 1; //env power is bigger than pm_max_power_level, reset value: 0x0, access type: RO
		uint32 accEnvSat : 1; //env rssi sat reading locked at "table go", reset value: 0x0, access type: RO
		uint32 accEnvLimitMin : 1; //env power is less than pm_min_power_level, reset value: 0x0, access type: RO
		uint32 accSignalIsEnvValid : 1; //signal power is bigger than pm_signal_max_level, use env power instead, reset value: 0x0, access type: RO
		uint32 accEnvIsSignalValid : 1; //env power is less than pm_env_min_level, use signal power instead, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1DfsPhyAgcAntAccStatus_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_RF_SYSTEM_GAIN 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accRfSystemGain : 9; //rf system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccRfSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_FB_SYSTEM_GAIN 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accFbSystemGain : 9; //fb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccFbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_SYSTEM_GAIN 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbSystemGain : 9; //bb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_POST_BB_SYSTEM_GAIN 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accPostBbSystemGain : 9; //pre bb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccPostBbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_SYSTEM_GAIN 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbSystemGain : 9; //hb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_SYSTEM_GAIN 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbSystemGain : 9; //ib system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_ACI_SYSTEM_GAIN 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accAciSystemGain : 9; //aci system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccAciSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_CCA_SYSTEM_GAIN 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accCcaSystemGain : 9; //cca system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccCcaSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DET_SYSTEM_GAIN 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetSystemGain : 9; //det system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccDetSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_DIGITAL_GAIN 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbDigitalGain : 9; //bb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_DIGITAL_GAIN 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbDigitalGain : 9; //hb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_DIGITAL_GAIN 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbDigitalGain : 9; //ib digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DET_DIGITAL_GAIN 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetDigitalGain : 9; //det digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_BOF 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbBof : 9; //bb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_TOTAL_DIGITAL_GAIN 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbTotalDigitalGain : 9; //bb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_DIGITAL_BOF 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbDigitalBof : 9; //bb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_BOF 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbBof : 9; //hb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_TOTAL_DIGITAL_GAIN 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbTotalDigitalGain : 9; //hb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_HB_DIGITAL_BOF 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbDigitalBof : 9; //hb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccHbDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_BOF 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbBof : 9; //ib backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_TOTAL_DIGITAL_GAIN 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbTotalDigitalGain : 9; //sum of ib digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_DIGITAL_BOF 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbDigitalBof : 9; //ib_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DET_BOF 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetBof : 9; //det backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccDetBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DET_TOTAL_DIGITAL_GAIN 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetTotalDigitalGain : 9; //sum of det digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccDetTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DET_DIGITAL_BOF 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetDigitalBof : 9; //det_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccDetDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DC_OFFSET_I 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDcOffsetI : 14; //chosen dc Q offset according to bw,pgc1_index,lna_index, reset value: 0x0, access type: ROS
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntAccDcOffsetI_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_DC_OFFSET_Q 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDcOffsetQ : 14; //chosen dc I offset according to bw,pgc1_index,lna_index, reset value: 0x0, access type: ROS
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntAccDcOffsetQ_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_AGC_RAMS_RM 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAgcTableRm : 3; //agc_gsp_table_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyAgcAntPmAgcRamsRm_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_GAIN_VALUES 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accLnaIndex : 4; //calculated lna_index, reset value: 0x0, access type: RO
		uint32 accSelS2DRx : 1; //calculated sel_s2d_rx, reset value: 0x0, access type: RO
		uint32 accPgc1Index : 3; //calculated pgc1_index, reset value: 0x0, access type: RO
		uint32 accRxFilter : 6; //calculated rx_filter tuning word, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntAccGainValues_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_FCSI_CONTROL 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmFcsiEnable : 1; //enable fcsi , reset value: 0x0, access type: RW
		uint32 pmFcsiHeader : 2; //header upper 1 bits of fcsi , reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 pmFcsiAddress : 7; //fcsi_address, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB1DfsPhyAgcAntPmFcsiControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_FCSI_VALUES 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accFcsiData : 16; //data which is sent to rfic via fcsi, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntAccFcsiValues_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_GAIN_ACC_CONTROL 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accDelayTimer : 16; //read acc_delay_timer counting, 0 means no timer, reset value: 0x0, access type: RO
		uint32 accStmCounter : 5; //read acc_stm_counter counting, 0 means no ongoing acceleration, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 accFcsiBusy : 1; //fcsi_busy, 0 means no fcsi access, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegB1DfsPhyAgcAntAccGainAccControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_CODE_ACC_THR 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmIbTimerThr : 16; //pm_ib_timer_thr, reset value: 0x0, access type: RW
		uint32 pmIbReleaseThr : 16; //pm_ib_release_thr, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmIbCodeAccThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_CODE_ACC_THR 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBbTimerThr : 16; //pm_bb_timer_thr, reset value: 0x0, access type: RW
		uint32 pmBbReleaseThr : 16; //pm_bb_release_thr, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBbCodeAccThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LONGTERM_RELEASE_THR 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLongtermReleaseThr : 16; //pm_longterm_release_thr, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntPmLongtermReleaseThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LONGTERM_THR 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLongtermThr : 9; //pm_longterm_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLongtermThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_FAST_LOW_THR 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerFastLowThr : 9; //pm_bb_power_fast_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPowerFastLowThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_DELTA_CODE_ACC_THR 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBbDeltaThr : 9; //pm_bb_delta_thr, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 pmIbDeltaThr : 9; //pm_ib_delta_thr, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegB1DfsPhyAgcAntPmDeltaCodeAccThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_UP_THR 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerUpThr : 9; //pm_bb_power_up_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPowerUpThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_SLOW_LOW_THR 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerSlowLowThr : 9; //pm_bb_power_slow_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPowerSlowLowThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_UP_THR 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerUpThr : 9; //pm_ib_power_up_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbPowerUpThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_SLOW_LOW_THR 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerSlowLowThr : 9; //pm_ib_power_slow_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbPowerSlowLowThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_SET_CODE_ACC_THR_CTRL 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSetBbThrCtrl : 1; //set_bb_thr_ctrl: 1 -from pm, 0- from hw accelerator, reset value: 0x0, access type: RW
		uint32 pmSetIbThrCtrl : 1; //set_ib_thr_ctrl: 1 -from pm, 0- from hw accelerator, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyAgcAntPmSetCodeAccThrCtrl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ALGO_CTRL 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAlgoCtrl : 16; //code acclerator algo ctrl, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntPmAlgoCtrl_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_EVENT_STATUS 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accCodeAccEventType : 9; //code acc event, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 accCodeAccEventIndication : 1; //code acc event bit wise or, reset value: 0x0, access type: RO
		uint32 reserved1 : 19;
	} bitFields;
} RegB1DfsPhyAgcAntAccEventStatus_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_SET_INC_GAP_DB 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThSetIncGapDb : 9; //margin to add above measured bb power to calculate energy increase threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbThSetIncGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_SET_DEC_GAP_DB 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThSetDecGapDb : 9; //margin to substract below measured bb power to calculate energy deccrease threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbThSetDecGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_TH_UP_EXTRA_GAP_DB 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThUpExtraGapDb : 9; //margin used inorder to limit enery increase threhold for high bb power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbThUpExtraGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_UP_DEF_THR 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerUpDefThr : 9; //default energy increases bb power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPowerUpDefThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BB_POWER_SLOW_LOW_MIN_THR 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerSlowLowMinThr : 9; //default energy decrease bb power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmBbPowerSlowLowMinThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_SET_INC_GAP_DB 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThSetIncGapDb : 9; //margin to add above measured ib power to calculate energy increase threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbThSetIncGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_SET_DEC_GAP_DB 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThSetDecGapDb : 9; //margin to substract below measured ib power to calculate energy deccrease threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbThSetDecGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_TH_UP_EXTRA_GAP_DB 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThUpExtraGapDb : 9; //margin used inorder to limit enery increase threhold for high ib power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbThUpExtraGapDb_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_UP_DEF_THR 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerUpDefThr : 9; //default energy increases ib power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbPowerUpDefThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_IB_POWER_SLOW_LOW_MIN_THR 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerSlowLowMinThr : 9; //default energy decrease ib power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmIbPowerSlowLowMinThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER_UP_THR 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPowerUpThr : 9; //bb energy increase threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbPowerUpThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BB_POWER_SLOW_LOW_THR 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPowerSlowLowThr : 9; //bb energy deccrease threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccBbPowerSlowLowThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER_UP_THR 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPowerUpThr : 9; //ib energy increase threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbPowerUpThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_IB_POWER_SLOW_LOW_THR 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPowerSlowLowThr : 9; //ib energy deccrease threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccIbPowerSlowLowThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_GAIN_WORD 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcTableGo : 1; //trigger agc gain accleration, reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
		uint32 calcSetCalibration : 1; //calc_set_calibration, reset value: 0x0, access type: RW
		uint32 calcSetMaxGain : 1; //calc_set_max_gain, reset value: 0x0, access type: RW
		uint32 calcSetStep1Gain : 1; //calc_set_step1_gain, reset value: 0x0, access type: RW
		uint32 calcNewLna : 1; //calc_new_lna, reset value: 0x0, access type: RW
		uint32 calcNewPgc1 : 1; //calc_new_pgc1, reset value: 0x0, access type: RW
		uint32 calcNewBbDigitalGain : 1; //calc_new_bb_digital_gain after difi2, reset value: 0x0, access type: RW
		uint32 calcNewHbDigitalGain : 1; //calc_new_hb_digital_gain after pre filter, reset value: 0x0, access type: RW
		uint32 calcNewIbDigitalGain : 1; //calc_new_ib_digital_gain after channel filter, reset value: 0x0, access type: RW
		uint32 calcNewDetDigitalGain : 1; //calc_new_det_digital_gain after band select for detector, reset value: 0x0, access type: RW
		uint32 calcNewDcValues : 1; //calc_new_dc_values offset after difi2, reset value: 0x0, access type: RW
		uint32 calcNewBlockerType : 1; //calc new blocker type, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 calcNoRfAccess : 1; //calc new gain without rf access, reset value: 0x0, access type: RW
		uint32 reserved2 : 16;
	} bitFields;
} RegB1DfsPhyAgcAntCalcAgcGainWord_u;

/*REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_STM_CLEAR_CONTROL 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcClearAgcTimers : 1; //clear agc timers in code acceleator stm, reset value: 0x0, access type: WO
		uint32 calcClearAgcEventType : 1; //clea agc event in code accelerator stm, reset value: 0x0, access type: WO
		uint32 calcClearAgcMaskOp : 1; //disable code accelerator, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyAgcAntCalcAgcStmClearControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_CALC_ACC_AUTO_CLOSE_MODE 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcAccAutoCloseMode : 1; //enables code acceleration, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntCalcAccAutoCloseMode_u;

/*REG_B1_DFS_PHY_AGC_ANT_CALC_POWER_THR_ACC_GO 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcBbPowerThrAccGo : 1; //calc_bb_power_thr_acc_go, reset value: 0x0, access type: WO
		uint32 calcIbPowerThrAccGo : 1; //calc_ib_power_thr_acc_go, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyAgcAntCalcPowerThrAccGo_u;

/*REG_B1_DFS_PHY_AGC_ANT_CALC_AGC_GAIN_CANCEL 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcAgcGainCancel : 1; //cancels gain acceleration timer and cancels table go, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntCalcAgcGainCancel_u;

/*REG_B1_DFS_PHY_AGC_ANT_DISABLE_PERIPHERALS 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disablePeripherals : 1; //disable_peripherals, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntDisablePeripherals_u;

/*REG_B1_DFS_PHY_AGC_ANT_AGC_STM_RDBACK 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agcStmRdback : 6; //agc_stm_rdback, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1DfsPhyAgcAntAgcStmRdback_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_BB_DIGITAL_GAIN 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccBbDigitalGain : 9; //max limit for bb digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxLimitAccBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_BB_DIGITAL_GAIN 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccBbDigitalGain : 9; //min limit for bb digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMinLimitAccBbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_HB_DIGITAL_GAIN 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccHbDigitalGain : 9; //max limit for hb digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxLimitAccHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_HB_DIGITAL_GAIN 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccHbDigitalGain : 9; //min limit for hb digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMinLimitAccHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_IB_DIGITAL_GAIN 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccIbDigitalGain : 9; //max limit for ib digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxLimitAccIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_IB_DIGITAL_GAIN 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccIbDigitalGain : 9; //min limit for ib digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMinLimitAccIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MAX_LIMIT_ACC_DET_DIGITAL_GAIN 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccDetDigitalGain : 9; //max limit for det digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMaxLimitAccDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_MIN_LIMIT_ACC_DET_DIGITAL_GAIN 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccDetDigitalGain : 9; //min limit for det digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmMinLimitAccDetDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_BLOCKER_CONTROL 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBlockerType : 2; //pm blocker type, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 accBlockerType : 2; //selected blocker type, reset value: 0x0, access type: RO
		uint32 reserved1 : 26;
	} bitFields;
} RegB1DfsPhyAgcAntBlockerControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BLOCKER_TH 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsAdjAltTh : 8; //threshold for adj alt between LSB and USB in 1 db, reset value: 0x0, access type: RW
		uint32 pmEnvAltTh : 8; //threshold for alt on env, reset value: 0x0, access type: RW
		uint32 pmEnvSigAltTh : 8; //threshold for alt on env compare to sig, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB1DfsPhyAgcAntPmBlockerTh_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ALGO_CTRL_SEL 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAlgoCtrlSelIn0 : 3; //select source in0: 0-rf, 1-fb, 2-bb, 3-hb, 4-ib, 5-aci, reset value: 0x2, access type: RW
		uint32 reserved0 : 1;
		uint32 pmAlgoCtrlSelIn1 : 3; //select source in1: 0-rf, 1-fb, 2-bb, 3-hb, 4-ib, 5-aci, reset value: 0x3, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB1DfsPhyAgcAntPmAlgoCtrlSel_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_BLOCKER_ADDR 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accLsbBlockerAddr : 7; //lsb blocker address, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 accUsbBlockerAddr : 7; //usb blocker address, reset value: 0x0, access type: RO
		uint32 reserved1 : 17;
	} bitFields;
} RegB1DfsPhyAgcAntAccBlockerAddr_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_INB_RSSI_PARAMS_REG 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upInbandRssiBlkSize : 3; //up inband rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 upInbandRssiIirPole : 4; //up inband rssi iir pole, reset value: 0x0, access type: RW
		uint32 upInbandRssiIirPoleMin : 3; //up inband rssi iir shift, reset value: 0x0, access type: RW
		uint32 upInbandRssiOffset1 : 16; //up inband rssi offset1, reset value: 0x0, access type: RW
		uint32 upInbandRssiCancelDc : 1; //up inband cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntUpInbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_INB_RSSI_OFFSET2_REG 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiOffset2 : 9; //up inband rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpInbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SYSTEM_GAIN_REG 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiSystemGain : 9; //up inband rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpInbandRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upInbandRssiSystemGainHwSelect : 1; //up inband rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntUpInbandRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_INBAND_RSSI_SAT_THR 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiSatThr : 9; //up inband rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpInbandRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_PARAMS_REG 0x414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upHbRssiBlkSize : 3; //up hb rssi decimatiion, 2^(block size), reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 upHbRssiIirPole : 4; //up hb rssi iir pole, reset value: 0x0, access type: RW
		uint32 upHbRssiIirPoleMin : 3; //up hb rssi iir shift, reset value: 0x0, access type: RW
		uint32 upHbRssiOffset1 : 16; //up hb rssi offset1, reset value: 0x0, access type: RW
		uint32 upHbRssiCancelDc : 1; //up hb rssi cancel DC in power calculation (1 - remove dc, 0 - no remove), reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiParamsReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_OFFSET2_REG 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiOffset2 : 9; //up hb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiOffset2Reg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SYSTEM_GAIN_REG 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiSystemGain : 9; //up hb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiSystemGainReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SYSTEM_GAIN_SELECT_REG 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upHbRssiSystemGainHwSelect : 1; //up hb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiSystemGainSelectReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_SAT_THR 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiSatThr : 9; //up hb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiSatThr_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_IB_RSSI_REG 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbRssi : 9; //up IB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpIbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_IB_POWER_REG 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbPower : 9; //up IB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpIbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_IB_SRSSI_REG 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbSrssi : 9; //up IB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpIbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_IB_SPOWER_REG 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbSpower : 9; //up IB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpIbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_IB_PWR_DIFF_REG 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upIbB1 : 9; //up IB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 upIbB2 : 9; //up IB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntUpIbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_RSSI_REG 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssi : 9; //up HB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbRssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_POWER_REG 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbPower : 9; //up HB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbPowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_SRSSI_REG 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbSrssi : 9; //up HB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbSrssiReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_SPOWER_REG 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbSpower : 9; //up HB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbSpowerReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_HB_PWR_DIFF_REG 0x44C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upHbB1 : 9; //up HB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 upHbB2 : 9; //up HB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyAgcAntUpHbPwrDiffReg_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_POWER 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbPower : 9; //programmble up hb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpHbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_POWER 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbPower : 9; //programmble up ib power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpIbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_ACI_POWER 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpAciPower : 9; //programmble up aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_POWER 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbPower : 9; //up hb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_POWER 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbPower : 9; //up ib power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_ACI_POWER 0x464 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpAciPower : 9; //up aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_UP_CONTROL 0x468 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbIsMax : 1; //for gain accelerator hb is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 ibIsMax : 1; //for gain accelerator ib is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 aciIsMax : 1; //for gain accelerator aci is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 hbSysGainSource : 2; //hb source: 0 - lo is lo & up is up, 1-primary for both, 2- max for both, reset value: 0x0, access type: RW
		uint32 ibSysGainSource : 2; //ib source: 0 - lo is lo & up is up, 1-primary for both, 2- max for both, reset value: 0x0, access type: RW
		uint32 aciSysGainSource : 2; //aci source: 0 - lo is lo & up is up, 1-primary for both, 2- max for both, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 eventHbIsMax : 1; //for event accelerator hb is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 eventIbIsMax : 1; //for event accelerator ib is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 eventAciIsMax : 1; //for event accelerator aci is max between up and lo / else is primary, reset value: 0x0, access type: RW
		uint32 reserved2 : 17;
	} bitFields;
} RegB1DfsPhyAgcAntUpControl_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_SYSTEM_GAIN_OFFSET 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbSystemGainOffset : 9; //offset for up hb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpHbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_SYSTEM_GAIN_OFFSET 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbSystemGainOffset : 9; //offset for up ib gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpIbSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_ACI_SYSTEM_GAIN_OFFSET 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpAciSystemGainOffset : 9; //offset for up aci gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpAciSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_CCA_SYSTEM_GAIN_OFFSET 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpCcaSystemGainOffset : 9; //offset for up cca gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpCcaSystemGainOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_BOF_OFFSET 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbBofOffset : 9; //convert up hb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpHbBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_RSSI_BOF_OFFSET 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbRssiBofOffset : 9; //convert up ib rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpIbRssiBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_ACI_BOF_OFFSET 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbAciBofOffset : 9; //convert up ib aci power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpIbAciBofOffset_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_HB_BOF_TARGET 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbBofTarget : 9; //target backoff from full scale at up hb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpHbBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_UP_IB_BOF_TARGET 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbBofTarget : 9; //target backoff from full scale at up ib rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmUpIbBofTarget_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_SYSTEM_GAIN 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbSystemGain : 9; //up hb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_SYSTEM_GAIN 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbSystemGain : 9; //up ib system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_ACI_SYSTEM_GAIN 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpAciSystemGain : 9; //up aci system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpAciSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_CCA_SYSTEM_GAIN 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpCcaSystemGain : 9; //up cca system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpCcaSystemGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_DIGITAL_GAIN 0x4A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbDigitalGain : 9; //up hb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_DIGITAL_GAIN 0x4A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbDigitalGain : 9; //up ib digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_BOF 0x4A8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbBof : 9; //up hb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_TOTAL_DIGITAL_GAIN 0x4AC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbTotalDigitalGain : 9; //up hb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_HB_DIGITAL_BOF 0x4B0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbDigitalBof : 9; //up hb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpHbDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_BOF 0x4B4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbBof : 9; //up ib backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_TOTAL_DIGITAL_GAIN 0x4B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbTotalDigitalGain : 9; //sum of up ib digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbTotalDigitalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_UP_IB_DIGITAL_BOF 0x4BC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbDigitalBof : 9; //up_ib_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccUpIbDigitalBof_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_ACC_GAIN_SAT_SOURCES 0x4C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainEnvSatSource : 3; //acc_gain_env_sat_source: 0- from rf, 1-from fb, 2 - from bb, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 pmAccGainSignalSatSource : 7; //acc_gain_signal_source: 0-from BB, 1-from IB, 2-from Aci, 3- from HB, 4-up_ib, 5-up_aci,6-up_hb, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB1DfsPhyAgcAntPmAccGainSatSources_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_0 0x4C4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut0 : 9; //lna gain in db for lna_index 0 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut0_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_1 0x4C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut1 : 9; //lna gain in db for lna_index 1 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut1_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_2 0x4CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut2 : 9; //lna gain in db for lna_index 2 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut2_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_3 0x4D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut3 : 9; //lna gain in db for lna_index 3 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut3_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_4 0x4D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut4 : 9; //lna gain in db for lna_index 4 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut4_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_5 0x4D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut5 : 9; //lna gain in db for lna_index 5 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut5_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_6 0x4DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut6 : 9; //lna gain in db for lna_index 6 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut6_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_7 0x4E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut7 : 9; //lna gain in db for lna_index 7 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut7_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_8 0x4E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut8 : 9; //lna gain in db for lna_index 8 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut8_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_9 0x4E8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut9 : 9; //lna gain in db for lna_index 9 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut9_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_10 0x4EC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut10 : 9; //lna gain in db for lna_index 10 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut10_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_11 0x4F0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut11 : 9; //lna gain in db for lna_index 11 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut11_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_12 0x4F4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut12 : 9; //lna gain in db for lna_index 12 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut12_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_GAIN_LUT_13 0x4F8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut13 : 9; //lna gain in db for lna_index 13 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaGainLut13_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B0_B4 0x500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsAbsThB0 : 8; //absolut threshold for for blocker in 1 db resolution for band 0, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB1 : 8; //absolut threshold for for blocker in 1 db resolution for band 1, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB2 : 8; //absolut threshold for for blocker in 1 db resolution for band 2, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB3 : 8; //absolut threshold for for blocker in 1 db resolution for band 3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsAbsThB0B4_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B5_B7 0x504 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsAbsThB4 : 8; //absolut threshold for for blocker in 1 db resolution for band 4, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB5 : 8; //absolut threshold for for blocker in 1 db resolution for band 5, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB6 : 8; //absolut threshold for for blocker in 1 db resolution for band 6, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB7 : 8; //absolut threshold for for blocker in 1 db resolution for band 7, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsAbsThB5B7_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_ABS_TH_B8_B11 0x508 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsAbsThB8 : 8; //absolut threshold for for blocker in 1 db resolution for band 8, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB9 : 8; //absolut threshold for for blocker in 1 db resolution for band 9, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB10 : 8; //absolut threshold for for blocker in 1 db resolution for band 10, reset value: 0x0, access type: RW
		uint32 pmBsAbsThB11 : 8; //absolut threshold for for blocker in 1 db resolution for band 11, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsAbsThB8B11_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B0_B4 0x50C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsOffThB0 : 8; //relative threshold for for blocker in 1 db resolution for band 0, reset value: 0x0, access type: RW
		uint32 pmBsOffThB1 : 8; //relative threshold for for blocker in 1 db resolution for band 1, reset value: 0x0, access type: RW
		uint32 pmBsOffThB2 : 8; //relative threshold for for blocker in 1 db resolution for band 2, reset value: 0x0, access type: RW
		uint32 pmBsOffThB3 : 8; //relative threshold for for blocker in 1 db resolution for band 3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsOffThB0B4_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B5_B7 0x510 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsOffThB4 : 8; //relative threshold for for blocker in 1 db resolution for band 4, reset value: 0x0, access type: RW
		uint32 pmBsOffThB5 : 8; //relative threshold for for blocker in 1 db resolution for band 5, reset value: 0x0, access type: RW
		uint32 pmBsOffThB6 : 8; //relative threshold for for blocker in 1 db resolution for band 6, reset value: 0x0, access type: RW
		uint32 pmBsOffThB7 : 8; //relative threshold for for blocker in 1 db resolution for band 7, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsOffThB5B7_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_BS_OFF_TH_B8_B11 0x514 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBsOffThB8 : 8; //relative threshold for for blocker in 1 db resolution for band 8, reset value: 0x0, access type: RW
		uint32 pmBsOffThB9 : 8; //relative threshold for for blocker in 1 db resolution for band 9, reset value: 0x0, access type: RW
		uint32 pmBsOffThB10 : 8; //relative threshold for for blocker in 1 db resolution for band 10, reset value: 0x0, access type: RW
		uint32 pmBsOffThB11 : 8; //relative threshold for for blocker in 1 db resolution for band 11, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyAgcAntPmBsOffThB8B11_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS0_B4 0x518 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accOffsetBs0 : 8; //offset to band select in 1/2 db resolution for band0, reset value: 0x0, access type: RO
		uint32 accOffsetBs1 : 8; //offset to band select in 1/2 db resolution for band1, reset value: 0x0, access type: RO
		uint32 accOffsetBs2 : 8; //offset to band select in 1/2 db resolution for band2, reset value: 0x0, access type: RO
		uint32 accOffsetBs3 : 8; //offset to band select in 1/2 db resolution for band3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyAgcAntAccOffsetBs0B4_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS4_B7 0x51C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accOffsetBs4 : 8; //offset to band select in 1/2 db resolution for band4, reset value: 0x0, access type: RO
		uint32 accOffsetBs5 : 8; //offset to band select in 1/2 db resolution for band5, reset value: 0x0, access type: RO
		uint32 accOffsetBs6 : 8; //offset to band select in 1/2 db resolution for band6, reset value: 0x0, access type: RO
		uint32 accOffsetBs7 : 8; //offset to band select in 1/2 db resolution for band7, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyAgcAntAccOffsetBs4B7_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_OFFSET_BS8_B11 0x520 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accOffsetBs8 : 8; //offset to band select in 1/2 db resolution for band8, reset value: 0x0, access type: RO
		uint32 accOffsetBs9 : 8; //offset to band select in 1/2 db resolution for band9, reset value: 0x0, access type: RO
		uint32 accOffsetBs10 : 8; //offset to band select in 1/2 db resolution for band10, reset value: 0x0, access type: RO
		uint32 accOffsetBs11 : 8; //offset to band select in 1/2 db resolution for band11, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyAgcAntAccOffsetBs8B11_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_ABS_BLOCKER_MAP 0x524 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accAbsBlockerMap : 32; //map of absolute threshold check, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyAgcAntAccAbsBlockerMap_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_TH_BLOCKER_MAP 0x528 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accThBlockerMap : 32; //map of relative threshold check, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyAgcAntAccThBlockerMap_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_MAX_BLOCKER_LSB_POWER 0x52C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accMaxBlockerLsbPower : 9; //max blocker power in lsb side in 1/2 db resolution, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccMaxBlockerLsbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_MAX_BLOCKER_USB_POWER 0x530 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accMaxBlockerUsbPower : 9; //max blocker power in usb side in 1/2 db resolution, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccMaxBlockerUsbPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_LNA_INTERNAL_GAIN 0x534 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaInternalGain : 9; //lna internal gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmLnaInternalGain_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PRI_ACI_POWER 0x538 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPriAciPower : 9; //programmble primary aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntPmPriAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ACC_PRI_ACI_POWER 0x53C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accPriAciPower : 9; //primary aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyAgcAntAccPriAciPower_u;

/*REG_B1_DFS_PHY_AGC_ANT_ULTRA_WIDE_BAND 0x540 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ultraWideBand : 1; //rf filter is open wide, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyAgcAntUltraWideBand_u;

/*REG_B1_DFS_PHY_AGC_ANT_PM_PRESET_CALIBRATION 0x544 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmPresetCalibrationLnaIndex : 4; //lna index for calibration, reset value: 0x0, access type: RW
		uint32 pmPresetCalibrationSelS2DRx : 1; //sel_s2d_rx for calibration, reset value: 0x0, access type: RW
		uint32 pmPresetCalibrationPgc1Index : 3; //pgc1 index for calibration, reset value: 0x0, access type: RW
		uint32 pmPresetCalibrationRxFilter : 6; //rx_filter for calibration, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyAgcAntPmPresetCalibration_u;



#endif // _B1_DFS_PHY_AGC_ANT_REGS_H_
