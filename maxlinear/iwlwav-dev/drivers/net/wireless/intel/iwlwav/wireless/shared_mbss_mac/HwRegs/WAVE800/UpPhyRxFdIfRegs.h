
/***********************************************************************************
File:				UpPhyRxFdIfRegs.h
Module:				upPhyRxFdIf
SOC Revision:		latest
Generated at:       2024-06-26 12:55:30
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _UP_PHY_RX_FD_IF_REGS_H_
#define _UP_PHY_RX_FD_IF_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define UP_PHY_RX_FD_IF_BASE_ADDRESS                MEMORY_MAP_UNIT_204_BASE_ADDRESS
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF100                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1400)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF101                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1404)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF102                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1408)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF103                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x140C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF104                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1410)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF_RISC_CAUSE_INT    (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1414)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF106                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1418)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF107                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x141C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF108                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1420)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF109                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1424)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x142C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10C                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1430)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1434)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1438)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x143C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF111                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1444)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF112                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1448)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF113                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x144C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x146C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1474)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11E                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1478)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x147C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF120                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1480)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF121                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1484)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF122                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1488)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF124                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1490)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF125                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1494)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF126                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1498)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF127                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x149C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF128                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14A0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF129                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14A4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12A                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14A8)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14AC)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12C                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14B0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14B4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14BC)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF134                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14D0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF135                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14D4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF136                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14D8)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14DC)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF138                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14E0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF139                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14E4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13A                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14E8)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14EC)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13C                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14F0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14F4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14F8)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x14FC)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF140                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1500)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF141                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1504)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF142                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1508)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF143                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x150C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF144                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1510)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF145                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1514)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF146                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1518)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF147                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x151C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF148                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1520)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x152C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14C                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1530)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1534)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14E                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1538)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x153C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF150                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1540)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF151                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1544)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF152                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1548)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF153                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x154C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF154                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1550)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF155                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1554)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF156                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1558)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF157                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x155C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15A                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1568)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x156C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15C                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1570)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15D                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1574)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15E                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1578)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15F                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x157C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF160                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1580)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF161                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1584)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF162                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1588)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF163                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x158C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF164                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1590)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF165                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1594)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF166                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x1598)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF167                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x159C)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF168                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15A0)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF169                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15A4)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16A                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15A8)
#define	REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B                (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15AC)
#define	REG_UP_PHY_RX_FD_IF_CT_DATA_MU                    (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15B0)
#define	REG_UP_PHY_RX_FD_IF_GEAR_SHIFT_SYM_NUM_DFE        (UP_PHY_RX_FD_IF_BASE_ADDRESS + 0x15B4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF100 0x1400 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg : 16; //FD sub blocks sw reset , One bit per sub block. Resets only the core logic. Programming model is unchaged. Writing "0" resets the sub block and holds it reset. Writing "1" resets the sub block and releases the resets autumaticaly. , [0]- fd csm , [1]- fft_0 , [2]- fft_1 , [3]- fft_2 , [4]-fft_3 , [5]- bin chooser , [6]- nco , [7]- pilot scrambler , [8]- phase tracking , [9]- organizer , [10]- channel tracking , [11]- channel smoothing , [12]- timing estimation , [13]- gsm , [14]- slicer , [15]- qr, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf100_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF101 0x1404 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 blocksEn : 16; //FD_sub blocks en. One bit per sub block. Enables or disables the sub block  , [0]- fd csm , [1]- fft_0 , [2]- fft_1 , [3]- fft_2 , [4]- fft_2 , [5]- bin chooser , [6]- nco , [7]- pilot scrambler , [8]- phase tracking , [9]- organizer , [10]- qr , [11]- channel smoothing , [12]- timing estimation , [13]- gsm , [14]- slicer , [15]- qr, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf101_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF102 0x1408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetGenerate : 16; //A SW reset procedure will be initiated by writing to this register to the bits that equal 1, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf102_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF103 0x140C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmSwResetMask : 16; //Which block to reset at the falling edge of the global enable. One bit per sub block - same mapping as in sw_reset_n_reg register, reset value: 0x5FFF, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf103_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF104 0x1410 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscResetModemP : 1; //Reset Modem Command From Risc. pulse, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf104_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF_RISC_CAUSE_INT 0x1414 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfdRiscCauseIntClear_rxfdRiscCauseInt : 10; //RXFD Risc Cause intterupt - , bit[9] - he packet , bit[8] - vht packet , bit[7] - effective snr , bit[6] - last symbol , bit[5] - fd_stm_aborted , bit[4] - td_stm_aborted , bit[3] - fd symbol ready , bit[2] - sym_strobe to td , bit[1] - green field packet , bit[0] - high throughput packet, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIfRiscCauseInt_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF106 0x1418 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfdRiscIntEnable : 10; //RXFD Risc intterupt enable , bit[9] - he packet , bit[8] - vht packet , bit[7] - effective snr , bit[6] - last symbol , bit[5] - fd_stm_aborted , bit[4] - td_stm_aborted , bit[3] - fd symbol ready , bit[2] - sym_strobe to td , bit[1] - green field packet , bit[0] - high throughput packet, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf106_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF107 0x141C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmTdNextState : 4; //GSM Td Next State, reset value: 0x0, access type: RO
		uint32 gsmFdNextState : 4; //GSM Fd Next State, reset value: 0x0, access type: RO
		uint32 rxfdCsm : 5; //RXFD control_stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf107_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF108 0x1420 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 antennaEn : 5; //Sanity Risc Antenna Enable , Risc Decision , (bit per antenna) , reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 antennaEnValid : 1; //Sanity Risc Antenna Enable Valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf108_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF109 0x1424 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataMu : 4; //Channel Tracking data Coefficient Mu, reset value: 0x0, access type: RW
		uint32 ctPilotMu : 4; //Channel Tracking pilot Coefficient Mu, reset value: 0x0, access type: RW
		uint32 ftShifter : 4; //Frequency Tracking shifter, reset value: 0x0, access type: RW
		uint32 lltfChSmoothBypass : 1; //L-LTF Channel smoothing Bypass, reset value: 0x1, access type: RW
		uint32 lltfTeBypass : 1; //L-LTF timing estimation bypass, reset value: 0x0, access type: RW
		uint32 htltfTeBypass : 1; //HT-LTF timing estimation bypass, reset value: 0x0, access type: RW
		uint32 htltfChSmoothBypass : 1; //HT-LTF Channel smoothing Bypass, reset value: 0x1, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf109_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10B 0x142C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phaseSmoothingSize : 5; //Phase Smoothing Size, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 timingSmoothingSize : 5; //Timing Smoothing Size, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf10B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10C 0x1430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 hModeCntRisc : 3; //h_mode counter, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf10C_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D 0x1434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscTdGlobalEn : 1; //risc override the td global enable, reset value: 0x0, access type: RW
		uint32 riscFdGlobalEn : 1; //risc override the fd global enable, reset value: 0x0, access type: RW
		uint32 riscBeGlobalEn : 1; //risc override the be global enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 riscTdGlobalEnSel : 1; //TD global enable selection: , 0- hw , 1-sw risc, reset value: 0x0, access type: RW
		uint32 riscFdGlobalEnSel : 1; //FD global enable selection: , 0- hw , 1-sw risc, reset value: 0x0, access type: RW
		uint32 riscBeGlobalEnSel : 1; //BE global enable selection: , 0- hw , 1-sw risc, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf10D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E 0x1438 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htMode : 1; //ht mode: , 0-NOT ht , 1-HT mode, reset value: 0x0, access type: RO
		uint32 gfDetected : 1; //green field detection result: , 1- green field detected, reset value: 0x0, access type: RO
		uint32 vhtMode : 1; //vht mode, reset value: 0x0, access type: RO
		uint32 heMode : 1; //he mode, reset value: 0x0, access type: RO
		uint32 ehtMode : 1; //HT detector metric information, reset value: 0x0, access type: RO
		uint32 htMetrics : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf10E_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F 0x143C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mcs32Rev0 : 1; //mcs32 backward compatible for rev_0, reset value: 0x0, access type: RW
		uint32 gfNcoBpEn : 1; //enable nco_bypass for ht_detector input data in L_sig (do not rotate the data_bins for the ht detector) , reset value: 0x0, access type: RW
		uint32 htNcoBpEn : 1; //enable nco_bypass for ht_detector input data in ht_sig1 (do not rotate the data_bins for the ht detector) , reset value: 0x0, access type: RW
		uint32 vhtNcoBpEn : 1; //enable nco_bypass for ht_detector input data in ht_sig2 (do not rotate the data_bins for the ht detector) , reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf10F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF111 0x1444 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscNumSymbols : 15; //number of symbols by RISC (override air time calculation), reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf111_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF112 0x1448 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 12;
		uint32 riscNumSymbolsValid : 1; //number of symbols by RISC valid, reset value: 0x0, access type: RW
		uint32 riscNumSymbolsSel : 1; //Select number of symbols source , 0-Hw , 1-Risc, reset value: 0x0, access type: RW
		uint32 fdTimRevertEn : 1; //FD Timing revert enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf112_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF113 0x144C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 4;
		uint32 teOffset : 4; //timing estimation offset, reset value: 0x0, access type: RW
		uint32 reserved1 : 24;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf113_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B 0x146C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gclkDebugBypass2 : 12; //One bit per gator. , [0]- fd mu csm , [1]- mu phases db , [2]- fdelta snr db , [3]- su phases db , [4]- tcr , [5]- phase align , [6]- impl channel prep , [7]- average snr calc , [8]- bf rx parser , , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rfGclkDebugBypass : 1; //Register File gclk_debug_bypass, reset value: 0x1, access type: RW
		uint32 freqTrckGclkDebugBypass : 1; //freq_trck_gclk_debug_bypass, reset value: 0x1, access type: RW
		uint32 riscIf2BiuGclkBypass : 1; //risc_if2biu gclk bypass, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf11B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11D 0x1474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gclkDebugBypass : 16; //One bit per gator. , [0]- fd csm , [1]- fft_0 , [2]- fft_1 , [3]- fft_2 , [4]-fft_3 , [5]- bin chooser , [6]- nco , [7]- pilot scrambler , [8]- phase tracking , [9]- organizer , [10]- channel tracking , [11]- channel smoothing , [12]- timing estimation , [13]- gsm , [14]- slicer , [15]- qr, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf11D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11E 0x1478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timingShiftAccum : 7; //timing shift accumulator - for metrics, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf11E_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11F 0x147C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 an0Magnitude : 16; //fd_timing - an0 magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf11F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF120 0x1480 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 an1Magnitude : 16; //fd_timing - an1 magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf120_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF121 0x1484 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 an2Magnitude : 16; //fd_timing - an2 magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf121_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF122 0x1488 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isHtGfMagnitude : 16; //gf/ht/vht detection magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf122_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF124 0x1490 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 9;
		uint32 effectiveSnrMcs : 7; //effective snr module result - effective_snr_mcs withput the msb (rank bit), reset value: 0x0, access type: RO
		uint32 reserved1 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf124_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF125 0x1494 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pgc1Ant0 : 6; //agc calculation - pgc1 ant0 , reset value: 0x0, access type: RW
		uint32 pgc2Ant0 : 6; //agc calculation - pgc2 ant0 , reset value: 0x0, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf125_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF126 0x1498 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pgc3Ant0 : 6; //agc calculation - pgc3 ant0 , reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 lnaAnt0 : 5; //agc calculation - lna ant0 , reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf126_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF127 0x149C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pgc1Ant1 : 6; //agc calculation - pgc1 ant1, reset value: 0x0, access type: RW
		uint32 pgc2Ant1 : 6; //agc calculation - pgc2 ant1, reset value: 0x0, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf127_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF128 0x14A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pgc3Ant1 : 6; //agc calculation - pgc3 ant1, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 lnaAnt1 : 5; //agc calculation - lna ant1, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf128_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF129 0x14A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pgc1Ant2 : 6; //agc calculation - pgc1 ant2, reset value: 0x0, access type: RW
		uint32 pgc2Ant2 : 6; //agc calculation - pgc2 ant2, reset value: 0x0, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf129_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12A 0x14A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pgc3Ant2 : 6; //agc calculation - pgc3 ant2, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 lnaAnt2 : 5; //agc calculation - lna ant2, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf12A_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12B 0x14AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 implChannPreFlatCorrectionAnt0 : 8; //flat correction calculation for implicit channel preparation antenna0, reset value: 0x0, access type: RW
		uint32 implChannPreFlatCorrectionAnt1 : 8; //flat correction calculation for implicit channel preparation antenna1, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf12B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12C 0x14B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 implChannPreFlatCorrectionAnt2 : 8; //flat correction calculation for implicit channel preparation antenna2, reset value: 0x0, access type: RW
		uint32 implChannPreFlatCorrectionAnt3 : 8; //flat correction calculation for implicit channel preparation antenna3, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf12C_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D 0x14B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 flatCorrectionCalcChPreUnitDone : 1; //this signal rise when genrisc finnish correction calculation , reset value: 0x0, access type: RW
		uint32 explImplNMode : 1; //0 - implicit bf mode , 1- explicit bf mode, reset value: 0x0, access type: RW
		uint32 genriscBfEn : 1; //bf enable, reset value: 0x1, access type: RW
		uint32 genriscUploadChEn : 1; //bf_rx_parser upload the channel, reset value: 0x0, access type: RW
		uint32 mcsFeedbackDisable : 1; //1 - ignore e_snr mcs decision, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf12D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12F 0x14BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 11;
		uint32 noiseTrkGainInMin : 5; //noise_trk_gain_in_min, reset value: 0x5, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf12F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF134 0x14D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 noiseTrkNoiseIdentityReg : 8; //noise_trk_noise_identity_reg, reset value: 0x10, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf134_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF135 0x14D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 an3Magnitude : 16; //fd_timing - an3 magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf135_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF136 0x14D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 an4Magnitude : 16; //fd_timing - an4 magnitude value, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf136_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137 0x14DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixLltfValid : 1; //fd_timing lltf result valid , reset value: 0x0, access type: RO
		uint32 timFixHtltfValid : 1; //fd_timing ht_ltf result valid , reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf137_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF138 0x14E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscEnableSphere : 1; //enable sphere, reset value: 0x1, access type: RW
		uint32 riscEnableLinear : 1; //enable linear, reset value: 0x1, access type: RW
		uint32 riscSumLlr : 1; //sum LLRs from linear and sphere, reset value: 0x1, access type: RW
		uint32 reserved0 : 1;
		uint32 riscForceNtGainCalc : 1; //force noise tracking gain calculation, reset value: 0x0, access type: RW
		uint32 riscForceNtGainCalc1Ss : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
		uint32 sphereMode : 2; //sphere_mode 0:regular,1:fastmode1,2:fastmode2, reset value: 0x0, access type: RW
		uint32 reserved2 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf138_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF139 0x14E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 linearViterbiShift : 10; //linear_viterbi_shift, reset value: 0xe, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf139_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13A 0x14E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerLoadGainStream0 : 12; //Power load gain for adaptive filter (Tx) stream 0 , reset value: 0x400, access type: RW
		uint32 powerLoadGainReady : 1; //Power load gain for adaptive filter is ready in registers for all streams, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13A_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13B 0x14EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerLoadGainStream1 : 12; //Power load gain for adaptive filter (Tx) stream 1 , reset value: 0x400, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13C 0x14F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerLoadGainStream2 : 12; //Power load gain for adaptive filter (Tx) stream 2 , reset value: 0x400, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13C_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13D 0x14F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 powerLoadGainStream3 : 12; //Power load gain for adaptive filter (Tx) stream 3 , reset value: 0x400, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E 0x14F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sigbMuData1Ss : 4; //Channel Tracking Coefficient Mu for SIGB 1ss, reset value: 0x1, access type: RW
		uint32 sigbMuData2Ss : 4; //Channel Tracking Coefficient Mu for SIGB 2ss, reset value: 0x2, access type: RW
		uint32 sigbMuData3Ss : 4; //Channel Tracking Coefficient Mu for SIGB 3ss, reset value: 0x3, access type: RW
		uint32 sigbMuData4Ss : 4; //Channel Tracking Coefficient Mu for SIGB 4ss, reset value: 0x3, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13E_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13F 0x14FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereViterbiShift : 10; //sphere_viterbi_shift, reset value: 0xe, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf13F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF140 0x1500 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bypassSmoothing : 4; //Bypass smoothing - bit per Spatial Stream, reset value: 0x0, access type: RW
		uint32 chEstiSmoothingCsd : 1; //CSD compensation in smoothing unit -remove CSD before smoothing and return value (reverse) after smoothing, reset value: 0x1, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf140_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF141 0x1504 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pgc1Ant3 : 6; //agc calculation - pgc1 ant3 , reset value: 0x0, access type: RW
		uint32 pgc2Ant3 : 6; //agc calculation - pgc2 ant3 , reset value: 0x0, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf141_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF142 0x1508 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pgc3Ant3 : 6; //agc calculation - pgc3 ant3 , reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 lnaAnt3 : 5; //agc calculation - lna ant3 , reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf142_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF143 0x150C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscStartOrganizer : 1; //risc_start_organizer (set when 'fine' is ready), reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf143_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF144 0x1510 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 payloadSymbolNum : 15; //payload_symbol_number, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf144_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF145 0x1514 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSwResetMask : 16; //Which units to software reset at at the beginning of Tx , [0]- fd csm , [1]- fft_0 , [2]- fft_1 , [3]- fft_2 , [4]-fft_3 , [5]- bin chooser , [6]- nco , [7]- pilot scrambler , [8]- phase tracking , [9]- organizer , [10]- channel tracking , [11]- channel smoothing , [12]- timing estimation , [13]- gsm , [14]- slicer/qr, reset value: 0x7FE1, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf145_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF146 0x1518 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gearShiftSymNum : 10; //The symbol number for changing the mu (Start counting from first DATA symbol), reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf146_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF147 0x151C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataMuGearShift : 4; //Channel Tracking Mu value for data to change after reaching the gear shift symbol, reset value: 0x0, access type: RW
		uint32 ctPilotMuGearShift : 4; //Channel Tracking Mu value for pilots to change after reaching the gear shift symbol, reset value: 0x0, access type: RW
		uint32 ftShifterGearShift : 4; //Frequency Tracking shift value when reaching the gear shift symbol, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf147_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF148 0x1520 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fdSymbolType : 5; //FD symbol type (from GSM), reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 fdBw : 3; //FD bandwidth , 1- 20M , 2 - 40M , 3 - 80M, reset value: 0x0, access type: RO
		uint32 reserved1 : 21;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf148_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14B 0x152C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTrainingMode : 1; //MU training mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf14B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14C 0x1530 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muStartTrainingMode : 1; //start MU training operation, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf14C_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14D 0x1534 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTrainingDone : 1; //mu training done, reset value: 0x1, access type: RO
		uint32 muTrainingIndex : 10; //mu training index (effective rate memory address), reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf14D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14E 0x1538 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUsersValidityReg : 16; //Reflecting the users validity in training. Bit per user, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf14E_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14F 0x153C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muRiscUsersValidityReg : 16; //The GenRisc reads the mu_users_validity_reg and write back to this register with option of modifying, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf14F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF150 0x1540 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTrainingBw : 2; //mu_training_bw, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf150_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF151 0x1544 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtPwrLDb0 : 5; //tx_ct_pwr_l_db0, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 txCtPwrLDb1 : 5; //tx_ct_pwr_l_db1, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf151_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF152 0x1548 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txCtPwrLDb2 : 5; //tx_ct_pwr_l_db2, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 txCtPwrLDb3 : 5; //tx_ct_pwr_l_db3, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf152_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF153 0x154C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetNReg2 : 16; //FD sub blocks sw reset , One bit per sub block. Resets only the core logic. Programming model is unchaged. Writing "0" resets the sub block and holds it reset. Writing "1" resets the sub block and releases the resets autumaticaly. , [0]- fd mu csm , [1]- mu phases db , [2]- fdelta snr db , [3]- su phases db , [4]- tcr , [5]- phase align , [6]- impl channel prep , [7]- average snr calc , [8]- bf rx parser , , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf153_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF154 0x1550 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 blocksEn2 : 16; //FD_sub blocks en. One bit per sub block. Enables or disables the sub block  , [0]- fd mu csm , [1]- mu phases db , [2]- fdelta snr db , [3]- su phases db , [4]- tcr , [5]- phase align , [6]- impl channel prep , [7]- average snr calc , [8]- bf rx parser , , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf154_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF155 0x1554 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetGenerate2 : 16; //A SW reset procedure will be initiated by writing to this register to the bits that equal 1, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf155_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF156 0x1558 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gsmSwResetMask2 : 16; //Which block to reset at the falling edge of the global enable. One bit per sub block - same mapping as in sw_reset_n_reg2 register, reset value: 0x7, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf156_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF157 0x155C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSwResetMask2 : 16; //Which units to software reset at at the beginning of Tx , [0]- fd mu csm , [1]- phases db , [2]- delta snr db, reset value: 0x7, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf157_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15A 0x1568 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfdMuNdp : 1; //rxfd_mu_ndp, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15A_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15B 0x156C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sphereActivationSymNum : 10; //The symbol number for activating sphere (Start counting from first DATA symbol), reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15B_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15C 0x1570 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muSphereActivationSymNum : 10; //MU - the symbol number for activating sphere (Start counting from first DATA symbol), reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15C_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15D 0x1574 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfdMuCsm : 3; //RXFD MU control stm, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15D_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15E 0x1578 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTxErrorCause : 12; //Cause indication to MU Tx error - , {user_3_nr_error ,user_2_nr_error ,user_1_nr_error ,user_0_nr_error, user_3_nc_error ,user_2_nc_error ,user_1_nc_error ,user_0_nc_error}, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15E_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15F 0x157C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 riscSuClearAll : 1; //clear su db valid, reset value: 0x0, access type: RW
		uint32 riscMuClearAll : 1; //clear mu db valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf15F_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF160 0x1580 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixAn0 : 10; //fd_timing result antenna_0, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf160_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF161 0x1584 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixAn1 : 10; //fd_timing result antenna_1, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf161_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF162 0x1588 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixAn2 : 10; //fd_timing result antenna_2, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf162_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF163 0x158C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixAn3 : 10; //fd_timing result antenna_3, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf163_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF164 0x1590 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hypRxfdClrRu : 1; //clear smooth ru ram, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf164_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF165 0x1594 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muTxErrorCause2 : 13; //Cause indication to MU Tx error 2nd : {user_bw_error_8       user_3_bw_error_6  ,   user_2_bw_error_6 , user_1_bw_error_6 , user_0_bw_error_6 ,user_3_bw_error_4  ,   user_2_bw_error_4 , user_1_bw_error_4 , user_0_bw_error_4 }, reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf165_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF166 0x1598 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 channelTrackingSigbBpsk12Mu : 4; //channel tracking data mu during VHT-SIG-B bpsk1/2, reset value: 0x0, access type: RW
		uint32 channelTrackingSigbBpsk12MuEn : 1; //Enable channel tracking data mu during VHT-SIG-B bpsk1/2, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf166_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF167 0x159C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 pgc1Ant4 : 6; //agc calculation - pgc1 ant4, reset value: 0x0, access type: RW
		uint32 pgc2Ant4 : 6; //agc calculation - pgc2 ant4, reset value: 0x0, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf167_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF168 0x15A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pgc3Ant4 : 6; //agc calculation - pgc3 ant4, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 lnaAnt4 : 5; //agc calculation - lna ant4, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf168_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF169 0x15A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 implChannPreFlatCorrectionAnt4 : 8; //flat correction calculation for implicit channel preparation antenna4, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf169_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16A 0x15A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timFixAn4 : 10; //fd_timing result antenna_4, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf16A_u;

/*REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B 0x15AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 amplitudeCorrectionEn : 1; //Phase tracking amplitude correction enable, reset value: 0x0, access type: RW
		uint32 amplitudeCorrectionAlfa : 3; //Phase tracking amplitude correction alfa, reset value: 0x0, access type: RW
		uint32 amplitudeCorrectionMinNss : 2; //Phase tracking amplitude correction minimum NSS, reset value: 0x0, access type: RW
		uint32 amplitudeCorrectionMinMt : 4; //Phase tracking amplitude correction minimum modulation, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfPhyRxfdIf16B_u;

/*REG_UP_PHY_RX_FD_IF_CT_DATA_MU 0x15B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctDataMuDfe : 4; //Phase tracking amplitude correction enable, reset value: 0x0, access type: RW
		uint32 ctDataMuAfterRewind : 4; //Phase tracking amplitude correction alfa, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegUpPhyRxFdIfCtDataMu_u;

/*REG_UP_PHY_RX_FD_IF_GEAR_SHIFT_SYM_NUM_DFE 0x15B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gearShiftSymNumDfe : 10; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegUpPhyRxFdIfGearShiftSymNumDfe_u;

//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF100 0x1400 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF100_FIELD_SW_RESET_N_REG_MASK                                               0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF101 0x1404 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF101_FIELD_BLOCKS_EN_MASK                                                    0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF102 0x1408 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF102_FIELD_SW_RESET_GENERATE_MASK                                            0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF103 0x140C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF103_FIELD_GSM_SW_RESET_MASK_MASK                                            0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF104 0x1410 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF104_FIELD_RISC_RESET_MODEM_P_MASK                                           0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF_RISC_CAUSE_INT 0x1414 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF_RISC_CAUSE_INT_FIELD_RXFD_RISC_CAUSE_INT_CLEAR_MASK                        0x000003FF
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF_RISC_CAUSE_INT_FIELD_RXFD_RISC_CAUSE_INT_MASK                              0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF106 0x1418 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF106_FIELD_RXFD_RISC_INT_ENABLE_MASK                                         0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF107 0x141C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF107_FIELD_GSM_TD_NEXT_STATE_MASK                                            0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF107_FIELD_GSM_FD_NEXT_STATE_MASK                                            0x000000F0
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF107_FIELD_RXFD_CSM_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF108 0x1420 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF108_FIELD_ANTENNA_EN_MASK                                                   0x0000001F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF108_FIELD_ANTENNA_EN_VALID_MASK                                             0x00000100
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF109 0x1424 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_CT_DATA_MU_MASK                                                   0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_CT_PILOT_MU_MASK                                                  0x000000F0
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_FT_SHIFTER_MASK                                                   0x00000F00
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_LLTF_CH_SMOOTH_BYPASS_MASK                                        0x00001000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_LLTF_TE_BYPASS_MASK                                               0x00002000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_HTLTF_TE_BYPASS_MASK                                              0x00004000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF109_FIELD_HTLTF_CH_SMOOTH_BYPASS_MASK                                       0x00008000
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10B 0x142C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10B_FIELD_PHASE_SMOOTHING_SIZE_MASK                                         0x0000001F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10B_FIELD_TIMING_SMOOTHING_SIZE_MASK                                        0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10C 0x1430 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10C_FIELD_H_MODE_CNT_RISC_MASK                                              0x00000070
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D 0x1434 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_TD_GLOBAL_EN_MASK                                            0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_FD_GLOBAL_EN_MASK                                            0x00000002
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_BE_GLOBAL_EN_MASK                                            0x00000004
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_TD_GLOBAL_EN_SEL_MASK                                        0x00000010
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_FD_GLOBAL_EN_SEL_MASK                                        0x00000020
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D_FIELD_RISC_BE_GLOBAL_EN_SEL_MASK                                        0x00000040
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E 0x1438 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_HT_MODE_MASK                                                      0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_GF_DETECTED_MASK                                                  0x00000002
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_VHT_MODE_MASK                                                     0x00000004
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_HE_MODE_MASK                                                      0x00000008
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_EHT_MODE_MASK                                                     0x00000010
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10E_FIELD_HT_METRICS_MASK                                                   0x0000FFE0
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F 0x143C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F_FIELD_MCS32_REV_0_MASK                                                  0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F_FIELD_GF_NCO_BP_EN_MASK                                                 0x00000002
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F_FIELD_HT_NCO_BP_EN_MASK                                                 0x00000004
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF10F_FIELD_VHT_NCO_BP_EN_MASK                                                0x00000008
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF111 0x1444 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF111_FIELD_RISC_NUM_SYMBOLS_MASK                                             0x00007FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF112 0x1448 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF112_FIELD_RISC_NUM_SYMBOLS_VALID_MASK                                       0x00001000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF112_FIELD_RISC_NUM_SYMBOLS_SEL_MASK                                         0x00002000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF112_FIELD_FD_TIM_REVERT_EN_MASK                                             0x00004000
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF113 0x144C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF113_FIELD_TE_OFFSET_MASK                                                    0x000000F0
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B 0x146C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B_FIELD_GCLK_DEBUG_BYPASS2_MASK                                           0x00000FFF
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B_FIELD_RF_GCLK_DEBUG_BYPASS_MASK                                         0x00002000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B_FIELD_FREQ_TRCK_GCLK_DEBUG_BYPASS_MASK                                  0x00004000
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11B_FIELD_RISC_IF2BIU_GCLK_BYPASS_MASK                                      0x00008000
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11D 0x1474 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11D_FIELD_GCLK_DEBUG_BYPASS_MASK                                            0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11E 0x1478 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11E_FIELD_TIMING_SHIFT_ACCUM_MASK                                           0x0000007F
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11F 0x147C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF11F_FIELD_AN0_MAGNITUDE_MASK                                                0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF120 0x1480 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF120_FIELD_AN1_MAGNITUDE_MASK                                                0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF121 0x1484 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF121_FIELD_AN2_MAGNITUDE_MASK                                                0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF122 0x1488 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF122_FIELD_IS_HT_GF_MAGNITUDE_MASK                                           0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF124 0x1490 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF124_FIELD_EFFECTIVE_SNR_MCS_MASK                                            0x0000FE00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF125 0x1494 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF125_FIELD_PGC1_ANT0_MASK                                                    0x000000FC
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF125_FIELD_PGC2_ANT0_MASK                                                    0x00003F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF126 0x1498 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF126_FIELD_PGC3_ANT0_MASK                                                    0x0000003F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF126_FIELD_LNA_ANT0_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF127 0x149C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF127_FIELD_PGC1_ANT1_MASK                                                    0x000000FC
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF127_FIELD_PGC2_ANT1_MASK                                                    0x00003F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF128 0x14A0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF128_FIELD_PGC3_ANT1_MASK                                                    0x0000003F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF128_FIELD_LNA_ANT1_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF129 0x14A4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF129_FIELD_PGC1_ANT2_MASK                                                    0x000000FC
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF129_FIELD_PGC2_ANT2_MASK                                                    0x00003F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12A 0x14A8 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12A_FIELD_PGC3_ANT2_MASK                                                    0x0000003F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12A_FIELD_LNA_ANT2_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12B 0x14AC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12B_FIELD_IMPL_CHANN_PRE_FLAT_CORRECTION_ANT0_MASK                          0x000000FF
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12B_FIELD_IMPL_CHANN_PRE_FLAT_CORRECTION_ANT1_MASK                          0x0000FF00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12C 0x14B0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12C_FIELD_IMPL_CHANN_PRE_FLAT_CORRECTION_ANT2_MASK                          0x000000FF
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12C_FIELD_IMPL_CHANN_PRE_FLAT_CORRECTION_ANT3_MASK                          0x0000FF00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D 0x14B4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D_FIELD_FLAT_CORRECTION_CALC_CH_PRE_UNIT_DONE_MASK                        0x00000004
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D_FIELD_EXPL_IMPL_N_MODE_MASK                                             0x00000008
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D_FIELD_GENRISC_BF_EN_MASK                                                0x00000010
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D_FIELD_GENRISC_UPLOAD_CH_EN_MASK                                         0x00000020
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12D_FIELD_MCS_FEEDBACK_DISABLE_MASK                                         0x00000040
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF12F 0x14BC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF12F_FIELD_NOISE_TRK_GAIN_IN_MIN_MASK                                        0x0000F800
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF134 0x14D0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF134_FIELD_NOISE_TRK_NOISE_IDENTITY_REG_MASK                                 0x000000FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF135 0x14D4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF135_FIELD_AN3_MAGNITUDE_MASK                                                0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF136 0x14D8 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF136_FIELD_AN4_MAGNITUDE_MASK                                                0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137 0x14DC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF137_FIELD_TIM_FIX_LLTF_VALID_MASK                                           0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF137_FIELD_TIM_FIX_HTLTF_VALID_MASK                                          0x00000002
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF138 0x14E0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_RISC_ENABLE_SPHERE_MASK                                           0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_RISC_ENABLE_LINEAR_MASK                                           0x00000002
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_RISC_SUM_LLR_MASK                                                 0x00000004
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_RISC_FORCE_NT_GAIN_CALC_MASK                                      0x00000010
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_RISC_FORCE_NT_GAIN_CALC_1SS_MASK                                  0x00000020
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF138_FIELD_SPHERE_MODE_MASK                                                  0x00000C00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF139 0x14E4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF139_FIELD_LINEAR_VITERBI_SHIFT_MASK                                         0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13A 0x14E8 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13A_FIELD_POWER_LOAD_GAIN_STREAM_0_MASK                                     0x00000FFF
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13A_FIELD_POWER_LOAD_GAIN_READY_MASK                                        0x00001000
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13B 0x14EC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13B_FIELD_POWER_LOAD_GAIN_STREAM_1_MASK                                     0x00000FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13C 0x14F0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13C_FIELD_POWER_LOAD_GAIN_STREAM_2_MASK                                     0x00000FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13D 0x14F4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13D_FIELD_POWER_LOAD_GAIN_STREAM_3_MASK                                     0x00000FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E 0x14F8 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E_FIELD_SIGB_MU_DATA_1SS_MASK                                             0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E_FIELD_SIGB_MU_DATA_2SS_MASK                                             0x000000F0
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E_FIELD_SIGB_MU_DATA_3SS_MASK                                             0x00000F00
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13E_FIELD_SIGB_MU_DATA_4SS_MASK                                             0x0000F000
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF13F 0x14FC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF13F_FIELD_SPHERE_VITERBI_SHIFT_MASK                                         0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF140 0x1500 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF140_FIELD_BYPASS_SMOOTHING_MASK                                             0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF140_FIELD_CH_ESTI_SMOOTHING_CSD_MASK                                        0x00000010
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF141 0x1504 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF141_FIELD_PGC1_ANT3_MASK                                                    0x000000FC
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF141_FIELD_PGC2_ANT3_MASK                                                    0x00003F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF142 0x1508 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF142_FIELD_PGC3_ANT3_MASK                                                    0x0000003F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF142_FIELD_LNA_ANT3_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF143 0x150C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF143_FIELD_RISC_START_ORGANIZER_MASK                                         0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF144 0x1510 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF144_FIELD_PAYLOAD_SYMBOL_NUM_MASK                                           0x00007FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF145 0x1514 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF145_FIELD_TX_SW_RESET_MASK_MASK                                             0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF146 0x1518 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF146_FIELD_GEAR_SHIFT_SYM_NUM_MASK                                           0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF147 0x151C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF147_FIELD_CT_DATA_MU_GEAR_SHIFT_MASK                                        0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF147_FIELD_CT_PILOT_MU_GEAR_SHIFT_MASK                                       0x000000F0
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF147_FIELD_FT_SHIFTER_GEAR_SHIFT_MASK                                        0x00000F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF148 0x1520 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF148_FIELD_FD_SYMBOL_TYPE_MASK                                               0x0000001F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF148_FIELD_FD_BW_MASK                                                        0x00000700
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14B 0x152C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14B_FIELD_MU_TRAINING_MODE_MASK                                             0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14C 0x1530 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14C_FIELD_MU_START_TRAINING_MODE_MASK                                       0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14D 0x1534 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14D_FIELD_MU_TRAINING_DONE_MASK                                             0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14D_FIELD_MU_TRAINING_INDEX_MASK                                            0x000007FE
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14E 0x1538 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14E_FIELD_MU_USERS_VALIDITY_REG_MASK                                        0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF14F 0x153C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF14F_FIELD_MU_RISC_USERS_VALIDITY_REG_MASK                                   0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF150 0x1540 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF150_FIELD_MU_TRAINING_BW_MASK                                               0x00000003
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF151 0x1544 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF151_FIELD_TX_CT_PWR_L_DB0_MASK                                              0x0000001F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF151_FIELD_TX_CT_PWR_L_DB1_MASK                                              0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF152 0x1548 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF152_FIELD_TX_CT_PWR_L_DB2_MASK                                              0x0000001F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF152_FIELD_TX_CT_PWR_L_DB3_MASK                                              0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF153 0x154C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF153_FIELD_SW_RESET_N_REG2_MASK                                              0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF154 0x1550 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF154_FIELD_BLOCKS_EN2_MASK                                                   0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF155 0x1554 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF155_FIELD_SW_RESET_GENERATE2_MASK                                           0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF156 0x1558 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF156_FIELD_GSM_SW_RESET_MASK2_MASK                                           0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF157 0x155C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF157_FIELD_TX_SW_RESET_MASK2_MASK                                            0x0000FFFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15A 0x1568 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15A_FIELD_RXFD_MU_NDP_MASK                                                  0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15B 0x156C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15B_FIELD_SPHERE_ACTIVATION_SYM_NUM_MASK                                    0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15C 0x1570 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15C_FIELD_MU_SPHERE_ACTIVATION_SYM_NUM_MASK                                 0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15D 0x1574 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15D_FIELD_RXFD_MU_CSM_MASK                                                  0x00000007
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15E 0x1578 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15E_FIELD_MU_TX_ERROR_CAUSE_MASK                                            0x00000FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF15F 0x157C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15F_FIELD_RISC_SU_CLEAR_ALL_MASK                                            0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF15F_FIELD_RISC_MU_CLEAR_ALL_MASK                                            0x00000002
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF160 0x1580 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF160_FIELD_TIM_FIX_AN0_MASK                                                  0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF161 0x1584 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF161_FIELD_TIM_FIX_AN1_MASK                                                  0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF162 0x1588 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF162_FIELD_TIM_FIX_AN2_MASK                                                  0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF163 0x158C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF163_FIELD_TIM_FIX_AN3_MASK                                                  0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF164 0x1590 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF164_FIELD_HYP_RXFD_CLR_RU_MASK                                              0x00000001
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF165 0x1594 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF165_FIELD_MU_TX_ERROR_CAUSE_2_MASK                                          0x00001FFF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF166 0x1598 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF166_FIELD_CHANNEL_TRACKING_SIGB_BPSK1_2MU_MASK                              0x0000000F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF166_FIELD_CHANNEL_TRACKING_SIGB_BPSK1_2MU_EN_MASK                           0x00000010
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF167 0x159C */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF167_FIELD_PGC1_ANT4_MASK                                                    0x000000FC
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF167_FIELD_PGC2_ANT4_MASK                                                    0x00003F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF168 0x15A0 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF168_FIELD_PGC3_ANT4_MASK                                                    0x0000003F
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF168_FIELD_LNA_ANT4_MASK                                                     0x00001F00
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF169 0x15A4 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF169_FIELD_IMPL_CHANN_PRE_FLAT_CORRECTION_ANT4_MASK                          0x000000FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16A 0x15A8 */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF16A_FIELD_TIM_FIX_AN4_MASK                                                  0x000003FF
//========================================
/* REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B 0x15AC */
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B_FIELD_AMPLITUDE_CORRECTION_EN_MASK                                      0x00000001
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B_FIELD_AMPLITUDE_CORRECTION_ALFA_MASK                                    0x0000000E
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B_FIELD_AMPLITUDE_CORRECTION_MIN_NSS_MASK                                 0x00000030
#define B2_UP_PHY_RX_FD_IF_PHY_RXFD_IF16B_FIELD_AMPLITUDE_CORRECTION_MIN_MT_MASK                                  0x000003C0
//========================================
/* REG_UP_PHY_RX_FD_IF_CT_DATA_MU 0x15B0 */
#define B2_UP_PHY_RX_FD_IF_CT_DATA_MU_FIELD_CT_DATA_MU_DFE_MASK                                                   0x0000000F
#define B2_UP_PHY_RX_FD_IF_CT_DATA_MU_FIELD_CT_DATA_MU_AFTER_REWIND_MASK                                          0x000000F0
//========================================
/* REG_UP_PHY_RX_FD_IF_GEAR_SHIFT_SYM_NUM_DFE 0x15B4 */
#define B2_UP_PHY_RX_FD_IF_GEAR_SHIFT_SYM_NUM_DFE_FIELD_GEAR_SHIFT_SYM_NUM_DFE_MASK                               0x000003FF


#endif // _UP_PHY_RX_FD_IF_REGS_H_
