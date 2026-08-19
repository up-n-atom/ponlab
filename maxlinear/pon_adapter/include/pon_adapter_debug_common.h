/******************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_debug_common.h
 * This file holds the debug interface definitions.
 */

/*
 * EXAMPLES:
 *
 * 1) Printing debug messages:
 *         int foo(int a, int *b) {
 *                 dbg_err("This is an error message\n");
 *                 dbg_wrn("This is a warning message\n");
 *                 dbg_prn("This is just a print\n");
 *                 dbg_msg("This is just a message\n");
 *                 return 0;
 *         }
 * Output:
 *         [DEBUG_MODULE] ERR in foo():57 - This is an error message
 *         [DEBUG_MODULE] WRN in foo():58 - This is a warning message
 *         [DEBUG_MODULE] PRN in foo():59 - This is just a print
 *         [DEBUG_MODULE] MSG in foo():60 - This is just a message
 *
 * 2) Log function call:
 *         int foo(int a, int *b) {
 *                 dbg_in();
 *                 dbg_out();
 *                 return 0;
 *         }
 * Output:
 *         [DEBUG_MODULE] MSG in foo():57 - called
 *         [DEBUG_MODULE] MSG in foo():58 - return
 *
 * 3) Same, but with arguments and with return value:
 *         int foo(int a, int *b) {
 *                 dbg_in_args("%d, %p", a, b);
 *                 dbg_out_ret("%d", 0);
 *                 return 0;
 *         }
 * Output:
 *         [DEBUG_MODULE] MSG in foo():57 - called with (0, 0xAF123465AFAFAF)
 *         [DEBUG_MODULE] MSG in foo():58 - return 0
 *
 * 4) Logging failed function call:
 *         int foo(int a, int *b) {
 *                 if (some_other_function()) {
 *                         dbg_err_fn(some_other_function);
 *                         return 1;
 *                 }
 *                 return 0;
 *         }
 * Output:
 *         [DEBUG_MODULE] ERR in foo():57 - some_other_function() failed
 *
 * 5) Logging failed function call with return value
 *         int foo(int a, int *b) {
 *                 int err;
 *                 err = some_other_function();
 *                 if (err) {
 *                         dbg_err_fn_ret(some_other_function, err);
 *                         return 1;
 *                 }
 *                 return 0;
 *         }
 * Output:
 *         [DEBUG_MODULE] ERR in foo():57 - some_other_function() failed with -1
 */

#ifndef _PON_ADAPTER_DEBUG_COMMON_H_
#define _PON_ADAPTER_DEBUG_COMMON_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(x)
#endif

#define DEBUG_MODULE_NAME_GET(str) _CONCAT(DEBUG_MODULE, str)
#define _CONCAT(a, b) CONCAT(a, b)
#define CONCAT(a, b) a ## b
#define _DBG_PREFIX [DEBUG_MODULE]
#define DBG_PREFIX DBG_PREFIX_STR(_DBG_PREFIX)
#define DBG_PREFIX_STR(x) _DBG_PREFIX_STR(x)
#define _DBG_PREFIX_STR(x) #x

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_DEBUG PON Adapter Debug Functions
 *
 *  Common debug functions used for PON Adapter software analysis.
 *
 *  @{
 */

/** Debug level definition */
enum dbg_lvl {
	/** Verbose outputs, prints, warnings and errors */
	DBG_MSG = 0,
	/** Prints, warnings and errors */
	DBG_PRN = 1,
	/** Warnings and errors */
	DBG_WRN = 2,
	/** Errors */
	DBG_ERR = 3,
	/** No output */
	DBG_OFF = 4,
	/** Number of debug levels */
	DBG_COUNT
};

/** Read the debug level module */
extern uint8_t DEBUG_MODULE_NAME_GET(_dbg_lvl);

/** Read the debug level operations */
extern struct pa_ll_dbg_lvl_ops DEBUG_MODULE_NAME_GET(_dbg_lvl_ops);

/** Verbose debug level description */
static const char *const DEBUG_MODULE_NAME_GET(_dbg_lvl_names[DBG_COUNT]) = {
	/** Message level */
	[DBG_MSG] = "MSG",
	/** Print level */
	[DBG_PRN] = "PRN",
	/** Warning level */
	[DBG_WRN] = "WRN",
	/** Error level */
	[DBG_ERR] = "ERR",
	/** No level */
	[DBG_OFF] = ""
};

/** */
static inline __attribute__ ((format (printf, 2, 0)))
void dbg_vprintf(enum dbg_lvl lvl, const char *fmt, va_list args)
{
#ifndef DEBUG
	if (DEBUG_MODULE_NAME_GET(_dbg_lvl) > (uint8_t)lvl)
		return;
#endif
	vfprintf(stderr, fmt, args);
}

/** Print formatted debug information */
static inline __attribute__ ((format (printf, 2, 3)))
void dbg_printf(enum dbg_lvl lvl, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	dbg_vprintf(lvl, fmt, args);
	va_end(args);
}

/** Print unformatted debug information */
static inline __attribute__ ((format (printf, 2, 3)))
void dbg_naked(enum dbg_lvl lvl, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	dbg_printf(lvl, "%s ", DBG_PREFIX);
	dbg_vprintf(lvl, fmt, args);
	va_end(args);
}

/** */
static inline __attribute__ ((format (printf, 5, 0)))
void vdbg(enum dbg_lvl lvl,
	  const char *prefix,
	  const char *function,
	  int line,
	  const char *fmt,
	  va_list args)
{
	dbg_printf(lvl, "%s ", prefix);
	dbg_printf(lvl, "%s in %s():%d - ", DEBUG_MODULE_NAME_GET(_dbg_lvl_names[lvl]),
		   function, line);
	dbg_vprintf(lvl, fmt, args);
}


/** */
static inline __attribute__ ((format (printf, 5, 6)))
void _dbg(enum dbg_lvl lvl,
	  const char *prefix,
	  const char *function,
	  int line,
	  const char *fmt,
	  ...)
{
	va_list args;

	va_start(args, fmt);
	vdbg(lvl, prefix, function, line, fmt, args);
	va_end(args);
}

/** Basic debug macro used to build other debug macros */
#define dbg(lvl, fmt, ...) _dbg(lvl, DBG_PREFIX, __func__, __LINE__, \
				fmt, ## __VA_ARGS__)
/** Use this macro to indicate an error */
#define dbg_err(fmt, ...) dbg(DBG_ERR, fmt, ## __VA_ARGS__)
/** Use this macro to indicate a warning */
#define dbg_wrn(fmt, ...) dbg(DBG_WRN, fmt, ## __VA_ARGS__)
/** Use this macro to just print an information */
#define dbg_prn(fmt, ...) dbg(DBG_PRN, fmt, ## __VA_ARGS__)
/** Use this macro to just print an information */
#define dbg_msg(fmt, ...) dbg(DBG_MSG, fmt, ## __VA_ARGS__)

/** Use this at the beginning of a function to indicate that it was called */
#define dbg_in() dbg_msg("called\n")
/** Use this instead dbg_in() if you want to print that function was called
 *  with certain arguments
 */
#define dbg_in_args(fmt, ...) \
	dbg_msg("called with (" fmt ")\n", ## __VA_ARGS__)
/** Use it at the end of a function to indicate that the function returned */
#define dbg_out() dbg_msg("return\n")
/** Same as dbg_out, but has the ability to print returned value */
#define dbg_out_ret(fmt, arg) dbg_msg("return " fmt "\n", arg)

/** Use it to indicate that some function call failed. Prints as an error */
#define dbg_err_fn(fn) dbg_err("%s() failed\n", #fn)
/** Use it to indicate that some function call failed. Prints as an error */
#define dbg_err_fn_ret(fn, ret) dbg_err("%s() failed with %d\n", #fn, ret)
/** Use it to indicate that some function call failed. Prints as a warning */
#define dbg_wrn_fn(fn) dbg_wrn("%s() failed\n", #fn)
/** Use it to indicate that some function call failed. Prints as a warning */
#define dbg_wrn_fn_ret(fn, ret) dbg_wrn("%s() failed with %d\n", #fn, ret)
/** Use it to indicate that some function call failed. Prints as a print */
#define dbg_prn_fn(fn, ret) dbg_prn("%s() failed with %d\n", ret)
/** Use it to indicate that some function call failed. Prints as a print */
#define dbg_prn_fn_ret(fn, ret) dbg_prn("%s() failed with %d\n", #fn, ret)
/** Use it to indicate that some function call failed. Prints as a message */
#define dbg_msg_fn(fn, ret) dbg_msg("%s() failed with %d\n", ret)
/** Use it to indicate that some function call failed. Prints as a message */
#define dbg_msg_fn_ret(fn, ret) dbg_msg("%s() failed with %d\n", #fn, ret)

#endif

/** @} */ /* PON_ADAPTER_DEBUG */

/** @} */ /* PON_ADAPTER */
