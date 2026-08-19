
/***********************************************************************************
File:				B01DmacWrapperRegs.h
Module:				b01DmacWrapper
SOC Revision:		latest
Generated at:       2022-05-04 10:56:57
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B01_DMAC_WRAPPER_REGS_H_
#define _B01_DMAC_WRAPPER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B01_DMAC_WRAPPER_BASE_ADDRESS                             MEMORY_MAP_UNIT_38_BASE_ADDRESS
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x600)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x604)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x608)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x60C)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x610)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x614)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x618)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET0         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x61C)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x620)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x624)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x628)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x62C)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x630)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x634)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x638)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET1         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x63C)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x640)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x644)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x648)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x64C)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x650)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x654)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x658)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET2         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x65C)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x660)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x664)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x668)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x66C)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x670)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x674)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x678)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET3         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x67C)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x680)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x684)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x688)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x68C)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x690)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x694)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x698)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET4         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x69C)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6A0)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6A4)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6A8)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6AC)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6B0)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6B4)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6B8)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET5         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6BC)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6C0)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6C4)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6C8)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6CC)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6D0)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6D4)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6D8)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET6         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6DC)
#define	REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6E0)
#define	REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6E4)
#define	REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6E8)
#define	REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6EC)
#define	REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6F0)
#define	REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6F4)
#define	REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6F8)
#define	REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET7         (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x6FC)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOBS                          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x700)
#define	REG_B01_DMAC_WRAPPER_DMA1_JOBS                          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x704)
#define	REG_B01_DMAC_WRAPPER_DMA2_JOBS                          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x708)
#define	REG_B01_DMAC_WRAPPER_DDR_ALIGN                          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x70C)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_INFORMATION               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x710)
#define	REG_B01_DMAC_WRAPPER_DMA1_JOB_INFORMATION               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x714)
#define	REG_B01_DMAC_WRAPPER_DMA2_JOB_INFORMATION               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x718)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG0     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x71C)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG1     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x720)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG2     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x724)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG3     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x728)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG4     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x72C)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG5     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x730)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG6     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x734)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG7     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x738)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG8     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x73C)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG9     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x740)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_HALT_31_0_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x744)
#define	REG_B01_DMAC_WRAPPER_DMA0_JOB_HALT_36_32_REG            (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x748)
#define	REG_B01_DMAC_WRAPPER_DMA0_USER_JOBS_SET                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x74C)
#define	REG_B01_DMAC_WRAPPER_DMA_ABORT_REG                      (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x80C)
#define	REG_B01_DMAC_WRAPPER_DMA1_MISC_REG                      (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x810)
#define	REG_B01_DMAC_WRAPPER_DMA_JOB_PUSH_WHILE_FULL_CLR_IND    (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x814)
#define	REG_B01_DMAC_WRAPPER_DMA1_JUNK_BASE_ADDR_REG            (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x818)
#define	REG_B01_DMAC_WRAPPER_DMA1_STAT1_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x820)
#define	REG_B01_DMAC_WRAPPER_TX1_SENDER_ALIGNER_EN              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x824)
#define	REG_B01_DMAC_WRAPPER_DMA2_STAT1_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x828)
#define	REG_B01_DMAC_WRAPPER_DMA0_MISC0_REG                     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x910)
#define	REG_B01_DMAC_WRAPPER_DMA0_MISC1_REG                     (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x914)
#define	REG_B01_DMAC_WRAPPER_PUSH_FIFO_FULL_CTL                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x918)
#define	REG_B01_DMAC_WRAPPER_DMA0_JUNK_BASE_ADDR_REG            (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x920)
#define	REG_B01_DMAC_WRAPPER_DMA0_STAT1_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x928)
#define	REG_B01_DMAC_WRAPPER_TX0_SENDER_ALIGNER_EN              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x92C)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_RECOVERY_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x930)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_RECOVERY_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x934)
#define	REG_B01_DMAC_WRAPPER_DMA0_STAT2_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0x938)
#define	REG_B01_DMAC_WRAPPER_AGENT_0_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA00)
#define	REG_B01_DMAC_WRAPPER_AGENT_0_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA04)
#define	REG_B01_DMAC_WRAPPER_AGENT_0_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA08)
#define	REG_B01_DMAC_WRAPPER_AGENT_1_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA10)
#define	REG_B01_DMAC_WRAPPER_AGENT_1_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA14)
#define	REG_B01_DMAC_WRAPPER_AGENT_1_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA18)
#define	REG_B01_DMAC_WRAPPER_AGENT_2_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA20)
#define	REG_B01_DMAC_WRAPPER_AGENT_2_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA24)
#define	REG_B01_DMAC_WRAPPER_AGENT_2_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA28)
#define	REG_B01_DMAC_WRAPPER_AGENT_3_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA30)
#define	REG_B01_DMAC_WRAPPER_AGENT_3_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA34)
#define	REG_B01_DMAC_WRAPPER_AGENT_3_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA38)
#define	REG_B01_DMAC_WRAPPER_AGENT_4_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA40)
#define	REG_B01_DMAC_WRAPPER_AGENT_4_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA44)
#define	REG_B01_DMAC_WRAPPER_AGENT_4_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA48)
#define	REG_B01_DMAC_WRAPPER_AGENT_5_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA50)
#define	REG_B01_DMAC_WRAPPER_AGENT_5_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA54)
#define	REG_B01_DMAC_WRAPPER_AGENT_5_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA58)
#define	REG_B01_DMAC_WRAPPER_AGENT_6_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA60)
#define	REG_B01_DMAC_WRAPPER_AGENT_6_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA64)
#define	REG_B01_DMAC_WRAPPER_AGENT_6_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA68)
#define	REG_B01_DMAC_WRAPPER_AGENT_7_INT_MASK_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA70)
#define	REG_B01_DMAC_WRAPPER_AGENT_7_PEND_INT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA74)
#define	REG_B01_DMAC_WRAPPER_AGENT_7_INT_CLEAR_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xA78)
#define	REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB00)
#define	REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC_DEC          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB04)
#define	REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC_CLR          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB08)
#define	REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB10)
#define	REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC_DEC          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB14)
#define	REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC_CLR          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xB18)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC00)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_SRC_ADDR        (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC04)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_DEST_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC08)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_C_REG           (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC0C)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_MPDUP_REG       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC10)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC20)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_SRC_ADDR        (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC24)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_DEST_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC28)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_C_REG           (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC2C)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_MPDUP_REG       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC30)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC40)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_SRC_ADDR        (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC44)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_DEST_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC48)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_C_REG           (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC4C)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC60)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_SRC_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC64)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_DEST_ADDR      (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC68)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_C_REG          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC6C)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_SRC_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC80)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_DEST_ADDR      (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC84)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_C_REG          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC88)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_SRC_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC90)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_DEST_ADDR      (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC94)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_C_REG          (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xC98)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD00)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_SRC_ADDR        (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD04)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_DEST_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD08)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_C_REG           (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD0C)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_MPDUP_REG       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD10)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_CONFIG_REG                (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD20)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_SRC_ADDR        (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD24)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_DEST_ADDR       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD28)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_C_REG           (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD2C)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_MPDUP_REG       (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xD30)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF00)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF04)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH0_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF08)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF10)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF14)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH1_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF18)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF20)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF24)
#define	REG_B01_DMAC_WRAPPER_DMA1_CH2_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF28)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF30)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF34)
#define	REG_B01_DMAC_WRAPPER_DMA2_CH0_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF38)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF40)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF44)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH0_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF48)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_JOB_CNT_REG               (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF50)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_BATCH_CNT_REG             (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF54)
#define	REG_B01_DMAC_WRAPPER_DMA0_CH1_STAT_IND_REG              (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF58)
#define	REG_B01_DMAC_WRAPPER_FIFO0_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF60)
#define	REG_B01_DMAC_WRAPPER_FIFO1_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF64)
#define	REG_B01_DMAC_WRAPPER_FIFO2_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF68)
#define	REG_B01_DMAC_WRAPPER_FIFO3_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF6C)
#define	REG_B01_DMAC_WRAPPER_FIFO4_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF70)
#define	REG_B01_DMAC_WRAPPER_FIFO5_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF74)
#define	REG_B01_DMAC_WRAPPER_FIFO6_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF78)
#define	REG_B01_DMAC_WRAPPER_FIFO7_STAT_IND_REG                 (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xF7C)
#define	REG_B01_DMAC_WRAPPER_SPARE_32_BIT_REG                   (B01_DMAC_WRAPPER_BASE_ADDRESS + 0xFF0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET0 0x600 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser0 : 6; //TX set0   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser1 : 6; //TX set0   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser2 : 6; //TX set0   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser3 : 6; //TX set0   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser4 : 6; //TX set0   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET0 0x604 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser5 : 6; //TX set0   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser6 : 6; //TX set0   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser7 : 6; //TX set0   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser8 : 6; //TX set0   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser9 : 6; //TX set0   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET0 0x608 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser10 : 6; //TX set0   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser11 : 6; //TX set0   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser12 : 6; //TX set0   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser13 : 6; //TX set0   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser14 : 6; //TX set0   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET0 0x60C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser15 : 6; //TX set0   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser16 : 6; //TX set0   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser17 : 6; //TX set0   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser18 : 6; //TX set0   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser19 : 6; //TX set0   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET0 0x610 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser20 : 6; //TX set0   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser21 : 6; //TX set0   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser22 : 6; //TX set0   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser23 : 6; //TX set0   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser24 : 6; //TX set0   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET0 0x614 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser25 : 6; //TX set0   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser26 : 6; //TX set0   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser27 : 6; //TX set0   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser28 : 6; //TX set0   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser29 : 6; //TX set0   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET0 0x618 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser30 : 6; //TX set0   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser31 : 6; //TX set0   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser32 : 6; //TX set0   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser33 : 6; //TX set0   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser34 : 6; //TX set0   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET0 0x61C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet0MaxJobsUser35 : 6; //TX set0   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet0MaxJobsUser36 : 6; //TX set0   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet0_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET1 0x620 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser0 : 6; //TX set1   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser1 : 6; //TX set1   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser2 : 6; //TX set1   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser3 : 6; //TX set1   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser4 : 6; //TX set1   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET1 0x624 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser5 : 6; //TX set1   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser6 : 6; //TX set1   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser7 : 6; //TX set1   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser8 : 6; //TX set1   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser9 : 6; //TX set1   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET1 0x628 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser10 : 6; //TX set1   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser11 : 6; //TX set1   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser12 : 6; //TX set1   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser13 : 6; //TX set1   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser14 : 6; //TX set1   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET1 0x62C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser15 : 6; //TX set1   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser16 : 6; //TX set1   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser17 : 6; //TX set1   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser18 : 6; //TX set1   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser19 : 6; //TX set1   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET1 0x630 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser20 : 6; //TX set1   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser21 : 6; //TX set1   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser22 : 6; //TX set1   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser23 : 6; //TX set1   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser24 : 6; //TX set1   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET1 0x634 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser25 : 6; //TX set1   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser26 : 6; //TX set1   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser27 : 6; //TX set1   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser28 : 6; //TX set1   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser29 : 6; //TX set1   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET1 0x638 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser30 : 6; //TX set1   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser31 : 6; //TX set1   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser32 : 6; //TX set1   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser33 : 6; //TX set1   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser34 : 6; //TX set1   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET1 0x63C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet1MaxJobsUser35 : 6; //TX set1   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet1MaxJobsUser36 : 6; //TX set1   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet1_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET2 0x640 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser0 : 6; //TX set2   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser1 : 6; //TX set2   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser2 : 6; //TX set2   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser3 : 6; //TX set2   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser4 : 6; //TX set2   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET2 0x644 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser5 : 6; //TX set2   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser6 : 6; //TX set2   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser7 : 6; //TX set2   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser8 : 6; //TX set2   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser9 : 6; //TX set2   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET2 0x648 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser10 : 6; //TX set2   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser11 : 6; //TX set2   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser12 : 6; //TX set2   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser13 : 6; //TX set2   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser14 : 6; //TX set2   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET2 0x64C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser15 : 6; //TX set2   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser16 : 6; //TX set2   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser17 : 6; //TX set2   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser18 : 6; //TX set2   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser19 : 6; //TX set2   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET2 0x650 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser20 : 6; //TX set2   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser21 : 6; //TX set2   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser22 : 6; //TX set2   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser23 : 6; //TX set2   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser24 : 6; //TX set2   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET2 0x654 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser25 : 6; //TX set2   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser26 : 6; //TX set2   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser27 : 6; //TX set2   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser28 : 6; //TX set2   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser29 : 6; //TX set2   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET2 0x658 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser30 : 6; //TX set2   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser31 : 6; //TX set2   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser32 : 6; //TX set2   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser33 : 6; //TX set2   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser34 : 6; //TX set2   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET2 0x65C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet2MaxJobsUser35 : 6; //TX set2   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet2MaxJobsUser36 : 6; //TX set2   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet2_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET3 0x660 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser0 : 6; //TX set3   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser1 : 6; //TX set3   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser2 : 6; //TX set3   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser3 : 6; //TX set3   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser4 : 6; //TX set3   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET3 0x664 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser5 : 6; //TX set3   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser6 : 6; //TX set3   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser7 : 6; //TX set3   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser8 : 6; //TX set3   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser9 : 6; //TX set3   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET3 0x668 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser10 : 6; //TX set3   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser11 : 6; //TX set3   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser12 : 6; //TX set3   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser13 : 6; //TX set3   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser14 : 6; //TX set3   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET3 0x66C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser15 : 6; //TX set3   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser16 : 6; //TX set3   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser17 : 6; //TX set3   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser18 : 6; //TX set3   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser19 : 6; //TX set3   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET3 0x670 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser20 : 6; //TX set3   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser21 : 6; //TX set3   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser22 : 6; //TX set3   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser23 : 6; //TX set3   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser24 : 6; //TX set3   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET3 0x674 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser25 : 6; //TX set3   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser26 : 6; //TX set3   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser27 : 6; //TX set3   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser28 : 6; //TX set3   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser29 : 6; //TX set3   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET3 0x678 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser30 : 6; //TX set3   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser31 : 6; //TX set3   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser32 : 6; //TX set3   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser33 : 6; //TX set3   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser34 : 6; //TX set3   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET3 0x67C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet3MaxJobsUser35 : 6; //TX set3   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet3MaxJobsUser36 : 6; //TX set3   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet3_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET4 0x680 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser0 : 6; //TX set4   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser1 : 6; //TX set4   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser2 : 6; //TX set4   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser3 : 6; //TX set4   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser4 : 6; //TX set4   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET4 0x684 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser5 : 6; //TX set4   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser6 : 6; //TX set4   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser7 : 6; //TX set4   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser8 : 6; //TX set4   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser9 : 6; //TX set4   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET4 0x688 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser10 : 6; //TX set4   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser11 : 6; //TX set4   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser12 : 6; //TX set4   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser13 : 6; //TX set4   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser14 : 6; //TX set4   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET4 0x68C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser15 : 6; //TX set4   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser16 : 6; //TX set4   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser17 : 6; //TX set4   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser18 : 6; //TX set4   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser19 : 6; //TX set4   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET4 0x690 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser20 : 6; //TX set4   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser21 : 6; //TX set4   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser22 : 6; //TX set4   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser23 : 6; //TX set4   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser24 : 6; //TX set4   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET4 0x694 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser25 : 6; //TX set4   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser26 : 6; //TX set4   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser27 : 6; //TX set4   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser28 : 6; //TX set4   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser29 : 6; //TX set4   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET4 0x698 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser30 : 6; //TX set4   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser31 : 6; //TX set4   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser32 : 6; //TX set4   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser33 : 6; //TX set4   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser34 : 6; //TX set4   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET4 0x69C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet4MaxJobsUser35 : 6; //TX set4   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet4MaxJobsUser36 : 6; //TX set4   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet4_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET5 0x6A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser0 : 6; //TX set5   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser1 : 6; //TX set5   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser2 : 6; //TX set5   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser3 : 6; //TX set5   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser4 : 6; //TX set5   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET5 0x6A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser5 : 6; //TX set5   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser6 : 6; //TX set5   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser7 : 6; //TX set5   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser8 : 6; //TX set5   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser9 : 6; //TX set5   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET5 0x6A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser10 : 6; //TX set5   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser11 : 6; //TX set5   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser12 : 6; //TX set5   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser13 : 6; //TX set5   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser14 : 6; //TX set5   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET5 0x6AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser15 : 6; //TX set5   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser16 : 6; //TX set5   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser17 : 6; //TX set5   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser18 : 6; //TX set5   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser19 : 6; //TX set5   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET5 0x6B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser20 : 6; //TX set5   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser21 : 6; //TX set5   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser22 : 6; //TX set5   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser23 : 6; //TX set5   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser24 : 6; //TX set5   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET5 0x6B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser25 : 6; //TX set5   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser26 : 6; //TX set5   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser27 : 6; //TX set5   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser28 : 6; //TX set5   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser29 : 6; //TX set5   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET5 0x6B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser30 : 6; //TX set5   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser31 : 6; //TX set5   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser32 : 6; //TX set5   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser33 : 6; //TX set5   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser34 : 6; //TX set5   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET5 0x6BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet5MaxJobsUser35 : 6; //TX set5   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet5MaxJobsUser36 : 6; //TX set5   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet5_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET6 0x6C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser0 : 6; //TX set6   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser1 : 6; //TX set6   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser2 : 6; //TX set6   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser3 : 6; //TX set6   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser4 : 6; //TX set6   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET6 0x6C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser5 : 6; //TX set6   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser6 : 6; //TX set6   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser7 : 6; //TX set6   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser8 : 6; //TX set6   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser9 : 6; //TX set6   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET6 0x6C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser10 : 6; //TX set6   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser11 : 6; //TX set6   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser12 : 6; //TX set6   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser13 : 6; //TX set6   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser14 : 6; //TX set6   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET6 0x6CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser15 : 6; //TX set6   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser16 : 6; //TX set6   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser17 : 6; //TX set6   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser18 : 6; //TX set6   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser19 : 6; //TX set6   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET6 0x6D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser20 : 6; //TX set6   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser21 : 6; //TX set6   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser22 : 6; //TX set6   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser23 : 6; //TX set6   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser24 : 6; //TX set6   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET6 0x6D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser25 : 6; //TX set6   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser26 : 6; //TX set6   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser27 : 6; //TX set6   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser28 : 6; //TX set6   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser29 : 6; //TX set6   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET6 0x6D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser30 : 6; //TX set6   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser31 : 6; //TX set6   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser32 : 6; //TX set6   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser33 : 6; //TX set6   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser34 : 6; //TX set6   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET6 0x6DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet6MaxJobsUser35 : 6; //TX set6   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet6MaxJobsUser36 : 6; //TX set6   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet6_u;

/*REG_B01_DMAC_WRAPPER_REG0_TX_USER_MAX_JOBS_SET7 0x6E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser0 : 6; //TX set7   Max outstanding Jobs User_0, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser1 : 6; //TX set7   Max outstanding Jobs User_1, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser2 : 6; //TX set7   Max outstanding Jobs User_2, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser3 : 6; //TX set7   Max outstanding Jobs User_3, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser4 : 6; //TX set7   Max outstanding Jobs User_4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg0TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG1_TX_USER_MAX_JOBS_SET7 0x6E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser5 : 6; //TX set7   Max outstanding Jobs User_5, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser6 : 6; //TX set7   Max outstanding Jobs User_6, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser7 : 6; //TX set7   Max outstanding Jobs User_7, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser8 : 6; //TX set7   Max outstanding Jobs User_8, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser9 : 6; //TX set7   Max outstanding Jobs User_9, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg1TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG2_TX_USER_MAX_JOBS_SET7 0x6E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser10 : 6; //TX set7   Max outstanding Jobs User_10, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser11 : 6; //TX set7   Max outstanding Jobs User_11, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser12 : 6; //TX set7   Max outstanding Jobs User_12, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser13 : 6; //TX set7   Max outstanding Jobs User_13, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser14 : 6; //TX set7   Max outstanding Jobs User_14, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg2TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG3_TX_USER_MAX_JOBS_SET7 0x6EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser15 : 6; //TX set7   Max outstanding Jobs User_15, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser16 : 6; //TX set7   Max outstanding Jobs User_16, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser17 : 6; //TX set7   Max outstanding Jobs User_17, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser18 : 6; //TX set7   Max outstanding Jobs User_18, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser19 : 6; //TX set7   Max outstanding Jobs User_19, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg3TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG4_TX_USER_MAX_JOBS_SET7 0x6F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser20 : 6; //TX set7   Max outstanding Jobs User_20, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser21 : 6; //TX set7   Max outstanding Jobs User_21, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser22 : 6; //TX set7   Max outstanding Jobs User_22, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser23 : 6; //TX set7   Max outstanding Jobs User_23, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser24 : 6; //TX set7   Max outstanding Jobs User_24, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg4TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG5_TX_USER_MAX_JOBS_SET7 0x6F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser25 : 6; //TX set7   Max outstanding Jobs User_25, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser26 : 6; //TX set7   Max outstanding Jobs User_26, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser27 : 6; //TX set7   Max outstanding Jobs User_27, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser28 : 6; //TX set7   Max outstanding Jobs User_28, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser29 : 6; //TX set7   Max outstanding Jobs User_29, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg5TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG6_TX_USER_MAX_JOBS_SET7 0x6F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser30 : 6; //TX set7   Max outstanding Jobs User_30, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser31 : 6; //TX set7   Max outstanding Jobs User_31, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser32 : 6; //TX set7   Max outstanding Jobs User_32, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser33 : 6; //TX set7   Max outstanding Jobs User_33, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser34 : 6; //TX set7   Max outstanding Jobs User_34, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperReg6TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_REG7_TX_USER_MAX_JOBS_SET7 0x6FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSet7MaxJobsUser35 : 6; //TX set7   Max outstanding Jobs User_35, reset value: 0x0, access type: RW
		uint32 txSet7MaxJobsUser36 : 6; //TX set7   Max outstanding Jobs User_36, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB01DmacWrapperReg7TxUserMaxJobsSet7_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOBS 0x700 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0MaxDmaJobs : 6; //dma0 max jobs, reset value: 0x28, access type: RW
		uint32 reserved0 : 2;
		uint32 dma0MaxCh0Jobs : 6; //dma0 max ch0 jobs, reset value: 0x28, access type: RW
		uint32 reserved1 : 2;
		uint32 dma0MaxCh1Jobs : 6; //dma0 max ch1 jobs, reset value: 0x16, access type: RW
		uint32 reserved2 : 10;
	} bitFields;
} RegB01DmacWrapperDma0Jobs_u;

/*REG_B01_DMAC_WRAPPER_DMA1_JOBS 0x704 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1MaxDmaJobs : 6; //dma1 max dma jobs, reset value: 0x8, access type: RW
		uint32 reserved0 : 2;
		uint32 dma1MaxCh0Jobs : 6; //dma1 max_ch0 jobs, reset value: 0x8, access type: RW
		uint32 reserved1 : 2;
		uint32 dma1MaxCh1Jobs : 6; //dma1 max ch1 jobs, reset value: 0x8, access type: RW
		uint32 reserved2 : 10;
	} bitFields;
} RegB01DmacWrapperDma1Jobs_u;

/*REG_B01_DMAC_WRAPPER_DMA2_JOBS 0x708 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2MaxDmaJobs : 6; //dma2_max_dma_jobs, reset value: 0x4, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB01DmacWrapperDma2Jobs_u;

/*REG_B01_DMAC_WRAPPER_DDR_ALIGN 0x70C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0DdrLineSize : 3; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma0DdrAlignEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 dma1DdrLineSize : 3; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 dma1DdrAlignEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved3 : 19;
	} bitFields;
} RegB01DmacWrapperDdrAlign_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_INFORMATION 0x710 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0JobCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0JobCntCh0 : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0JobCntCh1 : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0MaxJobBp : 1; //no description, reset value: 0x0, access type: RO
		uint32 dma0MaxJobCh0Bp : 1; //no description, reset value: 0x0, access type: RO
		uint32 dma0MaxJobCh1Bp : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 5;
	} bitFields;
} RegB01DmacWrapperDma0JobInformation_u;

/*REG_B01_DMAC_WRAPPER_DMA1_JOB_INFORMATION 0x714 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1JobCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma1JobCntCh0 : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma1JobCntCh1 : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma1MaxJobBp : 1; //no description, reset value: 0x0, access type: RO
		uint32 dma1MaxJobCh0Bp : 1; //no description, reset value: 0x0, access type: RO
		uint32 dma1MaxJobCh1Bp : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 5;
	} bitFields;
} RegB01DmacWrapperDma1JobInformation_u;

/*REG_B01_DMAC_WRAPPER_DMA2_JOB_INFORMATION 0x718 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2JobCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma2JobCntCh0 : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma2MaxJobBp : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 7;
		uint32 dma2MaxJobCh0Bp : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 7;
	} bitFields;
} RegB01DmacWrapperDma2JobInformation_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG0 0x71C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User0Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User1Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User2Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User3Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg0_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG1 0x720 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User4Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User5Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User6Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User7Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg1_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG2 0x724 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User8Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User9Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User10Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User11Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg2_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG3 0x728 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User12Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User13Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User14Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User15Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg3_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG4 0x72C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User16Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User17Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User18Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User19Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg4_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG5 0x730 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User20Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User21Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User22Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User23Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg5_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG6 0x734 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User24Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User25Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User26Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User27Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg6_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG7 0x738 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User28Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User29Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User30Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User31Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg7_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG8 0x73C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User32Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0User33Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0User34Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 dma0User35Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg8_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_USER_INFOFMATION_REG9 0x740 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0User36Jobs : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB01DmacWrapperDma0JobUserInfofmationReg9_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_HALT_31_0_REG 0x744 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0UserHalt310 : 32; //user BP information 31-0, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperDma0JobHalt310Reg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JOB_HALT_36_32_REG 0x748 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0UserHalt3632 : 5; //user BP information 36-32, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB01DmacWrapperDma0JobHalt3632Reg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_USER_JOBS_SET 0x74C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 28;
		uint32 dma0MaxUserJobDisable : 1; //dma0 max Jobs Per user disable, reset value: 0x1, access type: RW
		uint32 dma0MaxUserSet : 3; //dma0 max user set select , reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0UserJobsSet_u;

/*REG_B01_DMAC_WRAPPER_DMA_ABORT_REG 0x80C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 fifoClearReq : 8; //fifo clear, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegB01DmacWrapperDmaAbortReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_MISC_REG 0x810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 dma1LastJobIdChSel : 3; //selects which channel is connected to the "last job id" interrupts, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
		uint32 cdbMode : 1; //cdb mode - 0 - 3x1/4x0 (36 users to band0 no/one user for band1), 1 - 2x2 (18 users for band0 18 users band1), reset value: 0x1, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB01DmacWrapperDma1MiscReg_u;

/*REG_B01_DMAC_WRAPPER_DMA_JOB_PUSH_WHILE_FULL_CLR_IND 0x814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaClearPushWhileFullInd : 8; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 24;
	} bitFields;
} RegB01DmacWrapperDmaJobPushWhileFullClrInd_u;

/*REG_B01_DMAC_WRAPPER_DMA1_JUNK_BASE_ADDR_REG 0x818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1JunkBaseAddrConf : 32; //32 bit base address of junk yard. , NOTE: need to configure carefully to avoid memory rundown, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1JunkBaseAddrReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_STAT1_IND_REG 0x820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Status1DmacInterrupts : 8; //status dmac interrupts, reset value: 0x0, access type: RO
		uint32 dma1Status1WrapLastJobIndicationsTog : 8; //status wrapper last job indications (toggling), reset value: 0x0, access type: RO
		uint32 dma1Status1JobFifoFullIndications : 2; //status job fifo full indications, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
		uint32 dma1Status1DmacAbortInterrupt : 1; //status dmac abort interrupt, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperDma1Stat1IndReg_u;

/*REG_B01_DMAC_WRAPPER_TX1_SENDER_ALIGNER_EN 0x824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx1SenderAlignerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperTx1SenderAlignerEn_u;

/*REG_B01_DMAC_WRAPPER_DMA2_STAT1_IND_REG 0x828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dma2Status1WrapLastJobIndicationsTog : 8; //status wrapper last job indications (toggling), reset value: 0x0, access type: RO
		uint32 dma2Status1JobFifoFullIndications : 4; //status job fifo full indications, reset value: 0x0, access type: RO
		uint32 reserved1 : 11;
		uint32 dma2Status1DmacAbortInterrupt : 1; //status dmac abort interrupt, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperDma2Stat1IndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_MISC0_REG 0x910 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0MonHdrOffset0 : 6; //header monitor: offset to last header byte, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 dma0MonChSel0 : 3; //header monitor: selection of monitorred channel, reset value: 0x0, access type: RW
		uint32 dma0MonHdrEn0 : 1; //header monitor enable, reset value: 0x0, access type: RW
		uint32 dma0LastJobIdChSel : 3; //selects which channel is connected to the "last job id" interrupts, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 dma0Tx0BufferFullMargin : 10; //Tx0 buffer full margin, reset value: 0x80, access type: RW
		uint32 reserved2 : 6;
	} bitFields;
} RegB01DmacWrapperDma0Misc0Reg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_MISC1_REG 0x914 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0MonHdrOffset1 : 6; //header monitor: offset to last header byte, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 dma0MonChSel1 : 3; //header monitor: selection of monitorred channel, reset value: 0x0, access type: RW
		uint32 dma0MonHdrEn1 : 1; //header monitor enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
		uint32 dma0Tx1BufferFullMargin : 10; //Tx1 buffer full margin, reset value: 0x80, access type: RW
		uint32 reserved2 : 6;
	} bitFields;
} RegB01DmacWrapperDma0Misc1Reg_u;

/*REG_B01_DMAC_WRAPPER_PUSH_FIFO_FULL_CTL 0x918 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agentFullCtl : 8; //if 1- full indication when when insert Fifo half full 0- full when insert Fifo Full agent 0/1 TX 2/3 RX, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB01DmacWrapperPushFifoFullCtl_u;

/*REG_B01_DMAC_WRAPPER_DMA0_JUNK_BASE_ADDR_REG 0x920 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0JunkBaseAddrConf : 32; //32 bit base address of junk yard. , NOTE: need to configure carefully to avoid memory rundown, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0JunkBaseAddrReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_STAT1_IND_REG 0x928 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Status1DmacInterrupts : 8; //status dmac interrupts, reset value: 0x0, access type: RO
		uint32 dma0Status1WrapLastJobIndicationsTog : 8; //status wrapper last job indications (toggling), reset value: 0x0, access type: RO
		uint32 dma0Status1JobFifoFullIndications : 2; //status job fifo full indications, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
		uint32 dma0Status1DmacAbortInterrupt : 1; //status dmac abort interrupt, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperDma0Stat1IndReg_u;

/*REG_B01_DMAC_WRAPPER_TX0_SENDER_ALIGNER_EN 0x92C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx0SenderAlignerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperTx0SenderAlignerEn_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_RECOVERY_REG 0x930 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0ForceTxSkip : 1; //In case TX0 ch recovery mode, set this bit to 1'b1 and activate tx skip force on. This sets all the jobs from activation to skip mode in which no DMA job is actualy done only fictive job promotion to completion. It is SW responsibility to clear this bit at the end the recovery process in order to get back to normal job processing. , reset value: 0x0, access type: RW
		uint32 dma0Ch0ForceFcByp : 1; //In case TX0 ch recovery mode, set this bit to 1'b1 and activate fc byp force on. This sets all the jobs from activation to Flow Control bypass mode. This means that FC is not checked before job selection.  It is SW responsibility to clear this bit at the end the recovery process in order to get back to normal job processing., reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB01DmacWrapperDma0Ch0RecoveryReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_RECOVERY_REG 0x934 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1ForceTxSkip : 1; //In case TX1 ch recovery mode, set this bit to 1'b1 and activate tx skip force on. This sets all the jobs from activation to skip mode in which no DMA job is actualy done only fictive job promotion to completion.  It is SW responsibility to clear this bit at the end the recovery process in order to get back to normal job processing., reset value: 0x0, access type: RW
		uint32 dma0Ch1ForceFcByp : 1; //In case TX1 ch recovery mode, set this bit to 1'b1 and activate fc byp force on. This sets all the jobs from activation to Flow Control bypass mode. This means that FC is not checked before job selection.  It is SW responsibility to clear this bit at the end the recovery process in order to get back to normal job processing., reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB01DmacWrapperDma0Ch1RecoveryReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_STAT2_IND_REG 0x938 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txJobReadFsm : 3; //status tx job read fsm, reset value: 0x0, access type: RO
		uint32 rxJobReadFsm : 3; //status rx job read fsm, reset value: 0x0, access type: RO
		uint32 hostJobReadFsm : 3; //status host job read fsm, reset value: 0x0, access type: RO
		uint32 fwJobReadFsm : 3; //status fw job read fsm, reset value: 0x0, access type: RO
		uint32 tx0JobSelFsm : 2; //status tx0 job sel fsm, reset value: 0x0, access type: RO
		uint32 tx1JobSelFsm : 2; //status tx1 job sel fsm, reset value: 0x0, access type: RO
		uint32 rx0JobSelFsm : 2; //status rx0 job sel fsm, reset value: 0x0, access type: RO
		uint32 rx1JobSelFsm : 2; //status rx1 job sel fsm, reset value: 0x0, access type: RO
		uint32 hostJobSelFsm : 2; //status host job sel fsm, reset value: 0x0, access type: RO
		uint32 fwJobSelFsm : 2; //status fw job sel fsm, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB01DmacWrapperDma0Stat2IndReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_0_INT_MASK_REG 0xA00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent0Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent0Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent0Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent0Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent0Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent0Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent0HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent0FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent0Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent0Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent0Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent0Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent0Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent0Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent0IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_0_PEND_INT_REG 0xA04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent0Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent0Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent0Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent0Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent0Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent0Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent0HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent0FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent0Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent0Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent0Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent0Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent0Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent0Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent0PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_0_INT_CLEAR_REG 0xA08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent0Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent0Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent0Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent0Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent0Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent0Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent0HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent0FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent0Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent0Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent0Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent0Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent0Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent0Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent0IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_1_INT_MASK_REG 0xA10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent1Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent1Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent1Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent1Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent1Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent1Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent1HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent1FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent1Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent1Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent1Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent1Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent1Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent1Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent1IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_1_PEND_INT_REG 0xA14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent1Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent1Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent1Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent1Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent1Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent1Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent1HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent1FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent1Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent1Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent1Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent1Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent1Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent1Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent1PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_1_INT_CLEAR_REG 0xA18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent1Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent1Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent1Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent1Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent1Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent1Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent1HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent1FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent1Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent1Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent1Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent1Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent1Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent1Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent1IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_2_INT_MASK_REG 0xA20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent2Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent2Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent2Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent2Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent2Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent2Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent2HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent2FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent2Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent2Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent2Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent2Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent2Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent2Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent2IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_2_PEND_INT_REG 0xA24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent2Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent2Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent2Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent2Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent2Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent2Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent2HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent2FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent2Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent2Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent2Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent2Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent2Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent2Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent2PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_2_INT_CLEAR_REG 0xA28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent2Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent2Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent2Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent2Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent2Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent2Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent2HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent2FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent2Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent2Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent2Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent2Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent2Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent2Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent2IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_3_INT_MASK_REG 0xA30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent3Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent3Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent3Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent3Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent3Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent3Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent3HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent3FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent3Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent3Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent3Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent3Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent3Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent3Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent3IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_3_PEND_INT_REG 0xA34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent3Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent3Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent3Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent3Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent3Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent3Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent3HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent3FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent3Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent3Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent3Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent3Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent3Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent3Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent3PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_3_INT_CLEAR_REG 0xA38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent3Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent3Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent3Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent3Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent3Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent3Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent3HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent3FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent3Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent3Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent3Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent3Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent3Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent3Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent3IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_4_INT_MASK_REG 0xA40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent4Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent4Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent4Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent4Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent4Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent4Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent4HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent4FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent4Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent4Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent4Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent4Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent4Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent4Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent4IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_4_PEND_INT_REG 0xA44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent4Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent4Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent4Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent4Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent4Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent4Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent4HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent4FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent4Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent4Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent4Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent4Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent4Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent4Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent4PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_4_INT_CLEAR_REG 0xA48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent4Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent4Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent4Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent4Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent4Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent4Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent4HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent4FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent4Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent4Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent4Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent4Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent4Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent4Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent4IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_5_INT_MASK_REG 0xA50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent5Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent5Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent5Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent5Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent5Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent5Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent5HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent5FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent5Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent5Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent5Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent5Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent5Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent5Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent5IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_5_PEND_INT_REG 0xA54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent5Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent5Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent5Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent5Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent5Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent5Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent5HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent5FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent5Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent5Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent5Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent5Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent5Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent5Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent5PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_5_INT_CLEAR_REG 0xA58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent5Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent5Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent5Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent5Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent5Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent5Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent5HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent5FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent5Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent5Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent5Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent5Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent5Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent5Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent5IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_6_INT_MASK_REG 0xA60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent6Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent6Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent6Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent6Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent6Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent6Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent6HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent6FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent6Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent6Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent6Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent6Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent6Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent6Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent6IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_6_PEND_INT_REG 0xA64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent6Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent6Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent6Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent6Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent6Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent6Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent6HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent6FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent6Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent6Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent6Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent6Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent6Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent6Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent6PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_6_INT_CLEAR_REG 0xA68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent6Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent6Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent6Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent6Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent6Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent6Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent6HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent6FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent6Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent6Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent6Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent6Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent6Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent6Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent6IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_7_INT_MASK_REG 0xA70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent7Rsvd0IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent7Rsvd1IntEn : 4; //agent 0 (GenriscTX0) enable interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RW
		uint32 agent7Dmac0AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC0 abort, reset value: 0x0, access type: RW
		uint32 agent7Dmac1AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent7Dmac2AbortIntEn : 1; //agent 0 (GenriscTX0) enable interrupts DMAC1 abort, reset value: 0x0, access type: RW
		uint32 agent7Rsvd2IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent7HostLastJobIdIntEn : 8; //agent 0 (GenriscTX0) enable interrupts "host last job id", reset value: 0x0, access type: RW
		uint32 agent7FwLastJobIdIntEn : 3; //agent 0 (GenriscTX0) enable interrupts "fw last job id", reset value: 0x0, access type: RW
		uint32 agent7Rsvd3IntEn : 1; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent7Tx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent7Tx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "tx1 last job is done", reset value: 0x0, access type: RW
		uint32 agent7Rsvd4IntEn : 2; //agent 0 (GenriscTX0) enable interrupts Reserved, reset value: 0x0, access type: RW
		uint32 agent7Rx0LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx0 last job is done", reset value: 0x0, access type: RW
		uint32 agent7Rx1LastJobDoneIntEn : 1; //agent 0 (GenriscTX0) enable interrupts "rx1 last job is done", reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent7IntMaskReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_7_PEND_INT_REG 0xA74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent7Rsvd0PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC0 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent7Rsvd1PendInt : 4; //agent 0 (GenriscTX0) pending interrupts DMAC1 bits[7:4], reset value: 0x0, access type: RO
		uint32 agent7Dmac0AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC0 abort, reset value: 0x0, access type: RO
		uint32 agent7Dmac1AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent7Dmac2AbortPendInt : 1; //agent 0 (GenriscTX0) pending interrupts DMAC1 abort, reset value: 0x0, access type: RO
		uint32 agent7Rsvd2PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent7HostLastJobIdPendInt : 8; //agent 0 (GenriscTX0) pending interrupts "host last job id", reset value: 0x0, access type: RO
		uint32 agent7FwLastJobIdPendInt : 3; //agent 0 (GenriscTX0) pending interrupts "fw last job id", reset value: 0x0, access type: RO
		uint32 agent7Rsvd3PendInt : 1; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent7Tx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent7Tx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "tx1 last job is done", reset value: 0x0, access type: RO
		uint32 agent7Rsvd4PendInt : 2; //agent 0 (GenriscTX0) pending interrupts Reserved, reset value: 0x0, access type: RO
		uint32 agent7Rx0LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx0 last job is done", reset value: 0x0, access type: RO
		uint32 agent7Rx1LastJobDonePendInt : 1; //agent 0 (GenriscTX0) pending interrupts "rx1 last job is done", reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent7PendIntReg_u;

/*REG_B01_DMAC_WRAPPER_AGENT_7_INT_CLEAR_REG 0xA78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agent7Rsvd0IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC0 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent7Rsvd1IntClear : 4; //agent 0 (GenriscTX0) interrupt clear DMAC1 bits[7:4], reset value: 0x0, access type: WO
		uint32 agent7Dmac0AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC0 abort, reset value: 0x0, access type: WO
		uint32 agent7Dmac1AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent7Dmac2AbortIntClear : 1; //agent 0 (GenriscTX0) interrupt clear DMAC1 abort, reset value: 0x0, access type: WO
		uint32 agent7Rsvd2IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent7HostLastJobIdIntClear : 8; //agent 0 (GenriscTX0) interrupt clear "host last job id", reset value: 0x0, access type: WO
		uint32 agent7FwLastJobIdIntClear : 3; //agent 0 (GenriscTX0) interrupt clear "fw last job id", reset value: 0x0, access type: WO
		uint32 agent7Rsvd3IntClear : 1; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent7Tx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent7Tx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "tx1 last job is done", reset value: 0x0, access type: WO
		uint32 agent7Rsvd4IntClear : 2; //agent 0 (GenriscTX0) interrupt clear Reserved, reset value: 0x0, access type: WO
		uint32 agent7Rx0LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx0 last job is done", reset value: 0x0, access type: WO
		uint32 agent7Rx1LastJobDoneIntClear : 1; //agent 0 (GenriscTX0) interrupt clear "rx1 last job is done", reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
	} bitFields;
} RegB01DmacWrapperAgent7IntClearReg_u;

/*REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC 0xB00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx0JobDoneLastAcc : 8; //Accumulator of job done last of Channel1, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
		uint32 rx0JobDoneLastAccExceed : 1; //Accumulator of job done last of Channel1 exceed the maximum value indication (or decrement below the zero) - Clear by write to ch1_job_done_last_acc_clr, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperRx0JobDoneLastAcc_u;

/*REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC_DEC 0xB04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx0JobDoneLastAccDec : 1; //Write 0x1 - Decrement by one the accumulator of job done last of Channel1 , reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperRx0JobDoneLastAccDec_u;

/*REG_B01_DMAC_WRAPPER_RX0_JOB_DONE_LAST_ACC_CLR 0xB08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx0JobDoneLastAccClr : 1; //Write 0x1 - Clear the accumulator of job done last of Channel1 , reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperRx0JobDoneLastAccClr_u;

/*REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC 0xB10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx1JobDoneLastAcc : 8; //Accumulator of job done last of Channel1, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
		uint32 rx1JobDoneLastAccExceed : 1; //Accumulator of job done last of Channel1 exceed the maximum value indication (or decrement below the zero) - Clear by write to ch1_job_done_last_acc_clr, reset value: 0x0, access type: RO
	} bitFields;
} RegB01DmacWrapperRx1JobDoneLastAcc_u;

/*REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC_DEC 0xB14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx1JobDoneLastAccDec : 1; //Write 0x1 - Decrement by one the accumulator of job done last of Channel1 , reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperRx1JobDoneLastAccDec_u;

/*REG_B01_DMAC_WRAPPER_RX1_JOB_DONE_LAST_ACC_CLR 0xB18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rx1JobDoneLastAccClr : 1; //Write 0x1 - Clear the accumulator of job done last of Channel1 , reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01DmacWrapperRx1JobDoneLastAccClr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_CONFIG_REG 0xC00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '2' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma1Ch0EndianSwap : 2; //ch0 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma1Ch0ExtFcEnable : 1; //ch0 external flow control enable, reset value: 0x0, access type: RW
		uint32 dma1Ch0RxbufFcEnable : 1; //ch0 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 dma1Ch0WriteNonPost : 1; //ch0 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 dma1Ch0LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01DmacWrapperDma1Ch0ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_SRC_ADDR 0xC04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0FifoPushSrcAddr : 32; //ch0 fifo push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch0FifoPushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_DEST_ADDR 0xC08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0FifoPushDestAddr : 32; //ch0 fifo push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch0FifoPushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_C_REG 0xC0C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0FifoPushLength : 15; //ch0 fifo push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma1Ch0FifoPushEnd : 1; //ch0 fifo push end, reset value: 0x0, access type: RW
		uint32 dma1Ch0FifoPushLast : 1; //ch0 fifo push last, reset value: 0x0, access type: RW
		uint32 dma1Ch0FifoPushIdentifier : 7; //ch0 fifo push identifier, reset value: 0x0, access type: RW
		uint32 dma1Ch0FifoPushSkip : 1; //ch0 fifo skip, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 dma1Ch0FifoPushFcByp : 1; //ch0 fifo fc bypass, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB01DmacWrapperDma1Ch0FifoPushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_FIFO_PUSH_MPDUP_REG 0xC10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0FifoPushMpduPtr : 16; //ch0 fifo push rx mpdu pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01DmacWrapperDma1Ch0FifoPushMpdupReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_CONFIG_REG 0xC20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '2' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma1Ch1EndianSwap : 2; //ch1 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma1Ch1ExtFcEnable : 1; //ch1 external flow control enable, reset value: 0x0, access type: RW
		uint32 dma1Ch1RxbufFcEnable : 1; //ch1 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 dma1Ch1WriteNonPost : 1; //ch1 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 dma1Ch1LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01DmacWrapperDma1Ch1ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_SRC_ADDR 0xC24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1FifoPushSrcAddr : 32; //ch1 fifo push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch1FifoPushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_DEST_ADDR 0xC28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1FifoPushDestAddr : 32; //ch1 fifo push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch1FifoPushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_C_REG 0xC2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1FifoPushLength : 15; //ch1 fifo push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma1Ch1FifoPushEnd : 1; //ch1 fifo push end, reset value: 0x0, access type: RW
		uint32 dma1Ch1FifoPushLast : 1; //ch1 fifo push last, reset value: 0x0, access type: RW
		uint32 dma1Ch1FifoPushIdentifier : 7; //ch1 fifo push identifier, reset value: 0x0, access type: RW
		uint32 dma1Ch1FifoPushSkip : 1; //ch1 fifo skip, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 dma1Ch1FifoPushFcByp : 1; //ch1 fifo fc bypass, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB01DmacWrapperDma1Ch1FifoPushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_FIFO_PUSH_MPDUP_REG 0xC30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1FifoPushMpduPtr : 16; //ch1 fifo push rx mpdu pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01DmacWrapperDma1Ch1FifoPushMpdupReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_CONFIG_REG 0xC40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '2' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma1Ch2EndianSwap : 2; //ch2 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma1Ch2ExtFcEnable : 1; //ch2 external flow control enable, reset value: 0x0, access type: RW
		uint32 dma1Ch2WriteNonPost : 1; //ch2 external flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
		uint32 dma1Ch2LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01DmacWrapperDma1Ch2ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_SRC_ADDR 0xC44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2FifoPushSrcAddr : 32; //ch2 fifo push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch2FifoPushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_DEST_ADDR 0xC48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2FifoPushDestAddr : 32; //ch2 fifo push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma1Ch2FifoPushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_FIFO_PUSH_C_REG 0xC4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2FifoPushLength : 15; //ch2 fifo push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma1Ch2FifoPushEnd : 1; //ch2 fifo push end, reset value: 0x0, access type: RW
		uint32 dma1Ch2FifoPushLast : 1; //ch2 fifo push last, reset value: 0x0, access type: RW
		uint32 dma1Ch2FifoPushIdentifier : 3; //ch2 fifo push identifier, reset value: 0x0, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegB01DmacWrapperDma1Ch2FifoPushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_CONFIG_REG 0xC60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '2' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma2Ch0EndianSwap : 2; //ch3 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma2Ch0ExtFcEnable : 1; //ch3 external flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma2Ch0WriteNonPost : 1; //ch3 external flow control enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
		uint32 dma2Ch0LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB01DmacWrapperDma2Ch0ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_SRC_ADDR 0xC64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo0PushSrcAddr : 32; //ch3 fifo0 push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo0PushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_DEST_ADDR 0xC68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo0PushDestAddr : 32; //ch3 fifo0 push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo0PushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO0_PUSH_C_REG 0xC6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo0PushLength : 15; //ch3 fifo0 push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma2Ch0Fifo0PushEnd : 1; //ch3 fifo0 push end, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo0PushLast : 1; //ch3 fifo0 push last, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo0PushIdentifier : 2; //ch3 fifo0 push identifier, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo0PushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_SRC_ADDR 0xC80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo1PushSrcAddr : 32; //ch3 fifo1 push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo1PushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_DEST_ADDR 0xC84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo1PushDestAddr : 32; //ch3 fifo1 push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo1PushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO1_PUSH_C_REG 0xC88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo1PushLength : 15; //ch3 fifo1 push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma2Ch0Fifo1PushEnd : 1; //ch3 fifo1 push end, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo1PushLast : 1; //ch3 fifo1 push last, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo1PushIdentifier : 2; //ch3 fifo1 push identifier, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo1PushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_SRC_ADDR 0xC90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo2PushSrcAddr : 32; //ch3 fifo2 push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo2PushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_DEST_ADDR 0xC94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo2PushDestAddr : 32; //ch3 fifo2 push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo2PushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_FIFO2_PUSH_C_REG 0xC98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0Fifo2PushLength : 15; //ch3 fifo2 push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma2Ch0Fifo2PushEnd : 1; //ch3 fifo2 push end, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo2PushLast : 1; //ch3 fifo2 push last, reset value: 0x0, access type: RW
		uint32 dma2Ch0Fifo2PushIdentifier : 2; //ch3 fifo2 push identifier, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB01DmacWrapperDma2Ch0Fifo2PushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_CONFIG_REG 0xD00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '2' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma0Ch0EndianSwap : 2; //ch0 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma0Ch0ExtFcEnable : 1; //ch0 external flow control enable, reset value: 0x0, access type: RW
		uint32 dma0Ch0TxbufFcEnable : 1; //ch0 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 dma0Ch0WriteNonPost : 1; //ch0 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 dma0Ch0LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01DmacWrapperDma0Ch0ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_SRC_ADDR 0xD04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0FifoPushSrcAddr : 32; //ch0 fifo push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0Ch0FifoPushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_DEST_ADDR 0xD08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0FifoPushDestAddr : 32; //ch0 fifo push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0Ch0FifoPushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_C_REG 0xD0C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0FifoPushLength : 15; //ch0 fifo push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma0Ch0FifoPushEnd : 1; //ch0 fifo push end, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushLast : 1; //ch0 fifo push last, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushIdentifier : 7; //ch0 fifo push identifier, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushSkip : 1; //ch0 fifo skip, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 dma0Ch0FifoPushFcByp : 1; //ch0 fifo push fc bypass, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB01DmacWrapperDma0Ch0FifoPushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_FIFO_PUSH_MPDUP_REG 0xD10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0FifoPushTxMpduPtr : 16; //ch0 fifo push tx mpdu pointer, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushDoMpduHdr : 1; //ch0 fifo push tx do MPDU Header bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushDoMsduHdr : 1; //ch0 fifo push tx do MSDU Header bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushAddSecurity : 1; //ch0 fifo push tx add security bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushFirstMsduInMpdu : 1; //ch0 fifo push tx first MSDU in MPDU bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushLastMsduInMpdu : 1; //ch0 fifo push tx last MSDU in MPDU bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushEtype : 2; //ch0 fifo push tx Etype bits for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushAmsduPadd : 2; //ch0 fifo push tx AMSDU padding bits for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch0FifoPushSpp : 1; //ch0 fifo push tx SSP bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB01DmacWrapperDma0Ch0FifoPushMpdupReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_CONFIG_REG 0xD20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1BusAndTrnsfSize : 2; //ch max transfer '0' - 64 Bytes, '1' - 128 Bytes '1' - 256 Bytes , , reset value: 0x2, access type: RW
		uint32 dma0Ch1EndianSwap : 2; //ch1 endian swap (0=none;1=within shorts;2=within words;3=within doublewords), reset value: 0x0, access type: RW
		uint32 dma0Ch1ExtFcEnable : 1; //ch1 external flow control enable, reset value: 0x0, access type: RW
		uint32 dma0Ch1TxbufFcEnable : 1; //ch1 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 dma0Ch1WriteNonPost : 1; //ch1 tx buf flow control enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 dma0Ch1LegacyModeEn : 1; //'0' - legacy mode not active , '1' - legacy mode active, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01DmacWrapperDma0Ch1ConfigReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_SRC_ADDR 0xD24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1FifoPushSrcAddr : 32; //ch1 fifo push source address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0Ch1FifoPushSrcAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_DEST_ADDR 0xD28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1FifoPushDestAddr : 32; //ch1 fifo push destination address, reset value: 0x0, access type: RW
	} bitFields;
} RegB01DmacWrapperDma0Ch1FifoPushDestAddr_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_C_REG 0xD2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1FifoPushLength : 15; //ch1 fifo push length, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 dma0Ch1FifoPushEnd : 1; //ch1 fifo push end, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushLast : 1; //ch1 fifo push last, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushIdentifier : 7; //ch1 fifo push identifier, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushSkip : 1; //ch1 fifo skip, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 dma0Ch1FifoPushFcByp : 1; //ch1 fifo fc bypass, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB01DmacWrapperDma0Ch1FifoPushCReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_FIFO_PUSH_MPDUP_REG 0xD30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1FifoPushTxMpduPtr : 16; //ch1 fifo push tx mpdu pointer, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushDoMpduHdr : 1; //ch1 fifo push tx do MPDU Header bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushDoMsduHdr : 1; //ch1 fifo push tx do MSDU Header bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushAddSecurity : 1; //ch1 fifo push tx add security bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushFirstMsduInMpdu : 1; //ch1 fifo push tx first MSDU in MPDU bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushLastMsduInMpdu : 1; //ch1 fifo push tx last MSDU in MPDU bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushEtype : 2; //ch1 fifo push tx Etype bits for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushAmsduPadd : 2; //ch1 fifo push tx AMSDU padding bits for Header Converdion module, reset value: 0x0, access type: RW
		uint32 dma0Ch1FifoPushSpp : 1; //ch1 fifo push tx SSP bit for Header Converdion module, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB01DmacWrapperDma0Ch1FifoPushMpdupReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_JOB_CNT_REG 0xF00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0JobCnt : 10; //ch0 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch0JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_BATCH_CNT_REG 0xF04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0BatchCnt : 10; //ch0 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch0BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH0_STAT_IND_REG 0xF08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch0StatusLastJobIndicationTog : 1; //ch0 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma1Ch0StatusLastIdJob : 7; //ch0 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma1Ch0StatusChannelControllerSm : 3; //ch0 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma1Ch0StatusIsIdle : 1; //ch0 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma1Ch0StatusDmaChannelActive : 1; //ch0 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma1Ch0StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_JOB_CNT_REG 0xF10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1JobCnt : 10; //ch1 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch1JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_BATCH_CNT_REG 0xF14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1BatchCnt : 10; //ch1 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch1BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH1_STAT_IND_REG 0xF18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch1StatusLastJobIndicationTog : 1; //ch1 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma1Ch1StatusLastIdJob : 7; //ch1 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma1Ch1StatusChannelControllerSm : 3; //ch1 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma1Ch1StatusIsIdle : 1; //ch1 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma1Ch1StatusDmaChannelActive : 1; //ch1 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma1Ch1StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_JOB_CNT_REG 0xF20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2JobCnt : 10; //ch2 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch2JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_BATCH_CNT_REG 0xF24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2BatchCnt : 10; //ch2 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma1Ch2BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA1_CH2_STAT_IND_REG 0xF28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma1Ch2StatusLastJobIndicationTog : 1; //ch2 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma1Ch2StatusLastIdJob : 7; //ch2 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma1Ch2StatusChannelControllerSm : 3; //ch2 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma1Ch2StatusIsIdle : 1; //ch2 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma1Ch2StatusDmaChannelActive : 1; //ch2 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma1Ch2StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_JOB_CNT_REG 0xF30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0JobCnt : 10; //ch3 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma2Ch0JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_BATCH_CNT_REG 0xF34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0BatchCnt : 10; //ch3 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma2Ch0BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA2_CH0_STAT_IND_REG 0xF38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma2Ch0StatusLastJobIndicationTog : 1; //ch3 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma2Ch0StatusLastIdJob : 7; //ch3 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma2Ch0StatusChannelControllerSm : 3; //ch3 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma2Ch0StatusIsIdle : 1; //ch3 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma2Ch0StatusDmaChannelActive : 1; //ch3 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma2Ch0StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_JOB_CNT_REG 0xF40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0JobCnt : 10; //ch0 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma0Ch0JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_BATCH_CNT_REG 0xF44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0BatchCnt : 10; //ch0 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma0Ch0BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH0_STAT_IND_REG 0xF48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch0StatusLastJobIndicationTog : 1; //ch0 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma0Ch0StatusLastIdJob : 7; //ch0 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0Ch0StatusChannelControllerSm : 3; //ch0 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0Ch0StatusIsIdle : 1; //ch0 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma0Ch0StatusDmaChannelActive : 1; //ch0 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma0Ch0StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_JOB_CNT_REG 0xF50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1JobCnt : 10; //ch1 jobs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma0Ch1JobCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_BATCH_CNT_REG 0xF54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1BatchCnt : 10; //ch1 batch counter (cleared on read), reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB01DmacWrapperDma0Ch1BatchCntReg_u;

/*REG_B01_DMAC_WRAPPER_DMA0_CH1_STAT_IND_REG 0xF58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dma0Ch1StatusLastJobIndicationTog : 1; //ch1 status last job indication (toggling), reset value: 0x0, access type: RO
		uint32 dma0Ch1StatusLastIdJob : 7; //ch1 status last id job, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 dma0Ch1StatusChannelControllerSm : 3; //ch1 status channel controller state machine, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 dma0Ch1StatusIsIdle : 1; //ch1 status channel controller is idle indication, reset value: 0x1, access type: RO
		uint32 reserved2 : 1;
		uint32 dma0Ch1StatusDmaChannelActive : 1; //ch1 status DMAC channel active indication, reset value: 0x0, access type: RO
		uint32 reserved3 : 14;
	} bitFields;
} RegB01DmacWrapperDma0Ch1StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO0_STAT_IND_REG 0xF60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo0StatusJobFifoCount : 3; //fifo0 (TX0) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo0StatusJobFifoEmpty : 1; //fifo0 (TX0) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo0StatusJobFifoFull : 1; //fifo0 (TX0) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo0StatusLastFifoPushIgnored : 1; //fifo0 (TX0) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo0StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO1_STAT_IND_REG 0xF64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo1StatusJobFifoCount : 3; //fifo1 (TX1) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo1StatusJobFifoEmpty : 1; //fifo1 (TX1) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo1StatusJobFifoFull : 1; //fifo1 (TX1) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo1StatusLastFifoPushIgnored : 1; //fifo1 (TX1) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo1StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO2_STAT_IND_REG 0xF68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo2StatusJobFifoCount : 3; //fifo2 (RX0) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo2StatusJobFifoEmpty : 1; //fifo2 (RX0) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo2StatusJobFifoFull : 1; //fifo2 (RX0) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo2StatusLastFifoPushIgnored : 1; //fifo2 (RX0) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo2StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO3_STAT_IND_REG 0xF6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo3StatusJobFifoCount : 3; //fifo3 (RX1) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo3StatusJobFifoEmpty : 1; //fifo3 (RX1) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo3StatusJobFifoFull : 1; //fifo3 (RX1) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo3StatusLastFifoPushIgnored : 1; //fifo3 (RX1) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo3StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO4_STAT_IND_REG 0xF70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo4StatusJobFifoCount : 3; //fifo4 (HOST) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo4StatusJobFifoEmpty : 1; //fifo4 (HOST) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo4StatusJobFifoFull : 1; //fifo4 (HOST) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo4StatusLastFifoPushIgnored : 1; //fifo4 (HOST) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo4StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO5_STAT_IND_REG 0xF74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo5StatusJobFifoCount : 3; //fifo5 (CPU0) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo5StatusJobFifoEmpty : 1; //fifo5 (CPU0) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo5StatusJobFifoFull : 1; //fifo5 (CPU0) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo5StatusLastFifoPushIgnored : 1; //fifo5 (CPU0) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo5StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO6_STAT_IND_REG 0xF78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo6StatusJobFifoCount : 3; //fifo6 (CPU1) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo6StatusJobFifoEmpty : 1; //fifo6 (CPU1) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo6StatusJobFifoFull : 1; //fifo6 (CPU1) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo6StatusLastFifoPushIgnored : 1; //fifo6 (CPU1) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo6StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_FIFO7_STAT_IND_REG 0xF7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo7StatusJobFifoCount : 3; //fifo7 (CPU2) status job fifo count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 fifo7StatusJobFifoEmpty : 1; //fifo7 (CPU2) status job fifo empty, reset value: 0x1, access type: RO
		uint32 fifo7StatusJobFifoFull : 1; //fifo7 (CPU2) status job fifo full, reset value: 0x0, access type: RO
		uint32 fifo7StatusLastFifoPushIgnored : 1; //fifo7 (CPU2) status last fifo push ignored, reset value: 0x0, access type: RO
		uint32 reserved1 : 25;
	} bitFields;
} RegB01DmacWrapperFifo7StatIndReg_u;

/*REG_B01_DMAC_WRAPPER_SPARE_32_BIT_REG 0xFF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmacSpare32Bit : 32; //no description, reset value: 0x88888888, access type: RW
	} bitFields;
} RegB01DmacWrapperSpare32BitReg_u;



#endif // _B01_DMAC_WRAPPER_REGS_H_
