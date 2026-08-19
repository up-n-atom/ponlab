
/***********************************************************************************
File:				B0PhyTxbIfRegs.h
Module:				b0PhyTxbIf
SOC Revision:		latest
Generated at:       2024-06-26 12:54:31
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PHY_TXB_IF_REGS_H_
#define _B0_PHY_TXB_IF_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PHY_TXB_IF_BASE_ADDRESS                            MEMORY_MAP_UNIT_203_BASE_ADDRESS
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C0                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1700)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1704)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C2                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1708)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C4                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1710)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C5                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1714)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C6                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1718)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_SW_RESET_N_GEN         (B0_PHY_TXB_IF_BASE_ADDRESS + 0x171C)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C8                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1720)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C9                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1724)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS    (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1728)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x172C)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1730)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_GRISC_STATE_IND        (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1738)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E0                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1780)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E4                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1790)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E5                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1794)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E6                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1798)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E7                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x179C)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EC                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x17B0)
#define	REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EF                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x17BC)
#define	REG_B0_PHY_TXB_IF_SW_GCLK_BYPASS                          (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1858)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x185C)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1860)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_4_5                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1864)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_6_7                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1868)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_8_9                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x186C)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1870)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1874)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_4_5                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1878)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_6_7                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x187C)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_8_9                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1880)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_CORREL               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1884)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1888)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x188C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1890)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1894)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1898)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x189C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18A0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18A4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18A8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18AC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18B0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18B4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18B8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18BC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18C0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18C4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18C8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18CC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_0_1                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18D0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_2_3                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18D4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18D8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18DC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18E0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18E4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18E8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18EC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18F0)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18F4)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18F8)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x18FC)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1900)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1904)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1908)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x190C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1910)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1914)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1918)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x191C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1920)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1924)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1928)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x192C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1930)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1934)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1938)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x193C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1940)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1944)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1948)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x194C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1950)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1954)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1958)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x195C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1960)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1964)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1968)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x196C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1970)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1974)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1978)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x197C)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_0_1                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1980)
#define	REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_2_3                (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1984)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_PHY_DIS_DLY                   (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1988)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_PHY_ENA_DLY                   (B0_PHY_TXB_IF_BASE_ADDRESS + 0x198C)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_SERVICE_DATA                  (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1990)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_PHY_END_DLY                   (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1994)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_LO               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1998)
#define	REG_B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_HI               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x199C)
#define	REG_B0_PHY_TXB_IF_MODEM_CLK_CTRL                          (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19A0)
#define	REG_B0_PHY_TXB_IF_FRONTENDCTRL                            (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19A4)
#define	REG_B0_PHY_TXB_IF_MDMBCNTL00                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19A8)
#define	REG_B0_PHY_TXB_IF_MDMBCNTL01                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19AC)
#define	REG_B0_PHY_TXB_IF_MDMBPRMINIT                             (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19B0)
#define	REG_B0_PHY_TXB_IF_MDMBTALPHA                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19B4)
#define	REG_B0_PHY_TXB_IF_MDMBTBETA                               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19B8)
#define	REG_B0_PHY_TXB_IF_MDMBTMU                                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19BC)
#define	REG_B0_PHY_TXB_IF_MDMBCNTL1                               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19C0)
#define	REG_B0_PHY_TXB_IF_MDMBRFCNTL                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19C4)
#define	REG_B0_PHY_TXB_IF_MDMBCCA                                 (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19C8)
#define	REG_B0_PHY_TXB_IF_MDMBEQCNTL0                             (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19CC)
#define	REG_B0_PHY_TXB_IF_MDMBEQCNTL1                             (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19D0)
#define	REG_B0_PHY_TXB_IF_MDMBCNTL20                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19D4)
#define	REG_B0_PHY_TXB_IF_MDMBCNTL21                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19D8)
#define	REG_B0_PHY_TXB_IF_MDMBSTAT00                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19DC)
#define	REG_B0_PHY_TXB_IF_MDMBSTAT01                              (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19E0)
#define	REG_B0_PHY_TXB_IF_MDMBSTAT1                               (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19E4)
#define	REG_B0_PHY_TXB_IF_TX_SYNC_FIFO                            (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19E8)
#define	REG_B0_PHY_TXB_IF_RRC_N1                                  (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19EC)
#define	REG_B0_PHY_TXB_IF_RAS_DETECTION_ANT_MASK                  (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19F0)
#define	REG_B0_PHY_TXB_IF_TX_11B_DELAY_ENABLE                     (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19F4)
#define	REG_B0_PHY_TXB_IF_TX_11B_DELAY_2TX                        (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19F8)
#define	REG_B0_PHY_TXB_IF_TX_11B_DELAY_3TX                        (B0_PHY_TXB_IF_BASE_ADDRESS + 0x19FC)
#define	REG_B0_PHY_TXB_IF_TX_11B_DELAY_4TX                        (B0_PHY_TXB_IF_BASE_ADDRESS + 0x1A00)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C0 0x1700 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 trcN1 : 3; //TRC fix value, reset value: 0x2, access type: RW
		uint32 trcMult3 : 1; //TRC mult 3 of fix input, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C0_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1 0x1704 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dccStep : 5; //DC Canceler step size, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 dccMode : 2; //DC Canceler mode, reset value: 0x0, access type: RW
		uint32 dccUpdateRate : 4; //DC Canceler update rate, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxFreqShiftEn : 1; //RX Frequency shift enable, reset value: 0x0, access type: RW
		uint32 rxFreqShiftMode : 1; //RX Frequency shift mode, reset value: 0x0, access type: RW
		uint32 reserved2 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C1_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C2 0x1708 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dccRef : 16; //DC Canceler reference value, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C2_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C4 0x1710 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 detectorThreshold : 7; //Detectors threshold, reset value: 0x1e, access type: RW
		uint32 reserved0 : 1;
		uint32 det11BEnBypass : 1; //11B detectors bypass enable, reset value: 0x0, access type: RW
		uint32 riscIf2BiuGclkBypassEn : 1; //grisc interface to BIU gclk bypass enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C4_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C5 0x1714 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg : 10; //SW Reset register, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C5_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C6 0x1718 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sublockEnable : 10; //Block enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C6_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_SW_RESET_N_GEN 0x171C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNGen : 10; //SW Reset generate register, reset value: 0x0, access type: RW
		uint32 ofdmTxSwResetNGen : 1; //ofdm_tx_sw_reset_n_gen, reset value: 0x0, access type: WO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscRegSwResetNGen_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C8 0x1720 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmResetGen44 : 4; //gsm_reset_en, reset value: 0x0, access type: RW
		uint32 gsmResetTxGen : 6; //gsm_reset_en, reset value: 0x0, access type: RW
		uint32 gsmResetRxGen : 6; //gsm_reset_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C8_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C9 0x1724 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txbRiscIntEnable : 16; //Interrupt enable register, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1C9_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS 0x1728 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txbRiscIntStatus_txbRiscIntStatusClear : 16; //Interrupt status register, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscRegTxbRiscIntStatus_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB 0x172C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 dtRasChooseAnt : 1; //Indicate RAS to choose antenna, reset value: 0x0, access type: RW
		uint32 dtRasBlackoutEn : 1; //Indicate RAS to Enter into blackout period, reset value: 0x1, access type: RW
		uint32 dtBestEn : 1; //Enable RAS best detection., reset value: 0x0, access type: RW
		uint32 dtRasChooseAntTh : 5; //Timeout to automatically choose antenna, reset value: 0x6, access type: RW
		uint32 dtRasPreDetThr : 3; //RAS pre det samples threshold, reset value: 0x2, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1Cb_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC 0x1730 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtRasBlackoutPeriod : 5; //RAS blackout period , reset value: 0x15, access type: RW
		uint32 dtRasTimeoutTh : 5; //RAS timout threshold, reset value: 0xe, access type: RW
		uint32 dtRasThr2 : 3; //RAS samples threshold, reset value: 0x3, access type: RW
		uint32 dtRasSumNum : 3; //RAS sum number, reset value: 0x4, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1Cc_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG_GRISC_STATE_IND 0x1738 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 griscStateInd : 2; //Set State Machine to state, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 clkCtrl11BState : 2; //Read 11B clock control state, reset value: 0x0, access type: RO
		uint32 reserved1 : 26;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscRegGriscStateInd_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E0 0x1780 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 bestDtRxAnt : 2; //Best antenna for RX 11b, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 phyBeRxBErrorstat : 2; //Error status to MAC, reset value: 0x0, access type: RO
		uint32 reserved2 : 26;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1E0_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E4 0x1790 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dccFreqShiftEn : 1; //DC Canceler Frequency enable, reset value: 0x0, access type: RW
		uint32 dccFreqShiftMode : 1; //DC Canceler Frequency mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1E4_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E5 0x1794 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 coreEvmI : 14; //Core EVM I, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1E5_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E6 0x1798 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 coreEvmQ : 14; //Core EVM Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1E6_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E7 0x179C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 corePsdu : 16; //Core PSDU duration, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1E7_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EC 0x17B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rasFinishToRxDly : 8; //DETECT to RX state transition, reset value: 0x14, access type: RW
		uint32 rasFinishToRxDlyEn : 1; //DETECT to RX state transition, reset value: 0x1, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1Ec_u;

/*REG_B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EF 0x17BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacIfTxState : 4; //11B PHY-MAC IF TX State, reset value: 0x0, access type: RO
		uint32 phyMacIfRxState : 4; //11B PHY-MAC IF RX State, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxbIfPhyTxbRiscReg1Ef_u;

/*REG_B0_PHY_TXB_IF_SW_GCLK_BYPASS 0x1858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swGclkBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxbIfSwGclkBypass_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_0_1 0x185C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffK0 : 6; //Modem 802.11B Detectors K coefficent 0, reset value: 0x16, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffK1 : 6; //Modem 802.11B Detectors K coefficent 1, reset value: 0x9, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffK01_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_2_3 0x1860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffK2 : 6; //Modem 802.11B Detectors K coefficent 2, reset value: 0x6, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffK3 : 6; //Modem 802.11B Detectors K coefficent 3, reset value: 0x6, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffK23_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_4_5 0x1864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffK4 : 6; //Modem 802.11B Detectors K coefficent 4, reset value: 0x6, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffK5 : 6; //Modem 802.11B Detectors K coefficent 5, reset value: 0x6, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffK45_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_6_7 0x1868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffK6 : 6; //Modem 802.11B Detectors K coefficent 6, reset value: 0x6, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffK7 : 6; //Modem 802.11B Detectors K coefficent 7, reset value: 0x6, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffK67_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_8_9 0x186C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffK8 : 6; //Modem 802.11B Detectors K coefficent 8, reset value: 0x6, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffK9 : 6; //Modem 802.11B Detectors K coefficent 9, reset value: 0x6, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffK89_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_0_1 0x1870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffG0 : 6; //Modem 802.11B Detectors G coefficent 0, reset value: 0x10, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffG1 : 6; //Modem 802.11B Detectors G coefficent 1, reset value: 0x7, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffG01_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_2_3 0x1874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffG2 : 6; //Modem 802.11B Detectors G coefficent 2, reset value: 0x4, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffG3 : 6; //Modem 802.11B Detectors G coefficent 3, reset value: 0x4, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffG23_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_4_5 0x1878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffG4 : 6; //Modem 802.11B Detectors G coefficent 4, reset value: 0x4, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffG5 : 6; //Modem 802.11B Detectors G coefficent 5, reset value: 0x4, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffG45_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_6_7 0x187C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffG6 : 6; //Modem 802.11B Detectors G coefficent 6, reset value: 0x4, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffG7 : 6; //Modem 802.11B Detectors G coefficent 7, reset value: 0x4, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffG67_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_8_9 0x1880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffG8 : 6; //Modem 802.11B Detectors G coefficent 8, reset value: 0x4, access type: CONST
		uint32 reserved0 : 2;
		uint32 modem11BDtCoeffG9 : 6; //Modem 802.11B Detectors G coefficent 9, reset value: 0x4, access type: CONST
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffG89_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_CORREL 0x1884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BDtCoeffCorrel0 : 5; //Modem 802.11B Detectors corelator coefficent 0, reset value: 0x3, access type: CONST
		uint32 reserved0 : 3;
		uint32 modem11BDtCoeffCorrel1 : 5; //Modem 802.11B Detectors corelator coefficent 1, reset value: 0x1D, access type: CONST
		uint32 reserved1 : 19;
	} bitFields;
} RegB0PhyTxbIfModem11BDtCoeffCorrel_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_0_1 0x1888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr00 : 7; //tx_11b_lut_iq_addr0_0, reset value: 0x77, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr01 : 7; //tx_11b_lut_iq_addr0_1, reset value: 0x77, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr001_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_2_3 0x188C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr02 : 7; //tx_11b_lut_iq_addr0_2, reset value: 0x77, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr03 : 7; //tx_11b_lut_iq_addr0_3, reset value: 0x77, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr023_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_0_1 0x1890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr10 : 7; //tx_11b_lut_iq_addr1_0, reset value: 0x78, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr11 : 7; //tx_11b_lut_iq_addr1_1, reset value: 0x79, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr101_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_2_3 0x1894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr12 : 7; //tx_11b_lut_iq_addr1_2, reset value: 0x7A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr13 : 7; //tx_11b_lut_iq_addr1_3, reset value: 0x7B, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr123_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_0_1 0x1898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr20 : 7; //tx_11b_lut_iq_addr2_0, reset value: 0x76, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr21 : 7; //tx_11b_lut_iq_addr2_1, reset value: 0x6A, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr201_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_2_3 0x189C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr22 : 7; //tx_11b_lut_iq_addr2_2, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr23 : 7; //tx_11b_lut_iq_addr2_3, reset value: 0x3C, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr223_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_0_1 0x18A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr30 : 7; //tx_11b_lut_iq_addr3_0, reset value: 0x77, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr31 : 7; //tx_11b_lut_iq_addr3_1, reset value: 0x4B, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr301_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_2_3 0x18A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr32 : 7; //tx_11b_lut_iq_addr3_2, reset value: 0x7D, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr33 : 7; //tx_11b_lut_iq_addr3_3, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr323_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_0_1 0x18A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr40 : 7; //tx_11b_lut_iq_addr4_0, reset value: 0x27, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr41 : 7; //tx_11b_lut_iq_addr4_1, reset value: 0x1E, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr401_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_2_3 0x18AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr42 : 7; //tx_11b_lut_iq_addr4_2, reset value: 0x27, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr43 : 7; //tx_11b_lut_iq_addr4_3, reset value: 0x3C, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr423_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_0_1 0x18B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr50 : 7; //tx_11b_lut_iq_addr5_0, reset value: 0x27, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr51 : 7; //tx_11b_lut_iq_addr5_1, reset value: 0x20, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr501_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_2_3 0x18B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr52 : 7; //tx_11b_lut_iq_addr5_2, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr53 : 7; //tx_11b_lut_iq_addr5_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr523_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_0_1 0x18B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr60 : 7; //tx_11b_lut_iq_addr6_0, reset value: 0x26, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr61 : 7; //tx_11b_lut_iq_addr6_1, reset value: 0x11, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr601_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_2_3 0x18BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr62 : 7; //tx_11b_lut_iq_addr6_2, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr63 : 7; //tx_11b_lut_iq_addr6_3, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr623_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_0_1 0x18C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr70 : 7; //tx_11b_lut_iq_addr7_0, reset value: 0x26, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr71 : 7; //tx_11b_lut_iq_addr7_1, reset value: 0x13, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr701_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_2_3 0x18C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr72 : 7; //tx_11b_lut_iq_addr7_2, reset value: 0x7, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr73 : 7; //tx_11b_lut_iq_addr7_3, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr723_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_0_1 0x18C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr80 : 7; //tx_11b_lut_iq_addr8_0, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr81 : 7; //tx_11b_lut_iq_addr8_1, reset value: 0x6a, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr801_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_2_3 0x18CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr82 : 7; //tx_11b_lut_iq_addr8_2, reset value: 0x76, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr83 : 7; //tx_11b_lut_iq_addr8_3, reset value: 0x7b, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr823_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_0_1 0x18D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr90 : 7; //tx_11b_lut_iq_addr9_0, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr91 : 7; //tx_11b_lut_iq_addr9_1, reset value: 0x6b, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr901_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_2_3 0x18D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr92 : 7; //tx_11b_lut_iq_addr9_2, reset value: 0x79, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr93 : 7; //tx_11b_lut_iq_addr9_3, reset value: 0x7E, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr923_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_0_1 0x18D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr100 : 7; //tx_11b_lut_iq_addr10_0, reset value: 0x54, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr101 : 7; //tx_11b_lut_iq_addr10_1, reset value: 0x5D, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1001_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_2_3 0x18DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr102 : 7; //tx_11b_lut_iq_addr10_2, reset value: 0x54, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr103 : 7; //tx_11b_lut_iq_addr10_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1023_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_0_1 0x18E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr110 : 7; //tx_11b_lut_iq_addr11_0, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr111 : 7; //tx_11b_lut_iq_addr11_1, reset value: 0x5E, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1101_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_2_3 0x18E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr112 : 7; //tx_11b_lut_iq_addr11_2, reset value: 0x57, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr113 : 7; //tx_11b_lut_iq_addr11_3, reset value: 0x42, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1123_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_0_1 0x18E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr120 : 7; //tx_11b_lut_iq_addr12_0, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr121 : 7; //tx_11b_lut_iq_addr12_1, reset value: 0x11, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1201_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_2_3 0x18EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr122 : 7; //tx_11b_lut_iq_addr12_2, reset value: 0x26, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr123 : 7; //tx_11b_lut_iq_addr12_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1223_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_0_1 0x18F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr130 : 7; //tx_11b_lut_iq_addr13_0, reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr131 : 7; //tx_11b_lut_iq_addr13_1, reset value: 0x13, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1301_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_2_3 0x18F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr132 : 7; //tx_11b_lut_iq_addr13_2, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr133 : 7; //tx_11b_lut_iq_addr13_3, reset value: 0x42, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1323_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_0_1 0x18F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr140 : 7; //tx_11b_lut_iq_addr14_0, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr141 : 7; //tx_11b_lut_iq_addr14_1, reset value: 0x4, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1401_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_2_3 0x18FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr142 : 7; //tx_11b_lut_iq_addr14_2, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr143 : 7; //tx_11b_lut_iq_addr14_3, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1423_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_0_1 0x1900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr150 : 7; //tx_11b_lut_iq_addr15_0, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr151 : 7; //tx_11b_lut_iq_addr15_1, reset value: 0x5, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1501_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_2_3 0x1904 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr152 : 7; //tx_11b_lut_iq_addr15_2, reset value: 0x6, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr153 : 7; //tx_11b_lut_iq_addr15_3, reset value: 0x7, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1523_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_0_1 0x1908 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr160 : 7; //tx_11b_lut_iq_addr16_0, reset value: 0x7A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr161 : 7; //tx_11b_lut_iq_addr16_1, reset value: 0x79, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1601_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_2_3 0x190C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr162 : 7; //tx_11b_lut_iq_addr16_2, reset value: 0x78, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr163 : 7; //tx_11b_lut_iq_addr16_3, reset value: 0x77, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1623_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_0_1 0x1910 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr170 : 7; //tx_11b_lut_iq_addr17_0, reset value: 0x7B, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr171 : 7; //tx_11b_lut_iq_addr17_1, reset value: 0x7A, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1701_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_2_3 0x1914 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr172 : 7; //tx_11b_lut_iq_addr17_2, reset value: 0x7B, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr173 : 7; //tx_11b_lut_iq_addr17_3, reset value: 0x7B, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1723_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_0_1 0x1918 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr180 : 7; //tx_11b_lut_iq_addr18_0, reset value: 0x79, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr181 : 7; //tx_11b_lut_iq_addr18_1, reset value: 0x6b, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1801_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_2_3 0x191C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr182 : 7; //tx_11b_lut_iq_addr18_2, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr183 : 7; //tx_11b_lut_iq_addr18_3, reset value: 0x3C, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1823_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_0_1 0x1920 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr190 : 7; //tx_11b_lut_iq_addr19_0, reset value: 0x7A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr191 : 7; //tx_11b_lut_iq_addr19_1, reset value: 0x6D, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1901_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_2_3 0x1924 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr192 : 7; //tx_11b_lut_iq_addr19_2, reset value: 0x58, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr193 : 7; //tx_11b_lut_iq_addr19_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr1923_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_0_1 0x1928 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr200 : 7; //tx_11b_lut_iq_addr20_0, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr201 : 7; //tx_11b_lut_iq_addr20_1, reset value: 0x20, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2001_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_2_3 0x192C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr202 : 7; //tx_11b_lut_iq_addr20_2, reset value: 0x27, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr203 : 7; //tx_11b_lut_iq_addr20_3, reset value: 0x3C, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2023_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_0_1 0x1930 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr210 : 7; //tx_11b_lut_iq_addr21_0, reset value: 0x2A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr211 : 7; //tx_11b_lut_iq_addr21_1, reset value: 0x21, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2101_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_2_3 0x1934 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr212 : 7; //tx_11b_lut_iq_addr21_2, reset value: 0x2A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr213 : 7; //tx_11b_lut_iq_addr21_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2123_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_0_1 0x1938 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr220 : 7; //tx_11b_lut_iq_addr22_0, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr221 : 7; //tx_11b_lut_iq_addr22_1, reset value: 0x13, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2201_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_2_3 0x193C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr222 : 7; //tx_11b_lut_iq_addr22_2, reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr223 : 7; //tx_11b_lut_iq_addr22_3, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2223_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_0_1 0x1940 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr230 : 7; //tx_11b_lut_iq_addr23_0, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr231 : 7; //tx_11b_lut_iq_addr23_1, reset value: 0x14, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2301_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_2_3 0x1944 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr232 : 7; //tx_11b_lut_iq_addr23_2, reset value: 0x8, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr233 : 7; //tx_11b_lut_iq_addr23_3, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2323_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_0_1 0x1948 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr240 : 7; //tx_11b_lut_iq_addr24_0, reset value: 0x58, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr241 : 7; //tx_11b_lut_iq_addr24_1, reset value: 0x6B, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2401_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_2_3 0x194C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr242 : 7; //tx_11b_lut_iq_addr24_2, reset value: 0x77, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr243 : 7; //tx_11b_lut_iq_addr24_3, reset value: 0x7B, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2423_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_0_1 0x1950 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr250 : 7; //tx_11b_lut_iq_addr25_0, reset value: 0x58, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr251 : 7; //tx_11b_lut_iq_addr25_1, reset value: 0x6D, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2501_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_2_3 0x1954 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr252 : 7; //tx_11b_lut_iq_addr25_2, reset value: 0x7A, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr253 : 7; //tx_11b_lut_iq_addr25_3, reset value: 0x7E, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2523_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_0_1 0x1958 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr260 : 7; //tx_11b_lut_iq_addr26_0, reset value: 0x57, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr261 : 7; //tx_11b_lut_iq_addr26_1, reset value: 0x5E, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2601_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_2_3 0x195C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr262 : 7; //tx_11b_lut_iq_addr26_2, reset value: 0x55, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr263 : 7; //tx_11b_lut_iq_addr26_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2623_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_0_1 0x1960 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr270 : 7; //tx_11b_lut_iq_addr27_0, reset value: 0x57, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr271 : 7; //tx_11b_lut_iq_addr27_1, reset value: 0x60, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2701_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_2_3 0x1964 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr272 : 7; //tx_11b_lut_iq_addr27_2, reset value: 0x57, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr273 : 7; //tx_11b_lut_iq_addr27_3, reset value: 0x42, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2723_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_0_1 0x1968 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr280 : 7; //tx_11b_lut_iq_addr28_0, reset value: 0x7, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr281 : 7; //tx_11b_lut_iq_addr28_1, reset value: 0x13, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2801_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_2_3 0x196C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr282 : 7; //tx_11b_lut_iq_addr28_2, reset value: 0x26, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr283 : 7; //tx_11b_lut_iq_addr28_3, reset value: 0x3F, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2823_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_0_1 0x1970 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr290 : 7; //tx_11b_lut_iq_addr29_0, reset value: 0x8, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr291 : 7; //tx_11b_lut_iq_addr29_1, reset value: 0x14, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2901_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_2_3 0x1974 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr292 : 7; //tx_11b_lut_iq_addr29_2, reset value: 0x29, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr293 : 7; //tx_11b_lut_iq_addr29_3, reset value: 0x42, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr2923_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_0_1 0x1978 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr300 : 7; //tx_11b_lut_iq_addr30_0, reset value: 0x6, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr301 : 7; //tx_11b_lut_iq_addr30_1, reset value: 0x5, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr3001_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_2_3 0x197C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr302 : 7; //tx_11b_lut_iq_addr30_2, reset value: 0x4, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr303 : 7; //tx_11b_lut_iq_addr30_3, reset value: 0x3, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr3023_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_0_1 0x1980 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr310 : 7; //tx_11b_lut_iq_addr31_0, reset value: 0x7, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr311 : 7; //tx_11b_lut_iq_addr31_1, reset value: 0x7, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr3101_u;

/*REG_B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_2_3 0x1984 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BLutIqAddr312 : 7; //tx_11b_lut_iq_addr31_2, reset value: 0x7, access type: RW
		uint32 reserved0 : 1;
		uint32 tx11BLutIqAddr313 : 7; //tx_11b_lut_iq_addr31_3, reset value: 0x7, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfTx11BLutIqAddr3123_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_PHY_DIS_DLY 0x1988 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BPhyDisDly : 12; //Modem 802.11B PHY TX Disable Delay, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxbIfModem11BPhyDisDly_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_PHY_ENA_DLY 0x198C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BPhyEnaDly : 12; //Modem 802.11B PHY TX enable Delay, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxbIfModem11BPhyEnaDly_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_SERVICE_DATA 0x1990 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BServiceData : 8; //Modem 802.11B Sevice data, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxbIfModem11BServiceData_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_PHY_END_DLY 0x1994 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BPhyEndDly : 4; //Modem 802.11B PHY RX end Delay, reset value: 0x2, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxbIfModem11BPhyEndDly_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_LO 0x1998 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BRasPowerThLo : 16; //Modem 802.11B RAS power threshold , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfModem11BRasPowerThLo_u;

/*REG_B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_HI 0x199C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BRasPowerThHi : 6; //Modem 802.11B RAS power threshold , reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PhyTxbIfModem11BRasPowerThHi_u;

/*REG_B0_PHY_TXB_IF_MODEM_CLK_CTRL 0x19A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 modem11BStb40Enable : 1; //Modem 11b strobe 40 enable, reset value: 0x1, access type: RW
		uint32 clk44GenEnable : 1; //Modem 11b clk gen enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 clk44GenDiv0 : 4; //Modem 11b clk gen div 0, reset value: 0xd, access type: RW
		uint32 clk44GenDiv1 : 4; //Modem 11b clk gen div 1, reset value: 0xf, access type: RW
		uint32 clk44GenLength : 4; //Modem 11b clk gen length, reset value: 0xb, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegB0PhyTxbIfModemClkCtrl_u;

/*REG_B0_PHY_TXB_IF_FRONTENDCTRL 0x19A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intDcdisb : 1; //disable the dc offset estimation , reset value: 0x0, access type: RW
		uint32 intScaldisb : 1; //disable scaling, reset value: 0x0, access type: RW
		uint32 intMultscaldisb : 1; //disable amplifier, reset value: 0x0, access type: RW
		uint32 intMultscal : 7; //setting the amplifier's values of scaling_mult and scaling_shift , , reset value: 0x1, access type: RW
		uint32 intDcesttime : 1; //DC estimation time selection , , reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB0PhyTxbIfFrontendctrl_u;

/*REG_B0_PHY_TXB_IF_MDMBCNTL00 0x19A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPrepre : 6; //pre-preamble cycles, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 intTxc2Disb : 1; //no description, reset value: 0x0, access type: RW
		uint32 intSfdlen : 3; //short SFD preamble bits, reset value: 0x0, access type: RW
		uint32 intInterfildisb : 1; //no description, reset value: 0x0, access type: RW
		uint32 intSfderr : 3; //SFD error num, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfMdmbcntl00_u;

/*REG_B0_PHY_TXB_IF_MDMBCNTL01 0x19AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intScrambdisb : 1; //Scrambeling disable, reset value: 0x0, access type: RW
		uint32 intSpreaddisb : 1; //Spreading disable, reset value: 0x0, access type: RW
		uint32 intFirdisb : 1; //Tx and Rx filters disable, reset value: 0x0, access type: RW
		uint32 intEqdisb : 1; //Equalizer bypass, reset value: 0x0, access type: RW
		uint32 intCompdisb : 1; //Frequency offset compensation bypass, reset value: 0x0, access type: RW
		uint32 intDcoffdisb : 1; //DC Offset compensation bypass, reset value: 0x0, access type: RW
		uint32 intPrecompdisb : 1; //Frequency pre-compensation bypass, reset value: 0x0, access type: RW
		uint32 intGaindisb : 1; //Gain compensation bypass, reset value: 0x0, access type: RW
		uint32 intIqmmdisb : 1; //I/Q Mismatch compensation bypass, reset value: 0x1, access type: RW
		uint32 intInterpdisb : 1; //Timing interpolator bypass, reset value: 0x0, access type: RW
		uint32 intRxc2Disb : 1; //RX 2-s complement conversion bypass, reset value: 0x0, access type: RW
		uint32 intTlockdisb : 1; //Timing lock mode, reset value: 0x1, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxbIfMdmbcntl01_u;

/*REG_B0_PHY_TXB_IF_MDMBPRMINIT 0x19B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intAlpha : 2; //Equalizer initial ? parameter, reset value: 0x2, access type: RW
		uint32 reserved0 : 2;
		uint32 intBeta : 2; //Equalizer initial  ? parameter, reset value: 0x2, access type: RW
		uint32 reserved1 : 2;
		uint32 intMu : 2; //Phase and Carrier  ? parameter, reset value: 0x1, access type: RW
		uint32 reserved2 : 2;
		uint32 intRho : 2; //Phase and Carrier ? parameter, reset value: 0x2, access type: RW
		uint32 reserved3 : 18;
	} bitFields;
} RegB0PhyTxbIfMdmbprminit_u;

/*REG_B0_PHY_TXB_IF_MDMBTALPHA 0x19B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intTalpha0 : 4; //Equalizer ? parameter update interva ??0, reset value: 0x4, access type: RW
		uint32 intTalpha1 : 4; //Equalizer ? parameter update interva ??1, reset value: 0x3, access type: RW
		uint32 intTalpha2 : 4; //Equalizer ? parameter update interva ??2, reset value: 0x2, access type: RW
		uint32 intTalpha3 : 4; //Equalizer ? parameter update interva ??2, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfMdmbtalpha_u;

/*REG_B0_PHY_TXB_IF_MDMBTBETA 0x19B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intTbeta0 : 4; //Equalizer ? parameter update interva ??0, reset value: 0x4, access type: RW
		uint32 intTbeta1 : 4; //Equalizer ? parameter update interva ??1, reset value: 0x3, access type: RW
		uint32 intTbeta2 : 4; //Equalizer ? parameter update interva ??2, reset value: 0x2, access type: RW
		uint32 intTbeta3 : 4; //Equalizer ? parameter update interva ??3, reset value: 0x3, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfMdmbtbeta_u;

/*REG_B0_PHY_TXB_IF_MDMBTMU 0x19BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intTmu0 : 4; //Equalizer ? parameter update interva ??0, reset value: 0x5, access type: RW
		uint32 intTmu1 : 4; //Equalizer ? parameter update interva ??1, reset value: 0x3, access type: RW
		uint32 intTmu2 : 4; //Equalizer ? parameter update interva ??2, reset value: 0x5, access type: RW
		uint32 intTmu3 : 4; //Equalizer ? parameter update interva ??3, reset value: 0x1, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfMdmbtmu_u;

/*REG_B0_PHY_TXB_IF_MDMBCNTL1 0x19C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intRxmaxlength : 12; //Max accepted received length, reset value: 0x92a, access type: RW
		uint32 intRxlenchken : 1; //Rx length check mode, reset value: 0x0, access type: RW
		uint32 intDscrmodesel : 1; //Selects dscr_mode in decode path, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB0PhyTxbIfMdmbcntl1_u;

/*REG_B0_PHY_TXB_IF_MDMBRFCNTL 0x19C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intTxconst : 8; //I data DAC valuue before Tx packets, reset value: 0x0, access type: RW
		uint32 intTxenddel : 8; //Tx front-end delay, reset value: 0x30, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfMdmbrfcntl_u;

/*REG_B0_PHY_TXB_IF_MDMBCCA 0x19C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 intCcamode : 3; //CCA mode, reset value: 0x4, access type: RW
		uint32 reserved1 : 21;
	} bitFields;
} RegB0PhyTxbIfMdmbcca_u;

/*REG_B0_PHY_TXB_IF_MDMBEQCNTL0 0x19CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intEqtime : 4; //Equalizer switch from CCA delay, reset value: 0x5, access type: RW
		uint32 reserved0 : 1;
		uint32 intEsttime : 5; //Equalizer en to estimation delay, reset value: 0x1e, access type: RW
		uint32 intComptime : 5; //Estimation start to compensation start, reset value: 0x1e, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB0PhyTxbIfMdmbeqcntl0_u;

/*REG_B0_PHY_TXB_IF_MDMBEQCNTL1 0x19D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intEqhold : 12; //Last param update to equalizer stop, reset value: 0x3e8, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxbIfMdmbeqcntl1_u;

/*REG_B0_PHY_TXB_IF_MDMBCNTL20 0x19D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intLooptime : 4; //AGC/CCA to phase and carrier offset, reset value: 0xf, access type: RW
		uint32 reserved0 : 4;
		uint32 intSynctime : 6; //Peak detector sync duration, reset value: 0x9, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfMdmbcntl20_u;

/*REG_B0_PHY_TXB_IF_MDMBCNTL21 0x19D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPrecomp : 6; //Energey detection to frequency pre-compensation, reset value: 0x32, access type: RW
		uint32 reserved0 : 2;
		uint32 intMaxstage : 6; //timing offset compensation DSSS/CCK interpolator stages, reset value: 0x28, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfMdmbcntl21_u;

/*REG_B0_PHY_TXB_IF_MDMBSTAT00 0x19DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regDcoffseti : 6; //DC Offset Estimation I, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 regDcoffsetq : 6; //DC Offset Estimation Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 18;
	} bitFields;
} RegB0PhyTxbIfMdmbstat00_u;

/*REG_B0_PHY_TXB_IF_MDMBSTAT01 0x19E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regEqsumi : 8; //Equalizer coefficients sum I, reset value: 0x0, access type: RO
		uint32 regEqsumq : 8; //Equalizer coefficients sum Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfMdmbstat01_u;

/*REG_B0_PHY_TXB_IF_MDMBSTAT1 0x19E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 regFreqoffestim : 8; //Last Packet Frequency Offset Estimation, reset value: 0x0, access type: RO
		uint32 regIqgainestim : 7; //Last Packet Gain Estimation, reset value: 0xX, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PhyTxbIfMdmbstat1_u;

/*REG_B0_PHY_TXB_IF_TX_SYNC_FIFO 0x19E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gen11BEvenStrbEn : 1; //generate even strobes for 11b (should be enable only when no interpolation filter is used) , reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxbIfTxSyncFifo_u;

/*REG_B0_PHY_TXB_IF_RRC_N1 0x19EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rrcN1Ant0 : 3; //antenna 0 RRC fix value, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rrcN1Ant1 : 3; //antenna 1 RRC fix value, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rrcN1Ant2 : 3; //antenna 2 RRC fix value, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rrcN1Ant3 : 3; //antenna 3 RRC fix value, reset value: 0x3, access type: RW
		uint32 reserved3 : 17;
	} bitFields;
} RegB0PhyTxbIfRrcN1_u;

/*REG_B0_PHY_TXB_IF_RAS_DETECTION_ANT_MASK 0x19F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rasDetectionAntMask : 4; //which antennas can be part of the RAS, reset value: 0xF, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PhyTxbIfRasDetectionAntMask_u;

/*REG_B0_PHY_TXB_IF_TX_11B_DELAY_ENABLE 0x19F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BDelayEnable : 1; //11b delay is enabled, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PhyTxbIfTx11BDelayEnable_u;

/*REG_B0_PHY_TXB_IF_TX_11B_DELAY_2TX 0x19F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BDelay2Tx : 8; //11b delay when 2 antennas, reset value: 0x80, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PhyTxbIfTx11BDelay2Tx_u;

/*REG_B0_PHY_TXB_IF_TX_11B_DELAY_3TX 0x19FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BDelay3Tx : 12; //11b delay when 3 antennas, reset value: 0x840, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PhyTxbIfTx11BDelay3Tx_u;

/*REG_B0_PHY_TXB_IF_TX_11B_DELAY_4TX 0x1A00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tx11BDelay4Tx : 16; //11b delay when 4 antennas, reset value: 0x6420, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PhyTxbIfTx11BDelay4Tx_u;

//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C0 0x1700 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C0_FIELD_TRC_N1_MASK                                                       0x00000007
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C0_FIELD_TRC_MULT_3_MASK                                                   0x00000008
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1 0x1704 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1_FIELD_DCC_STEP_MASK                                                     0x0000001F
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1_FIELD_DCC_MODE_MASK                                                     0x000000C0
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1_FIELD_DCC_UPDATE_RATE_MASK                                              0x00000F00
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1_FIELD_RX_FREQ_SHIFT_EN_MASK                                             0x00004000
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C1_FIELD_RX_FREQ_SHIFT_MODE_MASK                                           0x00008000
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C2 0x1708 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C2_FIELD_DCC_REF_MASK                                                      0x0000FFFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C4 0x1710 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C4_FIELD_DETECTOR_THRESHOLD_MASK                                           0x0000007F
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C4_FIELD_DET_11B_EN_BYPASS_MASK                                            0x00000100
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C4_FIELD_RISC_IF2BIU_GCLK_BYPASS_EN_MASK                                   0x00000200
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5 0x1714 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C5_FIELD_SW_RESET_N_REG_MASK                                               0x000003FF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C6 0x1718 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C6_FIELD_SUBLOCK_ENABLE_MASK                                               0x000003FF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG_SW_RESET_N_GEN 0x171C */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_SW_RESET_N_GEN_FIELD_SW_RESET_N_GEN_MASK                                   0x000003FF
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_SW_RESET_N_GEN_FIELD_OFDM_TX_SW_RESET_N_GEN_MASK                           0x00000400
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C8 0x1720 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C8_FIELD_GSM_RESET_GEN_44_MASK                                             0x0000000F
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C8_FIELD_GSM_RESET_TX_GEN_MASK                                             0x000003F0
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C8_FIELD_GSM_RESET_RX_GEN_MASK                                             0x0000FC00
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C9 0x1724 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1C9_FIELD_TXB_RISC_INT_ENABLE_MASK                                          0x0000FFFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS 0x1728 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS_FIELD_TXB_RISC_INT_STATUS_MASK                         0x0000FFFF
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS_FIELD_TXB_RISC_INT_STATUS_CLEAR_MASK                   0x0000FFFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1CB 0x172C */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB_FIELD_DT_RAS_CHOOSE_ANT_MASK                                            0x00000010
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB_FIELD_DT_RAS_BLACKOUT_EN_MASK                                           0x00000020
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB_FIELD_DT_BEST_EN_MASK                                                   0x00000040
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB_FIELD_DT_RAS_CHOOSE_ANT_TH_MASK                                         0x00000F80
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CB_FIELD_DT_RAS_PRE_DET_THR_MASK                                           0x00007000
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1CC 0x1730 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC_FIELD_DT_RAS_BLACKOUT_PERIOD_MASK                                       0x0000001F
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC_FIELD_DT_RAS_TIMEOUT_TH_MASK                                            0x000003E0
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC_FIELD_DT_RAS_THR2_MASK                                                  0x00001C00
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1CC_FIELD_DT_RAS_SUM_NUM_MASK                                               0x0000E000
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG_GRISC_STATE_IND 0x1738 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_GRISC_STATE_IND_FIELD_GRISC_STATE_IND_MASK                                 0x00000003
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG_GRISC_STATE_IND_FIELD_CLK_CTRL_11B_STATE_MASK                              0x00000030
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1E0 0x1780 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E0_FIELD_BEST_DT_RX_ANT_MASK                                               0x00000006
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E0_FIELD_PHY_BE_RX_B_ERRORSTAT_MASK                                        0x00000030
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1E4 0x1790 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E4_FIELD_DCC_FREQ_SHIFT_EN_MASK                                            0x00000001
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E4_FIELD_DCC_FREQ_SHIFT_MODE_MASK                                          0x00000002
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1E5 0x1794 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E5_FIELD_CORE_EVM_I_MASK                                                   0x00003FFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1E6 0x1798 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E6_FIELD_CORE_EVM_Q_MASK                                                   0x00003FFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1E7 0x179C */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1E7_FIELD_CORE_PSDU_MASK                                                    0x0000FFFF
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1EC 0x17B0 */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EC_FIELD_RAS_FINISH_TO_RX_DLY_MASK                                         0x000000FF
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EC_FIELD_RAS_FINISH_TO_RX_DLY_EN_MASK                                      0x00000100
//========================================
/* REG_PHY_TXB_IF_PHY_TXB_RISC_REG1EF 0x17BC */
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EF_FIELD_PHY_MAC_IF_TX_STATE_MASK                                          0x0000000F
#define B0_PHY_TXB_IF_PHY_TXB_RISC_REG1EF_FIELD_PHY_MAC_IF_RX_STATE_MASK                                          0x000000F0
//========================================
/* REG_PHY_TXB_IF_SW_GCLK_BYPASS 0x1858 */
#define B0_PHY_TXB_IF_SW_GCLK_BYPASS_FIELD_SW_GCLK_BYPASS_MASK                                                    0x00000001
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_0_1 0x185C */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_0_1_FIELD_MODEM_11B_DT_COEFF_K_0_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_0_1_FIELD_MODEM_11B_DT_COEFF_K_1_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_2_3 0x1860 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_2_3_FIELD_MODEM_11B_DT_COEFF_K_2_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_2_3_FIELD_MODEM_11B_DT_COEFF_K_3_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_4_5 0x1864 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_4_5_FIELD_MODEM_11B_DT_COEFF_K_4_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_4_5_FIELD_MODEM_11B_DT_COEFF_K_5_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_6_7 0x1868 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_6_7_FIELD_MODEM_11B_DT_COEFF_K_6_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_6_7_FIELD_MODEM_11B_DT_COEFF_K_7_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_8_9 0x186C */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_8_9_FIELD_MODEM_11B_DT_COEFF_K_8_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_K_8_9_FIELD_MODEM_11B_DT_COEFF_K_9_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_0_1 0x1870 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_0_1_FIELD_MODEM_11B_DT_COEFF_G_0_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_0_1_FIELD_MODEM_11B_DT_COEFF_G_1_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_2_3 0x1874 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_2_3_FIELD_MODEM_11B_DT_COEFF_G_2_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_2_3_FIELD_MODEM_11B_DT_COEFF_G_3_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_4_5 0x1878 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_4_5_FIELD_MODEM_11B_DT_COEFF_G_4_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_4_5_FIELD_MODEM_11B_DT_COEFF_G_5_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_6_7 0x187C */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_6_7_FIELD_MODEM_11B_DT_COEFF_G_6_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_6_7_FIELD_MODEM_11B_DT_COEFF_G_7_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_8_9 0x1880 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_8_9_FIELD_MODEM_11B_DT_COEFF_G_8_MASK                                  0x0000003F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_G_8_9_FIELD_MODEM_11B_DT_COEFF_G_9_MASK                                  0x00003F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_DT_COEFF_CORREL 0x1884 */
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_CORREL_FIELD_MODEM_11B_DT_COEFF_CORREL_0_MASK                            0x0000001F
#define B0_PHY_TXB_IF_MODEM_11B_DT_COEFF_CORREL_FIELD_MODEM_11B_DT_COEFF_CORREL_1_MASK                            0x00001F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_0_1 0x1888 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_0_1_FIELD_TX_11B_LUT_IQ_ADDR0_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_0_1_FIELD_TX_11B_LUT_IQ_ADDR0_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_2_3 0x188C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_2_3_FIELD_TX_11B_LUT_IQ_ADDR0_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR0_2_3_FIELD_TX_11B_LUT_IQ_ADDR0_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_0_1 0x1890 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_0_1_FIELD_TX_11B_LUT_IQ_ADDR1_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_0_1_FIELD_TX_11B_LUT_IQ_ADDR1_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_2_3 0x1894 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_2_3_FIELD_TX_11B_LUT_IQ_ADDR1_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR1_2_3_FIELD_TX_11B_LUT_IQ_ADDR1_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_0_1 0x1898 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_0_1_FIELD_TX_11B_LUT_IQ_ADDR2_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_0_1_FIELD_TX_11B_LUT_IQ_ADDR2_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_2_3 0x189C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_2_3_FIELD_TX_11B_LUT_IQ_ADDR2_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR2_2_3_FIELD_TX_11B_LUT_IQ_ADDR2_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_0_1 0x18A0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_0_1_FIELD_TX_11B_LUT_IQ_ADDR3_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_0_1_FIELD_TX_11B_LUT_IQ_ADDR3_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_2_3 0x18A4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_2_3_FIELD_TX_11B_LUT_IQ_ADDR3_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR3_2_3_FIELD_TX_11B_LUT_IQ_ADDR3_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_0_1 0x18A8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_0_1_FIELD_TX_11B_LUT_IQ_ADDR4_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_0_1_FIELD_TX_11B_LUT_IQ_ADDR4_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_2_3 0x18AC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_2_3_FIELD_TX_11B_LUT_IQ_ADDR4_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR4_2_3_FIELD_TX_11B_LUT_IQ_ADDR4_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_0_1 0x18B0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_0_1_FIELD_TX_11B_LUT_IQ_ADDR5_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_0_1_FIELD_TX_11B_LUT_IQ_ADDR5_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_2_3 0x18B4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_2_3_FIELD_TX_11B_LUT_IQ_ADDR5_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR5_2_3_FIELD_TX_11B_LUT_IQ_ADDR5_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_0_1 0x18B8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_0_1_FIELD_TX_11B_LUT_IQ_ADDR6_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_0_1_FIELD_TX_11B_LUT_IQ_ADDR6_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_2_3 0x18BC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_2_3_FIELD_TX_11B_LUT_IQ_ADDR6_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR6_2_3_FIELD_TX_11B_LUT_IQ_ADDR6_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_0_1 0x18C0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_0_1_FIELD_TX_11B_LUT_IQ_ADDR7_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_0_1_FIELD_TX_11B_LUT_IQ_ADDR7_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_2_3 0x18C4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_2_3_FIELD_TX_11B_LUT_IQ_ADDR7_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR7_2_3_FIELD_TX_11B_LUT_IQ_ADDR7_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_0_1 0x18C8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_0_1_FIELD_TX_11B_LUT_IQ_ADDR8_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_0_1_FIELD_TX_11B_LUT_IQ_ADDR8_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_2_3 0x18CC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_2_3_FIELD_TX_11B_LUT_IQ_ADDR8_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR8_2_3_FIELD_TX_11B_LUT_IQ_ADDR8_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_0_1 0x18D0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_0_1_FIELD_TX_11B_LUT_IQ_ADDR9_0_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_0_1_FIELD_TX_11B_LUT_IQ_ADDR9_1_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_2_3 0x18D4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_2_3_FIELD_TX_11B_LUT_IQ_ADDR9_2_MASK                                    0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR9_2_3_FIELD_TX_11B_LUT_IQ_ADDR9_3_MASK                                    0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_0_1 0x18D8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_0_1_FIELD_TX_11B_LUT_IQ_ADDR10_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_0_1_FIELD_TX_11B_LUT_IQ_ADDR10_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_2_3 0x18DC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_2_3_FIELD_TX_11B_LUT_IQ_ADDR10_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR10_2_3_FIELD_TX_11B_LUT_IQ_ADDR10_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_0_1 0x18E0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_0_1_FIELD_TX_11B_LUT_IQ_ADDR11_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_0_1_FIELD_TX_11B_LUT_IQ_ADDR11_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_2_3 0x18E4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_2_3_FIELD_TX_11B_LUT_IQ_ADDR11_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR11_2_3_FIELD_TX_11B_LUT_IQ_ADDR11_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_0_1 0x18E8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_0_1_FIELD_TX_11B_LUT_IQ_ADDR12_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_0_1_FIELD_TX_11B_LUT_IQ_ADDR12_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_2_3 0x18EC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_2_3_FIELD_TX_11B_LUT_IQ_ADDR12_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR12_2_3_FIELD_TX_11B_LUT_IQ_ADDR12_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_0_1 0x18F0 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_0_1_FIELD_TX_11B_LUT_IQ_ADDR13_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_0_1_FIELD_TX_11B_LUT_IQ_ADDR13_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_2_3 0x18F4 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_2_3_FIELD_TX_11B_LUT_IQ_ADDR13_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR13_2_3_FIELD_TX_11B_LUT_IQ_ADDR13_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_0_1 0x18F8 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_0_1_FIELD_TX_11B_LUT_IQ_ADDR14_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_0_1_FIELD_TX_11B_LUT_IQ_ADDR14_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_2_3 0x18FC */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_2_3_FIELD_TX_11B_LUT_IQ_ADDR14_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR14_2_3_FIELD_TX_11B_LUT_IQ_ADDR14_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_0_1 0x1900 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_0_1_FIELD_TX_11B_LUT_IQ_ADDR15_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_0_1_FIELD_TX_11B_LUT_IQ_ADDR15_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_2_3 0x1904 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_2_3_FIELD_TX_11B_LUT_IQ_ADDR15_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR15_2_3_FIELD_TX_11B_LUT_IQ_ADDR15_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_0_1 0x1908 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_0_1_FIELD_TX_11B_LUT_IQ_ADDR16_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_0_1_FIELD_TX_11B_LUT_IQ_ADDR16_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_2_3 0x190C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_2_3_FIELD_TX_11B_LUT_IQ_ADDR16_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR16_2_3_FIELD_TX_11B_LUT_IQ_ADDR16_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_0_1 0x1910 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_0_1_FIELD_TX_11B_LUT_IQ_ADDR17_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_0_1_FIELD_TX_11B_LUT_IQ_ADDR17_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_2_3 0x1914 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_2_3_FIELD_TX_11B_LUT_IQ_ADDR17_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR17_2_3_FIELD_TX_11B_LUT_IQ_ADDR17_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_0_1 0x1918 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_0_1_FIELD_TX_11B_LUT_IQ_ADDR18_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_0_1_FIELD_TX_11B_LUT_IQ_ADDR18_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_2_3 0x191C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_2_3_FIELD_TX_11B_LUT_IQ_ADDR18_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR18_2_3_FIELD_TX_11B_LUT_IQ_ADDR18_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_0_1 0x1920 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_0_1_FIELD_TX_11B_LUT_IQ_ADDR19_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_0_1_FIELD_TX_11B_LUT_IQ_ADDR19_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_2_3 0x1924 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_2_3_FIELD_TX_11B_LUT_IQ_ADDR19_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR19_2_3_FIELD_TX_11B_LUT_IQ_ADDR19_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_0_1 0x1928 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_0_1_FIELD_TX_11B_LUT_IQ_ADDR20_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_0_1_FIELD_TX_11B_LUT_IQ_ADDR20_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_2_3 0x192C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_2_3_FIELD_TX_11B_LUT_IQ_ADDR20_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR20_2_3_FIELD_TX_11B_LUT_IQ_ADDR20_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_0_1 0x1930 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_0_1_FIELD_TX_11B_LUT_IQ_ADDR21_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_0_1_FIELD_TX_11B_LUT_IQ_ADDR21_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_2_3 0x1934 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_2_3_FIELD_TX_11B_LUT_IQ_ADDR21_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR21_2_3_FIELD_TX_11B_LUT_IQ_ADDR21_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_0_1 0x1938 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_0_1_FIELD_TX_11B_LUT_IQ_ADDR22_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_0_1_FIELD_TX_11B_LUT_IQ_ADDR22_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_2_3 0x193C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_2_3_FIELD_TX_11B_LUT_IQ_ADDR22_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR22_2_3_FIELD_TX_11B_LUT_IQ_ADDR22_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_0_1 0x1940 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_0_1_FIELD_TX_11B_LUT_IQ_ADDR23_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_0_1_FIELD_TX_11B_LUT_IQ_ADDR23_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_2_3 0x1944 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_2_3_FIELD_TX_11B_LUT_IQ_ADDR23_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR23_2_3_FIELD_TX_11B_LUT_IQ_ADDR23_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_0_1 0x1948 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_0_1_FIELD_TX_11B_LUT_IQ_ADDR24_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_0_1_FIELD_TX_11B_LUT_IQ_ADDR24_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_2_3 0x194C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_2_3_FIELD_TX_11B_LUT_IQ_ADDR24_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR24_2_3_FIELD_TX_11B_LUT_IQ_ADDR24_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_0_1 0x1950 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_0_1_FIELD_TX_11B_LUT_IQ_ADDR25_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_0_1_FIELD_TX_11B_LUT_IQ_ADDR25_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_2_3 0x1954 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_2_3_FIELD_TX_11B_LUT_IQ_ADDR25_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR25_2_3_FIELD_TX_11B_LUT_IQ_ADDR25_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_0_1 0x1958 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_0_1_FIELD_TX_11B_LUT_IQ_ADDR26_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_0_1_FIELD_TX_11B_LUT_IQ_ADDR26_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_2_3 0x195C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_2_3_FIELD_TX_11B_LUT_IQ_ADDR26_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR26_2_3_FIELD_TX_11B_LUT_IQ_ADDR26_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_0_1 0x1960 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_0_1_FIELD_TX_11B_LUT_IQ_ADDR27_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_0_1_FIELD_TX_11B_LUT_IQ_ADDR27_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_2_3 0x1964 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_2_3_FIELD_TX_11B_LUT_IQ_ADDR27_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR27_2_3_FIELD_TX_11B_LUT_IQ_ADDR27_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_0_1 0x1968 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_0_1_FIELD_TX_11B_LUT_IQ_ADDR28_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_0_1_FIELD_TX_11B_LUT_IQ_ADDR28_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_2_3 0x196C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_2_3_FIELD_TX_11B_LUT_IQ_ADDR28_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR28_2_3_FIELD_TX_11B_LUT_IQ_ADDR28_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_0_1 0x1970 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_0_1_FIELD_TX_11B_LUT_IQ_ADDR29_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_0_1_FIELD_TX_11B_LUT_IQ_ADDR29_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_2_3 0x1974 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_2_3_FIELD_TX_11B_LUT_IQ_ADDR29_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR29_2_3_FIELD_TX_11B_LUT_IQ_ADDR29_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_0_1 0x1978 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_0_1_FIELD_TX_11B_LUT_IQ_ADDR30_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_0_1_FIELD_TX_11B_LUT_IQ_ADDR30_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_2_3 0x197C */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_2_3_FIELD_TX_11B_LUT_IQ_ADDR30_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR30_2_3_FIELD_TX_11B_LUT_IQ_ADDR30_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_0_1 0x1980 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_0_1_FIELD_TX_11B_LUT_IQ_ADDR31_0_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_0_1_FIELD_TX_11B_LUT_IQ_ADDR31_1_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_2_3 0x1984 */
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_2_3_FIELD_TX_11B_LUT_IQ_ADDR31_2_MASK                                  0x0000007F
#define B0_PHY_TXB_IF_TX_11B_LUT_IQ_ADDR31_2_3_FIELD_TX_11B_LUT_IQ_ADDR31_3_MASK                                  0x00007F00
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_PHY_DIS_DLY 0x1988 */
#define B0_PHY_TXB_IF_MODEM_11B_PHY_DIS_DLY_FIELD_MODEM_11B_PHY_DIS_DLY_MASK                                      0x00000FFF
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_PHY_ENA_DLY 0x198C */
#define B0_PHY_TXB_IF_MODEM_11B_PHY_ENA_DLY_FIELD_MODEM_11B_PHY_ENA_DLY_MASK                                      0x00000FFF
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_SERVICE_DATA 0x1990 */
#define B0_PHY_TXB_IF_MODEM_11B_SERVICE_DATA_FIELD_MODEM_11B_SERVICE_DATA_MASK                                    0x000000FF
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_PHY_END_DLY 0x1994 */
#define B0_PHY_TXB_IF_MODEM_11B_PHY_END_DLY_FIELD_MODEM_11B_PHY_END_DLY_MASK                                      0x0000000F
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_LO 0x1998 */
#define B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_LO_FIELD_MODEM_11B_RAS_POWER_TH_LO_MASK                              0x0000FFFF
//========================================
/* REG_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_HI 0x199C */
#define B0_PHY_TXB_IF_MODEM_11B_RAS_POWER_TH_HI_FIELD_MODEM_11B_RAS_POWER_TH_HI_MASK                              0x0000003F
//========================================
/* REG_PHY_TXB_IF_MODEM_CLK_CTRL 0x19A0 */
#define B0_PHY_TXB_IF_MODEM_CLK_CTRL_FIELD_MODEM_11B_STB_40_ENABLE_MASK                                           0x00000001
#define B0_PHY_TXB_IF_MODEM_CLK_CTRL_FIELD_CLK_44_GEN_ENABLE_MASK                                                 0x00000002
#define B0_PHY_TXB_IF_MODEM_CLK_CTRL_FIELD_CLK_44_GEN_DIV_0_MASK                                                  0x000000F0
#define B0_PHY_TXB_IF_MODEM_CLK_CTRL_FIELD_CLK_44_GEN_DIV_1_MASK                                                  0x00000F00
#define B0_PHY_TXB_IF_MODEM_CLK_CTRL_FIELD_CLK_44_GEN_LENGTH_MASK                                                 0x0000F000
//========================================
/* REG_PHY_TXB_IF_FRONTENDCTRL 0x19A4 */
#define B0_PHY_TXB_IF_FRONTENDCTRL_FIELD_INT_DCDISB_MASK                                                          0x00000001
#define B0_PHY_TXB_IF_FRONTENDCTRL_FIELD_INT_SCALDISB_MASK                                                        0x00000002
#define B0_PHY_TXB_IF_FRONTENDCTRL_FIELD_INT_MULTSCALDISB_MASK                                                    0x00000004
#define B0_PHY_TXB_IF_FRONTENDCTRL_FIELD_INT_MULTSCAL_MASK                                                        0x000003F8
#define B0_PHY_TXB_IF_FRONTENDCTRL_FIELD_INT_DCESTTIME_MASK                                                       0x00000400
//========================================
/* REG_PHY_TXB_IF_MDMBCNTL00 0x19A8 */
#define B0_PHY_TXB_IF_MDMBCNTL00_FIELD_INT_PREPRE_MASK                                                            0x0000003F
#define B0_PHY_TXB_IF_MDMBCNTL00_FIELD_INT_TXC2DISB_MASK                                                          0x00000080
#define B0_PHY_TXB_IF_MDMBCNTL00_FIELD_INT_SFDLEN_MASK                                                            0x00000700
#define B0_PHY_TXB_IF_MDMBCNTL00_FIELD_INT_INTERFILDISB_MASK                                                      0x00000800
#define B0_PHY_TXB_IF_MDMBCNTL00_FIELD_INT_SFDERR_MASK                                                            0x00007000
//========================================
/* REG_PHY_TXB_IF_MDMBCNTL01 0x19AC */
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_SCRAMBDISB_MASK                                                        0x00000001
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_SPREADDISB_MASK                                                        0x00000002
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_FIRDISB_MASK                                                           0x00000004
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_EQDISB_MASK                                                            0x00000008
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_COMPDISB_MASK                                                          0x00000010
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_DCOFFDISB_MASK                                                         0x00000020
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_PRECOMPDISB_MASK                                                       0x00000040
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_GAINDISB_MASK                                                          0x00000080
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_IQMMDISB_MASK                                                          0x00000100
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_INTERPDISB_MASK                                                        0x00000200
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_RXC2DISB_MASK                                                          0x00000400
#define B0_PHY_TXB_IF_MDMBCNTL01_FIELD_INT_TLOCKDISB_MASK                                                         0x00000800
//========================================
/* REG_PHY_TXB_IF_MDMBPRMINIT 0x19B0 */
#define B0_PHY_TXB_IF_MDMBPRMINIT_FIELD_INT_ALPHA_MASK                                                            0x00000003
#define B0_PHY_TXB_IF_MDMBPRMINIT_FIELD_INT_BETA_MASK                                                             0x00000030
#define B0_PHY_TXB_IF_MDMBPRMINIT_FIELD_INT_MU_MASK                                                               0x00000300
#define B0_PHY_TXB_IF_MDMBPRMINIT_FIELD_INT_RHO_MASK                                                              0x00003000
//========================================
/* REG_PHY_TXB_IF_MDMBTALPHA 0x19B4 */
#define B0_PHY_TXB_IF_MDMBTALPHA_FIELD_INT_TALPHA0_MASK                                                           0x0000000F
#define B0_PHY_TXB_IF_MDMBTALPHA_FIELD_INT_TALPHA1_MASK                                                           0x000000F0
#define B0_PHY_TXB_IF_MDMBTALPHA_FIELD_INT_TALPHA2_MASK                                                           0x00000F00
#define B0_PHY_TXB_IF_MDMBTALPHA_FIELD_INT_TALPHA3_MASK                                                           0x0000F000
//========================================
/* REG_PHY_TXB_IF_MDMBTBETA 0x19B8 */
#define B0_PHY_TXB_IF_MDMBTBETA_FIELD_INT_TBETA0_MASK                                                             0x0000000F
#define B0_PHY_TXB_IF_MDMBTBETA_FIELD_INT_TBETA1_MASK                                                             0x000000F0
#define B0_PHY_TXB_IF_MDMBTBETA_FIELD_INT_TBETA2_MASK                                                             0x00000F00
#define B0_PHY_TXB_IF_MDMBTBETA_FIELD_INT_TBETA3_MASK                                                             0x0000F000
//========================================
/* REG_PHY_TXB_IF_MDMBTMU 0x19BC */
#define B0_PHY_TXB_IF_MDMBTMU_FIELD_INT_TMU0_MASK                                                                 0x0000000F
#define B0_PHY_TXB_IF_MDMBTMU_FIELD_INT_TMU1_MASK                                                                 0x000000F0
#define B0_PHY_TXB_IF_MDMBTMU_FIELD_INT_TMU2_MASK                                                                 0x00000F00
#define B0_PHY_TXB_IF_MDMBTMU_FIELD_INT_TMU3_MASK                                                                 0x0000F000
//========================================
/* REG_PHY_TXB_IF_MDMBCNTL1 0x19C0 */
#define B0_PHY_TXB_IF_MDMBCNTL1_FIELD_INT_RXMAXLENGTH_MASK                                                        0x00000FFF
#define B0_PHY_TXB_IF_MDMBCNTL1_FIELD_INT_RXLENCHKEN_MASK                                                         0x00001000
#define B0_PHY_TXB_IF_MDMBCNTL1_FIELD_INT_DSCRMODESEL_MASK                                                        0x00002000
//========================================
/* REG_PHY_TXB_IF_MDMBRFCNTL 0x19C4 */
#define B0_PHY_TXB_IF_MDMBRFCNTL_FIELD_INT_TXCONST_MASK                                                           0x000000FF
#define B0_PHY_TXB_IF_MDMBRFCNTL_FIELD_INT_TXENDDEL_MASK                                                          0x0000FF00
//========================================
/* REG_PHY_TXB_IF_MDMBCCA 0x19C8 */
#define B0_PHY_TXB_IF_MDMBCCA_FIELD_INT_CCAMODE_MASK                                                              0x00000700
//========================================
/* REG_PHY_TXB_IF_MDMBEQCNTL0 0x19CC */
#define B0_PHY_TXB_IF_MDMBEQCNTL0_FIELD_INT_EQTIME_MASK                                                           0x0000000F
#define B0_PHY_TXB_IF_MDMBEQCNTL0_FIELD_INT_ESTTIME_MASK                                                          0x000003E0
#define B0_PHY_TXB_IF_MDMBEQCNTL0_FIELD_INT_COMPTIME_MASK                                                         0x00007C00
//========================================
/* REG_PHY_TXB_IF_MDMBEQCNTL1 0x19D0 */
#define B0_PHY_TXB_IF_MDMBEQCNTL1_FIELD_INT_EQHOLD_MASK                                                           0x00000FFF
//========================================
/* REG_PHY_TXB_IF_MDMBCNTL20 0x19D4 */
#define B0_PHY_TXB_IF_MDMBCNTL20_FIELD_INT_LOOPTIME_MASK                                                          0x0000000F
#define B0_PHY_TXB_IF_MDMBCNTL20_FIELD_INT_SYNCTIME_MASK                                                          0x00003F00
//========================================
/* REG_PHY_TXB_IF_MDMBCNTL21 0x19D8 */
#define B0_PHY_TXB_IF_MDMBCNTL21_FIELD_INT_PRECOMP_MASK                                                           0x0000003F
#define B0_PHY_TXB_IF_MDMBCNTL21_FIELD_INT_MAXSTAGE_MASK                                                          0x00003F00
//========================================
/* REG_PHY_TXB_IF_MDMBSTAT00 0x19DC */
#define B0_PHY_TXB_IF_MDMBSTAT00_FIELD_REG_DCOFFSETI_MASK                                                         0x0000003F
#define B0_PHY_TXB_IF_MDMBSTAT00_FIELD_REG_DCOFFSETQ_MASK                                                         0x00003F00
//========================================
/* REG_PHY_TXB_IF_MDMBSTAT01 0x19E0 */
#define B0_PHY_TXB_IF_MDMBSTAT01_FIELD_REG_EQSUMI_MASK                                                            0x000000FF
#define B0_PHY_TXB_IF_MDMBSTAT01_FIELD_REG_EQSUMQ_MASK                                                            0x0000FF00
//========================================
/* REG_PHY_TXB_IF_MDMBSTAT1 0x19E4 */
#define B0_PHY_TXB_IF_MDMBSTAT1_FIELD_REG_FREQOFFESTIM_MASK                                                       0x000000FF
#define B0_PHY_TXB_IF_MDMBSTAT1_FIELD_REG_IQGAINESTIM_MASK                                                        0x00007F00
//========================================
/* REG_PHY_TXB_IF_TX_SYNC_FIFO 0x19E8 */
#define B0_PHY_TXB_IF_TX_SYNC_FIFO_FIELD_GEN_11B_EVEN_STRB_EN_MASK                                                0x00000001
//========================================
/* REG_PHY_TXB_IF_RRC_N1 0x19EC */
#define B0_PHY_TXB_IF_RRC_N1_FIELD_RRC_N1_ANT0_MASK                                                               0x00000007
#define B0_PHY_TXB_IF_RRC_N1_FIELD_RRC_N1_ANT1_MASK                                                               0x00000070
#define B0_PHY_TXB_IF_RRC_N1_FIELD_RRC_N1_ANT2_MASK                                                               0x00000700
#define B0_PHY_TXB_IF_RRC_N1_FIELD_RRC_N1_ANT3_MASK                                                               0x00007000
//========================================
/* REG_PHY_TXB_IF_RAS_DETECTION_ANT_MASK 0x19F0 */
#define B0_PHY_TXB_IF_RAS_DETECTION_ANT_MASK_FIELD_RAS_DETECTION_ANT_MASK_MASK                                    0x0000000F
//========================================
/* REG_PHY_TXB_IF_TX_11B_DELAY_ENABLE 0x19F4 */
#define B0_PHY_TXB_IF_TX_11B_DELAY_ENABLE_FIELD_TX_11B_DELAY_ENABLE_MASK                                          0x00000001
//========================================
/* REG_PHY_TXB_IF_TX_11B_DELAY_2TX 0x19F8 */
#define B0_PHY_TXB_IF_TX_11B_DELAY_2TX_FIELD_TX_11B_DELAY_2TX_MASK                                                0x000000FF
//========================================
/* REG_PHY_TXB_IF_TX_11B_DELAY_3TX 0x19FC */
#define B0_PHY_TXB_IF_TX_11B_DELAY_3TX_FIELD_TX_11B_DELAY_3TX_MASK                                                0x00000FFF
//========================================
/* REG_PHY_TXB_IF_TX_11B_DELAY_4TX 0x1A00 */
#define B0_PHY_TXB_IF_TX_11B_DELAY_4TX_FIELD_TX_11B_DELAY_4TX_MASK                                                0x0000FFFF


#endif // _PHY_TXB_IF_REGS_H_
