/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_dmnMngr.c                                     *
*         Description  : Vendor Daemon Instance Manager                        *
*                                                                              *
*  *****************************************************************************/

#include <swl/swl_common.h>

#include "wld/wld.h"
#include "wld/wld_radio.h"
#include "wld/wld_endpoint.h"
#include "wld/wld_daemon.h"
#include "wld/wld_secDmnGrp.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_util.h"
#include "wld/Utils/wld_dmnMgt.h"

#include "whm_mxl_module.h"
#include "whm_mxl_zwdfs.h"
#include "whm_mxl_cfgActions.h"
#include "whm_mxl_dmnMngr.h"

#define ME "mxlDmgr"

const char* cstr_DMN_DEBUG_LEVEL[] = {"Info", "Debug", "Msgdump", "Excessive", "Default", 0};
const char* cstr_DMN_LOG_OUTPUT[] = {"Stdout", "Syslog", 0};

static void s_initHapdStartArgs(char* startArgs) {
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_HOSTAPD);
    if (!pDmnCtx) {
        swl_str_copy(startArgs, sizeof(startArgs), HOSTAPD_DEFAULT_START_ARGS);
        SAH_TRACEZ_WARNING(ME, "HOSTAPD: No dmn ctx found, fallback to default starting args");
        return;
    }
    switch(pDmnCtx->dmnExecutionSettings.logDebugLevel) {
        case DMN_DEBUG_LEVEL_INFO:
            swl_str_copy(startArgs, sizeof(startArgs), "-t");
            break;
        case DMN_DEBUG_LEVEL_DEBUG:
            swl_str_copy(startArgs, sizeof(startArgs), "-dt");
            break;
        case DMN_DEBUG_LEVEL_EXCESSIVE:
            swl_str_copy(startArgs, sizeof(startArgs), "-dddt");
            break;
        case DMN_DEBUG_LEVEL_MSGDUMP:
        case DMN_DEBUG_LEVEL_DEFAULT:
        default:
            swl_str_copy(startArgs, sizeof(startArgs), HOSTAPD_DEFAULT_START_ARGS);
            break;
    }
    if(pDmnCtx->dmnExecutionSettings.logOutputPath == DMN_OUTPUT_SYSLOG) {
        swl_strlst_cat(startArgs, sizeof(startArgs), "", "s");
    }
}

static char* s_getGlobHapdArgsCb(wld_secDmnGrp_t* pSecDmnGrp, void* userData _UNUSED, const wld_process_t* pProc _UNUSED) {
    char* args = NULL;
    ASSERT_NOT_NULL(pSecDmnGrp, args, ME, "NULL");
    char startArgs[256] = {0};
    /* Prepare daemon starting args */
    s_initHapdStartArgs(startArgs);
    for(uint32_t i = 0; i < wld_secDmnGrp_getMembersCount(pSecDmnGrp); i++) {
        const wld_secDmn_t* pSecDmn = wld_secDmnGrp_getMemberByPos(pSecDmnGrp, i);
        if((pSecDmn == NULL) || (swl_str_isEmpty(pSecDmn->cfgFile))) {
            continue;
        }
        //concat all radio ifaces conf files
        swl_strlst_cat(startArgs, sizeof(startArgs), " ", pSecDmn->cfgFile);
    }
    SAH_TRACEZ_INFO(ME, "Global HOSTAPD startingArgs=%s", startArgs);
    swl_str_copyMalloc(&args, startArgs);
    return args;
}

static bool s_isHapdIfaceStartable(wld_secDmnGrp_t* pSecDmnGrp _UNUSED, void* userData _UNUSED, wld_secDmn_t* pSecDmn) {
    ASSERT_NOT_NULL(pSecDmn, false, ME, "NULL");
    T_Radio* pRad = (T_Radio*) pSecDmn->userData;
    ASSERT_TRUE(debugIsRadPointer(pRad), false, ME, "INVALID");
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    ASSERT_NOT_NULL(pRad->hostapd, false, ME, "pRad->hostapd is NULL");
    return (amxd_object_get_bool(pRad->pBus, "Enable", NULL) && wld_rad_hasEnabledVap(pRad));
}

static wld_secDmnGrp_EvtHandlers_t sGlbHapdEvtCbs = {
    .getArgsCb = s_getGlobHapdArgsCb,
    .isMemberStartableCb = s_isHapdIfaceStartable,
};

static bool s_isGlbHapdEnabled() {
    /* Global hostapd status is taken from generic dmn context */
    wld_dmnMgt_dmnCtx_t* genDmnCntx = (wld_dmnMgt_dmnCtx_t*)wld_dmnMgt_getDmnCtx(MXL_HOSTAPD);
    ASSERTS_NOT_NULL(genDmnCntx, false, ME, "%s: genDmnCntx is NULL, fallback to default secDmn starting args", pRad->Name);
    vendor_t* mxlVendor = wld_getVendorByName(MXL_VENDOR_NAME);
    ASSERTS_NOT_NULL(mxlVendor, false, ME, "mxlVendor is NULL");
    wld_dmnMgt_dmnExecInfo_t* gHapd = mxlVendor->globalHostapd;
    ASSERTS_NOT_NULL(gHapd, false, ME, "No glob hapd ctx");
    return ((genDmnCntx->exec.useGlobalInstance == SWL_TRL_TRUE) || ((genDmnCntx->exec.useGlobalInstance == SWL_TRL_AUTO) && gHapd->globalDmnRequired));
}

static void s_initMultipleHapdArgs(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    char confFilePath[128] = {0};
    char startArgs[128] = {0};
    swl_str_catFormat(confFilePath, sizeof(confFilePath), HOSTAPD_CONF_FILE_PATH_FORMAT, pRad->Name);
    s_initHapdStartArgs(startArgs);
    swl_strlst_cat(startArgs, sizeof(startArgs), " ", confFilePath);
    ASSERTS_NOT_NULL(pRad->hostapd, , ME, "%s: hostapd not initialized yet", pRad->Name);
    ASSERTS_NOT_NULL(pRad->hostapd->selfDmnProcess, , ME, "secDmn self process is NULL");
    SAH_TRACEZ_INFO(ME, "%s: HOSTAPD startingArgs=%s", pRad->Name, startArgs);
    wld_dmn_setArgList(pRad->hostapd->selfDmnProcess, startArgs);
}

static void s_setHapdDmnStartArgs(vendor_t* pVdr) {
    ASSERT_NOT_NULL(pVdr, , ME, "pVdr is NULL");
    bool enableGlobHapd = s_isGlbHapdEnabled();
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_HOSTAPD);
    ASSERT_NOT_NULL(pDmnCtx, , ME, "pDmnCtx is NULL");
    whm_mxl_dmnMngr_setDmnCtxGlbHpd(pDmnCtx, enableGlobHapd);
    ASSERTS_TRUE((whm_mxl_dmnMngr_getDmnCtxState(pDmnCtx) == MXL_SECDMN_STATE_RST), ,ME ,"Hostapd args already initialized");
    if (!enableGlobHapd) {
        T_Radio* pRad;
        wld_for_eachRad(pRad) {
            if (pRad && pRad->pBus) {
                s_initMultipleHapdArgs(pRad);
            }
        }
        /* Set ZWDFS sec daemon starting args */
        T_Radio* zwdfsRadio = mxl_rad_getZwDfsRadio();
        ASSERT_NOT_NULL(zwdfsRadio, , ME, "zwdfsRadio is NULL");
        s_initMultipleHapdArgs(zwdfsRadio);
    } else {
        wld_dmnMgt_dmnExecInfo_t* gHapd = pVdr->globalHostapd;
        ASSERTS_NOT_NULL(gHapd, , ME, "No glob hapd ctx");
        if(wld_secDmnGrp_isEnabled(gHapd->pGlobalDmnGrp) != enableGlobHapd) {
            wld_secDmnGrp_setEvtHandlers(gHapd->pGlobalDmnGrp, &sGlbHapdEvtCbs, pVdr);
        }
    }
    whm_mxl_dmnMngr_setDmnCtxState(pDmnCtx, MXL_SECDMN_STATE_READY);
}

static void s_whm_mxl_restartAllSupplicants() {
    T_Radio* pRad;
    T_EndPoint* pEP;
    wld_for_eachRad(pRad) {
        if (pRad) {
            wld_rad_forEachEp(pEP, pRad) {
                if (pEP && pEP->enable) {
                    wld_secDmn_restart(pEP->wpaSupp);
                }
            }
        }
    }
}

swl_rc_ne whm_mxl_dmnMngr_setDmnExecSettings(vendor_t* pVdr, const char* dmnName, wld_dmnMgt_dmnExecSettings_t* pCfg) {
    ASSERT_NOT_NULL(pVdr, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_STR(dmnName, SWL_RC_INVALID_PARAM, ME, "Empty");
    swl_rc_ne rc;
    CALL_NL80211_FTA_RET(rc, mfn_wvdr_setDmnExecSettings, pVdr, dmnName, pCfg);
    return rc;
}

static void s_setDmnCtxDefaults(mxl_dmnMngrCtx_t* pDmnCtx) {
    ASSERT_NOT_NULL(pDmnCtx, , ME, "pDmnCtx is NULL");
    pDmnCtx->isGlbDmn = false;
    pDmnCtx->state = MXL_SECDMN_STATE_RST;
    pDmnCtx->initPending = true;
    pDmnCtx->dmnExecutionSettings.logOutputPath = DMN_OUTPUT_STDOUT;
    pDmnCtx->dmnExecutionSettings.logDebugLevel = DMN_DEBUG_LEVEL_MSGDUMP;
    pDmnCtx->dmnExecutionSettings.wpaSupplicantMasterMode = false;
}

static void s_addDmnInst_oaf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const initialParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    char* name = amxd_object_get_cstring_t(object, "Name", NULL);
    ASSERT_STR(name, , ME, "daemon name is empty");
    mxl_dmnMngrCtx_t* pDmnCtx = calloc(1, sizeof(*pDmnCtx));
    if(pDmnCtx == NULL) {
        SAH_TRACEZ_ERROR(ME, "fail to alloc daemon ctx");
        free(name);
        return;
    }
    pDmnCtx->name = name;
    object->priv = pDmnCtx;
    pDmnCtx->object = object;
    s_setDmnCtxDefaults(pDmnCtx);
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sDmnDmHdlrs, ARR(), .instAddedCb = s_addDmnInst_oaf, );

void _whm_mxl_dmnMngr_setDmnEntry_ocf(const char* const sig_name,
                                    const amxc_var_t* const data,
                                    void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sDmnDmHdlrs, sig_name, data, priv);
}

amxd_status_t _whm_mxl_dmnMngr_delDmnEntry_odf(amxd_object_t* object,
                                                amxd_param_t* param,
                                                amxd_action_t reason,
                                                const amxc_var_t* const args,
                                                amxc_var_t* const retval,
                                                void* priv) {
    SAH_TRACEZ_IN(ME);
    amxd_status_t status = amxd_action_object_destroy(object, param, reason, args, retval, priv);
    ASSERT_EQUALS(status, amxd_status_ok, status, ME, "Fail to destroy obj instance st:%d", status);
    ASSERTS_EQUALS(amxd_object_get_type(object), amxd_object_instance, status, ME, "obj is not instance");
    const char* name = amxd_object_get_name(object, AMXD_OBJECT_NAMED);
    SAH_TRACEZ_INFO(ME, "%s: destroy instance object(%p)", name, object);
    mxl_dmnMngrCtx_t* pDmnCtx = (mxl_dmnMngrCtx_t*) object->priv;
    if(pDmnCtx != NULL) {
        object->priv = NULL;
        pDmnCtx->object = NULL;
        W_SWL_FREE(pDmnCtx->name);
        free(pDmnCtx);
    }
    SAH_TRACEZ_OUT(ME);
    return status;
}

mxl_dmnMngrCtx_t* whm_mxl_dmnMngr_getDmnCtx(const char* dmnName) {
    ASSERT_STR(dmnName, NULL, ME, "Empty");
    amxd_object_t* dmnObj = amxd_object_findf(get_wld_object(), "Vendor.Daemon.[Name == '%s']", dmnName);
    ASSERT_NOT_NULL(dmnObj, NULL, ME, "No daemon with name %s", dmnName);
    return dmnObj->priv;
}


static void s_setDmnExecOptsObj_ocf(void* priv _UNUSED, amxd_object_t* object, const amxc_var_t* const newParamValues _UNUSED) {
    SAH_TRACEZ_IN(ME);
    /* WiFi.Vendor.Daemon.{}.ExecOptions */
    amxd_object_t* pDmnObj = amxd_object_get_parent(object);
    ASSERT_NOT_NULL(pDmnObj, , ME, "pDmnObj is NULL");
    mxl_dmnMngrCtx_t* pDmnCtx = (mxl_dmnMngrCtx_t*) pDmnObj->priv;
    ASSERT_NOT_NULL(pDmnCtx, , ME, "pDmnCtx is NULL");
    bool suppMasterModeChanged = false;

    amxc_var_for_each(newValue, newParamValues) {
        char* valStr = NULL;
        bool newVal = false;
        const char* pname = amxc_var_key(newValue);
        if(swl_str_matches(pname, "LogOutputPath")) {
            valStr = amxc_var_dyncast(cstring_t, newValue);
            pDmnCtx->dmnExecutionSettings.logOutputPath = swl_conv_charToEnum(valStr, cstr_DMN_LOG_OUTPUT, DMN_OUTPUT_MAX, DMN_OUTPUT_STDOUT);
        } else if(swl_str_matches(pname, "LogDebugLevel")) {
            valStr = amxc_var_dyncast(cstring_t, newValue);
            pDmnCtx->dmnExecutionSettings.logDebugLevel = swl_conv_charToEnum(valStr, cstr_DMN_DEBUG_LEVEL, DMN_DEBUG_LEVEL_MAX, DMN_DEBUG_LEVEL_DEFAULT);
        } else if(swl_str_matches(pDmnCtx->name, MXL_WPASUPPLICANT) && swl_str_matches(pname, "SupplicantMasterMode")) {
            newVal = amxc_var_dyncast(bool, newValue);
            if (pDmnCtx->dmnExecutionSettings.wpaSupplicantMasterMode != newVal) {
                pDmnCtx->dmnExecutionSettings.wpaSupplicantMasterMode = newVal;
                suppMasterModeChanged = true;
            }
        } else {
            continue;
        }
        free(valStr);

    }

    SAH_TRACEZ_INFO(ME, "Dmn execution options set for dmn:%s logOutputPath=%d, logDebugLevel=%d, wpaSupplicantMasterMode=%d",
                                                        pDmnCtx->name,
                                                        pDmnCtx->dmnExecutionSettings.logOutputPath,
                                                        pDmnCtx->dmnExecutionSettings.logDebugLevel,
                                                        pDmnCtx->dmnExecutionSettings.wpaSupplicantMasterMode);

    vendor_t* pVendor = wld_getVendorByName(MXL_VENDOR_NAME);
    ASSERT_NOT_NULL(pVendor, , ME, "pVendor is NULL");
    if ((whm_mxl_dmnMngr_getDmnCtxState(pDmnCtx) == MXL_SECDMN_STATE_RST) && pDmnCtx->initPending) {
    /* Set daemon starting args after all DM has been loaded */
        if (swl_str_matches(pDmnCtx->name, MXL_HOSTAPD)) {
            swla_delayExec_add((swla_delayExecFun_cbf) s_setHapdDmnStartArgs, pVendor);
            pDmnCtx->initPending = false;
        }
    } else if (whm_mxl_dmnMngr_getDmnCtxState(pDmnCtx) == MXL_SECDMN_STATE_READY) {
        /* Update starting args and restart all radios in case of change
         * Note that for single hostapd, updating arguments is not needed since the getArgsCb will take
         * care of setting the new arguments upon hostapd restart.
         */
        if (swl_str_matches(pDmnCtx->name, MXL_HOSTAPD)) {
            if (!whm_mxl_dmnMngr_isDmnCtxGlbHpd(pDmnCtx)) {
                whm_mxl_dmnMngr_setDmnCtxState(pDmnCtx, MXL_SECDMN_STATE_RST);
                s_setHapdDmnStartArgs(pVendor);
            }
            whm_mxl_restartAllRadios();
        } else if (swl_str_matches(pDmnCtx->name, MXL_WPASUPPLICANT)) {
            if (suppMasterModeChanged) {
                /* restart all wpa_supplicants when Supplicant Master Mode is changed */
                s_whm_mxl_restartAllSupplicants();
            }
        }
    }
    SAH_TRACEZ_OUT(ME);
}

SWLA_DM_HDLRS(sDmnExecOpsDmHdlrs, ARR(), .objChangedCb = s_setDmnExecOptsObj_ocf);

void _whm_mxl_dmnMngr_setDmnExecOptsObj_ocf(const char* const sig_name,
                            const amxc_var_t* const data,
                            void* const priv) {
    swla_dm_procObjEvtOfLocalDm(&sDmnExecOpsDmHdlrs, sig_name, data, priv);
}

void whm_mxl_dmnMngr_setDmnCtxState(mxl_dmnMngrCtx_t* pDmnCtx , mxl_secDmn_state_t state) {
    ASSERT_NOT_NULL(pDmnCtx, , ME, "pDmnCtx is NULL");
    ASSERTS_TRUE(state < MXL_SECDMN_STATE_MAX, ,ME, "Bad state set request");
    SAH_TRACEZ_INFO(ME, "Setting dmn:%s state (%d) --> (%d)", pDmnCtx->name, pDmnCtx->state, state);
    pDmnCtx->state = state;
}

mxl_secDmn_state_t whm_mxl_dmnMngr_getDmnCtxState(mxl_dmnMngrCtx_t* pDmnCtx) {
    ASSERT_NOT_NULL(pDmnCtx, MXL_SECDMN_STATE_ERROR, ME, "pDmnCtx is NULL");
    return pDmnCtx->state;
}

void whm_mxl_dmnMngr_setDmnCtxGlbHpd(mxl_dmnMngrCtx_t* pDmnCtx , bool set) {
    ASSERT_NOT_NULL(pDmnCtx, , ME, "pDmnCtx is NULL");
    pDmnCtx->isGlbDmn = set;
}

bool whm_mxl_dmnMngr_isDmnCtxGlbHpd(mxl_dmnMngrCtx_t* pDmnCtx) {
    ASSERT_NOT_NULL(pDmnCtx, false, ME, "pDmnCtx is NULL");
    return pDmnCtx->isGlbDmn;
}

static bool s_initWpaSuppStartingArgs(char* startArgs) {
    bool ret;
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_WPASUPPLICANT);
    if(!pDmnCtx) {
       swl_str_copy(startArgs, sizeof(startArgs), WPASUPP_DEFAULT_START_ARGS);
       SAH_TRACEZ_WARNING(ME, "WPA_SUPPLICANT: No dmn ctx, falling back to default starting args");
       return true;
    }
    switch(pDmnCtx->dmnExecutionSettings.logDebugLevel) {
        case DMN_DEBUG_LEVEL_INFO:
            ret = swl_str_copy(startArgs, sizeof(startArgs), "-K");
            break;
        case DMN_DEBUG_LEVEL_DEBUG:
            ret = swl_str_copy(startArgs, sizeof(startArgs), "-dK");
            break;
        case DMN_DEBUG_LEVEL_EXCESSIVE:
            ret = swl_str_copy(startArgs, sizeof(startArgs), "-dddK");
            break;
        case DMN_DEBUG_LEVEL_MSGDUMP:
        case DMN_DEBUG_LEVEL_DEFAULT:
        default: 
            ret = swl_str_copy(startArgs, sizeof(startArgs), WPASUPP_DEFAULT_START_ARGS);
            break;
    }
    if(pDmnCtx->dmnExecutionSettings.logDebugLevel == DMN_OUTPUT_SYSLOG) {
        swl_strlst_cat(startArgs, sizeof(startArgs), "", "s");
    }
    ASSERTI_TRUE(ret, false, ME, "Copy error into wpa supplicant args");
    return ret;
}

swl_rc_ne s_writeWpaSuppArgsToBuf(char* args, size_t argsSize, char* confFilePath, size_t confFilePathSize, T_EndPoint* pEP) {
    ASSERT_NOT_NULL(pEP, SWL_RC_ERROR, ME, "NULL");
    bool ret;
    char cfgPath[128] = {0};
    ret = swl_str_catFormat(cfgPath, sizeof(cfgPath), WPASUPP_CONF_FILE_PATH_FORMAT, pEP->Name);
    ASSERTI_TRUE(ret, SWL_RC_ERROR, ME, "%s: writing wpaSupplicantConfigPath error", pEP->Name);

    if(confFilePath != NULL) {
        ret = swl_str_copy(confFilePath, confFilePathSize, cfgPath);
        ASSERTI_TRUE(ret, SWL_RC_ERROR, ME, "%s: writing wpaSupplicantConfigPath error", pEP->Name);
    }

    swl_str_copy(args, argsSize, "\0");
    ret = s_initWpaSuppStartingArgs(args);
    ASSERTI_TRUE(ret, SWL_RC_ERROR, ME, "%s: writing wpa supplicant args error", pEP->Name);
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_WPASUPPLICANT);
    ASSERT_NOT_NULL(pDmnCtx, SWL_RC_ERROR, ME, "pDmnCtx is NULL");

    if (pDmnCtx->dmnExecutionSettings.wpaSupplicantMasterMode) {
        if(!swl_str_isEmpty(pEP->bridgeName)) {
            ret = swl_str_catFormat(args, argsSize, WPASUPP_ARGS_STA_FORMAT_EXT, pEP->bridgeName, pEP->Name, cfgPath, pEP->pRadio->Name);
        } else {
            ret = swl_str_catFormat(args, argsSize, WPASUPP_ARGS_STA_FORMAT, pEP->Name, cfgPath, pEP->pRadio->Name);
        }
    } else {
        if(!swl_str_isEmpty(pEP->bridgeName)) {
            ret = swl_str_catFormat(args, argsSize, WPASUPP_ARGS_CERT_FORMAT_EXT, pEP->bridgeName, pEP->Name, cfgPath);
        } else {
            ret = swl_str_catFormat(args, argsSize, WPASUPP_ARGS_CERT_FORMAT, pEP->Name, cfgPath);
        }
    }
    ASSERTI_TRUE(ret, SWL_RC_ERROR, ME, "%s: writing wpaArgs error", pEP->Name);
    return SWL_RC_OK;
}

static char* s_getWpaSupArgsCb(wld_secDmn_t* pSecDmn, void* userdata _UNUSED) {
    ASSERT_NOT_NULL(pSecDmn, NULL, ME, "NULL");
    T_EndPoint* pEP = (T_EndPoint*) userdata;
    ASSERT_NOT_NULL(pEP, NULL, ME, "NULL");

    char startArgs[128] = {0};
    char confFilePath[128] = {0};
    s_writeWpaSuppArgsToBuf(startArgs, sizeof(startArgs), confFilePath, sizeof(confFilePath), pEP);

    char* args = NULL;
    swl_str_copyMalloc(&args, startArgs);
    SAH_TRACEZ_INFO(ME, "%s: WPA SUPPLICANT: args=%s", pEP->Name, args);
    return args;
}

swl_rc_ne whm_mxl_dmnMngr_setWpaSuppArgsHanlder(T_EndPoint* pEP) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERT_NOT_NULL(pEP->wpaSupp, SWL_RC_INVALID_PARAM, ME, "NULL");
    mxl_dmnMngrCtx_t* pDmnCtx = whm_mxl_dmnMngr_getDmnCtx(MXL_WPASUPPLICANT);
    ASSERT_NOT_NULL(pDmnCtx, SWL_RC_INVALID_PARAM, ME, "pDmnCtx is NULL");

    wld_secDmnEvtHandlers handlers = pEP->wpaSupp->handlers;
    //change getArg handlers, to provide custom startArgs just before starting wpa_supplicant
    handlers.getArgs = s_getWpaSupArgsCb;
    if (wld_secDmn_setEvtHandlers(pEP->wpaSupp, &handlers, pEP) == SWL_RC_OK) {
        pDmnCtx->initPending = false;
        whm_mxl_dmnMngr_setDmnCtxState(pDmnCtx, MXL_SECDMN_STATE_READY);
        return SWL_RC_OK;
    } else {
        SAH_TRACEZ_ERROR(ME, "Fail to set wpa_supplicant custom handlers");
        pDmnCtx->initPending = true;
        whm_mxl_dmnMngr_setDmnCtxState(pDmnCtx, MXL_SECDMN_STATE_RST);
        return SWL_RC_ERROR;
    }
}

bool whm_mxl_dmnMngr_isGlbHapdEnabled() {
    return s_isGlbHapdEnabled();
}
