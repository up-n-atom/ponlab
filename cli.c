/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define _GNU_SOURCE         
/*standard headers*/
#include <stdio.h>
#include <string.h>

#include "commondefs.h"
#include "qosal_debug.h"
#include "cli.h"
#include "cli_qoscfg.h"
#include "cli_classcfg.h"
#include "cli_qcfg.h"
#include "cli_ifcfg.h"
#include "qosal_queue_api.h"

#include "safe_str_lib.h"
#include "safe_mem_lib.h"

extern uint16_t LOGLEVEL, LOGTYPE;

char* cli_basename(char* param);

static const struct name2func axname2func[] = {
  {"classcfg",classcfg_main},
  {"qcfg",qcfg_main},
  {"qoscfg",qoscfg_main},
  {"ifcfg",ifcfg_main},
  {NULL,NULL},
};

static pfn_func_t func_get(const char *pcname)
{
	const struct name2func *pxconv=axname2func; 
  for (; pxconv->pcname != NULL; ++pxconv)
    if (strcmp(pxconv->pcname, pcname) == 0)
      return pxconv->pfnfunc;
  return NULL;
} 

char* cli_basename(char* param)
{
	char *ptr = strrchr(param, '/');
	if (ptr)
		return (ptr + 1);

	return param;
}

int
main(int32_t argc, char** argv)
{
	pfn_func_t pfnfunc=NULL;
 	struct x_qoscfg_t xqoscfg;

	memset_s(&xqoscfg, sizeof(xqoscfg), 0);
 	log_type = QOS_LOG_FILE;
	snprintf(log_file, MAX_NAME_LEN, "%s", LOG_FILE_LOC);

	char *pcname = cli_basename(*argv);
  if(pcname == NULL){
		printf("Name is NULL, please check\n");
		return LTQ_FAIL;
	}
  pfnfunc=func_get(pcname);
	if(pfnfunc==NULL){
		printf("%s","Invalid function name");
		return LTQ_FAIL;
	}

	/* get general cfg from fapi. */
	xqoscfg.uiflags |= QOS_STATUS_GET;
	if(fapi_qos_cfg_get(&xqoscfg.qoscfg, xqoscfg.uiflags) == LTQ_SUCCESS) {
		LOGLEVEL = xqoscfg.qoscfg.log_level;
		LOGTYPE = xqoscfg.qoscfg.log_type;
	}

	pfnfunc(argc,argv);
	return LTQ_SUCCESS;
}
