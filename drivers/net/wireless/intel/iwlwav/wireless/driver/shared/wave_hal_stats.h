/************************************************************************************
*
*  File:        wave_hal_stats.h
*  Description:
*  COPYRIGHT:
*    Copyright (c) 2023, MaxLinear, Inc.
*    (C) Intel India Ltd.
*    All rights are strictly reserved. Reproduction or divulgence in any
*    form whatsoever is not permitted without written authority from the
*    copyright owner. Issued by Intel India Ltd
*
**************************************************************************************/
#ifndef _HAL_STATISTICS_H_
#define _HAL_STATISTICS_H_

#include "vendor_cmds.h"

#define HAL_NUM_OF_ANTS             (8)
#define HAL_NUM_OF_BANDWIDTHS       (4)
#define HAL_NUM_OF_TIDS             (16)

/* Defined as per Wifi HAL values
 * 0 for 20Mhz, 1 for 40 MHz, 2 for 80 MHz, 2 for 160 MHz */
#define HAL_BW_20MHZ                (0)
#define HAL_BW_40MHZ                (1)
#define HAL_BW_80MHZ                (2)
#define HAL_BW_160MHZ               (2)

#define NUM_OF_RX_FRAMES       (4)
#define HAL_LEGACY_NSS         (1)

#define HAL_MAX_RU_ALLOCATIONS_DRV (74)
#define HAL_MAX_BSR                (32)
#define HAL_MAX_BSR_DRV            (8)

#define NUMBER_OF_RX_ANTENNAS      (4)

#ifdef CONFIG_WAVE_DEBUG
#define RATES_BIT_MASK_SIZE        (3)
#define NUM_OF_TIDS                (8)
#define PHY_STATISTICS_MAX_RX_ANT  (4)
#endif /* CONFIG_WAVE_DEBUG */

typedef struct _WifiAssociatedDev2_t
{
  uint32 DataFramesSentAck;
  uint32 DataFramesSentNoAck;
  uint32 BytesSent;
  uint32 BytesReceived;
  uint32 LastDataDownlinkRate;
  uint32 LastDataUplinkRate;
  uint32 Retransmissions;
  uint32 Disassociations;
  uint32 AuthenticationFailures;
  uint32 Associations;
  int32  SignalStrength;
  int32  RSSI[4];
  int32  MinRSSI;
  int32  MaxRSSI;
  unsigned char IPAddress[64];
  unsigned char OperatingStandard[64];
  unsigned char OperatingChannelBandwidth[64];
  char InterferenceSources[64];
  int8 SNR[4];
  IEEE_ADDR MACAddress;
  bool AuthenticationState;
  bool Active;
} wifiAssociatedDevDiagnostic2_t;


/* below Enum is defined similar to wifi_hal.h, for validating the Tx/Rx stats */
enum Tx_Rx_flag {
  HAS_BYTES,
  HAS_MSDUS,
  HAS_MPDUS,
  HAS_PPDUS,
  HAS_BW_80P80,
  HAS_RSSI_COMB,
  HAS_RSSI_ARRAY,
};

/* Structures defined in this file are similar to wifi_hal.h structures which are used by DWPAL.
 * DWPAL fetch these structures and forward it to wifi hal as part of PLUME API's */
typedef struct _rssiSnapshot_t
{
  uint8    rssi[NUM_OF_RX_FRAMES];      /* Last 4 RSSI frames received */
  uint8    time_s[NUM_OF_RX_FRAMES];    /* Time of when last 4 RSSI were received */
  uint16   count;        /* Sequence numer of received managemant (bcn, ack) frames */
} rssiSnapshot_t;

typedef struct _PeerFlowStats_t {
  uint64 cli_rx_bytes;
  uint64 cli_tx_bytes;
  uint64 cli_rx_frames;
  uint64 cli_tx_frames;
  uint64 cli_rx_retries;
  uint64 cli_tx_retries;
  uint64 cli_rx_errors;
  uint64 cli_tx_errors;
  uint64 cli_rx_rate;
  uint64 cli_tx_rate;
  rssiSnapshot_t cli_rssi_bcn;
  rssiSnapshot_t cli_rssi_ack;
} peerFlowStats;

typedef struct _PerRatePERStats_t {
  uint8 mcs;
  uint8 nss;
  uint8 gi;
  uint8 reserved1;
  uint16 bw;
  uint16 reserved2;
  uint32 packet_error_rate;
} perRatePERStats;

typedef struct _mtlk_priority_gpio_stats_t {
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
} mtlk_priority_gpio_stats_t;

typedef struct _PeerRateInfoRxStats_t {
  uint8 nss;
  uint8 mcs;
  uint16 bw;
  uint64 flags;
  uint64 bytes;
  uint64 msdus;
  uint64 mpdus;
  uint64 ppdus;
  uint64 retries;
  uint8 rssi_combined;
  uint8 rssi_array[HAL_NUM_OF_ANTS][HAL_NUM_OF_BANDWIDTHS];
} peerRateInfoRxStats;

typedef struct _PeerRateInfoTxStats_t {
  uint8 nss;
  uint8 mcs;
  uint16 bw;
  uint64 flags;
  uint64 bytes;
  uint64 msdus;
  uint64 mpdus;
  uint64 ppdus;
  uint64 retries;
  uint64 attempts;
} peerRateInfoTxStats;

/* This data structure must be aligned to wifi hal wifi_channelStats_t */
typedef struct _ChannelStatistics_t {
  int32  ch_number;                 /**< each channel is only 20MHz bandwidth */
  uint8  ch_in_pool;                /**< If ch_in_pool is false, driver do not need to scan this channel */
  int32  ch_noise;                  /**< this is used to return the average noise floor in dBm */
  uint8  ch_radar_noise;            /**< if ch_number is in DFS channel, this is used to return if radar signal is present on DFS channel (5G only) */
  int32  ch_max_80211_rssi;         /**< max RSSI from the neighbor AP in dbm on this channel */
  int32  ch_non_80211_noise;        /**< average non 802.11 noise in dBm */
  int32  ch_utilization;            /**< this is used to return the 802.11 utilization in percent */
  uint64 ch_utilization_total;      /**< Total time radio spent receiveing or transmitting on that channel (ch_utilization_active) */
  uint64 ch_utilization_busy;       /**< Time radio detected that channel was busy (Busy = Rx + Tx + Interference) */
  uint64 ch_utilization_busy_tx;    /**< Time time radio spent transmitting on channel */
  uint64 ch_utilization_busy_rx;    /**< Time radio spent receiving on channel (Rx = Rx_obss + Rx_self + Rx_errr (self and obss errors) */
  uint64 ch_utilization_busy_self;  /**< Time radio spent receiving on channel from its own connected clients */
  uint64 ch_utilization_busy_ext;   /**< Time radio detected that extended channel was busy (40MHz extention channel busy */
} wifi_channelStats_t;

typedef struct {
  wifi_channelStats_t      wifi_chan_stats;
  mtlk_osal_hr_timestamp_t chan_stat_ts;     /* timestamp */
  mtlk_osal_hr_timestamp_t chan_util_interf; /* Interference */
  mtlk_osal_hr_timestamp_t chan_active_time;
  mtlk_osal_hr_timestamp_t chan_idle_time;
} wave_drv_channel_stats_t;

typedef struct _PhyChannelStatistics_t {
  int8 noise;                 /* noise                        dBm */
  uint8 ch_load;              /* channel_load             0..100% */
  uint8 ch_util;              /* totalChannelUtilization  0..100% */
  uint8 airtime;              /* 0..100%                          */
  uint32 airtime_efficiency;  /* bytes/sec                        */
} mtlk_phy_channel_status;

typedef struct _PeerTidStats_t {
  uint8 ac;
  uint8 tid;
  uint64 ewma_time_ms;
  uint64 sum_time_ms;
  uint64 num_msdus;
} peerTidStats[HAL_NUM_OF_TIDS];

/* kernel AC type definition differs from the wifi hal definiton. Hence we are using the type from wifi hal */
typedef enum
{
    WIFI_RADIO_QUEUE_TYPE_VI = 0,
    WIFI_RADIO_QUEUE_TYPE_VO,
    WIFI_RADIO_QUEUE_TYPE_BE,
    WIFI_RADIO_QUEUE_TYPE_BK,
    WIFI_RADIO_QUEUE_TYPE_CAB,
    WIFI_RADIO_QUEUE_TYPE_BCN,
    WIFI_RADIO_QUEUE_MAX_QTY,
    WIFI_RADIO_QUEUE_TYPE_NONE = -1
} wifi_radioQueueType_t;

/* Result3 data structures */
typedef enum {
    wlanAccessCategoryBackground,
    wlanAccessCategoryBestEffort,
    wlanAccessCategoryVideo,
    wlanAccessCategoryVoice,
} wlanAccessCategory_t;

typedef enum {
    WIFI_RU_TYPE_26,
    WIFI_RU_TYPE_52,
    WIFI_RU_TYPE_106,
    WIFI_RU_TYPE_242,
    WIFI_RU_TYPE_484,
    WIFI_RU_TYPE_996,
    WIFI_RU_TYPE_1024,
} wlanRuType_t;

typedef enum {
    WIFI_DL_MU_TYPE_NONE,
    WIFI_DL_MU_TYPE_HE,
    WIFI_DL_MU_TYPE_MIMO,
    WIFI_DL_MU_TYPE_HE_MIMO
} wlanDlMuType_t;

typedef struct {
    wlanAccessCategory_t  accessCategory;
    uint32 queueSize;
} wlanBsr_t;

typedef struct {
    unsigned char   subchannels;
    wlanRuType_t  type;
} wlanRuAllocation_t;

typedef enum {
    WIFI_UL_MU_TYPE_NONE,
    WIFI_UL_MU_TYPE_HE,
} wlanUlMuType_t;

typedef enum {
    wlanTwtAgreementTypeIndividual,
    wlanTwtAgreementTypeBroadcast,
} wlanTwtAgreementType_t;

typedef struct {
    bool implicit;
    bool announced;
    bool triggerEnabled;
} wlanTwtOperation_t;

typedef struct {
    uint32 wakeTime;
    uint32 wakeInterval;
    uint32 minWakeDuration;
    uint32 channel;
} wlanTwtIndividualParams_t;

typedef struct {
    uint32 tragetBeacon;
    uint32 listenInterval;
} wlanTwtBroadcastParams_t;

typedef struct {
    wlanDlMuType_t       DownlinkMuType;
    wlanBsr_t            BufferStatus[HAL_MAX_BSR];
    unsigned char        AllocatedDownlinkRuNum;
    wlanRuAllocation_t   DownlinkRuAllocations[HAL_MAX_RU_ALLOCATIONS_DRV];
} wlanDlMuStats_t;

typedef struct {
    wlanUlMuType_t       UpinkMuType;
    unsigned char        ChannelStateInformation;
    wlanBsr_t            BufferStatus[HAL_MAX_BSR];
    unsigned char        AllocatedUplinkRuNum;
    wlanRuAllocation_t   UplinkRuAllocations[HAL_MAX_RU_ALLOCATIONS_DRV];
} wlanUlMuStats_t;

typedef struct {
    wlanTwtAgreementType_t  agreementType;
    wlanTwtOperation_t      operation;
    union {
        wlanTwtIndividualParams_t  individual;
        wlanTwtBroadcastParams_t   broadcast;
    } patams;
} wlanTwtParams_t;

typedef struct _WifiAssociatedDev3_t{
      uint64 PacketsSent;
      uint64 PacketsReceived;
      uint64 ErrorsSent;
      uint64 RetransCount;
      uint64 FailedRetransCount;
      uint64 RetryCount;
      uint64 MultipleRetryCount;
      uint64 TxFrames;
      uint64 RxRetries;
      uint64 RxErrors;
      uint32 MaxDownlinkRate;
      uint32 MaxUplinkRate;
      uint32 ActiveNumSpatialStreams;
      wifiAssociatedDevDiagnostic2_t wifiAssociatedDevDiagnostic2;
      wlanUlMuStats_t  UplinkMuStats;
      wlanDlMuStats_t  DownlinkMuStats;
      wlanTwtParams_t  TwtParams;
} wifiAssociatedDevDiagnostic3_t;

typedef struct _WifiAssociatedStationRateInfo {
  uint32 InfoFlag;
  uint32 DataRate;
  uint32 Sgi;
  int32 CbwIdx;
  int32 Nss;
} wifiAssociatedStationRateInfo_t;

typedef struct _WifiAssociatedStationStats {
  uint64 BytesSent;
  uint64 BytesReceived;
  wifiAssociatedStationRateInfo_t TxDataRateInfo;
  wifiAssociatedStationRateInfo_t RxDataRateInfo;
  int32 RSSI[4];
  uint8 IsFilled;
} wifiAssociatedStationStats_t;

#define MAX_NR            5
#define MAX_NR_700        4
#define MAX_NC            4
#define MAX_SUB_CARRIERS  48 /* only 20MHz is supported for CSI*/
#define CSI_RAW_DATA_SIZE 788
#define MAX_NR_SIZE       10 /* Includes 5 dummy words */

enum WAVE_CSI_BW_MODE {
  CSI_BW_20,
  CSI_BW_40,
  CSI_BW_80,
  CSI_BW_160,
  CSI_BW_320,
};

typedef signed short wifi_streams_rssi_t [MAX_NR];
typedef unsigned int wifi_carrier_data_t [MAX_NR][MAX_NC];
typedef wifi_carrier_data_t wifi_csi_matrix_t [MAX_SUB_CARRIERS];

typedef struct _wifi_frame_info
{
    unsigned char bw_mode;
    unsigned char mcs;
    unsigned char Nr;
    unsigned char Nc;
    wifi_streams_rssi_t nr_rssi;
    uint32 channel;
    unsigned short valid_mask;
    unsigned short phy_bw;
    unsigned short cap_bw;
    uint32 num_sc;
    unsigned char decimation;
    int16 frequency_offset;
    uint64 time_stamp;
} __attribute__((aligned(1), packed)) wifi_frame_info_t;

typedef struct _wifi_csi_driver_data_t {
  wifi_frame_info_t frame_info;
  int csi_raw_data[CSI_RAW_DATA_SIZE];
} wifi_csi_driver_data_t;

typedef struct _wifi_csi_driver_nl_event_data_t {
  IEEE_ADDR sta_addr; /* Station MAC addr */
  wifi_csi_driver_data_t csi_data;
} wifi_csi_driver_nl_event_data_t;

/*
  _wifi_frame_info_hw is populated from the CSI data header coming from PHY.
  PHY can only write a valid word followed by a dummy word - PHY limitation.
  To overcome PHY limitation and memcpy the data in Driver in one shot,
  Driver introduced dummy words after every valid word
*/

typedef struct _wifi_frame_info_hw
{
    unsigned char bw_mode;
    unsigned char mcs;
    uint16 dummy_1;
    unsigned char Nr;
    unsigned char Nc;
    uint16 dummy_2;
    signed short nr_rssi[MAX_NR_SIZE];
    uint16 channel;
    uint16 dummy_3;
    uint16 channel_reserved;
    uint16 dummy_4;
    unsigned short valid_mask;
    uint16 dummy_5;
    unsigned short phy_bw;
    uint16 dummy_6;
    unsigned short cap_bw;
    uint16 dummy_7;
    uint16 num_sc;
    uint16 dummy_8;
    uint16 num_sc_reserved;
    uint16 dummy_9;
    unsigned char decimation;
    unsigned char dummy_10[3];
    int16 frequency_offset;
    uint16 dummy_11;
    uint64 time_stamp;
} __attribute__((aligned(1), packed)) wifi_frame_info_hw_t;

typedef struct _wifi_csi_driver_data_hw_t {
  wifi_frame_info_hw_t frame_info;
  int csi_raw_data[CSI_RAW_DATA_SIZE];
} __attribute__((aligned(1), packed)) wifi_csi_driver_data_hw_t;

#ifdef CONFIG_WAVE_DEBUG
typedef struct _PeerHostIfQos_t {
  uint32 qosByteCountSta[NUM_OF_TIDS];
  uint32 qosTxSta[NUM_OF_TIDS];
} peerHostIfQos_t;

typedef struct _PeerHostIf_t {
  uint64 rxOutStaNumOfBytes;
  uint64 agerPdNoTransmitCountSta;
} peerHostIf_t;

typedef struct _PeerRxStats_t {
  uint64 rddelayed;
  uint64 swUpdateDrop;
  uint64 rdDuplicateDrop;
  uint64 missingSn;
} peerRxStats_t;

typedef struct _PeerUlBsrcTidStats_t {
  uint32 bufStsCnt0;
  uint32 bufStsCnt1;
  uint32 bufStsCnt2;
  uint32 bufStsCnt3;
  uint32 bufStsCnt4;
  uint32 bufStsCnt5;
  uint32 bufStsCnt6;
  uint32 bufStsCnt7;
} peerUlBsrcTidStats_t;

typedef struct _PeerBaaStats_t {
  uint64 mpduFirstRetransmission;
  uint64 mpduTransmitted;
  uint64 mpduByteTransmitted;
  uint64 mpduRetransmission;
  uint64 channelTransmitTime;
} peerBaaStats_t;

typedef struct _LinkAdaptionStats_t {
  uint32 DataPhyMode;
  uint32 ManagementPhyMode;
  uint32 powerData;
  uint32 powerManagement;
  uint32 antennaSelectionData;
  uint32 antennaSelectionManagement;
  uint32 scpData;
  uint32 stationCapabilities;
  uint32 dataBwLimit;
  uint32 bfModeData;
  uint32 stbcModeData;
  uint32 raState;
  uint32 raStability;
  uint32 nssData;
  uint32 nssManagement;
  uint32 mcsData;
  uint32 mcsManagement;
  uint32 bw;
  uint32 transmittedAmpdu;
  uint32 totalTxTime;
  uint32 mpduInAmpdu;
  uint32 ewmaTimeNsec;
  uint32 sumTimeNsec;
  uint32 numMsdus;
  uint32 per;
  uint32 totalPsduLength;
  uint32 totalZld;
  uint32 ratesMask[RATES_BIT_MASK_SIZE];
} linkAdaptionStats_t;

typedef struct _PlanManagerStats_t {
  uint32 uplinkMuType;
  uint32 ulRuSubChannels;
  uint32 ulRuType;
  uint32 downlinkMuType;
  uint32 dlRuSubChannels;
  uint32 dlRuType;
} planManagerStats_t;

typedef struct _TwtStats_t {
  uint32 numOfAgreementsForSta;
  uint32 state;
  uint32 agreementType;
  uint32 implicit;
  uint32 announced;
  uint32 triggerEnabled;
  uint32 wakeTime;
  uint32 wakeInterval;
  uint32 minWakeDuration;
  uint32 channel;
} twtStats_t;

typedef struct _PerClientStats_t {
  uint64 retryCount;
  uint64 successCount;
  uint64 exhaustedCount;
  uint64 clonedCount;
  uint64 oneOrMoreRetryCount;
  uint64 packetRetransCount;
  uint64 dropCntReasonClassifier;
  uint64 dropCntReasonDisconnect;
  uint64 dropCntReasonATF;
  uint64 dropCntReasonTSFlush;
  uint64 dropCntReasonReKey;
  uint64 dropCntReasonSetKey;
  uint64 dropCntReasonDiscard;
  uint64 dropCntReasonDsabled;
  uint64 dropCntReasonAggError;
  uint64 mpduRetryCount;
  uint64 mpduInAmpdu;
  uint64 ampdu;
} perClientStats_t;

typedef struct _PeerPhyRxStatus_t {
  int32   rssi;
  int32   avg_rcpi;
  uint32  phyRate;
  uint32  irad;
  uint32  lastTsf;
  uint32  perClientRxtimeUsage;
  uint32  psduRate;
  uint32  phyRateSynchedToPsduRate;
  int8    noise[WAVE_STAT_MAX_ANTENNAS];
  uint8   gain[WAVE_STAT_MAX_ANTENNAS];
  int8    rcpi[WAVE_STAT_MAX_ANTENNAS];
} peerPhyRxStatus_t;

typedef struct _PeerInfo_t {
  peerHostIfQos_t peer_host_if_qos;
  peerHostIf_t peer_host_if;
  peerRxStats_t peer_rx_stats;
  peerUlBsrcTidStats_t peer_ul_bsrc_tid;
  peerBaaStats_t peer_baa_stats; 
  planManagerStats_t plan_manager_stats;
  twtStats_t twt_stats;
  perClientStats_t per_client_stats;
  peerPhyRxStatus_t  phy_rx_status;
} peerInfo_t;
#endif /* CONFIG_WAVE_DEBUG */
#endif /* _HAL_STATISTICS_H_ */
