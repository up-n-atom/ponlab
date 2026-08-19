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
 * Radio module functionality
 *
 */

#include "mtlkinc.h"
#include <net/cfg80211.h>
#include <net/mac80211.h>
#ifndef CPTCFG_IWLWAV_X86_HOST_PC
#include <../net/wireless/core.h>
#include <../net/wireless/nl80211.h>
#endif

#include "mtlk_coreui.h"
#include "mtlk_mmb_drv.h"
#include "mtlkdfdefs.h"
#include "eeprom.h"
#include "mtlk_psdb.h"
#include "cfg80211.h"
#include "mac80211.h"
#include "hw_mmb.h"
#include "mtlkwlanirbdefs.h"
#include "vendor_cmds.h"
#include "mtlk_wss_debug.h"
#include "mtlk_dfg.h"
#include "mtlk_df_user_priv.h"
#include "mtlk_df_priv.h"
#include "mtlk_df.h"
#include "mtlk_packets.h"
#include "mtlk_df_nbuf.h"
#include "mtlk_param_db.h"
#include "scan_support.h"
#include "wave_radio.h"
#include "mhi_umi.h"
#include "core_config.h"
#include "wave_80211ax.h"
#include "wave_hal_stats.h"
#include "core_stats.h"
#include "ieee80211_crc32.h" /* for IEEE80211 32-bit CRC computation */


#define LOG_LOCAL_GID   GID_WAVE_RADIO
#define LOG_LOCAL_FID   0


#define STRING_PROGMODEL_SIZE   128

/* channels calibration definitions */

typedef enum {
  WAVE_RADIO_CHAN_G24_20,
  WAVE_RADIO_CHAN_G24_40,
  WAVE_RADIO_CHAN_G52_20,
  WAVE_RADIO_CHAN_G52_40,
  WAVE_RADIO_CHAN_G52_80,
  WAVE_RADIO_CHAN_G52_160,

/*for the 6GHz band support */
  WAVE_RADIO_CHAN_G6_20,
  WAVE_RADIO_CHAN_G6_40,
  WAVE_RADIO_CHAN_G6_80,
#ifdef MTLK_WAVE_700
  WAVE_RADIO_CHAN_G6_160,
  WAVE_RADIO_CHAN_G7_320
#else
 WAVE_RADIO_CHAN_G6_160
#endif
} wave_radio_chan_type_t;

#define G24_20_CH_NUM_MAX                           14
#define G24_40_CH_NUM_MAX                            9
#define G52_20_CH_NUM_MAX                           25
#define G52_40_CH_NUM_MAX       (G52_20_CH_NUM_MAX / 2)
#define G52_80_CH_NUM_MAX       (G52_40_CH_NUM_MAX / 2)
#define G52_160_CH_NUM_MAX  (G52_80_CH_NUM_MAX / 2 - 1)

/* for the 6GHz band support */
#define G6_20_CH_NUM_MAX       (NUM_TOTAL_CHANS_6G)    /* No. of 20MHz channels in U-NII 5,6,7,8 6GHz bands */
#define G6_40_CH_NUM_MAX       (G6_20_CH_NUM_MAX >> 1)
#define G6_80_CH_NUM_MAX       (G6_40_CH_NUM_MAX >> 1)
#define G6_160_CH_NUM_MAX      (G6_80_CH_NUM_MAX >> 1)
#ifdef MTLK_WAVE_700
#define G7_320_CH_NUM_MAX      (2 * (G6_160_CH_NUM_MAX >> 1)) /* 802.11BE: 320 MHz-1 and 320 MHz-2 channels */
#endif
#define WAVE_MASTER_PROC_NAME "master_vap_name"

#ifdef MTLK_WAVE_700
/* for GEN7 */
#define WLAN_ID_2G     0
#define WLAN_ID_5G     2
#define WLAN_ID_6G     4
#define WLAN_ID_UNDEF  0xF
#endif

#define ZWDFS_CDB_CFG(_radio) \
((wave_hw_radio_band_cfg_get(_radio->hw_api->hw) == WAVE_HW_RADIO_BAND_CFG_SCAN) && \
 (mtlk_is_band_supported((mtlk_handle_t)wave_radio_get_hw(_radio), wave_radio_id_get(_radio), MTLK_HW_BAND_5_2_GHZ)))

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G24_20_CH_NUM_MAX];
} g24_20_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G24_40_CH_NUM_MAX];
} g24_40_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G52_20_CH_NUM_MAX];
} g52_20_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G52_40_CH_NUM_MAX];
} g52_40_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G52_80_CH_NUM_MAX];
} g52_80_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G52_160_CH_NUM_MAX];
} g52_160_ch_tab_t;

/*for 6GHz band support */
typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G6_20_CH_NUM_MAX];
} g6_20_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G6_40_CH_NUM_MAX];
} g6_40_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G6_80_CH_NUM_MAX];
} g6_80_ch_tab_t;

typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G6_160_CH_NUM_MAX];
} g6_160_ch_tab_t;

#ifdef MTLK_WAVE_700
typedef struct {
  uint8 num_of_chan;
  uint8 ch_width;
  uint8 channel[G7_320_CH_NUM_MAX];
} g7_320_ch_tab_t;
#endif

static const uint8 g24_20_supported_ch[G24_20_CH_NUM_MAX] = {
  1,  2,  3,  4,  5,  6,  7,
  8,  9, 10, 11, 12, 13, 14
};

static const uint8 g24_40_supported_ch[G24_40_CH_NUM_MAX] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9
};

static const uint8 g52_20_supported_ch[G52_20_CH_NUM_MAX] = {
   36,  40,  44,  48,  52,
   56,  60,  64, 100, 104,
  108, 112, 116, 120, 124,
  128, 132, 136, 140, 144,
  149, 153, 157, 161, 165
};

static const uint8 g52_40_supported_ch[G52_40_CH_NUM_MAX] = {
  36, 44, 52, 60, 100, 108, 116, 124, 132, 140, 149, 157
};

static const uint8 g52_80_supported_ch[G52_80_CH_NUM_MAX] = {
  36, 52, 100, 116, 132, 149
};

static const uint8 g52_160_supported_ch[G52_160_CH_NUM_MAX] = {
  36, 100
};

/* for the 6GHz band support */

/* 59 20MHz channels */
static const uint8 g6_20_supported_ch[G6_20_CH_NUM_MAX] = {
   1,   5,   9,   13,  17,  21,  25,  29,  33,  37,
  41,  45,  49,   53,  57,  61,  65,  69,  73,  77,
  81,  85,  89,   93,  97, 101, 105, 109, 113, 117,
 121, 125, 129,  133, 137, 141, 145, 149, 153, 157,
 161, 165, 169,  173, 177, 181, 185, 189, 193, 197,
 201, 205, 209,  213, 217, 221, 225, 229, 233
};

/* 29 40MHz channels */
static const uint8 g6_40_supported_ch[G6_40_CH_NUM_MAX] = {
   1,   9,  17,  25,  33,   41,  49,  57,  65,  73,
  81,  89,  97, 105, 113,  121, 129, 137, 145, 153,
 161, 169, 177, 185, 193,  201, 209, 217, 225
};

/* 14 80MHz channels */
static const uint8 g6_80_supported_ch[G6_80_CH_NUM_MAX] = {
   1,  17,  33,  49,  65,   81,  97,
 113, 129, 145, 161, 177,  193, 209
};

/* 7 160MHz channels */
static const uint8 g6_160_supported_ch[G6_160_CH_NUM_MAX] = {
   1,  33,  65,  97, 129,  161, 193
};

#ifdef MTLK_WAVE_700
/* 6 320MHz channels: 320-1 and 320-2 */
static const uint8 g7_320_supported_ch[G7_320_CH_NUM_MAX] = {
   1,  33,  65,  97, 129,  161
};
#endif

/**************************************************************
    WSS counters for HW Radio statistics
 */
static const uint32 _wave_radio_wss_id_map[] =
{
  MTLK_WWSS_WLAN_STAT_ID_BYTES_SENT_64,                  /* WAVE_RADIO_CNT_BYTES_SENT */
  MTLK_WWSS_WLAN_STAT_ID_BYTES_RECEIVED_64,              /* WAVE_RADIO_CNT_BYTES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_PACKETS_SENT_64,                /* WAVE_RADIO_CNT_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_PACKETS_RECEIVED_64,            /* WAVE_RADIO_CNT_PACKETS_RECEIVED */

  MTLK_WWSS_WLAN_STAT_ID_UNICAST_PACKETS_SENT,        /* WAVE_RADIO_CNT_UNICAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_MULTICAST_PACKETS_SENT,      /* WAVE_RADIO_CNT_MULTICAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_BROADCAST_PACKETS_SENT,      /* WAVE_RADIO_CNT_BROADCAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_UNICAST_BYTES_SENT,          /* WAVE_RADIO_CNT_UNICAST_BYTES_SENT */
  MTLK_WWSS_WLAN_STAT_ID_MULTICAST_BYTES_SENT,        /* WAVE_RADIO_CNT_MULTICAST_BYTES_SENT */
  MTLK_WWSS_WLAN_STAT_ID_BROADCAST_BYTES_SENT,        /* WAVE_RADIO_CNT_BROADCAST_BYTES_SENT */

  MTLK_WWSS_WLAN_STAT_ID_UNICAST_PACKETS_RECEIVED,    /* WAVE_RADIO_CNT_UNICAST_PACKETS_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_MULTICAST_PACKETS_RECEIVED,  /* WAVE_RADIO_CNT_MULTICAST_PACKETS_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_BROADCAST_PACKETS_RECEIVED,  /* WAVE_RADIO_CNT_BROADCAST_PACKETS_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_UNICAST_BYTES_RECEIVED,      /* WAVE_RADIO_CNT_UNICAST_BYTES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_MULTICAST_BYTES_RECEIVED,    /* WAVE_RADIO_CNT_MULTICAST_BYTES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_BROADCAST_BYTES_RECEIVED,    /* WAVE_RADIO_CNT_BROADCAST_BYTES_RECEIVED */

  MTLK_WWSS_WLAN_STAT_ID_ERROR_PACKETS_SENT,          /* WAVE_RADIO_CNT_ERROR_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_ERROR_PACKETS_RECEIVED,      /* WAVE_RADIO_CNT_ERROR_PACKETS_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_DISCARD_PACKETS_RECEIVED,    /* WAVE_RADIO_CNT_DISCARD_PACKETS_RECEIVED */

  MTLK_WWSS_WLAN_STAT_ID_RX_PACKETS_DISCARDED_DRV_DUPLICATE,/* WAVE_RADIO_CNT_RX_PACKETS_DISCARDED_DRV_DUPLICATE */

  MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_RECEIVED,      /* WAVE_RADIO_CNT_802_1X_PACKETS_RECEIVED                                         */
  MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_SENT,          /* WAVE_RADIO_CNT_802_1X_PACKETS_SENT                                             */
  MTLK_WWSS_WLAN_STAT_ID_802_1X_PACKETS_DISCARDED,     /* WAVE_RADIO_CNT_802_1X_PACKETS_DISCARDED */
  MTLK_WWSS_WLAN_STAT_ID_PAIRWISE_MIC_FAILURE_PACKETS, /* WAVE_RADIO_CNT_PAIRWISE_MIC_FAILURE_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_GROUP_MIC_FAILURE_PACKETS,    /* WAVE_RADIO_CNT_GROUP_MIC_FAILURE_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_UNICAST_REPLAYED_PACKETS,     /* WAVE_RADIO_CNT_UNICAST_REPLAYED_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_MULTICAST_REPLAYED_PACKETS,   /* WAVE_RADIO_CNT_MULTICAST_REPLAYED_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_MANAGEMENT_REPLAYED_PACKETS,  /* WAVE_RADIO_CNT_MANAGEMENT_REPLAYED_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_FWD_RX_PACKETS,               /* WAVE_RADIO_CNT_FWD_RX_PACKETS */
  MTLK_WWSS_WLAN_STAT_ID_FWD_RX_BYTES,                 /* WAVE_RADIO_CNT_FWD_RX_BYTES */

  MTLK_WWSS_WLAN_STAT_ID_DAT_FRAMES_RECEIVED,          /* WAVE_RADIO_CNT_DAT_FRAMES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_CTL_FRAMES_RECEIVED,          /* WAVE_RADIO_CNT_CTL_FRAMES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_MAN_FRAMES_RES_QUEUE,         /* WAVE_RADIO_CNT_MAN_FRAMES_RES_QUEUE */
  MTLK_WWSS_WLAN_STAT_ID_MAN_FRAMES_SENT,              /* WAVE_RADIO_CNT_MAN_FRAMES_SENT */
  MTLK_WWSS_WLAN_STAT_ID_MAN_FRAMES_CONFIRMED,         /* WAVE_RADIO_CNT_MAN_FRAMES_CONFIRMED */
  MTLK_WWSS_WLAN_STAT_ID_MAN_FRAMES_RECEIVED,          /* WAVE_RADIO_CNT_MAN_FRAMES_RECEIVED */
  MTLK_WWSS_WLAN_STAT_ID_RX_MAN_FRAMES_RETRY_DROPPED,  /* WAVE_RADIO_CNT_RX_MAN_FRAMES_RETRY_DROPPED */
  MTLK_WWSS_WLAN_STAT_ID_MAN_FRAMES_FAILED,            /* WAVE_RADIO_CNT_MAN_FRAMES_FAILED */
  MTLK_WWSS_WLAN_STAT_ID_TX_PROBE_RESP_SENT,           /* WAVE_RADIO_CNT_TX_PROBE_RESP_SENT */
  MTLK_WWSS_WLAN_STAT_ID_TX_PROBE_RESP_DROPPED,        /* WAVE_RADIO_CNT_TX_PROBE_RESP_DROPPED */
  MTLK_WWSS_WLAN_STAT_ID_BSS_MGMT_TX_QUE_FULL,         /* WAVE_RADIO_CNT_BSS_MGMT_TX_QUE_FULL */
  MTLK_WWSS_WLAN_STAT_ID_PD_BROADCAST_PACKETS_SENT,    /* WAVE_RADIO_CNT_PD_BROADCAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_PD_BROADCAST_BYTES_SENT_64,   /* WAVE_RADIO_CNT_PD_BROADCAST_BYTES_SENT_64 */
  MTLK_WWSS_WLAN_STAT_ID_PD_UNICAST_PACKETS_SENT,      /* WAVE_RADIO_CNT_PD_UNICAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_PD_UNICAST_BYTES_SENT_64,     /* WAVE_RADIO_CNT_PD_UNICAST_BYTES_SENT_64 */
  MTLK_WWSS_WLAN_STAT_ID_PD_MULTICAST_PACKETS_SENT,    /* WAVE_RADIO_CNT_PD_MULTICAST_PACKETS_SENT */
  MTLK_WWSS_WLAN_STAT_ID_PD_MULTICAST_BYTES_SENT_64,   /* WAVE_RADIO_CNT_PD_MULTICAST_BYTES_SENT_64 */
};

/**************************************************************/

typedef struct _ieee_addr_sized_list_t {
  mtlk_osal_spinlock_t      ieee_addr_lock;
  mtlk_hash_t               hash;
  uint16                    size;
  uint8                     update_counter;
  uint8                     print_counter;
} ieee_addr_sized_list_t;

typedef struct _ieee_addr_time_entry_t {
  MTLK_HASH_ENTRY_T(ieee_addr) hentry;
  mtlk_osal_msec_t timestamp;
  int8 probe_rssi;
} ieee_addr_entry_time_t;

struct _wave_radio_t {
  mtlk_eeprom_data_t   *ee_data; /* EEPROM parsed data */
  hw_psdb_t            *psdb;

  wave_ant_params_t     ant_params;
  mtlk_atomic_t         cur_ant_mask;  /* Currently in use antennas mask. Currently same for RX/TX */

  char                  progmodel[STRING_PROGMODEL_SIZE];
  mtlk_vap_manager_t    *vap_manager;
  mtlk_df_proc_fs_node_t *proc_node_radio;
  beacon_manager_priv_t bmgr_priv;
  wave_radio_limits_t   limits;

  wave_radio_phy_stat_t phy_status; /* Radio Phy RX Status */
  mtlk_osal_spinlock_t  phy_status_lock;

  wave_radio_statsPD_t statsPD;
  wave_radio_statsPD_t statsPD_snapshot;

  uint32                total_traffic_delta; /* bytes */
  uint32                airtime_efficiency;  /* bytes/sec */

  wv_cfg80211_t         *cfg80211;
  wv_mac80211_t         *mac80211;
  mtlk_hw_api_t         *hw_api;
  mtlk_pdb_t            *param_db;
  mtlk_coc_t            *coc_mgmt;
  mtlk_erp_t            *erp_mgmt;

  UMI_HDK_CONFIG        current_hdkconfig; /* the currently in effect hdkconfig */
  mtlk_scan_support_t   scan_support;
  struct mtlk_chan_def  current_chandef; /* the channel that's set (even if just for scanning) */
  struct mtlk_chan_def  zwdfs_ant_chandef; /* Current channel for background zwdfs antenna. Applicable in WAV700 */
  int                   chan_switch_type; /* ST_something */
  uint8                 last_pm_freq; /* frequency of the last loaded programming model */
  BOOL                  is_interfdet_enabled; /* Interference Detection */
  BOOL                  init_done;
  atomic_t              sta_cnt;
  BOOL                  progmodel_loaded;
  unsigned              idx;
  mtlk_hw_band_e        wave_band;           /* band in wave format */
  nl80211_band_e        ieee_band;           /* band in cfg80211/nl80211 format */
  /* CDB configuration */
  wave_cdb_cfg_e        cdb_config;

  mtlk_wss_t           *wss;
  mtlk_wss_t           *wss_snapshot;
  mtlk_wss_cntr_handle_t  *wss_hcntrs[WAVE_RADIO_CNT_LAST];

  /* table of channels for the radio */
  g24_20_ch_tab_t       g24_20_ch_tab;
  g24_40_ch_tab_t       g24_40_ch_tab;
  g52_20_ch_tab_t       g52_20_ch_tab;
  g52_40_ch_tab_t       g52_40_ch_tab;
  g52_80_ch_tab_t       g52_80_ch_tab;
  g52_160_ch_tab_t      g52_160_ch_tab;

  /* table of channels for the 6GHz band support */
  g6_20_ch_tab_t        g6_20_ch_tab;
  g6_40_ch_tab_t        g6_40_ch_tab;
  g6_80_ch_tab_t        g6_80_ch_tab;
  g6_160_ch_tab_t       g6_160_ch_tab;
#ifdef MTLK_WAVE_700
  g7_320_ch_tab_t       g7_320_ch_tab;
  uint16                high_speed_stas_cntr; /* number of STA of this band which support 320MHz & have 4 NSS */
#endif
  uint8                 calib_done_mask[1 + NUM_TOTAL_CHANS]; /* 0th entry not used */
  uint8                 calib_done_mask_6g[1 + NUM_TOTAL_CHAN_FREQS_6G]; /* channels: 1,3,5,7,...,93,...,233   *
                                                                   * Index:1,3,5,7,...,117  =>  20MHz channels *
                                                                   * Index:2,6,10,,...,114  =>  40MHz channels *
                                                                   * Index:4,12,20,...,108  =>  80MHz channels *
                                                                   * Index:8,24,40,...,104  => 160MHz channels *
                                                                   * Index:16,32,48,...,96  => 320MHz channels */

  BOOL                  is_sync_hostapd_done; /* indicates that STA DB is synchronous in driver and hostapd */
  BOOL                  cac_pending;
  mtlk_osal_timer_t     bss_tx_timer;
  mtlk_atomic_t         roc_in_progress;
  mtlk_atomic_t         cca_msr_started;
  mtlk_atomic_t         cca_msr_chan;
  int                   cca_stats_chan;
  wave_cca_stats_user_t cca_stats_user;
  wave_cca_stats_t      cca_stats;
  ieee_addr_sized_list_t probe_resp_list;
  BOOL                  scan_was_done_since_prev_poll;

  struct intel_vendor_channel_data aocs_data;
  wave_radio_pdus_ofdma_stats64_t ul_radio_ofdma_stats_64;
  wave_radio_pdus_ofdma_stats64_t dl_radio_ofdma_stats_64;
  wave_radio_mimo_stats64_t       dl_radio_mimo_stats_64;
  /* 32 bit radio OFDMA stats */
  wave_radio_pdus_ofdma_stats_t   ul_radio_ofdma_stats;
  wave_radio_pdus_ofdma_stats_t   dl_radio_ofdma_stats;
  wave_radio_mimo_stats_t         dl_radio_mimo_stats;
#ifdef CONFIG_THERMAL
  /* Thermal mgmt data */
  struct wave_thermal *thermal_mgmt;
#endif
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_START_STATUS;
};

#define WAVE_PROBE_REQ_LIST_MAX_SIZE    500
#define WAVE_PROBE_REQ_LIST_MAX_AGE_MS  300000
#define WAVE_PROBE_REQ_LIST_COUNT       5
#define WAVE_PROBE_REQ_PRINT_COUNT      100
#define WAVE_PROBE_REQ_LIST_HASH_NOF_BUCKETS 16

extern const struct ieee80211_regdomain *get_cfg80211_regdom(void);

/*** Init / deinit ***/
MTLK_INIT_STEPS_LIST_BEGIN(wave_radio)
  MTLK_INIT_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_VAP_MANAGER_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_PHY_STATUS_LOCK)
MTLK_INIT_INNER_STEPS_BEGIN(wave_radio)
MTLK_INIT_STEPS_LIST_END(wave_radio);

MTLK_START_STEPS_LIST_BEGIN(wave_radio)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_PROBE_LIST_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_PDB_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_CONTEXT_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_WSS_CREATE)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_WSS_SNAPSHOT_CREATE)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_IEEE80211_NAME_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_IEEE80211_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_IEEE80211_REGISTER)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_PDB_INIT_FINALIZE)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_BEACON_MAN_PRIV_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_VAP_CREATE)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_SCAN_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_CHANDEF_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_COC_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_ERP_INIT)
  MTLK_START_STEPS_LIST_ENTRY(wave_radio, WAVE_RADIO_PROC_INIT)
MTLK_START_INNER_STEPS_BEGIN(wave_radio)
MTLK_START_STEPS_LIST_END(wave_radio);


static __INLINE uint32
__wave_radio_get_chip_id (wave_radio_t *radio)
{
  return hw_mmb_get_chip_id(radio->hw_api->hw);
}

static __INLINE BOOL
__wave_radio_is_gen6_d2 (wave_radio_t *radio)
{
  return _chipid_is_gen6_d2(__wave_radio_get_chip_id(radio));
}

BOOL __MTLK_IFUNC
wave_radio_is_zwdfs_gen6_d2 (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return (wave_radio_get_is_zwdfs_radio(radio) && __wave_radio_is_gen6_d2(radio));
}

#ifdef WAVE_VAPID_CONVERT_IN_DRV
BOOL __MTLK_IFUNC
wave_radio_is_5g_in_zwdfs_cdb_cfg (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return (ZWDFS_CDB_CFG(radio) && wave_radio_is_first(radio));
}
#endif

void __MTLK_IFUNC wave_radio_clean_probe_req_list(wave_radio_t *radio, BOOL flush_probe_list)
{
  ieee_addr_sized_list_t *list = &radio->probe_resp_list;
  mtlk_hash_enum_t              e;
  ieee_addr_entry_time_t *ieee_addr_entry = NULL;
  mtlk_osal_ms_diff_t timediff;
  mtlk_osal_msec_t current_time;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;

  if ((list->update_counter != WAVE_PROBE_REQ_LIST_COUNT) && (flush_probe_list == FALSE)) {
    list->update_counter++;
    return;
  }
  current_time = mtlk_osal_timestamp_to_ms(mtlk_osal_timestamp());
  mtlk_osal_lock_acquire(&list->ieee_addr_lock);
  list->update_counter = 0;
  h = mtlk_hash_enum_first_ieee_addr(&list->hash, &e);
  while (h) {
    ieee_addr_entry = MTLK_CONTAINER_OF(h, ieee_addr_entry_time_t, hentry);
    timediff = mtlk_osal_ms_time_diff(current_time, ieee_addr_entry->timestamp);
    if ((timediff > WAVE_PROBE_REQ_LIST_MAX_AGE_MS) || (flush_probe_list == TRUE)) {
      mtlk_hash_remove_ieee_addr(&list->hash, &ieee_addr_entry->hentry);
      mtlk_osal_mem_free(ieee_addr_entry);
      list->size--;
    }
    h = mtlk_hash_enum_next_ieee_addr(&list->hash, &e);
  }
  mtlk_osal_lock_release(&list->ieee_addr_lock);
}

void __MTLK_IFUNC wave_probe_req_list_add(wave_radio_t *radio, const IEEE_ADDR *addr, int8 rssi)
{
  ieee_addr_sized_list_t *list = &radio->probe_resp_list;
  ieee_addr_entry_time_t *ieee_addr_entry = NULL;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;

  mtlk_osal_lock_acquire(&list->ieee_addr_lock);

  if (list->size >= WAVE_PROBE_REQ_LIST_MAX_SIZE) {
    /* Should never get here */
    BOOL do_err_print = FALSE;
    if (0 == list->print_counter) {
      do_err_print = TRUE;
      list->print_counter = WAVE_PROBE_REQ_PRINT_COUNT;
    }
    list->print_counter--;
    mtlk_osal_lock_release(&list->ieee_addr_lock);

    if (do_err_print)
      ELOG_DD("RadioID %u: probe req list has reached max size - %d", radio->idx, WAVE_PROBE_REQ_LIST_MAX_SIZE);
    else
      ILOG1_DD("RadioID %u: probe req list has reached max size - %d", radio->idx, WAVE_PROBE_REQ_LIST_MAX_SIZE);

    return;
  }

  h = mtlk_hash_find_ieee_addr(&list->hash, addr);
  if (!h){
    size_t alloc_size = sizeof(ieee_addr_entry_time_t);
    ieee_addr_entry = mtlk_osal_mem_alloc(alloc_size, MTLK_MEM_TAG_IEEE_ADDR_LIST);
    if (!ieee_addr_entry) {
      mtlk_osal_lock_release(&list->ieee_addr_lock);
      WLOG_D("RadioID %u: Can't alloc list entry", radio->idx);
      return;
    }
    memset(ieee_addr_entry, 0, alloc_size);

    ieee_addr_entry->timestamp = mtlk_osal_timestamp_to_ms(mtlk_osal_timestamp());
    ieee_addr_entry->probe_rssi = rssi;
    list->size++;
    mtlk_hash_insert_ieee_addr(&list->hash, addr, &ieee_addr_entry->hentry);
    mtlk_osal_lock_release(&list->ieee_addr_lock);
  }
  else {
    ieee_addr_entry = MTLK_CONTAINER_OF(h, ieee_addr_entry_time_t, hentry);
    ieee_addr_entry->timestamp = mtlk_osal_timestamp_to_ms(mtlk_osal_timestamp());
    ieee_addr_entry->probe_rssi = rssi;
    mtlk_osal_lock_release(&list->ieee_addr_lock);
  }
}

static int _wave_probe_req_list_init(wave_radio_t *radio)
{
  ieee_addr_sized_list_t *list = &radio->probe_resp_list;
  int res;
  list->size = 0;
  list->update_counter = 0;
  list->print_counter = 0;
  res = mtlk_hash_init_ieee_addr(&list->hash, WAVE_PROBE_REQ_LIST_HASH_NOF_BUCKETS);
  if (res != MTLK_ERR_OK)
    return res;
  res = mtlk_osal_lock_init(&list->ieee_addr_lock);
  if (res != MTLK_ERR_OK) {
    mtlk_hash_cleanup_ieee_addr(&list->hash);
  }
  return res;
}

static void _wave_probe_req_list_destroy(wave_radio_t *radio)
{
  ieee_addr_sized_list_t *list = &radio->probe_resp_list;
  mtlk_hash_enum_t              e;
  ieee_addr_entry_time_t *ieee_addr_entry = NULL;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;
  h = mtlk_hash_enum_first_ieee_addr(&list->hash, &e);
  while (h) {
    ieee_addr_entry = MTLK_CONTAINER_OF(h, ieee_addr_entry_time_t, hentry);
    mtlk_hash_remove_ieee_addr(&list->hash, &ieee_addr_entry->hentry);
    mtlk_osal_mem_free(ieee_addr_entry);
    h = mtlk_hash_enum_next_ieee_addr(&list->hash, &e);
  }
  mtlk_hash_cleanup_ieee_addr(&list->hash);
  mtlk_osal_lock_cleanup(&list->ieee_addr_lock);
  list->size = 0;
  list->update_counter = 0;
  list->print_counter = 0;
}

static void _wave_radio_destroy(wave_radio_t *radio)
{
  MTLK_CLEANUP_BEGIN(wave_radio, MTLK_OBJ_PTR(radio))
    MTLK_CLEANUP_STEP(wave_radio, WAVE_RADIO_PHY_STATUS_LOCK, MTLK_OBJ_PTR(radio),
                      mtlk_osal_lock_cleanup, (&radio->phy_status_lock));
    MTLK_CLEANUP_STEP(wave_radio, WAVE_RADIO_VAP_MANAGER_CREATE, MTLK_OBJ_PTR(radio),
                      mtlk_vap_manager_delete, (radio->vap_manager));
  MTLK_CLEANUP_END(wave_radio, MTLK_OBJ_PTR(radio));
}

void wave_radio_destroy(wave_radio_descr_t *radio_descr)
{
  unsigned i;

  for (i = 0; i < radio_descr->num_radios; i++)
    _wave_radio_destroy(&radio_descr->radio[i]);

  mtlk_osal_mem_free(radio_descr->radio);
  mtlk_osal_mem_free(radio_descr);
  radio_descr = NULL;
}

static int _wave_radio_create(wave_radio_t *radio, mtlk_work_mode_e work_mode)
{
  MTLK_INIT_TRY(wave_radio, MTLK_OBJ_PTR(radio));
    MTLK_INIT_STEP_EX(wave_radio, WAVE_RADIO_VAP_MANAGER_CREATE, MTLK_OBJ_PTR(radio),
                      mtlk_vap_manager_create, (radio, radio->idx, radio->hw_api, work_mode),
                      radio->vap_manager, NULL != radio->vap_manager,
                      MTLK_ERR_UNKNOWN);
    MTLK_INIT_STEP(wave_radio, WAVE_RADIO_PHY_STATUS_LOCK, MTLK_OBJ_PTR(radio),
                      mtlk_osal_lock_init, (&radio->phy_status_lock));
  MTLK_INIT_FINALLY(wave_radio, MTLK_OBJ_PTR(radio));
  MTLK_INIT_RETURN(wave_radio, MTLK_OBJ_PTR(radio), _wave_radio_destroy, (radio));
}

wave_radio_descr_t* wave_radio_create (unsigned radio_num, mtlk_hw_api_t *hw_api,
                                       mtlk_work_mode_e work_mode)
{
  int res = MTLK_ERR_OK;
  unsigned i;
  wave_radio_t *radio = NULL;
  wave_radio_descr_t *radio_descr = NULL;

  ILOG1_D("Number of radios: %d", radio_num);

  if (!wave_card_num_of_radios_is_valid(radio_num)) {
    ELOG_D("Wrong number of radios: %u", radio_num);
    return NULL;
  }

  radio_descr = mtlk_osal_mem_alloc(sizeof(wave_radio_descr_t), MTLK_MEM_TAG_USER_RADIO);
  if (NULL == radio_descr) {
    goto wave_radio_create_error;
  }

  radio = mtlk_osal_mem_alloc(sizeof(wave_radio_t) * radio_num, MTLK_MEM_TAG_USER_RADIO);
  if (NULL == radio) {
    goto wave_radio_create_error;
  }

  memset(radio, 0, sizeof(wave_radio_t) * radio_num);
  memset(radio_descr, 0, sizeof(wave_radio_descr_t));

  radio_descr->radio = radio;
  radio_descr->num_radios = radio_num;

  for(i = 0; i < radio_num; i++) {
    radio_descr->radio[i].hw_api = hw_api;
    radio_descr->radio[i].idx = i;
    res = _wave_radio_create(&radio_descr->radio[i], work_mode);
    if (res != MTLK_ERR_OK) {
      /* delete radios that were created successfully */
      while (i--) {
        _wave_radio_destroy(&radio_descr->radio[i]);
      }
      goto wave_radio_create_error;
    }
  }

  return radio_descr;

wave_radio_create_error:
  if (radio)
    mtlk_osal_mem_free(radio);
  if (radio_descr)
    mtlk_osal_mem_free(radio_descr);

  return NULL;
}

#ifdef MTLK_WAVE_700
static BOOL __INLINE
wave_band_is_valid(mtlk_hw_band_e band)
{
  return ((MTLK_HW_BAND_2_4_GHZ == band) ||
          (MTLK_HW_BAND_5_2_GHZ == band) ||
          (MTLK_HW_BAND_6_GHZ == band));
}

int32 static __INLINE _wave_drv_get_wlan_id_gen7(mtlk_hw_band_e band)
{
  static const int32 band2wlan_id[MTLK_HW_BAND_MAX] = {WLAN_ID_5G, WLAN_ID_2G, WLAN_ID_6G};
  return (wave_band_is_valid(band) ? band2wlan_id[band] : WLAN_ID_UNDEF);
}

BOOL static __INLINE _wave_drv_is_band_valid(mtlk_hw_band_e band)
{
  return ((MTLK_HW_BAND_5_2_GHZ <= band) && (band <= MTLK_HW_BAND_6_GHZ));
}
#endif

static int _wave_radio_master_vap_create(wave_radio_t *radio, mtlk_work_role_e role)
{
  int res = MTLK_ERR_OK;
  uint32 _vap_index;
  int32 wlan_id;
  char ndev_name[IFNAMSIZ];

  /* re-use existing master VAP index if such exists */
  _vap_index = mtlk_vap_manager_get_master_vap_index(radio->vap_manager);
  if (_vap_index == MTLK_VAP_INVALID_IDX) {
    _vap_index = wave_radio_master_vap_id_get(radio);
    res = mtlk_vap_manager_check_free_vap_index(radio->vap_manager, _vap_index);
    if (MTLK_ERR_OK != res) {
      ELOG_V("No free slot for master VAP");
      return res;
    }
  }
#ifdef MTLK_WAVE_700
  if (wave_radio_is_gen7(radio))
  {
    mtlk_hw_band_e band = wave_radio_band_get(radio);
    wlan_id = _wave_drv_get_wlan_id_gen7(band);
    if (wlan_id == WLAN_ID_UNDEF) {
      ELOG_D("Unexpected band (%d). Master VAP not created!", band);
      return MTLK_ERR_UNKNOWN;
    }
  }
  else
#endif
  {
    mtlk_hw_band_e band = wave_radio_band_get(radio);
    switch (band) {
      case MTLK_HW_BAND_2_4_GHZ: 
        wlan_id = interface_index[phy_index[PHY_INDEX_CONFIG_2GHZ]];
        break;
      case MTLK_HW_BAND_5_2_GHZ: 
        if (wave_radio_get_is_zwdfs_radio(radio)) {
          wlan_id = interface_index[phy_index[PHY_INDEX_CONFIG_ZWDFS]];
        } else {
          wlan_id = interface_index[phy_index[PHY_INDEX_CONFIG_5GHZ]];
        }
        break;
      case MTLK_HW_BAND_6_GHZ: 
        wlan_id = interface_index[phy_index[PHY_INDEX_CONFIG_6GHZ]];
        break;
      default:
        wlan_id = -1;
    }
    if (wlan_id == -1) {
      ELOG_D("Unexpected band (%d). Master VAP not created!", band);
      return MTLK_ERR_UNKNOWN;
    }
  }

  mtlk_snprintf(ndev_name, sizeof(ndev_name), MTLK_NDEV_NAME "%d", wlan_id);
  ILOG0_SDD("ndev_name %s, VapID %u, band %d", ndev_name, _vap_index, wave_radio_band_get(radio));

  rtnl_lock();
  res = mtlk_vap_manager_create_vap(radio->vap_manager,
                                    _vap_index,
                                    ndev_name,
                                    role,
                                    true,
                                    NULL); /* ndev will be allocated when we call ieee80211_if_alloc */
  rtnl_unlock();
  return res;
}

static int _wave_radio_pdb_init_finalize (wave_radio_t *radio)
{
  static const UMI_SET_TXOP_CONFIG wave_radio_initial_txop_cfg_5ghz_6ghz = {WAVE_TXOP_CFG_STA_ID_DEFAULT,
                                                                           WAVE_TXOP_CFG_TXOP_DURATION_DEFAULT_5GHZ_6GHZ,
                                                                           WAVE_TXOP_CFG_MAX_STAS_NUM_DEFAULT,
                                                                           WAVE_TXOP_CFG_MODE_DEFAULT,
                                                                           0, 0, {0,0,0}};
  int res = MTLK_ERR_OK;
  enum mtlk_hw_band band = wave_radio_band_get(radio);

  /* Note: 2.4GHz default value has been set by wave_pdb_create().
           And we have to initialize 5.2GHz default value as soon as radio band is initialized */
  if ((MTLK_HW_BAND_5_2_GHZ == band) || (MTLK_HW_BAND_6_GHZ == band)) {
    res = WAVE_RADIO_PDB_SET_BINARY(radio, PARAM_DB_RADIO_TXOP_CFG,
                                    &wave_radio_initial_txop_cfg_5ghz_6ghz, sizeof(wave_radio_initial_txop_cfg_5ghz_6ghz));
    if (MTLK_ERR_OK != res)
      ELOG_D("Setup TxOP Parameters of 5GHz band in DB failed (res=%d)", res);
  }
  return res;
}

static int _wave_radio_scan_init(wave_radio_t *radio)
{
  mtlk_core_t* master_core = mtlk_vap_manager_get_master_core(radio->vap_manager);
  return scan_support_init(master_core, &radio->scan_support);
}

static int _wave_radio_chandef_init(wave_radio_t *radio)
{
  mtlk_core_t* master_core = mtlk_vap_manager_get_master_core(radio->vap_manager);
  return current_chandef_init(master_core, &radio->current_chandef);
}

static int _wave_radio_coc_init(wave_radio_t *radio)
{
  mtlk_core_t       *master_core = mtlk_vap_manager_get_master_core(radio->vap_manager);
  mtlk_txmm_t       *txmm = mtlk_vap_get_txmm(master_core->vap_handle);
  wave_ant_params_t *ant_params = &radio->ant_params;
  mtlk_coc_cfg_t     coc_cfg;

  coc_cfg.hw_antenna_cfg.num_tx_antennas = ant_params->tx_ant_num;
  coc_cfg.hw_antenna_cfg.num_rx_antennas = ant_params->rx_ant_num;
  coc_cfg.hw_tx_ant_mask                 = ant_params->tx_ant_mask; /* TX */
  coc_cfg.hw_rx_ant_mask                 = ant_params->rx_ant_mask; /* RX */

  /* time intervals in 100 ms units */
  coc_cfg.default_auto_cfg.starting_time = 1500;  /* 2.5min */
  coc_cfg.default_auto_cfg.interval_1x1  = 10;  /* 1s */

  if (coc_cfg.hw_antenna_cfg.num_tx_antennas == MAX_NUM_TX_ANTENNAS_654) {
    coc_cfg.default_auto_cfg.interval_2x2  = 300; /* 30s skip the 2x2 state by default */
  }
  else {
    coc_cfg.default_auto_cfg.interval_2x2  = 0 ;  /* 0s skip the 2x2 state by default */
  }

  coc_cfg.default_auto_cfg.interval_3x3  = 0;   /* 0s skip the 3x3 state by default */
  coc_cfg.default_auto_cfg.interval_4x4  = 300; /* 30s */
  coc_cfg.default_auto_cfg.transient_time  = 5; /* 0.5s */
  coc_cfg.default_auto_cfg.bandwidth_flag  = COC_BW_SWITCH_DISABLED; /* 0 = BW switch enabled, 1 = BW switch disabled */

  /* limits in Mbps units */
  coc_cfg.default_auto_cfg.high_limit_1x1 = 1;
  coc_cfg.default_auto_cfg.low_limit_2x2  = 1;
  coc_cfg.default_auto_cfg.high_limit_2x2 = 1;
  coc_cfg.default_auto_cfg.low_limit_3x3  = 1;
  coc_cfg.default_auto_cfg.high_limit_3x3 = 1;
  coc_cfg.default_auto_cfg.low_limit_4x4  = 1;

  /* RSSI */
  coc_cfg.default_auto_cfg.low_rssi_2x2   = -66;
  coc_cfg.default_auto_cfg.low_rssi_3x3   = -69;
  coc_cfg.default_auto_cfg.low_rssi_4x4   = -72;
  coc_cfg.default_auto_cfg.rssi_delta_down = -3;  /* 3dbm default value */

  coc_cfg.txmm = txmm;
  coc_cfg.vap_handle = master_core->vap_handle;
  radio->coc_mgmt = mtlk_coc_create(&coc_cfg);
  return (NULL != radio->coc_mgmt) ? MTLK_ERR_OK : MTLK_ERR_UNKNOWN;
}

static int _wave_radio_erp_init(wave_radio_t *radio)
{
  mtlk_core_t* master_core = mtlk_vap_manager_get_master_core(radio->vap_manager);
  mtlk_txmm_t *txmm = mtlk_vap_get_txmm(master_core->vap_handle);
  mtlk_coc_erp_cfg_t  erp_cfg;

  erp_cfg.erp_enabled = 0;
  erp_cfg.initial_wait_time = 20; /* 10 seconds */
  erp_cfg.radio_on_time     = 30;
  erp_cfg.radio_off_time    = 70;
  erp_cfg.max_num_sta       = 2;
  erp_cfg.rx_tp_max         =  400000; /* bytes/sec */
  erp_cfg.tx_20_max_tp      =  400000; /* bytes/sec */
  erp_cfg.tx_40_max_tp      = 1250000; /* bytes/sec */
  erp_cfg.tx_80_max_tp      = 1250000; /* bytes/sec */
  erp_cfg.tx_160_max_tp     = 1250000; /* bytes/sec */
#ifdef MTLK_WAVE_700
  erp_cfg.tx_320_max_tp     = 1250000; /* bytes/sec */
#endif

  erp_cfg.txmm = txmm;
  erp_cfg.vap_handle = master_core->vap_handle;
  radio->erp_mgmt = mtlk_erp_create(&erp_cfg);
  return (NULL != radio->erp_mgmt) ? MTLK_ERR_OK : MTLK_ERR_UNKNOWN;
}

static int _wave_radio_master_vap_proc_entry_read(mtlk_seq_entry_t *s, void *data)
{
  wave_radio_t *radio = mtlk_df_proc_seq_entry_get_df(s);
  mtlk_vap_handle_t vap_handle;
  int res;
  struct net_device *ndev;

  if (!mtlk_vap_manager_get_master_ndev_taken(radio->vap_manager)) {
    mtlk_aux_seq_printf(s, "unregistered");
    return 0;
  }

  res = mtlk_vap_manager_get_master_vap(radio->vap_manager, &vap_handle);
  if (res != MTLK_ERR_OK) {
    return res;
  }

  ndev = mtlk_df_user_get_ndev(mtlk_df_get_user(mtlk_vap_get_df(vap_handle)));
  mtlk_aux_seq_printf(s,"%s", ndev->name);
  return 0;
}

static int _wave_radio_proc_init(wave_radio_t *radio)
{
  struct ieee80211_hw *hw = wave_radio_ieee80211_hw_get(radio);
  int res;
  ILOG1_S("Creating dir %s", wiphy_name(hw->wiphy));
  radio->proc_node_radio = mtlk_df_proc_node_create(wiphy_name(hw->wiphy), mtlk_dfg_get_drv_proc_node());
  if (NULL == radio->proc_node_radio) {
    return MTLK_ERR_NO_MEM;
  }

  res = mtlk_df_proc_node_add_seq_entry(WAVE_MASTER_PROC_NAME, radio->proc_node_radio, radio, _wave_radio_master_vap_proc_entry_read);
  if (res != MTLK_ERR_OK) {
    mtlk_df_proc_node_delete(radio->proc_node_radio);
  }
  return res;
}

static void _wave_radio_coc_delete(wave_radio_t *radio)
{
  return mtlk_coc_delete(radio->coc_mgmt);
}

static void _wave_radio_erp_delete(wave_radio_t *radio)
{
  return mtlk_erp_delete(radio->erp_mgmt);
}

static void _wave_radio_proc_delete(wave_radio_t *radio)
{
  mtlk_df_proc_node_remove_entry(WAVE_MASTER_PROC_NAME, radio->proc_node_radio);
  mtlk_df_proc_node_delete(radio->proc_node_radio);
}

static __INLINE void
__wave_radio_phy_status_avg_reset (wave_radio_phy_stat_t *radio_status);

static int _wave_radio_context_init(wave_radio_t *radio)
{
  iwpriv_cca_th_t cca_th;
  const wave_ant_params_t *psdb_radio_ant_params;
  int res = MTLK_ERR_OK;

  ILOG2_D("Processing RadioID %u", radio->idx);

  /* Reset PhyStat averager */
  __wave_radio_phy_status_avg_reset(&radio->phy_status);

  radio->last_pm_freq = MTLK_HW_BAND_NONE;
  radio->progmodel_loaded = FALSE;
  radio->g24_20_ch_tab.ch_width  = CW_20;
  radio->g24_40_ch_tab.ch_width  = CW_40;
  radio->g52_20_ch_tab.ch_width  = CW_20;
  radio->g52_40_ch_tab.ch_width  = CW_40;
  radio->g52_80_ch_tab.ch_width  = CW_80;
  radio->g52_160_ch_tab.ch_width = CW_160;
  radio->g6_20_ch_tab.ch_width   = CW_20;
  radio->g6_40_ch_tab.ch_width   = CW_40;
  radio->g6_80_ch_tab.ch_width   = CW_80;
  radio->g6_160_ch_tab.ch_width  = CW_160;
#ifdef MTLK_WAVE_700
  if (wave_radio_is_gen7(radio))
    radio->g7_320_ch_tab.ch_width  = CW_320;
#endif

  radio->is_sync_hostapd_done = TRUE;
  radio->cac_pending = FALSE;

  radio->ee_data = NULL;
  (void)mtlk_hw_get_prop(radio->hw_api, MTLK_HW_PROP_EEPROM_DATA, &radio->ee_data, sizeof(typeof(radio->ee_data)));
  MTLK_ASSERT(radio->ee_data != NULL);
  if (radio->ee_data == NULL) {
    ELOG_D("RadioID %u: Error get EEPROM", radio->idx);
    return MTLK_ERR_EEPROM;
  }

  radio->psdb = mtlk_hw_get_psdb(radio->hw_api->hw);
  if(NULL == radio->psdb) {
    ELOG_D("RadioID %u: PSDB is NULL", radio->idx);
    return MTLK_ERR_EEPROM;
  }

  psdb_radio_ant_params = wave_psdb_get_radio_ant_params(radio->psdb, radio->idx);
  MTLK_ASSERT(psdb_radio_ant_params != NULL);
  radio->ant_params = *psdb_radio_ant_params;

  wave_radio_current_antenna_mask_reset(radio);

  radio->cdb_config = wave_psdb_get_radio_cdb_config(radio->psdb, radio->idx);
  ILOG1_DDS("RadioID %u: CBD config %d %s",
           radio->idx, radio->cdb_config, wave_cdb_cfg_to_string(radio->cdb_config));

  /* Update CCA Thresholds configuration if available in PSDB */
  if (MTLK_ERR_OK == wave_psdb_get_radio_cca_th_params(radio->psdb, radio->idx, &cca_th)) {
    res = wave_radio_cca_threshold_set(radio, &cca_th); /* Error already logged */
  }

  atomic_set(&radio->sta_cnt, 0);

#ifdef MTLK_WAVE_700
  radio->high_speed_stas_cntr = 0;
#endif

  return res;
}

static void _wave_radio_vap_delete(wave_radio_t *radio)
{
  rtnl_lock();
  mtlk_vap_manager_deallocate_vaps(radio->vap_manager);
  rtnl_unlock();
}

static void _wave_radio_scan_cleanup(wave_radio_t *radio)
{
  scan_support_cleanup(&radio->scan_support);
}

static void _wave_radio_deinit(wave_radio_t *radio)
{
  MTLK_STOP_BEGIN(wave_radio, MTLK_OBJ_PTR(radio))
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_PROC_INIT, MTLK_OBJ_PTR(radio),
                  _wave_radio_proc_delete, (radio));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_ERP_INIT, MTLK_OBJ_PTR(radio),
                  _wave_radio_erp_delete, (radio));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_COC_INIT, MTLK_OBJ_PTR(radio),
                  _wave_radio_coc_delete, (radio));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_CHANDEF_INIT, MTLK_OBJ_PTR(radio),
                   MTLK_NOACTION, ());
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_SCAN_INIT, MTLK_OBJ_PTR(radio),
                  _wave_radio_scan_cleanup, (radio));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_VAP_CREATE, MTLK_OBJ_PTR(radio),
                  _wave_radio_vap_delete, (radio));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_BEACON_MAN_PRIV_INIT, MTLK_OBJ_PTR(radio),
                   wave_beacon_man_private_cleanup, (&radio->bmgr_priv, radio->hw_api->hw));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_PDB_INIT_FINALIZE, MTLK_OBJ_PTR(radio),
                   MTLK_NOACTION, ());
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_IEEE80211_REGISTER, MTLK_OBJ_PTR(radio),
                   wv_ieee80211_unregister_if_needed, (radio->mac80211));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_IEEE80211_INIT, MTLK_OBJ_PTR(radio),
                   wv_ieee80211_cleanup, (radio->mac80211));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_IEEE80211_NAME_INIT, MTLK_OBJ_PTR(radio),
                   MTLK_NOACTION, ());
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_WSS_SNAPSHOT_CREATE, MTLK_OBJ_PTR(radio),
                      mtlk_wss_delete, (radio->wss_snapshot));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_WSS_CREATE, MTLK_OBJ_PTR(radio),
                      mtlk_wss_delete, (radio->wss));
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_CONTEXT_INIT, MTLK_OBJ_PTR(radio),
                   MTLK_NOACTION, ());
    MTLK_STOP_STEP(wave_radio, WAVE_RADIO_PDB_INIT, MTLK_OBJ_PTR(radio),
                   wave_pdb_delete, (radio->param_db));
   MTLK_STOP_STEP(wave_radio, WAVE_RADIO_PROBE_LIST_INIT, MTLK_OBJ_PTR(radio),
                  _wave_probe_req_list_destroy, (radio));
  MTLK_STOP_END(wave_radio, MTLK_OBJ_PTR(radio));
}

void wave_radio_deinit(wave_radio_descr_t *radio_descr)
{
  unsigned i;

  for (i = 0; i < radio_descr->num_radios; i++)
      _wave_radio_deinit(&radio_descr->radio[i]);
}

static int _wave_radio_init(wave_radio_t *radio, struct device *dev)
{
  const char* phy_dev_name = NULL;
  mtlk_card_type_t hw_type = MTLK_CARD_UNKNOWN;
  mtlk_card_type_info_t hw_type_info;
  int res;

  MTLK_START_TRY(wave_radio, MTLK_OBJ_PTR(radio));

  res = mtlk_hw_get_prop(radio->hw_api, MTLK_HW_PROP_CARD_TYPE, &hw_type, sizeof(hw_type));
  if (res != MTLK_ERR_OK) {
    ELOG_D("RadioID %u: Can't determine HW type", radio->idx);
    return res;
  }

  res = mtlk_hw_get_prop(radio->hw_api, MTLK_HW_PROP_CARD_TYPE_INFO, &hw_type_info, sizeof(hw_type_info));
  if (res != MTLK_ERR_OK) {
    ELOG_D("RadioID %u: Can't determine HW type info", radio->idx);
    return res;
  }

    MTLK_START_STEP(wave_radio, WAVE_RADIO_PROBE_LIST_INIT, MTLK_OBJ_PTR(radio),
                    _wave_probe_req_list_init, (radio));

    MTLK_START_STEP_EX(wave_radio, WAVE_RADIO_PDB_INIT, MTLK_OBJ_PTR(radio),
                       wave_pdb_create, (PARAM_DB_MODULE_ID_RADIO, hw_type, hw_type_info),
                       radio->param_db,
                       radio->param_db != NULL,
                       MTLK_ERR_NO_MEM);

    MTLK_START_STEP(wave_radio, WAVE_RADIO_CONTEXT_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_context_init, (radio));

    MTLK_START_STEP_EX(wave_radio, WAVE_RADIO_WSS_CREATE, MTLK_OBJ_PTR(radio),
                       mtlk_wss_create, (mtlk_dfg_get_driver_wss(),
                       _wave_radio_wss_id_map, ARRAY_SIZE(_wave_radio_wss_id_map)),
                       radio->wss, radio->wss != NULL, MTLK_ERR_NO_MEM);

    MTLK_START_STEP_EX(wave_radio, WAVE_RADIO_WSS_SNAPSHOT_CREATE, MTLK_OBJ_PTR(radio),
                       mtlk_wss_create, (mtlk_dfg_get_driver_wss(),
                       _wave_radio_wss_id_map, ARRAY_SIZE(_wave_radio_wss_id_map)),
                       radio->wss_snapshot, radio->wss_snapshot != NULL, MTLK_ERR_NO_MEM);

    MTLK_START_STEP(wave_radio, WAVE_RADIO_IEEE80211_NAME_INIT, MTLK_OBJ_PTR(radio),
                    wv_ieee80211_phy_name_init, (radio, HANDLE_T(radio->hw_api->hw), &phy_dev_name));

    MTLK_START_STEP_EX(wave_radio, WAVE_RADIO_IEEE80211_INIT, MTLK_OBJ_PTR(radio),
                       wv_ieee80211_init, (dev, radio, HANDLE_T(radio->hw_api->hw), phy_dev_name),
                       radio->mac80211,
                       NULL != radio->mac80211,
                       MTLK_ERR_UNKNOWN);

    MTLK_START_STEP(wave_radio, WAVE_RADIO_IEEE80211_REGISTER, MTLK_OBJ_PTR(radio),
                    wv_ieee80211_setup_register, (dev, radio, radio->mac80211, HANDLE_T(radio->hw_api->hw)));

    MTLK_START_STEP(wave_radio, WAVE_RADIO_PDB_INIT_FINALIZE, MTLK_OBJ_PTR(radio),
                    _wave_radio_pdb_init_finalize, (radio));

    MTLK_START_STEP(wave_radio, WAVE_RADIO_BEACON_MAN_PRIV_INIT, MTLK_OBJ_PTR(radio),
                    wave_beacon_man_private_init, (&radio->bmgr_priv));

    /* AP mode interface creation should be initiated here in order to create a wlan
     * device. As for station mode interface, mac80211 will create the first wlan
     * device for every radio when mac80211 is initialized and registered */
    MTLK_START_STEP(wave_radio, WAVE_RADIO_VAP_CREATE, MTLK_OBJ_PTR(radio),
                    _wave_radio_master_vap_create, (radio, MTLK_ROLE_AP));

    MTLK_START_STEP(wave_radio, WAVE_RADIO_SCAN_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_scan_init, (radio));
    MTLK_START_STEP(wave_radio, WAVE_RADIO_CHANDEF_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_chandef_init, (radio));
    MTLK_START_STEP(wave_radio, WAVE_RADIO_COC_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_coc_init, (radio));
    MTLK_START_STEP(wave_radio, WAVE_RADIO_ERP_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_erp_init, (radio));
    MTLK_START_STEP(wave_radio, WAVE_RADIO_PROC_INIT, MTLK_OBJ_PTR(radio),
                    _wave_radio_proc_init, (radio));

  MTLK_START_FINALLY(wave_radio, MTLK_OBJ_PTR(radio));
  MTLK_START_RETURN(wave_radio, MTLK_OBJ_PTR(radio), _wave_radio_deinit, (radio));
}

int wave_radio_init(wave_radio_descr_t *radio_descr, struct device *dev)
{
  int res = MTLK_ERR_OK;
  unsigned i;

  for(i = 0; i < radio_descr->num_radios; i++) {
    res = _wave_radio_init(&radio_descr->radio[i], dev);
    if (res != MTLK_ERR_OK) {
      /* cleanup radios that were initialized successfully */
      do {
        _wave_radio_deinit(&radio_descr->radio[i]);
      } while (i--);
      break;
    }
  }
  return res;
}

#ifdef CONFIG_WAVE_RTLOG_REMOTE
int    __MTLK_IFUNC mtlk_df_user_rtlog_register(uint32 hw_idx, mtlk_vap_handle_t vap_handle); /* FIXME */
void   __MTLK_IFUNC mtlk_df_user_rtlog_unregister(uint32 hw_idx); /* FIXME */
#endif

int wave_radio_init_finalize(wave_radio_descr_t *radio_descr)
{
  wave_radio_t* radio = &radio_descr->radio[0];
  mtlk_vap_handle_t vap_handle;
  mtlk_vap_info_internal_t *_info;
  unsigned i;
  int res = MTLK_ERR_OK;

  for (i = 0; i < radio_descr->num_radios; i++) {
    res = mtlk_vap_manager_get_master_vap(radio->vap_manager, &vap_handle);
    if (res != MTLK_ERR_OK)
      return res;

    res = mtlk_vap_start(vap_handle);
    if (MTLK_ERR_OK == res) {
      radio->init_done = TRUE;
    }
    else {
      _info = (mtlk_vap_info_internal_t *)vap_handle;
      ELOG_DDD("CID-%04x: Can't start VAP num=%d res=%d", mtlk_vap_get_oid(vap_handle), _info->id, res);
    }

    /* FW advertises 4x1 (Maximum Capability) in the PSD. Driver to bring-up
     * the configuration as 4x4+0x0 upon init.
     * ZWDFS antenna will be enabled runtime as required by the application.
     */
    if (wave_radio_get_is_zwdfs_radio(radio)) {
      wave_radio_antenna_cfg_update(radio, 0);
    }

    /* Included for enabling dumping of inband dfs radar fifo for WAV700 */
    if ((wave_radio_is_gen7(radio)) && (wave_radio_band_get(radio) == MTLK_HW_BAND_5_2_GHZ)) {
      WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADAR_FIFO_DUMP, 1);
    }

#ifdef CONFIG_WAVE_RTLOG_REMOTE
    /* HW RT-Logger */
    /* WAVE600: TODO: move it out to per HW, not per Radio! */
    if (0 == i)
      mtlk_df_user_rtlog_register(mtlk_vap_manager_get_wlan_index(radio->vap_manager), vap_handle);
#endif
    radio++;
  }

  if (res != MTLK_ERR_OK)
    return res; /* None radios */

  return MTLK_ERR_OK;
}

void wave_radio_deinit_finalize(wave_radio_descr_t *radio_descr)
{
  wave_radio_t* radio = &radio_descr->radio[0];
  unsigned i;

  for (i = 0; i < radio_descr->num_radios; i++) {
    if (radio->init_done) {
      mtlk_vap_manager_stop_all_vaps(radio->vap_manager);
      radio->init_done = FALSE;
    }

    radio++;
  }

#ifdef CONFIG_WAVE_RTLOG_REMOTE
  /* HW RT-Logger */
  /* WAVE600: TODO: move it out to per HW, not per Radio! */
  mtlk_df_user_rtlog_unregister(mtlk_vap_manager_get_wlan_index(radio_descr->radio[0].vap_manager));
#endif
}

void wave_radio_ieee80211_unregister(wave_radio_descr_t *radio_descr)
{
  wave_radio_t* radio = &radio_descr->radio[0];
  unsigned i;

  for (i = 0; i < radio_descr->num_radios; i++) {
#ifdef CPTCFG_MAC80211_DEBUGFS
    mtlk_vap_handle_t vap_handle;
    if (mtlk_vap_manager_get_master_vap(radio->vap_manager, &vap_handle) == MTLK_ERR_OK &&
        mtlk_vap_manager_get_master_ndev_taken(radio->vap_manager)) {
      struct ieee80211_vif * vif = wave_vap_get_vif(vap_handle);
      /* This is done as part of the DC_DP workaround where the driver
       * allocates and frees netdev himself as a workaround
       */
      if (vif) {
        debugfs_remove_recursive(vif->debugfs_dir);
        vif->debugfs_dir = NULL;
      }
    }
#endif
    wv_ieee80211_unregister_if_needed(radio->mac80211);
    radio++;
  }
}

void wave_radio_prepare_stop(wave_radio_descr_t *radio_descr)
{
  wave_radio_t* radio = &radio_descr->radio[0];
  unsigned i;

  for (i = 0; i < radio_descr->num_radios; i++) {
    mtlk_vap_manager_prepare_stop(radio->vap_manager);
    radio++;
  }
}

/*** Context accessors ***/

unsigned wave_radio_id_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->idx;
}

void *wave_radio_beacon_man_private_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return &radio->bmgr_priv;
}

mtlk_wss_t *
wave_radio_wss_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->wss;
}

void __MTLK_IFUNC
mtlk_radio_wss_reset_stat (wave_radio_t *radio)
{
  int i = 0;
  MTLK_ASSERT(NULL != radio);
  for(i = WAVE_RADIO_CNT_BYTES_SENT; i < WAVE_RADIO_CNT_LAST; i++)
  {
    if(i <= WAVE_RADIO_CNT_PACKETS_RECEIVED) /*First 4 Counters are 64bit */
      mtlk_wss_reset_stat_64(radio->wss, i);
    else if(i >= WAVE_RADIO_CNT_CTL_FRAMES_RECEIVED)
    {
      /* For Radio PD bytes sent stats */
      if ((i == WAVE_RADIO_CNT_PD_UNICAST_BYTES_SENT_64) || (i == WAVE_RADIO_CNT_PD_BROADCAST_BYTES_SENT_64) ||
          (i == WAVE_RADIO_CNT_PD_MULTICAST_BYTES_SENT_64))
      {
        mtlk_wss_reset_stat_64(radio->wss, i);
      }
      else
        mtlk_wss_reset_stat(radio->wss, i);
    }
  }
  /* this snapshot implementation is insufficient */
  wave_memcpy(radio->wss_snapshot, sizeof(radio->wss_snapshot), radio->wss, sizeof(radio->wss));
}

mtlk_vap_manager_t *
wave_radio_vap_manager_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->vap_manager;
}

mtlk_df_t *
wave_radio_df_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return mtlk_vap_manager_get_master_df(radio->vap_manager);
}

mtlk_core_t *
wave_radio_master_core_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return mtlk_vap_manager_get_master_core(radio->vap_manager);
}

mtlk_vap_manager_t *
wave_radio_descr_vap_manager_get (wave_radio_descr_t *radio_descr, unsigned radio_idx)
{
  MTLK_ASSERT(radio_idx < radio_descr->num_radios);
  return radio_idx < radio_descr->num_radios ? radio_descr->radio[radio_idx].vap_manager : NULL;
}

mtlk_hw_api_t *
wave_radio_descr_hw_api_get (wave_radio_descr_t *radio_descr, unsigned radio_idx)
{
  MTLK_ASSERT(radio_idx < radio_descr->num_radios);
  return radio_idx < radio_descr->num_radios ? radio_descr->radio[radio_idx].hw_api : NULL;
}

int
wave_radio_descr_master_vap_handle_get (wave_radio_descr_t *radio_descr, unsigned radio_idx, mtlk_vap_handle_t  *vap_handle)
{
  MTLK_ASSERT(radio_idx < radio_descr->num_radios);
  return mtlk_vap_manager_get_master_vap(radio_descr->radio[radio_idx].vap_manager, vap_handle);
}

static mtlk_txmm_t *
_wave_radio_txmm_get (wave_radio_t *radio)
{
  mtlk_txmm_t       *txmm = NULL;
  mtlk_vap_handle_t  vap_handle = NULL;

  MTLK_ASSERT(radio != NULL);

  if (radio) {
    mtlk_vap_manager_get_master_vap(radio->vap_manager, &vap_handle);
  }

  if (vap_handle) {
    txmm = mtlk_vap_get_txmm(vap_handle);
  }

  return txmm;
}

#ifdef MTLK_WAVE_700
mtlk_txmm_t *
wave_radio_txmm_get (wave_radio_t *radio) {
  return _wave_radio_txmm_get(radio);
}
#endif

wave_ant_params_t *
wave_radio_get_ant_params (wave_radio_t *radio)
{
  return ((radio) ? &radio->ant_params : NULL);
}

uint8 __MTLK_IFUNC
wave_radio_max_tx_antennas_get (wave_radio_t *radio)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  return ((ant_params) ? ant_params->tx_ant_num : 0);
}

uint8 __MTLK_IFUNC
wave_radio_max_rx_antennas_get (wave_radio_t *radio)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  return ((ant_params) ? ant_params->rx_ant_num : 0);
}

uint8 __MTLK_IFUNC
wave_radio_tx_antenna_mask_get (wave_radio_t *radio)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  return ((ant_params) ? ant_params->tx_ant_mask : 0);
}

uint8 __MTLK_IFUNC
wave_radio_rx_antenna_mask_get (wave_radio_t *radio)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  return ((ant_params) ? ant_params->rx_ant_mask : 0);
}

void __MTLK_IFUNC
wave_radio_ant_masks_num_sts_get (wave_radio_t *radio, u32 *tx_ant_mask, u32 *rx_ant_mask, u32 *num_sts)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  if (ant_params) {
    *tx_ant_mask  = ant_params->tx_ant_mask;
    *rx_ant_mask  = ant_params->rx_ant_mask;
    *num_sts      = wave_hw_get_num_sts_by_ant_mask (radio->hw_api->hw, *tx_ant_mask);
  }
}

uint8 __MTLK_IFUNC
wave_radio_current_antenna_mask_get (wave_radio_t *radio)
{
  MTLK_ASSERT(radio);
  return mtlk_osal_atomic_get(&radio->cur_ant_mask);
}

void __MTLK_IFUNC
wave_radio_current_antenna_mask_set (wave_radio_t *radio, uint8 mask)
{
  MTLK_ASSERT(radio);
  mtlk_osal_atomic_set(&radio->cur_ant_mask, mask);
}

void __MTLK_IFUNC
wave_radio_current_antenna_mask_reset (wave_radio_t *radio)
{
  MTLK_ASSERT(radio);
  mtlk_osal_atomic_set(&radio->cur_ant_mask, wave_radio_rx_antenna_mask_get(radio));
}

void __MTLK_IFUNC
wave_radio_antenna_cfg_update (wave_radio_t *radio, uint8 mask)
{
  wave_ant_params_t *ant_params = wave_radio_get_ant_params(radio);
  uint8 ant_num = count_bits_set(mask);
  uint8 tx_ant_num = ant_num;
  uint8 tx_ant_mask = mask;

  if (mtlk_hw_type_is_gen6_d2(radio->hw_api->hw) && ant_num == MAX_NUM_RX_ANTENNAS_665) {
    tx_ant_mask = wave_hw_get_psdb_tx_ant_mask(radio->hw_api->hw);
    tx_ant_num = count_bits_set(tx_ant_mask);
  }

  MTLK_ASSERT(radio);

  if (ant_params) {
    ant_params->tx_ant_num = tx_ant_num;
    ant_params->rx_ant_num = ant_num;
    ant_params->tx_ant_mask = tx_ant_mask;
    ant_params->rx_ant_mask = mask;
    ant_params->tx_ant_sel_mask = (uint8)wave_psdb_ant_mask_to_ant_sel_mask(ant_params->tx_ant_mask);
    ant_params->rx_ant_sel_mask = wave_psdb_ant_mask_to_ant_sel_mask(mask);

    wave_radio_current_antenna_mask_set(radio, mask);
  }
}

uint32 _wave_radio_chandef_width_get (const struct cfg80211_chan_def *c)
{
  uint32 width;

  switch (c->width) {
  case NL80211_CHAN_WIDTH_20:
  case NL80211_CHAN_WIDTH_20_NOHT:
    width = 20;
    break;
  case NL80211_CHAN_WIDTH_40:
    width = 40;
    break;
  case NL80211_CHAN_WIDTH_80P80:
  case NL80211_CHAN_WIDTH_80:
    width = 80;
    break;
  case NL80211_CHAN_WIDTH_160:
    width = 160;
    break;
  default:
    return 0;
  }
  return width;
}

uint32 wave_radio_chandef_width_get(const struct cfg80211_chan_def *c)
{
  return _wave_radio_chandef_width_get(c);
}

int
wave_radio_abort_and_prevent_scan (wave_radio_t *radio)
{
  int res;
  mtlk_clpb_t *clpb = NULL;
  mtlk_alter_scan_t alter_scan;

  /* Abort in case a scan is running (initiated by the sta interface)
     and prevent future scans */
  memset(&alter_scan, 0, sizeof(alter_scan));
  alter_scan.abort_scan = TRUE;
  alter_scan.pause_or_prevent = TRUE;

  res = _mtlk_df_user_invoke_core(wave_radio_df_get(radio),
            WAVE_RADIO_REQ_ALTER_SCAN, &clpb, &alter_scan, sizeof(alter_scan));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_ALTER_SCAN, TRUE);

  if (MTLK_ERR_OK != res)
    ELOG_V("Failed to abort or prevent scan");

  return res;
}

int wave_radio_cancel_hw_scan (wave_radio_t *radio)
{
  int res;
  mtlk_clpb_t *clpb = NULL;
  mtlk_alter_scan_t alter_scan;

  memset(&alter_scan, 0, sizeof(alter_scan));
  alter_scan.abort_scan = TRUE;
  ILOG0_V("Received abort scan");

  res = _mtlk_df_user_invoke_core(wave_radio_df_get(radio),
            WAVE_RADIO_REQ_ALTER_SCAN, &clpb, &alter_scan, sizeof(alter_scan));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_ALTER_SCAN, TRUE);

  if (MTLK_ERR_OK != res)
    ELOG_V("Failed to cancel scan");

  return res;
}

int
wave_radio_allow_or_resume_scan (wave_radio_t *radio)
{
  int res;
  mtlk_clpb_t *clpb = NULL;
  mtlk_alter_scan_t alter_scan;

  memset(&alter_scan, 0, sizeof(alter_scan));
  alter_scan.resume_or_allow = TRUE;

  res = _mtlk_df_user_invoke_core(wave_radio_df_get(radio),
            WAVE_RADIO_REQ_ALTER_SCAN, &clpb, &alter_scan, sizeof(alter_scan));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_ALTER_SCAN, TRUE);

  if (MTLK_ERR_OK != res)
    ELOG_V("Failed to resume or allow scan");

  return res;
}

static __INLINE void __wave_radio_beacon_data_copy(mtlk_beacon_data_t *mb, struct cfg80211_beacon_data *beacon)
{
  mb->head = beacon->head;
  mb->tail = beacon->tail;
  mb->head_len = (uint16)beacon->head_len;
  mb->tail_len = (uint16)beacon->tail_len;
  mb->probe_resp = beacon->probe_resp;
  mb->probe_resp_len = (uint16)beacon->probe_resp_len;
  mb->data = NULL;
  mb->dma_addr = 0;
}

int wave_radio_beacon_change(
  wave_radio_t *radio,
  struct net_device *ndev,
  struct cfg80211_beacon_data *beacon)
{
  int res = MTLK_ERR_OK;
  mtlk_vap_handle_t vap_handle;
  uint8 vap_idx;
  mtlk_df_t *master_df;
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  mtlk_beacon_data_t beacon_data;
  mtlk_clpb_t *clpb = NULL;
  mtlk_hw_t *hw;
  mtlk_core_t *core;

  MTLK_ASSERT(NULL != radio);
  MTLK_ASSERT(NULL != beacon);
  hw = radio->hw_api->hw;
  MTLK_ASSERT(NULL != hw);

  ILOG1_SSD("%s Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  vap_handle = mtlk_df_get_vap_handle(mtlk_df_user_get_df(df_user));
  vap_idx = mtlk_vap_get_id(vap_handle);
  core = mtlk_vap_get_core(vap_handle);

  master_df = mtlk_vap_manager_get_master_df(radio->vap_manager);
  MTLK_CHECK_DF(master_df);
  df_user = mtlk_df_get_user(master_df);

  if (wave_radio_get_is_zwdfs_radio(radio)) {
    ILOG1_V("Not setting beacon template in FW for ZWDFS VAP");
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  /* Master VAP is used as a dummy VAP (in order to support reconf
   * of all VAPs without having to restart hostapd) we avoid setting beacon
   * so that the dummy VAPs will not send beacons */
  if (mtlk_vap_is_master(vap_handle)){
    ILOG1_V("Dummy Master VAP, not setting beacon template in FW");
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  __wave_radio_beacon_data_copy(&beacon_data, beacon);
  beacon_data.vap_idx = vap_idx;
  beacon_data.bmgr_priv = &radio->bmgr_priv;

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_BEACON, &clpb, &beacon_data, sizeof(beacon_data));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_BEACON, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int _wave_radio_set_ap_beacon_info(wave_radio_t *radio, struct net_device *ndev,
                                   struct ieee80211_bss_conf *bss_conf)
{
  struct mtlk_beacon_info_parameters beacon_params;
  mtlk_vap_handle_t vap_handle;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(NULL != radio);

  if (NULL == ndev) {
    ELOG_V("ndev is NULL");
    return MTLK_ERR_PARAMS;
  }

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);
  vap_handle = mtlk_df_get_vap_handle(mtlk_df_user_get_df(df_user));

  if (!mtlk_vap_is_ap(vap_handle)) {
    ELOG_V("VAP is not AP");
    return MTLK_ERR_PARAMS;
  }

  if (mtlk_vap_is_master(vap_handle)) {
    /* Master VAP is used as a dummy VAP (in order to support reconf
     * of all VAPs without having to restart hostapd) we avoid setting beacon
     * so that the dummy VAPs will not send beacons */
    ILOG1_V("Dummy Master VAP, not setting ap beacon info in FW");
    return MTLK_ERR_OK;
  }

  memset(&beacon_params, 0, sizeof(beacon_params));
  beacon_params.vap_index = mtlk_vap_get_id(vap_handle);
  beacon_params.beacon_int = bss_conf->beacon_int;
  beacon_params.dtim_period = bss_conf->dtim_period;

  {
    int retry_counter = 0;
    do {
      res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user),
                WAVE_RADIO_REQ_SET_AP_BEACON_INFO, &clpb, &beacon_params, sizeof(beacon_params));
      res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_SET_AP_BEACON_INFO, TRUE);

      if (MTLK_ERR_RETRY != res) break;
      mtlk_osal_msleep(50);
      retry_counter++;
    } while ((MTLK_ERR_RETRY == res) && (retry_counter < MAX_SCAN_WAIT_RETRIES));

    if (retry_counter > 0) {
      ILOG0_SD("%s: Scan waited, number of retries %d", mtlk_df_user_get_name(df_user), retry_counter);
    }
  }

  return res;
}

static __INLINE void
__wave_radio_chandef_copy (struct mtlk_chan_def *mcd, struct cfg80211_chan_def *chandef)
{
  struct mtlk_channel       *mc = &mcd->chan;
  struct ieee80211_channel  *ic = chandef->chan;

  mcd->center_freq1 = chandef->center_freq1;
  mcd->center_freq2 = chandef->center_freq2;
  mcd->width = nlcw2mtlkcw(chandef->width);
  mcd->is_noht = (chandef->width == NL80211_CHAN_WIDTH_20_NOHT);
  mcd->power_level = MTLK_POWER_NOT_SET;

  mc->dfs_state_entered = ic->dfs_state_entered;
  mc->dfs_state = ic->dfs_state;
  mc->band = nlband2mtlkband(ic->band);
  mc->center_freq = ic->center_freq;
  mc->flags = ic->flags;
  mc->orig_flags = ic->orig_flags;
  mc->max_antenna_gain = ic->max_antenna_gain;
  mc->max_power = ic->max_power;
  mc->max_reg_power = ic->max_reg_power;
  mc->dfs_cac_ms = ic->dfs_cac_ms;
}

void
wave_radio_chandef_copy (struct mtlk_chan_def *mcd, struct cfg80211_chan_def *chandef)
{
  return __wave_radio_chandef_copy(mcd, chandef);
}

int
wave_radio_set_first_non_dfs_chandef (wave_radio_t * radio, struct mtlk_chan_def *ccd)
{
  struct wiphy *wiphy;
  struct mtlk_chan_def *mcd;
  struct ieee80211_channel *ic = NULL;
  struct mtlk_channel *mc;
  struct ieee80211_supported_band *band;
  int i;
  wiphy = wv_mac80211_wiphy_get(wave_radio_mac80211_get(radio));
  mcd = wave_radio_chandef_get(radio);

  if (!mcd->center_freq1) {
    ILOG1_V("no scan was done, no need to set channel");
    return MTLK_ERR_UNKNOWN;
  }

  band = wiphy->bands[mtlkband2nlband(mcd->chan.band)];
  mc = &ccd->chan;

  for (i=0; i < band->n_channels; i++) {
    ic = &band->channels[i];
    if (!(ic->flags & (IEEE80211_CHAN_RADAR | IEEE80211_CHAN_DISABLED)))
      break;
  }

  if (!ic)
    return MTLK_ERR_UNKNOWN;

  ccd->center_freq1 = ic->center_freq;
  ccd->center_freq2 = 0;
  ccd->width = CW_20;
  ccd->power_level = MTLK_POWER_NOT_SET;

  mc->dfs_state_entered = ic->dfs_state_entered;
  mc->dfs_state = ic->dfs_state;
  mc->band = nlband2mtlkband(ic->band);
  mc->center_freq = ic->center_freq;
  mc->flags = ic->flags;
  mc->orig_flags = ic->orig_flags;
  mc->max_antenna_gain = ic->max_antenna_gain;
  mc->max_power = ic->max_power;
  mc->max_reg_power = ic->max_reg_power;
  mc->dfs_cac_ms = ic->dfs_cac_ms;
  return MTLK_ERR_OK;
}

int wave_radio_ap_start(
  struct wiphy *wiphy,
  wave_radio_t *radio,
  struct net_device *ndev,
  struct ieee80211_vif *vif,
  struct ieee80211_bss_conf *bss_conf,
  struct cfg80211_beacon_data *beacon)
{
  int res = 0;
  mtlk_df_user_t *df_user;
  mtlk_df_t * df;
  mtlk_core_t *nic;
  struct mtlk_chan_def  *ccd;
  mtlk_core_t *master_core;
  u32 ap_short_ssid = 0;

  MTLK_ASSERT(NULL != radio);

  if (ndev == NULL) {
    mtlk_df_t *tmp_df = wave_radio_df_get(radio);
    MTLK_CHECK_DF(tmp_df);
    df_user = mtlk_df_get_user(tmp_df);
  } else {
    df_user = mtlk_df_user_from_ndev(ndev);
  }

  MTLK_ASSERT(NULL != df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  ccd = wave_radio_chandef_get(radio);
  master_core = mtlk_vap_manager_get_master_core(radio->vap_manager);

  /* FIXCFG80211: add all necessary configuration */
  MTLK_CHECK_DF_USER(df_user);

  if (MAX_SSID_LEN < vif->cfg.ssid_len)
    goto finish;

  df = mtlk_df_user_get_df(df_user);
  nic = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
  if (!mtlk_vap_is_sta(mtlk_df_get_vap_handle(df))) {
    MTLK_CORE_PDB_SET_BINARY(nic, PARAM_DB_CORE_ESSID, vif->cfg.ssid, vif->cfg.ssid_len);
    wave_pdb_set_int(mtlk_vap_get_param_db(nic->vap_handle), PARAM_DB_CORE_HIDDEN_SSID, bss_conf->hidden_ssid);

    /* Compute short SSID and store it in Param DB for later use */
    ap_short_ssid = cpu_to_le32(ieee80211_crc32(vif->cfg.ssid, vif->cfg.ssid_len));
    wave_pdb_set_int(mtlk_vap_get_param_db(nic->vap_handle), PARAM_DB_CORE_SHORT_SSID, ap_short_ssid);

    if(mtlk_hw_type_is_gen6_d2_or_gen7(radio->hw_api->hw)) {
        struct intel_vendor_he_capa he_non_advertised = {0};
        mtlk_pdb_size_t he_non_advertised_len = sizeof(he_non_advertised);
        MTLK_CORE_PDB_GET_BINARY(nic, PARAM_DB_CORE_HE_NON_ADVERTISED, &he_non_advertised,
                                 &he_non_advertised_len);
        he_non_advertised.he_mac_capab_info[HE_MAC_CAP3_IDX] = HE_NON_ADVERT_MAC_CAP3_D2;
        he_non_advertised.he_phy_capab_info[HE_PHY_CAP1_IDX] = HE_NON_ADVERT_PHY_CAP1_D2;
        he_non_advertised.he_phy_capab_info[HE_PHY_CAP3_IDX] = HE_NON_ADVERT_PHY_CAP3_D2;
        he_non_advertised.he_phy_capab_info[HE_PHY_CAP8_IDX] = HE_NON_ADVERT_PHY_CAP8_D2;
        MTLK_CORE_PDB_SET_BINARY(nic, PARAM_DB_CORE_HE_NON_ADVERTISED, &he_non_advertised,
                                 he_non_advertised_len);
    }

  }

  if ((res = wave_radio_beacon_change(radio, ndev, beacon)) != 0) {
    ELOG_SD("%s: failed to set beacon template (res=%i)", ndev->name, res);
    goto finish;
  }

  if ((res = _wave_radio_set_ap_beacon_info(radio, ndev, &vif->bss_conf)) != 0) {
    ELOG_SD("%s: failed to set beacon info (res=%i)", ndev->name, res);
    goto finish;
  }

  nic->slow_ctx->ap_started = TRUE;
  /* start stats timer */
  mtlk_df_user_start_stats_timer(df_user, wave_hw_mmb_get_stats_poll_period(radio->hw_api->hw));

#if 0
  /* Radio param DB test */
  {
    ELOG_V("*********** Radio param db test ***********");
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_TEST_PARAM0, 0xf1f1f1f1);
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_TEST_PARAM1, 0x2f2f2f2f);
    ELOG_D("PARAM_DB_RADIO_TEST_PARAM0 0x%x", WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_TEST_PARAM0));
    ELOG_D("PARAM_DB_RADIO_TEST_PARAM1 0x%x", WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_TEST_PARAM1));
  }
#endif

finish:
  return res;
}

int wave_radio_ap_stop(
  wave_radio_t *radio,
  struct net_device *ndev)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_df_t * df;
  char fils_discovery_null[] = "";
  unsolicit_tx_frame_info_t frame_info = {0, {NULL, NULL}, {0, 0}, {0, 0}};
  unsolicit_tx_frame_info_t *p_frame_info = &frame_info;
  mtlk_pdb_size_t size = sizeof(unsolicit_tx_frame_info_t);
  uint8 idx = 0;

  MTLK_ASSERT(NULL != radio);

  if (ndev == NULL) {
    ELOG_V("ndev is NULL");
    res = MTLK_ERR_PARAMS;
    goto finish;
  }

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_ASSERT(NULL != df_user);
  MTLK_CHECK_DF_USER(df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", mtlk_df_user_get_name(df_user), current->comm, current->pid);

  df = mtlk_df_user_get_df(df_user);
  if (!mtlk_vap_is_sta(mtlk_df_get_vap_handle(df))) {
    /*To make sure we won't send probe responses*/
    mtlk_core_t * nic = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
    nic->slow_ctx->ap_started = FALSE;

    /* Stop stats timer if ap stopped */
    mtlk_df_user_stop_stats_timer(df_user);

    if ((MTLK_HW_BAND_6_GHZ == wave_radio_band_get(radio)) &&
        (MTLK_CORE_PDB_GET_INT(nic, PARAM_DB_CORE_MBSSID_VAP) < WAVE_RADIO_OPERATION_MODE_MBSSID_NON_TRANSMIT_VAP)) {
      mtlk_pdb_t *radio_pdb = wave_radio_param_db_get(radio);
      mtlk_pdb_size_t fils_frame_len = wave_pdb_get_int(radio_pdb, PARAM_DB_RADIO_FILS_DISCOVERY_FRAME_LEN);
      ILOG1_V("Resetting the 6GHz unsolicited frame template in the radio PDB");
      wave_pdb_set_int(radio_pdb, PARAM_DB_RADIO_UNSOLICIT_PROBE_RESP_TEMPL_LEN, 0);
      wave_pdb_set_int(radio_pdb, PARAM_DB_RADIO_FILS_DISCOVERY_FRAME_LEN, 0);
      res = wave_pdb_set_binary(radio_pdb, PARAM_DB_RADIO_FILS_DISCOVERY_FRAME,
                                fils_discovery_null, fils_frame_len);
      res = wave_pdb_get_binary(radio_pdb, PARAM_DB_RADIO_UNSOLICIT_TX_FRAME_INFO,
                                p_frame_info, &size);
      if (res != MTLK_ERR_OK) {
        ELOG_D("Unable to retrieve unsolicited tx frame info from Param DB! res = %d", res);
        goto finish;
      }
      for (idx = 0; idx < MAX_NUM_UNSOLICIT_TX_BUFS; idx++) {
        wave_unsolicited_frame_templ_unmap_free(nic, p_frame_info, idx);
      }
      p_frame_info->buf_index = 0;
      res = wave_pdb_set_binary(radio_pdb, PARAM_DB_RADIO_UNSOLICIT_TX_FRAME_INFO,
                                (const void *)p_frame_info, sizeof(unsolicit_tx_frame_info_t));

      res = _wave_beacon_reset_btwt_config(nic);
    }
  }

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/* The next bitrate tables are filled according to values from hostapd */
static const u8 rateset_80211b[]  = {2, 4, 11, 22};                     /* 1, 2, 5.5, 11 Mbps */
static const u8 rateset_80211ag[] = {12, 18, 24, 36, 48, 72, 96, 108};  /* 6, 9, 12, 18, 24, 36, 48, 54 Mbps */

static BOOL
_wave_radio_rate_check(const u8 *rateset, int count, uint8 bitrate)
{
  do {
    if (*rateset++ == bitrate)
      return TRUE;
  } while (--count);
  return FALSE;
}

BOOL
wave_radio_is_rate_80211b(uint8 bitrate)
{
  return _wave_radio_rate_check(rateset_80211b, ARRAY_SIZE(rateset_80211b), bitrate);
}

BOOL
wave_radio_is_rate_80211ag(uint8 bitrate)
{
  return _wave_radio_rate_check(rateset_80211ag, ARRAY_SIZE(rateset_80211ag), bitrate);
}

int wave_radio_sta_change (
  wave_radio_t *radio,
  struct net_device *ndev,
  const uint8 *mac,
  struct station_parameters *params,
  struct ieee80211_sta * sta)
{
  int res = MTLK_ERR_PARAMS;
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  mtlk_core_ui_authorizing_cfg_t t_authorizing;

  MTLK_ASSERT(NULL != radio);

  memset(&t_authorizing, 0, sizeof(t_authorizing));

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!netif_running(ndev)) {
    WLOG_S("%s: You should bring interface up first", ndev->name);
    goto finish;
  }

  if(mac == NULL){
    WLOG_S("%s: MAC addr is not set", ndev->name);
    goto finish;
  }

  /* Use this only for authorizing/unauthorizing a station */
  if (!(params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED))){
    ILOG2_SD("%s: Only authorizing/unauthorizing is supporting, change_flags:0x%x", ndev->name, params->sta_flags_mask);
    res = MTLK_ERR_OK;
    goto finish;
  }

  MTLK_CFG_SET_ITEM_BY_FUNC_VOID(&t_authorizing, sta_addr, mtlk_osal_copy_eth_addresses, (t_authorizing.sta_addr.au8Addr, mac));
  MTLK_CFG_SET_ITEM(&t_authorizing, authorizing, !!(params->sta_flags_set & BIT(NL80211_STA_FLAG_AUTHORIZED)));
  MTLK_CFG_SET_ITEM(&t_authorizing, sta, sta);
  ILOG2_YD("STA:%Y authorizing flag:%d", mac, t_authorizing.authorizing);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_AUTHORIZING_STA, &clpb, &t_authorizing, sizeof(t_authorizing));
  res = _mtlk_df_user_process_core_retval_void(res, clpb, WAVE_CORE_REQ_AUTHORIZING_STA, TRUE);

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static __INLINE uint _TU_to_ms(uint tu)
{
  return (tu * 1024) / 1000;
}

int wave_radio_scan (
  wave_radio_t *radio,
  struct net_device *ndev,
  struct cfg80211_scan_request *request)
{
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_t *master_df;
  struct mtlk_scan_request *sr;
  int res = MTLK_ERR_OK;
  int i;
  void *wiphy;
  mtlk_core_t *core;
  uint8 vap_id = 0;
  struct ieee80211_vif *vif = NULL;
  mtlk_df_user_t *df_user;
  mtlk_df_t *df = NULL;
  int numchans;

  MTLK_ASSERT(NULL != radio);

  numchans = (wave_radio_band_get(radio) == MTLK_HW_BAND_6_GHZ) ?
              NUM_TOTAL_CHANS_6G : NUM_TOTAL_CHANS;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  master_df = mtlk_vap_manager_get_master_df(radio->vap_manager);
  MTLK_CHECK_DF(master_df);

  core = mtlk_vap_get_core(mtlk_df_get_vap_handle(master_df));

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  df = mtlk_df_user_get_df(df_user);
  vap_id = mtlk_vap_get_id(mtlk_df_get_vap_handle(df));
  vif = wv_mac80211_get_vif(radio->mac80211, vap_id);
  MTLK_ASSERT(NULL != vif);
  if (!vif)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_UNKNOWN);

  ILOG1_SSDPP("%s: Invoked from %s (%i): wihpy=%p, request=%p",
              mtlk_df_user_get_name(mtlk_df_get_user(master_df)), current->comm, current->pid, wiphy, request);

  if (request->n_channels > numchans
      || request->n_ssids > MAX_SCAN_SSIDS
      || request->ie_len > MAX_SCAN_IE_LEN)
  {
    ELOG_V("Maximum number of channels, ssids or extra IE length exceeded");
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_VALUE);
  }

  /* scan request is too big to put on the stack */
  if (!(sr = mtlk_osal_mem_alloc(sizeof(*sr), MTLK_MEM_TAG_CFG80211)))
  {
    ELOG_V("mac80211: can't allocate memory");
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_MEM);
  }

  memset(sr, 0, sizeof(*sr));
  if (vif->type == NL80211_IFTYPE_STATION)
    sr->type = MTLK_SCAN_STA;
  else
    sr->type = MTLK_SCAN_AP;
  sr->requester_vap_index = vap_id;
  sr->saved_request = request;
  sr->wiphy = wiphy;
  sr->n_channels = request->n_channels;

  /* The duration passed using attr NL80211_ATTR_MEASUREMENT_DURATION.
      CCA, ROC and general scan cannot run together, so here request->duration is copy of cfg kernel structure */
  sr->duration = _TU_to_ms(request->duration);
  sr->duration_mandatory = !!request->duration_mandatory;
  ILOG1_DDD("duration(TU):%u, duration(ms):%u, duration_mandatory:%u",
    request->duration, sr->duration, (unsigned)request->duration_mandatory);

  for (i = 0; i < (int)sr->n_channels; i++)
  {
    struct mtlk_channel *mc = &sr->channels[i];
    struct ieee80211_channel *ic = request->channels[i];

    mc->dfs_state_entered = ic->dfs_state_entered;
    mc->dfs_state = ic->dfs_state;
    mc->dfs_cac_ms = ic->dfs_cac_ms;
    mc->band = nlband2mtlkband(ic->band);
    mc->center_freq = ic->center_freq;
    mc->flags = ic->flags;
    mc->orig_flags = ic->orig_flags;
    mc->max_antenna_gain = ic->max_antenna_gain;
    mc->max_power = ic->max_power;
    mc->max_reg_power = ic->max_reg_power;
  }

  sr->flags = request->flags;

  for (i = 0; i < NUM_SUPPORTED_BANDS; i++) {
    if (mtlk_nl_band_is_supported(i)) {
      mtlk_hw_band_e hw_band = nlband2mtlkband(i);
      sr->rates[hw_band] = request->rates[i];
    }
  }

  sr->n_ssids = request->n_ssids;

  for (i = 0; i < sr->n_ssids; i++)
  {
    /* in case SSID-s with '\0'-s in the middle are acceptable, we'll need to keep the len, too */
    wave_strncopy(sr->ssids[i], request->ssids[i].ssid, sizeof(sr->ssids[i]), request->ssids[i].ssid_len);
  }

  sr->ie_len = request->ie_len;
  /* request->ie and request->ie_len can be NULL */
  if (request->ie_len) {
    wave_memcpy(sr->ie, sizeof(sr->ie), request->ie, request->ie_len);
  }

  /* Keep trying in case scan is in progress for the current radio */
  {
    int retry_counter = 0;

    do {
      /* any checks for preexisting scans done later to minimize locking */
      res = _mtlk_df_user_invoke_core(master_df, WAVE_RADIO_REQ_DO_SCAN, &clpb, sr, sizeof(*sr));
      res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_DO_SCAN, TRUE);

      if (MTLK_ERR_RETRY != res) break;
      mtlk_osal_msleep(50);
      retry_counter++;
    } while ((MTLK_ERR_RETRY == res) && (retry_counter < MAX_SCAN_WAIT_RETRIES));

    if (retry_counter > 0)
      ILOG0_SD("%s: Scan waited, number of retries %d", mtlk_df_user_get_name(mtlk_df_get_user(master_df)), retry_counter);
  }

  /* no waiting for the end of the scan, just return and let the kernel report the scan as started */
  mtlk_osal_mem_free(sr);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_qos_map_set(wave_radio_t *radio, struct net_device *ndev, struct cfg80211_qos_map *qos_map)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  mtlk_clpb_t *clpb = NULL;

  MTLK_ASSERT(NULL != radio);
  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (NULL == qos_map) {
    ILOG2_V("qos_map is NULL");
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_QOS_MAP, &clpb, qos_map, sizeof(struct cfg80211_qos_map));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_QOS_MAP, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static void _wave_radio_coc_power_params_set(BOOL mode, uint32 tx_ant, uint32 rx_ant,
  mtlk_coc_power_cfg_t* power_params)
{
  MTLK_ASSERT(NULL != power_params);

  power_params->is_auto_mode     = mode;
  power_params->antenna_params.num_tx_antennas = tx_ant;
  power_params->antenna_params.num_rx_antennas = rx_ant;
}

int wave_radio_antenna_set(wave_radio_t *radio, u32 tx_ant, u32 rx_ant)
{
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_t *df;
  mtlk_df_user_t *df_user;
  mtlk_coc_mode_cfg_t coc_cfg;
  BOOL auto_mode = FALSE;
  mtlk_coc_t *coc_mgmt = wave_radio_coc_mgmt_get(radio);

  MTLK_ASSERT(NULL != radio);

  df = wave_radio_df_get(radio);
  MTLK_CHECK_DF(df);

  df_user = mtlk_df_get_user(df);
  MTLK_ASSERT(NULL != df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", mtlk_df_user_get_name(df_user),
    current->comm, current->pid);

  memset(&coc_cfg, 0, sizeof(coc_cfg));
  auto_mode = mtlk_coc_get_auto_mode_cfg(coc_mgmt);
  MTLK_CFG_SET_ITEM_BY_FUNC_VOID(&coc_cfg, power_params,
    _wave_radio_coc_power_params_set, (auto_mode, count_bits_set(tx_ant), count_bits_set(rx_ant),
      &coc_cfg.power_params));
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_RADIO_REQ_SET_COC_CFG, &clpb, &coc_cfg, sizeof(coc_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_SET_COC_CFG,
    TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_antenna_get(wave_radio_t *radio, u32 *tx_ant, u32 *rx_ant)
{
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_user_t *df_user;
  mtlk_df_t *master_df;
  mtlk_coc_mode_cfg_t *coc_cfg;
  uint32 coc_cfg_size;
  uint32 chip_id;
  wave_ant_params_t *ant_params = &radio->ant_params;

  MTLK_ASSERT(NULL != radio);

  master_df = wave_radio_df_get(radio);
    /* get_antenna can be called before master_vap is created */
  if (NULL == master_df) {
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NOT_READY);
  }

  df_user = mtlk_df_get_user(master_df);
  MTLK_CHECK_DF_USER(df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", mtlk_df_user_get_name(df_user),
    current->comm, current->pid);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_RADIO_REQ_GET_COC_CFG, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_GET_COC_CFG, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  chip_id = __wave_radio_get_chip_id(radio);
  coc_cfg = mtlk_clpb_enum_get_next(clpb, &coc_cfg_size);
  MTLK_CLPB_TRY(coc_cfg, coc_cfg_size) {
    *rx_ant = coc_cfg->cur_ant_mask;
    if (_chipid_is_gen6_d2(chip_id) && (*rx_ant == ant_params->rx_ant_mask)) {
      *tx_ant = ant_params->tx_ant_mask;
    } else {
      *tx_ant = coc_cfg->cur_ant_mask;
    }
  }
  MTLK_CLPB_FINALLY(res) {
    mtlk_clpb_delete(clpb);
  }
  MTLK_CLPB_END;

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_rts_threshold_set(wave_radio_t *radio, u32 rts_threshold)
{
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_t *df;
  mtlk_df_user_t *df_user;
  mtlk_wlan_rts_threshold_cfg_t   rts_threshold_cfg;

  MTLK_ASSERT(NULL != radio);

  df = wave_radio_df_get(radio);
  MTLK_CHECK_DF(df);

  df_user = mtlk_df_get_user(df);
  MTLK_ASSERT(NULL != df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", mtlk_df_user_get_name(df_user),
	    current->comm, current->pid);
  MTLK_CFG_SET_ITEM(&rts_threshold_cfg, threshold, rts_threshold);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
      WAVE_RADIO_REQ_SET_RTS_THRESHOLD, &clpb, &rts_threshold_cfg, sizeof(rts_threshold_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_SET_RTS_THRESHOLD,
      TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_aid_get (struct net_device *ndev,
                        const uint8 *mac_addr,
                        u16 *aid_out)
{
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  ILOG1_Y("mac_addr = %Y", mac_addr);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(mac_addr)) {
    ELOG_SY("%s: Invalid MAC address: %Y", ndev->name, mac_addr);
    res = MTLK_ERR_PARAMS;
    goto finish;
  }

  if (!netif_running(ndev)) {
    WLOG_S("%s: You should bring interface up first", ndev->name);
    res = MTLK_ERR_WRONG_CONTEXT;
    goto finish;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_REQUEST_AID, &clpb, mac_addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_REQUEST_AID, FALSE);

  if (MTLK_ERR_OK == res) {
    u16 *aid;
    uint32 aid_size;

    aid = mtlk_clpb_enum_get_next(clpb, &aid_size);
    MTLK_CLPB_TRY(aid, aid_size)
      *aid_out = *aid;
    MTLK_CLPB_FINALLY(res)
      mtlk_clpb_delete(clpb); /* already deleted in error cases */
    MTLK_CLPB_END;
  }

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_aid_free (struct net_device *ndev, u16 aid)
{
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  ILOG1_DD("AID=%hi, SID=%hi", aid, aid - 1);

  if (!df_user)
    /* If the user performs ifconfig <slave vap> down, hostapd will send sid free
     * commands only after the vif was removed from the driver (by op_interface_remove)
     * By now, the driver has freed the vif's df_user.
     * In this case, all sids were already removed from FW in remove_interface callback.
     */
    goto finish;

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_REMOVE_AID, &clpb, &aid, sizeof(aid));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_REMOVE_AID, TRUE);

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_sync_done (struct net_device *ndev)
{
  mtlk_df_user_t *df_user = mtlk_df_user_from_ndev(ndev);
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!netif_running(ndev)) {
    WLOG_S("%s: You should bring interface up first", ndev->name);
    res = MTLK_ERR_WRONG_CONTEXT;
    goto finish;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SYNC_DONE, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SYNC_DONE, TRUE);

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/* Get/set param DB value of 11N_ACAX_COMPAT */
void __MTLK_IFUNC
_wave_radio_set_11n_acax_compat (wave_radio_t *radio, int value)
{
  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_11N_ACAX_COMPAT, value);
}

int __MTLK_IFUNC
wave_radio_get_11n_acax_compat (wave_radio_t *radio)
{
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_11N_ACAX_COMPAT);
}

int wave_radio_initial_data_receive (struct net_device *ndev,
                                     struct intel_vendor_initial_data_cfg *initial_data)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  MTLK_ASSERT(NULL != ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user), WAVE_RADIO_REQ_SET_INITAL_DATA,
                                  &clpb, initial_data, sizeof(*initial_data));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_SET_INITAL_DATA, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_deny_mac_set (struct net_device *ndev,
                             struct intel_vendor_blacklist_cfg *vendor_cfg)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct intel_vendor_blacklist_cfg blacklist_cfg;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  blacklist_cfg = *vendor_cfg; /* struct copy */

  if (!ieee_addr_is_valid(&blacklist_cfg.addr) &&
      (!ieee_addr_is_zero(&blacklist_cfg.addr) || !blacklist_cfg.remove))
  {
    ELOG_Y("Invalid MAC address %Y", blacklist_cfg.addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SET_BLACKLIST_ENTRY, &clpb, &blacklist_cfg,
    sizeof(blacklist_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SET_BLACKLIST_ENTRY, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_sta_steer (struct net_device *ndev,
                          struct intel_vendor_steer_cfg *steer_cfg)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!ieee_addr_is_valid(&steer_cfg->addr)) {
    ELOG_Y("Invalid MAC address %Y", steer_cfg->addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  ILOG2_DYY("CID-%04x: steer station %Y to BSSID %Y",
            mtlk_vap_get_oid(mtlk_df_get_vap_handle(mtlk_df_user_get_df(df_user))), steer_cfg->addr.au8Addr,
            steer_cfg->bssid.au8Addr);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user), WAVE_RADIO_REQ_STEER_STA,
                                  &clpb, steer_cfg, sizeof(*steer_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_STEER_STA, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static void _wave_radio_fill_sta_info(struct intel_vendor_sta_info* vendor_sta_info,
  struct driver_sta_info* sta_info)
{
#define ASSIGN_STA_INFO_PEER_RATES_INFO_PARAM(name)                     \
  do {                                                                  \
    vendor_sta_info->name = sta_info->rates_info.name;                  \
  } while (0)

#define ASSIGN_STA_INFO_PEER_STATS_PARAM(name)                          \
  do {                                                                  \
    vendor_sta_info->name = sta_info->peer_stats.name;                  \
  } while (0)

#define ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(name)                    \
  do {                                                                  \
    vendor_sta_info->name = sta_info->peer_stats.tr181_stats.name;      \
  } while (0)

#define ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM(name)                  \
  do {                                                                  \
    vendor_sta_info->name =                                             \
      sta_info->peer_stats.tr181_stats.traffic_stats.name;              \
  } while (0)

#define ASSIGN_STA_INFO_PEER_RETRANS_STATS_PARAM(name)                  \
  do {                                                                  \
    vendor_sta_info->name =                                             \
      sta_info->peer_stats.tr181_stats.retrans_stats.name;              \
  } while (0)

  int i;

  MTLK_ASSERT(vendor_sta_info != NULL);
  MTLK_ASSERT(sta_info != NULL);

  memset(vendor_sta_info, 0, sizeof(*vendor_sta_info));

  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(StationId);
  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(NetModesSupported);

  ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM(BytesSent);
  ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM(BytesReceived);
  ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM(PacketsSent);
  ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM(PacketsReceived);

  /* Excluding unavailable Retransmissions and MultipleRetryCount */
  ASSIGN_STA_INFO_PEER_RETRANS_STATS_PARAM(RetransCount);
  ASSIGN_STA_INFO_PEER_RETRANS_STATS_PARAM(FailedRetransCount);
  ASSIGN_STA_INFO_PEER_RETRANS_STATS_PARAM(RetryCount);

  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(LastDataUplinkRate);
  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(LastDataDownlinkRate);
  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(SignalStrength);
  ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM(ErrorsSent);

  ASSIGN_STA_INFO_PEER_RATES_INFO_PARAM(TxMgmtPwr);
  ASSIGN_STA_INFO_PEER_RATES_INFO_PARAM(TxStbcMode);

  for (i = 0; i < NUMBER_OF_RX_ANTENNAS; ++i) {
    ASSIGN_STA_INFO_PEER_STATS_PARAM(ShortTermRSSIAverage[i]);
    ASSIGN_STA_INFO_PEER_STATS_PARAM(snr[i]);
  }

  /* Fill Phy Data Rate info if it is available from TX/RX rates info */
  {
    mtlk_wssa_drv_peer_rate_info1_t   *rate_info = NULL;

    if (sta_info->rates_info.tx_data_rate_info.InfoFlag) {
      rate_info = &sta_info->rates_info.tx_data_rate_info;
    } else if (sta_info->rates_info.rx_data_rate_info.InfoFlag) {
      rate_info = &sta_info->rates_info.rx_data_rate_info;
    }

    if (rate_info) { /* available */
      vendor_sta_info->RateInfoFlag = 1;
      vendor_sta_info->RatePhyMode  = rate_info->PhyMode;
      vendor_sta_info->RateCbwMHz   = rate_info->CbwMHz;
      vendor_sta_info->RateMcs      = rate_info->Mcs;
      vendor_sta_info->RateNss      = rate_info->Nss;
    }
  }

  vendor_sta_info->MaxRate = sta_info->max_rate;

#undef ASSIGN_STA_INFO_PEER_RATES_INFO_PARAM
#undef ASSIGN_STA_INFO_PEER_STATS_PARAM
#undef ASSIGN_STA_INFO_PEER_TR181_STATS_PARAM
#undef ASSIGN_STA_INFO_PEER_TRAFFIC_STATS_PARAM
#undef ASSIGN_STA_INFO_PEER_RETRANS_STATS_PARAM
}

int wave_radio_sta_measurements_get (struct wiphy *wiphy,
                                     struct net_device *ndev,
                                     uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct driver_sta_info *sta_info;
  struct intel_vendor_sta_info vendor_sta_info;
  uint32 sta_info_size;

  MTLK_ASSERT(NULL != wiphy);
  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_STATION_MEASUREMENTS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_STATION_MEASUREMENTS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  sta_info = mtlk_clpb_enum_get_next(clpb, &sta_info_size);
  MTLK_CLPB_TRY(sta_info, sta_info_size)
    _wave_radio_fill_sta_info(&vendor_sta_info, sta_info);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &vendor_sta_info, sizeof(vendor_sta_info));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static void _wave_radio_fill_vap_info(struct intel_vendor_vap_info* vendor_vap_info,
  struct driver_vap_info* vap_info)
{
#define ASSIGN_VAP_INFO_PARAM(name)                                 \
  do {                                                              \
    vendor_vap_info->name = vap_info->name;                         \
  } while (0)
#define ASSIGN_VAP_INFO_VAP_STATS_PARAM(name)                       \
  do {                                                              \
    vendor_vap_info->name =                                         \
      vap_info->vap_stats.name;                                     \
  } while (0)
#define ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(name)                   \
  do {                                                              \
    vendor_vap_info->traffic_stats.name =                           \
      vap_info->vap_stats.traffic_stats.name;                       \
  } while (0)
#define ASSIGN_VAP_INFO_ERROR_STATS_PARAM(name)                     \
  do {                                                              \
    vendor_vap_info->error_stats.name =                             \
      vap_info->vap_stats.error_stats.name;                         \
  } while (0)

#define ASSIGN_VAP_INFO_RETRANS_STATS_PARAM(name)                   \
  do {                                                              \
    vendor_vap_info->name =                                         \
      vap_info->vap_stats.retrans_stats.name;                       \
  } while (0)

  MTLK_ASSERT(vendor_vap_info != NULL);
  MTLK_ASSERT(vap_info != NULL);

  memset(vendor_vap_info, 0, sizeof(*vendor_vap_info));
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(BytesSent);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(BytesReceived);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(PacketsSent);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(PacketsReceived);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(UnicastPacketsSent);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(UnicastPacketsReceived);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(MulticastPacketsSent);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(MulticastPacketsReceived);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(BroadcastPacketsSent);
  ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM(BroadcastPacketsReceived);

  ASSIGN_VAP_INFO_ERROR_STATS_PARAM(ErrorsSent);
  ASSIGN_VAP_INFO_ERROR_STATS_PARAM(ErrorsReceived);
  ASSIGN_VAP_INFO_ERROR_STATS_PARAM(DiscardPacketsSent);
  ASSIGN_VAP_INFO_ERROR_STATS_PARAM(DiscardPacketsReceived);

  /* Excluding unavailable Retransmissions */
  ASSIGN_VAP_INFO_RETRANS_STATS_PARAM(RetransCount);
  ASSIGN_VAP_INFO_RETRANS_STATS_PARAM(FailedRetransCount);
  ASSIGN_VAP_INFO_RETRANS_STATS_PARAM(RetryCount);
  ASSIGN_VAP_INFO_RETRANS_STATS_PARAM(MultipleRetryCount);

  ASSIGN_VAP_INFO_VAP_STATS_PARAM(ACKFailureCount);
  ASSIGN_VAP_INFO_VAP_STATS_PARAM(AggregatedPacketCount);
  ASSIGN_VAP_INFO_VAP_STATS_PARAM(UnknownProtoPacketsReceived);

  ASSIGN_VAP_INFO_PARAM(TransmittedOctetsInAMSDUCount);
  ASSIGN_VAP_INFO_PARAM(ReceivedOctetsInAMSDUCount);
  ASSIGN_VAP_INFO_PARAM(TransmittedOctetsInAMPDUCount);
  ASSIGN_VAP_INFO_PARAM(ReceivedOctetsInAMPDUCount);
  ASSIGN_VAP_INFO_PARAM(RTSSuccessCount);
  ASSIGN_VAP_INFO_PARAM(RTSFailureCount);
  ASSIGN_VAP_INFO_PARAM(TransmittedAMSDUCount);
  ASSIGN_VAP_INFO_PARAM(FailedAMSDUCount);
  ASSIGN_VAP_INFO_PARAM(AMSDUAckFailureCount);
  ASSIGN_VAP_INFO_PARAM(ReceivedAMSDUCount);
  ASSIGN_VAP_INFO_PARAM(TransmittedAMPDUCount);
  ASSIGN_VAP_INFO_PARAM(TransmittedMPDUsInAMPDUCount);
  ASSIGN_VAP_INFO_PARAM(AMPDUReceivedCount);
  ASSIGN_VAP_INFO_PARAM(MPDUInReceivedAMPDUCount);
  ASSIGN_VAP_INFO_PARAM(ImplicitBARFailureCount);
  ASSIGN_VAP_INFO_PARAM(ExplicitBARFailureCount);
  ASSIGN_VAP_INFO_PARAM(TwentyMHzFrameTransmittedCount);
  ASSIGN_VAP_INFO_PARAM(FortyMHzFrameTransmittedCount);
  ASSIGN_VAP_INFO_PARAM(SwitchChannel20To40);
  ASSIGN_VAP_INFO_PARAM(SwitchChannel40To20);
  ASSIGN_VAP_INFO_PARAM(FrameDuplicateCount);

#undef ASSIGN_VAP_INFO_PARAM
#undef ASSIGN_VAP_INFO_VAP_STATS_PARAM
#undef ASSIGN_VAP_INFO_TRAFFIC_STATS_PARAM
#undef ASSIGN_VAP_INFO_ERROR_STATS_PARAM
#undef ASSIGN_VAP_INFO_RETRANS_STATS_PARAM
}

int wave_radio_vap_measurements_get (struct wiphy *wiphy,
                                     struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct driver_vap_info *vap_info;
  struct intel_vendor_vap_info vendor_vap_info;
  uint32 vap_info_size;

  MTLK_ASSERT(NULL != wiphy);
  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_VAP_MEASUREMENTS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_VAP_MEASUREMENTS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  vap_info = mtlk_clpb_enum_get_next(clpb, &vap_info_size);
  MTLK_CLPB_TRY(vap_info, vap_info_size)
    _wave_radio_fill_vap_info(&vendor_vap_info, vap_info);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &vendor_vap_info, sizeof(vendor_vap_info));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static void _wave_radio_fill_radio_info(struct intel_vendor_radio_info* vendor_radio_info,
  struct driver_radio_info* radio_info)
{
#define ASSIGN_RADIO_INFO_PARAM(name)                                   \
  do {                                                                  \
    vendor_radio_info->name = radio_info->name;                         \
  } while (0)
#define ASSIGN_RADIO_INFO_HW_PARAM(name)                                \
  do {                                                                  \
    vendor_radio_info->name = radio_info->tr181_hw.name;             \
  } while (0)
#define ASSIGN_RADIO_INFO_HW_STATS_PARAM(name)                          \
  do {                                                                  \
    vendor_radio_info->name = radio_info->tr181_hw_stats.name; \
  } while (0)
#define ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(name)                     \
  do {                                                                  \
    vendor_radio_info->traffic_stats.name =                    \
      radio_info->tr181_hw_stats.traffic_stats.name;                    \
  } while (0)
#define ASSIGN_RADIO_INFO_ERROR_STATS_PARAM(name)                       \
  do {                                                                  \
    vendor_radio_info->error_stats.name =                      \
      radio_info->tr181_hw_stats.error_stats.name;                      \
  } while (0)

  MTLK_ASSERT(vendor_radio_info != NULL);
  MTLK_ASSERT(radio_info != NULL);

  memset(vendor_radio_info, 0, sizeof(*vendor_radio_info));
  ASSIGN_RADIO_INFO_HW_PARAM(Enable);
  ASSIGN_RADIO_INFO_HW_PARAM(Channel);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(BytesSent);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(BytesReceived);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(PacketsSent);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(PacketsReceived);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(UnicastPacketsSent);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(UnicastPacketsReceived);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(MulticastPacketsSent);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(MulticastPacketsReceived);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(BroadcastPacketsSent);
  ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM(BroadcastPacketsReceived);
  ASSIGN_RADIO_INFO_ERROR_STATS_PARAM(ErrorsSent);
  ASSIGN_RADIO_INFO_ERROR_STATS_PARAM(ErrorsReceived);
  ASSIGN_RADIO_INFO_ERROR_STATS_PARAM(DiscardPacketsSent);
  ASSIGN_RADIO_INFO_ERROR_STATS_PARAM(DiscardPacketsReceived);
  ASSIGN_RADIO_INFO_HW_STATS_PARAM(FCSErrorCount);
  ASSIGN_RADIO_INFO_HW_STATS_PARAM(Noise);
  ASSIGN_RADIO_INFO_PARAM(tsf_start_time);
  ASSIGN_RADIO_INFO_PARAM(load);
  ASSIGN_RADIO_INFO_PARAM(tx_pwr_cfg);
  ASSIGN_RADIO_INFO_PARAM(num_tx_antennas);
  ASSIGN_RADIO_INFO_PARAM(num_rx_antennas);
  ASSIGN_RADIO_INFO_PARAM(primary_center_freq);
  ASSIGN_RADIO_INFO_PARAM(center_freq1);
  ASSIGN_RADIO_INFO_PARAM(center_freq2);
  ASSIGN_RADIO_INFO_PARAM(width);

#undef ASSIGN_RADIO_INFO_PARAM
#undef ASSIGN_RADIO_INFO_HW_PARAM
#undef ASSIGN_RADIO_INFO_HW_STATS_PARAM
#undef ASSIGN_RADIO_INFO_TRAFFIC_STATS_PARAM
#undef ASSIGN_RADIO_INFO_ERROR_STATS_PARAM
}

int wave_radio_radio_info_get (struct wiphy *wiphy,
                               struct wireless_dev *wdev)
{
  struct net_device *ndev;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct driver_radio_info *radio_info;
  struct intel_vendor_radio_info vendor_radio_info;
  uint32 radio_info_size;

  MTLK_ASSERT(NULL != wiphy);
  MTLK_ASSERT(NULL != wdev);

  df_user = mtlk_df_user_from_wdev(wdev);
  ndev = wdev->netdev;
  MTLK_ASSERT(NULL != ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user),
            WAVE_RADIO_REQ_GET_RADIO_INFO, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
            WAVE_RADIO_REQ_GET_RADIO_INFO, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  radio_info = mtlk_clpb_enum_get_next(clpb, &radio_info_size);
  MTLK_CLPB_TRY(radio_info, radio_info_size)
    _wave_radio_fill_radio_info(&vendor_radio_info, radio_info);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &vendor_radio_info, sizeof(vendor_radio_info));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_unconnected_sta_get (struct wiphy *wiphy, struct wireless_dev *wdev,
                                    struct intel_vendor_unconnected_sta_req_cfg *sta_req_data)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct vendor_unconnected_sta_req_data_internal sta_req_data_internal;
  struct intel_vendor_unconnected_sta *sta_res_data_out;
  uint32 size;

  MTLK_ASSERT(NULL != wdev);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);



  if (!ieee_addr_is_valid(&sta_req_data->addr)) {
    ELOG_Y("Invalid MAC address %Y", &sta_req_data->addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  ILOG2_SY("%s: get unconnected station %Y", wdev->netdev->name, &sta_req_data->addr.au8Addr);

  memset(&sta_req_data_internal, 0, sizeof(sta_req_data_internal));
  sta_req_data_internal.addr = sta_req_data->addr;
  sta_req_data_internal.center_freq = sta_req_data->freq;
  sta_req_data_internal.cf1 = sta_req_data->center_freq1;
  sta_req_data_internal.cf2 = sta_req_data->center_freq2;
  sta_req_data_internal.chan_width = sta_req_data->bandwidth;
  sta_req_data_internal.wdev = wdev;
  sta_req_data_internal.req_type = sta_req_data->req_type;

  if(sta_req_data_internal.req_type == NASTA_STATS_REQ_ASYNC) {
    _mtlk_df_user_invoke_core_async(mtlk_df_user_get_master_df(df_user), WAVE_CORE_REQ_GET_UNCONNECTED_STATION,
                                    &sta_req_data_internal, sizeof(sta_req_data_internal), NULL, 0);
    ILOG1_S("%s: get unconnected station async request", wdev->netdev->name);
    goto end;
  }

  if(sta_req_data_internal.req_type != NASTA_STATS_REQ_SYNC) {
    ELOG_SD("%s: bad req_type (%u)", wdev->netdev->name, sta_req_data_internal.req_type);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  ILOG1_S("%s: get unconnected station sync request", wdev->netdev->name);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_UNCONNECTED_STATION, &clpb, &sta_req_data_internal, sizeof(sta_req_data_internal));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_UNCONNECTED_STATION, FALSE);
  
  if (res != MTLK_ERR_OK)
    goto end;

  sta_res_data_out = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY(sta_res_data_out, size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, sta_res_data_out, sizeof(*sta_res_data_out));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_set_atf_quotas (struct wiphy *wiphy,
                               struct net_device *ndev,
                               const void *data,
                               int total_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  uint8 *set_atf_result;
  uint32 ret_code_size;
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG1_SSDD("%s: SET_ATF_QUOTAS invoked from %s (%i), msg_len = %d", ndev->name,
    current->comm, current->pid, total_len);
  MTLK_CHECK_DF_USER(df_user);

  /* Invoke core to process the message further */
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
          WAVE_CORE_REQ_SET_ATF_QUOTAS, &clpb, data, total_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
          WAVE_CORE_REQ_SET_ATF_QUOTAS, FALSE);
  if(res != MTLK_ERR_OK)
    goto end;

  set_atf_result = mtlk_clpb_enum_get_next(clpb, &ret_code_size);
  MTLK_CLPB_TRY(set_atf_result, ret_code_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, set_atf_result, sizeof(*set_atf_result));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_set_wds_wpa_sta (struct net_device *ndev,
                                struct intel_vendor_mac_addr_list_cfg *addrlist_cfg)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!ieee_addr_is_valid(&addrlist_cfg->addr)) {
    ELOG_Y("Invalid MAC address %Y", addrlist_cfg->addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SET_WDS_WPA_LIST_ENTRY, &clpb, addrlist_cfg,
    sizeof(*addrlist_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SET_WDS_WPA_LIST_ENTRY, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

uint8 wave_radio_last_pm_freq_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->last_pm_freq;
}

void wave_radio_last_pm_freq_set(wave_radio_t *radio, uint8 last_pm_freq)
{
  MTLK_ASSERT(NULL != radio);
  radio->last_pm_freq = last_pm_freq;
}

mtlk_pdb_t *wave_radio_param_db_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->param_db;
}

unsigned wave_radio_scan_is_running(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return is_scan_running(&radio->scan_support);
}

unsigned wave_radio_bg_scan_is_running(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return is_bg_scan_running(&radio->scan_support);
}

unsigned wave_radio_bg_scan_on_break(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return is_bg_break(&radio->scan_support);
}

unsigned wave_radio_scan_is_ignorant(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return is_scan_ignorant(&radio->scan_support);
}

uint32 wave_radio_scan_flags_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->scan_support.flags;
}

mtlk_scan_support_t* wave_radio_scan_support_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return &radio->scan_support;
}

struct mtlk_chan_def* wave_radio_chandef_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return &radio->current_chandef;
}

struct mtlk_chan_def* wave_radio_zwdfs_chandef_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return &radio->zwdfs_ant_chandef;
}

int wave_radio_chan_switch_type_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->chan_switch_type;
}

void wave_radio_chan_switch_type_set(wave_radio_t *radio, int value)
{
  MTLK_ASSERT(NULL != radio);
  radio->chan_switch_type = value;
}

UMI_HDK_CONFIG *wave_radio_hdkconfig_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return &radio->current_hdkconfig;
}

mtlk_coc_t *wave_radio_coc_mgmt_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->coc_mgmt;
}

mtlk_erp_t *wave_radio_erp_mgmt_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->erp_mgmt;
}

void __MTLK_IFUNC
wave_radio_interfdet_set (wave_radio_t *radio, BOOL enable_flag, int8 threshold)
{
  MTLK_ASSERT(NULL != radio);
  radio->is_interfdet_enabled = enable_flag;
  if (enable_flag)
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_INTERFDET_THRESHOLD, threshold);
}

BOOL wave_radio_interfdet_get(wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->is_interfdet_enabled;
}

BOOL __MTLK_IFUNC
wave_radio_get_sta_vifs_exist(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);

  return wv_mac80211_get_sta_vifs_exist(radio->mac80211);
}

void wave_radio_sta_cnt_inc (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  atomic_inc(&radio->sta_cnt);
}

void wave_radio_sta_cnt_dec (wave_radio_t *radio)
{
  int res;

  MTLK_ASSERT(radio != NULL);
  res = atomic_dec_return(&radio->sta_cnt);

  MTLK_ASSERT(res >= 0);
  MTLK_UNREFERENCED_PARAM(res);
}

int wave_radio_sta_cnt_get (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return atomic_read(&radio->sta_cnt);
}

/* radio mode configuration */
uint32 wave_radio_mode_get(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_MODE_CURRENT);
}

void wave_radio_mode_set(wave_radio_t *radio, const uint32 radio_mode)
{
  MTLK_ASSERT(radio != NULL);
  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_MODE_REQUESTED, radio_mode);
}

wv_mac80211_t * __MTLK_IFUNC
wave_radio_mac80211_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->mac80211;
}

void __MTLK_IFUNC
wave_radio_recovery_deinit_vifs (wave_radio_t *radio)
{
  return wv_mac80211_recovery_deinit_vifs(radio->mac80211);
}

void __MTLK_IFUNC
wave_radio_recovery_init_vif (wave_radio_t *radio, u8 vap_index)
{
  if (!wv_mac80211_iface_get_is_initialized(radio->mac80211, vap_index))
    wv_mac80211_iface_set_initialized(radio->mac80211, vap_index);
  return;
}

struct ieee80211_hw * __MTLK_IFUNC
wave_radio_ieee80211_hw_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);

  return wv_ieee80211_hw_get(radio->mac80211);
}

struct  _wave_radio_t * __MTLK_IFUNC
wave_radio_descr_wave_radio_get (wave_radio_descr_t *radio_descr, unsigned idx)
{
  MTLK_ASSERT(idx < radio_descr->num_radios);
  if (idx < radio_descr->num_radios) {
    return &radio_descr->radio[idx];
  } else {
    return NULL;
  }
}

int __MTLK_IFUNC wave_radio_preamble_punct_cca_ov_cfg_get (wave_radio_t *radio, UMI_PREAMBLE_PUNCT_CCA_OVERRIDE *req)
{
  mtlk_pdb_size_t size = sizeof(*req);

  MTLK_ASSERT(radio);
  MTLK_ASSERT(req);

  if((!radio) || (!req)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  memset(req, 0, size);
  if (MTLK_ERR_OK != WAVE_RADIO_PDB_GET_BINARY(radio,
                       PARAM_DB_RADIO_PREAMBLE_PUNCTURE_CFG, req, &size)) {
    ELOG_V("Failed to get PREAMBLE_PUNCTURE_CFG from PDB");
  } else {
    MTLK_ASSERT(sizeof(*req) == size);
  }

  return MTLK_ERR_OK;
}

int __MTLK_IFUNC
wave_radio_cca_threshold_set (wave_radio_t *radio, iwpriv_cca_th_t *cca_th)
{
  int res;

  MTLK_ASSERT(radio);
  MTLK_ASSERT(cca_th);

  if((!radio) || (!cca_th)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  cca_th->is_updated = 1;

  res = WAVE_RADIO_PDB_SET_BINARY(radio, PARAM_DB_RADIO_CCA_THRESHOLD, cca_th, sizeof(*cca_th));
  if (MTLK_ERR_OK != res) {
    ELOG_V("Can't store CCA_TH in PDB");
  }

  return res;
}

int __MTLK_IFUNC
wave_radio_cca_threshold_get (wave_radio_t *radio, iwpriv_cca_th_t *cca_th)
{
  mtlk_pdb_size_t size = sizeof(*cca_th);

  MTLK_ASSERT(radio);
  MTLK_ASSERT(cca_th);

  if((!radio) || (!cca_th)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  memset(cca_th, 0, size);
  if (MTLK_ERR_OK != WAVE_RADIO_PDB_GET_BINARY(radio,
                        PARAM_DB_RADIO_CCA_THRESHOLD, cca_th, &size)) {
    ELOG_V("Failed to get CCA_TH from PDB");
  } else {
    MTLK_ASSERT(sizeof(*cca_th) == size);
  }

  return MTLK_ERR_OK;
}

BOOL __MTLK_IFUNC
wave_radio_progmodel_loaded_get (struct _wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);

  return radio->progmodel_loaded;
}

void __MTLK_IFUNC
wave_radio_progmodel_loaded_set (struct _wave_radio_t *radio, BOOL is_loaded)
{
  MTLK_ASSERT(NULL != radio);

  radio->progmodel_loaded = is_loaded;
}

void wave_radio_limits_set(wave_radio_descr_t *radio_descr, wave_radio_limits_t *radio_limits)
{
  wave_radio_t *radio;
  unsigned      i;

  MTLK_ASSERT(radio_descr != NULL);
  MTLK_ASSERT(radio_limits != NULL);

  radio = &radio_descr->radio[0];

  for (i = 0; i < radio_descr->num_radios; i++, radio++) {
    /* copy structure */
    radio->limits = radio_limits[i];
    /* FIXME: set a non-zero max_vaps (temp solution) */
    if (0 == radio->limits.max_vaps) {
      radio->limits.max_vaps = 1;
    }
    ILOG0_DDDD("RadioID %u supports max VAPs %u max STAs %u master VAP id %u", i,
        radio->limits.max_vaps, radio->limits.max_stas, radio->limits.master_vap_id);
  }
}

unsigned wave_radio_max_stas_get(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  ILOG3_DD("RadioID %u max STAs %u", radio->idx, radio->limits.max_stas);
  return radio->limits.max_stas;
}

unsigned wave_radio_max_vaps_get(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  ILOG3_DD("RadioID %u max VAPs %u", radio->idx, radio->limits.max_vaps);
  return radio->limits.max_vaps;
}

unsigned wave_radio_master_vap_id_get (wave_radio_t *radio)
{
  unsigned master_vap_id;
  MTLK_ASSERT(radio != NULL);
  master_vap_id = radio->limits.master_vap_id;
  ILOG0_DD("RadioID %u master VAP id %u", radio->idx, master_vap_id);
  return master_vap_id;
}

BOOL __MTLK_IFUNC
wave_radio_is_phy_dummy (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return wave_hw_mmb_card_is_phy_dummy(radio->hw_api->hw);
}

#ifdef MTLK_WAVE_700
BOOL __MTLK_IFUNC
wave_radio_is_gen7(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  MTLK_ASSERT(radio->hw_api != NULL);
  return mtlk_hw_type_is_gen7(radio->hw_api->hw);
}

BOOL __MTLK_IFUNC
wave_radio_is_320mhz_supported(wave_radio_t *radio)
{
  return ((wave_radio_band_get(radio) == MTLK_HW_BAND_6_GHZ) &&
          wave_radio_is_gen7(radio));
}

uint8 __MTLK_IFUNC
wave_convert_radio_band_to_link_id (mtlk_hw_band_e band)
{
  MTLK_ASSERT(wave_band_is_valid(band));
  return (band == MTLK_HW_BAND_2_4_GHZ) ? LINK_ID_2G : ((band == MTLK_HW_BAND_5_2_GHZ) ? LINK_ID_5G : LINK_ID_6G);
}
#endif

#define ADD_CH_TO_TAB(ch_tab_, max_size_, id_str_,channel_) \
  if(radio->ch_tab_.num_of_chan >= max_size_) { \
    ELOG_S("No more entrie in table %s", id_str_); \
    return MTLK_ERR_PARAMS; \
  } \
  radio->ch_tab_.channel[radio->ch_tab_.num_of_chan++] = channel_;

static int _wave_radio_supported_ch_add(wave_radio_t *radio, uint8 channel, wave_radio_chan_type_t ch_type)
{
  switch (ch_type) {
    case WAVE_RADIO_CHAN_G24_20:
      ADD_CH_TO_TAB(g24_20_ch_tab, G24_20_CH_NUM_MAX, "g24_20_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G24_40:
      ADD_CH_TO_TAB(g24_40_ch_tab, G24_40_CH_NUM_MAX, "g24_40_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G52_20:
      ADD_CH_TO_TAB(g52_20_ch_tab, G52_20_CH_NUM_MAX, "g52_20_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G52_40:
      ADD_CH_TO_TAB(g52_40_ch_tab, G52_40_CH_NUM_MAX, "g52_40_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G52_80:
      ADD_CH_TO_TAB(g52_80_ch_tab, G52_80_CH_NUM_MAX, "g52_80_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G52_160:
      ADD_CH_TO_TAB(g52_160_ch_tab, G52_160_CH_NUM_MAX, "g52_160_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G6_20:
      ADD_CH_TO_TAB(g6_20_ch_tab, G6_20_CH_NUM_MAX, "g6_20_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G6_40:
      ADD_CH_TO_TAB(g6_40_ch_tab, G6_40_CH_NUM_MAX, "g6_40_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G6_80:
      ADD_CH_TO_TAB(g6_80_ch_tab, G6_80_CH_NUM_MAX, "g6_80_ch_tab", channel);
      break;
    case WAVE_RADIO_CHAN_G6_160:
      ADD_CH_TO_TAB(g6_160_ch_tab, G6_160_CH_NUM_MAX, "g6_160_ch_tab", channel);
      break;
#ifdef MTLK_WAVE_700
    case WAVE_RADIO_CHAN_G7_320:
      if (wave_radio_is_320mhz_supported(radio)) {
        ADD_CH_TO_TAB(g7_320_ch_tab, G7_320_CH_NUM_MAX, "g7_320_ch_tab", channel);
      } else {
        ELOG_D("Invalid Channel Type %d", ch_type);
        return MTLK_ERR_PARAMS;
      }
      break;
#endif
    default:
      ELOG_D("Wrong chan_type %d", ch_type);
      return MTLK_ERR_PARAMS;
  }

  return MTLK_ERR_OK;
}

/* If enabled - calibrate only regulatory allowed channels */
#define WAVE_REGULATORY_CHANNEL_CHECK 0

#if WAVE_REGULATORY_CHANNEL_CHECK
#define SKIP_CHANNEL(ch_, search_tab_, idx_, max_size_) \
  if(idx_ <= max_size_) { \
    if(ch_ == search_tab_[idx_]) { \
      idx_++; \
    } \
  }
#endif /* WAVE_REGULATORY_CHANNEL_CHECK */

int wave_radio_channel_table_build_2ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels)
{
  int res = MTLK_ERR_OK;
  uint8 ch_num;
  wave_int i, ch20_idx, ch40_idx;

  MTLK_ASSERT(NULL != radio);
  MTLK_ASSERT(NULL != channels);
  if (0 == n_channels) {
    ELOG_V("No channels");
    return MTLK_ERR_PARAMS;
  }

  ch20_idx = ch40_idx = 0;

  for (i = 0; i < n_channels; i++, channels++) {
    /* freq to channel number */
    ch_num = ieee80211_frequency_to_channel(channels->center_freq);
    if (0 == ch_num) {
      ELOG_D("ch_num is 0 for center_freq %d", channels->center_freq);
      res = MTLK_ERR_PARAMS;
      break;
    }
    /* skip disabled channel */
    if (channels->flags & IEEE80211_CHAN_DISABLED) {
      ILOG0_D("channel %d disabled by regulatory", ch_num);
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* rewind search tables in case of match */
      SKIP_CHANNEL(ch_num, g24_20_supported_ch, ch20_idx, G24_20_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num, g24_40_supported_ch, ch40_idx, G24_40_CH_NUM_MAX);
      continue;
#endif
    }

    /* add 20MHz channel */
    if ((ch20_idx < G24_20_CH_NUM_MAX) && (ch_num == g24_20_supported_ch[ch20_idx])) {
      ch20_idx++;
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G24_20);
      if (res != MTLK_ERR_OK)
        break;
    }
    /* add 40MHz*/
    if ((ch40_idx < G24_40_CH_NUM_MAX) && (ch_num == g24_40_supported_ch[ch40_idx])) {
      ch40_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      if (IEEE80211_CHAN_NO_HT40 != (channels->flags & IEEE80211_CHAN_NO_HT40)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G24_40);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G24_40);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }
  }

  return res;
}

int wave_radio_channel_table_build_5ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels)
{
  int res = MTLK_ERR_OK;
  uint8 ch_num;
  wave_int i, ch20_idx, ch40_idx, ch80_idx, ch160_idx;

  MTLK_ASSERT(NULL != radio);
  MTLK_ASSERT(NULL != channels);
  if (0 == n_channels) {
    ELOG_V("No channels");
    return MTLK_ERR_PARAMS;
  }

  ch20_idx = ch40_idx = ch80_idx = ch160_idx = 0;

  for (i = 0; i < n_channels; i++, channels++) {
    /* freq to channel number */
    ch_num = ieee80211_frequency_to_channel(channels->center_freq);
    if (0 == ch_num) {
      ELOG_D("ch_num is 0 for center_freq %d", channels->center_freq);
      res = MTLK_ERR_PARAMS;
      break;
    }
    /* skip disabled channel */
    if (channels->flags & IEEE80211_CHAN_DISABLED) {
      ILOG0_D("channel %d disabled by regulatory", ch_num);
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* rewind search tables in case of match */
      SKIP_CHANNEL(ch_num,  g52_20_supported_ch,  ch20_idx,  G52_20_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num,  g52_40_supported_ch,  ch40_idx,  G52_40_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num,  g52_80_supported_ch,  ch80_idx,  G52_80_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num, g52_160_supported_ch, ch160_idx, G52_160_CH_NUM_MAX);
#endif
      continue;
    }

    /* add 20MHz channel */
    if ((ch20_idx < G52_20_CH_NUM_MAX) && (ch_num == g52_20_supported_ch[ch20_idx])) {
      ch20_idx++;
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_20);
      if (res != MTLK_ERR_OK)
        break;
    }
    /* add 40MHz*/
    if ((ch40_idx < G52_40_CH_NUM_MAX) && (ch_num == g52_40_supported_ch[ch40_idx])) {
      ch40_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      if (IEEE80211_CHAN_NO_HT40 != (channels->flags & IEEE80211_CHAN_NO_HT40)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_40);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_40);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }
    /* add 80MHz*/
    if ((ch80_idx < G52_80_CH_NUM_MAX) && (ch_num == g52_80_supported_ch[ch80_idx])) {
      ch80_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      if (0 == (channels->flags & IEEE80211_CHAN_NO_80MHZ)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_80);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_80);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }

    if ((ch160_idx < G52_160_CH_NUM_MAX) && (ch_num == g52_160_supported_ch[ch160_idx])) {
      ch160_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      if (0 == (channels->flags & IEEE80211_CHAN_NO_160MHZ)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_160);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G52_160);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }
  }

  return res;
}

int wave_radio_channel_table_build_6ghz(wave_radio_t *radio, struct ieee80211_channel *channels, int n_channels)
{
  int res = MTLK_ERR_OK;
  uint8 ch_num;
  wave_int i, ch20_idx, ch40_idx, ch80_idx, ch160_idx;
#ifdef MTLK_WAVE_700
  wave_int ch320_idx = 0;
  BOOL is_320mhz_supported = wave_radio_is_320mhz_supported(radio);
#endif

  MTLK_ASSERT(NULL != radio);
  MTLK_ASSERT(NULL != channels);
  if (0 == n_channels) {
    ELOG_V("No channels");
    return MTLK_ERR_PARAMS;
  }

  ch20_idx = ch40_idx = ch80_idx = ch160_idx = 0;

  for (i = 0; i < n_channels; i++, channels++) {
    /* freq to channel number */
    ch_num = ieee80211_frequency_to_channel(channels->center_freq);
    if (0 == ch_num) {
      ELOG_D("ch_num is 0 for center_freq %d", channels->center_freq);
      res = MTLK_ERR_PARAMS;
      break;
    }
    /* skip disabled channel */
    if (channels->flags & IEEE80211_CHAN_DISABLED) {
      ILOG0_D("channel %d disabled by regulatory", ch_num);
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* rewind search tables in case of match */
      SKIP_CHANNEL(ch_num,  g6_20_supported_ch,  ch20_idx,  G6_20_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num,  g6_40_supported_ch,  ch40_idx,  G6_40_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num,  g6_80_supported_ch,  ch80_idx,  G6_80_CH_NUM_MAX);
      SKIP_CHANNEL(ch_num, g6_160_supported_ch, ch160_idx, G6_160_CH_NUM_MAX);
#ifdef MTLK_WAVE_700
      if (is_320mhz_supported) {
        SKIP_CHANNEL(ch_num, g7_320_supported_ch, ch320_idx, G7_320_CH_NUM_MAX);
      }
#endif /*MTLK_WAVE_700*/
#endif
      continue;
    }

    /* add 20MHz channel */
    if ((ch20_idx < G6_20_CH_NUM_MAX) && (ch_num == g6_20_supported_ch[ch20_idx])) {
      ch20_idx++;
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_20);
      if (res != MTLK_ERR_OK)
        break;
    }
    /* add 40MHz*/
    if ((ch40_idx < G6_40_CH_NUM_MAX) && (ch_num == g6_40_supported_ch[ch40_idx])) {
      ch40_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* TODO: Confirm about the flag setting for 6GHz band */
      if (IEEE80211_CHAN_NO_HT40 != (channels->flags & IEEE80211_CHAN_NO_HT40)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_40);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_40);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }
    /* add 80MHz*/
    if ((ch80_idx < G6_80_CH_NUM_MAX) && (ch_num == g6_80_supported_ch[ch80_idx])) {
      ch80_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
     /* TODO: Confirm about the flag setting for 6GHz band */

      if (0 == (channels->flags & IEEE80211_CHAN_NO_80MHZ)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_80);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_80);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }

    if ((ch160_idx < G6_160_CH_NUM_MAX) && (ch_num == g6_160_supported_ch[ch160_idx])) {
      ch160_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* TODO: Confirm about the flag setting for 6GHz band */

      if (0 == (channels->flags & IEEE80211_CHAN_NO_160MHZ)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_160);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G6_160);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }

#ifdef MTLK_WAVE_700
    if (is_320mhz_supported &&
        (ch320_idx < G7_320_CH_NUM_MAX) && (ch_num == g7_320_supported_ch[ch320_idx])) {
      ch320_idx++;
#if WAVE_REGULATORY_CHANNEL_CHECK
      /* TODO: Confirm about the flag setting for 6GHz band */

      if (0 == (channels->flags & IEEE80211_CHAN_NO_320MHZ)) {
        res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G7_320);
        if (res != MTLK_ERR_OK)
          break;
      }
#else
      res = _wave_radio_supported_ch_add(radio, ch_num, WAVE_RADIO_CHAN_G7_320);
      if (res != MTLK_ERR_OK)
        break;
#endif
    }
#endif /*MTLK_WAVE_700*/
  }
  ILOG0_DDDD("num_of_chans: 20MHz = %d, 40MHz = %d, 80MHz = %d, 160MHz = %d",radio->g6_20_ch_tab.num_of_chan,radio->g6_40_ch_tab.num_of_chan,radio->g6_80_ch_tab.num_of_chan,radio->g6_160_ch_tab.num_of_chan);

#ifdef MTLK_WAVE_700
  if (is_320mhz_supported)
    ILOG0_D("num_of_chans: 320MHz = %d", radio->g7_320_ch_tab.num_of_chan);
#endif

  return res;
}


static void
_wave_radio_channel_table_print (const char *name, uint8 *channels, uint8 num_of_chan)
{
  if (num_of_chan) {
    ILOG1_SD("%s: %d", name, num_of_chan);
    mtlk_dump(2, channels, num_of_chan, "channels");
  }
}

void wave_radio_channel_table_print(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);

  ILOG1_D("RadioID:%u", radio->idx);

#define _PRINT_CHANNEL_TABLE_(name, table) \
          _wave_radio_channel_table_print(name, table.channel, table.num_of_chan)

  _PRINT_CHANNEL_TABLE_("2GHz channels supported 20MHz",  radio->g24_20_ch_tab);
  _PRINT_CHANNEL_TABLE_("2GHz channels supported 40MHz",  radio->g24_40_ch_tab);
  _PRINT_CHANNEL_TABLE_("5GHz channels supported 20MHz",  radio->g52_20_ch_tab);
  _PRINT_CHANNEL_TABLE_("5GHz channels supported 40MHz",  radio->g52_40_ch_tab);
  _PRINT_CHANNEL_TABLE_("5GHz channels supported 80MHz",  radio->g52_80_ch_tab);
  _PRINT_CHANNEL_TABLE_("5GHz channels supported 160MHz", radio->g52_160_ch_tab);
  _PRINT_CHANNEL_TABLE_("6GHz channels supported 20MHz",  radio->g6_20_ch_tab);
  _PRINT_CHANNEL_TABLE_("6GHz channels supported 40MHz",  radio->g6_40_ch_tab);
  _PRINT_CHANNEL_TABLE_("6GHz channels supported 80MHz",  radio->g6_80_ch_tab);
  _PRINT_CHANNEL_TABLE_("6GHz channels supported 160MHz",  radio->g6_160_ch_tab);
#ifdef MTLK_WAVE_700
  if (wave_radio_is_320mhz_supported(radio))
    _PRINT_CHANNEL_TABLE_("6GHz channels supported 320MHz",  radio->g7_320_ch_tab);
#endif

#undef _PRINT_CHANNEL_TABLE_
}

/* FIXME: prototype */
int __MTLK_IFUNC    mtlk_efuse_eeprom_load_to_hw (mtlk_hw_t *hw, mtlk_txmm_t *txmm, mtlk_hw_band_e band);
int __MTLK_IFUNC    mtlk_psdb_load_to_hw(mtlk_hw_t *hw, mtlk_txmm_t *txmm);
int __MTLK_IFUNC    mtlk_hw_send_hdk_antennas_config(mtlk_hw_t *hw);

static int
_wave_radio_hdk_config_send(wave_radio_t *radio, mtlk_txmm_t *txmm, uint32 offline_mask, uint32 online_mask)
{
  UMI_HDK_CONFIG   *current_hdkconfig = &radio->current_hdkconfig;
  UMI_HDK_CONFIG    updated_hdkconfig;
  UMI_HDK_CONFIG   *req;
  size_t            size = sizeof(UMI_HDK_CONFIG);
  mtlk_txmm_msg_t   man_msg;
  mtlk_txmm_data_t *man_entry;
  wave_int          band;
  int res = MTLK_ERR_OK;

  band = radio->wave_band;
  ILOG0_DDD("band=%u, offline_algo_mask=0x%08x, online_algo_mask=0x%08x",
            band, offline_mask, online_mask);

  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, &res)))
  {
    ELOG_D("UM_MAN_HDK_CONFIG_REQ init failed, err=%i", res);
    res = MTLK_ERR_NO_RESOURCES;
    goto end;
  }

  man_entry->id = UM_MAN_HDK_CONFIG_REQ;
  man_entry->payload_size = size;
  req = (UMI_HDK_CONFIG *)man_entry->payload;

  /* copy everything from our saved copy */
  *req = *current_hdkconfig; /* struct copy */

  /* change the things that can dynamically change */
  req->hdkConf.band = band;
  req->hdkConf.calibrationMasks.offlineCalMask = HOST_TO_MAC32(offline_mask);
  req->hdkConf.calibrationMasks.onlineCalMask  = HOST_TO_MAC32(online_mask);

  /* copy updated config before sending */
  updated_hdkconfig = *req; /* struct copy */

  SLOG0(0, 0, UMI_HDK_CONFIG, req);
  mtlk_dump(0, req, size, "dump of HDK_CONFIG_REQ");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
    ELOG_DD("Set UM_MAN_HDK_CONFIG_REQ failed, res=%i status=%hhu",
            res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
  }
  else {
    /* we succeeded, so just save our new settings */
    *current_hdkconfig = updated_hdkconfig; /* struct copy */
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_CALIBRATION_ALGO_MASK, offline_mask);
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_ONLINE_ACM, online_mask);
  }

  mtlk_txmm_msg_cleanup(&man_msg);

end:
  return res;
}

int __MTLK_IFUNC
wave_radio_send_hdk_config(wave_radio_t *radio, uint32 offline_mask, uint32 online_mask)
{
  MTLK_ASSERT(radio);

  if (radio == NULL) {
    return MTLK_ERR_PARAMS;
  }

  return _wave_radio_hdk_config_send(radio, _wave_radio_txmm_get(radio), offline_mask, online_mask);
}


int __MTLK_IFUNC
wave_radio_read_hdk_config(wave_radio_t *radio, uint32 *offline_mask, uint32 *online_mask)
{
  mtlk_txmm_t      *txmm;
  UMI_HDK_CONFIG   *req;
  mtlk_txmm_msg_t   man_msg;
  mtlk_txmm_data_t *man_entry;
  size_t            size = sizeof(UMI_HDK_CONFIG);
  int               res;

  MTLK_ASSERT(radio);

  if (radio == NULL) {
    return MTLK_ERR_PARAMS;
  }

  txmm = _wave_radio_txmm_get(radio);
  MTLK_ASSERT(txmm);
  if (!txmm) {
    return MTLK_ERR_PARAMS;
  }

  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, &res)))
  {
    ELOG_D("UM_MAN_HDK_CONFIG_REQ init failed, err=%i", res);
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_HDK_CONFIG_REQ;
  man_entry->payload_size = size;
  req = (UMI_HDK_CONFIG *)man_entry->payload;
  req->hdkConf.band = radio->wave_band;
  req->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
    ELOG_DD("Receive UM_MAN_HDK_CONFIG_REQ failed, res=%i status=%hhu",
            res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
  }
  else {
    *offline_mask = MAC_TO_HOST32(req->hdkConf.calibrationMasks.offlineCalMask);
    *online_mask  = MAC_TO_HOST32(req->hdkConf.calibrationMasks.onlineCalMask);
    ILOG2_DDD("RadioID %u: online mask 0x%x offline mask 0x%x", radio->idx, *online_mask, *offline_mask);
  }
  mtlk_txmm_msg_cleanup(&man_msg);

  return res;
}

static __INLINE BOOL
__wave_radio_is_first (wave_radio_t *radio)
{
  return wave_radio_id_is_first(radio->idx);
}

BOOL __MTLK_IFUNC
wave_radio_is_first (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return __wave_radio_is_first(radio);
}

static int _wave_radio_dummy_vap_add (wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  int res = MTLK_ERR_OK;
  mtlk_txmm_data_t* man_entry = NULL;
  mtlk_txmm_msg_t man_msg;
  UMI_ADD_VAP *req;

  ILOG2_V("Adding VAP");
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, &res);
  if (man_entry == NULL)
  {
    ELOG_V("Can't send UMI_ADD_VAP request to MAC due to the lack of MAN_MSG");
    return res;
  }

  man_entry->id = UM_MAN_ADD_VAP_REQ;
  man_entry->payload_size = sizeof(*req);

  req = (UMI_ADD_VAP *)man_entry->payload;
  memset(req, 0, sizeof(*req));

  ieee_addr_set(&req->sBSSID, mtlk_eeprom_get_nic_mac_addr(radio->ee_data));
  req->sBSSID.au8Addr[ETH_ALEN - 1] += radio->idx; /* unique for every radio */
  ILOG0_Y("Mac addr for calibration VAP is %Y", &req->sBSSID);

  req->vapId          = 0;
  /*
   * At this stage no need for:
   *  u8Rates
   *  u8Rates_Length
   */
  req->operationMode  = OPERATION_MODE_NORMAL;

  mtlk_dump(2, req, sizeof(*req), "dump of UMI_ADD_VAP");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
    ELOG_DD("ADD_VAP request failed, res=%d status=%hhu", res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

  ILOG2_V("VAP added");

FINISH:
  if (man_entry)
    mtlk_txmm_msg_cleanup(&man_msg);

  return res;
}

static int _wave_radio_dummy_vap_del(wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  mtlk_txmm_msg_t man_msg;
  mtlk_txmm_data_t *man_entry = NULL;
  UMI_REMOVE_VAP *req;
  int result = MTLK_ERR_OK;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, &result);
  if (man_entry == NULL) {
    ELOG_V("Can't send UM_MAN_REMOVE_VAP_REQ due to the lack of MAN_MSG");
    goto FINISH;
  }

  man_entry->id           = UM_MAN_REMOVE_VAP_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_REMOVE_VAP *) man_entry->payload;

  req->u16Status = HOST_TO_MAC16(UMI_OK);
  req->vapId = 0;

  mtlk_dump(2, req, sizeof(*req), "dump of UMI_REMOVE_VAP:");

  result = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (result != MTLK_ERR_OK) {
    ELOG_D("Can't send UM_MAN_REMOVE_VAP_REQ (err=%d)", result);
    goto FINISH;
  }

  if (MAC_TO_HOST16(req->u16Status) != UMI_OK) {
    WLOG_D("UM_MAN_REMOVE_VAP_REQ failed in FW (status=%u)", MAC_TO_HOST16(req->u16Status));
    result = MTLK_ERR_MAC;
    goto FINISH;
  }

  ILOG0_DD("VAP is removed (status %u), result %d", MAC_TO_HOST16(req->u16Status), result);

FINISH:
  if (man_entry)
    mtlk_txmm_msg_cleanup(&man_msg);

  return result;
}

static int
_wave_radio_calibrate_prepare (wave_radio_t *radio, mtlk_txmm_t *txmm, mtlk_hw_band_e band,
                               uint32 param_algomask, uint32 param_oalgomask)
{
  mtlk_hw_t *hw;
  int res;

  MTLK_ASSERT(radio != NULL);
  MTLK_ASSERT(txmm != NULL);

  hw = radio->hw_api->hw;

  /* download progmodels */
  if (MTLK_ERR_OK != (res = wave_progmodel_load(txmm, radio, radio->wave_band, mtlk_hw_type_is_gen7(hw))))
  {
    ELOG_V("Error while downloading progmodel files");
    return res;
  }

  /* CCA thresholds */
  res = mtlk_core_cfg_send_actual_cca_threshold(
            mtlk_vap_manager_get_master_core(radio->vap_manager));
  if (MTLK_ERR_OK != res) {
    return res; /* Error already logged */
  }

  /* EFUSE/EEPROM specific */
  if (MTLK_ERR_OK != (res = mtlk_efuse_eeprom_load_to_hw(hw, txmm, radio->wave_band))) {
    return res; /* Error already logged */
  }

  /* send HDK config */
  if (MTLK_ERR_OK != (res = _wave_radio_hdk_config_send(radio, txmm, param_algomask, param_oalgomask)))
  {
    /* Error already logged */
    return res;
  }

  /* Set HDK antennas configuration message */
  /* should be send once - when first band goes up */
  if (__wave_radio_is_first(radio)) {
    if (MTLK_ERR_OK != (res = mtlk_hw_send_hdk_antennas_config(hw))) {
      /* Error already logged */
      return res;
    }
  }

  /* Send Platform Specific */
  if (MTLK_ERR_OK != (res = mtlk_psdb_load_to_hw(hw, txmm))) {
    /* Error already logged */
    return res;
  }

  return MTLK_ERR_OK;
}

static int
_wave_radio_calibrate_send (wave_radio_t *radio, mtlk_txmm_t *txmm,
                            mtlk_hw_band_e band, unsigned cw,
                            uint8 *chans, unsigned num_chans)
{
  mtlk_txmm_msg_t man_msg;
  mtlk_txmm_data_t *man_entry;
  UMI_CALIBRATE_PARAMS *req;
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(MAX_CALIB_CHANS >= num_chans);

  /* limit number of channels to calibrate */
  if (num_chans > MAX_CALIB_CHANS) {
    WLOG_DD("num_chans(%d) > MAX_CALIB_CHANS(%d)", num_chans, MAX_CALIB_CHANS);
    num_chans = MAX_CALIB_CHANS;
  }

  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, &res)))
  {
    ELOG_D("UM_MAN_CALIBRATE_REQ init failed, err=%d", res);
    res = MTLK_ERR_NO_RESOURCES;
    goto end;
  }

  man_entry->id = UM_MAN_CALIBRATE_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_CALIBRATE_PARAMS *) man_entry->payload;
  memset(req, 0, sizeof(*req));

  wave_memcpy(req->chan_nums, sizeof(req->chan_nums), chans, num_chans);
  req->num_chans = (uint8)num_chans;
  req->chan_width = (uint8)cw;

  mtlk_dump(1, req, sizeof(*req), "dump of UMI_CALIBRATE_PARAMS:");

  res = mtlk_txmm_msg_send_blocked(&man_msg,
                                   MTLK_MM_BLOCKED_CALIBRATION_TIMEOUT_GEN6);

  if (res != MTLK_ERR_OK || UMI_OK != req->Status)
  {
    ELOG_DD("UM_MAN_CALIBRATE_REQ send failed, res=%i status=%hhu", res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);

end:
  return res;
}

static void _wave_radio_calibration_status_set(wave_radio_t *radio,
                                               uint8 *channels,
                                               uint8 nof_chan,
                                               uint8 ch_width)
{
  unsigned cw_bit = 1 << ch_width;
  int i;

  for (i = 0; i < nof_chan; i++) {
    unsigned idx = (unsigned)channum2cssidx(channels[i]);
    if (idx < ARRAY_SIZE(radio->calib_done_mask)) {
      radio->calib_done_mask[idx] |= cw_bit;
    } else {
      ELOG_D("idx %d outside of array size", idx);
    }
  }
}


static void _wave_radio_calibration_status_set_6g(wave_radio_t *radio,
                                               uint8 *channels,
                                               uint8 nof_chan,
                                               uint8 ch_width)
{
  unsigned cw_bit = 1 << ch_width;
  int i;
  unsigned idx;

  for (i = 0; i < nof_chan; i++) {
    idx = channum6g2chanidx(channels[i]);
    if (idx < ARRAY_SIZE(radio->calib_done_mask_6g)) {
      radio->calib_done_mask_6g[idx] |= cw_bit;
    } 
  }
}


static int _wave_radio_2G_calibrate(wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(radio->wave_band == MTLK_HW_BAND_2_4_GHZ);
  /* 2.4 Ghz 20 Mhz channels*/
  if (radio->g24_20_ch_tab.num_of_chan) {
    ILOG0_V("20Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g24_20_ch_tab.ch_width,
                                     radio->g24_20_ch_tab.channel,
                                     radio->g24_20_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g24_20_ch_tab.channel,
                                       radio->g24_20_ch_tab.num_of_chan,
                                       CW_20);
  }

  /* 2.4 Ghz 40 Mhz channels*/
  if (radio->g24_40_ch_tab.num_of_chan) {
    ILOG0_V("40Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g24_40_ch_tab.ch_width,
                                     radio->g24_40_ch_tab.channel,
                                     radio->g24_40_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g24_40_ch_tab.channel,
                                       radio->g24_40_ch_tab.num_of_chan,
                                       CW_40);
  }

  return res;
}

static int _wave_radio_5G_calibrate(wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(radio->wave_band == MTLK_HW_BAND_5_2_GHZ);
  /* 5.2 Ghz 20 Mhz channels*/
  if (radio->g52_20_ch_tab.num_of_chan) {
    ILOG0_V("20Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g52_20_ch_tab.ch_width,
                                     radio->g52_20_ch_tab.channel,
                                     radio->g52_20_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g52_20_ch_tab.channel,
                                       radio->g52_20_ch_tab.num_of_chan,
                                       CW_20);
  }

  /* 5.2 Ghz 40 Mhz channels*/
  if (radio->g52_40_ch_tab.num_of_chan) {
    ILOG0_V("40Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g52_40_ch_tab.ch_width,
                                     radio->g52_40_ch_tab.channel,
                                     radio->g52_40_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g52_40_ch_tab.channel,
                                       radio->g52_40_ch_tab.num_of_chan,
                                       CW_40);
  }

  /* 5.2 Ghz 80 Mhz channels*/
  if (radio->g52_80_ch_tab.num_of_chan) {
    ILOG0_V("80Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g52_80_ch_tab.ch_width,
                                     radio->g52_80_ch_tab.channel,
                                     radio->g52_80_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g52_80_ch_tab.channel,
                                       radio->g52_80_ch_tab.num_of_chan,
                                       CW_80);
  }

  /* 5.2 Ghz 160 Mhz channels*/
  if (radio->g52_160_ch_tab.num_of_chan) {
    ILOG0_V("160Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g52_160_ch_tab.ch_width,
                                     radio->g52_160_ch_tab.channel,
                                     radio->g52_160_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set(radio,
                                       radio->g52_160_ch_tab.channel,
                                       radio->g52_160_ch_tab.num_of_chan,
                                       CW_160);
  }

  return res;
}


static int _wave_radio_6G_calibrate(wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(radio->wave_band == MTLK_HW_BAND_6_GHZ);
  /* 6 Ghz 20 Mhz channels*/
  if (radio->g6_20_ch_tab.num_of_chan) {
    ILOG0_V("20Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g6_20_ch_tab.ch_width,
                                     radio->g6_20_ch_tab.channel,
                                     radio->g6_20_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set_6g(radio,
                                       radio->g6_20_ch_tab.channel,
                                       radio->g6_20_ch_tab.num_of_chan,
                                       CW_20);
  }

  /* 6 Ghz 40 Mhz channels*/
  if (radio->g6_40_ch_tab.num_of_chan) {
    ILOG0_V("40Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g6_40_ch_tab.ch_width,
                                     radio->g6_40_ch_tab.channel,
                                     radio->g6_40_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set_6g(radio,
                                       radio->g6_40_ch_tab.channel,
                                       radio->g6_40_ch_tab.num_of_chan,
                                       CW_40);
  }

  /* 6 Ghz 80 Mhz channels*/
  if (radio->g6_80_ch_tab.num_of_chan) {
    ILOG0_V("80Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g6_80_ch_tab.ch_width,
                                     radio->g6_80_ch_tab.channel,
                                     radio->g6_80_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set_6g(radio,
                                       radio->g6_80_ch_tab.channel,
                                       radio->g6_80_ch_tab.num_of_chan,
                                       CW_80);
  }

  /* 6 Ghz 160 Mhz channels*/
  if (radio->g6_160_ch_tab.num_of_chan) {
    ILOG0_V("160Mhz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g6_160_ch_tab.ch_width,
                                     radio->g6_160_ch_tab.channel,
                                     radio->g6_160_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set_6g(radio,
                                       radio->g6_160_ch_tab.channel,
                                       radio->g6_160_ch_tab.num_of_chan,
                                       CW_160);
  }

#ifdef MTLK_WAVE_700
  /* 6 Ghz 320 Mhz channels*/
  if (wave_radio_is_gen7(radio) && radio->g7_320_ch_tab.num_of_chan) {
    ILOG0_V("320MHz channels");
    res = _wave_radio_calibrate_send(radio, txmm, radio->wave_band,
                                     radio->g7_320_ch_tab.ch_width,
                                     radio->g7_320_ch_tab.channel,
                                     radio->g7_320_ch_tab.num_of_chan);
    if (MTLK_ERR_OK != res)
      return res;

    _wave_radio_calibration_status_set_6g(radio,
                                       radio->g7_320_ch_tab.channel,
                                       radio->g7_320_ch_tab.num_of_chan,
                                       CW_320);
  }
#endif
  return res;
}


static int _wave_radio_calibrate (wave_radio_t *radio, mtlk_txmm_t *txmm)
{
  int res;

  if (MTLK_ERR_OK != (res = _wave_radio_dummy_vap_add(radio, txmm))) {
    return res;
  }

  if (radio->ieee_band == NL80211_BAND_2GHZ) {
    if (MTLK_ERR_OK != (res = _wave_radio_2G_calibrate(radio, txmm))) {
      return res;
    }
  }

  if (radio->ieee_band == NL80211_BAND_5GHZ) {
    if (MTLK_ERR_OK != (res = _wave_radio_5G_calibrate(radio, txmm))) {
      return res;
    }
  }

  if (radio->ieee_band == NL80211_BAND_6GHZ) {
    if (MTLK_ERR_OK != (res = _wave_radio_6G_calibrate(radio, txmm))) {
      return res;
    }
  }


  if (MTLK_ERR_OK != (res = _wave_radio_dummy_vap_del(radio, txmm))) {
    return res;
  }

  /* Make sure radio is OFF */
  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_MODE_CURRENT, WAVE_RADIO_OFF);

  return MTLK_ERR_OK;
}

int wave_radio_calibrate(wave_radio_descr_t *radio_descr, BOOL is_recovery)
{
  wave_radio_t *radio;
  mtlk_txmm_t  *txmm = NULL;
  int           res = MTLK_ERR_OK;
  unsigned      i;

  /* Pre-calibration for all radios */
  for (i = 0; i < radio_descr->num_radios; i++) {
    ILOG0_D("Processing RadioID %u", i);
    radio = &radio_descr->radio[i];
    txmm = _wave_radio_txmm_get(radio);
    MTLK_ASSERT(txmm != NULL);

    res = _wave_radio_calibrate_prepare(
            radio, txmm, radio->wave_band,
            WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_CALIBRATION_ALGO_MASK),
            WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_ONLINE_ACM));

    ILOG0_DS("Pre-calibration for RadioID %u %s",
             i, (MTLK_ERR_OK == res) ? "succeeded" : "failed");

    if (MTLK_ERR_OK != res) {
      return res;
    }
  }

  /* Calibration for all radios is not performed on Fast Recovery and for Production Mode */
  radio = &radio_descr->radio[0];
  if ((WAVE_RADIO_FAST_RCVRY == is_recovery) ||
      wave_hw_mmb_eeprom_is_production_mode(radio->hw_api->hw)) {
    return res;
  }

  for (i = 0; i < radio_descr->num_radios; i++) {
    radio = &radio_descr->radio[i];
    txmm = _wave_radio_txmm_get(radio);
    if (wave_radio_is_zwdfs_gen6_d2(radio)) {
      continue;
    }

    if (NULL == txmm)
      return MTLK_ERR_NO_RESOURCES;

    res = _wave_radio_calibrate(radio, txmm);
    ILOG0_DS("Calibration for RadioID %u %s",
             i, (MTLK_ERR_OK == res) ? "succeeded" : "failed");

    if (res != MTLK_ERR_OK) {
      return res;
    }
  }

  return res;
}

void wave_radio_band_set(wave_radio_t *radio, nl80211_band_e ieee_band)
{
  MTLK_ASSERT(radio != NULL);
  radio->ieee_band = ieee_band;
  radio->wave_band = nlband2mtlkband(ieee_band);
}

mtlk_hw_band_e wave_radio_band_get(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return radio->wave_band;
}

wave_cdb_cfg_e wave_radio_cdb_config_get(wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return radio->cdb_config;
}

void wave_radio_calibration_status_get(wave_radio_t *radio,
                                       uint8 idx,
                                       uint8 *calib_done_mask,
                                       uint8 *calib_failed_mask)
{
  MTLK_ASSERT(radio != NULL);
  if (radio->wave_band == MTLK_HW_BAND_6_GHZ) {
    *calib_done_mask = radio->calib_done_mask_6g[idx];
  } else {
    *calib_done_mask   = radio->calib_done_mask[idx];
  }
  /* All channels in UM_MAN_CALIBRATE_REQ are calibrated */
  *calib_failed_mask = 0;
}

/**************************************************************
    Radio statistics handlers
 */

uint32 __MTLK_IFUNC
wave_radio_wss_cntr_get (wave_radio_t *radio, wave_radio_cnt_id_e id)
{
  MTLK_ASSERT(radio);
  return mtlk_wss_get_stat(radio->wss, id);
}

void __MTLK_IFUNC
wave_radio_total_traffic_delta_set (wave_radio_t *radio, uint32 total_traffic_delta)
{
  radio->total_traffic_delta = total_traffic_delta;
}

uint32 __MTLK_IFUNC
wave_radio_airtime_efficiency_get (wave_radio_t *radio)
{
  return radio->airtime_efficiency;
}

static uint32
_wave_radio_calculate_airtime_efficiency (wave_radio_t *radio, uint8 airtime)
{
  uint32 airtime_ms, efficiency = 0;
  mtlk_hw_t *hw;

  hw = radio->hw_api->hw;
  airtime_ms = MTLK_PERCENT_TO_VALUE(airtime, wave_hw_mmb_get_stats_poll_period(hw));
  if (airtime_ms) {
    efficiency = wave_hw_calculate_airtime_efficiency(
                    (uint64)radio->total_traffic_delta * MTLK_OSAL_MSEC_IN_SEC, airtime_ms);
  }

  return efficiency;
}

/* Averager for Radio Phy metrics */
static __INLINE void
__wave_radio_phy_status_avg_reset (wave_radio_phy_stat_t *radio_phy_stat)
{
  wave_phy_stat_averager_init(&radio_phy_stat->ch_util_averager);
  wave_phy_stat_averager_init(&radio_phy_stat->ch_load_averager);
  wave_phy_stat_zwdfs_ant_rssi_averager_init(&radio_phy_stat->zwdfs_ant_rssi_averager);
}

static __INLINE uint8
__wave_radio_phy_stat_get_ch_util_avg (wave_radio_phy_stat_t *radio_phy_stat)
{
  return wave_phy_stat_averager_get(&radio_phy_stat->ch_util_averager);
}

static __INLINE uint8
__wave_radio_phy_stat_get_ch_load_avg (wave_radio_phy_stat_t *radio_phy_stat)
{
  return wave_phy_stat_averager_get(&radio_phy_stat->ch_load_averager);
}

static __INLINE uint8
__wave_radio_phy_stat_get_airtime_avg (wave_radio_phy_stat_t *radio_phy_stat)
{
  return wave_phy_stat_averager_get(&radio_phy_stat->ch_util_averager) -
         wave_phy_stat_averager_get(&radio_phy_stat->ch_load_averager);
}

static __INLINE void
__wave_radio_phy_status_avg_proc (wave_radio_phy_stat_t *radio_phy_stat)
{
  wave_phy_stat_averager_proc(&radio_phy_stat->ch_util_averager, radio_phy_stat->ch_util);
  wave_phy_stat_averager_proc(&radio_phy_stat->ch_load_averager, radio_phy_stat->ch_load);
  wave_phy_stat_zwdfs_ant_rssi_averager_proc(&radio_phy_stat->zwdfs_ant_rssi_averager, radio_phy_stat->zwdfs_ant_rssi);

  ILOG3_DDDDDD("ch_util %u -> %u, ch_load %u -> %u, airtime %u, zwdfs_ant_rssi %d",
              radio_phy_stat->ch_util, __wave_radio_phy_stat_get_ch_util_avg(radio_phy_stat),
              radio_phy_stat->ch_load, __wave_radio_phy_stat_get_ch_load_avg(radio_phy_stat),
              __wave_radio_phy_stat_get_airtime_avg(radio_phy_stat),
              radio_phy_stat->zwdfs_ant_rssi);
}

static int8
__wave_radio_phy_stat_get_zwdfs_ant_rssi_avg (wave_radio_phy_stat_t *radio_phy_stat)
{
  wave_zwdfs_ant_rssi_averager_t *averager = &radio_phy_stat->zwdfs_ant_rssi_averager;
  int16 max_rssi_sum;
  int8 max_rssi_1 = MIN_RSSI, max_rssi_2 = MIN_RSSI, max_rssi_3 = MIN_RSSI;
  uint8 idx;

  for (idx = 0; idx < averager->window_size; idx++)
  {
    if (averager->rssi_values[idx] != 0)
    {
      if (averager->rssi_values[idx] > max_rssi_1) {
        max_rssi_3 = max_rssi_2;
        max_rssi_2 = max_rssi_1;
        max_rssi_1 = averager->rssi_values[idx];
      }
      else if (averager->rssi_values[idx] > max_rssi_2)
      {
        max_rssi_3 = max_rssi_2;
        max_rssi_2 = averager->rssi_values[idx];
      }
      else if (averager->rssi_values[idx] > max_rssi_3)
      {
        max_rssi_3 = averager->rssi_values[idx];
      }
    }
  }

  max_rssi_sum = max_rssi_1 + max_rssi_2 + max_rssi_3;

  return (max_rssi_sum / averager->num_max_rssi_samples);
}

/* Update Radio Phy Status by data from devicePhyRxStatus */
void __MTLK_IFUNC
wave_radio_phy_status_update (wave_radio_t *radio, wave_radio_phy_stat_t *params)
{
  wave_radio_phy_stat_t  *radio_status = &radio->phy_status;
  int       airtime;
  uint32    efficiency;


  /* Calculate air time and efficiency */
  airtime = (int)params->ch_util - (int)params->ch_load;
  if (airtime < 0) {
    ELOG_DDDD("RadioID %u: Invalid (negative) Total Airtime (%d) = Utilization (%u) - Load (%u)",
              radio->idx, airtime, params->ch_util, params->ch_load);

    airtime = 0;
  }

  efficiency = _wave_radio_calculate_airtime_efficiency(radio, airtime);

  mtlk_osal_lock_acquire(&radio->phy_status_lock);
  radio_status->noise   = params->noise;
  radio_status->ch_load = params->ch_load;
  radio_status->ch_util = params->ch_util;
  radio_status->zwdfs_ant_rssi = params->zwdfs_ant_rssi;
  radio_status->airtime = (uint8)airtime;
  radio_status->airtime_efficiency = efficiency;
  __wave_radio_phy_status_avg_proc(radio_status); /* Averager */
  mtlk_osal_lock_release(&radio->phy_status_lock);
}

void __MTLK_IFUNC
wave_radio_phy_status_get (wave_radio_t *radio, wave_radio_phy_stat_t *params)
{
  wave_radio_phy_stat_t  *radio_stat = &radio->phy_status;

  mtlk_osal_lock_acquire(&radio->phy_status_lock);
  *params = *radio_stat; /* struct copy */
  mtlk_osal_lock_release(&radio->phy_status_lock);
}

uint8 __MTLK_IFUNC
wave_radio_channel_load_get (wave_radio_t *radio)
{
  return radio->phy_status.ch_load;
}

#define MTLK_RADIO_GET_DIFF_STAT(radio,id) mtlk_wss_get_stat(radio->wss,id) - mtlk_wss_get_stat(radio->wss_snapshot,id)
#define MTLK_RADIO_GET_DIFF_STAT_64(radio,id) (uint64) (mtlk_wss_get_stat_64(radio->wss,id) - mtlk_wss_get_stat_64(radio->wss_snapshot,id))

#define MTLK_RADIO_GET_WSS_STAT(radio,id) mtlk_wss_get_stat(radio->wss,id)
#define MTLK_RADIO_GET_WSS_STAT_64(radio,id) (uint64) mtlk_wss_get_stat_64(radio->wss,id)


static void
_wave_radio_get_traffic_stats (wave_radio_t *radio, mtlk_wssa_drv_traffic_stats_t* stats)
{

  stats->UnicastPacketsSent       = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_UNICAST_PACKETS_SENT);
  stats->UnicastPacketsReceived   = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_UNICAST_PACKETS_RECEIVED);
  stats->MulticastPacketsSent     = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_MULTICAST_PACKETS_SENT);
  stats->MulticastPacketsReceived = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_MULTICAST_PACKETS_RECEIVED);
  stats->BroadcastPacketsSent     = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_BROADCAST_PACKETS_SENT);
  stats->BroadcastPacketsReceived = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_BROADCAST_PACKETS_RECEIVED);

  stats->PacketsSent              = MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PACKETS_SENT);
  stats->PacketsReceived          = MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PACKETS_RECEIVED);
  stats->BytesSent                = MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_BYTES_SENT);
  stats->BytesReceived            = MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_BYTES_RECEIVED);

  /* Radio PD statistics */
  stats->PacketPDSent             = (MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_PD_BROADCAST_PACKETS_SENT)) +
                                    (MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_PD_UNICAST_PACKETS_SENT)) +
                                    (MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_PD_MULTICAST_PACKETS_SENT));

  stats->PDBytesSent              = (MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PD_BROADCAST_BYTES_SENT_64)) +
                                    (MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PD_UNICAST_BYTES_SENT_64)) +
                                    (MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PD_MULTICAST_BYTES_SENT_64));

  stats->UnicastPDSent            = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_PD_UNICAST_PACKETS_SENT);

  stats->UnicastPDBytesSent       = MTLK_RADIO_GET_WSS_STAT_64(radio, WAVE_RADIO_CNT_PD_UNICAST_BYTES_SENT_64);

}

static void
_wave_radio_get_mgmt_stats (wave_radio_t *radio, mtlk_wssa_drv_mgmt_stats_t *stats)
{

  stats->MANFramesResQueue        = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_MAN_FRAMES_RES_QUEUE);
  stats->MANFramesSent            = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_MAN_FRAMES_SENT);
  stats->MANFramesConfirmed       = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_MAN_FRAMES_CONFIRMED);
  stats->MANFramesReceived        = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_MAN_FRAMES_RECEIVED);
  stats->MANFramesRetryDropped    = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_RX_MAN_FRAMES_RETRY_DROPPED);

  stats->ProbeRespSent            = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_TX_PROBE_RESP_SENT);
  stats->ProbeRespDropped         = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_TX_PROBE_RESP_DROPPED);
  stats->BssMgmtTxQueFull         = MTLK_RADIO_GET_WSS_STAT(radio, WAVE_RADIO_CNT_BSS_MGMT_TX_QUE_FULL);
}

static void
_wave_radio_get_tr181_error_stats (wave_radio_t *radio, mtlk_wssa_drv_tr181_error_stats_t *errors)
{
  errors->ErrorsSent             = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_ERROR_PACKETS_SENT);
  errors->ErrorsReceived         = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_ERROR_PACKETS_RECEIVED);
  errors->DiscardPacketsReceived = MTLK_RADIO_GET_DIFF_STAT(radio, WAVE_RADIO_CNT_DISCARD_PACKETS_RECEIVED);
  errors->DiscardPacketsSent     = errors->ErrorsSent;
}

void __MTLK_IFUNC
wave_radio_get_tr181_hw_stats (wave_radio_t *radio, mtlk_wssa_drv_tr181_hw_stats_t *stats)
{
  _wave_radio_get_traffic_stats(radio, &stats->traffic_stats);
  _wave_radio_get_tr181_error_stats(radio, &stats->error_stats);

  stats->Noise = radio->phy_status.noise;
  stats->FCSErrorCount = 0; /* FIXME: TBD */
}

void __MTLK_IFUNC
wave_radio_get_hw_stats (wave_radio_t *radio, mtlk_wssa_drv_hw_stats_t *stats)
{
  _wave_radio_get_traffic_stats(radio, &stats->traffic_stats);
  _wave_radio_get_mgmt_stats(radio, &stats->mgmt_stats);

  mtlk_osal_lock_acquire(&radio->phy_status_lock);
  stats->ChannelLoad        = radio->phy_status.ch_load;
  stats->ChannelUtil        = radio->phy_status.ch_util;
  stats->Airtime            = radio->phy_status.airtime;
  stats->AirtimeEfficiency  = radio->phy_status.airtime_efficiency;
  stats->zwdfsAntRssiAvg    = __wave_radio_phy_stat_get_zwdfs_ant_rssi_avg(&radio->phy_status);
  mtlk_osal_lock_release(&radio->phy_status_lock);

  /* HW statisticss */
  wave_hw_get_hw_stats(radio->hw_api->hw, stats);
}

void __MTLK_IFUNC wave_radio_sync_hostapd_done_set (wave_radio_t *radio, BOOL value)
{
  MTLK_ASSERT(NULL != radio);
  radio->is_sync_hostapd_done = value;
  ILOG2_DD("RadioID %u: Set HOSTAPD sync to %u", radio->idx, (unsigned)value);
}

BOOL __MTLK_IFUNC wave_radio_is_sync_hostapd_done (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  return radio->is_sync_hostapd_done;
}

int wave_radio_get_associated_dev_stats (struct wiphy *wiphy,
                                         struct net_device *ndev,
                                         const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerFlowStats *peer_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);
  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(peer_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_stats, sizeof(*peer_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_associated_dev_tid_stats (struct wiphy *wiphy,
                                             struct net_device *ndev,
                                             const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerTidStats *tid_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_TID_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_TID_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  tid_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(tid_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, tid_stats, sizeof(*tid_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_twt_params (struct wiphy *wiphy,
                               struct net_device *ndev,
                               const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  twt_params_int_t *twt_params = NULL;
  uint32 twtData_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
      WAVE_CORE_REQ_GET_TWT_PARAMETERS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
      WAVE_CORE_REQ_GET_TWT_PARAMETERS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  twt_params = mtlk_clpb_enum_get_next(clpb, &twtData_size);
  MTLK_CLPB_TRY(twt_params, twtData_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, twt_params, sizeof(*twt_params));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}


int __MTLK_IFUNC
wave_radio_get_channel_stats (struct wiphy *wiphy,
                              struct net_device *ndev,
                              const void *data,
                              size_t data_len)
{
  uint32 *channels_list = (uint32*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  wifi_channelStats_t *chan_stats = NULL;
  uint32 stats_size;
  size_t num_of_channels = data_len/sizeof(*channels_list);

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_WIFI_CHANNEL_STATS, &clpb, channels_list, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_WIFI_CHANNEL_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;
  
  chan_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY_EX(chan_stats, stats_size, sizeof(wifi_channelStats_t) * num_of_channels)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, chan_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_phy_channel_status (struct wiphy *wiphy,
                                       struct net_device *ndev)
{
  wave_radio_phy_stat_t *radio_phy_stat;
  mtlk_phy_channel_status phy_chan_status;
  mtlk_df_user_t *df_user;
  int res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;
  uint32 status_size;

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_RADIO_REQ_GET_PHY_CHAN_STATUS,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_GET_PHY_CHAN_STATUS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  radio_phy_stat = mtlk_clpb_enum_get_next(clpb, &status_size);
  MTLK_CLPB_TRY(radio_phy_stat, status_size)
    phy_chan_status.airtime_efficiency = radio_phy_stat->airtime_efficiency;
    phy_chan_status.noise = radio_phy_stat->noise;
    /* Averaged values */
    phy_chan_status.ch_util = __wave_radio_phy_stat_get_ch_util_avg(radio_phy_stat);
    phy_chan_status.ch_load = __wave_radio_phy_stat_get_ch_load_avg(radio_phy_stat);
    phy_chan_status.airtime = __wave_radio_phy_stat_get_airtime_avg(radio_phy_stat);

    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &phy_chan_status, sizeof(phy_chan_status));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_associated_dev_rate_info_rx_stats (struct wiphy *wiphy,
                                                      struct net_device *ndev,
                                                      const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerRateInfoRxStats *peer_rx_counters;
  uint32 stat_size;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_RATE_INFO_RX_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_RATE_INFO_RX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_rx_counters = mtlk_clpb_enum_get_next(clpb, &stat_size);

  MTLK_CLPB_TRY(peer_rx_counters, stat_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_rx_counters, sizeof(*peer_rx_counters));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_associated_dev_rate_info_tx_stats (struct wiphy *wiphy,
                                                      struct net_device *ndev,
                                                      const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerRateInfoTxStats *peer_tx_counters;
  uint32 stat_size;

  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_RATE_INFO_TX_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_ASSOCIATED_DEV_RATE_INFO_TX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_tx_counters = mtlk_clpb_enum_get_next(clpb, &stat_size);
  MTLK_CLPB_TRY(peer_tx_counters, stat_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_tx_counters, sizeof(*peer_tx_counters));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_list (struct wiphy *wiphy,
                              struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_peer_list_t *peer_list = NULL, *tmp_peer_list = NULL, *tmp_clpb = NULL;
  uint32 size, *no_of_stas, sta_cnt;

  df_user = mtlk_df_user_from_ndev(ndev);
  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

#ifdef ONEWIFI_IWLWAV_DEV
  /* Under OneWifi need to reject request to non-active interfaces */
  if (!df_user) {
    ILOG2_S("%s: interface is not active!", ndev->name);
    return -EFAULT;
  }
#else
  MTLK_CHECK_DF_USER(df_user);
#endif

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_LIST, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval_void(res, clpb,
    WAVE_CORE_REQ_GET_PEER_LIST, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  no_of_stas = mtlk_clpb_enum_get_next(clpb, &size);
  if ((sizeof(uint32) != size) || (no_of_stas == NULL)) {
    goto end;
  }

  if (0 != *no_of_stas) {
    if (NULL == (peer_list = mtlk_osal_mem_alloc(sizeof(mtlk_wssa_peer_list_t) * (*no_of_stas),
                                                MTLK_MEM_TAG_EXTENSION))) {
      ELOG_V("Can't allocate memory");
      res = MTLK_ERR_NO_MEM;
      goto end;
    }

    sta_cnt = *no_of_stas;
    tmp_peer_list = peer_list;
    /* enumerate sta entries */
    while ((NULL != (tmp_clpb = mtlk_clpb_enum_get_next(clpb, &size))) && ((*no_of_stas)--)) {
      if (sizeof(*tmp_clpb) != size) {
        res = MTLK_ERR_UNKNOWN;
        goto delete_clpb;
      }
      wave_memcpy(tmp_peer_list++, size, tmp_clpb, size);
    }

    MTLK_ASSERT(NULL != peer_list);

    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_list, sizeof(*peer_list) * sta_cnt);
    mtlk_clpb_delete(clpb);
    mtlk_osal_mem_free(peer_list);
    return res;
  }
  else
  {
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, NULL, 0);
    mtlk_clpb_delete(clpb);
    return res;
  }

delete_clpb:
  mtlk_osal_mem_free(peer_list);

end:
  if(clpb)
    mtlk_clpb_delete(clpb);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int __MTLK_IFUNC
wave_radio_copy_probe_req_list(wave_radio_t *radio, probe_req_info *info)
{
  ieee_addr_sized_list_t *list = &radio->probe_resp_list;
  mtlk_hash_enum_t              e;
  ieee_addr_entry_time_t *ieee_addr_entry = NULL;
  mtlk_osal_ms_diff_t timediff;
  IEEE_ADDR *addr = NULL;
  struct intel_vendor_probe_req_info *probe_list;
  MTLK_HASH_ENTRY_T(ieee_addr) *h;
  int counter = 0;
  mtlk_osal_msec_t current_time;

  info->data = NULL;

  mtlk_osal_lock_acquire(&list->ieee_addr_lock);
  ILOG2_D("size of probe req list is %d", list->size);
  if (list->size) {
    info->data = mtlk_osal_mem_alloc(list->size*sizeof(struct intel_vendor_probe_req_info),MTLK_MEM_TAG_EXTENSION);
    if (info->data == NULL) {
      mtlk_osal_lock_release(&list->ieee_addr_lock);
      return MTLK_ERR_NO_MEM;
    }
    probe_list = info->data;

    current_time = mtlk_osal_timestamp_to_ms(mtlk_osal_timestamp());
    h = mtlk_hash_enum_first_ieee_addr(&list->hash, &e);
    while (h) {
      ieee_addr_entry = MTLK_CONTAINER_OF(h, ieee_addr_entry_time_t, hentry);
      timediff = mtlk_osal_ms_time_diff(current_time, ieee_addr_entry->timestamp);
      if (timediff > WAVE_PROBE_REQ_LIST_MAX_AGE_MS) {
        h = mtlk_hash_enum_next_ieee_addr(&list->hash, &e);
        continue;
      }
      addr = MTLK_HASH_VALUE_GET_KEY(ieee_addr_entry, hentry);
      if (counter == list->size) {
        ELOG_D("RadioID %u: size of probe req list is bigger than saved size", radio->idx);
        break;
      }
      probe_list[counter].addr = *addr;
      probe_list[counter].age = timediff / MTLK_OSAL_MSEC_IN_SEC; /* in sec */
      probe_list[counter].rssi = ieee_addr_entry->probe_rssi;
      counter++;
      h = mtlk_hash_enum_next_ieee_addr(&list->hash, &e);
    }
  }
  mtlk_osal_lock_release(&list->ieee_addr_lock);
  info->size = counter*sizeof(struct intel_vendor_probe_req_info);

  return MTLK_ERR_OK;
}

int __MTLK_IFUNC
wave_radio_get_probe_req_list (struct wiphy *wiphy,
                               struct net_device *ndev,
                               uint8 isflush)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  uint32 info_size;
  probe_req_info *info;
  BOOL flush_probe_list = !!isflush;

  df_user = mtlk_df_user_from_ndev(ndev);
  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_RADIO_REQ_GET_PROBE_REQ_LIST, &clpb, &flush_probe_list, sizeof(flush_probe_list));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_RADIO_REQ_GET_PROBE_REQ_LIST, FALSE);

  if (res != MTLK_ERR_OK) {
    ELOG_D("invoke core failed with res=%d", res);
    goto end;
  }

  info = mtlk_clpb_enum_get_next(clpb, &info_size);
  MTLK_CLPB_TRY(info, info_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, info->data, info->size);
    if (info->data)
      mtlk_osal_mem_free(info->data);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    if (info->data)
      mtlk_osal_mem_free(info->data);
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_flow_status (struct wiphy *wiphy,
                                     struct net_device *ndev,
                                     const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_peer_stats_t *peer_flow_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_FLOW_STATUS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_FLOW_STATUS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_flow_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(peer_flow_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_flow_stats, sizeof(*peer_flow_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_capabilities (struct wiphy *wiphy,
                                      struct net_device *ndev,
                                      const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_peer_capabilities_t *peer_capabilities;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_CAPABILITIES, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_CAPABILITIES, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_capabilities = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(peer_capabilities, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_capabilities, sizeof(*peer_capabilities));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_rate_info (struct wiphy *wiphy,
                                   struct net_device *ndev,
                                   const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_peer_rates_info_t *peer_rate_info;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_RATE_INFO, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_RATE_INFO, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_rate_info = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(peer_rate_info, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_rate_info, sizeof(*peer_rate_info));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tr181_peer_statistics (struct wiphy *wiphy,
                                          struct net_device *ndev,
                                          const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  st_info_data_t info_data;
  mtlk_wssa_drv_tr181_peer_stats_t *tr181_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  info_data.sta = NULL;
  info_data.mac = addr;
  info_data.stinfo = NULL;
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TR181_PEER_STATS, &clpb, &info_data, sizeof(st_info_data_t));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TR181_PEER_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  tr181_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(tr181_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, tr181_stats, sizeof(*tr181_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_dev_diag_result2 (struct wiphy *wiphy,
                                     struct net_device *ndev,
                                     const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wifiAssociatedDevDiagnostic2_t *dev_diag_res2_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_DEV_DIAG_RESULT2, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_DEV_DIAG_RESULT2, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  dev_diag_res2_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(dev_diag_res2_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, dev_diag_res2_stats, sizeof(*dev_diag_res2_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_dev_diag_result3 (struct wiphy *wiphy,
                                     struct net_device *ndev,
                                     const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wifiAssociatedDevDiagnostic3_t *dev_diag_res3_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_DEV_DIAG_RESULT3, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_DEV_DIAG_RESULT3, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  dev_diag_res3_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(dev_diag_res3_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, dev_diag_res3_stats, sizeof(*dev_diag_res3_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_associated_stations_stats (struct wiphy *wiphy,
                                              struct net_device *ndev,
                                              const uint8 *addr_list,
                                              size_t data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_PARAMS;
  wifiAssociatedStationStats_t *assoc_stations_stats;
  uint32 num_addr = 0;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (data_len <= sizeof(IEEE_ADDR))
    num_addr = 1;
  else
    num_addr = data_len / sizeof(IEEE_ADDR);

  if (num_addr * sizeof(IEEE_ADDR) != data_len) {
    ELOG_D("bad data_len=%u", data_len);
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_ASSOC_STATIONS_STATS, &clpb, addr_list, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_ASSOC_STATIONS_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  assoc_stations_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY_EX(assoc_stations_stats, stats_size, (num_addr * sizeof(wifiAssociatedStationStats_t)))
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, assoc_stations_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tr181_wlan_statistics (struct wiphy *wiphy,
                                          struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tr181_wlan_stats_t *tr181_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TR181_WLAN_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TR181_WLAN_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  tr181_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(tr181_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, tr181_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tr181_hw_statistics (struct wiphy *wiphy,
                                        struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tr181_hw_stats_t *tr181_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TR181_HW_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TR181_HW_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  tr181_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(tr181_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, tr181_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_recovery_statistics (struct wiphy *wiphy,
                                        struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_recovery_stats_t *recovery_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_RECOVERY_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_RECOVERY_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  recovery_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(recovery_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, recovery_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_hw_flow_status (struct wiphy *wiphy,
                                   struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_hw_stats_t *hw_flow_status;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_HW_FLOW_STATUS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_HW_FLOW_STATUS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  hw_flow_status = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(hw_flow_status, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, hw_flow_status, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int __MTLK_IFUNC
wave_radio_cdb_antenna_cfg_update (wave_radio_t *radio, uint8 ant_mask)
{
  mtlk_coc_t *coc_mgmt;
  int res = MTLK_ERR_OK;

  MTLK_ASSERT(radio != NULL);

  coc_mgmt = wave_radio_coc_mgmt_get(radio);
  res = mtlk_core_cfg_update_antenna_mask(wave_radio_master_core_get(radio), ant_mask, TRUE);
  if (MTLK_ERR_OK != res)
    return res;
  mtlk_coc_dump_antenna_params(coc_mgmt);

  return res;
}

int __MTLK_IFUNC
wave_radio_country_code_set(wave_radio_t *radio, const mtlk_country_code_t *country_code)
{
  int res;

  res = WAVE_RADIO_PDB_SET_BINARY(radio, PARAM_DB_RADIO_COUNTRY_CODE_ALPHA, country_code, sizeof(*country_code));

  return res;
}

int __MTLK_IFUNC
wave_radio_country_code_set_by_str(wave_radio_t *radio, char *str, size_t len)
{
  mtlk_country_code_t   country_code;

  MTLK_ASSERT(NULL != str);

  /* Input string may be NOT NUL-terminated, e.g. alpha[2] */
  /* Prepare a NUL-terminated string */
  memset(&country_code, 0, sizeof(country_code));
  wave_memcpy(country_code.country, sizeof(country_code.country), str, MIN(len, (sizeof(country_code.country) - 1)));

  return wave_radio_country_code_set(radio, &country_code);
}

BOOL __MTLK_IFUNC
wave_radio_is_init_done(wave_radio_t *radio) {
  MTLK_ASSERT(NULL != radio);

  return radio->init_done;
}

BOOL __MTLK_IFUNC
wave_radio_get_is_zwdfs_radio (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return (ZWDFS_CDB_CFG(radio) && !wave_radio_is_first(radio));
}

BOOL __MTLK_IFUNC
wave_radio_get_is_zwdfs_operational_radio (wave_radio_t *radio)
{
  mtlk_hw_t *hw;
  mtlk_hw_band_e band;

  MTLK_ASSERT(radio != NULL);
  hw = radio->hw_api->hw;
  band = WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_FREQ_BAND_CUR);

  if ((wave_hw_radio_band_cfg_get(hw) == WAVE_HW_RADIO_BAND_CFG_SCAN) &&
      ((band == MTLK_HW_BAND_5_2_GHZ) || (band == MTLK_HW_BAND_2_4_GHZ)) &&
      wave_radio_is_first(radio)) {
    return TRUE;
  }
  return FALSE;
}

wave_radio_t * __MTLK_IFUNC
wave_radio_descr_get_zwdfs_radio (wave_radio_descr_t *radio_descr)
{
  wave_radio_t *radio;
  unsigned radio_idx;

  MTLK_ASSERT(radio_descr != NULL);

  for (radio_idx = 0; radio_idx < radio_descr->num_radios; radio_idx++) {
    radio = wave_radio_descr_wave_radio_get(radio_descr, radio_idx);
    if (wave_radio_get_is_zwdfs_radio(radio))
      return radio;
  }

  return NULL;
}

int __MTLK_IFUNC
wave_radio_get_zwdfs_ant_enabled (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_ZWDFS_ANT_ACTIVE);
}

void __MTLK_IFUNC
wave_radio_set_zwdfs_ant_enabled (wave_radio_t *radio, const int enable)
{
  MTLK_ASSERT(radio != NULL);
  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_ZWDFS_ANT_ACTIVE, enable);
}

int __MTLK_IFUNC
wave_radio_get_cts_to_self_to (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_CTS_TO_SELF_TO);
}

#define MTLK_CTS_TO_SELF_MIN 1
#define MTLK_CTS_TO_SELF_MAX 32

int __MTLK_IFUNC
wave_radio_set_cts_to_self_to (wave_radio_t *radio, const int cts_to_self_to)
{
  MTLK_ASSERT(radio != NULL);

  if((cts_to_self_to < MTLK_CTS_TO_SELF_MIN) || (cts_to_self_to > MTLK_CTS_TO_SELF_MAX)) {
    ELOG_DDD("Icorrect CTS to Self value %d, expected %d..%d",
             cts_to_self_to, MTLK_CTS_TO_SELF_MIN, MTLK_CTS_TO_SELF_MAX);
    return MTLK_ERR_VALUE;
  }

  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_CTS_TO_SELF_TO, cts_to_self_to);
  return MTLK_ERR_OK;
}

/* Read fils beaocon flag from PDB */
int __MTLK_IFUNC
wave_radio_get_fils_beacon_flag (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_FILS_BEACON_FLAG);
}

/* Store fils beacon flag from PDB */
void __MTLK_IFUNC
wave_radio_set_fils_beacon_flag (wave_radio_t *radio, const int fils_beacon_flag)
{
  MTLK_ASSERT(radio != NULL);
  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_FILS_BEACON_FLAG, fils_beacon_flag);
}

uint32 __MTLK_IFUNC
wave_radio_get_tx_ampdu_density (wave_radio_t *radio)
{
  MTLK_ASSERT(radio != NULL);
  return WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_TX_AMPDU_DENSITY);
}

int __MTLK_IFUNC
wave_radio_set_tx_ampdu_density (wave_radio_t *radio, const int ampdu_density)
{
  MTLK_ASSERT(radio != NULL);

  /* tx ampdu density value range is 0 to 7 */
  if((ampdu_density < MTLK_MIN_TX_AMPDU_DENSITY) || (ampdu_density > MTLK_MAX_TX_AMPDU_DENSITY)) {
    ELOG_DDD("Icorrect AMPDU density value %d, expected %d..%d",
             ampdu_density, MTLK_MIN_TX_AMPDU_DENSITY, MTLK_MAX_TX_AMPDU_DENSITY);
    return MTLK_ERR_VALUE;
  }

  WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_TX_AMPDU_DENSITY, ampdu_density);
  return MTLK_ERR_OK;
}

uint32 __MTLK_IFUNC
wave_radio_max_tx_ampdu_density_get(wave_radio_t *radio, uint32 sta_ampdu_density)
{
  uint32 usr_ampdu_density;

  MTLK_ASSERT(radio != NULL);
  usr_ampdu_density = wave_radio_get_tx_ampdu_density (radio);

  return  (MAX(usr_ampdu_density, sta_ampdu_density));
}

/*
 * BSS TX monitor
 */
#if WAVE_USE_BSS_TX_MONITOR

#define BSS_TX_POLL_PERIOD 1000u

static void _wave_radio_bss_tx_poll_cb (mtlk_handle_t user_context,
                                        int           processing_result,
                                        mtlk_clpb_t  *pclpb)
{
  int res;
  wave_radio_t *radio = HANDLE_T_PTR(wave_radio_t, user_context);

  res = _mtlk_df_user_process_core_retval_void(processing_result, pclpb,
                                               WAVE_RADIO_REQ_GET_BSS_TX_STATUS, FALSE);

  if (MTLK_ERR_OK != res) {
    WLOG_D("res %d", res);
  }

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  /* always executed in the main radio context, just double check for safety */
  if (radio->idx != 0) {
    ELOG_D("radio_idx %d", radio->idx);
    return;
  }

  if (!mtlk_osal_timer_is_cancelled(&radio->bss_tx_timer)) {
    mtlk_osal_timer_set(&radio->bss_tx_timer, BSS_TX_POLL_PERIOD);
  }
}

static uint32 _wave_radio_bss_tx_poll (mtlk_osal_timer_t *timer, mtlk_handle_t data)
{
  wave_radio_t *radio = HANDLE_T_PTR(wave_radio_t, data);
  mtlk_hw_t *hw;
  mtlk_df_t *df;

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return 0;
  }

  /* always executed in the main radio context, just double check for safety */
  if (radio->idx != 0) {
    ELOG_D("radio_idx %d", radio->idx);
    return 0;
  }

  hw = radio->hw_api->hw;

  /* if error is occurred in _pci_probe then serializer is absent */
  if (!wave_rcvry_pci_probe_error_get(wave_hw_mmb_get_mmb_base(hw))) {
    df = mtlk_vap_manager_get_master_df(radio->vap_manager);
    if (NULL == df) {
      ELOG_V("master df is NULL");
      return 0;
    }

    _mtlk_df_user_invoke_core_async(df, WAVE_RADIO_REQ_GET_BSS_TX_STATUS,
                                    NULL, 0, _wave_radio_bss_tx_poll_cb,
                                    HANDLE_T(radio));

  }

  return 0;
}

static int _wave_radio_bss_tx_timer_init (wave_radio_t* radio)
{
  int res;

  MTLK_ASSERT(radio != NULL);
  ILOG0_D("radio %d", radio->idx);

  res = wave_hw_bss_tx_map_init(radio->hw_api->hw);
  if (MTLK_ERR_OK != res)
    return res;

  mtlk_osal_timer_set(&radio->bss_tx_timer, BSS_TX_POLL_PERIOD);

  return MTLK_ERR_OK;
}

static void _wave_radio_bss_tx_timer_deinit (wave_radio_t* radio)
{
  MTLK_ASSERT(radio != NULL);
  ILOG0_D("radio %d", radio->idx);

  if (!mtlk_osal_timer_is_cancelled(&radio->bss_tx_timer)) {
    mtlk_osal_timer_cancel_sync(&radio->bss_tx_timer);
  }
  wave_hw_bss_tx_map_deinit(radio->hw_api->hw);
}

void __MTLK_IFUNC wave_radio_bss_tx_timer_deinit (wave_radio_descr_t *radio_descr)
{
  /* do deinit for the 1st radio only as BSS TX HD map table is per card */
  wave_radio_t* radio = &radio_descr->radio[0];

  _wave_radio_bss_tx_timer_deinit(radio);
  mtlk_osal_timer_cleanup(&radio->bss_tx_timer);
}

int __MTLK_IFUNC wave_radio_bss_tx_timer_init (wave_radio_descr_t *radio_descr)
{
  /* do init for the 1st radio only as BSS TX HD map table is per card */
  wave_radio_t* radio = &radio_descr->radio[0];
  int res;

  res = mtlk_osal_timer_init(&radio->bss_tx_timer, _wave_radio_bss_tx_poll, HANDLE_T(radio));
  if (MTLK_ERR_OK != res)
    return res;

  return _wave_radio_bss_tx_timer_init(radio);
}

/* recovery API executed only for radio 0 */
void __MTLK_IFUNC wave_radio_on_rcvry_isol (wave_radio_t *radio)
{
  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  if (radio->idx != 0) {
    return;
  }

  _wave_radio_bss_tx_timer_deinit(radio);
}

void __MTLK_IFUNC wave_radio_on_rcvry_restore (wave_radio_t *radio)
{
  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  if (radio->idx != 0) {
    return;
  }

  _wave_radio_bss_tx_timer_init(radio);
}
#endif /* WAVE_USE_BSS_TX_MONITOR */

void __MTLK_IFUNC wave_radio_set_cac_pending (wave_radio_t *radio, BOOL state)
{
  MTLK_ASSERT(radio);

  radio->cac_pending = state;
}

BOOL __MTLK_IFUNC wave_radio_get_cac_pending (wave_radio_t *radio)
{
  MTLK_ASSERT(radio);

  return radio->cac_pending;
}

#ifdef MTLK_WAVE_700
uint16 __MTLK_IFUNC wave_radio_get_high_speed_counter (wave_radio_t *radio)
{
    MTLK_ASSERT(radio);
    return radio->high_speed_stas_cntr;
}

uint16 __MTLK_IFUNC wave_radio_inc_high_speed_counter (wave_radio_t *radio)
{
    MTLK_ASSERT(radio);
    radio->high_speed_stas_cntr++;
    return radio->high_speed_stas_cntr;
}

uint16 __MTLK_IFUNC wave_radio_dec_high_speed_counter (wave_radio_t *radio)
{
    MTLK_ASSERT(radio);
    MTLK_ASSERT(radio->high_speed_stas_cntr > 0);
    radio->high_speed_stas_cntr--;
    return radio->high_speed_stas_cntr;
}

#endif

int wave_radio_get_priority_gpio_stats (struct wiphy *wiphy,
                                        struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_priority_gpio_stats_t *pta_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PRIORITY_GPIO_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PRIORITY_GPIO_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;
  pta_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(pta_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, pta_stats, stats_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static __INLINE int
mtlk_osal_valid_input_params(struct net_device *ndev,
                             wave_radio_t *radio,
                             void *wiphy,
                             mtlk_df_user_t *df_user,
                             u8 *addr,
                             int data_len)
{
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    return MTLK_ERR_PARAMS;
  }

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (data_len != sizeof(IEEE_ADDR)) {
    ELOG_D("Wrong peer host if qos data length: %d", data_len);
    return MTLK_ERR_PARAMS;;
  }

  if (mtlk_osal_eth_is_broadcast(addr)) {
    ELOG_V("Station address can't be broadcast address");
    return MTLK_ERR_PARAMS;
  }
  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    return MTLK_ERR_PARAMS;
  }
  return MTLK_ERR_OK;
}

void __MTLK_IFUNC wave_radio_roc_in_progress_set (wave_radio_t *radio, unsigned val)
{
  MTLK_ASSERT(NULL != radio);

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  mtlk_osal_atomic_set(&radio->roc_in_progress, val);
}

unsigned __MTLK_IFUNC wave_radio_roc_in_progress_get (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return 0;
  }

  return mtlk_osal_atomic_get(&radio->roc_in_progress);
}

void __MTLK_IFUNC wave_radio_cca_stat_reset (wave_radio_t *radio)
{
  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }
  /* Reset channel number to overwrite CCA stats on next wave_radio_cca_stat_update() */
  radio->cca_stats_chan = 0;
}

void __MTLK_IFUNC wave_radio_cca_stat_update (wave_radio_t *radio, wave_cca_stats_t *new_cca_stats, int channel)
{
  wave_cca_stats_t *radio_cca_stats = NULL;

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  radio_cca_stats = &radio->cca_stats;

  if (channel != radio->cca_stats_chan) { /* overwrite */
    /* structures assignment */
    *radio_cca_stats = *new_cca_stats;
    radio->cca_stats_chan = channel;
  } else { /* update */
    wave_cca_stats_t tmp_cca_stats;
    /* structures assignment */
    tmp_cca_stats = *new_cca_stats;
    /* calc delta */
    new_cca_stats->ccaIdle   -= radio_cca_stats->ccaIdle;
    new_cca_stats->ccaIntf   -= radio_cca_stats->ccaIntf;
    new_cca_stats->ccaTx     -= radio_cca_stats->ccaTx;
    new_cca_stats->ccaRx     -= radio_cca_stats->ccaRx;
    new_cca_stats->ccaActive -= radio_cca_stats->ccaActive;
    /* structures assignment */
    *radio_cca_stats = tmp_cca_stats;
  }
}

int __MTLK_IFUNC wave_radio_cca_stats_get (struct wiphy *wiphy, struct net_device *ndev)
{
  wave_radio_t *radio = NULL;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wave_cca_stats_user_t *cca_stats_user;
  uint32 stats_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  radio = wv_ieee80211_hw_radio_get(wiphy_to_ieee80211_hw(wiphy));
  if (NULL == radio) {
    res =  MTLK_ERR_PARAMS;
    goto end;
  }

  if (mtlk_osal_atomic_get(&radio->cca_msr_started)) {
    ELOG_D("CCA MSR in progress at freq %u", mtlk_osal_atomic_get(&radio->cca_msr_chan));
    res = MTLK_ERR_PENDING;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user),
    WAVE_CORE_REQ_CCA_STATS_GET, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_CCA_STATS_GET, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  cca_stats_user = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(cca_stats_user, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, cca_stats_user, sizeof(*cca_stats_user));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

struct ieee80211_channel * __MTLK_IFUNC wave_radio_channel_get(struct wiphy *wiphy, mtlk_hw_band_e band, uint32 chan)
{
  int freq;
  struct ieee80211_channel *channel;

  /* chan may be channel number or frequency*/
  freq = chan > 2000 ? (int)chan : ieee80211_channel_to_frequency(chan, mtlkband2nlband(band));
  if (!freq)
    return NULL;

  channel = ieee80211_get_channel(wiphy, freq);

  if (!channel || (channel->flags & IEEE80211_CHAN_DISABLED))
    return NULL;

  return channel;
}

int __MTLK_IFUNC wave_radio_cca_msr_start(struct wiphy *wiphy, struct net_device *ndev, struct intel_vendor_cca_msr_cfg *cca_msr_cfg)
{
  mtlk_error_t res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  wave_radio_t *radio = NULL;
  mtlk_hw_band_e band;
  struct ieee80211_channel *channel;
  mtlk_df_t *master_df, *df;
  struct mtlk_scan_request *scan_req = NULL;
  struct mtlk_channel *scan_chan;
  uint32 prev_chan;

  MTLK_ASSERT(NULL != wiphy);
  MTLK_ASSERT(NULL != ndev);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  master_df = mtlk_df_user_get_master_df(df_user);
  df = mtlk_df_user_get_df(df_user);
  MTLK_CHECK_DF(master_df);
  MTLK_CHECK_DF(df);

  radio = wv_ieee80211_hw_radio_get(wiphy_to_ieee80211_hw(wiphy));
  if (NULL == radio) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  band = wave_radio_band_get(radio);
  channel = wave_radio_channel_get(wiphy, band, cca_msr_cfg->channel);
  if (NULL == channel) {
    ELOG_DD("Freq %u not supported for band %d", cca_msr_cfg->channel, band);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  if (mtlk_osal_atomic_get(&radio->cca_msr_started)) {
    ELOG_D("CCA MSR in progress at freq %u", mtlk_osal_atomic_get(&radio->cca_msr_chan));
    res = MTLK_ERR_PENDING;
    goto end;
  }

  scan_req = mtlk_osal_mem_alloc(sizeof(*scan_req), MTLK_MEM_TAG_USER_RADIO);
  if (!scan_req) {
    ELOG_V("mem alloc failed");
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  mtlk_osal_atomic_set(&radio->cca_msr_started, 1);
  prev_chan = mtlk_osal_atomic_xchg(&radio->cca_msr_chan, channel->center_freq);

  memset(scan_req, 0, sizeof(*scan_req));
  scan_req->type = MTLK_SCAN_CCA;
  scan_req->requester_vap_index = mtlk_vap_get_id(mtlk_df_get_vap_handle(df));
  scan_req->wiphy = wiphy;
  scan_req->n_channels = 1;
  scan_req->duration = cca_msr_cfg->duration;
  scan_chan = &scan_req->channels[0];
  scan_chan->dfs_state_entered = channel->dfs_state_entered;
  scan_chan->dfs_state = channel->dfs_state;
  scan_chan->dfs_cac_ms = channel->dfs_cac_ms;
  scan_chan->band = nlband2mtlkband(channel->band);
  scan_chan->center_freq = channel->center_freq;
  scan_chan->flags = channel->flags;
  scan_chan->orig_flags = channel->orig_flags;
  scan_chan->max_antenna_gain = channel->max_antenna_gain;
  scan_chan->max_power = channel->max_power;
  scan_chan->max_reg_power = channel->max_reg_power;

  ILOG1_D("Starting CCA measurements at freq %u", scan_chan->center_freq);

  res = _mtlk_df_user_invoke_core(master_df, WAVE_RADIO_REQ_DO_SCAN, &clpb, scan_req, sizeof(*scan_req));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_DO_SCAN, TRUE);
  if (MTLK_ERR_OK != res) { /* restore parameters on scan failure */
    mtlk_osal_atomic_set(&radio->cca_msr_chan, prev_chan);
    mtlk_osal_atomic_set(&radio->cca_msr_started, 0);
  }

end:
  if (scan_req)
    mtlk_osal_mem_free(scan_req);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

void __MTLK_IFUNC wave_radio_cca_msr_done (wave_radio_t *radio, mtlk_core_t *core)
{
  int ch_num;
  unsigned chan_freq;
  wave_cca_stats_t cca_stats = { 0 };

  MTLK_ASSERT(NULL != core);
  MTLK_ASSERT(NULL != radio);

  if (!mtlk_osal_atomic_get(&radio->cca_msr_started)) {
    WLOG_V("ignore");
    return;
  }

  chan_freq = mtlk_osal_atomic_get(&radio->cca_msr_chan);
  ch_num = ieee80211_frequency_to_channel(chan_freq);
  wave_core_cca_stats_save(core, ch_num, &cca_stats, &radio->cca_stats_user, FALSE);

  ILOG1_V("CCA measurements has been completed");
  mtlk_osal_atomic_set(&radio->cca_msr_started, 0);
}

int __MTLK_IFUNC wave_radio_cca_msr_get(struct wiphy *wiphy, struct net_device *ndev, uint32 *user_chan)
{
  mtlk_error_t res = MTLK_ERR_OK;
  wave_radio_t *radio = NULL;
  mtlk_hw_band_e band;
  struct ieee80211_channel *channel;
  wave_cca_stats_user_t cca_stats_user;


  MTLK_ASSERT(NULL != wiphy);
  MTLK_ASSERT(NULL != ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  radio = wv_ieee80211_hw_radio_get(wiphy_to_ieee80211_hw(wiphy));
  if (NULL == radio) {
    res =  MTLK_ERR_PARAMS;
    goto end;
  }

  band = wave_radio_band_get(radio);
  channel = wave_radio_channel_get(wiphy, band, *user_chan);
  if (NULL == channel) {
    ELOG_DD("Freq %u not supported for band %d", *user_chan, band);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  if (mtlk_osal_atomic_get(&radio->cca_msr_started)) {
    ELOG_D("CCA MSR in progress at freq %u", mtlk_osal_atomic_get(&radio->cca_msr_chan));
    res = MTLK_ERR_PENDING;
    goto end;
  }

  if (channel->center_freq != mtlk_osal_atomic_get(&radio->cca_msr_chan)) {
    ELOG_D("No CCA MSR data at freq %u", channel->center_freq);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  /* no serializer required, just copy user stats from radio context */
  cca_stats_user = radio->cca_stats_user; /* struct assignment */
  return wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &cca_stats_user, sizeof(cca_stats_user));

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}


int __MTLK_IFUNC wave_radio_usage_stats_get (struct wiphy *wiphy, struct net_device *ndev)
{
  wave_radio_t *radio = NULL;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wave_radio_usage_stats_t *radio_usage_stats;
  uint32 stats_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  radio = wv_ieee80211_hw_radio_get(wiphy_to_ieee80211_hw(wiphy));
  if (NULL == radio) {
    res =  MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_master_df(df_user),
    WAVE_RADIO_REQ_GET_USAGE_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_RADIO_REQ_GET_USAGE_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  radio_usage_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(radio_usage_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, radio_usage_stats, sizeof(*radio_usage_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

BOOL __MTLK_IFUNC wave_radio_was_scan_done_since_prev_sta_poll (wave_radio_t *radio)
{
  MTLK_ASSERT(radio);
  return radio->scan_was_done_since_prev_poll;
}

void __MTLK_IFUNC wave_radio_set_scan_done_since_prev_sta_poll (wave_radio_t *radio, BOOL value)
{
  MTLK_ASSERT(radio);
  radio->scan_was_done_since_prev_poll = value;
}

int wave_reset_statistics (
  struct wiphy *wiphy,
  struct net_device *ndev,
  struct intel_vendor_reset_statistics *data)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_RESET_STATISTICS, &clpb, data, sizeof(struct intel_vendor_reset_statistics));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_RESET_STATISTICS, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int __MTLK_IFUNC wave_radio_get_peer_mixed_stats (struct wiphy *wiphy,
                                                  struct net_device *ndev,
                                                  const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  wave_peer_mixed_stats_t *peer_mixed_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_MIXED_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_MIXED_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_mixed_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(peer_mixed_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_mixed_stats, sizeof(*peer_mixed_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_csi_enable_params (struct wiphy *wiphy,
                                               struct net_device *ndev,
                                               const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 *csi_enable;
  uint32 csiData_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
      WAVE_CORE_REQ_GET_CSI_ENABLE, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
      WAVE_CORE_REQ_GET_CSI_ENABLE, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  csi_enable = mtlk_clpb_enum_get_next(clpb, &csiData_size);
  MTLK_CLPB_TRY(csi_enable, csiData_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, csi_enable, sizeof(uint32));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_csi_auto_params (struct wiphy *wiphy,
                                             struct net_device *ndev,
                                             const uint8 *addr)
{
#define CSI_CFG_SIZE 2
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 (*csi_enable)[CSI_CFG_SIZE];
  uint32 csiData_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
      WAVE_CORE_REQ_GET_CSI_AUTO_RATE, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
      WAVE_CORE_REQ_GET_CSI_AUTO_RATE, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  csi_enable = mtlk_clpb_enum_get_next(clpb, &csiData_size);
  MTLK_CLPB_TRY(csi_enable, csiData_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, csi_enable, sizeof(uint32) * CSI_CFG_SIZE);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_la_mu_groups_counters_stats(struct wiphy *wiphy,
                                                        struct net_device *ndev)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_wssa_drv_la_mu_groups_stats_t *stats = NULL;
  uint32 stats_size = 0;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG3_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_LA_MU_GROUPS_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_LA_MU_GROUPS_STATS, FALSE);

  if (res != MTLK_ERR_OK) {
    goto end;
  }

  stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, stats, sizeof(*stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

  end:
    return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_su_mu_ru_stats(struct wiphy *wiphy,
                                           struct net_device *ndev,
                                           wave_vendor_su_mu_ru_stats_t *data)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 size, payload_size = 0;
  void *ofdma_stats = NULL;

  df_user = mtlk_df_user_from_ndev(ndev);
  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

 if (((data->category != UL_OFDMA_CATEGORY_RADIO_STATS) &&
      (data->category != DL_OFDMA_CATEGORY_RADIO_STATS)) &&
       (!(mtlk_osal_is_valid_ether_addr(data->sta_addr.au8Addr)))) {
    ELOG_Y("Invalid MAC address %Y", data->sta_addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_LA_SU_MU_RU_OFDMA_STATS, &clpb, data, sizeof(wave_vendor_su_mu_ru_stats_t));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_LA_SU_MU_RU_OFDMA_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  switch (data->category) {
  case UL_OFDMA_CATEGORY_STA_STATS:
    payload_size = sizeof(wave_wssa_ofdma_pdu_sta_stats_t);
    break;
  case UL_OFDMA_CATEGORY_RADIO_STATS:
    payload_size = sizeof(wave_wssa_la_ofdma_radio_pdu_stats_t);
    break;
  case DL_OFDMA_CATEGORY_STA_STATS:
    payload_size = sizeof(wave_wssa_ofdma_pdu_sta_stats_t);
    break;
  case DL_OFDMA_CATEGORY_RADIO_STATS:
    payload_size = sizeof(wave_wssa_la_ofdma_radio_pdu_stats_t);
    break;
  }

  ofdma_stats = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY_EX(ofdma_stats, size, payload_size);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ofdma_stats, payload_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_CsiStats (struct wiphy *wiphy,
                                      struct net_device *ndev,
                                      const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wifi_csi_driver_data_t *csi_stats;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_CSI_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_CSI_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  csi_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(csi_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, csi_stats, sizeof(*csi_stats));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_CsiCounters (struct wiphy *wiphy,
                                         struct net_device *ndev,
                                         const uint8 *addr)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  wave_csi_sta_stats64_cntr_t *csi_counters;
  uint32 stats_size;

  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (!mtlk_osal_is_valid_ether_addr(addr)) {
    ELOG_Y("Invalid MAC address %Y", addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_CSI_COUNTERS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_CSI_COUNTERS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  csi_counters = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(csi_counters, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, csi_counters, sizeof(*csi_counters));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_csi_capability (struct wiphy *wiphy,
                                            struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 size;
  wave_radio_csi_slots_stats_t *csi_capability = NULL;

  df_user = mtlk_df_user_from_ndev(ndev);
  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_RADIO_REQ_GET_CSI_CAPABILITY, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_RADIO_REQ_GET_CSI_CAPABILITY, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  csi_capability = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY(csi_capability, size);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, csi_capability, sizeof(*csi_capability));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t wave_radio_get_mu_group_cfg (struct wiphy *wiphy,
                                          struct net_device *ndev,
                                          uint32 *formation_type)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 size;
  mtlk_mu_group_config_params_t *mu_group_cfg;

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  if ((*formation_type >= WAVE_NUM_FORMATION_TYPES)) {
    ELOG_D("Formation type must be from 0 to %d", WAVE_NUM_FORMATION_TYPES - 1);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_RADIO_REQ_GET_MU_GRP_CFG, &clpb, formation_type, sizeof(*formation_type));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_GET_MU_GRP_CFG, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  mu_group_cfg = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY(mu_group_cfg, size);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, mu_group_cfg, sizeof(*mu_group_cfg));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

struct intel_vendor_channel_data * __MTLK_IFUNC
wave_radio_get_aocs_data (wave_radio_t *radio)
{
  return &radio->aocs_data;
}

void __MTLK_IFUNC
wave_radio_init_aocs_scan_data (wave_radio_t *radio)
{
  memset(&radio->aocs_data, 0, sizeof(radio->aocs_data));
  radio->aocs_data.rssi = MIN_RSSI;
  radio->aocs_data.cwi_noise = MIN_NOISE;
}

void __MTLK_IFUNC
wave_radio_copy_chan_stats (wave_radio_t *radio, devicePhyRxStatusDb_t *stats_buff)
{
  wave_hw_copy_chan_stats(radio->hw_api->hw, radio->idx, stats_buff);
}

mtlk_error_t __MTLK_IFUNC
wave_radio_get_radio_peer_list (struct wiphy *wiphy,
                                struct net_device *ndev)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_peer_list_t *radio_peer_list = NULL;
  uint32 size, *no_of_stas;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_RADIO_REQ_GET_RADIO_PEER_LIST, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_RADIO_REQ_GET_RADIO_PEER_LIST, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  no_of_stas = mtlk_clpb_enum_get_next(clpb, &size);
  if ((sizeof(uint32) != size) || (no_of_stas == NULL)) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  if (*no_of_stas == 0) {
    mtlk_clpb_delete(clpb);
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  radio_peer_list = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY_EX(radio_peer_list, size,
                   (*no_of_stas * sizeof(mtlk_wssa_peer_list_t)))
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, radio_peer_list, size);
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

mtlk_error_t __MTLK_IFUNC
wave_radio_get_mimo_stats(struct wiphy *wiphy,
                          struct net_device *ndev,
                          wave_vendor_su_mu_ru_stats_t *data)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 size, payload_size = 0;
  void *ofdma_stats = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  MTLK_ASSERT(data != NULL);
  if (((data->category != UL_OFDMA_CATEGORY_RADIO_STATS) &&
      (data->category != DL_OFDMA_CATEGORY_RADIO_STATS)) &&
       (!(mtlk_osal_is_valid_ether_addr(data->sta_addr.au8Addr)))) {
    ELOG_Y("Invalid MAC address %Y", data->sta_addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_LA_MIMO_OFDMA_STATS, &clpb, data,
                  sizeof(wave_vendor_su_mu_ru_stats_t));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_LA_MIMO_OFDMA_STATS, FALSE);
  if (res != MTLK_ERR_OK)
    goto end;

  switch (data->category) {
  case DL_OFDMA_CATEGORY_STA_STATS:
    payload_size = sizeof(wave_wssa_mimo_sta_stats_t);
    break;
  case DL_OFDMA_CATEGORY_RADIO_STATS:
    payload_size = sizeof(wave_wssa_mimo_radio_stats_t);
    break;
  }

  ofdma_stats = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY_EX(ofdma_stats, size, payload_size);
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ofdma_stats, payload_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

#ifdef CONFIG_WAVE_DEBUG
int wave_radio_get_peer_host_if_qos (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerHostIfQos_t *peer_host_if_qos;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer host in qos data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_HOST_IF_QOS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_HOST_IF_QOS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_host_if_qos = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_host_if_qos, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_host_if_qos, sizeof(*peer_host_if_qos));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_host_if (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerHostIf_t *peer_host_if;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer host in data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_HOST_IF, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_HOST_IF, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_host_if = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_host_if, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_host_if, sizeof(*peer_host_if));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_rx_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerRxStats_t *peer_rx_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer rx stats data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_RX_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_RX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_rx_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_rx_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_rx_stats, sizeof(*peer_rx_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_ul_bsrc_tid_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerUlBsrcTidStats_t *peer_ul_bsrc_tid;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer ul bsrc tid data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_UL_BSRC_TID, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_UL_BSRC_TID, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_ul_bsrc_tid = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_ul_bsrc_tid, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_ul_bsrc_tid, sizeof(*peer_ul_bsrc_tid));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_baa_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerBaaStats_t *peer_baa_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer baa stats data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_BAA_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_BAA_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_baa_stats= mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_baa_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_baa_stats, sizeof(*peer_baa_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_link_adaption_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  linkAdaptionStats_t *link_adaption_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Peer link adaption data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_LINK_ADAPTION_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_LINK_ADAPTION_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  link_adaption_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(link_adaption_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, link_adaption_stats, sizeof(*link_adaption_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_plan_manager_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  planManagerStats_t *plan_manager_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("Plan manager data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PLAN_MANAGER_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PLAN_MANAGER_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  plan_manager_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(plan_manager_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, plan_manager_stats, sizeof(*plan_manager_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_twt_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  twtStats_t *twt_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("TWT stats data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TWT_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TWT_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  twt_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(twt_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, twt_stats, sizeof(*twt_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_per_client_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  perClientStats_t *per_client_stats;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("per client stats data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PER_CLIENT_STATS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PER_CLIENT_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  per_client_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(per_client_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, per_client_stats, sizeof(*per_client_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_phy_rx_status (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerPhyRxStatus_t *phy_rx_status;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("phy rx status data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_PHY_RX_STATUS, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_PHY_RX_STATUS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  phy_rx_status = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(phy_rx_status, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, phy_rx_status, sizeof(*phy_rx_status));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_peer_info (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 *addr = (u8*)data;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  peerInfo_t *peer_info;
  uint32 stats_size;

  if (NULL == data) {
    ELOG_V("peer info data is NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  res = mtlk_osal_valid_input_params(ndev, radio, wiphy, df_user, addr, data_len);
  if(res != MTLK_ERR_OK) {
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_PEER_INFO, &clpb, addr, IEEE_ADDR_LEN);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_PEER_INFO, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  peer_info = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(peer_info, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, peer_info, sizeof(*peer_info));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_wlan_host_if_qos (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_wlan_host_if_qos_t *wlan_host_if_qos;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_WLAN_HOST_IF_QOS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_WLAN_HOST_IF_QOS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  wlan_host_if_qos = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(wlan_host_if_qos, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, wlan_host_if_qos, sizeof(*wlan_host_if_qos));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_wlan_host_if (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_wlan_host_if_t *wlan_host_if;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_WLAN_HOST_IF, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_WLAN_HOST_IF, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  wlan_host_if = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(wlan_host_if, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, wlan_host_if, sizeof(*wlan_host_if));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_wlan_rx_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_wlan_rx_stats_t *wlan_rx_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_WLAN_RX_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_WLAN_RX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  wlan_rx_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(wlan_rx_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, wlan_rx_stats, sizeof(*wlan_rx_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_wlan_baa_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_wlan_baa_stats_t *wlan_baa_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_WLAN_BAA_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_WLAN_BAA_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  wlan_baa_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(wlan_baa_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, wlan_baa_stats, sizeof(*wlan_baa_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_radio_rx_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_radio_rx_stats_t *radio_rx_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_RADIO_RX_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_RADIO_RX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  radio_rx_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(radio_rx_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, radio_rx_stats, sizeof(*radio_rx_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_radio_baa_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_radio_baa_stats_t *radio_baa_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_RADIO_BAA_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_RADIO_BAA_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  radio_baa_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(radio_baa_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, radio_baa_stats, sizeof(*radio_baa_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tsman_init_tid_gl (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tsman_init_tid_gl_t *ts_init_tid_gl;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TSMAN_INIT_TID_GL, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TSMAN_INIT_TID_GL, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  ts_init_tid_gl = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(ts_init_tid_gl, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ts_init_tid_gl, sizeof(*ts_init_tid_gl));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tsman_init_sta_gl (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tsman_init_sta_gl_t *ts_init_sta_gl;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TSMAN_INIT_STA_GL, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TSMAN_INIT_STA_GL, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  ts_init_sta_gl = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(ts_init_sta_gl, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ts_init_sta_gl, sizeof(*ts_init_sta_gl));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}
int wave_radio_get_tsman_rcpt_tid_gl (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tsman_rcpt_tid_gl_t *ts_rcpt_tid_gl;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TSMAN_RCPT_TID_GL, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TSMAN_RCPT_TID_GL, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  ts_rcpt_tid_gl = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(ts_rcpt_tid_gl, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ts_rcpt_tid_gl, sizeof(*ts_rcpt_tid_gl));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_tsman_rcpt_sta_gl (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_tsman_rcpt_sta_gl_t *ts_rcpt_sta_gl;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TSMAN_RCPT_STA_GL, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TSMAN_RCPT_STA_GL, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  ts_rcpt_sta_gl = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(ts_rcpt_sta_gl, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ts_rcpt_sta_gl, sizeof(*ts_rcpt_sta_gl));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_radio_link_adapt_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_radio_link_adapt_stats_t *link_adapt_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_RADIO_LINK_ADAPT_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_RADIO_LINK_ADAPT_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  link_adapt_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(link_adapt_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, link_adapt_stats, sizeof(*link_adapt_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}
int wave_radio_get_multicast_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_multicast_stats_t *mc_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_MULTICAST_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_MULTICAST_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  mc_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(mc_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, mc_stats, sizeof(*mc_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_training_man_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_training_man_stats_t *trman_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_TR_MAN_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_TR_MAN_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  trman_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(trman_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, trman_stats, sizeof(*trman_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_group_man_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_group_man_stats_t *grman_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_GRP_MAN_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_GRP_MAN_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  grman_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(grman_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, grman_stats, sizeof(*grman_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_general_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_general_stats_t *gen_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_GENERAL_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_GENERAL_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  gen_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(gen_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, gen_stats, sizeof(*gen_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_cur_channel_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_cur_channel_stats_t *channel_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_CUR_CHANNEL_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_CUR_CHANNEL_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  channel_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(channel_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, channel_stats, sizeof(*channel_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_radio_phy_rx_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_radio_phy_rx_stats_t *phy_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_RADIO_PHY_RX_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_RADIO_PHY_RX_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  phy_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(phy_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, phy_stats, sizeof(*phy_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_dynamic_bw_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_dynamic_bw_stats_t *dy_bw_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_DYNAMIC_BW_STATS, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_DYNAMIC_BW_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  dy_bw_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(dy_bw_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, dy_bw_stats, sizeof(*dy_bw_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int wave_radio_get_la_mu_vht_stats (
  wave_radio_t *radio,
  struct net_device *ndev,
  const void *data,
  int data_len)
{
  void *wiphy;
  u8 mu;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  mtlk_wssa_drv_radio_la_mu_vht_stats_t *link_mu_stats;
  uint32 stats_size;

  wiphy = wv_mac80211_wiphy_get(radio->mac80211);
  MTLK_ASSERT(NULL != wiphy);
  if (NULL == wiphy) {
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  MTLK_ASSERT(NULL != radio);
  df_user = mtlk_df_user_from_ndev(ndev);

  ILOG1_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  MTLK_CHECK_DF_USER(df_user);

  if (NULL == data) {
    ELOG_V("MU group cannot be NULL");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  mu = *(u8*)data;
  if (mu > 0x15) {
    ELOG_V("Invalid MU Group");
    res = MTLK_ERR_PARAMS;
    goto end;
  }
  if(mu >= 0x10) {
    mu = mu-6;
  }
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_LA_MU_VHT_STATS, &clpb, &mu, sizeof(mu));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_LA_MU_VHT_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  link_mu_stats = mtlk_clpb_enum_get_next(clpb, &stats_size);

  MTLK_CLPB_TRY(link_mu_stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, link_mu_stats, sizeof(*link_mu_stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

void __MTLK_IFUNC wave_radio_on_rcvry_reset_mu_group_stats (wave_radio_t *radio)
{
  UMI_DBG_HE_MU_GROUP_STATS HeMuGroupStats[HE_MU_MAX_NUM_OF_GROUPS];
  mtlk_pdb_size_t HeMuGroupStatsSize = sizeof(HeMuGroupStats);

  if (NULL == radio) {
    ELOG_V("radio is NULL");
    return;
  }

  memset(&HeMuGroupStats[0], 0, HeMuGroupStatsSize);

  if (MTLK_ERR_OK != WAVE_RADIO_PDB_SET_BINARY(radio, PARAM_DB_RADIO_PLAN_MU_GROUP_STATS, &HeMuGroupStats[0], HeMuGroupStatsSize))
    ELOG_D("RadioId %d: Error in reset PLAN_MU_GROUP_STATS to PDB", radio->idx);
}
#endif /* CONFIG_WAVE_DEBUG */

/* UL and DL PPDU OFDMA stats */
void __MTLK_IFUNC
wave_hw_get_radio_ul_ofdma_stats_64 (wave_radio_t *radio, wave_radio_pdus_ofdma_stats64_t **stats)
{
  *stats = &radio->ul_radio_ofdma_stats_64;
}

void __MTLK_IFUNC
wave_hw_get_radio_dl_ofdma_stats_64 (wave_radio_t *radio, wave_radio_pdus_ofdma_stats64_t **stats)
{
  *stats = &radio->dl_radio_ofdma_stats_64;
}

void __MTLK_IFUNC
wave_hw_get_radio_ul_ofdma_stats (wave_radio_t *radio, wave_radio_pdus_ofdma_stats_t **stats)
{
  *stats = &radio->ul_radio_ofdma_stats;
}

void __MTLK_IFUNC
wave_hw_get_radio_dl_ofdma_stats (wave_radio_t *radio, wave_radio_pdus_ofdma_stats_t **stats)
{
  *stats = &radio->dl_radio_ofdma_stats;
}
/* HE-MIMO stats */
void __MTLK_IFUNC
wave_get_radio_dl_mimo_stats (wave_radio_t *radio, wave_radio_mimo_stats_t **stats)
{
  MTLK_ASSERT(NULL != radio);
  *stats = &radio->dl_radio_mimo_stats;
}

void __MTLK_IFUNC
wave_get_radio_dl_mimo_stats_64 (wave_radio_t *radio, wave_radio_mimo_stats64_t **stats)
{
  MTLK_ASSERT(NULL != radio);
  *stats = &radio->dl_radio_mimo_stats_64;
}

void __MTLK_IFUNC
wave_radio_reset_ofdma_stats (wave_radio_t *radio)
{
  MTLK_ASSERT(NULL != radio);
  memset(&radio->ul_radio_ofdma_stats_64, 0, sizeof(radio->ul_radio_ofdma_stats_64));
  memset(&radio->ul_radio_ofdma_stats, 0, sizeof(radio->ul_radio_ofdma_stats));
  memset(&radio->dl_radio_ofdma_stats_64, 0, sizeof(radio->dl_radio_ofdma_stats_64));
  memset(&radio->dl_radio_ofdma_stats, 0, sizeof(radio->dl_radio_ofdma_stats));
  memset(&radio->dl_radio_mimo_stats_64, 0, sizeof(radio->dl_radio_mimo_stats_64));
  memset(&radio->dl_radio_mimo_stats, 0, sizeof(radio->dl_radio_mimo_stats));
}

/* Must be called from master VAP serializer context */
void __MTLK_IFUNC
wv_cfg80211_radio_ch_switch_notify (wave_radio_t *radio, struct mtlk_chan_def *ccd)
{
  mtlk_core_t *core;
  int i, max_vaps_count;
  mtlk_vap_handle_t vap_handle;
  struct cfg80211_chan_def chandef = {0};
  mtlk_vap_manager_t *vap_mgr = wave_radio_vap_manager_get(radio);

  chandef.chan = ieee80211_get_channel(wv_mac80211_wiphy_get(radio->mac80211), ccd->chan.center_freq);
  chandef.width = mtlkcw2nlcw(ccd->width, ccd->is_noht);
  chandef.center_freq1 = ccd->center_freq1;
  chandef.center_freq2 = ccd->center_freq2;

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(vap_mgr);
  for (i = 0; i < max_vaps_count; i++) {
    if (MTLK_ERR_OK != mtlk_vap_manager_get_vap_handle(vap_mgr, i, &vap_handle))
      continue;

    if (!mtlk_vap_is_ap(vap_handle))
      continue;

    core = mtlk_vap_get_core(vap_handle);
    if (NET_STATE_CONNECTED != mtlk_core_get_net_state(core))
      continue;

    cfg80211_ch_switch_notify(mtlk_df_user_get_ndev(mtlk_df_get_user(mtlk_vap_get_df(vap_handle))), &chandef, 0, 0);
  }
}

BOOL __MTLK_IFUNC
wave_radio_get_is_zwdfs_background_supp (wave_radio_t *radio)
{
  mtlk_hw_t *hw;
  mtlk_hw_band_e band;
  hw_psdb_t *psdb;

  MTLK_ASSERT(radio != NULL);
  hw = radio->hw_api->hw;

  if (_chipid_is_gen7(hw_mmb_get_chip_id(hw))) {
    psdb = mtlk_hw_get_psdb(hw);
    band = wave_radio_band_get(radio);
    /* zwdfs antenna mask is enabled in psdb file for wav700 5G radio. Then background zwdfs antenna chain is supported on the operational radio */
    if ((psdb->cdb_cfg_table.zwdfs_ant_mask) && (band == MTLK_HW_BAND_5_2_GHZ)) {
      return TRUE;
    }
  }

  return FALSE;
}

mtlk_hw_t * __MTLK_IFUNC wave_radio_get_hw (wave_radio_t *radio)
{
  return radio->hw_api->hw;
}

#ifdef CONFIG_THERMAL
void __MTLK_IFUNC
wave_radio_set_thermal_mgmt_data (wave_radio_t *radio, wave_thermal_t *thermal_mgmt)
{
  MTLK_ASSERT(radio);
  radio->thermal_mgmt = thermal_mgmt;
}

wave_thermal_t*  __MTLK_IFUNC
wave_radio_get_thermal_mgmt_data (wave_radio_t *radio)
{
  MTLK_ASSERT(radio);
  return radio->thermal_mgmt;
}
#endif

struct ieee80211_regdomain * __MTLK_IFUNC
wave_copy_regd(const struct ieee80211_regdomain *src_regd)
{
  struct ieee80211_regdomain *regd;
  mtlk_pdb_size_t size;

  size = struct_size(regd, reg_rules, src_regd->n_reg_rules);
  regd = mtlk_osal_mem_alloc(size , MTLK_MEM_TAG_CFG80211);

  if (!regd)
    return NULL;

  size = sizeof(struct ieee80211_regdomain);
  wave_memcpy(regd, size, src_regd, size);

  size = src_regd->n_reg_rules * sizeof(struct ieee80211_reg_rule);
  wave_memcpy(regd->reg_rules, size, src_regd->reg_rules, size);

  return regd;
}

void __MTLK_IFUNC wv_set_regd_flag(struct ieee80211_regdomain *regd, u32 rrule_flags)
{
  unsigned int i;

  if (!regd)
    return ;

  for (i = 0; i < regd->n_reg_rules; i++)
    regd->reg_rules[i].flags = rrule_flags;
}

#ifdef MTLK_WAVE_700
uint8 wave_radio_get_rx_ant (wave_radio_t *radio)
{
    MTLK_ASSERT(radio);
    return (radio->ant_params.rx_ant_num);
}
#endif

