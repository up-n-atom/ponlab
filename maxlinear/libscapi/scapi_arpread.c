/********************************************************************************
  Copyright © 2020-2022 MaxLinear, Inc. 
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ********************************************************************************/

/*  ****************************************************************************** 
 *         File Name    : scapi_arpread.c                                         *
 *         Description  : This scapi is responsible for discovering the arp table *
 *                        entries and getting the Macs of host connected to interface*
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ulogging.h>
#include <ltq_api_include.h>

int scapi_getArpTable(arpTable_t *arpTable)
{
		char header[ARP_BUFFER_LEN];
		FILE *arpCache = scapi_getFilePtr(ARP_CACHE, "r");
		int i = 0;
		if (!arpCache)
		{
				LOGF_LOG_ERROR("Arp Cache: Failed to open file \"" ARP_CACHE "\"");
				return -1;
		}

		/* Ignore the first line, which contains the header */
		if (!fgets(header, sizeof(header), arpCache))
		{
				fclose(arpCache);
				return -1;
		}

		while (3 == fscanf(arpCache, ARP_LINE_FORMAT, arpTable->arpEntry[i].ipAddr,\
								arpTable->arpEntry[i].hwAddr, arpTable->arpEntry[i].device))
		{
				i++;
				arpTable->numOfEntires++;
		}
		fclose(arpCache);
		return 0;
}

int scapi_parseBrMacTable(macTable_t *macTable, const char sIntFace[IFNAMSIZ])
{
	char entry_buf[ARP_BUFFER_LEN] = {0};
	FILE *fp = scapi_getFilePtr(BR_MAC_CACHE, "r");
	int flag = 0, nRet = UGW_SUCCESS;
	char *psIfName = NULL, *pos = NULL, *tmp = NULL;
	size_t len = ARP_BUFFER_LEN;

	if (!macTable) {
		LOGF_LOG_ERROR("Mac Cache: MAC table pointer is NULL");
		goto end;
	}
	if (!fp) {
		LOGF_LOG_ERROR("Mac Cache: Failed to open file %s", BR_MAC_CACHE);
		return UGW_FAILURE;
	}

	memset(macTable, 0, sizeof(macTable_t));
	while (fgets(entry_buf, sizeof(entry_buf), fp)) {
		flag = 0;
		/* match with interface prefix name */
		if (strstr_s(entry_buf, len, sIntFace, IFNAMSIZ, &psIfName) == EOK) {
			flag = 1;
		}
		if (!flag)
			continue;

		/* Fill up the MAC address */
		pos = strtok_s(entry_buf, &len, " ", &tmp);
		if (pos) {
			if (sprintf_s(macTable->macEntry[macTable->numOfEntires].hwAddr,
			    sizeof(macTable->macEntry[macTable->numOfEntires].hwAddr), "%s", pos) <= 0) {
				LOGF_LOG_ERROR("Error : copying hwaddr failed %s", strerror(errno));
				nRet = UGW_FAILURE;
				goto end;
			}
		}
		/* Fill up the device name */
		tmp = NULL;
		len = ARP_BUFFER_LEN - (psIfName - entry_buf);
		pos = strtok_s(psIfName, &len, " ", &tmp);
		if (pos) {
			if (sprintf_s(macTable->macEntry[macTable->numOfEntires].device,
			    sizeof(macTable->macEntry[macTable->numOfEntires].device), "%s", pos) <= 0) {
				LOGF_LOG_ERROR("Error : copying device name failed %s", strerror(errno));
				nRet = UGW_FAILURE;
				goto end;
			}
		}
		/* Increase the number of MAC entries */
		macTable->numOfEntires++;
	}

end:
	/* close the fp */
	if(fp)
		fclose(fp);
	return nRet;
}

int scapi_getMacTable(macTable_t *macTable, char *pIntFace)
{
	int nChildExitStatus = UGW_FAILURE, nRet = UGW_SUCCESS;

	nRet = scapi_spawn(BR_MAC_CACHE_CMD, SCAPI_BLOCK, &nChildExitStatus);
	if (nRet != UGW_SUCCESS ) {
		nRet = UGW_FAILURE;
		LOGF_LOG_ERROR( "Spwaning Error :[%s]\n", BR_MAC_CACHE_CMD);
		goto end;
	} else if(nChildExitStatus == UGW_SUCCESS) {
		LOGF_LOG_INFO( "Spawning Success :[%s]\n", BR_MAC_CACHE_CMD);
	} else {
		LOGF_LOG_INFO( "Spawning [%s]\n", BR_MAC_CACHE_CMD);
	}

	nRet = scapi_parseBrMacTable(macTable, (const char *)pIntFace);
	if (nRet != UGW_SUCCESS)
		return UGW_FAILURE;

end:
	return nRet;
}
