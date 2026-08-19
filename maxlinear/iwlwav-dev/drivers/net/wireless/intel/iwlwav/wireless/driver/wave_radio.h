/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
 * $Id$
 *
 *
 *
 * Radio module definitions
 *
 */
#ifndef __WAVE_RADIO_H__
#define __WAVE_RADIO_H__

/**
*\file wave_radio.h
*/
#include "mtlkhal.h"
#include "cfg80211.h"
#include "stadb.h"

#define MAX_NUM_TX_ANTENNAS_654 (2)

typedef struct _wave_radio_t wave_radio_t;
typedef struct wave_thermal wave_thermal_t;

typedef struct {
  wave_radio_t  *radio;
  unsigned       num_radios;
} wave_radio_descr_t;

typedef struct {
  unsigned    max_vaps;
  unsigned    max_stas;
  unsigned    master_vap_id;
} wave_radio_limits_t;

typedef struct {
  uint32    txUnicastPD;
  uint32    txMulticastPD;
  uint32    txBroadcastPD;
  uint64    txUnicastBytePD;
  uint64    txMulticastBytePD;
  uint64    txBroadcastBytePD;
} wave_radio_statsPD_t;

/* These are values for parameter "is_recovery" of
   the function "wave_radio_calibrate" */
enum {
  WAVE_RADIO_FAST_RCVRY,
  WAVE_RADIO_FULL_RCVRY,
  WAVE_RADIO_NO_RCVRY = WAVE_RADIO_FULL_RCVRY
};

/* Multi BSS VAP operation modes */
enum {
  WAVE_RADIO_NON_MULTIBSS_VAP = 0,
  WAVE_RADIO_OPERATION_MODE_MBSSID_TRANSMIT_VAP = 1,
  WAVE_RADIO_OPERATION_MODE_MBSSID_NON_TRANSMIT_VAP = 2,
  WAVE_RADIO_OPERATION_MODE_MBSSID_LAST = WAVE_RADIO_OPERATION_MODE_MBSSID_NON_TRANSMIT_VAP
};

/* PhyStat averager with moving window */
#define WAVE_PHY_STAT_AVG_TIME_MS     (3 * 1000)
#define WAVE_PHY_STAT_AVG_WINSIZE     (WAVE_PHY_STAT_AVG_TIME_MS / _DF_STAT_POLL_PERIOD)

/* ZWDFS Antenna RSSI averager*/
// Time window (2 seconds)
#define WAVE_ZWDFS_ANT_RSSI_AVG_TIME_MS     (2 * 1000)
#define WAVE_ZWDFS_ANT_RSSI_AVG_WINSIZE     (WAVE_ZWDFS_ANT_RSSI_AVG_TIME_MS / _DF_STAT_POLL_PERIOD)
// Number of max RSSI samples to be considered within the window
#define WAVE_ZWDFS_ANT_NUM_MAX_RSSI_SAMPLES (3)

typedef struct {
  uint32  summator;
  uint32  window_size;
  uint32  idx;
  uint8   values[WAVE_PHY_STAT_AVG_WINSIZE]; /* can be made dynamic later */
} wave_phy_stat_averager_t;

typedef struct {
  int8    rssi_values[WAVE_ZWDFS_ANT_RSSI_AVG_WINSIZE];
  uint8   window_size;
  uint8   num_max_rssi_samples;
  uint8   idx;
} wave_zwdfs_ant_rssi_averager_t;

/* UL DL OFDMA MPDU radio stats */
typedef struct wave_mpdu_stats {
  uint32 success_mpdus_count;
  uint32 failed_mpdus_count;
} wave_mpdu_stats_t;

typedef struct wave_radio_mpdu_ofdma_stats {
  wave_mpdu_stats_t mu_mpdu_stats[MAX_NUM_OF_RU_SIZES];
  wave_mpdu_stats_t su_mpdu_stats[MAX_NUM_OF_BW];
} wave_radio_mpdu_ofdma_stats_t;

typedef struct wave_radio_ppdu_ofdma_stats {
  uint32 total_ppdus_count;
  uint32 su_ppdus_count;
  uint32 mu_ofdma_ppdus_count;
  uint32 mu_mimo_ppdus_count;
  uint32 mu_ru_ppdus[MAX_NUM_OF_RU_SIZES];
} wave_radio_ppdu_ofdma_stats_t;

typedef struct wave_radio_pdus_ofdma_stats {
  wave_radio_ppdu_ofdma_stats_t ppdu_stats;
  wave_radio_mpdu_ofdma_stats_t mpdu_stats;
} wave_radio_pdus_ofdma_stats_t;
/* UL DL OFDMA MPDU radio stats ends */

/* HE MIMO radio stats */
typedef struct wave_radio_rate_mimo_stats {
  uint32 nss_ppdus[MAX_REDUCED_NSS];
  uint32 mcs_ppdus[MAX_REDUCED_MCS];
} wave_radio_rate_mimo_stats_t;

typedef struct wave_radio_su_mimo_stats {
  wave_radio_rate_mimo_stats_t su_rate;
} wave_radio_su_mimo_stats_t;

typedef struct wave_radio_mu_mimo_stats {
  wave_radio_rate_mimo_stats_t mu_rate;
  uint32 succeeded_sounding;
  uint32 attemptedsounding_per_mcs[MAX_REDUCED_MCS];
} wave_radio_mu_mimo_stats_t;

typedef struct wave_radio_mimo_stats {
  wave_radio_su_mimo_stats_t su_mimo;
  wave_radio_mu_mimo_stats_t mu_mimo;
} wave_radio_mimo_stats_t;
/* HE MIMO radio stats ends */

/* UL DL OFDMA PPDU radio stats_64 */
typedef struct wave_mpdu_stats_64 {
  uint64 success_mpdus_count;
  uint64 failed_mpdus_count;
} wave_mpdu_stats64_t;

typedef struct wave_radio_mpdu_ofdma_stats_64 {
  wave_mpdu_stats64_t mu_mpdu_stats64[MAX_NUM_OF_RU_SIZES];
  wave_mpdu_stats64_t su_mpdu_stats64[MAX_NUM_OF_BW];
} wave_radio_mpdu_ofdma_stats64_t;

typedef struct wave_radio_ppdu_ofdma_stats_64 {
  uint32 bw_ru_in_ppdus[MAX_NUM_OF_BW][MAX_NUM_OF_RU_SIZES];
  uint64 total_ppdus_count;
  uint64 su_ppdus_count;
  uint64 mu_ofdma_ppdus_count;
  uint64 mu_mimo_ppdus_count;
  uint64 mu_ru_ppdus[MAX_NUM_OF_RU_SIZES];
  /* total mu_ru_ppdus of each Ru */
  uint64 total_mu_ru_ppdus;
} wave_radio_ppdu_ofdma_stats64_t;

typedef struct wave_radio_pdus_ofdma_stats64 {
  wave_radio_ppdu_ofdma_stats64_t ppdu_stats64;
  wave_radio_mpdu_ofdma_stats64_t mpdu_stats64;
} wave_radio_pdus_ofdma_stats64_t;
/* UL DL OFDMA PPDU radio stats_64 ends*/

/* HE MIMO radio stats_64 */
typedef struct wave_radio_rate_mimo_stats64 {
  uint64 nss_ppdus[MAX_REDUCED_NSS];
  uint64 mcs_ppdus[MAX_REDUCED_MCS];
} wave_radio_rate_mimo_stats64_t;

typedef struct wave_radio_su_mimo_stats64 {
  wave_radio_rate_mimo_stats64_t su_rate;
} wave_radio_su_mimo_stats64_t;

typedef struct wave_radio_mu_mimo_stats64 {
  wave_radio_rate_mimo_stats64_t mu_rate;
  uint64 succeeded_sounding;
  uint64 attemptedsounding_per_mcs[MAX_REDUCED_MCS];
} wave_radio_mu_mimo_stats64_t;

typedef struct wave_radio_mimo_stats64 {
  wave_radio_su_mimo_stats64_t su_mimo;
  wave_radio_mu_mimo_stats64_t mu_mimo;
} wave_radio_mimo_stats64_t;
/* HE MIMO radio stats_64 ends */

typedef struct
{
  uint8 type; /* from wave_mu_la_group_type_e */
  uint8 fw_idx;
  BOOL active;

  union {
    struct {
        uint32 protectionSentCnt;
        uint32 protectionSucceededCnt;
        uint32 dlPhaseAllocFailCnt;
        uint32 heMuFastProbeTriggeredCnt[MAX_STA_IN_GROUP];
        uint32 heMuFastProbeNotFinishedCnt[MAX_STA_IN_GROUP];
        uint32 heMuCollisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32 mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    } dl;

    struct {
        uint32 ulPCStabilityCnt;
        uint32 fastDropCnt[MAX_STA_IN_GROUP];
        uint32 invalidReportsCnt[MAX_STA_IN_GROUP];
        uint32 rxPPDUsCnt[MAX_STA_IN_GROUP];
        uint32 changedWpCnt[MAX_STA_IN_GROUP];
        uint32 collisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32 mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    } ul;
  };
} wave_mu_he_eht_la_stats32_t;

typedef struct
{
  union {
    struct {
        uint16 protectionSentCnt;
        uint16 protectionSucceededCnt;
        uint16 dlPhaseAllocFailCnt;
        uint16 heMuFastProbeTriggeredCnt[MAX_STA_IN_GROUP];
        uint16 heMuFastProbeNotFinishedCnt[MAX_STA_IN_GROUP];
        uint16 heMuCollisionDetectedCnt[MAX_STA_IN_GROUP];
        uint8  mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    } dl;

    struct {
        uint16 ulPCStabilityCnt;
        uint16 fastDropCnt[MAX_STA_IN_GROUP];
        uint16 invalidReportsCnt[MAX_STA_IN_GROUP];
        uint16 rxPPDUsCnt[MAX_STA_IN_GROUP];
        uint16 changedWpCnt[MAX_STA_IN_GROUP];
        uint16 collisionDetectedCnt[MAX_STA_IN_GROUP];
        uint8  mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS_GEN7];
    } ul;
  };
} wave_mu_he_eht_la_stats_t;

static __INLINE void
wave_phy_stat_averager_init (wave_phy_stat_averager_t *avg)
{
  avg->window_size = WAVE_PHY_STAT_AVG_WINSIZE;
  avg->summator = 0;
  avg->idx = 0;
  memset(avg->values, 0, WAVE_PHY_STAT_AVG_WINSIZE);
}

static __INLINE void
wave_phy_stat_zwdfs_ant_rssi_averager_init (wave_zwdfs_ant_rssi_averager_t *averager)
{
  memset(averager->rssi_values, MIN_RSSI, sizeof(averager->rssi_values));
  averager->window_size = WAVE_ZWDFS_ANT_RSSI_AVG_WINSIZE;
  averager->num_max_rssi_samples = WAVE_ZWDFS_ANT_NUM_MAX_RSSI_SAMPLES;
  averager->idx = 0;
}

static __INLINE void
wave_phy_stat_averager_proc (wave_phy_stat_averager_t *avg, uint8 val)
{
  avg->summator -= avg->values[avg->idx];
  avg->summator += val;
  avg->values[avg->idx] = val;
  INC_WRAP_IDX(avg->idx, avg->window_size);
}

static __INLINE void
wave_phy_stat_zwdfs_ant_rssi_averager_proc (wave_zwdfs_ant_rssi_averager_t *averager, int8 curr_rssi)
{
  averager->rssi_values[averager->idx] = curr_rssi;
  INC_WRAP_IDX(averager->idx, averager->window_size);
}

static __INLINE uint8
wave_phy_stat_averager_get (wave_phy_stat_averager_t *avg)
{
  uint32 summ;
  summ = avg->summator;
  summ += avg->window_size >> 1;  /* round up */
  return summ / avg->window_size;
}

/* Radio Phy Status according to devicePhyRxStatusDb */
typedef struct {
  /* Phy Rx Status data */
  int8                  noise;        /* noise                        dBm */
  uint8                 ch_load;      /* channel_load,            0..100% */
  uint8                 ch_util;      /* totalChannelUtilization, 0..100% */
  int8                  zwdfs_ant_rssi; /* ZWDFS Antenna RSSI value    dBm */
  /* Calculated */
  uint8                 airtime;            /*   0..100% */
  uint32                airtime_efficiency; /* bytes/sec */
  /* Averagers */
  wave_phy_stat_averager_t  ch_load_averager;
  wave_phy_stat_averager_t  ch_util_averager;
  wave_zwdfs_ant_rssi_averager_t zwdfs_ant_rssi_averager;
} wave_radio_phy_stat_t;

typedef struct _probe_req_info {
  uint32 size;
  struct intel_vendor_probe_req_info *data;
} probe_req_info;

/**************************************************************
    WSS counters for HW Radio statistics
 */
typedef enum
{
  WAVE_RADIO_CNT_BYTES_SENT,
  WAVE_RADIO_CNT_BYTES_RECEIVED,
  WAVE_RADIO_CNT_PACKETS_SENT,
  WAVE_RADIO_CNT_PACKETS_RECEIVED,

  WAVE_RADIO_CNT_UNICAST_PACKETS_SENT,
  WAVE_RADIO_CNT_MULTICAST_PACKETS_SENT,
  WAVE_RADIO_CNT_BROADCAST_PACKETS_SENT,
  WAVE_RADIO_CNT_UNICAST_BYTES_SENT,
  WAVE_RADIO_CNT_MULTICAST_BYTES_SENT,
  WAVE_RADIO_CNT_BROADCAST_BYTES_SENT,

  WAVE_RADIO_CNT_UNICAST_PACKETS_RECEIVED,
  WAVE_RADIO_CNT_MULTICAST_PACKETS_RECEIVED,
  WAVE_RADIO_CNT_BROADCAST_PACKETS_RECEIVED,
  WAVE_RADIO_CNT_UNICAST_BYTES_RECEIVED,
  WAVE_RADIO_CNT_MULTICAST_BYTES_RECEIVED,
  WAVE_RADIO_CNT_BROADCAST_BYTES_RECEIVED,

  WAVE_RADIO_CNT_ERROR_PACKETS_SENT,
  WAVE_RADIO_CNT_ERROR_PACKETS_RECEIVED,
  WAVE_RADIO_CNT_DISCARD_PACKETS_RECEIVED,

  WAVE_RADIO_CNT_RX_PACKETS_DISCARDED_DRV_DUPLICATE,

  WAVE_RADIO_CNT_802_1X_PACKETS_RECEIVED,
  WAVE_RADIO_CNT_802_1X_PACKETS_SENT,
  WAVE_RADIO_CNT_802_1X_PACKETS_DISCARDED,
  WAVE_RADIO_CNT_PAIRWISE_MIC_FAILURE_PACKETS,
  WAVE_RADIO_CNT_GROUP_MIC_FAILURE_PACKETS,
  WAVE_RADIO_CNT_UNICAST_REPLAYED_PACKETS,
  WAVE_RADIO_CNT_MULTICAST_REPLAYED_PACKETS,
  WAVE_RADIO_CNT_MANAGEMENT_REPLAYED_PACKETS,
  WAVE_RADIO_CNT_FWD_RX_PACKETS,
  WAVE_RADIO_CNT_FWD_RX_BYTES,
  WAVE_RADIO_CNT_DAT_FRAMES_RECEIVED,
  WAVE_RADIO_CNT_CTL_FRAMES_RECEIVED,
  WAVE_RADIO_CNT_MAN_FRAMES_RES_QUEUE,
  WAVE_RADIO_CNT_MAN_FRAMES_SENT,
  WAVE_RADIO_CNT_MAN_FRAMES_CONFIRMED,
  WAVE_RADIO_CNT_MAN_FRAMES_RECEIVED,
  WAVE_RADIO_CNT_RX_MAN_FRAMES_RETRY_DROPPED,
  WAVE_RADIO_CNT_MAN_FRAMES_FAILED,
  WAVE_RADIO_CNT_TX_PROBE_RESP_SENT,
  WAVE_RADIO_CNT_TX_PROBE_RESP_DROPPED,
  WAVE_RADIO_CNT_BSS_MGMT_TX_QUE_FULL,
  WAVE_RADIO_CNT_PD_BROADCAST_PACKETS_SENT,
  WAVE_RADIO_CNT_PD_BROADCAST_BYTES_SENT_64,
  WAVE_RADIO_CNT_PD_UNICAST_PACKETS_SENT,
  WAVE_RADIO_CNT_PD_UNICAST_BYTES_SENT_64,
  WAVE_RADIO_CNT_PD_MULTICAST_PACKETS_SENT,
  WAVE_RADIO_CNT_PD_MULTICAST_BYTES_SENT_64,

  WAVE_RADIO_CNT_LAST
} wave_radio_cnt_id_e;

uint32 __MTLK_IFUNC
wave_radio_wss_cntr_get (wave_radio_t *radio, wave_radio_cnt_id_e id);

/**************************************************************/

#define WAVE_RADIO_ID_FIRST     (0)
#define WAVE_RADIO_ID_SECOND    (1)
#define WAVE_RADIO_ID_THIRD     (2)

static __INLINE BOOL
wave_radio_id_is_first (unsigned radio_id)
{
  return (WAVE_RADIO_ID_FIRST == radio_id);
}

wave_radio_descr_t* wave_radio_create(unsigned radio_num, mtlk_hw_api_t *hw_api, mtlk_work_mode_e work_mode);
int wave_radio_init(wave_radio_descr_t *radio_descr, struct device *dev);
int wave_radio_init_finalize(wave_radio_descr_t *radio_descr);
void wave_radio_destroy(wave_radio_descr_t *radio_descr);
void wave_radio_deinit(wave_radio_descr_t *radio_descr);
void wave_radio_deinit_finalize(wave_radio_descr_t *radio_descr);
void wave_radio_ieee80211_unregister(wave_radio_descr_t *radio_descr);
void wave_radio_prepare_stop(wave_radio_descr_t *radio_descr);

mtlk_hw_api_t       *wave_radio_descr_hw_api_get(wave_radio_descr_t *radio_descr, unsigned radio_idx);
mtlk_vap_manager_t  *wave_radio_descr_vap_manager_get(wave_radio_descr_t *radio_descr, unsigned radio_idx);
int   wave_radio_descr_master_vap_handle_get (wave_radio_descr_t *radio_descr, unsigned radio_idx, mtlk_vap_handle_t  *vap_handle);

unsigned             wave_radio_id_get (wave_radio_t *radio);
mtlk_wss_t          *wave_radio_wss_get(wave_radio_t *radio);
mtlk_vap_manager_t  *wave_radio_vap_manager_get(wave_radio_t *radio);

mtlk_df_t           *wave_radio_df_get(wave_radio_t *radio);
mtlk_core_t         *wave_radio_master_core_get(wave_radio_t *radio);
int   wave_radio_vap_handle_get (wave_radio_t *radio, mtlk_vap_handle_t  *vap_handle);
void *wave_radio_beacon_man_private_get(wave_radio_t *radio);

int wave_radio_beacon_change(wave_radio_t *radio, struct net_device *ndev, struct cfg80211_beacon_data *beacon);
int wave_radio_abort_and_prevent_scan (wave_radio_t *radio);
int wave_radio_allow_or_resume_scan (wave_radio_t *radio);
int wave_radio_ap_start(struct wiphy *wiphy, wave_radio_t *radio, struct net_device *ndev, struct ieee80211_vif *vif, struct ieee80211_bss_conf *bss_conf, struct cfg80211_beacon_data *beacon);
int wave_radio_ap_stop(wave_radio_t *radio, struct net_device *ndev);
int wave_radio_sta_change(wave_radio_t *radio, struct net_device *ndev, const uint8 *mac, struct station_parameters *params, struct ieee80211_sta * sta);
int wave_radio_scan(wave_radio_t *radio, struct net_device *ndev, struct cfg80211_scan_request *request);
int wave_radio_cancel_hw_scan (wave_radio_t *radio);
int wave_radio_qos_map_set(wave_radio_t *radio, struct net_device *ndev, struct cfg80211_qos_map *qos_map);
int wave_radio_aid_get(struct net_device *ndev, const uint8 *mac_addr, u16 *aid);
int wave_radio_aid_free(struct net_device *ndev, u16 aid);
int wave_radio_sync_done(struct net_device *ndev);
int wave_radio_initial_data_receive(struct net_device *ndev, struct intel_vendor_initial_data_cfg *initial_data);
int wave_radio_deny_mac_set(struct net_device *ndev, struct intel_vendor_blacklist_cfg *blacklist_cfg);
int wave_radio_sta_steer(struct net_device *ndev, struct intel_vendor_steer_cfg *steer_cfg);
int wave_radio_sta_measurements_get(struct wiphy *wiphy, struct net_device *ndev, uint8 *addr);
int wave_radio_vap_measurements_get(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_set_atf_quotas(struct wiphy *wiphy, struct net_device *ndev, const void *data, int total_len);
int wave_radio_set_wds_wpa_sta(struct net_device *ndev, struct intel_vendor_mac_addr_list_cfg *addrlist_cfg);

int wave_radio_unconnected_sta_get(struct wiphy *wiphy, struct wireless_dev *wdev, struct intel_vendor_unconnected_sta_req_cfg *sta_req_data);

int wave_radio_radio_info_get(struct wiphy *wiphy, struct wireless_dev *wdev);

int wave_radio_antenna_set(wave_radio_t *radio, u32 tx_ant, u32 rx_ant);
int wave_radio_antenna_get(wave_radio_t *radio, u32 *tx_ant, u32 *rx_ant);

int wave_radio_rts_threshold_set(wave_radio_t *radio, u32 rts_threshold);

/* WIFI HAL* API's */
int wave_radio_get_associated_dev_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_channel_stats(struct wiphy *wiphy, struct net_device *ndev, const void *data, size_t data_len);
int wave_radio_get_phy_channel_status(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_associated_dev_tid_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_associated_dev_rate_info_rx_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_associated_dev_rate_info_tx_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);

int wave_radio_get_twt_params(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);

int wave_radio_get_peer_list(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_peer_flow_status(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_peer_capabilities(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_peer_rate_info(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_recovery_statistics(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_hw_flow_status(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_tr181_wlan_statistics(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_tr181_hw_statistics(struct wiphy *wiphy, struct net_device *ndev);
int wave_radio_get_tr181_peer_statistics(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_dev_diag_result2(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_dev_diag_result3(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
int wave_radio_get_associated_stations_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr_list, size_t data_len);
int wave_radio_get_priority_gpio_stats(struct wiphy *wiphy, struct net_device *ndev);

/* CCA API */
int __MTLK_IFUNC wave_radio_cca_stats_get (struct wiphy *wiphy, struct net_device *ndev);
int __MTLK_IFUNC wave_radio_cca_msr_start(struct wiphy *wiphy, struct net_device *ndev, struct intel_vendor_cca_msr_cfg *cca_msr_cfg);
int __MTLK_IFUNC wave_radio_cca_msr_get(struct wiphy *wiphy, struct net_device *ndev, uint32 *user_chan);
void __MTLK_IFUNC wave_radio_cca_msr_done (wave_radio_t *radio, mtlk_core_t *core);
void __MTLK_IFUNC wave_radio_cca_stat_update (wave_radio_t *radio, wave_cca_stats_t *new_cca_stats, int channel);
void __MTLK_IFUNC wave_radio_cca_stat_reset (wave_radio_t *radio);

/* General Radio stats */
int __MTLK_IFUNC wave_radio_usage_stats_get (struct wiphy *wiphy, struct net_device *ndev);

int __MTLK_IFUNC wave_radio_get_peer_mixed_stats(struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);

#ifdef CONFIG_WAVE_DEBUG
int wave_radio_get_peer_host_if_qos(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_host_if(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_rx_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_ul_bsrc_tid_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_baa_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_link_adaption_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_plan_manager_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_twt_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_per_client_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_phy_rx_status(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_peer_info(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_wlan_host_if_qos(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_wlan_host_if(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_wlan_rx_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_wlan_baa_stats(wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_radio_rx_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_radio_baa_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_tsman_init_tid_gl (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_tsman_init_sta_gl (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_tsman_rcpt_tid_gl (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_tsman_rcpt_sta_gl (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_radio_link_adapt_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_multicast_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_training_man_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_group_man_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_general_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_cur_channel_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_radio_phy_rx_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_dynamic_bw_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
int wave_radio_get_la_mu_vht_stats (wave_radio_t *radio, struct net_device *ndev, const void *data, int total_len);
void __MTLK_IFUNC wave_radio_on_rcvry_reset_mu_group_stats (wave_radio_t *radio);
#endif /* CONFIG_WAVE_DEBUG */

int wave_radio_he_operation_get(wave_radio_t *radio, struct wiphy *wiphy, struct wireless_dev *wdev);
int wave_radio_he_non_advertised_get(wave_radio_t *radio, struct wiphy *wiphy, struct wireless_dev *wdev);

void wave_radio_ab_register(wave_radio_t *radio);
void wave_radio_ab_unregister(wave_radio_t *radio);

uint8 wave_radio_last_pm_freq_get(wave_radio_t *radio);
void wave_radio_last_pm_freq_set(wave_radio_t *radio, uint8 last_pm_freq);
mtlk_pdb_t *wave_radio_param_db_get(wave_radio_t *radio);

void __MTLK_IFUNC _wave_radio_set_11n_acax_compat (wave_radio_t *radio, int value);
int __MTLK_IFUNC wave_radio_get_11n_acax_compat (wave_radio_t *radio);

#define WAVE_RADIO_PDB_GET_INT(radio, id) \
  wave_pdb_get_int(wave_radio_param_db_get((radio)), (id))

#define WAVE_RADIO_PDB_GET_UINT(radio, id) \
  (unsigned)wave_pdb_get_int(wave_radio_param_db_get((radio)), (id))

#define WAVE_RADIO_PDB_SET_INT(radio, id, value) \
  wave_pdb_set_int(wave_radio_param_db_get((radio)), (id), (unsigned)(value))

#define WAVE_RADIO_PDB_SET_UINT(radio, id, value) \
  wave_pdb_set_int(wave_radio_param_db_get((radio)), (id), (value))

#define WAVE_RADIO_PDB_GET_BINARY(radio, id, buf, size) \
  wave_pdb_get_binary(wave_radio_param_db_get((radio)), (id), (buf), (size))

#define WAVE_RADIO_PDB_SET_BINARY(radio, id, value, size) \
  wave_pdb_set_binary(wave_radio_param_db_get((radio)), (id), (value), (size))

#define WAVE_RADIO_PDB_GET_MAC(radio, id, mac) \
  wave_pdb_get_mac(wave_radio_param_db_get((radio)), (id), (mac))

#define WAVE_RADIO_PDB_SET_MAC(radio, id, mac) \
  wave_pdb_set_mac(wave_radio_param_db_get((radio)), (id), (mac))

unsigned wave_radio_scan_is_running(wave_radio_t *radio);
unsigned wave_radio_bg_scan_is_running(wave_radio_t *radio);
unsigned wave_radio_bg_scan_on_break(wave_radio_t *radio);
unsigned wave_radio_scan_is_ignorant(wave_radio_t *radio);
uint32 wave_radio_scan_flags_get(wave_radio_t *radio);
mtlk_scan_support_t* wave_radio_scan_support_get(wave_radio_t *radio);
struct mtlk_chan_def* wave_radio_chandef_get(wave_radio_t *radio);
int wave_radio_chan_switch_type_get(wave_radio_t *radio);
void wave_radio_chan_switch_type_set(wave_radio_t *radio, int value);
void wave_radio_chandef_copy(struct mtlk_chan_def *mcd, struct cfg80211_chan_def *chandef);
int wave_radio_set_first_non_dfs_chandef (wave_radio_t * radio, struct mtlk_chan_def *ccd);

UMI_HDK_CONFIG *wave_radio_hdkconfig_get(wave_radio_t *radio);
mtlk_coc_t *wave_radio_coc_mgmt_get(wave_radio_t *radio);
mtlk_erp_t *wave_radio_erp_mgmt_get(wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_interfdet_set(wave_radio_t *radio, BOOL enable_flag, int8 threshold);
BOOL wave_radio_interfdet_get(wave_radio_t *radio);
BOOL wave_radio_is_rate_80211b(uint8 bitrate);
BOOL wave_radio_is_rate_80211ag(uint8 bitrate);
uint32 wave_radio_mode_get(wave_radio_t *radio);
void wave_radio_mode_set(wave_radio_t *radio, const uint32 radio_mode);
void wave_radio_limits_set(wave_radio_descr_t *radio_descr, wave_radio_limits_t *radio_limits);
unsigned wave_radio_max_stas_get(wave_radio_t *radio);
unsigned wave_radio_max_vaps_get(wave_radio_t *radio);
unsigned wave_radio_master_vap_id_get(wave_radio_t *radio);

uint32 wave_radio_chandef_width_get(const struct cfg80211_chan_def *c);

BOOL    __MTLK_IFUNC wave_radio_progmodel_loaded_get (struct _wave_radio_t *radio);
void    __MTLK_IFUNC wave_radio_progmodel_loaded_set (struct _wave_radio_t *radio, BOOL is_loaded);

BOOL    __MTLK_IFUNC wave_radio_get_sta_vifs_exist (wave_radio_t *radio);
void    __MTLK_IFUNC wave_radio_recovery_deinit_vifs (wave_radio_t *radio);
void    __MTLK_IFUNC wave_radio_recovery_init_vif(wave_radio_t *radio, u8 vap_index);


void wave_radio_sta_cnt_inc(wave_radio_t *radio);
void wave_radio_sta_cnt_dec(wave_radio_t *radio);
int  wave_radio_sta_cnt_get(wave_radio_t *radio);

struct  ieee80211_hw    * __MTLK_IFUNC wave_radio_ieee80211_hw_get (wave_radio_t *radio);
struct  _wv_mac80211_t  * __MTLK_IFUNC wave_radio_mac80211_get (wave_radio_t *radio);
struct  _wave_radio_t   * __MTLK_IFUNC wave_radio_descr_wave_radio_get (wave_radio_descr_t *radio_descr, unsigned idx);

int  __MTLK_IFUNC wave_radio_cca_threshold_get (wave_radio_t *radio, iwpriv_cca_th_t *cca_th);
int  __MTLK_IFUNC wave_radio_cca_threshold_set (wave_radio_t *radio, iwpriv_cca_th_t *cca_th);

BOOL __MTLK_IFUNC wave_radio_is_phy_dummy(wave_radio_t *radio);
BOOL __MTLK_IFUNC wave_radio_is_first(wave_radio_t *radio);

int wave_radio_channel_table_build_2ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels);
int wave_radio_channel_table_build_5ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels);
int wave_radio_channel_table_build_6ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels);
void wave_radio_channel_table_print(wave_radio_t *radio);
int wave_radio_calibrate(wave_radio_descr_t *radio_descr, BOOL is_recovery);
void wave_radio_band_set(wave_radio_t *radio, nl80211_band_e ieee_band);
mtlk_hw_band_e wave_radio_band_get(wave_radio_t *radio);
wave_cdb_cfg_e wave_radio_cdb_config_get(wave_radio_t *radio);
struct ieee80211_channel * __MTLK_IFUNC wave_radio_channel_get(struct wiphy *wiphy, mtlk_hw_band_e band, uint32 chan);
uint32 _wave_radio_chandef_width_get (const struct cfg80211_chan_def *c);
void __MTLK_IFUNC wave_radio_ch_switch_event (struct wiphy *wiphy, mtlk_core_t *core, struct mtlk_chan_def *mcd);
void wave_radio_calibration_status_get(wave_radio_t *radio, uint8 idx, uint8 *calib_done_mask, uint8 *calib_failed_mask);

int __MTLK_IFUNC wave_radio_send_hdk_config(wave_radio_t *radio, uint32 offline_mask, uint32 online_mask);
int __MTLK_IFUNC wave_radio_read_hdk_config(wave_radio_t *radio, uint32 *offline_mask, uint32 *online_mask);

uint8 __MTLK_IFUNC  wave_radio_max_tx_antennas_get(wave_radio_t *radio);
uint8 __MTLK_IFUNC  wave_radio_max_rx_antennas_get(wave_radio_t *radio);
uint8 __MTLK_IFUNC  wave_radio_tx_antenna_mask_get(wave_radio_t *radio);
uint8 __MTLK_IFUNC  wave_radio_rx_antenna_mask_get(wave_radio_t *radio);

void  __MTLK_IFUNC  wave_radio_antenna_cfg_update(wave_radio_t *radio, uint8 mask);

void  __MTLK_IFUNC  wave_radio_current_antenna_mask_reset(wave_radio_t *radio);
void  __MTLK_IFUNC  wave_radio_current_antenna_mask_set(wave_radio_t *radio, uint8 mask);
uint8 __MTLK_IFUNC  wave_radio_current_antenna_mask_get(wave_radio_t *radio);

void __MTLK_IFUNC
wave_radio_ant_masks_num_sts_get (wave_radio_t *radio, u32 *tx_ant_mask, u32 *rx_ant_mask, u32 *num_sts);

wv_cfg80211_t * __MTLK_IFUNC wave_radio_cfg80211_get(wave_radio_t *radio);
int __MTLK_IFUNC wave_radio_country_code_set(wave_radio_t *radio, const mtlk_country_code_t *country_code);
int __MTLK_IFUNC wave_radio_country_code_set_by_str(wave_radio_t *radio, char *str, size_t len);

BOOL __MTLK_IFUNC wave_radio_is_init_done(wave_radio_t *radio);

void __MTLK_IFUNC
wave_radio_total_traffic_delta_set (wave_radio_t *radio, uint32 total_traffic_delta);

uint32 __MTLK_IFUNC
wave_radio_airtime_efficiency_get (wave_radio_t *radio);

void __MTLK_IFUNC
wave_radio_phy_status_update (wave_radio_t *radio, wave_radio_phy_stat_t *params);

void __MTLK_IFUNC
wave_radio_phy_status_get (wave_radio_t *radio, wave_radio_phy_stat_t *params);

uint8 __MTLK_IFUNC
wave_radio_channel_load_get (wave_radio_t *radio);

void __MTLK_IFUNC
wave_radio_get_tr181_hw_stats (wave_radio_t *radio, mtlk_wssa_drv_tr181_hw_stats_t *stats);

void __MTLK_IFUNC
wave_radio_get_hw_stats (wave_radio_t *radio, mtlk_wssa_drv_hw_stats_t *stats);

void __MTLK_IFUNC wave_radio_sync_hostapd_done_set(wave_radio_t *radio, BOOL value);
BOOL __MTLK_IFUNC wave_radio_is_sync_hostapd_done(wave_radio_t *radio);
int __MTLK_IFUNC wave_radio_get_num_of_channels (mtlk_hw_band_e hw_band, int* num_of_channels);
wave_ant_params_t *wave_radio_get_ant_params (wave_radio_t *radio);
int __MTLK_IFUNC
wave_radio_cdb_antenna_cfg_update (wave_radio_t *radio, uint8 ant_mask);
int __MTLK_IFUNC
wave_radio_get_zwdfs_ant_enabled (wave_radio_t *radio);
void __MTLK_IFUNC
wave_radio_set_zwdfs_ant_enabled (wave_radio_t *radio, const int enable);
BOOL __MTLK_IFUNC wave_radio_get_is_zwdfs_radio (wave_radio_t *radio);
BOOL __MTLK_IFUNC wave_radio_get_is_zwdfs_operational_radio (wave_radio_t *radio);
wave_radio_t * __MTLK_IFUNC wave_radio_descr_get_zwdfs_radio (wave_radio_descr_t *radio_descr);
BOOL __MTLK_IFUNC wave_radio_is_5g_in_zwdfs_cdb_cfg (wave_radio_t *radio);

int  __MTLK_IFUNC
wave_radio_set_cts_to_self_to (wave_radio_t *radio, const int cts_to_self_to);
int __MTLK_IFUNC wave_radio_get_cts_to_self_to (wave_radio_t *radio);

void __MTLK_IFUNC
wave_radio_set_fils_beacon_flag (wave_radio_t *radio, const int fils_beacon_flag);
int __MTLK_IFUNC wave_radio_get_fils_beacon_flag (wave_radio_t *radio);

int __MTLK_IFUNC
wave_radio_set_tx_ampdu_density (wave_radio_t *radio, const int ampdu_density);
uint32 __MTLK_IFUNC wave_radio_get_tx_ampdu_density (wave_radio_t *radio);
uint32 __MTLK_IFUNC  wave_radio_max_tx_ampdu_density_get(wave_radio_t *radio, uint32 sta_ampdu_density);
void __MTLK_IFUNC wave_radio_on_rcvry_isol(wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_on_rcvry_restore(wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_bss_tx_timer_deinit(wave_radio_descr_t *radio_descr);
int __MTLK_IFUNC wave_radio_bss_tx_timer_init(wave_radio_descr_t *radio_descr);

void __MTLK_IFUNC wave_radio_set_cac_pending(wave_radio_t *radio, BOOL state);
BOOL __MTLK_IFUNC wave_radio_get_cac_pending(wave_radio_t *radio);

void __MTLK_IFUNC wave_radio_set_unsolicited_frame_tx(wave_radio_t *radio, struct net_device *ndev, const void *data, int data_len);

void __MTLK_IFUNC wave_radio_roc_in_progress_set(wave_radio_t *radio, unsigned val);
unsigned __MTLK_IFUNC wave_radio_roc_in_progress_get(wave_radio_t *radio);

void __MTLK_IFUNC wave_radio_clean_probe_req_list(wave_radio_t *radio, BOOL flush_probe_list);
void __MTLK_IFUNC wave_probe_req_list_add(wave_radio_t *radio, const IEEE_ADDR *addr, int8 rssi);

int __MTLK_IFUNC wave_radio_get_probe_req_list (struct wiphy *wiphy, struct net_device *ndev, uint8 isflush);
int __MTLK_IFUNC
wave_radio_copy_probe_req_list(wave_radio_t *radio, probe_req_info *info);

int __MTLK_IFUNC wave_radio_preamble_punct_cca_ov_cfg_get (wave_radio_t *radio, UMI_PREAMBLE_PUNCT_CCA_OVERRIDE *req);

BOOL __MTLK_IFUNC wave_radio_was_scan_done_since_prev_sta_poll (wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_set_scan_done_since_prev_sta_poll (wave_radio_t *radio, BOOL value);
void __MTLK_IFUNC mtlk_radio_wss_reset_stat (wave_radio_t *radio);
int wave_reset_statistics (struct wiphy *wiphy, struct net_device *ndev, struct intel_vendor_reset_statistics *stats);
mtlk_error_t wave_radio_get_csi_enable_params (struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
mtlk_error_t wave_radio_get_csi_auto_params (struct wiphy *wiphy, struct net_device*ndev, const uint8 *addr);
mtlk_error_t wave_radio_get_CsiStats (struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
mtlk_error_t wave_radio_get_CsiCounters (struct wiphy *wiphy, struct net_device *ndev, const uint8 *addr);
mtlk_error_t wave_radio_get_csi_capability (struct wiphy *wiphy, struct net_device *ndev);
mtlk_error_t wave_radio_get_mu_group_cfg (struct wiphy *wiphy, struct net_device *ndev, uint32 *formation_type);
mtlk_error_t wave_radio_get_la_mu_groups_counters_stats (struct wiphy *wiphy, struct net_device *ndev);
#ifdef MTLK_WAVE_700
BOOL __MTLK_IFUNC wave_radio_is_gen7 (wave_radio_t *radio);
BOOL __MTLK_IFUNC wave_radio_is_320mhz_supported (wave_radio_t *radio);
uint16 __MTLK_IFUNC wave_radio_get_high_speed_counter (wave_radio_t *radio);
uint16 __MTLK_IFUNC wave_radio_inc_high_speed_counter (wave_radio_t *radio);
uint16 __MTLK_IFUNC wave_radio_dec_high_speed_counter (wave_radio_t *radio);
uint8 __MTLK_IFUNC wave_radio_get_rx_ant (wave_radio_t *radio);
uint8 __MTLK_IFUNC wave_convert_radio_band_to_link_id (mtlk_hw_band_e band);
#endif
BOOL __MTLK_IFUNC wave_radio_is_zwdfs_gen6_d2 (wave_radio_t *radio);
mtlk_error_t wave_radio_get_su_mu_ru_stats (struct wiphy *wiphy, struct net_device *ndev, wave_vendor_su_mu_ru_stats_t *data);
struct intel_vendor_channel_data * __MTLK_IFUNC wave_radio_get_aocs_data(wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_init_aocs_scan_data(wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_copy_chan_stats(wave_radio_t *radio, devicePhyRxStatusDb_t *stats_buff);
#ifdef MTLK_WAVE_700
mtlk_txmm_t *wave_radio_txmm_get (wave_radio_t *radio);
BOOL __MTLK_IFUNC wave_radio_is_gen7 (wave_radio_t *radio);
BOOL __MTLK_IFUNC wave_radio_is_320mhz_supported (wave_radio_t *radio);
#endif
void __MTLK_IFUNC
wave_hw_get_radio_ul_ofdma_stats_64 (wave_radio_t *radio, wave_radio_pdus_ofdma_stats64_t **stats);
void __MTLK_IFUNC
wave_hw_get_radio_dl_ofdma_stats_64 (wave_radio_t *radio, wave_radio_pdus_ofdma_stats64_t **stats);
void __MTLK_IFUNC
wave_hw_get_radio_ul_ofdma_stats (wave_radio_t *radio, wave_radio_pdus_ofdma_stats_t **stats);
void __MTLK_IFUNC
wave_hw_get_radio_dl_ofdma_stats (wave_radio_t *radio, wave_radio_pdus_ofdma_stats_t **stats);
void __MTLK_IFUNC
wave_radio_reset_ofdma_stats(wave_radio_t *radio);
void __MTLK_IFUNC
wv_cfg80211_radio_ch_switch_notify (wave_radio_t *radio, struct mtlk_chan_def *ccd);
mtlk_error_t __MTLK_IFUNC
wave_radio_get_radio_peer_list (struct wiphy *wiphy, struct net_device *ndev);
void __MTLK_IFUNC
wave_get_radio_dl_mimo_stats_64 (wave_radio_t *radio, wave_radio_mimo_stats64_t **stats);
void __MTLK_IFUNC
wave_get_radio_dl_mimo_stats (wave_radio_t *radio, wave_radio_mimo_stats_t **stats);
mtlk_error_t __MTLK_IFUNC
wave_radio_get_mimo_stats(struct wiphy *wiphy, struct net_device *ndev, wave_vendor_su_mu_ru_stats_t *data);
BOOL __MTLK_IFUNC wave_radio_get_is_zwdfs_background_supp (wave_radio_t *radio);
struct mtlk_chan_def* wave_radio_zwdfs_chandef_get(wave_radio_t *radio);
mtlk_hw_t * __MTLK_IFUNC wave_radio_get_hw (wave_radio_t *radio);
void __MTLK_IFUNC wave_radio_set_thermal_mgmt_data (wave_radio_t *radio, wave_thermal_t *thermal_mgmt);
wave_thermal_t* __MTLK_IFUNC wave_radio_get_thermal_mgmt_data (wave_radio_t *radio);
struct ieee80211_regdomain * __MTLK_IFUNC wave_copy_regd(const struct ieee80211_regdomain *src_regd);
void __MTLK_IFUNC wv_set_regd_flag(struct ieee80211_regdomain *regd, u32 rrule_flags);
#endif
