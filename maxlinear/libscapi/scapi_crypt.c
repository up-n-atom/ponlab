/********************************************************************************

  Copyright © 2020-2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/*  ****************************************************************************** 
*         File Name    : scapi_crypt.c	                                         *
*         Description  : This scapi provides the encryption and decryption 	 *
*			 functions.						 * 
*  *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ulogging.h>
#include <ltq_api_include.h>
#include <time.h>

#define MAX_BUF_SIZE 2056
#define MAX_CRYPTO_SIZE 256
#define SNPRINTF_CRYPTO(pDest, nBufSize, psFormat, ...) { \
        if((sprintf_s(pDest, nBufSize, psFormat, ##__VA_ARGS__)) <=0 ) { \
                LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); \
                nRet = EXIT_FAILURE; \
		goto end; \
	} \
}

static int get_random(void)
{
	srand(time(0));
	return rand();
}

int scapi_encryption(IN char *pcInFile, IN char *pcPassWd, OUT char *pcOutFile)
{
	int nRet = -EXIT_FAILURE;
	int nChildRet = -1;
	char sBuf[MAX_BUF_SIZE] = { 0 };
	char *pcSavePtr=NULL;
	size_t len = MAX_CRYPTO_SIZE;
	const char *sErrString = "parameter crypto_key is not existed";

	FILE    *fd = NULL, *fd1 = NULL;
	char    sCryptoKey[MAX_CRYPTO_SIZE] = {0};
	char    sTemp[MAX_CRYPTO_SIZE] = {0};

	if (pcPassWd != NULL) {
		SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "openssl aes-256-cbc -base64 -in %s -out %s -pass pass:%s", pcInFile, pcOutFile, pcPassWd);
	} else {
		fd = popen("uboot_env --get --name crypto_key", "r");
		if (fd != NULL) {
			if(fread(sCryptoKey, 1, sizeof(sCryptoKey), fd) > 0) {
				if(!strncmp (sCryptoKey, sErrString, sizeof("parameter crypto_key is not existed")-1)) {
					SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "echo -n \"%d\" | sha256sum", get_random());
					fd1 = popen(sBuf, "r");
					if (fd1 != NULL) {
						if(fread(sTemp, 1, sizeof(sTemp), fd1) > 0) {
							SNPRINTF_CRYPTO(sCryptoKey, sizeof(sCryptoKey), "%s", strtok_s(sTemp,&len," ",&pcSavePtr));
						}
						pclose(fd1);
						fd1=NULL;
					} else {
						SNPRINTF_CRYPTO(sCryptoKey, sizeof(sCryptoKey), "%d", get_random());
					}

					SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "uboot_env --add --name crypto_key --value %s", sCryptoKey);
					nRet = scapi_spawn(sBuf, SCAPI_BLOCK, &nChildRet);
				}
			}
			pclose(fd);
			fd=NULL;
		} else {
			LOGF_LOG_ERROR("Failed to open the pipe");
		}
	}
	SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "openssl aes-256-cbc -base64 -in %s -out %s -pass pass:%s", pcInFile, pcOutFile, sCryptoKey);
	nRet = scapi_spawn(sBuf, SCAPI_BLOCK, &nChildRet);
	if (nRet == UGW_SUCCESS) {
		if (nChildRet != UGW_SUCCESS) {
			nRet = nChildRet;
		}
	}
end:
	if (fd != NULL)
		pclose(fd);
	if (fd1 != NULL)
		pclose(fd1);
	LOGF_LOG_INFO("Encryption Completed with retrun code : %d \n", nRet);
	return nRet;
}

int scapi_decryption(IN char *pcInFile, IN char *pcPassWd, OUT char *pcOutFile)
{
	int nRet = -EXIT_FAILURE;
	int nChildRet = -1;
	char sBuf[MAX_BUF_SIZE] = { 0 };
	FILE    *fd = NULL;
	char    sCryptoKey[256] = {0};

	if (pcPassWd != NULL) {
		SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "openssl aes-256-cbc -base64 -d -in %s -out %s -pass pass:%s", pcInFile, pcOutFile, pcPassWd);
	} else {
		fd = popen("uboot_env --get --name crypto_key", "r");
		if (fd != NULL) {
			if(fread(sCryptoKey, 1, sizeof(sCryptoKey), fd) > 0) {
				SNPRINTF_CRYPTO(sBuf, MAX_BUF_SIZE, "openssl aes-256-cbc -base64 -d -in %s -out %s -pass pass:%s", pcInFile, pcOutFile, sCryptoKey);
			}
			pclose(fd);
			fd=NULL;
		} else {
			LOGF_LOG_ERROR("Failed to open the pipe");
		}
	}
	nRet = scapi_spawn(sBuf, SCAPI_BLOCK, &nChildRet);
	if (nRet == UGW_SUCCESS) {
		if (nChildRet != UGW_SUCCESS) {
			nRet = nChildRet;
			/* this error will hit if the upgrade is perfromed on older release versions
			   where UGW_SW-46505 change patch not applied by the customer */
			LOGF_LOG_ERROR("###### Not compatible with old keys ##### \n");
		}
	}
end:
	if(fd != NULL)
		pclose(fd);
	LOGF_LOG_INFO("Decryption Completed with retrun code : %d \n", nRet);
	return nRet;
}
