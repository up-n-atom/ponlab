#include "phy_lib.h"
#include "phy_access_definitions.h"
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
#include "PhyRxBeRegs.h"
#include "UpPhyRxFdIfRegs.h"
#include "DfsPhyRxTdRegs.h"
#include "DfsPhyRxtdAntRegs.h"
#include "fast_mem_psd2mips.h"
#include "fast_mem_psd2mips_dfs.h"
#include "PhyRegsIncluder.h"
#include "Utils_Api.h"
#include "ConfigurationManager_api.h"
#include "loggerAPI.h"
#define FCSI_BUSY_WAIT_TIME		10000 //maximum wait time for FCSI busy field in us

#define UP_OFFSET (B0_UP_PHY_RX_FD_BASE_ADDR - B0_PHY_RX_FD_BASE_ADDR) 

//Current situation, Driver loads LM0 and LM1 CPUs but only progmodels of Band0, therefore LM1 tries to write in progmodel not loaded causing an exception.
// Forcing PHY_REG_BAND_OFFSET to 0 until driver properly loads progmodel of all bands
#define PHY_REG_BAND_OFFSET 0 //  (ConfigurationManager_GetMyBand()*(B1_PHY_RX_TD_BASE_ADDR - B0_PHY_RX_TD_BASE_ADDR)) //Per band (0-2.4GHz, 1-5GHz, 2-6GHz) the address offset is [band * h400000].


uint32 gDbgDfsWritten = 0;	// ROBERT_DEBUG

void PHY_SECURE_WRITE()
{
	PHY_WRITE_REG(REG_EMERALD_ENV_SECURE_WRITE, 0, 0xaaaa);
	PHY_WRITE_REG(REG_EMERALD_ENV_SECURE_WRITE, 0, 0x5555);
}

void PhyResetSet(PHY_Domain_e domain_to_reset)
{
	if (PHY_RXTD == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x0);//TD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x0); // TD Block Enable
	}
	else if (PHY_DFS == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x0);//TD DFS SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x0); // TD DFS Block Enable
	}	
	else if (PHY_RXFD == domain_to_reset)
	{   //Frequency  RISC Domain - reset release
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0x0); //FD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0x0); // FD Block Enable

		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF153, 0, 0x0); //FD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF154, 0, 0x0); // FD Block Enable
		if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
		{
			/* set UP to reset */
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0x0); //FD SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0x0); //FD BLOCK Enable
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF153, 0, 0x0); //FD SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF154, 0, 0x0); //FD Block Enable
		}
	}
	else if (PHY_RXBE == domain_to_reset)
	{   //Back End  RISC Domain - reset release
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IF_PHY_RXBE_IF180, 0, 0x0); //BE SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IF_PHY_RXBE_IF181, 0, 0x0); //BE Block Enable
	}
	else if (PHY_TX == domain_to_reset)
	{   /* release TX from reset */
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_SW_RESET, 0, 0x0); //TX SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BLOCK_ENABLE, 0, 0x0); //TX Block Enable
		if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
		{
			/* release Modem 11B from reset */
			PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, 0x0); //TX SW REset
			PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C6, 0, 0x0); //TX Block Enable
		}
		else if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
		{
			/* set UP to reset reset */
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_SW_RESET, 0, 0x0); //TX SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BLOCK_ENABLE, 0, 0x0); //TX Block Enable
		}
	}
}

void PhyResetRelease(PHY_Domain_e domain_to_reset)
{
	if (PHY_RXTD == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xFFFFFFFF); //TD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xFFFFFFFF); // TD Block Enable
	}
	else if (PHY_DFS == domain_to_reset)
	{   //Time Domain
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xFFFFFFFF); //TD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xFFFFFFFF); // TD Block Enable
	}
	else if (PHY_RXFD == domain_to_reset)
	{   //Frequency  RISC Domain - reset release
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0xFFFFFFFF); //FD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0xFFFFFFFF); // FD Block Enable
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF153, 0, 0xFFFFFFFF); //FD SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF154, 0, 0xFFFFFFFF); // FD Block Enable
		if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
		{
			/* release UP from reset */
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF100, 0, 0xFFFFFFFF); //FD SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF101, 0, 0xFFFFFFFF); // FD Block Enable
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF153, 0, 0xFFFFFFFF); //FD SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF154, 0, 0xFFFFFFFF); // FD Block Enable
		}
	}
	else if (PHY_RXBE == domain_to_reset)
	{   //Back End  RISC Domain - reset release
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IF_PHY_RXBE_IF180, 0, 0xFFFFFFFF); //BE SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IF_PHY_RXBE_IF181, 0, 0xFFFFFFFF); //BE Block Enable
	}
	else if (PHY_TX == domain_to_reset)
	{   /* release TX from reset */
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_SW_RESET, 0, 0xFFFFFFFF); //TX SW REset
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BLOCK_ENABLE, 0, 0xFFFFFFFF); //TX Block Enable
		if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
		{		
			/* release Modem 11B from reset */
			PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, 0xFFFFFFFF); //TX SW REset
			PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C6, 0, 0xFFFFFFFF); //TX Block Enable
		}
		else if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
		{
			/* release UP from reset */
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_SW_RESET, 0, 0x0); //TX SW REset
			PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BLOCK_ENABLE, 0, 0x0); //TX Block Enable
		}
	}
}

void DigitalLpbk()
{
	RegPhyRxFdIfPhyRxfdIf10D_u if10Dmask;

	// turn on semafore 04, For Risc Usage, Risc in Loop back Mode
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF04, 0, 0x3);
	// FPGA Loop Back
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF1B, 0, 1);

	if10Dmask.val = 0;
	if10Dmask.bitFields.riscTdGlobalEn = 1;
	if10Dmask.bitFields.riscFdGlobalEn = 1;
	if10Dmask.bitFields.riscBeGlobalEn = 1;
	if10Dmask.bitFields.riscTdGlobalEnSel = 1;
	if10Dmask.bitFields.riscFdGlobalEnSel = 1;
	if10Dmask.bitFields.riscBeGlobalEnSel = 1;

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF10D, 0, if10Dmask.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF10D, 0, if10Dmask.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF11D, 0, 0xffffffff);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF11D, 0, 0xffffffff);
}

void PHY_Reset(uint32 band, uint32 value)
{
	RegMacGeneralPhyExtReset_u phy_ext_reset;

	PHY_READ_REG(REG_MAC_GENERAL_PHY_EXT_RESET, 0, &phy_ext_reset.val);

	//we assume here band_i corresponds to bit_i in above register
	SET_BIT_VALUE_IN_BITMAP(phy_ext_reset.val, band, value);

	PHY_WRITE_REG(REG_MAC_GENERAL_PHY_EXT_RESET, 0, phy_ext_reset.val);
}

// Disable DFS genrisc
void phy_disable_dfs (void)
{
	PhyResetSet(PHY_DFS);
}

void phy_disable (void)
{
	PhyResetSet(PHY_RXTD);
	PhyResetSet(PHY_RXBE);
	PhyResetSet(PHY_RXFD);
	PhyResetSet(PHY_TX);
}

// Enable DFS genrisc
void phy_enable_dfs (void)
{
	PhyResetRelease(PHY_DFS);
}

void phy_enable (void)
{
	PhyResetRelease(PHY_RXTD);
	PhyResetRelease(PHY_RXBE);
	PhyResetRelease(PHY_RXFD);
	PhyResetRelease(PHY_TX);
}


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

bool PhyStartModem()
{
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG18, 0, 1);
	return TRUE;
}


void PhyDfsRiscSyncResetReq(bool *success)
{
	TX_INTERRUPT_SAVE_AREA;
	uint32 block_enable;
	uint32 genRiscResetCfm = GENRISC_FINISHED_LOADING_DEFAULT;
		
	uint32 startTime = 0 ;
	uint32 pollingTime = 0;
	(*success) = TRUE;
	
	PHY_READ_REG(DFS_HW_GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);
	
	if (((block_enable & (0x1 << GENRISC_BLOCK_ENABLE_SHIFT))!=0) && (genRiscResetCfm != 0))
	{	
		OSAL_DISABLE_INTERRUPTS(&interrupt_save);
		startTime = GET_TSF_TIMER_LOW();
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF29, 0,0xffff);//set synched reset (interupt6)
		pollingTime =  GET_TSF_TIMER_LOW() - startTime;
		//poll for genRisc confirmation - synched reset is done
		while ((genRiscResetCfm != 0x0) && (pollingTime < GENRISC_FINISHED_LOADING__MICROS_TIMEOUT))
		{
			PHY_READ_REG(DFS_HW_GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
			pollingTime =  GET_TSF_TIMER_LOW() - startTime;
		}

		(*success) = (pollingTime < GENRISC_FINISHED_LOADING__MICROS_TIMEOUT) ? TRUE:FALSE;

		if((*success) == FALSE )
		{
			PHY_READ_REG(DFS_HW_GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
			ASSERT(genRiscResetCfm == 1);
			(*success) = TRUE;
		}
		OSAL_ENABLE_INTERRUPTS(interrupt_save);
	}
}
	
void PhyRiscSyncResetReq(bool *success)
{
	TX_INTERRUPT_SAVE_AREA;
	uint32 block_enable;
	uint32 genRiscResetCfm = GENRISC_FINISHED_LOADING_DEFAULT;
		
	uint32 startTime = 0 ;
	uint32 pollingTime = 0;
	(*success) = TRUE;
	
	PHY_READ_REG(GENRISC_FINISHED_LOADING, 0, &genRiscResetCfm);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);
	
	if (((block_enable & (0x1 << GENRISC_BLOCK_ENABLE_SHIFT))!=0) && (genRiscResetCfm != 0))
	{	
		OSAL_DISABLE_INTERRUPTS(&interrupt_save);
		startTime = GET_TSF_TIMER_LOW();
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF29, 0,0xffff);//set synched reset (interupt6)
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
}

// Stop DFS genrisc
void PhyStopDfsRiscOperation (void)
{
	uint32 SW_reset;
	uint32 block_enable;
	
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xf7ffffff & SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xf7ffffff & block_enable); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_IF_PHY_RXTD_IF84, 0, 1); //mask WD interrupt
}

void PhyStopRiscOperation()
{
	uint32 SW_reset;
	uint32 block_enable;
	
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0xf7ffffff & SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0xf7ffffff & block_enable); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF67, 0, 0x0); // disable watchdog done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF84, 0, 1); //mask WD interrupt
}

// Start DFS genrisc
void PhyStartDfsRiscOperation (void)
{
	uint32 SW_reset;
	uint32 block_enable;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x08000000 | SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x08000000 | block_enable); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_DFS_PHY_RX_TD_PHY_RXTD_REG022, 0, 1); //Risc start operation command
	PHY_DELAY(20);
}


void PhyStartRiscOperation()
{

	uint32 SW_reset;
	uint32 block_enable;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, &SW_reset);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, &block_enable);

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG00, 0, 0x08000000 | SW_reset); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG01, 0, 0x08000000 | block_enable); // Reset GenRisc done
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG022, 0, 1); //Risc start operation command
	PHY_DELAY(30);
}

void Phy_Config_802_11B()
{
	PHY_DELAY(36);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{	
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG_TXB_RISC_INT_STATUS, 0, 0x100);
	}
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
	else if (band == 2)
	{
		reg_val.bitFields.b2PhyActive = 1;
	}
	PHY_WRITE_REG(REG_MAC_GENERAL_PHY_BAND_CONFIG, 0, reg_val.val);
}



void PHY_RFIC_RESET(void)
{
	RegPhyRxFdIfPhyRxfdIf137_u reg137;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
	reg137.bitFields.timFixLltfValid = 0;
	reg137.bitFields.timFixHtltfValid = 0;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		/* reset UP from reset */
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
		reg137.bitFields.timFixLltfValid = 0;
		reg137.bitFields.timFixHtltfValid = 0;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
	}
}

void PHY_RFIC_RESET_RELEASE()
{
	RegPhyRxFdIfPhyRxfdIf137_u reg137;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
	reg137.bitFields.timFixLltfValid = 1;
	reg137.bitFields.timFixHtltfValid = 1;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		/* release UP from reset */
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, &reg137.val);
		reg137.bitFields.timFixLltfValid = 1;
		reg137.bitFields.timFixHtltfValid = 1;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_IF_PHY_RXFD_IF137, 0, reg137.val);
	}
}

void PHY_ENABLE_RX(void)
{
	RegPhyRxTdPhyRxtdReg18_u reg18;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG18, 0, &reg18.val);
	reg18.bitFields.adcActiveReg = 1;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG18, 0, reg18.val);
}


void PHY_ENABLE_RX_DFS(void)
{
	RegDfsPhyRxTdPhyRxtdReg18_u reg18;

	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG18, 0, &reg18.val);
	reg18.bitFields.adcActiveReg = 1;
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG18, 0, reg18.val);
}

void PHY_PRE_CAL(void)
{
	RegPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyTxbIfPhyTxbRiscReg1C5_u reg1c5;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, &reg1c5.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0xff;
	reg1c5.bitFields.swResetNReg = 0;
	reg80.bitFields.compress3AntEndAddr = 0x10;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{	
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, reg1c5.val);
	}
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
		reg80.bitFields.compress3AntEndAddr = 0x10;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	}
}

void PHY_POST_CAL(void)
{
	RegPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0;
	reg80.bitFields.compress3AntEndAddr = 0x3ef;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
		reg80.bitFields.compress3AntEndAddr = 0x3ef;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	}
}

void PHY_PRE_CAL_DFS(void)
{
	RegDfsPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyTxbIfPhyTxbRiscReg1C5_u reg1c5;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, &reg1c5.val);
	PHY_READ_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0xff;
	reg1c5.bitFields.swResetNReg = 0;
	reg80.bitFields.compress3AntEndAddr = 0x10;
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{	
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C5, 0, reg1c5.val);
	}
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80+UP_OFFSET, 0, reg80.val);
}

void PHY_POST_CAL_DFS(void)
{
	RegDfsPhyRxTdPhyRxtdReg062_u reg62;
	RegPhyRxFdPhyRxfdReg80_u reg80;

	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG062, 0, &reg62.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_FD_PHY_RXFD_REG80, 0, &reg80.val);
	reg62.bitFields.blockDetectionOnBands = 0;
	reg80.bitFields.compress3AntEndAddr = 0x3ef;
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG062, 0, reg62.val);
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80, 0, reg80.val);
	PHY_WRITE_REG(REG_PHY_RX_FD_PHY_RXFD_REG80+UP_OFFSET, 0, reg80.val);
}


void PHY_SET_VHT_NONAGGREGATE(uint8 enable)
{
	RegPhyRxBePhyRxbeReg67_u reg67;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_PHY_RXBE_REG67, 0, &reg67.val);
	reg67.bitFields.vhtNonAggregate = enable;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_PHY_RXBE_REG67, 0, reg67.val);
}

void PHY_SET_RX_HALFBAND(uint8 halfbandtype)
{
	RegPhyRxTdPhyRxtdReg05_u reg05;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG05, 0, &reg05.val);

	/*
	*  TODO BW Low Power Mode not implemented set 0 for now.
	*  LPI reg config look in 
	* https://maxlinear.sharepoint.com/sites/ger/wav700/Shared%20Documents/System/PHY/wave700_Specifications/PHY%20PM%20configuration%20for%20FW%20-%20Rel2.docx?d=wc1c2ccbc6e43461e8bcac1a356583f9d
	*/
	reg05.bitFields.lowPowerMode = 0; 
	switch(halfbandtype)
	{
		case 4: //20MHz
			reg05.bitFields.rfBw = 0;
			reg05.bitFields.digitalBw = 2;
			reg05.bitFields.rxAdcFs = 2;
			break;
		case 3: //40MHz
			reg05.bitFields.rfBw = 1;
			reg05.bitFields.digitalBw = 2;
			reg05.bitFields.rxAdcFs = 2;
			break;
		case 2: //80MHz
			reg05.bitFields.rfBw = 2;
			reg05.bitFields.digitalBw = 2;
			reg05.bitFields.rxAdcFs = 2;
			break;
		case 1: //160MHz
			reg05.bitFields.rfBw = 3;
			reg05.bitFields.digitalBw = 3;
			reg05.bitFields.rxAdcFs = 3;
			break;
		case 0: //320MHz
			reg05.bitFields.rfBw = 4;
			reg05.bitFields.digitalBw = 4;
			reg05.bitFields.rxAdcFs = 4;
			break;			
	}
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG05, 0, reg05.val);
}


void PHY_SET_RX_PRIMARY(uint8 rxprimarytype, uint8 antMask, uint8 spectrumMode)
{
	RegPhyRxTdPhyRxtdReg07D_u reg7d;
	RegPhyTxbIfPhyTxbRiscReg1C1_u reg1C1;
	RegPhyRxtdAnt0PhyRxtdAntReg53_u reg53[MAX_NUM_OF_RX_ANTENNAS];

	// OFDM
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG07D, 0, &reg7d.val);
	reg7d.bitFields.lsbIsCtrlChannel    = ~(rxprimarytype & 0x1);
	reg7d.bitFields.lsb40IsCtrlChannel  = ~((rxprimarytype & 0x2) >> 1);
	reg7d.bitFields.lsb80IsCtrlChannel  = ~((rxprimarytype & 0x4) >> 2);

	if(spectrumMode == BANDWIDTH_THREE_HUNDRED_TWENTY)
	{
		reg7d.bitFields.lsb160IsCtrlChannel = ~((rxprimarytype & 0x8) >> 3);
	}
	else
	{
		reg7d.bitFields.lsb160IsCtrlChannel = 0;
	}

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG07D, 0, reg7d.val);

	// 11B
	READ_ARR_FROM_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG53,(uint32*)reg53,antMask); //read from band antennas 
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, &reg1C1.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{	
		reg53[ANTENNA_0].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1); //write to all array (some of the antenna are not relervant to this band
		reg53[ANTENNA_1].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
		reg53[ANTENNA_2].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band
		reg53[ANTENNA_3].bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);//write to all array (some of the antenna are not relervant to this band

		WRITE_ARR_2_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG53,(uint32*)reg53,antMask);//write from band antennas


		reg1C1.bitFields.rxFreqShiftMode = (rxprimarytype & 0x1);
		PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, reg1C1.val);
	}

}

void PHY_SET_TX_HALFBAND(uint8 halfbandtype)
{
	RegPhyTxTxBeReg07_u reg07;
	RegPhyTxbIfPhyTxbRiscReg1C1_u reg1C1;
	RegPhyTxTxDigitalBw_u regtxDigitalBw;

	/*
	*  TODO BW Low Power Mode not implemented set 0 for now.
	*  LPI reg config look in 
	* https://maxlinear.sharepoint.com/sites/ger/wav700/Shared%20Documents/System/PHY/wave700_Specifications/PHY%20PM%20configuration%20for%20FW%20-%20Rel2.docx?d=wc1c2ccbc6e43461e8bcac1a356583f9d
	*/
	RegPhyTxEnableEhtFillers_u regTxEnableEhtFillers;
	RegPhyTxtdAnt0IdleModeEn_u regTxTdAnt0;
	RegPhyTxtdAnt1IdleModeEn_u regTxTdAnt1;
	RegPhyTxtdAnt2IdleModeEn_u regTxTdAnt2;
	RegPhyTxtdAnt3IdleModeEn_u regTxTdAnt3;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_DIGITAL_BW, 0, &regtxDigitalBw.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT0_IDLE_MODE_EN, 0, &regTxTdAnt0.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT1_IDLE_MODE_EN, 0, &regTxTdAnt1.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT2_IDLE_MODE_EN, 0, &regTxTdAnt2.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT3_IDLE_MODE_EN, 0, &regTxTdAnt3.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_ENABLE_EHT_FILLERS, 0, &regTxEnableEhtFillers.val);	
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_07, 0, &reg07.val);
	PHY_READ_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, &reg1C1.val);

	switch (halfbandtype)
	{
		case 4: //20MHz
			reg07.bitFields.txIsHalfBand20160 = 1;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			regtxDigitalBw.bitFields.txDigitalBw = 2;
			break;
		case 3: //40MHz
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 1;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 1;
			regtxDigitalBw.bitFields.txDigitalBw = 2;
			break;
		case 2: //80MHz
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 1;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			regtxDigitalBw.bitFields.txDigitalBw = 2;
			break;
		case 1: //160MHz
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			regtxDigitalBw.bitFields.txDigitalBw = 3;
			break;			
		default: //320MHz
			reg07.bitFields.txIsHalfBand20160 = 0;
			reg07.bitFields.txIsHalfBand40160 = 0;
			reg07.bitFields.txIsHalfBand80160 = 0;
			reg1C1.bitFields.rxFreqShiftEn = 0;
			regtxDigitalBw.bitFields.txDigitalBw = 4;
			break;
	}

	regTxTdAnt0.bitFields.pmTxIdleMode = 0;
	regTxTdAnt1.bitFields.pmTxIdleMode = 0;
	regTxTdAnt2.bitFields.pmTxIdleMode = 0;
	regTxTdAnt3.bitFields.pmTxIdleMode = 0;
	regTxEnableEhtFillers.bitFields.enableEhtFillers = 0;
	regTxEnableEhtFillers.bitFields.enableEhtFillersSmallRu = 0;

	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT0_IDLE_MODE_EN, 0, regTxTdAnt0.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT1_IDLE_MODE_EN, 0, regTxTdAnt1.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT2_IDLE_MODE_EN, 0, regTxTdAnt2.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT3_IDLE_MODE_EN, 0, regTxTdAnt3.val);	
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_ENABLE_EHT_FILLERS, 0, regTxEnableEhtFillers.val);		
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_DIGITAL_BW, 0, regtxDigitalBw.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_07, 0, reg07.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{
		reg1C1.bitFields.rxFreqShiftEn = 0;	
	}

	PHY_WRITE_REG(REG_PHY_TXB_IF_PHY_TXB_RISC_REG1C1, 0, reg1C1.val);

	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_ENABLE_EHT_FILLERS, 0, &regTxEnableEhtFillers.val);					
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT0_IDLE_MODE_EN, 0, &regTxTdAnt0.val);
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT1_IDLE_MODE_EN, 0, &regTxTdAnt1.val);
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT2_IDLE_MODE_EN, 0, &regTxTdAnt2.val);
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT3_IDLE_MODE_EN, 0, &regTxTdAnt3.val);

		regTxEnableEhtFillers.bitFields.enableEhtFillers = 0;
		regTxEnableEhtFillers.bitFields.enableEhtFillersSmallRu = 0;
		regTxTdAnt0.bitFields.pmTxIdleMode = 0;
		regTxTdAnt1.bitFields.pmTxIdleMode = 0;
		regTxTdAnt2.bitFields.pmTxIdleMode = 0;
		regTxTdAnt3.bitFields.pmTxIdleMode = 0;

		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_ENABLE_EHT_FILLERS, 0, regTxEnableEhtFillers.val);		
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT0_IDLE_MODE_EN, 0, regTxTdAnt0.val);
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT1_IDLE_MODE_EN, 0, regTxTdAnt1.val);
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT2_IDLE_MODE_EN, 0, regTxTdAnt2.val);
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TXTD_ANT3_IDLE_MODE_EN, 0, regTxTdAnt3.val);
	}
}

void PHY_SET_TX_PRIMARY(uint8 txprimarytype, uint8 halfbandtype, uint8 antMask)
{
	RegPhyTxtdAnt0TxtdMix11BFreqOffset_u Mix11BFreqOffsetReg[MAX_NUM_OF_TX_ANTENNAS];
	RegPhyTxTxBeReg48_u reg48;
	uint8 mix11bFreqOffset = 0;

	// OFDM
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_48, 0, &reg48.val);
	reg48.bitFields.txSsb20  = (txprimarytype & 0x1);
	reg48.bitFields.txSsb40  = (txprimarytype & 0x2) >> 1;
	reg48.bitFields.txSsb80  = (txprimarytype & 0x4) >> 2;

	if(halfbandtype == 0) //320MHz
	{
		reg48.bitFields.txSsb160 = (txprimarytype & 0x8) >> 3;
	}
	else
	{
		reg48.bitFields.txSsb160 = 0;
	}
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_48, 0, reg48.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		RegUpPhyTxTxBeReg48_u upReg48;
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_48, 0, &upReg48.val);
		upReg48.bitFields.txSsb20  = reg48.bitFields.txSsb20;
		upReg48.bitFields.txSsb40  = reg48.bitFields.txSsb40;
		upReg48.bitFields.txSsb80  = reg48.bitFields.txSsb80;
		upReg48.bitFields.txSsb160 = reg48.bitFields.txSsb160;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_48, 0, upReg48.val);
	}

	// 11B
	READ_ARR_FROM_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT0_TXTD_MIX11B_FREQ_OFFSET,(uint32*)Mix11BFreqOffsetReg,antMask); //read from band antennas
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{	
		if (halfbandtype == 3) // 40 MHz
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
		Mix11BFreqOffsetReg[ANTENNA_0].bitFields.pmMix11BFreqOffset = mix11bFreqOffset; //write to all array (some of the antenna are not relervant to this band
		Mix11BFreqOffsetReg[ANTENNA_1].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
		Mix11BFreqOffsetReg[ANTENNA_2].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
		Mix11BFreqOffsetReg[ANTENNA_3].bitFields.pmMix11BFreqOffset = mix11bFreqOffset;//write to all array (some of the antenna are not relervant to this band
		WRITE_ARR_2_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_TXTD_ANT0_TXTD_MIX11B_FREQ_OFFSET,(uint32*)Mix11BFreqOffsetReg,antMask);//write from band antennas
	}
}


void PHY_SET_11B_RX_ANT_SELECTION(uint8 antselection)
{
	RegPhyTxbIfRasDetectionAntMask_u rasDetectionMaskReg;

	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{
		PHY_READ_REG(REG_PHY_TXB_IF_RAS_DETECTION_ANT_MASK, 0, &rasDetectionMaskReg.val);
		rasDetectionMaskReg.bitFields.rasDetectionAntMask = antselection;
		PHY_WRITE_REG(REG_PHY_TXB_IF_RAS_DETECTION_ANT_MASK, 0, rasDetectionMaskReg.val);
	}
	else
	{
		UNUSED_PARAM(antselection);
	}
}

void PHY_SET_11B_TX_ANT_SELECTION(uint8 antSelection)
{
	RegPhyTxTxBeReg58_u phyTxBeReg58;

	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_0)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_58, 0, &phyTxBeReg58.val);
		phyTxBeReg58.bitFields.modem11BForceTransmitAnt = TRUE;
		phyTxBeReg58.bitFields.modem11BForceTransmitAntMask = antSelection;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_58, 0, phyTxBeReg58.val);
	}
	else
	{
		UNUSED_PARAM(antSelection);
	}	

}


void PHY_CLEAR_PHY_INTERRUPTS(void)
{
	RegPhyRxTdIfPhyRxtdIf08_u reg08;
	RegPhyRxTdIfPhyRxtdIf85_u reg85;
	
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF08, 0, &reg08.val);
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF85, 0, &reg85.val);
	reg08.bitFields.semaphoreReg08 = 0;
	reg85.bitFields.clearWdInt = 1;
	reg85.bitFields.clearTxPacketInt = 1;
	reg85.bitFields.clearPhyMacInt = 1;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF08, 0, reg08.val);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_IF_PHY_RXTD_IF85, 0, reg85.val);
}

void PHY_SET_SPACELESS(void)
{
	RegPhyTxTxBeReg11_u reg11;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_11, 0, &reg11.val);
	reg11.bitFields.txEndlessMode = 1;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_11, 0, reg11.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_11, 0, &reg11.val);
		reg11.bitFields.txEndlessMode = 1;
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_11, 0, reg11.val);
	}
}

void PHY_READ_RSSI(uint8 * rssiValues,uint8 * energyEstNumOfSamples)
{
	RegPhyRxBeIfPhyRxbeIf184_u regVal;
	//uint32 value;
	uint8  ant;
	// TODO review for W700
	for (ant = 0; ant < 5; ant++)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IF_PHY_RXBE_IF184, ant, &regVal.val);
		rssiValues[ant] = regVal.bitFields.ant0AdRssi;
	}
	//PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG13, 0, &value);
	*energyEstNumOfSamples = 1;
}

void PHY_SET_TX_ANT_OPERATION_SET(uint8 opSet)
{
	RegPhyTxTxBeReg07_u	txBeReg07;
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_07,0,&txBeReg07.val);
	txBeReg07.bitFields.tx0FeEnable = ((opSet & 0x1) >> 0);
	txBeReg07.bitFields.tx1FeEnable = ((opSet & 0x2) >> 1);
	txBeReg07.bitFields.tx2FeEnable = ((opSet & 0x4) >> 2);
	txBeReg07.bitFields.tx3FeEnable = ((opSet & 0x8) >> 3);
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_07,0,txBeReg07.val);
	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_07, 0, &txBeReg07.val);
		txBeReg07.bitFields.tx0FeEnable = ((opSet & 0x1) >> 0);
		txBeReg07.bitFields.tx1FeEnable = ((opSet & 0x2) >> 1);
		txBeReg07.bitFields.tx2FeEnable = ((opSet & 0x4) >> 2);
		txBeReg07.bitFields.tx3FeEnable = ((opSet & 0x8) >> 3);
		PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_UP_PHY_TX_TX_BE_REG_07, 0, txBeReg07.val);
	}
}

void PHY_SET_RX_ANT_OPERATION_SET_DFS(uint8 opSet)
{
	RegPhyRxTdPhyRxtdReg07B_u rxTdAntReg7B;
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B,0,&rxTdAntReg7B.val);
	rxTdAntReg7B.bitFields.hostAntennaEn = opSet;
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B,0,rxTdAntReg7B.val);
}

void PHY_SET_RX_ANT_OPERATION_SET(uint8 opSet)
{
	RegPhyRxTdPhyRxtdReg07B_u rxTdAntReg7B;
	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG07B,0,&rxTdAntReg7B.val);
	rxTdAntReg7B.bitFields.hostAntennaEn = opSet;
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_TD_PHY_RXTD_REG07B,0,rxTdAntReg7B.val);
}
void PHY_SET_FREQUENCY(uint32 freq, uint8 antMask)
{
	RegPhyRxtdAnt0PhyRxtdAntReg50_u rxTdReg50[MAX_NUM_OF_RX_ANTENNAS];

	READ_ARR_FROM_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);
	rxTdReg50[ANTENNA_0].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_1].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_2].bitFields.rfFreqMhz = freq;
	rxTdReg50[ANTENNA_3].bitFields.rfFreqMhz = freq;
	WRITE_ARR_2_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);

	if(ConfigurationManager_GetMyBand() == CONFIGURATION_MANAGER_BAND_2)
	{
		READ_ARR_FROM_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_UP_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);
			rxTdReg50[ANTENNA_0].bitFields.rfFreqMhz = freq;
			rxTdReg50[ANTENNA_1].bitFields.rfFreqMhz = freq;
			rxTdReg50[ANTENNA_2].bitFields.rfFreqMhz = freq;
			rxTdReg50[ANTENNA_3].bitFields.rfFreqMhz = freq;
		WRITE_ARR_2_MASK_ANT_REGS(PHY_REG_BAND_OFFSET+REG_UP_PHY_RXTD_ANT0_PHY_RXTD_ANT_REG50,(uint32*)rxTdReg50,antMask);
	}
}

// Not used
void PHY_SET_RX_DFS(uint8 isRadarEnabled, uint32 freq, uint8 u8SpectrumMode)
{
	RegDfsPhyRxtdAntPhyRxtdAntReg50_u    RxPhyDfsAntReg50;
	RegDfsPhyRxTdPhyRxtdReg05_u          RxPhyDfsReg05;
	RegDfsPhyRxTdPhyRxtdReg07B_u         RxPhyDfsReg07B;
	RegDfsPhyRxTdPhyRxtdReg0Ba_u         RxPhyDfsReg0BA;
	RegDfsPhyRxTdPhyRxtdReg0C7_u         RxPhyDfsReg0C7;

	PHY_READ_REG(REG_DFS_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50,0,&RxPhyDfsAntReg50.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG05,0,&RxPhyDfsReg05.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B,0,&RxPhyDfsReg07B.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA,0,&RxPhyDfsReg0BA.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0C7,0,&RxPhyDfsReg0C7.val);

	if (isRadarEnabled != 0)
	{

		RxPhyDfsAntReg50.bitFields.rfFreqMhz = freq;
		RxPhyDfsReg05.bitFields.rfBw = u8SpectrumMode; 		// default is 160MHz (3)
		RxPhyDfsReg05.bitFields.digitalBw = u8SpectrumMode; // default is 160MHz (3)
		RxPhyDfsReg05.bitFields.rxAdcFs = u8SpectrumMode; 	// default is 160MHz (3)
		RxPhyDfsReg05.bitFields.lowPowerMode = 0;
		RxPhyDfsReg07B.bitFields.hostAntennaEn = 1;
		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 1;			// general new radar HW enable bit
		// Use the isRadarEnabled bitfield to block not needed sub-channel events
		 /// @todo WAV700_ZWDFS positive or negative mask? Seems a 1 would disable. (default is 0).
//		RxPhyDfsReg0BA.bitFields.pmBlockDetectionMask = (isRadarEnabled ^ 0xFF);

		RxPhyDfsReg0C7.bitFields.pmEventsRamMode = 1;  		// Set RAM mode to 1 (128 words for all subbands)
//		RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0xF; 	// enable all types for report
		RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0x8; 	// enable only type radar

		PHY_WRITE_REG(REG_DFS_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50, 0, RxPhyDfsAntReg50.val);
		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG05,0, RxPhyDfsReg05.val);
		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B, 0, RxPhyDfsReg07B.val);
		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0C7, 0, RxPhyDfsReg0C7.val);
		// Write enable bit as last one
		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);

		gDbgDfsWritten = 1;
	}
	else
	{
		UNUSED_PARAM(freq);
		UNUSED_PARAM(u8SpectrumMode);
		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 0;			// general new radar HW enable bit
		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);
	}
}

// WAV700_ZWDFS
void PhyConfigureAndDisableDfsHw(uint32 freq, uint8 u8SpectrumMode)
{
	RegDfsPhyRxtdAntPhyRxtdAntReg50_u    RxPhyDfsAntReg50;
	RegDfsPhyRxTdPhyRxtdReg05_u          RxPhyDfsReg05;
	RegDfsPhyRxTdPhyRxtdReg07B_u         RxPhyDfsReg07B;
	RegDfsPhyRxTdPhyRxtdReg0Ba_u         RxPhyDfsReg0BA;
	RegDfsPhyRxTdPhyRxtdReg0C7_u         RxPhyDfsReg0C7;

	PHY_READ_REG(REG_DFS_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50,0,&RxPhyDfsAntReg50.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG05,0,&RxPhyDfsReg05.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B,0,&RxPhyDfsReg07B.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA,0,&RxPhyDfsReg0BA.val);
	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0C7,0,&RxPhyDfsReg0C7.val);

	RxPhyDfsAntReg50.bitFields.rfFreqMhz = freq;
	RxPhyDfsReg05.bitFields.rfBw = u8SpectrumMode; 		// default is 160MHz (3)
	RxPhyDfsReg05.bitFields.digitalBw = u8SpectrumMode; // default is 160MHz (3)
	RxPhyDfsReg05.bitFields.rxAdcFs = u8SpectrumMode; 	// default is 160MHz (3)
	RxPhyDfsReg05.bitFields.lowPowerMode = 0;
	RxPhyDfsReg07B.bitFields.hostAntennaEn = 1;
	RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 0;			// disable the radar HW
	// Use the isRadarEnabled bitfield to block not needed sub-channel events
		/// @todo WAV700_ZWDFS positive or negative mask? Seems a 1 would disable. (default is 0).
//		RxPhyDfsReg0BA.bitFields.pmBlockDetectionMask = (isRadarEnabled ^ 0xFF);

	RxPhyDfsReg0C7.bitFields.pmEventsRamMode = 1;  		// Set RAM mode to 1 (128 words for all subbands)
//		RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0xF; 	// enable all types for report
	RxPhyDfsReg0C7.bitFields.pmEventsRamVerbosity = 0x8; 	// enable only type radar

	PHY_WRITE_REG(REG_DFS_PHY_RXTD_ANT_PHY_RXTD_ANT_REG50, 0, RxPhyDfsAntReg50.val);
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG05,0, RxPhyDfsReg05.val);
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG07B, 0, RxPhyDfsReg07B.val);
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0C7, 0, RxPhyDfsReg0C7.val);
	// Write enable bit as last one
	PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);

	gDbgDfsWritten = 2;

}

// WAV700_ZWDFS
void PhyEnableDfsHw (uint8 isRadarEnabled)
{
	RegDfsPhyRxTdPhyRxtdReg0Ba_u         RxPhyDfsReg0BA;

	PHY_READ_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA,0,&RxPhyDfsReg0BA.val);

	if (isRadarEnabled != 0)
	{

		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 1;			// general new radar HW enable bit

		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);

		gDbgDfsWritten = 3;
	}
	else
	{
		RxPhyDfsReg0BA.bitFields.pmDfsAntMode = 0;			// general new radar HW enable bit

		PHY_WRITE_REG(REG_DFS_PHY_RX_TD_PHY_RXTD_REG0BA, 0, RxPhyDfsReg0BA.val);
	}
}

void PHY_SET_BAND(uint8 band)
{
	PHY_WRITE_REG(ENABLE_802_11_H,0, band == BAND_5_2_GHZ); //set ENABLE_802_11_H to 1 if we are in 5 Giga
	PHY_WRITE_REG(ENABLE_11_B, 0, band == BAND_2_4_GHZ); 	//set ENABLE_11_B to 1 if we are in 2.4 Giga
}


void PHY_SET_RX_320_CHANNEL(uint8 channel320Mode)
{
	RegPhyRxBeIs320UpperChannel_u regRxBeIs320;	

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IS_320_UPPER_CHANNEL, 0, &regRxBeIs320.val);
	regRxBeIs320.bitFields.is320UpperChannel = channel320Mode; //0 for 320_1, 1 for 320_2
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_RX_BE_IS_320_UPPER_CHANNEL, 0, regRxBeIs320.val);	


}
void PHY_SET_TX_320_CHANNEL(uint8 channel320Mode)
{
	RegPhyTxTxBeReg48_u reg48;

	PHY_READ_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_48, 0, &reg48.val);
	reg48.bitFields.is320UpperChannel = channel320Mode; //0 for 320_1, 1 for 320_2
	PHY_WRITE_REG(PHY_REG_BAND_OFFSET+REG_PHY_TX_TX_BE_REG_48, 0, reg48.val);


}


void PHY_SET_BSS_COLOR(uint8 vapId, uint8 bssColor)
{
	RegPhyRxBePhyRxbeReg78_u rxBeReg78;
	uint32 regAddr;
	uint8 offsetInReg;	
	regAddr = PHY_REG_BAND_OFFSET + REG_PHY_RX_BE_PHY_RXBE_REG78 + ( (vapId >> 2) << 2);
	
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


void PHY_DISABLE_BSS_COLOR(void)
{
	RegPhyRxBeRxbeMaskUnsupportedMcs_u rxBeMaskUnsupportedMcs;

	//for W700 value 0x80 disable the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x80;
	PHY_WRITE_REG(REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS,0,rxBeMaskUnsupportedMcs.val);

}

void PHY_ENABLE_BSS_COLOR(void)
{
	RegPhyRxBeRxbeMaskUnsupportedMcs_u rxBeMaskUnsupportedMcs;
	
	//the value 0 enables the phy filter by bss color for su
	rxBeMaskUnsupportedMcs.bitFields.rxbeMaskUnsupportedMcs = 0x0;
	PHY_WRITE_REG(REG_PHY_RX_BE_RXBE_MASK_UNSUPPORTED_MCS,0,rxBeMaskUnsupportedMcs.val);

}


