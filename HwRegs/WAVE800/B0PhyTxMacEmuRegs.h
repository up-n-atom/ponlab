
/***********************************************************************************
File:				B0PhyTxMacEmuRegs.h
Module:				b0PhyTxMacEmu
SOC Revision:		latest
Generated at:       2024-06-26 12:54:31
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PHY_TX_MAC_EMU_REGS_H_
#define _B0_PHY_TX_MAC_EMU_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PHY_TX_MAC_EMU_BASE_ADDRESS                            MEMORY_MAP_UNIT_203_BASE_ADDRESS
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2000)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2004)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2008)
#define	REG_B0_PHY_TX_MAC_EMU_ENDLESS_TYPE                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x200C)
#define	REG_B0_PHY_TX_MAC_EMU_MAC_EMU_MODE                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2010)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_MAIN_STM                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2014)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_DEL_STM                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2018)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x201C)
#define	REG_B0_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2020)
#define	REG_B0_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2024)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2028)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x202C)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2030)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2034)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2038)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_ZLD                                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x203C)
#define	REG_B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2040)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2044)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2048)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x204C)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2050)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2054)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_BF_LOW                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2058)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH                              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x205C)
#define	REG_B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2060)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TIMING                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2064)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_TIMING                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2068)
#define	REG_B0_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x206C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2070)
#define	REG_B0_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE                              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2074)
#define	REG_B0_PHY_TX_MAC_EMU_READ_LOGIC_CTRL                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2078)
#define	REG_B0_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x207C)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2100)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2104)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2108)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x210C)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2110)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2114)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2118)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x211C)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2120)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2124)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2128)
#define	REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x212C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_0                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2130)
#define	REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_1                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2134)
#define	REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_2                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2138)
#define	REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_3                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x213C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_0                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2140)
#define	REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_1                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2144)
#define	REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_2                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2148)
#define	REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_3                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x214C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2150)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2154)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2158)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x215C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2160)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2164)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2168)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x216C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_CBW                              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2170)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2174)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2178)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x217C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2180)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2184)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2188)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x218C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2190)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2194)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2198)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x219C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21A8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21AC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SCP                              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21B8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21BC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_STBC                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21C8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21CC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21D8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21DC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21E8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x21F8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2200)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2204)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2208)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x220C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2210)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_VALID                              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2214)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2218)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x221C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2220)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2224)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2228)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x222C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2230)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2234)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2238)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x223C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2240)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2244)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2248)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x224C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2250)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2254)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2258)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x225C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2260)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2264)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2268)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x226C)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2270)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2274)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2278)
#define	REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x227C)
#define	REG_B0_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2280)
#define	REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2284)
#define	REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2288)
#define	REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x228C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2290)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2              (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2294)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2298)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BEACON                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x229C)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22A8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22AC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B4)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_LDPC                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22B8)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22BC)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22C0)
#define	REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22C4)
#define	REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_0                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F0)
#define	REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_1                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F4)
#define	REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_2                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22F8)
#define	REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_3                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x22FC)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2800)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2804)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_SCP                                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2808)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_STBC                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x280C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2810)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_BW                                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2814)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2818)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_NESS                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x281C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2820)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2824)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2828)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1                     (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x282C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2830)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2834)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2838)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x283C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2840)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2844)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2848)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_CP                                (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x284C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_LTF                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2850)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2854)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR                             (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2858)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x285C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_MAX_PE                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2860)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2864)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2868)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x286C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2870)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2874)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_MODULATION                           (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2878)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x287C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2880)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2884)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2888)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x288C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW                   (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2890)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW                    (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2894)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA                          (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x2898)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x289C)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A0)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A4)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28A8)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS                  (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28AC)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B0)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES                         (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B4)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_T_PE                                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28B8)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28BC)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C0)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT                 (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C4)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28C8)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT                      (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28CC)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR                       (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D0)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_CBPS                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D4)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_DBPS                               (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28D8)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST                            (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28DC)
#define	REG_B0_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME                        (B0_PHY_TX_MAC_EMU_BASE_ADDRESS + 0x28E0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00 0x2000 */
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
} RegB0PhyTxMacEmuTxMacEmuReg00_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01 0x2004 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuNumOfPackets : 32; //Number of Packets, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTxMacEmuReg01_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02 0x2008 */
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
} RegB0PhyTxMacEmuTxMacEmuReg02_u;

/*REG_B0_PHY_TX_MAC_EMU_ENDLESS_TYPE 0x200C */
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
} RegB0PhyTxMacEmuEndlessType_u;

/*REG_B0_PHY_TX_MAC_EMU_MAC_EMU_MODE 0x2010 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macEmuMode : 1; //enable mac emulator mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuMacEmuMode_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_MAIN_STM 0x2014 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuMainStm : 3; //emu_main_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuEmuMainStm_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_DEL_STM 0x2018 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuDelStm : 5; //emu_del_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxMacEmuEmuDelStm_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER 0x201C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPacketCounter : 32; //emu_packet_counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuEmuPacketCounter_u;

/*REG_B0_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME 0x2020 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetEndOnAirTimeOfdm : 16; //packet_end_on_air_time_ofdm, reset value: 0x140, access type: RW
		uint32 packetEndOnAirTime11B : 16; //packet_end_on_air_time_11b, reset value: 0x500, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuPacketEndOnAirTime_u;

/*REG_B0_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE 0x2024 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigaBeamChangeActive : 1; //he_siga_beam_change_active, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuHeSigaBeamChangeActive_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A 0x2028 */
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
} RegB0PhyTxMacEmuTxMacEmuReg0A_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B 0x202C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsType : 32; //PRBS Type, reset value: 0x80000003, access type: CONST
	} bitFields;
} RegB0PhyTxMacEmuTxMacEmuReg0B_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C 0x2030 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsInit : 32; //PRBS Init Data, reset value: 0x162, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTxMacEmuReg0C_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D 0x2034 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuFixedData : 32; //Fixed Data Word, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTxMacEmuReg0D_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E 0x2038 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuIpgDly : 16; //IPG delay, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTxMacEmuReg0E_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_ZLD 0x203C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyZld : 32; //tx_phy_zld, reset value: 0x4E790001, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTxPhyZld_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF 0x2040 */
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
} RegB0PhyTxMacEmuTxMacPhyIf_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER 0x2044 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyPacketTimer : 32; //tx_phy_packet_timer, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTxPhyPacketTimer_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM 0x2048 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyMainStm : 4; //tx_phy_main_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyMainStm_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER 0x204C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyTcrCounter : 6; //tx_phy_tcr_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyTcrCounter_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER 0x2050 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyRcrCounter : 6; //tx_phy_rcr_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyRcrCounter_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER 0x2054 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhySymbolCounter : 15; //tx_phy_symbol_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuTxPhySymbolCounter_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_BF_LOW 0x2058 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyBfLow : 18; //tx_phy_bf_low, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyBfLow_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH 0x205C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyBfHigh : 18; //tx_phy_bf_high, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyBfHigh_u;

/*REG_B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS 0x2060 */
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
} RegB0PhyTxMacEmuSigCrcDefs_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TIMING 0x2064 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommon0Timing : 16; //tcr_common_0_timing, reset value: 0x140, access type: RW
		uint32 tcrCommon1Timing : 16; //tcr_common_1_timing, reset value: 0x280, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonTiming_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_TIMING 0x2068 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUser0Timing : 16; //tcr_user_0_timing, reset value: 0x3C0, access type: RW
		uint32 tcrUserLastTiming : 16; //tcr_user_last_timing, reset value: 0x640, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuTcrUserTiming_u;

/*REG_B0_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING 0x206C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetStartTrSwitching : 16; //packet_start_tr_switching, reset value: 0x640, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuPacketStartTrSwitching_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT 0x2070 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelect : 6; //tcr_user_select, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelect_u;

/*REG_B0_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE 0x2074 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPhyMuType : 2; //tx_phy_mu_type, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTxPhyMuType_u;

/*REG_B0_PHY_TX_MAC_EMU_READ_LOGIC_CTRL 0x2078 */
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
} RegB0PhyTxMacEmuReadLogicCtrl_u;

/*REG_B0_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC 0x207C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pllRate160StrbCntSync : 3; //sync value when to start tx, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuPllRate160StrbCntSync_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0 0x2100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr00 : 32; //emu_phy_tx_phy_tcr0_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr00_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1 0x2104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr01 : 32; //emu_phy_tx_phy_tcr0_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr01_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2 0x2108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr02 : 32; //emu_phy_tx_phy_tcr0_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr02_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3 0x210C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr03 : 32; //emu_phy_tx_phy_tcr0_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr03_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0 0x2110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr10 : 32; //emu_phy_tx_phy_tcr1_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr10_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1 0x2114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr11 : 32; //emu_phy_tx_phy_tcr1_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr11_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2 0x2118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr12 : 32; //emu_phy_tx_phy_tcr1_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr12_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3 0x211C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcr13 : 32; //emu_phy_tx_phy_tcr1_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcr13_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0 0x2120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU0 : 32; //emu_phy_tx_phy_tcr_u_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcrU0_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1 0x2124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU1 : 32; //emu_phy_tx_phy_tcr_u_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcrU1_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2 0x2128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU2 : 32; //emu_phy_tx_phy_tcr_u_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcrU2_u;

/*REG_B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3 0x212C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPhyTxPhyTcrU3 : 32; //emu_phy_tx_phy_tcr_u_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PhyTxMacEmuEmuPhyTxPhyTcrU3_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_0 0x2130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data0 : 32; //tcr0_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr0Data0_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_1 0x2134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data1 : 32; //tcr0_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr0Data1_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_2 0x2138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data2 : 32; //tcr0_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr0Data2_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR0_DATA_3 0x213C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr0Data3 : 32; //tcr0_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr0Data3_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_0 0x2140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data0 : 32; //tcr1_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr1Data0_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_1 0x2144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data1 : 32; //tcr1_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr1Data1_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_2 0x2148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data2 : 32; //tcr1_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr1Data2_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR1_DATA_3 0x214C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcr1Data3 : 32; //tcr1_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcr1Data3_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0 0x2150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData0 : 32; //tcr_user_select_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectData0_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1 0x2154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData1 : 32; //tcr_user_select_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectData1_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2 0x2158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData2 : 32; //tcr_user_select_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectData2_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3 0x215C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectData3 : 32; //tcr_user_select_data_3, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectData3_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT 0x2160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAntSelect : 16; //tcr_common_ant_select, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonAntSelect_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST 0x2164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAntBoost : 16; //tcr_common_ant_boost, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonAntBoost_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER 0x2168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonRfPower : 8; //tcr_common_rf_power, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonRfPower_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE 0x216C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonLoopMode : 2; //tcr_common_loop_mode, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonLoopMode_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_CBW 0x2170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonCbw : 3; //tcr_common_cbw, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonCbw_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE 0x2174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonPhyMode : 4; //tcr_common_phy_mode, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonPhyMode_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP 0x2178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHePuncturingMap : 16; //tcr_common_he_puncturing_map, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonHePuncturingMap_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX 0x217C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonForceTx : 1; //tcr_common_force_tx, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonForceTx_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR 0x2180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBssColor : 6; //tcr_common_bss_color, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonBssColor_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF 0x2184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonCustomBf : 2; //tcr_common_custom_bf, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonCustomBf_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF 0x2188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonNHeLtf : 3; //tcr_common_n_he_ltf, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonNHeLtf_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL 0x218C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonUlDl : 1; //tcr_common_ul_dl, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonUlDl_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED 0x2190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTxopPsNotAllowed : 1; //tcr_common_txop_ps_not_allowed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonTxopPsNotAllowed_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR 0x2194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonExpectRcr : 1; //tcr_common_expect_rcr, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonExpectRcr_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE 0x2198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBwChange : 1; //tcr_common_bw_change, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonBwChange_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC 0x219C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTxopDurationFromMac : 1; //tcr_common_txop_duration_from_mac, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonTxopDurationFromMac_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE 0x21A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonAggregate : 1; //tcr_common_aggregate, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonAggregate_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW 0x21A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonDynBw : 1; //tcr_common_dyn_bw, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonDynBw_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP 0x21A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMuPhyNdp : 1; //tcr_common_mu_phy_ndp, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonMuPhyNdp_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING 0x21AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMuTraining : 1; //tcr_common_mu_training, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonMuTraining_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP 0x21B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeCp : 2; //tcr_common_he_cp, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonHeCp_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF 0x21B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeLtf : 2; //tcr_common_he_ltf, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonHeLtf_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SCP 0x21B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonScp : 1; //tcr_common_scp, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonScp_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING 0x21BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonSmoothing : 1; //tcr_common_smoothing, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonSmoothing_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING 0x21C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonNotSounding : 1; //tcr_common_not_sounding   , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonNotSounding_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_STBC 0x21C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonStbc : 1; //tcr_common_stbc, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonStbc_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE 0x21C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonSpatialReuse : 16; //tcr_common_spatial_reuse, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonSpatialReuse_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA 0x21CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTriggerBasedData : 16; //tcr_common_trigger_based_data   , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonTriggerBasedData_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID 0x21D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonGroupId : 6; //tcr_common_group_id, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonGroupId_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE 0x21D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonHeSigbRate : 8; //tcr_common_he_sigb_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonHeSigbRate_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION 0x21D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonMacDuration : 15; //tcr_common_mac_duration, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonMacDuration_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH 0x21DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduLength : 23; //tcr_user_select_psdu_length, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectPsduLength_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND 0x21E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectSubBand : 4; //tcr_user_select_sub_band, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectSubBand_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU 0x21E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectStartRu : 4; //tcr_user_select_start_ru, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectStartRu_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE 0x21E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectRuSize : 4; //tcr_user_select_ru_size, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectRuSize_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF 0x21F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectTxbf : 1; //tcr_user_select_txbf, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectTxbf_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE 0x21F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduRate : 8; //tcr_user_select_psdu_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectPsduRate_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER 0x21F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectRelativePower : 4; //tcr_user_select_relative_power, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectRelativePower_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID 0x2200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectStationId : 11; //tcr_user_select_station_id, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectStationId_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX 0x2204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectBfIndex : 8; //tcr_user_select_bf_index, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectBfIndex_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS 0x2208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduMcs : 6; //tcr_user_select_psdu_mcs, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectPsduMcs_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK 0x220C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduRank : 2; //tcr_user_select_psdu_rank, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectPsduRank_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM 0x2210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectPsduDcm : 1; //tcr_user_select_psdu_dcm, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectPsduDcm_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_VALID 0x2214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeValid : 1; //air_time_valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeValid_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED 0x2218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLLengthExceed : 1; //air_time_common_l_length_exceed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonLLengthExceed_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM 0x221C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonNumSym : 15; //air_time_common_num_sym, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonNumSym_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR 0x2220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAFactor : 2; //air_time_common_a_factor, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonAFactor_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE 0x2224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonTPe : 5; //air_time_common_t_pe, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonTPe_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES 0x2228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAirTimeCycles : 26; //air_time_common_air_time_cycles, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonAirTimeCycles_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME 0x222C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonPreambleTime : 20; //air_time_common_preamble_time, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonPreambleTime_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL 0x2230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLdpcExtraSymbol : 1; //air_time_common_ldpc_extra_symbol, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonLdpcExtraSymbol_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT 0x2234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonAirTimeResult : 17; //air_time_common_air_time_result, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonAirTimeResult_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH 0x2238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonLLength : 12; //air_time_common_l_length, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonLLength_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1 0x223C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonVhtSigA2B1 : 1; //air_time_common_vht_sig_a2_b1, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonVhtSigA2B1_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY 0x2240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeCommonPeDisambiguity : 1; //air_time_common_pe_disambiguity, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeCommonPeDisambiguity_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE 0x2244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectCodingRate : 2; //air_time_user_select_coding_rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectCodingRate_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION 0x2248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectModulation : 3; //air_time_user_select_modulation, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectModulation_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW 0x224C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNshrtNcw : 16; //air_time_user_select_ldpc_rem_nshrt_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcRemNshrtNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW 0x2250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNpuncNcw : 16; //air_time_user_select_ldpc_rem_npunc_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcRemNpuncNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW 0x2254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcRemNrepNcw : 16; //air_time_user_select_ldpc_rem_nrep_ncw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcRemNrepNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA 0x2258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNData : 11; //air_time_user_select_ldpc_n_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcNData_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY 0x225C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNParity : 11; //air_time_user_select_ldpc_n_parity, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcNParity_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT 0x2260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNRepeat : 15; //air_time_user_select_ldpc_n_repeat, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcNRepeat_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE 0x2264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcCwType : 2; //air_time_user_select_ldpc_cw_type, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcCwType_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW 0x2268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectLdpcNCw : 16; //air_time_user_select_ldpc_n_cw, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectLdpcNCw_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING 0x226C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectPadding : 26; //air_time_user_select_padding, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectPadding_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES 0x2270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNes : 3; //air_time_user_select_nes, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectNes_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS 0x2274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNCbps : 18; //air_time_user_select_n_cbps, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectNCbps_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS 0x2278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNDbps : 18; //air_time_user_select_n_dbps, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectNDbps_u;

/*REG_B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST 0x227C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 airTimeUserSelectNSdLast : 12; //air_time_user_select_n_sd_last, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxMacEmuAirTimeUserSelectNSdLast_u;

/*REG_B0_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM 0x2280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSigbNumSym : 6; //he_sigb_num_sym, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxMacEmuHeSigbNumSym_u;

/*REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR 0x2284 */
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
} RegB0PhyTxMacEmuPhyMacError_u;

/*REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR 0x2288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMacErrorClear : 1; //clears all error indications, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuPhyMacErrorClear_u;

/*REG_B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN 0x228C */
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
} RegB0PhyTxMacEmuPhyMacErrorIntEn_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE 0x2290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonDpdCalibrate : 1; //tcr_common_dpd_calibrate, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonDpdCalibrate_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2 0x2294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonTriggerBasedSiga2 : 9; //tcr_common_trigger_based_siga2, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonTriggerBasedSiga2_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT 0x2298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonReuseScramblerInit : 1; //tcr_common_reuse_scrambler_init, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonReuseScramblerInit_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_BEACON 0x229C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonBeacon : 1; //tcr_common_beacon, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonBeacon_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED 0x22A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeSigbCompressed : 1; //tcr_user_select_he_sigb_compressed, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectHeSigbCompressed_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS 0x22A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeSigb16Symbols : 1; //tcr_user_select_he_sigb_16_symbols, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectHeSigb16Symbols_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS 0x22A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectHeTbStartingSts : 3; //tcr_user_select_he_tb_starting_sts, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectHeTbStartingSts_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX 0x22AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbRuToneSetIndex : 5; //tcr_user_select_ndp_fb_ru_tone_set_index, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectNdpFbRuToneSetIndex_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS 0x22B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbStartingSts : 1; //tcr_user_select_ndp_fb_starting_sts, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectNdpFbStartingSts_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE 0x22B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectNdpFbValue : 1; //tcr_user_select_ndp_fb_value, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectNdpFbValue_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_LDPC 0x22B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonLdpc : 1; //tcr_common_ldpc, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonLdpc_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION 0x22BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrCommonPacketExtension : 2; //tcr_common_packet_extension, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuTcrCommonPacketExtension_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU 0x22C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectMru : 4; //tcr_user_select_ru_size, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectMru_u;

/*REG_B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP 0x22C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrUserSelectDup : 1; //tcr_user_select_dup, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuTcrUserSelectDup_u;

/*REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_0 0x22F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData0 : 32; //first user data [31:0], reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuUserFirstData0_u;

/*REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_1 0x22F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData1 : 32; //first user data [63:32], reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuUserFirstData1_u;

/*REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_2 0x22F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData2 : 32; //first user data [95:64], reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuUserFirstData2_u;

/*REG_B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_3 0x22FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userFirstData3 : 32; //first user data [127:96], reset value: 0x0, access type: RO
	} bitFields;
} RegB0PhyTxMacEmuUserFirstData3_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START 0x2800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCalcStart : 1; //bb_cpu_air_time_calc_start       , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAirTimeCalcStart_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC 0x2804 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpc : 1; //bb_cpu_ldpc                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpc_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_SCP 0x2808 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuScp : 1; //bb_cpu_scp                       , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuScp_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_STBC 0x280C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuStbc : 2; //bb_cpu_stbc                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuStbc_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE 0x2810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPsduRate : 8; //bb_cpu_psdu_rate                 , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPsduRate_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_BW 0x2814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuBw : 3; //bb_cpu_bw                        , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuBw_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE 0x2818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPacketMode : 4; //bb_cpu_packet_mode               , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPacketMode_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_NESS 0x281C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNess : 3; //bb_cpu_ness                      , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNess_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH 0x2820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPacketLength : 23; //bb_cpu_packet_length             , reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPacketLength_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR 0x2824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxTxNSelector : 1; //bb_cpu_rx_tx_n_selector          , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuRxTxNSelector_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH 0x2828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxLLength : 12; //bb_cpu_rx_l_length               , reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuRxLLength_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1 0x282C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxVhtSigA2B1 : 1; //bb_cpu_rx_vht_sig_a2_b1          , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuRxVhtSigA2B1_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL 0x2830 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuRxLdpcExtraSymbol : 1; //bb_cpu_rx_ldpc_extra_symbol      , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuRxLdpcExtraSymbol_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE 0x2834 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuMuStage : 2; //bb_cpu_mu_stage                  , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuMuStage_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT 0x2838 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSymInit : 15; //bb_cpu_n_sym_init                , reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNSymInit_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM 0x283C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSym : 15; //bb_cpu_n_sym                     , reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNSym_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL 0x2840 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNLtfTotal : 3; //bb_cpu_n_ltf_total               , reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNLtfTotal_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE 0x2844 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeRuSize : 4; //bb_cpu_he_ru_size                , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuHeRuSize_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM 0x2848 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSigbSym : 8; //bb_cpu_n_sigb_sym                , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNSigbSym_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_CP 0x284C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeCp : 2; //bb_cpu_he_cp                     , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuHeCp_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_HE_LTF 0x2850 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuHeLtf : 2; //bb_cpu_he_ltf                    , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuHeLtf_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY 0x2854 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPeDisambiguity : 1; //bb_cpu_pe_disambiguity           , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPeDisambiguity_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR 0x2858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactor : 2; //bb_cpu_a_factor                  , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAFactor_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT 0x285C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorInit : 2; //bb_cpu_a_factor_init             , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAFactorInit_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_MAX_PE 0x2860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuMaxPe : 2; //bb_cpu_max_pe                    , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuMaxPe_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID 0x2864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCalcValid : 1; //bb_cpu_air_time_calc_valid       , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAirTimeCalcValid_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED 0x2868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLLengthExceed : 1; //bb_cpu_l_length_exceed           , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLLengthExceed_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1 0x286C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuVhtSigA2B1 : 1; //bb_cpu_vht_sig_a2_b1             , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuVhtSigA2B1_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL 0x2870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcExtraSymbol : 1; //bb_cpu_ldpc_extra_symbol         , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcExtraSymbol_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE 0x2874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuCodingRate : 2; //bb_cpu_coding_rate               , reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuCodingRate_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_MODULATION 0x2878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuModulation : 3; //bb_cpu_modulation                , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuModulation_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT 0x287C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeResult : 17; //bb_cpu_air_time_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAirTimeResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT 0x2880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLLengthResult : 12; //bb_cpu_l_length_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLLengthResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT 0x2884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNumSymResult : 15; //bb_cpu_num_sym_result            , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNumSymResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT 0x2888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNumSymInitResult : 15; //bb_cpu_num_sym_init_result       , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNumSymInitResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW 0x288C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNshrtNcw : 16; //bb_cpu_ldpc_rem_nshrt_ncw        , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcRemNshrtNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW 0x2890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNpuncNcw : 16; //bb_cpu_ldpc_rem_npunc_ncw        , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcRemNpuncNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW 0x2894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcRemNrepNcw : 16; //bb_cpu_ldpc_rem_nrep_ncw         , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcRemNrepNcw_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA 0x2898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNData : 11; //bb_cpu_ldpc_n_data               , reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcNData_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY 0x289C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNParity : 11; //bb_cpu_ldpc_n_parity             , reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcNParity_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT 0x28A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNRepeat : 15; //bb_cpu_ldpc_n_repeat             , reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcNRepeat_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE 0x28A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcCwType : 2; //bb_cpu_ldpc_cw_type              , reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcCwType_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW 0x28A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuLdpcNCw : 16; //bb_cpu_ldpc_n_cw                 , reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuLdpcNCw_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS 0x28AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuTxNumPaddingBits : 26; //bb_cpu_tx_num_padding_bits       , reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuTxNumPaddingBits_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC 0x28B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNPadPostFec : 17; //bb_cpu_n_pad_post_fec            , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNPadPostFec_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES 0x28B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuViterbiNEs : 3; //bb_cpu_viterbi_n_es              , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuViterbiNEs_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_T_PE 0x28B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuTPe : 5; //bb_cpu_t_pe                      , reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuTPe_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT 0x28BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPeDisambiguityResult : 1; //bb_cpu_pe_disambiguity_result    , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPeDisambiguityResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT 0x28C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorResult : 3; //bb_cpu_a_factor_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAFactorResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT 0x28C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAFactorInitResult : 3; //bb_cpu_a_factor_init_result      , reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAFactorInitResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES 0x28C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeCycles : 26; //bb_cpu_air_time_cycles           , reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAirTimeCycles_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT 0x28CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPhyRateResult : 17; //bb_cpu_phy_rate_result           , reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPhyRateResult_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR 0x28D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuAirTimeError : 1; //bb_cpu_air_time_error            , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuAirTimeError_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_CBPS 0x28D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNCbps : 18; //bb_cpu_n_cbps                    , reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNCbps_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_DBPS 0x28D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNDbps : 18; //bb_cpu_n_dbps                    , reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNDbps_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST 0x28DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuNSdLast : 12; //bb_cpu_n_sd_last                 , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuNSdLast_u;

/*REG_B0_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME 0x28E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbCpuPreambleTime : 20; //bb_cpu_preamble_time , reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0PhyTxMacEmuBbCpuPreambleTime_u;

//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00 0x2000 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00_FIELD_EMU_RUN_MASK                                                     0x00000001
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00_FIELD_EMU_TX_DONE_MASK                                                 0x00000002
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00_FIELD_EMU_LOAD_DATA_RAM_MASK                                           0x00000010
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG00_FIELD_EMU_LOAD_DATA_RAM_BUSY_MASK                                      0x00000020
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01 0x2004 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG01_FIELD_EMU_NUM_OF_PACKETS_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02 0x2008 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02_FIELD_EMU_CRC_IN_MODE_MASK                                             0x00000002
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02_FIELD_EMU_CRC_INVERT_MASK                                              0x00000004
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG02_FIELD_EMU_CRC_REVERSE_MASK                                             0x00000008
//========================================
/* REG_PHY_TX_MAC_EMU_ENDLESS_TYPE 0x200C */
#define B0_PHY_TX_MAC_EMU_ENDLESS_TYPE_FIELD_ENDLESS_PACKETS_MASK                                                 0x00000001
#define B0_PHY_TX_MAC_EMU_ENDLESS_TYPE_FIELD_ENDLESS_DATA_MASK                                                    0x00000002
#define B0_PHY_TX_MAC_EMU_ENDLESS_TYPE_FIELD_ENDLESS_LDPC_MASK                                                    0x00000004
//========================================
/* REG_PHY_TX_MAC_EMU_MAC_EMU_MODE 0x2010 */
#define B0_PHY_TX_MAC_EMU_MAC_EMU_MODE_FIELD_MAC_EMU_MODE_MASK                                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_MAIN_STM 0x2014 */
#define B0_PHY_TX_MAC_EMU_EMU_MAIN_STM_FIELD_EMU_MAIN_STM_MASK                                                    0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_DEL_STM 0x2018 */
#define B0_PHY_TX_MAC_EMU_EMU_DEL_STM_FIELD_EMU_DEL_STM_MASK                                                      0x0000001F
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER 0x201C */
#define B0_PHY_TX_MAC_EMU_EMU_PACKET_COUNTER_FIELD_EMU_PACKET_COUNTER_MASK                                        0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME 0x2020 */
#define B0_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME_FIELD_PACKET_END_ON_AIR_TIME_OFDM_MASK                           0x0000FFFF
#define B0_PHY_TX_MAC_EMU_PACKET_END_ON_AIR_TIME_FIELD_PACKET_END_ON_AIR_TIME_11B_MASK                            0xFFFF0000
//========================================
/* REG_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE 0x2024 */
#define B0_PHY_TX_MAC_EMU_HE_SIGA_BEAM_CHANGE_ACTIVE_FIELD_HE_SIGA_BEAM_CHANGE_ACTIVE_MASK                        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A 0x2028 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A_FIELD_EMU_PATTERN_MODE_MASK                                            0x00000003
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A_FIELD_EMU_INS_PRBS_ERR_MASK                                            0x00000010
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0A_FIELD_EMU_INS_ERR_NUM_MASK                                             0x0001FF00
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B 0x202C */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0B_FIELD_EMU_PRBS_TYPE_MASK                                               0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C 0x2030 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0C_FIELD_EMU_PRBS_INIT_MASK                                               0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D 0x2034 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0D_FIELD_EMU_FIXED_DATA_MASK                                              0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E 0x2038 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_EMU_REG0E_FIELD_EMU_IPG_DLY_MASK                                                 0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_ZLD 0x203C */
#define B0_PHY_TX_MAC_EMU_TX_PHY_ZLD_FIELD_TX_PHY_ZLD_MASK                                                        0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_MAC_PHY_IF 0x2040 */
#define B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF_FIELD_TX_PHY_ENABLE_MASK                                                  0x00000001
#define B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF_FIELD_TX_PHY_READY_MASK                                                   0x00000002
#define B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF_FIELD_TX_PHY_VALID_MASK                                                   0x00000008
#define B0_PHY_TX_MAC_EMU_TX_MAC_PHY_IF_FIELD_TX_PHY_BUS_STATE_MASK                                               0x000000F0
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER 0x2044 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_PACKET_TIMER_FIELD_TX_PHY_PACKET_TIMER_MASK                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM 0x2048 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_MAIN_STM_FIELD_TX_PHY_MAIN_STM_MASK                                              0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER 0x204C */
#define B0_PHY_TX_MAC_EMU_TX_PHY_TCR_COUNTER_FIELD_TX_PHY_TCR_COUNTER_MASK                                        0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER 0x2050 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_RCR_COUNTER_FIELD_TX_PHY_RCR_COUNTER_MASK                                        0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER 0x2054 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_SYMBOL_COUNTER_FIELD_TX_PHY_SYMBOL_COUNTER_MASK                                  0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_BF_LOW 0x2058 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_BF_LOW_FIELD_TX_PHY_BF_LOW_MASK                                                  0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH 0x205C */
#define B0_PHY_TX_MAC_EMU_TX_PHY_BF_HIGH_FIELD_TX_PHY_BF_HIGH_MASK                                                0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_SIG_CRC_DEFS 0x2060 */
#define B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS_FIELD_SIG_CRC_TYPE_MASK                                                    0x000000FF
#define B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS_FIELD_SIG_CRC_INIT_MASK                                                    0x0000FF00
#define B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS_FIELD_SIG_CRC_LEN_MASK                                                     0x000F0000
#define B0_PHY_TX_MAC_EMU_SIG_CRC_DEFS_FIELD_SIG_CRC_RESULT_INVERT_MASK                                           0x00100000
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_TIMING 0x2064 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TIMING_FIELD_TCR_COMMON_0_TIMING_MASK                                        0x0000FFFF
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TIMING_FIELD_TCR_COMMON_1_TIMING_MASK                                        0xFFFF0000
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_TIMING 0x2068 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_TIMING_FIELD_TCR_USER_0_TIMING_MASK                                            0x0000FFFF
#define B0_PHY_TX_MAC_EMU_TCR_USER_TIMING_FIELD_TCR_USER_LAST_TIMING_MASK                                         0xFFFF0000
//========================================
/* REG_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING 0x206C */
#define B0_PHY_TX_MAC_EMU_PACKET_START_TR_SWITCHING_FIELD_PACKET_START_TR_SWITCHING_MASK                          0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT 0x2070 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_FIELD_TCR_USER_SELECT_MASK                                              0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE 0x2074 */
#define B0_PHY_TX_MAC_EMU_TX_PHY_MU_TYPE_FIELD_TX_PHY_MU_TYPE_MASK                                                0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_READ_LOGIC_CTRL 0x2078 */
#define B0_PHY_TX_MAC_EMU_READ_LOGIC_CTRL_FIELD_READ_LOGIC_LIMIT_OVR_EN_MASK                                      0x00000001
#define B0_PHY_TX_MAC_EMU_READ_LOGIC_CTRL_FIELD_READ_LOGIC_LIMIT_VAL_MASK                                         0x000001F0
//========================================
/* REG_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC 0x207C */
#define B0_PHY_TX_MAC_EMU_PLL_RATE160_STRB_CNT_SYNC_FIELD_PLL_RATE160_STRB_CNT_SYNC_MASK                          0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0 0x2100 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_0_FIELD_EMU_PHY_TX_PHY_TCR0_0_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1 0x2104 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_1_FIELD_EMU_PHY_TX_PHY_TCR0_1_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2 0x2108 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_2_FIELD_EMU_PHY_TX_PHY_TCR0_2_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3 0x210C */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR0_3_FIELD_EMU_PHY_TX_PHY_TCR0_3_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0 0x2110 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_0_FIELD_EMU_PHY_TX_PHY_TCR1_0_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1 0x2114 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_1_FIELD_EMU_PHY_TX_PHY_TCR1_1_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2 0x2118 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_2_FIELD_EMU_PHY_TX_PHY_TCR1_2_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3 0x211C */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR1_3_FIELD_EMU_PHY_TX_PHY_TCR1_3_MASK                                  0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0 0x2120 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_0_FIELD_EMU_PHY_TX_PHY_TCR_U_0_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1 0x2124 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_1_FIELD_EMU_PHY_TX_PHY_TCR_U_1_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2 0x2128 */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_2_FIELD_EMU_PHY_TX_PHY_TCR_U_2_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3 0x212C */
#define B0_PHY_TX_MAC_EMU_EMU_PHY_TX_PHY_TCR_U_3_FIELD_EMU_PHY_TX_PHY_TCR_U_3_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR0_DATA_0 0x2130 */
#define B0_PHY_TX_MAC_EMU_TCR0_DATA_0_FIELD_TCR0_DATA_0_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR0_DATA_1 0x2134 */
#define B0_PHY_TX_MAC_EMU_TCR0_DATA_1_FIELD_TCR0_DATA_1_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR0_DATA_2 0x2138 */
#define B0_PHY_TX_MAC_EMU_TCR0_DATA_2_FIELD_TCR0_DATA_2_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR0_DATA_3 0x213C */
#define B0_PHY_TX_MAC_EMU_TCR0_DATA_3_FIELD_TCR0_DATA_3_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR1_DATA_0 0x2140 */
#define B0_PHY_TX_MAC_EMU_TCR1_DATA_0_FIELD_TCR1_DATA_0_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR1_DATA_1 0x2144 */
#define B0_PHY_TX_MAC_EMU_TCR1_DATA_1_FIELD_TCR1_DATA_1_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR1_DATA_2 0x2148 */
#define B0_PHY_TX_MAC_EMU_TCR1_DATA_2_FIELD_TCR1_DATA_2_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR1_DATA_3 0x214C */
#define B0_PHY_TX_MAC_EMU_TCR1_DATA_3_FIELD_TCR1_DATA_3_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0 0x2150 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_0_FIELD_TCR_USER_SELECT_DATA_0_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1 0x2154 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_1_FIELD_TCR_USER_SELECT_DATA_1_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2 0x2158 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_2_FIELD_TCR_USER_SELECT_DATA_2_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3 0x215C */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DATA_3_FIELD_TCR_USER_SELECT_DATA_3_MASK                                0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT 0x2160 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_SELECT_FIELD_TCR_COMMON_ANT_SELECT_MASK                                  0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST 0x2164 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_ANT_BOOST_FIELD_TCR_COMMON_ANT_BOOST_MASK                                    0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER 0x2168 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_RF_POWER_FIELD_TCR_COMMON_RF_POWER_MASK                                      0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE 0x216C */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_LOOP_MODE_FIELD_TCR_COMMON_LOOP_MODE_MASK                                    0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_CBW 0x2170 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_CBW_FIELD_TCR_COMMON_CBW_MASK                                                0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE 0x2174 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_PHY_MODE_FIELD_TCR_COMMON_PHY_MODE_MASK                                      0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP 0x2178 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_PUNCTURING_MAP_FIELD_TCR_COMMON_HE_PUNCTURING_MAP_MASK                    0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX 0x217C */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_FORCE_TX_FIELD_TCR_COMMON_FORCE_TX_MASK                                      0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR 0x2180 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_BSS_COLOR_FIELD_TCR_COMMON_BSS_COLOR_MASK                                    0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF 0x2184 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_CUSTOM_BF_FIELD_TCR_COMMON_CUSTOM_BF_MASK                                    0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF 0x2188 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_N_HE_LTF_FIELD_TCR_COMMON_N_HE_LTF_MASK                                      0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL 0x218C */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_UL_DL_FIELD_TCR_COMMON_UL_DL_MASK                                            0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED 0x2190 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_PS_NOT_ALLOWED_FIELD_TCR_COMMON_TXOP_PS_NOT_ALLOWED_MASK                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR 0x2194 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_EXPECT_RCR_FIELD_TCR_COMMON_EXPECT_RCR_MASK                                  0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE 0x2198 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_BW_CHANGE_FIELD_TCR_COMMON_BW_CHANGE_MASK                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC 0x219C */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TXOP_DURATION_FROM_MAC_FIELD_TCR_COMMON_TXOP_DURATION_FROM_MAC_MASK          0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE 0x21A0 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_AGGREGATE_FIELD_TCR_COMMON_AGGREGATE_MASK                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW 0x21A4 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_DYN_BW_FIELD_TCR_COMMON_DYN_BW_MASK                                          0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP 0x21A8 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_PHY_NDP_FIELD_TCR_COMMON_MU_PHY_NDP_MASK                                  0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING 0x21AC */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_MU_TRAINING_FIELD_TCR_COMMON_MU_TRAINING_MASK                                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP 0x21B0 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_CP_FIELD_TCR_COMMON_HE_CP_MASK                                            0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF 0x21B4 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_LTF_FIELD_TCR_COMMON_HE_LTF_MASK                                          0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_SCP 0x21B8 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_SCP_FIELD_TCR_COMMON_SCP_MASK                                                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING 0x21BC */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_SMOOTHING_FIELD_TCR_COMMON_SMOOTHING_MASK                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING 0x21C0 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_NOT_SOUNDING_FIELD_TCR_COMMON_NOT_SOUNDING_MASK                              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_STBC 0x21C4 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_STBC_FIELD_TCR_COMMON_STBC_MASK                                              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE 0x21C8 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_SPATIAL_REUSE_FIELD_TCR_COMMON_SPATIAL_REUSE_MASK                            0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA 0x21CC */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_DATA_FIELD_TCR_COMMON_TRIGGER_BASED_DATA_MASK                  0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID 0x21D0 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_GROUP_ID_FIELD_TCR_COMMON_GROUP_ID_MASK                                      0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE 0x21D4 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_HE_SIGB_RATE_FIELD_TCR_COMMON_HE_SIGB_RATE_MASK                              0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION 0x21D8 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_MAC_DURATION_FIELD_TCR_COMMON_MAC_DURATION_MASK                              0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH 0x21DC */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_LENGTH_FIELD_TCR_USER_SELECT_PSDU_LENGTH_MASK                      0x007FFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND 0x21E0 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_SUB_BAND_FIELD_TCR_USER_SELECT_SUB_BAND_MASK                            0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU 0x21E4 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_START_RU_FIELD_TCR_USER_SELECT_START_RU_MASK                            0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE 0x21E8 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RU_SIZE_FIELD_TCR_USER_SELECT_RU_SIZE_MASK                              0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF 0x21F0 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_TXBF_FIELD_TCR_USER_SELECT_TXBF_MASK                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE 0x21F4 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RATE_FIELD_TCR_USER_SELECT_PSDU_RATE_MASK                          0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER 0x21F8 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_RELATIVE_POWER_FIELD_TCR_USER_SELECT_RELATIVE_POWER_MASK                0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID 0x2200 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_STATION_ID_FIELD_TCR_USER_SELECT_STATION_ID_MASK                        0x000007FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX 0x2204 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_BF_INDEX_FIELD_TCR_USER_SELECT_BF_INDEX_MASK                            0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS 0x2208 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_MCS_FIELD_TCR_USER_SELECT_PSDU_MCS_MASK                            0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK 0x220C */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_RANK_FIELD_TCR_USER_SELECT_PSDU_RANK_MASK                          0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM 0x2210 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_PSDU_DCM_FIELD_TCR_USER_SELECT_PSDU_DCM_MASK                            0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_VALID 0x2214 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_VALID_FIELD_AIR_TIME_VALID_MASK                                                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED 0x2218 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_EXCEED_FIELD_AIR_TIME_COMMON_L_LENGTH_EXCEED_MASK              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM 0x221C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_NUM_SYM_FIELD_AIR_TIME_COMMON_NUM_SYM_MASK                              0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR 0x2220 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_A_FACTOR_FIELD_AIR_TIME_COMMON_A_FACTOR_MASK                            0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE 0x2224 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_T_PE_FIELD_AIR_TIME_COMMON_T_PE_MASK                                    0x0000001F
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES 0x2228 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_CYCLES_FIELD_AIR_TIME_COMMON_AIR_TIME_CYCLES_MASK              0x03FFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME 0x222C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PREAMBLE_TIME_FIELD_AIR_TIME_COMMON_PREAMBLE_TIME_MASK                  0x000FFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL 0x2230 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL_FIELD_AIR_TIME_COMMON_LDPC_EXTRA_SYMBOL_MASK          0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT 0x2234 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_AIR_TIME_RESULT_FIELD_AIR_TIME_COMMON_AIR_TIME_RESULT_MASK              0x0001FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH 0x2238 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_L_LENGTH_FIELD_AIR_TIME_COMMON_L_LENGTH_MASK                            0x00000FFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1 0x223C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_VHT_SIG_A2_B1_FIELD_AIR_TIME_COMMON_VHT_SIG_A2_B1_MASK                  0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY 0x2240 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_COMMON_PE_DISAMBIGUITY_FIELD_AIR_TIME_COMMON_PE_DISAMBIGUITY_MASK              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE 0x2244 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_CODING_RATE_FIELD_AIR_TIME_USER_SELECT_CODING_RATE_MASK            0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION 0x2248 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_MODULATION_FIELD_AIR_TIME_USER_SELECT_MODULATION_MASK              0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW 0x224C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW_FIELD_AIR_TIME_USER_SELECT_LDPC_REM_NSHRT_NCW_MASK 0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW 0x2250 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW_FIELD_AIR_TIME_USER_SELECT_LDPC_REM_NPUNC_NCW_MASK 0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW 0x2254 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW_FIELD_AIR_TIME_USER_SELECT_LDPC_REM_NREP_NCW_MASK 0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA 0x2258 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_DATA_FIELD_AIR_TIME_USER_SELECT_LDPC_N_DATA_MASK            0x000007FF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY 0x225C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_PARITY_FIELD_AIR_TIME_USER_SELECT_LDPC_N_PARITY_MASK        0x000007FF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT 0x2260 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_REPEAT_FIELD_AIR_TIME_USER_SELECT_LDPC_N_REPEAT_MASK        0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE 0x2264 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_CW_TYPE_FIELD_AIR_TIME_USER_SELECT_LDPC_CW_TYPE_MASK          0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW 0x2268 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_LDPC_N_CW_FIELD_AIR_TIME_USER_SELECT_LDPC_N_CW_MASK                0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING 0x226C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_PADDING_FIELD_AIR_TIME_USER_SELECT_PADDING_MASK                    0x03FFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES 0x2270 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_NES_FIELD_AIR_TIME_USER_SELECT_NES_MASK                            0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS 0x2274 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_CBPS_FIELD_AIR_TIME_USER_SELECT_N_CBPS_MASK                      0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS 0x2278 */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_DBPS_FIELD_AIR_TIME_USER_SELECT_N_DBPS_MASK                      0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST 0x227C */
#define B0_PHY_TX_MAC_EMU_AIR_TIME_USER_SELECT_N_SD_LAST_FIELD_AIR_TIME_USER_SELECT_N_SD_LAST_MASK                0x00000FFF
//========================================
/* REG_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM 0x2280 */
#define B0_PHY_TX_MAC_EMU_HE_SIGB_NUM_SYM_FIELD_HE_SIGB_NUM_SYM_MASK                                              0x0000003F
//========================================
/* REG_PHY_TX_MAC_EMU_PHY_MAC_ERROR 0x2284 */
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_TCR0_TIMING_ERROR_MASK                                           0x00000001
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_TCR1_TIMING_ERROR_MASK                                           0x00000002
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_TCR_U_TIMING_ERROR_MASK                                          0x00000004
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_TCR_MU_TIMING_ERROR_MASK                                         0x00000008
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_BF_ERROR_MASK                                                    0x00000010
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_L_LENGTH_EXCEED_ERROR_MASK                                       0x00000020
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_RCR_TIMING_ERROR_MASK                                            0x00000040
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_HE_SIGB_NUM_SYM_ERROR_MASK                                       0x00000080
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_END_OF_AIR_ERROR_MASK                                            0x00000100
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_FIELD_TX_TB_AIR_TIME_ERROR_MASK                                           0x00000200
//========================================
/* REG_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR 0x2288 */
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_CLEAR_FIELD_TX_MAC_ERROR_CLEAR_MASK                                       0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN 0x228C */
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_TCR0_TIMING_ERROR_INT_EN_MASK                             0x00000001
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_TCR1_TIMING_ERROR_INT_EN_MASK                             0x00000002
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_TCR_U_TIMING_ERROR_INT_EN_MASK                            0x00000004
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_TCR_MU_TIMING_ERROR_INT_EN_MASK                           0x00000008
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_BF_ERROR_INT_EN_MASK                                      0x00000010
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_L_LENGTH_EXCEED_ERROR_INT_EN_MASK                         0x00000020
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_RCR_TIMING_ERROR_INT_EN_MASK                              0x00000040
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_HE_SIGB_NUM_SYM_ERROR_INT_EN_MASK                         0x00000080
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_END_OF_AIR_ERROR_INT_EN_MASK                              0x00000100
#define B0_PHY_TX_MAC_EMU_PHY_MAC_ERROR_INT_EN_FIELD_TX_TB_AIR_TIME_ERROR_INT_EN_MASK                             0x00000200
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE 0x2290 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_DPD_CALIBRATE_FIELD_TCR_COMMON_DPD_CALIBRATE_MASK                            0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2 0x2294 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_TRIGGER_BASED_SIGA2_FIELD_TCR_COMMON_TRIGGER_BASED_SIGA2_MASK                0x000001FF
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT 0x2298 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_REUSE_SCRAMBLER_INIT_FIELD_TCR_COMMON_REUSE_SCRAMBLER_INIT_MASK              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_BEACON 0x229C */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_BEACON_FIELD_TCR_COMMON_BEACON_MASK                                          0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED 0x22A0 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_COMPRESSED_FIELD_TCR_USER_SELECT_HE_SIGB_COMPRESSED_MASK        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS 0x22A4 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS_FIELD_TCR_USER_SELECT_HE_SIGB_16_SYMBOLS_MASK        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS 0x22A8 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_HE_TB_STARTING_STS_FIELD_TCR_USER_SELECT_HE_TB_STARTING_STS_MASK        0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX 0x22AC */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX_FIELD_TCR_USER_SELECT_NDP_FB_RU_TONE_SET_INDEX_MASK 0x0000001F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS 0x22B0 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_STARTING_STS_FIELD_TCR_USER_SELECT_NDP_FB_STARTING_STS_MASK      0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE 0x22B4 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_NDP_FB_VALUE_FIELD_TCR_USER_SELECT_NDP_FB_VALUE_MASK                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_LDPC 0x22B8 */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_LDPC_FIELD_TCR_COMMON_LDPC_MASK                                              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION 0x22BC */
#define B0_PHY_TX_MAC_EMU_TCR_COMMON_PACKET_EXTENSION_FIELD_TCR_COMMON_PACKET_EXTENSION_MASK                      0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU 0x22C0 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_MRU_FIELD_TCR_USER_SELECT_MRU_MASK                                      0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP 0x22C4 */
#define B0_PHY_TX_MAC_EMU_TCR_USER_SELECT_DUP_FIELD_TCR_USER_SELECT_DUP_MASK                                      0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_USER_FIRST_DATA_0 0x22F0 */
#define B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_0_FIELD_USER_FIRST_DATA_0_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_USER_FIRST_DATA_1 0x22F4 */
#define B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_1_FIELD_USER_FIRST_DATA_1_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_USER_FIRST_DATA_2 0x22F8 */
#define B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_2_FIELD_USER_FIRST_DATA_2_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_USER_FIRST_DATA_3 0x22FC */
#define B0_PHY_TX_MAC_EMU_USER_FIRST_DATA_3_FIELD_USER_FIRST_DATA_3_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START 0x2800 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_START_FIELD_BB_CPU_AIR_TIME_CALC_START_MASK                        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC 0x2804 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_FIELD_BB_CPU_LDPC_MASK                                                      0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_SCP 0x2808 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_SCP_FIELD_BB_CPU_SCP_MASK                                                        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_STBC 0x280C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_STBC_FIELD_BB_CPU_STBC_MASK                                                      0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE 0x2810 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PSDU_RATE_FIELD_BB_CPU_PSDU_RATE_MASK                                            0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_BW 0x2814 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_BW_FIELD_BB_CPU_BW_MASK                                                          0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE 0x2818 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_MODE_FIELD_BB_CPU_PACKET_MODE_MASK                                        0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_NESS 0x281C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_NESS_FIELD_BB_CPU_NESS_MASK                                                      0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH 0x2820 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PACKET_LENGTH_FIELD_BB_CPU_PACKET_LENGTH_MASK                                    0x007FFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR 0x2824 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_RX_TX_N_SELECTOR_FIELD_BB_CPU_RX_TX_N_SELECTOR_MASK                              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH 0x2828 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_RX_L_LENGTH_FIELD_BB_CPU_RX_L_LENGTH_MASK                                        0x00000FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1 0x282C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_RX_VHT_SIG_A2_B1_FIELD_BB_CPU_RX_VHT_SIG_A2_B1_MASK                              0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL 0x2830 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_RX_LDPC_EXTRA_SYMBOL_FIELD_BB_CPU_RX_LDPC_EXTRA_SYMBOL_MASK                      0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE 0x2834 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_MU_STAGE_FIELD_BB_CPU_MU_STAGE_MASK                                              0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT 0x2838 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM_INIT_FIELD_BB_CPU_N_SYM_INIT_MASK                                          0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_SYM 0x283C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_SYM_FIELD_BB_CPU_N_SYM_MASK                                                    0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL 0x2840 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_LTF_TOTAL_FIELD_BB_CPU_N_LTF_TOTAL_MASK                                        0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE 0x2844 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_HE_RU_SIZE_FIELD_BB_CPU_HE_RU_SIZE_MASK                                          0x0000000F
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM 0x2848 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_SIGB_SYM_FIELD_BB_CPU_N_SIGB_SYM_MASK                                          0x000000FF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_HE_CP 0x284C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_HE_CP_FIELD_BB_CPU_HE_CP_MASK                                                    0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_HE_LTF 0x2850 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_HE_LTF_FIELD_BB_CPU_HE_LTF_MASK                                                  0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY 0x2854 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_FIELD_BB_CPU_PE_DISAMBIGUITY_MASK                                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR 0x2858 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_FIELD_BB_CPU_A_FACTOR_MASK                                              0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT 0x285C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_FIELD_BB_CPU_A_FACTOR_INIT_MASK                                    0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_MAX_PE 0x2860 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_MAX_PE_FIELD_BB_CPU_MAX_PE_MASK                                                  0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID 0x2864 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CALC_VALID_FIELD_BB_CPU_AIR_TIME_CALC_VALID_MASK                        0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED 0x2868 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_EXCEED_FIELD_BB_CPU_L_LENGTH_EXCEED_MASK                                0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1 0x286C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_VHT_SIG_A2_B1_FIELD_BB_CPU_VHT_SIG_A2_B1_MASK                                    0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL 0x2870 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_EXTRA_SYMBOL_FIELD_BB_CPU_LDPC_EXTRA_SYMBOL_MASK                            0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE 0x2874 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_CODING_RATE_FIELD_BB_CPU_CODING_RATE_MASK                                        0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_MODULATION 0x2878 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_MODULATION_FIELD_BB_CPU_MODULATION_MASK                                          0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT 0x287C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_RESULT_FIELD_BB_CPU_AIR_TIME_RESULT_MASK                                0x0001FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT 0x2880 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_L_LENGTH_RESULT_FIELD_BB_CPU_L_LENGTH_RESULT_MASK                                0x00000FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT 0x2884 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_RESULT_FIELD_BB_CPU_NUM_SYM_RESULT_MASK                                  0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT 0x2888 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_NUM_SYM_INIT_RESULT_FIELD_BB_CPU_NUM_SYM_INIT_RESULT_MASK                        0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW 0x288C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NSHRT_NCW_FIELD_BB_CPU_LDPC_REM_NSHRT_NCW_MASK                          0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW 0x2890 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NPUNC_NCW_FIELD_BB_CPU_LDPC_REM_NPUNC_NCW_MASK                          0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW 0x2894 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_REM_NREP_NCW_FIELD_BB_CPU_LDPC_REM_NREP_NCW_MASK                            0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA 0x2898 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_DATA_FIELD_BB_CPU_LDPC_N_DATA_MASK                                        0x000007FF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY 0x289C */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_PARITY_FIELD_BB_CPU_LDPC_N_PARITY_MASK                                    0x000007FF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT 0x28A0 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_REPEAT_FIELD_BB_CPU_LDPC_N_REPEAT_MASK                                    0x00007FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE 0x28A4 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_CW_TYPE_FIELD_BB_CPU_LDPC_CW_TYPE_MASK                                      0x00000003
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW 0x28A8 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_LDPC_N_CW_FIELD_BB_CPU_LDPC_N_CW_MASK                                            0x0000FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS 0x28AC */
#define B0_PHY_TX_MAC_EMU_BB_CPU_TX_NUM_PADDING_BITS_FIELD_BB_CPU_TX_NUM_PADDING_BITS_MASK                        0x03FFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC 0x28B0 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_PAD_POST_FEC_FIELD_BB_CPU_N_PAD_POST_FEC_MASK                                  0x0001FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES 0x28B4 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_VITERBI_N_ES_FIELD_BB_CPU_VITERBI_N_ES_MASK                                      0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_T_PE 0x28B8 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_T_PE_FIELD_BB_CPU_T_PE_MASK                                                      0x0000001F
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT 0x28BC */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PE_DISAMBIGUITY_RESULT_FIELD_BB_CPU_PE_DISAMBIGUITY_RESULT_MASK                  0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT 0x28C0 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_RESULT_FIELD_BB_CPU_A_FACTOR_RESULT_MASK                                0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT 0x28C4 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_A_FACTOR_INIT_RESULT_FIELD_BB_CPU_A_FACTOR_INIT_RESULT_MASK                      0x00000007
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES 0x28C8 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_CYCLES_FIELD_BB_CPU_AIR_TIME_CYCLES_MASK                                0x03FFFFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT 0x28CC */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PHY_RATE_RESULT_FIELD_BB_CPU_PHY_RATE_RESULT_MASK                                0x0001FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR 0x28D0 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_AIR_TIME_ERROR_FIELD_BB_CPU_AIR_TIME_ERROR_MASK                                  0x00000001
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_CBPS 0x28D4 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_CBPS_FIELD_BB_CPU_N_CBPS_MASK                                                  0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_DBPS 0x28D8 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_DBPS_FIELD_BB_CPU_N_DBPS_MASK                                                  0x0003FFFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST 0x28DC */
#define B0_PHY_TX_MAC_EMU_BB_CPU_N_SD_LAST_FIELD_BB_CPU_N_SD_LAST_MASK                                            0x00000FFF
//========================================
/* REG_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME 0x28E0 */
#define B0_PHY_TX_MAC_EMU_BB_CPU_PREAMBLE_TIME_FIELD_BB_CPU_PREAMBLE_TIME_MASK                                    0x000FFFFF


#endif // _PHY_TX_MAC_EMU_REGS_H_
