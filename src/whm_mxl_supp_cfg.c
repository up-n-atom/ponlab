/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_supp_cfg.c                                    *
*         Description  : Handles WPA Supplicant Configurations                 *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"
#include "swl/map/swl_mapCharFmt.h"

#include "wld/wld_radio.h"
#include "whm_mxl_supp_cfg.h"

#define ME "mxlSupC"

static swl_rc_ne s_whm_mxl_ep_updateGlobalConfig(T_EndPoint* pEP, wld_wpaSupp_config_t* configMap) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "pEP NULL");
    swl_mapChar_t* global = wld_wpaSupp_getGlobalConfig(configMap);
    ASSERTS_NOT_NULL(global, SWL_RC_INVALID_PARAM, ME, "global NULL");
    amxd_object_t* pEpVendorObj = amxd_object_get(pEP->pBus, "Vendor");
    ASSERT_NOT_NULL(pEpVendorObj, SWL_RC_INVALID_PARAM, ME, "pEpVendorObj NULL");
    uint32_t wds             = amxd_object_get_value(uint32_t, pEpVendorObj, "Wds", NULL);
    bool wpsCredAddSae      = amxd_object_get_value(bool, pEpVendorObj, "WpsCredAddSae", NULL);
    char* vendorElems       = amxd_object_get_value(cstring_t, pEpVendorObj, "VendorElements", NULL);

    if(!(swl_str_isEmpty(vendorElems))) {
        swl_mapCharFmt_addValStr(global, "vendor_elems", "%s", vendorElems);
    }
    free(vendorElems);

    MXL_EP_CHECK_GT_SET_NEW_PARAM(wds, 0, global, "wds");
    MXL_EP_CHECK_GT_SET_NEW_PARAM(wpsCredAddSae, 0, global, "wps_cred_add_sae");

    return SWL_RC_OK;
}

static swl_rc_ne s_whm_mxl_ep_updateNetworkConfig(T_EndPoint* pEP, wld_wpaSupp_config_t* configMap) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "pEP is NULL");
    T_EndPointProfile* epProfile = pEP->currentProfile;
    ASSERTS_NOT_NULL(epProfile, SWL_RC_INVALID_PARAM, ME, "epProfile NULL");
    swl_mapChar_t* network = wld_wpaSupp_getNetworkConfig(configMap);
    ASSERTS_NOT_NULL(network, SWL_RC_INVALID_PARAM, ME, "network NULL");
    amxd_object_t* pEpVendorObj = amxd_object_get(pEP->pBus, "Vendor");
    ASSERT_NOT_NULL(pEpVendorObj, SWL_RC_INVALID_PARAM, ME, "pEpVendorObj NULL");
    uint32_t multiApProfile  = amxd_object_get_value(uint32_t, pEpVendorObj, "MultiApProfile", NULL);

    bool ret = swl_str_isEmpty(epProfile->SSID);
    ASSERT_FALSE(ret, SWL_RC_ERROR, ME, "empty SSID");
    /* Do not proceed with network config unless SSID is present */

    if (pEP->multiAPEnable) {
        MXL_EP_CHECK_GT_SET_NEW_PARAM(multiApProfile, 0, network, "multi_ap_profile");
    }

    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_ep_updateConfigMaps(T_EndPoint* pEP, wld_wpaSupp_config_t* configMap) {
    ASSERTS_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "pEP NULL");

    /* Update wpa_supplicant global conf parameters */
    s_whm_mxl_ep_updateGlobalConfig(pEP, configMap);

    /* Update wpa_supplicant network conf parameters */
    s_whm_mxl_ep_updateNetworkConfig(pEP, configMap);

    return SWL_RC_OK;
}
