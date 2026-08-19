/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
   \file pon_adapter_debug.h
   This file holds the internal debug interface definitions.
*/

#ifndef _PON_ADAPTER_DEBUG_H_
#define _PON_ADAPTER_DEBUG_H_

#ifndef PA_DEBUG_LVL
#define PA_DEBUG_LVL 0
#endif

#define PA_DBG_PRINT(fmt, ...) \
	do { if (PA_DEBUG_LVL) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** \addtogroup PON_ADAPTER_DEBUG
 *
 *  @{
 */

/** PON Adapter debug levels */
enum pon_adapter_dbg {
	/** Verbose outputs */
	PA_MSG = 0,
	/** Prints */
	PA_PRN = 1,
	/** Warnings */
	PA_WRN = 2,
	/** Errors */
	PA_ERR = 3,
	/** No output */
	PA_OFF = 4
};

extern enum pon_adapter_dbg dbg_module_level;

/** @} */ /* PON_ADAPTER_DEBUG */
/** @} */ /* PON_ADAPTER */

#endif
