// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/percpu.h>
#include <linux/pktprs.h>
#include <linux/pp_api.h>
#include <linux/skbuff.h>

#include <net/datapath_api_qos.h>
#include <net/toe_np_lro.h>

#ifdef CONFIG_NETFILTER
#define NF_OPS_CNT            2

#define LRO_PORT_CNT          SZ_256
#define LRO_GROUP_CNT         SZ_32
#define LRO_PORT_PER_GROUP    (LRO_PORT_CNT / LRO_GROUP_CNT)

#define CURRENT_HDR(p) (&(p)->hdr->buf[(p)->buf_off])

struct parse_info {
	struct pktprs_hdr *hdr;
	enum pktprs_hdr_level lvl;
	u8 proto;
	u8 buf_off;
	u8 hdr_sz;
	u16 next_proto;
	u8 *prev_nxt;
	struct sk_buff *skb;
};

struct session_info {
	int signature;
	int ppid;
	int fid;

	struct list_head head;
};

struct session_request {
	struct pp_sess_create_args args;
	struct pktprs_hdr hdr;
	struct work_struct wk;
	struct net_device *ndev;
	struct pktprs_db *db;
};

struct pktprs_db {
	struct nf_hook_ops ops[NF_OPS_CNT];
	struct kmem_cache *cache;
	struct mutex sess_mtx;
	struct mutex fid_mtx;
	struct mutex lro_mtx;
	struct mutex pp_mtx;
	struct spinlock lock;
	struct device *dev;
	int logic_qid;

	struct work_struct reclaim_wk;
	bool reclaim;

	u32 sigs[LRO_PORT_CNT];
	u32 sig_cnt;

	u8 lro_groups[LRO_GROUP_CNT];
	u8 min_weight;
	u8 next_group;

	struct list_head sessions;
};

static int parse_lro(struct sk_buff *skb, struct pktprs_db *db);

static __u8 ip6_proto_get(struct sk_buff *skb)
{
	struct ipv6_opt_hdr *hdr;
	unsigned char *data = skb_network_header(skb);
	__u8 nexthdr = ipv6_hdr(skb)->nexthdr;
	u16 off = sizeof(struct ipv6hdr);

	while (ipv6_ext_hdr(nexthdr) && nexthdr != NEXTHDR_NONE) {
		hdr = (struct ipv6_opt_hdr *)(data + off);
		if (nexthdr == NEXTHDR_FRAGMENT)
			off += sizeof(struct frag_hdr);
		else if (nexthdr == NEXTHDR_AUTH)
			off += ipv6_authlen(hdr);
		else
			off += ipv6_optlen(hdr);
		nexthdr = hdr->nexthdr;
	}

	return nexthdr;
}

static unsigned int pktprs_local_handler(void *priv, struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
	struct pktprs_db *db = (struct pktprs_db *)priv;
	__u8 nexthdr;

	if (!skb->buf_base || skb->len < 1400 || db->reclaim)
		return NF_ACCEPT;

	switch (state->pf) {
	case NFPROTO_IPV4:
		nexthdr = ip_hdr(skb)->protocol;
		if (nexthdr != IPPROTO_TCP)
			return NF_ACCEPT;
		if (ipv4_is_multicast(ip_hdr(skb)->daddr))
			return NF_ACCEPT;
		break;
	case NFPROTO_IPV6:
		nexthdr = ip6_proto_get(skb);
		if (nexthdr != IPPROTO_TCP)
			return NF_ACCEPT;
		if (ipv6_addr_is_multicast(&ipv6_hdr(skb)->daddr))
			return NF_ACCEPT;
		break;
	default:
		return NF_ACCEPT;
	}

	parse_lro(skb, db);
	return NF_ACCEPT;
}

static int proto_add(struct parse_info *p)
{
	if (p->buf_off + p->hdr_sz > p->hdr->buf_sz) {
		p->next_proto = 0;
		return -ENOMEM;
	}
	if (p->prev_nxt)
		*p->prev_nxt = p->proto; /* set the preliminary next protocol */
	p->prev_nxt = &p->hdr->proto_info[p->proto][p->lvl].nxt;
	set_bit(p->proto, &p->hdr->proto_bmap[p->lvl]);
	p->hdr->proto_info[p->proto][p->lvl].off = p->buf_off;
	p->buf_off += p->hdr_sz;
	*p->prev_nxt = PKTPRS_PROTO_PAYLOAD;
	p->hdr->proto_info[PKTPRS_PROTO_PAYLOAD][p->lvl].off = p->buf_off;
	return 0;
}

static void parse_tcp(struct parse_info *p)
{
	struct tcphdr *hdr = (struct tcphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_TCP;
	p->hdr_sz = hdr->doff << 2;
	proto_add(p);
}

static void parse_ip6(struct parse_info *p)
{
	struct ipv6hdr *hdr = (struct ipv6hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_IPV6;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_tcp(p);
}

static void parse_ip(struct parse_info *p)
{
	struct iphdr *hdr = (struct iphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_IPV4;
	p->hdr_sz = hdr->ihl << 2;
	p->next_proto = hdr->protocol;
	if (proto_add(p))
		return;
	parse_tcp(p);
}

static void parse_network_hdr(struct parse_info *p)
{
	switch (p->next_proto) {
	case ETH_P_IP:
		parse_ip(p);
		break;
	case ETH_P_IPV6:
		parse_ip6(p);
		break;
	default:
		break;
	}
}

static void parse_eth(struct parse_info *p)
{
	struct ethhdr *hdr = (struct ethhdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_MAC;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = ntohs(hdr->h_proto);
	if (proto_add(p))
		return;
	parse_network_hdr(p);
}

static void hdr_copy_from_eth(struct sk_buff *skb, struct pktprs_hdr *h)
{
	struct vlan_ethhdr *veth;
	u16 hdr_sz = skb_end_pointer(skb) - skb_mac_header(skb);

	if (skb_vlan_tag_present(skb)) {
		h->buf_sz = min(hdr_sz + VLAN_HLEN , PKTPRS_HDR_BUF_SIZE);
		/* copy the header to off VLAN_HLEN */
		memcpy(&h->buf[VLAN_HLEN], eth_hdr(skb), h->buf_sz - VLAN_HLEN);
		/* move the mac address to the buffer head */
		memmove(h->buf, &h->buf[VLAN_HLEN], ETH_ALEN * 2);
		/* restore the vlan header from the skb */
		veth = (struct vlan_ethhdr *)h->buf;
		veth->h_vlan_proto = skb->vlan_proto;
		veth->h_vlan_TCI = htons(skb_vlan_tag_get(skb));
	} else {
		h->buf_sz = clamp_val(hdr_sz, ETH_HLEN, PKTPRS_HDR_BUF_SIZE);
		memcpy(h->buf, skb_mac_header(skb), h->buf_sz);
	}
}

static void hdr_parse(struct sk_buff *skb, struct net_device *dev,
		      struct pktprs_hdr *h)
{
	struct parse_info p;

	memset(&p, 0, sizeof(p));
	p.hdr = h;
	p.lvl = PKTPRS_HDR_LEVEL0;
	p.skb = skb;
	if (dev)
		h->ifindex = dev->ifindex;
	hdr_copy_from_eth(skb, h);
	parse_eth(&p);
	set_bit(PKTPRS_PROTO_PAYLOAD, &h->proto_bmap[p.lvl]);
	h->buf_sz = p.buf_off;
}

static int find_next_free_fid(struct pktprs_db *db)
{
	int fid = -1;

	mutex_lock(&db->fid_mtx);
	while (db->min_weight <= LRO_PORT_PER_GROUP) {
		unsigned long bm = db->lro_groups[db->next_group];

		if (hweight8(bm) == db->min_weight) {
			u8 next_bit = ffz(bm);

			fid = db->next_group * LRO_PORT_PER_GROUP + next_bit;
			__set_bit(next_bit, &bm);
			db->lro_groups[db->next_group] = bm;
		}
		if (++db->next_group == LRO_GROUP_CNT) {
			db->next_group = 0;
			db->min_weight++;
		}
		if (fid >= 0)
			break;
	}
	mutex_unlock(&db->fid_mtx);
	return fid;
}

static void del_fid(struct pktprs_db *db, int fid)
{
	unsigned long bm;
	int group;
	int off;

	mutex_lock(&db->fid_mtx);
	group = fid / LRO_PORT_PER_GROUP;
	off = fid % LRO_PORT_PER_GROUP;

	bm = db->lro_groups[group];
	__clear_bit(off, &bm);
	db->lro_groups[group] = bm;

	if (hweight8(bm) < db->min_weight) {
		db->next_group = group;
		db->min_weight = hweight8(bm);
	} else if (hweight8(bm) == db->min_weight && db->next_group > group) {
		db->next_group = group;
	}
	mutex_unlock(&db->fid_mtx);
}

static bool add_lro_sess(struct pktprs_db *db,
			 struct net_device *ndev,
			 struct pp_sess_create_args *args,
			 struct pktprs_hdr *hdr)
{
	struct lro_ops *lro;
	struct tcphdr *th;
	int lro_type;
	int ppid;
	int fid;
	int ret;
	int i;

	lro = dp_get_lro_ops();
	if (!lro || !db->logic_qid) {
		netdev_info(ndev, "LRO not ready\n");
		return false;
	}
	lro_type = LRO_TYPE_TCP;

	fid = find_next_free_fid(db);
	if (fid < 0) {
		netdev_info(ndev, "LRO session full\n");
		return false;
	}

	mutex_lock(&db->lro_mtx);
	ret = lro->lro_start(lro->toe, fid, lro_type);
	mutex_unlock(&db->lro_mtx);
	if (ret) {
		netdev_info(ndev, "failed to start LRO fid %d\n", fid);
		return false;
	}

	args->eg_port = 16;
	args->dst_q = db->logic_qid;
	args->lro_info = fid;
	args->tmp_ud_sz = 16; /* bytes */
	args->color = PP_COLOR_GREEN;

	for (i = 0; i < ARRAY_SIZE(args->sgc); i++)
		args->sgc[i] = PP_SGC_INVALID;

	for (i = 0; i < ARRAY_SIZE(args->tbm); i++)
		args->tbm[i] = PP_TBM_INVALID;

	args->ps = (lro_type & 0xFF) << 24;
	args->rx = hdr;
	args->tx = hdr;
	set_bit(PP_SESS_FLAG_LRO_INFO_BIT, &args->flags);
	set_bit(PP_SESS_FLAG_PS_COPY_BIT, &args->flags);
	set_bit(PP_SESS_FLAG_PS_VALID_BIT, &args->flags);
	th = pktprs_tcp_hdr(hdr, PKTPRS_HDR_LEVEL0);
	netdev_dbg(ndev, "src:%d dst:%d syn:%d rst:%d fid:%d qid:%d gpid:%d\n",
		   ntohs(th->source), ntohs(th->dest), th->syn, th->rst,
		   fid, args->dst_q, args->in_port);

	mutex_lock(&db->pp_mtx);
	ret = pp_session_create(args, &ppid, NULL);
	mutex_unlock(&db->pp_mtx);

	if (likely(!ret)) {
		struct session_info *si;

		si = devm_kzalloc(db->dev, sizeof(*si), GFP_KERNEL);
		if (si) {
			si->fid = fid;
			si->ppid = ppid;
			si->signature = args->hash.sig;
			mutex_lock(&db->sess_mtx);
			list_add(&si->head, &db->sessions);
			mutex_unlock(&db->sess_mtx);
			return true;
		}
		ret = -ENOMEM;
	}

	switch(ret) {
	case -EEXIST:
		netdev_info(ndev, "session created");
		break;
	case -ENOMEM:
		netdev_info(ndev, "not enough memory");
		break;
	default:
		netdev_info(ndev, "pp_session_create err %d\n", ret);
		break;
	}

	mutex_lock(&db->lro_mtx);
	lro->lro_stop(lro->toe, fid);
	mutex_unlock(&db->lro_mtx);

	del_fid(db, fid);
	return false;
}

static void del_sig(u32 sig, struct pktprs_db *db)
{
	int i;

	spin_lock_bh(&db->lock);
	if (db->sig_cnt < 2) {
		BUG_ON(db->sig_cnt == 1 && db->sigs[0] != sig);
		db->sig_cnt = 0;
		goto out;
	}
	for (i = 0; i < db->sig_cnt; i++) {
		if (db->sigs[i] != sig)
			continue;
		db->sigs[i] = db->sigs[--db->sig_cnt];
		break;
	}
out:
	spin_unlock_bh(&db->lock);
}

static void reclaim_request(struct work_struct *work)
{
	struct pktprs_db *db = container_of(work, struct pktprs_db, reclaim_wk);
	struct lro_ops *lro = dp_get_lro_ops();
	struct session_info *si;
	int fid;

	if (!lro)
		return;

	mutex_lock(&db->sess_mtx);
	mutex_lock(&db->fid_mtx);
	mutex_lock(&db->lro_mtx);
	mutex_lock(&db->pp_mtx);

	list_for_each_entry(si, &db->sessions, head) {
		pp_session_delete(si->ppid, NULL);
		devm_kfree(db->dev, si);
	}

	INIT_LIST_HEAD(&db->sessions);

	for (fid = 0; fid < LRO_PORT_CNT; fid++)
		lro->lro_stop(lro->toe, fid);

	memset(db->lro_groups, 0, sizeof(db->lro_groups));
	db->min_weight = 0;
	db->next_group = 0;
	db->sig_cnt = 0;

	mutex_unlock(&db->pp_mtx);
	mutex_unlock(&db->lro_mtx);
	mutex_unlock(&db->fid_mtx);
	mutex_unlock(&db->sess_mtx);

	db->reclaim = false;
}

static int add_sig(u32 sig, struct pktprs_db *db)
{
	int ret = -1;
	int i;

	spin_lock_bh(&db->lock);
	for (i = 0; i < db->sig_cnt; i++) {
		if (db->sigs[i] == sig)
			goto out;
	}
	if (db->sig_cnt < LRO_PORT_CNT) {
		db->sigs[db->sig_cnt] = sig;
		ret = db->sig_cnt++;
	} else {
		db->reclaim = true;
		schedule_work(&db->reclaim_wk);
	}
out:
	spin_unlock_bh(&db->lock);
	return ret;
}

static void new_session_request(struct work_struct *work)
{
	struct session_request *sr;

	sr = container_of(work, struct session_request, wk);

	if (!add_lro_sess(sr->db, sr->ndev, &sr->args, &sr->hdr))
		del_sig(sr->args.hash.sig, sr->db);
	kmem_cache_free(sr->db->cache, sr);
}

static int parse_lro(struct sk_buff *skb, struct pktprs_db *db)
{
	struct session_request *sr;
	int idx;
	u32 sig;

	sig = pp_get_signature(skb->buf_base);
	idx = add_sig(sig, db);
	if (idx < 0)
		return NET_RX_SUCCESS;

	sr = kmem_cache_alloc(db->cache, GFP_ATOMIC);
	if (!sr)
		return -ENOMEM;

	memset(sr, 0, sizeof(*sr));
	hdr_parse(skb, NULL, &sr->hdr);
	sr->args.hash.sig = sig;
	sr->db = db;
	sr->args.in_port = pp_get_rx_port(skb->buf_base);
	pp_get_hash(skb->buf_base, &sr->args.hash.h1, &sr->args.hash.h2);
	sr->ndev = skb->dev;
	INIT_WORK(&sr->wk, new_session_request);
	schedule_work(&sr->wk);
	return NET_RX_SUCCESS;
}

static int dp_get_lro_logic_qid(void)
{
	struct dp_spl_cfg conn;

	if (dp_spl_conn_get(0, DP_SPL_TOE, &conn, 1)) {
		struct dp_qos_q_logic q_logic = {
			.q_id = conn.egp[0].qid,
		};

		if(DP_SUCCESS == dp_qos_get_q_logic(&q_logic, 0))
			return q_logic.q_logic_id;
	}
	return -ENOENT;
}

int lro_pktprs_init(struct device *dev, void **priv)
{
	struct pktprs_db *db;
	int i;

	if (IS_ENABLED(CONFIG_MXL_SKB_EXT))
		return 0;

	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (!db)
		return -ENOMEM;

	db->logic_qid = dp_get_lro_logic_qid();
	if (db->logic_qid < 0)
		return db->logic_qid;

	for (i = 0; i < NF_OPS_CNT; i++) {
		db->ops[i].hook = pktprs_local_handler;
		db->ops[i].pf = NFPROTO_IPV4;
		db->ops[i].hooknum = NF_INET_LOCAL_IN;
		db->ops[i].priority = NF_IP_PRI_FILTER;
		db->ops[i].priv = db;
	}
	db->ops[1].pf = NFPROTO_IPV6;
	db->dev = dev;

	spin_lock_init(&db->lock);
	mutex_init(&db->sess_mtx);
	mutex_init(&db->fid_mtx);
	mutex_init(&db->lro_mtx);
	mutex_init(&db->pp_mtx);
	INIT_LIST_HEAD(&db->sessions);
	INIT_WORK(&db->reclaim_wk, reclaim_request);

	db->cache = kmem_cache_create("lro_pktprs",
				      sizeof(struct session_request),
				      0, SLAB_PANIC, NULL);
	if (!db->cache)
		return PTR_ERR(db->cache);

	if (nf_register_net_hooks(&init_net, db->ops, NF_OPS_CNT)) {
		dev_err(dev, "nf hooks registration failed\n");
		kmem_cache_destroy(db->cache);
		return -EINVAL;
	}
	*priv = db;
	return 0;
}

void lro_pktprs_exit(void **priv)
{
	struct pktprs_db *db = *((struct pktprs_db **)priv);

	if (IS_ENABLED(CONFIG_MXL_SKB_EXT))
		return;

	if (db) {
		nf_unregister_net_hooks(&init_net, db->ops, NF_OPS_CNT);
		kmem_cache_destroy(db->cache);
	}
	*priv = NULL;
}
#endif /* CONFIG_NETFILTER */
