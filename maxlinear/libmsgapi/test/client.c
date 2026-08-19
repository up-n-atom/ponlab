/********************************************************************************
 
  Copyright (c) 2016
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
********************************************************************************/

/* ****************************************************************************** 
 *         File Name    : client.c	                                        *
 *         Description  : Test utility to demostarte MSG API usage and flow	*
 * ******************************************************************************/

#include <ugw_msg_api.h>
#include "test.h"

/*  =============================================================================
 *   Function Name 	: main 							*
 *   Description 	: function uses MSG API to send own msges to daemon  	*
 *   			  which registred with ubusd				*
 *  ============================================================================*/
int main(__attribute__ ((unused))int argc, __attribute__ ((unused))char **argv)
{
	Msg_t xMsg={0};
	int nRet = UGW_SUCCESS;
	int nFlag=1;

	MyA_t a[5]={0};
	int nCnt;

	while (1)
	{

	
		if(nFlag)	
			sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "Myserver");
		else
			sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "Myserver1");
		
		printf("MSG API>> Sending Structure Type Msg to [%s]\n", xMsg.sSerName);

		sprintf_s(xMsg.sSLId, MAX_SL_NAME, "sl_app");
		xMsg.nOperType=OPT_ADD|OPT_THREAD_ENV;
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
			printf("MSG API>> Return success for struct type msg %d\n",nRet); 
		else
			printf("MSG API>> Return failed for struct type msg %d\n",nRet); 


		
		if(nFlag)	
			sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "Myserver");
		else
			sprintf_s(xMsg.sSerName, MAX_SERVER_NAME, "Myserver1");

		printf("\nMSG API>> Sending String Type Msg to [%s]\n", xMsg.sSerName);

		sprintf_s(xMsg.sSLId, MAX_SL_NAME, "sl_app");
		xMsg.nOperType=OPT_ADD|OPT_THREAD_ENV;
		xMsg.nMsgType=2;  //note 

		//filling dummy data
		char *pc=calloc(1,100);
		if(pc == NULL)
			return 0;	

		sprintf_s(pc,100, "name:value123,name:value,name:value,name:value,name:value,........,name:value");

		xMsg.pMsg=pc;
		xMsg.nMsgSize=100;

		nRet = msg_send(&xMsg); 
		
		if(!nRet)
			printf("MSG API>> Return success for string type msg %d\n",nRet); 
		else
			printf("MSG API>> Return failed for string type msg %d\n", nRet); 

		//repeat alternative send msg
		if(nFlag)
		    nFlag=0;
		else
		    nFlag++;
	
		if(pc != NULL)
			free(pc);
	
		sleep(5);
		    
	}

	return 0;
}
