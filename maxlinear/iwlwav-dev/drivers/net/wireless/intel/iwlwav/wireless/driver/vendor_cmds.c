/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*! \file   vendor_cmds.c
 *  \brief  iwlwav driver NL80211 vendor command handlers
 */

#include "mtlkinc.h"
#include "vendor_cmds.h"
#include "mtlk_df_priv.h"
#include "mtlk_df.h"
#include "cfg80211.h"
#include "mac80211.h"
#include "mtlk_df_nbuf.h"
#include "core_config.h"
#include "mtlk_coreui.h"
#include "wave_80211ax.h"

#define LOG_LOCAL_GID             GID_VENDOR_CMD
#define LOG_LOCAL_FID             0
#define TEXT_ARRAY_SIZE           1024

#define WAVE_CHECK_VENDOR_DATA(data) {                                              \
  if (__UNLIKELY(NULL == data)) {                                                   \
    ELOG_V("Received Vendor data is NULL");                                         \
    return -EINVAL;                                                                 \
  }                                                                                 \
}

#define WAVE_CHECK_VENDOR_SIZE(size, expected_size) {                               \
  if (__UNLIKELY(size != expected_size)) {                                          \
    ELOG_DD("Incorrect data size %u (expected %u)", size, expected_size);           \
    return -EINVAL;                                                                 \
  }                                                                                 \
}

#define WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, size, expected_size) {                \
  WAVE_CHECK_VENDOR_DATA(data);                                                     \
  WAVE_CHECK_VENDOR_SIZE(size, expected_size);                                      \
}

static const uint8 _bcast_addr[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/*! \brief      Release Association ID (AID) from driver/FW
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: AID, must not be NULL or 0
 *  \param[in]  data_len  Data length: must be sizeof(uint16)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_free_aid (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  uint16 aid;
  struct net_device *ndev = wdev->netdev;
  MTLK_ASSERT(NULL != ndev);

  if (wdev->iftype != NL80211_IFTYPE_AP) {
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(aid));

  aid = *(uint16 *)data;

  if (aid == 0)
    return -EINVAL;

  return wave_radio_aid_free(ndev, aid);
}

/*! \brief      Set softbolck thresholds for Specified STA MAC address in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_blacklist_cfg)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_hapd_set_softblock_threshold (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct intel_vendor_blacklist_cfg softblocklist_cfg;

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  /* Expecting Hostap to send only mac addr and the Probe Request High and Low Water Marks */
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(softblocklist_cfg));

  wave_memcpy(&softblocklist_cfg, sizeof(softblocklist_cfg), data, sizeof(softblocklist_cfg));

  if (!mtlk_osal_is_valid_ether_addr(softblocklist_cfg.addr.au8Addr)) {
    ELOG_Y("Zero or broadcast or multicast MAC address not allowed %Y", softblocklist_cfg.addr.au8Addr);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
          WAVE_CORE_REQ_SET_SOFTBLOCKLIST_ENTRY, &clpb, &softblocklist_cfg,
          sizeof(softblocklist_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb,
          WAVE_CORE_REQ_SET_SOFTBLOCKLIST_ENTRY, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/************************* SET FUNCTIONS *************************/
/*! \brief      Set integer parameters in driver specified by \a cmd
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *  \param[in]  cmd       Command ID: must be less than PRM_ID_RADIO_LAST
 *
 *  \return     Linux error code
 *
 */
static int set_int_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                           const void *data, int data_len, uint16 cmd)
{
  int res = MTLK_ERR_VALUE;
  uint16 recv_num_of_params = data_len / sizeof(int);
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);
  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    goto end;
  }

  WAVE_CHECK_VENDOR_DATA(data);

  res = mtlk_df_ui_set_cfg(df_user, cmd, (char*) data, MTLK_IW_PRIW_PARAM(INT_ITEM,recv_num_of_params));

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set MAC address data specified by \a cmd
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *  \param[in]  cmd       Command ID: must be less than PRM_ID_RADIO_LAST
 *
 *  \return     Linux error code
 *
 */
static int set_addr_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                            const void *data, int data_len, uint16 cmd)
{
  int res = MTLK_ERR_VALUE;
  struct sockaddr addr_param;
  uint16 recv_num_of_params = data_len / (sizeof(addr_param.sa_family) + ETH_ALEN);
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);

  if (mtlk_df_user_from_wdev(wdev) == NULL) {
    res = MTLK_ERR_NOT_READY;
    goto end;
  }

  if (recv_num_of_params != 1) {
    ELOG_V("Currently only one ADDR parameter is supported");
    goto end;
  }

  memset(&addr_param, 0, sizeof(addr_param));
  wave_memcpy(&addr_param, sizeof(addr_param), data, data_len);

  res = mtlk_df_ui_set_cfg(df_user, cmd, (char*) &addr_param, MTLK_IW_PRIW_PARAM(ADDR_ITEM,recv_num_of_params));

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set data from text specified by \a cmd
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *  \param[in]  cmd       Command ID: must be less than PRM_ID_RADIO_LAST
 *
 *  \return     Linux error code
 *
 */
static int set_text_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                            const void *data, int data_len, uint16 cmd)
{
  int res;
  uint16 recv_num_of_params = data_len / sizeof(char);
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);

  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    goto end;
  }

  WAVE_CHECK_VENDOR_DATA(data);

  res = mtlk_df_ui_set_cfg(df_user, cmd, (char*) data, MTLK_IW_PRIW_PARAM(TEXT_ITEM,recv_num_of_params));
end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Disable or Enable Softbolck feature in driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint32)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_hapd_set_disable_softblock (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  int softblock_disable;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint32));

  softblock_disable = *(uint32 *)data;
  return set_int_params(wiphy, wdev, &softblock_disable, sizeof(softblock_disable), PRM_ID_SFTBLCK_DIS);
}

/*! \brief      Disable or Enable radar detection in driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_11hRadarDetect (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_RADAR_DETECTION);
}

/*! \brief      Set debug Channel Availability Check (CAC) time in driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: debug CAC time, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_11hChCheckTime (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_CHANNEL_AVAILABILITY_CHECK_TIME);
}

/*! \brief      Emulate Radar detected event
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_11hEmulatRadar (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_EMULATE_RADAR_DETECTION);
}

/*! \brief      Emulate Interferer event
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_emulate_interferer (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  int res = MTLK_ERR_VALUE;
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);
  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    goto end;
  }
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  res = mtlk_df_ui_set_cfg(df_user, PRM_ID_EMULATE_INTERFERER, NULL, 0);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Legacy WDS, connect to peer AP by MAC address specified in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AddPeerAP (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_addr_params(wiphy, wdev, data, data_len, PRM_ID_ADD_PEERAP);
}

/*! \brief      Legacy WDS, disconnect peer AP by MAC address specified in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_DelPeerAP (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_addr_params(wiphy, wdev, data, data_len, PRM_ID_DEL_PEERAP);
}

/*! \brief      Legacy WDS, set WEP key index, 0 for Open security
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: key index, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_PeerAPkeyIdx (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PEERAP_KEY_IDX);
}

/*! \brief      Set Bridge mode in driver for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: bridge mode, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_BridgeMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_BRIDGE_MODE);
}

/*! \brief      Enable or Disable Reliable multicast for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: Reliable multicast flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ReliableMcast (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RELIABLE_MULTICAST);
}

/*! \brief      Enable or Disable AP Forwarding for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: AP Forwarding flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_APforwarding (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_FORWARDING);
}

/*! \brief      Enable or Disable LitePath for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: LitePath flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_LtPathEnabled (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
#if MTLK_USE_DIRECTCONNECT_DP_API
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DCDP_API_LITEPATH);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

/*! \brief      Enable or Disable PPA softpath for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: PPA softpath flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_IpxPpaEnabled (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
#if MTLK_USE_DIRECTCONNECT_DP_API
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DCDP_API_LITEPATH);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

/*! \brief      Set Code of Conduct (CoC) antenna power mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_CoCPower (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_COC_POWER_MODE);
}

/*! \brief      Set Code of Conduct (CoC) antenna Auto power mode parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_CoCAutoCfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_COC_AUTO_PARAMS);
}

/*! \brief      Set TPC Loop type (0 - Closed, 1 - Open)
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_TpcLoopType (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TPC_LOOP_TYPE);
}

/*! \brief      Set Interference Detection Thresholds
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_InterfDetThresh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_INTERFER_THRESH);
}

/*! \brief      Set Active antenna configuration for 802.11b mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_11bAntSelection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11B_ANTENNA_SELECTION);
}

/*! \brief      Set FW recovery configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_FWRecovery (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FW_RECOVERY);
}

/*! \brief      Enable or Disable out of scan beacon caching
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_OutOfScanCaching (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_OUT_OF_SCAN_CACHING);
}

/*! \brief      Allow or Disallow starting scan during CAC time
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: allow/disallow flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AllowScanDuringCac (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                          const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ALLOW_SCAN_DURING_CAC);
}

/*! \brief      Enable or Disable RF Radio in FW/PHY
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_EnableRadio (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RADIO_MODE);
}

/*! \brief      Set aggregation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AggrConfig (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AGGR_CONFIG);
}

/*! \brief      Set number of MSDUs in AMSDU
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_NumMsduInAmsdu (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AMSDU_NUM);
}

/*! \brief      Set aggregation rate limit
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AggRateLimit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AGG_RATE_LIMIT);
}

/*! \brief      Set available admission capacity
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AvailAdmCap (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ADMISSION_CAPACITY);
}

/*! \brief      Set high reception threshold value
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_SetRxTH (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RX_THRESHOLD);
}

/*! \brief      Set configuration of time intervals for high reception threshold
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_RxDutyCyc (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RX_DUTY_CYCLE);
}

/*! \brief      Reduce TX power by specified value in dBm
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_PowerSelection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TX_POWER_LIMIT_OFFSET);
}

/*! \brief      Set 802.11n protection method
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_11nProtection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PROTECTION_METHOD);
}

/*! \brief      Do calibration on demand
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_CalibOnDemand (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TEMPERATURE_SENSOR);
}

/*! \brief      Enable or Disable QAM Plus mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_QAMplus (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_QAMPLUS_MODE);
}

/*! \brief      Set ACS Update interval for operating channel in ms
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AcsUpdateTo (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ACS_UPDATE_TO);
}

/*! \brief      Enable or Disable 802.11ac Multi-User (MU) operation
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MuOperation (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MU_OPERATION);
}

/*! \brief      Set Clear Channel Assessment (CCA) threshold configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_CcaTh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_CCA_THRESHOLD);
}

/*! \brief      Set Clear Channel Assessment (CCA) adaptation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_CcaAdapt (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_CCA_ADAPT);
}

/*! \brief      Set Radar RSSI threshold
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_RadarRssiTh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RADAR_RSSI_TH);
}

/*! \brief      Set Fils Beacon Flag
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_FilsBeaconFlag (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FILS_BEACON_FLAG);
}

/*! \brief      Set RTS configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_RTSmode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RTS_MODE);
}

#ifdef WAVE_ENABLE_PIE
/*! \brief      Set Proportional Integral Controller Enhanced (PIE) configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_PIEcfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PIE_CFG);
}

static int _wave_cfg80211_vendor_set_aqm_sta_en (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AQM_STA_EN);
}
#endif /* WAVE_ENABLE_PIE */

/*! \brief      Set maximum MPDU length
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MaxMpduLen (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MAX_MPDU_LENGTH);
}

/*! \brief      Set beamforming (BF) mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_BfMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_BF_MODE);
}

/*! \brief      Set number of active antennas by antenna mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ActiveAntMask (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ACTIVE_ANT_MASK);
}

/*! \brief      Add four address STA for List mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AddFourAddrSta (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_addr_params(wiphy, wdev, data, data_len, PRM_ID_4ADDR_STA_ADD);
}

/*! \brief      Delete four address STA from List mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_DelFourAddrSta (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_addr_params(wiphy, wdev, data, data_len, PRM_ID_4ADDR_STA_DEL);
}

/*! \brief      Set transmit operation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_TxopConfig (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TXOP_CONFIG);
}

/*! \brief      Set SSB mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_SsbMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SSB_MODE);
}

/*! \brief      Set Multi Radio (BT, ZigBee, WiFi) coexistance mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_EnableMRCoex (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_COEX_CFG);
}

/*! \brief      Set IPv4 multicast range configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_McastRange (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_text_params(wiphy, wdev, data, data_len, PRM_ID_MCAST_RANGE_SETUP);
}

/*! \brief      Set IPv6 multicast range configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_McastRange6 (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_text_params(wiphy, wdev, data, data_len, PRM_ID_MCAST_RANGE_SETUP_IPV6);
}

/*! \brief      Set unknown multicast filter mode configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_forward_unknown_mcast_flag (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PPM_ID_FORWARD_UNKNOWN_MCAST_FLAG);
}

/*! \brief      Set Online Algo Calibration Mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_OnlineACM (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ONLINE_CALIBRATION_ALGO_MASK);
}

/*! \brief      Set Offline Algo Calibration Mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_AlgoCalibrMask (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_CALIBRATION_ALGO_MASK);
}

/*! \brief      Set Restricted AC mode configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_RestrictAcMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RESTRICTED_AC_MODE);
}

/*! \brief      Set PD threshold configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_PdThresh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PD_THRESHOLD);
}

/*! \brief      Enable or Disable Fast Drop feature
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_FastDrop (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FAST_DROP);
}

/*! \brief      Set Effective Radiated Power (ERP) configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ErpSet (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ERP);
}

/*! \brief      Set 802.11ax Multi-User Static Plan configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MuStatPlanCfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_STATIC_PLAN_CONFIG);
}

/*! \brief      Enable or Disable collecting of probe requests
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_probe_req_list_mode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PROBE_REQ_LIST);
}

/*! \brief      Set CTS to Self duration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: duration, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_cts_to_self_to (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_CTS_TO_SELF_TO);
}

/*! \brief      Set AMPDU transmit density
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: density, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_set_tx_ampdu_density (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TX_AMPDU_DENSITY);
}

/*! \brief      Set Dynamic MU type configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_dynamic_mu_type (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DYNAMIC_MU_TYPE);
}

/*! \brief      Set HE MU fixed parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_set_he_mu_fixed_parameters (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_HE_MU_FIXED_PARAMTERS);
}

/*! \brief      Set HE MU duration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_he_mu_duration (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_HE_MU_DURATION);
}

/*! \brief      Set ETSI PPDU duration limits
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_etsi_ppdu_limits (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ETSI_PPDU_LIMITS);
}

/*! \brief      Set AP retry limit
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ap_retry_limit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_RETRY_LIMIT);
}

static int _wave_ieee80211_vendor_set_ap_exce_retry_limit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_EXCE_RETRY_LIMIT);
}

static int _wave_ieee80211_vendor_set_preamble_puncture_cfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                            const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PREAMBLE_PUNCTURE_CFG);
}

/* WIFI HAL API's*/

/*! \brief      Get associated device statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_associated_dev_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_associated_dev_stats(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated device Traffic identifier (TID) statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_associated_dev_tid_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_associated_dev_tid_stats(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get statistics for channels specified in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: channel list, must not be NULL
 *  \param[in]  data_len  Data length: variable
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_channel_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA(data);

  return wave_radio_get_channel_stats(wiphy, wdev->netdev, data, data_len);
}

/*! \brief      Get PHY statistics for current channel
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_phy_chan_status (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  return wave_radio_get_phy_channel_status(wiphy, wdev->netdev);
}

/*! \brief      Get associated device rate info receive statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_associated_dev_rate_info_rx_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_associated_dev_rate_info_rx_stats(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated device rate info transmit statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_associated_dev_rate_info_tx_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_associated_dev_rate_info_tx_stats(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get list of associated devices per specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       List of associated devices is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_peer_list (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  return wave_radio_get_peer_list(wiphy, wdev->netdev);
}

static int _wave_ieee80211_vendor_get_probe_reqs_info (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));
  return wave_radio_get_probe_req_list(wiphy, wdev->netdev, *(uint8*)data);
}

/*! \brief      Get associated devices flow statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_peer_flow_status (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_peer_flow_status(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated devices diagnotic results v2
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_dev_diag_result2 (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_dev_diag_result2(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated devices diagnotic results v3
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_dev_diag_result3 (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_dev_diag_result3(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated devices diagnotic results v3
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_associated_stations_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA(data);

  return wave_radio_get_associated_stations_stats(wiphy, wdev->netdev, (uint8 *)data, data_len);
}

/*! \brief      Get associated devices capabilities
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Capabilities are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_peer_capabilities (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_peer_capabilities(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get associated devices rate info
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Rate info is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_peer_rate_info (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_peer_rate_info(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get Max Tx power
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: Channel, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \note       Rate info is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_get_max_tx_power (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  return wave_core_get_max_tx_power(wiphy, wdev->netdev, (uint32 *)data);
}

/*! \brief      Get FW recovery statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_recovery_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  return wave_radio_get_recovery_statistics(wiphy, wdev->netdev);
}

/*! \brief      Get HW flow statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_hw_flow_status (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  return wave_radio_get_hw_flow_status(wiphy, wdev->netdev);
}

/*! \brief      Get TR181 interface statistics specified by \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_tr181_wlan_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  return wave_radio_get_tr181_wlan_statistics(wiphy, wdev->netdev);
}

/*! \brief      Get TR181 HW statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_tr181_hw_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  return wave_radio_get_tr181_hw_statistics(wiphy, wdev->netdev);
}

/*! \brief      Get TR181 associated device statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_tr181_peer_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_tr181_peer_statistics(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get GPIO statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       GPIO statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_priority_gpio_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  return wave_radio_get_priority_gpio_stats(wiphy, wdev->netdev);
}

/*! \brief      Set RTS rate configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_rts_rate (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RTS_RATE);
}

/*! \brief      Enable or Disable per Station statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_stations_statistics (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                           const void *data, int data_len)
{
#ifdef MTLK_PER_RATE_STAT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_STATIONS_STATS);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif /* MTLK_PER_RATE_STAT */
}

/*! \brief      Set Legacy WDS encryption configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_wds_wep_enc_cfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  int                      res = MTLK_ERR_OK;
  mtlk_df_user_t           *df_user = mtlk_df_user_from_wdev(wdev);
  mtlk_clpb_t              *clpb = NULL;
  mtlk_core_ui_enc_cfg_t   enc_cfg;
  int                      key_id;
  char                     *wep_key_data;
  int                      wep_key_data_len;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  if (df_user == NULL){
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NOT_READY);
  }

  WAVE_CHECK_VENDOR_DATA(data);
  if (data_len < (int)sizeof(key_id)) {
    ELOG_D("WEP key vendor command data size %d too small", data_len);
    return -EINVAL;
  }

  wep_key_data     = (char *) data + sizeof(key_id);
  wep_key_data_len = data_len - sizeof(key_id);

  memset(&enc_cfg, 0, sizeof(enc_cfg));

  enc_cfg.wep_enabled = TRUE;
  wave_memcpy(&key_id, sizeof(key_id), data, sizeof(key_id));
  if (WAVE_WEP_NUM_DEFAULT_KEYS < key_id ) {
    WLOG_S("%s: Unsupported WEP key index", wdev->netdev->name);
    res = MTLK_ERR_PARAMS;
    goto finish;
  }
  enc_cfg.key_id = key_id - 1;

  if (mtlk_df_ui_validate_wep_key(wep_key_data, wep_key_data_len) != MTLK_ERR_OK) {
    WLOG_S("%s: Bad WEP key", wdev->netdev->name);
    res = MTLK_ERR_PARAMS;
    goto finish;
  }

  enc_cfg.wep_key.length = wep_key_data_len;
  wave_memcpy(enc_cfg.wep_key.data, sizeof(enc_cfg.wep_key.data), wep_key_data, wep_key_data_len);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_WDS_WEP_ENC_CFG,
                                  &clpb, (char*)&enc_cfg, sizeof(enc_cfg));
  res = _mtlk_df_user_process_core_retval_void(res, clpb, WAVE_CORE_REQ_SET_WDS_WEP_ENC_CFG, TRUE);

finish:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/************************* GET FUNCTIONS *************************/

static int _get_int_params (struct wiphy *wiphy, mtlk_df_user_t *df_user,
                            const void *data, int data_len, uint16 cmd, uint16 num_of_params)
{
  int res = MTLK_ERR_OK;
  int *recv_data;
  int array_size = sizeof(int) * num_of_params;
  uint16 len = 0;
  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }
  recv_data = mtlk_osal_mem_alloc(array_size, MTLK_MEM_TAG_VENDOR_CMD_DATA);

  if (!recv_data)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_MEM);

  res = mtlk_df_ui_get_cfg(df_user, cmd, (char*)recv_data, MTLK_IW_PRIW_PARAM(INT_ITEM,num_of_params), &len);
  if (res) {
    mtlk_osal_mem_free(recv_data);
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, recv_data, (sizeof(int) * len));
  mtlk_osal_mem_free(recv_data);

  return res;
}

/*! \brief      Get integer parameters from driver specified by \a cmd
 *
 *  \param[in]  wiphy          Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev           Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data           Pointer to data: unused
 *  \param[in]  data_len       Data length: unused
 *  \param[in]  cmd            Command ID: must be less than PRM_ID_RADIO_LAST
 *  \param[in]  num_of_params  Number of parameters to be returned
 *
 *  \note       Integer parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int get_int_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                           const void *data, int data_len, uint16 cmd, uint16 num_of_params)
{
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);
  return _get_int_params(wiphy, df_user, data, data_len, cmd, num_of_params);
}

/*! \brief      Get integer parameters from master of the driver specified by \a cmd
 *
 *  \param[in]  wiphy          Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev           Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data           Pointer to data: unused
 *  \param[in]  data_len       Data length: unused
 *  \param[in]  cmd            Command ID: must be less than PRM_ID_RADIO_LAST
 *  \param[in]  num_of_params  Number of parameters to be returned
 *
 *  \note       Integer parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int get_master_int_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                                  const void *data, int data_len, uint16 cmd, uint16 num_of_params)
{
  mtlk_df_user_t *df_user;
  mtlk_df_t *master_df;
  mtlk_df_user_t *master_df_user;
  
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  master_df = mtlk_df_user_get_master_df(df_user);
  MTLK_CHECK_DF(master_df);
  master_df_user = mtlk_df_get_user(master_df);
  
  return _get_int_params(wiphy, master_df_user, data, data_len, cmd, num_of_params);
}

/*! \brief      Get data as text specified by \a cmd
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *  \param[in]  cmd       Command ID: must be less than PRM_ID_RADIO_LAST
 *
 *  \note       Text is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int get_text_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                            const void *data, int data_len, uint16 cmd)
{
  mtlk_df_user_t *df_user = mtlk_df_user_from_wdev(wdev);
  int res = MTLK_ERR_OK;
  char *recv_data;
  uint16 len = 0;
  int array_size = TEXT_ARRAY_SIZE;
  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  recv_data = mtlk_osal_mem_alloc(array_size, MTLK_MEM_TAG_VENDOR_CMD_DATA);
  if (!recv_data)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_MEM);

  res = mtlk_df_ui_get_cfg(df_user, cmd, (char*)recv_data, MTLK_IW_PRIW_PARAM(TEXT_ITEM,array_size), &len);
  if (res) {
    mtlk_osal_mem_free(recv_data);
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }

  res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, recv_data, array_size);
  mtlk_osal_mem_free(recv_data);

  return res;
}

/*! \brief      Get radar detection state in driver - Disabled or Enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_11hRadarDetect (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_RADAR_DETECTION, 1);
}


static int _wave_ieee80211_vendor_get_preamble_puncture_cfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                            const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PREAMBLE_PUNCTURE_CFG, 4);
}

/*! \brief      Get debug Channel Availability Check (CAC) time from driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Debug CAC time is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_11hChCheckTime (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_CHANNEL_AVAILABILITY_CHECK_TIME, 1);
}

/*! \brief      Legacy WDS, get WEP key index, 0 for Open security
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Key index is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_PeerAPkeyIdx (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PEERAP_KEY_IDX, 1);
}

/*! \brief      Legacy WDS, get connected peer AP MAC addresses
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Peer AP MAC addresses are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_PeerAPs (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_PEERAP_LIST);

}

/*! \brief      Get Bridge mode from driver for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Bridge mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_BridgeMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_BRIDGE_MODE, 1);
}

/*! \brief      Get Reliable multicast state for specified \a wdev - Disabled or Enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Reliable multicast state is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ReliableMcast (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RELIABLE_MULTICAST, 1);
}

/*! \brief      Get AP Forwarding state for specified \a wdev - Disabled or Enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       AP Forwarding state is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_APforwarding (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_FORWARDING, 1);
}

/*! \brief      Get parsed EEPROM data as text from driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       EEPROM data is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_EEPROM (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_EEPROM);
}

/*! \brief      Get Litepath state for specified \a wdev - Disabled or Enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Litepath state is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_LtPathEnabled (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
#if MTLK_USE_DIRECTCONNECT_DP_API
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_DCDP_API_LITEPATH, 1);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

/*! \brief      Get PPA softpath state for specified \a wdev - Disabled or Enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       PPA softpath state is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_IpxPpaEnabled (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
#if MTLK_USE_DIRECTCONNECT_DP_API
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_DCDP_API_LITEPATH, 1);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

/*! \brief      Get Data path mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Data path mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_datapath_mode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_DCDP_API_DATAPATH_MODE);
}

/*! \brief      Get Code of Conduct (CoC) antenna power mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CoC antenna power mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_CoCPower (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_COC_POWER_MODE, MTLK_COC_PW_MAX_CFG_PARAMS);
}

/*! \brief      Get Code of Conduct (CoC) antenna Auto power mode parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CoC antenna Auto power mode parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_CoCAutoCfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_COC_AUTO_PARAMS, MTLK_COC_AUTO_MAX_CFG_PARAMS);
}

/*! \brief      Get Effective Radiated Power (ERP) configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       ERP configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ErpCfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ERP, 10);
}

/*! \brief      Get TPC Loop type
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       TPC Loop type is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_TpcLoopType (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TPC_LOOP_TYPE, 1);
}

/*! \brief      Get Interference detection state - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_InterfDetThresh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_INTERFER_THRESH, 1);
}

/*! \brief      Get Maximum number of supported STAs
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Maximum number of supported STAs are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_APCapsMaxSTAs (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_CAPABILITIES_MAX_STAs, 1);
}

/*! \brief      Get Maximum number of supported VAPs
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Maximum number of supported VAPs are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_APCapsMaxVAPs (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_CAPABILITIES_MAX_VAPs, 1);
}

/*! \brief      Get active antenna configuration for 802.111b mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_11bAntSelection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_11B_ANTENNA_SELECTION, 3);
}

/*! \brief      Get FW recovery configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_FWRecovery (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FW_RECOVERY, 5);
}

/*! \brief      Get FW recovery status
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Status is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_RcvryStats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RECOVERY_STATS, 6);
}

/*! \brief      Get state of Out of scan beacon caching - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_OutOfScanCaching (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_OUT_OF_SCAN_CACHING, 1);
}

/*! \brief      Get state of scan during CAC time - Allowed or Disallowed
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_AllowScanDuringCac (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                          const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ALLOW_SCAN_DURING_CAC, 1);
}

/*! \brief      Get state of RF Radio in FW/PHY - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_EnableRadio (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RADIO_MODE, 1);
}

/*! \brief      Get aggregation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_AggrConfig (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AGGR_CONFIG, 3);
}

/*! \brief      Get number of MSDUs in AMSDU
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Number of MSDUs in AMSDU are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_NumMsduInAmsdu (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AMSDU_NUM, 4);
}

/*! \brief      Get aggregation rate limit
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Aggregation rate limit is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_AggRateLimit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AGG_RATE_LIMIT, 2);
}

/*! \brief      Get available admission capacity
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Available admission capacity is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_AvailAdmCap (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ADMISSION_CAPACITY, 1);
}

/*! \brief      Get high reception threshold value
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Value is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_SetRxTH (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RX_THRESHOLD, 1);
}

/*! \brief      Get configuration of time intervals for high reception threshold
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_RxDutyCyc (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                 const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RX_DUTY_CYCLE, 2);
}

/*! \brief      Get the reduced TX power configuration in dBm
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_PowerSelection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TX_POWER_LIMIT_OFFSET, 1);
}

/*! \brief      Get 802.11n protection method
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_11nProtection (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PROTECTION_METHOD, 1);
}

/*! \brief      Get temperature per antenna
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_Temperature (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TEMPERATURE_SENSOR, 1);
}

/*! \brief      Get QAM plus mode - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_QAMplus (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_QAMPLUS_MODE, 1);
}

/*! \brief      Get ACS Update interval for operating channel in ms
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Update interval is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_AcsUpdateTo (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ACS_UPDATE_TO, 1);
}

/*! \brief      Get 802.11ac Multi-User (MU) operation - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       MU operation is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_MuOperation (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MU_OPERATION, 1);
}

/*! \brief      Get Clear Channel Assessment (CCA) threshold configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CCA threshold configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_CcaTh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_CCA_THRESHOLD, MTLK_CCA_TH_PARAMS_LEN);
}

/*! \brief      Get Clear Channel Assessment (CCA) adaptation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CCA adaptation configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_CcaAdapt (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_CCA_ADAPT, MTLK_CCA_ADAPT_PARAMS_LEN);
}

/*! \brief      Get Radar RSSI threshold
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Radar RSSI threshold is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_RadarRssiTh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RADAR_RSSI_TH, 1);
}


/*! \brief      Get Beacon Radar Flag
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_FilsBeaconFlag (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FILS_BEACON_FLAG, 1);
}

/*! \brief      Get RTS configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_RTSmode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RTS_MODE, 2);
}

#ifdef WAVE_ENABLE_PIE
/*! \brief      Get Proportional Integral Controller Enhanced (PIE) configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       PIE configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_PIEcfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PIE_CFG, WAVE_PIE_CFG_PARAM_COUNT);
}

static int _wave_cfg80211_vendor_get_aqm_sta_en (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AQM_STA_EN, GEN7_STATION_BITMAP_SIZE_WORDS);
}
#endif /* WAVE_ENABLE_PIE */

/*! \brief      Get maximum MPDU length
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Maximum MPDU length is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_MaxMpduLen (struct wiphy *wiphy, struct wireless_dev *wdev,
                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MAX_MPDU_LENGTH, 1);
}

/*! \brief      Get beamforming (BF) mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       BF mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_BfMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_BF_MODE, 1);
}

/*! \brief      Get active antennas mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Active antennas mask is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_ActiveAntMask (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ACTIVE_ANT_MASK, 1);
}

/*! \brief      Get four address STAs added in List mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Four address STAs are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_FourAddrStas (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_4ADDR_STA_LIST);
}

/*! \brief      Get transmit operation configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Transmit operation configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_TxopConfig (struct wiphy *wiphy, struct wireless_dev *wdev,
                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TXOP_CONFIG, WAVE_TXOP_CFG_SIZE);
}

/*! \brief      Get SSB mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       SSB mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_SsbMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SSB_MODE, 2);
}

/*! \brief      Get Multi Radio (BT, ZigBee, WiFi) coexistance mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CoEx mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_EnableMRCoex (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_COEX_CFG, 4);
}

/*! \brief      Get IPv4 multicast range configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_McastRange (struct wiphy *wiphy, struct wireless_dev *wdev,
                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_MCAST_RANGE_SETUP);
}

/*! \brief      Get IPv6 multicast range configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_McastRange6 (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_MCAST_RANGE_SETUP_IPV6);
}

/*! \brief      Get unknown multicast filter mode configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_forward_unknown_mcast_flag (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                          const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PPM_ID_FORWARD_UNKNOWN_MCAST_FLAG, 1);
}

/*! \brief      Get Online Algo Calibration Mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Online Algo Calibration Mask is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_OnlineACM (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ONLINE_CALIBRATION_ALGO_MASK, 1);
}

/*! \brief      Get Offline Algo Calibration Mask
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Offline Algo Calibration Mask is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_AlgoCalibrMask (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_CALIBRATION_ALGO_MASK, 1);
}

/*! \brief      Get Restricted AC mode configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Restricted AC mode configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_RestrictAcMode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RESTRICTED_AC_MODE, MTLK_RESTRICTED_AC_MODE_CFG_SIZE);
}

/*! \brief      Get PD threshold configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       PD threshold configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_PdThresh (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PD_THRESHOLD, MTLK_PD_THRESH_CFG_SIZE);
}


/*! \brief      Get Fast Drop state - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Fast Drop state is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_FastDrop (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FAST_DROP, 1);
}

/*! \brief      Get Voltage and Temperature from Power-Voltage-Temperature (PWT) sensor
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
int _wave_ieee80211_vendor_get_pvt_sensor (struct wiphy *wiphy, struct wireless_dev *wdev,
                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PVT_SENSOR, 2);
}

/*! \brief      Get mode of collecting of probe requests - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Mode is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_probe_req_list_mode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PROBE_REQ_LIST, 1);
}

/*! \brief      Get AP retry limit
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       AP retry limit is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ap_retry_limit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_RETRY_LIMIT, 3);
}

/*! \brief      Get CTS to Self duration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Get CTS to Self duration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_cts_to_self_to (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_CTS_TO_SELF_TO, 1);
}

/*! \brief      Get AMPDU transmit density
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       AMPDU transmit density is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_tx_ampdu_density (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TX_AMPDU_DENSITY, 1);
}

/*! \brief      Set Multi-BSS operation mode for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint8)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_mbssid_vap (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  int mbssid_vap;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));

  mbssid_vap = *(uint8 *)data;
  return set_int_params(wiphy, wdev, &mbssid_vap, sizeof(mbssid_vap), PRM_ID_MBSSID_VAP);
}

/*! \brief      GET Radio statistics for specified \a wiphy
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Radio statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_get_radio_info (struct wiphy *wiphy,
                                                       struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  return wave_radio_radio_info_get(wiphy, wdev);
}

/*! \brief      Request Unconnected STA measurements by MAC address and
 *              channel definition specified in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_unconnected_sta_req_cfg)
 *
 *  \note       Unconnected STA measurements are returned LTQ_NL80211_VENDOR_EVENT_UNCONNECTED_STA
 *              vendor event
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_get_unconnected_sta (struct wiphy *wiphy,
                                                            struct wireless_dev *wdev,
                                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_unconnected_sta_req_cfg));

  return wave_radio_unconnected_sta_get(wiphy, wdev, (struct intel_vendor_unconnected_sta_req_cfg *)data);
}

/*! \brief      Request Association ID (AID) from driver/FW for specified MAC address
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       AID is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_get_aid (struct wiphy *wiphy,
                                                struct wireless_dev *wdev,
                                                const void *data,
                                                int data_len)
{
  u16 aid;
  int aid_len = sizeof(aid);
  int res;

  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  res = wave_radio_aid_get(wdev->netdev, (uint8 *)data, &aid);
  if (res)
    return res;

  return wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &aid, aid_len);
}

/*! \brief      Store STA Deauthentication parameters required on channel switch
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_channel_switch_cfg)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_store_chan_switch_deauth_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  mtlk_df_user_t *df_user = NULL;
  mtlk_error_t res = MTLK_ERR_OK;
  mtlk_clpb_t *clpb = NULL;
  struct intel_vendor_channel_switch_cfg *vendor_cs_cfg = (struct intel_vendor_channel_switch_cfg*)data;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(*vendor_cs_cfg));

  /* Invoke core to process the message further */
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
          WAVE_CORE_REQ_SET_CHAN_SWITCH_DEAUTH_PARAMS, &clpb, vendor_cs_cfg, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
          WAVE_CORE_REQ_SET_CHAN_SWITCH_DEAUTH_PARAMS, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Receive required initial data for driver from hostapd:
 *                        - Country code
 *                        - is AP in 802.11b mode
 *                        - is AP AC/AX compatible in case HE enabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_initial_data_cfg)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_receive_initial_data (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_initial_data_cfg));

  return wave_radio_initial_data_receive(wdev->netdev, (struct intel_vendor_initial_data_cfg *)data);
}

/*! \brief      Notification from hostapd that Station sync had been completed
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused param
 *  \param[in]  data_len  Data length: unused param
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_sync_done (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  return wave_radio_sync_done(wdev->netdev);
}

/*! \brief      Flag if Radar Debug Simulation feature enabled on DFS channel
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint8)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_dfs_debug_radar_required_chan (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                                 const void *data, int data_len)
{
  int dfs_debug;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));
  dfs_debug = *(uint8 *)data;

  return set_int_params(wiphy, wdev, &dfs_debug, sizeof(dfs_debug), PRM_ID_DFS_DEBUG);
}

/*! \brief      Add specified MAC address to the Blacklist for \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_blacklist_cfg)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_deny_mac (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_blacklist_cfg));

  return wave_radio_deny_mac_set(wdev->netdev, (struct intel_vendor_blacklist_cfg *)data);
}

/*! \brief      Add specified MAC address to the Blacklist for all VAPs in \a wiphy,
 *              except for the VAP, which address is specified in \a data
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_steer_cfg)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_sta_steer (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_steer_cfg));

  return wave_radio_sta_steer(wdev->netdev, (struct intel_vendor_steer_cfg *)data);
}

/*! \brief      Get STA measurements statistics from driver for speficied STA MAC address
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       STA measurements statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_get_sta_measurements (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_sta_measurements_get(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      GET VAP measurements statistics for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       VAP measurements statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_get_vap_measurements (struct wiphy *wiphy,
                                                             struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  return wave_radio_vap_measurements_get(wiphy, wdev->netdev);
}

/*! \brief      Set AirTime Fairness (ATF) quotas to FW
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: variable
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_atf_quotas (struct wiphy *wiphy,
                                                       struct wireless_dev *wdev,
                                                       const void *data, int total_len)
{
  int expected_len;

  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA(data);

  if (total_len < (int)sizeof(struct intel_vendor_atf_quotas)) {
    ELOG_D("Too small ATF quotas data len %d", total_len);
    return -EINVAL;
  }

  expected_len = sizeof(struct intel_vendor_atf_quotas) + ((struct intel_vendor_atf_quotas*)data)->data_len;
  WAVE_CHECK_VENDOR_SIZE(total_len, expected_len);

  return wave_radio_set_atf_quotas(wiphy, wdev->netdev, data, total_len);
}

/*! \brief      Add or Remove WDS WPA entry to the driver's list specified by
 *              \a data MAC address and action (add or remove)
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_mac_addr_list_cfg)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_wds_wpa_sta (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP){
    ELOG_D("Wrong iterface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_mac_addr_list_cfg));

  return wave_radio_set_wds_wpa_sta(wdev->netdev, (struct intel_vendor_mac_addr_list_cfg *)data);
}

/*! \brief      Enable or Disable BSS Load IE in Beacons and Probe responses
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint8)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_bss_load (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  int bss_load;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));
  bss_load = *(uint8 *)data;

  return set_int_params(wiphy, wdev, &bss_load, sizeof(bss_load), PRM_ID_BSS_LOAD);
}

/*! \brief      Get statistics poll period
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Statistics poll period is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_stats_poll_period (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  uint32 *stats_period;
  uint32 stats_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  df_user = mtlk_df_user_from_wdev(wdev);
  if (df_user == NULL) {
    res = MTLK_ERR_NOT_READY;
    goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_GET_STATS_POLL_PERIOD, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_GET_STATS_POLL_PERIOD, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  stats_period = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(stats_period, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, stats_period, sizeof(*stats_period));
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set number of maximum VAPs for Multi-BSS group on specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint8)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_hapd_set_num_vaps_in_group (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  int vaps_in_group;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));

  vaps_in_group = *(uint8 *)data;
  return set_int_params(wiphy, wdev, &vaps_in_group, sizeof(vaps_in_group), PRM_ID_VAPS_IN_GRP);
}

/*! \brief      Set RSSI threshhold for ignoring the probe request
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int32)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_rssi_ignore_probe_request (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                                      const void *data, int data_len)
{
  int rssi_ignore_probe_request;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(int));
  rssi_ignore_probe_request = *(int *)data;

  return set_int_params(wiphy, wdev, &rssi_ignore_probe_request, sizeof(rssi_ignore_probe_request), PRM_ID_RSSI_IGNORE_PROBE_REQ);
}

/*! \brief      Set Multi-AP mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_hapd_set_multi_ap_mode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                          const void *data, int data_len)
{
  int multi_ap_mode;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(multi_ap_mode));
  multi_ap_mode = *(int *)data;

  return set_int_params(wiphy, wdev, &multi_ap_mode, sizeof(multi_ap_mode), PRM_ID_MULTI_AP_MODE);
}

/*! \brief      Set statistics poll period
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_stats_poll_period (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;

  ILOG1_SSDD("%s: SET_STATS_POLL_PERIOD invoked from %s (%i), msg_len = %d", wdev->netdev->name,
    current->comm, current->pid, data_len);

  df_user = mtlk_df_user_from_wdev(wdev);
  if (df_user == NULL) {
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NOT_READY);
  }

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint32));

  /* Invoke core to process the message further */
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
          WAVE_CORE_REQ_SET_STATS_POLL_PERIOD, &clpb, data, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
          WAVE_CORE_REQ_SET_STATS_POLL_PERIOD, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get RTS rate configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       RTS rate configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_rts_rate (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RTS_RATE, 1);
}

/*! \brief      Get per Station statistics status - Enabled or Disabled
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Status is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_stations_statistics (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                           const void *data, int data_len)
{
#ifdef MTLK_PER_RATE_STAT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_STATIONS_STATS, 1);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif /* MTLK_PER_RATE_STAT */
}

/*! \brief      Get 20Mhz tx power
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       20Mhz tx power is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_20mhz_tx_power (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct mxl_vendor_tx_power *tx_power;
  uint32 tx_power_size;

  ILOG2_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_RADIO_REQ_GET_20MHZ_TX_POWER,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_GET_20MHZ_TX_POWER, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  tx_power = mtlk_clpb_enum_get_next(clpb, &tx_power_size);
  MTLK_CLPB_TRY(tx_power, tx_power_size)
    mtlk_dump(3, tx_power, tx_power_size, "20MHz tx Power:");
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, tx_power, tx_power_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Request proprietary phy capabilities from driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       capabilities are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_prop_phy_cap (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  if (wdev->iftype != NL80211_IFTYPE_AP) {
    ELOG_D("Wrong interface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }

  return wv_cfg80211_get_prop_phy_cap(wiphy, wdev->netdev);
}

/*! \brief      Request OBSS BssColor bitmap from Driver
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       BssColor bitmap is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_obss_bsscolor_info(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  uint64 obss_bsscolor_bitmap;

  if (wdev->iftype != NL80211_IFTYPE_AP) {
    ELOG_D("Wrong interface type %d, supported only AP", wdev->iftype);
    return -EINVAL;
  }
  obss_bsscolor_bitmap = ieee80211_get_obss_bsscolor_bitmap(wiphy);

  return wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, &obss_bsscolor_bitmap, sizeof(obss_bsscolor_bitmap));
}

/*! \brief      Get RTS threshold
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       RTS threshold is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_rts_threshold (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_RTS_THRESHOLD, 1);
}

/*! \brief      Set Management frames rate
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: rate, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(uint8)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_hapd_set_management_frames_rate (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  int rate;
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint8));
  rate = *(uint8 *)data;

  return set_int_params(wiphy, wdev, &rate, sizeof(rate), PRM_ID_MGMT_RATE);
}

/*************************************************************/

/*! \brief      Get Dynamic MU type configuration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Dynamic MU type configuration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_dynamic_mu_type (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_DYNAMIC_MU_TYPE, WAVE_DYNAMIC_MU_TYPE_CFG_SIZE);
}

/*! \brief      Get HE MU fixed parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       HE MU fixed parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_he_mu_fixed_parameters (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_HE_MU_FIXED_PARAMTERS, WAVE_HE_MU_FIXED_PARAMTERS_CFG_SIZE);
}

/*! \brief      Get HE MU duration
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       HE MU duration is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_he_mu_duration (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_HE_MU_DURATION, WAVE_HE_MU_DURATION_CFG_SIZE);
}

/*! \brief      Get ETSI PPDU duration limits
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       ETSI PPDU duration limits are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_etsi_ppdu_limits (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ETSI_PPDU_LIMITS,
                                                            WAVE_ETSI_PPDU_DURATION_LIMITS_MIN_SIZE);
}

/*! \brief      Get HE Operation information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       HE Operation information is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_hapd_get_he_operation (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct intel_vendor_he_oper *he_operation;
  uint32 he_oper_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_HE_OPERATION,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_HE_OPERATION, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  he_operation = mtlk_clpb_enum_get_next(clpb, &he_oper_size);
  MTLK_CLPB_TRY(he_operation, he_oper_size)
    mtlk_dump(3, he_operation, he_oper_size, "HE Operation:");
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, he_operation, he_oper_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get HE Non-advertised information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       HE Non-advertised information is returned via cfg80211_vendor_cmd_reply()
 * 
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_get_he_non_advertised (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct intel_vendor_he_capa *he_non_advert;
  uint32 he_non_advert_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_HE_NON_ADVERTISED,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_HE_NON_ADVERTISED, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  he_non_advert = mtlk_clpb_enum_get_next(clpb, &he_non_advert_size);
  MTLK_CLPB_TRY(he_non_advert, he_non_advert_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, he_non_advert, he_non_advert_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set HE Debug information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct mtlk_he_debug_mode_data)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_set_he_debug_data (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct mtlk_he_debug_mode_data he_data = {0};

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_he_capa));

  /* make sure we can memcpy the vendor data to struct mtlk_he_debug_mode_data */
  MTLK_STATIC_ASSERT(sizeof(struct intel_vendor_he_capa) ==
                     MTLK_OFFSET_OF(struct mtlk_he_debug_mode_data, he_debug_mode_enabled));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct intel_vendor_he_capa,    he_mac_capab_info) ==
                     MTLK_OFFSET_OF(struct mtlk_he_debug_mode_data, he_cap_elem.mac_cap_info));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct intel_vendor_he_capa,    he_phy_capab_info) ==
                     MTLK_OFFSET_OF(struct mtlk_he_debug_mode_data, he_cap_elem.phy_cap_info));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct intel_vendor_he_capa,    he_txrx_mcs_support) ==
                     MTLK_OFFSET_OF(struct mtlk_he_debug_mode_data, he_mcs_nss_supp));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct intel_vendor_he_capa,    he_ppe_thresholds) ==
                     MTLK_OFFSET_OF(struct mtlk_he_debug_mode_data, ppe_thresholds));

  he_data.he_debug_mode_enabled = 1;
  wave_memcpy(&he_data, sizeof(he_data) - sizeof(he_data.he_debug_mode_enabled), data, data_len);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_HE_DEBUG,
                                  &clpb, &he_data, sizeof(he_data));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_HE_DEBUG, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set HE Non-advertised information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: sizeof(struct intel_vendor_he_capa)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_set_he_non_advertised (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_he_capa));

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_HE_NON_ADVERTISED,
                                  &clpb, data, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_HE_NON_ADVERTISED, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get PHY inband power
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       PHY inband power is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_phy_inband_power (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_PHY_INBAND_POWER);
}

/*! \brief      Get associated devices TWT parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       TWT parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_twt_parameters (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_radio_get_twt_params(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get AX default parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       AX default parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_ax_default_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_master_int_params(wiphy, wdev, data, data_len, PRM_ID_AX_DEFAULT_PARAMS, UMI_GET_AX_DEFAULT_SIZE);
}

/*! \brief      Enable or Disable Zero Wait DFS
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_zwdfs_ant (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ZWDFS_ANT_CONFIG);
}

/*! \brief      Get state of Zero Wait DFS
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       State is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_get_zwdfs_ant (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ZWDFS_ANT_CONFIG, 1);
}

/*! \brief      Set Foreground scan parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ScanParams (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PARAMS);
}

/*! \brief      Set Background scan parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_ScanParamsBG (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PARAMS_BG);
}

/*! \brief      Get Foreground scan parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ScanParams (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PARAMS, NUM_IWPRIV_SCAN_PARAMS);
}

/*! \brief      Get Background scan parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Parameters are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ScanParamsBG (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                    const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PARAMS_BG,
                        NUM_IWPRIV_SCAN_PARAMS_BG);
}

/*! \brief      Set unsolicited transmit frame parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_unsolicited_frame_tx (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                           const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_UNSOLICITED_FRAME_TX);
}

/*! \brief      Enable or Disable HE Beacon explicit setting for FW in 6 GHz band
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_he_beacon (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_SIZE(data_len, sizeof(int));

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_HE_BEACON);
}

/*! \brief      Set Dynamic Multicast Mode and Rate
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MulticastRate(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DYNAMIC_MC_RATE);
}

/*! \brief      Send FILS dicovery frame
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_send_fils_discovery_frame (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                             const void *data, int data_len)
{
  mtlk_df_user_t *df_user = NULL;
  mtlk_error_t res = MTLK_ERR_PARAMS;
  mtlk_clpb_t *clpb = NULL;

  frame_head_t *frame;
  frame_action_head_t *action;

  int expected_data_len = sizeof(struct intel_vendor_fils_discovery_info) + sizeof(frame_action_head_t) + sizeof(frame_head_t);
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  /* FILS frame template len could be variable, but at least == expected_data_len */
  if (data_len >=  expected_data_len)
    expected_data_len = data_len;

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, expected_data_len);

  frame = (frame_head_t *) data;
  mtlk_dump(1, data, data_len, "FILS Discovery template");

  if (!ieee80211_is_mgmt(frame->frame_control))
    goto end;

  if (!ieee80211_is_action(frame->frame_control))
    goto end;

  action = (frame_action_head_t *)(data + sizeof(frame_head_t));

  if((action->category != WLAN_CATEGORY_PUBLIC) && (action->action != WLAN_PUB_ACTION_FILS_DISCOVERY))
    goto end;

  /* Invoke core to process the message further */
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
          WAVE_CORE_REQ_SET_FILS_DISCOVERY_FRAME, &clpb, data, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb,
          WAVE_CORE_REQ_SET_FILS_DISCOVERY_FRAME, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get Mu Ofdma Beamforming mode and period
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \return     Linux error code
 *
 */

static int _wave_ieee80211_vendor_get_MuOfdmaBf(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MU_OFDMA_BF, 2);
}

/*! \brief      Set MU OFDMA BeamForming config
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MuOfdmaBf(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MU_OFDMA_BF);
}

static int _wave_ieee80211_vendor_set_SlowProbingMask(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PROBING_MASK);
}

int _wave_ieee80211_vendor_get_SlowProbingMask(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PROBING_MASK, 1);
}

/*! \brief      Advertise broadcast TWT Schedule
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_advertise_btwt_schedule(struct wiphy *wiphy, struct wireless_dev *wdev,
							  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ADVERTISE_BTWT_SCHEDULE);
}

/*! \brief      Get advertised broadcast TWT Schedules
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_advertised_btwt_schedule(struct wiphy *wiphy, struct wireless_dev *wdev,
							       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ADVERTISE_BTWT_SCHEDULE, MAX_NUM_ADVERTISE_BTWT_PARAMS);
}


/*! \brief      Terminate broadcast TWT Schedule
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_terminate_btwt_schedule(struct wiphy *wiphy, struct wireless_dev *wdev,
							  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TERMINATE_BTWT_SCHEDULE);
}

static int _wave_ieee80211_vendor_tx_twt_teardown(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TX_TWT_TEARDOWN);
}

static int _wave_ieee80211_vendor_set_ScanModifFlags(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_MODIFS);
}

static int _wave_ieee80211_vendor_get_ScanModifFlags(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_MODIFS, 1);
}

static int _wave_ieee80211_vendor_set_ScanPauseBGCache(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PAUSE_BG_CACHE);
}

static int _wave_ieee80211_vendor_get_ScanPauseBGCache(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_PAUSE_BG_CACHE, 1);
}

static int _wave_ieee80211_vendor_set_FixedLtfGi(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_LTF_AND_GI);
}

int _wave_ieee80211_vendor_get_FixedLtfGi(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_LTF_AND_GI, 2);
}

static int _wave_ieee80211_vendor_set_WhmConfig(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_WHM_CONFIG);
}

static int _wave_ieee80211_vendor_set_WhmReset(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_WHM_RESET);
}

static int _wave_ieee80211_vendor_set_WhmTrigger(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_WHM_TRIGGER);
}

static int _wave_cfg80211_vendor_reset_statistics (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_reset_statistics));
  ILOG1_D("size in vendor %d", data_len);
  return wave_reset_statistics(wiphy, wdev->netdev, (struct intel_vendor_reset_statistics *)data);
}

/*! \brief      Enable or Disable Intra-VAP Multicast handling
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_intra_vap_mcast (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_SIZE(data_len, sizeof(int));

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_INTRA_VAP_MCAST);
}

static int _wave_ieee80211_vendor_set_probereq_offload_mode (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_SIZE(data_len, sizeof(int));

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PROBEREQ_OFFLOAD_MODE);
}

/*! \brief      Set Management Frame Power Control method
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_MgmtFramePwrCtrl (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MGMT_FRM_PWR_CTRL);
}

/*! \brief      Get Management Frame Power Control method
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_MgmtFramePwrCtrl (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MGMT_FRM_PWR_CTRL, 1);
}

static int _wave_ieee80211_vendor_set_CsiEnable (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_text_params(wiphy, wdev, data, data_len, PRM_ID_CSI_ENABLE);
}

static int _wave_ieee80211_vendor_get_CsiEnable (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));
  return wave_radio_get_csi_enable_params(wiphy, wdev->netdev, (uint8 *)data);
}

static int _wave_ieee80211_vendor_set_CsiSendQosNull (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_addr_params(wiphy, wdev, data, data_len, PRM_ID_CSI_SEND_QOS_NULL);
}

static int _wave_ieee80211_vendor_set_CsiAutoRate (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_text_params(wiphy, wdev, data, data_len, PRM_ID_CSI_AUTO_RATE);
}

static int _wave_ieee80211_vendor_get_CsiAutoRate (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));
  return wave_radio_get_csi_auto_params(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get Channel State Information Statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_CsiStats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                     const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));
  return wave_radio_get_CsiStats(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get Channel State Information Monitor Counters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Results are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_get_CsiCounters (struct wiphy *wiphy, struct wireless_dev *wdev,
                                       const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));
  return wave_radio_get_CsiCounters(wiphy, wdev->netdev, (uint8 *)data);
}

/*! \brief      Get Link Adapt MuOFDMA counter statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_get_LaMuGroupsCountersStats(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return wave_radio_get_la_mu_groups_counters_stats(wiphy, wdev->netdev);
}

/*! \brief      Get list of associated devices per radio specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       List of associated devices to radio is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_radio_peer_list (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return wave_radio_get_radio_peer_list(wiphy, wdev->netdev);
}

/*! \brief      Get Link Adapt he mimo ofdma stats
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       mimo stats returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_mimo_ofdma_stats(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(wave_vendor_su_mu_ru_stats_t));
  return wave_radio_get_mimo_stats(wiphy, wdev->netdev, (wave_vendor_su_mu_ru_stats_t *)data);
}

#ifdef MTLK_WAVE_700
static int _wave_ieee80211_vendor_request_ml_sid(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct mxl_vendor_ml_sid *ml_sid;
  struct mxl_ml_sta_mac_addr *ml_sta_mac = (struct mxl_ml_sta_mac_addr *)data;
  uint32 size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  ILOG1_Y("mac_addr1 = %Y", ml_sta_mac->addr1);
  ILOG1_Y("mac_addr2 = %Y", ml_sta_mac->addr2);
  ILOG1_Y("mac_addr3 = %Y", ml_sta_mac->addr3);

  /* Validate MAC address */
  if ((ml_sta_mac->assoc_link_bitmap & LINK_BIT_2_4G_IS_SET) && (!mtlk_osal_is_valid_ether_addr(ml_sta_mac->addr1))) {
    ELOG_SY("%s: The ML STA MAC addr1 %Y is invalid", wdev->netdev->name, ml_sta_mac->addr1);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  if ((ml_sta_mac->assoc_link_bitmap & LINK_BIT_5G_IS_SET) && (!mtlk_osal_is_valid_ether_addr(ml_sta_mac->addr2))) {
    ELOG_SY("%s: The ML STA MAC addr2 %Y is invalid", wdev->netdev->name, ml_sta_mac->addr2);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  if ((ml_sta_mac->assoc_link_bitmap & LINK_BIT_6G_IS_SET) && (!mtlk_osal_is_valid_ether_addr(ml_sta_mac->addr3))) {
    ELOG_SY("%s: The ML STA MAC addr3 %Y is invalid", wdev->netdev->name, ml_sta_mac->addr3);
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, (sizeof(*ml_sta_mac)));

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_ML_SID,
                                  &clpb, (uint8 *)ml_sta_mac, sizeof(*ml_sta_mac));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_ML_SID, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  ml_sid = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY(ml_sid, size)
    mtlk_dump(3, ml_sid, size, "ML Request SID:");
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ml_sid, size);
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
  return res;

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_set_ml_sid (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_ml_sta_info *ml_sta_info;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof( struct mxl_ml_sta_info));

  ml_sta_info = (struct mxl_ml_sta_info *)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SET_ML_SID, &clpb, ml_sta_info, sizeof(*ml_sta_info));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SET_ML_SID, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_mld_remove (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_mld_remove *mld_rem;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_mld_remove));

  mld_rem = (struct mxl_mld_remove *)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_REMOVE_MLD, &clpb, mld_rem, sizeof(struct mxl_mld_remove));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_REMOVE_MLD, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_ml_sta_add (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_ml_sta_add_param *ml_sta_add_params;
  u8 *main_link_id;
  uint32 size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof( struct mxl_ml_sta_add_param));

  ml_sta_add_params = (struct mxl_ml_sta_add_param *)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_ML_STA_ADD, &clpb, ml_sta_add_params, sizeof(*ml_sta_add_params));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_ML_STA_ADD, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  main_link_id = mtlk_clpb_enum_get_next(clpb, &size);
  MTLK_CLPB_TRY(main_link_id, size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, main_link_id, size);
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
    return res;

  end:
    return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      MLD BSS critical update info
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_set_bss_critical_update_info (struct wiphy *wiphy,
                                                               struct wireless_dev *wdev,
                                                               const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_vendor_ml_critical_update *ml_critical_update;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_vendor_ml_critical_update));

  ml_critical_update = (struct mxl_vendor_ml_critical_update *)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SET_ML_CRITICAL_UPDATE, &clpb, ml_critical_update,
    sizeof(struct mxl_vendor_ml_critical_update));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SET_ML_CRITICAL_UPDATE, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_set_scs_enable(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCS_ENABLE);
}

static int _wave_ieee80211_vendor_get_scs_enable(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCS_ENABLE, 1);
}

static int _wave_ieee80211_vendor_scs_add_req(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_scs_add_req *scs_add_req;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_scs_add_req));

  scs_add_req = (struct mxl_scs_add_req*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),WAVE_CORE_REQ_SCS_ADD, &clpb, scs_add_req, sizeof(struct mxl_scs_add_req));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SCS_ADD, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}
static int _wave_ieee80211_vendor_scs_rem_req(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_scs_rem_req *scs_rem_req;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_scs_rem_req));

  scs_rem_req = (struct mxl_scs_rem_req*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),WAVE_CORE_REQ_SCS_REM, &clpb, scs_rem_req, sizeof(struct mxl_scs_rem_req));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SCS_REM, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_send_t2lm_teardown_frame(struct wiphy *wiphy,
                                                           struct wireless_dev *wdev,
                                                           const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb;
  int res = MTLK_ERR_OK;
  struct mxl_ml_sta_t2lm_teardown *ml_sta_t2lm_teardown;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_ml_sta_t2lm_teardown));

  ml_sta_t2lm_teardown = (struct mxl_ml_sta_t2lm_teardown*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_ML_TX_T2LM_TEARDOWN, &clpb,
    ml_sta_t2lm_teardown, sizeof(struct mxl_ml_sta_t2lm_teardown));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_ML_TX_T2LM_TEARDOWN, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_ml_sta_reassoc_notify(struct wiphy *wiphy,
                                                        struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb;
  int res;
  struct mxl_ml_sta_reassoc_notify *reassoc_notify;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_ml_sta_reassoc_notify));

  reassoc_notify = (struct mxl_ml_sta_reassoc_notify*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
                                  WAVE_CORE_REQ_ML_STA_REASSOC_NOTIFY, &clpb,
                                  reassoc_notify, sizeof(struct mxl_ml_sta_reassoc_notify));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_ML_STA_REASSOC_NOTIFY, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_set_mscs_enable(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MSCS_ENABLE);
}

static int _wave_ieee80211_vendor_get_mscs_enable(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MSCS_ENABLE, 1);
}

static int _wave_ieee80211_vendor_mscs_add_req(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct mxl_mscs_add_req *mscs_add_req;

  ILOG0_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_mscs_add_req));

  mscs_add_req = (struct mxl_mscs_add_req*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),WAVE_CORE_REQ_MSCS_ADD, &clpb, mscs_add_req, sizeof(struct mxl_mscs_add_req));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_MSCS_ADD, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_mscs_rem_req(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  uint16 aid;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint16));

  aid = *(uint16*)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),WAVE_CORE_REQ_MSCS_REM, &clpb, &aid, sizeof(uint16));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_MSCS_REM, TRUE);
  return _mtlk_df_mtlk_to_linux_error_code(res);
}
#endif

static int _wave_ieee80211_vendor_set_FixedRateCfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_RATE);
}

static int _wave_ieee80211_vendor_get_su_mu_ru_stats(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(wave_vendor_su_mu_ru_stats_t));
  return wave_radio_get_su_mu_ru_stats(wiphy, wdev->netdev, (wave_vendor_su_mu_ru_stats_t *)data);
}

static int _wave_ieee80211_vendor_get_csi_capability(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return wave_radio_get_csi_capability(wiphy, wdev->netdev);
}

static int _wave_ieee80211_vendor_set_DoDebugAssert(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  #define MAX_LMIPS_NUMBER    (MTLK_CORE_UI_ASSERT_TYPE_FW_LMIPS_MAX - MTLK_CORE_UI_ASSERT_TYPE_FW_LMIPS)

  uint32 *int_data = (uint32*)data;
  mtlk_df_user_t *df_user;
  mtlk_df_t *master_df;
  int res = MTLK_ERR_PARAMS;
  int arg_assert_type = MTLK_CORE_UI_ASSERT_TYPE_NONE;
  int hw_assert_type;
  int mips_no = -1;
  unsigned nr_1, nr_2 = 0;
  char *mac_name = "UpperMAC";
  mtlk_vap_handle_t vap_handle = NULL;
  mtlk_hw_t *hw = NULL;
  mtlk_clpb_t *clpb = NULL;

  mtlk_core_ui_dbg_assert_type_e    arg1_to_assert_type_table[] = {
                      MTLK_CORE_UI_ASSERT_TYPE_NONE,      /* 0 */
                      MTLK_CORE_UI_ASSERT_TYPE_FW_UMIPS,  /* 1 */
                      MTLK_CORE_UI_ASSERT_TYPE_FW_LMIPS,  /* 2 X */
                      MTLK_CORE_UI_ASSERT_TYPE_ALL_MACS,  /* 3 */
                      MTLK_CORE_UI_ASSERT_TYPE_DRV_DIV0,  /* 4 */
                      MTLK_CORE_UI_ASSERT_TYPE_DRV_BLOOP  /* 5 */ };

  df_user = mtlk_df_user_from_wdev(wdev);
  if (df_user == NULL){
    res = MTLK_ERR_NOT_READY;
    goto end;
  }

  master_df = mtlk_df_user_get_master_df(df_user);
  MTLK_CHECK_DF(master_df);
  vap_handle = mtlk_df_get_vap_handle(master_df);
  hw = mtlk_vap_get_hw(vap_handle);

  if (data_len != sizeof(uint32) && data_len != 2*sizeof(uint32)) {
        ELOG_DD("CID-%04x: Wrong number of parameters: %d", mtlk_vap_get_oid(vap_handle), data_len);
    goto end;
  }
  nr_1 = *int_data;
  if (data_len == 2*sizeof(uint32))
    nr_2 = *(int_data + 1);

  if (nr_1 < MTLK_ARRAY_SIZE(arg1_to_assert_type_table)) {
    arg_assert_type = arg1_to_assert_type_table[nr_1];
  }

  hw_assert_type = arg_assert_type;

  switch (arg_assert_type)
  {
  case MTLK_CORE_UI_ASSERT_TYPE_NONE:
    ELOG_DD("CID-%04x: Unsupported assert type: %u", mtlk_vap_get_oid(vap_handle), nr_1);
    goto end;
  case MTLK_CORE_UI_ASSERT_TYPE_ALL_MACS:
    ILOG0_D("CID-%04x: All CPUs assertion without any events confirmation", mtlk_vap_get_oid(vap_handle));
    break;
  case MTLK_CORE_UI_ASSERT_TYPE_FW_LMIPS:
    if (nr_2 > MAX_LMIPS_NUMBER) {
      ELOG_DD("CID-%04x: Illegal LowerMAC number: %u", mtlk_vap_get_oid(vap_handle), nr_2);
      goto end;
    }
    mac_name = "LowerMACx";
    fallthrough;
  case MTLK_CORE_UI_ASSERT_TYPE_FW_UMIPS:
    hw_assert_type += nr_2;
    mips_no = hw_assert_type_to_core_nr(hw, hw_assert_type);
    if (mips_no < 0) {
      ELOG_DDD("CID-%04x: Can't get MAC number from (%u, %u)", mtlk_vap_get_oid(vap_handle), nr_1, nr_2);
      goto end;
    }
    /* Check whether the MAC has already been asserted. If so - doesn't call the Core */
    res = wave_rcvry_mac_hang_evt_is_set(hw, mips_no);
    if (res == MTLK_ERR_OK) {
      ILOG0_DS("CID-%04x: %s already asserted", mtlk_vap_get_oid(vap_handle), mac_name);
      goto end;
    }
    break;
  default:
    break;
  }

  res = _mtlk_df_user_invoke_core(master_df, WAVE_RADIO_REQ_SET_MAC_ASSERT, &clpb,
                                   &hw_assert_type, sizeof(hw_assert_type));
  _mtlk_df_user_process_core_retval_void(res, clpb, WAVE_RADIO_REQ_SET_MAC_ASSERT, TRUE);

  if (res == MTLK_ERR_NOT_READY) {
    ILOG0_DS("CID-%04x: %s already asserted", mtlk_vap_get_oid(vap_handle), mac_name);
    goto end;
  } else if (res != MTLK_ERR_OK) { /* invocation error */
    WLOG_DSD("CID-%04x: %s assert invocation failed (res=%d)", mtlk_vap_get_oid(vap_handle), mac_name, res);
    goto end;
  }

  switch (arg_assert_type)
  {
  case MTLK_CORE_UI_ASSERT_TYPE_ALL_MACS:
    /* All CPUs assertion without any events confirmation */
    /* res = MTLK_ERR_OK; */
    break;
  case MTLK_CORE_UI_ASSERT_TYPE_FW_LMIPS:
  case MTLK_CORE_UI_ASSERT_TYPE_FW_UMIPS:
    /* Wait for MAC to detect ASSERTION */
    res = wave_rcvry_mac_hang_evt_wait(hw, mips_no);
    if (res != MTLK_ERR_OK) {
      WLOG_DSD("CID-%04x: %s assertion failed (res=%d)", mtlk_vap_get_oid(vap_handle), mac_name, res);
    } else {
      ILOG0_DS("CID-%04x: %s asserted", mtlk_vap_get_oid(vap_handle), mac_name);
    }
    break;
  default:
    break;
  }

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);

  #undef MAX_LMIPS_NUMBER
}

/*! \brief      Enable 3Address Multicast handling
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: enable/disable flag, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_allow_3addr_mcast (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                         const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_SIZE(data_len, sizeof(int));

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_ALLOW_3ADDR_MCAST);
}

/*! \brief      Get 3Address Multicast Flag
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_allow_3addr_mcast (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_ALLOW_3ADDR_MCAST, 1);
}

static int _wave_ieee80211_vendor_set_mu_groups_config (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MU_GROUP_CONFIG);
}
static int _wave_ieee80211_vendor_get_mu_groups_config (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint32));
  return wave_radio_get_mu_group_cfg(wiphy, wdev->netdev, (uint32 *)data);
}


#ifdef MTLK_WAVE_700
/*! \brief      Setup MLD
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: _mxl_vendor_mld_info
 *  \param[in]  data_len  Data length: size of _mxl_vendor_mld_info
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_setup_mld (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct _mxl_vendor_mld_info *ml_info = (struct _mxl_vendor_mld_info *)data;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct _mxl_vendor_mld_info));

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  /* Validate MAC address */
  if (!mtlk_osal_is_valid_ether_addr(ml_info->ap_mld_mac)) {
    ELOG_SY("%s: The AP MLD MAC %Y is invalid", wdev->netdev->name, ml_info->ap_mld_mac);
    res = MTLK_ERR_PARAMS;
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SETUP_MLD, &clpb, ml_info, sizeof(struct _mxl_vendor_mld_info));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SETUP_MLD, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Setup MLD
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: _mxl_vendor_mld_link_info
 *  \param[in]  data_len  Data length: size of _mxl_vendor_mld_link_info
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_mld_info (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                     const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  int res = MTLK_ERR_OK;
  struct _mxl_vendor_mld_link_info *ml_link_info = (struct _mxl_vendor_mld_link_info *)data;

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct _mxl_vendor_mld_link_info));

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  /* Validate MAC address */
  if (!mtlk_osal_is_valid_ether_addr(ml_link_info->ap_mld_mac)) {
    ELOG_SY("%s: The AP MLD MAC %Y is invalid", wdev->netdev->name, ml_link_info->ap_mld_mac);
    res = MTLK_ERR_PARAMS;
    return _mtlk_df_mtlk_to_linux_error_code(res);
  }
  /* store MLD */
  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
    WAVE_CORE_REQ_SET_MLD_INFO, &clpb, ml_link_info, sizeof(struct _mxl_vendor_mld_link_info));
  res = _mtlk_df_user_process_core_retval(res, clpb,
    WAVE_CORE_REQ_SET_MLD_INFO, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get link statistics for ML STA
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ML_link_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb;
  mtlk_error_t res;
  struct ml_link_stats *stats;
  struct net_device *ndev = wdev->netdev;
  uint32 mlstats_size;
  uint8 *ml_aid = (uint8 *)data;

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  if (!ml_aid) {
     ELOG_V("Invalid ML AID");
     res = MTLK_ERR_PARAMS;
     goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
                                  WAVE_CORE_REQ_GET_ML_STA_LINK_STATS, &clpb, ml_aid, sizeof(uint8));
  res = _mtlk_df_user_process_core_retval(res, clpb,
                                          WAVE_CORE_REQ_GET_ML_STA_LINK_STATS, FALSE);
  if (res != MTLK_ERR_OK)
      goto end;

  stats = mtlk_clpb_enum_get_next(clpb, &mlstats_size);
  MTLK_CLPB_TRY(stats, mlstats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, stats, sizeof(*stats));
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
    return res;

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_set_ML_link_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                               const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb;
  mtlk_error_t res;
  struct net_device *ndev = wdev->netdev;
  uint8 *ml_aid = (uint8 *)data;

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  if (!ml_aid) {
     ELOG_V("Invalid ML AID");
     res = MTLK_ERR_PARAMS;
     goto end;
  }

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
                                  WAVE_CORE_REQ_SET_ML_STA_LINK_STATS, &clpb, ml_aid, sizeof(uint8));
  _mtlk_df_user_process_core_retval_void(res, clpb, WAVE_CORE_REQ_SET_ML_STA_LINK_STATS, TRUE);

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get ML VAP list in all the radios
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Method is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_ML_vap_list (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb;
  mtlk_error_t res;
  struct ml_vap_list *ml_vap_list;
  struct net_device *ndev = wdev->netdev;
  uint32 mlvaplist_size, *ml_vap_count;

  ILOG2_SSD("%s: Invoked from %s (%i)", ndev->name, current->comm, current->pid);
  df_user = mtlk_df_user_from_ndev(ndev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
                                  WAVE_CORE_REQ_GET_ML_VAP_LIST, &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb,
                                          WAVE_CORE_REQ_GET_ML_VAP_LIST, FALSE);
  if (res != MTLK_ERR_OK)
      goto end;

  ml_vap_count = mtlk_clpb_enum_get_next(clpb, &mlvaplist_size);
  if ((sizeof(uint32) != mlvaplist_size) || (ml_vap_count == NULL)) {
     mtlk_clpb_delete(clpb);
     res = MTLK_ERR_UNKNOWN;
     goto end;
  }
  if (*ml_vap_count == 0) {
    mtlk_clpb_delete(clpb);
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }
  ml_vap_list = mtlk_clpb_enum_get_next(clpb, &mlvaplist_size);
  MTLK_CLPB_TRY_EX(ml_vap_list, mlvaplist_size, (*ml_vap_count * sizeof(struct ml_vap_list)))
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, ml_vap_list, mlvaplist_size);
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END
    return res;

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

#ifdef BEST_EFFORT_TID_SPREADING
static int _wave_ieee80211_vendor_set_str_tid_link_spreading (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                            const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_STR_TID_SPREADING);
}
#endif

static int
_wave_ieee80211_vendor_get_ml_peer_flow_status (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));

  return wave_core_get_ml_peer_flow_status(wiphy, wdev->netdev, (uint8 *)data);
}

static int
_wave_ieee80211_vendor_get_ML_sta_list (struct wiphy *wiphy, struct wireless_dev *wdev,
                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return wave_core_get_connected_ml_sta_list(wiphy, wdev->netdev);
}
#endif

int _wave_ieee80211_vendor_get_UnconnTime(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_UNCONNECTED_STA_SCAN_TIME, 1);
}

static int _wave_ieee80211_vendor_set_UnconnTime(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_UNCONNECTED_STA_SCAN_TIME);
}

extern void wv_print_regd_rules (const struct ieee80211_regdomain *regd);
extern const struct ieee80211_regdomain *get_cfg80211_regdom (void );

/*! \brief      afcd update regulatory info for standard-power AP.
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: Regulatory info from AFC Server, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_update_afc_info (struct wiphy *wiphy,
                                       struct wireless_dev *wdev,
                                       const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_df_t *df;
  mtlk_core_t *core;
  mtlk_clpb_t *clpb = NULL;
  wave_radio_reg_data_t mxl_wave_reg_data;
  struct ieee80211_regdomain *regd = (struct ieee80211_regdomain *) data;
  struct ieee80211_regdomain *nregd;
  mtlk_hw_band_e band;

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  df = mtlk_df_user_get_df(df_user);
  core = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
  MTLK_ASSERT(NULL != core);

  band = wave_radio_band_get(wave_vap_radio_get(core->vap_handle));

  if (band != MTLK_HW_BAND_6_GHZ)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NOT_SUPPORTED);

  /* When afcd is unable to obtain a valid response from the server, it sets n_reg_rules to zero upon updating */
  if (!regd->n_reg_rules) {
    /* Follow cfg regulatory rule and prevent channels from operating
     * in case regd from afcd is unavailable */
    nregd = wave_copy_regd(get_cfg80211_regdom());
    wv_set_regd_flag(nregd, NL80211_RRF_NO_IR);
    mxl_wave_reg_data.flags = AFC_UPDATE_STATUS_DEFAULT;
  } else {
    nregd = wave_copy_regd(regd);
    /*
     * AFC server may report all channel in 20MHz bandwidth, set
     * NL80211_RRF_AUTO_BW flag to let regd to find the max possible
     * bandwidth on it's own for each channel.
     */
    wv_set_regd_flag(nregd, NL80211_RRF_AUTO_BW);
    mxl_wave_reg_data.flags = AFC_UPDATE_STATUS_SERVER;
  }

  if (!nregd)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_MEM);

  mxl_wave_reg_data.regd = nregd;

  wv_print_regd_rules(mxl_wave_reg_data.regd);
  _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user),
                            WAVE_RADIO_REQ_UPDATE_AFC_REG_INFO, &clpb, &mxl_wave_reg_data,
                            sizeof(wave_radio_reg_data_t));

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Hostapd update 6GHz interface operating power mode
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: LOW-POWER-INDOOR/STANDARD-POWER, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(int)
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_reg_update_6ghz_oper_power_mode (struct wiphy *wiphy,
                                                                  struct wireless_dev *wdev,
                                                                  const void *data, int data_len)
{
  mtlk_df_user_t *df_user;
  mtlk_core_t *core;
  mtlk_df_t *df;
  mtlk_hw_band_e band;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  if (!wdev)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  df = mtlk_df_user_get_df(df_user);
  core = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
  MTLK_ASSERT(NULL != core);
  band = wave_radio_band_get(wave_vap_radio_get(core->vap_handle));

  if (band != MTLK_HW_BAND_6_GHZ)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NOT_SUPPORTED);

  return wv_ieee80211_6ghz_update_wiphy_power_mode_and_wiphy_regd(wiphy, wdev, (uint8 *)data);
}

static int _wave_ieee80211_vendor_set_FixedRateThermal (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_RATE_THERMAL);
}

static int _wave_ieee80211_vendor_get_FixedRateThermal (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_RATE_THERMAL, 3);
}

static int _wave_ieee80211_vendor_set_FixedPower(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_POWER);
}

int _wave_ieee80211_vendor_get_FixedPower(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_FIXED_POWER, 4);
}


/************************* DEBUG SET/GET FUNCTIONS *************************/
#ifdef CONFIG_WAVE_DEBUG

static int _wave_ieee80211_vendor_set_CountersSrc(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SWITCH_COUNTERS_SRC);
}

static int _wave_ieee80211_vendor_get_CountersSrc(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SWITCH_COUNTERS_SRC, 1);
}

static int _wave_ieee80211_vendor_set_CpuDmaLatency(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_SET_PM_QOS
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_CPU_DMA_LATENCY);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_get_CpuDmaLatency(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_SET_PM_QOS
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_CPU_DMA_LATENCY, 1);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_get_BfExplicitCap(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_BEAMFORM_EXPLICIT, 1);
}

static int _wave_ieee80211_vendor_set_TaskletLimits(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TASKLET_LIMITS);
}

static int _wave_ieee80211_vendor_get_TaskletLimits(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TASKLET_LIMITS, 5);
}

static int _wave_ieee80211_vendor_get_GenlFamilyId(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_GENL_FAMILY_ID, 1);
}

static int _wave_ieee80211_vendor_set_ScanExpTime(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_EXP_TIME);
}

static int _wave_ieee80211_vendor_get_ScanExpTime(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_SCAN_EXP_TIME, 1);
}

static int _wave_ieee80211_vendor_get_TADbg(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_text_params(wiphy, wdev, data, data_len, PRM_ID_TA_DBG);
}

static int _wave_ieee80211_vendor_set_TATimerRes(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TA_TIMER_RESOLUTION);
}

static int _wave_ieee80211_vendor_get_TATimerRes(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_TA_TIMER_RESOLUTION, 1);
}

static int _wave_ieee80211_vendor_set_PCoCAutoCfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PCOC_AUTO_PARAMS);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_get_PCoCAutoCfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PCOC_AUTO_PARAMS, 4);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_set_PCoCPower(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PCOC_POWER_MODE);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_get_PCoCPower(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PCOC_POWER_MODE, 3);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_set_PMCUDebug(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PCOC_PMCU_DEBUG);
#else
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_NO_ENTRY);
#endif
}

static int _wave_ieee80211_vendor_set_WDSHostTO(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_WDS_HOST_TIMEOUT);
}

static int _wave_ieee80211_vendor_get_WDSHostTO(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_WDS_HOST_TIMEOUT, 1);
}

static int _wave_ieee80211_vendor_set_MACWdPeriodMs(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MAC_WATCHDOG_PERIOD_MS);
}

static int _wave_ieee80211_vendor_get_MACWdPeriodMs(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MAC_WATCHDOG_PERIOD_MS, 1);
}

static int _wave_ieee80211_vendor_set_MACWdTimeoutMs(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MAC_WATCHDOG_TIMEOUT_MS);
}

static int _wave_ieee80211_vendor_get_MACWdTimeoutMs(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_MAC_WATCHDOG_TIMEOUT_MS, 1);
}

static int _wave_ieee80211_vendor_set_NonOccupatePrd(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_NOP);
}

static int _wave_ieee80211_vendor_get_NonOccupatePrd(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_NOP, 1);
}

static int _wave_ieee80211_vendor_set_11hBeaconCount(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_BEACON_COUNT);
}

static int _wave_ieee80211_vendor_get_11hBeaconCount(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_11H_BEACON_COUNT, 1);
}

static int _wave_ieee80211_vendor_set_enable_test_bus(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_TEST_BUS);
}

static int _wave_ieee80211_vendor_set_DoFwDebug(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FW_DEBUG);
}

static int _wave_ieee80211_vendor_set_DoSimpleCLI(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DBG_CLI);
}

static int _wave_ieee80211_vendor_set_FwLogSeverity(struct wiphy *wiphy, struct wireless_dev *wdev,
                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_FW_LOG_SEVERITY);
}

static int _wave_cfg80211_vendor_set_dbgCmdFw(struct wiphy *wiphy, struct wireless_dev *wdev,
  const void *data, int data_len)
{
  ILOG0_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DBG_CMD_FW);
}

static int _wave_cfg80211_vendor_set_radarFifoDump(struct wiphy *wiphy, struct wireless_dev *wdev,
  const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  ILOG0_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  if (wave_radio_band_get(radio) == MTLK_HW_BAND_5_2_GHZ) {
    return set_int_params(wiphy, wdev, data, data_len, PRM_ID_RADAR_FIFO_DUMP);
  }

  ILOG0_V("Enabling of radar fifo dump is only supported by 5G band");
  return MTLK_ERR_PARAMS;
}

static int _wave_cfg80211_vendor_set_vw_test_mode(struct wiphy *wiphy, struct wireless_dev *wdev,
  const void *data, int data_len)
{
  return set_int_params(wiphy, wdev, data, sizeof(data), PRM_ID_VW_TEST_MODE);
}

static int _wave_cfg80211_vendor_get_vw_test_mode (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_VW_TEST_MODE, 1);
}

static int _wave_cfg80211_vendor_get_peer_host_if_qos(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_host_if_qos(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_host_if(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_host_if(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_rx_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_rx_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_ul_bsrc_tid_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_ul_bsrc_tid_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_baa_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_baa_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_link_adaption_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_link_adaption_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_plan_manager_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_plan_manager_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_twt_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_twt_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_per_client_stats(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_per_client_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_phy_rx_status(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_phy_rx_status(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_peer_info(struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_peer_info(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_wlan_host_if_qos (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_wlan_host_if_qos(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_wlan_host_if (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_wlan_host_if(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_wlan_rx_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_wlan_rx_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_wlan_baa_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_wlan_baa_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_radio_rx_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_radio_rx_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_radio_baa_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_radio_baa_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_tsman_init_tid_gl (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_tsman_init_tid_gl(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_tsman_init_sta_gl (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_tsman_init_sta_gl(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_tsman_rcpt_tid_gl (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_tsman_rcpt_tid_gl(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_tsman_rcpt_sta_gl (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_tsman_rcpt_sta_gl(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_radio_link_adapt_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_radio_link_adapt_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_multicast_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_multicast_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_training_man_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_training_man_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_group_man_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_group_man_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_general_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_general_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_cur_channel_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_cur_channel_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_radio_phy_rx_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_radio_phy_rx_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_dynamic_bw_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_dynamic_bw_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_la_mu_vht_stats (struct wiphy *wiphy,
  struct wireless_dev *wdev, const void *data, int data_len)
{
  struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
  wave_radio_t *radio = wv_ieee80211_hw_radio_get(hw);
  return wave_radio_get_la_mu_vht_stats(radio, wdev->netdev, data, data_len);
}

static int _wave_cfg80211_vendor_get_la_mu_he_eht_stats (struct wiphy *wiphy,
                                                         struct wireless_dev *wdev,
                                                         const void *data, int data_len)
{
  uint32 stats_size;
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  mtlk_wssa_drv_la_mu_he_eht_input_t *input;
  mtlk_wssa_drv_la_mu_he_eht_stats_t *stats;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(*input));

  input = (mtlk_wssa_drv_la_mu_he_eht_input_t *)data;
  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_LA_MU_HE_EHT_STATS, &clpb, input, sizeof(*input));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_LA_MU_HE_EHT_STATS, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  if (input->reset_stats != LA_MU_HE_EHT_NO_RESET) {
    mtlk_clpb_delete(clpb);
    goto end;
  }

  stats = mtlk_clpb_enum_get_next(clpb, &stats_size);
  MTLK_CLPB_TRY(stats, stats_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, stats, sizeof(*stats));
    mtlk_clpb_delete(clpb);
    return res; /* Linux error code */
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_cfg80211_vendor_set_dbg_unprotected_deauth(struct wiphy *wiphy, struct wireless_dev *wdev,
  const void *data, int data_len)
{
  mtlk_df_user_t *df_user = NULL;
  mtlk_df_t *df = NULL;
  mtlk_core_t *core = NULL;
  mtlk_hw_band_e band;
  int *dbg_unprotected_deauth = (int *)data;

  ILOG0_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  df = mtlk_df_user_get_df(df_user);


  core = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
  MTLK_ASSERT(NULL != core);
  band = wave_radio_band_get(wave_vap_radio_get(core->vap_handle));

  if (dbg_unprotected_deauth && (data_len == sizeof(int)) && (band == MTLK_HW_BAND_6_GHZ)) {

      ILOG0_D("setting dbg_unprotected_deauth %d",*dbg_unprotected_deauth);
      wave_hw_mmb_set_dbg_unprotected_deauth(df, *dbg_unprotected_deauth);
  }

  return MTLK_ERR_OK;

}

static int _wave_cfg80211_vendor_set_dbg_pn_reset(struct wiphy *wiphy, struct wireless_dev *wdev,
  const void *data, int data_len)
{
  mtlk_df_user_t *df_user = NULL;
  mtlk_df_t *df = NULL;
  mtlk_core_t *core = NULL;
  mtlk_hw_band_e band;
  int *dbg_pn_reset = (int *)data;

  ILOG0_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  df = mtlk_df_user_get_df(df_user);
  core = mtlk_vap_get_core(mtlk_df_get_vap_handle(df));
  MTLK_ASSERT(NULL != core);
  band = wave_radio_band_get(wave_vap_radio_get(core->vap_handle));

  if (dbg_pn_reset && (data_len == sizeof(int)) && (band == MTLK_HW_BAND_6_GHZ)) {
      ILOG0_D("setting dbg_pn_reset %d",*dbg_pn_reset);
      wave_hw_mmb_set_dbg_pn_reset(df, *dbg_pn_reset);
  }

  return MTLK_ERR_OK;
}

static int _wave_ieee80211_vendor_set_MtlkLogLevel (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSDDD("%s: Invoked from %s (%i) data_len=[%d] data=[%d]", wdev->netdev->name, current->comm, current->pid, data_len, *(uint16 *)data);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_MTLK_DBG_LVL);
}

#endif /* CONFIG_WAVE_DEBUG */

/*! \brief      Starts a CCA (Clear Channel Assessment) scan for a given interface and the given channel using a specific dwell time.
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_set_start_cca_msr_off_chan (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_cca_msr_cfg));
  return wave_radio_cca_msr_start(wiphy, wdev->netdev, (struct intel_vendor_cca_msr_cfg *)data);
}

/*! \brief      Reports the CCA (Clear Channel Assessment) for a given interface and the requested channel.
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length
 *
 *  \note       CCA measurements are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_cca_msr_off_chan (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(uint32));
  return wave_radio_cca_msr_get(wiphy, wdev->netdev, (uint32 *)data);
}

/*! \brief      Get the CCA (Clear Channel Assessment) for a given interface and the current channel.
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       CCA measurements are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_cca_stats_current_chan (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  return wave_radio_cca_stats_get(wiphy, wdev->netdev);
}

/*! \brief      Get the Radio statistics for the current channel.
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       Radio statistics is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_ieee80211_vendor_get_radio_usage_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                             const void *data, int data_len)
{
  return wave_radio_usage_stats_get(wiphy, wdev->netdev);
}


static int _wave_ieee80211_vendor_get_ap_exce_retry_limit (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_AP_EXCE_RETRY_LIMIT, 1);
}

#ifdef MTLK_WAVE_700
/*! \brief      Set EHT Debug information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct wave_eht_debug_mode_data)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_set_eht_debug_data (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct wave_eht_debug_mode_data eht_data = {0};

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_vendor_eht_capa));

  /* make sure we can memcpy the vendor data to struct wave_eht_debug_mode_data */
  MTLK_STATIC_ASSERT(sizeof(struct mxl_vendor_eht_capa) ==
                     MTLK_OFFSET_OF(struct wave_eht_debug_mode_data, eht_debug_mode_enabled));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct mxl_vendor_eht_capa,    eht_mac_capab_info) ==
                     MTLK_OFFSET_OF(struct wave_eht_debug_mode_data, eht_cap_elem.fixed.mac_cap_info));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct mxl_vendor_eht_capa,    eht_phy_capab_info) ==
                     MTLK_OFFSET_OF(struct wave_eht_debug_mode_data, eht_cap_elem.fixed.phy_cap_info));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct mxl_vendor_eht_capa,    eht_txrx_mcs_support) ==
                     MTLK_OFFSET_OF(struct wave_eht_debug_mode_data, eht_mcs_nss_supp));
  MTLK_STATIC_ASSERT(MTLK_OFFSET_OF(struct mxl_vendor_eht_capa,    eht_ppe_thresholds) ==
                     MTLK_OFFSET_OF(struct wave_eht_debug_mode_data, eht_ppe_thresholds));

  eht_data.eht_debug_mode_enabled = 1;
  wave_memcpy(&eht_data, sizeof(eht_data) - sizeof(eht_data.eht_debug_mode_enabled), data, data_len);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_EHT_DEBUG,
                                  &clpb, &eht_data, sizeof(eht_data));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_EHT_DEBUG, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set EHT Operation information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: sizeof(struct mxl_vendor_eht_oper)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_set_eht_operation (struct wiphy *wiphy, struct wireless_dev *wdev,
                                              const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_vendor_eht_oper));

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_EHT_OPERATION,
                                  &clpb, data, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_EHT_OPERATION, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get EHT Operation information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       EHT Operation information is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int _wave_cfg80211_vendor_hapd_get_eht_operation (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                         const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct mxl_vendor_eht_oper *eht_operation;
  uint32 eht_oper_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_EHT_OPERATION,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_EHT_OPERATION, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  eht_operation = mtlk_clpb_enum_get_next(clpb, &eht_oper_size);
  MTLK_CLPB_TRY(eht_operation, eht_oper_size)
    mtlk_dump(3, eht_operation, eht_oper_size, "EHT Operation:");
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, eht_operation, eht_oper_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Set EHT Non-advertised information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: must not be NULL
 *  \param[in]  data_len  Data length: sizeof(struct mxl_vendor_eht_capa)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_set_eht_non_advertised (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct mxl_vendor_eht_capa));

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_SET_EHT_NON_ADVERTISED,
                                  &clpb, data, data_len);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_SET_EHT_NON_ADVERTISED, TRUE);

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

/*! \brief      Get EHT Non-advertised information for specified \a wdev
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: unused
 *  \param[in]  data_len  Data length: unused
 *
 *  \note       EHT Non-advertised information is returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_cfg80211_vendor_hapd_get_eht_non_advertised (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                  const void *data, int data_len)
{
  int res = MTLK_ERR_OK;
  mtlk_df_user_t *df_user;
  mtlk_clpb_t *clpb = NULL;
  struct mxl_vendor_eht_capa *eht_non_advert;
  uint32 eht_non_advert_size;

  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  df_user = mtlk_df_user_from_wdev(wdev);
  MTLK_CHECK_DF_USER(df_user);

  res = _mtlk_df_user_invoke_core(mtlk_df_user_get_df(df_user), WAVE_CORE_REQ_GET_EHT_NON_ADVERTISED,
                                  &clpb, NULL, 0);
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_CORE_REQ_GET_EHT_NON_ADVERTISED, FALSE);

  if (res != MTLK_ERR_OK)
    goto end;

  eht_non_advert = mtlk_clpb_enum_get_next(clpb, &eht_non_advert_size);
  MTLK_CLPB_TRY(eht_non_advert, eht_non_advert_size)
    res = wv_cfg80211_vendor_cmd_alloc_and_reply(wiphy, eht_non_advert, eht_non_advert_size);
    mtlk_clpb_delete(clpb);
    return res;
  MTLK_CLPB_FINALLY(res)
    mtlk_clpb_delete(clpb);
  MTLK_CLPB_END

end:
  return _mtlk_df_mtlk_to_linux_error_code(res);
}

static int _wave_ieee80211_vendor_set_LoggerFifoMuxCfg(struct wiphy *wiphy, struct wireless_dev *wdev,
                                                       const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_LOGGER_FIFO_MUX_CFG);
}

static int _wave_ieee80211_vendor_get_LoggerFifoMuxCfg (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                        const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return get_int_params(wiphy, wdev, data, data_len, PRM_ID_LOGGER_FIFO_MUX_CFG, 1);
}
#endif

/*! \brief      Set SoftBlock Timer ACL parameters
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: SoftBlock Timer ACL parameters, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(struct intel_vendor_sb_timer_acl_cfg)
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_hapd_set_sb_timer_acl_params (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                   const void *data, int data_len)
{
  wave_core_sb_timer_acl_t sb_timer_acl_cfg;
  struct intel_vendor_sb_timer_acl_cfg *pcfg = NULL;

  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(struct intel_vendor_sb_timer_acl_cfg));

  pcfg = (struct intel_vendor_sb_timer_acl_cfg *)data;

  sb_timer_acl_cfg.timer_acl_on = pcfg->timer_acl_on;
  sb_timer_acl_cfg.timer_acl_probe_drop = pcfg->timer_acl_probe_drop;

  return set_int_params(wiphy, wdev, &sb_timer_acl_cfg, sizeof(sb_timer_acl_cfg), PRM_ID_SB_TIMER_ACL);
}

/*! \brief      Get associated devices mixed statistics
 *
 *  \param[in]  wiphy     Pointer to wireless hardware description (wiphy) structure
 *  \param[in]  wdev      Pointer to wireless device state (wireless_dev) structure
 *  \param[in]  data      Pointer to data: MAC address, must not be NULL
 *  \param[in]  data_len  Data length: must be sizeof(IEEE_ADDR)
 *
 *  \note       Statistics are returned via cfg80211_vendor_cmd_reply()
 *
 *  \return     Linux error code
 *
 */
static int
_wave_ieee80211_vendor_get_peer_mixed_stats (struct wiphy *wiphy, struct wireless_dev *wdev,
                                            const void *data, int data_len)
{
  WAVE_CHECK_VENDOR_DATA_AND_SIZE(data, data_len, sizeof(IEEE_ADDR));
  return wave_radio_get_peer_mixed_stats(wiphy, wdev->netdev, (uint8 *)data);
}

static int
_wave_cfg80211_vendor_enable_dynamic_wmm (struct wiphy *wiphy, struct wireless_dev *wdev,
                                          const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DYNAMIC_WMM);
}

static int
_wave_ieee80211_vendor_set_dynamic_edca (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DYNAMIC_EDCA);
}

static int
_wave_ieee80211_vendor_set_duplicate_beacon (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);

  WAVE_CHECK_VENDOR_SIZE(data_len, sizeof(mtlk_core_dup_beacon_t));
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_DUPLICATE_BEACON);
}

static int
_wave_ieee80211_vendor_set_pcie_auto_gen_enable (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PCIE_SPEED);
}

static int
_wave_ieee80211_vendor_get_pcie_auto_gen_enable (struct wiphy *wiphy, struct wireless_dev *wdev,
                                         const void *data, int data_len)
{
 ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
 return get_int_params(wiphy, wdev, data, data_len, PRM_ID_PCIE_SPEED, 1);

}

static int 
_wave_ieee80211_vendor_set_pbac (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                const void *data, int data_len)
{
  ILOG1_SSD("%s: Invoked from %s (%i)", wdev->netdev->name, current->comm, current->pid);
  return set_int_params(wiphy, wdev, data, data_len, PRM_ID_PBAC);
}

#define WIPHY_VENDOR_CMD_NEED_WDEV_NETDEV_UP (WIPHY_VENDOR_CMD_NEED_WDEV |	\
                                              WIPHY_VENDOR_CMD_NEED_NETDEV |	\
                                              WIPHY_VENDOR_CMD_NEED_RUNNING)
#define VENDOR_CMD_WDEV_UP(cmd, func) {			\
  .info  = {OUI_LTQ, (LTQ_NL80211_VENDOR_SUBCMD_##cmd)},\
  .flags = WIPHY_VENDOR_CMD_NEED_WDEV_NETDEV_UP,	\
  .policy = VENDOR_CMD_RAW_DATA,			\
  .doit  = (func)               			\
}

#define VENDOR_CMD_MXL_WDEV_UP(cmd, func) {             \
  .info  = {OUI_MXL, (MXL_NL80211_VENDOR_SUBCMD_##cmd)},\
  .flags = WIPHY_VENDOR_CMD_NEED_WDEV_NETDEV_UP,	\
  .policy = VENDOR_CMD_RAW_DATA,			\
  .doit  = (func)               			\
}

#define WIPHY_VENDOR_CMD_NEED_WDEV_NETDEV    (WIPHY_VENDOR_CMD_NEED_WDEV |	\
                                              WIPHY_VENDOR_CMD_NEED_NETDEV)
#define VENDOR_CMD_WDEV(cmd, func) {			\
  .info  = {OUI_LTQ, (LTQ_NL80211_VENDOR_SUBCMD_##cmd)},\
  .flags = WIPHY_VENDOR_CMD_NEED_WDEV_NETDEV,		\
  .policy = VENDOR_CMD_RAW_DATA,			\
  .doit  = (func)               			\
}

static struct
wiphy_vendor_command _wave_mac80211_vendor_commands[] = {
  VENDOR_CMD_WDEV_UP(GET_AID, _wave_ieee80211_vendor_hapd_get_aid),
  VENDOR_CMD_WDEV(FREE_AID, _wave_ieee80211_vendor_hapd_free_aid),
  VENDOR_CMD_WDEV_UP(INITIAL_DATA, _wave_ieee80211_vendor_hapd_receive_initial_data),
  VENDOR_CMD_WDEV_UP(SYNC_DONE, _wave_ieee80211_vendor_hapd_sync_done),
  VENDOR_CMD_WDEV_UP(DFS_DEBUG, _wave_ieee80211_vendor_dfs_debug_radar_required_chan),
  VENDOR_CMD_WDEV_UP(SET_DENY_MAC, _wave_ieee80211_vendor_hapd_set_deny_mac),
  VENDOR_CMD_WDEV_UP(STA_STEER, _wave_ieee80211_vendor_hapd_sta_steer),
  VENDOR_CMD_WDEV_UP(GET_STA_MEASUREMENTS, _wave_ieee80211_vendor_hapd_get_sta_measurements),
  VENDOR_CMD_WDEV_UP(GET_UNCONNECTED_STA, _wave_ieee80211_vendor_hapd_get_unconnected_sta),
  VENDOR_CMD_WDEV_UP(GET_VAP_MEASUREMENTS, _wave_ieee80211_vendor_hapd_get_vap_measurements),
  VENDOR_CMD_WDEV_UP(GET_RADIO_INFO, _wave_ieee80211_vendor_hapd_get_radio_info),
  VENDOR_CMD_WDEV_UP(SET_ATF_QUOTAS, _wave_ieee80211_vendor_hapd_set_atf_quotas),
  VENDOR_CMD_WDEV_UP(SET_WDS_WPA_STA, _wave_ieee80211_vendor_hapd_set_wds_wpa_sta),
  VENDOR_CMD_WDEV_UP(SET_BSS_LOAD, _wave_ieee80211_vendor_hapd_set_bss_load),

  VENDOR_CMD_WDEV_UP(SET_MESH_MODE, _wave_ieee80211_vendor_hapd_set_multi_ap_mode),
  VENDOR_CMD_WDEV_UP(SET_SOFTBLOCK_THRESHOLDS, _wave_ieee80211_vendor_hapd_set_softblock_threshold),
  VENDOR_CMD_WDEV_UP(SET_SOFTBLOCK_DISABLE, _wave_ieee80211_vendor_hapd_set_disable_softblock),
  VENDOR_CMD_WDEV(SET_MBSSID_VAP, _wave_ieee80211_vendor_hapd_set_mbssid_vap),
  VENDOR_CMD_WDEV(SET_MBSSID_NUM_VAPS_IN_GROUP, _wave_ieee80211_vendor_hapd_set_num_vaps_in_group),

  VENDOR_CMD_WDEV(SET_RSSI_IGNORE_PROBE_REQ, _wave_ieee80211_vendor_hapd_set_rssi_ignore_probe_request),

  VENDOR_CMD_WDEV(SET_11H_RADAR_DETECT, _wave_ieee80211_vendor_set_11hRadarDetect),
  VENDOR_CMD_WDEV(GET_11H_RADAR_DETECT, _wave_ieee80211_vendor_get_11hRadarDetect),
  VENDOR_CMD_WDEV(SET_11H_CH_CHECK_TIME, _wave_ieee80211_vendor_set_11hChCheckTime),
  VENDOR_CMD_WDEV(GET_11H_CH_CHECK_TIME, _wave_ieee80211_vendor_get_11hChCheckTime),
  VENDOR_CMD_WDEV(SET_11H_EMULAT_RADAR, _wave_ieee80211_vendor_set_11hEmulatRadar),
  VENDOR_CMD_WDEV(EMULATE_INTERFERER, _wave_ieee80211_vendor_emulate_interferer),

  VENDOR_CMD_WDEV(SET_ADD_PEERAP, _wave_ieee80211_vendor_set_AddPeerAP),
  VENDOR_CMD_WDEV(SET_DEL_PEERAP, _wave_ieee80211_vendor_set_DelPeerAP),
  VENDOR_CMD_WDEV(SET_PEERAP_KEY_IDX, _wave_ieee80211_vendor_set_PeerAPkeyIdx),
  VENDOR_CMD_WDEV(GET_PEERAP_KEY_IDX, _wave_ieee80211_vendor_get_PeerAPkeyIdx),
  VENDOR_CMD_WDEV(GET_PEERAP_LIST, _wave_ieee80211_vendor_get_PeerAPs),

  VENDOR_CMD_WDEV(SET_BRIDGE_MODE, _wave_ieee80211_vendor_set_BridgeMode),
  VENDOR_CMD_WDEV(GET_BRIDGE_MODE, _wave_ieee80211_vendor_get_BridgeMode),
  VENDOR_CMD_WDEV(SET_RELIABLE_MULTICAST, _wave_ieee80211_vendor_set_ReliableMcast),
  VENDOR_CMD_WDEV(GET_RELIABLE_MULTICAST, _wave_ieee80211_vendor_get_ReliableMcast),
  VENDOR_CMD_WDEV(SET_AP_FORWARDING, _wave_ieee80211_vendor_set_APforwarding),
  VENDOR_CMD_WDEV(GET_AP_FORWARDING, _wave_ieee80211_vendor_get_APforwarding),

  VENDOR_CMD_WDEV(GET_EEPROM, _wave_ieee80211_vendor_get_EEPROM),

  VENDOR_CMD_WDEV(SET_DCDP_API_LITEPATH, _wave_ieee80211_vendor_set_LtPathEnabled),
  VENDOR_CMD_WDEV(GET_DCDP_API_LITEPATH, _wave_ieee80211_vendor_get_LtPathEnabled),
  VENDOR_CMD_WDEV(SET_DCDP_API_LITEPATH_COMP, _wave_ieee80211_vendor_set_IpxPpaEnabled),
  VENDOR_CMD_WDEV(GET_DCDP_API_LITEPATH_COMP, _wave_ieee80211_vendor_get_IpxPpaEnabled),

  VENDOR_CMD_WDEV(SET_COC_POWER_MODE,  _wave_ieee80211_vendor_set_CoCPower),
  VENDOR_CMD_WDEV(GET_COC_POWER_MODE,  _wave_ieee80211_vendor_get_CoCPower),
  VENDOR_CMD_WDEV(SET_COC_AUTO_PARAMS, _wave_ieee80211_vendor_set_CoCAutoCfg),
  VENDOR_CMD_WDEV(GET_COC_AUTO_PARAMS, _wave_ieee80211_vendor_get_CoCAutoCfg),

  VENDOR_CMD_WDEV(SET_PRM_ID_TPC_LOOP_TYPE, _wave_ieee80211_vendor_set_TpcLoopType),
  VENDOR_CMD_WDEV(GET_PRM_ID_TPC_LOOP_TYPE, _wave_ieee80211_vendor_get_TpcLoopType),

  VENDOR_CMD_WDEV(SET_INTERFER_THRESH, _wave_ieee80211_vendor_set_InterfDetThresh),
  VENDOR_CMD_WDEV(GET_INTERFER_MODE,   _wave_ieee80211_vendor_get_InterfDetThresh),

  VENDOR_CMD_WDEV(GET_AP_CAPABILITIES_MAX_STAs, _wave_ieee80211_vendor_get_APCapsMaxSTAs),
  VENDOR_CMD_WDEV(GET_AP_CAPABILITIES_MAX_VAPs, _wave_ieee80211_vendor_get_APCapsMaxVAPs),

  VENDOR_CMD_WDEV(SET_11B_ANTENNA_SELECTION, _wave_ieee80211_vendor_set_11bAntSelection),
  VENDOR_CMD_WDEV(GET_11B_ANTENNA_SELECTION, _wave_ieee80211_vendor_get_11bAntSelection),

  VENDOR_CMD_WDEV(SET_FW_RECOVERY, _wave_ieee80211_vendor_set_FWRecovery),
  VENDOR_CMD_WDEV(GET_FW_RECOVERY, _wave_ieee80211_vendor_get_FWRecovery),
  VENDOR_CMD_WDEV(GET_RCVRY_STATS, _wave_ieee80211_vendor_get_RcvryStats),

  VENDOR_CMD_WDEV(SET_SCAN_PARAMS,    _wave_ieee80211_vendor_set_ScanParams),
  VENDOR_CMD_WDEV(GET_SCAN_PARAMS,    _wave_ieee80211_vendor_get_ScanParams),
  VENDOR_CMD_WDEV(SET_SCAN_PARAMS_BG, _wave_ieee80211_vendor_set_ScanParamsBG),
  VENDOR_CMD_WDEV(GET_SCAN_PARAMS_BG, _wave_ieee80211_vendor_get_ScanParamsBG),

  VENDOR_CMD_WDEV(SET_OUT_OF_SCAN_CACHING, _wave_ieee80211_vendor_set_OutOfScanCaching),
  VENDOR_CMD_WDEV(GET_OUT_OF_SCAN_CACHING, _wave_ieee80211_vendor_get_OutOfScanCaching),

  VENDOR_CMD_WDEV(SET_ALLOW_SCAN_DURING_CAC, _wave_ieee80211_vendor_set_AllowScanDuringCac),
  VENDOR_CMD_WDEV(GET_ALLOW_SCAN_DURING_CAC, _wave_ieee80211_vendor_get_AllowScanDuringCac),

  VENDOR_CMD_WDEV(SET_RADIO_MODE, _wave_ieee80211_vendor_set_EnableRadio),
  VENDOR_CMD_WDEV(GET_RADIO_MODE, _wave_ieee80211_vendor_get_EnableRadio),

  VENDOR_CMD_WDEV(SET_AGGR_CONFIG, _wave_ieee80211_vendor_set_AggrConfig),
  VENDOR_CMD_WDEV(GET_AGGR_CONFIG, _wave_ieee80211_vendor_get_AggrConfig),

  VENDOR_CMD_WDEV(SET_AMSDU_NUM, _wave_ieee80211_vendor_set_NumMsduInAmsdu),
  VENDOR_CMD_WDEV(GET_AMSDU_NUM, _wave_ieee80211_vendor_get_NumMsduInAmsdu),

  VENDOR_CMD_WDEV(SET_AGG_RATE_LIMIT, _wave_ieee80211_vendor_set_AggRateLimit),
  VENDOR_CMD_WDEV(GET_AGG_RATE_LIMIT, _wave_ieee80211_vendor_get_AggRateLimit),

  VENDOR_CMD_WDEV(SET_ADMISSION_CAPACITY, _wave_ieee80211_vendor_set_AvailAdmCap),
  VENDOR_CMD_WDEV(GET_ADMISSION_CAPACITY, _wave_ieee80211_vendor_get_AvailAdmCap),

  VENDOR_CMD_WDEV(SET_RX_THRESHOLD, _wave_ieee80211_vendor_set_SetRxTH),
  VENDOR_CMD_WDEV(GET_RX_THRESHOLD, _wave_ieee80211_vendor_get_SetRxTH),

  VENDOR_CMD_WDEV(SET_RX_DUTY_CYCLE, _wave_ieee80211_vendor_set_RxDutyCyc),
  VENDOR_CMD_WDEV(GET_RX_DUTY_CYCLE, _wave_ieee80211_vendor_get_RxDutyCyc),

  VENDOR_CMD_WDEV(SET_TX_POWER_LIMIT_OFFSET, _wave_ieee80211_vendor_set_PowerSelection),
  VENDOR_CMD_WDEV(GET_TX_POWER_LIMIT_OFFSET, _wave_ieee80211_vendor_get_PowerSelection),

  VENDOR_CMD_WDEV(SET_PROTECTION_METHOD, _wave_ieee80211_vendor_set_11nProtection),
  VENDOR_CMD_WDEV(GET_PROTECTION_METHOD, _wave_ieee80211_vendor_get_11nProtection),

  VENDOR_CMD_WDEV_UP(SET_TEMPERATURE_SENSOR, _wave_ieee80211_vendor_set_CalibOnDemand),
  VENDOR_CMD_WDEV_UP(GET_TEMPERATURE_SENSOR, _wave_ieee80211_vendor_get_Temperature),

  VENDOR_CMD_WDEV(SET_QAMPLUS_MODE, _wave_ieee80211_vendor_set_QAMplus),
  VENDOR_CMD_WDEV(GET_QAMPLUS_MODE, _wave_ieee80211_vendor_get_QAMplus),

  VENDOR_CMD_WDEV(SET_ACS_UPDATE_TO, _wave_ieee80211_vendor_set_AcsUpdateTo),
  VENDOR_CMD_WDEV(GET_ACS_UPDATE_TO, _wave_ieee80211_vendor_get_AcsUpdateTo),

  VENDOR_CMD_WDEV(SET_MU_OPERATION, _wave_ieee80211_vendor_set_MuOperation),
  VENDOR_CMD_WDEV(GET_MU_OPERATION, _wave_ieee80211_vendor_get_MuOperation),

  VENDOR_CMD_WDEV(SET_CCA_THRESHOLD, _wave_ieee80211_vendor_set_CcaTh),
  VENDOR_CMD_WDEV(GET_CCA_THRESHOLD, _wave_ieee80211_vendor_get_CcaTh),

  VENDOR_CMD_WDEV(SET_CCA_ADAPT, _wave_ieee80211_vendor_set_CcaAdapt),
  VENDOR_CMD_WDEV(GET_CCA_ADAPT, _wave_ieee80211_vendor_get_CcaAdapt),

  VENDOR_CMD_WDEV(SET_RADAR_RSSI_TH, _wave_ieee80211_vendor_set_RadarRssiTh),
  VENDOR_CMD_WDEV(GET_RADAR_RSSI_TH, _wave_ieee80211_vendor_get_RadarRssiTh),

  VENDOR_CMD_WDEV(SET_FILS_BEACON_FLAG, _wave_ieee80211_vendor_set_FilsBeaconFlag),
  VENDOR_CMD_WDEV(GET_FILS_BEACON_FLAG, _wave_ieee80211_vendor_get_FilsBeaconFlag),

  VENDOR_CMD_WDEV(SET_RTS_MODE, _wave_ieee80211_vendor_set_RTSmode),
  VENDOR_CMD_WDEV(GET_RTS_MODE, _wave_ieee80211_vendor_get_RTSmode),

  VENDOR_CMD_WDEV(SET_MAX_MPDU_LENGTH, _wave_ieee80211_vendor_set_MaxMpduLen),
  VENDOR_CMD_WDEV(GET_MAX_MPDU_LENGTH, _wave_ieee80211_vendor_get_MaxMpduLen),

  VENDOR_CMD_WDEV(SET_BF_MODE, _wave_ieee80211_vendor_set_BfMode),
  VENDOR_CMD_WDEV(GET_BF_MODE, _wave_ieee80211_vendor_get_BfMode),

  VENDOR_CMD_WDEV_UP(SET_ACTIVE_ANT_MASK, _wave_ieee80211_vendor_set_ActiveAntMask),
  VENDOR_CMD_WDEV(GET_ACTIVE_ANT_MASK, _wave_ieee80211_vendor_get_ActiveAntMask),

  VENDOR_CMD_WDEV(SET_4ADDR_STA_ADD,  _wave_ieee80211_vendor_set_AddFourAddrSta),
  VENDOR_CMD_WDEV(SET_4ADDR_STA_DEL,  _wave_ieee80211_vendor_set_DelFourAddrSta),
  VENDOR_CMD_WDEV(GET_4ADDR_STA_LIST, _wave_ieee80211_vendor_get_FourAddrStas),

  VENDOR_CMD_WDEV(SET_TXOP_CONFIG, _wave_ieee80211_vendor_set_TxopConfig),
  VENDOR_CMD_WDEV(GET_TXOP_CONFIG, _wave_ieee80211_vendor_get_TxopConfig),

  VENDOR_CMD_WDEV(SET_SSB_MODE, _wave_ieee80211_vendor_set_SsbMode),
  VENDOR_CMD_WDEV(GET_SSB_MODE, _wave_ieee80211_vendor_get_SsbMode),

  VENDOR_CMD_WDEV(SET_COEX_CFG, _wave_ieee80211_vendor_set_EnableMRCoex),
  VENDOR_CMD_WDEV(GET_COEX_CFG, _wave_ieee80211_vendor_get_EnableMRCoex),

  VENDOR_CMD_WDEV(SET_MCAST_RANGE_SETUP,      _wave_ieee80211_vendor_set_McastRange),
  VENDOR_CMD_WDEV(GET_MCAST_RANGE_SETUP,      _wave_ieee80211_vendor_get_McastRange),
  VENDOR_CMD_WDEV(SET_MCAST_RANGE_SETUP_IPV6, _wave_ieee80211_vendor_set_McastRange6),
  VENDOR_CMD_WDEV(GET_MCAST_RANGE_SETUP_IPV6, _wave_ieee80211_vendor_get_McastRange6),
  VENDOR_CMD_WDEV(SET_FORWARD_UNKNOWN_MCAST_FLAG, _wave_ieee80211_vendor_set_forward_unknown_mcast_flag),
  VENDOR_CMD_WDEV(GET_FORWARD_UNKNOWN_MCAST_FLAG, _wave_ieee80211_vendor_get_forward_unknown_mcast_flag),

  VENDOR_CMD_WDEV(SET_ONLINE_CALIBRATION_ALGO_MASK, _wave_ieee80211_vendor_set_OnlineACM),
  VENDOR_CMD_WDEV(GET_ONLINE_CALIBRATION_ALGO_MASK, _wave_ieee80211_vendor_get_OnlineACM),
  VENDOR_CMD_WDEV(SET_CALIBRATION_ALGO_MASK,        _wave_ieee80211_vendor_set_AlgoCalibrMask),
  VENDOR_CMD_WDEV(GET_CALIBRATION_ALGO_MASK,        _wave_ieee80211_vendor_get_AlgoCalibrMask),

  VENDOR_CMD_WDEV(SET_RESTRICTED_AC_MODE, _wave_ieee80211_vendor_set_RestrictAcMode),
  VENDOR_CMD_WDEV(GET_RESTRICTED_AC_MODE, _wave_ieee80211_vendor_get_RestrictAcMode),

  VENDOR_CMD_WDEV(SET_PD_THRESHOLD, _wave_ieee80211_vendor_set_PdThresh),
  VENDOR_CMD_WDEV(GET_PD_THRESHOLD, _wave_ieee80211_vendor_get_PdThresh),

  VENDOR_CMD_WDEV(SET_FAST_DROP, _wave_ieee80211_vendor_set_FastDrop),
  VENDOR_CMD_WDEV(GET_FAST_DROP, _wave_ieee80211_vendor_get_FastDrop),

  VENDOR_CMD_WDEV(SET_ERP,      _wave_ieee80211_vendor_set_ErpSet),
  VENDOR_CMD_WDEV(GET_ERP_CFG , _wave_ieee80211_vendor_get_ErpCfg),

  VENDOR_CMD_WDEV(SET_STATIC_PLAN_CONFIG, _wave_ieee80211_vendor_set_MuStatPlanCfg),
  VENDOR_CMD_WDEV(SET_WDS_WEP_ENC_CFG, _wave_ieee80211_vendor_set_wds_wep_enc_cfg),
  VENDOR_CMD_WDEV(GET_PVT_SENSOR, _wave_ieee80211_vendor_get_pvt_sensor),
  
  VENDOR_CMD_WDEV(SET_RTS_RATE, _wave_ieee80211_vendor_set_rts_rate),
  VENDOR_CMD_WDEV(GET_RTS_RATE, _wave_ieee80211_vendor_get_rts_rate),
  VENDOR_CMD_WDEV(SET_STATIONS_STATISTICS, _wave_ieee80211_vendor_set_stations_statistics),
  VENDOR_CMD_WDEV(GET_STATIONS_STATISTICS, _wave_ieee80211_vendor_get_stations_statistics),

  VENDOR_CMD_WDEV(SET_ZWDFS_ANT, _wave_cfg80211_vendor_set_zwdfs_ant),
  VENDOR_CMD_WDEV(GET_ZWDFS_ANT, _wave_cfg80211_vendor_get_zwdfs_ant),
  VENDOR_CMD_WDEV(GET_RTS_THRESHOLD, _wave_ieee80211_vendor_get_rts_threshold),

  VENDOR_CMD_WDEV(GET_20MHZ_TX_POWER, _wave_ieee80211_vendor_get_20mhz_tx_power),

  VENDOR_CMD_WDEV(GET_PROP_PHY_CAP, _wave_ieee80211_vendor_get_prop_phy_cap),

  VENDOR_CMD_WDEV(GET_OBSS_BSSCOLOR_INFO, _wave_ieee80211_vendor_get_obss_bsscolor_info),

  VENDOR_CMD_WDEV(SET_DYNAMIC_EDCA, _wave_ieee80211_vendor_set_dynamic_edca),

  VENDOR_CMD_WDEV(GET_PCIE_AUTO_GEN_ENABLE, _wave_ieee80211_vendor_get_pcie_auto_gen_enable),
  VENDOR_CMD_WDEV(SET_PCIE_AUTO_GEN_ENABLE, _wave_ieee80211_vendor_set_pcie_auto_gen_enable),

/* IWLWAV statistics */
  VENDOR_CMD_WDEV(GET_ASSOCIATED_DEV_RATE_INFO_RX_STATS, _wave_cfg80211_vendor_get_associated_dev_rate_info_rx_stats),
  VENDOR_CMD_WDEV(GET_ASSOCIATED_DEV_RATE_INFO_TX_STATS, _wave_cfg80211_vendor_get_associated_dev_rate_info_tx_stats),
  VENDOR_CMD_WDEV(SET_STATS_POLL_PERIOD,        _wave_cfg80211_vendor_set_stats_poll_period),
  VENDOR_CMD_WDEV(GET_STATS_POLL_PERIOD,        _wave_cfg80211_vendor_get_stats_poll_period),
  VENDOR_CMD_WDEV(GET_ASSOCIATED_DEV_STATS,     _wave_cfg80211_vendor_get_associated_dev_stats),
  VENDOR_CMD_WDEV(GET_CHANNEL_STATS,            _wave_cfg80211_vendor_get_channel_stats),
  VENDOR_CMD_WDEV(GET_PHY_CHAN_STATUS,          _wave_cfg80211_vendor_get_phy_chan_status),
  VENDOR_CMD_WDEV(GET_ASSOCIATED_DEV_TID_STATS, _wave_cfg80211_vendor_get_associated_dev_tid_stats),
  VENDOR_CMD_WDEV(GET_PEER_LIST,                _wave_cfg80211_vendor_get_peer_list),
  VENDOR_CMD_WDEV(GET_PEER_FLOW_STATUS,         _wave_cfg80211_vendor_get_peer_flow_status),
  VENDOR_CMD_WDEV(GET_PEER_CAPABILITIES,        _wave_cfg80211_vendor_get_peer_capabilities),
  VENDOR_CMD_WDEV(GET_PEER_RATE_INFO,           _wave_cfg80211_vendor_get_peer_rate_info),
  VENDOR_CMD_WDEV(GET_RECOVERY_STATS,           _wave_cfg80211_vendor_get_recovery_stats),
  VENDOR_CMD_WDEV(GET_HW_FLOW_STATUS,           _wave_cfg80211_vendor_get_hw_flow_status),
  VENDOR_CMD_WDEV(GET_TR181_WLAN_STATS,         _wave_cfg80211_vendor_get_tr181_wlan_stats),
  VENDOR_CMD_WDEV(GET_TR181_HW_STATS,           _wave_cfg80211_vendor_get_tr181_hw_stats),
  VENDOR_CMD_WDEV(GET_TR181_PEER_STATS,         _wave_cfg80211_vendor_get_tr181_peer_stats),
  VENDOR_CMD_WDEV(GET_LAST_PROBE_REQS,          _wave_ieee80211_vendor_get_probe_reqs_info),
  VENDOR_CMD_WDEV(SET_CLT_PROBE_REQS_MODE,      _wave_ieee80211_vendor_set_probe_req_list_mode),
  VENDOR_CMD_WDEV(GET_CLT_PROBE_REQS_MODE,      _wave_ieee80211_vendor_get_probe_req_list_mode),

  VENDOR_CMD_WDEV_UP(SET_MGMT_FRAMES_RATE, _wave_ieee80211_vendor_hapd_set_management_frames_rate),

  VENDOR_CMD_WDEV_UP(GET_HE_OPERATION,           _wave_cfg80211_vendor_hapd_get_he_operation),
  VENDOR_CMD_WDEV(SET_DYNAMIC_MU_TYPE,        _wave_cfg80211_vendor_set_dynamic_mu_type),
  VENDOR_CMD_WDEV(GET_DYNAMIC_MU_TYPE,        _wave_cfg80211_vendor_get_dynamic_mu_type),
  VENDOR_CMD_WDEV(SET_HE_MU_FIXED_PARAMETERS, _wave_cfg80211_vendor_set_he_mu_fixed_parameters),
  VENDOR_CMD_WDEV(GET_HE_MU_FIXED_PARAMETERS, _wave_cfg80211_vendor_get_he_mu_fixed_parameters),
  VENDOR_CMD_WDEV(SET_HE_MU_DURATION,         _wave_cfg80211_vendor_set_he_mu_duration),
  VENDOR_CMD_WDEV(GET_HE_MU_DURATION,         _wave_cfg80211_vendor_get_he_mu_duration),
  VENDOR_CMD_WDEV_UP(GET_HE_NON_ADVERTISED,      _wave_cfg80211_vendor_hapd_get_he_non_advertised),
  VENDOR_CMD_WDEV_UP(SET_HE_NON_ADVERTISED,      _wave_cfg80211_vendor_hapd_set_he_non_advertised),
  VENDOR_CMD_WDEV_UP(SET_HE_DEBUG_DATA,          _wave_cfg80211_vendor_hapd_set_he_debug_data),

  VENDOR_CMD_WDEV_UP(GET_DEV_DIAG_RESULT2,  _wave_cfg80211_vendor_get_dev_diag_result2),
  VENDOR_CMD_WDEV_UP(GET_DEV_DIAG_RESULT3,  _wave_cfg80211_vendor_get_dev_diag_result3),
  VENDOR_CMD_WDEV_UP(GET_ASSOCIATED_STATIONS_STATS, _wave_cfg80211_vendor_get_associated_stations_stats),
  VENDOR_CMD_WDEV_UP(GET_TWT_PARAMETERS,    _wave_cfg80211_vendor_get_twt_parameters),
  VENDOR_CMD_WDEV_UP(GET_AX_DEFAULT_PARAMS, _wave_cfg80211_vendor_get_ax_default_params),
  VENDOR_CMD_WDEV(GET_PHY_INBAND_POWER,  _wave_cfg80211_vendor_get_phy_inband_power),

  VENDOR_CMD_WDEV(SET_ETSI_PPDU_LIMITS, _wave_cfg80211_vendor_set_etsi_ppdu_limits),
  VENDOR_CMD_WDEV(GET_ETSI_PPDU_LIMITS, _wave_cfg80211_vendor_get_etsi_ppdu_limits),

  VENDOR_CMD_WDEV(GET_DCDP_DATAPATH_MODE, _wave_ieee80211_vendor_get_datapath_mode),

#ifdef WAVE_ENABLE_PIE
  VENDOR_CMD_WDEV(SET_PIE_CFG , _wave_cfg80211_vendor_set_PIEcfg),
  VENDOR_CMD_WDEV(GET_PIE_CFG , _wave_cfg80211_vendor_get_PIEcfg),
  VENDOR_CMD_WDEV(SET_AQM_STA_EN , _wave_cfg80211_vendor_set_aqm_sta_en),
  VENDOR_CMD_WDEV(GET_AQM_STA_EN , _wave_cfg80211_vendor_get_aqm_sta_en),
#endif /* WAVE_ENABLE_PIE */

  VENDOR_CMD_WDEV(SET_AP_RETRY_LIMIT, _wave_ieee80211_vendor_set_ap_retry_limit),
  VENDOR_CMD_WDEV(GET_AP_RETRY_LIMIT, _wave_ieee80211_vendor_get_ap_retry_limit),

  VENDOR_CMD_WDEV(CHANNEL_SWITCH_DEAUTH_CFG, _wave_cfg80211_vendor_store_chan_switch_deauth_params),

  VENDOR_CMD_WDEV(SET_CTS_TO_SELF_TO,       _wave_ieee80211_vendor_set_cts_to_self_to),
  VENDOR_CMD_WDEV(GET_CTS_TO_SELF_TO,       _wave_ieee80211_vendor_get_cts_to_self_to),
  VENDOR_CMD_WDEV(SET_TX_AMPDU_DENSITY,     _wave_ieee80211_vendor_set_tx_ampdu_density),
  VENDOR_CMD_WDEV(GET_TX_AMPDU_DENSITY,     _wave_ieee80211_vendor_get_tx_ampdu_density),
  VENDOR_CMD_WDEV(GET_PRIORITY_GPIO,        _wave_cfg80211_vendor_get_priority_gpio_stats),
  VENDOR_CMD_WDEV(SET_FAST_DYNAMIC_MC_RATE, _wave_ieee80211_vendor_set_MulticastRate),
  VENDOR_CMD_WDEV(SET_HE_BEACON,             _wave_cfg80211_vendor_set_he_beacon),
  VENDOR_CMD_WDEV(SET_DUPLICATE_BEACON, _wave_ieee80211_vendor_set_duplicate_beacon),

  VENDOR_CMD_WDEV(SET_PBAC, _wave_ieee80211_vendor_set_pbac),

  VENDOR_CMD_WDEV(SET_MU_OFDMA_BF, _wave_ieee80211_vendor_set_MuOfdmaBf),
  VENDOR_CMD_WDEV(GET_MU_OFDMA_BF, _wave_ieee80211_vendor_get_MuOfdmaBf),
  VENDOR_CMD_WDEV(SET_CCA_PREAMBLE_PUNCTURE_CFG, _wave_ieee80211_vendor_set_preamble_puncture_cfg),
    /* CCA Measurement */
  VENDOR_CMD_WDEV(SET_START_CCA_MSR_OFF_CHAN,  _wave_ieee80211_vendor_set_start_cca_msr_off_chan),
  VENDOR_CMD_WDEV(GET_CCA_MSR_OFF_CHAN,        _wave_ieee80211_vendor_get_cca_msr_off_chan),
  VENDOR_CMD_WDEV(GET_CCA_STATS_CURRENT_CHAN,  _wave_ieee80211_vendor_get_cca_stats_current_chan),
  VENDOR_CMD_WDEV(GET_CCA_PREAMBLE_PUNCTURE_CFG, _wave_ieee80211_vendor_get_preamble_puncture_cfg),

    /* General Radio stats */
  VENDOR_CMD_WDEV(GET_RADIO_USAGE_STATS,  _wave_ieee80211_vendor_get_radio_usage_stats),

  VENDOR_CMD_WDEV(SET_PROBING_MASK, _wave_ieee80211_vendor_set_SlowProbingMask),
  VENDOR_CMD_WDEV(GET_PROBING_MASK, _wave_ieee80211_vendor_get_SlowProbingMask),

  VENDOR_CMD_WDEV(SET_SCAN_MODIFS, _wave_ieee80211_vendor_set_ScanModifFlags),
  VENDOR_CMD_WDEV(GET_SCAN_MODIFS, _wave_ieee80211_vendor_get_ScanModifFlags),
  VENDOR_CMD_WDEV(SET_WHM_CONFIG, _wave_ieee80211_vendor_set_WhmConfig),
  VENDOR_CMD_WDEV(SET_WHM_RESET, _wave_ieee80211_vendor_set_WhmReset),
  VENDOR_CMD_WDEV(SET_WHM_TRIGGER, _wave_ieee80211_vendor_set_WhmTrigger),

  VENDOR_CMD_WDEV(SET_SCAN_PAUSE_BG_CACHE, _wave_ieee80211_vendor_set_ScanPauseBGCache),
  VENDOR_CMD_WDEV(GET_SCAN_PAUSE_BG_CACHE, _wave_ieee80211_vendor_get_ScanPauseBGCache),

  /* Broadcast TWT */
  VENDOR_CMD_WDEV(ADVERTISE_BTWT_SCHEDULE, _wave_ieee80211_vendor_advertise_btwt_schedule),
  VENDOR_CMD_WDEV(TERMINATE_BTWT_SCHEDULE, _wave_ieee80211_vendor_terminate_btwt_schedule),
  VENDOR_CMD_WDEV(GET_ADVERTISED_BTWT_SCHEDULE, _wave_ieee80211_vendor_get_advertised_btwt_schedule),
  VENDOR_CMD_WDEV(TX_TWT_TEARDOWN, _wave_ieee80211_vendor_tx_twt_teardown),
  VENDOR_CMD_WDEV(SET_SB_TIMER_ACL_CFG, _wave_ieee80211_vendor_hapd_set_sb_timer_acl_params),

  VENDOR_CMD_WDEV(SET_FIXED_LTF_AND_GI, _wave_ieee80211_vendor_set_FixedLtfGi),
  VENDOR_CMD_WDEV(GET_FIXED_LTF_AND_GI, _wave_ieee80211_vendor_get_FixedLtfGi),

  /* Reset Statistics */
  VENDOR_CMD_WDEV(RESET_STATISTICS, _wave_cfg80211_vendor_reset_statistics),

  VENDOR_CMD_WDEV(SET_INTRA_VAP_MCAST, _wave_ieee80211_vendor_set_intra_vap_mcast),

  VENDOR_CMD_WDEV(SET_AP_EXCE_RETRY_LIMIT, _wave_ieee80211_vendor_set_ap_exce_retry_limit),
  VENDOR_CMD_WDEV(GET_AP_EXCE_RETRY_LIMIT, _wave_ieee80211_vendor_get_ap_exce_retry_limit),

  /* Management Frame Power Control */
  VENDOR_CMD_WDEV(SET_MGMT_FRAME_PWR_CTRL, _wave_ieee80211_vendor_set_MgmtFramePwrCtrl),
  VENDOR_CMD_WDEV(GET_MGMT_FRAME_PWR_CTRL, _wave_ieee80211_vendor_get_MgmtFramePwrCtrl),

  VENDOR_CMD_WDEV(GET_PEER_MIXED_STATS, _wave_ieee80211_vendor_get_peer_mixed_stats),

  /* Channel State Information */
  VENDOR_CMD_WDEV(SET_CSI_ENABLE,   _wave_ieee80211_vendor_set_CsiEnable),
  VENDOR_CMD_WDEV(GET_CSI_ENABLE,   _wave_ieee80211_vendor_get_CsiEnable),
  VENDOR_CMD_WDEV(CSI_SEND_NDP,     _wave_ieee80211_vendor_set_CsiSendQosNull),

  VENDOR_CMD_WDEV(SET_CSI_AUTO_RATE, _wave_ieee80211_vendor_set_CsiAutoRate),
  VENDOR_CMD_WDEV(GET_CSI_AUTO_RATE, _wave_ieee80211_vendor_get_CsiAutoRate),
  VENDOR_CMD_WDEV(GET_CSI_STATS,     _wave_cfg80211_vendor_get_CsiStats),
  VENDOR_CMD_WDEV(GET_CSI_COUNTERS,  _wave_cfg80211_vendor_get_CsiCounters),

  VENDOR_CMD_WDEV(GET_LINK_ADAPT_SU_MU_RU_OFDMA_STATS, _wave_ieee80211_vendor_get_su_mu_ru_stats),

  VENDOR_CMD_WDEV(SET_FIXED_RATE, _wave_ieee80211_vendor_set_FixedRateCfg),
  VENDOR_CMD_WDEV(SET_DBG_ASSERT, _wave_ieee80211_vendor_set_DoDebugAssert),

  VENDOR_CMD_WDEV(SET_ALLOW_3ADDR_MCAST, _wave_ieee80211_vendor_set_allow_3addr_mcast),
  VENDOR_CMD_WDEV(GET_ALLOW_3ADDR_MCAST, _wave_ieee80211_vendor_get_allow_3addr_mcast),

  VENDOR_CMD_WDEV(GET_CSI_CAPABILITY, _wave_ieee80211_vendor_get_csi_capability),

  /* Probe requests offload mode */
  VENDOR_CMD_WDEV(SET_PROBEREQ_OFFLOAD_MODE, _wave_ieee80211_vendor_set_probereq_offload_mode),

  VENDOR_CMD_WDEV(SET_MU_GROUPS_CONFIG, _wave_ieee80211_vendor_set_mu_groups_config),
  VENDOR_CMD_WDEV(GET_MU_GROUPS_CONFIG, _wave_ieee80211_vendor_get_mu_groups_config),

  VENDOR_CMD_WDEV(SET_UNCONNECTED_STA_SCAN_TIME, _wave_ieee80211_vendor_set_UnconnTime),
  VENDOR_CMD_WDEV(GET_UNCONNECTED_STA_SCAN_TIME, _wave_ieee80211_vendor_get_UnconnTime),

  VENDOR_CMD_WDEV(GET_RADIO_PEER_LIST, _wave_ieee80211_vendor_get_radio_peer_list),
  VENDOR_CMD_WDEV(GET_LINK_ADAPT_MIMO_OFDMA_STATS, _wave_ieee80211_vendor_get_mimo_ofdma_stats),
  VENDOR_CMD_WDEV(GET_LINK_ADAPT_MU_GROUPS_COUNTERS_STATS, _wave_ieee80211_vendor_get_LaMuGroupsCountersStats),
  VENDOR_CMD_WDEV(GET_MAX_TX_POWER, _wave_ieee80211_vendor_get_max_tx_power),
  /* Wave 700 inclusions */
#ifdef MTLK_WAVE_700
  VENDOR_CMD_MXL_WDEV_UP(SET_EHT_DEBUG_DATA,   _wave_cfg80211_vendor_hapd_set_eht_debug_data),
  VENDOR_CMD_MXL_WDEV_UP(SET_EHT_OPERATION,   _wave_cfg80211_vendor_hapd_set_eht_operation),
  VENDOR_CMD_MXL_WDEV_UP(GET_EHT_OPERATION,   _wave_cfg80211_vendor_hapd_get_eht_operation),
  VENDOR_CMD_MXL_WDEV_UP(SET_EHT_NON_ADVERTISED, _wave_cfg80211_vendor_hapd_set_eht_non_advertised),
  VENDOR_CMD_MXL_WDEV_UP(GET_EHT_NON_ADVERTISED, _wave_cfg80211_vendor_hapd_get_eht_non_advertised),
  VENDOR_CMD_MXL_WDEV_UP(SET_LOGGER_FIFO_MUX_CFG, _wave_ieee80211_vendor_set_LoggerFifoMuxCfg),
  VENDOR_CMD_MXL_WDEV_UP(GET_LOGGER_FIFO_MUX_CFG, _wave_ieee80211_vendor_get_LoggerFifoMuxCfg),
  /* MLD VAP info */
  VENDOR_CMD_WDEV(SETUP_MLD, _wave_ieee80211_vendor_setup_mld),
  VENDOR_CMD_WDEV(SET_MLD_INFO, _wave_ieee80211_vendor_set_mld_info),
  /* MLD STA request SID */
  VENDOR_CMD_WDEV(REQUEST_ML_SID, _wave_ieee80211_vendor_request_ml_sid),
  /* MLD STA set SID */
  VENDOR_CMD_WDEV(SET_ML_SID, _wave_ieee80211_vendor_set_ml_sid),
  /* ML ADD STA */
  VENDOR_CMD_WDEV(ML_STA_ADD, _wave_ieee80211_vendor_ml_sta_add),
  /* MLD_REMOVE */
  VENDOR_CMD_WDEV(MLD_REMOVE, _wave_ieee80211_vendor_mld_remove),
  /* MLD BSS critical update info */
  VENDOR_CMD_WDEV(SET_BSS_CRITICAL_UPDATE_INFO, _wave_cfg80211_vendor_set_bss_critical_update_info),
#ifdef BEST_EFFORT_TID_SPREADING
  VENDOR_CMD_WDEV(SET_STR_TID_LINK_SPREADING, _wave_ieee80211_vendor_set_str_tid_link_spreading),
#endif
  VENDOR_CMD_WDEV(SET_DYNAMIC_WMM, _wave_cfg80211_vendor_enable_dynamic_wmm),
  VENDOR_CMD_WDEV(SCS_ADD_REQ, _wave_ieee80211_vendor_scs_add_req),
  VENDOR_CMD_WDEV(SCS_REM_REQ, _wave_ieee80211_vendor_scs_rem_req),
  VENDOR_CMD_WDEV(SET_SCS_ENABLE, _wave_ieee80211_vendor_set_scs_enable),
  VENDOR_CMD_WDEV(GET_SCS_ENABLE, _wave_ieee80211_vendor_get_scs_enable),
  /* Send Tid to link mapping teardown frame to ML sta */
  VENDOR_CMD_WDEV(ML_SEND_T2LM_TEARDOWN_FRAME, _wave_ieee80211_vendor_send_t2lm_teardown_frame),
  /* Get Link statistics for ML STA */
  VENDOR_CMD_WDEV(GET_ML_LINK_STATS, _wave_ieee80211_vendor_get_ML_link_stats),
  /* Reset Link statistics for ML STA */
  VENDOR_CMD_WDEV(SET_ML_LINK_STATS, _wave_ieee80211_vendor_set_ML_link_stats),
  /* Get ML VAP list */
  VENDOR_CMD_WDEV(GET_ML_VAP_LIST, _wave_ieee80211_vendor_get_ML_vap_list),
  /* Set 6GHz interface operating power mode */
  VENDOR_CMD_WDEV(REG_UPDATE_6GHZ_OPER_POWER_MODE, _wave_cfg80211_vendor_reg_update_6ghz_oper_power_mode),
  /* afcd update regulatory info for standard power AP */
  VENDOR_CMD_WDEV(UPDATE_AFC_INFO, _wave_cfg80211_vendor_update_afc_info),
  /* ML STA reassoc notify */
  VENDOR_CMD_WDEV(ML_STA_REASSOC_NOTIFY, _wave_ieee80211_vendor_ml_sta_reassoc_notify),
  /* Get mlpeerflowstatus for ML STA */
  VENDOR_CMD_WDEV(GET_ML_PEER_FLOW_STATUS, _wave_ieee80211_vendor_get_ml_peer_flow_status),
  /* get connected ML STA List */
  VENDOR_CMD_WDEV(GET_ML_STA_LIST, _wave_ieee80211_vendor_get_ML_sta_list),
  /* Add MSCS req */
  VENDOR_CMD_WDEV(MSCS_ADD_REQ, _wave_ieee80211_vendor_mscs_add_req),
  /* Remove MSCS req */
  VENDOR_CMD_WDEV(MSCS_REM_REQ, _wave_ieee80211_vendor_mscs_rem_req),
  /* Set MSCS enable */
  VENDOR_CMD_WDEV(SET_MSCS_ENABLE, _wave_ieee80211_vendor_set_mscs_enable),
  /* Get MSCS enable */
  VENDOR_CMD_WDEV(GET_MSCS_ENABLE, _wave_ieee80211_vendor_get_mscs_enable),
#endif

  VENDOR_CMD_WDEV(SET_FIXED_RATE_THERMAL, _wave_ieee80211_vendor_set_FixedRateThermal),
  VENDOR_CMD_WDEV(GET_FIXED_RATE_THERMAL, _wave_ieee80211_vendor_get_FixedRateThermal),

  VENDOR_CMD_WDEV(SET_FIXED_POWER, _wave_ieee80211_vendor_set_FixedPower),
  VENDOR_CMD_WDEV(GET_FIXED_POWER, _wave_ieee80211_vendor_get_FixedPower),

  /******************** DEBUG COMMANDS ********************/
#ifdef CONFIG_WAVE_DEBUG

  VENDOR_CMD_WDEV(SET_COUNTERS_SRC, _wave_ieee80211_vendor_set_CountersSrc),
  VENDOR_CMD_WDEV(GET_COUNTERS_SRC, _wave_ieee80211_vendor_get_CountersSrc),

  VENDOR_CMD_WDEV(SET_CPU_DMA_LATENCY, _wave_ieee80211_vendor_set_CpuDmaLatency),
  VENDOR_CMD_WDEV(GET_CPU_DMA_LATENCY, _wave_ieee80211_vendor_get_CpuDmaLatency),

  VENDOR_CMD_WDEV(GET_BEAMFORM_EXPLICIT, _wave_ieee80211_vendor_get_BfExplicitCap),

  VENDOR_CMD_WDEV(SET_TASKLET_LIMITS, _wave_ieee80211_vendor_set_TaskletLimits),
  VENDOR_CMD_WDEV(GET_TASKLET_LIMITS, _wave_ieee80211_vendor_get_TaskletLimits),

  VENDOR_CMD_WDEV(GET_GENL_FAMILY_ID, _wave_ieee80211_vendor_get_GenlFamilyId),

  VENDOR_CMD_WDEV(SET_SCAN_EXP_TIME, _wave_ieee80211_vendor_set_ScanExpTime),
  VENDOR_CMD_WDEV(GET_SCAN_EXP_TIME, _wave_ieee80211_vendor_get_ScanExpTime),

  VENDOR_CMD_WDEV(GET_TA_DBG, _wave_ieee80211_vendor_get_TADbg),

  VENDOR_CMD_WDEV(SET_TA_TIMER_RESOLUTION, _wave_ieee80211_vendor_set_TATimerRes),
  VENDOR_CMD_WDEV(GET_TA_TIMER_RESOLUTION, _wave_ieee80211_vendor_get_TATimerRes),

  VENDOR_CMD_WDEV(SET_PCOC_AUTO_PARAMS, _wave_ieee80211_vendor_set_PCoCAutoCfg),
  VENDOR_CMD_WDEV(GET_PCOC_AUTO_PARAMS, _wave_ieee80211_vendor_get_PCoCAutoCfg),
  VENDOR_CMD_WDEV(SET_PCOC_POWER_MODE, _wave_ieee80211_vendor_set_PCoCPower),
  VENDOR_CMD_WDEV(GET_PCOC_POWER_MODE, _wave_ieee80211_vendor_get_PCoCPower),
  VENDOR_CMD_WDEV(SET_PCOC_PMCU_DEBUG, _wave_ieee80211_vendor_set_PMCUDebug),

  VENDOR_CMD_WDEV(SET_WDS_HOST_TIMEOUT, _wave_ieee80211_vendor_set_WDSHostTO),
  VENDOR_CMD_WDEV(GET_WDS_HOST_TIMEOUT, _wave_ieee80211_vendor_get_WDSHostTO),

  VENDOR_CMD_WDEV(SET_MAC_WATCHDOG_TIMEOUT_MS, _wave_ieee80211_vendor_set_MACWdTimeoutMs),
  VENDOR_CMD_WDEV(GET_MAC_WATCHDOG_TIMEOUT_MS, _wave_ieee80211_vendor_get_MACWdTimeoutMs),
  VENDOR_CMD_WDEV(SET_MAC_WATCHDOG_PERIOD_MS, _wave_ieee80211_vendor_set_MACWdPeriodMs),
  VENDOR_CMD_WDEV(GET_MAC_WATCHDOG_PERIOD_MS, _wave_ieee80211_vendor_get_MACWdPeriodMs),

  VENDOR_CMD_WDEV(SET_NON_OCCUPATED_PRD, _wave_ieee80211_vendor_set_NonOccupatePrd),
  VENDOR_CMD_WDEV(GET_NON_OCCUPATED_PRD, _wave_ieee80211_vendor_get_NonOccupatePrd),
  VENDOR_CMD_WDEV(SET_11H_BEACON_COUNT, _wave_ieee80211_vendor_set_11hBeaconCount),
  VENDOR_CMD_WDEV(GET_11H_BEACON_COUNT, _wave_ieee80211_vendor_get_11hBeaconCount),

  VENDOR_CMD_WDEV(SET_ENABLE_TEST_BUS, _wave_ieee80211_vendor_set_enable_test_bus),

  VENDOR_CMD_WDEV(SET_FW_DEBUG, _wave_ieee80211_vendor_set_DoFwDebug),
  VENDOR_CMD_WDEV(SET_DBG_CLI, _wave_ieee80211_vendor_set_DoSimpleCLI),

  VENDOR_CMD_WDEV(SET_FW_LOG_SEVERITY, _wave_ieee80211_vendor_set_FwLogSeverity),

  VENDOR_CMD_WDEV(SET_DBG_CMD_FW, _wave_cfg80211_vendor_set_dbgCmdFw),
  VENDOR_CMD_WDEV(SET_RADAR_FIFO_DUMP, _wave_cfg80211_vendor_set_radarFifoDump),
  VENDOR_CMD_WDEV(GET_PEER_HOST_IF_QOS, _wave_cfg80211_vendor_get_peer_host_if_qos),
  VENDOR_CMD_WDEV(GET_PEER_HOST_IF, _wave_cfg80211_vendor_get_peer_host_if),
  VENDOR_CMD_WDEV(GET_PEER_RX_STATS, _wave_cfg80211_vendor_get_peer_rx_stats),
  VENDOR_CMD_WDEV(GET_PEER_UL_BSRC_TID_STATS, _wave_cfg80211_vendor_get_peer_ul_bsrc_tid_stats),
  VENDOR_CMD_WDEV(GET_PEER_BAA_STATS, _wave_cfg80211_vendor_get_peer_baa_stats),
  VENDOR_CMD_WDEV(GET_LINK_ADAPTION_STATS, _wave_cfg80211_vendor_get_link_adaption_stats),
  VENDOR_CMD_WDEV(GET_PLAN_MANAGER_STATS, _wave_cfg80211_vendor_get_plan_manager_stats),
  VENDOR_CMD_WDEV(GET_PEER_TWT_STATS, _wave_cfg80211_vendor_get_peer_twt_stats),
  VENDOR_CMD_WDEV(GET_PER_CLIENT_STATS, _wave_cfg80211_vendor_get_per_client_stats),
  VENDOR_CMD_WDEV(GET_PEER_PHY_RX_STATUS, _wave_cfg80211_vendor_get_peer_phy_rx_status),
  VENDOR_CMD_WDEV(GET_PEER_INFO, _wave_cfg80211_vendor_get_peer_info),
  VENDOR_CMD_WDEV(GET_WLAN_HOST_IF_QOS, _wave_cfg80211_vendor_get_wlan_host_if_qos),
  VENDOR_CMD_WDEV(GET_WLAN_HOST_IF, _wave_cfg80211_vendor_get_wlan_host_if),
  VENDOR_CMD_WDEV(GET_WLAN_RX_STATS, _wave_cfg80211_vendor_get_wlan_rx_stats),
  VENDOR_CMD_WDEV(GET_WLAN_BAA_STATS, _wave_cfg80211_vendor_get_wlan_baa_stats),
  VENDOR_CMD_WDEV(GET_RADIO_RX_STATS, _wave_cfg80211_vendor_get_radio_rx_stats),
  VENDOR_CMD_WDEV(GET_RADIO_BAA_STATS, _wave_cfg80211_vendor_get_radio_baa_stats),
  VENDOR_CMD_WDEV(GET_TSMAN_INIT_TID_GL, _wave_cfg80211_vendor_get_tsman_init_tid_gl),
  VENDOR_CMD_WDEV(GET_TSMAN_INIT_STA_GL, _wave_cfg80211_vendor_get_tsman_init_sta_gl),
  VENDOR_CMD_WDEV(GET_TSMAN_RCPT_TID_GL, _wave_cfg80211_vendor_get_tsman_rcpt_tid_gl),
  VENDOR_CMD_WDEV(GET_TSMAN_RCPT_STA_GL, _wave_cfg80211_vendor_get_tsman_rcpt_sta_gl),
  VENDOR_CMD_WDEV(GET_RADIO_LINK_ADAPT_STATS, _wave_cfg80211_vendor_get_radio_link_adapt_stats),
  VENDOR_CMD_WDEV(GET_MULTICAST_STATS, _wave_cfg80211_vendor_get_multicast_stats),
  VENDOR_CMD_WDEV(GET_TRAINING_MAN_STATS, _wave_cfg80211_vendor_get_training_man_stats),
  VENDOR_CMD_WDEV(GET_GROUP_MAN_STATS, _wave_cfg80211_vendor_get_group_man_stats),
  VENDOR_CMD_WDEV(GET_GENERAL_STATS, _wave_cfg80211_vendor_get_general_stats),
  VENDOR_CMD_WDEV(GET_CUR_CHANNEL_STATS, _wave_cfg80211_vendor_get_cur_channel_stats),
  VENDOR_CMD_WDEV(GET_RADIO_PHY_RX_STATS, _wave_cfg80211_vendor_get_radio_phy_rx_stats),
  VENDOR_CMD_WDEV(GET_DYNAMIC_BW_STATS, _wave_cfg80211_vendor_get_dynamic_bw_stats),
  VENDOR_CMD_WDEV(GET_LA_MU_VHT_STATS, _wave_cfg80211_vendor_get_la_mu_vht_stats),
  VENDOR_CMD_WDEV(SET_UNPROTECTED_DEAUTH, _wave_cfg80211_vendor_set_dbg_unprotected_deauth),
  VENDOR_CMD_WDEV(SET_PN_RESET, _wave_cfg80211_vendor_set_dbg_pn_reset),
  VENDOR_CMD_WDEV(SET_MTLK_LOG_LEVEL, _wave_ieee80211_vendor_set_MtlkLogLevel),
  VENDOR_CMD_WDEV(GET_LA_MU_HE_EHT_STATS, _wave_cfg80211_vendor_get_la_mu_he_eht_stats),
  VENDOR_CMD_WDEV(SET_VW_TEST_MODE, _wave_cfg80211_vendor_set_vw_test_mode),
  VENDOR_CMD_WDEV(GET_VW_TEST_MODE, _wave_cfg80211_vendor_get_vw_test_mode),
#endif /* CONFIG_WAVE_DEBUG */
};

void _wave_mac80211_register_vendor_cmds(struct wiphy *wiphy)
{
  wiphy->vendor_commands = _wave_mac80211_vendor_commands;
  wiphy->n_vendor_commands = ARRAY_SIZE(_wave_mac80211_vendor_commands);
}

#define VENDOR_EVT(cmd) {OUI_LTQ, (cmd)}

static struct
nl80211_vendor_cmd_info _wave_mac80211_vendor_events[] = {
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_RX_EAPOL),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_FLUSH_STATIONS),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_CHAN_DATA),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_UNCONNECTED_STA),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_WDS_CONNECT),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_WDS_DISCONNECT),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_CSA_RECEIVED),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_RADAR_DETECTED),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_ASSERT_DUMP_READY),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_NO_DUMP),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_UNRECOVERABLE_ERROR),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_MAC_ERROR),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_SOFTBLOCK_DROP),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_CAL_FILE),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_COC_BEACON_UPDATE),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_WHM),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_CSI_STATS),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_DYNAMIC_WMM_UPDATE),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_REGDB_INFO_UPDATE),
  VENDOR_EVT(LTQ_NL80211_VENDOR_EVENT_RX_MEASURE),
};

void _wave_mac80211_register_vendor_evts(struct wiphy *wiphy)
{
  wiphy->vendor_events = _wave_mac80211_vendor_events;
  wiphy->n_vendor_events = ARRAY_SIZE(_wave_mac80211_vendor_events);
}

int __MTLK_IFUNC
wv_cfg80211_vendor_cmd_alloc_and_reply (struct wiphy *wiphy, const void *data,
                                        int data_len)
{
  uint8 *cp;
  mtlk_nbuf_t *rsp_nbuf;

  rsp_nbuf = wv_cfg80211_vendor_cmd_alloc_reply_skb(wiphy, data_len);
  if (!rsp_nbuf) {
    return -ENOMEM;
  }

  cp = mtlk_df_nbuf_put(rsp_nbuf, data_len);
  wave_memcpy(cp, data_len, data, data_len);

  return wv_cfg80211_vendor_cmd_reply(rsp_nbuf);
}

mtlk_error_t __MTLK_IFUNC
wave_ieee80211_vendor_send_fils_discovery_frame_wrap (struct wiphy *wiphy, struct wireless_dev *wdev,
                                                      const void *data, size_t data_len)
{
  if ((wiphy == NULL) || (wdev == NULL) || (data == NULL) || (data_len == 0))
    return MTLK_ERR_PARAMS;

  return (_wave_ieee80211_vendor_send_fils_discovery_frame(wiphy, wdev, data, data_len) ? MTLK_ERR_UNKNOWN : MTLK_ERR_OK);
}

mtlk_error_t __MTLK_IFUNC
wave_ieee80211_vendor_set_unsolicited_frame_tx_wrap (struct wiphy *wiphy,  struct wireless_dev *wdev,
                                                     const wave_unsolicited_cfg_t *wave_unsolicited_cfg)
{
  uint32 intvec[2];

  if ((wiphy == NULL) || (wdev == NULL) || (wave_unsolicited_cfg == NULL))
    return MTLK_ERR_PARAMS;

  intvec[0] = wave_unsolicited_cfg->cfg.mode;
  intvec[1] = wave_unsolicited_cfg->cfg.duration;
  return (_wave_cfg80211_vendor_set_unsolicited_frame_tx(wiphy, wdev, intvec, sizeof(intvec)) ? MTLK_ERR_UNKNOWN : MTLK_ERR_OK);
}
