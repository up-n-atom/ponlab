/******************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 * Copyright (c) 2013 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
#ifndef __top_h
#define __top_h

#include "top_config.h"
#include "top_std_defs.h"
#include "top_common.h"
#ifdef LINUX
#include "top_linux.h"
#endif
#ifdef ECOS
#include "top_ecos.h"
#define TOP_LINE_LEN 128
#define TOP_LINE_MAX 128
#endif

#define TOP_ROWS_DEFAULT 38
#define TOP_COLS_DEFAULT 120

struct top_context;

/** Counters group initialization handler */
typedef void (*top_page_init_t) (int init);

/** Get line from the page

   \param[in]  entry Line number; -1 for header
   \param[out] text  Line text

   \return - NULL - means that data in text
           - non-NULL - print returned pointer
*/
typedef char *(top_line_get_t) (struct top_context *ctx, const int entry, char *text);

/** Get complete page from device to application's memory

   \param[in] name of the file to be used

   \return Number of lines in the table
*/
typedef int (top_page_get_t) (struct top_context *ctx, const char *input_file_name);

/** Enter counters page */
typedef void (top_page_enter_t) (struct top_context *ctx);

/** Leave counters page */
typedef void (top_page_leave_t) (struct top_context *ctx);

/** Counters group description */
struct top_page_desc {
	/** ASCII symbol which uniquely identifies counters group
	  (should be A-Z, a-z, 0-9) */
	int group_key;

	/** ASCII symbol which uniquely identifies counters table
	  (should be A-Z, a-z, 0-9) */
	int key;

	/** Counters group name */
	const char *name;

	/** Get line */
	top_line_get_t *line_get;
	/** Get page */
	top_page_get_t *page_get;
	/** Enter page */
	top_page_enter_t *page_enter;
	/** Leave page */
	top_page_leave_t *page_leave;

	/* path to the profs file to be used */
	const char* input_file_name;
};

/** "Ctrl-A" key definition */
#define KEY_CTRL_A 1

/** "Ctrl-B" key definition */
#define KEY_CTRL_B 2

/** "Ctrl-C" key definition */
#define KEY_CTRL_C 3

/** "Ctrl-D" key definition */
#define KEY_CTRL_D 4

/** "Ctrl-E" key definition */
#define KEY_CTRL_E 5

/** "Ctrl-F" key definition */
#define KEY_CTRL_F 6

/** "Ctrl-H" key definition */
#define KEY_CTRL_H 8

/** "Ctrl-R" key definition */
#define KEY_CTRL_R 18

/** "Ctrl-U" key definition */
#define KEY_CTRL_U 21

/** "Ctrl-V" key definition */
#define KEY_CTRL_V 22

/** "Ctrl-W" key definition */
#define KEY_CTRL_W 23

/** "Ctrl-X" key definition */
#define KEY_CTRL_X 24

/** "Ctrl-Y" key definition */
#define KEY_CTRL_Y 25

/** "Backspace 2" key definition (used by default in PuTTY) */
#  define KEY_BACKSPACE2 127

#  include <termios.h>

/** "Enter" key definition */
#  define KEY_ENTER 10

/** "Enter" key definition */
#  define KEY_ENTER2 13

/** "Backspace" key definition */
#  define KEY_BACKSPACE 8

/** "Arrow up" key definition */
#  define KEY_UP 0403

/** "Arrow down" key definition */
#  define KEY_DOWN 0402

/** "Arrow left" key definition */
#  define KEY_LEFT 0404

/** "Arrow right" key definition */
#  define KEY_RIGHT 0405

/** "Page up" key definition */
#  define KEY_PPAGE 0523

/** "Page down" key definition */
#  define KEY_NPAGE 0522

/** "Home" key definition */
#  define KEY_HOME 0406

/** "End" key definition */
#  define KEY_END 0550

/** Print underlined text */
#define A_UNDERLINE 0

/** Print bright text */
#define A_STANDOUT 1

struct top_operations {
	void (*addstr)(struct top_context *ctx, const char *s);
	void (*clrtoeol)(struct top_context *ctx);
	void (*clear)(struct top_context *ctx);
	void (*move)(struct top_context *ctx, int y, int x);
	int (*getch)(struct top_context *ctx);
	int (*hasch)(struct top_context *ctx);
	void (*terminal_size_get)(struct top_context *ctx);

	/* optional */
	void (*attron)(struct top_context *ctx, int attr);
	void (*attroff)(struct top_context *ctx, int attr);

	void (*curs_set)(struct top_context *ctx, int flag);

	void (*refresh)(struct top_context *ctx);

	void (*cbreak)(struct top_context *ctx);
	void (*endwin)(struct top_context *ctx);

	int (*pre_iter)(struct top_context *ctx);
	void (*post_iter)(struct top_context *ctx);
	void (*do_iter)(struct top_context *ctx);
	int (*do_fprintf)(FILE *f, const char *fmt, ...);
	FILE *(*stream)(struct top_context *ctx);
};

typedef int (top_activity_check_t)(struct top_context *ctx, FILE *f);
typedef int (top_custom_key_t)(struct top_context *ctx, const int key);
typedef int (top_do_fprintf_t)(FILE *f, const char *fmt, ...);

/** Runtime page state */
struct top_page_state {
	/** Start line, used for scrolling */
	int start;
	/** Total line number */
	int total;
};

struct top_context {
	/** Terminal handling operations */
	struct top_operations const *ops;
	/** Device FD */
	int fd;
	/** Terminal size */
	unsigned int cols, rows;
	/** Additional terminal screen clear flag */
	int clear_screen_on_update;

	/** Array of page init handlers */
	top_page_init_t *page_init;
	/** Number of page init handlers */
	unsigned int page_init_num;

	/** Array of pages */
	const struct top_page_desc *page;
	/** Total number of pages */
	unsigned int page_num;
	/** Selected page index */
	unsigned int page_sel;
	/** Pages state */
	struct top_page_state *page_state;

	/** Counters update delay (in ms) */
	unsigned int upd_delay;

	/** Filter string */
	char filter[TOP_LINE_LEN];
	/** Buffer for the file data (for data exported via /proc) */
	char shared_buff[TOP_LINE_MAX][TOP_LINE_LEN];
	/** pointer to the lines */
	char *line_cache[TOP_LINE_MAX];

	/** Request main loop shutdown */
	volatile int need_shutdown;

	top_activity_check_t *activity_check;
	top_custom_key_t *custom_key;

	void *priv;
};

/** Initialize top */
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
             void *priv);

/** Shutdown top */
void top_shutdown(struct top_context *ctx);

/** Run top in batch mode */
void top_batch(struct top_context *ctx, const char *top_file);

/** Prepare UI (switch to non-canonical mode) */
void top_ui_prepare(struct top_context *ctx);
/** Start main loop */
void top_ui_main_loop(struct top_context *ctx);
/** Shutdown TUI (switch back to canonical mode) */
void top_ui_shutdown(struct top_context *ctx);
/** Stop main loop */
void top_ui_stop(struct top_context *ctx);

/** Select page by name */
int top_select_group(struct top_context *ctx, const char *group);

/** Configure update time */
void top_upd_delay_set(struct top_context *ctx, unsigned int upd_delay);

/** Print available pages to stdout */
void top_print_groups(struct top_context *ctx);

#endif
