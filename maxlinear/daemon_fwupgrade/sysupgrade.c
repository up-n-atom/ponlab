/********************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glob.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "include/sysupgrade.h"
#include "include/sysupgrade_ubus_api.h"
#include "include/process_handle.h"
#include "logdefs.h"
#include <ltq_api_include.h>

/* Sysupgrade run-time settings or options */
SysupgradeOptions xSysupgradeOpt;

#ifndef LOG_LEVEL
	uint16_t LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
	uint16_t LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
	uint16_t LOGTYPE = SYS_LOG_TYPE_FILE;
#else
	uint16_t LOGTYPE = LOG_TYPE;
#endif

/*=============================================================================
 * Function Name : sysupgrade_optionsInit
 * Description   : Apply sysupgrade settings / options
 *===========================================================================*/
static void sysupgrade_optionsInit(void)
{
	memset(&xSysupgradeOpt, 0, sizeof(SysupgradeOptions));
	if (xSysupgradeOpt.nLogValue >= 0) {
		LOGLEVEL = get_logLevel(xSysupgradeOpt.nLogValue);
		LOGTYPE = get_logType(xSysupgradeOpt.nLogValue);
	} else {
		xSysupgradeOpt.nLogValue = put_logLevel(xSysupgradeOpt.nLogValue, LOGLEVEL);
		xSysupgradeOpt.nLogValue = put_logType(xSysupgradeOpt.nLogValue, LOGTYPE);
	}
	set_libscapi_loglevel(LOGLEVEL, LOGTYPE);

	LOGF_LOG_DEBUG("Sysupgrade Options: loglevel %d, fork: %s, dumpmsg: %s\n",
			xSysupgradeOpt.nLogValue,
			xSysupgradeOpt.nFork ? "enabled" : "disabled",
			xSysupgradeOpt.nDumpMsg ? "enabled" : "disabled");
}

/*=============================================================================
* Function Name : sysupgrade_freeAll
* Description   : Function free all global library data structure
*===========================================================================*/
void exit_sysupgrade(uint16_t unStatus)
{
	exit(unStatus);
}

/*=============================================================================
 * Function Name : main / sysupgrade main
 * Description   : Register in ubusd ,wait in uloop.
 *===========================================================================*/
int main (int argc __attribute__((unused)), char **argv)
{
	openlog(argv[0], 0, LOG_DAEMON);

	sysupgrade_optionsInit();
	uloop_init();
	if (sysupgrade_ubusInit() != UGW_SUCCESS)
		exit_sysupgrade (UGW_FAILURE);

	/*register with ubusd */
	sysupgrade_ubusRegister();

	uloop_run();

	sysupgrade_ubusDone();
	uloop_done();

	exit_sysupgrade (UGW_SUCCESS);
	return 0;
}
