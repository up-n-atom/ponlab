// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <asm/dma-mapping.h>
#include <dt-bindings/net/mxl,lgm-cqm.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/inet.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <net/datapath_api.h>
#include <uapi/linux/icmp.h>
#include <uapi/linux/ip.h>

#define POLICY_ID_MASK		GENMASK_ULL(53, 46)
#define POLICY_ID_OFF		46

#define MAX_MTU_SIZE		0x600

enum dp_client {
	DP_WIFI,
	DP_DSL,
};

enum {
	RXIN,
	TXIN,
	RXOUT,
	TXOUT,
};

enum {
	SOC_DEVICE,
	DEV_DEVICE,
	TT_NUM
};

#define UMT_CNT_NUM  (16 * 16)
static const unsigned int const_umt_interval = 1000;  /* us */

struct dma_desc {
	u64 desc0;
	u64 desc1;
} __packed;

struct arphdrv4 {
	struct arphdr hdr;
	unsigned char ar_sha[ETH_ALEN];
	unsigned char ar_sip[4];
	unsigned char ar_tha[ETH_ALEN];
	unsigned char ar_tip[4];
} __packed;

struct client_vdev {
	struct list_head	 node;
	char			 mac[6];
	__be32			 ipaddr;
	u16			 ipid;
};

struct dp_device {
	unsigned int __iomem	 *soc_umt_cnt_addr;
	dma_addr_t		 soc_umt_cnt_paddr;
	unsigned int		 dev_umt_cnt_addr[UMT_CNT_NUM];
	struct dma_desc          *dev_txin;
	u64			 *dev_txout;
	u64			 *dev_rxin;
	struct dma_desc          *dev_rxout;
	unsigned int             buflen[TT_NUM][UMT_CNT_NUM]; /* desc num */
	unsigned int             fidx[UMT_CNT_NUM]; /* fill idx */
	unsigned int             gidx[UMT_CNT_NUM]; /* guard idx */
	unsigned int             tidx[UMT_CNT_NUM]; /* Transfer idx */
	struct dma_desc __iomem  *soc_txin;
	u64  __iomem             *soc_txout;
	u64  __iomem             *soc_rxin;
	struct dma_desc __iomem  *soc_rxout;
	dma_addr_t		 soc_paddr[UMT_CNT_NUM];
	unsigned int             umt_interval;
	unsigned int		 rx_policy;
	struct list_head	 head;
	/* todo : function call ptr to be added */
};

struct test_device {
	struct list_head         node;
	struct net_device        *netdev;
	struct device            *dev;
	int                      dpid;
	int			 dev_port; /* same name in dp function */
	char			 name[32];
	dp_subif_t		 subif_id;
	struct dp_dev_data	 dp_data;
	struct dp_device	 dp_dev;
	struct work_struct	 dev_work;
	struct dp_dc_res         dc_res;
};

struct umttest_priv {
	struct platform_device	*pdev;
	struct dentry		*debugfs;
	struct list_head	head;
	struct timer_list	timer;
	struct workqueue_struct	*wk_queue;
	struct test_device	*tdev;
};

static const u8 mac_addr[] = {0x0, 0x55, 0x66, 0x77, 0x88, 0x99};

static int interface = 1;		/* Legacy UMT interface */
static int dst_dw_offset_bitmap = 0xFFFF; /* Destination address as DW offset bitmap in memory */
static int dst_addr_cnt = 4;		/* UMT_MSG_SELFCNT */;
static int msg_mode;			/* UMT_MSG_SELFCNT */;
static int cnt_mode = 1;		/* UMT_CNT_ACC */
static int rx_msg_mode = 2;		/* UMT_4MSG_MODE */
static int fflag = 0x4;			/* UMT_NOT_SND_ZERO_CNT */
static int umt_port_0;			/* No */
static int manual_ops;			/* Update counters automatically */
static int cnt_idx_in_buf[UMT_CNT_NUM];

static void umttest_dump_info(struct test_device *tdev);

static int umttest_open(struct net_device *ndev)
{
	netif_carrier_on(ndev);
	netif_wake_queue(ndev);

	return 0;
}

static int umttest_stop(struct net_device *ndev)
{
	netif_stop_queue(ndev);
	netif_carrier_off(ndev);

	return 0;
}

static netdev_tx_t umttest_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct test_device *tdev = netdev_priv(ndev);

	/* Call the Datapath Library's TX function */
	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = tdev->subif_id.port_id;
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id = tdev->subif_id.subif;

	ndev->stats.tx_packets++;
	ndev->stats.tx_bytes += skb->len;

	return dp_xmit(ndev, &tdev->subif_id, skb, skb->len, DP_TX_NEWRET);
}

static const struct net_device_ops umttest_netdev_ops = {
	.ndo_open	= umttest_open,
	.ndo_stop	= umttest_stop,
	.ndo_start_xmit = umttest_start_xmit,
};

/* Find the nth set bit in a value
 * Parameter n should between 1 and 32.
 * Return: The bit position of the found nth set bit. -1 if not found.
 */
static int find_set_bit(unsigned int value, int n)
{
	int i, cnt = 0;

	for (i = 0; i < 32; i++) {
		if (value & (1 << i)) {
			cnt++;
			if (cnt == n)
				return i;
		}
	}

	return -1;
}

static unsigned int idx_add(struct dp_device *dp_dev, int idx, int dir, int device, int cnt)
{
	return (idx + cnt) % dp_dev->buflen[device][dir];
}

static unsigned int idx_inc(struct dp_device *dp_dev, int idx, int dir, int device)
{
	return idx_add(dp_dev, idx, dir, device, 1);
}

static unsigned int idx_sub(unsigned int end, unsigned int off, unsigned int q_len)
{
	long long idx;

	idx = (long long)end - (long long)off;
	if (idx >= 0)
		return (unsigned int)idx;
	else
		return (unsigned int)(idx + q_len);
}

static void umt_cnt_inc(struct dp_device *dp_dev, int dir, unsigned int cnt)
{
	dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[dir]] += cnt;
}

static void desc_to_soc(void *dst, void *src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		writeq((*(u64 *)(src + i * 8)), (dst + i * 8));
}

static void desc_from_soc(void *dst, void *src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		*(u64 *)(dst + i * 8) = readq(src + i * 8);
}

/**
 * endian: little edian: 1, big endian: 0
 *
 */
static void __maybe_unused dump_data(void *data, int size, int endian)
{
	int i;

	pr_debug("==========dump data start =============\n");
	for (i = 0; i < size; i++) {
		if (endian)
			pr_debug("[%d]:               0x%08x\n",
				 i, (*(unsigned int *)(data + i * 4)));
		else
			pr_debug("[%d]:               0x%08x\n",
				 i, htonl(*(unsigned int *)(data + i * 4)));
	}

	pr_debug("==========dump data end   =============\n");
}

static void desc_from_soc_fetch_remain(struct dp_device *dp_dev,
				       int dir, unsigned int idx)
{
	unsigned int size = dp_dev->buflen[SOC_DEVICE][dir];
	unsigned int soc_idx = idx % size;
	void *dst, *src;
	int i;

	if (dir != RXIN || soc_idx == 0 || manual_ops)
		return;

	dst = dp_dev->dev_rxin;
	src = dp_dev->soc_rxin;
	for (i = soc_idx; i < size; i++)
		*(u64 *)(dst + i * 8) = readq(src + i * 8);
}

static void process_fill_desc(struct dp_device *dp_dev, int idx, int dir, int cnt)
{
	int i;
	unsigned int soc_idx, dev_idx;

	dev_idx = idx;
	soc_idx = dev_idx % dp_dev->buflen[SOC_DEVICE][dir];

	for (i = 0; i < cnt; i++) {
		if (dir == RXOUT)
			desc_to_soc(&dp_dev->soc_rxout[soc_idx], &dp_dev->dev_rxout[dev_idx], 2);
		else if (dir == TXOUT)
			desc_to_soc(&dp_dev->soc_txout[soc_idx], &dp_dev->dev_txout[dev_idx], 1);
		else if (dir == RXIN)
			desc_from_soc(&dp_dev->dev_rxin[dev_idx], &dp_dev->soc_rxin[soc_idx], 1);
		else if (dir == TXIN)
			desc_from_soc(&dp_dev->dev_txin[dev_idx], &dp_dev->soc_txin[soc_idx], 2);

		soc_idx = idx_inc(dp_dev, soc_idx, dir, SOC_DEVICE);
		dev_idx = idx_inc(dp_dev, dev_idx, dir, DEV_DEVICE);
	}

	umt_cnt_inc(dp_dev, dir, cnt);
}

/**
 * TXIN:
 *   Add deescriptor:  move fill idx
 *   Take out descriptor: move transfer idx, guard idx
 */

static void process_fill_txin_desc(struct dp_device *dp_dev, int cnt)
{
	unsigned int idx = dp_dev->fidx[TXIN];

	process_fill_desc(dp_dev, idx, TXIN, cnt);
	dp_dev->fidx[TXIN] = idx_add(dp_dev, idx, TXIN, DEV_DEVICE, cnt);
}

static void process_fill_rxin_desc(struct dp_device *dp_dev, int cnt)
{
	unsigned int idx = dp_dev->fidx[RXIN];

	process_fill_desc(dp_dev, idx, RXIN, cnt);
	dp_dev->fidx[RXIN] = idx_add(dp_dev, idx, RXIN, DEV_DEVICE, cnt);
}

static phys_addr_t consume_one_rxin(struct dp_device *dp_dev, unsigned int *pool_id)
{
	phys_addr_t paddr;
	unsigned int idx, id;

	if (dp_dev->fidx[RXIN] == dp_dev->tidx[RXIN])
		return -1;

	idx = dp_dev->tidx[RXIN];
	paddr = ((dp_dev->dev_rxin[idx] & GENMASK_ULL(58, 55)) >> 23) |
		(dp_dev->dev_rxin[idx] & GENMASK_ULL(31, 0));
	id = (unsigned int)((dp_dev->dev_rxin[idx] & GENMASK_ULL(35, 32)) >> 32);

	dp_dev->tidx[RXIN] = idx_inc(dp_dev, idx, RXIN, DEV_DEVICE);
	dp_dev->gidx[RXIN] = idx_inc(dp_dev, dp_dev->gidx[RXIN], RXIN, DEV_DEVICE);
	*pool_id = id;

	return paddr;
}

static void form_dc_desc(u32 *dc_desc, phys_addr_t paddr, unsigned int size,
			 unsigned int pool_id, unsigned int policy_id, unsigned int dpid)
{
	u32 desc;

	*dc_desc = 0;  /* DC desc DW0 is 0 as we don't support VAP */

	/* DC desc DW1 (3:0) is physical address BIT 35:32 */
	dc_desc[1] = (paddr >> 32) & 0xF;
	/* port is the dp id from dp_port_alloc */
	dc_desc[1] |= (dpid << 16) & GENMASK(23, 16);

	dc_desc[2] = (u32)paddr;

	/* Data Length Bit 13:0 */
	desc = size & GENMASK(13, 0);
	/* pool policy 23:16 */
	desc |= (policy_id << 16) & GENMASK(23, 16);
	/* source pool */
	desc |= (pool_id << 24) & GENMASK(27, 24);
	/* own bit: 31 = 0,  sop: 29, eop: 28 = 1 */
	desc |= GENMASK(29, 28);

	dc_desc[3] = desc;
}

static void process_sendto_rxout(struct test_device *tdev, int txin_idx,
				 phys_addr_t paddr, unsigned int size, unsigned int pool_id)
{
	unsigned int rxout_idx;
	struct dp_device *dp_dev = &tdev->dp_dev;

	//sanity check if have enough space in rxout
	if (dp_dev->gidx[RXOUT] == dp_dev->fidx[RXOUT]) {
		pr_err("No enough space in RXOUT\n");
		return;
	}

	rxout_idx = dp_dev->fidx[RXOUT];
	form_dc_desc((u32 *)&dp_dev->dev_rxout[rxout_idx], paddr, size, pool_id,
		     dp_dev->rx_policy, tdev->dpid);
	dp_dev->fidx[RXOUT] = idx_inc(dp_dev, rxout_idx, RXOUT, DEV_DEVICE);

	process_fill_desc(dp_dev, rxout_idx, RXOUT, 1);
}

static void __maybe_unused process_txin_to_txout(struct test_device *tdev,
						 phys_addr_t addr,
						 unsigned int tx_poolid,
						 unsigned int tx_policyid)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int txout_idx;
	u64 desc = 0;

	txout_idx = dp_dev->fidx[TXOUT];
	desc = (addr & GENMASK_ULL(35, 32)) << 23;
	desc |= addr & GENMASK_ULL(31, 0);
	desc |= ((u64)tx_poolid & 0xF) << 32;
	desc |= ((u64)tx_policyid & 0xFF) << 46;

	dp_dev->dev_txout[txout_idx] = desc;

	dp_dev->fidx[TXOUT] = idx_inc(dp_dev, txout_idx, TXOUT, DEV_DEVICE);
	process_fill_desc(dp_dev, txout_idx, TXOUT, 1);
}

/* support DMA format desc */
static phys_addr_t ret_phy_addr_from_desc(void *descriptor)
{
	phys_addr_t paddr, tmp;
	u32 *desc = descriptor;

	tmp = desc[1] & GENMASK(3, 0);
	paddr = (tmp << 32) | desc[2];

	return paddr;
}

static void fill_ethhdr(void *buf, unsigned char *dst_mac, unsigned char *src_mac, u16 proto)
{
	struct ethhdr *eth = buf;

	memcpy(eth->h_dest, dst_mac, ETH_ALEN);
	memcpy(eth->h_source, src_mac, ETH_ALEN);
	eth->h_proto = htons(proto);
}

static void print_mac(unsigned char *mac, char *s)
{
	pr_debug("=== %s:  0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x====\n",
		 s,
		 mac[0], mac[1], mac[2],
		 mac[3], mac[4], mac[5]);
}

static void print_ip(unsigned int ipaddr, char *s)
{
	pr_debug("====%s: %u.%u.%u.%u ====\n",
		 s,
		 ipaddr >> 24, (ipaddr >> 16) & 0xff,
		 (ipaddr >> 8) & 0xff, ipaddr & 0xff);
}

static __be32 form_ipv4(unsigned char *ip)
{
	__be32 ipaddr = 0;

	ipaddr = (ip[3] << 24)  | (ip[2] << 16)  | (ip[1] << 8)  | ip[0];

	return ipaddr;
}

/**
 * record is 1 only in arp packet.
 * other case must be 0
 */
static int get_client_info(struct dp_device *dp_dev, int record,
			   unsigned char *mac, unsigned int ipaddr, u16 *id)
{
	struct client_vdev *cdev;
	struct test_device *tdev;

	list_for_each_entry(cdev, &dp_dev->head, node) {
		if (cdev->ipaddr == ipaddr) {
			print_ip(ipaddr, "match ip");
			print_mac(cdev->mac, "return mac");
			memcpy(mac, cdev->mac, ETH_ALEN);
			if (id)
				*id = cdev->ipid++;
			return 0;
		}
	}

	/* couldn't find a existing device has that same ip, create one*/

	if (!record) {
		pr_err("!!!No mac record in client!!!\n");
		return -ENODEV;
	}

	tdev = container_of(dp_dev, typeof(*tdev), dp_dev);
	cdev = devm_kzalloc(tdev->dev, sizeof(*cdev), GFP_KERNEL);
	if (!cdev)
		return -ENOMEM;

	INIT_LIST_HEAD(&cdev->node);
	cdev->ipaddr = ipaddr;
	cdev->ipid = 0;
	eth_random_addr(cdev->mac);
	memcpy(mac, cdev->mac, ETH_ALEN);
	print_mac(cdev->mac, "record mac");
	print_ip(cdev->ipaddr, "record ip");

	list_add_tail(&cdev->node, &dp_dev->head);

	return 0;
}

/**
 * replay ARP packet based on request IP address.
 * auto generated MAC
 * My IP/MAC is src
 */
static int __maybe_unused handle_arp_reply(struct dp_device *dp_dev, void *buf,
					   phys_addr_t *phy_addr,
					   unsigned int *pkt_sz,
					   unsigned int *pool_id)
{
	struct test_device *tdev = container_of(dp_dev, typeof(*tdev), dp_dev);
	unsigned char dst_mac[6], src_mac[6], dst_ip_addr[4], src_ip_addr[4];
	struct arphdrv4 *arp;
	u32 *crc32, crc, ipaddr;
	phys_addr_t paddr, tmp;
	void *vaddr;
	unsigned int size;

	if (!phy_addr || !pkt_sz)
		return -1;

	arp = buf + ETH_HLEN;
	memcpy(dst_mac, arp->ar_sha, ETH_ALEN);
	memcpy(dst_ip_addr, arp->ar_sip, 4);
	memcpy(src_ip_addr, arp->ar_tip, 4);

	ipaddr = form_ipv4(src_ip_addr);
	if (get_client_info(dp_dev, 1, src_mac, ipaddr, NULL))
		return -1;

	/* Get RXIN data pointer */
	paddr = consume_one_rxin(dp_dev, pool_id);
	if (paddr == (phys_addr_t)-1)
		return -1;

	tmp = paddr & 0xFFFFFFFF;
	vaddr = phys_to_virt(tmp);

	size = ETH_HLEN;
	arp = vaddr + size;
	//Fill MAC address head
	fill_ethhdr(vaddr, dst_mac, src_mac, ETH_P_ARP);
	arp->hdr.ar_hrd = htons(ARPHRD_ETHER);
	arp->hdr.ar_pro = htons(ETH_P_IP);
	arp->hdr.ar_hln = ETH_ALEN;
	arp->hdr.ar_pln = 4;
	arp->hdr.ar_op = htons(ARPOP_REPLY);
	memcpy(arp->ar_sha, src_mac, ETH_ALEN);
	memcpy(arp->ar_tha, dst_mac, ETH_ALEN);
	memcpy(arp->ar_sip, src_ip_addr, 4);
	memcpy(arp->ar_tip, dst_ip_addr, 4);
	size += sizeof(*arp);

	crc32 = vaddr + size;
	*crc32 = 0;
	crc = ether_crc_le(size, vaddr);

	*crc32 = htonl(crc);
	size += sizeof(crc);

	dma_map_single_attrs(tdev->dev, vaddr, size, DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);

	*phy_addr = paddr;
	*pkt_sz = size;

	return 0;
}

static int handle_icmp_reply(struct dp_device *dp_dev, void *buf,
			     phys_addr_t *phy_addr, unsigned int *pkt_sz,
			     unsigned int *pool_id)
{
	struct test_device *tdev = container_of(dp_dev, typeof(*tdev), dp_dev);
	struct iphdr *iph;
	struct icmphdr *icmph;
	unsigned char dst_mac[ETH_ALEN], src_mac[ETH_ALEN], mac[ETH_ALEN];
	__be32 srcip, dstip;
	u16 tot_len, id, icmp_id, icmp_seq;
	unsigned int size, data_len;
	u32 *crc32;

	phys_addr_t paddr, tmp;
	void *vaddr;

	iph = buf + ETH_HLEN;
	tot_len = iph->tot_len;
	icmph = (void *)iph + sizeof(*iph);

	//dump_data(buf, 4, 0);
	if (icmph->type != ICMP_ECHO) {
		pr_err("icmp type is not echo: %u", icmph->type);
		return -1;
	}

	memcpy(src_mac, buf, ETH_ALEN);
	memcpy(dst_mac, buf + ETH_ALEN, ETH_ALEN);
	srcip = iph->daddr; /* swap src/dest address */
	dstip = iph->saddr;
	icmp_id  = icmph->un.echo.id;
	icmp_seq = icmph->un.echo.sequence;

	id = 0;
	if (get_client_info(dp_dev, 0, mac, srcip, &id))
		return -1;

	if (memcmp(src_mac, mac, ETH_ALEN)) {
		pr_debug("===src mac: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x====\n",
			 src_mac[0], src_mac[1], src_mac[2],
			 src_mac[3], src_mac[4], src_mac[5]);
		pr_debug("===record mac: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x====\n",
			 mac[0], mac[1], mac[2],
			 mac[3], mac[4], mac[5]);
		pr_err("target mac address mismatch recorded src mac!\n");
		return -1;
	}

	/* Get RXIN data pointer */
	paddr = consume_one_rxin(dp_dev, pool_id);
	if (paddr == (phys_addr_t)-1)
		return -1;

	tmp = paddr & 0xFFFFFFFF;
	vaddr = phys_to_virt(tmp);

	size = ETH_HLEN;
	fill_ethhdr(vaddr, dst_mac, src_mac, ETH_P_IP);

	iph = vaddr + size;
	memset(iph, 0, sizeof(*iph));
	iph->ihl	= 5;
	iph->version	= 4;
	iph->tos	= 0;
	iph->tot_len	= tot_len;
	iph->id		= htons(id);
	iph->ttl	= 64;
	iph->protocol	= IPPROTO_ICMP;
	iph->saddr	= srcip;
	iph->daddr	= dstip;
	iph->check	= ip_fast_csum((u8 *)iph, iph->ihl);

	size += sizeof(*iph);
	icmph = vaddr + size;
	icmph->type	= ICMP_ECHOREPLY;
	icmph->code	= 0;
	icmph->un.echo.id	= icmp_id;
	icmph->un.echo.sequence = icmp_seq;

	size += sizeof(*icmph);
	data_len = ntohs(tot_len) - sizeof(*iph) - sizeof(*icmph);
	memcpy(vaddr + size, buf + size, data_len);
	icmph->checksum = ip_compute_csum((u8 *)icmph, sizeof(*icmph) + data_len);
	size += data_len;
	/* append ethernet CRC */
	crc32 = vaddr + size;
	size += sizeof(*crc32);
	dma_map_single_attrs(tdev->dev, vaddr, size, DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);

	*phy_addr = paddr;
	*pkt_sz = size;

	return 0;
}

static int handle_ip_reply(struct dp_device *dp_dev, void *buf,
			   phys_addr_t *phy_addr, unsigned int *pkt_sz,
			   unsigned int *pool_id)
{
	struct iphdr *ipv4hdr;

	ipv4hdr = buf + ETH_HLEN;

	switch (ipv4hdr->protocol) {
	case IPPROTO_UDP:
		return 1;
	case IPPROTO_ICMP:
		return handle_icmp_reply(dp_dev, buf, phy_addr, pkt_sz, pool_id);
	}

	return -1;
}

/**
 * return 0 means the packet is going to be sent to RXOUT.
 * return -1 means the packet should be dropped. it will be sent to TXOUT
 */
static int packet_parsing(struct dp_device *dp_dev, void *pkt,
			  phys_addr_t *paddr, unsigned int *size,
			  unsigned int *pool_id)
{
	char *buf;
	u16 eth_type;

	buf = pkt;
	eth_type = htons(*(u16 *)&buf[12]);

	if (eth_type == ETH_P_ARP)
		return handle_arp_reply(dp_dev, buf, paddr, size, pool_id);
	else if (eth_type == ETH_P_IP)
		return handle_ip_reply(dp_dev, buf, paddr, size, pool_id);

	return -1;
}

/**
 * This test driver should automatically reply all ARP, ICMP request.
 * Loopback TCP/IP, UDP/IP packet.
 *
 * Any other kind of packets will be dropped and directly return to TXOUT
 */
static void process_txin_packet(struct test_device *tdev, int cnt)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	void *pkt;
	phys_addr_t uc_paddr, paddr, rxpaddr;
	unsigned int i, idx, drop, size, pool_id, tx_poolid, tx_policyid;
	u32 *desc;

	idx = dp_dev->tidx[TXIN];
	for (i = 0; i < cnt; i++) {
		desc = (u32 *)&dp_dev->dev_txin[idx];

		tx_poolid = (desc[3] & GENMASK(27, 24)) >> 24;
		tx_policyid = (desc[3] & GENMASK(23, 16)) >> 16;

		uc_paddr = ret_phy_addr_from_desc(desc);
		paddr = uc_paddr & 0xFFFFFFFF;

		pkt = phys_to_virt(paddr);

		drop = packet_parsing(dp_dev, pkt, &rxpaddr, &size, &pool_id);
		if (!drop)
			process_sendto_rxout(tdev, idx, rxpaddr, size, pool_id);
		process_txin_to_txout(tdev, uc_paddr, tx_poolid, tx_policyid);
		idx = idx_inc(dp_dev, idx, TXIN, DEV_DEVICE);
	}
	dp_dev->tidx[TXIN] = idx;
	dp_dev->gidx[TXIN] = idx_add(dp_dev, dp_dev->gidx[TXIN], TXIN, DEV_DEVICE, cnt);
}

static void process_txin_flow(struct test_device *tdev, int cnt)
{
	process_fill_txin_desc(&tdev->dp_dev, cnt);
	process_txin_packet(tdev, cnt);
}

/**
 * TXOUT:
 *   Add deescriptor:  move fill idx
 *   Get response counters(consume): move transfer idx, guard idx
 */
static void __maybe_unused process_txout(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int unprocess_cnt, cnt, compensate_cnt;

	unprocess_cnt = idx_sub(dp_dev->fidx[TXOUT], dp_dev->tidx[TXOUT],
				dp_dev->buflen[DEV_DEVICE][TXOUT]);
	if (!unprocess_cnt)
		return;

	cnt = idx_sub(dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[TXOUT]],
		      dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[TXOUT]], (unsigned int)-1);
	if (cnt >= unprocess_cnt)
		return;

	compensate_cnt = unprocess_cnt - cnt;
	dp_dev->tidx[TXOUT] = (dp_dev->tidx[TXOUT] + compensate_cnt) %
			      dp_dev->buflen[DEV_DEVICE][TXOUT];
	dp_dev->gidx[TXOUT] = (dp_dev->gidx[TXOUT] + compensate_cnt) %
			      dp_dev->buflen[DEV_DEVICE][TXOUT];
}

/**
 * RXIN:
 *   Add deescriptor(data pointer)/Get RXIN counter:  move fill idx
 *   Take out descriptor: move transfer idx, guard idx
 */
static void process_rxin(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int unfill_cnt, cnt, compensate_cnt;

	unfill_cnt = idx_sub(dp_dev->gidx[RXIN], dp_dev->fidx[RXIN],
			     dp_dev->buflen[DEV_DEVICE][RXIN]);
	if (!unfill_cnt)
		return;

	cnt = idx_sub(dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXIN]],
		      dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXIN]], (unsigned int)-1);
	if (!cnt)
		return;

	WARN(cnt > 64, "rxin counter %u exceed expect(soc: %u, dev: %u)!\n",
	     cnt, dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXIN]],
	     dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXIN]]);

	netdev_dbg(tdev->netdev, "RXIN UMT cnt: %d\n", cnt);

	compensate_cnt = cnt < unfill_cnt ?  cnt : unfill_cnt;
	process_fill_rxin_desc(dp_dev, compensate_cnt);
}

static void __maybe_unused process_txin(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int unfill_cnt, cnt, compensate_cnt;

	unfill_cnt = idx_sub(dp_dev->gidx[TXIN], dp_dev->fidx[TXIN],
			     dp_dev->buflen[DEV_DEVICE][TXIN]);
	if (!unfill_cnt)
		return;

	cnt = idx_sub(dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[TXIN]],
		      dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[TXIN]], (unsigned int)-1);
	if (!cnt)
		return;
	WARN(cnt > 64, "txin counter %u exceed expect(soc: %u, dev: %u)!\n",
	     cnt, dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[TXIN]],
	     dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[TXIN]]);

	compensate_cnt = cnt < unfill_cnt ? cnt : unfill_cnt;

	process_txin_flow(tdev, compensate_cnt);
}

/**
 * RXOUT:
 *   Add deescriptor:  move fill idx
 *   Get response counters(consume): move transfer idx, guard idx
 */
static void __maybe_unused process_rxout(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int unprocess_cnt, cnt, compensate_cnt;

	unprocess_cnt = idx_sub(dp_dev->fidx[RXOUT], dp_dev->tidx[RXOUT],
				dp_dev->buflen[DEV_DEVICE][RXOUT]);
	if (!unprocess_cnt)
		return;

	cnt = dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXOUT]] -
	      dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXOUT]];
	cnt = idx_sub(dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXOUT]],
		      dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXOUT]], (unsigned int)-1);
	WARN(cnt > 64, "rxout counter %u exceed expect(dev: %u, soc: %u)!\n",
	     cnt, dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXOUT]],
	     dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXOUT]]);

	if (cnt >= unprocess_cnt)
		return;

	compensate_cnt = unprocess_cnt - cnt;
	dp_dev->tidx[RXOUT] = (dp_dev->tidx[RXOUT] + compensate_cnt) %
			      dp_dev->buflen[DEV_DEVICE][RXOUT];
	dp_dev->gidx[RXOUT] = (dp_dev->gidx[RXOUT] + compensate_cnt) %
			      dp_dev->buflen[DEV_DEVICE][RXOUT];
}

/**
 * No budget defined in this function as max
 * can only accumulate up to 64 counters
 *
 * Note: I'm using timer to trigger the workqueue.
 * the interval of timer is minimum 4ms which I suppose driver already finished processing.
 */
static void work_cnt_process(struct work_struct *work)
{
	struct test_device *tdev = container_of(work, struct test_device, dev_work);

	if (manual_ops)
		return;

	umttest_dump_info(tdev);
	process_rxin(tdev);
	process_txout(tdev);
	process_rxout(tdev);
	process_txin(tdev);
}

static int create_ethernet_driver(struct umttest_priv *priv,
				  struct test_device **test_dev)
{
	struct net_device *ndev;
	struct test_device *tdev;
	static int devid;
	int err;

	pr_err("%s: line: %u====\n", __func__, __LINE__);
	ndev = devm_alloc_etherdev(&priv->pdev->dev, sizeof(*tdev));
	if (!ndev)
		return -ENOMEM;

	tdev = netdev_priv(ndev);
	tdev->netdev = ndev;
	tdev->dev = &priv->pdev->dev;
	tdev->dev_port = devid++;
	INIT_LIST_HEAD(&tdev->node);
	INIT_WORK(&tdev->dev_work, work_cnt_process);
	sprintf(tdev->name, "umtt%d", tdev->dev_port);

	ndev->netdev_ops = &umttest_netdev_ops;
	SET_NETDEV_DEV(ndev, tdev->dev);
	ndev->min_mtu = ETH_ZLEN;
	ndev->max_mtu = MAX_MTU_SIZE;

	eth_hw_addr_set(ndev, mac_addr);

	err = register_netdev(ndev);
	if (err)
		return err;

	list_add_tail(&tdev->node, &priv->head);
	*test_dev = tdev;

	return 0;
}

static int delete_ethernet_driver(struct umttest_priv *priv)
{
	if (!priv->tdev) {
		pr_err("no test device found!\n");
		return -EINVAL;
	}

	unregister_netdev(priv->tdev->netdev);
	priv->tdev->netdev = NULL;
	return 0;
}

static int dp_sw_rx(struct net_device *rxif, struct net_device *txif,
		    struct sk_buff *skb, int32_t len)
{
	if (!rxif || !skb) {
		pr_err("dp rx information missing!\n");
		return -EINVAL;
	}

	skb->dev = rxif;
	skb->protocol = eth_type_trans(skb, rxif);

	netif_rx(skb);

	return 0;
}

static void umttest_dp_dev_preinit(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	size_t size = sizeof(u32) * UMT_CNT_NUM;

	dp_dev->soc_umt_cnt_addr = devm_kzalloc(tdev->dev, size, GFP_KERNEL);
	dp_dev->soc_umt_cnt_paddr = dma_map_single_attrs(tdev->dev,
							 tdev->dp_dev.soc_umt_cnt_addr,
							 size, DMA_TO_DEVICE, 0);
	dp_dev->umt_interval = const_umt_interval;
}

static void __maybe_unused umttest_dump_info(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;
	unsigned int cnt;

	netdev_dbg(tdev->netdev, "======SoC UMT info ==============\n");

	cnt = dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[TXIN]];
	netdev_dbg(tdev->netdev, "SoC TXIN CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[SOC_DEVICE][TXIN]);
	cnt = dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[TXOUT]];
	netdev_dbg(tdev->netdev, "SoC TXOUT CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[SOC_DEVICE][TXOUT]);
	cnt = dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXIN]];
	netdev_dbg(tdev->netdev, "SoC RXIN CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[SOC_DEVICE][RXIN]);
	cnt = dp_dev->soc_umt_cnt_addr[cnt_idx_in_buf[RXOUT]];
	netdev_dbg(tdev->netdev, "SoC RXOUT CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[SOC_DEVICE][RXOUT]);

	netdev_dbg(tdev->netdev, "------Dev UMT info ------\n");

	cnt = dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[TXIN]];
	netdev_dbg(tdev->netdev, "Dev TXIN CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[DEV_DEVICE][TXIN]);
	cnt = dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[TXOUT]];
	netdev_dbg(tdev->netdev, "Dev TXOUT CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[DEV_DEVICE][TXOUT]);
	cnt = dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXIN]];
	netdev_dbg(tdev->netdev, "Dev RXIN CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[DEV_DEVICE][RXIN]);
	cnt = dp_dev->dev_umt_cnt_addr[cnt_idx_in_buf[RXOUT]];
	netdev_dbg(tdev->netdev, "Dev RXOUT CNT: %u, idx: %u\n",
		   cnt, cnt % dp_dev->buflen[DEV_DEVICE][RXOUT]);
}

static void umttest_dump_dp_dev(struct test_device *tdev)
{
	struct dp_device *dp_dev = &tdev->dp_dev;

	netdev_dbg(tdev->netdev, "======soc rings info =========\n");

	netdev_dbg(tdev->netdev, "soc rxout: ring size:    %d\n",
		   dp_dev->buflen[SOC_DEVICE][RXOUT]);
	netdev_dbg(tdev->netdev, "soc rxout: ring paddr:   0x%llx\n",
		   dp_dev->soc_paddr[RXOUT]);
	netdev_dbg(tdev->netdev, "scc rxout: ring vaddr:   0x%llx\n",
		   (dma_addr_t)dp_dev->soc_rxout);

	netdev_dbg(tdev->netdev, "soc rxin: ring size:    %d\n", dp_dev->buflen[SOC_DEVICE][RXIN]);
	netdev_dbg(tdev->netdev, "soc rxin: ring paddr:   0x%llx\n", dp_dev->soc_paddr[RXIN]);
	netdev_dbg(tdev->netdev, "soc rxin: ring vaddr:   0x%llx\n", (dma_addr_t)dp_dev->soc_rxin);

	netdev_dbg(tdev->netdev, "soc txin: ring size:    %d\n", dp_dev->buflen[SOC_DEVICE][TXIN]);
	netdev_dbg(tdev->netdev, "soc txin: ring paddr:   0x%llx\n", dp_dev->soc_paddr[TXIN]);
	netdev_dbg(tdev->netdev, "soc txin: ring vaddr:   0x%llx\n", (dma_addr_t)dp_dev->soc_txin);

	netdev_dbg(tdev->netdev, "soc txout: ring size:    %d\n",
		   dp_dev->buflen[SOC_DEVICE][TXOUT]);
	netdev_dbg(tdev->netdev, "soc txout: ring paddr:   0x%llx\n", dp_dev->soc_paddr[TXOUT]);
	netdev_dbg(tdev->netdev, "soc txout: ring vaddr:   0x%llx\n",
		   (dma_addr_t)dp_dev->soc_txout);

	netdev_dbg(tdev->netdev, "soc umt paddr base:      0x%llx\n", dp_dev->soc_umt_cnt_paddr);
	netdev_dbg(tdev->netdev, "soc umt vaddr:           0x%llx\n",
		   (dma_addr_t)dp_dev->soc_umt_cnt_addr);

	netdev_dbg(tdev->netdev, "======dev rings info =========\n");

	netdev_dbg(tdev->netdev, "dev rxout: ring size:    %d\n",
		   dp_dev->buflen[DEV_DEVICE][RXOUT]);
	netdev_dbg(tdev->netdev, "dev rxout: ring vaddr:   0x%llx\n",
		   (dma_addr_t)dp_dev->dev_rxout);
	netdev_dbg(tdev->netdev, "dev rxout: ring tidx:    %u\n", dp_dev->tidx[RXOUT]);
	netdev_dbg(tdev->netdev, "dev rxout: ring fidx:    %u\n", dp_dev->fidx[RXOUT]);
	netdev_dbg(tdev->netdev, "dev rxout: ring gidx:    %u\n", dp_dev->gidx[RXOUT]);

	netdev_dbg(tdev->netdev, "dev rxin: ring size:    %d\n", dp_dev->buflen[DEV_DEVICE][RXIN]);
	netdev_dbg(tdev->netdev, "dev rxin: ring vaddr:   0x%llx\n", (dma_addr_t)dp_dev->dev_rxin);
	netdev_dbg(tdev->netdev, "dev rxin: ring tidx:    %u\n", dp_dev->tidx[RXIN]);
	netdev_dbg(tdev->netdev, "dev rxin: ring fidx:    %u\n", dp_dev->fidx[RXIN]);
	netdev_dbg(tdev->netdev, "dev rxin: ring gidx:    %u\n", dp_dev->gidx[RXIN]);

	netdev_dbg(tdev->netdev, "dev txin: ring size:    %d\n", dp_dev->buflen[DEV_DEVICE][TXIN]);
	netdev_dbg(tdev->netdev, "dev txin: ring vaddr:   0x%llx\n", (dma_addr_t)dp_dev->dev_txin);
	netdev_dbg(tdev->netdev, "dev txin: ring tidx:    %u\n", dp_dev->tidx[TXIN]);
	netdev_dbg(tdev->netdev, "dev txin: ring fidx:    %u\n", dp_dev->fidx[TXIN]);
	netdev_dbg(tdev->netdev, "dev txin: ring gidx:    %u\n", dp_dev->gidx[TXIN]);

	netdev_dbg(tdev->netdev, "dev txout: ring size:   %d\n", dp_dev->buflen[DEV_DEVICE][TXOUT]);
	netdev_dbg(tdev->netdev, "dev txout: ring vaddr:  0x%llx\n", (dma_addr_t)dp_dev->dev_txout);
	netdev_dbg(tdev->netdev, "dev txout: ring tidx:   %u\n", dp_dev->tidx[TXOUT]);
	netdev_dbg(tdev->netdev, "dev txout: ring fidx:   %u\n", dp_dev->fidx[TXOUT]);
	netdev_dbg(tdev->netdev, "dev txout: ring gidx:   %u\n", dp_dev->gidx[TXOUT]);

	netdev_dbg(tdev->netdev, "======dp dev print done=========\n");
}

static void umttest_dp_dev_init(struct test_device *tdev)
{
	const unsigned int dev_buf_mul = 2;
	struct dp_device *dp_dev = &tdev->dp_dev;
	struct dp_dev_data *dp_data = &tdev->dp_data;

	INIT_LIST_HEAD(&dp_dev->head);
	/* SoC settings */
	dp_dev->soc_txin = dp_data->tx_ring[0].in_deq_vaddr;
	dp_dev->soc_paddr[TXIN] = (dma_addr_t)dp_data->tx_ring[0].in_deq_paddr;
	dp_dev->buflen[SOC_DEVICE][TXIN] = dp_data->tx_ring[0].in_deq_ring_size;
	dp_dev->soc_txout = dp_data->tx_ring[0].out_free_vaddr;
	dp_dev->soc_paddr[TXOUT] = (dma_addr_t)dp_data->tx_ring[0].out_free_paddr;
	dp_dev->buflen[SOC_DEVICE][TXOUT] = dp_data->tx_ring[0].out_free_ring_size;

	dp_dev->soc_rxin = dp_data->rx_ring[0].in_alloc_vaddr;
	dp_dev->soc_paddr[RXIN] = (dma_addr_t)dp_data->rx_ring[0].in_alloc_paddr;
	dp_dev->buflen[SOC_DEVICE][RXIN] = dp_data->rx_ring[0].in_alloc_ring_size;
	dp_dev->soc_rxout = dp_data->rx_ring[0].out_enq_vaddr;
	dp_dev->soc_paddr[RXOUT] = (dma_addr_t)dp_data->rx_ring[0].out_enq_paddr;
	dp_dev->buflen[SOC_DEVICE][RXOUT] = dp_data->rx_ring[0].out_enq_ring_size;

	/* Dev settings */
	dp_dev->buflen[DEV_DEVICE][TXIN]  = dp_dev->buflen[SOC_DEVICE][TXIN] * dev_buf_mul;
	dp_dev->buflen[DEV_DEVICE][TXOUT] = dp_dev->buflen[SOC_DEVICE][TXOUT] * dev_buf_mul;
	dp_dev->buflen[DEV_DEVICE][RXIN]  = dp_dev->buflen[SOC_DEVICE][RXIN] * dev_buf_mul;
	dp_dev->buflen[DEV_DEVICE][RXOUT] = dp_dev->buflen[SOC_DEVICE][RXOUT] * dev_buf_mul;

	dp_dev->dev_txin = devm_kzalloc(tdev->dev, sizeof(struct dma_desc) *
					dp_dev->buflen[DEV_DEVICE][TXIN], GFP_KERNEL);
	dp_dev->dev_txout = devm_kzalloc(tdev->dev, sizeof(struct dma_desc) *
					 dp_dev->buflen[DEV_DEVICE][TXOUT], GFP_KERNEL);
	dp_dev->dev_rxin = devm_kzalloc(tdev->dev, sizeof(u64) *
					dp_dev->buflen[DEV_DEVICE][RXIN], GFP_KERNEL);
	dp_dev->dev_rxout = devm_kzalloc(tdev->dev, sizeof(struct dma_desc) *
					 dp_dev->buflen[DEV_DEVICE][RXOUT], GFP_KERNEL);

	dp_dev->gidx[TXIN] = dp_dev->buflen[DEV_DEVICE][TXIN] - 1;
	dp_dev->gidx[TXOUT] = dp_dev->buflen[DEV_DEVICE][TXOUT]  - 1;
	dp_dev->gidx[RXIN] = dp_dev->buflen[DEV_DEVICE][RXIN]  - 1;
	dp_dev->gidx[RXOUT] = dp_dev->buflen[DEV_DEVICE][RXOUT]  - 1;

	dp_dev->rx_policy = dp_data->rx_ring[0].rx_policy_base;
}

static void umttest_dp_set_data(struct test_device *tdev)
{
	struct umt_port_ctl *umt_ctl;
	struct umt_port_res *umt_res;
	int i;

	tdev->dp_data.num_rx_ring = 1;
	tdev->dp_data.num_tx_ring = 1;
	tdev->dp_data.num_umt_port = 1;
	tdev->dp_data.bm_policy_res_id = DP_RES_ID_WAV624;
	tdev->dp_data.tx_ring[0].f_out_auto_free = 1;
	tdev->dp_data.umt_dc[0].dc_idx = 0;
	tdev->dp_data.enable_cqm_meta = 1;
	tdev->dp_data.max_ctp = 32;
	tdev->dp_data.max_gpid = 32;
	tdev->dp_data.rx_ring[0].bpress_alloc = DP_BPRESS_DIS;
	tdev->dp_data.rx_ring[0].bpress_out = DP_BPRESS_DIS;
	tdev->dp_data.tx_ring[0].bpress_in = DP_BPRESS_DIS;
	tdev->dp_data.tx_ring[0].bpress_free = DP_BPRESS_DIS;

	umt_res = &tdev->dp_data.umt[0].res;
	if (umt_port_0) {
		umt_res->rx_src = UMT_RX_SRC_DMA;
		umt_res->dma_ch_num = 8;
	} else {
		umt_res->rx_src = UMT_RX_SRC_CQEM;
	}

	umt_ctl = &tdev->dp_data.umt[0].ctl;
	if (!interface) {
		umt_ctl->daddr = tdev->dp_dev.soc_umt_cnt_paddr;
	} else {
		umt_ctl->daddr = 0;
		umt_ctl->dst_addr_cnt = dst_addr_cnt;
		for (i = 0; i < umt_ctl->dst_addr_cnt; i++) {
			umt_ctl->dst[i] = tdev->dp_dev.soc_umt_cnt_paddr +
					  4 * cnt_idx_in_buf[i];
			netdev_dbg(tdev->netdev, "dst[%d] = 0x%llx\n",
				   i, umt_ctl->dst[i]);
		}
	}
	umt_ctl->msg_interval = tdev->dp_dev.umt_interval;
	umt_ctl->msg_mode = msg_mode;
	/* FIXIT: fixed to Accumulate mode at the first release
	 * TODO: support incremental mode
	 */
	umt_ctl->cnt_mode = cnt_mode;
	umt_ctl->rx_msg_mode = rx_msg_mode;
	umt_ctl->enable = 1;
	umt_ctl->fflag = (unsigned long)fflag;
}

static void umttest_dump_resource(struct test_device *tdev)
{
	struct dp_dev_data *data = &tdev->dp_data;
	struct dp_rx_ring *rxring;
	struct dp_tx_ring *txring;

	netdev_dbg(tdev->netdev, "rx ring num: %d\n", data->num_rx_ring);
	netdev_dbg(tdev->netdev, "tx ring num: %d\n", data->num_tx_ring);
	netdev_dbg(tdev->netdev, "umt port num: %d\n", data->num_umt_port);

	rxring = data->rx_ring;
	txring = data->tx_ring;

	netdev_dbg(tdev->netdev, "rxout: ring size:    %d\n", rxring->out_enq_ring_size);
	netdev_dbg(tdev->netdev, "rxout: ring port id: %d\n", rxring->out_enq_port_id);
	netdev_dbg(tdev->netdev, "rxout: ring paddr:   0x%llx\n",
		   (dma_addr_t)rxring->out_enq_paddr);
	netdev_dbg(tdev->netdev, "rxout: ring vaddr:   0x%llx\n",
		   (dma_addr_t)rxring->out_enq_vaddr);

	netdev_dbg(tdev->netdev, "rxin: ring size:    %d\n", rxring->in_alloc_ring_size);
	netdev_dbg(tdev->netdev, "rxin: ring paddr:   0x%llx\n",
		   (dma_addr_t)rxring->in_alloc_paddr);
	netdev_dbg(tdev->netdev, "rxin: ring vaddr:   0x%llx\n",
		   (dma_addr_t)rxring->in_alloc_vaddr);
	netdev_dbg(tdev->netdev, "rx: num_pkt:        %u\n", rxring->num_pkt);
	netdev_dbg(tdev->netdev, "rx: rx pkt size:    %u\n", rxring->rx_pkt_size);
	netdev_dbg(tdev->netdev, "rxin policy id:     %u\n", rxring->rx_policy_base);

	netdev_dbg(tdev->netdev, "txin: ring size:    %d\n", txring->in_deq_ring_size);
	netdev_dbg(tdev->netdev, "txin: ring port id: %d\n", txring->tx_deq_port);
	netdev_dbg(tdev->netdev, "txin: ring paddr:   0x%llx\n", (dma_addr_t)txring->in_deq_paddr);
	netdev_dbg(tdev->netdev, "txin: ring vaddr:   0x%llx\n", (dma_addr_t)txring->in_deq_vaddr);

	netdev_dbg(tdev->netdev, "txout: ring size:    %d\n", txring->out_free_ring_size);
	netdev_dbg(tdev->netdev, "txout: ring paddr:   0x%llx\n",
		   (dma_addr_t)txring->out_free_paddr);
	netdev_dbg(tdev->netdev, "txout: ring vaddr:   0x%llx\n",
		   (dma_addr_t)txring->out_free_vaddr);
	netdev_dbg(tdev->netdev, "tx: num_pkt:        %u\n", txring->num_tx_pkt);
	netdev_dbg(tdev->netdev, "tx: rx pkt size:    %u\n", txring->tx_pkt_size);
	netdev_dbg(tdev->netdev, "tx policy id:       %u\n", txring->txout_policy_base);
}

static int register_dp_datappath(struct test_device *tdev,  enum dp_client client)
{
	int dpid;
	unsigned int flag;
	dp_cb_t cb = {0};
	struct dp_subif_data data = {0};
	struct dp_dc_res *res = &tdev->dc_res;

	if (client == DP_WIFI)
		flag = DP_F_FAST_WLAN | DP_F_ACA;
	else if (client == DP_DSL)
		flag = DP_F_FAST_DSL | DP_F_ACA;
	else
		flag = 0;

	netdev_info(tdev->netdev, "=====dp allocate port!===\n");
	dpid = dp_alloc_port_ext(0, THIS_MODULE, tdev->netdev, tdev->dev_port, 0, NULL, NULL, flag);
	if (dpid < 0) {
		netdev_err(tdev->netdev, "dp alloc port failed!\n");
		return dpid;
	}
	tdev->dpid = dpid;

	res->inst = 0;
	res->dp_port = dpid;
	res->res_id = DP_RES_ID_WAV624;

	cb.rx_fn = dp_sw_rx;
	umttest_dp_set_data(tdev);

	netdev_info(tdev->netdev, "=====dp register dev!===\n");
	if (dp_register_dev_ext(0, THIS_MODULE, tdev->dpid, &cb, &tdev->dp_data, 0)) {
		netdev_err(tdev->netdev, "dp register failed\n");
		goto __reg_fail;
	}

	netdev_info(tdev->netdev, "=====dp register subif !===\n");
	tdev->subif_id.subif = -1;
	tdev->subif_id.port_id = tdev->dpid;

	data.deq_port_idx = 0;
	data.num_deq_port = 1;

	if (dp_register_subif_ext(0, THIS_MODULE, tdev->netdev, tdev->name,
				  &tdev->subif_id, &data, 0)) {
		netdev_err(tdev->netdev, "dp register subif failed!\n");
		goto __subif_fail;
	}

	netdev_info(tdev->netdev, "=====dp register done!===\n");

	umttest_dump_resource(tdev);
	return 0;

__subif_fail:
	dp_register_dev_ext(0, THIS_MODULE, tdev->dpid, &cb, &tdev->dp_data, DP_F_DEREGISTER);
__reg_fail:
	dp_alloc_port_ext(0, THIS_MODULE, tdev->netdev, tdev->dev_port, 0,
			  NULL, NULL, DP_F_DEREGISTER);

	return -ENODEV;
}

static int unregister_dp_datappath(struct test_device *tdev)
{
	struct dp_subif_data data = {0};

	data.deq_port_idx = 0;
	data.num_deq_port = 1;

	if (dp_register_subif_ext(0, THIS_MODULE, tdev->netdev, tdev->name,
				  &tdev->subif_id, &data, DP_F_DEREGISTER))
		netdev_err(tdev->netdev, "dp unregister subif failed!\n");

	if (dp_register_dev_ext(0, THIS_MODULE, tdev->dpid, NULL, &tdev->dp_data, DP_F_DEREGISTER))
		netdev_err(tdev->netdev, "dp unregister dev failed!\n");

	if (dp_alloc_port_ext(0, THIS_MODULE, tdev->netdev, tdev->dev_port, tdev->dpid,
			      NULL, NULL, DP_F_DEREGISTER))
		netdev_err(tdev->netdev, "dp unregister port failed!\n");

	return 0;
}

static void umttest_timer(struct timer_list *t)
{
	struct umttest_priv *priv = from_timer(priv, t, timer);
	struct test_device *tdev, *tdev_tmp;
	struct dp_device *dp_dev;

	list_for_each_entry_safe(tdev, tdev_tmp, &priv->head, node) {
		dp_dev = &tdev->dp_dev;
		queue_work(priv->wk_queue, &tdev->dev_work);
	}

	priv->timer.expires = jiffies + msecs_to_jiffies(1000);
	add_timer(&priv->timer);
}

static int umttest_parse_cnt_idx(void)
{
	cnt_idx_in_buf[RXIN] = find_set_bit(dst_dw_offset_bitmap, RXIN + 1);
	cnt_idx_in_buf[TXIN] = find_set_bit(dst_dw_offset_bitmap, TXIN + 1);
	cnt_idx_in_buf[RXOUT] = find_set_bit(dst_dw_offset_bitmap, RXOUT + 1);
	cnt_idx_in_buf[TXOUT] = find_set_bit(dst_dw_offset_bitmap, TXOUT + 1);

	if (cnt_idx_in_buf[RXIN] < 0 || cnt_idx_in_buf[TXIN] < 0 ||
	    cnt_idx_in_buf[RXOUT] < 0 || cnt_idx_in_buf[TXOUT] < 0) {
		pr_err("cnt idx error!\n");
		return -EINVAL;
	}

	return 0;
}

static int umttest_drv_probe(struct platform_device *pdev)
{
	struct umttest_priv *priv;
	int ret;

	if (umttest_parse_cnt_idx() < 0)
		return -EINVAL;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->pdev = pdev;
	INIT_LIST_HEAD(&priv->head);
	timer_setup(&priv->timer, umttest_timer, 0);
	priv->wk_queue = create_singlethread_workqueue("umttest_wq");
	if (!priv->wk_queue)
		goto __queue_fail;
	platform_set_drvdata(pdev, priv);

	ret = create_ethernet_driver(priv, &priv->tdev);
	if (ret)
		return ret;

	umttest_dp_dev_preinit(priv->tdev);

	ret = register_dp_datappath(priv->tdev, DP_WIFI);
	if (ret)
		return ret;

	umttest_dp_dev_init(priv->tdev);
	umttest_dump_dp_dev(priv->tdev);

	mod_timer(&priv->timer, jiffies + msecs_to_jiffies(10));
	dev_info(&pdev->dev, "MaxLinear UMT test driver loaded!\n");

	return 0;

__queue_fail:
	del_timer_sync(&priv->timer);
	return -EINVAL;
}

static int umttest_drv_remove(struct platform_device *pdev)
{
	struct umttest_priv *priv;
	struct dp_device *dp_dev;

	priv = platform_get_drvdata(pdev);

	flush_workqueue(priv->wk_queue);
	del_timer_sync(&priv->timer);

	if (priv->wk_queue) {
		destroy_workqueue(priv->wk_queue);
		priv->wk_queue = NULL;
	}

	dp_dev = &priv->tdev->dp_dev;
	desc_from_soc_fetch_remain(dp_dev, RXIN,
				   (dp_dev->fidx[RXIN] + 1) % dp_dev->buflen[SOC_DEVICE][RXIN]);

	unregister_dp_datappath(priv->tdev);

	delete_ethernet_driver(priv);

	debugfs_remove_recursive(priv->debugfs);

	dev_info(&pdev->dev, "MaxLinear UMT test driver removed!\n");

	return 0;
}

static const struct of_device_id umttest_dt_match[] = {
	{ .compatible = "mxl,umt-test" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, umttest_dt_match);

static struct platform_driver umt_test_driver = {
	.probe = umttest_drv_probe,
	.remove = umttest_drv_remove,
	.driver = {
		.name = "mxl,umt_test",
		.owner = THIS_MODULE,
		.of_match_table = umttest_dt_match,
	},
};

module_platform_driver(umt_test_driver);

module_param(interface, int, 0600);
MODULE_PARM_DESC(interface, "UMT interface: 0 - Legacy UMT interface, 1 - New UMT interface");
module_param(dst_dw_offset_bitmap, int, 0600);
MODULE_PARM_DESC(dst_dw_offset_bitmap, "UMT destination address dw offset bitmap.");
module_param(dst_addr_cnt, int, 0600);
MODULE_PARM_DESC(dst_addr_cnt, "UMT destination address count");
module_param(msg_mode, int, 0600);
MODULE_PARM_DESC(msg_mode, "UMT msg mode: 0 - SELFCNT, 1 - USER_MODE");
module_param(cnt_mode, int, 0600);
MODULE_PARM_DESC(cnt_mode, "UMT counter mode: 0 - UMT_CNT_INC, 1 - UMT_CNT_ACC");
module_param(rx_msg_mode, int, 0600);
MODULE_PARM_DESC(rx_msg_mode, "UMT rx msg mode: 0 - UMT_RXOUT_MSG_SUB, 1 - UMT_RXIN_MSG_ADD, 2 - UMT_4MSG_MODE");
module_param(fflag, int, 0600);
MODULE_PARM_DESC(fflag, "fflag: BIT(0) - UMT_SND_DIS, BIT(1) - UMT_CLEAR_CNT, BIT(2) - UMT_NOT_SND_ZERO_CNT, BIT(3) - UMT_SND_RX_CNT_ONLY");
module_param(umt_port_0, int, 0600);
MODULE_PARM_DESC(umt_port_0, "Whether UMT port 0 should be used: 0 - No, 1 - Yes");
module_param(manual_ops, int, 0600);
MODULE_PARM_DESC(manual_ops, "Manually update UMT counters: 0 - No, 1 - Yes");

MODULE_LICENSE("GPL v2");
