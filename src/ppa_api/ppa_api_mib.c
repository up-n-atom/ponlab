/*******************************************************************************
 **
 ** FILE NAME		: ppa_api_mib.c
 ** PROJECT		: PPA
 ** MODULES		: PPA API (Generic MIB APIs)
 **
 ** DATE		: 3 NOV 2008
 ** AUTHOR		: Xu Liang
 ** DESCRIPTION		: PPA Protocol Stack Hook API Miscellaneous Functions
 ** COPYRIGHT		:   Copyright (c) 2020-2023 MaxLinear, Inc.
 **                         Copyright (c) 2009
 **			    Lantiq Deutschland GmbH
 **			    Am Campeon 3; 85579 Neubiberg, Germany
 **
 **   For licensing information, see the file 'LICENSE' in the root folder of
 **   this software module.
 **
 ** HISTORY
 ** $Date		$Author		 $Comment
 ** 03 NOV 2008  Xu Liang		Initiate Version
 *******************************************************************************/
/*
 * ####################################
 *			  Head File
 * ####################################
 */
/*
 *  Common Head File
 */
#include <linux/version.h>
#include <generated/autoconf.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>

#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/ppa/ppa_hal_wrapper.h>
#include "ppa_api_misc.h"
#include "ppa_api_netif.h"
#include "ppa_api_session.h"
#include "ppa_api_hal_selector.h"
#include "ppa_api_mib.h"
#include "ppa_api_qos.h"


PPA_LOCK  g_general_lock;
/*Port MIB varabile*/
static uint32_t last_jiffy_port_mib;
static PPA_PORT_MIB g_port_mib_accumulated;		/* accumulatd mib counter */
static PPA_PORT_MIB g_port_mib_last_instant_read;	/* last instant read counter */
static PPA_PORT_MIB g_port_mib_accumulated_last;	/* last accumulatd mib counter */


#if IS_ENABLED(CONFIG_PPA_QOS)
/*QoS queue MIB varabiles*/
static PPA_QOS_STATUS g_qos_mib_accumulated[PPA_MAX_PORT_NUM];		/* accumulatd mib counter */
static PPA_QOS_STATUS g_qos_mib_last_instant_read[PPA_MAX_PORT_NUM];	/* last instant read counter */
static uint32_t last_jiffy_qos_mib[PPA_MAX_PORT_NUM];
static PPA_QOS_STATUS g_qos_mib_accumulated_last[PPA_MAX_PORT_NUM];	/* last accumulatd mib counter */
#endif

void reset_local_mib(void)
{
	uint32_t curr_jiffy=jiffies;
#if IS_ENABLED(CONFIG_PPA_QOS)
	int i;
#endif

	ppa_lock_get(&g_general_lock);
	last_jiffy_port_mib = curr_jiffy;
	ppa_memset( &g_port_mib_accumulated, 0, sizeof(g_port_mib_accumulated));
	ppa_memset( &g_port_mib_last_instant_read, 0, sizeof(g_port_mib_last_instant_read));
	ppa_memset( &g_port_mib_accumulated_last, 0, sizeof(g_port_mib_accumulated_last));

#if IS_ENABLED(CONFIG_PPA_QOS)
	for(i=0; i<PPA_MAX_PORT_NUM; i++ )
		last_jiffy_qos_mib[i] = curr_jiffy;
	ppa_memset( &g_qos_mib_accumulated, 0, sizeof(g_qos_mib_accumulated));
	ppa_memset( &g_qos_mib_last_instant_read, 0, sizeof(g_qos_mib_last_instant_read));
	ppa_memset( &g_qos_mib_accumulated_last, 0, sizeof(g_qos_mib_accumulated_last));
#endif
	ppa_lock_release(&g_general_lock);

}

#if IS_ENABLED(CONFIG_PPA_QOS)
static void update_port_mib64_item(uint64_t *curr, uint64_t *last, uint64_t *accumulated)
{
	if( *curr >= *last)
		*accumulated += (*curr - *last);
	else
		*accumulated += ((uint64_t)*curr + (uint64_t)WRAPROUND_32BITS - *last);
	*last = *curr;
}
#endif

#if IS_ENABLED(CONFIG_PPA_QOS)
/*note, so far only ioctl will set rate_flag to 1, otherwise it will be zero in ppa timer*/
int32_t ppa_update_qos_mib(PPA_QOS_STATUS *status, uint32_t rate_flag, uint32_t flag)
{
	uint32_t i, curr_jiffy, port_id;
	int32_t num;

	if( !status ) return PPA_FAILURE;
	if( status->qos_queue_portid >= PPA_MAX_PORT_NUM ) return PPA_FAILURE;

	num = ppa_get_qos_qnum(status->qos_queue_portid, 0 );
	if( num <= 0 ) {
		return PPA_FAILURE;
	}
	ppa_lock_get(&g_general_lock);
	if( num > PPA_MAX_QOS_QUEUE_NUM )
		num = PPA_MAX_QOS_QUEUE_NUM;
	status->max_buffer_size = num;
	port_id = status->qos_queue_portid;

	if( ppa_drv_get_qos_status( status, flag) != PPA_SUCCESS) {
		ppa_lock_release(&g_general_lock);
		ppa_debug(DBG_ENABLE_MASK_QOS,"ppa_drv_get_qos_status failed\n");
		return PPA_FAILURE;
	}
	curr_jiffy = jiffies;

	for(i=0; i<status->max_buffer_size; i++) {
		update_port_mib64_item( &status->mib[i].mib.total_rx_pkt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.total_rx_pkt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.total_rx_pkt);
		update_port_mib64_item( &status->mib[i].mib.total_rx_bytes,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.total_rx_bytes,
			&g_qos_mib_accumulated[port_id].mib[i].mib.total_rx_bytes);
		update_port_mib64_item( &status->mib[i].mib.total_tx_pkt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.total_tx_pkt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_pkt);
		update_port_mib64_item( &status->mib[i].mib.total_tx_bytes,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.total_tx_bytes,
			&g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_bytes);
		update_port_mib64_item( &status->mib[i].mib.cpu_path_small_pkt_drop_cnt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.cpu_path_small_pkt_drop_cnt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.cpu_path_small_pkt_drop_cnt);
		update_port_mib64_item( &status->mib[i].mib.cpu_path_total_pkt_drop_cnt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.cpu_path_total_pkt_drop_cnt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.cpu_path_total_pkt_drop_cnt);
		update_port_mib64_item( &status->mib[i].mib.fast_path_small_pkt_drop_cnt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.fast_path_small_pkt_drop_cnt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.fast_path_small_pkt_drop_cnt);
		update_port_mib64_item( &status->mib[i].mib.fast_path_total_pkt_drop_cnt,
			&g_qos_mib_last_instant_read[port_id].mib[i].mib.fast_path_total_pkt_drop_cnt,
			&g_qos_mib_accumulated[port_id].mib[i].mib.fast_path_total_pkt_drop_cnt);

		if( rate_flag ) {
			status->mib[i].mib.tx_diff =
				( g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_bytes >=
				g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_bytes )?
				(g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_bytes -
				g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_bytes) :
				(g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_bytes +
				(uint64_t)WRAPROUND_32BITS - g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_bytes);

			status->mib[i].mib.tx_diff_L1 = status->mib[i].mib.tx_diff +
				(g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_pkt -
				g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_pkt) * status->overhd_bytes;

			status->mib[i].mib.tx_diff_jiffy = ( curr_jiffy > last_jiffy_qos_mib[port_id]) ? \
							   (curr_jiffy - last_jiffy_qos_mib[port_id] ): \
							   (curr_jiffy + (uint32_t )WRAPROUND_32BITS -
								last_jiffy_qos_mib[port_id] );
			
			status->mib[i].mib.sys_hz = HZ;
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,
		"port[%d] queue[%d] bytes=%010llu(%010llu-%010llu) jiffy=%010llu(%010u-%010u) overhead=%010u pkts=%010u\n",
					port_id, i,
					status->mib[i].mib.tx_diff,
					g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_bytes,
					g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_bytes,
					status->mib[i].mib.tx_diff_jiffy,
					curr_jiffy,
					last_jiffy_qos_mib[port_id],
					status->overhd_bytes,
					(uint32_t)(g_qos_mib_accumulated[port_id].mib[i].mib.total_tx_pkt -
					g_qos_mib_accumulated_last[port_id].mib[i].mib.total_tx_pkt));
		}
	}

	if( rate_flag ) {
		g_qos_mib_accumulated_last[port_id] = g_qos_mib_accumulated[port_id];
		last_jiffy_qos_mib[port_id] = curr_jiffy;
	}
	ppa_lock_release(&g_general_lock);
	return PPA_SUCCESS;
}

#endif

#if IS_ENABLED(CONFIG_IPV4_IPV6_COUNTER_SUPPORT)
static void ppa_add_accel_stats(PPA_NETIF_ACCEL_STATS *p_stats, struct netif_info *p_info)
{
	p_stats->hw_accel_stats.rx_pkts_ipv4 += p_info->hw_accel_stats.rx_pkts_ipv4;
	p_stats->hw_accel_stats.tx_pkts_ipv4 += p_info->hw_accel_stats.tx_pkts_ipv4;
	p_stats->hw_accel_stats.rx_pkts_ipv6 += p_info->hw_accel_stats.rx_pkts_ipv6;
	p_stats->hw_accel_stats.tx_pkts_ipv6 += p_info->hw_accel_stats.tx_pkts_ipv6;

	p_stats->sw_accel_stats.rx_pkts_ipv4 += p_info->sw_accel_stats.rx_pkts_ipv4;
	p_stats->sw_accel_stats.tx_pkts_ipv4 += p_info->sw_accel_stats.tx_pkts_ipv4;
	p_stats->sw_accel_stats.rx_pkts_ipv6 += p_info->sw_accel_stats.rx_pkts_ipv6;
	p_stats->sw_accel_stats.tx_pkts_ipv6 += p_info->sw_accel_stats.tx_pkts_ipv6;
}

static int32_t ppa_add_upperif_accel_stats(PPA_NETIF_ACCEL_STATS *p_stats, struct netif_info *p_info)
{
	struct netif_info *upif = NULL;
	int32_t i = 0;

	if (p_info->sub_if_index == 0) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,
				"No upper interface found!\n");
		return PPA_FAILURE;
	}
	else {
		for (i = 0; i < p_info->sub_if_index; i++) {
			if (ppa_netif_lookup(p_info->sub_if_name[i], &upif) == PPA_SUCCESS) {
				ppa_add_accel_stats(p_stats, upif);
				ppa_add_upperif_accel_stats(p_stats, upif);
				ppa_netif_put(upif);
			}
		}
	}
	return PPA_SUCCESS;
}

int32_t ppa_ioctl_get_iface_mib(unsigned int cmd, unsigned long arg, PPA_CMD_DATA *cmd_info)
{
	int res = PPA_FAILURE;
	struct netif_info *ifinfo = NULL;
	PPA_NETIF_ACCEL_STATS *p_stats = NULL;

	ppa_memset(&cmd_info->if_info, 0, sizeof(cmd_info->if_info) );
	res = copy_from_user(&cmd_info->if_info, (void *)arg, sizeof(cmd_info->if_info));
	if (res != PPA_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Memory copy failed\n");
		return PPA_FAILURE;
	}

	res = ppa_netif_lookup(cmd_info->if_info.ifname, &ifinfo);
	if (res == PPA_SUCCESS) {
		p_stats = (PPA_NETIF_ACCEL_STATS *)ppa_malloc(sizeof(PPA_NETIF_ACCEL_STATS));
		if (!p_stats) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "Memory alloc failed\n");
			return PPA_ENOMEM;
		}
		ppa_memset(p_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS) );

		ppa_add_accel_stats(p_stats, ifinfo);
		ppa_add_upperif_accel_stats(p_stats, ifinfo);

		cmd_info->if_info.acc_rx_ipv4 = p_stats->hw_accel_stats.rx_pkts_ipv4
						+ p_stats->sw_accel_stats.rx_pkts_ipv4;
		cmd_info->if_info.acc_tx_ipv4 = p_stats->hw_accel_stats.tx_pkts_ipv4
						+ p_stats->sw_accel_stats.tx_pkts_ipv4;
		cmd_info->if_info.acc_rx_ipv6 = p_stats->hw_accel_stats.rx_pkts_ipv6
						+ p_stats->sw_accel_stats.rx_pkts_ipv6;
		cmd_info->if_info.acc_tx_ipv6 = p_stats->hw_accel_stats.tx_pkts_ipv6
						+ p_stats->sw_accel_stats.tx_pkts_ipv6;

		ppa_netif_put(ifinfo);

		res = ppa_copy_to_user( (void *)arg, &cmd_info->if_info, sizeof(cmd_info->if_info));
		if (res != PPA_SUCCESS) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "Memory copy failed\n");
			res = PPA_FAILURE;
		}
	} else {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Interface lookup failed\n");
	}

	if (p_stats != NULL)
		ppa_free(p_stats);

	return res;
}
EXPORT_SYMBOL(ppa_ioctl_get_iface_mib);
#endif /* CONFIG_IPV4_IPV6_COUNTER_SUPPORT */

EXPORT_SYMBOL(g_general_lock);
#if IS_ENABLED(CONFIG_PPA_QOS)
EXPORT_SYMBOL(ppa_update_qos_mib);
#endif
EXPORT_SYMBOL(reset_local_mib);
