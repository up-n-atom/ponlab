/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_WPA_SUPP_CFG_H__
#define __WHM_MXL_WPA_SUPP_CFG_H__

#include "wld/wld.h"

/* General Definitions Section */

/* Macros Section */
#define MXL_EP_CHECK_GT_SET_NEW_PARAM(refParamVal, gtVal, configMap, confName) \
    { \
        if(refParamVal > gtVal) { \
            swl_mapCharFmt_addValInt32(configMap, confName, refParamVal); \
        } \
    }

/* Function Declarations Section */
swl_rc_ne whm_mxl_ep_updateConfigMaps(T_EndPoint* pEP, wld_wpaSupp_config_t* configMap);

#endif /* __WHM_MXL_WPA_SUPP_CFG_H__ */
