/************************************************************************************
*
*	File:				Statistics_Descriptors.h
*	Description:	
*	COPYRIGHT:
*		Copyright(c) 2020, MaxLinear, Inc.
*		Copyright 2016 - 2020 Intel Corporation
*
*		All rights are strictly reserved. Reproduction or divulgence in any
*		form whatsoever is not permitted without written authority from the
*		copyright owner. Issued by Intel Israel Ltd
*
**************************************************************************************/
#ifndef _STATISTICS_DESC_WAVE700_H_
#define _STATISTICS_DESC_WAVE700_H_
  
#include "Statistics_Descriptors_Additional.h"
#include "mhi_umi.h"

#ifndef CPU_ARC

//TODO:WLANRTSYS-44066 -> Think where is the best place to share those values between fw and driver in order to avoid missalignments 
#define NUM_OF_TIDS              			(8)
#define RATES_BIT_MASK_SIZE     			(3)
#define TX_MU_GROUPS           				(16)
#define MU_NUM_OF_STATS       				(7)
#define PTA_STATE_NUM_OF_STATES 			(14)
#define PTA_EVENT_NUM_OF_EVENTS				(18)
#define PTA_RADIO_NUM_OF_TYPES  			(2)
#define ACCESS_CATEGORY_NUM					(4)
#define MAX_USP_IN_VHT_GROUP 				(4)
#define NUM_OF_MCS_GEN7						(14)
#define MAX_MCS_GEN7                        (NUM_OF_MCS_GEN7)
#define TWT_MAX_INDIVIDUAL_AGREEMENTS_ALLOWED_PER_STA           (1)
#define TWT_MAX_BROADCAST_MEMBERSHIPS_ALLOWED_PER_STA           (2)
#define MAX_STA_IN_GROUP					(8)
#else
#define MAX_MCS_GEN7                        (NUM_OF_MCS)
#endif //CPU_ARC

// Definitions for the LA MU Statistics
#define MAX_NUM_OF_HE_TWT_GROUPS			(2)
#define MAX_NUM_OF_HE_MU_GROUPS				(24)
#define MAX_NUM_OF_HE_GROUPS				(MAX_NUM_OF_HE_MU_GROUPS + MAX_NUM_OF_HE_TWT_GROUPS)
#define HE_GROUP_MANAGER_INVALID_GROUP_INDEX (MAX_NUM_OF_HE_GROUPS + 1)
#define MAX_NUM_OF_HE_MU_UL_GROUPS			((MAX_NUM_OF_HE_MU_GROUPS >> 1) + MAX_NUM_OF_HE_TWT_GROUPS)
#define MAX_NUM_OF_HE_MU_DL_GROUPS			(MAX_NUM_OF_HE_MU_GROUPS >> 1)

// Definitions for LA MU Groups Statistics
// Consider aligning MAX_REMOVAL_REASONS in driver if changing the value
#define MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS_GEN7 (51) // This number should be the same as STATISTICS_REMOVAL_REASON_TOTAL from StatisticsGroupRemovalReason_e

#ifndef CPU_ARC
typedef uint16		StaId;
#endif

#define MAX_NUM_OF_SP_BAND_0_700 			(128)	// number of SPs in Band_0 for wave700 only  - beware - also defined in twtManager.h
#define MAX_NUM_OF_SP_BAND_1_700			(64)	// number of SPs in Band_1 for wave700 only  - beware - also defined in twtManager.h
#define MAX_NUM_OF_SP_BAND_2_700			(128)	// number of SPs in Band_2 for wave700 only  - beware - also defined in twtManager.h
#define MAX_NUM_OF_MU_STA_STATS_ENTRIES_GEN7C0	(24) // MAX_NUM_OF_HE_MU_UL_GROUPS(12) * MAX_OFDMA_STAS_IN_GROUP(8) * 2 (Due to StatisticsManager not synced with HeGroupManager)
#define MAX_NUM_OF_MIMO_STA_STATS_ENTRIES_GEN7   (96) // MAX_NUM_OF_HE_MU_UL_GROUPS(12) * MAX_MIMO_STAS_IN_GROUP(4)  * 2 (Due to StatisticsManager not synced with HeGroupManager)
#define REDUCED_SPATIAL_STREAMS_SIZE_GEN7        (2) // A reduced number of SPATIAL streams done to reduce the memory footprint of the DL MIMO statistics
#define REDUCED_MCS_SIZE_GEN7                    (4) // A reduced number of MCS done to reduce the memory footprint of the DL MIMO statistics

#ifdef CPU_ARC
#if ((MAX_NUM_OF_SP_BAND_0 != MAX_NUM_OF_SP_BAND_0_700) 		\
	|| (MAX_NUM_OF_SP_BAND_1 != MAX_NUM_OF_SP_BAND_1_700)		\
	|| (MAX_NUM_OF_SP_BAND_2 != MAX_NUM_OF_SP_BAND_2_700)		\
	|| (MAX_NUM_OF_MU_STA_STATS_ENTRIES != MAX_NUM_OF_MU_STA_STATS_ENTRIES_GEN7C0)		\
) 
#error Check which define doesnt match 700 definitions
#endif

#endif


#if !defined MAX
#define MAX(x,y)      		  ((x) > (y) ? (x) : (y))
#endif

#ifdef CPU_ARC
#define HOST_INTERFACE_BYTE_COUNTER_RESET_VALUE				(0x0)

#ifdef WORKAROUND_FOR_HW_BUG_IN_OTFA_BYTE_COUNTER_CALCULATION
#error "Driver and FW byte counter usage is not match for Wave700"
#endif //WORKAROUND_FOR_HW_BUG_IN_OTFA_BYTE_COUNTER_CALCULATION

#else
#define HOST_INTERFACE_BYTE_COUNTER_RESET_VALUE_WAVE700		(0x0)
#endif

#ifdef CPU_ARC
typedef enum
{
	ENTRY_EMPTY             =  0,
	ENTRY_IN_GROUP          =  1,
	ENTRY_TO_REMOVE         =  2,

} MuGroupState_e;
#endif

typedef enum
{
#ifdef CPU_ARC
	RU_SIZE_26_TONE          =  0, //   2 MHz
	RU_SIZE_52_TONE          =  1, //   5 MHz
	RU_SIZE_106_TONE         =  2, //  10 MHz
	RU_SIZE_242_TONE         =  3, //  20 MHz
	RU_SIZE_484_TONE         =  4, //  40 MHz
	RU_SIZE_996_TONE         =  5, //  80 MHz
	RU_SIZE_996X2_TONE       =  6, // 160 MHz
	WAVE600_NUM_OF_RU_SIZES,
#endif
	RU_SIZE_996X4_TONE       =  WAVE600_NUM_OF_RU_SIZES,	// 320 MHz
	RU_SIZE_52_26_TONE       =  8,	//   7 MHz
	RU_SIZE_106_26_TONE      =  9,						//  12 MHz
	RU_SIZE_484_242_TONE     = 10,						//  60 MHz
	RU_SIZE_996_484_TONE     = 11,						// 120 MHz
	RU_SIZE_996_484_242_TONE = 12,						// 140 MHz
	RU_SIZE_996X2_484_TONE   = 13,						// 200 MHz
	RU_SIZE_996X3_TONE       = 14,						// 240 MHz
	RU_SIZE_996X3_484_TONE   = 15,						// 280 MHz
	WAVE700_NUM_OF_RU_SIZES
} RuSizeMapping_e_wave700;
#ifdef CPU_ARC
typedef RuSizeMapping_e_wave700 RuSizeMapping_e;
#endif

typedef struct
{
	uint32 qosByteCountSta[GEN7_MAX_SID][NUM_OF_TIDS];
	uint32 qosTxSta[GEN7_MAX_SID][NUM_OF_TIDS];
	uint32 qosTxVap[GEN7_MAX_VAP][NUM_OF_TIDS];
	uint32 qosByteCountVap[GEN7_MAX_VAP][NUM_OF_TIDS];
} HostIfQosCounters_t_wave700;


typedef struct
{	
	uint32 txInUnicastHd;
	uint32 txInMulticastHd;
	uint32 txInBroadcastHd;
	uint32 txInUnicastNumOfBytes;
	uint32 txInMulticastNumOfBytes;
	uint32 txInBroadcastNumOfBytes;
	uint32 rxOutUnicatHd;
	uint32 rxOutMulticastHd;
	uint32 rxOutBroadcastHd;
	uint32 rxOutUnicastNumOfBytes;
	uint32 rxOutMulticastNumOfBytes;
	uint32 rxOutBroadcastNumOfBytes;
	uint32 agerCount;
}  HostIfCountersPerVap_t_wave700;


typedef struct 
{	
	uint32 rxOutStaNumOfBytes; 		
	uint32 agerPdNoTransmitCountSta;
} HostIfCountersPerSta_t_wave700;


typedef struct
{
	HostIfCountersPerVap_t_wave700 vapCounts[GEN7_MAX_VAP];
	HostIfCountersPerSta_t_wave700 staCounts[GEN7_MAX_SID];
} HostIfCounters_t_wave700;


typedef struct 
{
	uint32 amsdu;
	uint32 amsduBytes;
	uint32 dropCount;
	uint32 mpduInAmpdu;
	uint32 octetsInAmpdu;
	uint32 rxCoorSecurityMismatch;
	uint32 tkipCount;
	uint32 securityFailure;
} RxCoordinatorCountersPerVap_t_wave700;


typedef struct
{
	uint32 									rdCount[GEN7_MAX_SID];
	RxCoordinatorCountersPerVap_t_wave700 	vapCounts[GEN7_MAX_VAP];
} RxCoordinatorCounters_t_wave700;

typedef struct 
{
	uint32 rddelayed;				
	uint32 swUpdateDrop;			
	uint32 rdDuplicateDrop;			
	uint32 missingSn;				
} RxCountersPerSta_t_wave700;


typedef struct 
{
	uint32 mpduUnicastOrMngmnt;
	uint32 mpduRetryCount;
	uint32 dropMpdu;
	uint32 ampdu;
	uint32 mpduTypeNotSupported;
	uint32 replayData;
	uint32 replayMngmnt;
	uint32 bcMcCountVap;
} RxCountersRxppPerVap_t_wave700;


typedef struct 
{
	uint32 barMpduCount;
	uint32 crcErrorCount;
	uint32 delCrcError;			
} RxCountersPerBand_t_wave700;



typedef struct 
{
	uint16 							qosRxSta[GEN7_MAX_SID];
	RxCountersPerSta_t_wave700 		staCounts[GEN7_MAX_SID];
	RxCountersRxppPerVap_t_wave700 	rxppVapCounts[GEN7_MAX_VAP];
	RxCountersPerBand_t_wave700 	bandCounts[GEN7_NUM_OF_BANDS];
} RxCounters_t_wave700;

typedef struct 
{
	uint16 bufStsCnt0;
	uint16 bufStsCnt1;
	uint16 bufStsCnt2;
	uint16 bufStsCnt3;
	uint16 bufStsCnt4;
	uint16 bufStsCnt5;
	uint16 bufStsCnt6;
	uint16 bufStsCnt7;
	uint8 maskSelBitmap;
	uint8 reserved[3];
} UplinkBsrcPerTidCnt_t_wave700;

typedef struct 
{
	UplinkBsrcPerTidCnt_t_wave700 BsrcPerTidCnt[GEN7_MAX_SID];
} UplinkBsrcTidCnt_t_wave700;

typedef struct 
{
	uint32 retryCount;
	uint32 multipleRetryCount;
} BaaCountersRetryPerBand_t_wave700;

typedef struct
{
	uint32 rtsSuccessCount;
	uint32 rtsFailure;
	uint32 transmitStreamRprtMSDUFailed;
	uint32 qosTransmittedFrames;
	uint32 transmittedAmsdu;
	uint32 transmittedOctetsInAmsdu;
	uint32 transmittedAmpdu;
	uint32 transmittedMpduInAmpdu;
	uint32 transmittedOctetsInAmpdu;
	uint32 beamformingFrames;
	uint32 ackFailure;
	uint32 failedAmsdu;
	uint32 retryAmsdu;
	uint32 multipleRetryAmsdu;
	uint32 amsduAckFailure;
	uint32 implicitBarFailure;
	uint32 explicitBarFailure;
	uint32 transmitStreamRprtMultipleRetryCount;
	uint32 transmitBw20;
	uint32 transmitBw40;
	uint32 transmitBw80;
	uint32 transmitBw160;
	uint32 transmitBw320;
} BaaCountersPerVap_t_wave700;

typedef struct 
{
	uint32 mpduFirstRetransmission;		
	uint32 mpduTransmitted;
	uint32 mpduByteTransmitted;			
	uint32 mpduRetransmission;			
	uint32 channelTransmitTime;			
} BaaCountersPerSta_t_wave700;

typedef struct 
{
	BaaCountersRetryPerBand_t_wave700 	retryCounts[GEN7_NUM_OF_BANDS];
	BaaCountersPerVap_t_wave700	 		vapCounts[GEN7_MAX_VAP];
	BaaCountersPerSta_t_wave700 		staCounts[GEN7_MAX_SID];
	uint32 fwMngmntframesSent[GEN7_NUM_OF_BANDS];
	uint32 fwMngmntFramesConfirmed[GEN7_NUM_OF_BANDS];
	uint32 fwMngmntframesRecieved[GEN7_NUM_OF_BANDS];
	uint32 fwctrlFramesSent[GEN7_NUM_OF_BANDS];
	uint32 fwctrlFramesRecieved[GEN7_NUM_OF_BANDS];
	uint32 fwMulticastReplayedPackets[GEN7_NUM_OF_BANDS];
	uint32 fwPairWiseMicFailurePackets[GEN7_NUM_OF_BANDS];
	uint32 groupMicFailurePackets[GEN7_NUM_OF_BANDS];
	uint32 beaconProbeResponsePhyType[GEN7_NUM_OF_BANDS];
	uint32 rxBasicReport[GEN7_NUM_OF_BANDS];
	uint32 txFrameErrorCount[GEN7_NUM_OF_BANDS];
	uint32 rxGroupFrame[GEN7_MAX_VAP];
	uint32 txSenderError[GEN7_MAX_VAP];
} BaaCounters_t_wave700;

typedef struct 
{
	uint32 InitiatorTidStatisticsEventAddbaRequestWasSent[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventAddbaRequestConfiremd[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndValid[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndInvalid[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusFailure[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventAddbaResponseTimeout[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventBaAgreementOpened[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventBarWasSent[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventBarWasDiscarded[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventBarWasAnsweredWithAck[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventBarWasAnsweredWithBa[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventInactivityTimeout[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventDelbaWasSent[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventDelbaReceived[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventCloseTid[GEN7_NUM_OF_BANDS];
	uint32 InitiatorTidStatisticsEventOpenTid[GEN7_NUM_OF_BANDS];
} TsManagerInitiatorTidGlobalStatistics_t_wave700;
#ifdef CPU_ARC
typedef TsManagerInitiatorTidGlobalStatistics_t_wave700 TsManagerInitiatorTidGlobalStatistics_t;
#endif

typedef struct 
{
	uint16 InitiatorStationStatisticsEventRemove[GEN7_NUM_OF_BANDS];
	uint16 InitiatorStationStatisticsEventAdd[GEN7_NUM_OF_BANDS];
	uint16 InitiatorStationStatisticsEventAddbaResponseReceivedWithIllegalTid[GEN7_NUM_OF_BANDS];
	uint16 Reserve[GEN7_NUM_OF_BANDS];
} TsManagerInitiatorStationGlobalStatistics_t_wave700;
#ifdef CPU_ARC
typedef TsManagerInitiatorStationGlobalStatistics_t_wave700 TsManagerInitiatorStationGlobalStatistics_t;
#endif

typedef struct 
{
	uint32 RecipientTidStatisticsEventAddbaRequestAccepted[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventAddbaRequestWasReceivedWithInvalidParameters[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventAddbaResponseWasSentWithStatusSuccess[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventAddbaResponseWasSentWithStatusFailure[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventBaAgreementOpened[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventBaAgreementClosed[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventBarWasReceivedWithBaAgreementOpened[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventInactivityTimeout[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventDelbaReceived[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventDelbaWasSent[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventIllegalAggregationWasReceived[GEN7_NUM_OF_BANDS];
	uint32 RecipientTidStatisticsEventCloseTid[GEN7_NUM_OF_BANDS];
} TsManagerRecipientTidGlobalStatistics_t_wave700;
#ifdef CPU_ARC
typedef TsManagerRecipientTidGlobalStatistics_t_wave700 TsManagerRecipientTidGlobalStatistics_t;
#endif

typedef struct 
{
	uint16 RecipientStationStatisticsEventRemove[GEN7_NUM_OF_BANDS];
	uint16 RecipientStationStatisticsEventAdd[GEN7_NUM_OF_BANDS];
	uint16 RecipientStationStatisticsEventAddbaRequestReceivedWithIllegalTid[GEN7_NUM_OF_BANDS];
	uint16 Reserve[GEN7_NUM_OF_BANDS];
} TsManagerRecipientStationGlobalStatistics_t_wave700;
#ifdef CPU_ARC
typedef TsManagerRecipientStationGlobalStatistics_t_wave700 TsManagerRecipientStationGlobalStatistics_t;
#endif

typedef struct 
{
	uint32 ratesMask[GEN7_MAX_SID][RATES_BIT_MASK_SIZE];			
	uint32 bfReportReceivedCounter[GEN7_NUM_OF_BANDS];
	uint32 protectionSentCounter[GEN7_NUM_OF_BANDS];
	uint32 protectionSucceededCounter[GEN7_NUM_OF_BANDS];
	uint8  DataPhyMode[GEN7_MAX_SID][BW_MAX_G7];			
	uint8  ManagementPhyMode[GEN7_MAX_SID][BW_MAX_G7];		
	uint8  powerData[GEN7_MAX_SID][BW_MAX_G7];				
	uint8  powerManagement[GEN7_MAX_SID][BW_MAX_G7];		
	uint8  antennaSelectionData[GEN7_MAX_SID][BW_MAX_G7];
	uint8  antennaSelectionManagement[GEN7_MAX_SID][BW_MAX_G7];
	uint8  scpData[GEN7_MAX_SID][BW_MAX_G7];
	uint8  stationCapabilities[GEN7_MAX_SID];
	uint8  dataBwLimit[GEN7_MAX_SID];
	uint8  bfModeData[GEN7_MAX_SID];
	uint8  stbcModeData[GEN7_MAX_SID];
	uint8  raState[GEN7_MAX_SID];
	uint8  raStability[GEN7_MAX_SID];
	uint8  nssData[GEN7_MAX_SID][BW_MAX_G7];
	uint8  nssManagement[GEN7_MAX_SID][BW_MAX_G7];
	uint8  mcsData[GEN7_MAX_SID][BW_MAX_G7];
	uint8  mcsManagement[GEN7_MAX_SID][BW_MAX_G7];
	uint8  minPower[GEN7_NUM_OF_BANDS][BW_MAX_G7];
	uint8  maxPower[GEN7_NUM_OF_BANDS][BW_MAX_G7];
	uint8  bw[GEN7_MAX_SID];							/* Band width used in last reception												*/ 
	uint32 transmittedAmpdu[GEN7_MAX_SID];			/* Number of PPDUs transmitted for given rate										*/
	uint32 totalTxTime[GEN7_MAX_SID];
	uint32 rxDc[MAX(BW_MAX_G7, MAX_NUM_OF_ANTENNAS)];
	uint32 txLo[MAX(BW_MAX_G7, MAX_NUM_OF_ANTENNAS)];
	uint32 rxIq[MAX(BW_MAX_G7, MAX_NUM_OF_ANTENNAS)];
	uint32 txIq[MAX(BW_MAX_G7, MAX_NUM_OF_ANTENNAS)];
	uint32 mpduInAmpdu[GEN7_MAX_SID];
	uint32 ewmaTimeNsec[GEN7_MAX_SID];				/* Moving average value based on last couple of transmitted msdus [Nano SECONDS]	*/
	uint32 sumTimeNsec[GEN7_MAX_SID];					/* Delta of cumulative msdus times over interval [Nano SECONDS] 					*/
	uint32 numMsdus[GEN7_MAX_SID];					/* Number of msdus in given interval												*/
	uint32 per[GEN7_MAX_SID];							/* PER																				*/
	uint32 totalPsduLength[GEN7_MAX_SID];			/* Accumulated totalPsduLength in 64Bytes units (quantized)							*/
	uint32 totalZld[GEN7_MAX_SID];				/* Accumulated total ZLDs length  64Bytes units (quantized)							*/
} LinkAdaptationStatistics_t_wave700;
#ifdef CPU_ARC
typedef LinkAdaptationStatistics_t_wave700 LinkAdaptationStatistics_t;
#endif

typedef struct
{
	uint32 groupsCreatedCounter;
	uint32 groupsRemovedCounter;
    uint32 groupsPlanFailCounter;
	uint16 groupsRemovalReasonCounters[MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS_GEN7];
} MuGroupsCounters_t_wave700;
#ifdef CPU_ARC
typedef MuGroupsCounters_t_wave700 MuGroupsCounters_t;
#endif

typedef struct
{
	MuGroupsCounters_t_wave700 dlOfdmaGroupsCounters;
	MuGroupsCounters_t_wave700 ulOfdmaGroupsCounters;
	MuGroupsCounters_t_wave700 dlMimoGroupsCounters;
	MuGroupsCounters_t_wave700 ulMimoGroupsCounters;
    MuGroupsCounters_t_wave700 ulTwtGroupsCounters;
} LinkAdaptationMuGroupsStatistics_t_wave700;
#ifdef CPU_ARC
typedef LinkAdaptationMuGroupsStatistics_t_wave700 LinkAdaptationMuGroupsStatistics_t;
#endif

typedef struct
{
	uint16 suPpdusCount[BW_MAX_G7];
} SuStationsPpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef SuStationsPpdusStatistics_t_wave700 SuStationsPpdusStatistics_t;
#endif

typedef struct
{
#ifdef CPU_ARC
	StaId staId;
    MuGroupState_e muGroupState;
#else
    /* The size of these variables should be explicitly stated
        or driver assumes that each enum is of size uint32 */
	uint16 staId;
    uint8 muGroupState;
#endif
	uint16 muOfdmaPpdusCount;
	uint16 muMimoPpdusCount;
	uint16 muRuPpdusCount[WAVE700_NUM_OF_RU_SIZES];
} MuStationsPpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef MuStationsPpdusStatistics_t_wave700 MuStationsPpdusStatistics_t;
#endif

typedef struct
{
	uint32 totalPpdusCount[GEN7_MAX_SID];
	uint32 suPpdusNewEntryBitArray[((GEN7_MAX_SID + (NUM_OF_BITS_IN_UINT32 -1)) / NUM_OF_BITS_IN_UINT32)];
	uint32 muPpdusNewEntryBitArray[((MAX_NUM_OF_MU_STA_STATS_ENTRIES  + (NUM_OF_BITS_IN_UINT32 -1)) / NUM_OF_BITS_IN_UINT32)];
	SuStationsPpdusStatistics_t_wave700 suStationsPpdus[GEN7_MAX_SID];
	MuStationsPpdusStatistics_t_wave700 muStationsPpdus[MAX_NUM_OF_MU_STA_STATS_ENTRIES_GEN7C0];
} StationsPpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef StationsPpdusStatistics_t_wave700 StationsPpdusStatistics_t;
#endif

typedef struct
{
	uint32 totalPpdusCount;
	uint32 suPpdusCount;
	uint32 muOfdmaPpdusCount;
	uint32 muMimoPpdusCount;
	uint32 muRuPpdusCount[WAVE700_NUM_OF_RU_SIZES];
	uint32 bwRusInPpdus[BW_MAX_G7][WAVE700_NUM_OF_RU_SIZES];
} BandsPpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef BandsPpdusStatistics_t_wave700 BandsPpdusStatistics_t;
#endif

typedef struct
{
	StationsPpdusStatistics_t_wave700 stationsPpdus;
	BandsPpdusStatistics_t_wave700 bandsPpdus[GEN7_NUM_OF_BANDS];
} PpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef PpdusStatistics_t_wave700 PpdusStatistics_t;
#endif

typedef struct 
{
    uint32 successMpdus;
    uint32 failedMpdus;
} MpdusStatisticsUnit_t_wave700;
#ifdef CPU_ARC
typedef MpdusStatisticsUnit_t_wave700 MpdusStatisticsUnit_t;
#endif

typedef struct 
{
    MpdusStatisticsUnit_t_wave700 staMpdu[BW_MAX_G7];
} SuStationsMpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef SuStationsMpdusStatistics_t_wave700 SuStationsMpdusStatistics_t;
#endif

typedef struct 
{
    MpdusStatisticsUnit_t_wave700 suBandMpdus[BW_MAX_G7];
    MpdusStatisticsUnit_t_wave700 muRuMpdus[WAVE700_NUM_OF_RU_SIZES];
} BandMpduStatistics_t_wave700;
#ifdef CPU_ARC
typedef BandMpduStatistics_t_wave700 BandMpduStatistics_t;
#endif

typedef struct
{
#ifdef CPU_ARC
	StaId staId;
#else
	uint16 staId;
#endif
	MpdusStatisticsUnit_t_wave700 muRuMpdusCount[WAVE700_NUM_OF_RU_SIZES];
} MuStationsMpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef MuStationsMpdusStatistics_t_wave700 MuStationsMpdusStatistics_t;
#endif
typedef struct
{
    SuStationsMpdusStatistics_t_wave700 suStationsMpdus[GEN7_MAX_SID];
    BandMpduStatistics_t_wave700 bandMpdus[GEN7_NUM_OF_BANDS];
	MuStationsMpdusStatistics_t_wave700 muStationsMpdus[MAX_NUM_OF_MU_STA_STATS_ENTRIES_GEN7C0];
} MpdusStatistics_t_wave700;
#ifdef CPU_ARC
typedef MpdusStatistics_t_wave700 MpdusStatistics_t;
#endif

typedef struct
{
    uint32 nssPpdus[REDUCED_SPATIAL_STREAMS_SIZE_GEN7];
    uint32 mcsPpdus[REDUCED_MCS_SIZE_GEN7];
} SuRateDlStatisticsUnit_t_wave700;
#ifdef CPU_ARC
typedef SuRateDlStatisticsUnit_t_wave700 SuRateDlStatisticsUnit_t;
#endif

typedef struct
{
	SuRateDlStatisticsUnit_t_wave700 staStatistics[GEN7_MAX_SID];
    SuRateDlStatisticsUnit_t_wave700 bandStatistics[GEN7_NUM_OF_BANDS];
} SuRateDlStatistics_t_wave700;
#ifdef CPU_ARC
typedef SuRateDlStatistics_t_wave700 SuRateDlStatistics_t;
#endif

typedef struct
{
    uint32 nssPpdus[REDUCED_SPATIAL_STREAMS_SIZE_GEN7];
    uint32 mcsPpdus[REDUCED_MCS_SIZE_GEN7];
    uint32 succeededSounding;
    uint32 attemptedSoundingPerMCS[REDUCED_MCS_SIZE_GEN7];
} MuMimoStatistics_t_wave700;
#ifdef CPU_ARC
typedef MuMimoStatistics_t_wave700 MuMimoStatistics_t;
#endif

typedef struct
{
#ifdef CPU_ARC
	StaId staId;
    MuGroupState_e muGroupState;
#else
    uint16 staId;
    uint8 muGroupState;
#endif
    MuMimoStatistics_t_wave700 muStatistics;
} MuMimoTableEntry_t_wave700;
#ifdef CPU_ARC
typedef MuMimoTableEntry_t_wave700 MuMimoTableEntry_t;
#endif

typedef struct
{
    uint8 mimoTableMapping[GEN7_MAX_SID];
    MuMimoTableEntry_t_wave700 muTableEntry[MAX_NUM_OF_MIMO_STA_STATS_ENTRIES_GEN7];
} StaMimoStatistics_t_wave700;
#ifdef CPU_ARC
typedef StaMimoStatistics_t_wave700 StaMimoStatistics_t;
#endif

typedef struct
{
	StaMimoStatistics_t_wave700 staStatistics;
    MuMimoStatistics_t_wave700 bandStatistics[GEN7_NUM_OF_BANDS];
} MimoStatistics_t_wave700;	
#ifdef CPU_ARC
typedef MimoStatistics_t_wave700 MimoStatistics_t;
#endif

typedef struct
{
    uint8 muDlPPDUTableMapping[GEN7_MAX_SID];
    uint8 muUlPPDUTableMapping[GEN7_MAX_SID];
	PpdusStatistics_t_wave700 ppdusDl;
	PpdusStatistics_t_wave700 ppdusUl;
    MpdusStatistics_t_wave700 mpdusDl;
    SuRateDlStatistics_t_wave700 suRateDl;
    MimoStatistics_t_wave700 mimoDl;
} LinkAdaptationSuMuStatistics_t_wave700;
#ifdef CPU_ARC
typedef LinkAdaptationSuMuStatistics_t_wave700 LinkAdaptationSuMuStatistics_t;
#endif

typedef struct 
{
	uint32 notEnoughClonePds;
	uint32 allClonesFinishedTransmission;
	uint32 noStationsInGroup;
} MulticastStatistics_t_wave700;
#ifdef CPU_ARC
typedef MulticastStatistics_t_wave700 MulticastStatistics_t;
#endif

typedef struct 
{
	uint32 trainingTimerExpiredCounter;
	uint32 trainingStartedCounter;
	uint32 trainingFinishedSuccessfullyCounter;
	uint32 trainingFinishedUnsuccessfullyCounter;
	uint32 trainingNotStartedCounter;
} TrainingManagerStatistics_t_wave700;
#ifdef CPU_ARC
typedef TrainingManagerStatistics_t_wave700 TrainingManagerStatistics_t;
#endif

typedef struct 
{
	uint8 groupInfoDb[TX_MU_GROUPS][MU_NUM_OF_STATS];
} GroupManagerStatistics_t_wave700;
#ifdef CPU_ARC
typedef GroupManagerStatistics_t_wave700 GroupManagerStatistics_t;
#endif

typedef struct 
{
	uint8 downlinkMuType;
	uint8 dlRuSubChannels;
	uint8 dlRuType;
	uint8 reserved;
} dl_mu_stats_t_wave700;
#ifdef CPU_ARC
typedef dl_mu_stats_t_wave700 dl_mu_stats_t;
#endif

typedef struct 
{
	uint8 uplinkMuType;
	uint8 ulRuSubChannels;
	uint8 ulRuType;
	uint8 reserved;
} ul_mu_stats_t_wave700;
#ifdef CPU_ARC
typedef ul_mu_stats_t_wave700 ul_mu_stats_t;
#endif

typedef struct 
{
	ul_mu_stats_t_wave700 uplinkMuStats[GEN7_MAX_SID];
	dl_mu_stats_t_wave700 downlinkMuStats[GEN7_MAX_SID];
} PlanManagerStatistics_t_wave700;
#ifdef CPU_ARC
typedef PlanManagerStatistics_t_wave700 PlanManagerStatistics_t;
#endif

typedef struct 
{
    uint16 protectionSentCnt;
    uint16 protectionSucceededCnt;
    uint16 dlPhaseAllocFailCnt;
    uint16 heMuFastProbeTriggeredCnt[MAX_STA_IN_GROUP];
    uint16 heMuFastProbeNotFinishedCnt[MAX_STA_IN_GROUP];
    uint16 heMuCollisionDetectedCnt[MAX_STA_IN_GROUP];
    StaId  staId[MAX_STA_IN_GROUP];
    int8   targetRssi[MAX_STA_IN_GROUP];
    int8   txPower[MAX_STA_IN_GROUP];
    uint8  mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    uint8  groupId;
#ifdef CPU_ARC
} LinkAdaptationMuHeEhtDLStatistics_t;
#else
} LinkAdaptationMuHeEhtDLStatistics_t_wave700;
#endif

typedef struct 
{
    uint16 ulPCStabilityCnt;
    uint16 fastDropCnt[MAX_STA_IN_GROUP];
    uint16 invalidReportsCnt[MAX_STA_IN_GROUP];
    uint16 rxPPDUsCnt[MAX_STA_IN_GROUP];
    uint16 changedWpCnt[MAX_STA_IN_GROUP];
    uint16 collisionDetectedCnt[MAX_STA_IN_GROUP];
    StaId  staId[MAX_STA_IN_GROUP];
    int8   targetRSSI[MAX_STA_IN_GROUP];
    uint8  longTermErrorRate[MAX_STA_IN_GROUP];
    uint8  workingPointErrorRate[MAX_STA_IN_GROUP];
    uint8  mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    uint8  groupId;
#ifdef CPU_ARC
} LinkAdaptationMuHeEhtULStatistics_t;
#else
} LinkAdaptationMuHeEhtULStatistics_t_wave700;
#endif

typedef struct
{
	uint32 dlBitmap;		// bitmap to indicate which dl group ids are used
	uint32 ulBitmap;		// bitmap to indicate which ul group ids are used
	uint32 dlToggleBitmap;	// bitmap where the bit matching a dl group Id toggles whenever the group is created (to identify when to reset counters in driver)
	uint32 ulToggleBitmap;	// bitmap where the bit matching a ul group Id toggles whenever the group is created (to identify when to reset counters in driver)
#ifdef CPU_ARC
	LinkAdaptationMuHeEhtDLStatistics_t laMuHeEhtDLStatistics[MAX_NUM_OF_HE_MU_DL_GROUPS];
	LinkAdaptationMuHeEhtULStatistics_t laMuHeEhtULStatistics[MAX_NUM_OF_HE_MU_UL_GROUPS];
#else
	LinkAdaptationMuHeEhtDLStatistics_t_wave700 laMuHeEhtDLStatistics[MAX_NUM_OF_HE_MU_DL_GROUPS];
    LinkAdaptationMuHeEhtULStatistics_t_wave700 laMuHeEhtULStatistics[MAX_NUM_OF_HE_MU_UL_GROUPS];
#endif
} LinkAdaptationMuHeEhtStatistics_t_wave700;
#ifdef CPU_ARC
typedef LinkAdaptationMuHeEhtStatistics_t_wave700 LinkAdaptationMuHeEhtStatistics_t;
#endif

typedef struct 
{
	uint32 ratesMask[TX_MU_GROUPS][MAX_USP_IN_VHT_GROUP];
	uint32 protectionSentCounter[TX_MU_GROUPS];
	uint32 protectionSucceededCounter[TX_MU_GROUPS];
	uint32 DataPhyMode[TX_MU_GROUPS];
	uint32 powerData[TX_MU_GROUPS][BW_MAX_G7];
	uint32 scpData[TX_MU_GROUPS][BW_MAX_G7];
	uint32 dataBwLimit[TX_MU_GROUPS];
	uint32 groupGoodput[TX_MU_GROUPS];
	uint32 raState[TX_MU_GROUPS];
	uint32 raStability[TX_MU_GROUPS][MAX_USP_IN_VHT_GROUP];
	uint32 nssData[TX_MU_GROUPS][MAX_USP_IN_VHT_GROUP];
	uint32 mcsData[TX_MU_GROUPS][MAX_USP_IN_VHT_GROUP];
} LinkAdaptationMuVhtStatistics_t_wave700;
#ifdef CPU_ARC
typedef LinkAdaptationMuVhtStatistics_t_wave700 LinkAdaptationMuVhtStatistics_t;
#endif

typedef struct 
{
	uint32 illegalEvent[PTA_EVENT_NUM_OF_EVENTS][PTA_STATE_NUM_OF_STATES][PTA_RADIO_NUM_OF_TYPES];
	uint32 ignoreEvent[PTA_EVENT_NUM_OF_EVENTS][PTA_STATE_NUM_OF_STATES][PTA_RADIO_NUM_OF_TYPES];	
	uint32 zigbeeStarvationTimerExpired;
	uint32 btStarvationTimerExpired;
	uint32 zigbeeDenyOverTime;
	uint32 btDenyOverTime;
	uint32 gpio0TxDemandLow;
	uint32 zigbeeGrnatNotUsed;
	uint32 btGrnatNotUsed;
	uint32 zigbeeGrnatUsed;
	uint32 btGrnatUsed;
	uint32 zigbeeTxOngoing;
	uint32 btTxOngoing;
	uint32 gpio;
	uint32 priorityIsHigh;
	uint32 priorityIsLow;
} PtaStatistics_t_wave700;
#ifdef CPU_ARC
typedef PtaStatistics_t_wave700 PtaStatistics_t;
#endif

typedef struct 
{
	uint8 implicit;
	uint8 announced;
	uint8 triggerEnabled;
	uint8 reserved;
} twt_operation_t_wave700;
#ifdef CPU_ARC
typedef twt_operation_t_wave700 twt_operation_t;
#endif

typedef struct 
{
	uint32 wakeTimeHigh;
	uint32 wakeTimeLow;
	uint32 wakeInterval;
	uint16 minWakeDuration;
	uint16 channel;
} twt_individual_params_t_wave700;
#ifdef CPU_ARC
typedef twt_individual_params_t_wave700 twt_individual_params_t;
#endif

typedef struct 
{
	uint32 tragetBeacon;
	uint32 listenInterval;
} twt_broadcast_params_t_wave700;
#ifdef CPU_ARC
typedef twt_broadcast_params_t_wave700 twt_broadcast_params_t;
#endif

typedef struct 
{
	uint16 state;
	uint16 agreementType;
	twt_operation_t_wave700 operation;
	union 
	{
		twt_individual_params_t_wave700 individual;
		twt_broadcast_params_t_wave700 broadcast;
	} params;
} twt_agreement_t_wave700;
#ifdef CPU_ARC
typedef twt_agreement_t_wave700 twt_agreement_t;
#endif

typedef struct 
{
	uint32 numOfAgreementsForSta;
	twt_agreement_t_wave700 twtIndividualAgreement[TWT_MAX_INDIVIDUAL_AGREEMENTS_ALLOWED_PER_STA];
	twt_agreement_t_wave700 twtBroadcastAgreement[TWT_MAX_BROADCAST_MEMBERSHIPS_ALLOWED_PER_STA];
} twt_params_t_wave700;
#ifdef CPU_ARC
typedef twt_params_t_wave700 twt_params_t;
#endif


typedef struct 
{
	uint32 numOfStaInSp[MAX_NUM_OF_SP_BAND_0_700 + MAX_NUM_OF_SP_BAND_1_700 + MAX_NUM_OF_SP_BAND_2_700];
    twt_params_t_wave700 twtStaParams[GEN7_MAX_SID];
} TwtStatistics_t_wave700;
#ifdef CPU_ARC
typedef TwtStatistics_t_wave700 TwtStatistics_t;
#endif

typedef struct 
{
	int8 	maxRssi[GEN7_MAX_SID];
	int8	minRssi[GEN7_MAX_SID];	
} AlphaFilterStatistics_t_wave700;
#ifdef CPU_ARC
typedef AlphaFilterStatistics_t_wave700 AlphaFilterStatistics_t;
#endif

typedef struct 
{
	uint32 calibrationMask[GEN7_NUM_OF_BANDS];
	uint32 onlineCalibrationMask[GEN7_NUM_OF_BANDS];
	uint32 dropReasonClassifier[GEN7_NUM_OF_BANDS];
	uint32 dropReasonDisconnect[GEN7_NUM_OF_BANDS];
	uint32 dropReasonATF[GEN7_NUM_OF_BANDS];
	uint32 dropReasonTSFlush[GEN7_NUM_OF_BANDS];
	uint32 dropReasonReKey[GEN7_NUM_OF_BANDS];
	uint32 dropReasonSetKey[GEN7_NUM_OF_BANDS];
	uint32 dropReasonDiscard[GEN7_NUM_OF_BANDS];
	uint32 dropReasonDsabled[GEN7_NUM_OF_BANDS];
	uint32 dropReasonAggError[GEN7_NUM_OF_BANDS];
	uint32 defragStart[GEN7_NUM_OF_BANDS];
	uint32 defragEnd[GEN7_NUM_OF_BANDS];
	uint32 defragTimeout[GEN7_NUM_OF_BANDS];
	uint32 classViolationErrors[GEN7_NUM_OF_BANDS];
	uint32 rxhErrors[GEN7_NUM_OF_BANDS];
	uint32 rxSmps[GEN7_NUM_OF_BANDS];
	uint32 rxOmn[GEN7_NUM_OF_BANDS];
	uint32 txMngFromHost[GEN7_NUM_OF_BANDS];
	uint32 txDataFromHost[GEN7_NUM_OF_BANDS];
	uint32 cyclicBufferOverflow[GEN7_NUM_OF_BANDS];
	uint32 beaconsTransmitted[GEN7_NUM_OF_BANDS];
	uint32 debugStatistics[GEN7_NUM_OF_BANDS];
	uint32 exceedsETSIMaxSizeUnicast[GEN7_NUM_OF_BANDS];
	uint32 exceedsETSIMaxSizeMulticast[GEN7_NUM_OF_BANDS];
} GeneralStatistics_t_wave700;
#ifdef CPU_ARC
typedef GeneralStatistics_t_wave700 GeneralStatistics_t;
#endif

typedef struct 
{
	/* TX */
	uint32 retryCount[GEN7_MAX_SID];
	uint32 successCount[GEN7_MAX_SID];
	uint32 successByteCount[GEN7_MAX_SID];
	uint32 exhaustedCount[GEN7_MAX_SID];
	uint32 clonedCount[GEN7_MAX_SID];
	uint32 oneOrMoreRetryCount[GEN7_MAX_SID];
	uint32 packetRetransCount[GEN7_MAX_SID];
	uint32 dropCntReasonClassifier[GEN7_MAX_SID];
	uint32 dropCntReasonDisconnect[GEN7_MAX_SID];
	uint32 dropCntReasonATF[GEN7_MAX_SID];
	uint32 dropCntReasonTSFlush[GEN7_MAX_SID];
	uint32 dropCntReasonReKey[GEN7_MAX_SID];
	uint32 dropCntReasonSetKey[GEN7_MAX_SID];
	uint32 dropCntReasonDiscard[GEN7_MAX_SID];
	uint32 dropCntReasonDsabled[GEN7_MAX_SID];
	uint32 dropCntReasonAggError[GEN7_MAX_SID];
	/* RX */
	uint32 mpduRetryCount[GEN7_MAX_SID];	/* Number of rx retries			*/
	uint32 mpduInAmpdu[GEN7_MAX_SID];		/* Number of MPDUs received		*/
	uint32 ampdu[GEN7_MAX_SID];	 		/* Number of PPDUs received 	*/
	
} PerClientStatistics_t_wave700;
#ifdef CPU_ARC
typedef PerClientStatistics_t_wave700 PerClientStatistics_t;
#endif

typedef struct 
{
	uint32	chUtilizationBusy; 		/* Time radio detected that channel was busy (Busy = Rx + Tx + Interference) 						*/		
	uint32	chUtilizationBusyTx; 	/* Time radio spent transmitting on channel 														*/ 	
	uint32	chUtilizationBusyRx; 	/* Time radio spent receiving on channel (Rx = Rx_obss + Rx_self + Rx_errr (self and obss errors)	*/ 	
	uint32	chUtilizationBusyRxSelf;/* Time radio spend receiving on channel from its own connected clients								*/
	uint32	chUtilizationBusyExt;	/* Time radio detected that extended channel was busy (40MHz extention channel busy)				*/
	uint32	chUtilization;			/* 9.4.2.28 BSS Load element. Channel Utilization paramter											*/
	uint64  chUtilizationCurrTime; 	/* Current TSF 																						*/
} chUtilization_t_wave700;
#ifdef CPU_ARC
typedef chUtilization_t_wave700 chUtilization_t;
#endif

typedef struct 
{
	chUtilization_t_wave700 	channelUtilizationStats[GEN7_NUM_OF_BANDS];
	int8						channelMax80211Rssi[GEN7_NUM_OF_BANDS];
	uint8						channelNum[GEN7_NUM_OF_BANDS];
	uint8						reserved[2];

} channelStatistics_t_wave700;
#ifdef CPU_ARC
typedef channelStatistics_t_wave700 channelStatistics_t;
#endif

typedef struct 
{	
	//word 0 (for SW use)
	uint8	channel_load;	
	uint8 	totalChannelUtilization;	/* Total Channel Utilization is an indication of how congested the medium is (all APs) */
	int8  	chNon80211Noise;
	int8	CWIvalue;
	
	//word 1 (for SW use)
	uint32	extStaRx;	

	//word 2 (for SW use)
	int16	txPower;
	uint8	channelNum;
	uint8	activeAntMask;
	//word 3-5
    uint16  backgroundNoise[PHY_STATISTICS_MAX_RX_ANT_700]; /* Long term average Background Noise per antenna */
	int8	noiseFloor[PHY_STATISTICS_MAX_RX_ANT_700];
	uint8 	reserved;
} devicePhyRxStatusDb_t_wave700;
#ifdef CPU_ARC
typedef devicePhyRxStatusDb_t_wave700 devicePhyRxStatusDb_t;
#endif

// Structure parameters aligned to 32 bits
typedef struct 
{
	//words 0,1,2,3
	int8 	noise[GEN7_NUM_OF_BANDS][PHY_STATISTICS_MAX_RX_ANT_700];
	uint8   reserved0[NUM_OF_BYTES_IN_WORD*4-PHY_STATISTICS_MAX_RX_ANT_700*GEN7_NUM_OF_BANDS]; // 21bit alpha factor, 3 bit reserved
	//words 4,5,6,7
	int8 	gain[GEN7_NUM_OF_BANDS][PHY_STATISTICS_MAX_RX_ANT_700];	
	uint8   reserved1[NUM_OF_BYTES_IN_WORD*4-PHY_STATISTICS_MAX_RX_ANT_700*GEN7_NUM_OF_BANDS]; // 21bit alpha factor, 3 bit reserved
	//words 8,9,10,11
	int8 	evm[GEN7_NUM_OF_BANDS][MAX_NUM_OF_ANTENNAS];	   			 // EVM is NSS related(4), not num of antennas (5)
	uint32  reserved2; 								// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	//words 12,13
	int8 	rssi[GEN7_NUM_OF_BANDS][PHY_STATISTICS_MAX_RX_ANT_700];	
	uint8   reserved3[NUM_OF_BYTES_IN_WORD*4-PHY_STATISTICS_MAX_RX_ANT_700*GEN7_NUM_OF_BANDS]; // 21bit alpha factor, 3 bit reserved
	//words 14,15
	uint32 	avgDeltaT;	
	uint32  reserved4;								// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	// words 16,17
	int16   dsnrMargin;
	uint16  reserved5;
	uint32  reserved6;								// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	// words 18,19
	int16   macPer;	
	uint16  reserved7;
	uint32  reserved8;								// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	// words 20,21
	uint32  macAvgDeltaT;
	uint32  reserved9;								// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	//word 22
	uint32 	phyRate;
	//word 23
	uint32 	tsf;
	//word 24
	uint32 	macTsf;
	uint32  reserved10;                              // Added for alignment issue with 64bit
} multiUserPhyRxStatusDb_t_wave700;
#ifdef CPU_ARC
typedef multiUserPhyRxStatusDb_t_wave700 multiUserPhyRxStatusDb_t;
#endif

typedef struct 
{
	devicePhyRxStatusDb_t_wave700 	devicePhyRxStatus[GEN7_NUM_OF_BANDS];
	stationPhyRxStatusDb_t_wave700	staPhyRxStatus[GEN7_MAX_SID];	
	multiUserPhyRxStatusDb_t_wave700  muPhyRxStatus;
	uint8 reserved[4];		//next structure is 64 bits aligned
} wholePhyRxStatusDb_t_wave700;
#ifdef CPU_ARC
typedef wholePhyRxStatusDb_t_wave700 wholePhyRxStatusDb_t;
#endif

typedef struct 
{
	uint32	dynamicBW20MHz[GEN7_NUM_OF_BANDS];
	uint32	dynamicBW40MHz[GEN7_NUM_OF_BANDS];
	uint32	dynamicBW80MHz[GEN7_NUM_OF_BANDS];
	uint32	dynamicBW160MHz[GEN7_NUM_OF_BANDS];
	// TODO: WLANRTSYS-30987
	//uint32	dynamicBW320MHz[GEN7_NUM_OF_BANDS];
} DynamicBwStatistics_t_wave700;
#ifdef CPU_ARC
typedef DynamicBwStatistics_t_wave700 DynamicBwStatistics_t;
#endif

typedef struct
{
	uint32 multiCastTxPackets[GEN7_MAX_VAP][MAX_NUM_OF_GROUPS];
	uint32 multiCastTxBytes[GEN7_MAX_VAP][MAX_NUM_OF_GROUPS];
} MultiCastGroupStatistics_t_wave700;
#ifdef CPU_ARC
typedef MultiCastGroupStatistics_t_wave700 MultiCastGroupStatistics_t;
#endif

/*
order should be aligned to StatisticType_e
*/
typedef struct _StatisticsDb_t_wave700
{
	HostIfQosCounters_t_wave700							hostIfQosCounters;
	HostIfCounters_t_wave700 							hostIfCounters;
	RxCoordinatorCounters_t_wave700						RxCoordinatorCounters;
	RxCounters_t_wave700								rxCounters;
	BaaCounters_t_wave700								baaCounters;
	UplinkBsrcTidCnt_t_wave700							uplinkBsrcTidCnt;
	TsManagerInitiatorTidGlobalStatistics_t_wave700		tsManagerInitiatorTidGlobalStats;
	TsManagerInitiatorStationGlobalStatistics_t_wave700	tsManagerInitiatorStationGlobalStats;
	TsManagerRecipientTidGlobalStatistics_t_wave700		tsManagerRecipientTidGlobalStats;
	TsManagerRecipientStationGlobalStatistics_t_wave700	tsManagerRecipientStationGlobalStats;
	LinkAdaptationStatistics_t_wave700					linkAdaptationStats;
	MulticastStatistics_t_wave700						multicastStats;
	TrainingManagerStatistics_t_wave700					trainingManagerStats;
	GroupManagerStatistics_t_wave700					groupManagerStats;
	LinkAdaptationMuVhtStatistics_t_wave700				linkAdaptationMuVhtStats;
	LinkAdaptationMuHeEhtStatistics_t_wave700			linkAdaptationMuHeEhtStats;
	PtaStatistics_t_wave700								ptaStats;
	TwtStatistics_t_wave700								twtStats;
	PlanManagerStatistics_t_wave700						planManagerStats;
	AlphaFilterStatistics_t_wave700						alphaFilterStats;
	PerClientStatistics_t_wave700						perClientStats;
	GeneralStatistics_t_wave700							generalStats;
	DynamicBwStatistics_t_wave700						dynamicBwStats;
	wholePhyRxStatusDb_t_wave700						phyStatistics;
	channelStatistics_t_wave700							currentChannelStats;
	LinkAdaptationMuGroupsStatistics_t_wave700			linkAdaptationMuGroupsStats;
	LinkAdaptationSuMuStatistics_t_wave700				linkAdaptationSuMuStatistics;
	MultiCastGroupStatistics_t_wave700					MultiCastGroupStatistics;
	/*
	order should be aligned to StatisticType_e
	*/
} StatisticsDb_t_wave700;
#ifdef CPU_ARC
typedef StatisticsDb_t_wave700 StatisticsDb_t;
#endif

#endif //_STATISTICS_DESC_WAVE700_H_

