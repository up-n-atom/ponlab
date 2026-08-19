/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
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
 * Description: PP micro-controller internal definitions
 */

#ifndef __PP_UC_INTERNAL_H__
#define __PP_UC_INTERNAL_H__

#include <linux/types.h>
#include "uc_regs.h"
#include "uc_host_common.h"

#define UC_VER_MAJOR(t)               (t ## _VER_MAJOR)
#define UC_VER_MINOR(t)               (t ## _VER_MINOR)

extern u32 ing_addr_offset;
extern u32 ing_ldb1_offset;
extern u32 ing_ldb2_offset;
/* Ingress configuration. Must be aligned with the gdb structure in uc */
#define ING_VER_SSRAM_OFF                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ver))
#define ING_LDB1_SSRAM_OFF                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ldb1_offset))
#define ING_LDB2_SSRAM_OFF                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ldb2_offset))
#define ING_HOST_INFO_PORT_SSRAM_OFF                                       \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, host_port))
#define ING_HOST_INFO_QUEUE_SSRAM_OFF                                      \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, host_q))
#define ING_HOST_INFO_BASE_POLICY_SSRAM_OFF                                \
	(ing_addr_offset +                                               \
	 offsetof(struct ingress_uc_gdb, host_base_policy))
#define ING_HOST_INFO_POLICY_BMAP_SSRAM_OFF                                \
	(ing_addr_offset +                                               \
	 offsetof(struct ingress_uc_gdb, host_policies_bmap))
#define ING_FROM_PRSR_PKTS_CNT_BASE                                            \
	(ing_addr_offset +                                               \
	 offsetof(struct ingress_uc_gdb, from_parser_pkts))
#define ING_FROM_CHK_PKTS_CNT_BASE                                             \
	(ing_addr_offset +                                               \
	 offsetof(struct ingress_uc_gdb, from_checker_pkts))
#define ING_MBOX_MSG_BASE                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ing_cmd))
#define ING_MBOX_MSG_RSP                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ing_cmd.rsp_valid))
#define ING_MBOX_STAT_BASE                                                  \
	(ing_addr_offset + offsetof(struct ingress_uc_gdb, ing_mbox_stat))
/* UC Egress SW implementation for A-STEP */
/* CPU 3 DCCM address for the port credit inc address */
#define UC_SW_WRITER_TX_PORT_CRED(p)  (UC_DCCM(EGRESS, 3, (p) << 2))
/* CPU 3 DCCM address for the port ring address */
#define UC_SW_WRITER_TX_PORT_RING(p)  (0x50 + UC_DCCM(EGRESS, 3, (p) << 5))
/* DCCM address for triggering the command */
#define UC_SW_WRITER_MBOX_CH_TRIGGER  UC_SW_WRITER_TX_PORT_CRED(16)

#define DCCM_INGRESS_ADDR(cpu, offset) (UC_DCCM(INGRESS, cpu, offset))
#define ING_WL_OFFSET                                                          \
	(ing_ldb1_offset + offsetof(struct ingress_uc_local_db, wl_rules))
#define ING_GRP_INFO_BASE(grp_id)                                              \
	(ing_ldb1_offset + offsetof(struct ingress_uc_local_db, grp) +        \
	 (grp_id) * sizeof(struct uc_gpid_group))
#define ING_STATS_BASE                                              \
	(ing_ldb1_offset + offsetof(struct ingress_uc_local_db, stats))
#define ING_CHK_BASE                                                           \
	(ing_ldb1_offset + offsetof(struct ingress_uc_local_db, chk_base))
#define ING_DFLT_PORT_PRIORITY_OFF(port_id)                                    \
	(ing_ldb2_offset +                                                    \
	 offsetof(struct ingress_uc_local_db_2, dflt_port_priority[port_id]))
#define ING_HASH_BIT_DB_OFF(id)                                                \
	(ing_ldb2_offset +                                                     \
	 offsetof(struct ingress_uc_local_db_2, hb.hash_bit[id]))
#define ING_HASH_BIT_DB_BASE                                                   \
	(ing_ldb2_offset + offsetof(struct ingress_uc_local_db_2, hb))

#define DCCM_EGRESS_ADDR(cpu, offset) (UC_DCCM(EGRESS, cpu, offset))

/**
 * @brief for B-step the tx manager uc ports starts from port 16
 */
#define UC_B_TXMGR_PORT_OFF 4

#define UC_TASK_REGS_NUM    32U
#define UC_LOGGER_BUFF_SZ  (sizeof(struct uc_log_msg) * 128)

/**
 * @brief active bitmap for the egress cluster, pit per cpu
 */
extern ulong eg_cpu_act_bmap;
#define for_each_egr_active_cpu(cid)                                           \
	for_each_set_bit(cid, &eg_cpu_act_bmap, UC_CPUS_MAX)

/**
 * @brief active bitmap for the ingress cluster, pit per cpu
 */
extern ulong in_cpu_act_bmap;
#define for_each_ing_active_cpu(cid)                                           \
	for_each_set_bit(cid, &in_cpu_act_bmap, UC_CPUS_MAX)

/**
 * @enum uc_egr_port
 * @brief uc egress port type
 */
enum uc_egr_port {
	/*! mailbox port is valid only for B-step */
	UC_MBOX_PORT = 0,
	/*! tx manager ports starting from port 0 */
	/*! in B-step required UC_B_TXMGR_PORT_OFF offset */
	UC_MCAST_PORT = 0,
	UC_REASS_PORT,
	UC_FRAG_PORT,
	UC_TDOX_PORT,
	UC_IPSEC_LLD_PORT,
	UC_REMARK_PORT,
	UC_EGR_MAX_PORT,
};

/**
 * @brief init the uc egress mailbox module
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_mbox_init(void);

/**
 * @brief exit the uc egress mailbox module
 */
void uc_egr_mbox_exit(void);

/**
 * @brief Get egress writter port enable
 * @param port port id
 * @param en
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_writer_port_en_get(u32 port, bool *en);

/**
 * @brief Get the writer port and queue mapping
 *        for a specific CPU
 * @param cid cpu id
 * @param port port id
 * @param queue the mapped queue
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_writer_map_get(u32 cid, u32 port, u32 *queue);

/**
 * @brief Get the writer credits for a the queue
 *        which is mapped to 'port' on CPU 'cid'
 * @param cid cpu id
 * @param port port id
 * @param credit the credits count
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_writer_queue_credit_get(u32 cid, u32 port, u32 *credit);

/**
 * @brief Get the writer credits for a specific port
 * @param port port id
 * @param credit the credits count
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_writer_port_credit_get(u32 port, u32 *credit);

/**
 * @brief Get the program counter register of CPU's task
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param tid task index
 * @param pc the program counter
 * @return s32 0 on success, error code otherwise
 */
s32 uc_task_pc_get(bool uc_is_egr, u32 cid, u32 tid, u32 *pc);

/**
 * @brief Get the status32 register of CPU's task
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param tid task index
 * @param status the status32
 * @return s32 0 on success, error code otherwise
 */
s32 uc_task_status32_get(bool uc_is_egr, u32 cid, u32 tid, u32 *status);

/**
 * @brief Get the ready to run bit of CPU's task
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param tid task index
 * @param ready the ready bit
 * @return s32 0 on success, error code otherwise
 */
s32 uc_task_ready_get(bool uc_is_egr, u32 cid, u32 tid, bool *ready);

/**
 * @brief Get the status and queue of POPA for a CPU's task
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param tid task index
 * @param active if the task is waiting for POPA
 * @param queue for which queue the task is waiting
 * @return s32 0 on success, error code otherwise
 */
s32 uc_task_popa_active_get(bool uc_is_egr, u32 cid, u32 tid, bool *active,
			    u32 *queue);

/**
 * @brief Get CPU's task registers
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param tid task index
 * @param regs registers array
 * @param num number of registers to get, 0 till 'num'
 * @return s32 0 on success, error code otherwise
 */
s32 uc_task_regs_get(bool uc_is_egr, u32 cid, u32 tid, u32 *regs, u32 num);

/**
 * @brief check if cpu is valid
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @return bool true if valid, fasle otherwise
 */
bool uc_is_cpu_valid(bool uc_is_egr, u32 cid);

/**
 * @brief run/halt cpu
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param enable run/halt
 */
void uc_run_set(bool uc_is_egr, u32 cid, bool enable);

/**
 * @brief enable/disable the cpu in ccu (take the CPU out of
 *        reset and turn on the clock)
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @param enable enable/disable
 */
void uc_ccu_enable_set(bool uc_is_egr, u32 cid, bool enable);

/**
 * @brief get the ccu interrupt request register
 * @param uc_is_egr select the uc cluster
 * @param ier_bitmap interrupts bitmap
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ccu_irr_get(bool uc_is_egr, u32 *irr_bitmap);

/**
 * @brief get the cluster bridge fat entry value
 * @param uc_is_egr select the uc cluster
 * @param ent fat entry index
 * @param val fat entry value
 * @param enable fat entry state
 * @return s32 0 on success, error code otherwise
 */
s32 uc_fat_entry_get(bool uc_is_egr, u32 ent, u32 *val, bool *enable);

/**
 * @brief set the cluster bridge fat entry value
 * @param uc_is_egr select the uc cluster
 * @param ent fat entry index
 * @param val fat entry value
 * @param enable enable/disable the entry
 * @return s32 0 on success, error code otherwise
 */
s32 uc_fat_entry_set(bool uc_is_egr, u8 ent, u32 val, bool enable);

/**
 * @brief Translate a given address to a UC perspective address
 * @param uc_is_egr select the uc cluster
 * @param addr address to translate
 * @return u64 the translated address is succuss, addr untouched otherwise
 */
u64 uc_fat_addr_rev_trans(bool uc_is_egr, u64 addr);

/**
 * @brief Translate a given UC perspective address to a real address
 * @param uc_is_egr select the uc cluster
 * @param addr address to translate
 * @return u64 the translated address is succuss, addr untouched otherwise
 */
u64 uc_fat_addr_trans(bool uc_is_egr, u32 addr);

/**
 * @brief get the cluster reader interrupt request register
 * @param uc_is_egr select the uc cluster
 * @param irr_bitmap interrupt request register value
 * @return s32 0 on success, error code otherwise
 */
s32 uc_reader_irr_get(bool uc_is_egr, u32 *irr_bitmap);

/**
 * @brief get the uc fw version
 * @param uc_is_egr select the uc cluster
 * @param ver version
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ver_get(bool uc_is_egr, u32 *ver);

/**
 * @brief Get a copy of the uC logger buffer
 * @param buff pointer to copy the log buffer
 * @param sz buffer size in bytes
 * @param is_egr specify if uc is egress or ingress
 * @return s32 0 on success, error code otherwise
 */
s32 uc_log_buff_info_get(void *buff, size_t sz, bool is_egr);

/**
 * @brief Reset uC log buffer, this will delete all log messages
 * @param is_egr specify if uc is egress or ingress
 * @return s32 0 on success, error code otherwise
 */
s32 uc_log_reset(bool is_egr);

/**
 * @brief Set the uC logger level
 * @param level the level to set
 * @param is_egr specify if uc is egress or ingress
 * @return s32 0 on success, error code otherwise
 */
s32 uc_log_level_set(enum uc_log_level level, bool is_egr);

/**
 * @brief Read uC auxiliary register, for full register list, see
 *        Figure 3-5 in the ARCv2 Programmer's Reference Manual
 * @note this will read the register from the CPU which
 *       handles the mailbox messages
 * @param reg the register to read
 * @param val pointer to get the value back
 * @return s32 0 on success, error code otherwise
 */
s32 uc_aux_reg_read(u32 reg, u32 *val);

/**
 * @brief Write uC auxiliary register value, for full register list, see
 *        Figure 3-5 in the ARCv2 Programmer's Reference Manual
 * @note this will write to a register in the CPU which
 *       handles the mailbox messages
 * @param reg the register to read
 * @param val the value to write
 * @return s32 0 on success, error code otherwise
 */
s32 uc_aux_reg_write(u32 reg, u32 val);

/**
 * @brief Read ingress uC SRAM database
 * @param dst buffer
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_db_get(struct ingress_uc_gdb *dst);

/**
 * @brief Reset white list rule counter
 * @param rule_type rule type
 * @param rule_index rule index
 * @return int 0 on success, error code otherwie
 */
s32 uc_ing_whitelist_rule_hits_reset(unsigned int rule_type,
				     unsigned int rule_index);

/**
 * @brief Get egress/ingress cpu sleep cycles count
 * @param uc_is_egr select the uc cluster
 * @param cid cpu id
 * @return s32 sleep cycles count in case cid is valid, 0 otherwise
 */
u32 uc_cpu_sleep_count(bool uc_is_egr, unsigned int cid);

/**
 * @brief check if soft event was handled by uc
 * @param idx soft event index
 * @return true if handle false if not handled
 */
bool uc_ing_is_soft_event_handled_by_uc(u8 idx);

/**
 * @brief send soft event index to uc
 * @param idx soft event index to send
 */
void uc_ing_issue_soft_event_to_uc(u8 idx);

/**
 * @brief show mbox statistics from ingress UC
 * @param stat statistics structure to read
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_host_mbox_cmd_stats(struct ing_host_mbox_stat *stat);

/**
 * @brief Init gpid group info
 * @return s32 0 on success, error code otherwise
 */
s32 uc_gpid_group_init(void);

#ifdef CONFIG_DEBUG_FS
/**
 * @brief uc debug init
 * @return s32 0 on success, error code otherwise
 */
s32 uc_dbg_init(struct dentry *parent);

/**
 * @brief uc debug cleanup
 */
void uc_dbg_clean(void);

/**
 * @brief uc dut debug init
 * @return s32 0 on success, error code otherwise
 */
s32 uc_dut_dbg_init(struct dentry *parent);

/**
 * @brief uc dut debug cleanup
 */
void uc_dut_dbg_clean(void);

void uc_check_for_exception(void);
#else /* !CONFIG_DEBUG_FS */
	static inline s32 uc_dbg_init(struct dentry *parent)
{
	return 0;
}

static inline void uc_dbg_clean(void)
{
}
#endif /* CONFIG_DEBUG_FS */

#endif /* __PP_UC_INTERNAL_H__ */
