/*
 * Copyright (C) 2022 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PPv4 Network adapter design under test
 */

#include <linux/version.h>
#include <generated/autoconf.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/net.h>

/*
 *	Chip Specific Head File
 */
#include <net/ppa/ppa_api.h>
#include <linux/pp_api.h>
#include <linux/pktprs.h>
#include <net/datapath_api.h>
#if IS_ENABLED(CONFIG_SOC_LGM)
#include <net/toe_np_lro.h>
#else
#include <net/intel_np_lro.h>
#endif
#include <net/datapath_api_qos.h>
#include "pp_network_adapter_internal.h"

/*To be used for all the uCs */
struct dut_nf_node {
	uint16_t gpid;		/*allocated gpid */
	uint16_t qid;		/*allocated qid*/
	uint16_t subif;		/*allocated subif*/
	uint16_t uc_id;		/*uC id returned by pp*/
	struct net_device *dev;	/*dummy netdevice*/
};

#define DUT_INGRESS_PKTHDR_SIZE (256)
static struct dut_nf_node g_dut_dev = {0};
static 	u32 dbg_pp_desc[DUT_HW_DESC_SIZE_WORDS];	
static char dut_dbg_pkt[FRAG_NUM][DUT_INGRESS_PKTHDR_SIZE];
static u16 dut_dbg_pkt_size[FRAG_NUM] = {0};
static u32 dbg_pkt_index = 0;
static bool dbg_pkt_exist=0;
static u16 dbg_frag_index = 0x1;
static u32 ack = 0x1;
static bool dut_dbg_pkt_ack = false;
static u16 dut_tcp_sessions = 1;
static u16 source_port = 0;

struct dut_dev_priv{
	struct module	*owner;
	dp_subif_t	dp_subif;
	struct rtnl_link_stats64 stats;
};

void dut_pkt_desc_get(unsigned char *desc)
{
	if (unlikely(!desc))
		return;

	memcpy(desc, dbg_pp_desc, sizeof(dbg_pp_desc));
}

void dut_dbg_pkt_set(u8 index, unsigned char *pkt, size_t pkt_size, bool is_tdox, u16 sessions)
{
	if (pkt_size <= DUT_INGRESS_PKTHDR_SIZE) {
		dut_dbg_pkt_size[index] = pkt_size;
		memcpy(dut_dbg_pkt[index], pkt, pkt_size);
		dbg_pkt_index = 0;
		dbg_frag_index = 0;
		dut_dbg_pkt_ack = is_tdox;
		dut_tcp_sessions = sessions;
		source_port = 0;
		if (dut_dbg_pkt_size[FRAG_0] || dut_dbg_pkt_size[FRAG_1])
			dbg_pkt_exist = true;
		else
			dbg_pkt_exist = false;
	} else {
		pr_info("Packet header configuration support up to %d\n", 
			DUT_INGRESS_PKTHDR_SIZE);
	}

}

static void dut_dev_get_stats64(struct net_device *dev,
		struct rtnl_link_stats64 *stats)
{
	struct dut_dev_priv *priv;
	priv = netdev_priv(g_dut_dev.dev);

	if (priv) {
		stats->tx_packets = priv->stats.tx_packets;
		stats->tx_bytes = priv->stats.tx_bytes;
	}
}

static int dut_dev_xmit(struct sk_buff *skb, struct net_device* dev)
{
	u32 flags;
	u16 orig_source_port;
	u16 orig_dest_port;
	int ret;
	int len;
	int index;
	struct dut_dev_priv *priv = netdev_priv(g_dut_dev.dev);
	struct pmac_tx_hdr pmac = {0};
	pmac.tcp_chksum = 0;
	pmac.class_en = 1;

	if (dbg_pkt_exist) {
		if (dut_dbg_pkt_size[FRAG_1]) {
			index = dbg_pkt_index % FRAG_NUM;
			dbg_pkt_index++;
		} else {
			index = 0;
		}
		memcpy(skb->data, dut_dbg_pkt[index],  dut_dbg_pkt_size[index]);
		if (dut_dbg_pkt_size[FRAG_1]) {
			ip_hdr(skb)->id = dbg_frag_index;
			/* take new ip len + L2 header length */
			skb->len = ntohs(ip_hdr(skb)->tot_len) + ETH_HLEN;
			if (index == FRAG_1) {
				dbg_frag_index++;
			}
		}
		if (dut_dbg_pkt_ack == true) {
			tcp_hdr(skb)->ack = ack;
			ack++;
			if (dut_tcp_sessions > 1) {
				orig_source_port = ntohs(tcp_hdr(skb)->source);
				orig_dest_port = ntohs(tcp_hdr(skb)->dest);
				
				tcp_hdr(skb)->source = ntohs(orig_source_port + source_port);
				tcp_hdr(skb)->dest = ntohs(orig_dest_port + source_port);
				pr_debug("%d %d\n", ntohs(tcp_hdr(skb)->source) ,ntohs(tcp_hdr(skb)->dest) );

				source_port++;
				if (source_port >= dut_tcp_sessions) {
					source_port = 0;
				}
			}
		}
	}

	switch (skb->protocol) {
		case htons(ETH_P_IP):
			if (ip_hdr(skb)->protocol == IPPROTO_TCP)
				pmac.tcp_type = TCP_OVER_IPV4;
			else if (ip_hdr(skb)->protocol == IPPROTO_UDP) 
				pmac.tcp_type = UDP_OVER_IPV4;
			break;
		case htons(ETH_P_IPV6):
			if (ipv6_hdr(skb)->nexthdr == NEXTHDR_TCP)
				pmac.tcp_type = TCP_OVER_IPV6;
			else if (ipv6_hdr(skb)->nexthdr == NEXTHDR_UDP)
				pmac.tcp_type = UDP_OVER_IPV6;
	}

	pmac.ip_offset = ((u32)skb_network_offset(skb)) >> 1;
	pmac.tcp_h_offset = (skb_network_header_len(skb)) >> 2;

	if (skb_headroom(skb) < sizeof(struct pmac_tx_hdr)) {
		consume_skb(skb);
		priv->stats.tx_dropped++;
		return -1;
	}

	/* Copy the pmac header to the begining og skb->data*/
	memcpy((void*)((unsigned long)skb->data - sizeof(struct pmac_tx_hdr)),
			&pmac, sizeof(struct pmac_tx_hdr));

	skb->ip_summed = CHECKSUM_NONE;

	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = priv->dp_subif.gpid;
	/* Set pmac and pre_l2 flags*/
	((struct dma_tx_desc_1 *)&skb->DW1)->field.pmac = 1;
	((struct dma_tx_desc_1 *)&skb->DW1)->field.pre_l2 = 1;
	((struct dma_tx_desc_1 *)&skb->DW1)->field.color = 1;
	/* set subifid*/
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id = priv->dp_subif.subif;

	pr_debug("dut_dev_xmit %d %d %d %d\n",skb->len ,priv->dp_subif.gpid, 
				priv->dp_subif.subif, priv->dp_subif.port_id);

	len = skb->len;

	flags = DP_TX_NEWRET | DP_TX_BYPASS_FLOW | DP_TX_WITH_PMAC;
	ret = dp_xmit(g_dut_dev.dev, &priv->dp_subif, skb, skb->len, flags);
	if (!ret) {
		priv->stats.tx_packets++;
		priv->stats.tx_bytes += len;
	} else if (ret != NETDEV_TX_BUSY) {
		priv->stats.tx_dropped++;
	}
	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static void dut_dev_tx_timeout(struct net_device *dev, unsigned int txqueue)
#else
static void dut_dev_tx_timeout(struct net_device *dev)
#endif
{
	unsigned int tx_q = 0;

	while (tx_q < dev->num_tx_queues) {
		if (netif_tx_queue_stopped(netdev_get_tx_queue(dev, tx_q)))
			pr_debug("dev: %s Tx Timeout txq: %d\n",
				 dev->name, tx_q);
		tx_q++;
	}
	netif_tx_wake_all_queues(dev);
}

static struct net_device_ops dut_dev_ops = {
	.ndo_start_xmit = dut_dev_xmit,
	.ndo_get_stats64 = dut_dev_get_stats64,
	.ndo_tx_timeout = dut_dev_tx_timeout,
};

static void dut_dev_setup(struct net_device *dev)
{
	dev->tx_queue_len = 1000;
	dev->netdev_ops = &dut_dev_ops;
	dev->mtu = 1500;
	eth_hw_addr_random(dev);
	strcpy(dev->name, "dutdev0");
}

int32_t dut_dev_rx_handler(struct net_device *rxif, struct net_device *txif,
        struct sk_buff *skb, int32_t len)
{
	memcpy(dbg_pp_desc, skb->buf_base, sizeof(dbg_pp_desc));
	consume_skb(skb);
	return 0;
}

static dp_cb_t dut_dp_cb = {
	.rx_fn = dut_dev_rx_handler,
};

static inline void uninit_dut_port(void)
{
	struct dp_spl_cfg dp_con = {0};

	dp_con.flag = DP_F_DEREGISTER;
	dp_con.type = DP_SPL_PP_DUT;
	dp_con.spl_id = g_dut_dev.uc_id;

	if (dp_spl_conn(0, &dp_con))
		pr_err("Deregister of dp spl conn for litepath failed\n");
	unregister_netdev(g_dut_dev.dev);
	free_netdev(g_dut_dev.dev);
	g_dut_dev.dev = NULL;
}

int32_t dut_port_init(void)
{
	int32_t ret = 0;
	struct dut_dev_priv *priv = NULL;
	struct dp_spl_cfg dp_con = {0};
	struct dp_qos_q_logic q_logic = {0};
	struct dp_spl_cfg tso_dut_dp_con = {0};
	struct dp_queue_map_set q_map = {0};
	
	memset(&g_dut_dev, 0, sizeof(g_dut_dev));
	/*Allocate netdevice */
	g_dut_dev.dev = alloc_etherdev_mq(sizeof(struct dut_dev_priv), 8);

	if (!g_dut_dev.dev) {
		pr_err("alloc_netdev failed for dutdev0\n");
		return -1;
	}

	dut_dev_setup(g_dut_dev.dev);

	priv = netdev_priv(g_dut_dev.dev);
	if (!priv) {
		pr_err("priv is NULL\n");
		return -1;
	}
	priv->owner = THIS_MODULE;

	/*Register netdevice*/
	if (register_netdev(g_dut_dev.dev)) {
		free_netdev(g_dut_dev.dev);
		g_dut_dev.dev = NULL;
		pr_err("register device \"dutdev0\" failed\n");
		return -1;
	}

	/*call the dp to allocate special connection */
	/*******************************************************/
	dp_con.flag = 0;/*DP_F_REGISTER;*/
	dp_con.type = DP_SPL_PP_DUT;
	dp_con.f_subif = 1;
	dp_con.f_gpid = 1;
	dp_con.f_hostif = 1;

	/*assign the netdevice */
	dp_con.dev = g_dut_dev.dev;

	/*callback to be invoked by dp when packet is received for this GPID*/
	dp_con.dp_cb = &dut_dp_cb;

	if ((ret = dp_spl_conn(0, &dp_con))) {
		pr_err("Regsiter spl conn for dut failed\n");
		return -1;
	}

	/*Store the gpid and uc_id*/
	g_dut_dev.uc_id = dp_con.spl_id;
	g_dut_dev.gpid = dp_con.gpid;
	g_dut_dev.subif = dp_con.subif;

	priv->dp_subif.port_id = dp_con.dp_port;
	priv->dp_subif.subif = dp_con.subif;
	priv->dp_subif.gpid = dp_con.gpid;

	/* Enable dp_rx*/
	if ((ret = dp_rx_enable(g_dut_dev.dev, g_dut_dev.dev->name, 1))) {
		pr_err("Enable rx_fn for litepath failed\n");
		return -1;
	}

	rtnl_lock();
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
	if (dev_open(g_dut_dev.dev)) {
#else
	if (dev_open(g_dut_dev.dev, NULL)) {
#endif
		pr_err( "[%s]: [%s] dev_open error! \n",__func__, g_dut_dev.dev->name);
		rtnl_unlock();
		uninit_dut_port();
		return -1;
	}
	rtnl_unlock();

	/* Setup Queue map table for lpdev_tx */
	q_map.map.dp_port = 0; 			/* CPU portid */
	q_map.map.subif = dp_con.subif;		/* Subif id */

	/* need to setup the queue map table */
	dp_spl_conn_get(0, DP_SPL_TOE, &tso_dut_dp_con, 1);

	/* Verify that the toe driver succeeds to load */
	if (tso_dut_dp_con.igp->egp) {
		q_map.q_id = tso_dut_dp_con.igp->egp->qid;
		q_map.mask.class = 1;			/* TC dont care*/
		q_map.mask.flowid = 1;			/* Flowid dont care*/
		q_map.mask.egflag = 1;			/* Flowid dont care*/

		if (dp_queue_map_set(&q_map, 0) == DP_FAILURE)
			pr_err("dp_queue_map_set failed for Queue [%d]\n",
			    dp_con.egp[1].qid);
	} else
		pr_err("TOE driver is not ready\n");

	/*Egress port qid in this case CPU queueid*/;
	q_logic.q_id = 2;

	/* physical to logical qid */
	if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
		pr_err("%s:%d ERROR Failed to Logical Queue Id\n", __func__, __LINE__);
		return -1;
	}

	/*Store the logical qid */
	g_dut_dev.qid = q_logic.q_logic_id;

	return ret;
}
