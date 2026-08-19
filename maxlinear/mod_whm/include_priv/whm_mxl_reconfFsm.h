/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_RECONF_FSM_H__
#define __WHM_MXL_RECONF_FSM_H__

#include "wld/wld_fsm.h"

#define MXL_RECONF_FSM_REQ_NO_OK    (false)
#define MXL_RECONF_FSM_REQ_OK       (true)

typedef enum {
    MXL_RECONF_FSM_IDLE,
    MXL_RECONF_FSM_RUNNING,
    MXL_RECONF_FSM_MAX
} whm_mxl_reconfFsm_brief_state_e;


FSM_STATE whm_mxl_reconf_fsm(T_Radio* pRad);
void whm_mxl_reconFsm_allRadioReset(void);
void whm_mxl_reconfFsm_init(T_Radio* pRad);

#endif /* __WHM_MXL_RECONF_FSM_H__ */
