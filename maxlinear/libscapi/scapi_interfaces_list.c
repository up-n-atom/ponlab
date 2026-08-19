/********************************************************************************

  Copyright (C) 2018 Intel Corporation
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ********************************************************************************/

/********************************************************************************
 *     File Name  : scapi_interfaces_list.c                                     *
 *     Project    : UGW                                                         *
 *     Description: includes Api to get the interfaces list                     *
 *                                                                              *
 ********************************************************************************/
#include "ulogging.h"
#include "scapi_interfaces_defines.h"

#define SNPRINTF_INTERFACE_REG(pDest, nBufSize, psFormat, ...) { \
        if(sprintf_s(pDest, nBufSize, psFormat, ##__VA_ARGS__) <=0 ) { \
                LOGF_LOG_ERROR( "Error : sprintf_s failed [%s]\n", strerror(errno)); \
                goto end; \
        } \
}


const char *gaDefaultConfigStr[e_configMAXEND] = {
	"false",
	"Static,DHCP, PPPoE,Bridge",
	"ETH",
	"0",
	"false",
	"false",
	"false",
	"eth1 nrgmii3",
};

/* =============================================================================
 * Function Name : scapi_createListNode
 * Description   : function to append an interface in the interface list. 
 * ============================================================================ */
IfaceCfg_t *scapi_createListNode(IfaceCfg_t * pxIfaceConfig, IfaceCfg_t * pxIfaceListHead)
{
	IfaceCfg_t *pxTmpNode = NULL;

	pxTmpNode = (IfaceCfg_t *) malloc(sizeof(IfaceCfg_t));
	if (pxTmpNode == NULL) {
		LOGF_LOG_ERROR("Malloc failed %d \n", ENOMEM);
		goto end;
	}
	SNPRINTF_INTERFACE_REG(pxTmpNode->cType, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cType);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cIfName, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cIfName);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cMacvLan, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cMacvLan);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cProtos, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cProtos);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cMode, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cMode);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cPort, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cPort);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cEnable, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cEnable);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cLinkEnable, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cLinkEnable);
	SNPRINTF_INTERFACE_REG(pxTmpNode->cBondMembers, MAX_LEN_PARAM_VALUE_D, "%s", pxIfaceConfig->cBondMembers);

	LOGF_LOG_DEBUG("Interface: [%s] Enable [%s] Type:[%s] MacvLan [%s] Proto [%s] Mode [%s] Port [%s] LinkEnable [%s] BondMembers [%s]", pxTmpNode->cIfName, pxTmpNode->cEnable, pxTmpNode->cType,
		       pxTmpNode->cMacvLan, pxTmpNode->cProtos, pxTmpNode->cMode, pxTmpNode->cPort, pxTmpNode->cLinkEnable, pxTmpNode->cBondMembers);
	list_add_tail((&pxTmpNode->xIfaceList), &pxIfaceListHead->xIfaceList);
 end:
	LOGF_LOG_DEBUG("%s return.. \n", __func__);
	return pxTmpNode;
}

/* =============================================================================
 * Function Name : scapi_deleteInterfaceList
 * Description   : function to delete the interface list. 
 * ============================================================================ */
void scapi_deleteInterfaceList(IfaceCfg_t ** pxIfaceListHead)
{
	IfaceCfg_t *pxElem = NULL, *pxTempElem = NULL;
	IfaceCfg_t *pxIfaceList = NULL;

	if (pxIfaceListHead == NULL)
		return;
	else
		pxIfaceList = *pxIfaceListHead;

	/*Iterate over the list */
	list_for_each_entry_safe(pxElem, pxTempElem, &pxIfaceList->xIfaceList, xIfaceList) {
		list_del(&pxElem->xIfaceList);
		free(pxElem);
		pxElem = NULL;
	}
	free(*pxIfaceListHead);
	*pxIfaceListHead = NULL;

}

/* =============================================================================
 * Function Name : scapi_validateParameterList
 * Description   : Validates the interface param,
 *                    if validation fails fill the default params. 
 * ============================================================================ */
void scapi_validateParameterList(E_ParameterType e_type, char *dataString)
{
	switch (e_type) {
	case e_configMacvLan:
		if (strcasecmp(dataString, "true") != 0) {
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", gaDefaultConfigStr[e_configMacvLan]);
		}
		break;
	case e_configProtos:
		if ((strstr(dataString, "Static") != NULL) || (strstr(dataString, "DHCP") != NULL) || (strstr(dataString, "PPPoE") != NULL)) {
			return;
		} else {
			LOGF_LOG_WARNING("Reading interface Protos : [%s]", dataString);
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", gaDefaultConfigStr[e_configProtos]);
		}
		break;
	case e_configMode:
		break;
	case e_configPort:
		if (atoi(dataString) <= 0) {
			LOGF_LOG_ERROR("Reading interface Port : [%d]", atoi(dataString));
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", gaDefaultConfigStr[e_configPort]);
		}
		break;
	case e_configEnable:
	case e_configLinkEnable:
		if (strcasecmp(dataString, "true") != 0) {
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", "false");
		}
		break;
	case e_configBond:
		if (strcasecmp(dataString, "true") != 0) {
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", "false");
		}
		break;
	case e_configBondMembers:
		if (dataString[0] == '\0') {
			SNPRINTF_INTERFACE_REG(dataString, 50, "%s", gaDefaultConfigStr[e_configBondMembers]);
		}
		break;
	default:
		break;
	}
end:
	LOGF_LOG_DEBUG("%s return.. \n", __func__);
}

/* =============================================================================
 * Function Name : scapi_fillDefaultStruct
 * Description   : fill the default paramters if validation fails. 
 * ============================================================================ */
int scapi_fillDefaultStruct(IfaceCfg_t * pxIfaceConfig)
{
	if (!pxIfaceConfig)
		return UGW_FAILURE;

	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cMacvLan, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configMacvLan]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cProtos, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configProtos]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cMode, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configMode]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cPort, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configPort]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cEnable, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configEnable]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cLinkEnable, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configLinkEnable]);
	SNPRINTF_INTERFACE_REG(pxIfaceConfig->cBondMembers, MAX_LEN_PARAM_VALUE_D, "%s", gaDefaultConfigStr[e_configBondMembers]);
end:
	LOGF_LOG_DEBUG("%s return.. \n", __func__);
	return UGW_SUCCESS;
}

/* =============================================================================
 * Function Name : scapi_getInterfaceList
 * Description   : Get the list of interfaces available. 
 * ============================================================================ */
int32_t scapi_getInterfaceList(IfaceCfg_t ** xIfaceCfgListHead, uint32_t flags, char *pcIfName)
{
	/*parse c header file, add nodes in Iface linked list */
	FILE *filePtr = NULL;
	int retValue = UGW_SUCCESS;
	char readLine[100] = { 0 };
	char readLine_temp[100] = { 0 };
	char valueString[50] = { 0 };
	char *equalCharFound = NULL;
	char *hashCharFound = NULL;
	int nIter = 0;
	int nFound = 0;
	int bNameMatch = 0;

	struct updateflags {
		uint32_t nTypeUpdate:1;
		uint32_t nIfaceNameUpdate:1;
		uint32_t nMacvLanUpdate:1;
		uint32_t nProtosUpdate:1;
		uint32_t nModeUpdate:1;
		uint32_t nSwitchPortUpdate:1;
		uint32_t nEnableUpdate:1;
		uint32_t nLinkEnableUpdate:1;
		uint32_t nBondMembersUpdate:1;
	} xUpdateFlag = {
	0};

	/*opening the file for parsing. */
	*xIfaceCfgListHead = (IfaceCfg_t *) malloc(sizeof(IfaceCfg_t));
	if (*xIfaceCfgListHead == NULL) {
		LOGF_LOG_ERROR("Malloc failed %d \n", ENOMEM);
		retValue = -ENOMEM;
		goto end;
	};
	INIT_LIST_HEAD(&(*xIfaceCfgListHead)->xIfaceList);

	IfaceCfg_t xIfaceConfig={0};
	scapi_fillDefaultStruct(&xIfaceConfig);

	filePtr = scapi_getFilePtr(CONFIG_FILE_INTERFACES, "r");
	if (filePtr == NULL) {
		/*if file is not present the create default configuration */
		LOGF_LOG_ERROR("Failed to open interfaces.cfg\n");
		retValue = UGW_FAILURE;
		return retValue;
	}
	/*reading the file and parsing lines. */
	while (fgets(readLine, sizeof(readLine), filePtr) != NULL) {
		if (strstr(readLine, "{") != NULL) {
			while (fgets(readLine, sizeof(readLine), filePtr) != NULL) {
				if (strstr(readLine, "}") != NULL) {
					break;
				}
				nIter = nFound = 0;
				while (readLine[nIter] != '\n') {
					readLine_temp[nIter - nFound] = readLine[nIter];
					if (readLine[nIter] == '\t' || readLine[nIter] == '\r' || readLine[nIter] == '\v') {
						nFound++;
					}
					nIter++;
				}
				readLine_temp[nIter - nFound] = '\0';
				SNPRINTF_INTERFACE_REG(readLine, 100, "%s", readLine_temp);
				memset(readLine_temp, 0, sizeof(readLine_temp));
				if ((equalCharFound = strstr(readLine, "=")) != NULL) {
					memset(valueString, 0x0, sizeof(valueString));
					hashCharFound = strstr(readLine, "#");
					if ((hashCharFound != NULL) && (hashCharFound < equalCharFound)) {
						continue;
					}
					if ((hashCharFound - equalCharFound) == sizeof(char)) {
						continue;
					}
					
					if(strnlen_s(readLine,100)<1)
						goto end;
						
					
					if (strncmp(readLine, CONFIG_FILE_TYPE, equalCharFound - readLine) == 0) {
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						if (valueString[0] != '\0') {
							if ((strcasecmp(valueString, "WAN") == 0) || (strcasecmp(valueString, "LAN") == 0)) {
								SNPRINTF_INTERFACE_REG(xIfaceConfig.cType, sizeof(xIfaceConfig.cType), "%s", valueString);
								xUpdateFlag.nTypeUpdate = 1;

							} else {
								LOGF_LOG_ERROR("Reading interface type:[%s] skip", valueString);
								xUpdateFlag.nTypeUpdate = 0;

							}
						} else {
							LOGF_LOG_ERROR("Empty Interface Type: Skip");
							xUpdateFlag.nTypeUpdate = 0;
						}

					} else if (strncmp(readLine, CONFIG_FILE_IFNAME, equalCharFound - readLine) == 0) {
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						if (valueString[0] != '\0') {
							xUpdateFlag.nIfaceNameUpdate = 1;
							SNPRINTF_INTERFACE_REG(xIfaceConfig.cIfName, sizeof(xIfaceConfig.cIfName), "%s", valueString);
							if((pcIfName != NULL) && (strncmp(valueString, pcIfName, strnlen_s(pcIfName,1024) + 1) == 0)) {
								bNameMatch = 1;
							}
						} else {
							LOGF_LOG_ERROR("Empty Interface Name: Skip");
							xUpdateFlag.nIfaceNameUpdate = 0;
						}
					} else if (strncmp(readLine, CONFIG_FILE_MACVLAN, equalCharFound - readLine) == 0) {
						xUpdateFlag.nMacvLanUpdate = 1;
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						scapi_validateParameterList(e_configMacvLan, valueString);
						SNPRINTF_INTERFACE_REG(xIfaceConfig.cMacvLan, sizeof(xIfaceConfig.cMacvLan), "%s", valueString);
					} else if (strncmp(readLine, CONFIG_FILE_PROTOS, equalCharFound - readLine) == 0) {
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						if (valueString[0] != '\0') {
							xUpdateFlag.nProtosUpdate = 1;
							scapi_validateParameterList(e_configProtos, valueString);
							SNPRINTF_INTERFACE_REG(xIfaceConfig.cProtos, sizeof(xIfaceConfig.cProtos), "%s", valueString);
						} else {
							LOGF_LOG_WARNING("Empty Interface Protos");
							xUpdateFlag.nProtosUpdate = 0;
						}
					} else if (strncmp(readLine, CONFIG_FILE_MODE, equalCharFound - readLine) == 0) {
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						if (valueString[0] != '\0') {
							xUpdateFlag.nModeUpdate = 1;

							scapi_validateParameterList(e_configMode, valueString);
							SNPRINTF_INTERFACE_REG(xIfaceConfig.cMode, sizeof(xIfaceConfig.cMode),"%s", valueString);
						} else {
							LOGF_LOG_ERROR("Empty Interface Mode.");
							xUpdateFlag.nModeUpdate = 0;
						}
					} else if (strncmp(readLine, CONFIG_FILE_PORT, equalCharFound - readLine) == 0) {
						xUpdateFlag.nSwitchPortUpdate = 1;
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						if (valueString[0] != '\0') {
							xUpdateFlag.nSwitchPortUpdate = 1;
							scapi_validateParameterList(e_configPort, valueString);
							SNPRINTF_INTERFACE_REG(xIfaceConfig.cPort, sizeof(xIfaceConfig.cPort), "%s", valueString);
						} else {
							LOGF_LOG_ERROR("Empty Interface Port");
							xUpdateFlag.nSwitchPortUpdate = 0;
						}
					} else if (strncmp(readLine, CONFIG_FILE_ENABLE, equalCharFound - readLine) == 0) {
						xUpdateFlag.nEnableUpdate = 1;
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						scapi_validateParameterList(e_configEnable, valueString);
						SNPRINTF_INTERFACE_REG(xIfaceConfig.cEnable, sizeof(xIfaceConfig.cEnable),"%s", valueString);
					} else if (strncmp(readLine, CONFIG_FILE_LINKENABLE, equalCharFound - readLine) == 0) {
						xUpdateFlag.nLinkEnableUpdate = 1;
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						scapi_validateParameterList(e_configLinkEnable, valueString);
						SNPRINTF_INTERFACE_REG(xIfaceConfig.cLinkEnable, sizeof(xIfaceConfig.cLinkEnable),"%s", valueString);
					} else if (strncmp(readLine, CONFIG_FILE_BONDMEMBERS, equalCharFound - readLine) == 0) {
						xUpdateFlag.nBondMembersUpdate = 1;
						if (hashCharFound > equalCharFound) {
							SNPRINTF_INTERFACE_REG(valueString, hashCharFound - equalCharFound, "%s", equalCharFound + 1);
						} else {
							strncpy_s(valueString, sizeof(valueString), equalCharFound + 1, sizeof(valueString));
						}
						scapi_validateParameterList(e_configBondMembers, valueString);
						SNPRINTF_INTERFACE_REG(xIfaceConfig.cBondMembers, sizeof(xIfaceConfig.cBondMembers), "%s", valueString);
					}
				}
			}

			if ((pcIfName == NULL && ((flags == ONLY_LAN_INTERFACE && (strcmp(xIfaceConfig.cType, "LAN") == 0)) || (flags == ONLY_WAN_INTERFACE && (strcmp(xIfaceConfig.cType, "WAN") == 0))
			    || (flags == BOTH_INTERFACES))) || ((pcIfName != NULL) && bNameMatch)) {
				/*if interface name is present then only add in the list */
				if (xUpdateFlag.nIfaceNameUpdate == 1 && xUpdateFlag.nTypeUpdate == 1) {
					if (scapi_createListNode(&xIfaceConfig, *xIfaceCfgListHead) == NULL) {
						LOGF_LOG_ERROR("Failed to create node\n");
						retValue = UGW_FAILURE;
						goto end;
					}
				}
			}
			scapi_fillDefaultStruct(&xIfaceConfig);
		}
		if(bNameMatch) {
			break;
		}
	}

 end:
	/*close the file if reading is finished. */
	LOGF_LOG_DEBUG("%s return.. ret - %d \n", __func__, retValue);
	if (filePtr != NULL)
		fclose(filePtr);
	return retValue;
}
