/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __MTLK_WSSA_DRVINFO_H__
#define __MTLK_WSSA_DRVINFO_H__

#include "mtlk_wss_debug.h"
#include "mhi_ieee_address.h"
#include "vendor_cmds.h"
#include "Statistics/WAVE600B/Statistics_Descriptors.h"
#include "Statistics/WAVE600D2/Statistics_Descriptors.h"
#include "Statistics/WAVE700/Statistics_Descriptors.h"

#ifdef CONFIG_WAVE_DEBUG
#define MAX_USP_IN_VHT_GROUP (4)
#define NUM_OF_BW            (4)
#define ACCESS_CATEGORY_NUM  (4)
#endif /* CONFIG_WAVE_DEBUG */

#define MAX_NUM_OF_RU_SIZES              WAVE700_NUM_OF_RU_SIZES
#define MAX_NUM_OF_BW                    BW_MAX_G7
#define MAX_REDUCED_NSS                  REDUCED_SPATIAL_STREAMS_SIZE_GEN7
#define MAX_REDUCED_MCS                  REDUCED_MCS_SIZE_GEN7

/* OFDMA ppdus stats */
#define UL_OFDMA_CATEGORY_STA_STATS      (2)
#define DL_OFDMA_CATEGORY_STA_STATS      (3)
#define UL_OFDMA_CATEGORY_RADIO_STATS    (0xF0)
#define DL_OFDMA_CATEGORY_RADIO_STATS    (0xF1)

/* Included the UL-MIMO Removal reason and will be filled only for GEN7 */
#define MAX_REMOVAL_REASONS              (MAX(MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS, MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS_GEN7))

#define MAX_CSI_SLOT_STATS               (10)
typedef enum{
  MTLK_WSSA_11A_SUPPORTED,
  MTLK_WSSA_11B_SUPPORTED,
  MTLK_WSSA_11G_SUPPORTED,
  MTLK_WSSA_11N_SUPPORTED,
  MTLK_WSSA_11AC_SUPPORTED,
  MTLK_WSSA_11AX_SUPPORTED,
  MTLK_WSSA_11BE_SUPPORTED,
}mtlk_wssa_net_modes_supported_e;

typedef enum{
  VENDOR_UNKNOWN,
  VENDOR_LANTIQ,
  VENDOR_W101,
}mtlk_wssa_peer_vendor_t;

typedef enum{
  MTLK_PHY_MODE_AG,
  MTLK_PHY_MODE_B,
  MTLK_PHY_MODE_N,
  MTLK_PHY_MODE_AC,
  MTLK_PHY_MODE_AX,
}mtlk_wssa_phy_mode_e;

typedef struct mtlk_wssa_peer_traffic_stats{
  uint64 BytesSent;
  uint64 BytesReceived;
  uint64 PacketsSent;
  uint64 PacketsReceived;
  uint32 UnicastPacketsSent;
  uint32 UnicastBytesSent;
  uint32 UnicastPacketsReceived;
  uint32 UnicastBytesReceived;
  uint32 MulticastPacketsSent;
  uint32 MulticastBytesSent;
  uint32 BroadcastPacketsReceived;
  uint32 BroadcastBytesReceived;
}mtlk_wssa_peer_traffic_stats_t;

typedef struct mtlk_wssa_retrans_stats{
  uint32 Retransmissions;
  uint32 RetransCount;
  uint32 RetryCount;
  uint32 MultipleRetryCount;
  uint32 FailedRetransCount;
}mtlk_wssa_retrans_stats_t;

typedef struct mtlk_wssa_drv_tr181_peer_stats{
  uint32 StationId;
  uint32 NetModesSupported;
  mtlk_wssa_peer_traffic_stats_t traffic_stats;
  mtlk_wssa_retrans_stats_t retrans_stats;
  uint32 ErrorsSent;
  uint32 LastDataDownlinkRate;
  uint32 LastDataUplinkRate;
  int32 SignalStrength;
}__attribute__ ((packed)) mtlk_wssa_drv_tr181_peer_stats_t;

typedef struct mtlk_wssa_drv_peer_stats{
  mtlk_wssa_drv_tr181_peer_stats_t tr181_stats;
  uint32 AirtimeEfficiency;
  uint8 AirtimeUsage;
  int8 ShortTermRSSIAverage[WAVE_STAT_MAX_ANTENNAS];
  int8 snr[WAVE_STAT_MAX_ANTENNAS];
  int8  LongTermNoiseAverage[WAVE_STAT_MAX_ANTENNAS];
  int8  avg_rssi;
}__attribute__ ((packed)) mtlk_wssa_drv_peer_stats_t;

typedef struct mtlk_wssa_drv_peer_rate_info1{
  uint32 InfoFlag;
  uint32 PhyMode;
  int32 CbwIdx;
  int32 CbwMHz;
  int32 Scp;
  int32 Mcs;
  int32 Nss;
}mtlk_wssa_drv_peer_rate_info1_t;

typedef struct mtlk_wssa_drv_peer_rates_info{
  mtlk_wssa_drv_peer_rate_info1_t rx_mgmt_rate_info;
  uint32 RxMgmtRate;
  mtlk_wssa_drv_peer_rate_info1_t rx_data_rate_info;
  uint32 RxDataRate;
  mtlk_wssa_drv_peer_rate_info1_t tx_data_rate_info;
  uint32 TxDataRate;
  uint32 TxBfMode;
  uint32 TxStbcMode;
  uint32 TxPwrCur;
  uint32 TxMgmtPwr;
}mtlk_wssa_drv_peer_rates_info_t;

typedef struct mtlk_wssa_drv_traffic_stats{
  uint64 BytesSent;
  uint64 BytesReceived;
  uint64 PacketsSent;
  uint64 PacketsReceived;
  uint32 UnicastPacketsSent;
  uint32 UnicastPacketsReceived;
  uint32 MulticastPacketsSent;
  uint32 MulticastPacketsReceived;
  uint32 BroadcastPacketsSent;
  uint32 BroadcastPacketsReceived;
  /* Tx/Rx Multicast PD packets stats */
  uint64 PacketPDSent;
  uint64 PDBytesSent;
  uint64 UnicastPDBytesSent;
  uint64 UnicastPDSent;
}mtlk_wssa_drv_traffic_stats_t;

typedef struct mtlk_wssa_drv_tr181_error_stats{
  uint32 ErrorsSent;
  uint32 ErrorsReceived;
  uint32 DiscardPacketsSent;
  uint32 DiscardPacketsReceived;
}mtlk_wssa_drv_tr181_error_stats_t;

typedef struct mtlk_wssa_drv_tr181_wlan_stats{
  mtlk_wssa_drv_traffic_stats_t traffic_stats;
  mtlk_wssa_drv_tr181_error_stats_t error_stats;
  mtlk_wssa_retrans_stats_t retrans_stats;
  uint32 ACKFailureCount;
  uint32 AggregatedPacketCount;
  uint32 UnknownProtoPacketsReceived;
}mtlk_wssa_drv_tr181_wlan_stats_t;

typedef struct mtlk_wssa_drv_tr181_hw{
  uint8 Enable;
  uint8 Channel;
}mtlk_wssa_drv_tr181_hw_t;

typedef struct mtlk_wssa_drv_tr181_hw_stats{
  mtlk_wssa_drv_traffic_stats_t traffic_stats;
  mtlk_wssa_drv_tr181_error_stats_t error_stats;
  uint32 FCSErrorCount;
  int32 Noise;
}mtlk_wssa_drv_tr181_hw_stats_t;

typedef struct mtlk_wssa_drv_recovery_stats{
  uint32 FastRcvryProcessed;
  uint32 FullRcvryProcessed;
  uint32 FastRcvryFailed;
  uint32 FullRcvryFailed;
  uint32 FastRcvryWithinTimer;
  uint32 FullRcvryWithinTimer;
}mtlk_wssa_drv_recovery_stats_t;

typedef struct mtlk_wssa_drv_peer_capabilities{
  uint32 NetModesSupported;
  uint32 WMMSupported;
  uint32 CBSupported;
  uint32 SGI20Supported;
  uint32 SGI40Supported;
  uint32 STBCSupported;
  uint32 LDPCSupported;
  uint32 BFSupported;
  uint32 Intolerant_40MHz;
  uint32 Vendor;
  uint32 MIMOConfigTX;
  uint32 MIMOConfigRX;
  uint32 AMPDUMaxLengthExp;
  uint32 AMPDUMinStartSpacing;
  uint32 AssociationTimestamp;
}mtlk_wssa_drv_peer_capabilities_t;

typedef struct mtlk_wssa_drv_hw_txm_stats{
  uint32 txmm_sent;
  uint32 txmm_cfmd;
  uint32 txmm_peak;
  uint32 txdm_sent;
  uint32 txdm_cfmd;
  uint32 txdm_peak;
}mtlk_wssa_drv_hw_txm_stats_t;

typedef struct mtlk_wssa_drv_mgmt_stats{
  uint32 MANFramesResQueue;
  uint32 MANFramesSent;
  uint32 MANFramesConfirmed;
  uint32 MANFramesReceived;
  uint32 MANFramesRetryDropped;
  uint32 BssMgmtTxQueFull;
  uint32 ProbeRespSent;
  uint32 ProbeRespDropped;
}mtlk_wssa_drv_mgmt_stats_t;

typedef struct mtlk_wssa_drv_hw_stats{
  mtlk_wssa_drv_recovery_stats_t rcvry_stats;
  mtlk_wssa_drv_hw_txm_stats_t   txm_stats;
  mtlk_wssa_drv_traffic_stats_t  traffic_stats;
  mtlk_wssa_drv_mgmt_stats_t     mgmt_stats;
  uint32 RadarsDetected;
  uint32 zwdfs_RadarsDetected;
  int32 zwdfsAntRssiAvg;
  uint32 AirtimeEfficiency;
  uint8  ChannelLoad;
  uint8  ChannelUtil;
  uint8  Airtime;
}mtlk_wssa_drv_hw_stats_t;

typedef struct mtlk_wssa_drv_peer_list{
  IEEE_ADDR addr;
  uint32 is_sta_auth;
}mtlk_wssa_peer_list_t;


/* The two data structures wave_cca_stats_user_t and
 * wave_cca_stats_t must have the same counter names
 */
typedef struct {
  /* One flag for all counters */
  uint32 overflowOccurred;
  /* All counters are in milliseconds */
  uint32 ccaActive;   /* Total count for CCA sampling */
  uint32 ccaTx;       /* CCA transmitting count */
  uint32 ccaRx;       /* CCA receiving to self count */
  uint32 ccaIntf;     /* CCA interference count */
  uint32 ccaIdle;     /* CCA idle count */
} wave_cca_stats_user_t;

typedef struct {
  /* All counters are in microseconds */
  uint64 ccaActive;   /* Total count for CCA sampling */
  uint64 ccaTx;       /* CCA transmitting count */
  uint64 ccaRx;       /* CCA receiving to self count */
  uint64 ccaIntf;     /* CCA interference count */
  uint64 ccaIdle;     /* CCA idle count */
} wave_cca_stats_t;

/* The two data structures must have the same element names
  *   wave_radio_usage_stats_internal_t
  *   wave_radio_usage_stats_t
 */
typedef struct {
  /* All time values are in microseconds */
  uint64 time_on_channel;  /* the channel active */
  uint64 busy;             /* the channel was busy (Rx + Tx + Interference) */
  uint64 busy_tx;          /* tx */
  uint64 busy_rx;          /* rx */
  uint64 busy_self;        /* rx to me (from its own connected clients) */
  uint64 interfer;         /* interference */
  uint64 idle;             /* idle */
  uint64 busy_ext;         /* extended channel was busy */
} wave_radio_usage_stats_internal_t;

typedef struct {
  /* All time values are in milliseconds */
  uint32 time_on_channel;  /* the channel active */
  uint32 busy;             /* the channel was busy (Rx + Tx + Interference) */
  uint32 busy_tx;          /* tx */
  uint32 busy_rx;          /* rx */
  uint32 busy_self;        /* rx to me (from its own connected clients) */
  uint32 interfer;         /* interference */
  uint32 idle;             /* idle */
  uint32 busy_ext;         /* extended channel was busy */
} wave_radio_usage_stats_t;

#define PEER_STATS_RATES_KBPS_MAX 32 /* WV_SUPP_RATES_MAX */
typedef struct {
  uint32 num_rates;                             /* number of supported data rates */
  uint32 rates_kbps[PEER_STATS_RATES_KBPS_MAX]; /* supported data rates in Kbps of the station sorted in ascending order */
  uint32 rate_max;                              /* max STA rate in Kbps per all antennas */
  uint32 rate_1ss;                              /* max STA rate in Kbps per one antenna */
  uint32 ant_max;                               /* max STA nss */
  uint32 net_modes;                             /* bit0: 802.11a, bit1: 802.11b, bit2: 802.11g, bit3: 802.11n, bit4: 802.11ac, bit5: 802.11ax */
  uint32 asleep;                                /* if the station's Wi-Fi is in sleep mode: not supported. */
  uint32 in_network_sec;                        /* time in seconds the station has been associated to the BSSID */
  uint32 idle_sec;                              /* time in seconds the station has been idle */
  uint64 tx_packets;                            /* total number of unicast packets transmitted to the station */
  uint64 rx_packets;                            /* total number of unicast packets received from the station */
} wave_peer_mixed_stats_t;

typedef struct wave_mhi_la_ofdma_ru_ppdu_stats {
  uint64 mu_ru_ppdus;
} wave_mhi_la_ofdma_ru_ppdu_stats_t;

typedef struct wave_mhi_la_ofdma_ru_mpdu_stats {
  /* number of mpdus pkts == mpdus successMpdus */
  uint64 ru_mpdus_count;
  /* number of error pkts == mpdus failedMpdus */
  uint64 ru_mpdus_err_count;
} wave_mhi_la_ofdma_ru_mpdu_stats_t;

typedef struct wave_mhi_la_ofdma_ppdus_stats {
  uint64 total_ppdus;
  uint64 mu_ofdma_ppdus;
  uint64 mu_mimo_ppdus;
  uint64 mu_ppdus;
  uint64 he_ppdus;
  /* total number of mu_ru_ppdus of all the RU */
  uint64 total_mu_ru_ppdus;
  wave_mhi_la_ofdma_ru_ppdu_stats_t ru_ppdu_stats[MAX_NUM_OF_RU_SIZES];
} wave_mhi_la_ofdma_ppdus_stats_t;

typedef struct wave_mhi_la_ofdma_mpdu_stats {
  wave_mhi_la_ofdma_ru_mpdu_stats_t ru_mpdu_stats[MAX_NUM_OF_RU_SIZES];
} wave_mhi_la_ofdma_mpdu_stats_t;

typedef struct wave_mhi_la_ofdma_sta_ppdu_stats {
  wave_mhi_la_ofdma_ppdus_stats_t sta_ppdus_stats;
  uint64 su_ppdus[MAX_NUM_OF_BW];
} wave_mhi_la_ofdma_sta_ppdu_stats_t;

typedef struct wave_mhi_la_ofdma_sta_pdu_stats {
  wave_mhi_la_ofdma_sta_ppdu_stats_t sta_ppdu;
  wave_mhi_la_ofdma_mpdu_stats_t sta_mpdu;
} wave_mhi_la_ofdma_sta_pdu_stats_t;

typedef struct wave_wssa_ul_dl_ofdma_pdu_sta_stats {
  uint16 sta_sid;
  IEEE_ADDR addr;
  uint8 no_of_ru_sizes;
  uint8 no_of_bw;
  wave_mhi_la_ofdma_sta_ppdu_stats_t sta_ppdu;
  wave_mhi_la_ofdma_mpdu_stats_t sta_mpdu;
} wave_wssa_ofdma_pdu_sta_stats_t;

typedef struct wave_wssa_la_ofdma_radio_ppdu_stats {
  uint8 no_of_ru_sizes;
  uint8 no_of_bw;
  uint32 bw_ru_in_ppdus[MAX_NUM_OF_BW][MAX_NUM_OF_RU_SIZES];
  wave_mhi_la_ofdma_ppdus_stats_t radio_ppdu;
  wave_mhi_la_ofdma_mpdu_stats_t radio_mpdu;
  uint64 su_ppdus;
} wave_wssa_la_ofdma_radio_pdu_stats_t;

/* HE MIMO sta stats */
typedef struct wave_mhi_mimo_rate_stats {
  uint32 nss_ppdus_percent[MAX_REDUCED_NSS];
  uint32 mcs_ppdus_percent[MAX_REDUCED_MCS];
  uint64 nss_ppdus[MAX_REDUCED_NSS];
  uint64 mcs_ppdus[MAX_REDUCED_MCS];
} wave_mhi_mimo_rate_stats_t;

typedef struct wave_mhi_su_mimo_stats {
  wave_mhi_mimo_rate_stats_t su_mimo_rate;
} wave_mhi_su_mimo_stats_t;

typedef struct wave_mhi_mu_mimo_stats {
  uint32 succeeded_sounding_percent;
  uint32 attemptedsounding_per_mcs_percent[MAX_REDUCED_MCS];
  wave_mhi_mimo_rate_stats_t mu_mimo_rate;
  uint64 succeeded_sounding;
  uint64 attemptedsounding_per_mcs[MAX_REDUCED_MCS];
  uint64 sum_attemptedsounding;
} wave_mhi_mu_mimo_stats_t;

typedef struct wave_mhi_mimo_stats {
  wave_mhi_su_mimo_stats_t su_mimo;
  wave_mhi_mu_mimo_stats_t mu_mimo;
} wave_wssa_mimo_stats_t;

typedef struct wave_wssa_mimo_sta_stats {
  uint16 sta_sid;
  IEEE_ADDR addr;
  uint8 no_of_nss;
  uint8 no_of_mcs;
  wave_wssa_mimo_stats_t sta_stats;
} wave_wssa_mimo_sta_stats_t;

typedef struct wave_wssa_mimo_radio_stats {
  uint8 no_of_nss;
  uint8 no_of_mcs;
  wave_wssa_mimo_stats_t mimo_stats;
} wave_wssa_mimo_radio_stats_t;

typedef struct wave_wssa_max_tx_power_stats {
  uint32 channel;
  uint32 max_tx_power;
} wave_wssa_max_tx_power_stats_t;
/* HE MIMO sta stats ends */

typedef struct wave_rssi_avg_info
{
  int32 avg_rssi_dbm;
  uint64 last_sample;
  uint64 avg_sample;
} wave_rssi_avg_info_t;

typedef struct wave_radio_csi_slots_stats {
  uint32 max_csi_slots;
  uint32 is_for_band;
} wave_radio_csi_slots_stats_t;

enum wave_la_mu_he_eht_reset
{
  LA_MU_HE_EHT_NO_RESET,
  LA_MU_HE_EHT_RESET_GROUP,
  LA_MU_HE_EHT_RESET_ALL,
};

typedef enum
{
  MU_GROUP_INVALID,
  MU_GROUP_DL,
  MU_GROUP_UL,
} wave_mu_la_group_type_e;

typedef struct mtlk_wssa_drv_la_mu_eh_eht_input
{
  int group_id;
  int reset_stats;
} mtlk_wssa_drv_la_mu_he_eht_input_t;

typedef struct mtlk_wssa_drv_la_mu_he_eht_stats
{
  uint8 type;
  uint8 active;
  uint32 creation_toggle_bitmap;

  union {
    struct {
        uint32 protectionSentCnt;
        uint32 protectionSucceededCnt;
        uint32 dlPhaseAllocFailCnt;
        uint32 heMuFastProbeTriggeredCnt[MAX_STA_IN_GROUP];
        uint32 heMuFastProbeNotFinishedCnt[MAX_STA_IN_GROUP];
        uint32 heMuCollisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32 mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];

        uint16 staId[MAX_STA_IN_GROUP];
        int8   targetRssi[MAX_STA_IN_GROUP];
        int8   txPower[MAX_STA_IN_GROUP];
    } dl;

    struct {
        uint32 ulPCStabilityCnt;
        uint32 fastDropCnt[MAX_STA_IN_GROUP];
        uint32 invalidReportsCnt[MAX_STA_IN_GROUP];
        uint32 rxPPDUsCnt[MAX_STA_IN_GROUP];
        uint32 changedWpCnt[MAX_STA_IN_GROUP];
        uint32 collisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32 mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];

        uint16 staId[MAX_STA_IN_GROUP];
        int8   targetRSSI[MAX_STA_IN_GROUP];
        uint8  longTermErrorRate[MAX_STA_IN_GROUP];
        uint8  workingPointErrorRate[MAX_STA_IN_GROUP];
    } ul;
  };
} mtlk_wssa_drv_la_mu_he_eht_stats_t;

typedef struct wave_la_mu_groups_stats {
  uint32 groupsCreated;
  uint32 groupsRemoved;
  uint16 removalReason[MAX_REMOVAL_REASONS];
} wave_mhi_la_mu_groups_stats_t;

typedef struct mtlk_wssa_drv_la_mu_groups_stats {
  bool is_gen6;
  wave_mhi_la_mu_groups_stats_t dlOfdmaGroupsCounters;
  wave_mhi_la_mu_groups_stats_t ulOfdmaGroupsCounters;
  wave_mhi_la_mu_groups_stats_t dlMimoGroupsCounters;
  wave_mhi_la_mu_groups_stats_t ulMimoGroupsCounters;
} mtlk_wssa_drv_la_mu_groups_stats_t;

#ifdef MTLK_WAVE_700
typedef struct mtlk_wssa_drv_ml_link_stats {
  IEEE_ADDR link_addr;
  mtlk_wssa_drv_peer_stats_t peer_flow_status;
}__attribute__ ((packed)) mtlk_wssa_drv_ml_link_stats_t;

typedef struct mtlk_wssa_drv_ml_peer_stats {
  bool is_single_link;
  mtlk_wssa_drv_ml_link_stats_t ml_peer_stats[2];
} __attribute__ ((packed)) mtlk_wssa_drv_ml_peer_stats_t;
#endif

#ifdef CONFIG_WAVE_DEBUG
typedef struct mtlk_wssa_drv_wlan_host_if_qos{
  uint32 qosTxVap[ACCESS_CATEGORY_NUM];
}mtlk_wssa_drv_wlan_host_if_qos_t;

typedef struct mtlk_wssa_drv_wlan_host_if{
  uint32 txInUnicastHd;
  uint32 txInMulticastHd;
  uint32 txInBroadcastHd;
  uint32 txInUnicastNumOfBytes;
  uint32 txInMulticastNumOfBytes;
  uint32 txInBroadcastNumOfBytes;
  uint32 rxOutUnicastHd;
  uint32 rxOutMulticastHd;
  uint32 rxOutBroadcastHd;
  uint32 rxOutUnicastNumOfBytes;
  uint32 rxOutMulticastNumOfBytes;
  uint32 rxOutBroadcastNumOfBytes;
  uint32 agerCount;
}mtlk_wssa_drv_wlan_host_if_t;

typedef struct mtlk_wssa_drv_wlan_rx_stats{
  uint32 amsdu;
  uint32 amsduBytes;
  uint32 dropCount;
  uint32 mpduInAmpdu;
  uint32 octetsInAmpdu;
  uint32 rxCoorSecurityMismatch;
  uint32 tkipCount;
  uint32 securityFailure;
  uint32 mpduUnicastOrMngmnt;
  uint32 mpduRetryCount;
  uint32 dropMpdu;
  uint32 ampdu;
  uint32 mpduTypeNotSupported;
  uint32 replayData;
  uint32 replayMngmnt;
  uint32 bcMcCountVap;
}mtlk_wssa_drv_wlan_rx_stats_t;

typedef struct mtlk_wssa_drv_wlan_baa_stats{
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
  uint32 rxGroupFrame;
  uint32 txSenderError;
}mtlk_wssa_drv_wlan_baa_stats_t;

typedef struct mtlk_wssa_drv_radio_rx_stats{
  uint32 barMpduCount;
  uint32 crcErrorCount;
  uint32 delCrcError;
}mtlk_wssa_drv_radio_rx_stats_t;

typedef struct mtlk_wssa_drv_radio_baa_stats{
  uint32 retryCount;
  uint32 multipleRetryCount;
  uint32 fwMngmntframesSent;
  uint32 fwMngmntFramesConfirmed;
  uint32 fwMngmntframesRecieved;
  uint32 fwctrlFramesSent;
  uint32 fwctrlFramesRecieved;
  uint32 fwMulticastReplayedPackets;
  uint32 fwPairWiseMicFailurePackets;
  uint32 groupMicFailurePackets;
  uint32 beaconProbeResponsePhyType;
  uint32 rxBasicReport;
  uint32 txFrameErrorCount;
}mtlk_wssa_drv_radio_baa_stats_t;

typedef struct mtlk_wssa_drv_tsman_init_tid_gl{

  uint32 InitTidStatsEventAddbaRequestWasSent;
  uint32 InitTidStatsEventAddbaRequestConfiremd;
  uint32 InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndValid;
  uint32 InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndInvalid;
  uint32 InitTidStatsEventAddbaResponseReceivedWithStatusFailure;
  uint32 InitTidStatsEventAddbaResponseTimeout;
  uint32 InitTidStatsEventBaAgreementOpened;
  uint32 InitTidStatsEventBarWasSent;
  uint32 InitTidStatsEventBarWasDiscarded;
  uint32 InitTidStatsEventBarWasAnsweredWithAck;
  uint32 InitTidStatsEventBarWasAnsweredWithBa;
  uint32 InitTidStatsEventInactivityTimeout;
  uint32 InitTidStatsEventDelbaWasSent;
  uint32 InitTidStatsEventDelbaReceived;
  uint32 InitTidStatsEventCloseTid;
  uint32 InitTidStatsEventOpenTid;
}mtlk_wssa_drv_tsman_init_tid_gl_t;

typedef struct mtlk_wssa_drv_tsman_init_sta_gl{
  uint32 InitStaStatsEventRemove;
  uint32 InitStaStatsEventAdd;
  uint32 InitStaStatsEventAddbaResponseReceivedWithIllegalTid;
}mtlk_wssa_drv_tsman_init_sta_gl_t;

typedef struct mtlk_wssa_drv_tsman_rcpt_tid_gl{
  uint32 RcpTidStatsEventAddbaRequestAccepted;
  uint32 RcpTidStatsEventAddbaRequestWasReceivedWithInvalidParameters;
  uint32 RcpTidStatsEventAddbaResponseWasSentWithStatusSuccess;
  uint32 RcpTidStatsEventAddbaResponseWasSentWithStatusFailure;
  uint32 RcpTidStatsEventBaAgreementOpened;
  uint32 RcpTidStatsEventBaAgreementClosed;
  uint32 RcpTidStatsEventBarWasReceivedWithBaAgreementOpened;
  uint32 RcpTidStatsEventInactivityTimeout;
  uint32 RcpTidStatsEventDelbaReceived;
  uint32 RcpTidStatsEventDelbaWasSent;
  uint32 RcpTidStatsEventIllegalAggregationWasReceived;
  uint32 RcpTidStatsEventCloseTid;
}mtlk_wssa_drv_tsman_rcpt_tid_gl_t;

typedef struct mtlk_wssa_drv_tsman_rcpt_sta_gl{
  uint32 RcptStaStatsEventRemove;
  uint32 RcptStaStatsEventAdd;
  uint32 RcptStaStatsEventAddbaRequestReceivedWithIllegalTid;
}mtlk_wssa_drv_tsman_rcpt_sta_gl_t;

typedef struct mtlk_wssa_drv_radio_link_adapt_stats{
  uint32 minPower[NUM_OF_BW];
  uint32 maxPower[NUM_OF_BW];
  uint32 bfReportReceivedCounter;
  uint32 protectionSentCounter;
  uint32 protectionSucceededCounter;
  uint32 rxDc[NUM_OF_BW];
  uint32 txLo[NUM_OF_BW];
  uint32 rxIq[NUM_OF_BW];
  uint32 txIq[NUM_OF_BW];
}mtlk_wssa_drv_radio_link_adapt_stats_t;

typedef struct mtlk_wssa_drv_multicast_stats{
  uint32 notEnoughClonePds;
  uint32 allClonesFinishedTransmission;
  uint32 noStationsInGroup;
}mtlk_wssa_drv_multicast_stats_t;

typedef struct mtlk_wssa_drv_training_man_stats{
  uint32 trainingTimerExpiredCounter;
  uint32 trainingStartedCounter;
  uint32 trainingFinishedSuccessfullyCounter;
  uint32 trainingFinishedUnsuccessfullyCounter;
  uint32 trainingNotStartedCounter;
}mtlk_wssa_drv_training_man_stats_t;

typedef struct mtlk_wssa_drv_group_man_stats{
  uint32 groupInfoDb;
}mtlk_wssa_drv_group_man_stats_t;

typedef struct mtlk_wssa_drv_general_stats{
  uint32 calibrationMask;
  uint32 onlineCalibrationMask;
  uint32 dropReasonClassifier;
  uint32 dropReasonDisconnect;
  uint32 dropReasonATF;
  uint32 dropReasonTSFlush;
  uint32 dropReasonReKey;
  uint32 dropReasonSetKey;
  uint32 dropReasonDiscard;
  uint32 dropReasonDsabled;
  uint32 dropReasonAggError;
  uint32 defragStart;
  uint32 defragEnd;
  uint32 defragTimeout;
  uint32 classViolationErrors;
  uint32 rxhErrors;
  uint32 rxSmps;
  uint32 rxOmn;
  uint32 txMngFromHost;
  uint32 txDataFromHost;
  uint32 cyclicBufferOverflow;
  uint32 beaconsTransmitted;
  uint32 debugStatistics;
  uint32 exceedsETSIMaxSizeUnicast;
  uint32 exceedsETSIMaxSizeMulticast;
}mtlk_wssa_drv_general_stats_t;

typedef struct mtlk_wssa_drv_cur_channel_stats{
  /* Time values in microseconds */
  uint64 chUtilizationActive;
  uint64 chUtilizationBusy;
  uint64 chUtilizationBusyTx;
  uint64 chUtilizationBusyRx;
  uint64 chUtilizationBusySelf;
  uint64 chUtilizationInterf;
  uint64 chUtilizationIdle;
  uint64 chUtilizationBusyExt;
  uint32 chUtilization; /* percents */
  int32  channelMax80211Rssi; /* dBm */
  uint32 channelNum;
}mtlk_wssa_drv_cur_channel_stats_t;

typedef struct mtlk_wssa_drv_radio_phy_rx_stats{
  uint32 channel_load;
  int32  chNon80211Noise;
  int32  CWIvalue;
  uint32 channelNum;
  int32  txPower;
  uint32 irad;
  uint32 tsf;
  uint32 extStaRx;
  int8   noise[WAVE_STAT_MAX_ANTENNAS];
  uint8  rf_gain[WAVE_STAT_MAX_ANTENNAS];
  int8   backgroundNoise[WAVE_STAT_MAX_ANTENNAS];
  uint8  activeAntMask;
}mtlk_wssa_drv_radio_phy_rx_stats_t;

typedef struct mtlk_wssa_drv_dynamic_bw_stats{
  uint32  dynamicBW20MHz;
  uint32  dynamicBW40MHz;
  uint32  dynamicBW80MHz;
  uint32  dynamicBW160MHz;
}mtlk_wssa_drv_dynamic_bw_stats_t;

typedef struct mtlk_wssa_drv_radio_la_mu_vht_stats{
  uint32 protectionSentCounter;
  uint32 protectionSucceededCounter;
  uint32 DataPhyMode;
  uint32 powerData[NUM_OF_BW];
  uint32 scpData[NUM_OF_BW];
  uint32 dataBwLimit;
  uint32 groupGoodput;
  uint32 raState;
  uint32 raStability[MAX_USP_IN_VHT_GROUP];
  uint32 nssData[MAX_USP_IN_VHT_GROUP];
  uint32 mcsData[MAX_USP_IN_VHT_GROUP];
}mtlk_wssa_drv_radio_la_mu_vht_stats_t;
#endif /* CONFIG_WAVE_DEBUG */
#endif /* __MTLK_WSSA_DRVINFO_H__ */
