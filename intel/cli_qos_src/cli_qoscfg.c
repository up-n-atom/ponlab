/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include <stdio.h>
#include <unistd.h> // getopt
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "commondefs.h"
#include "qosal_queue_api.h"
#include "fapi_high.h"
#include "cli_qoscfg.h"
#include "qosal_ugw_debug.h"
#include "qosal_utils.h"

#include "safe_str_lib.h"
#include "safe_mem_lib.h"

#define QCLI_MAX_STR 1024

static struct option long_options[] = {
  {"dump", no_argument, 0, 'd'},
  {"help", no_argument, 0, 0},
  {0, 0, 0, 0}
};

void
qoscli_print_usage(void);
int32_t
qoscli_invoke_fapi(struct x_qoscfg_t *pxcfg);
void
qoscli_print_usage(void)
{
	printf("Usage: qoscfg \n");
	printf("-I <1/0/2> [enable status ==> enable/disable/shut QoS]\n");
	printf("-l <1/0> [log operation enable/disable]\n");
	printf("-t <1/2> [log to file/log to console]\n");
	printf("-f <file_name> [log file in case -t 1 is specified]\n");
	printf("-w <1/0> [WMM QoS enable/disable]\n");
	printf("-L [Get current enable status of QoS]\n");
	printf("-v [Get current version of QoS FAPI]\n");
}

int32_t
qoscli_invoke_fapi(struct x_qoscfg_t *pxcfg)
{
	int32_t iret = LTQ_FAIL;
	if(pxcfg->uiflags == 0) {
		qoscli_print_usage();
		return iret;
	}

	if(pxcfg->uiflags&QOS_CFG_INIT){
		iret = fapi_qos_init(&pxcfg->qoscfg, pxcfg->uiflags);
	}
	else if(pxcfg->uiflags&QOS_CFG_SHUT){
		iret = fapi_qos_shut(pxcfg->uiflags);
	}
	else if(pxcfg->uiflags&QOS_STATUS_SET){
		iret = fapi_qos_cfg_set(&pxcfg->qoscfg, pxcfg->uiflags);
	}
	else if(pxcfg->uiflags&QOS_STATUS_GET || pxcfg->uiflags&QOS_OP_CFG_DUMP){
		iret = fapi_qos_cfg_get(&pxcfg->qoscfg, pxcfg->uiflags);
	}
	return iret;
}

int32_t
qoscfg_main(int32_t argc, char**argv)
{
	int32_t c, option_index = 0, ret;
	struct x_qoscfg_t xqoscfg, xqoscfgtmp;
	uint32_t status_flag = 0, version_flag = 0;
	bool bValidReq;
  
	memset_s(&xqoscfg, sizeof(xqoscfg), 0);
	memset_s(&xqoscfgtmp, sizeof(xqoscfgtmp), 0);
	xqoscfg.qoscfg.ena = -1;
	xqoscfg.qoscfg.wmm_ena = -1;
	bValidReq = true;

	while(1){
		c = getopt_long(argc, argv, "I:e:l:t:f:w:Ldv",
			long_options, &option_index);
		if(c==-1) break;
		switch(c){
			case 'I':
		        	xqoscfg.qoscfg.ena = atoi(optarg);
			        xqoscfg.qoscfg.wmm_ena = atoi(optarg);
				if (xqoscfg.qoscfg.ena == 2)
					xqoscfg.uiflags |= QOS_CFG_SHUT;
				else
					xqoscfg.uiflags |= QOS_CFG_INIT;
				bValidReq = false;
			        break;
			case 'l':
				if((atoi(optarg) < 1) || (atoi(optarg) > 7))
				{
					printf("log level value must be between 1 to 7 \n");
					return LTQ_FAIL;
				}
				xqoscfg.qoscfg.log_level = atoi(optarg);
				LOGLEVEL = xqoscfg.qoscfg.log_level;
				xqoscfg.uiflags |= QOS_STATUS_SET;
				break;
			case 't':
				xqoscfg.qoscfg.log_type = atoi(optarg);
				LOGTYPE = xqoscfg.qoscfg.log_type;
				xqoscfg.uiflags |= QOS_STATUS_SET;
				break;
			case 'f':
				if(xqoscfg.qoscfg.log_level != 0 && xqoscfg.qoscfg.log_type == QOS_LOG_FILE) {
					if(strncpy_s(xqoscfg.qoscfg.log_file, sizeof(xqoscfg.qoscfg.log_file), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
					{
						printf("Error copying Log file name !!\n");
						return LTQ_FAIL;
					}
					xqoscfg.uiflags |= QOS_STATUS_SET;
				}
				break;
			case 'e':
				xqoscfg.qoscfg.ena = atoi(optarg);
				xqoscfg.uiflags |= QOS_STATUS_SET;
				break;
			case 'w':
				xqoscfg.qoscfg.wmm_ena = atoi(optarg);
				xqoscfg.uiflags |= QOS_STATUS_SET;
				break;
			case 'L':
				xqoscfg.uiflags |= QOS_STATUS_GET;
				status_flag = 1;
				break;
			case 'd':
				xqoscfg.uiflags |= QOS_OP_CFG_DUMP;
				break;
			case 'v':
				xqoscfg.uiflags	|= QOS_STATUS_GET; 
				version_flag = 1;
				break;
			default:
				qoscli_print_usage();
				return LTQ_FAIL;
		}
	}

	if(bValidReq == true) {
		ret = fapi_qos_cfg_get(&xqoscfgtmp.qoscfg, 0);
		if(ret != LTQ_SUCCESS || xqoscfgtmp.qoscfg.ena != 1)
		{
			printf("Please initialize QoS first using I option.\n");
			return LTQ_FAIL;
		}
	}

	ret = qoscli_invoke_fapi(&xqoscfg);
	if(ret == LTQ_FAIL)
	{
		return LTQ_FAIL;
	}

	if(status_flag)
	{
		printf("QoS status: %s\n", (xqoscfg.qoscfg.ena)?"Enable":"Disable");
		printf("WMM status: %s\n", (xqoscfg.qoscfg.wmm_ena)?"Enable":"Disable");
		printf("Log Level : %d \n", xqoscfg.qoscfg.log_level);
		printf("Log Type  : %d \n",xqoscfg.qoscfg.log_type);
	}
	if(version_flag)	
		printf("Version of QOS FAPI: %s \n", xqoscfg.qoscfg.version);

	//return qoscli_invoke_fapi(&xqoscfg);
	return ret;
}
