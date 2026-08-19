
/***********************************************************************************
File:				DfsPhyRxTdIfRegs.h
Module:				dfsPhyRxTdIf
SOC Revision:		latest
Generated at:       2024-06-26 12:55:24
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _DFS_PHY_RX_TD_IF_REGS_H_
#define _DFS_PHY_RX_TD_IF_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define DFS_PHY_RX_TD_IF_BASE_ADDRESS                        MEMORY_MAP_UNIT_207_BASE_ADDRESS
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF00                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1000)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF01                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1004)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF02                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1008)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF03                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x100C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF04                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1010)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF05                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1014)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF06                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1018)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF07                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x101C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF08                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1020)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF09                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1024)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1B                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x106C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1C                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1070)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1074)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1078)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x107C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_ERROR_INT            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1080)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF21                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1084)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF22                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1088)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF23                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x108C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF24                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1090)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF25                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1094)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF26                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1098)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF28                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10A0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF29                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10A4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10A8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2B                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10AC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2C                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10B0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10B4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10B8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10BC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF30                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10C0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF31                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10C4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10C8)
#define	REG_DFS_PHY_RX_TD_IF_CCA20P                                 (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10CC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF34                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10D0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF3D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x10F4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1128)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1130)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1134)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CCA_INT                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1138)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x113C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF56                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1158)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF5F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x117C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x119C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11A8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6B                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11AC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11B4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6E                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11B8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11BC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF70                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11C0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF71                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11C4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF72                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11C8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF73                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11CC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF74                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11D0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF75                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11D4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF76                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11D8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF77                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11DC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF78                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11E0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF79                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11E4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11E8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_TIMERS_INT           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11EC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7C                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11F0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7D                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11F4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7E                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11F8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x11FC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1200)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SANITY_DELTA_F_VALID       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1208)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1210)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1214)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1218)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF87                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x121C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF88                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1220)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH      (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1224)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1228)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8B                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x122C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8F                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x123C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF90                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1240)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF91                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1244)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SW_RESET_GENERATE          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1248)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF93                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x124C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF94                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1250)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF95                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1254)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF96                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1258)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF97                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x125C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF98                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1260)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF99                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1264)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9A                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1268)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9B                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x126C)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_0                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x12D4)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_2                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x12D8)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_4                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x12DC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER0                     (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1320)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFC9                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1324)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER1                     (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1328)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFCB                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x132C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW_ONLY16       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1330)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID_ONLY16       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1334)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH_ONLY16      (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1338)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR_ONLY16      (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x133C)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_6                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1340)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_0                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1344)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_2                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1348)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF0                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x13C0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF1                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x13C4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF2                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x13C8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFB                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x13EC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFC                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x13F0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF102                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1408)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF103                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x140C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CSM_CLEAR_ERROR_INT        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1410)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10D                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1434)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10E                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1438)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF120                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1480)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF121                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1484)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF122                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1488)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x148C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF124                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1490)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF125                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1494)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CO_CH_COUNTER              (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x14E8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF13B                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x14EC)
#define	REG_DFS_PHY_RX_TD_IF_RSSI_0_DATA                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x14F8)
#define	REG_DFS_PHY_RX_TD_IF_RSSI_1_DATA                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x14FC)
#define	REG_DFS_PHY_RX_TD_IF_RSSI_2_DATA                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1500)
#define	REG_DFS_PHY_RX_TD_IF_RSSI_3_DATA                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1504)
#define	REG_DFS_PHY_RX_TD_IF_RSSI_4_DATA                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1508)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF143                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x150C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW              (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1510)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF145                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1514)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID              (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1518)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF147                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x151C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH             (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1520)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF149                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1524)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR             (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1528)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14B                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x152C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14C                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1530)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14D                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1534)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14E                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1538)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14F                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x153C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF150                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1540)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF151                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1544)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF152                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1548)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF153                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x154C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF154                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1550)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF156                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1558)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF158                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1560)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF15F                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x157C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF160                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1580)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF161                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1584)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF163                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x158C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF164                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1590)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_80                             (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1594)
#define	REG_DFS_PHY_RX_TD_IF_CCA_PWR_160                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1598)
#define	REG_DFS_PHY_RX_TD_IF_CCA80S_PIFS                            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x159C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF168                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15A0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF169                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15A4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16A                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15A8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH_2    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15AC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16C                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15B0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16D                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15B4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16E                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15B8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16F                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15BC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15C0)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF171                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15C4)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x15C8)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF175                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1604)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF176                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1608)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF177                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x160C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF178                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1610)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF179                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1614)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17A                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1618)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17B                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x161C)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17C                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1620)
#define	REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17D                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1624)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN01                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1628)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN23                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x162C)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN45                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1630)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI0                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1638)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI1                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x163C)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI2                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1640)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI3                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1644)
#define	REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI4                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1648)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CLK_CENTRAL                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x164C)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CLK_BIAS                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1650)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CONTROLS                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1654)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1658)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_WR_DATA                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x165C)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RD_DATA                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1660)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_BUSY                      (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1664)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_DEBUG                     (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1668)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS                        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1680)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_ANT_BUSY                          (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1684)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RESET_N                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1688)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_FORCE_DEFAULT             (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x168C)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_RESET_N                      (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1698)
#define	REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_FORCE_DEFAULT                (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x169C)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_WR_CAUSE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16A0)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_WR_CAUSE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16A4)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_WR_CAUSE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16A8)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_WR_CAUSE                  (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16AC)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT                        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16B0)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT                        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16B4)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT                        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16B8)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16BC)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT_TYPE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16C0)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT_TYPE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16C4)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT_TYPE                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16C8)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT_TYPE                  (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16CC)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_PENDING                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16D0)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_PENDING                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16D4)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_PENDING                    (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16D8)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_PENDING                   (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16DC)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_FROM_2G_RD_CAUSE                 (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16E0)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_FROM_5G_RD_CAUSE                 (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16E4)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_FROM_6G_RD_CAUSE                 (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16E8)
#define	REG_DFS_PHY_RX_TD_IF_GRISC_FROM_DFS_RD_CAUSE                (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16EC)
#define	REG_DFS_PHY_RX_TD_IF_PHY_MAC_GPIO_BAND_ACTIVE               (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16F0)
#define	REG_DFS_PHY_RX_TD_IF_DISABLE_LAST_EHT_SIG_FREQ_TRACK        (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16F4)
#define	REG_DFS_PHY_RX_TD_IF_IMPULSE_RESPONSE_IIR_OFFSET            (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16F8)
#define	REG_DFS_PHY_RX_TD_IF_CCA20SB_PIFS                           (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x16FC)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_0                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1700)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_2                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1704)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_4                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1708)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_6                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x170C)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_0                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1710)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_2                       (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1714)
#define	REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_80                         (DFS_PHY_RX_TD_IF_BASE_ADDRESS + 0x1718)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF00 0x1000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscGpiLow_riscGpoLow : 16; // IF Semaphore reg00, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf00_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF01 0x1004 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscGpiHigh_riscGpoHigh : 16; // IF Semaphore reg01, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf01_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF02 0x1008 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscGpioOenLow : 16; // IF Semaphore reg02, reset value: 0xffff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf02_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF03 0x100C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscGpioOenHigh : 16; // IF Semaphore reg03, reset value: 0xffff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf03_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF04 0x1010 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg04 : 16; //GenRisc operational mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf04_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF05 0x1014 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg05 : 16; // IF Semaphore reg05, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf05_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF06 0x1018 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg06 : 16; // IF Semaphore reg06, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf06_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF07 0x101C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg07 : 16; // IF Semaphore reg07, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf07_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF08 0x1020 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg08 : 16; // IF Semaphore reg08, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf08_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF09 0x1024 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 semaphoreReg09 : 16; // IF Semaphore reg09, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf09_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1B 0x106C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdEnableOverride : 1; //td enable override, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf1B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1C 0x1070 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscMaskInt : 11; //Gen risc Mask Interrupt, reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf1C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D 0x1074 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscClearInt0 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt1 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt2 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt3 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt4 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt5 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt6 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt7 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt8 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt9 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 genRiscClearInt10 : 1; //Gen risc Clear Int, reset value: 0x0, access type: WO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf1D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E 0x1078 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscNoiseCauseInt0 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscTxEnStartCauseInt1 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscPreTxCauseInt2 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscRxLatchFallingRxRdyCauseInt3 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscErrorCauseInt4 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscFdLastSymbolCauseInt5 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscHostCauseInt6 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscHostCauseInt7 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscHostCauseInt8 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscHostCauseInt9 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 genRiscHostCauseInt10 : 1; //Gen risc Cause Int, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf1E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1F 0x107C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableErrorInt : 16; //Gen Risc Enable Error Int, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf1F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_ERROR_INT 0x1080 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearErrorInt : 16; //Gen Risc Clear Enable Error Int, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfClearErrorInt_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF21 0x1084 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxRdyFallDelayIf : 13; //rx_rdy_fall_delay_if, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 rxRdyFallDelayIfEn : 1; //rx_rdy_fall_delay_if_en, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf21_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF22 0x1088 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntLowRt : 16; //Free Running Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf22_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF23 0x108C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ofdmEnableBit : 1; //ofdm enable bit for csm, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf23_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF24 0x1090 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csmAntennaEnable : 5; //csm_antenna_enable, reset value: 0x1F, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf24_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF25 0x1094 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 antennaDeletion : 5; //antenna_deletion, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf25_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF26 0x1098 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolPramReadyStm : 2; //symbol_pram_ready_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 deltaFStm : 2; //delta_f_stm, reset value: 0x0, access type: RO
		uint32 reserved1 : 26;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf26_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF28 0x10A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stopSignalThIf : 13; //stop_signal_th_if, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 stopSignalThIfEn : 1; //stop_signal_th_if_en, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf28_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF29 0x10A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscInt6 : 16; //Rx TD Risc int 6, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf29_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2A 0x10A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 tdCsmState : 3; //td_csm_state, reset value: 0x0, access type: RO
		uint32 reserved1 : 17;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2B 0x10AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscAirtimeUs : 16; //gen_risc_airtime_us, reset value: 0xfff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2C 0x10B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRisc4UsBoundaryOffset : 12; //gen_risc_4us_boundary_offset, reset value: 0x7ff, access type: RW
		uint32 ccaCntSrcCtrl : 4; //cca_cnt_src_ctrl (0 - gen risc, 1- hw), reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D 0x10B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htShortInd : 1; //ht_short_ind, reset value: 0x0, access type: RO
		uint32 ccaCs : 1; //cca_cs, reset value: 0x0, access type: RO
		uint32 heShortInd : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E 0x10B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 packetType : 1; //packet_type, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 isHtSw : 1; //is_ht, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 isHtSel : 1; //select HW ht of sw ht, reset value: 0x0, access type: RW
		uint32 genriscRdyEnable : 1; //select HW ht of sw ht, reset value: 0x0, access type: RW
		uint32 reserved2 : 23;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2F 0x10BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcRemovalShifter : 4; //DC removal shifter, reset value: 0x0, access type: RW
		uint32 dcRemovalShifterGearShift : 4; //DC removal shifter for gear shift, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf2F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF30 0x10C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscAirtimeUsMax : 1; //gen_risc_airtime_us_max, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf30_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF31 0x10C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscAirtimeUsOverride : 1; //gen_risc_airtime_us_max, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf31_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32 0x10C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaBusyGenRisc : 1; //cca_busy_gen_risc, reset value: 0x0, access type: RW
		uint32 ccaModeCtrl : 1; //cca_mode_ctrl, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 ccaBusyGenRiscSec : 1; //cca_busy_gen_risc_sec, reset value: 0x0, access type: RW
		uint32 ccaModeCtrlSec : 1; //cca_mode_ctrl_sec, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 ccaBusyGenRiscSec40 : 1; //cca_busy_gen_risc_sec_40, reset value: 0x0, access type: RW
		uint32 ccaModeCtrlSec40 : 1; //cca_mode_ctrl_sec_40, reset value: 0x0, access type: RW
		uint32 reserved2 : 22;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf32_u;

/*REG_DFS_PHY_RX_TD_IF_CCA20P 0x10CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cca20P : 1; //cca20p, reset value: 0x1, access type: RO
		uint32 cca20S : 1; //cca20s, reset value: 0x1, access type: RO
		uint32 cca40S : 1; //cca40s, reset value: 0x1, access type: RO
		uint32 cca20SPifs : 1; //cca20s_pifs, reset value: 0x1, access type: RO
		uint32 cca40SPifs : 1; //cca40s_pifs, reset value: 0x1, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfCca20P_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF34 0x10D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorCauseReg : 16; //Rx TD Risc Error Cause Reg, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf34_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF3D 0x10F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscTestBusEn : 8; //gen_risc_test_bus_en, reset value: 0x0, access type: RW
		uint32 genRiscTestBusEn1 : 8; //gen_risc_test_bus_en_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf3D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A 0x1128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoFull : 1; //test_fifo_full, reset value: 0x0, access type: RO
		uint32 testFifoFull1 : 1; //test_fifo_full_1, reset value: 0x0, access type: RO
		uint32 tbFifoError : 1; //tb fifo error, reset value: 0x0, access type: RO
		uint32 tbFifoError1 : 1; //tb fifo error_1, reset value: 0x0, access type: RO
		uint32 tbFifoErrorAbsorption : 8; //tb_fifo_error_absorption, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf4A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C 0x1130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorBypassEn : 1; //detector_bypass_en, reset value: 0x0, access type: RW
		uint32 detectorBypassResetRxtdEn : 1; //detector_bypass_reset_rxtd_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 detectorBypassCntValue : 12; //detector_bypass_cnt_value, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf4C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4D 0x1134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableCcaInt : 4; //enable_cca_int: [0]-last sample, [1]-ht_short,[2]-he_short,[3]-agc_event, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf4D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CCA_INT 0x1138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearCcaInt : 4; //clear_cca_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfCcaInt_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4F 0x113C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaCauseReg : 4; //cca_cause_reg, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf4F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF56 0x1158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstRiscAllValid : 1; //Noise_est_risc_all_valid, reset value: 0x0, access type: RW
		uint32 noiseEstRiscSelect : 1; //Noise_est_risc_select, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf56_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF5F 0x117C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detShortenedMa : 7; //Rx TD  Shortened moving average value, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 waitCycles : 8; //Rx TD  wait cycles , reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf5F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67 0x119C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wdCounterEn : 1; //Watch dog counter enble, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 clearWdCounter : 1; //Clear Watch dog counter enble, reset value: 0x0, access type: WO
		uint32 reserved1 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf67_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6A 0x11A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detNoiseEstValidEn : 1; //det_noise_est_valid_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf6A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6B 0x11AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityDetRawDetection : 8; //Sanity Detector Raw Detection, reset value: 0x0, access type: RO
		uint32 sanityDetTbRawDetection : 8; //Sanity Detector Raw Detection for Trigger-Based, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf6B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D 0x11B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityDetCoarseRdyAn0 : 1; //Sanity Detector Coarse Ready An0, reset value: 0x0, access type: RO
		uint32 sanityDetCoarseRdyAn1 : 1; //Sanity Detector Coarse Ready An1, reset value: 0x0, access type: RO
		uint32 sanityDetCoarseRdyAn2 : 1; //Sanity Detector Coarse Ready An2, reset value: 0x0, access type: RO
		uint32 sanityDetCoarseRdyAn3 : 1; //Sanity Detector Coarse Ready An3, reset value: 0x0, access type: RO
		uint32 sanityDetCommonCoarseErr : 1; //Sanity_det_common_coarse_err, reset value: 0x0, access type: RO
		uint32 sanityDetCommonFineErr : 1; //Sanity_det_common_fine_err, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf6D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6E 0x11B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityHostAntennaEn : 5; //sanity Host Antenna Enable, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf6E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6F 0x11BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn0CoarseCnt : 16; //Sanity An 0 Coarse Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf6F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF70 0x11C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn1CoarseCnt : 16; //Sanity An 1 Coarse Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf70_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF71 0x11C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn2CoarseCnt : 16; //Sanity An 2 Coarse Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf71_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF72 0x11C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn3CoarseCnt : 16; //Sanity An 3 Coarse Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf72_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF73 0x11CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn0FineCnt : 16; //Sanity An 0 Fine Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf73_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF74 0x11D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn1FineCnt : 16; //Sanity An 1 Fine Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf74_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF75 0x11D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn2FineCnt : 16; //Sanity An 2 Fine Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf75_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF76 0x11D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn3FineCnt : 16; //Sanity An 3 Fine Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf76_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF77 0x11DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityClkPerStrobe : 5; //Sanity Clock Per Strobe, reset value: 0xf, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf77_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF78 0x11E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cntReset : 1; //Sanity reset the Sanity Counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf78_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF79 0x11E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 commonCoarseRdyCnt : 16; //Sanity Detector Coarse Max Abs Sc An0, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf79_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7A 0x11E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableTimersInt : 2; //enable_timers_int, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf7A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_TIMERS_INT 0x11EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearTimersInt : 2; //clear_timers_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfClearTimersInt_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7C 0x11F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaTimersReg : 2; //cca_timers_reg, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf7C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7D 0x11F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 commonFineRdyCnt : 16; //common_fine_rdy_cnt, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf7D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7E 0x11F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxtdCsmNcoFreqData : 15; //Sanity Delta f value at the end of the packet, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf7E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F 0x11FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityDetFineRdyAn0 : 1; //Sanity Detector Fine Ready An0, reset value: 0x0, access type: RO
		uint32 sanityDetFineRdyAn1 : 1; //Sanity Detector Fine Ready An1, reset value: 0x0, access type: RO
		uint32 sanityDetFineRdyAn2 : 1; //Sanity Detector Fine Ready An2, reset value: 0x0, access type: RO
		uint32 sanityDetFineRdyAn3 : 1; //Sanity Detector Fine Ready An3, reset value: 0x0, access type: RO
		uint32 sanityDetCommonFineRdy : 1; //sanity_det_common_fine_rdy, reset value: 0x0, access type: RO
		uint32 sanityDetCommonCoarseRdy : 1; //sanity_det_common_coarse_rdy, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf7F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80 0x1200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityDetDetectorBw : 2; //Sanity Detector Top Is Full CB, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 sanityDetImdDetect : 1; //Sanity Detector Top Signal Detect, reset value: 0x0, access type: RO
		uint32 sanityDetSignalDetect : 1; //Sanity Detector Top Signal Detect, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 sanityDetDetectorMask : 8; //Sanity Detector Top Is Full CB, reset value: 0x0, access type: RO
		uint32 reserved2 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf80_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SANITY_DELTA_F_VALID 0x1208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityDeltaFValid : 1; //sanity Delta F Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfSanityDeltaFValid_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84 0x1210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacIntMask : 1; //phy_mac_int_mask, reset value: 0x0, access type: RW
		uint32 txPacketIntMask : 1; //tx_packet_int_mask, reset value: 0x0, access type: RW
		uint32 wdIntMask : 1; //wd_int_mask, reset value: 0x0, access type: RW
		uint32 dfsIntMask : 1; //dfs_int_mask, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf84_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85 0x1214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearPhyMacInt : 1; //clear_phy_mac_int, reset value: 0x0, access type: WO
		uint32 clearTxPacketInt : 1; //clear_tx_l_length_int, reset value: 0x0, access type: WO
		uint32 clearWdInt : 1; //clear_wd_int, reset value: 0x0, access type: WO
		uint32 clearDfsInt : 1; //clear_dfs_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf85_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86 0x1218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacIntCauseReg : 1; //phy_mac_int_cause_reg, reset value: 0x0, access type: RO
		uint32 txPacketIntCauseReg : 1; //tx_packet_int_cause_reg, reset value: 0x0, access type: RO
		uint32 wdIntCauseReg : 1; //wd_int_cause_reg, reset value: 0x0, access type: RO
		uint32 dfsIntCauseReg : 1; //dfs_int_cause_reg, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf86_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF87 0x121C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacInt : 1; //phy_mac_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf87_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF88 0x1220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntLow : 16; //free_running_cnt_low, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf88_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH 0x1224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntHigh : 16; //free_running_cnt_high, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfFreeRunningCntHigh_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8A 0x1228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntThLow : 16; //free_running_cnt_th_low, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf8A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8B 0x122C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntThHigh : 16; //free_running_cnt_th_high, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf8B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8F 0x123C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg : 16; //Rx TD Sub blocks SW Reset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf8F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF90 0x1240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 blockEn : 16; //Rx TD Sub blocks Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf90_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF91 0x1244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmSwResetMask : 16; //Rx TD Gsm Sw Reset., reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf91_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SW_RESET_GENERATE 0x1248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetGenerate : 16; //sw_reset_generate, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfSwResetGenerate_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF93 0x124C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataIAn0 : 13; //grisc_dc_removal_data_i_an0, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf93_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF94 0x1250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataQAn0 : 13; //grisc_dc_removal_data_q_an0, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf94_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF95 0x1254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataIAn1 : 13; //grisc_dc_removal_data_i_an1, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf95_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF96 0x1258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataQAn1 : 13; //grisc_dc_removal_data_q_an1, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf96_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF97 0x125C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataIAn2 : 13; //grisc_dc_removal_data_i_an2, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf97_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF98 0x1260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataQAn2 : 13; //grisc_dc_removal_data_q_an2, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf98_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF99 0x1264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataIAn3 : 13; //grisc_dc_removal_data_i_an3, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf99_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9A 0x1268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataQAn3 : 13; //grisc_dc_removal_data_q_an3, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf9A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9B 0x126C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalValid : 1; //grisc_dc_removal_valid, reset value: 0x0, access type: WO
		uint32 reserved0 : 3;
		uint32 griscDcShiftValueOverride : 1; //grisc_dc_shift_value_override, reset value: 0x0, access type: WO
		uint32 reserved1 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf9B_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_0 0x12D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr200 : 8; //cca_pwr_20_0, reset value: 0x0, access type: RO
		uint32 ccaPwr201 : 8; //cca_pwr_20_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr200_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_2 0x12D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr202 : 8; //cca_pwr_20_2, reset value: 0x0, access type: RO
		uint32 ccaPwr203 : 8; //cca_pwr_20_3, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr202_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_4 0x12DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr204 : 8; //cca_pwr_20_4, reset value: 0x0, access type: RO
		uint32 ccaPwr205 : 8; //cca_pwr_20_5, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr204_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER0 0x1320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timer0Th_timer0 : 16; //timer0, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTimer0_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFC9 0x1324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timer0StbTh : 4; //timer0_stb_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfc9_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER1 0x1328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timer1Th_timer1 : 16; //timer1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTimer1_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFCB 0x132C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timer1StbTh : 4; //timer1_stb_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfcb_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW_ONLY16 0x1330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugLowOnly16 : 16; //Rx TD Risc Test Bus 16 Low, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugLowOnly16_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID_ONLY16 0x1334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugMidOnly16 : 16; //Rx TD Risc Test Bus 16 mid, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugMidOnly16_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH_ONLY16 0x1338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugHighOnly16 : 16; //Rx TD Risc Test Bus 16 high, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugHighOnly16_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR_ONLY16 0x133C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugFourOnly16 : 16; //Rx TD Risc Test Bus 16 four, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugFourOnly16_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_6 0x1340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr206 : 8; //cca_pwr_20_6, reset value: 0x0, access type: RO
		uint32 ccaPwr207 : 8; //cca_pwr_20_7, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr206_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_0 0x1344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr400 : 8; //cca_pwr_40_0, reset value: 0x0, access type: RO
		uint32 ccaPwr401 : 8; //cca_pwr_40_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr400_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_2 0x1348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr402 : 8; //cca_pwr_40_2, reset value: 0x0, access type: RO
		uint32 ccaPwr403 : 8; //cca_pwr_40_3, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr402_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF0 0x13C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accumulatorResetValueI : 12; //accumulator_reset_value_I, reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIff0_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF1 0x13C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accumulatorResetValueQ : 12; //accumulator_reset_value_Q, reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIff1_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF2 0x13C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 resetValuesIq : 1; //reset_values_IQ, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIff2_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFB 0x13EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxtdGprLow : 16; //gpr low, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIffb_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFC 0x13F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxtdGprHigh : 16; //gpr high, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIffc_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF102 0x1408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csmErrorCauseReg : 5; //Rx TD Risc Error Cause Reg, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf102_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF103 0x140C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csmEnableErrorInt : 5; //Gen Risc Enable Error Int, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf103_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CSM_CLEAR_ERROR_INT 0x1410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csmClearErrorInt : 5; //Gen Risc Clear Enable Error Int, reset value: 0x0, access type: WO
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfCsmClearErrorInt_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10D 0x1434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortCalcGo : 1; //short_calc_go, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf10D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10E 0x1438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityRiscAntennaEn : 5; //Sanity Risc Antenna Enable, reset value: 0x0, access type: RW
		uint32 sanityRiscAntennaEnValid : 1; //Sanity Risc Antenna Enable Valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf10E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF120 0x1480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearPifsMem : 1; //clear_pifs_mem, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf120_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF121 0x1484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaClearPrim20Genrisc : 1; //cca_clear_prim20_genrisc, reset value: 0x0, access type: RW
		uint32 ccaClearPrim20Enable : 1; //cca_clear_prim20_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf121_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF122 0x1488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr20Max : 8; //cca_pwr_20_max, reset value: 0x0, access type: RO
		uint32 ccaSecPwr20Max : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf122_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123 0x148C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timingDiffTh : 7; //timing_diff_th , reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 useVerifiedDet : 1; //use_verified_det , reset value: 0x1, access type: RW
		uint32 reserved1 : 3;
		uint32 quickFirstDetection : 1; //quick_first_detection, reset value: 0x1, access type: RW
		uint32 reserved2 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf123_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF124 0x1490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 decimatorOobTap : 4; //decimator_oob_tap, reset value: 0x9, access type: RW
		uint32 decimatorOobTapHb : 4; //decimator_oob_tap_hb, reset value: 0xb, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf124_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF125 0x1494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 decimatorRiscBankEn : 1; //decimator_risc_bank_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 decimatorRiscBank : 2; //decimator_risc_bank, reset value: 0x0, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf125_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CO_CH_COUNTER 0x14E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 coChannelCounter : 16; //co_channel_detected, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfCoChCounter_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF13B 0x14EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lltfFalseDetection : 1; //lltf_false_detection, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf13B_u;

/*REG_DFS_PHY_RX_TD_IF_RSSI_0_DATA 0x14F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssi0Data : 11; //rssi_0_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfRssi0Data_u;

/*REG_DFS_PHY_RX_TD_IF_RSSI_1_DATA 0x14FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssi1Data : 11; //rssi_1_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfRssi1Data_u;

/*REG_DFS_PHY_RX_TD_IF_RSSI_2_DATA 0x1500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssi2Data : 11; //rssi_2_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfRssi2Data_u;

/*REG_DFS_PHY_RX_TD_IF_RSSI_3_DATA 0x1504 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssi3Data : 11; //rssi_3_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfRssi3Data_u;

/*REG_DFS_PHY_RX_TD_IF_RSSI_4_DATA 0x1508 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssi4Data : 11; //rssi_4_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegDfsPhyRxTdIfRssi4Data_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF143 0x150C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlug16Low : 16; //Rx TD Risc Test Bus 16 Low, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf143_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW 0x1510 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugLow : 8; //Rx TD Risc Test Bus low , reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugLow_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF145 0x1514 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlug16Mid : 16; //Rx TD Risc Test Bus 16 Mid, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf145_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID 0x1518 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugMid : 8; //Rx TD Risc Test Bus Mid, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugMid_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF147 0x151C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlug16High : 16; //Rx TD Risc Test Bus 16 High, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf147_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH 0x1520 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugHigh : 8; //Rx TD Risc Test Bus High, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugHigh_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF149 0x1524 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlug16Four : 16; //Rx TD Risc Test Bus 16 four, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf149_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR 0x1528 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugFour : 8; //Rx TD Risc Test Bus four, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfTestPlugFour_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14B 0x152C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAn0LocalFineCntFromCoarseRdy : 12; //det_an0_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf14B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14C 0x1530 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAn1LocalFineCntFromCoarseRdy : 12; //det_an0_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf14C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14D 0x1534 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAn2LocalFineCntFromCoarseRdy : 12; //det_an0_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf14D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14E 0x1538 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAn3LocalFineCntFromCoarseRdy : 12; //det_an0_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf14E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14F 0x153C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detCommonFineCntFromCoarseRdy : 12; //det_an0_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf14F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF150 0x1540 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerMode : 1; //logger_mode, reset value: 0x0, access type: RW
		uint32 testBusFifoLowThr : 15; //test_bus_fifo_low_thr, reset value: 0x500, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf150_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF151 0x1544 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 testBusFifoHighThr : 15; //test_bus_fifo_high_thr, reset value: 0x200, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf151_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF152 0x1548 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusDumpAllDataStrb : 1; //test_bus_dump_all_data_strb, reset value: 0x0, access type: WO
		uint32 testBusOneShotMode : 1; //test_bus_one_shot_mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf152_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF153 0x154C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyTxEn : 1; //pac_phy_tx_en, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf153_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF154 0x1550 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerRiscBytesInFifo : 8; //logger_risc_bytes_in_fifo, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf154_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF156 0x1558 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcCancellationOnHtStf : 1; //dc_cancellation_on_ht_stf, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf156_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF158 0x1560 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacRxPhyActive : 1; //phy pac rx phy active, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf158_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF15F 0x157C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorBypassCntDet : 16; //detector_bypass_cnt_det, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf15F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF160 0x1580 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorBypassCntCoarse : 16; //detector_bypass_cnt_coarse, reset value: 0x500, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf160_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF161 0x1584 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorBypassCntFine : 16; //detector_bypass_cnt_fine, reset value: 0xA00, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf161_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF163 0x158C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 waitCyclesPrimary : 8; //Rx TD  wait cycles priamry, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf163_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF164 0x1590 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macToPhyMinRssi : 8; //mac_to_phy_min_rssi, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf164_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_80 0x1594 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr800 : 8; //cca_pwr_80_0, reset value: 0x0, access type: RO
		uint32 ccaPwr801 : 8; //cca_pwr_80_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr80_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_PWR_160 0x1598 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPwr160 : 8; //cca_pwr_160, reset value: 0x0, access type: RO
		uint32 ccaSecPwr160 : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaPwr160_u;

/*REG_DFS_PHY_RX_TD_IF_CCA80S_PIFS 0x159C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cca80SPifs : 1; //cca80s_pifs, reset value: 0x1, access type: RO
		uint32 cca20PPwr : 1; //cca20p_pwr, reset value: 0x1, access type: RO
		uint32 cca20SPwrPifs : 1; //cca20s_pwr_pifs, reset value: 0x1, access type: RO
		uint32 cca40SPwrPifs : 1; //cca40s_pwr_pifs, reset value: 0x1, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfCca80SPifs_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF168 0x15A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdGearShiftSymNum : 10; //The symbol number for changing the mu (Start counting from first DATA symbol), reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf168_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF169 0x15A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntLowRt2 : 16; //Free Running Counter 2, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf169_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16A 0x15A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntLow2 : 16; //free_running_cnt_low_2, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf16A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH_2 0x15AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntHigh2 : 16; //free_running_cnt_high_2, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIfFreeRunningCntHigh2_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16C 0x15B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntThLow2 : 16; //free_running_cnt_th_low_2, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf16C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16D 0x15B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCntThHigh2 : 16; //free_running_cnt_th_high_2, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf16D_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16E 0x15B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cntReset2 : 1; //Sanity reset the Sanity Counters 2, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf16E_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16F 0x15BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityClkPerStrobe2 : 5; //Sanity Clock Per Strobe 2, reset value: 0xf, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf16F_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170 0x15C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gicombEn : 1; //gi combination enable, reset value: 0x0, access type: RW
		uint32 gicombOffset1 : 7; //gi combination offset1, reset value: 0x30, access type: RW
		uint32 gicombOffset2 : 7; //gi combination offset2, reset value: 0x30, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf170_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF171 0x15C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gicombSnrTh : 12; //gi combination snr threshold, reset value: 0x30, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf171_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172 0x15C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gicombMaxBw : 2; //gicomb_max_bw, reset value: 0x3, access type: RW
		uint32 gicombMaxMcs : 4; //gicomb_max_mcs, reset value: 0x0, access type: RW
		uint32 gicombMaxMcsLeg : 4; //gicomb_max_mcs_leg, reset value: 0x0, access type: RW
		uint32 gicombMaxNsts : 3; //gicomb_max_nsts, reset value: 0x1, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf172_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF175 0x1604 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 sanityDetCoarseRdyAn4 : 1; //Sanity Detector Coarse Ready An4, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf175_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF176 0x1608 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn4CoarseCnt : 16; //Sanity An 4 Coarse Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf176_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF177 0x160C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sanityAn4FineCnt : 16; //Sanity An 4 Fine Counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf177_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF178 0x1610 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 sanityDetFineRdyAn4 : 1; //Sanity Detector Fine Ready An4, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf178_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF179 0x1614 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataIAn4 : 13; //grisc_dc_removal_data_i_an4, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf179_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17A 0x1618 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscDcRemovalDataQAn4 : 13; //grisc_dc_removal_data_q_an4, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf17A_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17B 0x161C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zeroLastSampleShiftOnSlowRx : 1; //zero_last_sample_shift_on_slow_rx, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf17B_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17C 0x1620 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAn4LocalFineCntFromCoarseRdy : 12; //det_an4_local_fine_cnt_from_coarse, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf17C_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17D 0x1624 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscInt6Cause : 16; //cause of int6 interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfPhyRxtdIf17D_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN01 0x1628 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstRiscAn0 : 7; //Noise_est_risc_an0, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 noiseEstRiscAn1 : 7; //Noise_est_risc_an1, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstRiscAn01_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN23 0x162C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstRiscAn2 : 7; //Noise_est_risc_an2, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 noiseEstRiscAn3 : 7; //Noise_est_risc_an3, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstRiscAn23_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN45 0x1630 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstRiscAn4 : 7; //Noise_est_risc_an4, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstRiscAn45_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI0 0x1638 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstGi0Data : 7; //Rx TD Noise Estimation gi0 Data, reset value: 0x0, access type: RO
		uint32 noiseEstGi0DataValid : 1; //Rx TD Noise Estimation gi0 Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstGi0_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI1 0x163C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstGi1Data : 7; //Rx TD Noise Estimation gi1 Data, reset value: 0x0, access type: RO
		uint32 noiseEstGi1DataValid : 1; //Rx TD Noise Estimation gi1 Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstGi1_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI2 0x1640 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstGi2Data : 7; //Rx TD Noise Estimation gi2 Data, reset value: 0x0, access type: RO
		uint32 noiseEstGi2DataValid : 1; //Rx TD Noise Estimation gi2 Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstGi2_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI3 0x1644 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstGi3Data : 7; //Rx TD Noise Estimation gi3 Data, reset value: 0x0, access type: RO
		uint32 noiseEstGi3DataValid : 1; //Rx TD Noise Estimation gi3 Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstGi3_u;

/*REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI4 0x1648 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstGi4Data : 7; //Rx TD Noise Estimation gi4 Data, reset value: 0x0, access type: RO
		uint32 noiseEstGi4DataValid : 1; //Rx TD Noise Estimation gi4 Data Valid, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegDfsPhyRxTdIfNoiseEstGi4_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CLK_CENTRAL 0x164C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiClkWrDivrCentral : 6; //fcsi_clk_wr_divr_central, reset value: 0x4, access type: RW
		uint32 reserved0 : 2;
		uint32 fcsiClkRdDivrCentral : 6; //fcsi_clk_rd_divr_central, reset value: 0xf, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegDfsPhyRxTdIfFcsiClkCentral_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CLK_BIAS 0x1650 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiClkWrDivrBias : 6; //fcsi_clk_wr_divr_bias, reset value: 0x4, access type: RW
		uint32 reserved0 : 2;
		uint32 fcsiClkRdDivrBias : 6; //fcsi_clk_rd_divr_bias, reset value: 0xf, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegDfsPhyRxTdIfFcsiClkBias_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CONTROLS 0x1654 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiModeCentral : 1; //fcsi_mode_central, reset value: 0x1, access type: RW
		uint32 fcsiModeBias : 1; //fcsi_mode_bias, reset value: 0x1, access type: RW
		uint32 reserved0 : 6;
		uint32 fcsiAdditionalCycles : 4; //fcsi_additional_cycles, reset value: 0x0, access type: RW
		uint32 reserved1 : 20;
	} bitFields;
} RegDfsPhyRxTdIfFcsiControls_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS 0x1658 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralAddr : 7; //fcsi_central_addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 fcsiCentralRdWr : 1; //fcsi_central_rd_wr, reset value: 0x0, access type: RW
		uint32 fcsiCentralGo : 1; //fcsi_central_go, reset value: 0x0, access type: WO
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralAccess_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_WR_DATA 0x165C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralWrData : 16; //fcsi_central_wr_data, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralWrData_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RD_DATA 0x1660 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralRdData : 16; //fcsi_central_rd_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralRdData_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_BUSY 0x1664 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralBusy : 1; //fcsi_central_busy, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralBusy_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_DEBUG 0x1668 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralDebug : 16; //fcsi_central_debug, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralDebug_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS 0x1680 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAntMask : 4; //fcsi_ant_mask, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
		uint32 fcsiAntRdWr : 1; //fcsi_ant_rd_wr, reset value: 0x0, access type: RW
		uint32 fcsiAntGo : 1; //fcsi_ant_go, reset value: 0x0, access type: WO
		uint32 reserved1 : 16;
	} bitFields;
} RegDfsPhyRxTdIfFcsiAntAccess_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_ANT_BUSY 0x1684 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAntBusy : 4; //fcsi_ant_busy, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfFcsiAntBusy_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RESET_N 0x1688 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralResetN : 1; //fcsi_central_reset_n, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralResetN_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_FORCE_DEFAULT 0x168C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiCentralForceDefault : 1; //fcsi_central_force_default, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfFcsiCentralForceDefault_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_RESET_N 0x1698 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiBiasResetN : 1; //fcsi_bias_reset_n, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfFcsiBiasResetN_u;

/*REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_FORCE_DEFAULT 0x169C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiBiasForceDefault : 1; //fcsi_bias_force_default, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfFcsiBiasForceDefault_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_WR_CAUSE 0x16A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo2GWrCause : 4; //grisc_to_2g_wr_cause, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo2GWrCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_WR_CAUSE 0x16A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo5GWrCause : 4; //grisc_to_5g_wr_cause, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo5GWrCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_WR_CAUSE 0x16A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo6GWrCause : 4; //grisc_to_6g_wr_cause, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo6GWrCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_WR_CAUSE 0x16AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscToDfsWrCause : 4; //grisc_to_dfs_wr_cause, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscToDfsWrCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT 0x16B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo2GInt : 1; //grisc_to_2g_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo2GInt_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT 0x16B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo5GInt : 1; //grisc_to_5g_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo5GInt_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT 0x16B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo6GInt : 1; //grisc_to_6g_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo6GInt_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT 0x16BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscToDfsInt : 1; //grisc_to_dfs_int, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscToDfsInt_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT_TYPE 0x16C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo2GIntType : 1; //grisc_to_2g_int_type, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo2GIntType_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT_TYPE 0x16C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo5GIntType : 1; //grisc_to_5g_int_type, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo5GIntType_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT_TYPE 0x16C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo6GIntType : 1; //grisc_to_6g_int_type, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo6GIntType_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT_TYPE 0x16CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscToDfsIntType : 1; //grisc_to_dfs_int_type, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscToDfsIntType_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_PENDING 0x16D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo2GPending : 1; //grisc_to_2g_pending, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo2GPending_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_PENDING 0x16D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo5GPending : 1; //grisc_to_5g_pending, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo5GPending_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_PENDING 0x16D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscTo6GPending : 1; //grisc_to_6g_pending, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscTo6GPending_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_PENDING 0x16DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscToDfsPending : 1; //grisc_to_dfs_pending, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfGriscToDfsPending_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_FROM_2G_RD_CAUSE 0x16E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscFrom2GRdCause : 4; //grisc_from_2g_rd_cause, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscFrom2GRdCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_FROM_5G_RD_CAUSE 0x16E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscFrom5GRdCause : 4; //grisc_from_5g_rd_cause, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscFrom5GRdCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_FROM_6G_RD_CAUSE 0x16E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscFrom6GRdCause : 4; //grisc_from_6g_rd_cause, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscFrom6GRdCause_u;

/*REG_DFS_PHY_RX_TD_IF_GRISC_FROM_DFS_RD_CAUSE 0x16EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscFromDfsRdCause : 4; //grisc_from_dfs_rd_cause, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegDfsPhyRxTdIfGriscFromDfsRdCause_u;

/*REG_DFS_PHY_RX_TD_IF_PHY_MAC_GPIO_BAND_ACTIVE 0x16F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacGpioBandActive : 1; //indicate the phy is active (rx or tx), reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfPhyMacGpioBandActive_u;

/*REG_DFS_PHY_RX_TD_IF_DISABLE_LAST_EHT_SIG_FREQ_TRACK 0x16F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disableLastEhtSigFreqTrack : 1; //disable last eht sig freq track, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegDfsPhyRxTdIfDisableLastEhtSigFreqTrack_u;

/*REG_DFS_PHY_RX_TD_IF_IMPULSE_RESPONSE_IIR_OFFSET 0x16F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 impulseResponseIirOffset : 10; //change cp len one time before first eht ltf, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegDfsPhyRxTdIfImpulseResponseIirOffset_u;

/*REG_DFS_PHY_RX_TD_IF_CCA20SB_PIFS 0x16FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cca20SbPifs : 16; //no description, reset value: 0xFFFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCca20SbPifs_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_0 0x1700 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr200 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr201 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr200_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_2 0x1704 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr202 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr203 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr202_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_4 0x1708 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr204 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr205 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr204_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_6 0x170C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr206 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr207 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr206_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_0 0x1710 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr400 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr401 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr400_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_2 0x1714 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr402 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr403 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr402_u;

/*REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_80 0x1718 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecPwr800 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 ccaSecPwr801 : 8; //no description, reset value: 0xFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegDfsPhyRxTdIfCcaSecPwr80_u;

//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF00 0x1000 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF00_FIELD_RISC_GPI_LOW_MASK                                                 0x0000FFFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF00_FIELD_RISC_GPO_LOW_MASK                                                 0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF01 0x1004 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF01_FIELD_RISC_GPI_HIGH_MASK                                                0x0000FFFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF01_FIELD_RISC_GPO_HIGH_MASK                                                0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF02 0x1008 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF02_FIELD_RISC_GPIO_OEN_LOW_MASK                                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF03 0x100C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF03_FIELD_RISC_GPIO_OEN_HIGH_MASK                                           0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF04 0x1010 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF04_FIELD_SEMAPHORE_REG_04_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF05 0x1014 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF05_FIELD_SEMAPHORE_REG_05_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF06 0x1018 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF06_FIELD_SEMAPHORE_REG_06_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF07 0x101C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF07_FIELD_SEMAPHORE_REG_07_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF08 0x1020 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF08_FIELD_SEMAPHORE_REG_08_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF09 0x1024 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF09_FIELD_SEMAPHORE_REG_09_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1B 0x106C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1B_FIELD_TD_ENABLE_OVERRIDE_MASK                                           0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1C 0x1070 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1C_FIELD_GEN_RISC_MASK_INT_MASK                                            0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D 0x1074 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT0_MASK                                          0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT1_MASK                                          0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT2_MASK                                          0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT3_MASK                                          0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT4_MASK                                          0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT5_MASK                                          0x00000020
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT6_MASK                                          0x00000040
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT7_MASK                                          0x00000080
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT8_MASK                                          0x00000100
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT9_MASK                                          0x00000200
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1D_FIELD_GEN_RISC_CLEAR_INT10_MASK                                         0x00000400
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E 0x1078 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_NOISE_CAUSE_INT0_MASK                                    0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_TX_EN_START_CAUSE_INT1_MASK                              0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_PRE_TX_CAUSE_INT2_MASK                                   0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_RX_LATCH_FALLING_RX_RDY_CAUSE_INT3_MASK                  0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_ERROR_CAUSE_INT4_MASK                                    0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_FD_LAST_SYMBOL_CAUSE_INT5_MASK                           0x00000020
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_HOST_CAUSE_INT6_MASK                                     0x00000040
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_HOST_CAUSE_INT7_MASK                                     0x00000080
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_HOST_CAUSE_INT8_MASK                                     0x00000100
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_HOST_CAUSE_INT9_MASK                                     0x00000200
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1E_FIELD_GEN_RISC_HOST_CAUSE_INT10_MASK                                    0x00000400
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1F 0x107C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF1F_FIELD_ENABLE_ERROR_INT_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_ERROR_INT 0x1080 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_ERROR_INT_FIELD_CLEAR_ERROR_INT_MASK                                0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF21 0x1084 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF21_FIELD_RX_RDY_FALL_DELAY_IF_MASK                                         0x00001FFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF21_FIELD_RX_RDY_FALL_DELAY_IF_EN_MASK                                      0x00008000
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF22 0x1088 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF22_FIELD_FREE_RUNNING_CNT_LOW_RT_MASK                                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF23 0x108C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF23_FIELD_OFDM_ENABLE_BIT_MASK                                              0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF24 0x1090 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF24_FIELD_CSM_ANTENNA_ENABLE_MASK                                           0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF25 0x1094 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF25_FIELD_ANTENNA_DELETION_MASK                                             0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF26 0x1098 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF26_FIELD_SYMBOL_PRAM_READY_STM_MASK                                        0x00000003
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF26_FIELD_DELTA_F_STM_MASK                                                  0x00000030
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF28 0x10A0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF28_FIELD_STOP_SIGNAL_TH_IF_MASK                                            0x00001FFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF28_FIELD_STOP_SIGNAL_TH_IF_EN_MASK                                         0x00008000
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF29 0x10A4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF29_FIELD_RISC_INT6_MASK                                                    0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2A 0x10A8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2A_FIELD_TD_CSM_STATE_MASK                                                 0x00007000
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2B 0x10AC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2B_FIELD_GEN_RISC_AIRTIME_US_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2C 0x10B0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2C_FIELD_GEN_RISC_4US_BOUNDARY_OFFSET_MASK                                 0x00000FFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2C_FIELD_CCA_CNT_SRC_CTRL_MASK                                             0x0000F000
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D 0x10B4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D_FIELD_HT_SHORT_IND_MASK                                                 0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D_FIELD_CCA_CS_MASK                                                       0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2D_FIELD_HE_SHORT_IND_MASK                                                 0x00000004
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E 0x10B8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E_FIELD_PACKET_TYPE_MASK                                                  0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E_FIELD_IS_HT_SW_MASK                                                     0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E_FIELD_IS_HT_SEL_MASK                                                    0x00000080
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2E_FIELD_GENRISC_RDY_ENABLE_MASK                                           0x00000100
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2F 0x10BC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2F_FIELD_DC_REMOVAL_SHIFTER_MASK                                           0x0000000F
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF2F_FIELD_DC_REMOVAL_SHIFTER_GEAR_SHIFT_MASK                                0x000000F0
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF30 0x10C0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF30_FIELD_GEN_RISC_AIRTIME_US_MAX_MASK                                      0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF31 0x10C4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF31_FIELD_GEN_RISC_AIRTIME_US_OVERRIDE_MASK                                 0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32 0x10C8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_BUSY_GEN_RISC_MASK                                            0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_MODE_CTRL_MASK                                                0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_BUSY_GEN_RISC_SEC_MASK                                        0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_MODE_CTRL_SEC_MASK                                            0x00000020
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_BUSY_GEN_RISC_SEC_40_MASK                                     0x00000100
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF32_FIELD_CCA_MODE_CTRL_SEC_40_MASK                                         0x00000200
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA20P 0x10CC */
#define B1_DFS_PHY_RX_TD_IF_CCA20P_FIELD_CCA20P_MASK                                                              0x00000001
#define B1_DFS_PHY_RX_TD_IF_CCA20P_FIELD_CCA20S_MASK                                                              0x00000002
#define B1_DFS_PHY_RX_TD_IF_CCA20P_FIELD_CCA40S_MASK                                                              0x00000004
#define B1_DFS_PHY_RX_TD_IF_CCA20P_FIELD_CCA20S_PIFS_MASK                                                         0x00000008
#define B1_DFS_PHY_RX_TD_IF_CCA20P_FIELD_CCA40S_PIFS_MASK                                                         0x00000010
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF34 0x10D0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF34_FIELD_ERROR_CAUSE_REG_MASK                                              0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF3D 0x10F4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF3D_FIELD_GEN_RISC_TEST_BUS_EN_MASK                                         0x000000FF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF3D_FIELD_GEN_RISC_TEST_BUS_EN_1_MASK                                       0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A 0x1128 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A_FIELD_TEST_FIFO_FULL_MASK                                               0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A_FIELD_TEST_FIFO_FULL_1_MASK                                             0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A_FIELD_TB_FIFO_ERROR_MASK                                                0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A_FIELD_TB_FIFO_ERROR_1_MASK                                              0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4A_FIELD_TB_FIFO_ERROR_ABSORPTION_MASK                                     0x00000FF0
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C 0x1130 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C_FIELD_DETECTOR_BYPASS_EN_MASK                                           0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C_FIELD_DETECTOR_BYPASS_RESET_RXTD_EN_MASK                                0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4C_FIELD_DETECTOR_BYPASS_CNT_VALUE_MASK                                    0x0000FFF0
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4D 0x1134 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4D_FIELD_ENABLE_CCA_INT_MASK                                               0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CCA_INT 0x1138 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CCA_INT_FIELD_CLEAR_CCA_INT_MASK                                          0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4F 0x113C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF4F_FIELD_CCA_CAUSE_REG_MASK                                                0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF56 0x1158 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF56_FIELD_NOISE_EST_RISC_ALL_VALID_MASK                                     0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF56_FIELD_NOISE_EST_RISC_SELECT_MASK                                        0x00000002
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF5F 0x117C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF5F_FIELD_DET_SHORTENED_MA_MASK                                             0x0000007F
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF5F_FIELD_WAIT_CYCLES_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67 0x119C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67_FIELD_WD_COUNTER_EN_MASK                                                0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67_FIELD_CLEAR_WD_COUNTER_MASK                                             0x00000010
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6A 0x11A8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6A_FIELD_DET_NOISE_EST_VALID_EN_MASK                                       0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6B 0x11AC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6B_FIELD_SANITY_DET_RAW_DETECTION_MASK                                     0x000000FF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6B_FIELD_SANITY_DET_TB_RAW_DETECTION_MASK                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D 0x11B4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COARSE_RDY_AN0_MASK                                    0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COARSE_RDY_AN1_MASK                                    0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COARSE_RDY_AN2_MASK                                    0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COARSE_RDY_AN3_MASK                                    0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COMMON_COARSE_ERR_MASK                                 0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6D_FIELD_SANITY_DET_COMMON_FINE_ERR_MASK                                   0x00000020
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6E 0x11B8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6E_FIELD_SANITY_HOST_ANTENNA_EN_MASK                                       0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6F 0x11BC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF6F_FIELD_SANITY_AN0_COARSE_CNT_MASK                                        0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF70 0x11C0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF70_FIELD_SANITY_AN1_COARSE_CNT_MASK                                        0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF71 0x11C4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF71_FIELD_SANITY_AN2_COARSE_CNT_MASK                                        0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF72 0x11C8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF72_FIELD_SANITY_AN3_COARSE_CNT_MASK                                        0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF73 0x11CC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF73_FIELD_SANITY_AN0_FINE_CNT_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF74 0x11D0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF74_FIELD_SANITY_AN1_FINE_CNT_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF75 0x11D4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF75_FIELD_SANITY_AN2_FINE_CNT_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF76 0x11D8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF76_FIELD_SANITY_AN3_FINE_CNT_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF77 0x11DC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF77_FIELD_SANITY_CLK_PER_STROBE_MASK                                        0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF78 0x11E0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF78_FIELD_CNT_RESET_MASK                                                    0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF79 0x11E4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF79_FIELD_COMMON_COARSE_RDY_CNT_MASK                                        0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7A 0x11E8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7A_FIELD_ENABLE_TIMERS_INT_MASK                                            0x00000003
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_TIMERS_INT 0x11EC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CLEAR_TIMERS_INT_FIELD_CLEAR_TIMERS_INT_MASK                              0x00000003
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7C 0x11F0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7C_FIELD_CCA_TIMERS_REG_MASK                                               0x00000003
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7D 0x11F4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7D_FIELD_COMMON_FINE_RDY_CNT_MASK                                          0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7E 0x11F8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7E_FIELD_RXTD_CSM_NCO_FREQ_DATA_MASK                                       0x00007FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F 0x11FC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_FINE_RDY_AN0_MASK                                      0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_FINE_RDY_AN1_MASK                                      0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_FINE_RDY_AN2_MASK                                      0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_FINE_RDY_AN3_MASK                                      0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_COMMON_FINE_RDY_MASK                                   0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF7F_FIELD_SANITY_DET_COMMON_COARSE_RDY_MASK                                 0x00000020
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80 0x1200 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80_FIELD_SANITY_DET_DETECTOR_BW_MASK                                       0x00000003
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80_FIELD_SANITY_DET_IMD_DETECT_MASK                                        0x00000008
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80_FIELD_SANITY_DET_SIGNAL_DETECT_MASK                                     0x00000010
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF80_FIELD_SANITY_DET_DETECTOR_MASK_MASK                                     0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SANITY_DELTA_F_VALID 0x1208 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SANITY_DELTA_F_VALID_FIELD_SANITY_DELTA_F_VALID_MASK                      0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84 0x1210 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84_FIELD_PHY_MAC_INT_MASK_MASK                                             0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84_FIELD_TX_PACKET_INT_MASK_MASK                                           0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84_FIELD_WD_INT_MASK_MASK                                                  0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84_FIELD_DFS_INT_MASK_MASK                                                 0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85 0x1214 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85_FIELD_CLEAR_PHY_MAC_INT_MASK                                            0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85_FIELD_CLEAR_TX_PACKET_INT_MASK                                          0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85_FIELD_CLEAR_WD_INT_MASK                                                 0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF85_FIELD_CLEAR_DFS_INT_MASK                                                0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86 0x1218 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86_FIELD_PHY_MAC_INT_CAUSE_REG_MASK                                        0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86_FIELD_TX_PACKET_INT_CAUSE_REG_MASK                                      0x00000002
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86_FIELD_WD_INT_CAUSE_REG_MASK                                             0x00000004
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF86_FIELD_DFS_INT_CAUSE_REG_MASK                                            0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF87 0x121C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF87_FIELD_PHY_MAC_INT_MASK                                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF88 0x1220 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF88_FIELD_FREE_RUNNING_CNT_LOW_MASK                                         0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH 0x1224 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH_FIELD_FREE_RUNNING_CNT_HIGH_MASK                    0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8A 0x1228 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8A_FIELD_FREE_RUNNING_CNT_TH_LOW_MASK                                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8B 0x122C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8B_FIELD_FREE_RUNNING_CNT_TH_HIGH_MASK                                     0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8F 0x123C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF8F_FIELD_SW_RESET_N_REG_MASK                                               0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF90 0x1240 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF90_FIELD_BLOCK_EN_MASK                                                     0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF91 0x1244 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF91_FIELD_GSM_SW_RESET_MASK_MASK                                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SW_RESET_GENERATE 0x1248 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_SW_RESET_GENERATE_FIELD_SW_RESET_GENERATE_MASK                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF93 0x124C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF93_FIELD_GRISC_DC_REMOVAL_DATA_I_AN0_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF94 0x1250 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF94_FIELD_GRISC_DC_REMOVAL_DATA_Q_AN0_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF95 0x1254 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF95_FIELD_GRISC_DC_REMOVAL_DATA_I_AN1_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF96 0x1258 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF96_FIELD_GRISC_DC_REMOVAL_DATA_Q_AN1_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF97 0x125C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF97_FIELD_GRISC_DC_REMOVAL_DATA_I_AN2_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF98 0x1260 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF98_FIELD_GRISC_DC_REMOVAL_DATA_Q_AN2_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF99 0x1264 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF99_FIELD_GRISC_DC_REMOVAL_DATA_I_AN3_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9A 0x1268 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9A_FIELD_GRISC_DC_REMOVAL_DATA_Q_AN3_MASK                                  0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9B 0x126C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9B_FIELD_GRISC_DC_REMOVAL_VALID_MASK                                       0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF9B_FIELD_GRISC_DC_SHIFT_VALUE_OVERRIDE_MASK                                0x00000010
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_0 0x12D4 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_0_FIELD_CCA_PWR_20_0_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_0_FIELD_CCA_PWR_20_1_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_2 0x12D8 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_2_FIELD_CCA_PWR_20_2_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_2_FIELD_CCA_PWR_20_3_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_4 0x12DC */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_4_FIELD_CCA_PWR_20_4_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_4_FIELD_CCA_PWR_20_5_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER0 0x1320 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER0_FIELD_TIMER0_TH_MASK                                               0x0000FFFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER0_FIELD_TIMER0_MASK                                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFC9 0x1324 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFC9_FIELD_TIMER0_STB_TH_MASK                                                0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER1 0x1328 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER1_FIELD_TIMER1_TH_MASK                                               0x0000FFFF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TIMER1_FIELD_TIMER1_MASK                                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFCB 0x132C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFCB_FIELD_TIMER1_STB_TH_MASK                                                0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW_ONLY16 0x1330 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW_ONLY16_FIELD_TEST_PLUG_LOW_ONLY16_MASK                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID_ONLY16 0x1334 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID_ONLY16_FIELD_TEST_PLUG_MID_ONLY16_MASK                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH_ONLY16 0x1338 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH_ONLY16_FIELD_TEST_PLUG_HIGH_ONLY16_MASK                    0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR_ONLY16 0x133C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR_ONLY16_FIELD_TEST_PLUG_FOUR_ONLY16_MASK                    0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_20_6 0x1340 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_6_FIELD_CCA_PWR_20_6_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_20_6_FIELD_CCA_PWR_20_7_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_0 0x1344 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_40_0_FIELD_CCA_PWR_40_0_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_40_0_FIELD_CCA_PWR_40_1_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_40_2 0x1348 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_40_2_FIELD_CCA_PWR_40_2_MASK                                                  0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_40_2_FIELD_CCA_PWR_40_3_MASK                                                  0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF0 0x13C0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF0_FIELD_ACCUMULATOR_RESET_VALUE_I_MASK                                    0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF1 0x13C4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF1_FIELD_ACCUMULATOR_RESET_VALUE_Q_MASK                                    0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF2 0x13C8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFF2_FIELD_RESET_VALUES_IQ_MASK                                              0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFB 0x13EC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFB_FIELD_HYP_RXTD_GPR_LOW_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFC 0x13F0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IFFC_FIELD_HYP_RXTD_GPR_HIGH_MASK                                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF102 0x1408 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF102_FIELD_CSM_ERROR_CAUSE_REG_MASK                                         0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF103 0x140C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF103_FIELD_CSM_ENABLE_ERROR_INT_MASK                                        0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CSM_CLEAR_ERROR_INT 0x1410 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CSM_CLEAR_ERROR_INT_FIELD_CSM_CLEAR_ERROR_INT_MASK                        0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10D 0x1434 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10D_FIELD_SHORT_CALC_GO_MASK                                               0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10E 0x1438 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10E_FIELD_SANITY_RISC_ANTENNA_EN_MASK                                      0x0000001F
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF10E_FIELD_SANITY_RISC_ANTENNA_EN_VALID_MASK                                0x00000020
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF120 0x1480 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF120_FIELD_CLEAR_PIFS_MEM_MASK                                              0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF121 0x1484 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF121_FIELD_CCA_CLEAR_PRIM20_GENRISC_MASK                                    0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF121_FIELD_CCA_CLEAR_PRIM20_ENABLE_MASK                                     0x00000002
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF122 0x1488 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF122_FIELD_CCA_PWR_20_MAX_MASK                                              0x000000FF
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF122_FIELD_CCA_SEC_PWR_20_MAX_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123 0x148C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123_FIELD_TIMING_DIFF_TH_MASK                                              0x0000007F
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123_FIELD_USE_VERIFIED_DET_MASK                                            0x00000100
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF123_FIELD_QUICK_FIRST_DETECTION_MASK                                       0x00001000
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF124 0x1490 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF124_FIELD_DECIMATOR_OOB_TAP_MASK                                           0x0000000F
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF124_FIELD_DECIMATOR_OOB_TAP_HB_MASK                                        0x000000F0
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF125 0x1494 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF125_FIELD_DECIMATOR_RISC_BANK_EN_MASK                                      0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF125_FIELD_DECIMATOR_RISC_BANK_MASK                                         0x00000030
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CO_CH_COUNTER 0x14E8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_CO_CH_COUNTER_FIELD_CO_CHANNEL_COUNTER_MASK                               0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF13B 0x14EC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF13B_FIELD_LLTF_FALSE_DETECTION_MASK                                        0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_RSSI_0_DATA 0x14F8 */
#define B1_DFS_PHY_RX_TD_IF_RSSI_0_DATA_FIELD_RSSI_0_DATA_MASK                                                    0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_RSSI_1_DATA 0x14FC */
#define B1_DFS_PHY_RX_TD_IF_RSSI_1_DATA_FIELD_RSSI_1_DATA_MASK                                                    0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_RSSI_2_DATA 0x1500 */
#define B1_DFS_PHY_RX_TD_IF_RSSI_2_DATA_FIELD_RSSI_2_DATA_MASK                                                    0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_RSSI_3_DATA 0x1504 */
#define B1_DFS_PHY_RX_TD_IF_RSSI_3_DATA_FIELD_RSSI_3_DATA_MASK                                                    0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_RSSI_4_DATA 0x1508 */
#define B1_DFS_PHY_RX_TD_IF_RSSI_4_DATA_FIELD_RSSI_4_DATA_MASK                                                    0x000007FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF143 0x150C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF143_FIELD_TEST_PLUG_16_LOW_MASK                                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW 0x1510 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_LOW_FIELD_TEST_PLUG_LOW_MASK                                    0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF145 0x1514 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF145_FIELD_TEST_PLUG_16_MID_MASK                                            0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID 0x1518 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_MID_FIELD_TEST_PLUG_MID_MASK                                    0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF147 0x151C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF147_FIELD_TEST_PLUG_16_HIGH_MASK                                           0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH 0x1520 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_HIGH_FIELD_TEST_PLUG_HIGH_MASK                                  0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF149 0x1524 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF149_FIELD_TEST_PLUG_16_FOUR_MASK                                           0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR 0x1528 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_TEST_PLUG_FOUR_FIELD_TEST_PLUG_FOUR_MASK                                  0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14B 0x152C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14B_FIELD_DET_AN0_LOCAL_FINE_CNT_FROM_COARSE_RDY_MASK                      0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14C 0x1530 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14C_FIELD_DET_AN1_LOCAL_FINE_CNT_FROM_COARSE_RDY_MASK                      0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14D 0x1534 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14D_FIELD_DET_AN2_LOCAL_FINE_CNT_FROM_COARSE_RDY_MASK                      0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14E 0x1538 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14E_FIELD_DET_AN3_LOCAL_FINE_CNT_FROM_COARSE_RDY_MASK                      0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14F 0x153C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF14F_FIELD_DET_COMMON_FINE_CNT_FROM_COARSE_RDY_MASK                         0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF150 0x1540 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF150_FIELD_LOGGER_MODE_MASK                                                 0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF150_FIELD_TEST_BUS_FIFO_LOW_THR_MASK                                       0x0000FFFE
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF151 0x1544 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF151_FIELD_TEST_BUS_FIFO_HIGH_THR_MASK                                      0x0000FFFE
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF152 0x1548 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF152_FIELD_TEST_BUS_DUMP_ALL_DATA_STRB_MASK                                 0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF152_FIELD_TEST_BUS_ONE_SHOT_MODE_MASK                                      0x00000002
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF153 0x154C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF153_FIELD_PAC_PHY_TX_EN_MASK                                               0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF154 0x1550 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF154_FIELD_LOGGER_RISC_BYTES_IN_FIFO_MASK                                   0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF156 0x1558 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF156_FIELD_DC_CANCELLATION_ON_HT_STF_MASK                                   0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF158 0x1560 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF158_FIELD_PHY_PAC_RX_PHY_ACTIVE_MASK                                       0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF15F 0x157C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF15F_FIELD_DETECTOR_BYPASS_CNT_DET_MASK                                     0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF160 0x1580 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF160_FIELD_DETECTOR_BYPASS_CNT_COARSE_MASK                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF161 0x1584 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF161_FIELD_DETECTOR_BYPASS_CNT_FINE_MASK                                    0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF163 0x158C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF163_FIELD_WAIT_CYCLES_PRIMARY_MASK                                         0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF164 0x1590 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF164_FIELD_MAC_TO_PHY_MIN_RSSI_MASK                                         0x000000FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_80 0x1594 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_80_FIELD_CCA_PWR_80_0_MASK                                                    0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_80_FIELD_CCA_PWR_80_1_MASK                                                    0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_PWR_160 0x1598 */
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_160_FIELD_CCA_PWR_160_MASK                                                    0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_PWR_160_FIELD_CCA_SEC_PWR_160_MASK                                                0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA80S_PIFS 0x159C */
#define B1_DFS_PHY_RX_TD_IF_CCA80S_PIFS_FIELD_CCA80S_PIFS_MASK                                                    0x00000001
#define B1_DFS_PHY_RX_TD_IF_CCA80S_PIFS_FIELD_CCA20P_PWR_MASK                                                     0x00000002
#define B1_DFS_PHY_RX_TD_IF_CCA80S_PIFS_FIELD_CCA20S_PWR_PIFS_MASK                                                0x00000004
#define B1_DFS_PHY_RX_TD_IF_CCA80S_PIFS_FIELD_CCA40S_PWR_PIFS_MASK                                                0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF168 0x15A0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF168_FIELD_TD_GEAR_SHIFT_SYM_NUM_MASK                                       0x000003FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF169 0x15A4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF169_FIELD_FREE_RUNNING_CNT_LOW_RT_2_MASK                                   0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16A 0x15A8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16A_FIELD_FREE_RUNNING_CNT_LOW_2_MASK                                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH_2 0x15AC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF_FREE_RUNNING_CNT_HIGH_2_FIELD_FREE_RUNNING_CNT_HIGH_2_MASK                0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16C 0x15B0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16C_FIELD_FREE_RUNNING_CNT_TH_LOW_2_MASK                                   0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16D 0x15B4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16D_FIELD_FREE_RUNNING_CNT_TH_HIGH_2_MASK                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16E 0x15B8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16E_FIELD_CNT_RESET_2_MASK                                                 0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16F 0x15BC */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF16F_FIELD_SANITY_CLK_PER_STROBE_2_MASK                                     0x0000001F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170 0x15C0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170_FIELD_GICOMB_EN_MASK                                                   0x00000001
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170_FIELD_GICOMB_OFFSET1_MASK                                              0x000000FE
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF170_FIELD_GICOMB_OFFSET2_MASK                                              0x00007F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF171 0x15C4 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF171_FIELD_GICOMB_SNR_TH_MASK                                               0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172 0x15C8 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172_FIELD_GICOMB_MAX_BW_MASK                                               0x00000003
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172_FIELD_GICOMB_MAX_MCS_MASK                                              0x0000003C
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172_FIELD_GICOMB_MAX_MCS_LEG_MASK                                          0x000003C0
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF172_FIELD_GICOMB_MAX_NSTS_MASK                                             0x00001C00
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF175 0x1604 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF175_FIELD_SANITY_DET_COARSE_RDY_AN4_MASK                                   0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF176 0x1608 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF176_FIELD_SANITY_AN4_COARSE_CNT_MASK                                       0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF177 0x160C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF177_FIELD_SANITY_AN4_FINE_CNT_MASK                                         0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF178 0x1610 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF178_FIELD_SANITY_DET_FINE_RDY_AN4_MASK                                     0x00000008
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF179 0x1614 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF179_FIELD_GRISC_DC_REMOVAL_DATA_I_AN4_MASK                                 0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17A 0x1618 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17A_FIELD_GRISC_DC_REMOVAL_DATA_Q_AN4_MASK                                 0x00001FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17B 0x161C */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17B_FIELD_ZERO_LAST_SAMPLE_SHIFT_ON_SLOW_RX_MASK                           0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17C 0x1620 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17C_FIELD_DET_AN4_LOCAL_FINE_CNT_FROM_COARSE_RDY_MASK                      0x00000FFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17D 0x1624 */
#define B1_DFS_PHY_RX_TD_IF_PHY_RXTD_IF17D_FIELD_RISC_INT6_CAUSE_MASK                                             0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN01 0x1628 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN01_FIELD_NOISE_EST_RISC_AN0_MASK                                     0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN01_FIELD_NOISE_EST_RISC_AN1_MASK                                     0x00007F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN23 0x162C */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN23_FIELD_NOISE_EST_RISC_AN2_MASK                                     0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN23_FIELD_NOISE_EST_RISC_AN3_MASK                                     0x00007F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN45 0x1630 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_RISC_AN45_FIELD_NOISE_EST_RISC_AN4_MASK                                     0x0000007F
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI0 0x1638 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI0_FIELD_NOISE_EST_GI0_DATA_MASK                                           0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI0_FIELD_NOISE_EST_GI0_DATA_VALID_MASK                                     0x00000080
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI1 0x163C */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI1_FIELD_NOISE_EST_GI1_DATA_MASK                                           0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI1_FIELD_NOISE_EST_GI1_DATA_VALID_MASK                                     0x00000080
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI2 0x1640 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI2_FIELD_NOISE_EST_GI2_DATA_MASK                                           0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI2_FIELD_NOISE_EST_GI2_DATA_VALID_MASK                                     0x00000080
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI3 0x1644 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI3_FIELD_NOISE_EST_GI3_DATA_MASK                                           0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI3_FIELD_NOISE_EST_GI3_DATA_VALID_MASK                                     0x00000080
//========================================
/* REG_DFS_PHY_RX_TD_IF_NOISE_EST_GI4 0x1648 */
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI4_FIELD_NOISE_EST_GI4_DATA_MASK                                           0x0000007F
#define B1_DFS_PHY_RX_TD_IF_NOISE_EST_GI4_FIELD_NOISE_EST_GI4_DATA_VALID_MASK                                     0x00000080
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CLK_CENTRAL 0x164C */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CLK_CENTRAL_FIELD_FCSI_CLK_WR_DIVR_CENTRAL_MASK                                  0x0000003F
#define B1_DFS_PHY_RX_TD_IF_FCSI_CLK_CENTRAL_FIELD_FCSI_CLK_RD_DIVR_CENTRAL_MASK                                  0x00003F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CLK_BIAS 0x1650 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CLK_BIAS_FIELD_FCSI_CLK_WR_DIVR_BIAS_MASK                                        0x0000003F
#define B1_DFS_PHY_RX_TD_IF_FCSI_CLK_BIAS_FIELD_FCSI_CLK_RD_DIVR_BIAS_MASK                                        0x00003F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CONTROLS 0x1654 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CONTROLS_FIELD_FCSI_MODE_CENTRAL_MASK                                            0x00000001
#define B1_DFS_PHY_RX_TD_IF_FCSI_CONTROLS_FIELD_FCSI_MODE_BIAS_MASK                                               0x00000002
#define B1_DFS_PHY_RX_TD_IF_FCSI_CONTROLS_FIELD_FCSI_ADDITIONAL_CYCLES_MASK                                       0x00000F00
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS 0x1658 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS_FIELD_FCSI_CENTRAL_ADDR_MASK                                      0x0000007F
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS_FIELD_FCSI_CENTRAL_RD_WR_MASK                                     0x00004000
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_ACCESS_FIELD_FCSI_CENTRAL_GO_MASK                                        0x00008000
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_WR_DATA 0x165C */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_WR_DATA_FIELD_FCSI_CENTRAL_WR_DATA_MASK                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RD_DATA 0x1660 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RD_DATA_FIELD_FCSI_CENTRAL_RD_DATA_MASK                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_BUSY 0x1664 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_BUSY_FIELD_FCSI_CENTRAL_BUSY_MASK                                        0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_DEBUG 0x1668 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_DEBUG_FIELD_FCSI_CENTRAL_DEBUG_MASK                                      0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS 0x1680 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS_FIELD_FCSI_ANT_MASK_MASK                                              0x0000000F
#define B1_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS_FIELD_FCSI_ANT_RD_WR_MASK                                             0x00004000
#define B1_DFS_PHY_RX_TD_IF_FCSI_ANT_ACCESS_FIELD_FCSI_ANT_GO_MASK                                                0x00008000
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_ANT_BUSY 0x1684 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_ANT_BUSY_FIELD_FCSI_ANT_BUSY_MASK                                                0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RESET_N 0x1688 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_RESET_N_FIELD_FCSI_CENTRAL_RESET_N_MASK                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_FORCE_DEFAULT 0x168C */
#define B1_DFS_PHY_RX_TD_IF_FCSI_CENTRAL_FORCE_DEFAULT_FIELD_FCSI_CENTRAL_FORCE_DEFAULT_MASK                      0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_RESET_N 0x1698 */
#define B1_DFS_PHY_RX_TD_IF_FCSI_BIAS_RESET_N_FIELD_FCSI_BIAS_RESET_N_MASK                                        0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_FCSI_BIAS_FORCE_DEFAULT 0x169C */
#define B1_DFS_PHY_RX_TD_IF_FCSI_BIAS_FORCE_DEFAULT_FIELD_FCSI_BIAS_FORCE_DEFAULT_MASK                            0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_WR_CAUSE 0x16A0 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_2G_WR_CAUSE_FIELD_GRISC_TO_2G_WR_CAUSE_MASK                                  0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_WR_CAUSE 0x16A4 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_5G_WR_CAUSE_FIELD_GRISC_TO_5G_WR_CAUSE_MASK                                  0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_WR_CAUSE 0x16A8 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_6G_WR_CAUSE_FIELD_GRISC_TO_6G_WR_CAUSE_MASK                                  0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_WR_CAUSE 0x16AC */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_WR_CAUSE_FIELD_GRISC_TO_DFS_WR_CAUSE_MASK                                0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT 0x16B0 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT_FIELD_GRISC_TO_2G_INT_MASK                                            0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT 0x16B4 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT_FIELD_GRISC_TO_5G_INT_MASK                                            0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT 0x16B8 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT_FIELD_GRISC_TO_6G_INT_MASK                                            0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT 0x16BC */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT_FIELD_GRISC_TO_DFS_INT_MASK                                          0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT_TYPE 0x16C0 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_2G_INT_TYPE_FIELD_GRISC_TO_2G_INT_TYPE_MASK                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT_TYPE 0x16C4 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_5G_INT_TYPE_FIELD_GRISC_TO_5G_INT_TYPE_MASK                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT_TYPE 0x16C8 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_6G_INT_TYPE_FIELD_GRISC_TO_6G_INT_TYPE_MASK                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT_TYPE 0x16CC */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_INT_TYPE_FIELD_GRISC_TO_DFS_INT_TYPE_MASK                                0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_2G_PENDING 0x16D0 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_2G_PENDING_FIELD_GRISC_TO_2G_PENDING_MASK                                    0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_5G_PENDING 0x16D4 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_5G_PENDING_FIELD_GRISC_TO_5G_PENDING_MASK                                    0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_6G_PENDING 0x16D8 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_6G_PENDING_FIELD_GRISC_TO_6G_PENDING_MASK                                    0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_PENDING 0x16DC */
#define B1_DFS_PHY_RX_TD_IF_GRISC_TO_DFS_PENDING_FIELD_GRISC_TO_DFS_PENDING_MASK                                  0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_FROM_2G_RD_CAUSE 0x16E0 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_FROM_2G_RD_CAUSE_FIELD_GRISC_FROM_2G_RD_CAUSE_MASK                              0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_FROM_5G_RD_CAUSE 0x16E4 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_FROM_5G_RD_CAUSE_FIELD_GRISC_FROM_5G_RD_CAUSE_MASK                              0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_FROM_6G_RD_CAUSE 0x16E8 */
#define B1_DFS_PHY_RX_TD_IF_GRISC_FROM_6G_RD_CAUSE_FIELD_GRISC_FROM_6G_RD_CAUSE_MASK                              0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_GRISC_FROM_DFS_RD_CAUSE 0x16EC */
#define B1_DFS_PHY_RX_TD_IF_GRISC_FROM_DFS_RD_CAUSE_FIELD_GRISC_FROM_DFS_RD_CAUSE_MASK                            0x0000000F
//========================================
/* REG_DFS_PHY_RX_TD_IF_PHY_MAC_GPIO_BAND_ACTIVE 0x16F0 */
#define B1_DFS_PHY_RX_TD_IF_PHY_MAC_GPIO_BAND_ACTIVE_FIELD_PHY_MAC_GPIO_BAND_ACTIVE_MASK                          0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_DISABLE_LAST_EHT_SIG_FREQ_TRACK 0x16F4 */
#define B1_DFS_PHY_RX_TD_IF_DISABLE_LAST_EHT_SIG_FREQ_TRACK_FIELD_DISABLE_LAST_EHT_SIG_FREQ_TRACK_MASK            0x00000001
//========================================
/* REG_DFS_PHY_RX_TD_IF_IMPULSE_RESPONSE_IIR_OFFSET 0x16F8 */
#define B1_DFS_PHY_RX_TD_IF_IMPULSE_RESPONSE_IIR_OFFSET_FIELD_IMPULSE_RESPONSE_IIR_OFFSET_MASK                    0x000003FF
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA20SB_PIFS 0x16FC */
#define B1_DFS_PHY_RX_TD_IF_CCA20SB_PIFS_FIELD_CCA20SB_PIFS_MASK                                                  0x0000FFFF
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_0 0x1700 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_0_FIELD_CCA_SEC_PWR_20_0_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_0_FIELD_CCA_SEC_PWR_20_1_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_2 0x1704 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_2_FIELD_CCA_SEC_PWR_20_2_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_2_FIELD_CCA_SEC_PWR_20_3_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_4 0x1708 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_4_FIELD_CCA_SEC_PWR_20_4_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_4_FIELD_CCA_SEC_PWR_20_5_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_6 0x170C */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_6_FIELD_CCA_SEC_PWR_20_6_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_20_6_FIELD_CCA_SEC_PWR_20_7_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_0 0x1710 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_0_FIELD_CCA_SEC_PWR_40_0_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_0_FIELD_CCA_SEC_PWR_40_1_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_2 0x1714 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_2_FIELD_CCA_SEC_PWR_40_2_MASK                                          0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_40_2_FIELD_CCA_SEC_PWR_40_3_MASK                                          0x0000FF00
//========================================
/* REG_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_80 0x1718 */
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_80_FIELD_CCA_SEC_PWR_80_0_MASK                                            0x000000FF
#define B1_DFS_PHY_RX_TD_IF_CCA_SEC_PWR_80_FIELD_CCA_SEC_PWR_80_1_MASK                                            0x0000FF00


#endif // _DFS_PHY_RX_TD_IF_REGS_H_
