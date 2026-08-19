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

#define FAPI_DSL_CRLF IFXOS_CRLF

extern int dsl_fapi_cli_autogen_commands_register(struct cli_core_context_s *p_core_ctx);
extern int dsl_fapi_cli_misc_commands_register(struct cli_core_context_s *p_core_ctx);
extern int dsl_fapi_cli_ext_commands_register(struct cli_core_context_s *p_core_ctx);

cli_cmd_register__file my_cli_cmds[] = {
	dsl_fapi_cli_autogen_commands_register,
	dsl_fapi_cli_misc_commands_register,
	dsl_fapi_cli_ext_commands_register,
	NULL
};

char *white_space_remove(char *str)
{
	char *buf;
	char *pRead, *pWrite;
	int i = 0;

	/* remove leading whitespaces */
	for (buf = str; buf && *buf && isspace((int)(*buf)); ++buf) {
		;
	}

	/* remove double spaces in between and at the end */
	pRead = buf;
	pWrite = buf;
	for (i = 0; pWrite && pRead && *pRead != '\0'; ++pRead) {
		if (isspace((int)(*pRead))) {
			if ((i == 0) && (*(pRead + 1) != '\0')) {
				*pWrite = *pRead;
				pWrite++;
				i++;
			}
		} else {
			i = 0;
			*pWrite = *pRead;
			pWrite++;
		}
	}

	/* Write string termination */
	if (pWrite && (pWrite != pRead))
		*pWrite = '\0';

	return buf;
}

static int cli_fapi_dsl_init(struct fapi_dsl_ctx *p_ctx, const char *p_cmd)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = FAPI_DSL_STATUS_SUCCESS;
	struct fapi_dsl_init_cfg cfg;
	FILE *fp = NULL;
	char buf[256] = { 0 };
	char *name, *value, *tokbuf, *buf_ptr;
	int names_amount, i;
	struct fapi_dsl_init_parameter dsl_param_cfg_template[] = {
		{"X_LANTIQ_COM_BitswapUs_V", NULL},
		{"X_LANTIQ_COM_BitswapUs_A", NULL},
		{"X_LANTIQ_COM_BitswapDs_V", NULL},
		{"X_LANTIQ_COM_BitswapDs_A", NULL},
		{"X_LANTIQ_COM_ReTxUs", NULL},
		{"X_LANTIQ_COM_ReTxDs_V", NULL},
		{"X_LANTIQ_COM_ReTxDs_A", NULL},
		{"X_LANTIQ_COM_SraUs_V", NULL},
		{"X_LANTIQ_COM_SraUs_A", NULL},
		{"X_LANTIQ_COM_SraDs_V", NULL},
		{"X_LANTIQ_COM_SraDs_A", NULL},
		{"X_LANTIQ_COM_VirtualNoiseUs", NULL},
		{"X_LANTIQ_COM_VirtualNoiseDs", NULL},
		{"X_LANTIQ_COM_Vectoring", NULL},
		{"X_LANTIQ_COM_XTSE", NULL},
		{"X_LANTIQ_COM_LinkEncapsulationConfig", NULL},
		{"X_LANTIQ_COM_DSLNextMode", NULL},
		{"X_LANTIQ_COM_DSLActSeq", NULL},
		{"X_LANTIQ_COM_DSLActMode", NULL},
		{"X_LANTIQ_COM_DSLRemember", NULL},
		{"X_LANTIQ_COM_EntitiesEnabled", NULL},
	};

	/* skip parse for cfg_file name missed */
	if (strlen(p_cmd)) {

		fp = fopen(p_cmd, "r");
		if (!fp) {
			printf(DSL_FAPI "fail to open config file %s\n", p_cmd);
			return -1;
		}

		names_amount = sizeof(dsl_param_cfg_template) / sizeof(struct fapi_dsl_init_parameter);

		memset(&cfg, 0x0, sizeof(struct fapi_dsl_init_cfg));
		/* parse config file */
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			buf_ptr = white_space_remove(buf);
			name = strtok_r(buf_ptr, ",", &tokbuf);
			name = white_space_remove(name);
			value = strtok_r(NULL, "", &tokbuf);
			value = white_space_remove(value);

			/* search name from config file within template table */
			for (i = 0; i < names_amount; i++) {
				/* name match */
				if (!strcasecmp(name, dsl_param_cfg_template[i].name)) {
					/* if files "value" is not empty - ignore it */
					if (dsl_param_cfg_template[i].value != NULL) {
						printf(DSL_FAPI
						       "dublicate name \"%s\", old value \"%s\" will be ignored"
						       FAPI_DSL_CRLF, name, dsl_param_cfg_template[i].value);
						free(dsl_param_cfg_template[i].value);
					}

					/* save field "value" for matched name */
					dsl_param_cfg_template[i].value = malloc(strlen(value));
					if (dsl_param_cfg_template[i].value == NULL) {
						printf(DSL_FAPI "error: could not allocate memory"
						       FAPI_DSL_CRLF);
					} else {
						memcpy(dsl_param_cfg_template[i].value, value, strlen(value));
					}
					break;
				}
			}

			if (i < names_amount) {
				printf(DSL_FAPI "name \"%s\", value \"%s\"" FAPI_DSL_CRLF, name, value);
			} else {
				printf(DSL_FAPI "unknown name \"%s\" ignored " FAPI_DSL_CRLF, name);
			}
		}

		cfg.params.array_size = names_amount;
		cfg.params.array = dsl_param_cfg_template;

		fct_ret = fapi_dsl_init(p_ctx, &cfg);

		for (i = 0; i < names_amount; i++) {
			if (dsl_param_cfg_template[i].value) {
				free(dsl_param_cfg_template[i].value);
			}
		}

		fclose(fp);
	} else {
		fct_ret = fapi_dsl_init(p_ctx, NULL);
	}

	if (fct_ret != FAPI_DSL_STATUS_SUCCESS) {
		printf(DSL_FAPI "ERROR(%d) FAPI context init failed\n", fct_ret);
		return -1;
	}

	return ret;
}

int cli_start(const bool console, int entity, char *cfg_file)
{
	int ret = 0;
	enum fapi_dsl_status fct_ret = FAPI_DSL_STATUS_SUCCESS;
	static struct cli_core_context_s *core_ctx = NULL;
# if defined(INCLUDE_CLI_PIPE_SUPPORT)
	static struct cli_pipe_context_s *pipe_ctx = NULL;
# endif
	struct fapi_dsl_ctx *fapi_ctx = NULL;

	fapi_ctx = fapi_dsl_open(entity);
	if (fapi_ctx == NULL) {
		printf(DSL_FAPI "ERROR FAPI ctx create" FAPI_DSL_CRLF);
		return -1;
	}

	fct_ret = cli_fapi_dsl_init(fapi_ctx, cfg_file);
	if (fct_ret != FAPI_DSL_STATUS_SUCCESS) {
		printf(DSL_FAPI "ERROR FAPI ctx init" FAPI_DSL_CRLF);
		ret = fapi_dsl_close(fapi_ctx);
		return -1;
	}

	ret = cli_core_setup__file(&core_ctx, (unsigned int)-1, (void *)fapi_ctx, my_cli_cmds);

	if (ret != 0) {
		printf(DSL_FAPI "ERROR(%d) CLI init failed" FAPI_DSL_CRLF, ret);
	}
#if defined(INCLUDE_CLI_PIPE_SUPPORT)
	ret = cli_pipe_init(core_ctx, DSL_FAPI_MAX_CLI_PIPES, DSL_FAPI_PIPE_NAME, &pipe_ctx);
	if (ret != 0) {
		printf(DSL_FAPI "ERROR(%d) Pipe init failed" FAPI_DSL_CRLF, ret);
		return ret;
	}
#endif

	if (console == true) {
		/* run console */
		ret = cli_console_run(core_ctx, NULL, NULL);
		if (ret != 0) {
			printf(DSL_FAPI "ERROR(%d) CLI Console init failed" FAPI_DSL_CRLF, ret);
			return ret;
		}
	} else {
		/* start dummy interface to wait for quit */
		ret = cli_dummy_if_start(core_ctx, 1000);
		if (ret != 0) {
			printf(DSL_FAPI "CLI will be released..." FAPI_DSL_CRLF);
		}
	}

#ifdef INCLUDE_CLI_PIPE_SUPPORT
	ret = cli_pipe_release(core_ctx, &pipe_ctx);
	if (ret != 0) {
		printf(DSL_FAPI "ERROR(%d) CLI pipe release failed" FAPI_DSL_CRLF, ret);
		return ret;
	}
#endif
	ret = cli_core_release(&core_ctx, cli_cmd_core_out_mode_file);

	fct_ret = fapi_dsl_uninit(fapi_ctx);
	if (fct_ret != FAPI_DSL_STATUS_SUCCESS) {
		printf(DSL_FAPI "ERROR FAPI ctx uninit" FAPI_DSL_CRLF);
		ret = -1;
	}

	fct_ret = fapi_dsl_close(fapi_ctx);
	if (fct_ret != FAPI_DSL_STATUS_SUCCESS) {
		printf(DSL_FAPI "ERROR FAPI ctx close" FAPI_DSL_CRLF);
		ret = -1;
	}

	return ret;
}
