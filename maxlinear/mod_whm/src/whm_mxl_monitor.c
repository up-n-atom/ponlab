/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_monitor.c                                     *
*         Description  : Monitoring related API                                *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"

#include "wld/wld_radio.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_nl80211_utils.h"
#include "wld/wld_rad_nl80211.h"

#include "whm_mxl_utils.h"
#include "whm_mxl_evt.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_parser.h"
#include "whm_mxl_monitor.h"

#include <vendor_cmds_copy.h>

#define ME "mxlMon"

#define NASTA_ENTRIES_MAX                       (32)
#define SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS        (20)
#define SCAN_TIMEOUT_TOTAL_DEFAULT_MS           ((NASTA_ENTRIES_MAX * SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS) + 1000U)
#define SCAN_TIMEOUT_PER_CHAN_SAFETY_MULTIPLIER (10)


mxl_nastaEntryData_t* whm_mxl_monitor_fetchRunNaStaEntry(T_Radio* pRad, swl_macBin_t* pStaMac) {
    ASSERTS_NOT_NULL(pStaMac, NULL, ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(vendorData, NULL, ME, "NULL");
    amxc_llist_for_each(it, &vendorData->naSta.scanList) {
        mxl_nastaEntryData_t* pEntry = amxc_container_of(it, mxl_nastaEntryData_t, it);
        if(swl_typeMacBin_equalsRef(&pEntry->mac, pStaMac)) {
            return pEntry;
        }
    }
    return NULL;
}

mxl_nastaEntryData_t* whm_mxl_monitor_addRunNaStaEntry(T_Radio* pRad, swl_macBin_t* pStaMac) {
    ASSERTS_NOT_NULL(pStaMac, NULL, ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(vendorData, NULL, ME, "NULL");
    mxl_nastaEntryData_t* pEntry = whm_mxl_monitor_fetchRunNaStaEntry(pRad, pStaMac);
    ASSERTS_NULL(pEntry, pEntry, ME, "Found");
    pEntry = calloc(1, sizeof(mxl_nastaEntryData_t));
    ASSERT_NOT_NULL(pEntry, pEntry, ME, "NULL");
    memcpy(&pEntry->mac, pStaMac, sizeof(swl_macBin_t));
    pEntry->startTs = swl_timespec_getMonoVal();
    amxc_llist_append(&vendorData->naSta.scanList, &(pEntry->it));
    return pEntry;
}

void whm_mxl_monitor_delRunNaStaEntry(mxl_nastaEntryData_t* pEntry) {
    ASSERTS_NOT_NULL(pEntry, , ME, "NULL");
    amxc_llist_it_take(&pEntry->it);
    free(pEntry);
}

void whm_mxl_monitor_dropAllRunNaStaEntries(T_Radio* pRad) {
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(vendorData, , ME, "NULL");
    amxc_llist_for_each(it, &vendorData->naSta.scanList) {
        mxl_nastaEntryData_t* pEntry = amxc_container_of(it, mxl_nastaEntryData_t, it);
        whm_mxl_monitor_delRunNaStaEntry(pEntry);
    }
}

uint32_t whm_mxl_monitor_getRunNaStaEntryCount(T_Radio* pRad) {
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(vendorData, 0, ME, "NULL");
    return amxc_llist_size(&vendorData->naSta.scanList);
}

void whm_mxl_monitor_checkRunNaStaList(T_Radio* pRad) {
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERTS_NOT_NULL(vendorData, , ME, "NULL");
    amxc_llist_for_each(it, &vendorData->naSta.scanList) {
        mxl_nastaEntryData_t* pEntry = amxc_container_of(it, mxl_nastaEntryData_t, it);
        swl_timeSpecMono_t currTs = swl_timespec_getMonoVal();
        swl_timeSpecMono_t diffTs;
        swl_timespec_diff(&diffTs, &pEntry->startTs, &currTs);
        //safety delay: at most twice the scan time per channel
        if(swl_timespec_toMs(&diffTs) > (SCAN_TIMEOUT_PER_CHAN_SAFETY_MULTIPLIER * vendorData->naSta.scanTimeout)) {
            SAH_TRACEZ_WARNING(ME, "expired scan entry %s: clean it up", swl_typeMacBin_toBuf32Ref(&pEntry->mac).buf);
            whm_mxl_monitor_delRunNaStaEntry(pEntry);
        }
    }
    if(amxc_llist_is_empty(&vendorData->naSta.scanList)) {
        amxp_timer_stop(vendorData->naSta.timer);
    }
}

int whm_mxl_monitor_setupStamon(T_Radio* pRad, bool enable) {
    if(!enable) {
        whm_mxl_monitor_dropAllRunNaStaEntries(pRad);
        whm_mxl_monitor_checkRunNaStaList(pRad);
    } else {
        whm_mxl_monitor_setStaScanTimeOut(pRad, SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS);
    }
    return SWL_RC_OK;
}

static void s_scanTimeoutHandler(amxp_timer_t* timer _UNUSED, void* data) {
    (void) timer;
    T_Radio* pRad = (T_Radio*) data;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");

    // stop request as timeout is reached
    SAH_TRACEZ_WARNING(ME, "unconnected sta timeout reached");
    whm_mxl_monitor_dropAllRunNaStaEntries(pRad);
    whm_mxl_monitor_checkRunNaStaList(pRad);
}

static swl_rc_ne s_getStaScanTimeOutCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_Radio* pRad = (T_Radio*) priv;
    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, SWL_RC_ERROR, ME, "NULL");
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    uint32_t scanTimeout = nla_get_u32(tb[NL80211_ATTR_VENDOR_DATA]);
    SAH_TRACEZ_INFO(ME, "%s: Nasta scan timeout is %d", pRad->Name, scanTimeout);
    if(!scanTimeout) {
        vendorData->naSta.scanTimeout = SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS;
    } else {
        vendorData->naSta.scanTimeout = scanTimeout;
    }

    return rc;
}

int whm_mxl_monitor_getStaScanTimeOut(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, WLD_ERROR_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_UNCONNECTED_STA_SCAN_TIME;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, NULL, 0,
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getStaScanTimeOutCb, pRad);
    return rc;
}

int whm_mxl_monitor_setStaScanTimeOut(T_Radio* pRad, uint32_t scanTime) {
    ASSERT_NOT_NULL(pRad, WLD_ERROR_INVALID_PARAM, ME, "NULL");
    swl_rc_ne rc;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_UNCONNECTED_STA_SCAN_TIME;
    rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, (char*) &scanTime, sizeof(scanTime),
                                          VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
    return rc;
}

void whm_mxl_monitor_init(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
    amxp_timer_new(&vendorData->naSta.timer, s_scanTimeoutHandler, pRad);
    whm_mxl_monitor_getStaScanTimeOut(pRad);
}

void whm_mxl_monitor_deinit(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
    whm_mxl_monitor_setupStamon(pRad, false);
    amxp_timer_delete(&vendorData->naSta.timer);
}

static swl_rc_ne s_naStaStatsCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");
    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    T_Radio* pRad = (T_Radio*) priv;
    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "pRad is NULL");

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    if (mxl_parseNaStaStats(pRad, tb, NASTA_STATS_REQ_SYNC, false) == SWL_RC_OK) {
        whm_mxl_monitor_checkRunNaStaList(pRad);
    }

    return rc;
}

static swl_rc_ne s_getNaStaStats(T_Radio* pRad, T_NonAssociatedDevice* pMD, uint8_t reqType) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    ASSERT_NOT_NULL(pMD, SWL_RC_INVALID_PARAM, ME, "pMD is NULL");
    swl_rc_ne rc = SWL_RC_OK;
    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_UNCONNECTED_STA;
    swl_chanspec_t chanspec = wld_rad_getSwlChanspec(pRad);
    struct intel_vendor_unconnected_sta_req_cfg scanReq;

    if(pMD->channel) {
        chanspec.channel = pMD->channel;
    }
    if(pMD->operatingClass) {
        chanspec.band = swl_chanspec_operClassToFreq(pMD->operatingClass);
        chanspec.bandwidth = swl_chanspec_operClassToBandwidth(pMD->operatingClass);
    }

    int freq = wld_channel_getFrequencyOfChannel(chanspec);
    int center_channel = wld_channel_get_center_channel(chanspec);
    chanspec.channel = center_channel;
    int center_freq = wld_channel_getFrequencyOfChannel(chanspec);
    int nlBw = wld_nl80211_bwSwlToNl(chanspec.bandwidth);

    memset(&scanReq, 0, sizeof(scanReq));
    memcpy(scanReq.addr, pMD->MACAddress, sizeof(pMD->MACAddress));
    scanReq.bandwidth = nlBw;
    scanReq.freq = freq;
    scanReq.center_freq1 = center_freq;
    scanReq.req_type = reqType;

    SAH_TRACEZ_INFO(ME, "send request bw(nl:%d,swl:%d) chan(%d) freq(%d) cf1(%d) reqType(%d)",
                nlBw, chanspec.bandwidth, chanspec.channel, freq, center_freq, reqType);
    if (reqType == NASTA_STATS_REQ_ASYNC) {
        rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, &scanReq, sizeof(struct intel_vendor_unconnected_sta_req_cfg),
                                            VENDOR_SUBCMD_IS_ASYNC, VENDOR_SUBCMD_IS_WITH_ACK, 0, NULL, NULL);
    } else if (reqType == NASTA_STATS_REQ_SYNC) {
        rc = wld_rad_nl80211_sendVendorSubCmd(pRad, OUI_MXL, subcmd, &scanReq, sizeof(struct intel_vendor_unconnected_sta_req_cfg),
                                            VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_naStaStatsCb, pRad);
    } else {
        SAH_TRACEZ_NOTICE(ME, "%s: invalid nasta req type %d", pRad->Name, reqType);
        rc = SWL_RC_ERROR;
    }

    return rc;
}

int whm_mxl_monitor_updateMonStats(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_TRUE(pRad->stationMonitorEnabled, SWL_RC_INVALID_STATE, ME, "Station monitor disabled");
    ASSERT_TRUE(wld_rad_isUpAndReady(pRad), SWL_RC_INVALID_STATE, ME, "%s: radio state not ready for scan", pRad->Name);

    swl_rc_ne rc;
    uint32_t currStaCount = 0;

    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, SWL_RC_INVALID_PARAM, ME, "NULL");

    whm_mxl_monitor_checkRunNaStaList(pRad);

    if(vendorData->naSta.scanTimeout == 0) {
        whm_mxl_monitor_getStaScanTimeOut(pRad);
    }
    ASSERT_TRUE(vendorData->naSta.scanTimeout, SWL_RC_OK, ME, "%s: STA SCAN TIME is 0", pRad->Name);
    if(vendorData->naSta.scanTimeout != SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS) {
        SAH_TRACEZ_ERROR(ME, "%s: Setting to default NaSta scan time", pRad->Name);
        whm_mxl_monitor_setStaScanTimeOut(pRad, SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS);
        whm_mxl_monitor_getStaScanTimeOut(pRad);
    }

    amxc_llist_for_each(it, &pRad->naStations) {
        mxl_nastaEntryData_t* pEntry = NULL;
        T_NonAssociatedDevice* pMD = amxc_container_of(it, T_NonAssociatedDevice, it);
        if(!pMD || ((pEntry = whm_mxl_monitor_fetchRunNaStaEntry(pRad, (swl_macBin_t*) pMD->MACAddress)) != NULL)) {
            continue;
        }
        if(currStaCount > NASTA_ENTRIES_MAX) {
            SAH_TRACEZ_WARNING(ME, "%s: Number of non-associated devices scan limit reached", pRad->Name);
            break;
        }
        pEntry = whm_mxl_monitor_addRunNaStaEntry(pRad, (swl_macBin_t*) pMD->MACAddress);
        if(pEntry == NULL) {
            continue;
        }

        rc = s_getNaStaStats(pRad, pMD, NASTA_STATS_REQ_SYNC);
        if(rc < SWL_RC_OK) {
            whm_mxl_monitor_delRunNaStaEntry(pEntry);
        }
        currStaCount++;
    }

    uint32_t runCount = whm_mxl_monitor_getRunNaStaEntryCount(pRad);
    uint32_t delay = SWL_MAX((vendorData->naSta.scanTimeout * SCAN_TIMEOUT_PER_CHAN_SAFETY_MULTIPLIER) * runCount, SCAN_TIMEOUT_TOTAL_DEFAULT_MS);
    if(runCount > 0) {
        amxp_timer_start(vendorData->naSta.timer, delay /* ms */);
    }

    return SWL_RC_OK;
}

int whm_mxl_monitor_updateNaStaObj(T_Radio* pRad){

    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    amxd_trans_t trans;
    ASSERT_TRANSACTION_INIT(pRad->pBus, &trans, SWL_RC_ERROR, ME, "%s : trans init failure", pRad->Name);

    amxd_object_t* naStaMonObject = amxd_object_findf(get_wld_object(), "Radio.%u.NaStaMonitor", pRad->ref_index+1);
    ASSERT_NOT_NULL(naStaMonObject, SWL_RC_ERROR, ME, "No naStaMonObject object found");
    
    amxd_object_t* naDevObject = amxd_object_get(naStaMonObject, "NonAssociatedDevice");
    ASSERT_NOT_NULL(naDevObject, SWL_RC_ERROR, ME, "No naDevObject object found");

    amxd_object_for_each(instance, it, naDevObject) {
        amxd_object_t* instance = amxc_container_of(it, amxd_object_t, it);
        if(instance == NULL) {
            continue;
        }
        wld_nasta_t* pMD = (wld_nasta_t*) instance->priv;
        if(!pMD) {
            continue;
        }
        amxd_trans_select_object(&trans, instance);
        amxd_trans_set_int32_t(&trans, "SignalStrength", pMD->SignalStrength);
        amxd_trans_set_uint8_t(&trans, "Channel", pMD->channel);
        amxd_trans_set_uint8_t(&trans, "OperatingClass", pMD->operatingClass);
        swl_typeTimeMono_toTransParam(&trans, "TimeStamp", pMD->TimeStamp);
    }

    ASSERT_TRANSACTION_LOCAL_DM_END(&trans, SWL_RC_ERROR, ME, "%s : trans apply failure", pRad->Name);

    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_monitor_addStaMon(T_Radio* pRad, T_NonAssociatedDevice* pMD) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    ASSERT_NOT_NULL(pMD, SWL_RC_INVALID_PARAM, ME, "pMD is NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, SWL_RC_INVALID_PARAM, ME, "vendorData is NULL");

    SAH_TRACEZ_INFO(ME, "%s: Add NaSta MAC (%.2X:%.2X:%.2X:%.2X:%.2X:%.2X) channel (%d) opclass (%d)", pRad->Name,
                pMD->MACAddress[0], pMD->MACAddress[1], pMD->MACAddress[2],
                pMD->MACAddress[3], pMD->MACAddress[4], pMD->MACAddress[5], pMD->channel, pMD->operatingClass);
    
    if(vendorData->naSta.scanTimeout != SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS) {
        SAH_TRACEZ_NOTICE(ME, "%s: Setting default NaSta scan time - on NaSta add", pRad->Name);
        whm_mxl_monitor_setStaScanTimeOut(pRad, SCAN_TIMEOUT_PER_CHAN_DEFAULT_MS);
        whm_mxl_monitor_getStaScanTimeOut(pRad);
    }

    return SWL_RC_OK;
}

int whm_mxl_monitor_delStamon(T_Radio* pRad, T_NonAssociatedDevice* pMD) {
    ASSERT_NOT_NULL(pMD, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_nastaEntryData_t* pEntry = whm_mxl_monitor_fetchRunNaStaEntry(pRad, (swl_macBin_t*) pMD->MACAddress);
    ASSERTI_NOT_NULL(pEntry, SWL_RC_OK, ME, "Not found");
    whm_mxl_monitor_delRunNaStaEntry(pEntry);
    whm_mxl_monitor_checkRunNaStaList(pRad);
    return SWL_RC_OK;
}
