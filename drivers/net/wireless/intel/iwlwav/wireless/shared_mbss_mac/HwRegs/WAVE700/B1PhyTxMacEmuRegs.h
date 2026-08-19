
/***********************************************************************************
File:				B1PhyTxMacEmuRegs.h
Module:				b1PhyTxMacEmu
SOC Revision:		latest
Generated at:       2022-05-04 10:57:52
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_PHY_TX_MAC_EMU_REGS_H_
#define _B1_PHY_TX_MAC_EMU_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_PHY_TX_MAC_EMU_BASE_ADDRESS                             MEMORY_MAP_UNIT_10203_BASE_ADDRESS
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2000)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2004)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2008)
#define	REG_B1_PHY_TX_MAC_EMU_ENDLESS_TYPE                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x200C)
#define	REG_B1_PHY_TX_MAC_EMU_MAC_EMU_MODE                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2010)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_MAIN_STM                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2014)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_DEL_STM                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2018)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x201C)
#define	REG_B1_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2020)
#define	REG_B1_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2024)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2028)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x202C)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2030)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2034)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2038)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_ZLD                                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x203C)
#define	REG_B1_PHY_TX_MAC_EMU_TX_MAC_PHY_IF                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2040)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2044)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2048)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x204C)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2050)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2054)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_BF_LOW                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2058)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH                              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x205C)
#define	REG_B1_PHY_TX_MAC_EMU_SIG_CRC_DEFS                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2060)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TIMING                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2064)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_TIMING                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2068)
#define	REG_B1_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x206C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2070)
#define	REG_B1_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE                              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2074)
#define	REG_B1_PHY_TX_MAC_EMU_READ_LOGIC_CTRL                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2078)
#define	REG_B1_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x207C)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2100)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2104)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2108)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x210C)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2110)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2114)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2118)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x211C)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2120)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2124)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2128)
#define	REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x212C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_0                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2130)
#define	REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_1                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2134)
#define	REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_2                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2138)
#define	REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_3                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x213C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_0                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2140)
#define	REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_1                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2144)
#define	REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_2                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2148)
#define	REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_3                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x214C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2150)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2154)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2158)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x215C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2160)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2164)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2168)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x216C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_CBW                              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2170)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2174)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2178)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x217C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2180)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2184)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2188)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x218C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2190)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2194)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2198)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x219C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21AC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SCP                              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21BC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_STBC                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21CC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21DC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2200)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2204)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2208)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x220C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2210)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_VALID                              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2214)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2218)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x221C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2220)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2224)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2228)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x222C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2230)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2234)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2238)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x223C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2240)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2244)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2248)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x224C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2250)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2254)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2258)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x225C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2260)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2264)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2268)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x226C)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2270)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2274)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2278)
#define	REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x227C)
#define	REG_B1_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2280)
#define	REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2284)
#define	REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2288)
#define	REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x228C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2290)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2              (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2294)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2298)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BEACON                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x229C)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22AC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B4)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_LDPC                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B8)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22BC)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22C0)
#define	REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22C4)
#define	REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_0                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F0)
#define	REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_1                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F4)
#define	REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_2                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F8)
#define	REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_3                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22FC)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2800)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2804)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_SCP                                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2808)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_STBC                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x280C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2810)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_BW                                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2814)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2818)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_NESS                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x281C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2820)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2824)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2828)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1                     (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x282C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2830)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2834)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2838)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SYM                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x283C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2840)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2844)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2848)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_CP                                (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x284C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_LTF                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2850)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2854)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR                             (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2858)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x285C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_MAX_PE                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2860)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2864)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2868)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x286C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2870)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2874)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_MODULATION                           (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2878)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x287C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2880)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2884)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2888)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x288C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW                   (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2890)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW                    (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2894)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA                          (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2898)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x289C)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A0)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A4)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A8)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS                  (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28AC)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B0)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES                         (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B4)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_T_PE                                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B8)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28BC)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C0)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT                 (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C4)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C8)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT                      (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28CC)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR                       (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D0)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_CBPS                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D4)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_DBPS                               (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D8)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST                            (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28DC)
#define	REG_B1_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME                        (B1_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28E0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00 0x2000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuRun : 1; //Emulator Run, reset value: 0x0, access type: WO
		uint32 emuTxDone : 1; //Emulator Transmit Done, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 emuLoadDataRam : 1; //load data ram start, reset value: 0x0, access type: WO
		uint32 emuLoadDataRamBusy : 1; //load data ram busy, reset value: 0x0, access type: RO
		uint32 reserved1 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg00_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01 0x2004 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuNumOfPackets : 32; //Number of Packets, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg01_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02 0x2008 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 emuCrcInMode : 1; //Insert CRC, reset value: 0x0, access type: RW
		uint32 emuCrcInvert : 1; //Currupt CRC, reset value: 0x1, access type: RW
		uint32 emuCrcReverse : 1; //Reverse CRC, reset value: 0x1, access type: RW
		uint32 reserved1 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg02_u;

/*REG_B1_PHY_TX_MAC_EMU_ENDLESS_TYPE 0x200C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 endlessPackets : 1; //endless packet after packet, reset value: 0x0, access type: RW
		uint32 endlessData : 1; //space less, reset value: 0x0, access type: RW
		uint32 endlessLdpc : 1; //space less ldpc random, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuEndlessType_u;

/*REG_B1_PHY_TX_MAC_EMU_MAC_EMU_MODE 0x2010 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macEmuMode : 1; //enable mac emulator mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuMacEmuMode_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_MAIN_STM 0x2014 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuMainStm : 3; //emu_main_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuEmuMainStm_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_DEL_STM 0x2018 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuDelStm : 5; //emu_del_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1PhyTxMacEmuEmuDelStm_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER 0x201C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPacketCounter : 32; //emu_packet_counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuEmuPacketCounter_u;

/*REG_B1_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME 0x2020 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetEndOnAirTimeOfdm : 16; //packet_end_on_air_time_ofdm, reset value: 0x140, access type: RW
		uint32 packetEndOnAirTime11B : 16; //packet_end_on_air_time_11b, reset value: 0x500, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuPacketEndOnAirTime_u;

/*REG_B1_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE 0x2024 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigaBeamChangeActive : 1; //he_siga_beam_change_active, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuHeSigaBeamChangeActive_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A 0x2028 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPatternMode : 2; //Transmit pattern mode 0-prbs, 1-fix val, 2-cnt, 3-external (including TCRs), reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 emuInsPrbsErr : 1; //Insert PRBS Errors, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 emuInsErrNum : 9; //PRBS Error Rate, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg0A_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B 0x202C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsType : 32; //PRBS Type, reset value: 0x80000003, access type: CONST
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg0B_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C 0x2030 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsInit : 32; //PRBS Init Data, reset value: 0x162, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg0C_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D 0x2034 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuFixedData : 32; //Fixed Data Word, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg0D_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E 0x2038 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuIpgDly : 16; //IPG delay, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTxMacEmuReg0E_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_ZLD 0x203C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyZld : 32; //tx_phy_zld, reset value: 0x4E790001, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTxPhyZld_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_MAC_PHY_IF 0x2040 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyEnable : 1; //mac_phy_enable, reset value: 0x0, access type: RO
		uint32 txPhyReady : 1; //phy_mac_ready, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txPhyValid : 1; //mac_phy_valid, reset value: 0x0, access type: RO
		uint32 txPhyBusState : 4; //mac_phy_bus_state, reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegB1PhyTxMacEmuTxMacPhyIf_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER 0x2044 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyPacketTimer : 32; //tx_phy_packet_timer, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTxPhyPacketTimer_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM 0x2048 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyMainStm : 4; //tx_phy_main_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyMainStm_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER 0x204C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyTcrCounter : 6; //tx_phy_tcr_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyTcrCounter_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER 0x2050 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyRcrCounter : 6; //tx_phy_rcr_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyRcrCounter_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER 0x2054 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhySymbolCounter : 15; //tx_phy_symbol_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuTxPhySymbolCounter_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_BF_LOW 0x2058 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyBfLow : 18; //tx_phy_bf_low, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyBfLow_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH 0x205C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyBfHigh : 18; //tx_phy_bf_high, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyBfHigh_u;

/*REG_B1_PHY_TX_MAC_EMU_SIG_CRC_DEFS 0x2060 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigCrcType : 8; //sig_crc_type, reset value: 0x60, access type: RW
		uint32 sigCrcInit : 8; //sig_crc_init, reset value: 0xff, access type: RW
		uint32 sigCrcLen : 4; //sig_crc_len, reset value: 0x7, access type: RW
		uint32 sigCrcResultInvert : 1; //sig_crc_result_invert, reset value: 0x1, access type: RW
		uint32 reserved0 : 11;
	} bitFields;
} RegB1PhyTxMacEmuSigCrcDefs_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TIMING 0x2064 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommon0Timing : 16; //tcr_common_0_timing, reset value: 0x140, access type: RW
		uint32 tcrCommon1Timing : 16; //tcr_common_1_timing, reset value: 0x280, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonTiming_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_TIMING 0x2068 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUser0Timing : 16; //tcr_user_0_timing, reset value: 0x3C0, access type: RW
		uint32 tcrUserLastTiming : 16; //tcr_user_last_timing, reset value: 0x640, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuTcrUserTiming_u;

/*REG_B1_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING 0x206C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetStartTrSwitching : 16; //packet_start_tr_switching, reset value: 0x640, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuPacketStartTrSwitching_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT 0x2070 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelect : 6; //tcr_user_select, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelect_u;

/*REG_B1_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE 0x2074 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyMuType : 2; //tx_phy_mu_type, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTxPhyMuType_u;

/*REG_B1_PHY_TX_MAC_EMU_READ_LOGIC_CTRL 0x2078 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 readLogicLimitOvrEn : 1; //overide en of read logic limit, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 readLogicLimitVal : 5; //overide en of read logic limit value, reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB1PhyTxMacEmuReadLogicCtrl_u;

/*REG_B1_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC 0x207C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pllRate160StrbCntSync : 3; //sync value when to start tx, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuPllRate160StrbCntSync_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0 0x2100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr00 : 32; //emu_phy_tx_phy_tcr0_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr00_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1 0x2104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr01 : 32; //emu_phy_tx_phy_tcr0_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr01_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2 0x2108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr02 : 32; //emu_phy_tx_phy_tcr0_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr02_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3 0x210C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr03 : 32; //emu_phy_tx_phy_tcr0_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr03_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0 0x2110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr10 : 32; //emu_phy_tx_phy_tcr1_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr10_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1 0x2114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr11 : 32; //emu_phy_tx_phy_tcr1_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr11_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2 0x2118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr12 : 32; //emu_phy_tx_phy_tcr1_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr12_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3 0x211C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr13 : 32; //emu_phy_tx_phy_tcr1_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcr13_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0 0x2120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU0 : 32; //emu_phy_tx_phy_tcr_u_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcrU0_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1 0x2124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU1 : 32; //emu_phy_tx_phy_tcr_u_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcrU1_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2 0x2128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU2 : 32; //emu_phy_tx_phy_tcr_u_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcrU2_u;

/*REG_B1_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3 0x212C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU3 : 32; //emu_phy_tx_phy_tcr_u_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PhyTxMacEmuEmuPhyTxPhyTcrU3_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_0 0x2130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data0 : 32; //tcr0_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr0Data0_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_1 0x2134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data1 : 32; //tcr0_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr0Data1_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_2 0x2138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data2 : 32; //tcr0_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr0Data2_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR0_DATA_3 0x213C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data3 : 32; //tcr0_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr0Data3_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_0 0x2140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data0 : 32; //tcr1_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr1Data0_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_1 0x2144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data1 : 32; //tcr1_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr1Data1_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_2 0x2148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data2 : 32; //tcr1_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr1Data2_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR1_DATA_3 0x214C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data3 : 32; //tcr1_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcr1Data3_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0 0x2150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData0 : 32; //tcr_user_select_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectData0_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1 0x2154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData1 : 32; //tcr_user_select_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectData1_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2 0x2158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData2 : 32; //tcr_user_select_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectData2_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3 0x215C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData3 : 32; //tcr_user_select_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectData3_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT 0x2160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAntSelect : 16; //tcr_common_ant_select, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonAntSelect_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST 0x2164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAntBoost : 16; //tcr_common_ant_boost, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonAntBoost_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER 0x2168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonRfPower : 8; //tcr_common_rf_power, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonRfPower_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE 0x216C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonLoopMode : 2; //tcr_common_loop_mode, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonLoopMode_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_CBW 0x2170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonCbw : 3; //tcr_common_cbw, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonCbw_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE 0x2174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonPhyMode : 4; //tcr_common_phy_mode, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonPhyMode_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP 0x2178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHePuncturingMap : 16; //tcr_common_he_puncturing_map, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonHePuncturingMap_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX 0x217C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonForceTx : 1; //tcr_common_force_tx, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonForceTx_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR 0x2180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBssColor : 6; //tcr_common_bss_color, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonBssColor_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF 0x2184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonCustomBf : 2; //tcr_common_custom_bf, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonCustomBf_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF 0x2188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonNHeLtf : 3; //tcr_common_n_he_ltf, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonNHeLtf_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL 0x218C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonUlDl : 1; //tcr_common_ul_dl, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonUlDl_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED 0x2190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTxopPsNotAllowed : 1; //tcr_common_txop_ps_not_allowed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonTxopPsNotAllowed_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR 0x2194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonExpectRcr : 1; //tcr_common_expect_rcr, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonExpectRcr_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE 0x2198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBwChange : 1; //tcr_common_bw_change, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonBwChange_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC 0x219C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTxopDurationFromMac : 1; //tcr_common_txop_duration_from_mac, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonTxopDurationFromMac_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE 0x21A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAggregate : 1; //tcr_common_aggregate, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonAggregate_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW 0x21A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonDynBw : 1; //tcr_common_dyn_bw, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonDynBw_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP 0x21A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMuPhyNdp : 1; //tcr_common_mu_phy_ndp, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonMuPhyNdp_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING 0x21AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMuTraining : 1; //tcr_common_mu_training, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonMuTraining_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP 0x21B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeCp : 2; //tcr_common_he_cp, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonHeCp_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF 0x21B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeLtf : 2; //tcr_common_he_ltf, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonHeLtf_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SCP 0x21B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonScp : 1; //tcr_common_scp, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonScp_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING 0x21BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonSmoothing : 1; //tcr_common_smoothing, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonSmoothing_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING 0x21C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonNotSounding : 1; //tcr_common_not_sounding   , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonNotSounding_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_STBC 0x21C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonStbc : 1; //tcr_common_stbc, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonStbc_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE 0x21C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonSpatialReuse : 16; //tcr_common_spatial_reuse, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonSpatialReuse_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA 0x21CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTriggerBasedData : 16; //tcr_common_trigger_based_data   , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonTriggerBasedData_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID 0x21D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonGroupId : 6; //tcr_common_group_id, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonGroupId_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE 0x21D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeSigbRate : 8; //tcr_common_he_sigb_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonHeSigbRate_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION 0x21D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMacDuration : 15; //tcr_common_mac_duration, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonMacDuration_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH 0x21DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduLength : 23; //tcr_user_select_psdu_length, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectPsduLength_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND 0x21E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectSubBand : 4; //tcr_user_select_sub_band, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectSubBand_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU 0x21E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectStartRu : 4; //tcr_user_select_start_ru, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectStartRu_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE 0x21E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectRuSize : 4; //tcr_user_select_ru_size, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectRuSize_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF 0x21F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectTxbf : 1; //tcr_user_select_txbf, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectTxbf_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE 0x21F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduRate : 8; //tcr_user_select_psdu_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectPsduRate_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER 0x21F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectRelativePower : 4; //tcr_user_select_relative_power, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectRelativePower_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID 0x2200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectStationId : 11; //tcr_user_select_station_id, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectStationId_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX 0x2204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectBfIndex : 8; //tcr_user_select_bf_index, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectBfIndex_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS 0x2208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduMcs : 6; //tcr_user_select_psdu_mcs, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectPsduMcs_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK 0x220C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduRank : 2; //tcr_user_select_psdu_rank, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectPsduRank_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM 0x2210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduDcm : 1; //tcr_user_select_psdu_dcm, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectPsduDcm_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_VALID 0x2214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeValid : 1; //air_time_valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeValid_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED 0x2218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLLengthExceed : 1; //air_time_common_l_length_exceed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonLLengthExceed_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM 0x221C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonNumSym : 15; //air_time_common_num_sym, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonNumSym_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR 0x2220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAFactor : 2; //air_time_common_a_factor, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonAFactor_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE 0x2224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonTPe : 5; //air_time_common_t_pe, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonTPe_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES 0x2228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAirTimeCycles : 26; //air_time_common_air_time_cycles, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonAirTimeCycles_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME 0x222C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonPreambleTime : 20; //air_time_common_preamble_time, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonPreambleTime_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL 0x2230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLdpcExtraSymbol : 1; //air_time_common_ldpc_extra_symbol, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonLdpcExtraSymbol_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT 0x2234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAirTimeResult : 17; //air_time_common_air_time_result, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonAirTimeResult_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH 0x2238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLLength : 12; //air_time_common_l_length, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonLLength_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1 0x223C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonVhtSigA2B1 : 1; //air_time_common_vht_sig_a2_b1, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonVhtSigA2B1_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY 0x2240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonPeDisambiguity : 1; //air_time_common_pe_disambiguity, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeCommonPeDisambiguity_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE 0x2244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectCodingRate : 2; //air_time_user_select_coding_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectCodingRate_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION 0x2248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectModulation : 3; //air_time_user_select_modulation, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectModulation_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW 0x224C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNshrtNcw : 16; //air_time_user_select_ldpc_rem_nshrt_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcRemNshrtNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW 0x2250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNpuncNcw : 16; //air_time_user_select_ldpc_rem_npunc_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcRemNpuncNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW 0x2254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNrepNcw : 16; //air_time_user_select_ldpc_rem_nrep_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcRemNrepNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA 0x2258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNData : 11; //air_time_user_select_ldpc_n_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcNData_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY 0x225C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNParity : 11; //air_time_user_select_ldpc_n_parity, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcNParity_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT 0x2260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNRepeat : 15; //air_time_user_select_ldpc_n_repeat, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcNRepeat_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE 0x2264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcCwType : 2; //air_time_user_select_ldpc_cw_type, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcCwType_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW 0x2268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNCw : 16; //air_time_user_select_ldpc_n_cw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectLdpcNCw_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING 0x226C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectPadding : 26; //air_time_user_select_padding, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectPadding_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES 0x2270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNes : 3; //air_time_user_select_nes, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectNes_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS 0x2274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNCbps : 18; //air_time_user_select_n_cbps, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectNCbps_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS 0x2278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNDbps : 18; //air_time_user_select_n_dbps, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectNDbps_u;

/*REG_B1_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST 0x227C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNSdLast : 12; //air_time_user_select_n_sd_last, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PhyTxMacEmuAirTimeUserSelectNSdLast_u;

/*REG_B1_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM 0x2280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigbNumSym : 6; //he_sigb_num_sym, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PhyTxMacEmuHeSigbNumSym_u;

/*REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR 0x2284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTcr0TimingError : 1; //tx_tcr0_timing_error, reset value: 0x0, access type: RO
		uint32 txTcr1TimingError : 1; //tx_tcr1_timing_error, reset value: 0x0, access type: RO
		uint32 txTcrUTimingError : 1; //tx_tcr_u_timing_error, reset value: 0x0, access type: RO
		uint32 txTcrMuTimingError : 1; //tx_tcr_mu_timing_error, reset value: 0x0, access type: RO
		uint32 txBfError : 1; //tx_bf_error, reset value: 0x0, access type: RO
		uint32 txLLengthExceedError : 1; //tx_l_length_exceed_error, reset value: 0x0, access type: RO
		uint32 txRcrTimingError : 1; //tx_rcr_timing_error, reset value: 0x0, access type: RO
		uint32 txHeSigbNumSymError : 1; //tx_he_sigb_num_sym_error, reset value: 0x0, access type: RO
		uint32 txEndOfAirError : 1; //tx_end_of_air_error, reset value: 0x0, access type: RO
		uint32 txTbAirTimeError : 1; //tx_tb_air_time_error, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB1PhyTxMacEmuPhyMacError_u;

/*REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR 0x2288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMacErrorClear : 1; //clears all error indications, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuPhyMacErrorClear_u;

/*REG_B1_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN 0x228C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTcr0TimingErrorIntEn : 1; //tx_tcr0_timing_error_int_en, reset value: 0x1, access type: RW
		uint32 txTcr1TimingErrorIntEn : 1; //tx_tcr1_timing_error_int_en, reset value: 0x1, access type: RW
		uint32 txTcrUTimingErrorIntEn : 1; //tx_tcr_u_timing_error_int_en, reset value: 0x1, access type: RW
		uint32 txTcrMuTimingErrorIntEn : 1; //tx_tcr_mu_timing_error_int_en, reset value: 0x1, access type: RW
		uint32 txBfErrorIntEn : 1; //tx_bf_error_int_en, reset value: 0x1, access type: RW
		uint32 txLLengthExceedErrorIntEn : 1; //tx_l_length_exceed_error_int_en, reset value: 0x1, access type: RW
		uint32 txRcrTimingErrorIntEn : 1; //tx_rcr_timing_error_int_en, reset value: 0x1, access type: RW
		uint32 txHeSigbNumSymErrorIntEn : 1; //tx_he_sigb_num_sym_error_int_en, reset value: 0x1, access type: RW
		uint32 txEndOfAirErrorIntEn : 1; //tx_end_of_air_error_int_en, reset value: 0x1, access type: RW
		uint32 txTbAirTimeErrorIntEn : 1; //tx_tb_air_time_error_int_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1PhyTxMacEmuPhyMacErrorIntEn_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE 0x2290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonDpdCalibrate : 1; //tcr_common_dpd_calibrate, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonDpdCalibrate_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2 0x2294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTriggerBasedSiga2 : 9; //tcr_common_trigger_based_siga2, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonTriggerBasedSiga2_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT 0x2298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonReuseScramblerInit : 1; //tcr_common_reuse_scrambler_init, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonReuseScramblerInit_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_BEACON 0x229C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBeacon : 1; //tcr_common_beacon, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonBeacon_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED 0x22A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeSigbCompressed : 1; //tcr_user_select_he_sigb_compressed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectHeSigbCompressed_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS 0x22A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeSigb16Symbols : 1; //tcr_user_select_he_sigb_16_symbols, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectHeSigb16Symbols_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS 0x22A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeTbStartingSts : 3; //tcr_user_select_he_tb_starting_sts, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectHeTbStartingSts_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX 0x22AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbRuToneSetIndex : 5; //tcr_user_select_ndp_fb_ru_tone_set_index, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectNdpFbRuToneSetIndex_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS 0x22B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbStartingSts : 1; //tcr_user_select_ndp_fb_starting_sts, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectNdpFbStartingSts_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE 0x22B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbValue : 1; //tcr_user_select_ndp_fb_value, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectNdpFbValue_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_LDPC 0x22B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonLdpc : 1; //tcr_common_ldpc, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonLdpc_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION 0x22BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonPacketExtension : 2; //tcr_common_packet_extension, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuTcrCommonPacketExtension_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU 0x22C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectMru : 4; //tcr_user_select_ru_size, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectMru_u;

/*REG_B1_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP 0x22C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectDup : 1; //tcr_user_select_dup, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuTcrUserSelectDup_u;

/*REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_0 0x22F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData0 : 32; //first user data [31:0], reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuUserFirstData0_u;

/*REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_1 0x22F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData1 : 32; //first user data [63:32], reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuUserFirstData1_u;

/*REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_2 0x22F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData2 : 32; //first user data [95:64], reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuUserFirstData2_u;

/*REG_B1_PHY_TX_MAC_EMU_USER_FIRST_DATA_3 0x22FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData3 : 32; //first user data [127:96], reset value: 0x0, access type: RO
	} bitFields;
} RegB1PhyTxMacEmuUserFirstData3_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START 0x2800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCalcStart : 1; //bb_cpu_air_time_calc_start       , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAirTimeCalcStart_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC 0x2804 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpc : 1; //bb_cpu_ldpc                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpc_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_SCP 0x2808 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuScp : 1; //bb_cpu_scp                       , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuScp_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_STBC 0x280C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuStbc : 2; //bb_cpu_stbc                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuStbc_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE 0x2810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPsduRate : 8; //bb_cpu_psdu_rate                 , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPsduRate_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_BW 0x2814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuBw : 3; //bb_cpu_bw                        , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuBw_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE 0x2818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPacketMode : 4; //bb_cpu_packet_mode               , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPacketMode_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_NESS 0x281C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNess : 3; //bb_cpu_ness                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNess_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH 0x2820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPacketLength : 23; //bb_cpu_packet_length             , reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPacketLength_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR 0x2824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxTxNSelector : 1; //bb_cpu_rx_tx_n_selector          , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuRxTxNSelector_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH 0x2828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxLLength : 12; //bb_cpu_rx_l_length               , reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuRxLLength_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1 0x282C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxVhtSigA2B1 : 1; //bb_cpu_rx_vht_sig_a2_b1          , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuRxVhtSigA2B1_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL 0x2830 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxLdpcExtraSymbol : 1; //bb_cpu_rx_ldpc_extra_symbol      , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuRxLdpcExtraSymbol_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE 0x2834 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuMuStage : 2; //bb_cpu_mu_stage                  , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuMuStage_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT 0x2838 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSymInit : 15; //bb_cpu_n_sym_init                , reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNSymInit_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SYM 0x283C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSym : 15; //bb_cpu_n_sym                     , reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNSym_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL 0x2840 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNLtfTotal : 3; //bb_cpu_n_ltf_total               , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNLtfTotal_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE 0x2844 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeRuSize : 4; //bb_cpu_he_ru_size                , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuHeRuSize_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM 0x2848 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSigbSym : 8; //bb_cpu_n_sigb_sym                , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNSigbSym_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_CP 0x284C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeCp : 2; //bb_cpu_he_cp                     , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuHeCp_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_HE_LTF 0x2850 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeLtf : 2; //bb_cpu_he_ltf                    , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuHeLtf_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY 0x2854 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPeDisambiguity : 1; //bb_cpu_pe_disambiguity           , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPeDisambiguity_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR 0x2858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactor : 2; //bb_cpu_a_factor                  , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAFactor_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT 0x285C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorInit : 2; //bb_cpu_a_factor_init             , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAFactorInit_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_MAX_PE 0x2860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuMaxPe : 2; //bb_cpu_max_pe                    , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuMaxPe_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID 0x2864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCalcValid : 1; //bb_cpu_air_time_calc_valid       , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAirTimeCalcValid_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED 0x2868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLLengthExceed : 1; //bb_cpu_l_length_exceed           , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLLengthExceed_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1 0x286C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuVhtSigA2B1 : 1; //bb_cpu_vht_sig_a2_b1             , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuVhtSigA2B1_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL 0x2870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcExtraSymbol : 1; //bb_cpu_ldpc_extra_symbol         , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcExtraSymbol_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE 0x2874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuCodingRate : 2; //bb_cpu_coding_rate               , reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuCodingRate_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_MODULATION 0x2878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuModulation : 3; //bb_cpu_modulation                , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuModulation_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT 0x287C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeResult : 17; //bb_cpu_air_time_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAirTimeResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT 0x2880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLLengthResult : 12; //bb_cpu_l_length_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLLengthResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT 0x2884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNumSymResult : 15; //bb_cpu_num_sym_result            , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNumSymResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT 0x2888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNumSymInitResult : 15; //bb_cpu_num_sym_init_result       , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNumSymInitResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW 0x288C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNshrtNcw : 16; //bb_cpu_ldpc_rem_nshrt_ncw        , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcRemNshrtNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW 0x2890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNpuncNcw : 16; //bb_cpu_ldpc_rem_npunc_ncw        , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcRemNpuncNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW 0x2894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNrepNcw : 16; //bb_cpu_ldpc_rem_nrep_ncw         , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcRemNrepNcw_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA 0x2898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNData : 11; //bb_cpu_ldpc_n_data               , reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcNData_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY 0x289C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNParity : 11; //bb_cpu_ldpc_n_parity             , reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcNParity_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT 0x28A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNRepeat : 15; //bb_cpu_ldpc_n_repeat             , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcNRepeat_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE 0x28A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcCwType : 2; //bb_cpu_ldpc_cw_type              , reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcCwType_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW 0x28A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNCw : 16; //bb_cpu_ldpc_n_cw                 , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuLdpcNCw_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS 0x28AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuTxNumPaddingBits : 26; //bb_cpu_tx_num_padding_bits       , reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuTxNumPaddingBits_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC 0x28B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNPadPostFec : 17; //bb_cpu_n_pad_post_fec            , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNPadPostFec_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES 0x28B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuViterbiNEs : 3; //bb_cpu_viterbi_n_es              , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuViterbiNEs_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_T_PE 0x28B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuTPe : 5; //bb_cpu_t_pe                      , reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuTPe_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT 0x28BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPeDisambiguityResult : 1; //bb_cpu_pe_disambiguity_result    , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPeDisambiguityResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT 0x28C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorResult : 3; //bb_cpu_a_factor_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAFactorResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT 0x28C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorInitResult : 3; //bb_cpu_a_factor_init_result      , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAFactorInitResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES 0x28C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCycles : 26; //bb_cpu_air_time_cycles           , reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAirTimeCycles_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT 0x28CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPhyRateResult : 17; //bb_cpu_phy_rate_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPhyRateResult_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR 0x28D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeError : 1; //bb_cpu_air_time_error            , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuAirTimeError_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_CBPS 0x28D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNCbps : 18; //bb_cpu_n_cbps                    , reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNCbps_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_DBPS 0x28D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNDbps : 18; //bb_cpu_n_dbps                    , reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNDbps_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST 0x28DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSdLast : 12; //bb_cpu_n_sd_last                 , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuNSdLast_u;

/*REG_B1_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME 0x28E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPreambleTime : 20; //bb_cpu_preamble_time , reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1PhyTxMacEmuBbCpuPreambleTime_u;



#endif // _B1_PHY_TX_MAC_EMU_REGS_H_
