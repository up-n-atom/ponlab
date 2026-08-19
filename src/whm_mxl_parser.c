/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_parser.c                                      *
*         Description  : parser functions for vendor subcmds & evts structs    *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"

#include <nl80211_copy.h>

#include "wld/wld_radio.h"
#include "wld/wld_util.h"
#include "wld/wld_rad_stamon.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_nl80211_utils.h"
#include "wld/wld_nl80211_attr.h"
#include "wld/wld_nl80211_events.h"

#include "whm_mxl_utils.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_csi.h"
#include "whm_mxl_monitor.h"
#include <vendor_cmds_copy.h>

#define ME "mxlPars"

const char* s_reqToStr(uint8_t reqType) {
    switch (reqType) {
        case NASTA_STATS_REQ_ASYNC: return "async";
        case NASTA_STATS_REQ_SYNC: return "sync";
        default: return "unknown";
    }
}

swl_rc_ne mxl_parseNaStaStats(T_Radio* pRad, struct nlattr* tb[], uint8_t reqType, bool syncDm) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(tb, SWL_RC_INVALID_PARAM, ME, "NULL");
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    struct intel_vendor_unconnected_sta* nasta = (struct intel_vendor_unconnected_sta*) data;
    ASSERT_NOT_NULL(nasta, SWL_RC_ERROR, ME, "NULL");

    swl_macChar_t nastaMacStr = SWL_MAC_CHAR_NEW();
    ASSERT_TRUE(SWL_MAC_BIN_TO_CHAR(&nastaMacStr, nasta->addr), SWL_RC_INVALID_PARAM, ME, "invalid event mac");
    T_NonAssociatedDevice* pMD = wld_rad_staMon_getNonAssociatedDevice(pRad, nastaMacStr.cMac);
    ASSERTS_NOT_NULL(pMD, SWL_RC_OK, ME, "%s: unknown nasta mac %s", pRad->Name, nastaMacStr.cMac);

    SAH_TRACEZ_INFO(ME, "%s: received %s nasta event for %s", pRad->Name,
                                                    s_reqToStr(reqType),
                                                    nastaMacStr.cMac);

    uint16_t curRate = nasta->rate;
    int16_t bestRssi = -200;
    for(uint32_t i = 0; i < SWL_ARRAY_SIZE(nasta->rssi); i++) {
        SAH_TRACEZ_INFO(ME, "rssi[%d] = %d", i, nasta->rssi[i]);
        int8_t tempRssi = nasta->rssi[i];
        if((tempRssi != 0) && (tempRssi != -128) && (tempRssi > bestRssi)) {
            bestRssi = tempRssi;
        }
    }
    if(bestRssi != -200) {
        pMD->SignalStrength = bestRssi;
        pMD->TimeStamp = swl_time_getMonoSec();
    }
    SAH_TRACEZ_INFO(ME, "%s: %s bestRssi %d, savedRssi %d, rate %dMbps", pRad->Name, nastaMacStr.cMac, bestRssi, pMD->SignalStrength, curRate);

    if (syncDm) {
        whm_mxl_monitor_updateNaStaObj(pRad);
    }

    mxl_nastaEntryData_t* pEntry = whm_mxl_monitor_fetchRunNaStaEntry(pRad, (swl_macBin_t*) nasta->addr);
    if(pEntry) {
        SAH_TRACEZ_INFO(ME, "del executed runNaSta entry %s", nastaMacStr.cMac);
        whm_mxl_monitor_delRunNaStaEntry(pEntry);
    }

    return SWL_RC_OK;
}

swl_rc_ne mxl_parseChanDataEvt(T_Radio* pRad, struct nlattr* tb[]) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(tb, SWL_RC_INVALID_PARAM, ME, "NULL");
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    struct intel_vendor_channel_data* chanData = (struct intel_vendor_channel_data*) data;
    ASSERT_NOT_NULL(chanData, SWL_RC_ERROR, ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: chan %d , freq %d MHz", pRad->Name, chanData->channel, chanData->freq);
    return SWL_RC_OK;
}

swl_rc_ne mxl_parseCsiStatsEvt(T_Radio* pRad, struct nlattr* tb[]) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(tb, SWL_RC_INVALID_PARAM, ME, "NULL");
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    wifi_csi_driver_nl_event_data_t* csiStats = (wifi_csi_driver_nl_event_data_t*) data;
    ASSERT_NOT_NULL(csiStats, SWL_RC_ERROR, ME, "NULL");
    mxl_rad_sendCsiStatsOverUnixSocket(csiStats);
    return SWL_RC_OK;
}

static void s_parseWiphyDfsAntenna(struct nlattr* tb[], mxl_VendorWiphyInfo_t* vendorInfo) {
    ASSERTS_FALSE(vendorInfo->wiphyDfsAntenna, , ME, "DFS phy already detected");
    if(tb[NL80211_ATTR_WIPHY_DFS_ANTENNA]) {
        SAH_TRACEZ_INFO(ME, "phy%d is meant for DFS only", vendorInfo->wiphyId);
        vendorInfo->wiphyDfsAntenna = true;
    }
}

swl_rc_ne mxl_parseWiphyInfo(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERTS_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERTS_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    mxl_VendorWiphyInfo_t* vendorInfo = (mxl_VendorWiphyInfo_t*) priv;
    ASSERTS_NOT_NULL(vendorInfo, SWL_RC_ERROR, ME, "No request data");

    // parse the netlink message
    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTS_EQUALS(gnlh->cmd, NL80211_CMD_NEW_WIPHY, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);
    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // check gen ID attribute
    ASSERTS_NOT_NULL(tb[NL80211_ATTR_GENERATION], SWL_RC_ERROR, ME, "missing genId");

    // check wiphy ID attribute
    uint32_t wiphyId = wld_nl80211_getWiphy(tb);
    ASSERTS_NOT_EQUALS(wiphyId, WLD_NL80211_ID_UNDEF, SWL_RC_OK, ME, "unexpected wiphy ID");
    ASSERTS_EQUALS(wiphyId, vendorInfo->wiphyId, SWL_RC_OK, ME, "unexpected wiphy ID");

    // parse DFS antenna vendor attribute
    s_parseWiphyDfsAntenna(tb, vendorInfo);

    return SWL_RC_OK;
}
