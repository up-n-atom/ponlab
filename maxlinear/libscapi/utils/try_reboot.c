/******************************************************************************** 

  Copyright © 2020 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG 
  Lilienthalstrasse 15, 85579 Neubiberg, Germany  

  For licensing information, see the file 'LICENSE' in the root folder of 
  this software module. 

********************************************************************************/
//-----------------------------------------------------------------------
// Description:  
//   This tool try a normal reboot followed by a specified delay and perform
//   a force reboot including force faulting the INIT process. Can be used if
//   normal reboot fails or init process doesn't respond.
//   
//   WARNING: Use this tool only for a workaround as this may hide the real
//   problems that interrupted normal reboot. It is safe to fix the process that
//   interrupted normal reboot.
//-----------------------------------------------------------------------
// Author:       alexander.abraham@intel.com
// Created:      14-Aug-2020
//-----------------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

#define DFL_REBOOT_WAIT_SEC 20

#define MIN_REBOOT_WAIT_SEC 1
#define MAX_REBOOT_WAIT_SEC 60

static void my_handle(int sig __attribute__((unused)))
{
	return;
}

int main(int argc, char *argv[2])
{
	unsigned int i, delay = 0;

	signal(SIGINT, my_handle);
	signal(SIGKILL, my_handle);
	signal(SIGHUP, my_handle);
	signal(SIGTERM, my_handle);

	if (argc > 1)
		if (argv[1][0] == '-' && ((argv[1][1] == '-' && argv[1][2] == 'h') || argv[1][1] == 'h')) {
			printf("Usage:- %s <wait time in seconds after normal reboot>\n", argv[0]);
			return 0;
		} else {
			delay = atoi(argv[1]);
		}
	else
		delay = DFL_REBOOT_WAIT_SEC;

	if (delay > MAX_REBOOT_WAIT_SEC) {
		printf("Setting to max delay %d\n", MAX_REBOOT_WAIT_SEC);
		delay = MAX_REBOOT_WAIT_SEC;
	} else if (delay < MIN_REBOOT_WAIT_SEC) {
		printf("Setting to min delay %d\n", MIN_REBOOT_WAIT_SEC);
		delay = MIN_REBOOT_WAIT_SEC;
	}

	sync(); // Flush all cache
	kill(1, SIGTERM); // Send reboot signal to init process to do safe shutdown.

	for (i = 0; i < delay; i++) {
		sleep(1);
	}

	printf("System not responding for reboot! Attempting a force reboot..\n\n");
	sleep(1);
	reboot(LINUX_REBOOT_CMD_RESTART); // Force reboot signal to Kernel.

	for (i = 0; i < 5; i++) {
		sleep(1);
	}

	kill(1, SIGSEGV); // Send fault signal to INIT process.

	for (i = 0; i < 3; i++) {
		sleep(1);
	}

	printf("System halted! Please do a hard reboot.\n");

	return 0;
}
