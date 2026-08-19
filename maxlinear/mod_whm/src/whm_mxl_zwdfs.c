/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_zwdfs.c                                       *
*         Description  : Radio ZWDFS related API                               *
*                                                                              *
*  *****************************************************************************/

#include <swla/swla_mac.h>

#include "swl/swl_common.h"
#include "swl/map/swl_mapCharFmt.h"
#include "swla/swla_chanspec.h"

#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_chanmgt.h"
#include "wld/wld_bgdfs.h"
#include "wld/wld_rad_hostapd_api.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_eventing.h"

#include "whm_mxl_utils.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_cfgActions.h"

#include <vendor_cmds_copy.h>

#define ME "mxlZwd"

static T_Radio* s_pZwDfsRad = NULL;

static swl_rc_ne s_getZwDfsAntennaCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERT_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    uint32_t* bgDfsEnable = (uint32_t*) priv;

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "failed to parse netlink message");
        return SWL_RC_ERROR;
    }
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    *bgDfsEnable = nla_get_u32(tb[NL80211_ATTR_VENDOR_DATA]);

    SAH_TRACEZ_INFO(ME, "ZwDfs antenna is currently %s", *bgDfsEnable ? "enabled" : "disabled");
    return rc;
}

static swl_rc_ne s_getZwDfsAntenna(uint32_t* bgDfsEnable) {
    ASSERT_NOT_NULL(s_pZwDfsRad, WLD_ERROR_INVALID_PARAM, ME, "ZwDfs radio NULL");
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_ZWDFS_ANT;
    return wld_rad_nl80211_sendVendorSubCmd(s_pZwDfsRad, OUI_MXL, subcmd, NULL, 0,
                                            VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getZwDfsAntennaCb, bgDfsEnable);
}

static swl_rc_ne s_setZwDfsAntenna(uint8_t enable) {
    ASSERT_NOT_NULL(s_pZwDfsRad, WLD_ERROR_INVALID_PARAM, ME, "ZwDfs radio NULL");
    uint32_t bgDfsEnable = 0;
    swl_rc_ne rc = s_getZwDfsAntenna(&bgDfsEnable);
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "request error");
    ASSERT_TRUE(bgDfsEnable != enable, rc, ME, "%s: ZwDfs antenna is already %s", s_pZwDfsRad->Name, enable ? "enabled" : "disabled");

    SAH_TRACEZ_INFO(ME, "%s: %s ZwDfs antenna", s_pZwDfsRad->Name, enable ? "Enable" : "Disable");
    uint8_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_ZWDFS_ANT;
    return wld_rad_nl80211_sendVendorSubCmd(s_pZwDfsRad, OUI_MXL, subcmd, &enable, sizeof(uint8_t),
                                            VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
}

int whm_mxl_rad_bgDfsEnable(T_Radio* pRad _UNUSED, int enable) {
    int rc = s_setZwDfsAntenna(enable);

    /* In case BackgroundCac is enabled and PreclearEnable parameter enabled by user (no protection in this case), disable BackgroundCac */
    amxd_object_t* pVendorObj =  amxd_object_findf(pRad->pBus, "Vendor");
    ASSERT_NOT_NULL(pVendorObj, rc, ME, "pVendorObj is NULL");
    bool backgroundCac = amxd_object_get_value(bool, pVendorObj, "BackgroundCac", NULL);
    if (backgroundCac && pRad->bgdfs_config.enable) {
        SAH_TRACEZ_ERROR(ME, "%s: BackgroundCac and PreclearEnable can't be both enabled, disabling BackgroundCac", pRad->Name);
        amxd_trans_t trans;
        ASSERT_TRANSACTION_INIT(pVendorObj, &trans, rc, ME, "%s : trans init failure", pRad->Name);
        amxd_trans_set_bool(&trans, "BackgroundCac", false);
        ASSERT_TRANSACTION_LOCAL_DM_END(&trans, rc, ME, "%s : trans apply failure", pRad->Name);
    }

    return rc;
}

static int s_zwdfsSwitchChannel(wld_startBgdfsArgs_t* args) {
    ASSERT_NOT_NULL(s_pZwDfsRad, WLD_ERROR_INVALID_PARAM, ME, "ZwDfs radio NULL");
    ASSERT_TRUE(wld_channel_is_dfs_band(args->channel, args->bandwidth), SWL_RC_ERROR, ME,
                "%s chan %u/%u not dfs", s_pZwDfsRad->Name, args->channel, args->bandwidth);
    ASSERT_TRUE(wld_secDmn_isRunning(s_pZwDfsRad->hostapd), SWL_RC_INVALID_STATE, ME,
                "%s: hapd not running", s_pZwDfsRad->Name);
    ASSERT_TRUE(wld_wpaCtrlMngr_ping(s_pZwDfsRad->hostapd->wpaCtrlMngr), SWL_RC_INVALID_STATE, ME,
                "%s: hapd not ready", s_pZwDfsRad->Name);

    /* following the initial requirement, CHAN_SWITCH is needed to start BG CAC using the ZW DFS radio */
    s_pZwDfsRad->channelChangeReason = CHAN_REASON_MANUAL;
    swl_chanspec_t chanspec = SWL_CHANSPEC_NEW(args->channel, args->bandwidth, s_pZwDfsRad->operatingFrequencyBand);
    s_pZwDfsRad->channel = args->channel;
    s_pZwDfsRad->targetChanspec.chanspec = chanspec;
    s_pZwDfsRad->targetChanspec.reason = CHAN_REASON_MANUAL;
    s_pZwDfsRad->targetChanspec.changeTime = swl_time_getMonoSec();
    memset(s_pZwDfsRad->targetChanspec.reasonExt, 0, sizeof(s_pZwDfsRad->targetChanspec.reasonExt));
    s_pZwDfsRad->channelShowing = CHANNEL_INTERNAL_STATUS_CURRENT;
    return wld_rad_hostapd_switchChannel(s_pZwDfsRad);
}

static swl_rc_ne s_bgDfsStart(T_Radio* pRad, int channel, wld_startBgdfsArgs_t* args) {
    SAH_TRACEZ_INFO(ME, "%s: ZwDfs started", pRad->Name);
    swl_rc_ne rc = s_setZwDfsAntenna(1);
    ASSERT_EQUALS(rc, SWL_RC_OK, rc, ME, "fail to enable zwdfs antenna");

    if(args) {
        return s_zwdfsSwitchChannel(args);
    }
    wld_startBgdfsArgs_t dfsArgs;
    dfsArgs.channel = channel;
    dfsArgs.bandwidth = swl_bandwidth_defaults[pRad->operatingFrequencyBand];
    return s_zwdfsSwitchChannel(&dfsArgs);
}

static swl_rc_ne s_bgDfsStop(T_Radio* pRad) {
    SAH_TRACEZ_INFO(ME, "%s: ZwDfs stopped", pRad->Name);
    s_setZwDfsAntenna(0);
    return SWL_RC_DONE;
}

int whm_mxl_rad_bgDfsStart(T_Radio* pRad, int channel) {
    ASSERTS_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");
    swl_rc_ne rc = s_bgDfsStart(pRad, channel, NULL);
    return (rc < SWL_RC_OK) ? SWL_RC_ERROR : SWL_RC_DONE;
}

int whm_mxl_rad_bgDfsStartExt(T_Radio* pRad, wld_startBgdfsArgs_t* args) {
    ASSERTS_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");
    swl_rc_ne rc = s_bgDfsStart(pRad, 0, args);
    return (rc < SWL_RC_OK) ? SWL_RC_ERROR : SWL_RC_DONE;
}

int whm_mxl_rad_bgDfs_stop(T_Radio* pRad) {
    ASSERTS_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");
    return s_bgDfsStop(pRad);
}

int mxl_rad_addZwDfsRadio(const char* name, vendor_t* vendor, int index) {
    s_pZwDfsRad = wld_createRadio(name, vendor, index);
    if(s_pZwDfsRad) {
        SAH_TRACEZ_WARNING(ME, "%s: radInit vendor %s, index %u", s_pZwDfsRad->Name, vendor->name, index);
        return SWL_RC_OK;
    } else {
        SAH_TRACEZ_WARNING(ME, "failed to create %s radio instance", name);
        return SWL_RC_ERROR;
    }
}

void mxl_rad_deleteZwDfsRadio() {
    amxc_llist_for_each(it, &s_pZwDfsRad->llAP) {
        T_AccessPoint* pAP = wld_ap_fromIt(it);
        wld_ap_destroy(pAP);
    }
    wld_deleteRadioObj(s_pZwDfsRad);
}

T_Radio* mxl_rad_getZwDfsRadio() {
    return s_pZwDfsRad;
}

void mxl_rad_updateZwDfsRegDomain(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad pointer is NULL");
    T_Radio* pRadZwDfs = mxl_rad_getZwDfsRadio();
    ASSERT_NOT_NULL(pRadZwDfs, , ME, "ZWDFS Radio pointer is NULL");
    T_AccessPoint* primaryVap = wld_rad_firstAp(pRadZwDfs);
    ASSERT_NOT_NULL(primaryVap, , ME, "ZWDFS primaryVap is NULL");
    wld_hostapd_config_t* config = NULL;
    wld_hostapd_loadConfig(&config, pRadZwDfs->hostapd->cfgFile);
    swl_mapChar_t* zwdfsConfigMap = wld_hostapd_getConfigMap(config, NULL);
    ASSERT_NOT_NULL(zwdfsConfigMap, , ME, "ZWDFS Radio hostapd configMap pointer is NULL");
    char* countryCode = (char*) swl_mapChar_get(zwdfsConfigMap, "country_code");
    /* If the ZWDFS value differs from the 5G value: send SET command and toggle the ZWDFS vap */
    if (!swl_str_matches(countryCode, pRad->regulatoryDomain)) {
        char cmd[256] = {0};
        swl_mapCharFmt_addValStr(zwdfsConfigMap, "country_code", "%s", pRad->regulatoryDomain);
        /* Prepare message to hostapd */
        swl_str_catFormat(cmd, sizeof(cmd), "SET country_code %s", pRad->regulatoryDomain);
        SAH_TRACEZ_INFO(ME, "sending cmd : %s", cmd);
        bool ret = whm_mxl_hostapd_sendCommand(primaryVap, cmd, "SET country_code");
        if (!ret) {
            SAH_TRACEZ_WARNING(ME, "ZWDFS: SET country_code command failed");
            wld_hostapd_deleteConfig(config);
            return;
        }
        whm_mxl_toggleHapd(pRadZwDfs);
    }
    wld_hostapd_deleteConfig(config);
}
