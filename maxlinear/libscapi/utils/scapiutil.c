/*****************************************************************************

  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : scapiutil.c                                        	       *
 *     Project    : UGW                                                        *
 *     Description: To provide utility support of the scapi API's	       * 
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include "libsafec/safe_str_lib.h"
#include "scapi_util.h"
#include <fcntl.h> 
#include <unistd.h> 


#define FIRST_BOOT_CHK VENDOR_PATH "/etc/.certchk"

int main( int argc, char **argv)
{

	int nRet=0;
	int nChildSt;

	if( argc <= 1 )
	{
		printf("usage : %s -h\n",argv[0]);
		return 0;
	}

	if (strcmp(argv[1],"-h")==0)
	{
		printf("supported commands in %s \n", argv[0]);
		printf("%s get_mac \n", argv[0]);
		printf("%s get_key \n", argv[0]);
		printf("%s get_nextMac \n", argv[0]);
		printf("%s get_resevMac \n", argv[0]);
		printf("%s remove_mac \n", argv[0]);
	}
	else if (strcmp(argv[1], "get_mac")==0)
	{
		char sMac[32]={0};
		nRet = scapi_getBaseMac(sMac); 
		if(nRet == EXIT_SUCCESS)
			printf("%s",sMac);
	}

	else if (strcmp(argv[1], "get_key")==0)
	{
		
		int fd ;
		fd = open(FIRST_BOOT_CHK, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
		if (!(fd < 0)) 
		{ 
			nRet = scapi_spawn("key=`head -200 /dev/urandom | cksum | cut -f1 -d \" \"` ; openssl req -new -x509 -subj \"/C=IN/ST=KAR/L=Benguluru/O=UGW SW/CN=CPE \" -sha256 -newkey rsa:2048 -passin pass:$key -keyout /tmp/key.pem -out /tmp/cert.pem -days 7300 -nodes -passout pass:$key 2>/dev/null; cat /tmp/key.pem /tmp/cert.pem > /etc/certs/lighttpd/lighttpd.pem;rm -rf /tmp/*.pem",SCAPI_BLOCK,&nChildSt);
			if (nRet !=  0)
			{
				printf("@@@@@@@@@@@@@@@ Failed to create certificate @@@@@@@@@@@@@@@@@\n");
			}
			nRet = scapi_spawn("key=`head -200 /dev/urandom | cksum | cut -f1 -d \" \"` ; openssl req -new -x509 -subj \"/C=IN/ST=KAR/L=Benguluru/O=UGW SW/CN=CPE \" -sha256 -newkey rsa:2048 -passin pass:$key -keyout /tmp/key.pem -out /tmp/cert.pem -days 7300 -nodes -passout pass:$key 2>/dev/null; openssl rsa -in /tmp/key.pem -out /tmp/rsa_key.pem;cat /tmp/rsa_key.pem /tmp/cert.pem > /etc/certs/vsftpd/vsftpd.pem ;rm -rf /tmp/*.pem",SCAPI_BLOCK,&nChildSt);
			if (nRet !=  0)
			{
				printf("@@@@@@@@@@@@@@@ Failed to create rsa certificate @@@@@@@@@@@@@@@@@\n");
			}
		}
		if(fd >= 0) close(fd);
	}
	else if (strcmp(argv[1], "get_nextMac")==0)
	{
		if(argc >= 3)
		{
			char sMac[36];
			if(strcmp(argv[3],"private")==0)
				nRet = scapi_getNextMacaddr(argv[2],false,sMac);
			else
				nRet = scapi_getNextMacaddr(argv[2],true,sMac);

			if(nRet == EXIT_SUCCESS)
			  printf("%s",sMac);
			
		}
		else
		{
			printf("usage: %s get_nextMac <interface name>  <public/private>\n",argv[0]);
		}
	}
	else if (strcmp(argv[1], "get_resevMac")==0)
	{
		ResvMAC *pxMac=NULL; 
		int nResCnt=0;

		nRet = scapi_getReserevdMac(&pxMac, &nResCnt);

		if( nRet == EXIT_SUCCESS)
		{
			int i=0;
			for(i=0; i<nResCnt; i++)
			{
				printf("mac[%d] = %s\n",i,pxMac[i].pcMac);
			}
			//note free done by caller.
			free(pxMac);
		}
	}
	else if(strcmp(argv[1], "remove_mac")==0)
        {
		nRet = scapi_removeNextMacaddr(argv[2]);
		if(nRet != EXIT_SUCCESS)
		{
			printf("failed to remove %d\n", nRet);
		}
	}
	return nRet;
}
