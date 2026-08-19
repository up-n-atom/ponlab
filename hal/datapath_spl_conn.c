// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2023, MaxLinear, Inc.
 * Copyright 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include <linux/device.h>
#include <net/datapath_api.h>
#include <net/datapath_api_umt.h>
#if IS_ENABLED(CONFIG_SOC_LGM)
	#include <net/mxl_cbm_api.h>
#else
	#include <net/intel_cbm_api.h>
#endif
#include "../datapath.h"
#include "datapath_misc.h"
#include "datapath_ppv4.h"
#include "datapath_ppv4_session.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

struct spl_conn_name {
	enum DP_SPL_TYPE type;
	char *name;
};
static const struct spl_conn_name spl_conn_name[] = {
	{DP_SPL_TOE, "TOE"},
	{DP_SPL_VOICE, "VOICE"},
	{DP_SPL_VPNA, "VPN_ADAPTER"},
	{DP_SPL_APP_LITEPATH, "APP_LITEPATH"},
	{DP_SPL_PP_NF, "PP_NF"},
	{DP_SPL_PP_DUT, "PP_DUT"}
};

char *get_spl_name(enum DP_SPL_TYPE type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(spl_conn_name); i++) {
		if (spl_conn_name[i].type == type)
			return spl_conn_name[i].name;
	}
	return "unknown";
}

static void update_deq_dma_ch(int inst, int egp_id, int ch)
{
	struct cqm_port_info *deq;

	if (egp_id >= DP_MAX_PPV4_PORT) {
		pr_err("DPM: %s: wrong egp_id=%d\n", __func__, egp_id);
		return ;
	}
	if (inst >= DP_MAX_INST) {
		pr_err("DPM: %s: wrong inst=%d\n", __func__, inst);
		return ;
	}
	deq = get_dp_deqport_info(inst, egp_id);
	deq->dma_chan = ch;
	deq->f_dma_ch = 1;
	deq->dts_qos = dp_get_inter_qos_cfg(inst, egp_id);
}

static int egp_pre_update(int inst, const struct dp_spl_cfg *conn,
			  const struct dp_spl_egp *egp)
{
	struct cqm_port_info *deq = get_dp_deqport_info(inst, egp->egp_id);

	/* Perform update if ref_cnt is 0 */
	if (!deq->ref_cnt) {
		deq->tx_pkt_credit = egp->tx_pkt_credit;
		deq->txpush_addr = egp->tx_push_paddr;
		deq->txpush_addr_qos = egp->tx_push_paddr_qos;
		deq->tx_ring_size = egp->pp_ring_size;
		deq->dp_port[conn->dp_port] = 1;
		/* First blindly set to non-intemediate since no proper way to set yet.
		 * Later will tune in add_igp API
		 */
		deq->dts_qos = dp_get_qos_cfg(inst, CPU_PORT, DP_F_CPU, conn->type);
	}

	return deq->ref_cnt;
}

static void egp_post_update(int inst, struct dp_spl_egp *egp,
			    const struct ppv4_q_sch_port *pp)
{
	struct cqm_port_info *deq = get_dp_deqport_info(inst, egp->egp_id);

	egp->qid = pp->qid;
	deq->first_qid = pp->qid;
	deq->q_node = pp->q_node;
	DP_DEBUG(DP_DBG_FLAG_SPL, "EGP:%d QID:%d QNODE:%d\n",
		 egp->egp_id, egp->qid, deq->q_node);
}

static int get_dep_q_node(int inst, const struct dp_spl_egp *egp)
{
	struct cqm_port_info *deq = get_dp_deqport_info(inst, egp->egp_id);

	return deq->q_node;
}

static int release_egp_port(int inst, struct dp_spl_cfg *conn,
			    struct dp_spl_egp *egp)
{
	struct cqm_port_info *deq = get_dp_deqport_info(inst, egp->egp_id);
	struct dp_node_alloc alloc = {
		.inst = inst,
		.dp_port = conn->dp_port,
		.type = DP_NODE_QUEUE,
		.id.q_id = egp->qid,
	};

	deq->ref_cnt--;
	DP_DEBUG(DP_DBG_FLAG_SPL, "%s ref_cnt left=%u for deq=%d qid=%d\n",
		 __func__, deq->ref_cnt, egp->egp_id, alloc.id.q_id);
	if (deq->ref_cnt)
		return DP_SUCCESS;

	return dp_node_free(&alloc, 0);
}

static int setup_egp_port(int inst, struct dp_spl_cfg *conn,
				 struct dp_spl_egp *egp)
{
	struct ppv4_q_sch_port pp = {
		.inst    = inst,
		.dp_port = conn->dp_port,
		.ctp     = conn->spl_id,
		.cqe_deq = egp->egp_id,
	};
	struct cqm_port_info *deq = get_dp_deqport_info(inst, egp->egp_id);

	if (!egp_pre_update(inst, conn, egp)) {
		/* first time for this deq port */
		if (alloc_q_to_port(&pp, conn->flag) == DP_FAILURE)
			return DP_FAILURE;
		egp_post_update(inst, egp, &pp);
	} else {
		egp->qid = deq->qid[0];
		DP_DEBUG(DP_DBG_FLAG_SPL,
			 "id:%d egp:%d already allocated/shared",
			 conn->spl_id, egp->egp_id);
	}
	deq->ref_cnt++;

	return DP_SUCCESS;
}

static int remove_egp(int inst, struct dp_spl_cfg *conn)
{
	int ret = DP_SUCCESS;
	int i;

	for (i = 0; i < conn->num_egp; i++) {
		if (release_egp_port(inst, conn, &conn->egp[i]))
			ret = DP_FAILURE;
	}
	return ret;
}

static int add_egp(int inst, struct dp_spl_cfg *conn)
{
	int i;

	for (i = 0; i < conn->num_egp; i++) {
		int ret = setup_egp_port(inst, conn, &conn->egp[i]);

		if (ret) {
			pr_err("DPM: failed to setup egp%d(%d)\n",
			       conn->egp[i].egp_id, i);
			return ret;
		}
	}
	return DP_SUCCESS;
}

static int add_igp(int inst, struct dp_spl_cfg *conn)
{
	int i;
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;

	port_info = get_dp_port_info(inst, conn->dp_port);
	sif = get_dp_port_subif(port_info, conn->spl_id);
	for (i = 0; i < conn->num_igp; i++) {
		struct dp_spl_igp *igp = &conn->igp[i];

		if (!igp->egp)
			continue;
		if (igp->num_out_tx_dma_ch) {
			update_deq_dma_ch(inst, igp->egp->egp_id,
					  igp->igp_dma_ch_to_gswip);
		}
		if (conn->type == DP_SPL_TOE) {
			if (i == 0)
				dflt_q_toe[inst] = igp->egp[i].qid;
		}
	}
	return DP_SUCCESS;
}

/* Note: so far only support one UMT for spl connection */
static int add_umt(int inst, struct dp_spl_cfg *conn,
		   const struct pmac_port_info *port)
{
	struct umt_ops *ops = dp_get_umt_ops(inst);
	struct umt_port_res *res = &conn->umt->res;
	struct cqm_port_info *deq;
	int ret = DP_SUCCESS;

	res->rx_src = UMT_RX_SRC_CQEM;
	res->cqm_enq_pid = conn->igp[0].igp_id;
	res->cqm_dq_pid = conn->egp[0].egp_id;
	if (conn->num_umt_port > 1) {
		pr_err("DPM: so far not support more than 2 umt yet: %d\n",
		       conn->num_umt_port);
		return DP_FAILURE;
	}
	deq = get_dp_deqport_info(inst, conn->egp[0].egp_id);

	if (!ops) {
		pr_err("DPM: No UMT driver registered\n");
		return -ENODEV;
	}
	if (!deq->umt_info[0].ref_cnt_umt) {
		if (ops->umt_alloc(ops->umt_dev, conn->umt) < 0) {
			pr_err("DPM: Failed to allocate umt for spl type=%s\n",
			       get_spl_name(conn->type));
			return -ENODEV;
		}
		ret = ops->umt_request(ops->umt_dev, conn->umt);
		if (ret) {
			pr_err("DPM: Failed to umt_request for spl_type: %s\n",
			       get_spl_name(conn->type));
			ops->umt_release(ops->umt_dev, conn->umt->ctl.id);
			return DP_FAILURE;
		}
		/* save umt info */
		deq->umt_info[0].umt = conn->umt[0];
	} else {
		/* get saved umt info */
		conn->umt[0] = deq->umt_info[0].umt;
	}
	deq->umt_info[0].ref_cnt_umt++;

	return DP_SUCCESS;
}

static int remove_umt(int inst, const struct dp_spl_cfg *conn)
{
	struct umt_ops *ops = dp_get_umt_ops(inst);
	struct cqm_port_info *deq;

	if (!ops)
		return -ENODEV;
	deq = get_dp_deqport_info(inst, conn->egp[0].egp_id);
	deq->umt_info[0].ref_cnt_umt--;

	if (deq->umt_info[0].ref_cnt_umt)
		return DP_SUCCESS;

	/* Disable UMT port */
	if (ops->umt_enable(ops->umt_dev, conn->umt->ctl.id, 0))
		return DP_FAILURE;

	return ops->umt_release(ops->umt_dev, conn->umt->ctl.id);
}

static int sync_sif(int inst, struct dp_spl_cfg *conn,
			const struct pmac_port_info *port,
			struct dp_subif_info *sif)
{
	dp_subif_t *subif = NULL;
	const struct ctp_assign *ca;
	int ret = DP_FAILURE;

	if (!conn->dev) {
		DP_DEBUG(DP_DBG_FLAG_SPL, "%s dev NULL for spl_conn type=%d\n",
			 __func__, conn->type);
		return DP_SUCCESS;
	}
	if (conn->flag & DP_F_DEREGISTER) {
		if (dp_del_subif(conn->dev, NULL, NULL, NULL, 0)) {
			pr_err("DPM: dp_del_subif fail for %s\n", conn->dev->name);
			return DP_FAILURE;
		}
		return DP_SUCCESS;
	}
	ca = dp_gsw_get_ctp_assign(inst, DP_F_CPU);
	if (!ca)
		return DP_FAILURE;
	subif = dp_kzalloc(sizeof(struct dp_subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	subif->subif_common = sif->subif_common;
	subif->subif = sif->subif;
	subif->gpid = sif->gpid;
	dp_memcpy(subif->dfl_eg_sess_ext[0], sif->dfl_sess,
		  sizeof(sif->dfl_sess));

	dp_memcpy(&subif->subif_port_cmn, &port->subif_port_cmn,
		  sizeof(struct dp_subif_port_common));
	subif->inst = inst;
	subif->subif_num = 1;
	if (dp_update_subif(conn->dev, NULL, subif, conn->dev->name, 0, NULL)) {
		pr_err("DPM: dp_update_subif failed: %s\n", conn->dev->name);
		goto exit;
	}
	ret = DP_SUCCESS;
exit:
	kfree(subif);
	return ret;
}

/* note: just found there is no remove_igp to remove the queue yet
	 later need to add
 */
static int remove_spl_conn(int inst, struct dp_spl_cfg *conn,
			   const struct pmac_port_info *port)
{
	int ret = DP_SUCCESS;
	struct dp_subif_info *sif = get_dp_port_subif(port, conn->spl_id);

	if (sif->flags & PORT_FREE) {
		pr_err("DPM: dpm:Failed to remove spl_conn(%d): not allocted yet\n",
		       conn->spl_id);
		return DP_FAILURE;
	}

	if (sif->f_spl_gpid && dp_del_pp_gpid(inst, CPU_PORT, -1)) {
		pr_err("DPM: failed to delete SPL GPID:%d\n", conn->spl_gpid);
		ret = DP_FAILURE;
	}

	if (conn->f_gpid && dp_del_pp_gpid(inst, CPU_PORT, conn->spl_id)) {
		pr_err("DPM: failed to delete GPID:%d\n", conn->gpid);
		ret = DP_FAILURE;
	}

	if (conn->f_hostif && dp_del_hostif(inst, CPU_PORT, conn->spl_id)) {
		pr_err("DPM: failed to delete hostif for GPID:%d\n", conn->gpid);
		return DP_FAILURE;
	}

	if (dp_del_default_egress_sess(sif, 0)) {
		pr_err("DPM: Failed to delete def egress session\n");
		return DP_FAILURE;
	}

	/* remove exported subif */
	if (conn->dev)
		sync_sif(inst, conn, port, sif);

	/* remove internal subif */
	sif->flags = PORT_FREE;

	/* remove UMT */
	if (conn->num_umt_port) {
		if (remove_umt(inst, conn)) {
			pr_err("DPM: failed to release umt for spl type:%d\n",
			       conn->type);
		}
	}
	_dp_reset_subif_q_lookup_tbl(sif, 0, -1);
	/* later need to add remove_igp */

	/* remove PPv4 */
	if (remove_egp(inst, conn)) {
		pr_err("DPM: failed to release egp for spl type:%d\n", conn->type);
		ret = DP_FAILURE;
	}
	/* remove CQM */
	if (CBM_OPS(inst, cbm_dp_spl_conn, inst, conn))
		ret = DP_FAILURE;

	dp_memset(conn, 0, sizeof(struct dp_spl_cfg));
	return ret;
}

static int spl_get_free_idx(int inst, struct dp_spl_cfg *conn)
{
	struct hal_priv *hal = HAL(inst);
	int cnt = 0;
	int i, free_idx = -1;

	/* found out all matched number of type and get the first free entry */
	for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
		struct dp_spl_cfg *cfg = &hal->spl[i];

		if (!cfg->spl_id ) { /* free one */
			if (free_idx < 0) /* save first free entry */
				free_idx = i;
			continue;
		} else if (cfg->type != conn->type) {/* not match type */
			continue;
		}
		cnt++;
	}
	if (conn->type == DP_SPL_PP_NF) {
		if (cnt == MAX_PP_NF_CNT) {
			pr_err("DPM: reach maximum PPNF port count %d\n",
			       MAX_PP_NF_CNT);
			return MAX_SPL_CONN_CNT;
		}
	} else {
		if (cnt) {
			pr_err("DPM: %s CPU special path created already\n",
			       get_spl_name(conn->type));
			return MAX_SPL_CONN_CNT;
		}
	}
	DP_DEBUG(DP_DBG_FLAG_SPL, "%s:cnt=%d free_idx=%d spl_id=%d\n",
		 get_spl_name(conn->type),
		 cnt, free_idx, free_idx + CPU_CTP_NUM);

	if ((free_idx >= 0) && (free_idx < MAX_SPL_CONN_CNT))
		return free_idx;
	pr_err("DPM: No enough room to create CPU special path for %s\n",
	       get_spl_name(conn->type));
	return MAX_SPL_CONN_CNT;
}

static int add_spl_conn(int inst, struct dp_spl_cfg *conn,
			      const struct pmac_port_info *port)
{
	struct dp_subif_info *sif;
	struct cqm_deq_stat *m = NULL;
	int idx, i;

	if (is_invalid_inst(inst))
		return DP_FAILURE;
	if (conn->type >= DP_SPL_MAX) {
		pr_err("DPM: invalid spl conn type: %d\n", conn->type);
		return DP_FAILURE;
	}

	if (conn->dp_cb && conn->dp_cb->rx_fn && !conn->dev) {
		pr_err("DPM: cannot attach rx_fn without dev for %s\n",
		       get_spl_name(conn->type));
		return DP_FAILURE;
	}
	if ((conn->type == DP_SPL_TOE) && (conn->f_subif || conn->f_gpid)) {
		pr_err("DPM: invalid flag for %s\n", get_spl_name(conn->type));
		return DP_FAILURE;
	}
	if (!conn->f_subif && conn->f_gpid) {
		pr_err("DPM: dpm:f_subif must be set when f_gpid is set\n");
		return DP_FAILURE;
	}

	/* For CQM driver purpose to set DP_F_CPU */
	conn->flag |= DP_F_CPU;
	idx = spl_get_free_idx(inst, conn);
	if (idx == MAX_SPL_CONN_CNT)
		return DP_FAILURE;
	/* set spl_conn subif_grp */
	conn->spl_id = CPU_CTP_NUM + idx;
	sif = get_dp_port_subif(port, conn->spl_id);
	if (sif->flags != PORT_FREE) {
		pr_err("DPM: %s allocated already\n", get_spl_name(conn->type));
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SPL, "spl_conn type=%s(%d) idx=%d\n",
		 get_spl_name(conn->type),
		 conn->type, conn->spl_id);
	conn->dp_port = port->port_id;
	sif->port_info = port;
	BUG_ON(port->port_id != CPU_PORT);
	conn->gpid = port->gpid_base + conn->spl_id;
	conn->subif = SET_VAP(conn->spl_id, port->vap_offset, port->vap_mask);
	if (CBM_OPS(inst, cbm_dp_spl_conn, inst, conn))
		return DP_FAILURE;
	if (add_egp(inst, conn))
		return DP_FAILURE;
	if (add_igp(inst, conn))
		return DP_FAILURE;
	if (conn->num_umt_port) {
		if (add_umt(inst, conn, port)) {
			pr_err("DPM: failed to setup umt for %s\n",
			       get_spl_name(conn->type));
			return DP_FAILURE;
		}
	}

	sif = get_dp_port_subif(port, conn->spl_id);
	sif->subif_flag = conn->flag;
	sif->netif = conn->dev;
	sif->subif = conn->subif;
	sif->subif_groupid = conn->spl_id;
	sif->gpid = conn->gpid;
	sif->netif = conn->dev;
	sif->tx_policy_base = conn->policy[0].tx_policy_base;
	sif->tx_policy_num = conn->policy[0].tx_policy_num;
	sif->rx_policy_base = conn->policy[0].rx_policy_base;
	sif->rx_policy_num = conn->policy[0].rx_policy_num;
	sif->num_qid = 0;
	for (i = 0; i < conn->num_egp; i++) {
		/* here return all queue whether
		 * conn->egp[i].type == DP_EGP_TO_DEV or not
		 * For PP APP lite, in fact, no queue to device at all
		 */
		sif->qid_list[i] = conn->egp[i].qid;
		sif->q_node[i] = get_dep_q_node(inst, conn->egp);
		sif->num_qid++;
		m = &HAL(inst)->deq_port_stat[conn->egp[i].egp_id];
		sif->qos_deq_port[i] = m->node_id;
	}
	sif->num_igp = conn->num_igp;
	sif->num_egp = conn->num_egp;
	sif->prel2_len = conn->prel2_len;
	sif->spl_conn_type = conn->type;
	if (conn->dp_cb) {
		atomic_set(&sif->rx_flag, 1);
		sif->rx_fn = conn->dp_cb->rx_fn;
	}
	INIT_LIST_HEAD(&sif->logic_dev);

	if (conn->f_gpid) {
		if (dp_add_pp_gpid(inst, CPU_PORT, conn->spl_id, conn->gpid, 0, 0))
			return DP_FAILURE;
		if (conn->type == DP_SPL_VOICE) {
			BUG_ON(!port->gpid_spl);
			if (dp_add_pp_gpid(inst, CPU_PORT, 0, port->gpid_spl, 1, 0))
				return DP_FAILURE;
			conn->spl_gpid = port->gpid_spl;
			sif->f_spl_gpid = 1;
		}
	}

	if (conn->dev) {
		dp_memcpy(sif->device_name, conn->dev->name, IFNAMSIZ);
		if (conn->f_gpid && conn->type == DP_SPL_VOICE)
			dp_voice_dflt_egress_session(inst, CPU_PORT, conn->spl_id, 0);

		if (sync_sif(inst, conn, port, sif)) {
			pr_err("DPM: failed to sync subif for %s\n",
			       get_spl_name(conn->type));
			return DP_FAILURE;
		}
	} else {
		dp_strlcpy(sif->device_name, get_spl_name(conn->type),
			   sizeof(sif->device_name));
	}

	if (conn->f_hostif)
		dp_add_hostif(inst, CPU_PORT, conn->spl_id);
	if (conn->num_igp) {
		/* at present, DPM hardcoded here before DRVLIB_SW-4341 done
		 * once DRVLIB_SW-4341 is done, DPM should check out_qos_mode
		 */
		if (conn->type == DP_SPL_VPNA) {
			_dp_init_subif_q_map_rules(sif, 1);
			_dp_set_subif_q_lookup_tbl(sif, 0,
						   conn->igp->egp[0].qid, -1);
		}
	}

	/*default tc value for toe*/
	sif->toe_tc = TOE_TC_DEF_VALUE;
	sif->flags = PORT_SUBIF_REGISTERED;

	DP_DEBUG(DP_DBG_FLAG_SPL, "%s id:%d gpid:%d %s",
		 sif->device_name, conn->spl_id, sif->gpid,
		 conn->dev ? conn->dev->name : "");
	return DP_SUCCESS;
}

static void dump_spl_parameters(int inst, struct dp_spl_cfg *conn,
				   bool insert_line, char *s)
{
	int i;
	int size = 300;
	int idx;
	char *buf = dp_kmalloc(size, GFP_ATOMIC);

	if (!buf)
		return;

	DP_DUMP("DPM: %s%s\n", insert_line ? "\n" : "", s);
	DP_DUMP("   type:%s, spl_id:%d, f_subif:%d, f_gpid:%d, f_policy:%d, "
		"f_hostif:%d, dp_cb:%s, dev:%s\n",
		get_spl_name(conn->type), conn->spl_id,
		conn->f_subif, conn->f_gpid, conn->f_policy, conn->f_hostif,
		conn->dp_cb ? "set" : "NULL",
		conn->dev ? conn->dev->name : NULL);
	if (conn->flag) {
		idx = scnprintf(buf, size, "   flags(%x): ", conn->flag);
		for (i = 0; i < get_dp_port_type_str_size(); i++) {
			if (conn->flag & dp_port_flag[i])
				idx += scnprintf(buf + idx, size - idx - 1,
						"%s ", dp_port_type_str[i]);
		}
		idx += scnprintf(buf + idx, size - idx - 1, "\n");
		DP_DUMP("%s", buf);
	}
	for (i = 0; i < conn->num_igp; i++) {
		struct dp_spl_igp *igp = conn->igp + i;

		DP_DUMP("   [%02d]: IGP%d --> igp_ring_size:%d out_msg_mode:%d out_qos_mode:%d egp:%d\n",
			i, igp->igp_id, igp->igp_ring_size, igp->out_msg_mode,
			igp->out_qos_mode, igp->egp ? igp->egp->egp_id : -1);
	}
	for (i = 0; i < conn->num_egp; i++) {
		struct dp_spl_egp *egp = conn->egp + i;

		DP_DUMP("   [%02d]: EGP%d --> pp_ring_size:%d tx_push_paddr:0x%px tx_push_paddr_qos:0x%px\n",
			i, egp->egp_id, egp->pp_ring_size, egp->tx_push_paddr,
			egp->tx_push_paddr_qos);
	}

	if (conn->num_umt_port) {
		struct umt_port_ctl *c = &conn->umt->ctl;

		DP_DUMP("  %s:%d %s:%d %s:%d %s:%d %s:%d %s:%d %s:%d %s:%ld\n",
			"UMT id", c->id,
			"msg_interval", c->msg_interval,
			"msg_mode", c->msg_mode,
			"cnt_mode", c->cnt_mode,
			"sw_msg", c->sw_msg,
			"rx_msg_mode", c->rx_msg_mode,
			"enable", c->enable,
			"fflag", c->fflag);
		DP_DUMP("  dst_addr_cnt: %u\n", c->dst_addr_cnt);
		if (unlikely(c->dst_addr_cnt >= UMT_DST_ADDR_MAX)) {
			pr_err("  DPM: %s, "
			       "umt dst_addr_cnt(%u) >= UMT_DST_ADDR_MAX(%d)",
			       __func__, c->dst_addr_cnt, UMT_DST_ADDR_MAX);
		} else {
			if (c->dst_addr_cnt) {
				int i;
				DP_DUMP("  dst[]: ");
				for (i = 0; i < c->dst_addr_cnt; i++)
					DP_DUMP("%pad ", &c->dst[i]);
				DP_DUMP("\n");
			} else {
				DP_DUMP("  daddr: %pad\n", &c->daddr);
			}
		}
	}

	if (conn->f_policy)
		DP_DUMP("policy rx_pkt_size:%d\n", conn->policy->rx_pkt_size);
	kfree(buf);
}
#define EGP_SHIFT(x, offset) (struct dp_spl_egp *)((unsigned long) (x) + offset)
int _dp_spl_conn(int inst, struct dp_spl_cfg *conn)
{
	struct hal_priv *hal = HAL(inst);
	struct pmac_port_info *port;
	int ret = DP_FAILURE;

	if (!hal) {
		pr_err("DPM: DP HAL not initialized yet\n");
		return DP_FAILURE;
	}

	if (!dp_init_ok) {
		pr_err("DPM: DP not initialized yet\n");
		return DP_FAILURE;
	}
	port = get_dp_port_info(inst, CPU_PORT);
	if (!port) {
		pr_err("DPM: %s why port NULL\n", __func__);
		return DP_FAILURE;
	}
	if (IS_ENABLED(CONFIG_DPM_DATAPATH_DBG) &&
	    unlikely(dp_dbg_flag & DP_DBG_FLAG_SPL))
		dump_spl_parameters(inst, conn, 0,
			(conn->flag & DP_F_DEREGISTER) ? "de-register caller" :
			"register caller");
	DP_LIB_LOCK(&dp_lock);
	if ((conn->flag & DP_F_DEREGISTER)) { /* de_register spl_conn */
		int i;

		DP_DEBUG(DP_DBG_FLAG_SPL,
			 "de_register_spl: type=%s(%d) spid=%d %s\n",
			 get_spl_name(conn->type), conn->type,
			 conn->spl_id,
			 conn->dev ? conn->dev->name: "");

		for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
			struct dp_spl_cfg *cfg = &hal->spl[i];
			struct dp_subif_info *sif;

			if (cfg->spl_id != conn->spl_id)
				continue;
			cfg->flag = conn->flag;
			if (IS_ENABLED(CONFIG_DPM_DATAPATH_DBG) &&
			    unlikely(dp_dbg_flag & DP_DBG_FLAG_SPL))
				dump_spl_parameters(inst, conn, 0, "saved-conn");
			ret = remove_spl_conn(inst, cfg, port);
			sif = get_dp_port_subif(port, conn->spl_id);
			dp_del_default_egress_sess(sif, 0);
			sif->spl_cfg = NULL;
			break;
		}
		if (i == MAX_SPL_CONN_CNT )
			pr_err("DPM: No matching spl_id=%d %s\n",
			       conn->spl_id,
			       conn->dev ? conn->dev->name : "");
		else {
			ret = DP_SUCCESS;
			DP_DEBUG(DP_DBG_FLAG_SPL, "de-register-spl done\n\n");
		}

		goto exit;
	}

	/* register spl_conn */
	ret = add_spl_conn(inst, conn, port);
	if (ret == DP_SUCCESS) {
		struct dp_spl_cfg *tmp_cfg;
		struct dp_subif_info *sif;
		int i;
		unsigned long offset;

		tmp_cfg = &hal->spl[conn->spl_id - CPU_CTP_NUM];
		sif = get_dp_port_subif(port, conn->spl_id);
		dp_memcpy(tmp_cfg, conn, sizeof(struct dp_spl_cfg));
		/* update internal igp->egp pointers correctly */
		for (i = 0; i < conn->num_igp; i++) {
			if (conn->igp[i].egp) {
				offset = (unsigned long) conn->igp[i].egp - (unsigned long) conn->egp;
				tmp_cfg->igp[i].egp = EGP_SHIFT(tmp_cfg->egp,
								offset);
			} else {
				pr_err("DPM: %s: conn->igp[%d].egp NULL\n", __func__, i);
			}
		}
		/* Keep a reference of spl_cfg from hal_priv in subif */
		sif->spl_cfg = tmp_cfg;
#ifdef CONFIG_RFS_ACCEL
		if (conn->dev && port->rx_cpu_rmap) {
			conn->dev->rx_cpu_rmap = port->rx_cpu_rmap;
			if (unlikely(!conn->dev->rx_cpu_rmap))
				DP_DEBUG(DP_DBG_FLAG_SPL, "[%s]:rx_cpu_rmap NULL for [%s]\n", __func__ , conn->dev->name);
		}
#endif /* CONFIG_RFS_ACCEL */

		if (IS_ENABLED(CONFIG_DPM_DATAPATH_DBG) &&
		    unlikely(dp_dbg_flag & DP_DBG_FLAG_SPL)) {
			DP_DEBUG(DP_DBG_FLAG_SPL,
				 "register_spl: type=%s(%d) spid=%d %s\n",
				 get_spl_name(conn->type), conn->type,
				 conn->spl_id,
				 conn->dev ? conn->dev->name: "");
			dump_spl_parameters(inst, conn, 0, "spl_conn created");
		}
	} else {
		pr_err("DPM: %s failed: register_spl: spid=%d %s\n", __func__,
		       conn->spl_id,
		       conn->dev ? conn->dev->name: "");
	}
exit:
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}

int spl_conn_get(int inst, enum DP_SPL_TYPE type,
		    struct dp_spl_cfg *conns, u8 total)
{
	struct hal_priv *hal = HAL(inst);
	u8 cnt = 0;
	int i;

	if (!dp_init_ok) {
		pr_err("DPM: DP not initialized yet\n");
		return 0;
	}
	if (!hal) {
		pr_err_once("%s hal[%d] is NULL\n", __func__, inst);
		return 0;
	}
	if (type >= DP_SPL_MAX) {
		pr_err("DPM: wrong type: %d\n", type);
		return 0;
	}

	if (type != DP_SPL_PP_NF)
		total = 1;
	DP_DEBUG(DP_DBG_FLAG_SPL, "type:%s cnt:%d", get_spl_name(type), total);
	for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
		struct dp_spl_cfg *cfg = &hal->spl[i];
		/* for PP NF case it will return first PP NF */
		if (!cfg->spl_id || cfg->type != type)
			continue;
		dp_memcpy(conns + cnt, cfg, sizeof(struct dp_spl_cfg));
		if (++cnt >= total)
			break;
	}
	return cnt;
}

int _dp_spl_conn_get(int inst, enum DP_SPL_TYPE type,
		       struct dp_spl_cfg *conns, u8 total)
{
	int ret;

	DP_LIB_LOCK(&dp_lock);
	ret = spl_conn_get(inst, type, conns, total);
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}

int dp_spl_conn_get_num_of_egps(int inst, int egps[16])
{
	struct hal_priv *hal = HAL(inst);
	int cnt = 0;
	int i;

	if (!dp_init_ok) {
		pr_err("DPM: DP not initialized yet\n");
		return 0;
	}
	if (!hal) {
		pr_err_once("%s hal[%d] is NULL\n", __func__, inst);
		return 0;
	}

	DP_LIB_LOCK(&dp_lock);
	for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
		struct dp_spl_cfg *cfg = &hal->spl[i];
		int c;

		if (!cfg->spl_id)
			continue;
		for (c = 0; c < cfg->num_egp; c++)
			egps[cnt++] = cfg->egp[c].egp_id;
	}
	DP_LIB_UNLOCK(&dp_lock);
	return cnt;
}

static const char *get_dev_name(int inst, const struct dp_spl_cfg *c)
{
	return get_dp_port_subif(get_dp_port_info(inst, CPU_PORT),
				 c->spl_id)->device_name;
}

const char *dp_spl_conn_get_name_from_egp(int inst, int egp_id)
{
	struct hal_priv *hal = HAL(inst);
	const char *dev_name = NULL;
	int i;

	if (!dp_init_ok) {
		pr_err("DPM: DP not initialized yet\n");
		return 0;
	}
	if (!hal) {
		pr_err_once("%s hal[%d] is NULL\n", __func__, inst);
		return 0;
	}
	DP_LIB_LOCK(&dp_lock);
	for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
		struct dp_spl_cfg *cfg = &hal->spl[i];
		int c;

		if (!cfg->spl_id)
			continue;
		for (c = 0; c < cfg->num_egp; c++) {
			if (cfg->egp[c].egp_id == egp_id) {
				dev_name = get_dev_name(inst, cfg);
				goto exit;
			}
		}
	}
exit:
	DP_LIB_UNLOCK(&dp_lock);
	return dev_name;
}

/*Just to tell if this egp is belongs to spl device*/
int dp_is_spl_conn(int inst, int egp_id)
{
	int found = 0;
	struct hal_priv *hal = HAL(inst);
	int i;

	if (!dp_init_ok) {
		pr_err("DPM: DP not initialized yet\n");
		return 0;
	}
	if (!hal) {
		pr_err_once("%s hal[%d] is NULL\n", __func__, inst);
		return 0;
	}

	DP_LIB_LOCK(&dp_lock);
	for (i = 0; i < MAX_SPL_CONN_CNT; i++) {
		struct dp_spl_cfg *cfg = &hal->spl[i];
		int c;

		if (!cfg->spl_id)
			continue;
		for (c = 0; c < cfg->num_egp; c++) {
			if (cfg->egp[c].egp_id == egp_id) {
				found = 1;
				goto exit;
			}
		}
	}
exit:
	DP_LIB_UNLOCK(&dp_lock);
	return found;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

#endif

