/*******************************************************************************

  Copyright © 2020-2023 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/if.h>

#include <net/ppa/ppa_api.h>
#include "ppa_api_soc_al.h"
#include "ppa_api_netif.h"

/*
 * Update the interface MIB for the logical port
 * and also the under-laying interfaces
 */
int ppa_update_base_inf_mib(PPA_ITF_MIB_INFO *mib_tmp, uint32_t hal_id)
{
	struct netif_info *ifinfo_tmp =  mib_tmp->ifinfo;
	struct intf_mib *p = &(mib_tmp->mib);
	uint64_t diff_rx_pkts, diff_tx_pkts;
	uint64_t diff_rx_bytes, diff_tx_bytes;

	/* 42-bit packet counter */
	diff_rx_pkts =
		mib_wraparound(ifinfo_tmp->hw_accel_stats.rx_pkts_prev[hal_id],
			       p->rx_packets, 42);
	ifinfo_tmp->hw_accel_stats.rx_pkts += diff_rx_pkts;
	ifinfo_tmp->hw_accel_stats.rx_pkts_prev[hal_id] = p->rx_packets;

	diff_tx_pkts =
		mib_wraparound(ifinfo_tmp->hw_accel_stats.tx_pkts_prev[hal_id],
			       p->tx_packets, 42);
	ifinfo_tmp->hw_accel_stats.tx_pkts += diff_tx_pkts;
	ifinfo_tmp->hw_accel_stats.tx_pkts_prev[hal_id] = p->tx_packets;

	/* 52-bit byte counter */
	diff_rx_bytes =
		mib_wraparound(ifinfo_tmp->hw_accel_stats.rx_bytes_prev[hal_id],
			       p->rx_bytes, 52);
	ifinfo_tmp->hw_accel_stats.rx_bytes += diff_rx_bytes;
	ifinfo_tmp->hw_accel_stats.rx_bytes_prev[hal_id] = p->rx_bytes;

	diff_tx_bytes =
		mib_wraparound(ifinfo_tmp->hw_accel_stats.tx_bytes_prev[hal_id],
			       p->tx_bytes, 52);
	ifinfo_tmp->hw_accel_stats.tx_bytes += diff_tx_bytes;
	ifinfo_tmp->hw_accel_stats.tx_bytes_prev[hal_id] = p->tx_bytes;

	/* TODO: Byte counter adjustment based on each layer overhead */

	return PPA_SUCCESS;
}

void
ppa_netif_get_stats64(struct net_device *dev,
		      struct rtnl_link_stats64 *storage)
{
	PPA_NETIF_ACCEL_STATS ppa_stats;

	memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

	/* get PPA HW and SW accelerated session stats */
#if defined(PPA_IF_MIB) && PPA_IF_MIB
	ppa_get_netif_accel_stats(dev->name, &ppa_stats,
		(PPA_F_NETIF_HW_ACCEL | PPA_F_NETIF_SW_ACCEL), storage);
#endif
	/* add with ppa HW and SW accelerated stats */
	storage->rx_bytes += ppa_stats.hw_accel_stats.rx_bytes +
		ppa_stats.sw_accel_stats.rx_bytes;
	storage->tx_bytes += ppa_stats.hw_accel_stats.tx_bytes +
		ppa_stats.sw_accel_stats.tx_bytes;

	storage->rx_packets += ppa_stats.hw_accel_stats.rx_pkts +
		ppa_stats.sw_accel_stats.rx_pkts;
	storage->tx_packets += ppa_stats.hw_accel_stats.tx_pkts +
		ppa_stats.sw_accel_stats.tx_pkts;
}

bool is_pmac_rxcsum_enabled(int32_t port)
{
	int ret;
	struct core_ops *ops;
	GSW_QoS_queuePort_t q_port = {0};
	GSW_PMAC_Glbl_Cfg_t glbl_cfg = {0};

	ops = gsw_get_swcore_ops(0);
	if (!ops) {
		pr_err("failed in getting SW Core Ops\n");
		return false;
	}

	q_port.nPortId = port;
	q_port.nTrafficClassId = 0;
	ret = ops->gsw_qos_ops.QoS_QueuePortGet(ops, &q_port);
	if (ret) {
		pr_err("failed in getting QoS QueuePort: %d\n", ret);
		return false;
	}

	glbl_cfg.nPmacId = 2; /* to PPv4 */
	ret = ops->gsw_pmac_ops.Pmac_Gbl_CfgGet(ops, &glbl_cfg);
	if (ret) {
		pr_err("failed in getting PMAC Gbl Cfg: %d\n", ret);
		return false;
	}

	pr_debug("%s port:%d bEnableIngressPceBypass:%d bIPTransChkVerDis:%d\n",
		 __func__, port, q_port.bEnableIngressPceBypass,
		 glbl_cfg.bIPTransChkVerDis);

	return (!q_port.bEnableIngressPceBypass && !glbl_cfg.bIPTransChkVerDis);
}
