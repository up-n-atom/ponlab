/********************************************************************************
 
  Copyright (c) 2016
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
********************************************************************************/

/* ****************************************************************************** 
 *         File Name    : multiServ.c 	                                        *
 *         Description  : Test utility to demostarte MSG API usage and flow in	*
 *			  both direction along with CAL API
 * ******************************************************************************/

#include <ugw_msg_api.h>
#include <unistd.h>
#include "test.h"
#include <cal.h>

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

void* sendMsg(void *);
void* HostServer(__attribute__ ((unused))void *vagp);

/*  =============================================================================
 *   Function Name 	: sendMsg						*
 *   Description 	: call msg_send API in loop				*
 *  ============================================================================*/
void* sendMsg(__attribute__ ((unused))void *vagp)
{
	Msg_t xMsg={0};
	int nRet = UGW_SUCCESS;
	MyA_t a[5]={0};
	int nCnt;

	while(1)
	{
	
		printf("\nMSG API>> Sending Structure Type Msg From Thread to [SERVD]\n");
	
		sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "servd");
		sprintf_s(xMsg.sSLId, MAX_SL_NAME, "sl_app");
		xMsg.nOperType = OPT_ADD|OPT_THREAD_ENV;
		xMsg.nMsgType=1;  //note 

		//filling dummy data
		for(nCnt=0; nCnt<5; nCnt++)
		{
			a[nCnt].a=nCnt+1;
			sprintf_s(a[nCnt].b, MAX_VAL, "MsgAPI:%d", nCnt+2);
			a[nCnt].c=nCnt+3;
			a[nCnt].d=nCnt+3.3;
			a[nCnt].f.a=nCnt+4;
			sprintf_s(a[nCnt].f.b, MAX_VAL, "MsgAPI:%d", nCnt+5);
			a[nCnt].x=nCnt+4;
		}

		xMsg.nMsgSize=sizeof(a);
		xMsg.pMsg=&a;

		nRet = msg_send(&xMsg); 
		if(!nRet)
			printf("MSG API>> Return success for struct type msg\n"); 
		else
			printf("MSG API>> Return failed for struct type msg\n"); 


		printf("\n MSG API>> Sending String Type Msg to [SERVD] \n");

		sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "servd");
		sprintf_s(xMsg.sSLId, MAX_SL_NAME, "sl_app");
		xMsg.nOperType = OPT_ADD|OPT_THREAD_ENV;
		xMsg.nMsgType=2;  //note 

		//filling dummy data
		char *pc=calloc(1,100);
		if(pc == NULL)
			return 0;	

		sprintf_s(pc,100, "name:value,name:value,name:value,name:value,name:value,........,name:value");

		xMsg.pMsg=pc;
		xMsg.nMsgSize=100;

		nRet = msg_send(&xMsg); 
		
		if(!nRet)
			printf("MSG API>> Return success for string type msg\n"); 
		else
			printf("MSG API>> Return failed for string type msg\n"); 

	/* keep sending msg every 5 sec to Other server */
		sleep(5);
	}
}

/*  =============================================================================
 *   Function Name 	: cb_extractMsg						*
 *   Description 	: Call back function , it will be called when msg posted*
 *   			  by App SL / external daemon over ubus. structure type *
 *			  msg / string type / any other type.			*
 *  ============================================================================*/
static int32_t cb_extractMsg(Msg_t *pxMsg)
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
		printf("Printing Msg\n  %s\n\n", MsgstrType);
	}
	return RET_TEST_CODE;
}

/*  =============================================================================
 *   Function Name 	: HostServer						*
 *   Description 	: hosting server to receive msg				*
 *  ============================================================================*/
void* HostServer(__attribute__ ((unused))void *vagp)
{
	/* host server for incoming msges */
	cb_func myFuc;

	//define call back function assign call back function to be called on ubus msg arrival.	
	myFuc = cb_extractMsg;

	//creates ubus server with 'MyServer1' , waits for ubus msges in uloop
	printf("MSG API >> ubus server name [Myserver1]\n");
	msg_ubusConnect(myFuc, "Myserver1");
	
	return 0;
}


/*  =============================================================================
 *   Function Name 	: main 							*
 *   Description 	: function uses MSG API to create ubus socket, 		*
 *   			  and it wait in uloop for ubus msges.			*
 *  ============================================================================*/
int main(__attribute__ ((unused))int argc, __attribute__ ((unused))char **argv)
{

	int nRet=UGW_SUCCESS;
	MsgHeader pxMsg;
	
	
	// create thread to send out msges
	pthread_t  tid ,t1_id;
	nRet = pthread_create(&tid, NULL, &sendMsg, NULL);

	if (nRet != 0)
	    printf("\ncan't create thread :[%s]", strerror(nRet));

	nRet = pthread_create(&t1_id, NULL, &HostServer, NULL);

	if (nRet != 0)
	    printf("\ncan't create thread2 :[%s]", strerror(nRet));

	while(1)
	{
		HELP_CREATE_MSG(&pxMsg,MOPT_GET, SOPT_OBJVALUE , OWN_WEB,OPT_THREAD_ENV);   // note flag
		HELP_OBJECT_GET(pxMsg.pObjType,"Device.Time",pxMsg.unSubOper);
		nRet = cal_getValueFromDb(&pxMsg);

		if(nRet == UGW_SUCCESS)
		{
			printf("cal get success\n");
			HELP_PRINT_MSG(&pxMsg);	
		}
		else
		{
			printf("cal get failure\n");
			HELP_PRINT_MSG(&pxMsg);	
		}
	
		HELP_DELETE_MSG(&pxMsg);	
		sleep(10);
	}
			
	return 0;
}
