/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_vap.c                                         *
*         Description  : Vap related API                                       *
*                                                                              *
*  *****************************************************************************/

#include <swl/swl_common.h>
#include <swl/map/swl_mapCharFmt.h>
#include <swla/swla_mac.h>

#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_ap_nl80211.h"
#include "wld/wld_hostapd_ap_api.h"
#include "wld/wld_eventing.h"
#include "wld/Utils/wld_autoCommitMgr.h"

#include "whm_mxl_module.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_cfgActions.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_wmm.h"
#include "whm_mxl_mlo.h"
#include "whm_mxl_reconfMngr.h"

#define START_ENABLE_SYNC_TIMEOUT_MS 10000

#define ME "mxlVap"

SWL_TABLE(sOperStdMap,
          ARR(char* name; swl_radStd_e opStd; ),
          ARR(swl_type_charPtr, swl_type_uint32, ),
          ARR({"auto", SWL_RADSTD_AUTO},
              {"802.11a", SWL_RADSTD_A},
              {"802.11b", SWL_RADSTD_B},
              {"802.11g", SWL_RADSTD_G},
              {"802.11n", SWL_RADSTD_N},
              {"802.11ac", SWL_RADSTD_AC},
              {"802.11ax", SWL_RADSTD_AX},
              {"802.11be", SWL_RADSTD_BE},
              ));

mxl_VapVendorData_t* mxl_vap_getVapVendorData(const T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, NULL, ME, "pAP is NULL");
    return (mxl_VapVendorData_t*) pAP->vendorData;
}

static void s_enableSync(amxp_timer_t* timer _UNUSED, void* priv) {
    SAH_TRACEZ_IN(ME);
    T_AccessPoint* pAP = (T_AccessPoint*) priv;
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    mxl_VapVendorData_t* pVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendorData, , ME, "pVapVendorData is NULL");

    /* Check if vap on enable sync is still pending */
    if (pVapVendorData->vapEnableReloadPending) {
        setBitLongArray(pVapVendorData->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_RELOAD_BSS);
        whm_mxl_reconfMngr_notifyVapCommit(pAP);
        pVapVendorData->vapEnableReloadPending = false;
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_mxl_vap_init_vendordata(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, , ME, "pAP is NULL");
    /* Add desired initializations here*/
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    mxlVapVendorData->mloId = -1;
    mxlVapVendorData->MLO_destroyInProgress = 0;
    mxlVapVendorData->saeExtKey = 0;
    mxlVapVendorData->EnableWPA3PersonalCompatibility = 0;
    /* Init VAP enable sync timer */
    amxp_timer_new(&mxlVapVendorData->onVapEnableSyncTimer, s_enableSync, pAP);
    return;
}

static void s_mxlWdsIfaceChangeCb(wld_wds_intf_t* wdsIntf) {
    ASSERT_NOT_NULL(wdsIntf, , ME, "wdsIntf is NULL");
    T_AccessPoint* pAP = wdsIntf->ap;
    ASSERT_NOT_NULL(pAP, , ME, "pAP is NULL");

    if (wdsIntf->active) {
        SAH_TRACEZ_INFO(ME, "%s: wds entry %s added event", pAP->alias, wdsIntf->name);
        whm_mxl_vap_addWdsIfaceEvent(pAP, wdsIntf);
    } else {
        SAH_TRACEZ_INFO(ME, "%s: wds entry %s deleted event", pAP->alias, wdsIntf->name);
        whm_mxl_vap_delWdsIfaceEvent(pAP, wdsIntf);
    }
}

static wld_event_callback_t s_mxlWdsInterfaceChange = {
    .callback = (wld_event_callback_fun) s_mxlWdsIfaceChangeCb,
};

void whm_mxl_registerToWdsEvent(void) {
    /* Register to wds interface change events */
    wld_event_add_callback(gWld_queue_wdsInterface, &s_mxlWdsInterfaceChange);
}

void whm_mxl_unregisterToWdsEvent(void) {
    /* Unregister to wds interface change events */
    wld_event_remove_callback(gWld_queue_wdsInterface, &s_mxlWdsInterfaceChange);
}

int whm_mxl_vap_createHook(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    swl_rc_ne rc = SWL_RC_OK;
    CALL_NL80211_FTA_RET(rc, mfn_wvap_create_hook, pAP);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    pAP->vendorData = calloc(1, sizeof(mxl_VapVendorData_t));
    ASSERT_NOT_NULL(pAP->vendorData, SWL_RC_INVALID_PARAM, ME, "pAP->vendorData calloc returned NULL");
    s_mxl_vap_init_vendordata(pAP);

    /* Regstration to WDS events is done during Radio creation hook */

    return rc;
}

static void s_mxl_deinit_vendorVapData(mxl_VapVendorData_t* mxlVapVendorData) {
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    amxp_timer_delete(&mxlVapVendorData->onVapEnableSyncTimer);
    return;
}

void whm_mxl_vap_destroyHook(T_AccessPoint* pAP){
    ASSERT_NOT_NULL(pAP, , ME, "pAP is NULL");
    mxl_VapVendorData_t* mxlVapVendorData;
    CALL_NL80211_FTA(mfn_wvap_destroy_hook, pAP);
    mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    s_mxl_deinit_vendorVapData(mxlVapVendorData);
    /* Unregister to WDS events is done during Radio destroy hook */
    free(mxlVapVendorData);
}

static swl_rc_ne s_getDevDiagResults3Cb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_AssociatedDevice* pAD = (T_AssociatedDevice*) priv;
    ASSERT_NOT_NULL(pAD, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    wifiAssociatedDevDiagnostic3_t* devDiagRes3Stats = (wifiAssociatedDevDiagnostic3_t*) data;
    const char* opStdName = (const char*) devDiagRes3Stats->wifiAssociatedDevDiagnostic2.OperatingStandard;
    swl_radStd_e* pOpStd = (swl_radStd_e*) swl_table_getMatchingValue(&sOperStdMap, 1, 0, opStdName);
    pAD->operatingStandard = (pOpStd != NULL) ? *pOpStd : SWL_RADSTD_AUTO;
    int8_t sum = 0;
    for(int i = 0; i < PHY_STATISTICS_MAX_RX_ANT; i++) { // computing an average
        sum += devDiagRes3Stats->wifiAssociatedDevDiagnostic2.SNR[i];
    }
    pAD->SignalNoiseRatio = sum / PHY_STATISTICS_MAX_RX_ANT;
    pAD->TxBytes = devDiagRes3Stats->wifiAssociatedDevDiagnostic2.BytesSent;
    pAD->RxBytes = devDiagRes3Stats->wifiAssociatedDevDiagnostic2.BytesReceived;
    pAD->TxPacketCount = devDiagRes3Stats->PacketsSent;
    pAD->RxPacketCount = devDiagRes3Stats->PacketsReceived;
    pAD->Retransmissions = devDiagRes3Stats->wifiAssociatedDevDiagnostic2.Retransmissions;
    pAD->Tx_RetransmissionsFailed = devDiagRes3Stats->FailedRetransCount;

    return rc;
}

static swl_rc_ne s_getDevDiagResults3(T_AssociatedDevice* pAD) {
    swl_rc_ne rc = SWL_RC_OK;
    T_AccessPoint* pAP = (T_AccessPoint*) amxd_object_get_parent(amxd_object_get_parent(pAD->object))->priv;
    ASSERT_NOT_NULL(pAP, SWL_RC_ERROR, ME, "NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERTI_NOT_EQUALS(pRad->status, RST_ERROR, SWL_RC_INVALID_STATE, ME, "radio error status");
    ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");

    // execute LTQ_NL80211_VENDOR_SUBCMD_GET_DEV_DIAG_RESULT3 to get
    // OperatingStandard, SignalNoiseRatio, TxBytes, RxBytes,
    // TxPacketCount, RxPacketCount, Retransmissions, Tx_RetransmissionsFailed,

    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_DEV_DIAG_RESULT3;
    rc = wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, pAD->MACAddress, ETHER_ADDR_LEN,
                                         VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getDevDiagResults3Cb, pAD);
    return rc;
}

static swl_rc_ne s_getPeerFlowStatusCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_AssociatedDevice* pAD = (T_AssociatedDevice*) priv;
    ASSERT_NOT_NULL(pAD, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    mtlk_wssa_drv_peer_stats_t* peerFlowStats = (mtlk_wssa_drv_peer_stats_t*) data;
    ASSERT_NOT_NULL(peerFlowStats, SWL_RC_ERROR, ME, "NULL");
    pAD->Rx_Retransmissions = peerFlowStats->tr181_stats.retrans_stats.Retransmissions;
    pAD->RxUnicastPacketCount = peerFlowStats->tr181_stats.traffic_stats.UnicastPacketsReceived;
    pAD->TxMulticastPacketCount = peerFlowStats->tr181_stats.traffic_stats.MulticastPacketsSent;
    pAD->TxUnicastPacketCount = peerFlowStats->tr181_stats.traffic_stats.UnicastPacketsSent;
    pAD->SignalStrength = peerFlowStats->tr181_stats.SignalStrength;
    pAD->TxBytes = peerFlowStats->tr181_stats.traffic_stats.BytesSent;
    pAD->RxBytes = peerFlowStats->tr181_stats.traffic_stats.BytesReceived;
    pAD->LastDataUplinkRate = peerFlowStats->tr181_stats.LastDataUplinkRate;
    pAD->LastDataDownlinkRate = peerFlowStats->tr181_stats.LastDataDownlinkRate;
    pAD->MaxUplinkRateSupported = SWL_MAX(pAD->MaxUplinkRateSupported, peerFlowStats->tr181_stats.LastDataUplinkRate);
    pAD->MaxDownlinkRateSupported = SWL_MAX(pAD->MaxDownlinkRateSupported, peerFlowStats->tr181_stats.LastDataDownlinkRate);

    return rc;
}

static swl_rc_ne s_getPeerFlowStatus(T_AssociatedDevice* pAD) {
    swl_rc_ne rc = SWL_RC_OK;
    T_AccessPoint* pAP = wld_ap_fromObj(amxd_object_get_parent(amxd_object_get_parent(pAD->object)));
    ASSERT_NOT_NULL(pAP, SWL_RC_ERROR, ME, "NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERTI_NOT_EQUALS(pRad->status, RST_ERROR, SWL_RC_INVALID_STATE, ME, "NULL");
    ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");

    // execute LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_FLOW_STATUS to get
    // Rx_Retransmissions, RxUnicastPacketCount, TxMulticastPacketCount,
    // TxUnicastPacketCount, SignalStrength

    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_FLOW_STATUS;
    rc = wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, pAD->MACAddress, ETHER_ADDR_LEN,
                                         VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getPeerFlowStatusCb, pAD);
    return rc;
}

swl_rc_ne whm_mxl_vap_getSingleStationStats(T_AssociatedDevice* pAD) {
    ASSERT_NOT_NULL(pAD, SWL_RC_INVALID_PARAM, ME, "NULL");
    T_AccessPoint* pAP = wld_ap_fromObj(amxd_object_get_parent(amxd_object_get_parent(pAD->object)));
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_FALSE(pRad->detailedState == CM_RAD_DOWN, SWL_RC_INVALID_STATE, ME, "Radio state is down");
    swl_rc_ne rc = SWL_RC_OK;

    CALL_NL80211_FTA_RET(rc, mfn_wvap_get_single_station_stats, pAD);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");
    ASSERTI_TRUE(pAD->Active, SWL_RC_OK, ME, "%s: assocdev no more active", pAD->Name);

    rc = s_getDevDiagResults3(pAD);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get station info %s", pAD->Name);

    rc = s_getPeerFlowStatus(pAD);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get peer flow status %s", pAD->Name);

    return rc;
}

swl_rc_ne whm_mxl_vap_getStationStats(T_AccessPoint* pAP) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");

    for(int i = 0; i < pAP->AssociatedDeviceNumberOfEntries; i++) {
        T_AssociatedDevice* pAD = pAP->AssociatedDevice[i];
        if(!pAD) {
            SAH_TRACEZ_ERROR(ME, "nullpointer! %p", pAD);
            return SWL_RC_ERROR;
        }
        whm_mxl_vap_getSingleStationStats(pAD);
    }

    return SWL_RC_OK;
}

static swl_rc_ne s_getTr181WlanStatsApCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_AccessPoint* pAP = (T_AccessPoint*) priv;
    ASSERT_NOT_NULL(pAP, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    mtlk_wssa_drv_tr181_wlan_stats_t* tr181Stats = (mtlk_wssa_drv_tr181_wlan_stats_t*) data;
    ASSERT_NOT_NULL(tr181Stats, SWL_RC_ERROR, ME, "NULL");

    T_Stats* pApStats = &pAP->pSSID->stats;
    pApStats->BytesSent                   = tr181Stats->traffic_stats.BytesSent;
    pApStats->BytesReceived               = tr181Stats->traffic_stats.BytesReceived;
    pApStats->PacketsSent                 = tr181Stats->traffic_stats.PacketsSent;
    pApStats->PacketsReceived             = tr181Stats->traffic_stats.PacketsReceived;
    pApStats->ErrorsSent                  = tr181Stats->error_stats.ErrorsSent;
    pApStats->RetransCount                = tr181Stats->retrans_stats.RetransCount;
    pApStats->ErrorsReceived              = tr181Stats->error_stats.ErrorsReceived;
    pApStats->DiscardPacketsSent          = tr181Stats->error_stats.DiscardPacketsSent;
    pApStats->DiscardPacketsReceived      = tr181Stats->error_stats.DiscardPacketsReceived;
    pApStats->UnicastPacketsSent          = tr181Stats->traffic_stats.UnicastPacketsSent;
    pApStats->UnicastPacketsReceived      = tr181Stats->traffic_stats.UnicastPacketsReceived;
    pApStats->MulticastPacketsSent        = tr181Stats->traffic_stats.MulticastPacketsSent;
    pApStats->MulticastPacketsReceived    = tr181Stats->traffic_stats.MulticastPacketsReceived;
    pApStats->BroadcastPacketsSent        = tr181Stats->traffic_stats.BroadcastPacketsSent;
    pApStats->BroadcastPacketsReceived    = tr181Stats->traffic_stats.BroadcastPacketsReceived;
    pApStats->UnknownProtoPacketsReceived = tr181Stats->UnknownProtoPacketsReceived;
    pApStats->FailedRetransCount          = tr181Stats->retrans_stats.FailedRetransCount;
    pApStats->RetryCount                  = tr181Stats->retrans_stats.RetryCount;
    pApStats->MultipleRetryCount          = tr181Stats->retrans_stats.MultipleRetryCount;

    return rc;
}

int whm_mxl_vap_updateApStats(T_AccessPoint* pAP) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERTI_NOT_EQUALS(pRad->status, RST_ERROR, SWL_RC_INVALID_STATE, ME, "NULL");
    ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");

    SAH_TRACEZ_INFO(ME, "update Ap stats %s", pAP->pSSID->Name);
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_TR181_WLAN_STATS;
    rc = wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, pAP->pSSID->Name, strlen(pAP->pSSID->Name),
                                         VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getTr181WlanStatsApCb, pAP);

    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "%s: GET_TR181_WLAN_STATS failed", pAP->alias);
    rc = mxl_getWmmStats(pAP, &pAP->pSSID->stats, false);

    return rc;
}

static bool s_hasEnabledRealVap(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    T_AccessPoint* pAP = NULL;

    /* Check if no AP is enabled */
    wld_rad_forEachAp(pAP, pRad) {
        if((pAP->pBus != NULL) && pAP->enable) {
            return true;
        }
    }
    return false;
}

static void s_enableDummyVap(T_Radio* pRad, int enable) {
    T_AccessPoint* dumVap = wld_rad_getFirstVap(pRad);
    ASSERTS_NOT_NULL(dumVap, , ME, "no dummy vap");
    ASSERTS_NOT_NULL(dumVap->pSSID, , ME, "no dummy ssid");
    dumVap->enable = enable;
    dumVap->pSSID->enable = dumVap->enable;
}

static void s_startSyncOnEnable(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "pSSID is NULL");
    /* Start sync enable timer only when hostapd is enabled */
    ASSERT_TRUE(wld_secDmn_isEnabled(pRad->hostapd), , ME, "%s: secDmn not enabled", pAP->alias);
    mxl_VapVendorData_t* pVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendorData, , ME, "pVapVendorData is NULL");
    swl_chanspec_t chanspec = wld_rad_getSwlChanspec(pRad);
    uint32_t timeout = START_ENABLE_SYNC_TIMEOUT_MS;

    if (pRad->detailedState == CM_RAD_FG_CAC) {
        /* Extend timeout due to radio now in FG CAC clearing */
        timeout += wld_channel_get_band_clear_time(chanspec);
    }

    if (pAP->enable) {
        amxp_timer_state_t state = amxp_timer_get_state(pVapVendorData->onVapEnableSyncTimer);
        if ((state == amxp_timer_started ) || (state == amxp_timer_running)) {
            amxp_timer_stop(pVapVendorData->onVapEnableSyncTimer);
        }
        SAH_TRACEZ_INFO(ME, "%s: starting enable sync timer (timeout %u)", pAP->alias, timeout);
        amxp_timer_start(pVapVendorData->onVapEnableSyncTimer, timeout);
        pVapVendorData->vapEnableReloadPending = true;
    }
}

int whm_mxl_vap_enable(T_AccessPoint* pAP, int enable, int set) {
    int ret;
    SAH_TRACEZ_INFO(ME, "%s: vap enable %d --> %d - Set:%d", pAP->alias, pAP->enable, enable, set);
    CALL_NL80211_FTA_RET(ret, mfn_wvap_enable, pAP, enable, set);
    ASSERTS_FALSE(ret < 0, ret, ME, "%s: fail to enable AP flag 0x%x val %d", pAP->name, set, enable);
    if(set & SET) {
        pAP->enable = enable;
        T_Radio* pRad = (T_Radio*) pAP->pRadio;
        bool dumVapEnable = s_hasEnabledRealVap(pRad);
        s_enableDummyVap(pRad, dumVapEnable);
        if(pRad->operatingFrequencyBand == SWL_FREQ_BAND_EXT_5GHZ) {
            // enable ZwDfs dummy vap as well
            T_Radio* pZwDfsRadio = mxl_rad_getZwDfsRadio();
            T_AccessPoint* pZwDfsDumVap = wld_rad_getFirstVap(pZwDfsRadio);
            if((pZwDfsDumVap != NULL) && (!swl_str_matches(pRad->Name, pZwDfsRadio->Name))) {
                pZwDfsDumVap->enable = dumVapEnable;
                CALL_NL80211_FTA_RET(ret, mfn_wvap_enable, pZwDfsDumVap, pZwDfsDumVap->enable, set);
                wld_rad_doRadioCommit(pZwDfsRadio);
            }
            // Enable/disable background DFS for 5GHz main radio
            if(ret && !pRad->bgdfs_config.available) {
                SAH_TRACEZ_WARNING(ME, "%s: set background DFS available", pRad->Name);
                wld_bgdfs_setAvailable(pRad, true);
            }
        }
        /* When VAP is enabled dynamically - start sync enable timer and set reload pending */
        if (!(set & DIRECT)) {
            s_startSyncOnEnable(pAP);
        }
    }
    return ret;
}

int whm_mxl_vap_ssid(T_AccessPoint* pAP, char* buf, int bufsize, int set) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP NULL");
    T_SSID* pSSID = (T_SSID*) pAP->pSSID;
    ASSERTI_NOT_NULL(pSSID, SWL_RC_ERROR, ME, "pSSID is NULL");
    int rc = SWL_RC_OK;

    /* Set SSID internally first */
    if(set & SET) {
        swl_str_copy(pSSID->SSID, sizeof(pSSID->SSID), buf);
    } else {
        strncpy(buf, pSSID->SSID, bufsize);
    }

    /* Choose config flow */
    if ((set & SET) && (whm_mxl_chooseVapConfigFlow(pAP, WHM_MXL_CONFIG_TYPE_SSID) == WHM_MXL_CONFIG_FLOW_RECONF)) {
            SAH_TRACEZ_INFO(ME, "%s: SSID sync requesting reconf", pAP->alias);
            whm_mxl_vap_requestReconf(pAP);
    } else {
        /* Call generic implementation */
        SAH_TRACEZ_INFO(ME, "%s: Continue to generic SSID call", pAP->alias);
        CALL_NL80211_FTA_RET(rc, mfn_wvap_ssid, pAP, buf, bufsize, set);
    }

    return rc;
}

int whm_mxl_vap_wps_enable(T_AccessPoint* pAP, int enable, int set) {
    int rc = 0;
    if ((set & SET) && (whm_mxl_chooseVapConfigFlow(pAP, WHM_MXL_CONFIG_TYPE_SECURITY) == WHM_MXL_CONFIG_FLOW_RECONF)) {
        SAH_TRACEZ_INFO(ME, "%s: WPS enable requesting reconf", pAP->alias);
        whm_mxl_vap_requestReconf(pAP);
    } else {
        CALL_NL80211_FTA_RET(rc, mfn_wvap_wps_enable, pAP, enable, set);
    }
    return rc;
}

int whm_mxl_vap_bssid(T_Radio* pR, T_AccessPoint* pAP, unsigned char* buf, int bufsize, int set) {
    ASSERT_FALSE((pR == NULL) && (pAP == NULL), SWL_RC_INVALID_PARAM, ME, "NULL");
    T_SSID* pSSID;
    int rc = SWL_RC_OK;
    char* bssidStr[ETHER_ADDR_STR_LEN] = {0};

    if (set & SET) {
        /*when we change bssid on fly, pR will be NULL */
        if (pR == NULL && wld_secDmn_isAlive(pAP->pRadio->hostapd)) {
            pSSID = (T_SSID*) pAP->pSSID;
            SWL_MAC_BIN_TO_CHAR(bssidStr, pSSID->BSSID);
            wld_ap_hostapd_setParamValue(pAP, "bssid", (const char*)bssidStr, "set new bssid");
            whm_mxl_toggleHapd(pAP->pRadio);
            return rc;
        }
    }
    /* Call generic implementation */
    CALL_NL80211_FTA_RET(rc, mfn_wvap_bssid, pR, pAP, buf, bufsize, set);

    return rc;
}

int whm_mxl_vap_sec_sync(T_AccessPoint* pAP, int set) {
    int rc = 0;

    if ((set & SET) && (whm_mxl_chooseVapConfigFlow(pAP, WHM_MXL_CONFIG_TYPE_SECURITY) == WHM_MXL_CONFIG_FLOW_RECONF)) {
            SAH_TRACEZ_INFO(ME, "%s: Security sync requesting reconf", pAP->alias);
            whm_mxl_vap_requestReconf(pAP);
    } else {
        CALL_NL80211_FTA_RET(rc, mfn_wvap_sec_sync, pAP, set);
    }
    return rc;
}

int whm_mxl_vap_multiap_update_type(T_AccessPoint* pAP) {
    SAH_TRACEZ_IN(ME);
    ASSERTS_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, SWL_RC_INVALID_PARAM, ME, "mxlVapVendorData is NULL");
    uint32_t hapdMultiApType = 0;
    swl_rc_ne rc = SWL_RC_OK;

    /* Save multi AP type */
    if(SWL_BIT_IS_SET(pAP->multiAPType, MULTIAP_BACKHAUL_BSS)) {
        hapdMultiApType |= M_HOSTAPD_MULTI_AP_BBSS;
    }
    if(SWL_BIT_IS_SET(pAP->multiAPType, MULTIAP_FRONTHAUL_BSS)) {
        hapdMultiApType |= M_HOSTAPD_MULTI_AP_FBSS;
    }
    
    switch (hapdMultiApType) {
        case M_HOSTAPD_MULTI_AP_BBSS:
            mxlVapVendorData->mxlMultiApType = MXL_BACKHAUL_MAP;
            break;
        case M_HOSTAPD_MULTI_AP_FBSS:
            mxlVapVendorData->mxlMultiApType = MXL_FRONTHAUL_MAP;
            break;
        case M_HOSTAPD_MULTI_AP_HYBRID:
            mxlVapVendorData->mxlMultiApType = MXL_HYBRID_MAP;
            break;
        default:
            mxlVapVendorData->mxlMultiApType = MXL_MAP_OFF;
            break;
    }

    rc = whm_mxl_updateMultiAp(pAP);
    if (rc == SWL_RC_INVALID_STATE) {
        /* Call generic fta handler */
        CALL_NL80211_FTA_RET(rc, mfn_wvap_multiap_update_type, pAP);
    }
    SAH_TRACEZ_OUT(ME);
    return rc;
}

swl_rc_ne whm_mxl_vap_transfer_sta(T_AccessPoint* pAP, wld_transferStaArgs_t* params) {
    ASSERTS_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERTS_NOT_NULL(params, SWL_RC_INVALID_PARAM, ME, "NULL");
    T_Radio* pR = pAP->pRadio;
    ASSERTS_NOT_NULL(pR, SWL_RC_INVALID_PARAM, ME, "NULL");

    SAH_TRACEZ_INFO(ME, "%s: Send tranfer from %s to %s of %s", pR->Name, pAP->alias, params->targetBssid.cMac, params->sta.cMac);

    char cmd[256] = {'\0'};
    swl_str_catFormat(cmd, sizeof(cmd), "BSS_TM_REQ"
                      " %s"
                      " pref=%u abridged=%u"
                      , params->sta.cMac,
                      SWL_BIT_IS_SET(params->reqModeMask, SWL_IEEE802_BTM_REQ_MODE_PREF_LIST_INCL),
                      SWL_BIT_IS_SET(params->reqModeMask, SWL_IEEE802_BTM_REQ_MODE_ABRIDGED));
    if((params->transitionReason >= SWL_80211_WFA_MBO_TRANSITION_REASON_UNSPECIFIED) &&
       (params->transitionReason < SWL_80211_WFA_MBO_TRANSITION_REASON_MAX)) {
        swl_str_catFormat(cmd, sizeof(cmd),
                          " mbo=%d:%d:%d"  //mbo=<reason>:<reassoc_delay>:<cell_pref>
                          , params->transitionReason, (params->disassoc > 0) ? 100 : 0, 0);
    }
    if(params->disassoc > 0) {
        swl_str_catFormat(cmd, sizeof(cmd),
                          " disassoc_imminent=%u"
                          " disassoc_timer=%d"
                          , SWL_BIT_IS_SET(params->reqModeMask, SWL_IEEE802_BTM_REQ_MODE_DISASSOC_IMMINENT),
                          params->disassoc);
    }
    if(params->validity > 0) {
        //80211: validity interval 0 is a reserved value:
        //in this case, let hostapd/driver set the default number of beacon transmission times (TBTTs)
        //until the BSS transition candidate list is no longer valid
        swl_str_catFormat(cmd, sizeof(cmd),
                          " valid_int=%d"
                          , params->validity);
    }
    if(swl_mac_charIsValidStaMac(&params->targetBssid)) {
        swl_str_catFormat(cmd, sizeof(cmd),
                          " neighbor=%s"
                          ",%u,%d,%d,%d"   //<bssidInfo>,<operClass>,<channel>,<phyType>
                          , params->targetBssid.cMac
                          , params->bssidInfo, params->operClass, params->channel, swl_chanspec_operClassToPhyMode(params->operClass));
        if(SWL_BIT_IS_SET(params->reqModeMask, SWL_IEEE802_BTM_REQ_MODE_PREF_LIST_INCL)) {
            //add highest preference for the bss candidate: Tlv: Len:3,candidate:1,pref:255
            swl_str_catFormat(cmd, sizeof(cmd), ",0301ff");
        }
    }

    bool ret = whm_mxl_wpaCtrl_sendCmdCheckSpecificResponse(pAP, cmd, "bss transition management", "dialog_token=");
    ASSERT_TRUE(ret, SWL_RC_ERROR, ME, "%s: btm from %s to %s of station %s failed", pR->Name, pAP->alias, params->targetBssid.cMac, params->sta.cMac);
    return SWL_RC_OK;
}

/*
 * By default, inactive client is cleaned from WiFi driver (& hostapd) tables after inactivity polling
 * interval (60 sec by default).
 * In case that upper layer prefers to take responsibility of it, the following conditions must be met:
 * 1. The apMaxInactivity should be configured to max value (86400 sec = 24 H).
 *    This will assure that the internal WiFi driver mechanism will not interfere with the external one.
 * 2. The default value can be changed at build time in mod-whm-mxl_definition.odl for all APs at once.
 * 3. Otherwise, it can be changed at runtime per every AP using the following command:
 *    WiFi.AccessPoint.{}.Vendor.ApMaxInactivity=86400
 * 4. The upper layer app should manage an independedt inactivity polling for every client.
 * 5. In case that a client is inactive, the upper layer app should use the cleanStation API
 *    which will reach the below function: whm_mxl_vap_clean_sta().
 */
int whm_mxl_vap_clean_sta(T_AccessPoint* pAP, char* macStr, int macStrLen) {
    /* WiFi.AccessPoint.{}.cleanStation(macaddress=11:22:33:44:55:66) */
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is null");
    ASSERT_EQUALS(macStrLen, SWL_MAC_CHAR_LEN - 1, SWL_RC_INVALID_PARAM, ME, "macStrLen is invalid [%d]", macStrLen);
    ASSERT_TRUE(swl_mac_charIsValidStaMac((swl_macChar_t*) macStr), SWL_RC_INVALID_PARAM, ME, "macStr is invalid");

    char cmd[128] = {0};
    swl_str_catFormat(cmd, sizeof(cmd), "DEAUTHENTICATE %s tx=0", macStr);
    SAH_TRACEZ_INFO(ME, "%s: calling hostapd %s", pAP->alias, cmd);
    bool ret = whm_mxl_hostapd_sendCommand(pAP, cmd, "cleanStation DEAUTHENTICATE");
    /* If hostapd command failed (e.g. VAP not up or not exist etc.), print a warning but return ok code. */
    ASSERTW_TRUE(ret, SWL_RC_OK, ME, "%s: failed to execute 'deauthenticate' command for client %s", pAP->alias, macStr);
    SAH_TRACEZ_NOTICE(ME, "%s: cleanStation %s done", pAP->alias, macStr);
    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

amxd_status_t _whm_mxl_updateQoSMap(amxd_object_t* object,
                                    amxd_function_t* func _UNUSED,
                                    amxc_var_t* args _UNUSED,
                                    amxc_var_t* retval) {
    SAH_TRACEZ_IN(ME);

    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERTI_NOT_NULL(pAP, amxd_status_ok, ME, "No pAP mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, amxd_status_invalid_value, ME, "mxlVapVendorData is NULL");
    const char* qos_map_set = GET_CHAR(args, "QoSMap");

    if(!swl_str_isEmpty(qos_map_set)) {
        amxd_object_set_value(cstring_t, object, "QoSMap", mxlVapVendorData->QoSMap);
    }

    SAH_TRACEZ_OUT(ME);
    amxc_var_set(bool, retval, true);
    return amxd_status_ok;
}

amxd_status_t _whm_mxl_vap_validateNumResSta_pvf(amxd_object_t* object,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    /* WiFi.AccessPoint.{}.Vendor */
    T_AccessPoint* pAP = wld_ap_fromObj(amxd_object_get_parent(object));
    ASSERTI_NOT_NULL(pAP, amxd_status_ok, ME, "No pAP mapped");
    ASSERTI_TRUE((pAP->MaxStations >= 0), amxd_status_ok, ME, "MaxStations not ready yet to validate with");
    int newVal = amxc_var_dyncast(int32_t, args);
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, amxd_status_ok, ME, "mxlVapVendorData not mapped yet");

    if ((newVal <= pAP->MaxStations) || (newVal == 0)) {
        return amxd_status_ok;
    }
    SAH_TRACEZ_ERROR(ME, "%s: NumResSta should be less or equal to MaxNumStations[%d]", pAP->alias, pAP->MaxStations);
    return amxd_status_invalid_value;
}

amxd_status_t _whm_mxl_vap_validateFilsUpr_pvf(amxd_object_t* object,
                                           amxd_param_t* param,
                                           amxd_action_t reason _UNUSED,
                                           const amxc_var_t* const args,
                                           amxc_var_t* const retval _UNUSED,
                                           void* priv _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_status_t status = amxd_status_ok;
    uint32_t newVal = amxc_var_dyncast(uint32_t, args);
    ASSERTS_TRUE(newVal, status, ME, "Disabling UPR/FILS always allowed");
    const char* paramName = amxd_param_get_name(param);
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, amxd_status_ok, ME, "No AccessPoint Mapped");
    amxd_object_t* pVendorObj = amxd_object_get(pAP->pBus, "Vendor");
    ASSERT_NOT_NULL(pVendorObj, amxd_status_ok, ME, "pVendorObj NULL");

    if(swl_str_matches(paramName, "UnsolBcastProbeRespInterval")) {
        if(amxd_object_get_value(uint32_t, pVendorObj, "FilsDiscoveryMaxInterval", NULL) > 0) {
         status = amxd_status_invalid_value;
         SAH_TRACEZ_ERROR(ME, "UnsolBcastProbeRespInterval and FilsDiscoveryMaxInterval cannot be both above 0");
        }
    }
    else if(swl_str_matches(paramName, "FilsDiscoveryMaxInterval")) {
        if(amxd_object_get_value(uint32_t, pVendorObj, "UnsolBcastProbeRespInterval", NULL) > 0) {
         status = amxd_status_invalid_value;
         SAH_TRACEZ_ERROR(ME, "UnsolBcastProbeRespInterval and FilsDiscoveryMaxInterval cannot be both above 0");
        }
    }
    SAH_TRACEZ_OUT(ME);

    return status;
}

amxd_status_t _whm_mxl_vap_validateManagementFramesRate_pvf(amxd_object_t* object _UNUSED,
                                           amxd_param_t* param _UNUSED,
                                           amxd_action_t reason _UNUSED,
                                           const amxc_var_t* const args,
                                           amxc_var_t* const retval _UNUSED,
                                           void* priv _UNUSED) {
    int newVal = amxc_var_dyncast(int32_t, args);
    if ((newVal >= MGMT_FRAMES_RATE_ENABLED_MIN && newVal <= MGMT_FRAMES_RATE_ENABLED_MAX) || newVal == MGMT_FRAMES_RATE_DEFAULT) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}

amxd_status_t _whm_mxl_vap_validateOweTransSsid_pvf(amxd_object_t* object _UNUSED,
                                            amxd_param_t* param _UNUSED,
                                            amxd_action_t reason _UNUSED,
                                            const amxc_var_t* const args,
                                            amxc_var_t* const retval _UNUSED,
                                            void* priv _UNUSED) {
    amxd_status_t status = amxd_status_invalid_value;
    char* newSsidValue = amxc_var_dyncast(cstring_t, args);
    if (swl_str_isEmpty(newSsidValue) || (strlen(newSsidValue) < SSID_NAME_LEN)) {
        status = amxd_status_ok;
    } else {
        SAH_TRACEZ_ERROR(ME, "invalid OWE Transition SSID(%s)", newSsidValue);
    }
    free(newSsidValue);
    return status;
}

amxd_status_t _whm_mxl_vap_validateRadiusSecret_pvf(amxd_object_t* object _UNUSED,
                                            amxd_param_t* param _UNUSED,
                                            amxd_action_t reason _UNUSED,
                                            const amxc_var_t* const args,
                                            amxc_var_t* const retval _UNUSED,
                                            void* priv _UNUSED) {
    amxd_status_t status = amxd_status_invalid_value;
    char* newRadiusSecret = amxc_var_dyncast(cstring_t, args);
    if (swl_str_isEmpty(newRadiusSecret) || ((strlen(newRadiusSecret) <= 64) && (strlen(newRadiusSecret) >= 8))) {
        status = amxd_status_ok;
    } else {
        SAH_TRACEZ_ERROR(ME, "invalid Radius Secret Key(%s)", newRadiusSecret);
    }
    free(newRadiusSecret);
    return status;
}

static void s_setUnsolBcastPrbRespInterval_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    uint32_t unsolBcastPrbRespInterval = amxc_var_dyncast(uint32_t, newParamValues);
    char unsolValStr[MAX_NUM_OF_DIGITS] = {0};
    
    /* Sync only for 6G band */
    if(wld_rad_is_6ghz(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s was set on %s to %d", amxd_param_get_name(param), pRad->Name, amxc_var_dyncast(uint32_t, &param->value));
        swl_str_catFormat(unsolValStr, sizeof(unsolValStr), "%u",unsolBcastPrbRespInterval);
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), unsolValStr);
    } else {
        SAH_TRACEZ_INFO(ME, "%s is not relevant for %s (6G only), configuration will not be applied", amxd_param_get_name(param), pRad->Name);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setFilsDiscoveryMaxInterval_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    uint32_t filsDiscoveryMaxInterval = amxc_var_dyncast(uint32_t, newParamValues);
    char filsValStr[MAX_NUM_OF_DIGITS] = {0};

    /* Sync only for 6G band */
    if(wld_rad_is_6ghz(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s was set on %s to %d", amxd_param_get_name(param), pRad->Name, amxc_var_dyncast(uint32_t, &param->value));
        swl_str_catFormat(filsValStr, sizeof(filsValStr), "%u", filsDiscoveryMaxInterval);
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), filsValStr);
    } else {
        SAH_TRACEZ_INFO(ME, "%s is not relevant for %s (6G only), configuration will not be applied", amxd_param_get_name(param), pRad->Name);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setHairpin_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool enableHairpin = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (enableHairpin ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setApMaxInactivity_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int apMaxInactivity = amxc_var_dyncast(int32_t, newParamValues);
    char apMaxInactivityStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(apMaxInactivityStr, sizeof(apMaxInactivityStr), "%d", apMaxInactivity);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), apMaxInactivityStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setBssTransition_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int bssTransition = amxc_var_dyncast(int32_t, newParamValues);
    char bssTransitionStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(bssTransitionStr, sizeof(bssTransitionStr), "%d", bssTransition);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), bssTransitionStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setManagementFramesRate_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int managementFramesRate = amxc_var_dyncast(uint32_t, newParamValues);
    char managementFramesRateStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(managementFramesRateStr, sizeof(managementFramesRateStr), "%d", managementFramesRate);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), managementFramesRateStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setMgmtFramePowerControl_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int mgmtFramePowerControl = amxc_var_dyncast(int32_t, newParamValues);
    char mgmtFramePowerControlStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(mgmtFramePowerControlStr, sizeof(mgmtFramePowerControlStr), "%d", mgmtFramePowerControl);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), mgmtFramePowerControlStr);
    if(mxl_isApReadyToProcessVendorCmd(pAP)) {
        uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_MGMT_FRAME_PWR_CTRL;
        int rc = wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, &mgmtFramePowerControl, sizeof(mgmtFramePowerControl), VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
        if(rc < SWL_RC_OK) {
            SAH_TRACEZ_ERROR(ME, "%s: NL80211 ManagementFramePowerControl failed", pAP->alias); 
        }
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setNumResSta_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int numResSta = amxc_var_dyncast(int32_t, newParamValues);
    char numResStaStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(numResStaStr, sizeof(numResStaStr), "%d", numResSta);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), numResStaStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setVendorVht_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    bool vendorVht = amxc_var_dyncast(bool, newParamValues);
    mxlVapVendorData->vendorVht = vendorVht;
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (vendorVht ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setOWETransitionBSSID_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* oweTransitionBSSID = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->OWETransBSSID, sizeof(mxlVapVendorData->OWETransBSSID), oweTransitionBSSID);
    /* NULL value is provided because value will be applied via conf writing and sighup*/
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    free(oweTransitionBSSID);
    SAH_TRACEZ_OUT(ME);
}

static void s_setOWETransitionSSID_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* oweTransitionSSID = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->OWETransSSID, sizeof(mxlVapVendorData->OWETransSSID), oweTransitionSSID);
    /* NULL value is provided because value will be applied via conf writing and sighup*/
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    free(oweTransitionSSID);
    SAH_TRACEZ_OUT(ME);
}

static void s_setDynamicMulticast_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int paramValue = amxc_var_dyncast(uint32_t, newParamValues);
    char dynamicMulticastStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(dynamicMulticastStr, sizeof(dynamicMulticastStr), "%d", paramValue);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), dynamicMulticastStr);

    SAH_TRACEZ_OUT(ME);
}

static void s_setDisableBeaconProtection_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool disableBeaconProt = amxc_var_dyncast(bool, newParamValues);
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_BE)) {
      whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (disableBeaconProt ? "1" : "0"));
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setDisablePbac_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool disablePbac = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (disablePbac ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setBridgeMode_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool setBridgeModeNewVal = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (setBridgeModeNewVal ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setBssLoad_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool setBssLoadNewVal = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (setBssLoadNewVal ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setMboCellAware_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues){
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int paramValue = amxc_var_dyncast(int32_t, newParamValues);

    char MboCellAwareStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(MboCellAwareStr, sizeof(MboCellAwareStr), "%d", paramValue);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), MboCellAwareStr);

    SAH_TRACEZ_OUT(ME);
}

static void s_setSaeExtKey_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    bool saeExtKey = amxc_var_dyncast(bool, newParamValues);
    mxlVapVendorData->saeExtKey = saeExtKey;
    /* Override wpa_key_mgmt to SAE-EXT-KEY to support AKM24 */
    whm_mxl_configureSaeExt(pAP);
    SAH_TRACEZ_OUT(ME);
}

static void s_setEnableWPA3PersonalCompatibility_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    swl_rc_ne rc;

    bool EnableWPA3PersonalCompatibility = amxc_var_dyncast(bool, newParamValues);
    mxlVapVendorData->EnableWPA3PersonalCompatibility = EnableWPA3PersonalCompatibility;
    if (EnableWPA3PersonalCompatibility == true) {
        SAH_TRACEZ_INFO(ME, "Enabling WPA3 Personal Compatibility Mode for %s, DON'T trust security mode set in PWHM!", pAP->alias);
        whm_mxl_toggleWPA3PersonalCompatibility(pAP);
    } else {
        SAH_TRACEZ_INFO(ME, "Disabling WPA3 Personal Compatibility Mode for %s, reverting back to the security mode set in PWHM!", pAP->alias);
        CALL_NL80211_FTA_RET(rc, mfn_wvap_sec_sync, pAP, SET);
        wld_autoCommitMgr_notifyVapEdit(pAP);
        whm_mxl_toggleWPA3PersonalCompatibility(pAP);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setAggrConfig_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* aggrConfig = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->AggrConfig, sizeof(mxlVapVendorData->AggrConfig), aggrConfig);
    /* NULL value is provided because value will be applied via conf writing and sighup */
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    }
    free(aggrConfig);
    SAH_TRACEZ_OUT(ME);
}

static void s_setGroupMgmtCipher_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* cipher = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->GroupMgmtCipher, sizeof(mxlVapVendorData->GroupMgmtCipher), cipher);
    /* NULL value is provided because value will be applied via conf writing and sighup */
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    }
    free(cipher);
    SAH_TRACEZ_OUT(ME);
}

static void s_setGroupCipher_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* group_cipher = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->GroupCipher, sizeof(mxlVapVendorData->GroupCipher), group_cipher);
    /* NULL value is provided because value will be applied via conf writing and sighup */
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    }
    free(group_cipher);
    SAH_TRACEZ_OUT(ME);
}

static void s_setQoSMap_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* qos_map_set = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->QoSMap, sizeof(mxlVapVendorData->QoSMap), qos_map_set);
    /* NULL value is provided because value will be applied via conf writing and sighup */
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    }
    free(qos_map_set);
    SAH_TRACEZ_OUT(ME);
}

static void s_setH2eRequired_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    if(whm_mxl_isCertModeEnabled()) {
        bool newVal = amxc_var_dyncast(bool, newParamValues);
        mxlVapVendorData->h2eRequired = newVal;
        SAH_TRACEZ_WARNING(ME, "It is setting the H2E val %d", newVal);
        whm_mxl_toggleHapd(pAP->pRadio);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setRadiusSecretKey_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char* radiusSecret = amxc_var_dyncast(cstring_t, newParamValues);
    swl_str_copy(mxlVapVendorData->radiusSecretKey, sizeof(mxlVapVendorData->radiusSecretKey), radiusSecret);
    /* NULL value is provided because value will be applied via conf writing and sighup */
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), NULL);
    }
    free(radiusSecret);
    SAH_TRACEZ_OUT(ME);
}

static void s_set11nProtection_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int paramValue = amxc_var_dyncast(uint32_t, newParamValues);
    char set11nProtectionStr[MAX_NUM_OF_DIGITS] = {0};
    swl_str_catFormat(set11nProtectionStr, sizeof(set11nProtectionStr), "%d", paramValue);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), set11nProtectionStr);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setEmlCapabTransitionTimeout_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int paramValue = amxc_var_dyncast(uint32_t, newParamValues);
    char emlCapabTransitionTimeoutStr[MAX_NUM_OF_DIGITS] = {0};
    swl_str_catFormat(emlCapabTransitionTimeoutStr, sizeof(emlCapabTransitionTimeoutStr), "%d", paramValue);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), emlCapabTransitionTimeoutStr);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setBoolVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool newValue = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (newValue ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setBooleanCertVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    bool newValue = amxc_var_dyncast(bool, newParamValues);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (newValue ? "1" : "0"));
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setInt32CertVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor */
    amxd_object_t* vapObj = amxd_object_get_parent(object);
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    int32_t paramValue = amxc_var_dyncast(int32_t, newParamValues);
    char paramValueStr[MAX_NUM_OF_DIGITS] = {0};
    swl_str_catFormat(paramValueStr, sizeof(paramValueStr), "%d", paramValue);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), paramValueStr);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sVapVendorDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("ManagementFramesRate", s_setManagementFramesRate_pwf),
                  SWLA_DM_PARAM_HDLR("MgmtFramePowerControl", s_setMgmtFramePowerControl_pwf),
                  SWLA_DM_PARAM_HDLR("EnableHairpin", s_setHairpin_pwf),
                  SWLA_DM_PARAM_HDLR("ApMaxInactivity", s_setApMaxInactivity_pwf),
                  SWLA_DM_PARAM_HDLR("UnsolBcastProbeRespInterval", s_setUnsolBcastPrbRespInterval_pwf),
                  SWLA_DM_PARAM_HDLR("FilsDiscoveryMaxInterval", s_setFilsDiscoveryMaxInterval_pwf),
                  SWLA_DM_PARAM_HDLR("BssTransition", s_setBssTransition_pwf),
                  SWLA_DM_PARAM_HDLR("VendorVht", s_setVendorVht_pwf),
                  SWLA_DM_PARAM_HDLR("NumResSta", s_setNumResSta_pwf),
                  SWLA_DM_PARAM_HDLR("OWETransitionBSSID", s_setOWETransitionBSSID_pwf),
                  SWLA_DM_PARAM_HDLR("OWETransitionSSID", s_setOWETransitionSSID_pwf),
                  SWLA_DM_PARAM_HDLR("DynamicMulticastMode", s_setDynamicMulticast_pwf),
                  SWLA_DM_PARAM_HDLR("DynamicMulticastRate", s_setDynamicMulticast_pwf),
                  SWLA_DM_PARAM_HDLR("DisableBeaconProtection", s_setDisableBeaconProtection_pwf),
                  SWLA_DM_PARAM_HDLR("DisablePbac", s_setDisablePbac_pwf),
                  SWLA_DM_PARAM_HDLR("SetBridgeMode", s_setBridgeMode_pwf),
                  SWLA_DM_PARAM_HDLR("EnableBssLoad", s_setBssLoad_pwf),
                  SWLA_DM_PARAM_HDLR("MboCellAware", s_setMboCellAware_pwf),
                  SWLA_DM_PARAM_HDLR("SaeExtKey", s_setSaeExtKey_pwf),
                  SWLA_DM_PARAM_HDLR("EnableWPA3PersonalCompatibility", s_setEnableWPA3PersonalCompatibility_pwf),
                  SWLA_DM_PARAM_HDLR("SetAggrConfig", s_setAggrConfig_pwf),
                  SWLA_DM_PARAM_HDLR("Set11nProtection", s_set11nProtection_pwf),
                  SWLA_DM_PARAM_HDLR("EmlCapabTransitionTimeout", s_setEmlCapabTransitionTimeout_pwf),
                  SWLA_DM_PARAM_HDLR("ApProtectedKeepAliveRequired", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MldMediumsyncPresent", s_setBooleanCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MldMediumsyncDuration", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MldMediumsyncOfdmedthresh", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MldMediumsyncMaxtxop", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacEpcsPrioAccess", s_setBooleanCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MloT2lmSupport", s_setBooleanCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("GroupMgmtCipher", s_setGroupMgmtCipher_pwf),
                  SWLA_DM_PARAM_HDLR("RadiusSecretKey", s_setRadiusSecretKey_pwf),
                  SWLA_DM_PARAM_HDLR("RrmNeighRpt", s_setBooleanCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WnmBssTransQueryAutoresp", s_setBooleanCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVICWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVOCWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBECWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBKCWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVICWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVOCWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBECWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBKCWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVIAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVOAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBEAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBKAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVITXOP", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVOTXOP", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBETXOP", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBKTXOP", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVIAcm", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcVOAcm", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBEAcm", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("WmmAcBKAcm", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVICWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVOCWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBECWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBKCWMin", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVICWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVOCWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBECWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBKCWMax", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVIAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueVOAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBEAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("TxQueueBKAifs", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("GroupCipher", s_setGroupCipher_pwf),
                  SWLA_DM_PARAM_HDLR("GasCBDelay", s_setInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("H2eRequired", s_setH2eRequired_pwf),
                  SWLA_DM_PARAM_HDLR("SCSEnable", s_setBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MSCSEnable", s_setBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("QoSMap", s_setQoSMap_pwf))
              );

void _whm_mxl_vap_setVapVendorObj_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {                          
    swla_dm_procObjEvtOfLocalDm(&sVapVendorDmHdlrs, sig_name, data, priv);
}

static void s_setMultiApClientDis_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.MultiAp */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    int multiApProfileDis = amxc_var_dyncast(int32_t, newParamValues);
    char multiApClientDisStr[MAX_NUM_OF_DIGITS] = {0};

    if ((mxlVapVendorData->mxlMultiApType == MXL_HYBRID_MAP) || (mxlVapVendorData->mxlMultiApType == MXL_BACKHAUL_MAP)) {
        swl_str_catFormat(multiApClientDisStr, sizeof(multiApClientDisStr), "%d", multiApProfileDis);
        whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), multiApClientDisStr);
    } else {
        SAH_TRACEZ_INFO(ME, "%s: Incorrect multi AP mode (%d)", pAP->alias, mxlVapVendorData->mxlMultiApType);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sVendorMutliApDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("MultiApProfile2Dis", s_setMultiApClientDis_pwf))
              );

void _whm_mxl_vendorMultiAp_setConf_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sVendorMutliApDmHdlrs, sig_name, data, priv);
}

static void s_setMloBoolFlag_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.MLO */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    bool mloBoolParam = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (mloBoolParam ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setMloMacAddr_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.MLO */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    const char* mloMacAddr = amxc_var_constcast(cstring_t, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), mloMacAddr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setMloID_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.MLO */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    int32_t newMloId = amxc_var_get_int32_t(newParamValues);
    if (newMloId == -1 && !(mxlVapVendorData->MLO_destroyInProgress) && whm_mxl_isMLAssociatedVap(pAP)) {
        whm_mxl_destroyMLVap(pAP);
        return;
    }
    mxlVapVendorData->mloId = newMloId;
    whm_mxl_hostapd_setMldParams(pAP);
    char mloIdValStr[MAX_NUM_OF_DIGITS] = {0};
    swl_str_catFormat(mloIdValStr, sizeof(mloIdValStr), "%d", newMloId);
    int32_t numLinks = whm_mxl_getNumMLlinksbyID(newMloId);
    ASSERT_FALSE((numLinks > 2), , ME, "MLO: MLO_ID %d, is already utilized by 2 AP objects", newMloId);
    T_AccessPoint* sibBSS = whm_mxl_getSiblingBss(pAP, newMloId);
    ASSERT_NOT_NULL(sibBSS, , ME, "MLO: No Sibling BSS Found returning");
    SAH_TRACEZ_INFO(ME, "MLO: Sibling BSS Found: %s", sibBSS->alias);
    /* API to Validate MLO Vap Configuration and Do Interface Toggle */
    whm_mxl_createMLVap(sibBSS, pAP);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sVendorMLODmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("MloId", s_setMloID_pwf),
                  SWLA_DM_PARAM_HDLR("ApMldMac", s_setMloMacAddr_pwf),
                  SWLA_DM_PARAM_HDLR("WdsSingleMlAssoc", s_setMloBoolFlag_pwf),
                  SWLA_DM_PARAM_HDLR("WdsPrimaryLink", s_setMloBoolFlag_pwf))
              );

void _whm_mxl_vendorMLO_setConf_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sVendorMLODmHdlrs, sig_name, data, priv);
}

static void s_setSoftBlockAclEnable_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.SoftBlock */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    bool softBlockAclEnable = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (softBlockAclEnable ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setSoftBlockAclWaitTime_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.SoftBlock */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    uint32_t SoftBlockAclWaitTime = amxc_var_dyncast(uint32_t, newParamValues);
    char SoftBlockAclWaitTimeStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(SoftBlockAclWaitTimeStr, sizeof(SoftBlockAclWaitTimeStr), "%u", SoftBlockAclWaitTime);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), SoftBlockAclWaitTimeStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setSoftBlockAclAllowTime_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.SoftBlock */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    uint32_t SoftBlockAclAllowTime = amxc_var_dyncast(uint32_t, newParamValues);
    char SoftBlockAclAllowTimeStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(SoftBlockAclAllowTimeStr, sizeof(SoftBlockAclAllowTimeStr), "%u", SoftBlockAclAllowTime);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), SoftBlockAclAllowTimeStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_SoftBlockAclOnAuthReq_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.SoftBlock */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    int SoftBlockAclOnAuthReq = amxc_var_dyncast(int32_t, newParamValues);
    char SoftBlockAclOnAuthReqStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(SoftBlockAclOnAuthReqStr, sizeof(SoftBlockAclOnAuthReqStr), "%d", SoftBlockAclOnAuthReq);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), SoftBlockAclOnAuthReqStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setSoftBlockAclOnProbeReq_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.SoftBlock */
    amxd_object_t* vapObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    int SoftBlockAclOnProbeReq = amxc_var_dyncast(int32_t, newParamValues);
    char SoftBlockAclOnProbeReqStr[MAX_NUM_OF_DIGITS] = {0};

    swl_str_catFormat(SoftBlockAclOnProbeReqStr, sizeof(SoftBlockAclOnProbeReqStr), "%d", SoftBlockAclOnProbeReq);
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), SoftBlockAclOnProbeReqStr);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sVendorSoftBlockDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("SoftBlockAclEnable", s_setSoftBlockAclEnable_pwf),
                  SWLA_DM_PARAM_HDLR("SoftBlockAclWaitTime", s_setSoftBlockAclWaitTime_pwf),
                  SWLA_DM_PARAM_HDLR("SoftBlockAclAllowTime", s_setSoftBlockAclAllowTime_pwf),
                  SWLA_DM_PARAM_HDLR("SoftBlockAclOnAuthReq", s_SoftBlockAclOnAuthReq_pwf),
                  SWLA_DM_PARAM_HDLR("SoftBlockAclOnProbeReq", s_setSoftBlockAclOnProbeReq_pwf))
              );

void _whm_mxl_vendorSoftBlock_setConf_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sVendorSoftBlockDmHdlrs, sig_name, data, priv);
}


bool mxl_isApReadyToProcessVendorCmd(T_AccessPoint* pAP)
{
    ASSERT_NOT_NULL(pAP, false, ME, "NULL");
    return wld_linuxIfUtils_getLinkStateExt(pAP->alias) == 1;
}

static swl_rc_ne whm_mxl_vap_supvendModesChanged(T_AccessPoint* pAP, amxd_object_t* object, amxc_var_t* params _UNUSED) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(object, SWL_RC_INVALID_PARAM, ME, "object is NULL");
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, SWL_RC_INVALID_PARAM, ME, "mxlVapVendorData is NULL");

    if (swl_str_matches(amxd_object_get_name(object, AMXD_OBJECT_NAMED), "Vendor")) {
        /* Save pointer to AccessPoint.Vendor object */
        mxlVapVendorData->pVendorBus = object;
    }
    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

void s_selectAccessPoint(amxd_object_t* const object, int32_t depth _UNUSED, void* priv) {
    ASSERTS_EQUALS(amxd_object_get_type(object), amxd_object_instance, , ME, "Not instance");
    amxd_object_t* parent = amxd_object_get_parent(object);
    ASSERTS_TRUE(swl_str_matches(amxd_object_get_name(parent, AMXD_OBJECT_NAMED), "AccessPoint"), , ME, "Not under AccessPoint");
    ASSERTS_EQUALS(amxd_object_get_parent(parent), get_wld_object(), , ME, "Not under WiFi");
    T_AccessPoint** ppAP = (T_AccessPoint**) priv;
    ASSERTS_NOT_NULL(ppAP, , ME, "ppAp is NULL");
    *ppAP = object->priv;
}

void _whm_mxl_vap_setVendorData_ocf(const char* const sig_name _UNUSED,
                                const amxc_var_t* const data _UNUSED,
                                void* const priv _UNUSED) {
    amxd_dm_t* dm = get_wld_plugin_dm();
    amxd_object_t* object = amxd_dm_signal_get_object(dm, data);
    T_AccessPoint* pAP = NULL;
    amxd_object_hierarchy_walk(object, amxd_direction_up, NULL, s_selectAccessPoint, INT32_MAX, &pAP);
    ASSERT_TRUE(debugIsVapPointer(pAP), , ME, "NO AccessPoint Ctx");
    SAH_TRACEZ_INFO(ME, "%s: %s AP vendor data (%s)", pAP->alias, sig_name, GET_CHAR(data, "object"));
    whm_mxl_vap_supvendModesChanged(pAP, object, GET_ARG(data, "parameters"));
}

void _whm_mxl_vap_updateMaxAssociatedDevices(const char* const sig_name _UNUSED,
                                             const amxc_var_t* const data,
                                             void* const priv _UNUSED) {
    amxd_object_t* object = amxd_dm_signal_get_object(get_wld_plugin_dm(), data);
    ASSERTS_NOT_NULL(object, , ME, "object is NULL");
    T_AccessPoint* pAP = (T_AccessPoint*) object->priv;
    ASSERTS_NOT_NULL(pAP, , ME, "pAP is NULL");
    SAH_TRACEZ_NOTICE(ME, "%s: MaxAssociatedDevices changed event", pAP->alias);
    whm_mxl_updateOnEventMaxAssociatedDevices(pAP);
}

static swl_rc_ne s_get20MHzTxPowerCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    int* txPower = (int*) priv;
    ASSERT_NOT_NULL(txPower, SWL_RC_ERROR, ME, "txPower is NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if (nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    int* tmpTxPower = (int*) data;
    ASSERT_NOT_NULL(tmpTxPower, SWL_RC_ERROR, ME, "tmpTxPower is NULL");
    *txPower = *tmpTxPower;

    return rc;
}

static swl_rc_ne s_getNeighborPsd(T_AccessPoint* pNeighAp, uint8_t* psd) {
    ASSERT_NOT_NULL(pNeighAp, SWL_RC_INVALID_PARAM, ME, "pNeighAp is NULL");
    ASSERT_NOT_NULL(psd, SWL_RC_INVALID_PARAM, ME, "psd is NULL");
    T_Radio* pNeighRad = pNeighAp->pRadio;
    ASSERT_NOT_NULL(pNeighRad, SWL_RC_INVALID_PARAM, ME, "pNeighRad is NULL");
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pNeighRad);
    ASSERTS_NOT_NULL(masterVap, SWL_RC_INVALID_PARAM, ME, "masterVap is NULL");
    int tmpPsd = 0;
    int tx_power_20mhz = 0;
    swl_rc_ne rc = SWL_RC_OK;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_20MHZ_TX_POWER;

    ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(masterVap), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");
    rc = wld_ap_nl80211_sendVendorSubCmd(masterVap, OUI_MXL, subcmd, masterVap->pSSID->Name, strlen(masterVap->pSSID->Name),
                                         VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_get20MHzTxPowerCb, &tx_power_20mhz);
    ASSERT_TRUE((rc == SWL_RC_OK), SWL_RC_ERROR, ME, "Failed in call LTQ_NL80211_VENDOR_SUBCMD_GET_20MHZ_TX_POWER");

    /* Adjust received 20MHz TX power to the correct units of PSD subfield */
    tmpPsd = (tx_power_20mhz - MXL_HAPD_6GHZ_10LOG_20MHZ ) * MXL_HAPD_6GHZ_CONVERT_HALF_DB_UNIT;

    ASSERT_FALSE(((tmpPsd < MXL_HAPD_6GHZ_PSD_20MHZ_MIN) || (tmpPsd > MXL_HAPD_6GHZ_PSD_20MHZ_MAX)), SWL_RC_ERROR, ME,
                                                                "PSD is out of range (%d)", tmpPsd);
    if (tmpPsd < 0) {
        tmpPsd = tmpPsd + MXL_HAPD_BYTE_2S_COMPLEMENT;
    }

    *psd = (u8) tmpPsd;
    return rc;
}

static swl_rc_ne s_whm_mxl_setNeighborPsd(T_AccessPoint* pAP, T_ApNeighbour* pApNeighbor) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    ASSERT_NOT_NULL(pApNeighbor, SWL_RC_INVALID_PARAM, ME, "pApNeighbor is NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERTS_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    bool has11rFToDsEnabled = (pRad->IEEE80211rSupported && pAP->IEEE80211rEnable && pAP->IEEE80211rFTOverDSEnable);
    bool has11kNeighReportEnabled = (pRad->IEEE80211kSupported && pAP->IEEE80211kEnable);
    char cmd[256] = {0};
    uint8_t psd = 0;
    swl_rc_ne rc = SWL_RC_OK;

    if (has11rFToDsEnabled || has11kNeighReportEnabled) {
        /* Take pointer to 6G AP */
        T_AccessPoint* pColocAP = wld_ap_getVapByBssid((swl_macBin_t*) pApNeighbor->bssid);
        ASSERT_NOT_NULL(pColocAP, SWL_RC_INVALID_PARAM, ME, "pColocAP is NULL");
        ASSERT_TRUE(wld_rad_is_6ghz(pColocAP->pRadio), SWL_RC_INVALID_PARAM, ME, "%s: Not a 6G neighbor", pColocAP->alias);
        ASSERT_TRUE(pApNeighbor->colocatedAp, SWL_RC_INVALID_PARAM, ME, "%s: Not a co-located neighbor AP", pColocAP->alias);
        if (!swl_rc_isOk(s_getNeighborPsd(pColocAP, &psd))) {
            SAH_TRACEZ_ERROR(ME, "s_getNeighborPsd failed");
            return SWL_RC_ERROR;
        }
        /* Prepare message to hostapd */
        swl_str_catFormat(cmd, sizeof(cmd), "SET_NEIGHBOR_PSD %s psd_subfield=%d",
                                                swl_typeMacBin_toBuf32Ref((swl_macBin_t*) pApNeighbor->bssid).buf,
                                                (psd & 0xff));
        SAH_TRACEZ_INFO(ME, "sending cmd : %s", cmd);
        bool ret = whm_mxl_hostapd_sendCommand(pAP, cmd, "set_neighbor_psd");
        ASSERT_TRUE(ret, SWL_RC_ERROR, ME, "%s: set_neighbor_psd command failed", pAP->alias);
    }

    return rc;
}

swl_rc_ne whm_mxl_vap_updated_neighbor(T_AccessPoint* pAP, T_ApNeighbour* pApNeighbor) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    ASSERT_NOT_NULL(pApNeighbor, SWL_RC_INVALID_PARAM, ME, "pApNeighbor is NULL");
    swl_rc_ne rc;

    /* Call generic implementation first */
    CALL_NL80211_FTA_RET(rc, mfn_wvap_updated_neighbour, pAP, pApNeighbor);

    /* Set psd subfield for 6G neighbors */
    if (!swl_rc_isOk(s_whm_mxl_setNeighborPsd(pAP, pApNeighbor))) {
        SAH_TRACEZ_NOTICE(ME, "%s: s_whm_mxl_setNeighborPsd failed", pAP->alias);
    }

    SAH_TRACEZ_OUT(ME);
    return rc;
}

/**
 * @brief Request to reconf BSS from reconf FSM
 *
 * @param pAP accesspoint
 * @return none.
 */
void whm_mxl_vap_requestReconf(T_AccessPoint* pAP) {
    ASSERTI_NOT_NULL(pAP, , ME, "NULL");
    mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendor, , ME, "pVapVendor is NULL");
    setBitLongArray(pVapVendor->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF_BSS);
    whm_mxl_reconfMngr_notifyVapCommit(pAP);
}

swl_rc_ne whm_mxl_vap_postUpActions(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_VapVendorData_t* pVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendorData, SWL_RC_INVALID_PARAM, ME, "pVapVendorData is NULL");

    if (pVapVendorData->vapEnableReloadPending) {
        setBitLongArray(pVapVendorData->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_RELOAD_BSS);
        whm_mxl_reconfMngr_notifyVapCommit(pAP);
        pVapVendorData->vapEnableReloadPending = false;
    }

    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_vap_postDownActions(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    /* Add post VAP down actions here */
    return SWL_RC_OK;
}

static void s_setIgnore11vDiassoc_pwf(void* priv _UNUSED,
                              amxd_object_t* object _UNUSED,
                              amxd_param_t* param _UNUSED,
                              const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.AccessPoint.{}.Vendor.Steering */
    amxd_object_t* vendorObj = amxd_object_get_parent(object);
    ASSERT_NOT_NULL(vendorObj, , ME, "No Vendor Object found");
    amxd_object_t* vapObj = amxd_object_get_parent(vendorObj);
    ASSERT_NOT_NULL(vapObj, , ME, "No Vap Object found");
    T_AccessPoint* pAP = wld_ap_fromObj(vapObj);
    ASSERT_NOT_NULL(pAP, , ME, "No AccessPoint Mapped");

    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    bool ignoreDaTimer = amxc_var_dyncast(bool, newValue);

    mxlVapVendorData->ignore11vDiassoc = ignoreDaTimer;
    whm_mxl_determineVapParamAction(pAP, amxd_param_get_name(param), (ignoreDaTimer ? "1" : "0"));

    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sSteeringDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("Ignore11vDiassoc", s_setIgnore11vDiassoc_pwf))
             );

void _whm_mxl_vendorSteering_setConf_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sSteeringDmHdlrs, sig_name, data, priv);
}