/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 * Description: PPv4 Network adapter debugfs interface
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>
#include <linux/bitops.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/parser.h>
#include <linux/pp_api.h>
#include <linux/pp_qos_api.h>
#include <linux/pktprs.h>
#include "pp_network_adapter_internal.h"

#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_NET_ADAPTER]:%s:%d: " fmt, __func__, __LINE__
#endif

/**
 * @define Maximum packet size to use for debug session creation
 */
#define MAX_PKT_SZ      (2048)
/**
 * @define Maximum packet description
 */
#define MAX_PKT_DESC    (32)

#define TC_OFFSET       (58)
#define TC_SIZE         (2)
#define GPID_OFFSET     (96)
#define GPID_SIZE       (8)

#define DBGFS_STR_MAX   (1024)

#define pr_buf(buf, len, n, fmt, ...) \
	(n = scnprintf(buf, len, fmt, ##__VA_ARGS__))

#define pr_buf_cat(buf, len, n, fmt, ...) \
	((n) += scnprintf((buf) + (n), (len) - (n), fmt, ##__VA_ARGS__))

#define NULL_DEVICE "NULL"

struct debugfs_file {
	const char                   *name;
	const struct file_operations *fops;
	void *data;
};

/**
 * @struct define a debug packet for session creation
 * @desc packet description
 * @buf packet data
 * @len packet length
 * @port
 */
struct dbg_pkt {
	char desc[MAX_PKT_DESC];
	u8   buf[MAX_PKT_SZ];
	u32  len;
};

/**
 * @struct Debug session info required for creating a debug session
 * @in_pkt ingress packet
 * @eg_pkt egress packet
 * @hash hash results, must match the ingress packet
 * @autolearn register to pktprs notifier
 * @changed specify if the ingress or egress packets were changed by
 *          the user, if so, hash results will be taken from HW
 * @dst_queue destination queue logical id
 * @flags session flags
 * @mcast multicast session info
 * @is_egress if the session is egress (set bit 27 in PS)
 * @is_tdox if set, tdox flag will be set for TCP sessions
 */
struct dbg_sess_info {
	struct dbg_pkt in_pkt;
	struct dbg_pkt eg_pkt;
	struct dbg_pkt frag_pkt;
	bool   autolearn;
	bool   changed;
	bool   is_egress;
	bool   is_tdox;
	u16    sessions;
	u16    oob_tc;
	u16    oob_gpid;
	char   in_dev_name[32];
	char   eg_dev_name[32];
	struct pp_sess_create_args args;
};

/**
 * @brief Net adapter debugfs dir
 */
static struct dentry *dbgfs;
static unsigned long  req_id;
static spinlock_t  lock;

/**
 * @brief Default session with VLAN header added to egress packet
 *        Only modification is replacing l2
 */
static struct dbg_sess_info dbg_sess = {
	.changed   = false,
	.is_egress = true,
	.is_tdox   = false,
	.sessions = 1,
	.oob_gpid  = U16_MAX,
	.oob_tc    = 0,
	.autolearn = false,
	.eg_dev_name = NULL_DEVICE,
	.in_dev_name = NULL_DEVICE,
	.frag_pkt = {
		.desc = "frag",
		.len = 8,
		.buf = {
			/* MAC */
			0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, /* dest   */
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, /* source */
			0x08, 0x00,                         /* proto  */
			/* IPv4 */
			0x45, 0x00, 0x00, 0x32, 0x00, 0x00,
			0x00, 0x00, 0x40,
			0x11,                   /* protocol */
			0x74, 0xB6,             /* checksum */
			0x01, 0x01, 0x01, 0x01, /* source   */
			0x02, 0x02, 0x02, 0x02, /* dest     */
			/* UDP */
			0x04, 0xD2, /* source */
			0x16, 0x2E, /* dest   */
			0x00, 0x1E, /* length */
			0x70, 0x33, /* csum   */
			/* Data */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
	},
	.in_pkt = {
		.desc = "ingress",
		.len = 64,
		.buf = {
			/* MAC */
			0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, /* dest   */
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, /* source */
			0x08, 0x00,                         /* proto  */
			/* IPv4 */
			0x45, 0x00, 0x00, 0x32, 0x00, 0x00,
			0x00, 0x00, 0x40,
			0x11,                   /* protocol */
			0x74, 0xB6,             /* checksum */
			0x01, 0x01, 0x01, 0x01, /* source   */
			0x02, 0x02, 0x02, 0x02, /* dest     */
			/* UDP */
			0x04, 0xD2, /* source */
			0x16, 0x2E, /* dest   */
			0x00, 0x1E, /* length */
			0x70, 0x33, /* csum   */
			/* Data */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
	},
	.eg_pkt = {
		.desc = "egress",
		.len = 64,
		.buf = {
			/* MAC */
			0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, /* dest   */
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, /* source */
			0x81, 0x00, 0x02, 0x2B,		    /* vlan   */
			0x08, 0x00,			    /* proto  */
			/* IPv4 */
			0x45, 0x00, 0x00, 0x32, 0x00, 0x00,
			0x00, 0x00, 0x40,
			0x11,                   /* protocol */
			0x74, 0xB6,             /* checksum */
			0x01, 0x01, 0x01, 0x01, /* source   */
			0x02, 0x02, 0x02, 0x02, /* dest     */
			/* UDP */
			0x04, 0xD2, /* source */
			0x16, 0x2E, /* dest   */
			0x00, 0x1E, /* length */
			0x70, 0x33, /* csum   */
			/* Data */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
	},
	.args = {
		.in_port = PP_PORT_INVALID,
		.eg_port = PP_PORT_INVALID,
		.dst_q   = PP_QOS_INVALID_ID,
		.dst_q_high = PP_QOS_INVALID_ID,
		.color    = PP_COLOR_GREEN,
		.hash.h1  = 0xAB1FB,
		.hash.h2  = 0x4D571,
		.hash.sig = 0xCB3E08BC,
		.sgc[0]   = PP_SGC_INVALID,
		.sgc[1]   = PP_SGC_INVALID,
		.sgc[2]   = PP_SGC_INVALID,
		.sgc[3]   = PP_SGC_INVALID,
		.sgc[4]   = PP_SGC_INVALID,
		.sgc[5]   = PP_SGC_INVALID,
		.sgc[6]   = PP_SGC_INVALID,
		.sgc[7]   = PP_SGC_INVALID,
		.tbm[0]   = PP_TBM_INVALID,
		.tbm[1]   = PP_TBM_INVALID,
		.tbm[2]   = PP_TBM_INVALID,
		.tbm[3]   = PP_TBM_INVALID,
		.tbm[4]   = PP_TBM_INVALID,
	}
};

/**
 * @brief Debug callback for accepting session manager events
 * @param args event arguments
 */
static void __net_adapter_dbg_cb(struct pp_cb_args *args)
{
	if (unlikely(ptr_is_null(args)))
		return;

	if (unlikely(!__pp_is_event_valid(args->ev)))
		return;

	pr_debug("PP event %s(%u): args %p, req_id %lu, ret %d\n",
		 PP_EVENT_STR(args->ev), args->ev,
		 args, args->req_id, args->ret);

	switch (args->ev) {
	case PP_SESS_CREATE:
	{
		struct pp_sess_create_cb_args *cr_args;

		if (!dbg_sess.autolearn) {
			cr_args = container_of(
				args, struct pp_sess_create_cb_args, base);
			if (cr_args->base.ret == -EEXIST)
				pr_debug("Session already exist, id %u\n",
					 cr_args->sess_id);
			else if (!cr_args->base.ret)
				pr_info("Session %u created\n",
					cr_args->sess_id);
			else
				pr_info("Failed to create session\n");
		}
		break;
	}

	default:
		pr_err("Event %s not supported\n", PP_EVENT_STR(args->ev));
		break;
	}
}

static struct sk_buff *test_skb;

/**
 * @brief Pktprs done event handler
 * @param unused
 * @param ev
 * @param data pktprs descriptor
 * @return 0 always
 */
static s32 __net_adapter_dbg_pktprs_handler(struct notifier_block *unused,
					    unsigned long ev, void *data)
{
	struct pktprs_desc *pkt = (struct pktprs_desc *)data;
	struct pp_request req;
	struct pp_sess_create_args args;
	struct net_device *in_dev, *eg_dev;
	struct pp_desc *desc;
	struct pp_qos_dev *qdev;
	dp_subif_t *ig_subif, *eg_subif;
	struct dp_dequeue_res dq_res = {0};
	s32 ret;

	ig_subif = kzalloc(sizeof(*ig_subif), GFP_KERNEL);
	if (!ig_subif) {
		pr_err("failed to allocate memory for ig_subif\n");
		return 0;
	}
	eg_subif = kzalloc(sizeof(*eg_subif), GFP_KERNEL);
	if (!eg_subif) {
		pr_err("failed to allocate memory for eg_subif\n");
		kfree(ig_subif);
		return 0;
	}
	spin_lock(&lock);

	/* ignore all other skb if not autolearn */
	if (!dbg_sess.autolearn && (pkt->skb != test_skb))
		goto done;

	if (ev != PKTPRS_RXTX)
		goto done;

	memset(&req,  0, sizeof(req));
	memcpy(&args, &dbg_sess.args, sizeof(args));
	args.rx = pkt->rx;
	args.tx = pkt->tx;

	if (dbg_sess.is_egress) {
		/* Set eg_flag in stw ps 0 (bit 27) */
		args.ps = BIT(27);
		set_bit(PP_SESS_FLAG_PS_VALID_BIT, &args.flags);
	}

	if ((pkt->rx->ifindex) &&
	    (pkt->tx->ifindex)) {
		/* get qos device */
		qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
		if (unlikely(ptr_is_null(qdev)))
			goto done;

		in_dev = dev_get_by_index(&init_net,
					  pkt->rx->ifindex);
		eg_dev = dev_get_by_index(&init_net,
					  pkt->tx->ifindex);

		if (in_dev)
			dev_put(in_dev);

		if (eg_dev)
			dev_put(eg_dev);

		if (!in_dev || !eg_dev) {
			pr_err("Error fetching netowrk device\n");
			goto done;
		}

		if (dp_get_netif_subifid(in_dev,
					 NULL, NULL, NULL,
					 ig_subif, 0)) {
			pr_err("Failed fetching subif %s dp info\n",
			       in_dev->name);
			goto done;
		}

		if (dp_get_netif_subifid(eg_dev,
					 NULL, NULL, NULL,
					 eg_subif, 0)) {
			pr_err("Failed fetching subif %s dp info\n",
			       in_dev->name);
			goto done;
		}

		args.in_port = ig_subif->gpid;
		args.eg_port = eg_subif->gpid;
		dq_res.dev = eg_dev;
		dq_res.dp_port = eg_subif->port_id;
		dq_res.q_res = NULL;
		dq_res.q_res_size = 0;
		if (dp_deq_port_res_get(&dq_res, 0) != DP_SUCCESS) {
			pr_err("Failed to Get number of QOS queues for device %s (gpid %u)\n",
			       eg_dev->name, eg_subif->gpid);
			goto done;
		}

		if (dq_res.num_q == 0) {
			pr_err("No queues returned from dp_deq_port_res_get\n");
			goto done;
		}

		dq_res.q_res =
			kzalloc(sizeof(struct dp_queue_res) * dq_res.num_q,
				GFP_KERNEL);
		if (dq_res.q_res == NULL) {
			pr_err("Queue list allocation failed\n");
			goto done;
		}

		dq_res.q_res_size = dq_res.num_q;
		if (dp_deq_port_res_get(&dq_res, 0) != DP_SUCCESS) {
			pr_err("Failed to Get QOS queue list for device %s (gpid %u)\n",
			       eg_dev->name, eg_subif->gpid);
			goto free_mem;
		}

		if (dbg_sess.is_tdox &&
		    (PKTPRS_IS_TCP(args.rx, 0))) {
			if (dq_res.num_q == 1) {
				pr_err("Failed getting high queue\n");
				goto free_mem;
			}

			args.dst_q_high =
			    pp_qos_queue_id_get(qdev, dq_res.q_res[0].q_id);
			args.dst_q =
			    pp_qos_queue_id_get(qdev, dq_res.q_res[1].q_id);
			set_bit(PP_SESS_FLAG_TDOX_PRIO_BIT, &args.flags);
			set_bit(PP_SESS_FLAG_TDOX_SUPP_BIT, &args.flags);
		}
		else {
			args.dst_q =
			    pp_qos_queue_id_get(qdev, dq_res.q_res[0].q_id);
			args.dst_q_high = args.dst_q;
		}

		if (dbg_sess.autolearn) {
			desc = pp_pkt_desc_get(pkt->skb);
			if (!desc) {
				pr_err("Cookie Descriptor null\n");
				goto free_mem;
			} else {
				args.hash.h1 = desc->ud.hash_h1;
				args.hash.h2 = desc->ud.hash_h2;
				args.hash.sig = desc->ud.hash_sig;
			}
		} else if (dbg_sess.changed) {
			pp_session_dbg_hash_get(&args.hash);
			dbg_sess.changed = false;
		}

		pr_debug("Creating session %s(port %u) ==> %s(port %u Queue %u)\n",
			 in_dev->name, args.in_port, eg_dev->name,
			 args.eg_port, args.dst_q);
		kfree(dq_res.q_res);
	} else {
		/* Do not create static session if autolearn is enabled */
		if (dbg_sess.autolearn)
			goto done;

		if (dbg_sess.changed) {
			pp_session_dbg_hash_get(&args.hash);
			dbg_sess.changed = false;
		}
	}

	req.req_id = req_id++;
	req.cb     = __net_adapter_dbg_cb;
	ret = pp_session_create(&args, NULL, &req);
	if (ret)
		pr_err("Session create failed, ret = %d\n", ret);

done:
	spin_unlock(&lock);
	kfree(ig_subif);
	kfree(eg_subif);
	return 0;

free_mem:
	kfree(dq_res.q_res);
	spin_unlock(&lock);
	kfree(ig_subif);
	kfree(eg_subif);
	return 0;
}

/**
 * @struct Pktprs layer done event notifier
 */
static struct notifier_block notifier = {
	.notifier_call = __net_adapter_dbg_pktprs_handler,
};

/**
 * @brief Create a session with SKBs using the pktprs layer
 */
void __net_adapter_dbg_sess_create(struct seq_file *f)
{
	struct packet_type ptype;
	const struct ethhdr *eth;
	struct sk_buff *skb;
	struct net_device *net_dev = NULL;

	memset(&ptype, 0, sizeof(ptype));
	/* allocate an skb */
	skb = dev_alloc_skb(MAX_PKT_SZ);
	if (!skb)
		return;
	test_skb = skb;
	pktprs_register(&notifier, PKTPRS_RXTX);
	/* set ingress packet */
	memcpy(skb->data, dbg_sess.in_pkt.buf, dbg_sess.in_pkt.len);
	skb_put(skb, dbg_sess.in_pkt.len);
	skb_reset_mac_header(skb);
	eth = (const struct ethhdr *)skb_mac_header(skb);
	skb->protocol = eth->h_proto;

	if (strncmp(dbg_sess.in_dev_name, NULL_DEVICE, strlen(NULL_DEVICE)) != 0) {
		net_dev = dev_get_by_name(&init_net, dbg_sess.in_dev_name);
		if (net_dev == NULL) {
			pr_err("Failed to find net device with the name %s\n", dbg_sess.in_dev_name);
			goto done;
		} else {
			dev_put(net_dev);
		}
		skb->dev = net_dev;
	}

	pktprs_do_parse(skb, net_dev, PKTPRS_ETH_RX);

	/* set egress packet */
	skb_trim(skb, dbg_sess.in_pkt.len); /* trim ingress packet data */
	memcpy(skb->data, dbg_sess.eg_pkt.buf, dbg_sess.eg_pkt.len);
	skb_put(skb, dbg_sess.eg_pkt.len);
	skb_reset_mac_header(skb);
	eth = (const struct ethhdr *)skb_mac_header(skb);
	skb->protocol = eth->h_proto;
	skb->pkt_type = PACKET_OUTGOING;

	if (strncmp(dbg_sess.eg_dev_name, NULL_DEVICE, strlen(NULL_DEVICE)) != 0) {
		net_dev = dev_get_by_name(&init_net, dbg_sess.eg_dev_name);
		if (net_dev == NULL) {
			pr_err("Failed to find net device with the name %s\n", dbg_sess.eg_dev_name);
			goto done;
		} else {
			dev_put(net_dev);
		}
		skb->dev = net_dev;
	}

	pktprs_do_parse(skb, net_dev, PKTPRS_ETH_TX);

	/* unregister from pktprs done */
	if (!dbg_sess.autolearn)
		pktprs_unregister(&notifier, PKTPRS_RXTX);
done:
	consume_skb(skb);
}

static int __dbg_sess_create_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if (unlikely(!f))
		return 0;

	__net_adapter_dbg_sess_create(f);

	return 0;
}

static int __dbg_sess_create_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_sess_create_rd, inode->i_private);
}

static const struct file_operations dbg_sess_create_fops = {
	.open    = __dbg_sess_create_open,
	.read    = seq_read,
	.release = single_release,
};

/**
 * @brief Create a static session w/o packets
 */
void __net_adapter_dbg_static_sess_create(struct seq_file *f)
{
	struct pp_port_cfg cfg;
	u32 sess_id;
	s32 ret;

	dbg_sess.args.rx = NULL;
	dbg_sess.args.tx = NULL;

	if (dbg_sess.oob_gpid != U16_MAX) {
		if (pp_port_get(dbg_sess.oob_gpid, &cfg)) {
			pr_err("pp_port_get failed in to create oob sess\n");
			return;
		}

		cfg.rx.parse_type          = NO_PARSE;
		cfg.rx.cls.n_flds          = 2;
		cfg.rx.cls.cp[0].stw_off   = TC_OFFSET;
		cfg.rx.cls.cp[0].copy_size = TC_SIZE;
		cfg.rx.cls.cp[1].stw_off   = GPID_OFFSET;
		cfg.rx.cls.cp[1].copy_size = GPID_SIZE;

		if (pp_port_update(dbg_sess.oob_gpid, &cfg)) {
			pr_err("pp_port_get failed in to create oob sess\n");
			return;
		}

		dbg_sess.args.cls.n_flds = 2;
		dbg_sess.args.cls.fld_data[0] = dbg_sess.oob_tc;
		dbg_sess.args.cls.fld_data[1] = dbg_sess.oob_gpid;

		pr_info("oob_tc[%u] oob_gpid[%u]\n", dbg_sess.oob_tc,
			dbg_sess.oob_gpid);
	} else {
		pp_session_dbg_hash_get(&dbg_sess.args.hash);
	}

	ret = pp_session_create(&dbg_sess.args, &sess_id, NULL);

	if (ret == -EEXIST)
		pr_info("Session already exist, id %u\n", sess_id);
	else if (!ret)
		pr_info("Session %u created\n", sess_id);
	else
		pr_info("Failed to create session, ret %d\n", ret);
}

static int __dbg_oob_sess_create_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if (unlikely(!f))
		return 0;

	__net_adapter_dbg_static_sess_create(f);

	return 0;
}

static int __dbg_oob_sess_create_open(struct inode *inode,
					 struct file *file)
{
	return single_open(file, __dbg_oob_sess_create_rd,
			   inode->i_private);
}

static const struct file_operations dbg_oob_sess_create_fops = {
	.open    = __dbg_oob_sess_create_open,
	.read    = seq_read,
	.release = single_release,
};

/**
 * @brief Setting ingress and egress packets for debug session
 *        creation
 */
void __net_adapter_dbg_pkt_set(char *cmd_buf, void *data)
{
	struct dbg_pkt *pkt;
	char *str, byte[3];
	s32 ret;

	str       = cmd_buf;
	str       = strim(str);

	if (!num_is_even(strlen(str))) {
		pr_err("packet string must be an even number, %u\n",
		       (u32)strlen(str));
		return;
	}

	pkt = (struct dbg_pkt *)data;
	pkt->len = 0;
	while (*str) {
		/* we cannot use the original string we got cause it doesn't
		 * have any delimeters so we copy next 2 bytes to another
		 * buffer
		 */
		ret = strscpy(byte, str, sizeof(byte));
		ret = kstrtou8(byte, 16, &pkt->buf[pkt->len++]);
		if (ret) {
			pr_info("failed to parse '%s'\n", byte);
			return;
		}
		str += strlen(byte);
	}

	dbg_sess.changed = true;

	if (pkt == &dbg_sess.in_pkt)
		dut_dbg_pkt_set(FRAG_0, pkt->buf, pkt->len, dbg_sess.is_tdox, dbg_sess.sessions);
	if (pkt == &dbg_sess.frag_pkt)
		dut_dbg_pkt_set(FRAG_1, pkt->buf, pkt->len, dbg_sess.is_tdox, dbg_sess.sessions);
}

/**
 * @brief Print ingress and egress debug packets
 */
void __net_adapter_pr_pkt_buf_show(struct seq_file *f)
{
	struct dbg_pkt *pkt;
	u32 i;

	pkt = (struct dbg_pkt *)f->private;
	if (!pkt)
		return;

	seq_printf(f, "\n %s packet, len %u\n", pkt->desc, pkt->len);
	seq_puts(f, " =========================\n");
	for (i = 0; i < pkt->len; i++) {
		seq_printf(f, " 0x%02x", pkt->buf[i]);
		if (!((i + 1) % 16))
			seq_puts(f, "\n");
	}
	seq_puts(f, "\n\n");
}

static int __dbg_pkt_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if (unlikely(!f))
		return 0;

	__net_adapter_pr_pkt_buf_show(f);

	return 0;
}

static int __dbg_pkt_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_pkt_rd, inode->i_private);
}

static ssize_t __dbg_pkt_wr(struct file *file,
			    const char __user *buf,
			    size_t count, loff_t *pos)
{
	s32 ret;
	char cmd_buf[DBGFS_STR_MAX];

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;
	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (unlikely(ret != count))
		return ret >= 0 ? -EIO : ret;
	cmd_buf[count - 1] = '\0';
	__net_adapter_dbg_pkt_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations dbg_pkt_fops = {
	.open    = __dbg_pkt_open,
	.read    = seq_read,
	.write   = __dbg_pkt_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

enum sess_args_opts {
	sess_args_opt_help = 1,
	sess_args_opt_in_port,
	sess_args_opt_eg_port,
	sess_args_opt_fsqm_prio,
	sess_args_opt_color,
	sess_args_opt_flags,
	sess_args_opt_dst_q,
	sess_args_opt_dst_q_high,
	sess_args_opt_sgc_0,
	sess_args_opt_sgc_1,
	sess_args_opt_sgc_2,
	sess_args_opt_sgc_3,
	sess_args_opt_sgc_4,
	sess_args_opt_sgc_5,
	sess_args_opt_sgc_6,
	sess_args_opt_sgc_7,
	sess_args_opt_tbm_0,
	sess_args_opt_tbm_1,
	sess_args_opt_tbm_2,
	sess_args_opt_tbm_3,
	sess_args_opt_tbm_4,
	sess_args_opt_tmp_ud_sz,
	sess_args_opt_ps,
	sess_args_opt_h1,
	sess_args_opt_h2,
	sess_args_opt_sig,
	sess_args_opt_mc_grp_idx,
	sess_args_opt_mc_dst_idx,
	sess_args_opt_autolearn,
	sess_args_opt_isegress,
	sess_args_opt_istdox,
	sess_args_opt_oob_gpid,
	sess_args_opt_oob_tc,
	sess_args_opt_in_dev_name,
	sess_args_opt_eg_dev_name,
	sess_args_opt_tcp_sessions,
};

static const match_table_t sess_args_tokens = {
	{sess_args_opt_help,       "help"},
	{sess_args_opt_in_port,    "in_port=%u"},
	{sess_args_opt_eg_port,    "eg_port=%u"},
	{sess_args_opt_fsqm_prio,  "fsqm_prio=%u"},
	{sess_args_opt_color,      "color=%u"},
	{sess_args_opt_flags,      "flags=%x"},
	{sess_args_opt_dst_q,      "dst_q=%u"},
	{sess_args_opt_dst_q_high, "dst_q_high=%u"},
	{sess_args_opt_sgc_0,      "sgc0=%u"},
	{sess_args_opt_sgc_1,      "sgc1=%u"},
	{sess_args_opt_sgc_2,      "sgc2=%u"},
	{sess_args_opt_sgc_3,      "sgc3=%u"},
	{sess_args_opt_sgc_4,      "sgc4=%u"},
	{sess_args_opt_sgc_5,      "sgc5=%u"},
	{sess_args_opt_sgc_6,      "sgc6=%u"},
	{sess_args_opt_sgc_7,      "sgc7=%u"},
	{sess_args_opt_tbm_0,      "tbm0=%u"},
	{sess_args_opt_tbm_1,      "tbm1=%u"},
	{sess_args_opt_tbm_2,      "tbm2=%u"},
	{sess_args_opt_tbm_3,      "tbm3=%u"},
	{sess_args_opt_tbm_4,      "tbm4=%u"},
	{sess_args_opt_tmp_ud_sz,  "tmp_ud_sz=%u"},
	{sess_args_opt_ps,         "ps_value=%x"},
	{sess_args_opt_h1,         "h1=%x"},
	{sess_args_opt_h2,         "h2=%x"},
	{sess_args_opt_sig,        "sig=%x"},
	{sess_args_opt_mc_grp_idx, "mc_grp_idx=%u"},
	{sess_args_opt_mc_dst_idx, "mc_dst_idx=%u"},
	{sess_args_opt_autolearn,  "autolearn"},
	{sess_args_opt_isegress,   "isegress"},
	{sess_args_opt_istdox,     "istdox=%u"},
	{sess_args_opt_oob_gpid,   "oob_gpid=%u"},
	{sess_args_opt_oob_tc,     "oob_tc=%u"},
	{sess_args_opt_in_dev_name, "in_dev_name=%s"},
	{sess_args_opt_eg_dev_name, "eg_dev_name=%s"},
	{sess_args_opt_tcp_sessions, "sessions=%u"}
};

/**
 * @brief Session delete help
 */
static void __net_adapter_sess_args_help(void)
{
	pr_info("\n");
	pr_info(" Usage: echo <option>[=value] > sess_args\n");
	pr_info(" Options:\n");
	pr_info("   help       - print this help function\n");
	pr_info("   in_port    - set ingress port\n");
	pr_info("   eg_port    - set egress port\n");
	pr_info("   fsqm_prio  - set fsqm priority\n");
	pr_info("   color      - set color\n");
	pr_info("   flags      - set session flags\n");
	pr_info("   dst_q      - set destination queue\n");
	pr_info("   dst_q_high - set high destination queue\n");
	pr_info("   sgc_0      - set sgc 0\n");
	pr_info("   sgc_1      - set sgc 1\n");
	pr_info("   sgc_2      - set sgc 2\n");
	pr_info("   sgc_3      - set sgc 3\n");
	pr_info("   sgc_4      - set sgc 4\n");
	pr_info("   sgc_5      - set sgc 5\n");
	pr_info("   sgc_6      - set sgc 6\n");
	pr_info("   sgc_7      - set sgc 7\n");
	pr_info("   tbm_0      - set tbm 0\n");
	pr_info("   tbm_1      - set tbm 1\n");
	pr_info("   tbm_2      - set tbm 2\n");
	pr_info("   tbm_3      - set tbm 3\n");
	pr_info("   tbm_4      - set tbm 4\n");
	pr_info("   tmp_ud_sz  - set ud template size\n");
	pr_info("   ps_value   - set protocol specific value (32Msb)\n");
	pr_info("   h1         - set hash 1 result\n");
	pr_info("   h2         - set hash 2 result\n");
	pr_info("   sig        - set signature\n");
	pr_info("   mc_grp_idx - set multicast group id\n");
	pr_info("   mc_dst_idx - set multicast destination id\n");
	pr_info("   autolearn  - toggle auto learn\n");
	pr_info("   isegress   - toggle UD's is egress bit\n\n");
	pr_info("   oob_gpid   - set gpid for OOB session and enable NO_PARSE on port\n\n");
	pr_info("   oob_tc     - set tc for OOB session\n\n");
	pr_info("   in_dev_name  - device name ingress packet go through, insert NULL for none\n");
	pr_info("   eg_dev_name - device name egress packet go through, insert NULL for none \n");
}

/**
 * @brief Set Session create arguments
 */
void __net_adapter_sess_args_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum sess_args_opts opt;
	char *tok;
	int ret, val;
	u64 val64;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, sess_args_tokens, substr);
		switch (opt) {
		case sess_args_opt_help:
			__net_adapter_sess_args_help();
			break;
		case sess_args_opt_in_port:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.in_port = (u16)val;
			break;
		case sess_args_opt_eg_port:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.eg_port = (u16)val;
			break;
		case sess_args_opt_fsqm_prio:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.fsqm_prio = (u8)val;
			break;
		case sess_args_opt_color:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.color = (u8)val;
			break;
		case sess_args_opt_flags:
			ret = match_hex(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.flags = (ulong)val;
			break;
		case sess_args_opt_dst_q:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.dst_q = (u32)val;
			break;
		case sess_args_opt_dst_q_high:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.dst_q_high = (u32)val;
			break;
		case sess_args_opt_sgc_0:
		case sess_args_opt_sgc_1:
		case sess_args_opt_sgc_2:
		case sess_args_opt_sgc_3:
		case sess_args_opt_sgc_4:
		case sess_args_opt_sgc_5:
		case sess_args_opt_sgc_6:
		case sess_args_opt_sgc_7:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.sgc[opt - sess_args_opt_sgc_0] = (u16)val;
			break;
		case sess_args_opt_tbm_0:
		case sess_args_opt_tbm_1:
		case sess_args_opt_tbm_2:
		case sess_args_opt_tbm_3:
		case sess_args_opt_tbm_4:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.tbm[opt - sess_args_opt_tbm_0] = (u16)val;
			break;
		case sess_args_opt_tmp_ud_sz:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.tmp_ud_sz = (u8)val;
			break;
		case sess_args_opt_ps:
			ret = match_u64(&substr[0], &val64);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.ps = (u32)val64;
			set_bit(PP_SESS_FLAG_PS_VALID_BIT,
				&dbg_sess.args.flags);
			break;
		case sess_args_opt_h1:
			ret = match_hex(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.hash.h1 = (u32)val;
			dbg_sess.changed = false;
			break;
		case sess_args_opt_h2:
			ret = match_hex(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.hash.h2 = (u32)val;
			dbg_sess.changed = false;
			break;
		case sess_args_opt_sig:
			ret = match_u64(&substr[0], &val64);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.hash.sig = (u32)val64;
			dbg_sess.changed = false;
			break;
		case sess_args_opt_mc_grp_idx:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.mcast.grp_idx = (u16)val;
			break;
		case sess_args_opt_mc_dst_idx:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.args.mcast.dst_idx = (u8)val;
			break;
		case sess_args_opt_autolearn:
			dbg_sess.autolearn = !dbg_sess.autolearn;
			if (dbg_sess.autolearn)
				pktprs_register(&notifier, PKTPRS_RXTX);
			else
				pktprs_unregister(&notifier, PKTPRS_RXTX);
			break;
		case sess_args_opt_isegress:
			dbg_sess.is_egress = !dbg_sess.is_egress;
			break;
		case sess_args_opt_istdox:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.is_tdox = (bool)val;
			break;
		case sess_args_opt_tcp_sessions:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.sessions = (u16)val;
			break;
		case sess_args_opt_oob_tc:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.oob_tc = (u16)val;
			break;
		case sess_args_opt_oob_gpid:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dbg_sess.oob_gpid = (u16)val;
			break;
		case sess_args_opt_in_dev_name:
			ret = match_strlcpy(dbg_sess.in_dev_name,&substr[0], sizeof(dbg_sess.in_dev_name));
			if (!ret)
				goto parse_err;
			break;
		case sess_args_opt_eg_dev_name:
			ret = match_strlcpy(dbg_sess.eg_dev_name,&substr[0], sizeof(dbg_sess.eg_dev_name));
			if (!ret)
				goto parse_err;
			break;
		default:
			goto parse_err;
		}
	}
	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static void __net_adapter_sess_args_show(struct seq_file *f)
{
	u32 i;

	pr_info("\n");
	pr_info(" Session Create Arguments:\n");
	pr_info("  autolearn  - %s\n",   dbg_sess.autolearn ? "true" : "false");
	pr_info("  is_egress  - %s\n",   dbg_sess.is_egress ? "true" : "false");
	pr_info("  is_tdox    - %s\n",   dbg_sess.is_tdox ? "true" : "false");
	pr_info("  tcp_sess   - %u\n",   dbg_sess.sessions);
	pr_info("  in_port    - %u\n",   dbg_sess.args.in_port);
	pr_info("  eg_port    - %u\n",   dbg_sess.args.eg_port);
	pr_info("  fsqm_prio  - %u\n",   dbg_sess.args.fsqm_prio);
	pr_info("  color      - %u\n",   dbg_sess.args.color);
	pr_info("  flags      - %#lx\n", dbg_sess.args.flags);
	pr_info("  dst_q      - %u\n",   dbg_sess.args.dst_q);
	pr_info("  dst_q_high - %u\n",   dbg_sess.args.dst_q_high);
	pr_info("  tmp_ud_sz  - %u\n",   dbg_sess.args.tmp_ud_sz);
	pr_info("  ps_value   - %#x\n",  dbg_sess.args.ps);
	pr_info("  h1         - %#x\n",  dbg_sess.args.hash.h1);
	pr_info("  h2         - %#x\n",  dbg_sess.args.hash.h2);
	pr_info("  sig        - %#x\n",  dbg_sess.args.hash.sig);
	pr_info("  mc_grp_idx - %u\n",   dbg_sess.args.mcast.grp_idx);
	pr_info("  mc_dst_idx - %u\n",   dbg_sess.args.mcast.dst_idx);
	for (i = 0; i < PP_SI_SGC_MAX; i++)
		pr_info("  sgc[%u]     - %u\n", i, dbg_sess.args.sgc[i]);
	for (i = 0; i < PP_SI_TBM_MAX; i++)
		pr_info("  tbm[%u]     - %u\n", i, dbg_sess.args.tbm[i]);
	if (dbg_sess.oob_gpid != U16_MAX) {
		pr_info("  OOB        - gpid[%u] tc[%u]", dbg_sess.oob_gpid,
			dbg_sess.oob_tc);
	}
	else {
		pr_info("No OOB params   configured\n");
	}

	pr_info("  in_dev_name  - %s\n",   dbg_sess.in_dev_name );
	pr_info("  eg_dev_name  - %s\n",   dbg_sess.eg_dev_name );

	pr_info("\n");
}

static int __dbg_sess_args_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if (unlikely(!f))
		return 0;

	__net_adapter_sess_args_show(f);

	return 0;
}

static int __dbg_sess_args_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_sess_args_set_rd, inode->i_private);
}

static ssize_t __dbg_sess_args_set_wr(struct file *file, const char __user *buf,
				      size_t count, loff_t *pos)
{
	s32 ret;
	char cmd_buf[DBGFS_STR_MAX];

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;
	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (unlikely(ret != count))
		return ret >= 0 ? -EIO : ret;
	cmd_buf[count - 1] = '\0';
	__net_adapter_sess_args_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations sess_args_fops = {
	.open    = __dbg_sess_args_set_open,
	.read    = seq_read,
	.write   = __dbg_sess_args_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int __dbg_create_dut_session_rd(struct seq_file *f, void *offset)
{
	struct packet_type ptype;
	struct sk_buff *skb;
	struct net_device *net_dev = NULL;
	u32 pp_hw_desc[DUT_HW_DESC_SIZE_WORDS];
	int ret = 0;

	memset(&ptype, 0, sizeof(ptype));
	/* allocate an skb */
	skb = dev_alloc_skb(MAX_PKT_SZ);
	if (!skb) {
		pr_err("Session create failed due to SKB allocation failure");
		return -1;
	}

	/* set ingress packet */
	if (strncmp(dbg_sess.in_dev_name, NULL_DEVICE, strlen(NULL_DEVICE)) != 0) {
	net_dev = dev_get_by_name(&init_net, dbg_sess.in_dev_name);
		if (net_dev == NULL) {
			pr_err("Failed to find net device with the name %s\n",
				dbg_sess.in_dev_name);
			ret = -1;
			goto done;
		}

		dev_put(net_dev);
		skb->dev = net_dev;
	}

	memcpy(skb->data, dbg_sess.in_pkt.buf, dbg_sess.in_pkt.len);
	skb->protocol = eth_type_trans(skb, net_dev);
	skb_put(skb, dbg_sess.in_pkt.len);
	skb->buf_base = (unsigned char *)pp_hw_desc;
	dut_pkt_desc_get((unsigned char *)&pp_hw_desc);

	/* hook skb to attach cookie*/
	pp_rx_pkt_hook(skb);
	/* parse ingress packet */
	pktprs_do_parse(skb, net_dev, PKTPRS_ETH_RX);

	/* set egress packet */
	skb_trim(skb, dbg_sess.in_pkt.len); /* trim ingress packet data */
	if (strncmp(dbg_sess.eg_dev_name, NULL_DEVICE, strlen(NULL_DEVICE)) != 0) {
		net_dev = dev_get_by_name(&init_net, dbg_sess.eg_dev_name);
		if (net_dev == NULL) {
			pr_err("Failed to find net device with the name %s\n",
					dbg_sess.eg_dev_name);
			ret = -1;
			goto done;
		}

		dev_put(net_dev);
		skb->dev = net_dev;
	}

	memcpy(skb->data, dbg_sess.eg_pkt.buf, dbg_sess.eg_pkt.len);
	skb_put(skb, dbg_sess.eg_pkt.len);
	skb->pkt_type = PACKET_OUTGOING;
	skb->protocol = eth_type_trans(skb, net_dev);
	{
		u32 i=0;
		for(i=0;i<10;i++) {
			/* parse 1 for learning */
			pktprs_do_parse(skb, net_dev, PKTPRS_ETH_TX);
		}
	}
done:
	consume_skb(skb);

	return ret;
}

static int __dbg_create_dut_if_rd(struct seq_file *f, void *offset)
{
	s32 ret;
	static bool create_once = 1;

	if (create_once) {
		ret = dut_port_init();
		if (unlikely(ret)) {
			pr_err("Failed to initialized debug port, ret %d\n", ret);
			return -1;
		}
		create_once = 0;
		pr_info("initialized debug port, dutdev0\n");
	}
	return 0;
}

static int __create_dut_session_rd(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_create_dut_session_rd, inode->i_private);
}

static int __create_dut_if_rd(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_create_dut_if_rd, inode->i_private);
}

static const struct file_operations dbg_create_dut_session_fops = {
	.open    = __create_dut_session_rd,
	.read    = seq_read,
	.release = single_release,
};

static const struct file_operations dbg_create_dut_if_fops = {
	.open    = __create_dut_if_rd,
	.read    = seq_read,
	.release = single_release,
};
static struct debugfs_file debugfs_files[] = {
	{"create",      &dbg_sess_create_fops,     NULL},
	{"create_oob",  &dbg_oob_sess_create_fops, NULL},
	{"create_args", &sess_args_fops,           NULL},
	{"ingress_pkt", &dbg_pkt_fops,             &dbg_sess.in_pkt},
	{"egress_pkt",  &dbg_pkt_fops,             &dbg_sess.eg_pkt},
	{"frag_pkt",    &dbg_pkt_fops,             &dbg_sess.frag_pkt},
	{"dut_create",  &dbg_create_dut_session_fops, NULL},
	{"dut_if",      &dbg_create_dut_if_fops, NULL},
};

s32 net_adapter_dbg_init(void)
{
	struct debugfs_file *f;
	struct dentry *dent = NULL;
	s32 ret;
	u32 i;

	spin_lock_init(&lock);

	if (unlikely(!debugfs_initialized())) {
		pr_err("Debugfs not initialized yet\n");
		return -EPERM;
	}

	dbgfs = debugfs_create_dir("pp_net_adapter", NULL);
	if (unlikely(!dbgfs)) {
		pr_err("Failed to create pp net adapter debugfs dir\n");
		ret = -EINVAL;
		goto file_err;
	}

	for (i = 0; i < ARRAY_SIZE(debugfs_files); i++) {
		f = &debugfs_files[i];
		dent = debugfs_create_file(f->name, 0600, dbgfs,
					   f->data, f->fops);
		if (unlikely(IS_ERR_OR_NULL(dent))) {
			pr_err("Failed to create '%s' file\n", f->name);
			ret = (s32)PTR_ERR(dent);
			goto file_err;
		}
	}

	return 0;

file_err:
	debugfs_remove_recursive(dbgfs);
	pr_err("debugfs file error %d\n", ret);
	return ret;
}

s32 net_adapter_dbg_clean(void)
{
	debugfs_remove_recursive(dbgfs);
	dbgfs = NULL;

	return 0;
}
