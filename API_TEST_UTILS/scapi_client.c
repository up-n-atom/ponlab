/*****************************************************************************

  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    :  scapi_client.c                                    *
 *         Description  :  This file contains all the data declarations that are
                           shared across the modules                      *  
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <scapi_ipc.h>

#include "ulogging.h"
#ifndef LOG_LEVEL
uint16_t   LOGLEVEL = SYS_LOG_INFO + 1;
#else
uint16_t   LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
uint16_t   LOGTYPE = SYS_LOG_TYPE_CONSOLE;
#else
uint16_t   LOGTYPE = LOG_TYPE;
#endif

int main()
{
	x_ipc_msg_t xmsg;
	if (sprintf_s(xmsg.xhdr.aucfrom, MAX_MODID_LEN, "client") <= 0)
		return 0;
	if (sprintf_s(xmsg.xhdr.aucto, MAX_MODID_LEN, "server") <= 0) 
		return 0;
	xmsg.xhdr.unmsgsize=strnlen_s("request",32);
	if (sprintf_s(xmsg.acmsg,IPC_MAX_MSG_SIZE, "request") <= 0)
		return 0;
	printf("This is client\n");
	ipc_send_request(&xmsg);
	printf("From[%s]To[%s]Len[%d]Msg[%s]\n",xmsg.xhdr.aucfrom,xmsg.xhdr.aucto,xmsg.xhdr.unmsgsize,xmsg.acmsg);
}
