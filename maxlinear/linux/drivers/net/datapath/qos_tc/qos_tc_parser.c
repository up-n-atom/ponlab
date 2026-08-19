// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 *
 *****************************************************************************/

#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/version.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <net/ipv6.h>
#include <net/ndisc.h>
#include <net/pkt_cls.h>
#include <net/switch_api/lantiq_gsw_flow.h>
#include <uapi/linux/icmpv6.h>
#include "qos_tc_flower.h"
#include "qos_tc_parser.h"

#define PON_PORT_ID 2

static int parse_mask_ip(struct net_device *dev,
			 GSW_PCE_rule_t *rule,
			 u32 *mask,
			 int mask_size)
{
	int m, i, idx = 0;
	u32 mask_cpy;

	if (!mask)
		return -EIO;

	if (mask_size != 1 && mask_size != 4)
		return -EIO;

	/* Convert mask to nimble */
	for (m = 0; m < mask_size; m++) {
		mask_cpy = htonl(mask[m]);
		for (i = 0; i < 4; i++, idx += 2) {
			if (!(mask_cpy & 0x0f))
				rule->pattern.nDstIP_Mask |= 1 << idx;
			if (!(mask_cpy & 0xf0))
				rule->pattern.nDstIP_Mask |= 1 << (idx + 1);
			mask_cpy >>= 8;
		}
	}

	/* Reorder bytes in case of IPv6 masks because network byte order
	 * is expected. Not required in case of IPv4 as only the lowest
	 * significant byte is used and all others are ignored.
	 */
	if (mask_size == 4)
		rule->pattern.nDstIP_Mask = htonl(rule->pattern.nDstIP_Mask);

	netdev_dbg(dev, "%s: nDstIP_Mask: %x\n",
		   __func__, rule->pattern.nDstIP_Mask);

	return 0;
}

static int parse_ipv4(struct net_device *dev,
		      struct flow_cls_offload *f,
		      GSW_PCE_rule_t *rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_ipv4_addrs *key;
	struct flow_dissector_key_ipv4_addrs *mask;

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV4_ADDRS))
		return -EIO;

	key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_IPV4_ADDRS,
					qos_tc_get_key(f));
	mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_IPV4_ADDRS,
					 qos_tc_get_mask(f));

	rule->pattern.eDstIP_Select = GSW_PCE_IP_V4;
	rule->pattern.nDstIP.nIPv4 = htonl(key->dst);

	return parse_mask_ip(dev, rule, &mask->dst, 1);
}

static int parse_ipv6(struct net_device *dev,
		      struct flow_cls_offload *f,
		      GSW_PCE_rule_t *rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_ipv6_addrs *key;
	struct flow_dissector_key_ipv6_addrs *mask;
	int i;

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV6_ADDRS))
		return -EIO;

	key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_IPV6_ADDRS,
					qos_tc_get_key(f));
	mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_IPV6_ADDRS,
					 qos_tc_get_mask(f));

	rule->pattern.eDstIP_Select = GSW_PCE_IP_V6;
	/* IPv6 addresses inside structure GSW_PCE_rule_t are stored
	 * in an array of 16bit values
	 */
	for (i = 0; i < 8; ++i) {
		((uint16_t *)rule->pattern.nDstIP.nIPv6)[i] =
			htons(((uint16_t *)&key->dst)[i]);
	}

	return parse_mask_ip(dev, rule, mask->dst.in6_u.u6_addr32, 4);
}

static u16 get_addr_type(struct net_device *dev,
			 struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_control *key;

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_CONTROL))
		return 0;

	key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CONTROL,
					qos_tc_get_key(f));
	return key->addr_type;
}

int qos_tc_parse_ip(struct net_device *dev,
		    struct flow_cls_offload *f,
		    GSW_PCE_rule_t *pce_rule)
{
	u16 addr_type;

	addr_type = get_addr_type(dev, f);

	switch (addr_type) {
	case FLOW_DISSECTOR_KEY_IPV4_ADDRS:
		return parse_ipv4(dev, f, pce_rule);
	case FLOW_DISSECTOR_KEY_IPV6_ADDRS:
		return parse_ipv6(dev, f, pce_rule);
	default:
		return 0;
	}

	return 0;
}

static int check_mld_type(struct net_device *dev,
			  struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_icmp *key = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_ICMP)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_ICMP,
						qos_tc_get_key(f));

		switch (key->type) {
		case ICMPV6_MGM_QUERY:
		case ICMPV6_MGM_REPORT:
		case ICMPV6_MGM_REDUCTION:
		case ICMPV6_MLD2_REPORT:
		case ICMPV6_NI_REPLY:
			netdev_dbg(dev, "MLD type: %u\n", key->type);
			return 0;
		default:
			netdev_dbg(dev, "Invalid MLD type %#x\n", key->type);
			return -EIO;
		}
	}

	netdev_dbg(dev, "Missing MLD type\n");
	return -EIO;
}

bool qos_tc_parse_is_mcc(struct net_device *dev,
			 struct flow_cls_offload *f)
{
	struct flow_dissector_key_basic *key_basic = NULL;
	struct flow_dissector_key_ipv4_addrs *key_ipv4_addrs = NULL;
	struct flow_dissector_key_ipv6_addrs *key_ipv6_addrs = NULL;
	struct flow_dissector *d = qos_tc_get_dissector(f);
	u16 addr_type;
	int ret;

	addr_type = get_addr_type(dev, f);

	switch (addr_type) {
	case FLOW_DISSECTOR_KEY_IPV4_ADDRS:
		if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV4_ADDRS)) {
			key_ipv4_addrs = skb_flow_dissector_target(d,
						FLOW_DISSECTOR_KEY_IPV4_ADDRS,
						qos_tc_get_key(f));

			return ipv4_is_multicast(key_ipv4_addrs->dst);
		}

		break;
	case FLOW_DISSECTOR_KEY_IPV6_ADDRS:
		if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV6_ADDRS)) {
			key_ipv6_addrs = skb_flow_dissector_target(d,
						FLOW_DISSECTOR_KEY_IPV6_ADDRS,
						qos_tc_get_key(f));

			return ipv6_addr_is_multicast(&key_ipv6_addrs->dst);
		}

		break;
	default:
		if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
			key_basic = skb_flow_dissector_target(d,
						FLOW_DISSECTOR_KEY_BASIC,
						qos_tc_get_key(f));

			if (key_basic->ip_proto == IPPROTO_IGMP)
				return true;

			if (key_basic->ip_proto == IPPROTO_ICMPV6) {
				ret = check_mld_type(dev, f);
				if (ret == 0)
					return true;
			}
		}

		break;
	}

	return false;
}

void qos_tc_set_drop(struct net_device *dev,
		     struct flow_cls_offload *f,
		     GSW_PCE_rule_t *pce_rule)
{
	if (qos_tc_parse_is_mcc(dev, f)) {
		pce_rule->action.ePortFilterType_Action =
			GSW_PCE_PORT_FILTER_ACTION_6;
	} else {
		pce_rule->action.ePortFilterType_Action =
			GSW_PCE_PORT_FILTER_ACTION_1;
	}
}

/* Special tags are deactivated on LAN ports if CONFIG_MXL_ETHSW_NOSPTAG is set.
 * The match on subifid should not be used in this case as it cannot match.
 */
static void tc2pce_subif_set(GSW_PCE_rule_t *pce_rule, dp_subif_t *dp_subif)
{
	if (IS_ENABLED(CONFIG_MXL_ETHSW_NOSPTAG) &&
	    (dp_subif->alloc_flag & DP_F_FAST_ETH_LAN))
		return;

	pce_rule->pattern.eSubIfIdType = GSW_PCE_SUBIFID_TYPE_GROUP;
	pce_rule->pattern.bSubIfIdEnable = 1;
	pce_rule->pattern.nSubIfId = dp_subif->subif;
}

static void tc2pce_subif_parse(struct net_device *dev,
			       GSW_PCE_rule_t *pce_rule,
			       struct net_device *indev)
{
	dp_subif_t *dp_subif;
	int ret;

	dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
	if (!dp_subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return;
	}

	if (indev) {
		ret = dp_get_netif_subifid(indev, NULL, NULL, NULL,
					   dp_subif, 0);
	} else {
		ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, dp_subif, 0);
	}

	/* If dp_get_netif_subifid() fails assume that this rule should be
	 * configured for all sub interfaces. This is for example the case
	 * for pon0.
	 */
	if (ret != DP_SUCCESS) {
		kfree(dp_subif);
		if (dev != indev)
			return;
		/* If dev is equal indev assume that this rule should be
		 * configured only for PON sub interface.
		 */
		pce_rule->pattern.bPortIdEnable = 1;
		pce_rule->pattern.nPortId = PON_PORT_ID;
		return;
	}

	pce_rule->logicalportid = dp_subif->port_id;
	pce_rule->pattern.bPortIdEnable = 1;
	pce_rule->pattern.nPortId = dp_subif->port_id;

	if (dp_subif->flag_pmapper) {
		pce_rule->pattern.eSubIfIdType =
			GSW_PCE_SUBIFID_TYPE_BRIDGEPORT;

		pce_rule->subifidgroup = dp_subif->bport;
		pce_rule->pattern.bSubIfIdEnable = 1;
		pce_rule->pattern.nSubIfId = dp_subif->bport;
	} else {
		pce_rule->subifidgroup = dp_subif->subif_groupid;
		tc2pce_subif_set(pce_rule, dp_subif);
	}
	kfree(dp_subif);

	netdev_dbg(dev, "%s: nPortId = %d\n", __func__,
		   pce_rule->pattern.nPortId);
	netdev_dbg(dev, "%s: nSubIfId = %d\n", __func__,
		   pce_rule->pattern.nSubIfId);
}

static void tc2pce_proto_parse(struct net_device *dev,
			       struct flow_cls_offload *f,
			       GSW_PCE_rule_t *pce_rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		struct flow_dissector_key_basic *key =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_key(f));
		struct flow_dissector_key_basic *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_mask(f));
		if (mask->ip_proto) {
			pce_rule->pattern.bProtocolEnable = 1;
			pce_rule->pattern.nProtocol = key->ip_proto;
			netdev_dbg(dev, "%s: bProtocolEnable = %d\n",
				   __func__, pce_rule->pattern.bProtocolEnable);
			netdev_dbg(dev, "%s: nProtocol = %#x\n",
				   __func__, pce_rule->pattern.nProtocol);
		}
		if (mask->n_proto) {
			pce_rule->pattern.bEtherTypeEnable = 1;
			pce_rule->pattern.nEtherType = ntohs(key->n_proto);
			netdev_dbg(dev, "%s: bEtherTypeEnable = %d\n",
				   __func__,
				   pce_rule->pattern.bEtherTypeEnable);
			netdev_dbg(dev, "%s: nEtherType = %#x\n",
				   __func__, pce_rule->pattern.nEtherType);
		}
	}
}

static void tc2pce_mld_parse(struct net_device *dev,
			     struct flow_cls_offload *f,
			     GSW_PCE_rule_t *pce_rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		struct flow_dissector_key_basic *key =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_key(f));
		struct flow_dissector_key_basic *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_mask(f));

		/* Use flag FLAG_ICMPv6 in case of MC related rules for MLD
		 * instead of checking for ICMPv6 protocol.
		 * Note: nParserFlag1LSB contains bits 47 - 32:
		 * FLAG_ICMPv6 corresponds to bit 38: 38 - 32 = 6
		 */
		pce_rule->pattern.bParserFlag1LSB_Enable = 1;
		pce_rule->pattern.nParserFlag1LSB = (1U << 6);
		pce_rule->pattern.nParserFlag1LSB_Mask =
			~pce_rule->pattern.nParserFlag1LSB;
		netdev_dbg(dev, "%s: bParserFlag1LSB_Enable = %d\n", __func__,
			   pce_rule->pattern.bParserFlag1LSB_Enable);
		netdev_dbg(dev, "%s: nParserFlag1LSB = %#x\n", __func__,
			   pce_rule->pattern.nParserFlag1LSB);

		if (mask->n_proto) {
			pce_rule->pattern.bEtherTypeEnable = 1;
			pce_rule->pattern.nEtherType = ntohs(key->n_proto);
			netdev_dbg(dev, "%s: bEtherTypeEnable = %d\n",
				   __func__,
				   pce_rule->pattern.bEtherTypeEnable);
			netdev_dbg(dev, "%s: nEtherType = %#x\n",
				   __func__, pce_rule->pattern.nEtherType);
		}
	}
}

static bool is_mac_set(GSW_PCE_rule_t *pce_rule)
{
	return pce_rule->pattern.bMAC_SrcEnable ||
	       pce_rule->pattern.bMAC_DstEnable;
}

static void vlan_pce_dump(struct net_device *dev,
			  GSW_PCE_rule_t *pce_rule)
{
	/* dump vlan info */
	netdev_dbg(dev, "%s: bSLAN_Vid = %d\n",
		   __func__, pce_rule->pattern.bSLAN_Vid);
	netdev_dbg(dev, "%s: nSLAN_Vid = %d\n",
		   __func__, pce_rule->pattern.nSLAN_Vid);
	netdev_dbg(dev, "%s: nOuterVidRange = %d\n",
		   __func__, pce_rule->pattern.nOuterVidRange);
	netdev_dbg(dev, "%s: bSVidRange_Select = %d\n",
		   __func__, pce_rule->pattern.bSVidRange_Select);
	netdev_dbg(dev, "%s: bSLANVid_Exclude = %d\n",
		   __func__, pce_rule->pattern.bSLANVid_Exclude);
	/* dump cvlan info */
	netdev_dbg(dev, "%s: bVid = %d\n",
		   __func__, pce_rule->pattern.bVid);
	netdev_dbg(dev, "%s: nVid = %d\n",
		   __func__, pce_rule->pattern.nVid);
	netdev_dbg(dev, "%s: nVidRange = %d\n",
		   __func__, pce_rule->pattern.nVidRange);
	netdev_dbg(dev, "%s: bVidRange_Select = %d\n",
		   __func__, pce_rule->pattern.bVidRange_Select);
	netdev_dbg(dev, "%s: bVid_Exclude = %d\n",
		   __func__, pce_rule->pattern.bVid_Exclude);
}

static void tc2pce_vlan_parse(struct net_device *dev,
			      struct flow_cls_offload *f,
			      GSW_PCE_rule_t *pce_rule,
			      struct net_device *indev)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_vlan *key = NULL;
	struct flow_dissector_key_vlan *mask = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						 qos_tc_get_mask(f));

		if (mask->vlan_id == 0xfff) {
			pce_rule->pattern.bSLAN_Vid = 1;
			pce_rule->pattern.nSLAN_Vid = key->vlan_id;
			pce_rule->pattern.bSVidRange_Select = 0;
			pce_rule->pattern.nOuterVidRange = 0x0;
			pce_rule->pattern.bOuterVid_Original = 0;
		}
		/* In first step we support only DEI=0,
		 * Later there will be further extensions.
		 */
		if (mask->vlan_priority) {
			pce_rule->pattern.bSTAG_PCP_DEI_Enable = 1;
			pce_rule->pattern.nSTAG_PCP_DEI = key->vlan_priority;
		}

		if (!mask->vlan_id && !mask->vlan_priority) {
			netdev_dbg(dev, "%s: vlan %#x match all config\n",
				   __func__,
				   ntohs(key->vlan_tpid));
			/* Range over all VLAN ids */
			pce_rule->pattern.bSLAN_Vid = 1;
			pce_rule->pattern.nSLAN_Vid = 0;
			pce_rule->pattern.bSVidRange_Select = 1;
			pce_rule->pattern.nOuterVidRange = 4095;
			pce_rule->pattern.bOuterVid_Original = 0;
			pce_rule->pattern.bSLANVid_Exclude = 0;
		}
	}

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CVLAN)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CVLAN,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CVLAN,
						 qos_tc_get_mask(f));

		if (mask->vlan_id == 0xfff) {
			pce_rule->pattern.bVid = 1;
			pce_rule->pattern.nVid = key->vlan_id;
			pce_rule->pattern.bVidRange_Select = 0;
			pce_rule->pattern.nVidRange = 0x0;
			pce_rule->pattern.bVid_Original = 0;
		}

		if (mask->vlan_priority) {
			pce_rule->pattern.bPCP_Enable = 1;
			pce_rule->pattern.nPCP = key->vlan_priority;
		}

		if (!mask->vlan_id && !mask->vlan_priority) {
			netdev_dbg(dev, "%s: cvlan %#x match all config\n",
				   __func__,
				   ntohs(key->vlan_tpid));
			pce_rule->pattern.bVid = 1;
			pce_rule->pattern.nVid = 0;
			pce_rule->pattern.bVidRange_Select = 1;
			pce_rule->pattern.nVidRange = 4095;
			pce_rule->pattern.bVid_Original = 0;
			pce_rule->pattern.bVid_Exclude = 0;
		}
	}

	/* case 1: if tc proto is all and we have mac filter then
	 * match all untagged and tagged packets
	 */
	if (f->common.protocol == htons(ETH_P_ALL) && is_mac_set(pce_rule))
		return;

	/* case 2: if proto is all and we have indev match all
	 * untagged and tagged packets on this device
	 */
	if (f->common.protocol == htons(ETH_P_ALL) && dev == indev)
		return;

	if (!eth_type_vlan(f->common.protocol)) {
		/* match untagged */
		pce_rule->pattern.bSLAN_Vid = 1;
		pce_rule->pattern.nSLAN_Vid = 0;
		pce_rule->pattern.bSVidRange_Select = 1;
		pce_rule->pattern.nOuterVidRange = 4095;
		pce_rule->pattern.bOuterVid_Original = 0;
		pce_rule->pattern.bSLANVid_Exclude = 1;
	}

	vlan_pce_dump(dev, pce_rule);
}

static void tc2pce_icmp_parse(struct net_device *dev,
			      struct flow_cls_offload *f,
			      GSW_PCE_rule_t *pce_rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_ICMP)) {
		struct flow_dissector_key_icmp *key =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_ICMP,
						  qos_tc_get_key(f));
		struct flow_dissector_key_icmp *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_ICMP,
						  qos_tc_get_mask(f));

		if (mask->type) {
			pce_rule->pattern.bAppDataMSB_Enable = 1;
			pce_rule->pattern.nAppDataMSB = key->type << 8;
			/* mask last two nibbles */
			pce_rule->pattern.nAppMaskRangeMSB = 0x3;
			netdev_dbg(dev, "%s: nAppDataMSB = %#x\n",
				   __func__, pce_rule->pattern.nAppDataMSB);
		}
	}
}

static bool tc_parse_is_mld(struct net_device *dev,
			    struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_basic *key = NULL;
	int ret;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						qos_tc_get_key(f));
		if (key->ip_proto == IPPROTO_ICMPV6) {
			ret = check_mld_type(dev, f);
			if (ret == 0)
				return true;
		}
	}

	return false;
}

static void tc2pce_key_eth_addr_parse(struct net_device *dev,
				      struct flow_cls_offload *f,
				      GSW_PCE_rule_t *pce_rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_eth_addrs *key = NULL;
	struct flow_dissector_key_eth_addrs *mask = NULL;
	int i = 0;
	int idx = 0;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_ETH_ADDRS)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_ETH_ADDRS,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d,
						 FLOW_DISSECTOR_KEY_ETH_ADDRS,
						 qos_tc_get_mask(f));

		pce_rule->pattern.bMAC_DstEnable =
						!is_zero_ether_addr(key->dst);
		pce_rule->pattern.bMAC_SrcEnable =
						!is_zero_ether_addr(key->src);

		netdev_dbg(dev, "%s: bMAC_DstEnable = %d bMAC_SrcEnable = %d\n",
			   __func__, pce_rule->pattern.bMAC_DstEnable,
			   pce_rule->pattern.bMAC_SrcEnable);

		/* Set bit in mask for each nibble different from F */
		for (i = ETH_ALEN - 1; i >= 0; i--, idx += 2) {
			if ((mask->dst[i] & 0x0f) != 0xf)
				pce_rule->pattern.nMAC_DstMask |= 1 << idx;
			if ((mask->dst[i] & 0xf0) != 0xf0)
				pce_rule->pattern.nMAC_DstMask |=
					1 << (idx + 1);

			if ((mask->src[i] & 0x0f) != 0xf)
				pce_rule->pattern.nMAC_SrcMask |= 1 << idx;
			if ((mask->src[i] & 0xf0) != 0xf0)
				pce_rule->pattern.nMAC_SrcMask |=
					1 << (idx + 1);
		}

		if (pce_rule->pattern.bMAC_DstEnable == 1) {
			for (i = 0; i < ARRAY_SIZE(key->dst); i++) {
				pce_rule->pattern.nMAC_Dst[i] = key->dst[i];
				netdev_dbg(dev, "%s: pce_rule->pattern.nMAC_Dst[i] = %d\n",
					   __func__,
					   pce_rule->pattern.nMAC_Dst[i]);
			}
		}

		if (pce_rule->pattern.bMAC_SrcEnable == 1) {
			for (i = 0; i < ARRAY_SIZE(key->src); i++) {
				pce_rule->pattern.nMAC_Src[i] = key->src[i];
				netdev_dbg(dev, "%s: pce_rule->pattern.nMAC_Src[i] = %d\n",
					   __func__,
					   pce_rule->pattern.nMAC_Src[i]);
			}
		}
	}
}

static void tc2pce_set_traffic_class(struct net_device *dev,
				     unsigned int classid,
				     GSW_PCE_rule_t *pce_rule)
{
	netdev_dbg(dev, "%s: eTrafficClassAction = 0x%x\n",
		   __func__, classid);

	if (classid != 0) {
		pce_rule->action.eTrafficClassAction =
			GSW_PCE_ACTION_TRAFFIC_CLASS_ALTERNATIVE;
		pce_rule->action.nTrafficClassAlternate = classid & 0xf;

		netdev_dbg(dev, "%s: eTrafficClassAction = %d\n",
			   __func__, pce_rule->action.eTrafficClassAction);
		netdev_dbg(dev, "%s: nTrafficClassAlternate = %d\n",
			   __func__, pce_rule->action.nTrafficClassAlternate);
	}
}

static void tc2pce_eth_proto_parse(struct net_device *dev,
				   struct flow_cls_offload *f,
				   GSW_PCE_rule_t *pce_rule)
{
	/* Special handling for MLD related rules:
	 * Instead of checking for ICMPv6 protocol type, use flag FLAG_ICMPv6
	 * in order to cover both, protocol type ICMPv6 as well as
	 * Hop-By-Hop option header
	 */
	if (tc_parse_is_mld(dev, f))
		/* Use flag ICMPv6 and ethertype based on n_proto */
		tc2pce_mld_parse(dev, f, pce_rule);
	else
		/* Set protocol based on ip_proto and ethertype based
		 * on n_proto
		 */
		tc2pce_proto_parse(dev, f, pce_rule);
}

int qos_tc_to_pce(struct net_device *dev,
		  struct flow_cls_offload *f,
		  GSW_PCE_rule_t *pce_rule)
{
	struct net_device *indev;
	int ret;

	ret = qos_tc_parse_ip(dev, f, pce_rule);
	if (ret < 0)
		return ret;

	indev = qos_tc_get_indev(dev, f);
	/* Set subif and portid */
	tc2pce_subif_parse(dev, pce_rule, indev);

	tc2pce_eth_proto_parse(dev, f, pce_rule);

	/* Set source and destination MAC addresses*/
	tc2pce_key_eth_addr_parse(dev, f, pce_rule);

	/* Set SLAN_Vid */
	tc2pce_vlan_parse(dev, f, pce_rule, indev);

	/* Set ICMP type */
	tc2pce_icmp_parse(dev, f, pce_rule);

	/* Parse hw_tc */
	tc2pce_set_traffic_class(dev, f->classid, pce_rule);

	return 0;
}
