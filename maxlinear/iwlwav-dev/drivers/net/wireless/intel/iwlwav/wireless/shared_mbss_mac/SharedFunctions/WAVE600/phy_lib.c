#include "build_config.h"
#include "phy_lib.h"
#include "phy_access_definitions.h"
#include "lib_abb_api.h"
#include "PhyRxTdRegs.h"
#include "PhyRxFdRegs.h"
#include "PhyRxFdIfRegs.h"
#include "PhyRxBeIfRegs.h"
#include "PhyTxRegs.h"
#include "HwMemoryMap.h"
#include "PhyTxbIfRegs.h"
#include "EmeraldEnvRegs.h"
#include "PhyRxTdIfRegs.h"
#include "MacGeneralRegs.h"
#include "rfsys_common.h"
#ifdef ENET_INC_ARCH_WAVE600D2	
#include "PhyRxtdAnt0Regs.h"
#include "PhyRxtdAnt1Regs.h"
#include "PhyRxtdAnt2Regs.h"
#include "PhyRxtdAnt3Regs.h"
#include "PhyTxtdAnt0Regs.h"
#include "PhyRxBeRegs.h"
#include "lib_abb_api_legacy.h"
#include "PhyRxtdAntRegs.h"
#else
//#include "interface_abb.h"
//#include "abb_tb_defines.h"
#include "PhyRxTdAnt0Regs.h"
#include "PhyRxTdAnt1Regs.h"
#include "PhyRxTdAnt2Regs.h"
#include "PhyRxTdAnt3Regs.h"
#include "PhyTxTdAnt0Regs.h"
#include "PhyRxbeRegs.h"
#endif
#include "fast_mem_psd2mips.h"
#include "PhyRegsIncluder.h"
#if defined (WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && defined (ENET_INC_LMAC1)
#include "TxSender_ScratchPadApi.h"
#endif //(WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && (ENET_INC_LMAC1)
#include "loggerAPI.h"

#define FCSI_BUSY_WAIT_TIME		10000 //maximum wait time for FCSI busy filed in us

void PHY_SECURE_WRITE()
{
	PHY_WRITE_REG(REG_EMERALD_ENV_SECURE_WRITE, 0, 0xaaaa);
	PHY_WRITE_REG(REG_EMERALD_ENV_SECURE_WRITE, 0, 0x5555);
}

void PhyResetSet(PHY_Domain_e domain_to_reset)
{
	if (PHY_RXTD == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x0);//TD SW REset
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x0); // TD Block Enable
	}
	else if (PHY_RXFD == domain_to_reset)
	{   //Frequency  RISC Domain - reset release
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0x0); //FD SW REset
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0x0); // FD Block Enable
	}
	else if (PHY_RXBE == domain_to_reset)
	{   //Back End  RISC Domain - reset release
		PHY_WRITE_REG(REG_PHY_RX_BE_IF_PHY_RXBE_IF180, 0, 0x0); //BE SW REset
		PHY_WRITE_REG(REG_PHY_RX_BE_IF_PHY_RXBE_IF181, 0, 0x0); //BE Block Enable
	}
	else if (PHY_TX == domain_to_reset)
	{   /* release TX from reset */
		PHY_WRITE_REG(REG_PHY_TX_TX_SW_RESET, 0, 0x0); //TX SW REset
#ifndef ENET_INC_ARCH_WAVE600D2	
		PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_06, 0, 0x0); //TX SW REset
#endif
		PHY_WRITE_REG(REG_PHY_TX_TX_BLOCK_ENABLE, 0, 0x0); //TX Block Enable
#ifndef ENET_INC_ARCH_WAVE600D2	
		PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_14, 0, 0x0); //TX Block Enable 1
#endif
		/* release Modem 11B from reset */
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, 0x0); //TX SW REset
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C6, 0, 0x0); //TX Block Enable
	}
}

void PhyResetRelease(PHY_Domain_e domain_to_reset)
{
	if (PHY_RXTD == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xFFFFFFFF); //TD SW REset
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xFFFFFFFF); // TD Block Enable
	}
	else if (PHY_RXFD == domain_to_reset)
	{   //Frequency  RISC Domain - reset release
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0xFFFFFFFF); //FD SW REset
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0xFFFFFFFF); // FD Block Enable
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF153, 0, 0xFFFFFFFF); //FD SW REset
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF154, 0, 0xFFFFFFFF); // FD Block Enable
	}
	else if (PHY_RXBE == domain_to_reset)
	{   //Back End  RISC Domain - reset release
		PHY_WRITE_REG(REG_PHY_RX_BE_IF_PHY_RXBE_IF180, 0, 0xFFFFFFFF); //BE SW REset
		PHY_WRITE_REG(REG_PHY_RX_BE_IF_PHY_RXBE_IF181, 0, 0xFFFFFFFF); //BE Block Enable
	}
	else if (PHY_TX == domain_to_reset)
	{   /* release TX from reset */
		PHY_WRITE_REG(REG_PHY_TX_TX_SW_RESET, 0, 0xFFFFFFFF); //TX SW REset
#ifndef ENET_INC_ARCH_WAVE600D2	
		PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_06, 0, 0xFFFFFFFF); //TX SW REset
#endif
		PHY_WRITE_REG(REG_PHY_TX_TX_BLOCK_ENABLE, 0, 0xFFFFFFFF); //TX Block Enable
#ifndef ENET_INC_ARCH_WAVE600D2	
		PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_14, 0, 0xFFFFFFFF); //TX Block Enable 1
#endif
		/* release Modem 11B from reset */
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, 0xFFFFFFFF); //TX SW REset
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C6, 0, 0xFFFFFFFF); //TX Block Enable
	}
}

void DigitalLpbk()
{
	RegPhyRxFdIfPhyRxfdIf10D_u if10Dmask;
	uint32 mask;

	// turn on semafore 04, For Risc Usage, Risc in Loop back Mode
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF04, 0, 0x3);
	// FPGA Loop Back
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFFD, 0, 0x5555);
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF1B, 0, 1);

	if10Dmask.val = 0;
	if10Dmask.bitFields.riscTdGlobalEn = 1;
	if10Dmask.bitFields.riscFdGlobalEn = 1;
	if10Dmask.bitFields.riscBeGlobalEn = 1;
	if10Dmask.bitFields.riscTdGlobalEnSel = 1;
	if10Dmask.bitFields.riscFdGlobalEnSel = 1;
	if10Dmask.bitFields.riscBeGlobalEnSel = 1;

	PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF10D, 0, if10Dmask.val);
	/* fd gclk bypass (gsm) */
	mask = 1 << 13;
	PHY_READ_MODIFY_WRITE(REG_PHY_RX_FD_IF_PHY_RXFD_IF11D, 0, 13, mask, 1);
}

void PHY_Reset(uint32 band, uint32 value)
{
	RegMacGeneralPhyExtReset_u phy_ext_reset;
	
	PHY_READ_REG(REG_MAC_GENERAL_PHY_EXT_RESET, 0, &phy_ext_reset.val);
	if (band == 1)
	{
		phy_ext_reset.bitFields.macToPhyBand1ResetN = value;
	}
	else
	{
		phy_ext_reset.bitFields.macToPhyBand0ResetN = value;
	}
	PHY_WRITE_REG(REG_MAC_GENERAL_PHY_EXT_RESET, 0, phy_ext_reset.val);
}

void phy_disable()
{
	PhyResetSet(PHY_RXTD);
	PhyResetSet(PHY_RXBE);
	PhyResetSet(PHY_RXFD);
	PhyResetSet(PHY_TX);
}

void phy_enable()
{
	PhyResetRelease(PHY_RXTD);
	PhyResetRelease(PHY_RXBE);
	PhyResetRelease(PHY_RXFD);
	PhyResetRelease(PHY_TX);
}

#ifndef ENET_INC_ARCH_WAVE600D2	
void PhyResetStrobes()
{
	RegPhyTxTxBeReg04_u reg_val;
	uint32 i;

	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_04, 0, &reg_val.val);

	for(i = 0; i < 2; i++)
	{
		reg_val.bitFields.modem11BStb40Enable ^= 1;
		reg_val.bitFields.clk44GenEnable ^= 1;
		PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_04, 0, reg_val.val);
	}
}
#else
void PhyResetStrobes()
{
	RegPhyTxbIfModemClkCtrl_u reg_val;
	uint32 i;

	PHY_READ_REG(REG_PHY_TXB_IF_MODEM_CLK_CTRL, 0, &reg_val.val);

	for(i = 0; i < 2; i++)
	{
		reg_val.bitFields.modem11BStb40Enable ^= 1;
		reg_val.bitFields.clk44GenEnable ^= 1;
		PHY_WRITE_REG(REG_PHY_TXB_IF_MODEM_CLK_CTRL, 0, reg_val.val);
	}
}
#endif //ENET_INC_ARCH_WAVE600D2

bool PhyStartModem()
{
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG18, 0, 1);
	return TRUE;
}

	
void PhyRiscSyncResetReq(bool *success)
{
#ifdef ENET_INC_ARCH_WAVE600D2
	TX_INTERRUPT_SAVE_AREA;
	uint32 block_enable;
	uint32 genRiscResetCfm = GENRISC_FINISHED_LOADING_DEFAULT;
		
	uint32 startTime = 0 ;
	uint32 pollingTime = 0;
	(*success) = TRUE;
	
	PHY_READ_REG(GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);
	
	if (((block_enable & (0x1 << GENRISC_BLOCK_ENABLE_SHIFT))!=0) && (genRiscResetCfm != 0))
	{	
		OSAL_DISABLE_INTERRUPTS(&interrupt_save);
		startTime = GET_TSF_TIMER_LOW();
		PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF29, 0,0xffff);//set synched reset (interupt6)
		pollingTime =  GET_TSF_TIMER_LOW() - startTime;
		//poll for genRisc confirmation - synched reset is done
		while ((genRiscResetCfm != 0x0) && (pollingTime < GENRISC_FINISHED_LOADING__MICROS_TIMEOUT))
		{
			PHY_READ_REG(GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
			pollingTime =  GET_TSF_TIMER_LOW() - startTime;
		}
		OSAL_ENABLE_INTERRUPTS(interrupt_save);

		(*success) = (pollingTime < GENRISC_FINISHED_LOADING__MICROS_TIMEOUT) ? TRUE:FALSE;
	}
#else
	(*success) = TRUE;
#endif
}


void PhyStopRiscOperation()
{
	uint32 SW_reset;
	uint32 block_enable;
	
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xf7ffffff & SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xf7ffffff & block_enable); // Reset GenRisc done
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF84, 0, 1); //mask WD interrupt
}

void PhyStartRiscOperation()
{

	uint32 SW_reset;
	uint32 block_enable;

	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x08000000 | SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x08000000 | block_enable); // Reset GenRisc done
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG022, 0, 1); //Risc start operation command
	
#ifdef ENET_INC_ARCH_WAVE600D2
    PHY_DELAY(20);
#else
    PHY_DELAY(25);
#endif
}

void Phy_Config_802_11B()
{
	PHY_DELAY(36);
	PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS, 0, 0x100);
}


void PHY_MAC_BAND_CONFIG_REG_INIT()
{
	RegMacGeneralPhyBandConfig_u reg_val;
	reg_val.val = 0;

	PHY_WRITE_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, reg_val.val);
}

void PHY_SET_MAC_BAND(uint8 band)
{
	RegMacGeneralPhyBandConfig_u reg_val;

	PHY_READ_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, &reg_val.val);

	if (band == 0)
	{
		reg_val.bitFields.b0PhyActive = 1;
	}
	else if (band == 1)
	{
		reg_val.bitFields.b1PhyActive = 1;
	}
	PHY_WRITE_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, reg_val.val);
}
void PHY_SET_MAC_ANT_CONFIG(uint8 band, uint8 antMask)
{
		RegMacGeneralPhyBandConfig_u phyBandConfigReg;
		
#if defined (WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && defined (ENET_INC_LMAC1)
		uint8 numOfAntennas;	
		TxSender_ScratchPadApiParams_t *pScratchPadApiParams = (TxSender_ScratchPadApiParams_t *)(B1_MAC_GENRISC_TX_SPRAM_BASE_ADDR + (SCPAD_ADDRESS_TX_SENDER_SCRATCHPAD_API_STRUCTURE_START << 0x2));
#endif //(WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && (ENET_INC_LMAC1)
	
		/*Only one band should change the antenna configuration, the other one have the opposite configuration*/
		if (band == 0)
		{
			PHY_READ_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, &phyBandConfigReg.val);
			phyBandConfigReg.bitFields.phyAntenna0Sel = (!((antMask&0x1)>>0));
			phyBandConfigReg.bitFields.phyAntenna1Sel = (!((antMask&0x2)>>1));
			phyBandConfigReg.bitFields.phyAntenna2Sel = (!((antMask&0x4)>>2));
			phyBandConfigReg.bitFields.phyAntenna3Sel = (!((antMask&0x8)>>3));
#ifdef ENET_INC_ARCH_WAVE600D2
			phyBandConfigReg.bitFields.phyAntenna4Sel = (!((antMask&0x10)>>4));
#endif
			PHY_WRITE_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, phyBandConfigReg.val);
		}
		else if (band == 1)
		{
			PHY_READ_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, &phyBandConfigReg.val);
			phyBandConfigReg.bitFields.phyAntenna0Sel = (((antMask&0x1)>>0));
			phyBandConfigReg.bitFields.phyAntenna1Sel = (((antMask&0x2)>>1));
			phyBandConfigReg.bitFields.phyAntenna2Sel = (((antMask&0x4)>>2));
			phyBandConfigReg.bitFields.phyAntenna3Sel = (((antMask&0x8)>>3));
#ifdef ENET_INC_ARCH_WAVE600D2
			phyBandConfigReg.bitFields.phyAntenna4Sel = (((antMask&0x10)>>4));
#endif
			PHY_WRITE_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, phyBandConfigReg.val);
		}
#if defined (WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && defined (ENET_INC_LMAC1) //ask oren amitzur if its only 600A0 W/A
	numOfAntennas = (phyBandConfigReg.bitFields.phyAntenna0Sel + \
					phyBandConfigReg.bitFields.phyAntenna1Sel + \
					phyBandConfigReg.bitFields.phyAntenna2Sel + \
					phyBandConfigReg.bitFields.phyAntenna3Sel \
					);


	pScratchPadApiParams->numOfAllocatedAntennas = numOfAntennas;

#endif //(WORKAROUND_FOR_HW_BUG_IN_DMA_WRAPPER_BAND1) && (ENET_INC_LMAC1)
}

void PHY_RFIC_RESET(void)
{
	RegPhyRxFdIfPhyRxfdIf137_u reg137;

	PHY_READ_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
	reg137.bitFields.timFixLltfValid = 0;
	reg137.bitFields.timFixHtltfValid = 0;
	PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
}

void PHY_RFIC_RESET_RELEASE()
{
		RegPhyRxFdIfPhyRxfdIf137_u reg137;
	
		PHY_READ_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
		reg137.bitFields.timFixLltfValid = 1;
		reg137.bitFields.timFixHtltfValid = 1;
		PHY_WRITE_REG(REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
}

void PHY_ENABLE_RX(void)
{
	RegPhyRxTdPhyRxtdReg18_u reg18;

	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG18, 0, &reg18.val);
	reg18.bitFields.adcActiveReg = 1;
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG18, 0, reg18.val);
}

void PHY_PRE_CAL(void)
{
	RegPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyTxbIfPhyTxbRiscReg1C5_u reg1c5;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, &reg1c5.val);
	PHY_READ_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0xff;
	reg1c5.bitFields.swResetNReg = 0;
	reg80.bitFields.compress3AntEndAddr = 0x10;
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, reg1c5.val);
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
}

void PHY_POST_CAL(void)
{
	RegPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0;
	reg80.bitFields.compress3AntEndAddr = 0x3ef;
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
}

void PHY_SET_VHT_NONAGGREGATE(uint8 enable)
{
	RegPhyRxBePhyRxbeReg67_u reg67;

	PHY_READ_REG(REG_PHY_RX_BE_PHY_RXBE_REG67, 0, &reg67.val);
	reg67.bitFields.vhtNonAggregate = enable;
	PHY_WRITE_REG(REG_PHY_RX_BE_PHY_RXBE_REG67, 0, reg67.val);
}

void PHY_SET_RX_HALFBAND(uint8 halfbandtype)
{
	RegPhyRxTdPhyRxtdReg05_u reg05;

	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG05, 0, &reg05.val);
	reg05.bitFields.hbMode = halfbandtype;
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG05, 0, reg05.val);
}

void PHY_SET_RX_PRIMARY(uint8 rxprimarytype, uint8 antMask)
{
	RegPhyRxTdPhyRxtdReg07D_u reg7d;
	RegPhyTxbIfPhyTxbRiscReg1C1_u reg1C1;
	RegPhyRxtdAnt0PhyRxtdAntReg53_u reg53[MAX_NUM_OF_RX_ANTENNAS];

	// OFDM
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG07D, 0, &reg7d.val);
	reg7d.bitFields.lsbIsCtrlChannel    = ~(rxprimarytype & 0x1);
	reg7d.bitFields.lsb40IsCtrlChannel  = ~((rxprimarytype & 0x2) >> 1);
	reg7d.bitFields.lsb80IsCtrlChannel  = ~((rxprimarytype & 0x4) >> 2);
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG07D, 0, reg7d.val);

	// 11B
	READ_ARR_FROM_MASK_ANT_REGS(REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG53,(uint32*)reg53,antMask); //read from band antennas 
	reg53[ANTENNA_0].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1); //write to all array (some of the antenna are not relervant to this band
	reg53[ANTENNA_1].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
	reg53[ANTENNA_2].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
	reg53[ANTENNA_3].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
#if defined(RFIC_WRX600D2)
	reg53[ANTENNA_4].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
#endif
	WRITE_ARR_2_MASK_ANT_REGS(REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG53,(uint32*)reg53,antMask);//write from band antennas

	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, &reg1C1.val);
	reg1C1.bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);
	PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, reg1C1.val);
}

void PHY_SET_TX_HALFBAND(uint8 halfbandtype)
{
	RegPhyTxTxBeReg07_u reg07;
	RegPhyTxbIfPhyTxbRiscReg1C1_u reg1C1;

	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_07, 0, &reg07.val);
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, &reg1C1.val);

	switch (halfbandtype)
	{
		case 3:
			reg07.bitFields.txIsHalfBand20160 = 1;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			break;
		case 2:
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 1;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 1;
			break;
		case 1:
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 1;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			break;
		default:
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			break;
	}
	PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_07, 0, reg07.val);
	PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, reg1C1.val);
}

void PHY_SET_TX_PRIMARY(uint8 txprimarytype, uint8 halfbandtype, uint8 antMask)
{
	RegPhyTxtdAnt0TxtdMix11BFreqOffset_u Mix11BFreqOffsetReg[MAX_NUM_OF_TX_ANTENNAS];
	RegPhyTxTxBeReg48_u reg48;
	uint8 mix11bFreqOffset = 0;
	
	// OFDM
	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_48, 0, &reg48.val);
	reg48.bitFields.txSsb20  = (txprimarytype & 0x1);
	reg48.bitFields.txSsb40  = (txprimarytype & 0x2) >> 1;
	reg48.bitFields.txSsb80  = (txprimarytype & 0x4) >> 2;
	PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_48, 0, reg48.val);

	// 11B
	if (halfbandtype == 2) // 40 MHz
	{
		if (txprimarytype == 0)
		{
			mix11bFreqOffset = 3;	
		}
		else
		{
			mix11bFreqOffset = 1;	
		}
	}
	READ_ARR_FROM_MASK_ANT_REGS(REG_PHY_TXTD_ANT0_TXTD_MIX11B_FREQ_OFFSET,(uint32*)Mix11BFreqOffsetReg,antMask); //read from band antennas 
	Mix11BFreqOffsetReg[ANTENNA_0].bitFields.pmMix11BFreqOffset = mix11bFreqOffset; //write to all array (some of the antenna are not relervant to this band
	Mix11BFreqOffsetReg[ANTENNA_1].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
	Mix11BFreqOffsetReg[ANTENNA_2].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
	Mix11BFreqOffsetReg[ANTENNA_3].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
	WRITE_ARR_2_MASK_ANT_REGS(REG_PHY_TXTD_ANT0_TXTD_MIX11B_FREQ_OFFSET,(uint32*)Mix11BFreqOffsetReg,antMask);//write from band antennas	
}


void PHY_SET_11B_RX_ANT_SELECTION(uint8 antselection)
{
#ifndef ENET_INC_ARCH_WAVE600D2	
	RegPhyRxTdIfPhyRxtdIfd9_u regD9;
	
	PHY_READ_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, &regD9.val);
	regD9.bitFields.bOverrideRxAntSel = antselection;
	regD9.bitFields.bOverrideRxAntEn = 1;
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, regD9.val);
#else
	RegPhyTxbIfRasDetectionAntMask_u rasDetectionMaskReg;

	PHY_READ_REG(REG_PHY_TXB_IF_RAS_DETECTION_ANT_MASK, 0, &rasDetectionMaskReg.val);
	rasDetectionMaskReg.bitFields.rasDetectionAntMask = antselection;
	PHY_WRITE_REG(REG_PHY_TXB_IF_RAS_DETECTION_ANT_MASK, 0, rasDetectionMaskReg.val);
#endif
}

void PHY_SET_11B_TX_ANT_SELECTION(uint8 antSelection)
{
#ifndef ENET_INC_ARCH_WAVE600D2	
	RegPhyRxTdIfPhyRxtdIfd9_u regD9;
	
	PHY_READ_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, &regD9.val);
	regD9.bitFields.bOverrideTxAntSel = antSelection;
	regD9.bitFields.bOverrideTxAntEn = 1;
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, regD9.val);
#else
	RegPhyTxTxBeReg58_u phyTxBeReg58;

	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_58, 0, &phyTxBeReg58.val);
	phyTxBeReg58.bitFields.modem11BForceTransmitAnt = TRUE;
	phyTxBeReg58.bitFields.modem11BForceTransmitAntMask = antSelection;
	PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_58, 0, phyTxBeReg58.val);
#endif
}

#ifndef ENET_INC_ARCH_WAVE600D2	
void PHY_SET_11B_ROUNDROBIN(uint8 rate)
{
	RegPhyRxTdIfPhyRxtdIfd9_u regD9;
	RegPhyRxTdPhyRxtdReg034_u reg34;
	
	PHY_READ_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, &regD9.val);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG034, 0, &reg34.val);
	reg34.bitFields.switch11BCounterLimit = rate;
	regD9.bitFields.bOverrideRxAntEn = 0;
	regD9.bitFields.bOverrideTxAntEn = 0;
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IFD9, 0, regD9.val);
}
#endif

void PHY_CLEAR_PHY_INTERRUPTS(void)
{
	RegPhyRxTdIfPhyRxtdIf08_u reg08;
	RegPhyRxTdIfPhyRxtdIf85_u reg85;
	
	PHY_READ_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF08, 0, &reg08.val);
	PHY_READ_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF85, 0, &reg85.val);
	reg08.bitFields.semaphoreReg08 = 0;
	reg85.bitFields.clearWdInt = 1;
	reg85.bitFields.clearTxPacketInt = 1;
	reg85.bitFields.clearPhyMacInt = 1;
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF08, 0, reg08.val);
	PHY_WRITE_REG(REG_PHY_RX_TD_IF_PHY_RXTD_IF85, 0, reg85.val);
}

void PHY_SET_SPACELESS(void)
{
	RegPhyTxTxBeReg11_u reg11;
	
	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_11, 0, &reg11.val);
	reg11.bitFields.txEndlessMode = 1;
	PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_11, 0, reg11.val);
}

void PHY_READ_RSSI(uint8 * rssiValues,uint8 * energyEstNumOfSamples)
{
	RegPhyRxBeIfPhyRxbeIf184_u regVal;
	//uint32 value;
	uint8  ant;
#ifdef ENET_INC_ARCH_WAVE600D2
	for (ant = 0; ant < 5; ant++)
#else
	for (ant = 0; ant < 4; ant++)
#endif // 	ENET_INC_ARCH_WAVE600D2
	{
		PHY_READ_REG(REG_PHY_RX_BE_IF_PHY_RXBE_IF184, ant, &regVal.val);
		rssiValues[ant] = regVal.bitFields.ant0AdRssi;
	}
	//PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG13, 0, &value);
	*energyEstNumOfSamples = 1;
}

void PHY_SET_TX_ANT_OPERATION_SET(uint8 opSet)
{
	RegPhyTxTxBeReg07_u	txBeReg07;
	PHY_READ_REG(REG_PHY_TX_TX_BE_REG_07,0,&txBeReg07.val);
	txBeReg07.bitFields.tx0FeEnable = ((opSet & 0x1) >> 0);
	txBeReg07.bitFields.tx1FeEnable = ((opSet & 0x2) >> 1);
	txBeReg07.bitFields.tx2FeEnable = ((opSet & 0x4) >> 2);
	txBeReg07.bitFields.tx3FeEnable = ((opSet & 0x8) >> 3);
	PHY_WRITE_REG(REG_PHY_TX_TX_BE_REG_07,0,txBeReg07.val);
}

void PHY_SET_RX_ANT_OPERATION_SET(uint8 opSet)
{
	RegPhyRxTdPhyRxtdReg07B_u rxTdAntReg7B;
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG07B,0,&rxTdAntReg7B.val);
	rxTdAntReg7B.bitFields.hostAntennaEn = opSet;
	PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG07B,0,rxTdAntReg7B.val);
}
void PHY_SET_FREQUENCY(uint32 freq, uint8 antMask)
{
	RegPhyRxtdAnt0PhyRxtdAntReg50_u rxTdReg50[MAX_NUM_OF_RX_ANTENNAS];

	READ_ARR_FROM_MASK_ANT_REGS(REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);
	rxTdReg50[ANTENNA_0].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_1].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_2].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_3].bitFields.rfFreqMhz = freq;
#if defined(RFIC_WRX600D2)
	rxTdReg50[ANTENNA_4].bitFields.rfFreqMhz = freq;
#endif
	WRITE_ARR_2_MASK_ANT_REGS(REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);
}
void PHY_SET_BAND(uint8 band)
{
	PHY_WRITE_REG(ENABLE_802_11_H,0, band == BAND_5_2_GHZ); //set ENABLE_802_11_H to 1 if we are in 5 Giga
	PHY_WRITE_REG(ENABLE_11_B, 0, band == BAND_2_4_GHZ); //set ENABLE_11_B to 1 if we are in 2.4 Giga
}

void PHY_SET_BSS_COLOR(uint8 vapId, uint8 bssColor)
{
	RegPhyRxBePhyRxbeReg78_u rxBeReg78;
	uint32 regAddr;
	uint8 offsetInReg;	
	regAddr = REG_PHY_RX_BE_PHY_RXBE_REG78 + ( (vapId >> 2) << 2);
	
	PHY_READ_REG(regAddr,0,&rxBeReg78.val);
	offsetInReg = vapId % 4;
	switch (offsetInReg)
	{
		case 0:		
			rxBeReg78.bitFields.pmColour0 = bssColor;
			break;
		case 1:		
			rxBeReg78.bitFields.pmColour1 = bssColor;
			break;
		case 2:	
			rxBeReg78.bitFields.pmColour2 = bssColor;
			break;
		case 3:		
			rxBeReg78.bitFields.pmColour3 = bssColor;
			break;		
	}
	PHY_WRITE_REG(regAddr,0,rxBeReg78.val);
}

#ifdef ENET_INC_ARCH_WAVE600D2
void PHY_DISABLE_BSS_COLOR(void)
{
	RegPhyRxBeRxbeMaskUnsupportedMcs_u rxBeMaskUnsupportedMcs;
	
	//the value 0x40000 disable the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x40000;
	PHY_WRITE_REG(REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS,0,rxBeMaskUnsupportedMcs.val);
}

void PHY_ENABLE_BSS_COLOR(void)
{
	RegPhyRxBeRxbeMaskUnsupportedMcs_u rxBeMaskUnsupportedMcs;

	//the value 0 enables the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x0;
	PHY_WRITE_REG(REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS,0,rxBeMaskUnsupportedMcs.val);
}

#else
void PHY_DISABLE_BSS_COLOR(void)
{
	RegPhyRxBePhyRxbeReg41_u rxBeMaskUnsupportedMcs;

	PHY_READ_REG(REG_PHY_RX_BE_PHY_RXBE_REG41,0,&rxBeMaskUnsupportedMcs.val);
	//the value 0x2000 disables the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x2000;
	PHY_WRITE_REG(REG_PHY_RX_BE_PHY_RXBE_REG41,0,rxBeMaskUnsupportedMcs.val);
}

void PHY_ENABLE_BSS_COLOR(void)
{
	RegPhyRxBePhyRxbeReg41_u rxBeMaskUnsupportedMcs;
	
	PHY_READ_REG(REG_PHY_RX_BE_PHY_RXBE_REG41,0,&rxBeMaskUnsupportedMcs.val);
	//the value 0 enables the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x0;
	PHY_WRITE_REG(REG_PHY_RX_BE_PHY_RXBE_REG41,0,rxBeMaskUnsupportedMcs.val);
}

#endif

#ifdef ENET_INC_ARCH_WAVE600D2B
void PHY_SET_RX_DFS_600D2B(uint8 isRadarEnabled, uint32 freq, uint8 u8SpectrumMode)
{
	RegPhyRxtdAntPhyRxtdAntReg50_u    RxPhyDfsAntReg50;
	RegPhyRxTdPhyRxtdReg05_u          RxPhyDfsReg05;
	RegPhyRxTdPhyRxtdReg07B_u         RxPhyDfsReg07B;
	RegPhyRxTdPhyRxtdReg0Ba_u         RxPhyDfsReg0BA;
	RegPhyRxTdPhyRxtdReg0C7_u         RxPhyDfsReg0C7;

	PHY_READ_REG(REG_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50,0,&RxPhyDfsAntReg50.val);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG05,0,&RxPhyDfsReg05.val);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG07B,0,&RxPhyDfsReg07B.val);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG0BA,0,&RxPhyDfsReg0BA.val);
	PHY_READ_REG(REG_PHY_RX_TD_PHY_RXTD_REG0C7,0,&RxPhyDfsReg0C7.val);

	if (isRadarEnabled != 0)
	{

//		RxPhyDfsAntReg50.bitFields.rfFreqMhz = freq;
//		RxPhyDfsReg07B.bitFields.hostAntennaEn = 1;
//		RxPhyDfsReg05.bitFields.rfBw = u8SpectrumMode; 		//default is 160MHz (3)
//		RxPhyDfsReg05.bitFields.digitalBw = u8SpectrumMode; 	//default is 160MHz (3)
//		RxPhyDfsReg05.bitFields.rxAdcFs = u8SpectrumMode; 	//default is 160MHz (3)
//		RxPhyDfsReg05.bitFields.lowPowerMode = 0;

		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 1;				// general new radar HW enable bit
		RxPhyDfsReg0C7.bitFields.pmEventsRamMode = 1;  			// Set RAM mode to 1 (128 words for all subbands)
//		RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0xD; 	// enable all types for report except packet
		RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0x8; 	// enable only type radar

//		PHY_WRITE_REG(REG_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50, 0, RxPhyDfsAntReg50.val);
//		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG07B, 0, RxPhyDfsReg07B.val);
//		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG05, 0, RxPhyDfsReg05.val);
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG0C7, 0, RxPhyDfsReg0C7.val);
		// Write enable bit as last one
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);
	}
	else
	{
		UNUSED_PARAM(freq);
		UNUSED_PARAM(u8SpectrumMode);
		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 0;				// general new radar HW enable bit
		PHY_WRITE_REG(REG_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);
	}
}
#endif


