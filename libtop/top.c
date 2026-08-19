/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 * Copyright (c) 2013 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include "gpon_libs_config.h"
#include "top.h"

#include <sys/time.h>
#include <signal.h>

#ifdef LINUX
#include <malloc.h>
#endif

#define opt(p) if (p) p

#ifdef EXTRA_VERSION
#define lib_top_extra_ver_str "." EXTRA_VERSION
#else
#define lib_top_extra_ver_str ""
#endif

/** what string support, version string */
const char lib_top_whatversion[] = "@(#)MaxLinear Lib TOP, version "
				   PACKAGE_VERSION
				   lib_top_extra_ver_str;

static inline const struct top_page_desc *active_page(struct top_context *ctx)
{
	return &ctx->page[ctx->page_sel];
}

static inline struct top_page_state *active_page_state(struct top_context *ctx)
{
	return &ctx->page_state[ctx->page_sel];
}

/** Initialize groups

   \param init Where init or shutdown groups
*/
static void pages_init(struct top_context *ctx, int init)
{
	unsigned int i;

	for (i = 0; i < ctx->page_init_num; i++)
		ctx->page_init[i](init);
}

/** Get number of lines occupied by string */
static inline unsigned int lines_num(struct top_context *ctx, const char *s)
{
	unsigned int max = ctx->cols;
	unsigned int len = strlen(s);

	if (!len)
		return 1;

	return (len + max - 1) / max;
}

/** Check if line will be filtered (not showed)

   \param[in] str Input string

   \return 1 if line will be filtered
*/
static int is_filtered(struct top_context *ctx, const char *str)
{
	if (strlen(ctx->filter) == 0)
		return 0;

	/*
	if (strlen(str) == 0)
		return 0;
	*/

	if (strstr(str, ctx->filter) != NULL)
		return 0;

	return 1;
}

#ifdef LINUX
static volatile sig_atomic_t g_need_shutdown = 0;

/** shutdown handler

   \param[in] sig Signal
*/
static void shutdown(int sig)
{
	if (sig == SIGSEGV)
		fprintf(stderr, "Segmentation fault\n");

	g_need_shutdown = 1;
}
#endif

#ifdef SIGWINCH
/** resize indication */
static volatile sig_atomic_t g_need_resize = 0;

/** resize handler

   \param[in] sig Signal
*/
static void resize(int sig)
{
	g_need_resize = 1;

	signal(sig, resize);
}
#endif

static int activity_check(struct top_context *ctx, FILE *f)
{
	if (ctx->activity_check)
		if (ctx->activity_check(ctx, f)) {
			fprintf(f, "ERROR: activity check failed\n");
			return -1;
		}

	return 0;
}

/** Fetch counters (update application's data with device's one)

   \param[in] Counters group to fetch

   \return Number of lines in page; -1 if data fetch handler is
           not defined
*/
static int counters_fetch(struct top_context *ctx, unsigned int page_idx)
{
	if (!ctx->page[page_idx].page_get) {
		ctx->page_state[page_idx].total = 0;
		return -1;
	}

	ctx->page_state[page_idx].total = ctx->page[page_idx].page_get(ctx,
		ctx->page[page_idx].input_file_name);

	if (ctx->page_state[page_idx].start > ctx->page_state[page_idx].total)
		ctx->page_state[page_idx].start = ctx->page_state[page_idx].total;

	return ctx->page_state[page_idx].total;
}

/** Write table to file

   \param[in] out      File to write in
   \param[in] page_idx Index of page
*/
static void table_write(struct top_context *ctx, FILE *out, int page_idx)
{
	int i;
	char buff[TOP_LINE_LEN];
	char *p;
	top_do_fprintf_t *do_fprintf = ctx->ops->do_fprintf ?
						ctx->ops->do_fprintf : fprintf;
	FILE *stream = ctx->ops->stream ? ctx->ops->stream(ctx) : out;

	if (!ctx->page[page_idx].line_get)
		return;

	do_fprintf(stream, "Page: %s" TOP_CRLF, ctx->page[page_idx].name);

	buff[0] = 0;
	p = ctx->page[page_idx].line_get(ctx, -1, buff);
	if(p == NULL && buff[0] != 0)
		p = buff;
	if(p)
		do_fprintf(stream, "%s" TOP_CRLF, p);

	for (i = 0; i < ctx->page_state[page_idx].total; i++) {
		buff[0] = 0;
		p = ctx->page[page_idx].line_get(ctx, i, buff);
		if(p == NULL && buff[0] != 0)
			p = buff;
		if(p)
			do_fprintf(stream, "%s" TOP_CRLF, p);
	}
}

static int readkey(struct top_context *ctx)
{
	int c = ctx->ops->getch(ctx);
	if (c == '\033') {
		c = ctx->ops->getch(ctx);
		if (c != '[')
			return 0;

		c = ctx->ops->getch(ctx);

		/* suitable for PuTTY default settings; need to fix if other
		 * escape sequences are to be used */
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
			(void)ctx->ops->getch(ctx);
			return KEY_HOME;
		case '4':
			(void)ctx->ops->getch(ctx);
			return KEY_END;
		case '5':
			(void)ctx->ops->getch(ctx);
			return KEY_PPAGE;
		case '6':
			(void)ctx->ops->getch(ctx);
			return KEY_NPAGE;
		default:
			return 0;
		}
	}

	return (int)c;
}

/** prompt line

   \param[in]     prefix Prompt prefix
   \param[in,out] str    Return entered string
*/
static void prompt(struct top_context *ctx, const char *prefix, char *str)
{
	int ret = 0;
	char run = 1;

	if (ctx->ops->pre_iter)
		ret = ctx->ops->pre_iter(ctx);

	opt(ctx->ops->curs_set)(ctx, 1);

	if (ret == 0)
		opt(ctx->ops->do_iter)(ctx);

	opt(ctx->ops->post_iter)(ctx);

	while (run) {
		int len = strlen(str);
		int key;

		if (ctx->ops->pre_iter)
			ret = ctx->ops->pre_iter(ctx);

		ctx->ops->move(ctx, ctx->rows - 1, 0);
		ctx->ops->addstr(ctx, prefix);
		ctx->ops->addstr(ctx, str);
		ctx->ops->clrtoeol(ctx);
		opt(ctx->ops->refresh)(ctx);

		if (ret == 0)
			opt(ctx->ops->do_iter)(ctx);

		opt(ctx->ops->post_iter)(ctx);

		key = readkey(ctx);

		switch (key) {
		case KEY_CTRL_D:
		case KEY_ENTER:
		case KEY_ENTER2:
			run = 0;
			break;
		case KEY_BACKSPACE:
		case KEY_BACKSPACE2:
			if (len)
				str[--len] = 0;
			break;
		default:
			if (key != 0) {
				if (((key >= '!') && (key <= '~')) ||
				    (key == ' ')) {
					str[len] = key;
					len++;
					str[len] = 0;
				}
			}
			break;
		}
	}

	if (ctx->ops->pre_iter)
		ret = ctx->ops->pre_iter(ctx);

	opt(ctx->ops->curs_set)(ctx, 0);

	if (ret == 0)
		opt(ctx->ops->do_iter)(ctx);

	opt(ctx->ops->post_iter)(ctx);
}

#ifdef LINUX
/** Output information message

   \param[in] text Text to print
*/
static void notify(struct top_context *ctx, const char *text)
{
	ctx->ops->move(ctx, ctx->rows - 1, 0);
	ctx->ops->addstr(ctx, text);
	ctx->ops->clrtoeol(ctx);
	opt(ctx->ops->refresh)(ctx);
	readkey(ctx);
}
#endif /* #ifdef LINUX*/

/** Get first line number */
static int first_line_get(struct top_context *ctx)
{
	char buff[TOP_LINE_LEN];
	int line;

	for (line = 0; line < active_page_state(ctx)->total; line++) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0)
			return line;
	}

	return 0;
}

/** Get nth last line

   \param[in] nth nth line number
*/
static int last_line_get(struct top_context *ctx, int nth)
{
	char buff[TOP_LINE_LEN];
	int line;

	for (line = active_page_state(ctx)->total - 1; line >= 0; line--) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0) {
			if (nth > 0)
				nth -= lines_num(ctx, p);

			if (nth <= 0)
				return line;
		}
	}

	return -1;
}

/** Get next line after given

   \param[in] start Start search from this line
*/
static int next_line_get(struct top_context *ctx, int start)
{
	char buff[TOP_LINE_LEN];
	int line;

	for (line = start + 1; line < active_page_state(ctx)->total - 1; line++) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0)
			return line;
	}

	return -1;
}

/** Get next line before given

   \param[in] start Start search from this line
*/
static int prev_line_get(struct top_context *ctx, int start)
{
	char buff[TOP_LINE_LEN];
	int line;

	for (line = start - 1; line >= 0; line--) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0)
			return line;
	}

	return -1;
}

/** Get next page after given

   \param[in] start Start search from this line
*/
static int next_page_get(struct top_context *ctx, int start)
{
	char buff[TOP_LINE_LEN];
	int line;
	int page_lines = ctx->rows - 2;

	for (line = start + 1; line < active_page_state(ctx)->total - 1; line++) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0) {
			page_lines -= lines_num(ctx, p);

			if (page_lines <= 0) {
				return line;
			}
		}
	}

	return -1;
}

/** Get next page before given

   \param[in] start Start search from this line
*/
static int prev_page_get(struct top_context *ctx, int start)
{
	char buff[TOP_LINE_LEN];
	int line;
	int page_lines = ctx->rows - 2;

	for (line = start - 1; line > 0; line--) {
		char *p = active_page(ctx)->line_get(ctx, line, buff);

		if (!p)
			p = buff;

		if (is_filtered(ctx, p) == 0) {
			page_lines -= lines_num(ctx, p);

			if (page_lines <= 0) {
				return line;
			}
		}
	}

	return -1;
}

static void dump_all_tables(struct top_context *ctx, const char *top_file)
{
	unsigned int i;
	FILE *f;

#ifdef LINUX
	f = fopen(top_file, "w");
	if (!f) {
		fprintf(stderr, "Can't save dump to %s\n", top_file);
		return;
	}
#else
	f = stdout;
#endif

	for (i = 0; i < ctx->page_num; i++) {
		if (activity_check(ctx, f))
			break;

		if (counters_fetch(ctx, i) >= 0) {
			table_write(ctx, f, i);
			fprintf(f, "\n");
		}
	}

#ifdef LINUX
	fclose(f);
	printf("Saved dump to %s\n", top_file);
#endif

}

/** Count cursor position in percents */
static inline unsigned int pos_percent(unsigned int pos,
				       unsigned int total)
{
	if (pos == 0 || total == 0)
		return 0;

	return pos * 100 / total;
}

/** Return 1 if any counters group has been selected */
static int is_cnt_selected(struct top_context *ctx)
{
	return ctx->page_sel != 0xFFFFFFFF;
}

/** Select new counters page */
static void cnt_select(struct top_context *ctx, unsigned int net_page_sel)
{
	unsigned int prev_sel_cnt_grp = ctx->page_sel;

	if (is_cnt_selected(ctx) &&
	   prev_sel_cnt_grp != net_page_sel &&
	   ctx->page[prev_sel_cnt_grp].page_leave)
			ctx->page[prev_sel_cnt_grp].page_leave(ctx);

	ctx->page_sel = net_page_sel;

	if (is_cnt_selected(ctx) &&
	    prev_sel_cnt_grp != net_page_sel &&
	    ctx->page[net_page_sel].page_enter)
		ctx->page[net_page_sel].page_enter(ctx);
}

#ifdef ECOS
#define TICKS_PER_SEC 100
static void gettimeofday(struct timeval *tv, int unused)
{
	cyg_tick_count_t ticks = cyg_current_time();

	tv->tv_sec = ticks / TICKS_PER_SEC;
	tv->tv_usec = (ticks % TICKS_PER_SEC) * 10 * 1000;
}
#endif

#define NEED_REDRAW   (1 << 0)
#define NEED_UPDATE   (1 << 1)
#define NEED_SHUTDOWN (1 << 2)

/** Handle key and return true when we need to update page */
static int ui_process_key(struct top_context *ctx, int key)
{
	int line;
	static int group_key = 0;
#ifdef LINUX
	char buff[TOP_LINE_LEN];
	FILE *cnt_dump;
	struct timeval tv;
#endif
	unsigned int i;

	switch (key) {
	case 0:
		break;

	case KEY_ENTER:
		group_key = 0;
		break;

	case '/':
		prompt(ctx, "/", ctx->filter);

		line = prev_line_get(ctx, active_page_state(ctx)->start);
		if (line >= 0) {
			active_page_state(ctx)->start = line;
		} else {
			active_page_state(ctx)->start = first_line_get(ctx);
		}

		break;

	case KEY_HOME:
		line = first_line_get(ctx);

		if (active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = first_line_get(ctx);
		break;

	case KEY_END:
		line = last_line_get(ctx, ctx->rows - 2);
		if (line >= 0 && active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = line;
		break;

	case KEY_CTRL_X:
		return NEED_SHUTDOWN;

	case KEY_CTRL_E:
	case KEY_DOWN:
		line = next_line_get(ctx, active_page_state(ctx)->start);
		if (line >= 0 && active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = line;
		ctx->clear_screen_on_update = 1;

		break;

	case KEY_CTRL_Y:
	case KEY_UP:
		line = prev_line_get(ctx, active_page_state(ctx)->start);
		if (line >= 0 && active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = line;
		ctx->clear_screen_on_update = 1;

		break;

	case KEY_CTRL_D:
	case KEY_NPAGE:
		line = next_page_get(ctx, active_page_state(ctx)->start);
		if (line >= 0 && active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = line;
		ctx->clear_screen_on_update = 1;

		break;

	case KEY_CTRL_U:
	case KEY_PPAGE:
		line = prev_page_get(ctx, active_page_state(ctx)->start);
		if (line >= 0 && active_page_state(ctx)->start != line)
			active_page_state(ctx)->start = line;
		else if (line < 0 && active_page_state(ctx)->start != 0)
			active_page_state(ctx)->start = 0;
		ctx->clear_screen_on_update = 1;

		break;

#ifdef LINUX
	case KEY_CTRL_W:
		gettimeofday(&tv, 0);
		sprintf(buff, "/tmp/%lld_%lld_%s.txt",
			(long long)tv.tv_sec, (long long)tv.tv_usec,
			active_page(ctx)->name);

		cnt_dump = fopen(buff, "w");
		if (!cnt_dump) {
			ctx->ops->clear(ctx);

			sprintf(buff, "Can't save counters to "
				"'/tmp/%lld_%lld_%s.txt'. "
				"Press any key to continue...",
				(long long)tv.tv_sec, (long long)tv.tv_usec,
				active_page(ctx)->name);
			notify(ctx, buff);

			ctx->ops->clear(ctx);
			break;
		}

		table_write(ctx, cnt_dump, ctx->page_sel);

		fclose(cnt_dump);

		ctx->ops->clear(ctx);

		sprintf(buff, "Saved to '/tmp/%lld_%lld_%s.txt'. "
			"Press any key to continue...",
			(long long)tv.tv_sec, (long long)tv.tv_usec,
			active_page(ctx)->name);
		notify(ctx, buff);

		ctx->ops->clear(ctx);
		break;

	case KEY_CTRL_A:
		gettimeofday(&tv, 0);
		sprintf(buff, "/tmp/%lld_%lld.txt",
			(long long)tv.tv_sec, (long long)tv.tv_usec);

		ctx->ops->clear(ctx);
		ctx->ops->move(ctx, ctx->rows - 1, 0);
		ctx->ops->addstr(ctx, "Fetching all counters...");
		opt(ctx->ops->refresh)(ctx);

		cnt_dump = fopen(buff, "w");
		if (!cnt_dump) {
			ctx->ops->clear(ctx);

			sprintf(buff, "Can't save counters to "
				"'/tmp/%lld_%lld.txt'. "
				"Press any key to continue...",
				(long long)tv.tv_sec, (long long)tv.tv_usec);
			notify(ctx, buff);

			ctx->ops->clear(ctx);
			break;
		}

		for (i = 0; i < ctx->page_num; i++) {
			if (activity_check(ctx, cnt_dump))
				break;

			if (counters_fetch(ctx, i) >= 0) {
				table_write(ctx, cnt_dump, i);
				fprintf(cnt_dump, "\n");
			}
		}

		fclose(cnt_dump);

		ctx->ops->clear(ctx);

		sprintf(buff, "Saved to '/tmp/%lld_%lld.txt'. "
			"Press any key to continue...",
			(long long)tv.tv_sec, (long long)tv.tv_usec);
		notify(ctx, buff);

		ctx->ops->clear(ctx);
		break;
#endif

	default:

		if (ctx->custom_key) {
			if (ctx->custom_key(ctx, key) == 0)
				return 0;
		}

		for (i = 0; i < ctx->page_num; i++) {
			if (group_key == ctx->page[i].group_key
			    && key == ctx->page[i].key) {
				group_key = 0;
				cnt_select(ctx, i);

				/* new page is selected, fetch it and redraw
				 * the screen */
				ctx->clear_screen_on_update = 1;
				return NEED_UPDATE | NEED_REDRAW;
			}
		}

		if (!group_key) {
			for (i = 0; i < ctx->page_num; i++) {
				if (ctx->page[i].group_key == key) {
					group_key = key;
					break;
				}
			}
		} else {
			group_key = 0;
		}

		/* press key for unknown page, don't need to do anything */
		return 0;
	}

	/* use selected something besides new page (move around) so just
	 * redraw the screen */
	return NEED_REDRAW;
}

/** Fetch new page values (when NEED_UPDATE) and
 *  refresh screen (when NEED_REDRAW) */
static void ui_redraw(struct top_context *ctx, int need)
{
	char buff[TOP_LINE_LEN];

#ifdef SIGWINCH
	if (g_need_resize) {
		ctx->ops->terminal_size_get(ctx);

		g_need_resize = 0;
	}
#endif

	if (!active_page(ctx)->line_get || !active_page(ctx)->page_get) {
		fprintf(stderr, "ERROR: Can't retrieve "
				"table data for %s; "
				"no handler defined\n", active_page(ctx)->name);
		abort();
	} else {
		if (need & NEED_UPDATE) {
			if (activity_check(ctx, stderr))
				return;

			if (ctx->clear_screen_on_update) {
				ctx->ops->clear(ctx);
				ctx->clear_screen_on_update = 0;
			}

			(void)counters_fetch(ctx, ctx->page_sel);
		}
	}

	if (need & NEED_REDRAW) {
		int line;
		const char *p;
		unsigned int y;

		buff[0] = 0;
		p = active_page(ctx)->line_get(ctx, -1, buff);
		if (p == NULL) {
			if (buff[0] == 0)
				p = active_page(ctx)->name;
			else
				p = buff;
		}

		ctx->ops->move(ctx, 0, 0);
		opt(ctx->ops->attron)(ctx, A_UNDERLINE);
		ctx->ops->addstr(ctx, p);
		ctx->ops->clrtoeol(ctx);
		opt(ctx->ops->attroff)(ctx, A_UNDERLINE);

		/* data */
		for (line = active_page_state(ctx)->start, y = 1;
		     y + 1 < ctx->rows;
		     line++) {
			if (line >= active_page_state(ctx)->total) {
				ctx->ops->move(ctx, y, 0);
				ctx->ops->clrtoeol(ctx);
				y++;
				continue;
			}

			buff[0] = 0;
			p = active_page(ctx)->line_get(ctx, line, buff);
			if (p == NULL && buff[0] != 0)
				p = buff;
			if (p && is_filtered(ctx, p) == 0) {
				ctx->ops->move(ctx, y, 0);
				ctx->ops->addstr(ctx, p);
				y += lines_num(ctx, p);
				ctx->ops->clrtoeol(ctx);
			}
		}

		/* footer */
		ctx->ops->move(ctx, ctx->rows - 1, 0);
		ctx->ops->clrtoeol(ctx);
		ctx->ops->addstr(ctx, "Press ? or Ctrl-h for help");

		sprintf(buff,
			"%-30s                    Delay: %ums  %3d%%",
			active_page(ctx)->name,
			ctx->upd_delay,
			pos_percent(active_page_state(ctx)->start,
				    active_page_state(ctx)->total));

		ctx->ops->move(ctx, ctx->rows - 1,
			       ctx->cols - (int)strlen(buff) - 1);
		ctx->ops->addstr(ctx, buff);

		opt(ctx->ops->refresh)(ctx);
	}
}

static int top_ui_update_check(struct top_context *ctx, struct timeval *upd_time)
{
	struct timeval tv;

	gettimeofday(&tv, 0);

	if ((unsigned int)tv.tv_sec == upd_time->tv_sec + ctx->upd_delay / 1000) {
		if ((unsigned int)tv.tv_usec - upd_time->tv_usec > ctx->upd_delay % 1000 * 1000) {
			*upd_time = tv;
			return NEED_UPDATE | NEED_REDRAW;
		}
	} else {
		if ((unsigned int)tv.tv_sec - upd_time->tv_sec > ctx->upd_delay / 1000) {
			*upd_time = tv;
			return NEED_UPDATE | NEED_REDRAW;
		}
	}

	return 0;
}

/** Main window handler */
void top_ui_main_loop(struct top_context *ctx)
{
	int ret = 0;
	int key;
	struct timeval upd_time;
	int action = NEED_UPDATE | NEED_REDRAW;

	if (!is_cnt_selected(ctx))
		cnt_select(ctx, 0);

	gettimeofday(&upd_time, 0);

	while (1) {
		if (action) {
			if (ctx->ops->pre_iter)
				ret = ctx->ops->pre_iter(ctx);

			if (ret == 0) {
				ui_redraw(ctx, action);

				opt(ctx->ops->do_iter)(ctx);
			}

			opt(ctx->ops->post_iter)(ctx);
		}

		if (ctx->ops->hasch(ctx)) {
			key = readkey(ctx);

			if (key == KEY_CTRL_C)
				break;

			action = ui_process_key(ctx, key);
		} else {
			action = 0;
		}

		if ((action & NEED_SHUTDOWN) || ctx->need_shutdown)
			break;
#ifdef LINUX
		if (g_need_shutdown)
			break;
#endif

		action |= top_ui_update_check(ctx, &upd_time);
	}
}

int top_init(struct top_context *ctx,
             const struct top_operations *const ops,
             int fd,
             const struct top_page_desc *page,
             const unsigned int page_num,
             top_page_init_t *page_init,
             unsigned int page_init_num,
             unsigned int upd_delay,
             top_activity_check_t *activity_check,
             top_custom_key_t *custom_key,
             void *priv)
{

	ctx->ops = ops;
	ctx->priv = priv;
	ctx->fd = fd;

	ctx->page = page;
	ctx->page_num = page_num;
	ctx->page_init = page_init;
	ctx->page_init_num = page_init_num;
	ctx->page_sel = 0xFFFFFFFF;
	ctx->upd_delay = upd_delay;
	ctx->filter[0] = '\0';
	ctx->need_shutdown = 0;
	ctx->activity_check = activity_check;
	ctx->custom_key = custom_key;

	ctx->page_state = malloc(sizeof(struct top_page_state) * page_num);
	if (!ctx->page_state)
		return -1;

	memset(ctx->page_state, 0, sizeof(struct top_page_state) * page_num);

	pages_init(ctx, true);

	return 0;
}

void top_shutdown(struct top_context *ctx)
{
	free(ctx->page_state);
	ctx->page_state = NULL;
}

void top_batch(struct top_context *ctx, const char *top_file)
{
	if (is_cnt_selected(ctx)) {
		int ret = 0;

		if (ctx->page_sel >= ctx->page_num)
			cnt_select(ctx, 0);
		else
			cnt_select(ctx, ctx->page_sel);

		if (activity_check(ctx, stderr))
			return;

		(void)counters_fetch(ctx, ctx->page_sel);

		if (ctx->ops->pre_iter)
			ret = ctx->ops->pre_iter(ctx);
		if (ret == 0) {
			table_write(ctx, stdout, ctx->page_sel);
			opt(ctx->ops->do_iter)(ctx);
		}
		opt(ctx->ops->post_iter)(ctx);
	} else {
		dump_all_tables(ctx, top_file);
	}
}

void top_ui_prepare(struct top_context *ctx)
{
#ifdef LINUX
	signal(SIGINT, shutdown);
	signal(SIGSEGV, shutdown);
#endif
	ctx->ops->terminal_size_get(ctx);

	opt(ctx->ops->cbreak)(ctx);
	opt(ctx->ops->curs_set)(ctx, 0);

#ifdef SIGWINCH
	signal(SIGWINCH, resize);
#endif
}

void top_ui_shutdown(struct top_context *ctx)
{
	ctx->ops->terminal_size_get(ctx);

	opt(ctx->ops->curs_set)(ctx, 1);

	opt(ctx->ops->endwin)(ctx);
#ifdef LINUX
	/* make sure the cursor for prompt is below last outputs */
	printf("\n\n");
#endif

	pages_init(ctx, false);
}

void top_ui_stop(struct top_context *ctx)
{
	ctx->need_shutdown = 1;
}

int top_select_group(struct top_context *ctx, const char *group)
{
	unsigned int u;

	cnt_select(ctx, 0xFFFFFFFF);
	for (u = 0; u < ctx->page_num; u++) {
		if (strlen(group) == 1) {
			if (ctx->page[u].group_key == 0
			    && ctx->page[u].key == group[0]) {
				cnt_select(ctx, u);
				break;
			}
		} else {
			if (strcmp(ctx->page[u].name, group) == 0) {
				cnt_select(ctx, u);
				break;
			}
		}
	}

	return is_cnt_selected(ctx);
}

void top_upd_delay_set(struct top_context *ctx, unsigned int upd_delay)
{
	ctx->upd_delay = upd_delay;
}

#ifdef LINUX
void top_print_groups(struct top_context *ctx)
{
	unsigned int u;

	for (u = 0; u < ctx->page_num; u++) {
		if (ctx->page[u].group_key == 0)
			printf("\t                     %c - %s\n",
			       ctx->page[u].key,
			       ctx->page[u].name);
		else
			printf("\t                     %s\n",
			       ctx->page[u].name);
	}
}
#endif
