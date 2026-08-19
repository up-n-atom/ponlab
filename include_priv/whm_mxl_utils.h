/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_UTILS_H__
#define __WHM_MXL_UTILS_H__


typedef unsigned long long int u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint64 mtlk_osal_hr_timestamp_t;

#ifndef IEEE_ADDR_LEN
#define IEEE_ADDR_LEN                   (6)
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

/* Needed for iwlwav-driver-uci headers include */
#ifndef CONFIG_WAVE_DEBUG
#define CONFIG_WAVE_DEBUG
#endif

#include <mhi_umi.h>
#include <mhi_ieee_address.h>
#include <wave_hal_stats.h>
#include <mtlkwssa_drvinfo.h>

#include <vendor_cmds_copy.h>

#define VENDOR_SUBCMD_IS_SYNC              true
#define VENDOR_SUBCMD_IS_ASYNC             false
#define VENDOR_SUBCMD_IS_WITH_ACK          true
#define VENDOR_SUBCMD_IS_WITHOUT_ACK       false

#define BF_MODE_ENABLE_EXPLICIT  0
#define BF_MODE_ENABLE_IMPLICIT  1
#define BF_MODE_DISABLE_ALL      4
#define BF_MODE_ENABLE_ALL       255


#define MXL_COC_MODE_MANUAL 0
#define MXL_COC_MODE_AUTO 1

typedef enum {
    MXL_RESET_STATS_CATEGORY_FULL,
    MXL_RESET_STATS_CATEGORY_RADIO,
    MXL_RESET_STATS_CATEGORY_VAP,
    MXL_RESET_STATS_CATEGORY_STA,
    MXL_RESET_STATS_CATEGORY_MAX
} mxl_resetStatsCategory_e;

amxd_status_t mxl_convertStringToIntList(int *int_list, char *val, int exp_count);
void mxl_removeExtraSpacesfromString(char **str);
swl_rc_ne whm_mxl_utils_getIfBridge(char* iface, char* bridge);
bool whm_mxl_utils_isDummyVap(T_AccessPoint* pAP);
bool whm_mxl_utils_isAnyApActive(T_Radio* pRad);
bool whm_mxl_utils_hasDisabledVap(T_Radio* pRad);
int whm_mxl_utils_getNumOfVaps(T_Radio* pRad);
T_AccessPoint* whm_mxl_utils_getFirstEnabledVap(T_Radio* pRad);
T_AccessPoint* whm_mxl_utils_getMasterVap(T_Radio* pRad);
uint32_t whm_mxl_utils_numOfGrpMembers(T_Radio* pRad);
bool whm_mxl_isChannelWidthEqual(swl_chanspec_t chspec, swl_bandwidth_e chW);
bool whm_mxl_isTgtChannelWidthEqual(T_Radio* pRad, swl_bandwidth_e chW);
bool whm_mxl_isCurChannelWidthEqual(T_Radio* pRad, swl_bandwidth_e chW);

#endif /* __WHM_MXL_UTILS_H__ */
