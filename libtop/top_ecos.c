/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 * Copyright (c) 2013 - 2015 Lantiq Beteiligungs-GmbH & Co. KG
 *
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#ifdef ECOS

#include "gpon_libs_config.h"
#include "top_std_defs.h"
#include "top_common.h"
#include "top.h"

#include <cyg/io/io.h>
#include <cyg/io/serial.h>

int onu_proc_show(const char *name, char *buf, const uint32_t max_size, FILE *f);
int optic_proc_show(const char *name, char *buf, const uint32_t max_size, FILE *f);

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

/** Clear to the end of line */
static void console_clrtoeol(struct top_context *ctx)
{
	console_addstr(ctx, "\033[K");
}

/** Clear the window */
static void console_clear(struct top_context *ctx)
{
	console_addstr(ctx, "\033[2J");
}

static void console_move(struct top_context *ctx, int y, int x)
{
	(void)fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
}

static void console_refresh(struct top_context *ctx)
{
	fflush(stdout);
}

static int console_getch(struct top_context *ctx)
{
	uint8_t c;
	cyg_uint32 len = 1;
	cyg_io_handle_t t;
	Cyg_ErrNo err = cyg_io_lookup("/dev/ser0", &t);

	if (err)
		return 0;

	if (cyg_io_read(t, &c, &len))
		return 0;
	else
		return (int)c;
}

int console_hasch(struct top_context *ctx)
{
	cyg_io_handle_t handle;
	Cyg_ErrNo err = cyg_io_lookup("/dev/ser0", &handle);
	int has_input;

	(void)err;

	/* we don't want to enable CYGPKG_IO_SERIAL_SELECT_SUPPORT for
	 * 'cyg_io_select' because it depends on
	 * fileio and brings a lot of other stuff and problems; just
	 * read number of received byted directly for serial device */

	cyg_devtab_entry_t *t = (cyg_devtab_entry_t *)handle;
	serial_channel *chan = (serial_channel *)t->priv;

	cbuf_t *cbuf = &chan->in_cbuf;

	cyg_drv_mutex_lock(&cbuf->lock);
	has_input = cbuf->nb != 0;
	cyg_drv_mutex_unlock(&cbuf->lock);

	if (!has_input)
		cyg_thread_delay(10);

	return has_input;
}

static void console_terminal_size_get(struct top_context *ctx)
{
	ctx->cols = TOP_COLS_DEFAULT;
	ctx->rows = TOP_ROWS_DEFAULT;
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
};

int ecos_file_read(struct top_context *ctx, const char *name, const bool onu)
{
	int ret, i, k=0;
	char *p = &ctx->shared_buff[0][0];
	static char *more_data = "... more data available";

	memset(&ctx->line_cache[0], 0, sizeof(ctx->line_cache));
	*p = 0;

	if(onu)
		ret = onu_proc_show(name, &ctx->shared_buff[0][0], sizeof(ctx->shared_buff), NULL);
	else
		ret = optic_proc_show(name, &ctx->shared_buff[0][0], sizeof(ctx->shared_buff), NULL);
	if(ret < 0)
		return 0;

	for(i=0;i<sizeof(ctx->shared_buff) && k<TOP_LINE_MAX;i++,p++) {
		if(*p == 0)
			break;
		if(ctx->line_cache[k] == NULL)
			ctx->line_cache[k] = p;
		if(*p != '\r' && *p != '\n')
			continue;
		if(*p == '\n')
			k++;
		*p = 0;
	}

	if(k == TOP_LINE_MAX)
		ctx->line_cache[k-1] = more_data;

	return k;
}

int onu_top_proc_get(struct top_context *ctx, const char *name)
{
	return ecos_file_read(ctx, name, 1);
}

int optic_top_proc_get(struct top_context *ctx, const char *name)
{
	return ecos_file_read(ctx, name, 0);
}

#endif
