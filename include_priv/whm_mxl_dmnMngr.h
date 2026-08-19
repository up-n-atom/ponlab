/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_DMN_MNGR_H__
#define __WHM_MXL_DMN_MNGR_H__

#include "wld/wld.h"

#define HOSTAPD_CONF_FILE_PATH_FORMAT "/tmp/%s_hapd.conf"
#define HOSTAPD_DEFAULT_START_ARGS "-ddt"
#define MXL_HOSTAPD "hostapd"

#define WPASUPP_CONF_FILE_PATH_FORMAT "/tmp/%s_wpa_supplicant.conf"
#define WPASUPP_DEFAULT_START_ARGS "-ddK"
#define WPASUPP_ARGS_CERT_FORMAT_EXT " -b%s -i%s -Dnl80211 -c%s"
#define WPASUPP_ARGS_CERT_FORMAT " -i%s -Dnl80211 -c%s"
#define WPASUPP_ARGS_STA_FORMAT_EXT " -b%s -i%s -Dnl80211 -c%s -F -H /var/run/hostapd/%s"
#define WPASUPP_ARGS_STA_FORMAT " -i%s -Dnl80211 -c%s -F -H /var/run/hostapd/%s"
#define MXL_WPASUPPLICANT "wpa_supplicant"

enum {
    DMN_DEBUG_LEVEL_INFO,
    DMN_DEBUG_LEVEL_DEBUG,
    DMN_DEBUG_LEVEL_MSGDUMP,
    DMN_DEBUG_LEVEL_EXCESSIVE,
    DMN_DEBUG_LEVEL_DEFAULT,
    DMN_DEBUG_LEVEL_MAX,
};

enum {
    DMN_OUTPUT_STDOUT,
    DMN_OUTPUT_SYSLOG,
    DMN_OUTPUT_MAX,
};

typedef enum {
    MXL_SECDMN_STATE_ERROR = -1,
    MXL_SECDMN_STATE_RST = 0,
    MXL_SECDMN_STATE_READY,
    MXL_SECDMN_STATE_MAX,
} mxl_secDmn_state_t;

typedef struct {
    swl_enum_e logOutputPath;   //indicates wheter deamon outputs log messages to syslog or stdout: {Stdout, Syslog}
    swl_enum_e logDebugLevel;   //set daemon output log level: {Info, Debug, Msgdump, Excessive, Default}
    bool wpaSupplicantMasterMode; //set wpa_supplicant master mode: {true, false}
} mxl_dmnMngrExecSettings;

typedef struct mxl_dmnMngrCtx {
    bool isGlbDmn;
    mxl_secDmn_state_t state;
    bool initPending;
    char* name;
    amxd_object_t* object;
    mxl_dmnMngrExecSettings dmnExecutionSettings;
} mxl_dmnMngrCtx_t;

mxl_dmnMngrCtx_t* whm_mxl_dmnMngr_getDmnCtx(const char* dmnName);
void whm_mxl_dmnMngr_setDmnCtxState(mxl_dmnMngrCtx_t* pDmnCtx , mxl_secDmn_state_t state);
mxl_secDmn_state_t whm_mxl_dmnMngr_getDmnCtxState(mxl_dmnMngrCtx_t* pDmnCtx);
void whm_mxl_dmnMngr_setDmnCtxGlbHpd(mxl_dmnMngrCtx_t* pDmnCtx , bool set);
bool whm_mxl_dmnMngr_isDmnCtxGlbHpd(mxl_dmnMngrCtx_t* pDmnCtx);
swl_rc_ne whm_mxl_dmnMngr_setDmnExecSettings(vendor_t* pVdr, const char* dmnName, wld_dmnMgt_dmnExecSettings_t* pCfg);
void whm_mxl_setSecDmnStartingArgs(T_Radio* pRad);
bool whm_mxl_dmnMngr_isGlbHapdEnabled();
swl_rc_ne whm_mxl_dmnMngr_setWpaSuppArgsHanlder(T_EndPoint* pEP);

#endif /* __WHM_MXL_DMN_MNGR_H__ */
