
/***********************************************************************************
File:				BaAnalyzerRegs.h
Module:				baAnalyzer
SOC Revision:		latest
Generated at:       2024-06-26 12:55:12
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _BA_ANALYZER_REGS_H_
#define _BA_ANALYZER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define BA_ANALYZER_BASE_ADDRESS                                 MEMORY_MAP_UNIT_45_BASE_ADDRESS
#define	REG_BA_ANALYZER_MODE                                       (BA_ANALYZER_BASE_ADDRESS + 0x0)
#define	REG_BA_ANALYZER_INT_ERROR_STATUS                           (BA_ANALYZER_BASE_ADDRESS + 0x4)
#define	REG_BA_ANALYZER_INT_ERROR_EN                               (BA_ANALYZER_BASE_ADDRESS + 0x8)
#define	REG_BA_ANALYZER_CLEAR_INT_ERROR                            (BA_ANALYZER_BASE_ADDRESS + 0xC)
#define	REG_BA_ANALYZER_INT_STATUS                                 (BA_ANALYZER_BASE_ADDRESS + 0x10)
#define	REG_BA_ANALYZER_INT_EN                                     (BA_ANALYZER_BASE_ADDRESS + 0x14)
#define	REG_BA_ANALYZER_CLEAR_INT                                  (BA_ANALYZER_BASE_ADDRESS + 0x18)
#define	REG_BA_ANALYZER_SENDER_REG                                 (BA_ANALYZER_BASE_ADDRESS + 0x1C)
#define	REG_BA_ANALYZER_SENDER_REG_SEL                             (BA_ANALYZER_BASE_ADDRESS + 0x20)
#define	REG_BA_ANALYZER_DPL_IDX                                    (BA_ANALYZER_BASE_ADDRESS + 0x24)
#define	REG_BA_ANALYZER_TXH_PSDU_LENGHT_THR                        (BA_ANALYZER_BASE_ADDRESS + 0x28)
#define	REG_BA_ANALYZER_SENDER_TX_STATUS_WORD0                     (BA_ANALYZER_BASE_ADDRESS + 0x2C)
#define	REG_BA_ANALYZER_SENDER_TX_STATUS_WORD1                     (BA_ANALYZER_BASE_ADDRESS + 0x30)
#define	REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0              (BA_ANALYZER_BASE_ADDRESS + 0x34)
#define	REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1              (BA_ANALYZER_BASE_ADDRESS + 0x38)
#define	REG_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN                    (BA_ANALYZER_BASE_ADDRESS + 0x3C)
#define	REG_BA_ANALYZER_MBA_CFG                                    (BA_ANALYZER_BASE_ADDRESS + 0x40)
#define	REG_BA_ANALYZER_CBA_CFG                                    (BA_ANALYZER_BASE_ADDRESS + 0x44)
#define	REG_BA_ANALYZER_TRAFFIC_IND_CFG                            (BA_ANALYZER_BASE_ADDRESS + 0x48)
#define	REG_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP                    (BA_ANALYZER_BASE_ADDRESS + 0x4C)
#define	REG_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP                (BA_ANALYZER_BASE_ADDRESS + 0x50)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR                  (BA_ANALYZER_BASE_ADDRESS + 0x54)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE            (BA_ANALYZER_BASE_ADDRESS + 0x58)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB                 (BA_ANALYZER_BASE_ADDRESS + 0x5C)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM             (BA_ANALYZER_BASE_ADDRESS + 0x60)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT          (BA_ANALYZER_BASE_ADDRESS + 0x64)
#define	REG_BA_ANALYZER_STS_REPORT_FIFO_DEBUG                      (BA_ANALYZER_BASE_ADDRESS + 0x68)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR            (BA_ANALYZER_BASE_ADDRESS + 0x6C)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE      (BA_ANALYZER_BASE_ADDRESS + 0x70)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB           (BA_ANALYZER_BASE_ADDRESS + 0x74)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM       (BA_ANALYZER_BASE_ADDRESS + 0x78)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT    (BA_ANALYZER_BASE_ADDRESS + 0x7C)
#define	REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG                (BA_ANALYZER_BASE_ADDRESS + 0x80)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR              (BA_ANALYZER_BASE_ADDRESS + 0x84)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE        (BA_ANALYZER_BASE_ADDRESS + 0x88)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB             (BA_ANALYZER_BASE_ADDRESS + 0x8C)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM         (BA_ANALYZER_BASE_ADDRESS + 0x90)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT      (BA_ANALYZER_BASE_ADDRESS + 0x94)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0                 (BA_ANALYZER_BASE_ADDRESS + 0x98)
#define	REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1                 (BA_ANALYZER_BASE_ADDRESS + 0x9C)
#define	REG_BA_ANALYZER_RTS_CFG                                    (BA_ANALYZER_BASE_ADDRESS + 0xA0)
#define	REG_BA_ANALYZER_ETH_TYPE_OVERHEAD                          (BA_ANALYZER_BASE_ADDRESS + 0xA4)
#define	REG_BA_ANALYZER_STA_MODE_CFG0                              (BA_ANALYZER_BASE_ADDRESS + 0xA8)
#define	REG_BA_ANALYZER_STA_MODE_CFG1                              (BA_ANALYZER_BASE_ADDRESS + 0xAC)
#define	REG_BA_ANALYZER_FIFO_STATUS0                               (BA_ANALYZER_BASE_ADDRESS + 0xB0)
#define	REG_BA_ANALYZER_FIFO_STATUS1                               (BA_ANALYZER_BASE_ADDRESS + 0xB4)
#define	REG_BA_ANALYZER_FSM_STATUS0                                (BA_ANALYZER_BASE_ADDRESS + 0xB8)
#define	REG_BA_ANALYZER_FSM_STATUS1                                (BA_ANALYZER_BASE_ADDRESS + 0xBC)
#define	REG_BA_ANALYZER_LOGGER_REG                                 (BA_ANALYZER_BASE_ADDRESS + 0xC0)
#define	REG_BA_ANALYZER_LOGGER_FILTER0                             (BA_ANALYZER_BASE_ADDRESS + 0xC4)
#define	REG_BA_ANALYZER_LOGGER_FILTER1                             (BA_ANALYZER_BASE_ADDRESS + 0xC8)
#define	REG_BA_ANALYZER_LOGGER_STATISTICS_REG                      (BA_ANALYZER_BASE_ADDRESS + 0xCC)
#define	REG_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS                (BA_ANALYZER_BASE_ADDRESS + 0xD0)
#define	REG_BA_ANALYZER_BA_ANALYZER_SPARE                          (BA_ANALYZER_BASE_ADDRESS + 0xE0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_BA_ANALYZER_MODE 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crUpdateBw : 1; //Block window update enable , reset value: 0x1, access type: RW
		uint32 crBlockEn : 1; //BAA block enable , reset value: 0x1, access type: RW
		uint32 crUpdatePsCntPerFrag : 1; //Update PS counter per (successful/discarded) fragment MPDU. , '0' - only on last fragment. , '1' - per each fragment., reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerMode_u;

/*REG_BA_ANALYZER_INT_ERROR_STATUS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srUnexpectedTidInt : 1; //unexpected TID, set when received BA on an unexpected response TID., reset value: 0x0, access type: RO
		uint32 srUnexpectedAckType0Tid15Int : 1; //unexpected M-BA ack type 0 on TID 15., reset value: 0x0, access type: RO
		uint32 srUnexpectedFragNumInt : 1; //unexpected fragment number field within BA frame., reset value: 0x0, access type: RO
		uint32 srUnexpectedBaLengthInt : 1; //BA length is wrong, ended unexpectedly in the middle of a subfield format., reset value: 0x0, access type: RO
		uint32 srUnexpectedStaModeMpduLengthInt : 1; //Broadcast MBA length is wrong., reset value: 0x0, access type: RO
		uint32 srUnexpectedStaModeRepeatedAidTidInt : 1; //My AID in combination with TID repeated more than once in broadcast MBA., reset value: 0x0, access type: RO
		uint32 srDropTxStWhileFullInt : 1; //Tx status report dropped because FIFO was full., reset value: 0x0, access type: RO
		uint32 srDropCommonRtAdaptWhileFullInt : 1; //Rate adaptive common report dropped because FIFO was full., reset value: 0x0, access type: RO
		uint32 srDropFirstUserRtAdaptWhileFullInt : 1; //Rate adaptive first user report dropped because FIFO was full., reset value: 0x0, access type: RO
		uint32 srDropNonFirstUserRtAdaptWhileFullInt : 1; //Rate adaptive non first user report dropped because FIFO was full., reset value: 0x0, access type: RO
		uint32 srTxStUnderflowInt : 1; //Tx status report FIFO underflow indication., reset value: 0x0, access type: RO
		uint32 srCommonRtAdaptUnderflowInt : 1; //Rate adaptive common report FIFO underflow indication., reset value: 0x0, access type: RO
		uint32 srUserRtAdaptUnderflowInt : 1; //Rate adaptive user report FIFO underflow indication., reset value: 0x0, access type: RO
		uint32 srCalcCntSetToZeroInt : 1; //A calculated counter set to zero due to negative value result. , Refer to calc_cnt_set_to_zero_status register in order to identify the counter caused this interrupt to assert., reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegBaAnalyzerIntErrorStatus_u;

/*REG_BA_ANALYZER_INT_ERROR_EN 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crUnexpectedTidIntEn : 1; //Enable unexpected_tid_int., reset value: 0x1, access type: RW
		uint32 crUnexpectedAckType0Tid15IntEn : 1; //Enable unexpected_ack_type0_tid15_int., reset value: 0x1, access type: RW
		uint32 crUnexpectedFragNumIntEn : 1; //Enable unexpected_frag_num_int., reset value: 0x1, access type: RW
		uint32 crUnexpectedBaLengthIntEn : 1; //Enable unexpected_ba_length_int., reset value: 0x1, access type: RW
		uint32 crUnexpectedStaModeMpduLengthIntEn : 1; //Enable unexpected_sta_mode_mpdu_length_int., reset value: 0x1, access type: RW
		uint32 crUnexpectedStaModeRepeatedAidTidIntEn : 1; //Enable unexpected_sta_mode_repeated_aid_tid_int., reset value: 0x1, access type: RW
		uint32 crDropTxStWhileFullIntEn : 1; //Enable drop_tx_st_while_full_int., reset value: 0x1, access type: RW
		uint32 crDropCommonRtAdaptWhileFullIntEn : 1; //enable drop_common_rt_adapt_while_full_int., reset value: 0x1, access type: RW
		uint32 crDropFirstUserRtAdaptWhileFullIntEn : 1; //enable drop_first_user_rt_adapt_while_full_int., reset value: 0x1, access type: RW
		uint32 crDropNonFirstUserRtAdaptWhileFullIntEn : 1; //enable drop_non_first_user_rt_adapt_while_full_int., reset value: 0x1, access type: RW
		uint32 crTxStUnderflowIntEn : 1; //Enable tx_st_underflow_int., reset value: 0x1, access type: RW
		uint32 crCommonRtAdaptUnderflowIntEn : 1; //Enable common_rt_adapt_underflow_int., reset value: 0x1, access type: RW
		uint32 crUserRtAdaptUnderflowIntEn : 1; //Enable user_rt_adapt_underflow_int., reset value: 0x1, access type: RW
		uint32 crCalcCntSetToZeroIntEn : 1; //Enable calc_cnt_set_to_zero_int., reset value: 0x1, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegBaAnalyzerIntErrorEn_u;

/*REG_BA_ANALYZER_CLEAR_INT_ERROR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crClrUnexpectedTidInt : 1; //Clear unexpected_tid_int., reset value: 0x0, access type: WO
		uint32 crClrUnexpectedAckType0Tid15Int : 1; //clear unexpected_ack_type0_tid15_int., reset value: 0x0, access type: WO
		uint32 crClrUnexpectedFragNumInt : 1; //Clear unexpected_frag_num_int., reset value: 0x0, access type: WO
		uint32 crClrUnexpectedBaLengthInt : 1; //Clear unexpected_ba_length_int., reset value: 0x0, access type: WO
		uint32 crClrUnexpectedStaModeMpduLengthInt : 1; //Clear unexpected_sta_mode_mpdu_length_int., reset value: 0x0, access type: WO
		uint32 crClrUnexpectedStaModeRepeatedAidTidInt : 1; //Clear unexpected_sta_mode_repeated_aid_tid_int., reset value: 0x0, access type: WO
		uint32 reserved0 : 7;
		uint32 crClrCalcCntSetToZeroInt : 1; //Clear calc_cnt_set_to_zero_int., reset value: 0x0, access type: WO
		uint32 reserved1 : 18;
	} bitFields;
} RegBaAnalyzerClearIntError_u;

/*REG_BA_ANALYZER_INT_STATUS 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srTxStNotEmpty : 1; //umac_tx_st_not_empty, reset value: 0x0, access type: RO
		uint32 srCommonRateAdaptiveFifoNotEmpty : 1; //common_rate_adaptive_fifo_not_empty, reset value: 0x0, access type: RO
		uint32 srUserRateAdaptiveFifoNotEmpty : 1; //common_rate_adaptive_fifo_not_empty, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerIntStatus_u;

/*REG_BA_ANALYZER_INT_EN 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crTxStNotEmptyEn : 1; //umac_tx_st_not_empty_en, reset value: 0x1, access type: RW
		uint32 crCommonRateAdaptiveFifoNotEmptyEn : 1; //common_rate_adaptive_fifo_not_empty_en, reset value: 0x0, access type: RW
		uint32 crUserRateAdaptiveFifoNotEmptyEn : 1; //user_rate_adaptive_fifo_not_empty_en, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerIntEn_u;

/*REG_BA_ANALYZER_CLEAR_INT 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crClrTxStNotEmptyNc : 1; //clr_umac_tx_st_not_empty_nc, reset value: 0x0, access type: WO
		uint32 crClrCommonRateAdaptiveFifoNotEmptyNc : 1; //clr_common_rate_adaptive_fifo_not_empty_nc, reset value: 0x0, access type: WO
		uint32 crClrUserRateAdaptiveFifoNotEmptyNc : 1; //clr_user_rate_adaptive_fifo_not_empty_nc, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerClearInt_u;

/*REG_BA_ANALYZER_SENDER_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMulticast : 1; //Multicast indication from sender , reset value: 0x0, access type: RW
		uint32 crUpdateHandlerParamsEnOtherRx : 1; //update_handler_params_en_other_rx, reset value: 0x0, access type: RW
		uint32 crUpdateHandlerParamsEnBaOk : 1; //update_handler_params_en_BA_ok, reset value: 0x0, access type: RW
		uint32 crDoNotSeparateListOtherRx : 1; //In case this value is 1 BAA will not execute list separation ., reset value: 0x0, access type: RW
		uint32 crDoNotSeparateListBaOk : 1; //In case this value is 1 BAA will not execute list separation ., reset value: 0x0, access type: RW
		uint32 crRateAdaptUpdateBypass : 1; //rate_adapt_update_bypass, reset value: 0x0, access type: RW
		uint32 crTxFifoEnOtherRx : 1; //tx_fifo_en_other_rx, reset value: 0x0, access type: RW
		uint32 crTxFifoEnBaOk : 1; //tx_fifo_en_BA_ok, reset value: 0x0, access type: RW
		uint32 crRtsPacketTransmited : 1; //cts_packet_transmited, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
		uint32 crBarUpdateHandlerParamsEnOtherRx : 1; //update_handler_params_en_other_rx for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarUpdateHandlerParamsEnBaOk : 1; //update_handler_params_en_BA_ok for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarDoNotSeparateListOtherRx : 1; //In case this value is 1 BAA will not execute list separation for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarDoNotSeparateListBaOk : 1; //In case this value is 1 BAA will not execute list separation for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarRateAdaptUpdateBypass : 1; //rate_adapt_update_bypass for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarTxFifoEnOtherRx : 1; //tx_fifo_en_other_rx for BAR PD., reset value: 0x0, access type: RW
		uint32 crBarTxFifoEnBaOk : 1; //tx_fifo_en_BA_ok for BAR PD., reset value: 0x0, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegBaAnalyzerSenderReg_u;

/*REG_BA_ANALYZER_SENDER_REG_SEL 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRegSelPdTypeBitmap : 32; //When set BAR Sender register configuration is selected for that PD type., reset value: 0x10, access type: RW
	} bitFields;
} RegBaAnalyzerSenderRegSel_u;

/*REG_BA_ANALYZER_DPL_IDX 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crDplIdxSuccess : 8; //dpl_idx_success bit6  select qmgr, reset value: 0x8, access type: RW
		uint32 crDplIdxDiscard : 8; //dpl_idx_discard bit6  select qmgr, reset value: 0x1, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBaAnalyzerDplIdx_u;

/*REG_BA_ANALYZER_TXH_PSDU_LENGHT_THR 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crTxhPsduLenghtThr : 30; //txh_psdu_lenght_thr, reset value: 0x200, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegBaAnalyzerTxhPsduLenghtThr_u;

/*REG_BA_ANALYZER_SENDER_TX_STATUS_WORD0 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderTxStatusWord0 : 32; //sender_tx_status_word0, reset value: 0x0, access type: RW
	} bitFields;
} RegBaAnalyzerSenderTxStatusWord0_u;

/*REG_BA_ANALYZER_SENDER_TX_STATUS_WORD1 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderTxStatusWord1 : 32; //sender_tx_status_word1, reset value: 0x0, access type: RW
	} bitFields;
} RegBaAnalyzerSenderTxStatusWord1_u;

/*REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderRaCommonReportWord0 : 32; //sender word0 for RA common report., reset value: 0x0, access type: RW
	} bitFields;
} RegBaAnalyzerSenderRaCommonReportWord0_u;

/*REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderRaCommonReportWord1 : 32; //sender word1 for RA common report., reset value: 0x0, access type: RW
	} bitFields;
} RegBaAnalyzerSenderRaCommonReportWord1_u;

/*REG_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crBaAnalyzerCountersEn : 28; //ba_analyzer_counters_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegBaAnalyzerBaAnalyzerCountersEn_u;

/*REG_BA_ANALYZER_MBA_CFG 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMbaMaxNumTids : 2; //Max number of TIDs parsed in M-BA frame: , '00' - unlimited , '01' - up to 9 (first 9) , '10' -  up to number of expected TID response, reset value: 0x0, access type: RW
		uint32 crSupportAckType0Tid15 : 1; //Support M-BA Ack type 0 for TID 15. , If not supported TID is treated as other Rx., reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerMbaCfg_u;

/*REG_BA_ANALYZER_CBA_CFG 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSkipCheckTidMine : 1; //Ignore TID on C-BA frame parsing., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegBaAnalyzerCbaCfg_u;

/*REG_BA_ANALYZER_TRAFFIC_IND_CFG 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crTrafficBaAgreementOriginatorEn : 1; //Traffic indication (BA agreement originator) enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegBaAnalyzerTrafficIndCfg_u;

/*REG_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crClrLockPdTypeBitmap : 32; //Clear TID lock bit per PD type in case there was no failure., reset value: 0x10, access type: RW
	} bitFields;
} RegBaAnalyzerClrLockPdTypeBitmap_u;

/*REG_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crEnSsnAlignPdTypeBitmap : 32; //Enable SSN alignment per PD type. , Treat PD types that are disabled but there is a bit set in BA bitmap as succeeded. Excluding BAR that even if BA bitmap is all zeros is treated as a successful PD., reset value: 0x3, access type: RW
	} bitFields;
} RegBaAnalyzerEnSsnAlignPdTypeBitmap_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoBaseAddr : 22; //Tx status report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegBaAnalyzerStsReportFifoBaseAddr_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoDepthMinusOne : 9; //Tx status report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegBaAnalyzerStsReportFifoDepthMinusOne_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoClearStrb : 1; //Clear Tx status report FIFO. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crStsReportFifoClearFullDropCtrStrb : 1; //Clear Tx status report FIFO full drop counter. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crStsReportFifoClearDecLessThanZeroStrb : 1; //Clear Tx status report FIFO decrement amount less than zero indication. , By writing '1'., reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerStsReportFifoClearStrb_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoRdEntriesNum : 10; //Tx status report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerStsReportFifoRdEntriesNum_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srStsReportFifoNumEntriesCount : 10; //Tx status report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerStsReportFifoNumEntriesCount_u;

/*REG_BA_ANALYZER_STS_REPORT_FIFO_DEBUG 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srStsReportFifoWrPtr : 9; //Tx status report FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 srStsReportFifoNotEmpty : 1; //Tx status report FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 srStsReportFifoFull : 1; //Tx status report FIFO full indication, reset value: 0x0, access type: RO
		uint32 srStsReportFifoDecrementLessThanZero : 1; //Tx status report setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 srStsReportFifoFullDropCtr : 9; //Tx status report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
	} bitFields;
} RegBaAnalyzerStsReportFifoDebug_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoBaseAddr : 22; //Rate adaptive common report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoBaseAddr_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoDepthMinusOne : 9; //Rate adaptive common report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoDepthMinusOne_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoClearStrb : 1; //Clear Rate adaptive common report FIFO. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crRaCommonReportFifoClearFullDropCtrStrb : 1; //Clear Rate adaptive common report FIFO full drop counter. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crRaCommonReportFifoClearDecLessThanZeroStrb : 1; //Clear Rate adaptive common report FIFO decrement amount less than zero indication. , By writing '1'., reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoClearStrb_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoRdEntriesNum : 10; //Rate adaptive common report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoRdEntriesNum_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaCommonReportFifoNumEntriesCount : 10; //Rate adaptive common report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoNumEntriesCount_u;

/*REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaCommonReportFifoWrPtr : 9; //Rate adaptive common report FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 srRaCommonReportFifoNotEmpty : 1; //Rate adaptive common report FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 srRaCommonReportFifoFull : 1; //Rate adaptive common report FIFO full indication, reset value: 0x0, access type: RO
		uint32 srRaCommonReportFifoDecrementLessThanZero : 1; //Rate adaptive common report setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 srRaCommonReportFifoFullDropCtr : 9; //Rate adaptive common report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
	} bitFields;
} RegBaAnalyzerRaCommonReportFifoDebug_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoBaseAddr : 22; //Rate adaptive user report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoBaseAddr_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoDepthMinusOne : 9; //Rate adaptive user report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoDepthMinusOne_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoClearStrb : 1; //Clear Rate adaptive user report FIFO. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crRaFirstUserReportFifoClearFullDropCtrStrb : 1; //Clear Rate adaptive first user report FIFO full drop counter. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crRaNonFirstUserReportFifoClearFullDropCtrStrb : 1; //Clear Rate adaptive non first user report FIFO full drop counter. , By writing '1'., reset value: 0x0, access type: WO
		uint32 crRaUserReportFifoClearDecLessThanZeroStrb : 1; //Clear Rate adaptive user report FIFO decrement amount less than zero indication. , By writing '1'., reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoClearStrb_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoRdEntriesNum : 10; //Rate adaptive user report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoRdEntriesNum_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaUserReportFifoNumEntriesCount : 10; //Rate adaptive user report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoNumEntriesCount_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaUserReportFifoWrPtr : 9; //Rate adaptive user report FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 srRaUserReportFifoNotEmpty : 1; //Rate adaptive user report FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 srRaUserReportFifoFull : 1; //Rate adaptive user report FIFO full indication, reset value: 0x0, access type: RO
		uint32 srRaUserReportFifoDecrementLessThanZero : 1; //Rate adaptive user report setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 srRaFirstUserReportFifoFullDropCtr : 9; //Rate adaptive first user report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoDebug0_u;

/*REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaNonFirstUserReportFifoFullDropCtr : 9; //Rate adaptive non first user report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegBaAnalyzerRaUserReportFifoDebug1_u;

/*REG_BA_ANALYZER_RTS_CFG 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRtsRetryLimit : 4; //RTS retry limit. , When the sum of RTS counter and Retry counter exceeds or equals to this limit PD should be discarded. , '0' means unlimited., reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegBaAnalyzerRtsCfg_u;

/*REG_BA_ANALYZER_ETH_TYPE_OVERHEAD 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crEthType0Oh : 8; //Overhead length for Ether type = 0 , 8-bit two's complement value, reset value: 0xFA, access type: RW
		uint32 crEthType1Oh : 8; //Overhead length for Ether type = 1 , 8-bit two's complement value, reset value: 0xFA, access type: RW
		uint32 crEthType2Oh : 8; //Overhead length for Ether type = 2 , 8-bit two's complement value, reset value: 0xF2, access type: RW
		uint32 crEthType3Oh : 8; //Overhead length for Ether type = 3 , 8-bit two's complement value, reset value: 0xFA, access type: RW
	} bitFields;
} RegBaAnalyzerEthTypeOverhead_u;

/*REG_BA_ANALYZER_STA_MODE_CFG0 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStaModeAid : 11; //My AID in Station mode., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegBaAnalyzerStaModeCfg0_u;

/*REG_BA_ANALYZER_STA_MODE_CFG1 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStaModeDropMyAidTidRepeated : 1; //When set to '1' broadcast MBA in station mode is dropped if my AID in combination with TID is repeated. , When set to '0' only first AID/TID encountered is written to CB., reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegBaAnalyzerStaModeCfg1_u;

/*REG_BA_ANALYZER_FIFO_STATUS0 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srBaRdFifoEmpty : 1; //BA read FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srBaRdFifoFull : 1; //BA read FIFO full indication., reset value: 0x0, access type: RO
		uint32 srBaRdFifoBytes : 2; //Valid entries in BA read FIFO., reset value: 0x0, access type: RO
		uint32 srAreadyStageFifoEmpty : 1; //Aready stage FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srAreadyStageFifoFull : 1; //Aready stage FIFO full indication., reset value: 0x0, access type: RO
		uint32 srAreadyStageFifoBytes : 2; //Valid entries in Aready stage FIFO., reset value: 0x0, access type: RO
		uint32 srRdPtrFifoEmpty : 1; //Read pointer FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srRdPtrFifoFull : 1; //Read pointer FIFO full indication., reset value: 0x0, access type: RO
		uint32 srRdPtrFifoBytes : 3; //Valid entries in Read pointer FIFO., reset value: 0x0, access type: RO
		uint32 srUpdatePdPtrFifoEmpty : 1; //Update PD pointer FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srUpdatePdPtrFifoFull : 1; //Update PD pointer FIFO full indication., reset value: 0x0, access type: RO
		uint32 srUpdatePdPtrFifoBytes : 3; //Valid entries in Update PD pointer FIFO., reset value: 0x0, access type: RO
		uint32 srUpdatePdPtrSampFifoEmpty : 1; //Update PD pointer sample FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srUpdatePdPtrSampFifoFull : 1; //Update PD pointer sample FIFO full indication., reset value: 0x0, access type: RO
		uint32 srUpdatePdPtrSampFifoBytes : 2; //Valid entries in Update PD pointer sample FIFO., reset value: 0x0, access type: RO
		uint32 srStatisticsVapCntFifoEmpty : 1; //Statistics VAP counter FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srStatisticsVapCntFifoFull : 1; //Statistics VAP counter FIFO full indication., reset value: 0x0, access type: RO
		uint32 srStatisticsVapCntFifoBytes : 2; //Valid entries in Statistics VAP counter FIFO., reset value: 0x0, access type: RO
		uint32 srStatisticsUserCntFifoEmpty : 1; //Statistics User counter FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srStatisticsUserCntFifoFull : 1; //Statistics User counter FIFO full indication., reset value: 0x0, access type: RO
		uint32 srStatisticsUserCntFifoBytes : 2; //Valid entries in Statistics User counter FIFO., reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
	} bitFields;
} RegBaAnalyzerFifoStatus0_u;

/*REG_BA_ANALYZER_FIFO_STATUS1 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srStaModeFifoEmpty : 1; //Station mode FIFO empty indication., reset value: 0x1, access type: RO
		uint32 srStaModeFifoFull : 1; //Station mode FIFO full indication., reset value: 0x0, access type: RO
		uint32 srStaModeFifoBytes : 2; //Valid entries in Station mode FIFO., reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegBaAnalyzerFifoStatus1_u;

/*REG_BA_ANALYZER_FSM_STATUS0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRetryStdMapFsmState : 5; //Retry STD map FSM state., reset value: 0x0, access type: RO
		uint32 srRetryBaPosFetchFsmState : 2; //Retry BA position fetch FSM state., reset value: 0x0, access type: RO
		uint32 srRetryBaReadFsmState : 4; //Retry BA read FSM state., reset value: 0x0, access type: RO
		uint32 srRetrySsnAlignFsmState : 4; //Retry SSN alignment FSM state., reset value: 0x0, access type: RO
		uint32 srRetryRdTidInfoFsmState : 2; //Retry read TID info FSM state., reset value: 0x0, access type: RO
		uint32 srRetryRdTidInfoStdFsmState : 2; //Retry read TID info STD FSM state., reset value: 0x0, access type: RO
		uint32 srRetryFragHandlerFsmState : 3; //Retry Fragment handler FSM state., reset value: 0x0, access type: RO
		uint32 srRetryRetransUpdateFsmState : 2; //Retry retransmit update FSM state., reset value: 0x0, access type: RO
		uint32 srRetrySsnPsUpdateFsmState : 3; //Retry SSN and PS update FSM state., reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegBaAnalyzerFsmStatus0_u;

/*REG_BA_ANALYZER_FSM_STATUS1 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srPdsRdStateFsmState : 3; //PDS read state FSM state., reset value: 0x0, access type: RO
		uint32 srPdsRdPtrFsmState : 3; //PDS read pointer FSM state., reset value: 0x0, access type: RO
		uint32 srPdsUpdatePdPtrFsmState0 : 3; //PDS update PD pointer FSM state #0., reset value: 0x0, access type: RO
		uint32 srPdsUpdatePdPtrFsmState1 : 3; //PDS update PD pointer FSM state #1., reset value: 0x0, access type: RO
		uint32 srPdsRaFifoCommonUserSmDbg : 3; //RA common/user FIFO FSM state., reset value: 0x0, access type: RO
		uint32 srRtsPdFsmState : 2; //RTS PD FSM state., reset value: 0x0, access type: RO
		uint32 srRtsStaDbFsmState : 2; //RTS station DB FSM state., reset value: 0x0, access type: RO
		uint32 srStatisticsVapCntFsmState : 2; //Statistics VAP counters FSM state., reset value: 0x0, access type: RO
		uint32 srStatisticsUserCntFsmState : 2; //Statistics User counters FSM state., reset value: 0x0, access type: RO
		uint32 srStaModeFsmState : 3; //Station mode FSM state., reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegBaAnalyzerFsmStatus1_u;

/*REG_BA_ANALYZER_LOGGER_REG 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baaLoggerMsgId0RegEn : 1; //ba_analyzer_logger_reg_en, reset value: 0x0, access type: RW
		uint32 baaLoggerMsgId1To4RegEn : 1; //Enable Logger msg_id 1 to 4., reset value: 0x0, access type: RW
		uint32 baAnalyzerLoggerRegPriority : 2; //ba_analyzer_logger_reg_priority, reset value: 0x0, access type: RW
		uint32 baAnalyzerLoggerRegActive : 1; //ba_analyzer_logger_reg_active, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegBaAnalyzerLoggerReg_u;

/*REG_BA_ANALYZER_LOGGER_FILTER0 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crLogRtsOnly : 1; //Filter only RTS logs, reset value: 0x1, access type: RW
		uint32 crLogAllWoRts : 1; //Filter all logs excluding RTS, reset value: 0x1, access type: RW
		uint32 crLogTxTidIdx : 7; //Filter only TIDs with TID index equals cr_log_tx_tid_idx, reset value: 0x0, access type: RW
		uint32 crLogAllTxTidIdx : 1; //Log all TID indexes, reset value: 0x1, access type: RW
		uint32 crLogPdType : 5; //Filter only PDs with PD type equals cr_log_pd_type, reset value: 0x0, access type: RW
		uint32 crLogAllPdType : 1; //Log all PD types, reset value: 0x1, access type: RW
		uint32 crLogTxVapid : 7; //Filter only VAPs with VAP ID equals cr_log_tx_vapid, reset value: 0x0, access type: RW
		uint32 crLogAllTxVapid : 1; //Log all VAP IDs, reset value: 0x1, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegBaAnalyzerLoggerFilter0_u;

/*REG_BA_ANALYZER_LOGGER_FILTER1 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crLogTxStaid : 9; //Filter only Stations with Station ID equals cr_log_tx_staid, reset value: 0x0, access type: RW
		uint32 crLogAllTxStaid : 1; //Log all Station IDs, reset value: 0x1, access type: RW
		uint32 crLogTxTidBitmap : 9; //Filter only TIDs set in the bitmap, reset value: 0x1ff, access type: RW
		uint32 crLogSuccess : 1; //Log only if there was at least a single successful MPDU, reset value: 0x1, access type: RW
		uint32 crLogDiscard : 1; //Log only if there was at least a single discarded MPDU, reset value: 0x1, access type: RW
		uint32 crLogRetrans : 1; //Log only if there was at least a single retransmitted MPDU, reset value: 0x1, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegBaAnalyzerLoggerFilter1_u;

/*REG_BA_ANALYZER_LOGGER_STATISTICS_REG 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerStatisticsCntEn : 1; //Logger Statistics counters enable, reset value: 0x0, access type: RW
		uint32 loggerStatisticsCntPriority : 2; //Logger Statistics counters priority, reset value: 0x0, access type: RW
		uint32 loggerStatisticsCntActive : 1; //Logger Statistics counters active indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegBaAnalyzerLoggerStatisticsReg_u;

/*REG_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srStatisticsCntEventId4SetToZero : 1; //Statistics counter event ID 4 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId5SetToZero : 1; //Statistics counter event ID 5 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId6SetToZero : 1; //Statistics counter event ID 6 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId7SetToZero : 1; //Statistics counter event ID 7 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId13SetToZero : 1; //Statistics counter event ID 13 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId14SetToZero : 1; //Statistics counter event ID 14 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId15SetToZero : 1; //Statistics counter event ID 15 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId19SetToZero : 1; //Statistics counter event ID 19 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntEventId26SetToZero : 1; //Statistics counter event ID 26 set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntMsduRetryExhaustedSetToZero : 1; //Logger statistics counter msdu_retry_exhausted set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntTotalMsduSuccessSetToZero : 1; //Logger statistics counter total_msdu_success set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntTotalAmsduSuccessSetToZero : 1; //Logger statistics counter total_amsdu_success set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntAmsduTotalBytesSuccessSetToZero : 1; //Logger statistics counter amsdu_total_bytes_success set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntAmsduRetryExhaustedSetToZero : 1; //Logger statistics counter amsdu_retry_exhausted set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntAmsduOneRetrySetToZero : 1; //Logger statistics counter amsdu_one_retry set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntAmsduMultRetrySetToZero : 1; //Logger statistics counter amsdu_mult_retry set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntMsduMultRetrySucSetToZero : 1; //Logger statistics counter msdu_mult_retry_suc set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntTotalBytesSuccessSetToZero : 1; //Logger statistics counter total_bytes_success set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntRetransMsduSuccessSetToZero : 1; //Logger statistics counter retrans_msdu_success set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntMsduOneRetrySucSetToZero : 1; //Logger statistics counter msdu_one_retry_suc set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srStatisticsCntRetransmittedMpduSucceedSetToZero : 1; //Logger statistics counter retransmitted_mpdu_succeed set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srRaReportTotalSuccessTidLengthSetToZero : 9; //RA report counter per TID total_success_tid_length set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 srRaReportTotalFailLengthSetToZero : 1; //RA report counter total_fail_length set to zero, due to negative value result., reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegBaAnalyzerCalcCntSetToZeroStatus_u;

/*REG_BA_ANALYZER_BA_ANALYZER_SPARE 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crBaAnalyzerSpare : 32; //ba_analyzer_spare, reset value: 0x88888888, access type: RW
	} bitFields;
} RegBaAnalyzerBaAnalyzerSpare_u;

//========================================
/* REG_BA_ANALYZER_MODE 0x0 */
#define B0_BA_ANALYZER_MODE_FIELD_CR_UPDATE_BW_MASK                                                               0x00000001
#define B0_BA_ANALYZER_MODE_FIELD_CR_BLOCK_EN_MASK                                                                0x00000002
#define B0_BA_ANALYZER_MODE_FIELD_CR_UPDATE_PS_CNT_PER_FRAG_MASK                                                  0x00000004
//========================================
/* REG_BA_ANALYZER_INT_ERROR_STATUS 0x4 */
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_TID_INT_MASK                                          0x00000001
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_ACK_TYPE0_TID15_INT_MASK                              0x00000002
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_FRAG_NUM_INT_MASK                                     0x00000004
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_BA_LENGTH_INT_MASK                                    0x00000008
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_STA_MODE_MPDU_LENGTH_INT_MASK                         0x00000010
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_UNEXPECTED_STA_MODE_REPEATED_AID_TID_INT_MASK                    0x00000020
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_DROP_TX_ST_WHILE_FULL_INT_MASK                                   0x00000040
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_DROP_COMMON_RT_ADAPT_WHILE_FULL_INT_MASK                         0x00000080
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_DROP_FIRST_USER_RT_ADAPT_WHILE_FULL_INT_MASK                     0x00000100
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_DROP_NON_FIRST_USER_RT_ADAPT_WHILE_FULL_INT_MASK                 0x00000200
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_TX_ST_UNDERFLOW_INT_MASK                                         0x00000400
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_COMMON_RT_ADAPT_UNDERFLOW_INT_MASK                               0x00000800
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_USER_RT_ADAPT_UNDERFLOW_INT_MASK                                 0x00001000
#define B0_BA_ANALYZER_INT_ERROR_STATUS_FIELD_SR_CALC_CNT_SET_TO_ZERO_INT_MASK                                    0x00002000
//========================================
/* REG_BA_ANALYZER_INT_ERROR_EN 0x8 */
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_TID_INT_EN_MASK                                           0x00000001
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_ACK_TYPE0_TID15_INT_EN_MASK                               0x00000002
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_FRAG_NUM_INT_EN_MASK                                      0x00000004
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_BA_LENGTH_INT_EN_MASK                                     0x00000008
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_STA_MODE_MPDU_LENGTH_INT_EN_MASK                          0x00000010
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_UNEXPECTED_STA_MODE_REPEATED_AID_TID_INT_EN_MASK                     0x00000020
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_DROP_TX_ST_WHILE_FULL_INT_EN_MASK                                    0x00000040
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_DROP_COMMON_RT_ADAPT_WHILE_FULL_INT_EN_MASK                          0x00000080
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_DROP_FIRST_USER_RT_ADAPT_WHILE_FULL_INT_EN_MASK                      0x00000100
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_DROP_NON_FIRST_USER_RT_ADAPT_WHILE_FULL_INT_EN_MASK                  0x00000200
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_TX_ST_UNDERFLOW_INT_EN_MASK                                          0x00000400
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_COMMON_RT_ADAPT_UNDERFLOW_INT_EN_MASK                                0x00000800
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_USER_RT_ADAPT_UNDERFLOW_INT_EN_MASK                                  0x00001000
#define B0_BA_ANALYZER_INT_ERROR_EN_FIELD_CR_CALC_CNT_SET_TO_ZERO_INT_EN_MASK                                     0x00002000
//========================================
/* REG_BA_ANALYZER_CLEAR_INT_ERROR 0xC */
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_TID_INT_MASK                                       0x00000001
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_ACK_TYPE0_TID15_INT_MASK                           0x00000002
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_FRAG_NUM_INT_MASK                                  0x00000004
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_BA_LENGTH_INT_MASK                                 0x00000008
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_STA_MODE_MPDU_LENGTH_INT_MASK                      0x00000010
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_UNEXPECTED_STA_MODE_REPEATED_AID_TID_INT_MASK                 0x00000020
#define B0_BA_ANALYZER_CLEAR_INT_ERROR_FIELD_CR_CLR_CALC_CNT_SET_TO_ZERO_INT_MASK                                 0x00002000
//========================================
/* REG_BA_ANALYZER_INT_STATUS 0x10 */
#define B0_BA_ANALYZER_INT_STATUS_FIELD_SR_TX_ST_NOT_EMPTY_MASK                                                   0x00000001
#define B0_BA_ANALYZER_INT_STATUS_FIELD_SR_COMMON_RATE_ADAPTIVE_FIFO_NOT_EMPTY_MASK                               0x00000002
#define B0_BA_ANALYZER_INT_STATUS_FIELD_SR_USER_RATE_ADAPTIVE_FIFO_NOT_EMPTY_MASK                                 0x00000004
//========================================
/* REG_BA_ANALYZER_INT_EN 0x14 */
#define B0_BA_ANALYZER_INT_EN_FIELD_CR_TX_ST_NOT_EMPTY_EN_MASK                                                    0x00000001
#define B0_BA_ANALYZER_INT_EN_FIELD_CR_COMMON_RATE_ADAPTIVE_FIFO_NOT_EMPTY_EN_MASK                                0x00000002
#define B0_BA_ANALYZER_INT_EN_FIELD_CR_USER_RATE_ADAPTIVE_FIFO_NOT_EMPTY_EN_MASK                                  0x00000004
//========================================
/* REG_BA_ANALYZER_CLEAR_INT 0x18 */
#define B0_BA_ANALYZER_CLEAR_INT_FIELD_CR_CLR_TX_ST_NOT_EMPTY_NC_MASK                                             0x00000001
#define B0_BA_ANALYZER_CLEAR_INT_FIELD_CR_CLR_COMMON_RATE_ADAPTIVE_FIFO_NOT_EMPTY_NC_MASK                         0x00000002
#define B0_BA_ANALYZER_CLEAR_INT_FIELD_CR_CLR_USER_RATE_ADAPTIVE_FIFO_NOT_EMPTY_NC_MASK                           0x00000004
//========================================
/* REG_BA_ANALYZER_SENDER_REG 0x1C */
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_MULTICAST_MASK                                                         0x00000001
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_UPDATE_HANDLER_PARAMS_EN_OTHER_RX_MASK                                 0x00000002
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_UPDATE_HANDLER_PARAMS_EN_BA_OK_MASK                                    0x00000004
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_DO_NOT_SEPARATE_LIST_OTHER_RX_MASK                                     0x00000008
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_DO_NOT_SEPARATE_LIST_BA_OK_MASK                                        0x00000010
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_RATE_ADAPT_UPDATE_BYPASS_MASK                                          0x00000020
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_TX_FIFO_EN_OTHER_RX_MASK                                               0x00000040
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_TX_FIFO_EN_BA_OK_MASK                                                  0x00000080
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_RTS_PACKET_TRANSMITED_MASK                                             0x00000100
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_UPDATE_HANDLER_PARAMS_EN_OTHER_RX_MASK                             0x00020000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_UPDATE_HANDLER_PARAMS_EN_BA_OK_MASK                                0x00040000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_DO_NOT_SEPARATE_LIST_OTHER_RX_MASK                                 0x00080000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_DO_NOT_SEPARATE_LIST_BA_OK_MASK                                    0x00100000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_RATE_ADAPT_UPDATE_BYPASS_MASK                                      0x00200000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_TX_FIFO_EN_OTHER_RX_MASK                                           0x00400000
#define B0_BA_ANALYZER_SENDER_REG_FIELD_CR_BAR_TX_FIFO_EN_BA_OK_MASK                                              0x00800000
//========================================
/* REG_BA_ANALYZER_SENDER_REG_SEL 0x20 */
#define B0_BA_ANALYZER_SENDER_REG_SEL_FIELD_CR_REG_SEL_PD_TYPE_BITMAP_MASK                                        0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_DPL_IDX 0x24 */
#define B0_BA_ANALYZER_DPL_IDX_FIELD_CR_DPL_IDX_SUCCESS_MASK                                                      0x000000FF
#define B0_BA_ANALYZER_DPL_IDX_FIELD_CR_DPL_IDX_DISCARD_MASK                                                      0x0000FF00
//========================================
/* REG_BA_ANALYZER_TXH_PSDU_LENGHT_THR 0x28 */
#define B0_BA_ANALYZER_TXH_PSDU_LENGHT_THR_FIELD_CR_TXH_PSDU_LENGHT_THR_MASK                                      0x3FFFFFFF
//========================================
/* REG_BA_ANALYZER_SENDER_TX_STATUS_WORD0 0x2C */
#define B0_BA_ANALYZER_SENDER_TX_STATUS_WORD0_FIELD_CR_SENDER_TX_STATUS_WORD0_MASK                                0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_SENDER_TX_STATUS_WORD1 0x30 */
#define B0_BA_ANALYZER_SENDER_TX_STATUS_WORD1_FIELD_CR_SENDER_TX_STATUS_WORD1_MASK                                0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0 0x34 */
#define B0_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0_FIELD_CR_SENDER_RA_COMMON_REPORT_WORD0_MASK                  0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1 0x38 */
#define B0_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1_FIELD_CR_SENDER_RA_COMMON_REPORT_WORD1_MASK                  0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN 0x3C */
#define B0_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN_FIELD_CR_BA_ANALYZER_COUNTERS_EN_MASK                              0x0FFFFFFF
//========================================
/* REG_BA_ANALYZER_MBA_CFG 0x40 */
#define B0_BA_ANALYZER_MBA_CFG_FIELD_CR_MBA_MAX_NUM_TIDS_MASK                                                     0x00000003
#define B0_BA_ANALYZER_MBA_CFG_FIELD_CR_SUPPORT_ACK_TYPE0_TID15_MASK                                              0x00000004
//========================================
/* REG_BA_ANALYZER_CBA_CFG 0x44 */
#define B0_BA_ANALYZER_CBA_CFG_FIELD_CR_SKIP_CHECK_TID_MINE_MASK                                                  0x00000001
//========================================
/* REG_BA_ANALYZER_TRAFFIC_IND_CFG 0x48 */
#define B0_BA_ANALYZER_TRAFFIC_IND_CFG_FIELD_CR_TRAFFIC_BA_AGREEMENT_ORIGINATOR_EN_MASK                           0x00000001
//========================================
/* REG_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP 0x4C */
#define B0_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP_FIELD_CR_CLR_LOCK_PD_TYPE_BITMAP_MASK                              0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP 0x50 */
#define B0_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP_FIELD_CR_EN_SSN_ALIGN_PD_TYPE_BITMAP_MASK                      0xFFFFFFFF
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR 0x54 */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR_FIELD_CR_STS_REPORT_FIFO_BASE_ADDR_MASK                          0x003FFFFF
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE 0x58 */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE_FIELD_CR_STS_REPORT_FIFO_DEPTH_MINUS_ONE_MASK              0x000001FF
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB 0x5C */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB_FIELD_CR_STS_REPORT_FIFO_CLEAR_STRB_MASK                        0x00000001
#define B0_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB_FIELD_CR_STS_REPORT_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK          0x00000002
#define B0_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB_FIELD_CR_STS_REPORT_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK     0x00000004
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM 0x60 */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM_FIELD_CR_STS_REPORT_FIFO_RD_ENTRIES_NUM_MASK                0x000003FF
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT 0x64 */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT_FIELD_SR_STS_REPORT_FIFO_NUM_ENTRIES_COUNT_MASK          0x000003FF
//========================================
/* REG_BA_ANALYZER_STS_REPORT_FIFO_DEBUG 0x68 */
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEBUG_FIELD_SR_STS_REPORT_FIFO_WR_PTR_MASK                                 0x000001FF
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEBUG_FIELD_SR_STS_REPORT_FIFO_NOT_EMPTY_MASK                              0x00010000
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEBUG_FIELD_SR_STS_REPORT_FIFO_FULL_MASK                                   0x00020000
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEBUG_FIELD_SR_STS_REPORT_FIFO_DECREMENT_LESS_THAN_ZERO_MASK               0x00040000
#define B0_BA_ANALYZER_STS_REPORT_FIFO_DEBUG_FIELD_SR_STS_REPORT_FIFO_FULL_DROP_CTR_MASK                          0x1FF00000
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR 0x6C */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR_FIELD_CR_RA_COMMON_REPORT_FIFO_BASE_ADDR_MASK              0x003FFFFF
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE 0x70 */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE_FIELD_CR_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE_MASK  0x000001FF
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB 0x74 */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_COMMON_REPORT_FIFO_CLEAR_STRB_MASK            0x00000001
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_COMMON_REPORT_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK 0x00000002
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_COMMON_REPORT_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK 0x00000004
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM 0x78 */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM_FIELD_CR_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM_MASK    0x000003FF
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT 0x7C */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT_FIELD_SR_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT_MASK 0x000003FF
//========================================
/* REG_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG 0x80 */
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG_FIELD_SR_RA_COMMON_REPORT_FIFO_WR_PTR_MASK                     0x000001FF
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG_FIELD_SR_RA_COMMON_REPORT_FIFO_NOT_EMPTY_MASK                  0x00010000
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG_FIELD_SR_RA_COMMON_REPORT_FIFO_FULL_MASK                       0x00020000
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG_FIELD_SR_RA_COMMON_REPORT_FIFO_DECREMENT_LESS_THAN_ZERO_MASK   0x00040000
#define B0_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG_FIELD_SR_RA_COMMON_REPORT_FIFO_FULL_DROP_CTR_MASK              0x1FF00000
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR 0x84 */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR_FIELD_CR_RA_USER_REPORT_FIFO_BASE_ADDR_MASK                  0x003FFFFF
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE 0x88 */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE_FIELD_CR_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE_MASK      0x000001FF
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB 0x8C */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_USER_REPORT_FIFO_CLEAR_STRB_MASK                0x00000001
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_FIRST_USER_REPORT_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK 0x00000002
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_NON_FIRST_USER_REPORT_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK 0x00000004
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB_FIELD_CR_RA_USER_REPORT_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK 0x00000008
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM 0x90 */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM_FIELD_CR_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM_MASK        0x000003FF
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT 0x94 */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT_FIELD_SR_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT_MASK  0x000003FF
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0 0x98 */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0_FIELD_SR_RA_USER_REPORT_FIFO_WR_PTR_MASK                        0x000001FF
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0_FIELD_SR_RA_USER_REPORT_FIFO_NOT_EMPTY_MASK                     0x00010000
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0_FIELD_SR_RA_USER_REPORT_FIFO_FULL_MASK                          0x00020000
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0_FIELD_SR_RA_USER_REPORT_FIFO_DECREMENT_LESS_THAN_ZERO_MASK      0x00040000
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0_FIELD_SR_RA_FIRST_USER_REPORT_FIFO_FULL_DROP_CTR_MASK           0x1FF00000
//========================================
/* REG_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1 0x9C */
#define B0_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1_FIELD_SR_RA_NON_FIRST_USER_REPORT_FIFO_FULL_DROP_CTR_MASK       0x000001FF
//========================================
/* REG_BA_ANALYZER_RTS_CFG 0xA0 */
#define B0_BA_ANALYZER_RTS_CFG_FIELD_CR_RTS_RETRY_LIMIT_MASK                                                      0x0000000F
//========================================
/* REG_BA_ANALYZER_ETH_TYPE_OVERHEAD 0xA4 */
#define B0_BA_ANALYZER_ETH_TYPE_OVERHEAD_FIELD_CR_ETH_TYPE0_OH_MASK                                               0x000000FF
#define B0_BA_ANALYZER_ETH_TYPE_OVERHEAD_FIELD_CR_ETH_TYPE1_OH_MASK                                               0x0000FF00
#define B0_BA_ANALYZER_ETH_TYPE_OVERHEAD_FIELD_CR_ETH_TYPE2_OH_MASK                                               0x00FF0000
#define B0_BA_ANALYZER_ETH_TYPE_OVERHEAD_FIELD_CR_ETH_TYPE3_OH_MASK                                               0xFF000000
//========================================
/* REG_BA_ANALYZER_STA_MODE_CFG0 0xA8 */
#define B0_BA_ANALYZER_STA_MODE_CFG0_FIELD_CR_STA_MODE_AID_MASK                                                   0x000007FF
//========================================
/* REG_BA_ANALYZER_STA_MODE_CFG1 0xAC */
#define B0_BA_ANALYZER_STA_MODE_CFG1_FIELD_CR_STA_MODE_DROP_MY_AID_TID_REPEATED_MASK                              0x00000001
//========================================
/* REG_BA_ANALYZER_FIFO_STATUS0 0xB0 */
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_BA_RD_FIFO_EMPTY_MASK                                                0x00000001
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_BA_RD_FIFO_FULL_MASK                                                 0x00000002
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_BA_RD_FIFO_BYTES_MASK                                                0x0000000C
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_AREADY_STAGE_FIFO_EMPTY_MASK                                         0x00000010
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_AREADY_STAGE_FIFO_FULL_MASK                                          0x00000020
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_AREADY_STAGE_FIFO_BYTES_MASK                                         0x000000C0
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_RD_PTR_FIFO_EMPTY_MASK                                               0x00000100
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_RD_PTR_FIFO_FULL_MASK                                                0x00000200
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_RD_PTR_FIFO_BYTES_MASK                                               0x00001C00
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_FIFO_EMPTY_MASK                                        0x00002000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_FIFO_FULL_MASK                                         0x00004000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_FIFO_BYTES_MASK                                        0x00038000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_SAMP_FIFO_EMPTY_MASK                                   0x00040000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_SAMP_FIFO_FULL_MASK                                    0x00080000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_UPDATE_PD_PTR_SAMP_FIFO_BYTES_MASK                                   0x00300000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_VAP_CNT_FIFO_EMPTY_MASK                                   0x00400000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_VAP_CNT_FIFO_FULL_MASK                                    0x00800000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_VAP_CNT_FIFO_BYTES_MASK                                   0x03000000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_USER_CNT_FIFO_EMPTY_MASK                                  0x04000000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_USER_CNT_FIFO_FULL_MASK                                   0x08000000
#define B0_BA_ANALYZER_FIFO_STATUS0_FIELD_SR_STATISTICS_USER_CNT_FIFO_BYTES_MASK                                  0x30000000
//========================================
/* REG_BA_ANALYZER_FIFO_STATUS1 0xB4 */
#define B0_BA_ANALYZER_FIFO_STATUS1_FIELD_SR_STA_MODE_FIFO_EMPTY_MASK                                             0x00000001
#define B0_BA_ANALYZER_FIFO_STATUS1_FIELD_SR_STA_MODE_FIFO_FULL_MASK                                              0x00000002
#define B0_BA_ANALYZER_FIFO_STATUS1_FIELD_SR_STA_MODE_FIFO_BYTES_MASK                                             0x0000000C
//========================================
/* REG_BA_ANALYZER_FSM_STATUS0 0xB8 */
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_STD_MAP_FSM_STATE_MASK                                          0x0000001F
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_BA_POS_FETCH_FSM_STATE_MASK                                     0x00000060
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_BA_READ_FSM_STATE_MASK                                          0x00000780
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_SSN_ALIGN_FSM_STATE_MASK                                        0x00007800
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_RD_TID_INFO_FSM_STATE_MASK                                      0x00018000
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_RD_TID_INFO_STD_FSM_STATE_MASK                                  0x00060000
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_FRAG_HANDLER_FSM_STATE_MASK                                     0x00380000
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_RETRANS_UPDATE_FSM_STATE_MASK                                   0x00C00000
#define B0_BA_ANALYZER_FSM_STATUS0_FIELD_SR_RETRY_SSN_PS_UPDATE_FSM_STATE_MASK                                    0x07000000
//========================================
/* REG_BA_ANALYZER_FSM_STATUS1 0xBC */
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_PDS_RD_STATE_FSM_STATE_MASK                                           0x00000007
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_PDS_RD_PTR_FSM_STATE_MASK                                             0x00000038
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_PDS_UPDATE_PD_PTR_FSM_STATE0_MASK                                     0x000001C0
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_PDS_UPDATE_PD_PTR_FSM_STATE1_MASK                                     0x00000E00
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_PDS_RA_FIFO_COMMON_USER_SM_DBG_MASK                                   0x00007000
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_RTS_PD_FSM_STATE_MASK                                                 0x00018000
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_RTS_STA_DB_FSM_STATE_MASK                                             0x00060000
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_STATISTICS_VAP_CNT_FSM_STATE_MASK                                     0x00180000
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_STATISTICS_USER_CNT_FSM_STATE_MASK                                    0x00600000
#define B0_BA_ANALYZER_FSM_STATUS1_FIELD_SR_STA_MODE_FSM_STATE_MASK                                               0x03800000
//========================================
/* REG_BA_ANALYZER_LOGGER_REG 0xC0 */
#define B0_BA_ANALYZER_LOGGER_REG_FIELD_BAA_LOGGER_MSG_ID_0_REG_EN_MASK                                           0x00000001
#define B0_BA_ANALYZER_LOGGER_REG_FIELD_BAA_LOGGER_MSG_ID_1TO4_REG_EN_MASK                                        0x00000002
#define B0_BA_ANALYZER_LOGGER_REG_FIELD_BA_ANALYZER_LOGGER_REG_PRIORITY_MASK                                      0x0000000C
#define B0_BA_ANALYZER_LOGGER_REG_FIELD_BA_ANALYZER_LOGGER_REG_ACTIVE_MASK                                        0x00000010
//========================================
/* REG_BA_ANALYZER_LOGGER_FILTER0 0xC4 */
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_RTS_ONLY_MASK                                                  0x00000001
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_ALL_WO_RTS_MASK                                                0x00000002
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_TX_TID_IDX_MASK                                                0x000001FC
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_ALL_TX_TID_IDX_MASK                                            0x00000200
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_PD_TYPE_MASK                                                   0x00007C00
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_ALL_PD_TYPE_MASK                                               0x00008000
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_TX_VAPID_MASK                                                  0x007F0000
#define B0_BA_ANALYZER_LOGGER_FILTER0_FIELD_CR_LOG_ALL_TX_VAPID_MASK                                              0x00800000
//========================================
/* REG_BA_ANALYZER_LOGGER_FILTER1 0xC8 */
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_TX_STAID_MASK                                                  0x000001FF
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_ALL_TX_STAID_MASK                                              0x00000200
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_TX_TID_BITMAP_MASK                                             0x0007FC00
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_SUCCESS_MASK                                                   0x00080000
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_DISCARD_MASK                                                   0x00100000
#define B0_BA_ANALYZER_LOGGER_FILTER1_FIELD_CR_LOG_RETRANS_MASK                                                   0x00200000
//========================================
/* REG_BA_ANALYZER_LOGGER_STATISTICS_REG 0xCC */
#define B0_BA_ANALYZER_LOGGER_STATISTICS_REG_FIELD_LOGGER_STATISTICS_CNT_EN_MASK                                  0x00000001
#define B0_BA_ANALYZER_LOGGER_STATISTICS_REG_FIELD_LOGGER_STATISTICS_CNT_PRIORITY_MASK                            0x00000006
#define B0_BA_ANALYZER_LOGGER_STATISTICS_REG_FIELD_LOGGER_STATISTICS_CNT_ACTIVE_MASK                              0x00000008
//========================================
/* REG_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS 0xD0 */
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_4_SET_TO_ZERO_MASK            0x00000001
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_5_SET_TO_ZERO_MASK            0x00000002
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_6_SET_TO_ZERO_MASK            0x00000004
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_7_SET_TO_ZERO_MASK            0x00000008
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_13_SET_TO_ZERO_MASK           0x00000010
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_14_SET_TO_ZERO_MASK           0x00000020
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_15_SET_TO_ZERO_MASK           0x00000040
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_19_SET_TO_ZERO_MASK           0x00000080
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_EVENT_ID_26_SET_TO_ZERO_MASK           0x00000100
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_MSDU_RETRY_EXHAUSTED_SET_TO_ZERO_MASK  0x00000200
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_TOTAL_MSDU_SUCCESS_SET_TO_ZERO_MASK    0x00000400
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_TOTAL_AMSDU_SUCCESS_SET_TO_ZERO_MASK   0x00000800
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_AMSDU_TOTAL_BYTES_SUCCESS_SET_TO_ZERO_MASK 0x00001000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_AMSDU_RETRY_EXHAUSTED_SET_TO_ZERO_MASK 0x00002000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_AMSDU_ONE_RETRY_SET_TO_ZERO_MASK       0x00004000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_AMSDU_MULT_RETRY_SET_TO_ZERO_MASK      0x00008000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_MSDU_MULT_RETRY_SUC_SET_TO_ZERO_MASK   0x00010000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_TOTAL_BYTES_SUCCESS_SET_TO_ZERO_MASK   0x00020000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_RETRANS_MSDU_SUCCESS_SET_TO_ZERO_MASK  0x00040000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_MSDU_ONE_RETRY_SUC_SET_TO_ZERO_MASK    0x00080000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_STATISTICS_CNT_RETRANSMITTED_MPDU_SUCCEED_SET_TO_ZERO_MASK 0x00100000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_RA_REPORT_TOTAL_SUCCESS_TID_LENGTH_SET_TO_ZERO_MASK   0x3FE00000
#define B0_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS_FIELD_SR_RA_REPORT_TOTAL_FAIL_LENGTH_SET_TO_ZERO_MASK          0x40000000
//========================================
/* REG_BA_ANALYZER_BA_ANALYZER_SPARE 0xE0 */
#define B0_BA_ANALYZER_BA_ANALYZER_SPARE_FIELD_CR_BA_ANALYZER_SPARE_MASK                                          0xFFFFFFFF


#endif // _BA_ANALYZER_REGS_H_
