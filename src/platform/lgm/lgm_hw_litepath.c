/*****************************************************************************
 *
 * FILE NAME	: lgm_hw_litepath.c
 * PROJECT	: LGM
 * MODULES	: PPA Hardware litepath acceleration.
 *
 * DATE		: 22 May 2022
 * DESCRIPTION	: Litepath acceleration support for LGM
 * COPYRIGHT	: Copyright © 2020-2024 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 * Authors:	Gaurav Sharma <gsharma@maxlinear.com>
 *
 * HISTORY
 * $Date                 $Author                 $Comment
 * 09 JUNE 2022          Gaurav sharma           Litepath adaptations
 *
 *****************************************************************************/
/*!
 * ####################################
 *	Header file section
 * ####################################
 */
/*!
 *      Common header files
 */
#include <net/inet_hashtables.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/inet6_hashtables.h>
#endif
#include <linux/net.h>
#include <net/tcp.h>
#include <net/udp.h>
/*!
 *      Linux header files
 */
#include <linux/pp_api.h>
#include <net/datapath_api.h>
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hal_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
/*!
 *	PPA module header Files
 */
#include "../../ppa_api/ppa_api_netif.h"
#include "../../ppa_api/ppa_api_session.h"
/*!
 *	Chip Specific header files
 */
#if IS_ENABLED(CONFIG_QOS_MGR)
#include <net/qos_mgr/qos_hal_api.h>
#include <net/qos_mgr/qos_mgr_hook.h>
#endif
#if IS_ENABLED(CONFIG_LGM_TOE)
#if IS_ENABLED(CONFIG_SOC_LGM)
#include <net/toe_np_lro.h>
#else
#include <net/intel_np_lro.h>
#endif
#endif /* CONFIG_LGM_TOE */
#include "lgm_hw_litepath.h"
#include "lgm_pp_hal.h"

/*!
 * ####################################
 *	Macro section
 * ####################################
 */
#define PP_UD_DW3_SESSION_INDEX		28 /*! Index for session id from UD */
#define PP_UD_DW3_SESSION_INDEX_MASK	0xffffff /*! session id mask */
#define	PPPOE_HLEN			8 /*! pppoe header length */

static bool g_lp_enabled_dbg;

#define lp_dbg(format, arg...) do {\
			pr_debug(":%d:%s: " format "\n", __LINE__, __func__, ##arg); \
	} while (0)

/*!
 * ####################################
 *	Structure section
 * ####################################
 */
#ifdef CONFIG_RFS_ACCEL
/*!
 * \brief Support for steering stats and debugfs
 */
typedef struct steer_db_node {
	char *intf;
	u16 next_cpu;
	u16 next_dest_queue;
	u32 current_cpu;
	u16 current_cpu_queue;
	uint32_t steer_counter;
	int pp_queue;
	uint32_t applied_counter;
} PP_HAL_STEER_DB_NODE;
#endif /* CONFIG_RFS_ACCEL */

/*!
 * \brief Support for litepath subif and stats
 */
struct lpdev_priv {
	struct module	*owner;
	dp_subif_t	dp_subif;
	struct rtnl_link_stats64 stats;
};

/*!
 * ####################################
 *	Variables section
 * ####################################
 */
#if IS_ENABLED(LITEPATH_HW_OFFLOAD)
static bool g_lp_enabled = 1;
#else
static bool g_lp_enabled;
#endif /* LITEPATH_HW_OFFLOAD */

static const char dummy_eth_hdr[ETH_HLEN] = {
		0xb4, 0x00, 0x91, 0x33, 0xb3, 0x56,
		0x00, 0x10, 0x92, 0x00, 0x01, 0x41,
		0x08, 0x00 };

#ifdef CONFIG_RFS_ACCEL
extern u32 rps_cpu_mask;
static char dbg_intf[40] = {0};
static uint32_t g_max_record_rps_sessions = MAX_UC_SESSION_ENTRIES;
/* Support for steering stats and debugfs  */
static PP_HAL_STEER_DB_NODE *pp_hal_steer_db;
static spinlock_t g_hal_steer;
#endif /* CONFIG_RFS_ACCEL */

static struct nf_node g_lpdev = {0};
static uint32_t max_hw_sessions = MAX_UC_SESSION_ENTRIES;
static spinlock_t	g_hal_lp_rx_lock;

/*!
 * ####################################
 *	Function declaration section
 * ####################################
 */
extern int32_t ppa_sw_litepath_local_deliver(struct sk_buff *skb);
static struct dentry *ppa_lp_debugfs_dir;
#if defined(PPA_API_PROC)
static int proc_read_ppv4_lp_seq_open(struct inode *, struct file *);
static int proc_read_ppv4_lp_dbg_seq_open(struct inode *, struct file *);
static ssize_t proc_set_ppv4_lp(struct file *, const char __user *, size_t,
				loff_t *);
static ssize_t proc_set_ppv4_lp_dbg(struct file *, const char __user *, size_t,
				    loff_t *);

#ifdef CONFIG_RFS_ACCEL
/* Support for steering feature  */
extern int ppa_get_rps_cpu(struct net_device *dev, struct sk_buff *skb,
		struct rps_dev_flow **rflowp);

static struct dentry *ppa_rfs_debugfs_dir;
/* Support for steering stats and debugfs  */
static int proc_read_rps_interface_seq_open(struct inode *, struct file *);
static ssize_t ppa_set_interface_rfs_stats(struct file *, const char __user *,
					   size_t count, loff_t *);
static int dbgfs_interface_rfs_show(struct seq_file *seq, void *v);
static void *dbgfs_interface_rfs_start(struct seq_file *seq, loff_t *pos);
static void *dbgfs_interface_rfs_next(struct seq_file *seq, void *v,
				      loff_t *pos);
static void dbgfs_interface_rfs_stop(struct seq_file *seq, void *v);
#endif /* CONFIG_RFS_ACCEL */
#endif /*defined(PPA_API_PROC)*/

/*!
 * ####################################
 *            Local Functions
 * ####################################
 */
/**
 * @brief Litepath interface stats
 * @param litepath net device
 * @param rtnl_link_stats64 structure
 * @return void
 */
static void lpdev_get_stats64(struct net_device *dev,
		struct rtnl_link_stats64 *stats)
{
	struct lpdev_priv *priv;

	priv = netdev_priv(g_lpdev.dev);
	ppa_memcpy(stats, &priv->stats, sizeof(priv->stats));
}

/**
 * @brief Litepath interface queue timeout support.
 * @param litepath net device
 * @param tx queue.
 * @return void
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static void lpdev_tx_timeout(struct net_device *dev, unsigned int txqueue)
#else
static void lpdev_tx_timeout(struct net_device *dev)
#endif
{
	unsigned int tx_q = 0;

	while (tx_q < dev->num_tx_queues) {
		if (netif_tx_queue_stopped(netdev_get_tx_queue(dev, tx_q)))
			lp_dbg("dev: %s Tx Timeout txq: %d\n",
				 dev->name, tx_q);
		tx_q++;
	}
	netif_tx_wake_all_queues(dev);
}

/**
 * @brief Litepath xmit path
 * @param socket buffer from tcp tx path
 * @param litepath netdev
 * @return PPA_SUCCESS for successful tx.
 * @return PPA_FAILURE for error tx.
 */
static int lpdev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	int ret;
	int len;
	struct lpdev_priv *priv = netdev_priv(g_lpdev.dev);
	struct pmac_tx_hdr pmac = {0};
	uint32_t dp_flags = 0;

	/* Some devices (e.g., PON) expect 60B minimum for the short
	 * <TCP ACK> transmission
	 */
	if (skb_put_padto(skb, ETH_ZLEN)) {
		priv->stats.tx_dropped++;
		return NET_XMIT_DROP;
	}

	pmac.tcp_chksum = 1;
	pmac.class_en = 1;

	switch (ntohs(skb->protocol)) {
	case ETH_P_IP:
		if (ip_hdr(skb)->protocol == IPPROTO_TCP) {
			pmac.tcp_type = TCP_OVER_IPV4;
		} else if (ip_hdr(skb)->protocol == IPPROTO_UDP) {
			pmac.tcp_type = UDP_OVER_IPV4;
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
		if (ipv6_hdr(skb)->nexthdr == NEXTHDR_TCP) {
			pmac.tcp_type = TCP_OVER_IPV6;
		} else if (ipv6_hdr(skb)->nexthdr == NEXTHDR_UDP) {
			pmac.tcp_type = UDP_OVER_IPV6;
		}
		break;
#endif
	default:
		lp_dbg("Unsupported protocol 0x%x!!!", ntohs(skb->protocol));
		consume_skb(skb);
		priv->stats.tx_dropped++;
		return NET_XMIT_DROP;
	}

	pmac.ip_offset = ((u32)skb_network_offset(skb)) >> 1;
	pmac.tcp_h_offset = (skb_network_header_len(skb)) >> 2;

	if (skb_headroom(skb) < sizeof(struct pmac_tx_hdr)) {
		consume_skb(skb);
		priv->stats.tx_dropped++;
		return NET_XMIT_DROP;
	}

	/* Copy the pmac header to the beginning og skb->data*/
	memcpy((void *)((unsigned long)skb->data - sizeof(struct pmac_tx_hdr)),
			&pmac, sizeof(struct pmac_tx_hdr));

	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = priv->dp_subif.gpid;
	/* Set pmac and pre_l2 flags*/
	((struct dma_tx_desc_1 *)&skb->DW1)->field.pmac = 1;
	((struct dma_tx_desc_1 *)&skb->DW1)->field.pre_l2 = 1;
	((struct dma_tx_desc_1 *)&skb->DW1)->field.color = 1;
	((struct dma_tx_desc_1 *)&skb->DW1)->field.header_mode = 1;
	/* set subifid*/
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id =
		priv->dp_subif.subif;

	len = skb->len;

	/*
	 * If application has set chksum to CHECKSUM_NONE,litepath bypasses IP
	 * stack and IP checksum is still to be calculated, which is offloaded
	 * to PMAC below with DP_TX_WITH_PMAC and DP_TX_CAL_CHKSUM flags set.
	 */
	dp_flags = DP_TX_NEWRET | DP_TX_BYPASS_FLOW | DP_TX_WITH_PMAC |
				DP_TX_CAL_CHKSUM;

	ret = dp_xmit(g_lpdev.dev, &priv->dp_subif, skb, skb->len, dp_flags);
	if (!ret) {
		priv->stats.tx_packets++;
		priv->stats.tx_bytes += len;
	} else if (ret != NETDEV_TX_BUSY) {
		priv->stats.tx_dropped++;
	}
	return ret;
}

/**
 * @brief update socket
 * @param socket buffer from tcp tx path
 * @param litepath netdev
 * @return PPA_SUCCESS for successful tx.
 * @return PPA_FAILURE for error tx.
 */
struct sock *lpdev_sk_lookup(struct uc_session_node *p_item, int ifindex)
{
	struct sock *sk = NULL;
	u16 src_port, dst_port;
	u32 src_ip, dst_ip;
#if IS_ENABLED(CONFIG_IPV6)
	struct in6_addr *src_ip6, *dst_ip6;
#endif

	src_port = be16_to_cpu(p_item->pkt.src_port);
	dst_port = be16_to_cpu(p_item->pkt.dst_port);
	rcu_read_lock();
	switch (p_item->pkt.protocol) {
	case ETH_P_IP:
		src_ip = p_item->pkt.src_ip.ip;
		dst_ip = p_item->pkt.dst_ip.ip;
		if (p_item->pkt.ip_proto == IPPROTO_TCP) {
			sk = inet_lookup(dev_net(p_item->tx_if), &tcp_hashinfo,
					 NULL, 0, dst_ip, dst_port,
					 src_ip, src_port, ifindex);
		} else if (p_item->pkt.ip_proto == IPPROTO_UDP) {
			sk = __udp4_lib_lookup(dev_net(p_item->tx_if), dst_ip,
					dst_port, src_ip, src_port, ifindex,
					0, &udp_table, NULL);
			if (sk && !refcount_inc_not_zero(&sk->sk_refcnt))
				sk = NULL;
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
		src_ip6 = (struct in6_addr *)p_item->pkt.src_ip.ip6;
		dst_ip6 = (struct in6_addr *)p_item->pkt.dst_ip.ip6;
		if (p_item->pkt.ip_proto == IPPROTO_TCP) {
			sk = inet6_lookup(dev_net(p_item->tx_if), &tcp_hashinfo,
					NULL, 0, dst_ip6, dst_port,
					src_ip6, src_port, ifindex);
		} else {
			sk = __udp6_lib_lookup(dev_net(p_item->tx_if), dst_ip6,
					dst_port, src_ip6, src_port, ifindex,
					0, &udp_table, NULL);
			if (sk && !refcount_inc_not_zero(&sk->sk_refcnt))
				sk = NULL;
		}
		break;
#endif
	default:
		lp_dbg("Unsupported protocol 0x%x!!!", p_item->pkt.protocol);
		break;
	}
	rcu_read_unlock();

	return sk;
}

/**
 * @brief send the packet directly to SoC
 * @param socket buffer from tcp tx path
 * @return PPA_SUCCESS for successful tx.
 * @return PPA_FAILURE for error tx.
 */
static int __lpdev_tx(struct sk_buff *skb)
{
	if (!g_lpdev.dev || !g_lp_enabled) {
		lp_dbg("__lpdev_tx Error\n");
		return PPA_FAILURE;
	}

	skb_push(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	ppa_memcpy(skb->data, dummy_eth_hdr, sizeof(dummy_eth_hdr));
	eth_hdr(skb)->h_proto = skb->protocol;
	skb->dev = g_lpdev.dev;
	dev_queue_xmit(skb);

	return PPA_SUCCESS;
}

/**
 * @brief Wrapper for function to send the packet directly to SoC
 * @param socket buffer from tcp tx path
 * @return PPA_SUCCESS for successful tx.
 * @return PPA_FAILURE for error tx.
 */
int lpdev_tx(struct sk_buff *skb)
{
#if IS_ENABLED(LITEPATH_HW_OFFLOAD)
	return __lpdev_tx(skb);
#endif /* LITEPATH_HW_OFFLOAD */
	return PPA_FAILURE;
}
EXPORT_SYMBOL(lpdev_tx);

/**
 * @brief check if interface is litepath interface.
 * @param netdev structure.
 * @return PPA_SUCCESS for successful tx.
 * @return PPA_FAILURE for error tx.
 */
int32_t is_lpdev(struct net_device *dev)
{
#if IS_ENABLED(LITEPATH_HW_OFFLOAD)
	if (g_lp_enabled)
		return ppa_is_netif_equal(dev, g_lpdev.dev);
#endif /* LITEPATH_HW_OFFLOAD */
	return PPA_FAILURE;
}
EXPORT_SYMBOL(is_lpdev);

/**
 * @brief kernel structure support for litepath device operations
 */
static struct net_device_ops lpdev_ops = {
	.ndo_start_xmit = lpdev_xmit,
	.ndo_get_stats64 = lpdev_get_stats64,
	.ndo_tx_timeout = lpdev_tx_timeout,
#ifdef CONFIG_RFS_ACCEL
	.ndo_rx_flow_steer = ppa_flow_steer_update,
#endif
};

/**
 * @brief configure litepath interface properties.
 * @param netdev structure.
 * @return Void
 */
static void lpdev_setup(struct net_device *dev)
{
	dev->tx_queue_len = 1000;
	dev->netdev_ops = &lpdev_ops;
	dev->mtu = 1500;
	eth_hw_addr_random(dev);
	strcpy(dev->name, "lpdev0");
}

static inline int pkt_hndl(struct sk_buff *skb,
			      struct packet_type *ptype,
			      struct net_device *orig_dev)
{
	if (unlikely(skb_orphan_frags_rx(skb, GFP_ATOMIC)))
		return -ENOMEM;
	refcount_inc(&skb->users);

	return ptype->func(skb, skb->dev, ptype, orig_dev);
}

/**
 * @brief Litepath rx dbg support.
 * @param socket buffer from DPM.
 * @param session id from skb.
 * @param msg debug message.
 * @return void
 */
static void lpdev_rx_dbg(struct sk_buff *skb, int32_t session_id,
		char *msg)
{
	struct lp_info *lp_rxinfo = NULL;
	struct packet_type *ptype;

	if (!skb_mac_header_was_set(skb))
		skb_reset_mac_header(skb);
	skb_reset_network_header(skb);
	if (!skb_transport_header_was_set(skb))
		skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	list_for_each_entry_rcu(ptype, &g_lpdev.dev->ptype_all, list)
		pkt_hndl(skb, ptype, g_lpdev.dev);

	if (!msg)
		return;

	if (!printk_ratelimit())
		return;

	lp_dbg("%s", msg);
	lp_dbg("For session_id:(%d)", session_id);
	lp_dbg("================");
	lp_rxinfo = pp_hal_db_get_lp_rxinfo(session_id);
	if (lp_rxinfo) {
		lp_dbg("lp_rxinfo:");
		lp_dbg("l3_off %u l4_off %u netif %s lro_sessid %d",
				lp_rxinfo->l3_offset, lp_rxinfo->l4_offset,
				lp_rxinfo->netif->name, lp_rxinfo->lro_sessid);
		lp_rxinfo_put(lp_rxinfo);
	}
}

/**
 * @brief Litepath rx routine.
 * @param socket buffer from DPM.
 * @param session id from skb.
 * @param msg debug message.
 * @param msg debug len.
 * @return PPA_FAILURE
 * @return PPA_SUCCESS
 */
static int32_t lpdev_rx(struct sk_buff *skb, int32_t session_id,
		char *msg, size_t len)
{
	struct dst_entry *dst = NULL;
	uint8_t l3_off = 0, l4_off = 0;
	struct net_device *netif = NULL;
	struct lp_info *lp_rxinfo = NULL;
	bool is_soft_lro = false;
#ifdef CONFIG_RFS_ACCEL
	struct rps_dev_flow *rflow = NULL;
#endif /* CONFIG_RFS_ACCEL */

	lp_rxinfo = pp_hal_db_get_lp_rxinfo(session_id);
	if (!lp_rxinfo) {
		snprintf(msg, len, "Err:No lp_rxinfo");
		return PPA_FAILURE;
	}

	if ((session_id >= 0) && (session_id < max_hw_sessions)) {
		dst = lp_rxinfo->dst;
		netif = lp_rxinfo->netif;
		l3_off = lp_rxinfo->l3_offset;
		l4_off = lp_rxinfo->l4_offset;
		is_soft_lro = lp_rxinfo->is_soft_lro;
	}
	if (netif) {
		skb->dev = netif;
		skb->skb_iif = netif->ifindex;
	} else {
		snprintf(msg, len, "Err:no netif");
		lp_rxinfo_put(lp_rxinfo);
		return PPA_FAILURE;
	}

	skb->protocol = eth_type_trans(skb, netif);
	/* note: rxcsum_enabled status check on original net_device,
	 * but NETIF_F_RXCSUM feature check on lpdev0 net_device.
	 * For soft lro flow, rxcsum not necessary.
	 */
	if (is_soft_lro || (lp_rxinfo->rxcsum_enabled && (netif->features & NETIF_F_RXCSUM)))
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	if (unlikely(g_lp_enabled_dbg))
		lpdev_rx_dbg(skb, session_id, "rx");

	if (unlikely((l3_off == 0) || (l4_off == 0))) {
		snprintf(msg, len, "Err:L3/L4 offset null");
		lp_rxinfo_put(lp_rxinfo);
		return PPA_FAILURE;
	}
	while (eth_type_vlan(skb->protocol)) {
		struct vlan_hdr vhdr, *vh;

		vh = skb_header_pointer(skb, 0, sizeof(vhdr), &vhdr);
		if (unlikely(!vh))
			break;
		skb->protocol = vh->h_vlan_encapsulated_proto;
		skb_pull_inline(skb, sizeof(vhdr));
	}
	if (lp_rxinfo->is_pppoe) {
		skb_pull(skb, PPPOE_HLEN);
		skb->protocol = htons(lp_rxinfo->proto);
	}
	lp_rxinfo_put(lp_rxinfo);

#ifdef CONFIG_RFS_ACCEL
	if (!is_soft_lro)
		/* No steering for soft lro flows */
		ppa_get_rps_cpu(skb->dev, skb, &rflow);
#endif /* CONFIG_RFS_ACCEL */

	skb_reset_network_header(skb);
	skb_set_transport_header(skb, (l4_off - l3_off));

	skb->pkt_type = PACKET_HOST;

	/* Set skb length and remove any debris in the socket control block */
	if (ntohs(skb->protocol) == ETH_P_IP) {
		u32 ip_len;
		int err = 0;

		/* check for packet length error */
		ip_len = ntohs(ip_hdr(skb)->tot_len);
		/* For (skb->len > ip_len) kernel does pskb_trim(...) */
		if (skb->len < ip_len) { /* check for truncated packet */
			WARN_ONCE(1, "%s IP len err (skb->len:%d < ip_len:%d)!",
						__func__, skb->len, ip_len);
			err = PPA_FAILURE;
		}

		if (ip_len < (ip_hdr(skb)->ihl * 4)) { /* check for INHDRERRORS */
			WARN_ONCE(1, "%s IP hlen err (ip_len:%d < ip_hlen:%d)!",
						__func__, ip_len, (ip_hdr(skb)->ihl * 4));
			err = PPA_FAILURE;
		}

		if (err) {
			snprintf(msg, len, "Err:ip_len %u", ip_len);
			return PPA_FAILURE;
		}

		if (pskb_trim(skb, ip_len)) {
			snprintf(msg, len, "skb_trim err: ip_len %u", ip_len);
			return PPA_FAILURE;
		}

		memset(IPCB(skb), 0, sizeof(struct inet_skb_parm));
		IPCB(skb)->iif = skb->skb_iif;
	}
#if IS_ENABLED(CONFIG_IPV6)
	else if (ntohs(skb->protocol) == ETH_P_IPV6) {
		skb->len = ntohs(ipv6_hdr(skb)->payload_len)
			+ skb_network_header_len(skb);
		memset(IP6CB(skb), 0, sizeof(struct inet6_skb_parm));
		IP6CB(skb)->iif = skb->skb_iif;
		IP6CB(skb)->nhoff = offsetof(struct ipv6hdr, nexthdr);
	}
#endif
	else {
		snprintf(msg, len, "Unsupported protocol");
		return PPA_FAILURE;
	}

	/* set skb dst */
	if (dst && !(dst->flags & DST_METADATA)) {
		dst_hold_safe(dst);
		skb_dst_set(skb, dst);
	} else {
		netif_rx(skb);
		return PPA_SUCCESS;
	}
	/* set the skb->data point to the transport header */
	skb_pull(skb, skb_network_header_len(skb));
	/* disable preemption + softirqs */
	/* toe workqueue runs in current process context and might
	 * sleep/yield holding socket lock
	 */
	/* sock lock contention seen wrt to tcp stack timer softirqs
	 * e.g tcp_compressed_ack_kick
	 */
	spin_lock_bh(&g_hal_lp_rx_lock);
	ppa_sw_litepath_local_deliver(skb);
	spin_unlock_bh(&g_hal_lp_rx_lock);

	return PPA_SUCCESS;
}

/**
 * @brief callback invoked by dp when packets are received on g_litepath_gpid
 * @param Rx intercace from DPM.
 * @param Tx intercace from DPM.
 * @param socket buffer from DPM.
 * @param len.
 * @return PPA_SUCCESS for successful rx.
 * @return PPA_FAILURE for error rx.
 */
int32_t lpdev_rx_handler(struct net_device *rxif, struct net_device *txif,
	struct sk_buff *skb, int32_t len)
{
	int32_t sess_id = -1;
	char msg[SZ_64] = {0};
	struct lpdev_priv *priv = netdev_priv(g_lpdev.dev);

	/* Read session id from SKB */
	sess_id = *((int32_t *)(skb->buf_base + PP_UD_DW3_SESSION_INDEX)) &
		PP_UD_DW3_SESSION_INDEX_MASK;

	if (lpdev_rx(skb, sess_id, msg, sizeof(msg)) != PPA_SUCCESS) {
		if (unlikely(g_lp_enabled_dbg))
			lpdev_rx_dbg(skb, sess_id, msg);
		consume_skb(skb);
		priv->stats.rx_dropped++;
	} else {
		priv->stats.rx_packets++;
		priv->stats.rx_bytes += len;
	}

	return PPA_SUCCESS;
}

/**
 * @brief kernel structure support for callback invoked by dp when packets are
 *        received on g_litepath_gpid.
 */
static dp_cb_t lp_dp_cb = {
	.rx_fn = lpdev_rx_handler,
};

/**
 * @brief kernel structure support reading litepath enable or disable state.
 */
static const struct file_operations dbgfs_file_ppv4_lp_seq_fops = {
	.owner          = THIS_MODULE,
	.open           = proc_read_ppv4_lp_seq_open,
	.read           = seq_read,
	.write          = proc_set_ppv4_lp,
	.llseek         = seq_lseek,
	.release        = single_release,
};

/**
 * @brief kernel structure support setting litepath enable or disable state.
 */
static const struct file_operations dbgfs_file_ppv4_lp_dbg_seq_fops = {
	.owner          = THIS_MODULE,
	.open           = proc_read_ppv4_lp_dbg_seq_open,
	.read           = seq_read,
	.write          = proc_set_ppv4_lp_dbg,
	.llseek         = seq_lseek,
	.release        = single_release,
};

#ifdef CONFIG_RFS_ACCEL
/**
 * @brief kernel structure support for steering stats and debugfs.
 */
static const struct seq_operations dbgfs_interface_rfs_seq_ops = {
	.start = dbgfs_interface_rfs_start,
	.next = dbgfs_interface_rfs_next,
	.stop = dbgfs_interface_rfs_stop,
	.show = dbgfs_interface_rfs_show,
};

/**
 * @brief kernel structur support for steering stats and debugfs.
 */
static const struct file_operations dbgfs_interface_rfs_seq_fops = {
	.owner          = THIS_MODULE,
	.open           = proc_read_rps_interface_seq_open,
	.read           = seq_read,
	.write          = ppa_set_interface_rfs_stats,
	.llseek         = seq_lseek,
	.release        = seq_release,
};
#endif /* CONFIG_RFS_ACCEL */

/**
 * @brief kernel structure support for litepath debug fs.
 */
static struct ppa_debugfs_files lp_debugfs_files[] = {
	{ "accel", 0600, &dbgfs_file_ppv4_lp_seq_fops },
	{ "dbg", 0600, &dbgfs_file_ppv4_lp_dbg_seq_fops },
};

#ifdef CONFIG_RFS_ACCEL
/**
 * @brief kernel structure support for RFS debug fs.
 */
static struct ppa_debugfs_files rfs_debugfs_files[] = {
	{ "interface_rfs_stats", 0600, &dbgfs_interface_rfs_seq_fops }
};

/**
 * @brief Initializes RFS steer db.
 * @param Void
 * @return Void
 */
void ppa_rfs_alloc_steer_db(void)
{
	pp_hal_steer_db = (PP_HAL_STEER_DB_NODE *) ppa_malloc(
		sizeof(PP_HAL_STEER_DB_NODE) * g_max_record_rps_sessions);
	if (!pp_hal_steer_db) {
		pr_err("Failed to allocate hal db\n");
		return;
	}
	ppa_memset(pp_hal_steer_db, 0, sizeof(PP_HAL_STEER_DB_NODE) *
		g_max_record_rps_sessions);
}

/**
 * @brief Uninitializes RFS steer db.
 * @param Void
 * @return Void
 */
void ppa_rfs_free_steer_db(void)
{
	if (pp_hal_steer_db) {
		ppa_free(pp_hal_steer_db);
		pp_hal_steer_db = NULL;
	}
}

/**
 * @brief create RFS proc.
 * @param Void
 * @return Void
 */
void ppa_rfs_proc_file_create(void)
{
	ppa_debugfs_create(ppa_debugfs_dir_get(), "rfs",
			   &ppa_rfs_debugfs_dir, rfs_debugfs_files,
			   ARRAY_SIZE(rfs_debugfs_files));
}

/**
 * @brief Remove RFS proc .
 * @param Void
 * @return Void
 */
void ppa_rfs_proc_file_remove(void)
{
	ppa_debugfs_remove(ppa_rfs_debugfs_dir,
			   rfs_debugfs_files,
			   ARRAY_SIZE(rfs_debugfs_files));
}
#endif /* CONFIG_RFS_ACCEL */

/**
 * @brief create Litepath proc.
 * @param Void
 * @return Void
 */
static void ppa_lp_proc_file_create(void)
{
	ppa_debugfs_create(ppa_debugfs_dir_get(), "hw_litepath",
			   &ppa_lp_debugfs_dir, lp_debugfs_files,
			   ARRAY_SIZE(lp_debugfs_files));
}

/**
 * @brief Remove Litepath proc.
 * @param Void
 * @return Void
 */
static void ppa_lp_proc_file_remove(void)
{
	ppa_debugfs_remove(ppa_lp_debugfs_dir,
			   lp_debugfs_files,
			   ARRAY_SIZE(lp_debugfs_files));
}

/**
 * @brief Uninitializes Litepath module.
 * @param Void
 * @return Void
 */
void uninit_app_lp(void)
{
	struct dp_spl_cfg dp_con = {0};

	ppa_lp_proc_file_remove(); /*! Remove Litepath proc */

	dp_con.flag = DP_F_DEREGISTER;
	dp_con.type = DP_SPL_APP_LITEPATH;
	dp_con.spl_id = g_lpdev.uc_id;

	if (dp_spl_conn(0, &dp_con))
		pr_err("Deregister of dp spl conn for litepath failed\n");
	unregister_netdev(g_lpdev.dev);
	free_netdev(g_lpdev.dev);
	g_lpdev.dev = NULL;
}

/**
 * @brief Initializes Litepath module.
 * @param Void
 * @return PPA_SUCCESS/PPA_FAILURE
 */
int32_t init_app_lp(void)
{
	int32_t ret = PPA_SUCCESS;
	struct dp_spl_cfg dp_con = {0};
#if IS_ENABLED(CONFIG_LGM_TOE)
	struct dp_spl_cfg tso_lp_dp_con = {0};
	struct lro_ops *ops = NULL;
	struct dp_queue_map_set q_map = {0};
#endif /*IS_ENABLED(CONFIG_LGM_TOE)*/
	struct dp_qos_q_logic q_logic = {0};
	struct lpdev_priv *priv = NULL;

	ppa_memset(&g_lpdev, 0, sizeof(g_lpdev));
	/*Allocate netdevice */
	g_lpdev.dev = alloc_etherdev_mq(sizeof(struct lpdev_priv), 8);
	lpdev_setup(g_lpdev.dev);
	if (!g_lpdev.dev) {
		pr_err("alloc_netdev failed for lpdev0\n");
		return PPA_FAILURE;
	}

	priv = netdev_priv(g_lpdev.dev);
	if (!priv) {
		pr_err("priv is NULL\n");
		return PPA_FAILURE;
	}
	priv->owner = THIS_MODULE;

#if IS_ENABLED(CONFIG_LGM_TOE)
	ops = dp_get_lro_ops();
	if (ops)
		ops->cfg_netdev_feature(ops->toe, g_lpdev.dev, 1);
	else
		pr_err("[%s] failed to enabled toe features\n", __func__);
#endif /*IS_ENABLED(CONFIG_LGM_TOE)*/
#ifdef CONFIG_RFS_ACCEL
	g_lpdev.dev->features |= NETIF_F_NTUPLE;
	g_lpdev.dev->hw_features |= NETIF_F_NTUPLE;
#endif

	/*Register netdevice*/
	if (register_netdev(g_lpdev.dev)) {
		free_netdev(g_lpdev.dev);
		g_lpdev.dev = NULL;
		pr_err("register device \"lpdev0\" failed\n");
		return PPA_FAILURE;
	}

	/*call the dp to allocate special connection */
	/*******************************************************/
	dp_con.flag = 0;/*DP_F_REGISTER;*/
	dp_con.type = DP_SPL_APP_LITEPATH;
	dp_con.f_subif = 1;
	dp_con.f_gpid = 1;
	dp_con.f_hostif = 1;

	/*assign the netdevice */
	dp_con.dev = g_lpdev.dev;

	/*callback to be invoked by dp when packet is received for this GPID*/
	dp_con.dp_cb = &lp_dp_cb;

	if ((ret != dp_spl_conn(0, &dp_con))) {
		pr_err("Register spl conn for litepath failed\n");
		return PPA_FAILURE;
	}

	/*Store the gpid and uc_id*/
	g_lpdev.uc_id = dp_con.spl_id;
	g_lpdev.gpid = dp_con.gpid;
	g_lpdev.subif = dp_con.subif;

	priv->dp_subif.port_id = dp_con.dp_port;
	priv->dp_subif.subif = dp_con.subif;
	priv->dp_subif.gpid = dp_con.gpid;

	/*! Enable dp_rx */
	if ((ret != dp_rx_enable(g_lpdev.dev, g_lpdev.dev->name, 1))) {
		pr_err("Enable rx_fn for litepath failed\n");
		return PPA_FAILURE;
	}

	/*! Prepare an interface for use. Takes a device from down to up state */
	rtnl_lock();
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
	if (dev_open(g_lpdev.dev)) {
#else
	if (dev_open(g_lpdev.dev, NULL)) {
#endif
		pr_err("[%s]: [%s] dev_open error!\n", __func__,
			g_lpdev.dev->name);
		rtnl_unlock();
		uninit_app_lp();
		return PPA_FAILURE;
	}
	rtnl_unlock();
	/*******************************************************/
	lp_dbg("dp_spl_conn success dp_con.gpid=%d, dp_con.subif=%d \
		dp_con.spl_id=%d, dp_con.egp[0].qid=%d, dp_con.egp[1].qid=%d\n",
		dp_con.gpid, dp_con.subif, \
		dp_con.spl_id, dp_con.egp[0].qid, dp_con.egp[1].qid);

#if IS_ENABLED(CONFIG_LGM_TOE)
	/*! Setup Queue map table for lpdev_tx */
	q_map.map.dp_port = 0; 			/* CPU portid */
	q_map.map.subif = dp_con.subif;		/* Subif id */

	/*!< need to setup the queue map table */
	dp_spl_conn_get(0, DP_SPL_TOE, &tso_lp_dp_con, 1);

	/*!< Verify that the toe driver succeeds to load */
	if (tso_lp_dp_con.igp->egp) {
		q_map.q_id = tso_lp_dp_con.igp->egp->qid;

		q_map.mask.class  = 1;			/* TC dont care*/
		q_map.mask.flowid = 1;			/* Flowid dont care*/
		q_map.mask.egflag = 1;			/* Flowid dont care*/

		if (dp_queue_map_set(&q_map, 0) == DP_FAILURE)
			lp_dbg("dp_queue_map_set failed for Queue [%d]\n",
			    dp_con.egp[1].qid);

		if (ops)
			netif_set_gso_max_size(g_lpdev.dev,
					       ops->get_gso_max_size(ops->toe));
	} else
		pr_err("TOE driver is not ready\n");
#endif /* IS_ENABLED(CONFIG_LGM_TOE) */

	/*! Egress port qid in this case CPU queueid*/;
	/*! FIXME use correct API for QoS */
	q_logic.q_id = 2;

	/*! physical to logical qid */
	if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
		pr_err("%s:%d ERROR Failed to Logical Queue Id\n", __func__,
			__LINE__);
		return PPA_FAILURE;
	}

	/*! Store the logical qid */
	g_lpdev.qid = q_logic.q_logic_id;

	ret = pp_max_sessions_get(&max_hw_sessions);
	if (unlikely(ret)) {
		pr_err("%s:%d ERROR Failed get MAX sessions\n", __func__,__LINE__);
		return PPA_FAILURE;
	}
	g_max_record_rps_sessions = max_hw_sessions;

	spin_lock_init(&g_hal_lp_rx_lock);
	spin_lock_init(&g_hal_steer);

	ppa_lp_proc_file_create(); /*! create Litepath proc fs */

	return ret;
}

/**
 * @brief litepath read proc support.
 */
static int proc_read_ppv4_lp(struct seq_file *seq, void *v)
{
	if (!capable(CAP_SYSLOG)) {
		lp_dbg ("Read Permission denied");
		return 0;
	}
	seq_printf(seq,	"Litepath HW offload	: %s\n", g_lp_enabled ? "enabled" : "disabled");
	return 0;
}

/**
 * @brief litepath read dbg proc support.
 */
static int proc_read_ppv4_lp_dbg(struct seq_file *seq, void *v)
{
	if (!capable(CAP_SYSLOG)) {
		lp_dbg ("Read Permission denied");
		return 0;
	}
	seq_printf(seq,	"Litepath debug support	: %s\n", g_lp_enabled_dbg ? "enabled" : "disabled");
	return 0;
}

/**
 * @brief litepath read proc support.
 */
static int proc_read_ppv4_lp_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_ppv4_lp, NULL);
}

/**
 * @brief litepath enable proc support.
 */
static int proc_read_ppv4_lp_dbg_seq_open(struct inode *inode,
					  struct file *file)
{
	return single_open(file, proc_read_ppv4_lp_dbg, NULL);
}

/**
 * @brief litepath enable proc support.
 */
static ssize_t proc_set_ppv4_lp(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	int len;
	char str[40];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		lp_dbg("Write Permission denied");
		return 0;
	}

	len = min(count, (size_t)(sizeof(str) - 1));
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	if (strncmp(p, "enable", 6) == 0) {
		g_lp_enabled = 1;
		pr_err("Litepath HW offload enabled!!!\n");
	} else if (strncmp(p, "disable", 7) == 0) {
		g_lp_enabled = 0;
		pr_err("Litepath HW offload disabled!!!\n");
	} else {
		pr_err("usage : echo <enable/disable> > <debugfs>/ppa/hw_litepath/accel\n");
	}

	return len;
}

/**
 * @brief litepath enable proc debug support.
 */
static ssize_t proc_set_ppv4_lp_dbg(struct file *file, const char __user *buf,
				    size_t count, loff_t *data)
{
	int len;
	char str[40];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		lp_dbg("Write Permission denied");
		return 0;
	}

	len = min(count, (size_t)(sizeof(str) - 1));
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	if (strncmp(p, "enable", 6) == 0) {
		g_lp_enabled_dbg = 1;
		pr_err("Litepath debug support enabled!!!\n");
	} else if (strncmp(p, "disable", 7) == 0) {
		g_lp_enabled_dbg = 0;
		pr_err("Litepath debug support disabled!!!\n");
	} else {
		pr_err("usage : echo <enable/disable> > <debugfs>/ppa/hw_litepath/dbg\n");
	}

	return len;
}

#ifdef CONFIG_RFS_ACCEL
/**
 * @brief PPA RFS steer debug support.
 */
static int proc_read_dbgfs_interface_rfs_show(struct seq_file *seq, void *v)
{
	int index = *(loff_t *)v;

	if (!capable(CAP_SYSLOG)) {
		lp_dbg("Read Permission denied");
		return PPA_FAILURE;
	}
	if (index >= g_max_record_rps_sessions)
		return SEQ_SKIP;

	if (index == 0) {
		if (dbg_intf[0] != 0 && strcmp(dbg_intf, "c")) {
			seq_printf(seq,
					"+---------+--------------+-------------------+----------+----------------+-----------------+------------------+\n");
			seq_printf(seq,
			"| RFS statistics for interface: %-77s |\n", dbg_intf);
		}
		seq_printf(seq,
				"+---------+--------------+-------------------+----------+----------------+-----------------+------------------+\n");
		seq_printf(seq,
				"| sess_id | Current CPU  | Current CPU queue | Next CPU | Next CPU queue | Steer ReqCount  | Steer ApplCount  |\n");
		seq_printf(seq,
				"+---------+--------------+-------------------+----------+----------------+-----------------+------------------+\n");

	}
	if (pp_hal_steer_db[index].intf &&
		!strncmp(pp_hal_steer_db[index].intf, dbg_intf,
		strlen(pp_hal_steer_db[index].intf))) {
		seq_printf(seq, "| %-7u | %-12i | %-17hu | %-8hu | %-14hu | %-15u | %-16u |\n",
			index, pp_hal_steer_db[index].current_cpu,
			pp_hal_steer_db[index].current_cpu_queue,
			pp_hal_steer_db[index].next_cpu,
			pp_hal_steer_db[index].pp_queue-1,
			pp_hal_steer_db[index].steer_counter,
			pp_hal_steer_db[index].applied_counter);
		seq_printf(seq,
				"+---------+--------------+-------------------+----------+----------------+-----------------+------------------+\n");
	}
	return PPA_SUCCESS;
}

/**
 * @brief PPA RFS steer debug support.
 */
static int dbgfs_interface_rfs_show(struct seq_file *seq, void *v)
{
	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Read Permission denied\n");
		return PPA_FAILURE;
	}
	return proc_read_dbgfs_interface_rfs_show(seq, v);
}

/**
 * @brief PPA RFS steer debug support.
 */
static void *dbgfs_interface_rfs_start(struct seq_file *seq, loff_t *pos)
{
	if (*pos >= g_max_record_rps_sessions)
		return NULL;
	return pos;
}

/**
 * @brief PPA RFS steer debug support.
 */
static void *dbgfs_interface_rfs_next(struct seq_file *seq, void *v,
				      loff_t *pos)
{
	return (++*pos >= g_max_record_rps_sessions) ? NULL : pos;
}

/**
 * @brief PPA RFS steer debug support.
 */
static void dbgfs_interface_rfs_stop(struct seq_file *seq, void *v)
{
	lp_dbg("\n");
}

/**
 * @brief PPA RFS steer debug support.
 */
static int proc_read_rps_interface_seq_open(struct inode *inode,
					    struct file *file)
{
	return seq_open(file, &dbgfs_interface_rfs_seq_ops);
}

/**
 * @brief Enable rfs stats on selected interface.
 * @return
 */
static ssize_t ppa_set_interface_rfs_stats(struct file *file,
					   const char __user *buf,
					   size_t count, loff_t *data)
{
	int len;
	char str[40];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		lp_dbg("Write Permission denied");
		return 0;
	}

	len = min(count, (size_t)(sizeof(str) - 1));
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	memset(dbg_intf, 0, sizeof(dbg_intf));
	if (!strcmp(p, "c")) {
		lp_dbg("Clear all db entries\n");
		ppa_memset(pp_hal_steer_db, 0, sizeof(PP_HAL_STEER_DB_NODE) *
			   g_max_record_rps_sessions);
	} else {
		ppa_memcpy(&dbg_intf, p, strlen(p));
		lp_dbg("dbg_intf %s p %s\n", dbg_intf, p);
	}
	return len;
}

/**
 * @brief PPA RFS steer support.
 * @param netdev
 * @param skb packet skb
 * @param CPU rx queue
 * @param flow id
 * @return PPA_SUCCESS/PPA_FAILURE
 */
int32_t ppa_flow_steer_update(struct net_device *dev, const struct sk_buff *skb,
			      uint16_t dest_q, u32 flow_id)
{
	int sess_id, ret;
	struct dp_qos_q_logic q_logic = {0};
	struct netdev_rx_queue *rxqueue;
	struct rps_dev_flow *rflow;
	u32 ident;
	const struct rps_sock_flow_table *sock_flow_table;
	struct rps_dev_flow_table *flow_table;
	u32 hash;

	spin_lock_bh(&g_hal_steer);
	sess_id = *((int32_t *)(skb->buf_base + PP_UD_DW3_SESSION_INDEX)) &
				PP_UD_DW3_SESSION_INDEX_MASK;
	q_logic.q_id = dest_q + 1; //FIXME API from QoSMGR

	/*! update queue only if exception bit is not set in UD,
	 *  valid session created and queue not same as already configured
	 */
	if (((skb->buf_base[PP_UD_DW3_SESSION_INDEX+3] & 1) == 0) &&
			is_pp_sess_valid(sess_id) &&
			sess_id < g_max_record_rps_sessions &&
			pp_hal_steer_db[sess_id].pp_queue != q_logic.q_id) {
		if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
			pr_err("%s:%d ERROR Failed to Logical Queue Id\n",
				__func__, __LINE__);
			spin_unlock_bh(&g_hal_steer);
			return PPA_FAILURE;
		}
		lp_dbg("sess_id : %d q_logic.q_id : %d q_logic.q_logic_id: %d\n",
			sess_id, q_logic.q_id, q_logic.q_logic_id);
		ret = pp_session_dst_queue_update(sess_id, q_logic.q_logic_id);
		if (unlikely(ret)) {
			pr_err("Failed to set session %u dest queue to %u\n",
				sess_id, dest_q);
			spin_unlock_bh(&g_hal_steer);
			return PPA_FAILURE;
		}

		pp_hal_steer_db[sess_id].applied_counter++;
		pp_hal_steer_db[sess_id].pp_queue = q_logic.q_id;
	}

	/* support for steering debugs */
	if (g_lp_enabled_dbg) {
		rxqueue = skb->dev->_rx + skb_get_rx_queue(skb);
		pp_hal_steer_db[sess_id].intf =  skb->dev->name;
		pp_hal_steer_db[sess_id].next_dest_queue =  dest_q;

		sock_flow_table = rcu_dereference(rps_sock_flow_table);
		flow_table = rcu_dereference(rxqueue->rps_flow_table);
		rflow = &flow_table->flows[flow_id];
		pp_hal_steer_db[sess_id].current_cpu = rflow->cpu;
		pp_hal_steer_db[sess_id].current_cpu_queue = skb_get_rx_queue(skb);

		hash = skb_get_hash((struct sk_buff *) skb);
		ident = sock_flow_table->ents[hash & sock_flow_table->mask];
		pp_hal_steer_db[sess_id].next_cpu = ident & rps_cpu_mask;
		pp_hal_steer_db[sess_id].steer_counter++;
	}
	spin_unlock_bh(&g_hal_steer);

	return PPA_SUCCESS;
}
EXPORT_SYMBOL(ppa_flow_steer_update);
#endif /* CONFIG_RFS_ACCEL */

/**
 * @brief get litepath netif interface
 * @param void
 * @return litepath netif.
 */
struct net_device *ppa_get_lp_dev(void)
{
	return g_lpdev.dev;
}

/**
 * @brief get litepath interface DP gpid.
 * @param void
 * @return litepath interface gpid.
 */
uint16_t ppa_get_lp_gpid(void)
{
	return g_lpdev.gpid;
}

/**
 * @brief get litepath interface QoS ID.
 * @param void
 * @return QoS ID.
 */
uint16_t ppa_get_lp_qid(void)
{
	return g_lpdev.qid;
}

/**
 * @brief get litepath interface id.
 * @param void
 * @return subif
 */
uint16_t ppa_get_lp_subif(void)
{
	return g_lpdev.subif;
}

/**
 * @brief check for litepath status.
 * @param void
 * @return bool for litepath status.
 */
bool is_hw_litepath_enabled(void)
{
	return g_lp_enabled;
}

/**
 * @brief get the dummy L2 header.
 * @param void
 * @return base address for l2 header.
 */
const char *get_lp_dummy_l2_header(void)
{
	return &dummy_eth_hdr[0];
}
