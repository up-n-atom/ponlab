/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include "dsl_fapi_cli.h"
#include "ifx_types.h"
#include "ifxos_common.h"
#include "ifxos_print.h"
#include "stddef.h"
#include "lib_cli_config.h"
#include "lib_cli_core.h"

#define FAPI_DSL_CRLF IFXOS_CRLF

#ifdef INCLUDE_CLI_SUPPORT

/** \addtogroup FAPI_DSL_CLI_COMMANDS
   @{
*/

#include "dsl_fapi_cli_ext_autogen.c"

static int cli_fapi_dsl_get(struct fapi_dsl_ctx *p_ctx, const char *p_cmd, clios_file_io_t * p_out)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = (enum fapi_dsl_status)0;
	int size = sizeof(fapi_dsl_template) / sizeof(fapi_dsl_template[0]);
	int i, j;
	char obj_name[64], param_name[64];
	char tmp[2048] = { 0 };

#ifndef FAPI_DSL_DEBUG_DISABLE
	static const char usage[] =
	    "Long Form: get" FAPI_DSL_CRLF
	    "Short Form: get_object_parameter" FAPI_DSL_CRLF
	    FAPI_DSL_CRLF
	    "Input Parameter" FAPI_DSL_CRLF
	    "- char object name[64]" FAPI_DSL_CRLF
	    "- char parameter name[64]" FAPI_DSL_CRLF
	    "Output Parameter" FAPI_DSL_CRLF
	    "- enum fapi_dsl_status status" FAPI_DSL_CRLF
	    "- char parameter value[]" FAPI_DSL_CRLF FAPI_DSL_CRLF;
#else
#undef usage
#define usage ""
#endif

	if ((ret = cli_check_help__file(p_cmd, usage, p_out)) != 0) {
		return ret;
	}

	ret = cli_sscanf(p_cmd, "%s %s", obj_name, param_name);

	if (ret != 2)
		return cli_check_help__file("-h", usage, p_out);

	for (i = 0; i < size; i++) {
		if (!strcasecmp(fapi_dsl_template[i].obj_name, obj_name)) {
			j = 0;
			while (fapi_dsl_template[i].template[j].param_name != NULL) {
				if (!strcasecmp(fapi_dsl_template[i].template[j].param_name, param_name)) {
					fct_ret =
					    fapi_dsl_template[i].get_handler(p_ctx, fapi_dsl_template[i].obj);
					fapi_dsl_template[i].template[j].print_handler(tmp, (char *)
										       fapi_dsl_template
										       [i].obj +
										       fapi_dsl_template
										       [i].template[j].
										       offset);

					return IFXOS_FPrintf(p_out, "status=%d %s=%s" FAPI_DSL_CRLF,
							     (int)fct_ret, param_name, tmp);
					break;
				}
				j++;
			}

			fct_ret = FAPI_DSL_STATUS_ERROR;
			return IFXOS_FPrintf(p_out,
					     "status=%d unknown parameter %s for object %s" FAPI_DSL_CRLF,
					     (int)fct_ret, param_name, obj_name);
		}
	}

	fct_ret = FAPI_DSL_STATUS_ERROR;
	return IFXOS_FPrintf(p_out, "status=%d unknown object %s" FAPI_DSL_CRLF, (int)fct_ret, obj_name);
}

static int cli_fapi_dsl_set(struct fapi_dsl_ctx *p_ctx, const char *p_cmd, clios_file_io_t * p_out)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = (enum fapi_dsl_status)0;
	int size = sizeof(fapi_dsl_template) / sizeof(fapi_dsl_template[0]);
	int i, j;
	char tmp[2048] = { 0 };
	char *tokbuf, *buf_ptr;
	char *obj_name, *param_name, *scanf_options;

#ifndef FAPI_DSL_DEBUG_DISABLE
	static const char usage[] =
	    "Long Form: set" FAPI_DSL_CRLF
	    "Short Form: set_object_parameter" FAPI_DSL_CRLF
	    FAPI_DSL_CRLF
	    "Input Parameter" FAPI_DSL_CRLF
	    "- char object name[64" FAPI_DSL_CRLF
	    "- char parameter name[64]" FAPI_DSL_CRLF
	    "- char scanf parameters[64]" FAPI_DSL_CRLF
	    "Output Parameter" FAPI_DSL_CRLF
	    "- enum fapi_dsl_status status" FAPI_DSL_CRLF
	    "- char parameter value[]" FAPI_DSL_CRLF FAPI_DSL_CRLF;
#else
#undef usage
#define usage ""
#endif

	if ((ret = cli_check_help__file(p_cmd, usage, p_out)) != 0) {
		return ret;
	}

	buf_ptr = (char *)p_cmd;
	obj_name = strtok_r((char *)p_cmd, " \t", &tokbuf);
	param_name = strtok_r(NULL, " \t", &tokbuf);
	scanf_options = tokbuf;

	if (obj_name == NULL) {
		return cli_check_help__file("-h", usage, p_out);
	}

	if (param_name == NULL) {
		return cli_check_help__file("-h", usage, p_out);
	}

	for (i = 0; i < size; i++) {
		if (!strcasecmp(fapi_dsl_template[i].obj_name, obj_name)) {
			j = 0;
			while (fapi_dsl_template[i].template[j].param_name != NULL) {
				if (!strcasecmp(fapi_dsl_template[i].template[j].param_name, param_name)) {
					fct_ret =
					    fapi_dsl_template[i].get_handler(p_ctx, fapi_dsl_template[i].obj);
					ret =
					    fapi_dsl_template[i].template[j].scan_handler(scanf_options,
											  (char *)
											  fapi_dsl_template
											  [i].obj +
											  fapi_dsl_template
											  [i].
											  template[j].offset);

					/* todo: investigate return value for cli_sscanf (for array case)
					   if (ret == -1) {
					   return IFXOS_FPrintf(p_out, "status=%d uncorrect options %s" FAPI_DSL_CRLF,
					   (int)fct_ret, scanf_options);
					   }
					 */

					fapi_dsl_template[i].template[j].print_handler(tmp, (char *)
										       fapi_dsl_template
										       [i].obj +
										       fapi_dsl_template
										       [i].template[j].
										       offset);

					fct_ret =
					    fapi_dsl_template[i].set_handler(p_ctx, fapi_dsl_template[i].obj);

					return IFXOS_FPrintf(p_out, "status=%d %s=%s" FAPI_DSL_CRLF,
							     (int)fct_ret, param_name, tmp);
					break;
				}
				j++;
			}

			fct_ret = FAPI_DSL_STATUS_ERROR;
			return IFXOS_FPrintf(p_out,
					     "status=%d unknown parameter %s for object %s" FAPI_DSL_CRLF,
					     (int)fct_ret, param_name, obj_name);
		}
	}

	fct_ret = FAPI_DSL_STATUS_ERROR;
	return IFXOS_FPrintf(p_out, "status=%d unknown object %s" FAPI_DSL_CRLF, (int)fct_ret, obj_name);
}

static int cli_fapi_dsl_info(struct fapi_dsl_ctx *p_ctx, const char *p_cmd, clios_file_io_t * p_out)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = (enum fapi_dsl_status)0;
	int size = sizeof(fapi_dsl_template) / sizeof(fapi_dsl_template[0]);
	int i, j;
	char obj_name[64];

#ifndef FAPI_DSL_DEBUG_DISABLE
	static const char usage[] =
	    "Long Form: info" FAPI_DSL_CRLF
	    "Short Form: info_object_parameter" FAPI_DSL_CRLF
	    FAPI_DSL_CRLF
	    "Input Parameter" FAPI_DSL_CRLF
	    "- char object name[64] or empty for available objects" FAPI_DSL_CRLF
	    "Output Parameter" FAPI_DSL_CRLF
	    "- enum fapi_dsl_status status" FAPI_DSL_CRLF "- char info" FAPI_DSL_CRLF FAPI_DSL_CRLF;
#else
#undef usage
#define usage ""
#endif

	if ((ret = cli_check_help__file(p_cmd, usage, p_out)) != 0) {
		return ret;
	}

	ret = cli_sscanf(p_cmd, "%s", obj_name);

	if (ret > 1)
		return cli_check_help__file("-h", usage, p_out);

	if (ret == 0) {
		IFXOS_FPrintf(p_out, "status=%d, available objects:" FAPI_DSL_CRLF FAPI_DSL_CRLF,
			      (int)fct_ret);
		for (i = 0; i < size; i++) {
			IFXOS_FPrintf(p_out, "%s" FAPI_DSL_CRLF, fapi_dsl_template[i].obj_name);
		}
		return 0;
	}

	for (i = 0; i < size; i++) {
		if (!strcasecmp(fapi_dsl_template[i].obj_name, obj_name)) {
			j = 0;
			IFXOS_FPrintf(p_out, "status=%d, available parameters for object %s"
				      FAPI_DSL_CRLF FAPI_DSL_CRLF, (int)fct_ret, obj_name);
			while (fapi_dsl_template[i].template[j].param_name != NULL) {
				IFXOS_FPrintf(p_out, "%s" FAPI_DSL_CRLF,
					      fapi_dsl_template[i].template[j].info);
				j++;
			}
			return 0;
		}
	}

	IFXOS_FPrintf(p_out, "status=%d, unknown object %s"
		      FAPI_DSL_CRLF FAPI_DSL_CRLF, (int)fct_ret, obj_name);

	return 0;
}

/** Register misc commands */
int dsl_fapi_cli_ext_commands_register(struct cli_core_context_s *p_core_ctx)
{
	unsigned int group_mask = 0;
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "get", "get_object_parameter",
				     (cli_cmd_user_fct_file_t) cli_fapi_dsl_get);
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "set", "set_object_parameter",
				     (cli_cmd_user_fct_file_t) cli_fapi_dsl_set);
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "info", "info_object_parameter",
				     (cli_cmd_user_fct_file_t) cli_fapi_dsl_info);
	return IFX_SUCCESS;
}

/*! @} */

#endif
