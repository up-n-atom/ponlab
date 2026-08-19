/*****************************************************************************

  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : scapi_remove_nextmac.c                                        *
 *     Project    : UGW                                                        *
 *     Description: Removes the nextmac from config file
 *                                                                             *
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if_arp.h>
#include <ulogging.h>
#include <ltq_api_include.h>

#include <libsafec/safe_str_lib.h>

#define NEXT_MAC_CONF VENDOR_PATH      "/servd/etc/nextmac.conf"
#define TEMP_NEXT_MAC_CONF VENDOR_PATH "/servd/etc/temp_nextmac.conf"

#define MAX_LEN_PARAM_VALUE            64
/* 
 ** =============================================================================
 **   Function Name    :scapi_removeNextMacaddr
 **
 **   Description      :Removes next mac address entry from config file
 **
 **   Parameters       :pcIfname(IN) -> interface name
 **                      pcMac(IN) -> mac address to be removed
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 **
 ** ============================================================================
 */

int scapi_removeNextMacaddr(char* pcIfname)
{
	FILE* inFile = scapi_getFilePtr(NEXT_MAC_CONF, "r");
	FILE* outFile = scapi_getFilePtr(TEMP_NEXT_MAC_CONF, "a+");
	char line [MAX_LEN_PARAM_VALUE]; // maybe you have to user better value here
	int lineCount = 0;
	int nRet = -EXIT_FAILURE;

	if( inFile == NULL || outFile == NULL)
	{
		nRet = ERR_INPUT_VALIDATION_FAILED;
		LOGF_LOG_ERROR("ERROR = %d --> %s\n", nRet, strerror(-nRet));
		goto returnHandler;
	}

	while( fgets(line, sizeof(line), inFile) != NULL )
	{
		char* pcFind = strstr(line, pcIfname);
		/* Add only the lines that doesn't containt interface
		 * name to new file */
		if( pcFind == NULL){
			fprintf(outFile, "%s", line);
		}
		lineCount++;
	}
	nRet = EXIT_SUCCESS;

returnHandler:
	if(inFile != NULL)
		fclose(inFile);
	if(outFile != NULL)
		fclose(outFile);
	if(rename(TEMP_NEXT_MAC_CONF, NEXT_MAC_CONF) != 0){
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		fprintf(stderr, "%s:%d renaming file is failed after remove of mac\n", __func__,__LINE__);
	}

	return nRet;	 
}


/*
 ** =============================================================================
 **   Function Name    :scapi_swapNextMacaddr
 **
 **   Description      :Swaps next mac address entry from config file for two WAN interfaces
 **
 **   Parameters       :pcIfname_newDefWAN(IN) -> interface 1 name
 **                     pcIfname_oldDefWAN(IN) -> interface 2 name
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values
 **
 ** ============================================================================
 */

int scapi_swapNextMacaddr(char* pcIfname_newDefWAN, char* pcIfname_oldDefWAN)
{
	FILE* inFile = scapi_getFilePtr(NEXT_MAC_CONF, "r");
	FILE* outFile = scapi_getFilePtr(TEMP_NEXT_MAC_CONF, "w");
	char line [MAX_LEN_PARAM_VALUE];
	char MAC_newDefWAN [MAX_LEN_PARAM_VALUE];
	char MAC_oldDefWAN [MAX_LEN_PARAM_VALUE];
	char ChangedMAC_newDefWAN [MAX_LEN_PARAM_VALUE];
	char ChangedMAC_oldDefWAN [MAX_LEN_PARAM_VALUE];
	int lineCount = 0;
	int nRet = -EXIT_FAILURE;
	size_t len=0;

	if( inFile == NULL || outFile == NULL)
	{
		nRet = ERR_INPUT_VALIDATION_FAILED;
		LOGF_LOG_ERROR("ERROR = %d --> %s\n", nRet, strerror(-nRet));
		goto returnHandler;
	}

	while( fgets(line, sizeof(line), inFile) != NULL )
	{
		char* pcFind_newDefWAN = NULL;
		char* pcFind_oldDefWAN = NULL;
		char* pcFind_EthMacVlanIf = strstr(line, "eth1_wan");
		char* pcFind_PtmMacVlanIf = strstr(line, "ptm0_wan");

		if(pcFind_EthMacVlanIf == NULL && pcFind_PtmMacVlanIf == NULL) {
			pcFind_newDefWAN = strstr(line, pcIfname_newDefWAN);
			pcFind_oldDefWAN = strstr(line, pcIfname_oldDefWAN);
		}

		// Add only the lines that doesn't contain interface name to new file
		if(pcFind_newDefWAN == NULL && pcFind_oldDefWAN == NULL){
			fprintf(outFile, "%s", line);
		}
		// Get MAC address of both WAN interfaces that needs to be swapped
		else if( pcFind_newDefWAN != NULL) {
			if((len=strnlen_s(line, MAX_LEN_PARAM_VALUE)) <=0){
				nRet = UGW_FAILURE;
				LOGF_LOG_ERROR("Length of line containing %s is invalid.......\n", pcFind_newDefWAN);
				goto returnHandler;
			}
			char *token;
			char *next_token = NULL;

			token = strtok_s(line, &len, "> ", &next_token);
			token = strtok_s(NULL, &len, "\n", &next_token);
			sprintf_s(MAC_newDefWAN, MAX_LEN_PARAM_VALUE, "%s", token);
		}
		else if( pcFind_oldDefWAN != NULL) {
			if((len=strnlen_s(line, MAX_LEN_PARAM_VALUE)) <=0){
				nRet = UGW_FAILURE;
				LOGF_LOG_ERROR("Length of line containing %s is invalid.......\n", pcFind_oldDefWAN);
				goto returnHandler;
			}
			char *token;
			char *next_token = NULL;

			token = strtok_s(line, &len, "> ", &next_token);
			token = strtok_s(NULL, &len, "\n", &next_token);
			sprintf_s(MAC_oldDefWAN, MAX_LEN_PARAM_VALUE, "%s", token);
		}
		lineCount++;
	}
	nRet = EXIT_SUCCESS;

	// Swap the MAC addresses and add the lines to TEMP_NEXT_MAC_CONFIG file
	sprintf_s(ChangedMAC_newDefWAN, sizeof(ChangedMAC_newDefWAN), "%s>%s\n", pcIfname_newDefWAN, MAC_oldDefWAN);
	sprintf_s(ChangedMAC_oldDefWAN, sizeof(ChangedMAC_oldDefWAN), "%s>%s\n", pcIfname_oldDefWAN, MAC_newDefWAN);
	fprintf(outFile, "%s", ChangedMAC_newDefWAN);
	fprintf(outFile, "%s", ChangedMAC_oldDefWAN);
	LOGF_LOG_DEBUG("Swapped MAC Addresses : %s .... %s \n",ChangedMAC_newDefWAN,ChangedMAC_oldDefWAN);

returnHandler:
	if(inFile != NULL)
		fclose(inFile);
	if(outFile != NULL)
		fclose(outFile);
	if(nRet == EXIT_SUCCESS) {
		if(rename(TEMP_NEXT_MAC_CONF, NEXT_MAC_CONF) != 0){
			nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
			fprintf(stderr, "%s:%d renaming file is failed after swapping of mac\n", __func__,__LINE__);
		}
	}

	return nRet;
}
