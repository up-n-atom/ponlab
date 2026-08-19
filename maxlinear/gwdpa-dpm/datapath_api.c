// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2025, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <net/datapath_api.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/kgdb.h>
#include <linux/version.h>
#include "datapath.h"
#include "datapath_tx.h"
#include "datapath_rx.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"
#include "datapath_trace.h"
#include "datapath_ver.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define DP_FAST_LATE_INIT 0
u32 rx_desc_mask[4];
u32 tx_desc_mask[4];
u32 dp_drop_all_tcp_err;
u32 dp_pkt_size_check;
int dp_dbg_mode = 0; /* 0-pr_info, 1-trace_printk */
u8 g_toe_disable = 0; /*Gloabal toe disable default setting: enable toe*/
u64 dp_dbg_flag;
u64 dp_dbgfs_flag;
EXPORT_SYMBOL(dp_dbgfs_flag);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
module_param(dp_dbg_flag, ullong, 0660);
MODULE_PARM_DESC(dp_dbg_flag, " set the dp_dbg_flag value runtime");
module_param(dp_dbgfs_flag, ullong, 0660);
MODULE_PARM_DESC(dp_dbgfs_flag, " set the dp_dbgfs_flag value runtime");
#endif
char *log_buf;
int log_buf_len = 1000;
EXPORT_SYMBOL(dp_dbg_flag);

#undef DP_DBGFS_ENUM_OR_STRING
#define DP_DBGFS_ENUM_OR_STRING(name, short_name) short_name
char *dp_dbgfs_flag_str[] = DP_DBGFS_FLAG_LIST;

#undef DP_DBGFS_ENUM_OR_STRING
#define DP_DBGFS_ENUM_OR_STRING(name, short_name) name
u64 dp_dbgfs_flag_list[] = DP_DBGFS_FLAG_LIST;

static char *dp_dbg; /* for module_param */
bool dp_mod_exiting = false;
#undef DP_DBG_ENUM_OR_STRING
#define DP_DBG_ENUM_OR_STRING(name, short_name) short_name
char *dp_dbg_flag_str[] = DP_DBG_FLAG_LIST;

#undef DP_DBG_ENUM_OR_STRING
#define DP_DBG_ENUM_OR_STRING(name, short_name) name
u64 dp_dbg_flag_list[] = DP_DBG_FLAG_LIST;

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name, short_name) short_name
char *dp_port_type_str[] = DP_F_FLAG_LIST;

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name, short_name) name
u32 dp_port_flag[] = DP_F_FLAG_LIST;

char *dp_port_status_str[] = {
	"PORT_FREE",
	"PORT_ALLOCATED",
	"PORT_DEV_REGISTERED",
	"PORT_SUBIF_REGISTERED",
	"Invalid"
};

int dp_init_ok;
int dp_cpu_init_ok;
int dflt_bp[DP_MAX_INST] = {-1};  /* dummy bp for CTP block assignment usage */
atomic_t dp_status = ATOMIC_INIT(0);
DP_DEFINE_LOCK(dp_lock);
u32 dp_dbg_err = 1; /*print error */
EXPORT_SYMBOL(dp_dbg_err);

/* saving Globally, reinsert cqm deqport, to retrieve info later */
u32 reinsert_deq_port[DP_MAX_INST] = {0};

static int dp_register_dc(int inst, u32 port_id,
		struct dp_dev_data *data, u32 flags);

static int dp_build_cqm_data(int inst, u32 port_id,
		struct cbm_dp_alloc_complete_data *cbm_data,
		struct dp_dev_data *data);

struct platform_device *g_dp_dev;
/*port 0 is reserved and never assigned to any one */
int dp_inst_num;
/* Keep per DP instance information here */
struct inst_property dp_port_prop[DP_MAX_INST];
/* Keep all subif information per instance/LPID/subif */
struct pmac_port_info *dp_port_info[DP_MAX_INST];

/* dp_bp_tbl[] is mainly for PON case.
 * Only if multiple gem port are attached to same bridge port,
 * ie, both dev and data->ctp_dev are provided when calling dp_register_subif_ext API
 * This bridge port device will be recorded into this dp_bp_tbl[].
 * later other information, like pmapper ID/mapping table will be put here also
 */
struct bp_pmapper dp_bp_tbl[DP_MAX_INST][DP_MAX_BP_NUM];
int n_dp_bp = 0; /*number of already allocated BP */

/* q_tbl[] is mainly for the queue created/used during dp_register_subif_ext
 */
struct q_info dp_q_tbl[DP_MAX_INST][DP_MAX_QUEUE_NUM];

/* sched_tbl[] is mainly for the sched created/used during dp_register_subif_ext
 * Note: dp_sched_tbl is used for all logical node, not just for scheduler itself
 */
struct dp_sched_info dp_sched_tbl[DP_MAX_INST][DP_MAX_NODES];

/* dp_deq_port_tbl[] is to record cqm dequeue port info
 */
struct cqm_port_info dp_deq_port_tbl[DP_MAX_INST][DP_MAX_PPV4_PORT];

/* dp_enq_port_tbl[] is to record cqm enqueue port info
 */
struct cqm_igp_info dp_enq_port_tbl[DP_MAX_INST][DP_MAX_CQM_IGP];

/* DMA TX CH info: [inst][max_core*max_port*max_channel]
 * Valid offset start from 1.
 * offset zero is not valid dma offset
 */
struct dma_chan_info *dp_dma_chan_tbl[DP_MAX_INST];

struct parser_info pinfo[4];
int dp_print_len;

static void *dp_ops[DP_MAX_INST][DP_OPS_CNT];
struct cbm_ops *dp_cbm_ops[DP_MAX_INST];

/* Per CPU gobal Rx and Tx counters for DPM */
DEFINE_PER_CPU_SHARED_ALIGNED(struct mib_global_stats, mib_g_stats);

#if !IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
void (*dp_dev_get_ethtool_stats_fn)(struct net_device *dev,
		struct ethtool_stats *stats,
		u64 *data) = NULL;
EXPORT_SYMBOL(dp_dev_get_ethtool_stats_fn);

void dp_set_ethtool_stats_fn(int inst, void (*cb)(struct net_device *dev,
			struct ethtool_stats *stats, u64 *data))
{
	dp_dev_get_ethtool_stats_fn = cb;
}
EXPORT_SYMBOL(dp_set_ethtool_stats_fn);

int (*dp_get_dev_stat_strings_count_fn)(struct net_device *dev) = NULL;
EXPORT_SYMBOL(dp_get_dev_stat_strings_count_fn);

void dp_set_ethtool_stats_strings_cnt_fn(int inst,
		int (*cb)(struct net_device *dev))
{
	dp_get_dev_stat_strings_count_fn = cb;
}
EXPORT_SYMBOL(dp_set_ethtool_stats_strings_cnt_fn);

void (*dp_get_dev_ss_stat_strings_fn)(struct net_device *dev,
		u8 *data) = NULL;
EXPORT_SYMBOL(dp_get_dev_ss_stat_strings_fn);

void dp_set_ethtool_stats_strings_fn(int inst, void (*cb)(struct net_device *dev,
			u8 *data))
{
	dp_get_dev_ss_stat_strings_fn = cb;
}
EXPORT_SYMBOL(dp_set_ethtool_stats_strings_fn);
#endif /* CONFIG_DPM_DATAPATH_PARTIAL_FEED */

int dp_register_ops2(int inst, enum DP_OPS_TYPE type, void *ops)
{
	if (is_invalid_inst(inst) || type >= DP_OPS_CNT) {
		DP_DEBUG(DP_DBG_FLAG_REG, "wrong index\n");
		return DP_FAILURE;
	}
	if (!ops)
		return DP_FAILURE;

	dp_ops[inst][type] = ops;

	/* Get registered ops from CQM */
	if (type == DP_OPS_CQM)
		dp_cbm_ops[inst] = (struct cbm_ops *)ops;

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_register_ops2);

void *dp_get_ops2(int inst, enum DP_OPS_TYPE type)
{
	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return NULL;
	}

	if (is_invalid_inst(inst) || type >= DP_OPS_CNT) {
		DP_DEBUG(DP_DBG_FLAG_REG, "wrong index\n");
		return NULL;
	}
	return dp_ops[inst][type];
}

EXPORT_SYMBOL(dp_get_ops2);

char *get_dp_port_type_str(int k)
{
	return dp_port_type_str[k];
}
EXPORT_SYMBOL(get_dp_port_type_str);

u32 get_dp_port_flag(int k)
{
	return dp_port_flag[k];
}
EXPORT_SYMBOL(get_dp_port_flag);

int get_dp_port_type_str_size(void)
{
	return ARRAY_SIZE(dp_port_type_str);
}
EXPORT_SYMBOL(get_dp_port_type_str_size);

int get_dp_dbg_flag_str_size(void)
{
	return ARRAY_SIZE(dp_dbg_flag_str);
}

int get_dp_dbgfs_flag_str_size(void)
{
	return ARRAY_SIZE(dp_dbgfs_flag_str);
}

int get_dp_port_status_str_size(void)
{
	return ARRAY_SIZE(dp_port_status_str);
}

int parser_size_via_index(u8 index)
{
	if (index >= ARRAY_SIZE(pinfo)) {
		pr_err("DPM: Wrong index=%d, it should less than %zu\n", index,
				ARRAY_SIZE(pinfo));
		return 0;
	}

	return pinfo[index].size;
}

int parser_enabled(int ep, struct dma_rx_desc_1 *desc_1)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
	if (!desc_1) {
		pr_err("DPM: NULL desc_1 is not allowed\n");
		return 0;
	}
#endif

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32) || \
	IS_ENABLED(CONFIG_X86_INTEL_LGM)
#else
	if (!ep)
		return pinfo[(desc_1->field.mpe2 << 1) +
			desc_1->field.mpe1].size;
#endif
	return 0;
}

void dp_print_err_info(int res)
{
	switch (res) {
		case DP_ERR_SUBIF_NOT_FOUND:
			pr_err("DPM: subif not found\n");
			break;
		case DP_ERR_INIT_FAIL:
			pr_err("DPM: init not done\n");
			break;
		case DP_ERR_INVALID_PORT_ID:
			pr_err("DPM: invalid port id\n");
			break;
		case DP_ERR_MEM:
			pr_err("DPM: memory allocation failure\n");
			break;
		case DP_ERR_NULL_DATA:
			pr_err("DPM: exp data info is NULL\n");
			break;
		case DP_ERR_INVALID_SUBIF:
			pr_err("DPM: invalid subif\n");
			break;
		case DP_ERR_DEFAULT:
			pr_err("DPM: other generic error\n");
			break;
		default:
			pr_err("DPM: why come to here??(%s)\n", __func__);
			break;
	}
}

u32 *get_port_flag(int inst, int index)
{
	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return NULL;
	}

	if (is_invalid_inst(inst))
		return NULL;

	if (index < dp_port_prop[inst].info.cap.max_num_dp_ports)
		return &get_dp_port_info(inst, index)->alloc_flags;

	return NULL;
}
EXPORT_SYMBOL(get_port_flag);

struct pmac_port_info *get_port_info_via_dp_port(int inst, int dp_port)
{
	int i;

	for (i = 0; i < dp_port_prop[inst].info.cap.max_num_dp_ports; i++) {
		struct pmac_port_info *port = get_dp_port_info(inst, i);

		if ((port->status & PORT_DEV_REGISTERED) &&
				port->port_id == dp_port)
			return port;
	}

	return NULL;
}

int8_t parser_size(int8_t v)
{
	int ret;

	switch (v) {
		case DP_PARSER_F_DISABLE:
			ret = 0;
			break;
		case DP_PARSER_F_HDR_ENABLE:
			ret = PASAR_OFFSETS_NUM;
			break;
		case DP_PARSER_F_HDR_OFFSETS_ENABLE:
			ret = PASAR_OFFSETS_NUM + PASAR_FLAGS_NUM;
			break;
		default:
			pr_err("DPM: Wrong parser setting: %d\n", v);
			ret = -1;
			break;
	}

	return ret;
}

/*Only for SOC side, not for peripheral device side */
int dp_set_gsw_parser(u8 flag, u8 cpu, u8 mpe1, u8 mpe2, u8 mpe3)
{
	int inst = 0;
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	dp_info = get_dp_prop_info(inst);
	if (!dp_info->dp_set_gsw_parser)
		return -1;

	return dp_info->dp_set_gsw_parser(flag, cpu, mpe1, mpe2, mpe3);
}
EXPORT_SYMBOL(dp_set_gsw_parser);

int dp_get_gsw_parser(u8 *cpu, u8 *mpe1, u8 *mpe2, u8 *mpe3)
{
	int inst = 0;
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	dp_info = get_dp_prop_info(inst);
	if (!dp_info->dp_get_gsw_parser)
		return -1;

	return dp_info->dp_get_gsw_parser(cpu, mpe1, mpe2, mpe3);
}
EXPORT_SYMBOL(dp_get_gsw_parser);

char *parser_str(int index)
{
	if (index == 0)
		return "cpu";

	if (index == 1)
		return "mpe1";

	if (index == 2)
		return "mpe2";

	if (index == 3)
		return "mpe3";

	pr_err("DPM: Wrong index:%d\n", index);
	return "Wrong index";
}

/* some module may have reconfigure parser configuration in FMDA_PASER.
 * It is necessary to refresh the pinfo
 */
void dp_parser_info_refresh(u32 cpu, u32 mpe1, u32 mpe2,
		u32 mpe3, u32 verify)
{
	int i;

	pinfo[0].v = cpu;
	pinfo[1].v = mpe1;
	pinfo[2].v = mpe2;
	pinfo[3].v = mpe3;

	for (i = 0; i < ARRAY_SIZE(pinfo); i++) {
		if (verify && pinfo[i].size != parser_size(pinfo[i].v))
			pr_err("DPM: %s[%d](%d) != %s(%d)??\n",
					"Lcal parser pinfo", i, pinfo[i].size,
					"register cfg", parser_size(pinfo[i].v));

		/*force to update */
		pinfo[i].size = parser_size(pinfo[i].v);

		if (pinfo[i].size < 0 || pinfo[i].size > PKT_PMAC_OFFSET) {
			pr_err("DPM: Wrong parser setting for %s: %d\n",
					parser_str(i), pinfo[i].v);
		}
	}
}
EXPORT_SYMBOL(dp_parser_info_refresh);

void print_parser_status(struct seq_file *s)
{
	if (!s)
		return;

	dp_sprintf(s, "REG.cpu  value=%u size=%u\n", pinfo[0].v, pinfo[0].size);
	dp_sprintf(s, "REG.MPE1 value=%u size=%u\n", pinfo[1].v, pinfo[1].size);
	dp_sprintf(s, "REG.MPE2 value=%u size=%u\n", pinfo[2].v, pinfo[2].size);
	dp_sprintf(s, "REG.MPE3 value=%u size=%u\n", pinfo[3].v, pinfo[3].size);
}

void dp_dump_port_data(struct dp_port_data *d)
{
	if (!d)
		return;

	pr_cont("   dp_port_data: 0x%px\n", d);
	pr_cont("      flag_ops         : 0x%x\n", d->flag_ops);
	pr_cont("      resv_num_port    : %u\n", d->resv_num_port);
	pr_cont("      start_port_no    : %u\n", d->start_port_no);
	pr_cont("      num_resv_q       : %d\n", d->num_resv_q);
	pr_cont("      num_resv_sched   : %d\n", d->num_resv_sched);
	pr_cont("      deq_port_base    : %d\n", d->deq_port_base);
	pr_cont("      deq_num          : %d\n", d->deq_num);
	pr_cont("      link_speed_cap   : %d\n", d->link_speed_cap);
	pr_cont("      n_wib_credit_pkt : %d\n", d->n_wib_credit_pkt);
}

void dp_dump_pmac_cfg(dp_pmac_cfg_t *pmac_cfg)
{
	int i;

	if (!pmac_cfg)
		return;

	pr_cont("   pmac_cfg: 0x%px\n", pmac_cfg);
	pr_cont("      ig_pmac_flags: 0x%x\n", pmac_cfg->ig_pmac_flags);
	pr_cont("      eg_pmac_flags: 0x%x\n", pmac_cfg->eg_pmac_flags);
	pr_cont("      ig_pmac:\n");
	pr_cont("         tx_dma_chan: %d, err_disc: %d, pmac: %d, "
			"def_pmac: %d, def_pmac_pmap: %d\n"
			"         def_pmac_en_pmap: %d, def_pmac_tc: %d, "
			"def_pmac_en_tc: %d, def_pmac_subifid: %d\n"
			"         def_pmac_src_port: %d, res_ing: 0x%hx\n"
			"         def_pmac_hdr(len=%d): ",
			pmac_cfg->ig_pmac.tx_dma_chan,
			pmac_cfg->ig_pmac.err_disc,
			pmac_cfg->ig_pmac.pmac,
			pmac_cfg->ig_pmac.def_pmac,
			pmac_cfg->ig_pmac.def_pmac_pmap,
			pmac_cfg->ig_pmac.def_pmac_en_pmap,
			pmac_cfg->ig_pmac.def_pmac_tc,
			pmac_cfg->ig_pmac.def_pmac_en_tc,
			pmac_cfg->ig_pmac.def_pmac_subifid,
			pmac_cfg->ig_pmac.def_pmac_src_port,
			pmac_cfg->ig_pmac.res_ing, DP_MAX_PMAC_LEN);
	for (i = 0; i < DP_MAX_PMAC_LEN; i++)
		pr_cont("0x%02x ", pmac_cfg->ig_pmac.def_pmac_hdr[i]);
	pr_cont("\n");
	pr_cont("      eg_pmac:\n");
	pr_cont("         rx_dma_chan: %d, rm_l2hdr: %d, "
			"num_l2hdr_bytes_rm: %d, "
			"fcs: %d, pmac: %d, redir: %d, bsl_seg: %d, "
			"dst_port: %d\n"
			"         res_endw1: %d, res_dw1: %d, res1_endw0: %d, "
			"res1_dw0: %d, res2_endw0: %d, res2_dw0: %d"
			"tc_enable: %d, traffic_class: %d\n"
			"         flow_id: %d, dec_flag: %d, enc_flag: %d, "
			"mpe1_flag: %d, mpe2_flag: %d, res_eg: 0x%x\n",
			pmac_cfg->eg_pmac.rx_dma_chan,
			pmac_cfg->eg_pmac.rm_l2hdr,
			pmac_cfg->eg_pmac.num_l2hdr_bytes_rm,
			pmac_cfg->eg_pmac.fcs,
			pmac_cfg->eg_pmac.pmac,
			pmac_cfg->eg_pmac.redir,
			pmac_cfg->eg_pmac.bsl_seg,
			pmac_cfg->eg_pmac.dst_port,
			pmac_cfg->eg_pmac.res_endw1,
			pmac_cfg->eg_pmac.res_dw1,
			pmac_cfg->eg_pmac.res1_endw0,
			pmac_cfg->eg_pmac.res1_dw0,
			pmac_cfg->eg_pmac.res2_endw0,
			pmac_cfg->eg_pmac.res2_dw0,
			pmac_cfg->eg_pmac.tc_enable,
			pmac_cfg->eg_pmac.traffic_class,
			pmac_cfg->eg_pmac.flow_id,
			pmac_cfg->eg_pmac.dec_flag,
			pmac_cfg->eg_pmac.enc_flag,
			pmac_cfg->eg_pmac.mpe1_flag,
			pmac_cfg->eg_pmac.mpe2_flag,
			pmac_cfg->eg_pmac.res_eg);
}

/*note: dev can be NULL */
static int32_t dp_alloc_port_private(int inst,
		struct module *owner,
		struct net_device *dev,
		u32 dev_port, s32 port_id,
		dp_pmac_cfg_t *pmac_cfg,
		struct dp_port_data *data,
		u32 flags)
{
	int i;
	struct cbm_dp_alloc_data *cbm_data;
	struct pmac_port_info *port;
	struct inst_info *dp_info;

	if (!owner) {
		pr_err("DPM: Allocate port failed for owner NULL\n");
		return DP_FAILURE;
	}
	if (is_invalid_port(port_id) || is_invalid_inst(inst)) {
		pr_err("DPM: %s: wrong port_id=%d or inst=%d\n", __func__, port_id,
		       inst);
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		pr_cont("\n=====> DPM: %s input arguments:\n"
			"   inst: %d, owner: 0x%px(%s), dev: 0x%px, dev_port: %u, port_id: %d\n"
			"   pmac_cfg: 0x%px, data: 0x%px, Flags: ",
			__func__, inst, owner, owner->name, dev, dev_port, port_id, pmac_cfg,
			data);
		for (i = 0; i < ARRAY_SIZE(dp_port_type_str); i++)
			if (flags & dp_port_flag[i])
				pr_cont("%s| ", dp_port_type_str[i]);
		pr_cont("\n");
		/*Print dp_pmac_cfg*/
		dp_dump_pmac_cfg(pmac_cfg);
		dp_dump_port_data(data);
	}
#endif

	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		return DP_FAILURE;
	dp_info = get_dp_prop_info(inst);
	cbm_data->dp_inst = inst;
	cbm_data->cbm_inst = dp_port_prop[inst].cbm_inst;
	cbm_data->data = data;

	if (flags & DP_F_DEREGISTER) {	/*De-register */
		port = get_dp_port_info(inst, port_id);
		if (port->status != PORT_ALLOCATED) {
			pr_err("DPM: %s is failed, inst: %d, port_id: %d, "
			       "module: %s\n", __func__, inst, port_id,
			       owner->name);
			kfree(cbm_data);
			return DP_FAILURE;
		}

		dp_notifier_invoke(inst, dev, port_id, 0, NULL,
				DP_EVENT_DE_ALLOC_PORT);

		cbm_data->deq_port = port->deq_port_base;
		cbm_data->deq_port_num = port->deq_port_num;
		cbm_data->dma_chan = port->dma_chan;
		cbm_data->num_dma_chan = port->num_dma_chan;
		cbm_data->dp_port = port_id;
		for (i = 0; i < port->deq_port_num; i++)
			get_dp_deqport_info(inst, port->deq_ports[i])->dp_port[port_id] = 0;
		dp_dealloc_cqm_port(owner, dev_port, port, cbm_data,
				port->alloc_flags | flags);
		dp_inst_del_mod(owner, port_id, 0);
		DP_CB(inst, port_platform_set)(inst, port_id, data, flags);
		/* Only clear those fields we need to clear */
		dp_memset(port, 0, offsetof(struct pmac_port_info, tail));
		kfree(cbm_data);
		return DP_SUCCESS;
	}
	if (port_id) { /*with specified port_id */
		port = get_dp_port_info(inst, port_id);
		if (port->status != PORT_FREE) {
			pr_err("DPM: %s %s(%s %d) fail: port %d used by %s %d\n",
					"module", owner->name,
					"dev_port", dev_port, port_id,
					port->owner->name,
					port->dev_port);
			kfree(cbm_data);
			return DP_FAILURE;
		}
	}

	if (dp_alloc_cqm_port(owner, dev, dev_port, port_id, cbm_data, flags)) {
		kfree(cbm_data);
		return DP_FAILURE;
	}

	port_id = cbm_data->dp_port;
	if (port_id >= dp_info->cap.max_num_dp_ports) {
		pr_err("DPM: wrong dp_port: %d. Max Number of dp_port=%d\n",
				port_id, dp_info->cap.max_num_dp_ports);
		kfree(cbm_data);
		return DP_FAILURE;
	}
	/* Only clear those fields we need to clear */
	port = get_dp_port_info(inst, port_id);
	dp_memset(port, 0, offsetof(struct pmac_port_info, tail));
	port->type = DP_DATA_PORT_LINUX;
	/*save info from caller */
	port->owner = owner;
	port->dev = dev;
	port->dev_port = dev_port;
	port->alloc_flags = flags;
	port->status = PORT_ALLOCATED;
	port->n_wib_credit_pkt = data->n_wib_credit_pkt;

	/*save info from cbm_dp_port_alloc*/
	port->flag_other = cbm_data->flags;
	port->port_id = cbm_data->dp_port;
	port->deq_port_base = cbm_data->deq_port;
	port->deq_port_num = cbm_data->deq_port_num;
	port->inst_prop = get_dp_port_prop(inst);
	port->inst = inst;
	port->data_flag_ops = data->flag_ops;
	if (cbm_data->deq_port_num > ARRAY_SIZE(port->deq_ports)) {
		pr_err("DPM: wrong deq_port_num: %d\n", cbm_data->deq_port_num);
		dp_dealloc_cqm_port(owner, dev_port, port, cbm_data,
				flags);
		/* Only clear those fields we need to clear */
		dp_memset(port, 0, offsetof(struct pmac_port_info, tail));
		return DP_FAILURE;
	}
	/* here suppose it is continuous cqm dequeue port. For ACA non-continous
	 * case, later it will be updated during dp_register_dev_ext
	 * since deq_port_base and deq_port_num not accurate here according
	 * to old CQM driver's interface design
	 */
	for (i = 0; i < cbm_data->deq_port_num; i++) {
		port->deq_ports[i] = cbm_data->deq_port + i;
		get_dp_deqport_info(inst, port->deq_ports[i])->dp_port[cbm_data->dp_port] = 1;
	}
	port->num_dma_chan = cbm_data->num_dma_chan;

	if (cbm_data->num_dma_chan &&
			(cbm_data->flags & CBM_PORT_DMA_CHAN_SET)) {
		/* Note: for any ACA at present, num_dma_chan should be always zero
		 *       for non-ACA device:
		 *       1) Its dequeue port is always continuous
		 *       2) Its DMA channel  is always continuous
		 *       3) last but not least, no dequeue port sharing cross
		 *          different dp_port
		 * If one of these condition not meet, below logic will be wrong
		 */
		int dma_ch_base;

		dma_ch_base = get_dma_chan_idx(inst, cbm_data->dma_chan);
		if (dma_ch_base == DP_FAILURE) {
			pr_err("DPM: Failed get_dma_chan_idx!!\n");
			dp_dealloc_cqm_port(owner, dev_port, port, cbm_data,
					flags);
			/* Only clear those fields we need to clear */
			dp_memset(port, 0, offsetof(struct pmac_port_info, tail));
			kfree(cbm_data);
			return DP_FAILURE;
		}
		port->dma_chan_tbl_idx = dma_ch_base;
		DP_DEBUG(DP_DBG_FLAG_REG, "dma_chan_tbl_idx=%d for ep=%d\n",
				dma_ch_base, port->port_id);
	}
	/*save info to port data*/
	data->deq_port_base = port->deq_port_base;
	data->deq_num = port->deq_port_num;
	if (cbm_data->flags & CBM_PORT_DMA_CHAN_SET)
		port->dma_chan = cbm_data->dma_chan;
	if (cbm_data->flags & CBM_PORT_PKT_CRDT_SET)
		port->tx_pkt_credit = cbm_data->tx_pkt_credit;
	if (cbm_data->flags & CBM_PORT_BYTE_CRDT_SET)
		port->tx_b_credit = cbm_data->tx_b_credit;
	if (cbm_data->flags & CBM_PORT_RING_ADDR_SET) {
		port->txpush_addr = (void *)cbm_data->txpush_addr;
		port->txpush_addr_qos =	(void *)cbm_data->txpush_addr_qos;
	}
	if (cbm_data->flags & CBM_PORT_RING_SIZE_SET)
		port->tx_ring_size = cbm_data->tx_ring_size;
	if (cbm_data->flags & CBM_PORT_RING_OFFSET_SET)
		port->tx_ring_offset = cbm_data->tx_ring_offset;
	if (cbm_data->num_dma_chan > 1 &&
			cbm_data->deq_port_num != cbm_data->num_dma_chan) {
		pr_err("DPM: ERROR:deq_port_num=%d not equal to num_dma_chan=%d\n",
				cbm_data->deq_port_num, cbm_data->num_dma_chan);
		kfree(cbm_data);
		return DP_FAILURE;
	}

	if (dp_info->port_platform_set(inst, port_id, data, flags)) {
		pr_err("DPM: Failed port_platform_set for port_id=%d(%s)\n",
				port_id, owner ? owner->name : "");
		dp_dealloc_cqm_port(owner, dev_port, port, cbm_data, flags);
		/* Only clear those fields we need to clear */
		dp_memset(port, 0, offsetof(struct pmac_port_info, tail));
		kfree(cbm_data);
		return DP_FAILURE;
	}
	if (pmac_cfg)
		dp_pmac_set(inst, port_id, pmac_cfg);
	/*only 1st dp instance support real CPU path traffic */
	if (!inst && dp_info->init_dma_pmac_template)
		dp_info->init_dma_pmac_template(port_id, flags);
	for (i = 0; i < port->subif_max; i++)
		INIT_LIST_HEAD(&get_dp_port_subif(port, i)->logic_dev);
	dp_inst_insert_mod(owner, port_id, inst, 0);
#ifdef CONFIG_RFS_ACCEL
	if (is_soc_lgm(inst)) {
		struct pmac_port_info *cpu_port = NULL;
		/* Setup the Rx_cpu_map for the device, if present */
		cpu_port = get_dp_port_info(inst, PMAC_CPU_ID);
		if (cpu_port && dev && cpu_port->rx_cpu_rmap) {
			dev->rx_cpu_rmap = cpu_port->rx_cpu_rmap;
			if (unlikely(!dev->rx_cpu_rmap))
				DP_DEBUG(DP_DBG_FLAG_REG, "[%s]: dev->rx_cpu_rmap nul for [%s]\n", __func__, dev->name);
		}
	}
#endif /* CONFIG_RFS_ACCEL */
	DP_DEBUG(DP_DBG_FLAG_REG,
			"Port %d allocation succeed for module %s with dev_port %d\n",
			port_id, owner->name, dev_port);

	dp_notifier_invoke(inst, dev, port_id, 0, NULL, DP_EVENT_ALLOC_PORT);

	kfree(cbm_data);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		pr_cont("\n<===== DPM: %s output arguments:\n"
			"   inst: %d, owner: 0x%px(%s), dev: 0x%px, dev_port: %u, port_id: %d\n"
			"   pmac_cfg: 0x%px, data: 0x%px, Flags: ",
			__func__, inst, owner, owner->name, dev, dev_port, port_id, pmac_cfg,
			data);
		for (i = 0; i < ARRAY_SIZE(dp_port_type_str); i++)
			if (flags & dp_port_flag[i])
				pr_cont("%s| ", dp_port_type_str[i]);
		pr_cont("\n");
		/*Print dp_pmac_cfg*/
		dp_dump_pmac_cfg(pmac_cfg);
		dp_dump_port_data(data);
	}
#endif
	return port_id;
}

int dp_cbm_deq_port_enable(struct module *owner, int inst, int port_id,
		int deq_port, int num_deq_port, int flags,
		u32 dma_ch_offset)
{
	struct cbm_dp_en_data *cbm_data;
	struct pmac_port_info *port_info;
	u32 cqm_deq_port, dma_ch_ref;

	port_info = get_dp_port_info(inst, port_id);
	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		return DP_FAILURE;
	cbm_data->dp_inst = inst;
	cbm_data->num_dma_chan = port_info->num_dma_chan;
	cbm_data->cbm_inst = dp_port_prop[inst].cbm_inst;

	cqm_deq_port = deq_port;
	dma_ch_ref = atomic_read(&(dp_dma_chan_tbl[inst] +
				dma_ch_offset)->ref_cnt);
	cbm_data->deq_port = cqm_deq_port;
	/* Enhance below condition later to support
	 * both dp_register_subif & deq_update_info API
	 */
	/* to enable DMA */
	if (cbm_data->num_dma_chan) {
		if (flags & CBM_PORT_F_DISABLE) {
			if (!dma_ch_ref) /*Last*/
				cbm_data->dma_chnl_init = 1;
		} else {
			if (dma_ch_ref == 1) /*First*/
				cbm_data->dma_chnl_init = 1;
		}
	}
	if (dp_enable_cqm_port(owner, port_info, cbm_data, 0)) {
		kfree(cbm_data);
		return DP_FAILURE;
	}
	kfree(cbm_data);
	return DP_SUCCESS;
}

int32_t dp_register_subif_private(int inst, struct module *owner,
		struct net_device *dev,
		char *subif_name, dp_subif_t *subif_id,
		struct dp_subif_data *data, u32 flags)
{
	int i, port_id, start, end, j;
	struct pmac_port_info *port_info;
	struct cbm_dp_en_data *cbm_data;
	struct subif_platform_data platfrm_data = {0};
	struct dp_subif_info *sif;
	u32 cqm_deq_port;
	u32 dma_ch_offset;
	u32 dma_ch_ref_curr;
	struct inst_info *dp_info = get_dp_prop_info(inst);

	port_id = subif_id->port_id;
	port_info = get_dp_port_info(inst, port_id);
	subif_id->inst = inst;
	subif_id->subif_num = 1;
	platfrm_data.subif_data = data;
	platfrm_data.dev = dev;
	/*Sanity Check*/
	if (port_info->status < PORT_DEV_REGISTERED) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"register subif failed:%s is not a registered dev!\n",
				subif_name);
		return DP_FAILURE;
	}
	if (!dp_dma_chan_tbl[inst]) {
		pr_err("DPM: dp_dma_chan_tbl[%d] NULL\n", inst);
		return DP_FAILURE;
	}
	if (subif_id->subif < 0) {/*dynamic mode */
		if (flags & DP_F_SUBIF_LOGICAL) {
			if (!(dp_info->supported_logic_dev(inst, dev,
							subif_name))) {
				DP_DEBUG(DP_DBG_FLAG_REG,
						"reg subif fail:%s not support dev\n",
						subif_name);
				return DP_FAILURE;
			}
			if (!(flags & DP_F_ALLOC_EXPLICIT_SUBIFID)) {
				/*Share same subif with its base device
				 *For GRX350: nothing need except save it
				 *For PRX300: it need to allocate BP for it
				 */
				return add_logic_dev(inst, port_id, dev,
						subif_id, data, flags);
			}
		}
		start = 0;
		end = port_info->subif_max;
	} else {
		/*caller provided subif. Try to get its vap value as start */
		start = GET_VAP(subif_id->subif, port_info->vap_offset,
				port_info->vap_mask);
		end = start + 1;
	}

	/*allocate a free subif */
	for (i = start; i < end; i++) {
		sif = get_dp_port_subif(port_info, i);
		if (!sif->flags)
			break;
		if ((subif_id->subif > 0) && (start == i))
			pr_err("DPM: subifid(%d) is duplicated! vap(%d)\n",
					subif_id->subif, start);
	}
	if (i >= end) {
		pr_err("DPM: register subif failed for no matched vap(%s)\n",
				dev->name ? dev->name : "NULL");
		return DP_FAILURE;
	}
	if (data->num_deq_port == 0)
		data->num_deq_port = 1;

	cqm_deq_port = port_info->deq_ports[data->deq_port_idx];
	dma_ch_offset = DP_DEQ(inst, cqm_deq_port).dma_ch_offset;
	dma_ch_ref_curr = atomic_read(&(dp_dma_chan_tbl[inst] +
				dma_ch_offset)->ref_cnt);

	if (data->flag_ops & DP_SUBIF_PREL2)
		sif->prel2_len = 1;

	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		return DP_FAILURE;
	cbm_data->dp_inst = inst;
	cbm_data->num_dma_chan = dp_get_dma_ch_num(inst, port_id,
			data->num_deq_port);
	cbm_data->cbm_inst = dp_port_prop[inst].cbm_inst;
	cbm_data->f_policy = data->f_policy;
	cbm_data->bm_policy_res_id = data->bm_policy_res_id;
	cbm_data->data = data;

	for (j = 0; j < data->num_deq_port; j++) {
		cbm_data->deq_port = port_info->deq_ports[data->deq_port_idx + j];
		if (get_dp_deqport_info(inst, cbm_data->deq_port)->ref_cnt)
			continue;
		/* No need to enable DMA if
		 * a) there is no DMA (ie, port_info->num_dma_chan = 0) or
		 * b) dma for that deq port is already enabled(ie,
		 *    dma_ch_ref_curr > 0)
		 */
		cbm_data->dma_chnl_init = 0;
		if (cbm_data->num_dma_chan && !dma_ch_ref_curr){
			/* Valid num_dma_ch &&
			 * its ref count is 0
			 * then dma_ch_init=1
			 */
			cbm_data->dma_chnl_init = 1;
		}
		/* always call dp_enable_cqm_port even without US traffic.
		 * The reason is to get the unique policy for this subif.
		 */
		if (dp_enable_cqm_port(owner, port_info, cbm_data, 0)) {
			kfree(cbm_data);
			return DP_FAILURE;
		}
	}
	sif->port_info = port_info;
	if (data->f_policy) {
		sif->tx_policy_num = cbm_data->tx_policy_num;
		sif->tx_policy_base = cbm_data->tx_policy_base;
		sif->rx_policy_num = cbm_data->rx_policy_num;
		sif->rx_policy_base = cbm_data->rx_policy_base;

		/* copy back the policy information to caller */
		data->tx_policy_num = cbm_data->tx_policy_num;
		data->tx_policy_base = cbm_data->tx_policy_base;
		data->rx_policy_num = cbm_data->rx_policy_num;
		data->rx_policy_base = cbm_data->rx_policy_base;
	} else {
		sif->tx_policy_num = port_info->tx_policy_num;
		sif->tx_policy_base = port_info->tx_policy_base;
		sif->rx_policy_num = port_info->rx_policy_num;
		sif->rx_policy_base = port_info->rx_policy_base;
	}

	if (subif_id->subif < 0)
		sif->subif = SET_VAP(i, port_info->vap_offset,
				port_info->vap_mask);
	else /* provided by caller since it is alerady shifted properly */
		sif->subif = subif_id->subif;

	if (dp_info->subif_platform_set(inst, port_id, i, &platfrm_data,
				flags)) {
		pr_err("DPM: subif_platform_set fail\n");
		kfree(cbm_data);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_REG, "subif_platform_set succeed\n");

	sif->flags = 1;
	sif->netif = dev;
	sif->num_egp = data->num_deq_port;
	/* currently this field is used for EPON case. Later can enhance */
	sif->num_qid = data->num_deq_port;
	sif->deq_port_idx = data->deq_port_idx;
	if (port_info->gpid_spl > 0)
		sif->f_spl_gpid = 1;
	dp_strlcpy(sif->device_name, subif_name, sizeof(sif->device_name));
	sif->subif_flag = flags;
	sif->spl_conn_type = DP_SPL_INVAL;
	sif->data_flag_ops = data->flag_ops;
	sif->data_toe_disable = data->toe_disable;
	if (data->flag_ops & DP_SUBIF_RX_FLAG)
		STATS_SET(sif->rx_flag, !!data->rx_en_flag);
	else
		STATS_SET(sif->rx_flag, 1);

	if (!data->rx_fn)
		sif->rx_fn = port_info->cb.rx_fn;
	else
		sif->rx_fn = data->rx_fn;

	sif->get_subifid_fn = data->get_subifid_fn;
	if (!sif->get_subifid_fn)
		sif->get_subifid_fn = port_info->cb.get_subifid_fn;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	if (data->flag_ops & DP_SUBIF_SWDEV)
		sif->swdev_en = data->swdev_en_flag;
	else
		sif->swdev_en = port_info->swdev_en;
#endif
	port_info->status = PORT_SUBIF_REGISTERED;
	/* to disable CPU bridge port from bp member list */
	if (data->flag_ops & DP_SUBIF_BP_CPU_DISABLE)
		sif->cpu_port_en = 0;
	else
		sif->cpu_port_en = 1;
	subif_id->port_id = port_id;
	subif_id->subif = sif->subif;
	subif_id->def_qid = sif->qid;
	subif_id->bport = sif->bp;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	subif_id->gpid = sif->gpid;
#endif
	sif->subif_groupid = i;
	subif_id->domain_id = sif->domain_id;
	subif_id->domain_members = sif->domain_members;
	/* set port as LCT port */
	if (data->flag_ops & DP_F_DATA_LCT_SUBIF) {
		port_info->lct_idx = i;
		/* ignore a failure here, this will only result in missing
		 * counter corrections, but is not a critical problem
		 * we allocate two buffers for unicast/multicast counters
		 */
		port_info->lct_rx_cnt = dp_kzalloc(
			2 * sizeof(*port_info->lct_rx_cnt), GFP_ATOMIC);
	}
	port_info->num_subif++;

	DP_DEBUG(DP_DBG_FLAG_REG,
		 "need dp_inc_dev for dev=%s ctp=%s dp_port=%d subif=0x%x\n",
		 dev->name, sif->ctp_dev ? sif->ctp_dev->name : "",
		 port_id, subif_id->subif);
	/* note: here dev can be logic_dev, not sif->dev
	 * below dp_inc_dev is required for both old and v2 dev list
	 */
	dp_inc_dev(sif, dev, subif_id->inst, subif_id->port_id,
			sif->bp, subif_id->subif, flags, false);
	if (data->ctp_dev) /*ctp dev no bp unique bp assigned */
		dp_inc_dev(sif, data->ctp_dev, subif_id->inst, subif_id->port_id,
				sif->bp, subif_id->subif, flags, true);

	/*default tc value for toe*/
	sif->toe_tc = TOE_TC_DEF_VALUE;

	_dp_init_subif_q_map_rules(sif, 0);
	for (i = 0; i < data->num_deq_port; i++)
		_dp_set_subif_q_lookup_tbl(sif, i, 0, -1);

	kfree(cbm_data);
	return DP_SUCCESS;
}

int32_t dp_deregister_subif_private(
	int inst, struct module *owner, struct net_device *dev,
	char *subif_name, dp_subif_t *subif_id,
	struct dp_subif_data *data, u32 flags)
{
	int res = DP_FAILURE;
	int i, j, port_id, cqm_port = 0, bp;
	u8 find = 0;
	struct pmac_port_info *port_info;
	struct cbm_dp_en_data *cbm_data;
	struct subif_platform_data platfrm_data = {0};
	struct dp_subif_info *sif;
	u32 dma_ch_offset, dma_ch_ref;
	struct inst_info *dp_info = get_dp_prop_info(inst);

	port_id = subif_id->port_id;
	port_info = get_dp_port_info(inst, port_id);
	platfrm_data.subif_data = data;
	platfrm_data.dev = dev;

	if (port_info->status != PORT_SUBIF_REGISTERED) {
		pr_err("DPM: %s: Unregister failed:%s not registered subif!\n",
				__func__, dev->name);
		return res;
	}
	if (!dp_dma_chan_tbl[inst]) {
		pr_err("DPM: dp_dma_chan_tbl[%d] NULL\n", inst);
		return res;
	}

	for (i = 0; i < port_info->subif_max; i++) {
		sif = get_dp_port_subif(port_info, i);
		if (sif->subif == subif_id->subif) {
			find = 1;
			break;
		}
	}
	if (!find)
		return res;

	DP_DEBUG(DP_DBG_FLAG_REG,
			"Found matched subif: port_id=%d subif=%x vap=%d\n",
			subif_id->port_id, subif_id->subif, i);

	/* device not match. Maybe it is unexplicit logical dev */
	if (sif->netif != dev) {
		if ((res = del_logic_dev(inst, &sif->logic_dev, dev, flags))) {
			pr_err("DPM: %s: dev=%s not match %s\n", __func__, dev->name,
					sif->netif->name);
			return DP_FAILURE;
		}
		dp_dec_dev(dev, inst, port_id, subif_id->subif, 0);
		return res;
	}

	/* reset LCT port and free counter corrections */
	if (sif->data_flag_ops & DP_F_DATA_LCT_SUBIF) {
		port_info->lct_idx = 0;
		/* take mib_cnt_lock to avoid access to freed memory */
		spin_lock_bh(&port_info->mib_cnt_lock);
		kfree(port_info->lct_rx_cnt);
		port_info->lct_rx_cnt = NULL;
		spin_unlock_bh(&port_info->mib_cnt_lock);
	}
	if (!list_empty(&sif->logic_dev)) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"Unregister fail: logic_dev of %s not empty yet!\n",
				subif_name);
		return res;
	}
	/* disable dp_rx path for this device */
	STATS_SET(sif->rx_flag, 0);
	bp = sif->bp;

	DP_DEBUG(DP_DBG_FLAG_SWDEV,
			"need dp_dec_dev for dev=%s ctp=%s dp_port=%d subif=0x%x\n",
			dev->name, sif->ctp_dev ? sif->ctp_dev->name : "",
			port_id, subif_id->subif);

	if (sif->ctp_dev) {
		dp_notifier_invoke(inst, sif->ctp_dev, port_id, subif_id->subif,
				NULL, DP_EVENT_DE_REGISTER_SUBIF);
		dp_dec_dev(sif->ctp_dev, inst, port_id,
				subif_id->subif, 0);
	}
	/* for pmapper and non-pmapper both
	 *  1)for PRX300, dev is managed at its HAL level
	 *  2)for GRX350, bp/dev should be always zero/NULL at present
	 *        before adapting to new datapath framework
	 */
	if (!get_dp_bp_info(inst, bp)->dev) {
		dp_notifier_invoke(inst, dev, port_id, subif_id->subif,
				NULL, DP_EVENT_DE_REGISTER_SUBIF);
	}
	dp_dec_dev(dev, inst, port_id, subif_id->subif, 0);
	/* subif_hw_reset */
	if (dp_info->subif_platform_set(inst, port_id, i, &platfrm_data,
				flags)) {
		pr_err("DPM: subif_hw_reset fail\n");
	}
	/* reset mib, flag, and others */
	dp_memset(&sif->mib, 0, sizeof(sif->mib));
	sif->flags = 0;
	sif->netif = NULL;
	port_info->num_subif--;

	if (!port_info->num_subif)
		port_info->status = PORT_DEV_REGISTERED;

	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		return DP_FAILURE;
	for (j = 0; j < sif->num_qid; j++) {
		cqm_port = sif->cqm_deq_port[j];

		if (get_dp_deqport_info(inst, cqm_port)->ref_cnt)
			continue;
		/*disable cqm port */
		cbm_data->dp_inst = inst;
		cbm_data->cbm_inst = dp_port_prop[inst].cbm_inst;
		cbm_data->deq_port = cqm_port;
		cbm_data->num_dma_chan =
			dp_get_dma_ch_num(inst, port_id, sif->num_qid);
		dma_ch_offset = DP_DEQ(inst, cqm_port).dma_ch_offset;
		dma_ch_ref = atomic_read(&(dp_dma_chan_tbl[inst] +
					dma_ch_offset)->ref_cnt);

		/* PPA Directpath/LitePath don't have DMA CH */
		cbm_data->dma_chnl_init = 0;
		if (cbm_data->num_dma_chan && !dma_ch_ref)
			cbm_data->dma_chnl_init = 1;

		/* Note: for deregistration, caller no need to fill in policy
		 * information even if this subif once requested the unique
		 * policy during registration stage. Instead CQM driver will
		 * handle it
		 */

		if (dp_enable_cqm_port(owner, port_info, cbm_data,
					CBM_PORT_F_DISABLE)) {
			kfree(cbm_data);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_REG,
				"cbm_dp_enable ok:port=%d subix=%d cqm_port=%d\n",
				port_id, i, cqm_port);
	}

	DP_DEBUG(DP_DBG_FLAG_REG, "  dp_port=%d subif=%d cqm_port=%d\n",
			subif_id->port_id, subif_id->subif, cqm_port);
	res = DP_SUCCESS;

	kfree(cbm_data);
	return res;
}

static int dp_config_ctp(int inst, int vap, int dp_port, int bp,
		struct subif_platform_data *data)
{
	struct inst_info *inst_info;

	inst_info = get_dp_prop_info(inst);
	if (!inst_info->dp_set_ctp_bp) {
		pr_err("DPM: dp set ctp config not supported\n");
		return DP_FAILURE;
	}
	return inst_info->dp_set_ctp_bp(inst, vap, dp_port, bp, data);
}

static int dp_cbm_deq_update(int inst, struct dp_subif_upd_info *info)
{
	struct inst_info *inst_info;

	inst_info = get_dp_prop_info(inst);
	if (!inst_info->dp_deq_update_info) {
		pr_err("DPM: dp_deq_update_info not supported\n");
		return DP_FAILURE;
	}
	return inst_info->dp_deq_update_info(info);
}

static int dp_alloc_bp_priv(int inst, int port_id, int subif_ix,
		int fid, int bp_member, int flags)
{
	struct inst_info *inst_info;

	inst_info = get_dp_prop_info(inst);
	if (!inst_info->dp_alloc_bridge_port) {
		pr_err("DPM: dp_alloc_bridge_port not supported\n");
		return DP_FAILURE;
	}
	return inst_info->dp_alloc_bridge_port(inst, port_id, subif_ix,
			fid, bp_member, flags);
}

static int dp_free_bp_priv(int inst, int bp)
{
	struct inst_info *inst_info;

	inst_info = get_dp_prop_info(inst);
	if (!inst_info->dp_free_bridge_port) {
		pr_err("DPM: dp_free_bridge_port not supported\n");
		return DP_FAILURE;
	}
	return inst_info->dp_free_bridge_port(inst, bp);
}

bool dp_has_spare_bp(void)
{
	if (n_dp_bp < MAX_BP_AVAIL_NUM - 1)
		return true;
	return false;
}

bool dp_update_subif_info_check_ok(struct net_device *old_dev,
				struct net_device *old_ctp_dev,
				struct dp_subif_upd_info *info)
{
	bool res = false;
	struct {
		dp_subif_t subif_get;
	} *p = NULL;

	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return false;

	if (old_ctp_dev) {
		/* sanity check old_ctp_dev */
		if (dp_get_netif_subifid(old_ctp_dev, NULL, NULL, NULL,
		    &p->subif_get, 0)) {
			pr_err("DPM: %s:why dp_get_netif_subifid failed:%s\n",
					__func__, old_ctp_dev->name);
			goto exit;
		}
		/* for ctp_dev, subif_num must be 1 */
		if (p->subif_get.subif_num > 1) {
			pr_err("DPM: %s:why wrong subif_num %d for ctp_dev: %s\n",
					__func__, p->subif_get.subif_num,
					old_ctp_dev->name);
			goto exit;
		}
		/* new ctp_dev's port_id must be same with its original old_ctp_dev's port_id */
		if (info->dp_port != p->subif_get.port_id) {
			pr_err("DPM: %s:why do_port not match (%d/%d): %s\n",
			       __func__, info->dp_port, p->subif_get.port_id,
			       old_ctp_dev->name);
			goto exit;
		}
		/* new ctp_dev's subif must be same with old_ctp_dev's subif */
		if (info->subif != p->subif_get.subif_list[0]) {
			pr_err("DPM: %s:why subif not match (%x/%x): %s\n",
			       __func__, info->subif, p->subif_get.subif_list[0],
			       old_ctp_dev->name);
			goto exit;
		}
	}

	if ((old_dev == info->new_dev) && 
	    (old_ctp_dev != info->new_ctp_dev)) {
		/* no need to support such use case */
		pr_err("DPM: %s:not support old_dev == new_dev, but old_ctp_dev != new_ctp_dev\n",
		       __func__);
		goto exit;
	}

	if (old_dev != info->new_dev) {
		struct dp_dev *dp_dev = dp_dev_lookup(info->new_dev);
		if (!dp_dev || !dp_dev->count) {
			/* check whether there is spare BP */
			if (!dp_has_spare_bp())
				goto exit; /* BP out of resoruce */
		}
	}
	res = true;
exit:
	kfree(p);
	return res;
}

int dp_update_subif_info(struct dp_subif_upd_info *info)
{
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif = NULL;
	struct dp_dev *dp_dev;
	int vap, bp = 0, fid = 0, res = DP_FAILURE;
	int f_rem_old_dev = 0;
	int f_rem_old_ctp_dev = 0, f_notif = 1;
	struct net_device *old_dev = NULL, *old_ctp_dev = NULL;
	struct bp_pmapper *bp_info = NULL;
	struct bp_pmapper *old_bp_info = NULL;
	dp_subif_t *new_subif_id_sync = NULL, *old_subif_id_sync = NULL;
	dp_get_netif_subifid_fn_t subifid_fn = NULL;
	int flags = 0; /*to set CPU BP member */
	struct {
		struct dp_subif_data old_data, new_data;
		struct subif_platform_data platfrm_data;
		struct dp_subif_data data;
		u32 old_cqm_deq_idx, old_bp, old_domain_member;
		u8 old_domain_id;
		u16 old_qid;
		dp_subif_t dp_subif;
		int old_subif_num, new_subif_num;
	} *p = NULL;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst) ||
			is_invalid_port(info->dp_port)) {
		pr_err("DPM: dp_update_subif failed, info not valid\n");
		return res;
	}
	if (!info->new_dev) {
		pr_err("DPM: dp_update_subif failed, info->new_dev NULL\n");
		return res;
	}
	if (!info->new_num_cqm_deq) {
		pr_err("DPM: dp_update_subif failed, info->new_num_cqm_deq zero\n");
		return res;
	}
	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return res;
	port_info = get_dp_port_info(info->inst, info->dp_port);
	vap = GET_VAP(info->subif, port_info->vap_offset,
			port_info->vap_mask);
	sif = get_dp_port_subif(port_info, vap);
	/* please note for LGM we have subif level callback, not port level */
	subifid_fn = port_info->cb.get_subifid_fn;
	old_dev = sif->netif;
	old_ctp_dev = sif->ctp_dev;
	p->old_qid = sif->qid;
	p->old_bp = sif->bp;
	old_bp_info = get_dp_bp_info(info->inst, p->old_bp);
	if (!old_dev) {
		pr_err("DPM: %s: why old_dev NULL\n", __func__);
		goto exit;
	}
	if (sif->flags <= 0) {
		pr_err("DPM: %s: wrong parameter since this subif not valid:dp_port=%d subif=%x new_dev=%s new_ctp=%s\n",
		       __func__, info->dp_port, info->subif,
		       info->new_dev ? info->new_dev->name : "",
		       info->new_ctp_dev ? info->new_ctp_dev->name : "");
	}
	DP_DEBUG(DP_DBG_FLAG_REG, "\nDPM: %s before:\n  old_bp_info->ref_cnt=%d bp=%d port_id=%d, subif=%x, vap=%d\n  old_dev=%s old_ctp=%s \n  new_dev=%s new_ctp=%s\n",
		 __func__,
		 old_bp_info->ref_cnt, p->old_bp, info->dp_port, info->subif, vap,
		 old_dev ? old_dev->name : "",
		 old_ctp_dev ? old_ctp_dev->name : "",
		 info->new_dev ? info->new_dev->name : "",
		 info->new_ctp_dev ? info->new_ctp_dev->name : "");

	DP_LIB_LOCK(&dp_lock);
#ifdef DP_NOT_USE_NETDEV_REGISTER
	dp_dev = dp_dev_lookup(info->new_dev);
	if (!dp_dev) {
		dp_add_dev(info->new_dev);
		dp_dev = dp_dev_lookup(info->new_dev);
	}
	if (!dp_dev) {
		pr_err("DPM: %s: dp_add_dev fail for %s\n",
				__func__, info->new_dev->name);
		goto exit_lock;
	}
	if (!info->new_ctp_dev)
		goto ctp_dev_workaround;
	dp_dev = dp_dev_lookup(info->new_ctp_dev);
	if (!dp_dev) {
		dp_add_dev(info->new_ctp_dev);
		dp_dev = dp_dev_lookup(info->new_ctp_dev);
	}
	if (!dp_dev) {
		pr_err("DPM: %s: dp_add_dev fail for %s\n", __func__,
				info->new_ctp_dev->name);
		goto exit_lock;
	}
ctp_dev_workaround:
	if (!dp_update_subif_info_check_ok(old_dev, old_ctp_dev, info))
		goto exit_lock;
#endif
	if (!dp_get_netif_subifid(old_dev, NULL, NULL, NULL, &p->dp_subif, 0))
		p->old_subif_num = p->dp_subif.subif_num;
	if (!dp_get_netif_subifid(info->new_dev, NULL, NULL, NULL, &p->dp_subif, 0))
		p->new_subif_num = p->dp_subif.subif_num;
	if (old_ctp_dev != info->new_ctp_dev)
		dp_notifier_invoke(info->inst,
				   old_ctp_dev ? old_ctp_dev : info->new_ctp_dev,
				   info->dp_port,
				   info->subif, NULL,
				   DP_EVENT_DE_REGISTER_SUBIF);
	/* convert last pmapper subif to gemport device: for recovery */
	if ((old_dev != info->new_dev) &&
	    !info->new_ctp_dev && (p->old_subif_num == 1))
		dp_notifier_invoke(info->inst, old_dev, info->dp_port,
				   info->subif, NULL,
				   DP_EVENT_DE_REGISTER_SUBIF);

	/* note: must do dp_dec_dev before dp_inc_dev */
	if (old_ctp_dev &&
	    ((old_ctp_dev != info->new_ctp_dev) ||
	     (old_dev != info->new_dev))) {
		/* dp_dec_dev for old_ctp_dev */
		dp_dev = dp_dev_lookup(old_ctp_dev);
		if (!dp_dev) {
			pr_err("DPM: %s:why old_dev not found in dev list:%s\n",
					__func__, old_dev->name);
			goto exit_lock;
		}
		
		/* note: caller make sure old and new subif must same */
		dp_dec_dev(old_ctp_dev, info->inst, info->dp_port, info->subif,
			   0);
		/* remove the old CTP dev from pmapper from old BP */
		dp_ctp_dev_list_del(&old_bp_info->ctp_dev, old_ctp_dev);

		p->old_data.ctp_dev = old_ctp_dev;
		f_rem_old_ctp_dev = 1;
	}

	if (old_dev != info->new_dev) {
		dp_dev = dp_dev_lookup(old_dev);
		if (!dp_dev) {
			pr_err("DPM: %s:why old_dev not found in dev list:%s\n",
					__func__, old_dev->name);
			goto exit_lock;
		}
		/* note: caller make sure old and new subif must same */
		dp_dec_dev(old_dev, info->inst, info->dp_port, info->subif, 0);
	}
	if (info->new_dev != old_dev) {
		/* handle new_dev */
		dp_dev = dp_dev_lookup(info->new_dev);
		if (!dp_dev || !dp_dev->count) { /* new device found */
			DP_DEBUG(DP_DBG_FLAG_REG,
					"new master dev(%s) not exists\n",
					info->new_dev->name);
			if (!(sif->cpu_port_en))
				flags = DP_SUBIF_BP_CPU_DISABLE;
			/* Allocate new bp */
			bp = dp_alloc_bp_priv(info->inst, info->dp_port, vap,
					0, 0, flags);
			if (bp < 0) {
				pr_err("DPM: Fail to alloc bridge port\n");
				goto exit_lock;
			}
			bp_info = get_dp_bp_info(info->inst, bp);
			if (info->new_ctp_dev) {
				DP_DEBUG(DP_DBG_FLAG_REG,
						"master dev with new bp(%d)\n", bp);
				/* update bridge port table reference counter */
				bp_info->dev = info->new_dev;
				bp_info->ref_cnt = 1;
				bp_info->flag = 1;
				INIT_LIST_HEAD(&bp_info->ctp_dev);
				dp_ctp_dev_list_add(&bp_info->ctp_dev,
						info->new_ctp_dev, bp, vap);
			}
			bp_info->domain_id = 0;
			bp_info->domain_member = DP_DFLT_DOMAIN_MEMBER;
			dp_memset(bp_info->bp_member_map, 0,
				  sizeof(bp_info->bp_member_map));
		} else { /* new_dev already in dp_dev with ref_count > 0 */
			DP_DEBUG(DP_DBG_FLAG_REG,
					"new master dev(%s) exists\n",
					info->new_dev->name);
			bp = dp_dev->bp;
			fid = dp_dev->fid;
			bp_info = get_dp_bp_info(info->inst, bp);
			if (!bp_info->flag) {
				pr_err("DPM: why bp_info->flag:%d(%s)?\n",
						bp_info->flag, "expect 1");
				goto exit_lock;
			}
			if (info->new_ctp_dev) {
				bp_info->ref_cnt++;

				DP_DEBUG(DP_DBG_FLAG_REG,
						"update bp refcnt:%d\n",
						bp_info->ref_cnt);
				dp_ctp_dev_list_add(&bp_info->ctp_dev,
						info->new_ctp_dev, bp, vap);
			}
		}
		dp_inc_dev(sif, info->new_dev, info->inst,
				info->dp_port, bp, info->subif, 0, false);
		/* flag set to clear old dp dev information */
		f_rem_old_dev = 1;
		DP_DEBUG(DP_DBG_FLAG_REG, "update bp(%d) into GSWIP CTP tbl\n",
				bp);
		/* update GSWIP CtpPortconfig table */
		p->platfrm_data.dev = info->new_dev;
		p->data.ctp_dev = info->new_ctp_dev;
		p->platfrm_data.subif_data = &p->data;
		res = dp_config_ctp(info->inst, vap, info->dp_port, bp,
				&p->platfrm_data);
		if (res) { /* failed */
			if (info->new_ctp_dev)
				dp_ctp_dev_list_del(&bp_info->ctp_dev,
						    info->new_ctp_dev);
			goto exit_lock;
		}
	} else { /* same master dev and no change */
		dp_dev = dp_dev_lookup(info->new_dev);
		if (!dp_dev) {
			pr_err("DPM: %s:why not found in dev list:%s ?\n",
					__func__, old_dev->name);
			goto exit_lock;
		}
	}
	bp_info = get_dp_bp_info(info->inst, sif->bp);
	if ((!f_rem_old_dev) && (!f_rem_old_ctp_dev))
		f_notif = 0;

	/* remove old dev */
	if (old_bp_info && f_rem_old_dev) {
		/* note: if old_bp_info->ref_cnt is zero, it means old_dev is
		 * a non-pmapper device and no ctp_dev under this device
		 */
		if (old_bp_info->ref_cnt > 0)
			old_bp_info->ref_cnt--;
		
		/* free bp if necessary */
		if (!old_bp_info->ref_cnt) {
			/* reset domain information while freeing bridge port */
			bp_info->domain_id = 0;
			bp_info->domain_member = DP_DFLT_DOMAIN_MEMBER;
			dp_memset(bp_info->bp_member_map, 0, sizeof(bp_info->bp_member_map));
			dp_free_bp_priv(info->inst, p->old_bp);
		}
	}

	/* added to make the subif_sync info similar to de-register
	 * subif to force update rcu link list for old_dev/old_ctp_dev
	 * Later will set this flag correctly
	 */
	sif->flags = 0;
	/* collect old subif info to update rcu list */
	old_subif_id_sync = dp_kzalloc(sizeof(*old_subif_id_sync) * 2, GFP_ATOMIC);
	if (!old_subif_id_sync) {
		res = DP_ERR_MEM;
		goto exit_lock;
	}
	old_subif_id_sync->port_id = info->dp_port;
	old_subif_id_sync->inst = info->inst;
	res = dp_sync_subifid(old_dev, NULL, old_subif_id_sync, &p->old_data,
			DP_F_DEREGISTER);
	/* no need any DQ port modification if prev & new tcont are same */
	p->old_cqm_deq_idx = sif->cqm_port_idx;
	p->old_bp = sif->bp;
	if (sif->cqm_port_idx == info->new_cqm_deq_idx) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"same cbm deq index, no change in queue config\n");
	} else {
		/* new tcont, update DP deq info */
		res = dp_cbm_deq_update(info->inst, info);
		if (res) {
			kfree(old_subif_id_sync);
			goto exit_lock;
		}
	}
	/* update new subif info to current subif struct */
	if (f_rem_old_dev) {
		DP_DEBUG(DP_DBG_FLAG_REG, "%s%s bp=%d fid=%d into DP subif\n",
				"update master_dev=", info->new_dev->name, bp, fid);
		sif->netif = info->new_dev;
		sif->bp = bp;
		sif->fid = fid;
		dp_strlcpy(sif->device_name, info->new_dev->name,
			   sizeof(sif->device_name));
	}
	sif->ctp_dev = info->new_ctp_dev;
	sif->flags = 1;
	/* update ctp_dev in dev list */
	if (info->new_ctp_dev && (old_ctp_dev != info->new_ctp_dev)) {
		dp_inc_dev(sif, info->new_ctp_dev, info->inst,
				dp_dev->ep, bp, info->subif, 0, true);
	}
	p->new_data.ctp_dev = sif->ctp_dev;
	new_subif_id_sync = dp_kzalloc(sizeof(*new_subif_id_sync) * 2, GFP_ATOMIC);
	if (!new_subif_id_sync) {
		res = DP_ERR_MEM;
		goto exit_lock;
	}
	new_subif_id_sync->port_id = info->dp_port;
	new_subif_id_sync->inst = info->inst;
	res = dp_sync_subifid(sif->netif, NULL, new_subif_id_sync, &p->new_data, 0);
	if (res)
		goto exit_lock;
	
	bp_info = get_dp_bp_info(info->inst, sif->bp);
	p->old_domain_id = bp_info->domain_id;
	p->old_domain_member = bp_info->domain_member;
	if (info->new_domain_flag) {
		/* update bridge port table */
		bp_info->domain_id = info->new_domain_id;
		bp_info->domain_member = info->domain_members;
		if (!f_rem_old_dev) {
			/* config bridge member port map for existing BP */
			DP_CB(info->inst, dp_cfg_domain_for_bp_member)
				(info->inst, sif->bp);
			dp_update_shared_bp_to_subif(info->inst, sif->netif,
					sif->bp, info->dp_port);
		}
	}
	sif->domain_id = bp_info->domain_id;
	sif->domain_members = bp_info->domain_member;
	DP_LIB_UNLOCK(&dp_lock);

	/* remove old rcu info based on collected subif info */
	res = dp_sync_subifid_priv(old_dev, NULL, old_subif_id_sync, &p->old_data,
			DP_F_DEREGISTER, subifid_fn, f_notif, true);
	if (res) {
		pr_err("DPM: remove old dev from rcu fail(%s)\n",
			old_dev->name ? old_dev->name : "NULL");
		goto exit;
	}
	/* update rcu info based on new subif info */
	res = dp_sync_subifid_priv(info->new_dev, info->new_dev->name, new_subif_id_sync,
			&p->new_data, 0, subifid_fn, f_notif, true);
	if (res) {
		pr_err("DPM: update new dev into rcu fail(%s)\n",
		       info->new_dev->name ? info->new_dev->name : "NULL");
		goto exit;
	}

	/* convert 1st gemport dev to pmapper*/
	if ((old_dev != info->new_dev) &&
	    info->new_ctp_dev && (p->new_subif_num == 0))
		dp_notifier_invoke(info->inst,
				   info->new_dev,
				   info->dp_port,
				   info->subif, NULL,
				   DP_EVENT_REGISTER_SUBIF);
	if (old_ctp_dev != info->new_ctp_dev)
		dp_notifier_invoke(info->inst,
				   old_ctp_dev ? old_ctp_dev : info->new_ctp_dev,
				   info->dp_port,
				   info->subif, NULL,
				   DP_EVENT_REGISTER_SUBIF);

	trace_dp_update_subif_info(DP_SUCCESS, info, old_dev, old_ctp_dev,
			p->old_cqm_deq_idx, p->old_qid,
			p->old_domain_id, p->old_domain_member,
			p->old_bp, sif);
	res = DP_SUCCESS;
	goto exit;

exit_lock:
	DP_LIB_UNLOCK(&dp_lock);
exit:
	DP_DEBUG(DP_DBG_FLAG_REG, "DPM: %s after: \n  old_bp_info->ref_cnt=%d old_bp=%d with f_rem_old_dev=%d dp_port=%d, subif=%x \n",
		 __func__, old_bp_info->ref_cnt, p->old_bp, f_rem_old_dev,
		info->dp_port, info->subif);
	if (bp_info)
		DP_DEBUG(DP_DBG_FLAG_REG, "  new_bp_info->ref_cnt=%d new_bp=%d \n",
			 bp_info->ref_cnt, bp);
			
	if (unlikely(res)) {
		dp_print_err_info(res);
		res = DP_FAILURE;
	}
	trace_dp_update_subif_info(DP_FAILURE, info, old_dev, old_ctp_dev,
			p->old_cqm_deq_idx, p->old_qid,
			p->old_domain_id, p->old_domain_member,
			p->old_bp, sif);
	kfree(p);
	kfree(old_subif_id_sync);
	kfree(new_subif_id_sync);
	return res;
}
EXPORT_SYMBOL(dp_update_subif_info);

/*Note: For same owner, it should be in the same HW instance
 *          since dp_register_dev/subif no dev_port information at all,
 *          at the same time, dev is optional and can be NULL
 */

int32_t dp_alloc_port2(struct module *owner, struct net_device *dev,
		u32 dev_port, int32_t port_id,
		dp_pmac_cfg_t *pmac_cfg, u32 flags)
{
	struct dp_port_data data = {0};

	return dp_alloc_port_ext(0, owner, dev, dev_port, port_id, pmac_cfg,
			&data, flags);
}
EXPORT_SYMBOL(dp_alloc_port2);

static bool dp_late_init(void)
{
	if (atomic_cmpxchg(&dp_status, 0, 1) == 0)
		dp_init_module();
	if (!dp_init_ok)
		pr_err("DPM: dp_alloc_port fail: datapath can't init\n");
	return dp_init_ok;
}

int32_t dp_alloc_port_ext2(int inst, struct module *owner,
		struct net_device *dev,
		u32 dev_port, int32_t port_id,
		dp_pmac_cfg_t *pmac_cfg,
		struct dp_port_data *data, u32 flags)
{
	int res;
	struct dp_port_data *tmp_data;

	tmp_data = dp_kzalloc(sizeof(*tmp_data), GFP_ATOMIC);
	if (!tmp_data)
		return DP_FAILURE;
	if (!dp_late_init()) {
		kfree(tmp_data);
		return DP_FAILURE;
	}
	if (!dp_port_prop[0].valid) {
		pr_err("DPM: No Valid datapath instance yet?\n");
		kfree(tmp_data);
		return DP_FAILURE;
	}
	if (!data)
		data = tmp_data;
	DP_LIB_LOCK(&dp_lock);
	res = dp_alloc_port_private(inst, owner, dev, dev_port,
			port_id, pmac_cfg, data, flags);
	DP_LIB_UNLOCK(&dp_lock);
	kfree(tmp_data);
	if (inst) /* only inst zero need ACA workaround */
		return res;

	return res;
}
EXPORT_SYMBOL(dp_alloc_port_ext2);

int32_t dp_register_dev2(struct module *owner, u32 port_id,
		dp_cb_t *dp_cb, u32 flags)
{
	int inst;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!owner || is_invalid_port(port_id))
		return DP_FAILURE;

	inst = dp_get_inst_via_module(owner, port_id, 0);

	if (inst < 0) {
		pr_err("DPM: %s not valid module %s\n", __func__, owner->name);
		return -1;
	}

	return dp_register_dev_ext(inst, owner, port_id, dp_cb, NULL, flags);
}
EXPORT_SYMBOL(dp_register_dev2);

static int remove_umt(int inst, const struct dp_umt_port *umt)
{
	struct umt_ops *ops = dp_get_umt_ops(inst);

	if (!ops)
		return -ENODEV;

	/* Disable UMT port */
	if (ops->umt_enable(ops->umt_dev, umt->ctl.id, 0))
		return DP_FAILURE;

	/* Release UMT port */
	return ops->umt_release(ops->umt_dev, umt->ctl.id);
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
int dp_handle_enq (struct cqm_igp_info *enq, u32 port_id, u16 dc_idx, int umt_id)
{
	if (!enq) {
		pr_err("DPM: %s %d it should not be NULL\n", __func__, __LINE__);
		return 0;
	}

	enq->umt_port[umt_id]--;
	enq->dc_port[port_id][dc_idx]--;
	enq->dp_port[port_id]--;

	if (enq->ref_cnt)
		enq->ref_cnt--;

	return 1;
}
#endif

static int32_t dp_deregister_dev(int inst, struct module *owner,
		u32 port_id, struct dp_dev_data *data,
		u32 flags)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, port_id);
	struct cbm_dp_alloc_complete_data *cbm_data;
	struct cqm_port_info *deq;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct cqm_igp_info *enq;
#endif
	u16 dc_idx;
	u8 dc_umt_map_idx[DP_DC_NUM] = {0};
	int i, j, deq_p0, ret;

	if (port_info->status != PORT_DEV_REGISTERED) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"%s de-register dev failed due to wrong state %d\n",
				owner->name, port_info->status);
		return DP_FAILURE;
	}
	if (port_info->num_subif) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"%s de-register dev failed with %d subif registered\n",
				owner->name, port_info->num_subif);
		return DP_FAILURE;
	}
	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		return DP_FAILURE;

	dp_notifier_invoke(inst, port_info->dev, port_id, 0, NULL,
			DP_EVENT_DE_REGISTER_DEV);
	port_info->status = PORT_ALLOCATED;
	cbm_data->num_rx_ring = port_info->num_rx_ring;
	for (i = 0; i < port_info->num_rx_ring; i++) {
		cbm_data->rx_ring[i] = &data->rx_ring[i];
		cbm_data->rx_ring[i]->out_enq_port_id =
			port_info->rx_ring[i].out_enq_port_id;
		cbm_data->rx_ring[i]->out_dma_ch_to_gswip =
			port_info->rx_ring[i].out_dma_ch_to_gswip;
		cbm_data->rx_ring[i]->out_cqm_deq_port_id =
			port_info->rx_ring[i].out_cqm_deq_port_id;
		cbm_data->rx_ring[i]->rx_policy_base =
			port_info->rx_ring[i].rx_policy_base;
		cbm_data->rx_ring[i]->policy_num =
			port_info->rx_ring[i].policy_num;
	}

	cbm_data->num_tx_ring = port_info->num_tx_ring;
	for (i = 0; i < port_info->num_tx_ring; i++) {
		/*Clearing deq_port_num for ACA/DC device && non docsis*/
		if (!port_info->tx_ring[i].not_valid)
			port_info->deq_port_num--;
		cbm_data->tx_ring[i] = &data->tx_ring[i];
		cbm_data->tx_ring[i]->txout_policy_base =
			port_info->tx_ring[i].txout_policy_base;
		cbm_data->tx_ring[i]->policy_num =
			port_info->tx_ring[i].policy_num;
		cbm_data->tx_ring[i]->tx_deq_port =
			port_info->tx_ring[i].tx_deq_port;
		deq = get_dp_deqport_info(inst,
				port_info->tx_ring[i].tx_deq_port);
		deq->dp_port[port_id] = 0;
	}
	cbm_data->num_umt_port = port_info->num_umt_port;
	cbm_data->qid_base = port_info->qid_base;
	if (data)
		cbm_data->opt_param = data->opt_param;
	/* Free CQM resources allocated during dev register */
	ret = dp_cqm_port_alloc_complete(owner, port_info, port_id,
			cbm_data, DP_F_DEREGISTER);
	kfree(cbm_data);
	if (ret == DP_FAILURE)
		return ret;

	DP_CB(inst, dev_platform_set)(inst, port_id, data, flags);

	for (i = 0; i < port_info->num_umt_port; i++) {
		dc_idx = port_info->umt_dc_map_idx[i];
		deq = get_dp_deqport_info(inst,
				port_info->tx_ring[dc_idx].tx_deq_port);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		enq = get_dp_enqport_info(inst,
				port_info->rx_ring[dc_idx].out_enq_port_id);
#endif
		DP_DEBUG(DP_DBG_FLAG_REG,
				"umt_port=%d ref:%d deq port:%d mapidx=%d \n",
				port_info->umt[i]->ctl.id,
				deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt,
				port_info->tx_ring[dc_idx].tx_deq_port, dc_idx);
		if (!deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt) {
			pr_err("DPM: check why umt port=%d ref_cnt=%d?",
					port_info->umt[i]->ctl.id,
					deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt);
			return DP_FAILURE;
		}
		if (deq->umt_info[dc_umt_map_idx[dc_idx]].umt_valid &&
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
				dp_handle_enq(enq, port_id, dc_idx, port_info->umt[i]->ctl.id) &&
#endif
				!(--deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt)) {
			DP_DEBUG(DP_DBG_FLAG_REG, "remove umt[%d]: umt_port=%d\n",
					i, port_info->umt[i]->ctl.id);
			remove_umt(inst, port_info->umt[i]);
			deq->umt_info[dc_umt_map_idx[dc_idx]].umt_valid = false;
		} else {
			DP_DEBUG(DP_DBG_FLAG_REG,
					"umt[%d]: umt_port=%d, remaning ref_cout=%d\n",
					i, port_info->umt[i]->ctl.id,
					deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt);
		}
		dc_umt_map_idx[dc_idx]++;
	}

	for (i = 0; i < port_info->num_rx_ring; i++) {
		/* There may have multiple Dequeue port and multi DMA to GSIWP
		 * to serve diff priority
		 */
		for (j = 0; j < port_info->rx_ring[i].num_out_cqm_deq_port; j++) {
			deq_p0 = port_info->rx_ring[i].out_cqm_deq_port_id + j;
			deq = get_dp_deqport_info(inst, deq_p0);
			deq->dp_port[port_id] = 0;
		}
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* For PRX, there is no rxin_ring. So dummy read is not applicable in PRX */
	for (i = 0; i < port_info->num_rx_ring; i++) {
		enq = get_dp_enqport_info(inst,
				port_info->rx_ring[i].out_enq_port_id);
		if (!enq->ref_cnt) {
			enq->dc_dummy_read = 0;
			DP_DEBUG(DP_DBG_FLAG_REG, "dc enq dummy buffer req cleared on port %d\n",
				 port_info->rx_ring[i].out_enq_port_id);
		}
	}
#endif
	return DP_SUCCESS;
}

void dp_dump_dp_cb(dp_cb_t *dp_cb)
{
	if (!dp_cb)
		return;
	pr_cont("   dp_cb dump: 0x%px\n", dp_cb);
	pr_cont("      rx_fn                  : 0x%px\n", dp_cb->rx_fn);
	pr_cont("      stop_fn                : 0x%px\n", dp_cb->stop_fn);
	pr_cont("      restart_fn             : 0x%px\n", dp_cb->restart_fn);
	pr_cont("      get_subifid_fn         : 0x%px\n", dp_cb->get_subifid_fn);
	pr_cont("      reset_mib_fn           : 0x%px\n", dp_cb->reset_mib_fn);
	pr_cont("      get_mib_fn             : 0x%px\n", dp_cb->get_mib_fn);
	pr_cont("      dma_rx_irq             : 0x%px\n", dp_cb->dma_rx_irq);
	pr_cont("      aca_fw_stop            : 0x%px\n", dp_cb->aca_fw_stop);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_CPUFREQ)
	pr_cont("      dp_coc_confirm_stat_fn : 0x%px\n", dp_cb->dp_coc_confirm_stat_fn);
#endif
}

void dp_dump_tx_ring(struct dp_dev_data *data)
{
	int i;

	for (i = 0; i < data->num_tx_ring; i++) {
		pr_cont("      tx_ring: %d/%d\n", i, data->num_tx_ring);
		pr_cont("         in_deq_ring_size  : %d\n",
			data->tx_ring[i].in_deq_ring_size);
		pr_cont("         in_deq_paddr      : 0x%px\n",
			data->tx_ring[i].in_deq_paddr);
		pr_cont("         in_deq_vaddr      : 0x%px\n",
			data->tx_ring[i].in_deq_vaddr);
		pr_cont("         out_free_ring_size: %d\n",
			data->tx_ring[i].out_free_ring_size);
		pr_cont("         out_free_paddr    : 0x%px\n",
			data->tx_ring[i].out_free_paddr);
		pr_cont("         out_free_vaddr    : 0x%px\n",
			data->tx_ring[i].out_free_vaddr);
		pr_cont("         num_tx_pkt        : %u\n",
			data->tx_ring[i].num_tx_pkt);
		pr_cont("         tx_pkt_size       : %d\n",
			data->tx_ring[i].tx_pkt_size);
		pr_cont("         txout_policy_base : %d\n",
			data->tx_ring[i].txout_policy_base);
		pr_cont("         policy_num        : %d\n",
			data->tx_ring[i].policy_num);
		pr_cont("         tx_poolid         : %hu\n",
			data->tx_ring[i].tx_poolid);
		pr_cont("         f_out_auto_free   : %hhu\n",
			data->tx_ring[i].f_out_auto_free);
		pr_cont("         tx_deq_port       : %u\n",
			data->tx_ring[i].tx_deq_port);
		pr_cont("         txpush_addr       : 0x%px\n",
			data->tx_ring[i].txpush_addr);
		pr_cont("         txpush_addr_qos   : 0x%px\n",
			data->tx_ring[i].txpush_addr_qos);
		pr_cont("         not_valid         : %d\n",
			data->tx_ring[i].not_valid);
		pr_cont("         bpress_in         : %d\n",
			data->tx_ring[i].bpress_in);
		pr_cont("         bpress_free       : %d\n",
			data->tx_ring[i].bpress_free);
	}
	pr_cont("\n");
}

void dp_dump_rx_ring(struct dp_dev_data *data)
{
	int i, j;

	for (i = 0; i < data->num_rx_ring; i++) {
		pr_cont("      rx_ring: %d/%d\n", i, data->num_rx_ring);
		pr_cont("         out_enq_ring_size   : %d\n",
			data->rx_ring[i].out_enq_ring_size);
		pr_cont("         out_enq_port_id     : %u\n",
			data->rx_ring[i].out_enq_port_id);
		pr_cont("         out_enq_paddr       : 0x%px\n",
			data->rx_ring[i].out_enq_paddr);
		pr_cont("         out_enq_vaddr       : 0x%px\n",
			data->rx_ring[i].out_enq_vaddr);
		pr_cont("         out_dma_ch_to_gswip : %u\n",
			data->rx_ring[i].out_dma_ch_to_gswip);
		pr_cont("         num_out_tx_dma_ch   : %u\n",
			data->rx_ring[i].num_out_tx_dma_ch);
		pr_cont("         out_cqm_deq_port_id : %u\n",
			data->rx_ring[i].out_cqm_deq_port_id);
		pr_cont("         num_out_cqm_deq_port: %u\n",
			data->rx_ring[i].num_out_cqm_deq_port);
		pr_cont("         in_alloc_ring_size  : %d\n",
			data->rx_ring[i].in_alloc_ring_size);
		pr_cont("         in_alloc_paddr      : 0x%px\n",
			data->rx_ring[i].in_alloc_paddr);
		pr_cont("         in_alloc_vaddr      : 0x%px\n",
			data->rx_ring[i].in_alloc_vaddr);
		pr_cont("         num_pkt             : %u\n",
			data->rx_ring[i].num_pkt);
		pr_cont("         rx_pkt_size         : %d\n",
			data->rx_ring[i].rx_pkt_size);
		pr_cont("         rx_policy_base      : %hu\n",
			data->rx_ring[i].rx_policy_base);
		pr_cont("         policy_num          : %hhu\n",
			data->rx_ring[i].policy_num);
		pr_cont("         rx_poolid           : %hu\n",
			data->rx_ring[i].rx_poolid);
		pr_cont("         prefill_pkt_num     : %d\n",
			data->rx_ring[i].prefill_pkt_num);
		pr_cont("         pkt_base_paddr      : 0x%px\n",
			data->rx_ring[i].pkt_base_paddr);
		pr_cont("         pkt_base_vaddr      : 0x%px\n",
			data->rx_ring[i].pkt_base_vaddr);
		pr_cont("         pkt_list_vaddr      : 0x%px\n",
			data->rx_ring[i].pkt_list_vaddr);
		pr_cont("         umt out_msg_mode    : %d\n",
			data->rx_ring[i].out_msg_mode);
		pr_cont("         out_qos_mode        : %d\n",
			data->rx_ring[i].out_qos_mode);
		pr_cont("         bpress_out          : %d\n",
			data->rx_ring[i].bpress_out);
		pr_cont("         bpress_alloc        : %d\n",
			data->rx_ring[i].bpress_alloc);
		pr_cont("         num_egp             : %d\n",
			data->rx_ring[i].num_egp);
		for (j = 0; j < data->rx_ring[i].num_egp; j++) {
			pr_cont("         dp_rx_ring_egp      : %d/%d\n", j,
				data->rx_ring[i].num_egp);
			pr_cont("            deq_port            : %d\n",
				data->rx_ring[i].egp[j].deq_port);
			pr_cont("            tx_ring_size        : %d\n",
				data->rx_ring[i].egp[j].tx_ring_size);
			pr_cont("            tx_pkt_credit       : %u\n",
				data->rx_ring[i].egp[j].tx_pkt_credit);
			pr_cont("            txpush_addr         : 0x%px\n",
				data->rx_ring[i].egp[j].txpush_addr);
			pr_cont("            txpush_addr_qos     : 0x%px\n",
				data->rx_ring[i].egp[j].txpush_addr_qos);

		}
	}
	pr_cont("\n");
}

void dp_dump_umt(struct dp_dev_data *data)
{
	int i;

	for (i = 0; i < data->num_umt_port; i++) {
		pr_cont("      umt dump: %d/%d\n", i, data->num_umt_port);
		pr_cont("         ctl.id           : %d\n",
			data->umt[i].ctl.id);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		DP_DUMP("         ctl.dst_addr_cnt : %u (%s)\n",
				data->umt[i].ctl.dst_addr_cnt,
				data->umt[i].ctl.dst_addr_cnt ? "new" : "old");
		if (unlikely(data->umt[i].ctl.dst_addr_cnt >= UMT_DST_ADDR_MAX))
		{
			pr_err("  DPM: %s, "
			       "umt dst_addr_cnt(%u) >= UMT_DST_ADDR_MAX(%d)",
			       __func__, data->umt[i].ctl.dst_addr_cnt,
			       UMT_DST_ADDR_MAX);
		} else {
			if (data->umt[i].ctl.dst_addr_cnt) {
				int j;
				DP_DUMP("         ctl.dst[]        : ");
				for (j = 0; j < data->umt[i].ctl.dst_addr_cnt; j++)
					DP_DUMP("%pad ",
						&data->umt[i].ctl.dst[j]);
				DP_DUMP("\n");
			} else {
				pr_cont("         ctl.daddr        : %pad\n",
					&data->umt[i].ctl.daddr);

			}
		}

#else
		pr_cont("         ctl.daddr        : %pad\n",
			&data->umt[i].ctl.daddr);
#endif
		pr_cont("         ctl.msg_interval : %u\n",
			data->umt[i].ctl.msg_interval);
		pr_cont("         ctl.msg_mode     : %d\n",
			data->umt[i].ctl.msg_mode);
		pr_cont("         ctl.cnt_mode     : %d\n",
			data->umt[i].ctl.cnt_mode);
		pr_cont("         ctl.sw_msg       : %d\n",
			data->umt[i].ctl.sw_msg);
		pr_cont("         ctl.rx_msg_mode  : %d\n",
			data->umt[i].ctl.rx_msg_mode);
		pr_cont("         ctl.enable       : %d\n",
			data->umt[i].ctl.enable);
		pr_cont("         ctl.fflag        : %lu\n",
			data->umt[i].ctl.fflag);
		pr_cont("         ctl.na_src       : %u\n",
			data->umt[i].ctl.na_src);

		pr_cont("         res.dma_id       : %u\n",
			data->umt[i].res.dma_id);
		pr_cont("         res.dma_ch_num   : %u\n",
			data->umt[i].res.dma_ch_num);
		pr_cont("         res.cqm_enq_pid  : %u\n",
			data->umt[i].res.cqm_enq_pid);
		pr_cont("         res.cqm_dq_pid   : %u\n",
			data->umt[i].res.cqm_dq_pid);
		pr_cont("         res.rx_src       : %d\n",
			data->umt[i].res.rx_src);
	}
	pr_cont("\n");
}
void dp_dump_dp_dev_data(struct dp_dev_data *data)
{
	int i;

	if (!data)
		return;
	pr_cont("   dp_dev_data: 0x%px\n", data);
	pr_cont("      flag_ops         : 0x%x\n", data->flag_ops);
	pr_cont("      num_rx_ring      : %d\n", data->num_rx_ring);
	pr_cont("      num_tx_ring      : %d\n", data->num_tx_ring);
	pr_cont("      num_umt_port     : %d\n", data->num_umt_port);
	pr_cont("      enable_cqm_meta  : %d\n", data->enable_cqm_meta);
	pr_cont("      max_subif        : %d\n", data->max_subif);
	pr_cont("      max_ctp          : %d\n", data->max_ctp);
	pr_cont("      max_gpid         : %d\n", data->max_gpid);
	pr_cont("      num_resv_q       : %d\n", data->num_resv_q);
	pr_cont("      num_resv_sched   : %d\n", data->num_resv_sched);
	pr_cont("      qos_resv_q_base  : %d\n", data->qos_resv_q_base);
	pr_cont("      wib_tx_phy_addr  : 0x%llx\n", (u64)data->wib_tx_phy_addr);
	pr_cont("      bm_policy_res_id : %d\n", data->bm_policy_res_id);
	pr_cont("      opt_param.pcidata: 0x%px\n", data->opt_param.pcidata);

	dp_dump_rx_ring(data);
	dp_dump_tx_ring(data);

	pr_cont("      gpid_info dump:\n");
	pr_cont("         f_min_pkt_len: %d\n", data->gpid_info.f_min_pkt_len);
	pr_cont("         seg_en       : %d\n", data->gpid_info.seg_en);
	pr_cont("         min_pkt_len  : %d\n", data->gpid_info.min_pkt_len);

	dp_dump_umt(data);

	for (i = 0; i < data->num_rx_ring; i++)
		pr_cont("      dc_extra_info[%d].no_bm_dummy_read: %d\n",
			i, data->dc_info[i].no_bm_dummy_read);

	pr_cont("      umt_dc_map:\n");
	for (i = 0; i < data->num_umt_port; i++) {
		pr_cont("        umt_idx(%d) --> dc_idx(%d)  f_not_alloc: %d\n",
			i, data->umt_dc[i].dc_idx, data->umt_dc[i].f_not_alloc);
	}
	pr_cont("\n");
}

int32_t dp_register_dev_ext2(int inst, struct module *owner, u32 port_id,
		dp_cb_t *dp_cb, struct dp_dev_data *data,
		u32 flags)
{
	int res = DP_FAILURE;
	struct pmac_port_info *port_info;
	struct dp_dev_data *tmp_data;
	struct cbm_dp_alloc_complete_data *cbm_data = NULL;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "dp_register_dev failed for datapath not init yet\n");
		return res;
	}
	tmp_data = dp_kzalloc(sizeof(*tmp_data), GFP_ATOMIC);
	if (!tmp_data)
		return res;
	if (!data)
		data = tmp_data;

	if (is_invalid_port(port_id) || is_invalid_inst(inst)) {
		kfree(tmp_data);
		return res;
	}

	if (!owner) {
		pr_err("DPM: owner NULL\n");
		kfree(tmp_data);
		return res;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		pr_cont("\n=====> DPM: %s input arguments:\n"
			"   inst: %d, owner: 0x%px(%s), port_id: %d, dp_cb: 0x%px, "
			"data: 0x%px, flags: 0x%x\n",
			__func__, inst, owner, owner->name, port_id, dp_cb,
			data, flags);
		dp_dump_dp_cb(dp_cb);
		dp_dump_dp_dev_data(data);
	}
#endif
	port_info = get_dp_port_info(inst, port_id);

	DP_LIB_LOCK(&dp_lock);
	if (flags & DP_F_DEREGISTER) {	/*de-register */
		res = dp_deregister_dev(inst, owner, port_id, data,
				flags);
		goto exit;
	}

	/*register a device */
	if (port_info->status != PORT_ALLOCATED) {
		pr_err("DPM: register dev fail for %s for unknown status:%d\n",
				owner->name, port_info->status);
		goto exit;
	}

	if (port_info->owner != owner) {
		pr_err("DPM: No matched owner(%s):0x%px->0x%px\n",
				owner->name, owner, port_info->owner);
		goto exit;
	}

	port_info->res_qid_base = data->qos_resv_q_base;
	port_info->num_resv_q = data->num_resv_q;
	port_info->dts_qos = dp_get_qos_cfg(inst, port_id,
					    port_info->alloc_flags,
					    data->qos_id);

	cbm_data = dp_kzalloc(sizeof(*cbm_data), GFP_ATOMIC);
	if (!cbm_data)
		goto exit;

	if (dp_build_cqm_data(inst, port_id, cbm_data, data))
		goto exit;

	/* Register device to CQM */
	if (dp_cqm_port_alloc_complete(owner, port_info, port_id, cbm_data,
				flags))
		goto exit;

	/* For Streaming port save the policy base/num from CQM
	 * For DC port, dp_register_dc will overwrite from ring info
	 */
	port_info->tx_policy_base = cbm_data->tx_policy_base;
	port_info->tx_policy_num = cbm_data->tx_policy_num;
	port_info->qid_base = cbm_data->qid_base;

	res = dp_register_dc(inst, port_id, data, flags);
	if (res)
		goto exit;

	/*Sanity check for port->deq_port_num*/
	if (!port_info->deq_port_num) {
		pr_err("DPM: %s %d Sanity check failed as deq_port_num is still 0\n",
				__func__, __LINE__);
		res = DP_FAILURE;
		goto exit;
	}

	DP_CB(inst, dev_platform_set)(inst, port_id, data, flags);

	/* TODO: Need a HAL layer API for CQM and DMA Setup for CQM QoS path
	 * especially for LGM 4 Ring case
	 */
	port_info->status = PORT_DEV_REGISTERED;
	if (dp_cb)
		port_info->cb = *dp_cb;

	dp_notifier_invoke(inst, port_info->dev, port_id, 0, data,
			DP_EVENT_REGISTER_DEV);

	res = DP_SUCCESS;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		pr_cont("\n<===== DPM: %s output arguments:\n"
			"   inst: %d, owner: 0x%px(%s), port_id: %d, dp_cb: 0x%px, "
			"data: 0x%px, flags: 0x%x\n",
			__func__, inst, owner, owner->name, port_id, dp_cb,
			data, flags);
		dp_dump_dp_cb(dp_cb);
		dp_dump_dp_dev_data(data);
	}
#endif

exit:
	kfree(cbm_data);
	kfree(tmp_data);
	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_register_dev_ext2);

void dp_dump_dp_subif_data(struct dp_subif_data *data)
{
	if (!data)
		return;

	DP_DUMP("   dp_subif_data dump: 0x%px\n", data);
	DP_DUMP("   ------------------------\n");
	DP_DUMP("      deq_port_idx        : %hhd\n", data->deq_port_idx);
	DP_DUMP("      flag_ops            : 0x%x\n", (u32)data->flag_ops);
	DP_DUMP("      q_id                : %d\n", data->q_id);
	DP_DUMP("      ctp_dev             : 0x%px(%s)\n", data->ctp_dev,
		data->ctp_dev ? data->ctp_dev->name : "null");
	DP_DUMP("      rx_fn               : 0x%px\n", data->rx_fn);
	DP_DUMP("      get_subifid_fn      : 0x%px\n", data->get_subifid_fn);
	DP_DUMP("      f_policy            : %hhu\n", data->f_policy);
	DP_DUMP("      tx_pkt_size         : %hu\n", data->tx_pkt_size);
	DP_DUMP("      gpid_tx_info:\n"
		"         f_min_pkt_len: %d\n"
		"         seg_en       : %d\n"
		"         min_pkt_len  : %d\n",
		data->gpid_tx_info.f_min_pkt_len,
		data->gpid_tx_info.seg_en,
		data->gpid_tx_info.min_pkt_len);
	DP_DUMP("      tx_policy_base      : %hu\n", data->tx_policy_base);
	DP_DUMP("      tx_policy_num       : %hhu\n", data->tx_policy_num);
	DP_DUMP("      rx_policy_base      : %hu\n", data->rx_policy_base);
	DP_DUMP("      rx_policy_num       : %hhu\n", data->rx_policy_num);
	DP_DUMP("      txin_ring_size      : %d\n", data->txin_ring_size);
	DP_DUMP("      txin_ring_phy_addr  : 0x%px\n",
		data->txin_ring_phy_addr);
	DP_DUMP("      credit_add_phy_addr : 0x%px\n",
		data->credit_add_phy_addr);
	DP_DUMP("      credit_left_phy_addr: 0x%px\n",
		data->credit_left_phy_addr);
	DP_DUMP("      mac_learn_disable   : %hu\n", data->mac_learn_disable);
	DP_DUMP("      num_deq_port        : %hu\n", data->num_deq_port);
	DP_DUMP("      rx_en_flag          : %u\n", data->rx_en_flag);
	DP_DUMP("      bm_policy_res_id    : %u\n", data->bm_policy_res_id);
	DP_DUMP("      swdev_en_flag       : %hu\n", data->swdev_en_flag);
	DP_DUMP("      domain_id           : %hhu\n", data->domain_id);
	DP_DUMP("      domain_members      : 0x%x\n", data->domain_members);
	DP_DUMP("      toe_disable         : %d\n", data->toe_disable);
}

void dp_dump_dp_subif(dp_subif_t *subif_id)
{
	int i;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	int j;
#endif

	if (!subif_id)
		return;

	DP_DUMP("   dp_subif_t dump: 0x%px\n", subif_id);
	DP_DUMP("   -----------------------\n");
	DP_DUMP("      dp_subif_port_cmn:\n");
	DP_DUMP("         port_id      : %d\n", subif_id->port_id);
	DP_DUMP("         alloc_flag   : 0x%x\n", (u32)subif_id->alloc_flag);
	DP_DUMP("         lookup_mode  : %hhu\n", subif_id->lookup_mode);
	DP_DUMP("         gsw_mode     : %u\n", subif_id->gsw_mode);
	DP_DUMP("         gpid_spl     : %hd\n", subif_id->gpid_spl);
	DP_DUMP("      dp_subif_cmn:\n");
	DP_DUMP("         bport        : %hu\n", subif_id->bport);
	DP_DUMP("         data_flag    : 0x%x\n", subif_id->data_flag);

	DP_DUMP("         num_q        : %hhu (max: %d)\n", subif_id->num_q,
		DP_MAX_DEQ_PER_SUBIF);
	if (subif_id->num_q > DP_MAX_DEQ_PER_SUBIF)
		subif_id->num_q = DP_MAX_DEQ_PER_SUBIF;
	DP_DUMP(KERN_CONT "         def_qlist    :");
	for (i = 0; i < subif_id->num_q; i++)
		DP_DUMP(KERN_CONT " %hu", subif_id->def_qlist[i]);
	DP_DUMP(KERN_CONT "\n");
	
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	for (j = 0; j < subif_id->subif_num; j++) {
		DP_DUMP(KERN_CONT "      def_eg_sess for gpid(%d):",
				subif_id->gpid_list[j]);
		for (i = 0; i < DP_DFL_SESS_NUM; i++) {
			DP_DUMP(KERN_CONT " %d", subif_id->dfl_eg_sess_ext[j][i]);
		}
		DP_DUMP(KERN_CONT "\n");
	}
#endif
	DP_DUMP("      subif_groupid: %u\n", subif_id->subif_groupid);

	DP_DUMP("      subif_num : %d (max: %d)\n", subif_id->subif_num,
		DP_MAX_CTP_PER_DEV);
	if (subif_id->subif_num > DP_MAX_CTP_PER_DEV)
		subif_id->subif_num = DP_MAX_CTP_PER_DEV;
	DP_DUMP(KERN_CONT "      subif_list:");
	for (i = 0; i < subif_id->subif_num; i++)
		DP_DUMP(KERN_CONT " %d", subif_id->subif_list[i]);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	DP_DUMP(KERN_CONT "\n");
	DP_DUMP(KERN_CONT "      gpid_list:");
	for (i = 0; i < subif_id->subif_num; i++)
		DP_DUMP(KERN_CONT " %d", subif_id->gpid_list[i]);
#endif
	DP_DUMP(KERN_CONT "\n");

	DP_DUMP("      ctp_base  : %u\n", subif_id->ctp_base);
	DP_DUMP("      ctp       : %u\n", subif_id->ctp);

	DP_DUMP(KERN_CONT "      subif_flag:");
	for (i = 0; i < subif_id->subif_num; i++)
		DP_DUMP(KERN_CONT " 0x%x", (u32)subif_id->subif_flag[i]);
	DP_DUMP(KERN_CONT "\n");

	DP_DUMP("      flag_bp        : %u\n", subif_id->flag_bp);
	DP_DUMP("      flag_pmapper   : %u\n", subif_id->flag_pmapper);
	DP_DUMP("      domain_id      : %hhu\n", subif_id->domain_id);
	DP_DUMP("      domain_members : 0x%x\n", subif_id->domain_members);
	DP_DUMP("      DP_IO_PORT_TYPE: %d\n", subif_id->type);
	DP_DUMP("      associate_netif: 0x%px(%s)\n",
		subif_id->associate_netif,
		subif_id->associate_netif ? subif_id->associate_netif->name :
		"NULL");
	DP_DUMP("      peripheral_pvt : %u\n\n", subif_id->peripheral_pvt);
}

/* if subif_id->subif < 0: Dynamic mode
 * else subif is provided by caller itself
 * Note: 1) for register logical device, if DP_F_ALLOC_EXPLICIT_SUBIFID is not
 *       specified, subif will take its base dev's subif.
 *       2) for IPOA/PPPOA, dev is NULL and subif_name is dummy string.
 *          in this case, dev->name may not be subif_name
 */
int32_t dp_register_subif_ext2(int inst, struct module *owner,
		struct net_device *dev,
		char *subif_name, dp_subif_t *subif_id,
		/*device related info*/
		struct dp_subif_data *data, u32 flags)
{
	int res = DP_FAILURE;
	int n, port_id, old_subif = -1;
	struct pmac_port_info *port_info;
	struct dp_subif_data tmp_data = {0};
	dp_subif_t *subif_id_sync = NULL;
	dp_get_netif_subifid_fn_t subifid_fn = NULL;
	bool f_need_alloc_bp = true;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"dp_register_subif fail for datapath not init yet\n");
		res = DP_ERR_INIT_FAIL;
		dp_print_err_info(res);
		return DP_FAILURE;
	}

	if (!subif_id || !owner) {
		DP_DEBUG(DP_DBG_FLAG_REG, "Failed subif_id 0x%px owner 0x%px\n",
				subif_id, owner);
		res = DP_ERR_INVALID_SUBIF;
		dp_print_err_info(res);
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		DP_DUMP("\n=====> DPM: %s input arguments:\n", __func__);
		DP_DUMP("   %s: %d, %s: 0x%px(%s), %s: 0x%px(%s), %s: %s,",
			"inst", inst,
			"owner", owner, owner->name,
			"dev", dev, dev ? dev->name : "NULL",
			"subif_name", subif_name ? subif_name : "NULL");
		DP_DUMP("   %s: 0x%px, %s: 0x%px, %s: 0x%x\n",
			"subif_id", subif_id,
			"data", data,
			"flags", flags);

		dp_dump_dp_subif(subif_id);
		dp_dump_dp_subif_data(data);
	}
#endif
	old_subif = subif_id->subif;
	port_id = subif_id->port_id;
	if (is_invalid_port(port_id) || is_invalid_inst(inst)) {
		res = DP_ERR_INVALID_PORT_ID;
		DP_DEBUG(DP_DBG_FLAG_REG,
				"dp_register_subif fail, either inst: %d or port_id: %d is invalid\n"
				, inst, port_id);
		return DP_FAILURE;
	}
	port_info = get_dp_port_info(inst, port_id);

	if ((!dev && !is_dsl(port_info)) || !subif_name) {
		DP_DEBUG(DP_DBG_FLAG_REG, "Wrong dev=0x%px, subif_name=0x%px\n",
				dev, subif_name);
		return DP_FAILURE;
	}
	if (!data)
		data = &tmp_data;

	subif_id_sync = dp_kzalloc(sizeof(*subif_id_sync) * 2, GFP_ATOMIC);
	if (!subif_id_sync)
		return DP_FAILURE;
	if (!(flags & DP_F_DEREGISTER)) {
		/* sanity check */
		if (!dp_get_netif_subifid(dev, NULL, NULL, NULL, subif_id_sync, 0)) {
			if (subif_id_sync[0].subif_num > 0)
				f_need_alloc_bp = false; /* BP already allocated */
		}
		if (f_need_alloc_bp && !dp_has_spare_bp())
			goto EXIT;
	}

	DP_LIB_LOCK(&dp_lock);
	if (port_info->owner != owner) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"Unregister subif fail:Not matching:0x%px(%s)->0x%px(%s)\n",
				owner, owner->name, port_info->owner,
				port_info->owner->name);
		DP_LIB_UNLOCK(&dp_lock);
		goto EXIT;
	}
	if (dev)
		DP_DEBUG(DP_DBG_FLAG_OPS, "%s before %s%s netdev_ops=0x%px\n",
				dev->name,
				flags & DP_F_DEREGISTER ? "de-register" : "register",
				"_subif",
				dev->netdev_ops);

	if (flags & DP_F_DEREGISTER) /*de-register */
		res = dp_deregister_subif_private(inst, owner, dev, subif_name,
				subif_id, data, flags);
	else /*register */
		res = dp_register_subif_private(inst, owner, dev, subif_name,
				subif_id, data, flags);
	if (res) {
		DP_LIB_UNLOCK(&dp_lock);
		goto EXIT;
	}

	if (!(flags & DP_F_SUBIF_LOGICAL)) {
		n = GET_VAP(subif_id->subif, port_info->vap_offset,
				port_info->vap_mask);
		subifid_fn = get_dp_port_subif(port_info, n)->get_subifid_fn;
	}
	dp_memset(subif_id_sync, 0, sizeof(*subif_id_sync));
	subif_id_sync->port_id = port_id;
	subif_id_sync->inst = inst;
	subif_id_sync->domain_id = subif_id->domain_id;
	subif_id_sync->domain_members = subif_id->domain_members;
	res = dp_sync_subifid(dev, subif_name, subif_id_sync, data, flags);
	if (res) {
		DP_LIB_UNLOCK(&dp_lock);
		goto EXIT;
	}
	DP_LIB_UNLOCK(&dp_lock);
	if (!res)
		res = dp_sync_subifid_priv(dev, subif_name, subif_id_sync, data,
				flags, subifid_fn, 1, false);
EXIT:
	kfree(subif_id_sync);
	if (unlikely(res)) {
		dp_print_err_info(res);
		res = DP_FAILURE;
	}
	if (dev)
		DP_DEBUG(DP_DBG_FLAG_OPS, "%s after %s%s netdev_ops=0x%px\n",
				dev->name,
				flags & DP_F_DEREGISTER ? "de-register" : "register",
				"_subif",
				dev->netdev_ops);
	trace_dp_register_subif(res, inst, owner, dev, subif_name, subif_id,
			data, flags, old_subif);

	dp_dump_debugfs_all(port_id,
			GET_VAP(subif_id->subif, port_info->vap_offset, port_info->vap_mask));
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_REG)) {
		DP_DUMP("\n<===== DPM: %s output arguments:\n", __func__);
		DP_DUMP("   %s: %d, %s: 0x%px(%s), %s: 0x%px(%s), %s: %s,",
			"inst", inst,
			"owner", owner, owner->name,
			"dev", dev, dev ? dev->name : "NULL",
			"subif_name", subif_name ? subif_name : "NULL");
		DP_DUMP("   %s: 0x%px, %s: 0x%px, %s: 0x%x\n",
			"subif_id", subif_id,
			"data", data,
			"flags", flags);

		dp_dump_dp_subif(subif_id);
		dp_dump_dp_subif_data(data);
	}
#endif
	return res;
}
EXPORT_SYMBOL(dp_register_subif_ext2);

int32_t dp_register_subif2(struct module *owner, struct net_device *dev,
		char *subif_name, dp_subif_t *subif_id,
		u32 flags)
{
	int inst;
	struct dp_subif_data data = {0};

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT,
				"%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!subif_id || !owner || is_invalid_port(subif_id->port_id)) {
		DP_DEBUG(DP_DBG_FLAG_REG, "fail owner 0x%px subif_id 0x%px\n",
				subif_id, owner);
		return DP_FAILURE;
	}
	inst = dp_get_inst_via_module(owner, subif_id->port_id, 0);
	if (inst < 0) {
		pr_err("DPM: wrong inst for owner=%s with ep=%d\n", owner->name,
				subif_id->port_id);
		return DP_FAILURE;
	}
	return dp_register_subif_ext(inst, owner, dev, subif_name,
			subif_id, &data, flags);
}
EXPORT_SYMBOL(dp_register_subif2);

/* Register sub interface for special devices (CPU or reinsertion port)
 * Corresponding flags have to be set in order to be registered inside this
 * function.
 */
int32_t dp_register_subif_spl_dev2(int inst, struct net_device *dev,
		char *subif_name, dp_subif_t *subif_id,
		struct dp_subif_data *data, u32 flags)
{
	int res = DP_FAILURE;

	if (!dp_late_init())
		return DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT,
				"%s datapath is not yet initialized\n",
				__func__);
		dp_print_err_info(DP_ERR_INIT_FAIL);
		return DP_FAILURE;
	}

	if (!subif_id) {
		DP_DEBUG(DP_DBG_FLAG_REG, "%s invalid subif_id\n",
				__func__);
		dp_print_err_info(DP_ERR_INVALID_SUBIF);
		return DP_FAILURE;
	}

	if (!data) {
		DP_DEBUG(DP_DBG_FLAG_REG, "%s invalid subif data\n",
				__func__);
		dp_print_err_info(DP_ERR_NULL_DATA);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_REG,
			"%s:dev=%s(p=0x%px) %s=%s port_id=%d subif=%d(%s)\n",
			(flags & DP_F_DEREGISTER) ?
			"unregister subif:" : "register subif",
			dev ? dev->name : "NULL",
			dev, "subif_name", subif_name,
			subif_id->port_id, subif_id->subif,
			(subif_id->subif < 0) ? "dynamic" : "fixed");

	if (!(data->flag_ops & (DP_SUBIF_CPU | DP_SUBIF_REINSERT))) {
		pr_err("DPM: %s unsupported flags dev=%s, flag_ops=0x%x\n",
				__func__, dev ? dev->name : "NULL", data->flag_ops);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	if (flags & DP_F_DEREGISTER)
		res = dp_del_subif_spl_dev(inst, dev, subif_name,
				subif_id, data, flags);
	else
		res = dp_add_subif_spl_dev(inst, dev, subif_name,
				subif_id, data, flags);
	DP_LIB_UNLOCK(&dp_lock);

	return res;
}
EXPORT_SYMBOL(dp_register_subif_spl_dev2);

int32_t dp_get_netif_subifid2(struct net_device *netif, struct sk_buff *skb,
		void *subif_data, u8 dst_mac[DP_MAX_ETH_ALEN],
		dp_subif_t *subif, u32 flags)
{
	struct dp_subif_cache *dp_subif;
	struct dp_subif_info *sif;
	struct pmac_port_info *port_info;
	u32 idx;
	dp_get_netif_subifid_fn_t subifid_fn_t;
	int res = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT,
				"failed: datapath not initialized yet\n");
		return res;
	}

	idx = dp_subif_hash(netif);
	//TODO handle DSL case in future
	rcu_read_lock_bh();
	dp_subif = dp_subif_lookup_safe(&dp_subif_list[idx], netif, subif_data);
	if (!dp_subif) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "Failed dp_subif_lookup: %s\n",
				netif ? netif->name : "NULL");
		rcu_read_unlock_bh();
		return res;
	}
	dp_memcpy(subif, &dp_subif->subif, sizeof(*subif));
	subifid_fn_t = dp_subif->subif_fn;
	/* To return associate VUNI device if subif is VANI
	*/
	if (subif->data_flag & DP_SUBIF_VANI) {
		port_info = get_dp_port_info(subif->inst, subif->port_id);
		sif = get_dp_port_subif(port_info, !subif->subif);
		if (sif->netif && sif->flags)
			subif->associate_netif = sif->netif;
	}
	rcu_read_unlock_bh();
	if (subifid_fn_t) {
		/* subif->subif will be set by callback api itself */
		res = subifid_fn_t(netif, skb, subif_data, dst_mac, subif,
				flags);
		if (res != 0)
			DP_DEBUG(DP_DBG_FLAG_DBG,
					"dp_subif->subif_fn return fail\n");
	} else {
		res = DP_SUCCESS;
	}
	return res;
}
EXPORT_SYMBOL(dp_get_netif_subifid2);

/* check whether this netif is registered DPM device or not */
bool dp_valid_netif(const struct net_device *netif)
{
	u32 idx;
	int res = false;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT,
				"%s failed: datapath not initialized yet\n",
				__func__);
		return res;
	}
	if (!netif)
		return false;

	idx = dp_subif_hash((struct net_device *)netif);
	rcu_read_lock_bh();
	if (dp_subif_lookup_safe(&dp_subif_list[idx], netif, NULL)) {
		rcu_read_unlock_bh();
		return true;
	}
	rcu_read_unlock_bh();
	return res;
}

bool dp_is_pmapper_check2(struct net_device *dev)
{
	dp_subif_t *subif;
	int ret;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret != DP_SUCCESS) {
		netdev_err(dev, "can not get subif\n");
		kfree(subif);
		return false;
	}

	if (subif->flag_pmapper) {
		kfree(subif);
		return true;
	}

	kfree(subif);
	return false;
}
EXPORT_SYMBOL(dp_is_pmapper_check2);

/*Note:
 * get subif according to netif.
 * Use subif->port_id passed by caller to get port_info
 */
int32_t dp_get_subifid_for_update(int inst, struct net_device *netif,
		dp_subif_t *subif, u32 flags)
{
	int res = DP_FAILURE;
	int i;
	int port_id;
	u8 num = 0;
	struct dp_subif_info **sif_ptr_arr = NULL;
	struct logic_dev *tmp = NULL;
	struct pmac_port_info *p_info;
	bool max_subif_over = false;

	if (!subif) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"%s failed:subif is NULL\n", __func__);
		return DP_ERR_NULL_DATA;
	}
	port_id = subif->port_id;
	if (port_id < 0) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
				"failed: %s\n" ,
				netif ? netif->name : "NULL");
		res = DP_ERR_SUBIF_NOT_FOUND;
		goto EXIT;
	}

	p_info = get_dp_port_info(inst, port_id);

	/* For DSL ATM case netif will be NULL with valid port id */
	if ((!netif) && (!is_dsl(p_info))) {
		DP_DEBUG(DP_DBG_FLAG_REG,
				"failed: netif null\n");
		return DP_ERR_NULL_DATA;
	}

	sif_ptr_arr = dp_kzalloc(sizeof(struct dp_subif_info *) * DP_MAX_CTP_PER_DEV,
			GFP_ATOMIC);
	if (!sif_ptr_arr)
		return DP_ERR_MEM;

	subif->flag_pmapper = 0;

	/*search sub-interfaces/VAP */
	for (i = 0; i < p_info->subif_max; i++) {
		struct dp_subif_info *sif = get_dp_port_subif(p_info, i);

		/* FOR DSL ATM case when netif is NULL, no need subif info */
		if (is_dsl(p_info) && (!netif))
			break;

		if (!sif->flags)
			continue;

		if (sif->ctp_dev == netif) { /*for PON pmapper case*/
			if (num > 0) {
				pr_err("DPM: Multiple same ctp_dev exist\n");
				goto EXIT;
			}
			sif_ptr_arr[num] = sif;
			subif->flag_bp = 0;
			subif->num_q = sif->num_qid;
			dp_memcpy(subif->def_qlist, sif->qid_list,
				  sizeof(subif->def_qlist));
			dp_memcpy(&subif->subif_common, &sif->subif_common,
				  sizeof(struct dp_subif_common));
			res = DP_SUCCESS;
			num++;
			break;
		}
		if (sif->netif == netif) {
			subif->flag_bp = 1;
			if (num >= DP_MAX_CTP_PER_DEV) {
				pr_err("DPM: %s: Why CTP over %d\n",
						netif->name,
						DP_MAX_CTP_PER_DEV);
				max_subif_over = true;
				goto UPDATE;
			}
			/* some dev may have multiple
			 * subif,like pon
			 */
			sif_ptr_arr[num] = sif;
			dp_memcpy(&subif->subif_common, &sif->subif_common,
				  sizeof(struct dp_subif_common));
			subif->num_q = sif->num_qid;
			dp_memcpy(subif->def_qlist, sif->qid_list,
				  sizeof(subif->def_qlist));
			if (sif->ctp_dev)
				subif->flag_pmapper = 1;
			if (num && subif->bport != sif->bp) {
				pr_err("DPM: %s:Why many bp:%d %d\n",
						netif->name, sif->bp,
						subif->bport);
				goto EXIT;
			}
			res = DP_SUCCESS;
			num++;
		}
		if (num != 0)
			continue;
		/*continue search non-explicate logical device */
		list_for_each_entry(tmp, &sif->logic_dev, list) {
			if (tmp->dev == netif) {
				dp_memcpy(&subif->subif_common,
					  &sif->subif_common,
					  sizeof(struct dp_subif_common));
				subif->subif_num = 1;
				subif->inst = inst;
				subif->subif_list[0] = tmp->ctp;
				subif->bport = tmp->bp;
				subif->port_id = port_id;
				sif_ptr_arr[num] = sif;
				subif->ctp_base = sif->ctp_base;
				subif->ctp = sif->ctp;
				res = DP_SUCCESS;
				/*note: logical device no callback */
				goto EXIT;
			}
		}
		subif->ctp_base = sif->ctp_base;
		subif->ctp = sif->ctp;
	}
UPDATE:
	subif->inst = inst;
	dp_memcpy(&subif->subif_port_cmn, &p_info->subif_port_cmn,
		  sizeof(struct dp_subif_port_common));
	subif->subif_num = num;
	for (i = 0; i < num; i++) {
		subif->subif_list[i] = sif_ptr_arr[i]->subif;
		subif->subif_flag[i] = sif_ptr_arr[i]->subif_flag;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		subif->gpid_list[i] = sif_ptr_arr[i]->gpid;
		dp_memcpy(subif->dfl_eg_sess_ext[i], sif_ptr_arr[i]->dfl_sess,
			  sizeof(sif_ptr_arr[i]->dfl_sess));
#endif
	}
	if (!max_subif_over)
		res = DP_SUCCESS;
EXIT:
	kfree(sif_ptr_arr);
	return res;
}

static int dp_build_cqm_data(int inst, u32 port_id,
		struct cbm_dp_alloc_complete_data *cbm_data,
		struct dp_dev_data *data)
{
	int i = 0;

	if (data->num_rx_ring > DP_RX_RING_NUM ||
			data->num_tx_ring > DP_TX_RING_NUM) {
		pr_err("DPM: Error RxRing = %d TxRing = %d\n",
				data->num_rx_ring, data->num_tx_ring);
		return DP_FAILURE;
	}

	/* HOST -> ACA */
	/* For PRX300 No: of Tx Ring is 1 */
	cbm_data->num_tx_ring = data->num_tx_ring;

	/* Pass TX ring info to CQM */
	for (i = 0; i < data->num_tx_ring; i++)
		cbm_data->tx_ring[i] = &data->tx_ring[i];

	/* ACA -> HOST */
	/* For PRX300 No: of Rx Ring is 1 */
	cbm_data->num_rx_ring = data->num_rx_ring;

	/* Pass RX ring info to CQM */
	for (i = 0; i < data->num_rx_ring; i++)
		cbm_data->rx_ring[i] = &data->rx_ring[i];

	cbm_data->num_umt_port = data->num_umt_port;
	cbm_data->num_qid = data->num_resv_q;
	cbm_data->qid_base = data->qos_resv_q_base;
	cbm_data->bm_policy_res_id = data->bm_policy_res_id;
	cbm_data->opt_param = data->opt_param;
	cbm_data->data = data;

	return 0;
}

static int dp_register_dc(int inst, u32 port_id,
				 struct dp_dev_data *data, u32 flags)
{
	struct pmac_port_info *port = get_dp_port_info(inst, port_id);
	struct cqm_port_info *deq;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct cqm_igp_info *enq;
#endif
	struct umt_port_res *res;
	struct umt_ops *ops = dp_get_umt_ops(inst);
	int dc_idx;
	u8 dc_umt_map_idx[DP_DC_NUM] = {0};
	int i, j = 0, ret = DP_SUCCESS;
	int dma_chan, deq_p0;
	int dma_ch_base;
	/* The number of deq ports returned by dp_alloc_port() API */
	int ndeq_port = port->deq_port_num;

	if (!(port->alloc_flags & DP_F_ACA))
		return DP_SUCCESS;

	if (!data->num_rx_ring) {/* non ACA case num_rx_ring is zero */
		pr_err("DPM: %s why dp_port %d ACA num_rx_ring zero\n",
			__func__, port_id);
		return DP_SUCCESS;
	}

	_DP_DEBUG(DP_DBG_FLAG_REG, "DPM: %s executing:\n", __func__);
	/* Fill in the output data to the the DCDP driver for the RX rings
	 * and Save Info for debugging
	 */
	for (i = 0; i < data->num_rx_ring; i++)
		port->rx_ring[i] = data->rx_ring[i];

	for (i = 0; i < data->num_tx_ring; i++)
		port->tx_ring[i] = data->tx_ring[i];

	/* Save info for debugging */
	port->num_rx_ring = data->num_rx_ring;
	port->num_tx_ring = data->num_tx_ring;
	port->rx_policy_base = data->rx_ring[0].rx_policy_base;
	port->rx_policy_num = data->rx_ring[0].policy_num;
	if (!port->rx_policy_num)
		port->rx_policy_num = 1;

	if (data->num_tx_ring) {
		port->tx_policy_base = data->tx_ring[0].txout_policy_base;
		port->tx_policy_num = data->tx_ring[0].policy_num;
	} else {
		port->tx_policy_base = port->rx_policy_base;
		port->tx_policy_num = port->rx_policy_num;
	}

	/* update deq_ports and deq_port_num */
	for (i = 0; i < data->num_tx_ring; i++) {
		/* If a tx_ring[x].not_valid == true, then
		 * dont update info deq_port table
		 * Ex: docsis
		 */
		if (data->tx_ring[i].not_valid)
			continue;
		/* Sanity check: dp_alloc_port_ext2() and dp_register_dev_ext2(),
		 * should return consistent dequeue port, condition as below
		 * deq_port_base <= tx_ring[x].tx_deq_port < (deq_port_base + deq_port_num)
		 */
		if (ndeq_port && ((port->deq_port_base > data->tx_ring[i].tx_deq_port) ||
					(data->tx_ring[i].tx_deq_port >= (port->deq_port_base + ndeq_port)))) {
			pr_err("DPM: %s %d: CQM deq port sanity check failed\n"
				"iter: %d deq_port_num: %d deq_port_base: %d "
				"txring[%d].tx_deq_port: %d\n",
				__func__, __LINE__, i, ndeq_port,
				port->deq_port_base, i,
				data->tx_ring[i].tx_deq_port);
			return DP_FAILURE;
		}

		port->deq_ports[j++] = data->tx_ring[i].tx_deq_port;
		port->deq_port_num = j;
	}

	/* dp_alloc_port_ext2() for ACA device return no deq_ports, so
	 * deq_port_num is 0. But here it should be overwritten.
	 * Hence here it should be > 0
	 */
	if (!port->deq_port_num) {
		pr_err("DPM: %s: DP portid: %d, still the deq_port_num=%d\n",
			__func__, port_id, port->deq_port_num);
		return DP_FAILURE;
	}

	/* dp_alloc_port_ext2() for ACA device return no deq_ports, so
	 * deq_port_base is 0, Hence setting it here for ACA devices
	 */
	if (!port->deq_port_base)
		port->deq_port_base = port->deq_ports[0];

	/* UMT Interface is not supported for old products */
	if (get_dp_port_prop(inst)->info.type == GSWIP30_TYPE) {
		 _DP_DEBUG(DP_DBG_FLAG_REG, "   Returning as product type: "
			  "GSWIP30_TYPE(umt not supported)\n");
		return DP_SUCCESS;
	}

	port->num_umt_port = data->num_umt_port;

	if (!ops) {
		pr_err("DPM: %s, No UMT driver is registered\n", __func__);
		return -ENODEV;
	}

	/* set dequeue port information */
	dma_chan =  port->dma_chan;
	dma_ch_base = port->dma_chan_tbl_idx;
	deq_p0 = port->tx_ring[0].tx_deq_port;
	for (i = 0; i < port->num_tx_ring; i++) {
		if (port->tx_ring[i].not_valid) {
		 	_DP_DEBUG(DP_DBG_FLAG_REG,
				"   TxRing[%d]: not_valid flag is set\n", i);
			continue;
		}
		deq = get_dp_deqport_info(inst, port->tx_ring[i].tx_deq_port);
		_DP_DEBUG(DP_DBG_FLAG_REG,
			"   TxRing[%d]: DC deq_port_num=%d deq_port=%d deq_port0=%d\n",
			i, port->num_tx_ring, port->tx_ring[i].tx_deq_port,
			deq_p0);
		deq->txpush_addr = port->txpush_addr +
			(port->tx_ring_offset * (port->tx_ring[i].tx_deq_port - deq_p0));
		deq->txpush_addr_qos = port->txpush_addr_qos +
			(port->tx_ring_offset * (port->tx_ring[i].tx_deq_port - deq_p0));
		deq->tx_ring_size = port->tx_ring_size;
		deq->tx_pkt_credit = port->tx_pkt_credit;
		deq->dp_port[port_id] = 1;
		deq->dts_qos = port->dts_qos;

		/* For G.INT num_dma_chan 8 or 16, for other 1 */
		if (port->num_dma_chan > 1) {
			deq->f_dma_ch = 1;
			deq->dma_chan = dma_chan++;
			deq->dma_ch_offset = dma_ch_base + i;
		} else if (port->num_dma_chan == 1) {
			deq->f_dma_ch = 1;
			deq->dma_chan = dma_chan;
			deq->dma_ch_offset = dma_ch_base;
		} else {
			deq->dma_chan = 0;
			deq->dma_ch_offset = 0;
		}
		_DP_DEBUG(DP_DBG_FLAG_REG, "   txring[%d]: tx_deq_port[%d][%d].dma_chan=%x\n",
				i, inst, port->tx_ring[i].tx_deq_port, dma_chan);
	}

	for (i = 0; i < port->num_umt_port; i++) {
		dc_idx = data->umt_dc[i].dc_idx;
		_DP_DEBUG(DP_DBG_FLAG_REG,
			"   UMT->DC Mapping: UMT idx(%d) --> DC idx(%d)\n",
			i, dc_idx);
		res = &data->umt[i].res;
		res->cqm_dq_pid = data->tx_ring[dc_idx].tx_deq_port;
		res->cqm_enq_pid = data->rx_ring[dc_idx].out_enq_port_id;
		res->dma_id = data->rx_ring[dc_idx].out_dma_ch_to_gswip;
		res->dma_ch_num = data->rx_ring[dc_idx].num_out_tx_dma_ch;

		/* For PRX300, RXOUT is to DMA Channel,
		 * For LGM, RXOUT is to CQEM Deq port
		 */
		if (is_soc_prx(inst))
			res->rx_src = UMT_RX_SRC_DMA;

		deq = get_dp_deqport_info(inst, res->cqm_dq_pid);

		if (data->umt_dc[i].f_not_alloc)
			goto update;

		if (deq->umt_info[dc_umt_map_idx[dc_idx]].umt_valid) {
			pr_err("DPM: %s %d, UMT idx already valid, umt: %d, still trying to allocate\n"
					"DPM: %s %d May result in umt leak\n",
					__func__, __LINE__,
					deq->umt_info[dc_umt_map_idx[dc_idx]].umt.ctl.id,
					__func__, __LINE__);
			return DP_FAILURE;
		}

		if (is_soc_lgm(inst)) {
			ret = ops->umt_alloc(ops->umt_dev, &data->umt[i]);
			if (ret < 0) {
				pr_err("DPM: %s: umt %d alloc failed with %d\n",
				       __func__, i, ret);
				return DP_FAILURE;
			}
		}

		ret = ops->umt_request(ops->umt_dev, &data->umt[i]);
		if (ret) {
			pr_err("DPM: %s: umt %d request failed with %d\n",
			       __func__, i, ret);
			return DP_FAILURE;
		}
update:
		port->umt_dc_map_idx[i] = data->umt_dc[i].dc_idx;
		if (!deq->umt_info[dc_umt_map_idx[dc_idx]].umt_valid) {
			deq->umt_info[dc_umt_map_idx[dc_idx]].umt_valid = true;
			deq->umt_info[dc_umt_map_idx[dc_idx]].umt =
								data->umt[i];
		}
		deq->dp_port[port_id] = 1;
		deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt++;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		/* For PRX, there is no rxin_ring. So no out enq pid */
		enq = get_dp_enqport_info(inst, res->cqm_enq_pid);
		enq->ref_cnt++;
		enq->dp_port[port_id]++;
		enq->dc_port[port_id][dc_idx]++;
		enq->umt_port[deq->umt_info[dc_umt_map_idx[dc_idx]].umt.ctl.id]++;
		enq->igp = res->cqm_enq_pid;
#endif
		port->umt[i] = &deq->umt_info[dc_umt_map_idx[dc_idx]].umt;

		_DP_DEBUG(DP_DBG_FLAG_REG, "   dc_umt_map_idx[%d]: %d, "
			 "ref_cnt_umt: %d \n",
			 dc_idx, dc_umt_map_idx[dc_idx],
			 deq->umt_info[dc_umt_map_idx[dc_idx]].ref_cnt_umt);
		dc_umt_map_idx[dc_idx]++;
	}

	for (i = 0; i < data->num_rx_ring; i++) {
		/* There may have multiple Dequeue port and multi DMA to
		 * GSIWP to serve diff priority
		 */
		for (j = 0; j < data->rx_ring[i].num_out_cqm_deq_port; j++) {
			deq_p0 = data->rx_ring[i].out_cqm_deq_port_id + j;
			deq = get_dp_deqport_info(inst, deq_p0);
			deq->dp_port[port_id] = 1;
		}
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* For PRX, there is no rxin_ring. So dummy read is not applicable in PRX */
	for (i = 0; i < data->num_rx_ring; i++) {
		enq = get_dp_enqport_info(inst, data->rx_ring[i].out_enq_port_id);
		if ((!data->dc_info[i].no_bm_dummy_read) &&
				(!enq->dc_dummy_read)) {
			/* do a dummy read of buffers to trigger the UMT */
			if (cqm_dc_buffer_dummy_request(data->rx_ring[i].out_enq_port_id,
						-1)) {
				pr_err("DPM: %s: dc enq dummy buf req failed on port %d\n",
				       __func__, data->rx_ring[i].out_enq_port_id);
				return DP_FAILURE;
			}
			enq->dc_dummy_read = 1;
			_DP_DEBUG(DP_DBG_FLAG_REG, "   RxRing[%d]: dc enq dummy"
					" buffer req succeeded on port %d\n", i,
				data->rx_ring[i].out_enq_port_id);
		}
	}
#endif

	return DP_SUCCESS;
}

int dp_get_port_subitf_via_dev(struct net_device *dev, dp_subif_t *subif)
{
	int res;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	res = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	return res;
}
EXPORT_SYMBOL(dp_get_port_subitf_via_dev);

int dp_get_port_subitf_via_ifname_private(char *ifname, dp_subif_t *subif)
{
	int i, j;
	int inst;
	struct inst_info *dp_info;

	inst = dp_get_inst_via_dev(NULL, ifname, 0);
	dp_info = get_dp_prop_info(inst);

	for (i = 0; i < dp_info->cap.max_num_dp_ports; i++) {
		struct pmac_port_info *port = get_dp_port_info(inst, i);

		for (j = 0; j < port->subif_max; j++) {
			struct dp_subif_info *sif = get_dp_port_subif(port, j);

			if (strcmp(sif->device_name, ifname) == 0) {
				subif->port_id = i;
				subif->subif = SET_VAP(j, port->vap_offset,
						       port->vap_mask);
				subif->inst = inst;
				subif->bport = sif->bp;
				return DP_SUCCESS;
			}
		}
	}

	return DP_FAILURE;
}

int dp_get_port_subitf_via_ifname(char *ifname, dp_subif_t *subif)
{
	int res;
	struct net_device *dev;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!ifname)
		return -1;
	dev = dev_get_by_name(&init_net, ifname);
	if (!dev)
		return -1;
	res = dp_get_port_subitf_via_dev(dev, subif);
	dev_put(dev);
	return res;
}
EXPORT_SYMBOL(dp_get_port_subitf_via_ifname);

struct module *dp_get_module_owner(int ep)
{
	int inst = 0; /*here hardcode for PPA only */

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed for dp not init yet\n", __func__);
		return NULL;
	}

	if (ep >= 0 && ep < get_dp_prop_info(inst)->cap.max_num_dp_ports)
		return get_dp_port_info(inst, ep)->owner;

	return NULL;
}
EXPORT_SYMBOL(dp_get_module_owner);

/*if subif->vap == -1, it means all vap */
void dp_clear_mib(dp_subif_t *subif, u32 flag)
{
	int i, j, start_vap, end_vap;
	dp_reset_mib_fn_t reset_mib_fn;
	struct pmac_port_info *port_info;

	if (!subif || is_invalid_port(subif->port_id)) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "Wrong subif\n");
		return;
	}

	i = subif->port_id;
	port_info = get_dp_port_info(subif->inst, i);

	if (subif->subif == -1) {
		start_vap = 0;
		end_vap = port_info->num_subif;
	} else {
		start_vap = GET_VAP(subif->subif, port_info->vap_offset,
				    port_info->vap_mask);
		end_vap = start_vap + 1;
	}

	for (j = start_vap; j < end_vap; j++) {
		struct dp_subif_info *sif = get_dp_port_subif(port_info, i);
		struct dev_mib *mib = get_dp_port_subif_mib(sif);

		STATS_SET(port_info->tx_err_drop, 0);
		STATS_SET(port_info->rx_err_drop, 0);
		dp_memset(mib, 0, sizeof(struct dev_mib));
		reset_mib_fn = port_info->cb.reset_mib_fn;

		if (reset_mib_fn)
			reset_mib_fn(subif, 0);
	}
}

void dp_clear_all_mib_inside(u32 flag)
{
	dp_subif_t *subif;
	int i;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return;

	for (i = 0; i < MAX_DP_PORTS; i++) {
		subif->port_id = i;
		subif->subif = -1;
		dp_clear_mib(subif, flag);
	}
	kfree(subif);
}

int dp_get_drv_mib(dp_subif_t *subif, dp_drv_mib_t *mib, u32 flag)
{
	dp_get_mib_fn_t get_mib_fn;
	dp_drv_mib_t tmp;
	int i, vap;
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT,
			 "failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	if (!subif || !mib)
		return -1;
	dp_memset(mib, 0, sizeof(*mib));
	port_info = get_dp_port_info(subif->inst, subif->port_id);
	vap = GET_VAP(subif->subif, port_info->vap_offset,
		      port_info->vap_mask);
	get_mib_fn = port_info->cb.get_mib_fn;

	if (!get_mib_fn)
		return -1;

	if (!(flag & DP_F_STATS_SUBIF)) {
		/*get all VAP's  mib counters if it is -1 */
		for (i = 0; i < port_info->num_subif; i++) {
			sif = get_dp_port_subif(port_info, i);
			if (!sif->flags)
				continue;

			subif->subif = sif->subif;
			dp_memset(&tmp, 0, sizeof(tmp));
			get_mib_fn(subif, &tmp, flag);
			mib->rx_drop_pkts += tmp.rx_drop_pkts;
			mib->rx_error_pkts += tmp.rx_error_pkts;
			mib->tx_drop_pkts += tmp.tx_drop_pkts;
			mib->tx_error_pkts += tmp.tx_error_pkts;
		}
	} else {
		sif = get_dp_port_subif(port_info, vap);
		if (sif->flags)
			get_mib_fn(subif, mib, flag);
	}

	return 0;
}

int dp_get_netif_stats2(struct net_device *dev, dp_subif_t *subif_id,
			struct rtnl_link_stats64 *stats, u32 flags)
{
	dp_subif_t *subif;
	int res;
	int (*get_mib)(dp_subif_t *subif_id, void *priv,
		       struct rtnl_link_stats64 * stats,
		       u32 flags);

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (subif_id) {
		*subif = *subif_id;
	} else if (dev) {
		res = dp_get_port_subitf_via_dev(dev, subif);
		if (res) {
			DP_DEBUG(DP_DBG_FLAG_MIB,
				 "fail:%s not registered yet to datapath\n",
				 dev->name);
			kfree(subif);
			return DP_FAILURE;
		}
	} else {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "dev/subif_id both NULL\n");
		kfree(subif);
		return DP_FAILURE;
	}
	get_mib = get_dp_prop_info(subif->inst)->dp_get_port_vap_mib;
	if (!get_mib) {
		kfree(subif);
		return DP_FAILURE;
	}

	res = get_mib(subif, NULL, stats, flags);
	kfree(subif);
	return res;

}
EXPORT_SYMBOL(dp_get_netif_stats2);

int dp_clear_netif_stats(struct net_device *dev, dp_subif_t *subif_id,
			 u32 flag)
{
	dp_subif_t *subif;
	int (*clear_netif_mib_fn)(dp_subif_t *subif, void *priv, u32 flag);
	int i, res;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (subif_id) {
		clear_netif_mib_fn =
			get_dp_prop_info(subif_id->inst)->dp_clear_netif_mib;
		if (!clear_netif_mib_fn)
			return -1;
		return clear_netif_mib_fn(subif_id, NULL, flag);
	}
	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dev) {
		if (dp_get_port_subitf_via_dev(dev, subif)) {
			DP_DEBUG(DP_DBG_FLAG_MIB, "not register to %s\n",
				 dev->name);
			kfree(subif);
			return -1;
		}
		clear_netif_mib_fn =
			get_dp_prop_info(subif->inst)->dp_clear_netif_mib;
		if (!clear_netif_mib_fn) {
			kfree(subif);
			return -1;
		}
		res = clear_netif_mib_fn(subif, NULL, flag);
		kfree(subif);
		return res;
	}
	/*clear all */
	for (i = 0; i < DP_MAX_INST; i++) {
		clear_netif_mib_fn = get_dp_prop_info(i)->dp_clear_netif_mib;
		if (!clear_netif_mib_fn)
			continue;
		clear_netif_mib_fn(NULL, NULL, flag);
	}
	kfree(subif);
	return 0;
}
EXPORT_SYMBOL(dp_clear_netif_stats);

int dp_pmac_set(int inst, u32 port, dp_pmac_cfg_t *pmac_cfg)
{
	int (*dp_pmac_set_fn)(int inst, u32 port, dp_pmac_cfg_t *pmac_cfg);

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (is_invalid_inst(inst))
		return DP_FAILURE;

	dp_pmac_set_fn =  get_dp_prop_info(inst)->dp_pmac_set;
	if (!dp_pmac_set_fn)
		return DP_FAILURE;
	return dp_pmac_set_fn(inst, port, pmac_cfg);
}
EXPORT_SYMBOL(dp_pmac_set);

/*\brief Datapath Manager Pmapper Configuration Set
 *\param[in] dev: network device point to set pmapper
 *\param[in] mapper: buffer to get pmapper configuration
 *\param[in] flag: reserve for future
 *\return Returns 0 on succeed and -1 on failure
 *\note  for pcp mapper case, all 8 mapping must be configured properly
 *       for dscp mapper case, all 64 mapping must be configured properly
 *       def ctp will match non-vlan and non-ip case
 *	For drop case, assign CTP value == DP_PMAPPER_DISCARD_CTP
 */
int dp_set_pmapper(struct net_device *dev, struct dp_pmapper *mapper, u32 flag)
{
	int inst, ret, bport, i;
	dp_subif_t *subif;
	struct dp_pmapper *map = NULL;
	int res = DP_FAILURE;
	struct bp_pmapper *bp_info;
	struct inst_info *dp_info;

	if (!dev || !mapper) {
		pr_err("DPM: dev or mapper is NULL\n");
		return DP_FAILURE;
	}

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "Failed for datapath not init yet\n");
		return DP_FAILURE;
	}
	if (mapper->mode >= DP_PMAP_MAX) {
		pr_err("DPM: mapper->mode(%d) out of range %d\n",
		       mapper->mode, DP_PMAP_MAX);
		return DP_FAILURE;
	}
	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	/* get the subif from the dev */
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret == DP_FAILURE) {
		pr_err("DPM: Fail to get subif:dev=%s ret=%d flag_pmap=%d bp=%d\n",
		       dev->name, ret, subif->flag_pmapper, subif->bport);
		kfree(subif);
		return DP_FAILURE;
	}
	inst = subif->inst;

	dp_info = get_dp_prop_info(inst);

	if (!dp_info->dp_set_gsw_pmapper) {
		pr_err("DPM: Set pmapper is not supported\n");
		return DP_FAILURE;
	}

	bport = subif->bport;
	if (bport >= DP_MAX_BP_NUM) {
		kfree(subif);
		pr_err("DPM: BP port(%d) out of range %d\n", bport, DP_MAX_BP_NUM);
		return DP_FAILURE;
	}
	map = dp_kzalloc(sizeof(*map), GFP_ATOMIC);
	if (!map) {
		kfree(subif);
		return DP_FAILURE;
	}
	dp_memcpy(map, mapper, sizeof(*map));

	switch (mapper->mode) {
	case DP_PMAP_PCP:
	case DP_PMAP_DSCP:
		map->mode = GSW_PMAPPER_MAPPING_PCP;
		break;
	case DP_PMAP_DSCP_ONLY:
		map->mode = GSW_PMAPPER_MAPPING_DSCP;
		break;
	default:
		pr_err("DPM: Unknown mapper mode: %d\n", map->mode);
		goto EXIT;
	}
	/* workaround in case caller forget to set to default ctp */
	if (mapper->mode == DP_PMAP_PCP)
		for (i = 0; i < DP_PMAP_DSCP_NUM; i++)
			map->dscp_map[i] = mapper->def_ctp;

	ret = dp_info->dp_set_gsw_pmapper(inst, bport, subif->port_id, map,
					  flag);
	if (ret == DP_FAILURE) {
		pr_err("DPM: Failed to set mapper\n");
		goto EXIT;
	}

	bp_info = get_dp_bp_info(inst, bport);

	/* update local table for pmapper */
	bp_info->def_ctp = map->def_ctp;
	bp_info->mode = mapper->mode; /* original mode */
	for (i = 0; i < DP_PMAP_PCP_NUM; i++)
		bp_info->pcp[i] = map->pcp_map[i];
	for (i = 0; i < DP_PMAP_DSCP_NUM; i++)
		bp_info->dscp[i] = map->dscp_map[i];
	res = DP_SUCCESS;
EXIT:
	kfree(map);
	kfree(subif);
	return res;
}
EXPORT_SYMBOL(dp_set_pmapper);

/*\brief Datapath Manager Pmapper Configuration Get
 *\param[in] dev: network device point to set pmapper
 *\param[out] mapper: buffer to get pmapper configuration
 *\param[in] flag: reserve for future
 *\return Returns 0 on succeed and -1 on failure
 *\note  for pcp mapper case, all 8 mapping must be configured properly
 *       for dscp mapper case, all 64 mapping must be configured properly
 *       def ctp will match non-vlan and non-ip case
 *	 For drop case, assign CTP value == DP_PMAPPER_DISCARD_CTP
 */
int dp_get_pmapper(struct net_device *dev, struct dp_pmapper *mapper, u32 flag)
{
	int inst, ret, bport;
	dp_subif_t *subif;
	struct inst_info *dp_info;

	if (!dev || !mapper) {
		pr_err("DPM: The parameter dev or mapper can not be NULL\n");
		return DP_FAILURE;
	}

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "Failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	/*get the subif from the dev*/
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret == DP_FAILURE) {
		kfree(subif);
		pr_err("DPM: Can not get the subif from the dev\n");
		return DP_FAILURE;
	}
	inst = subif->inst;

	dp_info = get_dp_prop_info(inst);

	if (!dp_info->dp_get_gsw_pmapper) {
		kfree(subif);
		pr_err("DPM: Get pmapper is not supported\n");
		return DP_FAILURE;
	}

	bport = subif->bport;
	if (bport > DP_MAX_BP_NUM) {
		kfree(subif);
		pr_err("DPM: BP port(%d) out of range %d\n", bport, DP_MAX_BP_NUM);
		return DP_FAILURE;
	}
	/* init the subif into the dp_port_info*/
	/* call the switch api to get the HW*/
	ret = dp_info->dp_get_gsw_pmapper(inst, bport, subif->port_id, mapper,
					  flag);
	if (ret == DP_FAILURE) {
		kfree(subif);
		pr_err("DPM: Failed to get mapper\n");
		return DP_FAILURE;
	}
	kfree(subif);
	return ret;
}
EXPORT_SYMBOL(dp_get_pmapper);

int32_t dp_rx2(struct sk_buff *skb, u32 flags)
{
	struct sk_buff *next;
	int res = -1;
	int inst = 0;
	struct inst_info *dp_info = get_dp_prop_info(inst);

	if (unlikely(!dp_init_ok)) {
		while (skb) {
			next = skb->next;
			skb->next = 0;
			dev_kfree_skb_any(skb);
			skb = next;
		}
	}

	while (skb) {
		next = skb->next;
		skb->next = 0;
		res = dp_info->dp_rx(skb, flags);
		skb = next;
	}

	return res;
}
EXPORT_SYMBOL(dp_rx2);

int dp_lan_wan_bridging(int port_id, struct sk_buff *skb)
{
	dp_subif_t *subif;
	struct net_device *dev;
	static int lan_port = 4;
	int inst = 0, ret = DP_SUCCESS;
	struct dp_subif_info *sif;

	if (!skb)
		return DP_FAILURE;

	skb_pull(skb, 8);	/*remove pmac */

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	if (port_id == 15) {
		/*recv from WAN and forward to LAN via lan_port */
		subif->port_id = lan_port;	/*send to last lan port */
		subif->subif = 0;
	} else if (port_id <= 6) { /*recv from LAN and forward to WAN */
		subif->port_id = 15;
		subif->subif = 0;
		lan_port = port_id;	/*save lan port id */
	} else {
		dev_kfree_skb_any(skb);
		kfree(subif);
		return DP_FAILURE;
	}

	sif = get_dp_port_subif(get_dp_port_info(inst, subif->port_id), 0);
	dev = sif->netif;

	if (!sif->flags || !dev) {
		dev_kfree_skb_any(skb);
		kfree(subif);
		return DP_FAILURE;
	}

	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = subif->port_id;
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id =
		subif->subif;

	ret = dp_xmit(dev, subif, skb, skb->len, 0);
	kfree(subif);
	return ret;
}

void set_chksum(struct pmac_tx_hdr *pmac, u32 tcp_type,
		u32 ip_offset, int ip_off_hw_adjust,
		u32 tcp_h_offset)
{
	pmac->tcp_type = tcp_type;
	pmac->ip_offset = ip_offset + ip_off_hw_adjust;
	pmac->tcp_h_offset = tcp_h_offset >> 2;
}

int32_t dp_xmit2(struct net_device *rx_if, dp_subif_t *rx_subif,
		struct sk_buff *skb, int32_t len, u32 flags)
{
	int inst = 0;
	struct dp_tx_common_ex ex = {
		.cmn = {
			.inst = 0,
			.flags = flags,
			.subif = rx_subif->subif,
			.tx_portid = rx_subif->port_id,
			.toe_tc = TOE_TC_DEF_VALUE,
		},
	};
	struct pmac_port_info *port;
	u32 vap;
	struct inst_info *dp_info = get_dp_prop_info(inst);
	enum DP_TX_FN_RET ret = DP_TX_FN_DROPPED;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed for dp no init yet\n", __func__);
		dev_kfree_skb_any(skb);
		goto exit;
	}

	if (unlikely(!rx_subif)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "%s failed for rx_subif null\n", __func__);
		dev_kfree_skb_any(skb);
		goto exit;
	}

	if (unlikely(!skb)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "%s skb NULL\n", __func__);
		dev_kfree_skb_any(skb);
		goto exit;
	}

	if (unlikely(in_irq())) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "%s not allowed in interrupt context\n",
				   __func__);
		dev_kfree_skb_any(skb);
		goto exit;
	}
#endif

	if (unlikely(rx_subif->port_id >=
		     dp_info->cap.max_num_dp_ports)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "rx_subif->port_id >= max_ports");
		UP_STATS(get_dp_port_info(inst, 0)->tx_err_drop);
		MIB_G_STATS_INC(tx_drop);
		dev_kfree_skb_any(skb);
		goto exit;
	}

	port = get_dp_port_info(inst, rx_subif->port_id);
	if (unlikely(!rx_if && !is_dsl(port))) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "null dev but not DSL\n");
		dev_kfree_skb_any(skb);
		goto exit;
	}

	ex.dev = rx_if;
	ex.rx_subif = rx_subif;
	ex.port = port;
	vap = GET_VAP(ex.cmn.subif, port->vap_offset, port->vap_mask);
	ex.sif = get_dp_port_subif(port, vap);
	ex.mib = get_dp_port_subif_mib(ex.sif);
	ex.cmn.alloc_flags = port->alloc_flags;
	ret = dp_info->dp_tx(skb, &ex.cmn);
	if (likely(ret == DP_TX_FN_CONSUMED)) {
		MIB_G_STATS_INC(tx_pkts);
	} else if (ret != DP_TX_FN_BUSY) {
		if (ret == DP_TX_FN_CONTINUE)
			ret = DP_TX_FN_DROPPED;

		UP_STATS(ex.mib->tx_pkt_dropped);
		MIB_G_STATS_INC(tx_drop);

		/* In LGM Fail case DPM free skb, Success case CQM Free
		 * In PRX Fail and Success case CQM free
		 */
		if (is_soc_lgm(ex.cmn.inst))
			dev_kfree_skb_any(skb);
	}

	if (flags & DP_TX_NEWRET)
		return ret;

	if (ret == DP_TX_FN_BUSY) {
		UP_STATS(ex.mib->tx_pkt_dropped);
		MIB_G_STATS_INC(tx_drop);
		dev_kfree_skb_any(skb);
	}
exit:
	return ret ? DP_FAILURE : DP_SUCCESS;
}
EXPORT_SYMBOL(dp_xmit2);

void set_dp_dbg_flag(u64 flags)
{
	dp_dbg_flag = flags;
}

u64 get_dp_dbg_flag(void)
{
	return dp_dbg_flag;
}

u64 get_dp_dbgfs_flag(void)
{
	return dp_dbgfs_flag;
}

void set_dp_dbgfs_flag(u64 flags)
{
	dp_dbgfs_flag = flags;
}

/*!
 *@brief  The API is for dp_get_cap
 *@param[in,out] cap dp_cap pointer, caller must provide the buffer
 *@param[in] flag for future
 *@return 0 if OK / -1 if error
 */
int dp_get_cap(struct dp_cap *cap, int flag)
{
	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cap || is_invalid_inst(cap->inst))
		return DP_FAILURE;
	if (!hw_cap_list[cap->inst].valid)
		return DP_FAILURE;
	*cap = hw_cap_list[cap->inst].info.cap;

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_get_cap);

int dp_rx_enable2(struct net_device *netif, char *ifname, u32 flags)
{
	dp_subif_t *subif;
	struct pmac_port_info *port_info;
	int vap, i;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	if (dp_get_netif_subifid(netif, NULL, NULL, NULL, subif, 0)) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "get subifid fail(%s)\n",
			 netif ? netif->name : "NULL");
		kfree(subif);
		return DP_FAILURE;
	}
	port_info = get_dp_port_info(subif->inst, subif->port_id);
	for (i = 0; i < subif->subif_num; i++) {
		vap = GET_VAP(subif->subif_list[i], port_info->vap_offset,
				port_info->vap_mask);
		STATS_SET(get_dp_port_subif(port_info, vap)->rx_flag,
			  flags ? 1 : 0);
	}

	kfree(subif);
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_rx_enable2);

int dp_vlan_set2(struct dp_tc_vlan *vlan, int flags)
{
	dp_subif_t *subif;
	struct dp_tc_vlan_info info = {0};
	struct pmac_port_info *port_info;
	int res;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	if (dp_get_netif_subifid(vlan->dev, NULL, NULL, NULL, subif, 0)) {
		kfree(subif);
		return DP_FAILURE;
	}
	port_info = get_dp_port_info(subif->inst, subif->port_id);
	info.subix = GET_VAP(subif->subif, port_info->vap_offset,
			     port_info->vap_mask);
	info.bp = subif->bport;
	info.dp_port = subif->port_id;
	info.inst = subif->inst;

	if (vlan->def_apply == DP_VLAN_APPLY_CTP && subif->flag_pmapper == 1) {
		pr_err("DPM: cannot apply VLAN rule for pmapper device\n");
		kfree(subif);
		return DP_FAILURE;
	} else if (vlan->def_apply == DP_VLAN_APPLY_CTP) {
		info.dev_type = 0;
	} else {
		info.dev_type |= subif->flag_bp;
	}
	switch (vlan->mcast_flag) {
	case DP_MULTICAST_SESSION:
		info.dev_type |= BIT(1);
		break;
	case DP_NON_MULTICAST_SESSION:
		info.dev_type |= BIT(2);
		break;
	}
	DP_DEBUG(DP_DBG_FLAG_PAE, "dev_type:0x%x\n", info.dev_type);
	if (DP_CB(subif->inst, dp_tc_vlan_set)) {
		res = DP_CB(subif->inst, dp_tc_vlan_set)
			    (dp_port_prop[subif->inst].ops[0],
			    vlan, &info, flags);
		kfree(subif);
		return res;
	}

	kfree(subif);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_vlan_set2);

int dp_set_bp_attr2(struct dp_bp_attr *conf, u32 flag)
{
	struct pmac_port_info *port_info;
	dp_subif_t *subif;
	int ret = DP_SUCCESS;
	struct inst_info *dp_info;
	int vap;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!conf) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "conf passed is (%s)\n",
			 conf ? conf->dev->name : "NULL");
		return DP_FAILURE;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	DP_LIB_LOCK(&dp_lock);

	if (dp_get_netif_subifid(conf->dev, NULL, NULL, NULL, subif, 0)) {
		DP_LIB_UNLOCK(&dp_lock);
		kfree(subif);
		return DP_FAILURE;
	}

	port_info = get_dp_port_info(conf->inst, subif->port_id);
	vap = GET_VAP(subif->subif, port_info->vap_offset,
		      port_info->vap_mask);
	get_dp_port_subif(port_info, vap)->cpu_port_en = conf->en;
	DP_LIB_UNLOCK(&dp_lock);

	dp_info = get_dp_prop_info(subif->inst);

	/* Null check is needed since some platforms dont have this API */
	if (!dp_info->dp_set_bp_attr) {
		kfree(subif);
		return DP_FAILURE;
	}

	ret = dp_info->dp_set_bp_attr(conf, subif->bport, flag);

	kfree(subif);
	return ret;
}
EXPORT_SYMBOL(dp_set_bp_attr2);

int dp_lookup_mode_cfg(int inst, u32 lu_mode, struct dp_q_map *map,
		       u32 flag)
{
	cbm_queue_map_entry_t entry = {0};
	int ret = DP_SUCCESS;
	struct pmac_port_info *port_info;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (is_invalid_port(map->dp_port))
		return DP_FAILURE;

	port_info = get_dp_port_info(inst, map->dp_port);
	if (!port_info)
		return DP_FAILURE;

	DP_LIB_LOCK(&dp_lock);
	entry.ep = map->dp_port;
	if (!is_soc_lgm(inst)) {
		entry.mpe1 = map->mpe1;
		entry.mpe2 = map->mpe2;
	}
	if (flag & DP_CQM_LU_MODE_GET) {
		ret = CBM_OPS(inst, cqm_mode_table_get, inst, &lu_mode,
			      &entry, 0);
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "GET: inst: %d, dp_port: %d, lu_mode: %u\n",
			 inst, map->dp_port, lu_mode);
	} else {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "SET: inst: %d, dp_port: %d, lu_mode: %u, flags: 0x%x\n",
			 inst, map->dp_port, lu_mode,
			 CBM_QUEUE_MAP_F_MPE1_DONTCARE|CBM_QUEUE_MAP_F_MPE2_DONTCARE);
		ret = CBM_OPS(inst, cqm_mode_table_set, inst, &entry, lu_mode,
			      CBM_QUEUE_MAP_F_MPE1_DONTCARE |
			      CBM_QUEUE_MAP_F_MPE2_DONTCARE);
		port_info->cqe_lu_mode = lu_mode;
	}
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_lookup_mode_cfg);

/*Return the table entry index based on dev:
 *success: >=0
 *fail: DP_FAILURE
 */
int bp_pmapper_get(int inst, struct net_device *dev)
{
	int i;
	struct bp_pmapper *bp_info;

	if (!dev)
		return -1;
	for (i = 0; i < ARRAY_SIZE(dp_bp_tbl[inst]); i++) {
		bp_info = get_dp_bp_info(inst, i);
		if (!bp_info->flag)
			continue;
		if (bp_info->dev == dev) {
			DP_DEBUG(DP_DBG_FLAG_PAE, "matched %s\n", dev->name);
			return i;
		}
	}
	return -1;
}

#if !IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
/* ethtool statistics support */
void dp_net_dev_get_ss_stat_strings(struct net_device *dev, u8 *data)
{
	/* This is for the Mib counter wraparound module */
	if (dp_get_dev_ss_stat_strings_fn) {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "MIB call dp_get_dev_ss_stat_strings_fn callback\n");
		dp_get_dev_ss_stat_strings_fn(dev, data);
	}

	return;
}
EXPORT_SYMBOL(dp_net_dev_get_ss_stat_strings);

int dp_net_dev_get_ss_stat_strings_count(struct net_device *dev)
{
	/* This is for the Mib counter wraparound module */
	if (dp_get_dev_stat_strings_count_fn) {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "MIB call dp_get_dev_stat_strings_count_fn callback\n");
		return dp_get_dev_stat_strings_count_fn(dev);
	}

	return 0;
}
EXPORT_SYMBOL(dp_net_dev_get_ss_stat_strings_count);

void dp_net_dev_get_ethtool_stats(struct net_device *dev,
				  struct ethtool_stats *stats, u64 *data)
{
	/* This is for the Mib counter wraparound module */
	if (dp_dev_get_ethtool_stats_fn) {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "MIB call dp_dev_get_ethtool_stats_fn callback\n");
		return dp_dev_get_ethtool_stats_fn(dev, stats, data);
	}

	return;
}
EXPORT_SYMBOL(dp_net_dev_get_ethtool_stats);
#endif /* CONFIG_DPM_DATAPATH_PARTIAL_FEEDS */

int dp_spl_conn2(int inst, struct dp_spl_cfg *conn)
{
	struct inst_info *dp_info = get_dp_prop_info(inst);
	int res;

	if (is_invalid_inst(inst))
		return DP_FAILURE;

	if (!dp_late_init())
		return DP_FAILURE;

	if (!dp_info->dp_spl_conn)
		return DP_FAILURE;

	res = dp_info->dp_spl_conn(inst, conn);
	if (res || !conn->dev)
		return res;
	/* To add dp_spl_conn devices to dp_dev list for PPA to change the
	 * ndo_xxx in some ops via dp_set_net_dev_ops API, not for switchdev
	 * purpose at all.
	 * in old design, conn->dev will be auto-added into dp_dev list during
	 * dp_set_net_dev_ops if necessary. But it may cause memory
	 * leakage if caller forget to reset the ops for this device.
	 * In v2, it is required to manualy call it during register/de-register
	 * a dp_spl_conn. In case caller forget to reset ops before dp_del_dev,
	 * dpm will print an error message and continue to delete this device
	 * from dp_dev list
	 */
	DP_LIB_LOCK(&dp_lock);
	if (conn->flag & DP_F_DEREGISTER)
		dp_del_dev(conn->dev, 0);
	else {
		struct dp_dev *dp_dev;

		dp_add_dev(conn->dev);
		dp_dev = dp_dev_lookup(conn->dev);
		if (!dp_dev) {
			pr_err("DPM: %s: why dp_dev_lookup failed for %s\n",
			       __func__, conn->dev->name);
			DP_LIB_UNLOCK(&dp_lock);
			return -1;
		}
		dp_dev->inst = inst;
		dp_dev->ep = DP_CPU_LPID;
	}
	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_spl_conn2);

int dp_spl_conn_get2(int inst, enum DP_SPL_TYPE type,
		    struct dp_spl_cfg *conns, u8 cnt)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (is_invalid_inst(inst))
		return DP_FAILURE;

	if (!cnt || !conns) {
		pr_err("DPM: Wrong parameter\n");
		return DP_FAILURE;
	}

	dp_info = get_dp_prop_info(inst);

	if (!dp_info->dp_spl_conn_get)
		return DP_FAILURE;

	return dp_info->dp_spl_conn_get(inst, type, conns, cnt);
}
EXPORT_SYMBOL(dp_spl_conn_get2);

int dp_get_io_port_info(struct dp_io_port_info *info, u32 flag)
{
	struct dp_dpdk_io_per_core *core_info;
	struct pmac_port_info *port_info;
	struct dp_subif_info *subif_info;
	int i, j, k = 0, vap;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	DP_LIB_LOCK(&dp_lock);

	port_info = get_dp_port_info(info->inst, PMAC_CPU_ID);

	dp_memset(info, 0, sizeof(*info));
	for (i = 0; i < DP_MAX_CORE; i++) {
		vap = 2 * i;
		subif_info = get_dp_port_subif(port_info, vap);
		if (!subif_info || subif_info->type != DP_DATA_PORT_DPDK)
			continue;

		core_info = &info->info.info.info[k++];
		core_info->core_id = i;
		info->info.info.num++;
		core_info->num_policy = subif_info->tx_policy_num;
		core_info->policy_base = subif_info->tx_policy_base;
		core_info->num_tx_push = subif_info->tx_pkt_credit;
		core_info->num_igp = 2;
		core_info->num_egp = 1;
		core_info->egp_id[0] = subif_info->cqm_deq_port[0];
		core_info->num_subif = 1;
		core_info->f_igp_qos[0] = 1;
		core_info->f_igp_qos[1] = 0;

		for (j = 0; j < 2; j++) {
			core_info->subif[j] = subif_info->subif + j;
			core_info->gpid[j] = subif_info->gpid + j;
			core_info->igp_id[j] = subif_info->igp_id + j;
		}
	}

	DP_LIB_UNLOCK(&dp_lock);

	return 0;
}
EXPORT_SYMBOL(dp_get_io_port_info);

int dp_set_datapath_io_port(struct dp_port_conf *conf, u32 flag)
{
	struct pmac_port_info *port_info;
	struct dp_subif_info *subif_info;
	struct inst_info *info = NULL;
	int i, dpid;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!conf || is_invalid_inst(conf->inst))
		return DP_FAILURE;

	dpid = conf->info.info.dpid;

	if (is_invalid_port(dpid))
		return DP_FAILURE;

	info = get_dp_prop_info(conf->inst);

	DP_LIB_LOCK(&dp_lock);

	port_info = get_dp_port_info(conf->inst, dpid);
	if (!port_info) {
		DP_LIB_UNLOCK(&dp_lock);
		pr_err("DPM: Wrong dpid %d\n", dpid);
		return DP_FAILURE;
	}

	if (port_info->type == conf->info.info.type) {
		DP_LIB_UNLOCK(&dp_lock);
		return 0;
	}

	for (i = 0; i < info->cap.max_num_subif_per_port; i++) {
		subif_info = &port_info->subif_info[i];
		if (subif_info->flags) {
			if (info->dp_set_io_port(conf->inst, dpid, i,
						 conf->info.info.type)) {
				DP_LIB_UNLOCK(&dp_lock);
				return DP_FAILURE;
			}
			subif_info->type = conf->info.info.type;
		}
	}

	port_info->type = conf->info.info.type;

	DP_LIB_UNLOCK(&dp_lock);

	return 0;
}
EXPORT_SYMBOL(dp_set_datapath_io_port);

int dp_get_datapath_io_port(struct dp_port_conf *conf, u32 flag)
{
	struct pmac_port_info *port_info;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!conf || is_invalid_inst(conf->inst))
		return DP_FAILURE;

	DP_LIB_LOCK(&dp_lock);

	port_info = get_dp_port_info(conf->inst, conf->info.info.dpid);
	if (!port_info) {
		DP_LIB_UNLOCK(&dp_lock);
		pr_err("DPM: Wrong dpid %d\n", conf->info.info.dpid);
		return DP_FAILURE;
	}

	conf->info.info.type = port_info->type;

	DP_LIB_UNLOCK(&dp_lock);

	return 0;
}
EXPORT_SYMBOL(dp_get_datapath_io_port);

int dp_register_event_cb2(struct dp_event *info, u32 flag)
{
	int ret;

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	if (flag & DP_F_DEREGISTER)
		ret = unregister_dp_event_notifier(info);
	else
		ret = register_dp_event_notifier(info);

	return ret;
}
EXPORT_SYMBOL(dp_register_event_cb2);

int dp_free_buffer_by_policy(struct dp_buffer_info *info, u32 flag)
{
	struct cqm_bm_free data = {0};
	int ret;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info)
		return DP_FAILURE;

	data.flag = flag;
	data.buf = (void *)info->addr;
	data.policy_base = info->policy_base;
	data.policy_num = info->policy_num;

	ret = CBM_OPS(info->inst, cqm_buffer_free_by_policy, &data);
	if (ret != CBM_OK) {
		pr_err("DPM: cqm_buffer_free_by_policy failed with %d\n", ret);
		return DP_FAILURE;
	}
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_free_buffer_by_policy);

/* support dp_dbg string instead of value only.
 * the supported format: tx:rx:dbg:dbgfs_xxx...
 * dbgfs_xxx can be enabled only via 'dp_dbg' as string
 */
static int set_dbg_flag(char *str)
{
	struct {
		char *param_list[64];
		char *p1, str[120];
	} *p;
	int i, j, num, cmd_total, dbgfs_cmds;

	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return -ENOMEM;

	dp_strlcpy(p->str, str, sizeof(p->str));
	p->p1 = p->str;
	for (num = 0; num < ARRAY_SIZE(p->param_list); num++) {
		if (!strlen(p->p1))
			break;
		p->param_list[num] = dp_strsep(&p->p1, ":");
		if (!p->p1) {
			num++;
			break;
		}
	}
	cmd_total = get_dp_dbg_flag_str_size() - 1;
	dbgfs_cmds = get_dp_dbgfs_flag_str_size() - 1;
	for (i = 0; i < num; i++) {
		for (j = 0; j < cmd_total; j++) {
			if (!strcasecmp(p->param_list[i],
					dp_dbg_flag_str[j])) {
				dp_dbg_flag |= dp_dbg_flag_list[j];
				break;
			}
		}
		if (j < cmd_total) /*cmd found, so continue*/
		       continue;
		/*cmd  not found, so check the debugfs_flag list*/
		for (j = 0; j < dbgfs_cmds; j++) {
			if (!strcasecmp(p->param_list[i],
					dp_dbgfs_flag_str[j])) {
				dp_dbgfs_flag |= dp_dbgfs_flag_list[j];
				break;
			}
		}
	}

	kfree(p);
	return 0;
}

#if IS_ENABLED(CONFIG_KALLSYMS) && IS_ENABLED(CONFIG_KGDB)
/* copy below two struct from kernel/module.c */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
struct module_sect_attr {
	struct bin_attribute battr;
	unsigned long address;
};

struct module_sect_attrs {
	struct attribute_group grp;
	unsigned int nsections;
	struct module_sect_attr attrs[];
};
#else
struct module_sect_attr {
	/* purpose change mattr to battr to share same code of dp_get_addr */
	struct module_attribute battr;
	char *name;
	unsigned long address;
};

struct module_sect_attrs {
	struct attribute_group grp;
	unsigned int nsections;
	struct module_sect_attr attrs[0];
};
#endif

static unsigned long text, bss, data;
#endif

void dp_get_addr(void)
{
#if IS_ENABLED(CONFIG_KALLSYMS) && IS_ENABLED(CONFIG_KGDB)
	int i;
	struct module *module = THIS_MODULE;

	if (!module->sect_attrs) {
		pr_err("DPM: why THIS_MODULE->sect_attrs NULL ?\n");
		return;
	}
	for (i = 0; i < module->sect_attrs->nsections; i++) {
		if (strcmp(module->sect_attrs->attrs[i].battr.attr.name,
			".text") == 0) {
			text = module->sect_attrs->attrs[i].address;
			continue;
		}
		if (strcmp(module->sect_attrs->attrs[i].battr.attr.name,
			".data") == 0) {
			data = module->sect_attrs->attrs[i].address;
			continue;
		}
		if (strcmp(module->sect_attrs->attrs[i].battr.attr.name,
			".bss") == 0) {
			bss = module->sect_attrs->attrs[i].address;
			continue;
		}
	}
#endif
}

void dp_dump_addr(struct seq_file *s)
{
#if IS_ENABLED(CONFIG_KALLSYMS) && IS_ENABLED(CONFIG_KGDB)
	dp_sprintf(s, "dp add-symbol-file:0x%lx -s .data 0x%lx -s .bss 0x%lx\n",
			   text, data, bss);
#endif
}

void dp_gdb_break(void)
{
#if IS_ENABLED(CONFIG_KALLSYMS) && IS_ENABLED(CONFIG_KGDB)
	kgdb_breakpoint();
#endif
}

int dp_pre_init(void)
{
	char *p;
	/*mask to reset some field as SWAS required  all others try to keep */
	dp_memset(dp_port_prop, 0, sizeof(dp_port_prop));
	dp_memset(dp_port_info, 0, sizeof(dp_port_info));
#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
	dp_init_fn(0);
#endif
	if (dp_dbg_flag) /* high priority with local setting */
		goto SKIP_MODULE_DP_DBG_CHECK;
	/* 2nd priority: get dp_dbg flag from kernel cmdline */
	p = dp_get_dbg_cmdline();
	if (p && strlen(p)) {
		set_dbg_flag(p);
		if (!dp_dbg_flag) /* check number if no string match */
			dp_dbg_flag = dp_atoull(p);
	}
	if (dp_dbg_flag) {
		dp_dbg = p;
		goto SKIP_MODULE_DP_DBG_CHECK;
	}
	/* 3rd priority: get dp_dbg flag from module_param. */
	if (dp_dbg && strlen(dp_dbg)) {
		set_dbg_flag(dp_dbg);
		if (!dp_dbg_flag) /* check number if no string match */
			dp_dbg_flag = dp_atoull(dp_dbg);
	}
SKIP_MODULE_DP_DBG_CHECK:
	g_dp_dev = platform_device_register_simple("dp_plat_dev", 0, NULL, 0);
	if (IS_ERR(g_dp_dev)) {
		pr_err("DPM: dp_pre_init register platform device fail\n");
		return -1;
	}
	dp_get_addr();
	if (dp_dbg_flag & DP_DBG_FLAG_GDB) {
		dp_dump_addr(NULL);
		dp_gdb_break();
	}
	log_buf = devm_kzalloc(&g_dp_dev->dev, log_buf_len, GFP_ATOMIC);
	dp_proc_install();
	dp_inst_init(0);
	dp_subif_list_init();
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	dp_switchdev_init();
#endif
	if (dp_init_pce()) {
		pr_err("DPM: %s: Datapath Init PCE Failed\n", __func__);
		goto error;
	}

	if (dp_tx_ctx_init(0)) {
		pr_err("DPM: dp_pre_init tx_ctx_init fail\n");
		goto error;
	}
	if (dp_rx_ctx_init(0)) {
		pr_err("DPM: dp_pre_init rx_ctx_init fail\n");
		goto error;
	}
	register_dp_cap(0);
	/* register_netdev_notifier will immediately receive network event notifier
	 * so there should be no dp_lib_lock during register_netdev_notifier
	 */
	register_netdev_notifier(0);
	init_qos_setting();
	DP_DUMP("DPM: dp_pre_init done: dp_dbg=%s dp_dbg_flag=0x%llx\n",
		dp_dbg, dp_dbg_flag);

#if DP_FAST_LATE_INIT
	dp_late_init();
#endif
	return 0;
error:
	platform_device_unregister(g_dp_dev);
	return -1;
}

int dp_init_module(void)
{
	int res = 0;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
	/* workaround for DPM feeds workign with build-in top level drivers */
	dp_late_register_ops();
#endif
	if (dp_init_ok) /*alredy init */
		return 0;
	if (request_dp(0)) { /*register 1st dp instance */
		pr_err("DPM: register_dp instance fail\n");
		atomic_sub(1, &dp_status);
		return -1;
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
	DP_DUMP("\n%s: Context ==> preempt_count=0x%x\n", __func__,
		preempt_count());
	DP_DUMP("   irq: %d, softirq: %d, interrupt: %d, serv_softirq: %d, "
		"nmi: %d, task: %d\n", !!in_irq(), !!in_softirq(),
		!!in_interrupt(), !!in_serving_softirq(), !!in_nmi(),
		!!in_task());
#endif
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
    LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 15)
	dp_switchdev_register_notifiers();
#endif
	dp_init_ok = 1;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
	dp_late_register_event_cb();
	dp_late_register_tx();
	dp_late_register_rx();
#endif
	return res;
}

void  dp_cleanup_module(void)
{
	int i;

	dp_mod_exiting = true;

	DP_DUMP("start cleanup dp module\n");
	unregister_netdev_notifier(0);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
    LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 15)
	dp_switchdev_unregister_notifiers();
#endif
	DP_LIB_LOCK(&dp_lock);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	dp_switchdev_exit();
#endif
	if (dp_init_ok) {  /* map to dp_init_module */

		for (i = 0; i < dp_inst_num; i++) {
			DP_CB(i, dp_platform_set)(i, DP_PLATFORM_DE_INIT);
			free_dma_chan_tbl(i);
			free_dp_port_subif_info(i);
		}
		dp_init_ok = 0;
	}
	/* dp_subif_list_init */
	dp_subif_list_free();
	dp_free_remaining_dev_list();
	/* dp_inst_init */
	dp_inst_free();

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
	/* dp_proc_install */
	debugfs_remove_recursive(dp_proc_node);
#endif
	platform_device_unregister(g_dp_dev);
	dp_init_fn(DP_PLATFORM_DE_INIT);
	DP_LIB_UNLOCK(&dp_lock);
}

/*!
 * @brief get network device's MTU
 * @param[in] dev: network device pointer
 * @param[out] mtu_size: return the maximum MTU can be supported
 *                       for this device based on current HW configuration
 * @return DP_SUCCESS on succeed and DP_FAILURE on failure
 */
int dp_get_mtu_size2(struct net_device *dev, u32 *mtu_size)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct pmac_port_info *port;
	struct dp_subif_info *sif;
#endif
	dp_subif_t *subif;
	struct cbm_mtu mtu;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s dpm not ready\n", __func__);
		return DP_FAILURE;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;

	if (unlikely(dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0))) {
		kfree(subif);
		return DP_FAILURE;
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (!CBM_OPS(subif->inst, cbm_get_mtu_size, &mtu)) {
		if (mtu_size)
			*mtu_size = mtu.mtu;
		kfree(subif);
		return DP_SUCCESS;
	} else {
		kfree(subif);
		return DP_FAILURE;
	}
#endif
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)

	port = get_dp_port_info(subif->inst, subif->port_id);
	sif = get_dp_port_subif(port, GET_VAP(subif->subif, port->vap_offset,
					      port->vap_mask));

	mtu.cbm_inst = dp_port_prop[subif->inst].cbm_inst;
	mtu.dp_port = subif->port_id;
	mtu.policy_map = CBM_OPS(subif->inst, cqm_get_policy_map, subif->inst,
				 sif->tx_policy_base, sif->tx_policy_num,
				 port->alloc_flags, TX_POLICYMAP);
	mtu.alloc_flag = port->alloc_flags;
	mtu.subif_flag = sif->flags;

	if (unlikely(CBM_OPS(subif->inst, cbm_get_mtu_size, &mtu))) {
		kfree(subif);
		return DP_FAILURE;
	}
	mtu.mtu -= ETH_HLEN;
	if (is_stream_port(port->alloc_flags))
		mtu.mtu -= sizeof(struct pmac_tx_hdr);
	if (likely(mtu_size))
		*mtu_size = mtu.mtu;
	kfree(subif);
#endif
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_get_mtu_size2);

int dp_set_mtu_size2(struct net_device *dev, u32 mtu_size)
{
	dp_subif_t *subif;
	struct inst_info *dp_info;
	struct pmac_port_info *port;
	int vap, rc;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	rc = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (unlikely(rc)) {
		kfree(subif);
		return rc;
	}
	DP_LIB_LOCK(&dp_lock);
	dp_info = get_dp_prop_info(subif->inst);
	port = get_dp_port_info(subif->inst, subif->port_id);
	vap = GET_VAP(subif->subif, port->vap_offset, port->vap_mask);
	if (unlikely(!dp_info->subif_platform_change_mtu))
		goto EXIT;
	rc = dp_info->subif_platform_change_mtu(subif->inst, subif->port_id,
						vap, mtu_size + ETH_HLEN);
EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	kfree(subif);
	return rc;
}
EXPORT_SYMBOL(dp_set_mtu_size2);

int dp_set_net_dev_ops(struct net_device *dev, void *ops_cb, int ops_offset,
		       u32 flag)
{
	int res;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_INIT_STAT, "%s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);

	res = dp_set_net_dev_ops_priv(dev, ops_cb, ops_offset, flag, "non-dpm");

	DP_LIB_UNLOCK(&dp_lock);

	return res;
}
EXPORT_SYMBOL(dp_set_net_dev_ops);

int dp_get_dc_config(struct dp_dc_res *res, int flag)
{
	struct cbm_dc_res r;

	if (!res)
		return DP_FAILURE;

	r.cqm_inst = res->inst;
	r.dp_port = res->dp_port;
	r.res_id = res->res_id;
	r.alloc_flags = get_dp_port_info(res->inst, res->dp_port)->alloc_flags;

	if (CBM_OPS(res->inst, cbm_dp_get_dc_config, &r, flag)) {
		pr_err("DPM: %s: %s failed, inst=%d, dp_port=%d, res_id=%d\n, alloc_flags=0x%x",
		       __func__, "cbm_dp_get_dc_config",
		       r.cqm_inst, r.dp_port, r.res_id, r.alloc_flags);
		return DP_FAILURE;
	}
	res->rx_res = r.rx_res;
	res->tx_res = r.tx_res;
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_get_dc_config);

int dp_get_port_prop2(int inst, int port_id,
		      struct dp_port_prop *prop)
{
	struct pmac_port_info *port_info;
	if (!dp_init_ok)
		return DP_FAILURE;

	if (!prop)
		return DP_FAILURE;
	port_info = get_dp_port_info(inst, port_id);
	prop->vap_offset = port_info->vap_offset;
	prop->vap_mask = port_info->vap_mask;
	prop->alloc_flags = port_info->alloc_flags;
	prop->owner = port_info->owner;
	prop->num_subif = port_info->num_subif;
	prop->subif_max = port_info->subif_max;
	prop->ctp_max = port_info->ctp_max;
	prop->status = port_info->status;
	prop->port_id = port_info->port_id;
	prop->lct_idx = port_info->lct_idx;
	prop->deq_port_num = port_info->deq_port_num;

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_get_port_prop2);

int dp_get_subif_prop2(int inst, int port_id, int vap,
		       struct dp_subif_prop *prop)
{
	struct pmac_port_info *port_info;
	struct dp_subif_info *subif_info;

	if (!dp_init_ok)
		return DP_FAILURE;
	if (!prop)
		return DP_FAILURE;
	port_info = get_dp_port_info(inst, port_id);
	subif_info = get_dp_port_subif(port_info, vap);
	prop->flags = subif_info->flags;
	prop->netif = subif_info->netif;
	prop->ctp_dev = subif_info->ctp_dev;
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_get_subif_prop2);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
bool dp_is_ready2(void)
#else
bool dp_is_ready(void)
#endif
{
	if (dp_init_ok > 0)
		return true;
	return false;
}
#if !IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
EXPORT_SYMBOL(dp_is_ready);
#endif

/* Note, we can use kgdb or dp_dbg memroy write tool to set dp_dbg_flag to zero
 * and jump back to caller's context
 */
void dp_die(const char *func_name, int curr_v, int ref_v)
{
	pr_err("DPM: %s curr_v=%d ref_v=%d dp_dbg_flag=0x%px\n",
	       func_name ? func_name : "NULL",
	       curr_v, ref_v, &dp_dbg_flag);
	DPM_BUG_ON(1);
}

static int __init dp_init(void)
{
	printk("========> DPM: (DATAPATH MANAGER) Module, Version: %s <========\n",
			__stringify(DP_VER_MAJ.DP_VER_MID.DP_VER_MIN.DP_VER_TAG));
	return dp_pre_init();
}

static void __exit dp_exit(void)
{
	dp_cleanup_module();
}

/* parameter dp_dbg=-1, or dp_dbg=0x10, or
 *           dp_dbg=dbg:qos:tx
 */
#if !IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
static int __init dp_dbg_lvl_set(char *str)
{
	DP_DUMP("\n\ndp_dbg=%s\n\n", str);

	/* check string first if it is not set yet */
	if (!dp_dbg_flag || !dp_dbgfs_flag)
		set_dbg_flag(dp_dbg);
	/* check number if no string match */
	if (!dp_dbg_flag)
		dp_dbg_flag = dp_atoull(dp_dbg);

	return 0;
}
/* uboot pass dp_dbg= to linux in built-in kernel */
early_param("dp_dbg", dp_dbg_lvl_set);
#endif
module_param(dp_dbg, charp, S_IRUGO);
module_init(dp_init);
module_exit(dp_exit);
MODULE_LICENSE("GPL");
MODULE_VERSION(__stringify(DP_VER_MAJ.DP_VER_MID.DP_VER_MIN.DP_VER_TAG));

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
