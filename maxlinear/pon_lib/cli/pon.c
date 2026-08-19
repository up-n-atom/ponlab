/******************************************************************************
 *
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "fapi_pon_os.h"
#include "lib_cli_config.h"
#include "pon_cli.h"
#include "fapi_pon.h"
#include "fapi_pon_error.h"

#ifdef EXTRA_VERSION
#define pon_extra_ver_str "." EXTRA_VERSION
#else
#define pon_extra_ver_str ""
#endif

/** what string support, version string */
const char pon_whatversion[] = "@(#)MaxLinear PON library CLI, version "
	PACKAGE_VERSION pon_extra_ver_str;

static cli_cmd_register__file my_cli_cmds[] = {
	pon_cli_cmd_register,
	pon_ext_cli_cmd_register,
	0
};

static struct cli_core_context_s *p_glb_core_ctx;

int main(int argc, char *argv[])
{
	int retval = 0;
	int i = 0;
	struct pon_ctx *pon_context_cli;
	errno_t ret;

	if (fapi_pon_open(&pon_context_cli) != 0)
		return EXIT_FAILURE;

	retval = cli_core_setup__file(&p_glb_core_ctx,
				      (unsigned int)-3,
				      pon_context_cli,
				      my_cli_cmds);

	if (argc == 1) {
		char help_cmd[5];

		/* a terminating null is appended */
		if (strncpy_s(help_cmd, sizeof(help_cmd), "help", 4)) {
			fprintf(stderr, "%s: strncpy_s failed\n", __func__);
			return PON_STATUS_INPUT_ERR;
		}
		retval = cli_core_cmd_arg_exec__file(p_glb_core_ctx,
						     help_cmd,
						     0,
						     stdout);
	} else if (argc == 2) {
		retval = cli_core_cmd_arg_exec__file(p_glb_core_ctx,
						     argv[1],
						     0,
						     stdout);
	} else {
		char *prompt_line;
		unsigned int prompt_line_length = 0, maxp;

		for (i = 2; i < argc; i++)
			prompt_line_length += strnlen_s(argv[i],
							RSIZE_MAX_STR) + 1;
		maxp = (prompt_line_length + 2) * sizeof(char);
		prompt_line = malloc(maxp);
		if (!prompt_line)
			return EXIT_FAILURE;
		prompt_line[0] = '\0';

		for (i = 2; i < argc; i++) {
			ret = strncat_s(prompt_line, maxp,
				  " ", 1);
			if (ret) {
				fprintf(stderr, "%s: strncat_s failed\n",
					__func__);
				free(prompt_line);
				fapi_pon_close(pon_context_cli);
				return PON_STATUS_INPUT_ERR;
			}
			prompt_line_length--;
			ret = strncat_s(prompt_line, maxp,
				  argv[i], prompt_line_length);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr, "%s: strncat_s failed\n",
					__func__);
				free(prompt_line);
				fapi_pon_close(pon_context_cli);
				return PON_STATUS_INPUT_ERR;
			}
			prompt_line_length -= strnlen_s(argv[i], RSIZE_MAX_STR);
		}
		retval = cli_core_cmd_arg_exec__file(p_glb_core_ctx,
						     argv[1],
						     prompt_line + 1,
						     stdout);
		free(prompt_line);
	}

	retval = cli_core_release(&p_glb_core_ctx,
				  cli_cmd_core_out_mode_file);
	fapi_pon_close(pon_context_cli);

	return retval;
}
