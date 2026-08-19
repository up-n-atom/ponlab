/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_EVT_H__
#define __WHM_MXL_EVT_H__

#include "wld/wld.h"

#define WPA_MSG_LEVEL_INFO "<3>"
#define WPA_MSG_MAX_EVENT_NAME_LEN 64

typedef void (* evtParser_f)(void* userData, char* ifName, char* event, char* params);
typedef swl_rc_ne (* custEvtParser_f)(void* userData, char* ifName, char* event, char* params);

swl_rc_ne mxl_evt_setVendorEvtHandlers(T_Radio* pRad);

#endif /* __WHM_MXL_EVT_H__ */
