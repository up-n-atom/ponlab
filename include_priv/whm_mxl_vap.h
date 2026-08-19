/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_VAP_H__
#define __WHM_MXL_VAP_H__

#include "wld/wld.h"
#include "wld/wld_linuxIfUtils.h"
#include "whm_mxl_cfgActions.h"

/* General Definitions Section */
typedef enum {
    MXL_MAP_OFF = 0,
    MXL_BACKHAUL_MAP = 1,
    MXL_FRONTHAUL_MAP,
    MXL_HYBRID_MAP,
    MXL_MAP_TYPE_MAX
} whm_mxl_multi_ap_type_e;

typedef struct {
    /* MxL Multi AP Type */
    whm_mxl_multi_ap_type_e mxlMultiApType;
    /* vendor_vht is used in 2.4GHz 11n mode */
    bool vendorVht;
    /* Vendor object */
    amxd_object_t* pVendorBus;
    /* MxL reconf FSM - Used only to request VAP actions from Reconf FSM */
    T_FSM reconfFsm;
    /* OWE Transition SSID */
    char OWETransSSID[SSID_NAME_LEN];
    /* OWE Transition BSSID */
    char OWETransBSSID[ETHER_ADDR_STR_LEN];
    /* MLO ID */
    int32_t mloId;
    /* Flag to indicate ML VAP destory is in progress */
    bool MLO_destroyInProgress;
    /* To Configure AKM24 type SAE-EXT-KEY */
    bool saeExtKey;
    /* Flag to indicate WPA3 Personal Compatibility Mode */
    bool EnableWPA3PersonalCompatibility;
    /*Set Aggr config */
    char AggrConfig[MAX_NUM_OF_DIGITS];
    /* Setting the Management Group Cipher */
    char GroupMgmtCipher[MAX_NUM_OF_DIGITS];
    /* Setting Group Cipher */
    char GroupCipher[MAX_NUM_OF_DIGITS];
    /* Radius Secret Password for Enterprise Security modes */
    char radiusSecretKey[70];
    /* Enable sync timer */
    amxp_timer_t* onVapEnableSyncTimer;
    /* VAP on enable reload pending status */
    bool vapEnableReloadPending;
    /* For Certification H2E is mandated for sae_pwe */
    bool h2eRequired;
    /* Enable or Disable ignoring of 11vDiassoc timer */
    bool ignore11vDiassoc;
    /* Setting the QoS Map */
    char QoSMap[70];
} mxl_VapVendorData_t;

/* Macros Section */

/* Function Declarations Section */
int whm_mxl_vap_createHook(T_AccessPoint* pAP);
void whm_mxl_vap_destroyHook(T_AccessPoint* pAP);
mxl_VapVendorData_t* mxl_vap_getVapVendorData(const T_AccessPoint* pAP);
swl_rc_ne whm_mxl_vap_getSingleStationStats(T_AssociatedDevice* pAD);
swl_rc_ne whm_mxl_vap_getStationStats(T_AccessPoint* pAP);

int whm_mxl_vap_updateApStats(T_AccessPoint* pAP);
int whm_mxl_vap_enable(T_AccessPoint* pAP, int enable, int set);
int whm_mxl_vap_ssid(T_AccessPoint* pAP, char* buf, int bufsize, int set);
int whm_mxl_vap_wps_enable(T_AccessPoint* pAP, int enable, int set);
int whm_mxl_vap_bssid(T_Radio* pR, T_AccessPoint* pAP, unsigned char* buf, int bufsize, int set);
int whm_mxl_vap_sec_sync(T_AccessPoint* pAP, int set);
int whm_mxl_vap_multiap_update_type(T_AccessPoint* pAP);
int whm_mxl_vap_clean_sta(T_AccessPoint* pAP, char* macStr, int macStrLen);
swl_rc_ne whm_mxl_vap_updated_neighbor(T_AccessPoint* pAP, T_ApNeighbour* pApNeighbor);
swl_rc_ne whm_mxl_vap_transfer_sta(T_AccessPoint* pAP, wld_transferStaArgs_t* params);
bool mxl_isApReadyToProcessVendorCmd(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_vap_addWdsIfaceEvent(T_AccessPoint* pAP, wld_wds_intf_t* wdsIntf);
swl_rc_ne whm_mxl_vap_delWdsIfaceEvent(T_AccessPoint* pAP, wld_wds_intf_t* wdsIntf);
void whm_mxl_vap_requestReconf(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_vap_postUpActions(T_AccessPoint* pAP);
swl_rc_ne whm_mxl_vap_postDownActions(T_AccessPoint* pAP);
void whm_mxl_registerToWdsEvent(void);
void whm_mxl_unregisterToWdsEvent(void);

#endif /* __WHM_MXL_VAP_H__ */
