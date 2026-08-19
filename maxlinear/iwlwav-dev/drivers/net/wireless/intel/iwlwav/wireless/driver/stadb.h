/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
* $Id$
*
*
*
* Written by: Dmitry Fleytman
*
*/

#ifndef _MTLK_STADB_H_
#define _MTLK_STADB_H_

#include "mtlklist.h"
#include "mtlkqos.h"
#include "mtlkhash_ieee_addr.h"
#include "mtlk_clipboard.h"
#include "mtlkirbd.h"
#include "mtlk_wss.h"
#include "mtlk_analyzer.h"
#include "mtlkwssa_drvinfo.h"
#include "bitrate.h"
#include "eeprom.h"
#include "mtlkhal.h"
#include "mac80211.h"
#include <net/cfg80211.h>
#include "Statistics/WAVE600B/Statistics_Descriptors.h"
#include "Statistics/WAVE600D2/Statistics_Descriptors.h"
#include "Statistics/WAVE700/Statistics_Descriptors.h"
#include <net/mac80211.h>

#define   MTLK_IDEFS_ON
#include "mtlkidefs.h"

#define LOG_LOCAL_GID   GID_STADB
#define LOG_LOCAL_FID   0

/* Timeout change adaptation interval */
#define DEFAULT_KEEPALIVE_TIMEOUT   1000 /* ms */
#define HOST_INACTIVE_TIMEOUT       1800 /* sec */
#define WHM_DISCNCT_TIMEOUT         3600 /* sec */
#define WHM_TXRX_STALL_TIMEOUT         1 /* sec */
#define SKB_HASH_CLEANUP_TIMEOUT      30 /* sec */

#define STR_STA_EFFECTIVE_RATES_UPDATE_TIMEOUT  1 /* sec */

#define DEFAULT_BEACON_INTERVAL (100)

#define WV_SUPP_RATES_MAX MAX_NUM_SUPPORTED_RATES
#define WV_HT_MCS_MASK_LEN 10 /* must match IEEE80211_HT_MCS_MASK_LEN from include/linux/ieee80211.h */
#define WV_VHT_MCS_INFO_NUM_FIELDS 4


#define STA_FLAGS_WDS                     MTLK_BFIELD_INFO(0, 1)
#define STA_FLAGS_WMM                     MTLK_BFIELD_INFO(1, 1)
#define STA_FLAGS_MFP                     MTLK_BFIELD_INFO(2, 1)
#define STA_FLAGS_11n                     MTLK_BFIELD_INFO(3, 1)
#define STA_FLAGS_IS_8021X_FILTER_OPEN    MTLK_BFIELD_INFO(4, 1)
#define STA_FLAGS_11ac                    MTLK_BFIELD_INFO(5, 1)
#define STA_FLAGS_PBAC                    MTLK_BFIELD_INFO(6, 1)
#define STA_FLAGS_OPMODE_NOTIF            MTLK_BFIELD_INFO(7, 1)
#define STA_FLAGS_WDS_WPA                 MTLK_BFIELD_INFO(8, 1)
#define STA_FLAGS_OMN_SUPPORTED           MTLK_BFIELD_INFO(9, 1)
#define STA_FLAGS_11ax_6ghz				  MTLK_BFIELD_INFO(14, 1)
#define STA_FLAGS_11ax                    MTLK_BFIELD_INFO(15, 1)
#define STA_FLAGS_11be                    MTLK_BFIELD_INFO(16, 1)

#define BSS_COEX_IE_40_MHZ_INTOLERANT               MTLK_BFIELD_INFO(1, 1)
#define BSS_COEX_IE_20_MHZ_BSS_WIDTH_REQUEST        MTLK_BFIELD_INFO(2, 1)
#define BSS_COEX_IE_OBSS_SCANNING_EXEMPTION_REQUEST MTLK_BFIELD_INFO(3, 1)
#define BSS_COEX_IE_OBSS_SCANNING_EXEMPTION_GRANT   MTLK_BFIELD_INFO(4, 1)

#define RX_HE_MCS_MAP_LESS_OR_EQUAL_80_OFFSET 0
#define TX_HE_MCS_MAP_LESS_OR_EQUAL_80_OFFSET 2
#define RX_HE_MCS_MAP_160_OFFSET              4
#define TX_HE_MCS_MAP_160_OFFSET              6
#define RX_HE_MCS_MAP_80_PLUS_80_OFFSET       8
#define TX_HE_MCS_MAP_80_PLUS_80_OFFSET       10

#define MAX_HE_MCS_1_SS                   MTLK_BFIELD_INFO(0, 2)
#define MAX_HE_MCS_2_SS                   MTLK_BFIELD_INFO(2, 2)
#define MAX_HE_MCS_3_SS                   MTLK_BFIELD_INFO(4, 2)
#define MAX_HE_MCS_4_SS                   MTLK_BFIELD_INFO(6, 2)
#define MAX_HE_MCS_5_SS                   MTLK_BFIELD_INFO(8, 2)
#define MAX_HE_MCS_6_SS                   MTLK_BFIELD_INFO(10, 2)
#define MAX_HE_MCS_7_SS                   MTLK_BFIELD_INFO(12, 2)
#define MAX_HE_MCS_8_SS                   MTLK_BFIELD_INFO(14, 2)

#define HE_MCS_N_SS(he_mcs_map, n)                \
  MTLK_BFIELD_GET(he_mcs_map, MAX_HE_MCS_##n_SS)

#define MTLK_STA_HTCAP_LDPC_SUPPORTED           MTLK_BFIELD_INFO(0, 1)
#define MTLK_STA_HTCAP_CB_SUPPORTED             MTLK_BFIELD_INFO(1, 1)
#define MTLK_STA_HTCAP_SGI20_SUPPORTED          MTLK_BFIELD_INFO(5, 1)
#define MTLK_STA_HTCAP_SGI40_SUPPORTED          MTLK_BFIELD_INFO(6, 1)
#define MTLK_STA_HTCAP_MIMO_CONFIG_TX           MTLK_BFIELD_INFO(7, 1)
#define MTLK_STA_HTCAP_MIMO_CONFIG_RX           MTLK_BFIELD_INFO(8, 2)
#define MTLK_STA_HTCAP_40MHZ_INTOLERANT         MTLK_BFIELD_INFO(14, 1)

#define MTLK_STA_AMPDU_PARAMS_MAX_LENGTH_EXP    MTLK_BFIELD_INFO(0, 2)
#define MTLK_STA_AMPDU_PARAMS_MIN_START_SPACING MTLK_BFIELD_INFO(2, 3)

#define MAX_WHM_DISCONNECTIONS_PER_CYCLE 10 /* fine-tune this value in release Images */

#ifdef MTLK_PER_RATE_STAT

/* not used currently, will be limited by the
 * max number of stations able to connect */
#define MTLK_PER_RATE_STAT_MAX_STA_NUM 30

#define MTLK_PACK_ON
#include "mtlkpack.h"

typedef union _mtlk_rate_key
{
    struct {
       uint8 mcs;
       uint8 nss;
       uint16 bw;
    } key;
    uint32 complex_key;
} __MTLK_PACKED mtlk_rate_key;

typedef union _mtlk_per_key
{
    struct {
       uint8 mcs;
       uint8 nss;
       uint8 gi;
       uint8 bw;
    } key;
    uint32 complex_key;
} __MTLK_PACKED mtlk_per_key;

#define MTLK_PACK_OFF
#include "mtlkpack.h"

static __INLINE uint32
_mtlk_hash_rate_hashval (const mtlk_rate_key *key, uint32 nof_buckets)
{
  return key->complex_key % (nof_buckets -1);
}

static __INLINE int
_mtlk_hash_rate_keycmp (const mtlk_rate_key *key1, const mtlk_rate_key *key2)
{
  return key1->complex_key - key2->complex_key;
}

static __INLINE uint32
_mtlk_hash_per_hashval (const mtlk_per_key *key, uint32 nof_buckets)
{
  return key->complex_key % (nof_buckets -1);
}

static __INLINE int
_mtlk_hash_per_keycmp (const mtlk_per_key *key1, const mtlk_per_key *key2)
{
  return key1->complex_key - key2->complex_key;
}

MTLK_HASH_DECLARE_ENTRY_T(rate, mtlk_rate_key);
MTLK_HASH_DECLARE_ENTRY_T(per, mtlk_per_key);

MTLK_HASH_DECLARE_EXTERN(rate, mtlk_rate_key);
MTLK_HASH_DECLARE_EXTERN(per, mtlk_per_key);

typedef struct _mtlk_rate_stat_tx{
  MTLK_HASH_ENTRY_T(rate) hentry;
  peerRateInfoTxStats tx;
} mtlk_rate_stat_tx;

typedef struct _mtlk_rate_stat_rx{
  MTLK_HASH_ENTRY_T(rate) hentry;
  peerRateInfoRxStats rx;
} mtlk_rate_stat_rx;

typedef struct _mtlk_rate_stat_per{
  MTLK_HASH_ENTRY_T(per) hentry;
  perRatePERStats per;
} mtlk_rate_stat_per;

#endif /* MTLK_PER_RATE_STAT*/

typedef struct _peer_tx_rx_stall_warning{
 uint64 prev_tx_successcount;
 uint64 prev_tx_retryCount;
 uint32 tx_warn_count;
 uint64 prev_rx_count;
 uint64 prev_rx_retryCount;
 uint32 rx_warn_count;
} peer_tx_rx_stall_warning;

typedef enum
{
/* minimal statistic */

  MTLK_STAI_CNT_DISCARD_PACKETS_RECEIVED,

#if (!(MTLK_MTIDL_PEER_STAT_FULL))
  MTLK_STAI_CNT_LAST,
#endif /* MTLK_MTIDL_PEER_STAT_FULL */

  MTLK_STAI_CNT_RX_PACKETS_DISCARDED_DRV_DUPLICATE,
  MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DRV_NO_RESOURCES,
  MTLK_STAI_CNT_TX_PACKETS_DISCARDED_SQ_OVERFLOW,
  MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DROP_ALL_FILTER,
  MTLK_STAI_CNT_TX_PACKETS_DISCARDED_TX_QUEUE_OVERFLOW,

  MTLK_STAI_CNT_802_1X_PACKETS_RECEIVED,                /* number of 802_1x packets received */
  MTLK_STAI_CNT_802_1X_PACKETS_SENT,                    /* number of 802_1x packets transmitted */
  MTLK_STAI_CNT_802_1X_PACKETS_DISCARDED,               /* number of 802_1x packets discarded */

  MTLK_STAI_CNT_FWD_RX_PACKETS,
  MTLK_STAI_CNT_FWD_RX_BYTES,
  MTLK_STAI_CNT_PS_MODE_ENTRANCES,

/* DEBUG statistic */
#if MTLK_MTIDL_PEER_STAT_FULL
  MTLK_STAI_CNT_LAST
#endif /* MTLK_MTIDL_PEER_STAT_FULL */

} sta_info_cnt_id_e;

/* Statistic ALLOWED flags */
#define MTLK_STAI_CNT_DISCARD_PACKETS_RECEIVED_ALLOWED                  MTLK_WWSS_WLAN_STAT_ID_DISCARD_PACKETS_RECEIVED_ALLOWED

#define MTLK_STAI_CNT_RX_PACKETS_DISCARDED_DRV_DUPLICATE_ALLOWED        MTLK_WWSS_WLAN_STAT_ID_RX_PACKETS_DISCARDED_DRV_DUPLICATE_ALLOWED
#define MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DRV_NO_RESOURCES_ALLOWED     MTLK_WWSS_WLAN_STAT_ID_TX_PACKETS_DISCARDED_DRV_NO_RESOURCES_ALLOWED
#define MTLK_STAI_CNT_TX_PACKETS_DISCARDED_SQ_OVERFLOW_ALLOWED          MTLK_WWSS_WLAN_STAT_ID_TX_PACKETS_DISCARDED_DRV_SQ_OVERFLOW_ALLOWED
#define MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DROP_ALL_FILTER_ALLOWED      MTLK_WWSS_WLAN_STAT_ID_TX_PACKETS_DISCARDED_DRV_DROP_ALL_FILTER_ALLOWED
#define MTLK_STAI_CNT_TX_PACKETS_DISCARDED_TX_QUEUE_OVERFLOW_ALLOWED    MTLK_WWSS_WLAN_STAT_ID_TX_PACKETS_DISCARDED_DRV_TX_QUEUE_OVERFLOW_ALLOWED
#define MTLK_STAI_CNT_802_1X_PACKETS_RECEIVED_ALLOWED                   MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_RECEIVED_ALLOWED
#define MTLK_STAI_CNT_802_1X_PACKETS_SENT_ALLOWED                       MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_SENT_ALLOWED
#define MTLK_STAI_CNT_802_1X_PACKETS_DISCARDED_ALLOWED                  MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_DISCARDED_ALLOWED
#define MTLK_STAI_CNT_FWD_RX_PACKETS_ALLOWED                            MTLK_WWSS_WLAN_STAT_ID_FWD_RX_PACKETS_ALLOWED
#define MTLK_STAI_CNT_FWD_RX_BYTES_ALLOWED                              MTLK_WWSS_WLAN_STAT_ID_FWD_RX_BYTES_ALLOWED
#define MTLK_STAI_CNT_PS_MODE_ENTRANCES_ALLOWED                         MTLK_WWSS_WLAN_STAT_ID_PS_MODE_ENTRANCES_ALLOWED

typedef enum {
  MTLK_PCKT_FLTR_ALLOW_ALL,
  MTLK_PCKT_FLTR_ALLOW_802_1X,
  MTLK_PCKT_FLTR_DISCARD_ALL
} mtlk_pckt_filter_e;

typedef struct devicePhyRxStatusDb
{
  uint8    noise[WAVE_STAT_MAX_ANTENNAS];
  uint8    rf_gain[WAVE_STAT_MAX_ANTENNAS];
  uint32   irad;
  uint32   tsf;
  uint8    channel_load;
  uint8    totalChannelUtilization;    /* Total Channel Utilization is an indication of how congested the medium is (all APs) */
  int8     chNon80211Noise;
  int8     CWIvalue;
  uint32   extStaRx;
  int16    txPower;
  uint8    channelNum;
  uint8    activeAntMask;
  uint16   backgroundNoise[WAVE_STAT_MAX_ANTENNAS]; /* Long term average Background Noise per antenna */
  int8     zwdfsAntRssi;
}devicePhyRxStatusDb_t;

typedef struct stationPhyRxStatusDb
{
  int8             rssi[WAVE_STAT_MAX_ANTENNAS];
  uint32           phyRate;    /* 17 bits [20:4] */
  uint32           irad;
  uint32           lastTsf;
  uint32           perClientRxtimeUsage;
  uint8            noise[WAVE_STAT_MAX_ANTENNAS];
  uint8            gain[WAVE_STAT_MAX_ANTENNAS];
  uint8            evm[WAVE_STAT_MAX_ANTENNAS];
  rssiSnapshot_t   rssiAckAvarage; /* AP_MODE: RSSI from last 4 ack received */
  uint32           psduRate; /* PSDU RATE is copied from phy metrics. It is constructed from MCS & NSS [bits 0..7], PHY MODE [bits 8..10], BW [bits 11..12], SCP [bit 13], HE CP [bits 14..15] */
  uint8            rssiArray[WAVE_STAT_MAX_ANTENNAS][NUM_OF_EXTENTION_RSSI];   /* <! 4=antennas, 4=20+20+40+80 extension rssi */

  /* Per antenna RSSI (above noise floor) for all widths (primary,secondary)
  -----------------------------------------------
  | chain_num |  20MHz [pri20                   ]
  |           |  40MHZ [pri20,sec20             ]
  |           |  80MHz [pri20,sec20,sec40,      ]
  |           | 160MHz [pri20,sec20,sec40,sec80 ]
  -----------------------------------------------
  |  1        |  rssi  [pri20,sec20,sec40,sec80 ]
  |  ...      |  ...
  |  8        |  rssi  [pri20,sec20,sec40,sec80 ]
  ----------------------------------------------- */
  uint32 phyRateSynchedToPsduRate; /* Phy rate extraced by RX HANDLER from PHY METRICS (the above phy rate is extracted by PAC Extrapolator) */


  /* Last possible word num is 14 (total of 15 words that can be configured to pac Extrap for 4 bits parameter */
}stationPhyRxStatusDb_t;

/* This data structure is only for Driver purpose due to it is not relevent to FW shared headers */
typedef struct wholePhyRxStatusDb
{
  stationPhyRxStatusDb_t     *staPhyRxStatus;
  devicePhyRxStatusDb_t      *devicePhyRxStatus;
}wholePhyRxStatusDb_t;

typedef struct mtlk_mhi_stats_sta_tx_rate{
    uint8 dataBwLimit;
    uint8 DataPhyMode;
    uint8 scpData;
    uint8 mcsData;
    uint8 nssData;
    uint8 bfModeData;
    uint8 stbcModeData;
    uint8 powerData;
    uint8 powerManagement;
    uint8 ManagementPhyMode;
    uint8 antennaSelectionData;
    uint8 antennaSelectionManagement;
    uint8 stationCapabilities;
    uint8 raState;
    uint8 raStability;
    uint8 nssManagement;
    uint8 mcsManagement;
    uint8 bw;
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
}mtlk_mhi_stats_sta_tx_rate_t;

typedef uint32      mhi_sta_cntr_t;

typedef struct mtlk_mhi_stats_lamuofdma_cntr {
  uint32 staRemovedNbiqBelowThresholdCounter;
  uint32 staRemovedFavoringSuCounter;
  uint32 staNotAddedNbiqBelowThresholdCounter;
} mtlk_mhi_stats_lamuofdma_cntr_t;

typedef enum {
    STAT_STA_CNTR_MPDU_FIRST_RETRANSMISSION = 0,
    STAT_STA_CNTR_TX_FRAMES,
    STAT_STA_CNTR_TX_BYTES,
    STAT_STA_CNTR_RX_FRAMES,
    STAT_STA_CNTR_RX_BYTES,
    STAT_STA_CNTR_RX_SW_UPDATE_DROP,
    STAT_STA_CNTR_PCT_RETRY,
    STAT_STA_CNTR_PCT_RETRY_EXHAUSTED,
    STAT_STA_CNTR_TOTAL
} mtlk_mhi_stat_sta_cntr_e;

typedef struct twt_operation{
    uint8 implicit;
    uint8 announced;
    uint8 triggerEnabled;
    uint8 reserved;
}twt_operation_t;

typedef struct twt_individual_params{
    uint64 wakeTime;
    uint32 wakeInterval;
    uint16 minWakeDuration;
    uint16 channel;
}twt_individual_params_t;

typedef struct twt_broadcast_params_t{
    uint32 tragetBeacon;
    uint32 listenInterval;
}twt_broadcast_params_t;

typedef struct twt_agreement{
    uint16 state;
    uint16 agreementType;
    twt_operation_t operation;
    union {
        twt_individual_params_t individual;
        twt_broadcast_params_t broadcast;
    } params;
}twt_agreement_t;

typedef struct twt_params{
    uint32          numOfAgreementsForSta;
    twt_agreement_t  twtIndividualAgreement[TWT_MAX_INDIVIDUAL_AGREEMENTS_ALLOWED_PER_STA];
}twt_params_t;

typedef struct ul_ru_allocation{
        uint8 subChannels;
        uint8 type;
} ul_ru_allocation_t;

typedef struct {
    wlanAccessCategory_t  accessCategory;
    uint32                queueSize;
} ul_bsr_t;

typedef struct ul_mu_stats{
        uint8              uplinkMuType;
        ul_bsr_t           ulBufferStatus[HAL_MAX_BSR];
        uint8              allocatedUplinkRuNum;
        uint8              uplinkRuNumFlag;
        ul_ru_allocation_t uplinkRuAllocations[HAL_MAX_RU_ALLOCATIONS_DRV];
} ul_mu_stats_t;

typedef struct dl_ru_allocation{
        uint8 subChannels;
        uint8 type;
} dl_ru_allocation_t;

typedef struct {
        wlanAccessCategory_t  accessCategory;
        uint32                queueSize;
} dl_bsr_t;

typedef struct dl_mu_stats{
        uint8              downlinkMuType;
        dl_bsr_t           dlBufferStatus[HAL_MAX_BSR];
        uint8              allocatedDownlinkRuNum;
        uint8              downlinkRuNumFlag;
        dl_ru_allocation_t downlinkRuAllocations[HAL_MAX_RU_ALLOCATIONS_DRV];
} dl_mu_stats_t;

typedef struct wave_sta_ofdma_ppdus_stats64 {
    uint64 total_ppdus;
    uint64 su_ppdus[MAX_NUM_OF_BW];
    uint64 mu_ofdma_ppdus;
    uint64 mu_mimo_ppdus;
    uint64 mu_ppdus;
    uint64 mu_ru_ppdus[MAX_NUM_OF_RU_SIZES];
} wave_sta_ofdma_ppdus_stats64_t;

typedef struct wave_sta_ofdma_ppdus_stats {
    uint32 total_ppdus;
    uint16 su_ppdus[MAX_NUM_OF_BW];
    uint16 mu_ofdma_ppdus;
    uint16 mu_mimo_ppdus;
    uint16 mu_ru_ppdus[MAX_NUM_OF_RU_SIZES];
} wave_sta_ofdma_ppdus_stats_t;

typedef struct wave_sta_ofdma_su_mu_stats {
    uint32 success_mpdus;
    uint32 failed_mpdus;
} wave_sta_ofdma_su_mu_stats_t;

typedef struct wave_sta_ofdma_mpdus_stats {
    wave_sta_ofdma_su_mu_stats_t su_mpdus[MAX_NUM_OF_BW];
    wave_sta_ofdma_su_mu_stats_t mu_mpdus[MAX_NUM_OF_RU_SIZES];
} wave_sta_ofdma_mpdus_stats_t;

typedef struct wave_sta_ofdma_su_mu_stats64 {
    uint64 success_mpdus;
    uint64 failed_mpdus;
} wave_sta_ofdma_su_mu_stats64_t;

typedef struct wave_sta_ofdma_mpdus_stats64 {
    wave_sta_ofdma_su_mu_stats64_t su_mpdus[MAX_NUM_OF_BW];
    wave_sta_ofdma_su_mu_stats64_t mu_mpdus[MAX_NUM_OF_RU_SIZES];
} wave_sta_ofdma_mpdus_stats64_t;

typedef struct wave_sta_ofdma_mimo_rate_stats {
    uint32 nss_ppdus[MAX_REDUCED_NSS];
    uint32 mcs_ppdus[MAX_REDUCED_MCS];
} wave_sta_ofdma_mimo_rate_stats_t;

typedef struct wave_sta_su_stats {
    wave_sta_ofdma_mimo_rate_stats_t su_rate_stats;
} wave_sta_su_stats_t;

typedef struct wave_sta_mu_stats {
    wave_sta_ofdma_mimo_rate_stats_t mu_rate_stats;
    uint32 succeeded_sounding;
    uint32 attemptedsounding_per_mcs[MAX_REDUCED_MCS];
} wave_sta_mu_stats_t;

typedef struct wave_sta_ofdma_dl_mimo_stats {
    wave_sta_su_stats_t su_mimo_stats;
    wave_sta_mu_stats_t mu_mimo_stats;
} wave_sta_ofdma_mimo_stats_t;

typedef struct wave_sta_ofdma_mimo_rate_stats64 {
    uint64 nss_ppdus[MAX_REDUCED_NSS];
    uint64 mcs_ppdus[MAX_REDUCED_MCS];
} wave_sta_ofdma_mimo_rate_stats64_t;

typedef struct wave_sta_su_stats64 {
    wave_sta_ofdma_mimo_rate_stats64_t su_rate_stats64;
} wave_sta_su_stats64_t;

typedef struct wave_sta_mu_stats64 {
    wave_sta_ofdma_mimo_rate_stats64_t mu_rate_stats64;
    uint64 succeeded_sounding;
    uint64 attemptedsounding_per_mcs[MAX_REDUCED_MCS];
} wave_sta_mu_stats64_t;

typedef struct wave_sta_ofdma_dl_mimo_stats64 {
    wave_sta_su_stats64_t su_mimo_stats64;
    wave_sta_mu_stats64_t mu_mimo_stats64;
} wave_sta_ofdma_mimo_stats64_t;

typedef struct mtlk_mhi_sta_stats{
    uint32 mpduFirstRetransmission;
    uint32 mpduTransmitted;
    uint32 mpduByteTransmitted;
    uint32 rdCount;
    uint32 rxOutStaNumOfBytes;
    uint32 swUpdateDrop;
    uint32 txRetryCount;
    uint32 exhaustedCount;
    uint32 successCount;
    uint32 successByteCount;
    uint32 rddelayed;
    uint32 rdDuplicateDrop;
    uint32 missingSn;
    uint32 mpduInAmpdu;
    uint32 ampdu;
    uint32 rxRetryCount;
    uint32 transmittedAmpdu;
    uint32 tx_errors;
    uint32 ewmaTimeNsec;
    uint32 numMsdus;
    uint32 per;
    uint32 prevSumTimeNsec;
    uint32 maxRssi;
    uint32 minRssi;
    uint32 packetRetransCount;
    uint32 oneOrMoreRetryCount;
    uint32 dropCntReasonClassifier;
    uint32 dropCntReasonDisconnect;
    uint32 dropCntReasonATF;
    uint32 dropCntReasonTSFlush;
    uint32 dropCntReasonReKey;
    uint32 dropCntReasonSetKey;
    uint32 dropCntReasonDiscard;
    uint32 dropCntReasonDsabled;
    uint32 dropCntReasonAggError;
    twt_params_t twtStaParams;
    ul_mu_stats_t uplinkMuStats;
    dl_mu_stats_t downlinkMuStats;
    uint32 qosTxSta[NUM_OF_TIDS];
    uint32 channelTransmitTime;
    uint32 retryCount;
    uint32 clonedCount;
    uint32 perclientmpduInAmpdu;
    uint32 agerPdNoTransmitCountSta;
    uint32 ulRuSubChannels;
    uint32 ulRuType;
    uint32 dlRuSubChannels;
    uint32 dlRuType;
    uint32 multicastPacketSent;
    uint32 multicastByteSent;
    uint32 broadcastRecvPacketCount;
    uint32 broadcastRecvBytesCount;
    uint32 prevUnicastPacketSent;
    uint32 prevUnicastByteSent;
    uint32 prevMulticastPacketSent;
    uint32 prevMulticastByteSent;
    uint32 staRemovedNbiqBelowThresholdCounter;
    uint32 staRemovedFavoringSuCounter;
    uint32 staNotAddedNbiqBelowThresholdCounter;
    wave_sta_ofdma_ppdus_stats_t ul_ppdus;
    wave_sta_ofdma_ppdus_stats_t dl_ppdus;
    wave_sta_ofdma_mpdus_stats_t dl_mpdus;
    wave_sta_ofdma_mimo_stats_t dl_mimo;
} mtlk_mhi_stats_sta_cntr_t;

typedef struct mtlk_mhi_sta_stats64{
    uint64 mpduFirstRetransmission;
    uint64 mpduTransmitted;
    uint64 mpduByteTransmitted;
    uint64 rdCount;
    uint64 rxOutStaNumOfBytes;
    uint64 swUpdateDrop;
    uint64 txRetryCount;
    uint64 exhaustedCount;
    uint64 successCount;
    uint64 successByteCount;
    uint64 rddelayed;
    uint64 rdDuplicateDrop;
    uint64 missingSn;
    uint64 mpduInAmpdu;
    uint64 ampdu;
    uint64 rxRetryCount;
    uint64 transmittedAmpdu;
    uint64 tx_errors;
    uint64 sumTimeNsec;
    uint64 numMsdus;
    uint64 packetRetransCount;
    uint64 oneOrMoreRetryCount;
    uint64 dropCntReasonClassifier;
    uint64 dropCntReasonDisconnect;
    uint64 dropCntReasonATF;
    uint64 dropCntReasonTSFlush;
    uint64 dropCntReasonReKey;
    uint64 dropCntReasonSetKey;
    uint64 dropCntReasonDiscard;
    uint64 dropCntReasonDsabled;
    uint64 dropCntReasonAggError;
    uint64 channelTransmitTime;
    uint64 retryCount;
    uint64 clonedCount;
    uint64 perclientmpduInAmpdu;
    uint64 agerPdNoTransmitCountSta;
    uint64 staRemovedNbiqBelowThresholdCounter;
    uint64 staRemovedFavoringSuCounter;
    uint64 staNotAddedNbiqBelowThresholdCounter;
    wave_sta_ofdma_ppdus_stats64_t ul_ppdus;
    wave_sta_ofdma_ppdus_stats64_t dl_ppdus;
    wave_sta_ofdma_mpdus_stats64_t dl_mpdus;
    wave_sta_ofdma_mimo_stats64_t dl_mimo;
} mtlk_mhi_stats64_sta_cntr_t;

typedef struct wave_csi_sta_stats64{
  uint64 csiSendQosNullCount;
  uint64 csiRecvFrameCount;
  uint64 csiSendNlCsiData;
  uint64 csiReqInfoCount;
}wave_csi_sta_stats64_cntr_t;

typedef struct airtime_stats
{
  uint32           tx_time;
  uint32           rx_time;
  uint32           airtime_efficiency;
  unsigned long    update_time;
  uint8            airtime_usage;
} airtime_stats_t;

typedef struct _sta_stats
{
  mtlk_mhi_stats_sta_tx_rate_t  mhi_tx_stats; /* Table of TX statistics */

  airtime_stats_t               airtime_stats;

                                /* TX: Downlink - from AP to STA */
  mtlk_bitrate_params_t tx_data_rate_params; /* always rate parameters */

                                /* RX: Uplink - from STA to AP */
  mtlk_bitrate_info_t   rx_data_rate_info; /* phy_rate OR rate_info*/
  mtlk_bitrate_info_t   rx_mgmt_rate_info; /* phy_rate OR rate_info */
  uint32                last_phy_rate_synched_to_psdu_rate;
  uint32                max_phy_rate_synched_to_psdu_rate;
  uint32                last_tx_data_rate;
  uint32                max_tx_data_rate;
  uint32                rx_psdu_data_rate_info; /* psdu_rate */
  int8                  max_rssi;
  int8                  data_rssi[WAVE_STAT_MAX_ANTENNAS];	/* RSSI per antenna */
  int8                  mgmt_max_rssi;
  int8                  mgmt_rssi[WAVE_STAT_MAX_ANTENNAS];
  int8                  snr[WAVE_STAT_MAX_ANTENNAS];
  int8                  avg_rssi;
  int8                  noise[WAVE_STAT_MAX_ANTENNAS];
  uint8                 gain[WAVE_STAT_MAX_ANTENNAS];	/* RF_GAIN. This field is not valid in Wav700 */
#ifdef MTLK_WAVE_700
  int8                  rcpi[WAVE_STAT_MAX_ANTENNAS]; /* RCPI per antenna */
#endif
  uint32                perClientRxtimeUsage;
  uint32                lastTsf;
  uint32                irad;
  mtlk_mhi_stats_lamuofdma_cntr_t ofdma_count_stats;
  wave_mhi_la_ofdma_sta_pdu_stats_t ul_ofdma_stats;
  wave_mhi_la_ofdma_sta_pdu_stats_t dl_ofdma_stats;
} __MTLK_IDATA sta_stats;

typedef struct _sta_info
{
  uint16              sid;
  uint32              flags;
  uint16              bitrate_1ss; /* per one stream, internal units */
  uint16              bitrate_max; /* max supported, internal units */
  uint8               max_antennas;
  uint8               rates[WV_SUPP_RATES_MAX];
  uint8               bss_coex_20_40;
  uint8               ampdu_param;
  uint8               vendor;
  uint8               cipher;   /* TKIP or CCMP */
  uint8               supported_rates_len;
  uint32              listen_interval;
  sta_stats           stats;
  mtlk_pckt_filter_e  filter;      /* flag to filter packets */
  mtlk_peer_analyzer_t sta_analyzer;
  wave_rssi_avg_info_t rssi_avg;
#ifdef MTLK_WAVE_700
  wave_rssi_avg_info_t rcpi_avg;
#endif
  int32               rssi_dbm;
  uint32              tx_bf_cap_info;
  uint8               opmode_notif;
  uint8               sta_net_modes;
  uint8               WDS_client_type;
  uint8               max_supp_bw;
  uint8               max_supp_mcs;
  uint8               sgi_enable;
  uint8               MainVapId;
  struct sta_addr_and_stadb * params;
} __MTLK_IDATA sta_info;

#ifdef MTLK_WAVE_700
#ifdef BEST_EFFORT_TID_SPREADING
#define MAX_NUM_OF_LINKS 3 /* Number of supported links for MLO */
typedef struct _wave_ml_str_sta_tid_spreading_info {
  uint16 tid_flip;
  uint16 curr_load_high_rate_tid;
  uint16 curr_load_low_rate_tid;
  uint8  high_eff_rate_tid_percent;
  uint8  low_eff_rate_tid_percent;
  uint8  high_rate_tid;
  uint8  low_rate_tid;
  uint8  tid_to_link_bitmap[MAX_NUM_OF_LINKS];
  uint16 tid_remap_flip;
} __MTLK_IDATA wave_ml_str_sta_tid_spreading_info_t;
#endif /* BEST_EFFORT_TID_SPREADING */

typedef struct _wave_ml_remove_sta_mld
{
  mtlk_osal_spinlock_t lock;
  mtlk_atomic_t        remove_sta_cnt;
} wave_ml_rem_sta_mld_t;

typedef struct _wave_ml_sta_info
{
  wave_ml_rem_sta_mld_t *remove_sta_mld;
  sta_entry             *sibling_sta;           /* affiliated sta */
  BOOL                  rem_sta_mld_done;       /* flag to denote REMOVE_STA_MLD is intimated to FW */
  BOOL                  rem_sta_mld_inprogress; /* flag to indicate REMOVE_STA_MLD is ongoing */
  uint8                 ml_supp_mode;           /* MLD STA supported mode - refer multilink_modes_e */
  mtlk_osal_event_t     ml_discnt_event;
  wave_vap_id_t         assoc_vap_id_fw;
#ifdef BEST_EFFORT_TID_SPREADING
  uint32                prev_eff_phy_rate;
  wave_ml_str_sta_tid_spreading_info_t *sta_tid_spread_info;
#endif /* BEST_EFFORT_TID_SPREADING */
} __MTLK_IDATA wave_ml_sta_info_t;

typedef struct _wave_scs_stadb
{
   mtlk_dlist_t  scs_list;
   mtlk_osal_spinlock_t scs_list_lock;
} __MTLK_IDATA wave_scs_stadb;

typedef struct _wave_class_type10
{
  uint8 proto_instance;
  uint8 proto_num;
  uint8 filter_len;
  uint8 filter_val[MAX_TCLAS10_FILTER_LEN];
  uint8 filter_mask[MAX_TCLAS10_FILTER_LEN];
} __MTLK_IDATA wave_class_type10_t;

typedef struct _wave_scs_tclas_info
{
  uint8 tclass_up;
  uint8 tclass_len;
  uint8 tclass_type;
  uint8 tclass_mask;
} __MTLK_IDATA wave_scs_tclas_info_t;

typedef struct _wave_ipv4_params
{
  uint32  src_ip;
  uint32  dst_ip;
  uint16 src_port;
  uint16 dst_port;
  uint8  dscp;
  uint8  protocol;
} __MTLK_IDATA wave_ipv4_params_t;

typedef struct _wave_ipv6_params
{
  uint8  src_ip[16];
  uint8  dst_ip[16];
  uint16 src_port;
  uint16 dst_port;
  uint8  dscp;
  uint8  next_header;
  uint8 flow_label[3];
} __MTLK_IDATA wave_ipv6_params_t;

typedef struct _wave_ip_classifier
{
  uint8  ip_version;
  union {
    wave_ipv4_params_t ipv4;
    wave_ipv6_params_t ipv6;
  } u;
} __MTLK_IDATA wave_ip_classifier_t;

typedef struct _wave_scs_list_info
{
  uint8 scsid;
  uint8 aid;
  uint8 req_up;
  wave_scs_tclas_info_t tclas_info;
  wave_ip_classifier_t ip_tuple;
  wave_class_type10_t clas_type10;
  mtlk_dlist_entry_t lentry;
  //mtlk_osal_spinlock_t scs_list_lock;
} __MTLK_IDATA wave_scs_list_info_t;

typedef struct _wave_mscs_list_info
{
 uint8  user_priority;
 u64 timestamp;
 wave_ip_classifier_t ip_tuple;
 mtlk_dlist_entry_t lentry;
} __MTLK_IDATA wave_mscs_list_info_t;

typedef struct _wave_mscs_stadb
{
  BOOL   mscs_active;
  uint8  user_priority_bitmap;
  uint8  user_priority_limit;
  uint8  class_mask;
  uint16 aid;
  uint64 stream_timeout_ms;
  mtlk_dlist_t  mscs_list;
  mtlk_osal_spinlock_t mscs_list_lock;
} __MTLK_IDATA wave_mscs_stadb;
#endif /* MTLK_WAVE_700 */

struct _sta_db; /* TODO: replace it with Param DB/callbacks with opaque pointers */

static __INLINE uint32
_mtlk_hash_sid_hashval (const u16 *key, uint32 nof_buckets)
{
  return (*key & (nof_buckets - 1));
}

static __INLINE int
_mtlk_hash_sid_keycmp (const u16 *key1, const u16 *key2)
{
  return (*key1 != *key2);
}

MTLK_HASH_DECLARE_ENTRY_T(sid, u16);

MTLK_HASH_DECLARE_INLINE(sid, u16);

MTLK_HASH_DEFINE_INLINE(sid, u16,
                        _mtlk_hash_sid_hashval,
                        _mtlk_hash_sid_keycmp);

#ifdef BEST_EFFORT_TID_SPREADING
static __INLINE uint32
_mtlk_hash_skb_hash_hashval(const __u32 *key, uint32 nof_buckets)
{
  return (*key & (nof_buckets - 1));
}

static __INLINE int
__mtlk_hash_skb_hash_keycmp(const __u32 *key1, const __u32 *key2)
{
  return *key1 != *key2;
}

MTLK_HASH_DECLARE_ENTRY_T(skb_hash, __u32);
MTLK_HASH_DECLARE_INLINE(skb_hash, __u32);
MTLK_HASH_DEFINE_INLINE(skb_hash, __u32,
                        _mtlk_hash_skb_hash_hashval,
                        __mtlk_hash_skb_hash_keycmp);

typedef struct __tid_hash_map_info {
  MTLK_HASH_ENTRY_T(skb_hash) hentry;
  mtlk_osal_msec_t timestamp;
  __u16            tid;
  __u32            skb_hash;
  BOOL             higher_len_rcvd;
} tid_hash_map_info;
#endif

struct _sta_entry {
  MTLK_HASH_ENTRY_T(ieee_addr) hentry;
  MTLK_HASH_ENTRY_T(ieee_addr) hMLentry;
  MTLK_HASH_ENTRY_T(sid)       hentry_sid;
  mtlk_dlist_entry_t           vlan_pending_lentry;
#ifdef BEST_EFFORT_TID_SPREADING
  mtlk_hash_t                  skb_hash;
#endif
#ifdef MTLK_PER_RATE_STAT
  mtlk_hash_t                  per_rate_stat_hash_tx;
  mtlk_hash_t                  per_rate_stat_hash_rx;
  mtlk_hash_t                  per_rate_stat_hash_packet_error_rate;
  BOOL                         rate_stat_possible;
#endif
  peer_tx_rx_stall_warning     tx_rx_stall_warning;
  mtlk_atomic_t                ref_cnt;
  mtlk_osal_event_t            ref_cnt_event;
  sta_info                     info;
  BOOL                         is_added_to_driver;
  mtlk_osal_spinlock_t         lock;
  mtlk_osal_timestamp_t        activity_timestamp;
  mtlk_osal_timestamp_t        connection_timestamp;
  mtlk_osal_timer_t           *keepalive_timer;      // Timer for polling station
  uint8                        peer_ap;              // Is peer AP?
  /* Description of AP */
  mtlk_vap_handle_t            vap_handle;
  struct _sta_db              *paramdb;
  mtlk_mhi_stats_sta_cntr_t    sta_stats_cntrs; /* Table of statistic counters */
  mtlk_mhi_stats64_sta_cntr_t  sta_stats64_cntrs; /* Table of statistic counters */
  mtlk_mhi_stats_sta_cntr_t    sta_stats_cntrs_snapshot; /* Table holding statistic counters's snapshot*/
  mtlk_mhi_stats64_sta_cntr_t  sta_stats64_cntrs_snapshot; /* Table holding statistic counters's snapshot */
  mtlk_wss_t                  *wss;
  mtlk_wss_cntr_handle_t      *wss_hcntrs[MTLK_STAI_CNT_LAST];
  /* data path vap id for this station, will be different than vap_handle's vap id
   * if this station is a backhaul sta that connected using a secondary vap id */
  uint8                        secondary_vap_id;
  /* The name of the vlan/backhaul interface with the secondary df that the backhaul STA waits */
  char                         vlan_ifname[IFNAMSIZ + 1];
  uint8                        csi_enable;
  uint8                        csi_auto_enable;
  uint8                        csi_auto_rate;
  wifi_csi_driver_data_hw_t    csi_stats;
  wave_csi_sta_stats64_cntr_t  csi_sta_cntrs; /* CSI statistic counters */
#ifdef MTLK_WAVE_700
  wave_ml_sta_info_t           ml_sta_info;
  wave_scs_stadb               scs_db;
  wave_mscs_stadb              mscs_db;
#endif
  BOOL                         is_traffic_stopped;
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_START_STATUS;
  MTLK_DECLARE_START_LOOP(ROD_QUEs);
} __MTLK_IDATA;


static __INLINE int
mtlk_stadb_apply_rssi_offset(int phy_rssi, unsigned rssi_offs)
{
    int rssi = phy_rssi - rssi_offs;
    return MAX(MIN_RSSI, rssi);
}

static __INLINE
int8 mtlk_calculate_snr (int8 rssi, int8 noise)
{
  int snr;

  /* Unavailable noise */
  if (!noise)
    return 0;

  snr = rssi - noise;
  if (snr < MIN_INT8)
    return MIN_INT8;
  else if (snr > MAX_INT8)
    return MAX_INT8;

  return snr;
}

static __INLINE mtlk_mhi_stats64_sta_cntr_t *
mtlk_sta_get_mhi_stat64_array (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return (mtlk_mhi_stats64_sta_cntr_t *)&sta->sta_stats64_cntrs;
}

static __INLINE mtlk_mhi_stats_sta_cntr_t *
mtlk_sta_get_mhi_stat_array (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return (mtlk_mhi_stats_sta_cntr_t *)&sta->sta_stats_cntrs;
}

static __INLINE mtlk_mhi_stats64_sta_cntr_t *
mtlk_sta_get_mhi_stat64_snapshot_array (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return (mtlk_mhi_stats64_sta_cntr_t *)&sta->sta_stats64_cntrs_snapshot;
}

static __INLINE mtlk_mhi_stats_sta_cntr_t *
mtlk_sta_get_mhi_stat_snapshot_array (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return (mtlk_mhi_stats_sta_cntr_t *)&sta->sta_stats_cntrs_snapshot;
}


static __INLINE uint64
mtlk_sta_get_stat_cntr_tx_frames (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats64_cntrs.mpduTransmitted;
}

static __INLINE uint64
mtlk_sta_get_stat_cntr_rx_frames (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats64_cntrs.rdCount;
}

static __INLINE uint64
mtlk_sta_get_stat_cntr_tx_bytes (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats64_cntrs.mpduByteTransmitted;
}

static __INLINE uint64
mtlk_sta_get_stat_cntr_rx_bytes (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats64_cntrs.rxOutStaNumOfBytes;
}

static __INLINE uint32
mtlk_sta_get_stat_cntr_pct_retry (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats_cntrs.txRetryCount;
}

static __INLINE uint32
mtlk_sta_get_stat_cntr_pct_retry_exhausted (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats_cntrs.exhaustedCount;
}
static __INLINE uint32
mtlk_sta_get_stat_cntr_snapshot_pct_retry (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats_cntrs_snapshot.txRetryCount;
}

static __INLINE uint32
mtlk_sta_get_stat_cntr_snapshot_pct_retry_exhausted (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return sta->sta_stats_cntrs_snapshot.exhaustedCount;
}

/* MHI STA TX statistic */

static __INLINE mtlk_mhi_stats_sta_tx_rate_t *
mtlk_sta_get_mhi_tx_stats (const sta_entry *sta)
{
  return &((sta_entry *)sta)->info.stats.mhi_tx_stats; /* discard const qualifief */
}

static __INLINE mtlk_mhi_stats_lamuofdma_cntr_t *
mtlk_sta_get_ofdma_count_stats (const sta_entry *sta)
{
  return &((sta_entry *)sta)->info.stats.ofdma_count_stats;
}

static __INLINE wave_mhi_la_ofdma_sta_pdu_stats_t *
wave_sta_get_ul_ofdma_stats (const sta_entry *sta)
{
  return &((sta_entry *)sta)->info.stats.ul_ofdma_stats;
}

static __INLINE wave_mhi_la_ofdma_sta_pdu_stats_t *
wave_sta_get_dl_ofdma_stats (const sta_entry *sta)
{
  return &((sta_entry *)sta)->info.stats.dl_ofdma_stats;
}

static __INLINE uint8
mtlk_sta_get_mhi_tx_stat_bf_mode (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return ((sta_entry *)sta)->info.stats.mhi_tx_stats.bfModeData;
}

static __INLINE uint8
mtlk_sta_get_mhi_tx_stat_stbc_mode (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return ((sta_entry *)sta)->info.stats.mhi_tx_stats.stbcModeData;
}

static __INLINE uint8
mtlk_sta_get_mhi_tx_stat_power_data (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return ((sta_entry *)sta)->info.stats.mhi_tx_stats.powerData;
}

static __INLINE uint8
mtlk_sta_get_mhi_tx_stat_power_mgmt (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return ((sta_entry *)sta)->info.stats.mhi_tx_stats.powerManagement;
}

static __INLINE uint8
mtlk_sta_get_mhi_tx_stat_phymode_mgmt (const sta_entry  *sta)
{
  MTLK_ASSERT(sta);
  return ((sta_entry *)sta)->info.stats.mhi_tx_stats.ManagementPhyMode;
}

static __INLINE int
mtlk_sta_get_short_term_rssi (const sta_entry *sta, int rssi_index)
{
    return sta->info.stats.data_rssi[rssi_index];
}

static __INLINE int8
mtlk_sta_get_avg_rssi (const sta_entry *sta)
{
   return sta->info.rssi_avg.avg_rssi_dbm;
}

#ifdef MTLK_WAVE_700
static __INLINE int8
mtlk_sta_get_avg_rcpi (const sta_entry *sta)
{
   return sta->info.rcpi_avg.avg_rssi_dbm;
}
#endif

/* with checking ALLOWED option */
#define mtlk_sta_get_cnt(sta, id)         (TRUE == id##_ALLOWED) ? __mtlk_sta_get_cnt(sta, id) : 0

static __INLINE uint32
__mtlk_sta_get_cnt (const sta_entry  *sta,
                  sta_info_cnt_id_e cnt_id)
{
  MTLK_ASSERT(cnt_id >= 0 && cnt_id < MTLK_STAI_CNT_LAST);

  return mtlk_wss_get_stat(sta->wss, cnt_id);
}

#if MTLK_WWSS_WLAN_STAT_ANALYZER_LONG_NOISE_ALLOWED
static __INLINE int8
mtlk_sta_get_long_term_noise (const sta_entry  *sta, unsigned idx)
{
  return wave_peer_analyzer_get_long_noise(&sta->info.sta_analyzer, idx);
}
#endif

#if MTLK_WWSS_WLAN_STAT_ANALYZER_TX_RATE_ALLOWED
static __INLINE uint32
mtlk_sta_get_short_term_tx(const sta_entry  *sta)
{
  return mtlk_peer_analyzer_get_short_term_tx(&sta->info.sta_analyzer);
}

static __INLINE uint32
mtlk_sta_get_long_term_tx(const sta_entry  *sta)
{
  return mtlk_peer_analyzer_get_long_term_tx(&sta->info.sta_analyzer);
}
#endif /* MTLK_WWSS_WLAN_STAT_ANALYZER_TX_RATE_ALLOWED */

#if MTLK_WWSS_WLAN_STAT_ANALYZER_RX_RATE_ALLOWED
static __INLINE uint32
mtlk_sta_get_short_term_rx(const sta_entry  *sta)
{
  return mtlk_peer_analyzer_get_short_term_rx(&sta->info.sta_analyzer);
}

static __INLINE uint32
mtlk_sta_get_long_term_rx(const sta_entry  *sta)
{
  return mtlk_peer_analyzer_get_long_term_rx(&sta->info.sta_analyzer);
}
#endif /* MTLK_WWSS_WLAN_STAT_ANALYZER_RX_RATE_ALLOWED */

static __INLINE uint16
mtlk_sta_get_bitrate_1ss (const sta_entry *sta)
{
  return sta->info.bitrate_1ss;
}

static __INLINE uint16
mtlk_sta_get_bitrate_max (const sta_entry *sta)
{
  return sta->info.bitrate_max;
}

static __INLINE uint8
mtlk_sta_get_max_antennas (const sta_entry *sta)
{
  return sta->info.max_antennas;
}

static __INLINE uint8
mtlk_sta_get_cipher (const sta_entry *sta)
{
  return sta->info.cipher;
}

static __INLINE mtlk_pckt_filter_e
mtlk_sta_get_packets_filter (const sta_entry *sta)
{
  return sta->info.filter;
}

static __INLINE BOOL
mtlk_sta_is_dot11n (const sta_entry *sta)
{
  return MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11n);
}

static __INLINE BOOL
mtlk_sta_is_mfp (const sta_entry *sta)
{
  return MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_MFP);
}

static __INLINE BOOL
mtlk_sta_is_wds (const sta_entry *sta)
{
  return MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_WDS);
}

static __INLINE BOOL
mtlk_sta_info_is_4addr (const sta_info *info)
{
  return FOUR_ADDRESSES_STATION == info->WDS_client_type ||
         WPA_WDS == info->WDS_client_type;
}


static __INLINE BOOL
mtlk_sta_is_4addr (const sta_entry *sta)
{
  return mtlk_sta_info_is_4addr(&sta->info);
}


/* static __INLINE uint8 */
/* mtlk_sta_get_net_mode (const sta_entry *sta) */
/* { */
/*   return sta->info.net_mode; */
/* } */

/* Get time passed since station connected in seconds */
static __INLINE uint32
mtlk_sta_get_age (const sta_entry *sta)
{
  mtlk_osal_timestamp_diff_t diff = mtlk_osal_timestamp_diff(mtlk_osal_timestamp(), sta->connection_timestamp);
  return mtlk_osal_timestamp_to_ms(diff) / 1000; /* Converted from milliseconds to seconds */
}

/* Get inactive passed since last activity in milliseconds */
static __INLINE uint32
mtlk_sta_get_inactive_time (const sta_entry *sta)
{
  mtlk_osal_timestamp_diff_t diff = mtlk_osal_timestamp_diff(mtlk_osal_timestamp(), sta->activity_timestamp);
  return mtlk_osal_timestamp_to_ms(diff);
}

static __INLINE void
mtlk_sta_get_rssi (const sta_entry *sta, int8 *rssi, uint32 size)
{
  MTLK_ASSERT(rssi != NULL);
  MTLK_ASSERT(size == ARRAY_SIZE(sta->info.stats.data_rssi));

  wave_memcpy(rssi, size, sta->info.stats.data_rssi, ARRAY_SIZE(sta->info.stats.data_rssi));
}

static __INLINE int8
mtlk_sta_get_max_rssi (const sta_entry *sta)
{
  return sta->info.stats.max_rssi;
}

static __INLINE int8
mtlk_sta_get_mgmt_max_rssi (const sta_entry *sta)
{
  return sta->info.stats.mgmt_max_rssi;
}

static __INLINE struct ieee80211_sta *
wv_sta_entry_get_mac80211_sta (const sta_entry * sta)
{
  return container_of((void *)sta, struct ieee80211_sta, drv_priv);
}

static __INLINE const IEEE_ADDR *
mtlk_sta_get_addr (const sta_entry *sta)
{
  struct ieee80211_sta * mac80211_sta;
  if (sta == NULL) return NULL;
  mac80211_sta = wv_sta_entry_get_mac80211_sta(sta);
  return (const IEEE_ADDR *)(mac80211_sta->addr);
}

static __INLINE uint16
mtlk_sta_get_sid (const sta_entry *sta)
{
  return sta->info.sid;
}

static __INLINE uint16
mtlk_sta_get_aid (const sta_entry *sta)
{
  return wv_sta_entry_get_mac80211_sta(sta)->aid;
}

static __INLINE uint8
mtlk_sta_get_network_modes (const sta_entry *sta)
{
  return sta->info.sta_net_modes;
}

#ifdef MTLK_WAVE_700
static __INLINE BOOL
mtlk_sta_is_ml_disconnected(sta_entry *sta)
{
  return sta->ml_sta_info.rem_sta_mld_done;
}
/********************************************************/
void  __MTLK_IFUNC mtlk_sta_wait_ml_discnt(sta_entry *sta);
void  __MTLK_IFUNC mtlk_sta_ml_discnt_finish(sta_entry *sta);
#endif

void __MTLK_IFUNC
mtlk_sta_get_peer_stats(const sta_entry* sta, mtlk_wssa_drv_peer_stats_t* stats);

void __MTLK_IFUNC
mtlk_sta_get_peer_capabilities(const sta_entry* sta, mtlk_wssa_drv_peer_capabilities_t* capabilities);

#ifdef CONFIG_WAVE_DEBUG
void __MTLK_IFUNC
mtlk_sta_get_host_if_qos_stats (const sta_entry* sta, peerHostIfQos_t *peer_host_if_qos);
void __MTLK_IFUNC
mtlk_sta_get_host_if_stats (const sta_entry* sta, peerHostIf_t *peer_host_if);
void __MTLK_IFUNC
mtlk_sta_get_peer_rx_stats (const sta_entry* sta, peerRxStats_t *peer_rx_stats);
void __MTLK_IFUNC
mtlk_sta_get_peer_ul_bsrc_tid_stats (const sta_entry* sta, peerUlBsrcTidStats_t *peer_ul_bsrc_tid);
void __MTLK_IFUNC
mtlk_sta_get_peer_baa_stats (const sta_entry* sta, peerBaaStats_t *peer_baa_stats);
void __MTLK_IFUNC
mtlk_sta_get_link_adaption_stats(const sta_entry* sta, linkAdaptionStats_t *link_stats);
void __MTLK_IFUNC
mtlk_sta_get_plan_manager_stats (const sta_entry* sta, planManagerStats_t *plan_manager_stats);
void __MTLK_IFUNC
mtlk_sta_get_twt_stats (const sta_entry* sta, twtStats_t *twt_stats);
void __MTLK_IFUNC
mtlk_sta_get_per_client_stats (const sta_entry* sta, perClientStats_t *per_client_stats);
void __MTLK_IFUNC
mtlk_sta_get_phy_rx_status (const sta_entry* sta, peerPhyRxStatus_t *phy_rx_status);
void __MTLK_IFUNC
mtlk_sta_get_peer_info (const sta_entry* sta, peerInfo_t *peer_info);
#endif /* CONFIG_WAVE_DEBUG */

/********************************************************
 * WARNING: __mtlk_sta_on_unref_private is private API! *
 *          No one is allowed to use it except the      *
 *          mtlk_sta_decref.                            *
 ********************************************************/
void __MTLK_IFUNC
__mtlk_sta_on_unref_private(sta_entry *sta);
/********************************************************/

void __MTLK_IFUNC
mtlk_sta_wait_delete (struct ieee80211_sta *mac80211_sta);

#ifndef STA_REF_DBG
static __INLINE int
mtlk_sta_incref (sta_entry  *sta)
{
  return mtlk_osal_atomic_inc(&sta->ref_cnt);
}

static __INLINE void
mtlk_sta_decref (sta_entry *sta)
{
  uint32 ref_cnt = mtlk_osal_atomic_dec(&sta->ref_cnt);

  if (ref_cnt == 0) {
    /* should only happen once per sta, see mtlk_sta_wait_delete*/
    __mtlk_sta_on_unref_private(sta);
  }
}
#else
#define mtlk_sta_incref(sta) __mtlk_sta_incref_dbg(__FUNCTION__, __LINE__, (sta))

static __INLINE void
__mtlk_sta_incref_dbg (const char *f, int l, sta_entry *sta)
{
  uint32 ref_cnt = mtlk_osal_atomic_inc(&sta->ref_cnt);

  ILOG0_YSDD("+++ %Y in %s on %d . ref = %d",
            mtlk_sta_get_addr(sta),
            f,
            l,
            ref_cnt);
}

#define mtlk_sta_decref(sta) __mtlk_sta_decref_dbg(__FUNCTION__, __LINE__, (sta))

static __INLINE void
__mtlk_sta_decref_dbg (const char *f, int l, sta_entry *sta)
{
  uint32 ref_cnt = mtlk_osal_atomic_dec(&sta->ref_cnt);

  ILOG0_YSDD("-- %Y in %s on %d . ref = %d",
            mtlk_sta_get_addr(sta),
            f,
            l,
            ref_cnt);

  if (ref_cnt == 0) {
    __mtlk_sta_on_unref_private(sta);
  }
}
#endif /* STA_REF_DBG */

void __MTLK_IFUNC
mtlk_sta_on_packet_sent(sta_entry *sta, uint32 nbuf_len, uint32 nbuf_flags) __MTLK_INT_HANDLER_SECTION;

void __MTLK_IFUNC
mtlk_sta_on_man_frame_arrived(sta_entry *sta, const int8 rssi[]);

/* with DEBUG statistic */
#if MTLK_MTIDL_PEER_STAT_FULL
typedef enum
{
  MTLK_TX_DISCARDED_DRV_NO_RESOURCES  = MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DRV_NO_RESOURCES,
  MTLK_TX_DISCARDED_SQ_OVERFLOW       = MTLK_STAI_CNT_TX_PACKETS_DISCARDED_SQ_OVERFLOW,
  MTLK_TX_DISCARDED_DROP_ALL_FILTER   = MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DROP_ALL_FILTER,
  MTLK_TX_DISCARDED_TX_QUEUE_OVERFLOW = MTLK_STAI_CNT_TX_PACKETS_DISCARDED_TX_QUEUE_OVERFLOW,
} mtlk_tx_drop_reasons_e;

#define MTLK_TX_DISCARDED_DRV_NO_RESOURCES_ALLOWED  MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DRV_NO_RESOURCES_ALLOWED
#define MTLK_TX_DISCARDED_SQ_OVERFLOW_ALLOWED       MTLK_STAI_CNT_TX_PACKETS_DISCARDED_SQ_OVERFLOW_ALLOWED
#define MTLK_TX_DISCARDED_DROP_ALL_FILTER_ALLOWED   MTLK_STAI_CNT_TX_PACKETS_DISCARDED_DROP_ALL_FILTER_ALLOWED
#define MTLK_TX_DISCARDED_TX_QUEUE_OVERFLOW_ALLOWED MTLK_STAI_CNT_TX_PACKETS_DISCARDED_TX_QUEUE_OVERFLOW_ALLOWED

void __MTLK_IFUNC
mtlk_sta_on_packet_dropped(sta_entry *sta, mtlk_tx_drop_reasons_e reason);

#else /* MTLK_MTIDL_PEER_STAT_FULL */

#define mtlk_sta_on_packet_dropped(sta, id)     { /* empty */ }

#endif /* MTLK_MTIDL_PEER_STAT_FULL */

void __MTLK_IFUNC
mtlk_sta_on_packet_indicated(sta_entry *sta, mtlk_nbuf_t *nbuf, uint32 nbuf_flags) __MTLK_INT_HANDLER_SECTION;

void __MTLK_IFUNC
mtlk_sta_on_rx_packet_802_1x(sta_entry *sta);

void __MTLK_IFUNC
mtlk_sta_on_tx_packet_802_1x(sta_entry *sta);

void __MTLK_IFUNC
mtlk_sta_on_tx_packet_discarded_802_1x(sta_entry *sta);

void __MTLK_IFUNC
mtlk_sta_on_rx_packet_forwarded(sta_entry *sta, mtlk_nbuf_t *nbuf) __MTLK_INT_HANDLER_SECTION;

void __MTLK_IFUNC
mtlk_sta_set_pm_enabled(sta_entry *sta, BOOL enabled);

void __MTLK_IFUNC
mtlk_sta_update_phy_info (sta_entry *sta, mtlk_hw_t *hw, stationPhyRxStatusDb_t *sta_status);

void __MTLK_IFUNC
mtlk_sta_update_rx_rate_rssi_on_man_frame (sta_entry *sta, const mtlk_phy_info_t *phy_info);

#ifdef MTLK_WAVE_700
void __MTLK_IFUNC
wave_update_ml_sta_info (sta_entry *sta, wave_ml_sta_info_t *ml_sta_info, uint8 main_vap_id, wave_vap_id_t vap_id_fw);

void __MTLK_IFUNC
wave_cleanup_ml_sta_info (sta_entry *sta);
#endif

static __INLINE void
mtlk_sta_set_bitrate_1ss (sta_entry *sta, uint16 value)
{
  sta->info.bitrate_1ss = value;
}

static __INLINE void
mtlk_sta_set_bitrate_max (sta_entry *sta, uint16 value)
{
  sta->info.bitrate_max = value;
}

static __INLINE void
mtlk_sta_set_max_antennas (sta_entry *sta, uint8 value)
{
  sta->info.max_antennas = value;
}

static __INLINE void
mtlk_sta_set_cipher (sta_entry *sta,
                     uint8      cipher)
{
  sta->info.cipher = cipher;
}

static __INLINE void
mtlk_sta_set_max_mcs (sta_entry *sta,
                      uint8 max_mcs)
{
  sta->info.max_supp_mcs = max_mcs;
}

static __INLINE void
mtlk_sta_set_max_bw (sta_entry *sta,
                     uint8 max_bw)
{
  sta->info.max_supp_bw = max_bw;
}

static __INLINE void
mtlk_sta_set_short_gi (sta_entry *sta,
                       uint8 short_gi)
{
  sta->info.sgi_enable = short_gi;
}

static __INLINE uint8
mtlk_sta_get_short_gi (sta_entry *sta)
{
  return sta->info.sgi_enable;
}

static __INLINE uint8
mtlk_sta_get_max_bw (sta_entry *sta)
{
  return sta->info.max_supp_bw;
}

static __INLINE uint8
mtlk_sta_get_max_mcs (sta_entry *sta)
{
  return sta->info.max_supp_mcs;
}

void __MTLK_IFUNC
mtlk_sta_set_packets_filter(sta_entry         *sta,
                            mtlk_pckt_filter_e filter_type);

struct nic;

typedef struct
{
  mtlk_handle_t   usr_data;
  void            (__MTLK_IFUNC *on_sta_keepalive)(mtlk_handle_t usr_data, IEEE_ADDR addr);
} __MTLK_IDATA sta_db_wrap_api_t;

typedef struct _sta_db_cfg_t
{
  sta_db_wrap_api_t api;
  uint32            max_nof_stas;
  mtlk_wss_t       *parent_wss;
} __MTLK_IDATA sta_db_cfg_t;

typedef struct _sta_db
{
  mtlk_hash_t          hash;
  mtlk_hash_t          mlhash;
  mtlk_hash_t          sid_hash;
  mtlk_hash_t          whm_discnct_hash;
  mtlk_osal_spinlock_t lock;
  mtlk_osal_spinlock_t whm_discnct_lock;
  mtlk_osal_timer_t    whm_discnct_timer;
  mtlk_osal_timer_t    whm_tx_rx_stall_timer;
#ifdef BEST_EFFORT_TID_SPREADING
  mtlk_osal_timer_t    skb_hash_cleanup_timer;
  mtlk_osal_timer_t    str_sta_effective_rates_update_timer;
#endif
  uint32               hash_cnt;
  uint32               mlhash_cnt;
  uint32               keepalive_interval;
  sta_db_cfg_t         cfg;
  mtlk_vap_handle_t    vap_handle;
  uint32               flags;
  mtlk_atomic_t        four_addr_sta_cnt;
  mtlk_wss_t          *wss;
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_START_STATUS;
} __MTLK_IDATA sta_db;

typedef struct _global_stadb_t
{
  mtlk_atomic_t sta_cnt;
} __MTLK_IDATA global_stadb_t;


static __INLINE int
mtlk_stadb_get_sta_cnt (sta_db *sta_db)
{
  return sta_db->hash_cnt;
}

static __INLINE int
mtlk_stadb_get_ml_sta_cnt (sta_db *sta_db)
{
  return sta_db->mlhash_cnt;
}

int __MTLK_IFUNC
mtlk_stadb_init(sta_db *stadb, mtlk_vap_handle_t vap_handle);

void __MTLK_IFUNC
mtlk_stadb_configure(sta_db *stadb, const sta_db_cfg_t *cfg);

int __MTLK_IFUNC
mtlk_stadb_start(sta_db *stadb);

void __MTLK_IFUNC
mtlk_stadb_stop(sta_db *stadb);

void __MTLK_IFUNC
mtlk_stadb_cleanup(sta_db *stadb);

int __MTLK_IFUNC
mtlk_stadb_add_sta(sta_db *stadb, sta_entry * sta, sta_info *info_cfg);

void __MTLK_IFUNC
mtlk_stadb_remove_sta(sta_db *stadb, sta_entry *sta);

static __INLINE sta_entry *
wv_ieee80211_get_sta (struct ieee80211_sta * mac80211_sta)
{
  sta_entry * sta = ((sta_entry *)(mac80211_sta->drv_priv));
  mtlk_sta_incref(sta);
  return sta;
}

static __INLINE sta_entry *
mtlk_stadb_find_sid (sta_db *stadb, u16 sid)
{
  sta_entry                    *sta = NULL;
  MTLK_HASH_ENTRY_T(sid)       *h;

  mtlk_osal_lock_acquire(&stadb->lock);
  h = mtlk_hash_find_sid(&stadb->sid_hash, (u16 *)&sid);
  if (h) {
    sta = MTLK_CONTAINER_OF(h, sta_entry, hentry_sid);
    mtlk_sta_incref(sta); /* Reference by caller */
  }
  mtlk_osal_lock_release(&stadb->lock);

  return sta;
}

sta_entry * __MTLK_IFUNC
mtlk_vap_find_sta (mtlk_vap_handle_t vap_handle, const unsigned char *mac);

#ifndef STA_REF_DBG

static __INLINE sta_entry *
mtlk_stadb_find_sta (sta_db *stadb, const unsigned char *mac)
{
  sta_entry                    *sta = NULL;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;

  mtlk_osal_lock_acquire(&stadb->lock);
  h = mtlk_hash_find_ieee_addr(&stadb->hash, (IEEE_ADDR *)mac);
  if (h) {
    sta = MTLK_CONTAINER_OF(h, sta_entry, hentry);
    mtlk_sta_incref(sta); /* Reference by caller */
  }
  else { /* if not found, search ML hash */
    h = mtlk_hash_find_ieee_addr(&stadb->mlhash, (IEEE_ADDR *)mac);
    if (h) {
      sta = MTLK_CONTAINER_OF(h, sta_entry, hMLentry);
      mtlk_sta_incref(sta); /* Reference by caller */
    }
  }
  mtlk_osal_lock_release(&stadb->lock);

  return sta;
}

#else
#define mtlk_stadb_find_sta(stadb, mac) \
  __mtlk_stadb_find_sta_dbg(__FUNCTION__, __LINE__, (stadb), (mac))

static __INLINE sta_entry *
__mtlk_stadb_find_sta_dbg (const char *f, int l, sta_db *stadb, const unsigned char *mac)
{
  sta_entry                    *sta = NULL;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;

  mtlk_osal_lock_acquire(&stadb->lock);
  h = mtlk_hash_find_ieee_addr(&stadb->hash, (IEEE_ADDR *)mac);
  if (h) {
    sta = MTLK_CONTAINER_OF(h, sta_entry, hentry);
    __mtlk_sta_incref_dbg(f, l, sta); /* Reference by caller */
  }
  mtlk_osal_lock_release(&stadb->lock);

  return sta;
}
#endif
#ifdef UNUSED_CODE
sta_entry * __MTLK_IFUNC
mtlk_stadb_get_first_sta (sta_db *stadb);
#endif /* UNUSED_CODE */
sta_entry * __MTLK_IFUNC
mtlk_stadb_get_last_sta (sta_db *stadb);

static __INLINE sta_entry *
mtlk_stadb_get_ap (sta_db *stadb)
{
  return mtlk_stadb_get_last_sta(stadb);
}

static __INLINE uint32
mtlk_stadb_get_four_addr_sta_cnt (sta_db *stadb)
{
  return mtlk_osal_atomic_get(&stadb->four_addr_sta_cnt);
}

#ifdef MTLK_WAVE_700
static __INLINE sta_entry *
mtlk_get_sibling_sta(sta_entry *sta)
{
  MTLK_ASSERT(NULL != sta);
  return sta->ml_sta_info.sibling_sta;
}
#endif
static __INLINE BOOL
wave_is_mld_sta (sta_entry *sta)
{
  struct ieee80211_sta *mac80211_sta;
  if (sta != NULL) {
    mac80211_sta = wv_sta_entry_get_mac80211_sta(sta);
    return mac80211_sta->ml_sta_info.is_ml;
  }
  return FALSE;
}

static __INLINE const IEEE_ADDR *
wave_get_sta_mld_addr (sta_entry *sta)
{
  MTLK_ASSERT(NULL != sta);
  return (const IEEE_ADDR *)wv_sta_entry_get_mac80211_sta(sta)->ml_sta_info.mld_mac;
}

static __INLINE BOOL
wave_is_main_sta (sta_entry *sta)
{
  MTLK_ASSERT(NULL != sta);

  if (mtlk_vap_get_id_fw(sta->vap_handle) == sta->info.MainVapId)
    return TRUE;
  else
    return FALSE;
}

static __INLINE BOOL
mtlk_is_single_link_mld_sta(sta_entry *sta)
{
  return mtlk_osal_is_zero_address(wv_sta_entry_get_mac80211_sta(sta)->ml_sta_info.linked_sta_mac);
}

typedef enum multilink_modes {
  MLSR_MODE,
  EMLSR_MODE,
  STR_MODE
} multilink_modes_e;

static __INLINE uint8
mtlk_sta_get_ml_supp_mode (sta_entry *sta)
{
  return sta->ml_sta_info.ml_supp_mode;
}

typedef struct
{
  sta_entry **arr;
  uint32      size;
  uint32      idx;
  sta_db     *stadb;
} __MTLK_IDATA mtlk_stadb_iterator_t;

struct _hst_db; /* TODO: replace it with Param DB/callbacks with opaque pointers */

typedef enum {
  STAT_ID_STADB = 1,
  STAT_ID_HSTDB,
  STAT_ID_ML_STR_STA,
} __MTLK_IDATA mtlk_stadb_stat_id_e;

typedef struct {
  IEEE_ADDR addr;
  uint16    sta_sid;
  uint32    sta_rx_dropped;
  uint8     network_mode;
  uint16    tx_rate;
  uint32    aid;
  uint32    vap;
  BOOL      is_sta_auth;
  BOOL      is_four_addr;
  mtlk_wssa_drv_peer_stats_t  peer_stats;
  uint8     nss;
} __MTLK_IDATA mtlk_stadb_general_stat_t;

typedef struct {
  IEEE_ADDR addr;
} __MTLK_IDATA mtlk_hstdb_stat_t;

#ifdef BEST_EFFORT_TID_SPREADING
typedef struct {
  IEEE_ADDR sta_addr;
  IEEE_ADDR sib_sta_addr;
  uint32    aid;
  wave_ml_str_sta_tid_spreading_info_t cfg;
} __MTLK_IDATA wave_ml_str_sta_tid_spread_stat_t;
#endif

typedef struct {
  mtlk_stadb_stat_id_e    type;
  union {
    mtlk_stadb_general_stat_t general_stat;
    mtlk_hstdb_stat_t hstdb_stat;
#ifdef BEST_EFFORT_TID_SPREADING
    wave_ml_str_sta_tid_spread_stat_t tid_spread_stat;
#endif
  } u;
}__MTLK_IDATA mtlk_stadb_stat_t;

int __MTLK_IFUNC
mtlk_stadb_get_peer_list(sta_db *stadb, mtlk_clpb_t *clpb);

void __MTLK_IFUNC
mtlk_sta_get_associated_dev_stats(const sta_entry* sta, peerFlowStats* peer_stats);

int __MTLK_IFUNC
mtlk_stadb_get_stat(sta_db *stadb, struct _hst_db *hstdb, mtlk_clpb_t *clpb, uint8 group_cipher);

void __MTLK_IFUNC
wave_sta_get_dev_diagnostic_res2(mtlk_core_t *core, const sta_entry* sta, wifiAssociatedDevDiagnostic2_t *dev_diagnostic_stats);

void __MTLK_IFUNC
wave_sta_get_dev_diagnostic_res3(mtlk_core_t *core, sta_entry* sta, wifiAssociatedDevDiagnostic3_t *dev_diagnostic_stats);

void __MTLK_IFUNC
mtlk_sta_get_packet_error_rate_stats (sta_entry *sta, perRatePERStats *stats_per);

void __MTLK_IFUNC
mtlk_stadb_reset_cnts(sta_db *stadb);

typedef void (__MTLK_IFUNC * mtlk_stadb_disconnect_sta_clb_f)(mtlk_handle_t usr_ctx, sta_entry *sta);

void __MTLK_IFUNC
mtlk_stadb_disconnect_all(sta_db *stadb,
    mtlk_stadb_disconnect_sta_clb_f clb,
    mtlk_handle_t usr_ctx,
    BOOL wait_all_packets_confirmed);

static BOOL __INLINE
mtlk_stadb_is_empty(sta_db *stadb)
{
  return stadb->hash_cnt == 0;
}

uint32 __MTLK_IFUNC
mtlk_stadb_stas_num(sta_db *stadb);

const sta_entry * __MTLK_IFUNC
mtlk_stadb_iterate_first(sta_db *stadb, mtlk_stadb_iterator_t *iter);
const sta_entry * __MTLK_IFUNC
mtlk_stadb_iterate_next(mtlk_stadb_iterator_t *iter);
void __MTLK_IFUNC
mtlk_stadb_iterate_done(mtlk_stadb_iterator_t *iter);
void __MTLK_IFUNC
mtlk_stadb_iterate_done_none_decref(mtlk_stadb_iterator_t *iter);

BOOL __MTLK_IFUNC
mtlk_global_stadb_is_empty(void);

typedef struct _hst_db
{
  mtlk_hash_t          hash;
  mtlk_osal_spinlock_t lock;
  mtlk_osal_timestamp_t last_cleanup;
  uint32               hash_cnt;
  uint32               wds_host_timeout;
  mtlk_vap_handle_t    vap_handle;
  /* default host related */
  IEEE_ADDR            default_host;
  IEEE_ADDR            local_mac;
  MTLK_DECLARE_INIT_STATUS;
} __MTLK_IDATA hst_db;

typedef struct _host_entry {
  MTLK_HASH_ENTRY_T(ieee_addr) hentry;
  mtlk_osal_timer_t            idle_timer;
  mtlk_osal_timestamp_t        timestamp;
  IEEE_ADDR                    sta_addr;
  struct _hst_db              *paramdb;
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_START_STATUS;
} host_entry;

typedef struct _sta_whm_entry {
  MTLK_HASH_ENTRY_T(ieee_addr) hentry;
  mtlk_osal_timestamp_t        discnct_timestamp[MAX_WHM_DISCONNECTIONS_PER_CYCLE];
  uint32                       nof_discncts;
} sta_whm_entry;

typedef struct _sta_whm_discnt_data {
  IEEE_ADDR              sta_addr;
  mtlk_osal_timestamp_t  discnct_timestamp;
} sta_whm_discnt_data;

static __INLINE const IEEE_ADDR *
_mtlk_hst_get_addr (const host_entry *hst)
{
  return MTLK_HASH_VALUE_GET_KEY(hst, hentry);
}

int __MTLK_IFUNC
mtlk_hstdb_init(hst_db *hstdb, mtlk_vap_handle_t vap_handle);

void __MTLK_IFUNC
mtlk_hstdb_cleanup(hst_db *hstdb);

sta_entry * __MTLK_IFUNC
mtlk_hstdb_find_sta(hst_db* hstdb, const unsigned char *mac);

void __MTLK_IFUNC
mtlk_hstdb_update_host(hst_db *hstdb, const unsigned char *mac,
		               IEEE_ADDR * sta_addr);

int __MTLK_IFUNC
mtlk_hstdb_remove_host_by_addr(hst_db *hstdb, const IEEE_ADDR *mac);

void __MTLK_IFUNC
wv_hostdb_cleanup(hst_db *hstdb);

void __MTLK_IFUNC
mtlk_hstdb_update_default_host(hst_db* hstdb, const unsigned char *mac);

int __MTLK_IFUNC
mtlk_hstdb_remove_all_by_sta(hst_db *hstdb, const IEEE_ADDR *sta_addr);

int __MTLK_IFUNC
mtlk_hstdb_stop_all_by_sta(hst_db *hstdb, const IEEE_ADDR *sta_addr);

int __MTLK_IFUNC
mtlk_hstdb_start_all_by_sta(hst_db *hstdb, IEEE_ADDR *sta_addr);

int __MTLK_IFUNC
mtlk_hstdb_dcdp_remove_all_by_sta(hst_db *hstdb, const sta_entry *sta);
#ifdef UNUSED_CODE
mtlk_vap_handle_t __MTLK_IFUNC
mtlk_host_get_vap_handle(mtlk_handle_t host_handle);
#endif /* UNUSED_CODE */
static __INLINE IEEE_ADDR *
mtlk_hstdb_get_default_host (hst_db* hstdb)
{
  if (ieee_addr_is_valid(&hstdb->default_host))
    return &hstdb->default_host;
  return NULL;
}

typedef struct
{
  IEEE_ADDR *addr;
  uint32     size;
  uint32     idx;
} __MTLK_IDATA mtlk_hstdb_iterator_t;

const IEEE_ADDR * __MTLK_IFUNC
mtlk_hstdb_iterate_first(hst_db *hstdb, const IEEE_ADDR *sta_addr, mtlk_hstdb_iterator_t *iter);
const IEEE_ADDR * __MTLK_IFUNC
mtlk_hstdb_iterate_next(mtlk_hstdb_iterator_t *iter);
void __MTLK_IFUNC
mtlk_hstdb_iterate_done(mtlk_hstdb_iterator_t *iter);

void __MTLK_IFUNC
mtlk_sta_update_mhi_peers_stats(sta_entry *sta);

void
mtlk_sta_get_tr181_peer_stats(const sta_entry* sta, mtlk_wssa_drv_tr181_peer_stats_t *stats);

struct driver_sta_info;

void
mtlk_core_get_driver_sta_info (const sta_entry* sta, struct driver_sta_info *stats);

 #ifdef MTLK_PER_RATE_STAT
void __MTLK_IFUNC
mtlk_sta_rate_stat_clean_hash (sta_entry *sta);

int  __MTLK_IFUNC
mtlk_sta_rate_stat_hash_init (sta_entry * sta);
#endif /* MTLK_PER_RATE_STAT */

void
_mtlk_sta_get_peer_rates_info(const sta_entry *sta, mtlk_wssa_drv_peer_rates_info_t *rates_info);

void __MTLK_IFUNC
wave_sta_get_mixed_stats(const sta_entry* sta, wave_peer_mixed_stats_t *mixed_stats);

BOOL
mtlk_sta_get_is_sta_auth (const sta_entry *sta);

#ifdef BEST_EFFORT_TID_SPREADING
mtlk_error_t __MTLK_IFUNC
wave_stadb_cleanup_skb_hash (mtlk_handle_t hcore, const void *data, uint32 data_size);

mtlk_error_t __MTLK_IFUNC
wave_calculate_str_sta_effective_rates (mtlk_handle_t hcore, const void *data, uint32 data_size);

mtlk_error_t __MTLK_IFUNC
wave_get_ml_str_sta_tid_spread_stat (sta_db *stadb, mtlk_clpb_t *clpb);

static __INLINE wave_ml_str_sta_tid_spreading_info_t *
wave_get_str_sta_tid_spreading_info (sta_entry *sta)
{
  MTLK_ASSERT(sta != NULL);
  return sta->ml_sta_info.sta_tid_spread_info;
}
#endif

static __INLINE void
wave_sta_get_wssa_peer_list (const sta_entry *sta, mtlk_wssa_peer_list_t *peer_list)
{
  peer_list->addr = *(mtlk_sta_get_addr(sta));
  peer_list->is_sta_auth = mtlk_sta_get_is_sta_auth(sta);
}

static __INLINE void mtlk_stadb_stats_set_mgmt_rssi(sta_info  *info, int32 rssi)
{
  unsigned i;

  info->stats.mgmt_max_rssi = MAX(rssi, MIN_RSSI);
  for (i = 0; i < MTLK_ARRAY_SIZE(info->stats.mgmt_rssi); i++) {
    info->stats.mgmt_rssi[i] = info->stats.mgmt_max_rssi;
  }
}

/* Locates MSB that is not set */
static __INLINE int locate_ms0bit_16 (uint16 i_x)
{
  int i;
  for (i = 15; i >= 0; i--)
    if (MTLK_BIT_GET(i_x, i) == 0)
      return i;
  return -1;
}

static __INLINE uint64 wave_sta_get_rx_droped_cnt (const sta_entry* sta)
{
  return sta->sta_stats64_cntrs.swUpdateDrop +
         sta->sta_stats64_cntrs.rdDuplicateDrop +
         sta->sta_stats64_cntrs.missingSn;
}

#define   MTLK_IDEFS_OFF
#include "mtlkidefs.h"

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID

#endif /* !_MTLK_STADB_H_ */
