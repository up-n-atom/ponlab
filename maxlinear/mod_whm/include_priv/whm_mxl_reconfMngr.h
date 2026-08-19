/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_RECONF_MNGR_H__
#define __WHM_MXL_RECONF_MNGR_H__

#include "wld/wld_fsm.h"

typedef enum {
    RECONF_FSM_MOD_HAPD_CONF_FILE,      /* Write new configuration file for hostapd */
	RECONF_FSM_RELOAD_BSS,              /* Reload BSS configuration */
    RECONF_FSM_DO_RECONF_BSS,           /* Execute MXL Reconf on specific BSS */
    RECONF_FSM_DO_RECONF,               /* Execute MXL Reconf to all changed BSS */
    RECONF_FSM_SYNC_RECONF,             /* Sync all vaps in radio after Reconf */
    RECONF_FSM_SYNC,                    /* Sync action for general purpose */
    RECONF_FSM_MAX
} whm_mxl_reconfMngr_actions_e;

wld_fsmMngr_t* whm_mxl_get_reconfMngr(void);
int whm_mxl_reconfMngr_doCommit(T_Radio* pRad);
void whm_mxl_reconfMngr_notifyCommit(T_Radio* pRad);
void whm_mxl_reconfMngr_notifyVapCommit(T_AccessPoint* pAP);
void whm_mxl_reconfMngr_init(T_Radio* pRad);
void whm_mxl_reconfMngr_deinit(T_Radio* pRad);
void whm_mxl_reconfMngr_initEvents(void);

#endif /* __WHM_MXL_RECONF_MNGR_H__ */
