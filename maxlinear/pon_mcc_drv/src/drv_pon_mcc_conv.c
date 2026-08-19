/******************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/*
 * The set of functions included in this file allows
 * translate the parameters used in the API kernel functions
 * to the parameters needed to invoke low-level switch API functions.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>

#include <pon/pon_mcc_ikm.h>
#include "drv_pon_mcc_conv.h"

#include <net/switch_api/lantiq_gsw.h>
#include <net/datapath_api.h>

/* translate kernel API IP version to GSW IP version */
static GSW_IP_Select_t get_ip_version(enum mcc_l3proto proto)
{
	/*
	 * enum mcc_l3proto            GSW_Ip_Select_t
	 * MCC_L3_PROTO_IPV4   <->     GSW_IP_SELECT_IPV4
	 * MCC_L3_PROTO_IPV6   <->     GSW_IP_SELECT_IPV6
	 */
	if (proto == MCC_L3_PROTO_IPV4)
		return GSW_IP_SELECT_IPV4;
	else
		return GSW_IP_SELECT_IPV6;
}

/* translate GSW IP version to kernel API IP version */
static enum mcc_l3proto get_l3_proto(GSW_IP_Select_t ip_ver)
{
	if (ip_ver == GSW_IP_SELECT_IPV4)
		return MCC_L3_PROTO_IPV4;
	else
		return MCC_L3_PROTO_IPV6;
}

/* translate kernel API source filtering mode to GSW filtering mode */
static GSW_IGMP_MemberMode_t get_gsw_mode(enum src_flt_mode mode)
{
	switch (mode) {
	case SRC_FLT_MODE_INCLUDE:
		return GSW_IGMP_MEMBER_INCLUDE;
	case SRC_FLT_MODE_EXCLUDE:
		return GSW_IGMP_MEMBER_EXCLUDE;
	default:
		return GSW_IGMP_MEMBER_DONT_CARE;
	}
}

/* translate GSW source filtering mode to kernel API filtering mode */
static enum src_flt_mode get_l3_mode(GSW_IGMP_MemberMode_t mode)
{
	switch (mode) {
	case GSW_IGMP_MEMBER_INCLUDE:
		return SRC_FLT_MODE_INCLUDE;
	case GSW_IGMP_MEMBER_EXCLUDE:
		return SRC_FLT_MODE_EXCLUDE;
	default:
		return SRC_FLT_MODE_NA;
	}
}

/* translate kernel API IP address to GSW IP address */
static void l3_addr_to_gsw_ip(const union mcc_l3addr *l3addr,
			      enum mcc_l3proto l3proto, GSW_IP_t *gsw_ip)
{
	u8 i;

	if (l3proto == MCC_L3_PROTO_IPV6) {
		/* endianness conversion required for IPv6 addresses
		 * because IPv6 addresses inside structure GSW_IP_t
		 * are not defined as byte array but as 16 bit values.
		 */
		for (i = 0; i < 8; i++) {
			gsw_ip->nIPv6[i] =
				htons(l3addr->addr6.in6_u.u6_addr16[i]);
		}
	} else {
		/* endianness conversion required for IPv4 addresses
		 * because IPv4 addresses inside structure GSW_IP_t
		 * are not defined as byte array but as 32 bit value.
		 */
		gsw_ip->nIPv4 = htonl(l3addr->addr.s_addr);
	}
}

static void gsw_ip_to_l3_addr(const GSW_IP_t *gsw_ip,
			      GSW_IP_Select_t e_ip_version,
			      union mcc_l3addr *l3addr)
{
	u8 i;

	if (e_ip_version == GSW_IP_SELECT_IPV6) {
		/* endianness conversion required for IPv6 addresses
		 * because IPv6 addresses inside structure GSW_IP_t
		 * are not defined as byte array but as 16 bit values.
		 */
		for (i = 0; i < 8; i++) {
			l3addr->addr6.in6_u.u6_addr16[i] =
				htons(gsw_ip->nIPv6[i]);
		}
	} else {
		/* endianness conversion required for IPv4 addresses
		 * because IPv4 addresses inside structure GSW_IP_t
		 * are not defined as byte array but as 32 bit value.
		 */
		l3addr->addr.s_addr = htonl(gsw_ip->nIPv4);
	}
}

int l3tbl_key_to_gsw_entry(const struct mcc_drv_l3tbl_key *key,
			   GSW_multicastTable_t *entry)
{
	struct net_device *br_port;
	struct net_device *bridge = NULL;
	dp_subif_t s_subif;
	u32 flags = 0;
	int dp_instance_id = 0;
	int ret;

	br_port = dev_get_by_index(&init_net, key->ifindex);
	if (!br_port) {
		pr_err("%s: can't get net_device using ifindex=%u\n",
		       __func__, key->ifindex);
		return -ENODEV;
	}

	/* get subif information from net device */
	ret = dp_get_netif_subifid(br_port, NULL, NULL, NULL, &s_subif,
				   flags);
	if (ret != DP_SUCCESS) {
		pr_err("%s: function returned %d net_device %s\n",
		       __func__, ret, br_port->name);
		dev_put(br_port);
		return -EINVAL;
	}

	/* get master (bridge) net device */
	bridge = netdev_master_upper_dev_get(br_port);
	if (!bridge) {
		pr_err("%s: can't get master upper net device for %s\n",
		       __func__, br_port->name);
		dev_put(br_port);
		return -EINVAL;
	}

	/* get FID (bridge Id)*/
	ret = dp_get_fid_by_brname(bridge, &dp_instance_id);
	if (ret < 0) {
		pr_err("%s: dp_get_fid_by_brname returned FID: %d bridge: %s\n",
		       __func__, ret, bridge->name);
		dev_put(br_port);
		return -ENOENT;
	}
	entry->nFID = ret;

	pr_debug("%s: %s {portid: %d, subif: %d} of bridge %s (FID %d):\n",
		 __func__, br_port->name, s_subif.port_id, s_subif.subif,
		 bridge->name, entry->nFID);

	if (key->proto == MCC_L3_PROTO_IPV6)
		pr_debug("%s: proto: IPv%d, Gda: %pI6, Gsa: %pI6, flt_mode: %d, valid: %d\n",
			 __func__, 6, &key->grp.addr6,
			 &key->src.addr6, key->flt_mode, key->valid);
	else
		pr_debug("%s: proto: IPv%d, Gda: %pI4, Gsa: %pI4, flt_mode: %d, valid: %d\n",
			 __func__, 4, &key->grp.addr.s_addr,
			 &key->src.addr.s_addr, key->flt_mode, key->valid);

	/* decrement ref counter incremented by dev_get_by_index */
	dev_put(br_port);

	/* Datapath Port Id corresponds to bridge port Id.
	 * On GSWIP 3.1 nPortId should contain the bridge port ID,
	 * on older GSWIP versions it should contain the MAC port ID.
	 */
	entry->nPortId = s_subif.bport;

	/* Sub-interface Id - in our case uses only 15 bits*/
	entry->nSubIfId = (u16)s_subif.subif;

	entry->eIPVersion = get_ip_version(key->proto);

	l3_addr_to_gsw_ip(&key->grp, key->proto, &entry->uIP_Gda);

	l3_addr_to_gsw_ip(&key->src, key->proto, &entry->uIP_Gsa);

	entry->eModeMember = get_gsw_mode(key->flt_mode);

	return 0;
}

void l3tbl_key_to_gsw_entry_read(const struct mcc_drv_l3tbl_key *key,
				 bool initial,
				 GSW_multicastTableRead_t *entry_read)
{
	entry_read->bInitial = initial;
}

void gsw_entry_read_to_l3tbl_key(const GSW_multicastTableRead_t *entry_read,
				 struct mcc_drv_l3tbl_key *key, bool *last,
				 void *portmap)
{
	u16 i, j;
	u16 nportmap_len;
	i16 portid = -1;
	i16 portid_first = -1;
	u16 portid_cnt = 0;

	*last = entry_read->bLast;

	/* entry is valid if GSW_multicastTableRead_t.nPortId has MSB set */
	if (!entry_read->nPortId) {
		key->valid = false;
		return;
	}

	key->valid = true;

	/* u16 nPortMap[16] - each bit represents one port id
	 * e.g. nPortMap[0] represents ports id(s) from 0 to 15
	 *      nPortmap[1] represents ports id(s) from 16 to 31
	 */
	nportmap_len = sizeof(entry_read->nPortMap) /
			sizeof(entry_read->nPortMap[0]);

	/* The  switch API allows one multicast table entry to contain
	 * from 1 to 128 ports, but our API can only write and read one port.
	 * However, the reading must take account of the potential
	 * for more than one port
	 */
	for (i = 0; i < nportmap_len; i++) {
		for (j = 0; j < 8 * sizeof(entry_read->nPortMap[0]); j++) {
			if (entry_read->nPortMap[i] & (1 << j)) {
				portid = i * 8 * sizeof(entry_read->nPortMap[0])
						+ j;
				pr_debug("nPortMap[%d] has port id %-3d\n",
					 i, portid);
				/*
				 * note that can be more than one port id
				 * in this case special handling is required
				 */
				set_bit(portid, portmap);
				if (!portid_cnt)
					/* save only first portid to ifindex */
					portid_first = portid;
				portid_cnt++;
			}
		}
	}

	if (portid_cnt > 1)
		pr_debug("this mc table entry has %d port id(s)\n", portid_cnt);

	/* TODO get ifindex from portid, subif, fid
	 * net_dev = dp_fun_yet_unknown(portid, subif,[fid])
	 * key->ifindex = net_dev->ifindex;
	 */
	/* temporary ifindex returns port id */
	key->ifindex = portid_first;
	key->proto = get_l3_proto(entry_read->eIPVersion);

	gsw_ip_to_l3_addr(&entry_read->uIP_Gda, entry_read->eIPVersion,
			  &key->grp);
	gsw_ip_to_l3_addr(&entry_read->uIP_Gsa, entry_read->eIPVersion,
			  &key->src);
	key->flt_mode = get_l3_mode(entry_read->eModeMember);
}
