/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_utils.c                                        *
*         Description  : General functions                                      *
*                                                                               *
*  *****************************************************************************/


#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "wld/wld_chanmgt.h"
#include "wld/wld_secDmn.h"
#include "wld/wld_secDmnGrp.h"
#include "swl/swl_common.h"
#include "swla/swla_chanspec.h"
#include "whm_mxl_utils.h"

#define ME "mxlUtils"

void mxl_removeExtraSpacesfromString(char **str) {
    char* temp = (char*) *str;
    if (*temp == '\0') {
       return;
    }

    char *back = temp;
    char *front = temp + 1;
    /* In place removal of duplicate spaces */
    while(*front != '\0') {
        if (*front != ' ' || *back != ' ') {
            *(++back) = *front;
        }
        front++;
    }

    /* Add end of string and remove possible space at the end */
    if (*back == ' ') {
        *back = '\0';
    } else {
        *(back + 1) = '\0';
    }

    /* Remove possible space at the begining*/
    front = temp;
    if(*front == ' ') {
        for (int j = 1; *(front + j - 1) != '\0'; j++) {
            *(front + j - 1) = *(front + j);
        }
    }
}

amxd_status_t mxl_convertStringToIntList(int *int_list, char *val, int exp_count) {
    int count = 0;
    char* pos = val; 
    char* end = pos;
    bool space = true;

    // check if the string holds exp_count integers
    while (*pos != '\0') {
        if (*pos == ' ') {
            space = true;
        } else {
            if (space == true) {
                count ++;
                space = false;
            }
            if (*pos != '-' && (*pos < '0' || *pos > '9')) {
                return amxd_status_invalid_value;
            }
        }
        pos++;
    }
    if (count != exp_count) {
        return amxd_status_invalid_value;
    }

    // convert the sring to a list of int
    pos = val;
    count = 0;
    while (count != exp_count) {
        if(*pos != ' ') {
            end = strchr(pos, ' ');
            if (end) {
                *end = '\0';
            }
            int_list[count++] = atoi(pos);
            if (!end)
                break;
            pos = end + 1;
        } else {
            pos++; 
        }
    }
    return amxd_status_ok;
}

/**
 * @brief Retrieves the bridge interface name associated with a given network interface.
 *
 * This function determines the bridge interface name to which a specified network interface belongs.
 * It reads the symbolic link from the sysfs path and extracts the bridge name.
 *
 * @param iface The name of the network interface (e.g., "wlan0.1").
 * @param bridge A buffer to store the name of the bridge interface.
 * @return swl_rc_ne Returns SWL_RC_OK on success, or an appropriate error code on failure.
 */
swl_rc_ne whm_mxl_utils_getIfBridge(char* iface, char* bridge) {
    ASSERT_NOT_NULL(iface, SWL_RC_INVALID_PARAM, ME, "iface is NULL");
    char path[128] = {0};
    char brLink[128] = {0};

    swl_str_catFormat(path, sizeof(path), "/sys/class/net/%s/brport/bridge", iface);
    int res = readlink(path, brLink, sizeof(brLink));
    ASSERT_FALSE((res < 0), SWL_RC_ERROR, ME, "readlink failed for %s", path);

    char *br = strrchr(brLink, '/');
    ASSERT_NOT_NULL(br, SWL_RC_INVALID_PARAM, ME, "br is NULL");
    const char *brName = br + 1;
    swl_str_copy(bridge, IFNAMSIZ, brName);
    SAH_TRACEZ_INFO(ME, "%s: belongs to bridge %s", iface, bridge);
    return SWL_RC_OK;
}

/**
 * @brief Check if pAP is a dummy VAP
 *
 * @param pAP AccessPoint
 * @return return true if pAP is dummy VAP or false otherwise.
 */
bool whm_mxl_utils_isDummyVap(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, false, ME, "pAP is NULL");
    T_Radio* pRad = (T_Radio*) pAP->pRadio;
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    T_AccessPoint* dummyVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(dummyVap, false, ME, "dummyVap is NULL");
    return (dummyVap == pAP) ? true : false;
}

/**
 * @brief Check if at least one AccessPoint is enabled on radio, not including dummy VAP
 *
 * @param pRad Radio
 * @return return true if at least one AccessPoint is active, false otherwise.
 */
bool whm_mxl_utils_isAnyApActive(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    T_AccessPoint* dummyVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(dummyVap, false, ME, "dummyVap is NULL");
    T_AccessPoint* otherAp;
    wld_rad_forEachAp(otherAp, pRad) {
        if ((otherAp != NULL) && (otherAp->pSSID != NULL)) {
            if ((otherAp != dummyVap) && (otherAp->enable) && (otherAp->pSSID->enable)) {
                SAH_TRACEZ_INFO(ME,"%s: Is active", otherAp->alias);
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Check if at least one AccessPoint is disabled on radio, not including dummy VAP
 *
 * @param pRad Radio
 * @return return true if at least one AccessPoint is disabled, false otherwise.
 */
bool whm_mxl_utils_hasDisabledVap(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    T_AccessPoint* pAP = NULL;
    T_AccessPoint* dummyVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(dummyVap, false, ME, "dummyVap is NULL");
    /* Check ifone of the AP is disabled */
    wld_rad_forEachAp(pAP, pRad) {
        if (!pAP->enable && pAP != dummyVap) {
            SAH_TRACEZ_INFO(ME,"%s: Is disabled", pAP->alias);
            return true;
        }
    }
    return false;
}

/**
 * @brief Get number of VAPs in radio
 *
 * @param pAP Radui
 * @return return number of VAPs in radio.
 */
int whm_mxl_utils_getNumOfVaps(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, -1, ME, "pRad is NULL");
    int numOfVaps = 0;
    T_AccessPoint* pAP;
    wld_rad_forEachAp(pAP, pRad) {
        if (pAP) {
            numOfVaps++;
        }
    }
    return numOfVaps;
}

/**
 * @brief Get the first enabled AP in Radio
 *
 * @param pRad Radio
 * @return pAP Return AP context.
 */
T_AccessPoint* whm_mxl_utils_getFirstEnabledVap(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, NULL, ME, "pRad is NULL");
    T_AccessPoint* pAP;
    wld_rad_forEachAp(pAP, pRad) {
        if (pAP && !whm_mxl_utils_isDummyVap(pAP)) {
            if (pAP->enable && (pAP->status == APSTI_ENABLED)) {
                return pAP;
            }
        }
    }
    return NULL;
}

/**
 * @brief Get the master VAP of the Radio
 *
 * @param pRad Radio
 * @return pAP Return master VAP context.
 */
T_AccessPoint* whm_mxl_utils_getMasterVap(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, NULL, ME, "pRad is NULL");
    T_AccessPoint* pAP;
    wld_rad_forEachAp(pAP, pRad) {
        if (whm_mxl_utils_isDummyVap(pAP)) {
            return pAP;
        }
    }
    return NULL;
}

uint32_t whm_mxl_utils_numOfGrpMembers(T_Radio* pRad) {
    ASSERTS_NOT_NULL(pRad, 0, ME, "NULL");
    wld_secDmn_t* pSecDmn = pRad->hostapd;
    ASSERTS_NOT_NULL(pSecDmn->dmnProcess, 0, ME, "NULL");
    if (wld_secDmn_isGrpMember(pSecDmn)) {
        return wld_secDmnGrp_getMembersCount(wld_secDmn_getGrp(pSecDmn));
    }
    return 1;
}

/**
 * @brief Check if provided chanspec BW is equal to certain swl_bandwidth_e
 *
 * @param chspec chanspec to check
 * @param chW channel width to check against
 * @return return true if equal, false otherwise.
 */
bool whm_mxl_isChannelWidthEqual(swl_chanspec_t chspec, swl_bandwidth_e chW) {
    return (swl_chanspec_bwToInt(chspec.bandwidth) == swl_chanspec_bwToInt(chW));
}

/**
 * @brief Check if target chanspec BW is equal to certain swl_bandwidth_e
 *
 * @param pRad Radio context
 * @param chW channel width to check against
 * @return return true if equal, false otherwise.
 */
bool whm_mxl_isTgtChannelWidthEqual(T_Radio* pRad, swl_bandwidth_e chW) {
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    return whm_mxl_isChannelWidthEqual(wld_chanmgt_getTgtChspec(pRad), chW);
}

/**
 * @brief Check if current chanspec BW is equal to certain swl_bandwidth_e
 *
 * @param pRad Radio context
 * @param chW channel width to check against
 * @return return true if equal, false otherwise.
 */
bool whm_mxl_isCurChannelWidthEqual(T_Radio* pRad, swl_bandwidth_e chW) {
    ASSERT_NOT_NULL(pRad, false, ME, "pRad is NULL");
    return whm_mxl_isChannelWidthEqual(wld_chanmgt_getCurChspec(pRad), chW);
}
