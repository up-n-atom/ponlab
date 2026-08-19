
/***********************************************************************************
File:				B0PhyAgcAnt2Regs.h
Module:				b0PhyAgcAnt2
SOC Revision:		latest
Generated at:       2022-05-04 10:57:16
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PHY_AGC_ANT2_REGS_H_
#define _B0_PHY_AGC_ANT2_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PHY_AGC_ANT2_BASE_ADDRESS                             MEMORY_MAP_UNIT_222_BASE_ADDRESS
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_PARAMS_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x0)
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_OFFSET2_REG                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4)
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_SYSTEM_GAIN_REG                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x8)
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_SYSTEM_GAIN_SELECT_REG           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xC)
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_SAT_THR                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x10)
#define	REG_B0_PHY_AGC_ANT2_INB_RSSI_PARAMS_REG                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x14)
#define	REG_B0_PHY_AGC_ANT2_INB_RSSI_OFFSET2_REG                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x18)
#define	REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SYSTEM_GAIN_REG              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1C)
#define	REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x20)
#define	REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SAT_THR                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x24)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_PARAMS_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x28)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_OFFSET2_REG                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2C)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_SYSTEM_GAIN_REG                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x30)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_SYSTEM_GAIN_SELECT_REG           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x34)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_SAT_THR                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x38)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_PARAMS_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3C)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_OFFSET2_REG                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x40)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_SYSTEM_GAIN_REG                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x44)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_SYSTEM_GAIN_SELECT_REG           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x48)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_SAT_THR                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_PARAMS_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x50)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_RX_RANGE0                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x54)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_RX_RANGE1                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x58)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_TX                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x5C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET2_RX_REG                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x60)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET2_TX_REG                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x64)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_SYSTEM_GAIN_REG                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x68)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_SYSTEM_GAIN_SELECT_REG           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x6C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_SAT_THR_RANGE0                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x70)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_SAT_THR_RANGE1                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x74)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A_RANGE0_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x78)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A_RANGE1_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x7C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A2_RANGE0_REG               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x80)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A2_RANGE1_REG               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x84)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_B_RANGE0_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x88)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_B_RANGE1_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x8C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_C_RANGE0_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x90)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_C_RANGE1_REG                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x94)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_NLOG_RANGE0_REG             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x98)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_NLOG_RANGE1_REG             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x9C)
#define	REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_A_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xA0)
#define	REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_A2_REG                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xA4)
#define	REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_B_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xA8)
#define	REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_C_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xAC)
#define	REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_NLOG_REG                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xB0)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_TX_RX_REG                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xB4)
#define	REG_B0_PHY_AGC_ANT2_BB_RSSI_REG                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xB8)
#define	REG_B0_PHY_AGC_ANT2_BB_POWER_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xBC)
#define	REG_B0_PHY_AGC_ANT2_BB_SRSSI_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xC0)
#define	REG_B0_PHY_AGC_ANT2_BB_SPOWER_REG                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xC4)
#define	REG_B0_PHY_AGC_ANT2_BB_PWR_DIFF_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xC8)
#define	REG_B0_PHY_AGC_ANT2_IB_RSSI_REG                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xCC)
#define	REG_B0_PHY_AGC_ANT2_IB_POWER_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xD0)
#define	REG_B0_PHY_AGC_ANT2_IB_SRSSI_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xD4)
#define	REG_B0_PHY_AGC_ANT2_IB_SPOWER_REG                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xD8)
#define	REG_B0_PHY_AGC_ANT2_IB_PWR_DIFF_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xDC)
#define	REG_B0_PHY_AGC_ANT2_FB_RSSI_REG                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xE0)
#define	REG_B0_PHY_AGC_ANT2_FB_POWER_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xE4)
#define	REG_B0_PHY_AGC_ANT2_FB_SRSSI_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xE8)
#define	REG_B0_PHY_AGC_ANT2_FB_SPOWER_REG                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xEC)
#define	REG_B0_PHY_AGC_ANT2_FB_PWR_DIFF_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xF0)
#define	REG_B0_PHY_AGC_ANT2_HB_RSSI_REG                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xF4)
#define	REG_B0_PHY_AGC_ANT2_HB_POWER_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xF8)
#define	REG_B0_PHY_AGC_ANT2_HB_SRSSI_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0xFC)
#define	REG_B0_PHY_AGC_ANT2_HB_SPOWER_REG                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x100)
#define	REG_B0_PHY_AGC_ANT2_HB_PWR_DIFF_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x104)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_REG                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x108)
#define	REG_B0_PHY_AGC_ANT2_RF_POWER_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x10C)
#define	REG_B0_PHY_AGC_ANT2_RF_SRSSI_REG                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x110)
#define	REG_B0_PHY_AGC_ANT2_RF_SPOWER_REG                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x114)
#define	REG_B0_PHY_AGC_ANT2_SATURATION_RESULT                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x118)
#define	REG_B0_PHY_AGC_ANT2_RSSI_BLOCKS_GCLK_EN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x11C)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_DEC_OUT_STS                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x120)
#define	REG_B0_PHY_AGC_ANT2_RF_RSSI_HIGH_RES_REG                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x124)
#define	REG_B0_PHY_AGC_ANT2_PM_RF_POWER                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x140)
#define	REG_B0_PHY_AGC_ANT2_PM_FB_POWER                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x144)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x148)
#define	REG_B0_PHY_AGC_ANT2_PM_HB_POWER                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x14C)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_POWER                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x150)
#define	REG_B0_PHY_AGC_ANT2_PM_ACI_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x154)
#define	REG_B0_PHY_AGC_ANT2_PM_RF_OFFSET                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x158)
#define	REG_B0_PHY_AGC_ANT2_PM_FB_OFFSET                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x15C)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_OFFSET                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x160)
#define	REG_B0_PHY_AGC_ANT2_PM_HB_OFFSET                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x164)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_OFFSET                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x168)
#define	REG_B0_PHY_AGC_ANT2_PM_ACI_OFFSET                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x16C)
#define	REG_B0_PHY_AGC_ANT2_PM_SATURATION_REG                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x170)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_DELAY                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x174)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SOURCES                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x178)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_MAX_POWER_LEVEL              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x17C)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_MIN_POWER_LEVEL              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x180)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SIGNAL_MAX_LEVEL             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x184)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_ENV_MIN_LEVEL                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x188)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SIGNAL_ENV_DIFF_VALUES       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x18C)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_GSP_TABLE_CONTROL            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x190)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_LNA_CONTROL                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x194)
#define	REG_B0_PHY_AGC_ANT2_PM_SEL_S2D_RX_LUT                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1B4)
#define	REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_OFFSET                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1B8)
#define	REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_LIMITS                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1BC)
#define	REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_SHIFT                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1C0)
#define	REG_B0_PHY_AGC_ANT2_PM_RX_FILTER_CONTROL                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1C4)
#define	REG_B0_PHY_AGC_ANT2_PM_RF_SYSTEM_GAIN_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1C8)
#define	REG_B0_PHY_AGC_ANT2_PM_FB_SYSTEM_GAIN_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1CC)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_SYSTEM_GAIN_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1D0)
#define	REG_B0_PHY_AGC_ANT2_PM_POST_BB_SYSTEM_GAIN_OFFSET            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1D4)
#define	REG_B0_PHY_AGC_ANT2_PM_HB_SYSTEM_GAIN_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1D8)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_SYSTEM_GAIN_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1DC)
#define	REG_B0_PHY_AGC_ANT2_PM_ACI_SYSTEM_GAIN_OFFSET                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1E0)
#define	REG_B0_PHY_AGC_ANT2_PM_CCA_SYSTEM_GAIN_OFFSET                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1E4)
#define	REG_B0_PHY_AGC_ANT2_PM_DET_SYSTEM_GAIN_OFFSET                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1E8)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_BOF_OFFSET                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1EC)
#define	REG_B0_PHY_AGC_ANT2_PM_HB_BOF_OFFSET                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1F0)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_RSSI_BOF_OFFSET                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1F4)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_ACI_BOF_OFFSET                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1F8)
#define	REG_B0_PHY_AGC_ANT2_PM_DET_BOF_OFFSET                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x1FC)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_BOF_TARGET                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x200)
#define	REG_B0_PHY_AGC_ANT2_PM_HB_BOF_TARGET                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x204)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_BOF_TARGET                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x208)
#define	REG_B0_PHY_AGC_ANT2_PM_DET_BOF_TARGET                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x20C)
#define	REG_B0_PHY_AGC_ANT2_PM_DIFI1_DB_GAIN                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x210)
#define	REG_B0_PHY_AGC_ANT2_PM_PRESET_LNA_PGC1                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x214)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_BB_DIGITAL_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x218)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_BB_DIGITAL_GAIN                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x21C)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_BB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x220)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_BB_DIGITAL_GAIN      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x224)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_HB_DIGITAL_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x228)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_HB_DIGITAL_GAIN                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x22C)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_HB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x230)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_HB_DIGITAL_GAIN      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x234)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_IB_DIGITAL_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x238)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_IB_DIGITAL_GAIN                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x23C)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_IB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x240)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_IB_DIGITAL_GAIN      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x244)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_DET_DIGITAL_GAIN                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x248)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_DET_DIGITAL_GAIN                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x24C)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_DET_DIGITAL_GAIN        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x250)
#define	REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_DET_DIGITAL_GAIN     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x254)
#define	REG_B0_PHY_AGC_ANT2_ACC_RF_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x258)
#define	REG_B0_PHY_AGC_ANT2_ACC_FB_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x25C)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x260)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x264)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_POWER                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x268)
#define	REG_B0_PHY_AGC_ANT2_ACC_ACI_POWER                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x26C)
#define	REG_B0_PHY_AGC_ANT2_ACC_SIGNAL_POWER                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x270)
#define	REG_B0_PHY_AGC_ANT2_ACC_ENV_POWER                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x274)
#define	REG_B0_PHY_AGC_ANT2_ACC_STATUS                               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x278)
#define	REG_B0_PHY_AGC_ANT2_ACC_RF_SYSTEM_GAIN                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x27C)
#define	REG_B0_PHY_AGC_ANT2_ACC_FB_SYSTEM_GAIN                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x280)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_SYSTEM_GAIN                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x284)
#define	REG_B0_PHY_AGC_ANT2_ACC_POST_BB_SYSTEM_GAIN                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x288)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_SYSTEM_GAIN                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x28C)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_SYSTEM_GAIN                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x290)
#define	REG_B0_PHY_AGC_ANT2_ACC_ACI_SYSTEM_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x294)
#define	REG_B0_PHY_AGC_ANT2_ACC_CCA_SYSTEM_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x298)
#define	REG_B0_PHY_AGC_ANT2_ACC_DET_SYSTEM_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x29C)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_DIGITAL_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2A0)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_DIGITAL_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2A4)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_DIGITAL_GAIN                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2A8)
#define	REG_B0_PHY_AGC_ANT2_ACC_DET_DIGITAL_GAIN                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2AC)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_BOF                               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2B0)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_TOTAL_DIGITAL_GAIN                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2B4)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_DIGITAL_BOF                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2B8)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_BOF                               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2BC)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_TOTAL_DIGITAL_GAIN                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2C0)
#define	REG_B0_PHY_AGC_ANT2_ACC_HB_DIGITAL_BOF                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2C4)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_BOF                               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2C8)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_TOTAL_DIGITAL_GAIN                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2CC)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_DIGITAL_BOF                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2D0)
#define	REG_B0_PHY_AGC_ANT2_ACC_DET_BOF                              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2D4)
#define	REG_B0_PHY_AGC_ANT2_ACC_DET_TOTAL_DIGITAL_GAIN               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2D8)
#define	REG_B0_PHY_AGC_ANT2_ACC_DET_DIGITAL_BOF                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2DC)
#define	REG_B0_PHY_AGC_ANT2_ACC_DC_OFFSET_I                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2E0)
#define	REG_B0_PHY_AGC_ANT2_ACC_DC_OFFSET_Q                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2E4)
#define	REG_B0_PHY_AGC_ANT2_PM_AGC_RAMS_RM                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2E8)
#define	REG_B0_PHY_AGC_ANT2_ACC_GAIN_VALUES                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2EC)
#define	REG_B0_PHY_AGC_ANT2_PM_FCSI_CONTROL                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2F0)
#define	REG_B0_PHY_AGC_ANT2_ACC_FCSI_VALUES                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2F8)
#define	REG_B0_PHY_AGC_ANT2_ACC_GAIN_ACC_CONTROL                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x2FC)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_CODE_ACC_THR                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x300)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_CODE_ACC_THR                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x304)
#define	REG_B0_PHY_AGC_ANT2_PM_LONGTERM_RELEASE_THR                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x308)
#define	REG_B0_PHY_AGC_ANT2_PM_LONGTERM_THR                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x30C)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER_FAST_LOW_THR                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x310)
#define	REG_B0_PHY_AGC_ANT2_PM_DELTA_CODE_ACC_THR                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x314)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER_UP_THR                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x318)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER_SLOW_LOW_THR                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x31C)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_POWER_UP_THR                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x320)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_POWER_SLOW_LOW_THR                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x324)
#define	REG_B0_PHY_AGC_ANT2_PM_SET_CODE_ACC_THR_CTRL                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x328)
#define	REG_B0_PHY_AGC_ANT2_PM_ALGO_CTRL                             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x32C)
#define	REG_B0_PHY_AGC_ANT2_ACC_EVENT_STATUS                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x330)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_TH_SET_INC_GAP_DB                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x334)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_TH_SET_DEC_GAP_DB                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x338)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_TH_UP_EXTRA_GAP_DB                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x33C)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER_UP_DEF_THR                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x340)
#define	REG_B0_PHY_AGC_ANT2_PM_BB_POWER_SLOW_LOW_MIN_THR             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x344)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_TH_SET_INC_GAP_DB                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x348)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_TH_SET_DEC_GAP_DB                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x34C)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_TH_UP_EXTRA_GAP_DB                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x350)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_POWER_UP_DEF_THR                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x354)
#define	REG_B0_PHY_AGC_ANT2_PM_IB_POWER_SLOW_LOW_MIN_THR             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x358)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_POWER_UP_THR                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x35C)
#define	REG_B0_PHY_AGC_ANT2_ACC_BB_POWER_SLOW_LOW_THR                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x360)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_POWER_UP_THR                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x364)
#define	REG_B0_PHY_AGC_ANT2_ACC_IB_POWER_SLOW_LOW_THR                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x368)
#define	REG_B0_PHY_AGC_ANT2_CALC_AGC_GAIN_WORD                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x36C)
#define	REG_B0_PHY_AGC_ANT2_CALC_AGC_STM_CLEAR_CONTROL               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x370)
#define	REG_B0_PHY_AGC_ANT2_CALC_ACC_AUTO_CLOSE_MODE                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x374)
#define	REG_B0_PHY_AGC_ANT2_CALC_POWER_THR_ACC_GO                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x378)
#define	REG_B0_PHY_AGC_ANT2_CALC_AGC_GAIN_CANCEL                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x37C)
#define	REG_B0_PHY_AGC_ANT2_DISABLE_PERIPHERALS                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x380)
#define	REG_B0_PHY_AGC_ANT2_AGC_STM_RDBACK                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x384)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_BB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x388)
#define	REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_BB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x38C)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_HB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x390)
#define	REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_HB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x394)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_IB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x398)
#define	REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_IB_DIGITAL_GAIN         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x39C)
#define	REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_DET_DIGITAL_GAIN        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3A0)
#define	REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_DET_DIGITAL_GAIN        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3A4)
#define	REG_B0_PHY_AGC_ANT2_BLOCKER_CONTROL                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3C0)
#define	REG_B0_PHY_AGC_ANT2_PM_BLOCKER_TH                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3C4)
#define	REG_B0_PHY_AGC_ANT2_PM_ALGO_CTRL_SEL                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3D0)
#define	REG_B0_PHY_AGC_ANT2_ACC_BLOCKER_ADDR                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x3D4)
#define	REG_B0_PHY_AGC_ANT2_UP_INB_RSSI_PARAMS_REG                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x400)
#define	REG_B0_PHY_AGC_ANT2_UP_INB_RSSI_OFFSET2_REG                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x404)
#define	REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SYSTEM_GAIN_REG           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x408)
#define	REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x40C)
#define	REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SAT_THR                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x410)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_PARAMS_REG                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x414)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_OFFSET2_REG                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x418)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SYSTEM_GAIN_REG               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x41C)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SYSTEM_GAIN_SELECT_REG        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x420)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SAT_THR                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x424)
#define	REG_B0_PHY_AGC_ANT2_UP_IB_RSSI_REG                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x428)
#define	REG_B0_PHY_AGC_ANT2_UP_IB_POWER_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x42C)
#define	REG_B0_PHY_AGC_ANT2_UP_IB_SRSSI_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x430)
#define	REG_B0_PHY_AGC_ANT2_UP_IB_SPOWER_REG                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x434)
#define	REG_B0_PHY_AGC_ANT2_UP_IB_PWR_DIFF_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x438)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_REG                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x43C)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_POWER_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x440)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_SRSSI_REG                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x444)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_SPOWER_REG                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x448)
#define	REG_B0_PHY_AGC_ANT2_UP_HB_PWR_DIFF_REG                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x44C)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_HB_POWER                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x450)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_IB_POWER                           (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x454)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_ACI_POWER                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x458)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_POWER                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x45C)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_POWER                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x460)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_ACI_POWER                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x464)
#define	REG_B0_PHY_AGC_ANT2_UP_CONTROL                               (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x468)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_HB_SYSTEM_GAIN_OFFSET              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x46C)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_IB_SYSTEM_GAIN_OFFSET              (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x470)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_ACI_SYSTEM_GAIN_OFFSET             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x474)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_CCA_SYSTEM_GAIN_OFFSET             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x478)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_HB_BOF_OFFSET                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x47C)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_IB_RSSI_BOF_OFFSET                 (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x480)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_IB_ACI_BOF_OFFSET                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x484)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_HB_BOF_TARGET                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x488)
#define	REG_B0_PHY_AGC_ANT2_PM_UP_IB_BOF_TARGET                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x48C)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_SYSTEM_GAIN                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x490)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_SYSTEM_GAIN                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x494)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_ACI_SYSTEM_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x498)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_CCA_SYSTEM_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x49C)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_DIGITAL_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4A0)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_DIGITAL_GAIN                   (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4A4)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_BOF                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4A8)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_TOTAL_DIGITAL_GAIN             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4AC)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_HB_DIGITAL_BOF                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4B0)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_BOF                            (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4B4)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_TOTAL_DIGITAL_GAIN             (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4B8)
#define	REG_B0_PHY_AGC_ANT2_ACC_UP_IB_DIGITAL_BOF                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4BC)
#define	REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SAT_SOURCES                  (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4C0)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_0                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4C4)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_1                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4C8)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_2                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4CC)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_3                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4D0)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_4                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4D4)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_5                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4D8)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_6                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4DC)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_7                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4E0)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_8                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4E4)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_9                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4E8)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_10                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4EC)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_11                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4F0)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_12                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4F4)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_13                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x4F8)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B0_B4                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x500)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B5_B7                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x504)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B8_B11                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x508)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B0_B4                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x50C)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B5_B7                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x510)
#define	REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B8_B11                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x514)
#define	REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS0_B4                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x518)
#define	REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS4_B7                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x51C)
#define	REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS8_B11                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x520)
#define	REG_B0_PHY_AGC_ANT2_ACC_ABS_BLOCKER_MAP                      (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x524)
#define	REG_B0_PHY_AGC_ANT2_ACC_TH_BLOCKER_MAP                       (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x528)
#define	REG_B0_PHY_AGC_ANT2_ACC_MAX_BLOCKER_LSB_POWER                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x52C)
#define	REG_B0_PHY_AGC_ANT2_ACC_MAX_BLOCKER_USB_POWER                (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x530)
#define	REG_B0_PHY_AGC_ANT2_PM_LNA_INTERNAL_GAIN                     (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x534)
#define	REG_B0_PHY_AGC_ANT2_PM_PRI_ACI_POWER                         (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x538)
#define	REG_B0_PHY_AGC_ANT2_ACC_PRI_ACI_POWER                        (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x53C)
#define	REG_B0_PHY_AGC_ANT2_ULTRA_WIDE_BAND                          (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x540)
#define	REG_B0_PHY_AGC_ANT2_PM_PRESET_CALIBRATION                    (B0_PHY_AGC_ANT2_BASE_ADDRESS + 0x544)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PHY_AGC_ANT2_BB_RSSI_PARAMS_REG 0x0 */
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
} RegB0PhyAgcAnt2BbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_RSSI_OFFSET2_REG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiOffset2 : 9; //bb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_BB_RSSI_SYSTEM_GAIN_REG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiSystemGain : 9; //bb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_RSSI_SYSTEM_GAIN_SELECT_REG 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbRssiSystemGainHwSelect : 1; //bb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2BbRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_RSSI_SAT_THR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssiSatThr : 9; //bb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_INB_RSSI_PARAMS_REG 0x14 */
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
} RegB0PhyAgcAnt2InbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_INB_RSSI_OFFSET2_REG 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiOffset2 : 9; //inband rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2InbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SYSTEM_GAIN_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiSystemGain : 9; //inband rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2InbandRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 inbandRssiSystemGainHwSelect : 1; //inband rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2InbandRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_INBAND_RSSI_SAT_THR 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		int32  inbandRssiSatThr : 9; //inband rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2InbandRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_PARAMS_REG 0x28 */
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
} RegB0PhyAgcAnt2FbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_OFFSET2_REG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiOffset2 : 9; //full band rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_SYSTEM_GAIN_REG 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiSystemGain : 9; //full band rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_SYSTEM_GAIN_SELECT_REG 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fbRssiSystemGainHwSelect : 1; //full band rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2FbRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_SAT_THR 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssiSatThr : 9; //full band rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_PARAMS_REG 0x3C */
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
} RegB0PhyAgcAnt2HbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_OFFSET2_REG 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiOffset2 : 9; //hb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_SYSTEM_GAIN_REG 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiSystemGain : 9; //hb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_SYSTEM_GAIN_SELECT_REG 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbRssiSystemGainHwSelect : 1; //hb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2HbRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_SAT_THR 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssiSatThr : 9; //hb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_PARAMS_REG 0x50 */
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
} RegB0PhyAgcAnt2RfRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_RX_RANGE0 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1RxRange0 : 14; //rf rssi offset1, used for dc cancellation in rx range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiOffset1RxRange0_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_RX_RANGE1 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1RxRange1 : 14; //rf rssi offset1, used for dc cancellation in rx range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiOffset1RxRange1_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET1_TX 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset1Tx : 14; //rf rssi offset1, used for dc cancellation in tx, reset value: 0x0, access type: RWS
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiOffset1Tx_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET2_RX_REG 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset2Rx : 8; //rf rssi offset2, used for digital top power dBm conversion, RX mode, reset value: 0x0, access type: RWS
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiOffset2RxReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_OFFSET2_TX_REG 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiOffset2Tx : 8; //rf rssi offset2, used for digital top power dBm conversion, TX mode, reset value: 0x0, access type: RWS
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiOffset2TxReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_SYSTEM_GAIN_REG 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSystemGain : 9; //rf rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_SYSTEM_GAIN_SELECT_REG 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiSystemGainHwSelect : 1; //rf  rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_SAT_THR_RANGE0 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSatThrRange0 : 9; //rf rssi saturation threshold for range 0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiSatThrRange0_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_SAT_THR_RANGE1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiSatThrRange1 : 9; //rf rssi saturation threshold for range 1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiSatThrRange1_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A_RANGE0_REG 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefARange0 : 16; //rf rssi coef A range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefARange0Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A_RANGE1_REG 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefARange1 : 16; //rf rssi coef A range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefARange1Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A2_RANGE0_REG 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefA2Range0 : 16; //rf rssi coef A2 range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefA2Range0Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_A2_RANGE1_REG 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefA2Range1 : 16; //rf rssi coef A2 range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefA2Range1Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_B_RANGE0_REG 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefBRange0 : 16; //rf rssi coef B range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefBRange0Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_B_RANGE1_REG 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefBRange1 : 16; //rf rssi coef B range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefBRange1Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_C_RANGE0_REG 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefCRange0 : 16; //rf rssi coef C range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefCRange0Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_C_RANGE1_REG 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefCRange1 : 16; //rf rssi coef C range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefCRange1Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_NLOG_RANGE0_REG 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefNlogRange0 : 16; //rf rssi coef Nlog range0, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefNlogRange0Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_COEF_NLOG_RANGE1_REG 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiCoefNlogRange1 : 16; //rf rssi coef Nlog range1, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiCoefNlogRange1Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_A_REG 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefA : 16; //rf_tssi_coef_A, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfTssiCoefAReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_A2_REG 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefA2 : 16; //rf_tssi_coef_A2, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfTssiCoefA2Reg_u;

/*REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_B_REG 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefB : 16; //rf_tssi_coef_B, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfTssiCoefBReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_C_REG 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefC : 16; //rf_tssi_coef_C, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfTssiCoefCReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_TSSI_COEF_NLOG_REG 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfTssiCoefNlog : 16; //rf_tssi_coef_Nlog, reset value: 0x0, access type: RWS
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2RfTssiCoefNlogReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_TX_RX_REG 0xB4 */
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
} RegB0PhyAgcAnt2RfRssiTxRxReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_RSSI_REG 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbRssi : 9; //BB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_POWER_REG 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbPower : 9; //BB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_SRSSI_REG 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbSrssi : 9; //BB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_SPOWER_REG 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  bbSpower : 9; //BB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2BbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_BB_PWR_DIFF_REG 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbB1 : 9; //BB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 bbB2 : 9; //BB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2BbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_IB_RSSI_REG 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibRssi : 9; //IB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2IbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_IB_POWER_REG 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibPower : 9; //IB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2IbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_IB_SRSSI_REG 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibSrssi : 9; //IB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2IbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_IB_SPOWER_REG 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  ibSpower : 9; //IB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2IbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_IB_PWR_DIFF_REG 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ibB1 : 9; //IB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 ibB2 : 9; //IB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2IbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_RSSI_REG 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbRssi : 9; //FB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_POWER_REG 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbPower : 9; //FB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_SRSSI_REG 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbSrssi : 9; //FB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_SPOWER_REG 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		int32  fbSpower : 9; //FB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2FbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_FB_PWR_DIFF_REG 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fbB1 : 9; //FB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 fbB2 : 9; //FB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2FbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_RSSI_REG 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbRssi : 9; //HB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_POWER_REG 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbPower : 9; //HB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_SRSSI_REG 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbSrssi : 9; //HB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_SPOWER_REG 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		int32  hbSpower : 9; //HB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2HbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_HB_PWR_DIFF_REG 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hbB1 : 9; //HB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 hbB2 : 9; //HB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2HbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_REG 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssi : 9; //RF RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_POWER_REG 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfPower : 9; //RF power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_SRSSI_REG 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfSrssi : 9; //RF RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_RF_SPOWER_REG 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfSpower : 9; //RF Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2RfSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_SATURATION_RESULT 0x118 */
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
} RegB0PhyAgcAnt2SaturationResult_u;

/*REG_B0_PHY_AGC_ANT2_RSSI_BLOCKS_GCLK_EN 0x11C */
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
} RegB0PhyAgcAnt2RssiBlocksGclkEn_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_DEC_OUT_STS 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rfRssiDecOutSts : 14; //rf rssi decimator output for dc measurement, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiDecOutSts_u;

/*REG_B0_PHY_AGC_ANT2_RF_RSSI_HIGH_RES_REG 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		int32  rfRssiHighRes : 10; //RF RSSI output in 1/4 db resolution , reset value: 0x0, access type: ROS
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PhyAgcAnt2RfRssiHighResReg_u;

/*REG_B0_PHY_AGC_ANT2_PM_RF_POWER 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfPower : 9; //programmble rf power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmRfPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_FB_POWER 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbPower : 9; //programmble fb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmFbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPower : 9; //programmble bb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_HB_POWER 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbPower : 9; //programmble hb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmHbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_POWER 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPower : 9; //programmble ib power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACI_POWER 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciPower : 9; //programmble aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAciPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_RF_OFFSET 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfOffset : 9; //offset for rf power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmRfOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_FB_OFFSET 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbOffset : 9; //offset for fb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmFbOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_OFFSET 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbOffset : 9; //offset for bb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_HB_OFFSET 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbOffset : 9; //offset for hb power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmHbOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_OFFSET 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbOffset : 9; //offset for ib power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACI_OFFSET 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciOffset : 9; //offset for aci power gain acceleration (may be used for dBv to dBm) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAciOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_SATURATION_REG 0x170 */
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
} RegB0PhyAgcAnt2PmSaturationReg_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_DELAY 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainDelay : 16; //delay between table go to start acceleration, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainDelay_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SOURCES 0x178 */
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
} RegB0PhyAgcAnt2PmAccGainSources_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_MAX_POWER_LEVEL 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainMaxPowerLevel : 9; //acc gain HW limit max power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainMaxPowerLevel_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_MIN_POWER_LEVEL 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainMinPowerLevel : 9; //acc gain HW limit min power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainMinPowerLevel_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SIGNAL_MAX_LEVEL 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainSignalMaxLevel : 9; //acc gain HW limit for BB is RF  in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainSignalMaxLevel_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_ENV_MIN_LEVEL 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAccGainEnvMinLevel : 9; //acc gain HW limit for RF is BB in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainEnvMinLevel_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SIGNAL_ENV_DIFF_VALUES 0x18C */
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
} RegB0PhyAgcAnt2PmAccGainSignalEnvDiffValues_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_GSP_TABLE_CONTROL 0x190 */
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
} RegB0PhyAgcAnt2PmAccGainGspTableControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_LNA_CONTROL 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAccGainLnaIndexMode : 1; //lna_target or switching point, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2PmAccGainLnaControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_SEL_S2D_RX_LUT 0x1B4 */
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
} RegB0PhyAgcAnt2PmSelS2DRxLut_u;

/*REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_OFFSET 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPgc1GainOffset : 9; //pgc1 target power offset in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmPgc1GainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_LIMITS 0x1BC */
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
} RegB0PhyAgcAnt2PmPgc1GainLimits_u;

/*REG_B0_PHY_AGC_ANT2_PM_PGC1_GAIN_SHIFT 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmPgc1GainShift : 3; //pgc1 index shift to convert from gain to index, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyAgcAnt2PmPgc1GainShift_u;

/*REG_B0_PHY_AGC_ANT2_PM_RX_FILTER_CONTROL 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pmRxFilterBwInterferenceMode : 1; //rx_filter_bw interference_mode: 0-no interference, 1-w/ interference. Different filter bw for int, reset value: 0x0, access type: RW
		uint32 reserved1 : 29;
	} bitFields;
} RegB0PhyAgcAnt2PmRxFilterControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_RF_SYSTEM_GAIN_OFFSET 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmRfSystemGainOffset : 9; //offset for rf gain acceleration (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmRfSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_FB_SYSTEM_GAIN_OFFSET 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmFbSystemGainOffset : 9; //offset for fb gain acceleration (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmFbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_SYSTEM_GAIN_OFFSET 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbSystemGainOffset : 9; //offset for bb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_POST_BB_SYSTEM_GAIN_OFFSET 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPostBbSystemGainOffset : 9; //offset for pre bb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmPostBbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_HB_SYSTEM_GAIN_OFFSET 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbSystemGainOffset : 9; //offset for hb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmHbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_SYSTEM_GAIN_OFFSET 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbSystemGainOffset : 9; //offset for ib gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACI_SYSTEM_GAIN_OFFSET 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmAciSystemGainOffset : 9; //offset for aci gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmAciSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_CCA_SYSTEM_GAIN_OFFSET 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmCcaSystemGainOffset : 9; //offset for cca gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmCcaSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_DET_SYSTEM_GAIN_OFFSET 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetSystemGainOffset : 9; //offset for det gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmDetSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_BOF_OFFSET 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbBofOffset : 9; //convert bb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_HB_BOF_OFFSET 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbBofOffset : 9; //convert hb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmHbBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_RSSI_BOF_OFFSET 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbRssiBofOffset : 9; //convert ib rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbRssiBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_ACI_BOF_OFFSET 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbAciBofOffset : 9; //convert ib aci power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbAciBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_DET_BOF_OFFSET 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetBofOffset : 9; //convert det power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmDetBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_BOF_TARGET 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbBofTarget : 9; //target backoff from full scale at bb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_PM_HB_BOF_TARGET 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmHbBofTarget : 9; //target backoff from full scale at hb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmHbBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_BOF_TARGET 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbBofTarget : 9; //target backoff from full scale at ib rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_PM_DET_BOF_TARGET 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDetBofTarget : 9; //target backoff from full scale at det rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmDetBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_PM_DIFI1_DB_GAIN 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmDifi1DbGain : 9; //difi1 gain in 1/2 dB must be equal to the linear gain of pm_difi1_gain , reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmDifi1DbGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_PRESET_LNA_PGC1 0x214 */
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
} RegB0PhyAgcAnt2PmPresetLnaPgc1_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_BB_DIGITAL_GAIN 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxBbDigitalGain : 9; //max gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_BB_DIGITAL_GAIN 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1BbDigitalGain : 9; //step1 env & signal are saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1BbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_BB_DIGITAL_GAIN 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatBbDigitalGain : 9; //step1 env only is saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1EnvSatBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_BB_DIGITAL_GAIN 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatBbDigitalGain : 9; //step1 signal only is saturated gain bb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1SignalSatBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_HB_DIGITAL_GAIN 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxHbDigitalGain : 9; //max gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_HB_DIGITAL_GAIN 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1HbDigitalGain : 9; //step1 env & signal are saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1HbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_HB_DIGITAL_GAIN 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatHbDigitalGain : 9; //step1 env only is saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1EnvSatHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_HB_DIGITAL_GAIN 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatHbDigitalGain : 9; //step1 signal only is saturated gain hb digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1SignalSatHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_IB_DIGITAL_GAIN 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxIbDigitalGain : 9; //max gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_IB_DIGITAL_GAIN 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1IbDigitalGain : 9; //step1 env & signal are saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1IbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_IB_DIGITAL_GAIN 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatIbDigitalGain : 9; //step1 env only is saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1EnvSatIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_IB_DIGITAL_GAIN 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatIbDigitalGain : 9; //step1 signal only is saturated, gain ib digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1SignalSatIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_DET_DIGITAL_GAIN 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxDetDigitalGain : 9; //max gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_DET_DIGITAL_GAIN 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1DetDigitalGain : 9; //step1 env & signal are saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1DetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_ENV_SAT_DET_DIGITAL_GAIN 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1EnvSatDetDigitalGain : 9; //step1 env only is saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1EnvSatDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_STEP1_SIGNAL_SAT_DET_DIGITAL_GAIN 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmStep1SignalSatDetDigitalGain : 9; //step1 signal only is saturated, gain det digital gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmStep1SignalSatDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_RF_POWER 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accRfPower : 9; //rf power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccRfPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_FB_POWER 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accFbPower : 9; //fb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccFbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_POWER 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPower : 9; //bb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_POWER 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbPower : 9; //hb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_POWER 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPower : 9; //ib power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_ACI_POWER 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accAciPower : 9; //aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccAciPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_SIGNAL_POWER 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accSignalPower : 9; //signal power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccSignalPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_ENV_POWER 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accEnvPower : 9; //env power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccEnvPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_STATUS 0x278 */
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
} RegB0PhyAgcAnt2AccStatus_u;

/*REG_B0_PHY_AGC_ANT2_ACC_RF_SYSTEM_GAIN 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accRfSystemGain : 9; //rf system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccRfSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_FB_SYSTEM_GAIN 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accFbSystemGain : 9; //fb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccFbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_SYSTEM_GAIN 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbSystemGain : 9; //bb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_POST_BB_SYSTEM_GAIN 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accPostBbSystemGain : 9; //pre bb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccPostBbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_SYSTEM_GAIN 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbSystemGain : 9; //hb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_SYSTEM_GAIN 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbSystemGain : 9; //ib system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_ACI_SYSTEM_GAIN 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accAciSystemGain : 9; //aci system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccAciSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_CCA_SYSTEM_GAIN 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accCcaSystemGain : 9; //cca system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccCcaSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DET_SYSTEM_GAIN 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetSystemGain : 9; //det system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccDetSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_DIGITAL_GAIN 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbDigitalGain : 9; //bb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_DIGITAL_GAIN 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbDigitalGain : 9; //hb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_DIGITAL_GAIN 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbDigitalGain : 9; //ib digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DET_DIGITAL_GAIN 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetDigitalGain : 9; //det digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_BOF 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbBof : 9; //bb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_TOTAL_DIGITAL_GAIN 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbTotalDigitalGain : 9; //bb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_DIGITAL_BOF 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbDigitalBof : 9; //bb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_BOF 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbBof : 9; //hb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_TOTAL_DIGITAL_GAIN 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbTotalDigitalGain : 9; //hb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_HB_DIGITAL_BOF 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accHbDigitalBof : 9; //hb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccHbDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_BOF 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbBof : 9; //ib backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_TOTAL_DIGITAL_GAIN 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbTotalDigitalGain : 9; //sum of ib digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_DIGITAL_BOF 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbDigitalBof : 9; //ib_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DET_BOF 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetBof : 9; //det backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccDetBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DET_TOTAL_DIGITAL_GAIN 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetTotalDigitalGain : 9; //sum of det digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccDetTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DET_DIGITAL_BOF 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDetDigitalBof : 9; //det_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccDetDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DC_OFFSET_I 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDcOffsetI : 14; //chosen dc Q offset according to bw,pgc1_index,lna_index, reset value: 0x0, access type: ROS
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2AccDcOffsetI_u;

/*REG_B0_PHY_AGC_ANT2_ACC_DC_OFFSET_Q 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accDcOffsetQ : 14; //chosen dc I offset according to bw,pgc1_index,lna_index, reset value: 0x0, access type: ROS
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyAgcAnt2AccDcOffsetQ_u;

/*REG_B0_PHY_AGC_ANT2_PM_AGC_RAMS_RM 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAgcTableRm : 3; //agc_gsp_table_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyAgcAnt2PmAgcRamsRm_u;

/*REG_B0_PHY_AGC_ANT2_ACC_GAIN_VALUES 0x2EC */
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
} RegB0PhyAgcAnt2AccGainValues_u;

/*REG_B0_PHY_AGC_ANT2_PM_FCSI_CONTROL 0x2F0 */
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
} RegB0PhyAgcAnt2PmFcsiControl_u;

/*REG_B0_PHY_AGC_ANT2_ACC_FCSI_VALUES 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accFcsiData : 16; //data which is sent to rfic via fcsi, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2AccFcsiValues_u;

/*REG_B0_PHY_AGC_ANT2_ACC_GAIN_ACC_CONTROL 0x2FC */
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
} RegB0PhyAgcAnt2AccGainAccControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_CODE_ACC_THR 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmIbTimerThr : 16; //pm_ib_timer_thr, reset value: 0x0, access type: RW
		uint32 pmIbReleaseThr : 16; //pm_ib_release_thr, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyAgcAnt2PmIbCodeAccThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_CODE_ACC_THR 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBbTimerThr : 16; //pm_bb_timer_thr, reset value: 0x0, access type: RW
		uint32 pmBbReleaseThr : 16; //pm_bb_release_thr, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyAgcAnt2PmBbCodeAccThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_LONGTERM_RELEASE_THR 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLongtermReleaseThr : 16; //pm_longterm_release_thr, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2PmLongtermReleaseThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_LONGTERM_THR 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLongtermThr : 9; //pm_longterm_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLongtermThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER_FAST_LOW_THR 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerFastLowThr : 9; //pm_bb_power_fast_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPowerFastLowThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_DELTA_CODE_ACC_THR 0x314 */
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
} RegB0PhyAgcAnt2PmDeltaCodeAccThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER_UP_THR 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerUpThr : 9; //pm_bb_power_up_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPowerUpThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER_SLOW_LOW_THR 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerSlowLowThr : 9; //pm_bb_power_slow_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPowerSlowLowThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_POWER_UP_THR 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerUpThr : 9; //pm_ib_power_up_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbPowerUpThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_POWER_SLOW_LOW_THR 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerSlowLowThr : 9; //pm_ib_power_slow_low_thr, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbPowerSlowLowThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_SET_CODE_ACC_THR_CTRL 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSetBbThrCtrl : 1; //set_bb_thr_ctrl: 1 -from pm, 0- from hw accelerator, reset value: 0x0, access type: RW
		uint32 pmSetIbThrCtrl : 1; //set_ib_thr_ctrl: 1 -from pm, 0- from hw accelerator, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyAgcAnt2PmSetCodeAccThrCtrl_u;

/*REG_B0_PHY_AGC_ANT2_PM_ALGO_CTRL 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAlgoCtrl : 16; //code acclerator algo ctrl, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyAgcAnt2PmAlgoCtrl_u;

/*REG_B0_PHY_AGC_ANT2_ACC_EVENT_STATUS 0x330 */
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
} RegB0PhyAgcAnt2AccEventStatus_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_TH_SET_INC_GAP_DB 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThSetIncGapDb : 9; //margin to add above measured bb power to calculate energy increase threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbThSetIncGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_TH_SET_DEC_GAP_DB 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThSetDecGapDb : 9; //margin to substract below measured bb power to calculate energy deccrease threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbThSetDecGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_TH_UP_EXTRA_GAP_DB 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbThUpExtraGapDb : 9; //margin used inorder to limit enery increase threhold for high bb power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbThUpExtraGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER_UP_DEF_THR 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerUpDefThr : 9; //default energy increases bb power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPowerUpDefThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_BB_POWER_SLOW_LOW_MIN_THR 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmBbPowerSlowLowMinThr : 9; //default energy decrease bb power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmBbPowerSlowLowMinThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_TH_SET_INC_GAP_DB 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThSetIncGapDb : 9; //margin to add above measured ib power to calculate energy increase threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbThSetIncGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_TH_SET_DEC_GAP_DB 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThSetDecGapDb : 9; //margin to substract below measured ib power to calculate energy deccrease threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbThSetDecGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_TH_UP_EXTRA_GAP_DB 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbThUpExtraGapDb : 9; //margin used inorder to limit enery increase threhold for high ib power in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbThUpExtraGapDb_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_POWER_UP_DEF_THR 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerUpDefThr : 9; //default energy increases ib power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbPowerUpDefThr_u;

/*REG_B0_PHY_AGC_ANT2_PM_IB_POWER_SLOW_LOW_MIN_THR 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmIbPowerSlowLowMinThr : 9; //default energy decrease ib power threshold in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmIbPowerSlowLowMinThr_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_POWER_UP_THR 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPowerUpThr : 9; //bb energy increase threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbPowerUpThr_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BB_POWER_SLOW_LOW_THR 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accBbPowerSlowLowThr : 9; //bb energy deccrease threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccBbPowerSlowLowThr_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_POWER_UP_THR 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPowerUpThr : 9; //ib energy increase threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbPowerUpThr_u;

/*REG_B0_PHY_AGC_ANT2_ACC_IB_POWER_SLOW_LOW_THR 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accIbPowerSlowLowThr : 9; //ib energy deccrease threshold acceleration result, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccIbPowerSlowLowThr_u;

/*REG_B0_PHY_AGC_ANT2_CALC_AGC_GAIN_WORD 0x36C */
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
} RegB0PhyAgcAnt2CalcAgcGainWord_u;

/*REG_B0_PHY_AGC_ANT2_CALC_AGC_STM_CLEAR_CONTROL 0x370 */
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
} RegB0PhyAgcAnt2CalcAgcStmClearControl_u;

/*REG_B0_PHY_AGC_ANT2_CALC_ACC_AUTO_CLOSE_MODE 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcAccAutoCloseMode : 1; //enables code acceleration, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2CalcAccAutoCloseMode_u;

/*REG_B0_PHY_AGC_ANT2_CALC_POWER_THR_ACC_GO 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcBbPowerThrAccGo : 1; //calc_bb_power_thr_acc_go, reset value: 0x0, access type: WO
		uint32 calcIbPowerThrAccGo : 1; //calc_ib_power_thr_acc_go, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyAgcAnt2CalcPowerThrAccGo_u;

/*REG_B0_PHY_AGC_ANT2_CALC_AGC_GAIN_CANCEL 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcAgcGainCancel : 1; //cancels gain acceleration timer and cancels table go, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2CalcAgcGainCancel_u;

/*REG_B0_PHY_AGC_ANT2_DISABLE_PERIPHERALS 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disablePeripherals : 1; //disable_peripherals, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2DisablePeripherals_u;

/*REG_B0_PHY_AGC_ANT2_AGC_STM_RDBACK 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agcStmRdback : 6; //agc_stm_rdback, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyAgcAnt2AgcStmRdback_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_BB_DIGITAL_GAIN 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccBbDigitalGain : 9; //max limit for bb digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxLimitAccBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_BB_DIGITAL_GAIN 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccBbDigitalGain : 9; //min limit for bb digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMinLimitAccBbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_HB_DIGITAL_GAIN 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccHbDigitalGain : 9; //max limit for hb digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxLimitAccHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_HB_DIGITAL_GAIN 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccHbDigitalGain : 9; //min limit for hb digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMinLimitAccHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_IB_DIGITAL_GAIN 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccIbDigitalGain : 9; //max limit for ib digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxLimitAccIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_IB_DIGITAL_GAIN 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccIbDigitalGain : 9; //min limit for ib digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMinLimitAccIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MAX_LIMIT_ACC_DET_DIGITAL_GAIN 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMaxLimitAccDetDigitalGain : 9; //max limit for det digital gain after acceleration, reset value: 0x54, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMaxLimitAccDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_MIN_LIMIT_ACC_DET_DIGITAL_GAIN 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmMinLimitAccDetDigitalGain : 9; //min limit for det digital gain after acceleration, reset value: 0x1F4, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmMinLimitAccDetDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_BLOCKER_CONTROL 0x3C0 */
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
} RegB0PhyAgcAnt2BlockerControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_BLOCKER_TH 0x3C4 */
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
} RegB0PhyAgcAnt2PmBlockerTh_u;

/*REG_B0_PHY_AGC_ANT2_PM_ALGO_CTRL_SEL 0x3D0 */
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
} RegB0PhyAgcAnt2PmAlgoCtrlSel_u;

/*REG_B0_PHY_AGC_ANT2_ACC_BLOCKER_ADDR 0x3D4 */
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
} RegB0PhyAgcAnt2AccBlockerAddr_u;

/*REG_B0_PHY_AGC_ANT2_UP_INB_RSSI_PARAMS_REG 0x400 */
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
} RegB0PhyAgcAnt2UpInbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_INB_RSSI_OFFSET2_REG 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiOffset2 : 9; //up inband rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpInbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SYSTEM_GAIN_REG 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiSystemGain : 9; //up inband rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpInbandRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SYSTEM_GAIN_SELECT_REG 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upInbandRssiSystemGainHwSelect : 1; //up inband rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2UpInbandRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_INBAND_RSSI_SAT_THR 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upInbandRssiSatThr : 9; //up inband rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpInbandRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_PARAMS_REG 0x414 */
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
} RegB0PhyAgcAnt2UpHbRssiParamsReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_OFFSET2_REG 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiOffset2 : 9; //up hb rssi offset2, used for digital to power dBm conversion, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbRssiOffset2Reg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SYSTEM_GAIN_REG 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiSystemGain : 9; //up hb rssi system gain from progmodel, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbRssiSystemGainReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SYSTEM_GAIN_SELECT_REG 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upHbRssiSystemGainHwSelect : 1; //up hb rssi system gain selection: 1 from hw, 0 from pm, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2UpHbRssiSystemGainSelectReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_SAT_THR 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssiSatThr : 9; //up hb rssi saturation threshold, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbRssiSatThr_u;

/*REG_B0_PHY_AGC_ANT2_UP_IB_RSSI_REG 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbRssi : 9; //up IB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpIbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_IB_POWER_REG 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbPower : 9; //up IB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpIbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_IB_SRSSI_REG 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbSrssi : 9; //up IB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpIbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_IB_SPOWER_REG 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upIbSpower : 9; //up IB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpIbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_IB_PWR_DIFF_REG 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upIbB1 : 9; //up IB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 upIbB2 : 9; //up IB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2UpIbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_RSSI_REG 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbRssi : 9; //up HB RSSI output, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbRssiReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_POWER_REG 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbPower : 9; //up HB power output (0.5 dB), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbPowerReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_SRSSI_REG 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbSrssi : 9; //up HB RSSI, slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbSrssiReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_SPOWER_REG 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		int32  upHbSpower : 9; //up HB Spower output (0.5 dB) slow IIR decimation , reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2UpHbSpowerReg_u;

/*REG_B0_PHY_AGC_ANT2_UP_HB_PWR_DIFF_REG 0x44C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upHbB1 : 9; //up HB power delta  (diff between 2 last samples, abs(Rn-R(n-1))), reset value: 0x0, access type: RO
		uint32 upHbB2 : 9; //up HB power delta (delay diffs, (abs(R(n-1)-R(n-2)), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyAgcAnt2UpHbPwrDiffReg_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_HB_POWER 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbPower : 9; //programmble up hb power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpHbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_IB_POWER 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbPower : 9; //programmble up ib power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpIbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_ACI_POWER 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpAciPower : 9; //programmble up aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpAciPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_POWER 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbPower : 9; //up hb power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_POWER 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbPower : 9; //up ib power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_ACI_POWER 0x464 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpAciPower : 9; //up aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpAciPower_u;

/*REG_B0_PHY_AGC_ANT2_UP_CONTROL 0x468 */
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
} RegB0PhyAgcAnt2UpControl_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_HB_SYSTEM_GAIN_OFFSET 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbSystemGainOffset : 9; //offset for up hb gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpHbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_IB_SYSTEM_GAIN_OFFSET 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbSystemGainOffset : 9; //offset for up ib gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpIbSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_ACI_SYSTEM_GAIN_OFFSET 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpAciSystemGainOffset : 9; //offset for up aci gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpAciSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_CCA_SYSTEM_GAIN_OFFSET 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpCcaSystemGainOffset : 9; //offset for up cca gain acceleration  (may be used to overcome unexpected constant gain in HW) in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpCcaSystemGainOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_HB_BOF_OFFSET 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbBofOffset : 9; //convert up hb rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpHbBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_IB_RSSI_BOF_OFFSET 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbRssiBofOffset : 9; //convert up ib rssi power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpIbRssiBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_IB_ACI_BOF_OFFSET 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbAciBofOffset : 9; //convert up ib aci power to backoff in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpIbAciBofOffset_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_HB_BOF_TARGET 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpHbBofTarget : 9; //target backoff from full scale at up hb rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpHbBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_PM_UP_IB_BOF_TARGET 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmUpIbBofTarget : 9; //target backoff from full scale at up ib rssi in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmUpIbBofTarget_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_SYSTEM_GAIN 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbSystemGain : 9; //up hb system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_SYSTEM_GAIN 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbSystemGain : 9; //up ib system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_ACI_SYSTEM_GAIN 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpAciSystemGain : 9; //up aci system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpAciSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_CCA_SYSTEM_GAIN 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpCcaSystemGain : 9; //up cca system gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpCcaSystemGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_DIGITAL_GAIN 0x4A0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbDigitalGain : 9; //up hb digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_DIGITAL_GAIN 0x4A4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbDigitalGain : 9; //up ib digital gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_BOF 0x4A8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbBof : 9; //up hb backoff accelerator calculation result in 1/2 dB pre bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_TOTAL_DIGITAL_GAIN 0x4AC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbTotalDigitalGain : 9; //up hb_total_digital_gain accelerator calculation result in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_HB_DIGITAL_BOF 0x4B0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpHbDigitalBof : 9; //up hb_digital_bof accelerator calculation result in 1/2 dB post bb digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpHbDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_BOF 0x4B4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbBof : 9; //up ib backoff accelerator calculation result in 1/2 dB pre ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbBof_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_TOTAL_DIGITAL_GAIN 0x4B8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbTotalDigitalGain : 9; //sum of up ib digital gain compunents: agc acceleration, pm and hw (bw compensation), reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbTotalDigitalGain_u;

/*REG_B0_PHY_AGC_ANT2_ACC_UP_IB_DIGITAL_BOF 0x4BC */
typedef union
{
	uint32 val;
	struct
	{
		int32  accUpIbDigitalBof : 9; //up_ib_digital_bof accelerator calculation result in 1/2 dB post ib digital gain, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccUpIbDigitalBof_u;

/*REG_B0_PHY_AGC_ANT2_PM_ACC_GAIN_SAT_SOURCES 0x4C0 */
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
} RegB0PhyAgcAnt2PmAccGainSatSources_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_0 0x4C4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut0 : 9; //lna gain in db for lna_index 0 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut0_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_1 0x4C8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut1 : 9; //lna gain in db for lna_index 1 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut1_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_2 0x4CC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut2 : 9; //lna gain in db for lna_index 2 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut2_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_3 0x4D0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut3 : 9; //lna gain in db for lna_index 3 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut3_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_4 0x4D4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut4 : 9; //lna gain in db for lna_index 4 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut4_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_5 0x4D8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut5 : 9; //lna gain in db for lna_index 5 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut5_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_6 0x4DC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut6 : 9; //lna gain in db for lna_index 6 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut6_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_7 0x4E0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut7 : 9; //lna gain in db for lna_index 7 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut7_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_8 0x4E4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut8 : 9; //lna gain in db for lna_index 8 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut8_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_9 0x4E8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut9 : 9; //lna gain in db for lna_index 9 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut9_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_10 0x4EC */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut10 : 9; //lna gain in db for lna_index 10 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut10_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_11 0x4F0 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut11 : 9; //lna gain in db for lna_index 11 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut11_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_12 0x4F4 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut12 : 9; //lna gain in db for lna_index 12 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut12_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_GAIN_LUT_13 0x4F8 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaGainLut13 : 9; //lna gain in db for lna_index 13 in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaGainLut13_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B0_B4 0x500 */
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
} RegB0PhyAgcAnt2PmBsAbsThB0B4_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B5_B7 0x504 */
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
} RegB0PhyAgcAnt2PmBsAbsThB5B7_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_ABS_TH_B8_B11 0x508 */
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
} RegB0PhyAgcAnt2PmBsAbsThB8B11_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B0_B4 0x50C */
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
} RegB0PhyAgcAnt2PmBsOffThB0B4_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B5_B7 0x510 */
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
} RegB0PhyAgcAnt2PmBsOffThB5B7_u;

/*REG_B0_PHY_AGC_ANT2_PM_BS_OFF_TH_B8_B11 0x514 */
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
} RegB0PhyAgcAnt2PmBsOffThB8B11_u;

/*REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS0_B4 0x518 */
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
} RegB0PhyAgcAnt2AccOffsetBs0B4_u;

/*REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS4_B7 0x51C */
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
} RegB0PhyAgcAnt2AccOffsetBs4B7_u;

/*REG_B0_PHY_AGC_ANT2_ACC_OFFSET_BS8_B11 0x520 */
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
} RegB0PhyAgcAnt2AccOffsetBs8B11_u;

/*REG_B0_PHY_AGC_ANT2_ACC_ABS_BLOCKER_MAP 0x524 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accAbsBlockerMap : 32; //map of absolute threshold check, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyAgcAnt2AccAbsBlockerMap_u;

/*REG_B0_PHY_AGC_ANT2_ACC_TH_BLOCKER_MAP 0x528 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accThBlockerMap : 32; //map of relative threshold check, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyAgcAnt2AccThBlockerMap_u;

/*REG_B0_PHY_AGC_ANT2_ACC_MAX_BLOCKER_LSB_POWER 0x52C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accMaxBlockerLsbPower : 9; //max blocker power in lsb side in 1/2 db resolution, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccMaxBlockerLsbPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_MAX_BLOCKER_USB_POWER 0x530 */
typedef union
{
	uint32 val;
	struct
	{
		int32  accMaxBlockerUsbPower : 9; //max blocker power in usb side in 1/2 db resolution, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccMaxBlockerUsbPower_u;

/*REG_B0_PHY_AGC_ANT2_PM_LNA_INTERNAL_GAIN 0x534 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmLnaInternalGain : 9; //lna internal gain in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmLnaInternalGain_u;

/*REG_B0_PHY_AGC_ANT2_PM_PRI_ACI_POWER 0x538 */
typedef union
{
	uint32 val;
	struct
	{
		int32  pmPriAciPower : 9; //programmble primary aci power to gain acceleration in 1/2 dB, reset value: 0x0, access type: RWS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2PmPriAciPower_u;

/*REG_B0_PHY_AGC_ANT2_ACC_PRI_ACI_POWER 0x53C */
typedef union
{
	uint32 val;
	struct
	{
		int32  accPriAciPower : 9; //primary aci power reading locked at "table go" in 1/2 dB, reset value: 0x0, access type: ROS
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyAgcAnt2AccPriAciPower_u;

/*REG_B0_PHY_AGC_ANT2_ULTRA_WIDE_BAND 0x540 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ultraWideBand : 1; //rf filter is open wide, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyAgcAnt2UltraWideBand_u;

/*REG_B0_PHY_AGC_ANT2_PM_PRESET_CALIBRATION 0x544 */
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
} RegB0PhyAgcAnt2PmPresetCalibration_u;



#endif // _B0_PHY_AGC_ANT2_REGS_H_
