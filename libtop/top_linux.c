/******************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 * Copyright (c) 2013 - 2015 Lantiq Beteiligungs-GmbH & Co. KG
 *
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
#ifdef LINUX
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include "gpon_libs_config.h"
#include "top.h"
#include "top_linux.h"

static struct termios orig_opts;

static void console_cbreak(struct top_context *ctx)
{
	struct termios opts;
	int res = 0;

	res = tcgetattr(STDIN_FILENO, &orig_opts);
	assert(res == 0);

	memcpy(&opts, &orig_opts, sizeof(opts));
	opts.c_cc[VMIN] = 1;
	opts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | IEXTEN | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &opts);
}

static void console_endwin(struct top_context *ctx)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_opts);
}

static void console_addstr(struct top_context *ctx, const char *s)
{
	unsigned int len = strlen(s);
	unsigned int max = ctx->cols > 0 ? ctx->cols : TOP_COLS_DEFAULT;
	unsigned int lines = (len + max - 1) / max;
	unsigned int i;

	for (i = 0; i < lines; i++) {
		fwrite(s + i * max, 1,
		       (len > max ? max : len)
		       , stdout);

		if (i + 1 != lines)
			fputc('\n', stdout);

		len -= max;
	}
}

static void console_clrtoeol(struct top_context *ctx)
{
	console_addstr(ctx, "\033[K");
}

static void console_clear(struct top_context *ctx)
{
	console_addstr(ctx, "\033[2J");
}

static void console_move(struct top_context *ctx, int y, int x)
{
	(void)fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
}

static void console_curs_set(struct top_context *ctx, int flag)
{
	switch(flag) {
	case 0:
		console_addstr(ctx, "\033[?25l");
		break;
	case 1:
		console_addstr(ctx, "\033[?25h");
		break;
	default:
		break;
	}
}

static void console_refresh(struct top_context *ctx)
{
	fflush(stdout);
}

static void console_attron(struct top_context *ctx, int attr)
{
	switch (attr) {
		case A_UNDERLINE:
			console_addstr(ctx, "\033[4m");
			break;
		case A_STANDOUT:
			console_addstr(ctx, "\033[1m");
			break;
	}
}

static void console_attroff(struct top_context *ctx, int attr)
{
	switch (attr) {
		case A_UNDERLINE:
		case A_STANDOUT:
			console_addstr(ctx, "\033[0m");
			break;
	}
}

static void console_terminal_size_get(struct top_context *ctx)
{
	struct winsize win_size = { 0, 0, 0, 0 };

	ctx->cols = TOP_COLS_DEFAULT;
	ctx->rows = TOP_ROWS_DEFAULT;

	if (ioctl(0, TIOCGWINSZ, &win_size))
		return;

	if (win_size.ws_col)
		ctx->cols = win_size.ws_col;

	if (win_size.ws_row)
		ctx->rows = win_size.ws_row;
}

static int console_getch(struct top_context *ctx)
{
	char c = 10;
	ssize_t err;

	if (!read(0, &c, 1))
		return 0;

	if (c == '\033') {
		if (!read(0, &c, 1))
			return 0;

		if (c != '[')
			return 0;

		if (!read(0, &c, 1))
			return 0;

		/* suitable for PuTTY default settings; need to fix if other
		 * escase sequences are to be used */
		switch (c) {
		case 'A':
			return KEY_UP;
		case 'B':
			return KEY_DOWN;
		case 'C':
			return KEY_RIGHT;
		case 'D':
			return KEY_LEFT;
		case '1':
			err = read(0, &c, 1);
			if (err != 1)
				return 0;
			return KEY_HOME;
		case '4':
			err = read(0, &c, 1);
			if (err != 1)
				return 0;
			return KEY_END;
		case '5':
			err = read(0, &c, 1);
			if (err != 1)
				return 0;
			return KEY_PPAGE;
		case '6':
			err = read(0, &c, 1);
			if (err != 1)
				return 0;
			return KEY_NPAGE;
		default:

			return 0;
		}
	}

	return (int)c;
}

static int console_hasch(struct top_context *ctx)
{
	fd_set rfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 100;

	if (select(1, &rfds, 0, 0, &tv))
		/* for proper resize handling */
		if (select(1, &rfds, 0, 0, &tv))
			return 1;

	return 0;
}

const struct top_operations console_top_ops = {
	.addstr = console_addstr,
	.attron = console_attron,
	.attroff = console_attroff,
	.curs_set = console_curs_set,
	.clrtoeol = console_clrtoeol,
	.clear = console_clear,
	.move = console_move,
	.refresh = console_refresh,
	.getch = console_getch,
	.hasch = console_hasch,
	.terminal_size_get = console_terminal_size_get,

	.cbreak = console_cbreak,
	.endwin = console_endwin,
};

int linux_file_read(struct top_context *ctx, const char *name)
{
	int i, k=0;
	char *p = &ctx->shared_buff[0][0];
	static char *more_data = "... more data available";
	size_t s;
	FILE *f;

	memset(&ctx->line_cache[0], 0, sizeof(ctx->line_cache));
	*p = 0;

	f = fopen(name, "r");
	if (!f)
		return 0;

	s = fread(&ctx->shared_buff[0][0], 1, sizeof(ctx->shared_buff), f);

	fclose(f);

	for(i=0;(unsigned int)i<s && k<TOP_LINE_MAX;i++,p++) {
		if(*p == 0)
			break;
		if(ctx->line_cache[k] == NULL)
			ctx->line_cache[k] = p;
		if(*p != '\n')
			continue;
		*p = 0;
		k++;
	}

	if(k == TOP_LINE_MAX)
		ctx->line_cache[k-1] = more_data;

	return k;
}

int onu_top_proc_get(struct top_context *ctx, const char *name)
{
	char tmp[64];

	snprintf(tmp, sizeof(tmp), "/proc/driver/onu/%s", name);
	return linux_file_read(ctx, tmp);
}

int optic_top_proc_get(struct top_context *ctx, const char *name)
{
	char tmp[64];

	snprintf(tmp, sizeof(tmp), "/proc/driver/optic/%s", name);
	return linux_file_read(ctx, tmp);
}

#endif
