/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_FSM_LOCKER_H__
#define __WHM_MXL_FSM_LOCKER_H__

#include "wld/wld_types.h"
#include "wld/wld_fsm.h"

bool whm_mxl_reconfFsmLock(T_Radio* pRad);
void whm_mxl_reconfFsmUnLock(T_Radio* pRad);
void whm_mxl_reconfFsmEnsureLock(T_Radio* pRad);
void whm_mxl_extLocker_init(wld_fsmMngr_t* fsmMngr);

#endif /* __WHM_MXL_FSM_LOCKER_H__ */
