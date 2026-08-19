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

#include "commondefs.h"
#include "qosal_cl_api.h"
#include "fapi_high.h"
#include "cli_ifcfg.h"
#include "help_logging.h"
#include "qosal_utils.h"

#include "safe_str_lib.h"
#include "safe_mem_lib.h"

extern uint16_t LOGLEVEL, LOGTYPE;

static struct option long_options[] = {
  {"iface", no_argument, 0, 'i'},
  {"ifmap", no_argument, 0, 'm'}
};

void ifcli_print_usage(void);
void ifcli_print_if_abs(iftype_t ifgroup, int32_t inumifs, char ifnames[MAX_IF_PER_INGRESS_GROUP][MAX_IF_NAME_LEN]);
void ifcli_print_if_inggrp(ifinggrp_t ifgroup, int32_t inumifs, char ifnames[MAX_IF_PER_INGRESS_GROUP][MAX_IF_NAME_LEN]);
int32_t ifcli_invoke_fapi(struct x_ifcfg_t *pxifcfg);


static inline int check_interface_exists(const char *ifname)
{
	char path[50];

	if (!ifname)
		return LTQ_FAIL;

	memset_s(path, sizeof(path), 0);
	snprintf(path, 50, "/sys/class/net/%s/mtu", ifname);

	if (access(path, F_OK) < 0)
		return LTQ_FAIL;

	return LTQ_SUCCESS;
}

void
ifcli_print_usage(void)
{
	printf("Usage: ifcfg --iface/--ifmap\n");
	printf("-A/D/R [interface name] -t [interface category] -b [base interface]\n");
	printf("iface:interface, ifmap:interface category object\n"); 
	printf("Interface categories\n");
	printf("\tETH LAN - 0\n");
	printf("\tETH WAN - 1\n");
	printf("\tPTM WAN - 2\n");
	printf("\tATM WAN - 3\n");
	printf("\tLTQ WAN - 4\n");
	printf("\tWLAN DirectPath - 5\n");
	printf("\tWLAN Non-DirectPath - 6\n");
	printf("\tLocal - 7\n");
	printf("Ingress Group\n");
	printf("\tINGGRP0 - 0\n");
	printf("\tINGGRP1 - 1\n");
	printf("\tINGGRP2 - 2\n");
	printf("\tINGGRP3 - 3\n");
	printf("\tINGGRP4 - 4\n");
	printf("\tINGGRP5 - 5\n");
}

void
ifcli_print_if_abs(iftype_t ifgroup, int32_t inumifs, char ifnames[MAX_IF_PER_INGRESS_GROUP][MAX_IF_NAME_LEN])
{
	int32_t i;
	printf("Number of Interfaces under Interface Category ID %d: %d\n", (int32_t)ifgroup, inumifs);
	for(i = 0; i < inumifs; i++)
		printf("\tInterface Name: %s\n",ifnames[i]);
}

void
ifcli_print_if_inggrp(ifinggrp_t ifgroup, int32_t inumifs, char ifnames[MAX_IF_PER_INGRESS_GROUP][MAX_IF_NAME_LEN])
{
	int32_t i;
	printf("Number of Interfaces under Ingress Group Id %d: %d\n", (int32_t)ifgroup, inumifs);
	for (i = 0 ; i < inumifs; i++)
		printf("\tInterface Name: %s\n",ifnames[i]);
}


int32_t
ifcli_invoke_fapi(struct x_ifcfg_t *pxifcfg)
{
	int i;

	if (!pxifcfg->uiFlags) {
		ifcli_print_usage();
		return LTQ_FAIL;
	}

	if ((pxifcfg->uiFlags & QOS_OP_F_ADD) || (pxifcfg->uiFlags & QOS_OP_F_DELETE)) {
		if (IFGROUP_VALID(pxifcfg->ifgroup)) {
			/* if Logical interface is not given, use base interface as logical*/
			if ((strnlen_s(pxifcfg->logical_base[0], MAX_IF_NAME_LEN)) == EOK) {
				if(strncpy_s(pxifcfg->logical_base[0], MAX_IF_NAME_LEN, pxifcfg->base_ifnames[0],
					strnlen_s(pxifcfg->base_ifnames[0], MAX_IF_NAME_LEN)) != EOK) {
					printf("Copying Base interface to Logical interface Failed\n");
					return LTQ_FAIL;
				}
			}
			fapi_qos_if_abs_set(pxifcfg->ifgroup, pxifcfg->ifnames[0], pxifcfg->uiFlags);
			/*if (IFGROUP_IS_WAN(pxifcfg->ifgroup))*/
			fapi_qos_if_base_set(pxifcfg->ifnames[0], pxifcfg->base_ifnames[0], pxifcfg->logical_base[0], pxifcfg->uiFlags);
		}
		if (IFINGGRP_VALID(pxifcfg->ifinggrp)) {
			fapi_qos_if_inggrp_set(pxifcfg->ifinggrp, pxifcfg->ifnames[0], pxifcfg->uiFlags);
			if (IFGROUP_IS_WAN(pxifcfg->ifgroup))
				fapi_qos_if_inggrp_set(pxifcfg->ifinggrp, pxifcfg->base_ifnames[0], pxifcfg->uiFlags);
		}
	} else if (pxifcfg->uiFlags & QOS_PORT_F_RT_CHG) {
		fapi_qos_port_update(pxifcfg->ifnames[0], pxifcfg->uiFlags);
	} else {
		if (pxifcfg->uiFlags & CL_IF_ABS_GET) {
			if (pxifcfg->ifgroup == QOS_IF_CATEGORY_MAX) {
				for (i = 0; i < QOS_IF_CATEGORY_MAX; i++) {
					if (fapi_qos_if_abs_get(i, &pxifcfg->num_entries, pxifcfg->ifnames, pxifcfg->uiFlags) == LTQ_SUCCESS)
						ifcli_print_if_abs(i, pxifcfg->num_entries, pxifcfg->ifnames);
				}
			} else {
				if (fapi_qos_if_abs_get(pxifcfg->ifgroup, &pxifcfg->num_entries, pxifcfg->ifnames, pxifcfg->uiFlags) == LTQ_SUCCESS)
					ifcli_print_if_abs(pxifcfg->ifgroup, pxifcfg->num_entries, pxifcfg->ifnames);
			}
		}
		if(pxifcfg->uiFlags & CL_IF_INGR_GET){
			if (pxifcfg->ifinggrp == QOS_IF_CAT_INGGRP_MAX) {
				for (i = 0; i < QOS_IF_CAT_INGGRP_MAX; i++) {
					if (fapi_qos_if_inggrp_get(i, &pxifcfg->num_entries, pxifcfg->ifnames, pxifcfg->uiFlags) == LTQ_SUCCESS)
						ifcli_print_if_inggrp(i, pxifcfg->num_entries, pxifcfg->ifnames);
				}
			} else {
				if (fapi_qos_if_inggrp_get(pxifcfg->ifinggrp, &pxifcfg->num_entries, pxifcfg->ifnames, pxifcfg->uiFlags) == LTQ_SUCCESS)
					ifcli_print_if_inggrp(pxifcfg->ifinggrp, pxifcfg->num_entries, pxifcfg->ifnames);
			}
		}
	}

	return LTQ_SUCCESS;
}

int32_t
ifcfg_main(int32_t argc, char **argv)
{
  int32_t c, ifop = 0, ifmapop = 0, option_index = 0;
  struct x_ifcfg_t xifcfg;
  
  memset_s(&xifcfg, sizeof(xifcfg), 0);
  xifcfg.ifinggrp = QOS_IF_CAT_INGGRP_MAX;
  xifcfg.ifgroup = QOS_IF_CATEGORY_MAX;

  while(1) {
    c = getopt_long(argc, argv, "A:D:i:b:l:m:t:R:g:L",
			long_options, &option_index);
    if(c==-1) break;
    switch(c){
      case 'i':
	/* interface operation */
	if(ifmapop == 1) {
		printf("Please specify either i/m\n");
		return LTQ_FAIL;
	}
	ifop = 1;
        break;
      case 'm':
	/* interface category map operation */
	if(ifop == 1) {
		printf("Please specify either i/m\n");
		return LTQ_FAIL;
	}
	ifmapop = 1;
        break;
      case 'A':
	if(ifop == 1)
		xifcfg.uiFlags |= QOS_OP_F_ADD;
	else
		xifcfg.uiFlags |= QOS_OP_F_ADD;
#ifndef CONFIG_LANTIQ_OPENWRT
	if (check_interface_exists(optarg) != LTQ_SUCCESS) {
		printf("Interface %s does not exist.\n", optarg);
		return LTQ_FAIL;
	}
#endif
	if(strncpy_s(xifcfg.ifnames[0], MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, MAX_IF_NAME_LEN)) != EOK)
        {
                printf("Error copying Interface name !!\n");
                return LTQ_FAIL;
        }
        break;
      case 'D':
	if(ifop == 1)
		xifcfg.uiFlags |= QOS_OP_F_DELETE;
	else
		xifcfg.uiFlags |= QOS_OP_F_DELETE;

	if (check_interface_exists(optarg) != LTQ_SUCCESS) {
		printf("Interface %s does not exist.\n", optarg);
		return LTQ_FAIL;
	}
	if(strncpy_s(xifcfg.ifnames[0], MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, MAX_IF_NAME_LEN)) != EOK)
        {
                printf("Error copying Interface name !!\n");
                return LTQ_FAIL;
        }
        break;
      case 't':
	/* applicable when ifmapop is set */
	xifcfg.ifgroup = atoi(optarg);
        break;
      case 'g':
	/* applicable when ifmapop is set */
	xifcfg.ifinggrp = atoi(optarg);
	xifcfg.uiFlags |= QOS_OP_F_INGRESS;
        break;
      case 'b':
	/* applicable when ifmapop is set */
	if (check_interface_exists(optarg) != LTQ_SUCCESS) {
		printf("Interface %s does not exist.\n", optarg);
		return LTQ_FAIL;
	}
	if(strncpy_s(xifcfg.base_ifnames[0], MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, MAX_IF_NAME_LEN)) != EOK)
        {
                printf("Error copying base interface name !!\n");
                return LTQ_FAIL;
        }
        break;
      case 'l':
	/* applicable when ifmapop is set */
	if(strncpy_s(xifcfg.logical_base[0], MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, MAX_IF_NAME_LEN)) != EOK)
        {
                printf("Error copying logical interface name !!\n");
                return LTQ_FAIL;
        }
        break;
      case 'L':
	if(ifmapop == 1) {
		xifcfg.uiFlags |= CL_IF_ABS_GET | CL_IF_INGR_GET;
	}
	break;
      case 'R':
	if(ifmapop == 1) {
		printf("Please select option --iface/-i for interface rate update\n");
		return LTQ_FAIL;
	}
	xifcfg.uiFlags |= QOS_PORT_F_RT_CHG;
	if (check_interface_exists(optarg) != LTQ_SUCCESS) {
		printf("Interface %s does not exist.\n", optarg);
		return LTQ_FAIL;
	}
	if(strncpy_s(xifcfg.ifnames[0], MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, MAX_IF_NAME_LEN)) != EOK)
        {
                printf("Error copying interface name !!\n");
                return LTQ_FAIL;
        }
	break;
      default:
	ifcli_print_usage();
	return LTQ_FAIL;
    }
  }

	return ifcli_invoke_fapi(&xifcfg);
}
