
/***********************************************************************************
File:				PhyRxBeRegs.h
Module:				phyRxBe
SOC Revision:		latest
Generated at:       2024-06-26 12:55:17
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _PHY_RX_BE_REGS_H_
#define _PHY_RX_BE_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define PHY_RX_BE_BASE_ADDRESS                                    MEMORY_MAP_UNIT_201_BASE_ADDRESS
#define	REG_PHY_RX_BE_PHY_RXBE_REG04                              (PHY_RX_BE_BASE_ADDRESS + 0x10)
#define	REG_PHY_RX_BE_PHY_RXBE_REG05                              (PHY_RX_BE_BASE_ADDRESS + 0x14)
#define	REG_PHY_RX_BE_PHY_RXBE_REG07                              (PHY_RX_BE_BASE_ADDRESS + 0x1C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG08                              (PHY_RX_BE_BASE_ADDRESS + 0x20)
#define	REG_PHY_RX_BE_PHY_RXBE_REG18                              (PHY_RX_BE_BASE_ADDRESS + 0x60)
#define	REG_PHY_RX_BE_PHY_RXBE_REG19                              (PHY_RX_BE_BASE_ADDRESS + 0x64)
#define	REG_PHY_RX_BE_PHY_RXBE_REG1E                              (PHY_RX_BE_BASE_ADDRESS + 0x78)
#define	REG_PHY_RX_BE_PHY_RXBE_REG20                              (PHY_RX_BE_BASE_ADDRESS + 0x80)
#define	REG_PHY_RX_BE_PHY_RXBE_REG21                              (PHY_RX_BE_BASE_ADDRESS + 0x84)
#define	REG_PHY_RX_BE_PHY_RXBE_REG22                              (PHY_RX_BE_BASE_ADDRESS + 0x88)
#define	REG_PHY_RX_BE_PHY_RXBE_REG23                              (PHY_RX_BE_BASE_ADDRESS + 0x8C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG24                              (PHY_RX_BE_BASE_ADDRESS + 0x90)
#define	REG_PHY_RX_BE_PHY_RXBE_REG25                              (PHY_RX_BE_BASE_ADDRESS + 0x94)
#define	REG_PHY_RX_BE_PHY_RXBE_REG26                              (PHY_RX_BE_BASE_ADDRESS + 0x98)
#define	REG_PHY_RX_BE_PHY_RXBE_REG28                              (PHY_RX_BE_BASE_ADDRESS + 0xA0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG34                              (PHY_RX_BE_BASE_ADDRESS + 0xD0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG35                              (PHY_RX_BE_BASE_ADDRESS + 0xD4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG36                              (PHY_RX_BE_BASE_ADDRESS + 0xD8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG37                              (PHY_RX_BE_BASE_ADDRESS + 0xDC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG38                              (PHY_RX_BE_BASE_ADDRESS + 0xE0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG39                              (PHY_RX_BE_BASE_ADDRESS + 0xE4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG3F                              (PHY_RX_BE_BASE_ADDRESS + 0xFC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG41                              (PHY_RX_BE_BASE_ADDRESS + 0x104)
#define	REG_PHY_RX_BE_PHY_RXBE_REG42                              (PHY_RX_BE_BASE_ADDRESS + 0x108)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4A                              (PHY_RX_BE_BASE_ADDRESS + 0x128)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4B                              (PHY_RX_BE_BASE_ADDRESS + 0x12C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4C                              (PHY_RX_BE_BASE_ADDRESS + 0x130)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4D                              (PHY_RX_BE_BASE_ADDRESS + 0x134)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4E                              (PHY_RX_BE_BASE_ADDRESS + 0x138)
#define	REG_PHY_RX_BE_PHY_RXBE_REG4F                              (PHY_RX_BE_BASE_ADDRESS + 0x13C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG50                              (PHY_RX_BE_BASE_ADDRESS + 0x140)
#define	REG_PHY_RX_BE_PHY_RXBE_REG52                              (PHY_RX_BE_BASE_ADDRESS + 0x148)
#define	REG_PHY_RX_BE_PHY_RXBE_REG55                              (PHY_RX_BE_BASE_ADDRESS + 0x154)
#define	REG_PHY_RX_BE_PHY_RXBE_REG56                              (PHY_RX_BE_BASE_ADDRESS + 0x158)
#define	REG_PHY_RX_BE_PHY_RXBE_REG57                              (PHY_RX_BE_BASE_ADDRESS + 0x15C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG58                              (PHY_RX_BE_BASE_ADDRESS + 0x160)
#define	REG_PHY_RX_BE_PHY_RXBE_REG59                              (PHY_RX_BE_BASE_ADDRESS + 0x164)
#define	REG_PHY_RX_BE_PHY_RXBE_REG5A                              (PHY_RX_BE_BASE_ADDRESS + 0x168)
#define	REG_PHY_RX_BE_PHY_RXBE_REG5D                              (PHY_RX_BE_BASE_ADDRESS + 0x174)
#define	REG_PHY_RX_BE_PHY_RXBE_REG5E                              (PHY_RX_BE_BASE_ADDRESS + 0x178)
#define	REG_PHY_RX_BE_PHY_RXBE_REG64                              (PHY_RX_BE_BASE_ADDRESS + 0x190)
#define	REG_PHY_RX_BE_PHY_RXBE_REG65                              (PHY_RX_BE_BASE_ADDRESS + 0x194)
#define	REG_PHY_RX_BE_PHY_RXBE_REG67                              (PHY_RX_BE_BASE_ADDRESS + 0x19C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG68                              (PHY_RX_BE_BASE_ADDRESS + 0x1A0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG69                              (PHY_RX_BE_BASE_ADDRESS + 0x1A4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6A                              (PHY_RX_BE_BASE_ADDRESS + 0x1A8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6B                              (PHY_RX_BE_BASE_ADDRESS + 0x1AC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6C                              (PHY_RX_BE_BASE_ADDRESS + 0x1B0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6D                              (PHY_RX_BE_BASE_ADDRESS + 0x1B4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6E                              (PHY_RX_BE_BASE_ADDRESS + 0x1B8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG6F                              (PHY_RX_BE_BASE_ADDRESS + 0x1BC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG70                              (PHY_RX_BE_BASE_ADDRESS + 0x1C0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG71                              (PHY_RX_BE_BASE_ADDRESS + 0x1C4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG72                              (PHY_RX_BE_BASE_ADDRESS + 0x1C8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG73                              (PHY_RX_BE_BASE_ADDRESS + 0x1CC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG74                              (PHY_RX_BE_BASE_ADDRESS + 0x1D0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG75                              (PHY_RX_BE_BASE_ADDRESS + 0x1D4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG76                              (PHY_RX_BE_BASE_ADDRESS + 0x1D8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG77                              (PHY_RX_BE_BASE_ADDRESS + 0x1DC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG78                              (PHY_RX_BE_BASE_ADDRESS + 0x1E0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG79                              (PHY_RX_BE_BASE_ADDRESS + 0x1E4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG80                              (PHY_RX_BE_BASE_ADDRESS + 0x1E8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG81                              (PHY_RX_BE_BASE_ADDRESS + 0x1EC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG82                              (PHY_RX_BE_BASE_ADDRESS + 0x1F0)
#define	REG_PHY_RX_BE_PHY_RXBE_REG83                              (PHY_RX_BE_BASE_ADDRESS + 0x1F4)
#define	REG_PHY_RX_BE_PHY_RXBE_REG84                              (PHY_RX_BE_BASE_ADDRESS + 0x1F8)
#define	REG_PHY_RX_BE_PHY_RXBE_REG85                              (PHY_RX_BE_BASE_ADDRESS + 0x1FC)
#define	REG_PHY_RX_BE_PHY_RXBE_REG86                              (PHY_RX_BE_BASE_ADDRESS + 0x200)
#define	REG_PHY_RX_BE_PHY_RXBE_REG87                              (PHY_RX_BE_BASE_ADDRESS + 0x204)
#define	REG_PHY_RX_BE_PHY_RXBE_REG88                              (PHY_RX_BE_BASE_ADDRESS + 0x208)
#define	REG_PHY_RX_BE_PHY_RXBE_REG89                              (PHY_RX_BE_BASE_ADDRESS + 0x20C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG90                              (PHY_RX_BE_BASE_ADDRESS + 0x210)
#define	REG_PHY_RX_BE_PHY_RXBE_REG91                              (PHY_RX_BE_BASE_ADDRESS + 0x214)
#define	REG_PHY_RX_BE_PHY_RXBE_REG92                              (PHY_RX_BE_BASE_ADDRESS + 0x218)
#define	REG_PHY_RX_BE_PHY_RXBE_REG93                              (PHY_RX_BE_BASE_ADDRESS + 0x21C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG94                              (PHY_RX_BE_BASE_ADDRESS + 0x220)
#define	REG_PHY_RX_BE_PHY_RXBE_REG95                              (PHY_RX_BE_BASE_ADDRESS + 0x224)
#define	REG_PHY_RX_BE_PHY_RXBE_REG96                              (PHY_RX_BE_BASE_ADDRESS + 0x228)
#define	REG_PHY_RX_BE_PHY_RXBE_REG97                              (PHY_RX_BE_BASE_ADDRESS + 0x22C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG98                              (PHY_RX_BE_BASE_ADDRESS + 0x230)
#define	REG_PHY_RX_BE_PHY_RXBE_REG99                              (PHY_RX_BE_BASE_ADDRESS + 0x234)
#define	REG_PHY_RX_BE_PHY_RXBE_REG9A                              (PHY_RX_BE_BASE_ADDRESS + 0x238)
#define	REG_PHY_RX_BE_PHY_RXBE_REG9B                              (PHY_RX_BE_BASE_ADDRESS + 0x23C)
#define	REG_PHY_RX_BE_PHY_RXBE_REG9C                              (PHY_RX_BE_BASE_ADDRESS + 0x240)
#define	REG_PHY_RX_BE_PHY_RXBE_REGA1                              (PHY_RX_BE_BASE_ADDRESS + 0x254)
#define	REG_PHY_RX_BE_PHY_RXBE_REGA2                              (PHY_RX_BE_BASE_ADDRESS + 0x258)
#define	REG_PHY_RX_BE_LDPC_CALC_VALID                             (PHY_RX_BE_BASE_ADDRESS + 0x25C)
#define	REG_PHY_RX_BE_LDPC_VITERBI_USER_ACTIVE                    (PHY_RX_BE_BASE_ADDRESS + 0x260)
#define	REG_PHY_RX_BE_LDPC_USER_ACTIVE                            (PHY_RX_BE_BASE_ADDRESS + 0x264)
#define	REG_PHY_RX_BE_LDPC_MAX_N_CW                               (PHY_RX_BE_BASE_ADDRESS + 0x268)
#define	REG_PHY_RX_BE_LDPC_CYCLES_PER_CW                          (PHY_RX_BE_BASE_ADDRESS + 0x26C)
#define	REG_PHY_RX_BE_LDPC_MAX_CHECK_CONV                         (PHY_RX_BE_BASE_ADDRESS + 0x270)
#define	REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_0                  (PHY_RX_BE_BASE_ADDRESS + 0x274)
#define	REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_1                  (PHY_RX_BE_BASE_ADDRESS + 0x278)
#define	REG_PHY_RX_BE_RCR_ACTIVE_USERS_0                          (PHY_RX_BE_BASE_ADDRESS + 0x27C)
#define	REG_PHY_RX_BE_RCR_ACTIVE_USERS_1                          (PHY_RX_BE_BASE_ADDRESS + 0x280)
#define	REG_PHY_RX_BE_SIGB_STID_0                                 (PHY_RX_BE_BASE_ADDRESS + 0x284)
#define	REG_PHY_RX_BE_SIGB_STID_1                                 (PHY_RX_BE_BASE_ADDRESS + 0x288)
#define	REG_PHY_RX_BE_SIGB_STID_2                                 (PHY_RX_BE_BASE_ADDRESS + 0x28C)
#define	REG_PHY_RX_BE_SIGB_STID_3                                 (PHY_RX_BE_BASE_ADDRESS + 0x290)
#define	REG_PHY_RX_BE_SIGB_STID_4                                 (PHY_RX_BE_BASE_ADDRESS + 0x294)
#define	REG_PHY_RX_BE_SIGB_STID_5                                 (PHY_RX_BE_BASE_ADDRESS + 0x298)
#define	REG_PHY_RX_BE_SIGB_STID_6                                 (PHY_RX_BE_BASE_ADDRESS + 0x29C)
#define	REG_PHY_RX_BE_SIGB_STID_7                                 (PHY_RX_BE_BASE_ADDRESS + 0x2A0)
#define	REG_PHY_RX_BE_SIGB_STID_8                                 (PHY_RX_BE_BASE_ADDRESS + 0x2A4)
#define	REG_PHY_RX_BE_SIGB_STID_9                                 (PHY_RX_BE_BASE_ADDRESS + 0x2A8)
#define	REG_PHY_RX_BE_SIGB_STID_10                                (PHY_RX_BE_BASE_ADDRESS + 0x2AC)
#define	REG_PHY_RX_BE_SIGB_STID_11                                (PHY_RX_BE_BASE_ADDRESS + 0x2B0)
#define	REG_PHY_RX_BE_SIGB_STID_12                                (PHY_RX_BE_BASE_ADDRESS + 0x2B4)
#define	REG_PHY_RX_BE_SIGB_STID_13                                (PHY_RX_BE_BASE_ADDRESS + 0x2B8)
#define	REG_PHY_RX_BE_SIGB_STID_14                                (PHY_RX_BE_BASE_ADDRESS + 0x2BC)
#define	REG_PHY_RX_BE_SIGB_STID_15                                (PHY_RX_BE_BASE_ADDRESS + 0x2D0)
#define	REG_PHY_RX_BE_LDPC_CYCLES_LAST_SYMBOL                     (PHY_RX_BE_BASE_ADDRESS + 0x2D4)
#define	REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE                    (PHY_RX_BE_BASE_ADDRESS + 0x2D8)
#define	REG_PHY_RX_BE_LDPC_CYCLES_FIRST_CW                        (PHY_RX_BE_BASE_ADDRESS + 0x2DC)
#define	REG_PHY_RX_BE_LDPC_LAST_SYMBOL_CW_INDEX                   (PHY_RX_BE_BASE_ADDRESS + 0x2E0)
#define	REG_PHY_RX_BE_LDPC_CORE_GCLK_CHICKEN_BIT                  (PHY_RX_BE_BASE_ADDRESS + 0x2E4)
#define	REG_PHY_RX_BE_RXBE_FSM_FORCE                              (PHY_RX_BE_BASE_ADDRESS + 0x2F0)
#define	REG_PHY_RX_BE_PM_LEG_MCS_TH                               (PHY_RX_BE_BASE_ADDRESS + 0x2F8)
#define	REG_PHY_RX_BE_PM_LEG_EVM_TH                               (PHY_RX_BE_BASE_ADDRESS + 0x2FC)
#define	REG_PHY_RX_BE_LEG_BELOW_EVM_TH_ERR                        (PHY_RX_BE_BASE_ADDRESS + 0x300)
#define	REG_PHY_RX_BE_PM_MASK_LPLUSR                              (PHY_RX_BE_BASE_ADDRESS + 0x304)
#define	REG_PHY_RX_BE_PM_R_PLUS_L_MINIMUM_PACKET_SIZE             (PHY_RX_BE_BASE_ADDRESS + 0x308)
#define	REG_PHY_RX_BE_DESCRAMBLER_SEED                            (PHY_RX_BE_BASE_ADDRESS + 0x30C)
#define	REG_PHY_RX_BE_PM_SD_REGISTER                              (PHY_RX_BE_BASE_ADDRESS + 0x310)
#define	REG_PHY_RX_BE_PM_DS_REGISTER                              (PHY_RX_BE_BASE_ADDRESS + 0x314)
#define	REG_PHY_RX_BE_SIGB_ERROR_CODE                             (PHY_RX_BE_BASE_ADDRESS + 0x318)
#define	REG_PHY_RX_BE_HE_MU_COMPRESSED_MODE_INDICATION            (PHY_RX_BE_BASE_ADDRESS + 0x31C)
#define	REG_PHY_RX_BE_HW_MCS_UNSUPPORTED_CAUSE                    (PHY_RX_BE_BASE_ADDRESS + 0x320)
#define	REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS                   (PHY_RX_BE_BASE_ADDRESS + 0x324)
#define	REG_PHY_RX_BE_ROP_REGISTER                                (PHY_RX_BE_BASE_ADDRESS + 0x328)
#define	REG_PHY_RX_BE_EXT_RANGE_READ_ONLY                         (PHY_RX_BE_BASE_ADDRESS + 0x32C)
#define	REG_PHY_RX_BE_DISABLE_NEW_ER_MODE                         (PHY_RX_BE_BASE_ADDRESS + 0x330)
#define	REG_PHY_RX_BE_PM_MAC_DATA_0                               (PHY_RX_BE_BASE_ADDRESS + 0x334)
#define	REG_PHY_RX_BE_PM_MAC_DATA_1                               (PHY_RX_BE_BASE_ADDRESS + 0x338)
#define	REG_PHY_RX_BE_PM_MAC_DATA_2                               (PHY_RX_BE_BASE_ADDRESS + 0x33C)
#define	REG_PHY_RX_BE_PM_MAC_DATA_3                               (PHY_RX_BE_BASE_ADDRESS + 0x340)
#define	REG_PHY_RX_BE_PM_MAC_DATA_READY                           (PHY_RX_BE_BASE_ADDRESS + 0x344)
#define	REG_PHY_RX_BE_DC_CNT_RRAM_EN_CHK_BIT                      (PHY_RX_BE_BASE_ADDRESS + 0x348)
#define	REG_PHY_RX_BE_PM_MIN_BURST_LENGTH                         (PHY_RX_BE_BASE_ADDRESS + 0x34C)
#define	REG_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL                    (PHY_RX_BE_BASE_ADDRESS + 0x350)
#define	REG_PHY_RX_BE_PM_FORCE_RU_SIZE_7                          (PHY_RX_BE_BASE_ADDRESS + 0x354)
#define	REG_PHY_RX_BE_PM_EHT_SCRAMBLER_ENABLE                     (PHY_RX_BE_BASE_ADDRESS + 0x358)
#define	REG_PHY_RX_BE_LDPC_CONVERGENCE_PER_USER                   (PHY_RX_BE_BASE_ADDRESS + 0x35C)
#define	REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_0                         (PHY_RX_BE_BASE_ADDRESS + 0x360)
#define	REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_1                         (PHY_RX_BE_BASE_ADDRESS + 0x364)
#define	REG_PHY_RX_BE_NUM_OF_CORES                                (PHY_RX_BE_BASE_ADDRESS + 0x368)
#define	REG_PHY_RX_BE_USIG_OVERFLOW                               (PHY_RX_BE_BASE_ADDRESS + 0x36C)
#define	REG_PHY_RX_BE_EHT_INFO                                    (PHY_RX_BE_BASE_ADDRESS + 0x370)
#define	REG_PHY_RX_BE_SPARE_FROM_FD0                              (PHY_RX_BE_BASE_ADDRESS + 0x374)
#define	REG_PHY_RX_BE_SPARE_FROM_FD1                              (PHY_RX_BE_BASE_ADDRESS + 0x378)
#define	REG_PHY_RX_BE_SPARE_FROM_TX                               (PHY_RX_BE_BASE_ADDRESS + 0x37C)
#define	REG_PHY_RX_BE_SPARE_FROM_TD                               (PHY_RX_BE_BASE_ADDRESS + 0x380)
#define	REG_PHY_RX_BE_SPARE_TO_FD0                                (PHY_RX_BE_BASE_ADDRESS + 0x384)
#define	REG_PHY_RX_BE_SPARE_TO_FD1                                (PHY_RX_BE_BASE_ADDRESS + 0x388)
#define	REG_PHY_RX_BE_SPARE_TO_TX                                 (PHY_RX_BE_BASE_ADDRESS + 0x38C)
#define	REG_PHY_RX_BE_SPARE_TO_TD                                 (PHY_RX_BE_BASE_ADDRESS + 0x390)
#define	REG_PHY_RX_BE_LDPC_CYCLES_SS_OV                           (PHY_RX_BE_BASE_ADDRESS + 0x394)
#define	REG_PHY_RX_BE_LDPC_CYCLE_LAST_SYM_OV                      (PHY_RX_BE_BASE_ADDRESS + 0x398)
#define	REG_PHY_RX_BE_LDPC_LAST_SYM_CW_IDX                        (PHY_RX_BE_BASE_ADDRESS + 0x39C)
#define	REG_PHY_RX_BE_USIG_0                                      (PHY_RX_BE_BASE_ADDRESS + 0x3A0)
#define	REG_PHY_RX_BE_USIG_1                                      (PHY_RX_BE_BASE_ADDRESS + 0x3A4)
#define	REG_PHY_RX_BE_EHT_0                                       (PHY_RX_BE_BASE_ADDRESS + 0x3A8)
#define	REG_PHY_RX_BE_EHT_1                                       (PHY_RX_BE_BASE_ADDRESS + 0x3AC)
#define	REG_PHY_RX_BE_IS_320_UPPER_CHANNEL                        (PHY_RX_BE_BASE_ADDRESS + 0x3B0)
#define	REG_PHY_RX_BE_TEST_BUS_GCLK_EN                            (PHY_RX_BE_BASE_ADDRESS + 0x3B4)
#define	REG_PHY_RX_BE_BYTE_ERROR_CONTOL                           (PHY_RX_BE_BASE_ADDRESS + 0x3B8)
#define	REG_PHY_RX_BE_BYTE_ERROR_RATE                             (PHY_RX_BE_BASE_ADDRESS + 0x3BC)
#define	REG_PHY_RX_BE_BYTE_ERROR_RATE_RD                          (PHY_RX_BE_BASE_ADDRESS + 0x3C0)
#define	REG_PHY_RX_BE_DFE_CONTROL                                 (PHY_RX_BE_BASE_ADDRESS + 0x3C4)
#define	REG_PHY_RX_BE_DFE_VALID                                   (PHY_RX_BE_BASE_ADDRESS + 0x3C8)
#define	REG_PHY_RX_BE_EVM_PPRU_RAM_READY                          (PHY_RX_BE_BASE_ADDRESS + 0x3CC)
#define	REG_PHY_RX_BE_EHT_SIG_INFO_VALID                          (PHY_RX_BE_BASE_ADDRESS + 0x3D0)
#define	REG_PHY_RX_BE_LSIG_ERROR_CAUSE                            (PHY_RX_BE_BASE_ADDRESS + 0x3D4)
#define	REG_PHY_RX_BE_MAXIMUM_PACKET_SIZE                         (PHY_RX_BE_BASE_ADDRESS + 0x3D8)
#define	REG_PHY_RX_BE_SIG3_ERROR_CAUSE                            (PHY_RX_BE_BASE_ADDRESS + 0x3DC)
#define	REG_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE                       (PHY_RX_BE_BASE_ADDRESS + 0x3E0)
#define	REG_PHY_RX_BE_EHTSIG_ERROR_CAUSE                          (PHY_RX_BE_BASE_ADDRESS + 0x3E4)
#define	REG_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE                     (PHY_RX_BE_BASE_ADDRESS + 0x3E8)
#define	REG_PHY_RX_BE_RX_REWIND_CONFIG                            (PHY_RX_BE_BASE_ADDRESS + 0x3EC)
#define	REG_PHY_RX_BE_HAMMING_DISTANCE_ENABLE                     (PHY_RX_BE_BASE_ADDRESS + 0x3F0)
#define	REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE    (PHY_RX_BE_BASE_ADDRESS + 0x3F4)
#define	REG_PHY_RX_BE_PM_MAX_NSYM_REWIND                          (PHY_RX_BE_BASE_ADDRESS + 0x3F8)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_2G_0                            (PHY_RX_BE_BASE_ADDRESS + 0x3FC)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_2G_1                            (PHY_RX_BE_BASE_ADDRESS + 0x400)
#define	REG_PHY_RX_BE_PM_FD_OVERHEAD                              (PHY_RX_BE_BASE_ADDRESS + 0x404)
#define	REG_PHY_RX_BE_PM_TSYM_ACC_MAC_OVERRIDE                    (PHY_RX_BE_BASE_ADDRESS + 0x408)
#define	REG_PHY_RX_BE_PM_MIN_NSYM_REWIND                          (PHY_RX_BE_BASE_ADDRESS + 0x40C)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_0                     (PHY_RX_BE_BASE_ADDRESS + 0x410)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_1                     (PHY_RX_BE_BASE_ADDRESS + 0x414)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_2                     (PHY_RX_BE_BASE_ADDRESS + 0x418)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_3                     (PHY_RX_BE_BASE_ADDRESS + 0x41C)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_4                     (PHY_RX_BE_BASE_ADDRESS + 0x420)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_5                     (PHY_RX_BE_BASE_ADDRESS + 0x424)
#define	REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_6                     (PHY_RX_BE_BASE_ADDRESS + 0x428)
#define	REG_PHY_RX_BE_REWIND_READ_ONLY_0                          (PHY_RX_BE_BASE_ADDRESS + 0x42C)
#define	REG_PHY_RX_BE_REWIND_READ_ONLY_1                          (PHY_RX_BE_BASE_ADDRESS + 0x430)
#define	REG_PHY_RX_BE_PM_TSYM_ACC_FD_LOW_LIMIT                    (PHY_RX_BE_BASE_ADDRESS + 0x434)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_5G_0                            (PHY_RX_BE_BASE_ADDRESS + 0x438)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_5G_1                            (PHY_RX_BE_BASE_ADDRESS + 0x43C)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_6G_0                            (PHY_RX_BE_BASE_ADDRESS + 0x440)
#define	REG_PHY_RX_BE_PM_QR_LIMIT_6G_1                            (PHY_RX_BE_BASE_ADDRESS + 0x444)
#define	REG_PHY_RX_BE_LDPC_MAX_CYCLES                             (PHY_RX_BE_BASE_ADDRESS + 0x448)
#define	REG_PHY_RX_BE_DFE_POST_REWIND_ENABLE                      (PHY_RX_BE_BASE_ADDRESS + 0x44C)
#define	REG_PHY_RX_BE_LDPC_CYCLES_STEADY_REWIND_RO                (PHY_RX_BE_BASE_ADDRESS + 0x450)
#define	REG_PHY_RX_BE_PM_REWIND_ENABLE                            (PHY_RX_BE_BASE_ADDRESS + 0x454)
#define	REG_PHY_RX_BE_DFE_ENABLE_POST_REWIND                      (PHY_RX_BE_BASE_ADDRESS + 0x458)
#define	REG_PHY_RX_BE_PM_REWIND_ENABLE_WHEN_DFE_OFF               (PHY_RX_BE_BASE_ADDRESS + 0x45C)
#define	REG_PHY_RX_BE_PM_TSYM_ACC_TX_LOW_LIMIT                    (PHY_RX_BE_BASE_ADDRESS + 0x460)
#define	REG_PHY_RX_BE_PM_REWIND_POWER_SAVE                        (PHY_RX_BE_BASE_ADDRESS + 0x464)
#define	REG_PHY_RX_BE_REWIND_CANCEL_RO                            (PHY_RX_BE_BASE_ADDRESS + 0x468)
#define	REG_PHY_RX_BE_NCW_TH_REWIND_CANCEL_RO                     (PHY_RX_BE_BASE_ADDRESS + 0x46C)
#define	REG_PHY_RX_BE_PM_FORCE_CONV_FAIL_ON_CW                    (PHY_RX_BE_BASE_ADDRESS + 0x470)
#define	REG_PHY_RX_BE_PM_SUPPORT_HE_MU_DL                         (PHY_RX_BE_BASE_ADDRESS + 0x474)
#define	REG_PHY_RX_BE_CW_CONV_CNT_PRE_REWIND                      (PHY_RX_BE_BASE_ADDRESS + 0x478)
#define	REG_PHY_RX_BE_CW_CONV_CNT_POST_REWIND                     (PHY_RX_BE_BASE_ADDRESS + 0x47C)
#define	REG_PHY_RX_BE_CHK_BIT_EXTRA_LDPC_CHECK                    (PHY_RX_BE_BASE_ADDRESS + 0x480)
#define	REG_PHY_RX_BE_HE_STATION_INFO                             (PHY_RX_BE_BASE_ADDRESS + 0x484)
#define	REG_PHY_RX_BE_DFE_LOW_RATES_EN                            (PHY_RX_BE_BASE_ADDRESS + 0x488)
#define	REG_PHY_RX_BE_DFE_MAX_NSYM_WAIT_VHT                       (PHY_RX_BE_BASE_ADDRESS + 0x48C)
#define	REG_PHY_RX_BE_DFE_REQ_CW_VEC                              (PHY_RX_BE_BASE_ADDRESS + 0x490)
#define	REG_PHY_RX_BE_PM_MIN_NSYM_DFE_REWIND                      (PHY_RX_BE_BASE_ADDRESS + 0x494)
#define	REG_PHY_RX_BE_SENSING_MAC_INPUTS                          (PHY_RX_BE_BASE_ADDRESS + 0x498)
#define	REG_PHY_RX_BE_SENSING_MAC_VALID                           (PHY_RX_BE_BASE_ADDRESS + 0x49C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_PHY_RX_BE_PHY_RXBE_REG04 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpPhyMacEventLen : 1; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 tpPhyMacDataLen : 1; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 tpPhyMacStationIdLen : 1; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 tpLdpcCwStatLen : 1; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 tpLlrStreamLen : 1; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
		uint32 tpDebugCntEn : 1; //Test Plug counter enable, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg04_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG05 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpLlrStreamSel : 2; //RxBE Test bus control, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxBePhyRxbeReg05_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG07 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpDebugClk2Strb : 4; //Test Bus debug clock per strobe, reset value: 0x6, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBePhyRxbeReg07_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG08 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tpLowMuxCtrl : 2; //RxBE low test bus control, reset value: 0x0, access type: RW
		uint32 tpMidMuxCtrl : 2; //RxBE mid test bus control, reset value: 0x1, access type: RW
		uint32 tpHighMuxCtrl : 2; //RxBE high test bus control, reset value: 0x2, access type: RW
		uint32 tpFourMuxCtrl : 2; //RxBE high test bus control, reset value: 0x3, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBePhyRxbeReg08_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG18 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 viterbiTbDepth : 6; //Viterbi length, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBePhyRxbeReg18_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG19 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reportFullMetrics : 1; //Report full metrics, reset value: 0x0, access type: RW
		uint32 waitForGriscMetrics : 1; //Wait for gen risc metrics, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 waitForSnrMetrics : 1; //Wait for snr metrics, reset value: 0x1, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegPhyRxBePhyRxbeReg19_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG1E 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 numBitsInPacket : 25; //Number of bits in the packet, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
	} bitFields;
} RegPhyRxBePhyRxbeReg1E_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG20 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 macIfMinCycle : 8; //Cycle length, reset value: 0x4, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg20_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG21 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetLimit : 19; //Errors per Packet limit, reset value: 0x7ffff, access type: RW
		uint32 reserved0 : 13;
	} bitFields;
} RegPhyRxBePhyRxbeReg21_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG22 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsLoadVal : 32; //PRBS LSFR init value, reset value: 0x162, access type: CONST
	} bitFields;
} RegPhyRxBePhyRxbeReg22_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG23 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsCrcEnable : 1; //Enable PRBS Comparison, reset value: 0x0, access type: RW
		uint32 packetWithCrc : 1; //Enable CRC in packet, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxBePhyRxbeReg23_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG24 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearCounters : 1; //Clear PRBS Counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePhyRxbeReg24_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG25 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 latchCounters : 1; //Latch PRBS Counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePhyRxbeReg25_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG26 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsType : 32; //PRBS Polynom type, reset value: 0x80000003, access type: CONST
	} bitFields;
} RegPhyRxBePhyRxbeReg26_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG28 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigDecodeCrcLen : 3; //Signal CRC length, reset value: 0x7, access type: CONST
		uint32 reserved0 : 1;
		uint32 sigDecodeCrcType : 8; //Signal CRC type, reset value: 0x60, access type: CONST
		uint32 sigDecodeCrcCorrupt : 1; //Signal CRC corrupt, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 sigDecodeCrcInit : 8; //Signal CRC Init value, reset value: 0xff, access type: CONST
		uint32 reserved2 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg28_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG34 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsErrCntLatched : 32; //PRBS Error count - latched, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg34_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG35 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsBitCntLatched310 : 32; //PRBS Bit count [31:0] - latched, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg35_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG36 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsBitCntLatched4732 : 16; //PRBS Bit count [47:32] - latched, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBePhyRxbeReg36_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG37 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcPacketCntLatched : 32; //Number of compared packets - latched, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg37_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG38 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcErrPacketCntLatched : 32; //Number of compared errored packets with CRC error - latched, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg38_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG39 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsErrPacketCntLatched : 32; //Number of compared errored packets with PRBS error - latched, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg39_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG3F 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swBistStart : 1; //BIST start, reset value: 0x0, access type: RW
		uint32 clearRamMode : 1; //Clear RAM mode in BIST, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxBePhyRxbeReg3F_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG41 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxbeMaskParityErr : 1; //Mask parity error in signal 1, reset value: 0x0, access type: RW
		uint32 rxbeMaskRateErr : 1; //Mask invalid rate error in signal 1, reset value: 0x0, access type: RW
		uint32 rxbeMaskPSizeErr : 1; //Mask packet size error in signal 1, reset value: 0x0, access type: RW
		uint32 rxbeMaskReservedErr : 1; //Mask reserved bit error in signal 1, reset value: 0x0, access type: RW
		uint32 rxbeMaskCrcErr : 1; //Mask bad CRC error in signal 3, reset value: 0x0, access type: RW
		uint32 rxbeMaskSigbCrcErr : 1; //Mask bad CRC error in vht sigb, reset value: 0x0, access type: RW
		uint32 rxbeMaskSig3ReservedErr : 1; //Mask reserved bit error in signal 2/3, reset value: 0x0, access type: RW
		uint32 rxbeMaskSig3LengthErr : 1; //Mask min length check based on sig3 and sig1, reset value: 0x0, access type: RW
		uint32 rxbeMaskNdpErr : 1; //Mask ndp error, reset value: 0x0, access type: RW
		uint32 rxbeMaskNdpErrAll : 1; //rxbe_mask_p_max_size_err, reset value: 0x1, access type: RW
		uint32 rxbeMaskPMaxSizeErr : 1; //rxbe_mask_ndp_err_all, reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegPhyRxBePhyRxbeReg41_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG42 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimumPacketSize : 12; //Minimum packet size in bytes, reset value: 0xc, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePhyRxbeReg42_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4A 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsErrCnt : 32; //PRBS Error count - real time, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg4A_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4B 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsBitCnt310 : 32; //PRBS Bit count [31:0] - real time, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg4B_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4C 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsBitCnt4732 : 16; //PRBS Bit count [47:32] - real time, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBePhyRxbeReg4C_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4D 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcPacketCnt : 32; //Number of compared packets - real time, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg4D_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4E 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcErrPacketCnt : 32; //Number of compared errored packets with CRC error  - real time, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg4E_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG4F 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prbsErrPacketCnt : 32; //Number of compared errored packets with PRBS error - real time, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg4F_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG50 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacIfBitCounter : 23; //Phy MAC IF bit counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegPhyRxBePhyRxbeReg50_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG52 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxbeGpr : 32; //RxBE general purpose register, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg52_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG55 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deintRamRm : 3; //deint_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBePhyRxbeReg55_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG56 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rcrRamRm : 3; //rcr_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBePhyRxbeReg56_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG57 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 viterbiTbBlockRamRm : 3; //viterbi_tb_block_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBePhyRxbeReg57_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG58 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userRamRm : 3; //user_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBePhyRxbeReg58_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG59 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muRamRm : 3; //mu_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBePhyRxbeReg59_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG5A 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rRam2Rm : 3; //r_ram2_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 obufRamRm : 3; //obuf_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 sigbRamRm : 3; //sigb_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 cwParams96RamRm : 3; //cw_params_96_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 cwParams48RamRm : 3; //cw_params_48_ram_rm, reset value: 0x3, access type: RW
		uint32 burstBufferRm : 3; //burst_buffer, reset value: 0x3, access type: RW
		uint32 reserved4 : 10;
	} bitFields;
} RegPhyRxBePhyRxbeReg5A_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG5D 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gclkShutdownEn : 14; //gclk shutdown - when '1' clocks will be always deactive, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxBePhyRxbeReg5D_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG5E 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bistScrBypass : 1; //bist_scr_bypass, reset value: 0x0, access type: RW
		uint32 ramTestMode : 1; //ram_test_mode, reset value: 0x0, access type: RW
		uint32 ramTestRnm : 1; //ram_test_rnm, reset value: 0x0, access type: RW
		uint32 memGlobalRm : 2; //mem_global_rm, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxBePhyRxbeReg5E_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG64 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalRdyLimit : 16; //global_rdy_limit, reset value: 0x1CC0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBePhyRxbeReg64_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG65 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableAirTimeBlock : 1; //enable_air_time_block, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePhyRxbeReg65_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG67 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vhtNonAggregate : 1; //vht_non_aggregate, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePhyRxbeReg67_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG68 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr0 : 32; //spare_gpr_0, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg68_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG69 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr1 : 32; //spare_gpr_1, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg69_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6A 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr2 : 32; //spare_gpr_2, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg6A_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6B 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr3 : 32; //spare_gpr_3, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBePhyRxbeReg6B_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6C 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txGlobalResetMask : 12; //reset rxbe units for tx, reset value: 0xfdf, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePhyRxbeReg6C_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6D 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscAirTimeResult : 17; //risc_air_time_result, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegPhyRxBePhyRxbeReg6D_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6E 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscNumSym : 15; //risc_num_sym, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxBePhyRxbeReg6E_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG6F 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscPhyRate : 15; //risc_phy_rate, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxBePhyRxbeReg6F_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG70 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 evmResultSs0 : 8; //evm_result_ss0, reset value: 0x0, access type: RO
		uint32 evmResultSs1 : 8; //evm_result_ss1, reset value: 0x0, access type: RO
		uint32 evmResultSs2 : 8; //evm_result_ss2, reset value: 0x0, access type: RO
		uint32 evmResultSs3 : 8; //evm_result_ss3, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePhyRxbeReg70_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG71 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vhtSigA1 : 24; //vht_sig_a1, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg71_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG72 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vhtSigA2 : 24; //vht_sig_a2, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg72_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG73 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vhtSigB : 24; //vht_sig_b, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg73_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG74 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htSig2 : 24; //ht_sig2, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg74_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG75 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htSig3 : 24; //ht_sig3, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg75_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG76 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lsig : 24; //lsig, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxBePhyRxbeReg76_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG77 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfHeaderDelayLimit : 8; //bf_header_delay_limit, reset value: 0xf0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBePhyRxbeReg77_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG78 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour0 : 6; //pm_colour_0, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour1 : 6; //pm_colour_1, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour2 : 6; //pm_colour_2, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour3 : 6; //pm_colour_3, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg78_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG79 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour4 : 6; //pm_colour_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour5 : 6; //pm_colour_5, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour6 : 6; //pm_colour_6, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour7 : 6; //pm_colour_7, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg79_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG80 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour8 : 6; //pm_colour_8, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour9 : 6; //pm_colour_9, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour10 : 6; //pm_colour_10, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour11 : 6; //pm_colour_11, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg80_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG81 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour12 : 6; //pm_colour_12, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour13 : 6; //pm_colour_13, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour14 : 6; //pm_colour_14, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour15 : 6; //pm_colour_15, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg81_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG82 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour16 : 6; //pm_colour_16, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour17 : 6; //pm_colour_17, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour18 : 6; //pm_colour_18, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour19 : 6; //pm_colour_19, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg82_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG83 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour20 : 6; //pm_colour_20, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour21 : 6; //pm_colour_21, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour22 : 6; //pm_colour_22, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour23 : 6; //pm_colour_23, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg83_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG84 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour24 : 6; //pm_colour_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour25 : 6; //pm_colour_25, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour26 : 6; //pm_colour_26, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour27 : 6; //pm_colour_27, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg84_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG85 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour28 : 6; //pm_colour_28, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour29 : 6; //pm_colour_29, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour30 : 6; //pm_colour_30, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour31 : 6; //pm_colour_31, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg85_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG86 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour32 : 6; //pm_colour_32, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour33 : 6; //pm_colour_33, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour34 : 6; //pm_colour_34, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour35 : 6; //pm_colour_35, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg86_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG87 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour36 : 6; //pm_colour_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour37 : 6; //pm_colour_37, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour38 : 6; //pm_colour_38, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour39 : 6; //pm_colour_39, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg87_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG88 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour40 : 6; //pm_colour_40, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour41 : 6; //pm_colour_41, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour42 : 6; //pm_colour_42, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour43 : 6; //pm_colour_43, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg88_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG89 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour44 : 6; //pm_colour_44, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour45 : 6; //pm_colour_45, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour46 : 6; //pm_colour_46, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour47 : 6; //pm_colour_47, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg89_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG90 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour48 : 6; //pm_colour_48, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour49 : 6; //pm_colour_49, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour50 : 6; //pm_colour_50, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour51 : 6; //pm_colour_51, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg90_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG91 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour52 : 6; //pm_colour_52, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour53 : 6; //pm_colour_53, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour54 : 6; //pm_colour_54, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour55 : 6; //pm_colour_55, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg91_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG92 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour56 : 6; //pm_colour_56, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour57 : 6; //pm_colour_57, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour58 : 6; //pm_colour_58, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour59 : 6; //pm_colour_59, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg92_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG93 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmColour60 : 6; //pm_colour_60, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 pmColour61 : 6; //pm_colour_61, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmColour62 : 6; //pm_colour_62, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 pmColour63 : 6; //pm_colour_63, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxBePhyRxbeReg93_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG94 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAirTimeInterval : 10; //pm_air_time_interval, reset value: 0x14a, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegPhyRxBePhyRxbeReg94_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG95 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCurrentUser : 6; //pm_current_user, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBePhyRxbeReg95_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG96 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcTotalLastSym : 8; //ldpc_total_last_sym, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBePhyRxbeReg96_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG97 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcTotalCw : 16; //ldpc_total_cw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBePhyRxbeReg97_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG98 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigA1 : 26; //he_sig_a1, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBePhyRxbeReg98_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG99 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigA2 : 26; //he_sig_a2, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBePhyRxbeReg99_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG9A 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psduLen : 23; //psdu_len, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegPhyRxBePhyRxbeReg9A_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG9B 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userConfig : 12; //user_config, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePhyRxbeReg9B_u;

/*REG_PHY_RX_BE_PHY_RXBE_REG9C 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxbeSchPs : 5; //rxbe_sch_ps, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxBePhyRxbeReg9C_u;

/*REG_PHY_RX_BE_PHY_RXBE_REGA1 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcAlphaIndex : 2; //ldpc_alpha_index, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxBePhyRxbeRega1_u;

/*REG_PHY_RX_BE_PHY_RXBE_REGA2 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcQuantizeCycles : 1; //ldpc_quantize_cycles, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePhyRxbeRega2_u;

/*REG_PHY_RX_BE_LDPC_CALC_VALID 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCalcValid : 1; //ldpc cycles calculator done, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeLdpcCalcValid_u;

/*REG_PHY_RX_BE_LDPC_VITERBI_USER_ACTIVE 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcViterbiUserActive : 6; //number of active users viterbi+ldpc, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBeLdpcViterbiUserActive_u;

/*REG_PHY_RX_BE_LDPC_USER_ACTIVE 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcUserActive : 6; //number of active users ldpc only, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBeLdpcUserActive_u;

/*REG_PHY_RX_BE_LDPC_MAX_N_CW 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcMaxNCw : 16; //maximal value of ldpc_n_cw from all ldpc users, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeLdpcMaxNCw_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_PER_CW 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesPerCw : 12; //calculated cycles per cw, reset value: 0x480, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeLdpcCyclesPerCw_u;

/*REG_PHY_RX_BE_LDPC_MAX_CHECK_CONV 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcMaxChecksRo : 16; //ldpc_max_checks_ro, reset value: 0x0, access type: RO
		uint32 ldpcNoConvergence : 1; //ldpc_no_convergence, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegPhyRxBeLdpcMaxCheckConv_u;

/*REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_0 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 busPacPhyRxCrcCheck0 : 32; //bus_pac_phy_rx_crc_check_0, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeBusPacPhyRxCrcCheck0_u;

/*REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_1 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 busPacPhyRxCrcCheck1 : 4; //bus_pac_phy_rx_crc_check_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBeBusPacPhyRxCrcCheck1_u;

/*REG_PHY_RX_BE_RCR_ACTIVE_USERS_0 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rcrActiveUsers0 : 32; //rcr_active_users_0, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeRcrActiveUsers0_u;

/*REG_PHY_RX_BE_RCR_ACTIVE_USERS_1 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rcrActiveUsers1 : 4; //rcr_active_users_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBeRcrActiveUsers1_u;

/*REG_PHY_RX_BE_SIGB_STID_0 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid0 : 11; //sigb_stid_0, reset value: 0x0, access type: RW
		uint32 sigbStid0Enable : 1; //sigb_stid_0_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid1 : 11; //sigb_stid_1, reset value: 0x0, access type: RW
		uint32 sigbStid1Enable : 1; //sigb_stid_1_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid0_u;

/*REG_PHY_RX_BE_SIGB_STID_1 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid2 : 11; //sigb_stid_2, reset value: 0x0, access type: RW
		uint32 sigbStid2Enable : 1; //sigb_stid_2_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid3 : 11; //sigb_stid_3, reset value: 0x0, access type: RW
		uint32 sigbStid3Enable : 1; //sigb_stid_3_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid1_u;

/*REG_PHY_RX_BE_SIGB_STID_2 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid4 : 11; //sigb_stid_4, reset value: 0x0, access type: RW
		uint32 sigbStid4Enable : 1; //sigb_stid_4_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid5 : 11; //sigb_stid_5, reset value: 0x0, access type: RW
		uint32 sigbStid5Enable : 1; //sigb_stid_5_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid2_u;

/*REG_PHY_RX_BE_SIGB_STID_3 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid6 : 11; //sigb_stid_6, reset value: 0x0, access type: RW
		uint32 sigbStid6Enable : 1; //sigb_stid_6_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid7 : 11; //sigb_stid_7, reset value: 0x0, access type: RW
		uint32 sigbStid7Enable : 1; //sigb_stid_7_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid3_u;

/*REG_PHY_RX_BE_SIGB_STID_4 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid8 : 11; //sigb_stid_8, reset value: 0x0, access type: RW
		uint32 sigbStid8Enable : 1; //sigb_stid_8_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid9 : 11; //sigb_stid_9, reset value: 0x0, access type: RW
		uint32 sigbStid9Enable : 1; //sigb_stid_9_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid4_u;

/*REG_PHY_RX_BE_SIGB_STID_5 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid10 : 11; //sigb_stid_10, reset value: 0x0, access type: RW
		uint32 sigbStid10Enable : 1; //sigb_stid_10_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid11 : 11; //sigb_stid_11, reset value: 0x0, access type: RW
		uint32 sigbStid11Enable : 1; //sigb_stid_11_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid5_u;

/*REG_PHY_RX_BE_SIGB_STID_6 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid12 : 11; //sigb_stid_12, reset value: 0x0, access type: RW
		uint32 sigbStid12Enable : 1; //sigb_stid_12_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid13 : 11; //sigb_stid_13, reset value: 0x0, access type: RW
		uint32 sigbStid13Enable : 1; //sigb_stid_13_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid6_u;

/*REG_PHY_RX_BE_SIGB_STID_7 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid14 : 11; //sigb_stid_14, reset value: 0x0, access type: RW
		uint32 sigbStid14Enable : 1; //sigb_stid_14_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid15 : 11; //sigb_stid_15, reset value: 0x0, access type: RW
		uint32 sigbStid15Enable : 1; //sigb_stid_15_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid7_u;

/*REG_PHY_RX_BE_SIGB_STID_8 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid16 : 11; //sigb_stid_16, reset value: 0x0, access type: RW
		uint32 sigbStid16Enable : 1; //sigb_stid_16_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid17 : 11; //sigb_stid_17, reset value: 0x0, access type: RW
		uint32 sigbStid17Enable : 1; //sigb_stid_17_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid8_u;

/*REG_PHY_RX_BE_SIGB_STID_9 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid18 : 11; //sigb_stid_18, reset value: 0x0, access type: RW
		uint32 sigbStid18Enable : 1; //sigb_stid_18_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid19 : 11; //sigb_stid_19, reset value: 0x0, access type: RW
		uint32 sigbStid19Enable : 1; //sigb_stid_19_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid9_u;

/*REG_PHY_RX_BE_SIGB_STID_10 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid20 : 11; //sigb_stid_20, reset value: 0x0, access type: RW
		uint32 sigbStid20Enable : 1; //sigb_stid_20_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid21 : 11; //sigb_stid_21, reset value: 0x0, access type: RW
		uint32 sigbStid21Enable : 1; //sigb_stid_21_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid10_u;

/*REG_PHY_RX_BE_SIGB_STID_11 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid22 : 11; //sigb_stid_22, reset value: 0x0, access type: RW
		uint32 sigbStid22Enable : 1; //sigb_stid_22_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid23 : 11; //sigb_stid_23, reset value: 0x0, access type: RW
		uint32 sigbStid23Enable : 1; //sigb_stid_23_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid11_u;

/*REG_PHY_RX_BE_SIGB_STID_12 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid24 : 11; //sigb_stid_24, reset value: 0x0, access type: RW
		uint32 sigbStid24Enable : 1; //sigb_stid_24_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid25 : 11; //sigb_stid_25, reset value: 0x0, access type: RW
		uint32 sigbStid25Enable : 1; //sigb_stid_25_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid12_u;

/*REG_PHY_RX_BE_SIGB_STID_13 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid26 : 11; //sigb_stid_26, reset value: 0x0, access type: RW
		uint32 sigbStid26Enable : 1; //sigb_stid_26_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid27 : 11; //sigb_stid_27, reset value: 0x0, access type: RW
		uint32 sigbStid27Enable : 1; //sigb_stid_27_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid13_u;

/*REG_PHY_RX_BE_SIGB_STID_14 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid28 : 11; //sigb_stid_28, reset value: 0x0, access type: RW
		uint32 sigbStid28Enable : 1; //sigb_stid_28_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid29 : 11; //sigb_stid_29, reset value: 0x0, access type: RW
		uint32 sigbStid29Enable : 1; //sigb_stid_29_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid14_u;

/*REG_PHY_RX_BE_SIGB_STID_15 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbStid30 : 11; //sigb_stid_30, reset value: 0x0, access type: RW
		uint32 sigbStid30Enable : 1; //sigb_stid_30_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 sigbStid31 : 11; //sigb_stid_31, reset value: 0x0, access type: RW
		uint32 sigbStid31Enable : 1; //sigb_stid_31_enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeSigbStid15_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_LAST_SYMBOL 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesLastSymbol : 12; //ldpc_cycles_last_symbol, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeLdpcCyclesLastSymbol_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesSteadyState : 12; //ldpc_cycles_steady_state, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeLdpcCyclesSteadyState_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_FIRST_CW 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesFirstCw : 12; //ldpc_cycles_first_cw, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeLdpcCyclesFirstCw_u;

/*REG_PHY_RX_BE_LDPC_LAST_SYMBOL_CW_INDEX 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcLastSymbolCwIndex : 16; //ldpc_last_symbol_cw_index, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeLdpcLastSymbolCwIndex_u;

/*REG_PHY_RX_BE_LDPC_CORE_GCLK_CHICKEN_BIT 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCoreGclkChickenBit : 1; //bypass internal clock gating of ldpc core clock, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeLdpcCoreGclkChickenBit_u;

/*REG_PHY_RX_BE_RXBE_FSM_FORCE 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmForceLsigErr : 1; //pm_force_lsig_err, reset value: 0x0, access type: RW
		uint32 pmForceLplusrErPass : 1; //pm_force_lplusr_er_pass , reset value: 0x0, access type: RW
		uint32 pmForceLsigNotEqualRlsig : 1; //pm_force_lsog_not_equal_rlsig, reset value: 0x0, access type: RW
		uint32 pmForceLsigMcsEvmFailCond : 1; //pm_force_lsig_mcs_evm_fail_cond , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBeRxbeFsmForce_u;

/*REG_PHY_RX_BE_PM_LEG_MCS_TH 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLegMcsTh : 4; //pm_leg_mcs_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBePmLegMcsTh_u;

/*REG_PHY_RX_BE_PM_LEG_EVM_TH 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLegEvmTh : 8; //pm_leg_evm_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBePmLegEvmTh_u;

/*REG_PHY_RX_BE_LEG_BELOW_EVM_TH_ERR 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 legBelowEvmThErr : 1; //leg_below_evm_th_err, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeLegBelowEvmThErr_u;

/*REG_PHY_RX_BE_PM_MASK_LPLUSR 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMaskLplusrParityErr : 1; //pm_mask_ lplusr _parity_err, reset value: 0x0, access type: RW
		uint32 pmMaskLplusrRateErr : 1; //pm_mask_ lplusr _rate_err, reset value: 0x0, access type: RW
		uint32 pmMaskLplusrPSizeErr : 1; //pm_mask_ lplusr _p_size_err, reset value: 0x0, access type: RW
		uint32 pmMaskLplusrReserveErr : 1; //pm_mask_ lplusr _reserve_err, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBePmMaskLplusr_u;

/*REG_PHY_RX_BE_PM_R_PLUS_L_MINIMUM_PACKET_SIZE 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRPlusLMinimumPacketSize : 12; //pm_r_plus_l_minimum_packet_size, reset value: 0x13, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePmRPlusLMinimumPacketSize_u;

/*REG_PHY_RX_BE_DESCRAMBLER_SEED 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descramblerSeed : 11; //descrambler_seed, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegPhyRxBeDescramblerSeed_u;

/*REG_PHY_RX_BE_PM_SD_REGISTER 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSdSigbRam : 1; //pm_sd_sigb_ram, reset value: 0x0, access type: RW
		uint32 pmSdViterbiBankRam : 1; //pm_sd_viterbi_bank_ram, reset value: 0x0, access type: RW
		uint32 pmSdDeintRam : 1; //pm_sd_deint_ram, reset value: 0x0, access type: RW
		uint32 pmSdRcrRam : 1; //pm_sd_rcr_ram, reset value: 0x0, access type: RW
		uint32 pmSdCwParamsFifo : 1; //pm_sd_cw_params_fifo, reset value: 0x0, access type: RW
		uint32 pmSdObufCore : 1; //pm_sd_obuf_core, reset value: 0x0, access type: RW
		uint32 pmSdViterbiTbRam : 1; //pm_sd_viterbi_tb_ram, reset value: 0x0, access type: RW
		uint32 pmSdRram2 : 1; //pm_sd_rram2, reset value: 0x0, access type: RW
		uint32 pmSdBurstBuffer0 : 1; //pm_sd_burst_buffer0, reset value: 0x0, access type: RW
		uint32 pmSdBurstBuffer1 : 1; //pm_sd_burst_buffer1, reset value: 0x0, access type: RW
		uint32 pmSdBurstBuffer0Mru : 1; //pm_sd_burst_buffer0_mru, reset value: 0x0, access type: RW
		uint32 pmSdBurstBuffer1Mru : 1; //pm_sd_burst_buffer1_mru, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePmSdRegister_u;

/*REG_PHY_RX_BE_PM_DS_REGISTER 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmDsSigbRam : 1; //pm_ds_sigb_ram, reset value: 0x0, access type: RW
		uint32 pmDsViterbiBankRam : 1; //pm_ds_viterbi_bank_ram, reset value: 0x0, access type: RW
		uint32 pmDsDeintRam : 1; //pm_ds_deint_ram, reset value: 0x0, access type: RW
		uint32 pmDsRcrRam : 1; //pm_ds_rcr_ram, reset value: 0x0, access type: RW
		uint32 pmDsCwParamsFifo : 1; //pm_ds_cw_params_fifo, reset value: 0x0, access type: RW
		uint32 pmDsObufCore : 1; //pm_ds_obuf_core, reset value: 0x0, access type: RW
		uint32 pmDsViterbiTbRam : 1; //pm_ds_viterbi_tb_ram, reset value: 0x0, access type: RW
		uint32 pmDsRram2 : 1; //pm_ds_rram2, reset value: 0x0, access type: RW
		uint32 pmDsBurstBuffer0 : 1; //pm_ds_burst_buffer0, reset value: 0x0, access type: RW
		uint32 pmDsBurstBuffer1 : 1; //pm_ds_burst_buffer1, reset value: 0x0, access type: RW
		uint32 pmDsBurstBuffer0Mru : 1; //pm_ds_burst_buffer0_mru, reset value: 0x0, access type: RW
		uint32 pmDsBurstBuffer1Mru : 1; //pm_ds_burst_buffer1_mru, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePmDsRegister_u;

/*REG_PHY_RX_BE_SIGB_ERROR_CODE 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbErrorCode : 4; //sigb_error_code, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegPhyRxBeSigbErrorCode_u;

/*REG_PHY_RX_BE_HE_MU_COMPRESSED_MODE_INDICATION 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuCompressedModeIndication : 1; //he_mu_compressed_mode_indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeHeMuCompressedModeIndication_u;

/*REG_PHY_RX_BE_HW_MCS_UNSUPPORTED_CAUSE 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hwMcsUnsupportedCause : 26; //hw_mcs_unsupported_cause, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeHwMcsUnsupportedCause_u;

/*REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxbeMaskUnsupportedMcs : 26; //rxbe_mask_unsupported_mcs, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeRxbeMaskUnsupportedMcs_u;

/*REG_PHY_RX_BE_ROP_REGISTER 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ropDeintRam : 1; //rop_deint_ram, reset value: 0x0, access type: RO
		uint32 ropViterbiBankRam : 1; //rop_viterbi_bank_ram, reset value: 0x0, access type: RO
		uint32 ropViterbiTbRam : 1; //rop_viterbi_bank_ram, reset value: 0x0, access type: RO
		uint32 ropRcrRam : 1; //rop_rcr_ram, reset value: 0x0, access type: RO
		uint32 ropSigbRam : 1; //rop_sigb_ram, reset value: 0x0, access type: RO
		uint32 ropCwParamsFifo : 1; //rop_cw_params_fifo, reset value: 0x0, access type: RO
		uint32 ropRram2 : 1; //rop_rram2, reset value: 0x0, access type: RO
		uint32 ropObuf : 1; //rop_obuf, reset value: 0x0, access type: RO
		uint32 ropBurstBuffer0 : 1; //rop_burst_buffer0, reset value: 0x0, access type: RO
		uint32 ropBurstBuffer1 : 1; //rop_burst_buffer1, reset value: 0x0, access type: RO
		uint32 ropBurstBuffer0Mru : 1; //rop_burst_buffer0_mru, reset value: 0x0, access type: RO
		uint32 ropBurstBuffer1Mru : 1; //rop_burst_buffer1_mru, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeRopRegister_u;

/*REG_PHY_RX_BE_EXT_RANGE_READ_ONLY 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heExtBeCriteria : 1; //he_ext_be_criteria, reset value: 0x0, access type: RO
		uint32 heExtBeCriteriaValid : 1; //he_ext_be_criteria_valid, reset value: 0x0, access type: RO
		uint32 weakErCriteria : 1; //weak_er_criteria, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBeExtRangeReadOnly_u;

/*REG_PHY_RX_BE_DISABLE_NEW_ER_MODE 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disableNewErMode : 1; //disable_new_er_mode, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDisableNewErMode_u;

/*REG_PHY_RX_BE_PM_MAC_DATA_0 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMacData0 : 32; //pm_mac_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePmMacData0_u;

/*REG_PHY_RX_BE_PM_MAC_DATA_1 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMacData1 : 32; //pm_mac_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePmMacData1_u;

/*REG_PHY_RX_BE_PM_MAC_DATA_2 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMacData2 : 32; //pm_mac_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePmMacData2_u;

/*REG_PHY_RX_BE_PM_MAC_DATA_3 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMacData3 : 32; //pm_mac_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBePmMacData3_u;

/*REG_PHY_RX_BE_PM_MAC_DATA_READY 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMacDataReady : 1; //pm_mac_data_ready, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmMacDataReady_u;

/*REG_PHY_RX_BE_DC_CNT_RRAM_EN_CHK_BIT 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcCntRramEnChkBit : 1; //dc_cnt_rram_en_chk_bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDcCntRramEnChkBit_u;

/*REG_PHY_RX_BE_PM_MIN_BURST_LENGTH 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMinBurstLength : 5; //pm_min_burst_length, reset value: 0x8, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxBePmMinBurstLength_u;

/*REG_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLastSymbolLdpcHeOffset : 16; //offset to reduce from last symbol in he, reset value: 0x780, access type: RW
		uint32 pmLastSymbolLdpcNonHeOffset : 12; //offset to reduce from last symbol in non he, reset value: 0x0, access type: RW
		uint32 pmLastSymbolLdpcEn : 1; //allow to reduce from last symbol from HW, reset value: 0x0, access type: RW
		uint32 pmLastSymbolLdpcSwEn : 1; //force to reduce from last symbol from SW, reset value: 0x0, access type: RW
		uint32 pmFixLdpcLastSymCalc : 1; //fix last sym num cw calc, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegPhyRxBePmLastSymbolLdpcCtrl_u;

/*REG_PHY_RX_BE_PM_FORCE_RU_SIZE_7 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmForceRuSize7 : 1; //Force RU size 7 , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmForceRuSize7_u;

/*REG_PHY_RX_BE_PM_EHT_SCRAMBLER_ENABLE 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEhtScramblerEnable : 1; //pm_eht_scrambler_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmEhtScramblerEnable_u;

/*REG_PHY_RX_BE_LDPC_CONVERGENCE_PER_USER 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcConvergencePerUser : 32; //--ldpc_convergence_per_user, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeLdpcConvergencePerUser_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_0 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesLimitRate12 : 16; //ldpc_cycles_limit_rate_1_2 , reset value: 0x384, access type: RW
		uint32 ldpcCyclesLimitRate23 : 16; //ldpc_cycles_limit_rate_2_3 , reset value: 0x28a, access type: RW
	} bitFields;
} RegPhyRxBeLdpcCyclesLimit0_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_1 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesLimitRate34 : 16; //ldpc_cycles_limit_rate_3_4, reset value: 0x226, access type: RW
		uint32 ldpcCyclesLimitRate56 : 16; //ldpc_cycles_limit_rate_5_6, reset value: 0x226, access type: RW
	} bitFields;
} RegPhyRxBeLdpcCyclesLimit1_u;

/*REG_PHY_RX_BE_NUM_OF_CORES 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 numOfCores : 3; //num_of_cores , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxBeNumOfCores_u;

/*REG_PHY_RX_BE_USIG_OVERFLOW 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 usigOverflow : 20; //usig_overflow, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxBeUsigOverflow_u;

/*REG_PHY_RX_BE_EHT_INFO 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ehtInfo : 22; //eht_info, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegPhyRxBeEhtInfo_u;

/*REG_PHY_RX_BE_SPARE_FROM_FD0 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd0 : 16; //spare connectivity from FD0, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareFromFd0_u;

/*REG_PHY_RX_BE_SPARE_FROM_FD1 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd1 : 16; //spare connectivity from FD1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareFromFd1_u;

/*REG_PHY_RX_BE_SPARE_FROM_TX 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTx : 16; //spare connectivity from TX, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareFromTx_u;

/*REG_PHY_RX_BE_SPARE_FROM_TD 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTd : 16; //spare connectivity from TD, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareFromTd_u;

/*REG_PHY_RX_BE_SPARE_TO_FD0 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd0 : 16; //spare connectivity to FD0, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareToFd0_u;

/*REG_PHY_RX_BE_SPARE_TO_FD1 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd1 : 16; //spare connectivity to FD1, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareToFd1_u;

/*REG_PHY_RX_BE_SPARE_TO_TX 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTx : 16; //spare connectivity to TX, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareToTx_u;

/*REG_PHY_RX_BE_SPARE_TO_TD 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTd : 16; //spare connectivity to TD, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeSpareToTd_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_SS_OV 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesSteadyStateOverride : 12; //ldpc_cycles_steady_state_override, reset value: 0x0, access type: RW
		uint32 ldpcCyclesSteadyStateOverrideEn : 1; //ldpc_cycles_steady_state_override_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegPhyRxBeLdpcCyclesSsOv_u;

/*REG_PHY_RX_BE_LDPC_CYCLE_LAST_SYM_OV 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesLastSymOverride : 12; //ldpc_cycles_last_sym_override, reset value: 0x0, access type: RW
		uint32 ldpcCyclesLastSymOverrideEn : 1; //ldpc_cycles_last_sym_override_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegPhyRxBeLdpcCycleLastSymOv_u;

/*REG_PHY_RX_BE_LDPC_LAST_SYM_CW_IDX 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcLastSymbolCwIdxOverride : 16; //ldpc_last_symbol_cw_idx_override, reset value: 0x0, access type: RW
		uint32 ldpcLastSymbolCwIdxOverrideEn : 1; //ldpc_last_symbol_cw_idx_override_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegPhyRxBeLdpcLastSymCwIdx_u;

/*REG_PHY_RX_BE_USIG_0 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 usig0 : 26; //usig_0, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeUsig0_u;

/*REG_PHY_RX_BE_USIG_1 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 usig1 : 26; //usig_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeUsig1_u;

/*REG_PHY_RX_BE_EHT_0 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 eht0 : 26; //eht_0, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeEht0_u;

/*REG_PHY_RX_BE_EHT_1 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 eht1 : 26; //eht_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegPhyRxBeEht1_u;

/*REG_PHY_RX_BE_IS_320_UPPER_CHANNEL 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 is320UpperChannel : 1; //is_320_upper_channel, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeIs320UpperChannel_u;

/*REG_PHY_RX_BE_TEST_BUS_GCLK_EN 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusGclkEn : 1; //test_bus_gclk_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeTestBusGclkEn_u;

/*REG_PHY_RX_BE_BYTE_ERROR_CONTOL 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 byteErrorUser : 8; //byte error user index, reset value: 0x0, access type: CONST
		uint32 byteErrorLoad : 1; //load new rate , reset value: 0x0, access type: WO
		uint32 byteErrorClear : 1; //clear error, reset value: 0x0, access type: WO
		uint32 reserved0 : 22;
	} bitFields;
} RegPhyRxBeByteErrorContol_u;

/*REG_PHY_RX_BE_BYTE_ERROR_RATE 0x3BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 byteErrorRate : 32; //ber rate, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBeByteErrorRate_u;

/*REG_PHY_RX_BE_BYTE_ERROR_RATE_RD 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 byteErrorRateRd : 32; //ber rate rd, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeByteErrorRateRd_u;

/*REG_PHY_RX_BE_DFE_CONTROL 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeEnable : 1; //enable dfe, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 dfeMinNumSymbols : 4; //minimal packet length in symbols, reset value: 0x5, access type: RW
		uint32 dfeMinCwPerSymbol : 3; //minimal  cw per symbol, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 dfeVhtMinNumSymbols : 4; //minimal vht packet length in symbols, reset value: 0x6, access type: RW
		uint32 dfeVhtMinCwPerSymbol : 3; //minimal vht cw per symbol, reset value: 0x3, access type: RW
		uint32 reserved2 : 13;
	} bitFields;
} RegPhyRxBeDfeControl_u;

/*REG_PHY_RX_BE_DFE_VALID 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeValid : 1; //packet is applicable for dfe, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDfeValid_u;

/*REG_PHY_RX_BE_EVM_PPRU_RAM_READY 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 evmPpruRamReady : 1; //evm_ppru_ram_ready, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeEvmPpruRamReady_u;

/*REG_PHY_RX_BE_EHT_SIG_INFO_VALID 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ehtSigInfoValid : 1; //evm_sig_info_valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeEhtSigInfoValid_u;

/*REG_PHY_RX_BE_LSIG_ERROR_CAUSE 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sig1ParityErr : 1; //sig1_parity_err, reset value: 0x0, access type: RO
		uint32 sig1RateErr : 1; //sig1_rate_err, reset value: 0x0, access type: RO
		uint32 sig1LengthMinErr : 1; //sig1_length_min_err, reset value: 0x0, access type: RO
		uint32 sig1ReservedErr : 1; //sig1_reserved_err, reset value: 0x0, access type: RO
		uint32 sig1LengthMaxErr : 1; //sig1_length_max_err, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxBeLsigErrorCause_u;

/*REG_PHY_RX_BE_MAXIMUM_PACKET_SIZE 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maximumPacketSize : 12; //maximum_packet_size, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeMaximumPacketSize_u;

/*REG_PHY_RX_BE_SIG3_ERROR_CAUSE 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sig3CrcError : 1; //crc_error, reset value: 0x0, access type: RO
		uint32 sig3McsError : 1; //mcs_error, reset value: 0x0, access type: RO
		uint32 sig3AirTimeErr : 1; //air_time_err, reset value: 0x0, access type: RO
		uint32 sig3NdpPeErr : 1; //ndp_pe_err, reset value: 0x0, access type: RO
		uint32 sig3NdpInvalidHeSigaGiLtfErr : 1; //ndp_invalid_he_siga_gi_ltf_err, reset value: 0x0, access type: RO
		uint32 sig3BccMcsErr : 1; //bcc_mcs_err, reset value: 0x0, access type: RO
		uint32 sig3BccBwErr : 1; //bcc_bw_err, reset value: 0x0, access type: RO
		uint32 sig3HeErErr : 1; //he_er_err, reset value: 0x0, access type: RO
		uint32 sig3WrongBandErr : 1; //wrong_band_err, reset value: 0x0, access type: RO
		uint32 sig3HtPacket : 1; //ht_packet, reset value: 0x0, access type: RO
		uint32 sig3VhtPacket : 1; //vht_packet, reset value: 0x0, access type: RO
		uint32 sig3DcmErr : 1; //dcm_err, reset value: 0x0, access type: RO
		uint32 sig3EhtSigNsymErr : 1; //eht_sig_nsym_err, reset value: 0x0, access type: RO
		uint32 sig3ReservedErr : 1; //sig3_reserved_err, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxBeSig3ErrorCause_u;

/*REG_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maskSig3CrcError : 1; //mask_sig3_crc_error, reset value: 0x0, access type: RW
		uint32 maskSig3McsError : 1; //mask_sig3_mcs_error, reset value: 0x0, access type: RW
		uint32 maskSig3AirTimeErr : 1; //mask_sig3_air_time_err, reset value: 0x0, access type: RW
		uint32 maskSig3NdpPeErr : 1; //mask_sig3_ndp_pe_err, reset value: 0x0, access type: RW
		uint32 maskSig3NdpInvalidHeSigaGiLtfErr : 1; //mask_sig3_ndp_invalid_he_siga_gi_ltf_err, reset value: 0x0, access type: RW
		uint32 maskSig3BccMcsErr : 1; //mask_sig3_bcc_mcs_err, reset value: 0x0, access type: RW
		uint32 maskSig3BccBwErr : 1; //mask_sig3_bcc_bw_err, reset value: 0x0, access type: RW
		uint32 maskSig3HeErErr : 1; //mask_sig3_he_er_err, reset value: 0x0, access type: RW
		uint32 maskSig3WrongBandErr : 1; //mask_sig3_wrong_band_err, reset value: 0x0, access type: RW
		uint32 maskSig3HtPacket : 1; //mask_sig3_ht_packet, reset value: 0x0, access type: RW
		uint32 maskSig3VhtPacket : 1; //mask_sig3_vht_packet, reset value: 0x0, access type: RW
		uint32 maskSig3DcmErr : 1; //mask_sig3_dcm_err, reset value: 0x0, access type: RW
		uint32 maskSig3EhtSigNsymErr : 1; //mask_sig3_eht_sig_nsym_err, reset value: 0x0, access type: RW
		uint32 maskSig3ReservedErr : 1; //mask_sig3_reserved_err, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxBeMaskSig3ErrorCause_u;

/*REG_PHY_RX_BE_EHTSIG_ERROR_CAUSE 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ehtBccMcsErr : 1; //eht_bcc_mcs_err, reset value: 0x0, access type: RO
		uint32 ehtBccBwErr : 1; //eht_bcc_bw_err, reset value: 0x0, access type: RO
		uint32 ehtMcs15Err : 1; //eht_mcs15_err, reset value: 0x0, access type: RO
		uint32 ehtAirTimeErr : 1; //eht_air_time_err, reset value: 0x0, access type: RO
		uint32 ehtNdpErr : 1; //eht_ndp_err, reset value: 0x0, access type: RO
		uint32 ehtSuNehtLtfErr : 1; //eht_su_neht_ltf_err, reset value: 0x0, access type: RO
		uint32 ehtCrcErr : 1; //eht_crc_err, reset value: 0x0, access type: RO
		uint32 ehtDupMode5G : 1; //eht_dup_mode_5g, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBeEhtsigErrorCause_u;

/*REG_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maskEhtBccMcsErr : 1; //mask_eht_bcc_mcs_err, reset value: 0x0, access type: RW
		uint32 maskEhtBccBwErr : 1; //mask_eht_bcc_bw_err, reset value: 0x0, access type: RW
		uint32 maskEhtMcs15Err : 1; //mask_eht_mcs15_err, reset value: 0x0, access type: RW
		uint32 maskEhtAirTimeErr : 1; //mask_eht_air_time_err, reset value: 0x0, access type: RW
		uint32 maskEhtNdpErr : 1; //mask_eht_ndp_err, reset value: 0x0, access type: RW
		uint32 maskEhtSuNehtLtfErr : 1; //mask_eht_su_neht_ltf_err, reset value: 0x0, access type: RW
		uint32 maskEhtCrcErr : 1; //mask_eht_crc_err, reset value: 0x0, access type: RW
		uint32 maskEhtDupMode5G : 1; //mask_eht_dup_mode_5g, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxBeMaskEhtsigErrorCause_u;

/*REG_PHY_RX_BE_RX_REWIND_CONFIG 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 nsymRewindOverride : 6; //nsym_rewind_override, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rewindOverrideActive : 1; //rewind_override_active , reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
		uint32 tsymAccOverride : 14; //tsym_acc_override, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 rewindEnableOverride : 1; //rewind_enable_override, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxBeRxRewindConfig_u;

/*REG_PHY_RX_BE_HAMMING_DISTANCE_ENABLE 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hammingDistanceEnable : 1; //hamming_distance_enable , reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeHammingDistanceEnable_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesSteadyStateRewindOverride : 12; //ldpc_cycles_steady_state_rewind_override, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 ldpcCyclesSteadyStateRewindOverrideEn : 1; //ldpc_cycles_steady_state_rewind_override_en, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegPhyRxBeLdpcCyclesSteadyStateRewindOverride_u;

/*REG_PHY_RX_BE_PM_MAX_NSYM_REWIND 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMaxNsymRewindVhtCp04 : 6; //pm_max_nsym_rewind_vht_cp_04, reset value: 0x28, access type: RW
		uint32 pmMaxNsymRewindVhtCp08 : 6; //pm_max_nsym_rewind_vht_cp_08, reset value: 0x28, access type: RW
		uint32 pmMaxNsymRewindEhtCp08 : 6; //pm_max_nsym_rewind_eht_cp_08, reset value: 0xf, access type: RW
		uint32 pmMaxNsymRewindEhtCp16 : 6; //pm_max_nsym_rewind_eht_cp_16, reset value: 0xe, access type: RW
		uint32 pmMaxNsymRewindEhtCp32 : 6; //pm_max_nsym_rewind_eht_cp_32, reset value: 0xd, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxBePmMaxNsymRewind_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_2G_0 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit1Ss2G : 9; //pm_qr_limit_1ss_2g, reset value: 0x64, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit2Ss2G : 9; //pm_qr_limit_2ss_2g, reset value: 0x86, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit2G0_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_2G_1 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit3Ss2G : 9; //pm_qr_limit_3ss_2g, reset value: 0xc7, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit4Ss2G : 9; //pm_qr_limit_4ss_2g, reset value: 0xfe, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit2G1_u;

/*REG_PHY_RX_BE_PM_FD_OVERHEAD 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmFdOverhead : 10; //pm_fd_overhead, reset value: 0x1f4, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegPhyRxBePmFdOverhead_u;

/*REG_PHY_RX_BE_PM_TSYM_ACC_MAC_OVERRIDE 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTsymAccMacOverride : 14; //pm_tsym_acc_mac_override, reset value: 0x2049, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxBePmTsymAccMacOverride_u;

/*REG_PHY_RX_BE_PM_MIN_NSYM_REWIND 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMinNsymRewind : 6; //pm_min_nsym_rewind, reset value: 0x3, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBePmMinNsymRewind_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_0 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce500550 : 12; //pm_ldpc_cycles_reduce_500_550, reset value: 0x32, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce550600 : 12; //pm_ldpc_cycles_reduce_550_600, reset value: 0x3c, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce0_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_1 0x414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce600650 : 12; //pm_ldpc_cycles_reduce_600_650, reset value: 0x4b, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce650700 : 12; //pm_ldpc_cycles_reduce_650_700, reset value: 0x5a, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce1_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_2 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce700750 : 12; //pm_ldpc_cycles_reduce_700_750, reset value: 0x6e, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce750800 : 12; //pm_ldpc_cycles_reduce_750_800, reset value: 0x82, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce2_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_3 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce800900 : 12; //pm_ldpc_cycles_reduce_800_900, reset value: 0x96, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce9001000 : 12; //pm_ldpc_cycles_reduce_900_1000, reset value: 0xc8, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce3_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_4 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce10001100 : 12; //pm_ldpc_cycles_reduce_1000_1100, reset value: 0x10e, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce11001200 : 12; //pm_ldpc_cycles_reduce_1100_1200, reset value: 0x168, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce4_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_5 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce12001300 : 12; //pm_ldpc_cycles_reduce_1200_1300, reset value: 0x1cc, access type: RW
		uint32 reserved0 : 4;
		uint32 pmLdpcCyclesReduce13001400 : 12; //pm_ldpc_cycles_reduce_1300_1400, reset value: 0x230, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce5_u;

/*REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_6 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmLdpcCyclesReduce14001500 : 12; //pm_ldpc_cycles_reduce_1400_1500, reset value: 0x294, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBePmLdpcCyclesReduce6_u;

/*REG_PHY_RX_BE_REWIND_READ_ONLY_0 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsymPostRewindRo : 14; //tsym_post_rewind_ro, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 tsymPreRewindRo : 14; //tsym_pre_rewind_ro, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxBeRewindReadOnly0_u;

/*REG_PHY_RX_BE_REWIND_READ_ONLY_1 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rewindEnableRo : 1; //rewind_enable_ro, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
		uint32 nsymRewindRo : 6; //nsym_rewind_ro, reset value: 0x0, access type: RO
		uint32 reserved1 : 9;
		uint32 rewindCalcReadyRo : 1; //rewind_calc_ready_ro, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeRewindReadOnly1_u;

/*REG_PHY_RX_BE_PM_TSYM_ACC_FD_LOW_LIMIT 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTsymAccFdLowLimitHtVht : 14; //pm_tsym_acc_fd_low_limit_ht_vhr, reset value: 0x6a4, access type: RW
		uint32 reserved0 : 2;
		uint32 pmTsymAccFdLowLimitHeEht : 14; //pm_tsym_acc_fd_low_limit_he_eht, reset value: 0xf60, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxBePmTsymAccFdLowLimit_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_5G_0 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit1Ss5G : 9; //pm_qr_limit_1ss_5g, reset value: 0x44, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit2Ss5G : 9; //pm_qr_limit_2ss_5g, reset value: 0x44, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit5G0_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_5G_1 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit3Ss5G : 9; //pm_qr_limit_3ss_5g, reset value: 0x44, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit4Ss5G : 9; //pm_qr_limit_4ss_5g, reset value: 0x54, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit5G1_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_6G_0 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit1Ss6G : 9; //pm_qr_limit_1ss_6g, reset value: 0x44, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit2Ss6G : 9; //pm_qr_limit_2ss_6g, reset value: 0x44, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit6G0_u;

/*REG_PHY_RX_BE_PM_QR_LIMIT_6G_1 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmQrLimit3Ss6G : 9; //pm_qr_limit_3ss_6g, reset value: 0x44, access type: RW
		uint32 reserved0 : 7;
		uint32 pmQrLimit4Ss6G : 9; //pm_qr_limit_4ss_6g, reset value: 0x54, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxBePmQrLimit6G1_u;

/*REG_PHY_RX_BE_LDPC_MAX_CYCLES 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcMaxCyclesSu : 12; //ldpc_max_cycles_su , reset value: 0x5dc, access type: RW
		uint32 reserved0 : 4;
		uint32 ldpcMaxCyclesMu : 12; //ldpc_max_cycles_mu, reset value: 0x384, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegPhyRxBeLdpcMaxCycles_u;

/*REG_PHY_RX_BE_DFE_POST_REWIND_ENABLE 0x44C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfePostRewindEnable : 1; //dfe_post_rewind_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDfePostRewindEnable_u;

/*REG_PHY_RX_BE_LDPC_CYCLES_STEADY_REWIND_RO 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ldpcCyclesSteadyRewindRo : 12; //ldpc_cycles_steady_rewind_ro, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxBeLdpcCyclesSteadyRewindRo_u;

/*REG_PHY_RX_BE_PM_REWIND_ENABLE 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRewindEnable : 1; //pm_rewind_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmRewindEnable_u;

/*REG_PHY_RX_BE_DFE_ENABLE_POST_REWIND 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeEnablePostRewind : 1; //dfe_enable_post_rewind, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDfeEnablePostRewind_u;

/*REG_PHY_RX_BE_PM_REWIND_ENABLE_WHEN_DFE_OFF 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRewindEnableWhenDfeOff : 1; //pm_rewind_enable_when_dfe_off--- , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmRewindEnableWhenDfeOff_u;

/*REG_PHY_RX_BE_PM_TSYM_ACC_TX_LOW_LIMIT 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmTsymAccTxLowLimitHtVht : 14; //no description, reset value: 0x4B0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmTsymAccTxLowLimitHeEht : 14; //no description, reset value: 0x12C0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegPhyRxBePmTsymAccTxLowLimit_u;

/*REG_PHY_RX_BE_PM_REWIND_POWER_SAVE 0x464 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRewindPowerSave : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmRewindPowerSave_u;

/*REG_PHY_RX_BE_REWIND_CANCEL_RO 0x468 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rewindCancelRo : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeRewindCancelRo_u;

/*REG_PHY_RX_BE_NCW_TH_REWIND_CANCEL_RO 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ncwThRewindCancelRo : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBeNcwThRewindCancelRo_u;

/*REG_PHY_RX_BE_PM_FORCE_CONV_FAIL_ON_CW 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmForceConvFailOnCw : 16; //pm_force_conv_fail_on_cw , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxBePmForceConvFailOnCw_u;

/*REG_PHY_RX_BE_PM_SUPPORT_HE_MU_DL 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSupportHeMuDl : 1; //pm_support_he_mu_dl, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBePmSupportHeMuDl_u;

/*REG_PHY_RX_BE_CW_CONV_CNT_PRE_REWIND 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cwConvCntPreRewind : 16; //no description, reset value: 0x0, access type: RO
		uint32 cwNoConvCntPreRewind : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeCwConvCntPreRewind_u;

/*REG_PHY_RX_BE_CW_CONV_CNT_POST_REWIND 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cwConvCntPostRewind : 16; //no description, reset value: 0x0, access type: RO
		uint32 cwNoConvCntPostRewind : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxBeCwConvCntPostRewind_u;

/*REG_PHY_RX_BE_CHK_BIT_EXTRA_LDPC_CHECK 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chkBitExtraLdpcCheck : 1; //chk bit for LDPC core for extra parity/sign check enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeChkBitExtraLdpcCheck_u;

/*REG_PHY_RX_BE_HE_STATION_INFO 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stationNumUsers : 8; //no description, reset value: 0x0, access type: RO
		uint32 ourUserAddress : 7; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxBeHeStationInfo_u;

/*REG_PHY_RX_BE_DFE_LOW_RATES_EN 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeLowRatesEn : 1; //Enable DFE low rates calculation, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeDfeLowRatesEn_u;

/*REG_PHY_RX_BE_DFE_MAX_NSYM_WAIT_VHT 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeMaxNsymWaitVht : 16; //no description, reset value: 0x14, access type: RW
		uint32 dfeMaxNsymWaitEht : 16; //no description, reset value: 0x7, access type: RW
	} bitFields;
} RegPhyRxBeDfeMaxNsymWaitVht_u;

/*REG_PHY_RX_BE_DFE_REQ_CW_VEC 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeReqCwVec : 32; //[7:0] - dfe_req_cw for less than 1cw/sym , [15:8] - dfe_req_cw for less than 2cw/sym , [23:16] - dfe_req_cw for less than 3cw/sym , [31:24] - dfe_req_cw for more than 3cw/sym, reset value: 0x6050403, access type: RW
	} bitFields;
} RegPhyRxBeDfeReqCwVec_u;

/*REG_PHY_RX_BE_PM_MIN_NSYM_DFE_REWIND 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmMinNsymDfeRewind : 6; //Minimal allowed rewind+dfe symbols in pre-rewind stage, reset value: 0x2, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxBePmMinNsymDfeRewind_u;

/*REG_PHY_RX_BE_SENSING_MAC_INPUTS 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ndpType : 2; //ndp_type, reset value: 0x1, access type: RW
		uint32 sensingRepetition : 3; //sensing_repetition, reset value: 0x1, access type: RW
		uint32 txopNonTbSensing : 1; //txop_non_tb_sensing, reset value: 0x0, access type: RW
		uint32 enableSensingDebugPhyOnly : 1; //enable_sensing_debug_phy_only, reset value: 0x1, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegPhyRxBeSensingMacInputs_u;

/*REG_PHY_RX_BE_SENSING_MAC_VALID 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sensingMacValid : 1; //sensing_mac_valid, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxBeSensingMacValid_u;

//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG04 0x10 */
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_PHY_MAC_EVENT_LEN_MASK                                               0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_PHY_MAC_DATA_LEN_MASK                                                0x00000002
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_PHY_MAC_STATION_ID_LEN_MASK                                          0x00000004
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_LDPC_CW_STAT_LEN_MASK                                                0x00000008
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_LLR_STREAM_LEN_MASK                                                  0x00000010
#define B0_PHY_RX_BE_PHY_RXBE_REG04_FIELD_TP_DEBUG_CNT_EN_MASK                                                    0x80000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG05 0x14 */
#define B0_PHY_RX_BE_PHY_RXBE_REG05_FIELD_TP_LLR_STREAM_SEL_MASK                                                  0x00000003
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG07 0x1C */
#define B0_PHY_RX_BE_PHY_RXBE_REG07_FIELD_TP_DEBUG_CLK2STRB_MASK                                                  0x0000000F
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG08 0x20 */
#define B0_PHY_RX_BE_PHY_RXBE_REG08_FIELD_TP_LOW_MUX_CTRL_MASK                                                    0x00000003
#define B0_PHY_RX_BE_PHY_RXBE_REG08_FIELD_TP_MID_MUX_CTRL_MASK                                                    0x0000000C
#define B0_PHY_RX_BE_PHY_RXBE_REG08_FIELD_TP_HIGH_MUX_CTRL_MASK                                                   0x00000030
#define B0_PHY_RX_BE_PHY_RXBE_REG08_FIELD_TP_FOUR_MUX_CTRL_MASK                                                   0x000000C0
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG18 0x60 */
#define B0_PHY_RX_BE_PHY_RXBE_REG18_FIELD_VITERBI_TB_DEPTH_MASK                                                   0x0000003F
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG19 0x64 */
#define B0_PHY_RX_BE_PHY_RXBE_REG19_FIELD_REPORT_FULL_METRICS_MASK                                                0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG19_FIELD_WAIT_FOR_GRISC_METRICS_MASK                                             0x00000002
#define B0_PHY_RX_BE_PHY_RXBE_REG19_FIELD_WAIT_FOR_SNR_METRICS_MASK                                               0x00000010
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG1E 0x78 */
#define B0_PHY_RX_BE_PHY_RXBE_REG1E_FIELD_NUM_BITS_IN_PACKET_MASK                                                 0x01FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG20 0x80 */
#define B0_PHY_RX_BE_PHY_RXBE_REG20_FIELD_MAC_IF_MIN_CYCLE_MASK                                                   0x00FF0000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG21 0x84 */
#define B0_PHY_RX_BE_PHY_RXBE_REG21_FIELD_PACKET_LIMIT_MASK                                                       0x0007FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG22 0x88 */
#define B0_PHY_RX_BE_PHY_RXBE_REG22_FIELD_PRBS_LOAD_VAL_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG23 0x8C */
#define B0_PHY_RX_BE_PHY_RXBE_REG23_FIELD_PRBS_CRC_ENABLE_MASK                                                    0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG23_FIELD_PACKET_WITH_CRC_MASK                                                    0x00000002
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG24 0x90 */
#define B0_PHY_RX_BE_PHY_RXBE_REG24_FIELD_CLEAR_COUNTERS_MASK                                                     0x00000001
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG25 0x94 */
#define B0_PHY_RX_BE_PHY_RXBE_REG25_FIELD_LATCH_COUNTERS_MASK                                                     0x00000001
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG26 0x98 */
#define B0_PHY_RX_BE_PHY_RXBE_REG26_FIELD_PRBS_TYPE_MASK                                                          0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG28 0xA0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG28_FIELD_SIG_DECODE_CRC_LEN_MASK                                                 0x00000007
#define B0_PHY_RX_BE_PHY_RXBE_REG28_FIELD_SIG_DECODE_CRC_TYPE_MASK                                                0x00000FF0
#define B0_PHY_RX_BE_PHY_RXBE_REG28_FIELD_SIG_DECODE_CRC_CORRUPT_MASK                                             0x00001000
#define B0_PHY_RX_BE_PHY_RXBE_REG28_FIELD_SIG_DECODE_CRC_INIT_MASK                                                0x00FF0000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG34 0xD0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG34_FIELD_PRBS_ERR_CNT_LATCHED_MASK                                               0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG35 0xD4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG35_FIELD_PRBS_BIT_CNT_LATCHED_31_0_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG36 0xD8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG36_FIELD_PRBS_BIT_CNT_LATCHED_47_32_MASK                                         0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG37 0xDC */
#define B0_PHY_RX_BE_PHY_RXBE_REG37_FIELD_CRC_PACKET_CNT_LATCHED_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG38 0xE0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG38_FIELD_CRC_ERR_PACKET_CNT_LATCHED_MASK                                         0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG39 0xE4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG39_FIELD_PRBS_ERR_PACKET_CNT_LATCHED_MASK                                        0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG3F 0xFC */
#define B0_PHY_RX_BE_PHY_RXBE_REG3F_FIELD_SW_BIST_START_MASK                                                      0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG3F_FIELD_CLEAR_RAM_MODE_MASK                                                     0x00000002
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG41 0x104 */
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_PARITY_ERR_MASK                                               0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_RATE_ERR_MASK                                                 0x00000002
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_P_SIZE_ERR_MASK                                               0x00000004
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_RESERVED_ERR_MASK                                             0x00000008
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_CRC_ERR_MASK                                                  0x00000010
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_SIGB_CRC_ERR_MASK                                             0x00000020
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_SIG3_RESERVED_ERR_MASK                                        0x00000040
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_SIG3_LENGTH_ERR_MASK                                          0x00000080
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_NDP_ERR_MASK                                                  0x00000100
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_NDP_ERR_ALL_MASK                                              0x00000200
#define B0_PHY_RX_BE_PHY_RXBE_REG41_FIELD_RXBE_MASK_P_MAX_SIZE_ERR_MASK                                           0x00000400
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG42 0x108 */
#define B0_PHY_RX_BE_PHY_RXBE_REG42_FIELD_MINIMUM_PACKET_SIZE_MASK                                                0x00000FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4A 0x128 */
#define B0_PHY_RX_BE_PHY_RXBE_REG4A_FIELD_PRBS_ERR_CNT_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4B 0x12C */
#define B0_PHY_RX_BE_PHY_RXBE_REG4B_FIELD_PRBS_BIT_CNT_31_0_MASK                                                  0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4C 0x130 */
#define B0_PHY_RX_BE_PHY_RXBE_REG4C_FIELD_PRBS_BIT_CNT_47_32_MASK                                                 0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4D 0x134 */
#define B0_PHY_RX_BE_PHY_RXBE_REG4D_FIELD_CRC_PACKET_CNT_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4E 0x138 */
#define B0_PHY_RX_BE_PHY_RXBE_REG4E_FIELD_CRC_ERR_PACKET_CNT_MASK                                                 0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG4F 0x13C */
#define B0_PHY_RX_BE_PHY_RXBE_REG4F_FIELD_PRBS_ERR_PACKET_CNT_MASK                                                0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG50 0x140 */
#define B0_PHY_RX_BE_PHY_RXBE_REG50_FIELD_PHY_MAC_IF_BIT_COUNTER_MASK                                             0x007FFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG52 0x148 */
#define B0_PHY_RX_BE_PHY_RXBE_REG52_FIELD_HYP_RXBE_GPR_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG55 0x154 */
#define B0_PHY_RX_BE_PHY_RXBE_REG55_FIELD_DEINT_RAM_RM_MASK                                                       0x00000007
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG56 0x158 */
#define B0_PHY_RX_BE_PHY_RXBE_REG56_FIELD_RCR_RAM_RM_MASK                                                         0x00000007
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG57 0x15C */
#define B0_PHY_RX_BE_PHY_RXBE_REG57_FIELD_VITERBI_TB_BLOCK_RAM_RM_MASK                                            0x00000007
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG58 0x160 */
#define B0_PHY_RX_BE_PHY_RXBE_REG58_FIELD_USER_RAM_RM_MASK                                                        0x00000007
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG59 0x164 */
#define B0_PHY_RX_BE_PHY_RXBE_REG59_FIELD_MU_RAM_RM_MASK                                                          0x00000007
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG5A 0x168 */
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_R_RAM2_RM_MASK                                                          0x00000007
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_OBUF_RAM_RM_MASK                                                        0x00000070
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_SIGB_RAM_RM_MASK                                                        0x00000700
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_CW_PARAMS_96_RAM_RM_MASK                                                0x00007000
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_CW_PARAMS_48_RAM_RM_MASK                                                0x00070000
#define B0_PHY_RX_BE_PHY_RXBE_REG5A_FIELD_BURST_BUFFER_RM_MASK                                                    0x00380000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG5D 0x174 */
#define B0_PHY_RX_BE_PHY_RXBE_REG5D_FIELD_GCLK_SHUTDOWN_EN_MASK                                                   0x00003FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG5E 0x178 */
#define B0_PHY_RX_BE_PHY_RXBE_REG5E_FIELD_BIST_SCR_BYPASS_MASK                                                    0x00000001
#define B0_PHY_RX_BE_PHY_RXBE_REG5E_FIELD_RAM_TEST_MODE_MASK                                                      0x00000002
#define B0_PHY_RX_BE_PHY_RXBE_REG5E_FIELD_RAM_TEST_RNM_MASK                                                       0x00000004
#define B0_PHY_RX_BE_PHY_RXBE_REG5E_FIELD_MEM_GLOBAL_RM_MASK                                                      0x00000018
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG64 0x190 */
#define B0_PHY_RX_BE_PHY_RXBE_REG64_FIELD_GLOBAL_RDY_LIMIT_MASK                                                   0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG65 0x194 */
#define B0_PHY_RX_BE_PHY_RXBE_REG65_FIELD_ENABLE_AIR_TIME_BLOCK_MASK                                              0x00000001
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG67 0x19C */
#define B0_PHY_RX_BE_PHY_RXBE_REG67_FIELD_VHT_NON_AGGREGATE_MASK                                                  0x00000001
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG68 0x1A0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG68_FIELD_SPARE_GPR_0_MASK                                                        0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG69 0x1A4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG69_FIELD_SPARE_GPR_1_MASK                                                        0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6A 0x1A8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG6A_FIELD_SPARE_GPR_2_MASK                                                        0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6B 0x1AC */
#define B0_PHY_RX_BE_PHY_RXBE_REG6B_FIELD_SPARE_GPR_3_MASK                                                        0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6C 0x1B0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG6C_FIELD_TX_GLOBAL_RESET_MASK_MASK                                               0x00000FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6D 0x1B4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG6D_FIELD_RISC_AIR_TIME_RESULT_MASK                                               0x0001FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6E 0x1B8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG6E_FIELD_RISC_NUM_SYM_MASK                                                       0x00007FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG6F 0x1BC */
#define B0_PHY_RX_BE_PHY_RXBE_REG6F_FIELD_RISC_PHY_RATE_MASK                                                      0x00007FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG70 0x1C0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG70_FIELD_EVM_RESULT_SS0_MASK                                                     0x000000FF
#define B0_PHY_RX_BE_PHY_RXBE_REG70_FIELD_EVM_RESULT_SS1_MASK                                                     0x0000FF00
#define B0_PHY_RX_BE_PHY_RXBE_REG70_FIELD_EVM_RESULT_SS2_MASK                                                     0x00FF0000
#define B0_PHY_RX_BE_PHY_RXBE_REG70_FIELD_EVM_RESULT_SS3_MASK                                                     0xFF000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG71 0x1C4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG71_FIELD_VHT_SIG_A1_MASK                                                         0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG72 0x1C8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG72_FIELD_VHT_SIG_A2_MASK                                                         0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG73 0x1CC */
#define B0_PHY_RX_BE_PHY_RXBE_REG73_FIELD_VHT_SIG_B_MASK                                                          0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG74 0x1D0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG74_FIELD_HT_SIG2_MASK                                                            0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG75 0x1D4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG75_FIELD_HT_SIG3_MASK                                                            0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG76 0x1D8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG76_FIELD_LSIG_MASK                                                               0x00FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG77 0x1DC */
#define B0_PHY_RX_BE_PHY_RXBE_REG77_FIELD_BF_HEADER_DELAY_LIMIT_MASK                                              0x000000FF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG78 0x1E0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG78_FIELD_PM_COLOUR_0_MASK                                                        0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG78_FIELD_PM_COLOUR_1_MASK                                                        0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG78_FIELD_PM_COLOUR_2_MASK                                                        0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG78_FIELD_PM_COLOUR_3_MASK                                                        0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG79 0x1E4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG79_FIELD_PM_COLOUR_4_MASK                                                        0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG79_FIELD_PM_COLOUR_5_MASK                                                        0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG79_FIELD_PM_COLOUR_6_MASK                                                        0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG79_FIELD_PM_COLOUR_7_MASK                                                        0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG80 0x1E8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG80_FIELD_PM_COLOUR_8_MASK                                                        0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG80_FIELD_PM_COLOUR_9_MASK                                                        0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG80_FIELD_PM_COLOUR_10_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG80_FIELD_PM_COLOUR_11_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG81 0x1EC */
#define B0_PHY_RX_BE_PHY_RXBE_REG81_FIELD_PM_COLOUR_12_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG81_FIELD_PM_COLOUR_13_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG81_FIELD_PM_COLOUR_14_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG81_FIELD_PM_COLOUR_15_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG82 0x1F0 */
#define B0_PHY_RX_BE_PHY_RXBE_REG82_FIELD_PM_COLOUR_16_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG82_FIELD_PM_COLOUR_17_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG82_FIELD_PM_COLOUR_18_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG82_FIELD_PM_COLOUR_19_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG83 0x1F4 */
#define B0_PHY_RX_BE_PHY_RXBE_REG83_FIELD_PM_COLOUR_20_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG83_FIELD_PM_COLOUR_21_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG83_FIELD_PM_COLOUR_22_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG83_FIELD_PM_COLOUR_23_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG84 0x1F8 */
#define B0_PHY_RX_BE_PHY_RXBE_REG84_FIELD_PM_COLOUR_24_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG84_FIELD_PM_COLOUR_25_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG84_FIELD_PM_COLOUR_26_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG84_FIELD_PM_COLOUR_27_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG85 0x1FC */
#define B0_PHY_RX_BE_PHY_RXBE_REG85_FIELD_PM_COLOUR_28_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG85_FIELD_PM_COLOUR_29_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG85_FIELD_PM_COLOUR_30_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG85_FIELD_PM_COLOUR_31_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG86 0x200 */
#define B0_PHY_RX_BE_PHY_RXBE_REG86_FIELD_PM_COLOUR_32_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG86_FIELD_PM_COLOUR_33_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG86_FIELD_PM_COLOUR_34_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG86_FIELD_PM_COLOUR_35_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG87 0x204 */
#define B0_PHY_RX_BE_PHY_RXBE_REG87_FIELD_PM_COLOUR_36_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG87_FIELD_PM_COLOUR_37_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG87_FIELD_PM_COLOUR_38_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG87_FIELD_PM_COLOUR_39_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG88 0x208 */
#define B0_PHY_RX_BE_PHY_RXBE_REG88_FIELD_PM_COLOUR_40_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG88_FIELD_PM_COLOUR_41_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG88_FIELD_PM_COLOUR_42_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG88_FIELD_PM_COLOUR_43_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG89 0x20C */
#define B0_PHY_RX_BE_PHY_RXBE_REG89_FIELD_PM_COLOUR_44_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG89_FIELD_PM_COLOUR_45_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG89_FIELD_PM_COLOUR_46_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG89_FIELD_PM_COLOUR_47_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG90 0x210 */
#define B0_PHY_RX_BE_PHY_RXBE_REG90_FIELD_PM_COLOUR_48_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG90_FIELD_PM_COLOUR_49_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG90_FIELD_PM_COLOUR_50_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG90_FIELD_PM_COLOUR_51_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG91 0x214 */
#define B0_PHY_RX_BE_PHY_RXBE_REG91_FIELD_PM_COLOUR_52_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG91_FIELD_PM_COLOUR_53_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG91_FIELD_PM_COLOUR_54_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG91_FIELD_PM_COLOUR_55_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG92 0x218 */
#define B0_PHY_RX_BE_PHY_RXBE_REG92_FIELD_PM_COLOUR_56_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG92_FIELD_PM_COLOUR_57_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG92_FIELD_PM_COLOUR_58_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG92_FIELD_PM_COLOUR_59_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG93 0x21C */
#define B0_PHY_RX_BE_PHY_RXBE_REG93_FIELD_PM_COLOUR_60_MASK                                                       0x0000003F
#define B0_PHY_RX_BE_PHY_RXBE_REG93_FIELD_PM_COLOUR_61_MASK                                                       0x00003F00
#define B0_PHY_RX_BE_PHY_RXBE_REG93_FIELD_PM_COLOUR_62_MASK                                                       0x003F0000
#define B0_PHY_RX_BE_PHY_RXBE_REG93_FIELD_PM_COLOUR_63_MASK                                                       0x3F000000
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG94 0x220 */
#define B0_PHY_RX_BE_PHY_RXBE_REG94_FIELD_PM_AIR_TIME_INTERVAL_MASK                                               0x000003FF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG95 0x224 */
#define B0_PHY_RX_BE_PHY_RXBE_REG95_FIELD_PM_CURRENT_USER_MASK                                                    0x0000003F
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG96 0x228 */
#define B0_PHY_RX_BE_PHY_RXBE_REG96_FIELD_LDPC_TOTAL_LAST_SYM_MASK                                                0x000000FF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG97 0x22C */
#define B0_PHY_RX_BE_PHY_RXBE_REG97_FIELD_LDPC_TOTAL_CW_MASK                                                      0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG98 0x230 */
#define B0_PHY_RX_BE_PHY_RXBE_REG98_FIELD_HE_SIG_A1_MASK                                                          0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG99 0x234 */
#define B0_PHY_RX_BE_PHY_RXBE_REG99_FIELD_HE_SIG_A2_MASK                                                          0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG9A 0x238 */
#define B0_PHY_RX_BE_PHY_RXBE_REG9A_FIELD_PSDU_LEN_MASK                                                           0x007FFFFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG9B 0x23C */
#define B0_PHY_RX_BE_PHY_RXBE_REG9B_FIELD_USER_CONFIG_MASK                                                        0x00000FFF
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REG9C 0x240 */
#define B0_PHY_RX_BE_PHY_RXBE_REG9C_FIELD_RXBE_SCH_PS_MASK                                                        0x0000001F
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REGA1 0x254 */
#define B0_PHY_RX_BE_PHY_RXBE_REGA1_FIELD_LDPC_ALPHA_INDEX_MASK                                                   0x00000003
//========================================
/* REG_PHY_RX_BE_PHY_RXBE_REGA2 0x258 */
#define B0_PHY_RX_BE_PHY_RXBE_REGA2_FIELD_LDPC_QUANTIZE_CYCLES_MASK                                               0x00000001
//========================================
/* REG_PHY_RX_BE_LDPC_CALC_VALID 0x25C */
#define B0_PHY_RX_BE_LDPC_CALC_VALID_FIELD_LDPC_CALC_VALID_MASK                                                   0x00000001
//========================================
/* REG_PHY_RX_BE_LDPC_VITERBI_USER_ACTIVE 0x260 */
#define B0_PHY_RX_BE_LDPC_VITERBI_USER_ACTIVE_FIELD_LDPC_VITERBI_USER_ACTIVE_MASK                                 0x0000003F
//========================================
/* REG_PHY_RX_BE_LDPC_USER_ACTIVE 0x264 */
#define B0_PHY_RX_BE_LDPC_USER_ACTIVE_FIELD_LDPC_USER_ACTIVE_MASK                                                 0x0000003F
//========================================
/* REG_PHY_RX_BE_LDPC_MAX_N_CW 0x268 */
#define B0_PHY_RX_BE_LDPC_MAX_N_CW_FIELD_LDPC_MAX_N_CW_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_PER_CW 0x26C */
#define B0_PHY_RX_BE_LDPC_CYCLES_PER_CW_FIELD_LDPC_CYCLES_PER_CW_MASK                                             0x00000FFF
//========================================
/* REG_PHY_RX_BE_LDPC_MAX_CHECK_CONV 0x270 */
#define B0_PHY_RX_BE_LDPC_MAX_CHECK_CONV_FIELD_LDPC_MAX_CHECKS_RO_MASK                                            0x0000FFFF
#define B0_PHY_RX_BE_LDPC_MAX_CHECK_CONV_FIELD_LDPC_NO_CONVERGENCE_MASK                                           0x00010000
//========================================
/* REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_0 0x274 */
#define B0_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_0_FIELD_BUS_PAC_PHY_RX_CRC_CHECK_0_MASK                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_1 0x278 */
#define B0_PHY_RX_BE_BUS_PAC_PHY_RX_CRC_CHECK_1_FIELD_BUS_PAC_PHY_RX_CRC_CHECK_1_MASK                             0x0000000F
//========================================
/* REG_PHY_RX_BE_RCR_ACTIVE_USERS_0 0x27C */
#define B0_PHY_RX_BE_RCR_ACTIVE_USERS_0_FIELD_RCR_ACTIVE_USERS_0_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_RCR_ACTIVE_USERS_1 0x280 */
#define B0_PHY_RX_BE_RCR_ACTIVE_USERS_1_FIELD_RCR_ACTIVE_USERS_1_MASK                                             0x0000000F
//========================================
/* REG_PHY_RX_BE_SIGB_STID_0 0x284 */
#define B0_PHY_RX_BE_SIGB_STID_0_FIELD_SIGB_STID_0_MASK                                                           0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_0_FIELD_SIGB_STID_0_ENABLE_MASK                                                    0x00000800
#define B0_PHY_RX_BE_SIGB_STID_0_FIELD_SIGB_STID_1_MASK                                                           0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_0_FIELD_SIGB_STID_1_ENABLE_MASK                                                    0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_1 0x288 */
#define B0_PHY_RX_BE_SIGB_STID_1_FIELD_SIGB_STID_2_MASK                                                           0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_1_FIELD_SIGB_STID_2_ENABLE_MASK                                                    0x00000800
#define B0_PHY_RX_BE_SIGB_STID_1_FIELD_SIGB_STID_3_MASK                                                           0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_1_FIELD_SIGB_STID_3_ENABLE_MASK                                                    0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_2 0x28C */
#define B0_PHY_RX_BE_SIGB_STID_2_FIELD_SIGB_STID_4_MASK                                                           0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_2_FIELD_SIGB_STID_4_ENABLE_MASK                                                    0x00000800
#define B0_PHY_RX_BE_SIGB_STID_2_FIELD_SIGB_STID_5_MASK                                                           0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_2_FIELD_SIGB_STID_5_ENABLE_MASK                                                    0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_3 0x290 */
#define B0_PHY_RX_BE_SIGB_STID_3_FIELD_SIGB_STID_6_MASK                                                           0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_3_FIELD_SIGB_STID_6_ENABLE_MASK                                                    0x00000800
#define B0_PHY_RX_BE_SIGB_STID_3_FIELD_SIGB_STID_7_MASK                                                           0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_3_FIELD_SIGB_STID_7_ENABLE_MASK                                                    0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_4 0x294 */
#define B0_PHY_RX_BE_SIGB_STID_4_FIELD_SIGB_STID_8_MASK                                                           0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_4_FIELD_SIGB_STID_8_ENABLE_MASK                                                    0x00000800
#define B0_PHY_RX_BE_SIGB_STID_4_FIELD_SIGB_STID_9_MASK                                                           0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_4_FIELD_SIGB_STID_9_ENABLE_MASK                                                    0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_5 0x298 */
#define B0_PHY_RX_BE_SIGB_STID_5_FIELD_SIGB_STID_10_MASK                                                          0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_5_FIELD_SIGB_STID_10_ENABLE_MASK                                                   0x00000800
#define B0_PHY_RX_BE_SIGB_STID_5_FIELD_SIGB_STID_11_MASK                                                          0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_5_FIELD_SIGB_STID_11_ENABLE_MASK                                                   0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_6 0x29C */
#define B0_PHY_RX_BE_SIGB_STID_6_FIELD_SIGB_STID_12_MASK                                                          0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_6_FIELD_SIGB_STID_12_ENABLE_MASK                                                   0x00000800
#define B0_PHY_RX_BE_SIGB_STID_6_FIELD_SIGB_STID_13_MASK                                                          0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_6_FIELD_SIGB_STID_13_ENABLE_MASK                                                   0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_7 0x2A0 */
#define B0_PHY_RX_BE_SIGB_STID_7_FIELD_SIGB_STID_14_MASK                                                          0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_7_FIELD_SIGB_STID_14_ENABLE_MASK                                                   0x00000800
#define B0_PHY_RX_BE_SIGB_STID_7_FIELD_SIGB_STID_15_MASK                                                          0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_7_FIELD_SIGB_STID_15_ENABLE_MASK                                                   0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_8 0x2A4 */
#define B0_PHY_RX_BE_SIGB_STID_8_FIELD_SIGB_STID_16_MASK                                                          0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_8_FIELD_SIGB_STID_16_ENABLE_MASK                                                   0x00000800
#define B0_PHY_RX_BE_SIGB_STID_8_FIELD_SIGB_STID_17_MASK                                                          0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_8_FIELD_SIGB_STID_17_ENABLE_MASK                                                   0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_9 0x2A8 */
#define B0_PHY_RX_BE_SIGB_STID_9_FIELD_SIGB_STID_18_MASK                                                          0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_9_FIELD_SIGB_STID_18_ENABLE_MASK                                                   0x00000800
#define B0_PHY_RX_BE_SIGB_STID_9_FIELD_SIGB_STID_19_MASK                                                          0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_9_FIELD_SIGB_STID_19_ENABLE_MASK                                                   0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_10 0x2AC */
#define B0_PHY_RX_BE_SIGB_STID_10_FIELD_SIGB_STID_20_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_10_FIELD_SIGB_STID_20_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_10_FIELD_SIGB_STID_21_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_10_FIELD_SIGB_STID_21_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_11 0x2B0 */
#define B0_PHY_RX_BE_SIGB_STID_11_FIELD_SIGB_STID_22_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_11_FIELD_SIGB_STID_22_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_11_FIELD_SIGB_STID_23_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_11_FIELD_SIGB_STID_23_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_12 0x2B4 */
#define B0_PHY_RX_BE_SIGB_STID_12_FIELD_SIGB_STID_24_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_12_FIELD_SIGB_STID_24_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_12_FIELD_SIGB_STID_25_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_12_FIELD_SIGB_STID_25_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_13 0x2B8 */
#define B0_PHY_RX_BE_SIGB_STID_13_FIELD_SIGB_STID_26_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_13_FIELD_SIGB_STID_26_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_13_FIELD_SIGB_STID_27_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_13_FIELD_SIGB_STID_27_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_14 0x2BC */
#define B0_PHY_RX_BE_SIGB_STID_14_FIELD_SIGB_STID_28_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_14_FIELD_SIGB_STID_28_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_14_FIELD_SIGB_STID_29_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_14_FIELD_SIGB_STID_29_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_SIGB_STID_15 0x2D0 */
#define B0_PHY_RX_BE_SIGB_STID_15_FIELD_SIGB_STID_30_MASK                                                         0x000007FF
#define B0_PHY_RX_BE_SIGB_STID_15_FIELD_SIGB_STID_30_ENABLE_MASK                                                  0x00000800
#define B0_PHY_RX_BE_SIGB_STID_15_FIELD_SIGB_STID_31_MASK                                                         0x07FF0000
#define B0_PHY_RX_BE_SIGB_STID_15_FIELD_SIGB_STID_31_ENABLE_MASK                                                  0x08000000
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_LAST_SYMBOL 0x2D4 */
#define B0_PHY_RX_BE_LDPC_CYCLES_LAST_SYMBOL_FIELD_LDPC_CYCLES_LAST_SYMBOL_MASK                                   0x00000FFF
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE 0x2D8 */
#define B0_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_FIELD_LDPC_CYCLES_STEADY_STATE_MASK                                 0x00000FFF
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_FIRST_CW 0x2DC */
#define B0_PHY_RX_BE_LDPC_CYCLES_FIRST_CW_FIELD_LDPC_CYCLES_FIRST_CW_MASK                                         0x00000FFF
//========================================
/* REG_PHY_RX_BE_LDPC_LAST_SYMBOL_CW_INDEX 0x2E0 */
#define B0_PHY_RX_BE_LDPC_LAST_SYMBOL_CW_INDEX_FIELD_LDPC_LAST_SYMBOL_CW_INDEX_MASK                               0x0000FFFF
//========================================
/* REG_PHY_RX_BE_LDPC_CORE_GCLK_CHICKEN_BIT 0x2E4 */
#define B0_PHY_RX_BE_LDPC_CORE_GCLK_CHICKEN_BIT_FIELD_LDPC_CORE_GCLK_CHICKEN_BIT_MASK                             0x00000001
//========================================
/* REG_PHY_RX_BE_RXBE_FSM_FORCE 0x2F0 */
#define B0_PHY_RX_BE_RXBE_FSM_FORCE_FIELD_PM_FORCE_LSIG_ERR_MASK                                                  0x00000001
#define B0_PHY_RX_BE_RXBE_FSM_FORCE_FIELD_PM_FORCE_LPLUSR_ER_PASS_MASK                                            0x00000002
#define B0_PHY_RX_BE_RXBE_FSM_FORCE_FIELD_PM_FORCE_LSIG_NOT_EQUAL_RLSIG_MASK                                      0x00000004
#define B0_PHY_RX_BE_RXBE_FSM_FORCE_FIELD_PM_FORCE_LSIG_MCS_EVM_FAIL_COND_MASK                                    0x00000008
//========================================
/* REG_PHY_RX_BE_PM_LEG_MCS_TH 0x2F8 */
#define B0_PHY_RX_BE_PM_LEG_MCS_TH_FIELD_PM_LEG_MCS_TH_MASK                                                       0x0000000F
//========================================
/* REG_PHY_RX_BE_PM_LEG_EVM_TH 0x2FC */
#define B0_PHY_RX_BE_PM_LEG_EVM_TH_FIELD_PM_LEG_EVM_TH_MASK                                                       0x000000FF
//========================================
/* REG_PHY_RX_BE_LEG_BELOW_EVM_TH_ERR 0x300 */
#define B0_PHY_RX_BE_LEG_BELOW_EVM_TH_ERR_FIELD_LEG_BELOW_EVM_TH_ERR_MASK                                         0x00000001
//========================================
/* REG_PHY_RX_BE_PM_MASK_LPLUSR 0x304 */
#define B0_PHY_RX_BE_PM_MASK_LPLUSR_FIELD_PM_MASK_LPLUSR_PARITY_ERR_MASK                                          0x00000001
#define B0_PHY_RX_BE_PM_MASK_LPLUSR_FIELD_PM_MASK_LPLUSR_RATE_ERR_MASK                                            0x00000002
#define B0_PHY_RX_BE_PM_MASK_LPLUSR_FIELD_PM_MASK_LPLUSR_P_SIZE_ERR_MASK                                          0x00000004
#define B0_PHY_RX_BE_PM_MASK_LPLUSR_FIELD_PM_MASK_LPLUSR_RESERVE_ERR_MASK                                         0x00000008
//========================================
/* REG_PHY_RX_BE_PM_R_PLUS_L_MINIMUM_PACKET_SIZE 0x308 */
#define B0_PHY_RX_BE_PM_R_PLUS_L_MINIMUM_PACKET_SIZE_FIELD_PM_R_PLUS_L_MINIMUM_PACKET_SIZE_MASK                   0x00000FFF
//========================================
/* REG_PHY_RX_BE_DESCRAMBLER_SEED 0x30C */
#define B0_PHY_RX_BE_DESCRAMBLER_SEED_FIELD_DESCRAMBLER_SEED_MASK                                                 0x000007FF
//========================================
/* REG_PHY_RX_BE_PM_SD_REGISTER 0x310 */
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_SIGB_RAM_MASK                                                     0x00000001
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_VITERBI_BANK_RAM_MASK                                             0x00000002
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_DEINT_RAM_MASK                                                    0x00000004
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_RCR_RAM_MASK                                                      0x00000008
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_CW_PARAMS_FIFO_MASK                                               0x00000010
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_OBUF_CORE_MASK                                                    0x00000020
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_VITERBI_TB_RAM_MASK                                               0x00000040
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_RRAM2_MASK                                                        0x00000080
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_BURST_BUFFER0_MASK                                                0x00000100
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_BURST_BUFFER1_MASK                                                0x00000200
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_BURST_BUFFER0_MRU_MASK                                            0x00000400
#define B0_PHY_RX_BE_PM_SD_REGISTER_FIELD_PM_SD_BURST_BUFFER1_MRU_MASK                                            0x00000800
//========================================
/* REG_PHY_RX_BE_PM_DS_REGISTER 0x314 */
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_SIGB_RAM_MASK                                                     0x00000001
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_VITERBI_BANK_RAM_MASK                                             0x00000002
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_DEINT_RAM_MASK                                                    0x00000004
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_RCR_RAM_MASK                                                      0x00000008
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_CW_PARAMS_FIFO_MASK                                               0x00000010
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_OBUF_CORE_MASK                                                    0x00000020
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_VITERBI_TB_RAM_MASK                                               0x00000040
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_RRAM2_MASK                                                        0x00000080
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_BURST_BUFFER0_MASK                                                0x00000100
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_BURST_BUFFER1_MASK                                                0x00000200
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_BURST_BUFFER0_MRU_MASK                                            0x00000400
#define B0_PHY_RX_BE_PM_DS_REGISTER_FIELD_PM_DS_BURST_BUFFER1_MRU_MASK                                            0x00000800
//========================================
/* REG_PHY_RX_BE_SIGB_ERROR_CODE 0x318 */
#define B0_PHY_RX_BE_SIGB_ERROR_CODE_FIELD_SIGB_ERROR_CODE_MASK                                                   0x0000000F
//========================================
/* REG_PHY_RX_BE_HE_MU_COMPRESSED_MODE_INDICATION 0x31C */
#define B0_PHY_RX_BE_HE_MU_COMPRESSED_MODE_INDICATION_FIELD_HE_MU_COMPRESSED_MODE_INDICATION_MASK                 0x00000001
//========================================
/* REG_PHY_RX_BE_HW_MCS_UNSUPPORTED_CAUSE 0x320 */
#define B0_PHY_RX_BE_HW_MCS_UNSUPPORTED_CAUSE_FIELD_HW_MCS_UNSUPPORTED_CAUSE_MASK                                 0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS 0x324 */
#define B0_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS_FIELD_RXBE_MASK_UNSUPPORTED_MCS_MASK                               0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_ROP_REGISTER 0x328 */
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_DEINT_RAM_MASK                                                        0x00000001
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_VITERBI_BANK_RAM_MASK                                                 0x00000002
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_VITERBI_TB_RAM_MASK                                                   0x00000004
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_RCR_RAM_MASK                                                          0x00000008
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_SIGB_RAM_MASK                                                         0x00000010
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_CW_PARAMS_FIFO_MASK                                                   0x00000020
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_RRAM2_MASK                                                            0x00000040
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_OBUF_MASK                                                             0x00000080
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_BURST_BUFFER0_MASK                                                    0x00000100
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_BURST_BUFFER1_MASK                                                    0x00000200
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_BURST_BUFFER0_MRU_MASK                                                0x00000400
#define B0_PHY_RX_BE_ROP_REGISTER_FIELD_ROP_BURST_BUFFER1_MRU_MASK                                                0x00000800
//========================================
/* REG_PHY_RX_BE_EXT_RANGE_READ_ONLY 0x32C */
#define B0_PHY_RX_BE_EXT_RANGE_READ_ONLY_FIELD_HE_EXT_BE_CRITERIA_MASK                                            0x00000001
#define B0_PHY_RX_BE_EXT_RANGE_READ_ONLY_FIELD_HE_EXT_BE_CRITERIA_VALID_MASK                                      0x00000002
#define B0_PHY_RX_BE_EXT_RANGE_READ_ONLY_FIELD_WEAK_ER_CRITERIA_MASK                                              0x00000004
//========================================
/* REG_PHY_RX_BE_DISABLE_NEW_ER_MODE 0x330 */
#define B0_PHY_RX_BE_DISABLE_NEW_ER_MODE_FIELD_DISABLE_NEW_ER_MODE_MASK                                           0x00000001
//========================================
/* REG_PHY_RX_BE_PM_MAC_DATA_0 0x334 */
#define B0_PHY_RX_BE_PM_MAC_DATA_0_FIELD_PM_MAC_DATA_0_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PM_MAC_DATA_1 0x338 */
#define B0_PHY_RX_BE_PM_MAC_DATA_1_FIELD_PM_MAC_DATA_1_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PM_MAC_DATA_2 0x33C */
#define B0_PHY_RX_BE_PM_MAC_DATA_2_FIELD_PM_MAC_DATA_2_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PM_MAC_DATA_3 0x340 */
#define B0_PHY_RX_BE_PM_MAC_DATA_3_FIELD_PM_MAC_DATA_3_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PM_MAC_DATA_READY 0x344 */
#define B0_PHY_RX_BE_PM_MAC_DATA_READY_FIELD_PM_MAC_DATA_READY_MASK                                               0x00000001
//========================================
/* REG_PHY_RX_BE_DC_CNT_RRAM_EN_CHK_BIT 0x348 */
#define B0_PHY_RX_BE_DC_CNT_RRAM_EN_CHK_BIT_FIELD_DC_CNT_RRAM_EN_CHK_BIT_MASK                                     0x00000001
//========================================
/* REG_PHY_RX_BE_PM_MIN_BURST_LENGTH 0x34C */
#define B0_PHY_RX_BE_PM_MIN_BURST_LENGTH_FIELD_PM_MIN_BURST_LENGTH_MASK                                           0x0000001F
//========================================
/* REG_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL 0x350 */
#define B0_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL_FIELD_PM_LAST_SYMBOL_LDPC_HE_OFFSET_MASK                            0x0000FFFF
#define B0_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL_FIELD_PM_LAST_SYMBOL_LDPC_NON_HE_OFFSET_MASK                        0x0FFF0000
#define B0_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL_FIELD_PM_LAST_SYMBOL_LDPC_EN_MASK                                   0x10000000
#define B0_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL_FIELD_PM_LAST_SYMBOL_LDPC_SW_EN_MASK                                0x20000000
#define B0_PHY_RX_BE_PM_LAST_SYMBOL_LDPC_CTRL_FIELD_PM_FIX_LDPC_LAST_SYM_CALC_MASK                                0x40000000
//========================================
/* REG_PHY_RX_BE_PM_FORCE_RU_SIZE_7 0x354 */
#define B0_PHY_RX_BE_PM_FORCE_RU_SIZE_7_FIELD_PM_FORCE_RU_SIZE_7_MASK                                             0x00000001
//========================================
/* REG_PHY_RX_BE_PM_EHT_SCRAMBLER_ENABLE 0x358 */
#define B0_PHY_RX_BE_PM_EHT_SCRAMBLER_ENABLE_FIELD_PM_EHT_SCRAMBLER_ENABLE_MASK                                   0x00000001
//========================================
/* REG_PHY_RX_BE_LDPC_CONVERGENCE_PER_USER 0x35C */
#define B0_PHY_RX_BE_LDPC_CONVERGENCE_PER_USER_FIELD_LDPC_CONVERGENCE_PER_USER_MASK                               0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_0 0x360 */
#define B0_PHY_RX_BE_LDPC_CYCLES_LIMIT_0_FIELD_LDPC_CYCLES_LIMIT_RATE_1_2_MASK                                    0x0000FFFF
#define B0_PHY_RX_BE_LDPC_CYCLES_LIMIT_0_FIELD_LDPC_CYCLES_LIMIT_RATE_2_3_MASK                                    0xFFFF0000
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_LIMIT_1 0x364 */
#define B0_PHY_RX_BE_LDPC_CYCLES_LIMIT_1_FIELD_LDPC_CYCLES_LIMIT_RATE_3_4_MASK                                    0x0000FFFF
#define B0_PHY_RX_BE_LDPC_CYCLES_LIMIT_1_FIELD_LDPC_CYCLES_LIMIT_RATE_5_6_MASK                                    0xFFFF0000
//========================================
/* REG_PHY_RX_BE_NUM_OF_CORES 0x368 */
#define B0_PHY_RX_BE_NUM_OF_CORES_FIELD_NUM_OF_CORES_MASK                                                         0x00000007
//========================================
/* REG_PHY_RX_BE_USIG_OVERFLOW 0x36C */
#define B0_PHY_RX_BE_USIG_OVERFLOW_FIELD_USIG_OVERFLOW_MASK                                                       0x000FFFFF
//========================================
/* REG_PHY_RX_BE_EHT_INFO 0x370 */
#define B0_PHY_RX_BE_EHT_INFO_FIELD_EHT_INFO_MASK                                                                 0x003FFFFF
//========================================
/* REG_PHY_RX_BE_SPARE_FROM_FD0 0x374 */
#define B0_PHY_RX_BE_SPARE_FROM_FD0_FIELD_SPARE_FROM_FD0_MASK                                                     0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_FROM_FD1 0x378 */
#define B0_PHY_RX_BE_SPARE_FROM_FD1_FIELD_SPARE_FROM_FD1_MASK                                                     0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_FROM_TX 0x37C */
#define B0_PHY_RX_BE_SPARE_FROM_TX_FIELD_SPARE_FROM_TX_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_FROM_TD 0x380 */
#define B0_PHY_RX_BE_SPARE_FROM_TD_FIELD_SPARE_FROM_TD_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_TO_FD0 0x384 */
#define B0_PHY_RX_BE_SPARE_TO_FD0_FIELD_SPARE_TO_FD0_MASK                                                         0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_TO_FD1 0x388 */
#define B0_PHY_RX_BE_SPARE_TO_FD1_FIELD_SPARE_TO_FD1_MASK                                                         0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_TO_TX 0x38C */
#define B0_PHY_RX_BE_SPARE_TO_TX_FIELD_SPARE_TO_TX_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_BE_SPARE_TO_TD 0x390 */
#define B0_PHY_RX_BE_SPARE_TO_TD_FIELD_SPARE_TO_TD_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_SS_OV 0x394 */
#define B0_PHY_RX_BE_LDPC_CYCLES_SS_OV_FIELD_LDPC_CYCLES_STEADY_STATE_OVERRIDE_MASK                               0x00000FFF
#define B0_PHY_RX_BE_LDPC_CYCLES_SS_OV_FIELD_LDPC_CYCLES_STEADY_STATE_OVERRIDE_EN_MASK                            0x00001000
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLE_LAST_SYM_OV 0x398 */
#define B0_PHY_RX_BE_LDPC_CYCLE_LAST_SYM_OV_FIELD_LDPC_CYCLES_LAST_SYM_OVERRIDE_MASK                              0x00000FFF
#define B0_PHY_RX_BE_LDPC_CYCLE_LAST_SYM_OV_FIELD_LDPC_CYCLES_LAST_SYM_OVERRIDE_EN_MASK                           0x00001000
//========================================
/* REG_PHY_RX_BE_LDPC_LAST_SYM_CW_IDX 0x39C */
#define B0_PHY_RX_BE_LDPC_LAST_SYM_CW_IDX_FIELD_LDPC_LAST_SYMBOL_CW_IDX_OVERRIDE_MASK                             0x0000FFFF
#define B0_PHY_RX_BE_LDPC_LAST_SYM_CW_IDX_FIELD_LDPC_LAST_SYMBOL_CW_IDX_OVERRIDE_EN_MASK                          0x00010000
//========================================
/* REG_PHY_RX_BE_USIG_0 0x3A0 */
#define B0_PHY_RX_BE_USIG_0_FIELD_USIG_0_MASK                                                                     0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_USIG_1 0x3A4 */
#define B0_PHY_RX_BE_USIG_1_FIELD_USIG_1_MASK                                                                     0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_EHT_0 0x3A8 */
#define B0_PHY_RX_BE_EHT_0_FIELD_EHT_0_MASK                                                                       0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_EHT_1 0x3AC */
#define B0_PHY_RX_BE_EHT_1_FIELD_EHT_1_MASK                                                                       0x03FFFFFF
//========================================
/* REG_PHY_RX_BE_IS_320_UPPER_CHANNEL 0x3B0 */
#define B0_PHY_RX_BE_IS_320_UPPER_CHANNEL_FIELD_IS_320_UPPER_CHANNEL_MASK                                         0x00000001
//========================================
/* REG_PHY_RX_BE_TEST_BUS_GCLK_EN 0x3B4 */
#define B0_PHY_RX_BE_TEST_BUS_GCLK_EN_FIELD_TEST_BUS_GCLK_EN_MASK                                                 0x00000001
//========================================
/* REG_PHY_RX_BE_BYTE_ERROR_CONTOL 0x3B8 */
#define B0_PHY_RX_BE_BYTE_ERROR_CONTOL_FIELD_BYTE_ERROR_USER_MASK                                                 0x000000FF
#define B0_PHY_RX_BE_BYTE_ERROR_CONTOL_FIELD_BYTE_ERROR_LOAD_MASK                                                 0x00000100
#define B0_PHY_RX_BE_BYTE_ERROR_CONTOL_FIELD_BYTE_ERROR_CLEAR_MASK                                                0x00000200
//========================================
/* REG_PHY_RX_BE_BYTE_ERROR_RATE 0x3BC */
#define B0_PHY_RX_BE_BYTE_ERROR_RATE_FIELD_BYTE_ERROR_RATE_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_BYTE_ERROR_RATE_RD 0x3C0 */
#define B0_PHY_RX_BE_BYTE_ERROR_RATE_RD_FIELD_BYTE_ERROR_RATE_RD_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_DFE_CONTROL 0x3C4 */
#define B0_PHY_RX_BE_DFE_CONTROL_FIELD_DFE_ENABLE_MASK                                                            0x00000001
#define B0_PHY_RX_BE_DFE_CONTROL_FIELD_DFE_MIN_NUM_SYMBOLS_MASK                                                   0x000000F0
#define B0_PHY_RX_BE_DFE_CONTROL_FIELD_DFE_MIN_CW_PER_SYMBOL_MASK                                                 0x00000700
#define B0_PHY_RX_BE_DFE_CONTROL_FIELD_DFE_VHT_MIN_NUM_SYMBOLS_MASK                                               0x0000F000
#define B0_PHY_RX_BE_DFE_CONTROL_FIELD_DFE_VHT_MIN_CW_PER_SYMBOL_MASK                                             0x00070000
//========================================
/* REG_PHY_RX_BE_DFE_VALID 0x3C8 */
#define B0_PHY_RX_BE_DFE_VALID_FIELD_DFE_VALID_MASK                                                               0x00000001
//========================================
/* REG_PHY_RX_BE_EVM_PPRU_RAM_READY 0x3CC */
#define B0_PHY_RX_BE_EVM_PPRU_RAM_READY_FIELD_EVM_PPRU_RAM_READY_MASK                                             0x00000001
//========================================
/* REG_PHY_RX_BE_EHT_SIG_INFO_VALID 0x3D0 */
#define B0_PHY_RX_BE_EHT_SIG_INFO_VALID_FIELD_EHT_SIG_INFO_VALID_MASK                                             0x00000001
//========================================
/* REG_PHY_RX_BE_LSIG_ERROR_CAUSE 0x3D4 */
#define B0_PHY_RX_BE_LSIG_ERROR_CAUSE_FIELD_SIG1_PARITY_ERR_MASK                                                  0x00000001
#define B0_PHY_RX_BE_LSIG_ERROR_CAUSE_FIELD_SIG1_RATE_ERR_MASK                                                    0x00000002
#define B0_PHY_RX_BE_LSIG_ERROR_CAUSE_FIELD_SIG1_LENGTH_MIN_ERR_MASK                                              0x00000004
#define B0_PHY_RX_BE_LSIG_ERROR_CAUSE_FIELD_SIG1_RESERVED_ERR_MASK                                                0x00000008
#define B0_PHY_RX_BE_LSIG_ERROR_CAUSE_FIELD_SIG1_LENGTH_MAX_ERR_MASK                                              0x00000010
//========================================
/* REG_PHY_RX_BE_MAXIMUM_PACKET_SIZE 0x3D8 */
#define B0_PHY_RX_BE_MAXIMUM_PACKET_SIZE_FIELD_MAXIMUM_PACKET_SIZE_MASK                                           0x00000FFF
//========================================
/* REG_PHY_RX_BE_SIG3_ERROR_CAUSE 0x3DC */
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_CRC_ERROR_MASK                                                   0x00000001
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_MCS_ERROR_MASK                                                   0x00000002
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_AIR_TIME_ERR_MASK                                                0x00000004
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_NDP_PE_ERR_MASK                                                  0x00000008
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_NDP_INVALID_HE_SIGA_GI_LTF_ERR_MASK                              0x00000010
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_BCC_MCS_ERR_MASK                                                 0x00000020
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_BCC_BW_ERR_MASK                                                  0x00000040
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_HE_ER_ERR_MASK                                                   0x00000080
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_WRONG_BAND_ERR_MASK                                              0x00000100
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_HT_PACKET_MASK                                                   0x00000200
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_VHT_PACKET_MASK                                                  0x00000400
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_DCM_ERR_MASK                                                     0x00000800
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_EHT_SIG_NSYM_ERR_MASK                                            0x00001000
#define B0_PHY_RX_BE_SIG3_ERROR_CAUSE_FIELD_SIG3_RESERVED_ERR_MASK                                                0x00002000
//========================================
/* REG_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE 0x3E0 */
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_CRC_ERROR_MASK                                         0x00000001
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_MCS_ERROR_MASK                                         0x00000002
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_AIR_TIME_ERR_MASK                                      0x00000004
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_NDP_PE_ERR_MASK                                        0x00000008
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_NDP_INVALID_HE_SIGA_GI_LTF_ERR_MASK                    0x00000010
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_BCC_MCS_ERR_MASK                                       0x00000020
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_BCC_BW_ERR_MASK                                        0x00000040
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_HE_ER_ERR_MASK                                         0x00000080
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_WRONG_BAND_ERR_MASK                                    0x00000100
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_HT_PACKET_MASK                                         0x00000200
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_VHT_PACKET_MASK                                        0x00000400
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_DCM_ERR_MASK                                           0x00000800
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_EHT_SIG_NSYM_ERR_MASK                                  0x00001000
#define B0_PHY_RX_BE_MASK_SIG3_ERROR_CAUSE_FIELD_MASK_SIG3_RESERVED_ERR_MASK                                      0x00002000
//========================================
/* REG_PHY_RX_BE_EHTSIG_ERROR_CAUSE 0x3E4 */
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_BCC_MCS_ERR_MASK                                                0x00000001
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_BCC_BW_ERR_MASK                                                 0x00000002
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_MCS15_ERR_MASK                                                  0x00000004
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_AIR_TIME_ERR_MASK                                               0x00000008
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_NDP_ERR_MASK                                                    0x00000010
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_SU_NEHT_LTF_ERR_MASK                                            0x00000020
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_CRC_ERR_MASK                                                    0x00000040
#define B0_PHY_RX_BE_EHTSIG_ERROR_CAUSE_FIELD_EHT_DUP_MODE_5G_MASK                                                0x00000080
//========================================
/* REG_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE 0x3E8 */
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_BCC_MCS_ERR_MASK                                      0x00000001
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_BCC_BW_ERR_MASK                                       0x00000002
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_MCS15_ERR_MASK                                        0x00000004
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_AIR_TIME_ERR_MASK                                     0x00000008
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_NDP_ERR_MASK                                          0x00000010
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_SU_NEHT_LTF_ERR_MASK                                  0x00000020
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_CRC_ERR_MASK                                          0x00000040
#define B0_PHY_RX_BE_MASK_EHTSIG_ERROR_CAUSE_FIELD_MASK_EHT_DUP_MODE_5G_MASK                                      0x00000080
//========================================
/* REG_PHY_RX_BE_RX_REWIND_CONFIG 0x3EC */
#define B0_PHY_RX_BE_RX_REWIND_CONFIG_FIELD_NSYM_REWIND_OVERRIDE_MASK                                             0x0000003F
#define B0_PHY_RX_BE_RX_REWIND_CONFIG_FIELD_REWIND_OVERRIDE_ACTIVE_MASK                                           0x00000080
#define B0_PHY_RX_BE_RX_REWIND_CONFIG_FIELD_TSYM_ACC_OVERRIDE_MASK                                                0x3FFF0000
#define B0_PHY_RX_BE_RX_REWIND_CONFIG_FIELD_REWIND_ENABLE_OVERRIDE_MASK                                           0x80000000
//========================================
/* REG_PHY_RX_BE_HAMMING_DISTANCE_ENABLE 0x3F0 */
#define B0_PHY_RX_BE_HAMMING_DISTANCE_ENABLE_FIELD_HAMMING_DISTANCE_ENABLE_MASK                                   0x00000001
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE 0x3F4 */
#define B0_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE_FIELD_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE_MASK 0x00000FFF
#define B0_PHY_RX_BE_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE_FIELD_LDPC_CYCLES_STEADY_STATE_REWIND_OVERRIDE_EN_MASK 0x00008000
//========================================
/* REG_PHY_RX_BE_PM_MAX_NSYM_REWIND 0x3F8 */
#define B0_PHY_RX_BE_PM_MAX_NSYM_REWIND_FIELD_PM_MAX_NSYM_REWIND_VHT_CP_04_MASK                                   0x0000003F
#define B0_PHY_RX_BE_PM_MAX_NSYM_REWIND_FIELD_PM_MAX_NSYM_REWIND_VHT_CP_08_MASK                                   0x00000FC0
#define B0_PHY_RX_BE_PM_MAX_NSYM_REWIND_FIELD_PM_MAX_NSYM_REWIND_EHT_CP_08_MASK                                   0x0003F000
#define B0_PHY_RX_BE_PM_MAX_NSYM_REWIND_FIELD_PM_MAX_NSYM_REWIND_EHT_CP_16_MASK                                   0x00FC0000
#define B0_PHY_RX_BE_PM_MAX_NSYM_REWIND_FIELD_PM_MAX_NSYM_REWIND_EHT_CP_32_MASK                                   0x3F000000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_2G_0 0x3FC */
#define B0_PHY_RX_BE_PM_QR_LIMIT_2G_0_FIELD_PM_QR_LIMIT_1SS_2G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_2G_0_FIELD_PM_QR_LIMIT_2SS_2G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_2G_1 0x400 */
#define B0_PHY_RX_BE_PM_QR_LIMIT_2G_1_FIELD_PM_QR_LIMIT_3SS_2G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_2G_1_FIELD_PM_QR_LIMIT_4SS_2G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_PM_FD_OVERHEAD 0x404 */
#define B0_PHY_RX_BE_PM_FD_OVERHEAD_FIELD_PM_FD_OVERHEAD_MASK                                                     0x000003FF
//========================================
/* REG_PHY_RX_BE_PM_TSYM_ACC_MAC_OVERRIDE 0x408 */
#define B0_PHY_RX_BE_PM_TSYM_ACC_MAC_OVERRIDE_FIELD_PM_TSYM_ACC_MAC_OVERRIDE_MASK                                 0x00003FFF
//========================================
/* REG_PHY_RX_BE_PM_MIN_NSYM_REWIND 0x40C */
#define B0_PHY_RX_BE_PM_MIN_NSYM_REWIND_FIELD_PM_MIN_NSYM_REWIND_MASK                                             0x0000003F
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_0 0x410 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_0_FIELD_PM_LDPC_CYCLES_REDUCE_500_550_MASK                             0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_0_FIELD_PM_LDPC_CYCLES_REDUCE_550_600_MASK                             0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_1 0x414 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_1_FIELD_PM_LDPC_CYCLES_REDUCE_600_650_MASK                             0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_1_FIELD_PM_LDPC_CYCLES_REDUCE_650_700_MASK                             0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_2 0x418 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_2_FIELD_PM_LDPC_CYCLES_REDUCE_700_750_MASK                             0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_2_FIELD_PM_LDPC_CYCLES_REDUCE_750_800_MASK                             0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_3 0x41C */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_3_FIELD_PM_LDPC_CYCLES_REDUCE_800_900_MASK                             0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_3_FIELD_PM_LDPC_CYCLES_REDUCE_900_1000_MASK                            0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_4 0x420 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_4_FIELD_PM_LDPC_CYCLES_REDUCE_1000_1100_MASK                           0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_4_FIELD_PM_LDPC_CYCLES_REDUCE_1100_1200_MASK                           0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_5 0x424 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_5_FIELD_PM_LDPC_CYCLES_REDUCE_1200_1300_MASK                           0x00000FFF
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_5_FIELD_PM_LDPC_CYCLES_REDUCE_1300_1400_MASK                           0x0FFF0000
//========================================
/* REG_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_6 0x428 */
#define B0_PHY_RX_BE_PM_LDPC_CYCLES_REDUCE_6_FIELD_PM_LDPC_CYCLES_REDUCE_1400_1500_MASK                           0x00000FFF
//========================================
/* REG_PHY_RX_BE_REWIND_READ_ONLY_0 0x42C */
#define B0_PHY_RX_BE_REWIND_READ_ONLY_0_FIELD_TSYM_POST_REWIND_RO_MASK                                            0x00003FFF
#define B0_PHY_RX_BE_REWIND_READ_ONLY_0_FIELD_TSYM_PRE_REWIND_RO_MASK                                             0x3FFF0000
//========================================
/* REG_PHY_RX_BE_REWIND_READ_ONLY_1 0x430 */
#define B0_PHY_RX_BE_REWIND_READ_ONLY_1_FIELD_REWIND_ENABLE_RO_MASK                                               0x00000001
#define B0_PHY_RX_BE_REWIND_READ_ONLY_1_FIELD_NSYM_REWIND_RO_MASK                                                 0x003F0000
#define B0_PHY_RX_BE_REWIND_READ_ONLY_1_FIELD_REWIND_CALC_READY_RO_MASK                                           0x80000000
//========================================
/* REG_PHY_RX_BE_PM_TSYM_ACC_FD_LOW_LIMIT 0x434 */
#define B0_PHY_RX_BE_PM_TSYM_ACC_FD_LOW_LIMIT_FIELD_PM_TSYM_ACC_FD_LOW_LIMIT_HT_VHT_MASK                          0x00003FFF
#define B0_PHY_RX_BE_PM_TSYM_ACC_FD_LOW_LIMIT_FIELD_PM_TSYM_ACC_FD_LOW_LIMIT_HE_EHT_MASK                          0x3FFF0000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_5G_0 0x438 */
#define B0_PHY_RX_BE_PM_QR_LIMIT_5G_0_FIELD_PM_QR_LIMIT_1SS_5G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_5G_0_FIELD_PM_QR_LIMIT_2SS_5G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_5G_1 0x43C */
#define B0_PHY_RX_BE_PM_QR_LIMIT_5G_1_FIELD_PM_QR_LIMIT_3SS_5G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_5G_1_FIELD_PM_QR_LIMIT_4SS_5G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_6G_0 0x440 */
#define B0_PHY_RX_BE_PM_QR_LIMIT_6G_0_FIELD_PM_QR_LIMIT_1SS_6G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_6G_0_FIELD_PM_QR_LIMIT_2SS_6G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_PM_QR_LIMIT_6G_1 0x444 */
#define B0_PHY_RX_BE_PM_QR_LIMIT_6G_1_FIELD_PM_QR_LIMIT_3SS_6G_MASK                                               0x000001FF
#define B0_PHY_RX_BE_PM_QR_LIMIT_6G_1_FIELD_PM_QR_LIMIT_4SS_6G_MASK                                               0x01FF0000
//========================================
/* REG_PHY_RX_BE_LDPC_MAX_CYCLES 0x448 */
#define B0_PHY_RX_BE_LDPC_MAX_CYCLES_FIELD_LDPC_MAX_CYCLES_SU_MASK                                                0x00000FFF
#define B0_PHY_RX_BE_LDPC_MAX_CYCLES_FIELD_LDPC_MAX_CYCLES_MU_MASK                                                0x0FFF0000
//========================================
/* REG_PHY_RX_BE_DFE_POST_REWIND_ENABLE 0x44C */
#define B0_PHY_RX_BE_DFE_POST_REWIND_ENABLE_FIELD_DFE_POST_REWIND_ENABLE_MASK                                     0x00000001
//========================================
/* REG_PHY_RX_BE_LDPC_CYCLES_STEADY_REWIND_RO 0x450 */
#define B0_PHY_RX_BE_LDPC_CYCLES_STEADY_REWIND_RO_FIELD_LDPC_CYCLES_STEADY_REWIND_RO_MASK                         0x00000FFF
//========================================
/* REG_PHY_RX_BE_PM_REWIND_ENABLE 0x454 */
#define B0_PHY_RX_BE_PM_REWIND_ENABLE_FIELD_PM_REWIND_ENABLE_MASK                                                 0x00000001
//========================================
/* REG_PHY_RX_BE_DFE_ENABLE_POST_REWIND 0x458 */
#define B0_PHY_RX_BE_DFE_ENABLE_POST_REWIND_FIELD_DFE_ENABLE_POST_REWIND_MASK                                     0x00000001
//========================================
/* REG_PHY_RX_BE_PM_REWIND_ENABLE_WHEN_DFE_OFF 0x45C */
#define B0_PHY_RX_BE_PM_REWIND_ENABLE_WHEN_DFE_OFF_FIELD_PM_REWIND_ENABLE_WHEN_DFE_OFF_MASK                       0x00000001
//========================================
/* REG_PHY_RX_BE_PM_TSYM_ACC_TX_LOW_LIMIT 0x460 */
#define B0_PHY_RX_BE_PM_TSYM_ACC_TX_LOW_LIMIT_FIELD_PM_TSYM_ACC_TX_LOW_LIMIT_HT_VHT_MASK                          0x00003FFF
#define B0_PHY_RX_BE_PM_TSYM_ACC_TX_LOW_LIMIT_FIELD_PM_TSYM_ACC_TX_LOW_LIMIT_HE_EHT_MASK                          0x1FFF8000
//========================================
/* REG_PHY_RX_BE_PM_REWIND_POWER_SAVE 0x464 */
#define B0_PHY_RX_BE_PM_REWIND_POWER_SAVE_FIELD_PM_REWIND_POWER_SAVE_MASK                                         0x00000001
//========================================
/* REG_PHY_RX_BE_REWIND_CANCEL_RO 0x468 */
#define B0_PHY_RX_BE_REWIND_CANCEL_RO_FIELD_REWIND_CANCEL_RO_MASK                                                 0x00000001
//========================================
/* REG_PHY_RX_BE_NCW_TH_REWIND_CANCEL_RO 0x46C */
#define B0_PHY_RX_BE_NCW_TH_REWIND_CANCEL_RO_FIELD_NCW_TH_REWIND_CANCEL_RO_MASK                                   0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PM_FORCE_CONV_FAIL_ON_CW 0x470 */
#define B0_PHY_RX_BE_PM_FORCE_CONV_FAIL_ON_CW_FIELD_PM_FORCE_CONV_FAIL_ON_CW_MASK                                 0x0000FFFF
//========================================
/* REG_PHY_RX_BE_PM_SUPPORT_HE_MU_DL 0x474 */
#define B0_PHY_RX_BE_PM_SUPPORT_HE_MU_DL_FIELD_PM_SUPPORT_HE_MU_DL_MASK                                           0x00000001
//========================================
/* REG_PHY_RX_BE_CW_CONV_CNT_PRE_REWIND 0x478 */
#define B0_PHY_RX_BE_CW_CONV_CNT_PRE_REWIND_FIELD_CW_CONV_CNT_PRE_REWIND_MASK                                     0x0000FFFF
#define B0_PHY_RX_BE_CW_CONV_CNT_PRE_REWIND_FIELD_CW_NO_CONV_CNT_PRE_REWIND_MASK                                  0xFFFF0000
//========================================
/* REG_PHY_RX_BE_CW_CONV_CNT_POST_REWIND 0x47C */
#define B0_PHY_RX_BE_CW_CONV_CNT_POST_REWIND_FIELD_CW_CONV_CNT_POST_REWIND_MASK                                   0x0000FFFF
#define B0_PHY_RX_BE_CW_CONV_CNT_POST_REWIND_FIELD_CW_NO_CONV_CNT_POST_REWIND_MASK                                0xFFFF0000
//========================================
/* REG_PHY_RX_BE_CHK_BIT_EXTRA_LDPC_CHECK 0x480 */
#define B0_PHY_RX_BE_CHK_BIT_EXTRA_LDPC_CHECK_FIELD_CHK_BIT_EXTRA_LDPC_CHECK_MASK                                 0x00000001
//========================================
/* REG_PHY_RX_BE_HE_STATION_INFO 0x484 */
#define B0_PHY_RX_BE_HE_STATION_INFO_FIELD_STATION_NUM_USERS_MASK                                                 0x000000FF
#define B0_PHY_RX_BE_HE_STATION_INFO_FIELD_OUR_USER_ADDRESS_MASK                                                  0x00007F00
//========================================
/* REG_PHY_RX_BE_DFE_LOW_RATES_EN 0x488 */
#define B0_PHY_RX_BE_DFE_LOW_RATES_EN_FIELD_DFE_LOW_RATES_EN_MASK                                                 0x00000001
//========================================
/* REG_PHY_RX_BE_DFE_MAX_NSYM_WAIT_VHT 0x48C */
#define B0_PHY_RX_BE_DFE_MAX_NSYM_WAIT_VHT_FIELD_DFE_MAX_NSYM_WAIT_VHT_MASK                                       0x0000FFFF
#define B0_PHY_RX_BE_DFE_MAX_NSYM_WAIT_VHT_FIELD_DFE_MAX_NSYM_WAIT_EHT_MASK                                       0xFFFF0000
//========================================
/* REG_PHY_RX_BE_DFE_REQ_CW_VEC 0x490 */
#define B0_PHY_RX_BE_DFE_REQ_CW_VEC_FIELD_DFE_REQ_CW_VEC_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_BE_PM_MIN_NSYM_DFE_REWIND 0x494 */
#define B0_PHY_RX_BE_PM_MIN_NSYM_DFE_REWIND_FIELD_PM_MIN_NSYM_DFE_REWIND_MASK                                     0x0000003F
//========================================
/* REG_PHY_RX_BE_SENSING_MAC_INPUTS 0x498 */
#define B0_PHY_RX_BE_SENSING_MAC_INPUTS_FIELD_NDP_TYPE_MASK                                                       0x00000003
#define B0_PHY_RX_BE_SENSING_MAC_INPUTS_FIELD_SENSING_REPETITION_MASK                                             0x0000001C
#define B0_PHY_RX_BE_SENSING_MAC_INPUTS_FIELD_TXOP_NON_TB_SENSING_MASK                                            0x00000020
#define B0_PHY_RX_BE_SENSING_MAC_INPUTS_FIELD_ENABLE_SENSING_DEBUG_PHY_ONLY_MASK                                  0x00000040
//========================================
/* REG_PHY_RX_BE_SENSING_MAC_VALID 0x49C */
#define B0_PHY_RX_BE_SENSING_MAC_VALID_FIELD_SENSING_MAC_VALID_MASK                                               0x00000001


#endif // _PHY_RX_BE_REGS_H_
