/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_hostapd_cfg.c                                 *
*         Description  : Handles Hostapd Confugrations                         *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"
#include "swl/map/swl_mapCharFmt.h"
#include "swl/fileOps/swl_mapWriterKVP.h"
#include "swl/fileOps/swl_fileUtils.h"

#include "wld/wld_radio.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_chanmgt.h"
#include "wld/wld_accesspoint.h"

#include "whm_mxl_module.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_utils.h"

#define ME "mxlHpdC"

/*
 * Mapping of VHT Channel Width IDs from on hostapd.conf @ https://w1.fi/cgit/hostap/plain/hostapd/hostapd.conf
 * # 0 = 20 or 40 MHz operating Channel width
 * # 1 = 80 MHz channel width
 * # 2 = 160 MHz channel width
 * # 3 = 80+80 MHz channel width
 */
SWL_TABLE(sChWidthIDsMaps,
          ARR(uint32_t vhtChWidthIDs; uint32_t ehtChWidthIDs; swl_bandwidth_e swlBw; ),
          ARR(swl_type_uint32, swl_type_uint32, swl_type_uint32, ),
          ARR({0, 0, SWL_BW_20MHZ},
              {0, 0, SWL_BW_40MHZ},
              {1, 1, SWL_BW_80MHZ},
              {2, 2, SWL_BW_160MHZ},
              {2, 9, SWL_BW_320MHZ},
              ));

/*  *****************************************************************************
*                                                                              *
*                           Radio Related Configurations                       *
*                                                                              *
*  *****************************************************************************/
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
static swl_rc_ne whm_mxl_rad_acsUpdateConfigMap(T_Radio* pRad, mxl_VendorData_t* pRadVendor, swl_mapChar_t* configMap) {
    amxd_object_t* acsObj = amxd_object_get(pRadVendor->pBus, "ACS");
    ASSERT_NOT_NULL(acsObj, SWL_RC_ERROR, ME, "No ACS vendor obj");
    char *acsFallbackChan = amxd_object_get_value(cstring_t, acsObj, "AcsFallbackChan", NULL);
    char *acs6gOptChList = amxd_object_get_value(cstring_t, acsObj, "Acs6gOptChList", NULL);

    swl_mapCharFmt_addValStr(configMap, "acs_smart_info_file", "%s%s%s", "/tmp/acs_smart_info_", pRad->Name, ".txt");
    swl_mapCharFmt_addValStr(configMap, "acs_history_file", "%s%s%s", "/tmp/acs_history_", pRad->Name, ".txt");
    swl_mapCharFmt_addValInt32(configMap, "acs_num_scans", 1);
    swl_mapCharFmt_addValInt32(configMap, "acs_scan_mode", amxd_object_get_value(bool, acsObj, "AcsScanMode", NULL));
    swl_mapCharFmt_addValInt32(configMap, "acs_update_do_switch", amxd_object_get_value(bool, acsObj, "AcsUpdateDoSwitch", NULL));
    if (!swl_str_isEmpty(acsFallbackChan)) {
        swl_mapCharFmt_addValStr(configMap, "acs_fallback_chan", "%s", acsFallbackChan);
    }
    free(acsFallbackChan);
    if (!swl_str_isEmpty(acs6gOptChList)) {
        swl_mapCharFmt_addValStr(configMap, "acs_6g_opt_ch_list", "%s", acs6gOptChList);
    }
    free(acs6gOptChList);

    if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AX) && wld_rad_is_6ghz(pRad)) {
        swl_mapCharFmt_addValInt32(configMap, "acs_fils", amxd_object_get_value(bool, acsObj, "AcsFils", NULL));
    }

    if (wld_rad_is_6ghz(pRad)) {
        swl_mapCharFmt_addValInt32(configMap, "acs_6g_punct_mode", amxd_object_get_value(bool, acsObj, "Acs6gPunctMode", NULL));
    }

    if (!swl_str_isEmpty(pRadVendor->acs_exclusion_ch_list)) {
        swl_mapCharFmt_addValStr(configMap, "acs_exclude_opclass_ch_list", "%u %s",
                                                pRadVendor->acs_exclusion_list_count,
                                                pRadVendor->acs_exclusion_ch_list);
    } else {
        // In hostapd config, acs_exclude_opclass_ch_list is set as 0 0 then all channels are considered for ACS.
        swl_mapCharFmt_addValStr(configMap, "acs_exclude_opclass_ch_list", "0 0");
    }

    return SWL_RC_OK;
}
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

static swl_rc_ne whm_mxl_rad_afcUpdateConfigMap(amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pVendorObj, SWL_RC_ERROR, ME, "pVendorObj is NULL");
    amxd_object_t* afcObj = amxd_object_get(pVendorObj, "AFC");
    ASSERT_NOT_NULL(afcObj, SWL_RC_ERROR, ME, "afcObj is NULL");
    char *tmpStr = NULL;

    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcdSock", configMap, "afcd_sock");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcOpClass", configMap, "afc_op_class");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcFrequencyRange", configMap, "afc_freq_range");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcCertIds", configMap, "afc_cert_ids");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcSerialNumber", configMap, "afc_serial_number");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcLinearPolygon", configMap, "afc_linear_polygon");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, afcObj, "AfcLocationType", NULL), -1, configMap, "afc_location_type");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcRequestId", configMap, "afc_request_id");
    WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, afcObj, "AfcRequestVersion", configMap, "afc_request_version");

    return SWL_RC_OK;
}

static swl_rc_ne whm_mxl_rad_bssColorUpdateConfigMap(mxl_VendorData_t* pRadVendor, swl_mapChar_t* configMap) {
    amxd_object_t* BssColorObj = amxd_object_get(pRadVendor->pBus, "BssColor");
    ASSERT_NOT_NULL(BssColorObj, SWL_RC_ERROR, ME, "No BssColor vendor obj");

    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(bool, BssColorObj, "AutonomousColorChange", NULL), 1, configMap, "autonomous_color_change");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, BssColorObj, "ChangeTimeout", NULL), BSS_COLOR_CHANGE_TIMEOUT_DEFAULT, configMap, "bss_color_change_timeout");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, BssColorObj, "NumCollisionsThreshold", NULL), NUM_BSS_COLOR_COLL_THRESH_DEFAULT, configMap, "num_bss_color_coll_thresh");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, BssColorObj, "CollAgeThresh", NULL), BSS_COLOR_COLL_AGE_THRESH_DEFAULT, configMap, "bss_color_coll_age_thresh");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, BssColorObj, "UsedColorTableAgeing", NULL), USED_COLOR_TABLE_AGEING_DEFAULT, configMap, "used_color_table_ageing");
    if(pRadVendor->randomColor) {
        WHM_MXL_NE_SET_PARAM(pRadVendor->randomColor, 0, configMap, "he_bss_color");
        pRadVendor->randomColor = 0;
    } else {
        WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, BssColorObj, "HeBssColor", NULL), 0, configMap, "he_bss_color");
    }
    return SWL_RC_OK;
}

static swl_rc_ne whm_mxl_rad_delayedStartUpdateConfigMap(mxl_VendorData_t* pRadVendor, swl_mapChar_t* configMap) {
    amxd_object_t* DelayedStartObj = amxd_object_get(pRadVendor->pBus, "DelayedStart");
    ASSERT_NOT_NULL(DelayedStartObj, SWL_RC_ERROR, ME, "No DelayedStart vendor obj");

    char *startAfter = amxd_object_get_value(cstring_t, DelayedStartObj, "StartAfter", NULL);
    uint32_t startDelay = amxd_object_get_value(uint32_t, DelayedStartObj, "StartAfterDelay", NULL);
    uint32_t startWdTime = amxd_object_get_value(uint32_t, DelayedStartObj, "StartAfterWatchdogTime", NULL);

    if (!swl_str_isEmpty(startAfter)) {
        swl_mapCharFmt_addValStr(configMap, "start_after", "%s",startAfter);
    }
    free(startAfter);
    WHM_MXL_GT_SET_PARAM(startDelay, 0, configMap, "start_after_delay");
    WHM_MXL_GT_SET_PARAM(startWdTime, 0, configMap, "start_after_watchdog_time");

    return SWL_RC_OK;
}

static wld_channel_extensionPos_e whm_mxl_rad_getExtensionChannel(T_Radio* pRad) {
    if (!wld_rad_is_24ghz(pRad)) {
        if((pRad->channel / 4) % 2) {
            return WLD_CHANNEL_EXTENTION_POS_ABOVE;
        }
        return WLD_CHANNEL_EXTENTION_POS_BELOW;
    }
    if(pRad->extensionChannel == WLD_CHANNEL_EXTENTION_POS_AUTO) {
        if(pRad->channel < 7) {
            return WLD_CHANNEL_EXTENTION_POS_ABOVE;
        }
        return WLD_CHANNEL_EXTENTION_POS_BELOW;
    }
    return pRad->extensionChannel;
}

static void whm_mxl_rad_configHtCapabs(T_Radio* pRad, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad NULL");
    swl_80211_htCapInfo_m htCapabilities = pRad->htCapabilities;
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor NULL");
    amxd_object_t* htCapabsObj = amxd_object_get(pRadVendor->pBus, "HtCapabilities");
    ASSERT_NOT_NULL(htCapabsObj, , ME, "No HtCapabilities vendor obj");
    char htCapsBuffer[256] = {0};

    SAH_TRACEZ_INFO(ME, "%s: htCapabilities:%X", pRad->Name, htCapabilities);
 
    swl_mapCharFmt_addValStr(configMap, "ht_minimum_mpdu_start_spacing", "%u", amxd_object_get_value(uint8_t, htCapabsObj, "HtMinMpduStartSpacing", NULL));

    if(htCapabilities == 0) {
        SAH_TRACEZ_INFO(ME, "%s: Cannot add ht_capab", pRad->Name); 
        return;
    }

    if(swl_chanspec_bwToInt(pRad->targetChanspec.chanspec.bandwidth) >= swl_chanspec_bwToInt(SWL_BW_40MHZ)) {
        if (pRad->autoChannelEnable || pRadVendor->firstNonDfs) {
            swl_str_cat(htCapsBuffer, sizeof(htCapsBuffer), "[HT40+][HT40-]");
        }
        else {
            wld_channel_extensionPos_e extChanPos = whm_mxl_rad_getExtensionChannel(pRad);
            if(extChanPos == WLD_CHANNEL_EXTENTION_POS_ABOVE) {
                swl_str_cat(htCapsBuffer, sizeof(htCapsBuffer), "[HT40+]");
            } else if(extChanPos == WLD_CHANNEL_EXTENTION_POS_BELOW) {
                swl_str_cat(htCapsBuffer, sizeof(htCapsBuffer), "[HT40-]");
            }
        }
    }
    /* Check if HT capabilities are present from phy info and enabled in data model */
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_LDPC, htCapsBuffer, htCapabsObj, "Ldpc", "[LDPC]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_GREENFEELD, htCapsBuffer, htCapabsObj, "Greenfield", "[GF]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_SGI_20, htCapsBuffer, htCapabsObj, "ShortGi20", "[SHORT-GI-20]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_SGI_40 , htCapsBuffer, htCapabsObj, "ShortGi40", "[SHORT-GI-40]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_TX_STBC, htCapsBuffer, htCapabsObj, "TxStbc", "[TX-STBC]");
    WHM_MXL_SET_CAPABS_STBC(htCapabilities, M_SWL_80211_HTCAPINFO_RX_STBC, HTCAP_RX_STBC1, htCapsBuffer, "[RX-STBC1]");
    WHM_MXL_SET_CAPABS_STBC(htCapabilities, M_SWL_80211_HTCAPINFO_RX_STBC, HTCAP_RX_STBC12, htCapsBuffer, "[RX-STBC12]");
    WHM_MXL_SET_CAPABS_STBC(htCapabilities, M_SWL_80211_HTCAPINFO_RX_STBC, HTCAP_RX_STBC123, htCapsBuffer, "[RX-STBC123]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_MAX_AMSDU, htCapsBuffer, htCapabsObj, "MaxAmsdu", "[MAX-AMSDU-7935]");
    WHM_MXL_SET_CAPABS(htCapabilities, M_SWL_80211_HTCAPINFO_MAX_AMSDU, htCapsBuffer, htCapabsObj, "DsssCck40", "[DSSS_CCK-40]");

    /* Apply HT Capabilities */
    if(!swl_str_isEmpty(htCapsBuffer)) {
        swl_mapCharFmt_addValStr(configMap, "ht_capab", "%s", htCapsBuffer);
        SAH_TRACEZ_INFO(ME, "%s: Writing ht_capab=%s", pRad->Name, htCapsBuffer);
    }
}

static void whm_mxl_rad_configVhtCapabs(T_Radio* pRad, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad NULL");
    swl_80211_vhtCapInfo_m vhtCapabilities = pRad->vhtCapabilities;
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor NULL");
    amxd_object_t* vhtCapabsObj = amxd_object_get(pRadVendor->pBus, "VhtCapabilities");
    ASSERT_NOT_NULL(vhtCapabsObj, , ME, "No VhtCapabilities vendor obj");
    amxd_object_t* htCapabsObj = amxd_object_get(pRadVendor->pBus, "HtCapabilities");
    ASSERT_NOT_NULL(vhtCapabsObj, , ME, "No HtCapabilities vendor obj");
    wld_nl80211_wiphyInfo_t wiphyInfo;
    swl_rc_ne rc;
    char vhtCapsBuffer[512] = {0};
    char tempBuf[256] = {0};
    bool txStbc                 = amxd_object_get_value(bool, htCapabsObj, "TxStbc", NULL);
    bool txStbcTwoByOne         = amxd_object_get_value(bool, vhtCapabsObj, "TxStbc2by1", NULL);
    bool shortGi80              = amxd_object_get_value(bool, vhtCapabsObj, "ShortGi80", NULL);
    bool shortGi160             = amxd_object_get_value(bool, vhtCapabsObj, "ShortGi160", NULL);
    uint8_t vht160              = amxd_object_get_value(uint8_t, vhtCapabsObj, "Vht160", NULL);
    uint32_t vhtMaxMpdu         = amxd_object_get_value(uint32_t, vhtCapabsObj, "VhtMaxMpdu", NULL);
    uint8_t vhtMaxAmpduLenExp   = amxd_object_get_value(uint8_t, vhtCapabsObj, "VhtMaxAmpduLenExp", NULL);
    uint8_t vhtLinkAdapt        = amxd_object_get_value(uint8_t, vhtCapabsObj, "VhtLinkAdapt", NULL);
    const char* vhtOperChWidth  = swl_mapChar_get(configMap, "vht_oper_chwidth");
    bool implicitBf = (pRad->implicitBeamFormingSupported && pRad->implicitBeamFormingEnabled);
    bool explicitBf = (pRad->explicitBeamFormingSupported && pRad->explicitBeamFormingEnabled);
    bool muMimo = (pRad->multiUserMIMOSupported && pRad->multiUserMIMOEnabled);

    SAH_TRACEZ_INFO(ME, "%s: vhtCapabilities:%X", pRad->Name, vhtCapabilities);

    if(vhtCapabilities == 0) {
        SAH_TRACEZ_INFO(ME, "%s: Cannot add vht_capab", pRad->Name);
        return;
    }

    /* Get the vht_oper_chwidth */
    if(swl_str_matches(vhtOperChWidth, "0") || swl_str_matches(vhtOperChWidth, "1")) {
        vht160 = 0;
    }

    /* Check if VHT capabilities are present from phy info and enabled in data model */
    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_LDPC, vhtCapsBuffer, vhtCapabsObj, "Rxldpc", "[RXLDPC]");
    if((vhtCapabilities & M_SWL_80211_VHTCAPINFO_SGI_80) && (shortGi80)) {
        swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[SHORT-GI-80]");
    }
    if((vhtCapabilities & M_SWL_80211_VHTCAPINFO_SGI_160) && (shortGi160)) {
        swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[SHORT-GI-160]");
    }
    if((vhtCapabilities & M_SWL_80211_VHTCAPINFO_TX_STBC) && (txStbc & txStbcTwoByOne)) {
        swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[TX-STBC-2BY1]");
    }

    if(implicitBf) {
        if(SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_RECEIVE], RAD_BF_CAP_VHT_SU) &&
            (SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_RECEIVE], RAD_BF_CAP_DEFAULT) ||
            SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_RECEIVE], RAD_BF_CAP_VHT_SU))) {
            swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[SU-BEAMFORMEE]");
        }
    }
    if(explicitBf) {
        if(SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_VHT_SU) &&
            (SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_DEFAULT) ||
            SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_VHT_SU))) {
            swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[SU-BEAMFORMER]");
        }
        if(SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_VHT_MU) && muMimo &&
            (SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_DEFAULT) ||
            SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_VHT_MU))) {
            swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[MU-BEAMFORMER]");
        }
    }

    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_MU_BFE, vhtCapsBuffer, vhtCapabsObj, "MuBeamformee", "[MU-BEAMFORMEE]");
    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_TXOP_PS, vhtCapsBuffer, vhtCapabsObj, "VhtTxopPs", "[VHT-TXOP-PS]");
    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_HTC_CAP, vhtCapsBuffer, vhtCapabsObj, "HtcVht", "[HTC-VHT]");
    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_ANT_PAT_CONS, vhtCapsBuffer, vhtCapabsObj, "RxAntennaPattern", "[RX-ANTENNA-PATTERN]");
    WHM_MXL_SET_CAPABS(vhtCapabilities, M_SWL_80211_VHTCAPINFO_TX_ANT_PAT_CONS, vhtCapsBuffer, vhtCapabsObj, "TxAntennaPattern", "[TX-ANTENNA-PATTERN]");
    WHM_MXL_SET_CAPABS_STBC(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_STBC, VHTCAP_RX_STBC1, vhtCapsBuffer, "[RX-STBC-1]");
    WHM_MXL_SET_CAPABS_STBC(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_STBC, VHTCAP_RX_STBC12, vhtCapsBuffer, "[RX-STBC-12]");
    WHM_MXL_SET_CAPABS_STBC(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_STBC, VHTCAP_RX_STBC123, vhtCapsBuffer, "[RX-STBC-123]");
    WHM_MXL_SET_CAPABS_STBC(vhtCapabilities, M_SWL_80211_VHTCAPINFO_RX_STBC, VHTCAP_RX_STBC1234, vhtCapsBuffer, "[RX-STBC-1234]");

    switch(vhtCapabilities & M_SWL_80211_VHTCAPINFO_SUP_WIDTH) {
        case 4 :
            if(1 <= vht160) {
                swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[VHT160]");
            }
            break;
        case 8 :
            if(2 <= vht160) {
                swl_str_cat(vhtCapsBuffer, sizeof(vhtCapsBuffer), "[VHT160-80PLUS80]");
            }
            break;
    }

    switch(vhtCapabilities & M_SWL_80211_VHTCAPINFO_MAX_MPDU) {
        case 1:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(7991U, vhtMaxMpdu));
            break;
        case 2:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(11454U, vhtMaxMpdu));
            break;
        default:
            SAH_TRACEZ_INFO(ME, "VHTCAPINFO_MAX_MPDU - None of the above cases");
            break;
    }
    if(!swl_str_isEmpty(tempBuf)) {
        MXL_RAD_CAT_SET_CAPAB(vhtCapsBuffer, tempBuf, "MAX-MPDU-");
    }

    memset(tempBuf, 0, sizeof(tempBuf));
    switch(vhtCapabilities & M_SWL_80211_VHTCAPINFO_MAX_AMPDU_EXP) {
        case VHTCAP_MAX_AMPDU_EXP_M1:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(1,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M2:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(2,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M3:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(3,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M4:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(4,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M5:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(5,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M6:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(6,vhtMaxAmpduLenExp));
            break;
        case VHTCAP_MAX_AMPDU_EXP_M7:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(7,vhtMaxAmpduLenExp));
            break;
        default:
            SAH_TRACEZ_INFO(ME, "VHTCAPINFO_MAX_AMPDU_EXP - None of the above cases");
            break;
    }
    if(!swl_str_isEmpty(tempBuf)) {
        MXL_RAD_CAT_SET_CAPAB(vhtCapsBuffer, tempBuf, "MAX-A-MPDU-LEN-EXP");
    }

    memset(tempBuf, 0, sizeof(tempBuf));
    switch(vhtCapabilities & VHTCAPINFO_LINK_ADAPT_CAP) {
        case VHTCAP_LINK_ADAPT_CAP_M1:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(2,vhtLinkAdapt));
            break;
        case VHTCAPINFO_LINK_ADAPT_CAP:
            swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%u", SWL_MIN(3,vhtLinkAdapt));
            break;
        default:
            SAH_TRACEZ_INFO(ME, "None of the above cases");
            break;
    }
    if(!swl_str_isEmpty(tempBuf)) {
        MXL_RAD_CAT_SET_CAPAB(vhtCapsBuffer, tempBuf, "VHT-LINK-ADAPT-");
    }

    rc = wld_rad_nl80211_getWiphyInfo(pRad, &wiphyInfo);
    ASSERT_FALSE(rc < SWL_RC_OK, , ME, "Fail to get nl80211 wiphy info");
    swl_strlst_catFormat(tempBuf, sizeof(tempBuf), "", "%d", wiphyInfo.nrActiveAntenna[COM_DIR_TRANSMIT]);
    MXL_RAD_CAT_SET_CAPAB(vhtCapsBuffer, tempBuf, "BF-ANTENNA-");
    MXL_RAD_CAT_SET_CAPAB(vhtCapsBuffer, tempBuf, "SOUNDING-DIMENSION-");

    /* Apply VHT Capabilities */
    if(!swl_str_isEmpty(vhtCapsBuffer)) {
        swl_mapCharFmt_addValStr(configMap, "vht_capab", "%s", vhtCapsBuffer);
        SAH_TRACEZ_INFO(ME, "%s: Writing vht_capab=%s", pRad->Name, vhtCapsBuffer);
    }
}

static void whm_mxl_rad_configHe6gCapabs(amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pVendorObj, , ME, "pVendorObj is NULL");
    amxd_object_t* he6gCapabsObj = amxd_object_get(pVendorObj, "He6GhzCapabilities");
    ASSERT_NOT_NULL(he6gCapabsObj, , ME, "he6GCapabsObj is NULL");

    SAH_TRACEZ_INFO(ME, "Overwriting 6GHz HE Caps parameters");
    swl_mapCharFmt_addValInt32(configMap, "he_6ghz_max_mpdu", amxd_object_get_value(uint8_t, he6gCapabsObj, "he_6ghz_max_mpdu", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_6ghz_max_ampdu_len_exp", amxd_object_get_value(uint8_t, he6gCapabsObj, "he_6ghz_max_ampdu_len_exp", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_6ghz_rx_ant_pat", amxd_object_get_value(uint8_t, he6gCapabsObj, "he_6ghz_rx_ant_pat", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_6ghz_tx_ant_pat", amxd_object_get_value(uint8_t, he6gCapabsObj, "he_6ghz_tx_ant_pat", NULL));
}

static void s_rad_configEhtCapabs(T_Radio* pRad, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad NULL");

    swl_chanspec_t tgtChspec = wld_chanmgt_getTgtChspec(pRad);
    swl_bandwidth_e tgtChW = tgtChspec.bandwidth;
    uint32_t* pChWId = (uint32_t*) swl_table_getMatchingValue(&sChWidthIDsMaps, 0, 2, &tgtChW);
    uint32_t* pEhtChWId = (uint32_t*) swl_table_getMatchingValue(&sChWidthIDsMaps, 1, 2, &tgtChW);
    bool implicitBf = (pRad->implicitBeamFormingSupported && pRad->implicitBeamFormingEnabled);
    bool explicitBf = (pRad->explicitBeamFormingSupported && pRad->explicitBeamFormingEnabled);
    bool muMimo = (pRad->multiUserMIMOSupported && pRad->multiUserMIMOEnabled);

    if(pEhtChWId) {
        if(pChWId && (*pChWId != *pEhtChWId)) {
            tgtChspec.bandwidth = *(uint32_t*) swl_table_getMatchingValue(&sChWidthIDsMaps, 2, 1, pEhtChWId);
        }
        swl_channel_t centerChan = swl_chanspec_getCentreChannel(&tgtChspec);
        swl_mapCharFmt_addValInt32(configMap, "eht_oper_chwidth", *pEhtChWId);
        swl_mapCharFmt_addValInt32(configMap, "eht_oper_centr_freq_seg0_idx", centerChan);
    }
    if(implicitBf) {
        if(SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_RECEIVE], RAD_BF_CAP_EHT_SU) &&
           (SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_RECEIVE], RAD_BF_CAP_DEFAULT) ||
            SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_RECEIVE], RAD_BF_CAP_EHT_SU))) {
            swl_mapCharFmt_addValInt32(configMap, "eht_su_beamformee", 1);
        }
    }
    if(explicitBf) {
        if(SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_SU) &&
           (SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_DEFAULT) ||
            SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_SU))) {
            swl_mapCharFmt_addValInt32(configMap, "eht_su_beamformer", 1);
        }
        bool beamformerSupported = (tgtChW <= SWL_BW_80MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_80MHZ) :
            (tgtChW == SWL_BW_160MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_160MHZ) :
            (tgtChW == SWL_BW_320MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsSupported[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_320MHZ) : false;
        bool beamformerEnabled = (tgtChW <= SWL_BW_80MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_80MHZ) :
            (tgtChW == SWL_BW_160MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_160MHZ) :
            (tgtChW == SWL_BW_320MHZ) ? SWL_BIT_IS_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_EHT_MU_320MHZ) :
            SWL_BIT_IS_ONLY_SET(pRad->bfCapsEnabled[COM_DIR_TRANSMIT], RAD_BF_CAP_DEFAULT);
        if(muMimo && beamformerSupported && beamformerEnabled) {
            swl_mapCharFmt_addValInt32(configMap, "eht_mu_beamformer", 1);
        }
    }
}

static void whm_mxl_rad_configAxMxlParams(T_Radio* pRad, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: Configuring AX MxL he_mu_edca params", pRad->Name);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_qos_info_queue_request", "%u", 1);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_be_ecwmin", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_be_ecwmax", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_be_timer", "%u", 5);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_bk_aci", "%u", 1);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_bk_ecwmin", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_bk_ecwmax", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_bk_timer", "%u", 5);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vi_ecwmin", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vi_ecwmax", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vi_aci", "%u", 2);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vi_timer", "%u", 5);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vo_aci", "%u", 3);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vo_ecwmin", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vo_ecwmax", "%u", 15);
    swl_mapCharFmt_addValStr(configMap, "he_mu_edca_ac_vo_timer", "%u", 5);
}

static void  whm_mxl_rad_configCertification(T_Radio* pRad, amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pVendorObj, , ME, "pVendorObj is NULL");
    bool testbed_mode =  amxd_object_get_value(bool, pVendorObj, "TestBedMode", NULL);
    char *heMcsNssRxMapLessOrEqual80Mhz = amxd_object_get_value(cstring_t, pVendorObj, "HeMcsNssRxMapLessOrEqual80Mhz", NULL);
    char *heMcsNssTxMapLessOrEqual80Mhz = amxd_object_get_value(cstring_t, pVendorObj, "HeMcsNssTxMapLessOrEqual80Mhz", NULL);
    char *heMcsNssRxHeMcsMap160Mhz      = amxd_object_get_value(cstring_t, pVendorObj, "HeMcsNssRxHeMcsMap160Mhz", NULL);
    char *heMcsNssTxHeMcsMap160Mhz      = amxd_object_get_value(cstring_t, pVendorObj, "HeMcsNssTxHeMcsMap160Mhz", NULL);
    char *vhtMcsSetPart0                = amxd_object_get_value(cstring_t, pVendorObj, "VhtMcsSetPart0", NULL);
    char *vhtMcsSetPart1                = amxd_object_get_value(cstring_t, pVendorObj, "VhtMcsSetPart1", NULL);
    char *country3                      = amxd_object_get_value(cstring_t, pVendorObj, "Country3", NULL);

    swl_mapCharFmt_addValInt32(configMap, "he_phy_ldpc_coding_in_payload", amxd_object_get_value(bool, pVendorObj, "HePhyLdpcCodingInPayload", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mac_a_msdu_in_ack_enabled_a_mpdu_support", amxd_object_get_value(bool, pVendorObj, "HeMacMsduAckEnabledMpduSupport", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mac_maximum_a_mpdu_length_exponent", amxd_object_get_value(uint8_t, pVendorObj, "HeMacMaxAMpduLengthExponent", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mac_om_control_support", amxd_object_get_value(bool, pVendorObj, "HeMacOmControlSupport", NULL));
    swl_mapCharFmt_addValInt32(configMap, "ht_minimum_mpdu_start_spacing", amxd_object_get_value(uint8_t, pVendorObj, "HtMinMpduStartSpacing", NULL));
    swl_mapCharFmt_addValInt32(configMap, "multibss_enable", amxd_object_get_value(bool, pVendorObj, "MultibssEnable", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_phy_max_nc", amxd_object_get_value(uint32_t, pVendorObj, "HePhyMaxNc", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sr_control_field_hesiga_spatial_reuse_value15_allowed", amxd_object_get_value(bool, pVendorObj, "SrCtrlHesigaSpatialReuseVal15", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_operation_cohosted_bss", amxd_object_get_value(bool, pVendorObj, "HeOperationCohostedBss", NULL));

    swl_mapCharFmt_addValStr(configMap, "he_mcs_nss_rx_he_mcs_map_less_than_or_equal_80_mhz", "%s", heMcsNssRxMapLessOrEqual80Mhz);
    free(heMcsNssRxMapLessOrEqual80Mhz);
    swl_mapCharFmt_addValStr(configMap, "he_mcs_nss_tx_he_mcs_map_less_than_or_equal_80_mhz", "%s", heMcsNssTxMapLessOrEqual80Mhz);
    free(heMcsNssTxMapLessOrEqual80Mhz);
    swl_mapCharFmt_addValStr(configMap, "he_mcs_nss_rx_he_mcs_map_160_mhz", "%s", heMcsNssRxHeMcsMap160Mhz);
    free(heMcsNssRxHeMcsMap160Mhz);
    swl_mapCharFmt_addValStr(configMap, "he_mcs_nss_tx_he_mcs_map_160_mhz", "%s", heMcsNssTxHeMcsMap160Mhz);
    free(heMcsNssTxHeMcsMap160Mhz);
    swl_mapCharFmt_addValStr(configMap, "vht_mcs_set_part0", "%s", vhtMcsSetPart0);
    free(vhtMcsSetPart0);
    swl_mapCharFmt_addValStr(configMap, "vht_mcs_set_part1", "%s", vhtMcsSetPart1);
    free(vhtMcsSetPart1);
    if(!swl_str_isEmpty(country3)) {
        swl_mapCharFmt_addValStr(configMap, "country3", "%s", country3);
    }
    free(country3);

    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ie_present", amxd_object_get_value(bool, pVendorObj, "HeMuEdcaIePresent", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_phy_dcm_max_constellation_tx", amxd_object_get_value(bool, pVendorObj, "HePhyDcmMaxConstellationTx", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_phy_dcm_max_constellation_rx", amxd_object_get_value(bool, pVendorObj, "HePhyDcmMaxConstellationRx", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_phy_dcm_max_nss_tx", amxd_object_get_value(bool, pVendorObj, "HePhyDcmMaxNssTx", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_phy_dcm_max_nss_rx", amxd_object_get_value(bool, pVendorObj, "HePhyDcmMaxNssRx", NULL));
    swl_mapCharFmt_addValInt32(configMap, "twt_responder_support", amxd_object_get_value(int32_t, pVendorObj, "TwtResponderSupport", NULL));
    swl_mapCharFmt_addValInt32(configMap, "ieee80211n_acax_compat", amxd_object_get_value(bool, pVendorObj, "Ieee80211nAcAxCompat", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mac_twt_responder_support", amxd_object_get_value(int32_t, pVendorObj, "HeMacTwtResponderSupport", NULL));
    swl_mapCharFmt_addValInt32(configMap, "enable_he_debug_mode", amxd_object_get_value(bool, pVendorObj, "EnableHeDebugMode", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_be_aifsn", amxd_object_get_value(bool, pVendorObj, "HeMuEdcaAcBeAifsn", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_be_ecwmin", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcBeEcwmin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_be_ecwmax", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcBeEcwmax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_be_timer", amxd_object_get_value(uint32_t, pVendorObj, "HeMuEdcaAcBeTimer", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_bk_aifsn", amxd_object_get_value(bool, pVendorObj, "HeMuEdcaAcBkAifsn", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_bk_aci", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcBkAci", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_bk_ecwmin", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcBkEcwmin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_bk_ecwmax", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcBkEcwmax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_bk_timer", amxd_object_get_value(uint32_t, pVendorObj, "HeMuEdcaAcBkTimer", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vi_aifsn", amxd_object_get_value(bool, pVendorObj, "HeMuEdcaAcViAifsn", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vi_aci", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcViAci", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vi_ecwmin", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcViEcwmin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vi_ecwmax", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcViEcwmax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vi_timer", amxd_object_get_value(uint32_t, pVendorObj, "HeMuEdcaAcViTimer", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vo_aifsn", amxd_object_get_value(bool, pVendorObj, "HeMuEdcaAcVoAifsn", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vo_aci", amxd_object_get_value(uint8_t, pVendorObj, "HeMuEdcaAcVoAci", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vo_ecwmin", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcVoEcwmin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vo_ecwmax", amxd_object_get_value(int8_t, pVendorObj, "HeMuEdcaAcVoEcwmax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "he_mu_edca_ac_vo_timer", amxd_object_get_value(uint32_t, pVendorObj, "HeMuEdcaAcVoTimer", NULL));
    swl_mapCharFmt_addValInt32(configMap, "enable_eht_debug_mode", amxd_object_get_value(bool, pVendorObj, "EnableEhtDebugMode", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_eht_om_control", amxd_object_get_value(bool, pVendorObj, "EhtMacEhtOmControl", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_restricted_twt", amxd_object_get_value(bool, pVendorObj, "EhtMacRestrictedTwt", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_trig_txop_sharing_mode1", amxd_object_get_value(bool, pVendorObj, "EhtMacTrigTxopSharingMode1", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_trig_txop_sharing_mode2", amxd_object_get_value(bool, pVendorObj, "EhtMacTrigTxopSharingMode2", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_trig_mu_bf_partial_bw_fb", amxd_object_get_value(bool, pVendorObj, "EhtPhyTrigMuBfPartialBwFb", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_max_nc", amxd_object_get_value(bool, pVendorObj, "EhtPhyMaxNc", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzRx09", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzTx09", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzRx1011", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzTx1011", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzRx1213", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMapLessOrEq80MHzTx1213", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_max_ampdu_len_exp_ext", amxd_object_get_value(uint8_t, pVendorObj, "EhtPhyMaxAmpduLenExpExt", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_su_beamformer", amxd_object_get_value(bool, pVendorObj, "EhtPhySuBeamformer", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_su_beamformee", amxd_object_get_value(bool, pVendorObj, "EhtPhySuBeamformee", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_max_mpdu_len", amxd_object_get_value(uint32_t, pVendorObj, "EhtMacMaxMpduLen", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_ppe_thresholds_present", amxd_object_get_value(bool, pVendorObj, "EhtPhyPpeThresholdsPresent", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sDynamicMuTypeDownLink", amxd_object_get_value(bool, pVendorObj, "SetDynamicMuTypeDownLink", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sDynamicMuTypeUpLink", amxd_object_get_value(bool, pVendorObj, "SetDynamicMuTypeUpLink", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_mac_scs_traffic_desc", amxd_object_get_value(bool, pVendorObj, "EhtMacScsTrafficDesc", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eht_phy_common_nominal_pkt_pad", amxd_object_get_value(uint8_t, pVendorObj, "EhtPhyCommonNominalPktPad", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sMaxMpduLen", amxd_object_get_value(int32_t, pVendorObj, "SetMaxMpduLen", NULL));
    swl_mapCharFmt_addValInt32(configMap, "advertise_ecsa_ie", amxd_object_get_value(bool, pVendorObj, "AdvertiseEcsaIe", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sDynamicMuMinStationsInGroup", amxd_object_get_value(int8_t, pVendorObj, "SetDynamicMuMinStationsInGroup", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sDynamicMuMaxStationsInGroup", amxd_object_get_value(int8_t, pVendorObj, "SetDynamicMuMaxStationsInGroup", NULL));
    swl_mapCharFmt_addValInt32(configMap, "sDynamicMuCdbConfig", amxd_object_get_value(int8_t, pVendorObj, "SetDynamicMuCdbConfig", NULL));
    if (wld_rad_is_24ghz(pRad)) {
	    swl_mapCharFmt_addValInt32(configMap, "eht_mld_tsf_diff", amxd_object_get_value(int32_t, pVendorObj, "EhtMldTsfDiff", NULL));
    } else {
	    swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzRxMcs09", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzTxMcs09", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzTxMcs1011", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzRxMcs1011", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzTxMcs1213", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap160MHzRxMcs1213", NULL));
    }
    if (wld_rad_is_6ghz(pRad)) {
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzRxMcs09", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_0_9", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzTxMcs09", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzTxMcs1011", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_10_11", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzRxMcs1011", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzRxMcs1213", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_12_13", amxd_object_get_value(uint8_t, pVendorObj, "EhtMcsMap320MHzTxMcs1213", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_320_mhz_in_6_ghz", amxd_object_get_value(uint8_t, pVendorObj, "EhtPhy320MHzIn6GHz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "punct_bitmap", amxd_object_get_value(uint16_t, pVendorObj, "PunctureBitMap", NULL));
        swl_mapCharFmt_addValInt32(configMap, "rnr_6g_op_class_137_allowed", amxd_object_get_value(bool, pVendorObj, "Rnr6gOpClass137Allowed", NULL));
    }
    if (testbed_mode) {
        swl_mapCharFmt_addValInt32(configMap, "he_operation_txop_duration_rts_threshold", amxd_object_get_value(uint32_t, pVendorObj, "HeOpTxopDurationRtsThreshold", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_su_beamformee_capable", amxd_object_get_value(bool, pVendorObj, "HePhySuBeamformeeCapable", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_su_beamformer_capable", amxd_object_get_value(bool, pVendorObj, "HePhySuBeamformerCapable", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_beamformee_sts_for_less_than_or_equal_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyBeamformeeStsLesOrEq80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_beamformee_sts_for_greater_than_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyBeamformeeStsGreater80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_device_class", amxd_object_get_value(bool, pVendorObj, "HePhyDeviceClass", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_su_ppdu_with_1x_he_ltf_and_08_us_gi", amxd_object_get_value(bool, pVendorObj, "HePhySuPpdu1xHeLtfAnd08UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_su_ppdu_and_he_mu_with_4x_he_ltf_and_08us_gi", amxd_object_get_value(bool, pVendorObj, "HePhySuPpduHeMu4xHeLtf08UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_mu_beamformer_capable", amxd_object_get_value(bool, pVendorObj, "HePhyMuBeamformerCapable", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_ndp_with_4x_he_ltf_and_32_us_gi", amxd_object_get_value(bool, pVendorObj, "HePhyNdpWith4xHeLtfAnd32UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_ng_16_su_feedback", amxd_object_get_value(bool, pVendorObj, "HePhyNg16SuFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_ng_16_mu_feedback", amxd_object_get_value(bool, pVendorObj, "HePhyNg16MuFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_number_of_sounding_dimensions_for_less_than_or_equal_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyNumSoundDimenLeOrEq80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_number_of_sounding_dimensions_for_greater_than_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyNumSoundDimenGreater80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_triggered_su_beamforming_feedback", amxd_object_get_value(bool, pVendorObj, "HePhyTriggerSuBeamformFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_doppler_rx", amxd_object_get_value(bool, pVendorObj, "HePhyDopplerRx", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_doppler_tx", amxd_object_get_value(bool, pVendorObj, "HePhyDopplerTx", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_full_bandwidth_ul_mu_mimo", amxd_object_get_value(bool, pVendorObj, "HePhyFullBandwidthUlMuMimo", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_partial_bandwidth_ul_mu_mimo", amxd_object_get_value(bool, pVendorObj, "HePhyPartialBandwidthUlMuMimo", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_partial_bandwidth_extended_range", amxd_object_get_value(bool, pVendorObj, "HePhyPartialBWExtendedRange", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_triggered_cqi_feedback", amxd_object_get_value(bool, pVendorObj, "HePhyTriggeredCqiFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_ppe_thresholds_present", amxd_object_get_value(bool, pVendorObj, "HePhyPpeThresholdsPresent", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_codebook_size42_for_su_support", amxd_object_get_value(bool, pVendorObj, "HePhyCodebookSize42SuSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_codebook_size75_for_mu_support", amxd_object_get_value(bool, pVendorObj, "HePhyCodebookSize75MuSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_power_boost_factor_alpha_support", amxd_object_get_value(bool, pVendorObj, "HePhyPowBoostFactAlphaSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_om_control_ul_mu_data_disable_rx_support", amxd_object_get_value(bool, pVendorObj, "HeMacOmCtrlMuDisableRxSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_ul_2x996tone_ru_support", amxd_object_get_value(bool, pVendorObj, "HeMacUl2x996ToneRuSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_ack_enabled_aggregation_support", amxd_object_get_value(bool, pVendorObj, "HeMacAckEnabledAggrSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_broadcast_twt_support", amxd_object_get_value(bool, pVendorObj, "HeMacBroadcastTwtSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_dcm_max_bw", amxd_object_get_value(bool, pVendorObj, "HePhyDcmMaxBw", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_longer_than_16_he_sigb_ofdm_sym_support", amxd_object_get_value(bool, pVendorObj, "HePhyLong16HeSigOfdmSymSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_ndp_feedback_report_support", amxd_object_get_value(bool, pVendorObj, "HeMacNdpFeedbackReportSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_rx_1024_qam_lt_242_tone_ru_support", amxd_object_get_value(bool, pVendorObj, "HePhyRx1024QLt242ToneRuSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_rx_full_bw_su_using_mu_comp_sigb", amxd_object_get_value(bool, pVendorObj, "HePhyRxFullBwSuUsingMuCompSigb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_rx_full_bw_su_using_mu_non_comp_sigb", amxd_object_get_value(bool, pVendorObj, "HePhyRxFulBwUsingMuNonComSigb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_stbc_tx_less_than_or_equal_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyStbcTxLessThanOrEq80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_stbc_tx_greater_than_80mhz", amxd_object_get_value(bool, pVendorObj, "HePhyStbcTxGreaterThan80Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_operation_er_su_disable", amxd_object_get_value(bool, pVendorObj, "HeOperationErSuDisable", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_er_su_ppdu_4x_ltf_8us_gi", amxd_object_get_value(bool, pVendorObj, "HePhyErSuPpdu4xLtf8UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_phy_preamble_puncturing_rx", amxd_object_get_value(bool, pVendorObj, "HePhyPreamblePuncturingRx", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_multi_tid_aggregation_tx_support", amxd_object_get_value(bool, pVendorObj, "HeMacMultiTidAggrTxSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "he_mac_multi_tid_aggregation_rx_support", amxd_object_get_value(bool, pVendorObj, "HeMacMultiTidAggrRxSupport", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_num_sounding_dim_80_mhz_or_below", amxd_object_get_value(bool, pVendorObj, "EhtPhyNumSoundDim80MhzOrBelow", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_num_sounding_dim_160_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyNumSoundingDim160Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_num_sounding_dim_320_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyNumSoundingDim320Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_mu_beamformer_bw_80_mhz_or_below", amxd_object_get_value(bool, pVendorObj, "EhtPhyMuBeamformerBw80MhzBelow", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_mu_beamformer_bw_160_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyMuBeamformerBw160Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_mu_beamformer_bw_320_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyMuBeamformerBw320Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_ndp_4x_eht_ltf_and_3_2_us_gi", amxd_object_get_value(bool, pVendorObj, "EhtPhyNdp4xEhtLtfAnd32UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_partial_bw_ul_mu_mimo", amxd_object_get_value(bool, pVendorObj, "EhtPhyPartialBwUlMuMimo", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_beamformee_ss_80_mhz_or_below", amxd_object_get_value(bool, pVendorObj, "EhtPhyBeamformeeSs80MhzOrBelow", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_beamformee_ss_160_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyBeamformeeSs160Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_beamformee_ss_320_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyBeamformeeSs320Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_eht_dup_in_6_ghz", amxd_object_get_value(bool, pVendorObj, "EhtPhyEhtDupIn6Ghz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_20_mhz_operating_sta_rx_ndp_with_wider_bw", amxd_object_get_value(bool, pVendorObj, "EhtPhy20MhzOpStaRxNdpWiderBw", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_ng_16_su_feedback", amxd_object_get_value(bool, pVendorObj, "EhtPhyNg16SuFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_ng_16_mu_feedback", amxd_object_get_value(bool, pVendorObj, "EhtPhyNg16MuFeedback", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_codebook_size_4_2_su_fb", amxd_object_get_value(bool, pVendorObj, "EhtPhyCodebookSize42SuFb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_codebook_size_75_5_mu_fb", amxd_object_get_value(bool, pVendorObj, "EhtPhyCodebookSize755MuFb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_trig_su_bf_fb", amxd_object_get_value(bool, pVendorObj, "EhtPhyTrigSuBfFb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_trig_cqi_fb", amxd_object_get_value(bool, pVendorObj, "EhtPhyTrigCqiFb", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_partial_bw_dl_mu_mimo", amxd_object_get_value(bool, pVendorObj, "EhtPhyPartialBwDlMuMimo", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_psr_based_sr", amxd_object_get_value(bool, pVendorObj, "EhtPhyPsrBasedSr", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_eht_mu_ppdu_with_4x_eht_ltf_and_0_8_us_gi", amxd_object_get_value(bool, pVendorObj, "EhtPhyEhtMuPpdu4xEhtLtf08UsGi", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_rx_1024_qam_and_4096_qam_below_242_ru", amxd_object_get_value(bool, pVendorObj, "EhtPhyRx1024Qam4096QamBel242Ru", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_max_num_of_supported_eht_ltfs", amxd_object_get_value(bool, pVendorObj, "EhtPhyMaxNumOfSupportedEhtLtfs", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_mcs_15", amxd_object_get_value(bool, pVendorObj, "EhtPhyMcs15", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_non_ofdma_ul_mu_mimo_bw_80_mhz_or_below", amxd_object_get_value(bool, pVendorObj, "EhtPhyNonOfdmaMuMimo80MhzBelow", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_non_ofdma_ul_mu_mimo_bw_160_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyNonOfdmaUlMuMimoBw160Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "eht_phy_non_ofdma_ul_mu_mimo_bw_320_mhz", amxd_object_get_value(bool, pVendorObj, "EhtPhyNonOfdmaUlMuMimoBw320Mhz", NULL));
        swl_mapCharFmt_addValInt32(configMap, "rnr_tbtt_mld_non_zero_pad", amxd_object_get_value(uint8_t, pVendorObj, "RnrTbttMldNonZeroPad", NULL));
    }
}

static swl_rc_ne s_mxl_rad_configObssScanParams(amxd_object_t* pVendorObj, swl_mapChar_t* configMap, bool coexistanceEnabled)
{
    amxd_object_t* obssObj = amxd_object_get(pVendorObj, "ObssScanParams");
    ASSERT_NOT_NULL(obssObj, SWL_RC_ERROR, ME, "No ObssScanParams vendor obj");

    uint32_t scanPassiveDwell             = amxd_object_get_value(uint32_t, obssObj, "ScanPassiveDwell",             NULL);
    uint32_t scanActiveDwell              = amxd_object_get_value(uint32_t, obssObj, "ScanActiveDwell",              NULL);
    uint32_t scanPassiveTotalPerChannel   = amxd_object_get_value(uint32_t, obssObj, "ScanPassiveTotalPerChannel",   NULL);
    uint32_t scanActiveTotalPerChannel    = amxd_object_get_value(uint32_t, obssObj, "ScanActiveTotalPerChannel",    NULL);
    uint32_t channelTransitionDelayFactor = amxd_object_get_value(uint32_t, obssObj, "ChannelTransitionDelayFactor", NULL);
    uint32_t scanActivityThreshold        = amxd_object_get_value(uint32_t, obssObj, "ScanActivityThreshold",        NULL);

    /* Configure OBSS Interval
           If obss coexistance is enabled then obssInterval = value from DM
       otherwise value is set explicitly to 0
    */
    int obssInterval = amxd_object_get_value(int32_t, obssObj, "ObssInterval", NULL);
    swl_mapCharFmt_addValInt32(configMap, "obss_interval", (coexistanceEnabled ? obssInterval : 0));

    /* OBSS Scan Params */
    WHM_MXL_NE_SET_PARAM(scanPassiveDwell,             DEF_SCAN_PASSIVE_DWELL,           configMap, "scan_passive_dwell");
    WHM_MXL_NE_SET_PARAM(scanActiveDwell,              DEF_SCAN_ACTIVE_DWELL,            configMap, "scan_active_dwell");
    WHM_MXL_NE_SET_PARAM(scanPassiveTotalPerChannel,   DEF_SCAN_PASSIVE_TOTAL_PER_CHAN,  configMap, "scan_passive_total_per_channel");
    WHM_MXL_NE_SET_PARAM(scanActiveTotalPerChannel,    DEF_SCAN_ACTIVE_TOTAL_PER_CHAN,   configMap, "scan_active_total_per_channel");
    WHM_MXL_NE_SET_PARAM(channelTransitionDelayFactor, DEF_CHAN_TRANSITION_DELAY_FACTOR, configMap, "channel_transition_delay_factor");
    WHM_MXL_NE_SET_PARAM(scanActivityThreshold,        DEF_SCAN_ACTIVITY_THRESHOLD,      configMap, "scan_activity_threshold");

    return SWL_RC_OK;
}

static void s_mxl_rad_setPowerSelectionParam(amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pVendorObj, , ME, "pVendorObj is NULL");
    int8_t *txPowVal = whm_mxl_rad_txPercentToPower(amxd_object_get_value(int32_t, pVendorObj, "SetPowerSelection", NULL));
    ASSERT_NOT_NULL(txPowVal, , ME, "txPowVal is NULL");
    swl_mapCharFmt_addValInt32(configMap, "sPowerSelection", *txPowVal);
}

static swl_rc_ne s_mxl_rad_updateConfig(T_Radio* pRad, mxl_VendorData_t* pRadVendor, swl_mapChar_t* configMap) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(pRadVendor, SWL_RC_INVALID_PARAM, ME, "NULL");
    /*Take pointer to the Vendor Object*/
    amxd_object_t* pVendorObj = pRadVendor->pBus;
    ASSERT_NOT_NULL(pVendorObj, SWL_RC_INVALID_PARAM, ME, "NULL");
    int bfMode = amxd_object_get_value(int32_t, pVendorObj, "SetBfMode", NULL);
    bool heDebugMode = amxd_object_get_value(bool, pVendorObj, "HeDebugMode", NULL);
    bool heBeacon = amxd_object_get_value(bool, pVendorObj, "HeBeacon", NULL);
    bool disableMbssid = amxd_object_get_value(bool, pVendorObj, "OverrideMBSSID", NULL);
    bool duplicateBeacon = amxd_object_get_value(bool, pVendorObj, "DuplicateBeaconEnabled", NULL);
    bool backgroundCac = amxd_object_get_value(bool, pVendorObj, "BackgroundCac", NULL);
    uint32_t duplicateBeaconBw = amxd_object_get_value(uint32_t, pVendorObj, "DuplicateBeaconBw", NULL);
    uint32_t twtResponderSupport = amxd_object_get_value(uint32_t, pVendorObj, "TwtResponderSupport", NULL);
    uint32_t heMacTwtResponderSupport = amxd_object_get_value(uint32_t, pVendorObj, "HeMacTwtResponderSupport", NULL);
    uint32_t probeReqListTimer = amxd_object_get_value(uint32_t, pVendorObj, "ProbeReqListTimer", NULL);
    uint32_t subBandDFS = amxd_object_get_value(uint32_t, pVendorObj, "SubBandDFS", NULL);
    uint16_t apMaxSta = amxd_object_get_value(uint16_t, pVendorObj, "ApMaxNumSta", NULL);
    uint16_t punctureBitMap = amxd_object_get_value(uint16_t, pVendorObj, "PunctureBitMap", NULL);
    uint32_t max_bss = wld_rad_countMappedAPs(pRad);
    char *dfsChStateFile = amxd_object_get_value(cstring_t, pVendorObj, "DfsChStateFile", NULL);
    char *setCcaTh = amxd_object_get_value(cstring_t, pVendorObj, "SetCcaTh", NULL);
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
    swl_radBw_e curBandwidth = pRad->operatingChannelBandwidth;
    swl_chanspec_t tgtChspec = wld_chanmgt_getTgtChspec(pRad);
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

    /* Delete unsupported params first */
    if (swl_mapChar_has(configMap, "mbssid")) {
        swl_mapChar_delete(configMap, "mbssid");
    }

    /* 6G Band Only Parameters */
    if(wld_rad_is_6ghz(pRad)) {
        /* Prepare hostapd_conf AFC parameters */
        whm_mxl_rad_afcUpdateConfigMap(pVendorObj, configMap);
        /*
        Currently, PWHM is setting 6G HE Capabilities to a minimun, waiting for proper 6ghz caps parsing to improve configuration
        We will set it back to the default values as per hostapd-ng. 
        Values can still be configured via data model.
        */
        whm_mxl_rad_configHe6gCapabs(pVendorObj, configMap);

        WHM_MXL_NE_SET_PARAM(disableMbssid, 0, configMap, "override_6g_mbssid_default_mode");
        WHM_MXL_NE_SET_PARAM(heBeacon, 0, configMap, "he_beacon");
        WHM_MXL_NE_SET_PARAM(duplicateBeacon, 0, configMap, "duplicate_beacon_enabled");
        if(duplicateBeacon) {
            swl_mapCharFmt_addValInt32(configMap, "duplicate_beacon_bw", duplicateBeaconBw);
        }
        /* mod-whm should supply the max_bss count to the hostapd for the MBSSID feature */
        swl_mapCharFmt_addValInt32(configMap, "max_bss", max_bss);
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
        if(pRad->autoChannelEnable) {
            /* Setting the acs_eht_mode in the hostapd_conf when ACS is enabled in case of 320MHz*/
            if(tgtChspec.bandwidth == SWL_BW_320MHZ) {
                if(curBandwidth == SWL_RAD_BW_320MHZ2) {
                    swl_mapCharFmt_addValInt32(configMap, "acs_eht_mode", 2);
                } else if(curBandwidth == SWL_RAD_BW_320MHZ1) {
                    swl_mapCharFmt_addValInt32(configMap, "acs_eht_mode", 1);
                } else {
                    swl_mapCharFmt_addValInt32(configMap, "acs_eht_mode", 0);
                }
            }
        }
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
        if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_BE)) {
            WHM_MXL_NE_SET_PARAM(punctureBitMap, 0, configMap, "punct_bitmap");
        }
        if (whm_mxl_isTgtChannelWidthEqual(pRad, SWL_BW_20MHZ)) {
            if ((pRad->autoChannelEnable) || (pRadVendor->firstNonDfs)) {
                swl_mapCharFmt_addValInt32(configMap, "he_oper_centr_freq_seg0_idx", 0);
            }
        }
    }
    else { /* non 6G Band Parameters */
        bool ignore40MhzIntolerant = amxd_object_get_value(bool, pVendorObj, "Ignore40MhzIntolerant", NULL);
        int32_t obssBeaconRssiThreshold = amxd_object_get_value(int32_t, pVendorObj, "ObssBeaconRssiThreshold", NULL);

        /* ignore_40_mhz_intolerant:
         * 2.4 and 5 GHz: If set do not perform HT scan and no overlap rules check;
         * 2.4 GHz additional: Ignore 40 MHz intolerant STAs */
        WHM_MXL_GT_SET_PARAM(ignore40MhzIntolerant, 0, configMap, "ignore_40_mhz_intolerant");
        /* obss_beacon_rssi_threshold: Ignore overlapping BSSes whose RSSI is below this threshold */
        WHM_MXL_NE_SET_PARAM(obssBeaconRssiThreshold, DEF_OBSS_RSSI_THRESHOLD, configMap, "obss_beacon_rssi_threshold");
    }

    WHM_MXL_NE_SET_PARAM(apMaxSta, DEFAULT_AP_MAX_STA, configMap, "ap_max_num_sta");
    swl_mapCharFmt_addValInt32(configMap, "sProbeReqCltMode", amxd_object_get_value(bool, pVendorObj, "SetProbeReqCltMode", NULL));
    swl_mapCharFmt_addValInt32(configMap, "dynamic_edca", amxd_object_get_value(bool, pVendorObj, "DynamicEdca", NULL));
    WHM_MXL_GT_SET_PARAM(bfMode, -1, configMap, "sBfMode");
    WHM_MXL_NE_SET_PARAM(twtResponderSupport, 1, configMap, "twt_responder_support");
    WHM_MXL_GT_SET_PARAM(heMacTwtResponderSupport, 0, configMap, "he_mac_twt_responder_support");
    WHM_MXL_GT_SET_PARAM(probeReqListTimer, 0, configMap, "ProbeReqListTimer");

    s_mxl_rad_setPowerSelectionParam(pVendorObj, configMap);

    /* 2.4G Band Only Parameters */
    if (wld_rad_is_24ghz(pRad)) {
        s_mxl_rad_configObssScanParams(pVendorObj, configMap, pRad->obssCoexistenceEnabled);
        if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_N)) {
            swl_mapCharFmt_addValInt32(configMap, "sQAMplus", amxd_object_get_value(bool, pVendorObj, "SetQAMplus", NULL));
        }
    }

    /* 5G Band Only Parameters */
    if (wld_rad_is_5ghz(pRad)) {
        swl_mapCharFmt_addValInt32(configMap, "sRadarRssiTh", amxd_object_get_value(int8_t, pVendorObj, "SetRadarRssiTh", NULL));
        WHM_MXL_GT_SET_PARAM(subBandDFS, 0, configMap, "sub_band_dfs");
        WHM_MXL_GT_SET_PARAM(backgroundCac, 0, configMap, "background_cac");
    }

    if(!swl_str_isEmpty(dfsChStateFile)) {
        swl_mapCharFmt_addValStr(configMap, "dfs_channels_state_file_location", "%s", dfsChStateFile);
    }
    free(dfsChStateFile);
    if(!swl_str_matches(setCcaTh, "-62 -62 -72 -72 -69")) {
        swl_mapCharFmt_addValStr(configMap, "sCcaTh", "%s", setCcaTh);
    }
    free(setCcaTh);
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
    if(amxd_object_get_value(int32_t, pVendorObj, "DfsDebugChan", NULL) != -1) {
        swl_mapCharFmt_addValInt32(configMap, "dfs_debug_chan", amxd_object_get_value(int32_t, pVendorObj, "DfsDebugChan", NULL));
    }
    if(amxd_object_get_value(int32_t, pVendorObj, "ZwdfsDebugChan", NULL) != -1) {
        swl_mapCharFmt_addValInt32(configMap, "zwdfs_debug_chan", amxd_object_get_value(int32_t, pVendorObj, "ZwdfsDebugChan", NULL));
    }
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
    /* Configure 80211AX Only Params */
    if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AX)) {
        /* Configure AX MxL Params */
        whm_mxl_rad_configAxMxlParams(pRad, configMap);
        WHM_MXL_GT_SET_PARAM(heDebugMode, 0, configMap, "enable_he_debug_mode");
    }
    /* Configure 80211N Only Params */
    if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_N)) {
        /* Configure HT Capabilites */
        whm_mxl_rad_configHtCapabs(pRad, configMap);
    }
    /* Configure 80211AC Only Params */
    if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AC)) {
        /* Configure VHT Capabilites */
        whm_mxl_rad_configVhtCapabs(pRad, configMap);
        swl_mapCharFmt_addValStr(configMap, "opmode_notif", "%u", 1);
    }

    /* Force-enable BE if override is enabled */
    if (whm_mxl_rad_checkForceEnableBe(pRad)) {
        swl_mapCharFmt_addValInt32(configMap, "ieee80211be", 1);
        s_rad_configEhtCapabs(pRad, configMap);
    }

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
    /* Prepare hostapd_conf ACS parameters */
    whm_mxl_rad_acsUpdateConfigMap(pRad, pRadVendor, configMap);
    SAH_TRACEZ_INFO(ME, "%s autoChannelEnable : %d", pRad->Name, pRad->autoChannelEnable);
    if (pRad->autoChannelEnable) {
         /* Set hostapd_conf autoChannelEnable */
        SAH_TRACEZ_INFO(ME, "%s autoChannelEnable : %d", pRad->Name, pRad->autoChannelEnable);
        swl_mapCharFmt_addValStr(configMap, "channel", "%s", "acs_smart");
        /* Reset eht/vht/he seg0_idx in case ACS is enabled */
        if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AC)) {
            swl_mapCharFmt_addValInt32(configMap, "vht_oper_centr_freq_seg0_idx", 0);
        }
        if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_AX)) {
            swl_mapCharFmt_addValInt32(configMap, "he_oper_centr_freq_seg0_idx", 0);
        }
        if (wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_BE)) {
            swl_mapCharFmt_addValInt32(configMap, "eht_oper_centr_freq_seg0_idx", 0);
        }
        if (pRadVendor->bgAcsInterval) {
            /* Set bg acs interval in minutes */
            swl_mapCharFmt_addValInt32(configMap, "acs_bgscan_interval", pRadVendor->bgAcsInterval);
        }
    } else if (pRadVendor->firstNonDfs) {
#else
    if (pRadVendor->firstNonDfs) {
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
        SAH_TRACEZ_INFO(ME, "%s first_non_dfs : %d", pRad->Name, pRadVendor->firstNonDfs);
        /* Set hostapd_conf first_non_dfs parameters */
        swl_mapCharFmt_addValStr(configMap, "channel", "%s", "first_non_dfs");
    }

    /* Prepare hostapd_conf Bss Color parameters */
    whm_mxl_rad_bssColorUpdateConfigMap(pRadVendor, configMap);

    /* Prepare Start After parameters */
    whm_mxl_rad_delayedStartUpdateConfigMap(pRadVendor, configMap);

    /* Certification Params*/
    if (whm_mxl_isCertModeEnabled()) {
        whm_mxl_rad_configCertification(pRad, pVendorObj, configMap);
        swl_mapCharFmt_addValInt32(configMap, "max_bss", max_bss);
    }

    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

static swl_rc_ne s_rad_zwdfsUpdateConfigMap(swl_mapChar_t* configMap) {
    SAH_TRACEZ_IN(ME);

    /*
     * The ZW DFS radio instance is not added to the DM radio list because the device is meant only for DFS.
     * In this section we'll retrieve the main 5GHz radio config based on the frequency band.
     * After that, an override of HT/VHT/HE caps is needed to be able to start hostapd for the ZW DFS radio.
     */
    swl_mapCharFmt_addValStr(configMap, "channel", "%s", "first_non_dfs");
    swl_mapCharFmt_addValStr(configMap, "ht_capab", "%s", "[HT40+][HT40-][LDPC][SHORT-GI-20][SHORT-GI-40][TX-STBC][MAX-AMSDU-7935][DSSS_CCK-40]");
    swl_mapCharFmt_addValStr(configMap, "op_class", "%s", "130");
    swl_mapCharFmt_addValStr(configMap, "vht_oper_chwidth", "%s", "2");
    swl_mapCharFmt_addValStr(configMap, "vht_oper_centr_freq_seg0_idx", "%s", "0");
    swl_mapCharFmt_addValStr(configMap, "vht_capab", "%s",
    "[RXLDPC][SHORT-GI-80][SHORT-GI-160][TX-STBC-2BY1][SU-BEAMFORMER][SU-BEAMFORMEE][MU-BEAMFORMER][RX-ANTENNA-PATTERN][VHT160][MAX-MPDU-11454][MAX-A-MPDU-LEN-EXP7][BF-ANTENNA-][SOUNDING-DIMENSION-]");

    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_rad_updateConfigMap(T_Radio* pRad, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    ASSERTS_NOT_NULL(configMap, SWL_RC_INVALID_PARAM, ME, "configMap is NULL");

    T_Radio* pRadZwDfs = mxl_rad_getZwDfsRadio();
    if(pRadZwDfs && swl_str_matches(pRad->Name, pRadZwDfs->Name)) {
        return s_rad_zwdfsUpdateConfigMap(configMap);
    }

    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    return s_mxl_rad_updateConfig(pRad, pRadVendor, configMap);
}

/*  *****************************************************************************
*                                                                              *
*                           VAP Related Configurations                         *
*                                                                              *
*  *****************************************************************************/

static void whm_mxl_vap_multiApConfig(T_AccessPoint* pAP, amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    amxd_object_t* pMultiApObj = amxd_object_get(pVendorObj, "MultiAp");
    ASSERT_NOT_NULL(pMultiApObj, , ME, "pMultiApObj is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    whm_mxl_multi_ap_type_e multiApType = mxlVapVendorData->mxlMultiApType;
    int mapClientDisallow = amxd_object_get_value(int32_t, pMultiApObj, "ClientDisallow", NULL);

    switch(multiApType) {
        /* Hybrid AP */
        case MXL_HYBRID_MAP: {
            SAH_TRACEZ_INFO(ME, "%s: hybrid multi AP configured", pAP->alias);
            WHM_MXL_GT_SET_PARAM(mapClientDisallow, 0, configMap, "multi_ap_client_disallow");
            break;
        }
        /* Backhaul AP */
        case MXL_BACKHAUL_MAP: {
            SAH_TRACEZ_INFO(ME, "%s: backhaul multi AP configured", pAP->alias);
            WHM_MXL_GT_SET_PARAM(mapClientDisallow, 0, configMap, "multi_ap_client_disallow");
            break;
        }
        /* Fronthaul AP */ 
        case MXL_FRONTHAUL_MAP: {
            SAH_TRACEZ_INFO(ME, "%s: fronthaul multi AP configured", pAP->alias);
            break;
        }
        default:
            break;
    }
}

static void whm_mxl_vap_mloConfig(T_AccessPoint* pAP, amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    amxd_object_t* pMloObj = amxd_object_get(pVendorObj, "MLO");
    ASSERT_NOT_NULL(pMloObj, , ME, "pMloObj is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");

    if (mxlVapVendorData->mloId == -1) {
        swl_mapCharFmt_addValInt32(configMap, "mlo_enable", 0);
    }
    char*    apMldMac          = amxd_object_get_value(cstring_t, pMloObj, "ApMldMac", NULL);
    bool     wdsSingleMlAssoc  = amxd_object_get_value(bool, pMloObj, "WdsSingleMlAssoc", NULL);
    bool     wdsPrimaryLink    = amxd_object_get_value(bool, pMloObj, "WdsPrimaryLink", NULL);
    if (mxlVapVendorData->mloId > -1) {
        swl_mapCharFmt_addValInt32(configMap, "mlo_enable", 1);
    }
    swl_mapCharFmt_addValStr(configMap, "wds_single_ml_assoc", "%d", wdsSingleMlAssoc);
    swl_mapCharFmt_addValStr(configMap, "wds_primary_link", "%d", wdsPrimaryLink);
    if(!(swl_str_isEmpty(apMldMac))) {
        swl_mapCharFmt_addValStr(configMap, "ap_mld_mac", "%s", apMldMac);
    }
    free(apMldMac);
}

static void whm_mxl_vap_softBlockConfig(T_AccessPoint* pAP, amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    amxd_object_t* pSoftBlockObj = amxd_object_get(pVendorObj, "SoftBlock");
    ASSERT_NOT_NULL(pSoftBlockObj, , ME, "pSoftBlockObj is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");

    int32_t  SoftBlockAclEnable     = amxd_object_get_value(int32_t, pSoftBlockObj, "SoftBlockAclEnable", NULL);
    uint32_t SoftBlockAclWaitTime   = amxd_object_get_value(uint32_t, pSoftBlockObj, "SoftBlockAclWaitTime", NULL);
    uint32_t SoftBlockAclAllowTime  = amxd_object_get_value(uint32_t, pSoftBlockObj, "SoftBlockAclAllowTime", NULL);
    int32_t  SoftBlockAclOnAuthReq  = amxd_object_get_value(int32_t, pSoftBlockObj, "SoftBlockAclOnAuthReq", NULL);
    int32_t  SoftBlockAclOnProbeReq = amxd_object_get_value(int32_t, pSoftBlockObj, "SoftBlockAclOnProbeReq", NULL);

    swl_mapCharFmt_addValInt32(configMap, "soft_block_acl_enable", SoftBlockAclEnable);
    if(SoftBlockAclEnable) {
        swl_mapCharFmt_addValInt32(configMap, "soft_block_acl_wait_time", SoftBlockAclWaitTime);
        swl_mapCharFmt_addValInt32(configMap, "soft_block_acl_allow_time", SoftBlockAclAllowTime);
        swl_mapCharFmt_addValInt32(configMap, "soft_block_acl_on_auth_req", SoftBlockAclOnAuthReq);
        swl_mapCharFmt_addValInt32(configMap, "soft_block_acl_on_probe_req", SoftBlockAclOnProbeReq);
    }
}

static void s_whm_mxl_vap_securityConfig(T_AccessPoint* pAP, swl_mapChar_t* configMap) {
    SAH_TRACEZ_IN(ME);
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    bool forceEnableBe = whm_mxl_rad_checkForceEnableBe(pAP->pRadio);
    swl_security_mfpMode_e mfp = swl_security_getTargetMfpMode(pAP->secModeEnabled, pAP->mfpConfig);

    switch(pAP->secModeEnabled) {
        case SWL_SECURITY_APMODE_WPA2_WPA3_P:
            if(forceEnableBe) {
                swl_mapCharFmt_addValStr(configMap, "wpa_key_mgmt", "%s%s%s",
                                         ((mfp != SWL_SECURITY_MFPMODE_REQUIRED) ? "WPA-PSK " : ""),
                                         ((mfp == SWL_SECURITY_MFPMODE_REQUIRED) ? "WPA-PSK-SHA256 " : ""),
                                         "SAE SAE-EXT-KEY");
                swl_mapCharFmt_addValStr(configMap, "wpa_pairwise", "%s", "CCMP GCMP-256");
                swl_mapCharFmt_addValStr(configMap, "rsn_pairwise", "%s", "CCMP GCMP-256");
                swl_mapChar_add(configMap, "beacon_prot", "1");
            }
            break;
        case SWL_SECURITY_APMODE_WPA3_P:
            if(forceEnableBe) {
                swl_mapCharFmt_addValStr(configMap, "wpa_key_mgmt", "%s%s",
                                         "SAE SAE-EXT-KEY",
                                         (pAP->IEEE80211rEnable ? " FT-SAE FT-SAE-EXT-KEY" : ""));
                swl_mapCharFmt_addValStr(configMap, "wpa_pairwise", "%s", "CCMP GCMP-256");
                swl_mapCharFmt_addValStr(configMap, "rsn_pairwise", "%s", "CCMP GCMP-256");
                swl_mapChar_add(configMap, "beacon_prot", "1");
            }
            /* Overwrite 6G sae_pwe to 2, pWHM is setting it to 1 but at the moment its not supported by us */
            if(pAP->pFA->mfn_misc_has_support(pAP->pRadio, pAP, "SAE_PWE", 0)) {
                if(pAP->pRadio->operatingFrequencyBand == SWL_FREQ_BAND_EXT_6GHZ) {
                    SAH_TRACEZ_INFO(ME, "%s Set sae_pwe",pAP->alias);
                    swl_mapCharFmt_addValStr(configMap, "sae_pwe", "%s", "2");
                }
                if(whm_mxl_isCertModeEnabled()) {
                    /*
                    * Overriding the rsn_pairwise and wpa_pairwise as one of the
                    * test case is failing in checking for the suite count to be 1
                    * So this is a workaround till WFA updates their sniffer validation checks
                    */
                    if(!whm_mxl_isWpa3CertModeEnabled() && wld_rad_checkEnabledRadStd(pAP->pRadio, SWL_RADSTD_BE)) {
                        SAH_TRACEZ_INFO(ME, "%s Overriding the wpa_pairwise and rsn_pairwise",pAP->alias);
                        swl_mapChar_delete(configMap, "rsn_pairwise");
                        swl_mapChar_delete(configMap, "wpa_pairwise");
                        swl_mapCharFmt_addValStr(configMap, "wpa_pairwise", "%s", "GCMP-256");
                        swl_mapCharFmt_addValStr(configMap, "rsn_pairwise", "%s", "GCMP-256");
                    }
                    /*
                    * Overriding the sae_pwe value to 1 for all bands in BE mode
                    * So that the AP-MLD formed between the 2.4GHz + 6GHZ and 5GHZ + 6GHz
                    * Can come up without issue
                    */
                    if(wld_rad_checkEnabledRadStd(pAP->pRadio, SWL_RADSTD_BE) ||
                      (pAP->pRadio->operatingFrequencyBand == SWL_FREQ_BAND_EXT_6GHZ) ||
                      (mxlVapVendorData->h2eRequired)) {
                        SAH_TRACEZ_INFO(ME, "%s Set sae_pwe",pAP->alias);
                        swl_mapCharFmt_addValStr(configMap, "sae_pwe", "%s", "1");
                    }
                }
            }
            /* Overwrite wpa_key_mgmt to SAE-EXT-KEY to support AKM24 */
            if(mxlVapVendorData->saeExtKey) {
                swl_mapCharFmt_addValStr(configMap, "wpa_key_mgmt", "%s", "SAE-EXT-KEY");
            }
            break;
        case SWL_SECURITY_APMODE_OWE:
            if(forceEnableBe) {
                swl_mapCharFmt_addValStr(configMap, "wpa_pairwise", "%s", "CCMP GCMP-256");
                swl_mapCharFmt_addValStr(configMap, "rsn_pairwise", "%s", "CCMP GCMP-256");
                swl_mapChar_add(configMap, "beacon_prot", "1");
            }
            if(mxlVapVendorData && !swl_str_isEmpty(mxlVapVendorData->OWETransBSSID)) {
                swl_mapCharFmt_addValStr(configMap, "owe_transition_bssid", "%s", mxlVapVendorData->OWETransBSSID);
            }
            if(mxlVapVendorData && !swl_str_isEmpty(mxlVapVendorData->OWETransSSID)) {
                /* Hostapd requires OWE Tranisiton SSID to be included in double quotes */
                swl_mapCharFmt_addValStr(configMap, "owe_transition_ssid", "\"%s\"", mxlVapVendorData->OWETransSSID);
            }
            break;
        case SWL_SECURITY_APMODE_NONE:
            if(mxlVapVendorData && !swl_str_isEmpty(mxlVapVendorData->OWETransBSSID)) {
                swl_mapCharFmt_addValStr(configMap, "owe_transition_bssid", "%s", mxlVapVendorData->OWETransBSSID);
            }
            if(mxlVapVendorData && !swl_str_isEmpty(mxlVapVendorData->OWETransSSID)) {
                /* Hostapd requires OWE Tranisiton SSID to be included in double quotes */
                swl_mapCharFmt_addValStr(configMap, "owe_transition_ssid", "\"%s\"", mxlVapVendorData->OWETransSSID);
            }
            break;
        case SWL_SECURITY_APMODE_WPA2_E:
        case SWL_SECURITY_APMODE_WPA_WPA2_E:
        case SWL_SECURITY_APMODE_WPA2_WPA3_E:
        case SWL_SECURITY_APMODE_WPA3_E:
            if(whm_mxl_isCertModeEnabled()) {
                if(swl_mapChar_has(configMap, "auth_server_shared_secret")) {
                    swl_mapChar_delete(configMap, "auth_server_shared_secret");
                }
                if(mxlVapVendorData && !swl_str_isEmpty(mxlVapVendorData->radiusSecretKey)) {
                    swl_mapChar_add(configMap, "auth_server_shared_secret", mxlVapVendorData->radiusSecretKey);
                }
            }
        default:
            break;
    }

    // Set Security mode as WPA3 Personal Compatibility
    if (mxlVapVendorData->EnableWPA3PersonalCompatibility == true) {
        // Restore parameters from VAP data.
        swl_mapCharFmt_addValInt32(configMap, "wpa_group_rekey", pAP->rekeyingInterval);
        swl_mapCharFmt_addValStr(configMap, "wpa_ptk_rekey", "%s", "0");
        swl_mapCharFmt_addValStr(configMap, "wpa_passphrase", "%s", pAP->keyPassPhrase);
        if(!swl_str_isEmpty(pAP->saePassphrase)) {
            swl_mapCharFmt_addValStr(configMap, "sae_password", "%s", pAP->saePassphrase);
        }

        // Set common defaults
        swl_mapCharFmt_addValStr(configMap, "wpa", "%s", "2");
        swl_mapCharFmt_addValStr(configMap, "rsn_pairwise", "%s", "CCMP");
        swl_mapCharFmt_addValStr(configMap, "group_cipher", "%s", "CCMP");
        swl_mapCharFmt_addValStr(configMap, "sae_pwe", "%s", "1");
        swl_mapCharFmt_addValStr(configMap, "sae_require_mfp", "%s", "1");
        swl_mapCharFmt_addValStr(configMap, "rsn_override_key_mgmt_2", "%s", "SAE-EXT-KEY");
        swl_mapCharFmt_addValStr(configMap, "rsn_override_pairwise_2", "%s", "GCMP-256");
        swl_mapCharFmt_addValStr(configMap, "rsn_override_mfp_2", "%s", "2");
 
        // Set band specific defaults
        if (pAP->pRadio->operatingFrequencyBand == SWL_FREQ_BAND_EXT_2_4GHZ ||
            pAP->pRadio->operatingFrequencyBand == SWL_FREQ_BAND_EXT_5GHZ) {
            swl_mapCharFmt_addValStr(configMap, "wpa_key_mgmt", "%s", "WPA-PSK");
            swl_mapCharFmt_addValStr(configMap, "ieee80211w", "%s", "0");
            swl_mapCharFmt_addValStr(configMap, "rsn_override_key_mgmt", "%s", "SAE");
            swl_mapCharFmt_addValStr(configMap, "rsn_override_pairwise", "%s", "CCMP");
            swl_mapCharFmt_addValStr(configMap, "rsn_override_mfp", "%s", "2");
        } else if (pAP->pRadio->operatingFrequencyBand == SWL_FREQ_BAND_EXT_6GHZ) {
            swl_mapCharFmt_addValStr(configMap, "wpa_key_mgmt", "%s", "SAE");
            swl_mapCharFmt_addValStr(configMap, "ieee80211w", "%s", "2");
        }
    }

    SAH_TRACEZ_OUT(ME);
}

static void whm_mxl_vap_configCertification(T_AccessPoint* pAP, amxd_object_t* pVendorObj, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pVendorObj, , ME, "pVendorObj is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    char *setAggrConfig = amxd_object_get_value(cstring_t, pVendorObj, "SetAggrConfig", NULL);
    char *groupMgmtCipher = amxd_object_get_value(cstring_t, pVendorObj, "GroupMgmtCipher", NULL);
    char *groupCipher = amxd_object_get_value(cstring_t, pVendorObj, "GroupCipher", NULL);
    char *qosMap = amxd_object_get_value(cstring_t, pVendorObj, "QoSMap", NULL);

    if(!swl_str_isEmpty(setAggrConfig)) {
        swl_mapCharFmt_addValStr(configMap, "sAggrConfig", "%s", setAggrConfig);
    }
    free(setAggrConfig);
    if(!swl_str_isEmpty(qosMap)) {
        swl_mapCharFmt_addValStr(configMap, "qos_map_set", "%s", qosMap);
    }
    free(qosMap);
    swl_mapCharFmt_addValInt32(configMap, "s11nProtection", amxd_object_get_value(uint32_t, pVendorObj, "Set11nProtection", NULL));
    swl_mapCharFmt_addValInt32(configMap, "mld_mediumsync_present", amxd_object_get_value(bool, pVendorObj, "MldMediumsyncPresent", NULL));
    swl_mapCharFmt_addValInt32(configMap, "mlo_t2lm_support", amxd_object_get_value(bool, pVendorObj, "MloT2lmSupport", NULL));
    swl_mapCharFmt_addValInt32(configMap, "rrm_neighbor_report", amxd_object_get_value(bool, pVendorObj, "RrmNeighRpt", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wnm_bss_trans_query_auto_resp", amxd_object_get_value(bool, pVendorObj, "WnmBssTransQueryAutoresp", NULL));
    swl_mapCharFmt_addValInt32(configMap, "eml_capab_transition_timeout", amxd_object_get_value(uint8_t, pVendorObj, "EmlCapabTransitionTimeout", NULL));
    swl_mapCharFmt_addValInt32(configMap, "ap_protected_keep_alive_required", amxd_object_get_value(int32_t, pVendorObj, "ApProtectedKeepAliveRequired", NULL));
    swl_mapCharFmt_addValInt32(configMap, "mld_mediumsync_duration", amxd_object_get_value(uint8_t, pVendorObj, "MldMediumsyncDuration", NULL));
    swl_mapCharFmt_addValInt32(configMap, "mld_mediumsync_ofdmedthresh", amxd_object_get_value(uint8_t, pVendorObj, "MldMediumsyncOfdmedthresh", NULL));
    swl_mapCharFmt_addValInt32(configMap, "mld_mediumsync_maxtxop", amxd_object_get_value(uint8_t, pVendorObj, "MldMediumsyncMaxtxop", NULL));
    swl_mapCharFmt_addValStr(configMap, "group_mgmt_cipher", "%s", groupMgmtCipher);
    free(groupMgmtCipher);
    swl_mapCharFmt_addValStr(configMap, "group_cipher", "%s", groupCipher);
    free(groupCipher);
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vi_cwmin", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVICWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vi_cwmax", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVICWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vi_aifs", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVIAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vi_txop_limit", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVITXOP", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vi_acm", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVIAcm", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vo_cwmin", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVOCWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vo_cwmax", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVOCWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vo_aifs", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVOAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vo_txop_limit", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVOTXOP", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_vo_acm", amxd_object_get_value(int32_t, pVendorObj, "WmmAcVOAcm", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_be_cwmin", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBECWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_be_cwmax", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBECWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_be_aifs", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBEAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_be_txop_limit", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBETXOP", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_be_acm", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBEAcm", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_bk_cwmin", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBKCWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_bk_cwmax", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBKCWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_bk_aifs", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBKAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_bk_txop_limit", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBKTXOP", NULL));
    swl_mapCharFmt_addValInt32(configMap, "wmm_ac_bk_acm", amxd_object_get_value(int32_t, pVendorObj, "WmmAcBKAcm", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data1_cwmin", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVICWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data1_cwmax", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVICWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data1_aifs", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVIAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data0_cwmin", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVOCWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data0_cwmax", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVOCWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data0_aifs", amxd_object_get_value(int32_t, pVendorObj, "TxQueueVOAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data2_cwmin", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBECWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data2_cwmax", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBECWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data2_aifs", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBEAifs", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data3_cwmin", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBKCWMin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data3_cwmax", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBKCWMax", NULL));
    swl_mapCharFmt_addValInt32(configMap, "tx_queue_data3_aifs", amxd_object_get_value(int32_t, pVendorObj, "TxQueueBKAifs", NULL));
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(int32_t, pVendorObj, "GasCBDelay", NULL), 0, configMap, "gas_comeback_delay");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(bool, pVendorObj, "EhtMacEpcsPrioAccess", NULL), 0, configMap, "eht_mac_epcs_prio_access");
}

static void s_whm_mxl_vap_steeringConfig(T_AccessPoint* pAP, swl_mapChar_t* configMap){
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, , ME, "mxlVapVendorData is NULL");
    /* For certification,
        ignore_da_timer is explicitly set to false,
        otherwise value is from internal variable updated from DM.
    */
    if (whm_mxl_isCertModeEnabled()) {
        swl_mapCharFmt_addValInt32(configMap, "ignore_da_timer", false);
    } else {
        swl_mapCharFmt_addValInt32(configMap, "ignore_da_timer", mxlVapVendorData->ignore11vDiassoc);
    }
}

static swl_rc_ne s_mxl_vap_updateConfig(T_AccessPoint* pAP, swl_mapChar_t* configMap) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");

    /* Take pointer to the Vendor Object */
    amxd_object_t* pVendorObj = amxd_object_get(pAP->pBus, "Vendor");
    ASSERT_NOT_NULL(pVendorObj, SWL_RC_INVALID_PARAM, ME, "pVendorObj NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    int32_t bssTransition = amxd_object_get_value(int32_t, pVendorObj, "BssTransition", NULL);
    uint32_t managementFramesRate = amxd_object_get_value(uint32_t, pVendorObj, "ManagementFramesRate", NULL);
    int32_t mgmtFramePowerControl = amxd_object_get_value(int32_t, pVendorObj, "MgmtFramePowerControl", NULL);
    int32_t numResSta = amxd_object_get_value(int32_t, pVendorObj, "NumResSta", NULL);
    bool vendorVht = amxd_object_get_value(bool, pVendorObj, "VendorVht", NULL);
    bool disablePbac = amxd_object_get_value(bool, pVendorObj, "DisablePbac", NULL);
    bool bssLoadIe = amxd_object_get_value(bool, pVendorObj, "EnableBssLoad", NULL);

    /* Configure parameters */
    /* Beacon parameters */
    swl_mapCharFmt_addValStr(configMap, "vendor_elements", "%s", LEGACY_VENDOR_ELEMENTS);
    /* Note: pWHM is already adding bss_transition but its commented out so we configure it again here */
    WHM_MXL_GT_SET_PARAM(bssTransition, 0, configMap, "bss_transition");
    WHM_MXL_GT_SET_PARAM(pAP->cfg11u.interworkingEnable, 0, configMap, "interworking");
    WHM_MXL_GT_SET_PARAM(numResSta, 0, configMap, "num_res_sta");
    WHM_MXL_NE_SET_PARAM(managementFramesRate, MGMT_FRAMES_RATE_DEFAULT, configMap, "management_frames_rate");
    WHM_MXL_NE_SET_PARAM(mgmtFramePowerControl, 0, configMap, "mgmt_frame_power_control");
    /* When we disable one Radio(either 2.4G or 5G)
    *  the RNR(MXL Properietory one) is getting
    *  overwritten because of OpenSource
    *  RNR enabled by default
    */
    if(wld_ap_getDiscoveryMethod(pAP) == M_AP_DM_RNR) {
        swl_mapCharFmt_addValInt32(configMap, "rnr", 0);
    }
    /* 6G only parameters */
    if(wld_rad_is_6ghz(pRad)) {
        WHM_MXL_GT_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "UnsolBcastProbeRespInterval", NULL), 0, configMap, "unsol_bcast_probe_resp_interval");
        WHM_MXL_GT_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "FilsDiscoveryMaxInterval", NULL), 0, configMap, "fils_discovery_max_interval");
    }
    /* 2.4G only parameters */
    if(wld_rad_is_24ghz(pRad)) {
        if(wld_rad_checkEnabledRadStd(pRad, SWL_RADSTD_N)) {
            WHM_MXL_NE_SET_PARAM(vendorVht, 0, configMap, "vendor_vht");
        } else {
            swl_mapCharFmt_addValInt32(configMap, "vendor_vht", 0);
        }
    }
    /* Multi AP parameters */
    whm_mxl_vap_multiApConfig(pAP, pVendorObj, configMap);
    /* Other parameters */
    swl_mapCharFmt_addValInt32(configMap, "enable_hairpin", amxd_object_get_value(bool, pVendorObj, "EnableHairpin", NULL));
    swl_mapCharFmt_addValInt32(configMap, "ap_max_inactivity", amxd_object_get_value(int32_t, pVendorObj, "ApMaxInactivity", NULL));
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "DynamicMulticastMode", NULL), 0, configMap, "dynamic_multicast_mode");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "DynamicMulticastRate", NULL), 1, configMap, "dynamic_multicast_rate");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "SetBridgeMode", NULL), 0, configMap, "sBridgeMode");
    WHM_MXL_NE_SET_PARAM(disablePbac, 0, configMap, "disable_pbac");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(uint32_t, pVendorObj, "MboCellAware", NULL), 1, configMap, "mbo_cell_aware");
    swl_mapCharFmt_addValInt32(configMap, "enable_bss_load_ie", bssLoadIe);
    /* Soft Block parameters */
    whm_mxl_vap_softBlockConfig(pAP, pVendorObj, configMap);
    /* Security Parameters */
    s_whm_mxl_vap_securityConfig(pAP, configMap);
    /* Override pWHM value of use_driver_iface_addr */
    swl_mapCharFmt_addValInt32(configMap, "use_driver_iface_addr", 0);
    /* Since MLO is not supported by pwhm Override disable 11be ble*/

    if(whm_mxl_rad_checkForceEnableBe(pRad)) {
        /* Force-enable BE stuff if override is enabled */
        swl_mapChar_delete(configMap, "disable_11be");
        whm_mxl_vap_mloConfig(pAP, pVendorObj, configMap);
        swl_mapCharFmt_addValInt32(configMap, "disable_beacon_prot", amxd_object_get_value(bool, pVendorObj, "DisableBeaconProtection", NULL));
    }

    /* Steering parameters */
    s_whm_mxl_vap_steeringConfig(pAP, configMap);

    /* SCS Parameters */
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(bool, pVendorObj, "SCSEnable", NULL), 0, configMap, "scs_enable");
    WHM_MXL_NE_SET_PARAM(amxd_object_get_value(bool, pVendorObj, "MSCSEnable", NULL), 0, configMap, "mscs_enable");

    /* Certification Params*/
    if (whm_mxl_isCertModeEnabled()) {
        whm_mxl_vap_configCertification(pAP, pVendorObj, configMap);
    }

    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

static swl_rc_ne s_mxl_vap_updateDummyVapConfig(T_AccessPoint* pAP, swl_mapChar_t* configMap) {
    SAH_TRACEZ_IN(ME);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    /* pAP must be a dummy vap */
    ASSERT_TRUE(whm_mxl_utils_isDummyVap(pAP), SWL_RC_INVALID_PARAM, ME, "pAP is not a dummy VAP");

    if (whm_mxl_utils_isAnyApActive(pRad)) {
        SAH_TRACEZ_INFO(ME,"%s: Deleting start_disabled for dummy vap due to at least on AP enabled on radio", pAP->alias);
        swl_mapChar_delete(configMap, "start_disabled");
    }
    if(whm_mxl_rad_checkForceEnableBe(pRad)) {
        swl_mapChar_delete(configMap, "disable_11be");
    }
    /* disable pbac option in dummy vap regardless of config as it is not needed */
    swl_mapCharFmt_addValInt32(configMap, "disable_pbac", 1);

    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_vap_updateConfigMap(T_AccessPoint* pAP, swl_mapChar_t* configMap) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    ASSERTS_NOT_NULL(configMap, SWL_RC_INVALID_PARAM, ME, "configMap is NULL");
    swl_rc_ne rc = SWL_RC_OK;

    if (whm_mxl_utils_isDummyVap(pAP)) {
        /* Configure hostapd conf for dummy VAP */
        SAH_TRACEZ_INFO(ME, "%s: Writing mxl config for dummy VAP", pAP->alias);
        rc = s_mxl_vap_updateDummyVapConfig(pAP, configMap);
    } else {
        /* Configure hostapd conf for operational VAP */
        SAH_TRACEZ_INFO(ME, "%s: Writing mxl config for operational VAP", pAP->alias);
        rc = s_mxl_vap_updateConfig(pAP, configMap);
    }

    return rc;
}

