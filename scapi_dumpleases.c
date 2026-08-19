/******************************************************************************

  Copyright © 2020-2023 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/* 
 ** =============================================================================
 **   FILE NAME        : ltq_dumplease.c
 **   AUTHOR           : Lantiq DevM Team
 **   DESCRIPTION      : This file contains the C utility for dumplease system command
 **                      dumpleases" can list all DHCP Client information and all line content "seconds" text
 **
 **   Any use of this software is subject to the conclusion of a respective
 **   License agreement. Without such a License agreement no rights to the
 **   software are granted
 **   HISTORY          : 
 **   $Date            $Author                         $Comment
 **   Parameter        : 1. Dumplease file name (utility will consider default file, if it is NULL) [Input Parameter]
 2. Address of pointer to dump_lease_info structure [Output Parameter]
 3. &count [output parameter]
 **
 **   Returns          : Returns 0 on success and error number on failure.
 **
 ** ============================================================================
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <errno.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ulogging.h>
#include "ltq_api_include.h"

#define ENABLE_DEBUG 1
#define SNPRINTF_DUMP(pDest, nBufSize, psFormat, ...) { \
        if((sprintf_s(pDest, nBufSize, psFormat, ##__VA_ARGS__)) <=0 ) { \
                LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); \
                nRet = -EXIT_FAILURE; \
		goto end; \
        } \
}
//typedef uint32_t leasetime_t;

int scapi_dumplease(char *file, dump_lease_info **lease_info, int *no_entry)
{
	int nFd = 0, nRet = EXIT_SUCCESS;
	int i = 0,conn_count = 0;
	int64_t written_at = 0, curr, expires_abs = 0;
	struct dyn_lease lease;
	struct in_addr addr ;
	char *unknown_string = "unknown";
	dump_lease_info *temp_info = NULL, *temp_leaseinfo = NULL;

	*lease_info = NULL;

	if(strnlen_s(file,1024)<1)
		file=LEASES_FILE;

	nFd = open(file, O_RDONLY); 
	if(nFd < 0)
	{
		nRet = -errno;
		LOGF_LOG_ERROR("ERROR = %d --> %s\n", nRet, strerror(-nRet));
		return nRet;
	}

	if (read(nFd, &written_at, sizeof(written_at)) == -1) {
		nRet = -errno;
		LOGF_LOG_ERROR("ERROR = %d --> %s\n", nRet, strerror(-nRet));
		close(nFd);
		return nRet;
	}
	curr = time(NULL);
	if (curr < written_at)
		written_at = curr; /* lease file from future! :) */

	temp_leaseinfo = NULL;

	while (read(nFd, &lease, sizeof(lease)) == sizeof(lease))
	{

		temp_info= realloc(temp_leaseinfo,(sizeof(dump_lease_info)*(conn_count+1)));

		if(temp_info == NULL)
		{
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d --> %s\n", nRet, strerror(-nRet));
			close(nFd);
			return nRet;
		}
		temp_leaseinfo = temp_info; 

		for (i = 0; i < 6; i++) 
		{
			(temp_leaseinfo+conn_count)->lease_mac[i]=lease.lease_mac[i];
		}

		addr.s_addr = lease.lease_nip;

		/* actually, 15+1 and 19+1, +1 is a space between columns */
		/* lease.hostname is char[20] and is always NUL terminated */
		if ( *lease.hostname)
		{
			SNPRINTF_DUMP((temp_leaseinfo+conn_count)->ipaddr, 16, "%s", inet_ntoa(addr));
			SNPRINTF_DUMP((temp_leaseinfo+conn_count)->hostname, 20, "%s", lease.hostname);
		}
		else
		{
			SNPRINTF_DUMP((temp_leaseinfo+conn_count)->ipaddr, 16, "%s", inet_ntoa(addr));
			SNPRINTF_DUMP((temp_leaseinfo+conn_count)->hostname, 20, "%s", unknown_string);
		}                        

		expires_abs = ntohl(lease.expires) + written_at;
		if (expires_abs <= curr) 
		{
			(temp_leaseinfo+conn_count)->expires_seconds= -1;
			continue;
		}
		unsigned expires = expires_abs - curr;

		(temp_leaseinfo+conn_count)->expires_seconds= expires;
		conn_count++;
	}
end:
	close(nFd); 
	*no_entry=conn_count;
	*lease_info = temp_leaseinfo;
	return nRet;
}

