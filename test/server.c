/********************************************************************************
 
  Copyright (c) 2016
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/* ****************************************************************************** 
 *         File Name    : server.c	                                        *
 *         Description  : Test utility to demostarte MSG API usage and flow	*
 * ******************************************************************************/

#include <ugw_msg_api.h>
#include <unistd.h>
#include "test.h"

/*  =============================================================================
 *   Function Name 	: cb_extractMsg						*
 *   Description 	: Call back function , it will be called when msg posted*
 *   			  by App SL / external daemon over ubus. structure type *
 *			  msg / string type / any other type.			*
 *  ============================================================================*/
static int32_t cb_extractMsgServer(Msg_t *pxMsg)
{
	printf("\nMSG API>> Msg recived from the client ....\n");
	
	// Msg API's struct info
	printf("Msg owner:%s\n", pxMsg->sSerName);
	printf("Msg identifier:%s\n", pxMsg->sSLId);
	printf("Msg Type:%d\n", pxMsg->nMsgType);
	printf("Msg Size:%d\n", pxMsg->nMsgSize);
	
	//based on the msg type we will type cast to its original data type.
	// in test code we put '1' msgType carries sturcture type as data.

	if (pxMsg->nMsgType == 1)
	{
		MyA_t *pxMsgStructType;
		uint32_t nCnt;

		//typecasting structure 
		pxMsgStructType=((MyA_t *)pxMsg->pMsg);

		printf("Received Msg type is structure\n");
		printf("Printing Msg\n");
		// (Msg size/struct size ) to structure array count	
		for (nCnt=0;nCnt<(pxMsg->nMsgSize/sizeof(MyA_t));nCnt++)
		{
			printf("a(int)=%d, b(char)=%s, c(int)=%d, d(float)=%f, nested struct=%d, nested struct=%s, x(int)=%d\n",
						pxMsgStructType[nCnt].a, pxMsgStructType[nCnt].b, pxMsgStructType[nCnt].c, pxMsgStructType[nCnt].d, 
						pxMsgStructType[nCnt].f.a, pxMsgStructType[nCnt].f.b, pxMsgStructType[nCnt].x);
		}
	}
	// '2' carries string type as data 
	else if(pxMsg->nMsgType == 2)
	{
		char *MsgstrType;

		//typecasting char * 
		MsgstrType=((char *)pxMsg->pMsg);
		printf("Received Msg type is string\n");
		printf("Printing Msg\n  %s\n ", MsgstrType);
	}
	return RET_TEST_CODE;
}

/*  =============================================================================
 *   Function Name 	: main 							*
 *   Description 	: function uses MSG API to create ubus socket, 		*
 *   			  and it wait in uloop for ubus msges.			*
 *  ============================================================================*/
int main(__attribute__ ((unused))int argc, __attribute__ ((unused))char **argv)
{
	cb_func myFuc;

	//define call back function assign call back function to be called on ubus msg arrival.	
	myFuc = cb_extractMsgServer;

	//creates ubus server with 'MyServer' , waits for ubus msges in uloop
	printf("\nMsgAPI>> ubus server name [Myserver]\n");
	msg_ubusConnect(myFuc, "Myserver");
	
	return 0;
}
