
/***********************************************************************************
File:				RfEmulatorRegs.h
Module:				rfEmulator
SOC Revision:		latest
Generated at:       2024-06-26 12:55:09
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _RF_EMULATOR_REGS_H_
#define _RF_EMULATOR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define RF_EMULATOR_BASE_ADDRESS                   MEMORY_MAP_UNIT_69_BASE_ADDRESS
#define	REG_RF_EMULATOR_RX_SNR                       (RF_EMULATOR_BASE_ADDRESS + 0x0)
#define	REG_RF_EMULATOR_TX_SNR                       (RF_EMULATOR_BASE_ADDRESS + 0x4)
#define	REG_RF_EMULATOR_RX_ANT0_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x8)
#define	REG_RF_EMULATOR_RX_ANT0_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0xC)
#define	REG_RF_EMULATOR_RX_ANT0_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x10)
#define	REG_RF_EMULATOR_RX_ANT1_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x14)
#define	REG_RF_EMULATOR_RX_ANT1_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x18)
#define	REG_RF_EMULATOR_RX_ANT1_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x1C)
#define	REG_RF_EMULATOR_RX_ANT2_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x20)
#define	REG_RF_EMULATOR_RX_ANT2_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x24)
#define	REG_RF_EMULATOR_RX_ANT2_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x28)
#define	REG_RF_EMULATOR_RX_ANT3_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x2C)
#define	REG_RF_EMULATOR_RX_ANT3_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x30)
#define	REG_RF_EMULATOR_RX_ANT3_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x34)
#define	REG_RF_EMULATOR_TX_ANT0_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x38)
#define	REG_RF_EMULATOR_TX_ANT0_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x3C)
#define	REG_RF_EMULATOR_TX_ANT0_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x40)
#define	REG_RF_EMULATOR_TX_ANT1_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x44)
#define	REG_RF_EMULATOR_TX_ANT1_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x48)
#define	REG_RF_EMULATOR_TX_ANT1_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x4C)
#define	REG_RF_EMULATOR_TX_ANT2_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x50)
#define	REG_RF_EMULATOR_TX_ANT2_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x54)
#define	REG_RF_EMULATOR_TX_ANT2_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x58)
#define	REG_RF_EMULATOR_TX_ANT3_SEED0                (RF_EMULATOR_BASE_ADDRESS + 0x5C)
#define	REG_RF_EMULATOR_TX_ANT3_SEED1                (RF_EMULATOR_BASE_ADDRESS + 0x60)
#define	REG_RF_EMULATOR_TX_ANT3_SEED2                (RF_EMULATOR_BASE_ADDRESS + 0x64)
#define	REG_RF_EMULATOR_LOSS                         (RF_EMULATOR_BASE_ADDRESS + 0x68)
#define	REG_RF_EMULATOR_RF_CONTROL                   (RF_EMULATOR_BASE_ADDRESS + 0x6C)
#define	REG_RF_EMULATOR_TSSI2_ANT0                   (RF_EMULATOR_BASE_ADDRESS + 0x70)
#define	REG_RF_EMULATOR_TSSI2_ANT1                   (RF_EMULATOR_BASE_ADDRESS + 0x74)
#define	REG_RF_EMULATOR_TSSI2_ANT2                   (RF_EMULATOR_BASE_ADDRESS + 0x78)
#define	REG_RF_EMULATOR_TSSI2_ANT3                   (RF_EMULATOR_BASE_ADDRESS + 0x7C)
#define	REG_RF_EMULATOR_TSSI5_ANT0                   (RF_EMULATOR_BASE_ADDRESS + 0x80)
#define	REG_RF_EMULATOR_TSSI5_ANT1                   (RF_EMULATOR_BASE_ADDRESS + 0x84)
#define	REG_RF_EMULATOR_TSSI5_ANT2                   (RF_EMULATOR_BASE_ADDRESS + 0x88)
#define	REG_RF_EMULATOR_TSSI5_ANT3                   (RF_EMULATOR_BASE_ADDRESS + 0x8C)
#define	REG_RF_EMULATOR_FECTRL_ANT_2G_5G             (RF_EMULATOR_BASE_ADDRESS + 0x90)
#define	REG_RF_EMULATOR_FECTRL_C                     (RF_EMULATOR_BASE_ADDRESS + 0x94)
#define	REG_RF_EMULATOR_RF_GEN                       (RF_EMULATOR_BASE_ADDRESS + 0x98)
#define	REG_RF_EMULATOR_FECTRL_ANT_6G                (RF_EMULATOR_BASE_ADDRESS + 0x9C)
#define	REG_RF_EMULATOR_LNA_CTRL                     (RF_EMULATOR_BASE_ADDRESS + 0xA0)
#define	REG_RF_EMULATOR_SPARE_REG                    (RF_EMULATOR_BASE_ADDRESS + 0xA4)
#define	REG_RF_EMULATOR_RF_CONTROL_EXT               (RF_EMULATOR_BASE_ADDRESS + 0xA8)
#define	REG_RF_EMULATOR_EXTERNAL_PSEL                (RF_EMULATOR_BASE_ADDRESS + 0xAC)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0xB0)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0xB4)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0xB8)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0xBC)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0xC0)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0xC4)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0xC8)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0xCC)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0xD0)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0xD4)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0xD8)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0xDC)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0xE0)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0xE4)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0xE8)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0xEC)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0xF0)
#define	REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0xF4)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0xF8)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0xFC)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x100)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x104)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x108)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x10C)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x110)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x114)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x118)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x11C)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x120)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x124)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x128)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x12C)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x130)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x134)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x138)
#define	REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x13C)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x140)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x144)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x148)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x14C)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x150)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x154)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x158)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x16C)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x170)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x174)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x178)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x17C)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x180)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x184)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x188)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x18C)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x190)
#define	REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x194)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x198)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x19C)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1A0)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1A4)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x1A8)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x1AC)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x1B0)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x1B4)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x1B8)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x1BC)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1C0)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1C4)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x1C8)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x1CC)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x1D0)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x1D4)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x1D8)
#define	REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x1DC)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x1E0)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x1E4)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1E8)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x1EC)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x1F0)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x1F4)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x1F8)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x1FC)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x200)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x204)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x208)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x20C)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x210)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x214)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x218)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x21C)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x220)
#define	REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x224)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x228)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x22C)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x230)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x234)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x238)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x23C)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x240)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x244)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x248)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x24C)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x250)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x254)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x258)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x25C)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x260)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x264)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x268)
#define	REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x26C)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x270)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x274)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x278)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x27C)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x280)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x284)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x288)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x28C)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x290)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x294)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x298)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x29C)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x2A0)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x2A4)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x2A8)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x2AC)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x2B0)
#define	REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x2B4)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x2B8)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x2BC)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x2C0)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x2C4)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x2C8)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x2CC)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x2D0)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x2D4)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x2D8)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x2DC)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x2E0)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x2E4)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x2E8)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x2EC)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x2F0)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x2F4)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x2F8)
#define	REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x2FC)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x300)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x304)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x308)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x30C)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x310)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x314)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x318)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x31C)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x320)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x324)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x328)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x32C)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x330)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x334)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x338)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x33C)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x340)
#define	REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x344)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x348)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x34C)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x350)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x354)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x358)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x35C)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x360)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x364)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x368)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x36C)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x370)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x374)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x378)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x37C)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x380)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x384)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x388)
#define	REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x38C)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x390)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x394)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x398)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x39C)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x3A0)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x3A4)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x3A8)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x3AC)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x3B0)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x3B4)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x3B8)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x3BC)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x3C0)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x3C4)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x3C8)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x3CC)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x3D0)
#define	REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x3D4)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x3D8)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN2PIF       (RF_EMULATOR_BASE_ADDRESS + 0x3DC)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x3E0)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x3E4)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x3E8)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x3EC)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x3F0)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x3F4)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x3F8)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x3FC)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_COS_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x400)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_SIN_PMM2      (RF_EMULATOR_BASE_ADDRESS + 0x404)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_I          (RF_EMULATOR_BASE_ADDRESS + 0x408)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_Q          (RF_EMULATOR_BASE_ADDRESS + 0x40C)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_I2Q       (RF_EMULATOR_BASE_ADDRESS + 0x410)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_Q2I       (RF_EMULATOR_BASE_ADDRESS + 0x414)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_I    (RF_EMULATOR_BASE_ADDRESS + 0x418)
#define	REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_Q    (RF_EMULATOR_BASE_ADDRESS + 0x41C)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_COS2PIF        (RF_EMULATOR_BASE_ADDRESS + 0x420)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN2PIF        (RF_EMULATOR_BASE_ADDRESS + 0x424)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_COS_PMM2       (RF_EMULATOR_BASE_ADDRESS + 0x428)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN_PMM2       (RF_EMULATOR_BASE_ADDRESS + 0x42C)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_I           (RF_EMULATOR_BASE_ADDRESS + 0x430)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_Q           (RF_EMULATOR_BASE_ADDRESS + 0x434)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_I2Q        (RF_EMULATOR_BASE_ADDRESS + 0x438)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_Q2I        (RF_EMULATOR_BASE_ADDRESS + 0x43C)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_I     (RF_EMULATOR_BASE_ADDRESS + 0x440)
#define	REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_Q     (RF_EMULATOR_BASE_ADDRESS + 0x444)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_RF_EMULATOR_RX_SNR 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxSnrAnt0 : 8; //SNR value for the RX, reset value: 0x1, access type: RW
		uint32 rxSnrAnt1 : 8; //SNR value for the RX, reset value: 0x1, access type: RW
		uint32 rxSnrAnt2 : 8; //SNR value for the RX, reset value: 0x1, access type: RW
		uint32 rxSnrAnt3 : 8; //SNR value for the RX, reset value: 0x1, access type: RW
	} bitFields;
} RegRfEmulatorRxSnr_u;

/*REG_RF_EMULATOR_TX_SNR 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSnrAnt0 : 8; //SNR value for the TX, reset value: 0x1, access type: RW
		uint32 txSnrAnt1 : 8; //SNR value for the TX, reset value: 0x1, access type: RW
		uint32 txSnrAnt2 : 8; //SNR value for the TX, reset value: 0x1, access type: RW
		uint32 txSnrAnt3 : 8; //SNR value for the TX, reset value: 0x1, access type: RW
	} bitFields;
} RegRfEmulatorTxSnr_u;

/*REG_RF_EMULATOR_RX_ANT0_SEED0 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed0Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt0Seed0_u;

/*REG_RF_EMULATOR_RX_ANT0_SEED1 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed1Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt0Seed1_u;

/*REG_RF_EMULATOR_RX_ANT0_SEED2 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed2Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt0Seed2_u;

/*REG_RF_EMULATOR_RX_ANT1_SEED0 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed0Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt1Seed0_u;

/*REG_RF_EMULATOR_RX_ANT1_SEED1 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed1Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt1Seed1_u;

/*REG_RF_EMULATOR_RX_ANT1_SEED2 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed2Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt1Seed2_u;

/*REG_RF_EMULATOR_RX_ANT2_SEED0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed0Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt2Seed0_u;

/*REG_RF_EMULATOR_RX_ANT2_SEED1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed1Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt2Seed1_u;

/*REG_RF_EMULATOR_RX_ANT2_SEED2 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed2Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt2Seed2_u;

/*REG_RF_EMULATOR_RX_ANT3_SEED0 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed0Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt3Seed0_u;

/*REG_RF_EMULATOR_RX_ANT3_SEED1 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed1Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt3Seed1_u;

/*REG_RF_EMULATOR_RX_ANT3_SEED2 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBaseSeed2Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorRxAnt3Seed2_u;

/*REG_RF_EMULATOR_TX_ANT0_SEED0 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed0Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt0Seed0_u;

/*REG_RF_EMULATOR_TX_ANT0_SEED1 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed1Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt0Seed1_u;

/*REG_RF_EMULATOR_TX_ANT0_SEED2 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed2Ant0 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt0Seed2_u;

/*REG_RF_EMULATOR_TX_ANT1_SEED0 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed0Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt1Seed0_u;

/*REG_RF_EMULATOR_TX_ANT1_SEED1 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed1Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt1Seed1_u;

/*REG_RF_EMULATOR_TX_ANT1_SEED2 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed2Ant1 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt1Seed2_u;

/*REG_RF_EMULATOR_TX_ANT2_SEED0 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed0Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt2Seed0_u;

/*REG_RF_EMULATOR_TX_ANT2_SEED1 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed1Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt2Seed1_u;

/*REG_RF_EMULATOR_TX_ANT2_SEED2 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed2Ant2 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt2Seed2_u;

/*REG_RF_EMULATOR_TX_ANT3_SEED0 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed0Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt3Seed0_u;

/*REG_RF_EMULATOR_TX_ANT3_SEED1 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed1Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt3Seed1_u;

/*REG_RF_EMULATOR_TX_ANT3_SEED2 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txBaseSeed2Ant3 : 32; //seed value, reset value: 0x12, access type: RW
	} bitFields;
} RegRfEmulatorTxAnt3Seed2_u;

/*REG_RF_EMULATOR_LOSS 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lossValueAnt0 : 8; //loss value for local FIRs ant0, reset value: 0x7F, access type: RW
		uint32 lossValueAnt1 : 8; //loss value for local FIRs ant1, reset value: 0x7F, access type: RW
		uint32 lossValueAnt2 : 8; //loss value for local FIRs ant2, reset value: 0x7F, access type: RW
		uint32 lossValueAnt3 : 8; //loss value for local FIRs ant3, reset value: 0x7F, access type: RW
	} bitFields;
} RegRfEmulatorLoss_u;

/*REG_RF_EMULATOR_RF_CONTROL 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectHbLb : 5; //select between 2.4GHz antenna (1'b0) and 5GHz antenna (1'b1), reset value: 0xF, access type: RW
		uint32 addNoiseFromRfGain : 5; //per antenna bit to add the noise gain from the RF gain, reset value: 0x0, access type: RW
		uint32 channelAdd1Smp : 1; //Add 640MHz cycle delay before the channel emulator on the valid signal, reset value: 0x0, access type: RW
		uint32 channelRxStrbCntrl : 1; //Add 640MHz cycle delay for the RX strb in the PHY, reset value: 0x0, access type: RW
		uint32 enPgaBfilter : 4; //enable per antenna the PGA in the RF, reset value: 0xF, access type: RW
		uint32 rfRxDelay : 8; //640MHz cycle delay of RF module RX latency, reset value: 0x0, access type: RW
		uint32 rfTxDelay : 8; //640MHz cycle delay of RF module TX latency, reset value: 0x0, access type: RW
	} bitFields;
} RegRfEmulatorRfControl_u;

/*REG_RF_EMULATOR_TSSI2_ANT0 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi2Ant0 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi2Ant0_u;

/*REG_RF_EMULATOR_TSSI2_ANT1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi2Ant1 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi2Ant1_u;

/*REG_RF_EMULATOR_TSSI2_ANT2 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi2Ant2 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi2Ant2_u;

/*REG_RF_EMULATOR_TSSI2_ANT3 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi2Ant3 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi2Ant3_u;

/*REG_RF_EMULATOR_TSSI5_ANT0 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi5Ant0 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi5Ant0_u;

/*REG_RF_EMULATOR_TSSI5_ANT1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi5Ant1 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi5Ant1_u;

/*REG_RF_EMULATOR_TSSI5_ANT2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi5Ant2 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi5Ant2_u;

/*REG_RF_EMULATOR_TSSI5_ANT3 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swTssi5Ant3 : 24; //Set TSSI value, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorTssi5Ant3_u;

/*REG_RF_EMULATOR_FECTRL_ANT_2G_5G 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fectrlAnt02G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt12G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt22G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt32G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt05G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt15G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt25G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt35G : 4; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegRfEmulatorFectrlAnt2G5G_u;

/*REG_RF_EMULATOR_FECTRL_C 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fectrlC : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegRfEmulatorFectrlC_u;

/*REG_RF_EMULATOR_RF_GEN 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enPgaBfilterAnt4 : 1; //enable for ant4 the PGA in the RF, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegRfEmulatorRfGen_u;

/*REG_RF_EMULATOR_FECTRL_ANT_6G 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fectrlAnt06G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt16G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt26G : 4; //no description, reset value: 0x0, access type: RO
		uint32 fectrlAnt36G : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegRfEmulatorFectrlAnt6G_u;

/*REG_RF_EMULATOR_LNA_CTRL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLnaOverride : 5; //lna control per antenna, write 1 to override the value, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 swLnaOverrideVal : 5; //The value to be overrided into LNA in case sw_lna_override[*] is 1, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegRfEmulatorLnaCtrl_u;

/*REG_RF_EMULATOR_SPARE_REG 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareField : 24; //spare RW reg, reset value: 0x1234, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorSpareReg_u;

/*REG_RF_EMULATOR_RF_CONTROL_EXT 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 channelSkipRxAnt : 4; //skip the rx path through the channel and loss per antenna, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegRfEmulatorRfControlExt_u;

/*REG_RF_EMULATOR_EXTERNAL_PSEL 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 externalPsels : 8; //Hit value for configuring all registers. This field must be equal to the exact instance id in order to read/write , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegRfEmulatorExternalPsel_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS2PIF 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN2PIF 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS_PMM2 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN_PMM2 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_I 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_Q 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_I2Q 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_Q2I 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_I 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_Q 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_COS_PMM2 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_SIN_PMM2 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_I 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_Q 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_I2Q 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_Q2I 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_I 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_Q 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt0TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt0TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS2PIF 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN2PIF 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS_PMM2 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN_PMM2 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_I 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_Q 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_I2Q 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_Q2I 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_I 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_Q 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_COS_PMM2 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_SIN_PMM2 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_I 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_Q 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_I2Q 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_Q2I 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_I 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_Q 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt1TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt1TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS2PIF 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN2PIF 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS_PMM2 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN_PMM2 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_I 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_Q 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_I2Q 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_Q2I 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_I 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_Q 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_COS_PMM2 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_SIN_PMM2 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_I 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_Q 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_I2Q 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_Q2I 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_I 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_Q 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt2TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt2TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS2PIF 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN2PIF 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS_PMM2 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN_PMM2 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_I 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_Q 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_I2Q 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_Q2I 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_I 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_Q 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_COS_PMM2 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_SIN_PMM2 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_I 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxDcI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_Q 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxDcQ_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_I2Q 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_Q2I 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_I 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_Q 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp2GAnt3TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp2GAnt3TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS2PIF 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN2PIF 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS_PMM2 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN_PMM2 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_I 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_Q 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_I2Q 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_Q2I 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_I 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_Q 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_COS_PMM2 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_SIN_PMM2 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_I 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_Q 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_I2Q 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_Q2I 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_I 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_Q 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt0TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt0TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS2PIF 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN2PIF 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS_PMM2 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN_PMM2 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_I 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_Q 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_I2Q 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_Q2I 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_I 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_Q 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_COS_PMM2 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_SIN_PMM2 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_I 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_Q 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_I2Q 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_Q2I 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_I 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_Q 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt1TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt1TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS2PIF 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN2PIF 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS_PMM2 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN_PMM2 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_I 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_Q 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_I2Q 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_Q2I 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_I 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_Q 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_COS_PMM2 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_SIN_PMM2 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_I 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_Q 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_I2Q 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_Q2I 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_I 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_Q 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt2TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt2TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS2PIF 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN2PIF 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS_PMM2 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN_PMM2 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_I 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_Q 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_I2Q 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_Q2I 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_I 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_Q 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_SIN_PMM2 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_COS_PMM2 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_I 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_Q 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_I2Q 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_Q2I 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_I 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_Q 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GAnt3TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GAnt3TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS2PIF 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN2PIF 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS_PMM2 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN_PMM2 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_I 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_Q 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_I2Q 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_Q2I 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_I 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_Q 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_COS_PMM2 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_SIN_PMM2 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_I 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_Q 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_I2Q 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_Q2I 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_I 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_Q 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt0TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt0TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS2PIF 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN2PIF 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS_PMM2 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN_PMM2 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_I 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_Q 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_I2Q 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_Q2I 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_I 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_Q 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_COS_PMM2 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_SIN_PMM2 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_I 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_Q 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_I2Q 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_Q2I 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_I 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_Q 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt1TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt1TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS2PIF 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN2PIF 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS_PMM2 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN_PMM2 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_I 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_Q 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_I2Q 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_Q2I 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_I 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_Q 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_COS_PMM2 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_SIN_PMM2 0x3BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_I 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_Q 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_I2Q 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_Q2I 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_I 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_Q 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt2TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt2TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS2PIF 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN2PIF 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS_PMM2 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN_PMM2 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_I 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_Q 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_I2Q 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_Q2I 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_I 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_Q 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3RxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3RxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_COS_PMM2 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_SIN_PMM2 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_I 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxDcI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_Q 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxDcQ_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_I2Q 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_Q2I 0x414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_I 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_Q 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp6GAnt3TxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp6GAnt3TxSqrtGmmQ_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_COS2PIF 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxCos2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxCos2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN2PIF 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxSin2Pif : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxSin2Pif_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_COS_PMM2 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxCosPmm2 : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxCosPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN_PMM2 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxSinPmm2 : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxSinPmm2_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_I 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxDcI : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxDcI_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_Q 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxDcQ : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxDcQ_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_I2Q 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxLinI2Q : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxLinI2Q_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_Q2I 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxLinQ2I : 24; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxLinQ2I_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_I 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxSqrtGmmI : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxSqrtGmmI_u;

/*REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_Q 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 imp5GDfsRxSqrtGmmQ : 24; //no description, reset value: 0x400000, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegRfEmulatorImp5GDfsRxSqrtGmmQ_u;

//========================================
/* REG_RF_EMULATOR_RX_SNR 0x0 */
#define RF_EMULATOR_RX_SNR_FIELD_RX_SNR_ANT0_MASK                                                                 0x000000FF
#define RF_EMULATOR_RX_SNR_FIELD_RX_SNR_ANT1_MASK                                                                 0x0000FF00
#define RF_EMULATOR_RX_SNR_FIELD_RX_SNR_ANT2_MASK                                                                 0x00FF0000
#define RF_EMULATOR_RX_SNR_FIELD_RX_SNR_ANT3_MASK                                                                 0xFF000000
//========================================
/* REG_RF_EMULATOR_TX_SNR 0x4 */
#define RF_EMULATOR_TX_SNR_FIELD_TX_SNR_ANT0_MASK                                                                 0x000000FF
#define RF_EMULATOR_TX_SNR_FIELD_TX_SNR_ANT1_MASK                                                                 0x0000FF00
#define RF_EMULATOR_TX_SNR_FIELD_TX_SNR_ANT2_MASK                                                                 0x00FF0000
#define RF_EMULATOR_TX_SNR_FIELD_TX_SNR_ANT3_MASK                                                                 0xFF000000
//========================================
/* REG_RF_EMULATOR_RX_ANT0_SEED0 0x8 */
#define RF_EMULATOR_RX_ANT0_SEED0_FIELD_RX_BASE_SEED0_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT0_SEED1 0xC */
#define RF_EMULATOR_RX_ANT0_SEED1_FIELD_RX_BASE_SEED1_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT0_SEED2 0x10 */
#define RF_EMULATOR_RX_ANT0_SEED2_FIELD_RX_BASE_SEED2_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT1_SEED0 0x14 */
#define RF_EMULATOR_RX_ANT1_SEED0_FIELD_RX_BASE_SEED0_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT1_SEED1 0x18 */
#define RF_EMULATOR_RX_ANT1_SEED1_FIELD_RX_BASE_SEED1_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT1_SEED2 0x1C */
#define RF_EMULATOR_RX_ANT1_SEED2_FIELD_RX_BASE_SEED2_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT2_SEED0 0x20 */
#define RF_EMULATOR_RX_ANT2_SEED0_FIELD_RX_BASE_SEED0_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT2_SEED1 0x24 */
#define RF_EMULATOR_RX_ANT2_SEED1_FIELD_RX_BASE_SEED1_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT2_SEED2 0x28 */
#define RF_EMULATOR_RX_ANT2_SEED2_FIELD_RX_BASE_SEED2_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT3_SEED0 0x2C */
#define RF_EMULATOR_RX_ANT3_SEED0_FIELD_RX_BASE_SEED0_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT3_SEED1 0x30 */
#define RF_EMULATOR_RX_ANT3_SEED1_FIELD_RX_BASE_SEED1_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_RX_ANT3_SEED2 0x34 */
#define RF_EMULATOR_RX_ANT3_SEED2_FIELD_RX_BASE_SEED2_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT0_SEED0 0x38 */
#define RF_EMULATOR_TX_ANT0_SEED0_FIELD_TX_BASE_SEED0_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT0_SEED1 0x3C */
#define RF_EMULATOR_TX_ANT0_SEED1_FIELD_TX_BASE_SEED1_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT0_SEED2 0x40 */
#define RF_EMULATOR_TX_ANT0_SEED2_FIELD_TX_BASE_SEED2_ANT0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT1_SEED0 0x44 */
#define RF_EMULATOR_TX_ANT1_SEED0_FIELD_TX_BASE_SEED0_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT1_SEED1 0x48 */
#define RF_EMULATOR_TX_ANT1_SEED1_FIELD_TX_BASE_SEED1_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT1_SEED2 0x4C */
#define RF_EMULATOR_TX_ANT1_SEED2_FIELD_TX_BASE_SEED2_ANT1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT2_SEED0 0x50 */
#define RF_EMULATOR_TX_ANT2_SEED0_FIELD_TX_BASE_SEED0_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT2_SEED1 0x54 */
#define RF_EMULATOR_TX_ANT2_SEED1_FIELD_TX_BASE_SEED1_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT2_SEED2 0x58 */
#define RF_EMULATOR_TX_ANT2_SEED2_FIELD_TX_BASE_SEED2_ANT2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT3_SEED0 0x5C */
#define RF_EMULATOR_TX_ANT3_SEED0_FIELD_TX_BASE_SEED0_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT3_SEED1 0x60 */
#define RF_EMULATOR_TX_ANT3_SEED1_FIELD_TX_BASE_SEED1_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_TX_ANT3_SEED2 0x64 */
#define RF_EMULATOR_TX_ANT3_SEED2_FIELD_TX_BASE_SEED2_ANT3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_RF_EMULATOR_LOSS 0x68 */
#define RF_EMULATOR_LOSS_FIELD_LOSS_VALUE_ANT0_MASK                                                               0x000000FF
#define RF_EMULATOR_LOSS_FIELD_LOSS_VALUE_ANT1_MASK                                                               0x0000FF00
#define RF_EMULATOR_LOSS_FIELD_LOSS_VALUE_ANT2_MASK                                                               0x00FF0000
#define RF_EMULATOR_LOSS_FIELD_LOSS_VALUE_ANT3_MASK                                                               0xFF000000
//========================================
/* REG_RF_EMULATOR_RF_CONTROL 0x6C */
#define RF_EMULATOR_RF_CONTROL_FIELD_SELECT_HB_LB_MASK                                                            0x0000001F
#define RF_EMULATOR_RF_CONTROL_FIELD_ADD_NOISE_FROM_RF_GAIN_MASK                                                  0x000003E0
#define RF_EMULATOR_RF_CONTROL_FIELD_CHANNEL_ADD_1SMP_MASK                                                        0x00000400
#define RF_EMULATOR_RF_CONTROL_FIELD_CHANNEL_RX_STRB_CNTRL_MASK                                                   0x00000800
#define RF_EMULATOR_RF_CONTROL_FIELD_EN_PGA_BFILTER_MASK                                                          0x0000F000
#define RF_EMULATOR_RF_CONTROL_FIELD_RF_RX_DELAY_MASK                                                             0x00FF0000
#define RF_EMULATOR_RF_CONTROL_FIELD_RF_TX_DELAY_MASK                                                             0xFF000000
//========================================
/* REG_RF_EMULATOR_TSSI2_ANT0 0x70 */
#define RF_EMULATOR_TSSI2_ANT0_FIELD_SW_TSSI2_ANT0_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI2_ANT1 0x74 */
#define RF_EMULATOR_TSSI2_ANT1_FIELD_SW_TSSI2_ANT1_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI2_ANT2 0x78 */
#define RF_EMULATOR_TSSI2_ANT2_FIELD_SW_TSSI2_ANT2_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI2_ANT3 0x7C */
#define RF_EMULATOR_TSSI2_ANT3_FIELD_SW_TSSI2_ANT3_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI5_ANT0 0x80 */
#define RF_EMULATOR_TSSI5_ANT0_FIELD_SW_TSSI5_ANT0_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI5_ANT1 0x84 */
#define RF_EMULATOR_TSSI5_ANT1_FIELD_SW_TSSI5_ANT1_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI5_ANT2 0x88 */
#define RF_EMULATOR_TSSI5_ANT2_FIELD_SW_TSSI5_ANT2_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_TSSI5_ANT3 0x8C */
#define RF_EMULATOR_TSSI5_ANT3_FIELD_SW_TSSI5_ANT3_MASK                                                           0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_FECTRL_ANT_2G_5G 0x90 */
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT0_2G_MASK                                                    0x0000000F
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT1_2G_MASK                                                    0x000000F0
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT2_2G_MASK                                                    0x00000F00
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT3_2G_MASK                                                    0x0000F000
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT0_5G_MASK                                                    0x000F0000
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT1_5G_MASK                                                    0x00F00000
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT2_5G_MASK                                                    0x0F000000
#define RF_EMULATOR_FECTRL_ANT_2G_5G_FIELD_FECTRL_ANT3_5G_MASK                                                    0xF0000000
//========================================
/* REG_RF_EMULATOR_FECTRL_C 0x94 */
#define RF_EMULATOR_FECTRL_C_FIELD_FECTRL_C_MASK                                                                  0x000000FF
//========================================
/* REG_RF_EMULATOR_RF_GEN 0x98 */
#define RF_EMULATOR_RF_GEN_FIELD_EN_PGA_BFILTER_ANT4_MASK                                                         0x00000001
//========================================
/* REG_RF_EMULATOR_FECTRL_ANT_6G 0x9C */
#define RF_EMULATOR_FECTRL_ANT_6G_FIELD_FECTRL_ANT0_6G_MASK                                                       0x0000000F
#define RF_EMULATOR_FECTRL_ANT_6G_FIELD_FECTRL_ANT1_6G_MASK                                                       0x000000F0
#define RF_EMULATOR_FECTRL_ANT_6G_FIELD_FECTRL_ANT2_6G_MASK                                                       0x00000F00
#define RF_EMULATOR_FECTRL_ANT_6G_FIELD_FECTRL_ANT3_6G_MASK                                                       0x0000F000
//========================================
/* REG_RF_EMULATOR_LNA_CTRL 0xA0 */
#define RF_EMULATOR_LNA_CTRL_FIELD_SW_LNA_OVERRIDE_MASK                                                           0x0000001F
#define RF_EMULATOR_LNA_CTRL_FIELD_SW_LNA_OVERRIDE_VAL_MASK                                                       0x00001F00
//========================================
/* REG_RF_EMULATOR_SPARE_REG 0xA4 */
#define RF_EMULATOR_SPARE_REG_FIELD_SPARE_FIELD_MASK                                                              0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_RF_CONTROL_EXT 0xA8 */
#define RF_EMULATOR_RF_CONTROL_EXT_FIELD_CHANNEL_SKIP_RX_ANT_MASK                                                 0x0000000F
//========================================
/* REG_RF_EMULATOR_EXTERNAL_PSEL 0xAC */
#define RF_EMULATOR_EXTERNAL_PSEL_FIELD_EXTERNAL_PSELS_MASK                                                       0x000000FF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS2PIF 0xB0 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_COS2PIF_FIELD_IMP_2G_ANT0_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN2PIF 0xB4 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_SIN2PIF_FIELD_IMP_2G_ANT0_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_COS_PMM2 0xB8 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_COS_PMM2_FIELD_IMP_2G_ANT0_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_SIN_PMM2 0xBC */
#define RF_EMULATOR_IMP_2G_ANT0_RX_SIN_PMM2_FIELD_IMP_2G_ANT0_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_I 0xC0 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_DC_I_FIELD_IMP_2G_ANT0_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_DC_Q 0xC4 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_DC_Q_FIELD_IMP_2G_ANT0_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_I2Q 0xC8 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_LIN_I2Q_FIELD_IMP_2G_ANT0_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_LIN_Q2I 0xCC */
#define RF_EMULATOR_IMP_2G_ANT0_RX_LIN_Q2I_FIELD_IMP_2G_ANT0_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_I 0xD0 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_I_FIELD_IMP_2G_ANT0_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_Q 0xD4 */
#define RF_EMULATOR_IMP_2G_ANT0_RX_SQRT_GMM_Q_FIELD_IMP_2G_ANT0_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_COS_PMM2 0xD8 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_COS_PMM2_FIELD_IMP_2G_ANT0_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_SIN_PMM2 0xDC */
#define RF_EMULATOR_IMP_2G_ANT0_TX_SIN_PMM2_FIELD_IMP_2G_ANT0_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_I 0xE0 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_DC_I_FIELD_IMP_2G_ANT0_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_DC_Q 0xE4 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_DC_Q_FIELD_IMP_2G_ANT0_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_I2Q 0xE8 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_LIN_I2Q_FIELD_IMP_2G_ANT0_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_LIN_Q2I 0xEC */
#define RF_EMULATOR_IMP_2G_ANT0_TX_LIN_Q2I_FIELD_IMP_2G_ANT0_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_I 0xF0 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_I_FIELD_IMP_2G_ANT0_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_Q 0xF4 */
#define RF_EMULATOR_IMP_2G_ANT0_TX_SQRT_GMM_Q_FIELD_IMP_2G_ANT0_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS2PIF 0xF8 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_COS2PIF_FIELD_IMP_2G_ANT1_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN2PIF 0xFC */
#define RF_EMULATOR_IMP_2G_ANT1_RX_SIN2PIF_FIELD_IMP_2G_ANT1_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_COS_PMM2 0x100 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_COS_PMM2_FIELD_IMP_2G_ANT1_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_SIN_PMM2 0x104 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_SIN_PMM2_FIELD_IMP_2G_ANT1_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_I 0x108 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_DC_I_FIELD_IMP_2G_ANT1_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_DC_Q 0x10C */
#define RF_EMULATOR_IMP_2G_ANT1_RX_DC_Q_FIELD_IMP_2G_ANT1_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_I2Q 0x110 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_LIN_I2Q_FIELD_IMP_2G_ANT1_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_LIN_Q2I 0x114 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_LIN_Q2I_FIELD_IMP_2G_ANT1_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_I 0x118 */
#define RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_I_FIELD_IMP_2G_ANT1_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_Q 0x11C */
#define RF_EMULATOR_IMP_2G_ANT1_RX_SQRT_GMM_Q_FIELD_IMP_2G_ANT1_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_COS_PMM2 0x120 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_COS_PMM2_FIELD_IMP_2G_ANT1_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_SIN_PMM2 0x124 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_SIN_PMM2_FIELD_IMP_2G_ANT1_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_I 0x128 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_DC_I_FIELD_IMP_2G_ANT1_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_DC_Q 0x12C */
#define RF_EMULATOR_IMP_2G_ANT1_TX_DC_Q_FIELD_IMP_2G_ANT1_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_I2Q 0x130 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_LIN_I2Q_FIELD_IMP_2G_ANT1_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_LIN_Q2I 0x134 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_LIN_Q2I_FIELD_IMP_2G_ANT1_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_I 0x138 */
#define RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_I_FIELD_IMP_2G_ANT1_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_Q 0x13C */
#define RF_EMULATOR_IMP_2G_ANT1_TX_SQRT_GMM_Q_FIELD_IMP_2G_ANT1_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS2PIF 0x140 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_COS2PIF_FIELD_IMP_2G_ANT2_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN2PIF 0x144 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_SIN2PIF_FIELD_IMP_2G_ANT2_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_COS_PMM2 0x148 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_COS_PMM2_FIELD_IMP_2G_ANT2_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_SIN_PMM2 0x14C */
#define RF_EMULATOR_IMP_2G_ANT2_RX_SIN_PMM2_FIELD_IMP_2G_ANT2_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_I 0x150 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_DC_I_FIELD_IMP_2G_ANT2_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_DC_Q 0x154 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_DC_Q_FIELD_IMP_2G_ANT2_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_I2Q 0x158 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_LIN_I2Q_FIELD_IMP_2G_ANT2_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_LIN_Q2I 0x16C */
#define RF_EMULATOR_IMP_2G_ANT2_RX_LIN_Q2I_FIELD_IMP_2G_ANT2_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_I 0x170 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_I_FIELD_IMP_2G_ANT2_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_Q 0x174 */
#define RF_EMULATOR_IMP_2G_ANT2_RX_SQRT_GMM_Q_FIELD_IMP_2G_ANT2_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_COS_PMM2 0x178 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_COS_PMM2_FIELD_IMP_2G_ANT2_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_SIN_PMM2 0x17C */
#define RF_EMULATOR_IMP_2G_ANT2_TX_SIN_PMM2_FIELD_IMP_2G_ANT2_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_I 0x180 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_DC_I_FIELD_IMP_2G_ANT2_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_DC_Q 0x184 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_DC_Q_FIELD_IMP_2G_ANT2_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_I2Q 0x188 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_LIN_I2Q_FIELD_IMP_2G_ANT2_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_LIN_Q2I 0x18C */
#define RF_EMULATOR_IMP_2G_ANT2_TX_LIN_Q2I_FIELD_IMP_2G_ANT2_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_I 0x190 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_I_FIELD_IMP_2G_ANT2_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_Q 0x194 */
#define RF_EMULATOR_IMP_2G_ANT2_TX_SQRT_GMM_Q_FIELD_IMP_2G_ANT2_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS2PIF 0x198 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_COS2PIF_FIELD_IMP_2G_ANT3_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN2PIF 0x19C */
#define RF_EMULATOR_IMP_2G_ANT3_RX_SIN2PIF_FIELD_IMP_2G_ANT3_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_COS_PMM2 0x1A0 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_COS_PMM2_FIELD_IMP_2G_ANT3_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_SIN_PMM2 0x1A4 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_SIN_PMM2_FIELD_IMP_2G_ANT3_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_I 0x1A8 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_DC_I_FIELD_IMP_2G_ANT3_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_DC_Q 0x1AC */
#define RF_EMULATOR_IMP_2G_ANT3_RX_DC_Q_FIELD_IMP_2G_ANT3_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_I2Q 0x1B0 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_LIN_I2Q_FIELD_IMP_2G_ANT3_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_LIN_Q2I 0x1B4 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_LIN_Q2I_FIELD_IMP_2G_ANT3_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_I 0x1B8 */
#define RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_I_FIELD_IMP_2G_ANT3_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_Q 0x1BC */
#define RF_EMULATOR_IMP_2G_ANT3_RX_SQRT_GMM_Q_FIELD_IMP_2G_ANT3_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_COS_PMM2 0x1C0 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_COS_PMM2_FIELD_IMP_2G_ANT3_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_SIN_PMM2 0x1C4 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_SIN_PMM2_FIELD_IMP_2G_ANT3_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_I 0x1C8 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_DC_I_FIELD_IMP_2G_ANT3_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_DC_Q 0x1CC */
#define RF_EMULATOR_IMP_2G_ANT3_TX_DC_Q_FIELD_IMP_2G_ANT3_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_I2Q 0x1D0 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_LIN_I2Q_FIELD_IMP_2G_ANT3_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_LIN_Q2I 0x1D4 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_LIN_Q2I_FIELD_IMP_2G_ANT3_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_I 0x1D8 */
#define RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_I_FIELD_IMP_2G_ANT3_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_Q 0x1DC */
#define RF_EMULATOR_IMP_2G_ANT3_TX_SQRT_GMM_Q_FIELD_IMP_2G_ANT3_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS2PIF 0x1E0 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_COS2PIF_FIELD_IMP_5G_ANT0_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN2PIF 0x1E4 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_SIN2PIF_FIELD_IMP_5G_ANT0_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_COS_PMM2 0x1E8 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_COS_PMM2_FIELD_IMP_5G_ANT0_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_SIN_PMM2 0x1EC */
#define RF_EMULATOR_IMP_5G_ANT0_RX_SIN_PMM2_FIELD_IMP_5G_ANT0_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_I 0x1F0 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_DC_I_FIELD_IMP_5G_ANT0_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_DC_Q 0x1F4 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_DC_Q_FIELD_IMP_5G_ANT0_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_I2Q 0x1F8 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_LIN_I2Q_FIELD_IMP_5G_ANT0_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_LIN_Q2I 0x1FC */
#define RF_EMULATOR_IMP_5G_ANT0_RX_LIN_Q2I_FIELD_IMP_5G_ANT0_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_I 0x200 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_I_FIELD_IMP_5G_ANT0_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_Q 0x204 */
#define RF_EMULATOR_IMP_5G_ANT0_RX_SQRT_GMM_Q_FIELD_IMP_5G_ANT0_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_COS_PMM2 0x208 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_COS_PMM2_FIELD_IMP_5G_ANT0_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_SIN_PMM2 0x20C */
#define RF_EMULATOR_IMP_5G_ANT0_TX_SIN_PMM2_FIELD_IMP_5G_ANT0_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_I 0x210 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_DC_I_FIELD_IMP_5G_ANT0_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_DC_Q 0x214 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_DC_Q_FIELD_IMP_5G_ANT0_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_I2Q 0x218 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_LIN_I2Q_FIELD_IMP_5G_ANT0_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_LIN_Q2I 0x21C */
#define RF_EMULATOR_IMP_5G_ANT0_TX_LIN_Q2I_FIELD_IMP_5G_ANT0_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_I 0x220 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_I_FIELD_IMP_5G_ANT0_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_Q 0x224 */
#define RF_EMULATOR_IMP_5G_ANT0_TX_SQRT_GMM_Q_FIELD_IMP_5G_ANT0_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS2PIF 0x228 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_COS2PIF_FIELD_IMP_5G_ANT1_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN2PIF 0x22C */
#define RF_EMULATOR_IMP_5G_ANT1_RX_SIN2PIF_FIELD_IMP_5G_ANT1_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_COS_PMM2 0x230 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_COS_PMM2_FIELD_IMP_5G_ANT1_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_SIN_PMM2 0x234 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_SIN_PMM2_FIELD_IMP_5G_ANT1_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_I 0x238 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_DC_I_FIELD_IMP_5G_ANT1_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_DC_Q 0x23C */
#define RF_EMULATOR_IMP_5G_ANT1_RX_DC_Q_FIELD_IMP_5G_ANT1_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_I2Q 0x240 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_LIN_I2Q_FIELD_IMP_5G_ANT1_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_LIN_Q2I 0x244 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_LIN_Q2I_FIELD_IMP_5G_ANT1_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_I 0x248 */
#define RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_I_FIELD_IMP_5G_ANT1_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_Q 0x24C */
#define RF_EMULATOR_IMP_5G_ANT1_RX_SQRT_GMM_Q_FIELD_IMP_5G_ANT1_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_COS_PMM2 0x250 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_COS_PMM2_FIELD_IMP_5G_ANT1_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_SIN_PMM2 0x254 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_SIN_PMM2_FIELD_IMP_5G_ANT1_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_I 0x258 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_DC_I_FIELD_IMP_5G_ANT1_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_DC_Q 0x25C */
#define RF_EMULATOR_IMP_5G_ANT1_TX_DC_Q_FIELD_IMP_5G_ANT1_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_I2Q 0x260 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_LIN_I2Q_FIELD_IMP_5G_ANT1_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_LIN_Q2I 0x264 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_LIN_Q2I_FIELD_IMP_5G_ANT1_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_I 0x268 */
#define RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_I_FIELD_IMP_5G_ANT1_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_Q 0x26C */
#define RF_EMULATOR_IMP_5G_ANT1_TX_SQRT_GMM_Q_FIELD_IMP_5G_ANT1_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS2PIF 0x270 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_COS2PIF_FIELD_IMP_5G_ANT2_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN2PIF 0x274 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_SIN2PIF_FIELD_IMP_5G_ANT2_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_COS_PMM2 0x278 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_COS_PMM2_FIELD_IMP_5G_ANT2_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_SIN_PMM2 0x27C */
#define RF_EMULATOR_IMP_5G_ANT2_RX_SIN_PMM2_FIELD_IMP_5G_ANT2_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_I 0x280 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_DC_I_FIELD_IMP_5G_ANT2_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_DC_Q 0x284 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_DC_Q_FIELD_IMP_5G_ANT2_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_I2Q 0x288 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_LIN_I2Q_FIELD_IMP_5G_ANT2_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_LIN_Q2I 0x28C */
#define RF_EMULATOR_IMP_5G_ANT2_RX_LIN_Q2I_FIELD_IMP_5G_ANT2_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_I 0x290 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_I_FIELD_IMP_5G_ANT2_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_Q 0x294 */
#define RF_EMULATOR_IMP_5G_ANT2_RX_SQRT_GMM_Q_FIELD_IMP_5G_ANT2_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_COS_PMM2 0x298 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_COS_PMM2_FIELD_IMP_5G_ANT2_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_SIN_PMM2 0x29C */
#define RF_EMULATOR_IMP_5G_ANT2_TX_SIN_PMM2_FIELD_IMP_5G_ANT2_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_I 0x2A0 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_DC_I_FIELD_IMP_5G_ANT2_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_DC_Q 0x2A4 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_DC_Q_FIELD_IMP_5G_ANT2_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_I2Q 0x2A8 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_LIN_I2Q_FIELD_IMP_5G_ANT2_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_LIN_Q2I 0x2AC */
#define RF_EMULATOR_IMP_5G_ANT2_TX_LIN_Q2I_FIELD_IMP_5G_ANT2_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_I 0x2B0 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_I_FIELD_IMP_5G_ANT2_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_Q 0x2B4 */
#define RF_EMULATOR_IMP_5G_ANT2_TX_SQRT_GMM_Q_FIELD_IMP_5G_ANT2_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS2PIF 0x2B8 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_COS2PIF_FIELD_IMP_5G_ANT3_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN2PIF 0x2BC */
#define RF_EMULATOR_IMP_5G_ANT3_RX_SIN2PIF_FIELD_IMP_5G_ANT3_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_COS_PMM2 0x2C0 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_COS_PMM2_FIELD_IMP_5G_ANT3_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_SIN_PMM2 0x2C4 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_SIN_PMM2_FIELD_IMP_5G_ANT3_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_I 0x2C8 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_DC_I_FIELD_IMP_5G_ANT3_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_DC_Q 0x2CC */
#define RF_EMULATOR_IMP_5G_ANT3_RX_DC_Q_FIELD_IMP_5G_ANT3_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_I2Q 0x2D0 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_LIN_I2Q_FIELD_IMP_5G_ANT3_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_LIN_Q2I 0x2D4 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_LIN_Q2I_FIELD_IMP_5G_ANT3_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_I 0x2D8 */
#define RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_I_FIELD_IMP_5G_ANT3_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_Q 0x2DC */
#define RF_EMULATOR_IMP_5G_ANT3_RX_SQRT_GMM_Q_FIELD_IMP_5G_ANT3_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_SIN_PMM2 0x2E0 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_SIN_PMM2_FIELD_IMP_5G_ANT3_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_COS_PMM2 0x2E4 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_COS_PMM2_FIELD_IMP_5G_ANT3_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_I 0x2E8 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_DC_I_FIELD_IMP_5G_ANT3_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_DC_Q 0x2EC */
#define RF_EMULATOR_IMP_5G_ANT3_TX_DC_Q_FIELD_IMP_5G_ANT3_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_I2Q 0x2F0 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_LIN_I2Q_FIELD_IMP_5G_ANT3_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_LIN_Q2I 0x2F4 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_LIN_Q2I_FIELD_IMP_5G_ANT3_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_I 0x2F8 */
#define RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_I_FIELD_IMP_5G_ANT3_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_Q 0x2FC */
#define RF_EMULATOR_IMP_5G_ANT3_TX_SQRT_GMM_Q_FIELD_IMP_5G_ANT3_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS2PIF 0x300 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_COS2PIF_FIELD_IMP_6G_ANT0_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN2PIF 0x304 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_SIN2PIF_FIELD_IMP_6G_ANT0_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_COS_PMM2 0x308 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_COS_PMM2_FIELD_IMP_6G_ANT0_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_SIN_PMM2 0x30C */
#define RF_EMULATOR_IMP_6G_ANT0_RX_SIN_PMM2_FIELD_IMP_6G_ANT0_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_I 0x310 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_DC_I_FIELD_IMP_6G_ANT0_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_DC_Q 0x314 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_DC_Q_FIELD_IMP_6G_ANT0_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_I2Q 0x318 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_LIN_I2Q_FIELD_IMP_6G_ANT0_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_LIN_Q2I 0x31C */
#define RF_EMULATOR_IMP_6G_ANT0_RX_LIN_Q2I_FIELD_IMP_6G_ANT0_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_I 0x320 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_I_FIELD_IMP_6G_ANT0_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_Q 0x324 */
#define RF_EMULATOR_IMP_6G_ANT0_RX_SQRT_GMM_Q_FIELD_IMP_6G_ANT0_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_COS_PMM2 0x328 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_COS_PMM2_FIELD_IMP_6G_ANT0_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_SIN_PMM2 0x32C */
#define RF_EMULATOR_IMP_6G_ANT0_TX_SIN_PMM2_FIELD_IMP_6G_ANT0_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_I 0x330 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_DC_I_FIELD_IMP_6G_ANT0_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_DC_Q 0x334 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_DC_Q_FIELD_IMP_6G_ANT0_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_I2Q 0x338 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_LIN_I2Q_FIELD_IMP_6G_ANT0_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_LIN_Q2I 0x33C */
#define RF_EMULATOR_IMP_6G_ANT0_TX_LIN_Q2I_FIELD_IMP_6G_ANT0_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_I 0x340 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_I_FIELD_IMP_6G_ANT0_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_Q 0x344 */
#define RF_EMULATOR_IMP_6G_ANT0_TX_SQRT_GMM_Q_FIELD_IMP_6G_ANT0_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS2PIF 0x348 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_COS2PIF_FIELD_IMP_6G_ANT1_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN2PIF 0x34C */
#define RF_EMULATOR_IMP_6G_ANT1_RX_SIN2PIF_FIELD_IMP_6G_ANT1_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_COS_PMM2 0x350 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_COS_PMM2_FIELD_IMP_6G_ANT1_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_SIN_PMM2 0x354 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_SIN_PMM2_FIELD_IMP_6G_ANT1_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_I 0x358 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_DC_I_FIELD_IMP_6G_ANT1_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_DC_Q 0x35C */
#define RF_EMULATOR_IMP_6G_ANT1_RX_DC_Q_FIELD_IMP_6G_ANT1_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_I2Q 0x360 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_LIN_I2Q_FIELD_IMP_6G_ANT1_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_LIN_Q2I 0x364 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_LIN_Q2I_FIELD_IMP_6G_ANT1_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_I 0x368 */
#define RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_I_FIELD_IMP_6G_ANT1_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_Q 0x36C */
#define RF_EMULATOR_IMP_6G_ANT1_RX_SQRT_GMM_Q_FIELD_IMP_6G_ANT1_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_COS_PMM2 0x370 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_COS_PMM2_FIELD_IMP_6G_ANT1_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_SIN_PMM2 0x374 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_SIN_PMM2_FIELD_IMP_6G_ANT1_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_I 0x378 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_DC_I_FIELD_IMP_6G_ANT1_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_DC_Q 0x37C */
#define RF_EMULATOR_IMP_6G_ANT1_TX_DC_Q_FIELD_IMP_6G_ANT1_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_I2Q 0x380 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_LIN_I2Q_FIELD_IMP_6G_ANT1_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_LIN_Q2I 0x384 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_LIN_Q2I_FIELD_IMP_6G_ANT1_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_I 0x388 */
#define RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_I_FIELD_IMP_6G_ANT1_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_Q 0x38C */
#define RF_EMULATOR_IMP_6G_ANT1_TX_SQRT_GMM_Q_FIELD_IMP_6G_ANT1_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS2PIF 0x390 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_COS2PIF_FIELD_IMP_6G_ANT2_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN2PIF 0x394 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_SIN2PIF_FIELD_IMP_6G_ANT2_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_COS_PMM2 0x398 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_COS_PMM2_FIELD_IMP_6G_ANT2_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_SIN_PMM2 0x39C */
#define RF_EMULATOR_IMP_6G_ANT2_RX_SIN_PMM2_FIELD_IMP_6G_ANT2_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_I 0x3A0 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_DC_I_FIELD_IMP_6G_ANT2_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_DC_Q 0x3A4 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_DC_Q_FIELD_IMP_6G_ANT2_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_I2Q 0x3A8 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_LIN_I2Q_FIELD_IMP_6G_ANT2_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_LIN_Q2I 0x3AC */
#define RF_EMULATOR_IMP_6G_ANT2_RX_LIN_Q2I_FIELD_IMP_6G_ANT2_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_I 0x3B0 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_I_FIELD_IMP_6G_ANT2_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_Q 0x3B4 */
#define RF_EMULATOR_IMP_6G_ANT2_RX_SQRT_GMM_Q_FIELD_IMP_6G_ANT2_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_COS_PMM2 0x3B8 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_COS_PMM2_FIELD_IMP_6G_ANT2_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_SIN_PMM2 0x3BC */
#define RF_EMULATOR_IMP_6G_ANT2_TX_SIN_PMM2_FIELD_IMP_6G_ANT2_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_I 0x3C0 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_DC_I_FIELD_IMP_6G_ANT2_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_DC_Q 0x3C4 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_DC_Q_FIELD_IMP_6G_ANT2_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_I2Q 0x3C8 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_LIN_I2Q_FIELD_IMP_6G_ANT2_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_LIN_Q2I 0x3CC */
#define RF_EMULATOR_IMP_6G_ANT2_TX_LIN_Q2I_FIELD_IMP_6G_ANT2_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_I 0x3D0 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_I_FIELD_IMP_6G_ANT2_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_Q 0x3D4 */
#define RF_EMULATOR_IMP_6G_ANT2_TX_SQRT_GMM_Q_FIELD_IMP_6G_ANT2_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS2PIF 0x3D8 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_COS2PIF_FIELD_IMP_6G_ANT3_RX_COS2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN2PIF 0x3DC */
#define RF_EMULATOR_IMP_6G_ANT3_RX_SIN2PIF_FIELD_IMP_6G_ANT3_RX_SIN2PIF_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_COS_PMM2 0x3E0 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_COS_PMM2_FIELD_IMP_6G_ANT3_RX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_SIN_PMM2 0x3E4 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_SIN_PMM2_FIELD_IMP_6G_ANT3_RX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_I 0x3E8 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_DC_I_FIELD_IMP_6G_ANT3_RX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_DC_Q 0x3EC */
#define RF_EMULATOR_IMP_6G_ANT3_RX_DC_Q_FIELD_IMP_6G_ANT3_RX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_I2Q 0x3F0 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_LIN_I2Q_FIELD_IMP_6G_ANT3_RX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_LIN_Q2I 0x3F4 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_LIN_Q2I_FIELD_IMP_6G_ANT3_RX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_I 0x3F8 */
#define RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_I_FIELD_IMP_6G_ANT3_RX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_Q 0x3FC */
#define RF_EMULATOR_IMP_6G_ANT3_RX_SQRT_GMM_Q_FIELD_IMP_6G_ANT3_RX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_COS_PMM2 0x400 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_COS_PMM2_FIELD_IMP_6G_ANT3_TX_COS_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_SIN_PMM2 0x404 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_SIN_PMM2_FIELD_IMP_6G_ANT3_TX_SIN_PMM2_MASK                                    0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_I 0x408 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_DC_I_FIELD_IMP_6G_ANT3_TX_DC_I_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_DC_Q 0x40C */
#define RF_EMULATOR_IMP_6G_ANT3_TX_DC_Q_FIELD_IMP_6G_ANT3_TX_DC_Q_MASK                                            0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_I2Q 0x410 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_LIN_I2Q_FIELD_IMP_6G_ANT3_TX_LIN_I2Q_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_LIN_Q2I 0x414 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_LIN_Q2I_FIELD_IMP_6G_ANT3_TX_LIN_Q2I_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_I 0x418 */
#define RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_I_FIELD_IMP_6G_ANT3_TX_SQRT_GMM_I_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_Q 0x41C */
#define RF_EMULATOR_IMP_6G_ANT3_TX_SQRT_GMM_Q_FIELD_IMP_6G_ANT3_TX_SQRT_GMM_Q_MASK                                0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_COS2PIF 0x420 */
#define RF_EMULATOR_IMP_5G_DFS_RX_COS2PIF_FIELD_IMP_5G_DFS_RX_COS2PIF_MASK                                        0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN2PIF 0x424 */
#define RF_EMULATOR_IMP_5G_DFS_RX_SIN2PIF_FIELD_IMP_5G_DFS_RX_SIN2PIF_MASK                                        0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_COS_PMM2 0x428 */
#define RF_EMULATOR_IMP_5G_DFS_RX_COS_PMM2_FIELD_IMP_5G_DFS_RX_COS_PMM2_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_SIN_PMM2 0x42C */
#define RF_EMULATOR_IMP_5G_DFS_RX_SIN_PMM2_FIELD_IMP_5G_DFS_RX_SIN_PMM2_MASK                                      0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_I 0x430 */
#define RF_EMULATOR_IMP_5G_DFS_RX_DC_I_FIELD_IMP_5G_DFS_RX_DC_I_MASK                                              0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_DC_Q 0x434 */
#define RF_EMULATOR_IMP_5G_DFS_RX_DC_Q_FIELD_IMP_5G_DFS_RX_DC_Q_MASK                                              0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_I2Q 0x438 */
#define RF_EMULATOR_IMP_5G_DFS_RX_LIN_I2Q_FIELD_IMP_5G_DFS_RX_LIN_I2Q_MASK                                        0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_LIN_Q2I 0x43C */
#define RF_EMULATOR_IMP_5G_DFS_RX_LIN_Q2I_FIELD_IMP_5G_DFS_RX_LIN_Q2I_MASK                                        0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_I 0x440 */
#define RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_I_FIELD_IMP_5G_DFS_RX_SQRT_GMM_I_MASK                                  0x00FFFFFF
//========================================
/* REG_RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_Q 0x444 */
#define RF_EMULATOR_IMP_5G_DFS_RX_SQRT_GMM_Q_FIELD_IMP_5G_DFS_RX_SQRT_GMM_Q_MASK                                  0x00FFFFFF


#endif // _RF_EMULATOR_REGS_H_
