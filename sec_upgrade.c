/******************************************************************************

  Copyright © 2020-2025 MaxLinear, Inc.
  Copyright (C) 2019 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sec_upgrade.c 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include "type-up.h"
#include <mtd/mtd-user.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include <fapi_sec_service.h>
#include <fapi_img_auth.h>
#include <sec_upgrade.h>

#include "safe_str_lib.h"
#include <sys/file.h>
#include <signal.h>

static bool activate=true;
#define MAX_PATH_LEN 256
struct cmdopt {
	unsigned char auth:1;
	unsigned char ext4auth:1;
	unsigned char swbank:1;
	unsigned char commit:1;
	unsigned char reboot:1;
	unsigned char status:1;
	unsigned char upgrade:1;
	unsigned char :0;
};
static struct cmdopt cmdopt;

static void sig_handler(int signo)
{
	switch (signo) {
		case SIGTERM:
			fprintf(stdout, "secupg utility:received SIGTERM(%d)\n", signo);
			break;
		case SIGINT:
			fprintf(stdout, "secupg utility:received SIGINT(%d)\n", signo);
			break;
		case SIGQUIT:
			fprintf(stdout, "secupg utility:received SIGQUIT(%d)\n", signo);
			break;
		case SIGPIPE:
			fprintf(stdout, "secupg utility:received SIGPIPE(%d)\n", signo);
			break;
		case SIGUSR1:
			fprintf(stdout, "secupg utility:received SIGUSR1(%d)\n", signo);
			break;
		case SIGUSR2:
			fprintf(stdout, "secupg utility:received SIGUSR2(%d)\n", signo);
			break;
		case SIGABRT:
			fprintf(stdout, "secupg utility:received SIGABRT(%d)\n", signo);
			break;
		default:
			fprintf(stdout, "secupg utility:received %d\n", signo);
			break;
	}
}
static void trace_signal(void)
{
	if (signal(SIGTERM, sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGTERM\n");

	if (signal(SIGQUIT,sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGQUIT\n");

	if (signal(SIGINT,sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGINT\n");

	if (signal(SIGPIPE,sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGPIPE\n");

	if (signal(SIGUSR1,sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGUSR1\n");

	if (signal(SIGUSR2,sig_handler) == SIG_ERR)
		fprintf(stderr, "\ncan't catch SIGUSR2\n");
}

static void print_version(void)
{
	printf("%s\n", PACKAGE_VERSION);
}

/*
 *	secupg help menu.
 *	Shows an overview about the parameters and the options which are
 *	supported by the secupg utility.
 *
 *	return None
 */
static void print_help(char *prog)
{
	printf("\n");
	printf("secure upgrade util, Version %s\n", PACKAGE_VERSION);
	printf("Usage: %s [options] [parameter]\n", prog);
	printf("\n");
	printf("Options:\n");
	printf("  -a --auth <image>	Authenticate image\n");
	printf("  -e --ext4auth <image>	Authenticate ext4 image containing FIT images\n");
	printf("  -b --swbank <bank>	Switch software bank\n");
	printf("  -c --commit		Commit upgrade\n");
	printf("  -h --help		Display help information\n");
	printf("  -r --reboot		Reboot system\n");
	printf("  -s --status		Check upgrade status\n");
	printf("  -u --upgrade <image>	Upgrade image\n");
	printf("  -v --version		Display version information\n");
}

int main(int argc, char *argv[])
{
	int opt,ret = 0;
	char *image = NULL;
	int option_index = 0;

	char cStatus[MAX_PATH_LEN] = {0};
	char cTime[MAX_PATH_LEN] = {0};
	char *actbnk = NULL;
	trace_signal();

	static struct option long_options[] = {
		{"auth", 1, 0, 'a'},
		{"ext4auth", 1, 0, 'e'},
		{"swbank", 1, 0, 'b'},
		{"commit", 0, 0, 'c'},
		{"help", 0, 0, 'h'},
		{"reboot", 0, 0, 'r'},
		{"status", 0, 0, 's'},
		{"upgrade", 1, 0, 'u'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	if (argc == 1) {
		print_help(argv[0]);
		return 0;
	}

	while ((opt = getopt_long(argc, argv, "a:e:b:chrsu:v", long_options, &option_index)) != -1)
	{
		switch(opt)
		{
			case 'a':
				cmdopt.auth = 1;
				image = optarg;
				break;
			case 'e':
				cmdopt.ext4auth = 1;
				image = optarg;
				break;
			case 'b':
				cmdopt.swbank = 1;
				actbnk = optarg;
				break;
			case 'c':
				cmdopt.commit = 1;
				break;
			case 'h':
				print_help(argv[0]);
				return 0;
			case 'r':
				cmdopt.reboot = 1;
				activate=false;
				break;
			case 's':
				cmdopt.status = 1;
				break;
			case 'u':
				cmdopt.upgrade = 1;
				image = optarg;
				break;
			case 'v':
				print_version();
				return 0;
			case '?':
				printf("invalid or unknown argument: %c\n", optopt);
				print_help(argv[0]);
				return -1;
		}
	}

	for (; optind < argc; optind++) {
		printf("extra arguments: %s\n", argv[optind]);
		return -1;
	}

	if (cmdopt.commit & cmdopt.upgrade) {
		fprintf(stderr, "Image upgrade and commit both are not possible together\n");
		return -1;
	}
	if (cmdopt.swbank & cmdopt.upgrade) {
		fprintf(stderr, "Image upgrade and switch bank both are not possible together\n");
		return -1;
	}
	if (cmdopt.commit & cmdopt.auth) {
		fprintf(stderr, "Image Authentication and commit both are not possible together\n");
		return -1;
	}
	if (cmdopt.auth & cmdopt.upgrade) {
		fprintf(stderr, "Image upgrade and Authentication are invalid, during upgrade image authentication also happens\n");
		return -1;
	}
	if (cmdopt.swbank & cmdopt.auth) {
		fprintf(stderr, "Image Authentication and switch bank both are not possible together\n");
		return -1;
	}
	if (cmdopt.status & cmdopt.auth) {
		fprintf(stderr, "Image Authentication and status both are not possible together\n");
		return -1;
	}
	if (cmdopt.upgrade) {
		ret = fapi_Image_upgrade(image);
		if (ret) {
			fprintf(stderr, "Image upgrade failed \n");
			return ret;
		}
	} else if(cmdopt.commit) {
		ret = fapi_Image_commit();
		if (ret != 0)
			fprintf(stderr, "Commit failed\n");
	} else if(cmdopt.status) {
		ret = fapi_Get_lastupg_status(cStatus);
		if (ret) {
			fprintf(stderr, "Get last upgrade status failed\n");
			return ret;
		}
		ret = fapi_Get_lastupg_time(cTime);
		if (ret) {
			fprintf(stderr, "Get last upgrade time failed\n");
			return ret;
		}
		fprintf(stdout, "Last image upgrade was at time %s status: %s \n", cTime, cStatus);
	} else if(cmdopt.swbank) {
		ret = fapi_Switch_bank(actbnk);
		if (ret) {
			fprintf(stderr, "switch bank failed\n");
			return ret;
		}
		system("reboot");
		fprintf(stdout, "Switching is done successfully, rebooting the board to boot from new bank\n");
	} else if(cmdopt.auth) {
		ret = fapi_Image_Verify(image);
		if (ret) {
			fprintf(stderr, "Image Authentication failed\n");
			return ret;
		}
	} else if(cmdopt.ext4auth) {
		ret = fapi_Ext4_Image_Verify(image);
		if (ret) {
			fprintf(stderr, "Ext4 Image Authentication failed\n");
			return ret;
		}
	}

	if (!ret && cmdopt.reboot) {
		system("reboot");
		fprintf(stdout, "Rebooting....!\n");
	}
	return ret;
}
