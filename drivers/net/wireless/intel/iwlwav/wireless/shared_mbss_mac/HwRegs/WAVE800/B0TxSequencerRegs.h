
/***********************************************************************************
File:				B0TxSequencerRegs.h
Module:				b0TxSequencer
SOC Revision:		latest
Generated at:       2024-06-26 12:54:34
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TX_SEQUENCER_REGS_H_
#define _B0_TX_SEQUENCER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TX_SEQUENCER_BASE_ADDRESS                              MEMORY_MAP_UNIT_68_BASE_ADDRESS
#define	REG_B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x0)
#define	REG_B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0x4)
#define	REG_B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x8)
#define	REG_B0_TX_SEQUENCER_SEQUENCE_EXPECTED                           (B0_TX_SEQUENCER_BASE_ADDRESS + 0xC)
#define	REG_B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x10)
#define	REG_B0_TX_SEQUENCER_MINIMAL_SEQ_TIME_THRESHOLD                  (B0_TX_SEQUENCER_BASE_ADDRESS + 0x14)
#define	REG_B0_TX_SEQUENCER_MINIMAL_SU_TXOP_TIME_THRESHOLD              (B0_TX_SEQUENCER_BASE_ADDRESS + 0x18)
#define	REG_B0_TX_SEQUENCER_MINIMAL_VHT_MU_TXOP_TIME_THRESHOLD          (B0_TX_SEQUENCER_BASE_ADDRESS + 0x1C)
#define	REG_B0_TX_SEQUENCER_MINIMAL_HE_MU_TXOP_TIME_THRESHOLD           (B0_TX_SEQUENCER_BASE_ADDRESS + 0x20)
#define	REG_B0_TX_SEQUENCER_MINIMAL_BF_SEQ_TIME_THRESHOLD               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x24)
#define	REG_B0_TX_SEQUENCER_MINIMAL_CF_END_TIME_THRESHOLD               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x28)
#define	REG_B0_TX_SEQUENCER_MINIMAL_ETSI_FILLER_TIME_THRESHOLD          (B0_TX_SEQUENCER_BASE_ADDRESS + 0x2C)
#define	REG_B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x30)
#define	REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TX                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x34)
#define	REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_RX                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x38)
#define	REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TIMEOUT               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x3C)
#define	REG_B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x40)
#define	REG_B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x44)
#define	REG_B0_TX_SEQUENCER_BW_LIMITS                                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x48)
#define	REG_B0_TX_SEQUENCER_SET_BW_LIMIT                                (B0_TX_SEQUENCER_BASE_ADDRESS + 0x4C)
#define	REG_B0_TX_SEQUENCER_SET_FILLER_RA_BF_RPT_BW_LOWER_EN            (B0_TX_SEQUENCER_BASE_ADDRESS + 0x50)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS                     (B0_TX_SEQUENCER_BASE_ADDRESS + 0x54)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN                         (B0_TX_SEQUENCER_BASE_ADDRESS + 0x58)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0x5C)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS                 (B0_TX_SEQUENCER_BASE_ADDRESS + 0x60)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN                     (B0_TX_SEQUENCER_BASE_ADDRESS + 0x64)
#define	REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x68)
#define	REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ                           (B0_TX_SEQUENCER_BASE_ADDRESS + 0x6C)
#define	REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_EN                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0x70)
#define	REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_CLR                       (B0_TX_SEQUENCER_BASE_ADDRESS + 0x74)
#define	REG_B0_TX_SEQUENCER_BF_RPT_VALID                                (B0_TX_SEQUENCER_BASE_ADDRESS + 0x78)
#define	REG_B0_TX_SEQUENCER_BF_RPT_VALID_CLR                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0x7C)
#define	REG_B0_TX_SEQUENCER_AGG_REACT_PSDU_DUR_LIMIT_ADD                (B0_TX_SEQUENCER_BASE_ADDRESS + 0x80)
#define	REG_B0_TX_SEQUENCER_AGG_REACT_STATUS                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0x84)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_BASE_ADDR                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x88)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_DEPTH_MINUS_ONE              (B0_TX_SEQUENCER_BASE_ADDRESS + 0x8C)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_CLEAR_STRB                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x90)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_RD_ENTRIES_NUM               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x94)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_NUM_ENTRIES_COUNT            (B0_TX_SEQUENCER_BASE_ADDRESS + 0x98)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0x9C)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_CCA_CONTROL                       (B0_TX_SEQUENCER_BASE_ADDRESS + 0xA0)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_LOGGER                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xA4)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_LOGGER_ACTIVE                     (B0_TX_SEQUENCER_BASE_ADDRESS + 0xA8)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM                          (B0_TX_SEQUENCER_BASE_ADDRESS + 0xAC)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_DEBUG                             (B0_TX_SEQUENCER_BASE_ADDRESS + 0xB0)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_PLANNED_EXECUTED_DEBUG            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xB4)
#define	REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIMER                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xB8)
#define	REG_B0_TX_SEQUENCER_SENDER_USP_INDEX                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xBC)
#define	REG_B0_TX_SEQUENCER_SENDER_CURRENT_TRANSMITTED_PLAN_FLOW        (B0_TX_SEQUENCER_BASE_ADDRESS + 0xC0)
#define	REG_B0_TX_SEQUENCER_MAX_NUM_MULT_USERS_TRAINING                 (B0_TX_SEQUENCER_BASE_ADDRESS + 0xC4)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_REPORT_SENDER_BITS                (B0_TX_SEQUENCER_BASE_ADDRESS + 0xC8)
#define	REG_B0_TX_SEQUENCER_ABORT_WAIT_PHY_MULT_USERS_TRAINING          (B0_TX_SEQUENCER_BASE_ADDRESS + 0xCC)
#define	REG_B0_TX_SEQUENCER_PHY_SAMPLE_INFO_DEBUG_EN                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0xD0)
#define	REG_B0_TX_SEQUENCER_PHY_PAC_TX_PHY_MU_EFFECTIVE_RATE            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xD4)
#define	REG_B0_TX_SEQUENCER_PHY_PAC_TX_BF_31TO0                         (B0_TX_SEQUENCER_BASE_ADDRESS + 0xD8)
#define	REG_B0_TX_SEQUENCER_PHY_PAC_TX_BF_35TO32                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0xDC)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_END_IRQ                           (B0_TX_SEQUENCER_BASE_ADDRESS + 0xE0)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_END_IRQ_CLR                       (B0_TX_SEQUENCER_BASE_ADDRESS + 0xE4)
#define	REG_B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE_SETTING                (B0_TX_SEQUENCER_BASE_ADDRESS + 0xE8)
#define	REG_B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0xEC)
#define	REG_B0_TX_SEQUENCER_BAA_NEEDED_SET_31TO0                        (B0_TX_SEQUENCER_BASE_ADDRESS + 0xF0)
#define	REG_B0_TX_SEQUENCER_BAA_NEEDED_SET_35TO32                       (B0_TX_SEQUENCER_BASE_ADDRESS + 0xF4)
#define	REG_B0_TX_SEQUENCER_BAA_NEEDED_31TO0                            (B0_TX_SEQUENCER_BASE_ADDRESS + 0xF8)
#define	REG_B0_TX_SEQUENCER_BAA_NEEDED_35TO32                           (B0_TX_SEQUENCER_BASE_ADDRESS + 0xFC)
#define	REG_B0_TX_SEQUENCER_MAX_NUM_BF_SEQ_ITERATIONS                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x100)
#define	REG_B0_TX_SEQUENCER_TX_SEQ_DEL_RECIPE_POINTER                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x104)
#define	REG_B0_TX_SEQUENCER_BF_RPT_HE_MU_ONESHOT_PHASE                  (B0_TX_SEQUENCER_BASE_ADDRESS + 0x10C)
#define	REG_B0_TX_SEQUENCER_TRIGGER_CF_END_BAA_BUSY_EN                  (B0_TX_SEQUENCER_BASE_ADDRESS + 0x110)
#define	REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG1                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x114)
#define	REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG2                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x118)
#define	REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3                      (B0_TX_SEQUENCER_BASE_ADDRESS + 0x11C)
#define	REG_B0_TX_SEQUENCER_ETSI_MODE                                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x120)
#define	REG_B0_TX_SEQUENCER_SENDER_CFG_ETSI_BO_COUNT                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x124)
#define	REG_B0_TX_SEQUENCER_ETSI_BO_COUNTER                             (B0_TX_SEQUENCER_BASE_ADDRESS + 0x128)
#define	REG_B0_TX_SEQUENCER_ETSI_IFSN_ECW                               (B0_TX_SEQUENCER_BASE_ADDRESS + 0x12C)
#define	REG_B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_START                    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x130)
#define	REG_B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_RESULT                   (B0_TX_SEQUENCER_BASE_ADDRESS + 0x134)
#define	REG_B0_TX_SEQUENCER_LCG_RAND_FUNC_SEED                          (B0_TX_SEQUENCER_BASE_ADDRESS + 0x138)
#define	REG_B0_TX_SEQUENCER_LCG_RAND_FUNC_SHIFT_NUM                     (B0_TX_SEQUENCER_BASE_ADDRESS + 0x13C)
#define	REG_B0_TX_SEQUENCER_ADDITION_ETSI_FILLER_TXOP_TIME_THRESHOLD    (B0_TX_SEQUENCER_BASE_ADDRESS + 0x140)
#define	REG_B0_TX_SEQUENCER_SEQUENCER_TRANSACTION_NUMBER                (B0_TX_SEQUENCER_BASE_ADDRESS + 0x144)
#define	REG_B0_TX_SEQUENCER_SPARE_REGISTERS                             (B0_TX_SEQUENCER_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 successActivateBaaEn : 1; //Enable BAA activation, reset value: 0x0, access type: RW
		uint32 successVerifyCcaPrimaryEn : 1; //Enable cancel Tx due to CCA primary 20MHz busy, reset value: 0x0, access type: RW
		uint32 successVerifyCcaSecondaryEn : 1; //Enable verify CCA secondary, reset value: 0x0, access type: RW
		uint32 successUpdateDynamicBwEn : 1; //Enable update BW based on PHY status service bits, reset value: 0x0, access type: RW
		uint32 successBfIterationEn : 1; //Enable BF iteration, reset value: 0x0, access type: RW
		uint32 successClrBfReqTxSelBitmapEn : 1; //Enable clear of BF request in Tx Selector BitMAP, reset value: 0x0, access type: RW
		uint32 successAggReactivationEn : 1; //Enable AGG reactivation, reset value: 0x0, access type: RW
		uint32 successAggReactivationForce : 1; //Force AGG reactivation, reset value: 0x0, access type: RW
		uint32 successAggReactivationRdDb : 1; //Read DB in case of AGG reactivation, reset value: 0x0, access type: RW
		uint32 successAggVerifySifsLimitEn : 1; //Enable verify SIFS limit AGG operation, reset value: 0x0, access type: RW
		uint32 successTriggerTxReqEn : 1; //Enable triggering Tx request, reset value: 0x0, access type: RW
		uint32 successFillRateAdaptiveRptEn : 1; //Enable filling rate adaptive report, reset value: 0x0, access type: RW
		uint32 successTriggerCfEndForce : 1; //Force CF-end triggering, reset value: 0x0, access type: RW
		uint32 successTriggerCfEndEn : 1; //Enable CF-end triggering, reset value: 0x0, access type: RW
		uint32 successTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, reset value: 0x0, access type: RW
		uint32 successTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, reset value: 0x0, access type: RW
		uint32 successGenCpuIntEn : 1; //Generate interrupt to CPU, reset value: 0x0, access type: RW
		uint32 successClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, reset value: 0x0, access type: RW
		uint32 successVerifyMinSeqTimeEn : 1; //Enable verify minimum sequence time threshold, reset value: 0x0, access type: RW
		uint32 successWaitPhyMultUsersTrainingEn : 1; //Enable waiting PHY completes its operation on MU BF DB, reset value: 0x0, access type: RW
		uint32 successManageBfPollEn : 1; //Enable handling BF poll1 bit and STD ---Next BF report---, reset value: 0x0, access type: RW
		uint32 successEtsiFillerSettingEn : 1; //Enable setting CTS2Self filler, with cause of ETSI, reset value: 0x0, access type: RW
		uint32 successEtsiBoCountingEn : 1; //Enable counting BO before continue to the next Tx frame, reset value: 0x0, access type: RW
		uint32 successVerifyCcaPrimaryPostAggEn : 1; //Enable verify CCA primary 20MHz after Pre-AGG operation, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0TxSequencerSuccessfulSequenceEn_u;

/*REG_B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 otherActivateBaaEn : 1; //Enable BAA activation, reset value: 0x0, access type: RW
		uint32 otherVerifyCcaPrimaryEn : 1; //Enable cancel Tx due to CCA primary 20MHz busy, reset value: 0x0, access type: RW
		uint32 otherVerifyCcaSecondaryEn : 1; //Enable verify CCA secondary, reset value: 0x0, access type: RW
		uint32 otherUpdateDynamicBwEn : 1; //Enable update BW based on PHY status service bits, reset value: 0x0, access type: RW
		uint32 otherBfIterationEn : 1; //Enable BF iteration, reset value: 0x0, access type: RW
		uint32 otherClrBfReqTxSelBitmapEn : 1; //Enable clear of BF request in Tx Selector BitMAP, reset value: 0x0, access type: RW
		uint32 otherAggReactivationEn : 1; //Enable AGG reactivation, reset value: 0x0, access type: RW
		uint32 otherAggReactivationForce : 1; //Force AGG reactivation, reset value: 0x0, access type: RW
		uint32 otherAggReactivationRdDb : 1; //Read DB in case of AGG reactivation, reset value: 0x0, access type: RW
		uint32 otherAggVerifySifsLimitEn : 1; //Enable verify SIFS limit AGG operation, reset value: 0x0, access type: RW
		uint32 otherTriggerTxReqEn : 1; //Enable triggering Tx request, reset value: 0x0, access type: RW
		uint32 otherFillRateAdaptiveRptEn : 1; //Enable filling rate adaptive report, reset value: 0x0, access type: RW
		uint32 otherTriggerCfEndForce : 1; //Force CF-end triggering., reset value: 0x0, access type: RW
		uint32 otherTriggerCfEndEn : 1; //Enable CF-end triggering, reset value: 0x0, access type: RW
		uint32 otherTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, reset value: 0x0, access type: RW
		uint32 otherTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, reset value: 0x0, access type: RW
		uint32 otherGenCpuIntEn : 1; //Generate interrupt to CPU, reset value: 0x0, access type: RW
		uint32 otherClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, reset value: 0x0, access type: RW
		uint32 otherVerifyMinSeqTimeEn : 1; //Enable verify minimum sequence time threshold, reset value: 0x0, access type: RW
		uint32 otherWaitPhyMultUsersTrainingEn : 1; //Enable waiting PHY completes its operation on MU BF DB, reset value: 0x0, access type: RW
		uint32 otherManageBfPollEn : 1; //Enable handling BF poll1 bit and STD ---Next BF report---, reset value: 0x0, access type: RW
		uint32 otherEtsiFillerSettingEn : 1; //Enable setting CTS2Self filler, with cause of ETSI, reset value: 0x0, access type: RW
		uint32 otherEtsiBoCountingEn : 1; //Enable counting BO before continue to the next Tx frame, reset value: 0x0, access type: RW
		uint32 otherVerifyCcaPrimaryPostAggEn : 1; //Enable verify CCA primary 20MHz after Pre-AGG operation, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0TxSequencerOtherRxSequenceEn_u;

/*REG_B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timeoutActivateBaaEn : 1; //Enable BAA activation, reset value: 0x0, access type: RW
		uint32 timeoutVerifyCcaPrimaryEn : 1; //Enable cancel Tx due to CCA primary 20MHz busy, reset value: 0x0, access type: RW
		uint32 timeoutVerifyCcaSecondaryEn : 1; //Enable verify CCA secondary, reset value: 0x0, access type: RW
		uint32 timeoutUpdateDynamicBwEn : 1; //Enable update BW based on PHY status service bits, reset value: 0x0, access type: RW
		uint32 timeoutBfIterationEn : 1; //Enable BF iteration, reset value: 0x0, access type: RW
		uint32 timeoutClrBfReqTxSelBitmapEn : 1; //Enable clear of BF request in Tx Selector BitMAP, reset value: 0x0, access type: RW
		uint32 timeoutAggReactivationEn : 1; //Enable AGG reactivation, reset value: 0x0, access type: RW
		uint32 timeoutAggReactivationForce : 1; //Force AGG reactivation, reset value: 0x0, access type: RW
		uint32 timeoutAggReactivationRdDb : 1; //Read DB in case of AGG reactivation, reset value: 0x0, access type: RW
		uint32 timeoutAggVerifySifsLimitEn : 1; //Enable verify SIFS limit AGG operation, reset value: 0x0, access type: RW
		uint32 timeoutTriggerTxReqEn : 1; //Enable triggering Tx request, reset value: 0x0, access type: RW
		uint32 timeoutFillRateAdaptiveRptEn : 1; //Enable filling rate adaptive report, reset value: 0x0, access type: RW
		uint32 timeoutTriggerCfEndForce : 1; //Force CF-end triggering., reset value: 0x0, access type: RW
		uint32 timeoutTriggerCfEndEn : 1; //Enable CF-end triggering, reset value: 0x0, access type: RW
		uint32 timeoutTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, reset value: 0x0, access type: RW
		uint32 timeoutTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, reset value: 0x0, access type: RW
		uint32 timeoutGenCpuIntEn : 1; //Generate interrupt to CPU, reset value: 0x0, access type: RW
		uint32 timeoutClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, reset value: 0x0, access type: RW
		uint32 timeoutVerifyMinSeqTimeEn : 1; //Enable verify minimum sequence time threshold, reset value: 0x0, access type: RW
		uint32 timeoutWaitPhyMultUsersTrainingEn : 1; //Enable waiting PHY completes its operation on MU BF DB, reset value: 0x0, access type: RW
		uint32 timeoutManageBfPollEn : 1; //Enable handling BF poll1 bit and STD ---Next BF report---, reset value: 0x0, access type: RW
		uint32 timeoutEtsiFillerSettingEn : 1; //Enable setting CTS2Self filler, with cause of ETSI, reset value: 0x0, access type: RW
		uint32 timeoutEtsiBoCountingEn : 1; //Enable counting BO before continue to the next Tx frame, reset value: 0x0, access type: RW
		uint32 timeoutVerifyCcaPrimaryPostAggEn : 1; //Enable verify CCA primary 20MHz after Pre-AGG operation, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0TxSequencerRespTimeoutSequenceEn_u;

/*REG_B0_TX_SEQUENCER_SEQUENCE_EXPECTED 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 postTxSeqActivateStart : 1; //Sequencer activation after Post Tx, reset value: 0x0, access type: RW
		uint32 postRxSeqActivateStart : 1; //Sequencer activation after Post Rx, reset value: 0x0, access type: RW
		uint32 expectedRxAck : 1; //Expected Rx ACK, reset value: 0x0, access type: RW
		uint32 expectedRxBa : 1; //Expected Rx BA, reset value: 0x0, access type: RW
		uint32 expectedRxCts : 1; //Expected Rx CTS, reset value: 0x0, access type: RW
		uint32 expectedRxBfRpt : 1; //Expected Rx BF report, reset value: 0x0, access type: RW
		uint32 expectedRxAny : 1; //Expected Rx Any frame, reset value: 0x0, access type: RW
		uint32 expectedAutoRespAfterRx : 1; //Expected suto-resp after Rx, reset value: 0x0, access type: RW
		uint32 expectedRxNdpFb : 1; //Expected Rx NDP feedback, reset value: 0x0, access type: RW
		uint32 expectedRxBaBroadcast : 1; //Expected Rx non unicast BACK, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0TxSequencerSequenceExpected_u;

/*REG_B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cca20PBusyTriggerCfEndEn : 1; //Enable CF-end triggering, , In case of CCA 20MHz primary is busy., reset value: 0x0, access type: RW
		uint32 cca20PBusyTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, , In case of CCA 20MHz primary is busy, reset value: 0x0, access type: RW
		uint32 cca20PBusyTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, , In case of CCA 20MHz primary is busy, reset value: 0x0, access type: RW
		uint32 cca20PBusyGenCpuIntEn : 1; //Generate interrupt to CPU, , In case of CCA 20MHz primary is busy, reset value: 0x0, access type: RW
		uint32 cca20PBusyClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, , In case of CCA 20MHz primary is busy., reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 aggErrTriggerCfEndEn : 1; //Enable CF-end triggering, , In case of AGG builder error, reset value: 0x0, access type: RW
		uint32 aggErrTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, , In case of AGG builder error, reset value: 0x0, access type: RW
		uint32 aggErrTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, , In case of AGG builder error, reset value: 0x0, access type: RW
		uint32 aggErrGenCpuIntEn : 1; //Generate interrupt to CPU, , In case of AGG builder error, reset value: 0x0, access type: RW
		uint32 aggErrClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, , In case of AGG builder error, reset value: 0x0, access type: RW
		uint32 noFillerTriggerCfEndEn : 1; //Enable CF-end triggering, , In case of no filler supported, but there is a need to add a filler, reset value: 0x0, access type: RW
		uint32 noFillerTxSelBitmapUnlockEn : 1; //Enable unlock queue in Tx selector BitMAP, , In case of no filler supported, but there is a need to add a filler, reset value: 0x0, access type: RW
		uint32 noFillerTxhMapUnfreezeEn : 1; //Enable TXH MAP unfreeze, , In case of no filler supported, but there is a need to add a filler, reset value: 0x0, access type: RW
		uint32 noFillerGenCpuIntEn : 1; //Generate interrupt to CPU, , In case of no filler supported, but there is a need to add a filler, reset value: 0x0, access type: RW
		uint32 noFillerClrTxNavTimerEn : 1; //Enable clear Tx NAV timer, , In case of no filler supported, but there is a need to add a filler, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB0TxSequencerTxEndErrSequenceEn_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_SEQ_TIME_THRESHOLD 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalSeqTimeThreshold : 18; //Minimal time threshold for continue sequence flow, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalSeqTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_SU_TXOP_TIME_THRESHOLD 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalSuTxopTimeThreshold : 18; //Minimal time threshold for continue TXOP multiple main transmission sequence flow in SU, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalSuTxopTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_VHT_MU_TXOP_TIME_THRESHOLD 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalVhtMuTxopTimeThreshold : 18; //Minimal time threshold for continue TXOP multiple main transmission sequence flow in VHT MU, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalVhtMuTxopTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_HE_MU_TXOP_TIME_THRESHOLD 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalHeMuTxopTimeThreshold : 18; //Minimal time threshold for continue TXOP multiple main transmission sequence flow in HE MU, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalHeMuTxopTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_BF_SEQ_TIME_THRESHOLD 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalBfSeqTimeThreshold : 18; //Minimal time threshold for iteration of BF sequence, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalBfSeqTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_CF_END_TIME_THRESHOLD 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalCfEndTimeThreshold : 18; //Minimal time threshold for CF-end transmission, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalCfEndTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_MINIMAL_ETSI_FILLER_TIME_THRESHOLD 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalEtsiFillerTimeThreshold : 18; //Minimal time threshold for ETSI Filler transmission, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerMinimalEtsiFillerTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerSamplePostTxOfdmSifsComp : 4; //Time in micro seconds from "PHY-MAC Tx ready signal fall"---- till next Delia start, when the last Tx was in PHY mode of OFDM, reset value: 0x0, access type: RW
		uint32 txNavTimerSamplePostTx11BSifsComp : 4; //Time in micro seconds from "----PHY-MAC Tx ready signal fall"---- till next Delia start, when the last Tx was in PHY mode of 11B, reset value: 0x0, access type: RW
		uint32 txNavTimerSamplePostRxOfdmSifsComp : 4; //Time in micro seconds from "CCA 20p becomes free (after Rx session)" till next Delia start, when the last Rx was in PHY mode of OFDM, reset value: 0x0, access type: RW
		uint32 txNavTimerSamplePostRx11BSifsComp : 4; //Time in micro seconds from "CCA 20p becomes free (after Rx session)" till next Delia start, when the last Rx was in PHY mode of 11B, reset value: 0x0, access type: RW
		uint32 txNavTimerSamplePostTimeoutSifsComp : 4; //Time in micro seconds from "Response timeout expired"---- till next Delia start., reset value: 0x0, access type: RW
		uint32 txNavTimerSamplePostEtsiFillerSifsComp : 4; //Time in micro seconds from "----epsilon time at the last slot after ETSI filler"---- till next Delia start., reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0TxSequencerTxNavTimerSampleSifsComp_u;

/*REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TX 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSampleTimeDelayTxOfdm : 14; //Time delay in cycles from the PHY-MAC "Tx ready" signal fall (when the PHY mode of last Tx frame is OFDM) till when to sample CCA signals, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 ccaSampleTimeDelayTx11B : 14; //Time delay in cycles from the PHY-MAC "Tx ready" signal fall (when the PHY mode of last Tx frame is 11B) till when to sample CCA signals, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0TxSequencerCcaSampleTimeDelayTx_u;

/*REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_RX 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSampleTimeDelayRxOfdm : 14; //Time delay in cycles from "CCA 20p becomes free (after Rx session)"  (when the PHY mode of last Rx frame is OFDM) till when to sample CCA signals, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 ccaSampleTimeDelayRx11B : 14; //Time delay in cycles from "CCA 20p becomes free (after Rx session)"  (when the PHY mode of last Rx frame is 11B) till when to sample CCA signals, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB0TxSequencerCcaSampleTimeDelayRx_u;

/*REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TIMEOUT 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSampleTimeDelayTimeout : 14; //Time delay in cycles from timeout expired event till when to sample CCA signals, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TxSequencerCcaSampleTimeDelayTimeout_u;

/*REG_B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macPhyTxTxopSignalEn : 1; //Enable settings the MAC-PHY Tx TXOP signal, reset value: 0x1, access type: RW
		uint32 macPhyTxTxopSignalDebugOverrideEn : 1; //Enable override the MAC-PHY Tx TXOP signal, reset value: 0x0, access type: RW
		uint32 macPhyTxTxopSignalDebugOverrideVal : 1; //The override value of the MAC-PHY Tx TXOP signal, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TxSequencerMacPhyTxTxopSignalEn_u;

/*REG_B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macPhyTxTxopSignal : 1; //MAC-PHY Tx TXOP signal, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerMacPhyTxTxopSignal_u;

/*REG_B0_TX_SEQUENCER_BW_LIMITS 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBfRptMimoBw : 3; //BW limit based on BF report MIMO control, used only in case of SU, reset value: 0x0, access type: RO
		uint32 rxPhyStatusDynamicBwLimit : 3; //BW limit based on dynamic/static BW, reset value: 0x0, access type: RO
		uint32 lastGoodAggBwLimit : 3; //Current BW limit based on AGG builder activation, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB0TxSequencerBwLimits_u;

/*REG_B0_TX_SEQUENCER_SET_BW_LIMIT 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 setBwLimit : 3; //Set BW limit in the middle of TXOP, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TxSequencerSetBwLimit_u;

/*REG_B0_TX_SEQUENCER_SET_FILLER_RA_BF_RPT_BW_LOWER_EN 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 setFillerRaBfRptBwLowerEn : 1; //Enable settings Filler of rate adaptive on Rx BF report with BW lower than planned, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerSetFillerRaBfRptBwLowerEn_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 successRxBfRptIrq : 1; //Successful Rx BF report IRQ, reset value: 0x0, access type: RO
		uint32 successRxCtsIrq : 1; //Successful Rx CTS IRQ, reset value: 0x0, access type: RO
		uint32 successRxAckIrq : 1; //Successful Rx ACK IRQ, reset value: 0x0, access type: RO
		uint32 successRxBaIrq : 1; //Successful Rx BA IRQ, reset value: 0x0, access type: RO
		uint32 successRxAnyIrq : 1; //Successful Rx Any IRQ, reset value: 0x0, access type: RO
		uint32 successTxAutoRespIrq : 1; //Successful tx auto response IRQ, reset value: 0x0, access type: RO
		uint32 otherRxIrq : 1; //Other Rx IRQ, reset value: 0x0, access type: RO
		uint32 respTimeoutRxIrq : 1; //Response timeout Rx IRQ, reset value: 0x0, access type: RO
		uint32 postTxIrq : 1; //Post Tx IRQ, reset value: 0x0, access type: RO
		uint32 cca20PBusyIrq : 1; //Stop due CCA 20MHz primary busy IRQ, reset value: 0x0, access type: RO
		uint32 aggErrIrq : 1; //Stop due AGG builder error IRQ, reset value: 0x0, access type: RO
		uint32 noFillerSupportIrq : 1; //Stop due to no filler support IRQ, reset value: 0x0, access type: RO
		uint32 successRxBaBroadcastIrq : 1; //Successful Rx BA broadcast IRQ, reset value: 0x0, access type: RO
		uint32 successRxNdpFbIrq : 1; //Successful Rx NDP feedback IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TxSequencerTxSequencerIrqStatus_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 successRxBfRptIrqEn : 1; //Successful Rx BF report IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxCtsIrqEn : 1; //Successful Rx CTS IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxAckIrqEn : 1; //Successful Rx ACK IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxBaIrqEn : 1; //Successful Rx BA IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxAnyIrqEn : 1; //Successful Rx Any IRQ enable, reset value: 0x0, access type: RW
		uint32 successTxAutoRespIrqEn : 1; //Successful tx auto response IRQ enable, reset value: 0x0, access type: RW
		uint32 otherRxIrqEn : 1; //Other Rx IRQ enable, reset value: 0x0, access type: RW
		uint32 respTimeoutRxIrqEn : 1; //Response timeout Rx IRQ enable, reset value: 0x0, access type: RW
		uint32 postTxIrqEn : 1; //Post Tx IRQ enable, reset value: 0x0, access type: RW
		uint32 cca20PBusyIrqEn : 1; //Stop due CCA 20MHz primary busy IRQ enable, reset value: 0x0, access type: RW
		uint32 aggErrIrqEn : 1; //Stop due AGG builder error IRQ enable, reset value: 0x0, access type: RW
		uint32 noFillerSupportIrqEn : 1; //Stop due to no filler support IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxBaBroadcastIrqEn : 1; //Successful Rx BA broadcast IRQ enable, reset value: 0x0, access type: RW
		uint32 successRxNdpFbIrqEn : 1; //Successful Rx NDP feedback IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TxSequencerTxSequencerIrqEn_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 successRxBfRptIrqClr : 1; //Successful Rx BF report IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxCtsIrqClr : 1; //Successful Rx CTS IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxAckIrqClr : 1; //Successful Rx ACK IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxBaIrqClr : 1; //Successful Rx BA IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxAnyIrqClr : 1; //Successful Rx Any IRQ clear, reset value: 0x0, access type: WO
		uint32 successTxAutoRespIrqClr : 1; //Successful tx auto response IRQ clear, reset value: 0x0, access type: WO
		uint32 otherRxIrqClr : 1; //Other Rx IRQ clear, reset value: 0x0, access type: WO
		uint32 respTimeoutRxIrqClr : 1; //Response timeout Rx IRQ clear, reset value: 0x0, access type: WO
		uint32 postTxIrqClr : 1; //Post Tx IRQ clear, reset value: 0x0, access type: WO
		uint32 cca20PBusyIrqClr : 1; //Stop due CCA 20MHz primary busy IRQ clear, reset value: 0x0, access type: WO
		uint32 aggErrIrqClr : 1; //Stop due AGG builder error IRQ clear, reset value: 0x0, access type: WO
		uint32 noFillerSupportIrqClr : 1; //Stop due to no filler support IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxBaBroadcastIrqClr : 1; //Successful Rx BA broadcast IRQ clear, reset value: 0x0, access type: WO
		uint32 successRxNdpFbIrqClr : 1; //Successful Rx NDP feedback IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TxSequencerTxSequencerIrqClr_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compOwnerErrIrq : 1; //Compare owner bit failed IRQ, reset value: 0x0, access type: RO
		uint32 sequencerFifoFullDropIrq : 1; //Sequencer drop entry due to FIFO full IRQ, reset value: 0x0, access type: RO
		uint32 sequencerFifoDecrementLessThanZeroIrq : 1; //Sequencer FIFO amount has been decremented less than zero IRQ, reset value: 0x0, access type: RO
		uint32 baaNeedsStartWhileBusyIrq : 1; //Sequencer needed to activate BAA, but BAA is still busy IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerTxSequencerErrIrqStatus_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compOwnerErrIrqEn : 1; //Compare owner bit failed IRQ enable, reset value: 0x0, access type: RW
		uint32 sequencerFifoFullDropIrqEn : 1; //Sequencer drop entry due to FIFO full IRQ enable, reset value: 0x0, access type: RW
		uint32 sequencerFifoDecrementLessThanZeroIrqEn : 1; //Sequencer FIFO amount has been decremented less than zero IRQ enable, reset value: 0x0, access type: RW
		uint32 baaNeedsStartWhileBusyIrqEn : 1; //Sequencer needed to activate BAA, but BAA is still busy IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerTxSequencerErrIrqEn_u;

/*REG_B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 compOwnerErrIrqClr : 1; //Compare owner bit failed IRQ clear, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 1;
		uint32 sequencerFifoDecrementLessThanZeroIrqClr : 1; //Sequencer FIFO amount has been decremented less than zero IRQ clear, by write '1', reset value: 0x0, access type: WO
		uint32 baaNeedsStartWhileBusyIrqClr : 1; //Sequencer needed to activate BAA, but BAA is still busy IRQ clear, by write '1', reset value: 0x0, access type: WO
		uint32 reserved1 : 28;
	} bitFields;
} RegB0TxSequencerTxSequencerErrIrqClr_u;

/*REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuRaFillerIrq : 1; //CPU Rate adaptive filler IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerCpuRaFillerIrq_u;

/*REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_EN 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuRaFillerIrqEn : 1; //CPU Rate adaptive filler IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerCpuRaFillerIrqEn_u;

/*REG_B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_CLR 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuRaFillerIrqClr : 1; //CPU Rate adaptive filler IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerCpuRaFillerIrqClr_u;

/*REG_B0_TX_SEQUENCER_BF_RPT_VALID 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfRptSenderValid : 1; //BF report Sender valid bit, reset value: 0x0, access type: RO
		uint32 bfRptCpuValid : 1; //BF report CPU valid bit, reset value: 0x0, access type: RO
		uint32 bfRptSequencerValid : 1; //BF report Sequencer valid bit, reset value: 0x0, access type: RO
		uint32 bfRptHeMuOneshotSenderValid : 1; //BF report HE MU one-shot Sender valid bit, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerBfRptValid_u;

/*REG_B0_TX_SEQUENCER_BF_RPT_VALID_CLR 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfRptSenderValidClr : 1; //BF report Sender valid bit clear, reset value: 0x0, access type: WO
		uint32 bfRptCpuValidClr : 1; //BF report CPU valid bit clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 1;
		uint32 bfRptHeMuOneshotSenderValidClr : 1; //BF report HE MU one-shot Sender valid bit clear, reset value: 0x0, access type: WO
		uint32 reserved1 : 28;
	} bitFields;
} RegB0TxSequencerBfRptValidClr_u;

/*REG_B0_TX_SEQUENCER_AGG_REACT_PSDU_DUR_LIMIT_ADD 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aggReactPsduDurLimitAdd : 10; //Add time in [us] for the remaining PSDU duration limit in case of AGG builder reactivation, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0TxSequencerAggReactPsduDurLimitAdd_u;

/*REG_B0_TX_SEQUENCER_AGG_REACT_STATUS 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aggReactErrNoBuildPsdu : 1; //AGG reactivate error on PD type data - can't construct even one MPDU, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
		uint32 aggReactRequiredRaFiller : 1; //AGG reactivate required to set rate adaptive filler due to BF report, reset value: 0x0, access type: RO
		uint32 aggReactFirstPdNull : 1; //AGG reactivate stop due to first PD is NULL, reset value: 0x0, access type: RO
		uint32 aggReactLessMinTimer : 1; //AGG reactivate operation time is less than minimum , reset value: 0x0, access type: RO
		uint32 aggReactDone : 1; //AGG reactivation has been completed, reset value: 0x0, access type: RO
		uint32 aggReactUspPrimaryDrop : 1; //AGG reactivate stop due to Primary USP indicated as Drop, reset value: 0x0, access type: RO
		uint32 aggReactMuPrimaryNotDataPd : 1; //AGG reactivate stop due to MU Primary USP first PD is not data type, reset value: 0x0, access type: RO
		uint32 aggSeqReactOtfModeChange : 1; //AGG reactivate stop due to OTF mode change, reset value: 0x0, access type: RO
		uint32 reserved1 : 9;
	} bitFields;
} RegB0TxSequencerAggReactStatus_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_BASE_ADDR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerFifoBaseAddr : 22; //Sequencer FIFO base address (byte address), reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB0TxSequencerSequencerFifoBaseAddr_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_DEPTH_MINUS_ONE 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerFifoDepthMinusOne : 10; //Sequencer FIFO depth minus one (boundary of entries, each entry is 12 bytes), reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0TxSequencerSequencerFifoDepthMinusOne_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_CLEAR_STRB 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 sequencerFifoClearFullDropCtrStrb : 1; //Clear Sequencer fifo full drop counter, by write '1', reset value: 0x0, access type: WO
		uint32 sequencerFifoClearStrb : 1; //Clear Sequencer FIFO by write '1', reset value: 0x0, access type: WO
		uint32 reserved1 : 29;
	} bitFields;
} RegB0TxSequencerSequencerFifoClearStrb_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_RD_ENTRIES_NUM 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerFifoRdEntriesNum : 11; //Sequencer FIFO number of entries to decrement, reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB0TxSequencerSequencerFifoRdEntriesNum_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_NUM_ENTRIES_COUNT 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerFifoNumEntriesCount : 11; //Sequencer FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0TxSequencerSequencerFifoNumEntriesCount_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerFifoWrPtr : 10; //Sequencer FIFO write pointer entry index, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 sequencerFifoNotEmpty : 1; //Sequencer FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 sequencerFifoFull : 1; //Sequencer FIFO full indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 sequencerFifoFullDropCtr : 8; //Sequencer FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerSequencerFifoDebug_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_CCA_CONTROL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 forceDisable11BSampleCca : 1; //Force disable sample CCA signals in case the PHY mode is 11B by Sequencer, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerSequencerCcaControl_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_LOGGER 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerLoggerEn : 1; //Sequencer logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 sequencerLoggerPriority : 2; //Sequencer logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB0TxSequencerSequencerLogger_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_LOGGER_ACTIVE 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerLoggerActive : 1; //Sequencer logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerSequencerLoggerActive_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 seqMainSm : 7; //Sequencer main state machine, reset value: 0x0, access type: RO
		uint32 seqExpectedSm : 4; //Sequencer expected state machine, reset value: 0x0, access type: RO
		uint32 bfWaitSm : 2; //BF wait state machine, reset value: 0x0, access type: RO
		uint32 txselUnlockIfSm : 3; //Tx Selector unlock interface state machine, reset value: 0x0, access type: RO
		uint32 rdPrevStdSm : 3; //Read STD prev section state machine, reset value: 0x0, access type: RO
		uint32 etsiBoCountingSm : 3; //ETSI BO counting state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB0TxSequencerSequencerDebugSm_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_DEBUG 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 seqSenderConfig : 3; //Sequencer Sender config result: 0 - Success, 1 - timeout, 2 - Other Rx, 3 - Post Tx, 4 - Success response Tx, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 timeoutEventSticky : 4; //Timeout event sticky, 1 bit per USP, reset value: 0x0, access type: RO
		uint32 otherRxEventSticky : 4; //Other Rx event sticky, 1 bit per USP, reset value: 0x0, access type: RO
		uint32 bwChangeSticky : 1; //BW was changed during the sequence, sticky indication, reset value: 0x0, access type: RO
		uint32 aggSeqBfRptNssModifiedSticky : 1; //Nss was changed during the sequence due to BF report, sticky indication, reset value: 0x0, access type: RO
		uint32 aggSeqBfRptMcsModifiedSticky : 1; //MCS was changed during the sequence due to BF report, sticky indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 11;
	} bitFields;
} RegB0TxSequencerSequencerDebug_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_PLANNED_EXECUTED_DEBUG 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stdPlannedFlow : 16; //STD planned flow bits, read at the start of Tx sequencer operation, reset value: 0x0, access type: RO
		uint32 stdExecutedFlow : 16; //STD executed flow bits, read at the start of Tx sequencer operation, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerSequencerPlannedExecutedDebug_u;

/*REG_B0_TX_SEQUENCER_CCA_SAMPLE_TIMER 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaSampleTimer : 14; //CCA sample timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0TxSequencerCcaSampleTimer_u;

/*REG_B0_TX_SEQUENCER_SENDER_USP_INDEX 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderUspIndex : 2; //Sender configuration of USP index for the use of BAA and Sequencer , reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0TxSequencerSenderUspIndex_u;

/*REG_B0_TX_SEQUENCER_SENDER_CURRENT_TRANSMITTED_PLAN_FLOW 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderCurrentTransmittedPlanFlow : 16; //Sender configuration of which Tx plan bit has been currently transmitted, only 1bit should be set , reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerSenderCurrentTransmittedPlanFlow_u;

/*REG_B0_TX_SEQUENCER_MAX_NUM_MULT_USERS_TRAINING 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxNumMultUsersTraining : 4; //Maximum number of users during MU multiple users training (minus one), reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerMaxNumMultUsersTraining_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_REPORT_SENDER_BITS 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerReportSenderBits : 32; //Sender bits for Sequencer report, reset value: 0x0, access type: RW
	} bitFields;
} RegB0TxSequencerSequencerReportSenderBits_u;

/*REG_B0_TX_SEQUENCER_ABORT_WAIT_PHY_MULT_USERS_TRAINING 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 abortWaitPhyMultUsersTraining : 1; //Abort waiting PHY completse its operation on MU BF DB at the end of MU multiple users training sequence, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerAbortWaitPhyMultUsersTraining_u;

/*REG_B0_TX_SEQUENCER_PHY_SAMPLE_INFO_DEBUG_EN 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phySampleInfoDebugEn : 1; //Enable sample PHY Tx type and effective rate for debug, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerPhySampleInfoDebugEn_u;

/*REG_B0_TX_SEQUENCER_PHY_PAC_TX_PHY_MU_EFFECTIVE_RATE 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacTxPhyMuEffectiveRate : 16; //PHY Tx MU effective rate, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerPhyPacTxPhyMuEffectiveRate_u;

/*REG_B0_TX_SEQUENCER_PHY_PAC_TX_BF_31TO0 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacTxBf31To0 : 32; //PHY Tx BF of users [31:0], reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerPhyPacTxBf31To0_u;

/*REG_B0_TX_SEQUENCER_PHY_PAC_TX_BF_35TO32 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacTxBf35To32 : 4; //PHY Tx BF of users [35:32], reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerPhyPacTxBf35To32_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_END_IRQ 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerEndIrq : 1; //Sequencer end IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerSequencerEndIrq_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_END_IRQ_CLR 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerEndIrqClr : 1; //Sequencer end IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerSequencerEndIrqClr_u;

/*REG_B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE_SETTING 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAutoRespDisableSetting : 1; //Write 1 to disable transmit of auto-resp during the Tx sequence, , Write 0 to enable transmit of auto-resp during the Tx sequence., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerTxAutoRespDisableSetting_u;

/*REG_B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAutoRespDisable : 1; //Transmit of auto-resp disable indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerTxAutoRespDisable_u;

/*REG_B0_TX_SEQUENCER_BAA_NEEDED_SET_31TO0 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baaNeededSet31To0 : 32; //Set the BAA needed bits for users [31:0], 1 bit per USP, reset value: 0x0, access type: WO
	} bitFields;
} RegB0TxSequencerBaaNeededSet31To0_u;

/*REG_B0_TX_SEQUENCER_BAA_NEEDED_SET_35TO32 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baaNeededSet35To32 : 4; //Set the BAA needed bits for users [35:32], 1 bit per USP, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerBaaNeededSet35To32_u;

/*REG_B0_TX_SEQUENCER_BAA_NEEDED_31TO0 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baaNeeded31To0 : 32; //BAA needed bits for users [31:0], 1 bit per USP, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerBaaNeeded31To0_u;

/*REG_B0_TX_SEQUENCER_BAA_NEEDED_35TO32 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baaNeeded35To32 : 4; //BAA needed bits for users [35:32], 1 bit per USP, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxSequencerBaaNeeded35To32_u;

/*REG_B0_TX_SEQUENCER_MAX_NUM_BF_SEQ_ITERATIONS 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxNumBfSeqIterations : 2; //Maximum number of BF sequence iterations minus one, reset value: 0x3, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0TxSequencerMaxNumBfSeqIterations_u;

/*REG_B0_TX_SEQUENCER_TX_SEQ_DEL_RECIPE_POINTER 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSeqDelRecipePointer : 26; //Tx Sequencer to Delia recipe pointer (byte address), reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0TxSequencerTxSeqDelRecipePointer_u;

/*REG_B0_TX_SEQUENCER_BF_RPT_HE_MU_ONESHOT_PHASE 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfRptHeMuOneshotPhasePointer : 26; //Phase pointer latch on BF report in HE MU one-shot, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 bfRptHeMuOneshotPhaseIdx : 4; //Phase index latch on BF report in HE MU one-shot, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerBfRptHeMuOneshotPhase_u;

/*REG_B0_TX_SEQUENCER_TRIGGER_CF_END_BAA_BUSY_EN 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 triggerCfEndBaaBusyEn : 16; //Enable trigger CF-end when BAA is still busy, 1 bit per "DL Data type" enum in STD, reset value: 0x50, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerTriggerCfEndBaaBusyEn_u;

/*REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG1 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcSeqRxAck31To0 : 32; //RxC to sequencer Rx ACK result for users 0 till 31, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerRxcSeqRxResultDbg1_u;

/*REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG2 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcSeqRxBack31To0 : 32; //RxC to sequencer Rx BACK result for users 0 till 31, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TxSequencerRxcSeqRxResultDbg2_u;

/*REG_B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcSeqRxAck35To32 : 4; //RxC to sequencer Rx ACK result for users 32 till 35, reset value: 0x0, access type: RO
		uint32 rxcSeqRxBack35To32 : 4; //RxC to sequencer Rx BACK result for users 32 till 35, reset value: 0x0, access type: RO
		uint32 rxcSeqRxCts : 1; //RxC to sequencer Rx CTS result for user 0, reset value: 0x0, access type: RO
		uint32 rxcSeqRxAtLeastOneGoodMpdu : 1; //RxC to sequencer Rx result of at least one good MPDU, reset value: 0x0, access type: RO
		uint32 rxcSeqRxBaBroadcast : 1; //RxC success BACK non unicast MPDU, reset value: 0x0, access type: RO
		uint32 rxcSeqRxNdpFb : 1; //RxC success NDP feedback, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0TxSequencerRxcSeqRxResultDbg3_u;

/*REG_B0_TX_SEQUENCER_ETSI_MODE 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 etsiMode : 1; //Enable ETSI mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxSequencerEtsiMode_u;

/*REG_B0_TX_SEQUENCER_SENDER_CFG_ETSI_BO_COUNT 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderCfgEtsiBoCount : 16; //Sender to Sequencer, number of slots to count after ETSI filler, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerSenderCfgEtsiBoCount_u;

/*REG_B0_TX_SEQUENCER_ETSI_BO_COUNTER 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 etsiBoCounter : 16; //ETSI BO counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerEtsiBoCounter_u;

/*REG_B0_TX_SEQUENCER_ETSI_IFSN_ECW 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 etsiIfsnAcBe : 4; //ETSI IFSN of AC_BE, reset value: 0x3, access type: RW
		uint32 etsiIfsnAcBk : 4; //ETSI IFSN of AC_BK, reset value: 0x7, access type: RW
		uint32 etsiIfsnAcVi : 4; //ETSI IFSN of AC_VI, reset value: 0x1, access type: RW
		uint32 etsiIfsnAcVo : 4; //ETSI IFSN of AC_VO, reset value: 0x1, access type: RW
		uint32 etsiEcwAcBe : 4; //ETSI ECW of AC_BE, reset value: 0x4, access type: RW
		uint32 etsiEcwAcBk : 4; //ETSI ECW of AC_BK, reset value: 0x4, access type: RW
		uint32 etsiEcwAcVi : 4; //ETSI ECW of AC_VI, reset value: 0x3, access type: RW
		uint32 etsiEcwAcVo : 4; //ETSI ECW of AC_VO, reset value: 0x2, access type: RW
	} bitFields;
} RegB0TxSequencerEtsiIfsnEcw_u;

/*REG_B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_START 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 etsiBoRandomGenAcId : 2; //Write AC_ID, in order to calculate number of BO to count due to ETSI filler, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0TxSequencerEtsiBoRandomGenStart_u;

/*REG_B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_RESULT 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 etsiBoRandomGenResult : 16; //ETSI BO random generator result of number slots to count, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerEtsiBoRandomGenResult_u;

/*REG_B0_TX_SEQUENCER_LCG_RAND_FUNC_SEED 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcgRandFuncSeed : 32; //LCG random function seed, reset value: 0x12345678, access type: RW
	} bitFields;
} RegB0TxSequencerLcgRandFuncSeed_u;

/*REG_B0_TX_SEQUENCER_LCG_RAND_FUNC_SHIFT_NUM 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcgRandFuncShiftNum : 5; //LCG random function shift number, reset value: 0x10, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0TxSequencerLcgRandFuncShiftNum_u;

/*REG_B0_TX_SEQUENCER_ADDITION_ETSI_FILLER_TXOP_TIME_THRESHOLD 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 additionEtsiFillerTxopTimeThreshold : 18; //addition to minimal TXOP time threshold in case of setting filler is enabled, resolution [us]., reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegB0TxSequencerAdditionEtsiFillerTxopTimeThreshold_u;

/*REG_B0_TX_SEQUENCER_SEQUENCER_TRANSACTION_NUMBER 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sequencerTransactionNumber : 8; //Sequencer transaction number, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxSequencerSequencerTransactionNumber_u;

/*REG_B0_TX_SEQUENCER_SPARE_REGISTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegisters : 16; //spare registers, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxSequencerSpareRegisters_u;

//========================================
/* REG_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN 0x0 */
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_ACTIVATE_BAA_EN_MASK                                 0x00000001
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_VERIFY_CCA_PRIMARY_EN_MASK                           0x00000002
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_VERIFY_CCA_SECONDARY_EN_MASK                         0x00000004
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_UPDATE_DYNAMIC_BW_EN_MASK                            0x00000008
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_BF_ITERATION_EN_MASK                                 0x00000010
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_CLR_BF_REQ_TX_SEL_BITMAP_EN_MASK                     0x00000020
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_AGG_REACTIVATION_EN_MASK                             0x00000040
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_AGG_REACTIVATION_FORCE_MASK                          0x00000080
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_AGG_REACTIVATION_RD_DB_MASK                          0x00000100
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_AGG_VERIFY_SIFS_LIMIT_EN_MASK                        0x00000200
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_TRIGGER_TX_REQ_EN_MASK                               0x00000400
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_FILL_RATE_ADAPTIVE_RPT_EN_MASK                       0x00000800
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_TRIGGER_CF_END_FORCE_MASK                            0x00001000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_TRIGGER_CF_END_EN_MASK                               0x00002000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_TX_SEL_BITMAP_UNLOCK_EN_MASK                         0x00004000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_TXH_MAP_UNFREEZE_EN_MASK                             0x00008000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_GEN_CPU_INT_EN_MASK                                  0x00010000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_CLR_TX_NAV_TIMER_EN_MASK                             0x00020000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_VERIFY_MIN_SEQ_TIME_EN_MASK                          0x00040000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_WAIT_PHY_MULT_USERS_TRAINING_EN_MASK                 0x00080000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_MANAGE_BF_POLL_EN_MASK                               0x00100000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_ETSI_FILLER_SETTING_EN_MASK                          0x00200000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_ETSI_BO_COUNTING_EN_MASK                             0x00400000
#define B0_TX_SEQUENCER_SUCCESSFUL_SEQUENCE_EN_FIELD_SUCCESS_VERIFY_CCA_PRIMARY_POST_AGG_EN_MASK                  0x00800000
//========================================
/* REG_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN 0x4 */
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_ACTIVATE_BAA_EN_MASK                                     0x00000001
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_VERIFY_CCA_PRIMARY_EN_MASK                               0x00000002
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_VERIFY_CCA_SECONDARY_EN_MASK                             0x00000004
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_UPDATE_DYNAMIC_BW_EN_MASK                                0x00000008
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_BF_ITERATION_EN_MASK                                     0x00000010
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_CLR_BF_REQ_TX_SEL_BITMAP_EN_MASK                         0x00000020
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_AGG_REACTIVATION_EN_MASK                                 0x00000040
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_AGG_REACTIVATION_FORCE_MASK                              0x00000080
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_AGG_REACTIVATION_RD_DB_MASK                              0x00000100
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_AGG_VERIFY_SIFS_LIMIT_EN_MASK                            0x00000200
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_TRIGGER_TX_REQ_EN_MASK                                   0x00000400
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_FILL_RATE_ADAPTIVE_RPT_EN_MASK                           0x00000800
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_TRIGGER_CF_END_FORCE_MASK                                0x00001000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_TRIGGER_CF_END_EN_MASK                                   0x00002000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_TX_SEL_BITMAP_UNLOCK_EN_MASK                             0x00004000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_TXH_MAP_UNFREEZE_EN_MASK                                 0x00008000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_GEN_CPU_INT_EN_MASK                                      0x00010000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_CLR_TX_NAV_TIMER_EN_MASK                                 0x00020000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_VERIFY_MIN_SEQ_TIME_EN_MASK                              0x00040000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_WAIT_PHY_MULT_USERS_TRAINING_EN_MASK                     0x00080000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_MANAGE_BF_POLL_EN_MASK                                   0x00100000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_ETSI_FILLER_SETTING_EN_MASK                              0x00200000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_ETSI_BO_COUNTING_EN_MASK                                 0x00400000
#define B0_TX_SEQUENCER_OTHER_RX_SEQUENCE_EN_FIELD_OTHER_VERIFY_CCA_PRIMARY_POST_AGG_EN_MASK                      0x00800000
//========================================
/* REG_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN 0x8 */
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_ACTIVATE_BAA_EN_MASK                               0x00000001
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_VERIFY_CCA_PRIMARY_EN_MASK                         0x00000002
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_VERIFY_CCA_SECONDARY_EN_MASK                       0x00000004
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_UPDATE_DYNAMIC_BW_EN_MASK                          0x00000008
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_BF_ITERATION_EN_MASK                               0x00000010
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_CLR_BF_REQ_TX_SEL_BITMAP_EN_MASK                   0x00000020
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_AGG_REACTIVATION_EN_MASK                           0x00000040
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_AGG_REACTIVATION_FORCE_MASK                        0x00000080
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_AGG_REACTIVATION_RD_DB_MASK                        0x00000100
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_AGG_VERIFY_SIFS_LIMIT_EN_MASK                      0x00000200
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_TRIGGER_TX_REQ_EN_MASK                             0x00000400
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_FILL_RATE_ADAPTIVE_RPT_EN_MASK                     0x00000800
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_TRIGGER_CF_END_FORCE_MASK                          0x00001000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_TRIGGER_CF_END_EN_MASK                             0x00002000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_TX_SEL_BITMAP_UNLOCK_EN_MASK                       0x00004000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_TXH_MAP_UNFREEZE_EN_MASK                           0x00008000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_GEN_CPU_INT_EN_MASK                                0x00010000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_CLR_TX_NAV_TIMER_EN_MASK                           0x00020000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_VERIFY_MIN_SEQ_TIME_EN_MASK                        0x00040000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_WAIT_PHY_MULT_USERS_TRAINING_EN_MASK               0x00080000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_MANAGE_BF_POLL_EN_MASK                             0x00100000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_ETSI_FILLER_SETTING_EN_MASK                        0x00200000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_ETSI_BO_COUNTING_EN_MASK                           0x00400000
#define B0_TX_SEQUENCER_RESP_TIMEOUT_SEQUENCE_EN_FIELD_TIMEOUT_VERIFY_CCA_PRIMARY_POST_AGG_EN_MASK                0x00800000
//========================================
/* REG_TX_SEQUENCER_SEQUENCE_EXPECTED 0xC */
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_POST_TX_SEQ_ACTIVATE_START_MASK                                   0x00000001
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_POST_RX_SEQ_ACTIVATE_START_MASK                                   0x00000002
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_ACK_MASK                                              0x00000004
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_BA_MASK                                               0x00000008
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_CTS_MASK                                              0x00000010
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_BF_RPT_MASK                                           0x00000020
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_ANY_MASK                                              0x00000040
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_AUTO_RESP_AFTER_RX_MASK                                  0x00000080
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_NDP_FB_MASK                                           0x00000100
#define B0_TX_SEQUENCER_SEQUENCE_EXPECTED_FIELD_EXPECTED_RX_BA_BROADCAST_MASK                                     0x00000200
//========================================
/* REG_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN 0x10 */
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_CCA_20P_BUSY_TRIGGER_CF_END_EN_MASK                          0x00000001
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_CCA_20P_BUSY_TX_SEL_BITMAP_UNLOCK_EN_MASK                    0x00000002
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_CCA_20P_BUSY_TXH_MAP_UNFREEZE_EN_MASK                        0x00000004
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_CCA_20P_BUSY_GEN_CPU_INT_EN_MASK                             0x00000008
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_CCA_20P_BUSY_CLR_TX_NAV_TIMER_EN_MASK                        0x00000010
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_AGG_ERR_TRIGGER_CF_END_EN_MASK                               0x00000400
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_AGG_ERR_TX_SEL_BITMAP_UNLOCK_EN_MASK                         0x00000800
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_AGG_ERR_TXH_MAP_UNFREEZE_EN_MASK                             0x00001000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_AGG_ERR_GEN_CPU_INT_EN_MASK                                  0x00002000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_AGG_ERR_CLR_TX_NAV_TIMER_EN_MASK                             0x00004000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_NO_FILLER_TRIGGER_CF_END_EN_MASK                             0x00008000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_NO_FILLER_TX_SEL_BITMAP_UNLOCK_EN_MASK                       0x00010000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_NO_FILLER_TXH_MAP_UNFREEZE_EN_MASK                           0x00020000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_NO_FILLER_GEN_CPU_INT_EN_MASK                                0x00040000
#define B0_TX_SEQUENCER_TX_END_ERR_SEQUENCE_EN_FIELD_NO_FILLER_CLR_TX_NAV_TIMER_EN_MASK                           0x00080000
//========================================
/* REG_TX_SEQUENCER_MINIMAL_SEQ_TIME_THRESHOLD 0x14 */
#define B0_TX_SEQUENCER_MINIMAL_SEQ_TIME_THRESHOLD_FIELD_MINIMAL_SEQ_TIME_THRESHOLD_MASK                          0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_SU_TXOP_TIME_THRESHOLD 0x18 */
#define B0_TX_SEQUENCER_MINIMAL_SU_TXOP_TIME_THRESHOLD_FIELD_MINIMAL_SU_TXOP_TIME_THRESHOLD_MASK                  0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_VHT_MU_TXOP_TIME_THRESHOLD 0x1C */
#define B0_TX_SEQUENCER_MINIMAL_VHT_MU_TXOP_TIME_THRESHOLD_FIELD_MINIMAL_VHT_MU_TXOP_TIME_THRESHOLD_MASK          0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_HE_MU_TXOP_TIME_THRESHOLD 0x20 */
#define B0_TX_SEQUENCER_MINIMAL_HE_MU_TXOP_TIME_THRESHOLD_FIELD_MINIMAL_HE_MU_TXOP_TIME_THRESHOLD_MASK            0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_BF_SEQ_TIME_THRESHOLD 0x24 */
#define B0_TX_SEQUENCER_MINIMAL_BF_SEQ_TIME_THRESHOLD_FIELD_MINIMAL_BF_SEQ_TIME_THRESHOLD_MASK                    0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_CF_END_TIME_THRESHOLD 0x28 */
#define B0_TX_SEQUENCER_MINIMAL_CF_END_TIME_THRESHOLD_FIELD_MINIMAL_CF_END_TIME_THRESHOLD_MASK                    0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_MINIMAL_ETSI_FILLER_TIME_THRESHOLD 0x2C */
#define B0_TX_SEQUENCER_MINIMAL_ETSI_FILLER_TIME_THRESHOLD_FIELD_MINIMAL_ETSI_FILLER_TIME_THRESHOLD_MASK          0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP 0x30 */
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_TX_OFDM_SIFS_COMP_MASK       0x0000000F
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_TX_11B_SIFS_COMP_MASK        0x000000F0
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_RX_OFDM_SIFS_COMP_MASK       0x00000F00
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_RX_11B_SIFS_COMP_MASK        0x0000F000
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_TIMEOUT_SIFS_COMP_MASK       0x000F0000
#define B0_TX_SEQUENCER_TX_NAV_TIMER_SAMPLE_SIFS_COMP_FIELD_TX_NAV_TIMER_SAMPLE_POST_ETSI_FILLER_SIFS_COMP_MASK   0x00F00000
//========================================
/* REG_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TX 0x34 */
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TX_FIELD_CCA_SAMPLE_TIME_DELAY_TX_OFDM_MASK                         0x00003FFF
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TX_FIELD_CCA_SAMPLE_TIME_DELAY_TX_11B_MASK                          0x3FFF0000
//========================================
/* REG_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_RX 0x38 */
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_RX_FIELD_CCA_SAMPLE_TIME_DELAY_RX_OFDM_MASK                         0x00003FFF
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_RX_FIELD_CCA_SAMPLE_TIME_DELAY_RX_11B_MASK                          0x3FFF0000
//========================================
/* REG_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TIMEOUT 0x3C */
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIME_DELAY_TIMEOUT_FIELD_CCA_SAMPLE_TIME_DELAY_TIMEOUT_MASK                    0x00003FFF
//========================================
/* REG_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN 0x40 */
#define B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN_FIELD_MAC_PHY_TX_TXOP_SIGNAL_EN_MASK                            0x00000001
#define B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN_FIELD_MAC_PHY_TX_TXOP_SIGNAL_DEBUG_OVERRIDE_EN_MASK             0x00000002
#define B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_EN_FIELD_MAC_PHY_TX_TXOP_SIGNAL_DEBUG_OVERRIDE_VAL_MASK            0x00000004
//========================================
/* REG_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL 0x44 */
#define B0_TX_SEQUENCER_MAC_PHY_TX_TXOP_SIGNAL_FIELD_MAC_PHY_TX_TXOP_SIGNAL_MASK                                  0x00000001
//========================================
/* REG_TX_SEQUENCER_BW_LIMITS 0x48 */
#define B0_TX_SEQUENCER_BW_LIMITS_FIELD_RX_BF_RPT_MIMO_BW_MASK                                                    0x00000007
#define B0_TX_SEQUENCER_BW_LIMITS_FIELD_RX_PHY_STATUS_DYNAMIC_BW_LIMIT_MASK                                       0x00000038
#define B0_TX_SEQUENCER_BW_LIMITS_FIELD_LAST_GOOD_AGG_BW_LIMIT_MASK                                               0x000001C0
//========================================
/* REG_TX_SEQUENCER_SET_BW_LIMIT 0x4C */
#define B0_TX_SEQUENCER_SET_BW_LIMIT_FIELD_SET_BW_LIMIT_MASK                                                      0x00000007
//========================================
/* REG_TX_SEQUENCER_SET_FILLER_RA_BF_RPT_BW_LOWER_EN 0x50 */
#define B0_TX_SEQUENCER_SET_FILLER_RA_BF_RPT_BW_LOWER_EN_FIELD_SET_FILLER_RA_BF_RPT_BW_LOWER_EN_MASK              0x00000001
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS 0x54 */
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_BF_RPT_IRQ_MASK                                  0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_CTS_IRQ_MASK                                     0x00000002
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_ACK_IRQ_MASK                                     0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_BA_IRQ_MASK                                      0x00000008
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_ANY_IRQ_MASK                                     0x00000010
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_TX_AUTO_RESP_IRQ_MASK                               0x00000020
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_OTHER_RX_IRQ_MASK                                           0x00000040
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_RESP_TIMEOUT_RX_IRQ_MASK                                    0x00000080
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_POST_TX_IRQ_MASK                                            0x00000100
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_CCA_20P_BUSY_IRQ_MASK                                       0x00000200
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_AGG_ERR_IRQ_MASK                                            0x00000400
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_NO_FILLER_SUPPORT_IRQ_MASK                                  0x00000800
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_BA_BROADCAST_IRQ_MASK                            0x00001000
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_STATUS_FIELD_SUCCESS_RX_NDP_FB_IRQ_MASK                                  0x00002000
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN 0x58 */
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_BF_RPT_IRQ_EN_MASK                                   0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_CTS_IRQ_EN_MASK                                      0x00000002
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_ACK_IRQ_EN_MASK                                      0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_BA_IRQ_EN_MASK                                       0x00000008
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_ANY_IRQ_EN_MASK                                      0x00000010
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_TX_AUTO_RESP_IRQ_EN_MASK                                0x00000020
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_OTHER_RX_IRQ_EN_MASK                                            0x00000040
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_RESP_TIMEOUT_RX_IRQ_EN_MASK                                     0x00000080
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_POST_TX_IRQ_EN_MASK                                             0x00000100
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_CCA_20P_BUSY_IRQ_EN_MASK                                        0x00000200
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_AGG_ERR_IRQ_EN_MASK                                             0x00000400
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_NO_FILLER_SUPPORT_IRQ_EN_MASK                                   0x00000800
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_BA_BROADCAST_IRQ_EN_MASK                             0x00001000
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_EN_FIELD_SUCCESS_RX_NDP_FB_IRQ_EN_MASK                                   0x00002000
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR 0x5C */
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_BF_RPT_IRQ_CLR_MASK                                 0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_CTS_IRQ_CLR_MASK                                    0x00000002
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_ACK_IRQ_CLR_MASK                                    0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_BA_IRQ_CLR_MASK                                     0x00000008
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_ANY_IRQ_CLR_MASK                                    0x00000010
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_TX_AUTO_RESP_IRQ_CLR_MASK                              0x00000020
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_OTHER_RX_IRQ_CLR_MASK                                          0x00000040
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_RESP_TIMEOUT_RX_IRQ_CLR_MASK                                   0x00000080
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_POST_TX_IRQ_CLR_MASK                                           0x00000100
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_CCA_20P_BUSY_IRQ_CLR_MASK                                      0x00000200
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_AGG_ERR_IRQ_CLR_MASK                                           0x00000400
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_NO_FILLER_SUPPORT_IRQ_CLR_MASK                                 0x00000800
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_BA_BROADCAST_IRQ_CLR_MASK                           0x00001000
#define B0_TX_SEQUENCER_TX_SEQUENCER_IRQ_CLR_FIELD_SUCCESS_RX_NDP_FB_IRQ_CLR_MASK                                 0x00002000
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS 0x60 */
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS_FIELD_COMP_OWNER_ERR_IRQ_MASK                                 0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS_FIELD_SEQUENCER_FIFO_FULL_DROP_IRQ_MASK                       0x00000002
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS_FIELD_SEQUENCER_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_MASK        0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_STATUS_FIELD_BAA_NEEDS_START_WHILE_BUSY_IRQ_MASK                     0x00000008
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN 0x64 */
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN_FIELD_COMP_OWNER_ERR_IRQ_EN_MASK                                  0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN_FIELD_SEQUENCER_FIFO_FULL_DROP_IRQ_EN_MASK                        0x00000002
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN_FIELD_SEQUENCER_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_EN_MASK         0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_EN_FIELD_BAA_NEEDS_START_WHILE_BUSY_IRQ_EN_MASK                      0x00000008
//========================================
/* REG_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR 0x68 */
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR_FIELD_COMP_OWNER_ERR_IRQ_CLR_MASK                                0x00000001
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR_FIELD_SEQUENCER_FIFO_DECREMENT_LESS_THAN_ZERO_IRQ_CLR_MASK       0x00000004
#define B0_TX_SEQUENCER_TX_SEQUENCER_ERR_IRQ_CLR_FIELD_BAA_NEEDS_START_WHILE_BUSY_IRQ_CLR_MASK                    0x00000008
//========================================
/* REG_TX_SEQUENCER_CPU_RA_FILLER_IRQ 0x6C */
#define B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_FIELD_CPU_RA_FILLER_IRQ_MASK                                            0x00000001
//========================================
/* REG_TX_SEQUENCER_CPU_RA_FILLER_IRQ_EN 0x70 */
#define B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_EN_FIELD_CPU_RA_FILLER_IRQ_EN_MASK                                      0x00000001
//========================================
/* REG_TX_SEQUENCER_CPU_RA_FILLER_IRQ_CLR 0x74 */
#define B0_TX_SEQUENCER_CPU_RA_FILLER_IRQ_CLR_FIELD_CPU_RA_FILLER_IRQ_CLR_MASK                                    0x00000001
//========================================
/* REG_TX_SEQUENCER_BF_RPT_VALID 0x78 */
#define B0_TX_SEQUENCER_BF_RPT_VALID_FIELD_BF_RPT_SENDER_VALID_MASK                                               0x00000001
#define B0_TX_SEQUENCER_BF_RPT_VALID_FIELD_BF_RPT_CPU_VALID_MASK                                                  0x00000002
#define B0_TX_SEQUENCER_BF_RPT_VALID_FIELD_BF_RPT_SEQUENCER_VALID_MASK                                            0x00000004
#define B0_TX_SEQUENCER_BF_RPT_VALID_FIELD_BF_RPT_HE_MU_ONESHOT_SENDER_VALID_MASK                                 0x00000008
//========================================
/* REG_TX_SEQUENCER_BF_RPT_VALID_CLR 0x7C */
#define B0_TX_SEQUENCER_BF_RPT_VALID_CLR_FIELD_BF_RPT_SENDER_VALID_CLR_MASK                                       0x00000001
#define B0_TX_SEQUENCER_BF_RPT_VALID_CLR_FIELD_BF_RPT_CPU_VALID_CLR_MASK                                          0x00000002
#define B0_TX_SEQUENCER_BF_RPT_VALID_CLR_FIELD_BF_RPT_HE_MU_ONESHOT_SENDER_VALID_CLR_MASK                         0x00000008
//========================================
/* REG_TX_SEQUENCER_AGG_REACT_PSDU_DUR_LIMIT_ADD 0x80 */
#define B0_TX_SEQUENCER_AGG_REACT_PSDU_DUR_LIMIT_ADD_FIELD_AGG_REACT_PSDU_DUR_LIMIT_ADD_MASK                      0x000003FF
//========================================
/* REG_TX_SEQUENCER_AGG_REACT_STATUS 0x84 */
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_ERR_NO_BUILD_PSDU_MASK                                   0x00000001
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_REQUIRED_RA_FILLER_MASK                                  0x00010000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_FIRST_PD_NULL_MASK                                       0x00020000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_LESS_MIN_TIMER_MASK                                      0x00040000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_DONE_MASK                                                0x00080000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_USP_PRIMARY_DROP_MASK                                    0x00100000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_REACT_MU_PRIMARY_NOT_DATA_PD_MASK                              0x00200000
#define B0_TX_SEQUENCER_AGG_REACT_STATUS_FIELD_AGG_SEQ_REACT_OTF_MODE_CHANGE_MASK                                 0x00400000
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_BASE_ADDR 0x88 */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_BASE_ADDR_FIELD_SEQUENCER_FIFO_BASE_ADDR_MASK                              0x003FFFFF
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_DEPTH_MINUS_ONE 0x8C */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_DEPTH_MINUS_ONE_FIELD_SEQUENCER_FIFO_DEPTH_MINUS_ONE_MASK                  0x000003FF
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_CLEAR_STRB 0x90 */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_CLEAR_STRB_FIELD_SEQUENCER_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK              0x00000002
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_CLEAR_STRB_FIELD_SEQUENCER_FIFO_CLEAR_STRB_MASK                            0x00000004
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_RD_ENTRIES_NUM 0x94 */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_RD_ENTRIES_NUM_FIELD_SEQUENCER_FIFO_RD_ENTRIES_NUM_MASK                    0x000007FF
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_NUM_ENTRIES_COUNT 0x98 */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_NUM_ENTRIES_COUNT_FIELD_SEQUENCER_FIFO_NUM_ENTRIES_COUNT_MASK              0x000007FF
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG 0x9C */
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG_FIELD_SEQUENCER_FIFO_WR_PTR_MASK                                     0x000003FF
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG_FIELD_SEQUENCER_FIFO_NOT_EMPTY_MASK                                  0x00010000
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG_FIELD_SEQUENCER_FIFO_FULL_MASK                                       0x00020000
#define B0_TX_SEQUENCER_SEQUENCER_FIFO_DEBUG_FIELD_SEQUENCER_FIFO_FULL_DROP_CTR_MASK                              0xFF000000
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_CCA_CONTROL 0xA0 */
#define B0_TX_SEQUENCER_SEQUENCER_CCA_CONTROL_FIELD_FORCE_DISABLE_11B_SAMPLE_CCA_MASK                             0x00000001
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_LOGGER 0xA4 */
#define B0_TX_SEQUENCER_SEQUENCER_LOGGER_FIELD_SEQUENCER_LOGGER_EN_MASK                                           0x00000001
#define B0_TX_SEQUENCER_SEQUENCER_LOGGER_FIELD_SEQUENCER_LOGGER_PRIORITY_MASK                                     0x00000300
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_LOGGER_ACTIVE 0xA8 */
#define B0_TX_SEQUENCER_SEQUENCER_LOGGER_ACTIVE_FIELD_SEQUENCER_LOGGER_ACTIVE_MASK                                0x00000001
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_DEBUG_SM 0xAC */
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_SEQ_MAIN_SM_MASK                                                 0x0000007F
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_SEQ_EXPECTED_SM_MASK                                             0x00000780
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_BF_WAIT_SM_MASK                                                  0x00001800
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_TXSEL_UNLOCK_IF_SM_MASK                                          0x0000E000
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_RD_PREV_STD_SM_MASK                                              0x00070000
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_SM_FIELD_ETSI_BO_COUNTING_SM_MASK                                         0x00380000
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_DEBUG 0xB0 */
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_SEQ_SENDER_CONFIG_MASK                                              0x00000007
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_TIMEOUT_EVENT_STICKY_MASK                                           0x00003C00
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_OTHER_RX_EVENT_STICKY_MASK                                          0x0003C000
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_BW_CHANGE_STICKY_MASK                                               0x00040000
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_AGG_SEQ_BF_RPT_NSS_MODIFIED_STICKY_MASK                             0x00080000
#define B0_TX_SEQUENCER_SEQUENCER_DEBUG_FIELD_AGG_SEQ_BF_RPT_MCS_MODIFIED_STICKY_MASK                             0x00100000
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_PLANNED_EXECUTED_DEBUG 0xB4 */
#define B0_TX_SEQUENCER_SEQUENCER_PLANNED_EXECUTED_DEBUG_FIELD_STD_PLANNED_FLOW_MASK                              0x0000FFFF
#define B0_TX_SEQUENCER_SEQUENCER_PLANNED_EXECUTED_DEBUG_FIELD_STD_EXECUTED_FLOW_MASK                             0xFFFF0000
//========================================
/* REG_TX_SEQUENCER_CCA_SAMPLE_TIMER 0xB8 */
#define B0_TX_SEQUENCER_CCA_SAMPLE_TIMER_FIELD_CCA_SAMPLE_TIMER_MASK                                              0x00003FFF
//========================================
/* REG_TX_SEQUENCER_SENDER_USP_INDEX 0xBC */
#define B0_TX_SEQUENCER_SENDER_USP_INDEX_FIELD_SENDER_USP_INDEX_MASK                                              0x00000003
//========================================
/* REG_TX_SEQUENCER_SENDER_CURRENT_TRANSMITTED_PLAN_FLOW 0xC0 */
#define B0_TX_SEQUENCER_SENDER_CURRENT_TRANSMITTED_PLAN_FLOW_FIELD_SENDER_CURRENT_TRANSMITTED_PLAN_FLOW_MASK      0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_MAX_NUM_MULT_USERS_TRAINING 0xC4 */
#define B0_TX_SEQUENCER_MAX_NUM_MULT_USERS_TRAINING_FIELD_MAX_NUM_MULT_USERS_TRAINING_MASK                        0x0000000F
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_REPORT_SENDER_BITS 0xC8 */
#define B0_TX_SEQUENCER_SEQUENCER_REPORT_SENDER_BITS_FIELD_SEQUENCER_REPORT_SENDER_BITS_MASK                      0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_ABORT_WAIT_PHY_MULT_USERS_TRAINING 0xCC */
#define B0_TX_SEQUENCER_ABORT_WAIT_PHY_MULT_USERS_TRAINING_FIELD_ABORT_WAIT_PHY_MULT_USERS_TRAINING_MASK          0x00000001
//========================================
/* REG_TX_SEQUENCER_PHY_SAMPLE_INFO_DEBUG_EN 0xD0 */
#define B0_TX_SEQUENCER_PHY_SAMPLE_INFO_DEBUG_EN_FIELD_PHY_SAMPLE_INFO_DEBUG_EN_MASK                              0x00000001
//========================================
/* REG_TX_SEQUENCER_PHY_PAC_TX_PHY_MU_EFFECTIVE_RATE 0xD4 */
#define B0_TX_SEQUENCER_PHY_PAC_TX_PHY_MU_EFFECTIVE_RATE_FIELD_PHY_PAC_TX_PHY_MU_EFFECTIVE_RATE_MASK              0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_PHY_PAC_TX_BF_31TO0 0xD8 */
#define B0_TX_SEQUENCER_PHY_PAC_TX_BF_31TO0_FIELD_PHY_PAC_TX_BF_31TO0_MASK                                        0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_PHY_PAC_TX_BF_35TO32 0xDC */
#define B0_TX_SEQUENCER_PHY_PAC_TX_BF_35TO32_FIELD_PHY_PAC_TX_BF_35TO32_MASK                                      0x0000000F
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_END_IRQ 0xE0 */
#define B0_TX_SEQUENCER_SEQUENCER_END_IRQ_FIELD_SEQUENCER_END_IRQ_MASK                                            0x00000001
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_END_IRQ_CLR 0xE4 */
#define B0_TX_SEQUENCER_SEQUENCER_END_IRQ_CLR_FIELD_SEQUENCER_END_IRQ_CLR_MASK                                    0x00000001
//========================================
/* REG_TX_SEQUENCER_TX_AUTO_RESP_DISABLE_SETTING 0xE8 */
#define B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE_SETTING_FIELD_TX_AUTO_RESP_DISABLE_SETTING_MASK                      0x00000001
//========================================
/* REG_TX_SEQUENCER_TX_AUTO_RESP_DISABLE 0xEC */
#define B0_TX_SEQUENCER_TX_AUTO_RESP_DISABLE_FIELD_TX_AUTO_RESP_DISABLE_MASK                                      0x00000001
//========================================
/* REG_TX_SEQUENCER_BAA_NEEDED_SET_31TO0 0xF0 */
#define B0_TX_SEQUENCER_BAA_NEEDED_SET_31TO0_FIELD_BAA_NEEDED_SET_31TO0_MASK                                      0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_BAA_NEEDED_SET_35TO32 0xF4 */
#define B0_TX_SEQUENCER_BAA_NEEDED_SET_35TO32_FIELD_BAA_NEEDED_SET_35TO32_MASK                                    0x0000000F
//========================================
/* REG_TX_SEQUENCER_BAA_NEEDED_31TO0 0xF8 */
#define B0_TX_SEQUENCER_BAA_NEEDED_31TO0_FIELD_BAA_NEEDED_31TO0_MASK                                              0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_BAA_NEEDED_35TO32 0xFC */
#define B0_TX_SEQUENCER_BAA_NEEDED_35TO32_FIELD_BAA_NEEDED_35TO32_MASK                                            0x0000000F
//========================================
/* REG_TX_SEQUENCER_MAX_NUM_BF_SEQ_ITERATIONS 0x100 */
#define B0_TX_SEQUENCER_MAX_NUM_BF_SEQ_ITERATIONS_FIELD_MAX_NUM_BF_SEQ_ITERATIONS_MASK                            0x00000003
//========================================
/* REG_TX_SEQUENCER_TX_SEQ_DEL_RECIPE_POINTER 0x104 */
#define B0_TX_SEQUENCER_TX_SEQ_DEL_RECIPE_POINTER_FIELD_TX_SEQ_DEL_RECIPE_POINTER_MASK                            0x03FFFFFF
//========================================
/* REG_TX_SEQUENCER_BF_RPT_HE_MU_ONESHOT_PHASE 0x10C */
#define B0_TX_SEQUENCER_BF_RPT_HE_MU_ONESHOT_PHASE_FIELD_BF_RPT_HE_MU_ONESHOT_PHASE_POINTER_MASK                  0x03FFFFFF
#define B0_TX_SEQUENCER_BF_RPT_HE_MU_ONESHOT_PHASE_FIELD_BF_RPT_HE_MU_ONESHOT_PHASE_IDX_MASK                      0xF0000000
//========================================
/* REG_TX_SEQUENCER_TRIGGER_CF_END_BAA_BUSY_EN 0x110 */
#define B0_TX_SEQUENCER_TRIGGER_CF_END_BAA_BUSY_EN_FIELD_TRIGGER_CF_END_BAA_BUSY_EN_MASK                          0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG1 0x114 */
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG1_FIELD_RXC_SEQ_RX_ACK_31TO0_MASK                                    0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG2 0x118 */
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG2_FIELD_RXC_SEQ_RX_BACK_31TO0_MASK                                   0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3 0x11C */
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_ACK_35TO32_MASK                                   0x0000000F
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_BACK_35TO32_MASK                                  0x000000F0
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_CTS_MASK                                          0x00000100
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_AT_LEAST_ONE_GOOD_MPDU_MASK                       0x00000200
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_BA_BROADCAST_MASK                                 0x00000400
#define B0_TX_SEQUENCER_RXC_SEQ_RX_RESULT_DBG3_FIELD_RXC_SEQ_RX_NDP_FB_MASK                                       0x00000800
//========================================
/* REG_TX_SEQUENCER_ETSI_MODE 0x120 */
#define B0_TX_SEQUENCER_ETSI_MODE_FIELD_ETSI_MODE_MASK                                                            0x00000001
//========================================
/* REG_TX_SEQUENCER_SENDER_CFG_ETSI_BO_COUNT 0x124 */
#define B0_TX_SEQUENCER_SENDER_CFG_ETSI_BO_COUNT_FIELD_SENDER_CFG_ETSI_BO_COUNT_MASK                              0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_ETSI_BO_COUNTER 0x128 */
#define B0_TX_SEQUENCER_ETSI_BO_COUNTER_FIELD_ETSI_BO_COUNTER_MASK                                                0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_ETSI_IFSN_ECW 0x12C */
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_IFSN_AC_BE_MASK                                                  0x0000000F
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_IFSN_AC_BK_MASK                                                  0x000000F0
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_IFSN_AC_VI_MASK                                                  0x00000F00
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_IFSN_AC_VO_MASK                                                  0x0000F000
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_ECW_AC_BE_MASK                                                   0x000F0000
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_ECW_AC_BK_MASK                                                   0x00F00000
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_ECW_AC_VI_MASK                                                   0x0F000000
#define B0_TX_SEQUENCER_ETSI_IFSN_ECW_FIELD_ETSI_ECW_AC_VO_MASK                                                   0xF0000000
//========================================
/* REG_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_START 0x130 */
#define B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_START_FIELD_ETSI_BO_RANDOM_GEN_AC_ID_MASK                              0x00000003
//========================================
/* REG_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_RESULT 0x134 */
#define B0_TX_SEQUENCER_ETSI_BO_RANDOM_GEN_RESULT_FIELD_ETSI_BO_RANDOM_GEN_RESULT_MASK                            0x0000FFFF
//========================================
/* REG_TX_SEQUENCER_LCG_RAND_FUNC_SEED 0x138 */
#define B0_TX_SEQUENCER_LCG_RAND_FUNC_SEED_FIELD_LCG_RAND_FUNC_SEED_MASK                                          0xFFFFFFFF
//========================================
/* REG_TX_SEQUENCER_LCG_RAND_FUNC_SHIFT_NUM 0x13C */
#define B0_TX_SEQUENCER_LCG_RAND_FUNC_SHIFT_NUM_FIELD_LCG_RAND_FUNC_SHIFT_NUM_MASK                                0x0000001F
//========================================
/* REG_TX_SEQUENCER_ADDITION_ETSI_FILLER_TXOP_TIME_THRESHOLD 0x140 */
#define B0_TX_SEQUENCER_ADDITION_ETSI_FILLER_TXOP_TIME_THRESHOLD_FIELD_ADDITION_ETSI_FILLER_TXOP_TIME_THRESHOLD_MASK 0x0003FFFF
//========================================
/* REG_TX_SEQUENCER_SEQUENCER_TRANSACTION_NUMBER 0x144 */
#define B0_TX_SEQUENCER_SEQUENCER_TRANSACTION_NUMBER_FIELD_SEQUENCER_TRANSACTION_NUMBER_MASK                      0x000000FF
//========================================
/* REG_TX_SEQUENCER_SPARE_REGISTERS 0x1FC */
#define B0_TX_SEQUENCER_SPARE_REGISTERS_FIELD_SPARE_REGISTERS_MASK                                                0x0000FFFF


#endif // _TX_SEQUENCER_REGS_H_
