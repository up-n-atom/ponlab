/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_rad.c                                         *
*         Description  : Radio related API                                     *
*                                                                              *
*  *****************************************************************************/

#include <swla/swla_mac.h>

#include "swl/swl_common.h"
#include "swla/swla_chanspec.h"
#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "wld/wld_chanmgt.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_ap_nl80211.h"
#include "wld/wld_linuxIfUtils.h"
#include "wld/wld_rad_hostapd_api.h"
#include "wld/wld_eventing.h"

#include "whm_mxl_module.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_monitor.h"
#include "whm_mxl_evt.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_cfgActions.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_wmm.h"
#include "whm_mxl_reconfMngr.h"

#include <vendor_cmds_copy.h>

#define ME "mxlRad"

/* Global Translation Tables */
SWL_TABLE(s_powerTranslationTable,
    ARR(int8_t powerPercent; uint8_t powerVal; ),
    ARR(swl_type_int8, swl_type_uint8, ),
    ARR(
        {6,  12},
        {12,  9},
        {25,  6},
        {50,  3},
        {75,  1},
        {100, 0}
    )
);

int8_t *whm_mxl_rad_txPercentToPower(uint8_t percent)
{
    return (int8_t*)swl_table_getMatchingValue(&s_powerTranslationTable, 1, 0, &percent);
}

uint8_t *whm_mxl_rad_txPowerToPercent(int8_t power)
{
    return (uint8_t*)swl_table_getMatchingValue(&s_powerTranslationTable, 0, 1, &power);
}

mxl_VendorData_t* mxl_rad_getVendorData(const T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, NULL, ME, "NULL");
    return (mxl_VendorData_t*) pRad->vendorData;
}

static void s_mxl_rad_init_vendordata(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    whm_mxl_monitor_init(pRad);
    whm_mxl_rad_delVap_timer_init(pRad);
}

int whm_mxl_rad_supports(T_Radio* pRad, char* buf _UNUSED, int bufsize _UNUSED) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;

    CALL_NL80211_FTA_RET(rc, mfn_wrad_supports, pRad, buf, bufsize);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");
    /*
    The generic mfn_wrad_supports will reset the capabilities during radio init,
    Any additional capabilities from vendor plugin should be added after the generic call
    */

    /* Set OWE as supported for all bands */
    if (wld_rad_is_24ghz(pRad)) {
        wld_rad_addSuppDrvCap(pRad, SWL_FREQ_BAND_2_4GHZ, "OWE");
    } else if (wld_rad_is_5ghz(pRad)) {
        wld_rad_addSuppDrvCap(pRad, SWL_FREQ_BAND_5GHZ, "OWE");
    } else if (wld_rad_is_6ghz(pRad)) {
        wld_rad_addSuppDrvCap(pRad, SWL_FREQ_BAND_6GHZ, "OWE");
    }

    /*  
     *  Mark support for MBSSID - Driver does not populate NL80211_MBSSID_CONFIG_ATTR_MAX_INTERFACES
     *  so we will directly mark that we support MBSSID so that the generic MBSSID handling can be used
     *  i.e. deleting conf section of disabled VAPs when MBSSID is enabled on that radio
     */
    if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AX)) {
        W_SWL_BIT_SET(pRad->suppMbssidAdsModes, MBSSID_ADVERTISEMENT_MODE_ON);
        SAH_TRACEZ_INFO(ME, "%s: Set support MBSSID_ADVERTISEMENT_MODE_ON", pRad->Name);
    }

    // set vendor events handler after nl80211Listener is created (ie when radio wiphyId is known: after successful wrad_support)
    SAH_TRACEZ_INFO(ME, "%s: Set vendor event handler", pRad->Name);
    rc = mxl_evt_setVendorEvtHandlers(pRad);

    return rc;
}

static void s_vapStatusCb(wld_vap_status_change_event_t* event) {
    ASSERT_NOT_NULL(event, , ME, "NULL");
    T_AccessPoint* pAP = event->vap;
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    T_SSID* pSSID = pAP->pSSID;
    ASSERT_NOT_NULL(pSSID, , ME, "NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");

    if ((pAP->status == APSTI_ENABLED) && (pSSID->status == RST_UP)) {
        whm_mxl_vap_postUpActions(pAP);
    } else if (pSSID->status == RST_DOWN) {
        whm_mxl_vap_postDownActions(pAP);
    }
}

static wld_event_callback_t s_vapStatusEventCb = {
    .callback = (wld_event_callback_fun) s_vapStatusCb,
};

int whm_mxl_rad_createHook(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wrad_create_hook, pRad);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    pRad->vendorData = calloc(1, sizeof(mxl_VendorData_t));
    ASSERT_NOT_NULL(pRad->vendorData, SWL_RC_INVALID_PARAM, ME, "NULL");
    s_mxl_rad_init_vendordata(pRad);

    whm_mxl_reconfMngr_init(pRad);

    // set vendor events handler
    SAH_TRACEZ_INFO(ME, "%s: Set vendor event handler", pRad->Name);
    rc = mxl_evt_setVendorEvtHandlers(pRad);

    // Register to event queues
    wld_event_add_callback(gWld_queue_vap_onStatusChange, &s_vapStatusEventCb);
    whm_mxl_registerToWdsEvent();

    return rc;
}

static bool s_isHapdDisableRequired(chanmgt_rad_state radDetailedState) {
    return ((radDetailedState == CM_RAD_UP) || (radDetailedState == CM_RAD_FG_CAC) || (radDetailedState == CM_RAD_CONFIGURING));
}

static void s_syncOnRadDynamicEnable(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    bool isHapdReady = wld_secDmn_isRunning(pRad->hostapd) && wld_rad_firstCommitFinished(pRad);
    bool isSyncAllowed = (whm_mxl_utils_numOfGrpMembers(pRad) > 1);
    ASSERTI_TRUE((isHapdReady && isSyncAllowed), , ME, "%s: sync conditions (%d / %d)", pRad->Name, isHapdReady, isSyncAllowed);
    SAH_TRACEZ_INFO(ME, "%s: rad dynamically enabled - schedule sync", pRad->Name);
    whm_mxl_rad_setCtrlSockSyncNeeded(pRad, true);
    whm_mxl_rad_requestSync(pRad);
}

int whm_mxl_rad_enable(T_Radio* pRad, int val, int set) {
    int ret = val;
    chanmgt_rad_state radDetState = CM_RAD_UNKNOWN;
    swl_rc_ne rc;
    if((set & SET) && !(set & DIRECT)) {
        if (val) {
            s_syncOnRadDynamicEnable(pRad);
        }
    }
    if((set & DIRECT) && (set & SET)) {
        // let hostapd/wpa_supp manage the main iface enabling
        if(!val) {
            SAH_TRACEZ_INFO(ME, "%s: rad enable %d", pRad->Name, val);
            wld_linuxIfUtils_setState(wld_rad_getSocket(pRad), pRad->Name, false);
            if (wld_secDmn_isRunning(pRad->hostapd)) {
                rc = whm_mxl_hapd_getRadState(pRad, &radDetState);
                if (swl_rc_isOk(rc) && s_isHapdDisableRequired(radDetState)) {
                    // explicitly disable hostapd to sync with driver state of the interface
                    wld_rad_hostapd_disable(pRad);
                }
            }
        }
    } else {
        CALL_NL80211_FTA_RET(ret, mfn_wrad_enable, pRad, val, set);
    }
    return ret;
}

static void s_deinitRadVendorData(T_Radio* pRad) {
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
    if (vendorData->acs_exclusion_ch_list) {
        free(vendorData->acs_exclusion_ch_list);
    }
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
    free(vendorData);
}

void whm_mxl_rad_destroyHook(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    wld_event_remove_callback(gWld_queue_vap_onStatusChange, &s_vapStatusEventCb);
    whm_mxl_unregisterToWdsEvent();
    whm_mxl_reconfMngr_deinit(pRad);
    whm_mxl_rad_delVap_timer_deinit(pRad);
    whm_mxl_monitor_deinit(pRad);
    s_deinitRadVendorData(pRad);
    CALL_NL80211_FTA(mfn_wrad_destroy_hook, pRad);
}

static swl_rc_ne s_getVAPStatsCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_Stats* stats = (T_Stats*) priv;
    ASSERT_NOT_NULL(stats, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    size_t len;
    mtlk_wssa_drv_tr181_wlan_stats_t *drvStats;

    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    len = nla_len(tb[NL80211_ATTR_VENDOR_DATA]);
    ASSERT_EQUALS(sizeof(*drvStats), len, SWL_RC_ERROR, ME, "Wrong data size %zu received, expected %zu", len, sizeof(*drvStats));

    drvStats = data;
    ASSERT_NOT_NULL(drvStats, SWL_RC_ERROR, ME, "NULL");

    stats->BytesSent                   += drvStats->traffic_stats.BytesSent;
    stats->BytesReceived               += drvStats->traffic_stats.BytesReceived;
    stats->PacketsSent                 += drvStats->traffic_stats.PacketsSent;
    stats->PacketsReceived             += drvStats->traffic_stats.PacketsReceived;
    stats->ErrorsSent                  += drvStats->error_stats.ErrorsSent;
    stats->RetransCount                += drvStats->retrans_stats.RetransCount;
    stats->ErrorsReceived              += drvStats->error_stats.ErrorsReceived;
    stats->DiscardPacketsSent          += drvStats->error_stats.DiscardPacketsSent;
    stats->DiscardPacketsReceived      += drvStats->error_stats.DiscardPacketsReceived;
    stats->UnicastPacketsSent          += drvStats->traffic_stats.UnicastPacketsSent;
    stats->UnicastPacketsReceived      += drvStats->traffic_stats.UnicastPacketsReceived;
    stats->MulticastPacketsSent        += drvStats->traffic_stats.MulticastPacketsSent;
    stats->MulticastPacketsReceived    += drvStats->traffic_stats.MulticastPacketsReceived;
    stats->BroadcastPacketsSent        += drvStats->traffic_stats.BroadcastPacketsSent;
    stats->BroadcastPacketsReceived    += drvStats->traffic_stats.BroadcastPacketsReceived;
    stats->UnknownProtoPacketsReceived += drvStats->UnknownProtoPacketsReceived;
    stats->FailedRetransCount          += drvStats->retrans_stats.FailedRetransCount;
    stats->RetryCount                  += drvStats->retrans_stats.RetryCount;
    stats->MultipleRetryCount          += drvStats->retrans_stats.MultipleRetryCount;

    return rc;
}

struct cbData_t {
    size_t size;    // Length of the expected data in bytes
    void *data;     // Pointer to memory where received data will be written
};

/**
 * @brief Callback function for receiving data from wld_rad_nl80211_sendVendorSubCmd getters
 * 
 * @param priv Pointer to preallocated cbData_t structure
 * @return SWL_RC_OK in case of success
 *         <= SWL_RC_ERROR otherwise
 */
static swl_rc_ne s_getDataCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    struct cbData_t *getData = priv;
    ASSERT_NOT_NULL(getData, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    void* data = NULL;
    size_t len = 0;

    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    len = nla_len(tb[NL80211_ATTR_VENDOR_DATA]);
    ASSERT_EQUALS(getData->size, len, SWL_RC_ERROR, ME, "Wrong data size %zu received, expected %zu", len, getData->size);
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    ASSERT_NOT_NULL(data, SWL_RC_ERROR, ME, "NULL");

    memcpy(getData->data, data, len);
    return rc;
}

/**
 * @brief Callback function for receiving and copying bg scan params from GET_SCAN_PARAMS_BG
 *
 * @param priv Pointer to preallocated mxl_bgScanParams_t structure
 * @return SWL_RC_OK in case of success
 *         <= SWL_RC_ERROR otherwise
 */
static swl_rc_ne s_getBgScanParams(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    mxl_bgScanParams_t* pBgScanParams = (mxl_bgScanParams_t*) priv;
    ASSERT_NOT_NULL(pBgScanParams, SWL_RC_ERROR, ME, "pBgScanParams is NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data
    void* data = NULL;
    size_t len;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    ASSERT_NOT_NULL(data, SWL_RC_ERROR, ME, "data is NULL");
    len = nla_len(tb[NL80211_ATTR_VENDOR_DATA]);
    ASSERT_EQUALS(sizeof(*pBgScanParams), len, SWL_RC_ERROR, ME, "Wrong data size %zu received, expected %zu", len, sizeof(*pBgScanParams));

    memcpy(pBgScanParams, data, len);

    return rc;
}

static swl_rc_ne s_getRadCurrentTemp(T_Radio* pRad, int32_t* pTemp)
{
    swl_rc_ne rc;
    struct cbData_t getData;
    getData.size = sizeof(*pTemp);
    getData.data = pTemp;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, LTQ_NL80211_VENDOR_SUBCMD_GET_TEMPERATURE_SENSOR, NULL, 0,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getDataCb, &getData);

    return rc;
}

int whm_mxl_rad_stats(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, WLD_ERROR_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    T_Stats stats = {0};

    amxc_llist_for_each(it, &pRad->llAP) {
        T_AccessPoint* pAP = (T_AccessPoint*) amxc_llist_it_get_data(it, T_AccessPoint, it);

        SAH_TRACEZ_INFO(ME, "Radio AP interface = %s", pAP->alias);
        if ((pAP->index <= 0) || !mxl_isApReadyToProcessVendorCmd(pAP))
            continue;

        rc = wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, LTQ_NL80211_VENDOR_SUBCMD_GET_TR181_WLAN_STATS, NULL, 0,
                                             VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getVAPStatsCb, &stats);
        ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "%s: GET_TR181_WLAN_STATS failed", pAP->alias);
        rc = mxl_getWmmStats(pAP, &stats, true);
        ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "%s: Get WMM stats failed", pAP->alias);
    }

    pRad->stats = stats; /* struct copy */
    rc = wld_rad_getCurrentNoise(pRad, &pRad->stats.noise);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "%s: wld_rad_getCurrentNoise failed", pRad->Name);
    rc = s_getRadCurrentTemp(pRad, &pRad->stats.TemperatureDegreesCelsius);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "%s: s_mxl_rad_getCurrentTemp failed", pRad->Name);

    /* TODO: fill WMM counters */

    return rc;
}

/**
 * @brief Get BG scan params from driver for specific radio
 *
 * @param pRad Pointer radio context
 * @param pBgScanParams Pointer to preallocated mxl_bgScanParams_t structure
 * @return SWL_RC_OK in case of success
 *         <= SWL_RC_ERROR otherwise
 */
swl_rc_ne whm_mxl_getBgScanParams(T_Radio* pRad, mxl_bgScanParams_t *pBgScanParams) {
    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "pRad is NULL");
    ASSERT_NOT_NULL(pBgScanParams, SWL_RC_ERROR, ME, "pBgScanParams is NULL");
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_SCAN_PARAMS_BG;
    swl_rc_ne rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, NULL, 0,
                                                    VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getBgScanParams, pBgScanParams);
    return rc;
}

/**
 * @brief Set BG scan params from driver for specific radio
 *
 * @param pRad Pointer radio context
 * @param pBgScanParams Pointer to preallocated mxl_bgScanParams_t structure
 * @return SWL_RC_OK in case of success
 *         <= SWL_RC_ERROR otherwise
 */
swl_rc_ne whm_mxl_setBgScanParams(T_Radio* pRad, mxl_bgScanParams_t *pBgScanParams) {
    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "pRad is NULL");
    ASSERT_NOT_NULL(pBgScanParams, SWL_RC_ERROR, ME, "pBgScanParams is NULL");
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_SCAN_PARAMS_BG;
    swl_rc_ne rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, pBgScanParams, sizeof(mxl_bgScanParams_t),
                                                    VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
    return rc;
}

static uint32_t s_writeAntennaCtrl(T_Radio* pRad, com_dir_e comdir) {
    uint32_t maxMask = (1 << pRad->nrAntenna[comdir]) - 1;
    uint32_t setVal = (comdir == COM_DIR_TRANSMIT) ? pRad->txChainCtrl : pRad->rxChainCtrl;
    uint32_t val = pRad->actAntennaCtrl;
    if(setVal <= 0) {
        setVal = ((val <= 0) || (val > maxMask)) ? maxMask : val;
    }
    return setVal;
}

int whm_mxl_rad_antennaCtrl(T_Radio* pRad, int val, int set) {
    swl_rc_ne rc;
    ASSERT_TRUE(set & SET, WLD_ERROR_INVALID_PARAM, ME, "Get Only");
    uint32_t txMapAnt = s_writeAntennaCtrl(pRad, COM_DIR_TRANSMIT);
    uint32_t rxMapAnt = s_writeAntennaCtrl(pRad, COM_DIR_RECEIVE);

    int data[3] = {0};
    data[0] = (val == -1) ? MXL_COC_MODE_AUTO : MXL_COC_MODE_MANUAL;    // set to manual PCOC mode = 0 or auto 1 (when AntennaCtrl is -1 default value)
    data[1] = (val == -1) ? 0 : swl_bit32_getNrSet(txMapAnt);           // To get Y from map
    data[2] = (val == -1) ? 0 : swl_bit32_getNrSet(rxMapAnt);           // To get Z from map

    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_COC_POWER_MODE;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, (char*) data, sizeof(data),
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);

    pRad->nrActiveAntenna[COM_DIR_TRANSMIT] = swl_bit32_getNrSet(txMapAnt);
    pRad->nrActiveAntenna[COM_DIR_RECEIVE] = swl_bit32_getNrSet(rxMapAnt);
    wld_radio_updateAntenna(pRad);
    SAH_TRACEZ_INFO(ME, "%s: enable %u, ctrl %i %i-%i ; ant %i/%i - %i/%i",
                    pRad->Name, pRad->enable, pRad->actAntennaCtrl, pRad->txChainCtrl, pRad->rxChainCtrl,
                    pRad->nrActiveAntenna[COM_DIR_TRANSMIT], pRad->nrAntenna[COM_DIR_TRANSMIT],
                    pRad->nrActiveAntenna[COM_DIR_RECEIVE], pRad->nrAntenna[COM_DIR_RECEIVE]);
    return rc;
}

int whm_mxl_rad_beamforming(T_Radio* pRad, beamforming_type_t type, int val, int set) {
    swl_rc_ne rc;
    ASSERT_NOT_NULL(pRad, WLD_ERROR_INVALID_PARAM, ME, "NULL");
    ASSERT_TRUE(set & SET, WLD_ERROR_INVALID_PARAM, ME, "Get Only");

    uint32_t typeToSet = BF_MODE_DISABLE_ALL;

    if(val) {
        if(pRad->implicitBeamFormingEnabled && pRad->explicitBeamFormingEnabled) {
            typeToSet = BF_MODE_ENABLE_ALL;
        } else {
            if(type == beamforming_implicit) {
                typeToSet = BF_MODE_ENABLE_IMPLICIT;
            } else {
                typeToSet = BF_MODE_ENABLE_EXPLICIT;
            }
        }
    }

    SAH_TRACEZ_INFO(ME, "%s %d %d %d", pRad->Name, type, val, set);

    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_BF_MODE;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, &typeToSet, sizeof(uint32_t),
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);

    return rc;
}

static swl_rc_ne s_checkAndStartZwDfs(T_Radio* pRad, bool direct) {
    // check if 5GHz radio and is DFS channel and background DFS enabled
    if(!wld_rad_is_5ghz(pRad) ||
       !swl_channel_isDfs(pRad->targetChanspec.chanspec.channel) ||
       !wld_rad_isUpExt(pRad) ||
       (wld_chanmgt_getCurBw(pRad) > pRad->maxChannelBandwidth) ||
       (pRad->bgdfs_config.status == BGDFS_STATUS_OFF)) {
        return SWL_RC_DONE;
    }

    // check if ZW DFS is needed
    if(wld_channel_is_band_passive(pRad->targetChanspec.chanspec)) {
        swl_rc_ne rc = pRad->pFA->mfn_wrad_zwdfs_start(pRad, direct);
        if(!(rc < SWL_RC_OK)) {
            return SWL_RC_CONTINUE;
        }
    }

    return SWL_RC_DONE;
}

amxd_status_t _whm_mxl_rad_validateObssInterval_pvf(amxd_object_t* _UNUSED,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    int newVal = amxc_var_dyncast(int32_t, args);
    if ((newVal >= OBSS_INTERVAL_MIN && newVal <= OBSS_INTERVAL_MAX) || newVal == 0) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}

amxd_status_t _whm_mxl_rad_validateCcaTh_pvf(amxd_object_t* object,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, amxd_status_ok, ME, "No Radio Mapped");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, amxd_status_ok, ME, "pRadVendor NULL");
    char* ccaThStr = amxc_var_dyncast(cstring_t, args);
    ASSERT_NOT_NULL(ccaThStr, amxd_status_invalid_value, ME, "CCA Threshold string is NULL");
    if (mxl_convertStringToIntList(pRadVendor->ccaTh , ccaThStr, CCA_TH_SIZE) == amxd_status_invalid_value) {
        free(ccaThStr);
        return amxd_status_invalid_value;
    }
    free(ccaThStr);
    for(int i = 0; i < CCA_TH_SIZE; i++) {
        if (pRadVendor->ccaTh[i] > CCA_THRESHOLD_MAX || pRadVendor->ccaTh[i] < CCA_THRESHOLD_MIN) {
            return amxd_status_invalid_value;
        }
    }

    return amxd_status_ok;
}

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
amxd_status_t _whm_mxl_rad_validateDfsDebugChan_pvf(amxd_object_t* _UNUSED,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    int newVal = amxc_var_dyncast(int32_t, args);
    if ((newVal >= DFS_DEBUG_CHAN_MIN && newVal <= DFS_DEBUG_CHAN_MAX) || newVal == -1) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}

amxd_status_t _whm_mxl_rad_validateZwdfsDebugChan_pvf(amxd_object_t* _UNUSED,
                                                         amxd_param_t* param _UNUSED,
                                                         amxd_action_t reason _UNUSED,
                                                         const amxc_var_t* const args,
                                                         amxc_var_t* const retval _UNUSED,
                                                         void* priv _UNUSED) {
    int newVal = amxc_var_dyncast(int32_t, args);
    if ((newVal >= ZWDFS_DEBUG_CHAN_MIN && newVal <= ZWDFS_DEBUG_CHAN_MAX) || newVal == -1) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

amxd_status_t _whm_mxl_rad_validateFirstNonDfs_pvf(amxd_object_t* object,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, amxd_status_ok, ME, "No Radio Mapped");
    bool newVal = amxc_var_dyncast(bool, args);
    if (pRad->autoChannelEnable && newVal ) {
        SAH_TRACEZ_NOTICE(ME, "%s: ACS is enabled, First Non DFS won't be enabled", pRad->Name);
        return amxd_status_invalid_value;
    }
    return amxd_status_ok;
}

amxd_status_t _whm_mxl_rad_validateBssColorChangeTimeout_pvf(amxd_object_t* _UNUSED,
                                                       amxd_param_t* param _UNUSED,
                                                       amxd_action_t reason _UNUSED,
                                                       const amxc_var_t* const args,
                                                       amxc_var_t* const retval _UNUSED,
                                                       void* priv _UNUSED) {
    int newVal = amxc_var_dyncast(int32_t, args);
    if ((newVal >= BSS_COLOR_CHANGE_TIMEOUT_MIN && newVal <= BSS_COLOR_CHANGE_TIMEOUT_MAX) || newVal == BSS_COLOR_CHANGE_TIMEOUT_RANDOM) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}

amxd_status_t _whm_mxl_rad_validateDuplicateHeBeacon_pvf(amxd_object_t* object,
                                           amxd_param_t* param,
                                           amxd_action_t reason _UNUSED,
                                           const amxc_var_t* const args,
                                           amxc_var_t* const retval _UNUSED,
                                           void* priv _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_status_t status = amxd_status_ok;
    bool newVal = amxc_var_dyncast(bool, args);
    ASSERTS_TRUE(newVal, status, ME, "Disabling DuplicateBeacon/HeBeacon always allowed");
    const char* paramName = amxd_param_get_name(param);
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, amxd_status_ok, ME, "No Radio Mapped");
    amxd_object_t* pVendorObj = amxd_object_get(pRad->pBus, "Vendor");
    ASSERT_NOT_NULL(pVendorObj, amxd_status_ok, ME, "pVendorObj NULL");

    if(swl_str_matches(paramName, "HeBeacon")) {
        if(amxd_object_get_value(bool, pVendorObj, "DuplicateBeaconEnabled", NULL) > 0) {
         status = amxd_status_invalid_value;
         SAH_TRACEZ_INFO(ME, "HeBeacon and DuplicateBeaconEnabled cannot be both above 0");
        }
    }
    else if(swl_str_matches(paramName, "DuplicateBeaconEnabled")) {
        if(amxd_object_get_value(bool, pVendorObj, "HeBeacon", NULL) > 0) {
         status = amxd_status_invalid_value;
         SAH_TRACEZ_INFO(ME, "HeBeacon and DuplicateBeaconEnabled cannot be both above 0");
        }
    }
    SAH_TRACEZ_OUT(ME);

    return status;
}

amxd_status_t _whm_mxl_rad_validateBackgroundCac_pvf(amxd_object_t* object,
                                               amxd_param_t* param _UNUSED,
                                               amxd_action_t reason _UNUSED,
                                               const amxc_var_t* const args,
                                               amxc_var_t* const retval _UNUSED,
                                               void* priv _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_status_t status = amxd_status_ok;
    uint32_t newVal = amxc_var_dyncast(bool, args);
    ASSERTS_TRUE(newVal, status, ME, "Disabling Background CAC always allowed");
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, amxd_status_ok, ME, "No Radio Mapped");
    if (pRad->bgdfs_config.enable) {
        status = amxd_status_invalid_value;
        SAH_TRACEZ_ERROR(ME, "PreclearEnable and BackgroundCac cannot be both true");
    }
    SAH_TRACEZ_OUT(ME);
    return status;
}

static void s_setQAMplus_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    bool setQAMplusNewVal = amxc_var_dyncast(bool, newValue);

    /* Sync only for the 2G band & 11n */
    if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_N) && wld_rad_is_24ghz(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s: 2.4GHz sQAMplus Override is set to %d", pRad->Name, setQAMplusNewVal);
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), (setQAMplusNewVal ? "1" : "0"));
    } else {
        SAH_TRACEZ_INFO(ME, "%s: Parameter is not relevant for this radio, configuration will not be applied", pRad->Name);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setRadarRssiTh_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int8_t SetRadarRssiThNewVal = amxc_var_dyncast(int8_t, newValue);
    char newValStr[32] = {0};

    /* Sync only for the 5G */
    if (wld_rad_is_5ghz(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s: 5GHz sRadarRssiTh Override is set to %d", pRad->Name, SetRadarRssiThNewVal);
        swl_str_catFormat(newValStr, sizeof(newValStr), "%d", SetRadarRssiThNewVal);
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    } else {
        SAH_TRACEZ_INFO(ME, "%s: Parameter is not relevant for this radio, configuration will not be applied", pRad->Name);
    }

    SAH_TRACEZ_OUT(ME);
}

static void s_setCcaTh_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor NULL");

    /* Remove extra spaces from the ccaTh string */
    char* ccaThStr = amxc_var_dyncast(cstring_t, newValue);
    ASSERT_NOT_NULL(ccaThStr, , ME, "CCA Threshold string is NULL");
    mxl_removeExtraSpacesfromString(&ccaThStr);
    amxd_object_set_value(cstring_t, object, "SetCcaTh", ccaThStr);
    free(ccaThStr);

    /* send Nl command to the driver to update ccaTh parameter*/
    uint32_t subcmd= LTQ_NL80211_VENDOR_SUBCMD_SET_CCA_THRESHOLD;
    wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, (char*) pRadVendor->ccaTh, sizeof(pRadVendor->ccaTh),
                                     VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);

    SAH_TRACEZ_OUT(ME);
}

static void s_setOverrideMBSSID_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */ 
    amxd_object_t* radObj = amxd_object_get_parent(object); 
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    bool overrideMbssidNewVal = amxc_var_dyncast(bool, newValue);

    /* Sync only the 6G band */
    if(wld_rad_is_6ghz(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s: 6GHz MBSSID Override is set to %d", pRad->Name, overrideMbssidNewVal);
        whm_mxl_handleMbssidOverride(pRad, overrideMbssidNewVal);
    } else {
        SAH_TRACEZ_INFO(ME, "%s: Parameter is not relevant for this radio, configuration will not be applied", pRad->Name);
    } 
    
    SAH_TRACEZ_OUT(ME);
}

static void s_setBfMode_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int bfModeNewVal = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[64] = {0};
    if(bfModeNewVal != -1) {
        swl_str_catFormat(newValStr, sizeof(newValStr), "%d", bfModeNewVal);
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setPowerSelection_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int8_t *txPowVal = whm_mxl_rad_txPercentToPower(amxc_var_dyncast(int32_t, newValue));
    ASSERT_NOT_NULL(txPowVal, , ME, "%s: unable to find matching percent to power",pRad->Name);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", *txPowVal);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);

    SAH_TRACEZ_OUT(ME);
}

static void s_setApMaxNumSta_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint16_t apMaxNumSta = amxc_var_dyncast(uint16_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", apMaxNumSta);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setObssBeaconRssiThreshold_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int obssBeaconRssiTh = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[32] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%d", obssBeaconRssiTh);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setProbeReqListTimer_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int obssBeaconRssiTh = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[32] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%d", obssBeaconRssiTh);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setDfsChStateFile_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    char* dfsChStaFile = amxc_var_dyncast(cstring_t, newParamValues);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), dfsChStaFile);
    free(dfsChStaFile);
    SAH_TRACEZ_OUT(ME);
}

static void s_setCountryThird_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    char* country = amxc_var_dyncast(cstring_t, newParamValues);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), country);
    }
    free(country);
    SAH_TRACEZ_OUT(ME);
}

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
static void s_setDfsDebugChan_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int dfsDebugChan = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%d", dfsDebugChan);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setZwdfsDebugChan_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int zwdfsDebugChan = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%d", zwdfsDebugChan);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

static void s_setSubBandDFS_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint32_t subBandDfs = amxc_var_dyncast(uint32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", subBandDfs);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setEnable80211BeOverride_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param _UNUSED, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor NULL");

    pRadVendor->enable80211BeOverride = amxc_var_dyncast(bool, newParamValues);
    SAH_TRACEZ_INFO(ME, "%s: enable80211BeOverride set as %d", pRad->Name, pRadVendor->enable80211BeOverride);
    wld_rad_doSync(pRad);

    SAH_TRACEZ_OUT(ME);
}

static void s_setTwtResponderSupport_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint32_t twtResponderSupport = amxc_var_dyncast(uint32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", twtResponderSupport);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setHeMacTwtResponderSupport_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint32_t heMactwtResponderSupport = amxc_var_dyncast(uint32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", heMactwtResponderSupport);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setRadioBoolVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    bool newVal = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), (newVal ? "1" : "0"));
    SAH_TRACEZ_OUT(ME);
}

static void s_setRadioBoolCertVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    bool newVal = amxc_var_dyncast(bool, newParamValues);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), (newVal ? "1" : "0"));
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setRadioUint32CertVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint32_t newVal = amxc_var_dyncast(uint32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", newVal);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setRadioInt32CertVendorParam_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    int32_t newVal = amxc_var_dyncast(int32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%d", newVal);
    if(whm_mxl_isCertModeEnabled()) {
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setBackgroundCacTh_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    bool backgroundCac = amxc_var_dyncast(bool, newParamValues);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), (backgroundCac ? "1" : "0"));

    /* set ZWDFS antenna */
    pRad->pFA->mfn_wrad_bgdfs_enable(pRad, backgroundCac);

    SAH_TRACEZ_OUT(ME);
}

static void s_setFirstNonDfs_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor NULL");
    pRadVendor->firstNonDfs = amxc_var_dyncast(bool, newParamValues);
    if ((!pRad->autoChannelEnable) || (!pRadVendor->firstNonDfs)) {
        if (wld_secDmn_isAlive(pRad->hostapd)) {
            wld_rad_hostapd_setChannel(pRad);
        }
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), (pRadVendor->firstNonDfs ? "1" : "0"));
    } else {
        SAH_TRACEZ_NOTICE(ME, "%s: ACS is enabled, First Non DFS won't be enabled", pRad->Name);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setDuplicateBeaconBw_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    uint32_t duplicateBeaconBw = amxc_var_dyncast(uint32_t, newParamValues);
    char newValStr[64] = {0};
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", duplicateBeaconBw);
    whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    SAH_TRACEZ_OUT(ME);
}

static void s_setPunctureBitMap_pwf(void* priv _UNUSED, amxd_object_t* object, amxd_param_t* param, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor */
    amxd_object_t* radObj = amxd_object_get_parent(object);
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "No Radio Mapped");
    char newValStr[32] = {0};
    uint16_t punctureBitMap = amxc_var_dyncast(uint16_t, newParamValues);
    swl_str_catFormat(newValStr, sizeof(newValStr), "%u", punctureBitMap);
    if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_BE)) {
        whm_mxl_determineRadParamAction(pRad, amxd_param_get_name(param), newValStr);
    }

    SAH_TRACEZ_OUT(ME);
}

swl_rc_ne whm_mxl_rad_supvendModesChanged(T_Radio* pRad, T_AccessPoint* pAP _UNUSED, amxd_object_t* object, amxc_var_t* params _UNUSED) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(object, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_VendorData_t* pVendorRad = mxl_rad_getVendorData(pRad);
    if(pVendorRad != NULL) {
        /*save pointer to vendor object*/
        pVendorRad->pBus = amxd_object_get(pRad->pBus, "Vendor");
    }
    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

amxd_status_t _whm_mxl_rad_debug(amxd_object_t* object,
                                 amxd_function_t* func _UNUSED,
                                 amxc_var_t* args _UNUSED,
                                 amxc_var_t* retval) {

    amxd_object_t* wifiRadObj = amxd_object_get_parent(object);

    amxc_var_init(retval);
    amxc_var_set_type(retval, AMXC_VAR_ID_HTABLE);
    if ((amxd_object_get_type(wifiRadObj) != amxd_object_instance) ||
       (!debugIsRadPointer((T_Radio*) wifiRadObj->priv))) {
        amxc_var_add_key(cstring_t, retval, "Error", "Invalid rad object");
        return amxd_status_ok;
    }
    T_Radio* pRad = (T_Radio*) wifiRadObj->priv;
    if (!pRad) {
        amxc_var_add_key(cstring_t, retval, "Error", "Radio is NULL");
        return amxd_status_ok;
    }
    const char* feature = GET_CHAR(args, "op");

    if (swl_str_matches(feature, "StaScanTime")) {
        whm_mxl_monitor_getStaScanTimeOut(pRad);
        mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
        ASSERT_NOT_NULL(vendorData, amxd_status_invalid_value, ME, "vendorData NULL");
        amxc_var_add_key(int32_t, retval, "StaScanTime", vendorData->naSta.scanTimeout);
        amxc_var_add_key(cstring_t, retval, "Status", "executed command");
    } else if (swl_str_matches(feature, "NaStaMon")) {
        whm_mxl_monitor_updateMonStats(pRad);
        amxc_var_add_key(cstring_t, retval, "Status", "executed command");
    } else if (swl_str_matches(feature, "CommitReconfFsm")) {
        whm_mxl_reconfMngr_doCommit(pRad);
        amxc_var_add_key(cstring_t, retval, "Status", "executed command");
    } else {
        //help display
        amxc_var_add_key(cstring_t, retval, "help", "Please add argument 'op', with one of following debug operations:");
        amxc_var_t* opMap = amxc_var_add_key(amxc_htable_t, retval, "op", NULL);
        amxc_var_add_key(cstring_t, opMap, "StaScanTime", "To trigger a LTQ_NL80211_VENDOR_SUBCMD_GET_UNCONNECTED_STA_SCAN_TIME subcmd");
        amxc_var_add_key(cstring_t, opMap, "NaStaMon", "To trigger LTQ_NL80211_VENDOR_SUBCMD_GET_UNCONNECTED_STA subcmd");
        amxc_var_add_key(cstring_t, opMap, "CommitReconfFsm", "To trigger a dummy commit to the Reconf FSM");
    }

    return amxd_status_ok;
}

static swl_rc_ne s_prepareCertBgScan(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRadio is NULL");
    mxl_bgScanParams_t bgScanParams;
    if (wld_rad_is_6ghz(pRad)) {
        bgScanParams.passive = 22;
        bgScanParams.active = 22;
        bgScanParams.num_probe_reqs = 2;
        bgScanParams.probe_reqs_interval = 1;
        bgScanParams.num_chans_in_chunk = 1;
        bgScanParams.break_time = 100;
        bgScanParams.break_time_busy = 100;
        bgScanParams.window_slice = 22;
        bgScanParams.window_slice_overlap = 5;
        bgScanParams.cts_to_self_duration = 26;
    } else {
        bgScanParams.passive = 103;
        bgScanParams.active = 103;
        bgScanParams.num_probe_reqs = 2;
        bgScanParams.probe_reqs_interval = 1;
        bgScanParams.num_chans_in_chunk = 1;
        bgScanParams.break_time = 100;
        bgScanParams.break_time_busy = 100;
        bgScanParams.window_slice = 103;
        bgScanParams.window_slice_overlap = 5;
        bgScanParams.cts_to_self_duration = 32;
    }

    swl_rc_ne rc = whm_mxl_setBgScanParams(pRad, &bgScanParams);
    SAH_TRACEZ_NOTICE(ME, "%s: Set BG scan params in certification mode rc(%d)", pRad->Name, rc);
    return (rc < SWL_RC_OK) ? SWL_RC_ERROR : rc;
}

swl_rc_ne whm_mxl_rad_startScan(T_Radio* pRadio) {
    wld_nl80211_scanFlags_t flags = {.flush = true, .force = true};
    if (whm_mxl_isCertModeEnabled()) {
        s_prepareCertBgScan(pRadio);
    }
    return wld_rad_nl80211_startScanExt(pRadio, &flags);
}

SWLA_DM_HDLRS(sRadVendorDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("OverrideMBSSID", s_setOverrideMBSSID_pwf),
                  SWLA_DM_PARAM_HDLR("ApMaxNumSta", s_setApMaxNumSta_pwf),
                  SWLA_DM_PARAM_HDLR("SetProbeReqCltMode", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetBfMode", s_setBfMode_pwf),
                  SWLA_DM_PARAM_HDLR("SetPowerSelection", s_setPowerSelection_pwf),
                  SWLA_DM_PARAM_HDLR("Ignore40MhzIntolerant", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("ObssBeaconRssiThreshold", s_setObssBeaconRssiThreshold_pwf),
                  SWLA_DM_PARAM_HDLR("ProbeReqListTimer", s_setProbeReqListTimer_pwf),
                  SWLA_DM_PARAM_HDLR("DfsChStateFile", s_setDfsChStateFile_pwf),
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
                  SWLA_DM_PARAM_HDLR("DfsDebugChan", s_setDfsDebugChan_pwf),
                  SWLA_DM_PARAM_HDLR("ZwdfsDebugChan", s_setZwdfsDebugChan_pwf),
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
                  SWLA_DM_PARAM_HDLR("SubBandDFS", s_setSubBandDFS_pwf),
                  SWLA_DM_PARAM_HDLR("Enable80211BeOverride", s_setEnable80211BeOverride_pwf),
                  SWLA_DM_PARAM_HDLR("TwtResponderSupport", s_setTwtResponderSupport_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacTwtResponderSupport", s_setHeMacTwtResponderSupport_pwf),
                  SWLA_DM_PARAM_HDLR("DynamicEdca", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeDebugMode", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeBeacon", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("DuplicateBeaconEnabled", s_setRadioBoolVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("DuplicateBeaconBw", s_setDuplicateBeaconBw_pwf), 
                  SWLA_DM_PARAM_HDLR("SetQAMplus", s_setQAMplus_pwf),
                  SWLA_DM_PARAM_HDLR("SetRadarRssiTh", s_setRadarRssiTh_pwf),
                  SWLA_DM_PARAM_HDLR("SetCcaTh", s_setCcaTh_pwf),
                  SWLA_DM_PARAM_HDLR("BackgroundCac", s_setBackgroundCacTh_pwf),
                  SWLA_DM_PARAM_HDLR("FirstNonDfs", s_setFirstNonDfs_pwf),
                  SWLA_DM_PARAM_HDLR("PunctureBitMap", s_setPunctureBitMap_pwf),
		          SWLA_DM_PARAM_HDLR("TestBedMode", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyLdpcCodingInPayload", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacMsduAckEnabledMpduSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacOmControlSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HtMinMpduStartSpacing", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("MultibssEnable", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyMaxNc", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SrCtrlHesigaSpatialReuseVal15", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeOperationCohostedBss", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaIePresent", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDcmMaxConstellationTx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDcmMaxConstellationRx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDcmMaxNssTx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDcmMaxNssRx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("Ieee80211nAcAxCompat", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EnableHeDebugMode", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBeAifsn", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBkAifsn", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcViAifsn", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcVoAifsn", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EnableEhtDebugMode", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacEhtOmControl", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacRestrictedTwt", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacTrigTxopSharingMode1", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacTrigTxopSharingMode2", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyTrigMuBfPartialBwFb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMaxNc", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhySuBeamformer", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhySuBeamformee", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyPpeThresholdsPresent", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetDynamicMuTypeDownLink", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetDynamicMuTypeUpLink", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacScsTrafficDesc", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMldTsfDiff", s_setRadioInt32CertVendorParam_pwf),

                  SWLA_DM_PARAM_HDLR("HeMacMaxAMpduLengthExponent", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBeEcwmin", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBeEcwmax", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBeTimer", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBkAci", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBkEcwmin", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBkEcwmax", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcBkTimer", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcViAci", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcViEcwmin", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcViEcwmax", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcViTimer", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcVoAci", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcVoEcwmin", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcVoEcwmax", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMuEdcaAcVoTimer", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzRx09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzTx09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzRx1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzTx1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzRx1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMapLessOrEq80MHzTx1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMaxAmpduLenExpExt", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMacMaxMpduLen", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyCommonNominalPktPad", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzRxMcs09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzTxMcs09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzTxMcs1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzRxMcs1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzTxMcs1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap160MHzRxMcs1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzRxMcs09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzTxMcs09", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzRxMcs1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzTxMcs1011", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzRxMcs1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtMcsMap320MHzTxMcs1213", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhy320MHzIn6GHz", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("AdvertiseEcsaIe", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetMaxMpduLen", s_setRadioInt32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeOpTxopDurationRtsThreshold", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhySuBeamformeeCapable", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhySuBeamformerCapable", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyBeamformeeStsLesOrEq80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyBeamformeeStsGreater80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDeviceClass", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhySuPpdu1xHeLtfAnd08UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhySuPpduHeMu4xHeLtf08UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyMuBeamformerCapable", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyNdpWith4xHeLtfAnd32UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyNg16SuFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyNg16MuFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyNumSoundDimenLeOrEq80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyNumSoundDimenGreater80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyTriggerSuBeamformFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDopplerRx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDopplerTx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyFullBandwidthUlMuMimo", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyPartialBandwidthUlMuMimo", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyPartialBWExtendedRange", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyTriggeredCqiFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyPpeThresholdsPresent", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyCodebookSize42SuSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyCodebookSize75MuSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyPowBoostFactAlphaSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacOmCtrlMuDisableRxSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeOpTxopDurationRtsThreshold", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacUl2x996ToneRuSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacAckEnabledAggrSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacBroadcastTwtSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyDcmMaxBw", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyLong16HeSigOfdmSymSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacNdpFeedbackReportSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyRx1024QLt242ToneRuSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyRxFullBwSuUsingMuCompSigb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyRxFulBwUsingMuNonComSigb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyStbcTxLessThanOrEq80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyStbcTxGreaterThan80Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeOperationErSuDisable", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyErSuPpdu4xLtf8UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HePhyPreamblePuncturingRx", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacMultiTidAggrTxSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("HeMacMultiTidAggrRxSupport", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNumSoundDim80MhzOrBelow", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNumSoundingDim160Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNumSoundingDim320Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMuBeamformerBw80MhzBelow", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMuBeamformerBw160Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMuBeamformerBw320Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNdp4xEhtLtfAnd32UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyPartialBwUlMuMimo", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyBeamformeeSs80MhzOrBelow", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyBeamformeeSs160Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyBeamformeeSs320Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyEhtDupIn6Ghz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhy20MhzOpStaRxNdpWiderBw", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNg16SuFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNg16MuFeedback", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyCodebookSize42SuFb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyCodebookSize755MuFb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyTrigSuBfFb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyTrigCqiFb", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyPartialBwDlMuMimo", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyPsrBasedSr", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyEhtMuPpdu4xEhtLtf08UsGi", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyRx1024Qam4096QamBel242Ru", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMaxNumOfSupportedEhtLtfs", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyMcs15", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNonOfdmaMuMimo80MhzBelow", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNonOfdmaUlMuMimoBw160Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("EhtPhyNonOfdmaUlMuMimoBw320Mhz", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("Rnr6gOpClass137Allowed", s_setRadioBoolCertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetDynamicMuMinStationsInGroup", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetDynamicMuMaxStationsInGroup", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("SetDynamicMuCdbConfig", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("RnrTbttMldNonZeroPad", s_setRadioUint32CertVendorParam_pwf),
                  SWLA_DM_PARAM_HDLR("Country3", s_setCountryThird_pwf))
              );
void _whm_mxl_rad_setVendorObj_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {                          
    swla_dm_procObjEvtOfLocalDm(&sRadVendorDmHdlrs, sig_name, data, priv);
}

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
amxd_status_t _whm_mxl_rad_validateBgAcsInterval_pvf(amxd_object_t* _UNUSED,
                                                     amxd_param_t* param _UNUSED,
                                                     amxd_action_t reason _UNUSED,
                                                     const amxc_var_t* const args,
                                                     amxc_var_t* const retval _UNUSED,
                                                     void* priv _UNUSED) {
    uint16_t newVal = amxc_var_dyncast(uint16_t, args);
    if ((newVal >= MXL_BG_ACS_INTERVAL_MIN && newVal <= MXL_BG_ACS_INTERVAL_MAX) || (newVal == 0)) {
        return amxd_status_ok;
    }
    return amxd_status_invalid_value;
}

static void s_setAcsConfig_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.Acs. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    SAH_TRACEZ_INFO(ME, "%s: update vendor ACS config", pRad->Name);
    bool newVal;
    amxc_var_for_each(newValue, newParamValues) {
        char* newValStr = NULL; 
        const char* pname = amxc_var_key(newValue);
        if(swl_str_matches(pname, "AcsFallbackChan")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AcsScanMode")) {
            newVal = amxc_var_dyncast(bool, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, (newVal ? "1" : "0"));
        } else if(swl_str_matches(pname, "AcsUpdateDoSwitch")) {
            newVal = amxc_var_dyncast(bool, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, (newVal ? "1" : "0"));
        } else if(swl_str_matches(pname, "AcsFils")) {
            newVal = amxc_var_dyncast(bool, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, (newVal ? "1" : "0"));
        } else if(swl_str_matches(pname, "Acs6gPunctMode") && wld_rad_is_6ghz(pRad)) {
            newVal = amxc_var_dyncast(bool, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, (newVal ? "1" : "0"));
        } else if(swl_str_matches(pname, "Acs6gOptChList")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AcsStrictChList")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AcsBgScanInterval")) {
            if (pRadVendor) {
                pRadVendor->bgAcsInterval = amxc_var_dyncast(uint16_t, newValue);
                whm_mxl_configureBgAcs(pRad, pRadVendor->bgAcsInterval);
            }
        } else if(swl_str_matches(pname, "AcsFallbackPrimaryChan")) {
            ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
            pRadVendor->AcsFbPrimChan = amxc_var_dyncast(int32_t, newValue);
        } else if(swl_str_matches(pname, "AcsFallbackSecChan")) {
            ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
            pRadVendor->AcsFbSecChan = amxc_var_dyncast(int32_t, newValue);
        } else if(swl_str_matches(pname, "AcsFallbackBW")) {
            ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
            pRadVendor->AcsFbBw = amxc_var_dyncast(int32_t, newValue);
            if(pRadVendor->AcsFbBw != 0) {
                char AcsFallbackChannel[128] = {0};
                swl_str_catFormat(AcsFallbackChannel, sizeof(AcsFallbackChannel), "%d %d %d", pRadVendor->AcsFbPrimChan, pRadVendor->AcsFbSecChan, pRadVendor->AcsFbBw);
                amxd_object_set_value(cstring_t, object, "AcsFallbackChan", AcsFallbackChannel);
                whm_mxl_determineRadParamAction(pRad, "AcsFallbackChan", AcsFallbackChannel);
            }
        /**
         * AcsUse24Overlapped is missing need to implement
         */
        } else {
            continue;
        }
        free(newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sAcsConfigDmHdlrs, ARR(), .objChangedCb = s_setAcsConfig_ocf);

void _whm_mxl_rad_setAcsConf_ocf(const char* const sig_name,
                                  const amxc_var_t* const data,
                                  void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sAcsConfigDmHdlrs, sig_name, data, priv);
}
#else
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

static void s_setAfcConfig_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.Afc. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: update vendor AFC config", pRad->Name);
    amxc_var_for_each(newValue, newParamValues) {
        char* newValStr = NULL;
        const char* pname = amxc_var_key(newValue);
        if(swl_str_matches(pname, "AfcOpClass")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcdSock")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcFrequencyRange")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcCertIds")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcSerialNumber")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcLinearPolygon")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcLocationType")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcRequestId")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "AfcRequestVersion")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        }else {
            continue;
        }
        free(newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sAfcConfigDmHdlrs, ARR(), .objChangedCb = s_setAfcConfig_ocf);

void _whm_mxl_rad_setAfcConf_ocf(const char* const sig_name,
                                  const amxc_var_t* const data,
                                  void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sAfcConfigDmHdlrs, sig_name, data, priv);
}

static void s_setHtCapabs_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.HtCapabilities. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: updating HT Capabilities", pRad->Name);
    const char* paramName = amxd_object_get_name(object, AMXD_OBJECT_NAMED);
    SAH_TRACEZ_INFO(ME, "%s: object->name=%s", pRad->Name, paramName);
    whm_mxl_determineRadParamAction(pRad, paramName, NULL);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sHtCapabsDmHdlrs, ARR(), .objChangedCb = s_setHtCapabs_ocf);

void _whm_mxl_rad_setHtCapabs_ocf(const char* const sig_name,
                                  const amxc_var_t* const data,
                                  void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sHtCapabsDmHdlrs, sig_name, data, priv);
}

static void s_setVhtCapabs_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.VhtCapabilities. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: updating VHT Capabilities", pRad->Name);
    const char* paramName = amxd_object_get_name(object, AMXD_OBJECT_NAMED);
    SAH_TRACEZ_INFO(ME, "%s: object->name=%s", pRad->Name, paramName);
    whm_mxl_determineRadParamAction(pRad, paramName, NULL);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sVhtCapabsDmHdlrs, ARR(), .objChangedCb = s_setVhtCapabs_ocf);

void _whm_mxl_rad_setVhtCapabs_ocf(const char* const sig_name,
                                  const amxc_var_t* const data,
                                  void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sVhtCapabsDmHdlrs, sig_name, data, priv);
}

static void s_setBssColor_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.BssColor. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    T_AccessPoint* primaryVap = wld_rad_firstAp(pRad);
    ASSERT_NOT_NULL(primaryVap, , ME, "primaryVap is NULL");
    char cmd[100]= {0};
    char bssColorStr[10]= {0};
    uint32_t countdown = 0;

    if(wld_wpaCtrlInterface_isReady(primaryVap->wpaCtrlInterface)) {
        //for loop on every parameter that was changed in the bss color object
        amxc_var_for_each(newValue, newParamValues) {
            char* valStr = NULL; 
            const char* pname = amxc_var_key(newValue);
            if(swl_str_matches(pname, "AutonomousColorChange")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                wld_ap_hostapd_setParamValue(primaryVap, "autonomous_color_change", valStr, "BssColor parmeter update");
            } else if(swl_str_matches(pname, "ChangeTimeout")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                wld_ap_hostapd_setParamValue(primaryVap, "bss_color_change_timeout", valStr, "BssColor parmeter update");
            } else if(swl_str_matches(pname, "NumCollisionsThreshold")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                wld_ap_hostapd_setParamValue(primaryVap, "num_bss_color_coll_thresh", valStr, "BssColor parmeter update");
            } else if(swl_str_matches(pname, "CollAgeThresh")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                wld_ap_hostapd_setParamValue(primaryVap, "bss_color_coll_age_thresh", valStr, "BssColor parmeter update");
            } else if(swl_str_matches(pname, "UsedColorTableAgeing")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                wld_ap_hostapd_setParamValue(primaryVap, "used_color_table_ageing", valStr, "BssColor parmeter update");
            } else if(swl_str_matches(pname, "HeBssColorRandomize")) {
                pRadVendor->bssColorRandomize = amxc_var_dyncast(bool, newValue);
                if(pRadVendor->bssColorRandomize) {
                    countdown = amxd_object_get_value(uint32_t, object, "SwitchCountdown", NULL);
                    pRadVendor->randomColor = (rand() % HE_OPERATION_BSS_COLOR) + 1;
                    swl_str_catFormat(bssColorStr, sizeof(bssColorStr), "%d",pRadVendor->randomColor);
                    if(countdown) {
                        swl_str_catFormat(cmd, sizeof(cmd), HOSTAPD_COLOR_SWITCH_COUNTDOWN_FORMAT, bssColorStr, countdown);
                    } else {
                        swl_str_catFormat(cmd, sizeof(cmd), HOSTAPD_COLOR_SWITCH_FORMAT, bssColorStr);
                    }
                    whm_mxl_hostapd_sendCommand(primaryVap, cmd, "BssColor parmeter update");
                    amxd_object_set_value(int32_t, object, "HeBssColor", pRadVendor->randomColor);
                }
            } else if(swl_str_matches(pname, "HeBssColor")) {
                valStr = amxc_var_dyncast(cstring_t, newValue);
                countdown = amxd_object_get_value(uint32_t, object, "SwitchCountdown", NULL);
                if(countdown) {
                    swl_str_catFormat(cmd, sizeof(cmd), HOSTAPD_COLOR_SWITCH_COUNTDOWN_FORMAT, valStr, countdown);
                } else {
                    swl_str_catFormat(cmd, sizeof(cmd), HOSTAPD_COLOR_SWITCH_FORMAT, valStr);
                }
                whm_mxl_hostapd_sendCommand(primaryVap, cmd, "BssColor parmeter update");
                pRadVendor->bssColorRandomize = false; // if we entered here it means bss color changed manualy
                amxd_object_set_value(bool, object, "HeBssColorRandomize", pRadVendor->bssColorRandomize);
            } else {
                continue;
            }
            char* pvalue = swl_typeCharPtr_fromVariantDef(newValue, NULL);
            SAH_TRACEZ_INFO(ME, "%s: setting %s = %s", primaryVap->alias, pname, pvalue);
            free(pvalue);
            free(valStr);
        }
    }

    whm_mxl_confModHapd(pRad, primaryVap);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sBssColorDmHdlrs, ARR(), .objChangedCb = s_setBssColor_ocf);

void _whm_mxl_rad_setBssColor_ocf(const char* const sig_name,
                                  const amxc_var_t* const data,
                                  void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sBssColorDmHdlrs, sig_name, data, priv);
}

static void s_setDelayedStartConf_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.DelayedStart. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: update vendor DelayedStart config", pRad->Name);
    amxc_var_for_each(newValue, newParamValues) {
        char* newValStr = NULL;
        const char* pname = amxc_var_key(newValue);
        if(swl_str_matches(pname, "StartAfter")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "StartAfterDelay")) {
            uint32_t startAfterDelay = amxc_var_dyncast(uint32_t, newParamValues);
            swl_str_catFormat(newValStr, sizeof(newValStr), "%u", startAfterDelay);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "StartAfterWatchdogTime")) {
            uint32_t startAfterWatchdogTime = amxc_var_dyncast(uint32_t, newParamValues);
            swl_str_catFormat(newValStr, sizeof(newValStr), "%u", startAfterWatchdogTime);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else {
            continue;
        }
        free(newValStr);
    }

    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sDelayedStartDmHdlrs, ARR(), .objChangedCb = s_setDelayedStartConf_ocf);

void _whm_mxl_rad_setDelayedStartConf_ocf(const char* const sig_name,
                                        const amxc_var_t* const data,
                                        void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sDelayedStartDmHdlrs, sig_name, data, priv);
}

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
#define MAX_ACS_EXCLUSION_LIST_SIZE 2048
swl_rc_ne whm_mxl_rad_startPltfACS(T_Radio* pRad , const amxc_var_t* const args) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, SWL_RC_INVALID_PARAM, ME, "pRadVendor is NULL");
    char* cmd = NULL;
    size_t cmd_len = 0;
    bool first = true;
    uint32_t listCount = 0;
    T_AccessPoint* masterVap = whm_mxl_utils_getMasterVap(pRad);
    ASSERT_NOT_NULL(masterVap, SWL_RC_INVALID_PARAM, ME, "masterVap is NULL");
    bool ctrlIfaceReady = wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface);
    bool radioIsAlive = wld_rad_isActive(pRad);

    amxc_var_t* variant = GET_ARG(args, "acs_list");
    if (variant == NULL || (variant->type_id == AMXC_VAR_ID_LIST && amxc_llist_size(&variant->data.vl) == 0)) {
        SAH_TRACEZ_INFO(ME, "%s: ACS Exclude OpClass/Channel list is empty, unsetting ACS exclusion channel list", pRad->Name);
        if (ctrlIfaceReady && radioIsAlive) {
            whm_mxl_hostapd_sendCommand(masterVap, "ACS_EX_OP_LIST 0 0", "Unset ACS exclusion channel list");
        } else {
            SAH_TRACEZ_ERROR(ME, "%s: Unable to execute ACS exclusion channel list unset command: ctrlIfaceReady=%d, radioIsAlive=%d",
                             pRad->Name, ctrlIfaceReady, radioIsAlive);
        }
        if (pRadVendor->acs_exclusion_ch_list) {
            free(pRadVendor->acs_exclusion_ch_list);
            pRadVendor->acs_exclusion_ch_list = NULL;
        }
        pRadVendor->acs_exclusion_list_count = 0;
        return whm_mxl_confModHapd(pRad, masterVap);
    }

    amxc_var_for_each(data, variant) {
        uint32_t opclass = GET_UINT32(data, "opclass");
        amxc_var_t* exclude_channels_var = GET_ARG(data, "exclude_channels");
        if (exclude_channels_var == NULL) {
            SAH_TRACEZ_ERROR(ME, "exclude_channels_var is NULL");
            return SWL_RC_INVALID_PARAM;
        }

        amxc_llist_t* exclude_channels_list = amxc_var_dyncast(amxc_llist_t, exclude_channels_var);
        amxc_llist_for_each(it, exclude_channels_list) {
            amxc_var_t* item = amxc_var_from_llist_it(it);
            uint8_t channel = amxc_var_dyncast(uint8_t, item);

            if (!wld_rad_hasChannel(pRad, channel)) {
                SAH_TRACEZ_WARNING(ME, "%s: Skip invalid channel %u", pRad->Name, channel);
                continue;
            }

            // Calculate the required length for the new entry
            size_t new_entry_len = snprintf(NULL, 0, "%s%u/%u", first ? "" : " ", opclass, channel) + 1;

            // Reallocate memory for the command string
            char* new_cmd = realloc(cmd, cmd_len + new_entry_len);
            if (new_cmd == NULL) {
                SAH_TRACEZ_ERROR(ME, "Memory allocation failed");
                free(cmd);
                return SWL_RC_ERROR;
            }
            cmd = new_cmd;

            // Append the new entry to the command string
            snprintf(cmd + cmd_len, new_entry_len, "%s%u/%u", first ? "" : ",", opclass, channel);
            cmd_len += new_entry_len - 1;

            first = false;
            listCount++;
        }
    }

    if (cmd) {
        SAH_TRACEZ_INFO(ME, "%s: ACS Exclude OpClass/Channel list: %s", pRad->Name, cmd);
        if (ctrlIfaceReady && radioIsAlive) {
            // If cmd is valid then count should be non-zero
            if (listCount == 0) {
                SAH_TRACEZ_ERROR(ME, "%s: ACS Exclusion list count is zero, cannot set list", pRad->Name);
                free(cmd);
                return SWL_RC_ERROR;
            }
            size_t cmdSize = (swl_str_len(cmd) > MAX_ACS_EXCLUSION_LIST_SIZE) ? MAX_ACS_EXCLUSION_LIST_SIZE : swl_str_len(cmd);
            size_t formatSize = swl_str_len("ACS_EX_OP_LIST") + cmdSize + 32;
            char* formattedCmd = calloc(1, formatSize);
            ASSERT_NOT_NULL(formattedCmd, SWL_RC_ERROR, ME, "failed to allocate cmd buffer");
            swl_str_catFormat(formattedCmd, formatSize, "ACS_EX_OP_LIST %u %s", listCount, cmd);
            whm_mxl_hostapd_sendCommand(masterVap, formattedCmd, "Set ACS exclusion channel list");
            free(formattedCmd);
        } else {
            SAH_TRACEZ_ERROR(ME, "%s: Unable to execute ACS exclusion channel list set command: ctrlIfaceReady=%d, radioIsAlive=%d",
                             pRad->Name, ctrlIfaceReady, radioIsAlive);
        }
        //Setting new ACS exclusion channel list
        pRadVendor->acs_exclusion_ch_list = strdup(cmd);
        pRadVendor->acs_exclusion_list_count = listCount;
        if (pRadVendor->acs_exclusion_ch_list == NULL) {
            SAH_TRACEZ_ERROR(ME, "Failed to allocate memory for ACS exclusion channel list");
            free(cmd);
            return SWL_RC_ERROR;
        }
        whm_mxl_confModHapd(pRad, masterVap);
    }
    free(cmd);
    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

int whm_mxl_rad_autoChannelEnable(T_Radio* pRad, int enable, int set) {
    SAH_TRACEZ_IN(ME);
    int ret = SWL_RC_OK;
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);

    if(set & SET) {
        pRad->autoChannelSetByUser = pRad->autoChannelEnable = enable;

        if (enable == 0) {
            // Unset ACS exclusion channel list when auto channel is disabled
            SAH_TRACEZ_INFO(ME, "%s: Auto channel disabled, unsetting ACS exclusion channel list", pRad->Name);
            whm_mxl_rad_startPltfACS(pRad, NULL);
        }

        if (pRadVendor) {
            whm_mxl_configureBgAcs(pRad, (enable ? pRadVendor->bgAcsInterval : 0));
        }

        if (wld_secDmn_isAlive(pRad->hostapd)) {
            T_AccessPoint* primaryVap = wld_rad_firstAp(pRad);
            ASSERT_NOT_NULL(primaryVap, SWL_RC_INVALID_PARAM, ME, "primaryVap is NULL");
            whm_mxl_hostapd_sendCommand(primaryVap, "RESET_ACS_STATE", "Reset ACS state");
            wld_rad_hostapd_setChannel(pRad);
        }
        wld_rad_doSync(pRad);
    } else {
        ret = pRad->autoChannelEnable;
    }

    SAH_TRACEZ_OUT(ME);
    return ret;
}

static void s_disableAcsBootChannel(T_Radio* pRad) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    ASSERT_TRUE(pRad->hasDmReady, ,ME, "%s: dm not ready", pRad->Name);
    ASSERT_NOT_NULL(pRad->pBus, , ME, "pBus is NULL");
    if (pRad->acsBootChannel != -1) {
        amxd_object_t* chanMgtObj = amxd_object_findf(pRad->pBus, "ChannelMgt");
        ASSERT_NOT_NULL(chanMgtObj, , ME, "chanMgtObj is NULL");
        amxd_trans_t trans;
        ASSERT_TRANSACTION_INIT(chanMgtObj, &trans, , ME, "%s : trans init failure", pRad->Name);
        amxd_trans_set_int32_t(&trans, "AcsBootChannel", -1);
        ASSERT_TRANSACTION_LOCAL_DM_END(&trans, , ME, "%s : trans apply failure", pRad->Name);
    }
    SAH_TRACEZ_OUT(ME);
}

void _whm_mxl_rad_updateAcsBootChannel(const char* const sig_name _UNUSED,
                                       const amxc_var_t* const data,
                                       void* const priv _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.ChannelMgt */
    amxd_object_t* mgtObject = amxd_dm_signal_get_object(get_wld_plugin_dm(), data);
    ASSERTS_NOT_NULL(mgtObject, , ME, "mgtObject is NULL");
    amxd_object_t* radObject = amxd_object_get_parent(mgtObject);
    ASSERTS_NOT_NULL(radObject, , ME, "radObject is NULL");
    T_Radio* pRad = wld_rad_fromObj(radObject);
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    SAH_TRACEZ_INFO(ME, "%s: Schedule disable AcsBootChannel", pRad->Name);
    swla_delayExec_addTimeout((swla_delayExecFun_cbf) s_disableAcsBootChannel, pRad, DM_EVENT_HOOK_TIMEOUT_MS);
    SAH_TRACEZ_OUT(ME);
}
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

swl_rc_ne whm_mxl_rad_setChanspec(T_Radio* pRad, bool direct)
{
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc = SWL_RC_OK;

    SAH_TRACEZ_NOTICE(ME, "%s: tgt chanspec %s, current chanspec %s. requested chanspec: chan <%u> reason <%d> direct <%d>",
                      pRad->Name,
                      swl_typeChanspecExt_toBuf32(pRad->targetChanspec.chanspec).buf,
                      swl_typeChanspecExt_toBuf32(pRad->currentChanspec.chanspec).buf,
                      pRad->channel, pRad->channelChangeReason, direct
                      );

    // Try to check and start ZW DFS, if needed.
    rc = s_checkAndStartZwDfs(pRad, direct);
    SAH_TRACEZ_INFO(ME, "%s: ZwDfs check status: %s", pRad->Name, swl_rc_toString(rc));
    if(rc != SWL_RC_DONE) {
        return rc;
    }

    if (!direct) 
        goto end;

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY 
    /* Handle manual bandwidth or channel change when ACS enabled */
    if (wld_secDmn_isAlive(pRad->hostapd) && pRad->autoChannelEnable && (pRad->channelChangeReason == CHAN_REASON_MANUAL)) {
        if (pRad->channel != pRad->currentChanspec.chanspec.channel) {
            SAH_TRACEZ_INFO(ME, "%s: Disabling ACS due to manual channel was configured", pRad->Name);
            swl_typeUInt8_commitObjectParam(pRad->pBus, "AutoChannelEnable", 0);
        } else {
            T_AccessPoint* primaryVap = wld_rad_firstAp(pRad);
            ASSERT_NOT_NULL(primaryVap, SWL_RC_INVALID_PARAM, ME, "primaryVap is NULL");

            SAH_TRACEZ_INFO(ME, "%s: Toggle hostapd for ACS due to bandwidth change", pRad->Name);
            if(pRad->targetChanspec.chanspec.bandwidth == SWL_BW_320MHZ) {
                if(pRad->operatingChannelBandwidth == SWL_RAD_BW_320MHZ2) {
                    wld_ap_hostapd_setParamValue(primaryVap, "acs_eht_mode", "2", "");
                } else if(pRad->operatingChannelBandwidth == SWL_RAD_BW_320MHZ1) {
                    wld_ap_hostapd_setParamValue(primaryVap, "acs_eht_mode", "1", "");
                }
            }
            wld_rad_hostapd_setChannel(pRad);
            whm_mxl_hostapd_sendCommand(primaryVap, "RESET_ACS_STATE", "Reset ACS state");
            return whm_mxl_toggleHapd(pRad);
        }
    }
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

end:
    CALL_NL80211_FTA_RET(rc, mfn_wrad_setChanspec, pRad, direct);
    SAH_TRACEZ_OUT(ME);
    return rc;
}

int whm_mxl_rad_status(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, 0, ME, "NULL");
    chanmgt_rad_state prevDetState = pRad->detailedState;
    int ret = 0;
    CALL_NL80211_FTA_RET(ret, mfn_wrad_radio_status, pRad);
    if(ret && wld_bgdfs_isRunning(pRad) &&
       wld_rad_hasActiveIface(pRad)) {
        SAH_TRACEZ_INFO(ME, "%s: Restore BG DFS state radRetState(%d -> %d)", pRad->Name, pRad->detailedState, prevDetState);
        pRad->detailedState = prevDetState;
    }
    return ret;
}

swl_rc_ne whm_mxl_rad_regDomain(T_Radio* pRad, char* val, int bufsize, int set) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;

    CALL_NL80211_FTA_RET(rc, mfn_wrad_regdomain, pRad, val, bufsize, set);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    /* The ZWDFS Reg Domain value is derived from the 5G radio
     * Execute only in case Reg Domain is updated during runtime
     * Ignore the ZWDFS Reg Domain update in the init stage, it will be done in s_rad_zwdfsUpdateConfigMap */
    if (wld_rad_is_5ghz(pRad) && (set & SET) && !(set & DIRECT) && wld_rad_areAllVapsDone(pRad)) {
        mxl_rad_updateZwDfsRegDomain(pRad);
    }
    return rc;
}

static void s_mxl_setObssScanConfig_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Radio.{}.Vendor.ObssScanParams. */
    amxd_object_t* radObj = amxd_object_get_parent(amxd_object_get_parent(object));
    T_Radio* pRad = wld_rad_fromObj(radObj);
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    ASSERTS_TRUE(wld_rad_is_24ghz(pRad), , ME, "Not 2.4 GHz radio, OBSS scan not relevant");
    amxc_var_for_each(newValue, newParamValues) {
        char* newValStr = NULL; 
        const char* pname = amxc_var_key(newValue);
        if(swl_str_matches(pname, "ObssInterval")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ScanPassiveDwell")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ScanActiveDwell")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ScanPassiveTotalPerChannel")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ScanActiveTotalPerChannel")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ChannelTransitionDelayFactor")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else if(swl_str_matches(pname, "ScanActivityThreshold")) {
            newValStr = amxc_var_dyncast(cstring_t, newValue);
            whm_mxl_determineRadParamAction(pRad, pname, newValStr);
        } else {
            continue;
        }
        free(newValStr);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sObssScanConfigDmHdlrs, ARR(), .objChangedCb = s_mxl_setObssScanConfig_ocf);

void _whm_mxl_rad_setObssScanParams_ocf(const char* const sig_name,
                                        const amxc_var_t* const data,
                                        void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sObssScanConfigDmHdlrs, sig_name, data, priv);
}

static void s_updateObssInterval(T_Radio* pRad) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    amxd_object_t* pVendorObj = pRadVendor->pBus;
    ASSERT_NOT_NULL(pVendorObj, , ME, "pVendorObj is NULL");
    amxd_object_t* obssObj = amxd_object_get(pVendorObj, "ObssScanParams");
    ASSERT_NOT_NULL(obssObj, , ME, "obssObj is NULL");
    SAH_TRACEZ_NOTICE(ME, "%s: Updating obss interval due to change in ObssCoexistanceEnabled (%d)", pRad->Name, pRad->obssCoexistenceEnabled);
    amxd_trans_t trans;
    ASSERT_TRANSACTION_INIT(obssObj, &trans, , ME, "%s: trans init failure", pRad->Name);
    amxd_trans_set_int32_t(&trans, "ObssInterval", (pRad->obssCoexistenceEnabled ? DEF_OBSS_INTERVAL : 0));
        /* Prepare and do DM transaction */
    amxd_status_t status = swl_object_finalizeTransactionOnLocalDm(&trans);
    ASSERT_TRUE((status == amxd_status_ok), , ME, "%s: transaction to ObssInterval failed ", pRad->Name);
    SAH_TRACEZ_OUT(ME);
}

void _whm_mxl_rad_updateObssCoexistence(const char* const sig_name _UNUSED,
                                    const amxc_var_t* const data,
                                    void* const priv _UNUSED) {
    SAH_TRACEZ_IN(ME);
    amxd_object_t* object = amxd_dm_signal_get_object(get_wld_plugin_dm(), data);
    ASSERTS_NOT_NULL(object, , ME, "object is NULL");
    T_Radio* pRad = (T_Radio*) object->priv;
    ASSERTS_NOT_NULL(pRad, , ME, "pRad is NULL");
    ASSERTS_NOT_NULL(pRad->pBus, , ME, "pBus is NULL");
    ASSERT_FALSE((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN), , ME, "Invalid radio state");
    ASSERTI_TRUE(wld_rad_is_24ghz(pRad), , ME, "%s: ObssCoexistence only handled in 2.4G Radio", pRad->Name);
    SAH_TRACEZ_NOTICE(ME, "%s: ObssCoexistanceEnabled changed event", pRad->Name);
    swla_delayExec_addTimeout((swla_delayExecFun_cbf) s_updateObssInterval, pRad, DM_EVENT_HOOK_TIMEOUT_MS);
    SAH_TRACEZ_OUT(ME);
}

swl_rc_ne whm_mxl_rad_supstd(T_Radio* pRad, swl_radioStandard_m radioStandards) {
    ASSERTS_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "rad is NULL");
    ASSERT_TRUE(swl_radStd_isValid(radioStandards, "rad_supstd"), SWL_RC_INVALID_PARAM, ME,
                "%s rad_supstd : Invalid operatingStandards %#x",
                pRad->Name, radioStandards);
    swl_rc_ne rc;

    CALL_NL80211_FTA_RET(rc, mfn_wrad_supstd, pRad, radioStandards);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    if (wld_secDmn_isAlive(pRad->hostapd)) {
        if (wld_rad_is_24ghz(pRad)) {
            amxc_llist_for_each(it, &pRad->llAP) {
                T_AccessPoint* pAP = amxc_llist_it_get_data(it, T_AccessPoint, it);
                mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
                if ((pAP != NULL) && (mxlVapVendorData != NULL)) {
                    if (whm_mxl_utils_isDummyVap(pAP))
                        continue;
                    if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_N) && mxlVapVendorData->vendorVht) {
                        wld_ap_hostapd_setParamValue(pAP, "vendor_vht", "1", "");
                    } else {
                        wld_ap_hostapd_setParamValue(pAP, "vendor_vht", "0", "");
                    }
                }
            }
        }
    }
    return rc;
}

bool whm_mxl_rad_setCtrlSockSyncNeeded(T_Radio* pRad, bool flag) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendor is NULL");
    pRadVendor->checkWpaCtrlOnSync = flag;
    return true;
}

bool whm_mxl_rad_isCtrlSockSyncNeeded(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendor is NULL");
    return pRadVendor->checkWpaCtrlOnSync;
}

/**
 * @brief Request to reconf all changed BSS from reconf FSM
 *
 * @param T_Radio radio
 * @return none.
 */
void whm_mxl_rad_requestReconf(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERTI_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    setBitLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF);
    whm_mxl_reconfMngr_notifyCommit(pRad);
}

/**
 * @brief Request sync action from reconf FSM
 *
 * @param T_Radio radio
 * @return none.
 */
void whm_mxl_rad_requestSync(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERTI_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    setBitLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_SYNC);
    whm_mxl_reconfMngr_notifyCommit(pRad);
}

/**
 * @brief FTA Handler to fetch current transmit power in dBm
 *
 * @param T_Radio* Pointer to the radio
 * @param int32_t* Pointer to store the current transmit power in dBm
 * @return swl_rc_ne SWL_RC_OK on success, SWL_RC_ERROR code otherwise
 */
swl_rc_ne whm_mxl_rad_getTxPowerdBm(T_Radio* pRad, int32_t* dbm) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_TRUE(wld_rad_hasActiveIface(pRad), SWL_RC_ERROR, ME, "%s not ready", pRad->Name);
    uint32_t ifIndex = wld_rad_getFirstEnabledIfaceIndex(pRad);
    ASSERT_TRUE(ifIndex > 0, SWL_RC_ERROR, ME, "%s: rad has no enabled iface", pRad->Name);

    struct mxl_vendor_tx_power txPowerData;
    struct cbData_t getData = { .size = sizeof(struct mxl_vendor_tx_power), .data = &txPowerData };
    swl_rc_ne rc = SWL_RC_ERROR;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, LTQ_NL80211_VENDOR_SUBCMD_GET_20MHZ_TX_POWER, NULL, 0,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getDataCb, &getData);
    ASSERTI_FALSE(rc < SWL_RC_OK, SWL_RC_ERROR, ME, "Failed to call LTQ_NL80211_VENDOR_SUBCMD_GET_20MHZ_TX_POWER");
    *dbm = txPowerData.cur_tx_power;
    SAH_TRACEZ_INFO(ME, "%s: Received Current Tx Power of %d", pRad->Name, *dbm);

    return SWL_RC_OK;
}

/**
 * @brief FTA Handler to fetch maximum transmit power of a channel in dBm
 *
 * @param T_Radio* rad pointer to the radio
 * @param uint16_t channel channel number
 * @param int32_t* dbm pointer to store the current transmit power in dBm
 * @return swl_rc_ne SWL_RC_OK on success, error code otherwise
 */
swl_rc_ne whm_mxl_rad_getMaxTxPowerdBm(T_Radio* pRad, uint16_t channel, int32_t* dbm) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_TRUE(wld_rad_hasActiveIface(pRad), SWL_RC_ERROR, ME, "%s not ready", pRad->Name);
    uint32_t ifIndex = wld_rad_getFirstEnabledIfaceIndex(pRad);
    ASSERT_TRUE(ifIndex > 0, SWL_RC_ERROR, ME, "%s: rad has no enabled iface", pRad->Name);

    wave_wssa_max_tx_power_stats_t maxTxPowerData;
    struct cbData_t getData = { .size = sizeof(wave_wssa_max_tx_power_stats_t), .data = &maxTxPowerData };
    swl_rc_ne rc = SWL_RC_ERROR;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, LTQ_NL80211_VENDOR_SUBCMD_GET_MAX_TX_POWER, &channel, sizeof(channel),
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getDataCb, &getData);
    ASSERTI_FALSE(rc < SWL_RC_OK, SWL_RC_ERROR, ME, "Failed to call LTQ_NL80211_VENDOR_SUBCMD_GET_MAX_TX_POWER");
    ASSERT_TRUE(maxTxPowerData.channel == channel, SWL_RC_ERROR, ME, "%s: channel mismatch: expected %u, got %u",
                 pRad->Name, channel, maxTxPowerData.channel);
    *dbm = maxTxPowerData.max_tx_power;
    SAH_TRACEZ_INFO(ME, "%s: Received Max Tx Power of %d for channel %d", pRad->Name, *dbm, channel);

    return SWL_RC_OK;
}

/**
 * @brief Checks if the radio needs to force-enable 802.11be mode
 *
 * @note Ideally, this should be handled by pWHM. This won't be necessary once
 * MxL aligns with pWHM's approach to handling BE mode and MLO.
 *
 * @param T_Radio* rad pointer to the radio
 * @return bool true if BE mode should be enabled, false otherwise.
 */
bool whm_mxl_rad_checkForceEnableBe(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, SWL_RC_INVALID_PARAM, ME, "pRadVendor is NULL");
    bool isRadBeEnabled = wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_BE);

    return isRadBeEnabled && pRadVendor->enable80211BeOverride;
}
