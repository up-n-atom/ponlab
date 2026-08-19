/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_module.c                                      *
*         Description  : Module initialisation APIs                            *
*                                                                              *
*  *****************************************************************************/

#include <debug/sahtrace.h>

#include "wld/wld.h"
#include "wld/wld_radio.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_rad_nl80211.h"

#include "swla/swla_exec.h"

#include "whm_mxl_utils.h"

#include "whm_mxl_module.h"
#include "whm_mxl_monitor.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_ep.h"
#include "whm_mxl_csi.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_supp_cfg.h"
#include "whm_mxl_parser.h"
#include "whm_mxl_dmnMngr.h"
#include "whm_mxl_fsmLocker.h"
#include "whm_mxl_reconfMngr.h"

#define ME "mxlMod"

#define MXL_MAXNROF_RADIO (4)

static bool s_init = false;
static vendor_t* s_vendor = NULL;
static whm_mxl_module_mode_t mxlModule = {.mode = WHM_MXL_MOD_NORMAL, .wpa3CertMode = false};

static const char* s_defaultIfNames[SWL_FREQ_BAND_MAX] = {"wlan0", "wlan2", "wlan4"};
static const char* s_defaultZwDfsIfName = "wlan6";

static void s_mxl_addRadio(const char* name, int index) {
    if(swl_str_matches(name, s_defaultZwDfsIfName)) {
        // keep it in vendor module, do not add it to global radio list visible to pwhm
        SAH_TRACEZ_WARNING(ME, "Attach ZwDfs interface %s with index %d to vendor %s", name, index, s_vendor->name);
        mxl_rad_addZwDfsRadio(name, s_vendor, index);
    } else {
        SAH_TRACEZ_WARNING(ME, "Attach interface %s with index %d to vendor %s", name, index, s_vendor->name);
        wld_addRadio(name, s_vendor, index);
    }
    // Add background radar capability for 5GHz main radio
    T_Radio* pRadZwDfs = mxl_rad_getZwDfsRadio();
    T_Radio* pRad5GHzData = wld_getRadioByFrequency(SWL_FREQ_BAND_5GHZ);
    if(pRadZwDfs && pRad5GHzData && !pRad5GHzData->bgdfs_config.available) {
        SAH_TRACEZ_WARNING(ME, "%s: Add RADAR_BACKGROUND capability", pRad5GHzData->Name);
        wld_rad_addSuppDrvCap(pRad5GHzData, SWL_FREQ_BAND_5GHZ, "RADAR_BACKGROUND");
        wld_bgdfs_setAvailable(pRad5GHzData, true);
    }
}

static int s_checkAndAddRadios() {
    swl_rc_ne rc = SWL_RC_INVALID_PARAM;
    wld_nl80211_ifaceInfo_t wlIfacesInfo[MXL_MAXNROF_RADIO][MAXNROF_ACCESSPOINT];
    memset(wlIfacesInfo, 0, sizeof(wlIfacesInfo));
    rc = wld_nl80211_getInterfaces(MXL_MAXNROF_RADIO, MAXNROF_ACCESSPOINT, wlIfacesInfo);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail to get nl80211 interfaces");
    uint8_t index = 0;
    for(uint32_t i = 0; i < MXL_MAXNROF_RADIO; i++) {
        wld_nl80211_ifaceInfo_t* pMainIface = &wlIfacesInfo[i][0];
        if(pMainIface->ifIndex <= 0) {
            continue;
        }
        if(wld_rad_get_radio(pMainIface->name) != NULL) {
            index++;
            continue;
        }
        s_mxl_addRadio(pMainIface->name, index);
        index++;
    }
    return index;
}

swl_rc_ne whm_mxl_module_addRadios(void) {
    swl_rc_ne rc = SWL_RC_INVALID_PARAM;
    ASSERT_NOT_NULL(s_vendor, SWL_RC_INVALID_PARAM, ME, "NULL");
    uint8_t index = s_checkAndAddRadios();
    if(index == 0) {
        SAH_TRACEZ_INFO(ME, "NO Wireless interface found, check phy interfaces");
        // initialize nl80211 wiphy info
        uint32_t nrWiphy = 0;
        wld_nl80211_wiphyInfo_t aWiphyIfs[MXL_MAXNROF_RADIO] = {};

        // retrieve all wiphy info
        rc = wld_nl80211_getAllWiphyInfo(wld_nl80211_getSharedState(), MXL_MAXNROF_RADIO, aWiphyIfs, &nrWiphy);
        ASSERT_EQUALS(rc, SWL_RC_OK, rc, ME, "Fail to get all wiphy");

        // create the main interfaces on top of each detected wiphy
        for(uint32_t i = 0; i < nrWiphy; i++) {
            wld_nl80211_wiphyInfo_t* pWiphy = &aWiphyIfs[i];
            SAH_TRACEZ_WARNING(ME, "Add main interface for phy %s(%d)", pWiphy->name, pWiphy->wiphy);

            // retrieve and check the freq band
            swl_freqBand_e freqBand = SWL_FREQ_BAND_MAX;
            if(SWL_BIT_IS_SET(pWiphy->freqBandsMask, SWL_FREQ_BAND_5GHZ)) {
                freqBand = SWL_FREQ_BAND_5GHZ;
            } else if(SWL_BIT_IS_SET(pWiphy->freqBandsMask, SWL_FREQ_BAND_6GHZ)) {
                freqBand = SWL_FREQ_BAND_6GHZ;
            } else if(SWL_BIT_IS_SET(pWiphy->freqBandsMask, SWL_FREQ_BAND_2_4GHZ)) {
                freqBand = SWL_FREQ_BAND_2_4GHZ;
            } else {
                SAH_TRACEZ_WARNING(ME, "Skip phy %s: unsupported FreqBand", pWiphy->name);
                continue;
            }
            SAH_TRACEZ_NOTICE(ME, "%s is %s", pWiphy->name, swl_freqBand_str[freqBand]);

            // initialize vendor nl80211 wiphy info
            mxl_VendorWiphyInfo_t vendorInfo = {};
            vendorInfo.wiphyId = pWiphy->wiphy;

            // retrieve vendor wiphy info
            wld_nl80211_getVendorWiphyInfo(wld_nl80211_getSharedState(), vendorInfo.wiphyId, mxl_parseWiphyInfo, &vendorInfo);

            // set the main interface alias
            const char* alias = "";
            if(vendorInfo.wiphyDfsAntenna) {
                alias = s_defaultZwDfsIfName;
            } else {
                alias = s_defaultIfNames[freqBand];
            }

            // add the main interface
            swl_exec_result_t result;
            memset(&result, 0, sizeof(swl_exec_result_t));
            rc = SWL_EXEC_BUF_EXT(&result, "iw", "phy %s interface add %s type __ap", pWiphy->name, alias);
            if((result.exitInfo.isSignaled) || (result.exitInfo.exitStatus != 0)) {
                rc = SWL_RC_ERROR;
            }
            ASSERT_EQUALS(rc, SWL_RC_OK, rc, ME, "Fail to create main interface %s(%d)", alias, pWiphy->wiphy);
            SAH_TRACEZ_WARNING(ME, "Main interface %s added for phy %s(%d)", alias, pWiphy->name, pWiphy->wiphy);
        }
        index = s_checkAndAddRadios();
    }

    if(index == 0) {
        SAH_TRACEZ_INFO(ME, "NO Wireless interface found");
        return SWL_RC_ERROR;
    }

    return SWL_RC_OK;
}

bool whm_mxl_module_init(void) {
    ASSERT_FALSE(s_init, false, ME, "already initialized");
    SAH_TRACEZ_INFO(ME, "Mxl init");
    const T_CWLD_FUNC_TABLE* nl80211Fta = wld_nl80211_getVendorTable();
    ASSERT_NOT_NULL(nl80211Fta, false, ME, "nl80211 FTA is not initiated");

    T_CWLD_FUNC_TABLE fta;
    memset(&fta, 0, sizeof(T_CWLD_FUNC_TABLE));

    /* copy the same handlers table of default nl80211 implementation. */
    memcpy(&fta, nl80211Fta, sizeof(T_CWLD_FUNC_TABLE));

    /*Misc FTA handlers */
    fta.mfn_wvdr_setDmnExecSettings = whm_mxl_dmnMngr_setDmnExecSettings;

    /* customize handlers table by overwriting some specific API */
    fta.mfn_wrad_create_hook = whm_mxl_rad_createHook;
    fta.mfn_wrad_destroy_hook = whm_mxl_rad_destroyHook;
    fta.mfn_wrad_supports = whm_mxl_rad_supports;
    fta.mfn_wrad_addVapExt = whm_mxl_rad_addVapExt;
    fta.mfn_wrad_delvapif = whm_mxl_rad_delVapIf;
    fta.mfn_wrad_addendpointif = whm_mxl_rad_addEndpointIf;
    fta.mfn_wrad_enable = whm_mxl_rad_enable;
    fta.mfn_wrad_stats = whm_mxl_rad_stats;
    fta.mfn_wrad_antennactrl = whm_mxl_rad_antennaCtrl;
    fta.mfn_wrad_beamforming = whm_mxl_rad_beamforming;
    fta.mfn_wrad_update_mon_stats = whm_mxl_monitor_updateMonStats;
    fta.mfn_wrad_setup_stamon = whm_mxl_monitor_setupStamon;
    fta.mfn_wrad_add_stamon = whm_mxl_monitor_addStaMon;
    fta.mfn_wrad_del_stamon = whm_mxl_monitor_delStamon;
    fta.mfn_wrad_start_scan = whm_mxl_rad_startScan;
    fta.mfn_wifi_supvend_modes = whm_mxl_rad_supvendModesChanged;
    fta.mfn_wrad_sensing_cmd = whm_mxl_rad_sensingCmd;
    fta.mfn_wrad_sensing_addClient = whm_mxl_rad_sensingAddClient;
    fta.mfn_wrad_sensing_delClient = whm_mxl_rad_sensingDelClient;
    fta.mfn_wrad_sensing_csiStats = whm_mxl_rad_sensingCsiStats;
    fta.mfn_wrad_sensing_resetStats = whm_mxl_rad_sensingResetStats;
    fta.mfn_wrad_updateConfigMap = whm_mxl_rad_updateConfigMap;
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
    fta.mfn_wrad_autochannelenable = whm_mxl_rad_autoChannelEnable;
    fta.mfn_wrad_startPltfACS = whm_mxl_rad_startPltfACS;
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
    fta.mfn_wrad_setChanspec = whm_mxl_rad_setChanspec;
    fta.mfn_wrad_radio_status = whm_mxl_rad_status;
    fta.mfn_wrad_regdomain = whm_mxl_rad_regDomain;
    fta.mfn_wrad_supstd = whm_mxl_rad_supstd;
    fta.mfn_wrad_getCurrentTxPow_dBm = whm_mxl_rad_getTxPowerdBm;
    fta.mfn_wrad_getMaxTxPow_dBm = whm_mxl_rad_getMaxTxPowerdBm;

    /* zwdfs fta control */
    fta.mfn_wrad_bgdfs_enable = whm_mxl_rad_bgDfsEnable;
    fta.mfn_wrad_bgdfs_start = whm_mxl_rad_bgDfsStart;
    fta.mfn_wrad_bgdfs_start_ext = whm_mxl_rad_bgDfsStartExt;
    fta.mfn_wrad_bgdfs_stop = whm_mxl_rad_bgDfs_stop;

    fta.mfn_wvap_create_hook = whm_mxl_vap_createHook;
    fta.mfn_wvap_destroy_hook = whm_mxl_vap_destroyHook;
    fta.mfn_wvap_enable = whm_mxl_vap_enable;
    fta.mfn_wvap_ssid = whm_mxl_vap_ssid;
    fta.mfn_wvap_wps_enable = whm_mxl_vap_wps_enable;
    fta.mfn_wvap_bssid = whm_mxl_vap_bssid;
    fta.mfn_wvap_sec_sync = whm_mxl_vap_sec_sync;
    fta.mfn_wvap_get_station_stats = whm_mxl_vap_getStationStats;
    fta.mfn_wvap_get_single_station_stats = whm_mxl_vap_getSingleStationStats;
    fta.mfn_wvap_update_ap_stats = whm_mxl_vap_updateApStats;
    fta.mfn_wvap_updateConfigMap = whm_mxl_vap_updateConfigMap;
    fta.mfn_wvap_multiap_update_type = whm_mxl_vap_multiap_update_type;
    fta.mfn_wvap_clean_sta = whm_mxl_vap_clean_sta;
    fta.mfn_wvap_updated_neighbour = whm_mxl_vap_updated_neighbor;
    fta.mfn_wvap_transfer_sta = whm_mxl_vap_transfer_sta;

    fta.mfn_wendpoint_enable = whm_mxl_ep_enable;
    fta.mfn_wendpoint_create_hook = whm_mxl_ep_createHook;
    fta.mfn_wendpoint_destroy_hook = whm_mxl_ep_destroyHook;
    fta.mfn_wendpoint_stats = whm_mxl_epStats;
    fta.mfn_wendpoint_updateConfigMaps = whm_mxl_ep_updateConfigMaps;

    /* load private vendor datamodel extension and config options*/
    swl_rc_ne ret = wld_vendorModule_parseOdl(ODL_MAIN_FILE);
    ASSERT_EQUALS(ret, SWL_RC_OK, false, ME, "Fail to load vendor odl file");

    /* register private trace zones to pwhm */
    wld_vendorModule_loadPrivTraceZones(amxo_parser_get_config(get_wld_plugin_parser(), "mod-trace-zones"));

    /* register vendor */
    s_vendor = wld_registerVendor(MXL_VENDOR_NAME, &fta);
    ASSERT_NOT_NULL(s_vendor, false, ME, "fail to register vendor %s", MXL_VENDOR_NAME);

    /* share the same generic and native fsm manager, of nl80211 wld implementation */
    wld_fsm_init(s_vendor, (wld_fsmMngr_t*) wld_nl80211_getFsmMngr());
    /* Init generic FSM lock callbacks */
    whm_mxl_extLocker_init((wld_fsmMngr_t*) wld_nl80211_getFsmMngr());
    /* Register to event queues */
    whm_mxl_reconfMngr_initEvents();

    /* init done */
    s_init = true;

    /* detect and attach radios to newly created vendor. */
    ret = whm_mxl_module_addRadios();
    ASSERTW_EQUALS(ret, SWL_RC_OK, true, ME, "No radios added for vendor %s", MXL_VENDOR_NAME);

    return true;
}

bool whm_mxl_module_deInit(void) {
    ASSERT_TRUE(s_init, false, ME, "Not initialized");
    ASSERT_FALSE(wld_isVendorUsed(s_vendor), false, ME, "Still used");
    ASSERT_TRUE(wld_unregisterVendor(s_vendor), false, ME, "unregister failure");
    mxl_rad_deleteZwDfsRadio();
    s_init = false;
    s_vendor = NULL;
    return true;
}

bool whm_mxl_module_loadDefaults(void) {
    ASSERT_TRUE(s_init, false, ME, "Not initialized");
    swl_rc_ne ret = wld_vendorModule_loadOdls(ODL_DEFAULTS_DIR);
    ASSERT_EQUALS(ret, SWL_RC_OK, false, ME, "Fail to load vendor odl defaults");
    return true;
}

bool whm_mxl_isCertModeEnabled(void) {
    return (whm_mxl_getModuleMode() == WHM_MXL_MOD_CERTIFICATION);
}

whm_mxl_module_mode_e whm_mxl_getModuleMode(void) {
    return mxlModule.mode;
}

bool whm_mxl_isWpa3CertModeEnabled(void) {
    return mxlModule.wpa3CertMode;
}

static void s_setCertMode_pwf(void* priv _UNUSED, 
                              amxd_object_t* object _UNUSED, 
                              amxd_param_t* param _UNUSED, 
                              const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Vendor.ModuleMode */
    bool certMode = amxc_var_dyncast(bool, newValue);
    if (certMode) {
        mxlModule.mode = WHM_MXL_MOD_CERTIFICATION;
        SAH_TRACEZ_WARNING(ME, "%s: Certification mode is set", s_vendor->name);
    } else {
        mxlModule.mode = WHM_MXL_MOD_NORMAL;
        SAH_TRACEZ_NOTICE(ME, "%s: Normal operating mode", s_vendor->name);
    }
    SAH_TRACEZ_OUT(ME);
}

static void s_setWPA3CertMode_pwf(void* priv _UNUSED,
                              amxd_object_t* object _UNUSED,
                              amxd_param_t* param _UNUSED,
                              const amxc_var_t* const newValue) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Vendor.ModuleMode */
    bool wpa3Mode = amxc_var_dyncast(bool, newValue);
    if (wpa3Mode) {
        mxlModule.wpa3CertMode = true;
        SAH_TRACEZ_WARNING(ME, "%s: Certification WPA3 mode is set", s_vendor->name);
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sModuleModeDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("CertificationMode", s_setCertMode_pwf),
                  SWLA_DM_PARAM_HDLR("WPA3CertMode", s_setWPA3CertMode_pwf))
              );

void _whm_mxl_module_modeObj_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sModuleModeDmHdlrs, sig_name, data, priv);
}
