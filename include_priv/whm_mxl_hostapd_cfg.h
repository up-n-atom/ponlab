/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_HOSTAPD_CFG_H__
#define __WHM_MXL_HOSTAPD_CFG_H__

#include "wld/wld.h"

/* General Definitions Section */
#define HOSTAPD_COLOR_SWITCH_FORMAT "COLOR_SWITCH %s"
#define HOSTAPD_COLOR_SWITCH_COUNTDOWN_FORMAT "COLOR_SWITCH %s %d"

#define HTCAP_RX_STBC1      0x100
#define HTCAP_RX_STBC12     0x200
#define HTCAP_RX_STBC123    M_SWL_80211_HTCAPINFO_RX_STBC

#define VHTCAP_RX_STBC1     0x100
#define VHTCAP_RX_STBC12    0x200
#define VHTCAP_RX_STBC123   0x300
#define VHTCAP_RX_STBC1234  0x400

#define VHTCAP_MAX_AMPDU_EXP_M1 0x0800000
#define VHTCAP_MAX_AMPDU_EXP_M2 0x1000000
#define VHTCAP_MAX_AMPDU_EXP_M3 0x1800000
#define VHTCAP_MAX_AMPDU_EXP_M4 0x2000000
#define VHTCAP_MAX_AMPDU_EXP_M5 0x2800000
#define VHTCAP_MAX_AMPDU_EXP_M6 0x3000000
#define VHTCAP_MAX_AMPDU_EXP_M7 0x3800000

#define VHTCAPINFO_LINK_ADAPT_CAP 0x0c000000 //Defined again here because looks like its wrong in swl_80211.h
#define VHTCAP_LINK_ADAPT_CAP_M1  0x08000000

#define OBSS_INTERVAL_MIN                 10
#define OBSS_INTERVAL_MAX                 900
#define DFS_DEBUG_CHAN_MIN                32
#define DFS_DEBUG_CHAN_MAX                196
#define ZWDFS_DEBUG_CHAN_MIN              32
#define ZWDFS_DEBUG_CHAN_MAX              196
#define BSS_COLOR_CHANGE_TIMEOUT_MIN      1
#define BSS_COLOR_CHANGE_TIMEOUT_MAX      120
#define BSS_COLOR_CHANGE_TIMEOUT_RANDOM   255
#define BSS_COLOR_CHANGE_TIMEOUT_DEFAULT  50
#define NUM_BSS_COLOR_COLL_THRESH_DEFAULT 100
#define BSS_COLOR_COLL_AGE_THRESH_DEFAULT 10
#define USED_COLOR_TABLE_AGEING_DEFAULT   90
#define HE_OPERATION_BSS_COLOR            63
#define MGMT_FRAMES_RATE_DISABLED         0xFF
#define MGMT_FRAMES_RATE_DEFAULT          MGMT_FRAMES_RATE_DISABLED
#define MGMT_FRAMES_RATE_ENABLED_MIN      0
#define MGMT_FRAMES_RATE_ENABLED_MAX      7
#define CCA_THRESHOLD_MIN                 -90
#define CCA_THRESHOLD_MAX                 -20
#define RAD_MAX_STA_COUNT                 2007
#define DEFAULT_AP_MAX_STA                RAD_MAX_STA_COUNT
#define DEF_OBSS_INTERVAL                 300
#define DEF_OBSS_RSSI_THRESHOLD           (-60)
#define DEF_SCAN_PASSIVE_DWELL            20
#define DEF_SCAN_ACTIVE_DWELL             10
#define DEF_SCAN_PASSIVE_TOTAL_PER_CHAN   200
#define DEF_SCAN_ACTIVE_TOTAL_PER_CHAN    20
#define DEF_CHAN_TRANSITION_DELAY_FACTOR  5
#define DEF_SCAN_ACTIVITY_THRESHOLD       25

#define LEGACY_VENDOR_ELEMENTS "dd050009860100"
#define M_HOSTAPD_MULTI_AP_BBSS     0x1
#define M_HOSTAPD_MULTI_AP_FBSS     0x2
#define M_HOSTAPD_MULTI_AP_HYBRID   0x3

#define MXL_HAPD_6GHZ_PSD_20MHZ_MIN         (-127)    /* -63.5 dBm in 0.5dB steps */
#define MXL_HAPD_6GHZ_PSD_20MHZ_MAX         (126)     /* 63 dBm in 0.5dB steps */
#define MXL_HAPD_6GHZ_10LOG_20MHZ           (13)      /* 10log(20) */
#define MXL_HAPD_6GHZ_CONVERT_HALF_DB_UNIT  (2)       /* to convert to 0.5db unit */
#define MXL_HAPD_BYTE_2S_COMPLEMENT         (1<<8)

/* Macros Section */
#define WHM_MXL_SET_CAPABS(caps, mask, capsBuf, obj, paramName, capStrOutput) \
    { \
        if((caps & mask) && (amxd_object_get_value(bool, obj, paramName, NULL))) { \
            swl_str_cat(capsBuf, sizeof(capsBuf), capStrOutput); \
        } \
    }

#define WHM_MXL_SET_CAPABS_STBC(caps, mask, stbcMask, capsBuf, capStrOutput) \
    { \
        if((caps & mask) == (stbcMask)) { \
            swl_str_cat(capsBuf, sizeof(capsBuf), capStrOutput); \
        } \
    }

#define MXL_RAD_CAT_SET_CAPAB(targetBuf, tempBuf, capStrOutput) \
        swl_str_cat(targetBuf, sizeof(targetBuf), "["); \
        swl_str_cat(targetBuf, sizeof(targetBuf), capStrOutput); \
        swl_str_cat(targetBuf, sizeof(targetBuf), tempBuf); \
        swl_str_cat(targetBuf, sizeof(targetBuf), "]"); \

#define WHM_MXL_GT_SET_PARAM(refParamVal, gtVal, configMap, confName) \
    { \
        if(refParamVal > gtVal) { \
            swl_mapCharFmt_addValStr(configMap, confName, "%d", refParamVal); \
        } \
    }

#define WHM_MXL_NE_SET_PARAM(refParamVal, val, configMap, confName) \
    { \
        if(refParamVal != val) { \
            swl_mapCharFmt_addValStr(configMap, confName, "%d", refParamVal); \
        } \
    }

#define WHM_MXL_GET_AND_SET_STRING_PARAM(tmpStr, obj, paramName, configMap, confName) \
    { \
        tmpStr = amxd_object_get_value(cstring_t, obj, paramName, NULL); \
        if (!swl_str_isEmpty(tmpStr)) { \
            swl_mapCharFmt_addValStr(configMap, confName, "%s", tmpStr); \
        } \
        free(tmpStr); \
    }

/* Function Declarations Section */
/* Radio Configs */
swl_rc_ne whm_mxl_rad_updateConfigMap(T_Radio* pRad, swl_mapChar_t* configMap);

/* VAP Configs */
swl_rc_ne whm_mxl_vap_updateConfigMap(T_AccessPoint* pAP, swl_mapChar_t* configMap);

#endif /* __WHM_MXL_HOSTAPD_CFG_H__ */
