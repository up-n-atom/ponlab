// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
#include <linux/list.h>
#include <net/datapath_api.h>
#include <net/datapath_api_vlan.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "datapath_instance.h"
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#include "datapath_swdev.h"
#endif
#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
#include "datapath_api_fn.h"
#endif
#include "datapath_ver.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define DP_STRINGIFY(x) #x
#define DP_MACRO_STR(x) DP_STRINGIFY(x)
#define DP_DBGFS_NAME		"dp"
#define DBGFS_DBG		"dbg"
#define DBGFS_EVENT		"event"
#define DBGFS_INST		"inst"
#define DBGFS_DEV		"dev"
#define DBGFS_DEV_OPS		"dev_ops"
#define DBGFS_BR		"br"
#define DBGFS_INST_HAL		"inst_hal"
#define DBGFS_INST_MOD		"inst_mod"
#define DBGFS_MIB_GLOBAL	"mib_global_stats"
#define DBGFS_PCE		"pce"
#define DBGFS_CTP_LIST		"pmap_ctp_list"
#define DBGFS_PORT		"port"
#define DBGFS_QOS		"qos"
#define DBGFS_BR_VLAN		"vlan"
#define DBGFS_DPID		"dpid"
#define DBGFS_FN		"fn"
#define DBGFS_QID		"qid_def"
#define DBGFS_DEQ_PORT		"deq"
#define DBGFS_ENQ_PORT		"enq"
#define DBGFS_HOOKS		"hooks"
#define DBGFS_GDB		"gdb"
#define DBGFS_BP		"bp_pmapper"
#define DBGFS_DTS_RAW           "dts_raw"
#define DBGFS_DTS               "dts"
#define DBGFS_DTS_CATEGORY      "dts_category"

#define DEBUGFS_DBG DP_DEBUGFS_PATH	"/" DBGFS_DBG
#define DBG_CTP DP_DEBUGFS_PATH		"/" DBGFS_CTP_LIST
#define DBG_PORT DP_DEBUGFS_PATH	"/" DBGFS_PORT
#define DBG_DEV_NAME DP_DEBUGFS_PATH	"/" DBGFS_DEV_OPS

/* NOTE: keep in sync with enum DP_RXOUT_QOS_MODE */
const char *dp_rxout_qos_mode_str[] = {
	"bypass QoS only",	/* DP_RXOUT_BYPASS_QOS_ONLY */
	"with QoS",		/* DP_RXOUT_QOS */
	"bypass QoS and FSQM",	/* DP_RXOUT_BYPASS_QOS_FSQM */
	"invalid"		/* DP_RXOUT_QOS_MAX */
};

/* NOTE: keep in sync with enum umt_sw_msg */
const char *umt_sw_msg_str[] = {
	[UMT_NO_MSG] = "No MSG",		/* UMT_NO_MSG */
	[UMT_MSG0_ONLY] = "MSG0 only",		/* UMT_MSG0_ONLY */
	[UMT_MSG1_ONLY] = "MSG1 only",		/* UMT_MSG1_ONLY */
	[UMT_MSG0_MSG1] = "MSG0 and MSG1",	/* UMT_MSG0_MSG1 */
	[UMT_MSG4] = "MSG4"			/* UMT_MSG4 */
};

/* NOTE: keep in sync with enum umt_cnt_mode */
const char *umt_cnt_mode_str[] = {
	[UMT_CNT_INC] ="Inc",		/* Incremental count */
	[UMT_CNT_ACC] = "Accum"		/* Accumulate count */
};

/* NOTE: keep in sync with enum umt_rx_msg_mode */
const char *umt_rx_mode_str[] = {
	[UMT_RXOUT_MSG_SUB] = "Rx_out_sub",	/* RX OUT SUB mode */
	[UMT_RXIN_MSG_ADD] = "rx_in_add",	/* RX IN Add mode */
	[UMT_4MSG_MODE] = "4_msg"		/* 4 msg mode */
};

/* NOTE: keep in sync with enum umt_rx_src */
const char *umt_rx_src_str[] = {
	[UMT_RX_SRC_CQEM] = "from_CQM",		/* RX count from CQM */
	[UMT_RX_SRC_DMA] ="from_DMA"		/* RX count from DMA */
};

/* NOTE: keep in sync with enum umt_msg_mode */
const char *umt_msg_mode_str[] = {
	[UMT_MSG_SELFCNT] = "HW",	/* HW count mode */
	[UMT_MSG_USER_MODE] = "user"	/* User count mode */
};

/* CQM Lookup Mode for LGM */
const char *cqm_lookup_mode_lgm[] = {
	"subif_id[13:8] + class[1:0]",
	"subif_id[7:0]",
	"subif_id[11:8] + class[3:0]",
	"subif_id[4:0] + class[2:0]",
	"class[1:0] + subif_id[5:0]",
	"subif_id[15:8]",
	"subif_id[1:0] + class[3:0] + color[1:0]",
	"subif_id[14:8] + class[0]"
};

/* CQM Lookup Mode for FMx */
const char *cqm_lookup_mode_fmx[] = {
	"flowid[7:6] + dec + enc + class[3:0]",
	"subif_id[7:0]",
	"subif_id[11:8] + class[3:0]",
	"subif_id[4:0] + class[2:0]"
};

const char *dp_port_type[] = {
	"Linux",
	"DPDK"
};

const char *gsw_port_mode[] = {
	"8BIT_WLAN",
	"9BIT_WLAN",
	"PORT_GPON",
	"PORT_EPON",
	"PORT_GINT",
	"PORT_DOCSIS",
	"PORT_OTHERS"
};

const char *dp_spl_conn_type[] = {
	"NON_SPL",
	"SPL_TOE",
	"SPL_VOICE",
	"SPL_VPNA",
	"SPL_APP_LITEPATH",
	"SPL_PP_NF",
	"SPL_PP_DUT",
	"NOT VALID"
};
DPM_BUILD_BUG_ON(ARRAY_SIZE(dp_spl_conn_type) < DP_SPL_MAX,
		dp_spl_conn_type_lessthan_DP_SPL_MAX);

typedef int (*print_ctp_bp_t)(struct seq_file *s, int inst,
			      struct pmac_port_info *port,
			      int subif_index, u32 flag);

#define BUF_SIZE 1024
#define PROC_WRITE_PORT_DEF -1
static int tmp_inst;
static int proc_write_port = PROC_WRITE_PORT_DEF;
static int proc_write_vap = PROC_WRITE_PORT_DEF;
static bool dump_one_port;
static int dump_one_deq = -1;
static bool dump_one_vap;
static int proc_port_init(void *param);
int proc_port_dump(struct seq_file *s, int pos);
static ssize_t proc_port_write(struct file *file, const char *buf,
			       size_t count, loff_t *ppos);
#if defined(CONFIG_DPM_DATAPATH_DBG) && CONFIG_DPM_DATAPATH_DBG
static void proc_dbg_read(struct seq_file *s);
static ssize_t proc_dbg_write(struct file *, const char *, size_t, loff_t *);
#endif

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
static int proc_brvlan_dump(struct seq_file *s, int pos)
{
	struct pmac_port_info *port_info;
	struct bridge_member_port *bmp;
	struct br_info *brdev_info;
	struct vlist_entry *vlist;
	struct vlan_entry *vlan;
	struct hlist_head *head;
	struct pce_entry *pce;
	dp_subif_t *subif;
	int i, vap;
	bool entry;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if (pos == 0) {
		dp_sprintf(s, "|%-10s|%-10s|%-10s|%-10s|%-10s|%-5s",
			   "Br", "BrFid", "BrVlanID", "Dev", "Bp", "VID");
		dp_sprintf(s, "|%-5s|%-10s|%-10s|%-15s\n",
			   "FiD", "RefCnt", "PCE Idx", "PCE Status");
		for (i = 0; i < 100; i++)
			dp_sputs(s, "=");
		dp_sputs(s, "\n");
	}

	head = get_dp_g_bridge_id_entry_hash_table_info(pos);
	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	brdev_info = hlist_entry_safe(head->first, struct br_info, br_hlist);
	if (!brdev_info || !brdev_info->br_vlan_en)
		goto exit;

	entry = false;
	list_for_each_entry(vlist, &brdev_info->br_vlan_list, list) {
		if (!vlist->vlan_entry)
			continue;
		if (entry)
			dp_sputs(s, "\n");

		dp_sprintf(s, "|%-10s", brdev_info->dev->name);
		dp_sprintf(s, "|%-10d", brdev_info->fid);
		dp_sprintf(s, "|%-10d", vlist->vlan_entry->vlan_id);
		entry = true;
	}

	if (!entry)
		dp_sprintf(s, "%-11s", "");
	list_for_each_entry(bmp, &brdev_info->bp_list, list) {
		dp_sputs(s, "\n");
		dp_sprintf(s, "%-11s%-11s%-11s", "", "", "");
		if (dp_get_netif_subifid(bmp->dev, NULL, NULL, NULL,
					 subif, 0)) {
			dp_sprintf(s, "%s: dp_get_netif_subifid failed\n",
				   __func__);
			kfree(subif);
			return DP_FAILURE;
		}
		port_info = get_dp_port_info(subif->inst, subif->port_id);
		vap = GET_VAP(subif->subif, port_info->vap_offset,
			      port_info->vap_mask);
		entry = false;
		list_for_each_entry(vlist, &bmp->bport_vlan_list, list) {
			if (!vlist->vlan_entry)
				continue;
			vlan = vlist->vlan_entry;
			if (entry) {
				dp_sputs(s, "\n");
				dp_sprintf(s, "%-11s%-11s%-11s",
					   "", "", "");
			}
			dp_sprintf(s, "|%-10s", bmp->dev->name);
			dp_sprintf(s, "|%-10d", bmp->bportid);
			dp_sprintf(s, "|%-5d|%-5d|%-10d",
				   vlan->vlan_id, vlan->fid, vlan->ref_cnt);
			entry = true;
			pce =
			   get_pce_entry_from_ventry(vlan, subif->port_id, vap);
			if (!pce)
				continue;
			dp_sprintf(s, "|%-10d|%-15s",
				   pce->blk_info->info.subblk_firstidx +
				   pce->idx,
				   pce->disable ? "Disabled" : "Enabled");
		}
	}
	dp_sputs(s, "\n");

exit:
	if (!dp_seq_has_overflowed(s)) {
		pos++;
		if (pos == BR_ID_ENTRY_HASH_TABLE_SIZE)
			pos = -1;
	}
	kfree(subif);

	return pos;
}
#endif

static int proc_mib_stats_dump(struct seq_file *s, int pos)
{
	struct mib_global_stats stats = {0};
	int cpu;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!capable(CAP_SYS_PACCT))
		return -1;

	for_each_online_cpu(cpu) {
		stats.rx_rxif_pkts += MIB_G_STATS_GET(rx_rxif_pkts, cpu);
		stats.rx_txif_pkts += MIB_G_STATS_GET(rx_txif_pkts, cpu);
		stats.rx_drop += MIB_G_STATS_GET(rx_drop, cpu);
		stats.tx_pkts += MIB_G_STATS_GET(tx_pkts, cpu);
		stats.tx_drop += MIB_G_STATS_GET(tx_drop, cpu);
	}

	dp_sputs(s, "MIB Global Rx/Tx counters\n");
	dp_sprintf(s, "rx_rxif_pkts  = %llu\n", stats.rx_rxif_pkts);
	dp_sprintf(s, "rx_txif_pkts  = %llu\n", stats.rx_txif_pkts);
	dp_sprintf(s, "rx_drop       = %llu\n", stats.rx_drop);
	dp_sprintf(s, "tx_pkts       = %llu\n", stats.tx_pkts);
	dp_sprintf(s, "tx_drop       = %llu\n", stats.tx_drop);

	return -1;
}

ssize_t proc_mib_stats_write(struct file *file, const char *buf, size_t count,
			     loff_t *ppos)
{
	int cpu;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	/* only clear action is supported i.e. '0' */
	if (count > 2 || buf[0] != '0') {
		pr_info("usage: to clear stats\n");
		pr_info("  echo 0 > %s\n", DBGFS_MIB_GLOBAL);
	} else {
		for_each_online_cpu(cpu) {
			MIB_G_STATS_RESET(rx_rxif_pkts, cpu);
			MIB_G_STATS_RESET(rx_txif_pkts, cpu);
			MIB_G_STATS_RESET(rx_drop, cpu);
			MIB_G_STATS_RESET(tx_pkts, cpu);
			MIB_G_STATS_RESET(tx_drop, cpu);
		}
	}

	return count;
}

int proc_port_init(void *param)
{
	tmp_inst = 0;
	dump_one_vap = false;
	dump_one_port = false;

	/* Check if we have been requested to dump only a specific port */
	if (proc_write_port != PROC_WRITE_PORT_DEF) {
		dump_one_port = true;
		if (proc_write_vap != PROC_WRITE_PORT_DEF)
			dump_one_vap = true;
		/* Change to requested port id */
		return proc_write_port;
	}
	return 0;
}

static int dump_dc_info(struct seq_file *s, struct pmac_port_info *port)
{
	u8 cid, pid;
	u16 nid;
	int i, j;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct cqm_igp_info *enq;
#endif
	for (i = 0; i < port->num_tx_ring; i++) {
		dp_sprintf(s, "    DC TxRing:         %d\n", i);
		dp_sprintf(s, "      not_valid:                    %d\n",
			   port->tx_ring[i].not_valid);
		dp_sprintf(s, "      TXIN  tx_deq_port:            %d\n",
			   port->tx_ring[i].tx_deq_port);
		dp_sprintf(s, "      TXIN  DeqRingSize/paddr:      %d/0x%px\n",
			   port->tx_ring[i].in_deq_ring_size,
			   port->tx_ring[i].in_deq_paddr);
		dp_sprintf(s, "      TXOUT FreeRingSize/paddr:     %d/0x%px\n",
			   port->tx_ring[i].out_free_ring_size,
			   port->tx_ring[i].out_free_paddr);
		dp_sprintf(s, "      TXOUT PolicyBase/Poolid:      %d/%d\n",
			   port->tx_ring[i].txout_policy_base,
			   port->tx_ring[i].tx_poolid);
		dp_sprintf(s, "      PolicyNum:                    %d\n",
			   port->tx_ring[i].policy_num);
		dp_sprintf(s, "      NumOfTxPkt/TxPktSize:         %d/%d\n",
			   port->tx_ring[i].num_tx_pkt,
			   port->tx_ring[i].tx_pkt_size);
	}

	for (i = 0; i < port->num_rx_ring; i++) {
		dp_sprintf(s, "    DC RxRing:         %d\n", i);
		dp_sprintf(s, "      RXOUT EnqRingSize/paddr/pid:  %d/0x%px/%d\n",
			   port->rx_ring[i].out_enq_ring_size,
			   port->rx_ring[i].out_enq_paddr,
			   port->rx_ring[i].out_enq_port_id);
		dp_sprintf(s, "      RXOUT NumOfDmaCh:             %d\n",
			   port->rx_ring[i].num_out_tx_dma_ch);
		dp_dma_parse_id(port->rx_ring[i].out_dma_ch_to_gswip, &cid,
				&pid, &nid);
		dp_sprintf(s, "      RXOUT dma-ctrl/port/chan:     %d/%d/%d\n",
			   cid, pid, nid);
		dp_sprintf(s, "      RXOUT NumOfCqmDqPort/pid:     %d/%d\n",
			   port->rx_ring[i].num_out_cqm_deq_port,
			   port->rx_ring[i].out_cqm_deq_port_id);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		dp_sprintf(s, "      RXOUT Poolid:                 %d\n",
			   port->rx_ring[i].rx_poolid);
		enq = get_dp_enqport_info(tmp_inst,
					  port->rx_ring[i].out_enq_port_id);
		dp_sprintf(s, "      RXIN  DcDummyRead:            %d\n",
			   enq->dc_dummy_read);
#endif
		dp_sprintf(s, "      RXIN  InAllocRingSize/Paddr:  %d/0x%px\n",
			   port->rx_ring[i].in_alloc_ring_size,
			   port->rx_ring[i].in_alloc_paddr);
		dp_sprintf(s, "      NumPkt/Pktsize/Policybase:    %d/%d/%d\n",
			   port->rx_ring[i].num_pkt,
			   port->rx_ring[i].rx_pkt_size,
			   port->rx_ring[i].rx_policy_base);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		dp_sprintf(s, "      PolicyNum:                    %d\n",
			   port->rx_ring[i].policy_num);
		dp_sprintf(s, "      RXOUT MsgMode/QosMode:        %d/%d\n",
			   port->rx_ring[i].out_msg_mode,
			   port->rx_ring[i].out_qos_mode);
#endif
		dp_sprintf(s, "      PreFillPktNum/PktBase:        %d/0x%px\n",
			   port->rx_ring[i].prefill_pkt_num,
			   port->rx_ring[i].pkt_base_paddr);

		for (j = 0; j < port->rx_ring[i].num_egp; j++) {
			dp_sprintf(s, "      qos_egp[%d] port: %d\n",
				   j, port->rx_ring[i].egp[j].deq_port);
			dp_sprintf(s, "                  txpush_addr: 0x%px\n",
				   port->rx_ring[i].egp[j].txpush_addr);
			dp_sprintf(s, "                  txpush_addr_qos: 0x%px\n",
				   port->rx_ring[i].egp[j].txpush_addr_qos);
			dp_sprintf(s, "                  tx_pkt_credit: %d\n",
				   port->rx_ring[i].egp[j].tx_pkt_credit);
		}
	}

	dp_sprintf(s, "    Num of UMT:        %d\n", port->num_umt_port);
	for (i = 0; i < port->num_umt_port; i++) {
		dp_sprintf(s, "      UMT[%d]:\n", i);
		dp_sprintf(s, "        dc_umt_map_Index:                    %d\n",
			   port->umt_dc_map_idx[i]);
		dp_sprintf(s, "        UMT id/CqmDeqPid/CqmEnqPid/msg_mode: %d/%d/%d/%s\n",
			   port->umt[i]->ctl.id, port->umt[i]->res.cqm_dq_pid,
			   port->umt[i]->res.cqm_enq_pid,
			   umt_msg_mode_str[port->umt[i]->ctl.msg_mode]);
		dp_sprintf(s, "        rx_src/sw_msg/cnt/rx_msg mode:       %s/%s/%s/%s\n",
			   umt_rx_src_str[port->umt[i]->res.rx_src],
			   umt_sw_msg_str[port->umt[i]->ctl.sw_msg],
			   umt_cnt_mode_str[port->umt[i]->ctl.cnt_mode],
			   umt_rx_mode_str[port->umt[i]->ctl.rx_msg_mode]);
		dp_sprintf(s, "        UMT period/daddr:                    %d/0x%llx\n",
			   port->umt[i]->ctl.msg_interval,
			   (unsigned long long)port->umt[i]->ctl.daddr);
	}
	return 0;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
static void dump_subif_spl_cfg_info(struct seq_file *s,
				    struct dp_subif_info *sif)
{
	struct dp_spl_cfg *cfg = sif->spl_cfg;
	struct dp_spl_igp *igp;
	struct dp_spl_egp *egp;
	u8 cid, pid;
	u16 nid;
	int i;

	dp_sputs(s, "           CPU Special config info:\n");
	dp_sprintf(s, "           spl_conn_type:  %s\n",
		   dp_spl_conn_type[cfg->type]);
	dp_sprintf(s, "           spl_conn_idx:   %d\n", cfg->spl_id);

	if (cfg->spl_gpid)
		dp_sprintf(s, "           spl_gpid:       %d\n",
			   cfg->spl_gpid);
	dp_sprintf(s, "           dp_cb:          0x%px\n",
		   cfg->dp_cb);

	/* IGP info */
	for (i = 0; i < cfg->num_igp; i++) {
		igp = &cfg->igp[i];
		dp_sprintf(s, "           igp[%d]:\n", i);
		dp_sprintf(s, "             igp_id:               %d\n",
			   igp->igp_id);
		dp_sprintf(s, "             igp_ring_size:        %d\n",
			   igp->igp_ring_size);
		dp_sprintf(s, "             igp_paddr:            0x%px\n",
			   igp->igp_paddr);
		dp_sprintf(s, "             alloc_paddr:          0x%px\n",
			   igp->alloc_paddr);
		dp_sprintf(s, "             alloc_vaddr:          0x%px\n",
			   igp->alloc_vaddr);
		dp_dma_parse_id(igp->igp_dma_ch_to_gswip, &cid, &pid, &nid);
		if (igp->num_out_tx_dma_ch)
			dp_sprintf(s, "             dma-ctrl/port/chan:   %d/%d/%d\n",
				   cid, pid, nid);
		dp_sprintf(s, "             num_out_tx_dma_ch:    %d\n",
			   igp->num_out_tx_dma_ch);
		dp_sprintf(s, "             out_qos_mode:         %d:%s\n",
			   igp->out_qos_mode,
			   dp_rxout_qos_mode_str[igp->out_qos_mode]);
		dp_sprintf(s, "             num_out_cqm_deq_port: %d\n",
			   igp->num_out_cqm_deq_port);

		if (igp->egp) {
			dp_sprintf(s, "             igp->egp->egp_id:     %d\n",
				   igp->egp->egp_id);
			dp_sprintf(s, "             igp->egp->qid:        %d\n",
				   igp->egp->qid);
		}
	}

	/* EGP info */
	for (i = 0; i < cfg->num_egp; i++) {
		egp = &cfg->egp[i];
		dp_sprintf(s, "           egp[%d]:\n", i);
		dp_sprintf(s, "             egp_id:               %d\n",
			   egp->egp_id);
		dp_sprintf(s, "             port type:            to %s\n",
			   egp->type ? "GSWIP" : "DEV");
		dp_sprintf(s, "             pp_ring_size:         %d\n",
			   egp->pp_ring_size);
		dp_sprintf(s, "             egp_paddr:            0x%px\n",
			   egp->egp_paddr);
		dp_sprintf(s, "             tx_pkt_credit:        %d\n",
			   egp->tx_pkt_credit);
		dp_sprintf(s, "             tx_push_paddr_qos:    0x%px\n",
			   egp->tx_push_paddr_qos);
		dp_sprintf(s, "             free_paddr:           0x%px\n",
			   egp->free_paddr);
		dp_sprintf(s, "             free_vaddr:           0x%px\n",
			   egp->free_vaddr);
		if (egp->qid)
			dp_sprintf(s, "             qid:                  %d\n",
				   egp->qid);
	}

	/* SPL policy info */
	if (cfg->f_policy) {
		dp_sputs(s, "           Policy:\n");
		dp_sprintf(s, "             num_pkts:             %d\n",
			   cfg->policy[0].num_pkt);
		dp_sprintf(s, "             rx_pkt_size:          %d\n",
			   cfg->policy[0].rx_pkt_size);
		dp_sprintf(s, "             pool_id:              %d\n",
			   cfg->policy[0].pool_id[0]);
		dp_sprintf(s, "             policy_map:           0x%x\n",
			   cfg->policy[0].policy_map);
	}

	/* UMT info - print only if explicitly enabled */
	if (cfg->umt[0].ctl.enable == 1) {
		struct umt_port_ctl *ctl = &cfg->umt[0].ctl;
		struct umt_port_res *res = &cfg->umt[0].res;

		dp_sputs(s, "           UMT port:\n");
		dp_sprintf(s, "             port id:              %d\n",
			   ctl->id);
		dp_sprintf(s, "             dst_addr_cnt:         %u\n",
			   ctl->dst_addr_cnt);
		if (ctl->dst_addr_cnt) {
			dp_sprintf(s, "             dst[]:                ");
			for (i = 0; i < ctl->dst_addr_cnt; i++)
				dp_sprintf(s, "%pad ", &ctl->dst[i]);
			dp_sprintf(s, "\n");
		} else {
			dp_sprintf(s, "             daddr:                "
				   "%pad\n",
				   &ctl->daddr);
		}
		dp_sprintf(s, "             msg_interval:         %d\n",
			   ctl->msg_interval);
		dp_sprintf(s, "             msg_mode:             %d:%s\n",
			   ctl->msg_mode, ctl->msg_mode ? "User" : "Self");
		dp_sprintf(s, "             cnt_mode:             %d:%s count\n",
			   ctl->cnt_mode,
			   ctl->cnt_mode ? "Accumulate" : "Increamental");
		dp_sprintf(s, "             sw_msg:               %d:%s\n",
			   ctl->sw_msg, umt_sw_msg_str[ctl->sw_msg]);
		dp_sprintf(s, "             rx_msg_mode:          %d:Rx %s\n",
			   ctl->rx_msg_mode,
			   ctl->rx_msg_mode ? "IN Add" : "OUT Sub");
		dp_sprintf(s, "             enable:               %d\n",
			   ctl->enable);
		dp_sprintf(s, "             fflag:                0x%lx\n",
			   ctl->fflag);
		dp_sprintf(s, "             dma_id:               %d\n",
			   res->dma_id);
		dp_sprintf(s, "             dma_ch_num:           %d\n",
			   res->dma_ch_num);
		dp_sprintf(s, "             cqm_enq_pid:          %d\n",
			   res->cqm_enq_pid);
		dp_sprintf(s, "             cqm_dq_pid:           %d\n",
			   res->cqm_dq_pid);
		dp_sprintf(s, "             rx_src:               %d:from %s\n",
			   res->rx_src, res->rx_src ? "DMA" : "CQM");
	}
}
#endif

void dump_subif_info(struct seq_file *s, struct pmac_port_info *port, int n)
{
	struct dp_subif_info *sif = get_dp_port_subif(port, n);
	struct dev_mib *mib = get_dp_port_subif_mib(sif);
	print_ctp_bp_t print_ctp_bp = DP_CB(tmp_inst, proc_print_ctp_bp_info);
	struct dma_chan_info *dma;
	struct cqm_port_info *cqm;
	int i, cqm_p, l = 0;
	u8 cid, pid;
	u16 nid;
	char *flag_other;

	if (!sif->flags)
		return;

	flag_other = dp_kzalloc(BUF_SIZE, GFP_ATOMIC);
	if (!flag_other)
		return;

	dp_sprintf(s, "      [%02d]:%s=0x%04x %s=0x%0lx(%s=%s),%s=%s %s=%s\n",
		   n, "subif", sif->subif, "netif", (uintptr_t)sif->netif,
		   "netif", sif->netif ? sif->netif->name : "NULL/DSL",
		   "device_name", sif->device_name,
		   "dp_dev", sif->dp_dev ? sif->dp_dev->dev->name : "");
	dp_sprintf(s, "           subif_flag:     0x%08x", sif->subif_flag);
	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (sif->subif_flag & dp_port_flag[i])
			dp_sprintf(s, " %s ", dp_port_type_str[i]);
	}
	dp_sputs(s, "\n");
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dp_sprintf(s, "           type:           %d (%s)\n", sif->type,
		   dp_port_type[sif->type]);
#endif

	if (sif->data_flag_ops & DP_SUBIF_AUTO_NEW_Q)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_AUTO_NEW_Q");
	if (sif->data_flag_ops & DP_SUBIF_SPECIFIC_Q)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_SPECIFIC_Q");
	if (sif->data_flag_ops & DP_SUBIF_LCT)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_LCT");
	if (sif->data_flag_ops & DP_SUBIF_VANI)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_VANI");
	if (sif->data_flag_ops & DP_SUBIF_DEQPORT_NUM)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_DEQPORT_NUM");
	if (sif->data_flag_ops & DP_SUBIF_RX_FLAG)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_RX_FLAG");
	if (sif->data_flag_ops & DP_SUBIF_SWDEV)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_SWDEV");
	if (sif->data_flag_ops & DP_SUBIF_PREL2)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_PREL2");
	if (sif->data_flag_ops & DP_SUBIF_CPU_QMAP)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_CPU_QMAP");
	if (sif->data_flag_ops & DP_SUBIF_NO_HOSTIF)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_NO_HOSTIF");
	if (sif->data_flag_ops & DP_SUBIF_BR_DOMAIN)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_BR_DOMAIN");
	if (sif->data_flag_ops & DP_SUBIF_NON_CPU_MAC)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_NON_CPU_MAC");
	if (sif->data_flag_ops & DP_SUBIF_BR_DOMAIN)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_BR_DOMAIN");
	if (sif->data_flag_ops & DP_SUBIF_CPU)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_CPU");
	if (sif->data_flag_ops & DP_SUBIF_REINSERT)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_REINSERT");
	if (sif->data_flag_ops & DP_SUBIF_NO_US)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_NO_US");
	if (sif->data_flag_ops & DP_SUBIF_TOE)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SUBIF_TOE");

	dp_sprintf(s, "           data_flag_ops   0x%08x (%s)\n",
		   sif->data_flag_ops, flag_other);
	dp_sprintf(s, "           rx_fn_rxif_pkt  0x%08x\n",
		   STATS_GET(mib->rx_fn_rxif_pkt));
	dp_sprintf(s, "           rx_fn_txif_pkt  0x%08x\n",
		   STATS_GET(mib->rx_fn_txif_pkt));
	dp_sprintf(s, "           rx_fn_dropped   0x%08x\n",
		   STATS_GET(mib->rx_fn_dropped));
	dp_sprintf(s, "           tx_cbm_pkt      0x%08x\n",
		   STATS_GET(mib->tx_cbm_pkt));
	dp_sprintf(s, "           tx_tso_pkt      0x%08x\n",
		   STATS_GET(mib->tx_tso_pkt));
	dp_sprintf(s, "           tx_pkt_dropped  0x%08x\n",
		   STATS_GET(mib->tx_pkt_dropped));
	dp_sprintf(s, "           tx_clone_pkt    0x%08x\n",
		   STATS_GET(mib->tx_clone_pkt));
	dp_sprintf(s, "           tx_hdr_room_pkt 0x%08x\n",
		   STATS_GET(mib->tx_hdr_room_pkt));
	dp_sprintf(s, "           rx_fn           0x%px\n", sif->rx_fn);
	dp_sprintf(s, "           get_subifid_fn  0x%px\n",
		   sif->get_subifid_fn);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	dp_sprintf(s, "           swdev:	   %d (%s)\n", sif->swdev_en,
		   sif->swdev_en ? "Enabled" : "Disabled");
#endif
	dp_sprintf(s, "           domain_id:	   %d (%s:0x%08x)\n",
		   sif->domain_id, "member", sif->domain_members);
	dp_sprintf(s, "           subif_groupid:  %d\n",
		   sif->subif_groupid);
	if (print_ctp_bp)
		print_ctp_bp(s, tmp_inst, port, n, 0);
	dp_sprintf(s, "           num_qid:        %d\n", sif->num_qid);
	dp_sprintf(s, "           dqport_idx/num: %d/%d\n", sif->cqm_port_idx,
		   sif->num_egp);
	for (i = 0; i < sif->num_qid; i++) {
		dp_sprintf(s, "%13s[%02d]:qid/node:    %d/%d\n", "",
			   i, sif->qid_list[i], sif->q_node[i]);
		cqm_p = sif->cqm_deq_port[i];
		if (sif->spl_cfg && sif->spl_cfg->num_egp)
			cqm_p = sif->spl_cfg->egp[i].egp_id;
		cqm = get_dp_deqport_info(tmp_inst, cqm_p);
		dp_sprintf(s, "%18sport/node:   %d/%d(ref=%d)\n", "",
			   cqm_p, sif->qos_deq_port[i], cqm->ref_cnt);
		dp_dma_parse_id(cqm->dma_chan, &cid, &pid, &nid);
		dma = dp_dma_chan_tbl[tmp_inst];
		if (port->num_dma_chan && dma) {
			dma += cqm->dma_ch_offset;
			dp_sprintf(s, "%18stx_dma_ch:   0x%x(ref=%d,dma-ctrl=%d,port=%d,channel=%d)\n",
				   "", cqm->dma_chan,
				   atomic_read(&dma->ref_cnt),
				   cid, pid, nid);
		}
	}
	dp_sprintf(s, "           toe_disable:    %d ", sif->data_toe_disable);
	if (sif->data_flag_ops & DP_SUBIF_TOE)
		dp_sprintf(s, "(%s)\n", sif->data_toe_disable ? "Disabled" : "Enabled");
	else
		dp_sprintf(s, "(%s)\n", g_toe_disable ? "Disabled" : "Enabled");
	dp_sprintf(s, "           toe_tc:         %d\n", sif->toe_tc);
	dp_sprintf(s, "           mac_learn_dis:  %d (%s)\n",
		   sif->mac_learn_dis,
		   sif->mac_learn_dis ? "Disabled" : "Enabled");
	dp_sprintf(s, "           gpid:           %d\n", sif->gpid);
	dp_sprintf(s, "           cpu_port_en:    %d (%s)\n", sif->cpu_port_en,
		   sif->cpu_port_en ? "Enabled" : "Disabled");
	dp_sprintf(s, "           ctp_base:       %d\n", sif->ctp_base);
	dp_sprintf(s, "           ctp:            %d\n", sif->ctp);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dp_sprintf(s, "           dfl_egress_sess=");
	for (i = 0; i < DP_DFL_SESS_NUM; i++)
		dp_sprintf(s, "%d ", sif->dfl_sess[i]);
	dp_sprintf(s, "\n");

	dp_sprintf(s, "           prel2_len:      %d (%s)\n", sif->prel2_len,
		   sif->prel2_len ? "Enabled" : "Disabled");
	dp_sprintf(s, "           spl_conn_type:  %s\n",
		   dp_spl_conn_type[sif->spl_conn_type]);
#endif
	dp_sputs(s, "           ctp_dev:        ");
	if (sif->ctp_dev)
		dp_sputs(s, sif->ctp_dev->name);
	else
		dp_sputs(s, "NULL");
	dp_sputs(s, "\n");
	dp_sprintf(s, "           rx_en_flag:     %d\n",
		   STATS_GET(sif->rx_flag));
	dp_sprintf(s, "           tx_policy base/num/map:      %d/%d/%x\n",
		   sif->tx_policy_base, sif->tx_policy_num, sif->tx_policy_map);
	dp_sprintf(s, "           rx_policy base/num/map:      %d/%d/%x\n",
		   sif->rx_policy_base, sif->rx_policy_num, sif->rx_policy_map);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dp_sprintf(s, "           headroom/tailroom size:      %d/%d\n",
		   sif->headroom_size, sif->tailroom_size);
	dp_sprintf(s, "           spl_gpid(headroom/tailroom) size:      %d/%d\n",
		   sif->spl_gpid_headroom_size, sif->spl_gpid_tailroom_size);
	/* CPU Special path config */
	if (sif->spl_cfg)
		dump_subif_spl_cfg_info(s, sif);
#endif

	kfree(flag_other);
}

void dp_set_tmp_inst(int inst)
{
	tmp_inst = inst;
}

int proc_subif_dump_one(struct seq_file *s, int pos, int subif)
{
	struct pmac_port_info *port = get_dp_port_info(tmp_inst, pos);

	dump_subif_info(s, port, subif);

	return 0;
}

int proc_subif_dump(struct seq_file *s, int pos)
{
	struct pmac_port_info *port;
	int i, start_vap = 0, end_vap;

	port = get_dp_port_info(tmp_inst, pos);

	end_vap = port->subif_max;
	if (dump_one_vap) {
		start_vap = proc_write_vap;
		end_vap = proc_write_vap + 1;
	}

	dp_sprintf(s, "    Num of SIF:        %d\n", port->subif_max);
	for (i = start_vap; i < end_vap; i++)
		proc_subif_dump_one(s, pos, i);

	return 0;
}

int proc_port_dump_one(struct seq_file *s, int pos)
{
	struct pmac_port_info *port;
	struct dp_subif_info *sif;
	int i, l = 0;
	u16 start = 0, nid;
	u8 cid, pid;
	char *flag_other;
	struct cqm_port_info *cqm_info;

	port = get_dp_port_info(tmp_inst, pos);

	DP_CB(tmp_inst, get_itf_start_end)(port->itf_info, &start, NULL);

	if (port->status == PORT_FREE) {
		if (pos == 0) {
			sif = get_dp_port_subif(port, 0);
			dp_sprintf(s, "Reserved Port: %s=0x%08x  %s=0x%08x\n",
				   "rx_err_drop", STATS_GET(port->rx_err_drop),
				   "tx_err_drop", STATS_GET(port->tx_err_drop));
			dp_sprintf(s, "           qid/node:     %d/%d\n",
				   sif->qid, sif->q_node[0]);
			dp_sprintf(s, "           port/node:    %d/%d\n",
				   sif->cqm_deq_port[0], sif->qos_deq_port[0]);
		} else {
			dp_sprintf(s, "%02d: %s=0x%08x  %s=0x%08x\n",
				   pos,
				   "rx_err_drop", STATS_GET(port->rx_err_drop),
				   "tx_err_drop", STATS_GET(port->tx_err_drop));
		}

		return pos;
	}

	dp_sprintf(s, "%02d:%s=0x%0lx(%s:%8s) %s=%02d %s=%02d %s=%d(%s)\n",
		   pos, "module", (uintptr_t)port->owner,
		   "name", module_name(port->owner),
		   "dev_port", port->dev_port,
		   "dp_port", port->port_id,
		   "itf_base", start,
		   port->itf_info ? "Enabled" : "Not Enabled");
	dp_sprintf(s, "    status:            %s\n",
		   dp_port_status_str[port->status]);
	dp_sputs(s, "    allocate_flags:    ");
	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (port->alloc_flags & dp_port_flag[i])
			dp_sprintf(s, "%s ", dp_port_type_str[i]);
	}
	dp_sprintf(s, " dp_port: %d", pos);
	dp_sputs(s, "\n");
	if (get_dp_port_prop(tmp_inst)->info.type < GSWIP32_TYPE)
		dp_sprintf(s, "    Lookup mode:       %d [%s]\n",
			   port->cqe_lu_mode,
			   cqm_lookup_mode_fmx[port->cqe_lu_mode]);
	else
		dp_sprintf(s, "    Lookup mode:       %d [%s]\n",
			   port->cqe_lu_mode,
			   cqm_lookup_mode_lgm[port->cqe_lu_mode]);

	dp_sprintf(s, "    gswip mode:        %d [%s]\n", port->gsw_mode,
		   gsw_port_mode[port->gsw_mode]);
	dp_sprintf(s, "    Port owned by:     %d (%s)\n", port->type,
		   dp_port_type[port->type]);
	dp_sprintf(s, "    n_wib_credit_pkt:  %d\n", port->n_wib_credit_pkt);
	dp_sprintf(s, "    LCT:               %d\n", port->lct_idx);
	dp_sprintf(s, "    ctp_max:           %d\n", port->ctp_max);
	dp_sprintf(s, "    subif_max:         %d\n", port->subif_max);
	dp_sprintf(s, "    num_subif:         %d\n", port->num_subif);
	dp_sprintf(s, "    gpid_num:          %d\n", port->gpid_num);
	dp_sprintf(s, "    gpid_base/spl:     %d/%d\n", port->gpid_base,
			port->gpid_spl);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	dp_sprintf(s, "    port_swdev:        %d (%s)\n", port->swdev_en,
		   port->swdev_en ? "Enabled" : "Disabled");
#endif
	dp_sprintf(s, "    cb->rx_fn:         0x%px\n", port->cb.rx_fn);
	dp_sprintf(s, "    cb->restart_fn:    0x%px\n", port->cb.restart_fn);
	dp_sprintf(s, "    cb->stop_fn:       0x%px\n", port->cb.stop_fn);
	dp_sprintf(s, "    cb->get_subifid_fn:0x%px\n",
		   port->cb.get_subifid_fn);
	dp_sprintf(s, "    vap_offset/mask:   %d/0x%x\n", port->vap_offset,
		   port->vap_mask);
	dp_sprintf(s, "    bp_hairpin_cap:    %d\n", port->bp_hairpin_cap);
	flag_other = dp_kzalloc(BUF_SIZE, GFP_ATOMIC);
	if (!flag_other)
		return -ENOBUFS;

	if (port->flag_other & CBM_PORT_DP_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DP_SET");
	if (port->flag_other & CBM_PORT_DQ_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DQ_SET");
	if (port->flag_other & CBM_PORT_DMA_CHAN_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "DMA_CHAN_SET");
	if (port->flag_other & CBM_PORT_PKT_CRDT_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "PKT_CRDT_SET");
	if (port->flag_other & CBM_PORT_BYTE_CRDT_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "BYTE_CRDT_SET");
	if (port->flag_other & CBM_PORT_RING_ADDR_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "RING_ADDR_SET");
	if (port->flag_other & CBM_PORT_RING_SIZE_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "RING_SIZE_SET");
	if (port->flag_other & CBM_PORT_RING_OFFSET_SET)
		l += snprintf(flag_other + l, BUF_SIZE - l, "%s | ", "RING_OFFSET_SET");

	dp_sprintf(s, "    cbm_port_flags:    0x%x %s%s%s\n", port->flag_other,
					port->flag_other ? "[":"",
					flag_other,
					port->flag_other ? "]":"");

	kfree(flag_other);

	dp_sprintf(s, "    num_resv_queue:    %d\n", port->num_resv_q);
	dp_sprintf(s, "    resv_queue_base:   %d\n", port->res_qid_base);
	dp_sprintf(s, "    deq_port_base:     %d\n", port->deq_port_base);
	dp_sprintf(s, "    deq_port_num:      %d\n", port->deq_port_num);

	/* print first 4 dequeue ports in the array */
	dp_sprintf(s, "    deq_ports (1st 8): ");
	for (i = 0; (i < port->deq_port_num) && (i < 8); i++)
		dp_sprintf(s, "%d ", port->deq_ports[i]);
	dp_sprintf(s, "\n");

	dp_sprintf(s, "    num_dma_chan:      %d\n", port->num_dma_chan);
	dp_dma_parse_id(port->dma_chan, &cid, &pid, &nid);
	if (port->num_dma_chan)
		dp_sprintf(s, "    dma_chan:          "
				"0x%x(dma-ctrl=%d,port=%d,channel=%d)\n",
			   port->dma_chan, cid, pid, nid);
	dp_sprintf(s, "    dma_chan_tbl_idx:  %d\n", port->dma_chan_tbl_idx);
	dp_sprintf(s, "    PTP:               %d (%s)\n", port->f_ptp,
		   port->f_ptp ? "Enabled" : "Disabled");
	dp_sprintf(s, "    loop_dis:          %d\n", port->loop_dis);
	dp_sprintf(s, "    tx_policy_base/num:%d/%d\n",
		   port->tx_policy_base, port->tx_policy_num);
	dp_sprintf(s, "    rx_policy_base/num:%d/%d\n",
		   port->rx_policy_base, port->rx_policy_num);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dp_sprintf(s, "    spl_tx_policy_base/num/map:%d/%d/%x\n",
		   port->spl_tx_policy_base, port->spl_tx_policy_num,
		   port->spl_tx_policy_map);
	dp_sprintf(s, "    spl_rx_policy_base/num/map:%d/%d/%x\n",
		   port->spl_rx_policy_base, port->spl_rx_policy_num,
		   port->spl_rx_policy_map);
#endif
	dp_sprintf(s, "    tx_pkt_credit:     %d\n", port->tx_pkt_credit);
	dp_sprintf(s, "    tx_b_credit:       %02d\n", port->tx_b_credit);
	dp_sprintf(s, "    txpush_addr:       0x%px\n", port->txpush_addr);
	dp_sprintf(s, "    txpush_addr_qos:   0x%px\n", port->txpush_addr_qos);
	dp_sprintf(s, "    tx_ring_size:      %d\n", port->tx_ring_size);
	dp_sprintf(s, "    tx_ring_offset:    %d(to next dequeue port)\n",
		   port->tx_ring_offset);

	if (port->num_rx_ring || port->num_tx_ring)
		dump_dc_info(s, port);

	dp_sprintf(s, "    rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
		   STATS_GET(port->rx_err_drop),
		   STATS_GET(port->tx_err_drop));

	if (pos == 0 && reinsert_deq_port[tmp_inst]) {
		cqm_info = get_dp_deqport_info(tmp_inst,
					       reinsert_deq_port[tmp_inst]);
		dp_sprintf(s, "    insertion_qid:       %d\n",
			   cqm_info->qid[0]);
		dp_sprintf(s, "    insertion_deqport:   %d\n",
			   reinsert_deq_port[tmp_inst]);
		dp_dma_parse_id(cqm_info->dma_chan, &cid, &pid, &nid);
		dp_sprintf(s, "%s0x%x(%s=%d,%s=%d,%s=%d)\n",
			   "    insertion_tx_dma_ch: ", cqm_info->dma_chan,
			   "dma-ctrl", cid, "port", pid, "channel", nid);
	}

	if (port->dts_qos)
		dp_sprintf(s, "    dts_qos       :    node_name=%s category=%s index=%d/%d\n",
			   port->dts_qos->node_name,
			   port->dts_qos->category,
			   port->dts_qos->category_idx,
			   port->dts_qos->sub_category_idx);
	else
		dp_sprintf(s, "    dts_qos       :    NULL\n");

	return pos;

}

int proc_port_dump(struct seq_file *s, int pos)
{

	struct inst_info *info = get_dp_prop_info(tmp_inst);
	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if ((proc_write_port != PROC_WRITE_PORT_DEF) && (pos != proc_write_port))
		goto done;
	proc_port_dump_one(s, pos);
	proc_subif_dump(s, pos);

	/*To be printed after all ports*/
	if (dump_one_port || (pos == info->cap.max_num_dp_ports -1)) {
		dp_sprintf(s, "q_map info:\n");
		dp_sprintf(s, "  dflt_cpu_vap     =%d\n",
			   dflt_cpu_vap[tmp_inst]);
		dp_sprintf(s, "  reinsert_deq_port=%d\n",
			   reinsert_deq_port[tmp_inst]);
		dp_sprintf(s, "  dflt_q_cpu       =%d\n",
			   dflt_q_cpu[tmp_inst]);
		dp_sprintf(s, "  dflt_q_toe       =%d\n",
			   dflt_q_toe[tmp_inst]);
		dp_sprintf(s, "  dflt_q_re_insert =%d\n",
			   dflt_q_re_insert[tmp_inst]);
		dp_sprintf(s, "  dflt_q_drop      =%d\n",
			   dflt_q_drop[tmp_inst]);
		dp_sprintf(s, "  sif_reset_f      =0x%x\n",
			   get_subif_q_map_reset_flag(tmp_inst));
		dp_sprintf(s, "  dflt_bp          =%d\n",
			   dflt_bp[tmp_inst]);
		dp_sprintf(s, "g_toe_disable      =%d\n",
			   g_toe_disable);
		dp_sprintf(s, "version info: %d.%d.%d.%s\n",
			   DP_VER_MAJ, DP_VER_MID,
			   DP_VER_MIN, DP_MACRO_STR(DP_VER_TAG));
		dp_sputs(s, "switchdev: v2\n");
	}

done:
	if (!dp_seq_has_overflowed(s)) {
		pos++;
		if (pos >= info->cap.max_num_dp_ports || dump_one_port) {
			tmp_inst++;
			pos = 0;
		}
		if (tmp_inst >= dp_inst_num) {
			pos = -1;	/*end of the loop */
			dump_one_port = false;
			proc_write_port = PROC_WRITE_PORT_DEF;
		}
	}
	return pos;
}

static
void show_port_info(int inst, u8 pos, int vap_start, int vap_end, u32 flag)
{
	struct pmac_port_info *port = get_dp_port_info(inst, pos);
	u16 start;
	int i;

	if (port->status == PORT_FREE) {
		if (!pos)
			pr_info("%s:rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
				"Reserved Port",
				STATS_GET(port->rx_err_drop),
				STATS_GET(port->tx_err_drop));

		else
			pr_info("%02d:rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
				pos,
				STATS_GET(port->rx_err_drop),
				STATS_GET(port->tx_err_drop));

		return;
	}

	DP_CB(tmp_inst, get_itf_start_end)(port->itf_info, &start, NULL);

	pr_info("%02d: %s=0x0x%0lx(name:%8s) %s=%02d %s=%02d itf_base=%d(%s)\n",
		pos,
		"module", (uintptr_t)port->owner, port->owner->name,
		"dev_port", port->dev_port, "dp_port", port->port_id,
		start, port->itf_info ? "Enabled" : "Not Enabled");
	pr_info("    status:            %s\n",
		dp_port_status_str[port->status]);
	pr_info("    allocate_flags:    ");

	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (port->alloc_flags & dp_port_flag[i])
			pr_info("%s ", dp_port_type_str[i]);
	}

	pr_info("\n");

	if (!flag) {
		pr_info("    cb->rx_fn:         0x%0lx\n",
			(uintptr_t)port->cb.rx_fn);
		pr_info("    cb->restart_fn:    0x%0lx\n",
			(uintptr_t)port->cb.restart_fn);
		pr_info("    cb->stop_fn:       0x%0lx\n",
			(uintptr_t)port->cb.stop_fn);
		pr_info("    cb->get_subifid_fn:0x%0lx\n",
			(uintptr_t)port->cb.get_subifid_fn);
		pr_info("    num_subif:         %02d\n", port->num_subif);
	}

	for (i = vap_start; i < vap_end; i++) {
		struct dp_subif_info *sif = get_dp_port_subif(port, i);
		struct dev_mib *mib = get_dp_port_subif_mib(sif);

		if (sif->flags) {
			pr_info("      [%02d]:%s=0x%04x %s=0x%0lx(%s=%s),%s=%s\n",
				i, "subif", sif->subif,
				"netif", (uintptr_t)sif->netif,
				"device_name",
				sif->netif ? sif->netif->name : "NULL/DSL",
				"name", sif->device_name);
			pr_info("          : rx_fn_rxif_pkt =0x%08x\n",
				STATS_GET(mib->rx_fn_rxif_pkt));
			pr_info("          : rx_fn_txif_pkt =0x%08x\n",
				STATS_GET(mib->rx_fn_txif_pkt));
			pr_info("          : rx_fn_dropped  =0x%08x\n",
				STATS_GET(mib->rx_fn_dropped));
			pr_info("          : tx_cbm_pkt     =0x%08x\n",
				STATS_GET(mib->tx_cbm_pkt));
			pr_info("          : tx_tso_pkt     =0x%08x\n",
				STATS_GET(mib->tx_tso_pkt));
			pr_info("          : tx_pkt_dropped =0x%08x\n",
				STATS_GET(mib->tx_pkt_dropped));
			pr_info("          : tx_clone_pkt   =0x%08x\n",
				STATS_GET(mib->tx_clone_pkt));
			pr_info("          : tx_hdr_room_pkt=0x%08x\n",
				STATS_GET(mib->tx_hdr_room_pkt));
		}
	}

	pr_info("    rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
		STATS_GET(port->rx_err_drop), STATS_GET(port->tx_err_drop));
}

static int set_switchdev(int inst, int ep, int vap, bool en)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	struct pmac_port_info *port = get_dp_port_info(inst, ep);
	struct dp_subif_info *sif = get_dp_port_subif(port, vap);
	struct net_device *dev = sif->netif;
	struct net_device *br_dev;
	struct dp_dev *dp_dev;

	if (port->status != PORT_SUBIF_REGISTERED || !sif->flags) {
		pr_err("DPM: subif not registered\n");
		return DP_FAILURE;
	}

	if (!sif->swdev_en == !en)
		return DP_SUCCESS;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: dp_dev lookup failed\n");
		return DP_FAILURE;
	}

	rtnl_lock();
	br_dev = netdev_master_upper_dev_get(dev);
	rtnl_unlock();

	if (en) {
		sif->swdev_en = 1;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
		if (dp_register_switchdev_ops(dp_dev->dev, 0)) {
			pr_err("DPM: fail to register swdev ops\n");
			return DP_FAILURE;
		}
#endif
		pr_info("swdev is enabled\n");
		if (br_dev) {
			pr_info("master upper device (bridge?) detected\n");
			pr_info("please reinit network for switchdev to be effective\n");
		}
	} else {
		if (br_dev) {
			pr_err("DPM: master upper device (bridge?) detected\n");
			pr_err("DPM: please remove link to upper dev before disabling switchdev\n");
			return DP_FAILURE;
		}
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
		if (dp_register_switchdev_ops(dp_dev->dev, 1)) {
			pr_err("DPM: fail to deregister swdev ops\n");
			return DP_FAILURE;
		}
#endif
		sif->swdev_en = 0;
		pr_info("swdev is disabled\n");
	}
#endif
	return DP_SUCCESS;
}

ssize_t proc_port_write(struct file *file, const char *buf, size_t count,
			loff_t *ppos)
{
	struct local {
		int inst;
		int i;
		int num;
		int idx_s;
		int idx_e;
		int vap_s;
		int vap_e;
		int swdev_en;
	};
	struct inst_info *info = &dp_port_prop[0].info;
	struct pmac_port_info *port;
	char *param_list[10] = {NULL}, *str, *p;
	struct local *l;
	int ret;

	if (!capable(CAP_SYS_PACCT))
		return count;

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return -ENOMEM;
	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str) {
		ret = -ENOMEM;
		goto free_1;
	}
	if (dp_copy_from_user(str, buf, count)) {
		ret = -EFAULT;
		goto free_2;
	}

	p = str;
	for (l->num = 0; l->num < ARRAY_SIZE(param_list); l->num++) {
		if (!strlen(p))
			break;
		param_list[l->num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}

	l->idx_s = 0;
	l->idx_e = info->cap.max_num_dp_ports;
	
	if (param_list[1]) {
		l->idx_s = dp_atoi(param_list[1]);
		if (l->idx_s >= info->cap.max_num_dp_ports) {
			pr_err("DPM: invalid port index, expect 0 ~ %d\n",
			       info->cap.max_num_dp_ports - 1);
			ret = count;
			goto free_2;
		}
		if (l->idx_s < 0) {
			l->idx_s = 0;
			l->idx_e = info->cap.max_num_dp_ports;
		} else {
			l->idx_e = l->idx_s + 1;
		}
	}

	l->vap_s = 0;
	l->vap_e = info->cap.max_num_subif_per_port;

	if (param_list[2]) {
		l->vap_s = dp_atoi(param_list[2]);
		if (l->vap_s >= info->cap.max_num_subif_per_port) {
			pr_err("DPM: invalid VAP index, expect 0 ~ %d\n",
			       info->cap.max_num_subif_per_port - 1);
			ret = count;
			goto free_2;
		}
		if (l->vap_s < 0) {
			l->vap_s = 0;
			l->vap_e = info->cap.max_num_subif_per_port;
		} else {
			l->vap_e = l->vap_s + 1;
		}
	}

	if (!strcasecmp(param_list[0], "mib")) {
		for (l->inst = 0; l->inst < dp_inst_num; l->inst++)
			for (l->i = l->idx_s; l->i < l->idx_e; l->i++)
				show_port_info(l->inst, l->i, l->vap_s, l->vap_e, 1);
	} else if (!strcasecmp(param_list[0], "port")) {
		if (proc_write_port != dp_atoi(param_list[1])) {
			proc_write_port = l->idx_s;
			pr_info("%s: changed port id to be dumped to %d\n",
				__func__, proc_write_port);
		}

		if (param_list[2] && (proc_write_vap != dp_atoi(param_list[2])))
		{
			proc_write_vap = l->vap_s;
			pr_info("%s: changed sif id to be dumped to %d\n",
				__func__, proc_write_vap);
		}

		pr_info("%s: run 'cat %s' to dump requested port info\n",
			__func__, DBG_PORT);
	} else if (!strcasecmp(param_list[0], "swdev") && l->num == 4) {
		DP_LIB_LOCK(&dp_lock);
		port = get_dp_port_info(l->inst, l->idx_s);
		if (!port->num_subif) {
			pr_err("DPM: subif unavailable for port index %d\n", l->idx_s);
		} else if (l->vap_s >= port->num_subif) {
			pr_err("DPM: invalid VAP index, expect 0 ~ %d\n",
			       port->num_subif - 1);
		} else {
			l->swdev_en = dp_atoi(param_list[3]);
			set_switchdev(0, l->idx_s, l->vap_s, !!l->swdev_en);
		}
		DP_LIB_UNLOCK(&dp_lock);
	} else {
		pr_info("usage:\n");
		pr_info("  echo mib   [ep][vap] > %s\n", DBG_PORT);
		pr_info("  echo port  [ep][vap] > %s\n", DBG_PORT);
		pr_info("    to print all port info: echo port -1 > %s\n",
			DBG_PORT);
		pr_info("  echo swdev [ep][vap][swdev_en] > %s\n", DBG_PORT);
	}

	ret = count;

free_2:
	kfree(str);
free_1:
	kfree(l);
	return ret;
}

ssize_t proc_ctp_list_write(struct file *file, const char *buf, size_t count,
			    loff_t *ppos)
{
	char *param_list[5] = {NULL}, *str, *p;
	int num, ret;
	u16 bp;

	if (!capable(CAP_SYS_PACCT))
		return count;

	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return -ENOMEM;

	if (dp_copy_from_user(str, buf, count)) {
		ret = -EFAULT;
		goto free;
	}

	p = str;
	for (num = 0; num < ARRAY_SIZE(param_list); num++) {
		if (!strlen(p))
			break;
		param_list[num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}

	if (param_list[1])
		bp = dp_atoi(param_list[1]);
	else
		bp = 0;

	if (!strcasecmp(param_list[0], "bp_ctp")) {
		struct ctp_dev *ctp_dev;
		struct bp_pmapper *bp_info;

		bp_info = get_dp_bp_info(0, bp);
		pr_info("%-10s%-10d\n", "Bp", bp);

		if (!bp_info->ref_cnt) {
			pr_info("dev(%s) is not a pmapper\n",
				bp_info->dev ? bp_info->dev->name : "NULL");
			ret = count;
			goto free;
		}

		list_for_each_entry(ctp_dev, &bp_info->ctp_dev, list) {
			if (!ctp_dev->dev)
				continue;
			pr_info("%-s:%-s(%-s:%-d)\n", "ctp_dev",
				ctp_dev->dev->name, "ctp", ctp_dev->ctp);
		}
	} else {
		pr_info("usage:\n");
		pr_info("  echo bp_ctp [bp] > %s\n", DBG_CTP);
	}

	ret = count;
free:
	kfree(str);
	return ret;
}

#if defined(CONFIG_DPM_DATAPATH_DBG) && CONFIG_DPM_DATAPATH_DBG
void proc_dbg_read(struct seq_file *s)
{
	int i;

	if (!capable(CAP_SYS_ADMIN))
		return;

	dp_sprintf(s, "dp_dbg_flag=0x%llx\n", (u64)dp_dbg_flag);
	dp_sprintf(s, "Supported Flags =%d\n", get_dp_dbg_flag_str_size()-1);
	dp_sprintf(s, "dp_dbgfs_flag=0x%llx\n", (u64)dp_dbgfs_flag);
	dp_sprintf(s, "Supported Flags =%d\n", get_dp_dbgfs_flag_str_size()-1);

	dp_sprintf(s, "Enabled DBG Flags(0x%0llx):", (u64)dp_dbg_flag);
	for (i = 0; i < get_dp_dbg_flag_str_size(); i++)
		if ((dp_dbg_flag & dp_dbg_flag_list[i]) == dp_dbg_flag_list[i])
			dp_sprintf(s, "%s ", dp_dbg_flag_str[i]);

	dp_sprintf(s, "\nEnabled DBGFS Flags(0x%0llx):", (u64)dp_dbgfs_flag);
	for (i = 0; i < get_dp_dbgfs_flag_str_size(); i++)
		if ((dp_dbgfs_flag & dp_dbgfs_flag_list[i]) == dp_dbgfs_flag_list[i])
			dp_sprintf(s, "%s ", dp_dbgfs_flag_str[i]);

	dp_sputs(s, "\n\n");
	dp_sprintf(s, "dp_dbg_mode=%d(0-pr_info 1-trace_printk)\n", dp_dbg_mode);
	dp_sprintf(s, "dp_drop_all_tcp_err=%d @ 0x%px\n", dp_drop_all_tcp_err,
		   &dp_drop_all_tcp_err);
	dp_sprintf(s, "dp_pkt_size_check=%d @ 0x%px\n", dp_pkt_size_check,
		   &dp_pkt_size_check);
	dp_sprintf(s, "dp_dbg_err(flat to print error or not)=%d @ 0x%px\n",
		   dp_dbg_err,
		   &dp_dbg_err);
	seq_printf(s, "dpm gcc optimization is %s\n",
		   dp_dsable_optimize ? "disabled" : "enabled");

	print_parser_status(s);
}

ssize_t proc_dbg_write(struct file *file, const char *buf, size_t count,
		       loff_t *ppos)
{
	struct {
		int i;
		int j;
		int num;
		int dbg_str_list;
		int dbgfs_str_list;
		int en;
		char *param_list[20];
		char *p1, str[120];
		char tmp[400];
		int idx;
	} *p;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return -ENOMEM;

	if (dp_copy_from_user(p->str, buf, count)) {
		kfree(p);
		return -EFAULT;
	}
	p->p1 = p->str;
	for (p->num = 0; p->num < ARRAY_SIZE(p->param_list); p->num++) {
		if (!strlen(p->p1))
			break;
		p->param_list[p->num] = dp_strsep(&p->p1, ",: \n");
		if (!p->p1)
			break;
	}

	p->dbg_str_list = get_dp_dbg_flag_str_size() - 1;
	p->dbgfs_str_list = get_dp_dbgfs_flag_str_size() - 1;

	if (!strcasecmp(p->param_list[0], "enable")) {
		p->en = 1;
	} else if (!strcasecmp(p->param_list[0], "disable")) {
		p->en = -1;
	} else if (!strcasecmp(p->param_list[0], "mode")) {
		dp_dbg_mode = dp_atoi(p->param_list[1]);
		goto exit;
	}  else {
		p->idx = scnprintf(p->tmp, sizeof(p->tmp), "%s",
				"echo <enable/disable> ");
		for (p->i = 0; p->i < p->dbg_str_list; p->i++)
			p->idx += scnprintf(p->tmp + p->idx,
					    sizeof(p->tmp) - p->idx - 1, "%s ",
					    dp_dbg_flag_str[p->i]);
		for (p->i = 0; p->i < p->dbgfs_str_list; p->i++)
			p->idx += scnprintf(p->tmp + p->idx,
					    sizeof(p->tmp) - p->idx - 1, "%s ",
					    dp_dbgfs_flag_str[p->i]);
		p->idx += scnprintf(p->tmp + p->idx,
				    sizeof(p->tmp) - p->idx - 1,
				    " > %s\n", DEBUGFS_DBG);
		pr_info("%s", p->tmp);
		pr_info("idx=%d", p->idx);
		pr_info("echo <mode> <0/1>\n");
		pr_info("  0-pr_info 1--trace_printk\n");
		goto exit;
	}

	if (!(p->param_list[1])) {
		set_ltq_dbg_flag(dp_dbg_flag, p->en, -1);
		/*Force clear DP_DBGFS_FLAG_XXX flags if just disable given*/
		if (p->en < 0)
			set_ltq_dbg_flag(dp_dbgfs_flag, p->en, -1);
		goto exit;
	}

	for (p->i = 1; p->i < p->num; p->i++) {
		for (p->j = 0; p->j < p->dbg_str_list; p->j++)
			if (!strcasecmp(p->param_list[p->i],
					dp_dbg_flag_str[p->j])) {
				set_ltq_dbg_flag(dp_dbg_flag, p->en,
						 dp_dbg_flag_list[p->j]);
				break;
			}
		if (p->j < p->dbg_str_list)
			continue;
		for (p->j = 0; p->j < p->dbgfs_str_list; p->j++)
			if (!strcasecmp(p->param_list[p->i],
					dp_dbgfs_flag_str[p->j])) {
				set_ltq_dbg_flag(dp_dbgfs_flag, p->en,
						 dp_dbgfs_flag_list[p->j]);
				break;
			}

	}

exit:
	kfree(p);

	return count;
}
#endif

int proc_dpid_dump(struct seq_file *s, int pos)
{
	int tmp_inst = 0;
	struct inst_info *info = get_dp_prop_info(tmp_inst);
	struct pmac_port_info *port;
	struct dp_subif_info *sif;
	u16 ctp_s, ctp_e;
	int qid_cont = 1, deq_cont = 1;
	int i, j;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	port = get_dp_port_info(tmp_inst, pos);
	DP_CB(tmp_inst, get_itf_start_end)(port->itf_info, &ctp_s, &ctp_e);

	if (pos == 0) {
		dp_sprintf(s, "%4s %5s %3s %4s %4s/%-6s %3s %3s %11s %s\n",
			   "dpid", "subif", "grp", "gpid", "qid", "deq[0]",
			   "ctp", "bp", "comment", " extraInfo");
		dp_sprintf(s, "%4s %5s %3s %4s %5s/%-5s %3s %3s %11s\n",
			   "Dec", "Hex", "Dec", "Dec", "Dec", "Dec",
			   "Dec", "Dec", "");
	}
	for (i = 0; i < port->subif_max; i++) {
		sif = get_dp_port_subif(port, i);
		if (!sif->flags)
			continue;
		qid_cont = 1;
		deq_cont = 1;
		for (j = 1; j < sif->num_qid; j++) {
			if (qid_cont && ((sif->qid + j) != sif->qid_list[j])) {
				qid_cont = 0;
			}
			if (deq_cont && ((sif->cqm_deq_port[0] + j) != sif->cqm_deq_port[j])) {
				deq_cont = 0;
			}
			if (!deq_cont && !qid_cont)
				break;
		}
		/* pos subif subif_grp */
		dp_sprintf(s, "%4d %5x %3d ",
			   pos,
			   sif->subif,
			   GET_VAP(sif->subif,port->vap_offset,port->vap_mask));

		/* gpid */
		if (sif->gpid != (u16)(-1))
			dp_sprintf(s, "%4d ", sif->gpid);
		else
			dp_sprintf(s, "%4s ", "-");

		/* qid/deq */
		if (sif->num_qid) {
			dp_sprintf(s, "%4u /", sif->qid);

			if (sif->spl_cfg && sif->spl_cfg->num_egp)
				dp_sprintf(s, " %-4u ",
					   sif->spl_cfg->egp[0].egp_id);
			else
				dp_sprintf(s, " %-4u ", sif->cqm_deq_port[0]);
		} else {
			dp_sprintf(s, "%4s /", "-");
			dp_sprintf(s, " %-4s ", "-");
		}

		/* ctp bp */
		dp_sprintf(s, "%3u %3u ",
			   (ctp_s + i < ctp_e) ? (ctp_s + i) : ctp_e,
			   sif->bp);
		/* comment or dev->name */
		if (sif->netif)
			dp_sprintf(s, "%11s", sif->netif->name);
		else if (sif->spl_cfg)
			dp_sprintf(s, "%11s",
				   dp_spl_conn_type[sif->spl_cfg->type]);
		else if (port->alloc_flags & DP_F_CPU)
			dp_sprintf(s, "%11s",
				   dp_port_type[sif->type]);
		else if (port->alloc_flags & DP_F_FAST_DSL)
			dp_sprintf(s, "%11s", "DSL");
		else
			dp_sprintf(s, "%11s", "-");

		if (sif->num_qid > 1) {
			if (!qid_cont || !deq_cont) {
				if (sif->num_qid > 2)
					dp_sprintf(s, "  qid/deq[1~%d]: ", sif->num_qid-1);
				else
					dp_sprintf(s, "  qid/deq[1]: ");
				for (j = 1; j < sif->num_qid; j++) {
					dp_sprintf(s, "%u/", sif->qid_list[j]);
					if (sif->spl_cfg && sif->spl_cfg->num_egp)
						dp_sprintf(s, "%u, ",
							   sif->spl_cfg->egp[j].egp_id);
					else
						dp_sprintf(s, "%u, ",
							   sif->cqm_deq_port[j]);
				}
			} else {
				dp_sprintf(s, "  qid/deq[%d]: %u/", sif->num_qid-1,
						sif->qid_list[sif->num_qid-1]);
					if (sif->spl_cfg && sif->spl_cfg->num_egp)
						dp_sprintf(s, "%u",
							   sif->spl_cfg->egp[sif->num_qid-1].egp_id);
					else
						dp_sprintf(s, "%u",
							   sif->cqm_deq_port[sif->num_qid-1]);
			}
		} else {
			dp_sputs(s, "    -");
		}
		dp_sputs(s, "\n");
	}
	if (port->gpid_spl > 0) {
		dp_sprintf(s, "%4d %5s %3s %4u %4s / %-4s %3s %3s %11s %s\n",
			   pos,
			   "-",
			   "-",
			   port->gpid_spl,
			   "-",
			   "-",
			   "-",
			   "-",
			   "SPL_GPID", "   -");
	}
	if (pos == info->cap.max_num_dp_ports - 1) {
		/* print title again in order to avoid scroll the console */
		dp_sprintf(s, "%4s %5s %3s %4s %4s/%-6s %3s %3s %11s %s\n",
			   "dpid", "subif", "grp", "gpid", "qid", "deq[0]",
			   "ctp", "bp", "comment", " extraInfo");
	}
	if (dp_seq_has_overflowed(s))
		return pos;
	pos++;
	if (pos == info->cap.max_num_dp_ports)
		pos = -1; /* end of the loop */

	return pos;
}

int proc_registration_qid_dump(struct seq_file *s, int pos)
{
	int tmp_inst = 0;
	struct q_info *q = get_dp_q_info(tmp_inst, pos);

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (!q->flag)
		goto next;
	dp_sprintf(s, "qid:%d\n", pos);
	dp_sprintf(s, "  need_free   =%d\n", q->need_free);
	dp_sprintf(s, "  q_node_id   =%d\n", q->q_node_id);
	dp_sprintf(s, "  ref_cnt     =%d\n", q->ref_cnt);
	dp_sprintf(s, "  cqm_deq_port=%d\n", q->cqm_dequeue_port);
	if (dp_seq_has_overflowed(s))
		return pos;
next:
	pos++;
	if (pos >= DP_MAX_QUEUE_NUM) {
		pos = -1; /* end of the loop */
		/* print title again in order to avoid scroll the console */
		dp_sprintf(s, "%4s %6s %3s %4s %4s %8s %3s %3s %16s\n",
			   "dpid", "subif", "grp", "gpid", "qid", "deq_port",
			   "ctp", "bp", "comment");
	}

	return pos;
}

/* For PRX, there is no rxin_ring. So no CQM enq port*/
int proc_registration_enq_port_dump(struct seq_file *s, int pos)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	int tmp_inst = 0, i, j;
	struct cqm_igp_info *enq = get_dp_enqport_info(tmp_inst, pos);

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if (!enq->ref_cnt)
		goto next;

	dp_sprintf(s, "enq_port: %3d (ref=%2d)\n", pos, enq->ref_cnt);
	dp_sprintf(s, "  %-30s: %d\n", "DC dummy read", enq->dc_dummy_read);
	dp_sprintf(s, "  %-30s:", "DP port ids(id/ref)");
	for (i = 0; i < ARRAY_SIZE(enq->dp_port); i++) {
		if (enq->dp_port[i])
			dp_sprintf(s, " %d/%d", i, enq->dp_port[i]);
	}
	dp_sputs(s, "\n");

	dp_sprintf(s, "  %-30s:", "DC port ids(DC_idx/DP_id)");
	for (i = 0; i < ARRAY_SIZE(enq->dp_port); i++) {
		for (j = 0; j < DP_DC_NUM; j++) {
			if (enq->dc_port[i][j])
				dp_sprintf(s, " %d/%d", j, i);
		}
	}
	dp_sputs(s, "\n");

	dp_sprintf(s, "  %-30s:", "UMT port ids(id/ref)");
	for (i = 0; i < ARRAY_SIZE(enq->umt_port); i++) {
		if (enq->umt_port[i])
			dp_sprintf(s, " %d/%d", i, enq->umt_port[i]);
	}
	dp_sputs(s, "\n");

next:
	pos++;
	if (pos >= DP_MAX_CQM_IGP)
		pos = -1; /* end of the loop */

	return pos;
#else
	return -1;
#endif
}

void dp_dump_one_deq_port(struct seq_file *s, int deq_port,
		struct cqm_port_info *deq)
{
	int i, tmp_inst = 0;
	int dma_ref = atomic_read(&(dp_dma_chan_tbl[tmp_inst] +
				    deq->dma_ch_offset)->ref_cnt);
	if (deq->f_dma_ch)
		dp_sprintf(s, "deq_port:%3d(ref=%2d):dma offset/ch/ref=%02d/"
			   "%07x/%2d\n",
			   deq_port, deq->ref_cnt,
			   deq->dma_ch_offset,
			   deq->dma_chan, dma_ref);
	else
		dp_sprintf(s, "deq_port: %3d (ref=%2d)\n",
			   deq_port, deq->ref_cnt);
	for (i = 0; i < DP_MAX_UMT; i++)
		if (deq->umt_info[i].umt_valid)
			dp_sprintf(s, "  UMT%d_ref_cnt_umt    : %d\n", i,
				   deq->umt_info[i].ref_cnt_umt);
	dp_sprintf(s, "  %-20s: %d\n", "f_first_qid", deq->f_first_qid);
	dp_sprintf(s, "  %-20s: %d\n", "first_qid", deq->first_qid);
	dp_sprintf(s, "  %-20s: ", "qid");
	for (i = 0; i < ARRAY_SIZE(deq->qid); i++)
		dp_sprintf(s, "%d ", deq->qid[i]);
	dp_sputs(s, "\n");
	dp_sprintf(s, "  %-20s: %d\n", "q_node", deq->q_node);
	dp_sprintf(s, "  %-20s:", "dp_ports (ids)");
	for (i = 0; i < ARRAY_SIZE(deq->dp_port); i++)
		if(deq->dp_port[i])
			dp_sprintf(s, " %d", i);
	dp_sputs(s, "\n");
	dp_sprintf(s, "  %-20s: %d\n", "tx_pkt_credit", deq->tx_pkt_credit);
	dp_sprintf(s, "  %-20s: 0x%px\n", "txpush_addr", deq->txpush_addr);
	dp_sprintf(s, "  %-20s: 0x%px\n", "txpush_addr_qos", deq->txpush_addr_qos);
	dp_sprintf(s, "  %-20s: %d\n", "tx_ring_size", deq->tx_ring_size);
	if (deq->dts_qos)
		dp_sprintf(s, "  %-20s: node_name=%s category=%s index=%d/%d\n",
			   "dts_qos",
			   deq->dts_qos->node_name, deq->dts_qos->category,
			   deq->dts_qos->category_idx,
			   deq->dts_qos->sub_category_idx);
	else
		dp_sprintf(s, "  %-20s: NULL\n", "dts_qos");
}

int proc_registration_deq_port_dump(struct seq_file *s, int pos)
{
	int tmp_inst = 0;
	struct cqm_port_info *deq = NULL;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if ((dump_one_deq >= 0) && (dump_one_deq < DP_MAX_PPV4_PORT)) {
		deq = get_dp_deqport_info(tmp_inst, dump_one_deq);
		dp_dump_one_deq_port(s, dump_one_deq, deq);
		if (!dp_seq_has_overflowed(s)) {
			pos = -1;
			dump_one_deq = -1;
		}
	} else {
		deq = get_dp_deqport_info(tmp_inst, pos);
		if (!deq->ref_cnt)
			goto next;

		dp_dump_one_deq_port(s, pos, deq);
		if (dp_seq_has_overflowed(s))
			return pos;
next:
		pos++;
		if (pos >= DP_MAX_PPV4_PORT)
			pos = -1; /* end of the loop */
	}

	return pos;
}

ssize_t proc_deq_read(struct file *file, const char *buf, size_t count,
			loff_t *ppos)
{
	struct cqm_port_info *deqi = NULL;
	char *param_list[10] = {NULL}, *str, *p;
	int num, deq;
	int tmp_inst = 0;

	if (!capable(CAP_SYS_PACCT))
		return count;

	str = kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return -ENOMEM;

	if (dp_copy_from_user(str, buf, count)) {
		kfree(str);
		return -EFAULT;
	}

	p = str;

	for (num = 0; num < count; num++) {
		if (p[num] != '\n' && ((p[num] < '0') || (p[num] > '9'))) {
			pr_cont("DPM: %s, given str: %s, str[%d]: 0x%02x\n",
					__func__, p, num, p[num]);
			kfree(str);
			return -EINVAL;
		}
	}

	for (num = 0; num < ARRAY_SIZE(param_list); num++) {
		if (!strlen(p))
			break;
		param_list[num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}

	if (num != 1) {
		/* dump usage */
		pr_info("Usage: echo <decimal:deq_port> > %s\n", DBGFS_DEQ_PORT);
	} else {
		deq = dp_atoi(param_list[0]);
		if ((deq < 0) && (deq >= DP_MAX_PPV4_PORT)) {
			pr_err("DPM: %s deq port: %d is not valid\n", __func__,
					deq);
			kfree(str);
			return -EINVAL;
		}
		deqi = get_dp_deqport_info(tmp_inst, deq);
		dump_one_deq = deq;
		pr_info("Dumping deq port %d info: ------------>\n", deq);
		dp_dump_one_deq_port(NULL, deq, deqi);
	}

	kfree(str);
	return count;
}


void proc_gdb_read(struct seq_file *s)
{
	dp_dump_addr(s);
}

ssize_t proc_gdb_write(struct file *file, const char *buf, size_t count,
			loff_t *ppos)
{
	char *param_list[10] = {NULL}, *str, *p;
	int num;

	if (!capable(CAP_SYS_PACCT))
		return count;

	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return -ENOMEM;

	if (dp_copy_from_user(str, buf, count)) {
		kfree(str);
		return -EFAULT;
	}

	p = str;
	for (num = 0; num < ARRAY_SIZE(param_list); num++) {
		if (!strlen(p))
			break;
		param_list[num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}

	if (dp_strncmpi(param_list[0], "break", strlen("break")) == 0) {
		dp_gdb_break();
		kfree(str);
		return count;
	}

	/* dump usage */
	pr_info("  kgdb break: echo break > %s\n", DBGFS_GDB);
	kfree(str);
	return count;
}

void proc_dp_tx_rx_hooks_dump(struct seq_file *seq)
{
	proc_dp_active_tx_hook_dump(seq);
	proc_dp_active_rx_hook_dump(seq);
}

static struct dp_proc_entry dp_proc_entries[] = {
	/* name|single_callback|multi_callback|init_callback|write_callback */
#if defined(CONFIG_DPM_DATAPATH_DBG) && CONFIG_DPM_DATAPATH_DBG
	{DBGFS_DBG, proc_dbg_read, NULL, NULL, proc_dbg_write},
#endif
	{DBGFS_PORT, NULL, proc_port_dump, proc_port_init, proc_port_write},
	{DBGFS_DEV, NULL, proc_dev_dump, proc_dev_start, NULL},
	{DBGFS_DEV_OPS, NULL, proc_dev_ops_dump, proc_dev_ops_start, proc_dev_ops_write},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	{DBGFS_BR, NULL, proc_br_dump, proc_br_start, NULL},
#endif
	{DBGFS_INST_MOD, NULL, proc_inst_mod_dump, proc_inst_mod_start, NULL},
	{DBGFS_INST_HAL, NULL, proc_inst_hal_dump, NULL, NULL},
	{DBGFS_INST, NULL, proc_inst_dump, NULL, NULL},
	{DBGFS_QOS, NULL, proc_qos_dump, proc_qos_init, proc_qos_write},
	{DBGFS_EVENT, NULL, proc_dp_event_list_dump, NULL, NULL},
	{DBGFS_MIB_GLOBAL, NULL, proc_mib_stats_dump, NULL, proc_mib_stats_write},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	{DBGFS_BR_VLAN, NULL, proc_brvlan_dump, NULL, NULL},
#endif
	{DBGFS_CTP_LIST, NULL, NULL, NULL, proc_ctp_list_write},
	{DBGFS_PCE, NULL, proc_pce_dump, NULL, proc_pce_write},
	{DBGFS_DPID, NULL, proc_dpid_dump, NULL, NULL},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* For PRX, there is no rxin_ring. So no CQM enq port*/
	{DBGFS_ENQ_PORT, NULL, proc_registration_enq_port_dump, NULL, NULL},
#endif
	{DBGFS_HOOKS, proc_dp_tx_rx_hooks_dump, NULL, NULL, NULL},
	{DBGFS_DEQ_PORT, NULL, proc_registration_deq_port_dump, NULL, proc_deq_read},
	{DBGFS_QID, NULL, proc_registration_qid_dump, NULL, NULL},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
	{DBGFS_FN, proc_fn_read, NULL, NULL, NULL},
#endif
	{DBGFS_GDB, proc_gdb_read, NULL, NULL, proc_gdb_write},
	{DBGFS_BP, NULL, proc_bp_pmapper_dump, NULL, NULL},
#if IS_ENABLED(CONFIG_OF)
	{DBGFS_DTS_RAW, proc_qos_raw_dts_dump, NULL, NULL, proc_dts_raw_write},
#endif
	{DBGFS_DTS, NULL, proc_qos_cfg_dump, NULL, NULL},
	{DBGFS_DTS_CATEGORY, proc_qos_category_dump, NULL, NULL, NULL},
	/* last place holder */
	{NULL, NULL, NULL, NULL, NULL}
};

struct dentry *dp_proc_node;
EXPORT_SYMBOL(dp_proc_node);

struct dentry *dp_proc_install(void)
{
	dp_proc_node = debugfs_create_dir(DP_DBGFS_NAME, NULL);

	if (dp_proc_node) {
		int i;

		for (i = 0; i < ARRAY_SIZE(dp_proc_entries); i++)
			dp_proc_entry_create(dp_proc_node,
					     &dp_proc_entries[i]);
	} else {
		pr_err("DPM: %s:datapath cannot create proc entry", __func__);
		return NULL;
	}

	return dp_proc_node;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

#endif

