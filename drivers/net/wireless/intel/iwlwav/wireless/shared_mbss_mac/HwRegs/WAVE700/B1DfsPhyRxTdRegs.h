
/***********************************************************************************
File:				B1DfsPhyRxTdRegs.h
Module:				b1DfsPhyRxTd
SOC Revision:		latest
Generated at:       2022-05-04 10:57:34
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_DFS_PHY_RX_TD_REGS_H_
#define _B1_DFS_PHY_RX_TD_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_DFS_PHY_RX_TD_BASE_ADDRESS                             MEMORY_MAP_UNIT_10207_BASE_ADDRESS
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG00                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x10)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x14)
#define	REG_B1_DFS_PHY_RX_TD_MY_BAND_TYPE                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x18)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x28)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x30)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x34)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x38)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG10                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x40)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG11                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x44)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG12                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x48)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG13                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG15                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x54)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG16                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x58)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG17                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x5C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG18                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x60)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG19                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x64)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1A                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x68)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1B                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x6C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1C                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x70)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x74)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x78)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01F                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x7C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG021                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x84)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG022                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x88)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG023                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x8C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG024                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x90)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG025                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x94)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG026                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x98)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG027                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x9C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG028                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xA0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG029                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xA4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xA8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xAC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xB0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xB4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG2E                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xB8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG2F                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xBC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG30                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xC0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG31                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xC4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG032                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xC8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG033                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xCC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG035                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xD4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG036                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xD8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG037                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xDC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG038                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xE0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG039                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xE4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xE8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xEC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xF0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xF4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xF8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03F                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0xFC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG040                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x100)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG041                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x104)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG042                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x108)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG043                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x10C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG044                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x110)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG045                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x114)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG046                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x118)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG047                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x11C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG048                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x120)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG049                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x124)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x128)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x12C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x130)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x134)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG056                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x158)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG057                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x15C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG058                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x160)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG059                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x164)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x168)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x16C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x170)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x174)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x178)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_MSG                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x17C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_MSG_DONE                  (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x180)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_FAST_MSG                  (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x184)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG062                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x188)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG063                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x18C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG064                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x190)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG065                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x194)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG066                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x198)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG067                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x19C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG068                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1A0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG069                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1A4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1A8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1AC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1B0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG_LOOPBACK_CTRL                        (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1B4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1B8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06F                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1BC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG070                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1C0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG071                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1C4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG072                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1C8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG076                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1D8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG077                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1DC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_CONTROL                             (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1E0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1E4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1E8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1EC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_LOAD_VAL                            (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1F0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07D                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1F4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1F8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07F                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x1FC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG081                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x204)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG082                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x208)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG083                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x20C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG084                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x210)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG085                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x214)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG086                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x218)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG088                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x220)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG08B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x22C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG08C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x230)
#define	REG_B1_DFS_PHY_RX_TD_SIG3_READY_EHT                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x240)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG091                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x244)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG092                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x248)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG093                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x24C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG094                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x250)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG095                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x254)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG096                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x258)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG097                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x25C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG098                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x260)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG099                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x264)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09A                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x268)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09B                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x26C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09C                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x270)
#define	REG_B1_DFS_PHY_RX_TD_TD_DEBUG_CONTROL                                  (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x274)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09E                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x278)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09F                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x27C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x280)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x284)
#define	REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_HIGH_LOAD_VAL                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x288)
#define	REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_HIGH                                (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x28C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A4                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x290)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A5                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x294)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A6                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x298)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A7                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x29C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2A0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2A4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AA                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2A8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2AC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2B0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AD                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2B4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AE                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2B8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AF                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2BC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2C0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2C4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B2                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2C8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B3                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2CC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B4                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2D0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B7                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2DC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2E0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2E4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BA                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2E8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2EC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2F0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BD                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2F4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BE                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2F8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BF                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x2FC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x300)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x304)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C2                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x308)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C3                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x30C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C4                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x310)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C5                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x314)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C6                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x318)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C7                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x31C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x320)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x324)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CA                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x328)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x32C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x330)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CD                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x334)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CE                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x338)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CF                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x33C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x340)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x344)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D2                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x348)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D3                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x34C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D4                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x350)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D5                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x354)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D6                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x358)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D7                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x35C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x360)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x364)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DA                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x368)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x36C)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x370)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DD                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x374)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x380)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x384)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E2                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x388)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E5                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x398)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E6                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x39C)
#define	REG_B1_DFS_PHY_RX_TD_AFE_DYN_CTRL                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3A0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3A4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3A8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EA                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3AC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3B0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3B4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0ED                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3B8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EE                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3BC)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EF                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3C0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F0                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3C4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F1                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3C8)
#define	REG_B1_DFS_PHY_RX_TD_TB_BYTES_IN_FIFO_1                                (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3CC)
#define	REG_B1_DFS_PHY_RX_TD_TB_ACCESS_CONTROL                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3D0)
#define	REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_CONTROL                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3D4)
#define	REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_PACKET_COUNTER_LEVEL      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3D8)
#define	REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_CONTROL_1                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3DC)
#define	REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_PACKET_COUNTER_LEVEL_1    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3E0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F7                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3E4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F8                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3E8)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F9                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3EC)
#define	REG_B1_DFS_PHY_RX_TD_TEST_BUS_FIFO_CONFIG_ERROR                        (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3F0)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0FB                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3F4)
#define	REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0FC                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3F8)
#define	REG_B1_DFS_PHY_RX_TD_TEST_BUS_FIFO_POWER_CTRL                          (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x3FC)
#define	REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN0                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x400)
#define	REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN1                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x404)
#define	REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN2                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x408)
#define	REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN3                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x40C)
#define	REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN4                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x410)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN0                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x420)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN1                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x424)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN2                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x428)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN3                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x42C)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN4                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x430)
#define	REG_B1_DFS_PHY_RX_TD_SNR_EST_AN01                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x440)
#define	REG_B1_DFS_PHY_RX_TD_SNR_EST_AN23                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x444)
#define	REG_B1_DFS_PHY_RX_TD_SNR_EST_AN45                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x448)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN0                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x450)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN1                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x454)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN2                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x458)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN3                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x45C)
#define	REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN4                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x460)
#define	REG_B1_DFS_PHY_RX_TD_SIGNAL_DETECT_LOCK_EN                             (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x468)
#define	REG_B1_DFS_PHY_RX_TD_BAND_320                                          (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x46C)
#define	REG_B1_DFS_PHY_RX_TD_PM_IND_NOISE_CTRL                                 (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x470)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_FD0                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x474)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_FD1                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x478)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_TX                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x47C)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_BE                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x480)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_TO_FD0                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x484)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_TO_FD1                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x488)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_TO_TX                                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x48C)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_TO_BE                                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x490)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_TO_MAC                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x494)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_MAC                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x498)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_CPU                                    (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x49C)
#define	REG_B1_DFS_PHY_RX_TD_SPARE_FROM_HOST                                   (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4A0)
#define	REG_B1_DFS_PHY_RX_TD_RADAR_DETECTOR_TX_ON_OTHER_BAND_EN                (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4A4)
#define	REG_B1_DFS_PHY_RX_TD_TX_PER_BAND                                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4A8)
#define	REG_B1_DFS_PHY_RX_TD_GOERTZEL_MODE                                     (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4AC)
#define	REG_B1_DFS_PHY_RX_TD_DET_LPI_CTRL                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4B0)
#define	REG_B1_DFS_PHY_RX_TD_DET_LPI_THR0                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4B4)
#define	REG_B1_DFS_PHY_RX_TD_DET_LPI_THR1                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4B8)
#define	REG_B1_DFS_PHY_RX_TD_DET_LPI_THR2                                      (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4BC)
#define	REG_B1_DFS_PHY_RX_TD_EARLY_EHT_PPM_DELAY_CNT_VAL                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4C0)
#define	REG_B1_DFS_PHY_RX_TD_LAST_SAMPLE_OFFSET_REG1                           (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4C4)
#define	REG_B1_DFS_PHY_RX_TD_LAST_SAMPLE_OFFSET_REG2                           (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4C8)
#define	REG_B1_DFS_PHY_RX_TD_DFS_BLOCK_ACTIVE                                  (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4CC)
#define	REG_B1_DFS_PHY_RX_TD_EARLY_EHT_TRIG_MUX_DELAY_CNT_VAL                  (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4D0)
#define	REG_B1_DFS_PHY_RX_TD_GCLK_BYPASS                                       (B1_DFS_PHY_RX_TD_BASE_ADDRESS + 0x4D4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG00 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg : 32; //Sub blocks SW Reset, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg00_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 blockEn : 32; //Sub blocks Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg01_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg02 : 32; //Sub blocks SW Reset, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg02_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg03 : 32; //RxTD Hyperion Control Reg, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 scCyclicPeriod : 3; //sc cyclic period, reset value: 0x5, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg04_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bwMode : 2; //bw_mode, reset value: 0x2, access type: CONST
		uint32 hbMode : 2; //hb_mode, reset value: 0x0, access type: RW
		uint32 pmDetectorTriggeredMode : 1; //Detector Triggered based mode on, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pmAdjecentAnt : 2; //The adjecent channels choose from antenna ID, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmDetectorTriggeredMode2 : 1; //Detector Triggered based mode 2 on, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 digitalBw : 3; //maximum desired channel bandwidth , 1 - 40M, 2 - 80M, 3 - 160M, 4 - 320M - digital BW , reset value: 0x3, access type: RW
		uint32 rxAdcFs : 3; //no description, reset value: 0x3, access type: RW
		uint32 dpdAdcFs : 3; //no description, reset value: 0x3, access type: RW
		uint32 dftAdcFs : 3; //no description, reset value: 0x3, access type: RW
		uint32 rfBw : 3; //RF BW (previously hb_mode): , When the modem is statically configured to receive or transmit packet BW that is smaller than the maximum possible packet BW which was set by the digital_bw. It reflects the BW of the filter in the RF but also effect on digital chain. for example, RF_BW can be 80Mhz when the modem in configured to supports 160MHz. It also stands for the BW of the RF channel to which we are centralized. , The analog is not affected by the RF_BW, it keeps the same sample rate which is determine by the digital_bw. The RF has a different filter per RF_BW. , There is no RF_BW in digital_bw 320 , rf_bw[2:0] is the prog_model which tells which BW is used out of the maximum possible BW (digital_bw) , rf_bw[2:0]	4	3	2	1	0 , BW	320	160	80	40	20 , , reset value: 0x3, access type: RW
		uint32 lowPowerMode : 1; //TD works in low power mode, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05_u;

/*REG_B1_DFS_PHY_RX_TD_MY_BAND_TYPE 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 myBandType : 2; //my band type 0-2.4G, 1-5G, 2-6G, 3-DFS, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdMyBandType_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstEnResetAcc : 1; //Rx TD Noise Est Enable Reset Acc, reset value: 0x0, access type: CONST
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaTimingModeDelayOfdm : 16; //delay in clocks for an OFDM packet, reset value: 0xa00, access type: RW
		uint32 ccaTimingModeDelay11B : 16; //delay in clocks for a 11b packet, reset value: 0xa00, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detBypassMeanRemoval : 1; //Detection Mean Removal Bypass., reset value: 0x0, access type: RW
		uint32 detMrbDelayLineLength : 1; //Detection Mean Removal delay line length., reset value: 0x0, access type: RW
		uint32 detMrbMiddleTap : 3; //Detection Mean Removal middle tap., reset value: 0x0, access type: RW
		uint32 detMrbBpfMode : 1; //Detection Mean Removal BPF mode, reset value: 0x1, access type: RW
		uint32 reserved0 : 16;
		uint32 detCorrDL : 4; //Detection correlation-D low tap., reset value: 0x4, access type: RW
		uint32 detCorrDH : 4; //Detection correlation-D high tap., reset value: 0xb, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 detDiffMode : 1; //Detection correlation-64 differentiators Mode., reset value: 0x0, access type: RW
		uint32 detCntThForCorr16Size : 8; //Detection threshold for correlation-16 size., reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
		uint32 detFineErrCntInit : 11; //Detection fine Error Cnt Init., reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 detControlImd : 1; //Use common coarse rdy, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detFinePeakCntInit : 8; //Detection Fine timing Peak Cnt Init., reset value: 0x0, access type: RW
		uint32 detFineEnableDelay : 8; //Detection Fine timing Enable Delay., reset value: 0x0, access type: RW
		uint32 detFineDelaySize : 4; //Detection Fine frequency Delay Size., reset value: 0x0, access type: RW
		uint32 detFinePeakHeTrigMode2 : 8; //Detection Fine timing Peak Cnt Init in HE trigger based mode 2, reset value: 0x7, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detCoarsePeakCntInit : 8; //Detection Coarse timing Peak Cnt Init., reset value: 0x0, access type: RW
		uint32 detCoarseEnableDelay : 8; //Detection Coarse timing Enable Delay., reset value: 0x0, access type: RW
		uint32 detCoarseDelaySize : 5; //Detection Coarse frequency Delay Size., reset value: 0x0, access type: RW
		uint32 detCoarseErrCntInit : 11; //Detection coarse Error Cnt Init., reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG10 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTopCnt : 6; //Top Detection thershold register 2., reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 detTopTh3 : 15; //Top Detection thershold register 3., reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 detTopCntPri : 6; //Top Detection counter initialization., reset value: 0x0, access type: RW
		uint32 detTopMode : 2; //Top Detection mode., reset value: 0x3, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg10_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG11 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTopBlockCnt : 12; //Top Detection block counter., reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
		uint32 detMaDec : 4; //Rx TD ma_dec., reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg11_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG12 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htShortOffset : 16; //ht short offset, reset value: 0x4600, access type: RW
		uint32 lastSampleOffset : 16; //last sample offset, reset value: 0x2800, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg12_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG13 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaEnergy01Delay : 12; //delay in clocks of CCA rise event, reset value: 0x0, access type: RW
		uint32 pmCcaEnergy10Delay : 12; //delay in clocks of CCA fall event, reset value: 0x0, access type: RW
		uint32 pmCcaPifsHistClr : 1; //indicates wether LSB is the control channel (1 - LSB,0 - USB), reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg13_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG15 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctrlChannelCnt : 32; //counter for the control channel, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg15_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG16 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extChannelCnt : 32; //counter for the extension channel, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg16_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG17 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extChannelCnt40 : 32; //cca statistics counter: saturated count of secondary 40MHz cca activation cycles since last stats_reset event, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg17_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG18 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 adcActiveReg : 1; //Rx TD Adc Active., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg18_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG19 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 blockAuxChannelSec : 1; //block_aux_channel_sec, reset value: 0x0, access type: RW
		uint32 lsbIsCtrlChannelSec : 1; //lsb_is_ctrl_channel_sec, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg19_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1A 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bandSelectDelay : 8; //MU Antenna Enable. , Host antenna enable bits , can be changed after risc processing., reset value: 0x0, access type: RW
		uint32 bandSelectDelay2B : 8; //Antenna Enable. , Host antenna enable bits , can be changed after risc processing., reset value: 0x0, access type: RW
		uint32 bandSelectDelay4B : 8; //Band Select Delay, reset value: 0x0, access type: RW
		uint32 bandSelectDelay8B : 8; //Band Select Delay 2 Band, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg1A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1B 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stmAbortThr : 16; //Stm Abort Threshold, reset value: 0xfff0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg1B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG1C 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 pmForceDcEstMode : 2; //pm_force_dc_est_mode, reset value: 0x0, access type: RW
		uint32 pmAllowFallbackToDcAvgIfHtstfNotValid : 1; //pm_allow_fallback_to_dc_avg_if_htstf_not_valid, reset value: 0x0, access type: RW
		uint32 pmEnableBinErasing : 1; //pm_enable_bin_erasing, reset value: 0x0, access type: RW
		uint32 bypassDcAll : 1; //Bypass Dc Removal, reset value: 0x0, access type: RW
		uint32 pmAllowFallbackToDcAvgIfHtstfValid : 1; //pm_allow_fallback_to_dc_avg_if_htstf_valid, reset value: 0x0, access type: RW
		uint32 pmAllowFallbackToDcGzlIfHtstfValid : 1; //pm_allow_fallback_to_dc_gzl_if_htstf_valid, reset value: 0x0, access type: RW
		uint32 pmAllowFallbackToDcGzlIfHtstfNotValid : 1; //pm_allow_fallback_to_dc_gzl_if_htstf_not_valid, reset value: 0x0, access type: RW
		uint32 enableDcEstSsbDecision : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg1C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01D 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiSamplePolarity : 1; //Rssi Sample Polarity., reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 rssiStrbRate : 1; //ESSI strobe threshold: , 0 : for 40Mhz , 1: for 20Mhz, reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg01D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01E 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 parallelMode : 1; //Rx TD Hyp Pif Parallel Mode., reset value: 0x0, access type: CONST
		uint32 reserved1 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg01E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG01F 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deltaFCmpEn : 1; //Delta F Cmp En., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 deltaFFineErrEnable : 1; //Delta F fine error enable., reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 coarseFreqWeight : 4; //coarse_freq_weight, reset value: 0x0, access type: RW
		uint32 reserved2 : 20;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg01F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG021 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 riscPage : 1; //RIsc Page Address., reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg021_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG022 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscStartOp : 1; //Rx TD RIsc Start Opcode., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg022_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG023 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestPlugLenAn0 : 25; //Test Plug Local Enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg023_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG024 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestPlugLenAn1 : 25; //Test Plug Local Enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg024_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG025 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestPlugLenAn2 : 25; //Test Plug Local Enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg025_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG026 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestPlugLenAn3 : 25; //Test Plug Local Enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg026_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG027 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetGenerate : 32; //Rx TD Sw Reset Generate, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg027_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG028 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmSwResetMask : 32; //Rx TD Gsm Sw Reset., reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg028_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG029 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxRdyRiseDelay11A : 14; //rx_rdy_rise_delay_11a, reset value: 0x1180, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg029_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02A 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxRdyRiseDelay11N : 14; //rx_rdy_rise_delay_11n, reset value: 0x2A80, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg02A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02B 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxRdyFallDelay : 14; //rx_rdy_fall_delay, reset value: 0x2080, access type: RW
		uint32 reserved0 : 2;
		uint32 rxRdyFallDelay11B : 14; //rx_rdy_fall_delay, reset value: 0x1180, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg02B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02C 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stopSignalTh : 13; //stop_signal_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg02C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG02D 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extendAirTimeOffset : 16; //extend_air_time_offset, reset value: 0x0, access type: RW
		uint32 extendAirTimeOffsetEn : 1; //extend_air_time_offset_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg02D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG2E 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestBusGclkEn : 1; //td_test_bus_gclk_en , reset value: 0x0, access type: RW
		uint32 beTestBusGclkEn : 1; //be_test_bus_gclk_en , reset value: 0x0, access type: RW
		uint32 slicerTestBusGclkEn : 1; //slicer_test_bus_gclk_en , reset value: 0x0, access type: RW
		uint32 fdTestBusGclkEn : 1; //fd_test_bus_gclk_en , reset value: 0x0, access type: RW
		uint32 txTestBusGclkEn : 1; //tx_test_bus_gclk_en , reset value: 0x0, access type: RW
		uint32 loggerTestBusGclkEn : 1; //logger_test_bus_gclk_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 swBistStart : 1; //sw_bist_start, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 clearRamMode : 1; //clear_ram_mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 19;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg2E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG2F 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gclkBypassEn : 32; //gclk bypass - when '1' clocks will be always active, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg2F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG30 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tbFifoClear : 1; //Test Bus FIFO Clear first fifo, reset value: 0x0, access type: WO
		uint32 tbFifoClear1 : 1; //Test Bus FIFO Clear second fifo, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg30_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG31 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoCaptureMode : 1; //test_fifo_capture_mode, reset value: 0x0, access type: RW
		uint32 testFifoCyclicMode : 1; //test_fifo_cyclic_mode, reset value: 0x0, access type: RW
		uint32 testFifoExternalTrigEn : 1; //test_fifo_external_trig_en, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigEn : 1; //test_fifo_internal_trig_en, reset value: 0x0, access type: RW
		uint32 testFifoCaptureMode1 : 1; //test_fifo_capture_mode second fifo, reset value: 0x0, access type: RW
		uint32 testFifoCyclicMode1 : 1; //test_fifo_cyclic_mode second fifo, reset value: 0x0, access type: RW
		uint32 testFifoExternalTrigEn1 : 1; //test_fifo_external_trig_en second fifo, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigEn1 : 1; //test_fifo_internal_trig_en second fifo, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg31_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG032 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genRiscSramRm : 3; //gen_risc_sram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 genRiscIramRm : 3; //gen_risc_iram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
		uint32 memConverterRm : 3; //mem_converter_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 testFifoRamRm : 3; //test_fifo_large_ram_low_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg032_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG033 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcTableRm : 3; //dc_table_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 dcTable2Rm : 3; //dc_table2_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 agcTableRm : 3; //agc_table_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 21;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg033_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG035 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcCalcDecisionParams : 32; //dc_calc_decision_params, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg035_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG036 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bistScrBypass : 1; //bist_scr_bypass, reset value: 0x0, access type: RW
		uint32 ramTestMode : 2; //ram_test_mode, reset value: 0x0, access type: RW
		uint32 memGlobalRm : 2; //mem_global_rm, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg036_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG037 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorMrbRm : 3; //detector_mrb_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 detectorC16Rm : 3; //detector_c16_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 detectorC64Rm : 3; //detector_c64_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 detectorCfbRm : 3; //detector_cfb_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 detectorFfbRm : 3; //detector_ffb_rm, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 detectorLpiRm : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 9;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg037_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG038 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disableSecDetection : 1; //disable_sec_detection, reset value: 0x0, access type: RW
		uint32 disablePrimaryDetection : 1; //disable_primary_detection, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 detectionOnLltfBackoff : 4; //detection_on_lltf_backoff, reset value: 0x4, access type: RW
		uint32 pmDetectOnLtfCorrDDisable : 1; //pm_detect_on_ltf_corr_d_disable, reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg038_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG039 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 watchDogLimit : 32; //watch dog limit, reset value: 0xffffff, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg039_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03A 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 adjacentRejOnSecEn : 1; //adjacent_rej_on_sec_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 adjacentRejOnOobEn : 1; //adjacent_rej_on_oob_en, reset value: 0x1, access type: RW
		uint32 reserved1 : 3;
		uint32 powerDivOobTh : 11; //power_div_oob_th, reset value: 0x10, access type: RW
		uint32 reserved2 : 1;
		uint32 powerDivIbTh : 11; //power_div_ib_th, reset value: 0x64, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03B 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 midPacketEn : 1; //cca_mid_long_dec, reset value: 0x1, access type: RW
		uint32 reserved0 : 7;
		uint32 midPacketAnt : 2; //mid_packet_ant, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 pmCcaCombineSec20 : 1; //cca combine Secondary 20 in Primary, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 pmCcaMidDetThTime : 14; //cca_mid_det_energy_pass_th debounce time, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03C 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 midDcIirIsBypassed : 1; //mid_dc_iir_is_bypassed , reset value: 0x0, access type: RW
		uint32 midOobDcIirIsBypassed : 1; //mid_oob_dc_iir_is_bypassed , reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 midDcIirAlpha : 4; //mid_dc_iir_alpha , reset value: 0x7, access type: RW
		uint32 ccaSecMidPowerTh : 16; //cca_sec_mid_power_th, reset value: 0x19, access type: RW
		uint32 ccaMidMaxThPasses : 7; //cca_mid_max_th_passes, reset value: 0x7f, access type: RW
		uint32 reserved1 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03D 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaMidTh20Sec : 8; //cca mid power threshold Secondary 20MHz , reset value: 0xB8, access type: RW
		uint32 reserved0 : 4;
		uint32 pmCcaMidTh40 : 8; //cca mid power threshold Secondary 40MHz , reset value: 0xB8, access type: RW
		uint32 reserved1 : 4;
		uint32 pmCcaMidTh20Prim : 8; //cca mid power threshold Primary 20MHz , reset value: 0xB8, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03E 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaPifs1TimeLen : 15; //cca PIFS1 time length (clock cycles), reset value: 0x1f40, access type: RW
		uint32 reserved0 : 1;
		uint32 pmCcaWifiDetSecTime : 15; //CCA Secondary WiFi Detector Fall Hysteresis (on-shot)  of 24us, reset value: 0x1e00, access type: RW
		uint32 pmCcaWifiDetSecEnable : 1; //CCA enable OFDM WiFi Detector of the secondary bands, reset value: 0x1, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG03F 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaMpdDramRm : 3; //cca_mpd_dram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 ccaMpdSramRm : 3; //cca_mpd_sram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg03F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG040 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftExtraGain20 : 2; //fft_extra_gain, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 fftExtraGain40 : 2; //fft_extra_gain, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 fftExtraGain80 : 2; //fft_extra_gain, reset value: 0x0, access type: RW
		uint32 fftExtraGain160 : 2; //fft_extra_gain_160, reset value: 0x0, access type: RW
		uint32 reserved2 : 20;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg040_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG041 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaPifs2Start : 4; //CCA PIFS2 detection time start (us), reset value: 0x8, access type: RW
		uint32 pmCcaPifs2End : 6; //CCA PIFS2 detection time n end (us), reset value: 0x21, access type: RW
		uint32 reserved0 : 2;
		uint32 pmCcaReleasePifs2 : 1; //release pifs2 freeze, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg041_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG042 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaEnergy10DelayPrim : 14; //cca_energy_1_0_delay_prim, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg042_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG043 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 decisionMask : 8; //det_th_1ant_pri, reset value: 0xf, access type: RW
		uint32 detCombTh : 2; //det_th_1ant_pri, reset value: 0x3, access type: RW
		uint32 coChMode : 1; //det_th_1ant_pri, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 coChRestartLen : 10; //det_th_1ant_pri, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 detCombTh8Bands : 3; //det_th_1ant_pri, reset value: 0x3, access type: RW
		uint32 reserved2 : 5;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg043_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG044 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectionMask0 : 8; //detecttiom mask, reset value: 0xf, access type: RW
		uint32 detectionMask1 : 8; //detecttiom mask, reset value: 0x0, access type: RW
		uint32 detectionMask2 : 8; //detecttiom mask, reset value: 0x0, access type: RW
		uint32 detectionMask3 : 8; //detecttiom mask, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg044_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG045 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh1AntPri : 8; //det_th_1ant_pri, reset value: 0x0, access type: RW
		uint32 detTh2AntPri : 8; //det_th_2ant_pri, reset value: 0x0, access type: RW
		uint32 detTh3AntPri : 8; //det_th_3ant_pri, reset value: 0x0, access type: RW
		uint32 detTh4AntPri : 8; //det_th_4ant_pri, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg045_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG046 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh1AntSec : 8; //det_th_1ant_sec, reset value: 0x0, access type: RW
		uint32 detTh2AntSec : 8; //det_th_2ant_sec, reset value: 0x0, access type: RW
		uint32 detTh3AntSec : 8; //det_th_3ant_sec, reset value: 0x0, access type: RW
		uint32 detTh4AntSec : 8; //det_th_4ant_sec, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg046_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG047 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSecMidDetTh : 8; //cca_sec_mid_det_th, reset value: 0xc0, access type: RW
		uint32 ccaSecMidDetShortTh : 8; //cca_sec_mid_det_short_th, reset value: 0xcd, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg047_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG048 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr0 : 32; //spare_gpr_0, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg048_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG049 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr1 : 32; //spare_gpr_1, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg049_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04A 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr2 : 32; //spare_gpr_2, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg04A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04B 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareGpr3 : 32; //spare_gpr_3, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg04B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04C 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detCoarseCI : 22; //det_coarse_c_i, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg04C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG04D 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detCoarseCQ : 22; //det_coarse_c_q, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg04D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG056 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 thresholdMaxCorr : 12; //threshold_max_corr, reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg056_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG057 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 thresholdMaxCorrAdj160 : 12; //threshold_max_corr_adj_160, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
		uint32 thresholdMaxCorrAdj80 : 12; //threshold_max_corr_adj_80, reset value: 0x1, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg057_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG058 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 thresholdMaxCorrAdj20 : 12; //threshold_max_corr_adj_20, reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg058_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG059 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 thresholdMaxCorrAdj40 : 12; //threshold_max_corr_adj_40, reset value: 0x1, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg059_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05A 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 detFineTimingContinueAfterRdy : 8; //det_fine_timing_continue_after_rdy, reset value: 0x1, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05B 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh1AntCoCh : 8; //det_th_1ant_co_ch, reset value: 0x0, access type: RW
		uint32 detTh2AntCoCh : 8; //det_th_2ant_co_ch, reset value: 0x0, access type: RW
		uint32 detTh3AntCoCh : 8; //det_th_3ant_co_ch, reset value: 0x0, access type: RW
		uint32 detTh4AntCoCh : 8; //det_th_4ant_co_ch, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05C 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sig3ReadyOffset : 16; //offset in clk from ht short to sig3 ready, reset value: 0x1f5e, access type: RW
		uint32 sig3ReadyOffsetHeTrig : 16; //offset in clk from ht short to sig3 ready he trig, reset value: 0x295e, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05D 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCnt : 32; //free_running_cnt, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG05E 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaValidDetTimer : 8; //timer of 1.25us in 20Mhz from sec detect till primary, reset value: 0x32, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg05E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_MSG 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscTestPlugMsg : 32; //risc data to logger, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdRiscTestPlugMsg_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_MSG_DONE 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscTestPlugMsgDone : 1; //risc data to logger, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdRiscTestPlugMsgDone_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_RISC_TEST_PLUG_FAST_MSG 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscTestPlugFastMsg : 32; //risc data to logger, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdRiscTestPlugFastMsg_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG062 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 forceDetectionOnBands : 8; //force_detection_on_bands, reset value: 0x0, access type: RW
		uint32 blockDetectionOnBands : 8; //block_detection_on_bands, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg062_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG063 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn0Corr64AtCommonFineT : 28; //Detector an0 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg063_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG064 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn1Corr64AtCommonFineT : 28; //Detector an1 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg064_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG065 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn2Corr64AtCommonFineT : 28; //Detector an2 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg065_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG066 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn3Corr64AtCommonFineT : 28; //Detector an3 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg066_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG067 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsCommonCorr64AtCommonFineT : 28; //Detector common Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg067_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG068 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn0Corr16AtLocalCoarseT : 20; //Detector an0 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg068_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG069 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn1Corr16AtLocalCoarseT : 20; //Detector an1 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg069_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06A 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn2Corr16AtLocalCoarseT : 20; //Detector an2 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg06A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06B 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn3Corr16AtLocalCoarseT : 20; //Detector an3 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg06B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06C 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsCommonCorr16AtCommonCoarseT : 20; //Detector common Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg06C_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG_LOOPBACK_CTRL 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 digitalLpbk : 1; //digital LoopBack Mode, reset value: 0x0, access type: RW
		uint32 reversedDigitalLpbk : 1; //reveresed digital LoopBack Mode, reset value: 0x0, access type: RW
		uint32 reversedDigitalLpbkRssi : 1; //reversed LoopBack Mode rssi, reset value: 0x0, access type: RW
		uint32 tx0LpbkSelect : 1; //Tx0 LoopBack Select, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdRegLoopbackCtrl_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06E 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpTimingEnable : 1; //cp_timing_enable, reset value: 0x1, access type: RW
		uint32 cpTimingSkipTimingFromSig : 2; //cp_timing_skip_timing_from_sig, reset value: 0x1, access type: RW
		uint32 cpTimingSkipTimingFromLtfs : 2; //cp_timing_skip_timing_from_ltfs, reset value: 0x2, access type: RW
		uint32 cpTimingSkipTimingFromData : 2; //cp_timing_skip_timing_from_data, reset value: 0x2, access type: RW
		uint32 reserved0 : 1;
		uint32 cpTimingNSymToAvg : 7; //cp_timing_n_sym_to_avg, reset value: 0xa, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg06E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG06F 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpTimingWindowSize : 5; //cp_timing_window_size, reset value: 0x14, access type: RW
		uint32 cpTimingMinimalLtfsForEstimation : 3; //cp_timing)minimal_ltfs_for_estimation, reset value: 0x0, access type: RW
		uint32 cpTimingSync : 6; //cp_timing_sync, reset value: 0x1a, access type: RW
		uint32 reserved0 : 2;
		uint32 cpTimingWindowSizeForSigEstimation : 5; //cp_timing_window_size_for_sig_estimation, reset value: 0x14, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg06F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG070 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpTimingShiftFromLookahead : 6; //cp_timing_shift_from_lookahead, reset value: 0x38, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg070_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG071 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 m11BBLongOffset : 20; //m11b_b_long_offset, reset value: 0x1E6C0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg071_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG072 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTopTh3ForLtf : 15; //Top Detection thershold register 3 for ltf detection., reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg072_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG076 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaPacketCounter : 27; //cca_packet_counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg076_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG077 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detCoarseGclkEnCb : 1; //det_coarse_gclk_en_cb, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg077_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_CONTROL 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTsfTimerEn : 1; //phy_tsf_timer_en, reset value: 0x0, access type: RW
		uint32 phyTsfTimerClear : 1; //phy_tsf_timer_clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
		uint32 phyTsfTimerUnit : 10; //phy_tsf_timer_unit, reset value: 0x280, access type: RW
		uint32 reserved1 : 2;
		uint32 phyTsfTimerLoad : 1; //phy_tsf_timer_load, reset value: 0x0, access type: WO
		uint32 reserved2 : 15;
	} bitFields;
} RegB1DfsPhyRxTdPhyTsfTimerControl_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTsfTimer : 32; //phy_tsf_timer, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyTsfTimer_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07A 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaPdTh20Prim : 8; //CCA primary 20MHz power threshold value, reset value: 0xC2, access type: RW
		uint32 pmCcaPdTh20Sec : 8; //CCA Secondary 20MHz power threshold value, reset value: 0xC2, access type: RW
		uint32 pmCcaPdTh40 : 8; //CCA Secondary 40MHz power threshold value, reset value: 0xC5, access type: RW
		uint32 pmCcaPdTh80 : 8; //CCA Secondary 80MHz power threshold value, reset value: 0xC5, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg07A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07B 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostAntennaEn : 5; //Antenna Enable. , Host antenna enable bits , can be changed after risc processing., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg07B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_LOAD_VAL 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTsfTimerLoadVal : 32; //init value, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyTsfTimerLoadVal_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07D 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lsbIsCtrlChannel : 1; //delay in clocks of CCA rise event, reset value: 0x0, access type: RW
		uint32 lsb40IsCtrlChannel : 1; //delay in clocks of CCA fall event, reset value: 0x0, access type: RW
		uint32 lsb80IsCtrlChannel : 1; //indicates wether LSB is the control channel (1 - LSB,0 - USB), reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 blockAuxChannel : 1; //determines which 40m band is the primary (1 - LSB,0 - USB), reset value: 0x0, access type: RW
		uint32 blockAuxChannel40 : 1; //block_aux_channel, reset value: 0x0, access type: RW
		uint32 blockAuxChannel80 : 1; //block_aux_channel40, reset value: 0x0, access type: RW
		uint32 lsb160IsCtrlChannel : 1; //indicates if 160 Lower sub band (LSB) is the control channel (1 - LSB,0 - USB), reset value: 0x0, access type: RW
		uint32 reserved1 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg07D_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07E 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmDetSorpDelay : 8; //detecttion_s_or_p delay indication - starts the C64 process. For CSIm calibration, reset value: 0x0, access type: RW
		uint32 pmDetCt2FtDelay : 8; // coarse-timing to start fine-timing delay. For CSIM calibration, reset value: 0x0, access type: RW
		uint32 pmDetC16RestartDelay : 8; //delay from coarse-timing to C16 restart. For CSIM calibration, reset value: 0x0, access type: RW
		uint32 pmDetCnt2DetCalibrate : 4; //delay the detect-ready assert for CSIM vs RTL bit-acc, reset value: 0x0, access type: RW
		uint32 pmDetDisableMaskChAfterRdy : 1; //disable the feature that mask the channels after detection_ready., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg07E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG07F 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 thresholdMaxCorrCoChannel20 : 12; //threshold_max_corr_co_channel_20, reset value: 0x1, access type: RW
		uint32 thresholdMaxCorrCoChannel40 : 12; //threshold_max_corr_co_channel_40, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg07F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG081 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneDft : 1; //end of trigger frame transmit, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg081_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG082 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 triggerBasedCnt : 16; //count from end of tx trigger frame to start of he mu rx, reset value: 0x26C0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg082_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG083 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delayCoarseReadyForDcStart : 6; //delay coarse indication for dc calculation, reset value: 0xD, access type: RW
		uint32 reserved0 : 2;
		uint32 detCoarseOffsetHeTrigMode2 : 6; //det_coarse_offset_he_trig_mode_2, reset value: 0x7, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg083_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG084 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minNoiseEst : 31; //Rx TD Min Noise Estimation, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg084_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG085 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dcCfoThresholdRm : 3; //dc_cfo_threshold_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg085_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG086 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSnrThrForDc0 : 8; //snr threshold for dc, reset value: 0x40, access type: RW
		uint32 pmSnrThrForDc1 : 8; //snr threshold for dc, reset value: 0x60, access type: RW
		uint32 pmSnrThrForDc2 : 8; //snr threshold for dc, reset value: 0x85, access type: RW
		uint32 pmSnrThrForDc3 : 8; //snr threshold for dc, reset value: 0xBA, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg086_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG088 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cdbAntSel : 5; //cdb_ant_sel, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg088_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG08B 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deltaFDataReg : 20; //Sanity Delta F Data, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg08B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG08C 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 accumulatorResetValue : 18; //accumulator_reset_value, reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg08C_u;

/*REG_B1_DFS_PHY_RX_TD_SIG3_READY_EHT 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sig3ReadyOffsetEht : 16; //no description, reset value: 0x2e5e, access type: RW
		uint32 sig3ReadyOffsetEhtTrig : 16; //no description, reset value: 0x295e, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdSig3ReadyEht_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG091 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusFifoConfig : 16; //1 = Use memory , 0 = memory disconnected , [0..7] - FIFO 0..7 , [8..11] - SU BF 0..3 , [12..15] - SU BF 0..3  , B0:0x0FFF , B1:0xF0FF , B2:0xFFFF, reset value: 0x0, access type: RW
		uint32 testBusFifoConfig1 : 8; //1 = Use memory  , 0 = memory disconnected , [0..7] - FIFO 0..7, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg091_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG092 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 m11BBShortOffset : 20; //m11b_b_short_offset, reset value: 0xF6C0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg092_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG093 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sig3ReadyOffsetHe : 16; //offset in clk from ht short to he sig3 ready, reset value: 0x295e, access type: RW
		uint32 sig3ReadyOffsetHeExt : 16; //offset in clk from ht short to he sig3 ready ext, reset value: 0x3d5e, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg093_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG094 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heShortOffset : 16; //he short offset, reset value: 0x5000, access type: RW
		uint32 heShortOffsetExt : 16; //he short offset ext, reset value: 0x6400, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg094_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG095 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAcidetAciAntenna : 3; //pm_acidet_aci_antenna, reset value: 0x0, access type: RW
		uint32 pmAcidetAciLocal : 1; //pm_acidet_aci_local, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg095_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG096 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tbBytesInFifo : 19; //tb bytes in fifo first fifo, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
		uint32 loggerTdNextSeqAlowed : 1; //logger_td_next_seq_alowed, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg096_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG097 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaBusySwPrim20 : 1; //pm_cca_busy_sw_prim20, reset value: 0x0, access type: RW
		uint32 pmCcaBusySwSec20 : 1; //pm_cca_busy_sw_sec20, reset value: 0x0, access type: RW
		uint32 pmCcaBusySwSec40 : 1; //pm_cca_busy_sw_sec40 , reset value: 0x0, access type: RW
		uint32 pmCcaBusySwSec80 : 1; //pm_cca_busy_sw_sec80 , reset value: 0x0, access type: RW
		uint32 pmCcaBusySw20Sb : 8; //pm_cca_busy_sw_20sb , reset value: 0x0, access type: RW
		uint32 pmCcaSelBusyHw : 1; //pm_cca_sel_busy_hw, reset value: 0x0, access type: RW
		uint32 pmCcaSecBusySw20Sb : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 11;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg097_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG098 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmCcaClearSw : 1; //pm_cca_clear_sw, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pmCcaClearHwEn : 1; //clear hw enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg098_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG099 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusFifoDumpLast : 1; //write last samples to the fifo, reset value: 0x0, access type: WO
		uint32 testBusFifoDumpLast1 : 1; //write last samples to the fifo 1, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg099_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09A 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmDetTxShortPreamCnt : 16; //The count from reset till TX-Preamble internal generation, reset value: 0x1e8, access type: RW
		uint32 pmDetTxShortPreamByp : 1; //bypass TX-Preamble and generate it internally , reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pmDetRcrBwBypass : 1; //pm_det_rcr_bw_bypass , reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 pmDetRcrBwValue : 2; //pm_det_rcr_bw_value , reset value: 0x0, access type: RW
		uint32 reserved2 : 6;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg09A_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09B 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSnrUpdate : 1; //pm_snr_update, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg09B_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09C 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disableFreqEst : 1; //disable_freq_est, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg09C_u;

/*REG_B1_DFS_PHY_RX_TD_TD_DEBUG_CONTROL 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftFifoHostAntSel : 4; //select which antenna is chosen, reset value: 0x0, access type: RW
		uint32 fftFifoHostSel : 1; //fft (1) or td_fifo (0), reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdTdDebugControl_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09E 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugSdSel : 3; //test_plug_sd_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg09E_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG09F 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugFifoSel : 2; //test_plug_fifo_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg09F_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A0 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearHeTrigIndication : 1; //clear_he_trig_indication, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A1 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainOptStartPoint : 11; //hestf_fft_gain_opt_start_point, reset value: 0x280, access type: RW
		uint32 reserved0 : 1;
		uint32 hestfFftGainOptEndPoint : 11; //hestf_fft_gain_opt_end_point, reset value: 0x300, access type: RW
		uint32 reserved1 : 9;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_HIGH_LOAD_VAL 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTsfTimerHighLoadVal : 8; //init high value, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyTsfTimerHighLoadVal_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_TSF_TIMER_HIGH 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTsfTimerHigh : 8; //tsf timer bits 39:32, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyTsfTimerHigh_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A4 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold0Tmpd : 32; //Detector Threshold0 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A4_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A5 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold1Tmpd : 32; //Detector Threshold1 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A5_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A6 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold2Tmpd : 32; //Detector Threshold2 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A6_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A7 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold3Tmpd : 32; //Detector Threshold3 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A7_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A8 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold4Tmpd : 32; //Detector Threshold4 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0A9 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold5Tmpd : 32; //Detector Threshold5 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0A9_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AA 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold6Tmpd : 32; //Detector Threshold6 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Aa_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AB 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold7Tmpd : 32; //Detector Threshold7 tmpD register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ab_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AC 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold0Tmpr : 32; //Detector Threshold0 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ac_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AD 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold1Tmpr : 32; //Detector Threshold1 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ad_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AE 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold2Tmpr : 32; //Detector Threshold2 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ae_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0AF 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold3Tmpr : 32; //Detector Threshold3 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Af_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B0 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold4Tmpr : 32; //Detector Threshold4 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B1 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold5Tmpr : 32; //Detector Threshold5 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B2 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold6Tmpr : 32; //Detector Threshold6 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B2_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B3 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThreshold7Tmpr : 32; //Detector Threshold7 tmpR register (32 bits msb out of 36 bits) for GenRisc Radar detection , reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B3_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B4 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsDetThresholdsLock : 1; //lock coD and corR registers, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B4_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B7 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 freeRunningCnt2 : 32; //free_running_cnt_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B7_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B8 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmDetMaxCorrProtect : 16; //protect creteria-C in ofdm-detectoer on max_corr during noise, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0B9 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heShortOffsetExtInit : 16; //he short offset ext init, reset value: 0x5700, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0B9_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BA 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBlockDetectionAdjBandEnable : 1; //pm_block_detection_adj_band_enable, reset value: 0x0, access type: RW
		uint32 pmBlockDetectionAdjBandThDb : 12; //pm_block_detection_adj_band_th_db, reset value: 0x0, access type: RW
		uint32 pmBlockDetectionMask : 10; //pm_block_detection_mask, reset value: 0x0, access type: RW
		uint32 pmDfsAntMode : 1; //pm_dfs_ant_mode, reset value: 0x0, access type: RW
		uint32 pmDelayAfterReset : 8; //pm_delay_after_reset, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ba_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BB 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSystemGain1 : 7; //pm_system_gain_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmSystemGain2 : 7; //pm_system_gain_2, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmSystemGain3 : 7; //pm_system_gain_3, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmSystemGain4 : 7; //pm_system_gain_4, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Bb_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BC 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSystemGain5 : 7; //pm_system_gain_5, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmSystemGain6 : 7; //pm_system_gain_6, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmSystemGain7 : 7; //pm_system_gain_7, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmSystemGain8 : 7; //pm_system_gain_8, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Bc_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BD 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarDetectionThDb1 : 7; //pm_radar_detection_th_db_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmRadarDetectionThDb2 : 7; //pm_radar_detection_th_db_2, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmRadarDetectionThDb3 : 7; //pm_radar_detection_th_db_3, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmRadarDetectionThDb4 : 7; //pm_radar_detection_th_db_4, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Bd_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BE 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarDetectionThDb5 : 7; //pm_radar_detection_th_db_5, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmRadarDetectionThDb6 : 7; //pm_radar_detection_th_db_6, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmRadarDetectionThDb7 : 7; //pm_radar_detection_th_db_7, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmRadarDetectionThDb8 : 7; //pm_radar_detection_th_db_8, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Be_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0BF 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarDetectionMaxThDb1 : 7; //pm_radar_detection_max_th_db_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmRadarDetectionMaxThDb2 : 7; //pm_radar_detection_max_th_db_2, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmRadarDetectionMaxThDb3 : 7; //pm_radar_detection_max_th_db_3, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmRadarDetectionMaxThDb4 : 7; //pm_radar_detection_max_th_db_4, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Bf_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C0 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarDetectionMaxThDb5 : 7; //pm_radar_detection_max_th_db_5, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmRadarDetectionMaxThDb6 : 7; //pm_radar_detection_max_th_db_6, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 pmRadarDetectionMaxThDb7 : 7; //pm_radar_detection_max_th_db_7, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 pmRadarDetectionMaxThDb8 : 7; //pm_radar_detection_max_th_db_8, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C1 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEnergyIncreaseTh1 : 9; //pm_energy_increase_th_1, reset value: 0x0, access type: RW
		uint32 pmEnergyIncreaseTh2 : 9; //pm_energy_increase_th_2, reset value: 0x0, access type: RW
		uint32 pmEnergyDecreaseTh1 : 9; //pm_energy_decrease_th_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C2 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEnergyDecreaseTh2 : 9; //pm_energy_decrease_th_2, reset value: 0x0, access type: RW
		uint32 pmEnergyIncreaseRecalcQuiet : 9; //pm_energy_increase_recalc_quiet, reset value: 0x0, access type: RW
		uint32 pmEnergyDecreaseRecalcQuiet : 9; //pm_energy_decrease_recalc_quiet, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C2_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C3 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEnergyIncreaseDurTh1 : 4; //pm_energy_increase_dur_th_1, reset value: 0x0, access type: RW
		uint32 pmEnergyDecreaseDurTh1 : 7; //pm_energy_decrease_dur_th_1, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 pmEnergyIncreaseDurTh2 : 4; //pm_energy_increase_dur_th_2, reset value: 0x0, access type: RW
		uint32 pmEnergyDecreaseDurTh2 : 7; //pm_energy_decrease_dur_th_2, reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C3_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C4 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEnergyPacketDiffSamples : 8; //pm_energy_packet_diff_samples, reset value: 0x0, access type: RW
		uint32 pmEnergyPacketDiffPercentages : 8; //pm_energy_packet_diff_percentages, reset value: 0x0, access type: RW
		uint32 pmMinSamplesEnergyPacketDiffPercentages : 8; //pm_min_samples_energy_packet_diff_percentages, reset value: 0x0, access type: RW
		uint32 pmUnknownInterferenceMinDuration : 4; //pm_unknown_interference_min_duration, reset value: 0x0, access type: RW
		uint32 pmUnknownInterferenceMaxDuration : 4; //pm_unknown_interference_max_duration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C4_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C5 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarMaxDuration : 12; //pm_radar_max_duration, reset value: 0x0, access type: RW
		uint32 pmQuietEnergyWindowSize : 8; //pm_quiet_energy_window_size, reset value: 0x0, access type: RW
		uint32 pmSumAutoCorrMask : 3; //pm_mask_autocorr_sum, reset value: 0x0, access type: RW
		uint32 pmAutoCorrWindowSize : 4; //pm_autocorr_window_size, reset value: 0x0, access type: RW
		uint32 radarDetectorGenriscForceStartStopEnable : 1; //radar_detector_genrisc_force_start_stop_enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C5_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C6 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmBlockDetectionAdjBandDiffSamples : 8; //pm_block_detection_adj_band_diff_samples, reset value: 0x0, access type: RW
		uint32 pmBlockDetectionAdjBandDiffPercentages : 8; //pm_block_detection_adj_band_diff_percentages, reset value: 0x0, access type: RW
		uint32 pmDelayAfterBlockDetection : 8; //pm_delay_after_block_detection, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C6_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C7 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEnableDetectionSelfChipTx : 1; //pm_enable_detection_self_chip_tx, reset value: 0x0, access type: RW
		uint32 pmBlockDetectionSelfChipTx : 1; //pm_block_detection_self_chip_tx, reset value: 0x0, access type: RW
		uint32 pmEnableDetectionSideChipTx : 1; //pm_enable_detection_side_chip_tx, reset value: 0x0, access type: RW
		uint32 pmBlockDetectionSideChipTx : 1; //pm_block_detection_side_chip_tx, reset value: 0x0, access type: RW
		uint32 pmEventsRamVerbosity : 4; //pm_events_ram_verbosity, reset value: 0x0, access type: RW
		uint32 pmEventsRamMode : 1; //pm_events_ram_mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C7_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C8 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanTh1 : 10; //pm_autocorr_mean_th_1, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh2 : 10; //pm_autocorr_mean_th_2, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh3 : 10; //pm_autocorr_mean_th_3, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0C9 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanTh4 : 10; //pm_autocorr_mean_th_4, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh5 : 10; //pm_autocorr_mean_th_5, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh6 : 10; //pm_autocorr_mean_th_6, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0C9_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CA 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanTh7 : 10; //pm_autocorr_mean_th_7, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh8 : 10; //pm_autocorr_mean_th_8, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanTh9 : 10; //pm_autocorr_mean_th_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ca_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CB 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanTh10 : 10; //pm_autocorr_mean_th_10, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall1 : 10; //pm_autocorr_mean_th_small_1, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall2 : 10; //pm_autocorr_mean_th_small_2, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Cb_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CC 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanThSmall3 : 10; //pm_autocorr_mean_th_small_3, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall4 : 10; //pm_autocorr_mean_th_small_4, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall5 : 10; //pm_autocorr_mean_th_small_5, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Cc_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CD 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanThSmall6 : 10; //pm_autocorr_mean_th_small_6, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall7 : 10; //pm_autocorr_mean_th_small_7, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall8 : 10; //pm_autocorr_mean_th_small_8, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Cd_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CE 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrMeanThSmall9 : 10; //pm_autocorr_mean_th_small_9, reset value: 0x0, access type: RW
		uint32 pmAutoCorrMeanThSmall10 : 10; //pm_autocorr_mean_th_small_10, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ce_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0CF 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDuration1 : 11; //pm_autocorr_duration_1, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDuration2 : 11; //pm_autocorr_duration_2, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Cf_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D0 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDuration3 : 11; //pm_autocorr_duration_3, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDuration4 : 11; //pm_autocorr_duration_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D1 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDuration5 : 11; //pm_autocorr_duration_5, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDuration6 : 11; //pm_autocorr_duration_6, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D2 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDuration7 : 11; //pm_autocorr_duration_7, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDuration8 : 11; //pm_autocorr_duration_8, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D2_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D3 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDuration9 : 11; //pm_autocorr_duration_9, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDuration10 : 11; //pm_autocorr_duration_10, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D3_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D4 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDurationSmall1 : 11; //pm_autocorr_duration_small_1, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDurationSmall2 : 11; //pm_autocorr_duration_small_2, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D4_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D5 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDurationSmall3 : 11; //pm_autocorr_duration_small_3, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDurationSmall4 : 11; //pm_autocorr_duration_small_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D5_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D6 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDurationSmall5 : 11; //pm_autocorr_duration_small_5, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDurationSmall6 : 11; //pm_autocorr_duration_small_6, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D6_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D7 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDurationSmall7 : 11; //pm_autocorr_duration_small_7, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDurationSmall8 : 11; //pm_autocorr_duration_small_8, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D7_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D8 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmAutoCorrDurationSmall9 : 11; //pm_autocorr_duration_small_9, reset value: 0x0, access type: RW
		uint32 pmAutoCorrDurationSmall10 : 11; //pm_autocorr_duration_small_10, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0D9 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorMaPowerRm : 3; //radar_detector_ma_power_rm, reset value: 0x3, access type: RW
		uint32 radarDetectorMaAutocorrRm : 3; //radar_detector_ma_autocorr_rm, reset value: 0x3, access type: RW
		uint32 radarDetectorDlAutocorrRm : 3; //radar_detector_dl_autocorr_rm, reset value: 0x3, access type: RW
		uint32 radarDetectorEventsRm : 3; //radar_detector_events_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0D9_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DA 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh5AntPri : 8; //det_th_5ant_pri, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Da_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DB 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh5AntSec : 8; //det_th_5ant_sec, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Db_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DC 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detTh5AntCoCh : 8; //det_th_5ant_co_ch, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Dc_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0DD 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdTestPlugLenAn4 : 25; //Test Plug Local Enable., reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Dd_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E0 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn4Corr64AtCommonFineT : 28; //Detector an4 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E1 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detAbsAn4Corr16AtLocalCoarseT : 20; //Detector an4 Abs corr64 smoothed at fine timing, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E2 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmHeTbTrigDft : 1; //pm_he_tb_trig_dft, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E2_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E5 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRadarDetectorAntSel : 3; //radar_detector_band_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E5_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E6 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectionMask4 : 8; //detecttiom mask, reset value: 0x0, access type: RW
		uint32 detectionMask5 : 8; //detecttiom mask, reset value: 0x0, access type: RW
		uint32 detectionMask6 : 8; //detecttiom mask, reset value: 0x0, access type: RW
		uint32 detectionMask7 : 8; //detecttiom mask, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E6_u;

/*REG_B1_DFS_PHY_RX_TD_AFE_DYN_CTRL 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx0On : 1; //Rx0 Standby , reset value: 0x0, access type: RW
		uint32 rx1On : 1; //Rx1 Standby, reset value: 0x0, access type: RW
		uint32 rx2On : 1; //Rx2 Standby , reset value: 0x0, access type: RW
		uint32 rx3On : 1; //Rx2 Standby , reset value: 0x0, access type: RW
		uint32 rx4On : 1; //Rx4 Standby , reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 tx0On : 1; //Tx0 Standby, reset value: 0x0, access type: RW
		uint32 tx1On : 1; //Tx1 Standby , reset value: 0x0, access type: RW
		uint32 tx2On : 1; //Tx2 Standby, reset value: 0x0, access type: RW
		uint32 tx3On : 1; //Tx3 Standby, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
		uint32 aux0AdcOn : 1; //Aux0 Standby, reset value: 0x0, access type: RW
		uint32 aux1AdcOn : 1; //Aux1 Standby, reset value: 0x0, access type: RW
		uint32 aux2AdcOn : 1; //Aux2 Standby, reset value: 0x0, access type: RW
		uint32 aux3AdcOn : 1; //Aux3 Standby, reset value: 0x0, access type: RW
		uint32 aux4AdcOn : 1; //Aux4 Standby, reset value: 0x0, access type: RW
		uint32 dpdOn : 4; //no description, reset value: 0x0, access type: RW
		uint32 dftOn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 6;
	} bitFields;
} RegB1DfsPhyRxTdAfeDynCtrl_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E8 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorAllOrB0EventRamAdd : 32; //radar detector address for band 0 or for all bands, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0E9 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB1EventRamAdd : 32; //radar detector address for band 1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0E9_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EA 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB2EventRamAdd : 32; //radar detector address for band 2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ea_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EB 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB3EventRamAdd : 32; //radar detector address for band 3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Eb_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EC 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB4EventRamAdd : 32; //radar detector address for band 4, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ec_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0ED 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB5EventRamAdd : 32; //radar detector address for band 5, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ed_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EE 0x3BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB6EventRamAdd : 32; //radar detector address for band 6, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ee_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0EF 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorB7EventRamAdd : 32; //radar detector address for band 7, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Ef_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F0 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorGenriscForceStart : 1; //radar detector genrisc force start pulse, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F0_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F1 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorGenriscForceStop : 1; //radar detector genrisc force stop pulse, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F1_u;

/*REG_B1_DFS_PHY_RX_TD_TB_BYTES_IN_FIFO_1 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tbBytesInFifo1 : 19; //tb bytes in fifo second fifo, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
	} bitFields;
} RegB1DfsPhyRxTdTbBytesInFifo1_u;

/*REG_B1_DFS_PHY_RX_TD_TB_ACCESS_CONTROL 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tbPageAddr : 4; //select page for test bus, reset value: 0x0, access type: RW
		uint32 tbApbSel : 1; //select tb fifo 0 or 1 for apb access, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdTbAccessControl_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_CONTROL 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoInternalTrigSignalDetect : 1; //start record on signal_detect fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigCoarse : 1; //start record on coarse rdy fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigFine : 1; //start record on fine rdy fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigHtShort : 1; //start record on ht stf fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigHeShort : 1; //start record on he stf fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigPacketCounter : 1; //start record on tx packet counter fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigTxPacketCounter : 1; //start record on packet counter fifo 0, reset value: 0x0, access type: RW
		uint32 testFifoInternalStopOnPac : 1; //stop record on pac fifo 0, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdTestFifoInternalTrigControl_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_PACKET_COUNTER_LEVEL 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoInternalTrigPacketCounterLevel : 27; //interal trigger packet counter level reference fifo 0, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegB1DfsPhyRxTdTestFifoInternalTrigPacketCounterLevel_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_CONTROL_1 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoInternalTrigSignalDetect1 : 1; //start record on signal_detect fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigCoarse1 : 1; //start record on coarse rdy fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigFine1 : 1; //start record on fine rdy fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigHtShort1 : 1; //start record on ht stf fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigHeShort1 : 1; //start record on he stf fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigPacketCounter1 : 1; //start record on packet counter fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalTrigTxPacketCounter1 : 1; //start record on tx packet counter fifo 1, reset value: 0x0, access type: RW
		uint32 testFifoInternalStopOnPac1 : 1; //stop record on pac fifo 1, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1DfsPhyRxTdTestFifoInternalTrigControl1_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_FIFO_INTERNAL_TRIG_PACKET_COUNTER_LEVEL_1 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testFifoInternalTrigPacketCounterLevel1 : 27; //interal trigger packet counter level reference fifo 1, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegB1DfsPhyRxTdTestFifoInternalTrigPacketCounterLevel1_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F7 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 antennaMaskInternalLna : 5; //antenna_mask_internal_lna, reset value: 0x0, access type: RW
		uint32 disableFineTimingInternalLna : 1; //disable_fine_timing_internal_lna, reset value: 0x0, access type: RW
		uint32 disableFineFrequencyInternalLna : 1; //disable_fine_frequency_internal_lna, reset value: 0x0, access type: RW
		uint32 disableCoarseTimingInternalLna : 1; //disable_coarse_timing_internal_lna, reset value: 0x0, access type: RW
		uint32 disableCoarseFrequencyInternalLna : 1; //disable_coarse_frequency_internal_lna, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F7_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F8 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRamLs : 4; //pm_ram_ls, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 pmRamDs : 1; //pm_ram_ds, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 pmRamSd : 1; //pm_ram_sd, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
		uint32 pmRamPsHwEn : 4; //pm_ram_ps_hw_en, reset value: 0x0, access type: RW
		uint32 reserved3 : 4;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F8_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0F9 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmRamRop : 1; //pm_ram_rop, reset value: 0x1, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0F9_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_BUS_FIFO_CONFIG_ERROR 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusFifoConfigError : 1; //test_bus_fifo_config and test_bus_fifo_config_1 in both segments conflict, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdTestBusFifoConfigError_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0FB 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorGpiSel : 4; //radar_detector_gpi_sel, reset value: 0x0, access type: RW
		uint32 radarDetectorGpiEn : 1; //radar_detector_gpi_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Fb_u;

/*REG_B1_DFS_PHY_RX_TD_PHY_RXTD_REG0FC 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 channelFilterHbDelay : 5; //channel_filter_hb_delay, reset value: 0x19, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1DfsPhyRxTdPhyRxtdReg0Fc_u;

/*REG_B1_DFS_PHY_RX_TD_TEST_BUS_FIFO_POWER_CTRL 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusSd : 8; //shut down test bus rams, reset value: 0x0, access type: RW
		uint32 testBusDs : 8; //deep sleep test bus rams, reset value: 0x0, access type: RW
		uint32 testBusRop : 8; //test bus rams ROP, reset value: 0xff, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB1DfsPhyRxTdTestBusFifoPowerCtrl_u;

/*REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN0 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstAccOutAn0 : 31; //Rx TD Noise Estimation Accumulator Data An0, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdNoiseEstAccAn0_u;

/*REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN1 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstAccOutAn1 : 31; //Rx TD Noise Estimation Accumulator Data An1, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdNoiseEstAccAn1_u;

/*REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN2 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstAccOutAn2 : 31; //Rx TD Noise Estimation Accumulator Data An2, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdNoiseEstAccAn2_u;

/*REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN3 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstAccOutAn3 : 31; //Rx TD Noise Estimation Accumulator Data An3, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdNoiseEstAccAn3_u;

/*REG_B1_DFS_PHY_RX_TD_NOISE_EST_ACC_AN4 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseEstAccOutAn4 : 31; //Rx TD Noise Estimation Accumulator Data An4, reset value: 0x2, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdNoiseEstAccAn4_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN0 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSumIn0 : 31; //short_sum_in0, reset value: 0x7fffffff, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdShortSumAn0_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN1 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSumIn1 : 31; //short_sum_in1, reset value: 0x7fffffff, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdShortSumAn1_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN2 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSumIn2 : 31; //short_sum_in2, reset value: 0x7fffffff, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdShortSumAn2_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN3 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSumIn3 : 31; //short_sum_in3, reset value: 0x7fffffff, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdShortSumAn3_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SUM_AN4 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSumIn4 : 31; //short_sum_in4, reset value: 0x7fffffff, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB1DfsPhyRxTdShortSumAn4_u;

/*REG_B1_DFS_PHY_RX_TD_SNR_EST_AN01 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 snrEstAn0 : 10; //Rx TD SNR Estimation Data An0, reset value: 0x0, access type: RO
		uint32 snrEstAn1 : 10; //Rx TD SNR Estimation Data An1, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdSnrEstAn01_u;

/*REG_B1_DFS_PHY_RX_TD_SNR_EST_AN23 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 snrEstAn2 : 10; //Rx TD SNR Estimation Data An2, reset value: 0x0, access type: RO
		uint32 snrEstAn3 : 10; //Rx TD SNR Estimation Data An3, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB1DfsPhyRxTdSnrEstAn23_u;

/*REG_B1_DFS_PHY_RX_TD_SNR_EST_AN45 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 snrEstAn4 : 10; //Rx TD SNR Estimation Data An4, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdSnrEstAn45_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN0 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSnrIn0 : 10; //short_snr_in0, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdShortSnrAn0_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN1 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSnrIn1 : 10; //short_snr_in1, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdShortSnrAn1_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN2 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSnrIn2 : 10; //short_snr_in2, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdShortSnrAn2_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN3 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSnrIn3 : 10; //short_snr_in3, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdShortSnrAn3_u;

/*REG_B1_DFS_PHY_RX_TD_SHORT_SNR_AN4 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortSnrIn4 : 10; //short_snr_in4, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1DfsPhyRxTdShortSnrAn4_u;

/*REG_B1_DFS_PHY_RX_TD_SIGNAL_DETECT_LOCK_EN 0x468 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 signalDetectLockEn : 1; //signal_detect_lock_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdSignalDetectLockEn_u;

/*REG_B1_DFS_PHY_RX_TD_BAND_320 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band320 : 1; //band support 320MHz, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdBand320_u;

/*REG_B1_DFS_PHY_RX_TD_PM_IND_NOISE_CTRL 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmIndNoiseUpperSelect : 1; //Select which FD is getting the Noise gi results.  , leave default 0 for FD in Super-Segment  0  (FD0) , set to '1' for FD in Super-segment 1  (FD1) , , reset value: 0x0, access type: RW
		uint32 pmIndNoiseSplitCalc : 1; //Select split calculation for upper and lower antennas.  , If not changed from default, the same results are updated to both FD upper and lower.  , Leave at 0 to allow both FDs to latch same noise results. (default) , Set to 1 to allow different calculation for FD1.  , , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1DfsPhyRxTdPmIndNoiseCtrl_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_FD0 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd0 : 16; //spare connectivity from FD0, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromFd0_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_FD1 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd1 : 16; //spare connectivity from FD1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromFd1_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_TX 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTx : 16; //spare connectivity from TX, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromTx_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_BE 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromBe : 16; //spare connectivity from BE, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromBe_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_TO_FD0 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd0 : 16; //spare connectivity to FD0, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareToFd0_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_TO_FD1 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd1 : 16; //spare connectivity to FD1, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareToFd1_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_TO_TX 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTx : 16; //spare connectivity to TX, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareToTx_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_TO_BE 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToBe : 16; //spare connectivity to BE, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareToBe_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_TO_MAC 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToMac : 4; //spare to mac , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1DfsPhyRxTdSpareToMac_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_MAC 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromMac : 16; //spare from mac, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromMac_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_CPU 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromCpu : 16; //spare from cpu, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromCpu_u;

/*REG_B1_DFS_PHY_RX_TD_SPARE_FROM_HOST 0x4A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromHost : 16; //spare from host, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdSpareFromHost_u;

/*REG_B1_DFS_PHY_RX_TD_RADAR_DETECTOR_TX_ON_OTHER_BAND_EN 0x4A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorTxOnOtherBandEn : 3; //radar_detector_tx_on_other_band_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdRadarDetectorTxOnOtherBandEn_u;

/*REG_B1_DFS_PHY_RX_TD_TX_PER_BAND 0x4A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPerBand : 3; //tx per band, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdTxPerBand_u;

/*REG_B1_DFS_PHY_RX_TD_GOERTZEL_MODE 0x4AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 goertzelMode : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdGoertzelMode_u;

/*REG_B1_DFS_PHY_RX_TD_DET_LPI_CTRL 0x4B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detLpiEnable : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 detLpiDetcnt : 4; //Number of 20MHz cycles a detection must hold until it is declared (hold until reset), reset value: 0x5, access type: RW
		uint32 detLpiWaitCycles : 10; //Additional counter which shall give the STD detector time to detect more bands., reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
		uint32 detLpiMemctlError : 1; //no description, reset value: 0x0, access type: RO
		uint32 detLpiDeclared : 1; //This is a status bit and it reflects the declared output of the LPI Detector, reset value: 0x0, access type: RO
	} bitFields;
} RegB1DfsPhyRxTdDetLpiCtrl_u;

/*REG_B1_DFS_PHY_RX_TD_DET_LPI_THR0 0x4B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detLpiThrCorr : 16; //no description, reset value: 0x0, access type: RW
		uint32 detLpiThrCorrBand : 16; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdDetLpiThr0_u;

/*REG_B1_DFS_PHY_RX_TD_DET_LPI_THR1 0x4B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detLpiThrR : 16; //Power Threshold, reset value: 0x0, access type: RW
		uint32 detLpiThrDeriv : 16; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdDetLpiThr1_u;

/*REG_B1_DFS_PHY_RX_TD_DET_LPI_THR2 0x4BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detLpiThrMax : 16; //Maximum Correction Threshold, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdDetLpiThr2_u;

/*REG_B1_DFS_PHY_RX_TD_EARLY_EHT_PPM_DELAY_CNT_VAL 0x4C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 earlyEhtPktDelayCntVal : 16; //Extra Delay for regular EHT packets for PPM logic. , The value of the field is delay in clk cycles., reset value: 0x0, access type: RW
		uint32 earlyTrigPktDelayCntVal : 16; //Extra Delay for trigger EHT packets for PPM logic. , The value of the field is delay in clk cycles., reset value: 0x0, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdEarlyEhtPpmDelayCntVal_u;

/*REG_B1_DFS_PHY_RX_TD_LAST_SAMPLE_OFFSET_REG1 0x4C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSampleOffset3840 : 16; //no description, reset value: 0x2800, access type: RW
		uint32 lastSampleOffset960 : 16; //no description, reset value: 0x2800, access type: RW
	} bitFields;
} RegB1DfsPhyRxTdLastSampleOffsetReg1_u;

/*REG_B1_DFS_PHY_RX_TD_LAST_SAMPLE_OFFSET_REG2 0x4C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastSampleOffset480 : 16; //no description, reset value: 0x2800, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdLastSampleOffsetReg2_u;

/*REG_B1_DFS_PHY_RX_TD_DFS_BLOCK_ACTIVE 0x4CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 radarDetectorNotActive : 1; //dfs block active indication , reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1DfsPhyRxTdDfsBlockActive_u;

/*REG_B1_DFS_PHY_RX_TD_EARLY_EHT_TRIG_MUX_DELAY_CNT_VAL 0x4D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmEarlyEhtTrigMuxDelayCntVal : 16; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1DfsPhyRxTdEarlyEhtTrigMuxDelayCntVal_u;

/*REG_B1_DFS_PHY_RX_TD_GCLK_BYPASS 0x4D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dpdIqGclkBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 dftIqGclkBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 dcrfGclkBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1DfsPhyRxTdGclkBypass_u;



#endif // _B1_DFS_PHY_RX_TD_REGS_H_
