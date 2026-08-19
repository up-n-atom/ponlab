/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_ep.c                                          *
*         Description  : Endpoint related API                                  *
*                                                                              *
*  *****************************************************************************/

#include <swl/swl_common.h>
#include <swla/swla_mac.h>

#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_endpoint.h"

#include "whm_mxl_utils.h"
#include "whm_mxl_ep.h"
#include "whm_mxl_cfgActions.h"
#include "whm_mxl_dmnMngr.h"

#define ME "mxlEp"


static swl_rc_ne s_getPeerFlowStatusCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    T_EndPointStats* stats = (T_EndPointStats*) priv;
    ASSERT_NOT_NULL(stats, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);
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
    stats->LastDataDownlinkRate = peerFlowStats->tr181_stats.LastDataDownlinkRate;
    stats->LastDataUplinkRate = peerFlowStats->tr181_stats.LastDataUplinkRate;
    stats->RSSI = peerFlowStats->tr181_stats.SignalStrength;
    stats->txRetries = peerFlowStats->tr181_stats.retrans_stats.RetryCount;

    return rc;
}

static swl_rc_ne s_getPeerFlowStatus(T_EndPoint* pEP, T_EndPointStats* stats) {
    swl_rc_ne rc;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_FLOW_STATUS;
    rc = wld_rad_nl80211_sendVendorSubCmd(pEP->pRadio, OUI_MXL, subcmd, pEP->pSSID->BSSID, ETHER_ADDR_LEN,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getPeerFlowStatusCb, stats);
    return rc;
}

static swl_rc_ne s_getPeerCapabilitiesCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    T_EndPointStats* stats = (T_EndPointStats*) priv;
    ASSERT_NOT_NULL(stats, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);
    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }
    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    mtlk_wssa_drv_peer_capabilities_t* peerCapabilities = (mtlk_wssa_drv_peer_capabilities_t*) data;
    ASSERT_NOT_NULL(peerCapabilities, SWL_RC_ERROR, ME, "NULL");
    stats->assocCaps.htCapabilities |= (peerCapabilities->SGI20Supported ? M_SWL_STACAP_HT_SGI20 : 0);
    stats->assocCaps.htCapabilities |= (peerCapabilities->SGI40Supported ? M_SWL_STACAP_HT_SGI40 : 0);
    stats->assocCaps.htCapabilities |= (peerCapabilities->Intolerant_40MHz ? M_SWL_STACAP_HT_40MHZ_INTOL : 0);

    return rc;
}

static swl_rc_ne s_getPeerCapabilities(T_EndPoint* pEP, T_EndPointStats* stats) {
    swl_rc_ne rc;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_CAPABILITIES;
    rc = wld_rad_nl80211_sendVendorSubCmd(pEP->pRadio, OUI_MXL, subcmd, pEP->pSSID->BSSID, ETHER_ADDR_LEN,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getPeerCapabilitiesCb, stats);
    return rc;
}

static swl_rc_ne s_getPerClientStatsCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    T_EndPointStats* stats = (T_EndPointStats*) priv;
    ASSERT_NOT_NULL(stats, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);
    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }
    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    perClientStats_t* perClientStats = (perClientStats_t*) data;
    ASSERT_NOT_NULL(perClientStats, SWL_RC_ERROR, ME, "NULL");
    stats->rxRetries = perClientStats->mpduRetryCount;

    return rc;
}

static swl_rc_ne s_getPerClientStats(T_EndPoint* pEP, T_EndPointStats* stats) {
    swl_rc_ne rc;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_PER_CLIENT_STATS;
    rc = wld_rad_nl80211_sendVendorSubCmd(pEP->pRadio, OUI_MXL, subcmd, pEP->pSSID->BSSID, ETHER_ADDR_LEN,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getPerClientStatsCb, stats);
    return rc;
}

int whm_mxl_epStats(T_EndPoint* pEP, T_EndPointStats* stats) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(stats, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;

    CALL_NL80211_FTA_RET(rc, mfn_wendpoint_stats, pEP, stats);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    ASSERTI_NOT_NULL(pEP->pSSID, SWL_RC_OK, ME, "%s: No mapped ssid", pEP->Name);
    ASSERTI_FALSE(swl_mac_binIsNull((swl_macBin_t*) pEP->pSSID->BSSID),
                  SWL_RC_OK, ME, "%s: seems not connected (no remote bssid)", pEP->Name);

    // execute LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_FLOW_STATUS to get
    // - LastDataDownlinkRate
    // - LastDataUplinkRate
    // - RSSI
    // - Tx_Retransmissions
    rc = s_getPeerFlowStatus(pEP, stats);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get peer flow status");

    // execute LTQ_NL80211_VENDOR_SUBCMD_GET_PER_CLIENT_STATS to get
    // - Rx_Retransmissions
    rc = s_getPerClientStats(pEP, stats);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get per client stats");

    // execute LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_CAPABILITIES to get
    // - HtCapabilities
    rc = s_getPeerCapabilities(pEP, stats);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get peer capabilities");

    return rc;
}

int whm_mxl_ep_createHook(T_EndPoint* pEP) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wendpoint_create_hook, pEP);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");
    // register wpa_supplicant secDmn custom handlers
    return whm_mxl_dmnMngr_setWpaSuppArgsHanlder(pEP);
}

int whm_mxl_ep_destroyHook(T_EndPoint* pEP) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wendpoint_destroy_hook, pEP);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_WPASUPPLICANT);
    ASSERT_NOT_NULL(pDmnCtx, SWL_RC_INVALID_PARAM, ME, "pDmnCtx is NULL");
    pDmnCtx->initPending = true;
    whm_mxl_dmnMngr_setDmnCtxState(pDmnCtx, MXL_SECDMN_STATE_RST);
    return rc;
}

swl_rc_ne whm_mxl_ep_enable(T_EndPoint* pEP, bool enable) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "pEP is NULL");
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wendpoint_enable, pEP, enable);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");
    return rc;
}

static void s_setEpWds_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.EndPoint.{}.Vendor */
    amxd_object_t* epObj = amxd_object_get_parent(object);
    T_EndPoint* pEP = wld_ep_fromObj(epObj);
    ASSERT_NOT_NULL(pEP, , ME, "pEP is NULL");
    T_Radio* pRad = pEP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "pRad NULL");
    if (pRad->isWDS) {
        whm_mxl_determineEpParamAction(pEP, amxd_param_get_name(param));
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setVendorElements_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.EndPoint.{}.Vendor */
    amxd_object_t* epObj = amxd_object_get_parent(object);
    T_EndPoint* pEP = wld_ep_fromObj(epObj);
    ASSERT_NOT_NULL(pEP, , ME, "pEP is NULL");
    whm_mxl_determineEpParamAction(pEP, amxd_param_get_name(param));
    SAH_TRACEZ_OUT(ME);
}

static void s_setMultiApProfile_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.EndPoint.{}.Vendor */
    amxd_object_t* epObj = amxd_object_get_parent(object);
    T_EndPoint* pEP = wld_ep_fromObj(epObj);
    ASSERT_NOT_NULL(pEP, , ME, "pEP is NULL");
    whm_mxl_determineEpParamAction(pEP, amxd_param_get_name(param));
    SAH_TRACEZ_OUT(ME);
}

static void s_setWpsCredAddSae_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.EndPoint.{}.Vendor */
    amxd_object_t* epObj = amxd_object_get_parent(object);
    T_EndPoint* pEP = wld_ep_fromObj(epObj);
    ASSERT_NOT_NULL(pEP, , ME, "pEP is NULL");
    whm_mxl_determineEpParamAction(pEP, amxd_param_get_name(param));
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sEpVendorDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("Wds", s_setEpWds_pwf),
                  SWLA_DM_PARAM_HDLR("VendorElements", s_setVendorElements_pwf),
                  SWLA_DM_PARAM_HDLR("MultiApProfile", s_setMultiApProfile_pwf),
                  SWLA_DM_PARAM_HDLR("WpsCredAddSae", s_setWpsCredAddSae_pwf))
              );

void _whm_mxl_ep_setEpVendorObj_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sEpVendorDmHdlrs, sig_name, data, priv);
}
