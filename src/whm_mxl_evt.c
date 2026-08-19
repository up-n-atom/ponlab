/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_evt.c                                         *
*         Description  : Vendor events callback                                *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"

#include "wld/wld_radio.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_nl80211_attr.h"
#include "wld/wld_nl80211_events.h"
#include "wld/wld_chanmgt.h"
#include "wld/wld_util.h"

#include "whm_mxl_utils.h"
#include <vendor_cmds_copy.h>

#include "whm_mxl_rad.h"
#include "whm_mxl_parser.h"
#include "whm_mxl_monitor.h"
#include "whm_mxl_evt.h"

#define ME "mxlEvt"

static void s_updateNewChanspec(T_Radio* pRad, swl_chanspec_t* pChanSpec, wld_channelChangeReason_e reason) {
    ASSERTS_NOT_NULL(pRad, , ME, "NULL");
    ASSERTS_NOT_NULL(pChanSpec, , ME, "NULL");
    ASSERTS_TRUE(pChanSpec->channel > 0, , ME, "invalid chan");
    swl_rc_ne rc = wld_chanmgt_reportCurrentChanspec(pRad, *pChanSpec, reason);
    ASSERTS_FALSE(rc < SWL_RC_OK, , ME, "no changes to be reported");
    wld_rad_updateOperatingClass(pRad);
}

static void s_vendorEvtCb(void* pRef, void* pData _UNUSED, struct nlmsghdr* nlh, struct nlattr* tb[]) {
    T_Radio* pRad = (T_Radio*) pRef;
    ASSERT_NOT_NULL(pRad, , ME, "pRad NULL");
    ASSERT_NOT_NULL(tb, , ME, "tb NULL");

    int64_t subcmd = -1;

    SAH_TRACEZ_INFO(ME, "%s: treat vendor event data", pRad->Name);

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERT_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, , ME, "unexpected cmd %d", gnlh->cmd);

    SAH_TRACEZ_INFO(ME, "%s: nlh->nlmsg_len %d", pRad->Name, nlh->nlmsg_len);

    if(tb[NL80211_ATTR_VENDOR_ID]) {
        uint32_t vendorId = nla_get_u32(tb[NL80211_ATTR_VENDOR_ID]);
        SAH_TRACEZ_INFO(ME, "%s: vendorId 0x%04x", pRad->Name, vendorId);
    }

    if(tb[NL80211_ATTR_VENDOR_SUBCMD]) {
        subcmd = nla_get_u32(tb[NL80211_ATTR_VENDOR_SUBCMD]);
        SAH_TRACEZ_INFO(ME, "%s: subcmd %"PRId64"", pRad->Name, subcmd);
    }

    switch(subcmd) {
    case LTQ_NL80211_VENDOR_EVENT_UNCONNECTED_STA: {
        SAH_TRACEZ_INFO(ME, "%s: parse NaSta event", pRad->Name);
        if(mxl_parseNaStaStats(pRad, tb, NASTA_STATS_REQ_ASYNC, true) == SWL_RC_OK) {
            whm_mxl_monitor_checkRunNaStaList(pRad);
        }
        break;
    }
    case LTQ_NL80211_VENDOR_EVENT_CHAN_DATA: {
        SAH_TRACEZ_INFO(ME, "%s: received chan data event", pRad->Name);
        mxl_parseChanDataEvt(pRad, tb);
        break;
    }
    case LTQ_NL80211_VENDOR_EVENT_CSI_STATS: {
        SAH_TRACEZ_INFO(ME, "%s: received csi stats event", pRad->Name);
        mxl_parseCsiStatsEvt(pRad, tb);
        break;
    }
    default: {
        SAH_TRACEZ_INFO(ME, "%s: unknown vendor event %"PRId64"", pRad->Name, subcmd);
        break;
    }
    }
}

static T_Radio* s_mxl_fetchRadio(void* userData, char* ifname) {
    if (!swl_str_isEmpty(ifname)) {
        T_AccessPoint* pAP = wld_vap_from_name(ifname);
        if (pAP != NULL) {
            if (pAP->pRadio != NULL) {
                return pAP->pRadio;
            }
        } else {
            T_EndPoint* pEP = wld_vep_from_name(ifname);
            if (pEP != NULL) {
                if (pEP->pRadio != NULL) {
                    return pEP->pRadio;
                }
            }
        }
    }

    if (debugIsRadPointer(userData))
        return (T_Radio*) userData;

    return NULL;
}

static void mxl_6ghz_chanspec_from_centreFreq(swl_chanspec_t* chanSpec, int32_t centrFreq) {
    int32_t centrChannel = 0;

    /*Fetching the centre channel from the centre freq received from the ACS-COMPLETED event*/
    centrChannel = (centrFreq - SWL_CHANNEL_6G_FREQ_START) / SWL_CHANNEL_INTER_FREQ_5MHZ;

    /**
    * Updating the extensionHigh variable in the swl_chanspec_t structure
    * Based on the centre channel in case of 320MHz
    **/
    if(chanSpec->bandwidth == SWL_BW_320MHZ) {
        if(((centrChannel - 63) % 64) == 0) {
            chanSpec->extensionHigh = SWL_CHANSPEC_EXT_HIGH;
        } else {
            chanSpec->extensionHigh = SWL_CHANSPEC_EXT_LOW;
        }
    }
}

static void s_mxl_ACSCompletedEvt(void* userData, char* ifName, char* event _UNUSED, char* params) {
    T_Radio* pRad = s_mxl_fetchRadio(userData, ifName);
    ASSERT_NOT_NULL(pRad, , ME, "Could not get radio from ifname(%s)", ifName);
    ASSERTI_NOT_EQUALS(pRad, mxl_rad_getZwDfsRadio(), , ME, "ignore acs event on zwdfs radio (ifname:%s, radName:%s)",
                       ifName, pRad->Name);

    // expected msg format:
    // <3>ACS-COMPLETED freq=2412 channel=1 OperatingChannelBandwidt=20 ExtensionChannel=0 cf1=2412 cf2=0 reason=UNKNOWN dfs_chan=0
    swl_chanspec_t chanSpec = SWL_CHANSPEC_EMPTY;
    int32_t freq = 0;
    int32_t channel = 0;
    int32_t operCbw = 0;
    int32_t centreFreq = 0;

    ASSERTW_EQUALS(pRad->autoChannelEnable, true, , ME, "%s: ACS event when ACS not enabled", pRad->Name);

    chanSpec.band = pRad->operatingFrequencyBand;
    if (!wld_wpaCtrl_getValueIntExt(params, "freq", &freq)) {
        SAH_TRACEZ_ERROR(ME, "%s: cannot get frequency", pRad->Name);
        return;
    }

    swl_chanspec_channelFromMHz(&chanSpec, freq);
    ASSERT_EQUALS(pRad->operatingFrequencyBand, chanSpec.band, , ME, "%s: unmatched radio freqBand(%s)", pRad->Name, swl_freqBandExt_str[chanSpec.band]);

    if (!wld_wpaCtrl_getValueIntExt(params, "channel", &channel) || !channel) {
        SAH_TRACEZ_ERROR(ME, "%s: cannot get channel", pRad->Name);
        return;
    }

    if (!wld_wpaCtrl_getValueIntExt(params, "OperatingChannelBandwidt", &operCbw) || !operCbw) {
        SAH_TRACEZ_ERROR(ME, "%s: cannot get bandwidth", pRad->Name);
        return;
    }

    if (!wld_wpaCtrl_getValueIntExt(params, "cf1", &centreFreq)) {
        SAH_TRACEZ_ERROR(ME, "%s: cannot get frequency", pRad->Name);
        return;
    }

    chanSpec.bandwidth = swl_chanspec_intToBw(operCbw);
    chanSpec.channel = channel;

    if(wld_rad_is_6ghz(pRad)) {
        /* Updating the chanspec in case of 320MHZ based on centre frequency*/
        mxl_6ghz_chanspec_from_centreFreq(&chanSpec, centreFreq);
    }

    /* Update all channel instances with new channel in case ACS enabled, otherwise
     * manual channel switch to different channel later (without disabling ACS prior)
     * may not work as expected */
    SAH_TRACEZ_INFO(ME, "%s: ACS-COMPLETED Updating current chanspec %s", pRad->Name, swl_typeChanspecExt_toBuf32(chanSpec).buf);
    s_updateNewChanspec(pRad, &chanSpec, CHAN_REASON_AUTO);
}

SWL_TABLE(mxl_WpaCtrlEvents,
          ARR(char* evtName; void* evtParser; ),
          ARR(swl_type_charPtr, swl_type_voidPtr),
          ARR(
              {"ACS-COMPLETED", &s_mxl_ACSCompletedEvt},
              ));

static evtParser_f s_mxl_getEventParser(char* eventName) {
    evtParser_f* pfEvtHdlr = (evtParser_f*) swl_table_getMatchingValue(&mxl_WpaCtrlEvents, 1, 0, eventName);
    ASSERTS_NOT_NULL(pfEvtHdlr, NULL, ME, "no handler defined for evt(%s)", eventName);
    return *pfEvtHdlr;
}

static void s_mxl_WpaCtrlEvtMsg(void* userData, char* ifName, char* msgData) {
    ASSERTS_STR(msgData, , ME, "NULL or no content msgData");
    char* pEvent = strstr(msgData, WPA_MSG_LEVEL_INFO);
    ASSERTS_NOT_NULL(pEvent, , ME, "Not a valid WPA ctrl event");
    pEvent += sizeof(WPA_MSG_LEVEL_INFO) - 1;

    uint32_t eventNameLen = strlen(pEvent);
    char* pParams = strchr(pEvent, ' ');
    if (pParams) {
        eventNameLen = pParams - pEvent;
        pParams++;
    }
    char eventName[eventNameLen + 1];
    swl_str_copy(eventName, sizeof(eventName), pEvent);
    evtParser_f fEvtHdlr = s_mxl_getEventParser(eventName);
    ASSERTS_NOT_NULL(fEvtHdlr, , ME, "%s: No parser for evt(%s)", ifName, eventName);

    SAH_TRACEZ_INFO(ME, "%s: receive msg '%s'", ifName, msgData);
    fEvtHdlr(userData, ifName, eventName, pParams);
}

static void s_mxl_ObssCoexBwChngd(T_Radio* pRad, uint32_t channel, uint32_t operCbw) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    swl_chanspec_t newChanSpec = SWL_CHANSPEC_EMPTY;
    ASSERT_TRUE(wld_rad_is_24ghz(pRad), , ME, "%s: Received OBSS BW change event for wrong band", pRad->Name);
    pRad->targetChanspec.reason = CHAN_REASON_OBSS_COEX;
    pRad->obssCoexistenceActive = true;
    /* Prepare and update new chanspec */
    ASSERT_TRUE(wld_rad_hasChannel(pRad, channel), , ME, "%s: Bad channel(%d) from parsed event", pRad->Name, channel);
    newChanSpec.bandwidth = swl_chanspec_intToBw(operCbw);
    newChanSpec.channel = channel;
    SAH_TRACEZ_NOTICE(ME, "%s: Update chanspec due to 20/40 coexistence on channel(%d)", pRad->Name, channel);
    s_updateNewChanspec(pRad, &newChanSpec, CHAN_REASON_OBSS_COEX);
}

static swl_rc_ne s_mxl_apBwChanged(void* userData, char* ifName _UNUSED, char* event _UNUSED, char* params _UNUSED) {
    /* Expected msg format:
     * <3>AP-BW-CHANGED freq=%d Channel=%d OperatingChannelBandwidth=%d ExtensionChannel=%d cf1=%d cf2=%d reason=%s dfs_chan=%d
     */
    T_Radio* pRad = (T_Radio*) userData;
    uint32_t channel = wld_wpaCtrl_getValueInt(params, "Channel");
    uint32_t operCbw = wld_wpaCtrl_getValueInt(params, "OperatingChannelBandwidth");
    char reason[64] = {0};
    if (wld_wpaCtrl_getValueStr(params, "reason", reason, sizeof(reason)) > 0) {
        if (swl_str_matches(reason, "OBSS")) {
            s_mxl_ObssCoexBwChngd(pRad, channel, operCbw);
        }
    }
    return SWL_RC_DONE;
}

SWL_TABLE(sMxlChWidthMaps,
          ARR(uint32_t chWidthId; char* chWidthDesc; swl_bandwidth_e swlBw; ),
          ARR(swl_type_uint32, swl_type_charPtr, swl_type_uint32, ),
          ARR({0, "20 MHz (no HT)", SWL_BW_20MHZ}, //CHAN_WIDTH_20_NOHT
              {1, "20 MHz", SWL_BW_20MHZ},         //CHAN_WIDTH_20
              {2, "40 MHz", SWL_BW_40MHZ},         //CHAN_WIDTH_40
              {3, "80 MHz", SWL_BW_80MHZ},         //CHAN_WIDTH_80
              {4, "80+80 MHz", SWL_BW_160MHZ},     //CHAN_WIDTH_80P80
              {5, "160 MHz", SWL_BW_160MHZ},       //CHAN_WIDTH_160
              {10, "320 MHz", SWL_BW_320MHZ},      //CHAN_WIDTH_320
              ));

static swl_rc_ne s_freqParamToChanSpec(char* params, const char* key, swl_chanspec_t* pChanSpec) {
    ASSERTS_STR(params, SWL_RC_INVALID_PARAM, ME, "Empty");
    ASSERTS_NOT_NULL(pChanSpec, SWL_RC_INVALID_PARAM, ME, "NULL");
    uint32_t ctrlFreq = 0;
    ASSERTS_TRUE(wld_wpaCtrl_getValueIntExt(params, key, (int32_t*) &ctrlFreq), SWL_RC_ERROR,
                 ME, "Missing %s param", key);
    swl_chanspec_t chanSpec;
    swl_rc_ne rc = swl_chanspec_channelFromMHz(&chanSpec, ctrlFreq);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get chanspec for freq(%d)", ctrlFreq);
    pChanSpec->channel = chanSpec.channel;
    pChanSpec->band = chanSpec.band;
    return SWL_RC_OK;
}

static swl_rc_ne s_chWidthIdToChanSpec(char* params, const char* key, swl_chanspec_t* pChanSpec) {
    ASSERTS_STR(params, SWL_RC_INVALID_PARAM, ME, "Empty");
    ASSERTS_NOT_NULL(pChanSpec, SWL_RC_INVALID_PARAM, ME, "NULL");
    uint32_t chWId = 0;
    ASSERTS_TRUE(wld_wpaCtrl_getValueIntExt(params, key, (int32_t*) &chWId), SWL_RC_ERROR,
                 ME, "Missing %s param", key);
    swl_bandwidth_e* pBwEnu = (swl_bandwidth_e*) swl_table_getMatchingValue(&sMxlChWidthMaps, 2, 0, &chWId);
    ASSERTS_NOT_NULL(pBwEnu, SWL_RC_ERROR, ME, "unknown channel width id (%d)", chWId);
    pChanSpec->bandwidth = *pBwEnu;
    return SWL_RC_OK;
}

static swl_rc_ne s_mxl_DfsCacEvts(void* userData, char* ifName _UNUSED, char* event _UNUSED,
                            char* params _UNUSED) {

    T_Radio* pRad = (T_Radio*) userData;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    if (swl_str_matches(event, "DFS-CAC-START")) {
        if (strstr(params, "background")) {
            // Example: DFS-CAC-START freq=5500 chan=100 chan_offset=0 width=3 seg0=5530 seg1=0 cac_time=60s (background)
            swl_chanspec_t chanSpec = SWL_CHANSPEC_EMPTY;
            ASSERT_FALSE(s_freqParamToChanSpec(params, "freq", &chanSpec) < SWL_RC_OK,
                         SWL_RC_ERROR, ME, "fail to get freq");
            ASSERT_FALSE(s_chWidthIdToChanSpec(params, "width", &chanSpec) < SWL_RC_OK,
                         SWL_RC_ERROR, ME, "fail to get channel width");

            uint32_t cac_time = wld_wpaCtrl_getValueInt(params, "cac_time");

            SAH_TRACEZ_WARNING(ME, "%s: background cac started on channel=%d, width=%d,cac time=%d",
                               ifName, chanSpec.channel, chanSpec.bandwidth, cac_time);

            wld_channel_mark_passive_band(chanSpec);
            swl_chanspec_t tgtChanspec = wld_chanmgt_getTgtChspec(pRad);

            if (swl_channel_isInChanspec(&tgtChanspec, chanSpec.channel)) {
                pRad->detailedState = CM_RAD_BG_CAC;
            } else {
                pRad->detailedState = CM_RAD_BG_CAC_NS;
            }
            wld_rad_updateState(pRad, false);
            /* Returning SWL_RC_DONE will skip this event in standard event processing in
             * s_processStdEvent in wld_wpaCtrl_events.c.
             */
            return SWL_RC_DONE;
        }
    }
    /* Returning < SWL_RC_DONE will make pwhm to process foreground DFS-CAC-START
     * in  s_processStdEvent.
     */
    return  SWL_RC_OK;
}

SWL_TABLE(mxl_CustomWpaCtrlEvents,
          ARR(char* evtName; void* evtParser; ),
          ARR(swl_type_charPtr, swl_type_voidPtr),
          ARR(
              {"AP-BW-CHANGED", &s_mxl_apBwChanged},
              {"DFS-CAC-START", &s_mxl_DfsCacEvts},
              ));

static custEvtParser_f s_mxl_getCustomEventParser(char* eventName) {
    custEvtParser_f* pfEvtHdlr = ( custEvtParser_f *) swl_table_getMatchingValue(&mxl_CustomWpaCtrlEvents, 1, 0, eventName);
    ASSERTS_NOT_NULL(pfEvtHdlr, NULL, ME, "no handler defined for evt(%s)", eventName);
    return *pfEvtHdlr;
}

static swl_rc_ne s_mxl_WpaCustomCtrlEvtMsg(void* userData, char* ifName, char* msgData) {
    ASSERTS_STR(msgData, SWL_RC_ERROR, ME, "NULL or no content msgData");
    char* pEvent = strstr(msgData, WPA_MSG_LEVEL_INFO);
    ASSERTS_NOT_NULL(pEvent, SWL_RC_ERROR, ME, "Not a valid WPA ctrl event");
    pEvent += sizeof(WPA_MSG_LEVEL_INFO) - 1;
    uint32_t eventNameLen = strlen(pEvent);
    char* pParams = strchr(pEvent, ' ');
    swl_rc_ne rc = SWL_RC_ERROR;

    if (pParams) {
        eventNameLen = pParams - pEvent;
        pParams++;
    }
    char eventName[eventNameLen + 1];
    swl_str_copy(eventName, sizeof(eventName), pEvent);
    custEvtParser_f fEvtHdlr = s_mxl_getCustomEventParser(eventName);
    ASSERTS_NOT_NULL(fEvtHdlr, SWL_RC_ERROR, ME, "%s: No parser for evt(%s)", ifName, eventName)
    SAH_TRACEZ_INFO(ME, "%s: receive msg '%s'", ifName, msgData);
    rc = fEvtHdlr(userData, ifName, eventName, pParams);
    return rc;
}

static swl_rc_ne s_mxl_setRadioWpaCtrlEvtHandlers(T_Radio* pRad) {
    void* userdata = NULL;
    wld_wpaCtrl_radioEvtHandlers_cb handlers = {0};

    ASSERT_NOT_NULL(pRad->hostapd, SWL_RC_INVALID_PARAM, ME, "hostapd is NULL");
    ASSERT_NOT_NULL(pRad->hostapd->wpaCtrlMngr, SWL_RC_INVALID_PARAM, ME, "wpaCtrlMngr is NULL");

    if (!wld_wpaCtrlMngr_getEvtHandlers(pRad->hostapd->wpaCtrlMngr, &userdata, &handlers)) {
        SAH_TRACEZ_ERROR(ME, "%s: Failed to get event handlers", pRad->Name);
        return SWL_RC_ERROR;
    }

    if (userdata == NULL)
        userdata = pRad;

    handlers.fProcEvtMsg = s_mxl_WpaCtrlEvtMsg;
    handlers.fCustProcEvtMsg = s_mxl_WpaCustomCtrlEvtMsg;
    if (!wld_wpaCtrlMngr_setEvtHandlers(pRad->hostapd->wpaCtrlMngr, userdata, &handlers)) {
        SAH_TRACEZ_ERROR(ME, "%s: Failed to set event handlers", pRad->Name);
        return SWL_RC_ERROR;
    }

    return SWL_RC_OK;
}

swl_rc_ne mxl_evt_setVendorEvtHandlers(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad NULL");

    /*
     * set the nl80211 vendor event handler
     * after nl80211Listener is created (ie when radio wiphyId is known: after successful wrad_support)
     */
    if (pRad->nl80211Listener != NULL) {
        wld_nl80211_addVendorEvtListener(wld_nl80211_getSharedState(), pRad->nl80211Listener, s_vendorEvtCb);
    }

    s_mxl_setRadioWpaCtrlEvtHandlers(pRad);
    return SWL_RC_OK;
}
