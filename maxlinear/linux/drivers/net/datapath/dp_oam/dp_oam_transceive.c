// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 ******************************************************************************/

#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <net/addrconf.h>
#include <net/ndisc.h>
#include <net/datapath_api.h>
#include <linux/types.h>
#include <linux/if_ether.h>

#include "dp_oam_transceive.h"

#define CFM_MAID_LENGTH			48
#define CFM_CCM_PDU_LENGTH		75
#define CFM_PORT_STATUS_TLV_LENGTH	4
#define CFM_IF_STATUS_TLV_LENGTH	4
#define CFM_IF_STATUS_TLV_TYPE		4
#define CFM_PORT_STATUS_TLV_TYPE	2
#define CFM_ENDE_TLV_TYPE		0
#define CFM_CCM_MAX_FRAME_LENGTH	(VLAN_ETH_HLEN +\
					 CFM_CCM_PDU_LENGTH +\
					 CFM_PORT_STATUS_TLV_LENGTH +\
					 CFM_IF_STATUS_TLV_LENGTH)
#define CFM_FRAME_PRIO			7
#define CFM_CCM_TLV_OFFSET		70
#define CFM_CCM_PDU_MAID_OFFSET		10
#define CFM_CCM_PDU_MEPID_OFFSET	8
#define CFM_CCM_PDU_SEQNR_OFFSET	4
#define CFM_CCM_PDU_TLV_OFFSET		74
#define CFM_CCM_ITU_RESERVED_SIZE	16

#define ETH_P_CFM		0x8902 /* Connectivity Fault Management */

static bool transceive_loaded;
static u8 transceive_level;
static u8 src_mac[ETH_ALEN];

struct br_cfm_common_hdr {
	__u8 mdlevel_version;
	__u8 opcode;
	__u8 flags;
	__u8 tlv_offset;
};

struct tx_pdata {
	bool rx_ts_f;
};

enum br_cfm_opcodes {
	BR_CFM_OPCODE_CCM = 0x1,
};

/* max number of encapsulated vlan tags */
#define MAX_VLAN_TAGS	3

/* Iterates over the VLAN tags and increases nhoff for each VLAN tag.
 * returns 0 on success and -EINVAL in case of an parsing error.
 */
static int dp_oam_test_vlan_iter(struct sk_buff *skb, __be16 *proto,
				 int *nhoff)
{
	const struct vlan_hdr *vlan = NULL;
	struct vlan_hdr _vlan;
	int i;

	for (i = 0; i < MAX_VLAN_TAGS; i++) {
		vlan = skb_header_pointer(skb, *nhoff, sizeof(_vlan), &_vlan);
		if (!vlan)
			return -EINVAL;

		*proto = vlan->h_vlan_encapsulated_proto;
		*nhoff += sizeof(*vlan);
		if (!eth_type_vlan(*proto))
			break;
	}

	return 0;
}

static void frame_add_ethhdr(struct sk_buff *iskb, struct sk_buff *oskb)
{
	struct ethhdr *ehdr;
	struct vlan_hdr *veth;
	u8 mac_addr[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x30 };

	/* See ITU-T G.8013/Y.1731 10.1 Multicast destination addresses */
	mac_addr[ETH_ALEN - 1] += transceive_level;
	/* Ethernet header */
	ehdr = (struct ethhdr *)skb_put(oskb, sizeof(*ehdr));
	ether_addr_copy(ehdr->h_dest, mac_addr);
	ether_addr_copy(ehdr->h_source, src_mac);
	ehdr->h_proto = htons(ETH_P_CFM);
	__vlan_hwaccel_put_tag(oskb, iskb->vlan_proto, iskb->vlan_tci);

	if (eth_type_vlan(iskb->protocol)) {
		ehdr->h_proto = iskb->protocol;
		veth = (struct vlan_hdr *)skb_put(oskb, sizeof(*veth));
		skb_push(iskb, ETH_HLEN);
		__vlan_get_tag(iskb, &veth->h_vlan_TCI);
		skb_pull(iskb, ETH_HLEN);
		veth->h_vlan_encapsulated_proto = htons(ETH_P_CFM);
	}
}

static unsigned int cfm_frame_tx(struct sk_buff *skb)
{
	struct dma_tx_desc_0 *dw0;
	struct dma_tx_desc_1 *dw1;
	dp_subif_t subif = {0};
	int ret;

	ret = dp_get_netif_subifid(skb->dev, NULL, NULL, NULL, &subif, 0);
	if (ret != DP_SUCCESS) {
		netdev_err(skb->dev, "%s: can not get subif: %d\n", __func__,
			   ret);
		return -EIO;
	}

	skb = __vlan_hwaccel_push_inside(skb);
	if (WARN_ON(!skb)) {
		dev_kfree_skb_any(skb);
		return -ENOMEM;
	}

	dw0 = (struct dma_tx_desc_0 *)&skb->DW0;
	dw0->field.dest_sub_if_id = subif.subif;
	dw1 = (struct dma_tx_desc_1 *)&skb->DW1;
	dw1->field.ep = subif.port_id;

	print_hex_dump_debug("skb: ", DUMP_PREFIX_OFFSET, 16, 1,
			     skb->data, skb->len, false);
	ret = dp_xmit(skb->dev, &subif, skb, skb->len, DP_TX_INSERT);
	if (ret) {
		/* DPM can return busy on LGM then skb needs to be freed */
		if (ret == DP_TX_FN_BUSY)
			dev_kfree_skb_any(skb);
		netdev_err(skb->dev, "%s: insert fail\n", __func__);
		return -EIO;
	}

	return 0;
}

#define CFM_OPCODE_LMR 0x2A
#define CFM_TLV_LMR 0xC
#define CFM_CNT_SIZE 4
#define CFM_CNT_TX 4
#define CFM_CNT_RX 8
static struct sk_buff *lmr_frame_build(struct sk_buff *i_skb)
{
	struct br_cfm_common_hdr *chdr;
	struct sk_buff *skb;
	u8 *e_tlv;
	unsigned char counters[12];
	unsigned char *ptr = counters;
	u32 offset = eth_type_vlan(i_skb->protocol) ? VLAN_HLEN : 0;

	if (!skb_vlan_tagged(i_skb))
		return NULL;

	skb = dev_alloc_skb(CFM_CCM_MAX_FRAME_LENGTH);
	if (!skb)
		return NULL;

	skb->dev = i_skb->dev;
	skb->protocol = htons(i_skb->protocol);
	skb->priority = CFM_FRAME_PRIO;
	frame_add_ethhdr(i_skb, skb);
	/* Common CFM Header */
	chdr = (struct br_cfm_common_hdr *)skb_put(skb, sizeof(*chdr));
	chdr->mdlevel_version = (transceive_level << 5) & 0xFF;
	chdr->opcode = CFM_OPCODE_LMR;
	chdr->flags = 0;
	chdr->tlv_offset = CFM_TLV_LMR;

	ptr = skb_put(skb, sizeof(counters));
	memset(ptr, 0, sizeof(counters));
	/* TxFCf */
	memcpy(&ptr[0], i_skb->data + offset + CFM_CNT_TX, CFM_CNT_SIZE);
	/* RxFCf */
	memcpy(&ptr[4], i_skb->data + offset + CFM_CNT_RX, CFM_CNT_SIZE);

	/* End TLV */
	e_tlv = skb_put(skb, sizeof(*e_tlv));
	*e_tlv = CFM_ENDE_TLV_TYPE;

	if (skb_put_padto(skb, ETH_ZLEN)) {
		kfree_skb(skb);
		return NULL;
	}

	return skb;
}

#define CFM_OPCODE_DMR 0x2E
#define CFM_TLV_DMR 0x20
#define CFM_TS_SIZE 8
#define CFM_TS_TX 4
#define CFM_TS_RX 8
static struct sk_buff *dmr_frame_build(struct sk_buff *i_skb,
				       struct tx_pdata *priv)
{
	struct br_cfm_common_hdr *chdr;
	struct ethhdr *ehdr;
	struct sk_buff *skb;
	u8 *e_tlv;
	u8 src_mac_addr[ETH_ALEN] = { 0, 0x11, 0x22, 0x33, 0x44, 0x55 };
	u8 dst_mac_addr[ETH_ALEN] = { 0, 0x12, 0x23, 0x34, 0x45, 0x56 };
	unsigned char tstamps[16];
	unsigned char *ptr = tstamps;

	if (!skb_vlan_tagged(i_skb))
		return NULL;

	skb = dev_alloc_skb(CFM_CCM_MAX_FRAME_LENGTH);
	if (!skb)
		return NULL;

	skb->dev = i_skb->dev;
	skb->protocol = htons(i_skb->protocol);
	skb->priority = CFM_FRAME_PRIO;
	__vlan_hwaccel_put_tag(skb, i_skb->vlan_proto, i_skb->vlan_tci);

	/* Ethernet header */
	ehdr = (struct ethhdr *)skb_put(skb, sizeof(*ehdr));
	ether_addr_copy(ehdr->h_dest, dst_mac_addr);
	ether_addr_copy(ehdr->h_source, src_mac_addr);
	ehdr->h_proto = htons(ETH_P_CFM);

	/* Common CFM Header */
	chdr = (struct br_cfm_common_hdr *)skb_put(skb, sizeof(*chdr));
	chdr->mdlevel_version = (transceive_level << 5) & 0xFF;
	chdr->opcode = CFM_OPCODE_DMR;
	chdr->flags = 0;
	chdr->tlv_offset = CFM_TLV_DMR;

	ptr = skb_put(skb, sizeof(tstamps));
	memset(ptr, 0, sizeof(tstamps));
	/* TxTimeStamp */
	memcpy(&ptr[0], i_skb->data + CFM_TS_TX, CFM_TS_SIZE);
	if (!priv->rx_ts_f) {
		/* RxTimeStampf - appended at the end of the packet */
		print_hex_dump_debug("ts: ", DUMP_PREFIX_OFFSET, 16, 1,
				     i_skb->data + i_skb->len - DP_TS_HDRLEN,
				     DP_TS_HDRLEN, false);
		memcpy(&ptr[8], i_skb->data + i_skb->len - CFM_TS_RX,
		       CFM_TS_SIZE);
	} else {
		print_hex_dump_debug("ts: ", DUMP_PREFIX_OFFSET, 16, 1,
				     i_skb->data + CFM_TS_TX + CFM_TS_RX,
				     DP_TS_HDRLEN, false);
		memcpy(&ptr[8], i_skb->data + CFM_TS_TX + CFM_TS_RX,
		       CFM_TS_SIZE);
	}

	/* End TLV */
	e_tlv = skb_put(skb, sizeof(*e_tlv));
	*e_tlv = CFM_ENDE_TLV_TYPE;

	if (skb_put_padto(skb, ETH_ZLEN)) {
		kfree_skb(skb);
		return NULL;
	}

	return skb;
}

#define CFM_TLV_DMM 0x20
#define CFM_TS_SIZE 8
#define CFM_TS_TX 4
#define CFM_TS_RX 8
static struct sk_buff *dmm_frame_build(struct tx_data *data)
{
	struct br_cfm_common_hdr *chdr;
	struct net_device *dev;
	struct ethhdr *ehdr;
	struct sk_buff *skb;
	u8 src_mac_addr[ETH_ALEN] = { 0, 0x11, 0x22, 0x33, 0x44, 0x55 };
	u8 dst_mac_addr[ETH_ALEN] = { 0, 0x12, 0x23, 0x34, 0x45, 0x56 };
	u8 *e_tlv, *ptr;

	dev = dev_get_by_index(&init_net, data->ifi);
	if (!dev) {
		pr_err("%s: dev_get_by_index failed\n",
		       __func__);
		return NULL;
	}

	skb = dev_alloc_skb(CFM_CCM_MAX_FRAME_LENGTH);
	if (!skb)
		return NULL;

	skb->dev = dev;
	skb->protocol = htons(ETH_P_CFM);
	skb->priority = CFM_FRAME_PRIO;
	__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), htons(data->vid));

	/* Ethernet header */
	ehdr = (struct ethhdr *)skb_put(skb, sizeof(*ehdr));
	ether_addr_copy(ehdr->h_dest, dst_mac_addr);
	ether_addr_copy(ehdr->h_source, src_mac_addr);
	ehdr->h_proto = htons(ETH_P_CFM);

	/* Common CFM Header */
	chdr = (struct br_cfm_common_hdr *)skb_put(skb, sizeof(*chdr));
	chdr->mdlevel_version = (data->level << 5) & 0xFF;
	chdr->opcode = data->opcode;
	chdr->flags = 0;
	chdr->tlv_offset = CFM_TLV_DMM;

	ptr = skb_put(skb, CFM_TS_SIZE * 4);
	memset(ptr, 0, CFM_TS_SIZE * 4);
	/* End TLV */
	e_tlv = skb_put(skb, sizeof(*e_tlv));
	*e_tlv = CFM_ENDE_TLV_TYPE;
	if (skb_put_padto(skb, ETH_ZLEN)) {
		kfree_skb(skb);
		return NULL;
	}

	return skb;
}

#define CFM_TLV_1DM 0x10
#define CFM_TS_SIZE 8
#define CFM_TS_TX 4
#define CFM_TS_RX 8
static struct sk_buff *one_dm_frame_build(struct tx_data *data)
{
	struct br_cfm_common_hdr *chdr;
	struct net_device *dev;
	struct ethhdr *ehdr;
	struct sk_buff *skb;
	u8 src_mac_addr[ETH_ALEN] = { 0, 0x11, 0x22, 0x33, 0x44, 0x55 };
	u8 dst_mac_addr[ETH_ALEN] = { 0, 0x12, 0x23, 0x34, 0x45, 0x56 };
	u8 *e_tlv, *ptr;

	dev = dev_get_by_index(&init_net, data->ifi);
	if (!dev) {
		pr_err("%s: dev_get_by_index failed\n",
		       __func__);
		return NULL;
	}

	skb = dev_alloc_skb(CFM_CCM_MAX_FRAME_LENGTH);
	if (!skb)
		return NULL;

	skb->dev = dev;
	skb->protocol = htons(ETH_P_CFM);
	skb->priority = CFM_FRAME_PRIO;
	__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), htons(data->vid));

	/* Ethernet header */
	ehdr = (struct ethhdr *)skb_put(skb, sizeof(*ehdr));
	ether_addr_copy(ehdr->h_dest, dst_mac_addr);
	ether_addr_copy(ehdr->h_source, src_mac_addr);
	ehdr->h_proto = htons(ETH_P_CFM);

	/* Common CFM Header */
	chdr = (struct br_cfm_common_hdr *)skb_put(skb, sizeof(*chdr));
	chdr->mdlevel_version = (data->level << 5) & 0xFF;
	chdr->opcode = data->opcode;
	chdr->flags = 0;
	chdr->tlv_offset = CFM_TLV_1DM;

	ptr = skb_put(skb, CFM_TS_SIZE * 4);
	memset(ptr, 0, CFM_TS_SIZE * 4);
	/* End TLV */
	e_tlv = skb_put(skb, sizeof(*e_tlv));
	*e_tlv = CFM_ENDE_TLV_TYPE;
	if (skb_put_padto(skb, ETH_ZLEN)) {
		kfree_skb(skb);
		return NULL;
	}

	return skb;
}

static struct sk_buff *tx_frame_build(struct sk_buff *i_skb, void *priv)
{
	struct sk_buff *skb = NULL;

	pr_debug("Building cfm fame: %#x %#x\n",
		 i_skb->data[0], i_skb->data[1]);
	switch (i_skb->data[1]) {
	/* OAM LMM */
	case 0x2b:
		skb = lmr_frame_build(i_skb);
		break;
	/* OAM DMM */
	case 0x2f:
		skb = dmr_frame_build(i_skb, priv);
		break;
	default:
		pr_err("unknown oam packet opcode\n");
		break;
	}

	return skb;
}

static unsigned int dp_oam_test_hook(void *priv, struct sk_buff *skb,
				     const struct nf_hook_state *state)
{
	__be16 proto = skb->protocol;
	int nhoff = skb_network_offset(skb);
	struct sk_buff *i_skb = NULL;
	int err;

	if (eth_type_vlan(proto)) {
		err = dp_oam_test_vlan_iter(skb, &proto, &nhoff);
		if (err) {
			pr_debug("VLAN header broken, accepted");
			return NF_ACCEPT;
		}
	}

	switch (proto) {
	case htons(ETH_P_CFM):
		i_skb = tx_frame_build(skb, priv);
		if (i_skb) {
			skb_reset_network_header(i_skb);
			cfm_frame_tx(i_skb);
		}
		return NF_DROP;
	default:
		return NF_ACCEPT;
	}
}

static struct nf_hook_ops dp_oam_test_ops = {
	.hook = dp_oam_test_hook,
	.hooknum = NF_BR_PRE_ROUTING,
	.pf = NFPROTO_BRIDGE,
	.priority = NF_BR_PRI_FILTER_BRIDGED,
};

void dp_oam_transceive_init(void)
{
	transceive_loaded = false;
}

int dp_oam_tx(struct tx_data *data)
{
	struct sk_buff *skb = NULL;

	switch (data->opcode) {
	case 0x2f:
		skb = dmm_frame_build(data);
		break;
	case 0x2d:
		skb = one_dm_frame_build(data);
		break;
	default:
		pr_debug("%s: opcode not supported\n", __func__);
		break;
	}

	if (skb) {
		skb_reset_network_header(skb);
		cfm_frame_tx(skb);
	}

	return 0;
}

static struct tx_pdata tx_data = {
	.rx_ts_f = false,
};

int dp_oam_transceive_load(u8 level, bool rx_ts_f)
{
	int ret;

	if (transceive_loaded)
		return 0;

	transceive_loaded = true;
	transceive_level = level;
	tx_data.rx_ts_f = rx_ts_f;
	dp_oam_test_ops.priv = &tx_data;
	eth_random_addr(src_mac);
	ret = nf_register_net_hook(&init_net, &dp_oam_test_ops);
	return ret;
}

void dp_oam_transceive_unload(void)
{
	if (!transceive_loaded)
		return;

	transceive_loaded = false;
	nf_unregister_net_hook(&init_net, &dp_oam_test_ops);
}
