/******************************************************************************
 *
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include "pon_adapter.h"
#include "pon_adapter_debug.h"

#ifdef EXTRA_VERSION
#define pon_adapter_extra_ver_str "." EXTRA_VERSION
#else
#define pon_adapter_extra_ver_str ""
#endif

#ifdef PACKAGE_VERSION
/** what string support, version string */
const char pon_adapter_whatversion[] = "@(#)MaxLinear PON Adapter, version "
					PACKAGE_VERSION
					pon_adapter_extra_ver_str;
#endif

/** \addtogroup PON_ADAPTER
 *
 * @{
 */

enum pon_adapter_dbg dbg_module_level = PA_ERR;

void pa_dbg_level_set(const enum pon_adapter_dbg level)
{
	dbg_module_level = level;
}

enum pon_adapter_dbg pa_dbg_level_get(void)
{
	return dbg_module_level;
}

/** @} */
