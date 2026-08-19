// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_MISC_H
#define DATAPATH_MISC_H

#include <linux/notifier.h>
#include <linux/netdevice.h>
#include "datapath_ppv4.h"
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include "../datapath.h"
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include <linux/pp_api.h>
#include <linux/pp_buffer_mgr_api.h>
#include <linux/pp_qos_api.h>
#include "datapath_ppv4_session.h"
#endif

#define PMAC_MAX_NUM		16
#define PAMC_LAN_MAX_NUM	7
#define VAP_DSL_OFFSET		3
#define PMAPPER_DISC_CTP	255
/* Max special connection count */
#define MAX_SPL_CONN_CNT (CTP_OTHER_SPL_CONN_NUM + MAX_PP_NF_CNT)

#define dp_is_reinsert_deq_port(inst, egp) (egp == reinsert_deq_port[inst])

#if !IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#define dp_is_spl_conn(...) 0
#endif

/* For WLAN 8 bit Mode VAP=11:8/14:8
 * For WLAN 9 bit Mode VAP=11:9/14:9
 * For Non-WLAN Mode VAP=7:0
 */
#define VAP_8BIT_OFFSET		8
#define VAP_8BIT_MASK_31	GENMASK(3, 0)
#define VAP_8BIT_MASK_32	GENMASK(6, 0)
#define VAP_4BIT_MASK_32	GENMASK(3, 0)
#define VAP_9BIT_OFFSET		9
#define VAP_9BIT_MASK_31	GENMASK(2, 0)
#define VAP_9BIT_MASK_32	GENMASK(5, 0)
#define VAP_NONWLAN_8BIT_OFFSET	0
#define VAP_NONWLAN_8BIT_MASK	GENMASK(7, 0)
#define VAP_NONWLAN_6BIT_OFFSET	0
#define VAP_NONWLAN_6BIT_MASK	GENMASK(5, 0)

#define GSWIP_O_DEV_NAME	1
#define GSWIP_L			GSWIP_O_DEV_NAME
#define GSWIP_R			GSWIP_O_DEV_NAME
#define MAX_SUBIF_PER_PORT	256
#define MAX_BP_NUM		128
#define MAX_GPID		256
#define CPU_PORT		0
#define CPU_SUBIF		0 /* CPU default subif ID*/
#define CPU_BP			0 /* CPU default bridge port ID */
#define CPU_FID			0 /* CPU default bridge ID */

enum {
	DESC_MPE1 = BIT(0),		/* Desc MPE1 bit */
	DESC_MPE2 = BIT(1),		/* Desc MPE2 bit */
	DESC_ENC = BIT(2),		/* Desc ENC bit */
	DESC_DEC = BIT(3),		/* Desc DEC bit */
	DESC_IGP = BIT(4),		/* Desc IGP bit */
	DESC_EGP = BIT(5),		/* Desc EGP bit */
	DESC_COLR = BIT(6),		/* Desc COLR bit */
	DESC_CLASSID = BIT(7),		/* Desc CLASSID bit */
	DESC_REDIR = BIT(8),		/* Desc REDIR bit */
	DESC_PMAC = BIT(9),		/* Desc Pmac bit */
	DESC_PREL2 = BIT(10),		/* Desc PreL2 bit */
	DESC_DW0_ALL = BIT(11),		/* Desc DW0_ALL bit */
	DESC_DW1_ALL = BIT(12),		/* Desc DW1_ALL bit */
	P_INS = BIT(13),		/* Pmac Insertion */
	P_IGP_MSB = BIT(14),		/* Pmac Igp Msb */
	P_CLS_EN = BIT(15),		/* Pmac Class Enable */
	P_TCP_CSUM = BIT(16),		/* Pmac TCP Checksum */
	P_IGP_EGP = BIT(17),		/* Pmac Igp Egp */
	P_PTP = BIT(18),		/* Pmac Ptp */
	DESC_PMAC_FLAG_END = BIT(19),
};

/* CQM QMAP mask of all DONTCARE flags */
#define CQM_QMAP_F_ALL_DONTCARE \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE1_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE2_DONTCARE | \
	 CBM_QUEUE_MAP_F_EP_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE | \
	 CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)

/* CQM QMAP mask of all DONT CARE flags except EP_DONTCARE */
#define CQM_QMAP_F_EP_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE1_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE2_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE | \
	 CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)

/* CQM QMAP mask of all DONT CARE flags except EG_DONTCARE */
#define CQM_QMAP_F_EG_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE1_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE2_DONTCARE | \
	 CBM_QUEUE_MAP_F_EP_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE)

/* CQM QMAP mask of all DONT CARE flags except MPE1 and MPE2 */
#define CQM_QMAP_F_MPE1_MPE2_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_EP_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE | \
	 CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)

/* CQM QMAP mask of all DONT CARE flags except EP, MPE1 and MPE2 */
#define CQM_QMAP_F_EP_MPE1_MPE2_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE | \
	 CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)

/* CQM QMAP mask of all DONT CARE flags except EP and MPE2 */
#define CQM_QMAP_F_EP_MPE2_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_SUBIF_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE1_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE | \
	 CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)

/* CQM QMAP mask of Subif EP */
#define CQM_QMAP_F_EP_SUBIF_ONLY \
	(CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE | \
	 CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE | \
	 CBM_QUEUE_MAP_F_EN_DONTCARE | \
	 CBM_QUEUE_MAP_F_DE_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE1_DONTCARE | \
	 CBM_QUEUE_MAP_F_MPE2_DONTCARE | \
	 CBM_QUEUE_MAP_F_TC_DONTCARE | \
	 CBM_QUEUE_MAP_F_COLOR_DONTCARE)

enum CQE_LOOKUP_MODE {
	CQE_LU_MODE0,		/* subif_id[13:8] + class[1:0] */
	CQE_LU_MODE1,		/* subif_id[7:0]               */
	CQE_LU_MODE2,		/* subif_id[11:8] + class[3:0] */
	CQE_LU_MODE3,		/* subif_id[4:0]  + class[1:0] */
	CQE_LU_MODE4,
	CQE_LU_MODE5,
	CQE_LU_MODE6,
	CQE_LU_MODE7,
	CQE_LU_MODE_INVALID,
};

struct dp_dev_data;

struct gsw_itf {
	u8 ep;		/* -1 means no assigned yet for dynamic case */
	u8 fixed;	/* fixed (1) or dynamically allocate (0) */
	u16 start;
	u16 end;
	u16 n;
	u8 mode;
	u8 cqe_mode;	/* CQE look up mode */
	u8 gpid_start;	/* First GPID port */
	u8 gpid_num;	/* Number of GPID port allocated */
};

struct dp_gpid_map_table {
	int alloc_flags;
	int dpid;
	int subif;
	int ref_cnt;	/* reference counter */
	unsigned long f_hif;
};

struct ctp_assign {
	GSW_LogicalPortMode_t emode; /*mapped GSWIP CTP flag */
	u16 max_ctp;
	u16 max_subif;
	u16 max_gpid;
	u32 cqe_lu_md;
	u16 swdev_en;
	u32 vap_offset;
	/* vap mask is for GSWIP*/
	u32 vap_mask;
	/* OOB is for PP only*/
	u8 oob_subif_offset;
	u8 oob_subif_size;
	u8 oob_class_size;
};

#define SET_PMAC_SUBIF(pmac, subif) do { \
	(pmac)->src_dst_subif_id_lsb = (subif) & 0xff; \
	(pmac)->src_dst_subif_id_msb =  ((subif) >> 8) & 0x1f; \
} while (0)

struct cqm_deq_stat;
struct pp_queue_stat;

struct resv_q {
	int flag;
	int id;
	int physical_id;
};

struct resv_sch {
	int flag;
	int id;
};

struct resv_info {
	int num_resv_q; /*!< input:reserve the required number of queues*/
	int num_resv_sched; /*!< input:reserve required number of schedulers*/
	int flag_ops; /*!< saved op flags of enum DP_DEV_DATA_FLAG */
	struct resv_q *resv_q; /*!< reserved queues info*/
	struct resv_sch *resv_sched; /*!< reserved schedulers info */
};

struct pp_qos_dev;
struct hal_priv {
	struct cqm_deq_stat deq_port_stat[DP_MAX_PPV4_PORT];
	struct pp_queue_stat qos_queue_stat[DP_MAX_QUEUE_NUM];
	struct pp_sch_stat qos_sch_stat[DP_MAX_NODES];
	struct resv_info resv[MAX_DP_PORTS];
	int bp_def;
	int inst;
	struct pp_qos_dev *qdev; /* ppv4 qos dev */
	s32 ppv4_drop_q;  /* drop queue: physical id */
	int cqm_drop_p; /* cqm drop/flush port id*/
	u32 ppv4_drop_p;  /* drop qos port(logical node_id):workaround for
			   * PPV4 API issue to get physical queue id
			   * before pp_qos_queue_set
			   */
	u32 ppv4_tmp_p; /* workaround for ppv4 queue allocate to
			 * to get physical queue id
			 */
	struct dp_gpid_map_table gp_dp_map[MAX_GPID];	/* Map Table GPID
							 *       <-> DPID
							 */
	struct dp_spl_cfg spl[MAX_SPL_CONN_CNT];
	spinlock_t qos_lock;
};

/*!
 *@brief get PMAC ID
 *@param[in] inst        : Datapath instance
 *@param[in] cid         : DMA Controller ID
 *@return Pmac ID
 */
static inline int dp_get_pmac_id(int inst, u8 cid)
{
	/* Below are fixed setting in HW,
	 * if DMA Controller is 1 for PRX Pmac is 0 and LGM Pmac is 1
	 * if DMA Controller is 2 for PRX Pmac is 1 and LGM Pmac is 2
	 * DMA Controller 0 is only for LGM
	 */
	if (cid == 0) {
		return PMAC_0;
	} else if (cid == 1) {
		if (is_soc_lgm(inst))
			return PMAC_1;
		else
			return PMAC_0;
	} else if (cid == 2) {
		if (is_soc_lgm(inst))
			return PMAC_2;
		else
			return PMAC_1;
	}

	return 0;
}

static inline bool is_xpon(int flags)
{
	if (flags & (DP_F_GPON | DP_F_EPON))
		return true;

	return false;
}

static inline bool is_qid_valid(int q_id)
{
	if (q_id < 0 || q_id >= DP_MAX_QUEUE_NUM) {
		pr_err("DPM: Wrong Parameter: QID[%d]Out Of Range\n", q_id);
		return false;
	}
	return true;
}

static inline bool is_sch_valid(int sch_id)
{
	if (sch_id < 0 || sch_id >= DP_MAX_NODES) {
		pr_err("DPM: Wrong Parameter: Sched[%d]Out Of Range\n", sch_id);
		return false;
	}
	return true;
}

static inline bool is_deqport_valid(int deq_port)
{
	if (deq_port < 0 || deq_port >= DP_MAX_PPV4_PORT) {
		pr_err("DPM: Wrong Parameter: Port[%d]Out Of Range\n", deq_port);
		return false;
	}
	return true;
}

static inline bool is_sch_child_free(struct hal_priv *priv, int sch_id)
{
	if (priv->qos_sch_stat[sch_id].c_flag == PP_NODE_FREE) {
		pr_err("DPM: Sch child node is Free Sch ID %d flag:0x%x\n",
		       sch_id, priv->qos_sch_stat[sch_id].c_flag);
		return true;
	}
	return false;
}

static inline bool is_sch_parent_free(struct hal_priv *priv, int sch_id)
{
	if (priv->qos_sch_stat[sch_id].p_flag == PP_NODE_FREE) {
		pr_err("DPM: Sch Parent node is Free Sch ID %d Flag :0x%x\n",
		       sch_id, priv->qos_sch_stat[sch_id].p_flag);
		return true;
	}
	return false;
}

static inline bool is_q_node_free(struct hal_priv *priv, int q_id)
{
	if (priv->qos_queue_stat[q_id].flag == PP_NODE_FREE)
		return true;
	return false;
}

static inline bool is_port_node_free(struct hal_priv *priv, int port)
{
	if (priv->deq_port_stat[port].flag == PP_NODE_FREE) {
		pr_err("DPM: Deq Port Node is Free Deq port %d flag:0x%x\n",
		       port, priv->deq_port_stat[port].flag);
		return true;
	}
	return false;
}

struct datapath_ctrl {
	struct dentry *debugfs;
	const char *name;
};

/* brief structure to set special config for PON port registration */
struct pon_spl_cfg {
	int flag;
	u8 ingress; /* ingress special tag */
	u8 egress; /* egress special tag */
	u8 crc_check; /* Rx CRC check */
	u32 fcs_gen; /* Tx FCS */
	u32 flow_ctrl; /* Rx/Tx flow control */
	u32 link_sts; /* Link state */
	/* special Mac operations */
	MAC_OPER_CFG tx_spl_tag;
	MAC_OPER_CFG rx_time_stamp;
	MAC_OPER_CFG rx_spl_tag;
	MAC_OPER_CFG rx_fcs;
	MAC_IDX pmac;
	u32 qid; /* Ingress PCE Bypass Queue setting*/
};

#define SET_PMAC_IGP_EGP(pmac, port_id) ((pmac)->igp_egp = (port_id) & 0xF)

#define SET_PMAC_SUBIF(pmac, subif) do { \
	(pmac)->src_dst_subif_id_lsb = (subif) & 0xff; \
	(pmac)->src_dst_subif_id_msb =  ((subif) >> 8) & 0x1f; \
} while (0)

struct dp_tx_common;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
int set_gswip_ext_vlan(struct core_ops *ops, struct ext_vlan_info *vlan,
		       int flag);
#endif
int qos_platform_set(int cmd_id, void *node, int flag);
int _dp_node_alloc(struct dp_node_alloc *node, int flag);
int _dp_node_free(struct dp_node_alloc *node, int flag);
int _dp_deq_port_res_get(struct dp_dequeue_res *res, int flag);
int _dp_node_link_en_get(struct dp_node_link_enable *en, int flag);
int _dp_node_link_en_set(struct dp_node_link_enable *en, int flag);
int _dp_qos_link_prio_set(struct dp_node_prio *info, int flag);
int _dp_qos_link_prio_get(struct dp_node_prio *info, int flag);
int _dp_node_link_add(struct dp_node_link *info, int flag);
int _dp_link_add(struct dp_qos_link *cfg, int flag);
int _dp_link_get(struct dp_qos_link *cfg, int flag);
int _dp_node_unlink(struct dp_node_link *info, int flag);
int _dp_node_link_get(struct dp_node_link *info, int flag);
int _dp_queue_conf_set(struct dp_queue_conf *cfg, int flag);
int _dp_queue_conf_get(struct dp_queue_conf *cfg, int flag);
int _dp_shaper_conf_set(struct dp_shaper_conf *cfg, int flag);
int _dp_shaper_conf_get(struct dp_shaper_conf *cfg, int flag);
int _dp_queue_map_get(struct dp_queue_map_get *cfg, int flag);
int _dp_queue_map_set(struct dp_queue_map_set *cfg, int flag);
int _dp_counter_mode_set(struct dp_counter_conf *cfg, int flag);
int _dp_counter_mode_get(struct dp_counter_conf *cfg, int flag);
int dp_get_queue_logic(struct dp_qos_q_logic *cfg, int flag);
int _dp_children_get(struct dp_node_child *cfg, int flag);
int dp_free_children_via_parent(struct dp_node_alloc *node, int flag);
int dp_node_reserve(int inst, int ep, struct dp_dev_data *data, int flags);
int _dp_qos_max_level_get(struct dp_qos_level *dp, int flag);
int _dp_qos_global_info_get(struct dp_qos_cfg_info *info, int flag);
int _dp_qos_port_conf_set(struct dp_port_cfg_info *info, int flag);
int _dp_rx(struct sk_buff *skb, u32 flags);
int _dp_tx(struct sk_buff *skb, struct dp_tx_common *cmn);

int dp_lan_wan_bridging(int port_id, struct sk_buff *skb);
static inline char *parser_flag_str(u8 f)
{
	if (f == DP_PARSER_F_DISABLE)
		return "No Parser";
	else if (f == DP_PARSER_F_HDR_ENABLE)
		return "Parser Flag only";
	else if (f == DP_PARSER_F_HDR_OFFSETS_ENABLE)
		return "Parser Full";
	else
		return "Reserved";
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
int dp_sub_proc_install(void);
int proc_print_ctp_bp_info(struct seq_file *s, int inst,
			   struct pmac_port_info *port,
			   int subif_index, u32 flag);
#else
static inline int dp_sub_proc_install(void) { return 0; }
static inline int proc_print_ctp_bp_info(struct seq_file *s, int inst,
					 struct pmac_port_info *port,
					 int subif_index, u32 flag)
{
	return 0;
}
#endif

ssize_t proc_get_qid_via_index(struct file *file, const char *buf,
			       size_t count, loff_t *ppos);
int datapath_debugfs_init(struct datapath_ctrl *pctrl);
int tc_vlan_set(struct core_ops *ops, struct dp_tc_vlan *vlan,
		   struct dp_tc_vlan_info *info,
		   int flag);

int dp_queue_block_flush(struct dp_qos_blk_flush_queue *cfg, int flag);
int dp_port_block_flush(struct dp_qos_blk_flush_port *cfg, int flag);
int dp_get_queue_mib(struct dp_qos_queue_info *info, int flag);
int dp_get_lookup_qid_via_index(int inst, struct cbm_lookup *info);

/* Bridge port */
int dp_gsw_alloc_bp(int inst, int ep, int subif, int fid, int bp_member, int flags);
int dp_gsw_free_bp(int inst, int bp);
int dp_gsw_set_bp_attr(struct dp_bp_attr *conf, int bport, uint32_t flag);
int dp_cfg_domain_for_bp_member(int inst, int bp);

/* CTP port */
const struct ctp_assign *dp_gsw_get_ctp_assign(int inst, int flags);
struct gsw_itf *dp_gsw_assign_ctp(int inst, u8 ep, int bp_default, u32 flags,
				  struct dp_dev_data *data);

int gpid_port_assign(int inst, u8 ep, struct dp_dev_data *data, u32 flags);

int dp_gsw_set_pmac(int inst, u32 port, dp_pmac_cfg_t *pmac_cfg);
int dp_gsw_set_parser(u8 flag, u8 cpu, u8 mpe1, u8 mpe2, u8 mpe3);
int dp_gsw_get_parser(u8 *cpu, u8 *mpe1, u8 *mpe2, u8 *mpe3);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
int dp_gsw_add_mac_entry(int bport, int fid, int inst, u8 *addr);
int dp_gsw_del_mac_entry(int bport, int fid, int inst, u8 *addr);
#endif

int dp_gsw_set_pmapper(int inst, int bport, int ep, struct dp_pmapper *mapper, u32 flag);
int dp_gsw_get_pmapper(int inst, int bport, int ep, struct dp_pmapper *mapper, u32 flag);

int dp_gsw_alloc_meter(int inst, int *meterid, int flag);
int dp_gsw_add_meter(struct net_device *dev, struct dp_meter_cfg *meter,
		     int flag, struct dp_meter_subif *mtr_subif);
int dp_gsw_del_meter(struct net_device *dev, struct dp_meter_cfg *meter,
		     int flag, struct dp_meter_subif *mtr_subif);

int dp_get_port_vap_mib(dp_subif_t *subif_id, void *priv,
			struct rtnl_link_stats64 *stats,
			u32 flags);
int dp_gsw_reset_mib(int dev, u32 flag);
int dp_gsw_set_port_lu_md(int inst, u8 ep, u32 flags);
int dp_gsw_dis_cpu_vlan_md(int inst);
int dp_get_gswip_cap(struct dp_hw_cap *cap, int flag);
int dp_gsw_color_table_set(int inst);

ssize_t proc_mib_count_write(struct file *file, const char *buf,
			     size_t count, loff_t *ppos);
int proc_mib_count_dump(struct seq_file *s, int pos);
int proc_mib_count_init(void *param);

int dp_codel_cfg_get(struct dp_qos_codel_cfg *cfg, int flag);
int dp_codel_cfg_set(struct dp_qos_codel_cfg *cfg, int flag);

inline static void dp_rx_handle_fcs(struct pmac_port_info *port_info, u32 subif,
				    struct sk_buff *skb)
{
	/* The OMCI packets are not ethernet ones and thus they
	 * don't have FCS when pon fw uses FCS check only mode.
	 * The packets still have MIC at the end. The MIC is
	 * required by the higher layers so don't remove here.
	 */
	if (is_xpon(port_info->alloc_flags) && !subif)
		return;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (port_info->data_flag_ops & DP_F_REM_FCS)
#else
	if (((struct dma_rx_desc_1 *)&skb->DW1)->field.fcs)
#endif
		__pskb_trim(skb, skb->len - DP_FCS_LEN);
}

inline static void dp_rx_handle_timetag(
	int inst,
	struct pmac_rx_hdr *pmac,
	struct pmac_port_info *dp_port,
	struct sk_buff *skb)
{
	struct mac_ops *ops = dp_port_prop[inst].mac_ops[dp_port->port_id];

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (pmac && !pmac->oam) {
#else
	if (((struct dma_rx_desc_1 *)&skb->DW1)->field.ts) {
#endif
		/* as aligned, oam and ptp will not exist in same packet.
		 * Note: ops->do_rx_hwts will remove timetag
		 */
		if (dp_port->f_ptp && ops) {
			ops->do_rx_hwts(ops, skb);
			return;
		}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
		/* Assume only PON RX traffic always have timestamp in PRX */
		if (!is_xpon(dp_port->alloc_flags))
			return;
#endif

		__pskb_trim(skb, skb->len - DP_TS_HDRLEN);
	}
}

#endif
