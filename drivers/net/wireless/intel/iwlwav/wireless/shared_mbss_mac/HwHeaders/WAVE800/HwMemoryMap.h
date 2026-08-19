/************************************************************************************
*
* File: HwMemoryMap.h
* Module: Hw Headers
* Description: This file was auto generated using ExcelMemoryMapParserWave700.py
*
*  source file 1: memory_map_ver2.0.0.xlsx
*  sha256 1: 30c56a30c84c5c387fcb23681930f1aa75315df40a3d33c6e5e8423512dadef4
*
*  source file 2: wave700_rams_by_modules_ver2.0.0.xlsm
*  sha256 2: 82f160b83c9a67bb5d9689eb29f930403c99ec845148219e5f98a42845e28757
*
* COPYRIGHT:
*  Copyright(c) 2020, MaxLinear, Inc.
*  Copyright 2016 - 2020 Intel Corporation
*  Copyright 2014 - 2016 Lantiq Beteiligungs - GmbH & Co.KG
*  Copyright 2010 - 2014 Lantiq Deutschland GmbH
*  Copyright 2007 - 2010 Metalink Ltd.
*
**************************************************************************************/
#ifndef _MEMORY_MAP_H_ 
#define _MEMORY_MAP_H_ 

/* Unit Base Address Definitions*/
#define MEMORY_MAP_UNIT_1_BASE_ADDRESS                                            (SYSTEM_IF_UPPER_CPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_2_BASE_ADDRESS                                            (SYSTEM_IF_LOWER_B0_CPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_4_BASE_ADDRESS                                            (HOST_IF_REG_FILE_BASE_ADDR)
#define MEMORY_MAP_UNIT_5_BASE_ADDRESS                                            (VAP_ADDR_REG_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_6_BASE_ADDRESS                                            (B0_OTFA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_7_BASE_ADDRESS                                            (B0_RX_COORDINATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_8_BASE_ADDRESS                                            (B0_PAC_DELIA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_9_BASE_ADDRESS                                            (B0_PAC_RXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10_BASE_ADDRESS                                           (B0_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_11_BASE_ADDRESS                                           (B0_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_12_BASE_ADDRESS                                           (B0_BUF_STATUS_REPORT_BASE_ADDR)
#define MEMORY_MAP_UNIT_13_BASE_ADDRESS                                           (B0_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_14_BASE_ADDRESS                                           (B0_TF_DECODER_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_15_BASE_ADDRESS                                           (B0_MAC_BEACON_BASE_ADDR)
#define MEMORY_MAP_UNIT_16_BASE_ADDRESS                                           (B0_BSRC_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_17_BASE_ADDRESS                                           (B0_MAC_WEP_BASE_ADDR)
#define MEMORY_MAP_UNIT_18_BASE_ADDRESS                                           (B0_RAB_BASE_ADDR)
#define MEMORY_MAP_UNIT_19_BASE_ADDRESS                                           (PVT_BLOCK_BASE_ADDR)
#define MEMORY_MAP_UNIT_20_BASE_ADDRESS                                           (B0_MAC_TX_HC_BASE_ADDR)
#define MEMORY_MAP_UNIT_21_BASE_ADDRESS                                           (B0_PRE_AGG_BASE_ADDR)
#define MEMORY_MAP_UNIT_22_BASE_ADDRESS                                           (B0_MAC_ADDR2INDEX_BASE_ADDR)
#define MEMORY_MAP_UNIT_23_BASE_ADDRESS                                           (B0_TX_HANDLER_BASE_ADDR)
#define MEMORY_MAP_UNIT_24_BASE_ADDRESS                                           (B0_MAC_HT_EXTENSIONS_BASE_ADDR)
#define MEMORY_MAP_UNIT_25_BASE_ADDRESS                                           (B0_AUTO_RESP_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_26_BASE_ADDRESS                                           (B0_MAC_GENRISC_TX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_27_BASE_ADDRESS                                           (B0_MAC_GENRISC_RX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_28_BASE_ADDRESS                                           (MAC_HOSTIF_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_29_BASE_ADDRESS                                           (B0_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_30_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_TX1_BASE_ADDR)
#define MEMORY_MAP_UNIT_31_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_RX1_BASE_ADDR)
#define MEMORY_MAP_UNIT_32_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_LCPU1_BASE_ADDR)
#define MEMORY_MAP_UNIT_33_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_TX0_BASE_ADDR)
#define MEMORY_MAP_UNIT_34_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_HOST_BASE_ADDR)
#define MEMORY_MAP_UNIT_35_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_RX0_BASE_ADDR)
#define MEMORY_MAP_UNIT_36_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_UCPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_37_BASE_ADDRESS                                           (QUEUE_MANAGER_M01_LCPU0_BASE_ADDR)
#define MEMORY_MAP_UNIT_38_BASE_ADDRESS                                           (DMA_WRAPPER_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_39_BASE_ADDRESS                                           (HOST_IF_ACC_BASE_ADDR)
#define MEMORY_MAP_UNIT_40_BASE_ADDRESS                                           (DLM_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_41_BASE_ADDRESS                                           (MAC_GEN_REGFILE_BASE_ADDR)
#define MEMORY_MAP_UNIT_42_BASE_ADDRESS                                           (B0_TX_PD_ACC_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_43_BASE_ADDRESS                                           (B0_UL_POST_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_44_BASE_ADDRESS                                           (B0_TF_GENERATOR_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_45_BASE_ADDRESS                                           (B0_BA_ANALYZER_BASE_ADDR)
#define MEMORY_MAP_UNIT_46_BASE_ADDRESS                                           (PROT_DB_BASE_ADDR)
#define MEMORY_MAP_UNIT_47_BASE_ADDRESS                                           (RX_PP_REGS_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_50_BASE_ADDRESS                                           (WLAN_DMA_TX_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_51_BASE_ADDRESS                                           (WLAN_DMA_RX_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_52_BASE_ADDRESS                                           (WLAN_DMA_FW_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_53_BASE_ADDRESS                                           (B0_TXH_TXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_54_BASE_ADDRESS                                           (B0_TXH_NTD_BASE_ADDR)
#define MEMORY_MAP_UNIT_56_BASE_ADDRESS                                           (B0_TWT_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_57_BASE_ADDRESS                                           (B0_RX_CLASSIFIER_BASE_ADDR)
#define MEMORY_MAP_UNIT_58_BASE_ADDRESS                                           (B0_PS_SETTING_BASE_ADDR)
#define MEMORY_MAP_UNIT_59_BASE_ADDRESS                                           (TOP_REGBLOCK_BASE_ADDR)
#define MEMORY_MAP_UNIT_60_BASE_ADDRESS                                           (PCIE_DBI0_REGBLOCK_BASE_ADDR)
#define MEMORY_MAP_UNIT_61_BASE_ADDRESS                                           (DATA_LOGGER_BASE_ADDR)
#define MEMORY_MAP_UNIT_62_BASE_ADDRESS                                           (B0_PAC_EXTRAPOLATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_63_BASE_ADDRESS                                           (B0_RXH_RD_ACC_BASE_ADDR)
#define MEMORY_MAP_UNIT_64_BASE_ADDRESS                                           (B0_TIM_IE_BASE_ADDR)
#define MEMORY_MAP_UNIT_65_BASE_ADDRESS                                           (B0_RX_MSDU_PARSER_BASE_ADDR)
#define MEMORY_MAP_UNIT_66_BASE_ADDRESS                                           (B0_BESTRU_BASE_ADDR)
#define MEMORY_MAP_UNIT_67_BASE_ADDRESS                                           (BSRC_CNT_MEM_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_68_BASE_ADDRESS                                           (B0_TX_SEQUENCER_BASE_ADDR)
#define MEMORY_MAP_UNIT_69_BASE_ADDRESS                                           (RF_EMULATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_75_BASE_ADDRESS                                           (PCIE_PHY0_REGBLOCK_BASE_ADDR)
#define MEMORY_MAP_UNIT_200_BASE_ADDRESS                                          (B0_PHY_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_201_BASE_ADDRESS                                          (B0_PHY_RX_BE_BASE_ADDR)
#define MEMORY_MAP_UNIT_202_BASE_ADDRESS                                          (B0_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_203_BASE_ADDRESS                                          (B0_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_204_BASE_ADDRESS                                          (B0_UP_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_205_BASE_ADDRESS                                          (B0_UP_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_206_BASE_ADDRESS                                          (B0_PHY_DFS_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_207_BASE_ADDRESS                                          (B0_PHY_DFS_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_208_BASE_ADDRESS                                          (B0_PHY_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_209_BASE_ADDRESS                                          (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR)
#define MEMORY_MAP_UNIT_211_BASE_ADDRESS                                          (B0_PHY_RX_TD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_212_BASE_ADDRESS                                          (B0_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_213_BASE_ADDRESS                                          (B0_UP_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_214_BASE_ADDRESS                                          (B0_PHY_DFS_RXTD_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_215_BASE_ADDRESS                                          (B0_PHY_DFS_AGC_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_220_BASE_ADDRESS                                          (B0_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_221_BASE_ADDRESS                                          (B0_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_222_BASE_ADDRESS                                          (B0_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_223_BASE_ADDRESS                                          (B0_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_224_BASE_ADDRESS                                          (B0_UP_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_225_BASE_ADDRESS                                          (B0_UP_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_226_BASE_ADDRESS                                          (B0_UP_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_227_BASE_ADDRESS                                          (B0_UP_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_230_BASE_ADDRESS                                          (B0_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_231_BASE_ADDRESS                                          (B0_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_232_BASE_ADDRESS                                          (B0_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_233_BASE_ADDRESS                                          (B0_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_234_BASE_ADDRESS                                          (B0_UP_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_235_BASE_ADDRESS                                          (B0_UP_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_236_BASE_ADDRESS                                          (B0_UP_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_237_BASE_ADDRESS                                          (B0_UP_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_240_BASE_ADDRESS                                          (B0_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_241_BASE_ADDRESS                                          (B0_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_242_BASE_ADDRESS                                          (B0_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_243_BASE_ADDRESS                                          (B0_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_244_BASE_ADDRESS                                          (B0_UP_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_245_BASE_ADDRESS                                          (B0_UP_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_246_BASE_ADDRESS                                          (B0_UP_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_247_BASE_ADDRESS                                          (B0_UP_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_500_BASE_ADDRESS                                          (B0_HOB_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_501_BASE_ADDRESS                                          (B0_HOB_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_502_BASE_ADDRESS                                          (B0_HOB_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_503_BASE_ADDRESS                                          (B0_HOB_PAC_DEL_BASE_ADDR)
#define MEMORY_MAP_UNIT_504_BASE_ADDRESS                                          (B0_HOB_AUTO_RESP_BASE_ADDR)
#define MEMORY_MAP_UNIT_505_BASE_ADDRESS                                          (B0_HOB_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_1000_BASE_ADDRESS                                         (CPU_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1002_BASE_ADDRESS                                         (SYSTEM_IF_LOWER_B1_CPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_1003_BASE_ADDRESS                                         (SYSTEM_IF_LOWER_B2_CPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_1006_BASE_ADDRESS                                         (BEST_RU_DB_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1008_BASE_ADDRESS                                         (WLAN_DMA_LL_MEM_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1012_BASE_ADDRESS                                         (RX_COORDINATOR_COUNTERS_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1015_BASE_ADDRESS                                         (PROT_DB_SHADOW_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1017_BASE_ADDRESS                                         (BSRC_AGER_MEM_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1018_BASE_ADDRESS                                         (SHARED_RAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1020_BASE_ADDRESS                                         (B0_RX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_1021_BASE_ADDRESS                                         (B0_RX_PHY_STATUS_BASE_ADDR)
#define MEMORY_MAP_UNIT_1022_BASE_ADDRESS                                         (B0_TX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_1023_BASE_ADDRESS                                         (B0_TX_RECIPES_BASE_ADDR)
#define MEMORY_MAP_UNIT_1024_BASE_ADDRESS                                         (B0_MAC_GENRISC_TX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1025_BASE_ADDRESS                                         (HOST_IF_QOS_COUNTERS_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1026_BASE_ADDRESS                                         (B0_MAC_GENRISC_TX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1027_BASE_ADDRESS                                         (B0_TWT_GROUP_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1029_BASE_ADDRESS                                         (B0_MAC_GENRISC_TX_STD_BASE_ADDR)
#define MEMORY_MAP_UNIT_1031_BASE_ADDRESS                                         (B0_MAC_GENRISC_RX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1033_BASE_ADDRESS                                         (B0_MAC_GENRISC_RX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1038_BASE_ADDRESS                                         (BEST_RU_DB_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1041_BASE_ADDRESS                                         (B0_TF_DECODER_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1042_BASE_ADDRESS                                         (EXT_RX_PP_COUNTERS_BASE_ADDR)
#define MEMORY_MAP_UNIT_1048_BASE_ADDRESS                                         (B0_TWT_STATION_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1049_BASE_ADDRESS                                         (BA_ANALYZER_COUNTER_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1052_BASE_ADDRESS                                         (B0_RXF_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1053_BASE_ADDRESS                                         (B0_AUTO_RESP_MEMS_BASE_ADDR)
#define MEMORY_MAP_UNIT_1054_BASE_ADDRESS                                         (TRAFFIC_INDICATOR_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1055_BASE_ADDRESS                                         (B0_DUR_AUTO_RATE_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1056_BASE_ADDRESS                                         (RETRY_STATE_DB_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1058_BASE_ADDRESS                                         (B0_FRAME_CLASS_VIO_BUFFER_BASE_ADDR)
#define MEMORY_MAP_UNIT_1060_BASE_ADDRESS                                         (BAND1_INTERNAL_BASE_ADDR)
#define MEMORY_MAP_UNIT_1061_BASE_ADDRESS                                         (BAND1_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1062_BASE_ADDRESS                                         (BAND1_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1064_BASE_ADDRESS                                         (HOST_EX_MAP_CPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_1068_BASE_ADDRESS                                         (RETRY_POINTER_DB_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1069_BASE_ADDRESS                                         (B0_PAC_DELIA_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1073_BASE_ADDRESS                                         (B0_OTFA_MEM2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1075_BASE_ADDRESS                                         (HOST_IF_COUNTERS_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1077_BASE_ADDRESS                                         (B0_MAC_SEC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1078_BASE_ADDRESS                                         (BAND2_INTERNAL_BASE_ADDR)
#define MEMORY_MAP_UNIT_1080_BASE_ADDRESS                                         (TIM_GEN_BASE_ADDR)
#define MEMORY_MAP_UNIT_1082_BASE_ADDRESS                                         (SHARED_RAM_GRAY_BASE_ADDR)
#define MEMORY_MAP_UNIT_1084_BASE_ADDRESS                                         (RX_PP_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1085_BASE_ADDRESS                                         (B0_TF_GENERATOR_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1086_BASE_ADDRESS                                         (BOOT_ROM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1088_BASE_ADDRESS                                         (B0_MPDU_DESC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1089_BASE_ADDRESS                                         (SHARED_RAM_WHITE_BASE_ADDR)
#define MEMORY_MAP_UNIT_1094_BASE_ADDRESS                                         (RX_PP_COUNTERS_M01_BASE_ADDR)
#define MEMORY_MAP_UNIT_1095_BASE_ADDRESS                                         (SHARED_RAM_BLACK_BASE_ADDR)
#define MEMORY_MAP_UNIT_1097_BASE_ADDRESS                                         (B0_TX_PD_ACC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1101_BASE_ADDRESS                                         (PLAN_RAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1103_BASE_ADDRESS                                         (DESCRIPTOR_RAM_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1104_BASE_ADDRESS                                         (DESCRIPTOR_RAM_RX_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1105_BASE_ADDRESS                                         (VAP_ADDR_REG_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1106_BASE_ADDRESS                                         (DESCRIPTOR_RAM_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1108_BASE_ADDRESS                                         (RX_COORDINATOR_COUNTERS_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1109_BASE_ADDRESS                                         (HOST_IF_AQM_STAT_BASE_ADDR)
#define MEMORY_MAP_UNIT_1110_BASE_ADDRESS                                         (B0_MLD_DB_BASE_ADDR)
#define MEMORY_MAP_UNIT_1111_BASE_ADDRESS                                         (BAND2_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1112_BASE_ADDRESS                                         (BAND2_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_1115_BASE_ADDRESS                                         (CPU_XBAR_STATUS_BASE_ADDR)
#define MEMORY_MAP_UNIT_1116_BASE_ADDRESS                                         (MAC_XBAR_PM_BASE_ADDR)
#define MEMORY_MAP_UNIT_1117_BASE_ADDRESS                                         (DLM_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1120_BASE_ADDRESS                                         (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR)
#define MEMORY_MAP_UNIT_1121_BASE_ADDRESS                                         (BSRC_AGER_MEM_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1122_BASE_ADDRESS                                         (MAC_XBAR_STATUS_BASE_ADDR)
#define MEMORY_MAP_UNIT_1125_BASE_ADDRESS                                         (HOST_IF_QOS_COUNTERS_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1126_BASE_ADDRESS                                         (QUEUE_MANAGER_M01_TX2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1127_BASE_ADDRESS                                         (QUEUE_MANAGER_M01_RX2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1128_BASE_ADDRESS                                         (QUEUE_MANAGER_M01_LCPU2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1130_BASE_ADDRESS                                         (B0_TX_SENDER_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_1131_BASE_ADDRESS                                         (B0_RX_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_1132_BASE_ADDRESS                                         (B0_TX_SENDER_SINGLE_UC_BASE_ADDR)
#define MEMORY_MAP_UNIT_1133_BASE_ADDRESS                                         (B0_RX_SINGLE_UC_BASE_ADDR)
#define MEMORY_MAP_UNIT_1149_BASE_ADDRESS                                         (BA_ANALYZER_COUNTER_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1154_BASE_ADDRESS                                         (TRAFFIC_INDICATOR_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1156_BASE_ADDRESS                                         (RETRY_STATE_DB_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1167_BASE_ADDRESS                                         (BSRC_CNT_MEM_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1168_BASE_ADDRESS                                         (RETRY_POINTER_DB_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1175_BASE_ADDRESS                                         (HOST_IF_COUNTERS_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1194_BASE_ADDRESS                                         (RX_PP_COUNTERS_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1208_BASE_ADDRESS                                         (WLAN_DMA_LL_MEM_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1220_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_TX2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1221_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_RX2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1222_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_LCPU2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1230_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_TX1_BASE_ADDR)
#define MEMORY_MAP_UNIT_1231_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_RX1_BASE_ADDR)
#define MEMORY_MAP_UNIT_1232_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_LCPU1_BASE_ADDR)
#define MEMORY_MAP_UNIT_1233_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_TX0_BASE_ADDR)
#define MEMORY_MAP_UNIT_1234_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_HOST_BASE_ADDR)
#define MEMORY_MAP_UNIT_1235_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_RX0_BASE_ADDR)
#define MEMORY_MAP_UNIT_1236_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_UCPU_BASE_ADDR)
#define MEMORY_MAP_UNIT_1237_BASE_ADDRESS                                         (QUEUE_MANAGER_M2_LCPU0_BASE_ADDR)
#define MEMORY_MAP_UNIT_1238_BASE_ADDRESS                                         (DMA_WRAPPER_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1247_BASE_ADDRESS                                         (RX_PP_REGS_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1250_BASE_ADDRESS                                         (WLAN_DMA_TX_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1251_BASE_ADDRESS                                         (WLAN_DMA_RX_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1252_BASE_ADDRESS                                         (WLAN_DMA_FW_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_1284_BASE_ADDRESS                                         (RX_PP_M2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10006_BASE_ADDRESS                                        (B1_OTFA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10007_BASE_ADDRESS                                        (B1_RX_COORDINATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_10008_BASE_ADDRESS                                        (B1_PAC_DELIA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10009_BASE_ADDRESS                                        (B1_PAC_RXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10010_BASE_ADDRESS                                        (B1_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_10011_BASE_ADDRESS                                        (B1_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_10012_BASE_ADDRESS                                        (B1_BUF_STATUS_REPORT_BASE_ADDR)
#define MEMORY_MAP_UNIT_10013_BASE_ADDRESS                                        (B1_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_10014_BASE_ADDRESS                                        (B1_TF_DECODER_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10015_BASE_ADDRESS                                        (B1_MAC_BEACON_BASE_ADDR)
#define MEMORY_MAP_UNIT_10016_BASE_ADDRESS                                        (B1_BSRC_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_10017_BASE_ADDRESS                                        (B1_MAC_WEP_BASE_ADDR)
#define MEMORY_MAP_UNIT_10018_BASE_ADDRESS                                        (B1_RAB_BASE_ADDR)
#define MEMORY_MAP_UNIT_10020_BASE_ADDRESS                                        (B1_MAC_TX_HC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10021_BASE_ADDRESS                                        (B1_PRE_AGG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10022_BASE_ADDRESS                                        (B1_MAC_ADDR2INDEX_BASE_ADDR)
#define MEMORY_MAP_UNIT_10023_BASE_ADDRESS                                        (B1_TX_HANDLER_BASE_ADDR)
#define MEMORY_MAP_UNIT_10024_BASE_ADDRESS                                        (B1_MAC_HT_EXTENSIONS_BASE_ADDR)
#define MEMORY_MAP_UNIT_10025_BASE_ADDRESS                                        (B1_AUTO_RESP_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_10026_BASE_ADDRESS                                        (B1_MAC_GENRISC_TX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10027_BASE_ADDRESS                                        (B1_MAC_GENRISC_RX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10029_BASE_ADDRESS                                        (B1_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_10042_BASE_ADDRESS                                        (B1_TX_PD_ACC_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10043_BASE_ADDRESS                                        (B1_UL_POST_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_10044_BASE_ADDRESS                                        (B1_TF_GENERATOR_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10045_BASE_ADDRESS                                        (B1_BA_ANALYZER_BASE_ADDR)
#define MEMORY_MAP_UNIT_10053_BASE_ADDRESS                                        (B1_TXH_TXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10054_BASE_ADDRESS                                        (B1_TXH_NTD_BASE_ADDR)
#define MEMORY_MAP_UNIT_10056_BASE_ADDRESS                                        (B1_TWT_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10057_BASE_ADDRESS                                        (B1_RX_CLASSIFIER_BASE_ADDR)
#define MEMORY_MAP_UNIT_10058_BASE_ADDRESS                                        (B1_PS_SETTING_BASE_ADDR)
#define MEMORY_MAP_UNIT_10062_BASE_ADDRESS                                        (B1_PAC_EXTRAPOLATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_10063_BASE_ADDRESS                                        (B1_RXH_RD_ACC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10064_BASE_ADDRESS                                        (B1_TIM_IE_BASE_ADDR)
#define MEMORY_MAP_UNIT_10065_BASE_ADDRESS                                        (B1_RX_MSDU_PARSER_BASE_ADDR)
#define MEMORY_MAP_UNIT_10066_BASE_ADDRESS                                        (B1_BESTRU_BASE_ADDR)
#define MEMORY_MAP_UNIT_10068_BASE_ADDRESS                                        (B1_TX_SEQUENCER_BASE_ADDR)
#define MEMORY_MAP_UNIT_10200_BASE_ADDRESS                                        (B1_PHY_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_10201_BASE_ADDRESS                                        (B1_PHY_RX_BE_BASE_ADDR)
#define MEMORY_MAP_UNIT_10202_BASE_ADDRESS                                        (B1_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_10203_BASE_ADDRESS                                        (B1_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_10204_BASE_ADDRESS                                        (B1_UP_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_10205_BASE_ADDRESS                                        (B1_UP_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_10206_BASE_ADDRESS                                        (B1_PHY_DFS_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10207_BASE_ADDRESS                                        (B1_PHY_DFS_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_10208_BASE_ADDRESS                                        (B1_PHY_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_10209_BASE_ADDRESS                                        (B1_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR)
#define MEMORY_MAP_UNIT_10211_BASE_ADDRESS                                        (B1_PHY_RX_TD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10212_BASE_ADDRESS                                        (B1_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10213_BASE_ADDRESS                                        (B1_UP_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_10214_BASE_ADDRESS                                        (B1_PHY_DFS_RXTD_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_10215_BASE_ADDRESS                                        (B1_PHY_DFS_AGC_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_10220_BASE_ADDRESS                                        (B1_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10221_BASE_ADDRESS                                        (B1_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10222_BASE_ADDRESS                                        (B1_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10223_BASE_ADDRESS                                        (B1_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10224_BASE_ADDRESS                                        (B1_UP_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10225_BASE_ADDRESS                                        (B1_UP_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10226_BASE_ADDRESS                                        (B1_UP_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10227_BASE_ADDRESS                                        (B1_UP_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10230_BASE_ADDRESS                                        (B1_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10231_BASE_ADDRESS                                        (B1_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10232_BASE_ADDRESS                                        (B1_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10233_BASE_ADDRESS                                        (B1_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10234_BASE_ADDRESS                                        (B1_UP_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10235_BASE_ADDRESS                                        (B1_UP_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10236_BASE_ADDRESS                                        (B1_UP_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10237_BASE_ADDRESS                                        (B1_UP_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10240_BASE_ADDRESS                                        (B1_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10241_BASE_ADDRESS                                        (B1_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10242_BASE_ADDRESS                                        (B1_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10243_BASE_ADDRESS                                        (B1_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10244_BASE_ADDRESS                                        (B1_UP_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_10245_BASE_ADDRESS                                        (B1_UP_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_10246_BASE_ADDRESS                                        (B1_UP_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_10247_BASE_ADDRESS                                        (B1_UP_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_10500_BASE_ADDRESS                                        (B1_HOB_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_10501_BASE_ADDRESS                                        (B1_HOB_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_10502_BASE_ADDRESS                                        (B1_HOB_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_10503_BASE_ADDRESS                                        (B1_HOB_PAC_DEL_BASE_ADDR)
#define MEMORY_MAP_UNIT_10504_BASE_ADDRESS                                        (B1_HOB_AUTO_RESP_BASE_ADDR)
#define MEMORY_MAP_UNIT_10505_BASE_ADDRESS                                        (B1_HOB_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_11020_BASE_ADDRESS                                        (B1_RX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_11021_BASE_ADDRESS                                        (B1_RX_PHY_STATUS_BASE_ADDR)
#define MEMORY_MAP_UNIT_11022_BASE_ADDRESS                                        (B1_TX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_11023_BASE_ADDRESS                                        (B1_TX_RECIPES_BASE_ADDR)
#define MEMORY_MAP_UNIT_11024_BASE_ADDRESS                                        (B1_MAC_GENRISC_TX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11026_BASE_ADDRESS                                        (B1_MAC_GENRISC_TX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11027_BASE_ADDRESS                                        (B1_TWT_GROUP_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11029_BASE_ADDRESS                                        (B1_MAC_GENRISC_TX_STD_BASE_ADDR)
#define MEMORY_MAP_UNIT_11031_BASE_ADDRESS                                        (B1_MAC_GENRISC_RX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11033_BASE_ADDRESS                                        (B1_MAC_GENRISC_RX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11041_BASE_ADDRESS                                        (B1_TF_DECODER_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11048_BASE_ADDRESS                                        (B1_TWT_STATION_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11052_BASE_ADDRESS                                        (B1_RXF_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11053_BASE_ADDRESS                                        (B1_AUTO_RESP_MEMS_BASE_ADDR)
#define MEMORY_MAP_UNIT_11055_BASE_ADDRESS                                        (B1_DUR_AUTO_RATE_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11058_BASE_ADDRESS                                        (B1_FRAME_CLASS_VIO_BUFFER_BASE_ADDR)
#define MEMORY_MAP_UNIT_11069_BASE_ADDRESS                                        (B1_PAC_DELIA_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11073_BASE_ADDRESS                                        (B1_OTFA_MEM2_BASE_ADDR)
#define MEMORY_MAP_UNIT_11077_BASE_ADDRESS                                        (B1_MAC_SEC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11085_BASE_ADDRESS                                        (B1_TF_GENERATOR_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11088_BASE_ADDRESS                                        (B1_MPDU_DESC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11097_BASE_ADDRESS                                        (B1_TX_PD_ACC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_11110_BASE_ADDRESS                                        (B1_MLD_DB_BASE_ADDR)
#define MEMORY_MAP_UNIT_11130_BASE_ADDRESS                                        (B1_TX_SENDER_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_11131_BASE_ADDRESS                                        (B1_RX_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_11132_BASE_ADDRESS                                        (B1_TX_SENDER_SINGLE_UC_BASE_ADDR)
#define MEMORY_MAP_UNIT_11133_BASE_ADDRESS                                        (B1_RX_SINGLE_UC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20006_BASE_ADDRESS                                        (B2_OTFA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20007_BASE_ADDRESS                                        (B2_RX_COORDINATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_20008_BASE_ADDRESS                                        (B2_PAC_DELIA_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20009_BASE_ADDRESS                                        (B2_PAC_RXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20010_BASE_ADDRESS                                        (B2_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_20011_BASE_ADDRESS                                        (B2_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_20012_BASE_ADDRESS                                        (B2_BUF_STATUS_REPORT_BASE_ADDR)
#define MEMORY_MAP_UNIT_20013_BASE_ADDRESS                                        (B2_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_20014_BASE_ADDRESS                                        (B2_TF_DECODER_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20015_BASE_ADDRESS                                        (B2_MAC_BEACON_BASE_ADDR)
#define MEMORY_MAP_UNIT_20016_BASE_ADDRESS                                        (B2_BSRC_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_20017_BASE_ADDRESS                                        (B2_MAC_WEP_BASE_ADDR)
#define MEMORY_MAP_UNIT_20018_BASE_ADDRESS                                        (B2_RAB_BASE_ADDR)
#define MEMORY_MAP_UNIT_20020_BASE_ADDRESS                                        (B2_MAC_TX_HC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20021_BASE_ADDRESS                                        (B2_PRE_AGG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20022_BASE_ADDRESS                                        (B2_MAC_ADDR2INDEX_BASE_ADDR)
#define MEMORY_MAP_UNIT_20023_BASE_ADDRESS                                        (B2_TX_HANDLER_BASE_ADDR)
#define MEMORY_MAP_UNIT_20024_BASE_ADDRESS                                        (B2_MAC_HT_EXTENSIONS_BASE_ADDR)
#define MEMORY_MAP_UNIT_20025_BASE_ADDRESS                                        (B2_AUTO_RESP_REGS_BASE_ADDR)
#define MEMORY_MAP_UNIT_20026_BASE_ADDRESS                                        (B2_MAC_GENRISC_TX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20027_BASE_ADDRESS                                        (B2_MAC_GENRISC_RX_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20029_BASE_ADDRESS                                        (B2_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_20042_BASE_ADDRESS                                        (B2_TX_PD_ACC_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20043_BASE_ADDRESS                                        (B2_UL_POST_RX_BASE_ADDR)
#define MEMORY_MAP_UNIT_20044_BASE_ADDRESS                                        (B2_TF_GENERATOR_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20045_BASE_ADDRESS                                        (B2_BA_ANALYZER_BASE_ADDR)
#define MEMORY_MAP_UNIT_20053_BASE_ADDRESS                                        (B2_TXH_TXC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20054_BASE_ADDRESS                                        (B2_TXH_NTD_BASE_ADDR)
#define MEMORY_MAP_UNIT_20056_BASE_ADDRESS                                        (B2_TWT_REG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20057_BASE_ADDRESS                                        (B2_RX_CLASSIFIER_BASE_ADDR)
#define MEMORY_MAP_UNIT_20058_BASE_ADDRESS                                        (B2_PS_SETTING_BASE_ADDR)
#define MEMORY_MAP_UNIT_20062_BASE_ADDRESS                                        (B2_PAC_EXTRAPOLATOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_20063_BASE_ADDRESS                                        (B2_RXH_RD_ACC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20064_BASE_ADDRESS                                        (B2_TIM_IE_BASE_ADDR)
#define MEMORY_MAP_UNIT_20065_BASE_ADDRESS                                        (B2_RX_MSDU_PARSER_BASE_ADDR)
#define MEMORY_MAP_UNIT_20066_BASE_ADDRESS                                        (B2_BESTRU_BASE_ADDR)
#define MEMORY_MAP_UNIT_20068_BASE_ADDRESS                                        (B2_TX_SEQUENCER_BASE_ADDR)
#define MEMORY_MAP_UNIT_20200_BASE_ADDRESS                                        (B2_PHY_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_20201_BASE_ADDRESS                                        (B2_PHY_RX_BE_BASE_ADDR)
#define MEMORY_MAP_UNIT_20202_BASE_ADDRESS                                        (B2_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_20203_BASE_ADDRESS                                        (B2_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_20204_BASE_ADDRESS                                        (B2_UP_PHY_RX_FD_BASE_ADDR)
#define MEMORY_MAP_UNIT_20205_BASE_ADDRESS                                        (B2_UP_PHY_TX_BASE_ADDR)
#define MEMORY_MAP_UNIT_20206_BASE_ADDRESS                                        (B2_PHY_DFS_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20207_BASE_ADDRESS                                        (B2_PHY_DFS_RX_TD_BASE_ADDR)
#define MEMORY_MAP_UNIT_20208_BASE_ADDRESS                                        (B2_PHY_RISC_BASE_ADDR)
#define MEMORY_MAP_UNIT_20209_BASE_ADDRESS                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR)
#define MEMORY_MAP_UNIT_20211_BASE_ADDRESS                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20212_BASE_ADDRESS                                        (B2_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20213_BASE_ADDRESS                                        (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR)
#define MEMORY_MAP_UNIT_20214_BASE_ADDRESS                                        (B2_PHY_DFS_RXTD_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_20215_BASE_ADDRESS                                        (B2_PHY_DFS_AGC_ANT_BASE_ADDR)
#define MEMORY_MAP_UNIT_20220_BASE_ADDRESS                                        (B2_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20221_BASE_ADDRESS                                        (B2_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20222_BASE_ADDRESS                                        (B2_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20223_BASE_ADDRESS                                        (B2_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20224_BASE_ADDRESS                                        (B2_UP_PHY_AGC_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20225_BASE_ADDRESS                                        (B2_UP_PHY_AGC_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20226_BASE_ADDRESS                                        (B2_UP_PHY_AGC_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20227_BASE_ADDRESS                                        (B2_UP_PHY_AGC_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20230_BASE_ADDRESS                                        (B2_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20231_BASE_ADDRESS                                        (B2_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20232_BASE_ADDRESS                                        (B2_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20233_BASE_ADDRESS                                        (B2_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20234_BASE_ADDRESS                                        (B2_UP_PHY_TXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20235_BASE_ADDRESS                                        (B2_UP_PHY_TXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20236_BASE_ADDRESS                                        (B2_UP_PHY_TXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20237_BASE_ADDRESS                                        (B2_UP_PHY_TXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20240_BASE_ADDRESS                                        (B2_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20241_BASE_ADDRESS                                        (B2_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20242_BASE_ADDRESS                                        (B2_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20243_BASE_ADDRESS                                        (B2_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20244_BASE_ADDRESS                                        (B2_UP_PHY_RXTD_ANT0_BASE_ADDR)
#define MEMORY_MAP_UNIT_20245_BASE_ADDRESS                                        (B2_UP_PHY_RXTD_ANT1_BASE_ADDR)
#define MEMORY_MAP_UNIT_20246_BASE_ADDRESS                                        (B2_UP_PHY_RXTD_ANT2_BASE_ADDR)
#define MEMORY_MAP_UNIT_20247_BASE_ADDRESS                                        (B2_UP_PHY_RXTD_ANT3_BASE_ADDR)
#define MEMORY_MAP_UNIT_20500_BASE_ADDRESS                                        (B2_HOB_PAC_DUR_BASE_ADDR)
#define MEMORY_MAP_UNIT_20501_BASE_ADDRESS                                        (B2_HOB_MAC_BFM_BASE_ADDR)
#define MEMORY_MAP_UNIT_20502_BASE_ADDRESS                                        (B2_HOB_PAC_TIM_BASE_ADDR)
#define MEMORY_MAP_UNIT_20503_BASE_ADDRESS                                        (B2_HOB_PAC_DEL_BASE_ADDR)
#define MEMORY_MAP_UNIT_20504_BASE_ADDRESS                                        (B2_HOB_AUTO_RESP_BASE_ADDR)
#define MEMORY_MAP_UNIT_20505_BASE_ADDRESS                                        (B2_HOB_TX_SELECTOR_BASE_ADDR)
#define MEMORY_MAP_UNIT_21020_BASE_ADDRESS                                        (B2_RX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_21021_BASE_ADDRESS                                        (B2_RX_PHY_STATUS_BASE_ADDR)
#define MEMORY_MAP_UNIT_21022_BASE_ADDRESS                                        (B2_TX_CIRCULAR_BASE_ADDR)
#define MEMORY_MAP_UNIT_21023_BASE_ADDRESS                                        (B2_TX_RECIPES_BASE_ADDR)
#define MEMORY_MAP_UNIT_21024_BASE_ADDRESS                                        (B2_MAC_GENRISC_TX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21026_BASE_ADDRESS                                        (B2_MAC_GENRISC_TX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21027_BASE_ADDRESS                                        (B2_TWT_GROUP_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21029_BASE_ADDRESS                                        (B2_MAC_GENRISC_TX_STD_BASE_ADDR)
#define MEMORY_MAP_UNIT_21031_BASE_ADDRESS                                        (B2_MAC_GENRISC_RX_IRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21033_BASE_ADDRESS                                        (B2_MAC_GENRISC_RX_SPRAM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21041_BASE_ADDRESS                                        (B2_TF_DECODER_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21048_BASE_ADDRESS                                        (B2_TWT_STATION_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21052_BASE_ADDRESS                                        (B2_RXF_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21053_BASE_ADDRESS                                        (B2_AUTO_RESP_MEMS_BASE_ADDR)
#define MEMORY_MAP_UNIT_21055_BASE_ADDRESS                                        (B2_DUR_AUTO_RATE_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21058_BASE_ADDRESS                                        (B2_FRAME_CLASS_VIO_BUFFER_BASE_ADDR)
#define MEMORY_MAP_UNIT_21069_BASE_ADDRESS                                        (B2_PAC_DELIA_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21073_BASE_ADDRESS                                        (B2_OTFA_MEM2_BASE_ADDR)
#define MEMORY_MAP_UNIT_21077_BASE_ADDRESS                                        (B2_MAC_SEC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21085_BASE_ADDRESS                                        (B2_TF_GENERATOR_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21088_BASE_ADDRESS                                        (B2_MPDU_DESC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21097_BASE_ADDRESS                                        (B2_TX_PD_ACC_MEM_BASE_ADDR)
#define MEMORY_MAP_UNIT_21110_BASE_ADDRESS                                        (B2_MLD_DB_BASE_ADDR)
#define MEMORY_MAP_UNIT_21130_BASE_ADDRESS                                        (B2_TX_SENDER_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_21131_BASE_ADDRESS                                        (B2_RX_UC_CLUSTER_BASE_ADDR)
#define MEMORY_MAP_UNIT_21132_BASE_ADDRESS                                        (B2_TX_SENDER_SINGLE_UC_BASE_ADDR)
#define MEMORY_MAP_UNIT_21133_BASE_ADDRESS                                        (B2_RX_SINGLE_UC_BASE_ADDR)

/* Base Addresses From ARC Persepective*/
#define CPU_IRAM_BASE_ADDR                                                        (0xA0000000)
#define BOOT_ROM_BASE_ADDR                                                        (0xA05F0000)
#define SHARED_RAM_BASE_ADDR                                                      (0xA0600000)
#define DESCRIPTOR_RAM_RX_BASE_ADDR                                               (0xA0800000)
#define DESCRIPTOR_RAM_TX_BASE_ADDR                                               (0xA0900000)
#define DESCRIPTOR_RAM_RX_TX_BASE_ADDR                                            (0xA0A00000)
#define PLAN_RAM_BASE_ADDR                                                        (0xA0B00000)
#define BA_ANALYZER_COUNTER_M01_BASE_ADDR                                         (0xA0E00000)
#define BA_ANALYZER_COUNTER_BASE_ADDR                                             (0xA0E00000)
#define BA_ANALYZER_COUNTER_M2_BASE_ADDR                                          (0xA0E20000)
#define RX_PP_COUNTERS_M01_BASE_ADDR                                              (0xA0E40000)
#define RX_PP_COUNTERS_BASE_ADDR                                                  (0xA0E40000)
#define RX_PP_COUNTERS_M2_BASE_ADDR                                               (0xA0E50000)
#define RX_COORDINATOR_COUNTERS_M01_BASE_ADDR                                     (0xA0E60000)
#define RX_COORDINATOR_COUNTERS_BASE_ADDR                                         (0xA0E60000)
#define RX_COORDINATOR_COUNTERS_M2_BASE_ADDR                                      (0xA0E70000)
#define HOST_IF_COUNTERS_M01_BASE_ADDR                                            (0xA0E80000)
#define HOST_IF_COUNTERS_BASE_ADDR                                                (0xA0E80000)
#define HOST_IF_COUNTERS_M2_BASE_ADDR                                             (0xA0E90000)
#define HOST_IF_QOS_COUNTERS_M01_BASE_ADDR                                        (0xA0EA0000)
#define HOST_IF_QOS_COUNTERS_BASE_ADDR                                            (0xA0EA0000)
#define HOST_IF_QOS_COUNTERS_M2_BASE_ADDR                                         (0xA0EB0000)
#define HOST_IF_AQM_STAT_BASE_ADDR                                                (0xA0EC0000)
#define EXT_RX_PP_COUNTERS_BASE_ADDR                                              (0xA0EE0000)
#define SYSTEM_IF_LOWER_B0_CPU_BASE_ADDR                                          (0xA0F00000)
#define SYSTEM_IF_LOWER_B1_CPU_BASE_ADDR                                          (0xA0F10000)
#define SYSTEM_IF_LOWER_B2_CPU_BASE_ADDR                                          (0xA0F20000)
#define SYSTEM_IF_UPPER_CPU_BASE_ADDR                                             (0xA0F30000)
#define WLAN_DMA_LL_MEM_M01_BASE_ADDR                                             (0xA0F40000)
#define WLAN_DMA_LL_MEM_BASE_ADDR                                                 (0xA0F40000)
#define WLAN_DMA_TX_M01_BASE_ADDR                                                 (0xA0F45000)
#define WLAN_DMA_TX_BASE_ADDR                                                     (0xA0F45000)
#define WLAN_DMA_RX_M01_BASE_ADDR                                                 (0xA0F45800)
#define WLAN_DMA_RX_BASE_ADDR                                                     (0xA0F45800)
#define WLAN_DMA_FW_M01_BASE_ADDR                                                 (0xA0F46000)
#define WLAN_DMA_FW_BASE_ADDR                                                     (0xA0F46000)
#define DMA_WRAPPER_M01_BASE_ADDR                                                 (0xA0F47000)
#define DMA_WRAPPER_BASE_ADDR                                                     (0xA0F47000)
#define WLAN_DMA_LL_MEM_M2_BASE_ADDR                                              (0xA0F48000)
#define WLAN_DMA_TX_M2_BASE_ADDR                                                  (0xA0F4D000)
#define WLAN_DMA_RX_M2_BASE_ADDR                                                  (0xA0F4D800)
#define WLAN_DMA_FW_M2_BASE_ADDR                                                  (0xA0F4E000)
#define DMA_WRAPPER_M2_BASE_ADDR                                                  (0xA0F4F000)
#define DLM_M01_BASE_ADDR                                                         (0xA0F60000)
#define DLM_BASE_ADDR                                                             (0xA0F60000)
#define QUEUE_MANAGER_M01_TX0_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_TX0_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_TX1_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_TX1_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_TX2_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_TX2_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_RX0_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_RX0_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_RX1_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_RX1_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_RX2_BASE_ADDR                                           (0xA0F6C000)
#define QUEUE_MANAGER_RX2_BASE_ADDR                                               (0xA0F6C000)
#define QUEUE_MANAGER_M01_HOST_BASE_ADDR                                          (0xA0F6C000)
#define QUEUE_MANAGER_HOST_BASE_ADDR                                              (0xA0F6C000)
#define QUEUE_MANAGER_M01_LCPU0_BASE_ADDR                                         (0xA0F6C000)
#define QUEUE_MANAGER_LCPU0_BASE_ADDR                                             (0xA0F6C000)
#define QUEUE_MANAGER_M01_LCPU1_BASE_ADDR                                         (0xA0F6C000)
#define QUEUE_MANAGER_LCPU1_BASE_ADDR                                             (0xA0F6C000)
#define QUEUE_MANAGER_M01_LCPU2_BASE_ADDR                                         (0xA0F6C000)
#define QUEUE_MANAGER_LCPU2_BASE_ADDR                                             (0xA0F6C000)
#define QUEUE_MANAGER_M01_UCPU_BASE_ADDR                                          (0xA0F6C000)
#define QUEUE_MANAGER_UCPU_BASE_ADDR                                              (0xA0F6C000)
#define DLM_M2_BASE_ADDR                                                          (0xA0F70000)
#define QUEUE_MANAGER_M2_TX0_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_TX1_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_TX2_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_RX0_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_RX1_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_RX2_BASE_ADDR                                            (0xA0F7C000)
#define QUEUE_MANAGER_M2_HOST_BASE_ADDR                                           (0xA0F7C000)
#define QUEUE_MANAGER_M2_LCPU0_BASE_ADDR                                          (0xA0F7C000)
#define QUEUE_MANAGER_M2_LCPU1_BASE_ADDR                                          (0xA0F7C000)
#define QUEUE_MANAGER_M2_LCPU2_BASE_ADDR                                          (0xA0F7C000)
#define QUEUE_MANAGER_M2_UCPU_BASE_ADDR                                           (0xA0F7C000)
#define TOP_REGBLOCK_BASE_ADDR                                                    (0xA0F80000)
#define PCIE_PHY0_REGBLOCK_BASE_ADDR                                              (0xA0F88000)
#define PCIE_DBI0_REGBLOCK_BASE_ADDR                                              (0xA0F8B000)
#define PVT_BLOCK_BASE_ADDR                                                       (0xA0F90000)
#define CPU_XBAR_STATUS_BASE_ADDR                                                 (0xA0FA0000)
#define MAC_XBAR_PM_BASE_ADDR                                                     (0xA0FA2000)
#define MAC_XBAR_STATUS_BASE_ADDR                                                 (0xA0FA3000)
#define RX_PP_M01_BASE_ADDR                                                       (0xA0FC0000)
#define RX_PP_BASE_ADDR                                                           (0xA0FC0000)
#define RX_PP_REGS_M01_BASE_ADDR                                                  (0xA0FCF800)
#define RX_PP_REGS_BASE_ADDR                                                      (0xA0FCF800)
#define RX_PP_M2_BASE_ADDR                                                        (0xA0FD0000)
#define RX_PP_REGS_M2_BASE_ADDR                                                   (0xA0FDF800)
#define DATA_LOGGER_BASE_ADDR                                                     (0xA0FE0000)
#define PROT_DB_BASE_ADDR                                                         (0xA1000000)
#define PROT_DB_SHADOW_MEM_BASE_ADDR                                              (0xA1000200)
#define TRAFFIC_INDICATOR_M01_BASE_ADDR                                           (0xA1002000)
#define TRAFFIC_INDICATOR_BASE_ADDR                                               (0xA1002000)
#define TRAFFIC_INDICATOR_M2_BASE_ADDR                                            (0xA1002800)
#define VAP_ADDR_REG_M01_BASE_ADDR                                                (0xA1003000)
#define VAP_ADDR_REG_BASE_ADDR                                                    (0xA1003000)
#define VAP_ADDR_REG_M2_BASE_ADDR                                                 (0xA1003400)
#define MAC_GEN_REGFILE_BASE_ADDR                                                 (0xA1004000)
#define TIM_GEN_BASE_ADDR                                                         (0xA1005000)
#define HOST_IF_ACC_BASE_ADDR                                                     (0xA1008000)
#define HOST_IF_REG_FILE_BASE_ADDR                                                (0xA1009000)
#define BSRC_CNT_MEM_M01_BASE_ADDR                                                (0xA1088000)
#define BSRC_CNT_MEM_BASE_ADDR                                                    (0xA1088000)
#define BSRC_CNT_MEM_M2_BASE_ADDR                                                 (0xA108A000)
#define BSRC_AGER_MEM_M01_BASE_ADDR                                               (0xA108C000)
#define BSRC_AGER_MEM_BASE_ADDR                                                   (0xA108C000)
#define BSRC_AGER_MEM_M2_BASE_ADDR                                                (0xA108D000)
#define BEST_RU_DB_M01_BASE_ADDR                                                  (0xA1090000)
#define BEST_RU_DB_BASE_ADDR                                                      (0xA1090000)
#define BEST_RU_DB_M2_BASE_ADDR                                                   (0xA1098000)
#define RETRY_STATE_DB_M01_BASE_ADDR                                              (0xA10A8000)
#define RETRY_STATE_DB_BASE_ADDR                                                  (0xA10A8000)
#define RETRY_STATE_DB_M2_BASE_ADDR                                               (0xA10AC000)
#define RETRY_POINTER_DB_M01_BASE_ADDR                                            (0xA10C0000)
#define RETRY_POINTER_DB_BASE_ADDR                                                (0xA10C0000)
#define RETRY_POINTER_DB_M2_BASE_ADDR                                             (0xA10E0000)
#define MAC_HOSTIF_UC_INTERNAL_BASE_ADDR                                          (0xA1200000)
#define MAC_HOSTIF_UC_CLUSTER_BASE_ADDR                                           (0xA1220000)
#define B0_MAC_GENRISC_TX_IRAM_BASE_ADDR                                          (0xA1400000)
#define MAC_GENRISC_TX_IRAM_BASE_ADDR                                             (0xA1400000)
#define B1_MAC_GENRISC_TX_IRAM_BASE_ADDR                                          (0xA1800000)
#define B2_MAC_GENRISC_TX_IRAM_BASE_ADDR                                          (0xA1C00000)
#define B0_MAC_GENRISC_TX_SPRAM_BASE_ADDR                                         (0xA1410000)
#define MAC_GENRISC_TX_SPRAM_BASE_ADDR                                            (0xA1410000)
#define B1_MAC_GENRISC_TX_SPRAM_BASE_ADDR                                         (0xA1810000)
#define B2_MAC_GENRISC_TX_SPRAM_BASE_ADDR                                         (0xA1C10000)
#define B0_MAC_GENRISC_TX_REG_BASE_ADDR                                           (0xA1411000)
#define MAC_GENRISC_TX_REG_BASE_ADDR                                              (0xA1411000)
#define B1_MAC_GENRISC_TX_REG_BASE_ADDR                                           (0xA1811000)
#define B2_MAC_GENRISC_TX_REG_BASE_ADDR                                           (0xA1C11000)
#define B0_MAC_GENRISC_TX_STD_BASE_ADDR                                           (0xA1412000)
#define MAC_GENRISC_TX_STD_BASE_ADDR                                              (0xA1412000)
#define B1_MAC_GENRISC_TX_STD_BASE_ADDR                                           (0xA1812000)
#define B2_MAC_GENRISC_TX_STD_BASE_ADDR                                           (0xA1C12000)
#define B0_MAC_GENRISC_RX_IRAM_BASE_ADDR                                          (0xA1420000)
#define MAC_GENRISC_RX_IRAM_BASE_ADDR                                             (0xA1420000)
#define B1_MAC_GENRISC_RX_IRAM_BASE_ADDR                                          (0xA1820000)
#define B2_MAC_GENRISC_RX_IRAM_BASE_ADDR                                          (0xA1C20000)
#define B0_MAC_GENRISC_RX_SPRAM_BASE_ADDR                                         (0xA142F000)
#define MAC_GENRISC_RX_SPRAM_BASE_ADDR                                            (0xA142F000)
#define B1_MAC_GENRISC_RX_SPRAM_BASE_ADDR                                         (0xA182F000)
#define B2_MAC_GENRISC_RX_SPRAM_BASE_ADDR                                         (0xA1C2F000)
#define B0_MAC_GENRISC_RX_REG_BASE_ADDR                                           (0xA1430000)
#define MAC_GENRISC_RX_REG_BASE_ADDR                                              (0xA1430000)
#define B1_MAC_GENRISC_RX_REG_BASE_ADDR                                           (0xA1830000)
#define B2_MAC_GENRISC_RX_REG_BASE_ADDR                                           (0xA1C30000)
#define B0_TX_SENDER_UC_CLUSTER_BASE_ADDR                                         (0xA1438000)
#define TX_SENDER_UC_CLUSTER_BASE_ADDR                                            (0xA1438000)
#define B1_TX_SENDER_UC_CLUSTER_BASE_ADDR                                         (0xA1838000)
#define B2_TX_SENDER_UC_CLUSTER_BASE_ADDR                                         (0xA1C38000)
#define B0_RX_UC_CLUSTER_BASE_ADDR                                                (0xA143C000)
#define RX_UC_CLUSTER_BASE_ADDR                                                   (0xA143C000)
#define B1_RX_UC_CLUSTER_BASE_ADDR                                                (0xA183C000)
#define B2_RX_UC_CLUSTER_BASE_ADDR                                                (0xA1C3C000)
#define B0_TX_SENDER_SINGLE_UC_BASE_ADDR                                          (0xA1440000)
#define TX_SENDER_SINGLE_UC_BASE_ADDR                                             (0xA1440000)
#define B1_TX_SENDER_SINGLE_UC_BASE_ADDR                                          (0xA1840000)
#define B2_TX_SENDER_SINGLE_UC_BASE_ADDR                                          (0xA1C40000)
#define B0_RX_SINGLE_UC_BASE_ADDR                                                 (0xA1460000)
#define RX_SINGLE_UC_BASE_ADDR                                                    (0xA1460000)
#define B1_RX_SINGLE_UC_BASE_ADDR                                                 (0xA1860000)
#define B2_RX_SINGLE_UC_BASE_ADDR                                                 (0xA1C60000)
#define B0_MAC_HT_EXTENSIONS_BASE_ADDR                                            (0xA1481000)
#define MAC_HT_EXTENSIONS_BASE_ADDR                                               (0xA1481000)
#define B1_MAC_HT_EXTENSIONS_BASE_ADDR                                            (0xA1881000)
#define B2_MAC_HT_EXTENSIONS_BASE_ADDR                                            (0xA1C81000)
#define B0_RXH_RD_ACC_BASE_ADDR                                                   (0xA1482000)
#define RXH_RD_ACC_BASE_ADDR                                                      (0xA1482000)
#define B1_RXH_RD_ACC_BASE_ADDR                                                   (0xA1882000)
#define B2_RXH_RD_ACC_BASE_ADDR                                                   (0xA1C82000)
#define B0_AUTO_RESP_MEMS_BASE_ADDR                                               (0xA1488000)
#define AUTO_RESP_MEMS_BASE_ADDR                                                  (0xA1488000)
#define B1_AUTO_RESP_MEMS_BASE_ADDR                                               (0xA1888000)
#define B2_AUTO_RESP_MEMS_BASE_ADDR                                               (0xA1C88000)
#define B0_AUTO_RESP_REGS_BASE_ADDR                                               (0xA148B000)
#define AUTO_RESP_REGS_BASE_ADDR                                                  (0xA148B000)
#define B1_AUTO_RESP_REGS_BASE_ADDR                                               (0xA188B000)
#define B2_AUTO_RESP_REGS_BASE_ADDR                                               (0xA1C8B000)
#define B0_PAC_DUR_BASE_ADDR                                                      (0xA1500400)
#define PAC_DUR_BASE_ADDR                                                         (0xA1500400)
#define B1_PAC_DUR_BASE_ADDR                                                      (0xA1900400)
#define B2_PAC_DUR_BASE_ADDR                                                      (0xA1D00400)
#define B0_PAC_TIM_BASE_ADDR                                                      (0xA1500500)
#define PAC_TIM_BASE_ADDR                                                         (0xA1500500)
#define B1_PAC_TIM_BASE_ADDR                                                      (0xA1900500)
#define B2_PAC_TIM_BASE_ADDR                                                      (0xA1D00500)
#define B0_PAC_EXTRAPOLATOR_BASE_ADDR                                             (0xA1500600)
#define PAC_EXTRAPOLATOR_BASE_ADDR                                                (0xA1500600)
#define B1_PAC_EXTRAPOLATOR_BASE_ADDR                                             (0xA1900600)
#define B2_PAC_EXTRAPOLATOR_BASE_ADDR                                             (0xA1D00600)
#define B0_MAC_BEACON_BASE_ADDR                                                   (0xA1500800)
#define MAC_BEACON_BASE_ADDR                                                      (0xA1500800)
#define B1_MAC_BEACON_BASE_ADDR                                                   (0xA1900800)
#define B2_MAC_BEACON_BASE_ADDR                                                   (0xA1D00800)
#define B0_TXH_TXC_BASE_ADDR                                                      (0xA1500E00)
#define TXH_TXC_BASE_ADDR                                                         (0xA1500E00)
#define B1_TXH_TXC_BASE_ADDR                                                      (0xA1900E00)
#define B2_TXH_TXC_BASE_ADDR                                                      (0xA1D00E00)
#define B0_TXH_NTD_BASE_ADDR                                                      (0xA1500F00)
#define TXH_NTD_BASE_ADDR                                                         (0xA1500F00)
#define B1_TXH_NTD_BASE_ADDR                                                      (0xA1900F00)
#define B2_TXH_NTD_BASE_ADDR                                                      (0xA1D00F00)
#define B0_PS_SETTING_BASE_ADDR                                                   (0xA1501000)
#define PS_SETTING_BASE_ADDR                                                      (0xA1501000)
#define B1_PS_SETTING_BASE_ADDR                                                   (0xA1901000)
#define B2_PS_SETTING_BASE_ADDR                                                   (0xA1D01000)
#define B0_PAC_DELIA_REG_BASE_ADDR                                                (0xA1501C00)
#define PAC_DELIA_REG_BASE_ADDR                                                   (0xA1501C00)
#define B1_PAC_DELIA_REG_BASE_ADDR                                                (0xA1901C00)
#define B2_PAC_DELIA_REG_BASE_ADDR                                                (0xA1D01C00)
#define B0_PAC_DELIA_MEM_BASE_ADDR                                                (0xA1501E00)
#define PAC_DELIA_MEM_BASE_ADDR                                                   (0xA1501E00)
#define B1_PAC_DELIA_MEM_BASE_ADDR                                                (0xA1901E00)
#define B2_PAC_DELIA_MEM_BASE_ADDR                                                (0xA1D01E00)
#define B0_DUR_AUTO_RATE_MEM_BASE_ADDR                                            (0xA1503800)
#define DUR_AUTO_RATE_MEM_BASE_ADDR                                               (0xA1503800)
#define B1_DUR_AUTO_RATE_MEM_BASE_ADDR                                            (0xA1903800)
#define B2_DUR_AUTO_RATE_MEM_BASE_ADDR                                            (0xA1D03800)
#define B0_PAC_RXC_BASE_ADDR                                                      (0xA1504000)
#define PAC_RXC_BASE_ADDR                                                         (0xA1504000)
#define B1_PAC_RXC_BASE_ADDR                                                      (0xA1904000)
#define B2_PAC_RXC_BASE_ADDR                                                      (0xA1D04000)
#define B0_RXF_MEM_BASE_ADDR                                                      (0xA1504400)
#define RXF_MEM_BASE_ADDR                                                         (0xA1504400)
#define B1_RXF_MEM_BASE_ADDR                                                      (0xA1904400)
#define B2_RXF_MEM_BASE_ADDR                                                      (0xA1D04400)
#define B0_MAC_SEC_MEM_BASE_ADDR                                                  (0xA1510000)
#define MAC_SEC_MEM_BASE_ADDR                                                     (0xA1510000)
#define B1_MAC_SEC_MEM_BASE_ADDR                                                  (0xA1910000)
#define B2_MAC_SEC_MEM_BASE_ADDR                                                  (0xA1D10000)
#define B0_MAC_WEP_BASE_ADDR                                                      (0xA1518000)
#define MAC_WEP_BASE_ADDR                                                         (0xA1518000)
#define B1_MAC_WEP_BASE_ADDR                                                      (0xA1918000)
#define B2_MAC_WEP_BASE_ADDR                                                      (0xA1D18000)
#define B0_PRE_AGG_BASE_ADDR                                                      (0xA1520000)
#define PRE_AGG_BASE_ADDR                                                         (0xA1520000)
#define B1_PRE_AGG_BASE_ADDR                                                      (0xA1920000)
#define B2_PRE_AGG_BASE_ADDR                                                      (0xA1D20000)
#define B0_MLD_DB_BASE_ADDR                                                       (0xA152C000)
#define MLD_DB_BASE_ADDR                                                          (0xA152C000)
#define B1_MLD_DB_BASE_ADDR                                                       (0xA192C000)
#define B2_MLD_DB_BASE_ADDR                                                       (0xA1D2C000)
#define B0_TX_SEQUENCER_BASE_ADDR                                                 (0xA152E000)
#define TX_SEQUENCER_BASE_ADDR                                                    (0xA152E000)
#define B1_TX_SEQUENCER_BASE_ADDR                                                 (0xA192E000)
#define B2_TX_SEQUENCER_BASE_ADDR                                                 (0xA1D2E000)
#define B0_TX_PD_ACC_MEM_BASE_ADDR                                                (0xA152E800)
#define TX_PD_ACC_MEM_BASE_ADDR                                                   (0xA152E800)
#define B1_TX_PD_ACC_MEM_BASE_ADDR                                                (0xA192E800)
#define B2_TX_PD_ACC_MEM_BASE_ADDR                                                (0xA1D2E800)
#define B0_TX_PD_ACC_REG_BASE_ADDR                                                (0xA152EE00)
#define TX_PD_ACC_REG_BASE_ADDR                                                   (0xA152EE00)
#define B1_TX_PD_ACC_REG_BASE_ADDR                                                (0xA192EE00)
#define B2_TX_PD_ACC_REG_BASE_ADDR                                                (0xA1D2EE00)
#define B0_TF_GENERATOR_MEM_BASE_ADDR                                             (0xA152F000)
#define TF_GENERATOR_MEM_BASE_ADDR                                                (0xA152F000)
#define B1_TF_GENERATOR_MEM_BASE_ADDR                                             (0xA192F000)
#define B2_TF_GENERATOR_MEM_BASE_ADDR                                             (0xA1D2F000)
#define B0_TF_GENERATOR_REG_BASE_ADDR                                             (0xA152F600)
#define TF_GENERATOR_REG_BASE_ADDR                                                (0xA152F600)
#define B1_TF_GENERATOR_REG_BASE_ADDR                                             (0xA192F600)
#define B2_TF_GENERATOR_REG_BASE_ADDR                                             (0xA1D2F600)
#define B0_TF_DECODER_REG_BASE_ADDR                                               (0xA152F800)
#define TF_DECODER_REG_BASE_ADDR                                                  (0xA152F800)
#define B1_TF_DECODER_REG_BASE_ADDR                                               (0xA192F800)
#define B2_TF_DECODER_REG_BASE_ADDR                                               (0xA1D2F800)
#define B0_TF_DECODER_MEM_BASE_ADDR                                               (0xA152FA00)
#define TF_DECODER_MEM_BASE_ADDR                                                  (0xA152FA00)
#define B1_TF_DECODER_MEM_BASE_ADDR                                               (0xA192FA00)
#define B2_TF_DECODER_MEM_BASE_ADDR                                               (0xA1D2FA00)
#define B0_RAB_BASE_ADDR                                                          (0xA1532000)
#define RAB_BASE_ADDR                                                             (0xA1532000)
#define B1_RAB_BASE_ADDR                                                          (0xA1932000)
#define B2_RAB_BASE_ADDR                                                          (0xA1D32000)
#define B0_MAC_ADDR2INDEX_BASE_ADDR                                               (0xA1533000)
#define MAC_ADDR2INDEX_BASE_ADDR                                                  (0xA1533000)
#define B1_MAC_ADDR2INDEX_BASE_ADDR                                               (0xA1933000)
#define B2_MAC_ADDR2INDEX_BASE_ADDR                                               (0xA1D33000)
#define B0_TX_HANDLER_BASE_ADDR                                                   (0xA1534000)
#define TX_HANDLER_BASE_ADDR                                                      (0xA1534000)
#define B1_TX_HANDLER_BASE_ADDR                                                   (0xA1934000)
#define B2_TX_HANDLER_BASE_ADDR                                                   (0xA1D34000)
#define B0_BA_ANALYZER_BASE_ADDR                                                  (0xA1535000)
#define BA_ANALYZER_BASE_ADDR                                                     (0xA1535000)
#define B1_BA_ANALYZER_BASE_ADDR                                                  (0xA1935000)
#define B2_BA_ANALYZER_BASE_ADDR                                                  (0xA1D35000)
#define B0_TWT_STATION_MEM_BASE_ADDR                                              (0xA1536000)
#define TWT_STATION_MEM_BASE_ADDR                                                 (0xA1536000)
#define B1_TWT_STATION_MEM_BASE_ADDR                                              (0xA1936000)
#define B2_TWT_STATION_MEM_BASE_ADDR                                              (0xA1D36000)
#define B0_TWT_GROUP_MEM_BASE_ADDR                                                (0xA1537000)
#define TWT_GROUP_MEM_BASE_ADDR                                                   (0xA1537000)
#define B1_TWT_GROUP_MEM_BASE_ADDR                                                (0xA1937000)
#define B2_TWT_GROUP_MEM_BASE_ADDR                                                (0xA1D37000)
#define B0_TWT_REG_BASE_ADDR                                                      (0xA1537F00)
#define TWT_REG_BASE_ADDR                                                         (0xA1537F00)
#define B1_TWT_REG_BASE_ADDR                                                      (0xA1937F00)
#define B2_TWT_REG_BASE_ADDR                                                      (0xA1D37F00)
#define B0_TX_SELECTOR_BASE_ADDR                                                  (0xA1538000)
#define TX_SELECTOR_BASE_ADDR                                                     (0xA1538000)
#define B1_TX_SELECTOR_BASE_ADDR                                                  (0xA1938000)
#define B2_TX_SELECTOR_BASE_ADDR                                                  (0xA1D38000)
#define B0_RX_CLASSIFIER_BASE_ADDR                                                (0xA153B000)
#define RX_CLASSIFIER_BASE_ADDR                                                   (0xA153B000)
#define B1_RX_CLASSIFIER_BASE_ADDR                                                (0xA193B000)
#define B2_RX_CLASSIFIER_BASE_ADDR                                                (0xA1D3B000)
#define B0_TIM_IE_BASE_ADDR                                                       (0xA153C000)
#define TIM_IE_BASE_ADDR                                                          (0xA153C000)
#define B1_TIM_IE_BASE_ADDR                                                       (0xA193C000)
#define B2_TIM_IE_BASE_ADDR                                                       (0xA1D3C000)
#define B0_RX_MSDU_PARSER_BASE_ADDR                                               (0xA153E000)
#define RX_MSDU_PARSER_BASE_ADDR                                                  (0xA153E000)
#define B1_RX_MSDU_PARSER_BASE_ADDR                                               (0xA193E000)
#define B2_RX_MSDU_PARSER_BASE_ADDR                                               (0xA1D3E000)
#define B0_RX_COORDINATOR_BASE_ADDR                                               (0xA1541000)
#define RX_COORDINATOR_BASE_ADDR                                                  (0xA1541000)
#define B1_RX_COORDINATOR_BASE_ADDR                                               (0xA1941000)
#define B2_RX_COORDINATOR_BASE_ADDR                                               (0xA1D41000)
#define B0_FRAME_CLASS_VIO_BUFFER_BASE_ADDR                                       (0xA1541200)
#define FRAME_CLASS_VIO_BUFFER_BASE_ADDR                                          (0xA1541200)
#define B1_FRAME_CLASS_VIO_BUFFER_BASE_ADDR                                       (0xA1941200)
#define B2_FRAME_CLASS_VIO_BUFFER_BASE_ADDR                                       (0xA1D41200)
#define B0_OTFA_MEM2_BASE_ADDR                                                    (0xA1542000)
#define OTFA_MEM2_BASE_ADDR                                                       (0xA1542000)
#define B1_OTFA_MEM2_BASE_ADDR                                                    (0xA1942000)
#define B2_OTFA_MEM2_BASE_ADDR                                                    (0xA1D42000)
#define B0_OTFA_REG_BASE_ADDR                                                     (0xA1543800)
#define OTFA_REG_BASE_ADDR                                                        (0xA1543800)
#define B1_OTFA_REG_BASE_ADDR                                                     (0xA1943800)
#define B2_OTFA_REG_BASE_ADDR                                                     (0xA1D43800)
#define B0_MAC_TX_HC_BASE_ADDR                                                    (0xA1547800)
#define MAC_TX_HC_BASE_ADDR                                                       (0xA1547800)
#define B1_MAC_TX_HC_BASE_ADDR                                                    (0xA1947800)
#define B2_MAC_TX_HC_BASE_ADDR                                                    (0xA1D47800)
#define B0_BSRC_REGS_BASE_ADDR                                                    (0xA1547C00)
#define BSRC_REGS_BASE_ADDR                                                       (0xA1547C00)
#define B1_BSRC_REGS_BASE_ADDR                                                    (0xA1947C00)
#define B2_BSRC_REGS_BASE_ADDR                                                    (0xA1D47C00)
#define B0_MAC_BFM_BASE_ADDR                                                      (0xA1548000)
#define MAC_BFM_BASE_ADDR                                                         (0xA1548000)
#define B1_MAC_BFM_BASE_ADDR                                                      (0xA1948000)
#define B2_MAC_BFM_BASE_ADDR                                                      (0xA1D48000)
#define B0_UL_POST_RX_BASE_ADDR                                                   (0xA154A000)
#define UL_POST_RX_BASE_ADDR                                                      (0xA154A000)
#define B1_UL_POST_RX_BASE_ADDR                                                   (0xA194A000)
#define B2_UL_POST_RX_BASE_ADDR                                                   (0xA1D4A000)
#define B0_BESTRU_BASE_ADDR                                                       (0xA154B000)
#define BESTRU_BASE_ADDR                                                          (0xA154B000)
#define B1_BESTRU_BASE_ADDR                                                       (0xA194B000)
#define B2_BESTRU_BASE_ADDR                                                       (0xA1D4B000)
#define B0_BUF_STATUS_REPORT_BASE_ADDR                                            (0xA1550000)
#define BUF_STATUS_REPORT_BASE_ADDR                                               (0xA1550000)
#define B1_BUF_STATUS_REPORT_BASE_ADDR                                            (0xA1950000)
#define B2_BUF_STATUS_REPORT_BASE_ADDR                                            (0xA1D50000)
#define B0_MPDU_DESC_MEM_BASE_ADDR                                                (0xA1600000)
#define MPDU_DESC_MEM_BASE_ADDR                                                   (0xA1600000)
#define B1_MPDU_DESC_MEM_BASE_ADDR                                                (0xA1A00000)
#define B2_MPDU_DESC_MEM_BASE_ADDR                                                (0xA1E00000)
#define B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR                                    (0xA16F0000)
#define PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR                                       (0xA16F0000)
#define B1_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR                                    (0xA1AF0000)
#define B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR                                    (0xA1EF0000)
#define B0_PHY_RISC_BASE_ADDR                                                     (0xA1700000)
#define PHY_RISC_BASE_ADDR                                                        (0xA1700000)
#define B1_PHY_RISC_BASE_ADDR                                                     (0xA1B00000)
#define B2_PHY_RISC_BASE_ADDR                                                     (0xA1F00000)
#define B0_PHY_RX_TD_BASE_ADDR                                                    (0xA1710000)
#define PHY_RX_TD_BASE_ADDR                                                       (0xA1710000)
#define B1_PHY_RX_TD_BASE_ADDR                                                    (0xA1B10000)
#define B2_PHY_RX_TD_BASE_ADDR                                                    (0xA1F10000)
#define B0_PHY_RX_TD_DEBUG_BASE_ADDR                                              (0xA1714000)
#define PHY_RX_TD_DEBUG_BASE_ADDR                                                 (0xA1714000)
#define B1_PHY_RX_TD_DEBUG_BASE_ADDR                                              (0xA1B14000)
#define B2_PHY_RX_TD_DEBUG_BASE_ADDR                                              (0xA1F14000)
#define B0_PHY_RX_BE_BASE_ADDR                                                    (0xA171C000)
#define PHY_RX_BE_BASE_ADDR                                                       (0xA171C000)
#define B1_PHY_RX_BE_BASE_ADDR                                                    (0xA1B1C000)
#define B2_PHY_RX_BE_BASE_ADDR                                                    (0xA1F1C000)
#define B0_PHY_RX_FD_BASE_ADDR                                                    (0xA1720000)
#define PHY_RX_FD_BASE_ADDR                                                       (0xA1720000)
#define B1_PHY_RX_FD_BASE_ADDR                                                    (0xA1B20000)
#define B2_PHY_RX_FD_BASE_ADDR                                                    (0xA1F20000)
#define B0_PHY_RX_FD_DEBUG_BASE_ADDR                                              (0xA172C000)
#define PHY_RX_FD_DEBUG_BASE_ADDR                                                 (0xA172C000)
#define B1_PHY_RX_FD_DEBUG_BASE_ADDR                                              (0xA1B2C000)
#define B2_PHY_RX_FD_DEBUG_BASE_ADDR                                              (0xA1F2C000)
#define B0_PHY_TX_BASE_ADDR                                                       (0xA173C000)
#define PHY_TX_BASE_ADDR                                                          (0xA173C000)
#define B1_PHY_TX_BASE_ADDR                                                       (0xA1B3C000)
#define B2_PHY_TX_BASE_ADDR                                                       (0xA1F3C000)
#define B0_UP_PHY_RX_FD_BASE_ADDR                                                 (0xA1740000)
#define UP_PHY_RX_FD_BASE_ADDR                                                    (0xA1740000)
#define B1_UP_PHY_RX_FD_BASE_ADDR                                                 (0xA1B40000)
#define B2_UP_PHY_RX_FD_BASE_ADDR                                                 (0xA1F40000)
#define B0_UP_PHY_RX_FD_DEBUG_BASE_ADDR                                           (0xA174C000)
#define UP_PHY_RX_FD_DEBUG_BASE_ADDR                                              (0xA174C000)
#define B1_UP_PHY_RX_FD_DEBUG_BASE_ADDR                                           (0xA1B4C000)
#define B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR                                           (0xA1F4C000)
#define B0_UP_PHY_TX_BASE_ADDR                                                    (0xA175C000)
#define UP_PHY_TX_BASE_ADDR                                                       (0xA175C000)
#define B1_UP_PHY_TX_BASE_ADDR                                                    (0xA1B5C000)
#define B2_UP_PHY_TX_BASE_ADDR                                                    (0xA1F5C000)
#define B0_PHY_DFS_RISC_BASE_ADDR                                                 (0xA1760000)
#define PHY_DFS_RISC_BASE_ADDR                                                    (0xA1760000)
#define B1_PHY_DFS_RISC_BASE_ADDR                                                 (0xA1B60000)
#define B2_PHY_DFS_RISC_BASE_ADDR                                                 (0xA1F60000)
#define B0_PHY_DFS_RX_TD_BASE_ADDR                                                (0xA1770000)
#define PHY_DFS_RX_TD_BASE_ADDR                                                   (0xA1770000)
#define B1_PHY_DFS_RX_TD_BASE_ADDR                                                (0xA1B70000)
#define B2_PHY_DFS_RX_TD_BASE_ADDR                                                (0xA1F70000)
#define B0_PHY_DFS_RXTD_ANT_BASE_ADDR                                             (0xA1778000)
#define PHY_DFS_RXTD_ANT_BASE_ADDR                                                (0xA1778000)
#define B1_PHY_DFS_RXTD_ANT_BASE_ADDR                                             (0xA1B78000)
#define B2_PHY_DFS_RXTD_ANT_BASE_ADDR                                             (0xA1F78000)
#define B0_PHY_DFS_AGC_ANT_BASE_ADDR                                              (0xA177C000)
#define PHY_DFS_AGC_ANT_BASE_ADDR                                                 (0xA177C000)
#define B1_PHY_DFS_AGC_ANT_BASE_ADDR                                              (0xA1B7C000)
#define B2_PHY_DFS_AGC_ANT_BASE_ADDR                                              (0xA1F7C000)
#define B0_PHY_RXTD_ANT0_BASE_ADDR                                                (0xA1780000)
#define PHY_RXTD_ANT0_BASE_ADDR                                                   (0xA1780000)
#define B1_PHY_RXTD_ANT0_BASE_ADDR                                                (0xA1B80000)
#define B2_PHY_RXTD_ANT0_BASE_ADDR                                                (0xA1F80000)
#define B0_PHY_AGC_ANT0_BASE_ADDR                                                 (0xA1784000)
#define PHY_AGC_ANT0_BASE_ADDR                                                    (0xA1784000)
#define B1_PHY_AGC_ANT0_BASE_ADDR                                                 (0xA1B84000)
#define B2_PHY_AGC_ANT0_BASE_ADDR                                                 (0xA1F84000)
#define B0_PHY_TXTD_ANT0_BASE_ADDR                                                (0xA1788000)
#define PHY_TXTD_ANT0_BASE_ADDR                                                   (0xA1788000)
#define B1_PHY_TXTD_ANT0_BASE_ADDR                                                (0xA1B88000)
#define B2_PHY_TXTD_ANT0_BASE_ADDR                                                (0xA1F88000)
#define B0_PHY_RXTD_ANT1_BASE_ADDR                                                (0xA1790000)
#define PHY_RXTD_ANT1_BASE_ADDR                                                   (0xA1790000)
#define B1_PHY_RXTD_ANT1_BASE_ADDR                                                (0xA1B90000)
#define B2_PHY_RXTD_ANT1_BASE_ADDR                                                (0xA1F90000)
#define B0_PHY_AGC_ANT1_BASE_ADDR                                                 (0xA1794000)
#define PHY_AGC_ANT1_BASE_ADDR                                                    (0xA1794000)
#define B1_PHY_AGC_ANT1_BASE_ADDR                                                 (0xA1B94000)
#define B2_PHY_AGC_ANT1_BASE_ADDR                                                 (0xA1F94000)
#define B0_PHY_TXTD_ANT1_BASE_ADDR                                                (0xA1798000)
#define PHY_TXTD_ANT1_BASE_ADDR                                                   (0xA1798000)
#define B1_PHY_TXTD_ANT1_BASE_ADDR                                                (0xA1B98000)
#define B2_PHY_TXTD_ANT1_BASE_ADDR                                                (0xA1F98000)
#define B0_PHY_RXTD_ANT2_BASE_ADDR                                                (0xA17A0000)
#define PHY_RXTD_ANT2_BASE_ADDR                                                   (0xA17A0000)
#define B1_PHY_RXTD_ANT2_BASE_ADDR                                                (0xA1BA0000)
#define B2_PHY_RXTD_ANT2_BASE_ADDR                                                (0xA1FA0000)
#define B0_PHY_AGC_ANT2_BASE_ADDR                                                 (0xA17A4000)
#define PHY_AGC_ANT2_BASE_ADDR                                                    (0xA17A4000)
#define B1_PHY_AGC_ANT2_BASE_ADDR                                                 (0xA1BA4000)
#define B2_PHY_AGC_ANT2_BASE_ADDR                                                 (0xA1FA4000)
#define B0_PHY_TXTD_ANT2_BASE_ADDR                                                (0xA17A8000)
#define PHY_TXTD_ANT2_BASE_ADDR                                                   (0xA17A8000)
#define B1_PHY_TXTD_ANT2_BASE_ADDR                                                (0xA1BA8000)
#define B2_PHY_TXTD_ANT2_BASE_ADDR                                                (0xA1FA8000)
#define B0_PHY_RXTD_ANT3_BASE_ADDR                                                (0xA17B0000)
#define PHY_RXTD_ANT3_BASE_ADDR                                                   (0xA17B0000)
#define B1_PHY_RXTD_ANT3_BASE_ADDR                                                (0xA1BB0000)
#define B2_PHY_RXTD_ANT3_BASE_ADDR                                                (0xA1FB0000)
#define B0_PHY_AGC_ANT3_BASE_ADDR                                                 (0xA17B4000)
#define PHY_AGC_ANT3_BASE_ADDR                                                    (0xA17B4000)
#define B1_PHY_AGC_ANT3_BASE_ADDR                                                 (0xA1BB4000)
#define B2_PHY_AGC_ANT3_BASE_ADDR                                                 (0xA1FB4000)
#define B0_PHY_TXTD_ANT3_BASE_ADDR                                                (0xA17B8000)
#define PHY_TXTD_ANT3_BASE_ADDR                                                   (0xA17B8000)
#define B1_PHY_TXTD_ANT3_BASE_ADDR                                                (0xA1BB8000)
#define B2_PHY_TXTD_ANT3_BASE_ADDR                                                (0xA1FB8000)
#define B0_UP_PHY_RXTD_ANT0_BASE_ADDR                                             (0xA17C0000)
#define UP_PHY_RXTD_ANT0_BASE_ADDR                                                (0xA17C0000)
#define B1_UP_PHY_RXTD_ANT0_BASE_ADDR                                             (0xA1BC0000)
#define B2_UP_PHY_RXTD_ANT0_BASE_ADDR                                             (0xA1FC0000)
#define B0_UP_PHY_AGC_ANT0_BASE_ADDR                                              (0xA17C4000)
#define UP_PHY_AGC_ANT0_BASE_ADDR                                                 (0xA17C4000)
#define B1_UP_PHY_AGC_ANT0_BASE_ADDR                                              (0xA1BC4000)
#define B2_UP_PHY_AGC_ANT0_BASE_ADDR                                              (0xA1FC4000)
#define B0_UP_PHY_TXTD_ANT0_BASE_ADDR                                             (0xA17C8000)
#define UP_PHY_TXTD_ANT0_BASE_ADDR                                                (0xA17C8000)
#define B1_UP_PHY_TXTD_ANT0_BASE_ADDR                                             (0xA1BC8000)
#define B2_UP_PHY_TXTD_ANT0_BASE_ADDR                                             (0xA1FC8000)
#define B0_UP_PHY_RXTD_ANT1_BASE_ADDR                                             (0xA17D0000)
#define UP_PHY_RXTD_ANT1_BASE_ADDR                                                (0xA17D0000)
#define B1_UP_PHY_RXTD_ANT1_BASE_ADDR                                             (0xA1BD0000)
#define B2_UP_PHY_RXTD_ANT1_BASE_ADDR                                             (0xA1FD0000)
#define B0_UP_PHY_AGC_ANT1_BASE_ADDR                                              (0xA17D4000)
#define UP_PHY_AGC_ANT1_BASE_ADDR                                                 (0xA17D4000)
#define B1_UP_PHY_AGC_ANT1_BASE_ADDR                                              (0xA1BD4000)
#define B2_UP_PHY_AGC_ANT1_BASE_ADDR                                              (0xA1FD4000)
#define B0_UP_PHY_TXTD_ANT1_BASE_ADDR                                             (0xA17D8000)
#define UP_PHY_TXTD_ANT1_BASE_ADDR                                                (0xA17D8000)
#define B1_UP_PHY_TXTD_ANT1_BASE_ADDR                                             (0xA1BD8000)
#define B2_UP_PHY_TXTD_ANT1_BASE_ADDR                                             (0xA1FD8000)
#define B0_UP_PHY_RXTD_ANT2_BASE_ADDR                                             (0xA17E0000)
#define UP_PHY_RXTD_ANT2_BASE_ADDR                                                (0xA17E0000)
#define B1_UP_PHY_RXTD_ANT2_BASE_ADDR                                             (0xA1BE0000)
#define B2_UP_PHY_RXTD_ANT2_BASE_ADDR                                             (0xA1FE0000)
#define B0_UP_PHY_AGC_ANT2_BASE_ADDR                                              (0xA17E4000)
#define UP_PHY_AGC_ANT2_BASE_ADDR                                                 (0xA17E4000)
#define B1_UP_PHY_AGC_ANT2_BASE_ADDR                                              (0xA1BE4000)
#define B2_UP_PHY_AGC_ANT2_BASE_ADDR                                              (0xA1FE4000)
#define B0_UP_PHY_TXTD_ANT2_BASE_ADDR                                             (0xA17E8000)
#define UP_PHY_TXTD_ANT2_BASE_ADDR                                                (0xA17E8000)
#define B1_UP_PHY_TXTD_ANT2_BASE_ADDR                                             (0xA1BE8000)
#define B2_UP_PHY_TXTD_ANT2_BASE_ADDR                                             (0xA1FE8000)
#define B0_UP_PHY_RXTD_ANT3_BASE_ADDR                                             (0xA17F0000)
#define UP_PHY_RXTD_ANT3_BASE_ADDR                                                (0xA17F0000)
#define B1_UP_PHY_RXTD_ANT3_BASE_ADDR                                             (0xA1BF0000)
#define B2_UP_PHY_RXTD_ANT3_BASE_ADDR                                             (0xA1FF0000)
#define B0_UP_PHY_AGC_ANT3_BASE_ADDR                                              (0xA17F4000)
#define UP_PHY_AGC_ANT3_BASE_ADDR                                                 (0xA17F4000)
#define B1_UP_PHY_AGC_ANT3_BASE_ADDR                                              (0xA1BF4000)
#define B2_UP_PHY_AGC_ANT3_BASE_ADDR                                              (0xA1FF4000)
#define B0_UP_PHY_TXTD_ANT3_BASE_ADDR                                             (0xA17F8000)
#define UP_PHY_TXTD_ANT3_BASE_ADDR                                                (0xA17F8000)
#define B1_UP_PHY_TXTD_ANT3_BASE_ADDR                                             (0xA1BF8000)
#define B2_UP_PHY_TXTD_ANT3_BASE_ADDR                                             (0xA1FF8000)
#define BAND1_INTERNAL_BASE_ADDR                                                  (0xA1800000)
#define BAND2_INTERNAL_BASE_ADDR                                                  (0xA1C00000)
#define B0_RX_CIRCULAR_BASE_ADDR                                                  (0xA2000000)
#define RX_CIRCULAR_BASE_ADDR                                                     (0xA2000000)
#define B1_RX_CIRCULAR_BASE_ADDR                                                  (0xA2400000)
#define B2_RX_CIRCULAR_BASE_ADDR                                                  (0xA2800000)
#define B0_RX_PHY_STATUS_BASE_ADDR                                                (0xA21FE000)
#define RX_PHY_STATUS_BASE_ADDR                                                   (0xA21FE000)
#define B1_RX_PHY_STATUS_BASE_ADDR                                                (0xA25FE000)
#define B2_RX_PHY_STATUS_BASE_ADDR                                                (0xA29FE000)
#define B0_TX_CIRCULAR_BASE_ADDR                                                  (0xA2200000)
#define TX_CIRCULAR_BASE_ADDR                                                     (0xA2200000)
#define B1_TX_CIRCULAR_BASE_ADDR                                                  (0xA2600000)
#define B2_TX_CIRCULAR_BASE_ADDR                                                  (0xA2A00000)
#define B0_TX_RECIPES_BASE_ADDR                                                   (0xA23FE000)
#define TX_RECIPES_BASE_ADDR                                                      (0xA23FE000)
#define B1_TX_RECIPES_BASE_ADDR                                                   (0xA27FE000)
#define B2_TX_RECIPES_BASE_ADDR                                                   (0xA2BFE000)
#define BAND1_RX_BASE_ADDR                                                        (0xA2400000)
#define BAND1_TX_BASE_ADDR                                                        (0xA2600000)
#define BAND2_RX_BASE_ADDR                                                        (0xA2800000)
#define BAND2_TX_BASE_ADDR                                                        (0xA2A00000)
#define HOST_EX_MAP_CPU_BASE_ADDR                                                 (0xA4000000)
#define RF_EMULATOR_BASE_ADDR                                                     (0xA0F84000)
#define SHARED_RAM_GRAY_BASE_ADDR                                                 (0xA0600000)
#define SHARED_RAM_WHITE_BASE_ADDR                                                (0xA06A0000)
#define SHARED_RAM_BLACK_BASE_ADDR                                                (0xA06C0000)
#define B0_HOB_PAC_DUR_BASE_ADDR                                                  (0x0)
#define HOB_PAC_DUR_BASE_ADDR                                                     (0x0)
#define B1_HOB_PAC_DUR_BASE_ADDR                                                  (0x0)
#define B2_HOB_PAC_DUR_BASE_ADDR                                                  (0x0)
#define B0_HOB_MAC_BFM_BASE_ADDR                                                  (0x200)
#define HOB_MAC_BFM_BASE_ADDR                                                     (0x200)
#define B1_HOB_MAC_BFM_BASE_ADDR                                                  (0x200)
#define B2_HOB_MAC_BFM_BASE_ADDR                                                  (0x200)
#define B0_HOB_PAC_TIM_BASE_ADDR                                                  (0x400)
#define HOB_PAC_TIM_BASE_ADDR                                                     (0x400)
#define B1_HOB_PAC_TIM_BASE_ADDR                                                  (0x400)
#define B2_HOB_PAC_TIM_BASE_ADDR                                                  (0x400)
#define B0_HOB_PAC_DEL_BASE_ADDR                                                  (0x800)
#define HOB_PAC_DEL_BASE_ADDR                                                     (0x800)
#define B1_HOB_PAC_DEL_BASE_ADDR                                                  (0x800)
#define B2_HOB_PAC_DEL_BASE_ADDR                                                  (0x800)
#define B0_HOB_AUTO_RESP_BASE_ADDR                                                (0xA00)
#define HOB_AUTO_RESP_BASE_ADDR                                                   (0xA00)
#define B1_HOB_AUTO_RESP_BASE_ADDR                                                (0xA00)
#define B2_HOB_AUTO_RESP_BASE_ADDR                                                (0xA00)
#define B0_HOB_TX_SELECTOR_BASE_ADDR                                              (0xC00)
#define HOB_TX_SELECTOR_BASE_ADDR                                                 (0xC00)
#define B1_HOB_TX_SELECTOR_BASE_ADDR                                              (0xC00)
#define B2_HOB_TX_SELECTOR_BASE_ADDR                                              (0xC00)

 /* Base Addresses From Dma Persepective*/
#define CPU_IRAM_OFFSET_FROM_BAR0                                                 (0x00000000)
#define BOOT_ROM_OFFSET_FROM_BAR0                                                 (0x005F0000)
#define SHARED_RAM_OFFSET_FROM_BAR0                                               (0x00600000)
#define DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0                                        (0x00800000)
#define DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0                                        (0x00900000)
#define DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0                                     (0x00A00000)
#define PLAN_RAM_OFFSET_FROM_BAR0                                                 (0x00B00000)
#define BA_ANALYZER_COUNTER_M01_OFFSET_FROM_BAR0                                  (0x00E00000)
#define BA_ANALYZER_COUNTER_OFFSET_FROM_BAR0                                      (0x00E00000)
#define BA_ANALYZER_COUNTER_M2_OFFSET_FROM_BAR0                                   (0x00E20000)
#define RX_PP_COUNTERS_M01_OFFSET_FROM_BAR0                                       (0x00E40000)
#define RX_PP_COUNTERS_OFFSET_FROM_BAR0                                           (0x00E40000)
#define RX_PP_COUNTERS_M2_OFFSET_FROM_BAR0                                        (0x00E50000)
#define RX_COORDINATOR_COUNTERS_M01_OFFSET_FROM_BAR0                              (0x00E60000)
#define RX_COORDINATOR_COUNTERS_OFFSET_FROM_BAR0                                  (0x00E60000)
#define RX_COORDINATOR_COUNTERS_M2_OFFSET_FROM_BAR0                               (0x00E70000)
#define HOST_IF_COUNTERS_M01_OFFSET_FROM_BAR0                                     (0x00E80000)
#define HOST_IF_COUNTERS_OFFSET_FROM_BAR0                                         (0x00E80000)
#define HOST_IF_COUNTERS_M2_OFFSET_FROM_BAR0                                      (0x00E90000)
#define HOST_IF_QOS_COUNTERS_M01_OFFSET_FROM_BAR0                                 (0x00EA0000)
#define HOST_IF_QOS_COUNTERS_OFFSET_FROM_BAR0                                     (0x00EA0000)
#define HOST_IF_QOS_COUNTERS_M2_OFFSET_FROM_BAR0                                  (0x00EB0000)
#define HOST_IF_AQM_STAT_OFFSET_FROM_BAR0                                         (0x00EC0000)
#define EXT_RX_PP_COUNTERS_OFFSET_FROM_BAR0                                       (0x00EE0000)
#define SYSTEM_IF_LOWER_B0_CPU_OFFSET_FROM_BAR0                                   (0x00F00000)
#define SYSTEM_IF_LOWER_B1_CPU_OFFSET_FROM_BAR0                                   (0x00F10000)
#define SYSTEM_IF_LOWER_B2_CPU_OFFSET_FROM_BAR0                                   (0x00F20000)
#define SYSTEM_IF_UPPER_CPU_OFFSET_FROM_BAR0                                      (0x00F30000)
#define WLAN_DMA_LL_MEM_M01_OFFSET_FROM_BAR0                                      (0x00F40000)
#define WLAN_DMA_LL_MEM_OFFSET_FROM_BAR0                                          (0x00F40000)
#define WLAN_DMA_TX_M01_OFFSET_FROM_BAR0                                          (0x00F45000)
#define WLAN_DMA_TX_OFFSET_FROM_BAR0                                              (0x00F45000)
#define WLAN_DMA_RX_M01_OFFSET_FROM_BAR0                                          (0x00F45800)
#define WLAN_DMA_RX_OFFSET_FROM_BAR0                                              (0x00F45800)
#define WLAN_DMA_FW_M01_OFFSET_FROM_BAR0                                          (0x00F46000)
#define WLAN_DMA_FW_OFFSET_FROM_BAR0                                              (0x00F46000)
#define DMA_WRAPPER_M01_OFFSET_FROM_BAR0                                          (0x00F47000)
#define DMA_WRAPPER_OFFSET_FROM_BAR0                                              (0x00F47000)
#define WLAN_DMA_LL_MEM_M2_OFFSET_FROM_BAR0                                       (0x00F48000)
#define WLAN_DMA_TX_M2_OFFSET_FROM_BAR0                                           (0x00F4D000)
#define WLAN_DMA_RX_M2_OFFSET_FROM_BAR0                                           (0x00F4D800)
#define WLAN_DMA_FW_M2_OFFSET_FROM_BAR0                                           (0x00F4E000)
#define DMA_WRAPPER_M2_OFFSET_FROM_BAR0                                           (0x00F4F000)
#define DLM_M01_OFFSET_FROM_BAR0                                                  (0x00F60000)
#define DLM_OFFSET_FROM_BAR0                                                      (0x00F60000)
#define QUEUE_MANAGER_M01_TX0_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_TX0_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_TX1_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_TX1_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_TX2_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_TX2_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_RX0_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_RX0_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_RX1_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_RX1_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_RX2_OFFSET_FROM_BAR0                                    (0x00F6C000)
#define QUEUE_MANAGER_RX2_OFFSET_FROM_BAR0                                        (0x00F6C000)
#define QUEUE_MANAGER_M01_HOST_OFFSET_FROM_BAR0                                   (0x00F6C000)
#define QUEUE_MANAGER_HOST_OFFSET_FROM_BAR0                                       (0x00F6C000)
#define QUEUE_MANAGER_M01_LCPU0_OFFSET_FROM_BAR0                                  (0x00F6C000)
#define QUEUE_MANAGER_LCPU0_OFFSET_FROM_BAR0                                      (0x00F6C000)
#define QUEUE_MANAGER_M01_LCPU1_OFFSET_FROM_BAR0                                  (0x00F6C000)
#define QUEUE_MANAGER_LCPU1_OFFSET_FROM_BAR0                                      (0x00F6C000)
#define QUEUE_MANAGER_M01_LCPU2_OFFSET_FROM_BAR0                                  (0x00F6C000)
#define QUEUE_MANAGER_LCPU2_OFFSET_FROM_BAR0                                      (0x00F6C000)
#define QUEUE_MANAGER_M01_UCPU_OFFSET_FROM_BAR0                                   (0x00F6C000)
#define QUEUE_MANAGER_UCPU_OFFSET_FROM_BAR0                                       (0x00F6C000)
#define DLM_M2_OFFSET_FROM_BAR0                                                   (0x00F70000)
#define QUEUE_MANAGER_M2_TX0_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_TX1_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_TX2_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_RX0_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_RX1_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_RX2_OFFSET_FROM_BAR0                                     (0x00F7C000)
#define QUEUE_MANAGER_M2_HOST_OFFSET_FROM_BAR0                                    (0x00F7C000)
#define QUEUE_MANAGER_M2_LCPU0_OFFSET_FROM_BAR0                                   (0x00F7C000)
#define QUEUE_MANAGER_M2_LCPU1_OFFSET_FROM_BAR0                                   (0x00F7C000)
#define QUEUE_MANAGER_M2_LCPU2_OFFSET_FROM_BAR0                                   (0x00F7C000)
#define QUEUE_MANAGER_M2_UCPU_OFFSET_FROM_BAR0                                    (0x00F7C000)
#define TOP_REGBLOCK_OFFSET_FROM_BAR0                                             (0x00F80000)
#define PCIE_PHY0_REGBLOCK_OFFSET_FROM_BAR0                                       (0x00F88000)
#define PCIE_DBI0_REGBLOCK_OFFSET_FROM_BAR0                                       (0x00F8B000)
#define PVT_BLOCK_OFFSET_FROM_BAR0                                                (0x00F90000)
#define CPU_XBAR_STATUS_OFFSET_FROM_BAR0                                          (0x00FA0000)
#define MAC_XBAR_PM_OFFSET_FROM_BAR0                                              (0x00FA2000)
#define MAC_XBAR_STATUS_OFFSET_FROM_BAR0                                          (0x00FA3000)
#define RX_PP_M01_OFFSET_FROM_BAR0                                                (0x00FC0000)
#define RX_PP_OFFSET_FROM_BAR0                                                    (0x00FC0000)
#define RX_PP_REGS_M01_OFFSET_FROM_BAR0                                           (0x00FCF800)
#define RX_PP_REGS_OFFSET_FROM_BAR0                                               (0x00FCF800)
#define RX_PP_M2_OFFSET_FROM_BAR0                                                 (0x00FD0000)
#define RX_PP_REGS_M2_OFFSET_FROM_BAR0                                            (0x00FDF800)
#define DATA_LOGGER_OFFSET_FROM_BAR0                                              (0x00FE0000)
#define PROT_DB_OFFSET_FROM_BAR0                                                  (0x01000000)
#define PROT_DB_SHADOW_MEM_OFFSET_FROM_BAR0                                       (0x01000200)
#define TRAFFIC_INDICATOR_M01_OFFSET_FROM_BAR0                                    (0x01002000)
#define TRAFFIC_INDICATOR_OFFSET_FROM_BAR0                                        (0x01002000)
#define TRAFFIC_INDICATOR_M2_OFFSET_FROM_BAR0                                     (0x01002800)
#define VAP_ADDR_REG_M01_OFFSET_FROM_BAR0                                         (0x01003000)
#define VAP_ADDR_REG_OFFSET_FROM_BAR0                                             (0x01003000)
#define VAP_ADDR_REG_M2_OFFSET_FROM_BAR0                                          (0x01003400)
#define MAC_GEN_REGFILE_OFFSET_FROM_BAR0                                          (0x01004000)
#define TIM_GEN_OFFSET_FROM_BAR0                                                  (0x01005000)
#define HOST_IF_ACC_OFFSET_FROM_BAR0                                              (0x01008000)
#define HOST_IF_REG_FILE_OFFSET_FROM_BAR0                                         (0x01009000)
#define BSRC_CNT_MEM_M01_OFFSET_FROM_BAR0                                         (0x01088000)
#define BSRC_CNT_MEM_OFFSET_FROM_BAR0                                             (0x01088000)
#define BSRC_CNT_MEM_M2_OFFSET_FROM_BAR0                                          (0x0108A000)
#define BSRC_AGER_MEM_M01_OFFSET_FROM_BAR0                                        (0x0108C000)
#define BSRC_AGER_MEM_OFFSET_FROM_BAR0                                            (0x0108C000)
#define BSRC_AGER_MEM_M2_OFFSET_FROM_BAR0                                         (0x0108D000)
#define BEST_RU_DB_M01_OFFSET_FROM_BAR0                                           (0x01090000)
#define BEST_RU_DB_OFFSET_FROM_BAR0                                               (0x01090000)
#define BEST_RU_DB_M2_OFFSET_FROM_BAR0                                            (0x01098000)
#define RETRY_STATE_DB_M01_OFFSET_FROM_BAR0                                       (0x010A8000)
#define RETRY_STATE_DB_OFFSET_FROM_BAR0                                           (0x010A8000)
#define RETRY_STATE_DB_M2_OFFSET_FROM_BAR0                                        (0x010AC000)
#define RETRY_POINTER_DB_M01_OFFSET_FROM_BAR0                                     (0x010C0000)
#define RETRY_POINTER_DB_OFFSET_FROM_BAR0                                         (0x010C0000)
#define RETRY_POINTER_DB_M2_OFFSET_FROM_BAR0                                      (0x010E0000)
#define MAC_HOSTIF_UC_INTERNAL_OFFSET_FROM_BAR0                                   (0x01200000)
#define MAC_HOSTIF_UC_CLUSTER_OFFSET_FROM_BAR0                                    (0x01220000)
#define B0_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0                                   (0x01400000)
#define MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0                                      (0x01400000)
#define B1_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0                                   (0x01800000)
#define B2_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0                                   (0x01C00000)
#define B0_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0                                  (0x01410000)
#define MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0                                     (0x01410000)
#define B1_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0                                  (0x01810000)
#define B2_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0                                  (0x01C10000)
#define B0_MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0                                    (0x01411000)
#define MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0                                       (0x01411000)
#define B1_MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0                                    (0x01811000)
#define B2_MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0                                    (0x01C11000)
#define B0_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0                                    (0x01412000)
#define MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0                                       (0x01412000)
#define B1_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0                                    (0x01812000)
#define B2_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0                                    (0x01C12000)
#define B0_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0                                   (0x01420000)
#define MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0                                      (0x01420000)
#define B1_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0                                   (0x01820000)
#define B2_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0                                   (0x01C20000)
#define B0_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0                                  (0x0142F000)
#define MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0                                     (0x0142F000)
#define B1_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0                                  (0x0182F000)
#define B2_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0                                  (0x01C2F000)
#define B0_MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0                                    (0x01430000)
#define MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0                                       (0x01430000)
#define B1_MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0                                    (0x01830000)
#define B2_MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0                                    (0x01C30000)
#define B0_TX_SENDER_UC_CLUSTER_OFFSET_FROM_BAR0                                  (0x01438000)
#define TX_SENDER_UC_CLUSTER_OFFSET_FROM_BAR0                                     (0x01438000)
#define B1_TX_SENDER_UC_CLUSTER_OFFSET_FROM_BAR0                                  (0x01838000)
#define B2_TX_SENDER_UC_CLUSTER_OFFSET_FROM_BAR0                                  (0x01C38000)
#define B0_RX_UC_CLUSTER_OFFSET_FROM_BAR0                                         (0x0143C000)
#define RX_UC_CLUSTER_OFFSET_FROM_BAR0                                            (0x0143C000)
#define B1_RX_UC_CLUSTER_OFFSET_FROM_BAR0                                         (0x0183C000)
#define B2_RX_UC_CLUSTER_OFFSET_FROM_BAR0                                         (0x01C3C000)
#define B0_TX_SENDER_SINGLE_UC_OFFSET_FROM_BAR0                                   (0x01440000)
#define TX_SENDER_SINGLE_UC_OFFSET_FROM_BAR0                                      (0x01440000)
#define B1_TX_SENDER_SINGLE_UC_OFFSET_FROM_BAR0                                   (0x01840000)
#define B2_TX_SENDER_SINGLE_UC_OFFSET_FROM_BAR0                                   (0x01C40000)
#define B0_RX_SINGLE_UC_OFFSET_FROM_BAR0                                          (0x01460000)
#define RX_SINGLE_UC_OFFSET_FROM_BAR0                                             (0x01460000)
#define B1_RX_SINGLE_UC_OFFSET_FROM_BAR0                                          (0x01860000)
#define B2_RX_SINGLE_UC_OFFSET_FROM_BAR0                                          (0x01C60000)
#define B0_MAC_HT_EXTENSIONS_OFFSET_FROM_BAR0                                     (0x01481000)
#define MAC_HT_EXTENSIONS_OFFSET_FROM_BAR0                                        (0x01481000)
#define B1_MAC_HT_EXTENSIONS_OFFSET_FROM_BAR0                                     (0x01881000)
#define B2_MAC_HT_EXTENSIONS_OFFSET_FROM_BAR0                                     (0x01C81000)
#define B0_RXH_RD_ACC_OFFSET_FROM_BAR0                                            (0x01482000)
#define RXH_RD_ACC_OFFSET_FROM_BAR0                                               (0x01482000)
#define B1_RXH_RD_ACC_OFFSET_FROM_BAR0                                            (0x01882000)
#define B2_RXH_RD_ACC_OFFSET_FROM_BAR0                                            (0x01C82000)
#define B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0                                        (0x01488000)
#define AUTO_RESP_MEMS_OFFSET_FROM_BAR0                                           (0x01488000)
#define B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0                                        (0x01888000)
#define B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0                                        (0x01C88000)
#define B0_AUTO_RESP_REGS_OFFSET_FROM_BAR0                                        (0x0148B000)
#define AUTO_RESP_REGS_OFFSET_FROM_BAR0                                           (0x0148B000)
#define B1_AUTO_RESP_REGS_OFFSET_FROM_BAR0                                        (0x0188B000)
#define B2_AUTO_RESP_REGS_OFFSET_FROM_BAR0                                        (0x01C8B000)
#define B0_PAC_DUR_OFFSET_FROM_BAR0                                               (0x01500400)
#define PAC_DUR_OFFSET_FROM_BAR0                                                  (0x01500400)
#define B1_PAC_DUR_OFFSET_FROM_BAR0                                               (0x01900400)
#define B2_PAC_DUR_OFFSET_FROM_BAR0                                               (0x01D00400)
#define B0_PAC_TIM_OFFSET_FROM_BAR0                                               (0x01500500)
#define PAC_TIM_OFFSET_FROM_BAR0                                                  (0x01500500)
#define B1_PAC_TIM_OFFSET_FROM_BAR0                                               (0x01900500)
#define B2_PAC_TIM_OFFSET_FROM_BAR0                                               (0x01D00500)
#define B0_PAC_EXTRAPOLATOR_OFFSET_FROM_BAR0                                      (0x01500600)
#define PAC_EXTRAPOLATOR_OFFSET_FROM_BAR0                                         (0x01500600)
#define B1_PAC_EXTRAPOLATOR_OFFSET_FROM_BAR0                                      (0x01900600)
#define B2_PAC_EXTRAPOLATOR_OFFSET_FROM_BAR0                                      (0x01D00600)
#define B0_MAC_BEACON_OFFSET_FROM_BAR0                                            (0x01500800)
#define MAC_BEACON_OFFSET_FROM_BAR0                                               (0x01500800)
#define B1_MAC_BEACON_OFFSET_FROM_BAR0                                            (0x01900800)
#define B2_MAC_BEACON_OFFSET_FROM_BAR0                                            (0x01D00800)
#define B0_TXH_TXC_OFFSET_FROM_BAR0                                               (0x01500E00)
#define TXH_TXC_OFFSET_FROM_BAR0                                                  (0x01500E00)
#define B1_TXH_TXC_OFFSET_FROM_BAR0                                               (0x01900E00)
#define B2_TXH_TXC_OFFSET_FROM_BAR0                                               (0x01D00E00)
#define B0_TXH_NTD_OFFSET_FROM_BAR0                                               (0x01500F00)
#define TXH_NTD_OFFSET_FROM_BAR0                                                  (0x01500F00)
#define B1_TXH_NTD_OFFSET_FROM_BAR0                                               (0x01900F00)
#define B2_TXH_NTD_OFFSET_FROM_BAR0                                               (0x01D00F00)
#define B0_PS_SETTING_OFFSET_FROM_BAR0                                            (0x01501000)
#define PS_SETTING_OFFSET_FROM_BAR0                                               (0x01501000)
#define B1_PS_SETTING_OFFSET_FROM_BAR0                                            (0x01901000)
#define B2_PS_SETTING_OFFSET_FROM_BAR0                                            (0x01D01000)
#define B0_PAC_DELIA_REG_OFFSET_FROM_BAR0                                         (0x01501C00)
#define PAC_DELIA_REG_OFFSET_FROM_BAR0                                            (0x01501C00)
#define B1_PAC_DELIA_REG_OFFSET_FROM_BAR0                                         (0x01901C00)
#define B2_PAC_DELIA_REG_OFFSET_FROM_BAR0                                         (0x01D01C00)
#define B0_PAC_DELIA_MEM_OFFSET_FROM_BAR0                                         (0x01501E00)
#define PAC_DELIA_MEM_OFFSET_FROM_BAR0                                            (0x01501E00)
#define B1_PAC_DELIA_MEM_OFFSET_FROM_BAR0                                         (0x01901E00)
#define B2_PAC_DELIA_MEM_OFFSET_FROM_BAR0                                         (0x01D01E00)
#define B0_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0                                     (0x01503800)
#define DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0                                        (0x01503800)
#define B1_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0                                     (0x01903800)
#define B2_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0                                     (0x01D03800)
#define B0_PAC_RXC_OFFSET_FROM_BAR0                                               (0x01504000)
#define PAC_RXC_OFFSET_FROM_BAR0                                                  (0x01504000)
#define B1_PAC_RXC_OFFSET_FROM_BAR0                                               (0x01904000)
#define B2_PAC_RXC_OFFSET_FROM_BAR0                                               (0x01D04000)
#define B0_RXF_MEM_OFFSET_FROM_BAR0                                               (0x01504400)
#define RXF_MEM_OFFSET_FROM_BAR0                                                  (0x01504400)
#define B1_RXF_MEM_OFFSET_FROM_BAR0                                               (0x01904400)
#define B2_RXF_MEM_OFFSET_FROM_BAR0                                               (0x01D04400)
#define B0_MAC_SEC_MEM_OFFSET_FROM_BAR0                                           (0x01510000)
#define MAC_SEC_MEM_OFFSET_FROM_BAR0                                              (0x01510000)
#define B1_MAC_SEC_MEM_OFFSET_FROM_BAR0                                           (0x01910000)
#define B2_MAC_SEC_MEM_OFFSET_FROM_BAR0                                           (0x01D10000)
#define B0_MAC_WEP_OFFSET_FROM_BAR0                                               (0x01518000)
#define MAC_WEP_OFFSET_FROM_BAR0                                                  (0x01518000)
#define B1_MAC_WEP_OFFSET_FROM_BAR0                                               (0x01918000)
#define B2_MAC_WEP_OFFSET_FROM_BAR0                                               (0x01D18000)
#define B0_PRE_AGG_OFFSET_FROM_BAR0                                               (0x01520000)
#define PRE_AGG_OFFSET_FROM_BAR0                                                  (0x01520000)
#define B1_PRE_AGG_OFFSET_FROM_BAR0                                               (0x01920000)
#define B2_PRE_AGG_OFFSET_FROM_BAR0                                               (0x01D20000)
#define B0_MLD_DB_OFFSET_FROM_BAR0                                                (0x0152C000)
#define MLD_DB_OFFSET_FROM_BAR0                                                   (0x0152C000)
#define B1_MLD_DB_OFFSET_FROM_BAR0                                                (0x0192C000)
#define B2_MLD_DB_OFFSET_FROM_BAR0                                                (0x01D2C000)
#define B0_TX_SEQUENCER_OFFSET_FROM_BAR0                                          (0x0152E000)
#define TX_SEQUENCER_OFFSET_FROM_BAR0                                             (0x0152E000)
#define B1_TX_SEQUENCER_OFFSET_FROM_BAR0                                          (0x0192E000)
#define B2_TX_SEQUENCER_OFFSET_FROM_BAR0                                          (0x01D2E000)
#define B0_TX_PD_ACC_MEM_OFFSET_FROM_BAR0                                         (0x0152E800)
#define TX_PD_ACC_MEM_OFFSET_FROM_BAR0                                            (0x0152E800)
#define B1_TX_PD_ACC_MEM_OFFSET_FROM_BAR0                                         (0x0192E800)
#define B2_TX_PD_ACC_MEM_OFFSET_FROM_BAR0                                         (0x01D2E800)
#define B0_TX_PD_ACC_REG_OFFSET_FROM_BAR0                                         (0x0152EE00)
#define TX_PD_ACC_REG_OFFSET_FROM_BAR0                                            (0x0152EE00)
#define B1_TX_PD_ACC_REG_OFFSET_FROM_BAR0                                         (0x0192EE00)
#define B2_TX_PD_ACC_REG_OFFSET_FROM_BAR0                                         (0x01D2EE00)
#define B0_TF_GENERATOR_MEM_OFFSET_FROM_BAR0                                      (0x0152F000)
#define TF_GENERATOR_MEM_OFFSET_FROM_BAR0                                         (0x0152F000)
#define B1_TF_GENERATOR_MEM_OFFSET_FROM_BAR0                                      (0x0192F000)
#define B2_TF_GENERATOR_MEM_OFFSET_FROM_BAR0                                      (0x01D2F000)
#define B0_TF_GENERATOR_REG_OFFSET_FROM_BAR0                                      (0x0152F600)
#define TF_GENERATOR_REG_OFFSET_FROM_BAR0                                         (0x0152F600)
#define B1_TF_GENERATOR_REG_OFFSET_FROM_BAR0                                      (0x0192F600)
#define B2_TF_GENERATOR_REG_OFFSET_FROM_BAR0                                      (0x01D2F600)
#define B0_TF_DECODER_REG_OFFSET_FROM_BAR0                                        (0x0152F800)
#define TF_DECODER_REG_OFFSET_FROM_BAR0                                           (0x0152F800)
#define B1_TF_DECODER_REG_OFFSET_FROM_BAR0                                        (0x0192F800)
#define B2_TF_DECODER_REG_OFFSET_FROM_BAR0                                        (0x01D2F800)
#define B0_TF_DECODER_MEM_OFFSET_FROM_BAR0                                        (0x0152FA00)
#define TF_DECODER_MEM_OFFSET_FROM_BAR0                                           (0x0152FA00)
#define B1_TF_DECODER_MEM_OFFSET_FROM_BAR0                                        (0x0192FA00)
#define B2_TF_DECODER_MEM_OFFSET_FROM_BAR0                                        (0x01D2FA00)
#define B0_RAB_OFFSET_FROM_BAR0                                                   (0x01532000)
#define RAB_OFFSET_FROM_BAR0                                                      (0x01532000)
#define B1_RAB_OFFSET_FROM_BAR0                                                   (0x01932000)
#define B2_RAB_OFFSET_FROM_BAR0                                                   (0x01D32000)
#define B0_MAC_ADDR2INDEX_OFFSET_FROM_BAR0                                        (0x01533000)
#define MAC_ADDR2INDEX_OFFSET_FROM_BAR0                                           (0x01533000)
#define B1_MAC_ADDR2INDEX_OFFSET_FROM_BAR0                                        (0x01933000)
#define B2_MAC_ADDR2INDEX_OFFSET_FROM_BAR0                                        (0x01D33000)
#define B0_TX_HANDLER_OFFSET_FROM_BAR0                                            (0x01534000)
#define TX_HANDLER_OFFSET_FROM_BAR0                                               (0x01534000)
#define B1_TX_HANDLER_OFFSET_FROM_BAR0                                            (0x01934000)
#define B2_TX_HANDLER_OFFSET_FROM_BAR0                                            (0x01D34000)
#define B0_BA_ANALYZER_OFFSET_FROM_BAR0                                           (0x01535000)
#define BA_ANALYZER_OFFSET_FROM_BAR0                                              (0x01535000)
#define B1_BA_ANALYZER_OFFSET_FROM_BAR0                                           (0x01935000)
#define B2_BA_ANALYZER_OFFSET_FROM_BAR0                                           (0x01D35000)
#define B0_TWT_STATION_MEM_OFFSET_FROM_BAR0                                       (0x01536000)
#define TWT_STATION_MEM_OFFSET_FROM_BAR0                                          (0x01536000)
#define B1_TWT_STATION_MEM_OFFSET_FROM_BAR0                                       (0x01936000)
#define B2_TWT_STATION_MEM_OFFSET_FROM_BAR0                                       (0x01D36000)
#define B0_TWT_GROUP_MEM_OFFSET_FROM_BAR0                                         (0x01537000)
#define TWT_GROUP_MEM_OFFSET_FROM_BAR0                                            (0x01537000)
#define B1_TWT_GROUP_MEM_OFFSET_FROM_BAR0                                         (0x01937000)
#define B2_TWT_GROUP_MEM_OFFSET_FROM_BAR0                                         (0x01D37000)
#define B0_TWT_REG_OFFSET_FROM_BAR0                                               (0x01537F00)
#define TWT_REG_OFFSET_FROM_BAR0                                                  (0x01537F00)
#define B1_TWT_REG_OFFSET_FROM_BAR0                                               (0x01937F00)
#define B2_TWT_REG_OFFSET_FROM_BAR0                                               (0x01D37F00)
#define B0_TX_SELECTOR_OFFSET_FROM_BAR0                                           (0x01538000)
#define TX_SELECTOR_OFFSET_FROM_BAR0                                              (0x01538000)
#define B1_TX_SELECTOR_OFFSET_FROM_BAR0                                           (0x01938000)
#define B2_TX_SELECTOR_OFFSET_FROM_BAR0                                           (0x01D38000)
#define B0_RX_CLASSIFIER_OFFSET_FROM_BAR0                                         (0x0153B000)
#define RX_CLASSIFIER_OFFSET_FROM_BAR0                                            (0x0153B000)
#define B1_RX_CLASSIFIER_OFFSET_FROM_BAR0                                         (0x0193B000)
#define B2_RX_CLASSIFIER_OFFSET_FROM_BAR0                                         (0x01D3B000)
#define B0_TIM_IE_OFFSET_FROM_BAR0                                                (0x0153C000)
#define TIM_IE_OFFSET_FROM_BAR0                                                   (0x0153C000)
#define B1_TIM_IE_OFFSET_FROM_BAR0                                                (0x0193C000)
#define B2_TIM_IE_OFFSET_FROM_BAR0                                                (0x01D3C000)
#define B0_RX_MSDU_PARSER_OFFSET_FROM_BAR0                                        (0x0153E000)
#define RX_MSDU_PARSER_OFFSET_FROM_BAR0                                           (0x0153E000)
#define B1_RX_MSDU_PARSER_OFFSET_FROM_BAR0                                        (0x0193E000)
#define B2_RX_MSDU_PARSER_OFFSET_FROM_BAR0                                        (0x01D3E000)
#define B0_RX_COORDINATOR_OFFSET_FROM_BAR0                                        (0x01541000)
#define RX_COORDINATOR_OFFSET_FROM_BAR0                                           (0x01541000)
#define B1_RX_COORDINATOR_OFFSET_FROM_BAR0                                        (0x01941000)
#define B2_RX_COORDINATOR_OFFSET_FROM_BAR0                                        (0x01D41000)
#define B0_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0                                (0x01541200)
#define FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0                                   (0x01541200)
#define B1_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0                                (0x01941200)
#define B2_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0                                (0x01D41200)
#define B0_OTFA_MEM2_OFFSET_FROM_BAR0                                             (0x01542000)
#define OTFA_MEM2_OFFSET_FROM_BAR0                                                (0x01542000)
#define B1_OTFA_MEM2_OFFSET_FROM_BAR0                                             (0x01942000)
#define B2_OTFA_MEM2_OFFSET_FROM_BAR0                                             (0x01D42000)
#define B0_OTFA_REG_OFFSET_FROM_BAR0                                              (0x01543800)
#define OTFA_REG_OFFSET_FROM_BAR0                                                 (0x01543800)
#define B1_OTFA_REG_OFFSET_FROM_BAR0                                              (0x01943800)
#define B2_OTFA_REG_OFFSET_FROM_BAR0                                              (0x01D43800)
#define B0_MAC_TX_HC_OFFSET_FROM_BAR0                                             (0x01547800)
#define MAC_TX_HC_OFFSET_FROM_BAR0                                                (0x01547800)
#define B1_MAC_TX_HC_OFFSET_FROM_BAR0                                             (0x01947800)
#define B2_MAC_TX_HC_OFFSET_FROM_BAR0                                             (0x01D47800)
#define B0_BSRC_REGS_OFFSET_FROM_BAR0                                             (0x01547C00)
#define BSRC_REGS_OFFSET_FROM_BAR0                                                (0x01547C00)
#define B1_BSRC_REGS_OFFSET_FROM_BAR0                                             (0x01947C00)
#define B2_BSRC_REGS_OFFSET_FROM_BAR0                                             (0x01D47C00)
#define B0_MAC_BFM_OFFSET_FROM_BAR0                                               (0x01548000)
#define MAC_BFM_OFFSET_FROM_BAR0                                                  (0x01548000)
#define B1_MAC_BFM_OFFSET_FROM_BAR0                                               (0x01948000)
#define B2_MAC_BFM_OFFSET_FROM_BAR0                                               (0x01D48000)
#define B0_UL_POST_RX_OFFSET_FROM_BAR0                                            (0x0154A000)
#define UL_POST_RX_OFFSET_FROM_BAR0                                               (0x0154A000)
#define B1_UL_POST_RX_OFFSET_FROM_BAR0                                            (0x0194A000)
#define B2_UL_POST_RX_OFFSET_FROM_BAR0                                            (0x01D4A000)
#define B0_BESTRU_OFFSET_FROM_BAR0                                                (0x0154B000)
#define BESTRU_OFFSET_FROM_BAR0                                                   (0x0154B000)
#define B1_BESTRU_OFFSET_FROM_BAR0                                                (0x0194B000)
#define B2_BESTRU_OFFSET_FROM_BAR0                                                (0x01D4B000)
#define B0_BUF_STATUS_REPORT_OFFSET_FROM_BAR0                                     (0x01550000)
#define BUF_STATUS_REPORT_OFFSET_FROM_BAR0                                        (0x01550000)
#define B1_BUF_STATUS_REPORT_OFFSET_FROM_BAR0                                     (0x01950000)
#define B2_BUF_STATUS_REPORT_OFFSET_FROM_BAR0                                     (0x01D50000)
#define B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0                                         (0x01600000)
#define MPDU_DESC_MEM_OFFSET_FROM_BAR0                                            (0x01600000)
#define B1_MPDU_DESC_MEM_OFFSET_FROM_BAR0                                         (0x01A00000)
#define B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0                                         (0x01E00000)
#define B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0                             (0x016F0000)
#define PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0                                (0x016F0000)
#define B1_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0                             (0x01AF0000)
#define B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0                             (0x01EF0000)
#define B0_PHY_RISC_OFFSET_FROM_BAR0                                              (0x01700000)
#define PHY_RISC_OFFSET_FROM_BAR0                                                 (0x01700000)
#define B1_PHY_RISC_OFFSET_FROM_BAR0                                              (0x01B00000)
#define B2_PHY_RISC_OFFSET_FROM_BAR0                                              (0x01F00000)
#define B0_PHY_RX_TD_OFFSET_FROM_BAR0                                             (0x01710000)
#define PHY_RX_TD_OFFSET_FROM_BAR0                                                (0x01710000)
#define B1_PHY_RX_TD_OFFSET_FROM_BAR0                                             (0x01B10000)
#define B2_PHY_RX_TD_OFFSET_FROM_BAR0                                             (0x01F10000)
#define B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0                                       (0x01714000)
#define PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0                                          (0x01714000)
#define B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0                                       (0x01B14000)
#define B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0                                       (0x01F14000)
#define B0_PHY_RX_BE_OFFSET_FROM_BAR0                                             (0x0171C000)
#define PHY_RX_BE_OFFSET_FROM_BAR0                                                (0x0171C000)
#define B1_PHY_RX_BE_OFFSET_FROM_BAR0                                             (0x01B1C000)
#define B2_PHY_RX_BE_OFFSET_FROM_BAR0                                             (0x01F1C000)
#define B0_PHY_RX_FD_OFFSET_FROM_BAR0                                             (0x01720000)
#define PHY_RX_FD_OFFSET_FROM_BAR0                                                (0x01720000)
#define B1_PHY_RX_FD_OFFSET_FROM_BAR0                                             (0x01B20000)
#define B2_PHY_RX_FD_OFFSET_FROM_BAR0                                             (0x01F20000)
#define B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                       (0x0172C000)
#define PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                          (0x0172C000)
#define B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                       (0x01B2C000)
#define B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                       (0x01F2C000)
#define B0_PHY_TX_OFFSET_FROM_BAR0                                                (0x0173C000)
#define PHY_TX_OFFSET_FROM_BAR0                                                   (0x0173C000)
#define B1_PHY_TX_OFFSET_FROM_BAR0                                                (0x01B3C000)
#define B2_PHY_TX_OFFSET_FROM_BAR0                                                (0x01F3C000)
#define B0_UP_PHY_RX_FD_OFFSET_FROM_BAR0                                          (0x01740000)
#define UP_PHY_RX_FD_OFFSET_FROM_BAR0                                             (0x01740000)
#define B1_UP_PHY_RX_FD_OFFSET_FROM_BAR0                                          (0x01B40000)
#define B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0                                          (0x01F40000)
#define B0_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                    (0x0174C000)
#define UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                       (0x0174C000)
#define B1_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                    (0x01B4C000)
#define B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0                                    (0x01F4C000)
#define B0_UP_PHY_TX_OFFSET_FROM_BAR0                                             (0x0175C000)
#define UP_PHY_TX_OFFSET_FROM_BAR0                                                (0x0175C000)
#define B1_UP_PHY_TX_OFFSET_FROM_BAR0                                             (0x01B5C000)
#define B2_UP_PHY_TX_OFFSET_FROM_BAR0                                             (0x01F5C000)
#define B0_PHY_DFS_RISC_OFFSET_FROM_BAR0                                          (0x01760000)
#define PHY_DFS_RISC_OFFSET_FROM_BAR0                                             (0x01760000)
#define B1_PHY_DFS_RISC_OFFSET_FROM_BAR0                                          (0x01B60000)
#define B2_PHY_DFS_RISC_OFFSET_FROM_BAR0                                          (0x01F60000)
#define B0_PHY_DFS_RX_TD_OFFSET_FROM_BAR0                                         (0x01770000)
#define PHY_DFS_RX_TD_OFFSET_FROM_BAR0                                            (0x01770000)
#define B1_PHY_DFS_RX_TD_OFFSET_FROM_BAR0                                         (0x01B70000)
#define B2_PHY_DFS_RX_TD_OFFSET_FROM_BAR0                                         (0x01F70000)
#define B0_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0                                      (0x01778000)
#define PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0                                         (0x01778000)
#define B1_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0                                      (0x01B78000)
#define B2_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0                                      (0x01F78000)
#define B0_PHY_DFS_AGC_ANT_OFFSET_FROM_BAR0                                       (0x0177C000)
#define PHY_DFS_AGC_ANT_OFFSET_FROM_BAR0                                          (0x0177C000)
#define B1_PHY_DFS_AGC_ANT_OFFSET_FROM_BAR0                                       (0x01B7C000)
#define B2_PHY_DFS_AGC_ANT_OFFSET_FROM_BAR0                                       (0x01F7C000)
#define B0_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01780000)
#define PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                            (0x01780000)
#define B1_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01B80000)
#define B2_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01F80000)
#define B0_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                          (0x01784000)
#define PHY_AGC_ANT0_OFFSET_FROM_BAR0                                             (0x01784000)
#define B1_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                          (0x01B84000)
#define B2_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                          (0x01F84000)
#define B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01788000)
#define PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                            (0x01788000)
#define B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01B88000)
#define B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                         (0x01F88000)
#define B0_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01790000)
#define PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                            (0x01790000)
#define B1_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01B90000)
#define B2_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01F90000)
#define B0_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                          (0x01794000)
#define PHY_AGC_ANT1_OFFSET_FROM_BAR0                                             (0x01794000)
#define B1_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                          (0x01B94000)
#define B2_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                          (0x01F94000)
#define B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01798000)
#define PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                            (0x01798000)
#define B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01B98000)
#define B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                         (0x01F98000)
#define B0_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                         (0x017A0000)
#define PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                            (0x017A0000)
#define B1_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                         (0x01BA0000)
#define B2_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                         (0x01FA0000)
#define B0_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                          (0x017A4000)
#define PHY_AGC_ANT2_OFFSET_FROM_BAR0                                             (0x017A4000)
#define B1_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                          (0x01BA4000)
#define B2_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                          (0x01FA4000)
#define B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                         (0x017A8000)
#define PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                            (0x017A8000)
#define B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                         (0x01BA8000)
#define B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                         (0x01FA8000)
#define B0_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                         (0x017B0000)
#define PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                            (0x017B0000)
#define B1_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                         (0x01BB0000)
#define B2_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                         (0x01FB0000)
#define B0_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                          (0x017B4000)
#define PHY_AGC_ANT3_OFFSET_FROM_BAR0                                             (0x017B4000)
#define B1_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                          (0x01BB4000)
#define B2_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                          (0x01FB4000)
#define B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                         (0x017B8000)
#define PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                            (0x017B8000)
#define B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                         (0x01BB8000)
#define B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                         (0x01FB8000)
#define B0_UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                      (0x017C0000)
#define UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                         (0x017C0000)
#define B1_UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                      (0x01BC0000)
#define B2_UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0                                      (0x01FC0000)
#define B0_UP_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                       (0x017C4000)
#define UP_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                          (0x017C4000)
#define B1_UP_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                       (0x01BC4000)
#define B2_UP_PHY_AGC_ANT0_OFFSET_FROM_BAR0                                       (0x01FC4000)
#define B0_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                      (0x017C8000)
#define UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                         (0x017C8000)
#define B1_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                      (0x01BC8000)
#define B2_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0                                      (0x01FC8000)
#define B0_UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                      (0x017D0000)
#define UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                         (0x017D0000)
#define B1_UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                      (0x01BD0000)
#define B2_UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0                                      (0x01FD0000)
#define B0_UP_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                       (0x017D4000)
#define UP_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                          (0x017D4000)
#define B1_UP_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                       (0x01BD4000)
#define B2_UP_PHY_AGC_ANT1_OFFSET_FROM_BAR0                                       (0x01FD4000)
#define B0_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                      (0x017D8000)
#define UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                         (0x017D8000)
#define B1_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                      (0x01BD8000)
#define B2_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0                                      (0x01FD8000)
#define B0_UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                      (0x017E0000)
#define UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                         (0x017E0000)
#define B1_UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                      (0x01BE0000)
#define B2_UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0                                      (0x01FE0000)
#define B0_UP_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                       (0x017E4000)
#define UP_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                          (0x017E4000)
#define B1_UP_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                       (0x01BE4000)
#define B2_UP_PHY_AGC_ANT2_OFFSET_FROM_BAR0                                       (0x01FE4000)
#define B0_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                      (0x017E8000)
#define UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                         (0x017E8000)
#define B1_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                      (0x01BE8000)
#define B2_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0                                      (0x01FE8000)
#define B0_UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                      (0x017F0000)
#define UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                         (0x017F0000)
#define B1_UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                      (0x01BF0000)
#define B2_UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0                                      (0x01FF0000)
#define B0_UP_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                       (0x017F4000)
#define UP_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                          (0x017F4000)
#define B1_UP_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                       (0x01BF4000)
#define B2_UP_PHY_AGC_ANT3_OFFSET_FROM_BAR0                                       (0x01FF4000)
#define B0_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                      (0x017F8000)
#define UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                         (0x017F8000)
#define B1_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                      (0x01BF8000)
#define B2_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0                                      (0x01FF8000)
#define BAND1_INTERNAL_OFFSET_FROM_BAR0                                           (0x01800000)
#define BAND2_INTERNAL_OFFSET_FROM_BAR0                                           (0x01C00000)
#define B0_RX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02000000)
#define RX_CIRCULAR_OFFSET_FROM_BAR0                                              (0x02000000)
#define B1_RX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02400000)
#define B2_RX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02800000)
#define B0_RX_PHY_STATUS_OFFSET_FROM_BAR0                                         (0x021FE000)
#define RX_PHY_STATUS_OFFSET_FROM_BAR0                                            (0x021FE000)
#define B1_RX_PHY_STATUS_OFFSET_FROM_BAR0                                         (0x025FE000)
#define B2_RX_PHY_STATUS_OFFSET_FROM_BAR0                                         (0x029FE000)
#define B0_TX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02200000)
#define TX_CIRCULAR_OFFSET_FROM_BAR0                                              (0x02200000)
#define B1_TX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02600000)
#define B2_TX_CIRCULAR_OFFSET_FROM_BAR0                                           (0x02A00000)
#define B0_TX_RECIPES_OFFSET_FROM_BAR0                                            (0x023FE000)
#define TX_RECIPES_OFFSET_FROM_BAR0                                               (0x023FE000)
#define B1_TX_RECIPES_OFFSET_FROM_BAR0                                            (0x027FE000)
#define B2_TX_RECIPES_OFFSET_FROM_BAR0                                            (0x02BFE000)
#define BAND1_RX_OFFSET_FROM_BAR0                                                 (0x02400000)
#define BAND1_TX_OFFSET_FROM_BAR0                                                 (0x02600000)
#define BAND2_RX_OFFSET_FROM_BAR0                                                 (0x02800000)
#define BAND2_TX_OFFSET_FROM_BAR0                                                 (0x02A00000)
#define HOST_EX_MAP_CPU_OFFSET_FROM_BAR0                                          (0x04000000)
#define RF_EMULATOR_OFFSET_FROM_BAR0                                              (0x00F84000)
#define SHARED_RAM_GRAY_OFFSET_FROM_BAR0                                          (0x00600000)
#define SHARED_RAM_WHITE_OFFSET_FROM_BAR0                                         (0x006A0000)
#define SHARED_RAM_BLACK_OFFSET_FROM_BAR0                                         (0x006C0000)
#define B0_HOB_PAC_DUR_OFFSET_FROM_BAR0                                           (0x00000000)
#define HOB_PAC_DUR_OFFSET_FROM_BAR0                                              (0x00000000)
#define B1_HOB_PAC_DUR_OFFSET_FROM_BAR0                                           (0x00400000)
#define B2_HOB_PAC_DUR_OFFSET_FROM_BAR0                                           (0x00800000)
#define B0_HOB_MAC_BFM_OFFSET_FROM_BAR0                                           (0x00000200)
#define HOB_MAC_BFM_OFFSET_FROM_BAR0                                              (0x00000200)
#define B1_HOB_MAC_BFM_OFFSET_FROM_BAR0                                           (0x00400200)
#define B2_HOB_MAC_BFM_OFFSET_FROM_BAR0                                           (0x00800200)
#define B0_HOB_PAC_TIM_OFFSET_FROM_BAR0                                           (0x00000400)
#define HOB_PAC_TIM_OFFSET_FROM_BAR0                                              (0x00000400)
#define B1_HOB_PAC_TIM_OFFSET_FROM_BAR0                                           (0x00400400)
#define B2_HOB_PAC_TIM_OFFSET_FROM_BAR0                                           (0x00800400)
#define B0_HOB_PAC_DEL_OFFSET_FROM_BAR0                                           (0x00000800)
#define HOB_PAC_DEL_OFFSET_FROM_BAR0                                              (0x00000800)
#define B1_HOB_PAC_DEL_OFFSET_FROM_BAR0                                           (0x00400800)
#define B2_HOB_PAC_DEL_OFFSET_FROM_BAR0                                           (0x00800800)
#define B0_HOB_AUTO_RESP_OFFSET_FROM_BAR0                                         (0x00000A00)
#define HOB_AUTO_RESP_OFFSET_FROM_BAR0                                            (0x00000A00)
#define B1_HOB_AUTO_RESP_OFFSET_FROM_BAR0                                         (0x00400A00)
#define B2_HOB_AUTO_RESP_OFFSET_FROM_BAR0                                         (0x00800A00)
#define B0_HOB_TX_SELECTOR_OFFSET_FROM_BAR0                                       (0x00000C00)
#define HOB_TX_SELECTOR_OFFSET_FROM_BAR0                                          (0x00000C00)
#define B1_HOB_TX_SELECTOR_OFFSET_FROM_BAR0                                       (0x00400C00)
#define B2_HOB_TX_SELECTOR_OFFSET_FROM_BAR0                                       (0x00800C00)

 /* Modules Sizes */
#define CPU_IRAM_SIZE                                                             (0x00380000)
#define BOOT_ROM_SIZE                                                             (0x00010000)
#define SHARED_RAM_SIZE                                                           (0x000D0000)
#define DESCRIPTOR_RAM_RX_SIZE                                                    (0x00100000)
#define DESCRIPTOR_RAM_TX_SIZE                                                    (0x00100000)
#define DESCRIPTOR_RAM_RX_TX_SIZE                                                 (0x00100000)
#define PLAN_RAM_SIZE                                                             (0x00080000)
#define BA_ANALYZER_COUNTER_M01_SIZE                                              (0x00020000)
#define BA_ANALYZER_COUNTER_SIZE                                                  (0x00020000)
#define BA_ANALYZER_COUNTER_M2_SIZE                                               (0x00020000)
#define RX_PP_COUNTERS_M01_SIZE                                                   (0x00010000)
#define RX_PP_COUNTERS_SIZE                                                       (0x00010000)
#define RX_PP_COUNTERS_M2_SIZE                                                    (0x00010000)
#define RX_COORDINATOR_COUNTERS_M01_SIZE                                          (0x00010000)
#define RX_COORDINATOR_COUNTERS_SIZE                                              (0x00010000)
#define RX_COORDINATOR_COUNTERS_M2_SIZE                                           (0x00010000)
#define HOST_IF_COUNTERS_M01_SIZE                                                 (0x00010000)
#define HOST_IF_COUNTERS_SIZE                                                     (0x00010000)
#define HOST_IF_COUNTERS_M2_SIZE                                                  (0x00010000)
#define HOST_IF_QOS_COUNTERS_M01_SIZE                                             (0x00010000)
#define HOST_IF_QOS_COUNTERS_SIZE                                                 (0x00010000)
#define HOST_IF_QOS_COUNTERS_M2_SIZE                                              (0x00010000)
#define HOST_IF_AQM_STAT_SIZE                                                     (0x00020000)
#define EXT_RX_PP_COUNTERS_SIZE                                                   (0x00020000)
#define SYSTEM_IF_LOWER_B0_CPU_SIZE                                               (0x00004000)
#define SYSTEM_IF_LOWER_B1_CPU_SIZE                                               (0x00004000)
#define SYSTEM_IF_LOWER_B2_CPU_SIZE                                               (0x00004000)
#define SYSTEM_IF_UPPER_CPU_SIZE                                                  (0x00004000)
#define WLAN_DMA_LL_MEM_M01_SIZE                                                  (0x00005000)
#define WLAN_DMA_LL_MEM_SIZE                                                      (0x00005000)
#define WLAN_DMA_TX_M01_SIZE                                                      (0x00000800)
#define WLAN_DMA_TX_SIZE                                                          (0x00000800)
#define WLAN_DMA_RX_M01_SIZE                                                      (0x00000800)
#define WLAN_DMA_RX_SIZE                                                          (0x00000800)
#define WLAN_DMA_FW_M01_SIZE                                                      (0x00000800)
#define WLAN_DMA_FW_SIZE                                                          (0x00000800)
#define DMA_WRAPPER_M01_SIZE                                                      (0x00001000)
#define DMA_WRAPPER_SIZE                                                          (0x00001000)
#define WLAN_DMA_LL_MEM_M2_SIZE                                                   (0x00005000)
#define WLAN_DMA_TX_M2_SIZE                                                       (0x00000800)
#define WLAN_DMA_RX_M2_SIZE                                                       (0x00000800)
#define WLAN_DMA_FW_M2_SIZE                                                       (0x00000800)
#define DMA_WRAPPER_M2_SIZE                                                       (0x00001000)
#define DLM_M01_SIZE                                                              (0x0000C000)
#define DLM_SIZE                                                                  (0x0000C000)
#define QUEUE_MANAGER_M01_TX0_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_TX0_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_TX1_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_TX1_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_TX2_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_TX2_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_RX0_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_RX0_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_RX1_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_RX1_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_RX2_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_RX2_SIZE                                                    (0x00000400)
#define QUEUE_MANAGER_M01_HOST_SIZE                                               (0x00000400)
#define QUEUE_MANAGER_HOST_SIZE                                                   (0x00000400)
#define QUEUE_MANAGER_M01_LCPU0_SIZE                                              (0x00000400)
#define QUEUE_MANAGER_LCPU0_SIZE                                                  (0x00000400)
#define QUEUE_MANAGER_M01_LCPU1_SIZE                                              (0x00000400)
#define QUEUE_MANAGER_LCPU1_SIZE                                                  (0x00000400)
#define QUEUE_MANAGER_M01_LCPU2_SIZE                                              (0x00000400)
#define QUEUE_MANAGER_LCPU2_SIZE                                                  (0x00000400)
#define QUEUE_MANAGER_M01_UCPU_SIZE                                               (0x00000400)
#define QUEUE_MANAGER_UCPU_SIZE                                                   (0x00000400)
#define DLM_M2_SIZE                                                               (0x0000C000)
#define QUEUE_MANAGER_M2_TX0_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_TX1_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_TX2_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_RX0_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_RX1_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_RX2_SIZE                                                 (0x00000400)
#define QUEUE_MANAGER_M2_HOST_SIZE                                                (0x00000400)
#define QUEUE_MANAGER_M2_LCPU0_SIZE                                               (0x00000400)
#define QUEUE_MANAGER_M2_LCPU1_SIZE                                               (0x00000400)
#define QUEUE_MANAGER_M2_LCPU2_SIZE                                               (0x00000400)
#define QUEUE_MANAGER_M2_UCPU_SIZE                                                (0x00000400)
#define TOP_REGBLOCK_SIZE                                                         (0x00008000)
#define PCIE_PHY0_REGBLOCK_SIZE                                                   (0x00003000)
#define PCIE_DBI0_REGBLOCK_SIZE                                                   (0x00005000)
#define PVT_BLOCK_SIZE                                                            (0x00008000)
#define CPU_XBAR_STATUS_SIZE                                                      (0x00000400)
#define MAC_XBAR_PM_SIZE                                                          (0x00001000)
#define MAC_XBAR_STATUS_SIZE                                                      (0x00000800)
#define RX_PP_M01_SIZE                                                            (0x0000A000)
#define RX_PP_SIZE                                                                (0x0000A000)
#define RX_PP_REGS_M01_SIZE                                                       (0x00000800)
#define RX_PP_REGS_SIZE                                                           (0x00000800)
#define RX_PP_M2_SIZE                                                             (0x0000A000)
#define RX_PP_REGS_M2_SIZE                                                        (0x00000800)
#define DATA_LOGGER_SIZE                                                          (0x00001000)
#define PROT_DB_SIZE                                                              (0x00000200)
#define PROT_DB_SHADOW_MEM_SIZE                                                   (0x00000400)
#define TRAFFIC_INDICATOR_M01_SIZE                                                (0x00000800)
#define TRAFFIC_INDICATOR_SIZE                                                    (0x00000800)
#define TRAFFIC_INDICATOR_M2_SIZE                                                 (0x00000800)
#define VAP_ADDR_REG_M01_SIZE                                                     (0x00000400)
#define VAP_ADDR_REG_SIZE                                                         (0x00000400)
#define VAP_ADDR_REG_M2_SIZE                                                      (0x00000400)
#define MAC_GEN_REGFILE_SIZE                                                      (0x00001000)
#define TIM_GEN_SIZE                                                              (0x00001000)
#define HOST_IF_ACC_SIZE                                                          (0x00001000)
#define HOST_IF_REG_FILE_SIZE                                                     (0x00001000)
#define BSRC_CNT_MEM_M01_SIZE                                                     (0x00002000)
#define BSRC_CNT_MEM_SIZE                                                         (0x00002000)
#define BSRC_CNT_MEM_M2_SIZE                                                      (0x00002000)
#define BSRC_AGER_MEM_M01_SIZE                                                    (0x00001000)
#define BSRC_AGER_MEM_SIZE                                                        (0x00001000)
#define BSRC_AGER_MEM_M2_SIZE                                                     (0x00001000)
#define BEST_RU_DB_M01_SIZE                                                       (0x00008000)
#define BEST_RU_DB_SIZE                                                           (0x00008000)
#define BEST_RU_DB_M2_SIZE                                                        (0x00008000)
#define RETRY_STATE_DB_M01_SIZE                                                   (0x00004000)
#define RETRY_STATE_DB_SIZE                                                       (0x00004000)
#define RETRY_STATE_DB_M2_SIZE                                                    (0x00004000)
#define RETRY_POINTER_DB_M01_SIZE                                                 (0x00020000)
#define RETRY_POINTER_DB_SIZE                                                     (0x00020000)
#define RETRY_POINTER_DB_M2_SIZE                                                  (0x00020000)
#define MAC_HOSTIF_UC_INTERNAL_SIZE                                               (0x00020000)
#define MAC_HOSTIF_UC_CLUSTER_SIZE                                                (0x00004000)
#define B0_MAC_GENRISC_TX_IRAM_SIZE                                               (0x00010000)
#define MAC_GENRISC_TX_IRAM_SIZE                                                  (0x00010000)
#define B1_MAC_GENRISC_TX_IRAM_SIZE                                               (0x00010000)
#define B2_MAC_GENRISC_TX_IRAM_SIZE                                               (0x00010000)
#define B0_MAC_GENRISC_TX_SPRAM_SIZE                                              (0x00001000)
#define MAC_GENRISC_TX_SPRAM_SIZE                                                 (0x00001000)
#define B1_MAC_GENRISC_TX_SPRAM_SIZE                                              (0x00001000)
#define B2_MAC_GENRISC_TX_SPRAM_SIZE                                              (0x00001000)
#define B0_MAC_GENRISC_TX_REG_SIZE                                                (0x00000400)
#define MAC_GENRISC_TX_REG_SIZE                                                   (0x00000400)
#define B1_MAC_GENRISC_TX_REG_SIZE                                                (0x00000400)
#define B2_MAC_GENRISC_TX_REG_SIZE                                                (0x00000400)
#define B0_MAC_GENRISC_TX_STD_SIZE                                                (0x00002000)
#define MAC_GENRISC_TX_STD_SIZE                                                   (0x00002000)
#define B1_MAC_GENRISC_TX_STD_SIZE                                                (0x00002000)
#define B2_MAC_GENRISC_TX_STD_SIZE                                                (0x00002000)
#define B0_MAC_GENRISC_RX_IRAM_SIZE                                               (0x00004000)
#define MAC_GENRISC_RX_IRAM_SIZE                                                  (0x00004000)
#define B1_MAC_GENRISC_RX_IRAM_SIZE                                               (0x00004000)
#define B2_MAC_GENRISC_RX_IRAM_SIZE                                               (0x00004000)
#define B0_MAC_GENRISC_RX_SPRAM_SIZE                                              (0x00001000)
#define MAC_GENRISC_RX_SPRAM_SIZE                                                 (0x00001000)
#define B1_MAC_GENRISC_RX_SPRAM_SIZE                                              (0x00001000)
#define B2_MAC_GENRISC_RX_SPRAM_SIZE                                              (0x00001000)
#define B0_MAC_GENRISC_RX_REG_SIZE                                                (0x00000400)
#define MAC_GENRISC_RX_REG_SIZE                                                   (0x00000400)
#define B1_MAC_GENRISC_RX_REG_SIZE                                                (0x00000400)
#define B2_MAC_GENRISC_RX_REG_SIZE                                                (0x00000400)
#define B0_TX_SENDER_UC_CLUSTER_SIZE                                              (0x00004000)
#define TX_SENDER_UC_CLUSTER_SIZE                                                 (0x00004000)
#define B1_TX_SENDER_UC_CLUSTER_SIZE                                              (0x00004000)
#define B2_TX_SENDER_UC_CLUSTER_SIZE                                              (0x00004000)
#define B0_RX_UC_CLUSTER_SIZE                                                     (0x00004000)
#define RX_UC_CLUSTER_SIZE                                                        (0x00004000)
#define B1_RX_UC_CLUSTER_SIZE                                                     (0x00004000)
#define B2_RX_UC_CLUSTER_SIZE                                                     (0x00004000)
#define B0_TX_SENDER_SINGLE_UC_SIZE                                               (0x00020000)
#define TX_SENDER_SINGLE_UC_SIZE                                                  (0x00020000)
#define B1_TX_SENDER_SINGLE_UC_SIZE                                               (0x00020000)
#define B2_TX_SENDER_SINGLE_UC_SIZE                                               (0x00020000)
#define B0_RX_SINGLE_UC_SIZE                                                      (0x00020000)
#define RX_SINGLE_UC_SIZE                                                         (0x00020000)
#define B1_RX_SINGLE_UC_SIZE                                                      (0x00020000)
#define B2_RX_SINGLE_UC_SIZE                                                      (0x00020000)
#define B0_MAC_HT_EXTENSIONS_SIZE                                                 (0x00001000)
#define MAC_HT_EXTENSIONS_SIZE                                                    (0x00001000)
#define B1_MAC_HT_EXTENSIONS_SIZE                                                 (0x00001000)
#define B2_MAC_HT_EXTENSIONS_SIZE                                                 (0x00001000)
#define B0_RXH_RD_ACC_SIZE                                                        (0x00000200)
#define RXH_RD_ACC_SIZE                                                           (0x00000200)
#define B1_RXH_RD_ACC_SIZE                                                        (0x00000200)
#define B2_RXH_RD_ACC_SIZE                                                        (0x00000200)
#define B0_AUTO_RESP_MEMS_SIZE                                                    (0x00003000)
#define AUTO_RESP_MEMS_SIZE                                                       (0x00003000)
#define B1_AUTO_RESP_MEMS_SIZE                                                    (0x00003000)
#define B2_AUTO_RESP_MEMS_SIZE                                                    (0x00003000)
#define B0_AUTO_RESP_REGS_SIZE                                                    (0x00000800)
#define AUTO_RESP_REGS_SIZE                                                       (0x00000800)
#define B1_AUTO_RESP_REGS_SIZE                                                    (0x00000800)
#define B2_AUTO_RESP_REGS_SIZE                                                    (0x00000800)
#define B0_PAC_DUR_SIZE                                                           (0x00000100)
#define PAC_DUR_SIZE                                                              (0x00000100)
#define B1_PAC_DUR_SIZE                                                           (0x00000100)
#define B2_PAC_DUR_SIZE                                                           (0x00000100)
#define B0_PAC_TIM_SIZE                                                           (0x00000100)
#define PAC_TIM_SIZE                                                              (0x00000100)
#define B1_PAC_TIM_SIZE                                                           (0x00000100)
#define B2_PAC_TIM_SIZE                                                           (0x00000100)
#define B0_PAC_EXTRAPOLATOR_SIZE                                                  (0x00000200)
#define PAC_EXTRAPOLATOR_SIZE                                                     (0x00000200)
#define B1_PAC_EXTRAPOLATOR_SIZE                                                  (0x00000200)
#define B2_PAC_EXTRAPOLATOR_SIZE                                                  (0x00000200)
#define B0_MAC_BEACON_SIZE                                                        (0x00000400)
#define MAC_BEACON_SIZE                                                           (0x00000400)
#define B1_MAC_BEACON_SIZE                                                        (0x00000400)
#define B2_MAC_BEACON_SIZE                                                        (0x00000400)
#define B0_TXH_TXC_SIZE                                                           (0x00000100)
#define TXH_TXC_SIZE                                                              (0x00000100)
#define B1_TXH_TXC_SIZE                                                           (0x00000100)
#define B2_TXH_TXC_SIZE                                                           (0x00000100)
#define B0_TXH_NTD_SIZE                                                           (0x00000100)
#define TXH_NTD_SIZE                                                              (0x00000100)
#define B1_TXH_NTD_SIZE                                                           (0x00000100)
#define B2_TXH_NTD_SIZE                                                           (0x00000100)
#define B0_PS_SETTING_SIZE                                                        (0x00000100)
#define PS_SETTING_SIZE                                                           (0x00000100)
#define B1_PS_SETTING_SIZE                                                        (0x00000100)
#define B2_PS_SETTING_SIZE                                                        (0x00000100)
#define B0_PAC_DELIA_REG_SIZE                                                     (0x00000200)
#define PAC_DELIA_REG_SIZE                                                        (0x00000200)
#define B1_PAC_DELIA_REG_SIZE                                                     (0x00000200)
#define B2_PAC_DELIA_REG_SIZE                                                     (0x00000200)
#define B0_PAC_DELIA_MEM_SIZE                                                     (0x00000200)
#define PAC_DELIA_MEM_SIZE                                                        (0x00000200)
#define B1_PAC_DELIA_MEM_SIZE                                                     (0x00000200)
#define B2_PAC_DELIA_MEM_SIZE                                                     (0x00000200)
#define B0_DUR_AUTO_RATE_MEM_SIZE                                                 (0x00000800)
#define DUR_AUTO_RATE_MEM_SIZE                                                    (0x00000800)
#define B1_DUR_AUTO_RATE_MEM_SIZE                                                 (0x00000800)
#define B2_DUR_AUTO_RATE_MEM_SIZE                                                 (0x00000800)
#define B0_PAC_RXC_SIZE                                                           (0x00000400)
#define PAC_RXC_SIZE                                                              (0x00000400)
#define B1_PAC_RXC_SIZE                                                           (0x00000400)
#define B2_PAC_RXC_SIZE                                                           (0x00000400)
#define B0_RXF_MEM_SIZE                                                           (0x00000C00)
#define RXF_MEM_SIZE                                                              (0x00000C00)
#define B1_RXF_MEM_SIZE                                                           (0x00000C00)
#define B2_RXF_MEM_SIZE                                                           (0x00000C00)
#define B0_MAC_SEC_MEM_SIZE                                                       (0x00004000)
#define MAC_SEC_MEM_SIZE                                                          (0x00004000)
#define B1_MAC_SEC_MEM_SIZE                                                       (0x00004000)
#define B2_MAC_SEC_MEM_SIZE                                                       (0x00004000)
#define B0_MAC_WEP_SIZE                                                           (0x00000400)
#define MAC_WEP_SIZE                                                              (0x00000400)
#define B1_MAC_WEP_SIZE                                                           (0x00000400)
#define B2_MAC_WEP_SIZE                                                           (0x00000400)
#define B0_PRE_AGG_SIZE                                                           (0x00008000)
#define PRE_AGG_SIZE                                                              (0x00008000)
#define B1_PRE_AGG_SIZE                                                           (0x00008000)
#define B2_PRE_AGG_SIZE                                                           (0x00008000)
#define B0_MLD_DB_SIZE                                                            (0x00002000)
#define MLD_DB_SIZE                                                               (0x00002000)
#define B1_MLD_DB_SIZE                                                            (0x00002000)
#define B2_MLD_DB_SIZE                                                            (0x00002000)
#define B0_TX_SEQUENCER_SIZE                                                      (0x00000800)
#define TX_SEQUENCER_SIZE                                                         (0x00000800)
#define B1_TX_SEQUENCER_SIZE                                                      (0x00000800)
#define B2_TX_SEQUENCER_SIZE                                                      (0x00000800)
#define B0_TX_PD_ACC_MEM_SIZE                                                     (0x00000600)
#define TX_PD_ACC_MEM_SIZE                                                        (0x00000600)
#define B1_TX_PD_ACC_MEM_SIZE                                                     (0x00000600)
#define B2_TX_PD_ACC_MEM_SIZE                                                     (0x00000600)
#define B0_TX_PD_ACC_REG_SIZE                                                     (0x00000200)
#define TX_PD_ACC_REG_SIZE                                                        (0x00000200)
#define B1_TX_PD_ACC_REG_SIZE                                                     (0x00000200)
#define B2_TX_PD_ACC_REG_SIZE                                                     (0x00000200)
#define B0_TF_GENERATOR_MEM_SIZE                                                  (0x00000600)
#define TF_GENERATOR_MEM_SIZE                                                     (0x00000600)
#define B1_TF_GENERATOR_MEM_SIZE                                                  (0x00000600)
#define B2_TF_GENERATOR_MEM_SIZE                                                  (0x00000600)
#define B0_TF_GENERATOR_REG_SIZE                                                  (0x00000200)
#define TF_GENERATOR_REG_SIZE                                                     (0x00000200)
#define B1_TF_GENERATOR_REG_SIZE                                                  (0x00000200)
#define B2_TF_GENERATOR_REG_SIZE                                                  (0x00000200)
#define B0_TF_DECODER_REG_SIZE                                                    (0x00000200)
#define TF_DECODER_REG_SIZE                                                       (0x00000200)
#define B1_TF_DECODER_REG_SIZE                                                    (0x00000200)
#define B2_TF_DECODER_REG_SIZE                                                    (0x00000200)
#define B0_TF_DECODER_MEM_SIZE                                                    (0x00000600)
#define TF_DECODER_MEM_SIZE                                                       (0x00000600)
#define B1_TF_DECODER_MEM_SIZE                                                    (0x00000600)
#define B2_TF_DECODER_MEM_SIZE                                                    (0x00000600)
#define B0_RAB_SIZE                                                               (0x00001000)
#define RAB_SIZE                                                                  (0x00001000)
#define B1_RAB_SIZE                                                               (0x00001000)
#define B2_RAB_SIZE                                                               (0x00001000)
#define B0_MAC_ADDR2INDEX_SIZE                                                    (0x00001000)
#define MAC_ADDR2INDEX_SIZE                                                       (0x00001000)
#define B1_MAC_ADDR2INDEX_SIZE                                                    (0x00001000)
#define B2_MAC_ADDR2INDEX_SIZE                                                    (0x00001000)
#define B0_TX_HANDLER_SIZE                                                        (0x00001000)
#define TX_HANDLER_SIZE                                                           (0x00001000)
#define B1_TX_HANDLER_SIZE                                                        (0x00001000)
#define B2_TX_HANDLER_SIZE                                                        (0x00001000)
#define B0_BA_ANALYZER_SIZE                                                       (0x00001000)
#define BA_ANALYZER_SIZE                                                          (0x00001000)
#define B1_BA_ANALYZER_SIZE                                                       (0x00001000)
#define B2_BA_ANALYZER_SIZE                                                       (0x00001000)
#define B0_TWT_STATION_MEM_SIZE                                                   (0x00001000)
#define TWT_STATION_MEM_SIZE                                                      (0x00001000)
#define B1_TWT_STATION_MEM_SIZE                                                   (0x00001000)
#define B2_TWT_STATION_MEM_SIZE                                                   (0x00001000)
#define B0_TWT_GROUP_MEM_SIZE                                                     (0x00000F00)
#define TWT_GROUP_MEM_SIZE                                                        (0x00000F00)
#define B1_TWT_GROUP_MEM_SIZE                                                     (0x00000F00)
#define B2_TWT_GROUP_MEM_SIZE                                                     (0x00000F00)
#define B0_TWT_REG_SIZE                                                           (0x00000100)
#define TWT_REG_SIZE                                                              (0x00000100)
#define B1_TWT_REG_SIZE                                                           (0x00000100)
#define B2_TWT_REG_SIZE                                                           (0x00000100)
#define B0_TX_SELECTOR_SIZE                                                       (0x00003000)
#define TX_SELECTOR_SIZE                                                          (0x00003000)
#define B1_TX_SELECTOR_SIZE                                                       (0x00003000)
#define B2_TX_SELECTOR_SIZE                                                       (0x00003000)
#define B0_RX_CLASSIFIER_SIZE                                                     (0x00001000)
#define RX_CLASSIFIER_SIZE                                                        (0x00001000)
#define B1_RX_CLASSIFIER_SIZE                                                     (0x00001000)
#define B2_RX_CLASSIFIER_SIZE                                                     (0x00001000)
#define B0_TIM_IE_SIZE                                                            (0x00000400)
#define TIM_IE_SIZE                                                               (0x00000400)
#define B1_TIM_IE_SIZE                                                            (0x00000400)
#define B2_TIM_IE_SIZE                                                            (0x00000400)
#define B0_RX_MSDU_PARSER_SIZE                                                    (0x00000200)
#define RX_MSDU_PARSER_SIZE                                                       (0x00000200)
#define B1_RX_MSDU_PARSER_SIZE                                                    (0x00000200)
#define B2_RX_MSDU_PARSER_SIZE                                                    (0x00000200)
#define B0_RX_COORDINATOR_SIZE                                                    (0x00000200)
#define RX_COORDINATOR_SIZE                                                       (0x00000200)
#define B1_RX_COORDINATOR_SIZE                                                    (0x00000200)
#define B2_RX_COORDINATOR_SIZE                                                    (0x00000200)
#define B0_FRAME_CLASS_VIO_BUFFER_SIZE                                            (0x00000200)
#define FRAME_CLASS_VIO_BUFFER_SIZE                                               (0x00000200)
#define B1_FRAME_CLASS_VIO_BUFFER_SIZE                                            (0x00000200)
#define B2_FRAME_CLASS_VIO_BUFFER_SIZE                                            (0x00000200)
#define B0_OTFA_MEM2_SIZE                                                         (0x00001800)
#define OTFA_MEM2_SIZE                                                            (0x00001800)
#define B1_OTFA_MEM2_SIZE                                                         (0x00001800)
#define B2_OTFA_MEM2_SIZE                                                         (0x00001800)
#define B0_OTFA_REG_SIZE                                                          (0x00000800)
#define OTFA_REG_SIZE                                                             (0x00000800)
#define B1_OTFA_REG_SIZE                                                          (0x00000800)
#define B2_OTFA_REG_SIZE                                                          (0x00000800)
#define B0_MAC_TX_HC_SIZE                                                         (0x00000080)
#define MAC_TX_HC_SIZE                                                            (0x00000080)
#define B1_MAC_TX_HC_SIZE                                                         (0x00000080)
#define B2_MAC_TX_HC_SIZE                                                         (0x00000080)
#define B0_BSRC_REGS_SIZE                                                         (0x00000200)
#define BSRC_REGS_SIZE                                                            (0x00000200)
#define B1_BSRC_REGS_SIZE                                                         (0x00000200)
#define B2_BSRC_REGS_SIZE                                                         (0x00000200)
#define B0_MAC_BFM_SIZE                                                           (0x00001000)
#define MAC_BFM_SIZE                                                              (0x00001000)
#define B1_MAC_BFM_SIZE                                                           (0x00001000)
#define B2_MAC_BFM_SIZE                                                           (0x00001000)
#define B0_UL_POST_RX_SIZE                                                        (0x00000100)
#define UL_POST_RX_SIZE                                                           (0x00000100)
#define B1_UL_POST_RX_SIZE                                                        (0x00000100)
#define B2_UL_POST_RX_SIZE                                                        (0x00000100)
#define B0_BESTRU_SIZE                                                            (0x00000800)
#define BESTRU_SIZE                                                               (0x00000800)
#define B1_BESTRU_SIZE                                                            (0x00000800)
#define B2_BESTRU_SIZE                                                            (0x00000800)
#define B0_BUF_STATUS_REPORT_SIZE                                                 (0x00004000)
#define BUF_STATUS_REPORT_SIZE                                                    (0x00004000)
#define B1_BUF_STATUS_REPORT_SIZE                                                 (0x00004000)
#define B2_BUF_STATUS_REPORT_SIZE                                                 (0x00004000)
#define B0_MPDU_DESC_MEM_SIZE                                                     (0x00080000)
#define MPDU_DESC_MEM_SIZE                                                        (0x00080000)
#define B1_MPDU_DESC_MEM_SIZE                                                     (0x00080000)
#define B2_MPDU_DESC_MEM_SIZE                                                     (0x00080000)
#define B0_PHY_TEST_BUS_OR_DUMMY_PHY_SIZE                                         (0x00010000)
#define PHY_TEST_BUS_OR_DUMMY_PHY_SIZE                                            (0x00010000)
#define B1_PHY_TEST_BUS_OR_DUMMY_PHY_SIZE                                         (0x00010000)
#define B2_PHY_TEST_BUS_OR_DUMMY_PHY_SIZE                                         (0x00010000)
#define B0_PHY_RISC_SIZE                                                          (0x00010000)
#define PHY_RISC_SIZE                                                             (0x00010000)
#define B1_PHY_RISC_SIZE                                                          (0x00010000)
#define B2_PHY_RISC_SIZE                                                          (0x00010000)
#define B0_PHY_RX_TD_SIZE                                                         (0x00004000)
#define PHY_RX_TD_SIZE                                                            (0x00004000)
#define B1_PHY_RX_TD_SIZE                                                         (0x00004000)
#define B2_PHY_RX_TD_SIZE                                                         (0x00004000)
#define B0_PHY_RX_TD_DEBUG_SIZE                                                   (0x00008000)
#define PHY_RX_TD_DEBUG_SIZE                                                      (0x00008000)
#define B1_PHY_RX_TD_DEBUG_SIZE                                                   (0x00008000)
#define B2_PHY_RX_TD_DEBUG_SIZE                                                   (0x00008000)
#define B0_PHY_RX_BE_SIZE                                                         (0x00004000)
#define PHY_RX_BE_SIZE                                                            (0x00004000)
#define B1_PHY_RX_BE_SIZE                                                         (0x00004000)
#define B2_PHY_RX_BE_SIZE                                                         (0x00004000)
#define B0_PHY_RX_FD_SIZE                                                         (0x0000C000)
#define PHY_RX_FD_SIZE                                                            (0x0000C000)
#define B1_PHY_RX_FD_SIZE                                                         (0x0000C000)
#define B2_PHY_RX_FD_SIZE                                                         (0x0000C000)
#define B0_PHY_RX_FD_DEBUG_SIZE                                                   (0x00010000)
#define PHY_RX_FD_DEBUG_SIZE                                                      (0x00010000)
#define B1_PHY_RX_FD_DEBUG_SIZE                                                   (0x00010000)
#define B2_PHY_RX_FD_DEBUG_SIZE                                                   (0x00010000)
#define B0_PHY_TX_SIZE                                                            (0x00004000)
#define PHY_TX_SIZE                                                               (0x00004000)
#define B1_PHY_TX_SIZE                                                            (0x00004000)
#define B2_PHY_TX_SIZE                                                            (0x00004000)
#define B0_UP_PHY_RX_FD_SIZE                                                      (0x0000C000)
#define UP_PHY_RX_FD_SIZE                                                         (0x0000C000)
#define B1_UP_PHY_RX_FD_SIZE                                                      (0x0000C000)
#define B2_UP_PHY_RX_FD_SIZE                                                      (0x0000C000)
#define B0_UP_PHY_RX_FD_DEBUG_SIZE                                                (0x00010000)
#define UP_PHY_RX_FD_DEBUG_SIZE                                                   (0x00010000)
#define B1_UP_PHY_RX_FD_DEBUG_SIZE                                                (0x00010000)
#define B2_UP_PHY_RX_FD_DEBUG_SIZE                                                (0x00010000)
#define B0_UP_PHY_TX_SIZE                                                         (0x00004000)
#define UP_PHY_TX_SIZE                                                            (0x00004000)
#define B1_UP_PHY_TX_SIZE                                                         (0x00004000)
#define B2_UP_PHY_TX_SIZE                                                         (0x00004000)
#define B0_PHY_DFS_RISC_SIZE                                                      (0x00010000)
#define PHY_DFS_RISC_SIZE                                                         (0x00010000)
#define B1_PHY_DFS_RISC_SIZE                                                      (0x00010000)
#define B2_PHY_DFS_RISC_SIZE                                                      (0x00010000)
#define B0_PHY_DFS_RX_TD_SIZE                                                     (0x00008000)
#define PHY_DFS_RX_TD_SIZE                                                        (0x00008000)
#define B1_PHY_DFS_RX_TD_SIZE                                                     (0x00008000)
#define B2_PHY_DFS_RX_TD_SIZE                                                     (0x00008000)
#define B0_PHY_DFS_RXTD_ANT_SIZE                                                  (0x00004000)
#define PHY_DFS_RXTD_ANT_SIZE                                                     (0x00004000)
#define B1_PHY_DFS_RXTD_ANT_SIZE                                                  (0x00004000)
#define B2_PHY_DFS_RXTD_ANT_SIZE                                                  (0x00004000)
#define B0_PHY_DFS_AGC_ANT_SIZE                                                   (0x00004000)
#define PHY_DFS_AGC_ANT_SIZE                                                      (0x00004000)
#define B1_PHY_DFS_AGC_ANT_SIZE                                                   (0x00004000)
#define B2_PHY_DFS_AGC_ANT_SIZE                                                   (0x00004000)
#define B0_PHY_RXTD_ANT0_SIZE                                                     (0x00004000)
#define PHY_RXTD_ANT0_SIZE                                                        (0x00004000)
#define B1_PHY_RXTD_ANT0_SIZE                                                     (0x00004000)
#define B2_PHY_RXTD_ANT0_SIZE                                                     (0x00004000)
#define B0_PHY_AGC_ANT0_SIZE                                                      (0x00004000)
#define PHY_AGC_ANT0_SIZE                                                         (0x00004000)
#define B1_PHY_AGC_ANT0_SIZE                                                      (0x00004000)
#define B2_PHY_AGC_ANT0_SIZE                                                      (0x00004000)
#define B0_PHY_TXTD_ANT0_SIZE                                                     (0x00008000)
#define PHY_TXTD_ANT0_SIZE                                                        (0x00008000)
#define B1_PHY_TXTD_ANT0_SIZE                                                     (0x00008000)
#define B2_PHY_TXTD_ANT0_SIZE                                                     (0x00008000)
#define B0_PHY_RXTD_ANT1_SIZE                                                     (0x00004000)
#define PHY_RXTD_ANT1_SIZE                                                        (0x00004000)
#define B1_PHY_RXTD_ANT1_SIZE                                                     (0x00004000)
#define B2_PHY_RXTD_ANT1_SIZE                                                     (0x00004000)
#define B0_PHY_AGC_ANT1_SIZE                                                      (0x00004000)
#define PHY_AGC_ANT1_SIZE                                                         (0x00004000)
#define B1_PHY_AGC_ANT1_SIZE                                                      (0x00004000)
#define B2_PHY_AGC_ANT1_SIZE                                                      (0x00004000)
#define B0_PHY_TXTD_ANT1_SIZE                                                     (0x00008000)
#define PHY_TXTD_ANT1_SIZE                                                        (0x00008000)
#define B1_PHY_TXTD_ANT1_SIZE                                                     (0x00008000)
#define B2_PHY_TXTD_ANT1_SIZE                                                     (0x00008000)
#define B0_PHY_RXTD_ANT2_SIZE                                                     (0x00004000)
#define PHY_RXTD_ANT2_SIZE                                                        (0x00004000)
#define B1_PHY_RXTD_ANT2_SIZE                                                     (0x00004000)
#define B2_PHY_RXTD_ANT2_SIZE                                                     (0x00004000)
#define B0_PHY_AGC_ANT2_SIZE                                                      (0x00004000)
#define PHY_AGC_ANT2_SIZE                                                         (0x00004000)
#define B1_PHY_AGC_ANT2_SIZE                                                      (0x00004000)
#define B2_PHY_AGC_ANT2_SIZE                                                      (0x00004000)
#define B0_PHY_TXTD_ANT2_SIZE                                                     (0x00008000)
#define PHY_TXTD_ANT2_SIZE                                                        (0x00008000)
#define B1_PHY_TXTD_ANT2_SIZE                                                     (0x00008000)
#define B2_PHY_TXTD_ANT2_SIZE                                                     (0x00008000)
#define B0_PHY_RXTD_ANT3_SIZE                                                     (0x00004000)
#define PHY_RXTD_ANT3_SIZE                                                        (0x00004000)
#define B1_PHY_RXTD_ANT3_SIZE                                                     (0x00004000)
#define B2_PHY_RXTD_ANT3_SIZE                                                     (0x00004000)
#define B0_PHY_AGC_ANT3_SIZE                                                      (0x00004000)
#define PHY_AGC_ANT3_SIZE                                                         (0x00004000)
#define B1_PHY_AGC_ANT3_SIZE                                                      (0x00004000)
#define B2_PHY_AGC_ANT3_SIZE                                                      (0x00004000)
#define B0_PHY_TXTD_ANT3_SIZE                                                     (0x00008000)
#define PHY_TXTD_ANT3_SIZE                                                        (0x00008000)
#define B1_PHY_TXTD_ANT3_SIZE                                                     (0x00008000)
#define B2_PHY_TXTD_ANT3_SIZE                                                     (0x00008000)
#define B0_UP_PHY_RXTD_ANT0_SIZE                                                  (0x00004000)
#define UP_PHY_RXTD_ANT0_SIZE                                                     (0x00004000)
#define B1_UP_PHY_RXTD_ANT0_SIZE                                                  (0x00004000)
#define B2_UP_PHY_RXTD_ANT0_SIZE                                                  (0x00004000)
#define B0_UP_PHY_AGC_ANT0_SIZE                                                   (0x00004000)
#define UP_PHY_AGC_ANT0_SIZE                                                      (0x00004000)
#define B1_UP_PHY_AGC_ANT0_SIZE                                                   (0x00004000)
#define B2_UP_PHY_AGC_ANT0_SIZE                                                   (0x00004000)
#define B0_UP_PHY_TXTD_ANT0_SIZE                                                  (0x00008000)
#define UP_PHY_TXTD_ANT0_SIZE                                                     (0x00008000)
#define B1_UP_PHY_TXTD_ANT0_SIZE                                                  (0x00008000)
#define B2_UP_PHY_TXTD_ANT0_SIZE                                                  (0x00008000)
#define B0_UP_PHY_RXTD_ANT1_SIZE                                                  (0x00004000)
#define UP_PHY_RXTD_ANT1_SIZE                                                     (0x00004000)
#define B1_UP_PHY_RXTD_ANT1_SIZE                                                  (0x00004000)
#define B2_UP_PHY_RXTD_ANT1_SIZE                                                  (0x00004000)
#define B0_UP_PHY_AGC_ANT1_SIZE                                                   (0x00004000)
#define UP_PHY_AGC_ANT1_SIZE                                                      (0x00004000)
#define B1_UP_PHY_AGC_ANT1_SIZE                                                   (0x00004000)
#define B2_UP_PHY_AGC_ANT1_SIZE                                                   (0x00004000)
#define B0_UP_PHY_TXTD_ANT1_SIZE                                                  (0x00008000)
#define UP_PHY_TXTD_ANT1_SIZE                                                     (0x00008000)
#define B1_UP_PHY_TXTD_ANT1_SIZE                                                  (0x00008000)
#define B2_UP_PHY_TXTD_ANT1_SIZE                                                  (0x00008000)
#define B0_UP_PHY_RXTD_ANT2_SIZE                                                  (0x00004000)
#define UP_PHY_RXTD_ANT2_SIZE                                                     (0x00004000)
#define B1_UP_PHY_RXTD_ANT2_SIZE                                                  (0x00004000)
#define B2_UP_PHY_RXTD_ANT2_SIZE                                                  (0x00004000)
#define B0_UP_PHY_AGC_ANT2_SIZE                                                   (0x00004000)
#define UP_PHY_AGC_ANT2_SIZE                                                      (0x00004000)
#define B1_UP_PHY_AGC_ANT2_SIZE                                                   (0x00004000)
#define B2_UP_PHY_AGC_ANT2_SIZE                                                   (0x00004000)
#define B0_UP_PHY_TXTD_ANT2_SIZE                                                  (0x00008000)
#define UP_PHY_TXTD_ANT2_SIZE                                                     (0x00008000)
#define B1_UP_PHY_TXTD_ANT2_SIZE                                                  (0x00008000)
#define B2_UP_PHY_TXTD_ANT2_SIZE                                                  (0x00008000)
#define B0_UP_PHY_RXTD_ANT3_SIZE                                                  (0x00004000)
#define UP_PHY_RXTD_ANT3_SIZE                                                     (0x00004000)
#define B1_UP_PHY_RXTD_ANT3_SIZE                                                  (0x00004000)
#define B2_UP_PHY_RXTD_ANT3_SIZE                                                  (0x00004000)
#define B0_UP_PHY_AGC_ANT3_SIZE                                                   (0x00004000)
#define UP_PHY_AGC_ANT3_SIZE                                                      (0x00004000)
#define B1_UP_PHY_AGC_ANT3_SIZE                                                   (0x00004000)
#define B2_UP_PHY_AGC_ANT3_SIZE                                                   (0x00004000)
#define B0_UP_PHY_TXTD_ANT3_SIZE                                                  (0x00008000)
#define UP_PHY_TXTD_ANT3_SIZE                                                     (0x00008000)
#define B1_UP_PHY_TXTD_ANT3_SIZE                                                  (0x00008000)
#define B2_UP_PHY_TXTD_ANT3_SIZE                                                  (0x00008000)
#define BAND1_INTERNAL_SIZE                                                       (0x00400000)
#define BAND2_INTERNAL_SIZE                                                       (0x00400000)
#define B0_RX_CIRCULAR_SIZE                                                       (0x001FE000)
#define RX_CIRCULAR_SIZE                                                          (0x001FE000)
#define B1_RX_CIRCULAR_SIZE                                                       (0x001FE000)
#define B2_RX_CIRCULAR_SIZE                                                       (0x001FE000)
#define B0_RX_PHY_STATUS_SIZE                                                     (0x00001000)
#define RX_PHY_STATUS_SIZE                                                        (0x00001000)
#define B1_RX_PHY_STATUS_SIZE                                                     (0x00001000)
#define B2_RX_PHY_STATUS_SIZE                                                     (0x00001000)
#define B0_TX_CIRCULAR_SIZE                                                       (0x001FE000)
#define TX_CIRCULAR_SIZE                                                          (0x001FE000)
#define B1_TX_CIRCULAR_SIZE                                                       (0x001FE000)
#define B2_TX_CIRCULAR_SIZE                                                       (0x001FE000)
#define B0_TX_RECIPES_SIZE                                                        (0x00001400)
#define TX_RECIPES_SIZE                                                           (0x00001400)
#define B1_TX_RECIPES_SIZE                                                        (0x00001400)
#define B2_TX_RECIPES_SIZE                                                        (0x00001400)
#define BAND1_RX_SIZE                                                             (0x00200000)
#define BAND1_TX_SIZE                                                             (0x00200000)
#define BAND2_RX_SIZE                                                             (0x00200000)
#define BAND2_TX_SIZE                                                             (0x00200000)
#define HOST_EX_MAP_CPU_SIZE                                                      (0x04000000)
#define RF_EMULATOR_SIZE                                                          (0x00004000)
#define SHARED_RAM_GRAY_SIZE                                                      (0x000A0000)
#define SHARED_RAM_WHITE_SIZE                                                     (0x00020000)
#define SHARED_RAM_BLACK_SIZE                                                     (0x00010000)
#define B0_HOB_PAC_DUR_SIZE                                                       (0x200)
#define HOB_PAC_DUR_SIZE                                                          (0x200)
#define B1_HOB_PAC_DUR_SIZE                                                       (0x200)
#define B2_HOB_PAC_DUR_SIZE                                                       (0x200)
#define B0_HOB_MAC_BFM_SIZE                                                       (0x200)
#define HOB_MAC_BFM_SIZE                                                          (0x200)
#define B1_HOB_MAC_BFM_SIZE                                                       (0x200)
#define B2_HOB_MAC_BFM_SIZE                                                       (0x200)
#define B0_HOB_PAC_TIM_SIZE                                                       (0x200)
#define HOB_PAC_TIM_SIZE                                                          (0x200)
#define B1_HOB_PAC_TIM_SIZE                                                       (0x200)
#define B2_HOB_PAC_TIM_SIZE                                                       (0x200)
#define B0_HOB_PAC_DEL_SIZE                                                       (0x200)
#define HOB_PAC_DEL_SIZE                                                          (0x200)
#define B1_HOB_PAC_DEL_SIZE                                                       (0x200)
#define B2_HOB_PAC_DEL_SIZE                                                       (0x200)
#define B0_HOB_AUTO_RESP_SIZE                                                     (0x200)
#define HOB_AUTO_RESP_SIZE                                                        (0x200)
#define B1_HOB_AUTO_RESP_SIZE                                                     (0x200)
#define B2_HOB_AUTO_RESP_SIZE                                                     (0x200)
#define B0_HOB_TX_SELECTOR_SIZE                                                   (0x200)
#define HOB_TX_SELECTOR_SIZE                                                      (0x200)
#define B1_HOB_TX_SELECTOR_SIZE                                                   (0x200)
#define B2_HOB_TX_SELECTOR_SIZE                                                   (0x200)
#define REDUCED_DESCRIPTOR_RAM_SIZE                                               (0x00080000)
#define REDUCED_CPU_IRAM_SIZE                                                     (0x001C0000)

 /* auto generated from wave700_rams_by_modules_ver2.0.0.xlsm, sheet gen6_wlanip_rams_by_modules From ARC Perspective*/
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM                                    (B0_MAC_GENRISC_RX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1033_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM                                    (B0_MAC_GENRISC_TX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1026_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM                                    (B1_MAC_GENRISC_RX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11033_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM                                    (B1_MAC_GENRISC_TX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11026_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_0                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_1                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_2                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_3                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_0                                                (B0_TX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_1                                                (B0_TX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_2                                                (B0_TX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_3                                                (B0_TX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_0                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_1                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_2                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_3                                                (B0_TX_RECIPES_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_0                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_1                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_2                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_3                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_0                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_1                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_2                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_3                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_0                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_1                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_2                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_3                                                (B0_RX_CIRCULAR_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_0                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_1                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_2                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_3                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_0                                                 (BAND2_TX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_1                                                 (BAND2_TX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_2                                                 (BAND2_TX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_3                                                 (BAND2_TX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_0                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_1                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_2                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_3                                                 (BAND2_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_0                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_1                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_2                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_3                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_0                                                 (BAND2_RX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_1                                                 (BAND2_RX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_2                                                 (BAND2_RX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_3                                                 (BAND2_RX_BASE_ADDR + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_0                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_1                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_2                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_3                                                 (BAND2_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM0                                          (HOST_IF_QOS_COUNTERS_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1025_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM1                                          (HOST_IF_QOS_COUNTERS_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1125_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_0                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_1                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_2                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_3                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_0                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_1                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_2                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_3                                              (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_0                                               (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_1                                               (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_2                                               (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_3                                               (B0_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_0                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_1                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_2                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_3                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_0                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_1                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_2                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_3                                               (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_0                                                (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_1                                                (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_2                                                (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_3                                                (B2_MPDU_DESC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B01_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM                                      (DLM_M01_BASE_ADDR + 0x0000A848) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B01_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM                                      (DLM_M01_BASE_ADDR + 0x0000AA18) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM                                       (DLM_M2_BASE_ADDR + 0x0000A848) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B2_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM                                       (DLM_M2_BASE_ADDR + 0x0000AA18) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_RX_PP_RX_PP_COOR_STATISTICS_RAM                                       (RX_COORDINATOR_COUNTERS_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1012_BASE_ADDRESS */
#define B2_RX_PP_RX_PP_COOR_STATISTICS_RAM                                        (RX_COORDINATOR_COUNTERS_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1108_BASE_ADDRESS */
#define B0_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM                                        (B0_TWT_STATION_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1048_BASE_ADDRESS */
#define B0_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM                                      (B0_TWT_GROUP_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1027_BASE_ADDRESS */
#define B1_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM                                        (B1_TWT_STATION_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11048_BASE_ADDRESS */
#define B1_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM                                      (B1_TWT_GROUP_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11027_BASE_ADDRESS */
#define B0_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM                                     (B0_TX_SELECTOR_BASE_ADDR + 0x00001E00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM                                     (B1_TX_SELECTOR_BASE_ADDR + 0x00001E00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM                                      (B0_TX_SELECTOR_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM                                      (B0_TX_SELECTOR_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM                                      (B1_TX_SELECTOR_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_DUR_AUTO_RATE_DUR_MEM                                                  (B0_DUR_AUTO_RATE_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1055_BASE_ADDRESS */
#define B1_DUR_AUTO_RATE_DUR_MEM                                                  (B1_DUR_AUTO_RATE_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11055_BASE_ADDRESS */
#define B1_MAC_ADDR2INDEX_MEM                                                     (B1_MAC_ADDR2INDEX_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10022_BASE_ADDRESS */
#define HOST_IF_ACC_FW_CODE_RAM                                                   (HOST_IF_ACC_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_39_BASE_ADDRESS */
#define B0_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM                             (B0_BESTRU_BASE_ADDR + 0x00000100) /* MEMORY_MAP_UNIT_66_BASE_ADDRESS */
#define B0_RXC_RXC_FIELDS_CATCHER_MEM                                             (B0_RXF_MEM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_1052_BASE_ADDRESS */
#define B01_RX_PP_STA_BITMAP_RAM                                                  (RX_PP_M01_BASE_ADDR + 0x00009200) /* MEMORY_MAP_UNIT_1084_BASE_ADDRESS */
#define B2_RX_PP_STA_BITMAP_RAM                                                   (RX_PP_M2_BASE_ADDR + 0x00009200) /* MEMORY_MAP_UNIT_1284_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM0                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM1                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM2                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM3                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM4                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM5                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM6                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM7                                                (DESCRIPTOR_RAM_RX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM0                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM1                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM2                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM3                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM4                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM5                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM6                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM7                                                (DESCRIPTOR_RAM_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM0                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM1                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM2                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM3                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM4                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM5                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM6                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM7                                              (DESCRIPTOR_RAM_RX_TX_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM0                                                         (PLAN_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM1                                                         (PLAN_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM2                                                         (PLAN_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM3                                                         (PLAN_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM0                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM1                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM2                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM3                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM4                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM5                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM6                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM7                                                       (SHARED_RAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define B0_TX_SELECTOR_VAP_TID_RAM                                                (B0_TX_SELECTOR_BASE_ADDR + 0x00001A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_VAP_TID_RAM                                                (B1_TX_SELECTOR_BASE_ADDR + 0x00001A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_RX_COORDINATOR_FRAME_CLASS_VIO_MEM                                     (B0_FRAME_CLASS_VIO_BUFFER_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1058_BASE_ADDRESS */
#define B1_RX_COORDINATOR_FRAME_CLASS_VIO_MEM                                     (B1_FRAME_CLASS_VIO_BUFFER_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11058_BASE_ADDRESS */
#define B0_OTFA_TOP_FRAG_DB_MEM_I                                                 (B0_OTFA_MEM2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_1073_BASE_ADDRESS */
#define B1_OTFA_TOP_FRAG_DB_MEM_I                                                 (B1_OTFA_MEM2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_11073_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_VAP_RAM                                                (B0_TX_SELECTOR_BASE_ADDR + 0x00001D00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_MLD_DB_TOP_MLD_DB_MEMORY                                               (B0_MLD_DB_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1110_BASE_ADDRESS */
#define B1_MLD_DB_TOP_MLD_DB_MEMORY                                               (B1_MLD_DB_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11110_BASE_ADDRESS */
#define B01_RX_PP_RX_PP_STATISTICS_RAM                                            (RX_PP_COUNTERS_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1094_BASE_ADDRESS */
#define B2_RX_PP_RX_PP_STATISTICS_RAM                                             (RX_PP_COUNTERS_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1194_BASE_ADDRESS */
#define EXT_RX_PP_COUNTERS_EXT_RX_PP_STATISTICS_RAM                               (EXT_RX_PP_COUNTERS_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1042_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_USER_ATTR_MEM                                              (B1_MAC_SEC_MEM_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_KEY_MEM                                                    (B1_MAC_SEC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_0                                             (B0_AUTO_RESP_MEMS_BASE_ADDR + 0x00001B00) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_1                                             (B0_AUTO_RESP_MEMS_BASE_ADDR + 0x00001F00) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_2                                             (B0_AUTO_RESP_MEMS_BASE_ADDR + 0x00002300) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_3                                             (B0_AUTO_RESP_MEMS_BASE_ADDR + 0x00002700) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_0                                             (B1_AUTO_RESP_MEMS_BASE_ADDR + 0x00001B00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_1                                             (B1_AUTO_RESP_MEMS_BASE_ADDR + 0x00001D00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_2                                             (B1_AUTO_RESP_MEMS_BASE_ADDR + 0x00001F00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_3                                             (B1_AUTO_RESP_MEMS_BASE_ADDR + 0x00002100) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_0                                             (B2_AUTO_RESP_MEMS_BASE_ADDR + 0x00001B00) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_1                                             (B2_AUTO_RESP_MEMS_BASE_ADDR + 0x00001F00) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_2                                             (B2_AUTO_RESP_MEMS_BASE_ADDR + 0x00002300) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_3                                             (B2_AUTO_RESP_MEMS_BASE_ADDR + 0x00002700) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B0_AUTO_RESP_USER_DB_MEM                                                  (B0_AUTO_RESP_MEMS_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B1_AUTO_RESP_USER_DB_MEM                                                  (B1_AUTO_RESP_MEMS_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B2_AUTO_RESP_USER_DB_MEM                                                  (B2_AUTO_RESP_MEMS_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B0_MAC_BFM_BEST_RU_DB                                                     (B0_MAC_BFM_BASE_ADDR + 0x00000600) /* MEMORY_MAP_UNIT_11_BASE_ADDRESS */
#define B0_BAA_TOP_RA_USER_MEM                                                    (B0_BA_ANALYZER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_45_BASE_ADDRESS */
#define B1_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM                             (B1_BESTRU_BASE_ADDR + 0x00000100) /* MEMORY_MAP_UNIT_10066_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM                                     (B0_MAC_GENRISC_RX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1031_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM                                     (B1_MAC_GENRISC_RX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11031_BASE_ADDRESS */
#define B0_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM                                       (B0_TX_PD_ACC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1097_BASE_ADDRESS */
#define B0_OTFA_TOP_LOCAL_DB_MEM_I                                                (B0_OTFA_MEM2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1073_BASE_ADDRESS */
#define LOGGER_WRAPPER_UDP                                                        (DATA_LOGGER_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_61_BASE_ADDRESS */
#define B0_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM                                     (B0_TX_HANDLER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_23_BASE_ADDRESS */
#define B0_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM                                     (B0_TX_HANDLER_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_23_BASE_ADDRESS */
#define B1_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM                                     (B1_TX_HANDLER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10023_BASE_ADDRESS */
#define B1_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM                                     (B1_TX_HANDLER_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_10023_BASE_ADDRESS */
#define B01_Q_MANAGER_PD_DLMS_RAM                                                 (DLM_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_PD_DLMS_RAM                                                  (DLM_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM                                (BA_ANALYZER_COUNTER_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1049_BASE_ADDRESS */
#define B2_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM                                 (BA_ANALYZER_COUNTER_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1149_BASE_ADDRESS */
#define B01_RETRY_STATE_DB_DUAL_BAND_RAM                                          (RETRY_STATE_DB_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1056_BASE_ADDRESS */
#define B2_RETRY_STATE_DB_DUAL_BAND_RAM                                           (RETRY_STATE_DB_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1156_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD                                      (B0_MAC_GENRISC_TX_STD_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1029_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD                                      (B1_MAC_GENRISC_TX_STD_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11029_BASE_ADDRESS */
#define B0_TF_GENERATOR_TF_GEN_RU_INDEX_MEM                                       (B0_TF_GENERATOR_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1085_BASE_ADDRESS */
#define B1_TF_GENERATOR_TF_GEN_RU_INDEX_MEM                                       (B1_TF_GENERATOR_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11085_BASE_ADDRESS */
#define B0_TF_DECODER_TF_DEC_TX_POWER_MEM                                         (B0_TF_DECODER_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1041_BASE_ADDRESS */
#define B1_TF_DECODER_TF_DEC_TX_POWER_MEM                                         (B1_TF_DECODER_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11041_BASE_ADDRESS */
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM                                      (B1_TX_SELECTOR_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_TID_RAM0                                               (B1_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_TID_RAM1                                               (B1_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_USER_ATTR_MEM                                              (B0_MAC_SEC_MEM_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_KEY_MEM                                                    (B0_MAC_SEC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define B0_RX_CLASSIFIER_RX_CLASSIFIER_MEM                                        (B0_RX_CLASSIFIER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_57_BASE_ADDRESS */
#define B1_RX_CLASSIFIER_RX_CLASSIFIER_MEM                                        (B1_RX_CLASSIFIER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10057_BASE_ADDRESS */
#define HOST_IF_ACC_DIST_RAM                                                      (HOST_IF_ACC_BASE_ADDR + 0x00000c00) /* MEMORY_MAP_UNIT_39_BASE_ADDRESS */
#define B01_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM                                      (DLM_M01_BASE_ADDR + 0x0000A5C8) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B01_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM                                      (DLM_M01_BASE_ADDR + 0x0000A708) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM                                       (DLM_M2_BASE_ADDR + 0x0000A5C8) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B2_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM                                       (DLM_M2_BASE_ADDR + 0x0000A708) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_Q_MANAGER_DMA_DLMS_RAM                                                (DLM_M01_BASE_ADDR + 0x0000A480) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_DMA_DLMS_RAM                                                 (DLM_M2_BASE_ADDR + 0x0000A480) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define HOST_IF_AQM_CFG_MEM                                                       (HOST_IF_AQM_STAT_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1109_BASE_ADDRESS */
#define HOST_IF_AQM_STAT_BURST_MEM                                                (HOST_IF_AQM_STAT_BASE_ADDR + 0x00012000) /* MEMORY_MAP_UNIT_1109_BASE_ADDRESS */
#define B0_PRE_AGG_PRE_AGG_MEM                                                    (B0_PRE_AGG_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_PRE_AGG_MEM                                                    (B1_PRE_AGG_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B1_MAC_BFM_MBFM_TASK_SW_DB_L                                              (B1_MAC_BFM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_10011_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_MIC_MEM                                                    (B1_MAC_SEC_MEM_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B0_MAC_BFM_MBFM_TASK_SW_DB_L                                              (B0_MAC_BFM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_11_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_MIC_MEM                                                    (B0_MAC_SEC_MEM_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_6_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00180000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_7_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x001C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_8_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00200000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_9_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00240000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_10_IRAM_LOW                                               (CPU_IRAM_BASE_ADDR + 0x00280000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_11_IRAM_LOW                                               (CPU_IRAM_BASE_ADDR + 0x002C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_12_IRAM_LOW                                               (CPU_IRAM_BASE_ADDR + 0x00300000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_13_IRAM_LOW                                               (CPU_IRAM_BASE_ADDR + 0x00340000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define HOST_IF_CONTEXT_SAVE_MEMORY                                               (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR + 0x0000A000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_ICCM_RAM0                                                       (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR + 0x00010000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_DCCM_RAM_BANK0                                                  (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_DCCM_RAM_BANK1                                                  (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define B2_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM                                (BEST_RU_DB_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1006_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM                                    (B2_MAC_GENRISC_RX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21033_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM                                    (B2_MAC_GENRISC_TX_SPRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21026_BASE_ADDRESS */
#define B2_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM                                        (B2_TWT_STATION_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21048_BASE_ADDRESS */
#define B2_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM                                      (B2_TWT_GROUP_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21027_BASE_ADDRESS */
#define B2_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM                                     (B2_TX_SELECTOR_BASE_ADDR + 0x00001E00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM                                      (B2_TX_SELECTOR_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM                                      (B2_TX_SELECTOR_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_DUR_AUTO_RATE_DUR_MEM                                                  (B2_DUR_AUTO_RATE_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21055_BASE_ADDRESS */
#define B2_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM                             (B2_BESTRU_BASE_ADDR + 0x00000100) /* MEMORY_MAP_UNIT_20066_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_TID_RAM0                                               (B2_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_TID_RAM1                                               (B2_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_RXF_RXF_MEM                                                            (B2_RXF_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21052_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM                                     (B2_MAC_GENRISC_TX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21024_BASE_ADDRESS */
#define B2_PRE_AGG_TOP_PARAMS_CALC_MEM                                            (B2_PRE_AGG_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_MAC_ADDR2INDEX_MEM                                                     (B2_MAC_ADDR2INDEX_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20022_BASE_ADDRESS */
#define B2_RXC_RXC_FIELDS_CATCHER_MEM                                             (B2_RXF_MEM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_21052_BASE_ADDRESS */
#define B2_TX_SELECTOR_VAP_TID_RAM                                                (B2_TX_SELECTOR_BASE_ADDR + 0x00001A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_RX_COORDINATOR_FRAME_CLASS_VIO_MEM                                     (B2_FRAME_CLASS_VIO_BUFFER_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21058_BASE_ADDRESS */
#define B2_OTFA_TOP_FRAG_DB_MEM_I                                                 (B2_OTFA_MEM2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_21073_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_VAP_RAM                                                (B2_TX_SELECTOR_BASE_ADDR + 0x00001D00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_MLD_DB_TOP_MLD_DB_MEMORY                                               (B2_MLD_DB_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21110_BASE_ADDRESS */
#define B2_MAC_BFM_BEST_RU_DB                                                     (B2_MAC_BFM_BASE_ADDR + 0x00000600) /* MEMORY_MAP_UNIT_20011_BASE_ADDRESS */
#define B2_BAA_TOP_RA_USER_MEM                                                    (B2_BA_ANALYZER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20045_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM                                     (B2_MAC_GENRISC_RX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21031_BASE_ADDRESS */
#define B2_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM                                       (B2_TX_PD_ACC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21097_BASE_ADDRESS */
#define B2_OTFA_TOP_LOCAL_DB_MEM_I                                                (B2_OTFA_MEM2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21073_BASE_ADDRESS */
#define B2_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM                                     (B2_TX_HANDLER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20023_BASE_ADDRESS */
#define B2_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM                                     (B2_TX_HANDLER_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_20023_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD                                      (B2_MAC_GENRISC_TX_STD_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21029_BASE_ADDRESS */
#define B2_TF_GENERATOR_TF_GEN_RU_INDEX_MEM                                       (B2_TF_GENERATOR_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21085_BASE_ADDRESS */
#define B2_TF_DECODER_TF_DEC_TX_POWER_MEM                                         (B2_TF_DECODER_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21041_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_USER_ATTR_MEM                                              (B2_MAC_SEC_MEM_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_KEY_MEM                                                    (B2_MAC_SEC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_MEM                                        (B2_RX_CLASSIFIER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_20057_BASE_ADDRESS */
#define B2_PRE_AGG_PRE_AGG_MEM                                                    (B2_PRE_AGG_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_MAC_BFM_MBFM_TASK_SW_DB_L                                              (B2_MAC_BFM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_20011_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_MIC_MEM                                                    (B2_MAC_SEC_MEM_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_TID_RAM0                                               (B0_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_TID_RAM1                                               (B0_TX_SELECTOR_BASE_ADDR + 0x00000A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_RXF_RXF_MEM                                                            (B0_RXF_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1052_BASE_ADDRESS */
#define B1_RXF_RXF_MEM                                                            (B1_RXF_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11052_BASE_ADDRESS */
#define B1_RXC_RXC_FIELDS_CATCHER_MEM                                             (B1_RXF_MEM_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_11052_BASE_ADDRESS */
#define B01_DMAC_WRAPPER_U_LL_MEM                                                 (WLAN_DMA_LL_MEM_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1008_BASE_ADDRESS */
#define B2_DMAC_WRAPPER_U_LL_MEM                                                  (WLAN_DMA_LL_MEM_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1208_BASE_ADDRESS */
#define B01_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM                               (BEST_RU_DB_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1038_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM                                     (B0_MAC_GENRISC_TX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1024_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM                                     (B1_MAC_GENRISC_TX_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11024_BASE_ADDRESS */
#define B01_RETRY_POINTER_DB_DUAL_BAND_RAM_0                                      (RETRY_POINTER_DB_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1068_BASE_ADDRESS */
#define B2_RETRY_POINTER_DB_DUAL_BAND_RAM_0                                       (RETRY_POINTER_DB_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1168_BASE_ADDRESS */
#define B01_TRAFFIC_INDICATOR_MEM                                                 (TRAFFIC_INDICATOR_M01_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_1054_BASE_ADDRESS */
#define B2_TRAFFIC_INDICATOR_MEM                                                  (TRAFFIC_INDICATOR_M2_BASE_ADDR + 0x00000400) /* MEMORY_MAP_UNIT_1154_BASE_ADDRESS */
#define B1_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM                                       (B1_TX_PD_ACC_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11097_BASE_ADDRESS */
#define B1_MAC_BFM_BEST_RU_DB                                                     (B1_MAC_BFM_BASE_ADDR + 0x00000600) /* MEMORY_MAP_UNIT_10011_BASE_ADDRESS */
#define B1_BAA_TOP_RA_USER_MEM                                                    (B1_BA_ANALYZER_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10045_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM0                                   (HOST_IF_COUNTERS_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1075_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM1                                   (HOST_IF_COUNTERS_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1175_BASE_ADDRESS */
#define B01_RX_PP_PN_SN_RAM                                                       (RX_PP_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1084_BASE_ADDRESS */
#define B2_RX_PP_PN_SN_RAM                                                        (RX_PP_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1284_BASE_ADDRESS */
#define PROT_DB_SMC_LOCK_MEM                                                      (PROT_DB_SHADOW_MEM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1015_BASE_ADDRESS */
#define B1_OTFA_TOP_LOCAL_DB_MEM_I                                                (B1_OTFA_MEM2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_11073_BASE_ADDRESS */
#define B0_PRE_AGG_TOP_PARAMS_CALC_MEM                                            (B0_PRE_AGG_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_TOP_PARAMS_CALC_MEM                                            (B1_PRE_AGG_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B01_BSRC_AGER_MEM_COMMON_BAND_RAM                                         (BSRC_AGER_MEM_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1017_BASE_ADDRESS */
#define B2_BSRC_AGER_MEM_COMMON_BAND_RAM                                          (BSRC_AGER_MEM_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1121_BASE_ADDRESS */
#define B01_BSRC_CNT_MEM_COMMON_BAND_RAM                                          (BSRC_CNT_MEM_M01_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_67_BASE_ADDRESS */
#define B2_BSRC_CNT_MEM_COMMON_BAND_RAM                                           (BSRC_CNT_MEM_M2_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1167_BASE_ADDRESS */
#define B0_MAC_ADDR2INDEX_MEM                                                     (B0_MAC_ADDR2INDEX_BASE_ADDR + 0x00000800) /* MEMORY_MAP_UNIT_22_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_VAP_RAM                                                (B1_TX_SELECTOR_BASE_ADDR + 0x00001D00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_0_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_1_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00040000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_2_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00080000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_3_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x000C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_4_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00100000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_5_IRAM_LOW                                                (CPU_IRAM_BASE_ADDR + 0x00140000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define B0_RX_RCR_RCR_RAM_CDB0                                                    (B0_PHY_RX_BE_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_201_BASE_ADDRESS */
#define B1_RX_RCR_RCR_RAM_CDB1                                                    (B1_PHY_RX_BE_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10201_BASE_ADDRESS */
#define B0_RX_SIGB_SIGB_RAM_CDB0                                                  (B0_PHY_RX_BE_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_201_BASE_ADDRESS */
#define B1_RX_SIGB_SIGB_RAM_CDB1                                                  (B1_PHY_RX_BE_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10201_BASE_ADDRESS */
#define B2_RX_SIGB_SIGB_RAM_CDB0                                                  (B2_PHY_RX_BE_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20201_BASE_ADDRESS */
#define B2_RX_RCR_RCR_RAM_CDB0                                                    (B2_PHY_RX_BE_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20201_BASE_ADDRESS */
#define B0_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY                             (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM                             (B0_PHY_RX_FD_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_RXFD_CSM_RCR_RAM                                                   (B0_PHY_RX_FD_BASE_ADDR + 0x00005300) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM                                       (B0_PHY_RX_FD_BASE_ADDR + 0x00002E00) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_FILTER_SELECT_FILTER_RAM                                               (B0_PHY_RX_FD_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_LO                                (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_HI                                (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0                                       (B0_PHY_RX_FD_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_RXFD_BIN_CHOOSER_NE_RAM                                            (B0_PHY_RX_FD_BASE_ADDR + 0x00008000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_LINEAR_SLICER_EVM_MU                                                   (B0_PHY_RX_FD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_CH_RAM_0_DATA_CH_RAM_LO_R                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_0_DATA_CH_RAM_LO_L                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_1_DATA_CH_RAM_LO_R                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_1_DATA_CH_RAM_LO_L                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_2_DATA_CH_RAM_LO_R                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_2_DATA_CH_RAM_LO_L                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_3_DATA_CH_RAM_LO_R                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_3_DATA_CH_RAM_LO_L                                              (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE                  (B0_PHY_RX_FD_BASE_ADDR + 0x00006000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MCS_SNR_MCS_SNR_RAM                                                    (B0_PHY_RX_FD_BASE_ADDR + 0x00002400) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0                     (B0_PHY_RX_FD_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1                     (B0_PHY_RX_FD_BASE_ADDR + 0x00005080) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2                     (B0_PHY_RX_FD_BASE_ADDR + 0x00005100) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3                     (B0_PHY_RX_FD_BASE_ADDR + 0x00005180) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_HDR_MU_HDR_MEMORY                                                   (B0_PHY_RX_FD_BASE_ADDR + 0x00002C00) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0                                  (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1                                  (B0_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SU_HDR_SU_HDR_MEMORY                                                   (B0_PHY_RX_FD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_TXFD_TXTD_SCALE_RAM                                                (B0_PHY_TX_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B1_LONG_PREAMBLE_LONG_PREAMBLE_RAM                                        (B1_PHY_TX_BASE_ADDR + 0x00003800) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B1_HYP_TXFD_TXTD_SCALE_RAM                                                (B1_PHY_TX_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B0_HYP_TX_EXT_CONTROL                                                     (B0_PHY_TX_BASE_ADDR + 0x00003C00) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B1_HYP_TX_EXT_CONTROL                                                     (B1_PHY_TX_BASE_ADDR + 0x00003C00) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B0_LONG_PREAMBLE_LONG_PREAMBLE_RAM                                        (B0_PHY_TX_BASE_ADDR + 0x00003800) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B2_LONG_PREAMBLE_LONG_PREAMBLE_RAM                                        (B2_PHY_TX_BASE_ADDR + 0x00003800) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B2_HYP_TXFD_TXTD_SCALE_RAM                                                (B2_PHY_TX_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B2_UP_LONG_PREAMBLE_LONG_PREAMBLE_RAM                                     (B2_UP_PHY_TX_BASE_ADDR + 0x00003800) /* MEMORY_MAP_UNIT_20205_BASE_ADDRESS */
#define B2_UP_HYP_TXFD_TXTD_SCALE_RAM                                             (B2_UP_PHY_TX_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20205_BASE_ADDRESS */
#define B2_HYP_TX_EXT_CONTROL                                                     (B2_PHY_TX_BASE_ADDR + 0x00003C00) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B0_DC_ESTIMATION_CFO_THRESHOLD_RAM                                        (B0_PHY_RX_TD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_200_BASE_ADDRESS */
#define B1_DC_ESTIMATION_CFO_THRESHOLD_RAM                                        (B1_PHY_RX_TD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10200_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_0                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_1                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_2                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_3                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_4                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_5                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_6                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_7                                       (B0_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RIPPLE_RAM                                            (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_RIPPLE_RAM                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RIPPLE_RAM                                            (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_RIPPLE_RAM                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RIPPLE_RAM                                            (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_RIPPLE_RAM                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RIPPLE_RAM                                            (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_RIPPLE_RAM                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_IQ_RAM                                                (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_IQ_RAM                                                   (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_IQ_RAM                                                (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_IQ_RAM                                                   (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_IQ_RAM                                                (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_IQ_RAM                                                   (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_IQ_RAM                                                (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_IQ_RAM                                                   (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_CAL_RAM                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_CAL_RAM                                                  (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_CAL_RAM                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_CAL_RAM                                                  (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_CAL_RAM                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_CAL_RAM                                                  (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_CAL_RAM                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_CAL_RAM                                                  (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DELAY_FIFO                                            (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DELAY_FIFO                                               (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DELAY_FIFO                                            (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DELAY_FIFO                                               (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DELAY_FIFO                                            (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DELAY_FIFO                                               (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DELAY_FIFO                                            (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DELAY_FIFO                                               (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B0_GEN_RISC_GEN_RISC_IRAM                                                 (B0_PHY_RISC_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_208_BASE_ADDRESS */
#define B1_GEN_RISC_GEN_RISC_IRAM                                                 (B1_PHY_RISC_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10208_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_FFT_0_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_FFT_0_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_FFT_0_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_FFT_0_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_FFT_0_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_FFT_0_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_FFT_0_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_FFT_0_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_FFT_1_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_FFT_1_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_FFT_1_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_FFT_1_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_FFT_1_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_FFT_1_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_FFT_1_RAM                                             (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_FFT_1_RAM                                                (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_0                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_RXTD_FIFO_0                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_0                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_RXTD_FIFO_0                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_0                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_RXTD_FIFO_0                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_0                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_RXTD_FIFO_0                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_1                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_RXTD_FIFO_1                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_1                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_RXTD_FIFO_1                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_1                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_RXTD_FIFO_1                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_1                                           (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_RXTD_FIFO_1                                              (B0_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AE                                            (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DPD_RAM_AE                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AE                                            (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DPD_RAM_AE                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AE                                            (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DPD_RAM_AE                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AE                                            (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DPD_RAM_AE                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AO                                            (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DPD_RAM_AO                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AO                                            (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DPD_RAM_AO                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AO                                            (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DPD_RAM_AO                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AO                                            (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DPD_RAM_AO                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BE                                            (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DPD_RAM_BE                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BE                                            (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DPD_RAM_BE                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BE                                            (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DPD_RAM_BE                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BE                                            (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DPD_RAM_BE                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BO                                            (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DPD_RAM_BO                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BO                                            (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DPD_RAM_BO                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BO                                            (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DPD_RAM_BO                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BO                                            (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DPD_RAM_BO                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B1_RADAR_DETECTOR_EVENTS_RAM                                              (B1_PHY_RX_TD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10200_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_CFG                                           (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_DPD_RAM_CFG                                              (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_CFG                                           (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_DPD_RAM_CFG                                              (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_CFG                                           (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_DPD_RAM_CFG                                              (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_CFG                                           (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_DPD_RAM_CFG                                              (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_TPC_RAM                                               (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_TPC_RAM                                                  (B0_PHY_TXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_TPC_RAM                                               (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_TPC_RAM                                                  (B0_PHY_TXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_TPC_RAM                                               (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_TPC_RAM                                                  (B0_PHY_TXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_TPC_RAM                                               (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_TPC_RAM                                                  (B0_PHY_TXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_AGC_TABLE_RAM                                         (B0_PHY_AGC_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_220_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_AGC_TABLE_RAM                                            (B0_PHY_AGC_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_220_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_AGC_TABLE_RAM                                         (B0_PHY_AGC_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_221_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_AGC_TABLE_RAM                                            (B0_PHY_AGC_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_221_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_AGC_TABLE_RAM                                         (B0_PHY_AGC_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_222_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_AGC_TABLE_RAM                                            (B0_PHY_AGC_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_222_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_AGC_TABLE_RAM                                         (B0_PHY_AGC_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_223_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_AGC_TABLE_RAM                                            (B0_PHY_AGC_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_223_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT                                          (B1_PHY_RXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT                                          (B1_PHY_RXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT                                          (B1_PHY_RXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT                                          (B1_PHY_RXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_BAND_SEL_COEF                                         (B1_PHY_RXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_BAND_SEL_COEF                                         (B1_PHY_RXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_BAND_SEL_COEF                                         (B1_PHY_RXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_BAND_SEL_COEF                                         (B1_PHY_RXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RIPPLE_RAM                                            (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RIPPLE_RAM                                            (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RIPPLE_RAM                                            (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RIPPLE_RAM                                            (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_IQ_RAM                                                (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_IQ_RAM                                                (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_IQ_RAM                                                (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_IQ_RAM                                                (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_CAL_RAM                                               (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_CAL_RAM                                               (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_CAL_RAM                                               (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_CAL_RAM                                               (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DELAY_FIFO                                            (B1_PHY_RXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DELAY_FIFO                                            (B1_PHY_RXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DELAY_FIFO                                            (B1_PHY_RXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DELAY_FIFO                                            (B1_PHY_RXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_0                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_0                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_0                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_0                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_FFT_0_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_FFT_1_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_FFT_0_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_FFT_1_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_FFT_0_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_FFT_1_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_FFT_0_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_FFT_1_RAM                                             (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_1                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_1                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_1                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_1                                           (B1_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AE                                            (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AO                                            (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BE                                            (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BO                                            (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AE                                            (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AO                                            (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BE                                            (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BO                                            (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AE                                            (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AO                                            (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BE                                            (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BO                                            (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AE                                            (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AO                                            (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BE                                            (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BO                                            (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_CFG                                           (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_CFG                                           (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_CFG                                           (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_CFG                                           (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_TPC_RAM                                               (B1_PHY_TXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_TPC_RAM                                               (B1_PHY_TXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_TPC_RAM                                               (B1_PHY_TXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_TPC_RAM                                               (B1_PHY_TXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_AGC_TABLE_RAM                                         (B1_PHY_AGC_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10220_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_AGC_TABLE_RAM                                         (B1_PHY_AGC_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10221_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_AGC_TABLE_RAM                                         (B1_PHY_AGC_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10222_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_AGC_TABLE_RAM                                         (B1_PHY_AGC_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10223_BASE_ADDRESS */
#define B0_GEN_RISC_GEN_RISC_SRAM                                                 (B0_PHY_RISC_BASE_ADDR + 0x0000C000) /* MEMORY_MAP_UNIT_208_BASE_ADDRESS */
#define B1_GEN_RISC_GEN_RISC_SRAM                                                 (B1_PHY_RISC_BASE_ADDR + 0x0000C000) /* MEMORY_MAP_UNIT_10208_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT                                          (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_IQ_FIREQ_LUT                                             (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT                                          (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_IQ_FIREQ_LUT                                             (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT                                          (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_IQ_FIREQ_LUT                                             (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT                                          (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_IQ_FIREQ_LUT                                             (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_BAND_SEL_COEF                                         (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define HYP_TD_ANTENNA_0_BAND_SEL_COEF                                            (B0_PHY_RXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_BAND_SEL_COEF                                         (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define HYP_TD_ANTENNA_1_BAND_SEL_COEF                                            (B0_PHY_RXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_BAND_SEL_COEF                                         (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define HYP_TD_ANTENNA_2_BAND_SEL_COEF                                            (B0_PHY_RXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_BAND_SEL_COEF                                         (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define HYP_TD_ANTENNA_3_BAND_SEL_COEF                                            (B0_PHY_RXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B2_GEN_RISC_GEN_RISC_SRAM                                                 (B2_PHY_RISC_BASE_ADDR + 0x0000C000) /* MEMORY_MAP_UNIT_20208_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT                                          (B2_PHY_RXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT                                          (B2_PHY_RXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT                                          (B2_PHY_RXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT                                          (B2_PHY_RXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_BAND_SEL_COEF                                         (B2_PHY_RXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_BAND_SEL_COEF                                         (B2_PHY_RXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_BAND_SEL_COEF                                         (B2_PHY_RXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_BAND_SEL_COEF                                         (B2_PHY_RXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_DC_ESTIMATION_CFO_THRESHOLD_RAM                                        (B2_PHY_RX_TD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20200_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_0                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_1                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_2                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_3                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_4                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_5                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_6                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_7                                        (B2_PHY_TEST_BUS_OR_DUMMY_PHY_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RIPPLE_RAM                                            (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RIPPLE_RAM                                            (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RIPPLE_RAM                                            (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RIPPLE_RAM                                            (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_IQ_RAM                                                (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_IQ_RAM                                                (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_IQ_RAM                                                (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_IQ_RAM                                                (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_CAL_RAM                                               (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_CAL_RAM                                               (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_CAL_RAM                                               (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_CAL_RAM                                               (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DELAY_FIFO                                            (B2_PHY_RXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DELAY_FIFO                                            (B2_PHY_RXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DELAY_FIFO                                            (B2_PHY_RXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DELAY_FIFO                                            (B2_PHY_RXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_GEN_RISC_GEN_RISC_IRAM                                                 (B2_PHY_RISC_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20208_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_0                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_0                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_0                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_0                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_FFT_0_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_FFT_1_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_FFT_0_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_FFT_1_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_FFT_0_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_FFT_1_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_FFT_0_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_FFT_1_RAM                                             (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_1                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_1                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_1                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_1                                           (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE                                            (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO                                            (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE                                            (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO                                            (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE                                            (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO                                            (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE                                            (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO                                            (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE                                            (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO                                            (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE                                            (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO                                            (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE                                            (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO                                            (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00004400) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE                                            (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00004800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO                                            (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00004C00) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_CFG                                           (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_CFG                                           (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_CFG                                           (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_CFG                                           (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00001800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_TPC_RAM                                               (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_TPC_RAM                                               (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_TPC_RAM                                               (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_TPC_RAM                                               (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_AGC_TABLE_RAM                                         (B2_PHY_AGC_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20220_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_AGC_TABLE_RAM                                         (B2_PHY_AGC_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20221_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_AGC_TABLE_RAM                                         (B2_PHY_AGC_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20222_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_AGC_TABLE_RAM                                         (B2_PHY_AGC_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20223_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_BAND_SEL_COEF                                      (B2_UP_PHY_RXTD_ANT0_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20244_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_BAND_SEL_COEF                                      (B2_UP_PHY_RXTD_ANT1_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20245_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_BAND_SEL_COEF                                      (B2_UP_PHY_RXTD_ANT2_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20246_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_BAND_SEL_COEF                                      (B2_UP_PHY_RXTD_ANT3_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20247_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RIPPLE_RAM                                         (B2_UP_PHY_TXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RIPPLE_RAM                                         (B2_UP_PHY_TXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RIPPLE_RAM                                         (B2_UP_PHY_TXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RIPPLE_RAM                                         (B2_UP_PHY_TXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_IQ_RAM                                             (B2_UP_PHY_TXTD_ANT0_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_IQ_RAM                                             (B2_UP_PHY_TXTD_ANT1_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_IQ_RAM                                             (B2_UP_PHY_TXTD_ANT2_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_IQ_RAM                                             (B2_UP_PHY_TXTD_ANT3_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_CAL_RAM                                            (B2_UP_PHY_TXTD_ANT0_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_CAL_RAM                                            (B2_UP_PHY_TXTD_ANT1_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_CAL_RAM                                            (B2_UP_PHY_TXTD_ANT2_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_CAL_RAM                                            (B2_UP_PHY_TXTD_ANT3_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_DELAY_FIFO                                         (B2_UP_PHY_RXTD_ANT0_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20244_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_DELAY_FIFO                                         (B2_UP_PHY_RXTD_ANT1_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20245_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_DELAY_FIFO                                         (B2_UP_PHY_RXTD_ANT2_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20246_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_DELAY_FIFO                                         (B2_UP_PHY_RXTD_ANT3_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_20247_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_0                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_0                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_0                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_0                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_FFT_0_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_FFT_1_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_FFT_0_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_FFT_1_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_FFT_0_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_FFT_1_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_FFT_0_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_FFT_1_RAM                                          (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_1                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_1                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_1                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_1                                        (B2_PHY_RX_TD_DEBUG_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B1_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY                             (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM                             (B1_PHY_RX_FD_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HYP_RXFD_CSM_RCR_RAM                                                   (B1_PHY_RX_FD_BASE_ADDR + 0x00005300) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM                                       (B1_PHY_RX_FD_BASE_ADDR + 0x00002E00) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_FILTER_SELECT_FILTER_RAM                                               (B1_PHY_RX_FD_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0                                    (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1                                    (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0                                    (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1                                    (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0                                       (B1_PHY_RX_FD_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HYP_RXFD_BIN_CHOOSER_NE_RAM                                            (B1_PHY_RX_FD_BASE_ADDR + 0x00008000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_LINEAR_SLICER_EVM_MU                                                   (B1_PHY_RX_FD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_LO_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_LO_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_HI_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_HI_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_LO_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_LO_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_HI_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_HI_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_LO_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_LO_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_HI_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_HI_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_LO_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_LO_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_HI_R                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_HI_L                                              (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE                  (B1_PHY_RX_FD_BASE_ADDR + 0x00006000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MCS_SNR_MCS_SNR_RAM                                                    (B1_PHY_RX_FD_BASE_ADDR + 0x00002400) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0                     (B1_PHY_RX_FD_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1                     (B1_PHY_RX_FD_BASE_ADDR + 0x00005080) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2                     (B1_PHY_RX_FD_BASE_ADDR + 0x00005100) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3                     (B1_PHY_RX_FD_BASE_ADDR + 0x00005180) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_HDR_MU_HDR_MEMORY                                                   (B1_PHY_RX_FD_BASE_ADDR + 0x00002C00) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0                                  (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1                                  (B1_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SU_HDR_SU_HDR_MEMORY                                                   (B1_PHY_RX_FD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B2_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY                             (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM                             (B2_PHY_RX_FD_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HYP_RXFD_CSM_RCR_RAM                                                   (B2_PHY_RX_FD_BASE_ADDR + 0x00005300) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM                                       (B2_PHY_RX_FD_BASE_ADDR + 0x00002E00) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_FILTER_SELECT_FILTER_RAM                                               (B2_PHY_RX_FD_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0                                    (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1                                    (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0                                    (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1                                    (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0                                       (B2_PHY_RX_FD_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HYP_RXFD_BIN_CHOOSER_NE_RAM                                            (B2_PHY_RX_FD_BASE_ADDR + 0x00008000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_LINEAR_SLICER_EVM_MU                                                   (B2_PHY_RX_FD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_LO_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_LO_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_HI_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_HI_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_LO_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_LO_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_HI_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_HI_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_LO_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_LO_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_HI_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_HI_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_LO_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_LO_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_HI_R                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_HI_L                                              (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE                  (B2_PHY_RX_FD_BASE_ADDR + 0x00006000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MCS_SNR_MCS_SNR_RAM                                                    (B2_PHY_RX_FD_BASE_ADDR + 0x00002400) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0                     (B2_PHY_RX_FD_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1                     (B2_PHY_RX_FD_BASE_ADDR + 0x00005080) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2                     (B2_PHY_RX_FD_BASE_ADDR + 0x00005100) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3                     (B2_PHY_RX_FD_BASE_ADDR + 0x00005180) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_HDR_MU_HDR_MEMORY                                                   (B2_PHY_RX_FD_BASE_ADDR + 0x00002C00) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0                                  (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1                                  (B2_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SU_HDR_SU_HDR_MEMORY                                                   (B2_PHY_RX_FD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_UP_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY                          (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM                          (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HYP_RXFD_CSM_RCR_RAM                                                (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005300) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM                                    (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00002E00) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_FILTER_SELECT_FILTER_RAM                                            (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0                                 (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1                                 (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0                                 (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1                                 (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0                                    (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HYP_RXFD_BIN_CHOOSER_NE_RAM                                         (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00008000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_LINEAR_SLICER_EVM_MU                                                (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_R                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_L                                           (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B1_DFS_GEN_RISC_GEN_RISC_IRAM                                             (B1_PHY_DFS_RISC_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_10206_BASE_ADDRESS */
#define B1_DFS_RADAR_DETECTOR_EVENTS_RAM                                          (B1_PHY_DFS_RX_TD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10207_BASE_ADDRESS */
// Following added manually as it is needed for LMAC1 to access DFS FIFO RAM.
#define B0_DFS_RADAR_DETECTOR_EVENTS_RAM                                          (B0_PHY_DFS_RX_TD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_10207_BASE_ADDRESS */
#define B1_DFS_GEN_RISC_GEN_RISC_SRAM                                             (B1_PHY_DFS_RISC_BASE_ADDR + 0x0000C000) /* MEMORY_MAP_UNIT_10206_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_DELAY_FIFO                                          (B1_PHY_DFS_RXTD_ANT_BASE_ADDR + 0x00001000) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_AGC_TABLE_RAM                                       (B1_PHY_DFS_AGC_ANT_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10215_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_IQ_FIREQ_LUT                                        (B1_PHY_DFS_RXTD_ANT_BASE_ADDR + 0x00002800) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_BAND_SEL_COEF                                       (B1_PHY_DFS_RXTD_ANT_BASE_ADDR + 0x00002000) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B2_UP_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE               (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00006000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MCS_SNR_MCS_SNR_RAM                                                 (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00002400) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0                  (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1                  (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005080) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2                  (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005100) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3                  (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00005180) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_HDR_MU_HDR_MEMORY                                                (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00002C00) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0                               (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1                               (B2_UP_PHY_RX_FD_DEBUG_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SU_HDR_SU_HDR_MEMORY                                                (B2_UP_PHY_RX_FD_BASE_ADDR + 0x00003000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B0_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW                                        (B0_PRE_AGG_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW                                        (B1_PRE_AGG_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B2_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW                                        (B2_PRE_AGG_BASE_ADDR + 0x00004000) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE_1                                          (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO_1                                          (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE_1                                          (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO_1                                          (B2_PHY_TXTD_ANT0_BASE_ADDR + 0x00005C00) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE_1                                          (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO_1                                          (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE_1                                          (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO_1                                          (B2_PHY_TXTD_ANT1_BASE_ADDR + 0x00005C00) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE_1                                          (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO_1                                          (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE_1                                          (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO_1                                          (B2_PHY_TXTD_ANT2_BASE_ADDR + 0x00005C00) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE_1                                          (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00005000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO_1                                          (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00005400) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE_1                                          (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00005800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO_1                                          (B2_PHY_TXTD_ANT3_BASE_ADDR + 0x00005C00) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_6_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00180000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_7_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x001C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_8_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00200000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_9_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00240000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_10_IRAM_HIGH                                              (CPU_IRAM_BASE_ADDR + 0x00280000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_11_IRAM_HIGH                                              (CPU_IRAM_BASE_ADDR + 0x002C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_12_IRAM_HIGH                                              (CPU_IRAM_BASE_ADDR + 0x00300000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_13_IRAM_HIGH                                              (CPU_IRAM_BASE_ADDR + 0x00340000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_0_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00000000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_1_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00040000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_2_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00080000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_3_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x000C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_4_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00100000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_5_IRAM_HIGH                                               (CPU_IRAM_BASE_ADDR + 0x00140000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */

 /* auto generated from wave700_rams_by_modules_ver2.0.0.xlsm, sheet gen6_wlanip_rams_by_modules From DMA Perspective*/
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_OFFSET_FROM_BAR0                   (B0_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1033_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_OFFSET_FROM_BAR0                   (B0_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1026_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_OFFSET_FROM_BAR0                   (B1_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11033_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_OFFSET_FROM_BAR0                   (B1_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11026_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_0_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_1_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_2_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM2_3_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_0_OFFSET_FROM_BAR0                               (B0_TX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_1_OFFSET_FROM_BAR0                               (B0_TX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_2_OFFSET_FROM_BAR0                               (B0_TX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM1_3_OFFSET_FROM_BAR0                               (B0_TX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1022_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_0_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_1_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_2_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_TX_CIRC_RAM_MEM0_3_OFFSET_FROM_BAR0                               (B0_TX_RECIPES_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1023_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_0_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_1_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_2_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM2_3_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_0_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_1_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_2_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM1_3_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_0_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_1_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_2_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B01_MAC_RX_CIRC_RAM_MEM0_3_OFFSET_FROM_BAR0                               (B0_RX_CIRCULAR_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1020_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_0_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_1_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_2_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM2_3_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_0_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_1_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_2_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM1_3_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_0_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_1_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_2_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_TX_CIRC_RAM_MEM0_3_OFFSET_FROM_BAR0                                (BAND2_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1112_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_0_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_1_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_2_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM2_3_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_0_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_1_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_2_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM1_3_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00090000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_0_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_1_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_2_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define B2_MAC_RX_CIRC_RAM_MEM0_3_OFFSET_FROM_BAR0                                (BAND2_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1111_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM0_OFFSET_FROM_BAR0                         (HOST_IF_QOS_COUNTERS_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1025_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM1_OFFSET_FROM_BAR0                         (HOST_IF_QOS_COUNTERS_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1125_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_0_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_1_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_2_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM0_18K_3_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_0_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_1_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_2_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM1_18K_3_OFFSET_FROM_BAR0                             (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_0_OFFSET_FROM_BAR0                              (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_1_OFFSET_FROM_BAR0                              (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_2_OFFSET_FROM_BAR0                              (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B01_MD_MEM_TOP_MD_MEM2_1K_3_OFFSET_FROM_BAR0                              (B0_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_0_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_1_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_2_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM0_18K_3_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_0_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_1_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_2_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM1_18K_3_OFFSET_FROM_BAR0                              (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_0_OFFSET_FROM_BAR0                               (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_1_OFFSET_FROM_BAR0                               (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_2_OFFSET_FROM_BAR0                               (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B2_MD_MEM_TOP_MD_MEM2_1K_3_OFFSET_FROM_BAR0                               (B2_MPDU_DESC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21088_BASE_ADDRESS */
#define B01_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM_OFFSET_FROM_BAR0                     (DLM_M01_OFFSET_FROM_BAR0 + 0x0000A848) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B01_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM_OFFSET_FROM_BAR0                     (DLM_M01_OFFSET_FROM_BAR0 + 0x0000AA18) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM_OFFSET_FROM_BAR0                      (DLM_M2_OFFSET_FROM_BAR0 + 0x0000A848) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B2_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM_OFFSET_FROM_BAR0                      (DLM_M2_OFFSET_FROM_BAR0 + 0x0000AA18) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_RX_PP_RX_PP_COOR_STATISTICS_RAM_OFFSET_FROM_BAR0                      (RX_COORDINATOR_COUNTERS_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1012_BASE_ADDRESS */
#define B2_RX_PP_RX_PP_COOR_STATISTICS_RAM_OFFSET_FROM_BAR0                       (RX_COORDINATOR_COUNTERS_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1108_BASE_ADDRESS */
#define B0_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_OFFSET_FROM_BAR0                       (B0_TWT_STATION_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1048_BASE_ADDRESS */
#define B0_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_OFFSET_FROM_BAR0                     (B0_TWT_GROUP_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1027_BASE_ADDRESS */
#define B1_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_OFFSET_FROM_BAR0                       (B1_TWT_STATION_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11048_BASE_ADDRESS */
#define B1_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_OFFSET_FROM_BAR0                     (B1_TWT_GROUP_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11027_BASE_ADDRESS */
#define B0_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_OFFSET_FROM_BAR0                    (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001E00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_OFFSET_FROM_BAR0                    (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001E00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_OFFSET_FROM_BAR0                     (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_OFFSET_FROM_BAR0                     (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_OFFSET_FROM_BAR0                     (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_DUR_AUTO_RATE_DUR_MEM_OFFSET_FROM_BAR0                                 (B0_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1055_BASE_ADDRESS */
#define B1_DUR_AUTO_RATE_DUR_MEM_OFFSET_FROM_BAR0                                 (B1_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11055_BASE_ADDRESS */
#define B1_MAC_ADDR2INDEX_MEM_OFFSET_FROM_BAR0                                    (B1_MAC_ADDR2INDEX_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10022_BASE_ADDRESS */
#define HOST_IF_ACC_FW_CODE_RAM_OFFSET_FROM_BAR0                                  (HOST_IF_ACC_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_39_BASE_ADDRESS */
#define B0_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM_OFFSET_FROM_BAR0            (B0_BESTRU_OFFSET_FROM_BAR0 + 0x00000100) /* MEMORY_MAP_UNIT_66_BASE_ADDRESS */
#define B0_RXC_RXC_FIELDS_CATCHER_MEM_OFFSET_FROM_BAR0                            (B0_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_1052_BASE_ADDRESS */
#define B01_RX_PP_STA_BITMAP_RAM_OFFSET_FROM_BAR0                                 (RX_PP_M01_OFFSET_FROM_BAR0 + 0x00009200) /* MEMORY_MAP_UNIT_1084_BASE_ADDRESS */
#define B2_RX_PP_STA_BITMAP_RAM_OFFSET_FROM_BAR0                                  (RX_PP_M2_OFFSET_FROM_BAR0 + 0x00009200) /* MEMORY_MAP_UNIT_1284_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM0_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM1_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM2_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM3_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM4_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM5_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM6_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RX_MEM7_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_RX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1106_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM0_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM1_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM2_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM3_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM4_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM5_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM6_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_TX_MEM7_OFFSET_FROM_BAR0                               (DESCRIPTOR_RAM_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1103_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM0_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM1_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM2_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM3_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM4_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM5_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM6_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_DESCRIPTOR_RAM_RXTX_MEM7_OFFSET_FROM_BAR0                             (DESCRIPTOR_RAM_RX_TX_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1104_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM0_OFFSET_FROM_BAR0                                        (PLAN_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM1_OFFSET_FROM_BAR0                                        (PLAN_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM2_OFFSET_FROM_BAR0                                        (PLAN_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_PLAN_RAM_MEM3_OFFSET_FROM_BAR0                                        (PLAN_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1101_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM0_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM1_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM2_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM3_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM4_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM5_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM6_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define MAC_SHARED_RAM_MEM7_OFFSET_FROM_BAR0                                      (SHARED_RAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1018_BASE_ADDRESS */
#define B0_TX_SELECTOR_VAP_TID_RAM_OFFSET_FROM_BAR0                               (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B1_TX_SELECTOR_VAP_TID_RAM_OFFSET_FROM_BAR0                               (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_OFFSET_FROM_BAR0                    (B0_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1058_BASE_ADDRESS */
#define B1_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_OFFSET_FROM_BAR0                    (B1_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11058_BASE_ADDRESS */
#define B0_OTFA_TOP_FRAG_DB_MEM_I_OFFSET_FROM_BAR0                                (B0_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_1073_BASE_ADDRESS */
#define B1_OTFA_TOP_FRAG_DB_MEM_I_OFFSET_FROM_BAR0                                (B1_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_11073_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_VAP_RAM_OFFSET_FROM_BAR0                               (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001D00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_MLD_DB_TOP_MLD_DB_MEMORY_OFFSET_FROM_BAR0                              (B0_MLD_DB_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1110_BASE_ADDRESS */
#define B1_MLD_DB_TOP_MLD_DB_MEMORY_OFFSET_FROM_BAR0                              (B1_MLD_DB_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11110_BASE_ADDRESS */
#define B01_RX_PP_RX_PP_STATISTICS_RAM_OFFSET_FROM_BAR0                           (RX_PP_COUNTERS_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1094_BASE_ADDRESS */
#define B2_RX_PP_RX_PP_STATISTICS_RAM_OFFSET_FROM_BAR0                            (RX_PP_COUNTERS_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1194_BASE_ADDRESS */
#define EXT_RX_PP_COUNTERS_EXT_RX_PP_STATISTICS_RAM_OFFSET_FROM_BAR0              (EXT_RX_PP_COUNTERS_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1042_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_USER_ATTR_MEM_OFFSET_FROM_BAR0                             (B1_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_KEY_MEM_OFFSET_FROM_BAR0                                   (B1_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_0_OFFSET_FROM_BAR0                            (B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001B00) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_1_OFFSET_FROM_BAR0                            (B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001F00) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_2_OFFSET_FROM_BAR0                            (B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00002300) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B0_AUTO_RESP_TID_BITMAP_MEM_3_OFFSET_FROM_BAR0                            (B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00002700) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_0_OFFSET_FROM_BAR0                            (B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001B00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_1_OFFSET_FROM_BAR0                            (B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001D00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_2_OFFSET_FROM_BAR0                            (B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001F00) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B1_AUTO_RESP_TID_BITMAP_MEM_3_OFFSET_FROM_BAR0                            (B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00002100) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_0_OFFSET_FROM_BAR0                            (B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001B00) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_1_OFFSET_FROM_BAR0                            (B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00001F00) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_2_OFFSET_FROM_BAR0                            (B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00002300) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B2_AUTO_RESP_TID_BITMAP_MEM_3_OFFSET_FROM_BAR0                            (B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00002700) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B0_AUTO_RESP_USER_DB_MEM_OFFSET_FROM_BAR0                                 (B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1053_BASE_ADDRESS */
#define B1_AUTO_RESP_USER_DB_MEM_OFFSET_FROM_BAR0                                 (B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11053_BASE_ADDRESS */
#define B2_AUTO_RESP_USER_DB_MEM_OFFSET_FROM_BAR0                                 (B2_AUTO_RESP_MEMS_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21053_BASE_ADDRESS */
#define B0_MAC_BFM_BEST_RU_DB_OFFSET_FROM_BAR0                                    (B0_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000600) /* MEMORY_MAP_UNIT_11_BASE_ADDRESS */
#define B0_BAA_TOP_RA_USER_MEM_OFFSET_FROM_BAR0                                   (B0_BA_ANALYZER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_45_BASE_ADDRESS */
#define B1_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM_OFFSET_FROM_BAR0            (B1_BESTRU_OFFSET_FROM_BAR0 + 0x00000100) /* MEMORY_MAP_UNIT_10066_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_OFFSET_FROM_BAR0                    (B0_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1031_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_OFFSET_FROM_BAR0                    (B1_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11031_BASE_ADDRESS */
#define B0_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_OFFSET_FROM_BAR0                      (B0_TX_PD_ACC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1097_BASE_ADDRESS */
#define B0_OTFA_TOP_LOCAL_DB_MEM_I_OFFSET_FROM_BAR0                               (B0_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1073_BASE_ADDRESS */
#define LOGGER_WRAPPER_UDP_OFFSET_FROM_BAR0                                       (DATA_LOGGER_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_61_BASE_ADDRESS */
#define B0_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_OFFSET_FROM_BAR0                    (B0_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_23_BASE_ADDRESS */
#define B0_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_OFFSET_FROM_BAR0                    (B0_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_23_BASE_ADDRESS */
#define B1_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_OFFSET_FROM_BAR0                    (B1_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10023_BASE_ADDRESS */
#define B1_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_OFFSET_FROM_BAR0                    (B1_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_10023_BASE_ADDRESS */
#define B01_Q_MANAGER_PD_DLMS_RAM_OFFSET_FROM_BAR0                                (DLM_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_PD_DLMS_RAM_OFFSET_FROM_BAR0                                 (DLM_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0               (BA_ANALYZER_COUNTER_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1049_BASE_ADDRESS */
#define B2_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0                (BA_ANALYZER_COUNTER_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1149_BASE_ADDRESS */
#define B01_RETRY_STATE_DB_DUAL_BAND_RAM_OFFSET_FROM_BAR0                         (RETRY_STATE_DB_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1056_BASE_ADDRESS */
#define B2_RETRY_STATE_DB_DUAL_BAND_RAM_OFFSET_FROM_BAR0                          (RETRY_STATE_DB_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1156_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_OFFSET_FROM_BAR0                     (B0_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1029_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_OFFSET_FROM_BAR0                     (B1_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11029_BASE_ADDRESS */
#define B0_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_OFFSET_FROM_BAR0                      (B0_TF_GENERATOR_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1085_BASE_ADDRESS */
#define B1_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_OFFSET_FROM_BAR0                      (B1_TF_GENERATOR_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11085_BASE_ADDRESS */
#define B0_TF_DECODER_TF_DEC_TX_POWER_MEM_OFFSET_FROM_BAR0                        (B0_TF_DECODER_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1041_BASE_ADDRESS */
#define B1_TF_DECODER_TF_DEC_TX_POWER_MEM_OFFSET_FROM_BAR0                        (B1_TF_DECODER_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11041_BASE_ADDRESS */
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_OFFSET_FROM_BAR0                     (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_TID_RAM0_OFFSET_FROM_BAR0                              (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_TID_RAM1_OFFSET_FROM_BAR0                              (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_USER_ATTR_MEM_OFFSET_FROM_BAR0                             (B0_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_KEY_MEM_OFFSET_FROM_BAR0                                   (B0_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define B0_RX_CLASSIFIER_RX_CLASSIFIER_MEM_OFFSET_FROM_BAR0                       (B0_RX_CLASSIFIER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_57_BASE_ADDRESS */
#define B1_RX_CLASSIFIER_RX_CLASSIFIER_MEM_OFFSET_FROM_BAR0                       (B1_RX_CLASSIFIER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10057_BASE_ADDRESS */
#define HOST_IF_ACC_DIST_RAM_OFFSET_FROM_BAR0                                     (HOST_IF_ACC_OFFSET_FROM_BAR0 + 0x00000c00) /* MEMORY_MAP_UNIT_39_BASE_ADDRESS */
#define B01_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM_OFFSET_FROM_BAR0                     (DLM_M01_OFFSET_FROM_BAR0 + 0x0000A5C8) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B01_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM_OFFSET_FROM_BAR0                     (DLM_M01_OFFSET_FROM_BAR0 + 0x0000A708) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM_OFFSET_FROM_BAR0                      (DLM_M2_OFFSET_FROM_BAR0 + 0x0000A5C8) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B2_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM_OFFSET_FROM_BAR0                      (DLM_M2_OFFSET_FROM_BAR0 + 0x0000A708) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define B01_Q_MANAGER_DMA_DLMS_RAM_OFFSET_FROM_BAR0                               (DLM_M01_OFFSET_FROM_BAR0 + 0x0000A480) /* MEMORY_MAP_UNIT_40_BASE_ADDRESS */
#define B2_Q_MANAGER_DMA_DLMS_RAM_OFFSET_FROM_BAR0                                (DLM_M2_OFFSET_FROM_BAR0 + 0x0000A480) /* MEMORY_MAP_UNIT_1117_BASE_ADDRESS */
#define HOST_IF_AQM_CFG_MEM_OFFSET_FROM_BAR0                                      (HOST_IF_AQM_STAT_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1109_BASE_ADDRESS */
#define HOST_IF_AQM_STAT_BURST_MEM_OFFSET_FROM_BAR0                               (HOST_IF_AQM_STAT_OFFSET_FROM_BAR0 + 0x00012000) /* MEMORY_MAP_UNIT_1109_BASE_ADDRESS */
#define B0_PRE_AGG_PRE_AGG_MEM_OFFSET_FROM_BAR0                                   (B0_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_PRE_AGG_MEM_OFFSET_FROM_BAR0                                   (B1_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B1_MAC_BFM_MBFM_TASK_SW_DB_L_OFFSET_FROM_BAR0                             (B1_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_10011_BASE_ADDRESS */
#define B1_SEC_ENG_TOP_MIC_MEM_OFFSET_FROM_BAR0                                   (B1_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_11077_BASE_ADDRESS */
#define B0_MAC_BFM_MBFM_TASK_SW_DB_L_OFFSET_FROM_BAR0                             (B0_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_11_BASE_ADDRESS */
#define B0_SEC_ENG_TOP_MIC_MEM_OFFSET_FROM_BAR0                                   (B0_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_1077_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_6_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00180000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_7_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x001C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_8_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00200000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_9_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00240000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_10_IRAM_LOW_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00280000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_11_IRAM_LOW_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x002C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_12_IRAM_LOW_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00300000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_13_IRAM_LOW_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00340000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define HOST_IF_CONTEXT_SAVE_MEMORY_OFFSET_FROM_BAR0                              (MAC_HOSTIF_UC_INTERNAL_OFFSET_FROM_BAR0 + 0x0000A000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_ICCM_RAM0_OFFSET_FROM_BAR0                                      (MAC_HOSTIF_UC_INTERNAL_OFFSET_FROM_BAR0 + 0x00010000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_DCCM_RAM_BANK0_OFFSET_FROM_BAR0                                 (MAC_HOSTIF_UC_INTERNAL_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define HOST_IF_U_DCCM_RAM_BANK1_OFFSET_FROM_BAR0                                 (MAC_HOSTIF_UC_INTERNAL_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1120_BASE_ADDRESS */
#define B2_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM_OFFSET_FROM_BAR0               (BEST_RU_DB_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1006_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_OFFSET_FROM_BAR0                   (B2_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21033_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_OFFSET_FROM_BAR0                   (B2_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21026_BASE_ADDRESS */
#define B2_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_OFFSET_FROM_BAR0                       (B2_TWT_STATION_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21048_BASE_ADDRESS */
#define B2_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_OFFSET_FROM_BAR0                     (B2_TWT_GROUP_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21027_BASE_ADDRESS */
#define B2_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_OFFSET_FROM_BAR0                    (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001E00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_OFFSET_FROM_BAR0                     (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_OFFSET_FROM_BAR0                     (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_DUR_AUTO_RATE_DUR_MEM_OFFSET_FROM_BAR0                                 (B2_DUR_AUTO_RATE_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21055_BASE_ADDRESS */
#define B2_BESTRU_SCRATCH_MEM_WRAP_BESTRU_SCRATCH_MEM_OFFSET_FROM_BAR0            (B2_BESTRU_OFFSET_FROM_BAR0 + 0x00000100) /* MEMORY_MAP_UNIT_20066_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_TID_RAM0_OFFSET_FROM_BAR0                              (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_TID_RAM1_OFFSET_FROM_BAR0                              (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_RXF_RXF_MEM_OFFSET_FROM_BAR0                                           (B2_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21052_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_OFFSET_FROM_BAR0                    (B2_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21024_BASE_ADDRESS */
#define B2_PRE_AGG_TOP_PARAMS_CALC_MEM_OFFSET_FROM_BAR0                           (B2_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_MAC_ADDR2INDEX_MEM_OFFSET_FROM_BAR0                                    (B2_MAC_ADDR2INDEX_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20022_BASE_ADDRESS */
#define B2_RXC_RXC_FIELDS_CATCHER_MEM_OFFSET_FROM_BAR0                            (B2_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_21052_BASE_ADDRESS */
#define B2_TX_SELECTOR_VAP_TID_RAM_OFFSET_FROM_BAR0                               (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001A00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_OFFSET_FROM_BAR0                    (B2_FRAME_CLASS_VIO_BUFFER_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21058_BASE_ADDRESS */
#define B2_OTFA_TOP_FRAG_DB_MEM_I_OFFSET_FROM_BAR0                                (B2_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_21073_BASE_ADDRESS */
#define B2_TX_SELECTOR_STA_VAP_RAM_OFFSET_FROM_BAR0                               (B2_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001D00) /* MEMORY_MAP_UNIT_20029_BASE_ADDRESS */
#define B2_MLD_DB_TOP_MLD_DB_MEMORY_OFFSET_FROM_BAR0                              (B2_MLD_DB_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21110_BASE_ADDRESS */
#define B2_MAC_BFM_BEST_RU_DB_OFFSET_FROM_BAR0                                    (B2_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000600) /* MEMORY_MAP_UNIT_20011_BASE_ADDRESS */
#define B2_BAA_TOP_RA_USER_MEM_OFFSET_FROM_BAR0                                   (B2_BA_ANALYZER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20045_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_OFFSET_FROM_BAR0                    (B2_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21031_BASE_ADDRESS */
#define B2_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_OFFSET_FROM_BAR0                      (B2_TX_PD_ACC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21097_BASE_ADDRESS */
#define B2_OTFA_TOP_LOCAL_DB_MEM_I_OFFSET_FROM_BAR0                               (B2_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21073_BASE_ADDRESS */
#define B2_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_OFFSET_FROM_BAR0                    (B2_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20023_BASE_ADDRESS */
#define B2_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_OFFSET_FROM_BAR0                    (B2_TX_HANDLER_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_20023_BASE_ADDRESS */
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_OFFSET_FROM_BAR0                     (B2_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21029_BASE_ADDRESS */
#define B2_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_OFFSET_FROM_BAR0                      (B2_TF_GENERATOR_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21085_BASE_ADDRESS */
#define B2_TF_DECODER_TF_DEC_TX_POWER_MEM_OFFSET_FROM_BAR0                        (B2_TF_DECODER_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21041_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_USER_ATTR_MEM_OFFSET_FROM_BAR0                             (B2_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_KEY_MEM_OFFSET_FROM_BAR0                                   (B2_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_MEM_OFFSET_FROM_BAR0                       (B2_RX_CLASSIFIER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_20057_BASE_ADDRESS */
#define B2_PRE_AGG_PRE_AGG_MEM_OFFSET_FROM_BAR0                                   (B2_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_MAC_BFM_MBFM_TASK_SW_DB_L_OFFSET_FROM_BAR0                             (B2_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_20011_BASE_ADDRESS */
#define B2_SEC_ENG_TOP_MIC_MEM_OFFSET_FROM_BAR0                                   (B2_MAC_SEC_MEM_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_21077_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_TID_RAM0_OFFSET_FROM_BAR0                              (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_TX_SELECTOR_STA_TID_RAM1_OFFSET_FROM_BAR0                              (B0_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00000A00) /* MEMORY_MAP_UNIT_29_BASE_ADDRESS */
#define B0_RXF_RXF_MEM_OFFSET_FROM_BAR0                                           (B0_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1052_BASE_ADDRESS */
#define B1_RXF_RXF_MEM_OFFSET_FROM_BAR0                                           (B1_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11052_BASE_ADDRESS */
#define B1_RXC_RXC_FIELDS_CATCHER_MEM_OFFSET_FROM_BAR0                            (B1_RXF_MEM_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_11052_BASE_ADDRESS */
#define B01_DMAC_WRAPPER_U_LL_MEM_OFFSET_FROM_BAR0                                (WLAN_DMA_LL_MEM_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1008_BASE_ADDRESS */
#define B2_DMAC_WRAPPER_U_LL_MEM_OFFSET_FROM_BAR0                                 (WLAN_DMA_LL_MEM_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1208_BASE_ADDRESS */
#define B01_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM_OFFSET_FROM_BAR0              (BEST_RU_DB_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1038_BASE_ADDRESS */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_OFFSET_FROM_BAR0                    (B0_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1024_BASE_ADDRESS */
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_OFFSET_FROM_BAR0                    (B1_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11024_BASE_ADDRESS */
#define B01_RETRY_POINTER_DB_DUAL_BAND_RAM_0_OFFSET_FROM_BAR0                     (RETRY_POINTER_DB_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1068_BASE_ADDRESS */
#define B2_RETRY_POINTER_DB_DUAL_BAND_RAM_0_OFFSET_FROM_BAR0                      (RETRY_POINTER_DB_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1168_BASE_ADDRESS */
#define B01_TRAFFIC_INDICATOR_MEM_OFFSET_FROM_BAR0                                (TRAFFIC_INDICATOR_M01_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_1054_BASE_ADDRESS */
#define B2_TRAFFIC_INDICATOR_MEM_OFFSET_FROM_BAR0                                 (TRAFFIC_INDICATOR_M2_OFFSET_FROM_BAR0 + 0x00000400) /* MEMORY_MAP_UNIT_1154_BASE_ADDRESS */
#define B1_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_OFFSET_FROM_BAR0                      (B1_TX_PD_ACC_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11097_BASE_ADDRESS */
#define B1_MAC_BFM_BEST_RU_DB_OFFSET_FROM_BAR0                                    (B1_MAC_BFM_OFFSET_FROM_BAR0 + 0x00000600) /* MEMORY_MAP_UNIT_10011_BASE_ADDRESS */
#define B1_BAA_TOP_RA_USER_MEM_OFFSET_FROM_BAR0                                   (B1_BA_ANALYZER_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10045_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM0_OFFSET_FROM_BAR0                  (HOST_IF_COUNTERS_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1075_BASE_ADDRESS */
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM1_OFFSET_FROM_BAR0                  (HOST_IF_COUNTERS_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1175_BASE_ADDRESS */
#define B01_RX_PP_PN_SN_RAM_OFFSET_FROM_BAR0                                      (RX_PP_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1084_BASE_ADDRESS */
#define B2_RX_PP_PN_SN_RAM_OFFSET_FROM_BAR0                                       (RX_PP_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1284_BASE_ADDRESS */
#define PROT_DB_SMC_LOCK_MEM_OFFSET_FROM_BAR0                                     (PROT_DB_SHADOW_MEM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1015_BASE_ADDRESS */
#define B1_OTFA_TOP_LOCAL_DB_MEM_I_OFFSET_FROM_BAR0                               (B1_OTFA_MEM2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_11073_BASE_ADDRESS */
#define B0_PRE_AGG_TOP_PARAMS_CALC_MEM_OFFSET_FROM_BAR0                           (B0_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_TOP_PARAMS_CALC_MEM_OFFSET_FROM_BAR0                           (B1_PRE_AGG_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B01_BSRC_AGER_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0                        (BSRC_AGER_MEM_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1017_BASE_ADDRESS */
#define B2_BSRC_AGER_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0                         (BSRC_AGER_MEM_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1121_BASE_ADDRESS */
#define B01_BSRC_CNT_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0                         (BSRC_CNT_MEM_M01_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_67_BASE_ADDRESS */
#define B2_BSRC_CNT_MEM_COMMON_BAND_RAM_OFFSET_FROM_BAR0                          (BSRC_CNT_MEM_M2_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1167_BASE_ADDRESS */
#define B0_MAC_ADDR2INDEX_MEM_OFFSET_FROM_BAR0                                    (B0_MAC_ADDR2INDEX_OFFSET_FROM_BAR0 + 0x00000800) /* MEMORY_MAP_UNIT_22_BASE_ADDRESS */
#define B1_TX_SELECTOR_STA_VAP_RAM_OFFSET_FROM_BAR0                               (B1_TX_SELECTOR_OFFSET_FROM_BAR0 + 0x00001D00) /* MEMORY_MAP_UNIT_10029_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_0_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_1_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00040000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_2_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00080000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_3_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x000C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_4_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00100000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_5_IRAM_LOW_OFFSET_FROM_BAR0                               (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00140000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define B0_RX_RCR_RCR_RAM_CDB0_OFFSET_FROM_BAR0                                   (B0_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_201_BASE_ADDRESS */
#define B1_RX_RCR_RCR_RAM_CDB1_OFFSET_FROM_BAR0                                   (B1_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10201_BASE_ADDRESS */
#define B0_RX_SIGB_SIGB_RAM_CDB0_OFFSET_FROM_BAR0                                 (B0_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_201_BASE_ADDRESS */
#define B1_RX_SIGB_SIGB_RAM_CDB1_OFFSET_FROM_BAR0                                 (B1_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10201_BASE_ADDRESS */
#define B2_RX_SIGB_SIGB_RAM_CDB0_OFFSET_FROM_BAR0                                 (B2_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20201_BASE_ADDRESS */
#define B2_RX_RCR_RCR_RAM_CDB0_OFFSET_FROM_BAR0                                   (B2_PHY_RX_BE_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20201_BASE_ADDRESS */
#define B0_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_OFFSET_FROM_BAR0            (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_OFFSET_FROM_BAR0            (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_RXFD_CSM_RCR_RAM_OFFSET_FROM_BAR0                                  (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005300) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_OFFSET_FROM_BAR0                      (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002E00) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_FILTER_SELECT_FILTER_RAM_OFFSET_FROM_BAR0                              (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_LO_OFFSET_FROM_BAR0               (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_HI_OFFSET_FROM_BAR0               (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_OFFSET_FROM_BAR0                      (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_RXFD_BIN_CHOOSER_NE_RAM_OFFSET_FROM_BAR0                           (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00008000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_LINEAR_SLICER_EVM_MU_OFFSET_FROM_BAR0                                  (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_CH_RAM_0_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_0_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_1_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_1_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_2_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_2_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_3_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_CH_RAM_3_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_OFFSET_FROM_BAR0 (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00006000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MCS_SNR_MCS_SNR_RAM_OFFSET_FROM_BAR0                                   (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002400) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_OFFSET_FROM_BAR0    (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_OFFSET_FROM_BAR0    (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005080) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_OFFSET_FROM_BAR0    (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005100) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_OFFSET_FROM_BAR0    (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005180) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_HDR_MU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002C00) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_OFFSET_FROM_BAR0                 (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_OFFSET_FROM_BAR0                 (B0_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_212_BASE_ADDRESS */
#define B0_SU_HDR_SU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B0_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_202_BASE_ADDRESS */
#define B0_HYP_TXFD_TXTD_SCALE_RAM_OFFSET_FROM_BAR0                               (B0_PHY_TX_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B1_LONG_PREAMBLE_LONG_PREAMBLE_RAM_OFFSET_FROM_BAR0                       (B1_PHY_TX_OFFSET_FROM_BAR0 + 0x00003800) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B1_HYP_TXFD_TXTD_SCALE_RAM_OFFSET_FROM_BAR0                               (B1_PHY_TX_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B0_HYP_TX_EXT_CONTROL_OFFSET_FROM_BAR0                                    (B0_PHY_TX_OFFSET_FROM_BAR0 + 0x00003C00) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B1_HYP_TX_EXT_CONTROL_OFFSET_FROM_BAR0                                    (B1_PHY_TX_OFFSET_FROM_BAR0 + 0x00003C00) /* MEMORY_MAP_UNIT_10203_BASE_ADDRESS */
#define B0_LONG_PREAMBLE_LONG_PREAMBLE_RAM_OFFSET_FROM_BAR0                       (B0_PHY_TX_OFFSET_FROM_BAR0 + 0x00003800) /* MEMORY_MAP_UNIT_203_BASE_ADDRESS */
#define B2_LONG_PREAMBLE_LONG_PREAMBLE_RAM_OFFSET_FROM_BAR0                       (B2_PHY_TX_OFFSET_FROM_BAR0 + 0x00003800) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B2_HYP_TXFD_TXTD_SCALE_RAM_OFFSET_FROM_BAR0                               (B2_PHY_TX_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B2_UP_LONG_PREAMBLE_LONG_PREAMBLE_RAM_OFFSET_FROM_BAR0                    (B2_UP_PHY_TX_OFFSET_FROM_BAR0 + 0x00003800) /* MEMORY_MAP_UNIT_20205_BASE_ADDRESS */
#define B2_UP_HYP_TXFD_TXTD_SCALE_RAM_OFFSET_FROM_BAR0                            (B2_UP_PHY_TX_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20205_BASE_ADDRESS */
#define B2_HYP_TX_EXT_CONTROL_OFFSET_FROM_BAR0                                    (B2_PHY_TX_OFFSET_FROM_BAR0 + 0x00003C00) /* MEMORY_MAP_UNIT_20203_BASE_ADDRESS */
#define B0_DC_ESTIMATION_CFO_THRESHOLD_RAM_OFFSET_FROM_BAR0                       (B0_PHY_RX_TD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_200_BASE_ADDRESS */
#define B1_DC_ESTIMATION_CFO_THRESHOLD_RAM_OFFSET_FROM_BAR0                       (B1_PHY_RX_TD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10200_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_0_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_1_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_2_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_3_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_4_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_5_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_6_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_7_OFFSET_FROM_BAR0                      (B0_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_209_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_IQ_RAM_OFFSET_FROM_BAR0                               (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_IQ_RAM_OFFSET_FROM_BAR0                               (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_IQ_RAM_OFFSET_FROM_BAR0                               (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_IQ_RAM_OFFSET_FROM_BAR0                               (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_CAL_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_CAL_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_CAL_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_CAL_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DELAY_FIFO_OFFSET_FROM_BAR0                           (B0_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DELAY_FIFO_OFFSET_FROM_BAR0                           (B0_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DELAY_FIFO_OFFSET_FROM_BAR0                           (B0_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DELAY_FIFO_OFFSET_FROM_BAR0                           (B0_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B0_GEN_RISC_GEN_RISC_IRAM_OFFSET_FROM_BAR0                                (B0_PHY_RISC_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_208_BASE_ADDRESS */
#define B1_GEN_RISC_GEN_RISC_IRAM_OFFSET_FROM_BAR0                                (B1_PHY_RISC_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10208_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_FFT_0_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_FFT_0_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_FFT_0_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_FFT_0_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_FFT_1_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_FFT_1_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_FFT_1_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_FFT_1_RAM_OFFSET_FROM_BAR0                            (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B0_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_211_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B1_RADAR_DETECTOR_EVENTS_RAM_OFFSET_FROM_BAR0                             (B1_PHY_RX_TD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10200_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_TPC_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_230_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_TPC_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_231_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_TPC_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_232_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_TPC_RAM_OFFSET_FROM_BAR0                              (B0_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_233_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B0_PHY_AGC_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_220_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B0_PHY_AGC_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_221_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B0_PHY_AGC_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_222_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B0_PHY_AGC_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_223_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B1_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B1_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B1_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B1_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B1_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B1_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B1_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B1_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_IQ_RAM_OFFSET_FROM_BAR0                               (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_IQ_RAM_OFFSET_FROM_BAR0                               (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_IQ_RAM_OFFSET_FROM_BAR0                               (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_IQ_RAM_OFFSET_FROM_BAR0                               (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_CAL_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_CAL_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_CAL_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_CAL_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DELAY_FIFO_OFFSET_FROM_BAR0                           (B1_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10240_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DELAY_FIFO_OFFSET_FROM_BAR0                           (B1_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10241_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DELAY_FIFO_OFFSET_FROM_BAR0                           (B1_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10242_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DELAY_FIFO_OFFSET_FROM_BAR0                           (B1_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10243_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_FFT_0_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_FFT_1_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_FFT_0_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_FFT_1_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_FFT_0_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_FFT_1_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_FFT_0_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_FFT_1_RAM_OFFSET_FROM_BAR0                            (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B1_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10211_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_TPC_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10230_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_TPC_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10231_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_TPC_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10232_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_TPC_RAM_OFFSET_FROM_BAR0                              (B1_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10233_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B1_PHY_AGC_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10220_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B1_PHY_AGC_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10221_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B1_PHY_AGC_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10222_BASE_ADDRESS */
#define B1_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B1_PHY_AGC_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10223_BASE_ADDRESS */
#define B0_GEN_RISC_GEN_RISC_SRAM_OFFSET_FROM_BAR0                                (B0_PHY_RISC_OFFSET_FROM_BAR0 + 0x0000C000) /* MEMORY_MAP_UNIT_208_BASE_ADDRESS */
#define B1_GEN_RISC_GEN_RISC_SRAM_OFFSET_FROM_BAR0                                (B1_PHY_RISC_OFFSET_FROM_BAR0 + 0x0000C000) /* MEMORY_MAP_UNIT_10208_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B0_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B0_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B0_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B0_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_0_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B0_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_240_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_1_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B0_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_241_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_2_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B0_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_242_BASE_ADDRESS */
#define B0_HYP_TD_ANTENNA_3_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B0_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_243_BASE_ADDRESS */
#define B2_GEN_RISC_GEN_RISC_SRAM_OFFSET_FROM_BAR0                                (B2_PHY_RISC_OFFSET_FROM_BAR0 + 0x0000C000) /* MEMORY_MAP_UNIT_20208_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B2_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B2_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B2_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                         (B2_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B2_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B2_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B2_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_BAND_SEL_COEF_OFFSET_FROM_BAR0                        (B2_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_DC_ESTIMATION_CFO_THRESHOLD_RAM_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20200_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_0_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_1_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_2_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_3_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_4_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_5_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_6_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_7_OFFSET_FROM_BAR0                       (B2_PHY_TEST_BUS_OR_DUMMY_PHY_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20209_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RIPPLE_RAM_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_IQ_RAM_OFFSET_FROM_BAR0                               (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_IQ_RAM_OFFSET_FROM_BAR0                               (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_IQ_RAM_OFFSET_FROM_BAR0                               (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_IQ_RAM_OFFSET_FROM_BAR0                               (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_CAL_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_CAL_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_CAL_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_CAL_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DELAY_FIFO_OFFSET_FROM_BAR0                           (B2_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20240_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DELAY_FIFO_OFFSET_FROM_BAR0                           (B2_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20241_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DELAY_FIFO_OFFSET_FROM_BAR0                           (B2_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20242_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DELAY_FIFO_OFFSET_FROM_BAR0                           (B2_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20243_BASE_ADDRESS */
#define B2_GEN_RISC_GEN_RISC_IRAM_OFFSET_FROM_BAR0                                (B2_PHY_RISC_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20208_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_0_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_FFT_0_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_FFT_1_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_FFT_0_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_FFT_1_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_FFT_0_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_FFT_1_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_FFT_0_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_FFT_1_RAM_OFFSET_FROM_BAR0                            (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_1_OFFSET_FROM_BAR0                          (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004400) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO_OFFSET_FROM_BAR0                           (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00004C00) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_CFG_OFFSET_FROM_BAR0                          (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_TPC_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_TPC_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_TPC_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_TPC_RAM_OFFSET_FROM_BAR0                              (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B2_PHY_AGC_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20220_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B2_PHY_AGC_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20221_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B2_PHY_AGC_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20222_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_OFFSET_FROM_BAR0                        (B2_PHY_AGC_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20223_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_BAND_SEL_COEF_OFFSET_FROM_BAR0                     (B2_UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20244_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_BAND_SEL_COEF_OFFSET_FROM_BAR0                     (B2_UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20245_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_BAND_SEL_COEF_OFFSET_FROM_BAR0                     (B2_UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20246_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_BAND_SEL_COEF_OFFSET_FROM_BAR0                     (B2_UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20247_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RIPPLE_RAM_OFFSET_FROM_BAR0                        (B2_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RIPPLE_RAM_OFFSET_FROM_BAR0                        (B2_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RIPPLE_RAM_OFFSET_FROM_BAR0                        (B2_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RIPPLE_RAM_OFFSET_FROM_BAR0                        (B2_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_IQ_RAM_OFFSET_FROM_BAR0                            (B2_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_IQ_RAM_OFFSET_FROM_BAR0                            (B2_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_IQ_RAM_OFFSET_FROM_BAR0                            (B2_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_IQ_RAM_OFFSET_FROM_BAR0                            (B2_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_CAL_RAM_OFFSET_FROM_BAR0                           (B2_UP_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20234_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_CAL_RAM_OFFSET_FROM_BAR0                           (B2_UP_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20235_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_CAL_RAM_OFFSET_FROM_BAR0                           (B2_UP_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20236_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_CAL_RAM_OFFSET_FROM_BAR0                           (B2_UP_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_20237_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_DELAY_FIFO_OFFSET_FROM_BAR0                        (B2_UP_PHY_RXTD_ANT0_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20244_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_DELAY_FIFO_OFFSET_FROM_BAR0                        (B2_UP_PHY_RXTD_ANT1_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20245_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_DELAY_FIFO_OFFSET_FROM_BAR0                        (B2_UP_PHY_RXTD_ANT2_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20246_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_DELAY_FIFO_OFFSET_FROM_BAR0                        (B2_UP_PHY_RXTD_ANT3_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_20247_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_0_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_0_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_0_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_0_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_FFT_0_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_FFT_1_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_FFT_0_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_FFT_1_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_FFT_0_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_FFT_1_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_FFT_0_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_FFT_1_RAM_OFFSET_FROM_BAR0                         (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_1_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_1_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_1_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_1_OFFSET_FROM_BAR0                       (B2_PHY_RX_TD_DEBUG_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20211_BASE_ADDRESS */
#define B1_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_OFFSET_FROM_BAR0            (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_OFFSET_FROM_BAR0            (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HYP_RXFD_CSM_RCR_RAM_OFFSET_FROM_BAR0                                  (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005300) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_OFFSET_FROM_BAR0                      (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002E00) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_FILTER_SELECT_FILTER_RAM_OFFSET_FROM_BAR0                              (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_OFFSET_FROM_BAR0                   (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_OFFSET_FROM_BAR0                   (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_OFFSET_FROM_BAR0                   (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_OFFSET_FROM_BAR0                   (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_OFFSET_FROM_BAR0                      (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HYP_RXFD_BIN_CHOOSER_NE_RAM_OFFSET_FROM_BAR0                           (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00008000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_LINEAR_SLICER_EVM_MU_OFFSET_FROM_BAR0                                  (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_0_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_1_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_2_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_CH_RAM_3_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_OFFSET_FROM_BAR0 (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00006000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MCS_SNR_MCS_SNR_RAM_OFFSET_FROM_BAR0                                   (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002400) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_OFFSET_FROM_BAR0    (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_OFFSET_FROM_BAR0    (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005080) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_OFFSET_FROM_BAR0    (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005100) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_OFFSET_FROM_BAR0    (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005180) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_HDR_MU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002C00) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B1_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_OFFSET_FROM_BAR0                 (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_OFFSET_FROM_BAR0                 (B1_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10212_BASE_ADDRESS */
#define B1_SU_HDR_SU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B1_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10202_BASE_ADDRESS */
#define B2_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_OFFSET_FROM_BAR0            (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_OFFSET_FROM_BAR0            (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HYP_RXFD_CSM_RCR_RAM_OFFSET_FROM_BAR0                                  (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005300) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_OFFSET_FROM_BAR0                      (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002E00) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_FILTER_SELECT_FILTER_RAM_OFFSET_FROM_BAR0                              (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_OFFSET_FROM_BAR0                   (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_OFFSET_FROM_BAR0                   (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_OFFSET_FROM_BAR0                   (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_OFFSET_FROM_BAR0                   (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_OFFSET_FROM_BAR0                      (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HYP_RXFD_BIN_CHOOSER_NE_RAM_OFFSET_FROM_BAR0                           (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00008000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_LINEAR_SLICER_EVM_MU_OFFSET_FROM_BAR0                                  (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_0_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_1_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_2_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_CH_RAM_3_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                             (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_OFFSET_FROM_BAR0 (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00006000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MCS_SNR_MCS_SNR_RAM_OFFSET_FROM_BAR0                                   (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002400) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_OFFSET_FROM_BAR0    (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_OFFSET_FROM_BAR0    (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005080) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_OFFSET_FROM_BAR0    (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005100) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_OFFSET_FROM_BAR0    (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005180) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_HDR_MU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002C00) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_OFFSET_FROM_BAR0                 (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_OFFSET_FROM_BAR0                 (B2_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20212_BASE_ADDRESS */
#define B2_SU_HDR_SU_HDR_MEMORY_OFFSET_FROM_BAR0                                  (B2_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20202_BASE_ADDRESS */
#define B2_UP_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_OFFSET_FROM_BAR0         (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_OFFSET_FROM_BAR0         (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HYP_RXFD_CSM_RCR_RAM_OFFSET_FROM_BAR0                               (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005300) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_OFFSET_FROM_BAR0                   (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002E00) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_FILTER_SELECT_FILTER_RAM_OFFSET_FROM_BAR0                           (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_OFFSET_FROM_BAR0                (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_OFFSET_FROM_BAR0                (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_OFFSET_FROM_BAR0                (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_OFFSET_FROM_BAR0                (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_OFFSET_FROM_BAR0                   (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HYP_RXFD_BIN_CHOOSER_NE_RAM_OFFSET_FROM_BAR0                        (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00008000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_LINEAR_SLICER_EVM_MU_OFFSET_FROM_BAR0                               (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_R_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_L_OFFSET_FROM_BAR0                          (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B1_DFS_GEN_RISC_GEN_RISC_IRAM_OFFSET_FROM_BAR0                            (B1_PHY_DFS_RISC_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_10206_BASE_ADDRESS */
#define B1_DFS_RADAR_DETECTOR_EVENTS_RAM_OFFSET_FROM_BAR0                         (B1_PHY_DFS_RX_TD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_10207_BASE_ADDRESS */
#define B1_DFS_GEN_RISC_GEN_RISC_SRAM_OFFSET_FROM_BAR0                            (B1_PHY_DFS_RISC_OFFSET_FROM_BAR0 + 0x0000C000) /* MEMORY_MAP_UNIT_10206_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_DELAY_FIFO_OFFSET_FROM_BAR0                         (B1_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0 + 0x00001000) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_AGC_TABLE_RAM_OFFSET_FROM_BAR0                      (B1_PHY_DFS_AGC_ANT_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10215_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_IQ_FIREQ_LUT_OFFSET_FROM_BAR0                       (B1_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0 + 0x00002800) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B1_DFS_HYP_TD_ANTENNA_BAND_SEL_COEF_OFFSET_FROM_BAR0                      (B1_PHY_DFS_RXTD_ANT_OFFSET_FROM_BAR0 + 0x00002000) /* MEMORY_MAP_UNIT_10214_BASE_ADDRESS */
#define B2_UP_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_OFFSET_FROM_BAR0 (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00006000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MCS_SNR_MCS_SNR_RAM_OFFSET_FROM_BAR0                                (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002400) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_OFFSET_FROM_BAR0 (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_OFFSET_FROM_BAR0 (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005080) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_OFFSET_FROM_BAR0 (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005100) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_OFFSET_FROM_BAR0 (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00005180) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_HDR_MU_HDR_MEMORY_OFFSET_FROM_BAR0                               (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00002C00) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B2_UP_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_OFFSET_FROM_BAR0              (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_OFFSET_FROM_BAR0              (B2_UP_PHY_RX_FD_DEBUG_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_20213_BASE_ADDRESS */
#define B2_UP_SU_HDR_SU_HDR_MEMORY_OFFSET_FROM_BAR0                               (B2_UP_PHY_RX_FD_OFFSET_FROM_BAR0 + 0x00003000) /* MEMORY_MAP_UNIT_20204_BASE_ADDRESS */
#define B0_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_OFFSET_FROM_BAR0                       (B0_PRE_AGG_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_21_BASE_ADDRESS */
#define B1_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_OFFSET_FROM_BAR0                       (B1_PRE_AGG_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_10021_BASE_ADDRESS */
#define B2_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_OFFSET_FROM_BAR0                       (B2_PRE_AGG_OFFSET_FROM_BAR0 + 0x00004000) /* MEMORY_MAP_UNIT_20021_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT0_OFFSET_FROM_BAR0 + 0x00005C00) /* MEMORY_MAP_UNIT_20230_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT1_OFFSET_FROM_BAR0 + 0x00005C00) /* MEMORY_MAP_UNIT_20231_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT2_OFFSET_FROM_BAR0 + 0x00005C00) /* MEMORY_MAP_UNIT_20232_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00005000) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00005400) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00005800) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO_1_OFFSET_FROM_BAR0                         (B2_PHY_TXTD_ANT3_OFFSET_FROM_BAR0 + 0x00005C00) /* MEMORY_MAP_UNIT_20233_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_6_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00180000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_7_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x001C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_8_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00200000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_9_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00240000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_10_IRAM_HIGH_OFFSET_FROM_BAR0                             (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00280000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_11_IRAM_HIGH_OFFSET_FROM_BAR0                             (CPU_IRAM_OFFSET_FROM_BAR0 + 0x002C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_12_IRAM_HIGH_OFFSET_FROM_BAR0                             (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00300000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_13_IRAM_HIGH_OFFSET_FROM_BAR0                             (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00340000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_0_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00000000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_1_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00040000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_2_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00080000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_3_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x000C0000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_4_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00100000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */
#define IRAM_OCP64_IRAM_5_IRAM_HIGH_OFFSET_FROM_BAR0                              (CPU_IRAM_OFFSET_FROM_BAR0 + 0x00140000) /* MEMORY_MAP_UNIT_1000_BASE_ADDRESS */

 /* Rams Sizes */
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_SIZE                               (0x00001000)
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_SIZE                               (0x00001000)
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_SIZE                               (0x00001000)
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_SIZE                               (0x00001000)
#define B01_MAC_TX_CIRC_RAM_MEM2_0_SIZE                                           (0x00000900)
#define B01_MAC_TX_CIRC_RAM_MEM2_1_SIZE                                           (0x00000900)
#define B01_MAC_TX_CIRC_RAM_MEM2_2_SIZE                                           (0x00000900)
#define B01_MAC_TX_CIRC_RAM_MEM2_3_SIZE                                           (0x00000900)
#define B01_MAC_TX_CIRC_RAM_MEM1_0_SIZE                                           (0x00005000)
#define B01_MAC_TX_CIRC_RAM_MEM1_1_SIZE                                           (0x00005000)
#define B01_MAC_TX_CIRC_RAM_MEM1_2_SIZE                                           (0x00005000)
#define B01_MAC_TX_CIRC_RAM_MEM1_3_SIZE                                           (0x00005000)
#define B01_MAC_TX_CIRC_RAM_MEM0_0_SIZE                                           (0x00005600)
#define B01_MAC_TX_CIRC_RAM_MEM0_1_SIZE                                           (0x00005600)
#define B01_MAC_TX_CIRC_RAM_MEM0_2_SIZE                                           (0x00005600)
#define B01_MAC_TX_CIRC_RAM_MEM0_3_SIZE                                           (0x00005600)
#define B01_MAC_RX_CIRC_RAM_MEM2_0_SIZE                                           (0x00000800)
#define B01_MAC_RX_CIRC_RAM_MEM2_1_SIZE                                           (0x00000800)
#define B01_MAC_RX_CIRC_RAM_MEM2_2_SIZE                                           (0x00000800)
#define B01_MAC_RX_CIRC_RAM_MEM2_3_SIZE                                           (0x00000800)
#define B01_MAC_RX_CIRC_RAM_MEM1_0_SIZE                                           (0x00004800)
#define B01_MAC_RX_CIRC_RAM_MEM1_1_SIZE                                           (0x00004800)
#define B01_MAC_RX_CIRC_RAM_MEM1_2_SIZE                                           (0x00004800)
#define B01_MAC_RX_CIRC_RAM_MEM1_3_SIZE                                           (0x00004800)
#define B01_MAC_RX_CIRC_RAM_MEM0_0_SIZE                                           (0x00004C00)
#define B01_MAC_RX_CIRC_RAM_MEM0_1_SIZE                                           (0x00004C00)
#define B01_MAC_RX_CIRC_RAM_MEM0_2_SIZE                                           (0x00004C00)
#define B01_MAC_RX_CIRC_RAM_MEM0_3_SIZE                                           (0x00004C00)
#define B2_MAC_TX_CIRC_RAM_MEM2_0_SIZE                                            (0x00000900)
#define B2_MAC_TX_CIRC_RAM_MEM2_1_SIZE                                            (0x00000900)
#define B2_MAC_TX_CIRC_RAM_MEM2_2_SIZE                                            (0x00000900)
#define B2_MAC_TX_CIRC_RAM_MEM2_3_SIZE                                            (0x00000900)
#define B2_MAC_TX_CIRC_RAM_MEM1_0_SIZE                                            (0x00005000)
#define B2_MAC_TX_CIRC_RAM_MEM1_1_SIZE                                            (0x00005000)
#define B2_MAC_TX_CIRC_RAM_MEM1_2_SIZE                                            (0x00005000)
#define B2_MAC_TX_CIRC_RAM_MEM1_3_SIZE                                            (0x00005000)
#define B2_MAC_TX_CIRC_RAM_MEM0_0_SIZE                                            (0x00005600)
#define B2_MAC_TX_CIRC_RAM_MEM0_1_SIZE                                            (0x00005600)
#define B2_MAC_TX_CIRC_RAM_MEM0_2_SIZE                                            (0x00005600)
#define B2_MAC_TX_CIRC_RAM_MEM0_3_SIZE                                            (0x00005600)
#define B2_MAC_RX_CIRC_RAM_MEM2_0_SIZE                                            (0x00000800)
#define B2_MAC_RX_CIRC_RAM_MEM2_1_SIZE                                            (0x00000800)
#define B2_MAC_RX_CIRC_RAM_MEM2_2_SIZE                                            (0x00000800)
#define B2_MAC_RX_CIRC_RAM_MEM2_3_SIZE                                            (0x00000800)
#define B2_MAC_RX_CIRC_RAM_MEM1_0_SIZE                                            (0x00004800)
#define B2_MAC_RX_CIRC_RAM_MEM1_1_SIZE                                            (0x00004800)
#define B2_MAC_RX_CIRC_RAM_MEM1_2_SIZE                                            (0x00004800)
#define B2_MAC_RX_CIRC_RAM_MEM1_3_SIZE                                            (0x00004800)
#define B2_MAC_RX_CIRC_RAM_MEM0_0_SIZE                                            (0x00004C00)
#define B2_MAC_RX_CIRC_RAM_MEM0_1_SIZE                                            (0x00004C00)
#define B2_MAC_RX_CIRC_RAM_MEM0_2_SIZE                                            (0x00004C00)
#define B2_MAC_RX_CIRC_RAM_MEM0_3_SIZE                                            (0x00004C00)
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM0_SIZE                                     (0x00002D00)
#define HOST_IF_ACC_HOST_IF_ACC_QOS_RAM1_SIZE                                     (0x00002D00)
#define B01_MD_MEM_TOP_MD_MEM0_18K_0_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM0_18K_1_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM0_18K_2_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM0_18K_3_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM1_18K_0_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM1_18K_1_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM1_18K_2_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM1_18K_3_SIZE                                         (0x00001200)
#define B01_MD_MEM_TOP_MD_MEM2_1K_0_SIZE                                          (0x00000100)
#define B01_MD_MEM_TOP_MD_MEM2_1K_1_SIZE                                          (0x00000100)
#define B01_MD_MEM_TOP_MD_MEM2_1K_2_SIZE                                          (0x00000100)
#define B01_MD_MEM_TOP_MD_MEM2_1K_3_SIZE                                          (0x00000100)
#define B2_MD_MEM_TOP_MD_MEM0_18K_0_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM0_18K_1_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM0_18K_2_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM0_18K_3_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM1_18K_0_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM1_18K_1_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM1_18K_2_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM1_18K_3_SIZE                                          (0x00001200)
#define B2_MD_MEM_TOP_MD_MEM2_1K_0_SIZE                                           (0x00000100)
#define B2_MD_MEM_TOP_MD_MEM2_1K_1_SIZE                                           (0x00000100)
#define B2_MD_MEM_TOP_MD_MEM2_1K_2_SIZE                                           (0x00000100)
#define B2_MD_MEM_TOP_MD_MEM2_1K_3_SIZE                                           (0x00000100)
#define B01_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM_SIZE                                 (0x000001D0)
#define B01_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM_SIZE                                 (0x000001D0)
#define B2_Q_MANAGER_TX_MPDU_DESC0_DLMS_RAM_SIZE                                  (0x000001D0)
#define B2_Q_MANAGER_TX_MPDU_DESC1_DLMS_RAM_SIZE                                  (0x000001D0)
#define B01_RX_PP_RX_PP_COOR_STATISTICS_RAM_SIZE                                  (0x00000800)
#define B2_RX_PP_RX_PP_COOR_STATISTICS_RAM_SIZE                                   (0x00000800)
#define B0_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_SIZE                                   (0x00000800)
#define B0_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_SIZE                                 (0x00000200)
#define B1_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_SIZE                                   (0x00000400)
#define B1_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_SIZE                                 (0x00000200)
#define B0_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_SIZE                                (0x00000160)
#define B1_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_SIZE                                (0x00000140)
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_SIZE                                 (0x00000200)
#define B0_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_SIZE                                 (0x00000270)
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_SIZE                                 (0x00000270)
#define B0_DUR_AUTO_RATE_DUR_MEM_SIZE                                             (0x00000140)
#define B1_DUR_AUTO_RATE_DUR_MEM_SIZE                                             (0x00000140)
#define B1_MAC_ADDR2INDEX_MEM_SIZE                                                (0x00000300)
#define HOST_IF_ACC_FW_CODE_RAM_SIZE                                              (0x00000400)
#define B0_RXC_RXC_FIELDS_CATCHER_MEM_SIZE                                        (0x00000480)
#define B01_RX_PP_STA_BITMAP_RAM_SIZE                                             (0x000007BC)
#define B2_RX_PP_STA_BITMAP_RAM_SIZE                                              (0x000007BC)
#define MAC_DESCRIPTOR_RAM_RX_MEM0_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM1_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM2_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM3_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM4_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM5_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM6_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RX_MEM7_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM0_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM1_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM2_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM3_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM4_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM5_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM6_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_TX_MEM7_SIZE                                           (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM0_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM1_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM2_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM3_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM4_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM5_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM6_SIZE                                         (0x00020000)
#define MAC_DESCRIPTOR_RAM_RXTX_MEM7_SIZE                                         (0x00020000)
#define MAC_PLAN_RAM_MEM0_SIZE                                                    (0x00020000)
#define MAC_PLAN_RAM_MEM1_SIZE                                                    (0x00020000)
#define MAC_PLAN_RAM_MEM2_SIZE                                                    (0x00020000)
#define MAC_PLAN_RAM_MEM3_SIZE                                                    (0x00020000)
#define MAC_SHARED_RAM_MEM0_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM1_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM2_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM3_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM4_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM5_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM6_SIZE                                                  (0x0001A000)
#define MAC_SHARED_RAM_MEM7_SIZE                                                  (0x0001A000)
#define B0_TX_SELECTOR_VAP_TID_RAM_SIZE                                           (0x00000054)
#define B1_TX_SELECTOR_VAP_TID_RAM_SIZE                                           (0x00000054)
#define B0_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_SIZE                                (0x00000080)
#define B1_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_SIZE                                (0x00000080)
#define B0_OTFA_TOP_FRAG_DB_MEM_I_SIZE                                            (0x00000080)
#define B1_OTFA_TOP_FRAG_DB_MEM_I_SIZE                                            (0x00000080)
#define B0_TX_SELECTOR_STA_VAP_RAM_SIZE                                           (0x000000C0)
#define B0_MLD_DB_TOP_MLD_DB_MEMORY_SIZE                                          (0x000006C0)
#define B1_MLD_DB_TOP_MLD_DB_MEMORY_SIZE                                          (0x000006C0)
#define B01_RX_PP_RX_PP_STATISTICS_RAM_SIZE                                       (0x00001680)
#define B2_RX_PP_RX_PP_STATISTICS_RAM_SIZE                                        (0x00001680)
#define EXT_RX_PP_COUNTERS_EXT_RX_PP_STATISTICS_RAM_SIZE                          (0x00002000)
#define B1_SEC_ENG_TOP_USER_ATTR_MEM_SIZE                                         (0x000002B5)
#define B1_SEC_ENG_TOP_KEY_MEM_SIZE                                               (0x00000480)
#define B0_AUTO_RESP_TID_BITMAP_MEM_0_SIZE                                        (0x00000400)
#define B0_AUTO_RESP_TID_BITMAP_MEM_1_SIZE                                        (0x00000400)
#define B0_AUTO_RESP_TID_BITMAP_MEM_2_SIZE                                        (0x00000400)
#define B0_AUTO_RESP_TID_BITMAP_MEM_3_SIZE                                        (0x00000400)
#define B1_AUTO_RESP_TID_BITMAP_MEM_0_SIZE                                        (0x00000200)
#define B1_AUTO_RESP_TID_BITMAP_MEM_1_SIZE                                        (0x00000200)
#define B1_AUTO_RESP_TID_BITMAP_MEM_2_SIZE                                        (0x00000200)
#define B1_AUTO_RESP_TID_BITMAP_MEM_3_SIZE                                        (0x00000200)
#define B2_AUTO_RESP_TID_BITMAP_MEM_0_SIZE                                        (0x00000400)
#define B2_AUTO_RESP_TID_BITMAP_MEM_1_SIZE                                        (0x00000400)
#define B2_AUTO_RESP_TID_BITMAP_MEM_2_SIZE                                        (0x00000400)
#define B2_AUTO_RESP_TID_BITMAP_MEM_3_SIZE                                        (0x00000400)
#define B0_AUTO_RESP_USER_DB_MEM_SIZE                                             (0x000007E0)
#define B1_AUTO_RESP_USER_DB_MEM_SIZE                                             (0x000003F0)
#define B2_AUTO_RESP_USER_DB_MEM_SIZE                                             (0x000007E0)
#define B0_MAC_BFM_BEST_RU_DB_SIZE                                                (0x000000FC)
#define B0_BAA_TOP_RA_USER_MEM_SIZE                                               (0x000001C2)
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_SIZE                                (0x00004000)
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_SIZE                                (0x00004000)
#define B0_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_SIZE                                  (0x0000032A)
#define B0_OTFA_TOP_LOCAL_DB_MEM_I_SIZE                                           (0x00000780)
#define LOGGER_WRAPPER_UDP_SIZE                                                   (0x000000C0)
#define B0_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_SIZE                                (0x00000580)
#define B0_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_SIZE                                (0x00000270)
#define B1_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_SIZE                                (0x00000580)
#define B1_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_SIZE                                (0x00000270)
#define B01_Q_MANAGER_PD_DLMS_RAM_SIZE                                            (0x000066D0)
#define B2_Q_MANAGER_PD_DLMS_RAM_SIZE                                             (0x000066D0)
#define B01_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM_SIZE                           (0x00002100)
#define B2_BAA_STATISTICS_CNT_MEM_COMMON_BAND_RAM_SIZE                            (0x00002100)
#define B01_RETRY_STATE_DB_DUAL_BAND_RAM_SIZE                                     (0x00002500)
#define B2_RETRY_STATE_DB_DUAL_BAND_RAM_SIZE                                      (0x00002500)
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_SIZE                                 (0x00001400)
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_SIZE                                 (0x00001400)
#define B0_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_SIZE                                  (0x000002E0)
#define B1_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_SIZE                                  (0x000002E0)
#define B0_TF_DECODER_TF_DEC_TX_POWER_MEM_SIZE                                    (0x00000080)
#define B1_TF_DECODER_TF_DEC_TX_POWER_MEM_SIZE                                    (0x00000080)
#define B1_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_SIZE                                 (0x00000100)
#define B1_TX_SELECTOR_STA_TID_RAM0_SIZE                                          (0x00000308)
#define B1_TX_SELECTOR_STA_TID_RAM1_SIZE                                          (0x00000308)
#define B0_SEC_ENG_TOP_USER_ATTR_MEM_SIZE                                         (0x0000056A)
#define B0_SEC_ENG_TOP_KEY_MEM_SIZE                                               (0x00000900)
#define B0_RX_CLASSIFIER_RX_CLASSIFIER_MEM_SIZE                                   (0x00000120)
#define B1_RX_CLASSIFIER_RX_CLASSIFIER_MEM_SIZE                                   (0x00000120)
#define HOST_IF_ACC_DIST_RAM_SIZE                                                 (0x00000258)
#define B01_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM_SIZE                                 (0x00000140)
#define B01_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM_SIZE                                 (0x00000140)
#define B2_Q_MANAGER_RX_MPDU_DESC0_DLMS_RAM_SIZE                                  (0x00000140)
#define B2_Q_MANAGER_RX_MPDU_DESC1_DLMS_RAM_SIZE                                  (0x00000140)
#define B01_Q_MANAGER_DMA_DLMS_RAM_SIZE                                           (0x00000148)
#define B2_Q_MANAGER_DMA_DLMS_RAM_SIZE                                            (0x00000148)
#define HOST_IF_AQM_CFG_MEM_SIZE                                                  (0x00005A00)
#define HOST_IF_AQM_STAT_BURST_MEM_SIZE                                           (0x00001200)
#define B0_PRE_AGG_PRE_AGG_MEM_SIZE                                               (0x00000240)
#define B1_PRE_AGG_PRE_AGG_MEM_SIZE                                               (0x00000240)
#define B1_MAC_BFM_MBFM_TASK_SW_DB_L_SIZE                                         (0x00000190)
#define B1_SEC_ENG_TOP_MIC_MEM_SIZE                                               (0x00000240)
#define B0_MAC_BFM_MBFM_TASK_SW_DB_L_SIZE                                         (0x000002D0)
#define B0_SEC_ENG_TOP_MIC_MEM_SIZE                                               (0x00000480)
#define IRAM_OCP64_IRAM_6_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_7_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_8_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_9_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_10_IRAM_LOW_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_11_IRAM_LOW_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_12_IRAM_LOW_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_13_IRAM_LOW_SIZE                                          (0x00020000)
#define HOST_IF_CONTEXT_SAVE_MEMORY_SIZE                                          (0x00001400)
#define HOST_IF_U_ICCM_RAM0_SIZE                                                  (0x00004000)
#define HOST_IF_U_DCCM_RAM_BANK0_SIZE                                             (0x00001000)
#define HOST_IF_U_DCCM_RAM_BANK1_SIZE                                             (0x00001000)
#define B2_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM_SIZE                           (0x00003288)
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_SPRAM_SIZE                               (0x00001000)
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_SPRAM_SIZE                               (0x00001000)
#define B2_TWT_TX_SEL_TWT_SP_GROUP_STA_RAM_SIZE                                   (0x00000800)
#define B2_TWT_TX_SEL_TWT_SP_GROUP_COUNT_RAM_SIZE                                 (0x00000200)
#define B2_TX_SELECTOR_NEXT_CHOSEN_VAP_AC_MEM_SIZE                                (0x00000160)
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_STA_RAM_SIZE                                 (0x00000200)
#define B2_TX_SELECTOR_TX_SEL_VHT_MU_GRP_RAM_SIZE                                 (0x00000270)
#define B2_DUR_AUTO_RATE_DUR_MEM_SIZE                                             (0x00000140)
#define B2_TX_SELECTOR_STA_TID_RAM0_SIZE                                          (0x00000610)
#define B2_TX_SELECTOR_STA_TID_RAM1_SIZE                                          (0x00000610)
#define B2_RXF_RXF_MEM_SIZE                                                       (0x00000240)
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_SIZE                                (0x00010000)
#define B2_PRE_AGG_TOP_PARAMS_CALC_MEM_SIZE                                       (0x00000800)
#define B2_MAC_ADDR2INDEX_MEM_SIZE                                                (0x00000600)
#define B2_RXC_RXC_FIELDS_CATCHER_MEM_SIZE                                        (0x00000480)
#define B2_TX_SELECTOR_VAP_TID_RAM_SIZE                                           (0x00000054)
#define B2_RX_COORDINATOR_FRAME_CLASS_VIO_MEM_SIZE                                (0x00000080)
#define B2_OTFA_TOP_FRAG_DB_MEM_I_SIZE                                            (0x00000080)
#define B2_TX_SELECTOR_STA_VAP_RAM_SIZE                                           (0x000000C0)
#define B2_MLD_DB_TOP_MLD_DB_MEMORY_SIZE                                          (0x00000CC0)
#define B2_MAC_BFM_BEST_RU_DB_SIZE                                                (0x000000FC)
#define B2_BAA_TOP_RA_USER_MEM_SIZE                                               (0x000001C2)
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_RX_IRAM_SIZE                                (0x00004000)
#define B2_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_SIZE                                  (0x0000032A)
#define B2_OTFA_TOP_LOCAL_DB_MEM_I_SIZE                                           (0x00000780)
#define B2_TXH_MAP_HANDLER_TXH_REGULAR_AC_MEM_SIZE                                (0x00000580)
#define B2_TXH_MAP_HANDLER_TXH_SPECIAL_AC_MEM_SIZE                                (0x00000270)
#define B2_MAC_GEN_RISC_RAMS_GEN_RISC_TX_STD_SIZE                                 (0x00001400)
#define B2_TF_GENERATOR_TF_GEN_RU_INDEX_MEM_SIZE                                  (0x000002E0)
#define B2_TF_DECODER_TF_DEC_TX_POWER_MEM_SIZE                                    (0x00000080)
#define B2_SEC_ENG_TOP_USER_ATTR_MEM_SIZE                                         (0x0000056A)
#define B2_SEC_ENG_TOP_KEY_MEM_SIZE                                               (0x00000900)
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_MEM_SIZE                                   (0x00000120)
#define B2_PRE_AGG_PRE_AGG_MEM_SIZE                                               (0x00000240)
#define B2_MAC_BFM_MBFM_TASK_SW_DB_L_SIZE                                         (0x000002D0)
#define B2_SEC_ENG_TOP_MIC_MEM_SIZE                                               (0x00000480)
#define B0_TX_SELECTOR_STA_TID_RAM0_SIZE                                          (0x00000610)
#define B0_TX_SELECTOR_STA_TID_RAM1_SIZE                                          (0x00000610)
#define B0_RXF_RXF_MEM_SIZE                                                       (0x00000240)
#define B1_RXF_RXF_MEM_SIZE                                                       (0x00000240)
#define B1_RXC_RXC_FIELDS_CATCHER_MEM_SIZE                                        (0x00000240)
#define B01_DMAC_WRAPPER_U_LL_MEM_SIZE                                            (0x00001800)
#define B2_DMAC_WRAPPER_U_LL_MEM_SIZE                                             (0x00001800)
#define B01_BESTRU_DB_RAM_WRAP_SHARED_BESTRU_DB_MEM_SIZE                          (0x00003288)
#define B0_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_SIZE                                (0x00010000)
#define B1_MAC_GEN_RISC_RAMS_GEN_RISC_TX_IRAM_SIZE                                (0x00010000)
#define B01_RETRY_POINTER_DB_DUAL_BAND_RAM_0_SIZE                                 (0x0000B900)
#define B2_RETRY_POINTER_DB_DUAL_BAND_RAM_0_SIZE                                  (0x0000B900)
#define B01_TRAFFIC_INDICATOR_MEM_SIZE                                            (0x00000320)
#define B2_TRAFFIC_INDICATOR_MEM_SIZE                                             (0x00000320)
#define B1_TX_PD_ACC_TXPD_LOCAL_CONTEXT_MEM_SIZE                                  (0x000001C2)
#define B1_MAC_BFM_BEST_RU_DB_SIZE                                                (0x0000008C)
#define B1_BAA_TOP_RA_USER_MEM_SIZE                                               (0x000000FA)
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM0_SIZE                              (0x00000E80)
#define HOST_IF_ACC_HOST_IF_ACC_STATISTICS_RAM1_SIZE                              (0x00000E80)
#define B01_RX_PP_PN_SN_RAM_SIZE                                                  (0x000060F4)
#define B2_RX_PP_PN_SN_RAM_SIZE                                                   (0x000060F4)
#define PROT_DB_SMC_LOCK_MEM_SIZE                                                 (0x00000180)
#define B1_OTFA_TOP_LOCAL_DB_MEM_I_SIZE                                           (0x000003C0)
#define B0_PRE_AGG_TOP_PARAMS_CALC_MEM_SIZE                                       (0x00000800)
#define B1_PRE_AGG_TOP_PARAMS_CALC_MEM_SIZE                                       (0x00000800)
#define B01_BSRC_AGER_MEM_COMMON_BAND_RAM_SIZE                                    (0x00001000)
#define B2_BSRC_AGER_MEM_COMMON_BAND_RAM_SIZE                                     (0x00001000)
#define B01_BSRC_CNT_MEM_COMMON_BAND_RAM_SIZE                                     (0x00001160)
#define B2_BSRC_CNT_MEM_COMMON_BAND_RAM_SIZE                                      (0x00001160)
#define B0_MAC_ADDR2INDEX_MEM_SIZE                                                (0x00000600)
#define B1_TX_SELECTOR_STA_VAP_RAM_SIZE                                           (0x00000060)
#define IRAM_OCP64_IRAM_0_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_1_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_2_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_3_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_4_IRAM_LOW_SIZE                                           (0x00020000)
#define IRAM_OCP64_IRAM_5_IRAM_LOW_SIZE                                           (0x00020000)
#define B0_RX_RCR_RCR_RAM_CDB0_SIZE                                               (0x000001A0)
#define B1_RX_RCR_RCR_RAM_CDB1_SIZE                                               (0x000001A0)
#define B0_RX_SIGB_SIGB_RAM_CDB0_SIZE                                             (0x00000280)
#define B1_RX_SIGB_SIGB_RAM_CDB1_SIZE                                             (0x00000280)
#define B2_RX_SIGB_SIGB_RAM_CDB0_SIZE                                             (0x00000280)
#define B2_RX_RCR_RCR_RAM_CDB0_SIZE                                               (0x000001A0)
#define B0_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_SIZE                        (0x00002000)
#define B0_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_SIZE                        (0x0000006C)
#define B0_HYP_RXFD_CSM_RCR_RAM_SIZE                                              (0x0000006C)
#define B0_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_SIZE                                  (0x000000D8)
#define B0_FILTER_SELECT_FILTER_RAM_SIZE                                          (0x00000AB0)
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_LO_SIZE                           (0x00010000)
#define B0_MU_SU_RAMS_MX_SU_40_SU_PHASES_MEMORY_HI_SIZE                           (0x00010000)
#define B0_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_SIZE                                  (0x00000340)
#define B0_HYP_RXFD_BIN_CHOOSER_NE_RAM_SIZE                                       (0x00001740)
#define B0_LINEAR_SLICER_EVM_MU_SIZE                                              (0x000002D0)
#define B0_CH_RAM_0_DATA_CH_RAM_LO_R_SIZE                                         (0x00000D00)
#define B0_CH_RAM_0_DATA_CH_RAM_LO_L_SIZE                                         (0x00000D00)
#define B0_CH_RAM_1_DATA_CH_RAM_LO_R_SIZE                                         (0x00000D00)
#define B0_CH_RAM_1_DATA_CH_RAM_LO_L_SIZE                                         (0x00000D00)
#define B0_CH_RAM_2_DATA_CH_RAM_LO_R_SIZE                                         (0x00000D00)
#define B0_CH_RAM_2_DATA_CH_RAM_LO_L_SIZE                                         (0x00000D00)
#define B0_CH_RAM_3_DATA_CH_RAM_LO_R_SIZE                                         (0x00000D00)
#define B0_CH_RAM_3_DATA_CH_RAM_LO_L_SIZE                                         (0x00000D00)
#define B0_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_SIZE             (0x00000980)
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_SIZE                (0x0000004C)
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_SIZE                (0x0000004C)
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_SIZE                (0x0000004C)
#define B0_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_SIZE                (0x0000004C)
#define B0_MU_HDR_MU_HDR_MEMORY_SIZE                                              (0x00000080)
#define B0_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_SIZE                             (0x00005000)
#define B0_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_SIZE                             (0x00005000)
#define B0_SU_HDR_SU_HDR_MEMORY_SIZE                                              (0x00000800)
#define B0_HYP_TXFD_TXTD_SCALE_RAM_SIZE                                           (0x00000180)
#define B1_LONG_PREAMBLE_LONG_PREAMBLE_RAM_SIZE                                   (0x00000080)
#define B1_HYP_TXFD_TXTD_SCALE_RAM_SIZE                                           (0x00000180)
#define B0_HYP_TX_EXT_CONTROL_SIZE                                                (0x00000120)
#define B1_HYP_TX_EXT_CONTROL_SIZE                                                (0x00000120)
#define B0_LONG_PREAMBLE_LONG_PREAMBLE_RAM_SIZE                                   (0x00000080)
#define B2_LONG_PREAMBLE_LONG_PREAMBLE_RAM_SIZE                                   (0x00000080)
#define B2_HYP_TXFD_TXTD_SCALE_RAM_SIZE                                           (0x00000180)
#define B2_UP_LONG_PREAMBLE_LONG_PREAMBLE_RAM_SIZE                                (0x00000080)
#define B2_UP_HYP_TXFD_TXTD_SCALE_RAM_SIZE                                        (0x00000180)
#define B2_HYP_TX_EXT_CONTROL_SIZE                                                (0x00000220)
#define B0_DC_ESTIMATION_CFO_THRESHOLD_RAM_SIZE                                   (0x00000228)
#define B1_DC_ESTIMATION_CFO_THRESHOLD_RAM_SIZE                                   (0x00000228)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_0_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_1_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_2_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_3_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_4_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_5_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_6_SIZE                                  (0x00010000)
#define B01_HYP_RXTD_SHARED_TEST_BUS_FIFO_7_SIZE                                  (0x00010000)
#define B0_HYP_TD_ANTENNA_0_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B0_HYP_TD_ANTENNA_1_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B0_HYP_TD_ANTENNA_2_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B0_HYP_TD_ANTENNA_3_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B0_HYP_TD_ANTENNA_0_IQ_RAM_SIZE                                           (0x000005A0)
#define B0_HYP_TD_ANTENNA_1_IQ_RAM_SIZE                                           (0x000005A0)
#define B0_HYP_TD_ANTENNA_2_IQ_RAM_SIZE                                           (0x000005A0)
#define B0_HYP_TD_ANTENNA_3_IQ_RAM_SIZE                                           (0x000005A0)
#define B0_HYP_TD_ANTENNA_0_CAL_RAM_SIZE                                          (0x00000800)
#define B0_HYP_TD_ANTENNA_1_CAL_RAM_SIZE                                          (0x00000800)
#define B0_HYP_TD_ANTENNA_2_CAL_RAM_SIZE                                          (0x00000800)
#define B0_HYP_TD_ANTENNA_3_CAL_RAM_SIZE                                          (0x00000800)
#define B0_HYP_TD_ANTENNA_0_DELAY_FIFO_SIZE                                       (0x00000750)
#define B0_HYP_TD_ANTENNA_1_DELAY_FIFO_SIZE                                       (0x00000750)
#define B0_HYP_TD_ANTENNA_2_DELAY_FIFO_SIZE                                       (0x00000750)
#define B0_HYP_TD_ANTENNA_3_DELAY_FIFO_SIZE                                       (0x00000750)
#define B0_GEN_RISC_GEN_RISC_IRAM_SIZE                                            (0x0000A000)
#define B1_GEN_RISC_GEN_RISC_IRAM_SIZE                                            (0x0000A000)
#define B0_HYP_TD_ANTENNA_0_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_1_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_2_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_3_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_0_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_1_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_2_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_3_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_0_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_1_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_2_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_3_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B1_RADAR_DETECTOR_EVENTS_RAM_SIZE                                         (0x00000400)
#define B0_HYP_TD_ANTENNA_0_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B0_HYP_TD_ANTENNA_1_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B0_HYP_TD_ANTENNA_2_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B0_HYP_TD_ANTENNA_3_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B0_HYP_TD_ANTENNA_0_TPC_RAM_SIZE                                          (0x000007E8)
#define B0_HYP_TD_ANTENNA_1_TPC_RAM_SIZE                                          (0x000007E8)
#define B0_HYP_TD_ANTENNA_2_TPC_RAM_SIZE                                          (0x000007E8)
#define B0_HYP_TD_ANTENNA_3_TPC_RAM_SIZE                                          (0x000007E8)
#define B0_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B0_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B0_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B0_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B1_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B1_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B1_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B1_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B1_HYP_TD_ANTENNA_0_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B1_HYP_TD_ANTENNA_1_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B1_HYP_TD_ANTENNA_2_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B1_HYP_TD_ANTENNA_3_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B1_HYP_TD_ANTENNA_0_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B1_HYP_TD_ANTENNA_1_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B1_HYP_TD_ANTENNA_2_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B1_HYP_TD_ANTENNA_3_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B1_HYP_TD_ANTENNA_0_IQ_RAM_SIZE                                           (0x000005A0)
#define B1_HYP_TD_ANTENNA_1_IQ_RAM_SIZE                                           (0x000005A0)
#define B1_HYP_TD_ANTENNA_2_IQ_RAM_SIZE                                           (0x000005A0)
#define B1_HYP_TD_ANTENNA_3_IQ_RAM_SIZE                                           (0x000005A0)
#define B1_HYP_TD_ANTENNA_0_CAL_RAM_SIZE                                          (0x00000800)
#define B1_HYP_TD_ANTENNA_1_CAL_RAM_SIZE                                          (0x00000800)
#define B1_HYP_TD_ANTENNA_2_CAL_RAM_SIZE                                          (0x00000800)
#define B1_HYP_TD_ANTENNA_3_CAL_RAM_SIZE                                          (0x00000800)
#define B1_HYP_TD_ANTENNA_0_DELAY_FIFO_SIZE                                       (0x00000750)
#define B1_HYP_TD_ANTENNA_1_DELAY_FIFO_SIZE                                       (0x00000750)
#define B1_HYP_TD_ANTENNA_2_DELAY_FIFO_SIZE                                       (0x00000750)
#define B1_HYP_TD_ANTENNA_3_DELAY_FIFO_SIZE                                       (0x00000750)
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_0_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_0_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_1_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_1_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_2_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_2_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_3_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_3_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B1_HYP_TD_ANTENNA_0_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_1_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_2_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_3_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B1_HYP_TD_ANTENNA_0_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B1_HYP_TD_ANTENNA_1_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B1_HYP_TD_ANTENNA_2_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B1_HYP_TD_ANTENNA_3_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B1_HYP_TD_ANTENNA_0_TPC_RAM_SIZE                                          (0x000007E8)
#define B1_HYP_TD_ANTENNA_1_TPC_RAM_SIZE                                          (0x000007E8)
#define B1_HYP_TD_ANTENNA_2_TPC_RAM_SIZE                                          (0x000007E8)
#define B1_HYP_TD_ANTENNA_3_TPC_RAM_SIZE                                          (0x000007E8)
#define B1_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B1_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B1_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B1_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B0_GEN_RISC_GEN_RISC_SRAM_SIZE                                            (0x00004000)
#define B1_GEN_RISC_GEN_RISC_SRAM_SIZE                                            (0x00004000)
#define B0_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B0_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B0_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B0_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B0_HYP_TD_ANTENNA_0_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B0_HYP_TD_ANTENNA_1_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B0_HYP_TD_ANTENNA_2_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B0_HYP_TD_ANTENNA_3_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B2_GEN_RISC_GEN_RISC_SRAM_SIZE                                            (0x00004000)
#define B2_HYP_TD_ANTENNA_0_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B2_HYP_TD_ANTENNA_1_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B2_HYP_TD_ANTENNA_2_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B2_HYP_TD_ANTENNA_3_IQ_FIREQ_LUT_SIZE                                     (0x000000B4)
#define B2_HYP_TD_ANTENNA_0_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B2_HYP_TD_ANTENNA_1_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B2_HYP_TD_ANTENNA_2_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B2_HYP_TD_ANTENNA_3_BAND_SEL_COEF_SIZE                                    (0x00000210)
#define B2_DC_ESTIMATION_CFO_THRESHOLD_RAM_SIZE                                   (0x00000228)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_0_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_1_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_2_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_3_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_4_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_5_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_6_SIZE                                   (0x00010000)
#define B2_HYP_RXTD_SHARED_TEST_BUS_FIFO_7_SIZE                                   (0x00010000)
#define B2_HYP_TD_ANTENNA_0_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B2_HYP_TD_ANTENNA_1_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B2_HYP_TD_ANTENNA_2_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B2_HYP_TD_ANTENNA_3_RIPPLE_RAM_SIZE                                       (0x000001F8)
#define B2_HYP_TD_ANTENNA_0_IQ_RAM_SIZE                                           (0x000005A0)
#define B2_HYP_TD_ANTENNA_1_IQ_RAM_SIZE                                           (0x000005A0)
#define B2_HYP_TD_ANTENNA_2_IQ_RAM_SIZE                                           (0x000005A0)
#define B2_HYP_TD_ANTENNA_3_IQ_RAM_SIZE                                           (0x000005A0)
#define B2_HYP_TD_ANTENNA_0_CAL_RAM_SIZE                                          (0x00000800)
#define B2_HYP_TD_ANTENNA_1_CAL_RAM_SIZE                                          (0x00000800)
#define B2_HYP_TD_ANTENNA_2_CAL_RAM_SIZE                                          (0x00000800)
#define B2_HYP_TD_ANTENNA_3_CAL_RAM_SIZE                                          (0x00000800)
#define B2_HYP_TD_ANTENNA_0_DELAY_FIFO_SIZE                                       (0x00000750)
#define B2_HYP_TD_ANTENNA_1_DELAY_FIFO_SIZE                                       (0x00000750)
#define B2_HYP_TD_ANTENNA_2_DELAY_FIFO_SIZE                                       (0x00000750)
#define B2_HYP_TD_ANTENNA_3_DELAY_FIFO_SIZE                                       (0x00000750)
#define B2_GEN_RISC_GEN_RISC_IRAM_SIZE                                            (0x0000A000)
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_0_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_0_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_0_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_1_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_1_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_2_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_2_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_3_FFT_0_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_3_FFT_1_RAM_SIZE                                        (0x00001E00)
#define B2_HYP_TD_ANTENNA_0_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_1_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_2_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_3_RXTD_FIFO_1_SIZE                                      (0x00001E00)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO_SIZE                                       (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_CFG_SIZE                                      (0x00000140)
#define B2_HYP_TD_ANTENNA_0_TPC_RAM_SIZE                                          (0x000007E8)
#define B2_HYP_TD_ANTENNA_1_TPC_RAM_SIZE                                          (0x000007E8)
#define B2_HYP_TD_ANTENNA_2_TPC_RAM_SIZE                                          (0x000007E8)
#define B2_HYP_TD_ANTENNA_3_TPC_RAM_SIZE                                          (0x000007E8)
#define B2_HYP_TD_ANTENNA_0_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B2_HYP_TD_ANTENNA_1_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B2_HYP_TD_ANTENNA_2_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B2_HYP_TD_ANTENNA_3_AGC_TABLE_RAM_SIZE                                    (0x00001000)
#define B2_UP_HYP_TD_ANTENNA_0_BAND_SEL_COEF_SIZE                                 (0x00000210)
#define B2_UP_HYP_TD_ANTENNA_1_BAND_SEL_COEF_SIZE                                 (0x00000210)
#define B2_UP_HYP_TD_ANTENNA_2_BAND_SEL_COEF_SIZE                                 (0x00000210)
#define B2_UP_HYP_TD_ANTENNA_3_BAND_SEL_COEF_SIZE                                 (0x00000210)
#define B2_UP_HYP_TD_ANTENNA_0_RIPPLE_RAM_SIZE                                    (0x000001F8)
#define B2_UP_HYP_TD_ANTENNA_1_RIPPLE_RAM_SIZE                                    (0x000001F8)
#define B2_UP_HYP_TD_ANTENNA_2_RIPPLE_RAM_SIZE                                    (0x000001F8)
#define B2_UP_HYP_TD_ANTENNA_3_RIPPLE_RAM_SIZE                                    (0x000001F8)
#define B2_UP_HYP_TD_ANTENNA_0_IQ_RAM_SIZE                                        (0x000005A0)
#define B2_UP_HYP_TD_ANTENNA_1_IQ_RAM_SIZE                                        (0x000005A0)
#define B2_UP_HYP_TD_ANTENNA_2_IQ_RAM_SIZE                                        (0x000005A0)
#define B2_UP_HYP_TD_ANTENNA_3_IQ_RAM_SIZE                                        (0x000005A0)
#define B2_UP_HYP_TD_ANTENNA_0_CAL_RAM_SIZE                                       (0x00000800)
#define B2_UP_HYP_TD_ANTENNA_1_CAL_RAM_SIZE                                       (0x00000800)
#define B2_UP_HYP_TD_ANTENNA_2_CAL_RAM_SIZE                                       (0x00000800)
#define B2_UP_HYP_TD_ANTENNA_3_CAL_RAM_SIZE                                       (0x00000800)
#define B2_UP_HYP_TD_ANTENNA_0_DELAY_FIFO_SIZE                                    (0x00000750)
#define B2_UP_HYP_TD_ANTENNA_1_DELAY_FIFO_SIZE                                    (0x00000750)
#define B2_UP_HYP_TD_ANTENNA_2_DELAY_FIFO_SIZE                                    (0x00000750)
#define B2_UP_HYP_TD_ANTENNA_3_DELAY_FIFO_SIZE                                    (0x00000750)
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_0_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_0_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_0_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_0_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_0_FFT_0_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_0_FFT_1_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_1_FFT_0_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_1_FFT_1_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_2_FFT_0_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_2_FFT_1_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_3_FFT_0_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_3_FFT_1_RAM_SIZE                                     (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_0_RXTD_FIFO_1_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_1_RXTD_FIFO_1_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_2_RXTD_FIFO_1_SIZE                                   (0x00001E00)
#define B2_UP_HYP_TD_ANTENNA_3_RXTD_FIFO_1_SIZE                                   (0x00001E00)
#define B1_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_SIZE                        (0x00002000)
#define B1_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_SIZE                        (0x0000006C)
#define B1_HYP_RXFD_CSM_RCR_RAM_SIZE                                              (0x0000006C)
#define B1_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_SIZE                                  (0x000000D8)
#define B1_FILTER_SELECT_FILTER_RAM_SIZE                                          (0x00000AB0)
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_SIZE                               (0x00010000)
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_SIZE                               (0x00010000)
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_SIZE                               (0x00010000)
#define B1_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_SIZE                               (0x00010000)
#define B1_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_SIZE                                  (0x00000340)
#define B1_HYP_RXFD_BIN_CHOOSER_NE_RAM_SIZE                                       (0x00001740)
#define B1_LINEAR_SLICER_EVM_MU_SIZE                                              (0x000002D0)
#define B1_CH_RAM_0_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_0_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_0_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_0_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_1_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_1_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_1_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_1_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_2_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_2_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_2_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_2_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_3_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_3_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B1_CH_RAM_3_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B1_CH_RAM_3_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B1_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_SIZE             (0x00000980)
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_SIZE                (0x0000004C)
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_SIZE                (0x0000004C)
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_SIZE                (0x0000004C)
#define B1_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_SIZE                (0x0000004C)
#define B1_MU_HDR_MU_HDR_MEMORY_SIZE                                              (0x00000080)
#define B1_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_SIZE                             (0x00005000)
#define B1_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_SIZE                             (0x00005000)
#define B1_SU_HDR_SU_HDR_MEMORY_SIZE                                              (0x00000800)
#define B2_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_SIZE                        (0x00002000)
#define B2_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_SIZE                        (0x0000006C)
#define B2_HYP_RXFD_CSM_RCR_RAM_SIZE                                              (0x0000006C)
#define B2_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_SIZE                                  (0x000000D8)
#define B2_FILTER_SELECT_FILTER_RAM_SIZE                                          (0x00000AB0)
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_SIZE                               (0x00010000)
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_SIZE                               (0x00010000)
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_SIZE                               (0x00010000)
#define B2_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_SIZE                               (0x00010000)
#define B2_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_SIZE                                  (0x00000340)
#define B2_HYP_RXFD_BIN_CHOOSER_NE_RAM_SIZE                                       (0x00001740)
#define B2_LINEAR_SLICER_EVM_MU_SIZE                                              (0x000002D0)
#define B2_CH_RAM_0_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_0_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_0_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_0_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_1_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_1_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_1_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_1_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_2_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_2_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_2_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_2_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_3_DATA_CH_RAM_LO_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_3_DATA_CH_RAM_LO_L_SIZE                                         (0x00001A00)
#define B2_CH_RAM_3_DATA_CH_RAM_HI_R_SIZE                                         (0x00001A00)
#define B2_CH_RAM_3_DATA_CH_RAM_HI_L_SIZE                                         (0x00001A00)
#define B2_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_SIZE             (0x00000980)
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_SIZE                (0x0000004C)
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_SIZE                (0x0000004C)
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_SIZE                (0x0000004C)
#define B2_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_SIZE                (0x0000004C)
#define B2_MU_HDR_MU_HDR_MEMORY_SIZE                                              (0x00000080)
#define B2_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_SIZE                             (0x00005000)
#define B2_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_SIZE                             (0x00005000)
#define B2_SU_HDR_SU_HDR_MEMORY_SIZE                                              (0x00000800)
#define B2_UP_SU_PHASES_DATABASE_SU_EHT_HI_PHASES_MEMORY_SIZE                     (0x00002000)
#define B2_UP_SMOOTH_INTERP_CONTROL_PATH_USER_RU_MAP_RAM_SIZE                     (0x0000006C)
#define B2_UP_HYP_RXFD_CSM_RCR_RAM_SIZE                                           (0x0000006C)
#define B2_UP_CH_ENERGY_PER_RU_RSSI_PER_RU_RAM_SIZE                               (0x000000D8)
#define B2_UP_FILTER_SELECT_FILTER_RAM_SIZE                                       (0x00000AB0)
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_0_SIZE                            (0x00010000)
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_LO_1_SIZE                            (0x00010000)
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_0_SIZE                            (0x00010000)
#define B2_UP_MU_SU_RAMS_MX_SU_PHASES_MEMORY_HI_1_SIZE                            (0x00010000)
#define B2_UP_SPHERE_CLOCKS_SPHERE_CLOCKS_RAM0_SIZE                               (0x00000340)
#define B2_UP_HYP_RXFD_BIN_CHOOSER_NE_RAM_SIZE                                    (0x00001740)
#define B2_UP_LINEAR_SLICER_EVM_MU_SIZE                                           (0x000002D0)
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_0_DATA_CH_RAM_LO_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_0_DATA_CH_RAM_HI_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_1_DATA_CH_RAM_LO_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_1_DATA_CH_RAM_HI_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_2_DATA_CH_RAM_LO_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_2_DATA_CH_RAM_HI_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_3_DATA_CH_RAM_LO_L_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_R_SIZE                                      (0x00001A00)
#define B2_UP_CH_RAM_3_DATA_CH_RAM_HI_L_SIZE                                      (0x00001A00)
#define B1_DFS_GEN_RISC_GEN_RISC_IRAM_SIZE                                        (0x00008000)
#define B1_DFS_RADAR_DETECTOR_EVENTS_RAM_SIZE                                     (0x00000400)
#define B1_DFS_GEN_RISC_GEN_RISC_SRAM_SIZE                                        (0x00002000)
#define B1_DFS_HYP_TD_ANTENNA_DELAY_FIFO_SIZE                                     (0x00000750)
#define B1_DFS_HYP_TD_ANTENNA_AGC_TABLE_RAM_SIZE                                  (0x00001000)
#define B1_DFS_HYP_TD_ANTENNA_IQ_FIREQ_LUT_SIZE                                   (0x000000B4)
#define B1_DFS_HYP_TD_ANTENNA_BAND_SEL_COEF_SIZE                                  (0x00000210)
#define B2_UP_MU_EFF_RATE_DATABASE_MU_TRAIN_EFFECTIVE_RATE_DATABASE_SIZE          (0x00000980)
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM0_SIZE             (0x0000004C)
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM1_SIZE             (0x0000004C)
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM2_SIZE             (0x0000004C)
#define B2_UP_HE_STF_NOISE_GAIN_CALC_HE_STF_NOISE_GAIN_CALC_RAM3_SIZE             (0x0000004C)
#define B2_UP_MU_HDR_MU_HDR_MEMORY_SIZE                                           (0x00000080)
#define B2_UP_MU_PHASES_MEMORY_0_MU_PHASES_MEMORY_0_SIZE                          (0x00005000)
#define B2_UP_MU_PHASES_MEMORY_1_MU_PHASES_MEMORY_1_SIZE                          (0x00005000)
#define B2_UP_SU_HDR_SU_HDR_MEMORY_SIZE                                           (0x00000800)
#define B0_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_SIZE                                   (0x00002500)
#define B1_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_SIZE                                   (0x00002500)
#define B2_PRE_AGG_TOP_PRE_AGG_MRU_USER_BW_SIZE                                   (0x00002500)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_AO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_0_DPD_RAM_BO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_AO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_1_DPD_RAM_BO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_AO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_2_DPD_RAM_BO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_AO_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BE_1_SIZE                                     (0x00000300)
#define B2_HYP_TD_ANTENNA_3_DPD_RAM_BO_1_SIZE                                     (0x00000300)
#define IRAM_OCP64_IRAM_6_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_7_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_8_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_9_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_10_IRAM_HIGH_SIZE                                         (0x00020000)
#define IRAM_OCP64_IRAM_11_IRAM_HIGH_SIZE                                         (0x00020000)
#define IRAM_OCP64_IRAM_12_IRAM_HIGH_SIZE                                         (0x00020000)
#define IRAM_OCP64_IRAM_13_IRAM_HIGH_SIZE                                         (0x00020000)
#define IRAM_OCP64_IRAM_0_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_1_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_2_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_3_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_4_IRAM_HIGH_SIZE                                          (0x00020000)
#define IRAM_OCP64_IRAM_5_IRAM_HIGH_SIZE                                          (0x00020000)

 /* Host Interface Sizes */
#define HOST_IF_U_ICCM_RAM_ADDR_FROM_UMAC                                         (MAC_HOSTIF_UC_INTERNAL_BASE_ADDR + 0x00010000)
#define HOST_IF_U_ICCM_RAM_ADDR_FROM_HI                                           (0x70000000)
#define HOST_IF_U_ICCM_RAM_SIZE                                                   (0x00004000)
#define HOST_IF_U_DCCM_RAM_ADDR_FROM_HI                                           (0xF0000000)
#define HOST_IF_U_DCCM_RAM_SIZE                                                   (0x00002000)

#endif /* _MEMORY_MAP_H_*/
