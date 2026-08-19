
/***********************************************************************************
File:				PhyRxFdRegs.h
Module:				phyRxFd
SOC Revision:		latest
Generated at:       2024-06-26 12:55:17
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _PHY_RX_FD_REGS_H_
#define _PHY_RX_FD_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define PHY_RX_FD_BASE_ADDRESS                              MEMORY_MAP_UNIT_202_BASE_ADDRESS
#define	REG_PHY_RX_FD_PHY_RXFD_REG06                        (PHY_RX_FD_BASE_ADDRESS + 0x18)
#define	REG_PHY_RX_FD_PHY_RXFD_REG07                        (PHY_RX_FD_BASE_ADDRESS + 0x1C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG08                        (PHY_RX_FD_BASE_ADDRESS + 0x20)
#define	REG_PHY_RX_FD_PHY_RXFD_REG09                        (PHY_RX_FD_BASE_ADDRESS + 0x24)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0A                        (PHY_RX_FD_BASE_ADDRESS + 0x28)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0B                        (PHY_RX_FD_BASE_ADDRESS + 0x2C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0C                        (PHY_RX_FD_BASE_ADDRESS + 0x30)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0D                        (PHY_RX_FD_BASE_ADDRESS + 0x34)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0E                        (PHY_RX_FD_BASE_ADDRESS + 0x38)
#define	REG_PHY_RX_FD_PHY_RXFD_REG0F                        (PHY_RX_FD_BASE_ADDRESS + 0x3C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG11                        (PHY_RX_FD_BASE_ADDRESS + 0x44)
#define	REG_PHY_RX_FD_PHY_RXFD_REG12                        (PHY_RX_FD_BASE_ADDRESS + 0x48)
#define	REG_PHY_RX_FD_PHY_RXFD_REG13                        (PHY_RX_FD_BASE_ADDRESS + 0x4C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG14                        (PHY_RX_FD_BASE_ADDRESS + 0x50)
#define	REG_PHY_RX_FD_PHY_RXFD_REG15                        (PHY_RX_FD_BASE_ADDRESS + 0x54)
#define	REG_PHY_RX_FD_PHY_RXFD_REG16                        (PHY_RX_FD_BASE_ADDRESS + 0x58)
#define	REG_PHY_RX_FD_PHY_RXFD_REG1B                        (PHY_RX_FD_BASE_ADDRESS + 0x6C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG1C                        (PHY_RX_FD_BASE_ADDRESS + 0x70)
#define	REG_PHY_RX_FD_PHY_RXFD_REG1D                        (PHY_RX_FD_BASE_ADDRESS + 0x74)
#define	REG_PHY_RX_FD_PHY_RXFD_REG1E                        (PHY_RX_FD_BASE_ADDRESS + 0x78)
#define	REG_PHY_RX_FD_PHY_RXFD_REG34                        (PHY_RX_FD_BASE_ADDRESS + 0xD0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG35                        (PHY_RX_FD_BASE_ADDRESS + 0xD4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG36                        (PHY_RX_FD_BASE_ADDRESS + 0xD8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG38                        (PHY_RX_FD_BASE_ADDRESS + 0xE0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG3A                        (PHY_RX_FD_BASE_ADDRESS + 0xE8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG3B                        (PHY_RX_FD_BASE_ADDRESS + 0xEC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG3C                        (PHY_RX_FD_BASE_ADDRESS + 0xF0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG3E                        (PHY_RX_FD_BASE_ADDRESS + 0xF8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG4E                        (PHY_RX_FD_BASE_ADDRESS + 0x138)
#define	REG_PHY_RX_FD_PHY_RXFD_REG50                        (PHY_RX_FD_BASE_ADDRESS + 0x140)
#define	REG_PHY_RX_FD_PHY_RXFD_REG51                        (PHY_RX_FD_BASE_ADDRESS + 0x144)
#define	REG_PHY_RX_FD_PHY_RXFD_REG57                        (PHY_RX_FD_BASE_ADDRESS + 0x15C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG68                        (PHY_RX_FD_BASE_ADDRESS + 0x1A0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG74                        (PHY_RX_FD_BASE_ADDRESS + 0x1D0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG75                        (PHY_RX_FD_BASE_ADDRESS + 0x1D4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG76                        (PHY_RX_FD_BASE_ADDRESS + 0x1D8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG77                        (PHY_RX_FD_BASE_ADDRESS + 0x1DC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG78                        (PHY_RX_FD_BASE_ADDRESS + 0x1E0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG79                        (PHY_RX_FD_BASE_ADDRESS + 0x1E4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7A                        (PHY_RX_FD_BASE_ADDRESS + 0x1E8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7B                        (PHY_RX_FD_BASE_ADDRESS + 0x1EC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7C                        (PHY_RX_FD_BASE_ADDRESS + 0x1F0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7D                        (PHY_RX_FD_BASE_ADDRESS + 0x1F4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7E                        (PHY_RX_FD_BASE_ADDRESS + 0x1F8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG7F                        (PHY_RX_FD_BASE_ADDRESS + 0x1FC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG80                        (PHY_RX_FD_BASE_ADDRESS + 0x200)
#define	REG_PHY_RX_FD_PHY_RXFD_REG81                        (PHY_RX_FD_BASE_ADDRESS + 0x204)
#define	REG_PHY_RX_FD_PHY_RXFD_REG82                        (PHY_RX_FD_BASE_ADDRESS + 0x208)
#define	REG_PHY_RX_FD_PHY_RXFD_REG83                        (PHY_RX_FD_BASE_ADDRESS + 0x20C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG84                        (PHY_RX_FD_BASE_ADDRESS + 0x210)
#define	REG_PHY_RX_FD_PHY_RXFD_REG86                        (PHY_RX_FD_BASE_ADDRESS + 0x218)
#define	REG_PHY_RX_FD_PHY_RXFD_REG87                        (PHY_RX_FD_BASE_ADDRESS + 0x21C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG88                        (PHY_RX_FD_BASE_ADDRESS + 0x220)
#define	REG_PHY_RX_FD_PHY_RXFD_REG89                        (PHY_RX_FD_BASE_ADDRESS + 0x224)
#define	REG_PHY_RX_FD_PHY_RXFD_REG8A                        (PHY_RX_FD_BASE_ADDRESS + 0x228)
#define	REG_PHY_RX_FD_PHY_RXFD_REG8B                        (PHY_RX_FD_BASE_ADDRESS + 0x22C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG8C                        (PHY_RX_FD_BASE_ADDRESS + 0x230)
#define	REG_PHY_RX_FD_PHY_RXFD_REG8D                        (PHY_RX_FD_BASE_ADDRESS + 0x234)
#define	REG_PHY_RX_FD_PHY_RXFD_IF08E                        (PHY_RX_FD_BASE_ADDRESS + 0x238)
#define	REG_PHY_RX_FD_PHY_RXFD_REG90                        (PHY_RX_FD_BASE_ADDRESS + 0x240)
#define	REG_PHY_RX_FD_PHY_RXFD_REG91                        (PHY_RX_FD_BASE_ADDRESS + 0x244)
#define	REG_PHY_RX_FD_PHY_RXFD_REG92                        (PHY_RX_FD_BASE_ADDRESS + 0x248)
#define	REG_PHY_RX_FD_PHY_RXFD_REG93                        (PHY_RX_FD_BASE_ADDRESS + 0x24C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG94                        (PHY_RX_FD_BASE_ADDRESS + 0x250)
#define	REG_PHY_RX_FD_PHY_RXFD_REG95                        (PHY_RX_FD_BASE_ADDRESS + 0x254)
#define	REG_PHY_RX_FD_PHY_RXFD_REG96                        (PHY_RX_FD_BASE_ADDRESS + 0x258)
#define	REG_PHY_RX_FD_PHY_RXFD_REG97                        (PHY_RX_FD_BASE_ADDRESS + 0x25C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG98                        (PHY_RX_FD_BASE_ADDRESS + 0x260)
#define	REG_PHY_RX_FD_PHY_RXFD_REG99                        (PHY_RX_FD_BASE_ADDRESS + 0x264)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9A                        (PHY_RX_FD_BASE_ADDRESS + 0x268)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9B                        (PHY_RX_FD_BASE_ADDRESS + 0x26C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9C                        (PHY_RX_FD_BASE_ADDRESS + 0x270)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9D                        (PHY_RX_FD_BASE_ADDRESS + 0x274)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9E                        (PHY_RX_FD_BASE_ADDRESS + 0x278)
#define	REG_PHY_RX_FD_PHY_RXFD_REG9F                        (PHY_RX_FD_BASE_ADDRESS + 0x27C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA0                        (PHY_RX_FD_BASE_ADDRESS + 0x280)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA1                        (PHY_RX_FD_BASE_ADDRESS + 0x284)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA2                        (PHY_RX_FD_BASE_ADDRESS + 0x288)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA3                        (PHY_RX_FD_BASE_ADDRESS + 0x28C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA4                        (PHY_RX_FD_BASE_ADDRESS + 0x290)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA5                        (PHY_RX_FD_BASE_ADDRESS + 0x294)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA6                        (PHY_RX_FD_BASE_ADDRESS + 0x298)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA7                        (PHY_RX_FD_BASE_ADDRESS + 0x29C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA8                        (PHY_RX_FD_BASE_ADDRESS + 0x2A0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGA9                        (PHY_RX_FD_BASE_ADDRESS + 0x2A4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGAA                        (PHY_RX_FD_BASE_ADDRESS + 0x2A8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGAB                        (PHY_RX_FD_BASE_ADDRESS + 0x2AC)
#define	REG_PHY_RX_FD_PHY_RXFD_REGBE                        (PHY_RX_FD_BASE_ADDRESS + 0x2F8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC0                        (PHY_RX_FD_BASE_ADDRESS + 0x300)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC1                        (PHY_RX_FD_BASE_ADDRESS + 0x304)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC2                        (PHY_RX_FD_BASE_ADDRESS + 0x308)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC3                        (PHY_RX_FD_BASE_ADDRESS + 0x30C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC4                        (PHY_RX_FD_BASE_ADDRESS + 0x310)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC5                        (PHY_RX_FD_BASE_ADDRESS + 0x314)
#define	REG_PHY_RX_FD_PHY_RXFD_REGC6                        (PHY_RX_FD_BASE_ADDRESS + 0x318)
#define	REG_PHY_RX_FD_PHY_RXFD_REGCB                        (PHY_RX_FD_BASE_ADDRESS + 0x32C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGCC                        (PHY_RX_FD_BASE_ADDRESS + 0x330)
#define	REG_PHY_RX_FD_PHY_RXFD_REGCD                        (PHY_RX_FD_BASE_ADDRESS + 0x334)
#define	REG_PHY_RX_FD_PHY_RXFD_REGCE                        (PHY_RX_FD_BASE_ADDRESS + 0x338)
#define	REG_PHY_RX_FD_PHY_RXFD_REGCF                        (PHY_RX_FD_BASE_ADDRESS + 0x33C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD0                        (PHY_RX_FD_BASE_ADDRESS + 0x340)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD1                        (PHY_RX_FD_BASE_ADDRESS + 0x344)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD2                        (PHY_RX_FD_BASE_ADDRESS + 0x348)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD3                        (PHY_RX_FD_BASE_ADDRESS + 0x34C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD4                        (PHY_RX_FD_BASE_ADDRESS + 0x350)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD5                        (PHY_RX_FD_BASE_ADDRESS + 0x354)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD6                        (PHY_RX_FD_BASE_ADDRESS + 0x358)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD7                        (PHY_RX_FD_BASE_ADDRESS + 0x35C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD8                        (PHY_RX_FD_BASE_ADDRESS + 0x360)
#define	REG_PHY_RX_FD_PHY_RXFD_REGD9                        (PHY_RX_FD_BASE_ADDRESS + 0x364)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDA                        (PHY_RX_FD_BASE_ADDRESS + 0x368)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDB                        (PHY_RX_FD_BASE_ADDRESS + 0x36C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDC                        (PHY_RX_FD_BASE_ADDRESS + 0x370)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDD                        (PHY_RX_FD_BASE_ADDRESS + 0x374)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDE                        (PHY_RX_FD_BASE_ADDRESS + 0x378)
#define	REG_PHY_RX_FD_PHY_RXFD_REGDF                        (PHY_RX_FD_BASE_ADDRESS + 0x37C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE0                        (PHY_RX_FD_BASE_ADDRESS + 0x380)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE1                        (PHY_RX_FD_BASE_ADDRESS + 0x384)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE2                        (PHY_RX_FD_BASE_ADDRESS + 0x388)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE3                        (PHY_RX_FD_BASE_ADDRESS + 0x38C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE4                        (PHY_RX_FD_BASE_ADDRESS + 0x390)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE5                        (PHY_RX_FD_BASE_ADDRESS + 0x394)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE6                        (PHY_RX_FD_BASE_ADDRESS + 0x398)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE7                        (PHY_RX_FD_BASE_ADDRESS + 0x39C)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE8                        (PHY_RX_FD_BASE_ADDRESS + 0x3A0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGE9                        (PHY_RX_FD_BASE_ADDRESS + 0x3A4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGEA                        (PHY_RX_FD_BASE_ADDRESS + 0x3A8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGEB                        (PHY_RX_FD_BASE_ADDRESS + 0x3AC)
#define	REG_PHY_RX_FD_PHY_RXFD_REGEC                        (PHY_RX_FD_BASE_ADDRESS + 0x3B0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGED                        (PHY_RX_FD_BASE_ADDRESS + 0x3B4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGEE                        (PHY_RX_FD_BASE_ADDRESS + 0x3B8)
#define	REG_PHY_RX_FD_PHY_RXFD_APB_CONTROL                  (PHY_RX_FD_BASE_ADDRESS + 0x3C0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF1                        (PHY_RX_FD_BASE_ADDRESS + 0x3C4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF2                        (PHY_RX_FD_BASE_ADDRESS + 0x3C8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF3                        (PHY_RX_FD_BASE_ADDRESS + 0x3CC)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF4                        (PHY_RX_FD_BASE_ADDRESS + 0x3D0)
#define	REG_PHY_RX_FD_STREAMER_CONTROL                      (PHY_RX_FD_BASE_ADDRESS + 0x3D4)
#define	REG_PHY_RX_FD_STREAMER_ADDRESS                      (PHY_RX_FD_BASE_ADDRESS + 0x3D8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF7                        (PHY_RX_FD_BASE_ADDRESS + 0x3DC)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF8                        (PHY_RX_FD_BASE_ADDRESS + 0x3E0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGF9                        (PHY_RX_FD_BASE_ADDRESS + 0x3E4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFA                        (PHY_RX_FD_BASE_ADDRESS + 0x3E8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFB                        (PHY_RX_FD_BASE_ADDRESS + 0x3EC)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFC                        (PHY_RX_FD_BASE_ADDRESS + 0x3F0)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFD                        (PHY_RX_FD_BASE_ADDRESS + 0x3F4)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFE                        (PHY_RX_FD_BASE_ADDRESS + 0x3F8)
#define	REG_PHY_RX_FD_PHY_RXFD_REGFF                        (PHY_RX_FD_BASE_ADDRESS + 0x3FC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG100                       (PHY_RX_FD_BASE_ADDRESS + 0x400)
#define	REG_PHY_RX_FD_PHY_RXFD_REG101                       (PHY_RX_FD_BASE_ADDRESS + 0x404)
#define	REG_PHY_RX_FD_PHY_RXFD_REG102                       (PHY_RX_FD_BASE_ADDRESS + 0x408)
#define	REG_PHY_RX_FD_PHY_RXFD_REG103                       (PHY_RX_FD_BASE_ADDRESS + 0x40C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG104                       (PHY_RX_FD_BASE_ADDRESS + 0x410)
#define	REG_PHY_RX_FD_PHY_RXFD_REG105                       (PHY_RX_FD_BASE_ADDRESS + 0x414)
#define	REG_PHY_RX_FD_PHY_RXFD_REG106                       (PHY_RX_FD_BASE_ADDRESS + 0x418)
#define	REG_PHY_RX_FD_PHY_RXFD_REG107                       (PHY_RX_FD_BASE_ADDRESS + 0x41C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG108                       (PHY_RX_FD_BASE_ADDRESS + 0x420)
#define	REG_PHY_RX_FD_PHY_RXFD_REG109                       (PHY_RX_FD_BASE_ADDRESS + 0x424)
#define	REG_PHY_RX_FD_PHY_RXFD_REG110                       (PHY_RX_FD_BASE_ADDRESS + 0x428)
#define	REG_PHY_RX_FD_PHY_RXFD_REG111                       (PHY_RX_FD_BASE_ADDRESS + 0x42C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG112                       (PHY_RX_FD_BASE_ADDRESS + 0x430)
#define	REG_PHY_RX_FD_PHY_RXFD_REG113                       (PHY_RX_FD_BASE_ADDRESS + 0x434)
#define	REG_PHY_RX_FD_PHY_RXFD_REG114                       (PHY_RX_FD_BASE_ADDRESS + 0x438)
#define	REG_PHY_RX_FD_PHY_RXFD_REG115                       (PHY_RX_FD_BASE_ADDRESS + 0x43C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG116                       (PHY_RX_FD_BASE_ADDRESS + 0x440)
#define	REG_PHY_RX_FD_PHY_RXFD_REG117                       (PHY_RX_FD_BASE_ADDRESS + 0x444)
#define	REG_PHY_RX_FD_PHY_RXFD_REG118                       (PHY_RX_FD_BASE_ADDRESS + 0x448)
#define	REG_PHY_RX_FD_PHY_RXFD_REG119                       (PHY_RX_FD_BASE_ADDRESS + 0x44C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG120                       (PHY_RX_FD_BASE_ADDRESS + 0x450)
#define	REG_PHY_RX_FD_PHY_RXFD_REG121                       (PHY_RX_FD_BASE_ADDRESS + 0x454)
#define	REG_PHY_RX_FD_PHY_RXFD_REG122                       (PHY_RX_FD_BASE_ADDRESS + 0x458)
#define	REG_PHY_RX_FD_PHY_RXFD_REG123                       (PHY_RX_FD_BASE_ADDRESS + 0x45C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG124                       (PHY_RX_FD_BASE_ADDRESS + 0x460)
#define	REG_PHY_RX_FD_PHY_RXFD_REG125                       (PHY_RX_FD_BASE_ADDRESS + 0x464)
#define	REG_PHY_RX_FD_PHY_RXFD_REG126                       (PHY_RX_FD_BASE_ADDRESS + 0x468)
#define	REG_PHY_RX_FD_PHY_RXFD_REG127                       (PHY_RX_FD_BASE_ADDRESS + 0x46C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG128                       (PHY_RX_FD_BASE_ADDRESS + 0x470)
#define	REG_PHY_RX_FD_PHY_RXFD_REG129                       (PHY_RX_FD_BASE_ADDRESS + 0x474)
#define	REG_PHY_RX_FD_PHY_RXFD_REG130                       (PHY_RX_FD_BASE_ADDRESS + 0x478)
#define	REG_PHY_RX_FD_PHY_RXFD_REG131                       (PHY_RX_FD_BASE_ADDRESS + 0x47C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG132                       (PHY_RX_FD_BASE_ADDRESS + 0x480)
#define	REG_PHY_RX_FD_PHY_RXFD_REG133                       (PHY_RX_FD_BASE_ADDRESS + 0x484)
#define	REG_PHY_RX_FD_PHY_RXFD_REG134                       (PHY_RX_FD_BASE_ADDRESS + 0x488)
#define	REG_PHY_RX_FD_PHY_RXFD_REG135                       (PHY_RX_FD_BASE_ADDRESS + 0x48C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG136                       (PHY_RX_FD_BASE_ADDRESS + 0x490)
#define	REG_PHY_RX_FD_PHY_RXFD_REG137                       (PHY_RX_FD_BASE_ADDRESS + 0x494)
#define	REG_PHY_RX_FD_PHY_RXFD_REG138                       (PHY_RX_FD_BASE_ADDRESS + 0x498)
#define	REG_PHY_RX_FD_PHY_RXFD_REG139                       (PHY_RX_FD_BASE_ADDRESS + 0x49C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG140                       (PHY_RX_FD_BASE_ADDRESS + 0x4A0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG141                       (PHY_RX_FD_BASE_ADDRESS + 0x4A4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG142                       (PHY_RX_FD_BASE_ADDRESS + 0x4A8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG143                       (PHY_RX_FD_BASE_ADDRESS + 0x4AC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG144                       (PHY_RX_FD_BASE_ADDRESS + 0x4B0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG145                       (PHY_RX_FD_BASE_ADDRESS + 0x4B4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG146                       (PHY_RX_FD_BASE_ADDRESS + 0x4B8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG147                       (PHY_RX_FD_BASE_ADDRESS + 0x4BC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG148                       (PHY_RX_FD_BASE_ADDRESS + 0x4C0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG149                       (PHY_RX_FD_BASE_ADDRESS + 0x4C4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG150                       (PHY_RX_FD_BASE_ADDRESS + 0x4C8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG151                       (PHY_RX_FD_BASE_ADDRESS + 0x4D0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG152                       (PHY_RX_FD_BASE_ADDRESS + 0x4D4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG153                       (PHY_RX_FD_BASE_ADDRESS + 0x4D8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG154                       (PHY_RX_FD_BASE_ADDRESS + 0x4DC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG155                       (PHY_RX_FD_BASE_ADDRESS + 0x4E0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG156                       (PHY_RX_FD_BASE_ADDRESS + 0x4E4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG157                       (PHY_RX_FD_BASE_ADDRESS + 0x4E8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG158                       (PHY_RX_FD_BASE_ADDRESS + 0x4EC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG159                       (PHY_RX_FD_BASE_ADDRESS + 0x4F0)
#define	REG_PHY_RX_FD_PHY_RXFD_REG160                       (PHY_RX_FD_BASE_ADDRESS + 0x4F4)
#define	REG_PHY_RX_FD_PHY_RXFD_REG161                       (PHY_RX_FD_BASE_ADDRESS + 0x4F8)
#define	REG_PHY_RX_FD_PHY_RXFD_REG162                       (PHY_RX_FD_BASE_ADDRESS + 0x4FC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG163                       (PHY_RX_FD_BASE_ADDRESS + 0x500)
#define	REG_PHY_RX_FD_PHY_RXFD_REG164                       (PHY_RX_FD_BASE_ADDRESS + 0x504)
#define	REG_PHY_RX_FD_PHY_RXFD_REG165                       (PHY_RX_FD_BASE_ADDRESS + 0x508)
#define	REG_PHY_RX_FD_PHY_RXFD_REG166                       (PHY_RX_FD_BASE_ADDRESS + 0x50C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG167                       (PHY_RX_FD_BASE_ADDRESS + 0x510)
#define	REG_PHY_RX_FD_PHY_RXFD_REG168                       (PHY_RX_FD_BASE_ADDRESS + 0x514)
#define	REG_PHY_RX_FD_PHY_RXFD_REG169                       (PHY_RX_FD_BASE_ADDRESS + 0x518)
#define	REG_PHY_RX_FD_PHY_RXFD_REG170                       (PHY_RX_FD_BASE_ADDRESS + 0x51C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG171                       (PHY_RX_FD_BASE_ADDRESS + 0x520)
#define	REG_PHY_RX_FD_PHY_RXFD_REG172                       (PHY_RX_FD_BASE_ADDRESS + 0x524)
#define	REG_PHY_RX_FD_PHY_RXFD_REG173                       (PHY_RX_FD_BASE_ADDRESS + 0x528)
#define	REG_PHY_RX_FD_PHY_RXFD_REG174                       (PHY_RX_FD_BASE_ADDRESS + 0x52C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG175                       (PHY_RX_FD_BASE_ADDRESS + 0x530)
#define	REG_PHY_RX_FD_PHY_RXFD_REG176                       (PHY_RX_FD_BASE_ADDRESS + 0x534)
#define	REG_PHY_RX_FD_PHY_RXFD_REG177                       (PHY_RX_FD_BASE_ADDRESS + 0x538)
#define	REG_PHY_RX_FD_PHY_RXFD_REG178                       (PHY_RX_FD_BASE_ADDRESS + 0x53C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG179                       (PHY_RX_FD_BASE_ADDRESS + 0x540)
#define	REG_PHY_RX_FD_PHY_RXFD_REG180                       (PHY_RX_FD_BASE_ADDRESS + 0x544)
#define	REG_PHY_RX_FD_PHY_RXFD_REG181                       (PHY_RX_FD_BASE_ADDRESS + 0x548)
#define	REG_PHY_RX_FD_PHY_RXFD_REG182                       (PHY_RX_FD_BASE_ADDRESS + 0x54C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG183                       (PHY_RX_FD_BASE_ADDRESS + 0x550)
#define	REG_PHY_RX_FD_PHY_RXFD_REG184                       (PHY_RX_FD_BASE_ADDRESS + 0x554)
#define	REG_PHY_RX_FD_PHY_RXFD_REG185                       (PHY_RX_FD_BASE_ADDRESS + 0x558)
#define	REG_PHY_RX_FD_PHY_RXFD_REG186                       (PHY_RX_FD_BASE_ADDRESS + 0x55C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG187                       (PHY_RX_FD_BASE_ADDRESS + 0x560)
#define	REG_PHY_RX_FD_PHY_RXFD_REG188                       (PHY_RX_FD_BASE_ADDRESS + 0x564)
#define	REG_PHY_RX_FD_PHY_RXFD_REG189                       (PHY_RX_FD_BASE_ADDRESS + 0x568)
#define	REG_PHY_RX_FD_PHY_RXFD_REG190                       (PHY_RX_FD_BASE_ADDRESS + 0x56C)
#define	REG_PHY_RX_FD_PHY_RXFD_REG191                       (PHY_RX_FD_BASE_ADDRESS + 0x570)
#define	REG_PHY_RX_FD_PHY_RXFD_REG192                       (PHY_RX_FD_BASE_ADDRESS + 0x574)
#define	REG_PHY_RX_FD_SPARE_FROM_FD                         (PHY_RX_FD_BASE_ADDRESS + 0x578)
#define	REG_PHY_RX_FD_SPARE_FROM_TX                         (PHY_RX_FD_BASE_ADDRESS + 0x57C)
#define	REG_PHY_RX_FD_SPARE_FROM_BE                         (PHY_RX_FD_BASE_ADDRESS + 0x580)
#define	REG_PHY_RX_FD_SPARE_FROM_TD                         (PHY_RX_FD_BASE_ADDRESS + 0x584)
#define	REG_PHY_RX_FD_SPARE_TO_FD                           (PHY_RX_FD_BASE_ADDRESS + 0x588)
#define	REG_PHY_RX_FD_SPARE_TO_TX                           (PHY_RX_FD_BASE_ADDRESS + 0x58C)
#define	REG_PHY_RX_FD_SPARE_TO_BE                           (PHY_RX_FD_BASE_ADDRESS + 0x590)
#define	REG_PHY_RX_FD_SPARE_TO_TD                           (PHY_RX_FD_BASE_ADDRESS + 0x594)
#define	REG_PHY_RX_FD_NUM_OF_ACTIVE_USERS                   (PHY_RX_FD_BASE_ADDRESS + 0x598)
#define	REG_PHY_RX_FD_FD_TEST_BUS_GCLK_EN                   (PHY_RX_FD_BASE_ADDRESS + 0x59C)
#define	REG_PHY_RX_FD_DFE_ENABLE                            (PHY_RX_FD_BASE_ADDRESS + 0x5A0)
#define	REG_PHY_RX_FD_QR_LOW_POWER                          (PHY_RX_FD_BASE_ADDRESS + 0x5A4)
#define	REG_PHY_RX_FD_TEST_BUS_GROUPING                     (PHY_RX_FD_BASE_ADDRESS + 0x5A8)
#define	REG_PHY_RX_FD_MAX_QR_TIME                           (PHY_RX_FD_BASE_ADDRESS + 0x5AC)
#define	REG_PHY_RX_FD_FFT_OUT_MULT_SHIFT                    (PHY_RX_FD_BASE_ADDRESS + 0x5B0)
#define	REG_PHY_RX_FD_QR_NOISE_GAIN_FREEZE_DISABLE          (PHY_RX_FD_BASE_ADDRESS + 0x5B4)
#define	REG_PHY_RX_FD_MIN_SPHERE_CYCLES                     (PHY_RX_FD_BASE_ADDRESS + 0x5B8)
#define	REG_PHY_RX_FD_SMOOTH_WDATA_DELAY_RM                 (PHY_RX_FD_BASE_ADDRESS + 0x5BC)
#define	REG_PHY_RX_FD_RCR_MAX_VAL                           (PHY_RX_FD_BASE_ADDRESS + 0x5C0)
#define	REG_PHY_RX_FD_SU_BF_ERROR_MASK                      (PHY_RX_FD_BASE_ADDRESS + 0x5C4)
#define	REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG0                (PHY_RX_FD_BASE_ADDRESS + 0x5C8)
#define	REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG1                (PHY_RX_FD_BASE_ADDRESS + 0x5CC)
#define	REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG2                (PHY_RX_FD_BASE_ADDRESS + 0x5D0)
#define	REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG3                (PHY_RX_FD_BASE_ADDRESS + 0x5D4)
#define	REG_PHY_RX_FD_MU_TX_ERROR_CAUSE_MASK                (PHY_RX_FD_BASE_ADDRESS + 0x5D8)
#define	REG_PHY_RX_FD_BF_EQ_POWER_PER_SS                    (PHY_RX_FD_BASE_ADDRESS + 0x5DC)
#define	REG_PHY_RX_FD_QR_ACTIVATIONS                        (PHY_RX_FD_BASE_ADDRESS + 0x5E0)
#define	REG_PHY_RX_FD_NOISE_TRK_FREEZE_SHIFTER_THR          (PHY_RX_FD_BASE_ADDRESS + 0x5E4)
#define	REG_PHY_RX_FD_SUPERSEG_NOT_ON_PRIMARY_CB            (PHY_RX_FD_BASE_ADDRESS + 0x5E8)
#define	REG_PHY_RX_FD_HE_DFE_SYM_START                      (PHY_RX_FD_BASE_ADDRESS + 0x5EC)
#define	REG_PHY_RX_FD_VHT_DFE_SYM_START                     (PHY_RX_FD_BASE_ADDRESS + 0x5F0)
#define	REG_PHY_RX_FD_START_FROM_QR_0_ENABLE                (PHY_RX_FD_BASE_ADDRESS + 0x5F4)
#define	REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT0           (PHY_RX_FD_BASE_ADDRESS + 0x5F8)
#define	REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT1           (PHY_RX_FD_BASE_ADDRESS + 0x5FC)
#define	REG_PHY_RX_FD_PHY_RXFD_REG_RAM_RM                   (PHY_RX_FD_BASE_ADDRESS + 0x600)
#define	REG_PHY_RX_FD_PHY_RXFD_REG_CH_ESTI                  (PHY_RX_FD_BASE_ADDRESS + 0x604)
#define	REG_PHY_RX_FD_QR_LIMIT_1SS                          (PHY_RX_FD_BASE_ADDRESS + 0x608)
#define	REG_PHY_RX_FD_QR_LIMIT_2SS                          (PHY_RX_FD_BASE_ADDRESS + 0x60C)
#define	REG_PHY_RX_FD_QR_LIMIT_3SS                          (PHY_RX_FD_BASE_ADDRESS + 0x610)
#define	REG_PHY_RX_FD_QR_LIMIT_4SS                          (PHY_RX_FD_BASE_ADDRESS + 0x614)
#define	REG_PHY_RX_FD_RISC_SPHERE_CYCLES_REWIND_OVERRIDE    (PHY_RX_FD_BASE_ADDRESS + 0x618)
#define	REG_PHY_RX_FD_PM_FD_OVERHEAD                        (PHY_RX_FD_BASE_ADDRESS + 0x61C)
#define	REG_PHY_RX_FD_PHY_RXFD_BF_SETUP                     (PHY_RX_FD_BASE_ADDRESS + 0x620)
#define	REG_PHY_RX_FD_SMOOTHING_LONG_FILTER_ENABLE          (PHY_RX_FD_BASE_ADDRESS + 0x624)
#define	REG_PHY_RX_FD_SPHERE_CYC_CT_CAN_WORK                (PHY_RX_FD_BASE_ADDRESS + 0x628)
#define	REG_PHY_RX_FD_TD_GAIN_CONFIG                        (PHY_RX_FD_BASE_ADDRESS + 0x62C)
#define	REG_PHY_RX_FD_SPHERE_POWER_SAVE                     (PHY_RX_FD_BASE_ADDRESS + 0x630)
#define	REG_PHY_RX_FD_SPHERE_CYCLES_RO                      (PHY_RX_FD_BASE_ADDRESS + 0x634)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_PHY_RX_FD_PHY_RXFD_REG06 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptBypass : 1; //phase tracking bypass - zero outputs, reset value: 0x0, access type: RW
		uint32 ncoFuncBypass : 1; //nco functional bypass (force phase addr to zero), reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 chTrkBypass : 1; //ch_trk_bypass, reset value: 0x0, access type: RW
		uint32 indpndtNeSwBypass : 1; //independent noise estimation sw bypass, reset value: 0x0, access type: RW
		uint32 dpndtNeSwBypass : 1; //dependent noise estimation sw bypass, reset value: 0x0, access type: RW
		uint32 chTrkBypassLegacyPreamble : 1; //ch_trk_bypass_legacy_preamble, reset value: 0x1, access type: RW
		uint32 rxEnableOverride : 1; //GSM - test mode, reset value: 0x0, access type: RW
		uint32 dpndtNeSymmetricMode : 1; //dpndt_ne_symmetric_mode, reset value: 0x1, access type: RW
		uint32 spurCancSwBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 20;
	} bitFields;
} RegPhyRxFdPhyRxfdReg06_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG07 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftOlaSelect : 4; //fft_ola_select[3],fft_ola_select[2],fft_ola_select[1],fft_ola_select[0], reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 fftRnd : 2; //fft output round bit selection in CB80, reset value: 0x1, access type: RW
		uint32 reserved1 : 10;
		uint32 fcbShortCpTimShift : 5; //short cyclic prefix timing shift - 40M, reset value: 0x8, access type: RW
		uint32 reserved2 : 2;
		uint32 ncbShortCpTimShift : 5; //short cyclic prefix timing shift - 20M, reset value: 0x4, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg07_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG08 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftStartWaddr : 9; //fft_start_waddr, reset value: 0x1c0, access type: RW
		uint32 fftStartWaddr2X : 10; //fft_start_waddr_2x, reset value: 0x3c0, access type: RW
		uint32 fftStartWaddr4X : 11; //fft_start_waddr_4x, reset value: 0x7c0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg08_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG09 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortCpTimShift : 10; //short_cp_tim_shift, reset value: 0x0, access type: RW
		uint32 cp16TimShift : 10; //cp_1_6_tim_shift, reset value: 0x0, access type: RW
		uint32 cp32TimShift : 10; //cp_3_2_tim_shift, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg09_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0A 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptSmoothingVhtInitSym : 1; //Phase Tracking smoothing VHT init symbol , 0-second VHT_lLTF, 1-data_symbol, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 ptSmoothingPhaseInitMode : 2; //Phase Tracking smoothing - phase init mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 ptSmoothingTimingInitMode : 2; //Phase Tracking smoothing - timing init mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 ftEnable : 1; //frequency tracking enable, reset value: 0x0, access type: RW
		uint32 reserved3 : 7;
		uint32 numOfSymMask : 11; //frequency tracking symbol mask, reset value: 0x0, access type: RW
		uint32 rfDataType : 1; //frequency tracking data type, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg0A_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0B 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 5;
		uint32 ct2TbEn : 1; //channel (pilot & data) to test bus en (at end of packet), reset value: 0x0, access type: RW
		uint32 ctForceLlrValidChTrk : 1; //ct_force_llr_valid_ch_trk, reset value: 0x0, access type: RW
		uint32 ctForceLlrValidNoiseTrk : 1; //ct_force_llr_valid_noise_trk, reset value: 0x0, access type: RW
		uint32 ctTxEqualNormMode : 1; //ct_tx_equal_norm_mode, reset value: 0x0, access type: RW
		uint32 toneRotationEn : 1; //tone_rotation_en, reset value: 0x1, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegPhyRxFdPhyRxfdReg0B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0C 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceTable1Start : 10; //channel estimation (smoothing) HT and VHT 1SS 40M table start address, reset value: 0x40, access type: RW
		uint32 ceTable2Start : 10; //channel estimation (smoothing) VHT more than 1SS 80M table start address, reset value: 0xc2, access type: RW
		uint32 ceTable3Start : 10; //channel estimation (smoothing) VHT 1SS 80M table start address, reset value: 0x1c4, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg0C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0D 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceH0Set : 10; //channel estimation (smoothing) ho coefficients, reset value: 0xa, access type: CONST
		uint32 reserved0 : 6;
		uint32 ceH1Set : 10; //channel estimation (smoothing) h1 coefficients, reset value: 0x0, access type: CONST
		uint32 reserved1 : 6;
	} bitFields;
} RegPhyRxFdPhyRxfdReg0D_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0E 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceH2Set : 10; //channel estimation (smoothing) h2 coefficients, reset value: 0x3db, access type: CONST
		uint32 reserved0 : 6;
		uint32 ceH3Set : 10; //channel estimation (smoothing) h3 coefficients, reset value: 0x0, access type: CONST
		uint32 reserved1 : 6;
	} bitFields;
} RegPhyRxFdPhyRxfdReg0E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG0F 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceH4Set : 10; //channel estimation (smoothing) h4 coefficients, reset value: 0x9c, access type: CONST
		uint32 reserved0 : 6;
		uint32 ceH5Set : 10; //channel estimation (smoothing) h5 coefficients, reset value: 0x100, access type: CONST
		uint32 reserved1 : 6;
	} bitFields;
} RegPhyRxFdPhyRxfdReg0F_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG11 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 long1ScrmInitVal : 7; //long1_scrm_init_val, reset value: 0x3F, access type: CONST
		uint32 reserved0 : 1;
		uint32 fcbPrank3MaskAn0 : 8; //40M-scrambler pilots mask rank 3 antenna0, reset value: 0x3c, access type: CONST
		uint32 fcbPrank3MaskAn1 : 8; //40M-scrambler pilots mask rank 3 antenna1, reset value: 0x10, access type: CONST
		uint32 fcbPrank3MaskAn2 : 8; //40M-scrambler pilots mask rank 3 antenna2, reset value: 0x58, access type: CONST
	} bitFields;
} RegPhyRxFdPhyRxfdReg11_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG12 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 twoBandPrank1Mask : 8; //two_band-scrambler pilots mask rank 1, reset value: 0x11, access type: CONST
		uint32 long3FcbPrank1Mask : 8; //long3 40M-scrambler pilots mask rank 1, reset value: 0xf4, access type: CONST
		uint32 long3NcbPrank1Mask : 8; //long3 20M-scrambler pilots mask rank 1, reset value: 0xb0, access type: CONST
		uint32 ncbPrank3MaskAn0 : 8; //20M-scrambler pilots mask rank 3 antenna0, reset value: 0x30, access type: CONST
	} bitFields;
} RegPhyRxFdPhyRxfdReg12_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG13 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcbPrank1Mask : 8; //40M-scrambler pilots mask rank 1, reset value: 0x18, access type: CONST
		uint32 ncbPrank1Mask : 8; //20M-scrambler pilots mask rank 1, reset value: 0x10, access type: CONST
		uint32 ncbPrank3MaskAn1 : 8; //20M-scrambler pilots mask rank 3 antenna1, reset value: 0x50, access type: CONST
		uint32 ncbPrank3MaskAn2 : 8; //20M-scrambler pilots mask rank 3 antenna2, reset value: 0x90, access type: CONST
	} bitFields;
} RegPhyRxFdPhyRxfdReg13_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG14 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcbPrank2MaskAn0 : 8; //40M-scrambler pilots mask rank 2 antenna0, reset value: 0x3c, access type: CONST
		uint32 fcbPrank2MaskAn1 : 8; //40M-scrambler pilots mask rank 2 antenna1, reset value: 0x10, access type: CONST
		uint32 ncbPrank2MaskAn0 : 8; //20M-scrambler pilots mask rank 2 antenna0, reset value: 0x30, access type: CONST
		uint32 ncbPrank2MaskAn1 : 8; //20M-scrambler pilots mask rank 2 antenna1, reset value: 0x60, access type: CONST
	} bitFields;
} RegPhyRxFdPhyRxfdReg14_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG15 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tdHtModeJumpAdd : 4; //GSM td_ht_mode_jump_address, reset value: 0x0, access type: RW
		uint32 tdHtLtf2JumpAdd : 4; //GSM td_ht_ltf2_jump_address, reset value: 0x0, access type: RW
		uint32 tdHtCp1JumpAdd : 4; //GSM td_ht_cp_1_jump_address, reset value: 0x0, access type: RW
		uint32 tdHtCp0JumpAdd : 4; //GSM td_ht_cp_0_jump_address, reset value: 0x0, access type: RW
		uint32 tdHtLtf3JumpAdd : 4; //GSM td_ht_ltf3_jump_address, reset value: 0x0, access type: RW
		uint32 tdHtEltfSkipAdd : 4; //GSM td_ht_eltf_skip_addr , reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxFdPhyRxfdReg15_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG16 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fdHtModeJumpAdd : 4; //GSM fd_ht_mode_jump_address, reset value: 0x0, access type: RW
		uint32 fdHtLtf2JumpAdd : 4; //GSM fd_ht_ltf2_jump_address, reset value: 0x0, access type: RW
		uint32 fdHtBfLtfJumpAdd : 4; //GSM - fd extra ltf jump address, reset value: 0x7, access type: RW
		uint32 fdHtLtf3JumpAdd : 4; //GSM fd_ht_ltf3_jump_address, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg16_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG1B 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmAbortThr : 16; //gsm_abort_threshold, reset value: 0xFFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg1B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG1C 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lenTp0 : 1; //enable data to test_bus low, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 lenTp1 : 1; //enable data to test_bus mid, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 lenTp2 : 1; //enable data to test_bus high, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 lenTp3 : 1; //enable data to test_bus four, reset value: 0x0, access type: RW
		uint32 reserved3 : 15;
		uint32 gsm2TbRate : 4; //gsm information strobe rate x4  to the test bus, reset value: 0x1, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg1C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG1D 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareReg : 16; //spare register, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg1D_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG1E 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 tbRNonDiagonal : 1; //tb_r_non_diagonal, reset value: 0x0, access type: RW
		uint32 reserved1 : 28;
	} bitFields;
} RegPhyRxFdPhyRxfdReg1E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG34 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 swHtDetectorEn : 1; //1 - force detector output to be as sw_ht_detector_11n, reset value: 0x0, access type: CONST
		uint32 swHtDetector11N : 1; //force ht_detector output: , 0 - 11a, , 1- 11n, reset value: 0x0, access type: CONST
		uint32 cddPolarity : 2; //tim_est CDD polarity: , 00 - bypass , 01 - positive , 10- negative, reset value: 0x2, access type: CONST
		uint32 reserved1 : 1;
		uint32 ceUpdateCh : 1; //channel estimation - write back to channel, reset value: 0x1, access type: RW
		uint32 olaMode : 1; //ola mode, reset value: 0x0, access type: RW
		uint32 olaFftDone : 1; //fft done in ola mode, reset value: 0x0, access type: RO
		uint32 olaSwFftP : 1; //ola  mode - switch between the fft, reset value: 0x0, access type: WO
		uint32 greenfieldDtctrEn : 1; //enable green field detector, reset value: 0x1, access type: CONST
		uint32 reserved2 : 1;
		uint32 enableErrorEvent : 1; //GSM enable error event, reset value: 0x0, access type: RW
		uint32 reserved3 : 4;
	} bitFields;
} RegPhyRxFdPhyRxfdReg34_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG35 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceTable4Start : 10; //channel estimation VHT more than 1SS 40M table start address, reset value: 0x2c6, access type: RW
		uint32 ceTable5Start : 10; //channel estimation VHT more than 1SS 20M table start address, reset value: 0x348, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdReg35_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG36 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 20;
		uint32 swBistStart : 1; //sw_bist_start, reset value: 0x0, access type: RW
		uint32 clearRamMode : 1; //clear_ram_mode, reset value: 0x0, access type: RW
		uint32 testBusLowMuxCtrl : 2; //test_bus_low_mux_ctrl, reset value: 0x0, access type: RW
		uint32 testBusMidMuxCtrl : 2; //test_bus_mid_mux_ctrl, reset value: 0x1, access type: RW
		uint32 testBusHighMuxCtrl : 2; //test_bus_high_mux_ctrl, reset value: 0x2, access type: RW
		uint32 testBusFourMuxCtrl : 2; //test_bus_four_mux_ctrl, reset value: 0x3, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg36_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG38 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 fdGfLsigModeJumpAdd : 4; //GSM - fd (lsig) next address in case of greenfield (default htsig1), reset value: 0x4, access type: RW
		uint32 fdGfHtsig2ModeJumpAdd : 4; //GSM - fd (htsig2) next address in case of greenfield (default data), reset value: 0x2, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg38_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG3A 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 hypRxfdRcrRamRm : 3; //hyp_rxfd_rcr_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 hypRxfdNeRamRm : 3; //hyp_rxfd_ne_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 hypRxfdPhaseRamRm : 3; //hyp_rxfd_phase_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 hypRxfdPhaseSmoothingRamRm : 3; //hyp_rxfd_phase_smoothing_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 hypRxfdTimingSmoothingRamRm : 3; //hyp_rxfd_timing_smoothing_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg3A_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG3B 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdData0ChRamRm : 3; //hyp_rxfd_data_0_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 hypRxfdData1ChRamRm : 3; //hyp_rxfd_data_1_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 hypRxfdPilot0ChRamRm : 3; //hyp_rxfd_pilot_0_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 hypRxfdPilot1ChRamRm : 3; //hyp_rxfd_pilot_1_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 hypRxfdFft0ImageRamRm : 3; //hyp_rxfd_fft_0_image_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 hypRxfdFft0ImagePilotRamRm : 3; //hyp_rxfd_fft_0_image_pilot_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 hypRxfdPilot2ChRamRm : 3; //hyp_rxfd_pilot_2_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 hypRxfdPilot3ChRamRm : 3; //hyp_rxfd_pilot_3_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg3B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG3C 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdGsmRamStyleRm : 3; //hyp_rxfd_gsm_ram_style_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 hypRxfdESnrThrRamRm : 3; //hyp_rxfd_e_snr_thr_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 hypRxfdMuRamRm : 3; //hyp_rxfd_mu_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 hypRxfdNoiseTrkMuRamRm : 3; //hyp_rxfd_noise_trk_mu_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdReg3C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG3E 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 hypRxfdData2ChRamRm : 3; //hyp_rxfd_data_2_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 hypRxfdData3ChRamRm : 3; //hyp_rxfd_data_3_ch_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 hypRxfdImplChannPrePhaseRamRm : 3; //hyp_rxfd_impl_chann_pre_phase_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 hypRxfdImplChannPreTxRxRamRm : 3; //hyp_rxfd_impl_chann_pre_tx_rx_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg3E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG4E 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 effectiveSnrGclkBypass : 1; //effective_snr_gclk_bypass, reset value: 0x1, access type: RW
		uint32 bfRxParserGclkBypass : 1; //bf_rx_parser_gclk_bypass, reset value: 0x1, access type: RW
		uint32 guessUnitGclkBypass : 1; //guess_unit_gclk_bypass, reset value: 0x1, access type: RW
		uint32 fft0GclkBypass : 1; //no description, reset value: 0x1, access type: RW
		uint32 fft1GclkBypass : 1; //fft 1 gated clock bypass, reset value: 0x1, access type: RW
		uint32 fft2GclkBypass : 1; //fft 2 gated clock bypass, reset value: 0x1, access type: RW
		uint32 fft3GclkBypass : 1; //fft 3 gated clock bypass, reset value: 0x1, access type: RW
		uint32 slicerGclkBypass : 4; //slicer_gclk_bypass, reset value: 0xf, access type: RW
		uint32 qrInternalGclkBypass : 1; //qr_gclk_disable, reset value: 0x1, access type: RW
		uint32 sphereGclkBypass : 8; //sphere_gclk_bypass, reset value: 0xff, access type: RW
		uint32 ctGclkDis : 1; //channel tracking gated clock disable, reset value: 0x1, access type: RW
		uint32 reserved0 : 10;
		uint32 implChannPreGclkBypass : 1; //impl_chann_pre_gclk_bypass, reset value: 0x1, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg4E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG50 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase2 : 14; //compressed_fourier_phase_2, reset value: 0x3ffd, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase3 : 14; //compressed_fourier_phase_3, reset value: 0x3800, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg50_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG51 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase4 : 14; //compressed_fourier_phase_4, reset value: 0x39ba, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase5 : 14; //compressed_fourier_phase_5, reset value: 0x3aa9, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg51_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG57 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 channelEstimationUse3Ltfs : 1; //ignore stream #4 in channel estimation (orgnanizer) in HT 3ss, reset value: 0x0, access type: RW
		uint32 reserved1 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdReg57_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG68 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 numOfPgc1Values : 6; //Implicit channel preperation - number of PGC1 values, reset value: 0x0, access type: RW
		uint32 numOfPgc2Values : 6; //Implicit channel preperation - number of PGC2 values, reset value: 0x0, access type: RW
		uint32 numOfPgc3Values : 6; //Implicit channel preperation - number of PGC3 values, reset value: 0x0, access type: RW
		uint32 numOfLnaValues : 5; //Implicit channel preperation - number of lna values, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxFdPhyRxfdReg68_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG74 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 hypRxfdGclkDisable : 18; //hyp_rxfd_gclk_disable, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 memGlobalRm : 2; //mem_global_rm, reset value: 0x0, access type: RW
		uint32 memGlobalBistScrBypass : 1; //mem_global_bist_scr_bypass, reset value: 0x0, access type: RW
		uint32 memGlobalRamTestMode : 1; //mem_global_ram_test_mode, reset value: 0x0, access type: RW
		uint32 memGlobalRamTestRnmMode : 1; //mem_global_ram_test_rnm_mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdReg74_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG75 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseTrkAlphaFirst : 3; //noise_trk_alpha_first, reset value: 0x7, access type: RW
		uint32 reserved0 : 1;
		uint32 noiseTrkAlphaSecond : 3; //noise_trk_alpha_second, reset value: 0x6, access type: RW
		uint32 reserved1 : 11;
		uint32 noiseTrkBypassData : 1; //noise_trk_bypass_data, reset value: 0x0, access type: RW
		uint32 noiseTrkBypassSig : 1; //noise_trk_bypass_sig, reset value: 0x0, access type: RW
		uint32 noiseTrkNoiseGainLimit : 4; //Noise_trk_noise_gain_limit, reset value: 0xc, access type: RW
		uint32 noiseSplitEnable : 1; //noise_split_enable, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegPhyRxFdPhyRxfdReg75_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG76 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc0PmRmValue : 3; //qr_gc0_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc1PmRmValue : 3; //qr_gc1_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc2PmRmValue : 3; //qr_gc2_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc0Unit0HSMemRmValue : 3; //qr_gc0_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc0Unit0QVMemRmValue : 3; //qr_gc0_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc1Unit0HSMemRmValue : 3; //qr_gc1_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 qrGc1Unit0QVMemRmValue : 3; //qr_gc1_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 qrGc2Unit0HSMemRmValue : 3; //qr_gc2_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg76_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG77 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc2Unit0QVMemRmValue : 3; //qr_gc2_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc0Unit1HSMemRmValue : 3; //qr_gc0_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc0Unit1QVMemRmValue : 3; //qr_gc0_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc1Unit1HSMemRmValue : 3; //qr_gc1_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc1Unit1QVMemRmValue : 3; //qr_gc1_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc2Unit1HSMemRmValue : 3; //qr_gc2_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 qrGc2Unit1QVMemRmValue : 3; //qr_gc2_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 qrGc0Unit2HSMemRmValue : 3; //qr_gc0_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg77_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG78 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc0Unit2QVMemRmValue : 3; //qr_gc0_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc1Unit2HSMemRmValue : 3; //qr_gc1_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc1Unit2QVMemRmValue : 3; //qr_gc1_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc2Unit2HSMemRmValue : 3; //qr_gc2_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc2Unit2QVMemRmValue : 3; //qr_gc2_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrLpGcPhaseMemRmValue : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 qrLpHMemRmValue : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdReg78_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG79 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 qrSphereIfQapplyMem0RmValue : 3; //qr_sphere_if_qapply_mem_0_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrSphereIfQapplyMem1RmValue : 3; //qr_sphere_if_qapply_mem_1_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrSphereIfQapplyMem2RmValue : 3; //qr_sphere_if_qapply_mem_2_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrSphereIfQapplyMem3RmValue : 3; //qr_sphere_if_qapply_mem_3_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrSphereIfQrMem0RmValue : 3; //qr_sphere_if_qr_mem_0_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg79_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7A 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrSphereIfQrMem1RmValue : 3; //qr_sphere_if_qr_mem_1_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrSphereIfQrMem2RmValue : 3; //qr_sphere_if_qr_mem_2_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrSphereIfQrMem3RmValue : 3; //qr_sphere_if_qr_mem_3_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrSortMemRmValue : 3; //qr_sort_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrSortMemSphereRmValue : 3; //qr_sort_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrNcoMemRmValue : 3; //qr_nco_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 qrSphereOperationsCalcMemRmValue : 3; //qr_sphere_operations_calc_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved6 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7A_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7B 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrStartAddr : 10; //qr_start_addr, reset value: 0x0, access type: RW
		uint32 qrEndAddr : 10; //qr_end_addr, reset value: 0x5e, access type: RW
		uint32 qrStartAddr1Ss : 10; //qr_start_addr_1ss, reset value: 0x5f, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7C 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrEndAddr1Ss : 10; //qr_end_addr_1ss, reset value: 0x71, access type: RW
		uint32 svdCsdStartAddr : 10; //svd_csd_start_addr, reset value: 0x97, access type: RW
		uint32 svdCsdEndAddr : 10; //svd_csd_end_addr, reset value: 0xa2, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7D 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 svdTransCsdStartAddr : 10; //svd_trans_csd_start_addr, reset value: 0x3d4, access type: RW
		uint32 svdTransCsdEndAddr : 10; //svd_trans_csd_end_addr, reset value: 0x3dc, access type: RW
		uint32 svdQrStartAddr : 10; //svd_qr_start_addr, reset value: 0x3dd, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7D_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7E 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 svdQrEndAddr : 10; //svd_qr_end_addr, reset value: 0x3f8, access type: RW
		uint32 svdStartAddrImplicit : 10; //svd_start_addr_implicit, reset value: 0xa3, access type: RW
		uint32 svdEndAddrImplicit : 10; //svd_end_addr_implicit, reset value: 0x173, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG7F 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 svdMultByQEndAddr : 10; //svd_mult_by_q_end_addr, reset value: 0x0, access type: RW
		uint32 svdAddrForIndication : 10; //svd_addr_for_indication, reset value: 0x0, access type: RW
		uint32 compress4AntStartAddr : 10; //compress_4ant_start_addr, reset value: 0x174, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg7F_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG80 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compress4AntEndAddr : 10; //compress_4ant_end_addr, reset value: 0x1D3, access type: RW
		uint32 compress3AntStartAddr : 10; //compress_3ant_start_addr, reset value: 0x1D4, access type: RW
		uint32 compress3AntEndAddr : 10; //compress_3ant_end_addr, reset value: 0x1FD, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg80_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG81 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compress2AntStartAddr : 10; //compress_2ant_start_addr, reset value: 0x1FE, access type: RW
		uint32 compress2AntEndAddr : 10; //compress_2ant_end_addr, reset value: 0x20E, access type: RW
		uint32 decompress4AntStartAddr : 10; //decompress_4ant_start_addr, reset value: 0x20F, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg81_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG82 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 decompress4AntEndAddr : 10; //decompress_4ant_end_addr, reset value: 0x264, access type: RW
		uint32 decompress3AntStartAddr : 10; //decompress_3ant_start_addr, reset value: 0x265, access type: RW
		uint32 decompress3AntEndAddr : 10; //decompress_3ant_end_addr, reset value: 0x286, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg82_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG83 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 decompress2AntStartAddr : 10; //decompress_2ant_start_addr, reset value: 0x287, access type: RW
		uint32 decompress2AntEndAddr : 10; //decompress_2ant_end_addr, reset value: 0x291, access type: RW
		uint32 stbcStartAddr : 10; //stbc_start_addr, reset value: 0x72, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg83_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG84 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stbcEndAddr : 10; //stbc_end_addr, reset value: 0x96, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegPhyRxFdPhyRxfdReg84_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG86 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase0 : 14; //compressed_fourier_phase_0, reset value: 0x2001, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase1 : 14; //compressed_fourier_phase_1, reset value: 0x3ffd, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg86_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG87 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase6 : 14; //compressed_fourier_phase_6, reset value: 0x17fb, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase7 : 14; //compressed_fourier_phase_7, reset value: 0xcb2, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg87_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG88 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase8 : 14; //compressed_fourier_phase_8, reset value: 0x36b3, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase9 : 14; //compressed_fourier_phase_9, reset value: 0x39ba, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg88_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG89 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compressedFourierPhase10 : 14; //compressed_fourier_phase_10, reset value: 0xb44, access type: RW
		uint32 reserved0 : 2;
		uint32 compressedFourierPhase11 : 14; //compressed_fourier_phase_11, reset value: 0x37fe, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg89_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG8A 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ceTable6Start : 10; //channel estimation (smoothing) HT and VHT 1SS 20M table start address, reset value: 0x38c, access type: RW
		uint32 reserved0 : 2;
		uint32 ceTableEndAddr : 10; //channel estimation (smoothing) table end address, reset value: 0x3cf, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdReg8A_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG8B 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ncbPrank4MaskAn0 : 8; //20M-scrambler pilots mask rank 4 antenna0, reset value: 0x10, access type: RW
		uint32 ncbPrank4MaskAn1 : 8; //20M-scrambler pilots mask rank 4 antenna1, reset value: 0x20, access type: RW
		uint32 ncbPrank4MaskAn2 : 8; //20M-scrambler pilots mask rank 4 antenna2, reset value: 0x40, access type: RW
		uint32 ncbPrank4MaskAn3 : 8; //20M-scrambler pilots mask rank 4 antenna3, reset value: 0x80, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg8B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG8C 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcbPrank4MaskAn0 : 8; //40M-scrambler pilots mask rank 4 antenna0, reset value: 0x3c, access type: RW
		uint32 fcbPrank4MaskAn1 : 8; //40M-scrambler pilots mask rank 4 antenna1, reset value: 0x13, access type: RW
		uint32 fcbPrank4MaskAn2 : 8; //40M-scrambler pilots mask rank 4 antenna2, reset value: 0x58, access type: RW
		uint32 fcbPrank4MaskAn3 : 8; //40M-scrambler pilots mask rank 4 antenna3, reset value: 0x88, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg8C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG8D 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cb80Prank1Mask : 8; //80M-scrambler pilots mask rank 1, reset value: 0x18, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxFdPhyRxfdReg8D_u;

/*REG_PHY_RX_FD_PHY_RXFD_IF08E 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrMaxPrescale1SsQr : 3; //qr_max_prescale_1ss_qr, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 qrMaxPrescale2SsQr : 3; //qr_max_prescale_2ss_qr, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 qrMaxPrescale3SsQr : 3; //qr_max_prescale_3ss_qr, reset value: 0x1, access type: RW
		uint32 reserved2 : 1;
		uint32 qrMaxPrescale4SsQr : 3; //qr_max_prescale_4ss_qr, reset value: 0x2, access type: RW
		uint32 reserved3 : 1;
		uint32 qrMaxPrescale1SsQapply : 3; //qr_max_prescale_1ss_qapply, reset value: 0x1, access type: RW
		uint32 reserved4 : 1;
		uint32 qrMaxPrescale2SsQapply : 3; //qr_max_prescale_2ss_qapply, reset value: 0x1, access type: RW
		uint32 reserved5 : 1;
		uint32 qrMaxPrescale3SsQapply : 3; //qr_max_prescale_3ss_qapply, reset value: 0x1, access type: RW
		uint32 reserved6 : 1;
		uint32 qrMaxPrescale4SsQapply : 3; //qr_max_prescale_4ss_qapply, reset value: 0x2, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdIf08E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG90 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fdVhtSigbJumpAdd : 4; //GSM - fd_vht_sigb_jump_add, reset value: 0xb, access type: RW
		uint32 tdVhtSigbJumpAdd : 4; //GSM - td_vht_sigb_jump_add, reset value: 0xe, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxFdPhyRxfdReg90_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG91 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cb80ShortCpTimShift : 5; //short cyclic prefix timing shift - cb80, reset value: 0x10, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxFdPhyRxfdReg91_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG92 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEstiSmoothRmValue : 3; //ch_esti_smooth_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdReg92_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG93 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 long3Cb80Prank1Mask : 8; //80M long3 - scrambler pilots mask rank 1, reset value: 0x5f, access type: CONST
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxFdPhyRxfdReg93_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG94 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdGpr1 : 32; //RXFD spare reserved register, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg94_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG95 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdGpr2 : 32; //RXFD spare reserved register, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg95_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG96 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdGpr3 : 32; //RXFD spare reserved register, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg96_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG97 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdGpr4 : 32; //RXFD spare reserved register, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg97_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG98 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom12Ss0I : 8; //tx_ct_sm_matrix_custom1_2ss_0_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss0Q : 8; //tx_ct_sm_matrix_custom1_2ss_0_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss1I : 8; //tx_ct_sm_matrix_custom1_2ss_1_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss1Q : 8; //tx_ct_sm_matrix_custom1_2ss_1_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg98_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG99 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom12Ss2I : 8; //tx_ct_sm_matrix_custom1_2ss_2_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss2Q : 8; //tx_ct_sm_matrix_custom1_2ss_2_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss3I : 8; //tx_ct_sm_matrix_custom1_2ss_3_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss3Q : 8; //tx_ct_sm_matrix_custom1_2ss_3_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg99_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9A 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom12Ss4I : 8; //tx_ct_sm_matrix_custom1_2ss_4_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss4Q : 8; //tx_ct_sm_matrix_custom1_2ss_4_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss5I : 8; //tx_ct_sm_matrix_custom1_2ss_5_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss5Q : 8; //tx_ct_sm_matrix_custom1_2ss_5_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9A_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9B 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom12Ss6I : 8; //tx_ct_sm_matrix_custom1_2ss_6_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss6Q : 8; //tx_ct_sm_matrix_custom1_2ss_6_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss7I : 8; //tx_ct_sm_matrix_custom1_2ss_7_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom12Ss7Q : 8; //tx_ct_sm_matrix_custom1_2ss_7_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9B_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9C 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom22Ss0I : 8; //tx_ct_sm_matrix_custom2_2ss_0_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss0Q : 8; //tx_ct_sm_matrix_custom2_2ss_0_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss1I : 8; //tx_ct_sm_matrix_custom2_2ss_1_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss1Q : 8; //tx_ct_sm_matrix_custom2_2ss_1_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9C_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9D 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom22Ss2I : 8; //tx_ct_sm_matrix_custom2_2ss_2_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss2Q : 8; //tx_ct_sm_matrix_custom2_2ss_2_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss3I : 8; //tx_ct_sm_matrix_custom2_2ss_3_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss3Q : 8; //tx_ct_sm_matrix_custom2_2ss_3_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9D_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9E 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom22Ss4I : 8; //tx_ct_sm_matrix_custom2_2ss_4_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss4Q : 8; //tx_ct_sm_matrix_custom2_2ss_4_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss5I : 8; //tx_ct_sm_matrix_custom2_2ss_5_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss5Q : 8; //tx_ct_sm_matrix_custom2_2ss_5_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9E_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG9F 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom22Ss6I : 8; //tx_ct_sm_matrix_custom2_2ss_6_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss6Q : 8; //tx_ct_sm_matrix_custom2_2ss_6_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss7I : 8; //tx_ct_sm_matrix_custom2_2ss_7_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom22Ss7Q : 8; //tx_ct_sm_matrix_custom2_2ss_7_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg9F_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA0 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss0I : 8; //tx_ct_sm_matrix_custom1_3ss_0_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss0Q : 8; //tx_ct_sm_matrix_custom1_3ss_0_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss1I : 8; //tx_ct_sm_matrix_custom1_3ss_1_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss1Q : 8; //tx_ct_sm_matrix_custom1_3ss_1_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega0_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA1 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss2I : 8; //tx_ct_sm_matrix_custom1_3ss_2_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss2Q : 8; //tx_ct_sm_matrix_custom1_3ss_2_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss3I : 8; //tx_ct_sm_matrix_custom1_3ss_3_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss3Q : 8; //tx_ct_sm_matrix_custom1_3ss_3_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA2 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss4I : 8; //tx_ct_sm_matrix_custom1_3ss_4_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss4Q : 8; //tx_ct_sm_matrix_custom1_3ss_4_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss5I : 8; //tx_ct_sm_matrix_custom1_3ss_5_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss5Q : 8; //tx_ct_sm_matrix_custom1_3ss_5_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega2_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA3 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss6I : 8; //tx_ct_sm_matrix_custom1_3ss_6_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss6Q : 8; //tx_ct_sm_matrix_custom1_3ss_6_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss7I : 8; //tx_ct_sm_matrix_custom1_3ss_7_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss7Q : 8; //tx_ct_sm_matrix_custom1_3ss_7_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega3_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA4 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss8I : 8; //tx_ct_sm_matrix_custom1_3ss_8_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss8Q : 8; //tx_ct_sm_matrix_custom1_3ss_8_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss9I : 8; //tx_ct_sm_matrix_custom1_3ss_9_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss9Q : 8; //tx_ct_sm_matrix_custom1_3ss_9_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega4_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA5 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom13Ss10I : 8; //tx_ct_sm_matrix_custom1_3ss_10_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss10Q : 8; //tx_ct_sm_matrix_custom1_3ss_10_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss11I : 8; //tx_ct_sm_matrix_custom1_3ss_11_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom13Ss11Q : 8; //tx_ct_sm_matrix_custom1_3ss_11_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega5_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA6 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss0I : 8; //tx_ct_sm_matrix_custom2_3ss_0_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss0Q : 8; //tx_ct_sm_matrix_custom2_3ss_0_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss1I : 8; //tx_ct_sm_matrix_custom2_3ss_1_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss1Q : 8; //tx_ct_sm_matrix_custom2_3ss_1_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega6_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA7 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss2I : 8; //tx_ct_sm_matrix_custom2_3ss_2_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss2Q : 8; //tx_ct_sm_matrix_custom2_3ss_2_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss3I : 8; //tx_ct_sm_matrix_custom2_3ss_3_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss3Q : 8; //tx_ct_sm_matrix_custom2_3ss_3_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega7_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA8 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss4I : 8; //tx_ct_sm_matrix_custom2_3ss_4_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss4Q : 8; //tx_ct_sm_matrix_custom2_3ss_4_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss5I : 8; //tx_ct_sm_matrix_custom2_3ss_5_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss5Q : 8; //tx_ct_sm_matrix_custom2_3ss_5_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega8_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGA9 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss6I : 8; //tx_ct_sm_matrix_custom2_3ss_6_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss6Q : 8; //tx_ct_sm_matrix_custom2_3ss_6_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss7I : 8; //tx_ct_sm_matrix_custom2_3ss_7_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss7Q : 8; //tx_ct_sm_matrix_custom2_3ss_7_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRega9_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGAA 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss8I : 8; //tx_ct_sm_matrix_custom2_3ss_8_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss8Q : 8; //tx_ct_sm_matrix_custom2_3ss_8_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss9I : 8; //tx_ct_sm_matrix_custom2_3ss_9_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss9Q : 8; //tx_ct_sm_matrix_custom2_3ss_9_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegaa_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGAB 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtSmMatrixCustom23Ss10I : 8; //tx_ct_sm_matrix_custom2_3ss_10_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss10Q : 8; //tx_ct_sm_matrix_custom2_3ss_10_q, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss11I : 8; //tx_ct_sm_matrix_custom2_3ss_11_i, reset value: 0x0, access type: RW
		uint32 txCtSmMatrixCustom23Ss11Q : 8; //tx_ct_sm_matrix_custom2_3ss_11_q, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegab_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGBE 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 avgSnrAddFactor : 12; //avg_snr_add_factor, reset value: 0x373, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxFdPhyRxfdRegbe_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC0 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftRndHb20160 : 2; //fft output round bit selection in HB20160, reset value: 0x1, access type: RW
		uint32 fftRndHb40160 : 2; //fft output round bit selection in HB40160, reset value: 0x1, access type: RW
		uint32 fftRndHb80160 : 2; //fft output round bit selection in HB80160, reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 bypassHaltSphere : 1; //sphere will not be halted during operation (in rxfd_csm), reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc0_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC1 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muQrDecompress4AntStartAddr : 10; //mu_qr_decompress_4ant_start_addr, reset value: 0x292, access type: RW
		uint32 reserved0 : 2;
		uint32 muQrDecompress4AntEndAddr : 10; //mu_qr_decompress_4ant_end_addr, reset value: 0x2C4, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC2 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muQrDecompress3AntStartAddr : 10; //mu_qr_decompress_3ant_start_addr, reset value: 0x2C5, access type: RW
		uint32 reserved0 : 2;
		uint32 muQrDecompress3AntEndAddr : 10; //mu_qr_decompress_3ant_end_addr, reset value: 0x2E6, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc2_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC3 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muQrDecompress2AntStartAddr : 10; //mu_qr_decompress_2ant_start_addr, reset value: 0x2E7, access type: RW
		uint32 reserved0 : 2;
		uint32 muQrDecompress2AntEndAddr : 10; //mu_qr_decompress_2ant_end_addr, reset value: 0x2F3, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc3_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC4 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muQrQapplyStartAddr : 10; //mu_qr_decompress_2ant_start_addr, reset value: 0x2F4, access type: RW
		uint32 reserved0 : 2;
		uint32 muQrQapplyEndAddr : 10; //mu_qr_decompress_2ant_end_addr, reset value: 0x386, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc4_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC5 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muPhasesRmValue : 3; //mu_phases_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 muTrainEffRateRmValue : 3; //mu_train_eff_rate_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 muDeltaSnrRmValue : 3; //mu_delta_snr_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 hypRxfdESnrGamaRamRm : 3; //hyp_rxfd_e_snr_gama_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved3 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc5_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGC6 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muQr4X2StartAddr : 10; //mu_qr_qapply_4x2_start_addr, reset value: 0x387, access type: RW
		uint32 reserved0 : 2;
		uint32 muQr4X2EndAddr : 10; //mu_qr_qapply_4x2_end_addr, reset value: 0x3D3, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegPhyRxFdPhyRxfdRegc6_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGCB 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfHtCodebookInfo : 2; //bf_ht_codebook_info, reset value: 0x3, access type: RW
		uint32 bfVhtCodebookInfo : 1; //bf_ht_codebook_info, reset value: 0x1, access type: RW
		uint32 bfVhtMuCodebookInfo : 1; //bf_ht_codebook_info, reset value: 0x1, access type: RW
		uint32 bfHeCodebookInfo : 1; //no description, reset value: 0x1, access type: RW
		uint32 bfHeMuCodebookInfo : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxFdPhyRxfdRegcb_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGCC 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lltfSmoothEn : 1; //channel_smoothing_l_ltf, reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 htltfSmoothEn : 1; //channel_smoothing_ht_ltf, reset value: 0x1, access type: RW
		uint32 reserved1 : 3;
		uint32 forceSpecialFilters : 1; //force_special_filters, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 useSnrEstL : 1; //snr_est_for_legacy, reset value: 0x1, access type: RW
		uint32 useSnrEstHt : 1; //snr_est_for_non_legacy, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 cpOverride : 2; //override_cp_filter, reset value: 0x0, access type: RW
		uint32 reserved4 : 2;
		uint32 smoothIfBf : 1; //override_cp_filter, reset value: 0x0, access type: RW
		uint32 reserved5 : 3;
		uint32 smoothHeSuBf2XEn : 1; //smooth_he_su_bf_2x_en, reset value: 0x1, access type: RW
		uint32 reserved6 : 3;
		uint32 smoothHeSuBf4XEn : 1; //smooth_he_su_bf_4x_en, reset value: 0x1, access type: RW
		uint32 reserved7 : 3;
	} bitFields;
} RegPhyRxFdPhyRxfdRegcc_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGCD 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 snrThresholdLow : 7; //snr_threshold_low, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 snrThresholdMedium : 7; //snr_threshold_medium, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 snrThresholdHigh : 7; //snr_threshold_high, reset value: 0x0, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdRegcd_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGCE 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timingOffsetCp04 : 4; //timing_offset_cp04, reset value: 0x0, access type: RW
		uint32 timingOffsetCp08 : 4; //timing_offset_cp08, reset value: 0x0, access type: RW
		uint32 timingOffsetCp16 : 4; //timing_offset_cp16, reset value: 0x0, access type: RW
		uint32 timingOffsetCp32 : 4; //timing_offset_cp32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdRegce_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGCF 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptSuPhHisBpsk : 4; //pt_su_ph_his_bpsk, reset value: 0x0, access type: RW
		uint32 ptSuPhHisQpsk : 4; //pt_su_ph_his_qpsk, reset value: 0x0, access type: RW
		uint32 ptSuPhHis16Qam : 4; //pt_su_ph_his_16qam, reset value: 0x0, access type: RW
		uint32 ptSuPhHis64Qam : 4; //pt_su_ph_his_64qam, reset value: 0x0, access type: RW
		uint32 ptTrigPhHisBpsk : 4; //pt_trig_ph_his_bpsk, reset value: 0x0, access type: RW
		uint32 ptTrigPhHisQpsk : 4; //pt_trig_ph_his_qpsk, reset value: 0x0, access type: RW
		uint32 ptTrigPhHis16Qam : 4; //pt_trig_ph_his_16qam, reset value: 0x0, access type: RW
		uint32 ptTrigPhHis64Qam : 4; //pt_trig_ph_his_64qam, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegcf_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD0 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptSuTimHisBpsk : 5; //pt_su_tim_his_bpsk, reset value: 0x0, access type: RW
		uint32 ptSuTimHisQpsk : 5; //pt_su_tim_his_qpsk, reset value: 0x0, access type: RW
		uint32 ptSuTimHis16Qam : 5; //pt_su_tim_his_16qam, reset value: 0x0, access type: RW
		uint32 ptSuTimHis64Qam : 5; //pt_su_tim_his_64qam, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdRegd0_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD1 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptTrigTimHisBpsk : 5; //pt_trig_tim_his_bpsk, reset value: 0x0, access type: RW
		uint32 ptTrigTimHisQpsk : 5; //pt_trig_tim_his_qpsk, reset value: 0x0, access type: RW
		uint32 ptTrigTimHis16Qam : 5; //pt_trig_tim_his_16qam, reset value: 0x0, access type: RW
		uint32 ptTrigTimHis64Qam : 5; //pt_trig_tim_his_64qam, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdRegd1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD2 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptDupPhaseTrack : 1; //pt_dup_phase_track, reset value: 0x0, access type: RW
		uint32 ptMisc : 31; //pt_misc, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd2_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD3 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 long3Cb160Prank1Mask : 16; //160M long3 - scrambler pilots mask rank 1, reset value: 0x5f5f, access type: RW
		uint32 cb160Prank1Mask : 16; //160M  - scrambler pilots mask rank 1, reset value: 0x1818, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd3_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD4 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 smoothInterpFilterRamRm : 3; //smooth_interp_filter_ram_rm, reset value: 0x0, access type: RW
		uint32 smoothInterpUserRuMapRm : 3; //smooth_interp_user_ru_map_rm, reset value: 0x0, access type: RW
		uint32 smoothInterpMcsSnrRamRm : 3; //smooth_interp_mcs_snr_ram_rm, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegPhyRxFdPhyRxfdRegd4_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD5 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 smoothInterpMisc : 32; //smooth_interp_misc, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd5_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD6 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 smoothInterpMisc2 : 32; //smooth_interp_misc2, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd6_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD7 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEnergyENormLut30 : 32; //ch_energy_e_norm_lut_3_0, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd7_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD8 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEnergyENormLut74 : 32; //ch_energy_e_norm_lut_7_4, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd8_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGD9 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEnergyENormLut118 : 32; //ch_energy_e_norm_lut_11_8, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegd9_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDA 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEnergyENormLut1512 : 32; //ch_energy_e_norm_lut_15_12, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegda_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDB 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chEnergyENormLutSnrShift : 9; //ch_energy_e_norm_lut_snr_shift, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegPhyRxFdPhyRxfdRegdb_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDC 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heFdDetSnrThreshold : 7; //he_fd_det_snr_threshold, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 heFdDetVitErrorThreshold : 7; //he_fd_det_vit_error_threshold, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 heFdDetDecisionThreshold : 7; //he_fd_det_decision_threshold, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 heFdDetMaxDetBw : 2; //he_fd_det_max_det_bw, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 heFdDetExtOnly : 1; //he_fd_det_ext_only, reset value: 0x0, access type: RW
		uint32 reserved4 : 2;
		uint32 heFdDetEn : 1; //he_fd_det_en, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegdc_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDD 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftOutShift26B : 2; //fft_out_shift_26b, reset value: 0x3, access type: RW
		uint32 fftOutShift52B : 2; //fft_out_shift_52b, reset value: 0x0, access type: RW
		uint32 fftOutShift106B : 2; //fft_out_shift_106b, reset value: 0x0, access type: RW
		uint32 fftOutShift20 : 2; //fft_out_shift_20, reset value: 0x0, access type: RW
		uint32 fftOutShift40 : 2; //fft_out_shift_40, reset value: 0x0, access type: RW
		uint32 fftOutShift80 : 2; //fft_out_shift_80, reset value: 0x0, access type: RW
		uint32 fftOutShift160 : 2; //fft_out_shift_160, reset value: 0x0, access type: RW
		uint32 fftOutShiftHestf : 2; //fft_out_shift_hestf, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdRegdd_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDE 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftOutMult26B : 4; //fft_out_mult_26b, reset value: 0x8, access type: RW
		uint32 fftOutMult52B : 4; //fft_out_mult_52b, reset value: 0x8, access type: RW
		uint32 fftOutMult106B : 4; //fft_out_mult_106b, reset value: 0x8, access type: RW
		uint32 fftOutMult20 : 4; //fft_out_mult_20, reset value: 0x9, access type: RW
		uint32 fftOutMult40 : 4; //fft_out_mult_40, reset value: 0x9, access type: RW
		uint32 fftOutMult80 : 4; //fft_out_mult_80, reset value: 0xb, access type: RW
		uint32 fftOutMult160 : 4; //fft_out_mult_160, reset value: 0xb, access type: RW
		uint32 fftOutMultHestf : 4; //fft_out_mult_hestf, reset value: 0x8, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegde_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGDF 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptPhaseLimit : 10; //pt_phase_limit, reset value: 0x1ff, access type: RW
		uint32 ptPhaseLimitTrigBased : 10; //pt_phase_limit_trig_based, reset value: 0x1ff, access type: RW
		uint32 ptTimingLimit : 10; //pt_timing_limit, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdRegdf_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE0 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ptTimingLimitTrigBased : 10; //pt_timing_limit_trig_based, reset value: 0x1ff, access type: RW
		uint32 ptFftKUserRamRm : 5; //pt_fft_k_user_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdRege0_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE1 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseInitRegFromTd : 23; //noise_init_reg_from_td, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdRege1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE2 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiPerRuRamRm : 3; //rssi_per_ru_ram_rm, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdRege2_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE3 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heFdDetPreLoadRamRm : 3; //he_fd_det_pre_load_ram_rm, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdRege3_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE4 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heFdDetLsigMetric : 15; //he_fd_det_lsig_metric, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 heFdDetRlsigMetric : 15; //he_fd_det_rlsig_metric, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdRege4_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE5 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heFdDetMetric : 15; //he_fd_det_metric, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdRege5_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE6 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataE4InForIir0 : 32; //ct_data_e4_in_for_iir0, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRege6_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE7 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataE4InForIir1 : 32; //ct_data_e4_in_for_iir1, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRege7_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE8 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataE4InForIir2 : 32; //ct_data_e4_in_for_iir2, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRege8_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGE9 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataE4InForIir3 : 32; //ct_data_e4_in_for_iir3, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRege9_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGEA 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataE4InForIir4 : 32; //ct_data_e4_in_for_iir4, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRegea_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGEB 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereMaxLlr : 10; //sphere_max_llr, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereNoisePowerProgModel : 3; //sphere_noise_power_prog_model, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdRegeb_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGEC 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereTreePartThr : 3; //sphere_tree_part_thr, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 sphereLlrThreshold : 5; //sphere_llr_threshold, reset value: 0x0, access type: RW
		uint32 sphereNoisePowerTreePartThr : 1; //sphere_noise_power_tree_part_thr, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegPhyRxFdPhyRxfdRegec_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGED 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 linearFifoRamRm : 3; //linear_fifo_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 sphereFifoRamRm : 3; //sphere_fifo_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 tcrRamRm : 3; //tcr_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved2 : 21;
	} bitFields;
} RegPhyRxFdPhyRxfdReged_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGEE 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereBus0Len : 1; //sphere_bus0_len, reset value: 0x0, access type: RW
		uint32 sphereBus1Len : 1; //sphere_bus1_len, reset value: 0x0, access type: RW
		uint32 sphereBus2Len : 1; //sphere_bus2_len, reset value: 0x0, access type: RW
		uint32 sphereBus3Len : 1; //sphere_bus3_len, reset value: 0x0, access type: RW
		uint32 sphereTbMode : 1; //sphere_tb_mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxFdPhyRxfdRegee_u;

/*REG_PHY_RX_FD_PHY_RXFD_APB_CONTROL 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfdApbDebugSelect : 2; //apb debug select: 0 -ch_ram, 1-su_db, 2-mu db, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 chPage : 4; //ch_page, reset value: 0x0, access type: RW
		uint32 suPage : 3; //su_page, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 muPage : 1; //mu_page, reset value: 0x0, access type: RW
		uint32 reserved2 : 19;
	} bitFields;
} RegPhyRxFdPhyRxfdApbControl_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF1 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerLoadGainSw : 1; //power_load_gain_sw , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF2 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chRamBankSel : 2; //ch_ram indirect access , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf2_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF3 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc3Unit0HSMemRmValue : 3; //qr_gc3_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc3Unit0QVMemRmValue : 3; //qr_gc3_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc3Unit1HSMemRmValue : 3; //qr_gc3_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc3Unit1QVMemRmValue : 3; //qr_gc3_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc3Unit2HSMemRmValue : 3; //qr_gc3_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc3Unit2QVMemRmValue : 3; //qr_gc3_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf3_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF4 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc4Unit0HSMemRmValue : 3; //qr_gc4_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc4Unit0QVMemRmValue : 3; //qr_gc4_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc4Unit1HSMemRmValue : 3; //qr_gc4_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc4Unit1QVMemRmValue : 3; //qr_gc4_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc4Unit2HSMemRmValue : 3; //qr_gc4_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc4Unit2QVMemRmValue : 3; //qr_gc4_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 muUlHramMemRmValue : 3; //mu_ul_hram_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved6 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf4_u;

/*REG_PHY_RX_FD_STREAMER_CONTROL 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 streamerEnable : 1; //streamer_enable, reset value: 0x0, access type: RW
		uint32 streamerMode : 1; //streamer_mode ,  , 1= loop , 0= transmit once, reset value: 0x0, access type: RW
		uint32 streamerAllAntennasSameData : 1; //1 = all antennas get the same data from bits [31:0] , 0 = each antenna gets different data from bits [n*32+31 : n*32], reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 streamerRate : 3; //0=640M , 1=320M , 2=160M , 3=80M , 4=40M, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegPhyRxFdStreamerControl_u;

/*REG_PHY_RX_FD_STREAMER_ADDRESS 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 streamerStartAddress : 14; //streamer_start_address, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 streamerEndAddress : 14; //streamer_end_address, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdStreamerAddress_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF7 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suPhaseRamSel : 1; //su_phase_ram_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf7_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF8 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 linearEvmMuMemRm : 3; //linear_evm_mu_mem_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 linearRcrMemRm : 3; //linear_rcr_mem_rm, reset value: 0x3, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf8_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGF9 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heStfNoiseGainRamRm : 3; //he_stf_noise_gain_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdRegf9_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFA 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainMaxValueMinusModPar : 8; //fft_gain_max_value_minus_mod_par, reset value: 0x0, access type: RW
		uint32 hestfFftGainFftInShiftHestf : 3; //hestf_fft_gain_fft_in_shift_hestf, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 hestfFftLimitGain : 1; //hestf_fft_limit_gain, reset value: 0x0, access type: RW
		uint32 reserved1 : 14;
		uint32 hestfFftGainEn : 1; //hestf_fft_gain_en, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdRegfa_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFB 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainExtraNoiseBackoffMcs0 : 7; //hestf_fft_gain_extra_noise_backoff_mcs0, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 hestfFftGainExtraNoiseBackoffMcs1 : 7; //hestf_fft_gain_extra_noise_backoff_mcs1, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 hestfFftGainExtraNoiseBackoffMcs2 : 7; //hestf_fft_gain_extra_noise_backoff_mcs2, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 hestfFftGainExtraNoiseBackoffMcs3 : 7; //hestf_fft_gain_extra_noise_backoff_mcs3, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdRegfb_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFC 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainExtraNoiseBackoffMcs4 : 7; //hestf_fft_gain_extra_noise_backoff_mcs4, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 hestfFftGainExtraNoiseBackoffMcs5 : 7; //hestf_fft_gain_extra_noise_backoff_mcs5, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 hestfFftGainExtraBackoffNdpFdbk : 7; //hestf_fft_gain_extra_backoff_ndp_fdbk, reset value: 0x0, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdRegfc_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFD 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainResInShift : 3; //hestf_fft_gain_res_in_shift, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 hestfFftGainResInMult : 1; //hestf_fft_gain_res_in_mult, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 hestfFftGainResOutMult : 4; //hestf_fft_gain_res_out_mult, reset value: 0x0, access type: RO
		uint32 reserved2 : 19;
		uint32 hestfFftGainResValid : 1; //hestf_fft_gain_res_valid, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRegfd_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFE 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfMiscRo : 32; //hestf_misc_ro, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRegfe_u;

/*REG_PHY_RX_FD_PHY_RXFD_REGFF 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfNoiseGainSigmaMin : 23; //hestf_noise_gain_sigma_min, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 hestfNoiseGainResValid : 1; //hestf_noise_gain_res_valid, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdRegff_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG100 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerPerRuTdGainAnt0 : 9; //power_per_ru_td_gain_ant0, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 powerPerRuTdGainAnt1 : 9; //power_per_ru_td_gain_ant1, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxFdPhyRxfdReg100_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG101 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerPerRuTdGainAnt2 : 9; //power_per_ru_td_gain_ant2, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 powerPerRuTdGainAnt3 : 9; //power_per_ru_td_gain_ant3, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegPhyRxFdPhyRxfdReg101_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG102 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerPerRuSystemGainAnt0 : 8; //power_per_ru_system_gain_ant0, reset value: 0x0, access type: RW
		uint32 powerPerRuSystemGainAnt1 : 8; //power_per_ru_system_gain_ant1, reset value: 0x0, access type: RW
		uint32 powerPerRuSystemGainAnt2 : 8; //power_per_ru_system_gain_ant2, reset value: 0x0, access type: RW
		uint32 powerPerRuSystemGainAnt3 : 8; //power_per_ru_system_gain_ant3, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg102_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG103 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerPerRuFftGain : 10; //power_per_ru_fft_gain, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 powerPerRuNormOffset : 8; //power_per_ru_norm_offset, reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegPhyRxFdPhyRxfdReg103_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG104 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfFftGainResValidClr : 1; //hestf_fft_gain_res_valid_clr, reset value: 0x0, access type: WO
		uint32 hestfNoiseGainResValidClr : 1; //hestf_noise_gain_res_valid_clr, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdReg104_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG105 0x414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfNoiseGainAnt0 : 7; //hestf_noise_gain_ant0, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 hestfNoiseGainAnt1 : 7; //hestf_noise_gain_ant1, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 hestfNoiseGainAnt2 : 7; //hestf_noise_gain_ant2, reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 hestfNoiseGainAnt3 : 7; //hestf_noise_gain_ant3, reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg105_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG106 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusHeader : 24; //test_bus_header, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxFdPhyRxfdReg106_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG107 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfNoiseGainBypass : 1; //hestf_noise_gain_bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdReg107_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG108 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereCycles : 14; //sphere_cycles, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereCyclesLastSym : 14; //sphere_cycles_last_sym, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg108_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG109 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereCycelsValidPulse : 1; //sphere_cycels_valid_pulse, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdReg109_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG110 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereCycelsGrEnable : 1; //sphere_cycels_gr_enable, reset value: 0x0, access type: RW
		uint32 linearFifoWaitMech : 1; //linear fifo to BE wiat number of sohere-cycles before issuing next.This bit enables this mechanism (chicken bit), reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdReg110_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG111 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdNcoRamRm : 3; //hyp_rxfd_nco_ram_rm, reset value: 0x3, access type: RW
		uint32 hypRxfdTrainingRamRm : 3; //hyp_rxfd_training_ram_rm, reset value: 0x3, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxFdPhyRxfdReg111_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG112 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd0 : 32; //su_db_valid_rd0, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg112_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG113 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd1 : 32; //su_db_valid_rd1, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg113_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG114 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd2 : 32; //su_db_valid_rd2, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg114_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG115 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd3 : 32; //su_db_valid_rd3, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg115_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG116 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd4 : 32; //su_db_valid_rd4, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg116_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG117 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd5 : 32; //su_db_valid_rd5, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg117_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG118 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd6 : 32; //su_db_valid_rd6, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg118_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG119 0x44C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidRd7 : 32; //su_db_valid_rd7, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg119_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG120 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw0 : 32; //su_db_valid_sw0, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg120_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG121 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw1 : 32; //su_db_valid_sw1, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg121_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG122 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw2 : 32; //su_db_valid_sw2, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg122_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG123 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw3 : 32; //su_db_valid_sw3, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg123_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG124 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw4 : 32; //su_db_valid_sw4, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg124_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG125 0x464 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw5 : 32; //su_db_valid_sw5, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg125_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG126 0x468 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw6 : 32; //su_db_valid_sw6, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg126_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG127 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidSw7 : 32; //su_db_valid_sw7, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg127_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG128 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suDbValidClr : 1; //su_db_valid_clr, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdReg128_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG129 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfError : 16; //su_bf_error, reset value: 0x0, access type: RO
		uint32 suBfOfdmaError : 3; //su_bf_ofdma_error, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
	} bitFields;
} RegPhyRxFdPhyRxfdReg129_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG130 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfOfdmaSel : 6; //su_bf_ofdma_sel, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxFdPhyRxfdReg130_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG131 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfErrClr : 1; //su_bf_err_clr, reset value: 0x0, access type: WO
		uint32 suBfOfdmaErrClr : 1; //su_bf_ofdma_err_clr, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdReg131_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG132 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainLowTh : 9; //noise_gain_low_th, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 noiseGainHighTh : 9; //noise_gain_high_th, reset value: 0x1ff, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegPhyRxFdPhyRxfdReg132_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG133 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainHighThCons0 : 9; //noise_gain_high_th_cons_0, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 3;
		uint32 noiseGainHighThCons1 : 9; //noise_gain_high_th_cons_1, reset value: 0x1ff, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegPhyRxFdPhyRxfdReg133_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG134 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainHighThCons2 : 9; //noise_gain_high_th_cons_2, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 3;
		uint32 noiseGainHighThCons3 : 9; //noise_gain_high_th_cons_3, reset value: 0x1ff, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegPhyRxFdPhyRxfdReg134_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG135 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainHighThCons4 : 9; //noise_gain_high_th_cons_4, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 3;
		uint32 noiseGainHighThCons5 : 9; //noise_gain_high_th_cons_5, reset value: 0x1ff, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegPhyRxFdPhyRxfdReg135_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG136 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainHighThCons6 : 9; //noise_gain_high_th_cons_6, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegPhyRxFdPhyRxfdReg136_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG137 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enSphereFastConvergance : 1; //en_sphere_fast_convergance, reset value: 0x0, access type: RW
		uint32 neSmoothingPipeMultCfg : 1; //ne_smoothing_pipe_mult_cfg , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdReg137_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG138 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU13 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles80MU14 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80MU15 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x2, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles80MU16 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x3, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg138_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG139 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU17 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles80MU18 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80MU19 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles80MU20 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg139_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG140 0x4A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU21 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles80MU22 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x3, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80MU23 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x4, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles80MU24 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x5, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg140_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG141 0x4A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU25 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x7, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles80MU26 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80MU27 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x9, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles80MU28 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0xa, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg141_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG142 0x4A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU29 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0xa, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles80MU30 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0xb, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80MU31 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0xc, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles80MU32 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0xd, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg142_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG143 0x4AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles160MU13 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x2, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU14 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x2, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles160MU15 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x2, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles160MU16 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x4, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg143_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG144 0x4B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles160MU17 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x4, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU18 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x5, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles160MU19 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x5, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles160MU20 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x5, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg144_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG145 0x4B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles160MU21 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x6, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU22 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x6, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles160MU23 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x7, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles160MU24 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x7, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg145_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG146 0x4B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles160MU25 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x7, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU26 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x7, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles160MU27 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles160MU28 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg146_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG147 0x4BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles160MU29 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU30 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles160MU31 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x9, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCycles160MU32 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x8, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg147_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG148 0x4C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseForAvSnr : 24; //noise_for_av_snr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegPhyRxFdPhyRxfdReg148_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG149 0x4C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCycles80MU12 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 sphereReduceCycles160MU12 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x1, access type: RW
		uint32 reserved1 : 2;
		uint32 sphereReduceCycles80P80 : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved2 : 2;
		uint32 sphereReduceCyclesRisc : 6; //reduce numbter of cycles for sphere operation to keep symbol time, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg149_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG150 0x4C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsFdStatus1 : 32; //global_enable            //31 , number_of_active_antennas//30:29 , packet_type              //28:26 , bf_sm             //25:21 , data_csm                 //20:18 , sm_csm                   //17:13 , pt_csm                   //12: 9 , ct_csm                   // 8: 5 , fd_csm                   // 4: 0 , reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg150_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG151 0x4D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsFdStatus2 : 32; //last_sym                 //31 , qam_type_out             //30:28 , data_rank                //27:26 , num_symbols              //25:11 , num_of_users             //10: 5 , symbol_type_in           // 4: 0 , reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg151_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG152 0x4D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 peakError4Ss0 : 8; //EVM eeror4 peak, reset value: 0x0, access type: RO
		uint32 peakError4Ss1 : 8; //EVM eeror4 peak, reset value: 0x0, access type: RO
		uint32 peakError4Ss2 : 8; //EVM eeror4 peak, reset value: 0x0, access type: RO
		uint32 peakError4Ss3 : 8; //EVM eeror4 peak, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg152_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG153 0x4D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testPlugLen : 32; //nco_test_plug_len, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg153_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG154 0x4DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeFeatureEnable : 1; //symbol time mechanism - global feature enable , reset value: 0x1, access type: RW
		uint32 symbolTimeLegacyEnable : 1; //symbol time mechanism - Legacy packets feature enable   , reset value: 0x1, access type: RW
		uint32 symbolTimeHtEnable : 1; //symbol time mechanism - HT packets feature enable , reset value: 0x1, access type: RW
		uint32 symbolTimeVhtEnable : 1; //symbol time mechanism - VHT packets feature enable    , reset value: 0x1, access type: RW
		uint32 symbolTimeHeSuEnable : 1; //symbol time mechanism - HE-SU packets feature enable  , reset value: 0x1, access type: RW
		uint32 symbolTimeHeTbEnable : 1; //symbol time mechanism - HE-TB packets feature enable , reset value: 0x1, access type: RW
		uint32 reserved0 : 10;
		uint32 symbolTimeHeSuLcpCount : 14; //symbol time mechanism - HE-SU long cp   - 16   usec, reset value: 0x2800, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg154_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG155 0x4E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeHeSuMcpCount : 14; //symbol time mechanism - HE-SU medium cp - 14.4 usec, reset value: 0x2400, access type: RW
		uint32 reserved0 : 2;
		uint32 symbolTimeHeSuScpCount : 14; //symbol time mechanism - HE-SU short cp  - 13.6 usec, reset value: 0x2200, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg155_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG156 0x4E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeVhtLcpCount : 14; //symbol time mechanism - Legacy/HT/VHT long cp timer  - 4   usec, reset value: 0xa00, access type: RW
		uint32 reserved0 : 2;
		uint32 symbolTimeVhtScpCount : 14; //no description, reset value: 0x900, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg156_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG157 0x4E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeHeTbLcpCount : 14; //symbol time mechanism - HE-TB long cp   - 16   usec, reset value: 0x2800, access type: RW
		uint32 reserved0 : 2;
		uint32 symbolTimeHeTbMcpCount : 14; //symbol time mechanism - HE-SU medium cp - 14.4 usec, reset value: 0x2400, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdPhyRxfdReg157_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG158 0x4EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stsFdStatus3 : 32; //pt_160m_80p80                    , //31 , band_mode_in                     , //30:26 , csm_number_of_sphere_cycles[11:0], //25:14 , csm_number_of_sphere_cores       , //13:9 , csm_number_of_sphere_activations}; // 8:0   , , reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg158_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG159 0x4F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 log2EvmDone : 1; //evm_done indication to RISC, reset value: 0x0, access type: RO
		uint32 lsigEvmDone : 1; //evm_done indication to RISC on Lsig , reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 lsigEvm : 8; //evm value  to RISC on Lsig, reset value: 0x0, access type: RO
		uint32 reserved1 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg159_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG160 0x4F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shortPacketScyclesEnable : 1; //short packets force number of cycles feature enanle, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 shortPacketScyclesBw : 4; //short packets force number of cycles feature BW supprt - 160,80,40,20, reset value: 0x8, access type: RW
		uint32 shortPacketScyclesSymbolThr : 8; //short packets force number of cycles number of symbols threshold, reset value: 0xf, access type: RW
		uint32 shortPacketScyclesUserThr : 6; //short packets force number of cycles number_of users threshold, reset value: 0x18, access type: RW
		uint32 shortPacketScyclesValue : 10; //short packets force number of cycles feature sphere cycles, reset value: 0x6b, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg160_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG161 0x4F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heExtMode : 1; //HE extended-range detectcted , reset value: 0x0, access type: RO
		uint32 heExtModeValid : 1; //HE extended-range detectcted - valid bit, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdPhyRxfdReg161_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG162 0x4FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceWafaTonesHeCp1 : 6; //reduce numbter of cycles for sphere operation for WAFA plans , reset value: 0xc, access type: RW
		uint32 sphereReduceWafaTonesHeCp2 : 6; //no description, reset value: 0x14, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxFdPhyRxfdReg162_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG163 0x500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainMax : 9; //First symbol max noise gain, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 noiseGainBinNum : 11; //Number of bins accumulated for noise gain sum, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdReg163_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG164 0x504 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseGainSum : 20; //Accumulated noise gain during first symbol, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdReg164_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG165 0x508 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereReduceCyclesMuUl20M : 8; //MU-UL spere cycles reduction for 20M, reset value: 0x0, access type: RW
		uint32 sphereReduceCyclesMuUl40M : 8; //MU-UL spere cycles reduction for 40M, reset value: 0x0, access type: RW
		uint32 sphereReduceCyclesMuUl80M : 8; //MU-UL spere cycles reduction for 80M, reset value: 0x0, access type: RW
		uint32 sphereReduceCyclesMuUl160M : 8; //MU-UL spere cycles reduction for 160M, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg165_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG166 0x50C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerPerRuTdGainAnt4 : 9; //power_per_ru_td_gain_ant4, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 powerPerRuSystemGainAnt4 : 8; //power_per_ru_system_gain_ant4, reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegPhyRxFdPhyRxfdReg166_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG167 0x510 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hestfNoiseGainAnt4 : 7; //hestf_noise_gain_ant3, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegPhyRxFdPhyRxfdReg167_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG168 0x514 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ncbPrank4MaskAn4 : 8; //20M-scrambler pilots mask rank 4 antenna4, reset value: 0x80, access type: RW
		uint32 fcbPrank4MaskAn4 : 8; //40M-scrambler pilots mask rank 4 antenna4, reset value: 0x88, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdPhyRxfdReg168_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG169 0x518 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 secondDecodeAfterAidedPhaseCorrection : 1; //KPI feature enable, reset value: 0x0, access type: RW
		uint32 secondPhaseTrackingEnSmoothing : 1; //KPI smoothing enable on channel, reset value: 0x0, access type: RW
		uint32 kpiPreambleSymbolsSupport : 1; //Preamble (non data symbols) support, reset value: 0x1, access type: RW
		uint32 kpiPacketTypeSupport : 5; //Packet type support (on DATA symbols only): , [0] --- legacy , [1] --- HT/VHT , [2] --- HE , [3] --- HE-TB (single RU only) , [4] --- HE-TB (multi RUs), reset value: 0xF, access type: RW
		uint32 kpiBandwidthSupport : 4; //BW to support : , [0] --- 20M , [1] --- 40M , [2] --- 80M , [3] --- 160M, reset value: 0x3, access type: RW
		uint32 secondPhaseTrackingAddedSmoothingSnr : 8; //KPI smoothing snr added, reset value: 0xA, access type: RW
		uint32 kpiSphereCycles20M : 12; //KPI sphere cycles 20M, reset value: 0xF6, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg169_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG170 0x51C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ndpFdbkKRatio4Ants : 5; //K ratio factor for ndp feedback with 4 rx antennas. K value here is double the real k ratio used in ndp feedback (U(4,5)*2^(-1). So if default is 6 then real value used is 3, reset value: 0x6, access type: RW
		uint32 ndpFdbkKRatio3Ants : 5; //K ratio factor for ndp feedback with 3 rx antennas. K value here is double the real k ratio used in ndp feedback (U(4,5)*2^(-1). So if default is 6 then real value used is 3, reset value: 0x6, access type: RW
		uint32 ndpFdbkKRatio2Ants : 5; //K ratio factor for ndp feedback with 2 rx antennas. K value here is double the real k ratio used in ndp feedback (U(4,5)*2^(-1). So if default is 6 then real value used is 3, reset value: 0x6, access type: RW
		uint32 ndpFdbkKRatio1Ants : 5; //K ratio factor for ndp feedback with 1 rx antenna.  K value here is double the real k ratio used in ndp feedback (U(4,5)*2^(-1). So if default is 6 then real value used is 3, reset value: 0x6, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdReg170_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG171 0x520 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ndpFdbkPThresholdCfg : 20; //power threshold for ndp feedback P0 and P1 power level, reset value: 0x1, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdPhyRxfdReg171_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG172 0x524 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muHdrMemoryRm : 5; //mu_hdr_memory_rm, reset value: 0x3, access type: RW
		uint32 suHdrMemoryRm : 5; //su_hdr_memory_rm, reset value: 0x3, access type: RW
		uint32 suPhasesRmValue : 3; //su_phases_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegPhyRxFdPhyRxfdReg172_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG173 0x528 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastQrNumOfCycels : 8; //Minimum gap between last QR to first QR in next symbol, reset value: 0x3F, access type: RW
		uint32 muUl4SsEnableNoiseTrk : 1; //in MU-UL 4ss only enable noise-track (default is disabled), reset value: 0x1, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegPhyRxFdPhyRxfdReg173_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG174 0x52C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc5Unit0HSMemRmValue : 3; //qr_gc5_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc5Unit0QVMemRmValue : 3; //qr_gc5_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc5Unit1HSMemRmValue : 3; //qr_gc5_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc5Unit1QVMemRmValue : 3; //qr_gc5_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc5Unit2HSMemRmValue : 3; //qr_gc5_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc5Unit2QVMemRmValue : 3; //qr_gc5_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdReg174_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG175 0x530 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc6Unit0HSMemRmValue : 3; //qr_gc6_unit0_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc6Unit0QVMemRmValue : 3; //qr_gc6_unit0_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc6Unit1HSMemRmValue : 3; //qr_gc6_unit1_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc6Unit1QVMemRmValue : 3; //qr_gc6_unit1_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 qrGc6Unit2HSMemRmValue : 3; //qr_gc6_unit2_h_s_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 qrGc6Unit2QVMemRmValue : 3; //qr_gc6_unit2_q_v_mem_rm_value, reset value: 0x3, access type: RW
		uint32 reserved5 : 9;
	} bitFields;
} RegPhyRxFdPhyRxfdReg175_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG176 0x534 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrGc3PmRmValue : 3; //qr_gc3_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 qrGc4PmRmValue : 3; //qr_gc4_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 qrGc5PmRmValue : 3; //qr_gc5_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 qrGc6PmRmValue : 3; //qr_gc6_pm_rm_value, reset value: 0x3, access type: RW
		uint32 reserved3 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdReg176_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG177 0x538 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 error4Min0 : 18; //EVM error4 min, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 error4Bin0 : 11; //EVM error4 bin, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg177_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG178 0x53C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 error4Min1 : 18; //EVM error4 min, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 error4Bin1 : 11; //EVM error4 bin, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg178_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG179 0x540 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 error4Min2 : 18; //EVM error4 min, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 error4Bin2 : 11; //EVM error4 bin, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg179_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG180 0x544 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 error4Min3 : 18; //EVM error4 min, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 error4Bin3 : 11; //EVM error4 bin, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegPhyRxFdPhyRxfdReg180_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG181 0x548 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csmSmoothingOldSchem : 1; //Smoothing old (per antenna) activation schem, reset value: 0x1, access type: RW
		uint32 csmPacketDetectOldSchem : 1; //HE-Ext packet detect old schem, reset value: 0x1, access type: RW
		uint32 disableSphereColumnSorting : 1; //disbale column sort, reset value: 0x0, access type: RW
		uint32 disableSphereColumnSorting4Ss : 1; //disbale column sort on 4-streams, reset value: 0x0, access type: RW
		uint32 csmWaitForSig1 : 1; //wait for SIG1 bfore PT at HT-Sig2, reset value: 0x1, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxFdPhyRxfdReg181_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG182 0x54C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wait1X1CycLeg : 11; //Min cycles num between slicer done to start next symbol for legacy, reset value: 0x15E, access type: RW
		uint32 reserved0 : 5;
		uint32 wait1X1CycHe : 11; //Min cycles num between slicer done to start next symbol for he, reset value: 0x7D0, access type: RW
		uint32 reserved1 : 5;
	} bitFields;
} RegPhyRxFdPhyRxfdReg182_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG183 0x550 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memRxfdDs : 32; //memories DS , reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg183_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG184 0x554 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memRxfdSd : 32; //memories SD, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg184_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG185 0x558 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memRxfdRop : 32; //memories ROP, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg185_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG186 0x55C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memRxfdLs1 : 32; //memories LS, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg186_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG187 0x560 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memRxfdLs2 : 32; //memories LS, reset value: 0x0, access type: RW
	} bitFields;
} RegPhyRxFdPhyRxfdReg187_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG188 0x564 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdDataChRam1X1Rm : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdPhyRxfdReg188_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG189 0x568 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftSaturationSamplesAnt0 : 8; //no description, reset value: 0x0, access type: RO
		uint32 fftSaturationSamplesAnt1 : 8; //no description, reset value: 0x0, access type: RO
		uint32 fftSaturationSamplesAnt2 : 8; //no description, reset value: 0x0, access type: RO
		uint32 fftSaturationSamplesAnt3 : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdPhyRxfdReg189_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG190 0x56C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftSaturationSamplesAnt4 : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegPhyRxFdPhyRxfdReg190_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG191 0x570 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 secondPhaseTrackingAddedSmoothingSymbols : 15; //Smoothing KPI symbol number limit, reset value: 0xA, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxFdPhyRxfdReg191_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG192 0x574 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 kpiSphereCycles40M : 12; //KPI sphere cycles - 40M (or more), reset value: 0xF6, access type: RW
		uint32 reserved0 : 4;
		uint32 reduceSphereCycLastSymOfdmaVit : 1; //sphere_cycles reduction in last symbol , reset value: 0x1, access type: RW
		uint32 reduceSphereCycLastSymMimo20Ldpc : 1; //sphere_cycles reduction in last symbol , reset value: 0x0, access type: RW
		uint32 reduceSphereCycLastSymOfdma20 : 1; //sphere_cycles reduction in last symbol , reset value: 0x0, access type: RW
		uint32 reduceSphereCycLastSymHeTrig40 : 1; //sphere_cycles reduction in last symbol , reset value: 0x0, access type: RW
		uint32 reduceSphereCycLastSymHeTrig80 : 1; //sphere_cycles reduction in last symbol , reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 kpiSmoothingSecondSymbol : 1; //enable kpi-smoothing on SIg2 , reset value: 0x0, access type: RW
		uint32 kpiSmoothingThirdSymbolNotLegacy : 1; //enable kpi-smoothing on Sig3, reset value: 0x0, access type: RW
		uint32 reserved2 : 6;
	} bitFields;
} RegPhyRxFdPhyRxfdReg192_u;

/*REG_PHY_RX_FD_SPARE_FROM_FD 0x578 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromFd : 16; //spare connectivity from FD, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareFromFd_u;

/*REG_PHY_RX_FD_SPARE_FROM_TX 0x57C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTx : 16; //spare connectivity from TX, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareFromTx_u;

/*REG_PHY_RX_FD_SPARE_FROM_BE 0x580 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromBe : 16; //spare connectivity from BE, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareFromBe_u;

/*REG_PHY_RX_FD_SPARE_FROM_TD 0x584 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareFromTd : 16; //spare connectivity from TD, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareFromTd_u;

/*REG_PHY_RX_FD_SPARE_TO_FD 0x588 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToFd : 16; //spare connectivity to FD, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareToFd_u;

/*REG_PHY_RX_FD_SPARE_TO_TX 0x58C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTx : 16; //spare connectivity to TX, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareToTx_u;

/*REG_PHY_RX_FD_SPARE_TO_BE 0x590 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToBe : 16; //spare connectivity to BE, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareToBe_u;

/*REG_PHY_RX_FD_SPARE_TO_TD 0x594 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareToTd : 16; //spare connectivity to TD, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSpareToTd_u;

/*REG_PHY_RX_FD_NUM_OF_ACTIVE_USERS 0x598 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 numOfActiveUsers : 6; //number of active users in band, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxFdNumOfActiveUsers_u;

/*REG_PHY_RX_FD_FD_TEST_BUS_GCLK_EN 0x59C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fdTestBusGclkEn : 1; //test bus gclk en, reset value: 0x0, access type: RW
		uint32 testBusGeneratorsEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdFdTestBusGclkEn_u;

/*REG_PHY_RX_FD_DFE_ENABLE 0x5A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dfeEnable : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdDfeEnable_u;

/*REG_PHY_RX_FD_QR_LOW_POWER 0x5A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrUpdNumInitSym : 5; //no description, reset value: 0xa, access type: RW
		uint32 reserved0 : 3;
		uint32 qrUpdPeriod : 4; //no description, reset value: 0x4, access type: RW
		uint32 qrUpdCountAllOfdmSymbols : 1; //no description, reset value: 0x0, access type: RW
		uint32 qrUpdNumInitSymDfe : 5; //no description, reset value: 0xa, access type: RW
		uint32 reserved1 : 14;
	} bitFields;
} RegPhyRxFdQrLowPower_u;

/*REG_PHY_RX_FD_TEST_BUS_GROUPING 0x5A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusGrouping : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdTestBusGrouping_u;

/*REG_PHY_RX_FD_MAX_QR_TIME 0x5AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxQrTime : 10; //no description, reset value: 0x1ec, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegPhyRxFdMaxQrTime_u;

/*REG_PHY_RX_FD_FFT_OUT_MULT_SHIFT 0x5B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fftOutShift60 : 2; //no description, reset value: 0x0, access type: RW
		uint32 fftOutMult60 : 4; //no description, reset value: 0x9, access type: RW
		uint32 fftOutShift120 : 2; //no description, reset value: 0x0, access type: RW
		uint32 fftOutMult120 : 4; //no description, reset value: 0x8, access type: RW
		uint32 fftOutShift140 : 2; //no description, reset value: 0x0, access type: RW
		uint32 fftOutMult140 : 4; //no description, reset value: 0x8, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegPhyRxFdFftOutMultShift_u;

/*REG_PHY_RX_FD_QR_NOISE_GAIN_FREEZE_DISABLE 0x5B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrNoiseGainFreezeDisable : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdQrNoiseGainFreezeDisable_u;

/*REG_PHY_RX_FD_MIN_SPHERE_CYCLES 0x5B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minSphereCycles2Qr : 10; //no description, reset value: 0x10E, access type: RW
		uint32 minSphereCycles6Qr : 10; //no description, reset value: 0x58, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegPhyRxFdMinSphereCycles_u;

/*REG_PHY_RX_FD_SMOOTH_WDATA_DELAY_RM 0x5BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 smoothWdataDelayRm : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPhyRxFdSmoothWdataDelayRm_u;

/*REG_PHY_RX_FD_RCR_MAX_VAL 0x5C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxNss : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 maxMcs : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegPhyRxFdRcrMaxVal_u;

/*REG_PHY_RX_FD_SU_BF_ERROR_MASK 0x5C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfErrorMask : 16; //no description, reset value: 0xFFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegPhyRxFdSuBfErrorMask_u;

/*REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG0 0x5C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfOfdmaErrorReg0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdSuBfOfdmaErrorReg0_u;

/*REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG1 0x5CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfOfdmaErrorReg1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdSuBfOfdmaErrorReg1_u;

/*REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG2 0x5D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfOfdmaErrorReg2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdSuBfOfdmaErrorReg2_u;

/*REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG3 0x5D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfOfdmaErrorReg3 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegPhyRxFdSuBfOfdmaErrorReg3_u;

/*REG_PHY_RX_FD_MU_TX_ERROR_CAUSE_MASK 0x5D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTxErrorCauseMask : 12; //no description, reset value: 0xFFF, access type: RW
		uint32 muTxErrorCause2Mask : 13; //no description, reset value: 0x1FFF, access type: RW
		uint32 reserved0 : 7;
	} bitFields;
} RegPhyRxFdMuTxErrorCauseMask_u;

/*REG_PHY_RX_FD_BF_EQ_POWER_PER_SS 0x5DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfEqPowerPerSs : 1; //no description, reset value: 0x0, access type: RW
		uint32 bfEqPowerPerSsNStreams : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegPhyRxFdBfEqPowerPerSs_u;

/*REG_PHY_RX_FD_QR_ACTIVATIONS 0x5E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrActivations : 7; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegPhyRxFdQrActivations_u;

/*REG_PHY_RX_FD_NOISE_TRK_FREEZE_SHIFTER_THR 0x5E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseTrkFreezeShifterThr : 2; //no description, reset value: 0x2, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdNoiseTrkFreezeShifterThr_u;

/*REG_PHY_RX_FD_SUPERSEG_NOT_ON_PRIMARY_CB 0x5E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 supersegNotOnPrimaryCb : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdSupersegNotOnPrimaryCb_u;

/*REG_PHY_RX_FD_HE_DFE_SYM_START 0x5EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heDfeSymStart : 15; //no description, reset value: 0x3, access type: WO
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxFdHeDfeSymStart_u;

/*REG_PHY_RX_FD_VHT_DFE_SYM_START 0x5F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vhtDfeSymStart : 15; //no description, reset value: 0x4, access type: WO
		uint32 reserved0 : 17;
	} bitFields;
} RegPhyRxFdVhtDfeSymStart_u;

/*REG_PHY_RX_FD_START_FROM_QR_0_ENABLE 0x5F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startFromQr0Enable : 1; //no description, reset value: 0x1, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdStartFromQr0Enable_u;

/*REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT0 0x5F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeEht2SegLcpCount : 14; //no description, reset value: 0x27fc, access type: WO
		uint32 symbolTimeEht2SegMcpCount : 14; //no description, reset value: 0x23fc, access type: WO
		uint32 reserved0 : 4;
	} bitFields;
} RegPhyRxFdSymbolTimeEht2SegCount0_u;

/*REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT1 0x5FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 symbolTimeEht2SegScpCount : 14; //no description, reset value: 0x21fc, access type: WO
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxFdSymbolTimeEht2SegCount1_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG_RAM_RM 0x600 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ampCorRefPrvRamRm : 3; //no description, reset value: 0x0, access type: RW
		uint32 hypRxfdChEstiRamRm : 3; //rm of ch esti rams , these are th ex ltf rams, reset value: 0x3, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegPhyRxFdPhyRxfdRegRamRm_u;

/*REG_PHY_RX_FD_PHY_RXFD_REG_CH_ESTI 0x604 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 exLtfNoiseEn : 1; //whe 1 snr and noise are calculated at extra ltfs, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdRegChEsti_u;

/*REG_PHY_RX_FD_QR_LIMIT_1SS 0x608 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrLimit1SsBand2G : 9; //no description, reset value: 0x52, access type: RW
		uint32 qrLimit1SsBand5G : 9; //no description, reset value: 0x44, access type: RW
		uint32 qrLimit1SsBand6G : 9; //no description, reset value: 0x44, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegPhyRxFdQrLimit1Ss_u;

/*REG_PHY_RX_FD_QR_LIMIT_2SS 0x60C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrLimit2SsBand2G : 9; //no description, reset value: 0x86, access type: RW
		uint32 qrLimit2SsBand5G : 9; //no description, reset value: 0x44, access type: RW
		uint32 qrLimit2SsBand6G : 9; //no description, reset value: 0x44, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegPhyRxFdQrLimit2Ss_u;

/*REG_PHY_RX_FD_QR_LIMIT_3SS 0x610 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrLimit3SsBand2G : 9; //no description, reset value: 0xbe, access type: RW
		uint32 qrLimit3SsBand5G : 9; //no description, reset value: 0x44, access type: RW
		uint32 qrLimit3SsBand6G : 9; //no description, reset value: 0x44, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegPhyRxFdQrLimit3Ss_u;

/*REG_PHY_RX_FD_QR_LIMIT_4SS 0x614 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qrLimit4SsBand2G : 9; //no description, reset value: 0xf6, access type: RW
		uint32 qrLimit4SsBand5G : 9; //no description, reset value: 0x52, access type: RW
		uint32 qrLimit4SsBand6G : 9; //no description, reset value: 0x52, access type: RW
		uint32 reserved0 : 5;
	} bitFields;
} RegPhyRxFdQrLimit4Ss_u;

/*REG_PHY_RX_FD_RISC_SPHERE_CYCLES_REWIND_OVERRIDE 0x618 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscSphereCyclesRewindOverride : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegPhyRxFdRiscSphereCyclesRewindOverride_u;

/*REG_PHY_RX_FD_PM_FD_OVERHEAD 0x61C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmFdOverhead : 12; //no description, reset value: 0x1f4, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegPhyRxFdPmFdOverhead_u;

/*REG_PHY_RX_FD_PHY_RXFD_BF_SETUP 0x620 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suBfInterp : 1; //when set to "1" : su and ofdma tx smoothing will interpolate groups. When "0" - obselete  mode of group duplication., reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdPhyRxfdBfSetup_u;

/*REG_PHY_RX_FD_SMOOTHING_LONG_FILTER_ENABLE 0x624 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 smoothingLongFilterEnable : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdSmoothingLongFilterEnable_u;

/*REG_PHY_RX_FD_SPHERE_CYC_CT_CAN_WORK 0x628 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereCycCtCanWork : 14; //no description, reset value: 0x51, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegPhyRxFdSphereCycCtCanWork_u;

/*REG_PHY_RX_FD_TD_GAIN_CONFIG 0x62C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pmSuBfTdGainEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 pmSuBfSpmapNormalizationEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegPhyRxFdTdGainConfig_u;

/*REG_PHY_RX_FD_SPHERE_POWER_SAVE 0x630 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spherePowerSave : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegPhyRxFdSpherePowerSave_u;

/*REG_PHY_RX_FD_SPHERE_CYCLES_RO 0x634 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereCyclesInitVal : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 sphereCyclesRewind : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegPhyRxFdSphereCyclesRo_u;

//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG06 0x18 */
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_PT_BYPASS_MASK                                                          0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_NCO_FUNC_BYPASS_MASK                                                    0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_CH_TRK_BYPASS_MASK                                                      0x00000020
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_INDPNDT_NE_SW_BYPASS_MASK                                               0x00000040
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_DPNDT_NE_SW_BYPASS_MASK                                                 0x00000080
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_CH_TRK_BYPASS_LEGACY_PREAMBLE_MASK                                      0x00000100
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_RX_ENABLE_OVERRIDE_MASK                                                 0x00000200
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_DPNDT_NE_SYMMETRIC_MODE_MASK                                            0x00000400
#define B0_PHY_RX_FD_PHY_RXFD_REG06_FIELD_SPUR_CANC_SW_BYPASS_MASK                                                0x00000800
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG07 0x1C */
#define B0_PHY_RX_FD_PHY_RXFD_REG07_FIELD_FFT_OLA_SELECT_MASK                                                     0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REG07_FIELD_FFT_RND_MASK                                                            0x00000300
#define B0_PHY_RX_FD_PHY_RXFD_REG07_FIELD_FCB_SHORT_CP_TIM_SHIFT_MASK                                             0x01F00000
#define B0_PHY_RX_FD_PHY_RXFD_REG07_FIELD_NCB_SHORT_CP_TIM_SHIFT_MASK                                             0xF8000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG08 0x20 */
#define B0_PHY_RX_FD_PHY_RXFD_REG08_FIELD_FFT_START_WADDR_MASK                                                    0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG08_FIELD_FFT_START_WADDR_2X_MASK                                                 0x0007FE00
#define B0_PHY_RX_FD_PHY_RXFD_REG08_FIELD_FFT_START_WADDR_4X_MASK                                                 0x3FF80000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG09 0x24 */
#define B0_PHY_RX_FD_PHY_RXFD_REG09_FIELD_SHORT_CP_TIM_SHIFT_MASK                                                 0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG09_FIELD_CP_1_6_TIM_SHIFT_MASK                                                   0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG09_FIELD_CP_3_2_TIM_SHIFT_MASK                                                   0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0A 0x28 */
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_PT_SMOOTHING_VHT_INIT_SYM_MASK                                          0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_PT_SMOOTHING_PHASE_INIT_MODE_MASK                                       0x00000030
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_PT_SMOOTHING_TIMING_INIT_MODE_MASK                                      0x00000300
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_FT_ENABLE_MASK                                                          0x00001000
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_NUM_OF_SYM_MASK_MASK                                                    0x7FF00000
#define B0_PHY_RX_FD_PHY_RXFD_REG0A_FIELD_RF_DATA_TYPE_MASK                                                       0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0B 0x2C */
#define B0_PHY_RX_FD_PHY_RXFD_REG0B_FIELD_CT_2_TB_EN_MASK                                                         0x00000020
#define B0_PHY_RX_FD_PHY_RXFD_REG0B_FIELD_CT_FORCE_LLR_VALID_CH_TRK_MASK                                          0x00000040
#define B0_PHY_RX_FD_PHY_RXFD_REG0B_FIELD_CT_FORCE_LLR_VALID_NOISE_TRK_MASK                                       0x00000080
#define B0_PHY_RX_FD_PHY_RXFD_REG0B_FIELD_CT_TX_EQUAL_NORM_MODE_MASK                                              0x00000100
#define B0_PHY_RX_FD_PHY_RXFD_REG0B_FIELD_TONE_ROTATION_EN_MASK                                                   0x00000200
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0C 0x30 */
#define B0_PHY_RX_FD_PHY_RXFD_REG0C_FIELD_CE_TABLE_1_START_MASK                                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG0C_FIELD_CE_TABLE_2_START_MASK                                                   0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG0C_FIELD_CE_TABLE_3_START_MASK                                                   0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0D 0x34 */
#define B0_PHY_RX_FD_PHY_RXFD_REG0D_FIELD_CE_H0_SET_MASK                                                          0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG0D_FIELD_CE_H1_SET_MASK                                                          0x03FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0E 0x38 */
#define B0_PHY_RX_FD_PHY_RXFD_REG0E_FIELD_CE_H2_SET_MASK                                                          0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG0E_FIELD_CE_H3_SET_MASK                                                          0x03FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG0F 0x3C */
#define B0_PHY_RX_FD_PHY_RXFD_REG0F_FIELD_CE_H4_SET_MASK                                                          0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG0F_FIELD_CE_H5_SET_MASK                                                          0x03FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG11 0x44 */
#define B0_PHY_RX_FD_PHY_RXFD_REG11_FIELD_LONG1_SCRM_INIT_VAL_MASK                                                0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REG11_FIELD_FCB_PRANK3_MASK_AN0_MASK                                                0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG11_FIELD_FCB_PRANK3_MASK_AN1_MASK                                                0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG11_FIELD_FCB_PRANK3_MASK_AN2_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG12 0x48 */
#define B0_PHY_RX_FD_PHY_RXFD_REG12_FIELD_TWO_BAND_PRANK1_MASK_MASK                                               0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG12_FIELD_LONG3_FCB_PRANK1_MASK_MASK                                              0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG12_FIELD_LONG3_NCB_PRANK1_MASK_MASK                                              0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG12_FIELD_NCB_PRANK3_MASK_AN0_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG13 0x4C */
#define B0_PHY_RX_FD_PHY_RXFD_REG13_FIELD_FCB_PRANK1_MASK_MASK                                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG13_FIELD_NCB_PRANK1_MASK_MASK                                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG13_FIELD_NCB_PRANK3_MASK_AN1_MASK                                                0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG13_FIELD_NCB_PRANK3_MASK_AN2_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG14 0x50 */
#define B0_PHY_RX_FD_PHY_RXFD_REG14_FIELD_FCB_PRANK2_MASK_AN0_MASK                                                0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG14_FIELD_FCB_PRANK2_MASK_AN1_MASK                                                0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG14_FIELD_NCB_PRANK2_MASK_AN0_MASK                                                0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG14_FIELD_NCB_PRANK2_MASK_AN1_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG15 0x54 */
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_MODE_JUMP_ADD_MASK                                                0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_LTF2_JUMP_ADD_MASK                                                0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_CP_1_JUMP_ADD_MASK                                                0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_CP_0_JUMP_ADD_MASK                                                0x0000F000
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_LTF3_JUMP_ADD_MASK                                                0x000F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG15_FIELD_TD_HT_ELTF_SKIP_ADD_MASK                                                0x00F00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG16 0x58 */
#define B0_PHY_RX_FD_PHY_RXFD_REG16_FIELD_FD_HT_MODE_JUMP_ADD_MASK                                                0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REG16_FIELD_FD_HT_LTF2_JUMP_ADD_MASK                                                0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REG16_FIELD_FD_HT_BF_LTF_JUMP_ADD_MASK                                              0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REG16_FIELD_FD_HT_LTF3_JUMP_ADD_MASK                                                0x0000F000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG1B 0x6C */
#define B0_PHY_RX_FD_PHY_RXFD_REG1B_FIELD_GSM_ABORT_THR_MASK                                                      0x0000FFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG1C 0x70 */
#define B0_PHY_RX_FD_PHY_RXFD_REG1C_FIELD_LEN_TP0_MASK                                                            0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG1C_FIELD_LEN_TP1_MASK                                                            0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REG1C_FIELD_LEN_TP2_MASK                                                            0x00000100
#define B0_PHY_RX_FD_PHY_RXFD_REG1C_FIELD_LEN_TP3_MASK                                                            0x00001000
#define B0_PHY_RX_FD_PHY_RXFD_REG1C_FIELD_GSM_2_TB_RATE_MASK                                                      0xF0000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG1D 0x74 */
#define B0_PHY_RX_FD_PHY_RXFD_REG1D_FIELD_SPARE_REG_MASK                                                          0x0000FFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG1E 0x78 */
#define B0_PHY_RX_FD_PHY_RXFD_REG1E_FIELD_TB_R_NON_DIAGONAL_MASK                                                  0x00000008
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG34 0xD0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_SW_HT_DETECTOR_EN_MASK                                                  0x00010000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_SW_HT_DETECTOR_11N_MASK                                                 0x00020000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_CDD_POLARITY_MASK                                                       0x000C0000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_CE_UPDATE_CH_MASK                                                       0x00200000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_OLA_MODE_MASK                                                           0x00400000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_OLA_FFT_DONE_MASK                                                       0x00800000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_OLA_SW_FFT_P_MASK                                                       0x01000000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_GREENFIELD_DTCTR_EN_MASK                                                0x02000000
#define B0_PHY_RX_FD_PHY_RXFD_REG34_FIELD_ENABLE_ERROR_EVENT_MASK                                                 0x08000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG35 0xD4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG35_FIELD_CE_TABLE_4_START_MASK                                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG35_FIELD_CE_TABLE_5_START_MASK                                                   0x000FFC00
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG36 0xD8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_SW_BIST_START_MASK                                                      0x00100000
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_CLEAR_RAM_MODE_MASK                                                     0x00200000
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_TEST_BUS_LOW_MUX_CTRL_MASK                                              0x00C00000
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_TEST_BUS_MID_MUX_CTRL_MASK                                              0x03000000
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_TEST_BUS_HIGH_MUX_CTRL_MASK                                             0x0C000000
#define B0_PHY_RX_FD_PHY_RXFD_REG36_FIELD_TEST_BUS_FOUR_MUX_CTRL_MASK                                             0x30000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG38 0xE0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG38_FIELD_FD_GF_LSIG_MODE_JUMP_ADD_MASK                                           0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REG38_FIELD_FD_GF_HTSIG2_MODE_JUMP_ADD_MASK                                         0x0000F000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG3A 0xE8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG3A_FIELD_HYP_RXFD_RCR_RAM_RM_MASK                                                0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG3A_FIELD_HYP_RXFD_NE_RAM_RM_MASK                                                 0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG3A_FIELD_HYP_RXFD_PHASE_RAM_RM_MASK                                              0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG3A_FIELD_HYP_RXFD_PHASE_SMOOTHING_RAM_RM_MASK                                    0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG3A_FIELD_HYP_RXFD_TIMING_SMOOTHING_RAM_RM_MASK                                   0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG3B 0xEC */
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_DATA_0_CH_RAM_RM_MASK                                          0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_DATA_1_CH_RAM_RM_MASK                                          0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_PILOT_0_CH_RAM_RM_MASK                                         0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_PILOT_1_CH_RAM_RM_MASK                                         0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_FFT_0_IMAGE_RAM_RM_MASK                                        0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_FFT_0_IMAGE_PILOT_RAM_RM_MASK                                  0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_PILOT_2_CH_RAM_RM_MASK                                         0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG3B_FIELD_HYP_RXFD_PILOT_3_CH_RAM_RM_MASK                                         0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG3C 0xF0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG3C_FIELD_HYP_RXFD_GSM_RAM_STYLE_RM_MASK                                          0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG3C_FIELD_HYP_RXFD_E_SNR_THR_RAM_RM_MASK                                          0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG3C_FIELD_HYP_RXFD_MU_RAM_RM_MASK                                                 0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG3C_FIELD_HYP_RXFD_NOISE_TRK_MU_RAM_RM_MASK                                       0x00007000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG3E 0xF8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG3E_FIELD_HYP_RXFD_DATA_2_CH_RAM_RM_MASK                                          0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG3E_FIELD_HYP_RXFD_DATA_3_CH_RAM_RM_MASK                                          0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG3E_FIELD_HYP_RXFD_IMPL_CHANN_PRE_PHASE_RAM_RM_MASK                               0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG3E_FIELD_HYP_RXFD_IMPL_CHANN_PRE_TX_RX_RAM_RM_MASK                               0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG4E 0x138 */
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_EFFECTIVE_SNR_GCLK_BYPASS_MASK                                          0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_BF_RX_PARSER_GCLK_BYPASS_MASK                                           0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_GUESS_UNIT_GCLK_BYPASS_MASK                                             0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_FFT_0_GCLK_BYPASS_MASK                                                  0x00000008
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_FFT_1_GCLK_BYPASS_MASK                                                  0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_FFT_2_GCLK_BYPASS_MASK                                                  0x00000020
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_FFT_3_GCLK_BYPASS_MASK                                                  0x00000040
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_SLICER_GCLK_BYPASS_MASK                                                 0x00000780
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_QR_INTERNAL_GCLK_BYPASS_MASK                                            0x00000800
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_SPHERE_GCLK_BYPASS_MASK                                                 0x000FF000
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_CT_GCLK_DIS_MASK                                                        0x00100000
#define B0_PHY_RX_FD_PHY_RXFD_REG4E_FIELD_IMPL_CHANN_PRE_GCLK_BYPASS_MASK                                         0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG50 0x140 */
#define B0_PHY_RX_FD_PHY_RXFD_REG50_FIELD_COMPRESSED_FOURIER_PHASE_2_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG50_FIELD_COMPRESSED_FOURIER_PHASE_3_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG51 0x144 */
#define B0_PHY_RX_FD_PHY_RXFD_REG51_FIELD_COMPRESSED_FOURIER_PHASE_4_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG51_FIELD_COMPRESSED_FOURIER_PHASE_5_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG57 0x15C */
#define B0_PHY_RX_FD_PHY_RXFD_REG57_FIELD_CHANNEL_ESTIMATION_USE_3LTFS_MASK                                       0x00000004
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG68 0x1A0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG68_FIELD_NUM_OF_PGC1_VALUES_MASK                                                 0x000000FC
#define B0_PHY_RX_FD_PHY_RXFD_REG68_FIELD_NUM_OF_PGC2_VALUES_MASK                                                 0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG68_FIELD_NUM_OF_PGC3_VALUES_MASK                                                 0x000FC000
#define B0_PHY_RX_FD_PHY_RXFD_REG68_FIELD_NUM_OF_LNA_VALUES_MASK                                                  0x01F00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG74 0x1D0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG74_FIELD_HYP_RXFD_GCLK_DISABLE_MASK                                              0x0007FFFE
#define B0_PHY_RX_FD_PHY_RXFD_REG74_FIELD_MEM_GLOBAL_RM_MASK                                                      0x00C00000
#define B0_PHY_RX_FD_PHY_RXFD_REG74_FIELD_MEM_GLOBAL_BIST_SCR_BYPASS_MASK                                         0x01000000
#define B0_PHY_RX_FD_PHY_RXFD_REG74_FIELD_MEM_GLOBAL_RAM_TEST_MODE_MASK                                           0x02000000
#define B0_PHY_RX_FD_PHY_RXFD_REG74_FIELD_MEM_GLOBAL_RAM_TEST_RNM_MODE_MASK                                       0x04000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG75 0x1D4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_TRK_ALPHA_FIRST_MASK                                              0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_TRK_ALPHA_SECOND_MASK                                             0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_TRK_BYPASS_DATA_MASK                                              0x00040000
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_TRK_BYPASS_SIG_MASK                                               0x00080000
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_TRK_NOISE_GAIN_LIMIT_MASK                                         0x00F00000
#define B0_PHY_RX_FD_PHY_RXFD_REG75_FIELD_NOISE_SPLIT_ENABLE_MASK                                                 0x01000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG76 0x1D8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC0_PM_RM_VALUE_MASK                                                 0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC1_PM_RM_VALUE_MASK                                                 0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC2_PM_RM_VALUE_MASK                                                 0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC0_UNIT0_H_S_MEM_RM_VALUE_MASK                                      0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC0_UNIT0_Q_V_MEM_RM_VALUE_MASK                                      0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC1_UNIT0_H_S_MEM_RM_VALUE_MASK                                      0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC1_UNIT0_Q_V_MEM_RM_VALUE_MASK                                      0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG76_FIELD_QR_GC2_UNIT0_H_S_MEM_RM_VALUE_MASK                                      0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG77 0x1DC */
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC2_UNIT0_Q_V_MEM_RM_VALUE_MASK                                      0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC0_UNIT1_H_S_MEM_RM_VALUE_MASK                                      0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC0_UNIT1_Q_V_MEM_RM_VALUE_MASK                                      0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC1_UNIT1_H_S_MEM_RM_VALUE_MASK                                      0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC1_UNIT1_Q_V_MEM_RM_VALUE_MASK                                      0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC2_UNIT1_H_S_MEM_RM_VALUE_MASK                                      0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC2_UNIT1_Q_V_MEM_RM_VALUE_MASK                                      0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG77_FIELD_QR_GC0_UNIT2_H_S_MEM_RM_VALUE_MASK                                      0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG78 0x1E0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_GC0_UNIT2_Q_V_MEM_RM_VALUE_MASK                                      0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_GC1_UNIT2_H_S_MEM_RM_VALUE_MASK                                      0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_GC1_UNIT2_Q_V_MEM_RM_VALUE_MASK                                      0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_GC2_UNIT2_H_S_MEM_RM_VALUE_MASK                                      0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_GC2_UNIT2_Q_V_MEM_RM_VALUE_MASK                                      0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_LP_GC_PHASE_MEM_RM_VALUE_MASK                                        0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG78_FIELD_QR_LP_H_MEM_RM_VALUE_MASK                                               0x07000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG79 0x1E4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG79_FIELD_QR_SPHERE_IF_QAPPLY_MEM_0_RM_VALUE_MASK                                 0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG79_FIELD_QR_SPHERE_IF_QAPPLY_MEM_1_RM_VALUE_MASK                                 0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG79_FIELD_QR_SPHERE_IF_QAPPLY_MEM_2_RM_VALUE_MASK                                 0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG79_FIELD_QR_SPHERE_IF_QAPPLY_MEM_3_RM_VALUE_MASK                                 0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_REG79_FIELD_QR_SPHERE_IF_QR_MEM_0_RM_VALUE_MASK                                     0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7A 0x1E8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SPHERE_IF_QR_MEM_1_RM_VALUE_MASK                                     0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SPHERE_IF_QR_MEM_2_RM_VALUE_MASK                                     0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SPHERE_IF_QR_MEM_3_RM_VALUE_MASK                                     0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SORT_MEM_RM_VALUE_MASK                                               0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SORT_MEM_SPHERE_RM_VALUE_MASK                                        0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_NCO_MEM_RM_VALUE_MASK                                                0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REG7A_FIELD_QR_SPHERE_OPERATIONS_CALC_MEM_RM_VALUE_MASK                             0x07000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7B 0x1EC */
#define B0_PHY_RX_FD_PHY_RXFD_REG7B_FIELD_QR_START_ADDR_MASK                                                      0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG7B_FIELD_QR_END_ADDR_MASK                                                        0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG7B_FIELD_QR_START_ADDR_1SS_MASK                                                  0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7C 0x1F0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG7C_FIELD_QR_END_ADDR_1SS_MASK                                                    0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG7C_FIELD_SVD_CSD_START_ADDR_MASK                                                 0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG7C_FIELD_SVD_CSD_END_ADDR_MASK                                                   0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7D 0x1F4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG7D_FIELD_SVD_TRANS_CSD_START_ADDR_MASK                                           0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG7D_FIELD_SVD_TRANS_CSD_END_ADDR_MASK                                             0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG7D_FIELD_SVD_QR_START_ADDR_MASK                                                  0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7E 0x1F8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG7E_FIELD_SVD_QR_END_ADDR_MASK                                                    0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG7E_FIELD_SVD_START_ADDR_IMPLICIT_MASK                                            0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG7E_FIELD_SVD_END_ADDR_IMPLICIT_MASK                                              0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG7F 0x1FC */
#define B0_PHY_RX_FD_PHY_RXFD_REG7F_FIELD_SVD_MULT_BY_Q_END_ADDR_MASK                                             0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG7F_FIELD_SVD_ADDR_FOR_INDICATION_MASK                                            0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG7F_FIELD_COMPRESS_4ANT_START_ADDR_MASK                                           0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG80 0x200 */
#define B0_PHY_RX_FD_PHY_RXFD_REG80_FIELD_COMPRESS_4ANT_END_ADDR_MASK                                             0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG80_FIELD_COMPRESS_3ANT_START_ADDR_MASK                                           0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG80_FIELD_COMPRESS_3ANT_END_ADDR_MASK                                             0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG81 0x204 */
#define B0_PHY_RX_FD_PHY_RXFD_REG81_FIELD_COMPRESS_2ANT_START_ADDR_MASK                                           0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG81_FIELD_COMPRESS_2ANT_END_ADDR_MASK                                             0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG81_FIELD_DECOMPRESS_4ANT_START_ADDR_MASK                                         0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG82 0x208 */
#define B0_PHY_RX_FD_PHY_RXFD_REG82_FIELD_DECOMPRESS_4ANT_END_ADDR_MASK                                           0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG82_FIELD_DECOMPRESS_3ANT_START_ADDR_MASK                                         0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG82_FIELD_DECOMPRESS_3ANT_END_ADDR_MASK                                           0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG83 0x20C */
#define B0_PHY_RX_FD_PHY_RXFD_REG83_FIELD_DECOMPRESS_2ANT_START_ADDR_MASK                                         0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG83_FIELD_DECOMPRESS_2ANT_END_ADDR_MASK                                           0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REG83_FIELD_STBC_START_ADDR_MASK                                                    0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG84 0x210 */
#define B0_PHY_RX_FD_PHY_RXFD_REG84_FIELD_STBC_END_ADDR_MASK                                                      0x000003FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG86 0x218 */
#define B0_PHY_RX_FD_PHY_RXFD_REG86_FIELD_COMPRESSED_FOURIER_PHASE_0_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG86_FIELD_COMPRESSED_FOURIER_PHASE_1_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG87 0x21C */
#define B0_PHY_RX_FD_PHY_RXFD_REG87_FIELD_COMPRESSED_FOURIER_PHASE_6_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG87_FIELD_COMPRESSED_FOURIER_PHASE_7_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG88 0x220 */
#define B0_PHY_RX_FD_PHY_RXFD_REG88_FIELD_COMPRESSED_FOURIER_PHASE_8_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG88_FIELD_COMPRESSED_FOURIER_PHASE_9_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG89 0x224 */
#define B0_PHY_RX_FD_PHY_RXFD_REG89_FIELD_COMPRESSED_FOURIER_PHASE_10_MASK                                        0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG89_FIELD_COMPRESSED_FOURIER_PHASE_11_MASK                                        0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG8A 0x228 */
#define B0_PHY_RX_FD_PHY_RXFD_REG8A_FIELD_CE_TABLE_6_START_MASK                                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG8A_FIELD_CE_TABLE_END_ADDR_MASK                                                  0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG8B 0x22C */
#define B0_PHY_RX_FD_PHY_RXFD_REG8B_FIELD_NCB_PRANK4_MASK_AN0_MASK                                                0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG8B_FIELD_NCB_PRANK4_MASK_AN1_MASK                                                0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG8B_FIELD_NCB_PRANK4_MASK_AN2_MASK                                                0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG8B_FIELD_NCB_PRANK4_MASK_AN3_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG8C 0x230 */
#define B0_PHY_RX_FD_PHY_RXFD_REG8C_FIELD_FCB_PRANK4_MASK_AN0_MASK                                                0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG8C_FIELD_FCB_PRANK4_MASK_AN1_MASK                                                0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG8C_FIELD_FCB_PRANK4_MASK_AN2_MASK                                                0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG8C_FIELD_FCB_PRANK4_MASK_AN3_MASK                                                0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG8D 0x234 */
#define B0_PHY_RX_FD_PHY_RXFD_REG8D_FIELD_CB80_PRANK1_MASK_MASK                                                   0x000000FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_IF08E 0x238 */
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_1SS_QR_MASK                                             0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_2SS_QR_MASK                                             0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_3SS_QR_MASK                                             0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_4SS_QR_MASK                                             0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_1SS_QAPPLY_MASK                                         0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_2SS_QAPPLY_MASK                                         0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_3SS_QAPPLY_MASK                                         0x07000000
#define B0_PHY_RX_FD_PHY_RXFD_IF08E_FIELD_QR_MAX_PRESCALE_4SS_QAPPLY_MASK                                         0x70000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG90 0x240 */
#define B0_PHY_RX_FD_PHY_RXFD_REG90_FIELD_FD_VHT_SIGB_JUMP_ADD_MASK                                               0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REG90_FIELD_TD_VHT_SIGB_JUMP_ADD_MASK                                               0x000000F0
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG91 0x244 */
#define B0_PHY_RX_FD_PHY_RXFD_REG91_FIELD_CB80_SHORT_CP_TIM_SHIFT_MASK                                            0x0000001F
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG92 0x248 */
#define B0_PHY_RX_FD_PHY_RXFD_REG92_FIELD_CH_ESTI_SMOOTH_RM_VALUE_MASK                                            0x00000007
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG93 0x24C */
#define B0_PHY_RX_FD_PHY_RXFD_REG93_FIELD_LONG3_CB80_PRANK1_MASK_MASK                                             0x000000FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG94 0x250 */
#define B0_PHY_RX_FD_PHY_RXFD_REG94_FIELD_HYP_RXFD_GPR1_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG95 0x254 */
#define B0_PHY_RX_FD_PHY_RXFD_REG95_FIELD_HYP_RXFD_GPR2_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG96 0x258 */
#define B0_PHY_RX_FD_PHY_RXFD_REG96_FIELD_HYP_RXFD_GPR3_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG97 0x25C */
#define B0_PHY_RX_FD_PHY_RXFD_REG97_FIELD_HYP_RXFD_GPR4_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG98 0x260 */
#define B0_PHY_RX_FD_PHY_RXFD_REG98_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_0_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG98_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_0_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG98_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_1_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG98_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_1_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG99 0x264 */
#define B0_PHY_RX_FD_PHY_RXFD_REG99_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_2_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG99_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_2_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG99_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_3_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG99_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_3_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9A 0x268 */
#define B0_PHY_RX_FD_PHY_RXFD_REG9A_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_4_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9A_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_4_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9A_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_5_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9A_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_5_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9B 0x26C */
#define B0_PHY_RX_FD_PHY_RXFD_REG9B_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_6_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9B_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_6_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9B_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_7_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9B_FIELD_TX_CT_SM_MATRIX_CUSTOM1_2SS_7_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9C 0x270 */
#define B0_PHY_RX_FD_PHY_RXFD_REG9C_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_0_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9C_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_0_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9C_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_1_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9C_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_1_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9D 0x274 */
#define B0_PHY_RX_FD_PHY_RXFD_REG9D_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_2_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9D_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_2_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9D_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_3_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9D_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_3_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9E 0x278 */
#define B0_PHY_RX_FD_PHY_RXFD_REG9E_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_4_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9E_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_4_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9E_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_5_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9E_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_5_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG9F 0x27C */
#define B0_PHY_RX_FD_PHY_RXFD_REG9F_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_6_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG9F_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_6_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG9F_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_7_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG9F_FIELD_TX_CT_SM_MATRIX_CUSTOM2_2SS_7_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA0 0x280 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA0_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_0_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA0_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_0_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA0_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_1_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA0_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_1_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA1 0x284 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA1_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_2_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA1_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_2_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA1_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_3_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA1_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_3_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA2 0x288 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA2_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_4_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA2_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_4_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA2_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_5_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA2_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_5_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA3 0x28C */
#define B0_PHY_RX_FD_PHY_RXFD_REGA3_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_6_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA3_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_6_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA3_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_7_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA3_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_7_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA4 0x290 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA4_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_8_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA4_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_8_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA4_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_9_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA4_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_9_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA5 0x294 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA5_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_10_I_MASK                                   0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA5_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_10_Q_MASK                                   0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA5_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_11_I_MASK                                   0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA5_FIELD_TX_CT_SM_MATRIX_CUSTOM1_3SS_11_Q_MASK                                   0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA6 0x298 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA6_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_0_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA6_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_0_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA6_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_1_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA6_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_1_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA7 0x29C */
#define B0_PHY_RX_FD_PHY_RXFD_REGA7_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_2_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA7_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_2_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA7_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_3_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA7_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_3_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA8 0x2A0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA8_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_4_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA8_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_4_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA8_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_5_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA8_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_5_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGA9 0x2A4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGA9_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_6_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGA9_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_6_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGA9_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_7_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGA9_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_7_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGAA 0x2A8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGAA_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_8_I_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGAA_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_8_Q_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGAA_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_9_I_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGAA_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_9_Q_MASK                                    0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGAB 0x2AC */
#define B0_PHY_RX_FD_PHY_RXFD_REGAB_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_10_I_MASK                                   0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGAB_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_10_Q_MASK                                   0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REGAB_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_11_I_MASK                                   0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REGAB_FIELD_TX_CT_SM_MATRIX_CUSTOM2_3SS_11_Q_MASK                                   0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGBE 0x2F8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGBE_FIELD_AVG_SNR_ADD_FACTOR_MASK                                                 0x00000FFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC0 0x300 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC0_FIELD_FFT_RND_HB20160_MASK                                                    0x00000003
#define B0_PHY_RX_FD_PHY_RXFD_REGC0_FIELD_FFT_RND_HB40160_MASK                                                    0x0000000C
#define B0_PHY_RX_FD_PHY_RXFD_REGC0_FIELD_FFT_RND_HB80160_MASK                                                    0x00000030
#define B0_PHY_RX_FD_PHY_RXFD_REGC0_FIELD_BYPASS_HALT_SPHERE_MASK                                                 0x00000100
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC1 0x304 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC1_FIELD_MU_QR_DECOMPRESS_4ANT_START_ADDR_MASK                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGC1_FIELD_MU_QR_DECOMPRESS_4ANT_END_ADDR_MASK                                     0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC2 0x308 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC2_FIELD_MU_QR_DECOMPRESS_3ANT_START_ADDR_MASK                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGC2_FIELD_MU_QR_DECOMPRESS_3ANT_END_ADDR_MASK                                     0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC3 0x30C */
#define B0_PHY_RX_FD_PHY_RXFD_REGC3_FIELD_MU_QR_DECOMPRESS_2ANT_START_ADDR_MASK                                   0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGC3_FIELD_MU_QR_DECOMPRESS_2ANT_END_ADDR_MASK                                     0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC4 0x310 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC4_FIELD_MU_QR_QAPPLY_START_ADDR_MASK                                            0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGC4_FIELD_MU_QR_QAPPLY_END_ADDR_MASK                                              0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC5 0x314 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC5_FIELD_MU_PHASES_RM_VALUE_MASK                                                 0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGC5_FIELD_MU_TRAIN_EFF_RATE_RM_VALUE_MASK                                         0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REGC5_FIELD_MU_DELTA_SNR_RM_VALUE_MASK                                              0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REGC5_FIELD_HYP_RXFD_E_SNR_GAMA_RAM_RM_MASK                                         0x00007000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGC6 0x318 */
#define B0_PHY_RX_FD_PHY_RXFD_REGC6_FIELD_MU_QR_4X2_START_ADDR_MASK                                               0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGC6_FIELD_MU_QR_4X2_END_ADDR_MASK                                                 0x003FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGCB 0x32C */
#define B0_PHY_RX_FD_PHY_RXFD_REGCB_FIELD_BF_HT_CODEBOOK_INFO_MASK                                                0x00000003
#define B0_PHY_RX_FD_PHY_RXFD_REGCB_FIELD_BF_VHT_CODEBOOK_INFO_MASK                                               0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REGCB_FIELD_BF_VHT_MU_CODEBOOK_INFO_MASK                                            0x00000008
#define B0_PHY_RX_FD_PHY_RXFD_REGCB_FIELD_BF_HE_CODEBOOK_INFO_MASK                                                0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REGCB_FIELD_BF_HE_MU_CODEBOOK_INFO_MASK                                             0x00000020
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGCC 0x330 */
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_LLTF_SMOOTH_EN_MASK                                                     0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_HTLTF_SMOOTH_EN_MASK                                                    0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_FORCE_SPECIAL_FILTERS_MASK                                              0x00000100
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_USE_SNR_EST_L_MASK                                                      0x00001000
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_USE_SNR_EST_HT_MASK                                                     0x00002000
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_CP_OVERRIDE_MASK                                                        0x00030000
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_SMOOTH_IF_BF_MASK                                                       0x00100000
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_SMOOTH_HE_SU_BF_2X_EN_MASK                                              0x01000000
#define B0_PHY_RX_FD_PHY_RXFD_REGCC_FIELD_SMOOTH_HE_SU_BF_4X_EN_MASK                                              0x10000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGCD 0x334 */
#define B0_PHY_RX_FD_PHY_RXFD_REGCD_FIELD_SNR_THRESHOLD_LOW_MASK                                                  0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REGCD_FIELD_SNR_THRESHOLD_MEDIUM_MASK                                               0x00007F00
#define B0_PHY_RX_FD_PHY_RXFD_REGCD_FIELD_SNR_THRESHOLD_HIGH_MASK                                                 0x007F0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGCE 0x338 */
#define B0_PHY_RX_FD_PHY_RXFD_REGCE_FIELD_TIMING_OFFSET_CP04_MASK                                                 0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REGCE_FIELD_TIMING_OFFSET_CP08_MASK                                                 0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REGCE_FIELD_TIMING_OFFSET_CP16_MASK                                                 0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REGCE_FIELD_TIMING_OFFSET_CP32_MASK                                                 0x0000F000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGCF 0x33C */
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_SU_PH_HIS_BPSK_MASK                                                  0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_SU_PH_HIS_QPSK_MASK                                                  0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_SU_PH_HIS_16QAM_MASK                                                 0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_SU_PH_HIS_64QAM_MASK                                                 0x0000F000
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_TRIG_PH_HIS_BPSK_MASK                                                0x000F0000
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_TRIG_PH_HIS_QPSK_MASK                                                0x00F00000
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_TRIG_PH_HIS_16QAM_MASK                                               0x0F000000
#define B0_PHY_RX_FD_PHY_RXFD_REGCF_FIELD_PT_TRIG_PH_HIS_64QAM_MASK                                               0xF0000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD0 0x340 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD0_FIELD_PT_SU_TIM_HIS_BPSK_MASK                                                 0x0000001F
#define B0_PHY_RX_FD_PHY_RXFD_REGD0_FIELD_PT_SU_TIM_HIS_QPSK_MASK                                                 0x000003E0
#define B0_PHY_RX_FD_PHY_RXFD_REGD0_FIELD_PT_SU_TIM_HIS_16QAM_MASK                                                0x00007C00
#define B0_PHY_RX_FD_PHY_RXFD_REGD0_FIELD_PT_SU_TIM_HIS_64QAM_MASK                                                0x000F8000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD1 0x344 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD1_FIELD_PT_TRIG_TIM_HIS_BPSK_MASK                                               0x0000001F
#define B0_PHY_RX_FD_PHY_RXFD_REGD1_FIELD_PT_TRIG_TIM_HIS_QPSK_MASK                                               0x000003E0
#define B0_PHY_RX_FD_PHY_RXFD_REGD1_FIELD_PT_TRIG_TIM_HIS_16QAM_MASK                                              0x00007C00
#define B0_PHY_RX_FD_PHY_RXFD_REGD1_FIELD_PT_TRIG_TIM_HIS_64QAM_MASK                                              0x000F8000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD2 0x348 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD2_FIELD_PT_DUP_PHASE_TRACK_MASK                                                 0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REGD2_FIELD_PT_MISC_MASK                                                            0xFFFFFFFE
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD3 0x34C */
#define B0_PHY_RX_FD_PHY_RXFD_REGD3_FIELD_LONG3_CB160_PRANK1_MASK_MASK                                            0x0000FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REGD3_FIELD_CB160_PRANK1_MASK_MASK                                                  0xFFFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD4 0x350 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD4_FIELD_SMOOTH_INTERP_FILTER_RAM_RM_MASK                                        0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGD4_FIELD_SMOOTH_INTERP_USER_RU_MAP_RM_MASK                                       0x00000038
#define B0_PHY_RX_FD_PHY_RXFD_REGD4_FIELD_SMOOTH_INTERP_MCS_SNR_RAM_RM_MASK                                       0x000001C0
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD5 0x354 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD5_FIELD_SMOOTH_INTERP_MISC_MASK                                                 0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD6 0x358 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD6_FIELD_SMOOTH_INTERP_MISC2_MASK                                                0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD7 0x35C */
#define B0_PHY_RX_FD_PHY_RXFD_REGD7_FIELD_CH_ENERGY_E_NORM_LUT_3_0_MASK                                           0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD8 0x360 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD8_FIELD_CH_ENERGY_E_NORM_LUT_7_4_MASK                                           0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGD9 0x364 */
#define B0_PHY_RX_FD_PHY_RXFD_REGD9_FIELD_CH_ENERGY_E_NORM_LUT_11_8_MASK                                          0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDA 0x368 */
#define B0_PHY_RX_FD_PHY_RXFD_REGDA_FIELD_CH_ENERGY_E_NORM_LUT_15_12_MASK                                         0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDB 0x36C */
#define B0_PHY_RX_FD_PHY_RXFD_REGDB_FIELD_CH_ENERGY_E_NORM_LUT_SNR_SHIFT_MASK                                     0x000001FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDC 0x370 */
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_SNR_THRESHOLD_MASK                                            0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_VIT_ERROR_THRESHOLD_MASK                                      0x00007F00
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_DECISION_THRESHOLD_MASK                                       0x007F0000
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_MAX_DET_BW_MASK                                               0x03000000
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_EXT_ONLY_MASK                                                 0x10000000
#define B0_PHY_RX_FD_PHY_RXFD_REGDC_FIELD_HE_FD_DET_EN_MASK                                                       0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDD 0x374 */
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_26B_MASK                                                  0x00000003
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_52B_MASK                                                  0x0000000C
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_106B_MASK                                                 0x00000030
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_20_MASK                                                   0x000000C0
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_40_MASK                                                   0x00000300
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_80_MASK                                                   0x00000C00
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_160_MASK                                                  0x00003000
#define B0_PHY_RX_FD_PHY_RXFD_REGDD_FIELD_FFT_OUT_SHIFT_HESTF_MASK                                                0x0000C000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDE 0x378 */
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_26B_MASK                                                   0x0000000F
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_52B_MASK                                                   0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_106B_MASK                                                  0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_20_MASK                                                    0x0000F000
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_40_MASK                                                    0x000F0000
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_80_MASK                                                    0x00F00000
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_160_MASK                                                   0x0F000000
#define B0_PHY_RX_FD_PHY_RXFD_REGDE_FIELD_FFT_OUT_MULT_HESTF_MASK                                                 0xF0000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGDF 0x37C */
#define B0_PHY_RX_FD_PHY_RXFD_REGDF_FIELD_PT_PHASE_LIMIT_MASK                                                     0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGDF_FIELD_PT_PHASE_LIMIT_TRIG_BASED_MASK                                          0x000FFC00
#define B0_PHY_RX_FD_PHY_RXFD_REGDF_FIELD_PT_TIMING_LIMIT_MASK                                                    0x3FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE0 0x380 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE0_FIELD_PT_TIMING_LIMIT_TRIG_BASED_MASK                                         0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGE0_FIELD_PT_FFT_K_USER_RAM_RM_MASK                                               0x00007C00
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE1 0x384 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE1_FIELD_NOISE_INIT_REG_FROM_TD_MASK                                             0x007FFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE2 0x388 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE2_FIELD_RSSI_PER_RU_RAM_RM_MASK                                                 0x00000007
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE3 0x38C */
#define B0_PHY_RX_FD_PHY_RXFD_REGE3_FIELD_HE_FD_DET_PRE_LOAD_RAM_RM_MASK                                          0x00000007
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE4 0x390 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE4_FIELD_HE_FD_DET_LSIG_METRIC_MASK                                              0x00007FFF
#define B0_PHY_RX_FD_PHY_RXFD_REGE4_FIELD_HE_FD_DET_RLSIG_METRIC_MASK                                             0x7FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE5 0x394 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE5_FIELD_HE_FD_DET_METRIC_MASK                                                   0x00007FFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE6 0x398 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE6_FIELD_CT_DATA_E4_IN_FOR_IIR0_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE7 0x39C */
#define B0_PHY_RX_FD_PHY_RXFD_REGE7_FIELD_CT_DATA_E4_IN_FOR_IIR1_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE8 0x3A0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE8_FIELD_CT_DATA_E4_IN_FOR_IIR2_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGE9 0x3A4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGE9_FIELD_CT_DATA_E4_IN_FOR_IIR3_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGEA 0x3A8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGEA_FIELD_CT_DATA_E4_IN_FOR_IIR4_MASK                                             0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGEB 0x3AC */
#define B0_PHY_RX_FD_PHY_RXFD_REGEB_FIELD_SPHERE_MAX_LLR_MASK                                                     0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REGEB_FIELD_SPHERE_NOISE_POWER_PROG_MODEL_MASK                                      0x00007000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGEC 0x3B0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGEC_FIELD_SPHERE_TREE_PART_THR_MASK                                               0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGEC_FIELD_SPHERE_LLR_THRESHOLD_MASK                                               0x000001F0
#define B0_PHY_RX_FD_PHY_RXFD_REGEC_FIELD_SPHERE_NOISE_POWER_TREE_PART_THR_MASK                                   0x00000200
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGED 0x3B4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGED_FIELD_LINEAR_FIFO_RAM_RM_MASK                                                 0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGED_FIELD_SPHERE_FIFO_RAM_RM_MASK                                                 0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REGED_FIELD_TCR_RAM_RM_MASK                                                         0x00000700
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGEE 0x3B8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGEE_FIELD_SPHERE_BUS0_LEN_MASK                                                    0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REGEE_FIELD_SPHERE_BUS1_LEN_MASK                                                    0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REGEE_FIELD_SPHERE_BUS2_LEN_MASK                                                    0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REGEE_FIELD_SPHERE_BUS3_LEN_MASK                                                    0x00000008
#define B0_PHY_RX_FD_PHY_RXFD_REGEE_FIELD_SPHERE_TB_MODE_MASK                                                     0x00000010
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_APB_CONTROL 0x3C0 */
#define B0_PHY_RX_FD_PHY_RXFD_APB_CONTROL_FIELD_RXFD_APB_DEBUG_SELECT_MASK                                        0x00000003
#define B0_PHY_RX_FD_PHY_RXFD_APB_CONTROL_FIELD_CH_PAGE_MASK                                                      0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_APB_CONTROL_FIELD_SU_PAGE_MASK                                                      0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_APB_CONTROL_FIELD_MU_PAGE_MASK                                                      0x00001000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF1 0x3C4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGF1_FIELD_POWER_LOAD_GAIN_SW_MASK                                                 0x00000001
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF2 0x3C8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGF2_FIELD_CH_RAM_BANK_SEL_MASK                                                    0x00000003
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF3 0x3CC */
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT0_H_S_MEM_RM_VALUE_MASK                                      0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT0_Q_V_MEM_RM_VALUE_MASK                                      0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT1_H_S_MEM_RM_VALUE_MASK                                      0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT1_Q_V_MEM_RM_VALUE_MASK                                      0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT2_H_S_MEM_RM_VALUE_MASK                                      0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REGF3_FIELD_QR_GC3_UNIT2_Q_V_MEM_RM_VALUE_MASK                                      0x00700000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF4 0x3D0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT0_H_S_MEM_RM_VALUE_MASK                                      0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT0_Q_V_MEM_RM_VALUE_MASK                                      0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT1_H_S_MEM_RM_VALUE_MASK                                      0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT1_Q_V_MEM_RM_VALUE_MASK                                      0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT2_H_S_MEM_RM_VALUE_MASK                                      0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_QR_GC4_UNIT2_Q_V_MEM_RM_VALUE_MASK                                      0x00700000
#define B0_PHY_RX_FD_PHY_RXFD_REGF4_FIELD_MU_UL_HRAM_MEM_RM_VALUE_MASK                                            0x07000000
//========================================
/* REG_PHY_RX_FD_STREAMER_CONTROL 0x3D4 */
#define B0_PHY_RX_FD_STREAMER_CONTROL_FIELD_STREAMER_ENABLE_MASK                                                  0x00000001
#define B0_PHY_RX_FD_STREAMER_CONTROL_FIELD_STREAMER_MODE_MASK                                                    0x00000002
#define B0_PHY_RX_FD_STREAMER_CONTROL_FIELD_STREAMER_ALL_ANTENNAS_SAME_DATA_MASK                                  0x00000004
#define B0_PHY_RX_FD_STREAMER_CONTROL_FIELD_STREAMER_RATE_MASK                                                    0x00000070
//========================================
/* REG_PHY_RX_FD_STREAMER_ADDRESS 0x3D8 */
#define B0_PHY_RX_FD_STREAMER_ADDRESS_FIELD_STREAMER_START_ADDRESS_MASK                                           0x00003FFF
#define B0_PHY_RX_FD_STREAMER_ADDRESS_FIELD_STREAMER_END_ADDRESS_MASK                                             0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF7 0x3DC */
#define B0_PHY_RX_FD_PHY_RXFD_REGF7_FIELD_SU_PHASE_RAM_SEL_MASK                                                   0x00000001
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF8 0x3E0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGF8_FIELD_LINEAR_EVM_MU_MEM_RM_MASK                                               0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGF8_FIELD_LINEAR_RCR_MEM_RM_MASK                                                  0x00000070
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGF9 0x3E4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGF9_FIELD_HE_STF_NOISE_GAIN_RAM_RM_MASK                                           0x00000007
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFA 0x3E8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGFA_FIELD_HESTF_FFT_GAIN_MAX_VALUE_MINUS_MOD_PAR_MASK                             0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REGFA_FIELD_HESTF_FFT_GAIN_FFT_IN_SHIFT_HESTF_MASK                                  0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REGFA_FIELD_HESTF_FFT_LIMIT_GAIN_MASK                                               0x00010000
#define B0_PHY_RX_FD_PHY_RXFD_REGFA_FIELD_HESTF_FFT_GAIN_EN_MASK                                                  0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFB 0x3EC */
#define B0_PHY_RX_FD_PHY_RXFD_REGFB_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS0_MASK                            0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REGFB_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS1_MASK                            0x00007F00
#define B0_PHY_RX_FD_PHY_RXFD_REGFB_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS2_MASK                            0x007F0000
#define B0_PHY_RX_FD_PHY_RXFD_REGFB_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS3_MASK                            0x7F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFC 0x3F0 */
#define B0_PHY_RX_FD_PHY_RXFD_REGFC_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS4_MASK                            0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REGFC_FIELD_HESTF_FFT_GAIN_EXTRA_NOISE_BACKOFF_MCS5_MASK                            0x00007F00
#define B0_PHY_RX_FD_PHY_RXFD_REGFC_FIELD_HESTF_FFT_GAIN_EXTRA_BACKOFF_NDP_FDBK_MASK                              0x007F0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFD 0x3F4 */
#define B0_PHY_RX_FD_PHY_RXFD_REGFD_FIELD_HESTF_FFT_GAIN_RES_IN_SHIFT_MASK                                        0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REGFD_FIELD_HESTF_FFT_GAIN_RES_IN_MULT_MASK                                         0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REGFD_FIELD_HESTF_FFT_GAIN_RES_OUT_MULT_MASK                                        0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REGFD_FIELD_HESTF_FFT_GAIN_RES_VALID_MASK                                           0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFE 0x3F8 */
#define B0_PHY_RX_FD_PHY_RXFD_REGFE_FIELD_HESTF_MISC_RO_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REGFF 0x3FC */
#define B0_PHY_RX_FD_PHY_RXFD_REGFF_FIELD_HESTF_NOISE_GAIN_SIGMA_MIN_MASK                                         0x007FFFFF
#define B0_PHY_RX_FD_PHY_RXFD_REGFF_FIELD_HESTF_NOISE_GAIN_RES_VALID_MASK                                         0x80000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG100 0x400 */
#define B0_PHY_RX_FD_PHY_RXFD_REG100_FIELD_POWER_PER_RU_TD_GAIN_ANT0_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG100_FIELD_POWER_PER_RU_TD_GAIN_ANT1_MASK                                         0x01FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG101 0x404 */
#define B0_PHY_RX_FD_PHY_RXFD_REG101_FIELD_POWER_PER_RU_TD_GAIN_ANT2_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG101_FIELD_POWER_PER_RU_TD_GAIN_ANT3_MASK                                         0x01FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG102 0x408 */
#define B0_PHY_RX_FD_PHY_RXFD_REG102_FIELD_POWER_PER_RU_SYSTEM_GAIN_ANT0_MASK                                     0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG102_FIELD_POWER_PER_RU_SYSTEM_GAIN_ANT1_MASK                                     0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG102_FIELD_POWER_PER_RU_SYSTEM_GAIN_ANT2_MASK                                     0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG102_FIELD_POWER_PER_RU_SYSTEM_GAIN_ANT3_MASK                                     0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG103 0x40C */
#define B0_PHY_RX_FD_PHY_RXFD_REG103_FIELD_POWER_PER_RU_FFT_GAIN_MASK                                             0x000003FF
#define B0_PHY_RX_FD_PHY_RXFD_REG103_FIELD_POWER_PER_RU_NORM_OFFSET_MASK                                          0x00FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG104 0x410 */
#define B0_PHY_RX_FD_PHY_RXFD_REG104_FIELD_HESTF_FFT_GAIN_RES_VALID_CLR_MASK                                      0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG104_FIELD_HESTF_NOISE_GAIN_RES_VALID_CLR_MASK                                    0x00000002
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG105 0x414 */
#define B0_PHY_RX_FD_PHY_RXFD_REG105_FIELD_HESTF_NOISE_GAIN_ANT0_MASK                                             0x0000007F
#define B0_PHY_RX_FD_PHY_RXFD_REG105_FIELD_HESTF_NOISE_GAIN_ANT1_MASK                                             0x00007F00
#define B0_PHY_RX_FD_PHY_RXFD_REG105_FIELD_HESTF_NOISE_GAIN_ANT2_MASK                                             0x007F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG105_FIELD_HESTF_NOISE_GAIN_ANT3_MASK                                             0x7F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG106 0x418 */
#define B0_PHY_RX_FD_PHY_RXFD_REG106_FIELD_TEST_BUS_HEADER_MASK                                                   0x00FFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG107 0x41C */
#define B0_PHY_RX_FD_PHY_RXFD_REG107_FIELD_HESTF_NOISE_GAIN_BYPASS_MASK                                           0x00000001
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG108 0x420 */
#define B0_PHY_RX_FD_PHY_RXFD_REG108_FIELD_SPHERE_CYCLES_MASK                                                     0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG108_FIELD_SPHERE_CYCLES_LAST_SYM_MASK                                            0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG109 0x424 */
#define B0_PHY_RX_FD_PHY_RXFD_REG109_FIELD_SPHERE_CYCELS_VALID_PULSE_MASK                                         0x00000001
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG110 0x428 */
#define B0_PHY_RX_FD_PHY_RXFD_REG110_FIELD_SPHERE_CYCELS_GR_ENABLE_MASK                                           0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG110_FIELD_LINEAR_FIFO_WAIT_MECH_MASK                                             0x00000002
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG111 0x42C */
#define B0_PHY_RX_FD_PHY_RXFD_REG111_FIELD_HYP_RXFD_NCO_RAM_RM_MASK                                               0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG111_FIELD_HYP_RXFD_TRAINING_RAM_RM_MASK                                          0x00000038
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG112 0x430 */
#define B0_PHY_RX_FD_PHY_RXFD_REG112_FIELD_SU_DB_VALID_RD0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG113 0x434 */
#define B0_PHY_RX_FD_PHY_RXFD_REG113_FIELD_SU_DB_VALID_RD1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG114 0x438 */
#define B0_PHY_RX_FD_PHY_RXFD_REG114_FIELD_SU_DB_VALID_RD2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG115 0x43C */
#define B0_PHY_RX_FD_PHY_RXFD_REG115_FIELD_SU_DB_VALID_RD3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG116 0x440 */
#define B0_PHY_RX_FD_PHY_RXFD_REG116_FIELD_SU_DB_VALID_RD4_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG117 0x444 */
#define B0_PHY_RX_FD_PHY_RXFD_REG117_FIELD_SU_DB_VALID_RD5_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG118 0x448 */
#define B0_PHY_RX_FD_PHY_RXFD_REG118_FIELD_SU_DB_VALID_RD6_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG119 0x44C */
#define B0_PHY_RX_FD_PHY_RXFD_REG119_FIELD_SU_DB_VALID_RD7_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG120 0x450 */
#define B0_PHY_RX_FD_PHY_RXFD_REG120_FIELD_SU_DB_VALID_SW0_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG121 0x454 */
#define B0_PHY_RX_FD_PHY_RXFD_REG121_FIELD_SU_DB_VALID_SW1_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG122 0x458 */
#define B0_PHY_RX_FD_PHY_RXFD_REG122_FIELD_SU_DB_VALID_SW2_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG123 0x45C */
#define B0_PHY_RX_FD_PHY_RXFD_REG123_FIELD_SU_DB_VALID_SW3_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG124 0x460 */
#define B0_PHY_RX_FD_PHY_RXFD_REG124_FIELD_SU_DB_VALID_SW4_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG125 0x464 */
#define B0_PHY_RX_FD_PHY_RXFD_REG125_FIELD_SU_DB_VALID_SW5_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG126 0x468 */
#define B0_PHY_RX_FD_PHY_RXFD_REG126_FIELD_SU_DB_VALID_SW6_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG127 0x46C */
#define B0_PHY_RX_FD_PHY_RXFD_REG127_FIELD_SU_DB_VALID_SW7_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG128 0x470 */
#define B0_PHY_RX_FD_PHY_RXFD_REG128_FIELD_SU_DB_VALID_CLR_MASK                                                   0x00000001
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG129 0x474 */
#define B0_PHY_RX_FD_PHY_RXFD_REG129_FIELD_SU_BF_ERROR_MASK                                                       0x0000FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REG129_FIELD_SU_BF_OFDMA_ERROR_MASK                                                 0x00070000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG130 0x478 */
#define B0_PHY_RX_FD_PHY_RXFD_REG130_FIELD_SU_BF_OFDMA_SEL_MASK                                                   0x0000003F
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG131 0x47C */
#define B0_PHY_RX_FD_PHY_RXFD_REG131_FIELD_SU_BF_ERR_CLR_MASK                                                     0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG131_FIELD_SU_BF_OFDMA_ERR_CLR_MASK                                               0x00000002
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG132 0x480 */
#define B0_PHY_RX_FD_PHY_RXFD_REG132_FIELD_NOISE_GAIN_LOW_TH_MASK                                                 0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG132_FIELD_NOISE_GAIN_HIGH_TH_MASK                                                0x001FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG133 0x484 */
#define B0_PHY_RX_FD_PHY_RXFD_REG133_FIELD_NOISE_GAIN_HIGH_TH_CONS_0_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG133_FIELD_NOISE_GAIN_HIGH_TH_CONS_1_MASK                                         0x001FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG134 0x488 */
#define B0_PHY_RX_FD_PHY_RXFD_REG134_FIELD_NOISE_GAIN_HIGH_TH_CONS_2_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG134_FIELD_NOISE_GAIN_HIGH_TH_CONS_3_MASK                                         0x001FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG135 0x48C */
#define B0_PHY_RX_FD_PHY_RXFD_REG135_FIELD_NOISE_GAIN_HIGH_TH_CONS_4_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG135_FIELD_NOISE_GAIN_HIGH_TH_CONS_5_MASK                                         0x001FF000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG136 0x490 */
#define B0_PHY_RX_FD_PHY_RXFD_REG136_FIELD_NOISE_GAIN_HIGH_TH_CONS_6_MASK                                         0x000001FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG137 0x494 */
#define B0_PHY_RX_FD_PHY_RXFD_REG137_FIELD_EN_SPHERE_FAST_CONVERGANCE_MASK                                        0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG137_FIELD_NE_SMOOTHING_PIPE_MULT_CFG_MASK                                        0x00000002
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG138 0x498 */
#define B0_PHY_RX_FD_PHY_RXFD_REG138_FIELD_SPHERE_REDUCE_CYCLES_80M_U13_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG138_FIELD_SPHERE_REDUCE_CYCLES_80M_U14_MASK                                      0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG138_FIELD_SPHERE_REDUCE_CYCLES_80M_U15_MASK                                      0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG138_FIELD_SPHERE_REDUCE_CYCLES_80M_U16_MASK                                      0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG139 0x49C */
#define B0_PHY_RX_FD_PHY_RXFD_REG139_FIELD_SPHERE_REDUCE_CYCLES_80M_U17_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG139_FIELD_SPHERE_REDUCE_CYCLES_80M_U18_MASK                                      0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG139_FIELD_SPHERE_REDUCE_CYCLES_80M_U19_MASK                                      0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG139_FIELD_SPHERE_REDUCE_CYCLES_80M_U20_MASK                                      0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG140 0x4A0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG140_FIELD_SPHERE_REDUCE_CYCLES_80M_U21_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG140_FIELD_SPHERE_REDUCE_CYCLES_80M_U22_MASK                                      0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG140_FIELD_SPHERE_REDUCE_CYCLES_80M_U23_MASK                                      0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG140_FIELD_SPHERE_REDUCE_CYCLES_80M_U24_MASK                                      0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG141 0x4A4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG141_FIELD_SPHERE_REDUCE_CYCLES_80M_U25_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG141_FIELD_SPHERE_REDUCE_CYCLES_80M_U26_MASK                                      0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG141_FIELD_SPHERE_REDUCE_CYCLES_80M_U27_MASK                                      0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG141_FIELD_SPHERE_REDUCE_CYCLES_80M_U28_MASK                                      0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG142 0x4A8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG142_FIELD_SPHERE_REDUCE_CYCLES_80M_U29_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG142_FIELD_SPHERE_REDUCE_CYCLES_80M_U30_MASK                                      0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG142_FIELD_SPHERE_REDUCE_CYCLES_80M_U31_MASK                                      0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG142_FIELD_SPHERE_REDUCE_CYCLES_80M_U32_MASK                                      0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG143 0x4AC */
#define B0_PHY_RX_FD_PHY_RXFD_REG143_FIELD_SPHERE_REDUCE_CYCLES_160M_U13_MASK                                     0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG143_FIELD_SPHERE_REDUCE_CYCLES_160M_U14_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG143_FIELD_SPHERE_REDUCE_CYCLES_160M_U15_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG143_FIELD_SPHERE_REDUCE_CYCLES_160M_U16_MASK                                     0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG144 0x4B0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG144_FIELD_SPHERE_REDUCE_CYCLES_160M_U17_MASK                                     0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG144_FIELD_SPHERE_REDUCE_CYCLES_160M_U18_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG144_FIELD_SPHERE_REDUCE_CYCLES_160M_U19_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG144_FIELD_SPHERE_REDUCE_CYCLES_160M_U20_MASK                                     0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG145 0x4B4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG145_FIELD_SPHERE_REDUCE_CYCLES_160M_U21_MASK                                     0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG145_FIELD_SPHERE_REDUCE_CYCLES_160M_U22_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG145_FIELD_SPHERE_REDUCE_CYCLES_160M_U23_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG145_FIELD_SPHERE_REDUCE_CYCLES_160M_U24_MASK                                     0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG146 0x4B8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG146_FIELD_SPHERE_REDUCE_CYCLES_160M_U25_MASK                                     0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG146_FIELD_SPHERE_REDUCE_CYCLES_160M_U26_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG146_FIELD_SPHERE_REDUCE_CYCLES_160M_U27_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG146_FIELD_SPHERE_REDUCE_CYCLES_160M_U28_MASK                                     0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG147 0x4BC */
#define B0_PHY_RX_FD_PHY_RXFD_REG147_FIELD_SPHERE_REDUCE_CYCLES_160M_U29_MASK                                     0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG147_FIELD_SPHERE_REDUCE_CYCLES_160M_U30_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG147_FIELD_SPHERE_REDUCE_CYCLES_160M_U31_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG147_FIELD_SPHERE_REDUCE_CYCLES_160M_U32_MASK                                     0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG148 0x4C0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG148_FIELD_NOISE_FOR_AV_SNR_MASK                                                  0x00FFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG149 0x4C4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG149_FIELD_SPHERE_REDUCE_CYCLES_80M_U12_MASK                                      0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG149_FIELD_SPHERE_REDUCE_CYCLES_160M_U12_MASK                                     0x00003F00
#define B0_PHY_RX_FD_PHY_RXFD_REG149_FIELD_SPHERE_REDUCE_CYCLES_80P80_MASK                                        0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG149_FIELD_SPHERE_REDUCE_CYCLES_RISC_MASK                                         0x3F000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG150 0x4C8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG150_FIELD_STS_FD_STATUS1_MASK                                                    0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG151 0x4D0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG151_FIELD_STS_FD_STATUS2_MASK                                                    0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG152 0x4D4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG152_FIELD_PEAK_ERROR4_SS0_MASK                                                   0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG152_FIELD_PEAK_ERROR4_SS1_MASK                                                   0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG152_FIELD_PEAK_ERROR4_SS2_MASK                                                   0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG152_FIELD_PEAK_ERROR4_SS3_MASK                                                   0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG153 0x4D8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG153_FIELD_TEST_PLUG_LEN_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG154 0x4DC */
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_FEATURE_ENABLE_MASK                                        0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_LEGACY_ENABLE_MASK                                         0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_HT_ENABLE_MASK                                             0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_VHT_ENABLE_MASK                                            0x00000008
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_HE_SU_ENABLE_MASK                                          0x00000010
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_HE_TB_ENABLE_MASK                                          0x00000020
#define B0_PHY_RX_FD_PHY_RXFD_REG154_FIELD_SYMBOL_TIME_HE_SU_LCP_COUNT_MASK                                       0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG155 0x4E0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG155_FIELD_SYMBOL_TIME_HE_SU_MCP_COUNT_MASK                                       0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG155_FIELD_SYMBOL_TIME_HE_SU_SCP_COUNT_MASK                                       0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG156 0x4E4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG156_FIELD_SYMBOL_TIME_VHT_LCP_COUNT_MASK                                         0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG156_FIELD_SYMBOL_TIME_VHT_SCP_COUNT_MASK                                         0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG157 0x4E8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG157_FIELD_SYMBOL_TIME_HE_TB_LCP_COUNT_MASK                                       0x00003FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG157_FIELD_SYMBOL_TIME_HE_TB_MCP_COUNT_MASK                                       0x3FFF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG158 0x4EC */
#define B0_PHY_RX_FD_PHY_RXFD_REG158_FIELD_STS_FD_STATUS3_MASK                                                    0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG159 0x4F0 */
#define B0_PHY_RX_FD_PHY_RXFD_REG159_FIELD_LOG2_EVM_DONE_MASK                                                     0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG159_FIELD_LSIG_EVM_DONE_MASK                                                     0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG159_FIELD_LSIG_EVM_MASK                                                          0x0000FF00
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG160 0x4F4 */
#define B0_PHY_RX_FD_PHY_RXFD_REG160_FIELD_SHORT_PACKET_SCYCLES_ENABLE_MASK                                       0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG160_FIELD_SHORT_PACKET_SCYCLES_BW_MASK                                           0x000000F0
#define B0_PHY_RX_FD_PHY_RXFD_REG160_FIELD_SHORT_PACKET_SCYCLES_SYMBOL_THR_MASK                                   0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG160_FIELD_SHORT_PACKET_SCYCLES_USER_THR_MASK                                     0x003F0000
#define B0_PHY_RX_FD_PHY_RXFD_REG160_FIELD_SHORT_PACKET_SCYCLES_VALUE_MASK                                        0xFFC00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG161 0x4F8 */
#define B0_PHY_RX_FD_PHY_RXFD_REG161_FIELD_HE_EXT_MODE_MASK                                                       0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG161_FIELD_HE_EXT_MODE_VALID_MASK                                                 0x00000002
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG162 0x4FC */
#define B0_PHY_RX_FD_PHY_RXFD_REG162_FIELD_SPHERE_REDUCE_WAFA_TONES_HE_CP_1_MASK                                  0x0000003F
#define B0_PHY_RX_FD_PHY_RXFD_REG162_FIELD_SPHERE_REDUCE_WAFA_TONES_HE_CP_2_MASK                                  0x00000FC0
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG163 0x500 */
#define B0_PHY_RX_FD_PHY_RXFD_REG163_FIELD_NOISE_GAIN_MAX_MASK                                                    0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG163_FIELD_NOISE_GAIN_BIN_NUM_MASK                                                0x07FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG164 0x504 */
#define B0_PHY_RX_FD_PHY_RXFD_REG164_FIELD_NOISE_GAIN_SUM_MASK                                                    0x000FFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG165 0x508 */
#define B0_PHY_RX_FD_PHY_RXFD_REG165_FIELD_SPHERE_REDUCE_CYCLES_MU_UL_20M_MASK                                    0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG165_FIELD_SPHERE_REDUCE_CYCLES_MU_UL_40M_MASK                                    0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG165_FIELD_SPHERE_REDUCE_CYCLES_MU_UL_80M_MASK                                    0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG165_FIELD_SPHERE_REDUCE_CYCLES_MU_UL_160M_MASK                                   0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG166 0x50C */
#define B0_PHY_RX_FD_PHY_RXFD_REG166_FIELD_POWER_PER_RU_TD_GAIN_ANT4_MASK                                         0x000001FF
#define B0_PHY_RX_FD_PHY_RXFD_REG166_FIELD_POWER_PER_RU_SYSTEM_GAIN_ANT4_MASK                                     0x00FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG167 0x510 */
#define B0_PHY_RX_FD_PHY_RXFD_REG167_FIELD_HESTF_NOISE_GAIN_ANT4_MASK                                             0x0000007F
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG168 0x514 */
#define B0_PHY_RX_FD_PHY_RXFD_REG168_FIELD_NCB_PRANK4_MASK_AN4_MASK                                               0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG168_FIELD_FCB_PRANK4_MASK_AN4_MASK                                               0x0000FF00
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG169 0x518 */
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_SECOND_DECODE_AFTER_AIDED_PHASE_CORRECTION_MASK                        0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_SECOND_PHASE_TRACKING_EN_SMOOTHING_MASK                                0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_KPI_PREAMBLE_SYMBOLS_SUPPORT_MASK                                      0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_KPI_PACKET_TYPE_SUPPORT_MASK                                           0x000000F8
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_KPI_BANDWIDTH_SUPPORT_MASK                                             0x00000F00
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_SECOND_PHASE_TRACKING_ADDED_SMOOTHING_SNR_MASK                         0x000FF000
#define B0_PHY_RX_FD_PHY_RXFD_REG169_FIELD_KPI_SPHERE_CYCLES_20M_MASK                                             0xFFF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG170 0x51C */
#define B0_PHY_RX_FD_PHY_RXFD_REG170_FIELD_NDP_FDBK_K_RATIO_4_ANTS_MASK                                           0x0000001F
#define B0_PHY_RX_FD_PHY_RXFD_REG170_FIELD_NDP_FDBK_K_RATIO_3_ANTS_MASK                                           0x000003E0
#define B0_PHY_RX_FD_PHY_RXFD_REG170_FIELD_NDP_FDBK_K_RATIO_2_ANTS_MASK                                           0x00007C00
#define B0_PHY_RX_FD_PHY_RXFD_REG170_FIELD_NDP_FDBK_K_RATIO_1_ANTS_MASK                                           0x000F8000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG171 0x520 */
#define B0_PHY_RX_FD_PHY_RXFD_REG171_FIELD_NDP_FDBK_P_THRESHOLD_CFG_MASK                                          0x000FFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG172 0x524 */
#define B0_PHY_RX_FD_PHY_RXFD_REG172_FIELD_MU_HDR_MEMORY_RM_MASK                                                  0x0000001F
#define B0_PHY_RX_FD_PHY_RXFD_REG172_FIELD_SU_HDR_MEMORY_RM_MASK                                                  0x000003E0
#define B0_PHY_RX_FD_PHY_RXFD_REG172_FIELD_SU_PHASES_RM_VALUE_MASK                                                0x00001C00
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG173 0x528 */
#define B0_PHY_RX_FD_PHY_RXFD_REG173_FIELD_LAST_QR_NUM_OF_CYCELS_MASK                                             0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG173_FIELD_MU_UL_4SS_ENABLE_NOISE_TRK_MASK                                        0x00000100
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG174 0x52C */
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT0_H_S_MEM_RM_VALUE_MASK                                     0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT0_Q_V_MEM_RM_VALUE_MASK                                     0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT1_H_S_MEM_RM_VALUE_MASK                                     0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT1_Q_V_MEM_RM_VALUE_MASK                                     0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT2_H_S_MEM_RM_VALUE_MASK                                     0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG174_FIELD_QR_GC5_UNIT2_Q_V_MEM_RM_VALUE_MASK                                     0x00700000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG175 0x530 */
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT0_H_S_MEM_RM_VALUE_MASK                                     0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT0_Q_V_MEM_RM_VALUE_MASK                                     0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT1_H_S_MEM_RM_VALUE_MASK                                     0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT1_Q_V_MEM_RM_VALUE_MASK                                     0x00007000
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT2_H_S_MEM_RM_VALUE_MASK                                     0x00070000
#define B0_PHY_RX_FD_PHY_RXFD_REG175_FIELD_QR_GC6_UNIT2_Q_V_MEM_RM_VALUE_MASK                                     0x00700000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG176 0x534 */
#define B0_PHY_RX_FD_PHY_RXFD_REG176_FIELD_QR_GC3_PM_RM_VALUE_MASK                                                0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG176_FIELD_QR_GC4_PM_RM_VALUE_MASK                                                0x00000070
#define B0_PHY_RX_FD_PHY_RXFD_REG176_FIELD_QR_GC5_PM_RM_VALUE_MASK                                                0x00000700
#define B0_PHY_RX_FD_PHY_RXFD_REG176_FIELD_QR_GC6_PM_RM_VALUE_MASK                                                0x00007000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG177 0x538 */
#define B0_PHY_RX_FD_PHY_RXFD_REG177_FIELD_ERROR4_MIN_0_MASK                                                      0x0003FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REG177_FIELD_ERROR4_BIN_0_MASK                                                      0x7FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG178 0x53C */
#define B0_PHY_RX_FD_PHY_RXFD_REG178_FIELD_ERROR4_MIN_1_MASK                                                      0x0003FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REG178_FIELD_ERROR4_BIN_1_MASK                                                      0x7FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG179 0x540 */
#define B0_PHY_RX_FD_PHY_RXFD_REG179_FIELD_ERROR4_MIN_2_MASK                                                      0x0003FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REG179_FIELD_ERROR4_BIN_2_MASK                                                      0x7FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG180 0x544 */
#define B0_PHY_RX_FD_PHY_RXFD_REG180_FIELD_ERROR4_MIN_3_MASK                                                      0x0003FFFF
#define B0_PHY_RX_FD_PHY_RXFD_REG180_FIELD_ERROR4_BIN_3_MASK                                                      0x7FF00000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG181 0x548 */
#define B0_PHY_RX_FD_PHY_RXFD_REG181_FIELD_CSM_SMOOTHING_OLD_SCHEM_MASK                                           0x00000001
#define B0_PHY_RX_FD_PHY_RXFD_REG181_FIELD_CSM_PACKET_DETECT_OLD_SCHEM_MASK                                       0x00000002
#define B0_PHY_RX_FD_PHY_RXFD_REG181_FIELD_DISABLE_SPHERE_COLUMN_SORTING_MASK                                     0x00000004
#define B0_PHY_RX_FD_PHY_RXFD_REG181_FIELD_DISABLE_SPHERE_COLUMN_SORTING_4SS_MASK                                 0x00000008
#define B0_PHY_RX_FD_PHY_RXFD_REG181_FIELD_CSM_WAIT_FOR_SIG1_MASK                                                 0x00000010
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG182 0x54C */
#define B0_PHY_RX_FD_PHY_RXFD_REG182_FIELD_WAIT_1X1_CYC_LEG_MASK                                                  0x000007FF
#define B0_PHY_RX_FD_PHY_RXFD_REG182_FIELD_WAIT_1X1_CYC_HE_MASK                                                   0x07FF0000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG183 0x550 */
#define B0_PHY_RX_FD_PHY_RXFD_REG183_FIELD_MEM_RXFD_DS_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG184 0x554 */
#define B0_PHY_RX_FD_PHY_RXFD_REG184_FIELD_MEM_RXFD_SD_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG185 0x558 */
#define B0_PHY_RX_FD_PHY_RXFD_REG185_FIELD_MEM_RXFD_ROP_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG186 0x55C */
#define B0_PHY_RX_FD_PHY_RXFD_REG186_FIELD_MEM_RXFD_LS1_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG187 0x560 */
#define B0_PHY_RX_FD_PHY_RXFD_REG187_FIELD_MEM_RXFD_LS2_MASK                                                      0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG188 0x564 */
#define B0_PHY_RX_FD_PHY_RXFD_REG188_FIELD_HYP_RXFD_DATA_CH_RAM_1X1_RM_MASK                                       0x00000007
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG189 0x568 */
#define B0_PHY_RX_FD_PHY_RXFD_REG189_FIELD_FFT_SATURATION_SAMPLES_ANT0_MASK                                       0x000000FF
#define B0_PHY_RX_FD_PHY_RXFD_REG189_FIELD_FFT_SATURATION_SAMPLES_ANT1_MASK                                       0x0000FF00
#define B0_PHY_RX_FD_PHY_RXFD_REG189_FIELD_FFT_SATURATION_SAMPLES_ANT2_MASK                                       0x00FF0000
#define B0_PHY_RX_FD_PHY_RXFD_REG189_FIELD_FFT_SATURATION_SAMPLES_ANT3_MASK                                       0xFF000000
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG190 0x56C */
#define B0_PHY_RX_FD_PHY_RXFD_REG190_FIELD_FFT_SATURATION_SAMPLES_ANT4_MASK                                       0x000000FF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG191 0x570 */
#define B0_PHY_RX_FD_PHY_RXFD_REG191_FIELD_SECOND_PHASE_TRACKING_ADDED_SMOOTHING_SYMBOLS_MASK                     0x00007FFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG192 0x574 */
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_KPI_SPHERE_CYCLES_40M_MASK                                             0x00000FFF
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_REDUCE_SPHERE_CYC_LAST_SYM_OFDMA_VIT_MASK                              0x00010000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_REDUCE_SPHERE_CYC_LAST_SYM_MIMO_20_LDPC_MASK                           0x00020000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_REDUCE_SPHERE_CYC_LAST_SYM_OFDMA_20_MASK                               0x00040000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_REDUCE_SPHERE_CYC_LAST_SYM_HE_TRIG_40_MASK                             0x00080000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_REDUCE_SPHERE_CYC_LAST_SYM_HE_TRIG_80_MASK                             0x00100000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_KPI_SMOOTHING_SECOND_SYMBOL_MASK                                       0x01000000
#define B0_PHY_RX_FD_PHY_RXFD_REG192_FIELD_KPI_SMOOTHING_THIRD_SYMBOL_NOT_LEGACY_MASK                             0x02000000
//========================================
/* REG_PHY_RX_FD_SPARE_FROM_FD 0x578 */
#define B0_PHY_RX_FD_SPARE_FROM_FD_FIELD_SPARE_FROM_FD_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_FROM_TX 0x57C */
#define B0_PHY_RX_FD_SPARE_FROM_TX_FIELD_SPARE_FROM_TX_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_FROM_BE 0x580 */
#define B0_PHY_RX_FD_SPARE_FROM_BE_FIELD_SPARE_FROM_BE_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_FROM_TD 0x584 */
#define B0_PHY_RX_FD_SPARE_FROM_TD_FIELD_SPARE_FROM_TD_MASK                                                       0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_TO_FD 0x588 */
#define B0_PHY_RX_FD_SPARE_TO_FD_FIELD_SPARE_TO_FD_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_TO_TX 0x58C */
#define B0_PHY_RX_FD_SPARE_TO_TX_FIELD_SPARE_TO_TX_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_TO_BE 0x590 */
#define B0_PHY_RX_FD_SPARE_TO_BE_FIELD_SPARE_TO_BE_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SPARE_TO_TD 0x594 */
#define B0_PHY_RX_FD_SPARE_TO_TD_FIELD_SPARE_TO_TD_MASK                                                           0x0000FFFF
//========================================
/* REG_PHY_RX_FD_NUM_OF_ACTIVE_USERS 0x598 */
#define B0_PHY_RX_FD_NUM_OF_ACTIVE_USERS_FIELD_NUM_OF_ACTIVE_USERS_MASK                                           0x0000003F
//========================================
/* REG_PHY_RX_FD_FD_TEST_BUS_GCLK_EN 0x59C */
#define B0_PHY_RX_FD_FD_TEST_BUS_GCLK_EN_FIELD_FD_TEST_BUS_GCLK_EN_MASK                                           0x00000001
#define B0_PHY_RX_FD_FD_TEST_BUS_GCLK_EN_FIELD_TEST_BUS_GENERATORS_EN_MASK                                        0x00000002
//========================================
/* REG_PHY_RX_FD_DFE_ENABLE 0x5A0 */
#define B0_PHY_RX_FD_DFE_ENABLE_FIELD_DFE_ENABLE_MASK                                                             0x00000001
//========================================
/* REG_PHY_RX_FD_QR_LOW_POWER 0x5A4 */
#define B0_PHY_RX_FD_QR_LOW_POWER_FIELD_QR_UPD_NUM_INIT_SYM_MASK                                                  0x0000001F
#define B0_PHY_RX_FD_QR_LOW_POWER_FIELD_QR_UPD_PERIOD_MASK                                                        0x00000F00
#define B0_PHY_RX_FD_QR_LOW_POWER_FIELD_QR_UPD_COUNT_ALL_OFDM_SYMBOLS_MASK                                        0x00001000
#define B0_PHY_RX_FD_QR_LOW_POWER_FIELD_QR_UPD_NUM_INIT_SYM_DFE_MASK                                              0x0003E000
//========================================
/* REG_PHY_RX_FD_TEST_BUS_GROUPING 0x5A8 */
#define B0_PHY_RX_FD_TEST_BUS_GROUPING_FIELD_TEST_BUS_GROUPING_MASK                                               0x00000003
//========================================
/* REG_PHY_RX_FD_MAX_QR_TIME 0x5AC */
#define B0_PHY_RX_FD_MAX_QR_TIME_FIELD_MAX_QR_TIME_MASK                                                           0x000003FF
//========================================
/* REG_PHY_RX_FD_FFT_OUT_MULT_SHIFT 0x5B0 */
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_SHIFT_60_MASK                                               0x00000003
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_MULT_60_MASK                                                0x0000003C
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_SHIFT_120_MASK                                              0x000000C0
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_MULT_120_MASK                                               0x00000F00
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_SHIFT_140_MASK                                              0x00003000
#define B0_PHY_RX_FD_FFT_OUT_MULT_SHIFT_FIELD_FFT_OUT_MULT_140_MASK                                               0x0003C000
//========================================
/* REG_PHY_RX_FD_QR_NOISE_GAIN_FREEZE_DISABLE 0x5B4 */
#define B0_PHY_RX_FD_QR_NOISE_GAIN_FREEZE_DISABLE_FIELD_QR_NOISE_GAIN_FREEZE_DISABLE_MASK                         0x00000001
//========================================
/* REG_PHY_RX_FD_MIN_SPHERE_CYCLES 0x5B8 */
#define B0_PHY_RX_FD_MIN_SPHERE_CYCLES_FIELD_MIN_SPHERE_CYCLES_2QR_MASK                                           0x000003FF
#define B0_PHY_RX_FD_MIN_SPHERE_CYCLES_FIELD_MIN_SPHERE_CYCLES_6QR_MASK                                           0x000FFC00
//========================================
/* REG_PHY_RX_FD_SMOOTH_WDATA_DELAY_RM 0x5BC */
#define B0_PHY_RX_FD_SMOOTH_WDATA_DELAY_RM_FIELD_SMOOTH_WDATA_DELAY_RM_MASK                                       0x00000007
//========================================
/* REG_PHY_RX_FD_RCR_MAX_VAL 0x5C0 */
#define B0_PHY_RX_FD_RCR_MAX_VAL_FIELD_MAX_NSS_MASK                                                               0x00000003
#define B0_PHY_RX_FD_RCR_MAX_VAL_FIELD_MAX_MCS_MASK                                                               0x000000F0
//========================================
/* REG_PHY_RX_FD_SU_BF_ERROR_MASK 0x5C4 */
#define B0_PHY_RX_FD_SU_BF_ERROR_MASK_FIELD_SU_BF_ERROR_MASK_MASK                                                 0x0000FFFF
//========================================
/* REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG0 0x5C8 */
#define B0_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG0_FIELD_SU_BF_OFDMA_ERROR_REG0_MASK                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG1 0x5CC */
#define B0_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG1_FIELD_SU_BF_OFDMA_ERROR_REG1_MASK                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG2 0x5D0 */
#define B0_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG2_FIELD_SU_BF_OFDMA_ERROR_REG2_MASK                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG3 0x5D4 */
#define B0_PHY_RX_FD_SU_BF_OFDMA_ERROR_REG3_FIELD_SU_BF_OFDMA_ERROR_REG3_MASK                                     0xFFFFFFFF
//========================================
/* REG_PHY_RX_FD_MU_TX_ERROR_CAUSE_MASK 0x5D8 */
#define B0_PHY_RX_FD_MU_TX_ERROR_CAUSE_MASK_FIELD_MU_TX_ERROR_CAUSE_MASK_MASK                                     0x00000FFF
#define B0_PHY_RX_FD_MU_TX_ERROR_CAUSE_MASK_FIELD_MU_TX_ERROR_CAUSE2_MASK_MASK                                    0x01FFF000
//========================================
/* REG_PHY_RX_FD_BF_EQ_POWER_PER_SS 0x5DC */
#define B0_PHY_RX_FD_BF_EQ_POWER_PER_SS_FIELD_BF_EQ_POWER_PER_SS_MASK                                             0x00000001
#define B0_PHY_RX_FD_BF_EQ_POWER_PER_SS_FIELD_BF_EQ_POWER_PER_SS_N_STREAMS_MASK                                   0x0000001E
//========================================
/* REG_PHY_RX_FD_QR_ACTIVATIONS 0x5E0 */
#define B0_PHY_RX_FD_QR_ACTIVATIONS_FIELD_QR_ACTIVATIONS_MASK                                                     0x0000007F
//========================================
/* REG_PHY_RX_FD_NOISE_TRK_FREEZE_SHIFTER_THR 0x5E4 */
#define B0_PHY_RX_FD_NOISE_TRK_FREEZE_SHIFTER_THR_FIELD_NOISE_TRK_FREEZE_SHIFTER_THR_MASK                         0x00000003
//========================================
/* REG_PHY_RX_FD_SUPERSEG_NOT_ON_PRIMARY_CB 0x5E8 */
#define B0_PHY_RX_FD_SUPERSEG_NOT_ON_PRIMARY_CB_FIELD_SUPERSEG_NOT_ON_PRIMARY_CB_MASK                             0x00000001
//========================================
/* REG_PHY_RX_FD_HE_DFE_SYM_START 0x5EC */
#define B0_PHY_RX_FD_HE_DFE_SYM_START_FIELD_HE_DFE_SYM_START_MASK                                                 0x00007FFF
//========================================
/* REG_PHY_RX_FD_VHT_DFE_SYM_START 0x5F0 */
#define B0_PHY_RX_FD_VHT_DFE_SYM_START_FIELD_VHT_DFE_SYM_START_MASK                                               0x00007FFF
//========================================
/* REG_PHY_RX_FD_START_FROM_QR_0_ENABLE 0x5F4 */
#define B0_PHY_RX_FD_START_FROM_QR_0_ENABLE_FIELD_START_FROM_QR_0_ENABLE_MASK                                     0x00000001
//========================================
/* REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT0 0x5F8 */
#define B0_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT0_FIELD_SYMBOL_TIME_EHT_2SEG_LCP_COUNT_MASK                        0x00003FFF
#define B0_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT0_FIELD_SYMBOL_TIME_EHT_2SEG_MCP_COUNT_MASK                        0x0FFFC000
//========================================
/* REG_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT1 0x5FC */
#define B0_PHY_RX_FD_SYMBOL_TIME_EHT_2SEG_COUNT1_FIELD_SYMBOL_TIME_EHT_2SEG_SCP_COUNT_MASK                        0x00003FFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG_RAM_RM 0x600 */
#define B0_PHY_RX_FD_PHY_RXFD_REG_RAM_RM_FIELD_AMP_COR_REF_PRV_RAM_RM_MASK                                        0x00000007
#define B0_PHY_RX_FD_PHY_RXFD_REG_RAM_RM_FIELD_HYP_RXFD_CH_ESTI_RAM_RM_MASK                                       0x00000038
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_REG_CH_ESTI 0x604 */
#define B0_PHY_RX_FD_PHY_RXFD_REG_CH_ESTI_FIELD_EX_LTF_NOISE_EN_MASK                                              0x00000001
//========================================
/* REG_PHY_RX_FD_QR_LIMIT_1SS 0x608 */
#define B0_PHY_RX_FD_QR_LIMIT_1SS_FIELD_QR_LIMIT_1SS_BAND_2G_MASK                                                 0x000001FF
#define B0_PHY_RX_FD_QR_LIMIT_1SS_FIELD_QR_LIMIT_1SS_BAND_5G_MASK                                                 0x0003FE00
#define B0_PHY_RX_FD_QR_LIMIT_1SS_FIELD_QR_LIMIT_1SS_BAND_6G_MASK                                                 0x07FC0000
//========================================
/* REG_PHY_RX_FD_QR_LIMIT_2SS 0x60C */
#define B0_PHY_RX_FD_QR_LIMIT_2SS_FIELD_QR_LIMIT_2SS_BAND_2G_MASK                                                 0x000001FF
#define B0_PHY_RX_FD_QR_LIMIT_2SS_FIELD_QR_LIMIT_2SS_BAND_5G_MASK                                                 0x0003FE00
#define B0_PHY_RX_FD_QR_LIMIT_2SS_FIELD_QR_LIMIT_2SS_BAND_6G_MASK                                                 0x07FC0000
//========================================
/* REG_PHY_RX_FD_QR_LIMIT_3SS 0x610 */
#define B0_PHY_RX_FD_QR_LIMIT_3SS_FIELD_QR_LIMIT_3SS_BAND_2G_MASK                                                 0x000001FF
#define B0_PHY_RX_FD_QR_LIMIT_3SS_FIELD_QR_LIMIT_3SS_BAND_5G_MASK                                                 0x0003FE00
#define B0_PHY_RX_FD_QR_LIMIT_3SS_FIELD_QR_LIMIT_3SS_BAND_6G_MASK                                                 0x07FC0000
//========================================
/* REG_PHY_RX_FD_QR_LIMIT_4SS 0x614 */
#define B0_PHY_RX_FD_QR_LIMIT_4SS_FIELD_QR_LIMIT_4SS_BAND_2G_MASK                                                 0x000001FF
#define B0_PHY_RX_FD_QR_LIMIT_4SS_FIELD_QR_LIMIT_4SS_BAND_5G_MASK                                                 0x0003FE00
#define B0_PHY_RX_FD_QR_LIMIT_4SS_FIELD_QR_LIMIT_4SS_BAND_6G_MASK                                                 0x07FC0000
//========================================
/* REG_PHY_RX_FD_RISC_SPHERE_CYCLES_REWIND_OVERRIDE 0x618 */
#define B0_PHY_RX_FD_RISC_SPHERE_CYCLES_REWIND_OVERRIDE_FIELD_RISC_SPHERE_CYCLES_REWIND_OVERRIDE_MASK             0x000001FF
//========================================
/* REG_PHY_RX_FD_PM_FD_OVERHEAD 0x61C */
#define B0_PHY_RX_FD_PM_FD_OVERHEAD_FIELD_PM_FD_OVERHEAD_MASK                                                     0x00000FFF
//========================================
/* REG_PHY_RX_FD_PHY_RXFD_BF_SETUP 0x620 */
#define B0_PHY_RX_FD_PHY_RXFD_BF_SETUP_FIELD_SU_BF_INTERP_MASK                                                    0x00000001
//========================================
/* REG_PHY_RX_FD_SMOOTHING_LONG_FILTER_ENABLE 0x624 */
#define B0_PHY_RX_FD_SMOOTHING_LONG_FILTER_ENABLE_FIELD_SMOOTHING_LONG_FILTER_ENABLE_MASK                         0x00000001
//========================================
/* REG_PHY_RX_FD_SPHERE_CYC_CT_CAN_WORK 0x628 */
#define B0_PHY_RX_FD_SPHERE_CYC_CT_CAN_WORK_FIELD_SPHERE_CYC_CT_CAN_WORK_MASK                                     0x00003FFF
//========================================
/* REG_PHY_RX_FD_TD_GAIN_CONFIG 0x62C */
#define B0_PHY_RX_FD_TD_GAIN_CONFIG_FIELD_PM_SU_BF_TD_GAIN_EN_MASK                                                0x00000001
#define B0_PHY_RX_FD_TD_GAIN_CONFIG_FIELD_PM_SU_BF_SPMAP_NORMALIZATION_EN_MASK                                    0x00000002
//========================================
/* REG_PHY_RX_FD_SPHERE_POWER_SAVE 0x630 */
#define B0_PHY_RX_FD_SPHERE_POWER_SAVE_FIELD_SPHERE_POWER_SAVE_MASK                                               0x00000001
//========================================
/* REG_PHY_RX_FD_SPHERE_CYCLES_RO 0x634 */
#define B0_PHY_RX_FD_SPHERE_CYCLES_RO_FIELD_SPHERE_CYCLES_INIT_VAL_MASK                                           0x00003FFF
#define B0_PHY_RX_FD_SPHERE_CYCLES_RO_FIELD_SPHERE_CYCLES_REWIND_MASK                                             0x3FFF0000


#endif // _PHY_RX_FD_REGS_H_
