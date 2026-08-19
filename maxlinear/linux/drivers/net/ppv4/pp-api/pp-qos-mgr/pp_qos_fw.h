/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2017-2020 Intel Corporation
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
 * Description: Packet Processor QoS Driver
 */

#ifndef PP_QOS_FW_H
#define PP_QOS_FW_H

#include "pp_qos_utils.h"
#include "pp_qos_common.h"
#include "pp_qos_uc_defs.h"

extern u64 qos_misc_base_addr;
#define QOS_MISC_BASE_ADDR qos_misc_base_addr

struct ppv4_qos_fw {
	size_t size;
	const u8 *data;
};

s32 load_qos_firmware(struct pp_qos_dev *qdev, const char *name);

s32 qos_fw_init(struct pp_qos_dev *qdev);

s32 qos_fw_start(struct pp_qos_dev *qdev);

s32 wake_uc(void);

bool is_uc_awake(void);

void qos_fw_release(struct pp_qos_dev *qdev);
#ifndef PP_QOS_DISABLE_CMDQ
void create_move_cmd(struct pp_qos_dev *qdev, u16 dst, u16 src, u16 dst_port);

s32 create_set_port_cmd(struct pp_qos_dev *qdev,
			 const struct pp_qos_port_conf *conf, u32 phy,
			 u64 modified);

void create_set_queue_cmd(struct pp_qos_dev *qdev,
			  const struct pp_qos_queue_conf *conf,
			  u32 phy, const struct qos_node *node,
			  u64 modified, bool is_orphaned);

void create_set_sched_cmd(struct pp_qos_dev *qdev,
			  const struct pp_qos_sched_conf *conf,
			  const struct qos_node *node,
			  u64 modified);

void create_add_sync_queue_cmd(struct pp_qos_dev *qdev,
			       const struct pp_qos_queue_conf *conf,
			       u32 phy, u32 rlm, u64 modified);

void create_remove_node_cmd(struct pp_qos_dev *qdev, enum node_type type,
			    u32 phy, u32 rlm);

void update_parent(struct pp_qos_dev *qdev, u32 phy);
void update_preds(struct pp_qos_dev *qdev, u32 phy,
		  bool queue_port_changed);
void create_init_qos_cmd(struct pp_qos_dev *qdev);
void enqueue_cmds(struct pp_qos_dev *qdev);
bool is_ongoing(struct pp_qos_dev *qdev);
s32 check_completion(struct pp_qos_dev *qdev);

void create_get_queue_stats_cmd(struct pp_qos_dev *qdev, u32 phy, u32 rlm,
				struct queue_stats_s *qstat,
				bool reset);

void create_get_qm_stats_cmd(struct pp_qos_dev *qdev, u32 rlm,
			    struct qm_info *qstat);

void create_aqm_sf_set_cmd(struct pp_qos_dev *qdev, u8 sf_id, u8 enable,
			   struct pp_qos_aqm_lld_sf_config *sf_cfg,
			   u32 *queue_id);

void create_codel_cfg_set_cmd(struct pp_qos_dev *qdev,
			      struct pp_qos_codel_cfg *cfg);

void create_get_codel_stats_cmd(struct pp_qos_dev *qdev, u32 rlm,
				struct codel_qstats_s *qstat, bool reset_stats);

void create_get_port_stats_cmd(struct pp_qos_dev *qdev, u32 phy,
			       struct port_stats_s *pstat, bool reset_stats);

void create_get_hist_stats_cmd(struct pp_qos_dev *qdev, u8 sf_id,
			       struct hist_stats_s *hstat, bool reset);

s32 init_fwdata_internals(struct pp_qos_dev *qdev);
void clean_fwdata_internals(struct pp_qos_dev *qdev);

void create_init_logger_cmd(struct pp_qos_dev *qdev,
			    enum uc_logger_level level,
			    enum uc_logger_mode mode);

void create_add_shared_group_cmd(struct pp_qos_dev *qdev, u32 id, u32 limit);
void create_remove_shared_group_cmd(struct pp_qos_dev *qdev, u32 id);
void create_set_shared_group_cmd(struct pp_qos_dev *qdev, u32 id,
				 u32 max_burst);
void create_push_desc_cmd(struct pp_qos_dev *qdev, u32 queue, u32 size,
			  u32 pool, u32 policy, u32 color, u32 gpid,
			  u32 data_off, u32 addr);
void create_pop_desc_cmd(struct pp_qos_dev *qdev, u32 queue, u32 addr);
void create_mcdma_copy_cmd(struct pp_qos_dev *qdev, u32 src, u32 dst, u32 sz);
void create_get_node_info_cmd(struct pp_qos_dev *qdev, u32 phy,
			      struct hw_node_info_s *info);
void create_get_table_entry_cmd(struct pp_qos_dev *qdev, u32 phy,
				u32 table_type,
				union host_read_entry_t *read_entry,
				u32 entry_size);

void create_flush_queue_cmd(struct pp_qos_dev *qdev, u32 rlm);
void create_set_wsp_helper_cmd(struct pp_qos_dev *qdev,
			       struct wsp_helper_cfg *cfg);
#ifndef CONFIG_SOC_LGM
void create_get_wsp_helper_stats_cmd(struct pp_qos_dev *qdev,
				     bool reset, struct wsp_stats_t *stats);
#endif
void suspend_node(struct pp_qos_dev *qdev, u32 phy, u32 suspend);

void add_suspend_port(struct pp_qos_dev *qdev, u32 port);
void signal_uc(struct pp_qos_dev *qdev);
void signal_uc_clk_chng(struct pp_qos_dev *qdev);
void create_get_sys_info_cmd(struct pp_qos_dev *qdev,
			     struct system_stats_s *sys_info,
			     bool dump_entries);
void create_mod_log_bmap_set_cmd(struct pp_qos_dev *qdev, u32 bmap);
void create_mod_log_bmap_get_cmd(struct pp_qos_dev *qdev, u32 *bmap);
void create_aqm_q_to_ctx_cmd(struct pp_qos_dev *qdev, u32 act, u32 queue, u32 ctx);
void create_set_pci_addr_cmd(struct pp_qos_dev *qdev, struct pp_qos_pci_addr *pci_addr);
void create_set_aqm_engine_cmd(struct pp_qos_dev *qdev, u8 aqm_engine);
void print_fw_log(struct pp_qos_dev *qdev);
s32 qos_get_debug_stats(struct pp_qos_dev *qdev, debug_counters_t *dbg_stats);
s32 qos_get_aqm_info(struct pp_qos_dev *qdev, wred_aqm_db_t *aqm_db);
void fill_preds(const struct pp_nodes *nodes, u32 phy, u32 *preds, size_t size);
#elif defined(PRINT_CREATE_CMD)
#define create_move_cmd(qdev, dst, src, dst_port)                    \
	QOS_LOG_DEBUG("MOVE: %u ==> %u\n", src, dst)

#define create_set_port_cmd(qdev, conf, phy, modified)               \
	QOS_LOG_DEBUG("SET PORT: %u\n", phy)

#define create_set_sched_cmd(qdev, conf, node, modified)             \
	QOS_LOG_DEBUG("SET SCH\n")

#define create_add_sync_queue_cmd(qdev, conf, phy, rlm, modified)    \
	QOS_LOG_DEBUG("SET QUEUE: %u\n", phy)

#define create_set_queue_cmd(qdev, conf, phy, node, modified, is_orphaned) \
	QOS_LOG_DEBUG("SET QUEUE: %u\n", phy)

#define create_remove_node_cmd(qdev, type, phy, rlm)                 \
	QOS_LOG_DEBUG("REMOVE: %u(%u)\n", phy, type)

#define update_parent(qdev, phy)                                     \
	QOS_LOG_DEBUG("PARENT_CHANGE: %u\n", phy)

#define update_preds(qdev, phy, queue_port_changed)                  \
	QOS_LOG_DEBUG("UPDATE_PREDS: %u\n", phy)

#define create_init_qos_cmd(qdev)                                    \
	QOS_LOG_DEBUG("INIT QOS\n")

#define enqueue_cmds(qdev)                                           \
	QOS_LOG_DEBUG("ENQUEUE CMDS\n")

#define check_completion(qdev)                                       \
	QOS_LOG_DEBUG("CHECK COMPLITION\n")

#define print_fw_log(qdev)                                           \
	QOS_LOG_DEBUG("PRINT FW LOG\n")

#define init_fwdata_internals(qdev) 0
#define clean_fwdata_internals(qdev)

#define create_init_logger_cmd(qdev, level, mode)                    \
	QOS_LOG_DEBUG("INIT LOGGER\n")

#define create_add_shared_group_cmd(qdev, id, limit)                 \
	QOS_LOG_DEBUG("ADD SHARED GROUP: id %d, limit %d\n", id, limit)

#define create_set_shared_group_cmd(qdev, id, max_burst)                 \
	QOS_LOG_DEBUG("SET SHARED GROUP: id %d, limit %d\n", id, max_burst)

#define create_remove_shared_group_cmd(qdev, id)                     \
	QOS_LOG_DEBUG("REM SHARED GROUP: id %d\n", id)

#define create_get_queue_stats_cmd(qdev, phy, rlm, qstat, reset)\
	QOS_LOG_DEBUG("GET QUEUE STATS: %u\n", phy)

#define create_get_qm_stats_cmd(qdev, rlm, qstat)         \
	QOS_LOG_DEBUG("GET QM STATS\n")

#define create_aqm_sf_set_cmd(qdev, sf_id, enable, sf_cfg, queue_id) \
	QOS_LOG_DEBUG("SET AQM SF: %u\n", sf_id)

#define create_codel_cfg_set_cmd(qdev, cfg)                          \
	QOS_LOG_DEBUG("SET CODEL CFG\n")

#define create_get_codel_stats_cmd(qdev, rlm, qstat)           \
	QOS_LOG_DEBUG("GET CODEL QUEUE STATS: %u\n", rlm)

#define create_get_port_stats_cmd(qdev, phy, addr, pstat)            \
	QOS_LOG_DEBUG("GET PORT STATS: %u\n", phy)

#define create_get_hist_stats_cmd(qdev, sf_id, addr, reset)            \
	QOS_LOG_DEBUG("GET HIST STATS: %u\n", sf_id)

#define create_get_node_info_cmd(qdev, phy, info)              \
	QOS_LOG_DEBUG("GET NODE INFO: %u\n", phy)

#define create_get_table_entry_cmd(qdev, phy, addr, table_type)      \
	QOS_LOG_DEBUG("GET TABLE ENTRY: %u from phy %u\n",           \
	table_type, phy)

#define create_push_desc_cmd(qdev, queue, size, color, addr)         \
	QOS_LOG_DEBUG("PUSH DESC: size %u\n", size)

#define create_get_sys_info_cmd(qdev, sys_info, dump_entries)  \
	QOS_LOG_DEBUG("GET SYS STATS\n")

#define add_suspend_port(qdev, port)                                 \
	QOS_LOG_DEBUG("ADD SUSPEND PORT: %u\n", port)

#define create_flush_queue_cmd(qdev, rlm)                            \
	QOS_LOG_DEBUG("FLUSH QUEUE: rlm %u\n", rlm)

#define suspend_node(qdev, phy, suspend)                             \
	QOS_LOG_DEBUG("SUSPEND NODE: phy %u suspend %u\n", rlm, suspend)

#else
#define create_move_cmd(qdev, dst, src, dst_port)
#define create_set_port_cmd(qdev, conf, phy, modified)
#define create_set_sched_cmd(qdev, conf, node, modified)
#define create_add_sync_queue_cmd(qdev, conf, phy, rlm, modified)
#define create_set_queue_cmd(qdev, conf, phy, node, modified, is_orphaned)
#define create_remove_node_cmd(qdev, type, phy, rlm)
#define update_parent(qdev, phy)
#define update_preds(qdev, phy, queue_port_changed)
#define create_init_qos_cmd(qdev)
#define enqueue_cmds(qdev)
#define init_fwdata_internals(qdev) 0
#define clean_fwdata_internals(qdev)
#define print_fw_log(qdev)
#define create_init_logger_cmd(qdev, level, mode)
#define create_add_shared_group_cmd(qdev, id, limit)
#define create_set_shared_group_cmd(qdev, id, max_burst)
#define create_remove_shared_group_cmd(qdev, id)
#define create_get_queue_stats_cmd(qdev, phy, rlm, qstat, reset)
#define create_get_qm_stats_cmd(qdev, rlm, qstat)
#define create_aqm_sf_set_cmd(qdev, sf_id, enable, sf_cfg, queue_id)
#define create_codel_cfg_set_cmd(qdev, cfg)
#define create_get_codel_stats_cmd(qdev, rlm, qstat)
#define create_get_port_stats_cmd(qdev, phy, addr, pstat)
#define create_get_hist_stats_cmd(qdev, sf_id, addr, reset)
#define create_get_node_info_cmd(qdev, phy, info)
#define create_get_table_entry_cmd(qdev, phy, addr, table_type)
#define create_push_desc_cmd(qdev, queue, size, color, addr)
#define create_flush_queue_cmd(qdev, rlm)
#define create_get_sys_info_cmd(qdev, sys_info, dump_entries)
#define suspend_node(qdev, phy, suspend)
#define add_suspend_port(qdev, port)
#endif
#endif
