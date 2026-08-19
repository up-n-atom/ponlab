/********************************************************************************

  Copyright (c) 2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ********************************************************************************/

/***************************************************************************** *
 *     File Name  : scapi_deviceInfo.c                                         *
 *     Project    : UGW                                                        *
 *     Description: cal - get APIs Device Info Params                          *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ulogging.h>
#include <ltq_api_include.h>

#define PROC_CPUINFO "/proc/cpuinfo"
#define MODEL_NAME_STR "CONFIG_IFX_MODEL_NAME"
#define MODELNAME_CONFIG_FILE_PATH "/etc/config.sh"
#define MACHINE "machine"
#define SYSTEM_TYPE "system type"
#define PPMD_STATUS_INFO "/tmp/ppmd/ppmd_status"
#define ADP_COLLING "ADP cooling:"
#define ADP_DVFS_THRESHOLDS "ADP DVFS thresholds:"
#define ADP_DVFS_LEVEL "ADP DVFS level:"
#define AGGREGATED_LINK_SPEED "Aggregated total link speed:"
#define POWER "P[mW]:"
#define VOLTAGE "V[mV]:"
#define PM_UTIL_STATUS_INFO "/tmp/pm_util_status"

#define SNPRINTF_DEVINFO(pDest, nBufSize, psFormat, ...) { \
        if((sprintf_s(pDest, nBufSize, psFormat, ##__VA_ARGS__)) <=0 ) { \
                LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); \
                nRet = -EXIT_FAILURE; \
		goto end; \
        } \
}

/* =============================================================================
 * Function Name : scapi_getPowerMonitorStatus
 * Description   : function to get the pm_util status info.
 * ============================================================================ */
int32_t scapi_getPowerMonitorStatus(PowerMonitorInfo_t *pxInfo)
{
	FILE* fp = NULL;
	char *line = NULL;
	char* st = NULL;
	size_t len = 0;
	uint32_t len1=0;
	ssize_t read;
	char *saveptr = NULL;
	size_t unLen = 0;
	int nRet = UGW_SUCCESS;

	fp = scapi_getFilePtr(PM_UTIL_STATUS_INFO, "r");
	if (NULL == fp) {
		LOGF_LOG_CRITICAL("Error: Failed to open %s", PM_UTIL_STATUS_INFO);
		nRet = errno;
		goto end;
	}
	if ((read = getline(&line, &len, fp)) != -1) {
		if ((len1=strnlen_s(line,MAX_LEN_LINE))<=0) {
			fprintf(stderr, "Fetching len failed\n");
			nRet = errno;
			goto end;
		}
		if (*(line + (len1 - 1)) == '\n') {
			*(line + (len1 - 1)) = '\0';
		}
		if ((st = strstr(line, VOLTAGE)) != NULL) {
			st = st + sizeof(VOLTAGE);
			pxInfo->uVoltage = atoi(st);
		}
		if ((st = strstr(line, POWER)) != NULL) {
			st = st + sizeof(POWER);
			unLen = strnlen_s(st, MAX_LEN_LINE);
			st = strtok_s(st, &unLen, " ", &saveptr);
			if (st != NULL)
				pxInfo->uPower = atoi(st);
		}
	}
end:
	if (line)
		free(line);
	if (fp != NULL)
		pclose(fp);
	return nRet;
}

/* =============================================================================
 * Function Name : scapi_getCPUStatus
 * Description   : function to get the CPU status info.
 * ============================================================================ */
int32_t scapi_getCPUStatus(char *sFileName, char *sValue)
{
	FILE* fp = NULL;
	int nRet = UGW_SUCCESS;
	char *line = NULL;
	size_t len = 0;
	uint32_t strLen = 0;
	ssize_t read;

	fp = scapi_getFilePtr(sFileName, "r");
	if (NULL == fp) {
		LOGF_LOG_CRITICAL("Error: Failed to open %s", sFileName);
		nRet = errno;
		goto end;
	}
	if ((read = getline(&line, &len, fp)) != -1) {
		if ((strLen = strnlen_s(line, len)) <= 0) {
			fprintf(stderr, "Fetching len failed\n");
			nRet = errno;
			goto end;
		}
		if (*(line + (strLen - 1)) == '\n')
			*(line + (strLen - 1)) = '\0';
		SNPRINTF_DEVINFO(sValue, MAX_LEN_PARAM_VAL, "%s", line);
	}
end:
	if (line)
		free(line);
	if (fp)
		fclose(fp);
	return nRet;
}

/* =============================================================================
 * Function Name : scapi_getPPMDStatus
 * Description   : function to get the PPMD status.
 * ============================================================================ */
int32_t scapi_getPpmdStatus(PPMDInfo_t *pxInfo)
{
	FILE* fp = NULL;
	char *line = NULL;
	char* st = NULL;
	size_t len = 0;
	uint32_t len1=0;
	ssize_t read;
	int nRet = UGW_SUCCESS;

	fp = scapi_getFilePtr(PPMD_STATUS_INFO, "r");
	if (NULL == fp) {
		LOGF_LOG_CRITICAL("Error: Failed to open %s", PROC_CPUINFO);
		nRet = errno;
		goto end;
	}
	while ((read = getline(&line, &len, fp)) != -1) {
		if ((len1=strnlen_s(line,MAX_LEN_LINE))<=0) {
			fprintf(stderr, "Fetching len failed\n");
			nRet = errno;
			if (line)
				free(line);
			goto end;
		}
		if (*(line + (len1 - 1)) == '\n')
			*(line + (len1 - 1)) = '\0';
		if ((st = strstr(line, ADP_COLLING)) != NULL) {
			SNPRINTF_DEVINFO(pxInfo->sADPCooling, MAX_LEN_PARAM_VAL, "%s", st + sizeof(ADP_COLLING));
		} else if ((st = strstr(line, ADP_DVFS_THRESHOLDS)) != NULL) {
			SNPRINTF_DEVINFO(pxInfo->sADP_DVFS_Thresholds, MAX_LEN_PARAM_VAL, "%s", st + sizeof(ADP_DVFS_THRESHOLDS));
		} else if ((st = strstr(line, ADP_DVFS_LEVEL)) != NULL) {
			SNPRINTF_DEVINFO(pxInfo->sADP_DVFS_Level, MAX_LEN_PARAM_VAL, "%s", st + sizeof(ADP_DVFS_LEVEL));
		} else if ((st = strstr(line, AGGREGATED_LINK_SPEED)) != NULL) {
			SNPRINTF_DEVINFO(pxInfo->sAggregated_Total_Link_Speed, MAX_LEN_PARAM_VAL, "%s", st + sizeof(AGGREGATED_LINK_SPEED));
		}
	}
	if (line)
		free(line);
end:
	if (fp != NULL)
		pclose(fp);
	return nRet;
}

/* =============================================================================
 * Function Name : scapi_getHardwareVersion
 * Description   : function to get the Hardware Version. 
 * ============================================================================ */
int32_t scapi_getHardwareVersion(OUT char *pcHwVer)
{
	FILE *pFp = NULL;
	int32_t nRet = UGW_SUCCESS;
	char sBuf[MAX_LEN_PARAM_VALUE_D] = { 0 };
	char sHwType[MAX_LEN_PARAM_VALUE_D] = { 0 };
	char sHwName[MAX_LEN_PARAM_VALUE_D] = { 0 };
	pFp = scapi_getFilePtr(PROC_CPUINFO, "r");
	if (NULL == pFp) {
		LOGF_LOG_CRITICAL("Error: Failed to open %s", PROC_CPUINFO);
		nRet = errno;
		goto end;
	}

	while (NULL != fgets(sBuf, sizeof(sBuf), pFp)) {
		sBuf[strcspn(sBuf, "\r\n")] = 0;
		if (strstr(sBuf, SYSTEM_TYPE) != NULL) {
			SNPRINTF_DEVINFO(sHwName, MAX_LEN_PARAM_VALUE_D, "%s" , strchr(sBuf, ':') + 2);
		}
		if (strstr(sBuf, MACHINE) != NULL) {
			SNPRINTF_DEVINFO(sHwType, MAX_LEN_PARAM_VALUE_D, "%s", strchr(sBuf, ':') + 1);
		}
	}
	SNPRINTF_DEVINFO(pcHwVer, MAX_LEN_PARAM_VALUE_D, "%s,%s", sHwName, sHwType);

 end:
	if (pFp != NULL) {
		fclose(pFp);
		pFp = NULL;
	}
	return nRet;
}

/* =============================================================================
 * Function Name : scapi_getModelName
 * Description   : function to get the ModelName. 
 * ============================================================================ */
int32_t scapi_getModelName(OUT char *pcModelName)
{
	FILE *pFp = NULL;
	int32_t nRet = UGW_SUCCESS;
	char sBuf[MAX_LEN_PARAM_VALUE_D] = { 0 };
	pFp = scapi_getFilePtr(MODELNAME_CONFIG_FILE_PATH, "r");
	if (NULL == pFp) {
		LOGF_LOG_CRITICAL("Error: Failed to open %s", MODELNAME_CONFIG_FILE_PATH);
		nRet = errno;
		goto end;
	}

	while (NULL != fgets(sBuf, sizeof(sBuf), pFp)) {
		sBuf[strcspn(sBuf, "\r\n")] = 0;
		if (strstr(sBuf, MODEL_NAME_STR) != NULL) {
			SNPRINTF_DEVINFO(pcModelName, strrchr(sBuf, '"') - strchr(sBuf, '"') , "%s", strchr(sBuf, '"') + 1);
			break;
		}
	}

 end:
	if (pFp != NULL) {
		fclose(pFp);
		pFp = NULL;
	}
	return nRet;
}
