
/***********************************************************************************
File:				B2MacRabRegs.h
Module:				b2MacRab
SOC Revision:		latest
Generated at:       2024-06-26 12:54:50
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_MAC_RAB_REGS_H_
#define _B2_MAC_RAB_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_MAC_RAB_BASE_ADDRESS                          MEMORY_MAP_UNIT_20018_BASE_ADDRESS
#define	REG_B2_MAC_RAB_REVISION                           (B2_MAC_RAB_BASE_ADDRESS + 0x40)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE      (B2_MAC_RAB_BASE_ADDRESS + 0x50)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE     (B2_MAC_RAB_BASE_ADDRESS + 0x54)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT       (B2_MAC_RAB_BASE_ADDRESS + 0x60)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT      (B2_MAC_RAB_BASE_ADDRESS + 0x64)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD           (B2_MAC_RAB_BASE_ADDRESS + 0x70)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD          (B2_MAC_RAB_BASE_ADDRESS + 0x74)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2     (B2_MAC_RAB_BASE_ADDRESS + 0x80)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2    (B2_MAC_RAB_BASE_ADDRESS + 0x84)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2      (B2_MAC_RAB_BASE_ADDRESS + 0x90)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2     (B2_MAC_RAB_BASE_ADDRESS + 0x94)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2          (B2_MAC_RAB_BASE_ADDRESS + 0xA0)
#define	REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2         (B2_MAC_RAB_BASE_ADDRESS + 0xA4)
#define	REG_B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR          (B2_MAC_RAB_BASE_ADDRESS + 0xD8)
#define	REG_B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG          (B2_MAC_RAB_BASE_ADDRESS + 0xDC)
#define	REG_B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG1              (B2_MAC_RAB_BASE_ADDRESS + 0xE0)
#define	REG_B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG2              (B2_MAC_RAB_BASE_ADDRESS + 0xE4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_MAC_RAB_REVISION 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 revision : 12; //RAB  revision, reset value: 0x230, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB2MacRabRevision_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseTxPdAcc : 1; //TX_PD Acc SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxHc : 1; //TX_HC SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTim : 1; //TIM SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDel : 1; //Delia SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxc : 1; //RxC SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBeaconTimers : 1; //Beacon Timers SW registers enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDur : 1; //DUR SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseSecurity : 1; //Security SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePreAgg : 1; //Pre AGG SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseAddr2Index : 1; //Addr2index SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxHandler : 1; //Tx handler SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxSelector : 1; //Tx Selector SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxhTxc : 1; //TXH TXC SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxhNtd : 1; //TXH NTD SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBaAnalizer : 1; //ba_analizer  SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseOtfa : 1; //OTFA SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxClas : 1; //Rx Classifier SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePsSetting : 1; //PS Setting SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTfGenerator : 1; //TF Generator enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxSequencer : 1; //Tx Sequencer SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePacExtrap : 1; //PAC extrapolator SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxMsduParser : 1; //Rx MSDU Parser SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxCoordinator : 1; //Rx Coordinator SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseMbfm : 1; //MAC BFM SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseUlPostRx : 1; //UL Post Rx enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBsrc : 1; //BSRC enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseAutoResp : 1; //Auto Resp SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseGenriscTx : 1; //Genrisc TX SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseGenriscRx : 1; //Genrisc Rx SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxDmaAligner : 1; //Tx DMA aligner SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRta : 1; //RTA SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBestru : 1; //MAC BESTRU SW enable  Release, by write '1', reset value: 0x0, access type: WO
	} bitFields;
} RegB2MacRabPasSwResetSyncRegRelease_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseTxPdAccReg : 1; //TX_PD Acc registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxHcReg : 1; //TX_HC registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTimReg : 1; //TIM Reg block SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDelReg : 1; //Deliea Reg block SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxcReg : 1; //RxC Reg block SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBeaconTimersReg : 1; //Beacon reg block Timers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseDurReg : 1; //DUR Reg block SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseSecurityReg : 1; //Security reg block SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePreAggReg : 1; //Pre AGG registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseAddr2IndexReg : 1; //Addr2index registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxHandlerReg : 1; //Tx handler registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxSelectorReg : 1; //Tx Selector registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxhTxcReg : 1; //TXH TXC registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxhNtdReg : 1; //TXH NTD registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBaAnalizerReg : 1; //ba_analizer registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseOtfaReg : 1; //OTFA register file SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxClasReg : 1; //Rx Classifier registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePsSettingReg : 1; //PS Setting registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTfGeneratorReg : 1; //TF Generator registers enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxSequencerReg : 1; //Tx Sequencer registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleasePacExtrapReg : 1; //PAC extrapolator registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxMsduParserReg : 1; //Rx MSDU Parser Registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRxCoordinatorReg : 1; //Rx Coordinator registers SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseMbfmReg : 1; //MAC BFM SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseUlPostRxReg : 1; //UL Post RX registers enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBsrcReg : 1; //BSRC registers enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseAutoRespReg : 1; //Auto Resp registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseGenriscTxReserved : 1; //Reserved for Genrisc TX SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseGenriscRxReserved : 1; //Reserved for Genrisc Rx SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTxDmaAlignerReserved : 1; //Reserved for Tx DMA aligner SW enable  Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseRtaReserved : 1; //Reserved for RTA SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseBestruReg : 1; //MAC BESTRU SW enable  Release, by write '1', reset value: 0x0, access type: WO
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegRelease_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertTxPdAcc : 1; //TX_PD Acc SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxHc : 1; //TX_HC SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTim : 1; //TIM SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDel : 1; //Delia SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxc : 1; //RxC SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBeaconTimers : 1; //Beacon Timers SW registers enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDur : 1; //DUR SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertSecurity : 1; //Security SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPreAgg : 1; //Pre AGG SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertAddr2Index : 1; //Addr2index SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxHandler : 1; //Tx handler SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxSelector : 1; //Tx Selector SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxhTxc : 1; //TXH TXC SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxhNtd : 1; //TXH NTD SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBaAnalizer : 1; //ba_analizer  SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertOtfa : 1; //OTFA SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxClas : 1; //Rx Classifier SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPsSetting : 1; //PS Setting SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTfGenerator : 1; //TF Generator enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxSequencer : 1; //Tx Sequencer SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPacExtrap : 1; //PAC extrapolator SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxMsduParser : 1; //Rx MSDU Parser SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxCoordinator : 1; //Rx Coordinator SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertMbfm : 1; //MAC BFM SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertUlPostRx : 1; //UL Post Rx enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBsrc : 1; //BSRC enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertAutoResp : 1; //Auto Resp SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertGenriscTx : 1; //Genrisc TX SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertGenriscRx : 1; //Genrisc Rx SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxDmaAligner : 1; //Tx DMA aligner SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRta : 1; //RTA SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBestru : 1; //MAC BESTRU SW enable  assert, by write '1', reset value: 0x0, access type: WO
	} bitFields;
} RegB2MacRabPasSwResetSyncRegAssert_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertTxPdAccReg : 1; //TX_PD Acc registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxHcReg : 1; //TX_HC registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTimReg : 1; //TIM Reg block SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDelReg : 1; //Deliea Reg block SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxcReg : 1; //RxC Reg block SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBeaconTimersReg : 1; //Beacon reg block Timers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertDurReg : 1; //DUR Reg block SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertSecurityReg : 1; //Security reg block SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPreAggReg : 1; //Pre AGG registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertAddr2IndexReg : 1; //Addr2index registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxHandlerReg : 1; //Tx handler registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxSelectorReg : 1; //Tx Selector registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxhTxcReg : 1; //TXH TXC registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxhNtdReg : 1; //TXH NTD registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBaAnalizerReg : 1; //ba_analizer registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertOtfaReg : 1; //OTFA register file SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxClasReg : 1; //Rx Classifier registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPsSettingReg : 1; //PS Setting registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTfGeneratorReg : 1; //TF Generator registers enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxSequencerReg : 1; //Tx Sequencer registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertPacExtrapReg : 1; //PAC extrapolator registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxMsduParserReg : 1; //Rx MSDU Parser Registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRxCoordinatorReg : 1; //Rx Coordinator registers SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertMbfmReg : 1; //MAC BFM SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertUlPostRxReg : 1; //UL Post RX registers enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBsrcReg : 1; //BSRC registers enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertAutoRespReg : 1; //Auto Resp registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertGenriscTxReserved : 1; //Reserved for Genrisc TX SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertGenriscRxReserved : 1; //Reserved for Genrisc Rx SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTxDmaAlignerReserved : 1; //Reserved for Tx DMA aligner SW enable  assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertRtaReserved : 1; //Reserved for RTA SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertBestruReg : 1; //MAC BESTRU SW enable  assert, by write '1', reset value: 0x0, access type: WO
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegAssert_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdTxPdAcc : 1; //TX_PD Acc SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxHc : 1; //TX_HC SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTim : 1; //TIM SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDel : 1; //Delia SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxc : 1; //RxC SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBeaconTimers : 1; //Beacon Timers SW registers enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDur : 1; //DUR SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdSecurity : 1; //Security SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdPreAgg : 1; //Pre AGG SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdAddr2Index : 1; //Addr2index SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxHandler : 1; //Tx handler SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxSelector : 1; //Tx Selector SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxhTxc : 1; //TXH TXC SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxhNtd : 1; //TXH NTD SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBaAnalizer : 1; //ba_analizer  SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdOtfa : 1; //OTFA SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxClas : 1; //Rx Classifier SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdPsSetting : 1; //PS Setting SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdTfGenerator : 1; //TF Generator enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxSequencer : 1; //Tx Sequencer SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdPacExtrap : 1; //PAC extrapolator SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxMsduParser : 1; //Rx MSDU Parser SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxCoordinator : 1; //Rx Coordinator SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdMbfm : 1; //MAC BFM SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdUlPostRx : 1; //UL Post Rx enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBsrc : 1; //BSRC enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdAutoResp : 1; //Auto Resp SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdGenriscTx : 1; //Genrisc TX SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdGenriscRx : 1; //Genrisc Rx SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxDmaAligner : 1; //Tx DMA aligner SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRta : 1; //RTA SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBestru : 1; //MAC BESTRU SW enable  rd, reset value: 0x1, access type: RO
	} bitFields;
} RegB2MacRabPasSwResetSyncRegRd_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdTxPdAccReg : 1; //TX_PD Acc registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxHcReg : 1; //TX_HC registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTimReg : 1; //TIM Reg block SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDelReg : 1; //Deliea Reg block SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxcReg : 1; //RxC Reg block SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBeaconTimersReg : 1; //Beacon reg block Timers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdDurReg : 1; //DUR Reg block SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdSecurityReg : 1; //Security reg block SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdPreAggReg : 1; //Pre AGG registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdAddr2IndexReg : 1; //Addr2index registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxHandlerReg : 1; //Tx handler registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxSelectorReg : 1; //Tx Selector registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxhTxcReg : 1; //TXH TXC registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxhNtdReg : 1; //TXH NTD registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBaAnalizerReg : 1; //ba_analizer registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdOtfaReg : 1; //OTFA register file SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxClasReg : 1; //Rx Classifier registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdPsSettingReg : 1; //PS Setting registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdTfGeneratorReg : 1; //TF Generator registers enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxSequencerReg : 1; //Tx Sequencer registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdPacExtrapReg : 1; //PAC extrapolator registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxMsduParserReg : 1; //Rx MSDU Parser Registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRxCoordinatorReg : 1; //Rx Coordinator registers SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdMbfmReg : 1; //MAC BFM SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdUlPostRxReg : 1; //UL Post RX registers enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBsrcReg : 1; //BSRC registers enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdAutoRespReg : 1; //Auto Resp registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdGenriscTxReserved : 1; //Reserved for Genrisc TX SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdGenriscRxReserved : 1; //Reserved for Genrisc Rx SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdTxDmaAlignerReserved : 1; //Reserved for Tx DMA aligner SW enable  rd, reset value: 0x1, access type: RO
		uint32 swResetRdRtaReserved : 1; //Reserved for RTA SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdBestruReg : 1; //MAC BESTRU SW enable  rd, reset value: 0x1, access type: RO
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegRd_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseMacAirTimeCalc : 1; //MAC Air Time Calculator SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTfDecoder : 1; //TF Decoder SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseMldDb : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetSyncRegRelease2_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetReleaseMacAirTimeCalcReserved : 1; //Reserved for MAC Air Time Calculator SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseTfDecoderReg : 1; //TF Decoder registers SW enable Release, by write '1', reset value: 0x0, access type: WO
		uint32 swResetReleaseMldDbReg : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegRelease2_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertMacAirTimeCalc : 1; //MAC Air Time Calculator SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTfDecoder : 1; //TF Decoder SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertMldDb : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetSyncRegAssert2_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetAssertMacAirTimeCalcReserved : 1; //Reserved for MAC Air Time Calculator SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertTfDecoderReg : 1; //TF Decoder registers SW enable assert, by write '1', reset value: 0x0, access type: WO
		uint32 swResetAssertMldDbReg : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegAssert2_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdMacAirTimeCalc : 1; //MAC Air Time Calculator SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTfDecoder : 1; //TF Decoder SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdMldDb : 1; //no description, reset value: 0x1, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetSyncRegRd2_u;

/*REG_B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swResetRdMacAirTimeCalcReserved : 1; //Reserved for MAC Air Time Calculator SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdTfDecoderReg : 1; //TF Decoder registers SW enable rd, reset value: 0x1, access type: RO
		uint32 swResetRdMldDbReg : 1; //no description, reset value: 0x1, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabPasSwResetAsyncRegRd2_u;

/*REG_B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDmaAlignerFifoClear : 1; //Tx DMA aligner FIFO clear, reset value: 0x0, access type: WO
		uint32 txDmaAlignerFcClear : 1; //Write 1 for clearing the Flow Control vector. , The Flow Control value of all User IDs shall be set to 0., reset value: 0x0, access type: WO
		uint32 txDmaAlignerFifoMaxOccupancyClear : 1; //Write '1' for clearing tx_dma_aligner_fifo_occupancy_max field, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacRabTxDmaAlignerFifoClear_u;

/*REG_B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDmaAlignerFifoNumWords : 7; //Live value of Tx DMA aligner FIFO occupancy, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txDmaAlignerFifoEmpty : 1; //Tx DMA aligner FIFO empty, reset value: 0x1, access type: RO
		uint32 txDmaAlignerFifoFull : 1; //Tx DMA aligner FIFO full, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 txDmaAlignerFifoOccupancyMax : 7; //Max value of Tx DMA aligner FIFO occupancy. , Cleared by Writing '1' to tx_dma_aligner_fifo_max_occupancy_clear field within TX_DMA_ALIGNER_FIFO_CLEAR register, reset value: 0x0, access type: RO
		uint32 reserved2 : 13;
	} bitFields;
} RegB2MacRabTxDmaAlignerFifoDebug_u;

/*REG_B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG1 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 xbarToAlignerAccessCounter : 12; //xbar_to_aligner access counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 alignerToSmcAccessCounter : 12; //aligner_to_smc access counter, reset value: 0x0, access type: RO
		uint32 reserved1 : 4;
	} bitFields;
} RegB2MacRabTxDmaAlignerDebug1_u;

/*REG_B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG2 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 xbarToAlignerTotalTransactions : 12; //xbar_to_aligner total transactions, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 alignerToSmcTotalTransactions : 12; //aligner_to_smc total transactions, reset value: 0x0, access type: RO
		uint32 txDmaAlignerIdle : 1; //no description, reset value: 0x1, access type: RO
		uint32 reserved1 : 3;
	} bitFields;
} RegB2MacRabTxDmaAlignerDebug2_u;

//========================================
/* REG_MAC_RAB_REVISION 0x40 */
#define B2_MAC_RAB_REVISION_FIELD_REVISION_MASK                                                                   0x00000FFF
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE 0x50 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_PD_ACC_MASK                            0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_HC_MASK                                0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TIM_MASK                                  0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_DEL_MASK                                  0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RXC_MASK                                  0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BEACON_TIMERS_MASK                        0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_DUR_MASK                                  0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_SECURITY_MASK                             0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PRE_AGG_MASK                              0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_ADDR2INDEX_MASK                           0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_HANDLER_MASK                           0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_SELECTOR_MASK                          0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TXH_TXC_MASK                              0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TXH_NTD_MASK                              0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BA_ANALIZER_MASK                          0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_OTFA_MASK                                 0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_CLAS_MASK                              0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PS_SETTING_MASK                           0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TF_GENERATOR_MASK                         0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_SEQUENCER_MASK                         0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PAC_EXTRAP_MASK                           0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_MSDU_PARSER_MASK                       0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_COORDINATOR_MASK                       0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_MBFM_MASK                                 0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_UL_POST_RX_MASK                           0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BSRC_MASK                                 0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_AUTO_RESP_MASK                            0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_GENRISC_TX_MASK                           0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_GENRISC_RX_MASK                           0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_DMA_ALIGNER_MASK                       0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RTA_MASK                                  0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BESTRU_MASK                               0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE 0x54 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_PD_ACC_REG_MASK                       0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_HC_REG_MASK                           0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TIM_REG_MASK                             0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_DEL_REG_MASK                             0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RXC_REG_MASK                             0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BEACON_TIMERS_REG_MASK                   0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_DUR_REG_MASK                             0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_SECURITY_REG_MASK                        0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PRE_AGG_REG_MASK                         0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_ADDR2INDEX_REG_MASK                      0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_HANDLER_REG_MASK                      0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_SELECTOR_REG_MASK                     0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TXH_TXC_REG_MASK                         0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TXH_NTD_REG_MASK                         0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BA_ANALIZER_REG_MASK                     0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_OTFA_REG_MASK                            0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_CLAS_REG_MASK                         0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PS_SETTING_REG_MASK                      0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TF_GENERATOR_REG_MASK                    0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_SEQUENCER_REG_MASK                    0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_PAC_EXTRAP_REG_MASK                      0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_MSDU_PARSER_REG_MASK                  0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RX_COORDINATOR_REG_MASK                  0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_MBFM_REG_MASK                            0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_UL_POST_RX_REG_MASK                      0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BSRC_REG_MASK                            0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_AUTO_RESP_REG_MASK                       0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_GENRISC_TX_RESERVED_MASK                 0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_GENRISC_RX_RESERVED_MASK                 0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_TX_DMA_ALIGNER_RESERVED_MASK             0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_RTA_RESERVED_MASK                        0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE_FIELD_SW_RESET_RELEASE_BESTRU_REG_MASK                          0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT 0x60 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_PD_ACC_MASK                              0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_HC_MASK                                  0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TIM_MASK                                    0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_DEL_MASK                                    0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RXC_MASK                                    0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BEACON_TIMERS_MASK                          0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_DUR_MASK                                    0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_SECURITY_MASK                               0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PRE_AGG_MASK                                0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_ADDR2INDEX_MASK                             0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_HANDLER_MASK                             0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_SELECTOR_MASK                            0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TXH_TXC_MASK                                0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TXH_NTD_MASK                                0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BA_ANALIZER_MASK                            0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_OTFA_MASK                                   0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_CLAS_MASK                                0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PS_SETTING_MASK                             0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TF_GENERATOR_MASK                           0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_SEQUENCER_MASK                           0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PAC_EXTRAP_MASK                             0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_MSDU_PARSER_MASK                         0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_COORDINATOR_MASK                         0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_MBFM_MASK                                   0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_UL_POST_RX_MASK                             0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BSRC_MASK                                   0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_AUTO_RESP_MASK                              0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_GENRISC_TX_MASK                             0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_GENRISC_RX_MASK                             0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_DMA_ALIGNER_MASK                         0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RTA_MASK                                    0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BESTRU_MASK                                 0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT 0x64 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_PD_ACC_REG_MASK                         0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_HC_REG_MASK                             0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TIM_REG_MASK                               0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_DEL_REG_MASK                               0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RXC_REG_MASK                               0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BEACON_TIMERS_REG_MASK                     0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_DUR_REG_MASK                               0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_SECURITY_REG_MASK                          0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PRE_AGG_REG_MASK                           0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_ADDR2INDEX_REG_MASK                        0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_HANDLER_REG_MASK                        0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_SELECTOR_REG_MASK                       0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TXH_TXC_REG_MASK                           0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TXH_NTD_REG_MASK                           0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BA_ANALIZER_REG_MASK                       0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_OTFA_REG_MASK                              0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_CLAS_REG_MASK                           0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PS_SETTING_REG_MASK                        0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TF_GENERATOR_REG_MASK                      0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_SEQUENCER_REG_MASK                      0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_PAC_EXTRAP_REG_MASK                        0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_MSDU_PARSER_REG_MASK                    0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RX_COORDINATOR_REG_MASK                    0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_MBFM_REG_MASK                              0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_UL_POST_RX_REG_MASK                        0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BSRC_REG_MASK                              0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_AUTO_RESP_REG_MASK                         0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_GENRISC_TX_RESERVED_MASK                   0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_GENRISC_RX_RESERVED_MASK                   0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_TX_DMA_ALIGNER_RESERVED_MASK               0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_RTA_RESERVED_MASK                          0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT_FIELD_SW_RESET_ASSERT_BESTRU_REG_MASK                            0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD 0x70 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_PD_ACC_MASK                                      0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_HC_MASK                                          0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TIM_MASK                                            0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_DEL_MASK                                            0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_RXC_MASK                                            0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_BEACON_TIMERS_MASK                                  0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_DUR_MASK                                            0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_SECURITY_MASK                                       0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_PRE_AGG_MASK                                        0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_ADDR2INDEX_MASK                                     0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_HANDLER_MASK                                     0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_SELECTOR_MASK                                    0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TXH_TXC_MASK                                        0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TXH_NTD_MASK                                        0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_BA_ANALIZER_MASK                                    0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_OTFA_MASK                                           0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_RX_CLAS_MASK                                        0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_PS_SETTING_MASK                                     0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TF_GENERATOR_MASK                                   0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_SEQUENCER_MASK                                   0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_PAC_EXTRAP_MASK                                     0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_RX_MSDU_PARSER_MASK                                 0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_RX_COORDINATOR_MASK                                 0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_MBFM_MASK                                           0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_UL_POST_RX_MASK                                     0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_BSRC_MASK                                           0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_AUTO_RESP_MASK                                      0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_GENRISC_TX_MASK                                     0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_GENRISC_RX_MASK                                     0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_TX_DMA_ALIGNER_MASK                                 0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_RTA_MASK                                            0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD_FIELD_SW_RESET_RD_BESTRU_MASK                                         0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD 0x74 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_PD_ACC_REG_MASK                                 0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_HC_REG_MASK                                     0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TIM_REG_MASK                                       0x00000004
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_DEL_REG_MASK                                       0x00000008
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_RXC_REG_MASK                                       0x00000010
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_BEACON_TIMERS_REG_MASK                             0x00000020
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_DUR_REG_MASK                                       0x00000040
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_SECURITY_REG_MASK                                  0x00000080
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_PRE_AGG_REG_MASK                                   0x00000100
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_ADDR2INDEX_REG_MASK                                0x00000200
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_HANDLER_REG_MASK                                0x00000400
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_SELECTOR_REG_MASK                               0x00000800
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TXH_TXC_REG_MASK                                   0x00001000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TXH_NTD_REG_MASK                                   0x00002000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_BA_ANALIZER_REG_MASK                               0x00004000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_OTFA_REG_MASK                                      0x00008000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_RX_CLAS_REG_MASK                                   0x00010000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_PS_SETTING_REG_MASK                                0x00020000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TF_GENERATOR_REG_MASK                              0x00040000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_SEQUENCER_REG_MASK                              0x00080000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_PAC_EXTRAP_REG_MASK                                0x00100000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_RX_MSDU_PARSER_REG_MASK                            0x00200000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_RX_COORDINATOR_REG_MASK                            0x00400000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_MBFM_REG_MASK                                      0x00800000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_UL_POST_RX_REG_MASK                                0x01000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_BSRC_REG_MASK                                      0x02000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_AUTO_RESP_REG_MASK                                 0x04000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_GENRISC_TX_RESERVED_MASK                           0x08000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_GENRISC_RX_RESERVED_MASK                           0x10000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_TX_DMA_ALIGNER_RESERVED_MASK                       0x20000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_RTA_RESERVED_MASK                                  0x40000000
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD_FIELD_SW_RESET_RD_BESTRU_REG_MASK                                    0x80000000
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2 0x80 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_MAC_AIR_TIME_CALC_MASK                   0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_TF_DECODER_MASK                          0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_MLD_DB_MASK                              0x00000004
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2 0x84 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_MAC_AIR_TIME_CALC_RESERVED_MASK         0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_TF_DECODER_REG_MASK                     0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RELEASE2_FIELD_SW_RESET_RELEASE_MLD_DB_REG_MASK                         0x00000004
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2 0x90 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_MAC_AIR_TIME_CALC_MASK                     0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_TF_DECODER_MASK                            0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_MLD_DB_MASK                                0x00000004
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2 0x94 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_MAC_AIR_TIME_CALC_RESERVED_MASK           0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_TF_DECODER_REG_MASK                       0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_ASSERT2_FIELD_SW_RESET_ASSERT_MLD_DB_REG_MASK                           0x00000004
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2 0xA0 */
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2_FIELD_SW_RESET_RD_MAC_AIR_TIME_CALC_MASK                             0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2_FIELD_SW_RESET_RD_TF_DECODER_MASK                                    0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_SYNC_REG_RD2_FIELD_SW_RESET_RD_MLD_DB_MASK                                        0x00000004
//========================================
/* REG_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2 0xA4 */
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2_FIELD_SW_RESET_RD_MAC_AIR_TIME_CALC_RESERVED_MASK                   0x00000001
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2_FIELD_SW_RESET_RD_TF_DECODER_REG_MASK                               0x00000002
#define B2_MAC_RAB_PAS_SW_RESET_ASYNC_REG_RD2_FIELD_SW_RESET_RD_MLD_DB_REG_MASK                                   0x00000004
//========================================
/* REG_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR 0xD8 */
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR_FIELD_TX_DMA_ALIGNER_FIFO_CLEAR_MASK                                 0x00000001
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR_FIELD_TX_DMA_ALIGNER_FC_CLEAR_MASK                                   0x00000002
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_CLEAR_FIELD_TX_DMA_ALIGNER_FIFO_MAX_OCCUPANCY_CLEAR_MASK                   0x00000004
//========================================
/* REG_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG 0xDC */
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG_FIELD_TX_DMA_ALIGNER_FIFO_NUM_WORDS_MASK                             0x0000007F
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG_FIELD_TX_DMA_ALIGNER_FIFO_EMPTY_MASK                                 0x00000100
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG_FIELD_TX_DMA_ALIGNER_FIFO_FULL_MASK                                  0x00000200
#define B2_MAC_RAB_TX_DMA_ALIGNER_FIFO_DEBUG_FIELD_TX_DMA_ALIGNER_FIFO_OCCUPANCY_MAX_MASK                         0x0007F000
//========================================
/* REG_MAC_RAB_TX_DMA_ALIGNER_DEBUG1 0xE0 */
#define B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG1_FIELD_XBAR_TO_ALIGNER_ACCESS_COUNTER_MASK                                0x00000FFF
#define B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG1_FIELD_ALIGNER_TO_SMC_ACCESS_COUNTER_MASK                                 0x0FFF0000
//========================================
/* REG_MAC_RAB_TX_DMA_ALIGNER_DEBUG2 0xE4 */
#define B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG2_FIELD_XBAR_TO_ALIGNER_TOTAL_TRANSACTIONS_MASK                            0x00000FFF
#define B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG2_FIELD_ALIGNER_TO_SMC_TOTAL_TRANSACTIONS_MASK                             0x0FFF0000
#define B2_MAC_RAB_TX_DMA_ALIGNER_DEBUG2_FIELD_TX_DMA_ALIGNER_IDLE_MASK                                           0x10000000


#endif // _MAC_RAB_REGS_H_
