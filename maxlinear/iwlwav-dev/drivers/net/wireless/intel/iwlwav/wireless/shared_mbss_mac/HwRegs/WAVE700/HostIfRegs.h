
/***********************************************************************************
File:				HostIfRegs.h
Module:				hostIf
SOC Revision:		latest
Generated at:       2022-05-04 10:58:43
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOST_IF_REGS_H_
#define _HOST_IF_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOST_IF_BASE_ADDRESS                             MEMORY_MAP_UNIT_4_BASE_ADDRESS
#define	REG_HOST_IF_WLAN_SECURE_STATUS                     (HOST_IF_BASE_ADDRESS + 0x10)
#define	REG_HOST_IF_WLAN_EFUSE_INDICATION                  (HOST_IF_BASE_ADDRESS + 0x14)
#define	REG_HOST_IF_HOST_IRQ_STATUS                        (HOST_IF_BASE_ADDRESS + 0x18)
#define	REG_HOST_IF_HOST_IRQ_MASK                          (HOST_IF_BASE_ADDRESS + 0x1C)
#define	REG_HOST_IF_HOST_LS_CTL_STAT                       (HOST_IF_BASE_ADDRESS + 0x20)
#define	REG_HOST_IF_TX_IN_CORE_INT1                        (HOST_IF_BASE_ADDRESS + 0x24)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_ADD                     (HOST_IF_BASE_ADDRESS + 0x28)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_ADD                     (HOST_IF_BASE_ADDRESS + 0x2C)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_ADD                    (HOST_IF_BASE_ADDRESS + 0x30)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_ADD                    (HOST_IF_BASE_ADDRESS + 0x34)
#define	REG_HOST_IF_RX_IN_MGMT_ACCUM_ADD                   (HOST_IF_BASE_ADDRESS + 0x38)
#define	REG_HOST_IF_TX_IN_MGMT_ACCUM_ADD                   (HOST_IF_BASE_ADDRESS + 0x3C)
#define	REG_HOST_IF_RX_OUT_MGMT_ACCUM_ADD                  (HOST_IF_BASE_ADDRESS + 0x40)
#define	REG_HOST_IF_TX_OUT_MGMT_ACCUM_ADD                  (HOST_IF_BASE_ADDRESS + 0x44)
#define	REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_ADD             (HOST_IF_BASE_ADDRESS + 0x48)
#define	REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_ADD             (HOST_IF_BASE_ADDRESS + 0x4C)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_SUB                     (HOST_IF_BASE_ADDRESS + 0x50)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_SUB                     (HOST_IF_BASE_ADDRESS + 0x54)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_SUB                    (HOST_IF_BASE_ADDRESS + 0x58)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_SUB                    (HOST_IF_BASE_ADDRESS + 0x5C)
#define	REG_HOST_IF_RX_IN_MGMT_ACCUM_SUB                   (HOST_IF_BASE_ADDRESS + 0x60)
#define	REG_HOST_IF_TX_IN_MGMT_ACCUM_SUB                   (HOST_IF_BASE_ADDRESS + 0x64)
#define	REG_HOST_IF_RX_OUT_MGMT_ACCUM_SUB                  (HOST_IF_BASE_ADDRESS + 0x68)
#define	REG_HOST_IF_TX_OUT_MGMT_ACCUM_SUB                  (HOST_IF_BASE_ADDRESS + 0x6C)
#define	REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_SUB             (HOST_IF_BASE_ADDRESS + 0x70)
#define	REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_SUB             (HOST_IF_BASE_ADDRESS + 0x74)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_COUNT                   (HOST_IF_BASE_ADDRESS + 0x78)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_COUNT                   (HOST_IF_BASE_ADDRESS + 0x7C)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT                  (HOST_IF_BASE_ADDRESS + 0x80)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT                  (HOST_IF_BASE_ADDRESS + 0x84)
#define	REG_HOST_IF_RX_IN_MGMT_ACCUM_COUNT                 (HOST_IF_BASE_ADDRESS + 0x88)
#define	REG_HOST_IF_TX_IN_MGMT_ACCUM_COUNT                 (HOST_IF_BASE_ADDRESS + 0x8C)
#define	REG_HOST_IF_RX_OUT_MGMT_ACCUM_COUNT                (HOST_IF_BASE_ADDRESS + 0x90)
#define	REG_HOST_IF_TX_OUT_MGMT_ACCUM_COUNT                (HOST_IF_BASE_ADDRESS + 0x94)
#define	REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_COUNT           (HOST_IF_BASE_ADDRESS + 0x98)
#define	REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_COUNT           (HOST_IF_BASE_ADDRESS + 0x9C)
#define	REG_HOST_IF_TX_IN_CORE_INT                         (HOST_IF_BASE_ADDRESS + 0xA4)
#define	REG_HOST_IF_TX_OUT_CORE_INT                        (HOST_IF_BASE_ADDRESS + 0xA8)
#define	REG_HOST_IF_RX_IN_CORE_INT                         (HOST_IF_BASE_ADDRESS + 0xAC)
#define	REG_HOST_IF_RX_OUT_CORE_INT                        (HOST_IF_BASE_ADDRESS + 0xB0)
#define	REG_HOST_IF_HOST_IF_HD_COUNT_INT                   (HOST_IF_BASE_ADDRESS + 0xB8)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_ADD_BIG_END             (HOST_IF_BASE_ADDRESS + 0xC0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_ADD_BIG_END             (HOST_IF_BASE_ADDRESS + 0xC4)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_BIG_END            (HOST_IF_BASE_ADDRESS + 0xC8)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_BIG_END            (HOST_IF_BASE_ADDRESS + 0xCC)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_SUB_BIG_END             (HOST_IF_BASE_ADDRESS + 0xD0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_SUB_BIG_END             (HOST_IF_BASE_ADDRESS + 0xD4)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_BIG_END            (HOST_IF_BASE_ADDRESS + 0xD8)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_BIG_END            (HOST_IF_BASE_ADDRESS + 0xDC)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_BIG_END           (HOST_IF_BASE_ADDRESS + 0xE0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_BIG_END           (HOST_IF_BASE_ADDRESS + 0xE4)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_BIG_END          (HOST_IF_BASE_ADDRESS + 0xE8)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_BIG_END          (HOST_IF_BASE_ADDRESS + 0xEC)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_ADD_FREED               (HOST_IF_BASE_ADDRESS + 0xF0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_ADD_FREED               (HOST_IF_BASE_ADDRESS + 0xF4)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_READY              (HOST_IF_BASE_ADDRESS + 0xF8)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_READY              (HOST_IF_BASE_ADDRESS + 0xFC)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_SUB_FREED               (HOST_IF_BASE_ADDRESS + 0x100)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_SUB_FREED               (HOST_IF_BASE_ADDRESS + 0x104)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_READY              (HOST_IF_BASE_ADDRESS + 0x108)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_READY              (HOST_IF_BASE_ADDRESS + 0x10C)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_FREED             (HOST_IF_BASE_ADDRESS + 0x110)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_FREED             (HOST_IF_BASE_ADDRESS + 0x114)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_READY            (HOST_IF_BASE_ADDRESS + 0x118)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_READY            (HOST_IF_BASE_ADDRESS + 0x11C)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_ADD_FREED_BIG_END       (HOST_IF_BASE_ADDRESS + 0x120)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_ADD_FREED_BIG_END       (HOST_IF_BASE_ADDRESS + 0x124)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_READY_BIG_END      (HOST_IF_BASE_ADDRESS + 0x128)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_READY_BIG_END      (HOST_IF_BASE_ADDRESS + 0x12C)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_SUB_FREED_BIG_END       (HOST_IF_BASE_ADDRESS + 0x130)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_SUB_FREED_BIG_END       (HOST_IF_BASE_ADDRESS + 0x134)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_READY_BIG_END      (HOST_IF_BASE_ADDRESS + 0x138)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_READY_BIG_END      (HOST_IF_BASE_ADDRESS + 0x13C)
#define	REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_FREED_BIG_END     (HOST_IF_BASE_ADDRESS + 0x140)
#define	REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_FREED_BIG_END     (HOST_IF_BASE_ADDRESS + 0x144)
#define	REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_READY_BIG_END    (HOST_IF_BASE_ADDRESS + 0x148)
#define	REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_READY_BIG_END    (HOST_IF_BASE_ADDRESS + 0x14C)
#define	REG_HOST_IF_ENABLE_UPI_INTERRUPT                   (HOST_IF_BASE_ADDRESS + 0x160)
#define	REG_HOST_IF_UPI_INTERRUPT                          (HOST_IF_BASE_ADDRESS + 0x164)
#define	REG_HOST_IF_UPI_INTERRUPT_SET                      (HOST_IF_BASE_ADDRESS + 0x168)
#define	REG_HOST_IF_UPI_INTERRUPT_CLEAR                    (HOST_IF_BASE_ADDRESS + 0x16C)
#define	REG_HOST_IF_ENABLE_LPI_INTERRUPT                   (HOST_IF_BASE_ADDRESS + 0x170)
#define	REG_HOST_IF_LPI_INTERRUPT                          (HOST_IF_BASE_ADDRESS + 0x174)
#define	REG_HOST_IF_LPI_INTERRUPT_SET                      (HOST_IF_BASE_ADDRESS + 0x178)
#define	REG_HOST_IF_LPI_INTERRUPT_CLEAR                    (HOST_IF_BASE_ADDRESS + 0x17C)
#define	REG_HOST_IF_ENABLE_PHI_INTERRUPT                   (HOST_IF_BASE_ADDRESS + 0x180)
#define	REG_HOST_IF_PHI_INTERRUPT                          (HOST_IF_BASE_ADDRESS + 0x184)
#define	REG_HOST_IF_PHI_INTERRUPT_SET                      (HOST_IF_BASE_ADDRESS + 0x188)
#define	REG_HOST_IF_PHI_INTERRUPT_CLEAR                    (HOST_IF_BASE_ADDRESS + 0x18C)
#define	REG_HOST_IF_ENABLE_NPU2UPI_INTERRUPT               (HOST_IF_BASE_ADDRESS + 0x190)
#define	REG_HOST_IF_NPU2UPI_INTERRUPT                      (HOST_IF_BASE_ADDRESS + 0x194)
#define	REG_HOST_IF_NPU2UPI_INTERRUPT_SET                  (HOST_IF_BASE_ADDRESS + 0x198)
#define	REG_HOST_IF_NPU2UPI_INTERRUPT_CLEAR                (HOST_IF_BASE_ADDRESS + 0x19C)
#define	REG_HOST_IF_ENABLE_LPI1_INTERRUPT                  (HOST_IF_BASE_ADDRESS + 0x1A0)
#define	REG_HOST_IF_LPI1_INTERRUPT                         (HOST_IF_BASE_ADDRESS + 0x1A4)
#define	REG_HOST_IF_LPI1_INTERRUPT_SET                     (HOST_IF_BASE_ADDRESS + 0x1A8)
#define	REG_HOST_IF_LPI1_INTERRUPT_CLEAR                   (HOST_IF_BASE_ADDRESS + 0x1AC)
#define	REG_HOST_IF_CLIENT_0_SMPHR_TOGGLE_31TO00           (HOST_IF_BASE_ADDRESS + 0x1B0)
#define	REG_HOST_IF_CLIENT_0_SMPHR_TOGGLE_63TO32           (HOST_IF_BASE_ADDRESS + 0x1B4)
#define	REG_HOST_IF_CLIENT_1_SMPHR_TOGGLE_31TO00           (HOST_IF_BASE_ADDRESS + 0x1B8)
#define	REG_HOST_IF_CLIENT_1_SMPHR_TOGGLE_63TO32           (HOST_IF_BASE_ADDRESS + 0x1BC)
#define	REG_HOST_IF_CLIENT_2_SMPHR_TOGGLE_31TO00           (HOST_IF_BASE_ADDRESS + 0x1C0)
#define	REG_HOST_IF_CLIENT_2_SMPHR_TOGGLE_63TO32           (HOST_IF_BASE_ADDRESS + 0x1C4)
#define	REG_HOST_IF_CLIENT_0_SMPHR_STAT_15TO00             (HOST_IF_BASE_ADDRESS + 0x1C8)
#define	REG_HOST_IF_CLIENT_0_SMPHR_STAT_31TO16             (HOST_IF_BASE_ADDRESS + 0x1CC)
#define	REG_HOST_IF_CLIENT_0_SMPHR_STAT_47TO32             (HOST_IF_BASE_ADDRESS + 0x1D0)
#define	REG_HOST_IF_CLIENT_0_SMPHR_STAT_63TO48             (HOST_IF_BASE_ADDRESS + 0x1D4)
#define	REG_HOST_IF_CLIENT_1_SMPHR_STAT_15TO00             (HOST_IF_BASE_ADDRESS + 0x1D8)
#define	REG_HOST_IF_CLIENT_1_SMPHR_STAT_31TO16             (HOST_IF_BASE_ADDRESS + 0x1DC)
#define	REG_HOST_IF_CLIENT_1_SMPHR_STAT_47TO32             (HOST_IF_BASE_ADDRESS + 0x1E0)
#define	REG_HOST_IF_CLIENT_1_SMPHR_STAT_63TO48             (HOST_IF_BASE_ADDRESS + 0x1E4)
#define	REG_HOST_IF_CLIENT_2_SMPHR_STAT_15TO00             (HOST_IF_BASE_ADDRESS + 0x1E8)
#define	REG_HOST_IF_CLIENT_2_SMPHR_STAT_31TO16             (HOST_IF_BASE_ADDRESS + 0x1EC)
#define	REG_HOST_IF_CLIENT_2_SMPHR_STAT_47TO32             (HOST_IF_BASE_ADDRESS + 0x1F0)
#define	REG_HOST_IF_CLIENT_2_SMPHR_STAT_63TO48             (HOST_IF_BASE_ADDRESS + 0x1F4)
#define	REG_HOST_IF_UCPU_STAT                              (HOST_IF_BASE_ADDRESS + 0x200)
#define	REG_HOST_IF_NOCS_TIMEOUT_EN                        (HOST_IF_BASE_ADDRESS + 0x204)
#define	REG_HOST_IF_NOCS_FAULT_EN                          (HOST_IF_BASE_ADDRESS + 0x208)
#define	REG_HOST_IF_NOC_TIMEOUT_DIV_SEL                    (HOST_IF_BASE_ADDRESS + 0x20C)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_ADD                    (HOST_IF_BASE_ADDRESS + 0x214)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_ADD                    (HOST_IF_BASE_ADDRESS + 0x224)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_ADD                    (HOST_IF_BASE_ADDRESS + 0x234)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_SUB                    (HOST_IF_BASE_ADDRESS + 0x244)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_SUB                    (HOST_IF_BASE_ADDRESS + 0x254)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_SUB                    (HOST_IF_BASE_ADDRESS + 0x264)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT                  (HOST_IF_BASE_ADDRESS + 0x270)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT                  (HOST_IF_BASE_ADDRESS + 0x274)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT                  (HOST_IF_BASE_ADDRESS + 0x27C)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_FREED              (HOST_IF_BASE_ADDRESS + 0x280)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_FREED              (HOST_IF_BASE_ADDRESS + 0x284)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_FREED              (HOST_IF_BASE_ADDRESS + 0x288)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_FREED              (HOST_IF_BASE_ADDRESS + 0x28C)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_FREED              (HOST_IF_BASE_ADDRESS + 0x290)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_FREED              (HOST_IF_BASE_ADDRESS + 0x294)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_FREED            (HOST_IF_BASE_ADDRESS + 0x298)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_FREED            (HOST_IF_BASE_ADDRESS + 0x29C)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_FREED            (HOST_IF_BASE_ADDRESS + 0x2A0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2AC)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2B4)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2BC)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2C4)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2CC)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_BIG_END            (HOST_IF_BASE_ADDRESS + 0x2D4)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_BIG_END          (HOST_IF_BASE_ADDRESS + 0x2E0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_BIG_END          (HOST_IF_BASE_ADDRESS + 0x2E8)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_BIG_END          (HOST_IF_BASE_ADDRESS + 0x2F0)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x2F8)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x2FC)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x300)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x304)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x308)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_FREED_BIG_END      (HOST_IF_BASE_ADDRESS + 0x30C)
#define	REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_FREED_BIG_END    (HOST_IF_BASE_ADDRESS + 0x310)
#define	REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_FREED_BIG_END    (HOST_IF_BASE_ADDRESS + 0x314)
#define	REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_FREED_BIG_END    (HOST_IF_BASE_ADDRESS + 0x318)
#define	REG_HOST_IF_TSF_TIMER_LOW                          (HOST_IF_BASE_ADDRESS + 0x31C)
#define	REG_HOST_IF_TSF_TIMER_HIGH                         (HOST_IF_BASE_ADDRESS + 0x320)
#define	REG_HOST_IF_LPI2_INTERRUPT                         (HOST_IF_BASE_ADDRESS + 0x324)
#define	REG_HOST_IF_LPI2_INTERRUPT_SET                     (HOST_IF_BASE_ADDRESS + 0x328)
#define	REG_HOST_IF_LPI2_INTERRUPT_CLEAR                   (HOST_IF_BASE_ADDRESS + 0x32C)
#define	REG_HOST_IF_ENABLE_LPI2_INTERRUPT                  (HOST_IF_BASE_ADDRESS + 0x330)
#define	REG_HOST_IF_CLIENT_3_SMPHR_TOGGLE_31TO00           (HOST_IF_BASE_ADDRESS + 0x340)
#define	REG_HOST_IF_CLIENT_3_SMPHR_TOGGLE_63TO32           (HOST_IF_BASE_ADDRESS + 0x344)
#define	REG_HOST_IF_CLIENT_3_SMPHR_STAT_15TO00             (HOST_IF_BASE_ADDRESS + 0x348)
#define	REG_HOST_IF_CLIENT_3_SMPHR_STAT_31TO16             (HOST_IF_BASE_ADDRESS + 0x34C)
#define	REG_HOST_IF_CLIENT_3_SMPHR_STAT_47TO32             (HOST_IF_BASE_ADDRESS + 0x350)
#define	REG_HOST_IF_CLIENT_3_SMPHR_STAT_63TO48             (HOST_IF_BASE_ADDRESS + 0x354)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOST_IF_WLAN_SECURE_STATUS 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 firewallOn : 1; //Firewall On, reset value: 0x0, access type: RO
		uint32 spiDisable : 1; //SPI disable, reset value: 0x0, access type: RO
		uint32 jtagDisable : 1; //JTAG disable, reset value: 0x0, access type: RO
		uint32 secureBootOn : 1; //Secure boot on, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegHostIfWlanSecureStatus_u;

/*REG_HOST_IF_WLAN_EFUSE_INDICATION 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 chipType : 8; //Chip type, reset value: 0x0, access type: RO
		uint32 hwType : 8; //HW type, reset value: 0x0, access type: RO
		uint32 hwRevision : 8; //HW revision, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfWlanEfuseIndication_u;

/*REG_HOST_IF_HOST_IRQ_STATUS 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountNotEmptyIrq : 1; //Tx data HD count not empty IRQ ,  , , reset value: 0x0, access type: RO
		uint32 rxOutHdCountNotEmptyIrq : 1; //Rx data HD count not empty IRQ, reset value: 0x0, access type: RO
		uint32 txOutMgmtCountNotEmptyIrq : 1; //Tx management HD count not empty IRQ, reset value: 0x0, access type: RO
		uint32 rxOutMgmtCountNotEmptyIrq : 1; //Rx management HD count not empty IRQ, reset value: 0x0, access type: RO
		uint32 macHostMailboxCountNotEmptyIrq : 1; //MAC to host mailbox count not empty IRQ, reset value: 0x0, access type: RO
		uint32 rabPhiInterrupt : 1; //RAB PHI Interrupt, reset value: 0x0, access type: RO
		uint32 wlanMacXbarDmaErrorIrq : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegHostIfHostIrqStatus_u;

/*REG_HOST_IF_HOST_IRQ_MASK 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostIrqMask : 8; //Host IRQ mask. `0` = Mask; `1` = enable , reset value: 0x0, access type: RW
		uint32 socToHostIrqEnable : 4; //bit [0] soc_pvt_irq_enable , bit [1] soc_wdt_irq_enable , bits [2:3] general purpose, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegHostIfHostIrqMask_u;

/*REG_HOST_IF_HOST_LS_CTL_STAT 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostIramWakeCtl : 1; //Override IRAM light sleep control - wake up IRAM if in LS, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 iramLsViolationIndication : 1; //Light sleep violation indication: IRAM was accessed while in LS, reset value: 0x0, access type: RO
		uint32 iramLsViolationIndClear : 1; //LS error indication clear by write, reset value: 0x0, access type: WO
		uint32 reserved1 : 22;
	} bitFields;
} RegHostIfHostLsCtlStat_u;

/*REG_HOST_IF_TX_IN_CORE_INT1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdBufferFreeInt : 1; //TX in HD Buffer is free Interrupt, reset value: 0x1, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegHostIfTxInCoreInt1_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_ADD 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumAdd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_ADD 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountAddNum : 17; //Number of Ready HDs in the Host Ring0 to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumAdd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_ADD 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumAdd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_ADD 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumAdd_u;

/*REG_HOST_IF_RX_IN_MGMT_ACCUM_ADD 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInMgmtCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInMgmtAccumAdd_u;

/*REG_HOST_IF_TX_IN_MGMT_ACCUM_ADD 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInMgmtCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInMgmtAccumAdd_u;

/*REG_HOST_IF_RX_OUT_MGMT_ACCUM_ADD 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutMgmtCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutMgmtAccumAdd_u;

/*REG_HOST_IF_TX_OUT_MGMT_ACCUM_ADD 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutMgmtCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutMgmtAccumAdd_u;

/*REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_ADD 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostMacMailboxCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfHostMacMailboxAccumAdd_u;

/*REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_ADD 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macHostMailboxCountAddNum : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfMacHostMailboxAccumAdd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_SUB 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumSub_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_SUB 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountSubNum : 17; //Number of Ready HDs in the Host Ring0 to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumSub_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_SUB 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumSub_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_SUB 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumSub_u;

/*REG_HOST_IF_RX_IN_MGMT_ACCUM_SUB 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInMgmtCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInMgmtAccumSub_u;

/*REG_HOST_IF_TX_IN_MGMT_ACCUM_SUB 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInMgmtCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInMgmtAccumSub_u;

/*REG_HOST_IF_RX_OUT_MGMT_ACCUM_SUB 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutMgmtCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutMgmtAccumSub_u;

/*REG_HOST_IF_TX_OUT_MGMT_ACCUM_SUB 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutMgmtCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutMgmtAccumSub_u;

/*REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_SUB 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostMacMailboxCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfHostMacMailboxAccumSub_u;

/*REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_SUB 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macHostMailboxCountSubNum : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfMacHostMailboxAccumSub_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_COUNT 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumCount_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_COUNT 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCount : 17; //Number of Ready HDs in the Host Ring0, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumCount_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumCount_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumCount_u;

/*REG_HOST_IF_RX_IN_MGMT_ACCUM_COUNT 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInMgmtCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInMgmtAccumCount_u;

/*REG_HOST_IF_TX_IN_MGMT_ACCUM_COUNT 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInMgmtCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInMgmtAccumCount_u;

/*REG_HOST_IF_RX_OUT_MGMT_ACCUM_COUNT 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutMgmtCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutMgmtAccumCount_u;

/*REG_HOST_IF_TX_OUT_MGMT_ACCUM_COUNT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutMgmtCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutMgmtAccumCount_u;

/*REG_HOST_IF_HOST_MAC_MAILBOX_ACCUM_COUNT 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostMacMailboxCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfHostMacMailboxAccumCount_u;

/*REG_HOST_IF_MAC_HOST_MAILBOX_ACCUM_COUNT 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macHostMailboxCount : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfMacHostMailboxAccumCount_u;

/*REG_HOST_IF_TX_IN_CORE_INT 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 pdThresReachedInt : 1; //PD threshold reached interrupt, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegHostIfTxInCoreInt_u;

/*REG_HOST_IF_TX_OUT_CORE_INT 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutPendingJobsInt : 1; //TX out pending jobs Interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHostIfTxOutCoreInt_u;

/*REG_HOST_IF_RX_IN_CORE_INT 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdBufferFreeInt : 1; //rx in HD Buffer is free Interrupt, reset value: 0x1, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegHostIfRxInCoreInt_u;

/*REG_HOST_IF_RX_OUT_CORE_INT 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutPendingJobsInt : 1; //rx out pending jobs Interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
		uint32 rdThresReachedInt : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegHostIfRxOutCoreInt_u;

/*REG_HOST_IF_HOST_IF_HD_COUNT_INT 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountNotEmptyIrq : 1; //Number of ready HDs at the Host Ring0 is bigger than 0, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 rxInHdCountNotEmptyIrq : 1; //Number of free HDs at the RX path is bigger than 0, reset value: 0x0, access type: RO
		uint32 txInHdCountNotEmptyIrqRing1 : 1; //Number of ready HDs at the Host Ring1 is bigger than 0, reset value: 0x0, access type: RO
		uint32 txInHdCountNotEmptyIrqRing2 : 1; //Number of ready HDs at the Host Ring2 is bigger than 0, reset value: 0x0, access type: RO
		uint32 txInHdCountNotEmptyIrqRing3 : 1; //Number of ready HDs at the Host Ring3 is bigger than 0, reset value: 0x0, access type: RO
		uint32 reserved1 : 26;
	} bitFields;
} RegHostIfHostIfHdCountInt_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_ADD_BIG_END 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountAddNumBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxInHdCountAddNumBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxInHdAccumAddBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_ADD_BIG_END 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountAddNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountAddNumBigEnd : 16; //Number of Ready HDs in the Host Ring0 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccumAddBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_BIG_END 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountAddNumBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxOutHdCountAddNumBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxOutHdAccumAddBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_BIG_END 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountAddNumBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txOutHdCountAddNumBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxOutHdAccumAddBigEnd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_SUB_BIG_END 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountSubNumBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxInHdCountSubNumBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxInHdAccumSubBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_SUB_BIG_END 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountSubNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountSubNumBigEnd : 16; //Number of Ready HDs in the Host Ring0 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccumSubBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_BIG_END 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountSubNumBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxOutHdCountSubNumBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxOutHdAccumSubBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_BIG_END 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountSubNumBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txOutHdCountSubNumBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxOutHdAccumSubBigEnd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_BIG_END 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 rxInHdCountBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfRxInHdAccumCountBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_BIG_END 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountBigEnd : 16; //Number of Ready HDs in the Host Ring0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccumCountBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_BIG_END 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 rxOutHdCountBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfRxOutHdAccumCountBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_BIG_END 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txOutHdCountBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxOutHdAccumCountBigEnd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_ADD_FREED 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCountAddNumFreed : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumAddFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_ADD_FREED 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountAddNumFreed : 17; //Number of Ready HDs in the Host Ring0 to be added to "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumAddFreed_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_READY 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCountAddNumReady : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumAddReady_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_READY 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountAddNumReady : 17; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumAddReady_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_SUB_FREED 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCountSubNumFreed : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumSubFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_SUB_FREED 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountSubNumFreed : 17; //Number of Ready HDs in the Host Ring0 to be removed from "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumSubFreed_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_READY 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCountSubNumReady : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumSubReady_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_READY 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountSubNumReady : 17; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumSubReady_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_FREED 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInHdCountFreed : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxInHdAccumCountFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_FREED 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountFreed : 17; //Number of Ready HDs in the Host Ring 0 in "free" accum, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccumCountFreed_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_READY 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxOutHdCountReady : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfRxOutHdAccumCountReady_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_READY 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txOutHdCountReady : 17; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxOutHdAccumCountReady_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_ADD_FREED_BIG_END 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountAddNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxInHdCountAddNumFreedBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxInHdAccumAddFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_ADD_FREED_BIG_END 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountAddNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountAddNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring0 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccumAddFreedBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_ADD_READY_BIG_END 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountAddNumReadyBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxOutHdCountAddNumReadyBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxOutHdAccumAddReadyBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_ADD_READY_BIG_END 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountAddNumReadyBigEndMsb : 1; //Number of Ready HDs in the Host to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txOutHdCountAddNumReadyBigEnd : 16; //Number of Ready HDs in the Host to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxOutHdAccumAddReadyBigEnd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_SUB_FREED_BIG_END 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountSubNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxInHdCountSubNumFreedBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxInHdAccumSubFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_SUB_FREED_BIG_END 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountSubNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountSubNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring0 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccumSubFreedBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_SUB_READY_BIG_END 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountSubNumReadyBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 rxOutHdCountSubNumReadyBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfRxOutHdAccumSubReadyBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_SUB_READY_BIG_END 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountSubNumReadyBigEndMsb : 1; //Number of Ready HDs in the Host to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txOutHdCountSubNumReadyBigEnd : 16; //Number of Ready HDs in the Host to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxOutHdAccumSubReadyBigEnd_u;

/*REG_HOST_IF_RX_IN_HD_ACCUM_COUNT_FREED_BIG_END 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxInHdCountFreedBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 rxInHdCountFreedBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfRxInHdAccumCountFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM_COUNT_FREED_BIG_END 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring0 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountFreedBigEnd : 16; //Number of Ready HDs in the Host Ring0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccumCountFreedBigEnd_u;

/*REG_HOST_IF_RX_OUT_HD_ACCUM_COUNT_READY_BIG_END 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 rxOutHdCountReadyBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 rxOutHdCountReadyBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfRxOutHdAccumCountReadyBigEnd_u;

/*REG_HOST_IF_TX_OUT_HD_ACCUM_COUNT_READY_BIG_END 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txOutHdCountReadyBigEndMsb : 1; //Number of Ready HDs in the Host - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txOutHdCountReadyBigEnd : 16; //Number of Ready HDs in the Host, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxOutHdAccumCountReadyBigEnd_u;

/*REG_HOST_IF_ENABLE_UPI_INTERRUPT 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableUpiInterrupt : 24; //Enable Upper CPU RAB IRQ, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnableUpiInterrupt_u;

/*REG_HOST_IF_UPI_INTERRUPT 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upiInterrupt : 24; //RAB  upi interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfUpiInterrupt_u;

/*REG_HOST_IF_UPI_INTERRUPT_SET 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upiInterruptSet : 24; //RAB  upi interrupt set, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfUpiInterruptSet_u;

/*REG_HOST_IF_UPI_INTERRUPT_CLEAR 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 upiInterruptClear : 24; //RAB  upi interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfUpiInterruptClear_u;

/*REG_HOST_IF_ENABLE_LPI_INTERRUPT 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableLpiInterrupt : 24; //Enable Lower CPU RAB IRQ, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnableLpiInterrupt_u;

/*REG_HOST_IF_LPI_INTERRUPT 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpiInterrupt : 24; //RAB  lpi interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpiInterrupt_u;

/*REG_HOST_IF_LPI_INTERRUPT_SET 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpiInterruptSet : 24; //RAB  lpi interrupt set, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpiInterruptSet_u;

/*REG_HOST_IF_LPI_INTERRUPT_CLEAR 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpiInterruptClear : 24; //RAB  lpi interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpiInterruptClear_u;

/*REG_HOST_IF_ENABLE_PHI_INTERRUPT 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enablePhiInterrupt : 24; //Enable PHI host RAB IRQ, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnablePhiInterrupt_u;

/*REG_HOST_IF_PHI_INTERRUPT 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phiInterrupt : 24; //RAB  phi interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfPhiInterrupt_u;

/*REG_HOST_IF_PHI_INTERRUPT_SET 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phiInterruptSet : 24; //RAB  phi interrupt set, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfPhiInterruptSet_u;

/*REG_HOST_IF_PHI_INTERRUPT_CLEAR 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phiInterruptClear : 24; //RAB  phi interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfPhiInterruptClear_u;

/*REG_HOST_IF_ENABLE_NPU2UPI_INTERRUPT 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableNpu2UpiInterrupt : 24; //Enable NPU to UPI RAB IRQ, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnableNpu2UpiInterrupt_u;

/*REG_HOST_IF_NPU2UPI_INTERRUPT 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 npu2UpiInterrupt : 24; //RAB NPU to UPI interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfNpu2UpiInterrupt_u;

/*REG_HOST_IF_NPU2UPI_INTERRUPT_SET 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 npu2UpiInterruptSet : 24; //RAB NPU to UPI interrupt set, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfNpu2UpiInterruptSet_u;

/*REG_HOST_IF_NPU2UPI_INTERRUPT_CLEAR 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 npu2UpiInterruptClear : 24; //RAB NPU to UPI interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfNpu2UpiInterruptClear_u;

/*REG_HOST_IF_ENABLE_LPI1_INTERRUPT 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableLpi1Interrupt : 24; //no description, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnableLpi1Interrupt_u;

/*REG_HOST_IF_LPI1_INTERRUPT 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi1Interrupt : 24; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi1Interrupt_u;

/*REG_HOST_IF_LPI1_INTERRUPT_SET 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi1InterruptSet : 24; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi1InterruptSet_u;

/*REG_HOST_IF_LPI1_INTERRUPT_CLEAR 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi1InterruptClear : 24; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi1InterruptClear_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_TOGGLE_31TO00 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrToggle31To00 : 32; //Client 0 Semaphore Toggle of resources 31:0, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient0SmphrToggle31To00_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_TOGGLE_63TO32 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrToggle63To32 : 32; //Client 0 Semaphore Toggle of resources 63:32, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient0SmphrToggle63To32_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_TOGGLE_31TO00 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrToggle31To00 : 32; //Client 1 Semaphore Toggle of resources 31:0, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient1SmphrToggle31To00_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_TOGGLE_63TO32 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrToggle63To32 : 32; //Client 1 Semaphore Toggle of resources 63:32, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient1SmphrToggle63To32_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_TOGGLE_31TO00 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrToggle31To00 : 32; //Client 2 Semaphore Toggle of resources 31:0, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient2SmphrToggle31To00_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_TOGGLE_63TO32 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrToggle63To32 : 32; //Client 2 Semaphore Toggle of resources 63:32, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient2SmphrToggle63To32_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_STAT_15TO00 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrStat15To00 : 32; //Client 0 {Pending, Taken} tuples of resources 15:0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient0SmphrStat15To00_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_STAT_31TO16 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrStat31To16 : 32; //Client 0 {Pending, Taken} tuples of resources 31:16, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient0SmphrStat31To16_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_STAT_47TO32 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrStat47To32 : 32; //Client 0 {Pending, Taken} tuples of resources 47:32, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient0SmphrStat47To32_u;

/*REG_HOST_IF_CLIENT_0_SMPHR_STAT_63TO48 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client0SmphrStat63To48 : 32; //Client 0 {Pending, Taken} tuples of resources 63:48, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient0SmphrStat63To48_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_STAT_15TO00 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrStat15To00 : 32; //Client 1 {Pending, Taken} tuples of resources 15:0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient1SmphrStat15To00_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_STAT_31TO16 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrStat31To16 : 32; //Client 1 {Pending, Taken} tuples of resources 31:16, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient1SmphrStat31To16_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_STAT_47TO32 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrStat47To32 : 32; //Client 1 {Pending, Taken} tuples of resources 47:32, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient1SmphrStat47To32_u;

/*REG_HOST_IF_CLIENT_1_SMPHR_STAT_63TO48 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client1SmphrStat63To48 : 32; //Client 1 {Pending, Taken} tuples of resources 63:48, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient1SmphrStat63To48_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_STAT_15TO00 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrStat15To00 : 32; //Client 2 {Pending, Taken} tuples of resources 15:0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient2SmphrStat15To00_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_STAT_31TO16 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrStat31To16 : 32; //Client 2 {Pending, Taken} tuples of resources 31:16, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient2SmphrStat31To16_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_STAT_47TO32 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrStat47To32 : 32; //Client 2 {Pending, Taken} tuples of resources 47:32, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient2SmphrStat47To32_u;

/*REG_HOST_IF_CLIENT_2_SMPHR_STAT_63TO48 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client2SmphrStat63To48 : 32; //Client 2 {Pending, Taken} tuples of resources 63:48, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient2SmphrStat63To48_u;

/*REG_HOST_IF_UCPU_STAT 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostStartUcpu : 1; //while Upper CPU is inactive write 1 to this bit (self cleared) to activate it., reset value: 0x0, access type: WO
		uint32 ucpuActiveInd : 1; //0b0 - Upper CPU is inactive , 0b1 - Upper CPU is acive, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostIfUcpuStat_u;

/*REG_HOST_IF_NOCS_TIMEOUT_EN 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macNocTimeoutEn : 1; //enables timeout counter of the MAC NOC. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 cpuNocTimeoutEn : 1; //enables timeout counter of the CPU NOC. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 hostIfNocsTimeoutEn : 1; //enables timeout counter of all HostIF uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 txSenderNocsTimeoutEn : 1; //enables timeout counter of all Bands TxSender uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 rxhNocsTimeoutEn : 1; //enables timeout counter of all Bands RxH uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfNocsTimeoutEn_u;

/*REG_HOST_IF_NOCS_FAULT_EN 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macNocFaultEn : 1; //enables error logging of the MAC NOC. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 cpuNocFaultEn : 1; //enables error logging of the CPU NOC. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 hostIfNocsFaultEn : 1; //enables error logging of the HostIF uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 txSenderNocsFaultEn : 1; //enables error logging of all Bands Tx Sender uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 rxhNocsFaultEn : 1; //enables error logging of all Bands RxH uC Cluster NOCs. Set '1' at Init or by the Driver., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostIfNocsFaultEn_u;

/*REG_HOST_IF_NOC_TIMEOUT_DIV_SEL 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 nocTimeoutDivSel : 3; ////PCIe (ocp128) access timeout counter limit selector ('0' = no change = 4k cycles, '1' = 8k cycles timeout, '2'=16k, etc), reset value: 0x2, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegHostIfNocTimeoutDivSel_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_ADD 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1AddNum : 17; //Number of Ready HDs in the Host Ring1 to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1Add_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_ADD 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2AddNum : 17; //Number of Ready HDs in the Host Ring2 to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2Add_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_ADD 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3AddNum : 17; //Number of Ready HDs in the Host Ring3 to be added, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3Add_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_SUB 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1SubNum : 17; //Number of Ready HDs in the Host Ring1 to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1Sub_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_SUB 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2SubNum : 17; //Number of Ready HDs in the Host Ring2 to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2Sub_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_SUB 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3SubNum : 17; //Number of Ready HDs in the Host Ring3 to be removed, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3Sub_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1 : 17; //Number of Ready HDs in the Host Ring1, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1Count_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2 : 17; //Number of Ready HDs in the Host Ring2, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2Count_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3 : 17; //Number of Ready HDs in the Host Ring3, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3Count_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_FREED 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1AddNumFreed : 17; //Number of Ready HDs in the Host Ring1 to be added to "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1AddFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_FREED 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2AddNumFreed : 17; //Number of Ready HDs in the Host Ring2 to be added to "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2AddFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_FREED 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3AddNumFreed : 17; //Number of Ready HDs in the Host Ring3 to be added to "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3AddFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_FREED 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1SubNumFreed : 17; //Number of Ready HDs in the Host Ring1 to be removed from "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1SubFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_FREED 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2SubNumFreed : 17; //Number of Ready HDs in the Host Ring2 to be removed from "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2SubFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_FREED 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3SubNumFreed : 17; //Number of Ready HDs in the Host Ring3 to be removed from "free" accum, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3SubFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_FREED 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing1Freed : 17; //Number of Ready HDs in the Host Ring 0 in "free" accum, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum1CountFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_FREED 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing2Freed : 17; //Number of Ready HDs in the Host Ring 0 in "free" accum, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum2CountFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_FREED 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInHdCountRing3Freed : 17; //Number of Ready HDs in the Host Ring 0 in "free" accum, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegHostIfTxInHdAccum3CountFreed_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_BIG_END 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1AddNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring1 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1AddNumBigEnd : 16; //Number of Ready HDs in the Host Ring1 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum1AddBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_BIG_END 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2AddNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring2 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2AddNumBigEnd : 16; //Number of Ready HDs in the Host Ring2 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum2AddBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_BIG_END 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3AddNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring3 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3AddNumBigEnd : 16; //Number of Ready HDs in the Host Ring3 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum3AddBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_BIG_END 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1SubNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring1 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1SubNumBigEnd : 16; //Number of Ready HDs in the Host Ring1 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum1SubBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_BIG_END 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2SubNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring2 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2SubNumBigEnd : 16; //Number of Ready HDs in the Host Ring2 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum2SubBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_BIG_END 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3SubNumBigEndMsb : 1; //Number of Ready HDs in the Host Ring3 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3SubNumBigEnd : 16; //Number of Ready HDs in the Host Ring3 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum3SubBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_BIG_END 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1BigEndMsb : 1; //Number of Ready HDs in the Host Ring1- MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1BigEnd : 16; //Number of Ready HDs in the Host Ring1, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum1CountBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_BIG_END 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2BigEndMsb : 1; //Number of Ready HDs in the Host Ring2 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2BigEnd : 16; //Number of Ready HDs in the Host Ring2, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum2CountBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_BIG_END 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3BigEndMsb : 1; //Number of Ready HDs in the Host Ring3 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3BigEnd : 16; //Number of Ready HDs in the Host Ring3, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum3CountBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_ADD_FREED_BIG_END 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1AddNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring1 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1AddNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring1 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum1AddFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_ADD_FREED_BIG_END 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2AddNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring2 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2AddNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring2 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum2AddFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_ADD_FREED_BIG_END 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3AddNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring3 to be added - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3AddNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring3 to be added, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum3AddFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_SUB_FREED_BIG_END 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1SubNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring1 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1SubNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring1 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum1SubFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_SUB_FREED_BIG_END 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2SubNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring2 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2SubNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring2 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum2SubFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_SUB_FREED_BIG_END 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3SubNumFreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring3 to be removed - MSB bit, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3SubNumFreedBigEnd : 16; //Number of Ready HDs in the Host Ring3 to be removed, reset value: 0x0, access type: RW
	} bitFields;
} RegHostIfTxInHdAccum3SubFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM1_COUNT_FREED_BIG_END 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing1FreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring1 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing1FreedBigEnd : 16; //Number of Ready HDs in the Host Ring1, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum1CountFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM2_COUNT_FREED_BIG_END 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing2FreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring2 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing2FreedBigEnd : 16; //Number of Ready HDs in the Host Ring2, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum2CountFreedBigEnd_u;

/*REG_HOST_IF_TX_IN_HD_ACCUM3_COUNT_FREED_BIG_END 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 txInHdCountRing3FreedBigEndMsb : 1; //Number of Ready HDs in the Host Ring3 - MSB bit, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 txInHdCountRing3FreedBigEnd : 16; //Number of Ready HDs in the Host Ring3, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTxInHdAccum3CountFreedBigEnd_u;

/*REG_HOST_IF_TSF_TIMER_LOW 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerLow : 32; //Lower 32 bits of the 64b TSF value, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTsfTimerLow_u;

/*REG_HOST_IF_TSF_TIMER_HIGH 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerHigh : 32; //MS 32 bits of the 64b TSF value., reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfTsfTimerHigh_u;

/*REG_HOST_IF_LPI2_INTERRUPT 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi2Interrupt : 24; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi2Interrupt_u;

/*REG_HOST_IF_LPI2_INTERRUPT_SET 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi2InterruptSet : 24; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi2InterruptSet_u;

/*REG_HOST_IF_LPI2_INTERRUPT_CLEAR 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lpi2InterruptClear : 24; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfLpi2InterruptClear_u;

/*REG_HOST_IF_ENABLE_LPI2_INTERRUPT 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableLpi2Interrupt : 24; //no description, reset value: 0xffffff, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegHostIfEnableLpi2Interrupt_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_TOGGLE_31TO00 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrToggle31To00 : 32; //Client 3 Semaphore Toggle of resources 31:0, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient3SmphrToggle31To00_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_TOGGLE_63TO32 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrToggle63To32 : 32; //Client 3 Semaphore Toggle of resources 63:32, reset value: 0x0, access type: WO
	} bitFields;
} RegHostIfClient3SmphrToggle63To32_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_STAT_15TO00 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrStat15To00 : 32; //Client 3 {Pending, Taken} tuples of resources 15:0, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient3SmphrStat15To00_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_STAT_31TO16 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrStat31To16 : 32; //Client 3 {Pending, Taken} tuples of resources 31:16, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient3SmphrStat31To16_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_STAT_47TO32 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrStat47To32 : 32; //Client 3 {Pending, Taken} tuples of resources 47:32, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient3SmphrStat47To32_u;

/*REG_HOST_IF_CLIENT_3_SMPHR_STAT_63TO48 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 client3SmphrStat63To48 : 32; //Client 3 {Pending, Taken} tuples of resources 63:48, reset value: 0x0, access type: RO
	} bitFields;
} RegHostIfClient3SmphrStat63To48_u;



#endif // _HOST_IF_REGS_H_
