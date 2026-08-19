/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl.c                                             *
*         Description  : Vendor module entry point                             *
*                                                                              *
*  *****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <debug/sahtrace.h>
#include <errno.h>

#include "wld/wld.h"
#include "wld/wld_vendorModule.h"
#include "whm_mxl_module.h"

#define ME "mxlMain"

static bool s_initCb(wld_vendorModule_initInfo_t* pInitInfo _UNUSED) {
    SAH_TRACEZ_IN(ME);
    whm_mxl_module_init();
    SAH_TRACEZ_OUT(ME);
    return true;
}

static bool s_deinitCb() {
    SAH_TRACEZ_IN(ME);
    SAH_TRACEZ_WARNING(ME, "Stopping mxl vendor module");
    whm_mxl_module_deInit();
    SAH_TRACEZ_OUT(ME);
    return true;
}

static bool s_loadDefaultsCb() {
    SAH_TRACEZ_IN(ME);
    SAH_TRACEZ_WARNING(ME, "Loading default dm configs of mxl vendor module");
    whm_mxl_module_loadDefaults();
    SAH_TRACEZ_OUT(ME);
    return true;
}

static const wld_vendorModule_handlers_cb s_Cbs = {
    .fInitCb = s_initCb,
    .fDeinitCb = s_deinitCb,
    .fLoadDefaultsCb = s_loadDefaultsCb,
};

/**
 * @brief: Global amxm vendor module call handler.
 * This function is called for all apis invoked by wld plugin side.
 * No processing is done here (only display) ,
 * but the vendor module asks lib_wld to forward call (and convert args/ret)
 * to wld vendor module internal handler.
 *
 * @return 0 for success, < 0 when error occurs
 */
static int s_globalCb(const char* const funcName, amxc_var_t* args, amxc_var_t* ret _UNUSED) {
    SAH_TRACEZ_INFO(ME, "function %s called", funcName);
    return wld_vendorModule_forwardCall(funcName, args, ret, &s_Cbs);
}

static AMXM_CONSTRUCTOR s_onLoad(void) {
    SAH_TRACEZ_IN(ME);
    SAH_TRACEZ_WARNING(ME, "Load vendor module");
    wld_vendorModule_config_t config = {
        .fGlobalCb = s_globalCb,
    };
    wld_vendorModule_register(MXL_VENDOR_NAME, &config);
    SAH_TRACEZ_OUT(ME);
    return 0;
}

static AMXM_DESTRUCTOR s_onUnload(void) {
    SAH_TRACEZ_IN(ME);
    SAH_TRACEZ_WARNING(ME, "UnLoad vendor module");
    whm_mxl_module_deInit();
    wld_vendorModule_unregister(MXL_VENDOR_NAME);
    SAH_TRACEZ_OUT(ME);
    return 0;
}
