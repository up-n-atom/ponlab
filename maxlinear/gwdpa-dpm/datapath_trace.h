/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 *****************************************************************************/
#undef TRACE_SYSTEM
#define TRACE_SYSTEM dpm

#if !defined(DATAPATH_TRACE_H_) || defined(TRACE_HEADER_MULTI_READ)
#define DATAPATH_TRACE_H_

#include <linux/tracepoint.h>
#include <net/datapath_api.h>
#include <net/datapath_api_tx.h>
#include "datapath.h"
#include "hal/datapath_ppv4.h"
#include "datapath_switchdev.h"

#define DP_TX_INFO \
	" %s=%d %s=%d %s=%d\n"
#define DP_TX_DW0 \
	"%s=%d %s=0x%04x\n"
#define DP_TX_DW3 \
	"%s=%d %s=%d %s=%d\n"
#define DP_TX_PMAC\
	"0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n"
#define DP_RX_INFO \
	" %s=%d\n"

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
#define DP_TX_DW1 \
	"%s=%d %s=%d %s=%d\n"
#define DP_TX_PMAC_INFO\
	"%s %s=%d %s=%d %s=%d\n"
#define DP_RX_PMAC\
	"PMAC:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n %s=%d %s=%d %s=%d\n"
#else
#define DP_TX_DW1 \
	"%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n"
#define DP_TX_PMAC_INFO\
	"%s %s=%d %s=%d %s=%d %s=%d\n"
#define DP_RX_PMAC\
	"PMAC:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n %s=%d %s=%d %s=%d %s=%d\n"
#endif
#define DP_RX_DW0 DP_TX_DW0
#define DP_RX_DW1 DP_TX_DW1
#define DP_RX_DW3 DP_TX_DW3

TRACE_EVENT(dp_tx,
	TP_PROTO(int dpid, struct dma_rx_desc_0 *desc0,
		 struct dma_rx_desc_1 *desc1, struct dma_rx_desc_2 *desc2,
		 struct dma_rx_desc_3 *desc3, struct sk_buff *skb, bool tx, int vap),
	TP_ARGS(dpid, desc0, desc1, desc2, desc3, skb, tx, vap),
	TP_STRUCT__entry(
		__field(int, dp_port)
		__field(int, vap)
		__field(int, len)
		__field(u32, dataptr)
		__field(u32, data)
		__field(bool, tx)
		__field(u16, redir)
		__field(u16, subif)
		__field(u16, sess)
		__field(u16, flowid)
		__field(u16, ep)
		__field(u16, ip)
		__field(u16, header_mode)
		__field(u16, lro_type)
		__field(u16, desc_offset)
		__field(u16, pmac)
		__field(u16, haddr)
		__field(u16, pool)
		__field(u16, policy)
		__field(u16, own)
		),
	TP_fast_assign(
		__entry->dp_port = dpid;
		__entry->vap = vap;
		__entry->dataptr = desc2->field.data_ptr;
		__entry->len = skb->len;
		__entry->tx = tx;
		__entry->subif = desc0->field.dest_sub_if_id;
		__entry->ep = desc1->field.ep;
		__entry->own = desc3->field.own;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
		__entry->desc_offset = desc3->field.byte_offset;
		__entry->redir = desc0->field.redir;
		__entry->pool = desc3->field.pool;
		__entry->ip = desc1->field.ip;
		__entry->sess = desc1->field.session_id;
		__entry->policy = desc3->field.policy;
#else
		__entry->desc_offset = desc2->field.byte_offset;
		__entry->flowid = desc0->field.flow_id;
		__entry->redir = desc1->field.redir;
		__entry->header_mode = desc1->field.header_mode;
		__entry->lro_type = desc1->field.lro_type;
		__entry->pmac = desc1->field.pmac;
		__entry->haddr = desc3->field.haddr;
		__entry->pool = desc1->field.src_pool;
		__entry->policy = desc3->field.pool_policy;
#endif
		),

		/* res dptr offset len dpid vap redir dst_sif src_sif ep ip own
		* pool policy
		*/
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	TP_printk("%s %s=0x%x %s=%d" DP_TX_INFO DP_TX_DW0 DP_TX_DW1 DP_TX_DW3,
		   __entry->tx ? "orig" : "updated",
		  "Dataptr", __entry->dataptr, "offset", __entry->desc_offset,
		  "skb->len", __entry->len, "from dp_port", __entry->dp_port, "vap", __entry->vap,
		  "DW0:redir", __entry->redir, "dst_subif", __entry->subif,
		  "DW1:sess/src_sif", __entry->sess, "ep", __entry->ep, "ip", __entry->ip,
		  "DW3:own", __entry->own, "pool", __entry->pool, "policy", __entry->policy)
#else
	TP_printk("%s %s=0x%x %s=%d" DP_TX_INFO DP_TX_DW0 DP_TX_DW1 DP_TX_DW3,
		  __entry->tx ? "orig" : "updated",
		  "Dataptr", __entry->dataptr, "offset", __entry->desc_offset,
		  "skb->len", __entry->len, "from dp_port", __entry->dp_port, "vap", __entry->vap,
		  "DW0:flowid", __entry->flowid, "dst_subif", __entry->subif,
		  "DW1:redir", __entry->redir, "ep", __entry->ep,
		  "header_mode", __entry->header_mode, "lro_type", __entry->lro_type,
		  "pmac", __entry->pmac, "src_pool", __entry->pool,
		  "DW3:own", __entry->own, "policy", __entry->policy,
		  "haddr", __entry->haddr)
#endif
);

TRACE_EVENT(dp_tx_pmac,
	TP_PROTO(struct pmac_tx_hdr *pmac, bool flags),
	TP_ARGS(pmac, flags),
	TP_STRUCT__entry(
		__array(unsigned char, pmac_hdr, PMAC_TX_HDR_SIZE)
		__field(u16, src_dst_subif_msb)
		__field(u16, src_dst_subif_lsb)
		__field(u16, igp_egp)
		__field(u16, igp_msb)
		__field(bool, tx_flags)
		),
	TP_fast_assign(
		dp_memcpy(__entry->pmac_hdr, pmac, PMAC_TX_HDR_SIZE);
		__entry->src_dst_subif_msb = pmac->src_dst_subif_id_msb;
		__entry->src_dst_subif_lsb = pmac->src_dst_subif_id_lsb;
		__entry->igp_egp = pmac->igp_egp;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		__entry->igp_msb = pmac->src_dst_subif_id_14_12;
#endif
		__entry->tx_flags = flags;
		),

		/* pmac_ptr pmac_hdr src_dst_sif igp_egp */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	TP_printk("PMAC at 0x%px:" DP_TX_PMAC DP_TX_PMAC_INFO,
		  __entry->pmac_hdr, __entry->pmac_hdr[0], __entry->pmac_hdr[1],
		  __entry->pmac_hdr[2], __entry->pmac_hdr[3], __entry->pmac_hdr[4],
		  __entry->pmac_hdr[5], __entry->pmac_hdr[6], __entry->pmac_hdr[7],
		  __entry->tx_flags ? "before" : "after",
		  "src_dst_subif_id_msb", __entry->src_dst_subif_msb,
		  "src_dst_subif_id_lsb", __entry->src_dst_subif_lsb,
		  "igp_egp", __entry->igp_egp)
#else
	TP_printk("PMAC at 0x%px:" DP_TX_PMAC DP_TX_PMAC_INFO,
		__entry->pmac_hdr, __entry->pmac_hdr[0],
		__entry->pmac_hdr[1], __entry->pmac_hdr[2], __entry->pmac_hdr[3],
		__entry->pmac_hdr[4], __entry->pmac_hdr[5], __entry->pmac_hdr[6],
		__entry->pmac_hdr[7], __entry->tx_flags ? "before" : "after",
		"igp_msb", __entry->igp_msb,
		"src_dst_subif_id_msb", __entry->src_dst_subif_msb,
		"src_dst_subif_id_lsb", __entry->src_dst_subif_lsb,
		"igp_egp", __entry->igp_egp)
#endif
);

TRACE_EVENT(dp_rx,
	TP_PROTO(int res, int dpid, int gpid, struct dp_subif_info *sif, int vap,
			struct dma_rx_desc_0 *desc0,
		   struct dma_rx_desc_1 *desc1, struct dma_rx_desc_2 *desc2,
		   struct dma_rx_desc_3 *desc3, struct sk_buff *skb, struct pmac_rx_hdr *pmac),
	TP_ARGS(res, dpid, gpid, sif, vap, desc0, desc1, desc2, desc3, skb, pmac),
	TP_STRUCT__entry(
		__field(int, dp_port)
		__field(int, gpid)
		__field(int, vap)
		__array(char, dev_name, IFNAMSIZ)
		__field(int, res)
		__field(int, len)
		__field(u32, dataptr)
		__field(u32, data)
		__field(u16, redir)
		__field(u16, subif)
		__field(u16, sess)
		__field(u16, flowid)
		__field(u16, ep)
		__field(u16, ip)
		__field(u16, header_mode)
		__field(u16, lro_type)
		__field(u16, desc_offset)
		__field(u16, pmac)
		__field(u16, haddr)
		__field(u16, pool)
		__field(u16, policy)
		__field(u16, own)
		__array(unsigned char, pmac_hdr, PMAC_HDR_SIZE)
		__field(u16, src_dst_subif_msb)
		__field(u16, src_dst_subif_lsb)
		__field(u16, igp_egp)
		__field(u16, igp_msb)
		),
	TP_fast_assign(
		__entry->dp_port = dpid;
		__entry->vap = vap;
		__entry->res = res;
		strlcpy(__entry->dev_name,
			(sif ? (sif->netif ? sif->netif->name : "") : ""), IFNAMSIZ);
		__entry->dataptr = desc2->field.data_ptr;
		__entry->len = skb->len;
		__entry->subif = desc0->field.dest_sub_if_id;
		__entry->ep = desc1->field.ep;
		__entry->own = desc3->field.own;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
		__entry->desc_offset = desc3->field.byte_offset;
		__entry->redir = desc0->field.redir;
		__entry->pool = desc3->field.pool;
		__entry->ip = desc1->field.ip;
		__entry->sess = desc1->field.session_id;
		__entry->policy = desc3->field.policy;
#else
		__entry->gpid = gpid;
		__entry->desc_offset = desc2->field.byte_offset;
		__entry->flowid = desc0->field.flow_id;
		__entry->redir = desc1->field.redir;
		__entry->header_mode = desc1->field.header_mode;
		__entry->lro_type = desc1->field.lro_type;
		__entry->pmac = desc1->field.pmac;
		__entry->haddr = desc3->field.haddr;
		__entry->pool = desc1->field.src_pool;
		__entry->policy = desc3->field.pool_policy;
		__entry->igp_msb = pmac->src_dst_subif_id_14_12;
#endif
		dp_memcpy(__entry->pmac_hdr, pmac, PMAC_HDR_SIZE);
		__entry->src_dst_subif_msb = pmac->src_dst_subif_id_msb;
		__entry->src_dst_subif_lsb = pmac->src_dst_subif_id_lsb;
		__entry->igp_egp = pmac->igp_egp;
	),
	/* res dp_port dev vap gpid
	 * dptr offset len dpid vap redir dst_sif src_sif ep ip own
	 * pool policy
	 */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	TP_printk("%s %s=%d dev=%s %s=%d %s=0x%x %s=%d" DP_RX_INFO DP_RX_DW0 DP_RX_DW1 DP_RX_DW3 DP_RX_PMAC,
		  __entry->res ? "rx_err" : "rx_success", "dp_port", __entry->dp_port,
		  __entry->dev_name, "vap", __entry->vap,
		  "Dataptr", __entry->dataptr, "offset", __entry->desc_offset,
		  "skb->len", __entry->len,
		  "DW0:redir", __entry->redir, "dst_subif", __entry->subif,
		  "DW1:sess/src_sif", __entry->sess, "ep", __entry->ep, "ip", __entry->ip,
		  "DW3:own", __entry->own, "pool", __entry->pool, "policy", __entry->policy,
		  __entry->pmac_hdr[0], __entry->pmac_hdr[1],
		  __entry->pmac_hdr[2], __entry->pmac_hdr[3], __entry->pmac_hdr[4],
		  __entry->pmac_hdr[5], __entry->pmac_hdr[6], __entry->pmac_hdr[7],
		  "src_dst_subif_id_msb", __entry->src_dst_subif_msb,
		  "src_dst_subif_id_lsb", __entry->src_dst_subif_lsb, "igp_egp", __entry->igp_egp)
#else
	TP_printk("%s %s=%d dev=%s %s=%d %s=%d %s=0x%x %s=%d" DP_RX_INFO DP_TX_DW0 DP_TX_DW1 DP_TX_DW3 DP_RX_PMAC,
		  __entry->res ? "rx_err" : "rx_success", "dp_port", __entry->dp_port,
		  __entry->dev_name, "vap", __entry->vap, "gpid", __entry->gpid,
		  "Dataptr", __entry->dataptr, "offset", __entry->desc_offset,
		  "skb->len", __entry->len,
		  "DW0:flowid", __entry->flowid, "dst_subif", __entry->subif,
		  "DW1:redir", __entry->redir, "ep", __entry->ep,
		  "header_mode", __entry->header_mode, "lro_type", __entry->lro_type,
		  "pmac", __entry->pmac, "src_pool", __entry->pool,
		  "DW3:own", __entry->own, "policy", __entry->policy, "haddr", __entry->haddr,
		  __entry->pmac_hdr[0], __entry->pmac_hdr[1],
		  __entry->pmac_hdr[2], __entry->pmac_hdr[3], __entry->pmac_hdr[4],
		  __entry->pmac_hdr[5], __entry->pmac_hdr[6], __entry->pmac_hdr[7],
		  "igp_msb", __entry->igp_msb,
		  "src_dst_subif_id_msb", __entry->src_dst_subif_msb,
		  "src_dst_subif_id_lsb", __entry->src_dst_subif_lsb, "igp_egp", __entry->igp_egp)
#endif
);

TRACE_EVENT(dp_netdev_event,
	TP_PROTO(unsigned long netdev_event, struct net_device *dev),
	TP_ARGS(netdev_event, dev),
	TP_STRUCT__entry(
		__field(unsigned long, event)
		__array(char, dev_name, IFNAMSIZ)
	),
	TP_fast_assign(
		__entry->event = netdev_event;
		strlcpy(__entry->dev_name, dev ? dev->name : "", IFNAMSIZ);
		),

	TP_printk("event=(%ld)%s dev=%s\n", __entry->event,
		  get_netdev_evt_name(__entry->event), __entry->dev_name)
);

TRACE_EVENT(dp_swdev_event,
	TP_PROTO(struct dp_swdev_data *dev_data, unsigned long value, struct net_device *dev,
		 const struct switchdev_attr *attr),
	TP_ARGS(dev_data, value, dev, attr),
	TP_STRUCT__entry(
		__field(u16, id)
		__field(unsigned long, value)
		__array(char, dev_name, IFNAMSIZ)
	),
	TP_fast_assign(
		strlcpy(__entry->dev_name, dev_data ? dev_data->dev->name :
			(dev ? dev->name : ""), IFNAMSIZ);
		__entry->id = attr ? attr->id : 0;
		__entry->value = value;
		),

	TP_printk("dev=%s attr=%s stp_state=%s br_flags=%s %s=%s\n",
		  __entry->dev_name, get_swdev_attr_name(__entry->id),
		  ((__entry->id == SWITCHDEV_ATTR_ID_PORT_STP_STATE) &&
		   (__entry->value >= 0)) ? get_stp_stat_str(__entry->value) : "",
		  ((__entry->id == SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS) &&
		   (__entry->value >= 0)) ? get_bport_flags(__entry->value) : "",
		  "vlan_filtering",
		  ((__entry->id == SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING) ?
		   (__entry->value ? "enabled" : "disable") : ""))
);

TRACE_EVENT(dp_swdev,
	TP_PROTO(bool res, int bp, int fid, struct net_device *dev),
	TP_ARGS(res, bp, fid, dev),
	TP_STRUCT__entry(
		__field(u16, bp)
		__field(u16, fid)
		__field(bool, res)
		__array(char, dev_name, IFNAMSIZ)
	),
	TP_fast_assign(
		__entry->bp = bp;
		__entry->fid = fid;
		__entry->res = res;
		strlcpy(__entry->dev_name, dev ? dev->name : "", IFNAMSIZ);
		),

	TP_printk("%s dev=%s bp=%d %s FID=%d\n", __entry->res ? "add:" : "remove:",
		  __entry->dev_name, __entry->bp, __entry->res ? "to" : "from",
		  __entry->fid)
);

TRACE_EVENT(dp_register_subif,
	TP_PROTO(int res, int inst, struct module *owner,
		 struct net_device *dev,
		 char *subif_name, dp_subif_t *subif_id,
		 struct dp_subif_data *data, u32 flags,
		 int old_subif),
	TP_ARGS(res, inst, owner, dev, subif_name, subif_id, data, flags,
		old_subif),
	TP_STRUCT__entry(
		__field(int, flags)
		__field(int, inst)
		__field(int, dp_port)
		__field(int, subif)
		__field(int, request_subif)
		__array(char, dev_name, IFNAMSIZ)
		__array(char, subif_name, IFNAMSIZ)
		__array(char, ctp_name, IFNAMSIZ)
		__field(int, cqm_deq_idx)
		__field(int, num_cqm_deq)
		__field(u16, qid)
		__field(u8, domain_flag)
		__field(u8, domain_id)
		__field(u32, domain_member)
		__field(int, bp)
		__field(int, res)
	),
	TP_fast_assign(
		__entry->flags = flags;
		__entry->inst = inst;
		__entry->dp_port = subif_id->port_id;
		__entry->subif = subif_id->subif;
		__entry->request_subif = old_subif;
		strlcpy(__entry->dev_name, dev ? dev->name : "", IFNAMSIZ);
		strlcpy(__entry->subif_name, subif_name ? subif_name : "",
			   IFNAMSIZ);
		strlcpy(__entry->ctp_name,
			   data && data->ctp_dev ? data->ctp_dev->name : "",
			   IFNAMSIZ);
		__entry->cqm_deq_idx = data->deq_port_idx;
		__entry->num_cqm_deq = data->num_deq_port;
		__entry->domain_id = data->domain_id;
		__entry->domain_member = data->domain_members;
		__entry->qid = subif_id->def_qid;
		__entry->bp = subif_id->bport;
		__entry->res = res;
	),

	/*       res/state  inst  dp_port subif     dev  subif_ ctp_dev cqm_ qid  domain_ domain_ bp num_cqm_deq
	 *                                          name                deq_idx   id      member
	 */
	TP_printk("%s to %s %s=%d %s=%d %s=0x%x<-%x %s=%s %s=%s %s=%s %s=%d %s=%d %s=%d %s=0x%x %s=%d %s=%d\n",
		  !__entry->res ? "ok" : "fail",
		  __entry->flags & DP_F_DEREGISTER ? "deregister" : "register",
		  "inst", __entry->inst,
		  "dp_port", __entry->dp_port,
		  "subif", __entry->subif, __entry->request_subif,
		  "dev", __entry->dev_name,
		  "subif_name", __entry->subif_name,
		  "ctp_dev", __entry->ctp_name,
		  "cqm_deq_idx", __entry->cqm_deq_idx,
		  "qid", __entry->qid,
		  "domain_id", __entry->domain_id,
		  "domain_member", __entry->domain_member,
		  "bp", __entry->bp,
		  "num_cqm_deq", __entry->num_cqm_deq)
);

TRACE_EVENT(dp_update_subif_info,
	TP_PROTO(int res, struct dp_subif_upd_info *info,
		 struct net_device *old_dev, struct net_device *old_ctp_dev,
		 int old_cqm_deq_idx, u16 old_qid,
		 u8 old_domain_id, u32 old_domain_member, int old_bp,
		 struct dp_subif_info *sif),
	TP_ARGS(res, info, old_dev, old_ctp_dev, old_cqm_deq_idx, old_qid,
		old_domain_id, old_domain_member, old_bp, sif),
	TP_STRUCT__entry(
		__field(int, inst)
		__field(int, dp_port)
		__field(int, subif)
		__array(char, dev_name, IFNAMSIZ)
		__array(char, old_dev_name, IFNAMSIZ)
		__array(char, ctp_name, IFNAMSIZ)
		__array(char, old_ctp_name, IFNAMSIZ)
		__field(int, cqm_deq_idx)
		__field(int, old_cqm_deq_idx)
		__field(int, num_cqm_deq)
		__field(u16, qid)
		__field(u16, old_qid)
		__field(u8, domain_flag)
		__field(u8, domain_id)
		__field(u8, old_domain_id)
		__field(u32, domain_member)
		__field(u32, old_domain_member)
		__field(int, bp)
		__field(int, old_bp)
		__field(int, res)
	),
	TP_fast_assign(
		__entry->inst = info->inst;
		__entry->dp_port = info->dp_port;
		__entry->subif = info->subif;
		strlcpy(__entry->dev_name,
			   info->new_dev ? info->new_dev->name : "", IFNAMSIZ);
		strlcpy(__entry->old_dev_name,
			   old_dev ? old_dev->name : "", IFNAMSIZ);
		strlcpy(__entry->ctp_name,
			   info->new_ctp_dev ? info->new_ctp_dev->name : "",
			   IFNAMSIZ);
		strlcpy(__entry->old_ctp_name,
			   old_ctp_dev ? old_ctp_dev->name : "", IFNAMSIZ);
		__entry->cqm_deq_idx = info->new_cqm_deq_idx;
		__entry->old_cqm_deq_idx = old_cqm_deq_idx;
		__entry->num_cqm_deq = info->new_num_cqm_deq;
		__entry->domain_flag = info->new_domain_flag;
		__entry->domain_id = info->new_domain_flag ?
			info->new_domain_id : old_domain_id;
		__entry->old_domain_id = old_domain_id;
		__entry->domain_member = info->new_domain_flag ?
			info->domain_members : old_domain_member;
		__entry->old_domain_member = old_domain_member;
		__entry->qid = sif ? sif->qid : 0;
		__entry->old_qid = old_qid;
		__entry->bp = sif ? sif->bp : 0;
		__entry->old_bp = old_bp;
		__entry->res = res;
	),

	/*        res inst dp_port subif  dev       ctp_dev   cqm_      qid       domain_   domain_mem    bp       num_cqm_deq
	 *                                                    deq_idx             id
	 */
	TP_printk("%s %s=%d %s=%d %s=0x%x %s=%s<-%s %s=%s<-%s %s=%d<-%d %s=%d<-%d %s=%d<-%d %s=0x%x<-0x%x %s=%d<-%d %s=%d\n",
		  !__entry->res ? "ok" : "fail",
		  "inst", __entry->inst,
		  "dp_port", __entry->dp_port,
		  "subif", __entry->subif,
		  "dev", __entry->dev_name, __entry->old_dev_name,
		  "ctp_dev", __entry->ctp_name, __entry->old_ctp_name,
		  "cqm_deq_idx", __entry->cqm_deq_idx, __entry->old_cqm_deq_idx,
		  "qid", __entry->qid, __entry->old_qid,
		  "domain_id", __entry->domain_id, __entry->old_domain_id,
		  "domain_member", __entry->domain_member,
		  __entry->old_domain_member,
		  "bp", __entry->bp, __entry->old_bp,
		  "num_cqm_deq", __entry->num_cqm_deq)
);

TRACE_EVENT(dp_link_add,
	TP_PROTO(int res, struct dp_qos_link *old_cfg, struct dp_qos_link *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, old_cfg, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, indent)
		__field(int, res)
		__field(int, inst)
		__field(int, cqm_deq_port)
		__field(int, q_leaf)
		__field(int, n_sch_lvl)
		__field(int, qos_flag)
		__field(int, flag)
		__array(int, sch_leaf, DP_MAX_SCH_LVL)

		__field(int, dp_port)
		__field(int, q_id)
		__field(int, q_arbi)
		__field(int, q_prio_wfq)
		__array(int, sch_id, DP_MAX_SCH_LVL)
		__array(int, sch_prio_wfq, DP_MAX_SCH_LVL)
		__array(int, sch_arbi, DP_MAX_SCH_LVL)
		__field(int, old_dp_port)
		__field(int, old_q_id)
		__field(int, old_q_arbi)
		__field(int, old_q_prio_wfq)
		__array(int, old_sch_id, DP_MAX_SCH_LVL)
		__array(int, old_sch_prio_wfq, DP_MAX_SCH_LVL)
		__array(int, old_sch_arbi, DP_MAX_SCH_LVL)
		),
	TP_fast_assign(
		/*Unchanged variables*/
		__entry->indent = 71;
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->cqm_deq_port = old_cfg->cqm_deq_port;
		__entry->q_leaf = old_cfg->q_leaf;
		__entry->n_sch_lvl = old_cfg->n_sch_lvl;
		__entry->sch_leaf[0] = old_cfg->sch[0].leaf;
		__entry->sch_leaf[1] = old_cfg->sch[1].leaf;
		__entry->sch_leaf[2] = old_cfg->sch[2].leaf;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;

		/*Variables before change*/
		__entry->old_dp_port = old_cfg->dp_port;
		__entry->old_q_id = old_cfg->q_id;
		__entry->old_q_arbi = old_cfg->q_arbi;
		__entry->old_q_prio_wfq = old_cfg->q_prio_wfq;
		__entry->old_sch_id[0] = old_cfg->sch[0].id;
		__entry->old_sch_prio_wfq[0] = old_cfg->sch[0].prio_wfq;
		__entry->old_sch_arbi[0] = old_cfg->sch[0].arbi;
		__entry->old_sch_id[1] = old_cfg->sch[1].id;
		__entry->old_sch_prio_wfq[1] = old_cfg->sch[1].prio_wfq;
		__entry->old_sch_arbi[1] = old_cfg->sch[1].arbi;
		__entry->old_sch_id[2] = old_cfg->sch[2].id;
		__entry->old_sch_prio_wfq[2] = old_cfg->sch[2].prio_wfq;
		__entry->old_sch_arbi[2] = old_cfg->sch[2].arbi;

		/*Variables after change*/
		__entry->dp_port = cfg->dp_port;
		__entry->q_id = cfg->q_id;
		__entry->q_arbi = cfg->q_arbi;
		__entry->q_prio_wfq = cfg->q_prio_wfq;
		__entry->sch_id[0] = cfg->sch[0].id;
		__entry->sch_prio_wfq[0] = cfg->sch[0].prio_wfq;
		__entry->sch_leaf[0] = cfg->sch[0].leaf;
		__entry->sch_arbi[0] = cfg->sch[0].arbi;
		__entry->sch_id[1] = cfg->sch[1].id;
		__entry->sch_prio_wfq[1] = cfg->sch[1].prio_wfq;
		__entry->sch_leaf[1] = cfg->sch[1].leaf;
		__entry->sch_arbi[1] = cfg->sch[1].arbi;
		__entry->sch_id[2] = cfg->sch[2].id;
		__entry->sch_prio_wfq[2] = cfg->sch[2].prio_wfq;
		__entry->sch_leaf[2] = cfg->sch[2].leaf;
		__entry->sch_arbi[2] = cfg->sch[2].arbi;
		),
	TP_printk(
		"%s=%s %s=%d %s=%d %s=%d %s=%d %s=%s %s=%d\n"
		"%*s=%d->%d %s=%d->%d %s=%d->%d %s=%d->%d\n"
		"%*s: id: %d->%d, prio_wfq: %d->%d, leaf: %d, arbi: %d->%d\n"
		"%*s: id: %d->%d, prio_wfq: %d->%d, leaf: %d, arbi: %d->%d\n"
		"%*s: id: %d->%d, prio_wfq: %d->%d, leaf: %d, arbi: %d->%d\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,
		"cqm_deq_port", __entry->cqm_deq_port,
		"q_leaf", __entry->q_leaf,
		"n_sch_lvl", __entry->n_sch_lvl,
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag,

		__entry->indent, "dp_port", __entry->old_dp_port, __entry->dp_port,
		"q_id", __entry->old_q_id, __entry->q_id,
		"q_arbi", __entry->old_q_arbi, __entry->q_arbi,
		"q_prio_wfq", __entry->old_q_prio_wfq, __entry->q_prio_wfq,
#if (DP_MAX_SCH_LVL != 3)
#error "__func__: __LINE__: Need tuning trace event here"
#endif
		__entry->indent-1, __entry->n_sch_lvl > 0 ? "sch[0]" : "sch[Invalid]",
		__entry->old_sch_id[0], __entry->sch_id[0],
		__entry->old_sch_prio_wfq[0], __entry->sch_prio_wfq[0],
		__entry->sch_leaf[0],
		__entry->old_sch_arbi[0], __entry->sch_arbi[0],
		__entry->indent-1, __entry->n_sch_lvl > 1 ? "sch[1]" : "sch[Invalid]",
		__entry->old_sch_id[1], __entry->sch_id[1],
		__entry->old_sch_prio_wfq[1], __entry->sch_prio_wfq[1],
		__entry->sch_leaf[1],
		__entry->old_sch_arbi[1], __entry->sch_arbi[1],
		__entry->indent-1, __entry->n_sch_lvl > 2 ? "sch[2]" : "sch[Invalid]",
		__entry->old_sch_id[2], __entry->sch_id[2],
		__entry->old_sch_prio_wfq[2], __entry->sch_prio_wfq[2],
		__entry->sch_leaf[2],
		__entry->old_sch_arbi[2], __entry->sch_arbi[2])
);

TRACE_EVENT(dp_link_get,
	TP_PROTO(int res, struct dp_qos_link *cfg, enum QOS_FLAG qos_flag,
		int flag),
	TP_ARGS(res, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, indent)
		__field(int, res)
		__field(int, inst)
		__field(int, dp_port)
		__field(int, cqm_deq_port)
		__field(int, q_id)
		__field(int, q_arbi)
		__field(int, q_prio_wfq)
		__field(int, q_leaf)
		__field(int, n_sch_lvl)
		__array(int, sch_id, DP_MAX_SCH_LVL)
		__array(int, sch_prio_wfq, DP_MAX_SCH_LVL)
		__array(int, sch_leaf, DP_MAX_SCH_LVL)
		__array(int, sch_arbi, DP_MAX_SCH_LVL)
		__field(int, qos_flag)
		__field(int, flag)
		),
	TP_fast_assign(
		__entry->indent = 70;
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->dp_port = cfg->dp_port;
		__entry->cqm_deq_port = cfg->cqm_deq_port;
		__entry->q_id = cfg->q_id;
		__entry->q_arbi = cfg->q_arbi;
		__entry->q_prio_wfq = cfg->q_prio_wfq;
		__entry->q_leaf = cfg->q_leaf;
		__entry->n_sch_lvl = cfg->n_sch_lvl;
		__entry->sch_id[0] = cfg->sch[0].id;
		__entry->sch_prio_wfq[0] = cfg->sch[0].prio_wfq;
		__entry->sch_leaf[0] = cfg->sch[0].leaf;
		__entry->sch_arbi[0] = cfg->sch[0].arbi;
		__entry->sch_id[1] = cfg->sch[1].id;
		__entry->sch_prio_wfq[1] = cfg->sch[1].prio_wfq;
		__entry->sch_leaf[1] = cfg->sch[1].leaf;
		__entry->sch_arbi[1] = cfg->sch[1].arbi;
		__entry->sch_id[2] = cfg->sch[2].id;
		__entry->sch_prio_wfq[2] = cfg->sch[2].prio_wfq;
		__entry->sch_leaf[2] = cfg->sch[2].leaf;
		__entry->sch_arbi[2] = cfg->sch[2].arbi;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;
		),
	TP_printk("%s=%s %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n"
		"%*s: id: %d, prio_wfq: %d, leaf: %d, arbi: %d\n"
		"%*s: id: %d, prio_wfq: %d, leaf: %d, arbi: %d\n"
		"%*s: id: %d, prio_wfq: %d, leaf: %d, arbi: %d\n"
		"%*s=%s %s=%d\n",
		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,
		"dp_port", __entry->dp_port,
		"cqm_deq_port", __entry->cqm_deq_port,
		"q_id", __entry->q_id,
		"q_arbi", __entry->q_arbi,
		"q_prio_wfq", __entry->q_prio_wfq,
		"q_leaf", __entry->q_leaf,
		"n_sch_lvl", __entry->n_sch_lvl,
#if (DP_MAX_SCH_LVL != 3)
#error "__func__: __LINE__: Need tuning trace event here"
#endif
		__entry->indent, "sch[0]", __entry->sch_id[0],
			__entry->sch_prio_wfq[0], __entry->sch_leaf[0],
			__entry->sch_arbi[0],
		__entry->indent, "sch[1]", __entry->sch_id[1],
			__entry->sch_prio_wfq[1], __entry->sch_leaf[1],
			__entry->sch_arbi[1],
		__entry->indent, "sch[2]", __entry->sch_id[2],
			__entry->sch_prio_wfq[2], __entry->sch_leaf[2],
			__entry->sch_arbi[2],
		__entry->indent+2, "qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag)
);

TRACE_EVENT(dp_node_link_add,
	TP_PROTO(int res, struct dp_node_link *old_cfg, struct dp_node_link *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, old_cfg, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, indent)
		__field(int, res)
		__field(int, inst)
		__field(int, qos_flag)
		__field(int, flag)

		__field(int, dp_port)
		__field(int, node_type)
		__field(int, arbi)
		__field(int, prio_wfq)
		__field(int, leaf)
		__field(int, p_node_type)

		/*dp_node_id node_id*/
		__field(int, node_id)
		/*dp_node_id p_node_id*/
		__field(int, p_node_id)
		/*dp_node_id cqm_deq_port*/
		__field(int, cqm_dqp)

		/*old values*/
		__field(int, old_dp_port)
		__field(int, old_arbi)
		__field(int, old_prio_wfq)

		/*dp_node_id node_id*/
		__field(int, old_node_id)
		/*dp_node_id p_node_id*/
		__field(int, old_p_node_id)
		),
	TP_fast_assign(
		/*Unchanged variables*/
		__entry->indent = 71;
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->leaf = cfg->leaf;
		__entry->node_type = cfg->node_type;
		__entry->p_node_type = cfg->p_node_type;
		__entry->cqm_dqp = cfg->cqm_deq_port.q_id;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;

		/*Variables before change*/
		__entry->old_dp_port = old_cfg->dp_port;
		__entry->old_arbi = old_cfg->arbi;
		__entry->old_prio_wfq = old_cfg->prio_wfq;
		__entry->old_node_id = old_cfg->node_id.q_id;
		__entry->old_p_node_id = old_cfg->p_node_id.q_id;

		/*Variables after change*/
		__entry->dp_port = cfg->dp_port;
		__entry->arbi = cfg->arbi;
		__entry->prio_wfq = cfg->prio_wfq;
		__entry->node_id = cfg->node_id.q_id;
		__entry->p_node_id = cfg->p_node_id.q_id;
		),
	TP_printk(
		"%s=%s %s=%d %s=%d->%d %s=%d %s=%s %s=%d\n"
		"%*s=%s %s=%d->%d %s=%s->%s %s=%d->%d %s=%d\n"
		"%*s=%s %s=%d->%d\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,

		"dp_port", __entry->old_dp_port, __entry->dp_port,
		/*cqm_deq_port*/
		"cqm_deq_port",	__entry->cqm_dqp,
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag,
		__entry->indent+5,
		"node_type",
		P_NODE(__entry->node_type) ? "Port" :
			Q_NODE(__entry->node_type) ? "Queue" :
			S_NODE(__entry->node_type) ? "Sched" : "Unkwn",
		/*node_id*/
		"node_id", __entry->old_node_id, __entry->node_id,

		"arbi", dp_arbi_to_str(__entry->old_arbi),
			dp_arbi_to_str(__entry->arbi),
		"prio_wfq", __entry->old_prio_wfq, __entry->prio_wfq,
		"leaf", __entry->leaf,
		__entry->indent+5,
		"p_node_type",
		P_NODE(__entry->p_node_type) ? "Port " :
			Q_NODE(__entry->p_node_type) ? "Queue" :
			S_NODE(__entry->p_node_type) ? "Sched" : "Unkwn",
		/*p_node_id*/
		"p_node_id", __entry->old_p_node_id, __entry->p_node_id)
);

TRACE_EVENT(dp_node_link_get,
	TP_PROTO(int res, struct dp_node_link *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, indent)
		__field(int, res)
		__field(int, inst)

		__field(int, dp_port)
		__field(int, node_type)
		__field(int, arbi)
		__field(int, prio_wfq)
		__field(int, leaf)
		__field(int, p_node_type)

		/*dp_node_id node_id*/
		__field(int, node_id)
		/*dp_node_id p_node_id*/
		__field(int, p_node_id)
		/*dp_node_id cqm_deq_port*/
		__field(int, cqm_dqp)
		__field(int, qos_flag)
		__field(int, flag)
		),
	TP_fast_assign(
		__entry->indent = 71;
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->dp_port = cfg->dp_port;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;
		__entry->node_type = cfg->node_type;
		__entry->arbi = cfg->arbi;
		__entry->prio_wfq = cfg->prio_wfq;
		__entry->leaf = cfg->leaf;
		__entry->p_node_type = cfg->p_node_type;
		__entry->node_id = cfg->node_id.q_id;
		__entry->p_node_id = cfg->p_node_id.q_id;
		__entry->cqm_dqp = cfg->cqm_deq_port.q_id;
		),
	TP_printk(
		"%s=%s %s=%d %s=%d %s=%d %s=%s %s=%d\n"
		"%*s=%s %s=%d %s=%s %s=%d %s=%d\n"
		"%*s=%s, %s=%d\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,

		"dp_port", __entry->dp_port,
		/*cqm_deq_port*/
		"cqm_deq_port",	__entry->cqm_dqp,
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag,
		__entry->indent+5,
		"node_type",
		P_NODE(__entry->node_type) ? "Port" :
			Q_NODE(__entry->node_type) ? "Queue" :
			S_NODE(__entry->node_type) ? "Sched" : "Unkwn",
		/*node_id*/
		"node_id", __entry->node_id,
		"arbi", dp_arbi_to_str(__entry->arbi),
		"prio_wfq", __entry->prio_wfq,
		"leaf", __entry->leaf,
		__entry->indent+5,
		"p_node_type",
		P_NODE(__entry->p_node_type) ? "Port " :
			Q_NODE(__entry->p_node_type) ? "Queue" :
			S_NODE(__entry->p_node_type) ? "Sched" : "Unkwn",
		/*p_node_id*/
		"p_node_id", __entry->p_node_id)
);

TRACE_EVENT(dp_node_link_en_set,
	TP_PROTO(int res, struct dp_node_link_enable *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, res)
		__field(int, inst)
		__field(int, node_type)
		/*dp_node_id node_id*/
		__field(int, node_id)
		/*enable or disable*/
		__field(int, action)
		__field(int, qos_flag)
		__field(int, flag)
		),
	TP_fast_assign(
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->node_type = cfg->type;
		__entry->node_id = cfg->id.q_id;
		__entry->action = cfg->en;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;
		),
	TP_printk(
		"%s=%s %s=%d %s=%s %s=%d %s=%s %s=%s %s=%d\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,
		"node_type",
		P_NODE(__entry->node_type) ? "Port" :
			Q_NODE(__entry->node_type) ? "Queue" :
			S_NODE(__entry->node_type) ? "Sched" : "Unkwn",
		/*node_id*/
		"node_id", __entry->node_id,
		"action", (__entry->action & DP_NODE_DIS) ? "disable" :
			(__entry->action & DP_NODE_EN) ? "enable" :
			(__entry->action & DP_NODE_SUSPEND) ? "suspend" :
			(__entry->action & DP_NODE_RESUME) ? "resume" : "unkown",
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag)
);

TRACE_EVENT(dp_node_link_en_get,
	TP_PROTO(int res, struct dp_node_link_enable *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, res)
		__field(int, inst)
		__field(int, node_type)
		/*dp_node_id node_id*/
		__field(int, node_id)
		/*enable or disable*/
		__field(int, action)
		__field(int, qos_flag)
		__field(int, flag)
		),
	TP_fast_assign(
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->node_type = cfg->type;
		__entry->node_id = cfg->id.q_id;
		__entry->action = cfg->en;
		__entry->qos_flag = qos_flag;
		__entry->flag = flag;
		),
	TP_printk(
		"%s=%s %s=%d %s=%s %s=%d %s=%s %s=%s %s=%d\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,
		"node_type",
		P_NODE(__entry->node_type) ? "Port" :
			Q_NODE(__entry->node_type) ? "Queue" :
			S_NODE(__entry->node_type) ? "Sched" : "Unkwn",
		/*node_id*/
		"node_id", __entry->node_id,
		"action", (__entry->action & DP_NODE_DIS) ? "disable" :
			(__entry->action & DP_NODE_EN) ? "enable" :
			(__entry->action & DP_NODE_SUSPEND) ? "suspend" :
			(__entry->action & DP_NODE_RESUME) ? "resume" : "unkown",
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag)
);

TRACE_EVENT(dp_queue_map_set,
	TP_PROTO(int res, struct dp_queue_map_set *cfg,
		enum QOS_FLAG qos_flag, int flag),
	TP_ARGS(res, cfg, qos_flag, flag),
	TP_STRUCT__entry(
		__field(int, indent)
		__field(int, res)
		__field(int, inst)
		__field(int, qid)

		/*queue map*/
		__field(int, map_mpe1)
		__field(int, map_mpe2)
		__field(int, map_dp_port)
		__field(int, map_flowid)
		__field(int, map_subif)
		__field(int, map_dec)
		__field(int, map_enc)
		__field(int, map_class)
		__field(int, map_egflag)
		/*queue map mask(dont care)*/
		__field(int, mask_flowid)
		__field(int, mask_dec)
		__field(int, mask_enc)
		__field(int, mask_mpe1)
		__field(int, mask_mpe2)
		__field(int, mask_subif)
		__field(unsigned int, mask_subif_id_mask)
		__field(int, mask_dp_port)
		__field(int, mask_class)
		__field(int, mask_egflag)

		__field(int, qos_flag)
		__field(int, flag)
		),
	TP_fast_assign(
		__entry->indent = 71;
		__entry->res = res;
		__entry->inst = cfg->inst;
		__entry->qid = cfg->q_id;

		__entry->map_mpe1 = cfg->map.mpe1;
		__entry->map_mpe2 = cfg->map.mpe2;
		__entry->map_dp_port = cfg->map.dp_port;
		__entry->map_flowid = cfg->map.flowid;
		__entry->map_subif = cfg->map.subif;
		__entry->map_dec = cfg->map.dec;
		__entry->map_enc = cfg->map.enc;
		__entry->map_class = cfg->map.class;
		__entry->map_egflag = cfg->map.egflag;

		__entry->mask_mpe1 = cfg->mask.mpe1;
		__entry->mask_mpe2 = cfg->mask.mpe2;
		__entry->mask_dp_port = cfg->mask.dp_port;
		__entry->mask_flowid = cfg->mask.flowid;
		__entry->mask_subif = cfg->mask.subif;
		__entry->mask_subif_id_mask = cfg->mask.subif_id_mask;
		__entry->mask_dec = cfg->mask.dec;
		__entry->mask_enc = cfg->mask.enc;
		__entry->mask_class = cfg->mask.class;
		__entry->mask_egflag = cfg->mask.egflag;

		__entry->qos_flag = qos_flag;
		__entry->flag = flag;
		),
	TP_printk(
		"%s=%s %s=%d %s=%d %s=%s %s=%d\n"
		"%*s : %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n"
		"%*s: %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=0x%08x\n",

		"res", !__entry->res ? "ok":"fail",
		"inst", __entry->inst,
		"qid", __entry->qid,
		"qos_flag", dp_qos_flag_to_str(__entry->qos_flag),
		"flag", __entry->flag,

		__entry->indent, "map",
		"mpe1", __entry->map_mpe1,
		"mpe2", __entry->map_mpe2,
		"dp_port", __entry->map_dp_port,
		"flowid", __entry->map_flowid,
		"dec", __entry->map_dec,
		"enc", __entry->map_enc,
		"class", __entry->map_class,
		"egflag", __entry->map_egflag,
		"subif", __entry->map_subif,

		__entry->indent, "mask",
		"mpe1", __entry->mask_mpe1,
		"mpe2", __entry->mask_mpe2,
		"dp_port", __entry->mask_dp_port,
		"flowid", __entry->mask_flowid,
		"dec", __entry->mask_dec,
		"enc", __entry->mask_enc,
		"class", __entry->mask_class,
		"egflag", __entry->mask_egflag,
		"subif", __entry->mask_subif,
		"subif_id_mask", __entry->mask_subif_id_mask)
);

#endif /* DATAPATH_TRACE_H_ */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE datapath_trace

#include <trace/define_trace.h>
