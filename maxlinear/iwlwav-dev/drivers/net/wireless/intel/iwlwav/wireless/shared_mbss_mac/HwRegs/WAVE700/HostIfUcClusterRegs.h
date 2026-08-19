
/***********************************************************************************
File:				HostIfUcClusterRegs.h
Module:				hostIfUcCluster
SOC Revision:		latest
Generated at:       2022-05-04 10:58:44
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOST_IF_UC_CLUSTER_REGS_H_
#define _HOST_IF_UC_CLUSTER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOST_IF_UC_CLUSTER_BASE_ADDRESS                             MEMORY_MAP_UNIT_28_BASE_ADDRESS
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_START                   (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_00         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x80)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_01         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x84)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_02         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x88)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_03         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x8C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_04         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x90)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_05         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x94)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_06         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x98)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_07         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x9C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_08         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xA0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_09         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xA4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_10         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xA8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_11         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xAC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_12         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_13         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_14         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_15         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xBC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_16         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xC0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_17         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xC4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_18         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xC8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_19         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xCC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_20         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xD0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_21         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xD4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_22         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xD8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_23         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xDC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_24         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xE0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_25         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xE4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_26         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xE8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_27         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xEC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_28         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xF0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_29         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xF4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_30         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xF8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_31         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xFC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_DONE                    (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x100)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_IER                     (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x180)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_IRR                     (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x200)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_00             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x280)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_01             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x284)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_02             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x288)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_03             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x28C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_04             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x290)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_05             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x294)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_06             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x298)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_07             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x29C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_08             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2A0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_09             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2A4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_10             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2A8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_11             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2AC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_12             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2B0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_13             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2B4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_14             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2B8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_15             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2BC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_16             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2C0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_17             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2C4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_18             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2C8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_19             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2CC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_20             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2D0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_21             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2D4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_22             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2D8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_23             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2DC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_24             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2E0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_25             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2E4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_26             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2E8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_27             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2EC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_28             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2F0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_29             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2F4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_30             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2F8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_31             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x2FC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_00             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x300)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_01             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x304)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_02             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x308)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_03             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x30C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_04             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x310)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_05             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x314)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_06             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x318)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_07             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x31C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_08             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x320)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_09             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x324)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_10             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x328)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_11             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x32C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_12             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x330)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_13             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x334)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_14             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x338)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_15             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x33C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_16             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x340)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_17             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x344)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_18             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x348)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_19             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x34C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_20             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x350)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_21             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x354)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_22             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x358)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_23             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x35C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_24             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x360)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_25             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x364)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_26             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x368)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_27             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x36C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_28             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x370)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_29             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x374)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_30             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x378)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_31             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x37C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_00         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x380)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_01         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x384)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_02         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x388)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_03         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x38C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_04         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x390)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_05         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x394)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_06         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x398)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_07         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x39C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_08         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3A0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_09         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3A4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_10         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3A8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_11         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3AC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_12         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3B0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_13         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3B4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_14         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3B8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_15         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3BC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_16         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3C0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_17         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3C4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_18         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3C8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_19         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3CC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_20         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3D0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_21         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3D4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_22         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3D8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_23         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3DC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_24         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3E0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_25         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3E4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_26         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3E8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_27         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3EC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_28         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3F0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_29         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3F4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_30         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3F8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_31         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x3FC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_00          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x400)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_01          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x404)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_02          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x408)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_03          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x40C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_04          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x410)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_05          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x414)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_06          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x418)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_07          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x41C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_08          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x420)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_09          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x424)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_10          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x428)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_11          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x42C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_12          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x430)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_13          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x434)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_14          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x438)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_15          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x43C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_16          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x440)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_17          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x444)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_18          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x448)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_19          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x44C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_20          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x450)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_21          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x454)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_22          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x458)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_23          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x45C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_24          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x460)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_25          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x464)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_26          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x468)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_27          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x46C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_28          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x470)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_29          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x474)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_30          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x478)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_31          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x47C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_00          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x480)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_01          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x484)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_02          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x488)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_03          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x48C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_04          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x490)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_05          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x494)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_06          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x498)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_07          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x49C)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_08          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4A0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_09          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4A4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_10          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4A8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_11          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4AC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_12          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4B0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_13          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4B4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_14          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4B8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_15          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4BC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_16          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4C0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_17          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4C4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_18          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4C8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_19          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4CC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_20          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4D0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_21          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4D4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_22          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4D8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_23          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4DC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_24          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4E0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_25          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4E4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_26          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4E8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_27          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4EC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_28          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4F0)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_29          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4F4)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_30          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4F8)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_31          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x4FC)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_NULLIFY_PULSE           (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x500)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_STATUS_LO               (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x580)
#define	REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_STATUS_HI               (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x584)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_VER_REG                        (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x800)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_CONFIG_REG                     (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x880)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_DESTINATION_ADDRESS_REG        (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x900)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_SOURCE_ADDRESS_REG             (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x904)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_LEN_REG                        (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x908)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_CMD_DIRECT_COUNTER_REG         (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xA00)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_PRIO_0_CMD_COUNTER_REG_00      (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xA80)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_PRIO_1_CMD_COUNTER_REG_00      (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB00)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_TIMEOUT_CFG_REG                (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB80)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_TIMEOUT_ERROR_REG              (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB84)
#define	REG_HOST_IF_UC_CLUSTER_DMAE_PENDING_JOBS_REG               (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0xB88)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_UC_VERSION                  (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1000)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_MY_BAR                      (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1080)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_ICG_FORCE_CLK       (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1100)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CLK_GATED_ENABLE    (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1104)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_STATUS           (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1108)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_GRACE_COUNTER    (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x110C)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_REQ_STATUS       (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1110)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_KILL_CPU                    (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1180)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_NOC_CLIENT_NOT_PENDING      (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1200)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DMAE_FIFO_STATUS_0          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1280)
#define	REG_HOST_IF_UC_CLUSTER_REG_CCB_DMAE_FIFO_STATUS_1          (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1284)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRVLD                  (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x180C)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRCLR                  (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1810)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG0                 (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1814)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG1                 (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1818)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG3                 (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1820)
#define	REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG4                 (HOST_IF_UC_CLUSTER_BASE_ADDRESS + 0x1824)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_START 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventStart : 7; //Used_by_task_to_add_a_request_for_event, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegHostIfUcClusterRegHweventStart_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_00 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF00 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_01 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF01 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_02 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF02 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_03 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF03 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_04 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF04 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_05 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF05 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_06 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF06 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_07 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF07 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_08 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF08 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_09 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF09 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_10 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF10 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_11 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF11 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_12 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF12 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_13 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF13 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_14 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF14 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_15 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF15 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_16 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF16 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_17 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF17 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_18 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF18 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_19 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF19 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_20 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF20 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_21 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF21 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_22 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF22 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_23 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF23 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_24 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF24 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_25 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF25 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_26 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF26 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_27 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF27 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_28 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF28 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_29 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF29 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_30 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF30 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_EVENT_F_31 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftEventF31 : 1; //Indicates_that_HW_event_was_issued_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftEventF31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_DONE 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventDone : 5; //Used_by_task_to_indicate_the_host_that_it_finished_serving_an_event, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfUcClusterRegHweventDone_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_IER 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventIer : 32; //Interrupt_Enable_Register, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterRegHweventIer_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_IRR 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventIrr : 32; //Interrupt_Status_Register,_bit_per_event._Clear-on-Read_in_the_code., reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfUcClusterRegHweventIrr_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_00 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq00 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_01 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq01 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_02 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq02 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_03 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq03 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_04 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq04 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_05 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq05 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_06 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq06 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_07 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq07 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_08 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq08 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_09 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq09 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_10 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq10 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_11 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq11 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_12 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq12 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_13 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq13 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_14 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq14 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_15 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq15 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_16 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq16 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_17 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq17 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_18 0x2C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq18 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_19 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq19 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_20 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq20 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_21 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq21 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_22 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq22 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_23 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq23 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_24 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq24 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_25 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq25 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_26 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq26 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_27 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq27 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_28 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq28 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_29 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq29 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_30 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq30 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_REQ_31 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftReq31 : 6; //Used_by_task_to_add_a_request_for_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftReq31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_00 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq00 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_01 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq01 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_02 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq02 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_03 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq03 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_04 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq04 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_05 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq05 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_06 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq06 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_07 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq07 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_08 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq08 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_09 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq09 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_10 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq10 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_11 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq11 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_12 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq12 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_13 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq13 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_14 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq14 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_15 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq15 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_16 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq16 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_17 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq17 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_18 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq18 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_19 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq19 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_20 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq20 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_21 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq21 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_22 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq22 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_23 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq23 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_24 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq24 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_25 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq25 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_26 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq26 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_27 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq27 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_28 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq28 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_29 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq29 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_30 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq30 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_REQ_31 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardReq31 : 6; //Used_by_task_to_add_a_request_for_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfUcClusterRegHweventHardReq31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_00 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF00 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_01 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF01 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_02 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF02 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_03 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF03 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_04 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF04 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_05 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF05 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_06 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF06 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_07 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF07 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_08 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF08 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_09 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF09 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_10 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF10 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_11 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF11 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_12 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF12 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_13 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF13 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_14 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF14 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_15 0x3BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF15 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_16 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF16 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_17 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF17 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_18 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF18 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_19 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF19 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_20 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF20 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_21 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF21 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_22 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF22 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_23 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF23 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_24 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF24 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_25 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF25 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_26 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF26 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_27 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF27 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_28 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF28 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_29 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF29 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_30 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF30 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_EVENT_F_31 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardEventF31 : 1; //Indicates_that_a_task_is_waiting_for_this_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardEventF31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_00 0x400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF00 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_01 0x404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF01 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_02 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF02 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_03 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF03 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_04 0x410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF04 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_05 0x414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF05 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_06 0x418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF06 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_07 0x41C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF07 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_08 0x420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF08 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_09 0x424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF09 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_10 0x428 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF10 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_11 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF11 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_12 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF12 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_13 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF13 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_14 0x438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF14 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_15 0x43C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF15 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_16 0x440 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF16 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_17 0x444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF17 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_18 0x448 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF18 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_19 0x44C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF19 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_20 0x450 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF20 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_21 0x454 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF21 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_22 0x458 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF22 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_23 0x45C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF23 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_24 0x460 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF24 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_25 0x464 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF25 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_26 0x468 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF26 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_27 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF27 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_28 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF28 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_29 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF29 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_30 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF30 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_SOFT_TASK_F_31 0x47C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventSoftTaskF31 : 1; //Indicates_that_a_task_is_waiting_for_this_soft_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventSoftTaskF31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_00 0x480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF00 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF00_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_01 0x484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF01 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF01_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_02 0x488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF02 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF02_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_03 0x48C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF03 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF03_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_04 0x490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF04 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF04_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_05 0x494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF05 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF05_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_06 0x498 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF06 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF06_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_07 0x49C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF07 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF07_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_08 0x4A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF08 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF08_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_09 0x4A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF09 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF09_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_10 0x4A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF10 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF10_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_11 0x4AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF11 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF11_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_12 0x4B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF12 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF12_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_13 0x4B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF13 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF13_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_14 0x4B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF14 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF14_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_15 0x4BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF15 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF15_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_16 0x4C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF16 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF16_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_17 0x4C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF17 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF17_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_18 0x4C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF18 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF18_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_19 0x4CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF19 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF19_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_20 0x4D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF20 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF20_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_21 0x4D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF21 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF21_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_22 0x4D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF22 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF22_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_23 0x4DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF23 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF23_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_24 0x4E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF24 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF24_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_25 0x4E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF25 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF25_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_26 0x4E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF26 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF26_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_27 0x4EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF27 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF27_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_28 0x4F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF28 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF28_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_29 0x4F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF29 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF29_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_30 0x4F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF30 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF30_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_HARD_TASK_F_31 0x4FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventHardTaskF31 : 1; //Indicates_that_a_task_is_waiting_for_this_hard_event, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegHweventHardTaskF31_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_NULLIFY_PULSE 0x500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventNullifyPulse : 5; //Nullify_pulse_to_re-capture_hard-event_signal, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfUcClusterRegHweventNullifyPulse_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_STATUS_LO 0x580 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventStatusLo : 32; //32_LS_bits_of_Status, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfUcClusterRegHweventStatusLo_u;

/*REG_HOST_IF_UC_CLUSTER_REG_HWEVENT_STATUS_HI 0x584 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regHweventStatusHi : 32; //32_MS_bits_of_Status, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfUcClusterRegHweventStatusHi_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_VER_REG 0x800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaVerI : 32; //dma_ver, reset value: 0x2, access type: RO
	} bitFields;
} RegHostIfUcClusterDmaeVerReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_CONFIG_REG 0x880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxBurst : 16; //max_burst_-_the_DMA_will_deivide_the_transaction_to_several_burst_with_mac_burst_size. possible values are 1,...,16. Value N defines burst of N*16 bytes., reset value: 0x8, access type: RW
		uint32 ddrAlignEnable : 1; //ddr_align_enable-_the_DMA_will_deivide_the_transaction_to_burst_which_align_to_DDR_line_, reset value: 0x1, access type: RW
		uint32 outstanding : 2; //max_outstanding, Possible values are 1..2, reset value: 0x2, access type: RW
		uint32 reserved0 : 9;
		uint32 ddrLine : 4; //ddr_line-_to_be_use_in_DDR_align_access, reset value: 0x6, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeConfigReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_DESTINATION_ADDRESS_REG 0x900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 destinationAddressI : 32; //DMA_destination_address, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeDestinationAddressReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_SOURCE_ADDRESS_REG 0x904 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sourceAddressI : 32; //DMA_source_address, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeSourceAddressReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_LEN_REG 0x908 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sizeI : 32; //size, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeLenReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_CMD_DIRECT_COUNTER_REG 0xA00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cmdCounterI : 32; //Number_of_uc_command_request_passed(wrap_around)__RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeCmdDirectCounterReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_PRIO_0_CMD_COUNTER_REG_00 0xA80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prio0CmdCounterI00 : 32; //Number_of_uc_command_request_passed(wrap_around)__RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaePrio0CmdCounterReg00_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_PRIO_1_CMD_COUNTER_REG_00 0xB00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 prio1CmdCounterI00 : 32; //Number_of_uc_command_request_passed(wrap_around)__RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaePrio1CmdCounterReg00_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_TIMEOUT_CFG_REG 0xB80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaeTimeoutCfgI : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfUcClusterDmaeTimeoutCfgReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_TIMEOUT_ERROR_REG 0xB84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaeTimeoutErrorI : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterDmaeTimeoutErrorReg_u;

/*REG_HOST_IF_UC_CLUSTER_DMAE_PENDING_JOBS_REG 0xB88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaeWnpPendingI : 8; //no description, reset value: 0x0, access type: RO
		uint32 dmaeWpPendingI : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfUcClusterDmaePendingJobsReg_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_UC_VERSION 0x1000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbUcVersionChipVerMinor : 4; //chip_version_minor, reset value: 0x0, access type: RO
		uint32 regCcbUcVersionChipVerMajor : 4; //chip_version_major, reset value: 0x0, access type: RO
		uint32 regCcbUcVersionApp : 8; //0ingress,1egress,2DMAC,3HostIF,4Sneder,5RxH, reset value: 0x3, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfUcClusterRegCcbUcVersion_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_MY_BAR 0x1080 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbMyBar : 7; //PCIe_BAR_value, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegHostIfUcClusterRegCcbMyBar_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_ICG_FORCE_CLK 0x1100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDynamicIcgForceClk : 1; //Force_system_clk_and_enable_the_clock, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterRegCcbDynamicIcgForceClk_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CLK_GATED_ENABLE 0x1104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDynamicIcgEnable : 2; //uc_dynamic_clk_enable_, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfUcClusterRegCcbDynamicClkGatedEnable_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_STATUS 0x1108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDynamicIcgStatus : 2; //uc_dynamic_clk_status_, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfUcClusterRegCcbDynamicCgStatus_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_GRACE_COUNTER 0x110C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDynamicIcgGraceCycle : 10; //uc_sys_clock_counter_set_for_CG_grace_cycles, reset value: 0x20, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegHostIfUcClusterRegCcbDynamicCgGraceCounter_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DYNAMIC_CG_REQ_STATUS 0x1110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDynamicIcgReqStatus : 2; //Dynamic, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfUcClusterRegCcbDynamicCgReqStatus_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_KILL_CPU 0x1180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbKillCpu : 2; //kill_CPUs_signal_to_irq18_of_ARC-EMs, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfUcClusterRegCcbKillCpu_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_NOC_CLIENT_NOT_PENDING 0x1200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbNocClientNotPending : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegHostIfUcClusterRegCcbNocClientNotPending_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DMAE_FIFO_STATUS_0 0x1280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDmaeFifoUsedw : 8; //Provides debug info: num of entries, reset value: 0x0, access type: RO
		uint32 regCcbDmaeFifoWasOverflow : 1; //Shows if was_overflow; one-shot, reset value: 0x0, access type: RO
		uint32 regCcbDmaeFifoWasUnderflow : 1; //Shows if was_underflow; one-shot, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegHostIfUcClusterRegCcbDmaeFifoStatus0_u;

/*REG_HOST_IF_UC_CLUSTER_REG_CCB_DMAE_FIFO_STATUS_1 0x1284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regCcbDmaeFifoUsedw : 8; //no description, reset value: 0x0, access type: RO
		uint32 regCcbDmaeFifoWasOverflow : 1; //no description, reset value: 0x0, access type: RO
		uint32 regCcbDmaeFifoWasUnderflow : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegHostIfUcClusterRegCcbDmaeFifoStatus1_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRVLD 0x180C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clusterNocErrvld : 1; //1 indicates an error has been logged, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterClusterNocErrvld_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRCLR 0x1810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clusterNocErrclr : 1; //Set to 1 to clear ErrVld. NOTE The written value is not stored in ErrVld., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfUcClusterClusterNocErrclr_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG0 0x1814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lock : 1; //no description, reset value: 0x0, access type: RO
		uint32 opc : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 errcode : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 len1 : 12; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
		uint32 format : 1; //no description, reset value: 0x1, access type: RO
	} bitFields;
} RegHostIfUcClusterClusterNocErrlog0_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG1 0x1818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 routeid : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfUcClusterClusterNocErrlog1_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG3 0x1820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addr : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfUcClusterClusterNocErrlog3_u;

/*REG_HOST_IF_UC_CLUSTER_CLUSTER_NOC_ERRLOG4 0x1824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrMsb : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfUcClusterClusterNocErrlog4_u;



#endif // _HOST_IF_UC_CLUSTER_REGS_H_
