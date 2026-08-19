/******************************************************************************
 **
 ** FILE NAME	: qos_mgr_api.c
 ** PROJECT	: UGW
 ** MODULES	: QoS Manager API
 **
 ** DATE	: 08 Jan 2020
 ** AUTHOR	: Mohammed Aarif
 ** DESCRIPTION	: QoS Manager API Implementation
 ** COPYRIGHT  : Copyright (c) 2020 - 2025 MaxLinear, Inc.
 **              Copyright (c) 2019 - 2020 Intel Corporation
 **
 ** For licensing information, see the file 'LICENSE' in the root folder of
 ** this software module.
 **
 ** HISTORY
 ** $Date		$Author			$Comment
 ** 08 JAN 2020		Mohammed Aarif		Initiate Version
 *******************************************************************************/

#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/list.h>
#include <generated/autoconf.h>
#include <net/qos_mgr/qos_hal_api.h>
#if IS_ENABLED(CONFIG_QOS_MGR_TC_SUPPORT)
#include <net/qos_mgr/qos_mgr_tc_hook.h>
#endif
#include "qos_mgr_stack_al.h"
#include "qos_mgr_api.h"
#include <uapi/net/qos_mgr_common.h>
#include <net/qos_mgr/qos_mgr_hook.h>

#include "qos_mgr_ioctl.h"
#include "qos_mgr_api.h"
#include "qos_hal.h"
#include "qos_hal_debug.h"
#include <linux/pp_api.h>
#include <net/datapath_api.h>

#define QOS_MGR_MAX_Q 40
#define QOS_MGR_MAX_Q_LEN 100
#define QOS_MGR_Q_CAP 1
#define MAX_QOS_Q_CAPS 6
#define QOS_HAL 0
#define LOGICAL_QID_FLAG 1
#define PHYSICAL_QID_FLAG 0
#define MAX_PHP_QUEUES_PER_HIGH_PORT	7
#define CPU_HIGH_PORT_GPID_START_IDX	16
#define CPU_LOW_PORT_GPID_START_IDX	17

#define NETDEV_NAME(netdev) (netdev ? netdev_name(netdev) : "NULL net_device")
#define ETHWAN_PORT 15
int8_t FLEXI_PORT[]	= {7, 8, 9, 10, 11, 12, 13, 14};	/* Flexible Ports for DSL (ATM/PTM) WAN and WLAN. */
int8_t LAN_PORT[] 	= {2,3,4,5,6};
int8_t LAN_PORT_Q[][4] 	= { {8,9,10,11}, {12,13,14,15}, {16,17,18,19},{20,21,22,23},{24,25,26,27} };
int32_t WT_Q[] 		= {20480,20480,65535,65535};
int32_t SCH_Q[] 	= {1,1,0,0};
#define MAX_NUMBER_OF_LAN_PORTS 4

#define TMU_GREEN_DEFAULT_THRESHOLD	0x24
extern int gIngressQosEnable;
extern int32_t g_CPU_Count;

struct ingress_config_info
{
	char group_name[MAX_INGGROUP_NAME_LENGTH];
	uint8_t group_id;
	uint8_t member_count;
	char *ifnames;
	bool ingress_option;
	struct ingress_config_info *next;
};
struct ingress_config_info *ingress_config_info_list = NULL;

static QOS_SHAPER_LIST_ITEM *g_qos_shaper = NULL;
static QOS_MGR_LOCK g_qos_shaper_lock;

static QOS_INGGRP_LIST_ITEM *g_qos_inggrp_list = NULL;
static QOS_MGR_LOCK g_qos_inggrp_list_lock;

QOS_QUEUE_LIST_ITEM *g_qos_queue;
QOS_MGR_LOCK g_qos_queue_lock;

typedef enum
{
    WMM_TC_BE_1 = 0,
    WMM_TC_BK_1,
    WMM_TC_BK_2,
    WMM_TC_BE_2,
    WMM_TC_VI_1,
    WMM_TC_VI_2,
    WMM_TC_VO_1,
    WMM_TC_VO_2
} WMM_TC;

#if defined(WMM_QOS_CONFIG) && WMM_QOS_CONFIG
#define MAX_WLAN_DEV 6
int g_eth_class_prio_map[MAX_WLAN_DEV][MAX_TC_NUM] = {
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7},
};
#endif /*WMM_QOS_CONFIG*/

/*
 * Reduced Traffic Clsss : START
 */

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
#if 0
static uint16_t qos_mgr_tc_reduced(uint32_t tc_input[], uint8_t tc_num)
{
	uint16_t tc_curr;
	uint8_t i;
	uint16_t tc_reduced = 0x0;

	struct core_ops *gsw_ops;

	GSW_QoS_RdTraffic_ClassCfg_t rdClass = {0};
	gsw_ops = gsw_get_swcore_ops(0); /* gsw_get_swcore_ops 0 is for GSWIP L */
	if (!gsw_ops)
		return 0;
	gsw_ops->gsw_qos_ops.QoS_ReducedClassGet(gsw_ops, &rdClass);
	
	for(i = 0; i < tc_num; i++) {
		tc_curr = tc_input[i];

		/*
		 * To support the case of multiple tc to a queue, final reduced tc is then calculated
		 * by doing bitwise-or of all the reduced tcs
		 */
		tc_reduced = tc_reduced | (1 << rdClass.nRdTrafficClass[tc_curr]);
	}
	return tc_reduced;
}
#endif
#endif

/*
 * Reduced Traffic Clsss : END
 */

/*
#############STRUCTURE DEFINITIONS : START################
*/

/*
############# STRUCTURE DEFINITIONS : END################
*/
/*
############# SUPPORTING SHAPER FUNCTION DEFINITIONS : START################
*/
/* Shaper info data structures */
QOS_SHAPER_LIST_ITEM * qos_mgr_shaper_alloc_item(void)	/* alloc_netif_info */
{
	QOS_SHAPER_LIST_ITEM *obj;

	obj = (QOS_SHAPER_LIST_ITEM *)qos_mgr_malloc(sizeof(*obj));
	if ( obj ) {
		qos_mgr_memset(obj, 0, sizeof(*obj));
		qos_mgr_atomic_set(&obj->count, 1);
	}
	return obj;
}
EXPORT_SYMBOL(qos_mgr_shaper_alloc_item);

void qos_mgr_shaper_free_item(QOS_SHAPER_LIST_ITEM *obj)	/* free_netif_info*/
{
	if ( qos_mgr_atomic_dec(&obj->count) == 0 ) {
		qos_mgr_free(obj);
	}
}
EXPORT_SYMBOL(qos_mgr_shaper_free_item);

void qos_mgr_shaper_lock_list(void)	/* lock_netif_info_list*/
{
	qos_mgr_lock_get(&g_qos_shaper_lock);
}
EXPORT_SYMBOL(qos_mgr_shaper_lock_list);

void qos_mgr_shaper_unlock_list(void)	/* unlock_netif_info_list */
{
	qos_mgr_lock_release(&g_qos_shaper_lock);
}
EXPORT_SYMBOL(qos_mgr_shaper_unlock_list);

void __qos_mgr_shaper_add_item(QOS_SHAPER_LIST_ITEM *obj)	/* add_netif_info */
{
	qos_mgr_atomic_inc(&obj->count);
	obj->next = g_qos_shaper;
	g_qos_shaper = obj;
}
EXPORT_SYMBOL(__qos_mgr_shaper_add_item);

void qos_mgr_shaper_remove_item(int32_t s_num, QOS_MGR_IFNAME ifname[16],QOS_SHAPER_LIST_ITEM **pp_info)	/* remove_netif_info*/
{
	QOS_SHAPER_LIST_ITEM *p_prev, *p_cur;

	if ( pp_info )
		*pp_info = NULL;
	p_prev = NULL;
	qos_mgr_shaper_lock_list();
	for ( p_cur = g_qos_shaper; p_cur; p_prev = p_cur, p_cur = p_cur->next )
		if ( (p_cur->shaperid == s_num) && (strcmp(p_cur->ifname,ifname) == 0)) {
			if ( !p_prev )
				g_qos_shaper = p_cur->next;
			else
				p_prev->next = p_cur->next;
			if ( pp_info )
				*pp_info = p_cur;
			else
				qos_mgr_shaper_free_item(p_cur);
			break;
		}
	qos_mgr_shaper_unlock_list();
}
EXPORT_SYMBOL(qos_mgr_shaper_remove_item);

void qos_mgr_shaper_free_list(void)	/* free_netif_info_list*/
{
	QOS_SHAPER_LIST_ITEM *obj;

	qos_mgr_shaper_lock_list();
	while ( g_qos_shaper ) {
		obj = g_qos_shaper;
		g_qos_shaper = g_qos_shaper->next;

		qos_mgr_shaper_free_item(obj);
		obj = NULL;
	}
	qos_mgr_shaper_unlock_list();
}
EXPORT_SYMBOL(qos_mgr_shaper_free_list);

/* QoS Queue*/
int32_t __qos_mgr_shaper_lookup(int32_t s_num, QOS_MGR_IFNAME ifname[16],QOS_SHAPER_LIST_ITEM **pp_info)	/* netif_info_is_added*/
{
	int32_t ret = QOS_MGR_ENOTAVAIL;
	QOS_SHAPER_LIST_ITEM *p;

	for (p = g_qos_shaper; p; p = p->next ) {
		if ((p->shaperid == s_num) && (strcmp(p->ifname, ifname) == 0)) {
			ret = QOS_MGR_SUCCESS;
			if (pp_info) {
				qos_mgr_atomic_inc(&p->count);
				*pp_info = p;
			}
			break;
		}
	}

	return ret;
}
EXPORT_SYMBOL(__qos_mgr_shaper_lookup);

int32_t qos_mgr_shaper_lookup(int32_t s_num, QOS_MGR_IFNAME ifname[16],QOS_SHAPER_LIST_ITEM **pp_info)	/* netif_info_is_added*/
{
	int32_t ret;
	qos_mgr_shaper_lock_list();
	ret = __qos_mgr_shaper_lookup(s_num, ifname, pp_info);
	qos_mgr_shaper_unlock_list();

	return ret;
}
EXPORT_SYMBOL(qos_mgr_shaper_lookup);

int32_t qosal_add_shaper(QOS_MGR_CMD_RATE_INFO *rate_info, QOS_SHAPER_LIST_ITEM **pp_item)
{
	int32_t ret = QOS_MGR_SUCCESS;
	int32_t res;
	QOS_SHAPER_LIST_ITEM *p_item;
	QOS_SHAPER_LIST_ITEM *p_item1;
	QOS_MGR_ADD_SHAPER_CFG shaper_cfg;
	memset(&shaper_cfg,0x00,sizeof(QOS_MGR_ADD_SHAPER_CFG)); 
	p_item = qos_mgr_shaper_alloc_item();
	if ( !p_item ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"alloc shaper list item failed \n" ); 
		return QOS_MGR_ENOMEM;
	}
	qos_mgr_strncpy(p_item->ifname, rate_info->ifname, QOS_MGR_IF_NAME_SIZE);
	qos_mgr_strncpy(p_item->dev_name, rate_info->ifname, QOS_MGR_IF_NAME_SIZE);
	p_item->portid = rate_info->portid;
	p_item->shaperid = rate_info->shaperid;
	p_item->shaper.enable = rate_info->shaper.enable;
	p_item->shaper.pir = rate_info->shaper.pir;
	p_item->shaper.pbs = rate_info->shaper.pbs;
	p_item->shaper.cir = rate_info->shaper.cir;
	p_item->shaper.cbs = rate_info->shaper.cbs;
	p_item->shaper.flags = rate_info->shaper.flags;

	p_item1 = *pp_item;
	res = qos_mgr_shaper_lookup(rate_info->shaperid,rate_info->ifname,&p_item1);
	if( res == QOS_MGR_SUCCESS ) {
		p_item->p_entry = p_item1->p_entry;
	
		qos_mgr_shaper_free_item(p_item1);
		qos_mgr_shaper_remove_item(p_item1->shaperid,p_item1->ifname,NULL);
		qos_mgr_shaper_free_item(p_item1);
	}

	__qos_mgr_shaper_add_item(p_item);
	
	shaper_cfg.enable = rate_info->shaper.enable;
	shaper_cfg.mode = rate_info->shaper.mode;
	shaper_cfg.pir = rate_info->shaper.pir;
	shaper_cfg.pbs = rate_info->shaper.pbs;
	shaper_cfg.cir = rate_info->shaper.cir;
	shaper_cfg.cbs = rate_info->shaper.cbs;
	shaper_cfg.flags = rate_info->shaper.flags;
	ret = qos_mgr_set_qos_shaper(rate_info->shaperid,rate_info->rate,
			rate_info->burst,&shaper_cfg,rate_info->shaper.flags,QOS_HAL);

	if (ret == QOS_MGR_SUCCESS) {
		p_item->p_entry = (shaper_cfg.phys_shaperid);
	} else
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"qos_mgr_set_qos_shaper failed \n" );

	*pp_item = p_item;
	return ret; 
}

/*
############# SUPPORTING SHAPER FUNCTION DEFINITIONS : END################
*/

/*
############# SUPPORTING METER FUNCTION DEFINITIONS : START################
*/

/*
############# SUPPORTING METER FUNCTION DEFINITIONS : END################
*/
/*
############# SUPPORTING INGRESS FUNCTION DEFINITIONS : START################
*/
QOS_INGGRP_LIST_ITEM * qos_mgr_inggrp_alloc_item(void)	/*	alloc_netif_info*/
{
	QOS_INGGRP_LIST_ITEM *obj;

	obj = (QOS_INGGRP_LIST_ITEM *)qos_mgr_malloc(sizeof(*obj));
	if ( obj ) {
		qos_mgr_memset(obj, 0, sizeof(*obj));
		qos_mgr_atomic_set(&obj->count, 1);
	}
	return obj;
}
EXPORT_SYMBOL(qos_mgr_inggrp_alloc_item);

void qos_mgr_inggrp_free_item(QOS_INGGRP_LIST_ITEM *obj)	/* free_netif_info*/
{
	if ( qos_mgr_atomic_dec(&obj->count) == 0 ) {
		qos_mgr_free(obj);
	}
}
EXPORT_SYMBOL(qos_mgr_inggrp_free_item);

void qos_mgr_inggrp_lock_list(void)	/*	lock_netif_info_list*/
{
	qos_mgr_lock_get(&g_qos_inggrp_list_lock);
}
EXPORT_SYMBOL(qos_mgr_inggrp_lock_list);

void qos_mgr_inggrp_unlock_list(void)	/*	unlock_netif_info_list*/
{
	qos_mgr_lock_release(&g_qos_inggrp_list_lock);
}
EXPORT_SYMBOL(qos_mgr_inggrp_unlock_list);

void __qos_mgr_inggrp_add_item(QOS_INGGRP_LIST_ITEM *obj)	 /*	add_netif_info*/
{
	qos_mgr_atomic_inc(&obj->count);
	obj->next = (struct qos_inggrp_list_item *)g_qos_inggrp_list;
	g_qos_inggrp_list = obj;
}
EXPORT_SYMBOL(__qos_mgr_inggrp_add_item);

void qos_mgr_inggrp_remove_item(QOS_MGR_IFNAME ifname[16], QOS_INGGRP_LIST_ITEM **pp_info)	/*	remove_netif_info*/
{
	QOS_INGGRP_LIST_ITEM *p_prev, *p_cur;

	if ( pp_info )
		*pp_info = NULL;
	p_prev = NULL;
	qos_mgr_inggrp_lock_list();
	for ( p_cur = g_qos_inggrp_list; p_cur; p_prev = p_cur, p_cur = (QOS_INGGRP_LIST_ITEM *)p_cur->next )
		if ((strcmp(p_cur->ifname,ifname) == 0)) {
			if ( !p_prev )
				g_qos_inggrp_list = (QOS_INGGRP_LIST_ITEM *)p_cur->next;
			else
				p_prev->next = p_cur->next;
			if ( pp_info )
				*pp_info = p_cur;
			else
				qos_mgr_inggrp_free_item(p_cur);
			break;
		}
	qos_mgr_inggrp_unlock_list();
}
EXPORT_SYMBOL(qos_mgr_inggrp_remove_item);

void qos_mgr_inggrp_free_list(void)
{
	 QOS_INGGRP_LIST_ITEM *obj;

	qos_mgr_inggrp_lock_list();
	while ( g_qos_inggrp_list ) {
		obj = g_qos_inggrp_list;
		g_qos_inggrp_list = (QOS_INGGRP_LIST_ITEM *)g_qos_inggrp_list->next;

		qos_mgr_inggrp_free_item(obj);
		obj = NULL;
	}
	qos_mgr_inggrp_unlock_list();
}
EXPORT_SYMBOL(qos_mgr_inggrp_free_list);

int32_t __qos_mgr_inggrp_lookup(const QOS_MGR_IFNAME ifname[16],QOS_INGGRP_LIST_ITEM **pp_info)	/* netif_info_is_added*/
{
	int32_t ret = QOS_MGR_ENOTAVAIL;
	QOS_INGGRP_LIST_ITEM *p;
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: lookup ifname=%s\n", __func__, __LINE__, ifname);
	for ( p = g_qos_inggrp_list; p; p = (QOS_INGGRP_LIST_ITEM *)p->next ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: entry %p: ingress group=%d, ifname=%s\n", __func__,
				__LINE__, p, p->ingress_group, p->ifname); 
		if (strcmp(p->ifname,ifname) == 0) {
			ret = QOS_MGR_SUCCESS;
			if ( pp_info ) {
				qos_mgr_atomic_inc(&p->count);
				*pp_info = p;
			}
			break;
		}
	}

	return ret;
}
EXPORT_SYMBOL(__qos_mgr_inggrp_lookup);

int32_t qos_mgr_inggrp_lookup(const QOS_MGR_IFNAME ifname[16],QOS_INGGRP_LIST_ITEM **pp_info)	/* netif_info_is_added*/
{
	int32_t ret;
	qos_mgr_inggrp_lock_list();
	ret = __qos_mgr_inggrp_lookup(ifname, pp_info);
	qos_mgr_inggrp_unlock_list();

	return ret;
}
EXPORT_SYMBOL(qos_mgr_inggrp_lookup);

int32_t qos_mgr_set_qos_inggrp(QOS_MGR_INGGRP_CFG *inggrp_info, uint32_t hal_id)
{
	return QOS_MGR_SUCCESS;
}

int32_t qosal_get_qos_inggrp(QOS_MGR_INGGRP inggrp, QOS_MGR_IFNAME ifnames[QOS_MGR_MAX_IF_PER_INGGRP][QOS_MGR_IF_NAME_SIZE])
{
	QOS_INGGRP_LIST_ITEM *p;
	int32_t count = 0;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s;%d: find all interfaces in inggrp%d", __func__, __LINE__, inggrp);
	qos_mgr_inggrp_lock_list();
	for ( p = g_qos_inggrp_list; p && (count < QOS_MGR_MAX_IF_PER_INGGRP); p = (QOS_INGGRP_LIST_ITEM *)p->next ) {
		if (p->ingress_group == inggrp) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: found interface %s in inggrp %d", __func__, __LINE__, p->ifname, inggrp); 
			strncpy(ifnames[count++], p->ifname, QOS_MGR_IF_NAME_SIZE);
		}
	}
	qos_mgr_inggrp_unlock_list();
	return count;
}
EXPORT_SYMBOL(qosal_get_qos_inggrp);

void dump_fapi_ingress_config(void)
{
	uint8_t count = 0;
	struct ingress_config_info *ingress_config_info_instance = NULL;

	if (ingress_config_info_list == NULL) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"No ingress configuration from fapi.\n");
		return;
	}

	for (ingress_config_info_instance = ingress_config_info_list;
		ingress_config_info_instance != NULL;
		ingress_config_info_instance=ingress_config_info_instance->next) {

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"GroupId:[%d] Group Name:[%s] Group Member Count:[%d] Ingress Option:[%d]\n",
			ingress_config_info_instance->group_id,
			ingress_config_info_instance->group_name,
			ingress_config_info_instance->member_count,
			ingress_config_info_instance->ingress_option);

		for (count = 0; count < ingress_config_info_instance->member_count; count++)
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Ingress group Members:%s",ingress_config_info_instance->ifnames+(count*QOS_MGR_IF_NAME_SIZE));
		pr_debug("\n");
	}

	return;
}

void parse_fapi_req(QOS_MGR_CMD_INGGRP_INFO *inggrp_info)
{
	struct ingress_config_info *node = NULL;
	uint8_t count = 0;
	int idx = 0;

	for (idx = 0; (idx < NUM_CPU_INGRESS_GROUPS && inggrp_info->cpu_inggrp[idx].if_cnt > 0); idx++) {
		if (node != NULL) {
			node->next = (struct ingress_config_info *) qos_mgr_malloc(sizeof (struct ingress_config_info));
			node = node->next;
		} else {
			node = (struct ingress_config_info *) qos_mgr_malloc(sizeof (struct ingress_config_info));
			if(node == NULL)
				return;
		}
		memset(node, 0, sizeof(struct ingress_config_info));
		if (ingress_config_info_list == NULL)
			ingress_config_info_list = node;

		qos_mgr_strncpy(node->group_name, inggrp_info->cpu_inggrp[idx].grp_name, MAX_INGGROUP_NAME_LENGTH);
		node->group_id = inggrp_info->cpu_inggrp[idx].grp_id;
		node->member_count = inggrp_info->cpu_inggrp[idx].if_cnt;
		node->ingress_option = inggrp_info->cpu_inggrp[idx].ing_op;
		node->ifnames = (char *) qos_mgr_malloc(node->member_count*QOS_MGR_IF_NAME_SIZE);
		if (node->ifnames) {
			for (count = 0; count < inggrp_info->cpu_inggrp[idx].if_cnt; count++) {
				strcpy((node->ifnames + (count*QOS_MGR_IF_NAME_SIZE)),
						inggrp_info->cpu_inggrp[idx].ifnames[count]);
			}
		}
	}
}

void qos_mgr_configure_ingress_queues(const char *in_proc)
{
	QOS_MGR_CMD_INGGRP_INFO *inggrp_info = NULL;
	QOS_INGGRP_LIST_ITEM *p_item;
	int idx = 0;
	bool all = 0;

	if (inggrp_info == NULL) {
		inggrp_info = (QOS_MGR_CMD_INGGRP_INFO *)qos_mgr_malloc(sizeof(QOS_MGR_CMD_INGGRP_INFO));
		if (inggrp_info == NULL)
			return;
		memset(inggrp_info, 0, sizeof(QOS_MGR_CMD_INGGRP_INFO));
	}

	if (strcmp(in_proc, "all") == 0)
		all = 1;
	
	if ((strcmp(in_proc, "eth1") == 0) || (strcmp(in_proc, "ptm0") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=2;
		inggrp_info->cpu_inggrp[idx].grp_id=0;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "eth1");
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[1], "ptm0");
		idx++;
	}

	if ((strcmp(in_proc, "eth0_1") == 0) || (strcmp(in_proc, "eth0_2") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=2;
		inggrp_info->cpu_inggrp[idx].grp_id=1;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "eth0_1");
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[1], "eth0_2");
		idx++;
	}

	if ((strcmp(in_proc, "eth0_3") == 0) || (strcmp(in_proc, "eth0_4") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=2;
		inggrp_info->cpu_inggrp[idx].grp_id=2;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "eth0_3");
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[1], "eth0_4");
		idx++;
	}
	
	if ((strcmp(in_proc, "wlan0") == 0) || (strcmp(in_proc, "wlan1") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=2;
		inggrp_info->cpu_inggrp[idx].grp_id=3;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "wlan0");
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[1], "wlan1");
		idx++;
	}
	if ((strcmp(in_proc, "wlan4") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=1;
		inggrp_info->cpu_inggrp[idx].grp_id=4;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "wlan4");
		idx++;
	}
	if ((strcmp(in_proc, "VUNI0_0") == 0) || (all == 1)) {

		inggrp_info->cpu_inggrp[idx].if_cnt=1;
		inggrp_info->cpu_inggrp[idx].grp_id=5;
		inggrp_info->cpu_inggrp[idx].ing_op=0;
		strcpy(inggrp_info->cpu_inggrp[idx].ifnames[0], "VUNI0_0");
		idx++;
	}

	qosal_set_qos_inggrp(inggrp_info, &p_item);
	qos_mgr_free(inggrp_info);
	return;
}

static int32_t qos_mgr_get_ptrtoinggrp_from_ifname(char* ifname, QOS_INGGRP_LIST_ITEM** ptr_to_inggrp, int* ifidx)
{
	QOS_INGGRP_LIST_ITEM* ptr = NULL;
	int32_t ret = QOS_MGR_FAILURE;
	int ifiter = 0;

	if (ifidx == NULL || ptr_to_inggrp == NULL)
		return ret;

	for (ptr = g_qos_inggrp_list; ptr; ptr = (QOS_INGGRP_LIST_ITEM *)ptr->next) {
		for (ifiter = 0; ifiter < ptr->if_count; ifiter++) {
			if (strncmp(ptr->ifnames[ifiter], ifname, QOS_MGR_IFNAME_SIZE) == 0) {
				*ptr_to_inggrp = ptr;
				*ifidx = ifiter;
				return QOS_MGR_SUCCESS;
			}
		}
	}

	return ret;
}

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
static int32_t qos_mgr_update_inggrp_protected_queues(QOS_INGGRP_LIST_ITEM* ptr_to_protected_group)
{
	QOS_INGGRP_LIST_ITEM* ptr;
	uint8_t qiter 	= 0;
	uint8_t cpuiter = 0;

	if(ptr_to_protected_group == NULL)
		return QOS_MGR_FAILURE;

	for(ptr = g_qos_inggrp_list; ptr != NULL; ptr = ptr->next) {
		if(strncmp(ptr->ifname, ptr_to_protected_group->ifname, QOS_MGR_IFNAME_SIZE) != 0) {
			for(qiter = 0; qiter < MAX_INGRESS_PROTECTED_QUEUES_PER_PORT; qiter++) {
				for(cpuiter = 0; cpuiter < LGM_CPU_PORT_COUNT; cpuiter++) {
					ptr->user_queue_high[cpuiter][qiter].qid = ptr_to_protected_group->user_queue_high[cpuiter][qiter].qid;
				}
			}
		}
	}

	return QOS_MGR_SUCCESS;
}
#endif

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
static int32_t qos_mgr_create_pp_ing_groups(void)
{
	struct pp_cpu_info *pp_cpu_info_ptr = NULL;
	QOS_INGGRP_LIST_ITEM* ing_grp_iter = NULL;
	int cpu_iter = 0;
	int q_iter   = 0;
	int if_iter  = 0;

	pp_cpu_info_ptr = (struct pp_cpu_info*)qos_mgr_malloc(LGM_CPU_PORT_COUNT * sizeof(struct pp_cpu_info));

	if(pp_cpu_info_ptr == NULL)
		return QOS_MGR_FAILURE;

	for(ing_grp_iter = g_qos_inggrp_list; ing_grp_iter != NULL; ing_grp_iter = ing_grp_iter->next) {
		if(strcmp(ing_grp_iter->ifname, "INGGRP_phigh") != 0) {
			for(cpu_iter = 0; cpu_iter < LGM_CPU_PORT_COUNT; cpu_iter++) {
				for(q_iter = 0; q_iter < MAX_INGRESS_QUEUE_PER_GROUP_PER_PORT; q_iter++) {
					if(q_iter == MAX_INGRESS_PROTECTED_QUEUES_PER_PORT) {
						pp_cpu_info_ptr[cpu_iter].queue[q_iter].gpid = CPU_LOW_PORT_GPID_START_IDX + (cpu_iter*2);
						pp_cpu_info_ptr[cpu_iter].queue[q_iter].id = ing_grp_iter->user_queue_low[cpu_iter][q_iter - MAX_INGRESS_PROTECTED_QUEUES_PER_PORT].qid;
					}
					else {
						pp_cpu_info_ptr[cpu_iter].queue[q_iter].gpid = CPU_HIGH_PORT_GPID_START_IDX + (cpu_iter*2);
						pp_cpu_info_ptr[cpu_iter].num_q = MAX_INGRESS_QUEUE_PER_GROUP_PER_PORT;
						pp_cpu_info_ptr[cpu_iter].queue[q_iter].id = ing_grp_iter->user_queue_high[cpu_iter][q_iter].qid;
					}
				}
			}

			if(ing_grp_iter->pp_grp_id > 0) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"The PP ingress group already present grp id: [%d]\n", ing_grp_iter->pp_grp_id);
				goto QOS_MGR_RETURN_FAILURE;
			}

			ing_grp_iter->pp_grp_id = pp_gpid_group_create(ing_grp_iter->ifname, pp_cpu_info_ptr, LGM_CPU_PORT_COUNT);

			if(ing_grp_iter->pp_grp_id < 0) {
				goto QOS_MGR_RETURN_FAILURE;
			}
			else {
				for(if_iter = 0; if_iter < ing_grp_iter->if_count; if_iter++) {
					pp_gpid_group_add_port(ing_grp_iter->pp_grp_id, ing_grp_iter->gpids[if_iter], PP_GPID_GRP_LOWEST_PRIO);
				}
			}
		}
	}

	qos_mgr_free(pp_cpu_info_ptr);
	return QOS_MGR_SUCCESS;

QOS_MGR_RETURN_FAILURE:
	qos_mgr_free(pp_cpu_info_ptr);
	return QOS_MGR_FAILURE;
}
#endif

static int32_t qos_mgr_get_gpid_from_ifname(char* ifname, uint32_t* gpid)
{
	struct net_device *if_dev = NULL;
	dp_subif_t *dp_subif = NULL;
	if_dev = dev_get_by_name(&init_net, ifname);
	dp_subif = kzalloc(sizeof(dp_subif_t), GFP_ATOMIC);

	if (!dp_subif) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"The dp_subif is a NULL\n");
		goto QOS_MGR_RETURN_FAILURE;
	}

	if (dp_get_netif_subifid(if_dev, NULL, NULL, 0, dp_subif, 0) != QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"The get subifid failed\n");
		goto QOS_MGR_RETURN_FAILURE;
	}
	else {
		if(gpid == NULL)
			goto QOS_MGR_RETURN_FAILURE;

		*gpid = dp_subif->gpid;
		kfree(dp_subif);
		if (if_dev)
			dev_put(if_dev);

		return QOS_MGR_SUCCESS;
	}

QOS_MGR_RETURN_FAILURE:
	kfree(dp_subif);
	if (if_dev)
		dev_put(if_dev);

	return QOS_MGR_FAILURE;
}

int32_t qosal_set_qos_inggrp(QOS_MGR_CMD_INGGRP_INFO *qos_mgr_inggrp_info, QOS_INGGRP_LIST_ITEM **pp_item)
{
	struct ingress_config_info *ingress_config_info_instance = NULL;
	struct ingress_config_info *ingress_config_info_next = NULL;
	QOS_INGGRP_LIST_ITEM *p_item = NULL;
	QOS_MGR_CMD_INGGRP_INFO *inggrp_info = NULL;
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	QOS_MGR_IFNAME inggrp_name[QOS_MGR_IF_NAME_SIZE];
	QOS_INGGRP_LIST_ITEM *ptr_to_ing_group = NULL;
#endif
	int ifiter = 0;
	uint32_t gpid = 0;

	if (gIngressQosEnable == 0) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Ingress QoS is disabled in system\n");
		return QOS_MGR_SUCCESS;
	}

	parse_fapi_req(qos_mgr_inggrp_info);	
	dump_fapi_ingress_config();
	
	inggrp_info = (QOS_MGR_CMD_INGGRP_INFO *)qos_mgr_malloc(sizeof(QOS_MGR_CMD_INGGRP_INFO));
	if (inggrp_info == NULL)
		return QOS_MGR_FAILURE;

	for(ingress_config_info_instance = ingress_config_info_list;
		ingress_config_info_instance != NULL;
		ingress_config_info_instance=ingress_config_info_instance->next) {

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"GroupName:[%s] GroupId:[%d] Group Member Count:[%d] Ingress Option:[%d] Ingress Ifnames:[%s,%s]\n",
			ingress_config_info_instance->group_name,
			ingress_config_info_instance->group_id,
			ingress_config_info_instance->member_count,
			ingress_config_info_instance->ingress_option,
			ingress_config_info_instance->ifnames,
			ingress_config_info_instance->ifnames+QOS_MGR_IF_NAME_SIZE);

		memset(inggrp_info, 0, sizeof(QOS_MGR_CMD_INGGRP_INFO));
		qos_mgr_strncpy(inggrp_info->ifname, ingress_config_info_instance->group_name, QOS_MGR_IF_NAME_SIZE);
		inggrp_info->ingress_group = ingress_config_info_instance->group_id;
		inggrp_info->cpu_inggrp->ing_op = ingress_config_info_instance->ingress_option;
		inggrp_info->cpu_inggrp->if_cnt = ingress_config_info_instance->member_count;
		if(__qosal_set_qos_inggrp(inggrp_info, &p_item) == QOS_MGR_FAILURE) {
			qos_mgr_free(inggrp_info);
			return QOS_MGR_FAILURE;
		}

		p_item->if_count = ingress_config_info_instance->member_count;
		for(ifiter = 0; ifiter < ingress_config_info_instance->member_count; ifiter++) {
			strncpy(p_item->ifnames[ifiter], ingress_config_info_instance->ifnames+(ifiter*QOS_MGR_IF_NAME_SIZE), QOS_MGR_IF_NAME_SIZE);

			if(qos_mgr_get_gpid_from_ifname(p_item->ifnames[ifiter], &gpid) == QOS_MGR_SUCCESS)
				p_item->gpids[ifiter] = gpid;
		}
	}
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	snprintf(inggrp_name, QOS_MGR_IF_NAME_SIZE, "INGGRP_phigh");
	if (qos_mgr_inggrp_lookup(inggrp_name, &ptr_to_ing_group) != QOS_MGR_SUCCESS) {
		qos_mgr_free(inggrp_info);
		return QOS_MGR_FAILURE;
	}

	if(qos_mgr_update_inggrp_protected_queues(ptr_to_ing_group) != QOS_MGR_SUCCESS) {
		qos_mgr_free(inggrp_info);
		return QOS_MGR_FAILURE;
	}

	if(qos_mgr_create_pp_ing_groups() != QOS_MGR_SUCCESS) {
		qos_mgr_free(inggrp_info);
		return QOS_MGR_FAILURE;
	}
#endif
	for(ingress_config_info_instance = ingress_config_info_list;
	    ingress_config_info_instance != NULL;
	    ingress_config_info_instance = ingress_config_info_next) {
		ingress_config_info_next = ingress_config_info_instance->next;
		kfree(ingress_config_info_instance);
	}

	qos_mgr_free(inggrp_info);
	ingress_config_info_list = NULL;
	*pp_item = p_item;
	return QOS_MGR_SUCCESS;
}

void qos_mgr_dump_iface_ingress_info(QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index)
{
	uint8_t i,j,k;
	uint8_t group_queue_count_high, group_queue_count_low;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"---------------------- Ingress info [%s (gpid:%d)] ----------------------\n", qos_inggrp_list_index->ifname, qos_inggrp_list_index->gpid);
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"ingress_group:%d option_type:%d fapi_reg_status:%d dp_reg_status:%d queue_count_high:%d queue_count_low:%d\n", 
			qos_inggrp_list_index->ingress_group,
			qos_inggrp_list_index->option_type,
			qos_inggrp_list_index->fapi_reg_status,
			qos_inggrp_list_index->dp_reg_status,
			qos_inggrp_list_index->queue_count_high,
			qos_inggrp_list_index->queue_count_low);

	group_queue_count_high = qos_inggrp_list_index->queue_count_high/g_CPU_Count;
	group_queue_count_low = qos_inggrp_list_index->queue_count_low/g_CPU_Count;

	/* LOW PRIO CPU Port */
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Total queues created on each LOW PRIO CPU for group%d is %d\n",
			qos_inggrp_list_index->ingress_group,
			qos_inggrp_list_index->queue_count_low);

	for (j=0; j<group_queue_count_low; j++) {
		for (i=0; i<g_CPU_Count; i++) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[Port GPID:%d] CPU Type: LOW PRIO CPU Id:%d queue_index:%d qid:%d no_of_tc:%d\n",
					qos_inggrp_list_index->gpid,
					i, qos_inggrp_list_index->user_queue_low[i][j].queue_index,
					qos_inggrp_list_index->user_queue_low[i][j].qid,
					qos_inggrp_list_index->user_queue_low[i][j].no_of_tc);

			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"TC MAP =>");
			for (k = 0; k < MAX_TC_NUM; k++) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE," TC[%d]:%d", k,
						qos_inggrp_list_index->user_queue_low[i][j].qmap[k]);
			}
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"\n");
		}
	}

	/* HIGH PRIO CPU Port */
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Total queues created on each HIGH PRIO CPU for group%d is %d\n",
			qos_inggrp_list_index->ingress_group, 
			qos_inggrp_list_index->queue_count_high);
	for (j=0; j<group_queue_count_high; j++) {
		for (i=0; i<g_CPU_Count; i++) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[Port GPID:%d] CPU Type: HIGH PRIO CPU Id:%d queue_index:%d qid:%d no_of_tc:%d\n",
					qos_inggrp_list_index->gpid,
					i, qos_inggrp_list_index->user_queue_high[i][j].queue_index,
					qos_inggrp_list_index->user_queue_high[i][j].qid,
					qos_inggrp_list_index->user_queue_high[i][j].no_of_tc);
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"TC MAP =>");
			for (k = 0; k < MAX_TC_NUM; k++) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"TC[%d]:%d\n", k,
						qos_inggrp_list_index->user_queue_high[i][j].qmap[k]);
			}
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"\n");
		}
	}
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"---------------------------------------------------------------\n");
}

int32_t qos_mgr_clone_queue_info(QOS_INGGRP_LIST_ITEM *qos_inggrp_list_src_index,
                        QOS_INGGRP_LIST_ITEM *qos_inggrp_list_dst_index)
{
	int32_t ret = QOS_MGR_SUCCESS;

	qos_inggrp_list_dst_index->queue_count_low = qos_inggrp_list_src_index->queue_count_low;
	qos_inggrp_list_dst_index->queue_count_high = qos_inggrp_list_src_index->queue_count_high;
	memcpy(&qos_inggrp_list_dst_index->user_queue_low, &qos_inggrp_list_src_index->user_queue_low, sizeof(qos_inggrp_list_dst_index->user_queue_low));
	memcpy(&qos_inggrp_list_dst_index->user_queue_high, &qos_inggrp_list_src_index->user_queue_high, sizeof(qos_inggrp_list_dst_index->user_queue_high));

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s:%d: qos_inggrp_list_src_index=%p ifname:%s qos_inggrp_list_dst_index=%p ifname:%s\n",
			 __func__, __LINE__,
			qos_inggrp_list_src_index,
			qos_inggrp_list_src_index->ifname,
			qos_inggrp_list_dst_index,
			qos_inggrp_list_dst_index->ifname);

	qos_mgr_dump_iface_ingress_info(qos_inggrp_list_src_index);
	qos_mgr_dump_iface_ingress_info(qos_inggrp_list_dst_index);

	return ret;
}

int32_t qos_mgr_update_queue_info(QOS_MGR_CMD_QUEUE_INFO *q_info,
                        QOS_MGR_ADD_QUEUE_CFG *add_q_cfg,
                        QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index)
{
	int32_t ret = QOS_MGR_SUCCESS;
	uint8_t i,k,group_queue_index;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s:%d: qos_inggrp_list_index=%p ifname=%s, add_q_cfg.queue_id:%d CPU Dequeue PORT Type:%s\n",
			__func__, __LINE__,
			qos_inggrp_list_index,
			qos_inggrp_list_index->ifname,
			add_q_cfg->queue_id,
			(qos_inggrp_list_index->flags & QOS_MGR_Q_F_INGRESS_DEQ_PORT_HIGH) ? "HIGH" : "LOW");

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s] Ingress queue info dump before update ...\n", __func__);
	qos_mgr_dump_iface_ingress_info(qos_inggrp_list_index);

	if (q_info->flags & QOS_MGR_Q_F_INGRESS_DEQ_PORT_HIGH) { /* HIGH PRIO CPU Port */
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Update queue info for CPU DEQUEUE PORT HIGH ... \n");
		qos_inggrp_list_index->queue_count_high = qos_inggrp_list_index->queue_count_high + g_CPU_Count;
		group_queue_index = (qos_inggrp_list_index->queue_count_high/g_CPU_Count)-1;
		for (i=0; i<g_CPU_Count; i++) {
			for (k = 0; k < MAX_TC_NUM; k++) {
				qos_inggrp_list_index->user_queue_high[i][group_queue_index].qmap[k] = q_info->tc_map[k];
			}

			qos_inggrp_list_index->user_queue_high[i][group_queue_index].no_of_tc = q_info->tc_no;
			qos_inggrp_list_index->user_queue_high[i][group_queue_index].queue_index = add_q_cfg->queue_id;
			qos_inggrp_list_index->user_queue_high[i][group_queue_index].qid = qos_hal_get_logical_qid(add_q_cfg->queue_id, (i*2), NULL,  q_info->flags, LOGICAL_QID_FLAG);
		}
	} else { /* LOW PRIO CPU Port */
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Update queue info for CPU DEQUEUE PORT LOW ... \n");
		qos_inggrp_list_index->queue_count_low = qos_inggrp_list_index->queue_count_low + g_CPU_Count;
		group_queue_index = (qos_inggrp_list_index->queue_count_low/g_CPU_Count)-1;
		for (i=0; i<g_CPU_Count; i++) {
			for (k = 0; k < MAX_TC_NUM; k++) {
				qos_inggrp_list_index->user_queue_low[i][group_queue_index].qmap[k] = q_info->tc_map[k];
			}

			qos_inggrp_list_index->user_queue_low[i][group_queue_index].no_of_tc = q_info->tc_no;
			qos_inggrp_list_index->user_queue_low[i][group_queue_index].queue_index = add_q_cfg->queue_id;
			qos_inggrp_list_index->user_queue_low[i][group_queue_index].qid = qos_hal_get_logical_qid(add_q_cfg->queue_id, (1+(i*2)), NULL,  q_info->flags, LOGICAL_QID_FLAG);
		}
	}
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s] Ingress queue info dump after update ...\n", __func__);
	qos_mgr_dump_iface_ingress_info(qos_inggrp_list_index);

	return ret;
}

static void qos_mgr_init_hif_datapath(struct dp_hif_datapath *dp)
{
	int i;

	dp->color = PP_COLOR_INVALID;

	for (i = 0; i < ARRAY_SIZE(dp->sgc); i++)
		dp->sgc[i] = PP_SGC_INVALID;

	for (i = 0; i < ARRAY_SIZE(dp->tbm); i++)
		dp->tbm[i] = PP_TBM_INVALID;

	for (i = 0; i < ARRAY_SIZE(dp->eg); i++) {
		dp->eg[i].q_id = PP_QOS_INVALID_ID;
		dp->eg[i].cpu_gpid = PP_PORT_INVALID;
	}
}

int32_t qos_mgr_hostif_update(QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index)
{
	int32_t ret = QOS_MGR_SUCCESS;
	uint32_t i, j, k, group_queue_count, prio = 0;
	struct dp_hif_datapath dp[LGM_INGRESS_MAX_PRIO] = {0};

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s:%d] ifname:[%s] Ingress Group:[%d] Queue Count => LOW PRIO CPU :[%d] HIGH PRIO CPU:[%d] fapi_reg_status:%d dp_reg_status:%d\n",
			__func__, __LINE__,
			qos_inggrp_list_index->ifname,
			qos_inggrp_list_index->ingress_group,
			qos_inggrp_list_index->queue_count_low,
			qos_inggrp_list_index->queue_count_high,
			qos_inggrp_list_index->fapi_reg_status,
			qos_inggrp_list_index->dp_reg_status);

	if ((qos_inggrp_list_index->fapi_reg_status == 1)
			&& (qos_inggrp_list_index->dp_reg_status == 1)) {

		/* Hostif update for LOW PRIO CPU DEQUEUE Port */
		group_queue_count = qos_inggrp_list_index->queue_count_low/g_CPU_Count;
		for (j=0; j<group_queue_count; j++) {
			for (k=0; k<qos_inggrp_list_index->user_queue_low[0][j].no_of_tc; k++) {
				prio = qos_inggrp_list_index->user_queue_low[0][j].qmap[k];
				qos_mgr_init_hif_datapath(&dp[prio]);
				dp[prio].color = PP_COLOR_GREEN;

				for (i = 0; i <g_CPU_Count; i++) {
					dp[prio].eg[i].q_id = qos_inggrp_list_index->user_queue_low[i][j].qid; /* DP Physical QiD */
					dp[prio].eg[i].cpu_gpid = 17+(i*2); /* CPU GPID */ /* TODO */

					QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"dp[%d].eg[%d].qos_q:%d dp[%d].eg[%d].pid:%d\n",
							prio, i, dp[prio].eg[i].q_id, prio, i, dp[prio].eg[i].cpu_gpid);
				}
			}
		}

		/* Hostif update for HIGH PRIO CPU DEQUEUE Port */
		group_queue_count = qos_inggrp_list_index->queue_count_high/g_CPU_Count;
		for (j=0; j<group_queue_count; j++) {
			for (k=0; k<qos_inggrp_list_index->user_queue_high[0][j].no_of_tc; k++) {
				prio = qos_inggrp_list_index->user_queue_high[0][j].qmap[k];
				qos_mgr_init_hif_datapath(&dp[prio]);
				dp[prio].color = PP_COLOR_GREEN;

				for (i = 0; i < g_CPU_Count; i++) {
					dp[prio].eg[i].q_id = qos_inggrp_list_index->user_queue_high[i][j].qid; /* DP Physical QiD */
					dp[prio].eg[i].cpu_gpid = 16+(i*2); /* cpu gpid */ /* TODO */
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"dp[%d].eg[%d].qos_q:%d dp[%d].eg[%d].pid:%d\n",
							prio, i, dp[prio].eg[i].q_id, prio, i, dp[prio].eg[i].cpu_gpid);
				}
			}
		}
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 0)
		if (unlikely(dp_hostif_update(0, qos_inggrp_list_index->dpid, qos_inggrp_list_index->vap, dp))) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"dp_hostif_update fail:gpid=%u for %dth queue\n", qos_inggrp_list_index->gpid, j);
			ret = QOS_MGR_FAILURE;
		} else {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"dp_hostif_update success:gpid=%u for %dth queue\n", qos_inggrp_list_index->gpid, j);
			ret = QOS_MGR_SUCCESS;
		}
#endif
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface %s is not registerd to Datapath, Hostif update will be done during DP Registration event.\n", qos_inggrp_list_index->ifname);
	}

	return ret;
}

static void qos_mgr_populate_ingress_q_config(QOS_MGR_CMD_QUEUE_INFO *q_info,
		QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index,
		bool dequeue_port)
{
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Ingress option:[%d] Dequeue Port Type:[%d]\n", qos_inggrp_list_index->option_type, dequeue_port);

	snprintf(q_info->ifname, QOS_MGR_IF_NAME_SIZE, "%s", qos_inggrp_list_index->ifname);
	q_info->sched = QOS_MGR_SCHED_SP;
	q_info->flags |= QOS_MGR_Q_F_INGRESS;

	if (qos_inggrp_list_index->option_type == INGRESS_OPTION_0) {
		if (dequeue_port == CPU_LOW) {
			q_info->priority = 1;
			q_info->enable = 1;
			q_info->portid = 0;
			q_info->tc_no = 2;
			q_info->tc_map[0] = 0;
			q_info->tc_map[1] = 1;
			q_info->queue_num = 1;
			q_info->flags |= QOS_MGR_Q_F_INGRESS_DEQ_PORT_LOW;
		} else { /* CPU_HIGH */
			q_info->priority = 1;
			q_info->enable = 1;
			q_info->portid = 0;
			q_info->tc_no = 2;
			q_info->tc_map[0] = 2;
			q_info->tc_map[1] = 3;
			q_info->queue_num = 1;
			q_info->flags |= QOS_MGR_Q_F_INGRESS_DEQ_PORT_HIGH;
		}
	} else if (qos_inggrp_list_index->option_type == INGRESS_OPTION_1) {
		if (dequeue_port == CPU_LOW) {
			q_info->priority = 1;
			q_info->enable = 1;
			q_info->portid = 0;
			q_info->tc_no = 2;
			q_info->tc_map[0] = 0;
			q_info->tc_map[1] = 1;
			q_info->queue_num = 1;
			q_info->flags |= QOS_MGR_Q_F_INGRESS_DEQ_PORT_LOW;
		}
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Invalid Ingress option:[%d]\n", qos_inggrp_list_index->option_type);
	}

	return;
}

int32_t qos_mgr_add_ingress_queue_single_iface(QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index)
{
	int32_t ret = QOS_MGR_SUCCESS;
	QOS_MGR_CMD_QUEUE_INFO q_info = {0};
	QOS_QUEUE_LIST_ITEM *p_item;
	uint8_t phigh_iter = 0;

	if ((qos_inggrp_list_index->fapi_reg_status == 1)
			&& (qos_inggrp_list_index->dp_reg_status == 1)) {
		if (strcmp(qos_inggrp_list_index->ifname, "INGGRP_phigh") == 0) {
			memset(&q_info, 0, sizeof(q_info));
			for(phigh_iter = 0; phigh_iter < MAX_PHP_QUEUES_PER_HIGH_PORT; phigh_iter++) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s:%d] Adding queue to LOW prio ...\n", __func__, __LINE__);

				q_info.priority = phigh_iter + 1;
				q_info.enable = 1;
				q_info.portid = 0;
				strcpy(q_info.ifname, qos_inggrp_list_index->ifname);
				q_info.queue_num = 1;
				q_info.flags |= QOS_MGR_Q_F_INGRESS_DEQ_PORT_HIGH;
				q_info.flags |= QOS_MGR_Q_F_INGRESS;

				if (qosal_add_qos_queue(&q_info, &p_item) != QOS_MGR_SUCCESS) {
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed to create Ingress Queues with q_prio:0x%x for %s\n", q_info.priority, q_info.ifname);
					return QOS_MGR_FAILURE;
				}

				if (p_item->p_entry < 0) {
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Add Queue failed\n");
					return QOS_MGR_FAILURE;
				}

				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Returned Queue Id: [%d]\n",
						__func__, p_item->p_entry);
			}
		}
		else {
			/* Add new queues to LOW CPU PORT */
			memset(&q_info, 0, sizeof(q_info));
			qos_mgr_populate_ingress_q_config(&q_info, qos_inggrp_list_index, CPU_LOW);

			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s:%d] Adding queue to LOW prio ...\n", __func__, __LINE__);
			if (qosal_add_qos_queue(
						&q_info,
						&p_item) != QOS_MGR_SUCCESS) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed to create Ingress Queues with q_prio:0x%x for %s\n", q_info.priority, q_info.ifname);
				return QOS_MGR_FAILURE;
			}

			if (p_item->p_entry < 0) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Add Queue failed\n");
				return QOS_MGR_FAILURE;
			}

			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Returned Queue Id: [%d]\n",
					__func__, p_item->p_entry);
		}

		if (qos_inggrp_list_index->option_type == INGRESS_OPTION_0) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"[%s:%d] Adding queue to HIGH prio ...\n", __func__, __LINE__);
			/* Add new queues to HIGH CPU PORT */
			memset(&q_info, 0, sizeof(q_info));
			qos_mgr_populate_ingress_q_config(&q_info, qos_inggrp_list_index, CPU_HIGH);

			if (qosal_add_qos_queue(
						&q_info,
						&p_item) != QOS_MGR_SUCCESS) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed to create Ingress Queues with q_prio:0x%x for %s\n", q_info.priority, q_info.ifname);
				return QOS_MGR_FAILURE;
			}

			if (p_item->p_entry < 0) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Add Queue failed\n");
				return QOS_MGR_FAILURE;
			}

			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Returned Queue Id: [%d]\n",
					__func__, p_item->p_entry);

		} else if (qos_inggrp_list_index->option_type == INGRESS_OPTION_1) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"INGRESS_OPTION_1: Queues created by DPM in High Prio Dequeue port will be used.\n");
		} else {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"TODO: Unknown INGRESS_OPTION [%d]. \n", qos_inggrp_list_index->option_type);
		}
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface %s is not registerd to Datapath, Hostif update will be done during DP Registration event.\n", qos_inggrp_list_index->ifname);
	}
	return ret;
}

int32_t qos_mgr_handle_new_ingrp_iface(QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index)
{
	int32_t ret = QOS_MGR_SUCCESS;
	bool match_found = false;
	QOS_INGGRP_LIST_ITEM *p;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s:%d: lookup ingress_group=%d ifname=%s\n",
			__func__, __LINE__, qos_inggrp_list_index->ingress_group, qos_inggrp_list_index->ifname);

	if ((qos_inggrp_list_index->fapi_reg_status == 1)
			&& (qos_inggrp_list_index->dp_reg_status == 1)) {
		qos_mgr_inggrp_lock_list();

		for ( p = g_qos_inggrp_list; p; p = (QOS_INGGRP_LIST_ITEM *)p->next ) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s:%d: entry %p: ingress group=%d, ifname=%s queue count=[high:%d, low:%d] \n",
					__func__,__LINE__, p, p->ingress_group, p->ifname, p->queue_count_high, p->queue_count_low); 
			if ((p->ingress_group == qos_inggrp_list_index->ingress_group) &&
					((p->queue_count_low > 0) || (p->queue_count_high > 0))) {
				match_found = true;
				break;
			}
		}

		if (match_found) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Ingress queues for INGROUP%d are already created against interface %s.\n",
					qos_inggrp_list_index->ingress_group, p->ifname);
			qos_mgr_clone_queue_info(p, qos_inggrp_list_index);
			qos_mgr_inggrp_unlock_list();
		} else {
			qos_mgr_inggrp_unlock_list();

			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Ingress queues for INGROUP%d doesnot exists, creating against interface %s...\n",
					qos_inggrp_list_index->ingress_group,
					qos_inggrp_list_index->ifname);

			qos_mgr_add_ingress_queue_single_iface(qos_inggrp_list_index);
		}
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface %s is not registerd to Datapath, Hostif update will be done during DP Registration event.\n", qos_inggrp_list_index->ifname);
	}

	return ret;
}

int32_t qos_manager_dp_event_handler_ingress(char *ifname, uint32_t gpid, uint32_t dpid, int vap,
						enum DP_EVENT_TYPE type)
{
	int ret = QOS_MGR_SUCCESS;
	QOS_INGGRP_LIST_ITEM *p = NULL;
	int ifiter = 0;

	switch (type) {
	case DP_EVENT_REGISTER_SUBIF:
		qos_mgr_get_ptrtoinggrp_from_ifname(ifname, &p, &ifiter);

		if (p == NULL)
			goto QOS_MGR_EXIT;

		p->gpids[ifiter] = gpid;
		if (p->pp_grp_id > 0) {
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
			if (pp_gpid_group_add_port(p->pp_grp_id, p->gpids[ifiter],
				PP_GPID_GRP_LOWEST_PRIO) != QOS_MGR_SUCCESS) {
					ret = QOS_MGR_FAILURE;
					goto QOS_MGR_EXIT;
			} else {
				ret = QOS_MGR_SUCCESS;
			}
#endif
		} else {
			return QOS_MGR_FAILURE;
		}

		break;

	case DP_EVENT_DE_REGISTER_SUBIF:
		qos_mgr_get_ptrtoinggrp_from_ifname(ifname, &p, &ifiter);

		if (p == NULL)
			goto QOS_MGR_EXIT;

		if (p->pp_grp_id > 0) {
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
			if (pp_gpid_group_del_port(p->pp_grp_id, gpid) != QOS_MGR_SUCCESS) {
				ret = QOS_MGR_FAILURE;
				goto QOS_MGR_EXIT;
			} else {
				p->gpids[ifiter] = 0;
				ret = QOS_MGR_SUCCESS;
			}
#endif
		} else {
			return QOS_MGR_FAILURE;
		}
		break;
	default:
		/* Do nothing */
		break;
	}

QOS_MGR_EXIT:
	return ret;
}

int32_t __qosal_set_qos_inggrp(QOS_MGR_CMD_INGGRP_INFO *inggrp_info, QOS_INGGRP_LIST_ITEM **pp_item)
{
	QOS_INGGRP_LIST_ITEM *p_item = NULL;
	QOS_MGR_INGGRP_CFG hcfg;
	uint32_t ret=QOS_MGR_SUCCESS;
	struct net_device *if_dev = NULL;
	dp_subif_t *dp_subif = NULL;
	struct dp_port_prop port_prop;

	if (inggrp_info == NULL) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "inggrp_info is NULL\n");
		return QOS_MGR_FAILURE;
	}

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: add %s to inggrp %d\n", __func__, __LINE__,
			inggrp_info->ifname, inggrp_info->ingress_group);

	/* First, check if interface is already added */
	if (qos_mgr_inggrp_lookup(inggrp_info->ifname, &p_item) == QOS_MGR_SUCCESS) {
		/* interface found in inggrp list - modify ingress group */
		if (QOS_MGR_INGGRP_VALID(inggrp_info->ingress_group) && inggrp_info->ingress_group != p_item->ingress_group) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s:%d: WARNING: modify %s ingress group %d -> %d (won't affect already active queues!)\n",
							__func__, __LINE__, inggrp_info->ifname, p_item->ingress_group, inggrp_info->ingress_group);
			p_item->ingress_group = inggrp_info->ingress_group;
		}
		qos_mgr_inggrp_free_item(p_item);
	} else if (QOS_MGR_INGGRP_INVALID(inggrp_info->ingress_group)) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s:%d: ERROR: invalid ingress group %d (%s)\n",
								__func__, __LINE__, inggrp_info->ingress_group, inggrp_info->ifname);
			return QOS_MGR_FAILURE;
	}


	memset(&hcfg, 0, sizeof(hcfg));
	hcfg.ingress_group = inggrp_info->ingress_group;
	qos_mgr_strncpy(hcfg.ifname, inggrp_info->ifname, QOS_MGR_IF_NAME_SIZE);
	if (qos_mgr_set_qos_inggrp(&hcfg, QOS_HAL) != QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s:%d: failed to set Ingress Grouping for HAL id %d\n", __func__, __LINE__, QOS_HAL);
		return QOS_MGR_FAILURE;
	}

	if (!p_item) {
		/* Allocate a new node and add to the inggrp list */
		p_item = qos_mgr_inggrp_alloc_item();
		if (!p_item) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s:%d: alloc qos inggrp list item failed \n", __func__, __LINE__);
			return QOS_MGR_ENOMEM;
		}

		qos_mgr_strncpy(p_item->ifname, inggrp_info->ifname, QOS_MGR_IF_NAME_SIZE);
		p_item->ingress_group = inggrp_info->ingress_group;
		p_item->fapi_reg_status = true;
		p_item->option_type = inggrp_info->cpu_inggrp->ing_op;
		p_item->if_count = inggrp_info->cpu_inggrp->if_cnt;
		p_item->pp_grp_id = -1;
		p_item->queue_count_low = 0;
		p_item->queue_count_high = 0;
		if_dev = dev_get_by_name(&init_net, inggrp_info->ifname);
		if (if_dev == NULL) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "Interface %s not present in system.\n", inggrp_info->ifname);
		}
		dp_subif = kzalloc(sizeof(dp_subif_t), GFP_ATOMIC);
		if (!dp_subif) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "<%s> DP subif allocation failed\n",__func__);
			qos_mgr_inggrp_free_item(p_item); /* decrement reference counter */
			return QOS_MGR_FAILURE;
		}

		if (dp_get_netif_subifid(if_dev, NULL, NULL, 0, dp_subif, 0) != QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %s dp_get_netif_subifid failed\n",
				 __func__, NETDEV_NAME(if_dev));
			p_item->dp_reg_status = false;
		} else {
			p_item->dp_reg_status = true;
			p_item->gpid = dp_subif->gpid;
			p_item->dpid = dp_subif->port_id;

			if (dp_get_port_prop(dp_subif->inst, dp_subif->port_id, &port_prop)) {
				kfree(dp_subif);
				qos_mgr_inggrp_free_item(p_item);
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "dp_get_port_prop fail\n");
				return QOS_MGR_FAILURE;
			}
			p_item->vap = GET_VAP(dp_subif->subif, port_prop.vap_offset, port_prop.vap_mask);
		}
		/* dp_reg_status is always set to true, since the queue creation on CPU port is independent of the ingress interface */
		p_item->dp_reg_status = true;
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: Add item %p ifname=%s, inggrp=%d GPID=%d DPID=%d vap=%d\n", __func__, __LINE__,
				p_item, p_item->ifname, p_item->ingress_group,  p_item->gpid, p_item->dpid, p_item->vap);
		__qos_mgr_inggrp_add_item(p_item);
		
		qos_mgr_handle_new_ingrp_iface(p_item);
		kfree(dp_subif);
		if (if_dev)
			dev_put(if_dev);
	}

	/* success */
	*pp_item = p_item;
	return ret;
}

static uint32_t qos_mgr_set_ingress_qos_generic(const char *ifname, uint32_t *flags)
{
	QOS_INGGRP_LIST_ITEM *p_item;
	uint32_t ret=QOS_MGR_SUCCESS;

	if ((*flags & QOS_MGR_Q_F_INGRESS) || (*flags & QOS_MGR_OP_F_INGRESS))	{
		ret = qos_mgr_inggrp_lookup(ifname, &p_item);
		if (ret) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s:%d: ifname %s does not exist in qos inggrp list\n", __func__, __LINE__, ifname);
			return ret;
		}
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s:%d: set ifname %s to ingress_group %d\n", __func__, __LINE__,
				ifname, p_item->ingress_group);
		*flags |= inggrp2flags(p_item->ingress_group);
		qos_mgr_inggrp_free_item(p_item); /* decrement reference counter */
	}

	return QOS_MGR_SUCCESS;
}

int32_t qos_mgr_ioctl_set_qos_ingress_group(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA *cmd_info)
{
	QOS_INGGRP_LIST_ITEM *p_item;
	int res = QOS_MGR_FAILURE;

	res = qos_mgr_copy_from_user(&cmd_info->qos_inggrp_info, (void *)arg, sizeof(cmd_info->qos_inggrp_info));
	if (res != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

	if (qosal_set_qos_inggrp(&cmd_info->qos_inggrp_info, &p_item) != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

	return QOS_MGR_SUCCESS;
}
EXPORT_SYMBOL(qos_mgr_ioctl_set_qos_ingress_group);

int32_t qos_mgr_ioctl_get_qos_ingress_group(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA *cmd_info)
{
	/* Place Holder */
	return QOS_MGR_SUCCESS;
}
EXPORT_SYMBOL(qos_mgr_ioctl_get_qos_ingress_group);


/*
############# SUPPORTING INGRESS FUNCTION DEFINITIONS : END################
*/

/*
############# SUPPORTING WMM FUNCTION DEFINITIONS : START################
*/
#if DP_EVENT_CB
/* Notify callback registration ID (pointer) - used for deregister CB */
static void *g_notify_cb_id;
int32_t g_wmm_1;

#define WMM_ADD 1
#define WMM_ENABLED 1

bool g_qos_mgr_wmm_enable_flag = WMM_ENABLED;

struct wlan_iface_wmm_info {
	char ifname[QOS_MGR_IF_NAME_SIZE];
	bool iface_reg_status;
	bool queues_present;
	struct list_head list;
};

static LIST_HEAD(g_wlan_iface_wmm_info_list);

void qos_hal_global_wmm_status(struct seq_file *seq)
{
	struct wlan_iface_wmm_info *p, *n;

	seq_printf(seq, "-------------------------------\n");
	seq_printf(seq, "WMM Configuration in system:-\n");
	seq_printf(seq, "-------------------------------\n");
	seq_printf(seq, "WMM Functionality Status: %s\n", g_qos_mgr_wmm_enable_flag ? "Enable":"Disable");
	list_for_each_entry_safe(p, n, &g_wlan_iface_wmm_info_list, list) {
		seq_printf(seq, "<%s> Ifname:[%s] Reg Status:[%s] Queues Present:[%s]\n", __func__,
				p->ifname,
				p->iface_reg_status ? "Registered" : "Un-Registered",
				p->queues_present ? "Yes" : "No");
	}
	seq_printf(seq, "-------------------------------\n");

	return;
}

int32_t qos_hal_global_wmm_enable_disable(bool oper)
{
	int32_t ret = QOS_MGR_SUCCESS;
	struct wlan_iface_wmm_info *p, *n;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"WMM Functionality Status: Existing:[%s] New:[%s]\n",
			g_qos_mgr_wmm_enable_flag ? "Enable":"Disable",
			oper ? "Enable":"Disable");

	if (oper == g_qos_mgr_wmm_enable_flag) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"No change in WMM Functionality Status.\n");
		return ret;
	}

	g_qos_mgr_wmm_enable_flag = oper;

	list_for_each_entry_safe(p, n, &g_wlan_iface_wmm_info_list, list) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Ifname:[%s] Reg Status:[%s] Queues Present:[%s]\n", __func__,
				p->ifname,
				p->iface_reg_status ? "Registered" : "Un-Registered",
				p->queues_present ? "Yes" : "No");

		if (p->iface_reg_status == 1) {
			if (oper == WMM_ADD) {
				if (qos_hal_wmm_add(p->ifname) == QOS_MGR_SUCCESS)
					p->queues_present = 1;
			} else {
				if (p->queues_present) {
					if (qos_hal_wmm_del(p->ifname) == QOS_MGR_SUCCESS)
						p->queues_present = 0;
				}
			}
		}
	}

	return ret;
}

static void qos_manager_update_wmm_info (
	struct wlan_iface_wmm_info *wlan_iface_wmm_info,
	char *ifname,
	bool iface_reg_status,
	bool queues_present)
{
	wlan_iface_wmm_info->queues_present = queues_present;
	wlan_iface_wmm_info->iface_reg_status = iface_reg_status;
	snprintf(wlan_iface_wmm_info->ifname, QOS_MGR_IF_NAME_SIZE, ifname);

	return;
}

/* 	WLAN Should pass one flag which identifies 
	interface as base interface which doesnot 
	participate in datapath activity. Based on that
	creation of WMM Queues will be avoided in qos_mgr.
	Until this feature is avialable from WLAN,
	is_wlan_base_vap() is used as workaround */
static bool is_wlan_base_vap(struct dp_event_info *info)
{
	int vap;
	struct dp_port_prop port_prop = {0};

	if (dp_get_port_prop(info->inst,
			info->reg_subif_info.dpid, &port_prop)) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "dp_get_port_prop fail\n");
		return 0;
	}
	vap = GET_VAP(info->reg_subif_info.subif,
		port_prop.vap_offset, port_prop.vap_mask);

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,
			"vap:%d port_prop->subif_max:%d DPID:%d subif:%d\n",
			vap, port_prop.subif_max,
			info->reg_subif_info.dpid, info->reg_subif_info.subif);

	if ((port_prop.alloc_flags & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT))
			&& (vap == (port_prop.subif_max-1)))
		return 1;
	else
		return 0;
}

int32_t qos_manager_dp_event_handler(struct dp_event_info *info)
{
	int32_t ret = QOS_MGR_SUCCESS;
	int vap;
	dp_subif_t *dp_subif = NULL;
	struct dp_port_prop port_prop;
	struct wlan_iface_wmm_info *p, *n;
	struct wlan_iface_wmm_info *wlan_iface_wmm_info = NULL;

	if (info->reg_subif_info.dpid < 0)
		return QOS_MGR_SUCCESS;

	if (is_wlan_base_vap(info)) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,
			"WMM Queue creation is skipped for Base VAP [%s]\n",
			 info->reg_subif_info.dev->name);
		return QOS_MGR_SUCCESS;
	}

	switch (info->type) {
	case DP_EVENT_REGISTER_SUBIF:
	{
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Register subif:=> netdevice:[%s] subif info:[%d:%d]\n",
				info->reg_subif_info.dev->name,
				info->reg_subif_info.dpid,
				info->reg_subif_info.subif);
		dp_subif = kzalloc(sizeof(dp_subif_t), GFP_ATOMIC);
		if (!dp_subif) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"<%s> DP subif allocation failed\n", __func__);
			return QOS_MGR_FAILURE;
		}

		if (dp_get_netif_subifid(info->reg_subif_info.dev, NULL, NULL, 0, dp_subif, 0) != QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %s dp_get_netif_subifid failed\n",
				__func__, NETDEV_NAME(info->reg_subif_info.dev));
		}

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s>Alloc Flag: 0x%x  dp_subif->alloc_flag & DP_F_FAST_WLAN:0x%x DP_F_FAST_WLAN:0x%x \n",
				__func__, dp_subif->alloc_flag,
				dp_subif->alloc_flag & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT),
				DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT);

		if ((dp_subif->alloc_flag & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT)) &&
				(g_qos_mgr_wmm_enable_flag == WMM_ENABLED)) {

			list_for_each_entry_safe(p, n, &g_wlan_iface_wmm_info_list, list) {
				if (strcmp(p->ifname, info->reg_subif_info.dev->name) == 0) {
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface [%s] exists in list.\n", info->reg_subif_info.dev->name);
					wlan_iface_wmm_info = p;
					break;
				}
			}

			if (wlan_iface_wmm_info == NULL) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"New interface [%s] registered.\n", info->reg_subif_info.dev->name);
				wlan_iface_wmm_info = qos_mgr_malloc(sizeof(struct wlan_iface_wmm_info));
				if (wlan_iface_wmm_info == NULL) {
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"%s:%d qos_mgr_malloc failed.\n", __func__, __LINE__);
					kfree(dp_subif);
					return QOS_MGR_FAILURE;
				}
				list_add(&wlan_iface_wmm_info->list, &g_wlan_iface_wmm_info_list);
			}

			if (qos_hal_wmm_add(info->reg_subif_info.dev->name) == QOS_MGR_SUCCESS)
				qos_manager_update_wmm_info(wlan_iface_wmm_info, info->reg_subif_info.dev->name, 1, 1);
			else
				qos_manager_update_wmm_info(wlan_iface_wmm_info, info->reg_subif_info.dev->name, 1, 0);
		}
		if (gIngressQosEnable == 1) {
			if (dp_get_port_prop(dp_subif->inst, dp_subif->port_id, &port_prop)) {
				kfree(dp_subif);
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "dp_get_port_prop fail\n");
				return QOS_MGR_FAILURE;
			}
			vap = GET_VAP(dp_subif->subif, port_prop.vap_offset, port_prop.vap_mask);

			if (qos_manager_dp_event_handler_ingress(info->reg_subif_info.dev->name,
				dp_subif->gpid, dp_subif->port_id, vap, DP_EVENT_REGISTER_SUBIF) !=
				QOS_MGR_SUCCESS) {
				kfree(dp_subif);
				return QOS_MGR_FAILURE;
			}
		}
		kfree(dp_subif);
		break;
	}
	case DP_EVENT_DE_REGISTER_SUBIF:
	{

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"De-Register subif:=> netdevice:[%s] subif info:[%d:%d]\n",
				info->de_reg_subif_info.dev->name,
				info->de_reg_subif_info.dpid,
				info->de_reg_subif_info.subif);

		dp_subif = kzalloc(sizeof(dp_subif_t), GFP_ATOMIC);
		if (!dp_subif) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"<%s> DP subif allocation failed\n", __func__);
			return QOS_MGR_FAILURE;
		}
		if (dp_get_netif_subifid(info->reg_subif_info.dev, NULL, NULL, 0, dp_subif, 0) != QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %s dp_get_netif_subifid failed\n",
				__func__, NETDEV_NAME(info->reg_subif_info.dev));
		}

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s>Alloc Flag: 0x%x  dp_subif->alloc_flag & DP_F_FAST_WLAN:0x%x DP_F_FAST_WLAN:0x%x \n",
				__func__, dp_subif->alloc_flag,
				dp_subif->alloc_flag & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT),
				DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT);

		if ((dp_subif->alloc_flag & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT)) &&
				(g_qos_mgr_wmm_enable_flag == WMM_ENABLED)) {

			list_for_each_entry_safe(p, n, &g_wlan_iface_wmm_info_list, list) {
				if (strcmp(p->ifname, info->reg_subif_info.dev->name) == 0) {
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface [%s] exists in list.\n", info->reg_subif_info.dev->name);
					wlan_iface_wmm_info = p;
					break;
				}
			}

			if (!wlan_iface_wmm_info) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Interface [%s] is not registered.\n", info->reg_subif_info.dev->name);
			} else {
				 QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "Interface [%s] is registered.\n", info->reg_subif_info.dev->name);
				if (wlan_iface_wmm_info->queues_present) {
					if (qos_hal_wmm_del(info->reg_subif_info.dev->name) == QOS_MGR_SUCCESS) {
						qos_manager_update_wmm_info(wlan_iface_wmm_info, info->reg_subif_info.dev->name, 0, 0);
					} else {
						QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"WMM Queue deletion for interface [%s] is failed.\n", info->reg_subif_info.dev->name);
						qos_manager_update_wmm_info(wlan_iface_wmm_info, info->reg_subif_info.dev->name, 0, 1);
					}
				} else {
					 QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "Interface [%s] is registered but queues not present.\n", info->reg_subif_info.dev->name);
				}

				list_del(&wlan_iface_wmm_info->list);
				qos_mgr_free(wlan_iface_wmm_info);
			}
		}

		if (gIngressQosEnable == 1) {
			if (dp_get_port_prop(dp_subif->inst, dp_subif->port_id, &port_prop)) {
				kfree(dp_subif);
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "dp_get_port_prop fail\n");
				return QOS_MGR_FAILURE;
			}
			vap = GET_VAP(dp_subif->subif, port_prop.vap_offset, port_prop.vap_mask);

			if (qos_manager_dp_event_handler_ingress(info->reg_subif_info.dev->name,
				dp_subif->gpid, dp_subif->port_id, vap, DP_EVENT_DE_REGISTER_SUBIF)
				!= QOS_MGR_SUCCESS) {
				kfree(dp_subif);
				return QOS_MGR_FAILURE;
			}
		}

		kfree(dp_subif);
		break;
	}
	case DP_EVENT_REGISTER_DEV:
	{
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Register device\n");
		break;
	}
	case DP_EVENT_DE_REGISTER_DEV:
	{
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"De- Register device\n");
		break;
	}
	case DP_EVENT_ALLOC_PORT:
	{
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Alloc Port\n");
		break;	
	}
	default:
	return 0;
	}
	return ret;
}

int32_t qos_manager_register_event_dp(void)
{
	int32_t ret = QOS_MGR_SUCCESS;
	struct dp_event dp_event = {0};

	dp_event.owner = DP_EVENT_OWNER_PPA;
	dp_event.type = DP_EVENT_REGISTER_SUBIF | DP_EVENT_DE_REGISTER_SUBIF;
	dp_event.dp_event_cb = qos_manager_dp_event_handler;

	ret = dp_register_event_cb(&dp_event, 0);
	if (ret != DP_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Can't register DP_EVENT callback\n");
		return ret;
	}

	/* Save callback ID for deregistration purpose */
	g_notify_cb_id = dp_event.id;

	return ret;
}

int32_t qos_manager_deregister_event_dp(void)
{
	int32_t ret = QOS_MGR_SUCCESS;
	struct dp_event dp_event = {0};

	dp_event.owner = DP_EVENT_OWNER_PPA;
	dp_event.type = DP_EVENT_REGISTER_SUBIF | DP_EVENT_DE_REGISTER_SUBIF;
	dp_event.id = g_notify_cb_id;
	dp_event.dp_event_cb = qos_manager_dp_event_handler;

	ret = dp_register_event_cb(&dp_event, DP_F_DEREGISTER);
	if (ret != DP_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Can't de-register DP_EVENT callback\n");
		return ret;
	}

	return ret;
}

void qos_mgr_wmm_free_list(void)
{
	struct wlan_iface_wmm_info *p, *n;

	list_for_each_entry_safe(p, n, &g_wlan_iface_wmm_info_list, list) {
		list_del(&p->list);
		qos_mgr_free(p);
	}
}
#endif

static int32_t qos_hal_lro_queue_add(uint32_t shaper_in_kbps)
{
	int32_t lroqid = QOS_MGR_FAILURE;
	QOS_MGR_CMD_QUEUE_INFO lro_q_info = {0};
	QOS_MGR_ADD_SHAPER_CFG shaper_cfg = {0};
	QOS_QUEUE_LIST_ITEM *p_item;

	printk("Shaper Value:%d\n", shaper_in_kbps);

	/* Create queue on LRO */
	lro_q_info.priority = 1;
	lro_q_info.enable = 1;
	lro_q_info.tc_no = 1;
	lro_q_info.tc_map[0] = 7;
	lro_q_info.portid = 0; 
	lro_q_info.sched = QOS_MGR_SCHED_SP;
	lro_q_info.queue_num = 1;
	lro_q_info.flags |= QOS_MGR_Q_F_SPCL_CONN;

	if (qosal_add_qos_queue(&lro_q_info, &p_item) != QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Failed to create LRO Queue with q_prio:0x%x\n", lro_q_info.priority);
		return QOS_MGR_FAILURE;
	} else {
		lroqid = p_item->p_entry;
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"LRO Qid:%d\n", lroqid);
	}
	memset(&shaper_cfg,0x00,sizeof(QOS_MGR_ADD_SHAPER_CFG));

	shaper_cfg.enable = 1;
	shaper_cfg.pir = shaper_in_kbps;
	shaper_cfg.pbs = 8000;
	shaper_cfg.cir = shaper_in_kbps;
	shaper_cfg.cbs = 8000;
	shaper_cfg.flags |= QOS_MGR_Q_F_SPCL_CONN;

	qos_mgr_set_qos_shaper(-1, 0, 0, &shaper_cfg, shaper_cfg.flags, 0);

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Shaper index: %d\n", shaper_cfg.phys_shaperid);	
	qos_mgr_set_qos_rate(NULL, 0, lroqid, shaper_cfg.phys_shaperid, shaper_in_kbps, 0, shaper_cfg.flags, 0);

	return QOS_MGR_SUCCESS;
}

int32_t qos_hal_spl_conn_mod(uint8_t conn_type, bool oper, uint32_t len)
{
	if (oper == 1) {
		if (qos_hal_lro_queue_add(len) == QOS_MGR_FAILURE)
			 QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"New queue addition in LRO has failed.\n");
	} else {
		__qos_hal_spl_conn_init(conn_type, len);
	}

	return 0;
}

#if defined(WMM_QOS_CONFIG) && WMM_QOS_CONFIG
int32_t qos_mgr_create_c2p_map_for_wmm(QOS_MGR_IFNAME ifname[16],uint8_t c2p[])
{
	int32_t ret = QOS_MGR_ENOTAVAIL;
#if !IS_ENABLED(CONFIG_X86_INTEL_LGM) && !IS_ENABLED(CONFIG_SOC_LGM)
	int32_t i,j,c=0;
	QOS_QUEUE_LIST_ITEM *p;

	j = 0;
	qos_mgr_memset(&c2p[0],0,16);
	qos_mgr_queue_lock_list();
	for ( p = g_qos_queue; p; p = p->next ) {
		if ( strcmp(p->ifname,ifname) == 0) {
			ret = QOS_MGR_SUCCESS;
			for(i=0;i< p->tc_no;i++) {
				c2p[c] = (int)(p->tc_map[i]);
				c++;
			}
			j++;
		}
	}

	qos_mgr_queue_unlock_list();

	j = 0; /* USING default MAP. */
	if(j <= 1) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,
			"%s %s %d num. Queues <= 1: Return QOS_MGR_ENOTAVAIL to set default Map !!!\n", __FILE__,__FUNCTION__,__LINE__);
		ret = QOS_MGR_ENOTAVAIL;
	}
#endif /* !CONFIG_X86_INTEL_LGM && !CONFIG_SOC_LGM */

	return ret;
}
EXPORT_SYMBOL(qos_mgr_create_c2p_map_for_wmm);

static int32_t qos_mgr_set_wlan_wmm_prio(QOS_MGR_IFNAME *ifname,int32_t port_id,int8_t caller_flag)
{
	QOS_MGR_NETIF *netif = NULL;
	dp_subif_t *dp_subif = NULL;
	uint8_t *class2prio;
	uint8_t c2p[MAX_TC_NUM] = {0};
	uint8_t cl2p[MAX_TC_NUM] = {0};
	int32_t i;
	int8_t port = 0;

	if(ifname == NULL) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"ifname value is NULL \n");
		return QOS_MGR_FAILURE;
	}

	netif = qos_mgr_get_netif(ifname);
	switch (caller_flag) {
		case 1:
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d caller_case is %d!!!\n", __FILE__,
					__FUNCTION__,__LINE__,caller_flag);
			if (!strncmp(ifname, "wlan0", 5))
				port = 0;
			else if (!strcmp(ifname, "wlan1"))
				port = 1;
			else if (!strncmp(ifname, "wlan2", 5))
				port = 2;
			else if (!strcmp(ifname, "wlan3"))
				port = 3;
			else if (!strncmp(ifname, "wlan4", 5))
				port = 4;
			else if (!strcmp(ifname, "wlan5"))
				port = 5;
			if (port >= 0) {
				for (i = 0; i < MAX_TC_NUM; i++)
					c2p[i] = g_eth_class_prio_map[port][i];
			}
			class2prio = &c2p[0];
			qos_mgr_call_class2prio_notifiers(QOS_MGR_CLASS2PRIO_DEFAULT,
						      port_id, netif,
						      class2prio);
			break;
		case 2:
		case 3:
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d caller_case is %d!!!\n", __FILE__,
					__FUNCTION__,__LINE__,caller_flag);

			dp_subif = kzalloc(sizeof(dp_subif_t), GFP_ATOMIC);
			if (!dp_subif) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,
					"<%s> DP subif allocation failed\n",
					__func__);
				return QOS_MGR_FAILURE;
			}

			if (dp_get_netif_subifid(netif, NULL, NULL, NULL,
						 dp_subif, 0) != DP_SUCCESS) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d dp_get_netif_subifid failed!!!\n",
					__FILE__,__FUNCTION__,__LINE__);
				//return QOS_MGR_FAILURE;
			}

			if (!(dp_subif->alloc_flag & (DP_F_FAST_WLAN|DP_F_FAST_WLAN_EXT)) ||
						(dp_subif->alloc_flag & DP_F_FAST_DSL)) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d not a fastpath wave interface!!!\n",
					__FILE__,__FUNCTION__,__LINE__);
				kfree(dp_subif);
				return QOS_MGR_FAILURE;
			}

			if(qos_mgr_create_c2p_map_for_wmm(ifname,cl2p) == QOS_MGR_ENOTAVAIL) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d Setting default Map!!!\n",
					__FILE__,__FUNCTION__,__LINE__);
				if (!strncmp(ifname, "wlan0", 5))
					port=0;
				else if (!strcmp(ifname, "wlan1"))
					port = 1;
				else if (!strncmp(ifname, "wlan2", 5))
					port = 2;
				else if (!strcmp(ifname, "wlan3"))
					port = 3;
				else if (!strncmp(ifname, "wlan4", 5))
					port = 4;
				else if (!strcmp(ifname, "wlan5"))
					port = 5;
				if (port >= 0) {
					for (i = 0; i < MAX_TC_NUM; i++)
						cl2p[i] = g_eth_class_prio_map[port][i];
				}
			}
			class2prio = cl2p;
			qos_mgr_call_class2prio_notifiers(QOS_MGR_CLASS2PRIO_CHANGE,
						      dp_subif->port_id, netif,
						      class2prio);
			kfree(dp_subif);
			break;
		default:
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"Invalid Wmm caller case \n");
			break;
	}

	return QOS_MGR_SUCCESS;
}
#endif /* WMM_QOS_CONFIG */



int32_t qos_hal_wmm_add(char *ifname)
{
	int32_t ret = QOS_MGR_SUCCESS;
	QOS_MGR_CMD_QUEUE_INFO wmm_q_info = {0};
	QOS_QUEUE_LIST_ITEM *p_item;
	uint32_t q_index = 0;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Add WMM queue for wlan netdevice %s\n", __func__, ifname);
	snprintf(wmm_q_info.ifname, QOS_MGR_IF_NAME_SIZE, ifname);

	/*

	Traffic Type	Traffic Class(TC)   Priority
	------------    -----------------   ---------
	Voice (VO)	    6,7	    	    Highest
	Video (VI)	    4,5               -
	BestEffort (BE)	    0,3	              -
	Background (BK)	    1,2	    	    Lowest

	*/

	for (q_index=1; q_index<=4; q_index++) {
		wmm_q_info.priority = q_index;
		wmm_q_info.enable = 1;
		wmm_q_info.tc_no = 2;
		if (q_index == 1) { /* Voice (VO) */
			wmm_q_info.tc_map[0] = WMM_TC_VO_1;
			wmm_q_info.tc_map[1] = WMM_TC_VO_2;
		} if (q_index == 2) { /* Video (VI) */
			wmm_q_info.tc_map[0] = WMM_TC_VI_1;
			wmm_q_info.tc_map[1] = WMM_TC_VI_2;
		} if (q_index == 3) { /* Best Effort (BE) */
			wmm_q_info.tc_map[0] = WMM_TC_BE_1;
			wmm_q_info.tc_map[1] = WMM_TC_BE_2;
		} if (q_index == 4) { /* Background (BK) */
			wmm_q_info.tc_map[0] = WMM_TC_BK_1;
			wmm_q_info.tc_map[1] = WMM_TC_BK_2;
		}
		wmm_q_info.portid = 0; 
		wmm_q_info.sched = QOS_MGR_SCHED_SP;
		wmm_q_info.queue_num = q_index;

		if (qosal_add_qos_queue(
					&wmm_q_info,
					&p_item) != QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,
				"WMM Queues are not created with q_prio:0x%x for %s\n",
				 q_index, wmm_q_info.ifname);
			return QOS_MGR_FAILURE;
		}

#if defined(WMM_QOS_CONFIG) && WMM_QOS_CONFIG
	qos_mgr_set_wlan_wmm_prio(wmm_q_info.ifname,-1,2);
#endif /* WMM_QOS_CONFIG */

		if (p_item->p_entry < 0) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Add Queue failed\n");
			return QOS_MGR_FAILURE;
		}

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Returned Queue Id: [%d]\n",
				__func__, p_item->p_entry);
	}

	return ret;
}

int32_t qos_hal_wmm_del(char *ifname)
{
	int32_t ret = QOS_MGR_SUCCESS;
	QOS_MGR_CMD_QUEUE_INFO wmm_q_info = {0};
	QOS_QUEUE_LIST_ITEM *p_item;
	uint32_t q_index = 0;

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Del WMM queue for wlan netdevice %s\n", __func__, ifname);

	snprintf(wmm_q_info.ifname, QOS_MGR_IF_NAME_SIZE, ifname);
	wmm_q_info.portid = 0; 

	for (q_index=1; q_index<=4; q_index++) {
		wmm_q_info.queue_num = q_index;
		wmm_q_info.priority = q_index;

		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"<%s> Deleting WMM: [ifname:%s Q Num:%d]\n", __func__, wmm_q_info.ifname, wmm_q_info.queue_num);

		ret = qos_mgr_queue_lookup(wmm_q_info.queue_num, wmm_q_info.ifname, &p_item);
		if( ret == QOS_MGR_ENOTAVAIL ) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"<%s> lookup failed for wlan netdevice %s\n", __func__, wmm_q_info.ifname);
			return QOS_MGR_FAILURE;
		} else {
			if (qosal_delete_qos_queue(&wmm_q_info, p_item) != QOS_MGR_SUCCESS) {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed to remove WMM Queues with q_prio:0x%x for %s\n", q_index, wmm_q_info.ifname);
				qos_mgr_queue_free_item(p_item);
				return QOS_MGR_FAILURE;
			}
		}
		qos_mgr_queue_free_item(p_item);
		qos_mgr_queue_remove_item(wmm_q_info.queue_num, wmm_q_info.ifname, NULL);
		qos_mgr_queue_free_item(p_item);
	}

	return ret;
}

/*
############# SUPPORTING WMM FUNCTION DEFINITIONS : END################
*/

/*
############# INIT FUNCTION DEFINITIONS : START################
*/

int32_t qos_mgr_init_cfg( uint32_t flags, uint32_t hal_id)
{
	int32_t ret=QOS_MGR_SUCCESS;
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_INIT_CFG, NULL, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d qos init success!!!\n", __FILE__,__FUNCTION__,__LINE__);
	}
	return ret;
}

int32_t qos_mgr_uninit_cfg( uint32_t flags, uint32_t hal_id)
{
	int32_t ret=QOS_MGR_SUCCESS;
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_UNINIT_CFG, NULL, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d qos uninit success!!!\n", __FILE__,__FUNCTION__,__LINE__);
	}
	return ret;
}

int32_t qosal_eng_init_cfg(void)
{
	int32_t ret = QOS_MGR_SUCCESS;

	ret = qos_mgr_init_cfg(0,QOS_HAL);

	return ret;		
}

int32_t qosal_eng_uninit_cfg(void)
{
	int32_t ret = QOS_MGR_SUCCESS;
	ret = qos_mgr_uninit_cfg(0,QOS_HAL);
	return ret;		
}

int32_t qos_mgr_ioctl_qos_init_cfg(unsigned int cmd)
{
	int32_t ret = QOS_MGR_SUCCESS;

	switch ( cmd ) {
		case QOS_MGR_CMD_ENG_QUEUE_INIT: {
			ret = qosal_eng_init_cfg();
			break;
		}
		case QOS_MGR_CMD_ENG_QUEUE_UNINIT: {
			ret = qosal_eng_uninit_cfg();
			break;
		}
		default: {
			break;
		}
	}

	return ret;
}
/*
############# INIT FUNCTION DEFINITIONS : END################
*/

/*
############# QUEUE FUNCTION DEFINITIONS : START################
*/
int32_t qos_mgr_modify_qos_queue( char *ifname, QOS_MGR_MOD_QUEUE_CFG *q, uint32_t flags, uint32_t hal_id)
{
	int32_t ret=QOS_MGR_SUCCESS;
	QOS_Q_MOD_CFG tmu_q;
	memset(&tmu_q,0x00,sizeof(QOS_Q_MOD_CFG));
	tmu_q.ifname = ifname;
	tmu_q.portid = q->portid;
	tmu_q.priority = q->priority;
	tmu_q.qlen = q->qlen;
	tmu_q.q_type = q->q_type;
	tmu_q.weight = q->weight;
	if (q->drop.mode == QOS_MGR_DROP_CODEL) {
		tmu_q.drop.mode = QOS_MGR_DROP_CODEL;
	} else if(q->drop.mode == QOS_MGR_DROP_RED) {
		tmu_q.drop.mode = QOS_MGR_DROP_RED;
		tmu_q.drop.wred.min_th0 = q->drop.wred.min_th0;
		tmu_q.drop.wred.max_th0 = q->drop.wred.max_th0;
		tmu_q.drop.wred.max_p0 = q->drop.wred.max_p0;
	} else if(q->drop.mode == QOS_MGR_DROP_WRED) {
		tmu_q.drop.mode = QOS_MGR_DROP_WRED;
		tmu_q.drop.wred.weight = q->drop.wred.weight;
		tmu_q.drop.wred.min_th0 = q->drop.wred.min_th0;
		tmu_q.drop.wred.max_th0 = q->drop.wred.max_th0;
		tmu_q.drop.wred.max_p0 = q->drop.wred.max_p0;
		tmu_q.drop.wred.min_th1 = q->drop.wred.min_th1;
		tmu_q.drop.wred.max_th1 = q->drop.wred.max_th1;
		tmu_q.drop.wred.max_p1 = q->drop.wred.max_p1;
	} else {
		tmu_q.drop.mode = QOS_MGR_DROP_TAIL;
	}

	tmu_q.flags = q->flags;
	tmu_q.q_id = q->queue_id;
	
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_MODQUE_CFG, &tmu_q, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d modify success!!!\n", __FILE__,__FUNCTION__,__LINE__);
		q->queue_id = tmu_q.q_id;
	} else
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d modify failure!!!\n", __FILE__,__FUNCTION__,__LINE__);
	return ret;
}

QOS_QUEUE_LIST_ITEM *qos_mgr_queue_alloc_item(void)
{
	QOS_QUEUE_LIST_ITEM *obj;

	obj = (QOS_QUEUE_LIST_ITEM *)qos_mgr_malloc(sizeof(*obj));
	if (obj) {
		qos_mgr_memset(obj, 0, sizeof(*obj));
		qos_mgr_atomic_set(&obj->count, 1);
	}
	return obj;
}

void qos_mgr_queue_free_item(QOS_QUEUE_LIST_ITEM *obj)
{
	if (qos_mgr_atomic_dec(&obj->count) == 0)
		qos_mgr_free(obj);
}

void qos_mgr_queue_lock_list(void)
{
	qos_mgr_lock_get(&g_qos_queue_lock);
}

void qos_mgr_queue_unlock_list(void)
{
	qos_mgr_lock_release(&g_qos_queue_lock);
}

void __qos_mgr_queue_add_item(QOS_QUEUE_LIST_ITEM *obj)
{
	qos_mgr_atomic_inc(&obj->count);
	obj->next = g_qos_queue;
	g_qos_queue = obj;
}

void qos_mgr_queue_remove_item(int32_t q_num, QOS_MGR_IFNAME ifname[16], QOS_QUEUE_LIST_ITEM **pp_info)
{
	QOS_QUEUE_LIST_ITEM *p_prev, *p_cur;

	if (pp_info)
		*pp_info = NULL;
	p_prev = NULL;
	qos_mgr_queue_lock_list();
	for (p_cur = g_qos_queue; p_cur; p_prev = p_cur, p_cur = p_cur->next)
		if ((p_cur->queue_num == q_num) && (strcmp(p_cur->ifname, ifname) == 0)) {
			if (!p_prev)
				g_qos_queue = p_cur->next;
			else
				p_prev->next = p_cur->next;
			if (pp_info)
				*pp_info = p_cur;
			else
				qos_mgr_queue_free_item(p_cur);
			break;
		}
	qos_mgr_queue_unlock_list();
}

void qos_mgr_queue_free_list(void)
{
	QOS_QUEUE_LIST_ITEM *obj;

	qos_mgr_queue_lock_list();
	while (g_qos_queue) {
		obj = g_qos_queue;
		g_qos_queue = g_qos_queue->next;

		qos_mgr_queue_free_item(obj);
		obj = NULL;
	}
	qos_mgr_queue_unlock_list();
}

int32_t __qos_mgr_intfid_lookup(char *ifname, uint32_t *intfid)
{
	int32_t ret = QOS_MGR_ENOTAVAIL;
	QOS_QUEUE_LIST_ITEM *p;

	for (p = g_qos_queue; p; p = p->next)
		if ((strncmp(p->ifname, ifname, strlen(ifname)) == 0)) {
			ret = QOS_MGR_SUCCESS;
			*intfid = p->intfId;
			break;
		}

	return ret;
}

int32_t qos_mgr_intfid_lookup(char *ifname, uint32_t *intfid)
{
	int32_t ret;
	qos_mgr_queue_lock_list();
	ret = __qos_mgr_intfid_lookup(ifname, intfid);
	qos_mgr_queue_unlock_list();

	return ret;
}

int32_t __qos_mgr_queue_lookup(int32_t q_num, QOS_MGR_IFNAME ifname[16], QOS_QUEUE_LIST_ITEM **pp_info)
{
	int32_t ret = QOS_MGR_ENOTAVAIL;
	QOS_QUEUE_LIST_ITEM *p;

	for (p = g_qos_queue; p; p = p->next)
		if ((p->queue_num == q_num) && (strcmp(p->ifname, ifname) == 0)) {
			ret = QOS_MGR_SUCCESS;
			if (pp_info) {
				qos_mgr_atomic_inc(&p->count);
				*pp_info = p;
			}
			break;
		}

	return ret;
}

int32_t qos_mgr_queue_lookup(int32_t qnum, QOS_MGR_IFNAME ifname[16], QOS_QUEUE_LIST_ITEM **pp_info)
{
	int32_t ret;
	qos_mgr_queue_lock_list();
	ret = __qos_mgr_queue_lookup(qnum, ifname, pp_info);
	qos_mgr_queue_unlock_list();

	return ret;
}

int32_t qos_mgr_ioctl_add_qos_queue(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{
	QOS_QUEUE_LIST_ITEM *p_item;
	int res = QOS_MGR_FAILURE;

	res = copy_from_user(&cmd_info->qos_queue_info, (void *)arg, sizeof(cmd_info->qos_queue_info));
	if (res != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_queue_info.tc_no > MAX_TC_NUM) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid tc no..\n");
		res = QOS_MGR_FAILURE;
		return QOS_MGR_FAILURE;
	}

	if (qosal_add_qos_queue(&cmd_info->qos_queue_info,&p_item) != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

#if defined(WMM_QOS_CONFIG) && WMM_QOS_CONFIG
	qos_mgr_set_wlan_wmm_prio(cmd_info->qos_queue_info.ifname,-1,2);
#endif /* WMM_QOS_CONFIG */

	return QOS_MGR_SUCCESS;
}

int32_t qos_mgr_ioctl_modify_qos_queue(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{
	int32_t ret = QOS_MGR_SUCCESS;
	return ret;
}

int32_t qos_mgr_ioctl_delete_qos_queue(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{

	QOS_QUEUE_LIST_ITEM *p_item;
	QOS_SHAPER_LIST_ITEM *p_s_item;
	int32_t ret = QOS_MGR_FAILURE;

	ret = copy_from_user( &cmd_info->qos_queue_info, (void *)arg, sizeof(cmd_info->qos_queue_info));
	if (ret != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

	ret = qos_mgr_queue_lookup(cmd_info->qos_queue_info.queue_num,cmd_info->qos_queue_info.ifname,&p_item);
	if( ret == QOS_MGR_ENOTAVAIL ) {
		return QOS_MGR_FAILURE;
	} else {
		if (qos_mgr_set_ingress_qos_generic(cmd_info->qos_queue_info.ifname, &cmd_info->qos_queue_info.flags)) {
			qos_mgr_queue_free_item(p_item);
			return QOS_MGR_FAILURE;
		}
		ret = qosal_delete_qos_queue(&cmd_info->qos_queue_info,p_item);
	}
	/* Delete Shaper assigned to the Queue when the Queue is deleted */
	if(ret == QOS_MGR_SUCCESS) {
		if(qos_mgr_shaper_lookup(cmd_info->qos_queue_info.shaper_num,
				cmd_info->qos_queue_info.ifname,&p_s_item) == QOS_MGR_SUCCESS) {
			qos_mgr_shaper_free_item(p_s_item);
			qos_mgr_shaper_remove_item(p_s_item->shaperid,p_s_item->ifname,NULL);
			qos_mgr_shaper_free_item(p_s_item);
		}
	}
	qos_mgr_queue_free_item(p_item);
	qos_mgr_queue_remove_item(p_item->queue_num,p_item->ifname,NULL);
	qos_mgr_queue_free_item(p_item);

	return ret;
}

int32_t qos_mgr_get_pppoa_base_if( char *ifname, QOS_MGR_IFNAME base_ifname[QOS_MGR_IF_NAME_SIZE])
{
	return QOS_MGR_FAILURE;
}

int32_t qos_mgr_modify_qos_subif_to_port(QOS_MGR_CMD_SUBIF_PORT_INFO *subif_port_info)
{
	int32_t ret=QOS_MGR_SUCCESS;
	QOS_MOD_SUBIF_PORT_CFG SubifPort_info;

	memset(&SubifPort_info, 0, sizeof(QOS_MOD_SUBIF_PORT_CFG));

	qos_mgr_strncpy(SubifPort_info.ifname, subif_port_info->ifname, QOS_MGR_IF_NAME_SIZE);
	SubifPort_info.port_id = subif_port_info->port_id;
	SubifPort_info.priority_level = subif_port_info->priority_level;
	SubifPort_info.weight = subif_port_info->weight;
	SubifPort_info.flags = subif_port_info->flags;

	//if ( (ret = qos_mgr_hsel_mod_subif_port_cfg( &SubifPort_info, 0, TMU_HAL) ) != QOS_MGR_SUCCESS ) {
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_MOD_SUBIF_PORT_CFG, &SubifPort_info, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d hal select Mod subif to port failed!!!\n",
				__FILE__,__FUNCTION__,__LINE__);
		return QOS_MGR_FAILURE;
	}
	return ret;
}

int32_t qos_mgr_ioctl_mod_subif_port_config(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{
	int res = QOS_MGR_FAILURE;

	res = copy_from_user( &cmd_info->subif_port_info, (void *)arg, sizeof(cmd_info->subif_port_info));
	if (res != QOS_MGR_SUCCESS)
		return QOS_MGR_FAILURE;

	if( qos_mgr_modify_qos_subif_to_port(&cmd_info->subif_port_info)!= QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d Mod subif to port failed!!!\n", __FILE__,__FUNCTION__,__LINE__);
		return QOS_MGR_FAILURE;
	}

	return QOS_MGR_SUCCESS;
}

int32_t qos_mgr_add_qos_queue(char *ifname, QOS_MGR_ADD_QUEUE_CFG *q, uint32_t flags, uint32_t hal_id)
{
	int32_t ret = QOS_MGR_SUCCESS;
	int32_t k;
	char dev_name[QOS_MGR_IF_NAME_SIZE];
	QOS_Q_ADD_CFG tmu_q;
	memset(&tmu_q, 0x00, sizeof(QOS_Q_ADD_CFG));

	tmu_q.intfId_en = q->intfId_en;
	if (q->intfId_en)
		tmu_q.intfId = q->intfId;

	if (qos_mgr_get_pppoa_base_if(ifname, dev_name) == QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, " %s:%d Pseudo PPPOA interface = %s\n", __FUNCTION__, __LINE__, dev_name);
		tmu_q.dev_name = dev_name;
		tmu_q.ifname = ifname;
		tmu_q.flags |= QOS_MGR_F_PPPOATM;
	} else {
		tmu_q.ifname = ifname;
		tmu_q.dev_name = ifname;
	}

	tmu_q.portid = q->portid;
	tmu_q.priority = q->priority;
	tmu_q.q_type = q->q_type;
	tmu_q.weight = q->weight;
	tmu_q.flags |= flags;
	tmu_q.drop.mode = q->drop.mode;
	switch (q->drop.mode) {
		case QOS_MGR_DROP_TAIL:
			tmu_q.qlen = q->qlen;
			break;
		case QOS_MGR_DROP_RED:
			break;
		case QOS_MGR_DROP_WRED:
			tmu_q.drop.wred.weight = q->drop.wred.weight;
			tmu_q.drop.wred.min_th0 = q->drop.wred.min_th0;
			tmu_q.drop.wred.min_th1 = q->drop.wred.min_th1;
			tmu_q.drop.wred.max_th0 = q->drop.wred.max_th0;
			tmu_q.drop.wred.max_th1 = q->drop.wred.max_th1;
			tmu_q.drop.wred.max_p0 = q->drop.wred.max_p0;
			tmu_q.drop.wred.max_p1 = q->drop.wred.max_p1;
			break;
		case QOS_MGR_DROP_CODEL:
			break;
		default:
			tmu_q.qlen = q->qlen;
			break;
	}

	for (k = 0; k < MAX_TC_NUM; k++)
		tmu_q.tc_map[k] = q->tc_map[k];

	tmu_q.tc_no = q->tc_no;

	ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_ADDQUE_CFG, &tmu_q, 0);
	if (ret == QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s() Queue add success [%s] !!!\n",
			 __func__, tmu_q.ifname);
		q->queue_id = tmu_q.q_id;
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s() QoS is skipped for [%s]!!!\n",
			 __func__, tmu_q.ifname);
	}
	return ret;
}

int32_t qos_mgr_delete_qos_queue(char *ifname, char *dev_name, int32_t priority, uint32_t *queue_id, uint32_t portid,
			uint32_t hal_id, uint32_t flags)
{
	int32_t ret = QOS_MGR_SUCCESS;
	uint32_t infId;
	QOS_Q_DEL_CFG tmu_q;
	memset(&tmu_q, 0x00, sizeof(QOS_Q_DEL_CFG));

	/** For those netdevice which is not registered to DP, intfId is used 
	to store the cqm deq index which is passed by the caller while queue
	is added.
	*/
	if(qos_mgr_intfid_lookup(dev_name, &infId) == QOS_MGR_SUCCESS) {
		tmu_q.intfId = infId;
	}
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, " %s:%d Pseudo PPPOA interface = %s, ifname = %s, flags = %d, qID:%u\n",
			__FUNCTION__, __LINE__, dev_name, ifname, flags, *queue_id);
	tmu_q.dev_name = dev_name;
	tmu_q.ifname = ifname;
	tmu_q.flags = flags;
	tmu_q.portid = portid;
	tmu_q.q_id = *queue_id;
	tmu_q.priority = priority;
	tmu_q.intfId = infId;

	ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_DELQUE_CFG, &tmu_q, 0);
	if (ret == QOS_MGR_SUCCESS)
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %s %d delete success!!!\n", __FILE__, __FUNCTION__, __LINE__);
	return ret;
}

int32_t qosal_add_qos_queue(QOS_MGR_CMD_QUEUE_INFO *q_info, QOS_QUEUE_LIST_ITEM **pp_item)
{
	QOS_QUEUE_LIST_ITEM *p_item;
	QOS_INGGRP_LIST_ITEM *qos_inggrp_list_index;
	QOS_MGR_ADD_QUEUE_CFG add_q_cfg;
	uint32_t k, ret = QOS_MGR_SUCCESS;
	char dev_name[QOS_MGR_IF_NAME_SIZE];

	if (qos_mgr_set_ingress_qos_generic(q_info->ifname, &q_info->flags))
		return QOS_MGR_FAILURE;

	memset(&add_q_cfg, 0x00, sizeof(QOS_MGR_ADD_QUEUE_CFG));
	p_item = qos_mgr_queue_alloc_item();
	if (!p_item) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "alloc queue list item failed \n");
		return QOS_MGR_ENOMEM;
	}
	qos_mgr_strncpy(p_item->ifname, q_info->ifname, QOS_MGR_IF_NAME_SIZE);
	qos_mgr_strncpy(p_item->dev_name, q_info->ifname, QOS_MGR_IF_NAME_SIZE);
	p_item->weight = q_info->weight;
	p_item->priority = q_info->priority;
	p_item->portid = q_info->portid;
	p_item->queue_num = q_info->queue_num;
	p_item->shaper_num = q_info->queue_num;
	p_item->drop.mode = q_info->drop.mode;
	switch (q_info->drop.mode) {
	case QOS_MGR_DROP_TAIL:
		p_item->qlen = q_info->qlen;
		break;
	case QOS_MGR_DROP_RED:
		break;
	case QOS_MGR_DROP_WRED:
		p_item->drop.wred.weight = q_info->drop.wred.weight;
		p_item->drop.wred.min_th0 = q_info->drop.wred.min_th0;
		p_item->drop.wred.min_th1 = q_info->drop.wred.min_th1;
		p_item->drop.wred.max_th0 = q_info->drop.wred.max_th0;
		p_item->drop.wred.max_th1 = q_info->drop.wred.max_th1;
		p_item->drop.wred.max_p0 = q_info->drop.wred.max_p0;
		p_item->drop.wred.max_p1 = q_info->drop.wred.max_p1;
		break;
	case QOS_MGR_DROP_CODEL:
		break;
	default:
		p_item->qlen = q_info->qlen;
		break;
	}
	for (k = 0; k < MAX_TC_NUM; k++)
		p_item->tc_map[k] = q_info->tc_map[k];

	p_item->tc_no = q_info->tc_no;
	p_item->intfId_en = q_info->flowId_en;
	p_item->intfId = q_info->flowId;

	if (qos_mgr_get_pppoa_base_if(p_item->ifname, dev_name) == QOS_MGR_SUCCESS) {
		qos_mgr_strncpy(p_item->dev_name, dev_name, QOS_MGR_IF_NAME_SIZE);
		p_item->flags |= QOS_MGR_F_PPPOATM;
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, " %s:%d Pseudo PPPOA interface = %s : flags = %d\n", __FUNCTION__,
			__LINE__, dev_name, p_item->flags);
	}

	add_q_cfg.portid = q_info->portid;
	add_q_cfg.priority = q_info->priority;
	add_q_cfg.weight = q_info->weight;
	add_q_cfg.q_type = q_info->sched;
	for (k = 0; k < MAX_TC_NUM; k++)
		add_q_cfg.tc_map[k] = q_info->tc_map[k];
	add_q_cfg.tc_no = q_info->tc_no;
	add_q_cfg.drop.mode = q_info->drop.mode;
	switch (q_info->drop.mode) {
		case QOS_MGR_DROP_TAIL:
			add_q_cfg.qlen = q_info->qlen;
				break;
			case QOS_MGR_DROP_RED:
				break;
			case QOS_MGR_DROP_WRED:
				add_q_cfg.drop.wred.weight = q_info->drop.wred.weight;
				add_q_cfg.drop.wred.min_th0 = q_info->drop.wred.min_th0;
				add_q_cfg.drop.wred.min_th1 = q_info->drop.wred.min_th1;
				add_q_cfg.drop.wred.max_th0 = q_info->drop.wred.max_th0;
				add_q_cfg.drop.wred.max_th1 = q_info->drop.wred.max_th1;
				add_q_cfg.drop.wred.max_p0 = q_info->drop.wred.max_p0;
				add_q_cfg.drop.wred.max_p1 = q_info->drop.wred.max_p1;
					break;
			case QOS_MGR_DROP_CODEL:
					break;
			default:
				add_q_cfg.qlen = q_info->qlen;
				break;
	}
	if (q_info->flowId_en) {
		add_q_cfg.intfId_en = 1;
		add_q_cfg.intfId = q_info->flowId;
	}

	ret = qos_mgr_add_qos_queue(q_info->ifname, &add_q_cfg, q_info->flags, QOS_HAL);
	
	if (ret == QOS_MGR_SUCCESS) {
		if ((q_info->flags & QOS_MGR_Q_F_INGRESS) == QOS_MGR_Q_F_INGRESS) {
			if (qos_mgr_inggrp_lookup(q_info->ifname, &qos_inggrp_list_index) == QOS_MGR_SUCCESS) {
				ret = qos_mgr_update_queue_info(q_info, &add_q_cfg, qos_inggrp_list_index);
				if (ret != QOS_MGR_SUCCESS)
					QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,
						"%s:%d: qos_mgr_update_queue_info failed\n",
						 __func__, __LINE__);
				qos_mgr_inggrp_free_item(qos_inggrp_list_index);
			} else {
				QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,
					"%s() Interface %s not found\n", __func__, q_info->ifname);
			}
		}
		p_item->p_entry = (add_q_cfg.queue_id);
		__qos_mgr_queue_add_item(p_item);
	} else {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,
			"%s() QoS is not brought up for [%s]\n", __func__, q_info->ifname);
		qos_mgr_queue_free_item(p_item);
		*pp_item = NULL;
		return QOS_MGR_FAILURE;
	}

	*pp_item = p_item;
	return QOS_MGR_SUCCESS;
}

int32_t qosal_modify_qos_queue(QOS_MGR_CMD_QUEUE_INFO *q_info, QOS_QUEUE_LIST_ITEM **pp_item)
{
	int32_t ret = QOS_MGR_SUCCESS;
	QOS_QUEUE_LIST_ITEM *p_q_item;
	QOS_QUEUE_LIST_ITEM *p_q_item1;
	QOS_MGR_MOD_QUEUE_CFG mod_q_cfg;
	uint32_t *tmp;

	p_q_item1 = *pp_item;
	memset(&mod_q_cfg, 0x00, sizeof(QOS_MGR_MOD_QUEUE_CFG));
	/* Start of list del and add operation*/
	p_q_item = qos_mgr_queue_alloc_item();
	if (!p_q_item)
		goto UPDATE_FAILED;

	/* Update all info into p_q_item */
	qos_mgr_strncpy(p_q_item->ifname, q_info->ifname, QOS_MGR_IF_NAME_SIZE);
	p_q_item->weight = q_info->weight;
	p_q_item->priority = q_info->priority;
	p_q_item->qlen = q_info->qlen;
	p_q_item->drop.enable = q_info->drop.enable;
	p_q_item->drop.mode = q_info->drop.mode;
	p_q_item->drop.wred.weight = q_info->drop.wred.weight;
	p_q_item->drop.wred.min_th0 = q_info->drop.wred.min_th0;
	p_q_item->drop.wred.max_th0 = q_info->drop.wred.max_th0;
	p_q_item->drop.wred.max_p0 = q_info->drop.wred.max_p0;
	p_q_item->drop.wred.min_th1 = q_info->drop.wred.min_th1;
	p_q_item->drop.wred.max_th1 = q_info->drop.wred.max_th1;
	p_q_item->drop.wred.max_p1 = q_info->drop.wred.max_p1;
	p_q_item->num_caps = p_q_item1->num_caps;
	p_q_item->p_entry = p_q_item1->p_entry;
	p_q_item->queue_num = p_q_item1->queue_num;

	qos_mgr_queue_remove_item(p_q_item1->queue_num, p_q_item1->ifname, NULL);
	qos_mgr_queue_free_item(p_q_item1);

	__qos_mgr_queue_add_item(p_q_item);
	/* End of list del and add operation*/

					mod_q_cfg.portid = q_info->portid;
					mod_q_cfg.priority = q_info->priority;
					mod_q_cfg.qlen = q_info->qlen;
					mod_q_cfg.q_type = q_info->sched;
					mod_q_cfg.weight = q_info->weight;
					if (q_info->drop.mode == QOS_MGR_DROP_CODEL) {
						mod_q_cfg.drop.mode = QOS_MGR_DROP_CODEL;
					} else if (q_info->drop.mode == QOS_MGR_DROP_RED) {
						mod_q_cfg.drop.mode = QOS_MGR_DROP_RED;
						mod_q_cfg.drop.wred.min_th0 = q_info->drop.wred.min_th0;
						mod_q_cfg.drop.wred.max_th0 = q_info->drop.wred.max_th0;
						mod_q_cfg.drop.wred.max_p0 = q_info->drop.wred.max_p0;
					} else if (q_info->drop.mode == QOS_MGR_DROP_WRED) {
						mod_q_cfg.drop.mode = QOS_MGR_DROP_WRED;
						mod_q_cfg.drop.wred.weight = q_info->drop.wred.weight;
						mod_q_cfg.drop.wred.min_th0 = q_info->drop.wred.min_th0;
						mod_q_cfg.drop.wred.max_th0 = q_info->drop.wred.max_th0;
						mod_q_cfg.drop.wred.max_p0 = q_info->drop.wred.max_p0;
						mod_q_cfg.drop.wred.min_th1 = q_info->drop.wred.min_th1;
						mod_q_cfg.drop.wred.max_th1 = q_info->drop.wred.max_th1;
						mod_q_cfg.drop.wred.max_p1 = q_info->drop.wred.max_p1;
					} else {
						mod_q_cfg.drop.mode = QOS_MGR_DROP_TAIL;
					}

						mod_q_cfg.flags = q_info->flags;
						mod_q_cfg.queue_id = p_q_item->p_entry;

						ret = qos_mgr_modify_qos_queue(q_info->ifname, &mod_q_cfg, q_info->flags, QOS_HAL);
						if (ret != QOS_MGR_SUCCESS)
							QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "qos_mgr_modify_qos_queue failed \n");

						if (ret == QOS_MGR_SUCCESS) {
							tmp = &mod_q_cfg.queue_id;
							p_q_item->p_entry = *tmp;
						}
	/* returned success in step 1 proceed */
	return ret;

UPDATE_FAILED:
	return QOS_MGR_FAILURE;

}

int32_t qosal_delete_qos_queue(QOS_MGR_CMD_QUEUE_INFO *q_info, QOS_QUEUE_LIST_ITEM *p_item)
{
	int32_t ret = QOS_MGR_SUCCESS;
	if (qos_mgr_set_ingress_qos_generic(q_info->ifname, &q_info->flags))
		return QOS_MGR_FAILURE;

	ret = qos_mgr_delete_qos_queue(q_info->ifname, p_item->dev_name, q_info->priority, &(p_item->p_entry),
			q_info->portid, QOS_HAL, (q_info->flags|p_item->flags));

	if (ret != QOS_MGR_SUCCESS)
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "qos_mgr_delete_qos_queue failed \n");
	return ret;
}

/*
############# QUEUE FUNCTION DEFINITIONS : END################
*/

/*
############# VAPQOS FUNCTION DEFINITIONS : START################
*/
/*
############# VAPQOS FUNCTION DEFINITIONS : END################
*/


/*
############# QOS GET FUNCTION DEFINITIONS : START################
*/


/*
############# QOS GET FUNCTION DEFINITIONS : END################
*/

/*
############# LEGACY QOS FUNCTION DEFINITIONS : START################
*/

/*
############# LEGACY QOS FUNCTION DEFINITIONS : END################
*/

/*
############# QOS RATE LIMIT FUNCTION DEFINITIONS : START################
*/

int32_t qos_mgr_set_ctrl_qos_rate(uint32_t portid,	uint32_t f_enable, uint32_t flag)
{
	uint32_t i;
	QOS_MGR_COUNT_CFG count={0};
	QOS_MGR_ENABLE_CFG enable_cfg={0};
	QOS_MGR_RATE_SHAPING_CFG rate={0};

	count.portid = portid;
	count.flags = flag;
	//qos_mgr_drv_get_qos_qnum( &count, 0);
	qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_GET_QOS_QUEUE_NUM, &count, 0);

	if( count.num <= 0 )
		return QOS_MGR_FAILURE;

	enable_cfg.portid = portid;
	enable_cfg.flag = flag;
	enable_cfg.f_enable = f_enable;
	//qos_mgr_drv_set_ctrl_qos_rate( &enable_cfg, 0);
	qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_SET_QOS_RATE_SHAPING_CTRL, &enable_cfg, 0);

	for( i=0; i<count.num; i++ ) {
		rate.flag = 0;
		rate.portid = portid;
		rate.queueid	= i;
		//qos_mgr_drv_reset_qos_rate( &rate, 0);
		qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_RESET_QOS_RATE_SHAPING_CFG, &rate, 0);
	}

	return QOS_MGR_SUCCESS;
}
EXPORT_SYMBOL(qos_mgr_set_ctrl_qos_rate);

int32_t qos_mgr_get_ctrl_qos_rate(uint32_t portid,	uint32_t *f_enable, uint32_t flag)
{
	QOS_MGR_ENABLE_CFG enable_cfg={0};
	int32_t res;

	enable_cfg.portid = portid;
	enable_cfg.flag = flag;

	//res= qos_mgr_drv_get_ctrl_qos_rate( &enable_cfg, 0);
	res = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_GET_QOS_RATE_SHAPING_CTRL, &enable_cfg, 0);

	if( *f_enable ) *f_enable = enable_cfg.f_enable;
	return res;
}
EXPORT_SYMBOL(qos_mgr_get_ctrl_qos_rate);

int32_t qos_mgr_set_qos_rate( char *ifname, uint32_t portid, uint32_t queueid, int32_t shaperid, uint32_t rate, uint32_t burst, uint32_t flag, int32_t hal_id )
{

	int32_t ret=QOS_MGR_SUCCESS;

	bool is_lro=0;
	char dev_name[QOS_MGR_IF_NAME_SIZE];

	QOS_RATE_SHAPING_CFG tmu_rate_cfg;
	memset(&tmu_rate_cfg,0x00,sizeof(QOS_RATE_SHAPING_CFG));
	if ((flag & QOS_MGR_Q_F_SPCL_CONN) == QOS_MGR_Q_F_SPCL_CONN)
                is_lro = 1;
	if (!is_lro) {
		if(qos_mgr_get_pppoa_base_if(ifname,dev_name) == QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE," %s:%d Pseudo PPPOA interface = %s\n",__FUNCTION__,__LINE__,dev_name);
			tmu_rate_cfg.dev_name = dev_name;
			snprintf(tmu_rate_cfg.ifname, sizeof(tmu_rate_cfg.ifname), "%s", ifname);
			tmu_rate_cfg.flag |= QOS_MGR_F_PPPOATM;
		} else {
			snprintf(tmu_rate_cfg.ifname, sizeof(tmu_rate_cfg.ifname), "%s", ifname);
			tmu_rate_cfg.dev_name = ifname;
		}
	}
	tmu_rate_cfg.portid = portid;
	tmu_rate_cfg.queueid = queueid;
	tmu_rate_cfg.shaperid = shaperid;
	tmu_rate_cfg.rate_in_kbps = rate;
	tmu_rate_cfg.burst = burst;
	tmu_rate_cfg.flag |= flag;
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_SET_QOS_RATE_SHAPING_CFG, &tmu_rate_cfg, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d add success!!!\n", __FILE__,__FUNCTION__,__LINE__);
	}
	return ret;
}
EXPORT_SYMBOL(qos_mgr_set_qos_rate);

int32_t qos_mgr_get_qos_rate( uint32_t portid, uint32_t queueid, int32_t *shaperid, uint32_t *rate, uint32_t *burst, uint32_t flag)
{
	QOS_MGR_RATE_SHAPING_CFG rate_cfg={0};
	int32_t res = QOS_MGR_FAILURE;

	rate_cfg.portid = portid;
	rate_cfg.flag = flag;
	rate_cfg.queueid = queueid;
	rate_cfg.shaperid = -1;
	//res = qos_mgr_drv_get_qos_rate( &rate_cfg, 0);
	res = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_GET_QOS_RATE_SHAPING_CFG, &rate_cfg, 0);

	if( rate ) *rate = rate_cfg.rate_in_kbps;
	if( burst )	*burst = rate_cfg.burst;
	if( shaperid ) *shaperid = rate_cfg.shaperid;
	return res;

}
EXPORT_SYMBOL(qos_mgr_get_qos_rate);

int32_t qos_mgr_reset_qos_rate(char *ifname, char *dev_name, uint32_t portid, int32_t queueid, int32_t shaperid, uint32_t flag, int32_t hal_id)
{
	int32_t ret = QOS_MGR_FAILURE;
	QOS_RATE_SHAPING_CFG tmu_rate_cfg;
	memset(&tmu_rate_cfg,0x00,sizeof(QOS_RATE_SHAPING_CFG));

	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE," %s:%d Pseudo PPPOA interface = %s\n",__FUNCTION__,__LINE__,dev_name);
	tmu_rate_cfg.dev_name = dev_name;
	qos_mgr_strncpy(tmu_rate_cfg.ifname,ifname, QOS_MGR_IF_NAME_SIZE);
	tmu_rate_cfg.portid = portid;
	tmu_rate_cfg.queueid = queueid;
	tmu_rate_cfg.shaperid = shaperid;
	tmu_rate_cfg.flag |= flag;

	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_RESET_QOS_RATE_SHAPING_CFG, &tmu_rate_cfg, 0)) == QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d add success!!!\n", __FILE__,__FUNCTION__,__LINE__);
	}
	return ret;
}
EXPORT_SYMBOL(qos_mgr_reset_qos_rate);


int32_t qos_mgr_ioctl_set_ctrl_qos_rate(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int32_t res;

	qos_mgr_memset(&cmd_info->qos_ctrl_info, 0, sizeof(cmd_info->qos_ctrl_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_ctrl_info, (void *)arg, sizeof(cmd_info->qos_ctrl_info)) != 0 )
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_ctrl_info.portid > QOS_HAL_MAX_EGRESS_PORT) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid portid.\n");
		return QOS_MGR_FAILURE;
	}

	res = qos_mgr_set_ctrl_qos_rate(cmd_info->qos_ctrl_info.portid, cmd_info->qos_ctrl_info.enable, cmd_info->qos_ctrl_info.flags);
	if ( res != QOS_MGR_SUCCESS )
	{
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_set_ctrl_qos_rate fail\n");
		res = QOS_MGR_FAILURE;
	}

	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_set_ctrl_qos_rate);

int32_t qos_mgr_ioctl_get_ctrl_qos_rate(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int res = QOS_MGR_FAILURE;

	qos_mgr_memset(&cmd_info->qos_ctrl_info, 0, sizeof(cmd_info->qos_ctrl_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_ctrl_info, (void *)arg, sizeof(cmd_info->qos_ctrl_info)) != 0 )
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_ctrl_info.portid > QOS_HAL_MAX_EGRESS_PORT) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid portid.\n");
		return QOS_MGR_FAILURE;
	}

	res = qos_mgr_get_ctrl_qos_rate(cmd_info->qos_ctrl_info.portid, &cmd_info->qos_ctrl_info.enable, cmd_info->qos_ctrl_info.flags);
	if ( res != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_get_ctrl_qos_rate fail\n");
		res = QOS_MGR_FAILURE;
	}

	if ( qos_mgr_copy_to_user( (void *)arg, &cmd_info->qos_ctrl_info, sizeof(cmd_info->qos_ctrl_info)) != 0 )
		return QOS_MGR_FAILURE;

	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_get_ctrl_qos_rate);

int32_t qosal_reset_qos_rate(QOS_MGR_CMD_RATE_INFO *qos_rate_info, QOS_QUEUE_LIST_ITEM *p_item, QOS_SHAPER_LIST_ITEM *p_s_item)
{
	uint32_t ret=QOS_MGR_SUCCESS;

	if((qos_rate_info->shaperid == -1) && (qos_rate_info->queueid == -1)) {
		ret = qos_mgr_reset_qos_rate(qos_rate_info->ifname,p_s_item->dev_name,
		qos_rate_info->portid, -1 , p_s_item->p_entry,
		qos_rate_info->flags, QOS_HAL);
	}
	else {
		ret = qos_mgr_reset_qos_rate(qos_rate_info->ifname,p_s_item->dev_name,qos_rate_info->portid,
		p_item->p_entry, p_s_item->p_entry, qos_rate_info->flags, QOS_HAL);
	}

	if(ret != QOS_MGR_SUCCESS)
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"qos_mgr_set_qos_rate failed \n" );

	return ret;
}

int32_t qosal_set_qos_rate(QOS_MGR_CMD_RATE_INFO *qos_rate_info, QOS_QUEUE_LIST_ITEM *p_item, QOS_SHAPER_LIST_ITEM *p_s_item)
{
	uint32_t ret = QOS_MGR_SUCCESS;


	/* when init, these entry values are ~0, the max the number which can be detected by these functions*/
	if((qos_rate_info->shaperid == -1) && (qos_rate_info->queueid == -1)) {
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM) || IS_ENABLED(CONFIG_PRX300_HGU)
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"---> p_s_item->p_entry:[%d] flags:[%x]\n",
			p_s_item->p_entry,
			qos_rate_info->flags);
		if (qos_rate_info->flags & QOS_MGR_OP_F_MODIFY) {
			QOS_MGR_ADD_SHAPER_CFG shaper_cfg;
			memset(&shaper_cfg,0x00,sizeof(QOS_MGR_ADD_SHAPER_CFG));

			shaper_cfg.enable = qos_rate_info->shaper.enable;
			shaper_cfg.mode = qos_rate_info->shaper.mode;
			shaper_cfg.pir = qos_rate_info->shaper.pir;
			shaper_cfg.pbs = qos_rate_info->shaper.pbs;
			shaper_cfg.cir = qos_rate_info->shaper.cir;
			shaper_cfg.cbs = qos_rate_info->shaper.cbs;
			shaper_cfg.flags = qos_rate_info->shaper.flags;
			qos_mgr_set_qos_shaper(p_s_item->p_entry,
				qos_rate_info->rate,
				qos_rate_info->burst,
				&shaper_cfg,
				qos_rate_info->flags,
				QOS_HAL);
			}
#endif
		ret = qos_mgr_set_qos_rate(qos_rate_info->ifname,qos_rate_info->portid, -1 ,
		p_s_item->p_entry, qos_rate_info->rate,qos_rate_info->burst,
		qos_rate_info->flags, QOS_HAL);
	}
	else {
		ret = qos_mgr_set_qos_rate(qos_rate_info->ifname,qos_rate_info->portid,
		p_item->p_entry, p_s_item->p_entry, qos_rate_info->rate,qos_rate_info->burst,
		qos_rate_info->flags, QOS_HAL);
	}

	if(ret != QOS_MGR_SUCCESS)
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"qos_mgr_set_qos_rate failed \n" );

	return ret;

}
EXPORT_SYMBOL(qosal_set_qos_rate);

int32_t qos_mgr_ioctl_set_qos_rate(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int32_t res;


	uint32_t phy_qid,phy_shaperid;
		char dev_name[QOS_MGR_IF_NAME_SIZE];
	QOS_QUEUE_LIST_ITEM *p_item = NULL;
	QOS_SHAPER_LIST_ITEM *p_s_item;

	qos_mgr_memset(&cmd_info->qos_rate_info, 0, sizeof(cmd_info->qos_rate_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_rate_info, (void *)arg, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	phy_qid = cmd_info->qos_rate_info.queueid;
	phy_shaperid = cmd_info->qos_rate_info.shaperid;

	res = qos_mgr_shaper_lookup(cmd_info->qos_rate_info.shaperid,cmd_info->qos_rate_info.ifname,&p_s_item);
	if( res == QOS_MGR_ENOTAVAIL ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, " qos_mgr_ioctl_set_qos_rate: QOS_MGR_SHAPER_NOT_FOUND	\n");
		return QOS_MGR_FAILURE;
	}
	else
		phy_shaperid = p_s_item->p_entry;

	if(phy_qid != -1) {
		res = qos_mgr_queue_lookup(cmd_info->qos_rate_info.queueid,cmd_info->qos_queue_info.ifname,&p_item);
		if( res == QOS_MGR_ENOTAVAIL ) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, " qos_mgr_ioctl_set_qos_rate: QOS_MGR_QUEUE_NOT_FOUND	\n");
			qos_mgr_shaper_free_item(p_s_item);
			return QOS_MGR_FAILURE;
		} else
			phy_qid = p_item->p_entry;
	}
	if(qos_mgr_get_pppoa_base_if(cmd_info->qos_rate_info.ifname,dev_name) == QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE," %s:%d Pseudo PPPOA interface = %s\n",__FUNCTION__,__LINE__,dev_name);
		qos_mgr_strncpy(p_s_item->dev_name,dev_name,QOS_MGR_IF_NAME_SIZE);
		p_s_item->flags |= QOS_MGR_F_PPPOATM;
	}
	if (qos_mgr_set_ingress_qos_generic(cmd_info->qos_rate_info.ifname, &cmd_info->qos_rate_info.flags)) {
		if (p_item)
			qos_mgr_queue_free_item(p_item);
		qos_mgr_shaper_free_item(p_s_item);
		return QOS_MGR_FAILURE;
	}

	if ( p_item != NULL ) {
		res = qosal_set_qos_rate(&cmd_info->qos_rate_info,p_item,p_s_item);
		qos_mgr_queue_free_item(p_item);
	} else
		res = qosal_set_qos_rate(&cmd_info->qos_rate_info,NULL,p_s_item);

	if ( res != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_set_qos_rate fail\n");
		res = QOS_MGR_FAILURE;
	}

	qos_mgr_shaper_free_item(p_s_item);
	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_set_qos_rate);

int32_t qos_mgr_ioctl_reset_qos_rate(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int res = QOS_MGR_FAILURE;


	uint32_t phy_qid,phy_shaperid;
	QOS_QUEUE_LIST_ITEM *p_item = NULL;
	QOS_SHAPER_LIST_ITEM *p_s_item = NULL;

	qos_mgr_memset(&cmd_info->qos_rate_info, 0, sizeof(cmd_info->qos_rate_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_rate_info, (void *)arg, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	phy_qid = cmd_info->qos_rate_info.queueid;
	phy_shaperid = cmd_info->qos_rate_info.shaperid;

	res = qos_mgr_shaper_lookup(cmd_info->qos_rate_info.shaperid,cmd_info->qos_rate_info.ifname,&p_s_item);
	if( res == QOS_MGR_ENOTAVAIL || p_s_item == NULL ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, " qos_mgr_ioctl_reset_qos_rate: QOS_MGR_SHAPER_NOT_FOUND	\n");
		return QOS_MGR_FAILURE;
	}
	else
		phy_shaperid = p_s_item->p_entry;

	if(phy_qid != -1) {
		res = qos_mgr_queue_lookup(cmd_info->qos_rate_info.queueid,cmd_info->qos_queue_info.ifname,&p_item);
		if( res == QOS_MGR_ENOTAVAIL || p_item == NULL) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, " qos_mgr_ioctl_set_qos_rate: QOS_MGR_QUEUE_NOT_FOUND	\n");
			qos_mgr_shaper_free_item(p_s_item);
			return QOS_MGR_FAILURE;
		}
		else
			phy_qid = p_item->p_entry;
	}

	if (qos_mgr_set_ingress_qos_generic(cmd_info->qos_rate_info.ifname, &cmd_info->qos_rate_info.flags)) {
		if (p_item)
			qos_mgr_queue_free_item(p_item);
		qos_mgr_shaper_free_item(p_s_item);
		return QOS_MGR_FAILURE;
	}

	if ( p_item != NULL ) {
		res = qosal_reset_qos_rate(&cmd_info->qos_rate_info,p_item,p_s_item);
		qos_mgr_queue_free_item(p_item);
	} else {
		res = qosal_reset_qos_rate(&cmd_info->qos_rate_info,NULL,p_s_item);
	}

	if ( res != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_set_qos_rate fail\n");
		res = QOS_MGR_FAILURE;
	}

	qos_mgr_shaper_free_item(p_s_item);
	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_reset_qos_rate);

int32_t qos_mgr_ioctl_get_qos_rate(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int res = QOS_MGR_FAILURE;

	qos_mgr_memset(&cmd_info->qos_rate_info, 0, sizeof(cmd_info->qos_rate_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_rate_info, (void *)arg, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_rate_info.portid > QOS_HAL_MAX_EGRESS_PORT) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid portid.\n");
		return QOS_MGR_FAILURE;
	}

	res = qos_mgr_get_qos_rate(cmd_info->qos_rate_info.portid, cmd_info->qos_rate_info.queueid, 
			&cmd_info->qos_rate_info.shaperid, &cmd_info->qos_rate_info.rate,
			&cmd_info->qos_rate_info.burst, cmd_info->qos_rate_info.flags);
	if ( res != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_get_qos_rate fail\n");
		res = QOS_MGR_FAILURE;
	}

	if ( qos_mgr_copy_to_user( (void *)arg, &cmd_info->qos_rate_info, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_get_qos_rate);

int32_t qos_mgr_set_qos_shaper( int32_t shaperid, uint32_t rate, uint32_t burst, QOS_MGR_ADD_SHAPER_CFG *s, uint32_t flags, int32_t hal_id )
{
	uint32_t ret=QOS_MGR_SUCCESS;
	QOS_RATE_SHAPING_CFG tmu_shape_cfg;

	memset(&tmu_shape_cfg,0x00,sizeof(QOS_RATE_SHAPING_CFG));
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM) || IS_ENABLED(CONFIG_PRX300_HGU)
	tmu_shape_cfg.shaperid = shaperid;
#endif
	tmu_shape_cfg.shaper.mode = s->mode;
	tmu_shape_cfg.shaper.enable = s->enable;
	tmu_shape_cfg.shaper.pir = s->pir;
	tmu_shape_cfg.shaper.pbs = s->pbs;
	tmu_shape_cfg.shaper.cir = s->cir;
	tmu_shape_cfg.shaper.cbs = s->cbs;
	tmu_shape_cfg.shaper.flags = s->flags;
	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_SET_QOS_SHAPER_CFG, &tmu_shape_cfg, 0)) == QOS_MGR_SUCCESS ) {
		s->phys_shaperid = tmu_shape_cfg.phys_shaperid;
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d add success!!!\n", __FILE__,__FUNCTION__,__LINE__);
	} else
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE,"%s %s %d add failed!!!\n", __FILE__,__FUNCTION__,__LINE__);

	return ret;
}

int32_t qos_mgr_get_qos_shaper( int32_t shaperid, uint32_t *rate, uint32_t *burst, uint32_t flag)
{
	QOS_MGR_RATE_SHAPING_CFG rate_cfg={0};
	int32_t res = QOS_MGR_FAILURE;

	rate_cfg.flag = flag;
	rate_cfg.shaperid = shaperid;

	res = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_GET_QOS_RATE_SHAPING_CFG, &rate_cfg, 0);


	if( rate ) *rate = rate_cfg.rate_in_kbps;
	if( burst )	*burst = rate_cfg.burst;

	return res;

}

int32_t qos_mgr_add_qos_mib(char *ifname, QOS_MGR_CMD_MIB_INFO *q)
{
	int32_t ret = QOS_MGR_SUCCESS;
	QOS_MGR_MIB_INFO tmu_q;
	QOS_QUEUE_LIST_ITEM *p_item;

	ret = qos_mgr_queue_lookup(q->queue_num, ifname, &p_item);
	if (ret == QOS_MGR_ENOTAVAIL) {
		pr_err("%s(): qos_mgr_queue_lookup Failed\n", __func__);
		return QOS_MGR_FAILURE;
	}
	memset(&tmu_q, 0x00, sizeof(QOS_MGR_MIB_INFO));

	tmu_q.ifname = ifname;
	tmu_q.portid = q->portid;
	tmu_q.queueid = p_item->p_entry;
	tmu_q.mib.total_rx_pkt = q->mib.total_rx_pkt;
	tmu_q.mib.total_rx_bytes = q->mib.total_rx_bytes;
	tmu_q.mib.total_tx_pkt = q->mib.total_tx_pkt;
	tmu_q.mib.total_tx_bytes = q->mib.total_tx_bytes;
	tmu_q.mib.cpu_path_small_pkt_drop_cnt =
			q->mib.cpu_path_small_pkt_drop_cnt;
	tmu_q.mib.cpu_path_total_pkt_drop_cnt =
			q->mib.cpu_path_total_pkt_drop_cnt;
	tmu_q.mib.fast_path_small_pkt_drop_cnt =
			q->mib.fast_path_small_pkt_drop_cnt;
	tmu_q.mib.fast_path_total_pkt_drop_cnt =
			q->mib.fast_path_total_pkt_drop_cnt;
	tmu_q.mib.tx_diff = q->mib.tx_diff;
	tmu_q.mib.tx_diff_L1 = q->mib.tx_diff_L1;
	tmu_q.mib.tx_diff_jiffy = q->mib.tx_diff_jiffy;
	tmu_q.mib.sys_hz = q->mib.sys_hz;
	tmu_q.mib.q_occ = q->mib.q_occ;
	tmu_q.reg_addr = 0;
	tmu_q.flag = q->flags;

	ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_GET_QOS_MIB, &tmu_q, 0);
	if (ret  == QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %d success rx:%lld, tx=%lld, drp=%lld\n",
					__func__, __LINE__,
					tmu_q.mib.total_rx_pkt,
					tmu_q.mib.total_tx_pkt,
					tmu_q.mib.fast_path_total_pkt_drop_cnt);
		q->mib.total_rx_pkt = tmu_q.mib.total_rx_pkt;
		q->mib.total_tx_pkt = tmu_q.mib.total_tx_pkt;
		q->mib.fast_path_total_pkt_drop_cnt = tmu_q.mib.fast_path_total_pkt_drop_cnt;
	} else
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s %d failure!!!\n", __func__, __LINE__);
	return ret;
}

int32_t qos_mgr_ioctl_set_qos_shaper(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/

	QOS_SHAPER_LIST_ITEM *p_item;
	qos_mgr_memset(&cmd_info->qos_rate_info, 0, sizeof(cmd_info->qos_rate_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_rate_info, (void *)arg, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_rate_info.shaperid > QOS_HAL_MAX_SHAPER) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid shaperid.\n");
		return QOS_MGR_FAILURE;
	}

	if( qosal_add_shaper(&cmd_info->qos_rate_info,&p_item)!= QOS_MGR_SUCCESS) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qosal_add_shaper returned failure\n");
		return QOS_MGR_FAILURE;
	}
	return QOS_MGR_SUCCESS;
}
EXPORT_SYMBOL(qos_mgr_ioctl_set_qos_shaper);

int32_t qos_mgr_ioctl_get_qos_shaper(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA * cmd_info)
{ /*note, arg is a pointer from ioctl, not normally pointer	*/
	int res = QOS_MGR_FAILURE;

	qos_mgr_memset(&cmd_info->qos_rate_info, 0, sizeof(cmd_info->qos_rate_info) );

	if ( qos_mgr_copy_from_user( &cmd_info->qos_rate_info, (void *)arg, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	if (cmd_info->qos_rate_info.shaperid > QOS_HAL_MAX_SHAPER) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Ivalid shaperid.\n");
		return QOS_MGR_FAILURE;
	}

	res = qos_mgr_get_qos_shaper(cmd_info->qos_rate_info.shaperid, &cmd_info->qos_rate_info.rate,
			&cmd_info->qos_rate_info.burst, cmd_info->qos_rate_info.flags);
	if ( res != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "qos_mgr_ioctl_get_qos_shaper fail\n");
		res = QOS_MGR_FAILURE;
	}

	if ( qos_mgr_copy_to_user( (void *)arg, &cmd_info->qos_rate_info, sizeof(cmd_info->qos_rate_info)) != 0 )
		return QOS_MGR_FAILURE;

	return res;
}
EXPORT_SYMBOL(qos_mgr_ioctl_get_qos_shaper);

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
static int32_t qos_mgr_set_ing_class_rule(QOS_MGR_CMD_ADD_CLASSIFIER_INFO* ing_rule_info)
{
	QOS_INGGRP_LIST_ITEM* inggrp_list_item = NULL;
	int field_iter = 0;
	struct pp_whitelist_field* fields_ptr = NULL;
	fields_ptr = (struct pp_whitelist_field*) qos_mgr_malloc(ing_rule_info->rulecfg.chkrcfg.rules.field_count * sizeof (struct pp_whitelist_field));

	if(fields_ptr == NULL)
		return QOS_MGR_FAILURE;

	qos_mgr_inggrp_lookup(ing_rule_info->rulecfg.chkrcfg.group_name, &inggrp_list_item);
	if(inggrp_list_item == NULL) {
		qos_mgr_free(fields_ptr);
		return QOS_MGR_FAILURE;
	}

	inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.field_count = ing_rule_info->rulecfg.chkrcfg.rules.field_count;
	inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.priority = ing_rule_info->rulecfg.chkrcfg.priority;

	for(field_iter = 0; field_iter < inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.field_count; field_iter++) {
		switch(ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type)
		{
			case QOS_MGR_FLD_SRC_MAC:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				memcpy(&inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].src_mac,
					&ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_mac,
					ETH_ALEN * sizeof(inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].src_mac[0]));


				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				memcpy(&fields_ptr[field_iter].src_mac, &ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_mac,
					ETH_ALEN * sizeof(ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_mac[0]));
			break;

			case QOS_MGR_FLD_DST_MAC:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				memcpy(&inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].dst_mac,
					&ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_mac,
					ETH_ALEN * sizeof(inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].dst_mac[0]));


				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				memcpy(&fields_ptr[field_iter].dst_mac, &ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_mac,
					ETH_ALEN * sizeof(ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_mac[0]));
			break;

			case QOS_MGR_FLD_ETH_TYPE:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].eth_type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].eth_type;
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;

				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].eth_type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].eth_type;
			break;

			case QOS_MGR_FLD_IPV4_SRC_IP:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].src_ip.v4
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_ip.v4;
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;

				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].src_ip.v4 = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_ip.v4;
			break;

			case QOS_MGR_FLD_IPV4_DST_IP:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].dst_ip.v4
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_ip.v4;
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;

				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].dst_ip.v4 = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_ip.v4;
			break;

			case QOS_MGR_FLD_L4_SRC_PORT:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].src_port
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_port;
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].src_port = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].src_port;
			break;

			case QOS_MGR_FLD_L4_DST_PORT:
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].dst_port
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_port;
				inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.fields[field_iter].type
					= ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].type = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].type;
				fields_ptr[field_iter].dst_port = ing_rule_info->rulecfg.chkrcfg.rules.fields[field_iter].dst_port;
			break;

			default:

			break;
		}
	}

	if(pp_gpid_group_rule_add(inggrp_list_item->pp_grp_id,
					inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.priority,
					fields_ptr,
					inggrp_list_item->ing_class_rule_cfg[inggrp_list_item->num_ing_rules].rulecfg.chkrcfg.rules.field_count) != 0) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "Failed to add the rule in the PP ingress group: [%d]\n", inggrp_list_item->pp_grp_id);
		qos_mgr_free(fields_ptr);
		return QOS_MGR_FAILURE;
	}

	qos_mgr_free(fields_ptr);
	inggrp_list_item->num_ing_rules++;

	return QOS_MGR_SUCCESS;
}
#endif

static int32_t qos_mgr_set_ing_pce_rule(QOS_MGR_CMD_ADD_CLASSIFIER_INFO* pce_rule_info)
{
	GSW_PCE_rule_t pcecfg;
	int field_iter;
	int ret;

	qos_mgr_memset(&pcecfg, 0, sizeof(GSW_PCE_rule_t));
	field_iter = 0;
	ret = 0;

	if(pce_rule_info->rulecfg.pcecfg.region > 0) {
		pcecfg.region = pce_rule_info->rulecfg.pcecfg.region;
	}

	if(pce_rule_info->rulecfg.pcecfg.bEnable != 0) {
		pcecfg.pattern.bEnable = pce_rule_info->rulecfg.pcecfg.bEnable;
	}

	if(pce_rule_info->rulecfg.pcecfg.bPortIdEnable != 0) {
		pcecfg.pattern.bPortIdEnable = pce_rule_info->rulecfg.pcecfg.bPortIdEnable;
		pcecfg.pattern.nPortId = pce_rule_info->rulecfg.pcecfg.nPortId;
	}

	if(pce_rule_info->rulecfg.pcecfg.bPortIdEnable != 0) {
		pcecfg.pattern.bPortIdEnable = pce_rule_info->rulecfg.pcecfg.bPortIdEnable;
		pcecfg.pattern.nPortId = pce_rule_info->rulecfg.pcecfg.nPortId;
	}

	pcecfg.action.eTrafficClassAction = pce_rule_info->rulecfg.pcecfg.eTrafficClassAction;
	pcecfg.action.nTrafficClassAlternate = pce_rule_info->rulecfg.pcecfg.nTrafficClassAlternate;

	for(field_iter = 0; field_iter < pce_rule_info->rulecfg.pcecfg.rules.field_count; field_iter++) {
		switch(pce_rule_info->rulecfg.pcecfg.rules.fields[field_iter].type) {
			case QOS_MGR_FLD_ETH_TYPE:
				pcecfg.pattern.bEtherTypeEnable = pce_rule_info->rulecfg.pcecfg.bEtherTypeEnable;
				pcecfg.pattern.nEtherType = pce_rule_info->rulecfg.pcecfg.rules.fields[field_iter].eth_type;
				pcecfg.pattern.nEtherTypeMask = pce_rule_info->rulecfg.pcecfg.nEtherTypeMask;
			break;

			case QOS_MGR_FLD_IPV4_SRC_IP:
				pcecfg.pattern.eSrcIP_Select = pce_rule_info->rulecfg.pcecfg.eSrcIP_Select;
				pcecfg.pattern.nSrcIP.nIPv4 = pce_rule_info->rulecfg.pcecfg.rules.fields[field_iter].src_ip.v4;
				pcecfg.pattern.nSrcIP_Mask = pce_rule_info->rulecfg.pcecfg.nSrcIP_Mask;
			break;

			case QOS_MGR_FLD_IPV4_DST_IP:
				pcecfg.pattern.eDstIP_Select = pce_rule_info->rulecfg.pcecfg.eDstIP_Select;
				pcecfg.pattern.nDstIP.nIPv4 = pce_rule_info->rulecfg.pcecfg.rules.fields[field_iter].dst_ip.v4;
				pcecfg.pattern.nDstIP_Mask = pce_rule_info->rulecfg.pcecfg.nDstIP_Mask;
			break;

			default:

			break;
		}
	}

	if ( (ret = qos_hal_generic_hook(QOS_MGR_GENERIC_HAL_QOS_ADD_CLASSIFIER_CFG, &pcecfg, 0)) != QOS_MGR_SUCCESS ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"%s %d failed !!!\n",__FUNCTION__,__LINE__);
	}

	return 0;
}

int32_t qos_mgr_ioctl_set_classifier(unsigned int cmd, unsigned long arg, QOS_MGR_CMD_DATA *cmd_info)
{
	int ret = QOS_MGR_FAILURE;

	qos_mgr_memset(&cmd_info->qos_add_classifier_info, 0, sizeof(cmd_info->qos_add_classifier_info));

	if ( qos_mgr_copy_from_user( &cmd_info->qos_add_classifier_info, (void *)arg, sizeof(cmd_info->qos_add_classifier_info)) != 0 )
		return QOS_MGR_FAILURE;

	switch(cmd_info->qos_add_classifier_info.type)
	{
		case QOS_MGR_CHECKER:
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
			qos_mgr_set_ing_class_rule(&cmd_info->qos_add_classifier_info);
#endif
		break;

		case QOS_MGR_PCE:
			qos_mgr_set_ing_pce_rule(&cmd_info->qos_add_classifier_info);
		break;

		default:

		break;
	};

	return ret;
}

int32_t qos_mgr_ioctl_get_qos_mib(unsigned int cmd, unsigned long arg,
                                        QOS_MGR_CMD_DATA *cmd_info)
{
	int ret = QOS_MGR_FAILURE;
 
	qos_mgr_memset(&cmd_info->qos_mib_info, 0,
						sizeof(cmd_info->qos_mib_info));
	if (qos_mgr_copy_from_user(&cmd_info->qos_mib_info, (void *)arg,
			sizeof(cmd_info->qos_mib_info)) != 0) {
				return QOS_MGR_FAILURE;
	}
	ret = qos_mgr_add_qos_mib(cmd_info->qos_mib_info.ifname, &cmd_info->qos_mib_info);
	if (ret != QOS_MGR_SUCCESS) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s %d failed !!!\n",
								__func__, __LINE__);
	}
	QOS_HAL_DEBUG_MSG(QOS_DEBUG_TRACE, "%s %d cmd_info->data:ifname=%s, portid=%d, rx= %lld, tx=%lld, drp=%lld\n",
						__func__, __LINE__,
						cmd_info->qos_mib_info.ifname,
						cmd_info->qos_mib_info.portid,
						cmd_info->qos_mib_info.mib.total_rx_pkt,
						cmd_info->qos_mib_info.mib.total_tx_pkt,
						cmd_info->qos_mib_info.mib.fast_path_total_pkt_drop_cnt);
	if (qos_mgr_copy_to_user((void *)arg, &cmd_info->qos_mib_info,
								sizeof(cmd_info->qos_mib_info)) != 0) {
			QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR, "%s %d failed!!!\n",
								__func__, __LINE__);
			return QOS_MGR_FAILURE;
	}
	return ret;
}

/*
############# QOS RATE LIMIT FUNCTION DEFINITIONS : END################
*/

int32_t qos_mgr_api_create(void)
{
	if ( qos_mgr_lock_init(&g_qos_queue_lock) ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed in creating lock for qos queue list.\n");
		goto QOS_MGR_CREATE_FAIL;
	}
	if ( qos_mgr_lock_init(&g_qos_shaper_lock) ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed in creating lock for qos shaper list.\n");
		goto QOS_MGR_CREATE_FAIL;
	}
	if ( qos_mgr_lock_init(&g_qos_inggrp_list_lock) ) {
		QOS_HAL_DEBUG_MSG(QOS_DEBUG_ERR,"Failed in creating lock for qos ingress list.\n");
		goto QOS_MGR_CREATE_FAIL;
	}

#if IS_ENABLED(CONFIG_QOS_MGR_TC_SUPPORT)
	qos_mgr_tc_init();
#endif
#if DP_EVENT_CB
	qos_manager_register_event_dp();
#endif
	return QOS_MGR_SUCCESS;
QOS_MGR_CREATE_FAIL:
	qos_mgr_api_destroy();
	return QOS_MGR_EIO;
}

void qos_mgr_api_destroy(void)
{
#if IS_ENABLED(CONFIG_QOS_MGR_TC_SUPPORT)
	qos_mgr_tc_exit();
#endif
	qos_mgr_queue_free_list();
	qos_mgr_shaper_free_list();
	qos_mgr_inggrp_free_list();
	qos_mgr_lock_destroy(&g_qos_queue_lock);
	qos_mgr_lock_destroy(&g_qos_shaper_lock);
	qos_mgr_lock_destroy(&g_qos_inggrp_list_lock);
#if DP_EVENT_CB
	qos_mgr_wmm_free_list();
	qos_manager_deregister_event_dp();
#endif
}

EXPORT_SYMBOL(qos_mgr_ioctl_add_qos_queue);
EXPORT_SYMBOL(qos_mgr_ioctl_modify_qos_queue);
EXPORT_SYMBOL(qos_mgr_ioctl_delete_qos_queue);
EXPORT_SYMBOL(qos_mgr_ioctl_qos_init_cfg);
