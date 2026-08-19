
/***********************************************************************************
File:				SocRegsRegs.h
Module:				socRegs
SOC Revision:		latest
Generated at:       2022-05-04 10:58:48
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _SOC_REGS_REGS_H_
#define _SOC_REGS_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define SOC_REGS_BASE_ADDRESS                             MEMORY_MAP_UNIT_59_BASE_ADDRESS
#define	REG_SOC_REGS_RESERVE_LOC0                    (SOC_REGS_BASE_ADDRESS + 0x0)
#define	REG_SOC_REGS_SOFT_RESET                      (SOC_REGS_BASE_ADDRESS + 0x4)
#define	REG_SOC_REGS_SOFT_RESET_STROBE_ENABLE        (SOC_REGS_BASE_ADDRESS + 0x8)
#define	REG_SOC_REGS_RCU_BOOT                        (SOC_REGS_BASE_ADDRESS + 0xC)
#define	REG_SOC_REGS_GPIO_PULLUP                     (SOC_REGS_BASE_ADDRESS + 0x10)
#define	REG_SOC_REGS_GPIO_PULLDN                     (SOC_REGS_BASE_ADDRESS + 0x14)
#define	REG_SOC_REGS_GPIO_SLEW_CTRL                  (SOC_REGS_BASE_ADDRESS + 0x18)
#define	REG_SOC_REGS_GPIO_DRIVE_CTRL0                (SOC_REGS_BASE_ADDRESS + 0x1C)
#define	REG_SOC_REGS_GPIO_DRIVE_CTRL1                (SOC_REGS_BASE_ADDRESS + 0x20)
#define	REG_SOC_REGS_GPIO_OPEN_DRAIN                 (SOC_REGS_BASE_ADDRESS + 0x24)
#define	REG_SOC_REGS_EEPROM_SW_ACCESS                (SOC_REGS_BASE_ADDRESS + 0x28)
#define	REG_SOC_REGS_PCIE_EP_CHIP_RST_ENA            (SOC_REGS_BASE_ADDRESS + 0x2C)
#define	REG_SOC_REGS_PCIE_INTR_VEC                   (SOC_REGS_BASE_ADDRESS + 0x48)
#define	REG_SOC_REGS_PCIE_MSI_ENABLE                 (SOC_REGS_BASE_ADDRESS + 0x4C)
#define	REG_SOC_REGS_PCIE_LEGACY_INT_ENA             (SOC_REGS_BASE_ADDRESS + 0x50)
#define	REG_SOC_REGS_PCIE_DIAG_CTRL_BUS              (SOC_REGS_BASE_ADDRESS + 0x54)
#define	REG_SOC_REGS_PCIE_APPS_PM_WAKEUP             (SOC_REGS_BASE_ADDRESS + 0x60)
#define	REG_SOC_REGS_PCIE0_RADM_STATUS               (SOC_REGS_BASE_ADDRESS + 0x64)
#define	REG_SOC_REGS_PCIE_LINK_STATUS                (SOC_REGS_BASE_ADDRESS + 0x68)
#define	REG_SOC_REGS_EFUSE_DATA                      (SOC_REGS_BASE_ADDRESS + 0x70)
#define	REG_SOC_REGS_EFUSE_ADDRESS                   (SOC_REGS_BASE_ADDRESS + 0x74)
#define	REG_SOC_REGS_EFUSE_CONTROL                   (SOC_REGS_BASE_ADDRESS + 0x78)
#define	REG_SOC_REGS_EFUSE_STATUS                    (SOC_REGS_BASE_ADDRESS + 0x7C)
#define	REG_SOC_REGS_EFUSE_TIMER_CONFIG1             (SOC_REGS_BASE_ADDRESS + 0x80)
#define	REG_SOC_REGS_EFUSE_TIMER_CONFIG2             (SOC_REGS_BASE_ADDRESS + 0x84)
#define	REG_SOC_REGS_EFUSE_PCIE_FSM_OVR              (SOC_REGS_BASE_ADDRESS + 0x88)
#define	REG_SOC_REGS_EFUSE_PD                        (SOC_REGS_BASE_ADDRESS + 0x8C)
#define	REG_SOC_REGS_WATCHDOG_ENABLE                 (SOC_REGS_BASE_ADDRESS + 0x90)
#define	REG_SOC_REGS_WATCHDOG_RESET                  (SOC_REGS_BASE_ADDRESS + 0x94)
#define	REG_SOC_REGS_WATCHDOG_CMP_VALUE              (SOC_REGS_BASE_ADDRESS + 0x98)
#define	REG_SOC_REGS_WATCHDOG_COUNT                  (SOC_REGS_BASE_ADDRESS + 0x9C)
#define	REG_SOC_REGS_UART_ENABLE                     (SOC_REGS_BASE_ADDRESS + 0xA4)
#define	REG_SOC_REGS_PCIE_MSI_INTR_MODE              (SOC_REGS_BASE_ADDRESS + 0xAC)
#define	REG_SOC_REGS_CHIP_ID                         (SOC_REGS_BASE_ADDRESS + 0xB0)
#define	REG_SOC_REGS_EFUSE_INIT_VECTOR_0             (SOC_REGS_BASE_ADDRESS + 0xB4)
#define	REG_SOC_REGS_EFUSE_INIT_VECTOR_24            (SOC_REGS_BASE_ADDRESS + 0xB8)
#define	REG_SOC_REGS_EFUSE_DWORD_ENABLE              (SOC_REGS_BASE_ADDRESS + 0xC0)
#define	REG_SOC_REGS_EFUSE_DWORD_WR_MAP              (SOC_REGS_BASE_ADDRESS + 0xC4)
#define	REG_SOC_REGS_EFUSE_DWORD_RD_DATA             (SOC_REGS_BASE_ADDRESS + 0xC8)
#define	REG_SOC_REGS_EFUSE_DWORD_CONTROL             (SOC_REGS_BASE_ADDRESS + 0xCC)
#define	REG_SOC_REGS_EFUSE_DWORD_DONE                (SOC_REGS_BASE_ADDRESS + 0xD0)
#define	REG_SOC_REGS_EFUSE_BRIDGE_SOFT_RESET         (SOC_REGS_BASE_ADDRESS + 0xD8)
#define	REG_SOC_REGS_PCIE_APPS_NS_CTRL               (SOC_REGS_BASE_ADDRESS + 0xE8)
#define	REG_SOC_REGS_PCIE0_PTM_MANUAL_UPDATE         (SOC_REGS_BASE_ADDRESS + 0xF0)
#define	REG_SOC_REGS_PCIE0_PTM_AUTO_UPDATE_SIGNAL    (SOC_REGS_BASE_ADDRESS + 0xF4)
#define	REG_SOC_REGS_PCIE0_PTM_CONTEXT_VALID         (SOC_REGS_BASE_ADDRESS + 0xF8)
#define	REG_SOC_REGS_PCIE0_PTM_CLOCK_UPDATED         (SOC_REGS_BASE_ADDRESS + 0xFC)
#define	REG_SOC_REGS_PCIE0_PTM_LOCAL_CLOCK_L         (SOC_REGS_BASE_ADDRESS + 0x110)
#define	REG_SOC_REGS_PCIE0_PTM_LOCAL_CLOCK_U         (SOC_REGS_BASE_ADDRESS + 0x114)
#define	REG_SOC_REGS_PCIE0_PTM_CLOCK_CORRECTION_L    (SOC_REGS_BASE_ADDRESS + 0x118)
#define	REG_SOC_REGS_PCIE0_PTM_CLOCK_CORRECTION_U    (SOC_REGS_BASE_ADDRESS + 0x11C)
#define	REG_SOC_REGS_PCIE_GPIO_MUX_CONTROL           (SOC_REGS_BASE_ADDRESS + 0x140)
#define	REG_SOC_REGS_PCIE_PHY_CR_PARA_SEL            (SOC_REGS_BASE_ADDRESS + 0x144)
#define	REG_SOC_REGS_PCIE_MEM_CTRL_SP                (SOC_REGS_BASE_ADDRESS + 0x148)
#define	REG_SOC_REGS_PCIE_MEM_CTRL_DP                (SOC_REGS_BASE_ADDRESS + 0x14C)
#define	REG_SOC_REGS_PCIE_MEM_CTRL_TP                (SOC_REGS_BASE_ADDRESS + 0x150)
#define	REG_SOC_REGS_PCIE0_IATU_SEL                  (SOC_REGS_BASE_ADDRESS + 0x154)
#define	REG_SOC_REGS_DEBUG_OBS_CONTROL               (SOC_REGS_BASE_ADDRESS + 0x180)
#define	REG_SOC_REGS_EFUSE_WLAN                      (SOC_REGS_BASE_ADDRESS + 0x190)
#define	REG_SOC_REGS_EFUSE_VID                       (SOC_REGS_BASE_ADDRESS + 0x194)
#define	REG_SOC_REGS_GPIO_PULLUP1                    (SOC_REGS_BASE_ADDRESS + 0x200)
#define	REG_SOC_REGS_GPIO_PULLDN1                    (SOC_REGS_BASE_ADDRESS + 0x204)
#define	REG_SOC_REGS_GPIO_SLEW_CTRL1                 (SOC_REGS_BASE_ADDRESS + 0x208)
#define	REG_SOC_REGS_GPIO_DRIVE1_CTRL0               (SOC_REGS_BASE_ADDRESS + 0x20C)
#define	REG_SOC_REGS_GPIO_DRIVE1_CTRL1               (SOC_REGS_BASE_ADDRESS + 0x210)
#define	REG_SOC_REGS_GPIO_OPEN_DRAIN1                (SOC_REGS_BASE_ADDRESS + 0x214)
#define	REG_SOC_REGS_BME_RESET_MASK                  (SOC_REGS_BASE_ADDRESS + 0x220)
#define	REG_SOC_REGS_PCIE_PHY_SRAM                   (SOC_REGS_BASE_ADDRESS + 0x224)
#define	REG_SOC_REGS_SOC_SPARE_REG0                  (SOC_REGS_BASE_ADDRESS + 0x230)
#define	REG_SOC_REGS_SOC_SPARE_REG1                  (SOC_REGS_BASE_ADDRESS + 0x234)
#define	REG_SOC_REGS_SOC_SPARE_REG2                  (SOC_REGS_BASE_ADDRESS + 0x238)
#define	REG_SOC_REGS_SOC_SPARE_REG3                  (SOC_REGS_BASE_ADDRESS + 0x23C)
#define	REG_SOC_REGS_SOC_PCIE_DEBUG_REG0             (SOC_REGS_BASE_ADDRESS + 0x240)
#define	REG_SOC_REGS_SOC_PCIE_DEBUG_REG1             (SOC_REGS_BASE_ADDRESS + 0x244)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_SOC_REGS_RESERVE_LOC0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserveLoc0 : 16; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsReserveLoc0_u;

/*REG_SOC_REGS_SOFT_RESET 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swWlanRstn : 1; //wlan software reset request : ,  0 - wlan is in reset state ,  1 - wlan is in active state , , reset value: 0x1, access type: RW
		uint32 swCphyRstn : 1; //pcie cphy software reset request  :   , 0 - pcie cphy is in reset state , 1 - pcie cphy  is in active state , , reset value: 0x1, access type: RW
		uint32 swPhyRstn : 1; //pcie phy software reset request  :   , 0 - pcie phy is in reset state , 1 - pcie phy  is in active state , , reset value: 0x1, access type: RW
		uint32 swPcsRstn : 1; //pcie pcs  software reset request :  , 0 - pcie module is in reset state , 1 - pcie module is in active state , , reset value: 0x1, access type: RW
		uint32 swApbPresetn : 1; //pcie apb software reset request  :   , 0 - pcie apb bridge is in reset state , 1 - pcie apb bridge is in active state ,  , , reset value: 0x1, access type: RW
		uint32 reserved0 : 1;
		uint32 swPcie0Rstn : 1; //pcie 0 software reset request :  , 0 - pcie module is in reset state , 1 - pcie module is in active state , , reset value: 0x1, access type: RW
		uint32 reserved1 : 9;
		uint32 swChipRstn : 1; //global software reset request  :   , 0 - chip is in reset state , 1 - chip is in active state , , reset value: 0x1, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegSocRegsSoftReset_u;

/*REG_SOC_REGS_SOFT_RESET_STROBE_ENABLE 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swWlanRstnEn : 1; //wlan software reset strobe enable : , 0 - no wlan software reset generation , 1 - wlan software reset generation , , reset value: 0x1, access type: RW
		uint32 swCphyRstnEn : 1; //pcie cphy software reset strobe enable : , 0 - no pcie cphy software reset generation , 1 - pcie cphy software reset generation, reset value: 0x0, access type: RW
		uint32 swPhyRstnEn : 1; //pcie phy software reset strobe enable : , 0 - no pcie phy software reset generation , 1 - pcie phy software reset generation, reset value: 0x0, access type: RW
		uint32 swPcsRstnEn : 1; //pcie pcs software reset strobe enable : , 0 - no pcie pcs software reset generation , 1 - pcie pcs software reset generation, reset value: 0x0, access type: RW
		uint32 swApbPresetnEn : 1; //pcie apb software reset strobe enable : , 0 - no pcie apb software reset generation , 1 - pcie apb software reset generation, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 swPcie0RstnEn : 1; //pcie0 software reset strobe enable : , 0 - no pcie0 software reset generation , 1 - pcie0 software reset generation, reset value: 0x0, access type: RW
		uint32 reserved1 : 9;
		uint32 swChipRstnEn : 1; //global software reset strobe enable : , 0 - no global software reset generation , 1 - global software reset generation , , reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegSocRegsSoftResetStrobeEnable_u;

/*REG_SOC_REGS_RCU_BOOT 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 resetFsmState : 3; //RCU boot state machine status : ,    00 : HW_RESET ,    01 : INIT_EFUSE ,    10 : PCIE_EFUSE ,    11 : Reserved, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 efusePcieOvr : 1; //pcie efuse override : , 0- no pcie efuse override , 1- pcie efuse done, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 mspsStatus : 1; //PCIe Mode Select Pin-Strapping status : ,    1 : Mode 1 ,    0 : Mode 2, reset value: 0x0, access type: RO
		uint32 funStrapReg : 5; //Pin-strapping for future use, reset value: 0x1F, access type: RO
		uint32 reserved2 : 18;
	} bitFields;
} RegSocRegsRcuBoot_u;

/*REG_SOC_REGS_GPIO_PULLUP 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioPullup : 20; //gpio pull up enable : bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS.  , 0 - GPIO[0--19] pads pull up disable , 1 - GPIO[0--19] pads pull up enable, reset value: 0xFFEFF, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioPullup_u;

/*REG_SOC_REGS_GPIO_PULLDN 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioPulldn : 20; //gpio pull down enable : bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS. , 0 - GPIO[0--19] pads pull down disable , 1 - GPIO[0--19] pads pull down enable, reset value: 0x100, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioPulldn_u;

/*REG_SOC_REGS_GPIO_SLEW_CTRL 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioSlewCtrl : 20; //gpio slew rate control enable : bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS. , 0 - GPIO[0--19] pads slew rate disable , 1 - GPIO[0--19] pads slew rate enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioSlewCtrl_u;

/*REG_SOC_REGS_GPIO_DRIVE_CTRL0 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioDriveCtrl0 : 20; //gpio drive strength control ds0. ds1 and ds0 are combined to define tge drive strength of pad. bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS. , [ds1 ds0] 00 - 2mA , [ds1 ds0] 01 - 4mA , [ds1 ds0] 10 - 8mA , [ds1 ds0] 11 - 12mA , , reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioDriveCtrl0_u;

/*REG_SOC_REGS_GPIO_DRIVE_CTRL1 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioDriveCtrl1 : 20; //gpio drive strength control ds1. ds1 and ds0 are combined to define tge drive strength of pad. bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS. , [ds1 ds0] 00 - 2mA , [ds1 ds0] 01 - 4mA , [ds1 ds0] 10 - 8mA , [ds1 ds0] 11 - 12mA, reset value: 0xfffff, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioDriveCtrl1_u;

/*REG_SOC_REGS_GPIO_OPEN_DRAIN 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioOpenDrain : 20; //gpio open drain enable : bit 0, 1, ... to 15 are for GPIO0, GPIO1, ..., GPIO15, respectively. Bit 16 is for SPI DI. Bit 17 is for SPI DO. Bit 118 is for SPI CLK. Bit 19 is for SPI CS. , 0 - GPIO[0--19] pads open drain disable/push pull enable , 1 - GPIO[0--19] pads open drain enable/push pull  disable, reset value: 0x4040, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegSocRegsGpioOpenDrain_u;

/*REG_SOC_REGS_EEPROM_SW_ACCESS 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 eepromOverride : 1; //eeprom override : , 0 - EEPROM sw access is disable , 1 - EEPROM sw access is enable, reset value: 0x0, access type: RW
		uint32 eeResv1 : 3; //Reserved location, reset value: 0x0, access type: RO
		uint32 eeSclIn : 1; //eeprom scl input : , EEPROM serial clock is input, reset value: 0x0, access type: RO
		uint32 eeSclOut : 1; //eeprom scl output : , EEPROM serial clock is output, reset value: 0x0, access type: RW
		uint32 eeSclOe : 1; //eeprom scl output enable : , 0 - EEPROM scl is disable , 1 - EEPROM scl is enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 eeSdaIn : 1; //eeprom sda input : , EEPROM serial data is input, reset value: 0x0, access type: RO
		uint32 eeSdaOut : 1; //eeprom sda output : , EEPROM serial data is output, reset value: 0x0, access type: RW
		uint32 eeSdaOe : 1; //eeprom sda output enable : , 0 - EEPROM sda is disable , 1 - EEPROM sda is enable, reset value: 0x0, access type: RW
		uint32 eeResv2 : 21; //Reserved location, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEepromSwAccess_u;

/*REG_SOC_REGS_PCIE_EP_CHIP_RST_ENA 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0EpChipRstEna : 1; //pcie endpoint chip reset enable : , 0 - pcie endpoint chip reset is disable , 1 - pcie endpoint chip reset is enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPcieEpChipRstEna_u;

/*REG_SOC_REGS_PCIE_INTR_VEC 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieIntrVec : 8; //pcie interrupt vector status : , 0. Host I/F data Tx out interrupt , 1. Host I/F data Rx out interrupt , 2. MGMT Tx out interrupt , 3. MGMT Rx out interrupt , 4. FW-Driver mailbox interrupt , 5. RAB PHI interrupt , 6. WDT interrupt , 7. PVT interrupt, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegSocRegsPcieIntrVec_u;

/*REG_SOC_REGS_PCIE_MSI_ENABLE 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 msiMsixIntrMode : 1; //MSI or MSI-X Mode: ,   0 : MSI ,   1 : MSI-X , , reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 msixTc : 3; //MSI-X TC , Traffic Class for MSI-X. , reset value: 0x0, access type: RW
		uint32 reserved2 : 17;
	} bitFields;
} RegSocRegsPcieMsiEnable_u;

/*REG_SOC_REGS_PCIE_LEGACY_INT_ENA 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieLegacyIntEna : 1; //pcie legacy interrupt enable : , 0 - disable , 1 - enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPcieLegacyIntEna_u;

/*REG_SOC_REGS_PCIE_DIAG_CTRL_BUS 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0DiagCtrlBus : 3; // Description: Diagnostic Control Bus :   , -       x01: Insert LCRC error by inverting the LSB of LCRC  , -       x10: Insert ECRC error by inverting the LSB of ECRC  , -       The rising edge of these two signals ([1:0]) enable the core to  , assert an LCRC or ECRC to the packet that it currently being  , transferred. When LCRC or ECRC error packets are transmitted  , by the core, the core asserts diag_status_bus[lcrc_err_asserted]  , or diag_status_bus[ecrc_err_asserted] to report that the  , requested action has been completed. This handshake between  , control and status allows your application to control a specific  , packet being injected with an CRC or ECRC error. The LCRC and  , ECRC errors are generated by simply inverting the last bit of the  , LCRC or ECRC value.  , -       1xx: Select Fast Link Mode. Sets all internal timers to Fast Mode  , for speeding up simulation purposes. Forces the LTSSM training  , (link initialization) to use shorter timeouts and to link up faster.  , The scaling factor is 1024 for all internal timers. Fast Link Mode  , can also be activated by setting the "Fast Link Mode" bit of the  , "Port Link Control Register"., reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegSocRegsPcieDiagCtrlBus_u;

/*REG_SOC_REGS_PCIE_APPS_PM_WAKEUP 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0OutbandPwrupCmd : 1; //Wake Up: , Used by application logic to wake up the PMC state machine from a D1, D2 or D3 power state. Upon wake-up, the core sends a PM_PME Message. Needs to be asserted for one clock cycle. There is a separate apps_pm_xmt_pme input bit for each function in your core configuration. This port is functionally identical to outband_pwrup_cmd. This signal or outband_pwrup_cmd them must be used to request a return from D1/D2/D3 to D0/, reset value: 0x0, access type: RW
		uint32 pcie0AppsPmXmtPme : 1; // Wake Up. Used by application logic to wake up the  , PMC state machine from a D1, D2 or D3 power state. Upon wake-up,  , the core sends a PM_PME Message. Needs to be asserted for one  , clock cycle. There is a separate apps_pm_xmt_pme input bit for  , each function in your core configuration. This port is functionally  , identical to outband_pwrup_cmd. This signal or outband_pwrup_cmd  , them must be used to request a return from D1/D2/D3 to D0/, reset value: 0x0, access type: RW
		uint32 pcie0AppReqExitL1 : 1; //Application request to Exit L1. Request from your application to exit ASPM state L1. It is only effective when L1 is enabled., reset value: 0x0, access type: RW
		uint32 pcie0AppXferPending : 1; //ndicates that your application has , transfers pending and prevents the core from entering , L1, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegSocRegsPcieAppsPmWakeup_u;

/*REG_SOC_REGS_PCIE0_RADM_STATUS 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 pcie0RadmCplTimeout : 1; // Indicates that the completion TLP for a request has not been received within the expected time window., reset value: 0x0, access type: RO
		uint32 pcie0RadmTimeoutFuncNum : 3; //The function Number of the timed out completion. Function numbering starts at '0'., reset value: 0x0, access type: RO
		uint32 pcie0RadmTimeoutCplTc : 3; //The Traffic Class of the timed out completion., reset value: 0x0, access type: RO
		uint32 pcie0RadmTimeoutCplAttr : 2; //The Attributes field of the timed out completion., reset value: 0x0, access type: RO
		uint32 pcie0RadmTimeoutCplLen : 12; //Length (in bytes) of the timed out completion : , For a split completion, it indicates the number of bytes remaining to be delivered when the completion timed out., reset value: 0x0, access type: RO
		uint32 pcie0RadmTimeoutCplTag : 10; //The Tag field of the timed out completion., reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsPcie0RadmStatus_u;

/*REG_SOC_REGS_PCIE_LINK_STATUS 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0RdlhLinkUp : 1; //Data link layer up/down indicator :  , This status from the flow control initialization state machine indicates that flow control has been initiated and the Data link layer is ready to transmit and receive  , packets. For multi-VC designs, this signal indicates status for VC0 only.   , -       1: Link is up  , -       0: Link is down, reset value: 0x0, access type: RO
		uint32 pcie0PmCurntState : 3; //Indicates the current power state :  , The pm_curnt_state output is intended for debugging purposes, not for system operation., reset value: 0x0, access type: RO
		uint32 pcie0SmlhLtssmState : 6; // Current state of the LTSSM. , reset value: 0x0, access type: RO
		uint32 pcie0SmlhLinkUp : 1; //PHY Link up/down indicator:   , -       1: Link is up  , -       0: Link is down, reset value: 0x0, access type: RO
		uint32 pcie0LinkReqRstNot : 1; //Active low hot reset request: 0: REQ Reset requested 1: NOR Reset is not requested, reset value: 0x1, access type: RO
		uint32 reserved0 : 1;
		uint32 pipeLane0Phystatus : 1; //Communicates completion of PHY functions: 0:Idle 1:PHY is up and clock is runing, reset value: 0x1, access type: RO
		uint32 reserved1 : 15;
		uint32 pipeLane1Phystatus : 1; //Communicates completion of PHY functions: 0:Idle 1:PHY is up and clock is runing, reset value: 0x1, access type: RO
		uint32 reserved2 : 2;
	} bitFields;
} RegSocRegsPcieLinkStatus_u;

/*REG_SOC_REGS_EFUSE_DATA 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseData : 32; //Data read from the fuse array. It holds the byte returned by last completed read operation., reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEfuseData_u;

/*REG_SOC_REGS_EFUSE_ADDRESS 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseAddr : 13; //BIT_ADDRESS[4:0] :	Bit address use only for programming operations. The value is ignored for reads. , WORD_ADDRESS[12:5] : WORD address of the fuse array use for read and program operation., reset value: 0x5F, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegSocRegsEfuseAddress_u;

/*REG_SOC_REGS_EFUSE_CONTROL 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseWrRdn : 1; //1- program EFUSE array. , 0- read from EFUSE array. , Upon accessing this register a cs strobe will be generated towards the efuse_control_sm. , , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsEfuseControl_u;

/*REG_SOC_REGS_EFUSE_STATUS 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseBusy : 1; //This RO bit is set when the module is busy writing or reading E-Fuse bits on either mode of EFUSE operation. It is cleared when EFUSE control sm has completed operation and is ready to receive a new read or write command., reset value: 0x0, access type: RO
		uint32 efuseInitDone : 1; //INIT machine will set this bit to indicate the EFUSE automatic read is done. If EFUSE_fail_n is active init_done will be set automatically., reset value: 0x0, access type: RO
		uint32 efuseFsmState : 3; //This is a RO register for debug purposes only to read the EFUSE control state machine., reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegSocRegsEfuseStatus_u;

/*REG_SOC_REGS_EFUSE_TIMER_CONFIG1 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseTpgm : 16; //TIMER_TPGM , This register will define the length of STROBE pulse during read write. , During write the strobe should be 12us. , , reset value: 0x328, access type: RW
		uint32 efuseTsuCsStrobe : 16; //TIMER_TSU_CS_STROBE , Setup time beween CSB activation to STROBE activation. , , reset value: 0xD, access type: RW
	} bitFields;
} RegSocRegsEfuseTimerConfig1_u;

/*REG_SOC_REGS_EFUSE_TIMER_CONFIG2 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseTrd : 16; //TIMER_TRD  , Strobe width during read. Minimum size is 120ns. , , reset value: 0x12, access type: RW
		uint32 efuseTsuPsCsb : 16; //TSU_PS_CSB , Setup time between PS and CSB : 45ns, reset value: 0xE, access type: RW
	} bitFields;
} RegSocRegsEfuseTimerConfig2_u;

/*REG_SOC_REGS_EFUSE_PCIE_FSM_OVR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efusePcieFsmOvr : 1; //efuse control override : , 0 - no override , 1 - override by software, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsEfusePcieFsmOvr_u;

/*REG_SOC_REGS_EFUSE_PD 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efusePd : 1; //Power down power switch, when accessing efuse for read/write the pd should be de-activated. Default value of bit is '0'., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsEfusePd_u;

/*REG_SOC_REGS_WATCHDOG_ENABLE 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 watchdogEnable : 1; //watchdog counter enable : , 0 - disable , 1 - enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsWatchdogEnable_u;

/*REG_SOC_REGS_WATCHDOG_RESET 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 watchdogReset : 1; //watchdog reset : , 0 - normal operation , 1 - generates a reset pulse, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsWatchdogReset_u;

/*REG_SOC_REGS_WATCHDOG_CMP_VALUE 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 watchdogCmpValue : 32; //32-bit expiration compare value for the watchdog counter., reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsWatchdogCmpValue_u;

/*REG_SOC_REGS_WATCHDOG_COUNT 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 watchdogCount : 32; //32-bit watchdog counter status., reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsWatchdogCount_u;

/*REG_SOC_REGS_UART_ENABLE 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 uartEnable : 1; //Uart mode enable : , 0 - disable uart mode , 1 - enable uart mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsUartEnable_u;

/*REG_SOC_REGS_PCIE_MSI_INTR_MODE 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieMsiIntrMode : 2; //pcie msi interrupt modes  :  ,  00 : 8 interrupts mode ,  01 : 4 interrupts mode ,  10 : 1 interrupts mode ,  11 : reserve, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegSocRegsPcieMsiIntrMode_u;

/*REG_SOC_REGS_CHIP_ID 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 chipId : 32; //WAV700 ChipID  : , Manufacturer ID = 0x461 , Part number = 0x3171 , version ID = 0x0 , , reset value: 0x31718C3, access type: RO
	} bitFields;
} RegSocRegsChipId_u;

/*REG_SOC_REGS_EFUSE_INIT_VECTOR_0 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseInitVector0 : 32; //First WORD of EFUSE, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEfuseInitVector0_u;

/*REG_SOC_REGS_EFUSE_INIT_VECTOR_24 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseInitVector24 : 32; //24th WORD of EFUSE, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEfuseInitVector24_u;

/*REG_SOC_REGS_EFUSE_DWORD_ENABLE 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseDwordEnable : 1; //Efuse dword enable , 0 - Efuse dword operation is disabled , 1 - Efuse dword operation is enabled, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsEfuseDwordEnable_u;

/*REG_SOC_REGS_EFUSE_DWORD_WR_MAP 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseDwordWrMap : 32; //efuse_dword_wr_map : , Indicates 32 bit efuse write operation map , 0 - Efuse bit need not burn , 1 - Efuse bit need to be burn, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsEfuseDwordWrMap_u;

/*REG_SOC_REGS_EFUSE_DWORD_RD_DATA 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseDwordRdData : 32; //efuse_dword_rd_data: , This contains 32 bits Efuse read data., reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEfuseDwordRdData_u;

/*REG_SOC_REGS_EFUSE_DWORD_CONTROL 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseDwordAddress : 8; //efuse_dword_address: , This contains dword aligned address for efuse read/write operations., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
		uint32 efuseDwordWrRd : 1; //efuse_dword_wr_rd : , 0: Efuse dword read operation , 1: Efuse dword write operation, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsEfuseDwordControl_u;

/*REG_SOC_REGS_EFUSE_DWORD_DONE 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 efuseDwordDone : 1; //efuse_dword_done : , This indicates efuse dword operation done indication in toggle manner for each consecutive phase of operations., reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsEfuseDwordDone_u;

/*REG_SOC_REGS_EFUSE_BRIDGE_SOFT_RESET 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 swEfuseRstn : 1; //efuse software reset request for bridge :   , 0 - efuse bridge module is in reset state , 1 - efuse bridge module is in active state , , reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegSocRegsEfuseBridgeSoftReset_u;

/*REG_SOC_REGS_PCIE_APPS_NS_CTRL 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0RdTlpNoSnoop : 1; //No Snoop attribute bit in the PCIe0, read TLP header shall be set to the configured value in this register. The no_snoop side band signal on PCIe0 read AXI slave shall be driven with this value. , reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 pcie0WrTlpNoSnoop : 1; //No Snoop attribute bit in the PCIe0, write TLP header shall be set to the configured value in this register. The no_snoop side band signal on PCIe0 write AXI slave shall be driven with this value., reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegSocRegsPcieAppsNsCtrl_u;

/*REG_SOC_REGS_PCIE0_PTM_MANUAL_UPDATE 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmManUpdate : 1; //Indicates that the PCIe0 controller should update the PTM Requester Context and Clock now., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPcie0PtmManualUpdate_u;

/*REG_SOC_REGS_PCIE0_PTM_AUTO_UPDATE_SIGNAL 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 pcie0PtmAutoUpdateSignal : 1; //Indicates that the PCIe1 controller should update the PTM Requester Context and Clock automatically every 10ms., reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegSocRegsPcie0PtmAutoUpdateSignal_u;

/*REG_SOC_REGS_PCIE0_PTM_CONTEXT_VALID 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmContextValid : 1; //PTM Context Valid , This bit is not working in L1 mode based on pre-silicon test. Other mode is not yet tested. It is not recommended to use this bit. Alternatively, PCIe controller register PTM_REQ_STATUS_OFF  bit 0 (ptm_req_context_valid) can be used., reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPcie0PtmContextValid_u;

/*REG_SOC_REGS_PCIE0_PTM_CLOCK_UPDATED 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmClockUpdated : 1; //PTM Clock Updated, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPcie0PtmClockUpdated_u;

/*REG_SOC_REGS_PCIE0_PTM_LOCAL_CLOCK_L 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmLocalClockL : 32; //Lower 32 bits of local clock value, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsPcie0PtmLocalClockL_u;

/*REG_SOC_REGS_PCIE0_PTM_LOCAL_CLOCK_U 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmLocalClockU : 32; //Upper 32 bits of local clock value, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsPcie0PtmLocalClockU_u;

/*REG_SOC_REGS_PCIE0_PTM_CLOCK_CORRECTION_L 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmClockCorrectionL : 32; //Lower 32 bits of local corrected clock value, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsPcie0PtmClockCorrectionL_u;

/*REG_SOC_REGS_PCIE0_PTM_CLOCK_CORRECTION_U 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PtmClockCorrectionU : 32; //Upper 32 bits of local corrected clock value, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsPcie0PtmClockCorrectionU_u;

/*REG_SOC_REGS_PCIE_GPIO_MUX_CONTROL 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieClkreqEn : 1; //Enable GPIO6 for use of CLKREQ_N ,   0 : Disable. GPIO6 as GPIO ,   1 : Enable. GPIO6 as CLKREQ_N, reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 pcieWakeEn : 1; //Enable GPIO14 for use of WAKE_N ,   0 : Disable. GPIO14 as GPIO ,   1 : Enable. GPIO14 used as WAKE_N, reset value: 0x1, access type: RW
		uint32 reserved1 : 7;
		uint32 vid : 1; //Enable GPIO12 and GPIO15 for use as vid0/1 from SW/FW ,   0 : Disable. GPIO12 and GPIO15  as GPIO or VID0/1 from WLAN IP. VID0/1 out from WLAN IP as default ,   1 : Enable. GPIO12 as vid0 and GPIO15 as vid1 Output from vid0_out  and vid1_out of pcie_gpio_mux_control, reset value: 0x1, access type: RW
		uint32 reserved2 : 3;
		uint32 vid0Out : 1; //VID0_OUT: GPIO12 out data if VID=1. FW read VID0 efuse bit and write to this VID0_OUT as output to GPIO12. , reset value: 0x0, access type: RW
		uint32 vid1Out : 1; //VID1_OUT: GPIO15 out data if VID=1. FW read VID1 efuse bit and write to this VID1_OUT as output to GPIO12. , reset value: 0x0, access type: RW
		uint32 reserved3 : 11;
		uint32 pcieL1 : 1; //Enable GPIO29 for pcie L1 state observation ,   0 : GPIO29 is used as GPIO ,   1 : GPIO29 is used for PCIe L1 state observation. GPIO29 is 0 if PCIe is not in L1 state. GPIO29 is 1 if PCIe is in L1 state, reset value: 0x0, access type: RW
		uint32 reserved4 : 2;
	} bitFields;
} RegSocRegsPcieGpioMuxControl_u;

/*REG_SOC_REGS_PCIE_PHY_CR_PARA_SEL 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pciePhyCrParaSel : 1; //PCIE PHY Control Register (CR) Parallel Interface Select (Controls selection between JTAG and CR interfaces) : ,  0 - JTAG ,  1 - CR, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsPciePhyCrParaSel_u;

/*REG_SOC_REGS_PCIE_MEM_CTRL_SP 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieSpHdRfRm : 4; //RM configuration for PCIE SP HD RFs, reset value: 0x2, access type: RW
		uint32 pcieSpHdRfRme : 1; //RME configuration for PCIE SP HD RFs, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pcieSpHsSramRm : 4; //RM configuration for PCIE SP HS memories, reset value: 0x2, access type: RW
		uint32 pcieSpHsSramRme : 1; //RME configuration for PCIE SP HS memories, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegSocRegsPcieMemCtrlSp_u;

/*REG_SOC_REGS_PCIE_MEM_CTRL_DP 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieDpHdSramRma : 4; //RMA configuration for PCIE DP HD memories, reset value: 0x2, access type: RW
		uint32 pcieDpHdSramRmea : 1; //RMEA configuration for PCIE DP HD memories, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pcieDpHdSramRmb : 4; //RMB configuration for PCIE SP DP memories, reset value: 0x2, access type: RW
		uint32 pcieDpHdSramRmeb : 1; //RMEB configuration for PCIE DP HD memories, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegSocRegsPcieMemCtrlDp_u;

/*REG_SOC_REGS_PCIE_MEM_CTRL_TP 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcieTpHdRfRma : 4; //RMA configuration for PCIE TP HD RFs, reset value: 0x2, access type: RW
		uint32 pcieTpHdRfRmea : 1; //RMEA configuration for PCIE TP HD RFs, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pcieTpHdRfRmb : 4; //RMB configuration for PCIE TP HD RFs, reset value: 0x2, access type: RW
		uint32 pcieTpHdRfRmeb : 1; //RMEB configuration for PCIE TP HD RFs, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegSocRegsPcieMemCtrlTp_u;

/*REG_SOC_REGS_PCIE0_IATU_SEL 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0IatuSel : 16; //PCIe iATU registers access enable. To access iATU registers, this register shall be configured to 16'h0030. To access MSI-X table/PBA, 16'h4030. Otherwise, it shall be configured to 16'b0., reset value: 0x0, access type: RW
		uint32 msiXTablePbaSel : 1; //PCIe MSI-X table and PBA select. 0 - MSI-X Table. 1 - MSI-X PBA., reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegSocRegsPcie0IatuSel_u;

/*REG_SOC_REGS_DEBUG_OBS_CONTROL 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioClkDivVal : 16; //Clock divider value for GPIO clock observation, reset value: 0x2, access type: RW
		uint32 gpioClkSel : 4; //Select the which clocks to be output to GPIO 24 when clock observation mode is selected , 0: XTAL_CLK (default) , 1: PLL CLK , 2: current system clock , 3: pcie aux clock , 4: sms_wrck , 5: ijtag_tck , 6: edt_clk , 7: TCK , 8: scan_clk , 9: ejtag_tck, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 clkObsEn : 1; //CGU_CLK_OUT: Enable CGU clock to GPIO24 output and CGU PLL lock  to GPIO25 outpu for observation.  ,   0 : Disable. GPIO24 and GPIO25 as GPIO ,   1 : Enable. GPIO24 as CGU Clock Output and GPIO25 as CGU PLL lock output , reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 phyDebugEn : 1; //Enable GPIO26 and GPIO27 for use as PHY debug ,   0 : Disable. GPIO26 and GPIO 27 as GPIO ,   1 : Enable. GPIO26 and GPIO27 used as PCIE PHY debug, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 pcieClk : 1; //PCIE_CLK_OUT: Enable PCIe clock  to GPIO30 out for observation.  ,   0 : Disable. GPIO30 as GPIO ,   1 : Enable. GPIO30 as PCIE Clock Out, reset value: 0x1, access type: RW
		uint32 pcieReset : 1; //PCIE_RESET_OUT: Enable PCIe reset to GPIO31 out for observation.  ,   0 : Disable. GPIO31 as GPIO ,   1 : Enable. GPIO31 as PCIE Reset Out, reset value: 0x1, access type: RW
	} bitFields;
} RegSocRegsDebugObsControl_u;

/*REG_SOC_REGS_EFUSE_WLAN 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 firewallOn : 1; //Indicates that this is a productive device (non-development one). Firewall is applided based on this indication and FW in boot ROM needs to authenticate the loaded FW , In default (non-blown fuse), device should be in non-protected mode., reset value: 0x0, access type: RO
		uint32 spiDisable : 1; //Inidcates that SPI-based debug interface is disabled. In default (non-blown fuse), SPI should be enabled. This is an indication only! SPI disable is implemented in top level!, reset value: 0x0, access type: RO
		uint32 jtagDisable : 1; //Inidcates that EJTAG interface is disabled.In default (non-blown fuse), EJTAG should be enabled. This is an indication only! EJTAG disable is implemented in top level!, reset value: 0x0, access type: RO
		uint32 securityBootOn : 1; //Indication that FW needs to be checking authentication of loaded FW, reset value: 0x0, access type: RO
		uint32 skipPllInit : 1; //This bit is being read by FW (boot code) , reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 chipType : 8; //WAV7712 Chip Type: MXL31712: 0x72, MXL31713: 0x73, MXL31714: 0x74, MXL31715: 0x75, MXL31716: 0x76, reset value: 0x0, access type: RO
		uint32 hwType : 8; //Hardware type - indication reflected to driver only, reset value: 0x0, access type: RO
		uint32 hwRevision : 8; //Hardware revision - indication reflected to driver only, reset value: 0x0, access type: RO
	} bitFields;
} RegSocRegsEfuseWlan_u;

/*REG_SOC_REGS_EFUSE_VID 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vid0 : 1; //vid0, reset value: 0x0, access type: RO
		uint32 vid1 : 1; //vid1, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegSocRegsEfuseVid_u;

/*REG_SOC_REGS_GPIO_PULLUP1 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioPullup1 : 16; //gpio pull up enable 1 : bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , 0 - GPIO[16--31] pads pull up disable , 1 - GPIO16--31] pads pull up enable, reset value: 0xEFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioPullup1_u;

/*REG_SOC_REGS_GPIO_PULLDN1 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioPulldn1 : 16; //gpio pull down enable 1 : bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , 0 - GPIO16--31] pads pull down disable , 1 - GPIO16--31] pads pull down enable, reset value: 0x1000, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioPulldn1_u;

/*REG_SOC_REGS_GPIO_SLEW_CTRL1 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioSlewCtrl1 : 16; //gpio slew rate control enable 1 : bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , 0 - GPIO16--31] pads slew rate disable , 1 - GPIO16--31] pads slew rate enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioSlewCtrl1_u;

/*REG_SOC_REGS_GPIO_DRIVE1_CTRL0 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioDrive1Ctrl0 : 16; //gpio drive strength control 1 for ds0. ds1 and ds0 are combined to define tge drive strength of pad. Bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , [ds1 ds0] 00 - 2mA , [ds1 ds0] 01 - 4mA , [ds1 ds0] 10 - 8mA , [ds1 ds0] 11 - 12mA , , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioDrive1Ctrl0_u;

/*REG_SOC_REGS_GPIO_DRIVE1_CTRL1 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioDrive1Ctrl1 : 16; //gpio drive strength control 1 for ds1. ds1 and ds0 are combined to define tge drive strength of pad. Bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , [ds1 ds0] 00 - 2mA , [ds1 ds0] 01 - 4mA , [ds1 ds0] 10 - 8mA , [ds1 ds0] 11 - 12mA, reset value: 0xFFFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioDrive1Ctrl1_u;

/*REG_SOC_REGS_GPIO_OPEN_DRAIN1 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpioOpenDrain1 : 16; //gpio open drain enable 1 : bit 0, 1, ... to 15 are for GPIO16, GPIO17, ..., GPIO31, respectively.  , 0 - GPIO[16--31] pads open drain disable/push pull enable , 1 - GPIO[16--31] pads open drain enable/push pull  disable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsGpioOpenDrain1_u;

/*REG_SOC_REGS_BME_RESET_MASK 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bmeMask : 1; //bme reset mask : , 0 - BME toggle from 0 to 1 will reset the WLAN IP.  , 1 - BME toggle from 0 to 1 will not reset WLAN IP. , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegSocRegsBmeResetMask_u;

/*REG_SOC_REGS_PCIE_PHY_SRAM 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sramUpdate : 1; //PCIe PHY SRAM Update: , 0 - No SRAM update.  , 1 - SRAM Update. , reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 ltssm : 1; //LTSSM0: 0 - link training is disabled. 1 - Link training is enabled., reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 ltssmEnMuxSel : 1; //LTSSM Mux Select: 0 - FSM. 1 - SoC Register pcie_phy_sram bit field ltssm ., reset value: 0x0, access type: RW
		uint32 reserved2 : 19;
	} bitFields;
} RegSocRegsPciePhySram_u;

/*REG_SOC_REGS_SOC_SPARE_REG0 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 socSpare0 : 32; //SoC Spare register 0 , Bit 0 is used for LDO_EN_MASK, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsSocSpareReg0_u;

/*REG_SOC_REGS_SOC_SPARE_REG1 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 socSpare1 : 32; //SoC Spare register 1, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsSocSpareReg1_u;

/*REG_SOC_REGS_SOC_SPARE_REG2 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 socSpare2 : 32; //SoC Spare register 2, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsSocSpareReg2_u;

/*REG_SOC_REGS_SOC_SPARE_REG3 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 socSpare3 : 32; //SoC Spare register 3, reset value: 0x0, access type: RW
	} bitFields;
} RegSocRegsSocSpareReg3_u;

/*REG_SOC_REGS_SOC_PCIE_DEBUG_REG0 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pcie0PhyRstN : 1; //pcie0_phy_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0PhyReset : 1; //pcie0_phy_reset, reset value: 0x0, access type: RO
		uint32 pcie0PwrRstN : 1; //pcie0_pwr_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0AuxclkPowerUpRstN : 1; //pcie0_auxclk_power_up_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0SyncPowerUpRstN : 1; //pcie0_sync_power_up_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0SyncSoftWarmRstN : 1; //pcie0_sync_soft_warm_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0SyncButtonRstN : 1; //pcie0_sync_button_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0PlclkLinkDownRstN : 1; //pcie0_plclk_link_down_rst_n, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 pcie0AuxclkLinkDownRstN : 1; //pcie0_auxclk_link_down_rst_n, reset value: 0x0, access type: RO
		uint32 pcie0PmReqPhyRst : 1; //pcie0_pm_req_phy_rst, reset value: 0x0, access type: RO
		uint32 pcie0PmReqCoreRst : 1; //pcie0_pm_req_core_rst, reset value: 0x0, access type: RO
		uint32 pcie0PmSelAuxclk : 1; //pcie0_pm_sel_auxclk, reset value: 0x0, access type: RO
		uint32 pcie0AuxclkSelReg : 1; //pcie0_auxclk_sel_reg, reset value: 0x0, access type: RO
		uint32 pcie0AuxclkSelCoreclk : 1; //pcie0_auxclk_sel_coreclk, reset value: 0x0, access type: RO
		uint32 pcie0AuxclkEn : 1; //pcie0_auxclk_en, reset value: 0x0, access type: RO
		uint32 pcie0CoreclkActive : 1; //pcie0_coreclk_active, reset value: 0x0, access type: RO
		uint32 pcie0PmEnCoreClk : 1; //pcie0_pm_en_core_clk, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 clockSelect : 3; //SoC PCIE Debugging register 0: Clock select. Select clock to be observed at GPIO30 , 0: pcie0_pclk divided by 8 , 1: pcie0_muxd_aux_clk divided by 8 , 2: pcie0_ref_dig_clk divided by 1 , 3: pcie0_auxclk divided by 2 , 4: pcie0_mplla_word_clk divided by 8 , 5: pcie0_mpllb_word_clk divided by 16 , , reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 resetSelect : 5; //SoC PCIE Debugging register 0: Reset select. Select reset or other signal to be observed at GPIO31. , 0: pcie0_phy_rst_n , 1: pcie0_phy_reset , 2: pcie0_pwr_rst_n , 3: pcie0_auxclk_power_up_rst_n , 4: pcie0_sync_power_up_rst_n , 5: pcie0_sync_soft_warm_rst_n , 6: pcie0_sync_button_rst_n , 7: pcie0_plclk_link_down_rst_n , 8: pcie0_link_req_rst_not , 9: pcie0_auxclk_link_down_rst_n , 10: pcie0_pm_req_phy_rst , 11: pcie0_pm_req_core_rst , 12: pcie0_pm_sel_auxclk , 13: pcie0_auxclk_sel_reg , 14: pcie0_auxclk_sel_coreclk , 15: pcie0_auxclk_en , 16: pcie0_coreclk_active , 17: pcie0_pm_en_core_clk , 18: pipe_lane0_phystatus_sync , 19: pipe_lane1_phystatus_sync, reset value: 0x0, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegSocRegsSocPcieDebugReg0_u;

/*REG_SOC_REGS_SOC_PCIE_DEBUG_REG1 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 debugControl : 16; //SoC PCIE Debugging register 1: Debugging control. Detail is yet to be defined , , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegSocRegsSocPcieDebugReg1_u;



#endif // _SOC_REGS_REGS_H_
