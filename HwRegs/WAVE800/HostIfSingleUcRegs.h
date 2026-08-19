
/***********************************************************************************
File:				HostIfSingleUcRegs.h
Module:				hostIfSingleUc
SOC Revision:		latest
Generated at:       2024-06-26 12:55:06
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOST_IF_SINGLE_UC_REGS_H_
#define _HOST_IF_SINGLE_UC_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOST_IF_SINGLE_UC_BASE_ADDRESS                 MEMORY_MAP_UNIT_1120_BASE_ADDRESS
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_00         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4000)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_01         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4004)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_02         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4008)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_03         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x400C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_04         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4010)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_05         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4014)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_06         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4018)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_07         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x401C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_08         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4020)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_09         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4024)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_10         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4028)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_11         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x402C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_12         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4030)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_13         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4034)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_14         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4038)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_15         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x403C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_16         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4040)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_17         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4044)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_18         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4048)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_19         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x404C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_CORE_RUN_HALT           (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4080)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_MY_CID                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4100)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_00               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4180)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_01               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4184)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_00               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4200)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_01               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4204)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_02               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4208)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_03               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x420C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_04               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4210)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_05               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4214)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_06               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4218)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_07               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x421C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_08               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4220)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_09               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4224)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_10               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4228)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_11               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x422C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_12               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4230)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_13               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4234)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_14               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4238)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_15               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x423C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_16               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4240)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_17               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4244)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_18               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4248)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_19               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x424C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_20               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4250)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_21               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4254)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_22               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4258)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_23               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x425C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_24               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4260)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_25               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4264)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_26               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4268)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_27               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x426C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_28               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4270)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_29               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4274)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_30               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4278)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_31               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x427C)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_RUN_HALT_ACK            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4280)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_00         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4400)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_01         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4404)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_00    (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4480)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_01    (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4484)
#define	REG_HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4500)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_NEXT                    (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4800)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_PREV                    (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4880)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_SLEEP_EN                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4900)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_START_SM                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4980)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_VAL                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4A00)
#define	REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_RESET               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x4A80)
#define	REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_NTID               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5000)
#define	REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ENABLE             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5080)
#define	REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ALGO               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5100)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_00                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5800)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_01                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5804)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_02                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5808)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_03                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x580C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_04                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5810)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_05                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5814)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_06                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5818)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_07                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x581C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_08                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5820)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_09                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5824)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_10                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5828)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_11                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x582C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_12                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5830)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_13                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5834)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_14                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5838)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_15                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x583C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_16                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5840)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_17                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5844)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_18                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5848)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_19                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x584C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_20                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5850)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_21                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5854)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_22                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5858)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_23                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x585C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_24                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5860)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_25                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5864)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_26                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5868)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_27                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x586C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_28                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5870)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_29                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5874)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_30                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5878)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_31                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x587C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_00              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5880)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_01              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5884)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_02              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5888)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_03              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x588C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_04              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5890)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_05              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5894)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_06              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5898)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_07              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x589C)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_08              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58A0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_09              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58A4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_10              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58A8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_11              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58AC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_12              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58B0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_13              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58B4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_14              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58B8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_15              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58BC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_16              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58C0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_17              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58C4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_18              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58C8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_19              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58CC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_20              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58D0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_21              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58D4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_22              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58D8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_23              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58DC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_24              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58E0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_25              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58E4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_26              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58E8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_27              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58EC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_28              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58F0)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_29              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58F4)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_30              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58F8)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_31              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x58FC)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_CTID                    (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5900)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_QUSTAMP                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5980)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_LOW       (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5A00)
#define	REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_HIGH      (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x5A04)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_START                   (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6000)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_00                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6080)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_01                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6084)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_02                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6088)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_03                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x608C)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_04                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6090)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_05                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6094)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_06                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6098)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_07                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x609C)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_08                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60A0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_09                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60A4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_10                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60A8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_11                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60AC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_12                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60B0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_13                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60B4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_14                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60B8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_15                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60BC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_16                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60C0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_17                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60C4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_18                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60C8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_19                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60CC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_20                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60D0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_21                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60D4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_22                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60D8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_23                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60DC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_24                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60E0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_25                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60E4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_26                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60E8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_27                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60EC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_28                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60F0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_29                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60F4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_30                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60F8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_31                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x60FC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_TID                     (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6100)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_00             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6180)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_01             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6184)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_02             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6188)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_03             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x618C)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_04             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6190)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_05             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6194)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_06             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6198)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_07             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x619C)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_08             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61A0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_09             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61A4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_10             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61A8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_11             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61AC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_12             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61B0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_13             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61B4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_14             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61B8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_15             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61BC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_16             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61C0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_17             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61C4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_18             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61C8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_19             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61CC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_20             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61D0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_21             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61D4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_22             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61D8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_23             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61DC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_24             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61E0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_25             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61E4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_26             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61E8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_27             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61EC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_28             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61F0)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_29             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61F4)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_30             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61F8)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_31             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x61FC)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_00              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6200)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_01              (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6204)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_00           (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6300)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_01           (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6304)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_00         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6400)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_01         (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6404)
#define	REG_HOST_IF_SINGLE_UC_REG_POPA_LAST_JOB                (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6480)
#define	REG_HOST_IF_SINGLE_UC_REG_PUSHE_PUTJOB                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6800)
#define	REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_00            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6880)
#define	REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_01            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6884)
#define	REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_02            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x6888)
#define	REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_03            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x688C)
#define	REG_HOST_IF_SINGLE_UC_REG_BGCOPY_DMA_REQ               (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x7000)
#define	REG_HOST_IF_SINGLE_UC_REG_DELAYA_START                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x7800)
#define	REG_HOST_IF_SINGLE_UC_REG_DELAYA_SCALE                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x7880)
#define	REG_HOST_IF_SINGLE_UC_REG_DELAYA_ABORT                 (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0x7900)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRVLD             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE00C)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRCLR             (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE010)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE014)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG1            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE018)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG3            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE020)
#define	REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG4            (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE024)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRVLD                   (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE40C)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRCLR                   (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE410)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE414)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG1                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE418)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG3                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE420)
#define	REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG4                  (HOST_IF_SINGLE_UC_BASE_ADDRESS + 0xE424)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_00 0x4000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector00 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector00_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_01 0x4004 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector01 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector01_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_02 0x4008 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector02 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector02_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_03 0x400C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector03 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector03_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_04 0x4010 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector04 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector04_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_05 0x4014 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector05 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector05_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_06 0x4018 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector06 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector06_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_07 0x401C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector07 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector07_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_08 0x4020 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector08 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector08_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_09 0x4024 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector09 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector09_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_10 0x4028 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector10 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector10_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_11 0x402C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector11 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector11_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_12 0x4030 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector12 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector12_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_13 0x4034 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector13 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector13_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_14 0x4038 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector14 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector14_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_15 0x403C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector15 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector15_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_16 0x4040 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector16 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector16_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_17 0x4044 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector17 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector17_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_18 0x4048 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector18 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector18_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_19 0x404C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootResetVector19 : 32; //uC_vector_table,_set_by_host, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootResetVector19_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_CORE_RUN_HALT 0x4080 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootCoreRunHalt : 2; //1_upon_reset,_cleared_by_host, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegBootCoreRunHalt_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_MY_CID 0x4100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootMyCid : 6; //holds_MyCID,_hard-wired, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegBootMyCid_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_00 0x4180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootGpReg00 : 32; //General_Purpose_Register, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootGpReg00_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_01 0x4184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootGpReg01 : 32; //General_Purpose_Register, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootGpReg01_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_00 0x4200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg00 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg00_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_01 0x4204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg01 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg01_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_02 0x4208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg02 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg02_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_03 0x420C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg03 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg03_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_04 0x4210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg04 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg04_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_05 0x4214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg05 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg05_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_06 0x4218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg06 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg06_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_07 0x421C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg07 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg07_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_08 0x4220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg08 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg08_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_09 0x4224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg09 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg09_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_10 0x4228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg10 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg10_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_11 0x422C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg11 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg11_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_12 0x4230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg12 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg12_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_13 0x4234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg13 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg13_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_14 0x4238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg14 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg14_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_15 0x423C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg15 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg15_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_16 0x4240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg16 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg16_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_17 0x4244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg17 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg17_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_18 0x4248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg18 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg18_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_19 0x424C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg19 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg19_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_20 0x4250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg20 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg20_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_21 0x4254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg21 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg21_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_22 0x4258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg22 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg22_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_23 0x425C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg23 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg23_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_24 0x4260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg24 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg24_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_25 0x4264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg25 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg25_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_26 0x4268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg26 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg26_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_27 0x426C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg27 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg27_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_28 0x4270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg28 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg28_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_29 0x4274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg29 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg29_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_30 0x4278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg30 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg30_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_31 0x427C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootSwReg31 : 32; //General_Purpose_Register, reset value: 0x1, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootSwReg31_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_RUN_HALT_ACK 0x4280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootRunHaltAck : 2; //Run_Ack, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegBootRunHaltAck_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_00 0x4400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuApParam00 : 32; //CPU_AP_PARAM, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootCpuApParam00_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_01 0x4404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuApParam01 : 32; //CPU_AP_PARAM, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegBootCpuApParam01_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_00 0x4480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuApParamWrite00 : 1; //CPU_AP_PARAM_Write, reset value: 0x0, access type: RW
		uint32 cpuApParamRead00 : 1; //CPU_AP_PARAM_Read, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegBootCpuApParamCtrl00_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_01 0x4484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuApParamWrite01 : 1; //CPU_AP_PARAM_Write, reset value: 0x0, access type: RW
		uint32 cpuApParamRead01 : 1; //CPU_AP_PARAM_Read, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegBootCpuApParamCtrl01_u;

/*REG_HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL 0x4500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regBootPageDccm : 2; //sets_DCCM_page, reset value: 0x0, access type: RW
		uint32 regBootPageIccm : 2; //sets_ICCM_page, reset value: 0x0, access type: RW
		uint32 regBootPageEngn : 1; //sets_Engine_page, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfSingleUcRegBootPageCtrl_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_NEXT 0x4800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmNext : 6; //Holds_the_ID_of_the_next_task, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegTssmNext_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_PREV 0x4880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmPrev : 6; //Holds_the_ID_of_the_previous_task, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegTssmPrev_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_SLEEP_EN 0x4900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmSleepEn : 1; //Sleep_Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegTssmSleepEn_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_START_SM 0x4980 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmStartSm : 1; //Start_Task_Switch_State_Machine, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegTssmStartSm_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_VAL 0x4A00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmWdtVal : 20; //Watch_dog_initial_value, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegHostIfSingleUcRegTssmWdtVal_u;

/*REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_RESET 0x4A80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTssmWdtReset : 1; //Enables_the_uC_to_reset_the_WDT, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegTssmWdtReset_u;

/*REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_NTID 0x5000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTaskschedNtid : 6; //Holds_tha_last_result_of_the_schedular_calculation_RWV_, reset value: 0x3F, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegTaskschedNtid_u;

/*REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ENABLE 0x5080 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTaskschedEnable : 1; //1_indicates_that_the_TaskSched_is_running, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegTaskschedEnable_u;

/*REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ALGO 0x5100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regTaskschedAlgo : 2; //Algo_selection, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegTaskschedAlgo_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_00 0x5800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady00 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady00_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_01 0x5804 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady01 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady01_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_02 0x5808 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady02 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady02_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_03 0x580C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady03 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady03_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_04 0x5810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady04 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady04_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_05 0x5814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady05 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady05_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_06 0x5818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady06 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady06_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_07 0x581C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady07 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady07_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_08 0x5820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady08 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady08_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_09 0x5824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady09 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady09_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_10 0x5828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady10 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady10_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_11 0x582C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady11 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady11_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_12 0x5830 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady12 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady12_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_13 0x5834 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady13 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady13_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_14 0x5838 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady14 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady14_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_15 0x583C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady15 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady15_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_16 0x5840 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady16 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady16_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_17 0x5844 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady17 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady17_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_18 0x5848 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady18 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady18_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_19 0x584C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady19 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady19_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_20 0x5850 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady20 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady20_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_21 0x5854 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady21 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady21_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_22 0x5858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady22 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady22_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_23 0x585C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady23 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady23_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_24 0x5860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady24 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady24_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_25 0x5864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady25 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady25_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_26 0x5868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady26 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady26_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_27 0x586C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady27 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady27_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_28 0x5870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady28 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady28_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_29 0x5874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady29 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady29_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_30 0x5878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady30 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady30_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_31 0x587C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrReady31 : 1; //Ready_bit_per_task_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrReady31_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_00 0x5880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior00 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior00_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_01 0x5884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior01 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior01_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_02 0x5888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior02 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior02_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_03 0x588C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior03 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior03_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_04 0x5890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior04 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior04_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_05 0x5894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior05 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior05_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_06 0x5898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior06 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior06_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_07 0x589C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior07 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior07_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_08 0x58A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior08 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior08_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_09 0x58A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior09 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior09_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_10 0x58A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior10 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior10_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_11 0x58AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior11 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior11_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_12 0x58B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior12 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior12_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_13 0x58B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior13 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior13_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_14 0x58B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior14 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior14_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_15 0x58BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior15 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior15_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_16 0x58C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior16 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior16_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_17 0x58C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior17 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior17_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_18 0x58C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior18 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior18_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_19 0x58CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior19 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior19_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_20 0x58D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior20 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior20_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_21 0x58D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior21 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior21_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_22 0x58D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior22 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior22_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_23 0x58DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior23 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior23_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_24 0x58E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior24 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior24_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_25 0x58E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior25 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior25_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_26 0x58E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior26 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior26_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_27 0x58EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior27 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior27_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_28 0x58F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior28 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior28_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_29 0x58F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior29 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior29_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_30 0x58F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior30 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior30_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_31 0x58FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrHiprior31 : 1; //Schedule_this_task_in_strict_high_priority_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegMtsrHiprior31_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_CTID 0x5900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrCtid : 5; //Current_Task_ID, reset value: 0x1F, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfSingleUcRegMtsrCtid_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_QUSTAMP 0x5980 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regMtsrQustamp : 16; //Queue_Stamp,_Get_the_counts_from_POPA, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfSingleUcRegMtsrQustamp_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_LOW 0x5A00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuSleepL : 32; //CPU_Sleep_Counter_Low_RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegMtsrCpuSleepCntLow_u;

/*REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_HIGH 0x5A04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuSleepH : 32; //CPU_Sleep_Counter_High_RWV_, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegMtsrCpuSleepCntHigh_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_START 0x6000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaStart : 7; //Start_Operation_RWV_, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegHostIfSingleUcRegPopaStart_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_00 0x6080 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap00 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap00_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_01 0x6084 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap01 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap01_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_02 0x6088 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap02 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap02_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_03 0x608C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap03 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap03_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_04 0x6090 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap04 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap04_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_05 0x6094 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap05 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap05_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_06 0x6098 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap06 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap06_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_07 0x609C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap07 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap07_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_08 0x60A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap08 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap08_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_09 0x60A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap09 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap09_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_10 0x60A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap10 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap10_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_11 0x60AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap11 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap11_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_12 0x60B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap12 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap12_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_13 0x60B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap13 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap13_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_14 0x60B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap14 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap14_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_15 0x60BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap15 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap15_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_16 0x60C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap16 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap16_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_17 0x60C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap17 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap17_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_18 0x60C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap18 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap18_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_19 0x60CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap19 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap19_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_20 0x60D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap20 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap20_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_21 0x60D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap21 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap21_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_22 0x60D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap22 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap22_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_23 0x60DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap23 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap23_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_24 0x60E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap24 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap24_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_25 0x60E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap25 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap25_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_26 0x60E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap26 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap26_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_27 0x60EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap27 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap27_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_28 0x60F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap28 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap28_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_29 0x60F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap29 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap29_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_30 0x60F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap30 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap30_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_31 0x60FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaMap31 : 6; //reg_extinsjob_job_offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaMap31_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_TID 0x6100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaTid : 6; //Task_ID, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegPopaTid_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_00 0x6180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType00 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType00_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_01 0x6184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType01 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType01_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_02 0x6188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType02 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType02_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_03 0x618C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType03 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType03_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_04 0x6190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType04 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType04_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_05 0x6194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType05 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType05_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_06 0x6198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType06 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType06_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_07 0x619C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType07 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType07_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_08 0x61A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType08 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType08_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_09 0x61A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType09 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType09_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_10 0x61A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType10 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType10_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_11 0x61AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType11 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType11_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_12 0x61B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType12 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType12_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_13 0x61B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType13 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType13_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_14 0x61B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType14 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType14_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_15 0x61BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType15 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType15_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_16 0x61C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType16 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType16_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_17 0x61C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType17 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType17_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_18 0x61C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType18 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType18_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_19 0x61CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType19 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType19_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_20 0x61D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType20 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType20_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_21 0x61D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType21 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType21_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_22 0x61D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType22 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType22_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_23 0x61DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType23 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType23_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_24 0x61E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType24 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType24_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_25 0x61E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType25 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType25_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_26 0x61E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType26 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType26_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_27 0x61EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType27 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType27_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_28 0x61F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType28 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType28_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_29 0x61F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType29 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType29_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_30 0x61F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType30 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType30_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_31 0x61FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaPopType31 : 1; //POP_Type, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcRegPopaPopType31_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_00 0x6200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaJobIdx00 : 8; //Read_Pointer_Job_Index, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHostIfSingleUcRegPopaJobIdx00_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_01 0x6204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaJobIdx01 : 8; //Read_Pointer_Job_Index, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHostIfSingleUcRegPopaJobIdx01_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_00 0x6300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaActiveLsb00 : 32; //POPA_Active, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcRegPopaActiveLsb00_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_01 0x6304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaActiveLsb01 : 32; //POPA_Active, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcRegPopaActiveLsb01_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_00 0x6400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaNewInQueue00 : 8; //New_In_Queue, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHostIfSingleUcRegPopaNewInQueue00_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_01 0x6404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaNewInQueue01 : 8; //New_In_Queue, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHostIfSingleUcRegPopaNewInQueue01_u;

/*REG_HOST_IF_SINGLE_UC_REG_POPA_LAST_JOB 0x6480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPopaLastJob : 16; //Last_Job_(RJOB/RJOB2), reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfSingleUcRegPopaLastJob_u;

/*REG_HOST_IF_SINGLE_UC_REG_PUSHE_PUTJOB 0x6800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPushePutjob : 16; //Used_by_task_to_push_a_job, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfSingleUcRegPushePutjob_u;

/*REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_00 0x6880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPusheJobDone00 : 2; //Job_done(k)(j)1_indicates_that_job_j_in_queue_k_is_done_and_can_be_forwarded_to_the_Reader, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegPusheJobDone00_u;

/*REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_01 0x6884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPusheJobDone01 : 2; //Job_done(k)(j)1_indicates_that_job_j_in_queue_k_is_done_and_can_be_forwarded_to_the_Reader, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegPusheJobDone01_u;

/*REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_02 0x6888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPusheJobDone02 : 2; //Job_done(k)(j)1_indicates_that_job_j_in_queue_k_is_done_and_can_be_forwarded_to_the_Reader, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegPusheJobDone02_u;

/*REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_03 0x688C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regPusheJobDone03 : 2; //Job_done(k)(j)1_indicates_that_job_j_in_queue_k_is_done_and_can_be_forwarded_to_the_Reader, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfSingleUcRegPusheJobDone03_u;

/*REG_HOST_IF_SINGLE_UC_REG_BGCOPY_DMA_REQ 0x7000 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bgcopyDmaeReq : 14; //Holds_, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegHostIfSingleUcRegBgcopyDmaReq_u;

/*REG_HOST_IF_SINGLE_UC_REG_DELAYA_START 0x7800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regDelayaStart : 32; //Trigger_a_delay_operation, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfSingleUcRegDelayaStart_u;

/*REG_HOST_IF_SINGLE_UC_REG_DELAYA_SCALE 0x7880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regDelayaScale : 5; //Scale_Factor, reset value: 0x5, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfSingleUcRegDelayaScale_u;

/*REG_HOST_IF_SINGLE_UC_REG_DELAYA_ABORT 0x7900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regDelayaAbort : 6; //Unblock, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegHostIfSingleUcRegDelayaAbort_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRVLD 0xE00C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errvld : 1; //1 indicates an error has been logged, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcSingleUcNocErrvld_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRCLR 0xE010 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errclr : 1; //Set to 1 to clear ErrVld. NOTE The written value is not stored in ErrVld., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcSingleUcNocErrclr_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0 0xE014 */
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
} RegHostIfSingleUcSingleUcNocErrlog0_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG1 0xE018 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 routeid : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcSingleUcNocErrlog1_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG3 0xE020 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addr : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcSingleUcNocErrlog3_u;

/*REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG4 0xE024 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrMsb : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfSingleUcSingleUcNocErrlog4_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRVLD 0xE40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errvld : 1; //1 indicates an error has been logged, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcDmiNocErrvld_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRCLR 0xE410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errclr : 1; //Set to 1 to clear ErrVld. NOTE The written value is not stored in ErrVld., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfSingleUcDmiNocErrclr_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0 0xE414 */
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
} RegHostIfSingleUcDmiNocErrlog0_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG1 0xE418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 routeid : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcDmiNocErrlog1_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG3 0xE420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addr : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfSingleUcDmiNocErrlog3_u;

/*REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG4 0xE424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrMsb : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHostIfSingleUcDmiNocErrlog4_u;

//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_00 0x4000 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_00_FIELD_REG_BOOT_RESET_VECTOR_00_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_01 0x4004 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_01_FIELD_REG_BOOT_RESET_VECTOR_01_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_02 0x4008 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_02_FIELD_REG_BOOT_RESET_VECTOR_02_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_03 0x400C */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_03_FIELD_REG_BOOT_RESET_VECTOR_03_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_04 0x4010 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_04_FIELD_REG_BOOT_RESET_VECTOR_04_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_05 0x4014 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_05_FIELD_REG_BOOT_RESET_VECTOR_05_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_06 0x4018 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_06_FIELD_REG_BOOT_RESET_VECTOR_06_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_07 0x401C */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_07_FIELD_REG_BOOT_RESET_VECTOR_07_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_08 0x4020 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_08_FIELD_REG_BOOT_RESET_VECTOR_08_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_09 0x4024 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_09_FIELD_REG_BOOT_RESET_VECTOR_09_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_10 0x4028 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_10_FIELD_REG_BOOT_RESET_VECTOR_10_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_11 0x402C */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_11_FIELD_REG_BOOT_RESET_VECTOR_11_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_12 0x4030 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_12_FIELD_REG_BOOT_RESET_VECTOR_12_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_13 0x4034 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_13_FIELD_REG_BOOT_RESET_VECTOR_13_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_14 0x4038 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_14_FIELD_REG_BOOT_RESET_VECTOR_14_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_15 0x403C */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_15_FIELD_REG_BOOT_RESET_VECTOR_15_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_16 0x4040 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_16_FIELD_REG_BOOT_RESET_VECTOR_16_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_17 0x4044 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_17_FIELD_REG_BOOT_RESET_VECTOR_17_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_18 0x4048 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_18_FIELD_REG_BOOT_RESET_VECTOR_18_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_19 0x404C */
#define HOST_IF_SINGLE_UC_REG_BOOT_RESET_VECTOR_19_FIELD_REG_BOOT_RESET_VECTOR_19_MASK                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_CORE_RUN_HALT 0x4080 */
#define HOST_IF_SINGLE_UC_REG_BOOT_CORE_RUN_HALT_FIELD_REG_BOOT_CORE_RUN_HALT_MASK                                0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_MY_CID 0x4100 */
#define HOST_IF_SINGLE_UC_REG_BOOT_MY_CID_FIELD_REG_BOOT_MY_CID_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_00 0x4180 */
#define HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_00_FIELD_REG_BOOT_GP_REG_00_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_01 0x4184 */
#define HOST_IF_SINGLE_UC_REG_BOOT_GP_REG_01_FIELD_REG_BOOT_GP_REG_01_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_00 0x4200 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_00_FIELD_REG_BOOT_SW_REG_00_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_01 0x4204 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_01_FIELD_REG_BOOT_SW_REG_01_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_02 0x4208 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_02_FIELD_REG_BOOT_SW_REG_02_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_03 0x420C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_03_FIELD_REG_BOOT_SW_REG_03_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_04 0x4210 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_04_FIELD_REG_BOOT_SW_REG_04_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_05 0x4214 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_05_FIELD_REG_BOOT_SW_REG_05_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_06 0x4218 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_06_FIELD_REG_BOOT_SW_REG_06_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_07 0x421C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_07_FIELD_REG_BOOT_SW_REG_07_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_08 0x4220 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_08_FIELD_REG_BOOT_SW_REG_08_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_09 0x4224 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_09_FIELD_REG_BOOT_SW_REG_09_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_10 0x4228 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_10_FIELD_REG_BOOT_SW_REG_10_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_11 0x422C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_11_FIELD_REG_BOOT_SW_REG_11_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_12 0x4230 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_12_FIELD_REG_BOOT_SW_REG_12_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_13 0x4234 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_13_FIELD_REG_BOOT_SW_REG_13_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_14 0x4238 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_14_FIELD_REG_BOOT_SW_REG_14_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_15 0x423C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_15_FIELD_REG_BOOT_SW_REG_15_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_16 0x4240 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_16_FIELD_REG_BOOT_SW_REG_16_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_17 0x4244 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_17_FIELD_REG_BOOT_SW_REG_17_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_18 0x4248 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_18_FIELD_REG_BOOT_SW_REG_18_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_19 0x424C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_19_FIELD_REG_BOOT_SW_REG_19_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_20 0x4250 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_20_FIELD_REG_BOOT_SW_REG_20_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_21 0x4254 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_21_FIELD_REG_BOOT_SW_REG_21_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_22 0x4258 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_22_FIELD_REG_BOOT_SW_REG_22_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_23 0x425C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_23_FIELD_REG_BOOT_SW_REG_23_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_24 0x4260 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_24_FIELD_REG_BOOT_SW_REG_24_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_25 0x4264 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_25_FIELD_REG_BOOT_SW_REG_25_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_26 0x4268 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_26_FIELD_REG_BOOT_SW_REG_26_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_27 0x426C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_27_FIELD_REG_BOOT_SW_REG_27_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_28 0x4270 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_28_FIELD_REG_BOOT_SW_REG_28_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_29 0x4274 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_29_FIELD_REG_BOOT_SW_REG_29_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_30 0x4278 */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_30_FIELD_REG_BOOT_SW_REG_30_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_31 0x427C */
#define HOST_IF_SINGLE_UC_REG_BOOT_SW_REG_31_FIELD_REG_BOOT_SW_REG_31_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_RUN_HALT_ACK 0x4280 */
#define HOST_IF_SINGLE_UC_REG_BOOT_RUN_HALT_ACK_FIELD_REG_BOOT_RUN_HALT_ACK_MASK                                  0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_00 0x4400 */
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_00_FIELD_CPU_AP_PARAM_00_MASK                                     0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_01 0x4404 */
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_01_FIELD_CPU_AP_PARAM_01_MASK                                     0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_00 0x4480 */
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_00_FIELD_CPU_AP_PARAM_WRITE_00_MASK                          0x00000001
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_00_FIELD_CPU_AP_PARAM_READ_00_MASK                           0x00000002
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_01 0x4484 */
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_01_FIELD_CPU_AP_PARAM_WRITE_01_MASK                          0x00000001
#define HOST_IF_SINGLE_UC_REG_BOOT_CPU_AP_PARAM_CTRL_01_FIELD_CPU_AP_PARAM_READ_01_MASK                           0x00000002
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL 0x4500 */
#define HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL_FIELD_REG_BOOT_PAGE_DCCM_MASK                                        0x00000003
#define HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL_FIELD_REG_BOOT_PAGE_ICCM_MASK                                        0x0000000C
#define HOST_IF_SINGLE_UC_REG_BOOT_PAGE_CTRL_FIELD_REG_BOOT_PAGE_ENGN_MASK                                        0x00000010
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_NEXT 0x4800 */
#define HOST_IF_SINGLE_UC_REG_TSSM_NEXT_FIELD_REG_TSSM_NEXT_MASK                                                  0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_PREV 0x4880 */
#define HOST_IF_SINGLE_UC_REG_TSSM_PREV_FIELD_REG_TSSM_PREV_MASK                                                  0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_SLEEP_EN 0x4900 */
#define HOST_IF_SINGLE_UC_REG_TSSM_SLEEP_EN_FIELD_REG_TSSM_SLEEP_EN_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_START_SM 0x4980 */
#define HOST_IF_SINGLE_UC_REG_TSSM_START_SM_FIELD_REG_TSSM_START_SM_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_VAL 0x4A00 */
#define HOST_IF_SINGLE_UC_REG_TSSM_WDT_VAL_FIELD_REG_TSSM_WDT_VAL_MASK                                            0x000FFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TSSM_WDT_RESET 0x4A80 */
#define HOST_IF_SINGLE_UC_REG_TSSM_WDT_RESET_FIELD_REG_TSSM_WDT_RESET_MASK                                        0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_NTID 0x5000 */
#define HOST_IF_SINGLE_UC_REG_TASKSCHED_NTID_FIELD_REG_TASKSCHED_NTID_MASK                                        0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ENABLE 0x5080 */
#define HOST_IF_SINGLE_UC_REG_TASKSCHED_ENABLE_FIELD_REG_TASKSCHED_ENABLE_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_TASKSCHED_ALGO 0x5100 */
#define HOST_IF_SINGLE_UC_REG_TASKSCHED_ALGO_FIELD_REG_TASKSCHED_ALGO_MASK                                        0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_00 0x5800 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_00_FIELD_REG_MTSR_READY_00_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_01 0x5804 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_01_FIELD_REG_MTSR_READY_01_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_02 0x5808 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_02_FIELD_REG_MTSR_READY_02_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_03 0x580C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_03_FIELD_REG_MTSR_READY_03_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_04 0x5810 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_04_FIELD_REG_MTSR_READY_04_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_05 0x5814 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_05_FIELD_REG_MTSR_READY_05_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_06 0x5818 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_06_FIELD_REG_MTSR_READY_06_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_07 0x581C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_07_FIELD_REG_MTSR_READY_07_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_08 0x5820 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_08_FIELD_REG_MTSR_READY_08_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_09 0x5824 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_09_FIELD_REG_MTSR_READY_09_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_10 0x5828 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_10_FIELD_REG_MTSR_READY_10_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_11 0x582C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_11_FIELD_REG_MTSR_READY_11_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_12 0x5830 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_12_FIELD_REG_MTSR_READY_12_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_13 0x5834 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_13_FIELD_REG_MTSR_READY_13_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_14 0x5838 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_14_FIELD_REG_MTSR_READY_14_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_15 0x583C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_15_FIELD_REG_MTSR_READY_15_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_16 0x5840 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_16_FIELD_REG_MTSR_READY_16_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_17 0x5844 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_17_FIELD_REG_MTSR_READY_17_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_18 0x5848 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_18_FIELD_REG_MTSR_READY_18_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_19 0x584C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_19_FIELD_REG_MTSR_READY_19_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_20 0x5850 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_20_FIELD_REG_MTSR_READY_20_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_21 0x5854 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_21_FIELD_REG_MTSR_READY_21_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_22 0x5858 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_22_FIELD_REG_MTSR_READY_22_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_23 0x585C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_23_FIELD_REG_MTSR_READY_23_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_24 0x5860 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_24_FIELD_REG_MTSR_READY_24_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_25 0x5864 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_25_FIELD_REG_MTSR_READY_25_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_26 0x5868 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_26_FIELD_REG_MTSR_READY_26_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_27 0x586C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_27_FIELD_REG_MTSR_READY_27_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_28 0x5870 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_28_FIELD_REG_MTSR_READY_28_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_29 0x5874 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_29_FIELD_REG_MTSR_READY_29_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_30 0x5878 */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_30_FIELD_REG_MTSR_READY_30_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_READY_31 0x587C */
#define HOST_IF_SINGLE_UC_REG_MTSR_READY_31_FIELD_REG_MTSR_READY_31_MASK                                          0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_00 0x5880 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_00_FIELD_REG_MTSR_HIPRIOR_00_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_01 0x5884 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_01_FIELD_REG_MTSR_HIPRIOR_01_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_02 0x5888 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_02_FIELD_REG_MTSR_HIPRIOR_02_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_03 0x588C */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_03_FIELD_REG_MTSR_HIPRIOR_03_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_04 0x5890 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_04_FIELD_REG_MTSR_HIPRIOR_04_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_05 0x5894 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_05_FIELD_REG_MTSR_HIPRIOR_05_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_06 0x5898 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_06_FIELD_REG_MTSR_HIPRIOR_06_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_07 0x589C */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_07_FIELD_REG_MTSR_HIPRIOR_07_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_08 0x58A0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_08_FIELD_REG_MTSR_HIPRIOR_08_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_09 0x58A4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_09_FIELD_REG_MTSR_HIPRIOR_09_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_10 0x58A8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_10_FIELD_REG_MTSR_HIPRIOR_10_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_11 0x58AC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_11_FIELD_REG_MTSR_HIPRIOR_11_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_12 0x58B0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_12_FIELD_REG_MTSR_HIPRIOR_12_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_13 0x58B4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_13_FIELD_REG_MTSR_HIPRIOR_13_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_14 0x58B8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_14_FIELD_REG_MTSR_HIPRIOR_14_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_15 0x58BC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_15_FIELD_REG_MTSR_HIPRIOR_15_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_16 0x58C0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_16_FIELD_REG_MTSR_HIPRIOR_16_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_17 0x58C4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_17_FIELD_REG_MTSR_HIPRIOR_17_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_18 0x58C8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_18_FIELD_REG_MTSR_HIPRIOR_18_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_19 0x58CC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_19_FIELD_REG_MTSR_HIPRIOR_19_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_20 0x58D0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_20_FIELD_REG_MTSR_HIPRIOR_20_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_21 0x58D4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_21_FIELD_REG_MTSR_HIPRIOR_21_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_22 0x58D8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_22_FIELD_REG_MTSR_HIPRIOR_22_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_23 0x58DC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_23_FIELD_REG_MTSR_HIPRIOR_23_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_24 0x58E0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_24_FIELD_REG_MTSR_HIPRIOR_24_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_25 0x58E4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_25_FIELD_REG_MTSR_HIPRIOR_25_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_26 0x58E8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_26_FIELD_REG_MTSR_HIPRIOR_26_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_27 0x58EC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_27_FIELD_REG_MTSR_HIPRIOR_27_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_28 0x58F0 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_28_FIELD_REG_MTSR_HIPRIOR_28_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_29 0x58F4 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_29_FIELD_REG_MTSR_HIPRIOR_29_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_30 0x58F8 */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_30_FIELD_REG_MTSR_HIPRIOR_30_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_31 0x58FC */
#define HOST_IF_SINGLE_UC_REG_MTSR_HIPRIOR_31_FIELD_REG_MTSR_HIPRIOR_31_MASK                                      0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_CTID 0x5900 */
#define HOST_IF_SINGLE_UC_REG_MTSR_CTID_FIELD_REG_MTSR_CTID_MASK                                                  0x0000001F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_QUSTAMP 0x5980 */
#define HOST_IF_SINGLE_UC_REG_MTSR_QUSTAMP_FIELD_REG_MTSR_QUSTAMP_MASK                                            0x0000FFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_LOW 0x5A00 */
#define HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_LOW_FIELD_CPU_SLEEP_L_MASK                                       0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_HIGH 0x5A04 */
#define HOST_IF_SINGLE_UC_REG_MTSR_CPU_SLEEP_CNT_HIGH_FIELD_CPU_SLEEP_H_MASK                                      0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_START 0x6000 */
#define HOST_IF_SINGLE_UC_REG_POPA_START_FIELD_REG_POPA_START_MASK                                                0x0000007F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_00 0x6080 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_00_FIELD_REG_POPA_MAP_00_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_01 0x6084 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_01_FIELD_REG_POPA_MAP_01_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_02 0x6088 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_02_FIELD_REG_POPA_MAP_02_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_03 0x608C */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_03_FIELD_REG_POPA_MAP_03_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_04 0x6090 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_04_FIELD_REG_POPA_MAP_04_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_05 0x6094 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_05_FIELD_REG_POPA_MAP_05_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_06 0x6098 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_06_FIELD_REG_POPA_MAP_06_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_07 0x609C */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_07_FIELD_REG_POPA_MAP_07_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_08 0x60A0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_08_FIELD_REG_POPA_MAP_08_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_09 0x60A4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_09_FIELD_REG_POPA_MAP_09_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_10 0x60A8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_10_FIELD_REG_POPA_MAP_10_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_11 0x60AC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_11_FIELD_REG_POPA_MAP_11_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_12 0x60B0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_12_FIELD_REG_POPA_MAP_12_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_13 0x60B4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_13_FIELD_REG_POPA_MAP_13_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_14 0x60B8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_14_FIELD_REG_POPA_MAP_14_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_15 0x60BC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_15_FIELD_REG_POPA_MAP_15_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_16 0x60C0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_16_FIELD_REG_POPA_MAP_16_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_17 0x60C4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_17_FIELD_REG_POPA_MAP_17_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_18 0x60C8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_18_FIELD_REG_POPA_MAP_18_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_19 0x60CC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_19_FIELD_REG_POPA_MAP_19_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_20 0x60D0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_20_FIELD_REG_POPA_MAP_20_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_21 0x60D4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_21_FIELD_REG_POPA_MAP_21_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_22 0x60D8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_22_FIELD_REG_POPA_MAP_22_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_23 0x60DC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_23_FIELD_REG_POPA_MAP_23_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_24 0x60E0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_24_FIELD_REG_POPA_MAP_24_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_25 0x60E4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_25_FIELD_REG_POPA_MAP_25_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_26 0x60E8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_26_FIELD_REG_POPA_MAP_26_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_27 0x60EC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_27_FIELD_REG_POPA_MAP_27_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_28 0x60F0 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_28_FIELD_REG_POPA_MAP_28_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_29 0x60F4 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_29_FIELD_REG_POPA_MAP_29_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_30 0x60F8 */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_30_FIELD_REG_POPA_MAP_30_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_MAP_31 0x60FC */
#define HOST_IF_SINGLE_UC_REG_POPA_MAP_31_FIELD_REG_POPA_MAP_31_MASK                                              0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_TID 0x6100 */
#define HOST_IF_SINGLE_UC_REG_POPA_TID_FIELD_REG_POPA_TID_MASK                                                    0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_00 0x6180 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_00_FIELD_REG_POPA_POP_TYPE_00_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_01 0x6184 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_01_FIELD_REG_POPA_POP_TYPE_01_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_02 0x6188 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_02_FIELD_REG_POPA_POP_TYPE_02_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_03 0x618C */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_03_FIELD_REG_POPA_POP_TYPE_03_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_04 0x6190 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_04_FIELD_REG_POPA_POP_TYPE_04_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_05 0x6194 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_05_FIELD_REG_POPA_POP_TYPE_05_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_06 0x6198 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_06_FIELD_REG_POPA_POP_TYPE_06_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_07 0x619C */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_07_FIELD_REG_POPA_POP_TYPE_07_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_08 0x61A0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_08_FIELD_REG_POPA_POP_TYPE_08_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_09 0x61A4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_09_FIELD_REG_POPA_POP_TYPE_09_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_10 0x61A8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_10_FIELD_REG_POPA_POP_TYPE_10_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_11 0x61AC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_11_FIELD_REG_POPA_POP_TYPE_11_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_12 0x61B0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_12_FIELD_REG_POPA_POP_TYPE_12_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_13 0x61B4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_13_FIELD_REG_POPA_POP_TYPE_13_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_14 0x61B8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_14_FIELD_REG_POPA_POP_TYPE_14_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_15 0x61BC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_15_FIELD_REG_POPA_POP_TYPE_15_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_16 0x61C0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_16_FIELD_REG_POPA_POP_TYPE_16_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_17 0x61C4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_17_FIELD_REG_POPA_POP_TYPE_17_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_18 0x61C8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_18_FIELD_REG_POPA_POP_TYPE_18_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_19 0x61CC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_19_FIELD_REG_POPA_POP_TYPE_19_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_20 0x61D0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_20_FIELD_REG_POPA_POP_TYPE_20_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_21 0x61D4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_21_FIELD_REG_POPA_POP_TYPE_21_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_22 0x61D8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_22_FIELD_REG_POPA_POP_TYPE_22_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_23 0x61DC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_23_FIELD_REG_POPA_POP_TYPE_23_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_24 0x61E0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_24_FIELD_REG_POPA_POP_TYPE_24_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_25 0x61E4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_25_FIELD_REG_POPA_POP_TYPE_25_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_26 0x61E8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_26_FIELD_REG_POPA_POP_TYPE_26_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_27 0x61EC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_27_FIELD_REG_POPA_POP_TYPE_27_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_28 0x61F0 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_28_FIELD_REG_POPA_POP_TYPE_28_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_29 0x61F4 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_29_FIELD_REG_POPA_POP_TYPE_29_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_30 0x61F8 */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_30_FIELD_REG_POPA_POP_TYPE_30_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_31 0x61FC */
#define HOST_IF_SINGLE_UC_REG_POPA_POP_TYPE_31_FIELD_REG_POPA_POP_TYPE_31_MASK                                    0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_00 0x6200 */
#define HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_00_FIELD_REG_POPA_JOB_IDX_00_MASK                                      0x000000FF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_01 0x6204 */
#define HOST_IF_SINGLE_UC_REG_POPA_JOB_IDX_01_FIELD_REG_POPA_JOB_IDX_01_MASK                                      0x000000FF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_00 0x6300 */
#define HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_00_FIELD_REG_POPA_ACTIVE_LSB_00_MASK                                0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_01 0x6304 */
#define HOST_IF_SINGLE_UC_REG_POPA_ACTIVE_LSB_01_FIELD_REG_POPA_ACTIVE_LSB_01_MASK                                0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_00 0x6400 */
#define HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_00_FIELD_REG_POPA_NEW_IN_QUEUE_00_MASK                            0x000000FF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_01 0x6404 */
#define HOST_IF_SINGLE_UC_REG_POPA_NEW_IN_QUEUE_01_FIELD_REG_POPA_NEW_IN_QUEUE_01_MASK                            0x000000FF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_POPA_LAST_JOB 0x6480 */
#define HOST_IF_SINGLE_UC_REG_POPA_LAST_JOB_FIELD_REG_POPA_LAST_JOB_MASK                                          0x0000FFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_PUSHE_PUTJOB 0x6800 */
#define HOST_IF_SINGLE_UC_REG_PUSHE_PUTJOB_FIELD_REG_PUSHE_PUTJOB_MASK                                            0x0000FFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_00 0x6880 */
#define HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_00_FIELD_REG_PUSHE_JOB_DONE_00_MASK                                  0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_01 0x6884 */
#define HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_01_FIELD_REG_PUSHE_JOB_DONE_01_MASK                                  0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_02 0x6888 */
#define HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_02_FIELD_REG_PUSHE_JOB_DONE_02_MASK                                  0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_03 0x688C */
#define HOST_IF_SINGLE_UC_REG_PUSHE_JOB_DONE_03_FIELD_REG_PUSHE_JOB_DONE_03_MASK                                  0x00000003
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_BGCOPY_DMA_REQ 0x7000 */
#define HOST_IF_SINGLE_UC_REG_BGCOPY_DMA_REQ_FIELD_BGCOPY_DMAE_REQ_MASK                                           0x00003FFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_DELAYA_START 0x7800 */
#define HOST_IF_SINGLE_UC_REG_DELAYA_START_FIELD_REG_DELAYA_START_MASK                                            0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_DELAYA_SCALE 0x7880 */
#define HOST_IF_SINGLE_UC_REG_DELAYA_SCALE_FIELD_REG_DELAYA_SCALE_MASK                                            0x0000001F
//========================================
/* REG_HOST_IF_SINGLE_UC_REG_DELAYA_ABORT 0x7900 */
#define HOST_IF_SINGLE_UC_REG_DELAYA_ABORT_FIELD_REG_DELAYA_ABORT_MASK                                            0x0000003F
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRVLD 0xE00C */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRVLD_FIELD_ERRVLD_MASK                                                  0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRCLR 0xE010 */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRCLR_FIELD_ERRCLR_MASK                                                  0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0 0xE014 */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0_FIELD_LOCK_MASK                                                   0x00000001
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0_FIELD_OPC_MASK                                                    0x0000001E
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0_FIELD_ERRCODE_MASK                                                0x00000700
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0_FIELD_LEN1_MASK                                                   0x0FFF0000
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG0_FIELD_FORMAT_MASK                                                 0x80000000
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG1 0xE018 */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG1_FIELD_ROUTEID_MASK                                                0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG3 0xE020 */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG3_FIELD_ADDR_MASK                                                   0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG4 0xE024 */
#define HOST_IF_SINGLE_UC_SINGLE_UC_NOC_ERRLOG4_FIELD_ADDR_MSB_MASK                                               0x0000FFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRVLD 0xE40C */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRVLD_FIELD_ERRVLD_MASK                                                        0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRCLR 0xE410 */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRCLR_FIELD_ERRCLR_MASK                                                        0x00000001
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0 0xE414 */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0_FIELD_LOCK_MASK                                                         0x00000001
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0_FIELD_OPC_MASK                                                          0x0000001E
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0_FIELD_ERRCODE_MASK                                                      0x00000700
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0_FIELD_LEN1_MASK                                                         0x0FFF0000
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG0_FIELD_FORMAT_MASK                                                       0x80000000
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG1 0xE418 */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG1_FIELD_ROUTEID_MASK                                                      0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG3 0xE420 */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG3_FIELD_ADDR_MASK                                                         0xFFFFFFFF
//========================================
/* REG_HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG4 0xE424 */
#define HOST_IF_SINGLE_UC_DMI_NOC_ERRLOG4_FIELD_ADDR_MSB_MASK                                                     0x0000FFFF


#endif // _HOST_IF_SINGLE_UC_REGS_H_
