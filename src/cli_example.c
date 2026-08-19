/******************************************************************************

                              Copyright (c) 2010
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   Command Line Interface (CLI) - Example.

   Overview
   The CLI library provides the following feature blocks:
   # CLI core context
     - Control data (lock, ...)
     - Command core, all registerd user CLI commands in form of a binary tree.
     - List of registered user interfaces (CLI to user direction: exit, event and dump actions).


   Attention:
   The user CLI commands are registered in 2 steps:
   - count commands --> calculate memory space
   - register commands


*/

#include "ifx_types.h"
#include "lib_cli_config.h"
#include "lib_cli_osmap.h"
#include "lib_cli_core.h"
#include "lib_cli_console.h"
#include "lib_cli_pipe.h"

struct dummy_cli_fct_data_s
{
	int index;
	char const *p_description;
};

static struct dummy_cli_fct_data_s dummy_cli_fct_data[4] =
{
	{0, "user data 0"},
	{1, "user data 1"},
	{2, "user data A"},
	{3, "user data B"}
};

#if (CLI_SUPPORT_FILE_OUT == 1)
static int dummy_my_function_0__file(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_file)
{
	int ret = 0;
#ifndef CLI_DEBUG_DISABLE
	static const char USAGE[] =
		"Long Form: my_function_0" CLI_CRLF "Short Form: mf0" CLI_CRLF
		CLI_CRLF
		"Input Parameter" CLI_CRLF
		CLI_CRLF
		"Output Parameter" CLI_CRLF
		"- int errorcode" CLI_CRLF CLI_CRLF;
#else
#undef USAGE
#define USAGE ""
#endif
	if ((ret = cli_check_help__file(p_cmd, USAGE, p_file)) > 0)
		return ret;

	ret = clios_fprintf(p_file,
		"Dummy CLI Call 0" CLI_CRLF);

	return ret;
}

static int dummy_my_function_1__file(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_file)
{
	int ret = 0;

#ifndef CLI_DEBUG_DISABLE
	static const char USAGE[] =
		"Long Form: my_function_1" CLI_CRLF "Short Form: mf1" CLI_CRLF
		CLI_CRLF
		"Input Parameter" CLI_CRLF
		CLI_CRLF
		"Output Parameter" CLI_CRLF
		"- int errorcode" CLI_CRLF CLI_CRLF;
#else
#undef USAGE
#define USAGE ""
#endif
	if ((ret = cli_check_help__file(p_cmd, USAGE, p_file)) > 0)
		return ret;

	ret = clios_fprintf(p_file,
		"Dummy CLI Call 1" CLI_CRLF);

	return ret;
}

static int group_function_0__file(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_file)
{
	int ret = 0;
#ifndef CLI_DEBUG_DISABLE
	static const char USAGE[] =
		"Long Form: group_function_0" CLI_CRLF "Short Form: gf0" CLI_CRLF
		CLI_CRLF
		"Input Parameter" CLI_CRLF
		CLI_CRLF
		"Output Parameter" CLI_CRLF
		"- int errorcode" CLI_CRLF CLI_CRLF;
#else
#undef USAGE
#define USAGE ""
#endif
	if ((ret = cli_check_help__file(p_cmd, USAGE, p_file)) > 0)
		return ret;

	ret = clios_fprintf(p_file,
		"Dummy CLI Call Group 0" CLI_CRLF);

	return ret;
}

static int group_function_1__file(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_file)
{
	int ret = 0;

#ifndef CLI_DEBUG_DISABLE
	static const char USAGE[] =
		"Long Form: group_function_1" CLI_CRLF "Short Form: gf1" CLI_CRLF
		CLI_CRLF
		"Input Parameter" CLI_CRLF
		CLI_CRLF
		"Output Parameter" CLI_CRLF
		"- int errorcode" CLI_CRLF CLI_CRLF;
#else
#undef USAGE
#define USAGE ""
#endif
	if ((ret = cli_check_help__file(p_cmd, USAGE, p_file)) > 0)
		return ret;

	ret = clios_fprintf(p_file,
		"Dummy CLI Call Group 1" CLI_CRLF);

	return ret;
}

#endif	/* #if (CLI_SUPPORT_FILE_OUT == 1) */

/*
   Register 2 dummy commands for testing
*/
static int command_register__file(
	struct cli_core_context_s *p_core_ctx)
{
	unsigned int group_mask = 0;

	(void)cli_core_key_add__file(p_core_ctx, group_mask, "mf0", "my_function_0", dummy_my_function_0__file);
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "mf1", "my_function_1", dummy_my_function_1__file);

	return 0;
}


static int command_register_group_fct__file(
	struct cli_core_context_s *p_core_ctx)
{
	unsigned int group_mask = 0;

	(void)cli_core_key_add__file(p_core_ctx, group_mask, "gf0", "group_function_0", group_function_0__file);
	(void)cli_core_key_add__file(p_core_ctx, group_mask, "gf1", "group_function_1", group_function_1__file);

	return 0;
}

/*
   Collect the functions to register my commands.
*/
static cli_cmd_register__file my_cli_cmds[] =
{
	command_register__file,
	command_register_group_fct__file,
	IFX_NULL
};

static char const *p_cmd_group_list[] =
{
	"CLI Example my funtions",
	"CLI Example group funtions",
	IFX_NULL
};

struct cli_group_key_entry_s cmd_group_key_list[] =
{
	{"my_", 0},
	{"group_", 1},
	{IFX_NULL, 0xFF}
};

static struct cli_core_context_s *p_glb_core_ctx = IFX_NULL;

#if (CLI_SUPPORT_PIPE == 1)
static struct cli_pipe_context_s *p_cli_pipe_context = IFX_NULL;
#endif

int main(int argc, char *argv[])
{
	int retval = 0, run_pipe = 0;

#if defined(CLI_HAVE_GROUP_SUPPORT) && (CLI_HAVE_GROUP_SUPPORT == 1)
	retval = cli_core_group_setup__file(
		&p_glb_core_ctx, (unsigned int)-1,
		(void *)dummy_cli_fct_data, my_cli_cmds,
		p_cmd_group_list, cmd_group_key_list);
#else
	retval = cli_core_setup__file(
		&p_glb_core_ctx, (unsigned int)-1,
		(void *)dummy_cli_fct_data, my_cli_cmds);
#endif

	if (argc == 2)
	{
#		if (CLI_SUPPORT_PIPE == 1)
		run_pipe = 1;
#		endif
	}

	if (run_pipe == 0)
	{
		retval = cli_console_run(p_glb_core_ctx, IFX_NULL, IFX_NULL);
	}
	else
	{
#		if (CLI_SUPPORT_PIPE == 1)
		retval = cli_pipe_init(
			p_glb_core_ctx, 1, argv[1] /* "ive" */, &p_cli_pipe_context);

		/* start dummy interface to wait for quit */
		retval = cli_dummy_if_start(p_glb_core_ctx, 1000);

		(void)cli_pipe_release(p_glb_core_ctx, &p_cli_pipe_context);

#		endif
	}

	retval = cli_core_release(&p_glb_core_ctx, cli_cmd_core_out_mode_file);

	return retval;
}



