/*
 * Command line editing and history
 * Copyright (c) 2010-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 */

#include <string.h>

int command_init(void (*cmd_cb)(void *ctx, char *cmd, struct afc_ctrl *ctrl),
		 void (*eof_cb)(int sig, void *ctx),
		 struct afc_ctrl *ctrl, const char *ps);
void command_deinit(void);