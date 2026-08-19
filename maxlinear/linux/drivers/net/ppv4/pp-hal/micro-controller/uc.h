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
 * Description: PP micro-comtroller definitions
 */

#ifndef __PP_UC_H__
#define __PP_UC_H__

#include <linux/types.h>
#include <linux/debugfs.h>      /* struct dentry */
#include <linux/pp_api.h>
#include "uc_host_common.h"

/**
 * @define used as parameter to the uc API for egress uc
 *         operations
 */
#define UC_IS_EGR        true

/**
 * @define used as parameter to the uc API for ingress uc
 *         operations
 */
#define UC_IS_ING        !UC_IS_EGR

/**
 * @struct uc_cpu_params
 */
struct uc_cpu_params {
	u32 max_cpu;               /*! number of cpus     */
	u32 cpu_prof[UC_CPUS_MAX]; /*! cpus profile array */
};

/**
 * @struct uc_egr_init_params
 */
struct uc_egr_init_params {
	bool   valid;
	struct uc_cpu_params cpus; /*! cpus information            */
	u64    uc_base;            /*! uc base address             */
	phys_addr_t chk_base;      /*! chk base address            */
	phys_addr_t bm_base;       /*! bm base address             */
	phys_addr_t qm_base;       /*! qm base address             */
	phys_addr_t wred_base;     /*! wred base address           */
	phys_addr_t client_base;   /*! wred client base address    */
	phys_addr_t txm_cred_base; /*! TX-MGR credit base address  */
	phys_addr_t cls_base;      /*! classifier base address     */
	phys_addr_t wred_cfg_base; /*! wred cfg base address       */
	phys_addr_t qos_misc_base; /*! qos uc misc base address    */
};

/**
 * @struct uc_ing_init_params
 */
struct uc_ing_init_params {
	bool   valid;
	struct uc_cpu_params cpus; /*! cpus information   */
	u64    uc_base;            /*! uc base address    */
};

struct pool_info {
	dma_addr_t addr;
	size_t sz;
};

struct uc_init_params {
	struct uc_ing_init_params ing;
	struct uc_egr_init_params egr;
	struct dentry *dbgfs;

	size_t      ppv4_regs_sz;
	phys_addr_t ppv4_base;
	/*! Per BMGR pool address info to mapped into the UC FAT table */
	struct pool_info pools[PP_BM_MAX_POOLS];
};

#ifdef CONFIG_SOC_LGM
/**
 * @define maximum of supported turbodox sessions
 */
#define UC_MAX_TDOX_SESSIONS  (512)

/**
 * @enum tdox uc states
 */
enum tdox_uc_state {
	TDOX_UNUSED,
	/*! Newly created */
	TDOX_NEW,
	/*! Session tagged as qualified but not flowing through uc */
	TDOX_QUALIFIED,
	/*! Session tagged as qualified and flowing through uc */
	TDOX_NON_QUALIFIED,
};

/**
 * @struct aqm_lld_stats
 * @brief host only - struct used to store the global lld statistics
 *	(aggregated for all the SFs + rx and ctx error counters)
 */
struct aqm_lld_global_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! Context error packet counter */
	u64 ctx_error_pkt;
	/*! general error packet counter */
	u64 error_pkt;
	/*! lld counters aggregated for all the SFs */
	struct lld_sf_stats lld_aggr;
	/*! aqm counters aggregated for all the SFs */
	struct aqm_sw_sf_stats aqm_sw_aggr;
};

/**
 * @enum uc_mbox_set
 * @brief mailbox set, 2 sets of mailbox registers are supported
 */
enum uc_mbox_set {
	/*! mailbox registers set 0 */
	UC_EGR_MBOX0,
	/*! TODO: not supported in driver */
	UC_EGR_MBOX1,
};

/**
 * @brief Check whether gpid group id is valid
 * @param id id to check
 * @return bool true if it is, false otherwise
 */
bool __uc_gpid_group_id_is_valid(u32 id);

/**
 * @brief Check whether gpid group id is valid and prints
 *        an error message if not
 * @param id id to check
 * @return bool true if it is, false otherwise
 */
bool uc_gpid_group_id_is_valid(u32 id);

/**
 * @brief Check whether gpid group id is valid and prints
 *        an error message if not
 * @param prio priority to check
 * @return bool true if it is, false otherwise
 */
bool uc_gpid_group_priority_is_valid(u32 prio);

/**
 * @brief get the maximum supported cpus in cluster
 * @param uc_is_egr select the uc cluster
 * @param max_cpus max supported cpus
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ccu_maxcpus_get(bool uc_is_egr, u8 *max_cpus);

/**
 * @brief get the reassembly packet counters per uc CPU
 * @param cid cpu index
 * @param stats reassembly statistics
 * @return s32 0 on success, error code otherwise
 */
s32 uc_reass_cpu_stats_get(u32 cid, struct reassembly_stats *stats);

/**
 * @brief Get the total multicast statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_mcast_stats_get(struct mcast_stats *stats);

/**
 * @brief Get the total lld statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_lld_total_stats_get(struct aqm_lld_global_stats *stats);

/**
 * @brief Get the lld statistics per SF
 * @param lld_ctx
 * @param sf_stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_lld_per_sf_stats_get(u8 lld_ctx, struct lld_sf_stats *sf_stats);

/**
 * @brief Get the sw aqm statistics per sf
 * @param sf_indx
 * @param sf_tats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_aqm_sw_per_sf_stats_get(u8 sf_indx, struct aqm_sw_sf_stats *sf_stats);

/**
 * @brief Get the total ipsec statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ipsec_stats_get(struct ipsec_stats *stats);

/**
 * @brief Get the total reassembly statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_reass_stats_get(struct reassembly_stats *stats);

/**
 * @brief Get the total fragmentation statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_frag_stats_get(struct frag_stats *stats);

/**
 * @brief Get the total remarking statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_remark_stats_get(struct remarking_stats *stats);

/**
 * @brief Get the total lro statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_lro_stats_get(struct lro_stats *stats);

/**
 * @brief Get the tdox statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_tdox_stats_get(struct tdox_uc_stats *stats);

/**
 * @brief Get the egress uc global statistics, which is not
 *        related to a specific NF
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_global_stats_get(struct egr_glb_stats *stats);

/**
 * @brief reset UC tdox statistics
 * @param NONE
 * @return s32 0 on success, error code otherwise
 */
s32 uc_tdox_stats_reset(void);

/**
 * @brief set the LLD context information
 * @param cfg LLD context configuration
 * @return s32 0 on success, error code otherwise
 */
s32 uc_lld_ctx_set(struct lld_ctx_cfg *cfg);

/**
 * @brief set the AQM context information
 * @param cfg AQM context configuration
 * @return s32 0 on success, error code otherwise
 */

s32 uc_aqm_ctx_set(struct aqm_ctx_cfg *cfg);
/**
 * @brief Get LLD Histogram
 * @param ctx LLD Context
 * @param hist Histogram pointer
 * @param reset reset histogram after retrieving
 * @return s32 0 on success, error code otherwise
 */
s32 uc_sf_hist_get(u8 ctx, struct pp_sf_hist_stat *hist, bool reset);

/**
 * @brief set the LLD Allowed AQ
 * @param cmd LLD Allowed AQ cmd
 * @return s32 0 on success, error code otherwise
 */
s32 uc_lld_allowed_aq_set(struct lld_allowed_aq_set_cmd *cmd);

/**
 * @brief Get the ingress uc statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ingress_stats_get(struct ing_stats *stats);

/**
 * @brief reset ingress uc statistics
 * @param NONE
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ingress_stats_reset(void);

/**
 * @brief send a mailbox command to the egress(only) uc
 * @param type command type
 * @param param command parameter (optional)
 * @param in_buf buffer to send (optional)
 * @param in_len in_buf size in bytes
 * @param out_buf buffer to send (optional)
 * @param out_len out_buf size in bytes
 * @return s32 0 on success, error code otherwise
 */
s32 uc_egr_mbox_cmd_send(enum uc_mbox_cmd_type type, u32 param,
			 const void *in_buf, u16 in_len,
			 void *out_buf, u16 out_len);

/**
 * @brief allocate a DDR buffer for mailbox command
 * @param phys allocated buffer physical address
 * @param sz buffer size
 * @return void* allocated buffer cpu address, NULL if failed
 */
void *uc_egr_mbox_buff_alloc(dma_addr_t *phys, size_t sz);

/**
 * @brief free the mailbox command DDR buffer
 * @param virt buffer cpu address
 * @param phys buffer physical address
 * @param sz buffer size
 */
void uc_egr_mbox_buff_free(void *virt, dma_addr_t phys, size_t sz);

/**
 * @brief set uc network function parameters
 * @param nf network function type
 * @param pid port id (phy)
 * @param subif DP subif
 * @param qos_port qos port id (phy)
 * @param tx_queue nf tx queue (phy)
 * @param uc_q uc queue (phisical queue)
 * @param dflt_hif default host interface to use for diverting
 *        packets to the host
 * @param data optional data that relevant to the nf
 * @return s32 0 on success, error code otherwise
 */
s32 uc_nf_set(enum pp_nf_type nf, u16 pid, u16 subif, u16 qos_port,
	      u16 tx_queue, u16 uc_q, u16 dflt_hif, void *data);

/**
 * @brief check if cpu is active
 * @param uc_is_egr select the uc cluster
 * @param cid cpu index
 * @return bool true if valid, fasle otherwise
 */
bool uc_is_cpu_active(bool uc_is_egr, u32 cid);

/**
 * @brief check if cluster is valid (at least 1 cpu is valid)
 * @param uc_is_egr select the uc cluster
 * @return bool true if valid, fasle otherwise
 */
bool uc_is_cluster_valid(bool uc_is_egr);

/**
 * @brief get ccu GP register value
 * @param uc_is_egr select the uc cluster
 * @param idx gp register index
 * @param gpreg gp register value
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ccu_gpreg_get(bool uc_is_egr, u32 idx, u32 *gpreg);

/**
 * @brief set ccu GP register value
 * @param uc_is_egr select the uc cluster
 * @param idx gp register index
 * @param gpreg gp register value
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ccu_gpreg_set(bool uc_is_egr, u32 idx, u32 gpreg_val);

/**
 * @brief Create GPID group in ingress uC FW
 * @note all cpu entries MUST be set
 * @param grp_id group id
 * @param cpu cpus info
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_gpid_group_create(u32 grp_id,
			     struct ing_host_cpu_info cpu[PP_MAX_HOST_CPUS]);

/**
 * @brief Delete GPID group from ingress uC FW
 * @param grp_id group id
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_gpid_group_delete(u32 grp_id);

/**
 * @brief Set lowest priority for gpid port
 * @param id port id
 * @param prio priority
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_gpid_group_dflt_prio_port_set(u32 id, u32 prio);

/**
 * @brief Enable/Disable rule for a gpid group
 * @param grp_id group index
 * @param type rule type
 * @param idx rule index
 * @param en enable/disable
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_gpid_group_rule_set(u32 grp_id, enum ing_wl_rule_type type, u32 idx,
			       bool en);

/**
 * @brief Enable rule for gpid group
 * @param g group id
 * @param t rule type
 * @param i rule index
 * @return s32 0 on succuss, error code otherwise
 */
static inline s32 uc_ing_gpid_group_rule_en(u32 g, enum ing_wl_rule_type t,
					    u32 i)
{
	return uc_ing_gpid_group_rule_set(g, t, i, true);
}

/**
 * @brief Disable rule for gpid group
 * @param g group id
 * @param t rule type
 * @param i rule index
 * @return s32 0 on succuss, error code otherwise
 */
static inline s32 uc_ing_gpid_group_rule_dis(u32 g, enum ing_wl_rule_type t,
					     u32 i)
{
	return uc_ing_gpid_group_rule_set(g, t, i, false);
}

/**
 * @brief Test whether a rule is enabled for a group
 * @param grp_id the group id
 * @param type rule type
 * @param idx rule index
 * @return bool true if it is enabled, false otherwise
 */
bool uc_ing_gpid_group_rule_is_en(u32 grp_id, enum ing_wl_rule_type type,
				  u32 idx);

/**
 * @brief Set gpid group gpid for specific host CPU
 * @param grp_id group id
 * @param host_cid host cpu id
 * @param prio priority
 * @param gpid gpid
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_gpid_group_gpid_set(u32 grp_id, u32 host_cid, u32 prio, u32 gpid);

/**
 * @brief Set gpid group queue id for specific host CPU
 * @param grp_id group id
 * @param host_cid host cpu id
 * @param prio priority
 * @param phy_qid physical queue id
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_gpid_group_queue_set(u32 grp_id, u32 host_cid, u32 prio,
				u32 phy_qid);

/**
 * @brief Return rule type string description
 * @param type rule type
 * @return const char*
 */
const char *uc_wl_rule_type_name(enum ing_wl_rule_type type);

/**
 * @brief Get GPID group info
 * @param grp_id group id
 * @param grp buffer to copy into
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_gpid_group_info_get(u32 grp_id, struct uc_gpid_group *grp);

/**
 * @brief Enable/disable bit hash logic for a group
 * @param grp_id group id
 * @param en enable/disable
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_gpid_group_hash_bit_set(u32 grp_id, bool en);

/**
 * @brief Test whether a bit hash is enabled for a group
 * @param grp_id group id
 * @return bool true if it is enabled, false otherwise
 */
bool uc_ing_gpid_group_hash_bit_is_en(u32 grp_id);

/**
 * @brief reset all rules in DB, used mostly for debug
 * @return none
 */
void uc_ing_whitelist_reset_all_rules(void);

/**
 * @brief rules test function used for debug purpose only
 * @return none
 */
void uc_ing_whitelist_rules_test(void);

/**
 * @brief Add a new white list rule to the ingress uC
 * @param rule the rule to add
 * @param type rule type
 * @param idx the index the rule was stored in
 * @return int 0 on success, error code otherwise
 */
s32 uc_ing_whitelist_rule_add(union ing_wl_rule_union *rule_data,
			      enum ing_wl_rule_type type, u32 *idx);
/**
 * @brief Delete an existing white list rule from the ingress uC
 * @param type rule type
 * @param idx rule index to delete
 * @return int 0 on success, error code otherwise
 */
s32 uc_ing_whitelist_rule_del(enum ing_wl_rule_type type, u32 idx);

/**
 * @brief Read ingress uC whitelist database
 * @param dst buffer
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_whitelist_db_get(struct ing_wl_rules *dst);

/**
 * @brief Find and return whitelist rule id in the uC database
 * @param rule the rule data to search for
 * @param rule_type the rule type to search for
 * @param id the rule id if it is exist in the DB
 * @return bool true if rule exist in DB, false otherwise
 */
bool uc_is_whitelist_rule_exist(union ing_wl_rule_union *rule_data,
				enum ing_wl_rule_type rule_type, u32 *id);
/**
 * @brief Get whitelist rule hit counter from ingress DB
 * @param rule_type the rule type to search for
 * @param rule_index rule index in rule type DB
 * @param hits_val the returned hits value
 * @return s32 0 on succuss, error code otherwise
 */
s32 uc_ing_whitelist_rule_hits_get(unsigned int rule_type,
				  unsigned int rule_index,
				  unsigned int *hits_val);

/**
 * @brief Resets the uc statistics
 */
void uc_stats_reset(void);

/**
 * @brief hash bit table get DB
 * @param dst array to copy into
 * @param cnt number of elements to copy
 */
s32 uc_ing_hash_bit_db_get(struct hash_bit_db *dst, u32 cnt);

/**
 * @brief enable bit in hash bit table
 */
s32 uc_ing_hash_bit_enable(u32 index);

/**
 * @brief  disable bit in hash bit table
 */
s32 uc_ing_hash_bit_disable(u32 index);

/**
 * @brief reset Hash bit table
 */
s32 uc_ing_hash_bit_reset(void);

/**
 * @brief initialized the uc module
 * @param init_params initial parameters
 * @return s32 0 on success, error code otherwise
 */
s32 uc_init(struct uc_init_params *init_params);

/**
 * @brief send mbox command to ingress UC
 * @param msg message structure to send
 * @param wait_for_done wait untill command is processed by UC
 * @return s32 0 on success, error code otherwise
 */
s32 uc_ing_host_mbox_cmd_send(struct uc_ing_cmd *msg, bool wait_for_done);

/**
 * @brief reset egress tdox stats
 * @return s32 0 on success, error code otherwise
 */
void uc_egr_tdox_stats_reset(u32 dccm_addr, size_t size);

/**
 * @brief Get egress uc cnt64 timers ticks configuration register
 *        physical address
 * @return dma_addr_t
 */
dma_addr_t uc_egr_cnt64_tmr_ticks_phys_get(void);

/**
 * @brief Get egress uc tdox timers ticks configuration register
 *        physical address
 * @return dma_addr_t
 */
dma_addr_t uc_egr_tdox_tmr_ticks_phys_get(void);

/**
 * @brief Get egress uc LLD AQM info phyisical address
 * @return dma_addr_t
 */
dma_addr_t uc_egr_lld_aqm_info_phys_addr_get(void);

/**
 * @brief exit the uc module
 */
void uc_exit(void);
#else
static inline s32 uc_init(struct uc_init_params *init_params)
{
	if (init_params->egr.valid || init_params->ing.valid)
		return -EINVAL;
	else
		return 0;
}

static inline void uc_exit(void)
{
}
#endif /* CONFIG_SOC_LGM */
#endif /* __PP_UC_H__ */
