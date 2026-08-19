/*****************************************************************************
 *
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <malloc.h>
#include <string.h>
#include <uci.h>

#include "uci_config.h"

int uci_config_get(const char *path, const char *sec, const char *opt,
		   char *out)
{
	struct uci_context *uci;
	char tuple[UCI_TUPLE_STR_MAX_SIZE];
	struct uci_ptr ptr;
	int len, ret = 0;

	if (!out)
		return -1;

	if (path && sec) {
		len = sprintf(tuple, "%s.%s", path, sec);
		if (len < 0)
			return -1;
	} else {
		return -1;
	}

	if (opt)
		if (sprintf(&tuple[len], ".%s", opt) < 0)
			return -1;

	uci = uci_alloc_context();
	if (!uci)
		return -1;

	if (uci_lookup_ptr(uci, &ptr, tuple, true) != UCI_OK) {
		ret = -1;
		goto on_exit;
	}

	if (!(ptr.flags & UCI_LOOKUP_COMPLETE)) {
		ret = -1;
		goto on_exit;
	}

	switch (ptr.last->type) {
	case UCI_TYPE_SECTION:
		len = strlen(ptr.s->type);
		if (len > UCI_PARAM_STR_MAX_SIZE) {
			ret = -1;
			break;
		}
		strcpy(out, ptr.s->type);
		break;
	case UCI_TYPE_OPTION:
		switch (ptr.o->type) {
		case UCI_TYPE_STRING:
			len = strlen(ptr.o->v.string);
			if (len > UCI_PARAM_STR_MAX_SIZE) {
				ret = -1;
				break;
			}
			strcpy(out, ptr.o->v.string);
			break;
		default:
			ret = -1;
			break;
		}
		break;
	default:
		ret = -1;
		break;
	}

on_exit:
	uci_free_context(uci);
	return ret;
}
