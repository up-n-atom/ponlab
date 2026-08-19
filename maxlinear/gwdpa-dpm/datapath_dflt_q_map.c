/******************************************************************************
 * Copyright (c) 2021 - 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <net/datapath_api.h>
#include "datapath.h"
#include "hal/datapath_misc.h"

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
#include "datapath_dflt_q_map_prx300.h"
#else
#include "datapath_dflt_q_map_lgm.h"
#endif

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

/* default cpu VAP set */
int dflt_cpu_vap[DP_MAX_INST] = {-1};
/* default queue of cpu for CPU RX path. */
int dflt_q_cpu[DP_MAX_INST] = {-1};
/* default queue of toe for TOE Engine to CQM enqueue */
int dflt_q_toe[DP_MAX_INST] = {-1};
/* default queue of re-insertion traffic:
 * from CPU to re-insertion queeu to GSWP
 */
int dflt_q_re_insert[DP_MAX_INST] = {-1};
int dflt_q_drop[DP_MAX_INST] = {-1}; /* drop queue */

static struct q_map_info q_map_ports[] = {
	{
		.name = "CPU",
		.alloc_flag = DP_F_CPU,
		.num = ARRAY_SIZE(q_map_cpu),
		.entry = q_map_cpu,
	},
	{
		.name = "Ethernet",
		.alloc_flag = DP_F_FAST_ETH_LAN | DP_F_FAST_ETH_WAN | DP_F_GINT,
		.num = ARRAY_SIZE(q_map_stream_ethernet),
		.entry = q_map_stream_ethernet,
	},
	{
		.name = "PON",
		.alloc_flag = DP_F_GPON | DP_F_EPON,
		.num = ARRAY_SIZE(q_map_pon),
		.entry = q_map_pon,
	},
	{
		.name = "VUNI",
		.alloc_flag = DP_F_VUNI,
		.num = ARRAY_SIZE(q_map_vUNI),
		.entry = q_map_vUNI,
	},
#if !IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	{
		.name = "DOCSIS",
		.alloc_flag = DP_F_DOCSIS,
		.num = ARRAY_SIZE(q_map_docsis),
		.entry = q_map_docsis,
	},
#endif
	{
		.name = "ACA/DC",
		.alloc_flag = DP_F_ACA,
		.num = ARRAY_SIZE(q_map_aca),
		.entry = q_map_aca,
	},
	{
		.name = "DIRECTPATH",
		.alloc_flag = DP_F_DIRECT,
		.num = ARRAY_SIZE(q_map_directpath),
		.entry = q_map_directpath,
	}
};

static struct q_map_info q_map_spl_conn[] = {
	{
		.name = "SPL_CONN",
		.num = ARRAY_SIZE(q_map_spl_conn_igp),
		.entry = q_map_spl_conn_igp,
	}
};

static struct q_map_info q_map_reins[] = {
	{
		.name = "REINS",
		.num = ARRAY_SIZE(q_map_reinsert),
		.entry = q_map_reinsert,
	}
};

/* Initialize one subif's queue map rule content only,
 * not really setting CQM lookup queue mapping table
 */
int _dp_init_subif_q_map_rules(
	struct dp_subif_info *subif_info,
	int spl_conn_igp)
{
	int i;
	int res = DP_FAILURE;
	const struct pmac_port_info *port_info; /* points to port_info */

	if (!subif_info) {
		pr_err("DPM: %s subif_info NULL\n", __func__);
		return DP_FAILURE;
	}
	if (!subif_info->port_info) {
		pr_err("DPM: %s subif_info->port_info NULL\n", __func__);
		return DP_FAILURE;
	}
	if (!subif_info->port_info->inst_prop) {
		pr_err("DPM: %s subif_info->port_info->inst_prop NULL\n", __func__);
		return DP_FAILURE;
	}
	port_info = subif_info->port_info;

	if (spl_conn_igp) {
		subif_info->q_map = q_map_spl_conn;
		res = DP_SUCCESS;
	} else if (subif_info->data_flag_ops & DP_SUBIF_VANI) {
		subif_info->q_map = NULL;
		res = DP_SUCCESS;
	} else {
		for (i = 0; i < ARRAY_SIZE(q_map_ports); i++) {
			if (q_map_ports[i].alloc_flag &
			    port_info->alloc_flags) {
				if (q_map_ports[i].num)
					subif_info->q_map = &q_map_ports[i];
				else /* empty queue map rule */
					subif_info->q_map = NULL;
				res = DP_SUCCESS;
				break;
			}
		}
	}
	if (res != DP_SUCCESS) /* not match */
		subif_info->q_map = NULL;

	_DP_DEBUG(DP_DBG_FLAG_QMAP, "DPM: %s:%s=%d %s=%x %s=%x %s=%x %s=%d %s\n",
		 __func__,
		 "ep", port_info->port_id,
		 "subif_grp", subif_info->subif_groupid,
		 "alloc_flag", port_info->alloc_flags,
		 "subif_flag_ops", subif_info->data_flag_ops,
		 "spl_conn", spl_conn_igp,
		 subif_info->q_map ?  subif_info->q_map->name : "Drop_Queue");
	return res;
}

/* reset one subif's CQM lookup queue mapping entries */
int _dp_reset_subif_q_lookup_tbl(
	struct dp_subif_info *subif_info,
	int cls_idx, int reins_inst)
{
	int ret = DP_FAILURE;
	cbm_queue_map_entry_t *lookup;
	struct cqm_port_info *cqm_info;
	u8 cid, pid;
	u16 nid;

	/* subif_info can be NULL for reinsertion */
	if (!subif_info)
		goto LOOKUP;

	if (!subif_info->port_info) {
		pr_err("DPM: %s subif_info->port_info NULL\n", __func__);
		return DP_FAILURE;
	}
	if (!subif_info->port_info->inst_prop) {
		pr_err("DPM: %s subif_info->port_info->inst_prop NULL\n", __func__);
		return DP_FAILURE;
	}

	if (dflt_q_drop[subif_info->port_info->inst] < 0)
		dflt_q_drop[subif_info->port_info->inst] = 0;
LOOKUP:
	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;

	if (!subif_info && reins_inst != -1) {
		cqm_info = get_dp_deqport_info(reins_inst,
					       reinsert_deq_port[reins_inst]);
		dp_dma_parse_id(cqm_info->dma_chan, &cid, &pid, &nid);
		lookup->ep = dp_get_pmac_id(reins_inst, cid);
		if (cbm_queue_map_set(0, dflt_q_drop[reins_inst], lookup,
				      subif_q_map_reset_lookup_f)) {
			pr_err("DPM: %s: cbm_queue_map_set failed\n", __func__);
			goto EXIT;
		}
		_DP_DEBUG(DP_DBG_FLAG_QMAP, "DPM: %s:%s=%d %s=0x%x %s=0x%x %s=%d\n",
			 __func__, "dp_port", lookup->ep,
			 "subif", lookup->sub_if_id,
			 "lookup_f", subif_q_map_reset_lookup_f,
			  "qid", dflt_q_drop[reins_inst]);
		ret = DP_SUCCESS;
		goto EXIT;
	} else if (!subif_info) {
		ret = DP_FAILURE;
		goto EXIT;
	} else if (subif_info->port_info->alloc_flags & DP_F_EPON) {
		lookup->sub_if_id = subif_info->deq_port_idx + cls_idx;
	} else if (cls_idx) {
		/* for non-EPON, no need special handling with
		 *  non zero cls_idx and just exit
		 */
		ret = DP_SUCCESS;
		goto EXIT;
	} else {
		lookup->sub_if_id = subif_info->subif;
	}
	lookup->ep = subif_info->port_info->port_id;
	if (cbm_queue_map_set(subif_info->port_info->inst_prop->cbm_inst,
			      dflt_q_drop[subif_info->port_info->inst], lookup,
			      subif_q_map_reset_lookup_f)) {
		pr_err("DPM: %s: cbm_queue_map_set failed\n", __func__);
		goto EXIT;
	}

	_DP_DEBUG(DP_DBG_FLAG_QMAP,
		 "DPM: %s:%s=%d %s=0x%x %s=0x%x %s=%d\n",
		 __func__,
		 "dp_port", lookup->ep,
		 "subif", lookup->sub_if_id,
		 "lookup_f", subif_q_map_reset_lookup_f,
		 "qid", dflt_q_drop[subif_info->port_info->inst]);
	ret = DP_SUCCESS;
EXIT:
	kfree(lookup);
	return ret;
}

/* reset whole lookup table */
int _dp_reset_q_lookup_tbl(int inst)
{
	int ret = DP_FAILURE;
	cbm_queue_map_entry_t *lookup;
	struct inst_property *inst_prop = get_dp_port_prop(inst);

	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;

	if (cbm_queue_map_set(inst_prop->cbm_inst,
			      dflt_q_drop[inst], lookup, -1)) {
		pr_err("DPM: %s: cbm_queue_map_set failed\n", __func__);
		goto EXIT;
	}
	ret = DP_SUCCESS;
EXIT:
	kfree(lookup);
	return ret;
}

#define get_deq_port_idx(x, y) x->rx_ring[0].egp[y].deq_port

/* set one subif's CQM lookup queue mapping entries */
int _dp_set_subif_q_lookup_tbl(
	struct dp_subif_info *subif_info,
	int cls_idx, int spl_conn_igp_qid,
	int reins_inst)
{
	int ret = DP_FAILURE;
	int i, j;
	int qid = -1;
	int inst = 0, num_egp, idx;
	bool workaround_done = false;
	cbm_queue_map_entry_t *lookup = NULL;
	struct q_map_info *q_map;
	struct cqm_port_info *cqm_info;
	const struct pmac_port_info *port_info;
	u8 cid, pid;
	u16 nid;
	int cbm_inst;

	/* subif_info can be NULL for reinsertion */
	if (!subif_info) {
		inst = reins_inst;
		cbm_inst = 0;
		port_info = get_dp_port_info(inst, 0);
		goto LOOKUP;
	}
	port_info = subif_info->port_info;
	cbm_inst = port_info->inst_prop->cbm_inst;

	if (!subif_info->port_info) {
		pr_err("DPM: %s subif_info->port_info NULL\n", __func__);
		return DP_FAILURE;
	}
	if (!subif_info->port_info->inst_prop) {
		pr_err("DPM: %s subif_info->port_info->inst_prop NULL\n", __func__);
		return DP_FAILURE;
	}

	inst = subif_info->port_info->inst;
	/* if no q_map rules, then map to drop queue for ths subif */
	if (!subif_info->q_map) {
		_dp_reset_subif_q_lookup_tbl(subif_info, cls_idx, -1);
		return DP_SUCCESS;
	}
LOOKUP:
	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;
	if (!subif_info) {
		cqm_info = get_dp_deqport_info(inst, reinsert_deq_port[inst]);
		dp_dma_parse_id(cqm_info->dma_chan, &cid, &pid, &nid);
		lookup->ep = dp_get_pmac_id(inst, cid);
		q_map = q_map_reins;
		goto LOOP;
	} else if (port_info->alloc_flags & DP_F_EPON) {
		lookup->sub_if_id = subif_info->deq_port_idx + cls_idx;
	} else if(cls_idx) {
		/* for non-EPON, no need special handling with
		 * non zero cls_idx
		 * For all other non-EPON device,DPM will only map to
		 * first queue even with multiple dequeue port/queue
		 */
		kfree(lookup);
		return DP_SUCCESS;
	} else {
		lookup->sub_if_id = subif_info->subif;
	}
	lookup->ep = port_info->port_id;
	q_map = subif_info->q_map;
LOOP:
	for (i = 0; (i < q_map->num) && q_map->entry; i++) { /* loop on rule */
		/* sanity check traffic class range */
		if (q_map->entry[i].cls1 > MAX_TC_ID)
			q_map->entry[i].cls1 = MAX_TC_ID;
		if (q_map->entry[i].cls2 > MAX_TC_ID)
			q_map->entry[i].cls2 = MAX_TC_ID;
		if (q_map->entry[i].cls1 > q_map->entry[i].cls2)
			q_map->entry[i].cls1 =  q_map->entry[i].cls2;

		lookup->mpe1 = q_map->entry[i].mpe1;
		lookup->mpe2 = q_map->entry[i].mpe2;
		lookup->enc = q_map->entry[i].enc;
		lookup->egflag = q_map->entry[i].egflag;

		if (q_map->entry[i].q_type == DP_MAP_Q_CPU) {
			qid = dflt_q_cpu[inst];
		} else if (q_map->entry[i].q_type == DP_MAP_Q_SUBIF &&
			   subif_info) {
			qid = subif_info->qid_list[cls_idx];
		} else if (q_map->entry[i].q_type == DP_MAP_Q_REINSERT) {
			qid = dflt_q_re_insert[inst];
		} else if (q_map->entry[i].q_type == DP_MAP_Q_TOE) {
			qid = dflt_q_toe[inst];
		} else if (q_map->entry[i].q_type == DP_MAP_Q_DC_RXOUT_QOS) {
			/* set dynamic qid per tc later */
		} else if (spl_conn_igp_qid) {
			qid = spl_conn_igp_qid;
		} else { /* subif default queue */
			DP_INFO("%s wrong q_type=%d\n", __func__,
				q_map->entry[i].q_type);
			goto EXIT;
		}
		if (qid < 0) {
			/* maybe TOE related driver not insmod yet
			 * So skip it and continue other rules
			 */
			_DP_DEBUG(DP_DBG_FLAG_QMAP, "DPM: %s: may be some driver has "
					"not registered yet, for eg: toe and so on, "
					"%s=%d %s=%d %s=%d %s=%d\n",
					__func__,
					"qid", qid,
					"q_type", q_map->entry[i].q_type,
					"spl_conn_igp_qid", spl_conn_igp_qid,
					"cls_idx", cls_idx);
			continue;
		}
		/* apply lookup per traffic class */
		for (j = q_map->entry[i].cls1; j <= q_map->entry[i].cls2; j++) {
			lookup->tc = j;

			if ((q_map->entry[i].q_type == DP_MAP_Q_DC_RXOUT_QOS) &&
			    port_info->num_rx_ring &&
			    port_info->rx_ring[0].num_egp) {
				/* tune qid per tc for special handling:
				 * for non qos bypass mode, ie, in PP QOS mode
				 * in rxout path
				 */
				num_egp = port_info->rx_ring[0].num_egp;
				if (j >= num_egp) /* use last valid egp */
					idx = get_deq_port_idx(port_info,
							       num_egp - 1);
				else
					idx = get_deq_port_idx(port_info, j);
				cqm_info = get_dp_deqport_info(inst, idx);
				qid = cqm_info->qid[0];
			}
			if (cbm_queue_map_set(cbm_inst, qid, lookup,
					      q_map->entry[i].lookup_f)) {
				pr_err("DPM: %s: cbm_queue_map_set failed\n", __func__);
				goto EXIT;
			}
		}

		if (subif_info && (q_map->entry[i].q_type == DP_MAP_Q_TOE)) {
			subif_info->toe_tc = q_map->entry[i].cls2;
		}
		if (is_soc_prx(inst))
			_DP_DEBUG(DP_DBG_FLAG_QMAP,
				 "DPM: %s:%s=%d %s=0x%x %s=%d %s=%d %s=%d %s=%d-%d %s=%x %s=0x%x %s=%d\n",
				 __func__,
				 "dp_port", lookup->ep,
				 "subif", lookup->sub_if_id,
				 "mpe1", lookup->mpe1,
				 "mpe2", lookup->mpe2,
				 "enc", lookup->enc,
				 "tc", q_map->entry[i].cls1, q_map->entry[i].cls2,
				 "subif_flag_ops",
				 subif_info ? subif_info->data_flag_ops :
					DP_SUBIF_REINSERT,
				 "lookup_f", q_map->entry[i].lookup_f,
				 "qid", qid);
		else
			_DP_DEBUG(DP_DBG_FLAG_QMAP,
				 "DPM: %s:%s=%d %s=0x%x %s=%d %s=%d-%d %s=%x %s=0x%x %s=%d\n",
				 __func__,
				 "dp_port", lookup->ep,
				 "subif", lookup->sub_if_id,
				 "egflag", lookup->egflag,
				 "tc", q_map->entry[i].cls1, q_map->entry[i].cls2,
				 "subif_flag_ops",
				 subif_info ? subif_info->data_flag_ops :
					DP_SUBIF_REINSERT,
				 "lookup_f", q_map->entry[i].lookup_f,
				 "qid", qid);

		if (!subif_info) {
			ret = DP_SUCCESS;
			goto EXIT;
		}

		if (subif_bit8_workaround(subif_info) && !workaround_done) {
			/* set bit 8 1 */
			lookup->sub_if_id = subif_info->subif | BIT(8);
			workaround_done = true;
			goto LOOP;
		}

	}
	ret = DP_SUCCESS;
EXIT:
	kfree(lookup);
	return ret;
}

u32 get_subif_q_map_reset_flag(int inst)
{
	return subif_q_map_reset_lookup_f;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

