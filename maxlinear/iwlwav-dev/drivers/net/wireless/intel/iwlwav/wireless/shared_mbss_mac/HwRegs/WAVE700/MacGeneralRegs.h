
/***********************************************************************************
File:				MacGeneralRegs.h
Module:				macGeneral
SOC Revision:		latest
Generated at:       2022-05-04 10:58:45
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _MAC_GENERAL_REGS_H_
#define _MAC_GENERAL_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define MAC_GENERAL_BASE_ADDRESS                             MEMORY_MAP_UNIT_41_BASE_ADDRESS
#define	REG_MAC_GENERAL_CONFIG_0                                 (MAC_GENERAL_BASE_ADDRESS + 0x0)
#define	REG_MAC_GENERAL_SHRAM_MON_BASE_ADDR                      (MAC_GENERAL_BASE_ADDRESS + 0x4)
#define	REG_MAC_GENERAL_SHRAM_MON_INT_MASK                       (MAC_GENERAL_BASE_ADDRESS + 0x8)
#define	REG_MAC_GENERAL_SHRAM_MON_STATUS_CLEAR                   (MAC_GENERAL_BASE_ADDRESS + 0xC)
#define	REG_MAC_GENERAL_SHRAM_MON_STATUS_REG                     (MAC_GENERAL_BASE_ADDRESS + 0x10)
#define	REG_MAC_GENERAL_SHRAM_MON_CLIENT_AT_IRQ                  (MAC_GENERAL_BASE_ADDRESS + 0x14)
#define	REG_MAC_GENERAL_SHRAM_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x18)
#define	REG_MAC_GENERAL_ROM_FIREWALL_ON                          (MAC_GENERAL_BASE_ADDRESS + 0x1C)
#define	REG_MAC_GENERAL_RETRY_DB_BAND1_SINGLE_MODE               (MAC_GENERAL_BASE_ADDRESS + 0x20)
#define	REG_MAC_GENERAL_PHY_EXT_RESET                            (MAC_GENERAL_BASE_ADDRESS + 0x24)
#define	REG_MAC_GENERAL_LOGGER_FIFOS_SRC_SEL                     (MAC_GENERAL_BASE_ADDRESS + 0x28)
#define	REG_MAC_GENERAL_ENDIAN_SWAP_CTL                          (MAC_GENERAL_BASE_ADDRESS + 0x2C)
#define	REG_MAC_GENERAL_LINK_ADAPTATION_SPARE_REG                (MAC_GENERAL_BASE_ADDRESS + 0x30)
#define	REG_MAC_GENERAL_AHB_ARB_BBCPU_PAGE_REG                   (MAC_GENERAL_BASE_ADDRESS + 0x34)
#define	REG_MAC_GENERAL_AHB_ARB_GENRISC_HOST_PAGE_REG            (MAC_GENERAL_BASE_ADDRESS + 0x38)
#define	REG_MAC_GENERAL_RAM_CTL                                  (MAC_GENERAL_BASE_ADDRESS + 0x3C)
#define	REG_MAC_GENERAL_EFUSE_CTL                                (MAC_GENERAL_BASE_ADDRESS + 0x40)
#define	REG_MAC_GENERAL_HYP_MAC_TOP_GPO_PRE                      (MAC_GENERAL_BASE_ADDRESS + 0x44)
#define	REG_MAC_GENERAL_ACTIVITY_LOGGER_CTL                      (MAC_GENERAL_BASE_ADDRESS + 0x48)
#define	REG_MAC_GENERAL_TIMEOUT_PARAMS_0                         (MAC_GENERAL_BASE_ADDRESS + 0x4C)
#define	REG_MAC_GENERAL_TIMEOUT_PARAMS_1                         (MAC_GENERAL_BASE_ADDRESS + 0x50)
#define	REG_MAC_GENERAL_TIMEOUT_PARAMS_2                         (MAC_GENERAL_BASE_ADDRESS + 0x54)
#define	REG_MAC_GENERAL_TIMEOUT_PARAMS_3                         (MAC_GENERAL_BASE_ADDRESS + 0x58)
#define	REG_MAC_GENERAL_DEFAULT_ROM_RESPONSE                     (MAC_GENERAL_BASE_ADDRESS + 0x5C)
#define	REG_MAC_GENERAL_RAMS1_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x60)
#define	REG_MAC_GENERAL_RAMS2_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x64)
#define	REG_MAC_GENERAL_RAMS3_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x68)
#define	REG_MAC_GENERAL_RAMS4_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x6C)
#define	REG_MAC_GENERAL_RAMS5_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x70)
#define	REG_MAC_GENERAL_RAMS6_RM                                 (MAC_GENERAL_BASE_ADDRESS + 0x74)
#define	REG_MAC_GENERAL_LMAC_START_CTL                           (MAC_GENERAL_BASE_ADDRESS + 0x78)
#define	REG_MAC_GENERAL_CLK_CONTROL_REG_FORCE_ON                 (MAC_GENERAL_BASE_ADDRESS + 0x80)
#define	REG_MAC_GENERAL_CLK_CONTROL_REG_DYN_EN                   (MAC_GENERAL_BASE_ADDRESS + 0x84)
#define	REG_MAC_GENERAL_MAC_PHY_GPIO_OUT_SEL                     (MAC_GENERAL_BASE_ADDRESS + 0x88)
#define	REG_MAC_GENERAL_LMAC0_GPIO_OUT_SEL                       (MAC_GENERAL_BASE_ADDRESS + 0x8C)
#define	REG_MAC_GENERAL_LMAC1_GPIO_OUT_SEL                       (MAC_GENERAL_BASE_ADDRESS + 0x90)
#define	REG_MAC_GENERAL_LMAC2_GPIO_OUT_SEL                       (MAC_GENERAL_BASE_ADDRESS + 0x94)
#define	REG_MAC_GENERAL_POWER_GPIO_CTRL                          (MAC_GENERAL_BASE_ADDRESS + 0xA0)
#define	REG_MAC_GENERAL_SEC_MODE_CTRL                            (MAC_GENERAL_BASE_ADDRESS + 0xB0)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_EXT                       (MAC_GENERAL_BASE_ADDRESS + 0xC0)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_INT                       (MAC_GENERAL_BASE_ADDRESS + 0xC4)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_PCIH_WRITE_CAUSE          (MAC_GENERAL_BASE_ADDRESS + 0xC8)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_PCIH_READ_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xCC)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA0_CAUSE              (MAC_GENERAL_BASE_ADDRESS + 0xD0)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA1_CAUSE              (MAC_GENERAL_BASE_ADDRESS + 0xD4)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA2_CAUSE              (MAC_GENERAL_BASE_ADDRESS + 0xD8)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA0_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xDC)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA1_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xE0)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA2_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xE4)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA0_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xE8)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA1_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xEC)
#define	REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA2_CAUSE           (MAC_GENERAL_BASE_ADDRESS + 0xF0)
#define	REG_MAC_GENERAL_HW_LOGGER_STATUS                         (MAC_GENERAL_BASE_ADDRESS + 0xF4)
#define	REG_MAC_GENERAL_WLAN_IP_BASE_ADDR                        (MAC_GENERAL_BASE_ADDRESS + 0xF8)
#define	REG_MAC_GENERAL_HYP_MAC_TOP_GPR                          (MAC_GENERAL_BASE_ADDRESS + 0xFC)
#define	REG_MAC_GENERAL_HYP_MAC_TOP_GPR1                         (MAC_GENERAL_BASE_ADDRESS + 0x100)
#define	REG_MAC_GENERAL_HYP_MAC_TOP_GPI_INT                      (MAC_GENERAL_BASE_ADDRESS + 0x104)
#define	REG_MAC_GENERAL_UCPU_TIMEOUT_ADDR_L                      (MAC_GENERAL_BASE_ADDRESS + 0x108)
#define	REG_MAC_GENERAL_LCPU0_TIMEOUT_ADDR_L                     (MAC_GENERAL_BASE_ADDRESS + 0x10C)
#define	REG_MAC_GENERAL_LCPU1_TIMEOUT_ADDR_L                     (MAC_GENERAL_BASE_ADDRESS + 0x110)
#define	REG_MAC_GENERAL_LCPU2_TIMEOUT_ADDR_L                     (MAC_GENERAL_BASE_ADDRESS + 0x114)
#define	REG_MAC_GENERAL_PCIH_TIMEOUT_ADDR_L                      (MAC_GENERAL_BASE_ADDRESS + 0x118)
#define	REG_MAC_GENERAL_HOST_GENRISC_TIMEOUT_ADDR_L              (MAC_GENERAL_BASE_ADDRESS + 0x11C)
#define	REG_MAC_GENERAL_B0_RX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x120)
#define	REG_MAC_GENERAL_B0_TX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x124)
#define	REG_MAC_GENERAL_B1_RX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x128)
#define	REG_MAC_GENERAL_B1_TX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x12C)
#define	REG_MAC_GENERAL_B2_RX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x130)
#define	REG_MAC_GENERAL_B2_TX_GENRISC_TIMEOUT_ADDR_L             (MAC_GENERAL_BASE_ADDRESS + 0x134)
#define	REG_MAC_GENERAL_APB_TIMEOUT_LEVEL_EVENTS                 (MAC_GENERAL_BASE_ADDRESS + 0x138)
#define	REG_MAC_GENERAL_APB_TIMEOUT_CLEAR_PULSES                 (MAC_GENERAL_BASE_ADDRESS + 0x13C)
#define	REG_MAC_GENERAL_APB_TIMEOUT_LEVEL_MASK                   (MAC_GENERAL_BASE_ADDRESS + 0x140)
#define	REG_MAC_GENERAL_PHY_ARBITER_GCLK_BYPASS                  (MAC_GENERAL_BASE_ADDRESS + 0x148)
#define	REG_MAC_GENERAL_WL_MAC_GENERAL_IRQ_CLR                   (MAC_GENERAL_BASE_ADDRESS + 0x14C)
#define	REG_MAC_GENERAL_WL_MAC_GENERAL_IRQ_STATUS                (MAC_GENERAL_BASE_ADDRESS + 0x150)
#define	REG_MAC_GENERAL_EXTERNAL_IRQ_INVERT                      (MAC_GENERAL_BASE_ADDRESS + 0x154)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_RELEASE            (MAC_GENERAL_BASE_ADDRESS + 0x158)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_RELEASE           (MAC_GENERAL_BASE_ADDRESS + 0x15C)
#define	REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_RELEASE           (MAC_GENERAL_BASE_ADDRESS + 0x160)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_ASSERT             (MAC_GENERAL_BASE_ADDRESS + 0x164)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_ASSERT            (MAC_GENERAL_BASE_ADDRESS + 0x168)
#define	REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_ASSERT            (MAC_GENERAL_BASE_ADDRESS + 0x16C)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_RD                 (MAC_GENERAL_BASE_ADDRESS + 0x170)
#define	REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_RD                (MAC_GENERAL_BASE_ADDRESS + 0x174)
#define	REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_RD                (MAC_GENERAL_BASE_ADDRESS + 0x178)
#define	REG_MAC_GENERAL_IRAM_LS_CTRL0                            (MAC_GENERAL_BASE_ADDRESS + 0x17C)
#define	REG_MAC_GENERAL_IRAM_LS_CTRL1                            (MAC_GENERAL_BASE_ADDRESS + 0x180)
#define	REG_MAC_GENERAL_IRAM_SLEEP_CTRL                          (MAC_GENERAL_BASE_ADDRESS + 0x184)
#define	REG_MAC_GENERAL_LS_MODE_CONTROL                          (MAC_GENERAL_BASE_ADDRESS + 0x188)
#define	REG_MAC_GENERAL_SPARE_CONFIG_A2D                         (MAC_GENERAL_BASE_ADDRESS + 0x190)
#define	REG_MAC_GENERAL_SPARE_CONFIG_D2A                         (MAC_GENERAL_BASE_ADDRESS + 0x194)
#define	REG_MAC_GENERAL_LS_MODE_DBG_SHARED_RAM                   (MAC_GENERAL_BASE_ADDRESS + 0x200)
#define	REG_MAC_GENERAL_LS_MODE_DBG_DESC_RAM                     (MAC_GENERAL_BASE_ADDRESS + 0x204)
#define	REG_MAC_GENERAL_APB_SNIFFER_CTRL                         (MAC_GENERAL_BASE_ADDRESS + 0x208)
#define	REG_MAC_GENERAL_APB_SNIFFER_ADDR                         (MAC_GENERAL_BASE_ADDRESS + 0x20C)
#define	REG_MAC_GENERAL_APB_SNIFFER_ADDR_MASK                    (MAC_GENERAL_BASE_ADDRESS + 0x210)
#define	REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_SOURCE0_3             (MAC_GENERAL_BASE_ADDRESS + 0x214)
#define	REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_SOURCE4_7             (MAC_GENERAL_BASE_ADDRESS + 0x218)
#define	REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_INPUT                 (MAC_GENERAL_BASE_ADDRESS + 0x220)
#define	REG_MAC_GENERAL_B01_TX_LIB_HALT                          (MAC_GENERAL_BASE_ADDRESS + 0x224)
#define	REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_SOURCE0_3              (MAC_GENERAL_BASE_ADDRESS + 0x230)
#define	REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_SOURCE4_7              (MAC_GENERAL_BASE_ADDRESS + 0x234)
#define	REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_INPUT                  (MAC_GENERAL_BASE_ADDRESS + 0x23C)
#define	REG_MAC_GENERAL_B2_TX_LIB_HALT                           (MAC_GENERAL_BASE_ADDRESS + 0x240)
#define	REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_SOURCE4_7             (MAC_GENERAL_BASE_ADDRESS + 0x250)
#define	REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_INPUT                 (MAC_GENERAL_BASE_ADDRESS + 0x258)
#define	REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_SOURCE0_3             (MAC_GENERAL_BASE_ADDRESS + 0x25C)
#define	REG_MAC_GENERAL_B01_RX_LIB_HALT                          (MAC_GENERAL_BASE_ADDRESS + 0x260)
#define	REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_SOURCE0_3              (MAC_GENERAL_BASE_ADDRESS + 0x264)
#define	REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_SOURCE4_7              (MAC_GENERAL_BASE_ADDRESS + 0x268)
#define	REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_INPUT                  (MAC_GENERAL_BASE_ADDRESS + 0x270)
#define	REG_MAC_GENERAL_B2_RX_LIB_HALT                           (MAC_GENERAL_BASE_ADDRESS + 0x274)
#define	REG_MAC_GENERAL_LIBERATOR_MAX_MESSAGE_TO_LOGGER          (MAC_GENERAL_BASE_ADDRESS + 0x280)
#define	REG_MAC_GENERAL_B01_TX_LIBERATOR_DELAY_VALUE_TO_START    (MAC_GENERAL_BASE_ADDRESS + 0x284)
#define	REG_MAC_GENERAL_B2_TX_LIBERATOR_DELAY_VALUE_TO_START     (MAC_GENERAL_BASE_ADDRESS + 0x288)
#define	REG_MAC_GENERAL_B01_RX_LIBERATOR_DELAY_VALUE_TO_START    (MAC_GENERAL_BASE_ADDRESS + 0x28C)
#define	REG_MAC_GENERAL_B2_RX_LIBERATOR_DELAY_VALUE_TO_START     (MAC_GENERAL_BASE_ADDRESS + 0x290)
#define	REG_MAC_GENERAL_LIBERATOR_DEBUG                          (MAC_GENERAL_BASE_ADDRESS + 0x294)
#define	REG_MAC_GENERAL_LIBERATOR_LOGGER_REG                     (MAC_GENERAL_BASE_ADDRESS + 0x298)
#define	REG_MAC_GENERAL_FCSI_CLK_ABB_PLL                         (MAC_GENERAL_BASE_ADDRESS + 0x2CC)
#define	REG_MAC_GENERAL_FCSI_CLK_XO                              (MAC_GENERAL_BASE_ADDRESS + 0x2D0)
#define	REG_MAC_GENERAL_FCSI_CONTROLS                            (MAC_GENERAL_BASE_ADDRESS + 0x2D4)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_ACCESS                      (MAC_GENERAL_BASE_ADDRESS + 0x2D8)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_WR_DATA                     (MAC_GENERAL_BASE_ADDRESS + 0x2DC)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_RD_DATA                     (MAC_GENERAL_BASE_ADDRESS + 0x2E0)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_BUSY                        (MAC_GENERAL_BASE_ADDRESS + 0x2E4)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_DEBUG                       (MAC_GENERAL_BASE_ADDRESS + 0x2E8)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_RESET_N                     (MAC_GENERAL_BASE_ADDRESS + 0x2EC)
#define	REG_MAC_GENERAL_FCSI_ABB_PLL_FORCE_DEFAULT               (MAC_GENERAL_BASE_ADDRESS + 0x2F0)
#define	REG_MAC_GENERAL_FCSI_XO_RESET_N                          (MAC_GENERAL_BASE_ADDRESS + 0x2F4)
#define	REG_MAC_GENERAL_FCSI_XO_FORCE_DEFAULT                    (MAC_GENERAL_BASE_ADDRESS + 0x2F8)
#define	REG_MAC_GENERAL_PHY_BAND_CONFIG                          (MAC_GENERAL_BASE_ADDRESS + 0x2FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_MAC_GENERAL_CONFIG_0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac1AutoAddrMap : 1; //if this is set then hardware automatically adds 12MB to any access that targets Band0 address space, reset value: 0x0, access type: RW
		uint32 b1GenriscRxAutoAddrMap : 1; //if this is set then hardware automatically adds 12MB to any access that targets Band0 address space, reset value: 0x0, access type: RW
		uint32 b1GenriscTxAutoAddrMap : 1; //if this is set then hardware automatically adds 12MB to any access that targets Band0 address space, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 rxcMem1Band0Usg : 1; //Selects between band0 and band1. If '1' then Band 0 supports up to 36 users., reset value: 0x1, access type: RW
		uint32 txcMem1Band0Usg : 1; //Selects between band0 and band1. If '1' then Band 0 supports up to 36 users., reset value: 0x1, access type: RW
		uint32 reserved1 : 2;
		uint32 band0MinVap : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 band0MinSta : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved3 : 4;
	} bitFields;
} RegMacGeneralConfig0_u;

/*REG_MAC_GENERAL_SHRAM_MON_BASE_ADDR 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramBaseAddr : 24; //Shared RAM monitor base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegMacGeneralShramMonBaseAddr_u;

/*REG_MAC_GENERAL_SHRAM_MON_INT_MASK 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramIntMask : 16; //Shared RAM monitor interrupt mask, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralShramMonIntMask_u;

/*REG_MAC_GENERAL_SHRAM_MON_STATUS_CLEAR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramStatusClear : 16; //Shared RAM monitor status clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralShramMonStatusClear_u;

/*REG_MAC_GENERAL_SHRAM_MON_STATUS_REG 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramStatusReg : 16; //Shared RAM monitor status register, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralShramMonStatusReg_u;

/*REG_MAC_GENERAL_SHRAM_MON_CLIENT_AT_IRQ 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramClientAtIrq : 5; //Shared RAM client at IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegMacGeneralShramMonClientAtIrq_u;

/*REG_MAC_GENERAL_SHRAM_RM 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramRm0 : 3; //Shared RAM RM_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 shramRm1 : 3; //Shared RAM RM_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 shramRm2 : 3; //Shared RAM RM_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 21;
	} bitFields;
} RegMacGeneralShramRm_u;

/*REG_MAC_GENERAL_ROM_FIREWALL_ON 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 romFirewallOn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralRomFirewallOn_u;

/*REG_MAC_GENERAL_RETRY_DB_BAND1_SINGLE_MODE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 retryDbBand1SingleMode : 1; //Retry DB Band 1 mode. 0 - Band 1 is not single mode. It uses TID Indexes [18:35] in Retry DB;   1 - Band 1 is single mode. It uses TID Index 36 in Retry DB., reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralRetryDbBand1SingleMode_u;

/*REG_MAC_GENERAL_PHY_EXT_RESET 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macToPhyBand0ResetN : 1; //Reset PHY for Band 0, reset value: 0x1, access type: RW
		uint32 macToPhyBand1ResetN : 1; //Reset PHY for Band 1, reset value: 0x1, access type: RW
		uint32 macToPhyBand2ResetN : 1; //Reset PHY for Band 2, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegMacGeneralPhyExtReset_u;

/*REG_MAC_GENERAL_LOGGER_FIFOS_SRC_SEL 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo0SrcSel : 4; //FIFO0 source select, reset value: 0x0, access type: RW
		uint32 fifo1SrcSel : 4; //FIFO1 source select, reset value: 0x0, access type: RW
		uint32 fifo2SrcSel : 4; //FIFO2 source select, reset value: 0x0, access type: RW
		uint32 fifo3SrcSel : 4; //FIFO3 source select, reset value: 0x0, access type: RW
		uint32 bandSelB2B0 : 1; //selects Band interfaces connected to Logger FIFOs. , "0" - Band0 Logger ifs connected. , "1" - Band2 Logger ifs connected. , Not real-time!, reset value: 0x0, access type: RW
		uint32 bandSelB2B1 : 1; //selects Band interfaces connected to Logger FIFOs. , "0" - Band1 Logger ifs connected. , "1" - Band2 Logger ifs connected. , Not real-time!, reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegMacGeneralLoggerFifosSrcSel_u;

/*REG_MAC_GENERAL_ENDIAN_SWAP_CTL 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 endianWrCvtFullSwapEna : 1; //PCIH APB byte swapper for write data bus, reset value: 0x0, access type: RW
		uint32 endianRdCvtFullSwapEna : 1; //PCIH APB byte swapper for read data bus, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 axiM0WrSwapMode : 3; //AXI master 0 write data swap mode, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 axiM0RdSwapMode : 3; //AXI master 0 read data swap mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 axiM1WrSwapMode : 3; //AXI master 1 write data swap mode, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
		uint32 axiM1RdSwapMode : 3; //AXI master 1 read data swap mode, reset value: 0x0, access type: RW
		uint32 reserved4 : 1;
		uint32 axiSWrSwapMode : 3; //AXI slave write data swap mode, reset value: 0x0, access type: RW
		uint32 reserved5 : 1;
		uint32 axiSRdSwapMode : 3; //AXI slave read data swap mode, reset value: 0x0, access type: RW
		uint32 reserved6 : 1;
	} bitFields;
} RegMacGeneralEndianSwapCtl_u;

/*REG_MAC_GENERAL_LINK_ADAPTATION_SPARE_REG 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 linkAdaptationSpareReg : 32; //Link adaptation spare register, reset value: 0xfe00, access type: RW
	} bitFields;
} RegMacGeneralLinkAdaptationSpareReg_u;

/*REG_MAC_GENERAL_AHB_ARB_BBCPU_PAGE_REG 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bbcpuPageReg : 16; //BBCPU page register, reset value: 0x0, access type: RW
		uint32 bbcpuPageMask : 16; //BBCPU page mask, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralAhbArbBbcpuPageReg_u;

/*REG_MAC_GENERAL_AHB_ARB_GENRISC_HOST_PAGE_REG 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscHostPageReg : 16; //MAC GenRIsc Host page register, reset value: 0x0, access type: RW
		uint32 genriscHostPageMask : 16; //MAC GenRisc Host page mask, reset value: 0xffff, access type: RW
	} bitFields;
} RegMacGeneralAhbArbGenriscHostPageReg_u;

/*REG_MAC_GENERAL_RAM_CTL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ramTestMode : 1; //RAM test mode (TEST1) , , reset value: 0x0, access type: RW
		uint32 ramTestRnm : 1; //RAM test mode (TEST_RNM) , , reset value: 0x0, access type: RW
		uint32 memGlobalRm : 2; //RAM global RM value , , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegMacGeneralRamCtl_u;

/*REG_MAC_GENERAL_EFUSE_CTL 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseWriteDisable : 1; //no description, reset value: 0x0, access type: WO
		uint32 efuseWriteDisabled : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegMacGeneralEfuseCtl_u;

/*REG_MAC_GENERAL_HYP_MAC_TOP_GPO_PRE 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypMacTopGpoPre : 16; //hyp_mac_top_gpo ,  , , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralHypMacTopGpoPre_u;

/*REG_MAC_GENERAL_ACTIVITY_LOGGER_CTL 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 actLoggerPeriod : 18; //Sets period between samples taken by the logger. Given in system clock cycles , , reset value: 0x27f, access type: RW
		uint32 actLoggerReportLength : 6; //Report length given in activity samples (each is 64 bit) , , reset value: 0x3d, access type: RW
		uint32 actLoggerEnaB0 : 1; //Activity logger enable - band0, reset value: 0x0, access type: RW
		uint32 actLoggerEnaB1 : 1; //Activity logger enable - band1, reset value: 0x0, access type: RW
		uint32 actLoggerPrio : 2; //no description, reset value: 0x0, access type: RW
		uint32 actLoggerEnaB2 : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegMacGeneralActivityLoggerCtl_u;

/*REG_MAC_GENERAL_TIMEOUT_PARAMS_0 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ucpuTimeoutVal : 16; //UCPU timeout value, reset value: 0x1234, access type: RW
		uint32 lcpu0TimeoutVal : 16; //LCPU0 timeout value, reset value: 0x1234, access type: RW
	} bitFields;
} RegMacGeneralTimeoutParams0_u;

/*REG_MAC_GENERAL_TIMEOUT_PARAMS_1 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcpu1TimeoutVal : 16; //LCPU1 timeout value, reset value: 0x1234, access type: RW
		uint32 pcihTimeoutVal : 16; //PCIH timeout value, reset value: 0x1234, access type: RW
	} bitFields;
} RegMacGeneralTimeoutParams1_u;

/*REG_MAC_GENERAL_TIMEOUT_PARAMS_2 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostGenriscTimeoutVal : 16; //Host genrisc timeout value, reset value: 0x1234, access type: RW
		uint32 rxTxGenriscTimeoutVal : 16; //Rx_Tx genrisc timeout value, reset value: 0x1234, access type: RW
	} bitFields;
} RegMacGeneralTimeoutParams2_u;

/*REG_MAC_GENERAL_TIMEOUT_PARAMS_3 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcpu2TimeoutVal : 16; //LCPU2 timeout value, reset value: 0x1234, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralTimeoutParams3_u;

/*REG_MAC_GENERAL_DEFAULT_ROM_RESPONSE 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 defaultRomResponse : 32; //no description, reset value: 0xCAFEFEED, access type: RW
	} bitFields;
} RegMacGeneralDefaultRomResponse_u;

/*REG_MAC_GENERAL_RAMS1_RM 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams1Rm0 : 3; //Various MAC RAMs RM_1_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams1Rm1 : 3; //Various MAC RAMs RM_1_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams1Rm2 : 3; //Various MAC RAMs RM_1_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams1Rm3 : 3; //Various MAC RAMs RM_1_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams1Rm4 : 3; //Various MAC RAMs RM_1_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams1Rm5 : 3; //Various MAC RAMs RM_1_5, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams1Rm6 : 3; //Various MAC RAMs RM_1_6, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams1Rm7 : 3; //Various MAC RAMs RM_1_7, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams1Rm_u;

/*REG_MAC_GENERAL_RAMS2_RM 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams2Rm0 : 3; //Various MAC RAMs RM_2_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams2Rm1 : 3; //Various MAC RAMs RM_2_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams2Rm2 : 3; //Various MAC RAMs RM_2_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams2Rm3 : 3; //Various MAC RAMs RM_2_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams2Rm4 : 3; //Various MAC RAMs RM_2_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams2Rm5 : 3; //Various MAC RAMs RM_2_5, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams2Rm6 : 3; //Various MAC RAMs RM_2_6, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams2Rm7 : 3; //Various MAC RAMs RM_2_7, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams2Rm_u;

/*REG_MAC_GENERAL_RAMS3_RM 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams3Rm0 : 3; //Various MAC RAMs RM_3_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams3Rm1 : 3; //Various MAC RAMs RM_3_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams3Rm2 : 3; //Various MAC RAMs RM_3_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams3Rm3 : 3; //Various MAC RAMs RM_3_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams3Rm4 : 3; //Various MAC RAMs RM_3_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams3Rm5 : 3; //Various MAC RAMs RM_3_5, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams3Rm6 : 3; //Various MAC RAMs RM_3_6, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams3Rm7 : 3; //Various MAC RAMs RM_3_7, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams3Rm_u;

/*REG_MAC_GENERAL_RAMS4_RM 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams4Rm0 : 3; //Various MAC RAMs RM_4_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams4Rm1 : 3; //Various MAC RAMs RM_4_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams4Rm2 : 3; //Various MAC RAMs RM_4_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams4Rm3 : 3; //Various MAC RAMs RM_4_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams4Rm4 : 3; //Various MAC RAMs RM_4_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams4Rm5 : 3; //Various MAC RAMs RM_4_5, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams4Rm6 : 3; //Various MAC RAMs RM_4_6, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams4Rm7 : 3; //Various MAC RAMs RM_4_7, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams4Rm_u;

/*REG_MAC_GENERAL_RAMS5_RM 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams5Rm0 : 3; //Various MAC RAMs RM_5_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams5Rm1 : 3; //Various MAC RAMs RM_5_1, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams5Rm2 : 3; //Various MAC RAMs RM_5_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams5Rm3 : 3; //Various MAC RAMs RM_5_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams5Rm4 : 3; //Various MAC RAMs RM_5_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams5Rm5 : 3; //Various MAC RAMs RM_5_5, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams5Rm6 : 3; //Various MAC RAMs RM_5_6, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams5Rm7 : 3; //Various MAC RAMs RM_5_7, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams5Rm_u;

/*REG_MAC_GENERAL_RAMS6_RM 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams6Rm0 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams6Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams6Rm2 : 3; //Various MAC RAMs RM_6_2, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams6Rm3 : 3; //Various MAC RAMs RM_6_3, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams6Rm4 : 3; //Various MAC RAMs RM_6_4, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams6Rm5 : 3; //RM value for RX_COOR statistics counters RAM, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams6Rm6 : 3; //RM value for mac_xbar reorder buffer memories (see "urfh2ec39a0d" instance name)., reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams6Rm7 : 3; //RM value for mac_xbar DMA buffer memories (see "urfh5fd8eed5_0", "urfh5fd8eed5_1" instance names)., reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegMacGeneralRams6Rm_u;

/*REG_MAC_GENERAL_LMAC_START_CTL 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac0Start : 1; //no description, reset value: 0x0, access type: WO
		uint32 lmac0Status : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 lmac1Start : 1; //no description, reset value: 0x0, access type: WO
		uint32 lmac1Status : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 lmac2Start : 1; //no description, reset value: 0x0, access type: WO
		uint32 lmac2Status : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 22;
	} bitFields;
} RegMacGeneralLmacStartCtl_u;

/*REG_MAC_GENERAL_CLK_CONTROL_REG_FORCE_ON 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clkControlForceOnQManager0 : 1; //Clock Force On Q_MANAGER_0, reset value: 0x0, access type: RW
		uint32 clkControlForceOnQManager1 : 1; //Clock Force On Q_MANAGER_1, reset value: 0x0, access type: RW
		uint32 clkControlForceOnQManager2 : 1; //Clock Force On Q_MANAGER_2, reset value: 0x0, access type: RW
		uint32 clkControlForceOnQManager3 : 1; //Clock Force On Q_MANAGER_3, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm0 : 1; //Clock Force On DLM_0, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm1 : 1; //Clock Force On DLM_1, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm2 : 1; //Clock Force On DLM_2, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm3 : 1; //Clock Force On DLM_3, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm4 : 1; //Clock Force On DLM_4, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm5 : 1; //Clock Force On DLM_5, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm6 : 1; //Clock Force On DLM_6, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm7 : 1; //Clock Force On DLM_7, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDlm8 : 1; //Clock Force On DLM_8, reset value: 0x0, access type: RW
		uint32 clkControlForceOnQManagerAger : 1; //Clock Force On Q_MANAGER_AGER, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxPp0 : 1; //Clock Force On Reserved 0, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxPp1 : 1; //Clock Force On Reserved 1, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDma0 : 1; //Clock Force On DMA_0, reset value: 0x0, access type: RW
		uint32 clkControlForceOnDma1 : 1; //Clock Force On DMA_1, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTxLiberator : 1; //Clock Force On Tx liberator, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxLiberator : 1; //Clock Force On Rx liberator, reset value: 0x0, access type: RW
		uint32 clkControlForceOnProtDb : 1; //Clock Force On PROT_DB, reset value: 0x0, access type: RW
		uint32 clkControlForceOnSemaphore : 1; //Clock Force On Semaphore, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxAddr1ToVapIdx : 1; //Clock Force On Rx address1 to VAP index, reset value: 0x0, access type: RW
		uint32 clkControlForceOnLogger : 1; //Clock Force On logger, reset value: 0x0, access type: RW
		uint32 clkControlForceOnGenriscHost : 1; //Clock Force On GenRisc Host, reset value: 0x0, access type: RW
		uint32 clkControlForceOnStatCnt : 1; //Clock Force On all Statistics counters., reset value: 0x0, access type: RW
		uint32 clkControlForceOnDescRamArb : 1; //Clock Force On for descriptor RAM arbiter , , reset value: 0x1, access type: RW
		uint32 clkControlForceOnSharedRamArb : 1; //Clock Force On for shared RAM arbiter , , reset value: 0x1, access type: RW
		uint32 clkControlForceOnDma2 : 1; //Clock Force On Reserved 11, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTimGen : 1; //Clock Force On TIM_GEN module , , reset value: 0x0, access type: RW
		uint32 clkControlForceOnReserved13 : 1; //Clock Force On Reserved 13, reset value: 0x0, access type: RW
		uint32 clkControlForceOnReserved14 : 1; //Clock Force On Reserved 14, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralClkControlRegForceOn_u;

/*REG_MAC_GENERAL_CLK_CONTROL_REG_DYN_EN 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clkControlDynEnQManager0 : 1; //Clock Dynamic Enable Q_MANAGER_0, reset value: 0x0, access type: RW
		uint32 clkControlDynEnQManager1 : 1; //Clock Dynamic Enable Q_MANAGER_1, reset value: 0x0, access type: RW
		uint32 clkControlDynEnQManager2 : 1; //Clock Dynamic Enable Q_MANAGER_2, reset value: 0x0, access type: RW
		uint32 clkControlDynEnQManager3 : 1; //Clock Dynamic Enable Q_MANAGER_3, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm0 : 1; //Clock Dynamic Enable DLM_0, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm1 : 1; //Clock Dynamic Enable DLM_1, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm2 : 1; //Clock Dynamic Enable DLM_2, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm3 : 1; //Clock Dynamic Enable DLM_3, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm4 : 1; //Clock Dynamic Enable DLM_4, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm5 : 1; //Clock Dynamic Enable DLM_5, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm6 : 1; //Clock Dynamic Enable DLM_6, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm7 : 1; //Clock Dynamic Enable DLM_7, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDlm8 : 1; //Clock Dynamic Enable DLM_8, reset value: 0x0, access type: RW
		uint32 clkControlDynEnQManagerAger : 1; //Clock Dynamic Enable Q_MANAGER_AGER, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxPp0 : 1; //Clock Dynamic Enable Reserved 0, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxPp1 : 1; //Clock Dynamic Enable Reserved 1, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDma0 : 1; //Clock Dynamic Enable DMA_0, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDma1 : 1; //Clock Dynamic Enable DMA_1, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTxLiberator : 1; //Clock Dynamic Enable Tx liberator, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxLiberator : 1; //Clock Dynamic Enable Rx liberator, reset value: 0x0, access type: RW
		uint32 clkControlDynEnProtDb : 1; //Clock Dynamic Enable PROT_DB, reset value: 0x0, access type: RW
		uint32 clkControlDynEnSemaphore : 1; //Clock Dynamic Enable Semaphore, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxAddr1ToVapIdx : 1; //Clock Dynamic Enable Rx address1 to VAP index, reset value: 0x0, access type: RW
		uint32 clkControlDynEnLogger : 1; //Clock Dynamic Enable logger, reset value: 0x0, access type: RW
		uint32 clkControlDynEnGenriscHost : 1; //Clock Enable for HostIF uC Cluster, reset value: 0x1, access type: RW
		uint32 clkControlDynEnStatCnt : 1; //Clock Dynamic Enable all Statistics counters., reset value: 0x0, access type: RW
		uint32 clkControlDynEnDescRamArb : 1; //Clock Dynamic Enable for descriptor RAM arbiter, reset value: 0x0, access type: RW
		uint32 clkControlDynEnSharedRamArb : 1; //Clock Dynamic Enable for sharedRAM arbiter, reset value: 0x0, access type: RW
		uint32 clkControlDynEnDma2 : 1; //Clock Dynamic Enable Reserved 11, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTimGen : 1; //Clock Dynamic Enable TIM_GEN module , , reset value: 0x0, access type: RW
		uint32 clkControlDynEnReserved13 : 1; //Clock Dynamic Enable Reserved 13, reset value: 0x0, access type: RW
		uint32 clkControlDynEnReserved14 : 1; //Clock Dynamic Enable Reserved 14, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralClkControlRegDynEn_u;

/*REG_MAC_GENERAL_MAC_PHY_GPIO_OUT_SEL 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macPhyGpioOutSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralMacPhyGpioOutSel_u;

/*REG_MAC_GENERAL_LMAC0_GPIO_OUT_SEL 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac0GpioOutSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralLmac0GpioOutSel_u;

/*REG_MAC_GENERAL_LMAC1_GPIO_OUT_SEL 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac1GpioOutSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralLmac1GpioOutSel_u;

/*REG_MAC_GENERAL_LMAC2_GPIO_OUT_SEL 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac2GpioOutSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralLmac2GpioOutSel_u;

/*REG_MAC_GENERAL_POWER_GPIO_CTRL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerGpioLowValue : 2; //Low power configuration for GPIOs controlling external power regulator. , Value of 0 means higher voltage, reset value: 0x0, access type: RW
		uint32 powerGpioNominalValue : 2; //Nominal power configuration for GPIOs controlling external power regulator , Value of 0 means higher voltage, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 powerGpioLutSetting : 8; //Lut power setting to control a 8->1 states from PHY's active status, reset value: 0x0, access type: RW
		uint32 powerGpioB0Behavior : 2; //selects behavior of "active" indication from PHY - Band0, reset value: 0x0, access type: RW
		uint32 powerGpioB1Behavior : 2; //selects behavior of "active" indication from PHY - Band1, reset value: 0x0, access type: RW
		uint32 powerGpioB2Behavior : 2; //selects behavior of "active" indication from PHY - Band0, reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
		uint32 powerGpioSel : 1; //select if power logic controls power GPIO (1) or UMAC/LMAC (0), reset value: 0x1, access type: RW
	} bitFields;
} RegMacGeneralPowerGpioCtrl_u;

/*REG_MAC_GENERAL_SEC_MODE_CTRL 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugSecurityMode : 1; //This bit controls the behavior of the security filters during the time that the security firewall selects the Non-secured mode. , 0b0 - no security , 0b1- In this mode the filters acts as they act in stage 2 from IRQ point of view, but the access is indeed complete, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 apbBlockedAccessReturnPslverr : 1; //In secured mode, when a violated access occurs, this bit selects whether to reply with pslverr on APB interface., reset value: 0x0, access type: RW
		uint32 phyAddressSpaceGrey : 1; //Controls the "color" of phy's address space. 0 means grey (read but not write). 1 means black (no read and write, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 macSecurityLevel : 2; //2'b00;       In this mode the filters acts as they act in stage 2 from IRQ point of view, but the access is indeed complete , 2'b01;       Pre UCPU boot                                                                                                  , 2'b10;       Post UCPU boot                                                                                                 , 2'b11;       No security filters                                                                                           , reset value: 0x0, access type: RO
		uint32 reserved2 : 22;
	} bitFields;
} RegMacGeneralSecModeCtrl_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_EXT 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extBlockedAccessIrq : 1; //Main interrupt for any security filter detection generated by PCIh, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 pcihVioWriteAccessIrq : 1; //Interrupt detected for write access from PCI Host, reset value: 0x0, access type: RO
		uint32 pcihVioReadAccessIrq : 1; //Interrupt detected for read access from PCI Host, reset value: 0x0, access type: RO
		uint32 reserved1 : 10;
		uint32 pcihVioWriteAccessIrqClr : 1; //Clear interrupt detected for write access from PCI Host, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 pcihVioReadAccessIrqClr : 1; //Clear interrupt detected for read access from PCI Host, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 reserved2 : 14;
	} bitFields;
} RegMacGeneralSecFilterIrqExt_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_INT 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intBlockedAccessIrq : 1; //Main interrupt for any security filter detection generated by either DMA or Delia masters, reset value: 0x0, access type: RO
		uint32 wrp0Dma0VioOcpAccessIrq : 1; //Interrupt detected for write access from WRP0 DMA0, reset value: 0x0, access type: RO
		uint32 wrp0Dma1VioOcpAccessIrq : 1; //Interrupt detected for write access from WRP0 DMA1, reset value: 0x0, access type: RO
		uint32 wrp0Dma2VioOcpAccessIrq : 1; //Interrupt detected for read access from WRP0 DMA2, reset value: 0x0, access type: RO
		uint32 wrp1Dma0VioOcpAccessIrq : 1; //Interrupt detected for write access from WRP1 DMA0, reset value: 0x0, access type: RO
		uint32 wrp1Dma1VioOcpAccessIrq : 1; //Interrupt detected for write access from WRP1 DMA1, reset value: 0x0, access type: RO
		uint32 wrp1Dma2VioOcpAccessIrq : 1; //Interrupt detected for read access from WRP1 DMA2, reset value: 0x0, access type: RO
		uint32 delia0BlockedAccessIrq : 1; //Interrupt detected for any access from Delia band0, reset value: 0x0, access type: RO
		uint32 delia1BlockedAccessIrq : 1; //Interrupt detected for any access from Delia band1, reset value: 0x0, access type: RO
		uint32 delia2BlockedAccessIrq : 1; //Interrupt detected for any access from Delia band2, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 wrp0Dma0VioOcpAccessIrqClr : 1; //Clear interrupt detected for write access from DMA0, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 wrp0Dma1VioOcpAccessIrqClr : 1; //Clear interrupt detected for write access from DMA1, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 wrp0Dma2VioOcpAccessIrqClr : 1; //Clear interrupt detected for read access from DMA2, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 wrp1Dma0VioOcpAccessIrqClr : 1; //Clear interrupt detected for write access from DMA0, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 wrp1Dma1VioOcpAccessIrqClr : 1; //Clear interrupt detected for write access from DMA1, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 wrp1Dma2VioOcpAccessIrqClr : 1; //Clear interrupt detected for read access from DMA2, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 delia0BlockedAccessIrqClr : 1; //Clear interrupt detected for any access from Delia band0, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 delia1BlockedAccessIrqClr : 1; //Clear interrupt detected for any access from Delia band1, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 delia2BlockedAccessIrqClr : 1; //Clear interrupt detected for any access from Delia band2, this is a self cleared bit, reset value: 0x0, access type: WO
		uint32 reserved1 : 7;
	} bitFields;
} RegMacGeneralSecFilterIrqInt_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_PCIH_WRITE_CAUSE 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcihVioWriteAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 pcihVioWriteSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqPcihWriteCause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_PCIH_READ_CAUSE 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcihVioReadAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 pcihVioReadSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqPcihReadCause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA0_CAUSE 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delia0BlockedAccessAddrFetch : 22; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 delia0MacSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 delia0BlockedAccessWrFetch : 1; //1 - The violation was done by a write access , 0 - The violation was done by a read access, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegMacGeneralSecFilterIrqDelia0Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA1_CAUSE 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delia1BlockedAccessAddrFetch : 22; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 delia1MacSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 delia1BlockedAccessWrFetch : 1; //1 - The violation was done by a write access , 0 - The violation was done by a read access, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegMacGeneralSecFilterIrqDelia1Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_DELIA2_CAUSE 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delia2BlockedAccessAddrFetch : 22; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 delia2MacSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 delia2BlockedAccessWrFetch : 1; //1 - The violation was done by a write access , 0 - The violation was done by a read access, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegMacGeneralSecFilterIrqDelia2Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA0_CAUSE 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp0Dma0VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp0Dma0VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp0Dma0Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA1_CAUSE 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp0Dma1VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp0Dma1VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp0Dma1Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP0_DMA2_CAUSE 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp0Dma2VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp0Dma2VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp0Dma2Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA0_CAUSE 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp1Dma0VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp1Dma0VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp1Dma0Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA1_CAUSE 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp1Dma1VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp1Dma1VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp1Dma1Cause_u;

/*REG_MAC_GENERAL_SEC_FILTER_IRQ_WRP1_DMA2_CAUSE 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wrp1Dma2VioOcpAddrFetch : 26; //Address of the access that caused filter to set violation, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 wrp1Dma2VioOcpSecurityLevelFetch : 2; //Mac security level during the violation.  , 1 - stage1 , 2 - stage2 , 3 - non-secured mode, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegMacGeneralSecFilterIrqWrp1Dma2Cause_u;

/*REG_MAC_GENERAL_HW_LOGGER_STATUS 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 actLoggerRunning0 : 1; //Activity Logger 0 running indication, reset value: 0x0, access type: RO
		uint32 actLoggerRunning1 : 1; //Activity Logger 1 running indication, reset value: 0x0, access type: RO
		uint32 actLoggerRunning2 : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegMacGeneralHwLoggerStatus_u;

/*REG_MAC_GENERAL_WLAN_IP_BASE_ADDR 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wlanIpBaseAddr : 32; //WLAN IP Base address (The configured 7 bit field is located in bits 31:25. All other bits are constant zeros), reset value: 0x18000000, access type: RO
	} bitFields;
} RegMacGeneralWlanIpBaseAddr_u;

/*REG_MAC_GENERAL_HYP_MAC_TOP_GPR 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypMacTopGpr : 32; //hyp_mac_top_gpr , , reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralHypMacTopGpr_u;

/*REG_MAC_GENERAL_HYP_MAC_TOP_GPR1 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypMacTopGpr1 : 32; //hyp_mac_top_gpr1, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralHypMacTopGpr1_u;

/*REG_MAC_GENERAL_HYP_MAC_TOP_GPI_INT 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypMacTopGpiInt : 16; //hyp_mac_top_gpi , , reset value: 0x200, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralHypMacTopGpiInt_u;

/*REG_MAC_GENERAL_UCPU_TIMEOUT_ADDR_L 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ucpuTimeoutAddrL : 32; //UCPU timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralUcpuTimeoutAddrL_u;

/*REG_MAC_GENERAL_LCPU0_TIMEOUT_ADDR_L 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcpu0TimeoutAddrL : 32; //LCPU0 timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralLcpu0TimeoutAddrL_u;

/*REG_MAC_GENERAL_LCPU1_TIMEOUT_ADDR_L 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcpu1TimeoutAddrL : 32; //LCPU1 timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralLcpu1TimeoutAddrL_u;

/*REG_MAC_GENERAL_LCPU2_TIMEOUT_ADDR_L 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcpu2TimeoutAddrL : 32; //LCPU2 timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralLcpu2TimeoutAddrL_u;

/*REG_MAC_GENERAL_PCIH_TIMEOUT_ADDR_L 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcihTimeoutAddrL : 32; //PCIH timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralPcihTimeoutAddrL_u;

/*REG_MAC_GENERAL_HOST_GENRISC_TIMEOUT_ADDR_L 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hostGenriscTimeoutAddrL : 32; //Host Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralHostGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B0_RX_GENRISC_TIMEOUT_ADDR_L 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b0RxGenriscTimeoutAddrL : 32; //Band 0 Rx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB0RxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B0_TX_GENRISC_TIMEOUT_ADDR_L 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b0TxGenriscTimeoutAddrL : 32; //Band 0 Tx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB0TxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B1_RX_GENRISC_TIMEOUT_ADDR_L 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b1RxGenriscTimeoutAddrL : 32; //Band 1 Rx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB1RxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B1_TX_GENRISC_TIMEOUT_ADDR_L 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b1TxGenriscTimeoutAddrL : 32; //Band 1 Tx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB1TxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B2_RX_GENRISC_TIMEOUT_ADDR_L 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxGenriscTimeoutAddrL : 32; //Band 2 Rx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB2RxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_B2_TX_GENRISC_TIMEOUT_ADDR_L 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxGenriscTimeoutAddrL : 32; //Band 2 Tx Genrisc timeout address, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralB2TxGenriscTimeoutAddrL_u;

/*REG_MAC_GENERAL_APB_TIMEOUT_LEVEL_EVENTS 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 apbTimeoutLevelEvents : 12; //Read event of timeout towards this master , 1'b1 - timeout event occurred , 1'b0 - timeout event didn't occurred or was cleared ,  , bit 0 - ucpu          , bit 1 - lcpu0         , bit 2 - lcpu1         , bit 3 - pcih          , bit 4 - host_genrisc  , bit 5 - b0_rx_genrisc , bit 6 - b0_tx_genrisc , bit 7 - b1_rx_genrisc , bit 8 - b1_tx_genrisc, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralApbTimeoutLevelEvents_u;

/*REG_MAC_GENERAL_APB_TIMEOUT_CLEAR_PULSES 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 apbTimeoutClearPulses : 12; //write 1'b1 to clear apb timeout event interrupt for a specific module. It is possible to write to multi bits at a time to clear multiple interrupts. ,  , bit 0 - ucpu          , bit 1 - lcpu0         , bit 2 - lcpu1         , bit 3 - pcih          , bit 4 - host_genrisc  , bit 5 - b0_rx_genrisc , bit 6 - b0_tx_genrisc , bit 7 - b1_rx_genrisc , bit 8 - b1_tx_genrisc, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralApbTimeoutClearPulses_u;

/*REG_MAC_GENERAL_APB_TIMEOUT_LEVEL_MASK 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 apbTimeoutLevelMask : 12; //Mask the timeout event of a master from the apb timeout interrupt ,  , 1'b1 - interrupt is masked for this master , 1'b0 - interrupt is enabled for this master ,  , bit 0 - ucpu          , bit 1 - lcpu0         , bit 2 - lcpu1         , bit 3 - pcih          , bit 4 - host_genrisc  , bit 5 - b0_rx_genrisc , bit 6 - b0_tx_genrisc , bit 7 - b1_rx_genrisc , bit 8 - b1_tx_genrisc, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralApbTimeoutLevelMask_u;

/*REG_MAC_GENERAL_PHY_ARBITER_GCLK_BYPASS 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyArbiterGclkBypass : 1; //Phy arbiter gclk bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralPhyArbiterGclkBypass_u;

/*REG_MAC_GENERAL_WL_MAC_GENERAL_IRQ_CLR 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 externalIrqClr : 4; //External IRQ clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegMacGeneralWlMacGeneralIrqClr_u;

/*REG_MAC_GENERAL_WL_MAC_GENERAL_IRQ_STATUS 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 externalIrqStatus : 4; //External IRQ status, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegMacGeneralWlMacGeneralIrqStatus_u;

/*REG_MAC_GENERAL_EXTERNAL_IRQ_INVERT 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 externalIrqInvert : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegMacGeneralExternalIrqInvert_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_RELEASE 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseDataLogger : 1; //DATA LOGGER SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxAddr1ToVapIdx : 1; //Rx address1 to VAP index SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseProtDb : 1; //PROT_DB SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxPp : 1; //RX_PP SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDmacWrapper : 1; //DMAC_WRAPPER SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseHostGenrisc : 1; //HOST GENRISC SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTrafficIndicator : 1; //Traffic indicator SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseSemaphore : 1; //SEMAPHORE SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseHostIf : 5; //HOST_IF SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxLiberator : 1; //TX_LIBERATOR SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxLiberator : 1; //RX_LIBERATOR SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseSmcMem : 1; //smc_mem SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralPasSwResetSyncRegRelease_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_RELEASE 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseQManagerAger : 1; //Q_MANAGER Ager SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseQManagerDlm : 10; //Q_MANAGER DLMs SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTimGen : 1; //TIM_GEN SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralPasSwResetSyncReg2Release_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_RELEASE 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseDataLoggerReg : 1; //DATA LOGGER reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxAddr1ToVapIdxReg : 1; //Rx address1 to VAP index reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseProtDbReg : 1; //PROT_DB reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxPpReg : 1; //RX_PP reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDmacWrapperReg : 1; //DMAC_WRAPPER reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDma0 : 1; //ARM DMA0 SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDma1 : 1; //ARM DMA1 SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDma2 : 1; //no description, reset value: 0x0, access type: WO
		uint32 swResetReleaseHostIfReg : 1; //HOST_IF reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseQManagerReg : 9; //Q_MANAGER reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTimGenReg : 1; //TIM_GEN reg file SW enable release, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 13;
	} bitFields;
} RegMacGeneralPasSwResetAsyncRegRelease_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_ASSERT 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertDataLogger : 1; //DATA LOGGER SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxAddr1ToVapIdx : 1; //Rx address1 to VAP index SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertProtDb : 1; //PROT_DB SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxPp : 1; //RX_PP SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDmacWrapper : 1; //DMAC_WRAPPER SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertHostGenrisc : 1; //HOST GENRISC SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTrafficIndicator : 1; //Traffic indicator SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertSemaphore : 1; //SEMAPHORE SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertHostIf : 5; //HOST_IF SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxLiberator : 1; //TX_LIBERATOR SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxLiberator : 1; //RX_LIBERATOR SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertSmcMem : 1; //smc_mem SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralPasSwResetSyncRegAssert_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_ASSERT 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertQManagerAger : 1; //Q_MANAGER Ager SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertQManagerDlm : 10; //Q_MANAGER DLMs SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTimGen : 1; //TIM_GEN SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralPasSwResetSyncReg2Assert_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_ASSERT 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertDataLoggerReg : 1; //DATA LOGGER reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxAddr1ToVapIdxReg : 1; //Rx address1 to VAP index reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertProtDbReg : 1; //PROT_DB reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxPpReg : 1; //RX_PP reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDmacWrapperReg : 1; //DMAC_WRAPPER reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDma0 : 1; //ARM DMA0 SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDma1 : 1; //ARM DMA1 SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDma2 : 1; //no description, reset value: 0x0, access type: WO
		uint32 swResetAssertHostIfReg : 1; //HOST_IF reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertQManagerReg : 9; //Q_MANAGER reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTimGenReg : 1; //TIM_GEN reg file SW enable Assert, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 13;
	} bitFields;
} RegMacGeneralPasSwResetAsyncRegAssert_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG_RD 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdDataLogger : 1; //DATA LOGGER SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxAddr1ToVapIdx : 1; //Rx address1 to VAP index SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdProtDb : 1; //PROT_DB SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxPp : 1; //RX_PP SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDmacWrapper : 1; //DMAC_WRAPPER SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdHostGenrisc : 1; //HOST GENRISC SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTrafficIndicator : 1; //Traffic indicator SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdSemaphore : 1; //SEMAPHORE SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdHostIf : 5; //HOST_IF SW enable rd, reset value: 0x1f, access type: RO
		uint32 swResetRdTxLiberator : 1; //TX_LIBERATOR SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxLiberator : 1; //RX_LIBERATOR SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdSmcMem : 1; //smc_mem SW enable rd, reset value: 0x1, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralPasSwResetSyncRegRd_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_SYNC_REG2_RD 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdQManagerAger : 1; //Q_MANAGER Ager SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdQManagerDlm : 10; //Q_MANAGER DLMs SW enable rd, reset value: 0x3ff, access type: RO
		uint32 swResetRdTimGen : 1; //TIM_GEN SW enable rd, reset value: 0x1, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegMacGeneralPasSwResetSyncReg2Rd_u;

/*REG_MAC_GENERAL_PAS_SW_RESET_ASYNC_REG_RD 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdDataLoggerReg : 1; //DATA LOGGER reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxAddr1ToVapIdxReg : 1; //Rx address1 to VAP index reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdProtDbReg : 1; //PROT_DB reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxPpReg : 1; //RX_PP reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDmacWrapperReg : 1; //DMAC_WRAPPER reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDma0 : 1; //ARM DMA0 SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDma1 : 1; //ARM DMA1 SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDma2 : 1; //no description, reset value: 0x1, access type: RO
		uint32 swResetRdHostIfReg : 1; //HOST_IF reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdQManagerReg : 9; //Q_MANAGER reg file SW enable rd, reset value: 0x1ff, access type: RO
		uint32 swResetRdTimGenReg : 1; //TIM_GEN reg file SW enable rd, reset value: 0x1, access type: RO
		uint32 reserved0 : 13;
	} bitFields;
} RegMacGeneralPasSwResetAsyncRegRd_u;

/*REG_MAC_GENERAL_IRAM_LS_CTRL0 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lmac0IramBlockUsed : 10; //no description, reset value: 0x0, access type: RW
		uint32 lmac1IramBlockUsed : 10; //no description, reset value: 0x0, access type: RW
		uint32 lmac2IramBlockUsed : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegMacGeneralIramLsCtrl0_u;

/*REG_MAC_GENERAL_IRAM_LS_CTRL1 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacIramBlockUsed : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegMacGeneralIramLsCtrl1_u;

/*REG_MAC_GENERAL_IRAM_SLEEP_CTRL 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLsModeEnIram : 10; //no description, reset value: 0x0, access type: RW
		uint32 swDsModeEnIram : 10; //no description, reset value: 0x0, access type: RW
		uint32 swShutDownIram : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegMacGeneralIramSleepCtrl_u;

/*REG_MAC_GENERAL_LS_MODE_CONTROL 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLsModeEnDescRam : 1; //Enable the LS mode mechanism for descriptor RAM, reset value: 0x0, access type: RW
		uint32 swLsModeEnSharedRam : 1; //Enable the LS mode mechanism for shared RAM, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 swLsModeTargetDescRam : 4; //This number controls the number of cycles to wait after any activity to the descriptor RAM before entering LS mode. The real counted number will be this nuber times 8, reset value: 0x8, access type: RW
		uint32 reserved1 : 4;
		uint32 swLsModeTargetSharedRam : 4; //This number controls the number of cycles to wait after any activity to the shared RAM before entering LS mode. The real counted number will be this nuber times 8, reset value: 0x8, access type: RW
		uint32 swLsModeEnaArcEm : 1; //when '1': enables to ARC_EM (e.g. HostIF uC) to control automatically light-sleep of all its memories., reset value: 0x0, access type: RW
		uint32 reserved2 : 11;
	} bitFields;
} RegMacGeneralLsModeControl_u;

/*REG_MAC_GENERAL_SPARE_CONFIG_A2D 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareConfigA2D : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegMacGeneralSpareConfigA2D_u;

/*REG_MAC_GENERAL_SPARE_CONFIG_D2A 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareConfigD2A : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegMacGeneralSpareConfigD2A_u;

/*REG_MAC_GENERAL_LS_MODE_DBG_SHARED_RAM 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgLsModeEnSharedRam : 1; //Enable the debug counters during LS mode in shared RAM. This mechanism should be active only in case the LS mode mecanim is enabled in the shared RAM, reset value: 0x0, access type: RW
		uint32 swDbgLsModeStartPulseSharedRam : 1; //clear the RO counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
		uint32 swDbgLsModeCntTimeSharedRam : 18; //Counts the clock cycles when LS mode is active. When reached maximum value this counter stops to count and also stalls the other counter, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 swDbgLsModeCntToggleSharedRam : 9; //Counts amount of LS mode is activations. When reached maximum value this counter stops to count and also stalls the other counter, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralLsModeDbgSharedRam_u;

/*REG_MAC_GENERAL_LS_MODE_DBG_DESC_RAM 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgLsModeEnDescRam : 1; //Enable the debug counters during LS mode in descriptor RAM. This mechanism should be active only in case the LS mode mecanim is enabled in the descriptor RAM, reset value: 0x0, access type: RW
		uint32 swDbgLsModeStartPulseDescRam : 1; //clear the RO counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 2;
		uint32 swDbgLsModeCntTimeDescRam : 18; //Counts the clock cycles when LS mode is active. When reached maximum value this counter stops to count and also stalls the other counter, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 swDbgLsModeCntToggleDescRam : 9; //Counts amount of LS mode is activations. When reached maximum value this counter stops to count and also stalls the other counter, reset value: 0x0, access type: RO
	} bitFields;
} RegMacGeneralLsModeDbgDescRam_u;

/*REG_MAC_GENERAL_APB_SNIFFER_CTRL 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swCfgApbSniffAccessType : 2; //bit 0 tells if we hit for write. Bit 1 tells if we hit for read. Write 2'b11 to catch both. Write 2'b00 to totally disable this mechnism., reset value: 0x0, access type: RW
		uint32 swCfgApbSniffConsiderPslverr : 1; //0 - ignore PSLVERR in the response. 1 - in case PSLVERR is set, don't sniff this transaction, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 swCfgApbSniffClr : 1; //Pulse. Clear the sticky indications, reset value: 0x0, access type: WO
		uint32 reserved1 : 11;
		uint32 apbSniffDetectedLevel : 12; //Indicates which master had performed the access. These bits are sticky. Master's list is implemented in apb_xbar_mac_top module, reset value: 0x0, access type: RO
		uint32 reserved2 : 4;
	} bitFields;
} RegMacGeneralApbSnifferCtrl_u;

/*REG_MAC_GENERAL_APB_SNIFFER_ADDR 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swCfgApbSniffAddressToHit : 32; //32 bits address to perrom the hit, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralApbSnifferAddr_u;

/*REG_MAC_GENERAL_APB_SNIFFER_ADDR_MASK 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swCfgApbSniffAddressMask : 32; //32 bits mask to mask required address and the address generated by the master. Only the bits that are high in the mask will participate in the comparison., reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralApbSnifferAddrMask_u;

/*REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_SOURCE0_3 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorDplIdxSource0 : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 b01TxLiberatorDplIdxSource1 : 7; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 b01TxLiberatorDplIdxSource2 : 7; //no description, reset value: 0x2, access type: RW
		uint32 reserved2 : 1;
		uint32 b01TxLiberatorDplIdxSource3 : 7; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB01TxLibDplIdxSource03_u;

/*REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_SOURCE4_7 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorDplIdxSource4 : 7; //no description, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 b01TxLiberatorDplIdxSource5 : 7; //no description, reset value: 0x5, access type: RW
		uint32 reserved1 : 1;
		uint32 b01TxLiberatorDplIdxSource6 : 7; //no description, reset value: 0x6, access type: RW
		uint32 reserved2 : 1;
		uint32 b01TxLiberatorDplIdxSource7 : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB01TxLibDplIdxSource47_u;

/*REG_MAC_GENERAL_B01_TX_LIB_DPL_IDX_INPUT 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorDplIdxIn : 6; //no description, reset value: 0x9, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacGeneralB01TxLibDplIdxInput_u;

/*REG_MAC_GENERAL_B01_TX_LIB_HALT 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorHaltWork : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralB01TxLibHalt_u;

/*REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_SOURCE0_3 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxLiberatorDplIdxSource0 : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 b2TxLiberatorDplIdxSource1 : 7; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 b2TxLiberatorDplIdxSource2 : 7; //no description, reset value: 0x2, access type: RW
		uint32 reserved2 : 1;
		uint32 b2TxLiberatorDplIdxSource3 : 7; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB2TxLibDplIdxSource03_u;

/*REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_SOURCE4_7 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxLiberatorDplIdxSource4 : 7; //no description, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 b2TxLiberatorDplIdxSource5 : 7; //no description, reset value: 0x5, access type: RW
		uint32 reserved1 : 1;
		uint32 b2TxLiberatorDplIdxSource6 : 7; //no description, reset value: 0x6, access type: RW
		uint32 reserved2 : 1;
		uint32 b2TxLiberatorDplIdxSource7 : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB2TxLibDplIdxSource47_u;

/*REG_MAC_GENERAL_B2_TX_LIB_DPL_IDX_INPUT 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxLiberatorDplIdxIn : 6; //no description, reset value: 0x9, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacGeneralB2TxLibDplIdxInput_u;

/*REG_MAC_GENERAL_B2_TX_LIB_HALT 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxLiberatorHaltWork : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralB2TxLibHalt_u;

/*REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_SOURCE4_7 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01RxLiberatorDplIdxSource4 : 7; //no description, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 b01RxLiberatorDplIdxSource5 : 7; //no description, reset value: 0x5, access type: RW
		uint32 reserved1 : 1;
		uint32 b01RxLiberatorDplIdxSource6 : 7; //no description, reset value: 0x6, access type: RW
		uint32 reserved2 : 1;
		uint32 b01RxLiberatorDplIdxSource7 : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB01RxLibDplIdxSource47_u;

/*REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_INPUT 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01RxLiberatorDplIdxIn : 6; //no description, reset value: 0x9, access type: RW
		uint32 reserved0 : 2;
		uint32 b01RxLiberatorDplIdxToAdd : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegMacGeneralB01RxLibDplIdxInput_u;

/*REG_MAC_GENERAL_B01_RX_LIB_DPL_IDX_SOURCE0_3 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01RxLiberatorDplIdxSource0 : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 b01RxLiberatorDplIdxSource1 : 7; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 b01RxLiberatorDplIdxSource2 : 7; //no description, reset value: 0x2, access type: RW
		uint32 reserved2 : 1;
		uint32 b01RxLiberatorDplIdxSource3 : 7; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB01RxLibDplIdxSource03_u;

/*REG_MAC_GENERAL_B01_RX_LIB_HALT 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01RxLiberatorHaltWork : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralB01RxLibHalt_u;

/*REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_SOURCE0_3 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxLiberatorDplIdxSource0 : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 b2RxLiberatorDplIdxSource1 : 7; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 b2RxLiberatorDplIdxSource2 : 7; //no description, reset value: 0x2, access type: RW
		uint32 reserved2 : 1;
		uint32 b2RxLiberatorDplIdxSource3 : 7; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB2RxLibDplIdxSource03_u;

/*REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_SOURCE4_7 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxLiberatorDplIdxSource4 : 7; //no description, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 b2RxLiberatorDplIdxSource5 : 7; //no description, reset value: 0x5, access type: RW
		uint32 reserved1 : 1;
		uint32 b2RxLiberatorDplIdxSource6 : 7; //no description, reset value: 0x6, access type: RW
		uint32 reserved2 : 1;
		uint32 b2RxLiberatorDplIdxSource7 : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegMacGeneralB2RxLibDplIdxSource47_u;

/*REG_MAC_GENERAL_B2_RX_LIB_DPL_IDX_INPUT 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxLiberatorDplIdxIn : 6; //no description, reset value: 0x9, access type: RW
		uint32 reserved0 : 2;
		uint32 b2RxLiberatorDplIdxToAdd : 7; //no description, reset value: 0x7, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegMacGeneralB2RxLibDplIdxInput_u;

/*REG_MAC_GENERAL_B2_RX_LIB_HALT 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxLiberatorHaltWork : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralB2RxLibHalt_u;

/*REG_MAC_GENERAL_LIBERATOR_MAX_MESSAGE_TO_LOGGER 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorMaxMessageToLogger : 8; //no description, reset value: 0x20, access type: RW
		uint32 b01RxLiberatorMaxMessageToLogger : 8; //no description, reset value: 0x20, access type: RW
		uint32 b2TxLiberatorMaxMessageToLogger : 8; //no description, reset value: 0x20, access type: RW
		uint32 b2RxLiberatorMaxMessageToLogger : 8; //no description, reset value: 0x20, access type: RW
	} bitFields;
} RegMacGeneralLiberatorMaxMessageToLogger_u;

/*REG_MAC_GENERAL_B01_TX_LIBERATOR_DELAY_VALUE_TO_START 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorDelayValueToStart : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralB01TxLiberatorDelayValueToStart_u;

/*REG_MAC_GENERAL_B2_TX_LIBERATOR_DELAY_VALUE_TO_START 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2TxLiberatorDelayValueToStart : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralB2TxLiberatorDelayValueToStart_u;

/*REG_MAC_GENERAL_B01_RX_LIBERATOR_DELAY_VALUE_TO_START 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01RxLiberatorDelayValueToStart : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralB01RxLiberatorDelayValueToStart_u;

/*REG_MAC_GENERAL_B2_RX_LIBERATOR_DELAY_VALUE_TO_START 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b2RxLiberatorDelayValueToStart : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegMacGeneralB2RxLiberatorDelayValueToStart_u;

/*REG_MAC_GENERAL_LIBERATOR_DEBUG 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorWorking : 1; //no description, reset value: 0x0, access type: RO
		uint32 b01RxLiberatorWorking : 1; //no description, reset value: 0x0, access type: RO
		uint32 b2TxLiberatorWorking : 1; //no description, reset value: 0x0, access type: RO
		uint32 b2RxLiberatorWorking : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegMacGeneralLiberatorDebug_u;

/*REG_MAC_GENERAL_LIBERATOR_LOGGER_REG 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b01TxLiberatorLoggerRegEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 b01TxLiberatorLoggerRegPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 b01TxLiberatorLoggerRegActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 b01RxLiberatorLoggerRegEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 b01RxLiberatorLoggerRegPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 b01RxLiberatorLoggerRegActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 b2TxLiberatorLoggerRegEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 b2TxLiberatorLoggerRegPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 b2TxLiberatorLoggerRegActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 b2RxLiberatorLoggerRegEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 b2RxLiberatorLoggerRegPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 b2RxLiberatorLoggerRegActive : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegMacGeneralLiberatorLoggerReg_u;

/*REG_MAC_GENERAL_FCSI_CLK_ABB_PLL 0x2CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiClkWrDivrAbbPll : 6; //fcsi_clk_wr_divr_abb_pll, reset value: 0x4, access type: RW
		uint32 reserved0 : 2;
		uint32 fcsiClkRdDivrAbbPll : 6; //fcsi_clk_rd_divr_abb_pll, reset value: 0xf, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegMacGeneralFcsiClkAbbPll_u;

/*REG_MAC_GENERAL_FCSI_CLK_XO 0x2D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiClkWrDivrXo : 6; //fcsi_clk_wr_divr_xo, reset value: 0x4, access type: RW
		uint32 reserved0 : 2;
		uint32 fcsiClkRdDivrXo : 6; //fcsi_clk_rd_divr_xo, reset value: 0xf, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegMacGeneralFcsiClkXo_u;

/*REG_MAC_GENERAL_FCSI_CONTROLS 0x2D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiModeAbbPll : 1; //fcsi_mode_abb_pll, reset value: 0x1, access type: RW
		uint32 fcsiModeXo : 1; //fcsi_mode_xo, reset value: 0x1, access type: RW
		uint32 reserved0 : 6;
		uint32 fcsiAdditionalCycles : 4; //fcsi_additional_cycles, reset value: 0x0, access type: RW
		uint32 reserved1 : 20;
	} bitFields;
} RegMacGeneralFcsiControls_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_ACCESS 0x2D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllAddr : 7; //fcsi_abb_pll_addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 fcsiAbbPllRdWr : 1; //fcsi_abb_pll_rd_wr, reset value: 0x0, access type: RW
		uint32 fcsiAbbPllGo : 1; //fcsi_abb_pll_go, reset value: 0x0, access type: WO
		uint32 reserved1 : 16;
	} bitFields;
} RegMacGeneralFcsiAbbPllAccess_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_WR_DATA 0x2DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllWrData : 16; //fcsi_abb_pll_wr_data, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralFcsiAbbPllWrData_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_RD_DATA 0x2E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllRdData : 16; //fcsi_abb_pll_rd_data, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralFcsiAbbPllRdData_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_BUSY 0x2E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllBusy : 1; //fcsi_abb_pll_busy, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralFcsiAbbPllBusy_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_DEBUG 0x2E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllDebug : 16; //fcsi_abb_pll_debug, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegMacGeneralFcsiAbbPllDebug_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_RESET_N 0x2EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllResetN : 1; //fcsi_abb_pll_reset_n, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralFcsiAbbPllResetN_u;

/*REG_MAC_GENERAL_FCSI_ABB_PLL_FORCE_DEFAULT 0x2F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiAbbPllForceDefault : 1; //fcsi_abb_pll_force_default, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralFcsiAbbPllForceDefault_u;

/*REG_MAC_GENERAL_FCSI_XO_RESET_N 0x2F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiXoResetN : 1; //fcsi_xo_reset_n, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralFcsiXoResetN_u;

/*REG_MAC_GENERAL_FCSI_XO_FORCE_DEFAULT 0x2F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsiXoForceDefault : 1; //fcsi_xo_force_default, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacGeneralFcsiXoForceDefault_u;

/*REG_MAC_GENERAL_PHY_BAND_CONFIG 0x2FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b0PhyActive : 1; //Band 0's PHY active, reset value: 0x0, access type: RW
		uint32 b1PhyActive : 1; //Band 1's PHY active, reset value: 0x0, access type: RW
		uint32 b2PhyActive : 1; //Band 2's PHY active, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 fcsiCenResetN : 1; //Central FCSI reset, reset value: 0x0, access type: RW
		uint32 fcsiCenBlockEn : 1; //Central FCSI block enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegMacGeneralPhyBandConfig_u;



#endif // _MAC_GENERAL_REGS_H_
