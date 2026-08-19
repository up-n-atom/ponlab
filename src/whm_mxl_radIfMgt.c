/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_radIfMgt.c                                    *
*         Description  : Radio interface management related API                *
*                                                                              *
*  *****************************************************************************/

#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include "wld/wld.h"
#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_endpoint.h"
#include "wld/wld_ssid.h"
#include "wld/wld_util.h"
#include "wld/wld_linuxIfUtils.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_linuxIfUtils.h"
#include "wld/wld_wpaCtrlMngr.h"
#include "wld/wld_wpaCtrl_api.h"
#include "swl/swl_common.h"

#include "whm_mxl_cfgActions.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_dmnMngr.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_reconfMngr.h"

#define ME "mxlRadI"
#define MXL_VAP_DELETE_TIMER_RETRIES 40
#define MXL_VAP_DELETE_TIMER_DELAY 250 /* in ms */

static const char* s_defaultEpIfNames[SWL_FREQ_BAND_MAX] = {"wlan1", "wlan3", "wlan5"};

static swl_rc_ne s_createDummyVap(T_Radio* pRad, const char* ssid, int idx) {
    // creating the dummy VAP
    T_AccessPoint* dumAP = wld_ap_create(pRad, pRad->Name, idx);
    ASSERT_NOT_NULL(dumAP, SWL_RC_ERROR, ME, "NULL");
    dumAP->enable = false;
    // creating the dummy SSID
    T_SSID* pSSID = wld_ssid_createApSsid(dumAP);
    ASSERT_NOT_NULL(pSSID, SWL_RC_ERROR, ME, "NULL");
    pSSID->enable = false;
    snprintf(pSSID->SSID, sizeof(pSSID->SSID), "%s", ssid);
    snprintf(pSSID->Name, sizeof(pSSID->Name), "%s", pRad->Name);
    amxc_llist_prepend(&pRad->llAP, &dumAP->it);
    // generic call to add the dummy VAP
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wrad_addVapExt, pRad, dumAP);
    if(rc < SWL_RC_OK) {
        SAH_TRACEZ_ERROR(ME, "%s: fail in add iface for dummy vap(%s)", pRad->Name, dumAP->name);
        wld_ap_destroy(dumAP);
    }
    return rc;
}

int whm_mxl_rad_addVapExt(T_Radio* pRad, T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    SAH_TRACEZ_IN(ME);
    swl_rc_ne rc;
    char ssid[SSID_NAME_LEN] = {0};

    // create dummy vap for ZwDfs radio if radio ctx is already created successfully
    if(pRad->operatingFrequencyBand == SWL_FREQ_BAND_EXT_5GHZ) {
        T_Radio* pRadZwDfs = mxl_rad_getZwDfsRadio();
        if(pRadZwDfs && (wld_rad_countIfaces(pRadZwDfs) == 0)) {
            SAH_TRACEZ_INFO(ME, "%s: creating dummy vap for ZwDfs radio", pRadZwDfs->Name);
            snprintf(ssid, sizeof(ssid), "dummy_ssid_ZWDFS");
            rc = s_createDummyVap(pRadZwDfs, ssid, amxc_llist_size(&pRadZwDfs->llAP));
            if(rc < SWL_RC_OK) {
                SAH_TRACEZ_ERROR(ME, "%s: failed to create ZwDfs dummy vap", pRadZwDfs->Name);
            }
        }
    }

    pRad->isSTASup = false;
    // check if Vap is private, need to shift it first to make it appear in BSS section of hostapd conf file
    if(wld_rad_countIfaces(pRad) == 0) {
        SAH_TRACEZ_INFO(ME, "%s: first vap to be added, shift is needed ", pRad->Name);
        snprintf(ssid, sizeof(ssid), "dummy_ssid_%s", swl_freqBand_str[wld_rad_getFreqBand(pRad)]);
        rc = s_createDummyVap(pRad, ssid, pAP->ref_index);
        if(rc < SWL_RC_OK) {
            goto exit;
        }
    }

    pAP->ref_index = amxc_llist_it_index_of(&pAP->it);
    CALL_NL80211_FTA_RET(rc, mfn_wrad_addVapExt, pRad, pAP);
    if(rc < SWL_RC_OK) {
        SAH_TRACEZ_ERROR(ME, "%s: fail in add iface for vap(%s)", pRad->Name, pAP->name);
    }

exit:
    pRad->isSTASup = amxd_object_get_bool(pRad->pBus, "STASupported_Mode", false);
    SAH_TRACEZ_OUT(ME);
    return rc;
}

static void s_delVapTimerHandler(amxp_timer_t* timer _UNUSED, void* data) {
    T_Radio* pRad = (T_Radio*) data;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
    bool pendingFsmActions = pRad->fsmRad.FSM_SyncAll || areBitsSetLongArray(pRad->fsmRad.FSM_BitActionArray, FSM_BW);
    vendorData->delVapTimerRetries++;

    if((vendorData->delVapTimerRetries >= MXL_VAP_DELETE_TIMER_RETRIES) ||
       ((pRad->pFA->mfn_wrad_fsm_state(pRad) == FSM_IDLE) && !pendingFsmActions)) {
        whm_mxl_rad_delVap_timer_enable(pRad, false);
        whm_mxl_sighupHapd(pRad);
    }
}

void whm_mxl_rad_delVap_timer_init(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
    amxp_timer_new(&vendorData->delVapTimer, s_delVapTimerHandler, pRad);
}

void whm_mxl_rad_delVap_timer_deinit(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");
    amxp_timer_delete(&vendorData->delVapTimer);
}

void whm_mxl_rad_delVap_timer_enable(T_Radio* pRad, bool enable) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* vendorData = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(vendorData, , ME, "NULL");

    amxp_timer_state_t state = amxp_timer_get_state(vendorData->delVapTimer);
    if (state == amxp_timer_running) {
        amxp_timer_stop(vendorData->delVapTimer);
    }

    vendorData->delVapTimerRetries = 0;

    if(enable) {
        amxp_timer_set_interval(vendorData->delVapTimer, MXL_VAP_DELETE_TIMER_DELAY);
        amxp_timer_start(vendorData->delVapTimer, MXL_VAP_DELETE_TIMER_DELAY);
    }
}

static void whm_mxl_restartHapdCb(T_Radio* pRad) {
    whm_mxl_restartHapd(pRad);
}

int whm_mxl_rad_delVapIf(T_Radio* pRad, char* vapName) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    ASSERT_NOT_NULL(vapName, SWL_RC_INVALID_PARAM, ME, "Vap Name is NULL");
    SAH_TRACEZ_IN(ME);
    swl_rc_ne rc;

    CALL_NL80211_FTA_RET(rc, mfn_wrad_delvapif, pRad, vapName);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "fail in generic call");

    if(whm_mxl_dmnMngr_isGlbHapdEnabled()) {
         whm_mxl_rad_delVap_timer_enable(pRad, true);
    }
    else {
         swla_delayExec_add((swla_delayExecFun_cbf) whm_mxl_restartHapdCb, pRad);
    }

    SAH_TRACEZ_OUT(ME);
    return SWL_RC_OK;
}

void whm_mxl_dynamicAddVapSync(T_Radio* pRad, T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    ASSERTI_TRUE(wld_secDmn_isRunning(pRad->hostapd), , ME, "%s: hostapd is not running", pRad->Name);
    ASSERTI_TRUE(whm_mxl_dmnMngr_isGlbHapdEnabled(), , ME, "%s: not single hostapd", pRad->Name);
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERTI_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");

    SAH_TRACEZ_INFO(ME, "New vap %s dynamically created - scheduling sync", pAP->alias);
    whm_mxl_rad_setCtrlSockSyncNeeded(pRad, true);
    whm_mxl_rad_requestSync(pRad);
}

int whm_mxl_rad_addEndpointIf(T_Radio* pRad, char* buf, int bufsize) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(buf, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_TRUE(bufsize > 0, SWL_RC_INVALID_PARAM, ME, "null size");
    T_EndPoint* pEP = wld_rad_getFirstEp(pRad);
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_STATE, ME, "No EP ctx");
    //support only one EP per radio
    ASSERT_FALSE(pEP->index > 0, SWL_RC_ERROR, ME, "%s: already created EP[%s] with ifname(%s) and ifIndex(%d)",
                 pRad->Name, pEP->alias, pEP->Name, pEP->index);

    const char* intfName = s_defaultEpIfNames[pRad->operatingFrequencyBand];
    swl_str_copy(pEP->Name, sizeof(pEP->Name), intfName);
    pEP->index = 0;
    swl_rc_ne rc = wld_rad_nl80211_addEpInterface(pRad, pEP);
    ASSERT_FALSE(rc < SWL_RC_OK, rc, ME, "failed to create EP interface %s", pEP->Name);

    wld_nl80211_setInterfaceUse4Mac(wld_nl80211_getSharedState(), pEP->index, true);

    swl_macBin_t epMacAddr = SWL_MAC_BIN_NEW();
    uint32_t epIndex = wld_rad_countIfaces(pRad);
    wld_ssid_generateMac(pRad, pEP->pSSID, epIndex, &epMacAddr);
    wld_ssid_setMac(pEP->pSSID, &epMacAddr);
    if(wld_linuxIfUtils_setMac(wld_rad_getSocket(pRad), pEP->Name, (swl_macBin_t*) &epMacAddr) < 0) {
        SAH_TRACEZ_ERROR(ME, "%s:fail to set intf mac ["SWL_MAC_FMT "] , error %d (%s)",
                         pEP->Name, SWL_MAC_ARG(epMacAddr.bMac), errno, strerror(errno));
    }

    swl_str_copy(buf, bufsize, intfName);

    /* Return the ep iface index number */
    return pEP->index;
}

/**
 * @brief Adds a network interface to a bridge.
 *
 * This function adds a network interface, specified by its index, to a bridge.
 *
 * @param sock The socket file descriptor.
 * @param ifIndex The index of the network interface to be added.
 * @param brName The name of the bridge to which the interface will be added.
 * @return swl_rc_ne Returns SWL_RC_OK on success, otherwise SWL_RC_ERROR.
 */
static swl_rc_ne s_addIdxToBridge(int sock, int32_t ifIndex, const char* brName) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_ifindex = ifIndex;
    swl_str_copy(ifr.ifr_name, IFNAMSIZ, brName);
    int err = ioctl(sock, SIOCBRADDIF, &ifr);
    ASSERT_TRUE((err == 0), SWL_RC_ERROR, ME, "ioctl addif to bridge err=%d", err);
    return SWL_RC_OK;
}

/**
 * @brief Adds a WDS interface to the bridge of the given Access Point.
 *
 * This function retrieves the LAN bridge associated with the given Access Point (AP)
 * and adds the specified bSTA WDS interface to that bridge.
 *
 * @param pAP Pointer to the Access Point structure.
 * @param wdsIntf Pointer to the WDS interface structure.
 * @return swl_rc_ne Return code indicating the success or failure of the operation.
 *
 * @note The function assumes that the provided pointers are valid and non-NULL.
 *       It uses assertions to validate the input parameters and logs the operation status.
 */
static swl_rc_ne s_addWdsIfaceToBridge(T_AccessPoint* pAP, wld_wds_intf_t* wdsIntf) {
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    swl_rc_ne rc = SWL_RC_OK;
    /* Get lan bridge of the AP context */
    char bridge[IFNAMSIZ] = {0};
    rc = whm_mxl_utils_getIfBridge(pAP->alias, bridge);
    ASSERT_TRUE((rc == SWL_RC_OK), rc, ME, "failed to get bridge for %s", pAP->alias);
    /* Add wds iface to bridge */
    rc = s_addIdxToBridge(wld_rad_getSocket(pRad), wdsIntf->index, bridge);
    SAH_TRACEZ_NOTICE(ME, "%s: wdsIface %s add to bridge %s status %s", pAP->alias,
                        wdsIntf->name, 
                        bridge,
                        (rc == 0) ? "SUCCESS" : "FAILURE");
    return SWL_RC_OK;
}

/**
 * @brief Adds a newly created bSTA WDS interface to the VAP bridge.
 *
 * This function takes an access point and a bSTA WDS interface as parameters,
 * validates them, and then adds the bSTA WDS interface to the VAP bridge.
 *
 * @param pAP Pointer to the access point structure.
 * @param wdsIntf Pointer to the WDS interface structure.
 * @return swl_rc_ne Status code indicating the result of the operation.
 *         - SWL_RC_OK: Operation was successful.
 *         - SWL_RC_INVALID_PARAM: One or more parameters are NULL.
 */
swl_rc_ne whm_mxl_vap_addWdsIfaceEvent(T_AccessPoint* pAP, wld_wds_intf_t* wdsIntf) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    ASSERT_NOT_NULL(wdsIntf, SWL_RC_INVALID_PARAM, ME, "wdsIntf is NULL");
    /* Add the newly created bSTA WDS interface to VAP bridge */
    s_addWdsIfaceToBridge(pAP, wdsIntf);
    return SWL_RC_OK;
}

/**
 * @brief Handles the deletion event of a WDS interface for a given access point.
 *
 * This function is called when a bSTA WDS interface is deleted.
 * It performs necessary checks and logs the deletion event.
 *
 * @param pAP Pointer to the access point structure.
 * @param wdsIntf Pointer to the WDS interface structure.
 * @return swl_rc_ne Returns SWL_RC_OK on successful handling of the event.
 */
swl_rc_ne whm_mxl_vap_delWdsIfaceEvent(T_AccessPoint* pAP, wld_wds_intf_t* wdsIntf) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    ASSERT_NOT_NULL(wdsIntf, SWL_RC_INVALID_PARAM, ME, "wdsIntf is NULL");
    SAH_TRACEZ_NOTICE(ME, "%s: wds iface %s deleted", pAP->alias, wdsIntf->name);
    /* No specific handling for the moment */
    return SWL_RC_OK;
}

static wld_wpaCtrlInterface_t* s_getMainIface(T_Radio* pRad) {
    ASSERTS_NOT_NULL(pRad, NULL, ME, "NULL");
    ASSERTS_NOT_NULL(pRad->hostapd, NULL, ME, "NULL");
    return wld_wpaCtrlMngr_getFirstReadyInterface(pRad->hostapd->wpaCtrlMngr);
}

SWL_TABLE(sHapdStatesMaps,
          ARR(char* hapdStateStr; chanmgt_rad_state radDetState; ),
          ARR(swl_type_charPtr, swl_type_uint32, ),
          ARR({"UNKNOWN", CM_RAD_UNKNOWN},
              {"UNINITIALIZED", CM_RAD_ERROR},
              {"DISABLED", CM_RAD_DOWN},
              {"COUNTRY_UPDATE", CM_RAD_CONFIGURING},
              {"ACS", CM_RAD_CONFIGURING},
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
              {"ACS_DONE", CM_RAD_CONFIGURING},
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
              {"HT_SCAN", CM_RAD_CONFIGURING},
              {"DFS", CM_RAD_FG_CAC},
              {"ENABLED", CM_RAD_UP},
              ));

swl_rc_ne whm_mxl_hapd_getRadState(T_Radio* pRad, chanmgt_rad_state* pDetailedState) {
    ASSERTS_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    wld_wpaCtrlInterface_t* mainIface = s_getMainIface(pRad);
    ASSERTS_NOT_NULL(mainIface, SWL_RC_ERROR, ME, "%s: No main hapd wpactrl iface", pRad->Name);
    ASSERTI_TRUE(wld_wpaCtrlInterface_isReady(mainIface), SWL_RC_ERROR,
                 ME, "%s: main wpactrl iface is not ready", pRad->Name);
    char reply[1024] = {0};
    char state[64] = {0};
    if((!wld_wpaCtrl_sendCmdSynced(mainIface, "STATUS", reply, sizeof(reply) - 1)) ||
       (wld_wpaCtrl_getValueStr(reply, "state", state, sizeof(state)) <= 0)) {
        SAH_TRACEZ_INFO(ME, "%s: status not yet available", pRad->Name);
        return SWL_RC_ERROR;
    }
    chanmgt_rad_state* pRadDetState = (chanmgt_rad_state*) swl_table_getMatchingValue(&sHapdStatesMaps, 1, 0, state);
    ASSERTI_NOT_NULL(pRadDetState, SWL_RC_ERROR, ME, "%s: unknown hapd state(%s)", pRad->Name, state);
    W_SWL_SETPTR(pDetailedState, *pRadDetState);
    SAH_TRACEZ_INFO(ME, "%s: hapd state[%s] -> radDetState[%d]", pRad->Name, state, *pRadDetState);
    return SWL_RC_OK;
}
