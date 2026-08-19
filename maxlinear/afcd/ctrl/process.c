/*
 * Command line editing and history
 * Copyright (c) 2010-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 */

#include <unistd.h>
#include <termios.h>
#include "eloop.h"
#include "ctrl.h"
#include "process.h"
#include "afc.h"

void (*edit_cmd_cb)(void *ctx, char *cmd, struct afc_ctrl *ctrl);
void (*edit_eof_cb)(int sig, void *ctx);
#define CMD_BUF_LEN 4096
static char cmdbuf[CMD_BUF_LEN];
static int currbuf_valid;
static int cmdbuf_len;
static int cmdbuf_pos;
static struct termios prevt, newt;
static const char *ps2;

enum edit_key_code {
	EDIT_KEY_NONE = 256,
	EDIT_KEY_TAB,
	EDIT_KEY_UP,
	EDIT_KEY_DOWN,
	EDIT_KEY_RIGHT,
	EDIT_KEY_LEFT,
	EDIT_KEY_ENTER,
	EDIT_KEY_BACKSPACE,
	EDIT_KEY_INSERT,
	EDIT_KEY_DELETE,
	EDIT_KEY_HOME,
	EDIT_KEY_END,
	EDIT_KEY_PAGE_UP,
	EDIT_KEY_PAGE_DOWN,
	EDIT_KEY_F1,
	EDIT_KEY_F2,
	EDIT_KEY_F3,
	EDIT_KEY_F4,
	EDIT_KEY_CTRL_B,
	EDIT_KEY_CTRL_F,
	EDIT_KEY_CTRL_J,
	EDIT_KEY_EOF
};

void edit_redraw(void)
{
	char tmp;

	cmdbuf[cmdbuf_len] = '\0';
	printf("\r%s> %s", ps2 ? ps2 : "", cmdbuf);
	if (cmdbuf_pos != cmdbuf_len) {
		tmp = cmdbuf[cmdbuf_pos];
		cmdbuf[cmdbuf_pos] = '\0';
		printf("\r%s> %s", ps2 ? ps2 : "", cmdbuf);
		cmdbuf[cmdbuf_pos] = tmp;
	}
	fflush(stdout);
}

static void insert_char(int c)
{
	if (cmdbuf_len >= (int)sizeof(cmdbuf) - 1)
		return;
	if (cmdbuf_len == cmdbuf_pos) {
		cmdbuf[cmdbuf_pos++] = c;
		cmdbuf_len++;
		putchar(c);
		fflush(stdout);
	} else {
		memmove(cmdbuf + cmdbuf_pos + 1, cmdbuf + cmdbuf_pos,
			cmdbuf_len - cmdbuf_pos);
		cmdbuf[cmdbuf_pos++] = c;
		cmdbuf_len++;
		edit_redraw();
	}
}

void edit_clear_line(void)
{
	int i;

	putchar('\r');
	for (i = 0; i < cmdbuf_len + 2 + (ps2 ? (int)strlen(ps2) : 0); i++)
	putchar(' ');
}

static void delete_left(void)
{
	if (cmdbuf_pos == 0)
		return;

	edit_clear_line();
	memmove(cmdbuf + cmdbuf_pos - 1, cmdbuf + cmdbuf_pos,
		cmdbuf_len - cmdbuf_pos);
	cmdbuf_pos--;
	cmdbuf_len--;
	edit_redraw();
}

static void move_start(void)
{
	cmdbuf_pos = 0;
	edit_redraw();
}

static void move_end(void)
{
	cmdbuf_pos = cmdbuf_len;
	edit_redraw();
}

static void move_right(void)
{
	if (cmdbuf_pos < cmdbuf_len) {
		cmdbuf_pos++;
		edit_redraw();
	}
}

static void move_left(void)
{
	if (cmdbuf_pos > 0) {
		cmdbuf_pos--;
		edit_redraw();
	}
}

static void show_esc_buf(const char *esc_buf, char c, int i)
{
	edit_clear_line();
	printf("\rESC buffer '%s' c='%c' [%d]\n", esc_buf, c, i);
	edit_redraw();
}

static void process_cmd(struct afc_ctrl *ctrl)
{
	currbuf_valid = 0;
	if (cmdbuf_len == 0) {
		printf("\n%s> ", ps2 ? ps2 : "");
		fflush(stdout);
		return;
	}
	printf("\n");
	cmdbuf[cmdbuf_len] = '\0';
	cmdbuf_pos = 0;
	cmdbuf_len = 0;
	edit_cmd_cb(NULL, cmdbuf, ctrl);
	printf("%s> ", ps2 ? ps2 : "");
	fflush(stdout);
}

static enum edit_key_code esc_seq_to_key1_no(char last)
{
	switch (last) {
	case 'A':
		return EDIT_KEY_UP;
	case 'B':
		return EDIT_KEY_DOWN;
	case 'C':
		return EDIT_KEY_RIGHT;
	case 'D':
		return EDIT_KEY_LEFT;
	default:
		return EDIT_KEY_NONE;
	}
}

static enum edit_key_code esc_seq_to_key1(int param1, int param2, char last)
{
	if (param1 < 0 && param2 < 0)
		return esc_seq_to_key1_no(last);

	return EDIT_KEY_NONE;
}

static enum edit_key_code esc_seq_to_key2(int param1, int param2, char last)
{
	if (param1 >= 0 || param2 >= 0)
		return EDIT_KEY_NONE;

	switch (last) {
	case 'F':
		return EDIT_KEY_END;
	case 'H':
		return EDIT_KEY_HOME;
	case 'P':
		return EDIT_KEY_F1;
	case 'Q':
		return EDIT_KEY_F2;
	case 'R':
		return EDIT_KEY_F3;
	case 'S':
		return EDIT_KEY_F4;
	default:
		return EDIT_KEY_NONE;
	}
}

static enum edit_key_code esc_seq_to_key(char *seq)
{
	char last, *pos;
	int param1 = -1, param2 = -1;
	enum edit_key_code ret = EDIT_KEY_NONE;

	last = '\0';
	for (pos = seq; *pos; pos++)
		last = *pos;

	if (seq[1] >= '0' && seq[1] <= '9') {
		param1 = atoi(&seq[1]);
		pos = strchr(seq, ';');
		if (pos)
			param2 = atoi(pos + 1);
	}

	if (seq[0] == '[')
		ret = esc_seq_to_key1(param1, param2, last);
	else if (seq[0] == 'O')
		ret = esc_seq_to_key2(param1, param2, last);

	if (ret != EDIT_KEY_NONE)
		return ret;

	edit_clear_line();
	printf("\rUnknown escape sequence '%s'\n", seq);
	edit_redraw();
	return EDIT_KEY_NONE;
}

static enum edit_key_code edit_read_key(int sock)
{
	int c;
	int res;
	static int esc = -1;
	unsigned char buf[1];
	static char esc_buf[7];

	res = read(sock, buf, 1);
	if (res < 0)
		perror("read");
	if (res <= 0)
		return EDIT_KEY_EOF;

	c = buf[0];

	if (esc >= 0) {
		if (c == 27 /* ESC */) {
			esc = 0;
			return EDIT_KEY_NONE;
		}

		if (esc == 6) {
			show_esc_buf(esc_buf, c, 0);
			esc = -1;
		} else {
			esc_buf[esc++] = c;
			esc_buf[esc] = '\0';
		}
	}

	if (esc == 1) {
		if (esc_buf[0] != '[' && esc_buf[0] != 'O') {
			show_esc_buf(esc_buf, c, 1);
			esc = -1;
			return EDIT_KEY_NONE;
		} else {
			return EDIT_KEY_NONE; /* Escape sequence continues */
		}
	}

	if (esc > 1) {
		if ((c >= '0' && c <= '9') || c == ';')
			return EDIT_KEY_NONE; /* Escape sequence continues */

		if (c == '~' || (c >= 'A' && c <= 'Z')) {
			esc = -1;
			return esc_seq_to_key(esc_buf);
		}

		show_esc_buf(esc_buf, c, 2);
		esc = -1;
		return EDIT_KEY_NONE;
	}

	switch (c) {
	case 2:
		return EDIT_KEY_CTRL_B;
	case 6:
		return EDIT_KEY_CTRL_F;
	case 9:
		return EDIT_KEY_TAB;
	case 10:
		return EDIT_KEY_CTRL_J;
	case 13: /* CR */
		return EDIT_KEY_ENTER;
	case 27: /* ESC */
		esc = 0;
		return EDIT_KEY_NONE;
	case 127:
		return EDIT_KEY_BACKSPACE;
	default:
		return c;
	}
}

static void edit_read_char(int sock, void *eloop_ctx, void *sock_ctx)
{
	enum edit_key_code c;
	struct afc_ctrl *ctrl = (struct afc_ctrl *)eloop_ctx;
	UNUSED_PARAM(sock_ctx);

	c = edit_read_key(sock);

	switch (c) {
	case EDIT_KEY_RIGHT:
	case EDIT_KEY_CTRL_F:
		move_right();
		break;
	case EDIT_KEY_LEFT:
	case EDIT_KEY_CTRL_B:
		move_left();
		break;
	case EDIT_KEY_EOF:
		printf("IN EOF");
		edit_eof_cb(0, NULL);
		break;
	case EDIT_KEY_CTRL_J:
	case EDIT_KEY_ENTER:
		process_cmd(ctrl);
		break;
	case EDIT_KEY_BACKSPACE:
		delete_left();
		break;
	case EDIT_KEY_END:
		move_end();
		break;
	case EDIT_KEY_HOME:
		move_start();
		break;
	case EDIT_KEY_NONE:
		break;
	default:
		if (c >= 32 && c <= 255)
			insert_char(c);
	}
}

int command_init(void (*cmd_cb)(void *ctx, char *cmd, struct afc_ctrl *ctrl),
		 void (*eof_cb)(int sig, void *ctx),
		 struct afc_ctrl *ctrl, const char *ps)
{
	edit_cmd_cb = cmd_cb;
	edit_eof_cb = eof_cb;

	tcgetattr(STDIN_FILENO, &prevt);
	newt = prevt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	eloop_register_read_sock(STDIN_FILENO, edit_read_char, ctrl, NULL);

	ps2 = ps;
	printf("%s> ", ps2 ? ps2 : "");
	fflush(stdout);

	return 0;
}

void command_deinit(void)
{
	edit_clear_line();
	putchar('\r');
	fflush(stdout);
	eloop_unregister_read_sock(STDIN_FILENO);
	tcsetattr(STDIN_FILENO, TCSANOW, &prevt);
}
