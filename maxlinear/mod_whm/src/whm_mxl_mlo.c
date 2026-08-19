/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_mlo.c                                         *
*         Description  : MLO related API                                       *
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
#include "wld/wld_rad_hostapd_api.h"

#include "whm_mxl_utils.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_cfgActions.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_mlo.h"

#define ME "mxlMlo"

/**
 * @brief Counts number of AccessPoints Objects associated with the input MloId, not including dummy VAP
 *
 * @param id MloId
 * @return return count of AP objects with input MloId.
 */
int32_t whm_mxl_getNumMLlinksbyID(int32_t id) {
    ASSERT_FALSE((id < 0), 0, ME, "MLO: bad mloId(%d)", id);
    int32_t nrAp = 0;
    T_Radio* pRad = NULL;
    wld_for_eachRad(pRad) {
        T_AccessPoint* pAP = NULL;
        wld_rad_forEachAp(pAP, pRad) {
            mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
            if (mxlVapVendorData == NULL || (whm_mxl_utils_isDummyVap(pAP))) {
                continue;
            }
            if(mxlVapVendorData->mloId == id) {
                nrAp++;
            }
        }
    }
    return nrAp;
}

/**
 * @brief Check for the Sibling AP object based on the MloId, not including dummy VAP
 *
 * @param pAP AccessPoint
 * @param id MloId
 * @return return AP object with Matching MloId, otherwise NULL.
 */
T_AccessPoint* whm_mxl_getSiblingBss(T_AccessPoint* pAP, int32_t id) {
    ASSERTS_NOT_NULL(pAP, NULL, ME, "pAP is NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    T_Radio* tRad = NULL;
    T_AccessPoint* tAP = NULL;
    ASSERT_FALSE((id < 0), NULL, ME, "%s: bad mloId(%d)", pAP->alias, id);

    wld_for_eachRad(tRad) {
        if (tRad != pRad) {
            wld_rad_forEachAp(tAP, tRad) {
                mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(tAP);
                if (mxlVapVendorData == NULL || (whm_mxl_utils_isDummyVap(tAP))) {
                    continue;
                }
                if(mxlVapVendorData->mloId == id) {
                    return tAP;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief Validate and update MLO configuration between 2 AP Objects.
 *
 * @param pAPlink1 AccessPoint
 * @param pAplink2 AccessPoint
 * @return return OK if configs match and interface Toggle successful, ERROR otherwise.
 */
swl_rc_ne whm_mxl_createMLVap(T_AccessPoint* pAPlink1, T_AccessPoint* pAPlink2) {
    amxd_object_t* link1VendorObj = amxd_object_get(pAPlink1->pBus, "Vendor");
    amxd_object_t* link1MloObj = amxd_object_get(link1VendorObj, "MLO");
    ASSERT_NOT_NULL(link1MloObj, SWL_RC_INVALID_PARAM, ME, "link1MloObj is NULL");
    amxd_object_t* link2VendorObj = amxd_object_get(pAPlink2->pBus, "Vendor");
    amxd_object_t* link2MloObj = amxd_object_get(link2VendorObj, "MLO");
    ASSERT_NOT_NULL(link2MloObj, SWL_RC_INVALID_PARAM, ME, "link2MloObj is NULL");
    /*compare SSID*/
    T_SSID* link1SSID = (T_SSID*) pAPlink1->pSSID;
    T_SSID* link2SSID = (T_SSID*) pAPlink2->pSSID;
    T_Radio* pRadLink1 = pAPlink1->pRadio;
    T_Radio* pRadLink2 = pAPlink2->pRadio;
    if (!swl_str_matches(link1SSID->SSID, link2SSID->SSID)) {
        SAH_TRACEZ_ERROR(ME, "MLO: SSID Mismatch Between Both the Links");
        return SWL_RC_ERROR;
     }
    /*Compare Security Config*/
    if (pAPlink1->secModeEnabled != pAPlink2->secModeEnabled) {
        // Avoid the check if one link is WPA2-WPA3-Personal for 2G/5G and the other is 6G with WPA3_PERSONAL
        if (!((pAPlink1->secModeEnabled == SWL_SECURITY_APMODE_WPA2_WPA3_P || pAPlink2->secModeEnabled == SWL_SECURITY_APMODE_WPA2_WPA3_P)
             && (wld_rad_is_6ghz(pRadLink1) || wld_rad_is_6ghz(pRadLink2)))) {
            SAH_TRACEZ_ERROR(ME, "MLO: Security Mode Mismatch Between Both the Links");
            return SWL_RC_ERROR;
        }
    }
    char* link1MldMac = amxd_object_get_value(cstring_t, link1MloObj, "ApMldMac", NULL);
    char* link2MldMac = amxd_object_get_value(cstring_t, link2MloObj, "ApMldMac", NULL);
    if (link1MldMac && link2MldMac && (!swl_str_matches(link1MldMac, link2MldMac))) {
        SAH_TRACEZ_ERROR(ME, "MLO: ap_mld_mac Mismatch Between Both the Links");
        free(link1MldMac);
        free(link2MldMac);
        return SWL_RC_ERROR;
    }
    /* set ap_mld_mac with link1 MAC Addr, if not configured explicitly */
    if(swl_str_isEmpty(link1MldMac) || swl_str_isEmpty(link2MldMac)) {
        swl_macChar_t ApMldMacStr;
        SWL_MAC_BIN_TO_CHAR(&ApMldMacStr, link1SSID->BSSID);
        swl_typeCharPtr_commitObjectParam(link1MloObj, "ApMldMac", ApMldMacStr.cMac);
        swl_typeCharPtr_commitObjectParam(link2MloObj, "ApMldMac", ApMldMacStr.cMac);
    }
    free(link1MldMac);
    free(link2MldMac);
    whm_mxl_mlo_restartHapd(pRadLink1);
    return SWL_RC_OK;
}

/**
 * @brief Deinitializes MLO configuration based ML Associated AP Object.
 *
 * @param pAP AccessPoint
 */
void whm_mxl_destroyMLVap(T_AccessPoint* pAP) {
    /* Fetch main link VAP obj details*/
    amxd_object_t* link1VendorObj = amxd_object_get(pAP->pBus, "Vendor");
    amxd_object_t* link1MloObj = amxd_object_get(link1VendorObj, "MLO");
    ASSERT_NOT_NULL(link1MloObj, , ME, "link1MloObj is NULL");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, , ME, "VapVendorData is NULL");
    T_Radio* pRad   = pAP->pRadio;
    /* Fetch Sibling VAP details from pAP object */
    T_AccessPoint* sibAP = whm_mxl_getSiblingBss(pAP, mxlVapVendorData->mloId);
    ASSERT_NOT_NULL(sibAP, , ME, "sibAP is NULL");
    amxd_object_t* link2VendorObj = amxd_object_get(sibAP->pBus, "Vendor");
    amxd_object_t* link2MloObj = amxd_object_get(link2VendorObj, "MLO");
    ASSERT_NOT_NULL(link2MloObj, , ME, "link2MloObj is NULL");
    mxl_VapVendorData_t* mxlSibVapVendorData = mxl_vap_getVapVendorData(sibAP);
    ASSERT_NOT_NULL(mxlSibVapVendorData, , ME, "VapVendorData is NULL");
    /* Set MLO Destroy flag to prevent calling destroyMLVAP for both Links configuration */
    mxlVapVendorData->MLO_destroyInProgress = true;
    mxlSibVapVendorData->MLO_destroyInProgress = true;
    /* Clearing ApMldMac for Both VAPs to maintain consistency between DM and hostap */
    swl_typeCharPtr_commitObjectParam(link1MloObj, "ApMldMac", "");
    swl_typeCharPtr_commitObjectParam(link2MloObj, "ApMldMac", "");
    /* setting mloId -1 to update hostapd conf from VapConfigMap update*/
    mxlVapVendorData->mloId = -1;
    mxlSibVapVendorData->mloId = -1;
    /* clear conf file ML params*/
    whm_mxl_mlo_restartHapd(pRad);
    /* Unset once Deinitialization of ML VAP is completed in hostapd */
    mxlVapVendorData->MLO_destroyInProgress = false;
    mxlSibVapVendorData->MLO_destroyInProgress = false;
}

/**
 * @brief Check if the AP object is part of MLO VAP.
 *
 * @param pAP AccessPoint
 * @return return true if AP object is part of MLO VAP, false otherwise.
 */
bool whm_mxl_isMLAssociatedVap(T_AccessPoint* pAP) {
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(mxlVapVendorData, false, ME, "VapVendorData is NULL");
    if (whm_mxl_getSiblingBss(pAP, mxlVapVendorData->mloId)) {
        return true;
    }
    return false;
}
