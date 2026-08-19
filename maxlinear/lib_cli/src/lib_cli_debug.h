#ifndef _LIB_CLI_DEBUG_H
#define _LIB_CLI_DEBUG_H
/****************************************************************************

                              Copyright (c) 2010
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/

/** \file
   Command Line Interface (CLI) - Debug defines.
*/
#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   Global Includes
   ========================================================================= */
#include "ifx_types.h"
#include "lib_cli_config.h"
#include "lib_cli_osmap.h"

#if (   !defined(IFXOS_HAVE_PRINT) \
     || (defined(IFXOS_HAVE_PRINT) && (IFXOS_HAVE_PRINT == 0)) )
#	ifdef CLI_DEBUG_PRINTOUT
#		undef CLI_DEBUG_PRINTOUT
#	endif

#	ifdef CLI_ERROR_PRINTOUT
#		undef CLI_ERROR_PRINTOUT
#	endif

#	define CLI_DEBUG_PRINTOUT  0
#	define CLI_ERROR_PRINTOUT  0
#endif      /* #if ( !defined(IFXOS_HAVE_PRINT) || ... ) */


/* ============================================================================
   MEI3 Driver Printout - Printout Levels
   ========================================================================= */

#define CLI_PRN_OFF	4
#define CLI_PRN_HIGH	3
#define CLI_PRN_NORMAL	2
#define CLI_PRN_LOW	1

#define CLI_PRN_ERR	CLI_PRN_HIGH
#define CLI_PRN_WRN	CLI_PRN_NORMAL

/* ============================================================================
   MEI3 Driver Printout - Macros for Printout Level Handling
   ========================================================================= */
#if ((CLI_DEBUG_PRINTOUT == 1) || (CLI_ERROR_PRINTOUT == 1))

/** PRINTOUT - Declare a print level variables for a given module. */
#define CLI_PRN_MODULE_DECL(moduel_name) \
	/*lint -e{752}*/ \
	extern volatile IFX_uint32_t cli_prn_usr_module_##moduel_name

/** PRINTOUT - Create a print level variables for a given module. */
#define CLI_PRN_MODULE_CREATE(moduel_name, init_level) \
	volatile IFX_uint32_t cli_prn_usr_module_##moduel_name = init_level


/** DEBUG PRINTOUT - Set the debug level (user print) for a given module. */
#define CLI_PRN_USR_LEVEL_SET(moduel_name, new_level) \
	{ cli_prn_usr_module_##moduel_name = (IFX_uint32_t)\
	   ((new_level>IVE_PRN_OFF) ?  IVE_PRN_OFF : \
	    ((new_level<IVE_PRN_LOW) ? IVE_PRN_OFF :  new_level)); }

/** DEBUG PRINTOUT - Get the debug level (user print) for a given module. */
#define CLI_PRN_USR_LEVEL_GET(moduel_name)  cli_prn_usr_module_##moduel_name

#else  /* #if (CLI_DEBUG_PRINTOUT == 1) || (CLI_ERROR_PRINTOUT == 1) */

#define CLI_PRN_MODULE_DECL(moduel_name)  /*lint -e{19} */
#define CLI_PRN_MODULE_CREATE(moduel_name, init_level)  /*lint -e{19} */
#define CLI_PRN_USR_LEVEL_SET(moduel_name, new_level)  /*lint -e{19} */
#define CLI_PRN_USR_LEVEL_GET(moduel_name)  IVE_PRN_OFF

#endif  /* #if (CLI_DEBUG_PRINTOUT == 1) || (CLI_ERROR_PRINTOUT == 1)  */

#if (CLI_DEBUG_PRINTOUT == 1)

/** Debug printout - triggered from user call */
#define CLI_USR_DBG(module_name, dbg_level, print_message) \
	/*lint -e{717}*/ \
	do { \
		if (dbg_level >= cli_prn_usr_module_##module_name) \
		{ (void) CLIOS_PRINT_USR_DBG print_message ; }\
} while(0)

#else

#define CLI_USR_DBG(module_name, dbg_level, print_message)  /*lint -e{19} */

#endif  /* #if (CLI_DEBUG_PRINTOUT == 1) */


#if (CLI_ERROR_PRINTOUT == 1)

/** ERROR printout - triggered from user call */
#define CLI_USR_ERR(module_name, print_message) \
	/*lint -e{717}*/ \
	do { \
		if(CLI_PRN_ERR >= cli_prn_usr_module_##module_name) \
		{ \
			(void) CLIOS_PRINT_USR_ERR print_message ; \
		} \
	} while(0)

#else
#define CLI_USR_ERR(module_name, print_message)  /*lint -e{19} */
#endif      /* #if (CLI_ERROR_PRINTOUT == 1) */


CLI_PRN_MODULE_DECL(LIB_CLI);

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef _LIB_CLI_DEBUG_H */

