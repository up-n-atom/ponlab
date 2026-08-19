/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_MLO_H__
#define __WHM_MXL_MLO_H__

#include "wld/wld.h"
#include "wld/wld_linuxIfUtils.h"

/* Function Declarations Section */
int32_t whm_mxl_getNumMLlinksbyID(int32_t id);
T_AccessPoint* whm_mxl_getSiblingBss(T_AccessPoint* pAP, int32_t id);
swl_rc_ne whm_mxl_createMLVap(T_AccessPoint* pAPlink1, T_AccessPoint* pAPlink2);
void whm_mxl_destroyMLVap(T_AccessPoint* pAP);
bool whm_mxl_isMLAssociatedVap(T_AccessPoint* pAP);
#endif /* __WHM_MXL_MLO_H__ */
