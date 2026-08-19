/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_MONITOR_H__
#define __WHM_MXL_MONITOR_H__

#include "wld/wld.h"
#include "swl/swl_common_time_spec.h"

typedef struct nastaEntryData {
    amxc_llist_it_t it;
    swl_macBin_t mac;
    swl_timeSpecMono_t startTs;
} mxl_nastaEntryData_t;

typedef struct nastaData {
    /**
     * Unconnected STA scan timeout value from driver
     */
    uint32_t scanTimeout;

    /**
     * Timer to stop requested unconnected sta data when scanTimeout passes before getting any reponse
     */
    amxp_timer_t* timer;

    /*
     * list of runtime Unassoc stations being scanned, and waiting for measurements
     */
    amxc_llist_t scanList;
} mxl_nastaData_t;

void whm_mxl_monitor_init(T_Radio* pRad);
void whm_mxl_monitor_deinit(T_Radio* pRad);
int whm_mxl_monitor_getStaScanTimeOut(T_Radio* pRad);
int whm_mxl_monitor_setStaScanTimeOut(T_Radio* pRad, uint32_t scanTime);

int whm_mxl_monitor_setupStamon(T_Radio* pRad, bool enable);
int whm_mxl_monitor_updateMonStats(T_Radio* pRad);
int whm_mxl_monitor_updateNaStaObj(T_Radio* pRad);
swl_rc_ne whm_mxl_monitor_addStaMon(T_Radio* pRad, T_NonAssociatedDevice* pMD);
int whm_mxl_monitor_delStamon(T_Radio* pRad, T_NonAssociatedDevice* pMD);

mxl_nastaEntryData_t* whm_mxl_monitor_fetchRunNaStaEntry(T_Radio* pRad, swl_macBin_t* pStaMac);
mxl_nastaEntryData_t* whm_mxl_monitor_addRunNaStaEntry(T_Radio* pRad, swl_macBin_t* pStaMac);
void whm_mxl_monitor_delRunNaStaEntry(mxl_nastaEntryData_t* pEntry);
void whm_mxl_monitor_dropAllRunNaStaEntries(T_Radio* pRad);
void whm_mxl_monitor_checkRunNaStaList(T_Radio* pRad);
uint32_t whm_mxl_monitor_getRunNaStaEntryCount(T_Radio* pRad);

#endif /* __WHM_MXL_MONITOR_H__ */
