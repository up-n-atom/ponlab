/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_EP_H__
#define __WHM_MXL_EP_H__

#include "wld/wld.h"

int whm_mxl_epStats(T_EndPoint* pEP, T_EndPointStats* stats);
swl_rc_ne whm_mxl_ep_enable(T_EndPoint* pEP, bool enable);
int whm_mxl_ep_createHook(T_EndPoint* pEP);
int whm_mxl_ep_destroyHook(T_EndPoint* pEP);

#endif /* __WHM_MXL_EP_H__ */
