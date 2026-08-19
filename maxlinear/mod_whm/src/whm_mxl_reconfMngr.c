/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_reconfFsm.c                                   *
*         Description  : MXL Reconf Manager API                                *
*                                                                              *
*  *****************************************************************************/

#include "wld/wld.h"
#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_ssid.h"
#include "wld/wld_util.h"
#include "wld/wld_hostapd_cfgFile.h"
#include "wld/wld_linuxIfUtils.h"
#include "wld/wld_rad_nl80211.h"
#include "wld/wld_wpaCtrlMngr.h"
#include "wld/wld_wpaCtrl_api.h"
#include "wld/wld_secDmn.h"
#include "wld/wld_hostapd_ap_api.h"
#include "wld/wld_eventing.h"

#include <swl/swl_common.h>

#include "whm_mxl_reconfMngr.h"
#include "whm_mxl_reconfFsm.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_fsmLocker.h"
#include "whm_mxl_utils.h"

#define ME "mxlRcfM"

#define MAX_COMMITS_PENDING             30
#define MAX_WAIT_FROM_FIRST_COMMIT_SEC  30
#define REFRESH_PERIOD_MS               750

typedef struct {
    bool enable;
    uint32_t delay;
    uint32_t bootDelay;
} whm_mxl_reconfCommitCfg_t;

static whm_mxl_reconfCommitCfg_t reconfCommitMngr = {
    .enable = true,
    .delay = 1000,
    .bootDelay = 10000,
};

static bool s_reconfCheckRequirements(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, MXL_RECONF_FSM_REQ_NO_OK, ME, "NULL");
    if (pRad->enable && pRad->isReady && wld_secDmn_isRunning(pRad->hostapd)) {
        return MXL_RECONF_FSM_REQ_OK;
    }
    return MXL_RECONF_FSM_REQ_NO_OK;
}

static void s_rescheduleAction(mxl_VendorData_t* pRadVendor,
                               whm_mxl_reconfMngr_actions_e newAction,
                               bool sameAction) {
    ASSERTS_NOT_NULL(pRadVendor, , ME, "NULL");
    /* Used to reschedule actions during FSM run */
    if (sameAction) {
        setBitLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW, newAction);
        pRadVendor->reconfFsm.FSM_ComPend++;
    } else {
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, newAction);
    }
}

static bool s_modConfFile(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendorData is NULL");
    SAH_TRACEZ_INFO(ME, "%s: Create new config file for hostapd", pRad->Name);
    pRadVendor->reconfFsm.timeout_msec = 100;
    wld_hostapd_cfgFile_createExt(pRad);
    return true;
}

static bool s_reloadBss(T_AccessPoint* pAP, T_Radio* pRad _UNUSED) {
    ASSERT_NOT_NULL(pAP, false, ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: reloading bss", pAP->alias);
    ASSERTI_FALSE(whm_mxl_utils_isDummyVap(pAP), false, ME, "%s: skip reload bss for dummy vap", pAP->alias);
    ASSERTI_TRUE(wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface), true, ME, "%s: wpactrl link disconnected", pAP->alias);
    return wld_wpaCtrl_sendCmdCheckResponse(pAP->wpaCtrlInterface, "RELOAD_BSS", "OK");
}

/**
 * @brief Send RECONF command to hostapd and reconf specific BSS
 *
 * @param T_AccessPoint accesspoint
 * @param T_Radio radio
 * @return bool.
 */
static bool s_reconfBss(T_AccessPoint* pAP, T_Radio* pRad) {
    ASSERT_NOT_NULL(pAP, false, ME, "pAP is NULL");
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    ASSERT_FALSE(whm_mxl_utils_isDummyVap(pAP), false, ME, "%s: reconf on master vap is not allowed", pAP->alias);
    ASSERT_TRUE((s_reconfCheckRequirements(pRad) == MXL_RECONF_FSM_REQ_OK), false, ME, "%s: requirements for bss reconf not met", pAP->alias);
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendorData is NULL");
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(masterVap, false, ME, "masterVap is NULL");
    char cmd[64] = {0};

    /* Prepare reconf command */
    swl_str_catFormat(cmd, sizeof(cmd), "RECONF %s", pAP->alias);

    if (wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface)) {
        SAH_TRACEZ_INFO(ME, "%s: start reconf bss using master vap", pAP->alias);
        wld_wpaCtrl_sendCmdCheckResponseExt(masterVap->wpaCtrlInterface, cmd, "OK", 5000);
    } else {
        wld_wpaCtrlMngr_t* pMgr = wld_secDmn_getWpaCtrlMgr(pRad->hostapd);
        wld_wpaCtrlInterface_t* pIface = wld_wpaCtrlMngr_getDefaultInterface(pMgr);
        ASSERT_NOT_NULL(pIface, false, ME, "no available iface found");
        SAH_TRACEZ_INFO(ME, "%s: start reconf bss", pAP->alias);
        wld_wpaCtrl_sendCmd(pIface, cmd);
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_SYNC_RECONF);
    }

    pRadVendor->reconfFsm.timeout_msec = 1000;
    return true;
}

/**
 * @brief Send BSS_RECONF command to reconf all changed BSS
 *
 * @param T_Radio radio
 * @return bool.
 */
static bool s_reconfAllChangedBss(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendorData is NULL");
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(masterVap, false, ME, "masterVap is NULL");
    if (!wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface)) {
        SAH_TRACEZ_NOTICE(ME, "%s: master vap ctrl iface not ready - opening connetion", pRad->Name);
        bool res = wld_wpaCtrlInterface_open(masterVap->wpaCtrlInterface);
        ASSERT_TRUE(res, false, ME, "%s: failed to open wpaCtrl interface with master vap", pRad->Name);
        wld_wpaCtrlInterface_setEnable(masterVap->wpaCtrlInterface, true);
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_SYNC_RECONF);
    }
    SAH_TRACEZ_INFO(ME, "%s: start reconf on all changed interfaces", pRad->Name);
    wld_wpaCtrl_sendCmdCheckResponseExt(masterVap->wpaCtrlInterface, "BSS_RECONF", "OK", 5000);
    pRadVendor->reconfFsm.timeout_msec = 1000;
    return true;
}

static void s_update6gBeacons(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    T_AccessPoint* pAP = whm_mxl_utils_getFirstEnabledVap(pRad);

    if (pAP) {
        SAH_TRACEZ_INFO(ME, "%s: Update 6G beacons", pAP->alias);
        wld_ap_hostapd_sendCommand(pAP, "UPDATE_BEACON", "update 6G beacon");
    } else {
        T_AccessPoint* masterVap = whm_mxl_utils_getMasterVap(pRad);
        ASSERT_NOT_NULL(masterVap, , ME, "masterVap is NULL");
        SAH_TRACEZ_INFO(ME, "%s: Update 6G beacons using master vap", pRad->Name);
        wld_ap_hostapd_sendCommand(masterVap, "UPDATE_BEACON", "update 6G beacon");
    }
}

static void s_updateColocatedBeacons(void) {
    swl_rc_ne rc;
    chanmgt_rad_state radDetState = CM_RAD_UNKNOWN;
    T_Radio* pOtherRad;

    wld_for_eachRad(pOtherRad) {
        if (pOtherRad && wld_rad_is_6ghz(pOtherRad)) {
            continue;
        }
        radDetState = CM_RAD_UNKNOWN;
        rc = whm_mxl_hapd_getRadState(pOtherRad, &radDetState);
        if ((rc == SWL_RC_OK) && (radDetState == CM_RAD_UP)) {
            T_AccessPoint* pAP;
            wld_rad_forEachAp(pAP, pOtherRad) {
                /* Skip dummy VAP */
                if (whm_mxl_utils_isDummyVap(pAP)) {
                    continue;
                }
                if (pAP && pAP->enable && wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface)) {
                    wld_ap_hostapd_sendCommand(pAP, "UPDATE_BEACON", "update beacon");
                }
            }
        }
    }
}

static bool s_syncReconf(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendorData is NULL");
    SAH_TRACEZ_INFO(ME, "%s: reconf sync", pRad->Name);

    T_AccessPoint* pAP = NULL;
    wld_rad_forEachAp(pAP, pRad) {
        wld_nl80211_ifaceInfo_t ifaceInfo;
        int32_t curIfIdx = 0;
        if (pAP && !whm_mxl_utils_isDummyVap(pAP)) {
            swl_str_copy(ifaceInfo.name, sizeof(ifaceInfo.name), pAP->alias);
            wld_linuxIfUtils_getIfIndex(wld_rad_getSocket(pRad), pAP->alias, &curIfIdx);
            if ((curIfIdx > 0) && (pAP->index != curIfIdx) && (pAP->index > 0)) {
                if (wld_nl80211_getInterfaceInfo(wld_nl80211_getSharedState(), curIfIdx, &ifaceInfo) >= SWL_RC_OK) {
                    wld_vap_setNetdevIndex(pAP, ifaceInfo.ifIndex);
                    pAP->wDevId = ifaceInfo.wDevId;
                }
            }
            if (!wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface)) {
                SAH_TRACEZ_NOTICE(ME, "%s: reconnecting to %s wpa ctrl socket after reconf", pRad->Name, pAP->alias);
                wld_wpaCtrlInterface_setEnable(pAP->wpaCtrlInterface, true);
                wld_wpaCtrlInterface_open(pAP->wpaCtrlInterface);
                /* Update state becuase we might have missed events while socket was disconnected */
                wld_vap_updateState(pAP);
            }
        }
    }

    /* Update 6G co-located beacons after Reconf */
    if (wld_rad_is_6ghz(pRad)) {
        /* Update 6G beacons after Reconf */
        s_update6gBeacons(pRad);
        /* Update 6G co-located beacons after Reconf */
        s_updateColocatedBeacons();
    }

    pRadVendor->reconfFsm.timeout_msec = 100;
    return true;
}

static swl_rc_ne s_checkCtrlIfaces(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");
    T_AccessPoint* pAP = NULL;

    wld_rad_forEachAp(pAP, pRad) {
        if (pAP) {
            if (!wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface)) {
                SAH_TRACEZ_NOTICE(ME, "%s: restarting wpa ctrl manager %s is not ready", pRad->Name, pAP->alias);
                wld_wpaCtrlMngr_disconnect(wld_secDmn_getWpaCtrlMgr(pRad->hostapd));
                wld_wpaCtrlMngr_connect(wld_secDmn_getWpaCtrlMgr(pRad->hostapd));
                return SWL_RC_CONTINUE;
            }
        }
    }
    return SWL_RC_OK;
}

static bool s_sync(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, false, ME, "pRadVendorData is NULL");
    SAH_TRACEZ_ERROR(ME, "%s: general sync", pRad->Name);
    bool updateState = true;

    /* First set timeout for sync action - overwrite later if needed */
    pRadVendor->reconfFsm.timeout_msec = 100;

    if (whm_mxl_rad_isCtrlSockSyncNeeded(pRad)) {
        whm_mxl_rad_setCtrlSockSyncNeeded(pRad, false);
        if (wld_secDmn_isRunning(pRad->hostapd)) {
            if (s_checkCtrlIfaces(pRad) == SWL_RC_CONTINUE) {
                pRadVendor->reconfFsm.timeout_msec = 1000;
                /* Schedule another sync cycle to refresh radio state after wpa ctrl is established */
                s_rescheduleAction(pRadVendor, RECONF_FSM_SYNC, true);
                updateState = false;
            }
        } else {
            SAH_TRACEZ_NOTICE(ME, "%s: wpa ctrl sync requested but hostapd is not running", pRad->Name);
            updateState = false;
        }
    }
    /* Update radio state */
    if (updateState) {
        wld_rad_updateState(pRad, false);
    }
    return true;
}

static void s_checkPreRadDependency(T_Radio* pRad) {
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendorData is NULL");

    /* If RECONF action is requested from fsm - check pre dependency */
    if (isBitSetLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF)) {
        pRadVendor->reconfFsm.FSM_Error = s_reconfCheckRequirements(pRad);
    }

    /* Add other pre-dependencies check for other possible requested actions from the fsm */
}

static void s_checkRadDependency(T_Radio* pRad) {
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendorData is NULL");

    /* Prepare required execution bits for the Reconf sequence
     * Reconf sequence is as follows:
     *      1. Prepare new configuration file for hostapd.
     *      2. Call reconf via the radio ctrl interface.
     *      3. Sync AP index, wdev index and reconnect to all the re-created wpa sockets.
     */
    if (isBitSetLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF)) {
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_MOD_HAPD_CONF_FILE); 
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_SYNC_RECONF);
    }

    /* Add other required sequence bits for any possible request from the fsm */
}

static void s_checkVapDependency(T_AccessPoint* pAP, T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendorData is NULL");
    mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendor, , ME, "pVapVendor is NULL");

    if (isBitSetLongArray(pVapVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF_BSS)) {
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_MOD_HAPD_CONF_FILE); 
        setBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, RECONF_FSM_SYNC_RECONF);
    }    
}

static bool s_tryRadioLock(T_Radio* pRad) {
    return whm_mxl_reconfFsmLock(pRad);
}

static void s_doRadioUnlock(T_Radio* pRad) {
    whm_mxl_reconfFsmUnLock(pRad);
}

static void s_doRadioEnsureLock(T_Radio* pRad) {
    whm_mxl_reconfFsmEnsureLock(pRad);
}

wld_fsmMngr_action_t actions[RECONF_FSM_MAX] = {
    {FSM_ACTION(RECONF_FSM_MOD_HAPD_CONF_FILE),     .doRadFsmAction = s_modConfFile},
	{FSM_ACTION(RECONF_FSM_RELOAD_BSS),             .doVapFsmAction = s_reloadBss},
    {FSM_ACTION(RECONF_FSM_DO_RECONF_BSS),          .doVapFsmAction = s_reconfBss},
    {FSM_ACTION(RECONF_FSM_DO_RECONF),              .doRadFsmAction = s_reconfAllChangedBss},
    {FSM_ACTION(RECONF_FSM_SYNC_RECONF),            .doRadFsmAction = s_syncReconf},
    {FSM_ACTION(RECONF_FSM_SYNC),                   .doRadFsmAction = s_sync},
};

wld_fsmMngr_t reconfMngr = {
    .doLock = s_tryRadioLock,
    .doUnlock = s_doRadioUnlock,
    .ensureLock = s_doRadioEnsureLock,
    .checkPreDependency = s_checkPreRadDependency,
    .checkRadDependency = s_checkRadDependency,
    .checkVapDependency = s_checkVapDependency,
    .actionList = actions,
    .nrFsmBits = RECONF_FSM_MAX,
};

wld_fsmMngr_t* whm_mxl_get_reconfMngr(void) {
    return (wld_fsmMngr_t*)&reconfMngr;
}

int whm_mxl_reconfMngr_doCommit(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, -1, ME, "NULL");
    int res = 0;
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, -1, ME, "pRadVendor is NULL");

    if ((pRadVendor->reconfFsmBriefState == MXL_RECONF_FSM_IDLE) && (!pRadVendor->reconfFsm.timer)) {
        res = whm_mxl_reconf_fsm(pRad);
    } else {
        /* We cannot run reconf FSM - so mark commit is pending */
        pRadVendor->reconfFsm.FSM_ComPend++;
        swl_timeMono_t now = swl_time_getMonoSec();
        if (pRadVendor->reconfFsm.FSM_ComPend == 0) {
            pRadVendor->reconfFsm.FSM_ComPend_Start = now;
        }
        time_t diff = now - pRadVendor->reconfFsm.FSM_ComPend_Start;
        SAH_TRACEZ_INFO(ME, "%s: Commits are pending %d", pRad->Name, pRadVendor->reconfFsm.FSM_ComPend);
        res = 0;
        if ((pRadVendor->reconfFsm.FSM_ComPend > MAX_COMMITS_PENDING) && (diff > MAX_WAIT_FROM_FIRST_COMMIT_SEC)) {
            SAH_TRACEZ_WARNING(ME, "%s: CANT COMMIT AFTER MANY ATTEMPTS - Forcing FSM reset in all radios", pRad->Name);
            whm_mxl_reconFsm_allRadioReset();
            res = -1;
        }
    }
    return res;
}

static void s_reconfCommit_th(amxp_timer_t* timer _UNUSED, void* userdata) {
    T_Radio* pRad = (T_Radio*) userdata;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    whm_mxl_reconfMngr_doCommit(pRad);
}

void whm_mxl_reconfMngr_init(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    pRadVendor->reconfFsmMngr = whm_mxl_get_reconfMngr();
    ASSERT_NOT_NULL(pRadVendor->reconfFsmMngr, , ME, "reconfFsmMngr is NULL");
    amxp_timer_new(&pRadVendor->commitTimer, s_reconfCommit_th, pRad);
    whm_mxl_reconfFsm_init(pRad);
    SAH_TRACEZ_NOTICE(ME, "%s: Reconf manager successfuly initailized", pRad->Name);
}

void whm_mxl_reconfMngr_deinit(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    if (pRadVendor->reconfFsm.timer) {
        amxp_timer_delete(&pRadVendor->reconfFsm.timer);
    }
    pRadVendor->reconfFsm.timer = NULL;
    pRadVendor->reconfFsmMngr = NULL;
    amxp_timer_delete(&pRadVendor->commitTimer);
    pRadVendor->commitTimer = NULL;
    SAH_TRACEZ_NOTICE(ME, "%s: Reconf manager successfuly de-initailized", pRad->Name);
}

static void s_apChangeEventCb(wld_vap_changeEvent_t* event) {
    ASSERT_NOT_NULL(event, , ME, "NULL");
    T_AccessPoint* pAP = event->vap;
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    SAH_TRACEZ_INFO(ME, "%s: reconf mngr receiving vap change event %d", pAP->alias, event->changeType);
    if (event->changeType == WLD_VAP_CHANGE_EVENT_CREATE_FINAL) {
        /* New vap was added dynamically */
        if (wld_secDmn_isRunning(pRad->hostapd) && wld_rad_firstCommitFinished(pRad)) {
            whm_mxl_dynamicAddVapSync(pRad, pAP);
        }
    }
}

static wld_event_callback_t s_apChangeCbEvt = {
    .callback = (wld_event_callback_fun) s_apChangeEventCb,
};

static void s_stationChangeEventCb(wld_ad_changeEvent_t* event) {
    ASSERT_NOT_NULL(event, , ME, "NULL");
    T_AccessPoint* pAP = event->vap;
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    T_AssociatedDevice* pAD = event->ad;
    SAH_TRACEZ_INFO(ME, "%s: reconf mngr receiving ad change event %d", pAP->alias, event->changeType);
    if (event->changeType == WLD_AD_CHANGE_EVENT_CREATE) {
        pAD->onActionReadCtx.minRefreshPeriodMs = REFRESH_PERIOD_MS;
    }
}

static wld_event_callback_t s_stationChangeCbEvt = {
    .callback = (wld_event_callback_fun) s_stationChangeEventCb,
};

void whm_mxl_reconfMngr_initEvents(void) {
    wld_event_add_callback(gWld_queue_vap_onChangeEvent, &s_apChangeCbEvt);
    wld_event_add_callback(gWld_queue_sta_onChangeEvent, &s_stationChangeCbEvt);
}

static void s_startReconfCommit(T_Radio* pRad) {
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    swl_timeSpecMono_t time;
    swl_timespec_getMono(&time);
    swl_timeSpecMono_t* initTime = wld_getInitTime();

    ASSERTI_TRUE(reconfCommitMngr.enable, , ME, "Reconf commit manager is disabled");
    ASSERT_NOT_NULL(pRadVendor->commitTimer, , ME, "commitTimer is NULL");
    amxp_timer_state_t state = pRadVendor->commitTimer->state;

    ASSERTI_TRUE(state != amxp_timer_running && state != amxp_timer_started, , ME, "%s: commit timer already started", pRad->Name);
    ASSERTI_TRUE(pRadVendor->reconfFsm.FSM_ComPend == 0, , ME, "%s: Already have pending commits (%d)", pRad->Name, pRadVendor->reconfFsm.FSM_ComPend);

    int64_t mSecSinceInit = swl_timespec_diffToMillisec(initTime, &time);
    uint32_t minDelay = 0;
    if ((mSecSinceInit > 0 ) && (mSecSinceInit < reconfCommitMngr.bootDelay)) {
        minDelay = reconfCommitMngr.bootDelay - (uint32_t) mSecSinceInit;
    }
    uint32_t finalDelay = SWL_MAX(reconfCommitMngr.delay, minDelay);
    SAH_TRACEZ_INFO(ME, "%s: Starting reconf commit timer with delay %u", pRad->Name, finalDelay);
    amxp_timer_start(pRadVendor->commitTimer, finalDelay);
}

/**
 * @brief Notify Reconf commit manager that there is a change to work on
 * 
 * @param pRad Pointer to Radio context
 * @return Void
 */
void whm_mxl_reconfMngr_notifyCommit(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    bool pending = areBitsSetLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW);
    ASSERTI_TRUE(pending, , ME, "%s: Request array is not set", pRad->Name);
    s_startReconfCommit(pRad);
}

/**
 * @brief Notify Reconf commit manager that there is a change in VAP to work on
 * 
 * @param pRad Pointer to AP context
 * @return Void
 */
void whm_mxl_reconfMngr_notifyVapCommit(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, , ME, "NULL");
    ASSERT_NOT_NULL(pAP->pRadio, , ME, "pRadio is NULL");
    mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
    ASSERT_NOT_NULL(pVapVendor, , ME, "pVapVendor is NULL");
    bool pending = areBitsSetLongArray(pVapVendor->reconfFsm.FSM_BitActionArray, FSM_BW);
    ASSERTI_TRUE(pending, , ME, "%s: Request array is not set", pAP->alias);
    s_startReconfCommit(pAP->pRadio);
}

static void s_reconfMngrCommitEnable_pwf(void* priv _UNUSED, amxd_object_t* object _UNUSED,
                                 amxd_param_t* param _UNUSED, const amxc_var_t* const newValue) {
    bool enable = amxc_var_dyncast(bool, newValue);
    SAH_TRACEZ_INFO(ME, "Reconf Mngr enable set from %u to %u", reconfCommitMngr.enable, enable);
    reconfCommitMngr.enable = enable;
}

static void s_reconfMngrDelay_pwf(void* priv _UNUSED, amxd_object_t* object _UNUSED,
                                amxd_param_t* param _UNUSED, const amxc_var_t* const newValue) {
    uint32_t delay = amxc_var_dyncast(uint32_t, newValue);
    SAH_TRACEZ_INFO(ME, "Reconf Mngr delay set from %u to %u", reconfCommitMngr.delay, delay);
    reconfCommitMngr.delay = delay;
}

static void s_reconfMngrBootDelay_pwf(void* priv _UNUSED, amxd_object_t* object _UNUSED,
                                    amxd_param_t* param _UNUSED, const amxc_var_t* const newValue) {
    uint32_t bootDelay = amxc_var_dyncast(uint32_t, newValue);
    SAH_TRACEZ_INFO(ME, "Reconf Mngr boot delay set from %u to %u", reconfCommitMngr.bootDelay, bootDelay);
    reconfCommitMngr.bootDelay = bootDelay;
}

SWLA_DM_HDLRS(sReconfMngrMgrDmHdlrs,
              ARR(SWLA_DM_PARAM_HDLR("CommitEnable", s_reconfMngrCommitEnable_pwf),
                  SWLA_DM_PARAM_HDLR("CommitDelay", s_reconfMngrDelay_pwf),
                  SWLA_DM_PARAM_HDLR("BootBlockDelay", s_reconfMngrBootDelay_pwf)));

void _whm_mxl_reconfMngr_configure_ocf(const char* const sig_name,
                                       const amxc_var_t* const data,
                                       void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sReconfMngrMgrDmHdlrs, sig_name, data, priv);
}
