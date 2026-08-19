/*******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <linux/version.h>
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
	uint32_t diff_rx_pkts, diff_tx_pkts;
	PPA_NETIF *netif_tmp = NULL;
	struct netif_info *ifinfo_base = NULL;

	if (p->rx_packets == 0)
		ifinfo_tmp->hw_accel_stats.rx_pkts_prev[hal_id] = 0;

	diff_rx_pkts =
		p->rx_packets - ifinfo_tmp->hw_accel_stats.rx_pkts_prev[hal_id];
	ifinfo_tmp->hw_accel_stats.rx_pkts += diff_rx_pkts;
	ifinfo_tmp->hw_accel_stats.rx_pkts_prev[hal_id] = p->rx_packets;

	if (p->tx_packets == 0)
		ifinfo_tmp->hw_accel_stats.tx_pkts_prev[hal_id] = 0;

	diff_tx_pkts =
		p->tx_packets - ifinfo_tmp->hw_accel_stats.tx_pkts_prev[hal_id];
	ifinfo_tmp->hw_accel_stats.tx_pkts += diff_tx_pkts;
	ifinfo_tmp->hw_accel_stats.tx_pkts_prev[hal_id] = p->tx_packets;

	/* Check if the interface is in bridge,
	 * if yes update the packet count for the bridge interface
	 */
	netif_tmp = ifinfo_tmp->netif;
	if (ppa_is_netif_bridged(netif_tmp)) {
		if (ifinfo_tmp->brif != NULL) {
			__ppa_netif_lookup(ifinfo_tmp->brif->name,
					   &ifinfo_base);
			if (ifinfo_base != NULL) {
				ifinfo_base->hw_accel_stats.rx_pkts +=
					diff_rx_pkts;
				ifinfo_base->hw_accel_stats.tx_pkts +=
					diff_tx_pkts;
				ppa_netif_put(ifinfo_base);
			}
		}
	}

	/* Handle the byte count adjustment */
	ifinfo_tmp->hw_accel_stats.rx_bytes -=
		diff_rx_pkts * (1500 - ifinfo_tmp->mtu);
	ifinfo_tmp->hw_accel_stats.tx_bytes -=
		diff_tx_pkts * (1500 - ifinfo_tmp->mtu);

	return PPA_SUCCESS;
}

#if KERNEL_VERSION(4, 10, 17) < LINUX_VERSION_CODE
void
ppa_netif_get_stats64(struct net_device *dev,
		      struct rtnl_link_stats64 *storage)
#else
struct rtnl_link_stats64*
ppa_netif_get_stats64(struct net_device *dev,
		      struct rtnl_link_stats64 *storage)
#endif
{
	PPA_NETIF_ACCEL_STATS ppa_stats;

	/* NOTE : Linux driver maintains only MIPS counters */

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

#if KERNEL_VERSION(4, 10, 17) >= LINUX_VERSION_CODE
	return storage;
#endif
}

bool is_pmac_rxcsum_enabled(int32_t port)
{
	return true;
}
