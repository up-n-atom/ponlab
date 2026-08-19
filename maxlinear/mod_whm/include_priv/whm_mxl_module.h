/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_MODULE_H__
#define __WHM_MXL_MODULE_H__

#include "wld/wld_vendorModule.h"

#define MXL_VENDOR_NAME "whm-mxl"

/**
 * @brief path of vendor module main odl file
 * eg: modules/mod-<vendorName>/mod-<vendorName>.odl
 */
#define VENDOR_MODULE_TOKEN WLD_VENDOR_MODULE_PREFIX MXL_VENDOR_NAME
#define ODL_DIR "modules/" VENDOR_MODULE_TOKEN
#define ODL_MAIN_FILE ODL_DIR "/" VENDOR_MODULE_TOKEN ".odl"
#define ODL_DEFAULTS_DIR ODL_DIR "/" VENDOR_MODULE_TOKEN "_defaults/"

typedef enum {
    WHM_MXL_MOD_NORMAL,
    WHM_MXL_MOD_CERTIFICATION,
    WHM_MXL_MOD_MAX
} whm_mxl_module_mode_e;

typedef struct {
    whm_mxl_module_mode_e mode;
    /* Add other configs here if needed */
    bool wpa3CertMode;
} whm_mxl_module_mode_t;

bool whm_mxl_module_init();
bool whm_mxl_module_deInit();
bool whm_mxl_module_loadDefaults();
swl_rc_ne whm_mxl_module_addRadios();
whm_mxl_module_mode_e whm_mxl_getModuleMode();
bool whm_mxl_isCertModeEnabled();
bool whm_mxl_isWpa3CertModeEnabled();

#endif // __WHM_MXL_MODULE_H__
