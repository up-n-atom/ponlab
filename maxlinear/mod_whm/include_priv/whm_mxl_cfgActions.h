/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_CONFIG_ACTIONS_H__
#define __WHM_MXL_CONFIG_ACTIONS_H__

#include "wld/wld.h"

#define MSG_LENGTH 4096
#define MAX_NUM_OF_DIGITS 20
#define DM_EVENT_HOOK_TIMEOUT_MS 100

typedef enum {
    HAPD_ACTION_ERROR = -1,
    HAPD_ACTION_NONE = 0,
    HAPD_ACTION_NEED_UPDATE_CONF,
    HAPD_ACTION_NEED_UPDATE_BEACON,
    HAPD_ACTION_NEED_TOGGLE,
    HAPD_ACTION_NEED_SIGHUP,
    HAPD_ACTION_NEED_RECONF,
    HAPD_ACTION_NEED_RESTART
} whm_mxl_hapd_action_e;

typedef enum {
    WPA_SUPP_ACTION_ERROR = -1,
    WPA_SUPP_ACTION_NONE = 0,
    WPA_SUPP_ACTION_NEED_UPDATE
} whm_mxl_supplicant_action_e;

typedef enum {
    WHM_MXL_CONFIG_TYPE_GENRAL,
    WHM_MXL_CONFIG_TYPE_SSID,
    WHM_MXL_CONFIG_TYPE_SECURITY,
    WHM_MXL_CONFIG_TYPE_MAX
} whm_mxl_config_type_e;

typedef enum {
    WHM_MXL_CONFIG_FLOW_GENERIC,
    WHM_MXL_CONFIG_FLOW_RECONF,
    WHM_MXL_CONFIG_FLOW_MAX
} whm_mxl_config_flow_e;

typedef swl_rc_ne (* whm_mxl_actionHandler_f) (T_Radio* pRad, T_AccessPoint* pAP);
typedef swl_rc_ne (* whm_mxl_actionEpHandler_f) (T_Radio* pRad, T_EndPoint* pEP);

swl_rc_ne whm_mxl_determineRadParamAction(T_Radio* pRad, const char* paramName, const char* paramValue);
swl_rc_ne whm_mxl_determineVapParamAction(T_AccessPoint* pAP, const char* paramName, const char* paramValue);
swl_rc_ne whm_mxl_determineEpParamAction(T_EndPoint* pEP, const char* paramName);
swl_rc_ne whm_mxl_restartHapd(T_Radio* pRad);
swl_rc_ne whm_mxl_mlo_restartHapd(T_Radio* pRad);
swl_rc_ne whm_mxl_toggleHapd(T_Radio* pRad);
swl_rc_ne whm_mxl_sighupHapd(T_Radio* pRad);
swl_rc_ne whm_mxl_updateBeaconHapd(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_confModHapd(T_Radio* pRad, T_AccessPoint* pAP);
swl_rc_ne whm_mxl_restartAllRadios();
swl_rc_ne whm_mxl_set_vendorMultipleParams(T_AccessPoint* pAP, const char* params[], uint32_t nParams);
swl_rc_ne whm_mxl_updateMultiAp(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_handleMbssidOverride(T_Radio* pRad, bool overideMbssid);
swl_rc_ne whm_mxl_updateOnEventMaxAssociatedDevices(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_configureSaeExt(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_hostapd_setMldParams(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_toggleWPA3PersonalCompatibility(T_AccessPoint* pAP);
whm_mxl_config_flow_e whm_mxl_chooseVapConfigFlow(T_AccessPoint* pAP, whm_mxl_config_type_e type);
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
swl_rc_ne whm_mxl_configureBgAcs(T_Radio* pRad, uint16_t bgAcsInterval);
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
bool whm_mxl_hostapd_sendCommand(T_AccessPoint* pAP, char* cmd, const char* reason);
bool whm_mxl_wpaCtrl_sendCmdCheckSpecificResponse(T_AccessPoint* pAP, char* cmd, const char* reason, char* expectedResponse);

#endif /* __WHM_MXL_CONFIG_ACTIONS_H__ */
