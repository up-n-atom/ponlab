/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *  Copyright (c) 2016 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
/**
 *  \file
 *  This file holds the internal PON library debug interface definitions.
 */

#ifndef _FAPI_PON_DEBUG_H_
#define _FAPI_PON_DEBUG_H_

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */

/** \defgroup PON_FAPI_DEBUG PON Library Core Debug Interface
 *
 *  This chapter describes the internal debug interface.
 *
 *  @{
 */

#if defined(WIN32)
#  define PON_CRLF  "\r\n"
#else
#  define PON_CRLF  "\n"
#endif

#undef IFXOS_CRLF
#define IFXOS_CRLF PON_CRLF

#if defined(_DEBUG)
/** Enable debug printouts */
#  define INCLUDE_DEBUG_SUPPORT
#endif

/** Debug message prefix */
#  define DEBUG_PREFIX        "[ponlib]"

#ifdef INCLUDE_DEBUG_SUPPORT
/** Debug level */
extern enum pon_debug_level pon_dbg_lvl;

#if !defined(__GNUC__) && !defined(__attribute__)
#  define __attribute__(x)
#endif

uint32_t pon_debug_print(const enum pon_debug_level level,
	const char *format, ...) __attribute__ ((format (printf, 2, 3)));

#define PON_DEBUG_ERR(fmt, ...) \
	pon_debug_print(PON_DBG_ERR, "%s %s: " fmt, \
			DEBUG_PREFIX, __func__,  ##__VA_ARGS__)
#define PON_DEBUG_WRN(fmt, ...) \
	pon_debug_print(PON_DBG_WRN, "%s %s: " fmt, \
			DEBUG_PREFIX, __func__, ##__VA_ARGS__)
#define PON_DEBUG_MSG(fmt, ...) \
	pon_debug_print(PON_DBG_MSG, "%s %s: " fmt, \
			DEBUG_PREFIX, __func__, ##__VA_ARGS__)

#else

#define PON_DEBUG_ERR(fmt, ...)		do {} while (0)
#define PON_DEBUG_WRN(fmt, ...)		do {} while (0)
#define PON_DEBUG_MSG(fmt, ...)		do {} while (0)

#endif				/* INCLUDE_DEBUG_SUPPORT */

/*! @} */

/*! @} */

#endif
