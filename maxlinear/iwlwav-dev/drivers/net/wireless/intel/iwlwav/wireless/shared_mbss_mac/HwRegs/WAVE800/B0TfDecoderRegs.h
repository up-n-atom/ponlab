
/***********************************************************************************
File:				B0TfDecoderRegs.h
Module:				b0TfDecoder
SOC Revision:		latest
Generated at:       2024-06-26 12:54:33
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TF_DECODER_REGS_H_
#define _B0_TF_DECODER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TF_DECODER_BASE_ADDRESS                                   MEMORY_MAP_UNIT_14_BASE_ADDRESS
#define	REG_B0_TF_DECODER_TF_DEC_PARAMS_0                                (B0_TF_DECODER_BASE_ADDRESS + 0x0)
#define	REG_B0_TF_DECODER_TF_DEC_PARAMS_1                                (B0_TF_DECODER_BASE_ADDRESS + 0x4)
#define	REG_B0_TF_DECODER_TF_DEC_PARAMS_2                                (B0_TF_DECODER_BASE_ADDRESS + 0x8)
#define	REG_B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS                      (B0_TF_DECODER_BASE_ADDRESS + 0xC)
#define	REG_B0_TF_DECODER_TF_DEC_TCR_BASE_ADDRESS                        (B0_TF_DECODER_BASE_ADDRESS + 0x10)
#define	REG_B0_TF_DECODER_TF_DEC_CONTROL_BITS                            (B0_TF_DECODER_BASE_ADDRESS + 0x20)
#define	REG_B0_TF_DECODER_TF_DEC_LOGGER_CONTROL                          (B0_TF_DECODER_BASE_ADDRESS + 0x40)
#define	REG_B0_TF_DECODER_TF_DEC_LOGGER_STATUS                           (B0_TF_DECODER_BASE_ADDRESS + 0x44)
#define	REG_B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS                        (B0_TF_DECODER_BASE_ADDRESS + 0x50)
#define	REG_B0_TF_DECODER_TF_DEC_INT_ERROR_EN                            (B0_TF_DECODER_BASE_ADDRESS + 0x54)
#define	REG_B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR                         (B0_TF_DECODER_BASE_ADDRESS + 0x58)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_STATS_CONTROL                       (B0_TF_DECODER_BASE_ADDRESS + 0x60)
#define	REG_B0_TF_DECODER_DBG_RXC_TF_COUNT_0                             (B0_TF_DECODER_BASE_ADDRESS + 0x64)
#define	REG_B0_TF_DECODER_DBG_RXC_TF_COUNT_1                             (B0_TF_DECODER_BASE_ADDRESS + 0x68)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_STATS                               (B0_TF_DECODER_BASE_ADDRESS + 0x6C)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0                 (B0_TF_DECODER_BASE_ADDRESS + 0x80)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1                 (B0_TF_DECODER_BASE_ADDRESS + 0x84)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0    (B0_TF_DECODER_BASE_ADDRESS + 0x88)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0           (B0_TF_DECODER_BASE_ADDRESS + 0x8C)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO           (B0_TF_DECODER_BASE_ADDRESS + 0x90)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0                (B0_TF_DECODER_BASE_ADDRESS + 0x94)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1                (B0_TF_DECODER_BASE_ADDRESS + 0x98)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2                (B0_TF_DECODER_BASE_ADDRESS + 0x9C)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3                (B0_TF_DECODER_BASE_ADDRESS + 0xA0)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4                (B0_TF_DECODER_BASE_ADDRESS + 0xA4)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5                (B0_TF_DECODER_BASE_ADDRESS + 0xA8)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6                (B0_TF_DECODER_BASE_ADDRESS + 0xAC)
#define	REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7                (B0_TF_DECODER_BASE_ADDRESS + 0xB0)
#define	REG_B0_TF_DECODER_TF_DEC_SPARE_REGISTER                          (B0_TF_DECODER_BASE_ADDRESS + 0xD0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TF_DECODER_TF_DEC_PARAMS_0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 systemBw : 2; //System BW configuration. It is used for verifying that the UL BW (which is extracted from the TF) is never greater than the System BW. It is coded as follows: 0 - 20MHz; 1 - 40MHz; 2 --------- 80MHz; 3 --------- 160MHz, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 primary20 : 3; //Primary 20 Sub-Band configuration. It represents the spectral location of the Primary Channel within the full BW supported by the system. It is used by the RU Conversion process. It is the SW responsibility to configure legal values (documneted in the spec), reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB0TfDecoderTfDecParams0_u;

/*REG_B0_TF_DECODER_TF_DEC_PARAMS_1 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 resourceRequestBufferThresholdExp : 8; //Exponent of Threshold used for generating FEEDBACK_STATUS field within the TCR in case of NFRP. This number is the exponent. The actual Threshold in bytes is 2^(Resource Request Buffer Threshold Exp), reset value: 0x8, access type: RW
		uint32 nfrpSupport : 1; //This configuration defines whether NFRP is supported. If NFRP TF is received, but the configuration is disabled, then the TF shall be aborted, reset value: 0x1, access type: RW
		uint32 multiTidInfoCalcFullModeMyAid : 1; //This configuration controls the analysis of TID field in case of Multi-TID MU-BAR. It is used only when the encountered AID matches the AID to search for and when TID > 7. , 0 - TID analysis is done based on TID[2:0] (TID[3] is ignored) , 1 ----- TID analysis is done based on TID[3:0], reset value: 0x1, access type: RW
		uint32 multiTidInfoGt7Abort : 1; //This configuration controls the analysis of TID_INFO field in case of Multi-TID MU-BAR, when TID_INFO > 7. It is used when the encountered AID is different than the AID to search for. , 0 -TF Decoding continues to the next user (if available), based on multi_tid_info_calc_full_mode configuration , 1 -If TID_INFO > 7, then the current TF is aborted, reset value: 0x0, access type: RW
		uint32 multiTidInfoCalcFullMode : 1; //This configuration controls the analysis of TID_INFO field in case of Multi-TID MU-BAR, when TID_INFO > 7 and MULTI_TID_INFO_GT7_ABORT is 0. It is used when the encountered AID is different than the AID to search for. , 0 - Jumping to the next user is done based on TID_INFO[2:0] , 1 - Jumping to the next user is done based on TID_INFO[3:0], reset value: 0x1, access type: RW
		uint32 unicastCheckMyAid : 1; //This configuration is used in case of Unicast. It controls whether to compare the extracted AID (from the TF) against MyAID in case of Unicast. , 0 - The extracted AID is ignored and not compared against MyAID. Processing continues normally. , 1 - The extracted AID is compared against MyAID. In case of a mismatch, the current TF is aborted, reset value: 0x1, access type: RW
		uint32 rfMaxOffset : 8; //This field designates the offset from MAX Tx Power within the TX Power Calculation process. It is used in case a defualt TX Power is selected. The format is identical to that of the TX Power Table: , - 8bit, Signed value , - dBm units, LSB=0.5dB, reset value: 0x30, access type: RW
		uint32 applyDefaultRfPower : 1; //Enable bit for using Default RF Power (within TX Power Calculation process). , 0 --------- TF Decoder applies RF Power in TCR according to UL Power calculations , 1 --------- TF Decoder applies RF Power in TCR according to Max_Tx_pwr value (MCS and BW dependent) from TX Power Table, reset value: 0x0, access type: RW
		uint32 applyCl : 1; //This field controls the CL value within the TCR. , 0 --------- TF Decoder shall apply Tx loop mode=0 (CL) in TCR , 1 - TF Decoder shall apply Tx loop mode=0 (CL) in TCR, reset value: 0x1, access type: RW
		uint32 applyTxBf : 1; //This field controls the TxBF value within the TCR. , 0 --------- TxBF is set to 0 , 1 - TxBF is calculated based on the STA DB parameters, reset value: 0x0, access type: RW
		uint32 forceTx : 8; //This is a bitmap configuration per Trigger Type. The Force_TX field within the TCR is updated with the value of this configuration, based on the Trigger Type. Note that only 8 bits are allocated, since Trigger Types of 8-15 are invalid., reset value: 0xff, access type: RW
	} bitFields;
} RegB0TfDecoderTfDecParams1_u;

/*REG_B0_TF_DECODER_TF_DEC_PARAMS_2 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 totalNumOfSts : 3; //This field sets the threshold for (Starting SS + NSS) in case of UL MU-MIMO, reset value: 0x3, access type: RW
		uint32 reserved0 : 1;
		uint32 maxNss : 2; //This field sets the threshold for NSS in case of both OFDMA and UL MU-MIMO, reset value: 0x3, access type: RW
		uint32 maxMcs : 2; //This field defines whether values of 12-13 are allowed (values of 0-11 are always legal).  0 --------- Max value is 11;   1 --------- Max value is 12;   2 --------- Max value is 13;   3 --------- Reserved, reset value: 0x0, access type: RW
		uint32 reserved1 : 24;
	} bitFields;
} RegB0TfDecoderTfDecParams2_u;

/*REG_B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 antennaSelection : 16; //This field is used as the source of antenna_selection field within the TCR and also for calculating the Number of Tx Antennas (within Tx Power Calculation process). Each 2 bits designate an antenna, reset value: 0x0, access type: RW
		uint32 ant0Boost : 2; //This field is used as the source of ant0_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant1Boost : 2; //This field is used as the source of ant1_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant2Boost : 2; //This field is used as the source of ant2_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant3Boost : 2; //This field is used as the source of ant3_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant4Boost : 2; //This field is used as the source of ant4_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant5Boost : 2; //This field is used as the source of ant5_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant6Boost : 2; //This field is used as the source of ant6_boost field within the TCR, reset value: 0x0, access type: RW
		uint32 ant7Boost : 2; //This field is used as the source of ant7_boost field within the TCR, reset value: 0x0, access type: RW
	} bitFields;
} RegB0TfDecoderTfDecTcrAntennaParams_u;

/*REG_B0_TF_DECODER_TF_DEC_TCR_BASE_ADDRESS 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrBaseAddress : 22; //Byte Address of the TCR location within the Tx Cyclic buffer, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB0TfDecoderTfDecTcrBaseAddress_u;

/*REG_B0_TF_DECODER_TF_DEC_CONTROL_BITS 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecFifoClear : 1; //Write 1 to this field in order to clear TF_DEC FIFO, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TfDecoderTfDecControlBits_u;

/*REG_B0_TF_DECODER_TF_DEC_LOGGER_CONTROL 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecLoggerEn : 1; //Logger Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 tfDecLoggerPriority : 2; //tf_dec Logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegB0TfDecoderTfDecLoggerControl_u;

/*REG_B0_TF_DECODER_TF_DEC_LOGGER_STATUS 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecLoggerIdle : 1; //Indication that the logger is IDLE. Use this indication after the logger is disabled, in order to verify that it has finished any work in progress., reset value: 0x1, access type: RO
		uint32 tfDecLoggerSm : 3; //State of the logger state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TfDecoderTfDecLoggerStatus_u;

/*REG_B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecLengthErrorStatus : 1; //Incoming length is greater than declared MPDU Length , reset value: 0x0, access type: RO
		uint32 tfDecCompressedMuBarTidGt7Status : 1; //TID_INFO > 7 in case of Compressed MU-BAR. Asserted only in case  of FCS_OK, reset value: 0x0, access type: RO
		uint32 tfDecMultiTidMuBarTidGt7Status : 1; //TID_INFO > 7 in case of Multi-TID MU-BAR. This interrupt is set only if MyAID was found and MULTI_TID_INFO_CALC_FULL_MODE_MY_AID configuration is set. Asserted only in case  of FCS_OK, reset value: 0x0, access type: RO
		uint32 tfDecMultiTidMuBarTidNonUniqueStatus : 1; //The same TID arrived more than once in case of Multi-TID MU-BAR. This interrupt is set only if MyAID was found. If MULTI_TID_INFO_CALC_FULL_MODE_MY_AID configuration is not set, then duplication is identified based on identical TID[2:0] only. Asserted only in case  of FCS_OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TfDecoderTfDecIntErrorStatus_u;

/*REG_B0_TF_DECODER_TF_DEC_INT_ERROR_EN 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecLengthErrorEn : 1; //Length Error interrupt enable, reset value: 0x0, access type: RW
		uint32 tfDecCompressedMuBarTidGt7En : 1; //Compressed MU-BAR TID GT7 interrupt enable, reset value: 0x0, access type: RW
		uint32 tfDecMultiTidMuBarTidGt7En : 1; //Multi-TID MU-BAR TID GT7 interrupt enable, reset value: 0x0, access type: RW
		uint32 tfDecMultiTidMuBarTidNonUniqueEn : 1; //Multi-TID MU-BAR TID Non-Unique interrupt enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TfDecoderTfDecIntErrorEn_u;

/*REG_B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tfDecLengthErrorClr : 1; //Length Error interrupt clear, reset value: 0x0, access type: WO
		uint32 tfDecCompressedMuBarTidGt7Clr : 1; //Compressed MU-BAR TID GT7 interrupt clear, reset value: 0x0, access type: WO
		uint32 tfDecMultiTidMuBarTidGt7Clr : 1; //Multi-TID MU-BAR TID GT7 interrupt clear, reset value: 0x0, access type: WO
		uint32 tfDecMultiTidMuBarTidNonUniqueClr : 1; //Multi-TID MU-BAR TID Non-Unique interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TfDecoderTfDecIntErrorClear_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_STATS_CONTROL 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcTfCountClear : 1; //Write 1 to this field in order to clear all rxc_tf_count counters, reset value: 0x0, access type: WO
		uint32 airTimeCalcAbortCountClear : 1; //Write 1 to this field in order to clear num_of_air_time_calculator_aborts counter, reset value: 0x0, access type: WO
		uint32 tfDecFifoOccupancyMaxClear : 1; //Write 1 to this field in order to reset tf_dec_fifo_occupancy_max, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TfDecoderDbgTfDecStatsControl_u;

/*REG_B0_TF_DECODER_DBG_RXC_TF_COUNT_0 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcNumOfTotalFrames : 16; //Total number of frames that were received from the RXC. This counter is reset by writing 1 to rxc_tf_count_clear, reset value: 0x0, access type: RO
		uint32 rxcNumOfGoodFrames : 16; //Number of good frames (frames with FCS OK) that were received from the RXC. This counter is reset by writing 1 to rxc_tf_count_clear, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgRxcTfCount0_u;

/*REG_B0_TF_DECODER_DBG_RXC_TF_COUNT_1 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcNumOfBadFrames : 16; //Number of bad frames (frames with Bad FCS) that were received from the RXC. This counter is reset by writing 1 to rxc_tf_count_clear, reset value: 0x0, access type: RO
		uint32 rxcNumOfVerifiedFrames : 16; //Number of frames that were declared as valid by the TF Decoder out of the frames with FCS OK. This counter is reset by writing 1 to rxc_tf_count_clear, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgRxcTfCount1_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_STATS 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 numOfBadFcsBeforeGoodInRxSession : 12; //Number of bad frames (frames with Bad FCS) that were received from the RXC before receiving a good frame (frame with FCS OK) in each Rx session. This counter restarts every rx session (rx_session_post_status_stb restarts the counter), reset value: 0x0, access type: RO
		uint32 numOfAirTimeCalculatorAborts : 16; //Number of times the Air Time Calculator was aborted (i.e. clear command was issued while still processing). This counter is reset by writing 1 to air_time_calc_abort_count_clear, reset value: 0x0, access type: RO
		uint32 tfDecFifoOccupancyMax : 3; //Maximal occupancy of the TF Decoder input FIFO. This indication is reset by writing 1 to tf_dec_occupancy_max_clear, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB0TfDecoderDbgTfDecStats_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 triggerType : 4; //Trigger Type field within Common Info, reset value: 0x0, access type: RO
		uint32 ulLength : 12; //UL Length field within Common Info, reset value: 0x0, access type: RO
		uint32 moreTf : 1; //More TF field within Common Info, reset value: 0x0, access type: RO
		uint32 csRequired : 1; //CS Required field within Common Info, reset value: 0x0, access type: RO
		uint32 ulBw : 2; //UL BW field within Common Info, reset value: 0x0, access type: RO
		uint32 giLtfType : 2; //GI & LTF Type field within Common Info, reset value: 0x0, access type: RO
		uint32 muMimoLtfMode : 1; //MU-MIMO LTF Mode field within Common Info, reset value: 0x0, access type: RO
		uint32 numHeltfMidamblePeriodicity : 3; //Number Of HE-LTF Symbols And Midamble Periodicity field within Common Info, reset value: 0x0, access type: RO
		uint32 ulStbc : 1; //UL STBC field within Common Info, reset value: 0x0, access type: RO
		uint32 ldpcExtraSymbolSegment : 1; //LDPC Extra Symbol Segment field within Common Info, reset value: 0x0, access type: RO
		uint32 count0 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgTfDecTfDbCommonInfo0_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 apTxPower : 6; //AP Tx Power field within Common Info, reset value: 0x0, access type: RO
		uint32 preFecPaddingFactor : 2; //Pre-FEC Padding Factor field within Common Info, reset value: 0x0, access type: RO
		uint32 peDisambiguity : 1; //PE Disambiguity field within Common Info, reset value: 0x0, access type: RO
		uint32 ulSpatialReuse : 16; //UL Spatial Reuse field within Common Info, reset value: 0x0, access type: RO
		uint32 doppler : 1; //Doppler field within Common Info, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 count1 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgTfDecTfDbCommonInfo1_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulHeSigA2Reserved : 9; //UL HE-SIG-A2 Reserved field within Common Info, reset value: 0x0, access type: RO
		uint32 aid12 : 12; //AID12 field within General User Info. In case of NFRP it represents Starting AID, reset value: 0x0, access type: RO
		uint32 ulFecCodingType : 1; // UL FEC Coding Type field within General User Info, reset value: 0x0, access type: RO
		uint32 ulMcs : 4; //UL MCS field within General User Info, reset value: 0x0, access type: RO
		uint32 ulDcm : 1; //UL DCM field within General User Info, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 count2 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgTfDecTfDbCommonGeneralUserInfo0_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ruAllocation : 8; //RU Allocation field within General User Info, reset value: 0x0, access type: RO
		uint32 startingSpatialStream : 3; //Starting Spatial tream field within General User Info, reset value: 0x0, access type: RO
		uint32 numberOfSpatialStreams : 3; //Number of Spatial Streams field within General User Info, reset value: 0x0, access type: RO
		uint32 ulTargetRssi : 7; //UL Target RSSI field within General User Info (also valid in case of NFRP), reset value: 0x0, access type: RO
		uint32 nfrpFeedbackType : 4; //NFRP Feedback Type field within General User Info. Valid in case of NFRP, reset value: 0x0, access type: RO
		uint32 nfrpMultiplexingFlag : 1; //NFRP Multiplexing Flag field within General User Info. Valid in case of NFRP, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 count3 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgTfDecTfDbGeneralUserInfo0_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 basicTriggerMpduMuSpacingFactor : 2; //MPDU MU Spacing Factor field within Trigger Dependent User info of Basic Trigger, reset value: 0x0, access type: RO
		uint32 basicTriggerTidAggLimit : 3; //TID Agg Limit field within Trigger Dependent User info of Basic Trigger, reset value: 0x0, access type: RO
		uint32 basicTriggerPreferredAc : 2; //Preferred AC field within Trigger Dependent User info of Basic Trigger, reset value: 0x0, access type: RO
		uint32 bfrpFbSgmntRetransmissionBitmap : 8; //Feedback Segment Retransmission Bitmap field within Trigger Dependent User info of BFRP, reset value: 0x0, access type: RO
		uint32 muBarControlBarAckPolicy : 1; //BAR Ack Policy field within Trigger Dependent User info of MU-BAR, reset value: 0x0, access type: RO
		uint32 muBarControlBarType : 4; //BAR Type field within Trigger Dependent User info of MU-BAR, reset value: 0x0, access type: RO
		uint32 muBarControlTidInfo : 4; //TID Info field within Trigger Dependent User info of MU-BAR, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 count4 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TfDecoderDbgTfDecTfDbDependentUserInfo_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber0 : 4; //Fragment Number field of TID 0 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber0 : 12; //Starting Sequence Number field of TID 0 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count5 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation0_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber1 : 4; //Fragment Number field of TID 1 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber1 : 12; //Starting Sequence Number field of TID 1 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count6 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation1_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber2 : 4; //Fragment Number field of TID 2 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber2 : 12; //Starting Sequence Number field of TID 2 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count7 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation2_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber3 : 4; //Fragment Number field of TID 3 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber3 : 12; //Starting Sequence Number field of TID 3 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count8 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation3_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber4 : 4; //Fragment Number field of TID 4 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber4 : 12; //Starting Sequence Number field of TID 4 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count9 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation4_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber5 : 4; //Fragment Number field of TID 5 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber5 : 12; //Starting Sequence Number field of TID 5 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count10 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation5_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber6 : 4; //Fragment Number field of TID 6 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber6 : 12; //Starting Sequence Number field of TID 6 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count11 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation6_u;

/*REG_B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentNumber7 : 4; //Fragment Number field of TID 7 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 startingSequenceNumber7 : 12; //Starting Sequence Number field of TID 7 within Bar Information of MU-BAR. Valid in case of MU-BAR, reset value: 0x0, access type: RO
		uint32 count12 : 4; //Incremental counter of frames with FCS OK, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0TfDecoderDbgTfDecMuBarInformation7_u;

/*REG_B0_TF_DECODER_TF_DEC_SPARE_REGISTER 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegister : 16; //Spare Register, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TfDecoderTfDecSpareRegister_u;

//========================================
/* REG_TF_DECODER_TF_DEC_PARAMS_0 0x0 */
#define B0_TF_DECODER_TF_DEC_PARAMS_0_FIELD_SYSTEM_BW_MASK                                                        0x00000003
#define B0_TF_DECODER_TF_DEC_PARAMS_0_FIELD_PRIMARY_20_MASK                                                       0x00000070
//========================================
/* REG_TF_DECODER_TF_DEC_PARAMS_1 0x4 */
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_RESOURCE_REQUEST_BUFFER_THRESHOLD_EXP_MASK                            0x000000FF
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_NFRP_SUPPORT_MASK                                                     0x00000100
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_MULTI_TID_INFO_CALC_FULL_MODE_MY_AID_MASK                             0x00000200
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_MULTI_TID_INFO_GT7_ABORT_MASK                                         0x00000400
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_MULTI_TID_INFO_CALC_FULL_MODE_MASK                                    0x00000800
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_UNICAST_CHECK_MY_AID_MASK                                             0x00001000
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_RF_MAX_OFFSET_MASK                                                    0x001FE000
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_APPLY_DEFAULT_RF_POWER_MASK                                           0x00200000
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_APPLY_CL_MASK                                                         0x00400000
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_APPLY_TX_BF_MASK                                                      0x00800000
#define B0_TF_DECODER_TF_DEC_PARAMS_1_FIELD_FORCE_TX_MASK                                                         0xFF000000
//========================================
/* REG_TF_DECODER_TF_DEC_PARAMS_2 0x8 */
#define B0_TF_DECODER_TF_DEC_PARAMS_2_FIELD_TOTAL_NUM_OF_STS_MASK                                                 0x00000007
#define B0_TF_DECODER_TF_DEC_PARAMS_2_FIELD_MAX_NSS_MASK                                                          0x00000030
#define B0_TF_DECODER_TF_DEC_PARAMS_2_FIELD_MAX_MCS_MASK                                                          0x000000C0
//========================================
/* REG_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS 0xC */
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANTENNA_SELECTION_MASK                                      0x0000FFFF
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT0_BOOST_MASK                                             0x00030000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT1_BOOST_MASK                                             0x000C0000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT2_BOOST_MASK                                             0x00300000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT3_BOOST_MASK                                             0x00C00000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT4_BOOST_MASK                                             0x03000000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT5_BOOST_MASK                                             0x0C000000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT6_BOOST_MASK                                             0x30000000
#define B0_TF_DECODER_TF_DEC_TCR_ANTENNA_PARAMS_FIELD_ANT7_BOOST_MASK                                             0xC0000000
//========================================
/* REG_TF_DECODER_TF_DEC_TCR_BASE_ADDRESS 0x10 */
#define B0_TF_DECODER_TF_DEC_TCR_BASE_ADDRESS_FIELD_TCR_BASE_ADDRESS_MASK                                         0x003FFFFF
//========================================
/* REG_TF_DECODER_TF_DEC_CONTROL_BITS 0x20 */
#define B0_TF_DECODER_TF_DEC_CONTROL_BITS_FIELD_TF_DEC_FIFO_CLEAR_MASK                                            0x00000001
//========================================
/* REG_TF_DECODER_TF_DEC_LOGGER_CONTROL 0x40 */
#define B0_TF_DECODER_TF_DEC_LOGGER_CONTROL_FIELD_TF_DEC_LOGGER_EN_MASK                                           0x00000001
#define B0_TF_DECODER_TF_DEC_LOGGER_CONTROL_FIELD_TF_DEC_LOGGER_PRIORITY_MASK                                     0x00000030
//========================================
/* REG_TF_DECODER_TF_DEC_LOGGER_STATUS 0x44 */
#define B0_TF_DECODER_TF_DEC_LOGGER_STATUS_FIELD_TF_DEC_LOGGER_IDLE_MASK                                          0x00000001
#define B0_TF_DECODER_TF_DEC_LOGGER_STATUS_FIELD_TF_DEC_LOGGER_SM_MASK                                            0x0000000E
//========================================
/* REG_TF_DECODER_TF_DEC_INT_ERROR_STATUS 0x50 */
#define B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS_FIELD_TF_DEC_LENGTH_ERROR_STATUS_MASK                               0x00000001
#define B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS_FIELD_TF_DEC_COMPRESSED_MU_BAR_TID_GT7_STATUS_MASK                  0x00000002
#define B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_GT7_STATUS_MASK                   0x00000004
#define B0_TF_DECODER_TF_DEC_INT_ERROR_STATUS_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_NON_UNIQUE_STATUS_MASK            0x00000008
//========================================
/* REG_TF_DECODER_TF_DEC_INT_ERROR_EN 0x54 */
#define B0_TF_DECODER_TF_DEC_INT_ERROR_EN_FIELD_TF_DEC_LENGTH_ERROR_EN_MASK                                       0x00000001
#define B0_TF_DECODER_TF_DEC_INT_ERROR_EN_FIELD_TF_DEC_COMPRESSED_MU_BAR_TID_GT7_EN_MASK                          0x00000002
#define B0_TF_DECODER_TF_DEC_INT_ERROR_EN_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_GT7_EN_MASK                           0x00000004
#define B0_TF_DECODER_TF_DEC_INT_ERROR_EN_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_NON_UNIQUE_EN_MASK                    0x00000008
//========================================
/* REG_TF_DECODER_TF_DEC_INT_ERROR_CLEAR 0x58 */
#define B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR_FIELD_TF_DEC_LENGTH_ERROR_CLR_MASK                                   0x00000001
#define B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR_FIELD_TF_DEC_COMPRESSED_MU_BAR_TID_GT7_CLR_MASK                      0x00000002
#define B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_GT7_CLR_MASK                       0x00000004
#define B0_TF_DECODER_TF_DEC_INT_ERROR_CLEAR_FIELD_TF_DEC_MULTI_TID_MU_BAR_TID_NON_UNIQUE_CLR_MASK                0x00000008
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_STATS_CONTROL 0x60 */
#define B0_TF_DECODER_DBG_TF_DEC_STATS_CONTROL_FIELD_RXC_TF_COUNT_CLEAR_MASK                                      0x00000001
#define B0_TF_DECODER_DBG_TF_DEC_STATS_CONTROL_FIELD_AIR_TIME_CALC_ABORT_COUNT_CLEAR_MASK                         0x00000002
#define B0_TF_DECODER_DBG_TF_DEC_STATS_CONTROL_FIELD_TF_DEC_FIFO_OCCUPANCY_MAX_CLEAR_MASK                         0x00000004
//========================================
/* REG_TF_DECODER_DBG_RXC_TF_COUNT_0 0x64 */
#define B0_TF_DECODER_DBG_RXC_TF_COUNT_0_FIELD_RXC_NUM_OF_TOTAL_FRAMES_MASK                                       0x0000FFFF
#define B0_TF_DECODER_DBG_RXC_TF_COUNT_0_FIELD_RXC_NUM_OF_GOOD_FRAMES_MASK                                        0xFFFF0000
//========================================
/* REG_TF_DECODER_DBG_RXC_TF_COUNT_1 0x68 */
#define B0_TF_DECODER_DBG_RXC_TF_COUNT_1_FIELD_RXC_NUM_OF_BAD_FRAMES_MASK                                         0x0000FFFF
#define B0_TF_DECODER_DBG_RXC_TF_COUNT_1_FIELD_RXC_NUM_OF_VERIFIED_FRAMES_MASK                                    0xFFFF0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_STATS 0x6C */
#define B0_TF_DECODER_DBG_TF_DEC_STATS_FIELD_NUM_OF_BAD_FCS_BEFORE_GOOD_IN_RX_SESSION_MASK                        0x00000FFF
#define B0_TF_DECODER_DBG_TF_DEC_STATS_FIELD_NUM_OF_AIR_TIME_CALCULATOR_ABORTS_MASK                               0x0FFFF000
#define B0_TF_DECODER_DBG_TF_DEC_STATS_FIELD_TF_DEC_FIFO_OCCUPANCY_MAX_MASK                                       0x70000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0 0x80 */
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_TRIGGER_TYPE_MASK                                      0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_UL_LENGTH_MASK                                         0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_MORE_TF_MASK                                           0x00010000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_CS_REQUIRED_MASK                                       0x00020000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_UL_BW_MASK                                             0x000C0000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_GI_LTF_TYPE_MASK                                       0x00300000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_MU_MIMO_LTF_MODE_MASK                                  0x00400000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_NUM_HELTF_MIDAMBLE_PERIODICITY_MASK                    0x03800000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_UL_STBC_MASK                                           0x04000000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_LDPC_EXTRA_SYMBOL_SEGMENT_MASK                         0x08000000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_0_FIELD_COUNT_0_MASK                                           0xF0000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1 0x84 */
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_AP_TX_POWER_MASK                                       0x0000003F
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_PRE_FEC_PADDING_FACTOR_MASK                            0x000000C0
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_PE_DISAMBIGUITY_MASK                                   0x00000100
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_UL_SPATIAL_REUSE_MASK                                  0x01FFFE00
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_DOPPLER_MASK                                           0x02000000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_INFO_1_FIELD_COUNT_1_MASK                                           0xF0000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0 0x88 */
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_UL_HE_SIG_A2_RESERVED_MASK                0x000001FF
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_AID12_MASK                                0x001FFE00
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_UL_FEC_CODING_TYPE_MASK                   0x00200000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_UL_MCS_MASK                               0x03C00000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_UL_DCM_MASK                               0x04000000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_COMMON_GENERAL_USER_INFO_0_FIELD_COUNT_2_MASK                              0xF0000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0 0x8C */
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_RU_ALLOCATION_MASK                               0x000000FF
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_STARTING_SPATIAL_STREAM_MASK                     0x00000700
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_NUMBER_OF_SPATIAL_STREAMS_MASK                   0x00003800
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_UL_TARGET_RSSI_MASK                              0x001FC000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_NFRP_FEEDBACK_TYPE_MASK                          0x01E00000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_NFRP_MULTIPLEXING_FLAG_MASK                      0x02000000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_GENERAL_USER_INFO_0_FIELD_COUNT_3_MASK                                     0xF0000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO 0x90 */
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_BASIC_TRIGGER_MPDU_MU_SPACING_FACTOR_MASK        0x00000003
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_BASIC_TRIGGER_TID_AGG_LIMIT_MASK                 0x0000001C
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_BASIC_TRIGGER_PREFERRED_AC_MASK                  0x00000060
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_BFRP_FB_SGMNT_RETRANSMISSION_BITMAP_MASK         0x00007F80
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_MU_BAR_CONTROL_BAR_ACK_POLICY_MASK               0x00008000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_MU_BAR_CONTROL_BAR_TYPE_MASK                     0x000F0000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_MU_BAR_CONTROL_TID_INFO_MASK                     0x00F00000
#define B0_TF_DECODER_DBG_TF_DEC_TF_DB_DEPENDENT_USER_INFO_FIELD_COUNT_4_MASK                                     0xF0000000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0 0x94 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0_FIELD_FRAGMENT_NUMBER_0_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0_FIELD_STARTING_SEQUENCE_NUMBER_0_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_0_FIELD_COUNT_5_MASK                                          0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1 0x98 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1_FIELD_FRAGMENT_NUMBER_1_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1_FIELD_STARTING_SEQUENCE_NUMBER_1_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_1_FIELD_COUNT_6_MASK                                          0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2 0x9C */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2_FIELD_FRAGMENT_NUMBER_2_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2_FIELD_STARTING_SEQUENCE_NUMBER_2_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_2_FIELD_COUNT_7_MASK                                          0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3 0xA0 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3_FIELD_FRAGMENT_NUMBER_3_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3_FIELD_STARTING_SEQUENCE_NUMBER_3_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_3_FIELD_COUNT_8_MASK                                          0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4 0xA4 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4_FIELD_FRAGMENT_NUMBER_4_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4_FIELD_STARTING_SEQUENCE_NUMBER_4_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_4_FIELD_COUNT_9_MASK                                          0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5 0xA8 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5_FIELD_FRAGMENT_NUMBER_5_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5_FIELD_STARTING_SEQUENCE_NUMBER_5_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_5_FIELD_COUNT_10_MASK                                         0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6 0xAC */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6_FIELD_FRAGMENT_NUMBER_6_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6_FIELD_STARTING_SEQUENCE_NUMBER_6_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_6_FIELD_COUNT_11_MASK                                         0x000F0000
//========================================
/* REG_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7 0xB0 */
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7_FIELD_FRAGMENT_NUMBER_7_MASK                                0x0000000F
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7_FIELD_STARTING_SEQUENCE_NUMBER_7_MASK                       0x0000FFF0
#define B0_TF_DECODER_DBG_TF_DEC_MU_BAR_INFORMATION_7_FIELD_COUNT_12_MASK                                         0x000F0000
//========================================
/* REG_TF_DECODER_TF_DEC_SPARE_REGISTER 0xD0 */
#define B0_TF_DECODER_TF_DEC_SPARE_REGISTER_FIELD_SPARE_REGISTER_MASK                                             0x0000FFFF


#endif // _TF_DECODER_REGS_H_
