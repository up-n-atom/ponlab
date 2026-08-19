// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include "datapath.h"

void print_cbm_alloc_data(char *str, struct cbm_dp_alloc_data *data)
{
	DP_DUMP("--------- %s: cbm_dp_alloc_data ------\n", str);
	DP_DUMP("   dp_inst              = %d\n", data->dp_inst);
	DP_DUMP("   cbm_inst             = %d\n", data->cbm_inst);
	DP_DUMP("   flags                = 0x%x\n", data->flags);
	DP_DUMP("   dp_port              = %d\n", data->dp_port);
	DP_DUMP("   deq_port_num         = %d\n", data->deq_port_num);
	DP_DUMP("   deq_port             = %d\n", data->deq_port);
	DP_DUMP("   dma_chan             = %08x\n", data->dma_chan);
	DP_DUMP("   tx_pkt_credit        = %d\n", data->tx_pkt_credit);
	DP_DUMP("   tx_b_credit          = %d\n", data->tx_b_credit);
	DP_DUMP("   txpush_addr_qos      = 0x%px\n",
				(void *)data->txpush_addr_qos);
	DP_DUMP("   txpush_addr          = 0x%px\n",
				(void *)data->txpush_addr);
	DP_DUMP("   tx_ring_size         = %d\n", data->tx_ring_size);
	DP_DUMP("   tx_ring_offset       = %d\n", data->tx_ring_offset);
	DP_DUMP("   tx_ring_addr_txpush  = 0x%px\n", data->tx_ring_addr_txpush);
	DP_DUMP("   num_dma_chan         = %d\n", data->num_dma_chan);
	DP_DUMP("   (dp_port_data *)data = 0x%px\n", data->data);
	DP_DUMP("\n");
}

void print_cbm_en_data(char *str, struct cbm_dp_en_data *data)
{
	DP_DUMP("------- %s: cbm_dp_en_data -------\n", str);
	DP_DUMP("   dp_inst               = %d\n", data->dp_inst);
	DP_DUMP("   cbm_inst              = %d\n", data->cbm_inst);
	DP_DUMP("   deq_port              = %d\n", data->deq_port);
	DP_DUMP("   num_deq_port          = %d\n", data->num_deq_port);
	DP_DUMP("   dma_chnl_init         = %d\n", data->dma_chnl_init);
	DP_DUMP("   f_policy              = %d\n", data->f_policy);
	DP_DUMP("   tx_policy_num         = %d\n", data->tx_policy_num);
	DP_DUMP("   tx_max_pkt_size       = %d\n", data->tx_max_pkt_size[0]);
	DP_DUMP("   tx_policy_base        = %d\n", data->tx_policy_base);
	DP_DUMP("   tx_ring_size          = %d\n", data->tx_ring_size);
	DP_DUMP("   rx_policy_num         = %d\n", data->rx_policy_num);
	DP_DUMP("   rx_policy_base        = %d\n", data->rx_policy_base);
	DP_DUMP("   num_dma_chan          = %d\n", data->num_dma_chan);
	DP_DUMP("   bm_policy_res_id      = %d\n", data->bm_policy_res_id);
	DP_DUMP("   (dp_subif_data *)data = 0x%px\n", data->data);
	DP_DUMP("\n");
}

int dump_ring_info(struct cbm_dp_alloc_complete_data *data)
{
	u8 cid, pid;
	int i, j;
	u16 nid;

	if (data->num_tx_ring || data->num_rx_ring)
		DP_INFO("\n-------------\n");
	for (i = 0; i < data->num_tx_ring; i++) {
		DP_DUMP("   DC TxRing: %d\n", i);

		if (data->tx_ring[i]) {
			DP_DUMP("      not_valid                   : %d\n",
				data->tx_ring[i]->not_valid);
			DP_DUMP("      TXIN  tx_deq_port           : %d\n",
				data->tx_ring[i]->tx_deq_port);
			DP_DUMP("      TXIN  DeqRingSize/paddr     : %d/0x%px\n",
				data->tx_ring[i]->in_deq_ring_size,
				data->tx_ring[i]->in_deq_paddr);
			DP_DUMP("      TXOUT FreeRingSize/paddr    : %d/0x%px\n",
				data->tx_ring[i]->out_free_ring_size,
				data->tx_ring[i]->out_free_paddr);
			DP_DUMP("      TXOUT PolicyBase/Poolid     : %d/%d\n",
				data->tx_ring[i]->txout_policy_base,
				data->tx_ring[i]->tx_poolid);
			DP_DUMP("      PolicyNum                   : %d\n",
				data->tx_ring[i]->policy_num);
			DP_DUMP("      NumOfTxPkt/TxPktSize        : %d/%d\n",
				data->tx_ring[i]->num_tx_pkt,
				data->tx_ring[i]->tx_pkt_size);
		}
	}

	for (i = 0; i < data->num_rx_ring; i++) {
		DP_DUMP("   DC RxRing: %d\n", i);

		if (data->rx_ring[i]) {
			DP_DUMP("      %s : %d/0x%px/%d\n",
				"RXOUT EnqRingSize/paddr/pid",
				data->rx_ring[i]->out_enq_ring_size,
				data->rx_ring[i]->out_enq_paddr,
				data->rx_ring[i]->out_enq_port_id);
			DP_DUMP("      %s            : %d\n",
				"RXOUT NumOfDmaCh",
				data->rx_ring[i]->num_out_tx_dma_ch);
			dp_dma_parse_id(data->rx_ring[i]->out_dma_ch_to_gswip,
					&cid, &pid, &nid);
			DP_DUMP("      %s    : %d/%d/%d\n",
				"RXOUT dma-ctrl/port/chan",
				cid, pid, nid);
			DP_DUMP("      RXOUT NumOfCqmDqPort/pid    : %d/%d\n",
				data->rx_ring[i]->num_out_cqm_deq_port,
				data->rx_ring[i]->out_cqm_deq_port_id);
			DP_DUMP("      RXOUT Poolid                : %d\n",
				data->rx_ring[i]->rx_poolid);
			DP_DUMP("      %s : %d/0x%px\n",
				"RXIN  InAllocRingSize/Paddr",
				data->rx_ring[i]->in_alloc_ring_size,
				data->rx_ring[i]->in_alloc_paddr);
			DP_DUMP("      %s   : %d/%d/%d\n",
				"NumPkt/Pktsize/Policybase",
				data->rx_ring[i]->num_pkt,
				data->rx_ring[i]->rx_pkt_size,
				data->rx_ring[i]->rx_policy_base);
			DP_DUMP("      %s       : %d/0x%px\n",
				"PreFillPktNum/PktBase",
				data->rx_ring[i]->prefill_pkt_num,
				data->rx_ring[i]->pkt_base_paddr);

			for (j = 0; j < data->rx_ring[i]->num_egp; j++) {
				DP_DUMP("      qos_egp[%d] deq_port: %d\n",
					j, data->rx_ring[i]->egp[j].deq_port);
				DP_DUMP("                  txpush_addr    : 0x%px\n",
					data->rx_ring[i]->egp[j].txpush_addr);
				DP_DUMP("                  txpush_addr_qos: 0x%px\n",
					data->rx_ring[i]->egp[j].txpush_addr_qos);
				DP_DUMP("                  tx_pkt_credit  : %d\n",
					data->rx_ring[i]->egp[j].tx_pkt_credit);
			}
		}
	}

	return 0;
}

void print_cqm_alloc_comp_data(
	struct cbm_dp_alloc_complete_data *data,
	char *s)
{
	DP_DUMP("--------- %s: cbm_dp_alloc_complete_data ------\n", s);
	DP_DUMP("   num_rx_ring             = %d\n", data->num_rx_ring);
	DP_DUMP("   num_tx_ring             = %d\n", data->num_tx_ring);
	DP_DUMP("   num_umt_port            = %d\n", data->num_umt_port);
	DP_DUMP("   enable_cqm_meta         = %d\n", data->enable_cqm_meta);
	DP_DUMP("   qid_base                = %d\n", data->qid_base);
	DP_DUMP("   num_qid                 = %d\n", data->num_qid);
	DP_DUMP("   bm_policy_res_id        = %u\n", data->bm_policy_res_id);
	DP_DUMP("   pcidata                 = 0x%px\n",
		data->opt_param.pcidata);
	DP_DUMP("   tx_policy_base          = %d\n", data->tx_policy_base);
	DP_DUMP("   tx_policy_num           = %d\n", data->tx_policy_num);
	DP_DUMP("   gpid_info.f_min_pkt_len = %d\n",
		data->gpid_info.f_min_pkt_len);
	DP_DUMP("   gpid_info.seg_en        = %d\n", data->gpid_info.seg_en);
	DP_DUMP("   gpid_info.min_pkt_len   = %d\n",
		data->gpid_info.min_pkt_len);
	DP_DUMP("   (dp_dev_data *)data     = 0x%px\n", data->data);

	dump_ring_info(data);
	DP_DUMP("\n");
}

void print_cqm_deq_res(cbm_dq_port_res_t *res)
{
	int i = 0;
	cbm_dq_info_t	*deq_info;

	DP_DUMP("===========================\n");
	DP_DUMP("cbm_buf_free_base      = 0x%px\n", res->cbm_buf_free_base);
	DP_DUMP("num_free_entries       = %d\n", res->num_free_entries);
	DP_DUMP("num_deq_ports          = %d\n", res->num_deq_ports);

	for (i = 0; i < res->num_deq_ports; i++) {
		deq_info = &res->deq_info[i];
		DP_DUMP("deq port               = %d\n", i);
		DP_DUMP("port_no                = %d\n", deq_info->port_no);
		DP_DUMP("cbm_dq_port_base       = 0x%px\n",
			deq_info->cbm_dq_port_base);
		DP_DUMP("dma_tx_chan            = %d\n", deq_info->dma_tx_chan);
		DP_DUMP("num_desc               = %d\n", deq_info->num_desc);
		DP_DUMP("num_free_burst         = %d\n",
			deq_info->num_free_burst);
	}
	DP_DUMP("\n\n");
}

int dp_dealloc_cqm_port(struct module *owner, u32 dev_port,
			struct pmac_port_info *port,
			struct cbm_dp_alloc_data *data, u32 flags)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		DP_DUMP("before cbm_dp_port_dealloc: %s:0x%px, %s:%d, %s:%d, %s:0x%x\n",
			"owner", owner,
			"dev_port", dev_port,
			"cbm_port_id", port->port_id,
			"flags", flags | DP_F_DEREGISTER);
		print_cbm_alloc_data("Before cbm_dp_port_dealloc() call", data);
	}
#endif

	if (CBM_OPS(data->dp_inst, cbm_dp_port_dealloc, owner, dev_port, port->port_id,
		    data, flags | DP_F_DEREGISTER)) {
		pr_err("DPM: %s: %s:0x%px, %s:%d, %s:%d, %s:0x%x\n",
		       "cbm_dp_port_dealloc Failed",
		       "owner", owner,
		       "dev_port", dev_port,
		       "cbm_port_id", port->port_id,
		       "flags", flags | DP_F_DEREGISTER);
		print_cbm_alloc_data("Failed cbm_dp_port_dealloc() call", data);
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		print_cbm_alloc_data("After cbm_dp_port_dealloc() call", data);
		DP_DUMP("DPM: %s: Successful\n", __func__);
	}
#endif

	return DP_SUCCESS;
}

int dp_alloc_cqm_port(struct module *owner, struct net_device *dev,
		      u32 dev_port, s32 port_id,
		      struct cbm_dp_alloc_data *cbm_data, u32 flags)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		DP_DUMP("%s: %s=0x%px, %s=%s, %s=%d, %s=%d, %s=0x%x\n",
			__func__,
			"owner", owner,
			"dev_name", dev ? dev->name : "NULL",
			"dev_port", dev_port,
			"dp_port", port_id,
			"flags", flags);
		print_cbm_alloc_data("Before cbm_dp_port_alloc() call", cbm_data);
	}
#endif
	if (CBM_OPS(cbm_data->dp_inst, cbm_dp_port_alloc, owner, dev, dev_port,
		    port_id, cbm_data, flags)) {
		pr_err("DPM: %s failed: %s:0x%px, %s:%s, %s:%d, %s:%d, %s:0x%x\n",
			__func__,
			"owner", owner,
			"dev_name", dev ? dev->name : "NULL",
			"dev_port", dev_port,
			"cbm_port_id", port_id,
			"flags", flags);
		print_cbm_alloc_data("Failed cbm_dp_port_alloc() call", cbm_data);
		return DP_FAILURE;
	}

	if (!(cbm_data->flags & CBM_PORT_DP_SET) &&
	    !(cbm_data->flags & CBM_PORT_DQ_SET)) {
		pr_err("DPM: %s failed, NO DP_SET/DQ_SET(0x%x):%s/dev_port %d\n",
		       "cbm_dp_port_alloc",
		       cbm_data->flags,
		       owner->name, dev_port);
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		print_cbm_alloc_data("After cbm_dp_port_alloc() call", cbm_data);
		DP_DUMP("DPM: %s: Successful\n", __func__);
	}
#endif

	return DP_SUCCESS;
}

int dp_enable_cqm_port(struct module *owner, struct pmac_port_info *port,
		       struct cbm_dp_en_data *data, u32 flags)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		DP_INFO("%s:0x%px, %s:%d, %s:0x%x, %s:0x%x\n",
			"owner", owner,
			"dp_port", port->port_id,
			"flags", flags,
			"alloc_flags", port->alloc_flags);
		print_cbm_en_data("Before cbm_dp_enable() call", data);
	}
#endif

	if (CBM_OPS(data->dp_inst, cbm_dp_enable, owner, port->port_id, data,
				flags)) {
		pr_err("DPM: %s, %s:0x%px, %s:%d, %s:0x%x, %s:0x%x\n",
		       "cbm_dp_enable Failed",
		       "owner", owner,
		       "dp_port", port->port_id,
		       "flags", flags,
		       "alloc_flags", port->alloc_flags);
		print_cbm_en_data("Failed cbm_dp_enable() call", data);
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		print_cbm_en_data("After cbm_dp_enable() call", data);
		DP_DUMP("DPM: %s: Successful\n", __func__);
	}
#endif

	return DP_SUCCESS;
}

int dp_cqm_port_alloc_complete(struct module *owner,
			       struct pmac_port_info *port, s32 dp_port,
			       struct cbm_dp_alloc_complete_data *data,
			       u32 flags)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		DP_DUMP("%s: %s:0x%px %s:%s, %s:%d, %s:%d, %s:0x%x, %s:0x%x\n",
			__func__,
			"owner", owner,
			"dev_name", port->dev ? port->dev->name : "NULL",
			"dev_port", port->dev_port,
			"dp_port", dp_port,
			"flags", flags,
			"alloc_flags", port->alloc_flags);
		print_cqm_alloc_comp_data(data,
			"Before cbm_dp_port_alloc_complete() call");
	}
#endif

	if (CBM_OPS(port->inst, cbm_dp_port_alloc_complete, owner, port->dev,
		    port->dev_port, dp_port, data, port->alloc_flags | flags)) {
		pr_err("DPM: %s: %s:0x%px, %s:%s, %s:%d, %s:%d, %s:0x%x, %s:0x%x\n",
		       "cbm_dp_port_alloc_complete Failed",
		       "owner", owner,
		       "dev_name", port->dev ? port->dev->name : "NULL",
		       "dev_port", port->dev_port,
		       "dp_port", dp_port,
			"flags", flags,
		       "alloc_flags", port->alloc_flags);
		print_cqm_alloc_comp_data(data, "Failed cbm_dp_port_alloc_complete() call");
		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		print_cqm_alloc_comp_data(data, "After  cbm_dp_port_alloc_complete() call");
		DP_DUMP("DPM: %s: Successful\n", __func__);
	}
#endif

	return DP_SUCCESS;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
int dp_cqm_gpid_lpid_map(int inst, struct cbm_gpid_lpid *map)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG) {
		DP_DUMP("DPM: %s: %s:%d, %s (%2d) <-> %s (%2d) %s:0x%x\n",
			__func__,
			"cbm_inst", map->cbm_inst,
			"gpid", map->gpid,
			"lpid", map->lpid,
			"flag", map->flag);
	}
#endif
	if (CBM_OPS(inst, cbm_gpid_lpid_map, map)) {
		pr_err("DPM: %s: %s:%d, %s (%d) <-> %s (%d) %s:0x%x\n",
		       "cbm_gpid_lpid_map Failed",
		       "cbm_inst", map->cbm_inst,
		       "gpid", map->gpid,
		       "lpid", map->lpid,
		       "flag", map->flag);

		return DP_FAILURE;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_REG)
		DP_DUMP("%27s%s\n", "", "---> Successfully mapped");
#endif

	return DP_SUCCESS;
}
#endif
