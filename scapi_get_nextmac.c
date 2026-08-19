/*****************************************************************************

  Copyright (C) 2020-2023 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : scapi_get_nextmac.c                                        *
 *     Project    : UGW                                                        *
 *     Description: API to get next mac address by validating mac address
 *                  other interfaces                                           *
 *                                                                             *
 ******************************************************************************/

#define _GNU_SOURCE
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
#include <glob.h>
#include <ulogging.h>
#include <ltq_api_include.h>
#include <regex.h>
#include <scapi_structs.h>

#define SNPRINTF_GET_MAC(pDest, nBufSize, psFormat, ...) { \
        if((sprintf_s(pDest, nBufSize, psFormat, ##__VA_ARGS__)) <0 ) { \
                LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); \
                nRet = EXIT_FAILURE; \
		goto returnHandler; \
        } \
}
#define SUPPORTED_MAC_TABLE_FILE VENDOR_PATH "/servd/etc/.resv-mac.conf"
#define SUPPORTED_6G_MAC_TABLE_FILE VENDOR_PATH "/servd/etc/.resv-mac-6g.conf"
#define DEF_MAC_RESERVED_CNT 2
#define DEF_MAC_SUPPORT_CNT 64
#define DEF_6g_MAC_SUPPORT_CNT 32

#define MAC_ADDR_BYTES 6
#define MAX_U8_VAL     0xFF
#define MAX_6g_U8_VAL     0x1F
#define EDQUOTA   122

#define SCAPI_MAC_LEN 32
#define NEXT_MAC_CONF VENDOR_PATH "/servd/etc/nextmac.conf"
#define NEXT_MAC_6g_CONF VENDOR_PATH "/servd/etc/nextmac-6g.conf"
#define BOOT_CHK "/tmp/.bootchk"
#define GLOB_INTERFACES "/sys/class/net/*/address"
#define MAC_IN_USE 422
#define MAX_CNT 1000

#define UCI_PHY_DIR "/tmp/wlan-detect"
#define UCI_6G_FILE UCI_PHY_DIR"/6G-phy"
#define UNUSED(var) (void)(var)

/*  =============================================================================

*   Function name      : set_libscapi_loglevel                                 *

*   Description        : Function to set loglevel of this library              *

*  ============================================================================*/

void set_libscapi_loglevel(uint16_t unLevel, uint16_t unType)

{
        LOGLEVEL = unLevel;
        LOGTYPE = unType;
}

/*
 ** =============================================================================
 **   Function Name    :scapi_isValidMac
 **
 **   Description      :validate the incoming mac address
 **
 **   Parameters       :pcMac(IN) -> mac address
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                     Failure -> Different -ve values
 **
 ** ============================================================================
 */
static int scapi_isValidMac(const char *pcMac)
{
	int i=0;
	for(i = 0; i < 17; i++)
	{
		if(i % 3 != 2 && !isxdigit(pcMac[i]))
		{
			LOGF_LOG_ERROR("Invalid mac address retrived from conf file\n");
			return EXIT_FAILURE;
		}
		if(i % 3 == 2 && pcMac[i] != ':')
		{
			LOGF_LOG_ERROR("Invalid mac address retrived from conf file\n");
			return EXIT_FAILURE;
		}
	}
	if(pcMac[17] != '\0')
	{
		LOGF_LOG_ERROR("Invalid mac address retrived from conf file\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
 ** =============================================================================
 **   Function Name    :scapi_getFilePtr
 **
 **   Description      : get file ptr
 **
 **   Parameters       :fname(IN) -> file name to open
 mode(IN) -> file mode
 **
 **   Return Value     :Success -> file ptr
 **                     Failure -> NULL
 **
 ** ============================================================================
 */

FILE * scapi_getFilePtr(const char *fname, char *mode)
{
	FILE *file;
	if (fopen_s(&file, fname, mode) == EOK) {
		if(file != NULL)
			return file;
	}
	return NULL;

}

/*
 ** =============================================================================
 **   Function Name    :scapi_fileExists
 **
 **   Description      :check whether file is opened or not
 **
 **   Parameters       :fname(IN) -> file name
 **
 **   Return Value     :Success -> UGW_SUCCESS
 **                     Failure -> UGW_FAILURE
 **
 ** ============================================================================
 */

int scapi_fileExists(const char *fname)
{
	FILE *file;
	if ((file = scapi_getFilePtr(fname, "r")))
	{
		fclose(file);
		return UGW_SUCCESS;
	}
	return UGW_FAILURE;

}

/* 
 ** =============================================================================
 **   Function Name    :scapi_incrementMacaddr
 **
 **   Description      :Increments mac address properly till last 3 octets
 **
 **   Parameters       :pxIfr(IN) -> Pointer to ifreq type struct which is holding mac
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                     Failure -> Different -ve values
 **   
 **   NOTES            :If incremented mac has reached the maximum condition of XX:XX:XX:FF:FF:FF,
 **			API will fail
 ** ============================================================================
 */
static int scapi_incrementMacaddr(struct ifreq *pxIfr,bool bGlobal){
	int nRet = -EXIT_FAILURE;
	int nCurrentIndex = 5, nParentIndex = 4;

	//private pool
	if(!bGlobal)
	{
		pxIfr->ifr_hwaddr.sa_data[0] = 2;
	}

	pxIfr->ifr_hwaddr.sa_data[nCurrentIndex] += 1;

	/* If incremented value is 00, that means old octet is
	 * FF. You need to increment the parent index octet in
	 * this case
	 */
	for(;pxIfr->ifr_hwaddr.sa_data[nCurrentIndex] == 0;){
		/* only last 3 octets are allowed to change */

		/* Extreme case: 
		 * XX:XX:XX:FF:FF:FF ---> XX:XX:XX:00:00:00
		 *                                   */
		if(nParentIndex < 3){
			nRet = ERR_INPUT_VALIDATION_FAILED;
			goto returnHandler;
		}

		pxIfr->ifr_hwaddr.sa_data[nParentIndex] += 1;
		nCurrentIndex = nParentIndex;
		nParentIndex = nParentIndex -1;
	}
	nRet = EXIT_SUCCESS;
returnHandler:
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_get6gMacFromFile
 **
 **   Description      :gets mac address of an interface from config file(NEXT_MAC_CONFIG)
 **
 **   Parameters       :pcMac(OUT) -> Pointer to a buffer which will hold the mac
 **                                     address returned by this API
 **			pcIfname(IN) -> Interface name
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** ============================================================================
 */
static int scapi_get6gMacFromFile(char* pcIfname,char* pcMac){
	FILE *fp = scapi_getFilePtr(NEXT_MAC_6g_CONF, "ab+");
	char line[64] = {0};
	size_t len=0;
	char* saveptr = NULL;
	char* token = NULL;
	char* pos = NULL;
	char* pcFind = NULL;
	bool got_mac = false;

	char interfaceName[SCAPI_MAC_LEN] = {0};

	int nRet = -EXIT_FAILURE;

	if(fp == NULL){
		LOGF_LOG_ERROR("Couldn't open Mac Config File %d (%s)\n", nRet, strerror(errno));
		goto returnHandler;
	}
	SNPRINTF_GET_MAC(interfaceName, SCAPI_MAC_LEN, "%s>", pcIfname);
	while (fgets(line, sizeof(line), fp)) {
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';

		pcFind = strstr(line, interfaceName);
		/* Interface found */
		if( pcFind != NULL){
			/* strtok_s is used so that we don't have to trim
			 * extra white spaces. We have to do it manually
			 * with strchr*/
			if((len=strnlen_s(pcFind,MAC_IN_USE))<=0){
				LOGF_LOG_ERROR("Fetchig len failed\n");
				nRet = ERR_INPUT_VALIDATION_FAILED;
				goto returnHandler;
			}
			token = strtok_s(pcFind, &len, "> ", &saveptr);
			/* This is our mac address */
			token = strtok_s(NULL, &len , "> ", &saveptr);
			if (token == NULL){
				nRet = ERR_INPUT_VALIDATION_FAILED;
				fprintf(stderr, "[%s:%d]Paring Mac config string Failed\n",__func__,__LINE__);
				goto returnHandler;
			}
			else{
				got_mac = true;
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%s", token);
			}
			break;
		}

	}

	/* We parsed whole file but could not retreive mac address of
	   the interface */
	if(got_mac != true){
		nRet = -EXIT_FAILURE;
		LOGF_LOG_INFO("Failed to get Mac for the interface %s from file, generate new mac. \n", pcIfname);
		goto returnHandler;
	}

	nRet = EXIT_SUCCESS;
	LOGF_LOG_INFO("Mac <-> Iface - [ %s ] [ %s ]\n",pcIfname, pcMac);
returnHandler:
	if(fp != NULL)
		fclose(fp);
	return nRet;
}
/* 
 ** =============================================================================
 **   Function Name    :scapi_getMacFromFile
 **
 **   Description      :gets mac address of an interface from config file(NEXT_MAC_CONFIG)
 **
 **   Parameters       :pcMac(OUT) -> Pointer to a buffer which will hold the mac
 **                                     address returned by this API
 **			pcIfname(IN) -> Interface name
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** ============================================================================
 */
static int scapi_getMacFromFile(char* pcIfname,char* pcMac){
	FILE *fp = scapi_getFilePtr(NEXT_MAC_CONF, "ab+");
	char line[64] = {0};
	size_t len=0;

	char* saveptr = NULL;
	char* token = NULL;
	bool got_mac = false;

	char interfaceName[SCAPI_MAC_LEN] = {0};

	int nRet = -EXIT_FAILURE;

	if(fp == NULL){
		LOGF_LOG_ERROR("Couldn't open Mac Config File %d (%s)\n", nRet, strerror(errno));
		goto returnHandler;
	}
	/* or else 'eth0' will match with eth0_1 during strstr */
	SNPRINTF_GET_MAC(interfaceName, SCAPI_MAC_LEN, "%s>", pcIfname);
	while (fgets(line, sizeof(line), fp)) {
		char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';

		char* pcFind = strstr(line, interfaceName);
		/* Interface found */
		if( pcFind != NULL){
			/* strtok_s is used so that we don't have to trim
			 * extra white spaces. We have to do it manually
			 * with strchr*/
			if((len=strnlen_s(pcFind,MAC_IN_USE))<=0){
				LOGF_LOG_ERROR("Fetchig len failed\n");
				nRet = ERR_INPUT_VALIDATION_FAILED;
				goto returnHandler;
			}
			token = strtok_s(pcFind, &len, "> ", &saveptr);
			/* This is our mac address */
			token = strtok_s(NULL, &len , "> ", &saveptr);
			if (token == NULL){
				nRet = ERR_INPUT_VALIDATION_FAILED;
				fprintf(stderr, "[%s:%d]Paring Mac config string Failed\n",__func__,__LINE__);
				goto returnHandler;
			}
			else{
				got_mac = true;
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%s", token);
			}
			break;
		}

	}

	/* We parsed whole file but could not retreive mac address of
	 * the interface */
	if(got_mac != true){
		nRet = -EXIT_FAILURE;
		LOGF_LOG_INFO("Failed to get Mac for the interface %s from file, generate new mac. \n", pcIfname);
		goto returnHandler;
	}

	nRet = EXIT_SUCCESS;
	LOGF_LOG_INFO("Mac <-> Iface - [ %s ] [ %s ]\n",pcIfname, pcMac);
returnHandler:
	if(fp != NULL)
		fclose(fp);
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_validate6gMacaddr
 **
 **   Description      :Validates a given mac address (incremented) with the mac
 **			addresses of other interfaces in the system as well as
 **			with the interfaces (which might be not added to the system yet)
 **			in the config file
 **
 **   Parameters       :pcMac(IN) -> Mac address to be validated
 **                     pcIfName(IN) -> Interface name
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values
 **
 **   NOTES            :Doesn't validate with mac of pcIfname because it might be garbage
 **			When the interface is just up 
 ** ============================================================================
 */
static int scapi_validate6gMacaddr(char* pcIfname, char* pcMac){
	int nRet = -EXIT_FAILURE;
	FILE *fp = scapi_getFilePtr(NEXT_MAC_6g_CONF, "r");
	char line[128] = {0};

	UNUSED(pcIfname);
	if(fp == NULL){
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		LOGF_LOG_ERROR("Couldn't open interfaces config File : %s \n", NEXT_MAC_CONF);
		goto returnHandler;
	}

	/* Validate with interfaces in config file now. Some interfaces
	   might be added to the config but not to the system */
	while (fgets(line, sizeof(line), fp)) {
		char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
		char* pcFind = strstr(line, pcMac);
		/* This mac is already in use for some other interface*/
		if( pcFind != NULL){
			nRet = -MAC_IN_USE;
			LOGF_LOG_ERROR("Mac [ %s ] is been already used by other interface\n", pcMac);
			goto returnHandler;
		}

	}
	nRet = EXIT_SUCCESS;
returnHandler:
	//globfree(&results);
	if(fp != NULL)
		fclose(fp);
	return nRet;
}
/* 
 ** =============================================================================
 **   Function Name    :scapi_validateMacaddr
 **
 **   Description      :Validates a given mac address (incremented) with the mac
 **			addresses of other interfaces in the system as well as
 **			with the interfaces (which might be not added to the system yet)
 **			in the config file
 **
 **   Parameters       :pcMac(IN) -> Mac address to be validated
 **                     pcIfName(IN) -> Interface name
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values
 **
 **   NOTES            :Doesn't validate with mac of pcIfname because it might be garbage
 **			When the interface is just up 
 ** ============================================================================
 */
static int scapi_validateMacaddr(char* pcIfname, char* pcMac){
	int nRet = -EXIT_FAILURE;
	FILE *fp = scapi_getFilePtr(NEXT_MAC_CONF, "r");
	char ifcStr[64] = {0};
	glob_t results = {0};
	char line[128] = {0};


	if(fp == NULL){
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		LOGF_LOG_ERROR("Couldn't open interfaces config File : %s \n", NEXT_MAC_CONF);
		goto returnHandler;
	}

	nRet = glob(GLOB_INTERFACES, 0, NULL, &results);

	if(nRet != 0){
		nRet = ERR_MACADDRESS_GLOB_FETCH_FAILED;
		fprintf(stderr,"Glob fetch failed ret=%d , reason=%s\n", nRet, strerror(errno));
		goto returnHandler;
	}

	unsigned int i = 0;
	/* Validate the mac with interfaces in the system */
	/* Loop through all interfaces interfaces in the system and
	 * validate*/
	for (i = 0; i < results.gl_pathc; i++){
		SNPRINTF_GET_MAC(ifcStr, 64, "%s", results.gl_pathv[i]);
		char* pcCurrentIfName = strrchr(ifcStr, '/');
		if(pcCurrentIfName == NULL){
			nRet = ERR_INPUT_VALIDATION_FAILED;
			fprintf(stderr,"[%s:%d] Couldn't get interface name\n",__func__, __LINE__);
			goto returnHandler;
		}
		*pcCurrentIfName = '\0';
		pcCurrentIfName = strrchr(ifcStr, '/');
		pcCurrentIfName++;
		/* Validate against all other interfaces except pcIfname*/
		if(strcmp(pcIfname, pcCurrentIfName) != 0){
			char pcMacIfc[SCAPI_MAC_LEN] = {0};
			nRet = scapi_getIfcMacaddr(pcCurrentIfName, pcMacIfc);
			if( nRet < 0){
				if ( (-nRet) == ENODEV ) {
					nRet = 0;
					LOGF_LOG_ERROR("Failed to get Mac Address for [ %s ],No such device\n", pcCurrentIfName);
					continue;
				} else {
					nRet = ERR_MACADDRESS_INTERFACE_NOT_UP;
					LOGF_LOG_ERROR("Failed to get Mac Address for [ %s ]\n", pcCurrentIfName);
					goto returnHandler;
				}
			}
			//printf("Interface = %s, IfcMac = %s, MacTest = %s\n", pcCurrentIfName, pcMacIfc, pcMac);
			/* If a match is found. Current mac is invalid*/
			if( strcmp(pcMac, pcMacIfc) == 0){
				nRet = -MAC_IN_USE;
				LOGF_LOG_INFO("MAC address in use, Mac should be incremented further\n");
				goto returnHandler;
			}
		}
	}

	/* Validate with interfaces in config file now. Some interfaces
	   might be added to the config but not to the system */
	while (fgets(line, sizeof(line), fp)) {
		char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
		//printf("Line = %s, MacTest = %s\n", line, pcMac);
		char* pcFind = strstr(line, pcMac);
		/* This mac is already in use for some other interface*/
		if( pcFind != NULL){
			nRet = -MAC_IN_USE;
			LOGF_LOG_ERROR("Mac [ %s ] is been already used by other interface\n", pcMac);
			goto returnHandler;
		}

	}
	nRet = EXIT_SUCCESS;
returnHandler:
	globfree(&results);
	if(fp != NULL)
		fclose(fp);
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_getBaseMac
 **
 **   Description      :Gets base mac from /proc/cmdline.
 **
 **   Parameters       :pcMac(OUT) -> Pointer to a buffer which will hold the mac
 **					address returned by this API
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** ============================================================================
 */

int scapi_getBaseMac(char* pcMac){
	FILE* fpProc = NULL;
	char fbuf[512] = {0};
	//char proc_mac_addr[SCAPI_MAC_LEN] = {0};
	int nRet = -EXIT_FAILURE, check = 0;
	struct stat st;

	check = stat("/nvram/appcpu.cfg", &st);
	if(!check) {
		fpProc = popen("/usr/sbin/nvram_env.sh get ethaddr", "r");
	} else {
		fpProc = scapi_getFilePtr("/proc/cmdline", "r");
	}
	if (fpProc == NULL)
	{
		if (!check) {
			LOGF_LOG_ERROR("popen(/usr/sbin/nvram_env.sh) failed due to '%m'\n");
		} else {
			LOGF_LOG_ERROR("/proc/cmdline not found!!\n");
		}
		nRet = ERR_MACADDRESS_GET_BASE_MAC_FAILED;
		goto returnHandler;
	}
	else
	{
		char *p = fgets(fbuf, sizeof(fbuf), fpProc);
		if (p == NULL) {
			nRet = ERR_MACADDRESS_GET_BASE_MAC_FAILED;
			goto returnHandler;
		}
		if (!check) {
			p = fbuf;
		} else {
			p = strstr(fbuf, "ethaddr"); /* Searching for 'ethaddr=MAC' (or may be 'ethaddr MAC') from /proc/cmdline */
			p = ((p) ? (p + 8) : NULL);
		}
		if(p != NULL)
		{
			int i = 0;
			for(i=0;i<17;i++)
			{
				pcMac[i] = *(p + i);
			}

		}
		else{
			LOGF_LOG_ERROR("Failed to Read Mac from /proc/cmdline\n");
			nRet = ERR_MACADDRESS_GET_BASE_MAC_FAILED;
			goto returnHandler;
		}
	}
	nRet = EXIT_SUCCESS;
	LOGF_LOG_INFO("Base Mac Generated is [ %s ] \n",pcMac);
returnHandler:
	if (fpProc != NULL) {
		if (!check) {
			pclose(fpProc);
		} else {
			fclose(fpProc);
		}
	}
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_validateWithBase
 **
 **   Description      :Validates a given mac with base mac from /proc/cmdline.
 **			validation is done by comparing first 3 octets of the mac
 **
 **   Parameters       :pcMac(IN) -> Mac address to validate
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** 
 **
 ** ============================================================================
 */

static int scapi_validateWithBase(char* pcMac){
	char proc_mac_addr[SCAPI_MAC_LEN] = {0};
	int nRet = -EXIT_FAILURE;

	if (scapi_getBaseMac(proc_mac_addr) != EXIT_SUCCESS) {
		goto returnHandler;
	}

	//printf("Mac = %s, Proc Mac = %s\n", pcMac, proc_mac_addr);
	/* Check if first 3 octets are matching or else fail */
	if(strncasecmp(pcMac, proc_mac_addr, 9) != 0){
		nRet = -EINVAL;
		goto returnHandler;
	}
	nRet = EXIT_SUCCESS;

returnHandler:
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_incMacAddr
 **
 **   Description      : Increment the public mac.
 **
 **   Parameters       :pAddr(INOUT) -> Mac address to increment
 **			pnIncCnt -> increment count
 ** 			pnPrCnt -> prent count
 ** 			max_val -> increment range
 ** 
 **   Return Value     :
 **
 ** ============================================================================
 */
static void scapi_incMacAddr(unsigned char *pAddr, unsigned int *pnIncCnt, unsigned int *pnPrCnt, unsigned int max_val)
{
	unsigned int incr_index, parent_index ;
	incr_index = *pnIncCnt ;
	parent_index = *pnPrCnt ;
	if ( pAddr[incr_index] == max_val )
	{
		if ( pAddr[parent_index] == max_val) 
		{
			pAddr[parent_index] = 0 ;
			parent_index--;
		}
		pAddr[parent_index] +=1 ;
		pAddr[incr_index] = 0 ;
	}
	else
	{
		pAddr[incr_index]++ ;
	}

	*pnIncCnt = incr_index ;
	*pnPrCnt = parent_index ;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_validate6gLimit
 **
 **   Description      :validate the limit of the mac address supported by DUT
 **
 **   Parameters       :pcMac(IN) -> mac address
 **                      bGlobal(IN) -> flag to chk private or global mac
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** 
 **   Notes            :Top layer should allocate a NULL terminated array of size SCAPI_MAC_LEN  
 **
 ** ============================================================================
 */
static int scapi_validate6gLimit(char *pcMac, bool bGlobal)
{

	FILE *fFp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int nRet;
	if(bGlobal == true)
	{
		// verify against reserved mac
		fFp = scapi_getFilePtr(SUPPORTED_6G_MAC_TABLE_FILE, "r");	
		if(fFp != NULL)
		{
			while ((read = getline(&line, &len, fFp)) != -1) 
			{
				if(strncasecmp(pcMac,line,17)==0)
				{
					fclose(fFp);
					nRet=EXIT_SUCCESS;
					goto finish;
				}
			}
		}
		if(fFp != NULL) fclose(fFp);
		nRet=EDQUOTA;
	}
	else
	{
		// mac is from private pool.
		nRet=EXIT_SUCCESS;
	}
finish:
	if(line) free(line);
	return nRet;
}
/* 
 ** =============================================================================
 **   Function Name    :scapi_validateLimit
 **
 **   Description      :validate the limit of the mac address supported by DUT
 **
 **   Parameters       :pcMac(IN) -> mac address
 **                      bGlobal(IN) -> flag to chk private or global mac
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** 
 **   Notes            :Top layer should allocate a NULL terminated array of size SCAPI_MAC_LEN  
 **
 ** ============================================================================
 */
static int scapi_validateLimit(char *pcMac, bool bGlobal)
{

	FILE *fFp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int nRet;
	if(bGlobal == true)
	{
		// verify against reserved mac
		fFp = scapi_getFilePtr(SUPPORTED_MAC_TABLE_FILE, "r");	
		if(fFp != NULL)
		{
			while ((read = getline(&line, &len, fFp)) != -1) 
			{
				if(strncasecmp(pcMac,line,17)==0)
				{
					fclose(fFp);
					nRet=EXIT_SUCCESS;
					goto finish;
				}
			}
		}
		if(fFp != NULL) fclose(fFp);
		nRet=EDQUOTA;
	}
	else
	{
		// mac is from private pool.
		nRet=EXIT_SUCCESS;
	}
finish:
	if(line) free(line);
	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_getRunTimeVal
 **
 **   Description      :function to get the runtime value uboot or nvram 
 **
 **   Parameters       :pcMac(IN) -> mac address
 **                      pnVal(out) -> value will returned on successful
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> -EXIT_FAILURE
 ** 
 ** ============================================================================
 */
static int scapi_getRunTimeVal(char *pcMac, int *pnVal)
{
	FILE* fp = NULL;
	struct stat st;
	char sBuf[SCAPI_MAC_LEN]={0};
	int nChk;

	nChk = stat("/nvram/appcpu.cfg", &st);
	if(strcmp(pcMac,"sup_mac")==0){
		if(nChk==0)
			fp = popen("/usr/sbin/nvram_env.sh get sup_mac", "r");
		else
			fp = popen("uboot_env --get --name sup_mac", "r");
	}else{
		if(nChk==0)
			fp = popen("/usr/sbin/nvram_env.sh get res_mac", "r");
		else
			fp = popen("uboot_env --get --name res_mac", "r");
	}

	if (fp == NULL) {
		if(strcmp(pcMac,"sup_mac")==0){
			//sup_mac variable not found, return default value
			fprintf(stderr, "\n @@@ Using default value as sup_mac variable not defined @@@ \n");
			*pnVal = DEF_MAC_SUPPORT_CNT;
			return EXIT_SUCCESS;
		}
		else {
			fprintf(stderr,"Fetching Run Timevalue is failed with reason (%s)", strerror(errno));
			return ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		}
	}else{
		if(fread(sBuf, 1, sizeof(sBuf), fp) <= 0) {
			if(strcmp(pcMac,"sup_mac")==0)
				*pnVal=DEF_MAC_SUPPORT_CNT;
			else
				*pnVal=DEF_MAC_RESERVED_CNT;
		}else{
			//sup_mac variable not found in uboot use default value
			if((strcmp(pcMac,"sup_mac")==0) && (atoi(sBuf)==0) ){
				fprintf(stderr, "\n @@@ Using default value as sup_mac variable not defined @@@ \n");
				*pnVal = DEF_MAC_SUPPORT_CNT;
			}
			else
				*pnVal = atoi(sBuf);

			LOGF_LOG_INFO("\n @@@@@@@@@ mac %d @@@@@@@@ \n", *pnVal);
		}
		pclose(fp);
	}
	return EXIT_SUCCESS;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_gen6gResevMacTable
 **
 **   Description      :function to generate the mac table
 **
 **   Parameters       :pcBaseMac(IN) -> mac address
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> -EXIT_FAILURE
 ** 
 ** ============================================================================
 */
static int scapi_gen6gResevMacTable(char *pcBaseMac)
{
	int nCnt=0;
	FILE *fp=NULL;
	unsigned char mac_addr[MAC_ADDR_BYTES]={0};
	char *pcTmp=NULL,*pcSavePtr=NULL;
	unsigned int nMacCurIdx=MAC_ADDR_BYTES-1,nPranetIdx=MAC_ADDR_BYTES-2;
	int nMacMaxCnt=0, nResMacCnt=0;
	size_t len=0;
	int nRet=EXIT_SUCCESS;

	char *pcMac=NULL;

	if((pcMac=strdup(pcBaseMac)) == NULL){
		nRet = ERR_INPUT_VALIDATION_FAILED;
		goto returnHandler;
	}

	fp = scapi_getFilePtr(SUPPORTED_6G_MAC_TABLE_FILE, "a+");	
	if( fp == NULL){
		fprintf(stderr, "[%s: %d] Failed to open mac table file\n",__func__, __LINE__);
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		goto returnHandler;
	}else{
		nMacMaxCnt=0; nResMacCnt=0; 

		LOGF_LOG_INFO ("\n @@@@@ mac : %s @@@@@@ \n",pcMac);
		if((len=strnlen_s(pcMac,SCAPI_MAC_LEN)) <=0 ){
			fprintf(stderr, "[%s: %d] Fectching len failed\n",__func__, __LINE__);
			nRet = ERR_INPUT_VALIDATION_FAILED;
			goto returnHandler;
		}
		pcTmp=strtok_s(pcMac,&len, ":",&pcSavePtr);
		nCnt=0;
		while(pcTmp!=NULL)
		{
			sscanf_s (pcTmp,"%hhx",&(mac_addr[nCnt]));
			nCnt++;
			pcTmp=strtok_s(NULL,&len, ":",&pcSavePtr);
		}

		nMacMaxCnt = DEF_6g_MAC_SUPPORT_CNT;
 		fprintf(fp,"sup_mac=%d\n",nMacMaxCnt);
		nCnt = nMacMaxCnt-nResMacCnt;


		if(nCnt > MAX_CNT)
		{
			nRet=ERR_MACADDRESS_POOL_EXHAUSTED;
			fprintf(stderr, "[%s: %d]Max MAC ADDRESS count going behind the soft limit(1000) \n",__func__, __LINE__);
			goto returnHandler;
		}

		while (nCnt>0)
		{
			LOGF_LOG_INFO("%02x:%02x:%02x:%02x:%02x:%02x\n", 
					mac_addr[0], mac_addr[1], mac_addr[2], 
					mac_addr[3], mac_addr[4], mac_addr[5]);
			fprintf(fp,"%02x:%02x:%02x:%02x:%02x:%02x\n", 
					mac_addr[0], mac_addr[1], mac_addr[2], 
					mac_addr[3], mac_addr[4], mac_addr[5]);
			scapi_incMacAddr(mac_addr, &nMacCurIdx, &nPranetIdx, MAX_U8_VAL);
			nCnt--;
		}
	}
returnHandler:
	if(fp)
		fclose(fp);
	if(pcMac != NULL)
		free(pcMac);
	return nRet;
}
/* 
 ** =============================================================================
 **   Function Name    :scapi_genResevMacTable
 **
 **   Description      :function to generate the mac table
 **
 **   Parameters       :pcBaseMac(IN) -> mac address
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> -EXIT_FAILURE
 ** 
 ** ============================================================================
 */
static int scapi_genResevMacTable(char *pcBaseMac)
{
	int nCnt=0;
	FILE *fp=NULL;
	unsigned char mac_addr[MAC_ADDR_BYTES]={0};
	char *pcTmp=NULL,*pcSavePtr=NULL;
	unsigned int nMacCurIdx=MAC_ADDR_BYTES-1,nPranetIdx=MAC_ADDR_BYTES-2;
	int nMacMaxCnt=0, nResMacCnt=0;
	size_t len=0;
	int nRet=EXIT_SUCCESS;

	char *pcMac=NULL;

	if((pcMac=strdup(pcBaseMac)) == NULL){
		nRet = ERR_INPUT_VALIDATION_FAILED;
		goto returnHandler;
	}

	fp = scapi_getFilePtr(SUPPORTED_MAC_TABLE_FILE, "a+");	
	if( fp == NULL){
		fprintf(stderr, "[%s: %d] Failed to open mac table file\n",__func__, __LINE__);
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		goto returnHandler;
	}else{
		nMacMaxCnt=0; nResMacCnt=0; 

		LOGF_LOG_INFO ("\n @@@@@ mac : %s @@@@@@ \n",pcMac);
		if((len=strnlen_s(pcMac,SCAPI_MAC_LEN)) <=0 ){
			fprintf(stderr, "[%s: %d] Fectching len failed\n",__func__, __LINE__);
			nRet = ERR_INPUT_VALIDATION_FAILED;
			goto returnHandler;
		}
		pcTmp=strtok_s(pcMac,&len, ":",&pcSavePtr);
		nCnt=0;
		while(pcTmp!=NULL)
		{
			sscanf_s (pcTmp,"%hhx",&(mac_addr[nCnt]));
			nCnt++;
			pcTmp=strtok_s(NULL,&len, ":",&pcSavePtr);
		}

		// call api to read uboot value
		if (scapi_getRunTimeVal("sup_mac", &nMacMaxCnt) != EXIT_SUCCESS)
		{
			goto returnHandler;
		}

		if (scapi_getRunTimeVal("res_mac", &nResMacCnt) != EXIT_SUCCESS)
		{
			goto returnHandler;
		}

		fprintf(fp,"sup_mac=%d\n",nMacMaxCnt);
		nCnt = nMacMaxCnt-nResMacCnt;


		if(nCnt > MAX_CNT)
		{
			nRet=ERR_MACADDRESS_POOL_EXHAUSTED;
			fprintf(stderr, "[%s: %d]Max MAC ADDRESS count going behind the soft limit(1000) \n",__func__, __LINE__);
			goto returnHandler;
		}

		while (nCnt>0)
		{
			LOGF_LOG_INFO("%02x:%02x:%02x:%02x:%02x:%02x\n", 
					mac_addr[0], mac_addr[1], mac_addr[2], 
					mac_addr[3], mac_addr[4], mac_addr[5]);
			fprintf(fp,"%02x:%02x:%02x:%02x:%02x:%02x\n", 
					mac_addr[0], mac_addr[1], mac_addr[2], 
					mac_addr[3], mac_addr[4], mac_addr[5]);
			scapi_incMacAddr(mac_addr, &nMacCurIdx, &nPranetIdx, MAX_U8_VAL);
			nCnt--;
		}
	}
returnHandler:
	if(fp)
		fclose(fp);
	if(pcMac != NULL)
		free(pcMac);
	return nRet;
}

/*
 ** =============================================================================
 **   Function Name    :scapi_getMacAddrFromResvList
 **
 **   Description      :Gets Base Mac and Max Supported Mac from resv list
 **
 **   Parameters       :pFile(IN) -> Max Supported Mac
 **                      pMac(OUT) -> base mac address
 **                      nSupMac(OUT) -> Max Supported Mac.
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values
 **
 ** ============================================================================
 */
static int scapi_getMacAddrFromResvList(const char *pFile, char *pMac, int *nSupMac)
{
	/* file available check whether max mac address count modified or not */
	char * line = NULL, *s = NULL, *pcSavePtr = NULL;
	size_t len = 0; ssize_t read, nCt = 0;
	size_t len1 = 0;
	int nRet = -EXIT_FAILURE;
	FILE *pFp = NULL;

	/* Read Base Mac and Max Supported Mac from resv list */
	pFp = scapi_getFilePtr(pFile, "r");
	if (!pFp) {
		LOGF_LOG_DEBUG("%s reserve MAC list file open failed\n", pFile);
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		goto fail;
	}
	while (((read = getline(&line, &len, pFp)) != -1) || nCt < 2) {
		if ((len1 = strnlen_s(line, MAC_IN_USE)) <= 0) {
			fprintf(stderr, "Fectching len failed\n");
			nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
			goto fail;
		}
		if ((s = strtok_s(line, &len1, "=", &pcSavePtr)) != NULL) {
			s = strtok_s(NULL, &len1, "\n", &pcSavePtr);
			if (s != NULL)
				*nSupMac = atoi(s);
			else
				*nSupMac = DEF_MAC_SUPPORT_CNT;
		}
		/* validate if base mac is modified or not */
		if (nCt == 1) {
			if (sprintf_s(pMac, SCAPI_MAC_LEN, "%s", line) <= 0) {
				nRet = -EXIT_FAILURE;
				goto fail;
			}
		}
		nCt++;
	}
	nRet = EXIT_SUCCESS;

fail:
	if (pFp)
		fclose(pFp);
	if (line)
		free(line);

	return nRet;
}

/* 
 ** =============================================================================
 **   Function Name    :scapi_getMbssidNextMacaddr
 **
 **   Description      :Gets next mac address of an interface and saves it in config file
 **
 **   Parameters       :pcIfname(IN) -> interface name
 **                      pcMac(OUT) -> resultant mac address. 
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** 
 **   Notes            :Top layer should allocate a NULL terminated array of size SCAPI_MAC_LEN  
 **
 ** ============================================================================
 */

int scapi_getMbssidNextMacaddr(char* pcIfname, bool bGlobal, char* pcMac)
{
	struct ifreq xIfr = {.ifr_ifru={0}};
	int nRet = -EXIT_FAILURE, nCnt = 0;
	FILE* fp = NULL;
	struct stat st;
	int nMacMaxCnt = 0, nMacCnt = 0, nMacModified = 0;
	char sBaseMac[SCAPI_MAC_LEN] = {0};
	char sBuf[SCAPI_MAC_LEN] = {0};

	memset(&st, 0, sizeof(st));

	if(pcIfname == NULL || pcMac == NULL || !strcmp(pcIfname,""))
	{
		nRet = ERR_INPUT_VALIDATION_FAILED;
		LOGF_LOG_CRITICAL("Invalid Values either pcIfname[%s] or pcMac is NULL\n", pcIfname);
		goto returnHandler;
	}
	/*Only Global address saved in the DB*/
	if(pcMac[0] != '\0')
	{
		if(bGlobal == false)
		{
			pcMac[0] = '0';
			pcMac[1] = '2';
		}
	}

	/* get base mac */
	nRet = scapi_getBaseMac(sBaseMac);
	sBaseMac[15] = '0';
	sBaseMac[16] = '0';

	if(nRet != EXIT_SUCCESS)
	{
		fprintf(stderr,"scapi_getBaseMac(..) returned Failure for [ %s ] = ret(%d)\n", 
				pcIfname, nRet);
		goto returnHandler;
	}

	sscanf_s(sBaseMac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[0]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[1]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[2]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[3]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[4]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[5]));


	if (scapi_fileExists(SUPPORTED_6G_MAC_TABLE_FILE) == UGW_FAILURE) {
		//call api to generate res mac table	
		nRet = scapi_gen6gResevMacTable(sBaseMac);
		if( nRet != EXIT_SUCCESS)
			goto returnHandler;
	} else {
		if (scapi_fileExists(UCI_6G_FILE) == UGW_FAILURE) {
			/* Take the last byte Base Mac from resv list from Second reboot onwards */
			if (scapi_fileExists(SUPPORTED_6G_MAC_TABLE_FILE) == UGW_SUCCESS) {
				nRet = scapi_getMacAddrFromResvList(SUPPORTED_6G_MAC_TABLE_FILE, sBuf, &nMacMaxCnt);
				if (nRet != EXIT_SUCCESS) {
					LOGF_LOG_DEBUG("Get BaseMAC for resv list failed\n");
					goto returnHandler;
				}
			}
			if ((nRet = scapi_getRunTimeVal("sup_mac",&nMacCnt)) == EXIT_SUCCESS) {
				if ((nMacMaxCnt != nMacCnt) || (strncasecmp(sBuf, sBaseMac,17) != 0)) {
					LOGF_LOG_INFO( "uboot env cnt:%d previous cnt: %d current mac:%s previous mac :%s\n",
							nMacCnt, nMacMaxCnt, sBuf, sBaseMac);
					/* regenrate the mac table */
					remove(NEXT_MAC_6g_CONF);
					remove(SUPPORTED_6G_MAC_TABLE_FILE);
					nMacModified = 1;
					if (scapi_gen6gResevMacTable(sBaseMac) != EXIT_SUCCESS)
						goto returnHandler;
				}
			} else {
				fprintf(stderr, "Retrival of supported mac info is failed reason:%d\n",nRet);
				goto returnHandler;
			}	
		}
	}

	LOGF_LOG_INFO("Generating Mac for [%s] From BaseMac \n",pcIfname);

	if(scapi_fileExists(NEXT_MAC_6g_CONF) == UGW_SUCCESS)
	{
		nRet = scapi_get6gMacFromFile(pcIfname, pcMac);
		if(nRet == EXIT_SUCCESS)
		{
			if(scapi_isValidMac(pcMac) == EXIT_SUCCESS)
			{
				nRet=UGW_SUCCESS;
				goto returnHandler;
			}
			else
			{
				// if retrived mac is corrupted
				nRet=scapi_removeNextMacaddr(pcIfname);
				if(nRet != EXIT_SUCCESS)
					remove(NEXT_MAC_CONF);
			}
		}else if(nRet == ERR_INPUT_VALIDATION_FAILED){
			goto returnHandler;
		}else{
			LOGF_LOG_INFO("Getting MAC Address from file is failed,new mac request\n");
		}
	}
	else
	{
		if(bGlobal == false)
		{
			if(pcMac[0] == '\0' || nMacModified == 1)
			{
				xIfr.ifr_hwaddr.sa_data[0]=2;	
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
						(unsigned char)xIfr.ifr_hwaddr.sa_data[0],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[1],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[2],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[3],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[4],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[5]);
			}
		}
		else
		{
			if(pcMac[0] == '\0' || nMacModified == 1)
			{
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%s",sBaseMac);
			}
			//validate mac against limit
			nRet = scapi_validate6gLimit(pcMac,bGlobal);
			if(nRet == EDQUOTA)
			{
				fprintf(stderr, " [%s:%d] Generated mac not matching with reserved mac list or max limit reached %s\n",__func__,__LINE__,pcMac);
				nRet = ERR_MACADDRESS_POOL_EXHAUSTED;
				goto returnHandler;
			}
		}
		goto finish;
	}

	nRet = UGW_SUCCESS;
	//KL fix
	while(nCnt<1000)
	{
		memset(&st, 0, sizeof(st));

		if((pcMac[0] == '\0') || (nMacModified == 1) || (nRet == -MAC_IN_USE))
		{
			if (stat(NEXT_MAC_6g_CONF, &st) == 0 && st.st_size != 0) {
				nRet = scapi_incrementMacaddr(&xIfr,bGlobal);

				if(nRet != EXIT_SUCCESS){
					fprintf(stderr, "[%s:%d] Next incrementMacAddr API failed \n", __func__,__LINE__); 
					goto returnHandler;
				}
			}

			/* Top layer should provide a NULL terminated array of size SCAPI_MAC_LEN */
			/* get result */
			SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
					(unsigned char)xIfr.ifr_hwaddr.sa_data[0],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[1],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[2],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[3],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[4],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[5]);

		}
		nRet = scapi_validate6gLimit(pcMac,bGlobal);

		if(nRet == EDQUOTA)
		{
			fprintf(stderr, " [%s:%d] Generated mac not matching with reserved mac list or max limit reached %s\n",__func__,__LINE__,pcMac);
			nRet = ERR_MACADDRESS_POOL_EXHAUSTED;
			goto returnHandler;
		}

		nRet  = scapi_validate6gMacaddr(pcIfname, pcMac);

		/* Validation successful */
		if( nRet == 0){
			break;
		}else if( nRet == -MAC_IN_USE) {
			nCnt++;
			continue;
		} else {
			fprintf(stderr,"Unable to validate the mac with interfaces ret=%d\n",nRet);
			goto returnHandler;
		}
	}
finish:
	/* This file is already created by scapi_getNextMacFromFile() API */
	fp = scapi_getFilePtr(NEXT_MAC_6g_CONF, "a+");
	if(fp == NULL){
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		fprintf(stderr,"[%s:%d] next_mac_conf file update failed reason:%s\n", __func__, __LINE__, strerror(errno));
		goto returnHandler;
	}
	fprintf(fp, "%s> %s\n", pcIfname, pcMac);

	nRet = UGW_SUCCESS;
	LOGF_LOG_INFO("Mac Generated [ %s ] : [%s ]\n",pcIfname, pcMac);
returnHandler:
	if(fp != NULL)
		fclose(fp);
	return nRet;
}


/* 
 ** =============================================================================
 **   Function Name    :scapi_getNextMacaddr
 **
 **   Description      :Gets next mac address of an interface and saves it in config file
 **
 **   Parameters       :pcIfname(IN) -> interface name
 **                      pcMac(OUT) -> resultant mac address. 
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                      Failure -> Different -ve values 
 ** 
 **   Notes            :Top layer should allocate a NULL terminated array of size SCAPI_MAC_LEN  
 **
 ** ============================================================================
 */

int scapi_getNextMacaddr(char* pcIfname, bool bGlobal, char* pcMac)
{
	struct ifreq xIfr = {.ifr_ifru={0}};
	int nRet = -EXIT_FAILURE, nCnt = 0;
	FILE* fp = NULL;
	struct stat st;
	int nMacMaxCnt = 0, nMacCnt = 0, nFd = -1, nMacModified = 0;
	char sBaseMac[SCAPI_MAC_LEN] = {0};
	char sBuf[SCAPI_MAC_LEN] = {0};

	memset(&st, 0, sizeof(st));

	if(pcIfname == NULL || pcMac == NULL || !strcmp(pcIfname,""))
	{
		nRet = ERR_INPUT_VALIDATION_FAILED;
		LOGF_LOG_CRITICAL("Invalid Values either pcIfname[%s] or pcMac is NULL\n", pcIfname);
		goto returnHandler;
	}
	/*Only Global address saved in the DB*/
	if(pcMac[0] != '\0')
	{
		if(bGlobal == false)
		{
			pcMac[0] = '0';
			pcMac[1] = '2';
		}
	}

	/* get base mac */
	nRet = scapi_getBaseMac(sBaseMac);

	if(nRet != EXIT_SUCCESS)
	{
		fprintf(stderr,"scapi_getBaseMac(..) returned Failure for [ %s ] = ret(%d)\n", 
				pcIfname, nRet);
		goto returnHandler;
	}

	/* Modify Base MAC if 6G is enabled for first boot */
	if (scapi_fileExists(UCI_6G_FILE) == UGW_SUCCESS) {
		sBaseMac[15] = '2';
		sBaseMac[16] = '0';
	} else {
		/* Take the last byte Base Mac from resv list from Second reboot onwards */
		if (scapi_fileExists(SUPPORTED_MAC_TABLE_FILE) == UGW_SUCCESS) {
			nRet = scapi_getMacAddrFromResvList(SUPPORTED_MAC_TABLE_FILE, sBuf, &nMacMaxCnt);
			if (nRet != EXIT_SUCCESS) {
				LOGF_LOG_DEBUG("Get BaseMAC for resv list failed\n");
				goto returnHandler;
			}
			sBaseMac[15] = sBuf[15];
			sBaseMac[16] = sBuf[16];
		}
	}

	sscanf_s(sBaseMac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[0]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[1]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[2]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[3]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[4]),
			(unsigned char *)&(xIfr.ifr_hwaddr.sa_data[5]));


	if(scapi_fileExists(SUPPORTED_MAC_TABLE_FILE) == UGW_FAILURE) {
		//call api to generate res mac table	
		nRet = scapi_genResevMacTable(sBaseMac);
		if ( nRet != EXIT_SUCCESS) {
			goto returnHandler;
		}
	} else {
		if (scapi_fileExists(BOOT_CHK) == UGW_FAILURE) {
			if ((nRet = scapi_getRunTimeVal("sup_mac", &nMacCnt)) == EXIT_SUCCESS) {
				if ((nMacMaxCnt != nMacCnt) || (strncasecmp(sBuf, sBaseMac, 17) != 0)) {
					LOGF_LOG_INFO( "uboot env cnt:%d previous cnt: %d current mac:%s previous mac :%s\n",
							nMacCnt, nMacMaxCnt, sBuf, sBaseMac);
					/* regenrate the mac table */
					remove(NEXT_MAC_CONF);
					remove(SUPPORTED_MAC_TABLE_FILE);
					nMacModified = 1;
					if (scapi_genResevMacTable(sBaseMac) != EXIT_SUCCESS) {
						goto returnHandler;
					}
				}
			} else {
				fprintf(stderr, "Retrival of supported mac info is failed reason:%d\n", nRet);
				goto returnHandler;
			}	
		}
	}

	if(scapi_fileExists(BOOT_CHK) == UGW_FAILURE)
	{
		if((nFd = open(BOOT_CHK, O_RDWR|O_CREAT,0666)) == -1) 
			perror("open");
		if(nFd >= 0)
			close(nFd);
	}

	LOGF_LOG_INFO("Generating Mac for [%s] From BaseMac \n",pcIfname);

	if(scapi_fileExists(NEXT_MAC_CONF) == UGW_SUCCESS)
	{
		nRet = scapi_getMacFromFile(pcIfname, pcMac);
		if(nRet == EXIT_SUCCESS)
		{
			if(scapi_isValidMac(pcMac) == EXIT_SUCCESS)
			{
				nRet=UGW_SUCCESS;
				goto returnHandler;
			}
			else
			{
				// if retrived mac is corrupted
				nRet=scapi_removeNextMacaddr(pcIfname);
				if(nRet != EXIT_SUCCESS)
					remove(NEXT_MAC_CONF);
			}
		}else if(nRet == ERR_INPUT_VALIDATION_FAILED){
			goto returnHandler;
		}else{
			LOGF_LOG_INFO("Getting MAC Address from file is failed,new mac request\n");
		}
	}
	else
	{
		if(bGlobal == false)
		{
			if(pcMac[0] == '\0' || nMacModified == 1)
			{
				xIfr.ifr_hwaddr.sa_data[0]=2;	
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
						(unsigned char)xIfr.ifr_hwaddr.sa_data[0],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[1],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[2],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[3],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[4],
						(unsigned char)xIfr.ifr_hwaddr.sa_data[5]);
			}
		}
		else
		{
			if(pcMac[0] == '\0' || nMacModified == 1)
			{
				SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%s",sBaseMac);
			}
			//validate mac against limit
			nRet = scapi_validateLimit(pcMac,bGlobal);
			if(nRet == EDQUOTA)
			{
				fprintf(stderr, " [%s:%d] Generated mac not matching with reserved mac list or max limit reached %s\n",__func__,__LINE__,pcMac);
				nRet = ERR_MACADDRESS_POOL_EXHAUSTED;
				goto returnHandler;
			}
		}
		goto finish;
	}

	nRet = UGW_SUCCESS;
	//KL fix
	while(nCnt<1000)
	{
		memset(&st, 0, sizeof(st));

		if((pcMac[0] == '\0') || (nMacModified == 1) || (nRet == -MAC_IN_USE))
		{
			if (stat(NEXT_MAC_CONF, &st) == 0 && st.st_size != 0) {
				nRet = scapi_incrementMacaddr(&xIfr,bGlobal);

				if(nRet != EXIT_SUCCESS){
					fprintf(stderr, "[%s:%d] Next incrementMacAddr API failed \n", __func__,__LINE__); 
					goto returnHandler;
				}
			}

			/* Top layer should provide a NULL terminated array of size SCAPI_MAC_LEN */
			/* get result */
			SNPRINTF_GET_MAC(pcMac, SCAPI_MAC_LEN, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
					(unsigned char)xIfr.ifr_hwaddr.sa_data[0],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[1],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[2],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[3],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[4],
					(unsigned char)xIfr.ifr_hwaddr.sa_data[5]);

		}
		nRet = scapi_validateLimit(pcMac,bGlobal);

		if(nRet == EDQUOTA)
		{
			fprintf(stderr, " [%s:%d] Generated mac not matching with reserved mac list or max limit reached %s\n",__func__,__LINE__,pcMac);
			nRet = ERR_MACADDRESS_POOL_EXHAUSTED;
			goto returnHandler;
		}

		nRet  = scapi_validateMacaddr(pcIfname, pcMac);

		/* Validation successful */
		if( nRet == 0){
			break;
		}else if( nRet == -MAC_IN_USE) {
			nCnt++;
			continue;
		} else {
			fprintf(stderr,"Unable to validate the mac with interfaces ret=%d\n",nRet);
			goto returnHandler;
		}
	}
finish:
	/* This file is already created by scapi_getNextMacFromFile() API */
	fp = scapi_getFilePtr(NEXT_MAC_CONF, "a+");
	if(fp == NULL){
		nRet = ERR_MACADDRESS_DEFAULT_FILE_OPER_FAILED;
		fprintf(stderr,"[%s:%d] next_mac_conf file update failed reason:%s\n", __func__, __LINE__, strerror(errno));
		goto returnHandler;
	}
	fprintf(fp, "%s> %s\n", pcIfname, pcMac);

	nRet = UGW_SUCCESS;
	LOGF_LOG_INFO("Mac Generated [ %s ] : [%s ]\n",pcIfname, pcMac);
returnHandler:
	if(fp != NULL)
		fclose(fp);
	return nRet;
}
/* 
 ** =============================================================================
 **   Function Name    :scapi_isValidMacAddress
 **
 **   Description      :API to validate a Mac aaddress.
 **
 **   Parameters       :pcMac(IN) -> Mac Adsress 
 **
 **   Return Value     :Success -> EXIT_SUCCESS in case of validation success
 **                      Failure -> EXIT_FAILURE in case of validation fails 
 ** 
 ** ============================================================================
 */
int32_t scapi_isValidMacAddress(IN const char *pcMac)
{
	int32_t nRet = UGW_SUCCESS;
	regex_t xRegex = {0};
	int32_t nRegRet;
	char sExp[] = "^[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]$";
	char msgbuf[100];

	if (strnlen_s(pcMac,SCAPI_MAC_LEN) > 0) {
		/* Compile regular expression */
		if (regcomp(&xRegex, sExp, REG_ICASE)) {
			LOGF_LOG_ERROR("Could not compile xRegex\n");
			nRet = UGW_FAILURE;
			goto end;
		}
		/* Execute regular expression */
		nRegRet = regexec(&xRegex, pcMac, 0, NULL, 0);
		if (!nRegRet) {
			nRet = UGW_SUCCESS;
		} else if (nRegRet == REG_NOMATCH) {
			nRet = ERR_VALIDATION_FAILED;
		} else {
			regerror(nRegRet, &xRegex, msgbuf, sizeof(msgbuf));
			LOGF_LOG_ERROR("Regex match failed: %s\n", msgbuf);
			nRet = ERR_VALIDATION_FAILED;
		}
		/* Free compiled regular expression */
		regfree(&xRegex);
	}
end:
	return nRet;
}    

/* 
 ** ======================================================================================
 **   Function Name    :scapi_getReserevdMac
 **
 **   Description      :API to get reserved mac
 **
 **   Parameters       :pcMac(INOUT) -> Mac Address
 **
 **   Return Value     :Success -> mac address and EXIT_SUCCESS in case of successful fetch 
 **                      Failure -> EXIT_FAILURE in case of failure
 ** 
 ** =======================================================================================
 */
int scapi_getReserevdMac(INOUT ResvMAC **pxMac, INOUT int *nResCnt)
{
	int32_t nMacMaxCnt=0,nResMacCnt=0,nCnt=0,nRet=EXIT_SUCCESS;
	unsigned char mac_addr[MAC_ADDR_BYTES]={0};
	char *pcTmp=NULL,*pcSavePtr=NULL;
	char sBuf[SCAPI_MAC_LEN]={0};
	unsigned int nMacCurIdx=MAC_ADDR_BYTES-1,nPranetIdx=MAC_ADDR_BYTES-2;
	size_t len=0;

	if (scapi_getRunTimeVal("sup_mac", &nMacMaxCnt) != EXIT_SUCCESS)
	{
		LOGF_LOG_ERROR("Supported mac address value is not set\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}

	if (scapi_getRunTimeVal("res_mac", &nResMacCnt) != EXIT_SUCCESS)
	{
		LOGF_LOG_ERROR("Reserved mac address value is not set\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}

	*nResCnt = nResMacCnt;
	// verify against reserved mac

	sBuf[0]='\0'; 

	nRet = scapi_getBaseMac(sBuf);
	if(nRet != EXIT_SUCCESS)
	{
		LOGF_LOG_ERROR("scapi_getBaseMac(..) returned Failure\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}

	if((len=strnlen_s(sBuf,SCAPI_MAC_LEN)) <= 0){
		LOGF_LOG_ERROR("Fetching len failed\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}
	pcTmp=strtok_s(sBuf,&len, ":",&pcSavePtr);
	nCnt=0;
	while(pcTmp!=NULL)
	{
		sscanf_s (pcTmp,"%hhx",&(mac_addr[nCnt]));
		nCnt++;
		pcTmp=strtok_s(NULL,&len, ":",&pcSavePtr);
	}	

	nCnt = nMacMaxCnt-nResMacCnt;

	if(nCnt > MAX_CNT)
	{

		LOGF_LOG_ERROR("max limit count reached\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}

	while (nCnt>1)	
	{
		scapi_incMacAddr(mac_addr, &nMacCurIdx, &nPranetIdx, MAX_U8_VAL);
		nCnt--;
	}


	*pxMac=(ResvMAC *)malloc(nResMacCnt * sizeof(ResvMAC));
	if(*pxMac == NULL)
	{
		LOGF_LOG_ERROR("Memory alloc failed\n");
		nRet = -EXIT_FAILURE;
		goto returnHandler;
	}
	for(nCnt=0; nCnt<nResMacCnt; nCnt++)
	{
		scapi_incMacAddr(mac_addr, &nMacCurIdx, &nPranetIdx, MAX_U8_VAL);

		SNPRINTF_GET_MAC((*pxMac+nCnt)->pcMac,SCAPI_MAC_LEN, "%02x:%02x:%02x:%02x:%02x:%02x", 
				mac_addr[0], mac_addr[1], mac_addr[2], 
				mac_addr[3], mac_addr[4], mac_addr[5]);
	}
returnHandler:
	return nRet;
}
