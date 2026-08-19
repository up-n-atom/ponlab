/***************************************************************************

 * Copyright © 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2017-2018 Intel Corporation
 * Copyright (C) 2010-2016 Lantiq Beteiligungs-GmbH & Co. KG
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 * Author  : Purnendu Ghosh
 * 
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 ***************************************************************************/

#ifndef _qos_hal_debug_h
#define _qos_hal_debug_h

#if defined(_DEBUG)
/** enable debug printouts */
#  define INCLUDE_DEBUG_SUPPORT
#endif
#  define INCLUDE_DEBUG_SUPPORT
void qos_hal_proc_destroy(void);
int qos_hal_proc_create(void);

extern uint32_t g_qos_dbg;

#define QOS_DEBUG_ERR (1<<0)
#define QOS_DEBUG_TRACE (1<<1)
#define QOS_DEBUG_HIGH (1<<2)
#define QOS_ENABLE_ALL_DEBUG (~0)

#if defined(INCLUDE_DEBUG_SUPPORT)
#undef QOS_HAL_DEBUG_MSG
#define QOS_HAL_DEBUG_MSG(level, format, arg...) do { \
	if ((g_qos_dbg & level)) { \
		printk(KERN_ERR format, ##arg); \
	} \
} while (0)
#else
#undef QOS_HAL_DEBUG_MSG
#define QOS_HAL_DEBUG_MSG(level, format, arg...)
#endif

#define QOS_HAL_DEBUG_ERR QOS_HAL_DEBUG_MSG

/** QOS HAL Debug Levels */
enum qos_hal_debug_level {
	/** Message */
	QOS_HAL_DBG_MSG,
	/** Warning */
	QOS_HAL_DBG_WRN,
	/** Error */
	QOS_HAL_DBG_ERR,
	/** Off */
	QOS_HAL_DBG_OFF
};
extern enum qos_hal_debug_level qos_hal_debug_lvl;

/** Debug message prefix */
#  define DEBUG_PREFIX        "[qos hal]"

#if defined(WIN32)
#  define QOS_HAL_CRLF  "\r\n"
#else
#  define QOS_HAL_CRLF  "\n"
#endif

#ifndef STATIC
#define STATIC static
#endif

#endif
