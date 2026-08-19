
/***************************************************************************************************/
/***************************************************************************************************/
/*   PHY library functions definition                                                              */

/***************************************************************************************************/
/***************************************************************************************************/

#ifndef _PHY_LIB_H_
#define _PHY_LIB_H_

#include "phy_access_definitions.h"

/* PHY address domain enum */
typedef enum PHY_Domain_e
{
	PHY_RXTD = 0,
	PHY_RXFD,
	PHY_RXBE,
	PHY_TX,
	PHY_DFS			// DFS genrisc
} PHY_Domain_e;



/* reset PHY block */
void PhyResetSet(PHY_Domain_e domain_to_reset);
/* release PHy block from reset */
void PhyResetRelease(PHY_Domain_e domain_to_reset);
/* digital loopback configuration */
void DigitalLpbk(void);
/* PHY HW reset */
void PHY_Reset(uint32 band, uint32 value);
/* disbale all PHY blocks */
void phy_disable(void);
/* release all PHY blocks from reset */
void phy_enable(void);
/* reset strobes */
void PhyResetStrobes(void);
/* start modem */
bool PhyStartModem(void);
/* Request GenRisc to halt prior to reset (intrupt6)*/
void PhyRiscSyncResetReq(bool *success);
/* stop PHY GenRisc */
void PhyStopRiscOperation(void);
/* start PHY GenRisc */
void PhyStartRiscOperation(void);
/* configure 11B modem */
void Phy_Config_802_11B(void);
#ifdef ENET_INC_ARCH_WAVE700
void PhyDfsRiscSyncResetReq(bool *success);
void phy_enable_dfs (void);
void phy_disable_dfs (void);
void PhyStartDfsRiscOperation (void);
void PhyStopDfsRiscOperation (void);
#endif

/******************************************************************
* 	PHY_BAND_INIT()
*
* 	configiration of mac_phy_band_config register
*
* 	reg field:
*   b0_phy_active	  	[0]  1b Band0 Phy active
*   b1_phy_active  		[1]  1b Band1 Phy active
*   fcsi_cen_reset_n   	[2]  1b Central FCSI reset_n
*   fcsi_cen_block_en 	[3]  1b Central FCSI block enable
*   phy_antenna0_sel  	[4]  1b Antenna0 assignment (0 = Band0 / 1 = Band1)
*   phy_antenna1_sel 	[5]  1b Antenna1 assignment (0 = Band0 / 1 = Band1)
*   phy_antenna2_sel  	[6]  1b Antenna2 assignment (0 = Band0 / 1 = Band1)
*   phy_antenna3_sel 	[7]  1b Antenna3 assignment (0 = Band0 / 1 = Band1)
*******************************************************************/
void PHY_MAC_BAND_CONFIG_REG_INIT(void);
void PHY_SET_MAC_BAND(uint8 band);
void PHY_RFIC_RESET(void);
void PHY_RFIC_RESET_RELEASE(void);
void PHY_ENABLE_RX(void);
void PHY_PRE_CAL(void);
void PHY_POST_CAL(void);
void PHY_SET_VHT_NONAGGREGATE(uint8 enable);
void PHY_SET_RX_HALFBAND(uint8 halfbandtype);
void PHY_SET_TX_HALFBAND(uint8 halfbandtype);
void PHY_SET_TX_PRIMARY(uint8 txprimarytype, uint8 halfbandtype, uint8 antMask);
void PHY_SET_11B_RX_ANT_SELECTION(uint8 antselection);
void PHY_SET_11B_TX_ANT_SELECTION(uint8 antSelection);
#ifndef ENET_INC_ARCH_WAVE600D2	
void PHY_SET_11B_ROUNDROBIN(uint8 rate);
#endif
void PHY_CLEAR_PHY_INTERRUPTS(void);
void PHY_SET_SPACELESS(void);
void PHY_READ_RSSI(uint8 *rssiValues, uint8 *energyEstNumOfSamples);
#ifdef ENET_INC_ARCH_WAVE700
void PHY_SET_RX_PRIMARY(uint8 rxprimarytype, uint8 antMask, uint8 spectrumMode);
void PHY_SET_RX_DFS(uint8 isRadarEnabled, uint32 freq, uint8 u8SpectrumMode);
void PhyConfigureAndDisableDfsHw(uint32 freq, uint8 u8SpectrumMode);
void PhyEnableDfsHw(uint8 isRadarEnabled);
void PHY_SET_RX_320_CHANNEL(uint8 Channel320Mode);
void PHY_SET_TX_320_CHANNEL(uint8 Channel320Mode);
void PHY_PRE_CAL_DFS(void);
void PHY_POST_CAL_DFS(void);
void PHY_ENABLE_RX_DFS(void);
void PHY_SET_RX_ANT_OPERATION_SET_DFS(uint8 opSet);
#else
void PHY_SET_RX_PRIMARY(uint8 rxprimarytype, uint8 antMask);
void PHY_SET_MAC_ANT_CONFIG(uint8 band, uint8 antMask);
#endif
void PHY_SET_TX_ANT_OPERATION_SET(uint8 opSet);
void PHY_SET_RX_ANT_OPERATION_SET(uint8 opSet);
void PHY_SET_FREQUENCY(uint32 freq, uint8 antMask);
void PHY_SET_BAND(uint8 band);
void PHY_SET_BSS_COLOR(uint8 vapId, uint8 bssColor);
void PHY_DISABLE_BSS_COLOR(void);
void PHY_ENABLE_BSS_COLOR(void);
void PHY_SECURE_WRITE(void);
#ifdef ENET_INC_ARCH_WAVE600D2B
void PHY_SET_RX_DFS_600D2B(uint8 isRadarEnabled, uint32 freq, uint8 u8SpectrumMode);
#endif
/************** Temp for v.27 ***********************/
void PHY_GCLK_BYPASS_RESET(uint8 reset);
#endif //_PHY_LIB_H_

