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

#include "gpon_libs_config.h"
#include "top.h"

#include <sys/time.h>
#include <sys/ioctl.h>

int help_get(struct top_context *ctx, const char *dummy)
{
	unsigned int i;

	static const char *help_predef[] = {
		" ",
		" Up, Ctrl-y      Scroll up                     "
		"Pg up, Ctrl-u   Scroll page up",
		" Down, Ctrl-e    Scroll down                   "
		"Pg down, Ctrl-d Scroll page down",
		" Home            Jump to first line            "
		"End             Jump to last line",
		" /               Define filter                 "
		"Enter           Drop group key",
		" ",
#ifdef LINUX
		" Ctrl-w          Write selected (current page) "
		"counters to file /tmp/<Date>_<Time>_<Group>.txt",
		" Ctrl-a          Dump all pages to file "
		"/tmp/<Date>_<Time>.txt",
		"",
#endif
		" Ctrl-x, Ctrl-c  Exit program",
		""
	};

	ctx->line_cache[0] = NULL;

	for (i = 0; i < ARRAY_SIZE(help_predef); i++)
		strcpy(ctx->shared_buff[i], help_predef[i]);

	for (i = 0; i < ctx->page_num; i += 2)
		if (i + 1 < ctx->page_num)
			help_entry_render(ctx,
					  ctx->shared_buff[ARRAY_SIZE(help_predef)
					  + i / 2], i, i + 1);
		else
			help_entry_render(ctx,
					  ctx->shared_buff[ARRAY_SIZE(help_predef)
					  + i / 2], i, -1);

	return ARRAY_SIZE(help_predef)
		+ ctx->page_num / 2
		+ ctx->page_num % 2;
}

void help_entry_render(struct top_context *ctx,
		       char *buff, int group1, int group2)
{
	char shortcut1[4] = "";

	if (ctx->page[group1].group_key) {
		shortcut1[0] = ctx->page[group1].group_key;
		shortcut1[1] = '-';
		shortcut1[2] = ctx->page[group1].key;
		shortcut1[3] = '\0';
	} else {
		shortcut1[0] = ctx->page[group1].key;
		shortcut1[1] = '\0';
	}

	if (group2 >= 0) {
		char shortcut2[4] = "";

		if (ctx->page[group2].group_key) {
			shortcut2[0] = ctx->page[group2].group_key;
			shortcut2[1] = '-';
			shortcut2[2] = ctx->page[group2].key;
			shortcut2[3] = '\0';
		} else {
			shortcut2[0] = ctx->page[group2].key;
			shortcut2[1] = '\0';
		}

		sprintf(buff,
			" %-3s             %-30s"
			"%-3s             %s",
			shortcut1,
			ctx->page[group1].name,
			shortcut2,
			ctx->page[group2].name);
	} else {
		sprintf(buff,
			" %-3s             %s",
			shortcut1,
			ctx->page[group1].name);
	}
}

char *top_proc_line_get(struct top_context *ctx, const int line, char *text)
{
	if (line != -1 && line < TOP_LINE_MAX) {
		if(ctx->line_cache[0])
			return ctx->line_cache[line];
		else
			return ctx->shared_buff[line];
	}
	return NULL;
}
