/*****************************************************************************

  Copyright © 2020-2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : scapi_meminfo.c                                            *
 *     Project    : UGW                                                        *
 *     Description: cal - /proc/meminfo get APIs                               *
 *                                                                             *
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ltq_api_include.h>

#define PROC_MEMINFO "/proc/meminfo"
#define MAX_BUF_SIZE 256


int scapi_getMeminfo(MemInfo *xInfo)
{
    FILE *fp;
    char sBuf[MAX_BUF_SIZE] = { 0 };
    char sVal[MAX_BUF_SIZE] = { 0 };
    int nRet = -EXIT_FAILURE;
    uint32_t uVal;
    int indicator;

    fp = scapi_getFilePtr(PROC_MEMINFO, "r");
    if(fp == NULL)
    {
	nRet = -errno;
	printf("ERROR = %d -> %s\n", nRet, strerror(-nRet));
	return nRet;
    }

    while ((fscanf(fp, "%255s %u %255s\n", sVal, &uVal, sBuf)) == 3)
    {
	if ((strcmp_s(sVal, sizeof(sVal), "MemTotal:", &indicator) == EOK) && (indicator == 0))
	{
	    xInfo->uTotal = uVal ;
	}
	else if ((strcmp_s(sVal, sizeof(sVal), "MemFree:", &indicator) == EOK) && (indicator == 0))
	{
	    xInfo->uFree = uVal ;
	}
	else if ((strcmp_s(sVal, sizeof(sVal), "Buffers:", &indicator) == EOK) && (indicator == 0))
	{
	    xInfo->uBuffers = uVal ;
	}
	else if ((strcmp_s(sVal, sizeof(sVal), "Cached:", &indicator) == EOK) && (indicator == 0))
	{
	    xInfo->uCached = uVal ;
	}
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
