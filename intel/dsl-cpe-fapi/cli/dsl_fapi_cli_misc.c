/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include "stddef.h"
#include "stdlib.h"
#include "dsl_fapi.h"
#include "dsl_fapi_cli.h"
#include "ifx_types.h"
#include "ifxos_common.h"
#include "ifxos_print.h"

#ifdef INCLUDE_CLI_SUPPORT

#define EMPTY " "
#include "lib_cli_config.h"
#include "lib_cli_core.h"

#define FAPI_DSL_CRLF IFXOS_CRLF

/** \addtogroup FAPI_DSL_CLI_COMMANDS
   @{
*/

/** Handle command

   \param[in] p_ctx     FAPI_DSL context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_dsl_version_get(struct fapi_dsl_ctx *p_ctx, const char *p_cmd, clios_file_io_t * p_out)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = (enum fapi_dsl_status)0;

#ifndef FAPI_DSL_DEBUG_DISABLE
	static const char usage[] =
	    "Long Form: version_information_get" FAPI_DSL_CRLF
	    "Short Form: vig" FAPI_DSL_CRLF
	    FAPI_DSL_CRLF
	    "Output Parameter" FAPI_DSL_CRLF
	    "- enum fapi_dsl_status status" FAPI_DSL_CRLF
	    "- char fapi_dsl_version" FAPI_DSL_CRLF FAPI_DSL_CRLF;
#else
#undef usage
#define usage ""
#endif

	if ((ret = cli_check_help__file(p_cmd, usage, p_out)) != 0) {
		return ret;
	}

	return IFXOS_FPrintf(p_out, "status=%d dsl_fapi_version=\"%s\" " FAPI_DSL_CRLF,
			     (int)fct_ret, &dsl_fapi_version[4]);
}

/** Register misc commands */
int dsl_fapi_cli_misc_commands_register(struct cli_core_context_s *p_core_ctx)
{
	unsigned int group_mask = 0;
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "vig", "version_information_get",
				     (cli_cmd_user_fct_file_t) cli_fapi_dsl_version_get);
	return IFX_SUCCESS;
}

/*! @} */

#endif
