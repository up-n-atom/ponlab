/******************************************************************************
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

/* ifxos_file_access.h for linux seems to be broken (ifxos 1.5.10);
   thus define IFXOS_HAVE_FILE_ACCESS separately */
#if defined(WIN32) || defined(LINUX)
#define IFXOS_HAVE_FILE_ACCESS 1
#endif

#include "ifx_types.h"
#include "lib_cli_config.h"
#include "lib_cli_osmap.h"
#include "lib_cli_debug.h"
#include "lib_cli_core.h"
#include "lib_cli_console.h"

#ifdef CLI_STATIC
#	undef CLI_STATIC
#endif

#ifdef CLI_DEBUG
#	define CLI_STATIC
#else
#	define CLI_STATIC   static
#endif


#if (CLI_SUPPORT_CONSOLE == 1)

/** \addtogroup LIB_CLI_CONSOLE
   @{
*/

#define CLI_MAX_OLD_COMMAND		(16 / 4)
#define CLI_MAX_COMMAND_LINE_LENGTH	(2560 / 4)
#define CLI_MAX_FUNCTION_NAME_LENGTH	64

#define KEY_INPUT_NONE	0
#define KEY_INPUT_ENTER	1
#define KEY_INPUT_BS	2
#define KEY_INPUT_UP	3
#define KEY_INPUT_DOWN	4
#define KEY_INPUT_CHAR	5
#define KEY_INPUT_TAB	6

#ifndef WIN32
#define BOLD ""
#define NORMAL ""
#define CLREOL "\033[K"
#else
#define BOLD   ""
#define NORMAL ""
#define CLREOL ""
#endif

struct cli_console_context_s {
	/** CLI core */
	struct cli_core_context_s *p_cli_core_ctx;
	/** user data for callback handling */
	struct cli_user_context_s *p_user_ctx;
	/** function to read in a character from the given input stream */
	cli_console_char_read_fct fct_char_read;
	/** current prompt line (prompt + user input) */
	char prompt_line[CLI_MAX_COMMAND_LINE_LENGTH];
	/** the current user command */
	char user_cmd[CLI_MAX_FUNCTION_NAME_LENGTH];
	/** table of commands (step up and down) */
	char old_command[CLI_MAX_OLD_COMMAND][CLI_MAX_COMMAND_LINE_LENGTH];
	/** current table index */
	int idx;
	/** old table index */
	int old_idx;
	/** previous table index */
	int prev_idx;
	/** console input source - set from user */
	clios_file_t *file_in;
	/** console output source - set from user */
	clios_file_t *file_out;
	/** console run flag */
	volatile IFX_boolean_t b_run;
	/** points to a console prompt get function to set a user defined prompt
	   (used while run-time for dynamic prompt setup, time, date info) */
	cli_console_prompt_fct_t fct_cons_prompt_get;
	/** default prompt, can be set from outside, used of no prompt function is set */
	char default_prompt[CLI_CONSOLE_PROMPT_SIZE];
#if (CLI_CONSOLE_NON_BLOCKING == 1)
	/*
	   console non-blocking mode via select, supported under:
	   - Linux (user space)
	   - VxWorks
	*/
	/** select cycle time [ms] */
	unsigned int cycle_time_ms;
	/** fd of the input stream (for select handling) */
	CLIOS_ioFd_t io_fd;
	/** max fd for select handling */
	CLIOS_ioFd_t max_io_fd;
	/** fd set struct for select handling */
	CLIOS_ioFd_set_t io_fd_set;
#endif
};

struct cli_console_complete_s {
	/** Current user input */
	const char *input;
	/** Console output handle */
	clios_file_t *p_out;
	/** Current index of matched command */
	int match_i;
	/** Total number of matched commands */
	int match_num;
	/** Matched command (for substitution) */
	const char *match;
	/** Number of matched characters in the matched command */
	int match_chars;
	/** Length of longest matched command */
	int max_match_len;
	/** Number of matched commands on a line (for printing) */
	int matches_per_line;
	/** Length of the match 'column' (matched string is left-justified
	 * within this column) */
	int match_col_len;
};

#if (CLI_CONSOLE_NON_BLOCKING == 1)

#if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS)
#	define clios_fileno	fileno
#elif defined(WIN32)
#	define clios_fileno	_fileno
#endif

#define CLIOS_PTR_CHECK	1

#if (CLIOS_PTR_CHECK == 1)
#	define CLIOS_RETURN_IF_POINTER_NULL(p_argument, ret_value) \
		do {if (p_argument == IFX_NULL) {return (ret_value);}} while (0)
#	define CLIOS_RETURN_VOID_IF_POINTER_NULL(p_argument, ret_value) \
		do {if (p_argument == IFX_NULL) {return ;}} while (0)
#else
#	define CLIOS_RETURN_IF_POINTER_NULL(p_argument, ret_value)
#	define CLIOS_RETURN_VOID_IF_POINTER_NULL(p_argument, ret_value)
#endif


/*
   ToDo: check if it makes sence so move this functions to the IFXOS.
   Supported OS:
   - Linux
   - VxWorks
   - [Win32 ? (the select under windows is designed for socket handling)]
*/

IFX_void_t clios_io_fd_set(
	CLIOS_ioFd_t io_fd,
	CLIOS_ioFd_set_t *p_io_fd)
{
#if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)
	CLIOS_RETURN_VOID_IF_POINTER_NULL(p_io_fd, IFX_ERROR);

	FD_SET(io_fd, p_io_fd);
#endif
	return;
}

IFX_int_t clios_io_fd_isset(
	CLIOS_ioFd_t io_fd,
	CLIOS_ioFd_set_t *p_io_fd)
{
#if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)
	CLIOS_RETURN_IF_POINTER_NULL(p_io_fd, IFX_ERROR);

	return FD_ISSET(io_fd, p_io_fd);
#else
	return 0;
#endif
}

IFX_void_t clios_io_fd_zero(
	CLIOS_ioFd_set_t *p_io_fd)
{
#if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)
	CLIOS_RETURN_VOID_IF_POINTER_NULL(p_io_fd, IFX_ERROR);

	FD_ZERO(p_io_fd);
	return;
#endif
	return;
}

IFX_int_t clios_io_select(
	CLIOS_ioFd_t max_io_fd,
	CLIOS_ioFd_set_t *p_io_fd_read,
	CLIOS_ioFd_set_t *p_io_fd_write,
	CLIOS_ioFd_set_t *p_io_fd_except,
	IFX_int_t timeout_ms )
{
#if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)
	IFX_int_t retval = 0;
	struct timeval   tv, *p_time;

	p_time = &tv;

	/* set timeout value */
	switch (timeout_ms)
	{
		case CLIOS_IO_WAIT_FOREVER:
			p_time = NULL;
		break;

		case CLIOS_IO_NO_WAIT:
			tv.tv_sec = 0;
			tv.tv_usec = 0;
		break;

		default:
			tv.tv_sec = timeout_ms / 1000;
			tv.tv_usec = (timeout_ms % 1000) * 1000;
		break;
	}

	/* call selct function itself */
	retval = (IFX_int_t)select(
		max_io_fd, p_io_fd_read, p_io_fd_write, p_io_fd_except, p_time);

	return retval;
#else
	/* not supported */
	return IFX_ERROR;
#endif
}

#endif	/* #if (CLI_CONSOLE_NON_BLOCKING == 1) */


#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
CLI_STATIC int clios_terminal_width_get(void)
{
	int def = 80;

#if defined(LINUX) && !defined(__KERNEL__)
	struct winsize w;

	if (clios_ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		if (!w.ws_col)
			return def;
		else
			return w.ws_col;
	}
#endif

	return def;
}
#endif	/* #if (CLI_SUPPORT_AUTO_COMPLETION == 1) */


CLI_STATIC int console_non_block_init(
	struct cli_console_context_s *p_console_ctx,
	int non_blocking_enable,
	unsigned int cycle_time_ms)
{
#if (CLI_CONSOLE_NON_BLOCKING == 1)
#	if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)
	int io_fd = -1;

	p_console_ctx->io_fd = -1;
	p_console_ctx->max_io_fd = 0;
	p_console_ctx->cycle_time_ms = 0;
	if (non_blocking_enable == 1)
	{
		io_fd = clios_fileno(p_console_ctx->file_in);
		if (io_fd < 0)
		{
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console: non-blocking init, get fd failed" CLI_CRLF));
			return IFX_ERROR;
		}
		p_console_ctx->io_fd = io_fd;
		p_console_ctx->max_io_fd = io_fd + 1;
		p_console_ctx->cycle_time_ms = (cycle_time_ms == 0) ? 100 : cycle_time_ms;
	}

	return IFX_SUCCESS;
#	else
	/* OS not supported */
	p_console_ctx->io_fd = -1;
	p_console_ctx->max_io_fd = 0;
	p_console_ctx->cycle_time_ms = 0;

	CLI_USR_ERR(LIB_CLI,
		("ERR CLI Console: non-blocking init, OS not supported" CLI_CRLF));

	return IFX_ERROR;
#	endif
#else
	CLI_USR_ERR(LIB_CLI,
		("ERR CLI Console: non-blocking init, not built-in" CLI_CRLF));
	return IFX_ERROR;
#endif
}

#if (CLI_CONSOLE_NON_BLOCKING == 1)
CLI_STATIC int console_non_block_check_in(
	struct cli_console_context_s *p_console_ctx)
{
#	if (defined(LINUX) && !defined(__KERNEL__)) || defined(VXWORKS) || defined(WIN32)

	clios_io_fd_zero(&p_console_ctx->io_fd_set);
	clios_io_fd_set(p_console_ctx->io_fd, &p_console_ctx->io_fd_set);

	if (clios_io_select(
		p_console_ctx->max_io_fd,
		&p_console_ctx->io_fd_set, IFX_NULL, IFX_NULL,
		p_console_ctx->cycle_time_ms) < 0)
	{
		return IFX_ERROR;
	}

	if (clios_io_fd_isset(p_console_ctx->io_fd, &p_console_ctx->io_fd_set))
		{return 1;}
	else
		{return 0;}

#	else
	CLI_USR_ERR(LIB_CLI,
		("ERR CLI Console: non-blocking mode check in not supported" CLI_CRLF));
	return IFX_ERROR;
#	endif
}

#endif /* #if (CLI_CONSOLE_NON_BLOCKING == 1) */

#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
/* Iterate over all registered CLI commands and calculate the following:
 * - number of commands (`shared->match_num`) that match input line
 *   (start with `shared->input`);
 * - longest matched command (`shared->max_match_len`);
 * - longest common sequence between input line and all matched commands
 *   (`shared->match` and `shared->match_chars`). */
CLI_STATIC int cli_complete_prepare(const char *cli_cmd, void *user_data)
{
	struct cli_console_complete_s *shared = user_data;
	int len;
	int i;

	if (clios_strlen(shared->input) > clios_strlen(cli_cmd))
		return 0;

	if (clios_memcmp(shared->input,
		cli_cmd,
		clios_strlen(shared->input)) == 0) {

		if (clios_strlen(cli_cmd) > (unsigned int)shared->max_match_len)
			shared->max_match_len = clios_strlen(cli_cmd);

		if (shared->match) {
			/* find longest common match */
			len = clios_strlen(cli_cmd) > clios_strlen(shared->match) ?
				clios_strlen(cli_cmd) : clios_strlen(shared->match);

			for (i = 0; i <= len; i++) {
				if (cli_cmd[i] != shared->match[i]) {
					if (i < shared->match_chars)
						shared->match_chars = i;

					break;
				}
			}
		}

		shared->match_num++;
		shared->match = cli_cmd;
	}

	return 0;
}

/* Iterate over all registered CLI commands and print the matched ones
 * taking into account that we need to print at most `shared->matches_per_line`
 * commands on a line. Also, pad each command to the left with spaces (up to
 * `shared->match_col_len`) so they form pretty looking table. */
CLI_STATIC int cli_complete_print(const char *cli_cmd, void *user_data)
{
	struct cli_console_complete_s *shared = user_data;
	int spacing;

	if (clios_strlen(shared->input) > clios_strlen(cli_cmd))
		return 0;

	if (clios_memcmp(shared->input,
		cli_cmd,
		clios_strlen(shared->input)) == 0) {

		(void)clios_fprintf(shared->p_out, "%s", cli_cmd);

		if (shared->match_i != 0 &&
		    shared->match_i + 1 != shared->match_num &&
		    (shared->match_i + 1) % shared->matches_per_line == 0) {
			(void)clios_fprintf(shared->p_out, "%s", CLI_CRLF);
		} else {
			spacing = shared->match_col_len - clios_strlen(cli_cmd);
			if (spacing > 0)
				while (spacing--)
					(void)clios_fprintf(shared->p_out, " ");
		}

		shared->match_i++;
	}

	return 0;
}
#endif	/* #if (CLI_SUPPORT_AUTO_COMPLETION == 1) */

/**

\returns
- number of characters
- 1 and char = '0' --> eof
- < 0 --> error
*/
CLI_STATIC int char_read(
	struct cli_console_context_s *p_console_ctx,
	clios_file_io_t *p_in_io,
	int *p_b_eof,
	char *p_c)
{
	int retval = 0, b_eof = 0;
	char c = '\0';
	clios_file_t *p_in = (clios_file_io_t *)p_in_io;

#if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
	if ((int)clios_fread(&c, 1, 1, p_in) == 0)
	{
		if (clios_feof(p_in))
		{
			b_eof = 1;
			retval = 1;
		}
	}
#else
	retval = clios_getchar();
	if (retval == EOF)
	{
		b_eof = 1;
	}
	c = (char)retval;
	retval = 1;
#endif

	if (p_b_eof != IFX_NULL) {*p_b_eof = b_eof;}
	if (p_c != IFX_NULL) {*p_c = c;}
	return  retval;
}

/**
   Read command line.

\param[in] p_console_ctx Console context pointer
\param[in] s  Destination pointer for command line
\param[in] len  Maximum length of command line
\param[in] p_in  File for key input (e.g. IFXOS_STDIN)
\param[in] p_out File for output (e.g. IFXOS_STDOUT)
\return Length of command string
*/
CLI_STATIC int cmd_line_read(
	struct cli_console_context_s *p_console_ctx,
	char *s,
	unsigned int len,
	clios_file_t *p_in,
	clios_file_t *p_out)
{
	char c;
	/*char echo;*/
	int b_eof = 0;
	int blank_cnt = 0;
	int first = 1;
	int ret = KEY_INPUT_CHAR;

	if (clios_strlen(s))
		first = 0;

	len -= (unsigned int)clios_strlen(s);
	s += (unsigned int)clios_strlen(s);

	while (len--) {
		/*echo = 0;*/

		(void)p_console_ctx->fct_char_read(
			p_console_ctx, (clios_file_io_t *)p_in, &b_eof, &c);
		if (b_eof == 1)
		{
			ret = KEY_INPUT_NONE;
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */
		}

		if (first && (c == ' ')) {
			len++;
			continue;
		}

		first = 0;

		if (c != ' ') {
			blank_cnt = 0;
		}

		switch (c) {
		default:
			/*echo = c;*/
			break;

		case ' ':
			if (blank_cnt) {
				len++;
				s--;
			} /* else {
				echo = ' ';
			} */
			blank_cnt++;
			break;

			/* backspace */
		case 127:	/* BS in Linux Terminal */
		case 8:	/* BS in TTY */
			len++;
			s--;
			ret = KEY_INPUT_BS;
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */

#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
			/* Ctrl-D */
		case 4:
			ret = KEY_INPUT_NONE;
			p_console_ctx->b_run = IFX_FALSE;
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */
			break;

			/* tab */
		case 9:
			ret = KEY_INPUT_TAB;
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */
			break;
#endif	/* #if (CLI_SUPPORT_AUTO_COMPLETION == 1) */

		case '\r':
		case '\n':
			/*(void)clios_putchar('\n', p_out);*/
#if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
			(void)clios_fflush(p_out);
#endif
			ret = KEY_INPUT_ENTER;
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */

		case 033:
			(void)p_console_ctx->fct_char_read(
				p_console_ctx, (clios_file_io_t *)p_in, &b_eof, &c);
			if (b_eof == 1)
				{break;}

			switch (c) {
			case 27:
				/* escape */
				break;

			case '[':
				(void)p_console_ctx->fct_char_read(
					p_console_ctx, (clios_file_io_t *)p_in, &b_eof, &c);
				if (b_eof == 1)
					{break;}

				switch (c) {
				case 'D':
					/* left */
					break;
				case 'C':
					/* right */
					break;
				case 'A':
					/* up */
					ret = KEY_INPUT_UP;
					break;
				case 'B':
					/* down */
					ret = KEY_INPUT_DOWN;
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
			/*lint -save -e(801) */
			goto KEY_FINISH;
			/*lint -restore */
		}

		*s++ = c;
		/*if (echo)
			(void)clios_putchar(echo, p_out);*/
#if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
		(void)clios_fflush(p_out);
#endif
	}

KEY_FINISH:

	*s++ = 0;

	return ret;
}

/** Exit CLI

   \param[in] user callback context pointer
*/
CLI_STATIC int console_exit(void *p_cb_ctx)
{
	struct cli_console_context_s *p_console_ctx;

	p_console_ctx = (struct cli_console_context_s *)p_cb_ctx;
	p_console_ctx->p_cli_core_ctx = IFX_NULL;
	p_console_ctx->b_run = IFX_FALSE;

	return IFX_SUCCESS;
}

CLI_STATIC void console_prompt_print(
	struct cli_console_context_s *p_console_ctx)
{
	clios_file_t *p_out = p_console_ctx->file_out;
	const char *p_prompt = IFX_NULL;

#if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
	(void)clios_fflush(p_out);
#endif
	if (p_console_ctx->fct_cons_prompt_get)
		{p_prompt = p_console_ctx->fct_cons_prompt_get(p_console_ctx);}
	else
		{p_prompt = (const char *)p_console_ctx->default_prompt;}

	(void)clios_fprintf(p_out, "\r" CLREOL BOLD "%s" NORMAL "%s",
		p_prompt, p_console_ctx->prompt_line);

#if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
	(void)clios_fflush(p_out);
#endif

	return;
}

/** Handle console input

   \param[in] p_console_ctx Console context pointer
*/
CLI_STATIC int console_handle(
	struct cli_console_context_s *p_console_ctx,
	int b_new_call,
	int b_non_blocking,
	unsigned int *p_timeout_ms)
{
	int result, len, i, b_upd_prompt;
	unsigned int read_cnt = 0, timeout_ms = 0;
	struct cli_core_context_s *p_cli_core_ctx = p_console_ctx->p_cli_core_ctx;
	clios_file_t *p_in = p_console_ctx->file_in;
	clios_file_t *p_out = p_console_ctx->file_out;
#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
	struct cli_console_complete_s shared;
	int terminal_width = clios_terminal_width_get();
#endif

	if (p_console_ctx == IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console Handle: missing context" CLI_CRLF));
		return IFX_ERROR;
	}

	if (p_console_ctx->fct_char_read == IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console Handle: invalid setup, missing char read function" CLI_CRLF));
		return IFX_ERROR;
	}

	if (b_non_blocking == 1)
	{
#	if (CLI_CONSOLE_NON_BLOCKING == 1)

		if (p_timeout_ms == IFX_NULL) {
			CLI_USR_ERR(LIB_CLI,
				("ERR CLI Console Handle: missing arg - timeout" CLI_CRLF));
			return IFX_ERROR;
		}

		if ((p_console_ctx->io_fd < 0) ||
		    (p_console_ctx->max_io_fd == 0) ||
		    (p_console_ctx->cycle_time_ms == 0))
		{
			CLI_USR_ERR(LIB_CLI,
				("ERR CLI Console Handle: non blocking mode, missing init" CLI_CRLF));
			return IFX_ERROR;
		}

		timeout_ms = *p_timeout_ms;
		if (timeout_ms == CLIOS_IO_WAIT_FOREVER)
		{
			read_cnt = CLIOS_IO_WAIT_FOREVER;
		}
		else
		{
			read_cnt = ((timeout_ms / p_console_ctx->cycle_time_ms) == 0) ?
					1 : (timeout_ms / p_console_ctx->cycle_time_ms);
		}
#	else
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console Handle: non blocking mode, not built-in" CLI_CRLF));
		return IFX_ERROR;
#	endif
	}
	else
	{
		read_cnt = 1;
		timeout_ms = CLIOS_IO_WAIT_FOREVER;
	}

	if (b_new_call == 1)
	{
		p_console_ctx->user_cmd[0] = 0;
		p_console_ctx->prompt_line[0] = 0;
		clios_memset(p_console_ctx->prompt_line, 0x00, sizeof(p_console_ctx->prompt_line));
		/*(void)clios_fprintf(p_out, CLI_CRLF);*/

		console_prompt_print(p_console_ctx);
	}

	i = p_console_ctx->old_idx;
	b_upd_prompt = 0;
	while ((p_console_ctx->b_run == IFX_TRUE) && (read_cnt > 0))
	{
		int key_in = 0;

		if (b_upd_prompt == 1)
		{
			console_prompt_print(p_console_ctx);
			b_upd_prompt = 0;
		}

#		if (CLI_CONSOLE_NON_BLOCKING == 1)
		if (b_non_blocking == 1)
		{
			int retval;

			if ((retval = console_non_block_check_in(p_console_ctx)) < 0)
				{return IFX_ERROR;}

			if (retval == 0)
			{
				if ( (read_cnt != CLIOS_IO_WAIT_FOREVER) && (read_cnt > 0))
				{
					read_cnt--;
					timeout_ms = read_cnt * p_console_ctx->cycle_time_ms;
				}

				continue;
			}
		}
#		endif

		key_in = cmd_line_read(p_console_ctx,
			p_console_ctx->prompt_line,
			CLI_MAX_COMMAND_LINE_LENGTH, p_in, p_out);
		switch (key_in)
		{
		case KEY_INPUT_UP:
			if (p_console_ctx->old_idx)
				p_console_ctx->old_idx--;
			else
				p_console_ctx->old_idx = CLI_MAX_OLD_COMMAND - 1;

			if (clios_strlen(p_console_ctx->old_command[p_console_ctx->old_idx]))
			{
				clios_strcpy(
					p_console_ctx->prompt_line,
					p_console_ctx->old_command[p_console_ctx->old_idx]);
				b_upd_prompt = 1;
			}
			else
			{
				p_console_ctx->old_idx = i;
			}
			break;

		case KEY_INPUT_DOWN:
			if (++p_console_ctx->old_idx >= CLI_MAX_OLD_COMMAND)
				p_console_ctx->old_idx = 0;

			if (clios_strlen(p_console_ctx->old_command[p_console_ctx->old_idx]))
			{
				if (p_console_ctx->old_idx >= CLI_MAX_OLD_COMMAND)
					p_console_ctx->old_idx = 0;

				clios_strcpy(
					p_console_ctx->prompt_line,
					p_console_ctx->old_command[p_console_ctx->old_idx]);
				b_upd_prompt = 1;
			}
			else
			{
				p_console_ctx->old_idx = i;
			}
			break;

		case KEY_INPUT_BS:
			b_upd_prompt = 1;
			break;

#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
		case KEY_INPUT_TAB:
			/* tab */
			shared.input = p_console_ctx->prompt_line;
			shared.p_out = p_out;
			shared.match_i = 0;
			shared.match_num = 0;
			shared.match_chars = sizeof(p_console_ctx->prompt_line) - 1;
			shared.max_match_len = 0;
			shared.match = IFX_NULL;

			cli_traverse(p_console_ctx->p_cli_core_ctx,
				cli_complete_prepare, &shared);

			shared.matches_per_line = terminal_width / (shared.max_match_len + 1);

			if (shared.matches_per_line == 0)
				shared.matches_per_line = 1;

			shared.match_col_len = terminal_width / shared.matches_per_line;

			if (shared.match_num == 1)
			{
				/* we found exactly one match, so just use it */
				clios_strcpy(p_console_ctx->prompt_line, shared.match);
			}
			else
			{
				/* print all possible matches */
				(void)clios_fprintf(p_out, CLI_CRLF);
				cli_traverse(p_console_ctx->p_cli_core_ctx,
					cli_complete_print, &shared);

				(void)clios_fprintf(p_out, CLI_CRLF);

				if (shared.match_num > 1 && shared.match_chars)
				{
					/* use longest common sequence as our input */
					clios_memcpy(p_console_ctx->prompt_line, shared.match, shared.match_chars);
					p_console_ctx->prompt_line[shared.match_chars] = '\0';
				}
			}
			b_upd_prompt = 1;
			break;
#endif	/* #if (CLI_SUPPORT_AUTO_COMPLETION == 1) */

		case KEY_INPUT_ENTER:
			read_cnt = 0;
			break;

		case KEY_INPUT_NONE:
			break;

		default:
			/* read_cnt = 0; */
			break;
		}	/* switch (key_in) */
	}	/* while ((p_console_ctx->b_run == IFX_TRUE) && (read_cnt > 0)) */

	if (p_console_ctx->b_run == IFX_FALSE)
		/* indicate command "quit" */
		return IFX_ERROR;

	/* get command name */
	if (clios_sscanf(p_console_ctx->prompt_line,
			 "%" _MKSTR(CLI_MAX_FUNCTION_NAME_LENGTH) "s",
			 p_console_ctx->user_cmd) < 1)
		/* no command found */
		return IFX_ERROR;

	len = (int)clios_strlen(p_console_ctx->user_cmd);

	if (len)
	{
		if (clios_strncmp(
			p_console_ctx->old_command[p_console_ctx->prev_idx],
			p_console_ctx->prompt_line, (unsigned int)len) != 0)
		{
			clios_strcpy(
				p_console_ctx->old_command[p_console_ctx->idx],
				p_console_ctx->prompt_line);
			p_console_ctx->prev_idx = p_console_ctx->idx;
			if (++p_console_ctx->idx >= CLI_MAX_OLD_COMMAND)
				p_console_ctx->idx = 0;
		}

		p_console_ctx->old_idx = p_console_ctx->idx;

#		if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
		(void)clios_fflush(p_out);
#		endif

		result = cli_core_cmd_arg_exec__file(
			p_cli_core_ctx, p_console_ctx->user_cmd, p_console_ctx->prompt_line + len + 1, p_out);

#		if defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1)
		(void)clios_fflush(p_out);
#		endif

		if (result == 1) {
			/* indicate command "quit" */
			return IFX_ERROR;
		}
	}

	if (p_timeout_ms != IFX_NULL) {*p_timeout_ms = timeout_ms;}

	return IFX_SUCCESS;
}


#define CLI_CONSOLE_INVALID "CONS_INVALID"

/** Default CLI console prompt handler.

   \param[in] p_console_ctx  points to the console context.
*/
CLI_STATIC const char *cli_default_prompt_get(struct cli_console_context_s *p_console_ctx)
{
	if (p_console_ctx == IFX_NULL)
		{return (const char *)CLI_CONSOLE_INVALID;}

	if (p_console_ctx->p_cli_core_ctx == IFX_NULL)
		{return (const char *)CLI_CONSOLE_INVALID;}

	clios_sprintf(p_console_ctx->default_prompt, "#[%d]>",
		cli_core_inst_num_get(p_console_ctx->p_cli_core_ctx));

	return (const char *)p_console_ctx->default_prompt;
}


/** Initialize CLI

   \param[in] p_cli_core_ctx  CLI core context pointer
   \param[in] p_in  Input FD
   \param[in] p_out  Output FD
   \param[in] pp_console_ctx  Returns the CLI Console Context pointer
*/
int cli_console_init(
	struct cli_core_context_s *p_cli_core_ctx,
	clios_file_io_t *p_in_io,
	clios_file_io_t *p_out_io,
	struct cli_console_context_s **pp_console_ctx)
{
	struct cli_console_context_s *p_console_ctx;
	clios_file_t *p_in = (clios_file_t *)p_in_io;
	clios_file_t *p_out = (clios_file_t *)p_out_io;

	if (*pp_console_ctx != IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console: init - invalid arg, context pointer not zero" CLI_CRLF));
		return IFX_ERROR;
	}

	p_console_ctx = (struct cli_console_context_s *)
	    clios_memalloc(sizeof(struct cli_console_context_s));
	if (p_console_ctx == IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Console: init - no memory" CLI_CRLF));
		return IFX_ERROR;
	}

	clios_memset(p_console_ctx, 0x00, sizeof(struct cli_console_context_s));
	p_console_ctx->fct_char_read = char_read;
	p_console_ctx->p_cli_core_ctx = p_cli_core_ctx;
	p_console_ctx->file_in = p_in;
	p_console_ctx->file_out = p_out;

	/* run the default function once to init the context default prompt buffer */
	(void)cli_default_prompt_get(p_console_ctx);

#	if 0
	/* set the default function --> prompt will be generated with each handling */
	p_console_ctx->fct_cons_prompt_get = cli_default_prompt_get;
#	endif

	p_console_ctx->b_run = IFX_TRUE;

#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
#if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) )
	if (p_out == IFXOS_STDOUT)
		clios_echo_off();
	if (p_in == IFXOS_STDIN)
		clios_keypress_set();
#endif
#endif

	(void)clios_fprintf(p_out, CLI_CRLF "CLI management interface" CLI_CRLF);
	(void)clios_fprintf(p_out, "Enter 'help' for a list of built-in commands." CLI_CRLF);
	/* (void)omci_cli_help_print(context, "-h", p_out); */

	(void)cli_user_if_register(
		p_cli_core_ctx, p_console_ctx,
		console_exit, IFX_NULL, IFX_NULL,
		&p_console_ctx->p_user_ctx);

	*pp_console_ctx = p_console_ctx;
	return IFX_SUCCESS;
}


/** Shutdown CLI

   \param[in] pp_console_ctx Contains the console context pointer
*/
int cli_console_release(
	struct cli_core_context_s *p_cli_core_ctx,
	struct cli_console_context_s **pp_console_ctx)
{
	struct cli_console_context_s *p_console_ctx;

	if (*pp_console_ctx)
	{
		p_console_ctx = *pp_console_ctx;
		*pp_console_ctx = IFX_NULL;

#if (CLI_SUPPORT_AUTO_COMPLETION == 1)
#if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) )
		if (p_console_ctx->file_out == IFXOS_STDOUT) {
			clios_echo_on();
		}
		if (p_console_ctx->file_in == IFXOS_STDIN) {
			clios_keypress_reset();
		}
#endif
#endif

		if (cli_user_if_unregister(
			p_cli_core_ctx, &p_console_ctx->p_user_ctx) == IFX_ERROR)
		{
			CLI_USR_ERR(LIB_CLI,
				("ERR CLI Console: shutdown - failed" CLI_CRLF));
			return -1;
		}
		clios_memfree(p_console_ctx);
	}

	return IFX_SUCCESS;
}

/** Handle console input

   \param[in] p_console_ctx Console context pointer
*/
int cli_console_handle(
	struct cli_console_context_s *p_console_ctx)
{
	return console_handle(p_console_ctx,
		1, /* b_new_call */
		0, /* b_non_blocking */
		IFX_NULL  /* timeout_ms */);
}

/** Handle console init non-blocking mode

\param[in]
   p_console_ctx Console context pointer
\param[in]
   non_blocking_enable
\param[in]
   non_blocking_enable

\return
   0 if success
   <0 in case of error
*/
int cli_console_init_nb(
	struct cli_console_context_s *p_console_ctx,
	int non_blocking_enable,
	unsigned int cycle_time_ms)
{
#if (CLI_CONSOLE_NON_BLOCKING == 1)
	return console_non_block_init(
		p_console_ctx, non_blocking_enable, cycle_time_ms);
#else
	CLI_USR_ERR(LIB_CLI,
		("ERR CLI Console Init (non-blocking): not built-in" CLI_CRLF));

	return IFX_ERROR;
#endif
}

/** Handle console input (non-blocking)

\param[in]
   p_console_ctx Console context pointer
\param[in]
   timeout_ms
*/
int cli_console_handle_nb(
	struct cli_console_context_s *p_console_ctx,
	unsigned int *p_timeout_ms)
{
#if (CLI_CONSOLE_NON_BLOCKING == 1)

	return console_handle(p_console_ctx,
		1, /* b_new_call */
		1, /* b_non_blocking */
		p_timeout_ms  /* timeout_ms */);

#else
	CLI_USR_ERR(LIB_CLI,
		("ERR CLI Console Handle (non-blocking): not built-in" CLI_CRLF));

	return IFX_ERROR;
#endif
}

/** Handle console input

\param[in] p_console_ctx Console context pointer
\param[in] p_in  File for key input (e.g. IFXOS_STDIN)
\param[in] p_out File for output (e.g. IFXOS_STDOUT)
*/
int cli_console_run(
	struct cli_core_context_s *p_cli_core_ctx,
	clios_file_io_t *p_in_io,
	clios_file_io_t *p_out_io)
{
	int retval = 0;
	struct cli_console_context_s *p_console_ctx = IFX_NULL;
	clios_file_io_t *p_in_used  = (p_in_io)  ? p_in_io  : (clios_file_io_t *)IFXOS_STDIN;
	clios_file_io_t *p_out_used = (p_out_io) ? p_out_io : (clios_file_io_t *)IFXOS_STDOUT;

	if ((retval = cli_console_init(
			p_cli_core_ctx, p_in_used, p_out_used, &p_console_ctx)) != IFX_SUCCESS) {
		return IFX_ERROR;
	}

	do {
		retval = cli_console_handle(p_console_ctx);
	} while ((retval == IFX_SUCCESS) && (p_console_ctx->b_run == IFX_TRUE));

	(void)cli_console_release(p_cli_core_ctx, &p_console_ctx);

	return 0;
}

/* Check if the current console instance is running. */
unsigned int cli_console_is_running(
	struct cli_console_context_s *p_console_ctx)
{
	return p_console_ctx->b_run == IFX_TRUE ? 1 : 0;
}

/* This function sets console prompt handler. */
int cli_console_prompt_fct_set(
	struct cli_console_context_s *p_console_ctx,
	const cli_console_prompt_fct_t f_cons_prompt)
{
	if (p_console_ctx == IFX_NULL)
		return IFX_ERROR;

	p_console_ctx->fct_cons_prompt_get = f_cons_prompt;

	return IFX_SUCCESS;
}

/* This function sets console prompt handler. */
int cli_console_default_prompt_set(
	struct cli_console_context_s *p_console_ctx,
	const char *p_new_prompt)
{
	if (p_console_ctx == IFX_NULL)
		return IFX_ERROR;

	if (p_new_prompt == IFX_NULL)
	{
		/* reset to the internal default */
		(void)cli_default_prompt_get(p_console_ctx);
	}
	else
	{
		if (clios_strlen(p_new_prompt) >= CLI_CONSOLE_PROMPT_SIZE)
			{return IFX_ERROR;}
		else
			{clios_sprintf(p_console_ctx->default_prompt, "%s", p_new_prompt);}
	}

	return IFX_SUCCESS;
}

/* This function sets the char read function for the console. */
int cli_console_char_read_fct_set(
	struct cli_console_context_s *p_console_ctx,
	const cli_console_char_read_fct f_char_read)
{
	if (p_console_ctx == IFX_NULL)
		{return IFX_ERROR;}

	if (f_char_read != IFX_NULL)
		{p_console_ctx->fct_char_read = f_char_read;}
	else
		{p_console_ctx->fct_char_read = char_read;}

	return IFX_SUCCESS;
}

/** @} */

#endif /* #if (CLI_SUPPORT_CONSOLE == 1) */

