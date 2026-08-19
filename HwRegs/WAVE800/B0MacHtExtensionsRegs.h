
/***********************************************************************************
File:				B0MacHtExtensionsRegs.h
Module:				b0MacHtExtensions
SOC Revision:		latest
Generated at:       2024-06-26 12:54:25
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_MAC_HT_EXTENSIONS_REGS_H_
#define _B0_MAC_HT_EXTENSIONS_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_MAC_HT_EXTENSIONS_BASE_ADDRESS              MEMORY_MAP_UNIT_24_BASE_ADDRESS
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_BAND_ID                      (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x0)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL                 (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x4)
#define	REG_B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR                   (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x10)
#define	REG_B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS                (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x14)
#define	REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMER_PRESCALER            (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x20)
#define	REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_VAL                (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x24)
#define	REG_B0_MAC_HT_EXTENSIONS_GP_IF_SW_REQ                     (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x28)
#define	REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_TIME               (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x2C)
#define	REG_B0_MAC_HT_EXTENSIONS_GP_IF_RESULT                     (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x30)
#define	REG_B0_MAC_HT_EXTENSIONS_ACTIVITY_LOGGER_SW_EVENT         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x34)
#define	REG_B0_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x38)
#define	REG_B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL             (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x3C)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ           (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x40)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_MASK      (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x44)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x48)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x4C)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x50)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x54)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x58)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x5C)
#define	REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x60)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS1_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x6C)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS2_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x70)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS3_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x74)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS4_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x78)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS5_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x7C)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS6_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x80)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS7_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x84)
#define	REG_B0_MAC_HT_EXTENSIONS_RAMS8_RM                         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x90)
#define	REG_B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS            (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0x94)
#define	REG_B0_MAC_HT_EXTENSIONS_EVENT_CONTROL                    (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xA0)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xA4)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN           (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xA8)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2        (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xAC)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB          (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xB0)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2          (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xC0)
#define	REG_B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPO                 (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xC4)
#define	REG_B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPI                 (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xC8)
#define	REG_B0_MAC_HT_EXTENSIONS_PHY_MAC_IF_TEST_BUS_STOP         (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xF0)
#define	REG_B0_MAC_HT_EXTENSIONS_VAP_MODE_BITMAP                  (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xF4)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3        (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xF8)
#define	REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3          (B0_MAC_HT_EXTENSIONS_BASE_ADDRESS + 0xFC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_MAC_HT_EXTENSIONS_MAC_BAND_ID 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macBandId : 2; //Band ID Indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacHtExtensionsMacBandId_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staAidOffset65En : 1; //AID offset from STA index: , 0: AID equals to STA index + 1. , 1: AID equals to STA index + 65., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 aspmEnBand : 1; //enable ASPM indication towards PCIe core per band, reset value: 0x0, access type: RW
		uint32 aspmOnOverrideBand : 1; //override ASPM indication towards PCIe core per band, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 bandPrimaryChannel : 4; //set 1-15 the primary 20Mz channel inside up to 320MHz vector, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
		uint32 bandAllowedChannels : 16; //Configure the allowed channels for this band. , 20MHz granularity. , Starting from lowest frequency (as written in RCR/TCR puncturing map field), reset value: 0xffff, access type: RW
	} bitFields;
} RegB0MacHtExtensionsMacBandControl_u;

/*REG_B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 del2GenriscTxEndIrqClr : 1; //Delia to genrisc Tx end IRQ clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 reserved1 : 6;
		uint32 delAutoFillEndIrqClr : 1; //Delia auto fill IRQ clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 reserved2 : 2;
		uint32 pasPacTickTimerClr : 1; //PAS PAC Tick timer clear  (write '1' to clear), reset value: 0x0, access type: WO
		uint32 macPhyGpIfIrqClr : 1; //MAC_PHY GP IF IRQ clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 pasPac0TxErrorClr : 1; //PAS PAC 0 TX Error clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 reserved3 : 4;
		uint32 delLimitTimerExpiredIrqClr : 1; //del limit timer expired IRQ clear (write '1' to clear), reset value: 0x0, access type: WO
		uint32 reserved4 : 6;
	} bitFields;
} RegB0MacHtExtensionsWlMacIrqClr_u;

/*REG_B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 del2GenriscTxEndIrqStatus : 1; //Delia 0 to genrisc Tx end IRQ status, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 delAutoFillEndIrqStatus : 1; //Delia auto fill IRQ status, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 pasPacTickTimerStatus : 1; //PAS PAC Tick timer status, reset value: 0x0, access type: RO
		uint32 macPhyGpIfIrqStatus : 1; //MAC_PHY GP IF IRQ status, reset value: 0x0, access type: RO
		uint32 pasPacTxErrorStatus : 1; //PAS PAC 0 TX Error status, reset value: 0x0, access type: RO
		uint32 reserved3 : 4;
		uint32 delLimitTimerExpiredIrqStatus : 1; //del limit timer expired IRQ status, reset value: 0x0, access type: RO
		uint32 reserved4 : 6;
	} bitFields;
} RegB0MacHtExtensionsWlMacIrqStatus_u;

/*REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMER_PRESCALER 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpIfTimerPrescaler : 10; //MAC Phy General purpose I/F timer prescaler, reset value: 0x27f, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0MacHtExtensionsGpIfTimerPrescaler_u;

/*REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_VAL 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpIfTimeoutVal : 20; //MAC Phy General purpose interface timeout value; given in prescaler ticks, reset value: 0x1869f, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegB0MacHtExtensionsGpIfTimeoutVal_u;

/*REG_B0_MAC_HT_EXTENSIONS_GP_IF_SW_REQ 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpIfSwReqStb : 1; //no description, reset value: 0x0, access type: WO
		uint32 pacPhyRxGpType : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacHtExtensionsGpIfSwReq_u;

/*REG_B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_TIME 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpIfTimeoutTime : 20; //GP interface timer current value, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0MacHtExtensionsGpIfTimeoutTime_u;

/*REG_B0_MAC_HT_EXTENSIONS_GP_IF_RESULT 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gpIfTimeoutEvent : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacHtExtensionsGpIfResult_u;

/*REG_B0_MAC_HT_EXTENSIONS_ACTIVITY_LOGGER_SW_EVENT 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 actLoggerSwEvent : 8; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0MacHtExtensionsActivityLoggerSwEvent_u;

/*REG_B0_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscRxFreeHostRdsListIdx : 7; //Free Host RDs DLM list index which used as an interrupt to Rx GenRisc, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 genriscRxFreeFwRdsListIdx : 7; //Free FW RDs DLM list index which used as an interrupt to Rx GenRisc, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 genriscRxFreeDriverRdsListIdx : 7; //Free Driver RDs DLM list index which used as an interrupt to Rx GenRisc, reset value: 0x0, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegB0MacHtExtensionsGenriscRxFreeListIdx_u;

/*REG_B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifo0SrcSel : 3; //FIFO0 source select, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 fifo1SrcSel : 3; //FIFO1 source select, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 fifo2SrcSel : 3; //FIFO2 source select, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 fifo3SrcSel : 3; //FIFO3 source select, reset value: 0x0, access type: RW
		uint32 reserved3 : 1;
		uint32 fifo4SrcSel : 3; //FIFO4 source select, reset value: 0x0, access type: RW
		uint32 reserved4 : 1;
		uint32 fifo5SrcSel : 3; //FIFO4 source select, reset value: 0x0, access type: RW
		uint32 reserved5 : 9;
	} bitFields;
} RegB0MacHtExtensionsLoggerFifosSrcSel_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderPasPacTxError : 1; //Delia Tx error IRQ, 1 bit per Delia, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 senderDel2GenriscTxEndIrq : 1; //Delia Tx end IRQ, reset value: 0x0, access type: RO
		uint32 reserved1 : 27;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxEndIrq_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_MASK 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderPasPacTxErrorMask : 1; //Delia Tx error IRQ enable, 1 bit per Delia, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 senderDel2GenriscTxEndIrqMask : 1; //Delia Tx end IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxEndIrqMask_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleConf : 1; //Tx PD accelerator in IDLE, reset value: 0x1, access type: RO
		uint32 dma0WrapChIsIdleConf : 1; //DMA wrapper channel is IDLE, reset value: 0x1, access type: RO
		uint32 senderBaaIrqConf : 1; //Sender BAA IRQ , reset value: 0x0, access type: RO
		uint32 txhNtdTxGenriscSwReqIrqConf : 1; //TxH NTD genrisc_tx SW req IRQ , reset value: 0x0, access type: RO
		uint32 delLimitTimerExpiredIrqConf : 1; //Delia limit timer expired IRQ , reset value: 0x0, access type: RO
		uint32 arc2MacGenriscTxIrqConf : 2; //arc2mac genrisc_tx IRQ , reset value: 0x0, access type: RO
		uint32 sequencerIrqConf : 1; //Sequencer IRQ , reset value: 0x0, access type: RO
		uint32 txSecurityFailIrqConf : 1; //Tx Security fail IRQ, reset value: 0x0, access type: RO
		uint32 txSecurityDoneIrqConf : 1; //Tx Security done IRQ , reset value: 0x0, access type: RO
		uint32 txSecurityArbSmIdleConf : 1; //Tx Security engine arbiter state is not in Tx, reset value: 0x1, access type: RO
		uint32 sequencerEndIrqConf : 1; //Sequencer End IRQ , reset value: 0x0, access type: RO
		uint32 otfaIdleConf : 1; //OTFA in IDLE, reset value: 0x1, access type: RO
		uint32 baaIdleConf : 1; //BAA in IDLE, reset value: 0x1, access type: RO
		uint32 txHcIdleConf : 1; //TX_HC in IDLE, reset value: 0x1, access type: RO
		uint32 txDmaAlignerIdleConf : 1; //TX DMA Aligner in IDLE, reset value: 0x1, access type: RO
		uint32 dmaDoneConf : 1; //TX DMA Done indication, reset value: 0x0, access type: RO
		uint32 durationReadyConf : 1; //TX MPDU Duration field ready, reset value: 0x0, access type: RO
		uint32 deliaRecoveryDoneConf : 1; //Delia recovery done, reset value: 0x0, access type: RO
		uint32 tfGeneratorIdleConf : 1; //TF generator IDLE, reset value: 0x0, access type: RO
		uint32 bsrcSenderCmdDoneIrqConf : 1; //BSRC Sender command has completed, reset value: 0x0, access type: RO
		uint32 arDelTbFrameReadyConf : 1; //Auto-resp TB frame ready, reset value: 0x0, access type: RO
		uint32 autoReplyDurationReadyConf : 1; //Auto-reply duration ready, reset value: 0x0, access type: RO
		uint32 macGenriscTxIrqConfRsvd23Conf : 1; //Reserved, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxLevelIrqConf_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask0 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask0 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask0 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask0 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask0 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask0 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask0 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask0 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask0 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask0 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask0 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask0 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask0 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask0 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask0 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask0 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask0 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask0 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask0 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask0 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask0 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask0 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask0 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask0_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask1 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask1 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask1 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask1 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask1 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask1 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask1 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask1 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask1 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask1 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask1 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask1 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask1 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask1 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask1 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask1 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask1 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask1 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask1 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask1 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask1 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask1 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask1 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask1_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask2 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask2 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask2 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask2 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask2 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask2 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask2 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask2 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask2 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask2 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask2 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask2 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask2 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask2 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask2 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask2 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask2 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask2 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask2 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask2 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask2 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask2 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask2 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask2_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask3 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask3 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask3 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask3 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask3 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask3 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask3 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask3 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask3 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask3 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask3 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask3 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask3 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask3 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask3 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask3 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask3 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask3 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask3 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask3 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask3 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask3 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask3 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask3_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask4 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask4 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask4 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask4 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask4 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask4 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask4 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask4 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask4 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask4 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask4 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask4 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask4 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask4 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask4 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask4 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask4 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask4 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask4 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask4 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask4 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask4 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask4 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask4_u;

/*REG_B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txPdAccIdleMask5 : 1; //Tx PD accelerator in IDLE mask, reset value: 0x0, access type: RW
		uint32 dma0WrapChIsIdleMask5 : 1; //DMA wrapper channel is IDLE mask, reset value: 0x0, access type: RW
		uint32 senderBaaIrqMask5 : 1; //Sender BAA IRQ mask, reset value: 0x0, access type: RW
		uint32 txhNtdTxGenriscSwReqIrqMask5 : 1; //TxH NTD genrisc_tx SW req IRQ mask, reset value: 0x0, access type: RW
		uint32 delLimitTimerExpiredIrqMask5 : 1; //Delia limit timer expired IRQ mask, reset value: 0x0, access type: RW
		uint32 arc2MacGenriscTxIrqMask5 : 2; //arc2mac genrisc_tx IRQ mask, reset value: 0x0, access type: RW
		uint32 sequencerIrqMask5 : 1; //Sequencer IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityFailIrqMask5 : 1; //Tx Security fail IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityDoneIrqMask5 : 1; //Tx Security done IRQ mask, reset value: 0x0, access type: RW
		uint32 txSecurityArbSmIdleMask5 : 1; //Tx Security engine arbiter state is not in Tx mask, reset value: 0x0, access type: RW
		uint32 sequencerEndIrqMask5 : 1; //Sequencer End IRQ mask, reset value: 0x0, access type: RW
		uint32 otfaIdleMask5 : 1; //OTFA in IDLE mask, reset value: 0x0, access type: RW
		uint32 baaIdleMask5 : 1; //BAA in IDLE mask, reset value: 0x0, access type: RW
		uint32 txHcIdleMask5 : 1; //TX_HC in IDLE mask, reset value: 0x0, access type: RW
		uint32 txDmaAlignerIdleMask5 : 1; //TX DMA Aligner in IDLE mask, reset value: 0x0, access type: RW
		uint32 dmaDoneMask5 : 1; //TX DMA Done indication mask, reset value: 0x0, access type: RW
		uint32 durationReadyMask5 : 1; //TX MPDU Duration field ready mask, reset value: 0x0, access type: RW
		uint32 deliaRecoveryDoneMask5 : 1; //Delia recovery done mask, reset value: 0x0, access type: RW
		uint32 tfGeneratorIdleMask5 : 1; //TF generator IDLE mask, reset value: 0x0, access type: RW
		uint32 bsrcSenderCmdDoneIrqMask5 : 1; //BSRC Sender command done IRQ mask, reset value: 0x0, access type: RW
		uint32 arDelTbFrameReadyMask5 : 1; //Auto-resp TB frame ready mask, reset value: 0x0, access type: RW
		uint32 autoReplyDurationReadyMask5 : 1; //Auto-reply duration ready mask, reset value: 0x0, access type: RW
		uint32 macGenriscTxIrqConfRsvd23Mask5 : 1; //Reserved, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacHtExtensionsMacGenriscTxIrqConfMask5_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS1_RM 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams1Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams1Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams1Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams1Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams1Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams1Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams1Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams1Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams1Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS2_RM 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams2Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams2Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams2Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams2Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams2Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams2Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams2Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams2Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams2Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS3_RM 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams3Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams3Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams3Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams3Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams3Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams3Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams3Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams3Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams3Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS4_RM 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams4Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams4Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams4Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams4Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams4Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams4Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams4Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams4Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams4Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS5_RM 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams5Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams5Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams5Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams5Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams5Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams5Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams5Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams5Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams5Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS6_RM 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams6Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams6Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams6Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams6Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams6Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams6Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams6Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams6Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams6Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS7_RM 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams7Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams7Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams7Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams7Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams7Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams7Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams7Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams7Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams7Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_RAMS8_RM 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rams8Rm0 : 3; //Other MAC RAMs RM, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 rams8Rm1 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved1 : 1;
		uint32 rams8Rm2 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 1;
		uint32 rams8Rm3 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved3 : 1;
		uint32 rams8Rm4 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved4 : 1;
		uint32 rams8Rm5 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved5 : 1;
		uint32 rams8Rm6 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved6 : 1;
		uint32 rams8Rm7 : 3; //no description, reset value: 0x3, access type: RW
		uint32 reserved7 : 1;
	} bitFields;
} RegB0MacHtExtensionsRams8Rm_u;

/*REG_B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscPreAggStdContention : 1; //STD access contention between Genrisc and Pre-aggregator. Cleared by writing '1' to std_contention_clr field, reset value: 0x0, access type: RO
		uint32 genriscSequencerStdContention : 1; //STD access contention between Genrisc and Sequencer. Cleared by writing '1' to std_contention_clr field, reset value: 0x0, access type: RO
		uint32 preAggSequencerStdContention : 1; //STD access contention between Pre-aggregator and Sequencer. Cleared by writing '1' to std_contention_clr field, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 genriscPreAggStdContentionAddr : 11; //STD contention address in case of contention between Genrisc and Pre-aggregator. Locked for the first contention event. In order to reuse, need to clear genrisc_pre_agg_std_contention, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 genriscSequencerStdContentionAddr : 11; //STD contention address in case of contention between Genrisc and Sequencer. Locked for the first contention event. In order to reuse, need to clear genrisc_sequencer_std_contention, reset value: 0x0, access type: RO
		uint32 reserved2 : 5;
	} bitFields;
} RegB0MacHtExtensionsStdContentionEvents_u;

/*REG_B0_MAC_HT_EXTENSIONS_EVENT_CONTROL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stdContentionClr : 1; //Clear STD Contention events., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacHtExtensionsEventControl_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clkControlForceOnMacGenriscTx : 1; //Clock Force On MAC_GENRISC_TX, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMacGenriscRx : 1; //Clock Force On MAC_GENRISC_RX, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMacGenriscTxStm : 1; //Clock Force On MAC_GENRISC_TX_STM, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMacGenriscRxStm : 1; //Clock Force On MAC_GENRISC_RX_STM, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxMsduParser : 1; //Clock Force On MSDU_PARSER, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxMsduParserLogger : 1; //Clock Force On MSDU_PARSER_LOGGER, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxClassifier : 1; //Clock Force On Rx Classifier, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTxHc : 1; //Clock Force On TX_HC, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMacAddr2Index : 1; //Clock Force On MAC_ADDR2INDEX, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTxDmaAligner : 1; //Clock Force On TX_DMA_ALIGNER, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxCoordinatorStatusTrace : 1; //Clock Force On RX coordinator status trace unit, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxCoordinatorStatusTraceFifos : 1; //Clock Force On RX coordinator status trace unit fifos, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxCoordinatorWrapUp : 1; //Clock Force On RX coordinator wrap up, reset value: 0x0, access type: RW
		uint32 clkControlForceOnRxCoordinatorStatistics : 1; //Clock Force On RX coordinator statistics, reset value: 0x0, access type: RW
		uint32 secEngGclkBypass : 5; //Security Clock Force On, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMacAirTimeCalc : 1; //Clock Force On MAC Air Time Calculator, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 delGclkBypass : 4; //Delia Clock Force On, reset value: 0x0, access type: RW
		uint32 clkControlForceOnAutoResp : 2; //Clock bypass for Auto response. bit [0] - bypass clock gater for auto resp general logic. bit[1] - bypass clock gater frame gen and length calculation, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTxPdAcc : 1; //Clock Force On TX_PD Acc, reset value: 0x0, access type: RW
		uint32 clkControlForceOnPreAgg : 1; //Clock Force On for Pre AGG. , reset value: 0x0, access type: RW
		uint32 beaconTimersGclkBypass : 1; //Clock Force On for Beacon timers, reset value: 0x0, access type: RW
		uint32 bestruGclkBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 clkControlForceOnUlPostRx : 1; //UL_POST_RX Clock Force On, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacHtExtensionsClkControlRegForceOn_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clkControlDynEnMacGenriscTx : 1; //Clock Dynamic Enable MAC_GENRISC_TX, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMacGenriscRx : 1; //Clock Dynamic Enable MAC_GENRISC_RX, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMacGenriscTxStm : 1; //Clock Dynamic Enable MAC_GENRISC_TX_STM, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMacGenriscRxStm : 1; //Clock Dynamic Enable MAC_GENRISC_RX_STM, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxMsduParser : 1; //Clock Dynamic Enable MSDU_PARSER, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxMsduParserLogger : 1; //Clock Dynamic Enable MSDU_PARSER_LOGGER, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxClassifier : 1; //Clock Dynamic Enable Rx Classifier, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTxHc : 1; //Clock Dynamic Enable TX_HC, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMacAddr2Index : 1; //Clock Dynamic Enable MAC_ADDR2INDEX, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTxDmaAligner : 1; //Clock Dynamic Enable TX_DMA_ALIGNER, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxCoordinatorStatusTrace : 1; //Clock Dynamic Enable RX Coordinator status trace, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxCoordinatorStatusTraceFifos : 1; //Clock Dynamic Enable RX Coordinator status trace fifos, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxCoordinatorWrapUp : 1; //Clock Dynamic Enable RX Coordinator wrap up, reset value: 0x0, access type: RW
		uint32 clkControlDynEnRxCoordinatorStatistics : 1; //Clock Dynamic Enable RX Coordinator statistics, reset value: 0x0, access type: RW
		uint32 secEngSwGclkEn : 5; //Security Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMacAirTimeCalc : 1; //Clock Dynamic Enable MAC Air Time Calculator, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 delSwGclkEn : 4; //Delia Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 clkControlDynEnAutoResp : 2; //Clock Dynamic Enable Auto response. bit [0] - dynamic enable for auto resp general logic. bit[1] -  dynamic enable for frame gen and length calculation, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTxPdAcc : 1; //Clock Dynamic Enable TX_PD Acc, reset value: 0x0, access type: RW
		uint32 clkControlDynEnPreAgg : 1; //Clock Dynamic Enable Pre Agg, reset value: 0x0, access type: RW
		uint32 beaconTimersSwGclkEn : 1; //Clock Dynamic Enable Beacon timers, reset value: 0x0, access type: RW
		uint32 bestruGclkEn : 1; //BestRU Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 clkControlDynEnUlPostRx : 1; //UL_POST_RX Clock Dynamic Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacHtExtensionsClkControlRegDynEn_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 otfaGclkBypass : 2; //OTFA Clock Force On, reset value: 0x0, access type: RW
		uint32 txSequencerGclkBypass : 1; //Tx Sequencer Clock Force On, reset value: 0x0, access type: RW
		uint32 txSelGclkBypass : 8; //Tx Selector Clock Force On, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTfGenerator : 1; //Clock Force On TF_GENERATOR, reset value: 0x0, access type: RW
		uint32 clkControlForceOnMbfm : 1; //Clock Force On MBFM, reset value: 0x0, access type: RW
		uint32 psSettingGclkBypass : 1; //PS setting Clock Force On, reset value: 0x0, access type: RW
		uint32 bsrcGclkBypass : 4; //BSRC Clock Force On, reset value: 0x0, access type: RW
		uint32 rxcGclkBypass : 1; //RxC Clock Force On, reset value: 0x0, access type: RW
		uint32 baaGclkBypass : 8; //BAA Clock Force On, reset value: 0x0, access type: RW
		uint32 pacExtrapGclkBypass : 1; //PAC extrap Clock Force On, reset value: 0x0, access type: RW
		uint32 txhMapGclkBypass : 1; //TxH MAP Clock Force On, reset value: 0x0, access type: RW
		uint32 txhTxcGclkBypass : 1; //TxH TxC Clock Force On, reset value: 0x0, access type: RW
		uint32 txhNtdGclkBypass : 1; //TxH NTD Clock Force On, reset value: 0x0, access type: RW
		uint32 durGclkBypass : 1; //DUR Clock Force On, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacHtExtensionsClkControlRegForceOn2_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swGclkEnMdRam : 1; //gclk_en for MD RAM arbiter, reset value: 0x1, access type: RW
		uint32 swGclkBypassMdRam : 1; //gclk_bypass for MD RAM arbiter, reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 swGclkEnTxCircRam : 1; //gclk_en for TX Circ RAM arbiter, reset value: 0x1, access type: RW
		uint32 swGclkBypassTxCircRam : 1; //gclk_bypass for TX Circ RAM arbiter, reset value: 0x1, access type: RW
		uint32 reserved1 : 2;
		uint32 swGclkEnRxCircRam : 1; //gclk_en for RX Circ RAM arbiter, reset value: 0x1, access type: RW
		uint32 swGclkBypassRxCircRam : 1; //gclk_bypass for RX Circ RAM arbiter, reset value: 0x1, access type: RW
		uint32 reserved2 : 22;
	} bitFields;
} RegB0MacHtExtensionsClkControlRegMpaArb_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 otfaSwGclkEn : 1; //OTFA Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 txSequencerSwGclkEn : 1; //Tx Sequencer Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 txSelSwGclkEn : 8; //Tx Selector Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTfGenerator : 1; //Clock Dynamic Enable TF_GENERATOR, reset value: 0x0, access type: RW
		uint32 clkControlDynEnMbfm : 1; //Clock Dynamic Enable MBFM, reset value: 0x0, access type: RW
		uint32 psSettingSwGclkEn : 1; //PS setting Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 bsrcSwGclkEn : 4; //BSRC Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 rxcSwGclkEn : 1; //RxC Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 baaSwGclkEn : 8; //BAA Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 pacExtrapGclkEn : 1; //PAC extrap Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 txhMapSwGclkEn : 1; //TxH MAP Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 txhTxcSwGclkEn : 1; //TxH TxC Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 txhNtdSwGclkEn : 1; //TxH NTD Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 durSwGclkEn : 1; //DUR Clock Dynamic Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacHtExtensionsClkControlRegDynEn2_u;

/*REG_B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPO 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htExtSpareGpo : 8; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0MacHtExtensionsHtExtSpareGpo_u;

/*REG_B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPI 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htExtSpareGpi : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0MacHtExtensionsHtExtSpareGpi_u;

/*REG_B0_MAC_HT_EXTENSIONS_PHY_MAC_IF_TEST_BUS_STOP 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacIfTestBusStop : 1; //Write '1' to stop test bus of PHY-MAC I/F, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacHtExtensionsPhyMacIfTestBusStop_u;

/*REG_B0_MAC_HT_EXTENSIONS_VAP_MODE_BITMAP 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapModeBitmap : 32; //VAP mode bitmap. The location of each bit designates the VAP Index. Used for differntiating between AP mode and Station mode.  0 - AP;  1 - STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacHtExtensionsVapModeBitmap_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bestruGclkSwBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 clkControlForceOnTfDecoder : 1; //TF Decoder Clock Force On, reset value: 0x0, access type: RW
		uint32 rdAccGclkSwBypass : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0MacHtExtensionsClkControlRegForceOn3_u;

/*REG_B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bestruGclkSwEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 clkControlDynEnTfDecoder : 1; //TF Decoder Clock Dynamic Enable, reset value: 0x0, access type: RW
		uint32 rdAccGclkSwEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0MacHtExtensionsClkControlRegDynEn3_u;

//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_BAND_ID 0x0 */
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_ID_FIELD_MAC_BAND_ID_MASK                                                   0x00000003
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL 0x4 */
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL_FIELD_STA_AID_OFFSET_65_EN_MASK                                     0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL_FIELD_ASPM_EN_BAND_MASK                                             0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL_FIELD_ASPM_ON_OVERRIDE_BAND_MASK                                    0x00000020
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL_FIELD_BAND_PRIMARY_CHANNEL_MASK                                     0x00000F00
#define B0_MAC_HT_EXTENSIONS_MAC_BAND_CONTROL_FIELD_BAND_ALLOWED_CHANNELS_MASK                                    0xFFFF0000
//========================================
/* REG_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR 0x10 */
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_DEL2GENRISC_TX_END_IRQ_CLR_MASK                                 0x00000100
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_DEL_AUTO_FILL_END_IRQ_CLR_MASK                                  0x00008000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_PAS_PAC_TICK_TIMER_CLR_MASK                                     0x00040000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_MAC_PHY_GP_IF_IRQ_CLR_MASK                                      0x00080000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_PAS_PAC0_TX_ERROR_CLR_MASK                                      0x00100000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_CLR_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_CLR_MASK                            0x02000000
//========================================
/* REG_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS 0x14 */
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_DEL2GENRISC_TX_END_IRQ_STATUS_MASK                           0x00000100
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_DEL_AUTO_FILL_END_IRQ_STATUS_MASK                            0x00008000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_PAS_PAC_TICK_TIMER_STATUS_MASK                               0x00040000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_MAC_PHY_GP_IF_IRQ_STATUS_MASK                                0x00080000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_PAS_PAC_TX_ERROR_STATUS_MASK                                 0x00100000
#define B0_MAC_HT_EXTENSIONS_WL_MAC_IRQ_STATUS_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_STATUS_MASK                      0x02000000
//========================================
/* REG_MAC_HT_EXTENSIONS_GP_IF_TIMER_PRESCALER 0x20 */
#define B0_MAC_HT_EXTENSIONS_GP_IF_TIMER_PRESCALER_FIELD_GP_IF_TIMER_PRESCALER_MASK                               0x000003FF
//========================================
/* REG_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_VAL 0x24 */
#define B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_VAL_FIELD_GP_IF_TIMEOUT_VAL_MASK                                       0x000FFFFF
//========================================
/* REG_MAC_HT_EXTENSIONS_GP_IF_SW_REQ 0x28 */
#define B0_MAC_HT_EXTENSIONS_GP_IF_SW_REQ_FIELD_GP_IF_SW_REQ_STB_MASK                                             0x00000001
#define B0_MAC_HT_EXTENSIONS_GP_IF_SW_REQ_FIELD_PAC_PHY_RX_GP_TYPE_MASK                                           0x00000002
//========================================
/* REG_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_TIME 0x2C */
#define B0_MAC_HT_EXTENSIONS_GP_IF_TIMEOUT_TIME_FIELD_GP_IF_TIMEOUT_TIME_MASK                                     0x000FFFFF
//========================================
/* REG_MAC_HT_EXTENSIONS_GP_IF_RESULT 0x30 */
#define B0_MAC_HT_EXTENSIONS_GP_IF_RESULT_FIELD_GP_IF_TIMEOUT_EVENT_MASK                                          0x00000001
//========================================
/* REG_MAC_HT_EXTENSIONS_ACTIVITY_LOGGER_SW_EVENT 0x34 */
#define B0_MAC_HT_EXTENSIONS_ACTIVITY_LOGGER_SW_EVENT_FIELD_ACT_LOGGER_SW_EVENT_MASK                              0x000000FF
//========================================
/* REG_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX 0x38 */
#define B0_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX_FIELD_GENRISC_RX_FREE_HOST_RDS_LIST_IDX_MASK                0x0000007F
#define B0_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX_FIELD_GENRISC_RX_FREE_FW_RDS_LIST_IDX_MASK                  0x00007F00
#define B0_MAC_HT_EXTENSIONS_GENRISC_RX_FREE_LIST_IDX_FIELD_GENRISC_RX_FREE_DRIVER_RDS_LIST_IDX_MASK              0x007F0000
//========================================
/* REG_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL 0x3C */
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO0_SRC_SEL_MASK                                        0x00000007
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO1_SRC_SEL_MASK                                        0x00000070
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO2_SRC_SEL_MASK                                        0x00000700
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO3_SRC_SEL_MASK                                        0x00007000
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO4_SRC_SEL_MASK                                        0x00070000
#define B0_MAC_HT_EXTENSIONS_LOGGER_FIFOS_SRC_SEL_FIELD_FIFO5_SRC_SEL_MASK                                        0x00700000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ 0x40 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_FIELD_SENDER_PAS_PAC_TX_ERROR_MASK                            0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_FIELD_SENDER_DEL2GENRISC_TX_END_IRQ_MASK                      0x00000010
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_MASK 0x44 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_MASK_FIELD_SENDER_PAS_PAC_TX_ERROR_MASK_MASK                  0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_END_IRQ_MASK_FIELD_SENDER_DEL2GENRISC_TX_END_IRQ_MASK_MASK            0x00000010
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF 0x48 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_PD_ACC_IDLE_CONF_MASK                         0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_DMA0_WRAP_CH_IS_IDLE_CONF_MASK                   0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_SENDER_BAA_IRQ_CONF_MASK                         0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_CONF_MASK          0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_CONF_MASK            0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_ARC2MAC_GENRISC_TX_IRQ_CONF_MASK                 0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_SEQUENCER_IRQ_CONF_MASK                          0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_SECURITY_FAIL_IRQ_CONF_MASK                   0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_SECURITY_DONE_IRQ_CONF_MASK                   0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_SECURITY_ARB_SM_IDLE_CONF_MASK                0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_SEQUENCER_END_IRQ_CONF_MASK                      0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_OTFA_IDLE_CONF_MASK                              0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_BAA_IDLE_CONF_MASK                               0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_HC_IDLE_CONF_MASK                             0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TX_DMA_ALIGNER_IDLE_CONF_MASK                    0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_DMA_DONE_CONF_MASK                               0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_DURATION_READY_CONF_MASK                         0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_DELIA_RECOVERY_DONE_CONF_MASK                    0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_TF_GENERATOR_IDLE_CONF_MASK                      0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_BSRC_SENDER_CMD_DONE_IRQ_CONF_MASK               0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_AR_DEL_TB_FRAME_READY_CONF_MASK                  0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_AUTO_REPLY_DURATION_READY_CONF_MASK              0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_LEVEL_IRQ_CONF_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_CONF_MASK         0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0 0x4C */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_PD_ACC_IDLE_MASK0_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK0_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_SENDER_BAA_IRQ_MASK0_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK0_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK0_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK0_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_SEQUENCER_IRQ_MASK0_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_SECURITY_FAIL_IRQ_MASK0_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_SECURITY_DONE_IRQ_MASK0_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK0_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_SEQUENCER_END_IRQ_MASK0_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_OTFA_IDLE_MASK0_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_BAA_IDLE_MASK0_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_HC_IDLE_MASK0_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TX_DMA_ALIGNER_IDLE_MASK0_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_DMA_DONE_MASK0_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_DURATION_READY_MASK0_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_DELIA_RECOVERY_DONE_MASK0_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_TF_GENERATOR_IDLE_MASK0_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK0_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_AR_DEL_TB_FRAME_READY_MASK0_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_AUTO_REPLY_DURATION_READY_MASK0_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK0_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK0_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1 0x50 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_PD_ACC_IDLE_MASK1_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK1_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_SENDER_BAA_IRQ_MASK1_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK1_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK1_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK1_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_SEQUENCER_IRQ_MASK1_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_SECURITY_FAIL_IRQ_MASK1_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_SECURITY_DONE_IRQ_MASK1_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK1_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_SEQUENCER_END_IRQ_MASK1_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_OTFA_IDLE_MASK1_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_BAA_IDLE_MASK1_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_HC_IDLE_MASK1_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TX_DMA_ALIGNER_IDLE_MASK1_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_DMA_DONE_MASK1_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_DURATION_READY_MASK1_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_DELIA_RECOVERY_DONE_MASK1_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_TF_GENERATOR_IDLE_MASK1_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK1_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_AR_DEL_TB_FRAME_READY_MASK1_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_AUTO_REPLY_DURATION_READY_MASK1_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK1_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK1_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2 0x54 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_PD_ACC_IDLE_MASK2_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK2_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_SENDER_BAA_IRQ_MASK2_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK2_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK2_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK2_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_SEQUENCER_IRQ_MASK2_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_SECURITY_FAIL_IRQ_MASK2_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_SECURITY_DONE_IRQ_MASK2_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK2_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_SEQUENCER_END_IRQ_MASK2_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_OTFA_IDLE_MASK2_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_BAA_IDLE_MASK2_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_HC_IDLE_MASK2_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TX_DMA_ALIGNER_IDLE_MASK2_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_DMA_DONE_MASK2_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_DURATION_READY_MASK2_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_DELIA_RECOVERY_DONE_MASK2_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_TF_GENERATOR_IDLE_MASK2_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK2_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_AR_DEL_TB_FRAME_READY_MASK2_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_AUTO_REPLY_DURATION_READY_MASK2_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK2_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK2_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3 0x58 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_PD_ACC_IDLE_MASK3_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK3_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_SENDER_BAA_IRQ_MASK3_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK3_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK3_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK3_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_SEQUENCER_IRQ_MASK3_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_SECURITY_FAIL_IRQ_MASK3_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_SECURITY_DONE_IRQ_MASK3_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK3_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_SEQUENCER_END_IRQ_MASK3_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_OTFA_IDLE_MASK3_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_BAA_IDLE_MASK3_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_HC_IDLE_MASK3_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TX_DMA_ALIGNER_IDLE_MASK3_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_DMA_DONE_MASK3_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_DURATION_READY_MASK3_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_DELIA_RECOVERY_DONE_MASK3_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_TF_GENERATOR_IDLE_MASK3_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK3_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_AR_DEL_TB_FRAME_READY_MASK3_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_AUTO_REPLY_DURATION_READY_MASK3_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK3_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK3_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4 0x5C */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_PD_ACC_IDLE_MASK4_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK4_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_SENDER_BAA_IRQ_MASK4_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK4_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK4_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK4_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_SEQUENCER_IRQ_MASK4_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_SECURITY_FAIL_IRQ_MASK4_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_SECURITY_DONE_IRQ_MASK4_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK4_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_SEQUENCER_END_IRQ_MASK4_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_OTFA_IDLE_MASK4_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_BAA_IDLE_MASK4_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_HC_IDLE_MASK4_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TX_DMA_ALIGNER_IDLE_MASK4_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_DMA_DONE_MASK4_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_DURATION_READY_MASK4_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_DELIA_RECOVERY_DONE_MASK4_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_TF_GENERATOR_IDLE_MASK4_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK4_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_AR_DEL_TB_FRAME_READY_MASK4_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_AUTO_REPLY_DURATION_READY_MASK4_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK4_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK4_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5 0x60 */
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_PD_ACC_IDLE_MASK5_MASK                        0x00000001
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_DMA0_WRAP_CH_IS_IDLE_MASK5_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_SENDER_BAA_IRQ_MASK5_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TXH_NTD_TX_GENRISC_SW_REQ_IRQ_MASK5_MASK         0x00000008
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_DEL_LIMIT_TIMER_EXPIRED_IRQ_MASK5_MASK           0x00000010
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_ARC2MAC_GENRISC_TX_IRQ_MASK5_MASK                0x00000060
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_SEQUENCER_IRQ_MASK5_MASK                         0x00000080
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_SECURITY_FAIL_IRQ_MASK5_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_SECURITY_DONE_IRQ_MASK5_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_SECURITY_ARB_SM_IDLE_MASK5_MASK               0x00000400
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_SEQUENCER_END_IRQ_MASK5_MASK                     0x00000800
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_OTFA_IDLE_MASK5_MASK                             0x00001000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_BAA_IDLE_MASK5_MASK                              0x00002000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_HC_IDLE_MASK5_MASK                            0x00004000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TX_DMA_ALIGNER_IDLE_MASK5_MASK                   0x00008000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_DMA_DONE_MASK5_MASK                              0x00010000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_DURATION_READY_MASK5_MASK                        0x00020000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_DELIA_RECOVERY_DONE_MASK5_MASK                   0x00040000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_TF_GENERATOR_IDLE_MASK5_MASK                     0x00080000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_BSRC_SENDER_CMD_DONE_IRQ_MASK5_MASK              0x00100000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_AR_DEL_TB_FRAME_READY_MASK5_MASK                 0x00200000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_AUTO_REPLY_DURATION_READY_MASK5_MASK             0x00400000
#define B0_MAC_HT_EXTENSIONS_MAC_GENRISC_TX_IRQ_CONF_MASK5_FIELD_MAC_GENRISC_TX_IRQ_CONF_RSVD23_MASK5_MASK        0x00800000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS1_RM 0x6C */
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS1_RM_FIELD_RAMS1_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS2_RM 0x70 */
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS2_RM_FIELD_RAMS2_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS3_RM 0x74 */
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS3_RM_FIELD_RAMS3_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS4_RM 0x78 */
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS4_RM_FIELD_RAMS4_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS5_RM 0x7C */
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS5_RM_FIELD_RAMS5_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS6_RM 0x80 */
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS6_RM_FIELD_RAMS6_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS7_RM 0x84 */
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS7_RM_FIELD_RAMS7_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_RAMS8_RM 0x90 */
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_0_MASK                                                       0x00000007
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_1_MASK                                                       0x00000070
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_2_MASK                                                       0x00000700
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_3_MASK                                                       0x00007000
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_4_MASK                                                       0x00070000
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_5_MASK                                                       0x00700000
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_6_MASK                                                       0x07000000
#define B0_MAC_HT_EXTENSIONS_RAMS8_RM_FIELD_RAMS8_RM_7_MASK                                                       0x70000000
//========================================
/* REG_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS 0x94 */
#define B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS_FIELD_GENRISC_PRE_AGG_STD_CONTENTION_MASK                      0x00000001
#define B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS_FIELD_GENRISC_SEQUENCER_STD_CONTENTION_MASK                    0x00000002
#define B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS_FIELD_PRE_AGG_SEQUENCER_STD_CONTENTION_MASK                    0x00000004
#define B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS_FIELD_GENRISC_PRE_AGG_STD_CONTENTION_ADDR_MASK                 0x00007FF0
#define B0_MAC_HT_EXTENSIONS_STD_CONTENTION_EVENTS_FIELD_GENRISC_SEQUENCER_STD_CONTENTION_ADDR_MASK               0x07FF0000
//========================================
/* REG_MAC_HT_EXTENSIONS_EVENT_CONTROL 0xA0 */
#define B0_MAC_HT_EXTENSIONS_EVENT_CONTROL_FIELD_STD_CONTENTION_CLR_MASK                                          0x00000001
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON 0xA4 */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_GENRISC_TX_MASK              0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_GENRISC_RX_MASK              0x00000002
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_GENRISC_TX_STM_MASK          0x00000004
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_GENRISC_RX_STM_MASK          0x00000008
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_MSDU_PARSER_MASK              0x00000010
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_MSDU_PARSER_LOGGER_MASK       0x00000020
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_CLASSIFIER_MASK               0x00000040
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_TX_HC_MASK                       0x00000080
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_ADDR2INDEX_MASK              0x00000100
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_TX_DMA_ALIGNER_MASK              0x00000200
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_COORDINATOR_STATUS_TRACE_MASK 0x00000400
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_COORDINATOR_STATUS_TRACE_FIFOS_MASK 0x00000800
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_COORDINATOR_WRAP_UP_MASK      0x00001000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_RX_COORDINATOR_STATISTICS_MASK   0x00002000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_SEC_ENG_GCLK_BYPASS_MASK                              0x0007C000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_MAC_AIR_TIME_CALC_MASK           0x00080000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_DEL_GCLK_BYPASS_MASK                                  0x01E00000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_AUTO_RESP_MASK                   0x06000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_TX_PD_ACC_MASK                   0x08000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_PRE_AGG_MASK                     0x10000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_BEACON_TIMERS_GCLK_BYPASS_MASK                        0x20000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_BESTRU_GCLK_BYPASS_MASK                               0x40000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON_FIELD_CLK_CONTROL_FORCE_ON_UL_POST_RX_MASK                  0x80000000
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN 0xA8 */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_GENRISC_TX_MASK                  0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_GENRISC_RX_MASK                  0x00000002
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_GENRISC_TX_STM_MASK              0x00000004
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_GENRISC_RX_STM_MASK              0x00000008
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_MSDU_PARSER_MASK                  0x00000010
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_MSDU_PARSER_LOGGER_MASK           0x00000020
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_CLASSIFIER_MASK                   0x00000040
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_TX_HC_MASK                           0x00000080
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_ADDR2INDEX_MASK                  0x00000100
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_TX_DMA_ALIGNER_MASK                  0x00000200
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_COORDINATOR_STATUS_TRACE_MASK     0x00000400
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_COORDINATOR_STATUS_TRACE_FIFOS_MASK 0x00000800
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_COORDINATOR_WRAP_UP_MASK          0x00001000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_RX_COORDINATOR_STATISTICS_MASK       0x00002000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_SEC_ENG_SW_GCLK_EN_MASK                                 0x0007C000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_MAC_AIR_TIME_CALC_MASK               0x00080000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_DEL_SW_GCLK_EN_MASK                                     0x01E00000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_AUTO_RESP_MASK                       0x06000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_TX_PD_ACC_MASK                       0x08000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_PRE_AGG_MASK                         0x10000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_BEACON_TIMERS_SW_GCLK_EN_MASK                           0x20000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_BESTRU_GCLK_EN_MASK                                     0x40000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN_FIELD_CLK_CONTROL_DYN_EN_UL_POST_RX_MASK                      0x80000000
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2 0xAC */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_OTFA_GCLK_BYPASS_MASK                                0x00000003
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_TX_SEQUENCER_GCLK_BYPASS_MASK                        0x00000004
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_TX_SEL_GCLK_BYPASS_MASK                              0x000007F8
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_CLK_CONTROL_FORCE_ON_TF_GENERATOR_MASK               0x00000800
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_CLK_CONTROL_FORCE_ON_MBFM_MASK                       0x00001000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_PS_SETTING_GCLK_BYPASS_MASK                          0x00002000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_BSRC_GCLK_BYPASS_MASK                                0x0003C000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_RXC_GCLK_BYPASS_MASK                                 0x00040000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_BAA_GCLK_BYPASS_MASK                                 0x07F80000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_PAC_EXTRAP_GCLK_BYPASS_MASK                          0x08000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_TXH_MAP_GCLK_BYPASS_MASK                             0x10000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_TXH_TXC_GCLK_BYPASS_MASK                             0x20000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_TXH_NTD_GCLK_BYPASS_MASK                             0x40000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON2_FIELD_DUR_GCLK_BYPASS_MASK                                 0x80000000
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB 0xB0 */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_EN_MD_RAM_MASK                                 0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_BYPASS_MD_RAM_MASK                             0x00000002
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_EN_TX_CIRC_RAM_MASK                            0x00000010
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_BYPASS_TX_CIRC_RAM_MASK                        0x00000020
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_EN_RX_CIRC_RAM_MASK                            0x00000100
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_MPA_ARB_FIELD_SW_GCLK_BYPASS_RX_CIRC_RAM_MASK                        0x00000200
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2 0xC0 */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_OTFA_SW_GCLK_EN_MASK                                   0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_TX_SEQUENCER_SW_GCLK_EN_MASK                           0x00000004
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_TX_SEL_SW_GCLK_EN_MASK                                 0x000007F8
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_CLK_CONTROL_DYN_EN_TF_GENERATOR_MASK                   0x00000800
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_CLK_CONTROL_DYN_EN_MBFM_MASK                           0x00001000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_PS_SETTING_SW_GCLK_EN_MASK                             0x00002000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_BSRC_SW_GCLK_EN_MASK                                   0x0003C000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_RXC_SW_GCLK_EN_MASK                                    0x00040000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_BAA_SW_GCLK_EN_MASK                                    0x07F80000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_PAC_EXTRAP_GCLK_EN_MASK                                0x08000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_TXH_MAP_SW_GCLK_EN_MASK                                0x10000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_TXH_TXC_SW_GCLK_EN_MASK                                0x20000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_TXH_NTD_SW_GCLK_EN_MASK                                0x40000000
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN2_FIELD_DUR_SW_GCLK_EN_MASK                                    0x80000000
//========================================
/* REG_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPO 0xC4 */
#define B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPO_FIELD_HT_EXT_SPARE_GPO_MASK                                         0x000000FF
//========================================
/* REG_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPI 0xC8 */
#define B0_MAC_HT_EXTENSIONS_HT_EXT_SPARE_GPI_FIELD_HT_EXT_SPARE_GPI_MASK                                         0x000000FF
//========================================
/* REG_MAC_HT_EXTENSIONS_PHY_MAC_IF_TEST_BUS_STOP 0xF0 */
#define B0_MAC_HT_EXTENSIONS_PHY_MAC_IF_TEST_BUS_STOP_FIELD_PHY_MAC_IF_TEST_BUS_STOP_MASK                         0x00000001
//========================================
/* REG_MAC_HT_EXTENSIONS_VAP_MODE_BITMAP 0xF4 */
#define B0_MAC_HT_EXTENSIONS_VAP_MODE_BITMAP_FIELD_VAP_MODE_BITMAP_MASK                                           0xFFFFFFFF
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3 0xF8 */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3_FIELD_BESTRU_GCLK_SW_BYPASS_MASK                           0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3_FIELD_CLK_CONTROL_FORCE_ON_TF_DECODER_MASK                 0x00000002
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_FORCE_ON3_FIELD_RD_ACC_GCLK_SW_BYPASS_MASK                           0x00000004
//========================================
/* REG_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3 0xFC */
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3_FIELD_BESTRU_GCLK_SW_EN_MASK                                 0x00000001
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3_FIELD_CLK_CONTROL_DYN_EN_TF_DECODER_MASK                     0x00000002
#define B0_MAC_HT_EXTENSIONS_CLK_CONTROL_REG_DYN_EN3_FIELD_RD_ACC_GCLK_SW_EN_MASK                                 0x00000004


#endif // _MAC_HT_EXTENSIONS_REGS_H_
