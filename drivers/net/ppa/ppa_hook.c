// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 **
 ** FILE NAME 	: ppa_hook.c
 ** PROJECT 	: PPA
 ** MODULES 	: PPA Protocol Stack Hooks
 **
 ** DATE 	: 3 NOV 2008
 ** AUTHOR 	: Xu Liang
 ** DESCRIPTION : PPA Protocol Stack Hook Pointers
 ** COPYRIGHT	: Copyright (c) 2020-2024 MaxLinear, Inc
 **		  Copyright (c) 2017 - 2018 Intel Corporation
 **		  Copyright (c) 2010 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 **
 ** HISTORY
 ** $Date $Author $Comment
 ** 03 NOV 2008 Xu Liang Initiate Version
 ** 20 SEP 2017 Eradath Kamal PPA leraning based on nf_hook is added
 *******************************************************************************/
/*
 * ####################################
 * Head File
 * ####################################
 */

/*
 * Common Head File
 */
#include <linux/version.h>
#include <net/protocol.h>
#include <net/xfrm.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_bridge.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <soc/mxl/mxl_skb_ext.h>

/*
 * Chip Specific Head File
 */
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_stack_al.h>
#ifdef CONFIG_RFS_ACCEL
#include <linux/cpu_rmap.h>
#endif /* CONFIG_RFS_ACCEL */
/*
 * ####################################
 * Export PPA hook functions
 * ####################################
 */

/**********************************************************************************************
 * PPA unicast routing hook function:ppa_hook_session_add_fn
 * It it used to delete a unicast routing session when it is timeout, reset or purposely.
 * input parameter PPA_SESSION *: the session pointer
 * input parameter uint32_t: for future purpose
 * return: PPA_SUCCESS: this session is deleted sucessfully from PPE FW
 * ...
 **********************************************************************************************/
int32_t (*ppa_hook_session_del_fn)(PPA_SESSION *, uint32_t) = NULL;

/**********************************************************************************************
 * PPA session priority hook function:ppa_hook_session_prio_fn
 * It is used to get session priority of a ppa session.
 * input parameter PPA_SESSION *: the session pointer
 * input parameter uint32_t: for future purpose
 * return: session priority
 * ...
 **********************************************************************************************/
int32_t (*ppa_hook_session_prio_fn)(PPA_SESSION *, uint32_t) = NULL;

/**********************************************************************************************
 * PPA unicast routing hook function:ppa_hook_get_ct_stats_fn
 * It it used to read the statistics counters of specified session
 * input parameter PPA_SESSION *: the session pointer
 * input parameter PPA_CT_COUNTER *: the session stats counter structure pointer
 * return: PPA_SUCCESS: the session statistics read sucessfully
 **********************************************************************************************/

int32_t (*ppa_hook_get_ct_stats_fn)(PPA_SESSION *, PPA_CT_COUNTER*) = NULL;
/**********************************************************************************************
 * PPA unicast hook function:ppa_hook_inactivity_status_fn
 * It it used to check whether a unicast session is timeout or not.
 * Normally it is called by network stack which want to delete a session without any traffic for sometime.
 * input parameter PPA_U_SESSION *: the unicast session pointer
 * return: PPA_HIT: the session still hit and should keep it
 * PPA_TIMEOUT: timeout already
 **********************************************************************************************/
int32_t (*ppa_hook_inactivity_status_fn)(PPA_U_SESSION *) = NULL;

/**********************************************************************************************
 * PPA unicast hook function:ppa_hook_set_inactivity_fn
 * It it used to set one unicast session timeout value
 * Normally it is called to prolong one sessions duration.
 * input parameter PPA_U_SESSION *: the unicast session pointer
 * input parameter int32_t: the timeout value
 * return: PPA_SUCCESS: update timeout sucessfully
 * PPA_FAILURE: fail to update timeout value

 **********************************************************************************************/
int32_t (*ppa_hook_set_inactivity_fn)(PPA_U_SESSION*, int32_t) = NULL;

/**********************************************************************************************
 * PPA bridge hook function:ppa_hook_bridge_entry_hit_time_fn
 * It it used to get last hit time
 * input parameter uint8_t *: the mac address
 * input parameter PPA_NETIF *: bridge interface
 * input parameter uint32_t *: the last hit time
 * return: PPA_HIT: sucessfully get the last hit time
 * PPA_SESSION_NOT_ADDED: the mac address not in PPE FW yet
 **********************************************************************************************/
int32_t (*ppa_hook_bridge_entry_hit_time_fn)(uint8_t *, PPA_NETIF *, uint32_t *) = NULL;

#if defined(PPA_IF_MIB) && PPA_IF_MIB
/**********************************************************************************************
 * PPA mibs hook function:ppa_hook_get_accel_stats_fn
 * It it used to port acclerated mib status, like accelerated rx/tx packet number and so on
 * input parameter PPA_IFNAME *: specify which port's acceleration mibs want to get
 * output parameter PPA_ACCEL_STATS *
 * input parameter uint32_t: for future purpose
 * return: PPA_SUCCESS: get sucessfully
 * PPA_FAILURE: fail to get
 **********************************************************************************************/
int32_t (*ppa_hook_get_netif_accel_stats_fn)(PPA_IFNAME *, PPA_NETIF_ACCEL_STATS *, uint32_t) = NULL;
#endif

#if IS_ENABLED(CONFIG_PPA_API_DIRECTCONNECT) && CONFIG_PPA_API_DIRECTCONNECT
/**********************************************************************************************
 * PPA interface hook function:ppa_check_if_netif_fastpath_fn
 * It it used to check if network interface like, WAVE500, ACA is a fastpath interface
 * input parameter PPA_NETIF *: pointer to stack network interface structure
 * input parameter char *: interface name
 * input parameter uint32_t: for future purpose
 * return: 1: if ACA or WLAN fastpath interface
 * 0: otherwise
 **********************************************************************************************/
int32_t (*ppa_check_if_netif_fastpath_fn)(PPA_NETIF *, char *, uint32_t) = NULL;

/**********************************************************************************************
 * PPA interface hook function:ppa_hook_disconn_if_fn
 * It is used to delete one WAVE500 STA from PPA
 * input parameter PPA_NETIF *: the linux interface name, like wlan0
 * input parameter PPA_DP_SUBIF *: WAVE500 port id and subif id including station id
 * input parameter uint8_t *: WAVE500 STA mac address
 * input parameter uint32_t: for future purpose
 * return: PPA_SUCCESS: delete sucessfully
 * PPA_FAILURE: fail to delete
 **********************************************************************************************/
int32_t (*ppa_hook_disconn_if_fn)(PPA_NETIF *, PPA_DP_SUBIF *, uint8_t *, uint32_t) = NULL;

#endif /* CONFIG_PPA_API_DIRECTCONNECT */

uint32_t (*ppa_is_ipv4_gretap_fn)(struct net_device *dev) = NULL;
uint32_t (*ppa_is_ipv6_gretap_fn)(struct net_device *dev) = NULL;

/*PPP related functions */
/* ppp_generic.c will register function for getting ppp info once the ppp.ko is insmoded */
int32_t (*ppa_ppp_get_chan_info_fn)(struct net_device *ppp_dev, struct ppp_channel **chan) = NULL;
int32_t (*ppa_check_pppoe_addr_valid_fn)(struct net_device *dev, struct pppoe_addr *pa) = NULL;
int32_t (*ppa_get_pppoa_info_fn)(struct net_device *dev, void *pvcc, uint32_t pppoa_id, void *value) = NULL;
int32_t (*ppa_get_pppol2tp_info_fn)(struct net_device *dev, void *po, uint32_t pppol2tp_id, void *value) = NULL;
int32_t (*ppa_if_is_ipoa_fn)(struct net_device *netdev, char *ifname) = NULL;
int32_t (*ppa_if_is_br2684_fn)(struct net_device *netdev, char *ifname) = NULL;
int32_t (*ppa_br2684_get_vcc_fn)(struct net_device *netdev, struct atm_vcc **pvcc) = NULL;
int32_t (*ppa_if_ops_veth_xmit_fn)(struct net_device *dev) = NULL;

#if IS_ENABLED(CONFIG_PPA_QOS)
int32_t (*ppa_hook_get_qos_qnum)(uint32_t portid, uint32_t flag) = NULL;
int32_t (*ppa_hook_get_qos_mib)(uint32_t portid, uint32_t queueid, PPA_QOS_MIB *mib, uint32_t flag) = NULL;
int32_t (*ppa_hook_set_ctrl_qos_rate)(uint32_t portid, uint32_t enable, uint32_t flag) = NULL;
int32_t (*ppa_hook_get_ctrl_qos_rate)(uint32_t portid, uint32_t *enable, uint32_t flag) = NULL;
int32_t (*ppa_hook_set_qos_rate)(uint32_t portid, uint32_t queueid, uint32_t rate, uint32_t burst, uint32_t flag) = NULL;
int32_t (*ppa_hook_get_qos_rate)(uint32_t portid, uint32_t queueid, uint32_t *rate, uint32_t *burst, uint32_t flag) = NULL;
int32_t (*ppa_hook_reset_qos_rate)(uint32_t portid, uint32_t queueid, uint32_t flag) = NULL;
int32_t (*ppa_hook_set_ctrl_qos_wfq)(uint32_t portid, uint32_t enable, uint32_t flag) = NULL;
int32_t (*ppa_hook_get_ctrl_qos_wfq)(uint32_t portid, uint32_t *enable, uint32_t flag) = NULL;
int32_t (*ppa_hook_set_qos_wfq)(uint32_t portid, uint32_t queueid, uint32_t weight_level, uint32_t flag) = NULL;
int32_t (*ppa_hook_get_qos_wfq)(uint32_t portid, uint32_t queueid, uint32_t *weight_level, uint32_t flag) = NULL;
int32_t (*ppa_hook_reset_qos_wfq)(uint32_t portid, uint32_t queueid, uint32_t flag) = NULL;
#endif /*end of CONFIG_PPA_QOS*/

#ifdef CONFIG_INTEL_IPQOS_MARK_SKBPRIO
/*
 * Function to mark priority based on specific criteria
 */
static inline
int skb_mark_priority(struct sk_buff *skb)
{
	unsigned old_priority = skb->priority;
	u32 mark = 0;
	unsigned value;

	/*
	 * IPQoS in UGW: added copy of nfmark set in classifier to skb->priority
	 * to be used in hardware queues.
	 * nfmark range = 1-8 if QoS is enabled; priority range = 0-7;
	 * else preserve original priority
	 */
#ifdef HAVE_QOS_EXTMARK
	mark = ppa_get_skb_extmark(skb);
	GET_DATA_FROM_MARK_OPT(mark, QUEPRIO_MASK, QUEPRIO_START_BIT_POS, value);
#else
	mark = skb->mark;
	GET_DATA_FROM_MARK_OPT(mark, MARK_QUEPRIO_MASK, MARK_QUEPRIO_START_BIT_POS, value);
#endif /* HAVE_QOS_EXTMARK */
	if (value)
		skb->priority = value - 1;
	return old_priority;
}
#endif /* CONFIG_INTEL_IPQOS_MARK_SKBPRIO*/

#if IS_ENABLED(CONFIG_MXL_VPN)
/**************************************************************************************************
 * PPA based VPN acceleration learning hook
 * to be called by VPNA
 * input : sk_buff ingress/egress netdevice and the tunnel id
 * output: PPA_SUCCESS if the session learning call succeeded
 *		PPA_FAILURE if the session learning call failed
 **************************************************************************************************/
int32_t (*ppa_vpn_ig_lrn_hook)(struct sk_buff *skb, struct net_device *dev, uint16_t tunnel_id) = NULL;
int32_t (*ppa_vpn_eg_lrn_hook)(struct sk_buff *skb, struct net_device *dev, uint16_t tunnel_id) = NULL;
int32_t (*ppa_vpn_tunn_del_hook)(uint16_t tunnel_id) = NULL;
#endif /*IS_ENABLED(CONFIG_MXL_VPN)*/

#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
/**************************************************************************************************
 * PPA based software acceleration function hook
 * gets called from netif_rx in dev.c
 * bye pass the linux sw stack and routes the packet based on the ppa session entry
 * input : sk_buff of incoming packet
 * output: PPA_SUCCESS if the packet is accelerated
 * 	 PPA_FAILURE if the packet is not accelerated
 **************************************************************************************************/
int32_t (*ppa_hook_sw_fastpath_send_fn)(struct sk_buff *skb) = NULL;
int32_t (*ppa_hook_set_sw_fastpath_enable_fn)(uint32_t flags) = NULL;
int32_t (*ppa_hook_get_sw_fastpath_status_fn)(uint32_t flags) = NULL;
#endif

#if IS_ENABLED(CONFIG_L2NAT_MODULE) || defined(CONFIG_L2NAT)
/**********************************************************************************************
 * PPA interface hook function:ppa_check_if_netif_l2nat_fn
 * It it used to check if network interface is an l2nat interface
 * input parameter PPA_NETIF *: pointer to stack network interface structure
 * input parameter char *: interface name
 * input parameter uint32_t: for future purpose
 * return: 1: if network interafce is an l2nat interface
 * 0: otherwise
 **********************************************************************************************/
int32_t (*ppa_check_if_netif_l2nat_fn)(PPA_NETIF *, char *, uint32_t) = NULL;
#endif

EXPORT_SYMBOL(ppa_hook_session_del_fn);
EXPORT_SYMBOL(ppa_hook_get_ct_stats_fn);
EXPORT_SYMBOL(ppa_hook_session_prio_fn);
EXPORT_SYMBOL(ppa_hook_inactivity_status_fn);
EXPORT_SYMBOL(ppa_hook_set_inactivity_fn);

EXPORT_SYMBOL(ppa_hook_bridge_entry_hit_time_fn);

#if defined(PPA_IF_MIB) && PPA_IF_MIB
EXPORT_SYMBOL(ppa_hook_get_netif_accel_stats_fn);
#endif

#if IS_ENABLED(CONFIG_PPA_API_DIRECTCONNECT)
EXPORT_SYMBOL(ppa_check_if_netif_fastpath_fn);
EXPORT_SYMBOL(ppa_hook_disconn_if_fn);
#endif

EXPORT_SYMBOL(ppa_get_pppoa_info_fn);
EXPORT_SYMBOL(ppa_check_pppoe_addr_valid_fn);
EXPORT_SYMBOL(ppa_ppp_get_chan_info_fn);
EXPORT_SYMBOL(ppa_is_ipv4_gretap_fn);
EXPORT_SYMBOL(ppa_is_ipv6_gretap_fn);
EXPORT_SYMBOL(ppa_get_pppol2tp_info_fn);
EXPORT_SYMBOL(ppa_if_is_ipoa_fn);
EXPORT_SYMBOL(ppa_if_is_br2684_fn);
EXPORT_SYMBOL(ppa_br2684_get_vcc_fn);
EXPORT_SYMBOL(ppa_if_ops_veth_xmit_fn);

#if IS_ENABLED(CONFIG_MXL_VPN)
EXPORT_SYMBOL(ppa_vpn_ig_lrn_hook);
EXPORT_SYMBOL(ppa_vpn_eg_lrn_hook);
EXPORT_SYMBOL(ppa_vpn_tunn_del_hook);
#endif

#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
EXPORT_SYMBOL(ppa_hook_set_sw_fastpath_enable_fn);
EXPORT_SYMBOL(ppa_hook_get_sw_fastpath_status_fn);
EXPORT_SYMBOL(ppa_hook_sw_fastpath_send_fn);
#endif
#if IS_ENABLED(CONFIG_L2NAT)
EXPORT_SYMBOL(ppa_check_if_netif_l2nat_fn);
#endif

/* Stack adaptation layer APIs that directly hooks
with un exported kernel APIs needs to be in builtin */
int sysctl_ip_default_ttl __read_mostly = IPDEFTTL;
EXPORT_SYMBOL(sysctl_ip_default_ttl);

int32_t ppa_is_pkt_local(PPA_BUF *ppa_buf)
{
	struct dst_entry *dst = ppa_dst(ppa_buf);

	return dst != NULL && (void *)(dst->input) == (void *)ip_local_deliver ? 1 : 0;
}
EXPORT_SYMBOL(ppa_is_pkt_local);

int32_t ppa_is_pkt_routing(PPA_BUF *ppa_buf)
{
	struct dst_entry *dst = ppa_dst(ppa_buf);
#if IS_ENABLED(CONFIG_IPV6)
	if (ppa_is_pkt_ipv6(ppa_buf)) {
		return dst != NULL && (void *)(dst->input) == (void *)ip6_forward ? 1 : 0;
	}
#endif
	return dst != NULL && (void *)(dst->input) == (void *)ip_forward ? 1 : 0;
}
EXPORT_SYMBOL(ppa_is_pkt_routing);

int32_t ppa_is_pkt_mc_routing(PPA_BUF *ppa_buf)
{
	struct dst_entry *dst = ppa_dst(ppa_buf);
#if IS_ENABLED(CONFIG_IPV6)
	if (ppa_is_pkt_ipv6(ppa_buf)) {
		return 0;/*dst != NULL && (uint32_t)(dst->input) == (uint32_t)ip6_mc_input ? 1 : 0;*/
	}
#endif
	return dst != NULL && (void *)(dst->output) == (void *)ip_mc_output ? 1 : 0;
}
EXPORT_SYMBOL(ppa_is_pkt_mc_routing);

struct rtable *ppa_ip_route_output_key(struct net *net, struct flowi4 *flp)
{
	return ip_route_output_key(net, flp);
}
EXPORT_SYMBOL(ppa_ip_route_output_key);

#if IS_ENABLED(CONFIG_IPV6)
struct dst_entry *ppa_ip6_route_output(struct net *net,
		const struct sock *sk,
		struct flowi6 *fl6)
{
	return ip6_route_output(net, sk, fl6);
}
EXPORT_SYMBOL(ppa_ip6_route_output);
#endif

/* calculate teh hash for the conntrack 			 */
/* copied from the nf_conntrack_core.c function hash_conntrack_raw*/
/* needs to be updated if the original function is updated */
static unsigned int ppa_nf_conntrack_hash_rnd;

u32 ppa_hash_conntrack_raw(const struct nf_conntrack_tuple *tuple,
		const struct net *net)
{
	unsigned int n;
	u32 seed;

	get_random_once(&ppa_nf_conntrack_hash_rnd, sizeof(ppa_nf_conntrack_hash_rnd));

	/* The direction must be ignored, so we hash everything up to the
	 * destination ports (which is a multiple of 4) and treat the last
	 * three bytes manually.
	 */
	seed = ppa_nf_conntrack_hash_rnd ^ net_hash_mix(net);
	n = (sizeof(tuple->src) + sizeof(tuple->dst.u3)) / sizeof(u32);
	return jhash2((u32 *)tuple, n, seed ^
			(((__force __u16)tuple->dst.u.all << 16) |
			 tuple->dst.protonum));
}
EXPORT_SYMBOL_GPL(ppa_hash_conntrack_raw);

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
static inline int ppa_ipv4_local_deliver(struct sk_buff *skb)
{
	int ret = 0;
	const struct net_protocol *ipprot;
	struct net *net = dev_net(skb->dev);
	int protocol;

	protocol = ip_hdr(skb)->protocol;
resubmit:
	ipprot = rcu_dereference(inet_protos[protocol]);
	if (ipprot) {
		if (!ipprot->no_policy) {
			if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
				kfree_skb(skb);
				pr_err("%s:%d xfrm4_policy_check failed!\n",
				       __func__, __LINE__);
				goto out;
			}
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 3, 18)
			nf_reset_ct(skb);
#else
			nf_reset(skb);
#endif /* LINUX_VERSION_CODE > KERNEL_VERSION(5, 3, 18) */
		}
		ret = ipprot->handler(skb);
		if (ret < 0) {
			protocol = -ret;
			pr_err("%s:%d L4 handler failed for proto 0x%x"
			       , __func__, __LINE__, protocol);
			goto resubmit;
		}
		__IP_INC_STATS(net, IPSTATS_MIB_INDELIVERS);
	} else {
		__IP_INC_STATS(net, IPSTATS_MIB_INDELIVERS);
		consume_skb(skb);
	}
out:
	return ret;
}

static inline int ppa_ipv6_local_deliver(struct sk_buff *skb)
{
	int ret = 0;
#if IS_ENABLED(CONFIG_IPV6)
	int protocol, nhoff;
	struct net *net = dev_net(skb->dev);
	const struct inet6_protocol *ip6prot;
	struct inet6_dev *idev;

resubmit_v6:
	idev = ip6_dst_idev(skb_dst(skb));
	nhoff = IP6CB(skb)->nhoff;
	protocol = skb_network_header(skb)[nhoff];
resubmit_v6_final:
	ip6prot = rcu_dereference(inet6_protos[protocol]);
	if (ip6prot) {
		if (!(ip6prot->flags & INET6_PROTO_NOPOLICY) &&
		    !xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
			goto discard;
		ret = ip6prot->handler(skb);
		if (ret > 0) {
			if (ip6prot->flags & INET6_PROTO_FINAL) {
				protocol = ret;
				goto resubmit_v6_final;
			} else {
				goto resubmit_v6;
			}
		} else if (ret == 0) {
			__IP6_INC_STATS(net, idev, IPSTATS_MIB_INDELIVERS);
		}
	} else {
		pr_err("%s:%d No handler for nexthdr/L4 proto 0x%x!",
		       __func__, __LINE__, protocol);
discard:
		consume_skb(skb);
		__IP6_INC_STATS(net, idev, IPSTATS_MIB_INDISCARDS);
	}
#else
	consume_skb(skb);
	pr_err("%s:%d No IPv6 support in kernel!!", __func__, __LINE__);
	ret = PPA_FAILURE;
#endif /* CONFIG_IPV6 */

	return ret;
}

int32_t ppa_sw_litepath_local_deliver(struct sk_buff *skb)
{
	int ret = 0;

	rcu_read_lock();
	if (skb->protocol == htons(ETH_P_IP))
		ret = ppa_ipv4_local_deliver(skb);
	else
		ret = ppa_ipv6_local_deliver(skb);
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL_GPL(ppa_sw_litepath_local_deliver);
#endif /* (CONFIG_PPA_TCP_LITEPATH || CONFIG_PPA_UDP_LITEPATH) */

#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
static int32_t ppa_ct_ext_id = MXL_SKB_EXT_INVALID;
extern int32_t (*ppa_sw_fastpath_send_hook)(PPA_SKBUF *skb);

static int32_t register_ct_ext(void)
{
	/* Check if ID alredy exist*/
	if (ppa_ct_ext_id != MXL_SKB_EXT_INVALID) {
		pr_debug("User %s exists %s %d \n",
			    PPA_EXT_NAME, __FUNCTION__, __LINE__);
		return PPA_SUCCESS;
	}

	ppa_ct_ext_id = mxl_skb_ext_register(PPA_EXT_NAME,
					     sizeof(struct ppa_ct_ext));
	if (ppa_ct_ext_id == MXL_SKB_EXT_INVALID) {
		printk(KERN_INFO "mxl_skb_ext_register failed %s %d \n",
		       __FUNCTION__, __LINE__);
		return PPA_FAILURE;
	}
	return PPA_SUCCESS;
}

static unsigned int ppa_in_hook_fn(void *priv, struct sk_buff *skb,
				   const struct nf_hook_state *state)
{
	struct nf_conn *ct = NULL;
	enum ip_conntrack_info ctinfo;
	struct ppa_ct_ext *ext = NULL;
	uint8_t proto;
	uint16_t port;

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct || ppa_ct_ext_id == MXL_SKB_EXT_INVALID)
		return NF_ACCEPT;

	proto = nf_ct_protonum(ct);
	switch (proto) {
	case IPPROTO_UDP:
		port = ntohs(nf_ct_tuple(ct,
					 CTINFO2DIR(ctinfo))->dst.u.udp.port);
		/* for L2TP(1701)/ESP(4500)/VxLAN(4789) over UDP,
		 * the internal header tuple is needed
		 */
		if (port == 1701 || port == 4500 || port == 4789)
			break;
		fallthrough;
	case IPPROTO_TCP:
		/* connection track needs to be stored along with the skb
		 * so that it can be used in learning path
		 */
		if (mxl_skb_ext_find(skb, ppa_ct_ext_id))
			break;
		ext = mxl_skb_ext_add(skb, ppa_ct_ext_id);
		if (!ext) {
			pr_err("failed to add ppa_ct_ext extension!\n");
			break;
		}

		ext->ct = ct;
		ext->ctinfo = (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL ?
				       PPA_F_SESSION_ORG_DIR :
				       PPA_F_SESSION_REPLY_DIR);
	default:
		break;
	}

	return NF_ACCEPT;
}

static struct nf_hook_ops ipt_hook_ops[] __read_mostly = {
	/* hook for pre-routing ipv4 packets */
	{
		.hook 		= ppa_in_hook_fn,
		.hooknum 	= 0, /*NF_IP_PRE_ROUTING*/
		.pf		= PF_INET,
		.priority	= NF_IP_PRI_CONNTRACK_HELPER,
	},
	/* hook for pre-routing ipv6 packets */
	{
		.hook 		= ppa_in_hook_fn,
		.hooknum 	= 0, /*NF_IP_PRE_ROUTING*/
		.pf		= PF_INET6,
		.priority	= NF_IP6_PRI_CONNTRACK_HELPER,
	},
	/* hook for local-out ipv4 packets */
	{
		.hook 		= ppa_in_hook_fn,
		.hooknum 	= 3, /*NF_IP_LOCAL_OUT*/
		.pf		= PF_INET,
		.priority	= NF_IP_PRI_LAST,
	},
	/* hook for local-out ipv6 packets */
	{
		.hook 		= ppa_in_hook_fn,
		.hooknum 	= 3, /*NF_IP_LOCAL_OUT*/
		.pf		= PF_INET6,
		.priority	= NF_IP6_PRI_LAST,
	}
};

struct ppa_ct_ext *ppa_get_skb_ct_ext(struct sk_buff *skb)
{
	return mxl_skb_ext_find(skb, ppa_ct_ext_id);
}
EXPORT_SYMBOL(ppa_get_skb_ct_ext);
#endif /* CONFIG_MXL_SKB_EXT */
#endif /*CONFIG_PPA_EXT_PKT_LEARNING*/

#if IS_ENABLED(CONFIG_INTEL_IPQOS)
static unsigned int ppa_qos_postrt_hook_fn(void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
#if IS_ENABLED(CONFIG_INTEL_IPQOS_MARK_SKBPRIO)
	skb_mark_priority(skb);
#endif
	return NF_ACCEPT;
}

static struct nf_hook_ops qos_hook_ops[] __read_mostly = {
	/* hook for post-routing ipv4 packets */
	{
		.hook		= ppa_qos_postrt_hook_fn,
		.hooknum	= 4, /*NF_IP_POST_ROUTING*/
		.pf		= PF_INET,
		.priority	= NF_IP_PRI_LAST,
	},
	/* hook for post-routing ipv6 packets */
	{
		.hook		= ppa_qos_postrt_hook_fn,
		.hooknum	= 4, /*NF_IP_POST_ROUTING*/
		.pf		= PF_INET6,
		.priority	= NF_IP6_PRI_LAST,
	}
};
#endif /* CONFIG_INTEL_IPQOS*/

#if IS_ENABLED(CONFIG_PPA_BR_SESS_LEARNING)
static unsigned int ppa_br_prert_hook_fn (void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
	if (ppa_hook_session_add_fn != NULL) {

		ppa_hook_session_add_fn(skb,
				NULL, PPA_F_BRIDGED_SESSION|PPA_F_BEFORE_NAT_TRANSFORM);
	}
	return NF_ACCEPT;
}

static unsigned int ppa_br_postrt_hook_fn(void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
	if (ppa_hook_session_add_fn != NULL) {
		//printk(KERN_INFO"br Post-routing hook \n");
		ppa_hook_session_add_fn(skb,
				NULL,
				PPA_F_BRIDGED_SESSION);
	}
	return NF_ACCEPT;
}

static struct nf_hook_ops ebt_hook_ops[] __read_mostly = {
	/* hook for bridge pre-routing packets */
	{
		.hook 		= ppa_br_prert_hook_fn,
		.hooknum 	= 0, /*NF_IP_PRE_ROUTING*/
		.pf		= NFPROTO_BRIDGE,
		.priority	= NF_BR_PRI_NAT_DST_BRIDGED,
	},
	/* hook for bridge post-routing packets */
	{
		.hook 		= ppa_br_postrt_hook_fn,
		.hooknum 	= 4, /*NF_IP_POST_ROUTING*/
		.pf		= NFPROTO_BRIDGE,
		.priority	= NF_BR_PRI_NAT_SRC,
	}
};
#endif /* CONFIG_PPA_BR_SESS_LEARNING*/

#if IS_ENABLED(CONFIG_INTEL_IPQOS)
#if defined(CONFIG_VLAN_8021Q_COPY_TO_EXTMARK)
static unsigned int ppa_qos_br_prert_hook_fn(void *priv,
					     struct sk_buff *skb,
					     const struct nf_hook_state *state)
{
	u16 vlan_id;
	u32 vprio;
	u32 mark = 0, mask = 0;

	/* currently it is for vlan id/prio,
	 * so return NF_ACCEPT immediately if vlan not present
	 */
	if (!skb_vlan_tag_present(skb))
		return NF_ACCEPT;

	vlan_id = skb_vlan_tag_get_id(skb);
	vprio = skb_vlan_tag_get_prio(skb);

	SET_DATA_FROM_MARK_OPT(mark, VLANID_MASK, VLANID_START_BIT_POS, vlan_id);
	SET_DATA_FROM_MARK_OPT(mark, VPRIO_MASK, VPRIO_START_BIT_POS, vprio);
	mask |= (VLANID_MASK | VPRIO_MASK);
#ifdef HAVE_QOS_EXTMARK
	ppa_set_skb_extmark(skb, mark, mask);
#else
	SET_DATA_FROM_MARK_OPT(skb->mark, mask, 0, mark);
#endif /* HAVE_QOS_EXTMARK */

	return NF_ACCEPT;
}
#endif

static unsigned int ppa_qos_br_postrt_hook_fn(void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
#if IS_ENABLED(CONFIG_INTEL_IPQOS_MARK_SKBPRIO)
	skb_mark_priority(skb);
#endif
	return NF_ACCEPT;
}

static struct nf_hook_ops qos_ebt_hook_ops[] __read_mostly = {
#if defined(CONFIG_VLAN_8021Q_COPY_TO_EXTMARK)
	/* hook for bridge pre-routing packets */
	{
		.hook           = ppa_qos_br_prert_hook_fn,
		.hooknum        = NF_BR_PRE_ROUTING,
		.pf             = NFPROTO_BRIDGE,
		.priority       = NF_BR_PRI_NAT_DST_BRIDGED,
	},
#endif
	/* hook for bridge post-routing packets */
	{
		.hook		= ppa_qos_br_postrt_hook_fn,
		.hooknum	= 4, /*NF_BR_POST_ROUTING*/
		.pf		= NFPROTO_BRIDGE,
		.priority	= NF_BR_PRI_NAT_SRC,
	}
};
#endif /* CONFIG_INTEL_IPQOS*/

/*Refresh conntrack for this many jiffies and do accounting */
/*This replictes the functionality of kernel api __nf_ct_refresh_acct*/
void ppa_nf_ct_refresh_acct(struct nf_conn *ct,
		enum ip_conntrack_info ctinfo,
		unsigned long extra_jiffies,
		unsigned long bytes,
		unsigned int pkts)
{
	struct nf_conn_acct *acct = NULL;

	if (!ct)
		return;

	/* Only update if this is not a fixed timeout */
	if (test_bit(IPS_FIXED_TIMEOUT_BIT, &ct->status))
		goto do_acct;

	/* If not in hash table, timer will not be active yet */
	if (nf_ct_is_confirmed(ct)) {
		extra_jiffies += nfct_time_stamp;

		if (READ_ONCE(ct->timeout) < extra_jiffies)
			WRITE_ONCE(ct->timeout, extra_jiffies);
	}

do_acct:
	acct = nf_conn_acct_find(ct);
	if (acct) {
		struct nf_conn_counter *counter = acct->counter;

		atomic64_add(pkts, &counter[CTINFO2DIR(ctinfo)].packets);
		atomic64_add(bytes, &counter[CTINFO2DIR(ctinfo)].bytes);
	}
}
EXPORT_SYMBOL(ppa_nf_ct_refresh_acct);

static const struct nf_ct_hook __rcu *orig_nfct_hook;
static struct nf_ct_hook ppa_nfct_hook={0};

/* note: already called with rcu_read_lock from nf_conntrack_destroy() */
static void ppa_destroy_conntrack(struct nf_conntrack *nfct)
{
	struct nf_conn *ct = (struct nf_conn *)nfct;
	const struct nf_ct_hook *ct_hook;

	if (ppa_hook_session_del_fn != NULL && !in_irq() && !irqs_disabled()) {
		ppa_hook_session_del_fn(ct, PPA_F_SESSION_ORG_DIR | PPA_F_SESSION_REPLY_DIR);
	}

	ct_hook = rcu_dereference(orig_nfct_hook);
	if (ct_hook)
		ct_hook->destroy(nfct);
}

#ifdef CONFIG_RFS_ACCEL
static struct rps_dev_flow * ppa_set_rps_cpu(struct net_device *dev, struct sk_buff *skb,
            struct rps_dev_flow *rflow, u16 next_cpu)
{
	if (next_cpu < nr_cpu_ids) {
#ifdef CONFIG_RFS_ACCEL
		struct netdev_rx_queue *rxqueue;
		struct rps_dev_flow_table *flow_table;
		struct rps_dev_flow *old_rflow;
		u32 flow_id;
		u16 rxq_index;
		int rc;

		/* Should we steer this flow to a different hardware queue? */
		if (!skb_rx_queue_recorded(skb) || !dev->rx_cpu_rmap ||
				!(dev->features & NETIF_F_NTUPLE))
			goto out;
		rxq_index = cpu_rmap_lookup_index(dev->rx_cpu_rmap, next_cpu);
		if (rxq_index == skb_get_rx_queue(skb))
			goto out;

		rxqueue = dev->_rx + rxq_index;
		flow_table = rcu_dereference(rxqueue->rps_flow_table);
		if (!flow_table)
			goto out;
		flow_id = skb_get_hash(skb) & flow_table->mask;
		rc = dev->netdev_ops->ndo_rx_flow_steer(dev, skb,
				rxq_index, flow_id);
		if (rc < 0)
			goto out;
		old_rflow = rflow;
		rflow = &flow_table->flows[flow_id];
		rflow->filter = rc;
		if (old_rflow->filter == rflow->filter)
			old_rflow->filter = RPS_NO_FILTER;
out:
#endif
		rflow->last_qtail =
			per_cpu(softnet_data, next_cpu).input_queue_head;
	}

	rflow->cpu = next_cpu;
	return rflow;
}

int ppa_get_rps_cpu(struct net_device *dev, struct sk_buff *skb,
                       struct rps_dev_flow **rflowp)
{
	const struct rps_sock_flow_table *sock_flow_table;
	struct netdev_rx_queue *rxqueue = dev->_rx;
	struct rps_dev_flow_table *flow_table;
	struct rps_map *map;
	int cpu = -1;
	u32 tcpu;
	u32 hash;

	if (skb_rx_queue_recorded(skb)) {
		u16 index = skb_get_rx_queue(skb);

		if (unlikely(index >= dev->real_num_rx_queues)) {
			WARN_ONCE(dev->real_num_rx_queues > 1,
					"%s received packet on queue %u, but number "
					"of RX queues is %u\n",
					dev->name, index, dev->real_num_rx_queues);
			goto done;
		}
		rxqueue += index;
	}

	/* Avoid computing hash if RFS/RPS is not active for this rxqueue */

	flow_table = rcu_dereference(rxqueue->rps_flow_table);
	map = rcu_dereference(rxqueue->rps_map);
	if (!flow_table && !map)
		goto done;

	skb_reset_network_header(skb);
	hash = skb_get_hash(skb);
	if (!hash)
		goto done;

	sock_flow_table = rcu_dereference(rps_sock_flow_table);
	if (flow_table && sock_flow_table) {
		struct rps_dev_flow *rflow;
		u32 next_cpu;
		u32 ident;

		/* First check into global flow table if there is a match */
		ident = sock_flow_table->ents[hash & sock_flow_table->mask];
		if ((ident ^ hash) & ~rps_cpu_mask)
			goto try_rps;

		next_cpu = ident & rps_cpu_mask;

		/* OK, now we know there is a match,
		 * we can look at the local (per receive queue) flow table
		 */
		rflow = &flow_table->flows[hash & flow_table->mask];
		tcpu = rflow->cpu;

		/*
		 * If the desired CPU (where last recvmsg was done) is
		 * different from current CPU (one in the rx-queue flow
		 * table entry), switch if one of the following holds:
		 *   - Current CPU is unset (>= nr_cpu_ids).
		 *   - Current CPU is offline.
		 *   - The current CPU's queue tail has advanced beyond the
		 *     last packet that was enqueued using this table entry.
		 *     This guarantees that all previous packets for the flow
		 *     have been dequeued, thus preserving in order delivery.
		 */
		if (unlikely(tcpu != next_cpu) &&
				(tcpu >= nr_cpu_ids || !cpu_online(tcpu) ||
				 ((int)(per_cpu(softnet_data, tcpu).input_queue_head -
					 rflow->last_qtail)) >= 0)) {
			tcpu = next_cpu;
			rflow = ppa_set_rps_cpu(dev, skb, rflow, next_cpu);
		}

		if (tcpu < nr_cpu_ids && cpu_online(tcpu)) {
			*rflowp = rflow;
			cpu = tcpu;
			goto done;
		}
	}

try_rps:
	if (map) {
		tcpu = map->cpus[reciprocal_scale(hash, map->len)];
		if (cpu_online(tcpu)) {
			cpu = tcpu;
			goto done;
		}
	}

done:
	return cpu;
}
EXPORT_SYMBOL(ppa_get_rps_cpu);
#endif /* CONFIG_RFS_ACCEL */

static void ppa_register_delhook(void)
{
	const struct nf_ct_hook *ct_hook;

	rcu_read_lock();
	ct_hook = rcu_dereference(nf_ct_hook);
	if (ct_hook) {
		memcpy(&ppa_nfct_hook, ct_hook, sizeof(struct nf_ct_hook));
		ppa_nfct_hook.destroy = ppa_destroy_conntrack;

		RCU_INIT_POINTER(orig_nfct_hook, ct_hook);
		RCU_INIT_POINTER(nf_ct_hook, &ppa_nfct_hook);
	}
	rcu_read_unlock();
}

static void ppa_unregister_delhook(void)
{
	RCU_INIT_POINTER(nf_ct_hook, orig_nfct_hook);
	RCU_INIT_POINTER(orig_nfct_hook, NULL);
}

int ppa_api_register_hooks(void)
{
	int ret = 0;

#if IS_ENABLED(CONFIG_INTEL_IPQOS)
	/*qos ipt hooks*/
	nf_register_net_hooks(&init_net, qos_hook_ops, ARRAY_SIZE(qos_hook_ops));
	/*qos ebt hooks*/
	nf_register_net_hooks(&init_net, qos_ebt_hook_ops, ARRAY_SIZE(qos_ebt_hook_ops));
#endif /* CONFIG_INTEL_IPQOS*/

#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
	register_ct_ext();
	/*ipt hooks*/
	nf_register_net_hooks(&init_net, ipt_hook_ops, ARRAY_SIZE(ipt_hook_ops));
#endif /* CONFIG_MXL_SKB_EXT */
#endif /*CONFIG_PPA_EXT_PKT_LEARNING*/

	/*delete conntrack hook*/
	ppa_register_delhook();
	return ret;
}
EXPORT_SYMBOL(ppa_api_register_hooks);

void ppa_api_unregister_hooks(void)
{
#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
	/*ipt hooks*/
	nf_unregister_net_hooks(&init_net, ipt_hook_ops, ARRAY_SIZE(ipt_hook_ops));
#endif /*def MXL_SKB_EXT*/
#endif /*CONFIG_PPA_EXT_PKT_LEARNING*/

#if IS_ENABLED(CONFIG_INTEL_IPQOS)
	/*qos ipt hooks*/
	nf_unregister_net_hooks(&init_net, qos_hook_ops, ARRAY_SIZE(qos_hook_ops));
	/*qos ebt hooks*/
	nf_unregister_net_hooks(&init_net, qos_ebt_hook_ops, ARRAY_SIZE(qos_ebt_hook_ops));
#endif /* CONFIG_INTEL_IPQOS*/

	/*delete conntrack hook*/
	ppa_unregister_delhook();
}
EXPORT_SYMBOL(ppa_api_unregister_hooks);

int32_t (*ppa_update_pkt_devqos_priority_hook)(struct net_device *netif,
					       struct dp_subif *subif,
					       struct sk_buff *skb) = NULL;
EXPORT_SYMBOL(ppa_update_pkt_devqos_priority_hook);
