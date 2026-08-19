/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP micro-controllers Driver
 */

#define pr_fmt(fmt) "[PP_UC]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/sort.h>
#include <linux/pp_api.h>

#include "pp_regs.h"
#include "pp_common.h"
#include "pp_dma.h"
#include "infra.h"
#include "classifier.h"
#include "checker.h"
#include "rx_dma.h"
#include "uc.h"
#include "uc_regs.h"
#include "uc_dut.h"
#include "uc_internal.h"
#include <soc/mxl/mxl_sec.h>

struct logger_buff {
	void *virt;
	dma_addr_t phys;
	size_t sz;
};

#define UC_FAT_ENT_MSK   GENMASK_ULL(31, 26)
#define UC_FAT_LSB_MSK   GENMASK_ULL(25, 0)
#define UC_FAT_MSB_MSK   GENMASK_ULL(35, 26)
#define UC_FAT_NUM_BITS  HWEIGHT64(UC_FAT_MSB_MSK)
struct fat_tbl {
	u16 fat[UC_FAT_ALL_ENT_CNT]; /*! the entries values */
	ulong bmap[BITS_TO_LONGS(UC_FAT_ALL_ENT_CNT)]; /*! used entries */
	u64 msb_msk; /*! extract MSB value from an address
		      *  this value eventually gets configured in the FAT
		      */
	u64 lsb_msk; /*! extract LSB value from an address
		         the LSB is used in the translation
		      */
	u64 ent_msk; /*! extract entry id from an address */
	u32 win_sz; /*! entry window size */
	u32 sz; /*! number of entries in the table */
};

#define UC_SRAM_BASE (0xE4000000)
#define UC_CODE_START_OFFSET (0)
#define UC_INIT_CLUSTER_ONCE (0x100)
/**
 * @define boot done value is written by the uc
 */
#define UC_BOOT_DONE_VAL   0x20
/**
 * @define ccu done value is written by the uc
 */
#define UC_CCU_DONE_VAL    1
/**
 * @define turn on value
 */
#define UC_TURN_ON_VAL     1

/**
 * @brief egress uc HW base address
 */
u64 uc_egr_base_addr;
u64 uc_egr_cpus_addr;
#define UC_EGR_CPUS_OFF 0x200000

/**
 * @brief ingress uc HW base address
 */
u64 uc_ing_base_addr;
u64 uc_ing_cpus_addr;
#define UC_ING_CPUS_OFF 0x200000

/**
 * @brief Fat tables
 */
struct fat_tbl uc_fat;

/**
 * @brief TX-Manager credit base address (phy)
 */
static u32 txm_cred_base;

/**
 * @brief checker physical base address
 */
static phys_addr_t chk_base_addr;

/**
 * @brief classifier physical base address
 */
static phys_addr_t cls_base_addr;

/**
 * @brief wred_cfg physical base address
 */
static phys_addr_t wred_cfg_base_addr;

/**
 * @brief misc physical base address
 */
static phys_addr_t qos_misc_base_addr;

/**
 * @brief uc main logger buffer, this is splitted into 2 buffers
 *        for ingress and egress uCs
 */
struct logger_buff logger_buf;

/**
 * @brief Egress UC logger info
 */
struct logger_buff egr_log_buf;

/**
 * @brief Ingress UC logger info
 */
struct logger_buff ing_log_buf;

/**
 * @define base address for the tx manager add packet credit
 *         register per port.
 */
#define UC_TXMGR_CRED_INC_ADDR(p) (txm_cred_base + 8 + ((p) << 4))

/**
 * @brief valid bitmap for the egress cluster, pit per cpu
 */
static ulong eg_cpu_valid_bmap;
/**
 * @brief active bitmap for the egress cluster, pit per cpu
 */
ulong eg_cpu_act_bmap;

/**
 * @brief valid bitmap for the ingress cluster, pit per cpu
 */
static ulong in_cpu_valid_bmap;

/**
 * @brief active bitmap for the ingress cluster, pit per cpu
 */
ulong in_cpu_act_bmap;

u32 ing_addr_offset;
u32 ing_ldb1_offset;
u32 ing_ldb2_offset;
u32 uc_error_off;

struct uc_error_info_t {
	u32 dump_is_ready;	//indicates dump is ready. 0 be default.
	u32 vector_num;
	u32 exception_return_address;
	u32 exception_return_branch_target_address;
	u32 exception_return_status;
	u32 exception_cause_reg;
	u32 exception_fault_address;
	u32 cause_tid;	    //tid of the task that raised the exception
	u32 job_base;
	u32 stack_base;
	u32 stack_top;
	u32 status32;
};

static inline void __uc_cpu_active_set(bool uc_is_egr, u32 cid, bool enable);

bool uc_is_cluster_valid(bool uc_is_egr)
{
	ulong valid_bmap = uc_is_egr ? eg_cpu_valid_bmap : in_cpu_valid_bmap;

	if (unlikely(!valid_bmap)) {
		pr_err("invalid cluster %s\n",
		       uc_is_egr ? "EGRESS" : "INGRESS");
		return false;
	}

	return true;
}

bool uc_is_cpu_valid(bool uc_is_egr, u32 cid)
{
	ulong valid_bmap = uc_is_egr ? eg_cpu_valid_bmap : in_cpu_valid_bmap;

	if (unlikely(cid >= (BITS_PER_BYTE * sizeof(valid_bmap)) - 1)) {
		pr_err("invalid uc cpu id %u\n", cid);
		return false;
	}

	if (likely(valid_bmap & BIT(cid)))
		return true;

	pr_err("invalid %s cpu %u\n", uc_is_egr ? "EGRESS" : "INGRESS", cid);
	return false;
}

/**
 * @brief Test whether a task id is valid or not
 * @param uc_is_egr select cluster
 * @param tid task id
 * @return bool true in case it is valid, false otherwise
 */
static bool uc_is_task_id_valid(bool uc_is_egr, u32 tid)
{
	u32 num_tasks = uc_is_egr ? UC_EGR_MAXTASK : UC_ING_MAXTASK;

	if (tid < num_tasks)
		return true;

	pr_err("invalid %s task id %u\n", uc_is_egr ? "EGRESS" : "INGRESS",
	       tid);
	return false;
}

/**
 * @brief Test whether egress port id is valid or not
 * @param port the port id
 * @return bool true in case it is valid, false otherwise
 */
static bool uc_is_egr_port_valid(u32 port)
{
	if (port < UC_EGR_MAX_PORT)
		return true;

	pr_err("invalid egress port id %u\n", port);
	return false;
}

s32 uc_egr_writer_port_en_get(u32 port, bool *en)
{
	u64 addr;

	if (unlikely(ptr_is_null(en)))
		return -EINVAL;

	if (!uc_is_egr_port_valid(port))
		return -EINVAL;

	addr = UC_EGR_B_WRITER_PORT_ENABLE(port + UC_B_TXMGR_PORT_OFF);

	*en = !!PP_REG_RD32(addr);

	return 0;
}

s32 uc_egr_writer_map_get(u32 cid, u32 port, u32 *queue)
{
	u64 addr;

	if (unlikely(ptr_is_null(queue)))
		return -EINVAL;

	if (!uc_is_cpu_valid(UC_IS_EGR, cid) || !uc_is_egr_port_valid(port))
		return -EINVAL;

	addr = UC_EGR_B_WRITER_MAP(cid, port + UC_B_TXMGR_PORT_OFF);

	*queue = PP_REG_RD32(addr);

	return 0;
}

s32 uc_egr_writer_queue_credit_get(u32 cid, u32 port, u32 *credit)
{
	u64 addr;

	if (unlikely(ptr_is_null(credit)))
		return -EINVAL;

	if (!uc_is_cpu_valid(UC_IS_EGR, cid) || !uc_is_egr_port_valid(port))
		return -EINVAL;

	addr = UC_EGR_B_WRITER_QUEUE_CREDIT(cid, port + UC_B_TXMGR_PORT_OFF);

	*credit = PP_REG_RD32(addr);

	return 0;
}

s32 uc_egr_writer_port_credit_get(u32 port, u32 *credit)
{
	u64 addr;

	if (unlikely(ptr_is_null(credit)))
		return -EINVAL;

	if (!uc_is_egr_port_valid(port))
		return -EINVAL;

	addr = UC_EGR_B_WRITER_PORT_CREDIT(port + UC_B_TXMGR_PORT_OFF);

	*credit = PP_REG_RD32(addr);

	return 0;
}

void uc_stats_reset(void)
{
	PP_REG_WR32(UC_SR(EGRESS, UC_SR_RESET_OFF), 1);
}

s32 uc_reass_cpu_stats_get(u32 cid, struct reassembly_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	if (!uc_is_cpu_valid(UC_IS_EGR, cid))
		return -EINVAL;

	off = offsetof(struct egress_stats, cpu[cid].reass);

	PP_REG_RD_REP(UC_SR(EGRESS, off), stats, sizeof(*stats));

	return 0;
}

s32 uc_egr_global_stats_get(struct egr_glb_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, glb);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_mcast_stats_get(struct mcast_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, mcast);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_tdox_stats_get(struct tdox_uc_stats *stats)
{
	s32 ret = 0;

	ret = uc_egr_mbox_cmd_send(UC_CMD_TDOX_STATS, 0, NULL, 0,
				stats, sizeof(struct tdox_uc_stats));
	if (unlikely(ret)) {
		pr_err("failed to get tdox counters\n");
		return ret;
	}

	return 0;
}

s32 uc_tdox_stats_reset(void)
{
	s32 ret = 0;
	struct tdox_uc_stats stats;

	ret = uc_egr_mbox_cmd_send(UC_CMD_TDOX_STATS, true, NULL, 0, &stats,
				   sizeof(struct tdox_uc_stats));
	if (unlikely(ret)) {
		pr_err("failed to get tdox counters\n");
		return ret;
	}

	return 0;
}

s32 uc_lld_ctx_set(struct lld_ctx_cfg *cfg)
{
	s32 ret = 0;

	ret = uc_egr_mbox_cmd_send(UC_CMD_LLD_CTX_CFG_SET, 0, cfg,
				sizeof(struct lld_ctx_cfg), NULL, 0);
	if (unlikely(ret)) {
		pr_err("failed to set LLD Context\n");
		return ret;
	}

	return 0;
}

s32 uc_aqm_ctx_set(struct aqm_ctx_cfg *cfg)
{
	s32 ret = 0;

	ret = uc_egr_mbox_cmd_send(UC_CMD_AQM_CTX_CFG_SET, 0, cfg,
				   sizeof(struct aqm_ctx_cfg), NULL, 0);
	if (unlikely(ret)) {
		pr_err("failed to set AQM Context\n");
		return ret;
	}

	return 0;
}

s32 uc_sf_hist_get(u8 ctx, struct pp_sf_hist_stat *hist, bool reset)
{
	s32 ret = 0;
	struct aqm_lld_hist_get_out_cmd uc_hist;
	struct aqm_lld_hist_get_in_cmd in_cmd;

	in_cmd.ctx = ctx;
	in_cmd.reset = reset;

	ret = uc_egr_mbox_cmd_send(UC_CMD_AQM_LLD_HIST_GET, 0, &in_cmd,
							sizeof(struct aqm_lld_hist_get_in_cmd), &uc_hist,
							sizeof(struct aqm_lld_hist_get_out_cmd));
	if (unlikely(ret)) {
		pr_err("failed to get LLD Histogram\n");
		return ret;
	}

	hist->num_hist_bins = uc_hist.num_hist_bins;
	memcpy(hist->hist_counter, uc_hist.hist_counter,
		sizeof(hist->hist_counter));
	hist->hist_updates = uc_hist.hist_updates;
	hist->max_latency = uc_hist.max_latency;

	return 0;
}

s32 uc_lld_allowed_aq_set(struct lld_allowed_aq_set_cmd *cmd)
{
	s32 ret = 0;

	ret = uc_egr_mbox_cmd_send(UC_CMD_LLD_ALLOWED_AQ_SET, 0, cmd,
				sizeof(struct lld_allowed_aq_set_cmd), NULL,
				0);
	if (unlikely(ret)) {
		pr_err("failed to set LLD allowed AQ\n");
		return ret;
	}

	return 0;
}

s32 uc_ingress_stats_get(struct ing_stats *stats)
{
	u64 addr;
	u32 cid;
	struct ing_stats lo_stats;

	for (cid = 0; cid < UC_CPUS_MAX; cid++) {
		if (in_cpu_act_bmap & BIT(cid)) {
			addr = DCCM_INGRESS_ADDR(cid, ING_STATS_BASE);
			memcpy_fromio(&lo_stats, (void *)addr, sizeof(lo_stats));
			stats->rx_checker += lo_stats.rx_checker;
			stats->rx_parser += lo_stats.rx_parser;
			stats->rx_host += lo_stats.rx_host;
			stats->rx_frag += lo_stats.rx_frag;
			stats->rx_frag_eg += lo_stats.rx_frag_eg;
			stats->rx_wl += lo_stats.rx_wl;
			stats->rx_lro += lo_stats.rx_lro;
		}
	}

	return 0;
}

s32 uc_ingress_stats_reset(void)
{
	u64 addr;
	u32 cid;

	for (cid = 0; cid < UC_CPUS_MAX; cid++) {
		if (in_cpu_act_bmap & BIT(cid)) {
			addr = DCCM_INGRESS_ADDR(cid, ING_STATS_BASE);
			memset_io((void *)addr, 0, sizeof(struct ing_stats));
		}
	}

	return 0;
}

s32 uc_lld_total_stats_get(struct aqm_lld_global_stats *stats)
{
	struct aqm_lld_stats aqm_lld_stats;
	u32 lld_ctx;
	u32 off;
	u32 sf_indx;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, aqm_lld);
	memcpy_fromio(&aqm_lld_stats, (void *)UC_SR(EGRESS, off),
				sizeof(aqm_lld_stats));

	memset(stats, 0, sizeof(struct aqm_lld_global_stats));
	stats->rx_pkt        = aqm_lld_stats.rx_pkt;
	stats->ctx_error_pkt = aqm_lld_stats.ctx_error_pkt;
	stats->error_pkt = aqm_lld_stats.error_pkt;

	/* LLD stats per SF */
	for (lld_ctx= 0; lld_ctx < MAX_LLD_CONTEXT; lld_ctx++) {
		stats->lld_aggr.rx_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].rx_pkt;
		stats->lld_aggr.rx_ect0_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].rx_ect0_pkt;
		stats->lld_aggr.rx_ect1_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].rx_ect1_pkt;
		stats->lld_aggr.rx_ce_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].rx_ce_pkt;
		stats->lld_aggr.tx_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].tx_pkt;
		stats->lld_aggr.mark_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].mark_pkt;
		stats->lld_aggr.sanction_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].sanction_pkt;
		stats->lld_aggr.drop_pkt +=
			aqm_lld_stats.lld_sf[lld_ctx].drop_pkt;
	}

	/* Buff ctrl stats per SF */
	for (sf_indx = 0; sf_indx < MAX_AQM_CONTEXT; sf_indx++) {
		stats->aqm_sw_aggr.rx_pkt +=
			aqm_lld_stats.aqm_sf[sf_indx].rx_pkt;
		stats->aqm_sw_aggr.bc_drop_pkt +=
			aqm_lld_stats.aqm_sf[sf_indx].bc_drop_pkt;
		stats->aqm_sw_aggr.aqm_drop_pkt +=
			aqm_lld_stats.aqm_sf[sf_indx].aqm_drop_pkt;
		stats->aqm_sw_aggr.tx_pkt +=
			aqm_lld_stats.aqm_sf[sf_indx].tx_pkt;
	}

	return 0;
}

s32 uc_lld_per_sf_stats_get(u8 lld_ctx, struct lld_sf_stats *sf_stats)
{
	struct aqm_lld_stats stats;
	u32 off;

	if (unlikely(ptr_is_null(sf_stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, aqm_lld);

	memcpy_fromio(&stats, (void *)UC_SR(EGRESS, off), sizeof(stats));

	memcpy(sf_stats, &stats.lld_sf[lld_ctx], sizeof(*sf_stats));

	return 0;
}

s32 uc_aqm_sw_per_sf_stats_get(u8 sf_indx, struct aqm_sw_sf_stats *sf_stats)
{
	struct aqm_lld_stats stats;
	u32 off;

	if (unlikely(ptr_is_null(sf_stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, aqm_lld);

	memcpy_fromio(&stats, (void *)UC_SR(EGRESS, off), sizeof(stats));

	memcpy(sf_stats, &stats.aqm_sf[sf_indx], sizeof(*sf_stats));

	return 0;
}

s32 uc_reass_stats_get(struct reassembly_stats *stats)
{
	struct reassembly_stats cpu_stats;
	s32 ret;
	u32 i;
	u64 *it1, *it2;

	for (i = 0; i < UC_CPUS_REASS_MAX; i++) {
		if (!uc_is_cpu_active(UC_IS_EGR, i))
			continue;

		ret = uc_reass_cpu_stats_get(i, &cpu_stats);
		if (unlikely(ret)) {
			pr_err("failed to get reassembly uc cpu %u counters\n",
			       i);
			return ret;
		}

		/* add all counters to stats */
		it2 = (u64 *)(&cpu_stats);
		for_each_struct_mem(stats, it1, it2++)
			*it1 += *it2;
	}


	return 0;
}

s32 uc_frag_stats_get(struct frag_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, frag);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_remark_stats_get(struct remarking_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, remark);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_lro_stats_get(struct lro_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, lro);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_ipsec_stats_get(struct ipsec_stats *stats)
{
	u32 off;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	off = offsetof(struct egress_stats, ipsec);

	memcpy_fromio(stats, (void *)UC_SR(EGRESS, off), sizeof(*stats));

	return 0;
}

s32 uc_task_pc_get(bool uc_is_egr, u32 cid, u32 tid, u32 *pc)
{
	u64 addr;

	if (ptr_is_null(pc))
		return -EINVAL;

	if (!uc_is_cpu_active(uc_is_egr, cid))
		return -EINVAL;

	if (!uc_is_task_id_valid(uc_is_egr, tid))
		return -EINVAL;

	if (uc_is_egr)
		addr = UC_B_AUX_PC_REG(EGRESS, cid, tid);
	else
		addr = UC_B_AUX_PC_REG(INGRESS, cid, tid);

	*pc = PP_REG_RD32(addr);

	return 0;
}

s32 uc_task_status32_get(bool uc_is_egr, u32 cid, u32 tid, u32 *status)
{
	u64 addr;

	if (ptr_is_null(status))
		return -EINVAL;

	if (!uc_is_cpu_active(uc_is_egr, cid))
		return -EINVAL;

	if (!uc_is_task_id_valid(uc_is_egr, tid))
		return -EINVAL;

	if (uc_is_egr)
		addr = UC_B_AUX_S32_REG(EGRESS, cid, tid);
	else
		addr = UC_B_AUX_S32_REG(INGRESS, cid, tid);

	*status = PP_REG_RD32(addr);

	return 0;
}

s32 uc_task_ready_get(bool uc_is_egr, u32 cid, u32 tid, bool *ready)
{
	u64 addr;

	if (ptr_is_null(ready))
		return -EINVAL;

	if (!uc_is_cpu_active(uc_is_egr, cid))
		return -EINVAL;

	if (!uc_is_task_id_valid(uc_is_egr, tid))
		return -EINVAL;

	if (uc_is_egr)
		addr = UC_MTSR_READY_REG(EGRESS, cid, tid);
	else
		addr = UC_MTSR_READY_REG(INGRESS, cid, tid);

	*ready = !!PP_REG_RD32(addr);

	return 0;
}

s32 uc_task_popa_active_get(bool uc_is_egr, u32 cid, u32 tid, bool *active,
			    u32 *queue)
{
	u32 num_queues, i;
	ulong active_map;
	u64 addr;

	if (ptr_is_null(active) || ptr_is_null(queue))
		return -EINVAL;

	if (!uc_is_cpu_active(uc_is_egr, cid))
		return -EINVAL;

	if (!uc_is_task_id_valid(uc_is_egr, tid))
		return -EINVAL;

	if (uc_is_egr)
		num_queues = UC_EGR_MAXQUEUE;
	else
		num_queues = UC_ING_MAXQUEUE;

	*active = false;
	for (i = 0; i < num_queues; i++) {
		if (uc_is_egr)
			addr = UC_B_POPA_ACT_LO_REG(EGRESS, cid, i);
		else
			addr = UC_B_POPA_ACT_LO_REG(INGRESS, cid, i);

		active_map = (ulong)PP_REG_RD32(addr);

		if (!test_bit(tid, &active_map))
			continue;

		if (!*active) {
			*active = true;
			*queue = i;
		} else {
			pr_debug("%s task %u.%u is already active on queue %u\n",
				 uc_is_egr ? "EGRESS" : "INGRESS", cid, tid,
				 *queue);
		}
	}

	return 0;
}

s32 uc_task_regs_get(bool uc_is_egr, u32 cid, u32 tid, u32 *regs, u32 num)
{
	u64 addr;
	u32 i;

	if (ptr_is_null(regs))
		return -EINVAL;

	if (!uc_is_cpu_active(uc_is_egr, cid))
		return -EINVAL;

	if (!uc_is_task_id_valid(uc_is_egr, tid))
		return -EINVAL;

	for (i = 0; i < min(num, UC_TASK_REGS_NUM); i++) {
		if (uc_is_egr)
			addr = UC_B_PRIV_GP_REG(EGRESS, cid, i, tid);
		else
			addr = UC_B_PRIV_GP_REG(INGRESS, cid, i, tid);

		regs[i] = PP_REG_RD32(addr);
	}

	return 0;
}

bool uc_is_cpu_active(bool uc_is_egr, u32 cid)
{
	ulong act_bmap = uc_is_egr ? eg_cpu_act_bmap : in_cpu_act_bmap;

	if (!uc_is_cpu_valid(uc_is_egr, cid))
		return false;

	if (likely(act_bmap & BIT(cid)))
		return true;

	pr_err("inactive %s cpu %u\n", uc_is_egr ? "EGRESS" : "INGRESS", cid);
	return false;
}

static void __uc_egr_reader_wred_addr_set(u32 addr)
{
	u64 reg;

	reg = UC_B_READER_WRED_BASE(EGRESS);

	PP_REG_WR32(reg, uc_fat_addr_rev_trans(UC_IS_EGR, addr));
}

static void __uc_egr_reader_client_addr_set(u32 addr)
{
	u64 reg;

	reg = UC_B_READER_CLIENT_BASE(EGRESS);

	PP_REG_WR32(reg, addr >> 20);
}

static void __uc_egr_reader_qm_addr_set(u32 addr)
{
	u64 reg;

	reg = UC_B_READER_QMNGR_BASE(EGRESS);

	PP_REG_WR32(reg, uc_fat_addr_rev_trans(UC_IS_EGR, addr));
}

static void __uc_egr_reader_bm_addr_set(u32 addr)
{
	u64 reg;
	u32 val = PP_FIELD_PREP(UC_READER_BMNGR_BASE_MSK,
				(uc_fat_addr_rev_trans(UC_IS_EGR, addr)) >> 16);

	reg = UC_B_READER_BMNGR_BASE(EGRESS);

	PP_REG_WR32(reg, val);
}

s32 uc_ccu_maxcpus_get(bool uc_is_egr, u8 *max_cpus)
{
	u64 reg;

	if (unlikely(ptr_is_null(max_cpus)))
		return -EINVAL;

	if (uc_is_egr)
		reg = PP_REG_RD32(UC_B_CCU_MAXCPU_REG(EGRESS));
	else
		reg = PP_REG_RD32(UC_B_CCU_MAXCPU_REG(INGRESS));

	*max_cpus = PP_FIELD_GET(UC_CCU_MAXCPU_MSK, reg);

	return 0;
}

s32 uc_ccu_gpreg_get(bool uc_is_egr, u32 idx, u32 *gpreg)
{
	u64 reg;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	if (unlikely(ptr_is_null(gpreg)))
		return -EINVAL;

	if (unlikely(idx >= UC_CCU_GPREG_MAX)) {
		pr_err("invalid %s gpreg index %u, max=%u\n",
		       uc_is_egr ? "EGRESS" : "INGRESS", idx, UC_CCU_GPREG_MAX);
		return -EINVAL;
	}

	/* read ccu gpreg */
	if (uc_is_egr)
		reg = UC_B_CCU_GPREG_REG(EGRESS, idx);
	else
		reg = UC_B_CCU_GPREG_REG(INGRESS, idx);

	*gpreg = PP_REG_RD32(reg);

	return 0;
}

s32 uc_ccu_gpreg_set(bool uc_is_egr, u32 idx, u32 gpreg_val)
{
	u64 reg;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	if (unlikely(idx >= UC_CCU_GPREG_MAX)) {
		pr_err("invalid %s gpreg index %u, max=%u\n",
		       uc_is_egr ? "EGRESS" : "INGRESS", idx, UC_CCU_GPREG_MAX);
		return -EINVAL;
	}

	/* write ccu gpreg */
	if (uc_is_egr)
		reg = UC_B_CCU_GPREG_REG(EGRESS, idx);
	else
		reg = UC_B_CCU_GPREG_REG(INGRESS, idx);

	PP_REG_WR32(reg, gpreg_val);

	return 0;
}

s32 uc_ccu_irr_get(bool uc_is_egr, u32 *irr_bitmap)
{
	u64 reg;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	if (unlikely(ptr_is_null(irr_bitmap)))
		return -EINVAL;

	/* read interrupt request register */
	if (uc_is_egr)
		reg = UC_B_CCU_IRR_REG(EGRESS);
	else
		reg = UC_B_CCU_IRR_REG(INGRESS);

	*irr_bitmap = PP_REG_RD32(reg);

	return 0;
}

void uc_ccu_enable_set(bool uc_is_egr, u32 cid, bool enable)
{
	u32 val = enable ? UC_TURN_ON_VAL : 0;

	if (unlikely(!uc_is_cpu_valid(uc_is_egr, cid)))
		return;

	if (uc_is_egr) {
		/* take the uc in/out of reset */
		PP_REG_WR32(UC_B_CCU_UC_UNRESET_REG(EGRESS, cid), val);
		/* enable/disable the uc clock */
		PP_REG_WR32(UC_B_CCU_UC_CLK_EN_REG(EGRESS, cid), val);
	} else {
		/* take the uc in/out of reset */
		PP_REG_WR32(UC_B_CCU_UC_UNRESET_REG(INGRESS, cid), val);
		/* enable/disable the uc clock */
		PP_REG_WR32(UC_B_CCU_UC_CLK_EN_REG(INGRESS, cid), val);
	}

	__uc_cpu_active_set(uc_is_egr, cid, enable);
}

void uc_run_set(bool uc_is_egr, u32 cid, bool enable)
{
	u32 val = enable ? UC_TURN_ON_VAL : 0;

	if (unlikely(!uc_is_cpu_active(uc_is_egr, cid)))
		return;

	/* run the uc core */
	if (uc_is_egr)
		PP_REG_WR32(UC_BOOT_CORE_RUN_REG(EGRESS, cid), val);
	else
		PP_REG_WR32(UC_BOOT_CORE_RUN_REG(INGRESS, cid), val);
}

s32 uc_reader_irr_get(bool uc_is_egr, u32 *irr_bitmap)
{
	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	if (unlikely(ptr_is_null(irr_bitmap)))
		return -EINVAL;

	/* read interrupt request register */
	if (uc_is_egr)
		*irr_bitmap = PP_REG_RD32(UC_B_READER_IRR(EGRESS));
	else
		*irr_bitmap = PP_REG_RD32(UC_B_READER_IRR(INGRESS));

	return 0;
}

void uc_check_for_exception(void)
{
	struct uc_error_info_t error_info;
	u64 addr;
	u8 hw_max_cpu;
	s32 ret;
	u32 cpu;

	ret = uc_ccu_maxcpus_get(true, &hw_max_cpu);
	if (unlikely(ret)) {
		pr_err("Failed to read from CCU max cpu reg\n");
		return;
	}

	for (cpu = 0 ; cpu < hw_max_cpu ; cpu++) {
		addr = (UC_DCCM(EGRESS, cpu, 0)) | (uc_error_off & 0xFFFF);
		memcpy_fromio(&error_info, (void *)addr, sizeof(error_info));
		if (error_info.dump_is_ready) {
			pr_err("\e[0;31m");
			pr_err("======================================================================================\n");
			pr_err(" _   _    ____     _____  __  __   ____   _____   ____    _____   ___    ___    _   _ \n");
			pr_err("| | | |  / ___|   | ____| \\ \\/ /  / ___| | ____| |  _ \\  |_   _| |_ _|  / _ \\  | \\ | |\n");
			pr_err("| | | | | |       |  _|    \\  /  | |     |  _|   | |_) |   | |    | |  | | | | |  \\| |\n");
			pr_err("| |_| | | |___    | |___   /  \\  | |___  | |___  |  __/    | |    | |  | |_| | | |\\  |\n");
			pr_err(" \\___/   \\____|   |_____| /_/\\_\\  \\____| |_____| |_|       |_|   |___|  \\___/  |_| \\_|\n");
			pr_err("======================================================================================\n");
			pr_err("\e[0;39m");
			pr_err("cpu %u", cpu);
			pr_err("vector_num %u", error_info.vector_num);
			pr_err("exception_return_address %#x", error_info.exception_return_address);
			pr_err("exception_return_branch_target_address %#x", error_info.exception_return_branch_target_address);
			pr_err("exception_return_status %#x", error_info.exception_return_status);
			pr_err("exception_cause_reg %#x", error_info.exception_cause_reg);
			pr_err("exception_fault_address %#x", error_info.exception_fault_address);
			pr_err("cause_tid %u", error_info.cause_tid);
			pr_err("job_base %#x", error_info.job_base);
			pr_err("stack_base %#x", error_info.stack_base);
			pr_err("stack_top %#x", error_info.stack_top);
			pr_err("status32 %#x", error_info.status32);
		}
	}
}

s32 uc_ver_get(bool uc_is_egr, u32 *ver)
{
	s32 ret;
	u64 reg;

	if (unlikely(ptr_is_null(ver)))
		return -EINVAL;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	if (uc_is_egr) {
		/* send the mailbox command */
		ret = uc_egr_mbox_cmd_send(UC_CMD_VERSION, 0, NULL, 0,
					   ver, sizeof(*ver));
		if (unlikely(ret))
			return ret;

		reg = UC_B_CCU_GPREG_REG(EGRESS, CCU_DB_ADDR_IDX);
		uc_error_off = PP_REG_RD32(reg) & 0xFFFF;
	} else {
		reg = UC_B_CCU_GPREG_REG(INGRESS, CCU_DB_ADDR_IDX);
		ing_addr_offset = PP_REG_RD32(reg) & 0xFFFF;
		/* read version from SRAM */
		*ver = PP_REG_RD32(UC_SSRAM(INGRESS, ING_VER_SSRAM_OFF));
		ing_ldb1_offset = PP_REG_RD32(UC_SSRAM(INGRESS, ING_LDB1_SSRAM_OFF));
		ing_ldb2_offset = PP_REG_RD32(UC_SSRAM(INGRESS, ING_LDB2_SSRAM_OFF));
	}

	pr_debug("pp uc %s version:MAJOR[%u], MINOR[%u], BUILD[%u]\n",
		uc_is_egr ? "egress" : "ingress", (*ver >> 16) & U8_MAX,
		(*ver >> 8) & U8_MAX, *ver & U8_MAX);

	return 0;
}

/**
 * @brief enable/disable the active bitmap for cpu
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param enable enable/disable
 */
static inline void __uc_cpu_active_set(bool uc_is_egr, u32 cid, bool enable)
{
	ulong *act_bmap = uc_is_egr ? &eg_cpu_act_bmap : &in_cpu_act_bmap;

	if (enable)
		*act_bmap |= BIT(cid);
	else
		*act_bmap &= ~BIT(cid);
}

/**
 * @brief check if the uc init was done
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @return bool true if done, fasle otherwise
 */
static bool __uc_is_uc_init_done(bool uc_is_egr, u32 cid)
{
	u64 reg_addr;
	u32 reg_val;

	if (uc_is_egr)
		reg_addr = UC_BOOT_GPREG_REG(EGRESS, cid);
	else
		reg_addr = UC_BOOT_GPREG_REG(INGRESS, cid);

	pr_debug("start polling on uc %u init done reg type %#x\n", cid, uc_is_egr);
	/* wait until the uc boot init will be done */
	if (unlikely(pp_reg_poll_x(reg_addr, UC_BOOT_GPREG_MSK,
				   UC_BOOT_DONE_VAL, PP_REG_MAX_RETRIES))) {
		reg_val = PP_REG_RD32((unsigned long)reg_addr);
		pr_err("Failed polling on uc %u init done reg contain %#x 0x%llx type %#x fail_reason %#x\n",
		       cid, reg_val, reg_addr, uc_is_egr, ((reg_val >> 16) & 0xFF));
		return false;
	}
	pr_debug("uc boot done, cid %d\n", cid);

	return true;
}

/**
 * @brief enable/disable the valid bitmap for cpu
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param enable enable/disable
 */
static inline void __uc_cpu_valid_set(bool uc_is_egr, u32 cid, bool enable)
{
	ulong *valid_bmap = uc_is_egr ? &eg_cpu_valid_bmap : &in_cpu_valid_bmap;

	if (enable)
		*valid_bmap |= BIT(cid);
	else
		*valid_bmap &= ~BIT(cid);
}

#define UC_BIN_FW_IMAGE_SZ (0x10000)
/**
 * @brief download the image to the uc shared SRAM
 * @param name bin file name
 * @param rst_vec reset vector array
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_download_image_bin(const char *name, bool uc_is_egr)
{
	const struct firmware *fw = NULL;
	s32 ret = 0;
	static struct mxl_fw_dl_data data = {0};
	void *virt_txt;
	dma_addr_t phys_txt;

	data.sai = -1;
	data.opt = MXL_FW_OPT_SKIP_HW_FWRULE;
	virt_txt = mxl_soc_alloc_firmware_buf(pp_dev_get(), UC_BIN_FW_IMAGE_SZ,
							&data);
	if (!virt_txt)	{
		pr_err("failed to alloc firmware memory\n");
		goto free_resources;
	}

	phys_txt = dma_map_single(pp_dev_get(), virt_txt,
			UC_BIN_FW_IMAGE_SZ, DMA_FROM_DEVICE);
	ret = mxl_soc_request_firmware_into_buf(&fw, name,
						pp_dev_get(), virt_txt,
						UC_BIN_FW_IMAGE_SZ, &data);
	if (unlikely(ret)) {
		pr_err("failed to load firmware: %s\n",
			name);
		goto free_resources;
	}
	pr_debug("virt 0x%lx phy 0x%lx allocSize = %d\n ",
		 (unsigned long)virt_txt, (unsigned long)phys_txt,
		 UC_BIN_FW_IMAGE_SZ);

	if (uc_is_egr)
		memcpy_toio((void *)UC_SSRAM(EGRESS, UC_CODE_START_OFFSET),
					fw->data, fw->size);
	else
		memcpy_toio((void *)UC_SSRAM(INGRESS, UC_CODE_START_OFFSET),
					fw->data, fw->size);

free_resources :
	if (fw)
		mxl_soc_release_firmware(fw, pp_dev_get(), NULL, 0, &data);

	if (virt_txt)
		mxl_soc_free_firmware_buf(pp_dev_get(), virt_txt,
					  UC_BIN_FW_IMAGE_SZ, &data);
	return ret;
}

/**
 * @brief verify the cluster by checking the number of cpus
 *        available in HW
 * @param uc_is_egr select the uc cluster
 * @param max_cpu number of cpus requested from the driver
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_cluster_verify(bool uc_is_egr, u32 max_cpu)
{
	u8 hw_max_cpu;
	s32 ret;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	ret = uc_ccu_maxcpus_get(uc_is_egr, &hw_max_cpu);
	if (unlikely(ret))
		return ret;

	if ((u32)hw_max_cpu < max_cpu) {
		pr_err("invalid %s max cpus parameter %u, max_hw %hhu\n",
		       uc_is_egr ? "egress" : "ingress", max_cpu, hw_max_cpu);
		return -EINVAL;
	}

	return 0;
}

s32 uc_fat_entry_get(bool uc_is_egr, u32 ent, u32 *val, bool *enable)
{
	u64 reg;
	u32 max_ent;

	if (unlikely(ptr_is_null(val) || ptr_is_null(enable)))
		return -EINVAL;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	max_ent = UC_FAT_ALL_ENT_CNT;

	if (unlikely(ent >= max_ent)) {
		pr_err("invalid %s cluster fat entry %u\n",
		       uc_is_egr ? "EGRESS" : "INGRESS", ent);
		return -EINVAL;
	}

	if (uc_is_egr) {
		reg = UC_B_CLUSTERBRIDGE_VALID(EGRESS, ent);
		*enable = PP_FIELD_GET(UC_B_FAT_EN_MSK,
						PP_REG_RD32(reg));
		if (ent < UC_FAT_ENT_CNT) {
			reg = UC_B_CLUSTERBRIDGE_FAT(EGRESS, ent);
			*val = PP_FIELD_GET(UC_B_FAT_VAL_MSK,
						PP_REG_RD32(reg));
		} else {
			ent -= UC_FAT_ENT_CNT;
			reg = UC_B_CLUSTERBRIDGE_BASE(EGRESS, ent);
			*val = PP_REG_RD32(reg);
		}
	} else {
		reg = UC_B_CLUSTERBRIDGE_VALID(INGRESS, ent);
		*enable = PP_FIELD_GET(UC_B_FAT_EN_MSK,
						PP_REG_RD32(reg));
		if (ent < UC_FAT_ENT_CNT) {
			reg = UC_B_CLUSTERBRIDGE_FAT(INGRESS, ent);
			*val = PP_FIELD_GET(UC_B_FAT_VAL_MSK,
						PP_REG_RD32(reg));
		} else {
			ent -= UC_FAT_ENT_CNT;
			reg = UC_B_CLUSTERBRIDGE_BASE(INGRESS, ent);
			*val = PP_REG_RD32(reg);
		}
	}

	return 0;
}

s32 uc_fat_entry_set(bool uc_is_egr, u8 ent, u32 val, bool enable)
{
	u32 max_ent;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	max_ent = UC_FAT_ALL_ENT_CNT;

	if (unlikely(ent >= max_ent)) {
		pr_err("invalid %s cluster fat entry %u\n",
		       uc_is_egr ? "EGRESS" : "INGRESS", ent);
		return -EINVAL;
	}

	/* enable/disable the cluster bridge fat */
	if (uc_is_egr) {
		PP_REG_WR32(UC_B_CLUSTERBRIDGE_VALID(EGRESS, ent),
				enable);
		if (ent < UC_FAT_ENT_CNT) {
			PP_REG_WR32(UC_B_CLUSTERBRIDGE_FAT(EGRESS, ent),
					val);
		} else {
			ent -= UC_FAT_ENT_CNT;
			PP_REG_WR32(UC_B_CLUSTERBRIDGE_BASE(EGRESS,
								ent), val);
		}
	} else {
		PP_REG_WR32(UC_B_CLUSTERBRIDGE_VALID(INGRESS, ent),
				enable);
		if (ent < UC_FAT_ENT_CNT) {
			PP_REG_WR32(UC_B_CLUSTERBRIDGE_FAT(INGRESS,
								ent), val);
		} else {
			ent -= UC_FAT_ENT_CNT;
			PP_REG_WR32(UC_B_CLUSTERBRIDGE_BASE(INGRESS,
								ent), val);
		}
	}

	return 0;
}

/**
 * @brief Fix address and size for fat table mapping
 *        all addresses mapped to fat table needs to be aligned
 *        to a fat entry size.
 *        The function align the address and fix the size accordingly
 * @param fat the fat table info
 * @param addr address to fixup
 * @param sz size to map in bytes
 * @param cnt set to the number of entries to map in the table
 */
void fat_addr_fixup(struct fat_tbl *fat, u64 *addr, size_t *sz, u32 *cnt)
{
	size_t align_diff = *addr - (*addr & fat->msb_msk);
	u32 _cnt;

	_cnt = DIV_ROUND_UP(*sz + align_diff, fat->win_sz);
	pr_debug("addr %llx ==> %llx, sz %zu ==> %zu, cnt %u\n", *addr,
		 *addr & fat->msb_msk, *sz, *sz + align_diff, _cnt);
	*sz += align_diff;
	*addr &= fat->msb_msk;
	if (cnt)
		*cnt = _cnt;
}

/**
 * @brief Adds a new entry to a given fat table
 * The uc fat table is capable of translating 32 bits addresses to 32-36
 * bits addresses using 10 MSB translation.
 * The FAT replaces the 6 MSB with 10 bits from the FAT to get up to 36 bits
 * address.
 * There are 8 special entries for FAT where it behaves a bit different.
 * The entries are 32 bits long and used as a base address.
 *
 * The FAT table has a total of 56 entries and behave as follow:
 * for address X, i = X[31-26]
 * if (x < 48):
 *      X &= 0x3ffffff // clean 6 MSB
 *      X |= fat[i]    // adds the 10 bits from the FAT
 * else:
 *      // take only the 26 LSB and adds them to the base address in the FAT
 *      // the base address is shifted by 4 to get up to 36 bits addresses
 *      X = (fat[i] << 4) + (X & 0x3ffffff)
 *
 * e.g. to access PP HW in address 0xF0000000
 * 6 MSB is 0x3C, fat[0x3] = 0x3C (any other entry can be used),
 * uc will access 0x0C000000, 0x0C is actually 0x3 if you take only the 6 MSB.
 *
 * Note that in some cases, due to aligns limitations the address and size
 * are changed and actual mapping is larger then required.
 * e.g. mapping address 0x3f000000 with size of 0x20_00000
 * mapping should be for [0x3f00_0000, 0x4100_0000)
 * w/o fixing it the FAT only maps 0x3c00_0000 - 0x4000_0000
 * so instead:
 *     align_diff = 0x3f00_0000 - 0x3c00_0000 = 0x300_0000
 *     addr_base  = 0x3c00_0000
 *     map size   = 0x200_0000 + 0x3000000 = 0x500_0000
 *     mapping    = [0x3c00_0000, 0x4100_0000)
 * Final mapping will be using 2 adjacent entries in the FAT.
 * That said, minimum actual mapping size is 64 MB (fat entry size) and all
 * addresses are aligned accordingly.
 * @param fat the fat table
 * @param addr the address to add to the fat
 * @param win window size to map
 * @return s32 0 on succuss, error code otherwise
 */
static s32 fat_entry_add(struct fat_tbl *fat, u64 addr, size_t win)
{
	u16 ent, start;
	u16 align;
	u32 cnt;

	/* the 6 MSB we want to translate from */
	ent = PP_FIELD_GET(fat->ent_msk, addr);
	pr_debug("Try to map %#llx, win %#zx, ent %#x\n", addr, win, ent);
	fat_addr_fixup(fat, &addr, &win, &cnt);

	/* find an empty entry to use, first we try the 6 LSB entry
	 * if its not supported or used, we used other entries
	 */
	if (fat->sz < ent)
		ent = 0;

	align = cnt > 1 ? 3 : 0; /* 3 means to align by 4 */
	start = bitmap_find_next_zero_area(fat->bmap, fat->sz, ent, cnt, 0);
	if (start == ent)
		goto alloc_entries;

	pr_debug("Entries %#x-%#x are not available, start %#x\n", ent,
		 ent + cnt - 1, start);

	/* no space starting at ent, try from the start of the table */
	start = bitmap_find_next_zero_area(fat->bmap, fat->sz, 0, cnt, align);
	if (start < fat->sz)
		goto alloc_entries;

	/* no free entries available */
	pr_err("No %u free Entries in fat\n", cnt);
	return -ENOSPC;

alloc_entries:
	bitmap_set(fat->bmap, start, cnt);
	for (ent = start; ent < start + cnt; ent++) {
		/* the 10 MSB we want to translate to */
		fat->fat[ent] = PP_FIELD_GET(fat->msb_msk, addr);
		pr_debug("addr %#llx added to entry %#x\n", addr, ent);
		addr += fat->win_sz;
	}

	return 0;
}

bool fat_is_addr_mapped(struct fat_tbl *fat, u64 addr, size_t win)
{
	u16 cnt, msb, _msb;
	u16 i, start;
	u64 _addr;

	cnt = DIV_ROUND_UP(win, fat->win_sz);
	msb = PP_FIELD_GET(fat->msb_msk, addr);
	for_each_set_bit(i, fat->bmap, fat->sz) {
		if (msb != fat->fat[i])
			continue;

		_addr = addr;
		start = i;
		for (; i < start + cnt; i++) {
			if (!test_bit(i, fat->bmap))
				break;
			_msb = PP_FIELD_GET(fat->msb_msk, _addr);
			if (msb != fat->fat[i])
				break;
			_addr += fat->win_sz;
		}

		if (i < start + cnt)
			break;

		pr_debug("Address %#llx mapped at entries %#x-%#x, %llx\n",
			 addr, start, start + cnt - 1, addr & fat->msb_msk);
		return true;
	}

	/* entry not found */
	pr_debug("Address %#llx isn't mapped\n", addr);
	return false;
}

u64 uc_fat_addr_trans(bool uc_is_egr, u32 addr)
{
	u16 ent = PP_FIELD_GET(uc_fat.ent_msk, addr);
	u32 val;
	s32 ret;
	u64 taddr;
	bool en = false;

	ret = uc_fat_entry_get(uc_is_egr, ent, &val, &en);
	if (ret || !en) {
		pr_err("Failed to translate address %#x, ent %u, en %s\n",
		       addr, ent, BOOL2EN(en));
		return 0;
	}

	/* get the base for the translated address */
	if (ent < UC_FAT_ENT_CNT)
		taddr = PP_FIELD_PREP(uc_fat.msb_msk, val);
	else
		taddr = (u64)val << 4;

	/* add the LSB bits */
	taddr += PP_FIELD_GET(uc_fat.lsb_msk, addr);

	pr_debug("address %#x ==> address %#llx\n", addr, taddr);
	return taddr;
}

u64 uc_fat_addr_rev_trans(bool uc_is_egr, u64 addr)
{
	u16 msb = PP_FIELD_GET(uc_fat.msb_msk, addr);
	u32 val, i;
	s32 ret;
	u64 base;
	bool en;

	for (i = 0; i < UC_FAT_ALL_ENT_CNT; i++) {
		ret = uc_fat_entry_get(uc_is_egr, i, &val, &en);
		if (ret)
			return addr;
		if (!en)
			continue;

		if (i < UC_FAT_ENT_CNT) {
			if (msb != val)
				continue;
			base = PP_FIELD_PREP(uc_fat.ent_msk, i);
		} else {
			base = (u64)val << 4;
			/* in entry range ? */
			if (!(base < addr && addr < base + uc_fat.win_sz))
				continue;
		}

		pr_debug("address %#llx ==> address %#llx, found on entry %u\n",
			 addr, (base + (addr & uc_fat.lsb_msk)), i);
		addr &= uc_fat.lsb_msk;
		return base + addr;
	}

	/* entry not found */
	pr_err("Failed to translate address %#llx\n", addr);
	return addr;
}

/**
 * @brief set the default uc cluster bridge fat configuration
 *        for B-step
 * @param uc_is_egr select the uc cluster
 * @return s32 0 on success, error code otherwise
 */
static s32 uc_cluster_bridge_b_default_set(bool uc_is_egr)
{
	s32 ret;
	u32 i, spl;
	u64 addr;

	if (unlikely(!uc_is_cluster_valid(uc_is_egr)))
		return -EPERM;

	/* configure regular FAT entries */
	for (i = 0; i < UC_FAT_ENT_CNT; i++) {
		ret = uc_fat_entry_set(uc_is_egr, i, uc_fat.fat[i], true);
		if (unlikely(ret))
			return ret;
	}

	/* configure the FAT base entries ,these entries are configured as
	 * full 36 bits base addresses which is shifted right
	 * by 4 for 32 bits register. i is the entry index in the table
	 */
	for (spl = 0; spl < UC_FAT_BASE_ENT_CNT; spl++, i++) {
		addr = PP_FIELD_PREP(uc_fat.msb_msk, uc_fat.fat[i]);
		ret = uc_fat_entry_set(uc_is_egr, i, (addr >> 4), true);

		if (unlikely(ret))
			return ret;
	}

	return 0;
}

static s32 uc_cluster_mem_access_set(bool uc_is_egr)
{
	return uc_cluster_bridge_b_default_set(uc_is_egr);
}

/**
 * @brief set the uc shared accelerators
 * @param cfg uc init parameters
 * @param max_cpu num of cpus
 * @param uc_is_egr select the uc cluster
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_shrd_acc_set(struct uc_egr_init_params *cfg, u32 max_cpu,
			     bool uc_is_egr)
{
	s32 ret, i;
	u8 hw_max_cpu;

	ret = uc_ccu_maxcpus_get(uc_is_egr, &hw_max_cpu);
	if (unlikely(ret))
		return ret;

	for (i = 0; i < hw_max_cpu; i++)
		__uc_cpu_valid_set(uc_is_egr, i, true);

	ret = __uc_cluster_verify(uc_is_egr, max_cpu);
	if (unlikely(ret))
		return ret;

	ret = uc_cluster_mem_access_set(uc_is_egr);
	if (unlikely(ret))
		return ret;

	if (uc_is_egr == UC_IS_EGR) {
		__uc_egr_reader_wred_addr_set(cfg->wred_base);
		__uc_egr_reader_client_addr_set(cfg->client_base);
		__uc_egr_reader_qm_addr_set(cfg->qm_base);
		__uc_egr_reader_bm_addr_set(cfg->bm_base);
	}

	return 0;
}

/**
 * @brief set the uc profile
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param prof profile index
 */
static void __uc_profile_set(bool uc_is_egr, u32 cid, u32 prof)
{
	u32 val;

	pr_debug("pp uc cpu %u profile %u\n", cid, prof);
	if (cid == 0)
		val = (UC_INIT_CLUSTER_ONCE | prof);
	else
		val = prof;
	/* set the uc profile */
	if (uc_is_egr)
		PP_REG_WR32(UC_BOOT_GPREG_REG(EGRESS, cid), val);
	else
		PP_REG_WR32(UC_BOOT_GPREG_REG(INGRESS, cid), val);
}

/**
 * @brief set the uc reset vector
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param vec reset vector array
 */
static void __uc_rst_vec_set(bool uc_is_egr, u32 cid)
{
	/* set the uc reset vector */
	if (uc_is_egr) {
		PP_REG_WR32(UC_BOOT_RST_VEC_REG(EGRESS, cid, 0), UC_SRAM_BASE);
	} else {
		PP_REG_WR32(UC_BOOT_RST_VEC_REG(INGRESS, cid, 0), UC_SRAM_BASE);
	}
}

static s32 __uc_txmgr_port_update(u16 port, ulong ring_addr, u16 ring_sz,
				  u16 credit)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_port_conf conf;
	u32 logical_id;
	s32 ret;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EPERM;

	logical_id = pp_qos_port_id_get(qdev, port);

	ret = pp_qos_port_conf_get(qdev, logical_id, &conf);
	if (unlikely(ret)) {
		pr_err("pp_qos_port_conf_get failed\n");
		goto done;
	}
	conf.ring_address = ring_addr;
	conf.ring_size    = ring_sz;
	conf.credit       = credit;
	ret = pp_qos_port_set(qdev, logical_id, &conf);
	if (unlikely(ret)) {
		pr_err("pp_qos_port_set failed\n");
		goto done;
	}

	pr_debug("set port done: phys: %d. logical: %d\n", port, logical_id);

done:
	return ret;
}

s32 uc_aux_reg_read(u32 reg, u32 *val)
{
	if (ptr_is_null(val))
		return -EINVAL;

	return uc_egr_mbox_cmd_send(UC_CMD_AUX_REG_RD, reg, NULL, 0,
				    val, sizeof(*val));
}

s32 uc_aux_reg_write(u32 reg, u32 val)
{
	struct mbox_aux_reg_wr args;

	args.reg = reg;
	args.val = val;

	return uc_egr_mbox_cmd_send(UC_CMD_AUX_REG_WR, 0, (const void *)&args,
				    sizeof(args), NULL, 0);
}

u32 uc_cpu_sleep_count(bool uc_is_egr, unsigned int cid)
{
	if (!uc_is_cpu_valid(uc_is_egr, cid))
		return 0;

	if (uc_is_egr)
		return PP_REG_RD32(UC_B_EGRESS_CPU_SLEEP_CNT(cid));
	else
		return PP_REG_RD32(UC_B_INGRESS_CPU_SLEEP_CNT(cid));
}

s32 __nf_reass_set(u16 tx_queue, u16 dflt_hif)
{
	struct reassembly_info info;
	struct pp_si dflt_si;
	s32 ret;
	struct rx_dma_port_cfg port_cfg;

	memset(&info, 0, sizeof(info));

	ret = chk_exception_session_si_get(dflt_hif, &dflt_si);
	if (unlikely(ret)) {
		pr_err("failed to get dflt host i/f %u si, ret %d\n",
		       dflt_hif, ret);
		return ret;
	}

	info.pp_rx_q            = tx_queue;
	info.host_q             = dflt_si.dst_q;
	info.host_base_policy   = dflt_si.base_policy;
	info.host_policies_bmap = dflt_si.policies_map;
	info.host_pid           = dflt_si.eg_port;
	info.si_base = cls_si_phys_base_get(NULL);

	port_cfg.port_id = dflt_si.eg_port;
	ret = rx_dma_port_get(&port_cfg);
	if (unlikely(ret)) {
		pr_err("rx_dma_port_get err %d\n", ret);
		return ret;
	}

	info.host_port_hr = port_cfg.headroom_size;
	info.host_port_tr = port_cfg.tailroom_size;
	info.host_port_flags = 0;
	if (port_cfg.wr_desc)
		info.host_port_flags |= BUFF_EX_FLAG_WR_DESC;

	/* Write host info to SRAM to be used by ingress uc */
	PP_REG_WR32(UC_SSRAM(INGRESS, ING_HOST_INFO_PORT_SSRAM_OFF),
		    dflt_si.eg_port);
	PP_REG_WR32(UC_SSRAM(INGRESS, ING_HOST_INFO_QUEUE_SSRAM_OFF),
		    dflt_si.dst_q);
	PP_REG_WR32(UC_SSRAM(INGRESS, ING_HOST_INFO_BASE_POLICY_SSRAM_OFF),
		    dflt_si.base_policy);
	PP_REG_WR32(UC_SSRAM(INGRESS, ING_HOST_INFO_POLICY_BMAP_SSRAM_OFF),
		    dflt_si.policies_map);

	return uc_egr_mbox_cmd_send(UC_CMD_REASSEMBLY_INFO, 0,
				    (const void *)&info, sizeof(info), NULL, 0);
}

static s32 __writer_port_enable(u32 uc_port, u16 qos_port)
{
	ulong r_addr, p_enable_reg;
	u16 r_sz, r_cred;
	void *r_addr_virt;
	s32 ret;

	p_enable_reg = UC_EGR_B_WRITER_PORT_ENABLE(uc_port);

	if (PP_REG_RD32(p_enable_reg) == UC_TURN_ON_VAL) {
		pr_err("uc nf type %u already enabled\n", uc_port);
		return -EPERM;
	}

	/* set the tx manager port address */
	PP_REG_WR32(UC_EGR_B_WRITER_TX_PORT(uc_port),
		    uc_fat_addr_rev_trans(UC_IS_EGR,
					  UC_TXMGR_CRED_INC_ADDR(qos_port)));
	r_sz = 1;
	r_cred = 4;
	r_addr_virt = (void *)(ulong)UC_EGR_B_WRITER_PORT_ADDR(uc_port);

	r_addr = (ulong)pp_virt_to_phys(r_addr_virt);
	ret = __uc_txmgr_port_update(qos_port, r_addr, r_sz, r_cred);
	if (unlikely(ret))
		return ret;

	/* enable the port */
	PP_REG_WR32(p_enable_reg, UC_TURN_ON_VAL);

	return 0;
}

s32 uc_nf_set(enum pp_nf_type nf, u16 pid, u16 subif, u16 qos_port,
	      u16 tx_queue, u16 uc_q, u16 dflt_hif, void *data)
{
	struct ipsec_lld_info ipsec_lld_info = { 0 };
	u16 vpn_gpid;
	u32 uc_port;
	s32 ret;

	if (!uc_is_cluster_valid(UC_IS_EGR))
		return -EPERM;

	switch (nf) {
	case PP_NF_MULTICAST:
		uc_port = UC_MCAST_PORT;
		/* set the mcast tx queue */
		ret = uc_egr_mbox_cmd_send(UC_CMD_MCAST_QUEUE, tx_queue, NULL,
					   0, NULL, 0);
		if (unlikely(ret))
			return ret;
		/* set the mcast pid */
		ret = uc_egr_mbox_cmd_send(UC_CMD_MCAST_PID, pid, NULL, 0,
					   NULL, 0);
		if (unlikely(ret))
			return ret;
		break;
	case PP_NF_IPSEC_LLD:
		uc_port = UC_IPSEC_LLD_PORT;
		vpn_gpid = PP_PORT_INVALID;
		if (data)
			vpn_gpid = *(u16 *)data;
		ipsec_lld_info.ipsec.tx_gpid  = pid;
		ipsec_lld_info.ipsec.tx_q     = tx_queue;
		ipsec_lld_info.ipsec.tx_subif = subif;
		ipsec_lld_info.ipsec.vpn_gpid = vpn_gpid;
		ipsec_lld_info.lld.nf_queue = uc_q;

		ret = uc_egr_mbox_cmd_send(UC_CMD_IPSEC_INFO, 0,
					   &ipsec_lld_info,
					   sizeof(ipsec_lld_info), NULL, 0);
		if (unlikely(ret))
			return ret;
		break;
	case PP_NF_REASSEMBLY:
		uc_port = UC_REASS_PORT;
		ret = __nf_reass_set(tx_queue, dflt_hif);
		if (unlikely(ret)) {
			pr_err("__nf_reass_set failed, ret %d\n", ret);
			return ret;
		}
		break;
	case PP_NF_TURBODOX:
		uc_port = UC_TDOX_PORT;
		break;
	case PP_NF_FRAGMENTER:
		uc_port = UC_FRAG_PORT;
		break;
	case PP_NF_REMARKING:
		uc_port = UC_REMARK_PORT;
		break;
	default:
		pr_err("nf type is not supported\n");
		return -EINVAL;
	}

	uc_port += UC_B_TXMGR_PORT_OFF;

	return __writer_port_enable(uc_port, qos_port);
}

/**
 * @brief set the uc private accelerators
 * @param uc_is_egr select the uc cluster
 * @param cfg cpus init parameters
 * @param rst_vec reset vector array
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_priv_acc_set(bool uc_is_egr, struct uc_cpu_params *cfg)
{
	u32 cid;

	for (cid = 0; cid < cfg->max_cpu; cid++) {
		uc_ccu_enable_set(uc_is_egr, cid, true);
		__uc_profile_set(uc_is_egr, cid, cfg->cpu_prof[cid]);
		__uc_rst_vec_set(uc_is_egr, cid);
		uc_run_set(uc_is_egr, cid, true);
		/* in this stage, the uc is run and execute the
		 * cluster_init() and app_init() routines
		 */
		/* verify the uc is up and running */
		if (unlikely(!__uc_is_uc_init_done(uc_is_egr, cid)))
			return -EBUSY;
	}

	return 0;
}

/**
 * @brief verify the uc version major and minor numbers
 * @param uc_is_egr select the uc cluster
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_ver_verify(bool uc_is_egr)
{
	u32 ver;
	u8 major, minor;
	s32 ret;

	/* get fw version */
	ret = uc_ver_get(uc_is_egr, &ver);
	if (unlikely(ret))
		return ret;

	if (uc_is_egr) {
		major = UC_VER_MAJOR(EGRESS);
		minor = UC_VER_MINOR(EGRESS);
	} else {
		major = UC_VER_MAJOR(INGRESS);
		minor = UC_VER_MINOR(INGRESS);
	}
	if ((major != ((ver >> 16) & U8_MAX)) ||
	    (minor != ((ver >> 8)  & U8_MAX))) {
		pr_err("uc %s version mismatch error, driver version %u.%u, fw version %u.%u\n",
		       uc_is_egr ? "egrees" : "ingress", major, minor,
		       ((ver >> 16) & U8_MAX), ((ver >> 8) & U8_MAX));
		pr_err("\e[0;31m");
		pr_err("==================================================================================================================================================\n");
		pr_err("==================================================================================================================================================\n");
 		pr_err(" _____  __        __   __     __  _____   ____    ____    ___    ___    _   _     __  __   ___   ____    __  __      _      _____    ____   _   _ \n");
 		pr_err("|  ___| \\ \\      / /   \\ \\   / / | ____| |  _ \\  / ___|  |_ _|  / _ \\  | \\ | |   |  \\/  | |_ _| / ___|  |  \\/  |    / \\    |_   _|  / ___| | | | |\n");
 		pr_err("| |_     \\ \\ /\\ / /     \\ \\ / /  |  _|   | |_) | \\___ \\   | |  | | | | |  \\| |   | |\\/| |  | |  \\___ \\  | |\\/| |   / _ \\     | |   | |     | |_| |\n");
 		pr_err("|  _|     \\ V  V /       \\ V /   | |___  |  _ <   ___) |  | |  | |_| | | |\\  |   | |  | |  | |   ___) | | |  | |  / ___ \\    | |   | |___  |  _  |\n");
 		pr_err("|_|        \\_/\\_/         \\_/    |_____| |_| \\_\\ |____/  |___|  \\___/  |_| \\_|   |_|  |_| |___| |____/  |_|  |_| /_/   \\_\\   |_|    \\____| |_| |_|\n");
		pr_err("==================================================================================================================================================\n");
		pr_err("==================================================================================================================================================\n");
		pr_err("\e[0;39m");
		return -EACCES;
	}

	pr_info("pp %s uc version: MAJOR[%u], MINOR[%u], BUILD[%u]\n",
		uc_is_egr ? "egress " : "ingress", (ver >> 16) & U8_MAX,
		(ver >> 8) & U8_MAX, ver & U8_MAX);

	return 0;
}

void uc_egr_tdox_stats_reset(u32 dccm_addr, size_t size)
{
	u64 addr = (UC_DCCM(EGRESS, TDOX_CPU, 0)) |
			(dccm_addr & 0xFFFF);
	memset_io((void *)addr, 0, size);
}

s32 __uc_egr_info_set(struct uc_init_params *cfg)
{
	struct eg_uc_init_info info;
	struct pp_rx_dma_init_params rxdma_cfg;
	struct pool_info *pool;
	u32 i;

	egr_log_buf.virt = logger_buf.virt;
	egr_log_buf.phys = logger_buf.phys;
	egr_log_buf.sz   = UC_LOGGER_BUFF_SZ;

	memset(&info, 0, sizeof(info));
	info.logger_buff = egr_log_buf.phys;
	info.logger_buff_sz = egr_log_buf.sz;
	info.chk_base = chk_base_addr;
	info.cls_base = cls_base_addr;
	info.wred_cfg_base = wred_cfg_base_addr;
	info.qos_misc_base = qos_misc_base_addr;
	rx_dma_config_get(&rxdma_cfg);
	for (i = 0 ; i < ARRAY_SIZE(info.buffer_size) ; i++)
		info.buffer_size[i] = rxdma_cfg.buffer_size[i];

	/* Add other addresses to the FAT */
	i = 0;
	for_each_arr_entry(pool, cfg->pools, ARRAY_SIZE(cfg->pools), i++) {
		if (!pool->addr || !pool->sz)
			continue;

		info.pool[i].base = (u64)pool->addr;
		info.pool[i].sz = (u32)pool->sz;
	}

	return uc_egr_mbox_cmd_send(UC_CMD_INIT, 0, (const void *)&info,
				    sizeof(info), NULL, 0);
}

s32 uc_load_init(const char *name, struct uc_egr_init_params *egr,
		 struct uc_cpu_params *cpu_params, u8 max_cpu, bool is_egr)
{
	s32 ret = 0;

	/* copy image to cluster SRAM */
	ret = __uc_download_image_bin(name, is_egr);
	if (unlikely(ret))
		return ret;

	/* configure the shared accelerators */
	ret = __uc_shrd_acc_set(egr, max_cpu, is_egr);
	if (unlikely(ret))
		return ret;

	/* configure the private accelerators */
	ret = __uc_priv_acc_set(is_egr, cpu_params);
	if (unlikely(ret))
		return ret;

	return ret;
}

/**
 * @brief initialized the egress uc cluster
 * @param cfg egress uc init parameters
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_egr_init(struct uc_init_params *cfg)
{
	struct uc_egr_init_params *egr_cfg = &cfg->egr;
	s32 ret = 0;

	pr_debug("pp uc egress max-cpu: %u\n", egr_cfg->cpus.max_cpu);

	/* Save the tx manager address for NF configuration */
	txm_cred_base = (u32)egr_cfg->txm_cred_base;

	/* copy image to cluster SRAM */
	ret = __uc_download_image_bin("egress_uc.bin", UC_IS_EGR);
	if (unlikely(ret))
		return ret;

	/* configure the shared accelerators */
	ret = __uc_shrd_acc_set(egr_cfg, egr_cfg->cpus.max_cpu, UC_IS_EGR);
	if (unlikely(ret))
		return ret;

	/* configure the private accelerators */
	ret = __uc_priv_acc_set(UC_IS_EGR, &egr_cfg->cpus);
	if (unlikely(ret))
		return ret;


	/* init the uc egress mbox */
	ret = uc_egr_mbox_init();
	if (unlikely(ret))
		return ret;

	/* verify fw version */
	ret = __uc_ver_verify(UC_IS_EGR);
	if (unlikely(ret))
		return ret;

	ret = __uc_egr_info_set(cfg);
	if (unlikely(ret))
		return ret;

	pr_debug("pp uc egress init done\n");
	return 0;
}

/**
 * @brief exit the uc egress cluster
 */
static void __uc_egr_exit(void)
{
	uc_egr_mbox_exit();
	pr_debug("done\n");
}

s32 uc_ing_db_get(struct ingress_uc_gdb *dst)
{
	u64 addr = UC_SSRAM(INGRESS, ing_addr_offset);

	if (ptr_is_null(dst))
		return -EINVAL;

	memcpy_fromio(dst, (void *)addr, sizeof(*dst));

	return 0;
}

s32 uc_ing_logger_init(void)
{
	struct uc_ing_cmd msg = {0};
	int ret;

	if (ptr_is_null(logger_buf.virt))
		return -EINVAL;

	ing_log_buf.virt = logger_buf.virt + UC_LOGGER_BUFF_SZ;
	ing_log_buf.phys = logger_buf.phys + UC_LOGGER_BUFF_SZ;
	ing_log_buf.sz   = UC_LOGGER_BUFF_SZ;

	msg.msg_type = ING_MBOX_LOGGER_INIT;
	msg.ing_cmd.log_init.buff = ing_log_buf.phys;
	msg.ing_cmd.log_init.sz = ing_log_buf.sz;

	ret = uc_ing_host_mbox_cmd_send(&msg, true);
	if (!ret)
		return 0;

	pr_err("Failed to init ingress logger, ret %d\n", ret);
	return -EINVAL;
}

void uc_ing_init_dccm(void)
{
	u32 i, chk_base;
	phys_addr_t addr; /* checker base register address in dccm */

	chk_base = uc_fat_addr_rev_trans(UC_IS_ING, chk_base_addr);
	/* copy necessary info for all ingress CPUs */
	for_each_ing_active_cpu(i) {
		addr = DCCM_INGRESS_ADDR(i, ING_CHK_BASE);
		memcpy_toio((void *)addr, &chk_base, sizeof(chk_base));
	}
}

/**
 * @brief initialized the ingress uc cluster
 * @param cfg ingress uc init parameters
 * @return s32 0 on success, error code otherwise
 */
static s32 __uc_ing_init(struct uc_ing_init_params *cfg)
{
	s32 ret = 0;

	pr_debug("pp uc ingress max-cpu: %u\n", cfg->cpus.max_cpu);

	/* initialized whitlist DB */
	ret = uc_gpid_group_init();
	if (unlikely(ret))
		return ret;

	/* copy image to SRAM */
	ret = __uc_download_image_bin("ingress_uc.bin", UC_IS_ING);
	if (unlikely(ret))
		return ret;

	/* configure the shared accelerators */
	ret = __uc_shrd_acc_set(NULL, cfg->cpus.max_cpu, UC_IS_ING);
	if (unlikely(ret))
		return ret;

	/* configure the private accelerators */
	ret = __uc_priv_acc_set(UC_IS_ING, &cfg->cpus);
	if (unlikely(ret))
		return ret;

	/* verify fw version */
	ret = __uc_ver_verify(UC_IS_ING);
	if (unlikely(ret))
		return ret;

	/* init dccm stuff */
	uc_ing_init_dccm();

	/* write whitelist info to sram */
	uc_ing_logger_init();

	pr_debug("pp uc ingress init done\n");
	return 0;
}

/**
 * @brief exit the uc ingress cluster
 */
static void __uc_ing_exit(void)
{
	pr_debug("done\n");
}

#if IS_ENABLED(PP_POWER_ON_REGS_TEST)
s32 uc_dbg_clk_set(struct uc_init_params *init_param, bool en)
{
	u32 cid;

	if (!init_param->ing.valid || !init_param->egr.valid)
		return -EINVAL;

	uc_egr_base_addr = init_param->egr.uc_base;
	uc_ing_base_addr = init_param->ing.uc_base;

	for (cid = 0; cid < init_param->egr.cpus.max_cpu; cid++) {
		__uc_cpu_valid_set(UC_IS_EGR, cid, true);
		uc_ccu_enable_set(UC_IS_EGR, cid, en);
		__uc_cpu_valid_set(UC_IS_EGR, cid, false);
	}
	for (cid = 0; cid < init_param->ing.cpus.max_cpu; cid++) {
		__uc_cpu_valid_set(UC_IS_ING, cid, true);
		uc_ccu_enable_set(UC_IS_ING, cid, en);
		__uc_cpu_valid_set(UC_IS_EGR, cid, false);
	}

	return 0;
}
#endif

s32 uc_log_buff_info_get(void *buff, size_t sz, bool is_egr)
{
	void *virt;

	if (unlikely(ptr_is_null(buff) || ptr_is_null(logger_buf.virt)))
		return -EINVAL;
	if (!sz)
		return -ENOBUFS;

	virt = is_egr ? egr_log_buf.virt : ing_log_buf.virt;
	sz   = min(sz, is_egr ? egr_log_buf.sz : ing_log_buf.sz);

	pp_cache_invalidate(virt, sz);
	memcpy(buff, virt, sz);

	return 0;
}

s32 uc_log_reset(bool is_egr)
{
	struct uc_ing_cmd msg = {0};
	int ret;

	if (is_egr)
		return uc_egr_mbox_cmd_send(UC_CMD_LOGGER_RESET, 0, NULL, 0,
					    NULL, 0);

	msg.msg_type = ING_MBOX_LOGGER_RESET;
	ret = uc_ing_host_mbox_cmd_send(&msg, false);
	if (!ret)
		return 0;

	pr_err("Failed to reset ingress logger, ret %d\n", ret);
	return -EINVAL;
}

s32 uc_log_level_set(enum uc_log_level l, bool is_egr)
{
	struct uc_ing_cmd msg = {0};
	int ret;

	if (is_egr)
		return uc_egr_mbox_cmd_send(UC_CMD_LOGGER_LEVEL_SET, l,
					    NULL, 0, NULL, 0);

	msg.msg_type = ING_MBOX_LOGGER_LEVEL_SET;
	msg.ing_cmd.log_lvl.level = l;
	ret = uc_ing_host_mbox_cmd_send(&msg, false);
	if (!ret)
		return 0;

	pr_err("Failed to set ingress logger level to %u, ret %d\n", l, ret);
	return -EINVAL;
}

static void __uc_logger_init(void)
{
	void *virt;
	dma_addr_t phys;
	size_t sz;

	sz   = PAGE_ALIGN(UC_LOGGER_BUFF_SZ * 2);
	virt = pp_dma_alloc(sz, &phys, PP_DMA_NONECOHERENT);
	if (unlikely(!virt)) {
		pr_err("Could not allocate %zu bytes for egress uc logger\n",
			sz);
		return;
	}

	memset(virt, 0, sz);
	pp_cache_writeback(virt, sz);
	logger_buf.virt = virt;
	logger_buf.phys = phys;
	logger_buf.sz   = sz;
}

static void __uc_logger_destroy(void)
{
	if (logger_buf.virt)
		pp_dma_free(logger_buf.virt, logger_buf.sz);

	memset(&egr_log_buf, 0, sizeof(egr_log_buf));
	memset(&ing_log_buf, 0, sizeof(ing_log_buf));
	memset(&logger_buf, 0, sizeof(logger_buf));
}

#define UC_EGR_LONG_TMR_TICKS_OFF 0x3D0200

dma_addr_t uc_egr_cnt64_tmr_ticks_phys_get(void)
{
	return pp_virt_to_phys(
		(void *)(ulong)(uc_egr_base_addr + UC_EGR_LONG_TMR_TICKS_OFF));
}

#define UC_EGR_TDOX_TMR_TICKS_OFF 0x3D8200

dma_addr_t uc_egr_tdox_tmr_ticks_phys_get(void)
{
	return pp_virt_to_phys(
		(void *)(ulong)(uc_egr_base_addr + UC_EGR_TDOX_TMR_TICKS_OFF));
}

dma_addr_t uc_egr_lld_aqm_info_phys_addr_get(void)
{
	u32 val;

	if (uc_ccu_gpreg_get(true, CCU_LLD_AQM_INFO_IDX, &val)) {
		pr_err("uc_ccu_gpreg_get failed to get LLD AQM Info\n");
		return 0;
	}

	return pp_virt_to_phys(
		(void *)(ulong)(DCCM_EGRESS_ADDR(((val & 0xFFFF0000) >> 16), (val & 0xFFFF))));
}

#define UC_PP_REGS_MAP_SZ (UC_FAT_ENT_WIN_SZ * 4) /* 256 MB */

s32 uc_init_fat(struct uc_init_params *param, struct fat_tbl *fat)
{
	/* block bitmap which specify which addresses should be mapped into
	 * the FAT table.
	 * each bit represent 64MB memory block due to the HW FAT
	 * implementation which replaces 6 MSB to 10 MSB.
	 * 6 MSB means each entry of the FAT is 64MB (2^26)
	 */
	ulong blocks_map[BITS_TO_LONGS(BIT(UC_FAT_NUM_BITS))];
	struct pool_info *pool;
	u64 addr;
	size_t sz;
	u32 i, j, cnt;
	s32 ret;

	memset(&blocks_map, 0, sizeof(blocks_map));
	memset(&uc_fat, 0, sizeof(uc_fat));
	uc_fat.ent_msk = UC_FAT_ENT_MSK;
	uc_fat.lsb_msk = UC_FAT_LSB_MSK;
	uc_fat.msb_msk = UC_FAT_MSB_MSK;
	uc_fat.win_sz = UC_FAT_ENT_WIN_SZ;
	uc_fat.sz = UC_FAT_ALL_ENT_CNT;

	/* Static memories, PP registers and SI */
	addr = param->ppv4_base;
	sz = UC_PP_REGS_MAP_SZ;
	fat_addr_fixup(fat, &addr, &sz, &cnt);
	bitmap_set(blocks_map, PP_FIELD_GET(fat->msb_msk, addr), cnt);
	/* add SI region from classifier */
	addr = cls_si_phys_base_get(&sz);
	fat_addr_fixup(fat, &addr, &sz, &cnt);
	bitmap_set(blocks_map, PP_FIELD_GET(fat->msb_msk, addr), cnt);
	/* UC logger buffer */
	addr = logger_buf.phys;
	sz = logger_buf.sz;
	fat_addr_fixup(fat, &addr, &sz, &cnt);
	bitmap_set(blocks_map, PP_FIELD_GET(fat->msb_msk, addr), cnt);

	for_each_arr_entry(pool, param->pools, ARRAY_SIZE(param->pools)) {
		if (!pool->addr || !pool->sz)
			continue;

		pr_debug("Pool[%u]: addr %llx, sz %zu\n",
			 arr_entry_idx(param->pools, pool), pool->addr,
			 pool->sz);

		addr = pool->addr;
		sz = pool->sz;
		fat_addr_fixup(fat, &addr, &sz, &cnt);
		/* turn on relevant bits */
		bitmap_set(blocks_map, PP_FIELD_GET(fat->msb_msk, addr), cnt);
	}

	pr_debug("blocks_map: %*pbl\n", (u32)BIT(UC_FAT_NUM_BITS), blocks_map);
	bitmap_for_each_set_region(blocks_map, i, j, 0, BIT(UC_FAT_NUM_BITS)) {
		addr = PP_FIELD_PREP(fat->msb_msk, i);
		sz = (j - i) * fat->win_sz;
		pr_debug("Add Block [%u, %u] address %#llx, sz %#zx\n", i, j,
			 addr, sz);

		if (fat_is_addr_mapped(fat, addr, sz))
			continue;

		ret = fat_entry_add(fat, addr, sz);
		if (ret)
			return ret;
	}

	/* enable all other entires as passthrough */
	for_each_clear_bit(i, fat->bmap, fat->sz) {
		pr_debug("Bypassing entry %#x\n", i);
		ret = fat_entry_add(fat, PP_FIELD_PREP(fat->msb_msk, i),
				    fat->win_sz);
		if (ret)
			return ret;
	}

	return 0;
}

s32 uc_init(struct uc_init_params *init_param)
{
	s32 ret = 0;

	if (!init_param->ing.valid || !init_param->egr.valid)
		return -EINVAL;

	__uc_logger_init();

#ifdef UC_DUT_ENABLE
	uc_dut_save_egr_init(&init_param->egr);
#endif
	uc_egr_base_addr 	= init_param->egr.uc_base;
	uc_egr_cpus_addr 	= init_param->egr.uc_base + UC_EGR_CPUS_OFF;
	uc_ing_base_addr 	= init_param->ing.uc_base;
	uc_ing_cpus_addr 	= init_param->ing.uc_base + UC_ING_CPUS_OFF;
	chk_base_addr    	= init_param->egr.chk_base;
	cls_base_addr     	= init_param->egr.cls_base;
	wred_cfg_base_addr	= init_param->egr.wred_cfg_base;
	qos_misc_base_addr  = init_param->egr.qos_misc_base;

	ret = uc_init_fat(init_param, &uc_fat);
	if (unlikely(ret))
		return ret;

	if (init_param->egr.cpus.max_cpu) {
		ret = __uc_egr_init(init_param);
		if (unlikely(ret))
			return ret;
	}

	if (init_param->ing.cpus.max_cpu) {
		ret = __uc_ing_init(&init_param->ing);
		if (unlikely(ret))
			return ret;
	}

	/* debugfs */
	ret = uc_dbg_init(init_param->dbgfs);
	if (unlikely(ret))
		return ret;

	pr_debug("done\n");
	return 0;
}

void uc_exit(void)
{
	__uc_egr_exit();
	__uc_ing_exit();
	uc_dbg_clean();
	__uc_logger_destroy();
}
