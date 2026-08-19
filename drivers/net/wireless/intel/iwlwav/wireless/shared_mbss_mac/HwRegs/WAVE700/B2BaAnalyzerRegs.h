
/***********************************************************************************
File:				B2BaAnalyzerRegs.h
Module:				b2BaAnalyzer
SOC Revision:		latest
Generated at:       2022-05-04 10:58:02
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_BA_ANALYZER_REGS_H_
#define _B2_BA_ANALYZER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_BA_ANALYZER_BASE_ADDRESS                             MEMORY_MAP_UNIT_20045_BASE_ADDRESS
#define	REG_B2_BA_ANALYZER_MODE                                       (B2_BA_ANALYZER_BASE_ADDRESS + 0x0)
#define	REG_B2_BA_ANALYZER_INT_ERROR_STATUS                           (B2_BA_ANALYZER_BASE_ADDRESS + 0x4)
#define	REG_B2_BA_ANALYZER_INT_ERROR_EN                               (B2_BA_ANALYZER_BASE_ADDRESS + 0x8)
#define	REG_B2_BA_ANALYZER_CLEAR_INT_ERROR                            (B2_BA_ANALYZER_BASE_ADDRESS + 0xC)
#define	REG_B2_BA_ANALYZER_INT_STATUS                                 (B2_BA_ANALYZER_BASE_ADDRESS + 0x10)
#define	REG_B2_BA_ANALYZER_INT_EN                                     (B2_BA_ANALYZER_BASE_ADDRESS + 0x14)
#define	REG_B2_BA_ANALYZER_CLEAR_INT                                  (B2_BA_ANALYZER_BASE_ADDRESS + 0x18)
#define	REG_B2_BA_ANALYZER_SENDER_REG                                 (B2_BA_ANALYZER_BASE_ADDRESS + 0x1C)
#define	REG_B2_BA_ANALYZER_SENDER_REG_SEL                             (B2_BA_ANALYZER_BASE_ADDRESS + 0x20)
#define	REG_B2_BA_ANALYZER_DPL_IDX                                    (B2_BA_ANALYZER_BASE_ADDRESS + 0x24)
#define	REG_B2_BA_ANALYZER_TXH_PSDU_LENGHT_THR                        (B2_BA_ANALYZER_BASE_ADDRESS + 0x28)
#define	REG_B2_BA_ANALYZER_SENDER_TX_STATUS_WORD0                     (B2_BA_ANALYZER_BASE_ADDRESS + 0x2C)
#define	REG_B2_BA_ANALYZER_SENDER_TX_STATUS_WORD1                     (B2_BA_ANALYZER_BASE_ADDRESS + 0x30)
#define	REG_B2_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0              (B2_BA_ANALYZER_BASE_ADDRESS + 0x34)
#define	REG_B2_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1              (B2_BA_ANALYZER_BASE_ADDRESS + 0x38)
#define	REG_B2_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN                    (B2_BA_ANALYZER_BASE_ADDRESS + 0x3C)
#define	REG_B2_BA_ANALYZER_MBA_CFG                                    (B2_BA_ANALYZER_BASE_ADDRESS + 0x40)
#define	REG_B2_BA_ANALYZER_CBA_CFG                                    (B2_BA_ANALYZER_BASE_ADDRESS + 0x44)
#define	REG_B2_BA_ANALYZER_TRAFFIC_IND_CFG                            (B2_BA_ANALYZER_BASE_ADDRESS + 0x48)
#define	REG_B2_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP                    (B2_BA_ANALYZER_BASE_ADDRESS + 0x4C)
#define	REG_B2_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP                (B2_BA_ANALYZER_BASE_ADDRESS + 0x50)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR                  (B2_BA_ANALYZER_BASE_ADDRESS + 0x54)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE            (B2_BA_ANALYZER_BASE_ADDRESS + 0x58)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB                 (B2_BA_ANALYZER_BASE_ADDRESS + 0x5C)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM             (B2_BA_ANALYZER_BASE_ADDRESS + 0x60)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT          (B2_BA_ANALYZER_BASE_ADDRESS + 0x64)
#define	REG_B2_BA_ANALYZER_STS_REPORT_FIFO_DEBUG                      (B2_BA_ANALYZER_BASE_ADDRESS + 0x68)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR            (B2_BA_ANALYZER_BASE_ADDRESS + 0x6C)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE      (B2_BA_ANALYZER_BASE_ADDRESS + 0x70)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB           (B2_BA_ANALYZER_BASE_ADDRESS + 0x74)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM       (B2_BA_ANALYZER_BASE_ADDRESS + 0x78)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT    (B2_BA_ANALYZER_BASE_ADDRESS + 0x7C)
#define	REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG                (B2_BA_ANALYZER_BASE_ADDRESS + 0x80)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR              (B2_BA_ANALYZER_BASE_ADDRESS + 0x84)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE        (B2_BA_ANALYZER_BASE_ADDRESS + 0x88)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB             (B2_BA_ANALYZER_BASE_ADDRESS + 0x8C)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM         (B2_BA_ANALYZER_BASE_ADDRESS + 0x90)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT      (B2_BA_ANALYZER_BASE_ADDRESS + 0x94)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0                 (B2_BA_ANALYZER_BASE_ADDRESS + 0x98)
#define	REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1                 (B2_BA_ANALYZER_BASE_ADDRESS + 0x9C)
#define	REG_B2_BA_ANALYZER_RTS_CFG                                    (B2_BA_ANALYZER_BASE_ADDRESS + 0xA0)
#define	REG_B2_BA_ANALYZER_ETH_TYPE_OVERHEAD                          (B2_BA_ANALYZER_BASE_ADDRESS + 0xA4)
#define	REG_B2_BA_ANALYZER_STA_MODE_CFG0                              (B2_BA_ANALYZER_BASE_ADDRESS + 0xA8)
#define	REG_B2_BA_ANALYZER_STA_MODE_CFG1                              (B2_BA_ANALYZER_BASE_ADDRESS + 0xAC)
#define	REG_B2_BA_ANALYZER_FIFO_STATUS0                               (B2_BA_ANALYZER_BASE_ADDRESS + 0xB0)
#define	REG_B2_BA_ANALYZER_FIFO_STATUS1                               (B2_BA_ANALYZER_BASE_ADDRESS + 0xB4)
#define	REG_B2_BA_ANALYZER_FSM_STATUS0                                (B2_BA_ANALYZER_BASE_ADDRESS + 0xB8)
#define	REG_B2_BA_ANALYZER_FSM_STATUS1                                (B2_BA_ANALYZER_BASE_ADDRESS + 0xBC)
#define	REG_B2_BA_ANALYZER_LOGGER_REG                                 (B2_BA_ANALYZER_BASE_ADDRESS + 0xC0)
#define	REG_B2_BA_ANALYZER_LOGGER_FILTER0                             (B2_BA_ANALYZER_BASE_ADDRESS + 0xC4)
#define	REG_B2_BA_ANALYZER_LOGGER_FILTER1                             (B2_BA_ANALYZER_BASE_ADDRESS + 0xC8)
#define	REG_B2_BA_ANALYZER_LOGGER_STATISTICS_REG                      (B2_BA_ANALYZER_BASE_ADDRESS + 0xCC)
#define	REG_B2_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS                (B2_BA_ANALYZER_BASE_ADDRESS + 0xD0)
#define	REG_B2_BA_ANALYZER_BA_ANALYZER_SPARE                          (B2_BA_ANALYZER_BASE_ADDRESS + 0xE0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_BA_ANALYZER_MODE 0x0 */
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
} RegB2BaAnalyzerMode_u;

/*REG_B2_BA_ANALYZER_INT_ERROR_STATUS 0x4 */
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
} RegB2BaAnalyzerIntErrorStatus_u;

/*REG_B2_BA_ANALYZER_INT_ERROR_EN 0x8 */
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
} RegB2BaAnalyzerIntErrorEn_u;

/*REG_B2_BA_ANALYZER_CLEAR_INT_ERROR 0xC */
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
} RegB2BaAnalyzerClearIntError_u;

/*REG_B2_BA_ANALYZER_INT_STATUS 0x10 */
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
} RegB2BaAnalyzerIntStatus_u;

/*REG_B2_BA_ANALYZER_INT_EN 0x14 */
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
} RegB2BaAnalyzerIntEn_u;

/*REG_B2_BA_ANALYZER_CLEAR_INT 0x18 */
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
} RegB2BaAnalyzerClearInt_u;

/*REG_B2_BA_ANALYZER_SENDER_REG 0x1C */
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
} RegB2BaAnalyzerSenderReg_u;

/*REG_B2_BA_ANALYZER_SENDER_REG_SEL 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRegSelPdTypeBitmap : 32; //When set BAR Sender register configuration is selected for that PD type., reset value: 0x10, access type: RW
	} bitFields;
} RegB2BaAnalyzerSenderRegSel_u;

/*REG_B2_BA_ANALYZER_DPL_IDX 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crDplIdxSuccess : 8; //dpl_idx_success bit6  select qmgr, reset value: 0x8, access type: RW
		uint32 crDplIdxDiscard : 8; //dpl_idx_discard bit6  select qmgr, reset value: 0x1, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2BaAnalyzerDplIdx_u;

/*REG_B2_BA_ANALYZER_TXH_PSDU_LENGHT_THR 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crTxhPsduLenghtThr : 30; //txh_psdu_lenght_thr, reset value: 0x200, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2BaAnalyzerTxhPsduLenghtThr_u;

/*REG_B2_BA_ANALYZER_SENDER_TX_STATUS_WORD0 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderTxStatusWord0 : 32; //sender_tx_status_word0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2BaAnalyzerSenderTxStatusWord0_u;

/*REG_B2_BA_ANALYZER_SENDER_TX_STATUS_WORD1 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderTxStatusWord1 : 32; //sender_tx_status_word1, reset value: 0x0, access type: RW
	} bitFields;
} RegB2BaAnalyzerSenderTxStatusWord1_u;

/*REG_B2_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD0 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderRaCommonReportWord0 : 32; //sender word0 for RA common report., reset value: 0x0, access type: RW
	} bitFields;
} RegB2BaAnalyzerSenderRaCommonReportWord0_u;

/*REG_B2_BA_ANALYZER_SENDER_RA_COMMON_REPORT_WORD1 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSenderRaCommonReportWord1 : 32; //sender word1 for RA common report., reset value: 0x0, access type: RW
	} bitFields;
} RegB2BaAnalyzerSenderRaCommonReportWord1_u;

/*REG_B2_BA_ANALYZER_BA_ANALYZER_COUNTERS_EN 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crBaAnalyzerCountersEn : 28; //ba_analyzer_counters_en, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
	} bitFields;
} RegB2BaAnalyzerBaAnalyzerCountersEn_u;

/*REG_B2_BA_ANALYZER_MBA_CFG 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMbaMaxNumTids : 2; //Max number of TIDs parsed in M-BA frame: , '00' - unlimited , '01' - up to 9 (first 9) , '10' -  up to number of expected TID response, reset value: 0x0, access type: RW
		uint32 crSupportAckType0Tid15 : 1; //Support M-BA Ack type 0 for TID 15. , If not supported TID is treated as other Rx., reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2BaAnalyzerMbaCfg_u;

/*REG_B2_BA_ANALYZER_CBA_CFG 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crSkipCheckTidMine : 1; //Ignore TID on C-BA frame parsing., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2BaAnalyzerCbaCfg_u;

/*REG_B2_BA_ANALYZER_TRAFFIC_IND_CFG 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crTrafficBaAgreementOriginatorEn : 1; //Traffic indication (BA agreement originator) enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2BaAnalyzerTrafficIndCfg_u;

/*REG_B2_BA_ANALYZER_CLR_LOCK_PD_TYPE_BITMAP 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crClrLockPdTypeBitmap : 32; //Clear TID lock bit per PD type in case there was no failure., reset value: 0x10, access type: RW
	} bitFields;
} RegB2BaAnalyzerClrLockPdTypeBitmap_u;

/*REG_B2_BA_ANALYZER_EN_SSN_ALIGN_PD_TYPE_BITMAP 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crEnSsnAlignPdTypeBitmap : 32; //Enable SSN alignment per PD type. , Treat PD types that are disabled but there is a bit set in BA bitmap as succeeded. Excluding BAR that even if BA bitmap is all zeros is treated as a successful PD., reset value: 0x3, access type: RW
	} bitFields;
} RegB2BaAnalyzerEnSsnAlignPdTypeBitmap_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_BASE_ADDR 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoBaseAddr : 22; //Tx status report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2BaAnalyzerStsReportFifoBaseAddr_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_DEPTH_MINUS_ONE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoDepthMinusOne : 9; //Tx status report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2BaAnalyzerStsReportFifoDepthMinusOne_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_CLEAR_STRB 0x5C */
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
} RegB2BaAnalyzerStsReportFifoClearStrb_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_RD_ENTRIES_NUM 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStsReportFifoRdEntriesNum : 10; //Tx status report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerStsReportFifoRdEntriesNum_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_NUM_ENTRIES_COUNT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srStsReportFifoNumEntriesCount : 10; //Tx status report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerStsReportFifoNumEntriesCount_u;

/*REG_B2_BA_ANALYZER_STS_REPORT_FIFO_DEBUG 0x68 */
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
} RegB2BaAnalyzerStsReportFifoDebug_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_BASE_ADDR 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoBaseAddr : 22; //Rate adaptive common report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2BaAnalyzerRaCommonReportFifoBaseAddr_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEPTH_MINUS_ONE 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoDepthMinusOne : 9; //Rate adaptive common report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2BaAnalyzerRaCommonReportFifoDepthMinusOne_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_CLEAR_STRB 0x74 */
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
} RegB2BaAnalyzerRaCommonReportFifoClearStrb_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_RD_ENTRIES_NUM 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaCommonReportFifoRdEntriesNum : 10; //Rate adaptive common report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerRaCommonReportFifoRdEntriesNum_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_NUM_ENTRIES_COUNT 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaCommonReportFifoNumEntriesCount : 10; //Rate adaptive common report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerRaCommonReportFifoNumEntriesCount_u;

/*REG_B2_BA_ANALYZER_RA_COMMON_REPORT_FIFO_DEBUG 0x80 */
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
} RegB2BaAnalyzerRaCommonReportFifoDebug_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_BASE_ADDR 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoBaseAddr : 22; //Rate adaptive user report FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2BaAnalyzerRaUserReportFifoBaseAddr_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEPTH_MINUS_ONE 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoDepthMinusOne : 9; //Rate adaptive user report FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2BaAnalyzerRaUserReportFifoDepthMinusOne_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_CLEAR_STRB 0x8C */
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
} RegB2BaAnalyzerRaUserReportFifoClearStrb_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_RD_ENTRIES_NUM 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRaUserReportFifoRdEntriesNum : 10; //Rate adaptive user report FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerRaUserReportFifoRdEntriesNum_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_NUM_ENTRIES_COUNT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaUserReportFifoNumEntriesCount : 10; //Rate adaptive user report FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB2BaAnalyzerRaUserReportFifoNumEntriesCount_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG0 0x98 */
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
} RegB2BaAnalyzerRaUserReportFifoDebug0_u;

/*REG_B2_BA_ANALYZER_RA_USER_REPORT_FIFO_DEBUG1 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRaNonFirstUserReportFifoFullDropCtr : 9; //Rate adaptive non first user report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2BaAnalyzerRaUserReportFifoDebug1_u;

/*REG_B2_BA_ANALYZER_RTS_CFG 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crRtsRetryLimit : 4; //RTS retry limit. , When the sum of RTS counter and Retry counter exceeds or equals to this limit PD should be discarded. , '0' means unlimited., reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2BaAnalyzerRtsCfg_u;

/*REG_B2_BA_ANALYZER_ETH_TYPE_OVERHEAD 0xA4 */
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
} RegB2BaAnalyzerEthTypeOverhead_u;

/*REG_B2_BA_ANALYZER_STA_MODE_CFG0 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStaModeAid : 11; //My AID in Station mode., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB2BaAnalyzerStaModeCfg0_u;

/*REG_B2_BA_ANALYZER_STA_MODE_CFG1 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crStaModeDropMyAidTidRepeated : 1; //When set to '1' broadcast MBA in station mode is dropped if my AID in combination with TID is repeated. , When set to '0' only first AID/TID encountered is written to CB., reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2BaAnalyzerStaModeCfg1_u;

/*REG_B2_BA_ANALYZER_FIFO_STATUS0 0xB0 */
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
} RegB2BaAnalyzerFifoStatus0_u;

/*REG_B2_BA_ANALYZER_FIFO_STATUS1 0xB4 */
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
} RegB2BaAnalyzerFifoStatus1_u;

/*REG_B2_BA_ANALYZER_FSM_STATUS0 0xB8 */
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
} RegB2BaAnalyzerFsmStatus0_u;

/*REG_B2_BA_ANALYZER_FSM_STATUS1 0xBC */
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
} RegB2BaAnalyzerFsmStatus1_u;

/*REG_B2_BA_ANALYZER_LOGGER_REG 0xC0 */
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
} RegB2BaAnalyzerLoggerReg_u;

/*REG_B2_BA_ANALYZER_LOGGER_FILTER0 0xC4 */
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
} RegB2BaAnalyzerLoggerFilter0_u;

/*REG_B2_BA_ANALYZER_LOGGER_FILTER1 0xC8 */
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
} RegB2BaAnalyzerLoggerFilter1_u;

/*REG_B2_BA_ANALYZER_LOGGER_STATISTICS_REG 0xCC */
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
} RegB2BaAnalyzerLoggerStatisticsReg_u;

/*REG_B2_BA_ANALYZER_CALC_CNT_SET_TO_ZERO_STATUS 0xD0 */
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
} RegB2BaAnalyzerCalcCntSetToZeroStatus_u;

/*REG_B2_BA_ANALYZER_BA_ANALYZER_SPARE 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crBaAnalyzerSpare : 32; //ba_analyzer_spare, reset value: 0x88888888, access type: RW
	} bitFields;
} RegB2BaAnalyzerBaAnalyzerSpare_u;



#endif // _B2_BA_ANALYZER_REGS_H_
