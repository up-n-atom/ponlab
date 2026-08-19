/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include <stdio.h>
#include <unistd.h> // getopt
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "commondefs.h"
#include "qosal_debug.h"
#include "qosal_utils.h"
#include "qosal_cl_api.h"
#include "cli_classcfg.h"
#include "fapi_high.h"

#include "safe_str_lib.h"
#include "safe_mem_lib.h"

extern uint16_t LOGLEVEL, LOGTYPE;
//void freeSubStr(char **subStr, int32_t nSubCnt);
int isValidMacAddress(const char* macAddr);
int32_t ipaddr_conv(char *sIp, struct in_addr *addr);
int32_t ip6addr_conv(char *sIp, struct in6_addr *addr);
int32_t ipmask_conv(char *sMask, struct in_addr *mask);
int32_t ip6mask_conv(char *sMask, struct in6_addr *mask);
int32_t is_exclude(char *sBuf);
extern int find_error_message(IN int respCode, OUT char *sErrMsg);


#if 0
void freeSubStr(char **subStr, int32_t nSubCnt)
{
	while(--nSubCnt >= 0) {
		if((subStr + nSubCnt) != NULL) {
			free(subStr + nSubCnt);
		}
	}
}
#endif

int isValidMacAddress(const char* macAddr)
{
    int nDigit = 0, nSeparator = 0;

    while (*macAddr) {
       if (isxdigit(*macAddr)) {
          nDigit++;
       } else if (*macAddr == ':' || *macAddr == '-') {
			if (nDigit == 0 || ((nDigit/2 - 1) != nSeparator)) {
				break;
			}
			++nSeparator;
		} else {
			nSeparator = -1;
		}
		++macAddr;
	}
	return (nDigit == 12 && ((nSeparator == 5) || (nSeparator == 0)));
}

int32_t ipaddr_conv(char *sIp, struct in_addr *addrout)
{
  unsigned char *addrp = NULL;
  struct in_addr addr;
  char subStr[5][64] = {0};
  char *ptr = NULL;
  int nSubCnt = 0, nCnt = 0;

  memset_s(&addr, sizeof(addr), 0);
  addrp = (void *)&addr.s_addr;
  splitStr(sIp, ".", subStr, &nSubCnt);
  if(nSubCnt < 4) {
    printf("Ip address is invalid %s\n", sIp);
    //freeSubStr(subStr, nSubCnt);
    return -1;
  }
  while(nCnt < nSubCnt) {
    addrp[nCnt] = strtoul(subStr[nCnt], &ptr, 10);
    nCnt++;
  }
  memcpy_s(addrout, sizeof(struct in_addr), addrp, nCnt);
  //freeSubStr(subStr, nSubCnt);
  return 0;
}

int32_t ip6addr_conv(char *sIp, struct in6_addr *addr)
{
  if(inet_pton(AF_INET6, sIp, addr) == 1)
    return 0;
  return -1;
}

int32_t ipmask_conv(char *sMask, struct in_addr *mask)
{
  if(inet_pton(AF_INET, sMask, mask) == 1)
    return 0;
  return -1;
}

int32_t ip6mask_conv(char *sMask, struct in6_addr *mask)
{
  if(inet_pton(AF_INET6, sMask, mask) == 1)
    return 0;
  return -1;
}

int32_t is_exclude(char *sBuf)
{
  return (strchr(sBuf, '!') != NULL)?1:0;
}

name_value_t axprototype[]={
  {"icmp",CLI_PROTO_ICMP},
  {"tcp",CLI_PROTO_TCP},
  {"udp",CLI_PROTO_UDP},
  {"all",CLI_PROTO_ALL},
  {"\0",0}
};

name_value_t axtcpflags[]={
  {"ack",CLI_TCP_ACK},
  {"syn",CLI_TCP_SYN},
  {"fin",CLI_TCP_FIN},
  {"rst",CLI_TCP_RST},
  {"urg",CLI_TCP_URG},
  {"\0",0}
};

name_value_t axfilteraction[]={
  {"mark",CLI_ACTION_MARK},
  {"drop",CLI_ACTION_DROP},
  {"dscp",CLI_ACTION_DSCP},
  {"vlan",CLI_ACTION_VLAN},
  {"policer",CLI_ACTION_POLICER},
  {"policyrt",CLI_ACTION_POLICYRT},
  {"accl",CLI_ACTION_ACCL},
  {"frag",CLI_ACTION_FRAG},
  {"\0",0}
};

name_value_t axdhcpmode[]={
  {"exact",CLI_DHCP_EXACT},
  {"prefix",CLI_DHCP_PREFIX},
  {"suffix",CLI_DHCP_SUFFIX},
  {"substr",CLI_DHCP_SUBSTR},
  {"\0",0}
};

name_value_t axmatchmode[]={
  {"app",CLI_MATCH_APP},
  {"layer7",CLI_MATCH_L7},
  {"dhcp",CLI_MATCH_DHCP},
  {"\0",0}
};

name_value_t axappname[]={
  {"app1",CLI_APP_APP1},
  {"app2",CLI_APP_APP2},
  {"app1!",CLI_APP_APP1},
  {"app2!",CLI_APP_APP2},
  {"\0",0}
};

name_value_t axsubprotoname[]={
  {"sub1",CLI_APP_APP1},
  {"sub2",CLI_APP_APP2},
  {"sub1!",CLI_APP_APP1},
  {"sub2!",CLI_APP_APP2},
  {"\0",0}
};

static struct option long_options[] = {
  {"vendor", required_argument, 0, 0},
  {"mode", required_argument, 0, 0},
  {"v6vendor", required_argument, 0, 0},
  {"client", required_argument, 0, 0},
  {"class", required_argument, 0, 0},
  {"vinfo", required_argument, 0, 0},
  {"smac", required_argument, 0, 0},
  {"dmac", required_argument, 0, 0},
  {"802_3-ssap", required_argument, 0, 0},
  {"802_3-dsap", required_argument, 0, 0},
  {"vlan-id-inner", required_argument, 0, 0},
  {"vlan-id-outer", required_argument, 0, 0},
  {"vlan-prio-inner", required_argument, 0, 0},
  {"vlan-prio-outer", required_argument, 0, 0},
  {"vlan-dei-inner", required_argument, 0, 0},
  {"vlan-dei-outer", required_argument, 0, 0},
  {"llc", required_argument, 0, 0},
  {"snapoui", required_argument, 0, 0},
  {"policer", required_argument, 0, 0},
  {"cr", required_argument, 0, 0},
  {"cbs", required_argument, 0, 0},
  {"pr", required_argument, 0, 0},
  {"pbs", required_argument, 0, 0},
  {"ebr", required_argument, 0, 0},
  {"actdrop", required_argument, 0, 0},
  {"actdscp", required_argument, 0, 0},
  {"actethprio", required_argument, 0, 0},
  {"mark", required_argument, 0, 0},
  {"mark_ing", required_argument, 0, 0},
  {"priority", required_argument, 0, 0},
  {"mark-loc", required_argument, 0, 0},
  {"actaccl", required_argument, 0, 0},
  {"actfrag", required_argument, 0, 0},
  {"vid-inner", required_argument, 0, 0},
  {"vid-outer", required_argument, 0, 0},
  {"pcp-inner", required_argument, 0, 0},
  {"pcp-outer", required_argument, 0, 0},
  {"policyrt", required_argument, 0, 0},
  {"gw", required_argument, 0, 0},
  {"iface", required_argument, 0, 0},
  {"metric", required_argument, 0, 0},
  {"app-name", required_argument, 0, 0},
  {"l7proto", required_argument, 0, 0},
  {"subproto-name", required_argument, 0, 0},
  {"src-ip-inner", required_argument, 0, 0},
  {"dst-ip-inner", required_argument, 0, 0},
  {"eth-type", required_argument, 0, 0},
  {"iphdr-len-min", required_argument, 0, 0},
  {"iphdr-len-max", required_argument, 0, 0},
  {"iphdr-dscp", required_argument, 0, 0},
  {"cl-eng", required_argument, 0, 0},
  /* Not sub commads */ 
  {"dport", required_argument, 0, 'T'},
  {"sport", required_argument, 0, 'S'},
  {"tcp-flags", required_argument, 0, 'F'},
  {0,0,0,0}
};

char actmp[QCLI_MAX_STR];

char* classcfg_getoptions(name_value_t *pxnv);
int32_t classcfg_getval_fromname(const char *pcname,name_value_t *pxnv);
void 
classcfg_parse_portrange( const char *optstring, const char* optarg, cl_port_match_t *pxportmatch);
void 
classcfg_parse_macaddr_macmask( const char *optstring, const char* optarg, cl_macaddr_match_t *pxmacmatch);
void 
classcfg_parse_ipaddr_ipmask( const char *optstring, const char* optarg, cl_ipaddr_match_t *pxipmatch);
void
classcfg_set_param_in_ex(int32_t exclude_req, int32_t param , cl_param_in_ex_t *pxparaminex);
int32_t
classcfg_handle_subcmd(int32_t index, const char* optarg, int32_t exclude, qos_class_cfg_t *pxclscfg);
void classcfg_print_usage(void);
int32_t
classcfg_show_classifiers(uint32_t order, char *sIface, int32_t num_entries, cl_match_filter_t *filter, cl_action_t *action);
int32_t
classcfg_invoke_fapi(qos_class_cfg_t *pxclscfg);
static int32_t
classcfg_is_police_supported(qos_class_cfg_t xclscfg);
void classcfg_print_policer_usage(void);

static int32_t classcfg_is_police_supported(qos_class_cfg_t xclscfg)
{
if (xclscfg.filter.param_in_ex.param_in & CLSCFG_PARAM_INNER_IP
		|| xclscfg.filter.param_in_ex.param_in & CLSCFG_PARAM_TX_IF
		|| xclscfg.filter.param_in_ex.param_in & CLSCFG_PARAM_TCP_HDR
		|| xclscfg.filter.param_in_ex.param_in & CLSCFG_PARAM_L7_PROTO
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_SSAP
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_DSAP
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_VLANID
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_INNER_VLANID
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_PRIO
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_INNER_PRIO
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_VLAN_DEI
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_INNER_VLAN_DEI
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_LLC_CONTROL
		|| xclscfg.filter.ethhdr.param_in_ex.param_in & CLSCFG_PARAM_ETH_HDR_OUI
		|| xclscfg.filter.iphdr.param_in_ex.param_in & CLSCFG_PARAM_IP_HDR_LEN_MIN
		|| xclscfg.filter.iphdr.param_in_ex.param_in & CLSCFG_PARAM_IP_HDR_LEN_MAX

		|| xclscfg.filter.param_in_ex.param_ex & CLSCFG_PARAM_SRC_MAC
		|| xclscfg.filter.param_in_ex.param_ex & CLSCFG_PARAM_DST_MAC
		|| xclscfg.filter.param_in_ex.param_ex & CLSCFG_PARAM_OUTER_IP
		|| xclscfg.filter.outer_ip.param_in_ex.param_ex & CLSCFG_PARAM_OUTER_IP_PROTO
		|| xclscfg.filter.outer_ip.param_in_ex.param_ex & CLSCFG_PARAM_OUTER_IP_SRC_PORT
		|| xclscfg.filter.outer_ip.param_in_ex.param_ex & CLSCFG_PARAM_OUTER_IP_DST_PORT
		|| xclscfg.filter.iphdr.param_in_ex.param_ex & CLSCFG_PARAM_IP_HDR_DSCP ) {
		printf("some/all of the configured filters are not supported with policer action\n\n");
		return LTQ_FAIL;
	}
	return LTQ_SUCCESS;
}

char* classcfg_getoptions(name_value_t *pxnv)
{
	name_value_t *pxtmp=pxnv;
	int32_t ilen=0;
	while(pxtmp&&pxtmp->name[0]){
		ilen += snprintf(actmp+ilen,QCLI_MAX_STR,"%s/",pxtmp->name);
		pxtmp++;
	}
	return actmp;
}

int32_t classcfg_getval_fromname(const char *pcname,name_value_t *pxnv)
{
	name_value_t *pxtmp=pxnv;
	while(pxtmp && strnlen_s(pxtmp->name, QCLI_MAX_STR)){
		//LOGF_LOG_DEBUG("[%s]==[%s]\n",pxtmp->name,pcname);
		if(strcasecmp(pxtmp->name,pcname)==0){
			//LOGF_LOG_DEBUG("Returning Val: %d\n",pxtmp->value);
			return pxtmp->value;
		}
		pxtmp++;
	}
	return -1;
}

void classcfg_parse_portrange( const char *optstring, const char* optarg, cl_port_match_t *pxportmatch)
{
	char option_string[256];
	uint32_t start_port = 0, end_port = 0;
	char subStr[3][64] = {0};
	int32_t nSubCnt = 0;
	//copy to the local string
	if(strncpy_s(option_string, sizeof(option_string), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
	{
		printf("Error copying to local string !!\n");
		exit(0);
	}

	splitStr(option_string, ":", subStr, &nSubCnt);
	switch(nSubCnt) {
		case 1:
			start_port = atoi(subStr[0]);
			end_port = 0;
			break;
		case 2:
			start_port = atoi(subStr[0]);
			end_port = atoi(subStr[1]);
			break;
		default:
			printf("Wrong number of parameter's passed to option %s \n",optstring);
			exit(0);
	}
	//freeSubStr(subStr, nSubCnt);

	if( (start_port > 0 && start_port <= 65535) && (end_port > 0 && end_port <= 65535 )) {
		pxportmatch->start_port = start_port;
		pxportmatch->end_port = end_port;
	} else {
		printf("***** Invalid port range values %s to %s\n",optarg, optstring);
		exit(0);
	}
}

void classcfg_parse_macaddr_macmask( const char *optstring, const char* optarg, cl_macaddr_match_t *pxmacmatch)
{
	char option_string[256];
	char subStr[3][64] = {0};
	int32_t nSubCnt = 0;
	//copy to the local string
	if(strncpy_s(option_string, sizeof(option_string), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying to local string !!\n");
                exit(0);
        }

	splitStr(option_string, "/", subStr, &nSubCnt);
	switch(nSubCnt) {
		case 1:
			if(isValidMacAddress(subStr[0])) {
				if(strncpy_s(pxmacmatch->mac_addr, sizeof(pxmacmatch->mac_addr), subStr[0], strnlen_s(subStr[0], QCLI_MAX_STR)) != EOK) {
					printf("Error copying the MAC Address !!\n");
					exit(0);
				}
			}else {
				//freeSubStr(subStr, nSubCnt);
				printf("***** Invalid Mac address %s passed to option %s \n",optarg, optstring);
				exit(0);
			} 
			memset_s(pxmacmatch->mac_addr_mask, sizeof(pxmacmatch->mac_addr_mask), 0);
			break;
		case 2:
			if(isValidMacAddress(subStr[0])) {
				if(strncpy_s(pxmacmatch->mac_addr, sizeof(pxmacmatch->mac_addr), subStr[0], strnlen_s(subStr[0], QCLI_MAX_STR)) != EOK) {
                                        printf("Error copying the MAC Address !!\n");
                                        exit(0);
                                }
			}else {
				//freeSubStr(subStr, nSubCnt);
				printf("***** Invalid Mac address %s passed to option %s \n",optarg, optstring);
				exit(0);
			} 
			if(isValidMacAddress(subStr[1])) {
				if(strncpy_s(pxmacmatch->mac_addr_mask, sizeof(pxmacmatch->mac_addr_mask), subStr[1], strnlen_s(subStr[1], QCLI_MAX_STR)) != EOK) {
                                        printf("Error copying the MAC Address Mask!!\n");
                                        exit(0);
                                }
			}else {
				//freeSubStr(subStr, nSubCnt);
				printf("***** Invalid Mac address mask %s passed to option %s \n",optarg, optstring);
				exit(0);
			} 
			break;
		default:
			//freeSubStr(subStr, nSubCnt);
			printf("Wrong number of parameter's passed to option %s \n",optstring);
			exit(0);
	}
	//freeSubStr(subStr, nSubCnt);
}


void classcfg_parse_ipaddr_ipmask( const char *optstring, const char* optarg, cl_ipaddr_match_t *pxipmatch)
{
	struct in_addr ipaddr; 
	struct in_addr mask;
	struct in6_addr ipaddr6;
	struct in6_addr mask6;
	char option_string[256];
	char subStr[3][64] = {0};
	int32_t nSubCnt = 0;

	if(strncpy_s(option_string, sizeof(option_string), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying to local string !!\n");
                exit(0);
        }
	splitStr(option_string, "/", subStr, &nSubCnt);
	if ( nSubCnt == 0 || nSubCnt > 2 ) {
		//freeSubStr(subStr, nSubCnt);
		printf("Wrong number of parameter's passed to option %s \n",optstring);
		exit(0);
	}
	switch(util_get_ipaddr_family(subStr[0])) {

		case AF_INET:
			ipaddr_conv(subStr[0], &ipaddr);	
			inet_ntop(AF_INET, &(ipaddr), pxipmatch->ipaddr, INET_ADDRSTRLEN);	
			if(nSubCnt == 2) {
				ipmask_conv(subStr[1], &mask);
				inet_ntop(AF_INET, &(mask), pxipmatch->mask, INET_ADDRSTRLEN);
			}
			break;

		case AF_INET6:
			ip6addr_conv(subStr[0], &ipaddr6);	
			inet_ntop(AF_INET6, &(ipaddr6), pxipmatch->ipaddr, INET_ADDRSTRLEN);	
			if(nSubCnt == 2) {
				ip6mask_conv(subStr[1], &mask6);
				inet_ntop(AF_INET6, &(mask6), pxipmatch->mask, INET_ADDRSTRLEN);
			}
			break;

		default:
			//freeSubStr(subStr, nSubCnt);
			printf("***** Invalid ipaddres/Mask %s passed to %s\n", option_string, optstring);
			exit(0);
	}
	//freeSubStr(subStr, nSubCnt);
}

void classcfg_set_param_in_ex(int32_t exclude_req, int32_t param , cl_param_in_ex_t *pxparaminex)
{
	pxparaminex->param_in |= param;

	if( exclude_req )
		pxparaminex->param_ex |= param ;
}

int32_t classcfg_handle_subcmd(int32_t index, const char* optarg, int32_t exclude, qos_class_cfg_t *pxclscfg)
{
	int32_t app_id;
	int32_t ret = LTQ_SUCCESS;
	struct in_addr ipaddr; 
	struct in6_addr ipaddr6; 
	char option_string[256];

	if(strncpy_s(option_string, sizeof(option_string), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying to local string !!\n");
                exit(0);
        }

	LOGF_LOG_DEBUG("Index [%d] option [%s] value [%s]\n", index,long_options[index].name, optarg);
	switch(index)  {
		case CLI_CLSCFG_PARAM_APP_NAME:
			/*app-anme*/
			app_id = classcfg_getval_fromname(optarg, axappname); 
			if ( app_id != LTQ_FAIL) {
				pxclscfg->filter.app.app_id = app_id ; 
			} else {
				return LTQ_FAIL;
			}
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_APP_ID, &pxclscfg->filter.app.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_L7_PROTO_NAME:
			if(strncpy_s(pxclscfg->filter.l7_proto, sizeof(pxclscfg->filter.l7_proto), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        		{
                		printf("Error copying L7 Protocol Name !!\n");
                		exit(0);
        		}
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_L7_PROTO, &pxclscfg->filter.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_SUBPROTO_NAME:
			/*subproto-name*/
			app_id = classcfg_getval_fromname(optarg, axsubprotoname); 
			if ( app_id != LTQ_FAIL) {
				pxclscfg->filter.app.subproto_id = app_id ; 
			} else 
				return LTQ_FAIL;
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SUBPROTO_ID, &pxclscfg->filter.app.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VENDOR:
			/*vendor*/
			if(strncpy_s(pxclscfg->filter.src_dhcp.vendor_classid, sizeof(pxclscfg->filter.src_dhcp.vendor_classid), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Vendor classid !!\n");
                                exit(0);
                        }
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_VENDOR_CLASS_ID, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_MODE:
			/*mode*/
			switch(classcfg_getval_fromname(optarg, axdhcpmode)) {

				case CLI_DHCP_EXACT:	
					pxclscfg->filter.src_dhcp.vendor_classid_mode = STR_EXACT_MATCH; /*!< Exact match required */
					break;

				case CLI_DHCP_PREFIX:
					pxclscfg->filter.src_dhcp.vendor_classid_mode = STR_PREFIX_MATCH; /*!< Prefix match required */
					break;

				case CLI_DHCP_SUFFIX:
					pxclscfg->filter.src_dhcp.vendor_classid_mode = STR_SUFFIX_MATCH; /*!< Suffix match required */
					break;

				case CLI_DHCP_SUBSTR:
					pxclscfg->filter.src_dhcp.vendor_classid_mode = STR_SUBSTRING_MATCH; /*!< Substring match required */
					break;

				default:
					return LTQ_FAIL;
			}
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_VENDOR_CLASS_ID_MODE, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_V6VENDOR:
			/*v6Vendor*/
			if(strncpy_s(pxclscfg->filter.src_dhcp.vendor_classid_v6, sizeof(pxclscfg->filter.src_dhcp.vendor_classid_v6), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying V6 Vendor Calss Id !!\n");
                                exit(0);
                        }
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_VENDOR_CLASS_ID_V6, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;
		case CLI_CLSCFG_PARAM_CLIENT:
			/*client*/
			if(strncpy_s(pxclscfg->filter.src_dhcp.clientid, sizeof(pxclscfg->filter.src_dhcp.clientid), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Client Id !!\n");
                                exit(0);
                        }
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_CLIENT_ID, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_CLASS:
			/*class*/
			if(strncpy_s(pxclscfg->filter.src_dhcp.user_classid, sizeof(pxclscfg->filter.src_dhcp.user_classid), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying User Class Id !!\n");
                                exit(0);
                        }
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_USER_CLASS_ID, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VINFO:
			/*vinfo*/
			if(strncpy_s(pxclscfg->filter.src_dhcp.vendor_specific_info, sizeof(pxclscfg->filter.src_dhcp.vendor_specific_info), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Vendor Info. !!\n");
                                exit(0);
                        }
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_DHCP_VENDOR_SPECIFIC_INFO, &pxclscfg->filter.src_dhcp.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_ETH_TYPE:
			/*eth-type*/
            pxclscfg->filter.ethhdr.eth_type = (int32_t)strtol(optarg, NULL, 0);
            if ((pxclscfg->filter.ethhdr.eth_type > USHRT_MAX || pxclscfg->filter.ethhdr.eth_type == LONG_MIN) || (pxclscfg->filter.ethhdr.eth_type == 0)) {
                LOGF_LOG_ERROR("strtol failed please check the arguement\n");
                exit(0);
            }
            LOGF_LOG_DEBUG("ETHTYPE classification for %d\n",pxclscfg->filter.ethhdr.eth_type);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_TYPE, &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_TYPE, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_SMAC:
			/*smac*/
			classcfg_parse_macaddr_macmask("--smac", optarg, &pxclscfg->filter.src_mac);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_MAC, &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_MAC_ADDRS, &pxclscfg->filter.src_mac.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_SRC_MAC_MASK, &pxclscfg->filter.src_mac.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_DMAC:
			/*dmac*/
			classcfg_parse_macaddr_macmask("--dmac", optarg, &pxclscfg->filter.dst_mac);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_DST_MAC, &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_DST_MAC_ADDRS, &pxclscfg->filter.dst_mac.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_DST_MAC_MASK, &pxclscfg->filter.dst_mac.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_802_3_SSAP:
		case CLI_CLSCFG_PARAM_802_3_DSAP:
			/*802_3-ssap*/	
			pxclscfg->filter.ethhdr.ssap = strtol(optarg, NULL, 0); //atoi(optarg);
			/*802_3-dsap*/
			pxclscfg->filter.ethhdr.dsap = strtol(optarg, NULL, 0);//atoi(optarg);
			/*802_3-ssap*/	
			/*802_3-dsap*/
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_SSAP, &pxclscfg->filter.ethhdr.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_DSAP, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VLAN_ID_INNER:
			/*vlan-id-inner*/
			pxclscfg->filter.ethhdr.inner_vlanid = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_INNER_VLANID, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VLAN_ID_OUTER:
			/*vlan-id-outer*/
			pxclscfg->filter.ethhdr.vlanid = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_VLANID, &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_VLANID, &pxclscfg->filter.ethhdr.param_in_ex);
			break;


		case CLI_CLSCFG_PARAM_VLAN_PRIO_OUTER:
			/*vlan-prio-outer*/
			if( atoi(optarg) >=1 && atoi(optarg) <=7 )
				pxclscfg->filter.ethhdr.prio = atoi(optarg);
			else {
				pxclscfg->filter.ethhdr.prio = 0;
			}
				classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_PRIO, &pxclscfg->filter.ethhdr.param_in_ex);
				classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR, &pxclscfg->filter.param_in_ex);
			break;
		case CLI_CLSCFG_PARAM_VLAN_PRIO_INNER:
			/*vlan-prio-inner*/
			if( atoi(optarg) >=1 && atoi(optarg) <=7) {
				//pxclscfg->filter.ethhdr.prio = atoi(optarg);
				pxclscfg->action.vlan.pcp = atoi(optarg);
			} else {
				//pxclscfg->filter.ethhdr.prio = 0;
				pxclscfg->action.vlan.pcp = 0;
			}
				pxclscfg->action.flags |=CL_ACTION_VLAN;
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_PRIO, &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_PRIO, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VLAN_DEI_INNER:
			/*vlan-dei-inner*/
			pxclscfg->filter.ethhdr.inner_vlan_dei = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_INNER_VLAN_DEI, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VLAN_DEI_OUTER:
			/*vlan-dei-outer*/
			pxclscfg->filter.ethhdr.vlan_dei = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_VLAN_DEI, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_LLC:
			/*llc*/
			pxclscfg->filter.ethhdr.llc_control = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_LLC_CONTROL, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_SNAPOUI:
			/*snapoui*/
			pxclscfg->filter.ethhdr.oui = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ETH_HDR_OUI, &pxclscfg->filter.ethhdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_POLICER:
			/*policer*/	
			if(strncpy_s(pxclscfg->action.policer.name, sizeof(pxclscfg->action.policer.name), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Policer name !!\n");
                                exit(0);
                        }
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER, &pxclscfg->action.param_in_ex);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_NAME, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_CR:
			/*cr*/
			pxclscfg->action.policer.cr = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_CR, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_CBS:
			/*cbs*/
			pxclscfg->action.policer.cbs = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_CBS, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_PR:
			/*pr*/
			pxclscfg->action.policer.pr = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_PR, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_PBS:
			/*pbs*/
			pxclscfg->action.policer.pbs = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_PBS, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_EBS:
			/*ebs*/
			pxclscfg->action.policer.ebs = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICER_EBS, &pxclscfg->action.policer.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_ACTDROP:
			/*actdrop*/
			if(pxclscfg->action.flags & CL_ACTION_DROP) {
				pxclscfg->action.drop = atoi(optarg);
			} else {
				pxclscfg->action.policer.confg.drop = atoi(optarg);
			}
			break;

		case CLI_CLSCFG_PARAM_ACTDSCP:
			/*actdscp*/
			if(pxclscfg->action.flags & CL_ACTION_DSCP) {
				pxclscfg->action.dscp = atoi(optarg);
			} else {
				pxclscfg->action.policer.confg.dscp = atoi(optarg);
			}
			break;

		case CLI_CLSCFG_PARAM_ACTETHPRIO:
			/*actethprio*/
			pxclscfg->action.policer.confg.pcp = atoi(optarg);
			break;

		case CLI_CLSCFG_PARAM_MARK:
			/*mark*/
			pxclscfg->action.tc = atoi(optarg);
			if((pxclscfg->action.tc <= 0) || (pxclscfg->action.tc > 16)) {
				printf("Wrong TC Value, Egress TC should be b/w 1 to 16\n");
				exit(0);
			}
			pxclscfg->action.flags |= CL_ACTION_EGRESS_TC_SET;
			break;
#ifdef PLATFORM_XRX500
		case CLI_CLSCFG_PARAM_MARK_ING:
			/*mark_ing*/
			pxclscfg->action.tc_ing = atoi(optarg);
			if((pxclscfg->action.tc_ing <= 0) || (pxclscfg->action.tc_ing > 16)) {
				printf("Wrong TC Value, Ingress TC should be b/w 1 to 16\n");
				exit(0);
			}
			pxclscfg->action.flags |= CL_ACTION_INGRESS_TC_SET;
			break;
#endif
		case CLI_CLSCFG_PARAM_MARK_PRIORITY:
                       pxclscfg->action.priority = atoi(optarg);
                       break;

		case CLI_CLSCFG_PARAM_MARK_LOC:
			/*mark location*/
			if(atoi(optarg) == 1)
				pxclscfg->action.flags &= ~CL_ACTION_TC_INGRESS;
			else if(atoi(optarg) == 2)
				pxclscfg->action.flags |= CL_ACTION_TC_INGRESS;
#ifdef PLATFORM_XRX500
			else if(atoi(optarg) == 3)
				pxclscfg->action.flags |= CL_ACTION_TC_INGRESS_EGRESS;
#endif
			else {
				printf("Given --mark-loc value is wrong\n");
				exit(0);
			}
			break;

		case CLI_CLSCFG_PARAM_ACCL:
			/*mark*/
			pxclscfg->action.accl = atoi(optarg);
			break;

		case CLI_CLSCFG_PARAM_FRAG:
			pxclscfg->action.flags |= CL_ACTION_FRAGMENT;
			pxclscfg->action.frag = atoi(optarg);
			break;

		case CLI_CLSCFG_PARAM_VID_INNER:
			/*vid-inner*/
			pxclscfg->action.vlan.inner_vlan_id = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_VLAN, &pxclscfg->action.param_in_ex);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_VLAN_ID_INNER, &pxclscfg->action.vlan.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_VID_OUTER:
			/*vid-outer*/
			pxclscfg->action.vlan.vlan_id = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_VLAN_ID, &pxclscfg->action.vlan.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_PCP_INNER:
			/*pcp-inner*/
			//pxclscfg->filter.ethhdr.prio = atoi(optarg);
			pxclscfg->action.vlan.inner_pcp = atoi(optarg);
			pxclscfg->action.flags |=CL_ACTION_VLAN;
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_VLAN_PCP_INNER, &pxclscfg->action.vlan.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_PCP_OUTER:
			/*pcp-outer*/
			pxclscfg->action.vlan.pcp = atoi(optarg);
			pxclscfg->action.flags |=CL_ACTION_VLAN;
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_VLAN_PCP, &pxclscfg->action.vlan.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_POLICYRT:
			/*policyrt*/
			pxclscfg->action.policy_rt.enable = 1;
			if(strncpy_s(pxclscfg->action.policy_rt.name, sizeof(pxclscfg->action.policy_rt.name), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Policy route !!\n");
                                exit(0);
                        }
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICYRT, &pxclscfg->action.param_in_ex);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICYRT_NAME, &pxclscfg->action.policy_rt.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_GW:
			/*gw*/
			switch(util_get_ipaddr_family((option_string))) {

				case AF_INET:
					ipaddr_conv(option_string, &ipaddr);	
					inet_ntop(AF_INET, &(ipaddr), pxclscfg->action.policy_rt.gw_ip , INET_ADDRSTRLEN);
					break;

				case AF_INET6:
					ip6addr_conv(option_string, &ipaddr6);	
					inet_ntop(AF_INET6, &(ipaddr6), pxclscfg->action.policy_rt.gw_ip , INET_ADDRSTRLEN);
					break;

				default:
					printf("***** Invalid ipaddres %s passed to gateway ip\n", option_string);
					exit(0);
			}
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICYRT_GW_IP, &pxclscfg->action.policy_rt.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_IFACE:
			/*iface*/
			if(strncpy_s(pxclscfg->action.policy_rt.dst_if, sizeof(pxclscfg->action.policy_rt.dst_if), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                        {
                                printf("Error copying Policy route destination iface.!!\n");
                                exit(0);
                        }
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICYRT_DST_IF, &pxclscfg->action.policy_rt.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_METRIC:
			/*metric*/
			pxclscfg->action.policy_rt.metric = atoi(optarg);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT_POLICYRT_METRIC, &pxclscfg->action.policy_rt.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_DST_IP_INNER:
			/*dst-ip-inner*/
			classcfg_parse_ipaddr_ipmask("--dst-ip-inner", optarg, &pxclscfg->filter.inner_ip.dst_ip);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_INNER_IP, &pxclscfg->filter.param_in_ex);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_INNER_IP_DST_IP, &pxclscfg->filter.inner_ip.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_SRC_IP_INNER:
			/*src-ip-inner*/
			classcfg_parse_ipaddr_ipmask("--src-ip-inner", optarg, &pxclscfg->filter.inner_ip.src_ip);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_INNER_IP, &pxclscfg->filter.param_in_ex);
			//classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_INNER_IP_SRC_IP, &pxclscfg->filter.inner_ip.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_IP_HDR_LEN_MIN:
			/*ip-hdr-len-min*/
			pxclscfg->filter.iphdr.ip_len_min = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR,  &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR_LEN_MIN,  &pxclscfg->filter.iphdr.param_in_ex);
			break;

		case CLI_CLSCFG_PARAM_IP_HDR_LEN_MAX:
			/*ip-hdr-len-max*/
			pxclscfg->filter.iphdr.ip_len_max = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR,  &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR_LEN_MAX,  &pxclscfg->filter.iphdr.param_in_ex);
			break;

		case  CLI_CLSCFG_PARAM_IP_HDR_DSCP:
			/*ip-hdr-dscp*/
			pxclscfg->filter.iphdr.dscp = atoi(optarg);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR,  &pxclscfg->filter.param_in_ex);
			classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_IP_HDR_DSCP,  &pxclscfg->filter.iphdr.param_in_ex);
			break;
#ifdef PLATFORM_XRX500
		case  CLI_CLSCFG_PARAM_ENGINE:
			if(atoi(optarg) == 1)
				pxclscfg->action.flags |= CL_ACTION_CL_SW_ENGINE;
			else if(atoi(optarg) == 2)
				pxclscfg->action.flags |= CL_ACTION_CL_HW_ENGINE;
			else {
				printf("Wrong cl-eng Value, it should be either 1 or 2\n");
				exit(0);
			}
			break;
#endif
	}
	return ret;
}

void classcfg_print_policer_usage(void)
{
	printf("Only following filter are supported for policer action: \n");
	printf("-d [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("-s [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("-p [Protocol Name] => Sets the IP protocol for the rule, which can be either icmp, tcp, udp #\n");
	printf("--dport [port] => Sets the destination port for the packet such as -p tcp --dport 3000. The supported values are 0-65535 #\n");
	printf("--sport [port] => Sets the destination port for the packet such as -p tcp --sport 3000. The supported values are 0-65535 #\n");
	printf("--smac address[/mask] => Source MAC #\n");
	printf("--dmac address[/mask] => Dst mac #\n");
	printf("--iphdr-dscp <dscp value> => DSCP value #\n");
	printf("\n# invert of match is not supported for any parameter.\n");
}

void classcfg_print_usage(void)
{
	printf("Usage: ./classcfg -A/-D/-L \n");
	printf("-A [name] => Appends a rule of specified name \n");
	printf("-D [name] => Deletes a rule of name \n");
	printf("-M [name] OR [location number]:[name] => Modifies a rule of name OR Modifies location also if specified \n");
	printf("-L [order] => If no order is specified list all rules else list only the rule that matches the order \n");
	printf("-I [location number]:[name] => Inserts a rule of name at location \n");
	printf("-d [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("-s [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("--src-ip-inner [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("--dst-ip-inner [IP/Mask] => Eg. 192.168.0.0/255.255.255.0 or 192.168.0.0/24. #\n");
	printf("-p [Protocol Name] => Sets the IP protocol for the rule, which can be either icmp, tcp, udp , or all #\n");
	printf("\t [Protocol Values] %s \n",classcfg_getoptions(axprototype));
	printf("-i <interface name> => Match the Incoming network interface. such as eth0 or ppp0 #\n");
	printf("-o <interface name> => Match the Outgoing network interface. such as eth0 or ppp0 #\n");
	printf("--dport [startport:endport] => Sets the destination port for the packet separte the two numbers with a colon(:), such as -p tcp --dport 3000:3200.The largest valid range is 0:65535 #\n");
	printf("--sport [startport:endport] => Sets the source port for the packet separte the two numbers with a colon (:), such as -p tcp --dport 3000:3200. The largest valid range is 0:65535 #\n");
	printf("--tcp-flags <flag name> =>-Eg. -p tcp --tcp-flags #\n");
	printf("\t [tcp-flag values :  %s] \n",classcfg_getoptions(axtcpflags));
	printf("-m <match name> => Match the Application \n");
	printf("\t [match values :     %s] \n",classcfg_getoptions(axmatchmode));
	printf("\t When -m=app --appname <name>\n");
	printf("\t [Appname values : %s] \n",classcfg_getoptions(axappname));
	printf("\t When -m=layer7 --l7proto <name>\n");
	printf("\t [l7proto values : ftp/sip/http etc.] \n");
	printf("\t When -m=dhcp --vendor <vendorId> --mode <mode name> --v6Vendor <vendorId> --client <clientId> --class <classId> --vinfo <vendor specific info>\n");
	printf("\t [Mode values] %s\n",classcfg_getoptions(axdhcpmode));
	printf("--smac address[/mask] => Source MAC #\n");
	printf("--dmac address[/mask] => Dst mac #\n");
	printf("--802_3-ssap sap =>DSAP and SSAP are two one byte 802.3 fields. The bytes are always equal, so only one byte (hexadecimal) is needed as an argument. #\n");
	printf("--vlan-id-inner/--vlan-id-outer id => The VLAN identifier field (VID) #\n");
	printf("--vlan-prio-inner/--vlan-prio-outer prio => The user priority field, a decimal number from 0 to 7. \n The VID should be set to 0 (\"null VID\") or unspecified (in the latter case the VID is deliberately set to 0 #\n");
	printf("--eth-type => Ethernet type #\n");
	printf("--llc [llc control field] => LLC control field\n");
	printf("--snapoui [snapoui] => SNAP OUI header\n");
	printf("--iphdr-len-min <length> => Minimum ip header length #\n");
	printf("--iphdr-len-max <length> => Max ip header length #\n");
	printf("--iphdr-dscp <dscp value> => DSCP value #\n");
#ifdef PLATFORM_XRX500
	printf("--cl-eng <1/2> => Where 1 is for SW engine classification and 2 is for HW engine classification\n");
#endif
	printf("-j <Action name> => Action name \n");
	printf("\t [Action Values] %s\n",classcfg_getoptions(axfilteraction));
	printf("\t When J is POLICER => --policer <name>  --cr < commited rate> --cbs <commited burst size> --pr <peak rate> \n");
	printf("\t --pbs <peak burst size> --ebr <excess burst rate> --actdscp <dscpval> --actdrop <dropval> --actethprio <ethprio>\n");
#ifdef PLATFORM_XRX500
	printf("\t When J is MARK => --mark <tcval> --mark_ing <tcval> ingress tc value, --mark-loc <1/2/3> where 1 is for egress queue, 2 for ingress queue, 3 for egress queue and ingress queue\n");
#else
	printf("\t When J is MARK => --mark <tcval> --mark-loc <1/2> where 1 is for egress queue, 2 for ingress queue\n");
#endif
	printf("\t When J is DSCP => --actdscp <dscpval>\n");
	printf("\t When J is DROP => --actdrop <dropval>\n");
	printf("\t When J is VLAN => --vid-inner <VLANID> --vid-outer <VLANID> --pcp-inner <pcpval> --pcp-outer <pcpval> \n");
	printf("\t When J is POLICYRT => --policyrt <name> --gw <gateway> --iface <dst interface> --metric <metric> \n");
	printf("\t When J is ACCL => --actaccl <0/1>\n");
	printf("\n# For invert match of these parameters, specify '!' at beginning.\n");
}

int32_t classcfg_show_classifiers(uint32_t order, char *sIface, int32_t num_entries, cl_match_filter_t *filter, cl_action_t *action)
{
	int32_t i = 0, nCnt = 0;

	UNUSED(action);

	printf("\nClassifier configuration:\n");
	for (i=0; i<num_entries; i++) {
		if(strnlen_s(sIface, QCLI_MAX_STR) > 0) {
			if(strcmp(sIface, (filter + i)->rx_if) != 0) {
				continue;
			}
		}
		if(order > 0) {
			if(order != (filter + i)->order) {
				continue;
			}
		}
		printf("%d. Classifier name: %s\n", nCnt+1, (filter+i)->name);
		printf("   Order: %u\n", (filter+i)->order);
		printf("   Status: %d\n", (filter+i)->status); /* Status is not updated in es */
		printf("   Interace: %s\n\n", (filter+i)->rx_if);
		nCnt++;
		/*
		   if ((filter+i)->param_in_ex.param_in){
		   if ((filter+i)->param_in_ex.param_in & CLSCFG_PARAM_APP_ID){
		   printf("   Classifier name: %s\n", (filter+i)->name);
		   }
		   }
		   */
	}

	return LTQ_SUCCESS;
}

int32_t classcfg_invoke_fapi(qos_class_cfg_t *pxclscfg)
{
	int32_t iret = LTQ_FAIL, nRet = LTQ_FAIL;
	int32_t num_entries = 0;
	char sErrMsg[128] = {0};
	cl_match_filter_t *filter = NULL;
	cl_action_t *action = NULL;

	if(!pxclscfg->filter.flags) {
		classcfg_print_usage();
		return LTQ_FAIL;
	}

	if(( pxclscfg->filter.flags & CL_FILTER_ADD ) || (pxclscfg->filter.flags & CL_FILTER_DELETE ) || pxclscfg->filter.flags & CL_FILTER_MODIFY) {
		/*Validate --mark-loc and TC's*/
		if((pxclscfg->action.flags & CL_ACTION_TC_INGRESS_EGRESS)) {
			if(!(pxclscfg->action.flags & CL_ACTION_EGRESS_TC_SET)) {
				printf("--mark-loc is both ingress/egress but --mark <egress tc> is not set");
				return LTQ_FAIL;
			}
			if(!(pxclscfg->action.flags & CL_ACTION_INGRESS_TC_SET)) {
				printf("--mark-loc is both ingress/egress but --mark_ing <ingress tc> is not set");
				return LTQ_FAIL;
			}
			if(pxclscfg->action.flags & CL_ACTION_CL_HW_ENGINE) {
				printf("--mark-loc is both ingress/egress supportted only with SW Engine ");
				return LTQ_FAIL;
			}
		} else {
			if(!(pxclscfg->action.flags & CL_ACTION_TC_INGRESS)) {
				if(!(pxclscfg->action.flags & CL_ACTION_EGRESS_TC_SET)) {
					printf("--mark-loc is egress but --mark <egress TC> is not set");
					return LTQ_FAIL;
				}
				if(pxclscfg->action.flags & CL_ACTION_INGRESS_TC_SET) {
					printf("--mark-loc is egress but --mark_ing <ingress tc> is set");
					return LTQ_FAIL;
				}
			}
		}

		if(pxclscfg->action.flags & CL_ACTION_TC_INGRESS) {
			if(pxclscfg->action.flags & CL_ACTION_EGRESS_TC_SET) {
				printf("--mark-loc is ingress but --mark <egress tc> is set");
				return LTQ_FAIL;
			}
			if(!(pxclscfg->action.flags & CL_ACTION_INGRESS_TC_SET)) {
				printf("--mark-loc is ingress but --mark_ing <ingress tc> is not set");
				return LTQ_FAIL;
			}
		}

		iret = fapi_qos_classifier_set(pxclscfg->filter.order, pxclscfg->filter, pxclscfg->action, pxclscfg->filter.flags);
		//fprintf(stderr, "l7proto:%s, flags:%s\n",pxclscfg->filter.l7_proto,(pxclscfg->filter.param_in_ex.param_in & CLSCFG_PARAM_L7_PROTO)?"yes":"No");
	} else if( pxclscfg->filter.flags & CL_FILTER_GET ) {
		iret = fapi_qos_classifier_get(pxclscfg->filter.order, &num_entries ,&filter, &action, pxclscfg->filter.flags);
		LOGF_LOG_DEBUG("returned from fapi, classifier count:%d\n",num_entries);
		classcfg_show_classifiers(pxclscfg->filter.order, pxclscfg->filter.rx_if, num_entries, filter, action);
		os_free(filter);
		os_free(action);
	}
	if(iret<0){
		nRet = find_error_message(iret, sErrMsg);
		printf("Classifier configuration failed.\n");
		printf("Reason : ");
		printf("%s\n", (nRet == LTQ_SUCCESS)?sErrMsg:"Unknown.");
	}
	else {
		printf("Classifier configuration successful.\n");
	}
	return iret;
}

int32_t classcfg_main(int32_t argc, char**argv)
{
  int32_t c, option_index = 0, exclude=0, proto_configured=0;
  qos_class_cfg_t xclscfg;
  int32_t ret = -1;	
  int32_t acclflag = 0;
  char subStr[3][64] = {0};
  int32_t nSubCnt = 0;
  memset_s(&xclscfg, sizeof(qos_class_cfg_t), 0);

  while(1) {
    c = getopt_long(argc, argv, "-:A:M:I:D:L:T:S:F:d:s:p:i:o:m:j:z",
        long_options, &option_index);
    if(c == -1) break;
    if( optarg ) { 
      if(is_exclude((char*) optarg) ) {
        exclude = 1;
        continue;		
      }			
    } else {
      printf("****** No/Invalid option value passed to %s \n",long_options[option_index].name);
      classcfg_print_usage();
      return LTQ_FAIL;
    }
    switch(c) {
      case 0:
        if(optarg){
          if ( ( ret = classcfg_handle_subcmd(option_index, optarg, exclude, &xclscfg) )  == LTQ_FAIL ) {
            printf("****** Invalid option value %s passed to %s \n",optarg, long_options[option_index].name);
            classcfg_print_usage();
            return LTQ_FAIL;
          } 
        }
        break;

      case 'A':
        //action add
        xclscfg.filter.flags |= CL_FILTER_ADD;
	if(strncpy_s(xclscfg.filter.name, sizeof(xclscfg.filter.name), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
	{
		printf("Error copying filter name!!\n");
		exit(0);
	}
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_FILTER_NAME, &xclscfg.filter.param_in_ex);
        break;

      case 'M':
        //action modify
        splitStr(optarg, ":", subStr, &nSubCnt);
	if( nSubCnt == 2) {
		xclscfg.filter.order = atoi(subStr[0]);
		if(strncpy_s(xclscfg.filter.name, sizeof(xclscfg.filter.name), subStr[1], strnlen_s(subStr[1], QCLI_MAX_STR)) != EOK)
		{
			printf("Error copying filter name!!\n");
			exit(0);
		}
	}else {
		if(strncpy_s(xclscfg.filter.name, sizeof(xclscfg.filter.name), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
                {
                        printf("Error copying filter name!!\n");
                        exit(0);
                }
	}
        xclscfg.filter.flags |= CL_FILTER_MODIFY;
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_FILTER_NAME, &xclscfg.filter.param_in_ex);
        //freeSubStr(subStr, nSubCnt);
        break;

      case 'I':
        //action insert
        splitStr(optarg, ":", subStr, &nSubCnt);
        if( nSubCnt != 2) {
          printf("****** Invalid usage/wrong number of parameter passed to -I \n");
          //freeSubStr(subStr, nSubCnt);
          classcfg_print_usage();
          return LTQ_FAIL;
        }
        xclscfg.filter.flags |= CL_FILTER_ADD;
        xclscfg.filter.order = atoi(subStr[0]);
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ORDER_NUM, &xclscfg.filter.param_in_ex);
	if(strncpy_s(xclscfg.filter.name, sizeof(xclscfg.filter.name), subStr[1], strnlen_s(subStr[1], QCLI_MAX_STR)) != EOK)
	{
		printf("Error copying filter name!!\n");
		exit(0);
	}
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_FILTER_NAME, &xclscfg.filter.param_in_ex);
        //freeSubStr(subStr, nSubCnt);
        break;

      case 'D':
        xclscfg.filter.flags |= CL_FILTER_DELETE;
	if(strncpy_s(xclscfg.filter.name, sizeof(xclscfg.filter.name), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying filter name!!\n");
                exit(0);
        }
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_FILTER_NAME, &xclscfg.filter.param_in_ex);
        break;

      case 'L':
        xclscfg.filter.flags |= CL_FILTER_GET;
        if(optarg)
          xclscfg.filter.order = atoi (optarg);
        //classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_FILTER_NAME, &xclscfg.filter.param_in_ex);
        break;

      case 'T':
        if (proto_configured) {
          classcfg_parse_portrange("--dport", optarg, &xclscfg.filter.outer_ip.dst_port);
          //set dport include/exclude flag
          classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP, &xclscfg.filter.param_in_ex);
          classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP_DST_PORT, &xclscfg.filter.outer_ip.param_in_ex);
        } else {
          printf("****** Protocol name not set, please specify the protocol name before setting port range \n");
          classcfg_print_usage();
          return LTQ_FAIL;
        }
        break;

      case 'F':
        //check whether tcp protocol is set, then use tcp-flags
        switch(classcfg_getval_fromname(optarg, axtcpflags)) {
          case CLI_TCP_ACK:
            xclscfg.filter.tcphdr.tcp_ack = 1;
            classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR_ACK, &xclscfg.filter.tcphdr.param_in_ex);
            break;

          case CLI_TCP_SYN:
            xclscfg.filter.tcphdr.tcp_syn = 1;
            classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR_SYN, &xclscfg.filter.tcphdr.param_in_ex);
            break;

          case CLI_TCP_FIN:
            xclscfg.filter.tcphdr.tcp_fin = 1;
            classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR_FIN, &xclscfg.filter.tcphdr.param_in_ex);
            break;

          case CLI_TCP_RST:	
            xclscfg.filter.tcphdr.tcp_rst = 1;
            classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR_RST, &xclscfg.filter.tcphdr.param_in_ex);
            break;

          case CLI_TCP_URG:	
            xclscfg.filter.tcphdr.tcp_urg = 1;
            classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR_URG, &xclscfg.filter.tcphdr.param_in_ex);
            break;

          default:
            printf("****** Invalid TCP flag values to --tcp-flags %s\n", optarg);
            classcfg_print_usage();
            return LTQ_FAIL;
        }
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TCP_HDR, &xclscfg.filter.param_in_ex);
        break;

      case 'd':
        classcfg_parse_ipaddr_ipmask("-d", optarg, &xclscfg.filter.outer_ip.dst_ip);
        //set outer ip and outer ip.dst_ip been selected
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP, &xclscfg.filter.param_in_ex);
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP_DST_IP, &xclscfg.filter.outer_ip.param_in_ex);
        break;

      case 's':
        classcfg_parse_ipaddr_ipmask("-s", optarg, &xclscfg.filter.outer_ip.src_ip);
        //set outer ip and outer ip.src_ip been selected
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP, &xclscfg.filter.param_in_ex);
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP_SRC_IP, &xclscfg.filter.outer_ip.param_in_ex);
        break;

      case 'p':
        switch(classcfg_getval_fromname(optarg, axprototype)) {
          case CLI_PROTO_ICMP:
            xclscfg.filter.outer_ip.ip_p = CLI_PROTO_ICMP;
            break;

          case CLI_PROTO_TCP:
            xclscfg.filter.outer_ip.ip_p = CLI_PROTO_TCP;
            break;

          case CLI_PROTO_UDP:
            xclscfg.filter.outer_ip.ip_p = CLI_PROTO_UDP;
            break;

          case CLI_PROTO_ALL:
            xclscfg.filter.outer_ip.ip_p = CLI_PROTO_ALL;
            break;

          default:
            printf("****** Invalid protocol value provided in option -p %s\n", optarg);
            classcfg_print_usage();
            return LTQ_FAIL;
        }
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP, &xclscfg.filter.param_in_ex);
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP_PROTO, &xclscfg.filter.outer_ip.param_in_ex);
        proto_configured = 1;
        break;

      case 'i':
	if(strncpy_s(xclscfg.filter.rx_if, sizeof(xclscfg.filter.rx_if), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying filter Rx iface!!\n");
                exit(0);
        }
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_RX_IF, &xclscfg.filter.param_in_ex);
        break;

      case 'o':
	if(strncpy_s(xclscfg.filter.tx_if, sizeof(xclscfg.filter.tx_if), optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        {
                printf("Error copying filter Tx iface!!\n");
                exit(0);
        }
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_TX_IF, &xclscfg.filter.param_in_ex);
        break;

      case 'm':
        LOGF_LOG_DEBUG("Option m %s\n", optarg);
        break;

      case 'j':
        //set capabality action been selected
        switch(classcfg_getval_fromname(optarg, axfilteraction)) {
          case CLI_ACTION_MARK: //mark
            xclscfg.action.flags |=CL_ACTION_TC;
            //xclscfg.action.tc = 1;
            //set capabality action tc selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_TC , &xclscfg.action.param_in_ex);
            break;

          case CLI_ACTION_DROP:
            xclscfg.action.flags |=CL_ACTION_DROP;
            //set capabality action drop selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_DROP , &xclscfg.action.param_in_ex);
            break;

          case CLI_ACTION_DSCP:
            xclscfg.action.flags |=CL_ACTION_DSCP;
            //set capabality action dscp selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_DSCP, &xclscfg.action.param_in_ex);
            break;

          case CLI_ACTION_VLAN:
            xclscfg.action.flags |=CL_ACTION_VLAN;
            //set capabality vlan selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_VLAN, &xclscfg.action.param_in_ex);
            break;

          case CLI_ACTION_POLICER:
            xclscfg.action.flags |=CL_ACTION_POLICER;
            xclscfg.action.policer.enable = 1;
            //set capabality policer selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_POLICER, &xclscfg.action.param_in_ex);
            break;

          case CLI_ACTION_POLICYRT:
            xclscfg.action.flags |=CL_ACTION_POLICY_ROUTE;
            xclscfg.action.policy_rt.enable = 1;
            //set capabality policyrt selected 
            //classcfg_set_param_in_ex(exclude, CL_ACTION_POLICY_ROUTE, &xclscfg.action.param_in_ex);
            break;

	  case CLI_ACTION_ACCL:
	    xclscfg.action.flags |=CL_ACTION_ACCELERATION;
	    acclflag = atoi(optarg);
	    xclscfg.action.accl = acclflag;
            break;

	  case CLI_ACTION_FRAG:
	    xclscfg.action.flags |= CL_ACTION_FRAGMENT;
	    xclscfg.action.frag = atoi(optarg);
	    break;

          default:
            printf("****** Invalid filters values to -j %s\n", optarg);
            classcfg_print_usage();
            return LTQ_FAIL;
        }
        //TODO: check where exactly need to be set this
        classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_ACT, &xclscfg.filter.param_in_ex);
        break;

      case 'S':
        if (proto_configured) {
          classcfg_parse_portrange("--sport", optarg, &xclscfg.filter.outer_ip.src_port);
          //set sport include/exclude flag
          classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP, &xclscfg.filter.param_in_ex);
          classcfg_set_param_in_ex(exclude, CLSCFG_PARAM_OUTER_IP_SRC_PORT, &xclscfg.filter.outer_ip.param_in_ex);
        } else {
          printf("****** Protocol name not set, please specify the protocol name before setting port range \n");
          classcfg_print_usage();
          return LTQ_FAIL;
        }
        break;

      default:
        classcfg_print_usage();
        return LTQ_FAIL;

    }
    exclude=0;
  }
  if(strnlen_s(xclscfg.filter.rx_if, QCLI_MAX_STR) == 0 && ((xclscfg.filter.flags & CL_FILTER_GET) == 0))
  {
      printf("please specify the interface name \n\n");
      classcfg_print_usage();
      return LTQ_FAIL;
  }

	if (xclscfg.action.flags & CL_ACTION_POLICER) {
		if (classcfg_is_police_supported(xclscfg) == LTQ_FAIL) {
			classcfg_print_policer_usage();
			return LTQ_SUCCESS;
		}
	}

  classcfg_invoke_fapi(&xclscfg);
  return LTQ_SUCCESS;
}
