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
 * Core configuration implementation
 *
 */
#include "mtlkinc.h"
#include "core.h"
#include "core_stats.h"
#include "core_config.h"
#include "mtlk_vap_manager.h"
#include "eeprom.h"
#include "hw_mmb.h"
#include "mtlkaux.h"
#include "mtlk_df_user_priv.h"
#include "mtlkdfdefs.h"
#include "mhi_umi.h"
#include "mhi_umi_propr.h"
#include "scan_support.h"
#include "mtlk_df_priv.h"
#include "mtlk_df.h"
#include "cfg80211.h"
#include "mac80211.h"
#include "mcast.h"
#include "wave_hal_stats.h"
#include "bitrate.h"
#include <linux/jiffies.h>
#include "wave_radio.h"

#define LOG_LOCAL_GID   GID_CORE
#define LOG_LOCAL_FID   5

/* convert TID to AC   */
static const uint8 convert_tid_to_AC[HAL_NUM_OF_TIDS] = {
    WIFI_RADIO_QUEUE_TYPE_BE,
    WIFI_RADIO_QUEUE_TYPE_BK,
    WIFI_RADIO_QUEUE_TYPE_BK,
    WIFI_RADIO_QUEUE_TYPE_BE,
    WIFI_RADIO_QUEUE_TYPE_VI,
    WIFI_RADIO_QUEUE_TYPE_VI,
    WIFI_RADIO_QUEUE_TYPE_VO,
    WIFI_RADIO_QUEUE_TYPE_VO,
    WIFI_RADIO_QUEUE_TYPE_BE,
    WIFI_RADIO_QUEUE_TYPE_BK,
    WIFI_RADIO_QUEUE_TYPE_BK,
    WIFI_RADIO_QUEUE_TYPE_BE,
    WIFI_RADIO_QUEUE_TYPE_VI,
    WIFI_RADIO_QUEUE_TYPE_VI,
    WIFI_RADIO_QUEUE_TYPE_VO,
    WIFI_RADIO_QUEUE_TYPE_VO,
};

static inline unsigned int mtlk_nsecs_to_msecs(u64 nsecs)
{
  return jiffies_to_msecs(nsecs_to_jiffies(nsecs));
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_get_statistics (mtlk_core_t *nic, mtlk_hw_t *hw)
{
  int state = mtlk_core_get_net_state(nic);
  if (state == NET_STATE_ACTIVATING || state == NET_STATE_CONNECTED) {
    mtlk_hw_set_stats_available(hw, TRUE);
    return mtlk_hw_get_statistics(hw);
  }
  return MTLK_ERR_OK;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_get_station_measurements (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  struct driver_sta_info sta_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)
    if (mtlk_vap_is_ap(core->vap_handle) && !wave_core_sync_hostapd_done_get(core)) {
      ILOG1_D("CID-%04x: HOSTAPD STA database is not synced", mtlk_vap_get_oid(core->vap_handle));
      MTLK_CLPB_EXIT(MTLK_ERR_NOT_READY);
    }

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    }

    mtlk_core_get_driver_sta_info(sta, &sta_info);
    sta_info.max_rate = MTLK_BITRATE_TO_KBPS(sta->info.bitrate_max);

  MTLK_CLPB_FINALLY(res)
    if (sta)
      mtlk_sta_decref(sta);
    return mtlk_clpb_push_res_data(clpb, res, &sta_info,
      sizeof(sta_info));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_associated_dev_tid_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerTidStats tid_stats;
  int tid_idx;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      memset(&tid_stats, 0, sizeof(peerTidStats));
      /* Plume needs the following per TID stats, however FW maintains per STA
         Thus we are filling the same statistic for all 16 TIDs                */
      for (tid_idx = 0; tid_idx < HAL_NUM_OF_TIDS; tid_idx++) {
        /* Number of msdus in given interval for per STA */
        tid_stats[tid_idx].num_msdus    = sta->sta_stats64_cntrs.numMsdus;
        /* Moving average value based on last couple of transmitted msdus for per station */
        tid_stats[tid_idx].ewma_time_ms = mtlk_nsecs_to_msecs((uint64)sta->sta_stats_cntrs.ewmaTimeNsec);
        /* Delta of cumulative msdus times over interval for per station  */
        tid_stats[tid_idx].sum_time_ms  = mtlk_nsecs_to_msecs(sta->sta_stats64_cntrs.sumTimeNsec);
        tid_stats[tid_idx].tid = tid_idx;
        tid_stats[tid_idx].ac  = convert_tid_to_AC[tid_idx];
      }
      mtlk_sta_decref(sta);
  }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &tid_stats, sizeof(peerTidStats));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_wifi_channel_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  wifi_channelStats_t *chan_stats = NULL;
  int res = MTLK_ERR_OK;
  uint32 num_of_channels = 0, channels_list_size, chan_idx, *channels_list;
  size_t total_size = 0;
  mtlk_hw_t *hw = NULL;

  channels_list = mtlk_clpb_enum_get_next(clpb, &channels_list_size);
  num_of_channels = channels_list_size/(sizeof(*channels_list));
  MTLK_CLPB_TRY_EX(channels_list, channels_list_size, sizeof(*channels_list)*num_of_channels)

    hw = mtlk_vap_get_hw(core->vap_handle);

    total_size = sizeof(chan_stats[0]) * num_of_channels;
    chan_stats = mtlk_osal_mem_alloc(total_size, MTLK_MEM_TAG_EXTENSION);
    if (NULL == chan_stats) {
      ELOG_V("Can't allocate memory");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(chan_stats, 0, total_size);

    for (chan_idx = 0; chan_idx < num_of_channels; chan_idx++) {
      mtlk_hw_copy_stats_per_channel(wave_vap_radio_get(core->vap_handle), &chan_stats[chan_idx], channels_list[chan_idx]);
    }
  MTLK_CLPB_FINALLY(res)
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, chan_stats, total_size);
    if (MTLK_ERR_OK != res && chan_stats != NULL) {
      mtlk_osal_mem_free(chan_stats);
    }
    return res;
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_drv_channel_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  wave_drv_channel_stats_t *chan_stats = NULL;
  int res = MTLK_ERR_OK;
  uint32 num_of_channels = 0, channels_list_size, chan_idx, *channels_list;
  size_t total_size = 0;
  mtlk_hw_t *hw = NULL;

  channels_list = mtlk_clpb_enum_get_next(clpb, &channels_list_size);
  num_of_channels = channels_list_size/(sizeof(*channels_list));
  MTLK_CLPB_TRY_EX(channels_list, channels_list_size, sizeof(*channels_list)*num_of_channels)

    hw = mtlk_vap_get_hw(core->vap_handle);

    total_size = sizeof(chan_stats[0]) * num_of_channels;
    chan_stats = mtlk_osal_mem_alloc(total_size, MTLK_MEM_TAG_EXTENSION);
    if (NULL == chan_stats) {
      ELOG_V("Can't allocate memory");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(chan_stats, 0, total_size);

    for (chan_idx = 0; chan_idx < num_of_channels; chan_idx++) {
      mtlk_hw_copy_drv_stats_per_channel(wave_vap_radio_get(core->vap_handle), &chan_stats[chan_idx], channels_list[chan_idx]);
    }
  MTLK_CLPB_FINALLY(res)
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, chan_stats, total_size);
    if (MTLK_ERR_OK != res && chan_stats != NULL) {
      mtlk_osal_mem_free(chan_stats);
    }
    return res;
  MTLK_CLPB_END;
}

static const uint16 hal_cbw[] = {HAL_BW_20MHZ, HAL_BW_40MHZ, HAL_BW_80MHZ, HAL_BW_160MHZ};
void __MTLK_IFUNC
mtlk_core_get_associated_dev_rate_info_rx_stats (sta_entry * sta, peerRateInfoRxStats * peer_rx_rate_info)
{
  uint8 cbw, i, mode;
  uint16 psdu_rate;
#ifdef MTLK_WAVE_700
  mtlk_core_t *mcore = mtlk_core_get_master(sta->vap_handle);
  wave_radio_t *radio = wave_vap_radio_get(mcore->vap_handle);
  MTLK_ASSERT((mcore != NULL) && (radio != NULL));
  if (wave_radio_is_gen7(radio)) {
    psdu_rate = sta->info.stats.rx_psdu_data_rate_info & 0xFFFF;
    cbw  = mtlk_bitrate_params_get_psdu_cbw_g7(psdu_rate);
    mode = mtlk_bitrate_params_get_psdu_mode_g7(psdu_rate);
  }
  else
#endif
  {
    psdu_rate = sta->info.stats.rx_psdu_data_rate_info;
    cbw = mtlk_bitrate_params_get_psdu_cbw(sta->info.stats.rx_psdu_data_rate_info);
    mode = mtlk_bitrate_params_get_psdu_mode(sta->info.stats.rx_psdu_data_rate_info);
  }

  memset(peer_rx_rate_info, 0, sizeof(*peer_rx_rate_info));

  /* Handling mcs seperately incase of HT(11n) mode because WIFI HAL expects mcs value <0-7> for HT(11n)
    * but as per statndard HT mcs value is <0-32> */
  if (PHY_MODE_N == mode){
    mtlk_bitrate_params_get_11n_mcs_and_nss_for_plume(sta->info.stats.rx_psdu_data_rate_info, &peer_rx_rate_info->mcs, &peer_rx_rate_info->nss);
  } else {
    mtlk_bitrate_params_get_psdu_mcs_and_nss(mode, psdu_rate, &peer_rx_rate_info->mcs, &peer_rx_rate_info->nss);
  }

  peer_rx_rate_info->bw = hal_cbw[cbw];
  peer_rx_rate_info->bytes = sta->sta_stats64_cntrs.rxOutStaNumOfBytes;
  peer_rx_rate_info->flags |= BIT_ULL(HAS_BYTES);
  peer_rx_rate_info->msdus = sta->sta_stats64_cntrs.rdCount;
  peer_rx_rate_info->flags |= BIT_ULL(HAS_MSDUS);
  peer_rx_rate_info->mpdus = sta->sta_stats64_cntrs.mpduInAmpdu;
  peer_rx_rate_info->flags |= BIT_ULL(HAS_MPDUS);
  peer_rx_rate_info->ppdus = sta->sta_stats64_cntrs.ampdu;
  peer_rx_rate_info->flags |= BIT_ULL(HAS_PPDUS);
  peer_rx_rate_info->retries = sta->sta_stats64_cntrs.rxRetryCount;
  peer_rx_rate_info->rssi_combined = sta->info.stats.max_rssi;
  peer_rx_rate_info->flags |= BIT_ULL(HAS_RSSI_COMB);
  for (i = 0; i < MAX_NUM_RX_ANTENNAS; i++) {
    peer_rx_rate_info->rssi_array[i][0] = sta->info.stats.data_rssi[i];
  }
  peer_rx_rate_info->flags |= BIT_ULL(HAS_RSSI_ARRAY);
}

int __MTLK_IFUNC
mtlk_core_get_associated_dev_rate_info_rx_stats_clb (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerRateInfoRxStats peer_rx_rate_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_core_get_associated_dev_rate_info_rx_stats(sta, &peer_rx_rate_info);
      mtlk_sta_decref(sta);
    }
  MTLK_CLPB_FINALLY(res)
  return mtlk_clpb_push_res_data(clpb, res, &peer_rx_rate_info, sizeof(peerRateInfoRxStats));
  MTLK_CLPB_END;
}

void __MTLK_IFUNC
mtlk_core_calc_associated_dev_rate_info_rx_stats (peerRateInfoRxStats * dest, peerRateInfoRxStats * new, peerRateInfoRxStats * old)
{
  dest->bytes += new->bytes - old->bytes;
  dest->msdus += new->msdus - old->msdus;
  dest->mpdus += new->mpdus - old->mpdus;
  dest->ppdus += new->ppdus - old->ppdus;
  dest->retries += new->retries - old->retries;
  dest->rssi_combined = new->rssi_combined;
  wave_memcpy(dest->rssi_array, sizeof(dest->rssi_array), new->rssi_array, sizeof(new->rssi_array));
}

void __MTLK_IFUNC
mtlk_core_get_associated_dev_rate_info_tx_stats (sta_entry * sta, peerRateInfoTxStats * peer_tx_rate_info)
{
  uint8 cbw, mode;
  mode = mtlk_bitrate_params_get_mode(sta->info.stats.tx_data_rate_params);

  memset(peer_tx_rate_info, 0, sizeof(*peer_tx_rate_info));

  if ((mode == PHY_MODE_AG) || (mode == PHY_MODE_B)) {
    peer_tx_rate_info->nss = HAL_LEGACY_NSS;
  } else {
    peer_tx_rate_info->nss = mtlk_bitrate_params_get_nss(sta->info.stats.tx_data_rate_params);
  }

  peer_tx_rate_info->mcs = mtlk_bitrate_params_get_mcs(sta->info.stats.tx_data_rate_params);
  cbw = mtlk_bitrate_params_get_cbw(sta->info.stats.tx_data_rate_params);
  peer_tx_rate_info->bw = hal_cbw[cbw];
  peer_tx_rate_info->bytes = sta->sta_stats64_cntrs.mpduByteTransmitted;
  peer_tx_rate_info->flags |= BIT_ULL(HAS_BYTES);
  peer_tx_rate_info->msdus = sta->sta_stats64_cntrs.successCount + sta->sta_stats64_cntrs.exhaustedCount;
  peer_tx_rate_info->flags |= BIT_ULL(HAS_MSDUS);
  peer_tx_rate_info->mpdus = sta->sta_stats64_cntrs.mpduTransmitted;
  peer_tx_rate_info->flags |= BIT_ULL(HAS_MPDUS);
  peer_tx_rate_info->ppdus = sta->sta_stats64_cntrs.transmittedAmpdu;
  peer_tx_rate_info->flags |= BIT_ULL(HAS_PPDUS);
  peer_tx_rate_info->retries = sta->sta_stats64_cntrs.txRetryCount;
  peer_tx_rate_info->attempts = sta->sta_stats64_cntrs.mpduFirstRetransmission;
}

int __MTLK_IFUNC
mtlk_core_get_associated_dev_rate_info_tx_stats_clb (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerRateInfoTxStats peer_tx_rate_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_core_get_associated_dev_rate_info_tx_stats(sta, &peer_tx_rate_info);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
  return mtlk_clpb_push_res_data(clpb, res, &peer_tx_rate_info, sizeof(peerRateInfoTxStats));
  MTLK_CLPB_END;
}

void __MTLK_IFUNC
mtlk_core_calc_associated_dev_rate_info_tx_stats (peerRateInfoTxStats * dest, peerRateInfoTxStats * new, peerRateInfoTxStats * old)
{
  dest->bytes += new->bytes - old->bytes;
  dest->msdus += new->msdus - old->msdus;
  dest->mpdus += new->mpdus - old->mpdus;
  dest->retries += new->retries - old->retries;
  dest->attempts += new->attempts - old->attempts;
}

int __MTLK_IFUNC
mtlk_core_get_associated_dev_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerFlowStats peer_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      memset(&peer_stats , 0, sizeof(peerFlowStats));
      mtlk_sta_get_associated_dev_stats(sta, &peer_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_stats, sizeof(peerFlowStats));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_stats_poll_period_get (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  int res = MTLK_ERR_OK;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_hw_t *hw = NULL;
  uint32 poll_period = 0;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  hw = mtlk_vap_get_hw(core->vap_handle);
  MTLK_ASSERT(hw);

  /* read config from internal db */
  poll_period = wave_hw_mmb_get_stats_poll_period(hw);

  /* push result and config to clipboard*/
  res = mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &poll_period,
    sizeof(poll_period));

  return res;
}

int __MTLK_IFUNC
mtlk_core_stats_poll_period_set (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  int res = MTLK_ERR_OK;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_df_t *df = mtlk_vap_get_df(core->vap_handle);
  mtlk_df_user_t *df_user = mtlk_df_get_user(df);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  uint32 *poll_period = NULL, poll_period_size;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  /* get configuration */
  poll_period = mtlk_clpb_enum_get_next(clpb, &poll_period_size);
  MTLK_CLPB_TRY(poll_period, poll_period_size)
    wave_hw_mmb_set_stats_poll_period(mtlk_vap_get_hw(core->vap_handle), *poll_period);
    mtlk_df_user_start_stats_timer(df_user, *poll_period);
  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res(clpb, res);
  MTLK_CLPB_END
}

int __MTLK_IFUNC
mtlk_core_get_peer_list (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *nic = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;

  if (0 == (mtlk_core_get_net_state(nic) & (NET_STATE_HALTED | NET_STATE_CONNECTED))) {
    return MTLK_ERR_OK;
  }

  return mtlk_stadb_get_peer_list(&nic->slow_ctx->stadb, clpb);
}

int __MTLK_IFUNC
wave_core_probe_req_list_get(mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  wave_radio_t *radio = wave_vap_radio_get(core->vap_handle);
  int res = MTLK_ERR_OK;
  mtlk_error_t ret;
  probe_req_info info;
  BOOL *flush_probe_list;
  uint32 flush_probe_list_size;

  memset(&info, 0, sizeof(info));

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  flush_probe_list = mtlk_clpb_enum_get_next(clpb, &flush_probe_list_size);
  MTLK_CLPB_TRY(flush_probe_list, flush_probe_list_size)
    res = wave_radio_copy_probe_req_list(radio, &info);

    if (MTLK_ERR_OK == res) {
      ILOG2_D("flush probe request list = %u", *flush_probe_list);
      /* flush probe request list if queried by hostapd timer
         if queried by iw command, would not flush probe request list */
      if (*flush_probe_list == TRUE ) {
        wave_radio_clean_probe_req_list(radio, TRUE);
      }
    }

  MTLK_CLPB_FINALLY(res)
    ret = mtlk_clpb_push_res_data(clpb, res, &info, sizeof(probe_req_info));
  MTLK_CLPB_END

  if ((res != MTLK_ERR_OK || ret != MTLK_ERR_OK) && info.data) {
    mtlk_osal_mem_free(info.data);
  }
  return ret;
}

int __MTLK_IFUNC
mtlk_core_get_peer_flow_status (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  mtlk_wssa_drv_peer_stats_t peer_flow_status;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_stats(sta, &peer_flow_status);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_flow_status, sizeof(mtlk_wssa_drv_peer_stats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_peer_capabilities (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  mtlk_wssa_drv_peer_capabilities_t peer_capabilities;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_capabilities(sta, &peer_capabilities);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_capabilities, sizeof(mtlk_wssa_drv_peer_capabilities_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_peer_rate_info (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  mtlk_wssa_drv_peer_rates_info_t peer_rate_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      _mtlk_sta_get_peer_rates_info(sta, &peer_rate_info);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_rate_info, sizeof(mtlk_wssa_drv_peer_rates_info_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_recovery_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_recovery_stats_t recovery_stats;
  mtlk_hw_t *hw = NULL;

  hw = mtlk_vap_get_hw(core->vap_handle);
  MTLK_ASSERT(hw);

  wave_hw_get_recovery_stats(hw, &recovery_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &recovery_stats, sizeof(mtlk_wssa_drv_recovery_stats_t));
}

int __MTLK_IFUNC
mtlk_core_get_hw_flow_status (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  wave_radio_t  *radio = wave_vap_radio_get(core->vap_handle);
  mtlk_wssa_drv_hw_stats_t hw_flow_status;

  wave_radio_get_hw_stats(radio, &hw_flow_status);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &hw_flow_status, sizeof(mtlk_wssa_drv_hw_stats_t));
}

int __MTLK_IFUNC
mtlk_core_get_tr181_wlan_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_tr181_wlan_stats_t tr181_stats;

  mtlk_core_get_tr181_wlan_stats(core, &tr181_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &tr181_stats, sizeof(mtlk_wssa_drv_tr181_wlan_stats_t));
}

int __MTLK_IFUNC
mtlk_core_get_tr181_hw_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  wave_radio_t  *radio = wave_vap_radio_get(core->vap_handle);
  mtlk_wssa_drv_tr181_hw_stats_t tr181_stats;

  wave_radio_get_tr181_hw_stats(radio, &tr181_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &tr181_stats, sizeof(mtlk_wssa_drv_tr181_hw_stats_t));
}

int __MTLK_IFUNC
mtlk_core_get_tr181_peer_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  st_info_data_t *info_data;
  unsigned info_data_size;
  sta_entry *sta = NULL;
  mtlk_wssa_drv_tr181_peer_stats_t tr181_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  info_data = mtlk_clpb_enum_get_next(clpb, &info_data_size);
  MTLK_CLPB_TRY(info_data, info_data_size)

  if (info_data->sta)
    sta = wv_ieee80211_get_sta(info_data->sta);
  else
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, info_data->mac);

  if (NULL == sta) {
    ILOG1_DY("CID-%04x: station %Y not found",
               mtlk_vap_get_oid(core->vap_handle), info_data->mac);
    MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
  } else {
      mtlk_sta_get_tr181_peer_stats(sta, &tr181_stats);
      mtlk_sta_decref(sta);
  }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &tr181_stats, sizeof(mtlk_wssa_drv_tr181_peer_stats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_dev_diag_result2 (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  wifiAssociatedDevDiagnostic2_t dev_diag_res2_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      wave_sta_get_dev_diagnostic_res2(core, sta, &dev_diag_res2_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &dev_diag_res2_stats, sizeof(wifiAssociatedDevDiagnostic2_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_dev_diag_result3 (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  wifiAssociatedDevDiagnostic3_t *dev_diag_res3_stats = NULL;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)
    dev_diag_res3_stats = mtlk_osal_mem_alloc(sizeof(wifiAssociatedDevDiagnostic3_t), MTLK_MEM_TAG_EXTENSION);
    if (NULL == dev_diag_res3_stats) {
      ELOG_V("Can't allocate memory for dev_diag_res3_stats struct");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(dev_diag_res3_stats, 0, sizeof(wifiAssociatedDevDiagnostic3_t));

    /* find station in stadb */
    sta = mtlk_vap_find_sta(core->vap_handle, addr->au8Addr);
    if (NULL == sta) {
      ILOG1_DY("CID-%04x: station %Y not found", mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      wave_sta_get_dev_diagnostic_res3(core, sta, dev_diag_res3_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, dev_diag_res3_stats, sizeof(wifiAssociatedDevDiagnostic3_t));
    if (MTLK_ERR_OK != res && dev_diag_res3_stats != NULL) {
      mtlk_osal_mem_free(dev_diag_res3_stats);
    }
    return res;
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_associated_stations_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr_list, *addr;
  uint32 i, addr_list_size, num_addr = 0, alloc_size = 0;
  sta_entry *sta = NULL;
  wifiAssociatedStationStats_t *assoc_stations_stats = NULL, *assoc_sta_stats;
  mtlk_wssa_drv_peer_rates_info_t peer_rates_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr_list = mtlk_clpb_enum_get_next(clpb, &addr_list_size);
  if (addr_list_size <= sizeof(IEEE_ADDR))
    num_addr = 1;
  else
    num_addr = addr_list_size / sizeof(IEEE_ADDR);

  MTLK_CLPB_TRY_EX(addr_list, addr_list_size, (sizeof(IEEE_ADDR) * num_addr))
    alloc_size = sizeof(wifiAssociatedStationStats_t) * num_addr;
    assoc_stations_stats = mtlk_osal_mem_alloc(alloc_size, MTLK_MEM_TAG_EXTENSION);
    if (NULL == assoc_stations_stats) {
      ELOG_V("Can't allocate memory for assoc_stations_stats struct");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(assoc_stations_stats, 0, alloc_size);

    for (i = 0; i < num_addr; ++i) {
      /* find station in stadb */
      addr = &addr_list[i];
      assoc_sta_stats = &assoc_stations_stats[i];

      sta = mtlk_vap_find_sta(core->vap_handle, addr->au8Addr);
      if (NULL == sta) {
        ILOG2_DY("CID-%04x: station %Y not found", mtlk_vap_get_oid(core->vap_handle), addr);
      } else {
        int j;

        _mtlk_sta_get_peer_rates_info(sta, &peer_rates_info);

        assoc_sta_stats->BytesSent = sta->sta_stats64_cntrs.mpduByteTransmitted;
        assoc_sta_stats->BytesReceived = sta->sta_stats64_cntrs.rxOutStaNumOfBytes;

        /* TX */
        assoc_sta_stats->TxDataRateInfo.InfoFlag = peer_rates_info.tx_data_rate_info.InfoFlag;
        assoc_sta_stats->TxDataRateInfo.DataRate = peer_rates_info.TxDataRate;
        assoc_sta_stats->TxDataRateInfo.CbwIdx = peer_rates_info.tx_data_rate_info.CbwIdx;
        assoc_sta_stats->TxDataRateInfo.Nss = peer_rates_info.tx_data_rate_info.Nss;
        assoc_sta_stats->TxDataRateInfo.Sgi = wave_short_gi_from_tx_data_rate_info(&peer_rates_info.tx_data_rate_info);

        /* RX */
        assoc_sta_stats->RxDataRateInfo.InfoFlag = peer_rates_info.rx_data_rate_info.InfoFlag;
        assoc_sta_stats->RxDataRateInfo.DataRate = peer_rates_info.RxDataRate;
        assoc_sta_stats->RxDataRateInfo.CbwIdx = peer_rates_info.rx_data_rate_info.CbwIdx;
        assoc_sta_stats->RxDataRateInfo.Nss = peer_rates_info.rx_data_rate_info.Nss;
        assoc_sta_stats->TxDataRateInfo.Sgi = wave_short_gi_from_rx_data_rate_info(&peer_rates_info.rx_data_rate_info);

        MTLK_STATIC_ASSERT(NUMBER_OF_RX_ANTENNAS == ARRAY_SIZE(assoc_sta_stats->RSSI));
        for (j = 0; j < NUMBER_OF_RX_ANTENNAS; ++j) {
          assoc_sta_stats->RSSI[j] = mtlk_sta_get_short_term_rssi(sta, j);
        }

        /* Let upper layer know the station was found, and statistics were filled.
         * This is needed since we don't wish to fail the call if one or more mac addr
         * are no longer connected to driver. (not found) */
        assoc_sta_stats->IsFilled = 1;

        mtlk_sta_decref(sta);
      }
    }

  MTLK_CLPB_FINALLY(res)
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, assoc_stations_stats, alloc_size);
    if (MTLK_ERR_OK != res && assoc_stations_stats != NULL) {
      mtlk_osal_mem_free(assoc_stations_stats);
    }
    return res;
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
mtlk_core_get_priority_gpio_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_hw_t *hw = NULL;
  mtlk_priority_gpio_stats_t pta_stats;

  hw = mtlk_vap_get_hw(core->vap_handle);
  mtlk_hw_get_priority_gpio_stats(hw, &pta_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &pta_stats, sizeof(mtlk_priority_gpio_stats_t));
}

static void _wave_core_cca_stats_to_cca_stats_user (wave_cca_stats_t *cca, wave_cca_stats_user_t *user)
{
  BOOL flag = FALSE;
  int64 busy, diff;

  user->overflowOccurred = 0;
  ILOG1_HHHH("Internal: TimeOnChan:%llu Tx:%llu Rx:%llu Intf:%llu", cca->ccaActive, cca->ccaTx, cca->ccaRx, cca->ccaIntf);

#define _CALC_CHECK_SAVE_(item) \
    /* microseconds to milliseconds */ \
    cca->item += 500; /* for round up */ \
    do_div(cca->item, 1000ul); \
    /* save to the user context */ \
    user->item = (uint32)cca->item; \
    /* check for overflow: u64 vs u32 comparison */ \
    if (cca->item >user->item) { \
      ILOG1_V(#item " overflow"); \
      flag = TRUE; \
    }

    _CALC_CHECK_SAVE_(ccaActive);
    _CALC_CHECK_SAVE_(ccaTx);
    _CALC_CHECK_SAVE_(ccaRx);
    _CALC_CHECK_SAVE_(ccaIntf);
#undef  _CALC_CHECK_SAVE_

  if (flag)
    goto overflow;

  /* A special handling for Idle:
   * Idle = Total - (Tx + Rx + Interference)
   * A minimum difference is allowed as a result of rounding up
   */
  busy = cca->ccaTx + cca->ccaRx + cca->ccaIntf;
  diff = cca->ccaActive - busy;
  cca->ccaIdle = diff;
  user->ccaIdle = (uint32)cca->ccaIdle;
  ILOG1_DDDDD("ToUser: TimeOnChan:%u Tx:%u Rx:%u Intf:%u Idle:%u", user->ccaActive, user->ccaTx, user->ccaRx, user->ccaIntf, user->ccaIdle);
  if (diff < 0) { /* Active < Busy */
    if (diff <= -3) { /* max allowed diff */
      user->ccaActive -= diff; /* increase Active to remove difference */
      user->ccaIdle = 0;
    }
  }
  if (cca->ccaIdle > user->ccaIdle) {
      ILOG1_V("ccaIdle overflow");
      flag = TRUE;
  }

  if (!flag) {
    return;
  }

overflow:
  /* Reset all counters and set flag */
  memset(user, 0, sizeof(*user));
  user->overflowOccurred = 1;
}

void __MTLK_IFUNC
wave_core_cca_stats_save (mtlk_core_t *core, int channel, wave_cca_stats_t *cca_stats, wave_cca_stats_user_t *cca_stats_user, BOOL on_chan)
{
  wave_radio_t  *radio = wave_vap_radio_get(core->vap_handle);

  wave_hw_cca_stats_get(radio, channel, cca_stats);

  if (on_chan)
    wave_radio_cca_stat_update(radio, cca_stats, channel);

  _wave_core_cca_stats_to_cca_stats_user(cca_stats, cca_stats_user);
}

int __MTLK_IFUNC
 wave_core_cca_stats_get (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *master_core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_hw_t *hw = NULL;
  wave_cca_stats_t cca_stats = { 0 };
  wave_cca_stats_user_t cca_stats_user = { 0 };
  int channel;

  MTLK_ASSERT(NULL != master_core);

  hw = mtlk_vap_get_hw(master_core->vap_handle);
  channel = WAVE_RADIO_PDB_GET_INT(wave_vap_radio_get(master_core->vap_handle), PARAM_DB_RADIO_CHANNEL_CUR);

  ILOG1_D("channel %d", channel);
  wave_core_cca_stats_save(master_core, channel, &cca_stats, &cca_stats_user, TRUE);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &cca_stats_user, sizeof(cca_stats_user));
}

int __MTLK_IFUNC
wave_core_radio_usage_stats_get (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *master_core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_hw_t *hw = NULL;
  wave_radio_usage_stats_t radio_stats = { 0 };
  int channel, radio_id;

  MTLK_ASSERT(NULL != master_core);

  hw = mtlk_vap_get_hw(master_core->vap_handle);
  channel = WAVE_RADIO_PDB_GET_INT(wave_vap_radio_get(master_core->vap_handle), PARAM_DB_RADIO_CHANNEL_CUR);
  radio_id = wave_vap_radio_id_get(master_core->vap_handle);

  ILOG1_D("channel %d", channel);
  wave_hw_radio_usage_stats_get(hw, radio_id, channel, &radio_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &radio_stats, sizeof(radio_stats));
}

static void
_wave_core_reset_sta_reset(mtlk_hw_t *hw, sta_entry *sta)
{
  sta->sta_stats64_cntrs_snapshot = sta->sta_stats64_cntrs;
  sta->sta_stats_cntrs_snapshot = sta->sta_stats_cntrs;
  return;
}

static void
_wave_core_reset_radio_statistics(mtlk_hw_t *hw, wave_radio_t *radio)
{
  wave_hw_reset_recovery_stats(hw);
  mtlk_radio_wss_reset_stat(radio);
  wave_hw_reset_txm_stats(hw);
  wave_radio_reset_ofdma_stats(radio);
}

static void
_wave_core_reset_all_vap_sta_reset(mtlk_hw_t *hw, mtlk_df_user_t *df_user)
{
    mtlk_vap_manager_t *vap_mgr;
    mtlk_vap_handle_t vap_handle;
    mtlk_df_t *master_df = mtlk_df_user_get_master_df(df_user);
    mtlk_df_user_t *df_master_user;
    unsigned vaps_count, vap_id;
    mtlk_stadb_iterator_t iter;
    const sta_entry      *sta_loop = NULL;

    MTLK_CHECK_DF_NORES(master_df);
    df_master_user = mtlk_df_get_user(master_df);
    vap_mgr = mtlk_df_get_vap_manager(master_df);
    vaps_count = mtlk_vap_manager_get_max_vaps_count(vap_mgr);

    for (vap_id = 0; vap_id < vaps_count; vap_id++)
    {
        if (mtlk_vap_manager_get_vap_handle(vap_mgr, vap_id, &vap_handle) == MTLK_ERR_OK) {
          mtlk_core_t *core = mtlk_vap_get_core(vap_handle);
          memset(&core->pstats, 0, sizeof(core->pstats));
          sta_loop = mtlk_stadb_iterate_first(&core->slow_ctx->stadb, &iter);
          if (sta_loop) {
            do {
              _wave_core_reset_sta_reset(hw,(sta_entry *)sta_loop);
              sta_loop = mtlk_stadb_iterate_next(&iter);
            } while(sta_loop);
            mtlk_stadb_iterate_done(&iter);
          }
        }
    }

    return;
}

int __MTLK_IFUNC
wave_core_reset_statistics(mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_hw_t *hw = NULL;
  wave_radio_t *radio = NULL;
  int res = MTLK_ERR_OK;
  struct intel_vendor_reset_statistics *reset_stats = NULL;
  uint32 reset_stats_size;
  mtlk_df_t *df = NULL;
  MTLK_ASSERT(core != NULL);
  hw = mtlk_vap_get_hw(core->vap_handle);
  radio = wave_vap_radio_get(core->vap_handle);
  df = mtlk_vap_get_df(core->vap_handle);
  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  reset_stats = mtlk_clpb_enum_get_next(clpb, &reset_stats_size);
  MTLK_CLPB_TRY(reset_stats, reset_stats_size)
    switch (reset_stats->category) {
      case (RESET_FULL):
        wave_hw_stats_lock(hw, TRUE);
        wave_hw_capture_stats_snapshot_for_reset(hw);
        wave_core_reset_vap_stats(core);
        _wave_core_reset_all_vap_sta_reset(hw, mtlk_df_get_user(df));
        _wave_core_reset_radio_statistics(hw, radio);
        wave_hw_stats_lock(hw, FALSE);
        wave_hw_reset_chan_statistics(hw);
        break;
      case (RESET_RADIO):
        wave_radio_reset_chan_statistics(hw, 0, wave_radio_id_get(radio), TRUE);
        break;
      default:
        ILOG1_D("Unsupported statistics reset category %d", reset_stats->category);
        break;
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res(clpb, res);
  MTLK_CLPB_END
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_peer_mixed_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_error_t res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  wave_peer_mixed_stats_t peer_mixed_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      memset(&peer_mixed_stats , 0, sizeof(peer_mixed_stats));
      wave_sta_get_mixed_stats(sta, &peer_mixed_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_mixed_stats, sizeof(peer_mixed_stats));
  MTLK_CLPB_END;
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_la_su_mu_ru_ofdma_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  sta_entry       *sta = NULL;
  uint32          addr_size, ru_id;
  wave_vendor_su_mu_ru_stats_t       *vendor_stats = NULL;
  /* UL DL OFDMA PPDU sta stats */
  wave_wssa_ofdma_pdu_sta_stats_t *ul_dl_ofdma_stats = NULL;
  wave_mhi_la_ofdma_sta_pdu_stats_t  *sta_ul_dl_stats = NULL;
  mtlk_error_t res    = MTLK_ERR_OK;
  mtlk_core_t *nic    = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb   = *(mtlk_clpb_t **) data;
  wave_radio_t *radio = wave_vap_radio_get(nic->vap_handle);
  /*UL DL OFDMA ppdus radio stats */
  wave_wssa_la_ofdma_radio_pdu_stats_t  *ul_dl_radio_stats = NULL;
  wave_radio_pdus_ofdma_stats64_t  *radio_stats = NULL;
  void *payload       = NULL;
  uint32 payload_size = 0, chip_id;
  uint8_t no_of_bw = 0, no_of_ru_sizes = 0;
  mtlk_hw_t *hw;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  vendor_stats = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(vendor_stats, addr_size)
    if ((vendor_stats->category != UL_OFDMA_CATEGORY_RADIO_STATS) &&
        (vendor_stats->category != DL_OFDMA_CATEGORY_RADIO_STATS)) {
      /* find station in stadb */
      sta = mtlk_stadb_find_sta(&nic->slow_ctx->stadb, vendor_stats->sta_addr.au8Addr);
      if ((NULL == sta)) {
        ELOG_DY("CID-%04x: station %Y not found", mtlk_vap_get_oid(nic->vap_handle), vendor_stats->sta_addr.au8Addr);
        MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
      }
    }

    hw = mtlk_vap_get_hw(nic->vap_handle);
    chip_id = hw_mmb_get_chip_id(hw);

    if (_chipid_is_gen7(chip_id)) {
      no_of_bw = BW_MAX_G7;
      no_of_ru_sizes = WAVE700_NUM_OF_RU_SIZES;
    } else {
      no_of_bw = NUM_OF_BW;
      no_of_ru_sizes = WAVE600_NUM_OF_RU_SIZES;
    }

    if ((vendor_stats->category == UL_OFDMA_CATEGORY_RADIO_STATS) || /* UL OFDMA ppdus radio stats */
               (vendor_stats->category == DL_OFDMA_CATEGORY_RADIO_STATS)) { /* DL OFDMA ppdus radio stats */
      ul_dl_radio_stats = mtlk_osal_mem_alloc(sizeof(wave_wssa_la_ofdma_radio_pdu_stats_t), MTLK_MEM_TAG_EXTENSION);
      if (NULL == ul_dl_radio_stats) {
        ELOG_V("Can't allocate memory for wave_mhi_la_ofdma_radio_ppdu_stats_t struct");
        MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
      }

      memset(ul_dl_radio_stats, 0, sizeof(wave_wssa_la_ofdma_radio_pdu_stats_t));
      ul_dl_radio_stats->no_of_bw = no_of_bw;
      ul_dl_radio_stats->no_of_ru_sizes = no_of_ru_sizes;

      if (vendor_stats->category == UL_OFDMA_CATEGORY_RADIO_STATS)
        wave_hw_get_radio_ul_ofdma_stats_64 (radio, &radio_stats);
      else if (vendor_stats->category == DL_OFDMA_CATEGORY_RADIO_STATS)
        wave_hw_get_radio_dl_ofdma_stats_64 (radio, &radio_stats);

      ul_dl_radio_stats->radio_ppdu.total_ppdus        = radio_stats->ppdu_stats64.total_ppdus_count;
      ul_dl_radio_stats->su_ppdus                      = radio_stats->ppdu_stats64.su_ppdus_count;
      ul_dl_radio_stats->radio_ppdu.mu_ofdma_ppdus     = radio_stats->ppdu_stats64.mu_ofdma_ppdus_count;
      ul_dl_radio_stats->radio_ppdu.mu_mimo_ppdus      = radio_stats->ppdu_stats64.mu_mimo_ppdus_count;
      ul_dl_radio_stats->radio_ppdu.mu_ppdus           = ul_dl_radio_stats->radio_ppdu.mu_ofdma_ppdus +
                                                         ul_dl_radio_stats->radio_ppdu.mu_mimo_ppdus;
      ul_dl_radio_stats->radio_ppdu.he_ppdus           = ul_dl_radio_stats->su_ppdus +
                                                         ul_dl_radio_stats->radio_ppdu.mu_ppdus;
      ul_dl_radio_stats->radio_ppdu.total_mu_ru_ppdus  = radio_stats->ppdu_stats64.total_mu_ru_ppdus;

      /* MPDU related stats are present only for the DL stats */
      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
        ul_dl_radio_stats->radio_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_count =
               radio_stats->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count +
               radio_stats->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count;

        ul_dl_radio_stats->radio_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_err_count =
              radio_stats->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count;
      }

      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++)
        ul_dl_radio_stats->radio_ppdu.ru_ppdu_stats[ru_id].mu_ru_ppdus         = radio_stats->ppdu_stats64.mu_ru_ppdus[ru_id];

      MTLK_STATIC_ASSERT(sizeof(ul_dl_radio_stats->bw_ru_in_ppdus) == sizeof(radio_stats->ppdu_stats64.bw_ru_in_ppdus));
      wave_memcpy(ul_dl_radio_stats->bw_ru_in_ppdus, sizeof(ul_dl_radio_stats->bw_ru_in_ppdus),
                  radio_stats->ppdu_stats64.bw_ru_in_ppdus, sizeof(radio_stats->ppdu_stats64.bw_ru_in_ppdus));
      payload = ul_dl_radio_stats;
      payload_size = sizeof(*ul_dl_radio_stats);

    } else {
      if ((vendor_stats->category == UL_OFDMA_CATEGORY_STA_STATS) || /* UL OFDMA STA ppdus stats */
                 (vendor_stats->category == DL_OFDMA_CATEGORY_STA_STATS)) { /* DL OFDMA STA ppdus stats */
        ul_dl_ofdma_stats = mtlk_osal_mem_alloc(sizeof(wave_wssa_ofdma_pdu_sta_stats_t), MTLK_MEM_TAG_EXTENSION);
        if (NULL == ul_dl_ofdma_stats) {
          ELOG_V("Can't allocate memory for wave_wssa_ofdma_pdu_sta_stats_t struct");
          MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
        }

        memset(ul_dl_ofdma_stats, 0, sizeof(wave_wssa_ofdma_pdu_sta_stats_t));
        ul_dl_ofdma_stats->sta_sid = mtlk_sta_get_sid(sta);
        ul_dl_ofdma_stats->addr    = *(mtlk_sta_get_addr(sta));
        ul_dl_ofdma_stats->no_of_bw = no_of_bw;
        ul_dl_ofdma_stats->no_of_ru_sizes = no_of_ru_sizes;

        if (vendor_stats->category == UL_OFDMA_CATEGORY_STA_STATS)
          sta_ul_dl_stats = wave_sta_get_ul_ofdma_stats(sta);
        else if (vendor_stats->category == DL_OFDMA_CATEGORY_STA_STATS)
          sta_ul_dl_stats = wave_sta_get_dl_ofdma_stats(sta);

        ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.total_ppdus    = sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.total_ppdus;
        MTLK_STATIC_ASSERT(sizeof(ul_dl_ofdma_stats->sta_ppdu.su_ppdus) == sizeof(sta_ul_dl_stats->sta_ppdu.su_ppdus));
        wave_memcpy(ul_dl_ofdma_stats->sta_ppdu.su_ppdus, sizeof(ul_dl_ofdma_stats->sta_ppdu.su_ppdus),
                    sta_ul_dl_stats->sta_ppdu.su_ppdus, sizeof(sta_ul_dl_stats->sta_ppdu.su_ppdus));
        ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus = sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus;
        ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus  = sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus;
        ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus       = sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus;
        ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.he_ppdus       = sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.he_ppdus;

        for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
          ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.total_mu_ru_ppdus +=
                                                                   sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.ru_ppdu_stats[ru_id].mu_ru_ppdus;
          ul_dl_ofdma_stats->sta_ppdu.sta_ppdus_stats.ru_ppdu_stats[ru_id].mu_ru_ppdus =
                                                                   sta_ul_dl_stats->sta_ppdu.sta_ppdus_stats.ru_ppdu_stats[ru_id].mu_ru_ppdus;
          /* DL MPDU OFDMA stats update */
          ul_dl_ofdma_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_count =
                                                                   sta_ul_dl_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_count +
                                                                   sta_ul_dl_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_err_count;
          ul_dl_ofdma_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_err_count =
                                                                   sta_ul_dl_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_err_count;
        }

        payload = ul_dl_ofdma_stats;
        payload_size = sizeof(*ul_dl_ofdma_stats);
      }
    }

  MTLK_CLPB_FINALLY(res)
    if (sta) mtlk_sta_decref(sta); /* De-reference of find */
    res = mtlk_clpb_push_res_data(clpb, res, payload, payload_size);
    if (ul_dl_radio_stats != NULL)
      mtlk_osal_mem_free(ul_dl_radio_stats);
    if (ul_dl_ofdma_stats != NULL)
      mtlk_osal_mem_free(ul_dl_ofdma_stats);
    return res;
  MTLK_CLPB_END;
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_link_adapt_mimo_statistics(mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_error_t                 res = MTLK_ERR_OK;
  wave_radio_t                 *radio = NULL;
  mtlk_df_t                    *df_user = NULL, *master_df = NULL;
  uint16                       vaps_count, vap_id;
  mtlk_vap_manager_t           *vap_mgr;
  mtlk_vap_handle_t            vap_handle;
  wave_vendor_su_mu_ru_stats_t *vendor_stats = NULL;
  sta_entry      *sta = NULL;
  uint32 addr_size;
  uint8 nss, mcs, no_of_mcs = 0, no_of_nss = 0;
  wave_wssa_mimo_sta_stats_t *wssa_mimo_stats;
  wave_wssa_mimo_radio_stats_t *radio_mimo_stats = NULL;
  wave_radio_mimo_stats64_t *radio_stats = NULL;
  mtlk_mhi_stats64_sta_cntr_t *sta_stats64;
  mtlk_mhi_stats64_sta_cntr_t *sta_stats64_snap;
  mtlk_hw_t *hw;
  void *payload       = NULL;
  uint32 payload_size = 0, chip_id;
  uint64 total_su_count = 0, total_mu_count = 0;

  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  hw = mtlk_vap_get_hw(core->vap_handle);
  chip_id = hw_mmb_get_chip_id(hw);
  df_user = mtlk_vap_get_df(core->vap_handle);
  MTLK_CHECK_DF_USER(df_user);
  radio = wave_vap_radio_get(core->vap_handle);
  MTLK_ASSERT(radio != NULL);
  master_df = mtlk_df_user_get_master_df(mtlk_df_get_user(df_user));
  MTLK_RET_CHECK_DF(master_df);
  vap_mgr = mtlk_df_get_vap_manager(master_df);
  vaps_count = mtlk_vap_manager_get_max_vaps_count(vap_mgr);
  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  vendor_stats = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(vendor_stats, addr_size)
    if (_chipid_is_gen7(chip_id)) {
      no_of_nss = REDUCED_SPATIAL_STREAMS_SIZE_GEN7;
      no_of_mcs = REDUCED_MCS_SIZE_GEN7;
    } else {
      no_of_nss = REDUCED_SPATIAL_STREAMS_SIZE;
      no_of_mcs = REDUCED_MCS_SIZE;
    }

    if ((vendor_stats->category == DL_OFDMA_CATEGORY_RADIO_STATS)) {
      radio_mimo_stats = mtlk_osal_mem_alloc(sizeof(wave_wssa_mimo_radio_stats_t), MTLK_MEM_TAG_EXTENSION);
      if (NULL == radio_mimo_stats) {
        ELOG_V("Can't allocate memory for wave_wssa_mimo_radio_stats_t struct");
        MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
      }
      memset(radio_mimo_stats, 0, sizeof(wave_wssa_mimo_radio_stats_t));

      if (vendor_stats->category == DL_OFDMA_CATEGORY_RADIO_STATS)
        wave_get_radio_dl_mimo_stats_64 (radio, &radio_stats);

      radio_mimo_stats->no_of_nss = no_of_nss;
      radio_mimo_stats->no_of_mcs = no_of_mcs;

      /* Calculaton related to SU MU NSS */
      for (nss = 0; nss < no_of_nss; nss++) {
        radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.nss_ppdus[nss] =
                               radio_stats->su_mimo.su_rate.nss_ppdus[nss];
        total_su_count += radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.nss_ppdus[nss];

        radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss] =
                               radio_stats->mu_mimo.mu_rate.nss_ppdus[nss];
        total_mu_count += radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss];
      }

      /* Percentage calculation for SU MU NSS */
      for (nss = 0; nss < no_of_nss; nss++) {
        radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.nss_ppdus_percent[nss] =
            WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.nss_ppdus[nss],
                                total_su_count);

        radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.nss_ppdus_percent[nss] =
            WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss],
                                total_mu_count);
      }
      total_su_count = 0;
      total_mu_count = 0;

      /* Calculaton related to SU MU MCS */
      for (mcs = 0; mcs < no_of_mcs; mcs++) {
        radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs] =
                               radio_stats->su_mimo.su_rate.mcs_ppdus[mcs];
        total_su_count += radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs];

        radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs] =
                               radio_stats->mu_mimo.mu_rate.mcs_ppdus[mcs];
        total_mu_count += radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs];

        radio_mimo_stats->mimo_stats.mu_mimo.attemptedsounding_per_mcs[mcs] =
                               radio_stats->mu_mimo.attemptedsounding_per_mcs[mcs];
        radio_mimo_stats->mimo_stats.mu_mimo.sum_attemptedsounding +=
                               radio_mimo_stats->mimo_stats.mu_mimo.attemptedsounding_per_mcs[mcs];
      }

      /* Percentage calculation for SU MU MCS */
      for (mcs = 0; mcs < no_of_mcs; mcs ++) {
        radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.mcs_ppdus_percent[mcs] =
            WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs],
                                total_su_count);

        radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.mcs_ppdus_percent[mcs] =
            WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs],
                                total_mu_count);

        radio_mimo_stats->mimo_stats.mu_mimo.attemptedsounding_per_mcs_percent[mcs] =
            WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.mu_mimo.attemptedsounding_per_mcs_percent[mcs],
                                radio_mimo_stats->mimo_stats.mu_mimo.sum_attemptedsounding);
      }
      radio_mimo_stats->mimo_stats.mu_mimo.succeeded_sounding =
                             radio_stats->mu_mimo.succeeded_sounding;
      total_mu_count = radio_mimo_stats->mimo_stats.mu_mimo.succeeded_sounding +
                               radio_mimo_stats->mimo_stats.mu_mimo.sum_attemptedsounding;

      radio_mimo_stats->mimo_stats.mu_mimo.succeeded_sounding_percent =
          WAVE_GET_PERCENTAGE(radio_mimo_stats->mimo_stats.mu_mimo.succeeded_sounding,
                              total_mu_count);

      payload = radio_mimo_stats;
      payload_size = sizeof(*radio_mimo_stats);
    } else if (vendor_stats->category == DL_OFDMA_CATEGORY_STA_STATS) {
      for (vap_id = 0; vap_id < vaps_count; vap_id++) {
        if (mtlk_vap_manager_get_vap_handle(vap_mgr, vap_id, &vap_handle) == MTLK_ERR_OK) {
          mtlk_core_t *cur_nic = mtlk_vap_get_core(vap_handle);
          sta = mtlk_stadb_find_sta(&cur_nic->slow_ctx->stadb, vendor_stats->sta_addr.au8Addr);
          if (sta)
            break;
        }
      }
      if (NULL == sta) {
        ELOG_Y("station %Y not found", vendor_stats->sta_addr.au8Addr);
        MTLK_CLPB_EXIT(MTLK_ERR_PARAMS);
      }
      wssa_mimo_stats = mtlk_osal_mem_alloc(
                        sizeof(wave_wssa_mimo_sta_stats_t),
                        MTLK_MEM_TAG_EXTENSION);
      if (NULL == wssa_mimo_stats) {
        ELOG_V("Can't allocate memory for wave_wssa_mimo_sta_stats_t struct");
        MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
      }
      memset(wssa_mimo_stats, 0, sizeof(wave_wssa_mimo_sta_stats_t));
      sta_stats64      = mtlk_sta_get_mhi_stat64_array(sta);
      sta_stats64_snap = mtlk_sta_get_mhi_stat64_snapshot_array(sta);
      wssa_mimo_stats->addr    = *(mtlk_sta_get_addr(sta));
      wssa_mimo_stats->sta_sid = mtlk_sta_get_sid(sta);
      wssa_mimo_stats->no_of_nss = no_of_nss;
      wssa_mimo_stats->no_of_mcs = no_of_mcs;
      /* Calculaton related to SU MU NSS */
      for (nss = 0; nss < no_of_nss; nss++) {
        wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.nss_ppdus[nss] =
                         sta_stats64->dl_mimo.su_mimo_stats64.su_rate_stats64.nss_ppdus[nss] -
                         sta_stats64_snap->dl_mimo.su_mimo_stats64.su_rate_stats64.nss_ppdus[nss];
        total_su_count += wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.nss_ppdus[nss];

        wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss] =
                         sta_stats64->dl_mimo.mu_mimo_stats64.mu_rate_stats64.nss_ppdus[nss] -
                         sta_stats64_snap->dl_mimo.mu_mimo_stats64.mu_rate_stats64.nss_ppdus[nss];
        total_mu_count += wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss];
      }
      /* Percentage calculation for SU MU NSS */
      for (nss = 0; nss < no_of_nss; nss++) {
        wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.nss_ppdus_percent[nss] =
                         WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.nss_ppdus[nss],
                         total_su_count);

        wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.nss_ppdus_percent[nss] =
                         WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.nss_ppdus[nss],
                         total_mu_count);
      }
      /* Calculaton related to SU MU MCS */
      for (mcs = 0; mcs < no_of_mcs; mcs ++) {
        wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs] =
                         sta_stats64->dl_mimo.su_mimo_stats64.su_rate_stats64.mcs_ppdus[mcs] -
                         sta_stats64_snap->dl_mimo.su_mimo_stats64.su_rate_stats64.mcs_ppdus[mcs];
        total_su_count += wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs];

        wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs] =
                         sta_stats64->dl_mimo.mu_mimo_stats64.mu_rate_stats64.mcs_ppdus[mcs] -
                         sta_stats64_snap->dl_mimo.mu_mimo_stats64.mu_rate_stats64.mcs_ppdus[mcs];
        total_mu_count += wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs];

        wssa_mimo_stats->sta_stats.mu_mimo.attemptedsounding_per_mcs[mcs] =
                         sta_stats64->dl_mimo.mu_mimo_stats64.attemptedsounding_per_mcs[mcs] -
                         sta_stats64_snap->dl_mimo.mu_mimo_stats64.attemptedsounding_per_mcs[mcs];
        wssa_mimo_stats->sta_stats.mu_mimo.sum_attemptedsounding +=
                         wssa_mimo_stats->sta_stats.mu_mimo.attemptedsounding_per_mcs[mcs];
      }
      /* Percentage calculation for SU MU NSS */
      for (mcs = 0; mcs < no_of_mcs; mcs ++) {
        wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.mcs_ppdus_percent[mcs] =
                         WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.su_mimo.su_mimo_rate.mcs_ppdus[mcs],
                         total_su_count);

        wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.mcs_ppdus_percent[mcs] =
                         WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.mu_mimo.mu_mimo_rate.mcs_ppdus[mcs],
                         total_mu_count);

        wssa_mimo_stats->sta_stats.mu_mimo.attemptedsounding_per_mcs_percent[mcs] =
                         WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.mu_mimo.attemptedsounding_per_mcs_percent[mcs],
                         wssa_mimo_stats->sta_stats.mu_mimo.sum_attemptedsounding);
      }

      wssa_mimo_stats->sta_stats.mu_mimo.succeeded_sounding =
                       sta_stats64->dl_mimo.mu_mimo_stats64.succeeded_sounding -
                       sta_stats64_snap->dl_mimo.mu_mimo_stats64.succeeded_sounding;
      total_mu_count = wssa_mimo_stats->sta_stats.mu_mimo.succeeded_sounding +
                       wssa_mimo_stats->sta_stats.mu_mimo.sum_attemptedsounding;

      wssa_mimo_stats->sta_stats.mu_mimo.succeeded_sounding_percent =
                       WAVE_GET_PERCENTAGE(wssa_mimo_stats->sta_stats.mu_mimo.succeeded_sounding,
                       total_mu_count);
      payload = wssa_mimo_stats;
      payload_size = sizeof(*wssa_mimo_stats);
    } else {
      ELOG_V("Invalid parameters");
      MTLK_CLPB_EXIT(MTLK_ERR_PARAMS);
    }
  MTLK_CLPB_FINALLY(res)
    if (sta) mtlk_sta_decref(sta); /* De-reference of find */
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, payload, payload_size);
    if (MTLK_ERR_OK != res && payload != NULL) {
      mtlk_osal_mem_free(payload);
    }
    return res;
  MTLK_CLPB_END;
}

static uint
wave_get_csi_bw_hw_map (enum WAVE_CSI_BW_MODE bw_mode)
{
  switch(bw_mode) {
    case CSI_BW_20:
      return 20;
    case CSI_BW_40:
      return 40;
    case CSI_BW_80:
      return 80;
    case CSI_BW_160:
      return 160;
    case CSI_BW_320:
      return 320;
    default:
      return 0;
  }
}

void __MTLK_IFUNC
wave_sta_get_csi_stats (mtlk_core_t *core, sta_entry* sta, wifi_csi_driver_data_t *csi_stats)
{
  uint16 index;
  int16 freq_offset;

  MTLK_ASSERT(sta != NULL);
  MTLK_ASSERT(csi_stats != NULL);

  mtlk_osal_lock_acquire(&sta->lock);
  csi_stats->frame_info.bw_mode = sta->csi_stats.frame_info.bw_mode;
  csi_stats->frame_info.mcs = sta->csi_stats.frame_info.mcs;
  csi_stats->frame_info.Nr = sta->csi_stats.frame_info.Nr;
  csi_stats->frame_info.Nc = sta->csi_stats.frame_info.Nc;
  /* Ignore dummy values from src array */
  for (index = 0; index < MAX_NR_SIZE; index += 2) {
    csi_stats->frame_info.nr_rssi[index/2] = MAC_TO_HOST16(sta->csi_stats.frame_info.nr_rssi[index]);
  }

  csi_stats->frame_info.channel = mtlk_core_get_channel(core);
  csi_stats->frame_info.valid_mask = MAC_TO_HOST16(sta->csi_stats.frame_info.valid_mask);
  csi_stats->frame_info.phy_bw = (unsigned short) wave_get_csi_bw_hw_map(MAC_TO_HOST16(sta->csi_stats.frame_info.phy_bw));
  csi_stats->frame_info.cap_bw = (unsigned short) wave_get_csi_bw_hw_map(MAC_TO_HOST16(sta->csi_stats.frame_info.cap_bw));
  csi_stats->frame_info.num_sc = (uint32) MAC_TO_HOST16(sta->csi_stats.frame_info.num_sc);
  csi_stats->frame_info.decimation = sta->csi_stats.frame_info.decimation;

  freq_offset = MAC_TO_HOST16(sta->csi_stats.frame_info.frequency_offset);
  if (freq_offset >= (1 << 14))
    freq_offset -= (1 << 15);
  csi_stats->frame_info.frequency_offset = freq_offset;

  for (index = 0; index < ARRAY_SIZE(sta->csi_stats.csi_raw_data); index++) {
    csi_stats->csi_raw_data[index] = MAC_TO_HOST32(sta->csi_stats.csi_raw_data[index]);
  }
  mtlk_osal_lock_release(&sta->lock);
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_csi_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_error_t res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  wifi_csi_driver_data_t *csi_stats = NULL;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)
    csi_stats = mtlk_osal_mem_alloc(sizeof(wifi_csi_driver_data_t), MTLK_MEM_TAG_EXTENSION);
    if (NULL == csi_stats) {
      ELOG_V("Can't allocate memory for wifi_csi_data_t struct");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(csi_stats, 0, sizeof(wifi_csi_driver_data_t));

    /* find station in stadb */
    sta = mtlk_vap_find_sta(core->vap_handle, addr->au8Addr);
    if (NULL == sta) {
      ELOG_DY("CID-%04x: station %Y not found", mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      /* Check if CSI is enabled */
      if(!(sta->csi_enable || sta->csi_auto_enable)) {
        ELOG_DY("CID-%04x: CSI not enabled for Sta %Y", mtlk_vap_get_oid(core->vap_handle), addr);
        MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
      } else {
        wave_sta_get_csi_stats(core, sta, csi_stats);
        sta->csi_sta_cntrs.csiReqInfoCount++;
      }
    }
  MTLK_CLPB_FINALLY(res)
    if (sta) mtlk_sta_decref(sta);
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, csi_stats, sizeof(wifi_csi_driver_data_t));
    if (MTLK_ERR_OK != res && csi_stats != NULL) {
      mtlk_osal_mem_free(csi_stats);
    }
    return res;
  MTLK_CLPB_END;
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_csi_counters (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_error_t res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  wave_csi_sta_stats64_cntr_t *csi_counters = NULL;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)
    csi_counters = mtlk_osal_mem_alloc(sizeof(wave_csi_sta_stats64_cntr_t), MTLK_MEM_TAG_EXTENSION);
    if (NULL == csi_counters) {
      ELOG_V("Can't allocate memory for wave_csi_sta_stats64_cntr_t struct");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }
    memset(csi_counters, 0, sizeof(wave_csi_sta_stats64_cntr_t));

    /* find station in stadb */
    sta = mtlk_vap_find_sta(core->vap_handle, addr->au8Addr);
    if (NULL == sta) {
      ELOG_DY("CID-%04x: station %Y not found", mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
	  /* Check if CSI is enabled */
      if(!(sta->csi_enable || sta->csi_auto_enable)) {
        ELOG_DY("CID-%04x: CSI not enabled for Sta %Y", mtlk_vap_get_oid(core->vap_handle), addr);
        MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
      } else {
        csi_counters->csiSendQosNullCount = sta->csi_sta_cntrs.csiSendQosNullCount;
        csi_counters->csiRecvFrameCount = sta->csi_sta_cntrs.csiRecvFrameCount;
        csi_counters->csiSendNlCsiData = sta->csi_sta_cntrs.csiSendNlCsiData;
        csi_counters->csiReqInfoCount = sta->csi_sta_cntrs.csiReqInfoCount;
      }
    }
  MTLK_CLPB_FINALLY(res)
    if (sta) mtlk_sta_decref(sta);
    res = mtlk_clpb_push_res_data_no_copy(clpb, res, csi_counters, sizeof(wave_csi_sta_stats64_cntr_t));
    if (MTLK_ERR_OK != res && csi_counters != NULL) {
      mtlk_osal_mem_free(csi_counters);
    }
    return res;
  MTLK_CLPB_END;
}

void __MTLK_IFUNC
mtlk_core_reset_stats_internal (mtlk_core_t *core)
{
  if (mtlk_vap_is_ap(core->vap_handle)) {
    mtlk_stadb_reset_cnts(&core->slow_ctx->stadb);
  }
  memset(&core->pstats, 0, sizeof(core->pstats));
}

#ifdef CONFIG_WAVE_DEBUG

mtlk_error_t __MTLK_IFUNC
mtlk_core_reset_stats (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t res = MTLK_ERR_OK;
  unsigned uint32_size;
  uint32 *reset_radar_cnt;
  mtlk_core_t *nic = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  reset_radar_cnt = mtlk_clpb_enum_get_next(clpb, &uint32_size);
  MTLK_CLPB_TRY(reset_radar_cnt, uint32_size)
  {
    if (*reset_radar_cnt) {
      mtlk_hw_reset_radar_cntr(mtlk_vap_get_hw(nic->vap_handle));
        MTLK_CLPB_EXIT(res);
    }

    if (mtlk_core_get_net_state(nic) != NET_STATE_HALTED) {
      ELOG_D("CID-%04x: Can not reset stats when core is active", mtlk_vap_get_oid(nic->vap_handle));
      res = MTLK_ERR_NOT_READY;
    } else {
      mtlk_core_reset_stats_internal(nic);
    }
  }
  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res(clpb, res);
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_host_if_qos_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerHostIfQos_t peer_host_if_qos;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_host_if_qos_stats(sta, &peer_host_if_qos);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_host_if_qos, sizeof(peerHostIfQos_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_host_if_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerHostIf_t peer_host_if;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_host_if_stats(sta, &peer_host_if);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_host_if, sizeof(peerHostIf_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_rx_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerRxStats_t peer_rx_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_rx_stats(sta, &peer_rx_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_rx_stats, sizeof(peerRxStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_ul_bsrc_tid_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerUlBsrcTidStats_t peer_ul_bsrc_tid;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_ul_bsrc_tid_stats(sta, &peer_ul_bsrc_tid);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_ul_bsrc_tid, sizeof(peerUlBsrcTidStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_baa_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerBaaStats_t peer_baa_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_baa_stats(sta, &peer_baa_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_baa_stats, sizeof(peerBaaStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_link_adaption_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  linkAdaptionStats_t link_adaption_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_link_adaption_stats(sta, &link_adaption_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &link_adaption_stats, sizeof(linkAdaptionStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_plan_manager_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  planManagerStats_t plan_manager_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_plan_manager_stats(sta, &plan_manager_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &plan_manager_stats, sizeof(planManagerStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_twt_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  twtStats_t twt_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_twt_stats(sta, &twt_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &twt_stats, sizeof(twtStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_per_client_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  perClientStats_t per_client_stats;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_per_client_stats(sta, &per_client_stats);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &per_client_stats, sizeof(perClientStats_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_phy_rx_status (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerPhyRxStatus_t phy_rx_status;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_phy_rx_status(sta, &phy_rx_status);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &phy_rx_status, sizeof(peerPhyRxStatus_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_peer_info_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  int res = MTLK_ERR_OK;
  IEEE_ADDR *addr;
  uint32 addr_size;
  sta_entry *sta = NULL;
  peerInfo_t peer_info;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  addr = mtlk_clpb_enum_get_next(clpb, &addr_size);
  MTLK_CLPB_TRY(addr, addr_size)

    /* find station in stadb */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, addr->au8Addr);
    if (NULL == sta) {
      WLOG_DY("CID-%04x: station %Y not found",
        mtlk_vap_get_oid(core->vap_handle), addr);
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    } else {
      mtlk_sta_get_peer_info(sta, &peer_info);
      mtlk_sta_decref(sta);
    }

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res_data(clpb, res, &peer_info, sizeof(peerInfo_t));
  MTLK_CLPB_END;
}

int __MTLK_IFUNC
wave_core_get_wlan_host_if_qos_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_wlan_host_if_qos_t wlan_host_if_qos;

  wave_core_get_wlan_host_if_qos_stats(core, &wlan_host_if_qos);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &wlan_host_if_qos, sizeof(mtlk_wssa_drv_wlan_host_if_qos_t));
}

int __MTLK_IFUNC
wave_core_get_wlan_host_if_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_wlan_host_if_t wlan_host_if;

  wave_core_get_wlan_host_if_stats(core, &wlan_host_if);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &wlan_host_if, sizeof(mtlk_wssa_drv_wlan_host_if_t));
}

int __MTLK_IFUNC
wave_core_get_wlan_rx_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_wlan_rx_stats_t wlan_rx_stats;

  wave_core_get_wlan_rx_stats(core, &wlan_rx_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &wlan_rx_stats, sizeof(mtlk_wssa_drv_wlan_rx_stats_t));
}

int __MTLK_IFUNC
wave_core_get_wlan_baa_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_wlan_baa_stats_t wlan_baa_stats;

  wave_core_get_wlan_baa_stats(core, &wlan_baa_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &wlan_baa_stats, sizeof(mtlk_wssa_drv_wlan_baa_stats_t));
}

int __MTLK_IFUNC
wave_core_get_radio_rx_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_radio_rx_stats_t radio_rx_stats;

  wave_core_get_radio_rx_stats(core, &radio_rx_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &radio_rx_stats, sizeof(mtlk_wssa_drv_radio_rx_stats_t));
}

int __MTLK_IFUNC
wave_core_get_radio_baa_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_radio_baa_stats_t radio_baa_stats;

  wave_core_get_radio_baa_stats(core, &radio_baa_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &radio_baa_stats, sizeof(mtlk_wssa_drv_radio_baa_stats_t));
}

int __MTLK_IFUNC
wave_core_get_tsman_init_tid_gl_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_tsman_init_tid_gl_t ts_init_tid_gl;

  wave_core_get_tsman_init_tid_gl_stats(core, &ts_init_tid_gl);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &ts_init_tid_gl, sizeof(mtlk_wssa_drv_tsman_init_tid_gl_t));
}

int __MTLK_IFUNC
wave_core_get_tsman_init_sta_gl_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_tsman_init_sta_gl_t ts_init_sta_gl;

  wave_core_get_tsman_init_sta_gl_stats(core, &ts_init_sta_gl);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &ts_init_sta_gl, sizeof(mtlk_wssa_drv_tsman_init_sta_gl_t));
}

int __MTLK_IFUNC
wave_core_get_tsman_rcpt_tid_gl_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_tsman_rcpt_tid_gl_t ts_rcpt_tid_gl;

  wave_core_get_tsman_rcpt_tid_gl_stats(core, &ts_rcpt_tid_gl);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &ts_rcpt_tid_gl, sizeof(mtlk_wssa_drv_tsman_rcpt_tid_gl_t));
}

int __MTLK_IFUNC
wave_core_get_tsman_rcpt_sta_gl_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_tsman_rcpt_sta_gl_t ts_rcpt_sta_gl;

  wave_core_get_tsman_rcpt_sta_gl_stats(core, &ts_rcpt_sta_gl);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &ts_rcpt_sta_gl, sizeof(mtlk_wssa_drv_tsman_rcpt_sta_gl_t));
}

int __MTLK_IFUNC
wave_core_get_radio_link_adapt_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_radio_link_adapt_stats_t link_adapt_stats;

  wave_core_get_radio_link_adapt_stats(core, &link_adapt_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &link_adapt_stats, sizeof(mtlk_wssa_drv_radio_link_adapt_stats_t));
}

int __MTLK_IFUNC
wave_core_get_multicast_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_multicast_stats_t mc_stats;

  wave_core_get_multicast_stats(core, &mc_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &mc_stats, sizeof(mtlk_wssa_drv_multicast_stats_t));
}

int __MTLK_IFUNC
wave_core_get_training_man_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_training_man_stats_t tr_man_stats;

  wave_core_get_training_man_stats(core, &tr_man_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &tr_man_stats, sizeof(mtlk_wssa_drv_training_man_stats_t));
}

int __MTLK_IFUNC
wave_core_get_grp_man_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_group_man_stats_t gr_man_stats;

  wave_core_get_grp_man_stats(core, &gr_man_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &gr_man_stats, sizeof(mtlk_wssa_drv_group_man_stats_t));
}

int __MTLK_IFUNC
wave_core_get_general_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_general_stats_t gen_stats;

  wave_core_get_general_stats(core, &gen_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &gen_stats, sizeof(mtlk_wssa_drv_general_stats_t));
}

static void
_wave_core_fill_drv_channel_stats (mtlk_core_t *core, wave_drv_channel_stats_t *stats, int ch_num)
{
  mtlk_hw_copy_drv_stats_per_channel(wave_vap_radio_get(core->vap_handle), stats, ch_num);
}

static void
_wave_core_fill_wssa_drv_chan_stats (mtlk_core_t *core, mtlk_wssa_drv_cur_channel_stats_t *wssa_chan_stats, int chan_num)
{
  wave_drv_channel_stats_t drv_chan_stats;
  wifi_channelStats_t     *wifi_chan_stats = &drv_chan_stats.wifi_chan_stats;

  memset(&drv_chan_stats, 0, sizeof(drv_chan_stats));
  _wave_core_fill_drv_channel_stats(core, &drv_chan_stats, chan_num);

  wssa_chan_stats->chUtilizationActive    = drv_chan_stats.chan_active_time;
  wssa_chan_stats->chUtilizationBusy      = wifi_chan_stats->ch_utilization_busy;
  wssa_chan_stats->chUtilizationBusyTx    = wifi_chan_stats->ch_utilization_busy_tx;
  wssa_chan_stats->chUtilizationBusyRx    = wifi_chan_stats->ch_utilization_busy_rx;
  wssa_chan_stats->chUtilizationBusySelf  = wifi_chan_stats->ch_utilization_busy_self;
  wssa_chan_stats->chUtilizationBusyExt   = wifi_chan_stats->ch_utilization_busy_ext;
  wssa_chan_stats->channelMax80211Rssi    = wifi_chan_stats->ch_max_80211_rssi;
  wssa_chan_stats->chUtilizationInterf    = drv_chan_stats.chan_util_interf;
  wssa_chan_stats->chUtilizationIdle      = drv_chan_stats.chan_idle_time;
  wssa_chan_stats->chUtilization          = wifi_chan_stats->ch_utilization;
  wssa_chan_stats->channelNum             = chan_num;
}

int __MTLK_IFUNC
wave_core_get_cur_channel_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_cur_channel_stats_t wssa_chan_stats;

  _wave_core_fill_wssa_drv_chan_stats(core, &wssa_chan_stats, mtlk_core_get_channel(core));

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &wssa_chan_stats, sizeof(mtlk_wssa_drv_cur_channel_stats_t));
}

int __MTLK_IFUNC
wave_core_get_radio_phy_rx_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_radio_phy_rx_stats_t phy_rx_stats;

  wave_core_get_radio_phy_rx_stats(core, &phy_rx_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &phy_rx_stats, sizeof(mtlk_wssa_drv_radio_phy_rx_stats_t));
}

int __MTLK_IFUNC
wave_core_get_dynamic_bw_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  //wave_radio_t  *radio = wave_vap_radio_get(core->vap_handle);
  mtlk_wssa_drv_dynamic_bw_stats_t dy_bw_stats;

  wave_core_get_dynamic_bw_stats(core, &dy_bw_stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &dy_bw_stats, sizeof(mtlk_wssa_drv_dynamic_bw_stats_t));
}

int __MTLK_IFUNC
wave_core_get_la_mu_vht_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  uint8 *mu;
  uint32 mu_size;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_radio_la_mu_vht_stats_t link_adapt_stats;
  int res= MTLK_ERR_OK;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  mu = mtlk_clpb_enum_get_next(clpb, &mu_size);
  MTLK_CLPB_TRY(mu, mu_size)
  wave_core_get_radio_la_mu_vht_stats(core, &link_adapt_stats, mu);
  MTLK_CLPB_FINALLY(res)
  return mtlk_clpb_push_res_data(clpb, res, &link_adapt_stats, sizeof(mtlk_wssa_drv_radio_la_mu_vht_stats_t));
  MTLK_CLPB_END;
}

#endif /* CONFIG_WAVE_DEBUG */

mtlk_error_t __MTLK_IFUNC
wave_core_get_la_mu_he_eht_stats (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  uint32 input_size;
  mtlk_error_t res = MTLK_ERR_OK;
  mtlk_wssa_drv_la_mu_he_eht_input_t *input;
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_la_mu_he_eht_stats_t *stats = NULL;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  input = mtlk_clpb_enum_get_next(clpb, &input_size);
  MTLK_CLPB_TRY(input, input_size)
    if ((input->group_id < 0) || (input->group_id >= MAX_NUM_OF_HE_GROUPS)) {
      ELOG_D("Invalid group ID %d provided", input->group_id);
      MTLK_CLPB_EXIT(MTLK_ERR_PARAMS);
    }

    if ((input->reset_stats < 0) || (input->reset_stats > LA_MU_HE_EHT_RESET_ALL)) {
      ELOG_D("Invalid reset stats value %d provided", input->reset_stats);
      MTLK_CLPB_EXIT(MTLK_ERR_PARAMS);
    }

    if (input->reset_stats == LA_MU_HE_EHT_NO_RESET) {
      stats = mtlk_osal_mem_alloc(sizeof(*stats), MTLK_MEM_TAG_CLPB);
      if (!stats)
        MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }

    res = wave_hw_get_la_mu_he_eht_stats(mtlk_vap_get_hw(core->vap_handle), stats, input);
  MTLK_CLPB_FINALLY(res)
    if ((MTLK_ERR_OK == res) && stats) {
      res = mtlk_clpb_push_res_data_no_copy(clpb, res, stats, sizeof(*stats));
      if (MTLK_ERR_OK != res)
        mtlk_osal_mem_free(stats);
      return res;
    } else {
      if (stats)
        mtlk_osal_mem_free(stats);
      return mtlk_clpb_push_res(clpb, res);
    }
  MTLK_CLPB_END;
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_la_mu_groups_statistics (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_wssa_drv_la_mu_groups_stats_t stats;

  wave_hw_get_la_mu_groups_stats(mtlk_vap_get_hw(core->vap_handle), &stats);

  return mtlk_clpb_push_res_data(clpb, MTLK_ERR_OK, &stats, sizeof(stats));
}

enum _wave_fixed_ltf_and_gi_e
{
  SHORT_GI_SHORT_LTF,   /* 0 - HT/VHT: 0.4us GI; HE: 0.8us GI, 1x LTF */
  MEDIUM_GI_SHORT_LTF,  /* 1 - HT/VHT: 0.8us GI; HE: 1.6us GI, 1x LTF */
  SHORT_GI_MED_LTF,     /* 2 - HT/VHT: invalid;  HE: 0.8us GI, 2x LTF */
  MEDIUM_GI_MEDIUM_LTF, /* 3 - HT/VHT: invalid;  HE: 1.6us GI, 2x LTF */
  SHORT_GI_LONG_LTF,    /* 4 - HT/VHT: invalid;  HE: 0.8us GI, 4x LTF */
  LONG_GI_LONG_LTF,     /* 5 - HT/VHT: invalid;  HE: 3.2us GI, 4x LTF */
};

static uint32 _wave_update_ht_and_vht_gi_if_fixed (UMI_FIXED_LTF_AND_GI_REQ *ltf_and_gi, uint32 curr_gi)
{
  if (ltf_and_gi->isAuto)
    return curr_gi;

  if (ltf_and_gi->ltfAndGi == SHORT_GI_SHORT_LTF)
    return 1; /* Short 0.4us GI supported */
  if (ltf_and_gi->ltfAndGi == MEDIUM_GI_SHORT_LTF)
    return 0; /* Short 0.4us GI not supported */

  return curr_gi;
}

void __MTLK_IFUNC
mtlk_core_find_max_sta_rate (struct nic *nic, sta_entry *sta, struct ieee80211_sta *mac80211_sta)
{
  uint32 max_nss = 0;
  uint32 max_mcs = 0;
  uint32 max_rate = 0;
  uint32 rate_1ss = 0; /* per one stream */
  uint32 ht_bw = IEEE80211_STA_RX_BW_20;
  uint32 ht_scp = 0;
  uint8  max_supp_mcs = 0;
  uint8  sgi_enable = 0;
  uint8  max_supp_bw = IEEE80211_STA_RX_BW_20;
  struct mtlk_chan_def *cd;
  mtlk_core_t *mcore;
  uint32 width;
  size_t i;
  UMI_FIXED_LTF_AND_GI_REQ gi_and_ltf;

  MTLK_ASSERT(NULL != sta);
  MTLK_ASSERT(NULL != mac80211_sta);
  MTLK_ASSERT(NULL != nic);

  mcore = mtlk_core_get_master(nic->vap_handle);
  MTLK_ASSERT(NULL != mcore);

  memset(&gi_and_ltf, 0, sizeof(gi_and_ltf));
  mtlk_core_cfg_get_fixed_ltf_and_gi(mcore, &gi_and_ltf);
  cd = __wave_core_chandef_get(mcore);
  width = cd->width;
  ILOG1_D("current radio BW:%d", width);

  /* Get max non-HT rate */
  if (!MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11n)) {
    unsigned  supp_rate = 0;
    for (i = 0; i < sta->info.supported_rates_len; i++) {
      supp_rate = MAX(supp_rate, sta->info.rates[i]);
    }
    max_nss = 1;
    max_rate = MTLK_SUPP_RATE_TO_BITRATE(supp_rate);
    rate_1ss = max_rate;
    ILOG1_DD("STA non-HT supported rate:%u max_phy_rate:%u", supp_rate, max_rate);
  }

  /* Get max HT rate */
  if (MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11n)) {
    int i,y;
    uint32 ht_nss = 0;
    uint32 ht_mcs = 0;
    uint16 ht_cap;
    uint32 ht_max_phy_rate = 0;
    uint32 ht_1ss_phy_rate = 0;

    /* TBD: BITRATE_MCS32 for BPSK */

    for (i = 3; i >= 0; i--) { /* we support max 4 nss */
      uint8 rx_mask = mac80211_sta->deflink.ht_cap.mcs.rx_mask[i];
      if (rx_mask) {
        ht_nss = i + 1;
        for (y = 7; y >= 0; y--) {
          if (MTLK_BIT_GET(rx_mask, y) != 0) {
            ht_mcs = y + i*8;
            break;
          }
        }
        break;
      }
    }

    ht_cap = mac80211_sta->deflink.ht_cap.cap;
    if ((ht_cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40) &&
          !(ht_cap & IEEE80211_HT_CAP_40MHZ_INTOLERANT)) {
      ht_bw = IEEE80211_STA_RX_BW_40;
      if (ht_cap & IEEE80211_HT_CAP_SGI_40)
        ht_scp = 1;
    } else {
      ht_bw = 0;
      if (ht_cap & IEEE80211_HT_CAP_SGI_20)
        ht_scp = 1;
    }

    ht_scp = _wave_update_ht_and_vht_gi_if_fixed(&gi_and_ltf, ht_scp);
    ht_bw = min(width, ht_bw);
    ht_1ss_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_N, ht_bw, ht_scp,
        (ht_mcs == BITRATE_MCS32) ? BITRATE_MCS32 : (ht_mcs & 7), 1);
    ht_max_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_N, ht_bw, ht_scp, ht_mcs, ht_nss);
    if (max_rate < ht_max_phy_rate) {
      max_nss  = ht_nss;
      max_rate = ht_max_phy_rate;
      rate_1ss = ht_1ss_phy_rate;
    }
    ILOG1_DDDDDD("STA HT IE max_mcs:%u nss:%u bw:%u scp:%u phy_rate (1ss/max):(%u/%u)",
                 ht_mcs, ht_nss, ht_bw, ht_scp, ht_1ss_phy_rate, ht_max_phy_rate);
    max_supp_mcs = ht_mcs;
    max_supp_bw = ht_bw;
    sgi_enable = ht_scp;
  }

  /* Get max VHT rate */
  if (MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11ac)) {
    uint32 vht_nss = 0;
    uint32 vht_mcs = 0;
    uint16 vht_tx_mcs_map = 0;
    uint32 vht_max_phy_rate = 0;
    uint32 vht_1ss_phy_rate = 0;

    vht_tx_mcs_map = WLAN_TO_HOST16(mac80211_sta->deflink.vht_cap.vht_mcs.rx_mcs_map);
    for (vht_nss = 8; vht_nss > 0; vht_nss--) {
      vht_mcs = ((vht_tx_mcs_map >> (2 * (vht_nss - 1))) & 3);
      if (vht_mcs != IEEE80211_VHT_MCS_NOT_SUPPORTED)
        break;
    }

    if (vht_mcs != IEEE80211_VHT_MCS_NOT_SUPPORTED) {
      uint32 vht_cap;
      uint32 vht_bw;
      uint32 vht_scp = 0;
      uint32 mcs_1ss, mcs_max;

      vht_mcs = 7 + vht_mcs;
      vht_cap = mac80211_sta->deflink.vht_cap.cap;

      /* For 2.4 GHz and 5 GHz bands up to 40 MHz bandwidth, take bw and scp from HT IE's */
      if (width < CW_80) {
        vht_bw = ht_bw;
        vht_scp = ht_scp;
      } else {
        if (vht_cap & IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ) {
          vht_bw = IEEE80211_STA_RX_BW_160;
          if (vht_cap & IEEE80211_VHT_CAP_SHORT_GI_160)
            vht_scp = 1;
        } else {
          vht_bw = IEEE80211_STA_RX_BW_80;
          if (vht_cap & IEEE80211_VHT_CAP_SHORT_GI_80)
            vht_scp = 1;
        }
      }

      vht_scp = _wave_update_ht_and_vht_gi_if_fixed(&gi_and_ltf, vht_scp);

      vht_bw = min(width, vht_bw);
      /* Look for max MCS supported for both 1 and max SS */
      for (mcs_1ss = vht_mcs; mcs_1ss >= 7; mcs_1ss--) {
        if (mtlk_bitrate_hw_params_supported_rate(&vht_1ss_phy_rate, PHY_MODE_AC, vht_bw, vht_scp, mcs_1ss, 1))
          break;
      }
      for (mcs_max = vht_mcs; mcs_max >= 7; mcs_max--) {
        if (mtlk_bitrate_hw_params_supported_rate(&vht_max_phy_rate, PHY_MODE_AC, vht_bw, vht_scp, mcs_max, vht_nss))
          break;
      }
      if (rate_1ss < vht_1ss_phy_rate) {
        rate_1ss = vht_1ss_phy_rate;
      }
      if (max_rate < vht_max_phy_rate) {
        max_nss  = vht_nss;
        max_rate = vht_max_phy_rate;
      }
      max_supp_mcs = vht_mcs;
      max_supp_bw = vht_bw;
      sgi_enable = vht_scp;
      ILOG1_DDDDDD("STA VHT IE max_mcs:%u nss:%u bw:%u scp:%u phy_rate (1ss/max):(%u/%u)",
                   vht_mcs, vht_nss, vht_bw, vht_scp, vht_1ss_phy_rate, vht_max_phy_rate);
    } else {
      ELOG_V("VHT MCS is not found");
    }
  }

  /* Get max HE rate */
  if (MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11ax)) {
    uint32 he_nss = 0;
    uint32 he_mcs = 0;
    uint16 he_tx_mcs_map = 0;
    uint8  *he_phy_cap_info = mac80211_sta->deflink.he_cap.he_cap_elem.phy_cap_info;
    uint32 he_max_phy_rate = 0;
    uint32 he_1ss_phy_rate = 0;
    uint32 he_bw= IEEE80211_STA_RX_BW_20;
    uint32 he_scp = 0;

    if (!gi_and_ltf.isAuto)
      he_scp = gi_and_ltf.ltfAndGi;

    /* Process BW */
    if (MTLK_HW_BAND_2_4_GHZ == core_cfg_get_freq_band_cur(nic)) {
      if (he_phy_cap_info[0] &  IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_IN_2G)
       he_bw = IEEE80211_STA_RX_BW_40;
      else
       he_bw = IEEE80211_STA_RX_BW_20;
      } else {
      if (he_phy_cap_info[0] &  HE_5G_6G_PHY_CAP0_CHAN_WIDTH_SET_40MHZ_80MHZ)
        he_bw = IEEE80211_STA_RX_BW_80;
      else
        he_bw = IEEE80211_STA_RX_BW_20;
    }

    /* Process <= 80 MHz BW */
    he_tx_mcs_map = WLAN_TO_HOST16(mac80211_sta->deflink.he_cap.he_mcs_nss_supp.rx_mcs_80);
    for (he_nss = 8; he_nss > 0; he_nss--) {
      he_mcs = ((he_tx_mcs_map >> (2 * (he_nss - 1))) & 3);
       if (he_mcs != IEEE80211_HE_MCS_NOT_SUPPORTED)
         break;
     }

    if (he_mcs != IEEE80211_HE_MCS_NOT_SUPPORTED) {

      he_mcs = 7 + he_mcs * 2;
      he_bw = min(width, he_bw);
      he_1ss_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_AX, he_bw, he_scp, he_mcs, 1);
      he_max_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_AX, he_bw, he_scp, he_mcs, he_nss);
      if (max_rate < he_max_phy_rate) {
        max_nss  = he_nss;
        max_mcs  = he_mcs;
        max_rate = he_max_phy_rate;
        rate_1ss = he_1ss_phy_rate;
      }
      ILOG1_DDDDDD("STA HE <= 80MHz IE max_mcs:%u nss:%u bw:%d scp:%u phy_rate (1ss/max):(%u/%u)",
                   he_mcs, he_nss, he_bw, he_scp, he_1ss_phy_rate, he_max_phy_rate);
    } else {
      ELOG_V("HE MCS is not found");
    }

    if (width == IEEE80211_STA_RX_BW_160) {
      /* Process 160 MHz BW */
      if ((MTLK_HW_BAND_5_2_GHZ == core_cfg_get_freq_band_cur(nic) || MTLK_HW_BAND_6_GHZ == core_cfg_get_freq_band_cur(nic)) &&
        (he_phy_cap_info[0] &  HE_5G_6G_PHY_CAP0_CHAN_WIDTH_SET_160MHZ)) {

        he_bw  = width;
        he_mcs = 0;
        he_nss = 0;

        he_tx_mcs_map = WLAN_TO_HOST16(mac80211_sta->deflink.he_cap.he_mcs_nss_supp.rx_mcs_160);
        for (he_nss = 8; he_nss > 0; he_nss--) {
          he_mcs = ((he_tx_mcs_map >> (2 * (he_nss - 1))) & 3);
          if (he_mcs != IEEE80211_HE_MCS_NOT_SUPPORTED)
            break;
        }

        if (he_mcs != IEEE80211_HE_MCS_NOT_SUPPORTED) {
          he_mcs = 7 + he_mcs * 2;
          he_1ss_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_AX, he_bw, he_scp, he_mcs, 1);
          he_max_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_AX, he_bw, he_scp, he_mcs, he_nss);
          if (max_rate < he_max_phy_rate) {
            max_nss  = he_nss;
            max_mcs  = he_mcs;
            max_rate = he_max_phy_rate;
            rate_1ss = he_1ss_phy_rate;
          }
          ILOG1_DDDDDD("STA HE 160 MHz IE max_mcs:%u nss:%u bw:%u scp:%u phy_rate (1ss/max):(%u/%u)",
                       he_mcs, he_nss, he_bw, he_scp, he_1ss_phy_rate, he_max_phy_rate);
        } else {
          ELOG_V("HE MCS is not found");
        }
      }
    }
    /* Get max EHT rate */
    if (MTLK_BFIELD_GET(sta->info.flags, STA_FLAGS_11be) ) {
      uint32 eht_nss = 0;
      uint32 eht_mcs = 0;
      uint32 eht_bw= width;
      uint8  eht_tx_mcs_map = 0;
      int eht_mcs_group = 0;
      uint32 eht_max_phy_rate = 0;
      uint32 eht_1ss_phy_rate = 0;
      uint32 eht_scp = he_scp;
      uint8  *eht_phy_cap_info = mac80211_sta->deflink.eht_cap.eht_cap_elem.phy_cap_info;
      uint8  *p_mcs_nss_supp = NULL;

      /* Process 320 MHz BW */
      if (width == IEEE80211_STA_RX_BW_320) {
        if ((MTLK_HW_BAND_6_GHZ == core_cfg_get_freq_band_cur(nic)) &&
            (eht_phy_cap_info[0] &  IEEE80211_EHT_PHY_CAP0_320MHZ_IN_6GHZ)) {
 
          /* eht_mcs_group represents 0 -> 0-9 MCS  1 -> 10-11  2 -> 12-13 */
          p_mcs_nss_supp = mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.bw._320.rx_tx_max_nss;
          for (eht_mcs_group = 2; eht_mcs_group >=0 ; eht_mcs_group--) {
          eht_tx_mcs_map = p_mcs_nss_supp[eht_mcs_group];
          eht_nss = eht_tx_mcs_map & 0x0f;
            /* Nss Values : Not supported -> 0  Supported -> 1-8  Reserved -> 9-15 */
            if ( eht_nss != 0 && eht_nss < 9 )
              break;
          }

        }
      } else if (width == IEEE80211_STA_RX_BW_160) {
      /* Process 160 MHz BW */
        if ((MTLK_HW_BAND_5_2_GHZ == core_cfg_get_freq_band_cur(nic) || MTLK_HW_BAND_6_GHZ == core_cfg_get_freq_band_cur(nic)) &&
            (he_phy_cap_info[0] &  HE_5G_6G_PHY_CAP0_CHAN_WIDTH_SET_160MHZ)) {

          p_mcs_nss_supp = mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.bw._160.rx_tx_max_nss;
          /* eht_mcs_group represents 0 -> 0-9 MCS  1 -> 10-11  2 -> 12-13 */
          for (eht_mcs_group = 2; eht_mcs_group >=0 ; eht_mcs_group--) {
            eht_tx_mcs_map = p_mcs_nss_supp[eht_mcs_group];
            eht_nss = eht_tx_mcs_map & 0x0f;
            /* Nss Values : Not supported -> 0  Supported -> 1-8  Reserved -> 9-15 */
            if ( eht_nss != 0 && eht_nss < 9 )
              break;
          }
        }
      } else if (width == IEEE80211_STA_RX_BW_40 || width == IEEE80211_STA_RX_BW_80 || mtlk_sta_is_wds(sta)) { /* Process 40,80 MHz BW */
        p_mcs_nss_supp = mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.bw._80.rx_tx_max_nss;
        /* eht_mcs_group represents 0 -> 0-9 MCS  1 -> 10-11  2 -> 12-13 */
        for (eht_mcs_group = 2; eht_mcs_group >= 0; eht_mcs_group--) {
          eht_tx_mcs_map = p_mcs_nss_supp[eht_mcs_group];
          eht_nss = eht_tx_mcs_map & 0x0f;
          /* Nss Values : Not supported -> 0  Supported -> 1-8  Reserved -> 9-15 */
        if (eht_nss != 0 && eht_nss < 9)
          break;
        }
      } else { /* Process 20 MHz BW. Only for Non-AP STA */
        /* eht_mcs_group represents 0 -> 0-7 MCS  1 -> 8-9  2 -> 10-11  3 -> 12-13 */
        p_mcs_nss_supp = (uint8 *)&(mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.only_20mhz.rx_tx_mcs7_max_nss);
        for (eht_mcs_group = 3; eht_mcs_group >= 0; eht_mcs_group--) {
          eht_tx_mcs_map = p_mcs_nss_supp[eht_mcs_group];
          eht_nss = eht_tx_mcs_map & 0x0f;
          if (eht_nss != 0 && eht_nss < 9)
            break;
        }
      }

      /* We only support 4Nss*/
      if ( eht_nss != 0 && eht_nss < 5 ) {
        if (width == IEEE80211_STA_RX_BW_20)
          eht_mcs = 7 + (eht_mcs_group * 2);
        else
          eht_mcs = 9 + (eht_mcs_group * 2);
        eht_1ss_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_11BE, eht_bw, eht_scp, eht_mcs, 1);
        eht_max_phy_rate = mtlk_bitrate_hw_params_to_rate(PHY_MODE_11BE, eht_bw, eht_scp, eht_mcs, eht_nss);
        if (max_rate < eht_max_phy_rate) {
          max_nss  = eht_nss;
          max_mcs  = eht_mcs;
          max_rate = eht_max_phy_rate;
          rate_1ss = eht_1ss_phy_rate;
        }
        ILOG1_DDDDDDD("STA EHT %d MHz IE max_mcs:%u nss:%u bw:%u scp:%u phy_rate (1ss/max):(%u/%u)",
                      width, eht_mcs, eht_nss, eht_bw, eht_scp, eht_1ss_phy_rate, eht_max_phy_rate);
      } else {
          ELOG_D("%d MHz not supported in this band",width);
      }
    }
    max_supp_mcs = max_mcs;
    max_supp_bw = he_bw;
    sgi_enable = he_scp;
  }

  mtlk_sta_set_bitrate_1ss(sta, rate_1ss);
  mtlk_sta_set_bitrate_max(sta, max_rate);
  mtlk_sta_set_max_antennas(sta, max_nss);
  mtlk_sta_set_max_mcs(sta, max_supp_mcs);
  mtlk_sta_set_max_bw(sta, max_supp_bw);
  mtlk_sta_set_short_gi(sta, sgi_enable);
  ILOG1_DDD("STA max_antennas:%u phy_rate (1ss/max):(%u/%u)", max_nss, rate_1ss, max_rate);
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_radio_sta_list (mtlk_handle_t hcore, const void *data, uint32 data_size)
{
  mtlk_error_t res = MTLK_ERR_OK;
  uint32 radio_sta_cnt = 0, size = 0, ret;
  wave_radio_t  *radio;
  mtlk_df_t *df = NULL;
  mtlk_df_t *master_df = NULL;
  mtlk_df_user_t *df_user = NULL;
  mtlk_vap_manager_t *vap_mgr;
  mtlk_vap_handle_t vap_handle;
  unsigned vaps_count, vap_id;
  mtlk_stadb_iterator_t iter;
  const sta_entry      *sta = NULL;
  mtlk_wssa_peer_list_t *radio_peer_list = NULL, *tmp_list = NULL;
  mtlk_wssa_peer_list_t peer_list;
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);
  MTLK_ASSERT(core != NULL);
  df = mtlk_vap_get_df(core->vap_handle);
  radio = wave_vap_radio_get(core->vap_handle);
  MTLK_ASSERT(radio != NULL);
  df_user = mtlk_df_get_user(df);
  master_df = mtlk_df_user_get_master_df(df_user);
  MTLK_RET_CHECK_DF(master_df);

  MTLK_CLPB_TRY_NODATA()
    radio_sta_cnt = wave_radio_sta_cnt_get(radio);
    if (radio_sta_cnt == 0) {
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    }

    size = sizeof(mtlk_wssa_peer_list_t) * (radio_sta_cnt);
    radio_peer_list = mtlk_osal_mem_alloc(size, MTLK_MEM_TAG_EXTENSION);
    if (radio_peer_list == NULL) {
      ELOG_V("Can't allocate memory for radio peers");
      MTLK_CLPB_EXIT(MTLK_ERR_NO_MEM);
    }

    vap_mgr = mtlk_df_get_vap_manager(master_df);
    vaps_count = mtlk_vap_manager_get_max_vaps_count(vap_mgr);
    tmp_list = radio_peer_list;
    for (vap_id = 0; vap_id < vaps_count; vap_id++) {
      if (mtlk_vap_manager_get_vap_handle(vap_mgr, vap_id, &vap_handle) == MTLK_ERR_OK) {
        mtlk_core_t *core = mtlk_vap_get_core(vap_handle);
        sta = mtlk_stadb_iterate_first(&core->slow_ctx->stadb, &iter);
        if (sta) {
          do {
            wave_sta_get_wssa_peer_list(sta, &peer_list);
            wave_memcpy(tmp_list++, sizeof(mtlk_wssa_peer_list_t),
                        &peer_list, sizeof(mtlk_wssa_peer_list_t));
            sta = mtlk_stadb_iterate_next(&iter);
            radio_sta_cnt --;
          } while(sta && radio_sta_cnt > 0);
          mtlk_stadb_iterate_done(&iter);
        }
      }
    }
  MTLK_CLPB_FINALLY(res)
    ret = mtlk_clpb_push_res(clpb, res);
    if (MTLK_ERR_OK == ret && MTLK_ERR_OK == res)
      ret = mtlk_clpb_push(clpb, &radio_sta_cnt, sizeof(uint32));
    if (MTLK_ERR_OK == ret && MTLK_ERR_OK == res)
      ret = mtlk_clpb_push_nocopy(clpb, radio_peer_list, size);
    if (MTLK_ERR_OK != ret && radio_peer_list != NULL) {
      mtlk_osal_mem_free(radio_peer_list);
    }
    return ret;
  MTLK_CLPB_END;
}

void __MTLK_IFUNC
mtlk_core_save_chan_statistics_info (mtlk_core_t *core, struct intel_vendor_channel_data *ch_data)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  struct mtlk_chan_def *ccd = __wave_core_chandef_get(core);
  BOOL ch_radar_noise = mtlk_core_get_ieee80211_radar_detected(core, ccd->chan.center_freq);
  uint8   radio_id;

  radio_id = wave_vap_radio_id_get(core->vap_handle);

  if (mtlk_hw_type_is_gen7(hw)) {
    MTLK_ASSERT(radio_id < GEN7_NUM_OF_BANDS);
  } else {
    MTLK_ASSERT(radio_id < GEN6_NUM_OF_BANDS);
  }

  mtlk_hw_save_chan_statistics_info(hw, ccd, ch_data, ch_radar_noise, radio_id);
  return;
}
