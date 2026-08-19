/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include "dsl_fapid.h"
#include "dsl_fapi_cli.h"
#include <unistd.h>
#include <getopt.h>

static int g_entity = 0;
static bool g_console = false;
static char g_buf[256] = { 0 };

static bool g_help = false;

static struct option long_options[] = {
/*  0 */ {"help   ", no_argument, NULL, 'h'},
/*  1 */ {"console", no_argument, NULL, 'c'},
/*  2 */ {"entity ", required_argument, NULL, 'e'},
/*  3 */ {"init   ", required_argument, NULL, 'i'},
	{NULL, 0, NULL, 0}
};

static char description[][64] = {
/*  0 */ {"help screen"},
/*  1 */ {"use console"},
/*  2 */ {"entity (-e 0/1)"},
/*  3 */ {"init cfg file (-i dsl_fapi.cfg)"},
	{0}
};

#define GETOPT_LONG_OPTSTRING "hce:i:"

static void args_parse(int argc, char *argv[])
{
	int parm_no = 0;

	while (1) {
		int option_index = 0;
		int c;

		/* 1 colon means there is a required parameter */
		/* 2 colons means there is an optional parameter */
		c = getopt_long(argc, argv, GETOPT_LONG_OPTSTRING, long_options, &option_index);

		if (c == -1) {
			if (parm_no == 0)
				g_help = true;
			return;
		}

		parm_no++;

		switch (c) {
		case 'h':
			g_help = true;
			break;
		case 'c':
			g_console = true;
			break;
		case 'e':
			g_entity = atoi(optarg);
			break;
		case 'i':
			memcpy(g_buf, optarg, strlen(optarg));
			break;
		}		/* switch(c) {...} */
	}			/* while(1) {...} */

	if (optind < argc) {
		printf(DSL_FAPI "Sorry, there are unrecognized options: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
}

static void help_print(void)
{
	struct option *ptr;
	char *desc = description[0];
	ptr = long_options;

	printf("usage: DSL FAPI CLI [options]\n");
	printf("following options defined:\n");

	while (ptr->name) {
		printf(" --%s (-%c)\t- %s\n", ptr->name, ptr->val, desc);
		ptr++;
		desc += sizeof(description[0]);
	}

	printf("\n");
	return;
}

int main(int argc, char *argv[])
{
	int ret = 0;

	args_parse(argc, argv);

	if (g_help == true) {
		help_print();
		return ret;
	}

	ret = cli_start(g_console, g_entity, g_buf);

	return ret;
}
