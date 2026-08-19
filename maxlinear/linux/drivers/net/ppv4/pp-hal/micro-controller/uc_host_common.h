/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
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
 * Description: PP micro-controller and HOST CPU common definitions
 */

#ifndef __PP_UC_HOST_COMMON_H__
#define __PP_UC_HOST_COMMON_H__

/* UC version - version major and minor numbers must be aligned with
 * the fw version, if the fw was changed without any interface changes
 * only the build number should be changed and no need to update
 * the version here
 */
#define EGRESS_VER_MAJOR  1
#define EGRESS_VER_MINOR  48

#define INGRESS_VER_MAJOR 1
#define INGRESS_VER_MINOR 26

/* some attributes shortcuts */
#ifndef __packed
#define __packed __attribute__((packed))
#endif
#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif
#ifndef __packed_aligned
#define __packed_aligned(x) __attribute__((packed, aligned(x)))
#endif

#define TDOX_CPU (2)
#define TDOX_MAX_NON_AGGRESSIVE_SESSIONS (32)

/* TDOX default configurations */
#define TDOX_TIMER_TIMEOUT_USEC     (1000)
#define TDOX_TIMER_MAX_TIMEOUT_USEC (5000)
#define TDOX_MAX_REACH_TARGET       (250)
#define TDOX_MAX_SUPP_RATIO         (30)
#define TDOX_MAX_SUPP_BYTES         (1460 * 2 * TDOX_MAX_SUPP_RATIO)

/* AQM/LLD related */
#define MAX_LLD_CONTEXT    8
#define MAX_AQM_CONTEXT    PP_QOS_MAX_SERVICE_FLOWS

/* General */
#define UC_CPUS_MAX        (4)
#define UC_CPUS_REASS_MAX  (2)

/* Enable this define both on Host & Fw side to debug WL_RULES */
// #define DEBUG_WL_RULES_TIME

/**
 * @define CMD_DONE_REG_IDX
 * @brief CCU GP register index for the mailbox command done
 *        signal
 */
#define CMD_DONE_REG_IDX  2

/**
 * @define CCU_GDB_SRAM_OFF_IDX
 * @brief CCU GP register index for the egress GDB address
 */
#define CCU_GDB_SRAM_OFF_IDX 5

/**
 * @define CCU_LLD_AQM_INFO_IDX
 * @brief CCU GP register index for the LLD AQM info
 */
#define CCU_LLD_AQM_INFO_IDX 6

/**
 * @define CCU_DB_ADDR_IDX
 * @brief CCU GP register index for the ingress DB address
 */
#define CCU_DB_ADDR_IDX 7

/**
 * @define BUFF_EX_FLAG, to be used with struct buffer_ex_info.flags si_ud_reass_info.flags
 */
#define BUFF_EX_FLAG_WR_DESC           (BIT(0))
#define BUFF_EX_FLAG_PREL2_EN          (BIT(1))
/**
 * @define BUFF_EX_FLAG, to be used with struct si_ud_reass_info.flags
 * NOTE - si_ud_reass_info.flags also includes buffer_ex_info.flags!
 */
#define REASS_INFO_FLAG_EXT_REASS      (BIT(2))
#define REASS_INFO_FLAG_INT_REASS      (BIT(3))
#define REASS_INFO_FLAG_SGC_VALID      (BIT(4))

/**
 * @enum uc_mbox_cmd_type
 * @brief mailbox command type
 */
enum uc_mbox_cmd_type {
	UC_CMD_FIRST,
	/*! synchronized the host with the mbox queue and cpu id */
	UC_CMD_SYNC = UC_CMD_FIRST,
	/*! send init info to UC */
	UC_CMD_INIT,
	/*! get the egress uc version */
	UC_CMD_VERSION,
	/*! reset the UC logger */
	UC_CMD_LOGGER_RESET,
	/*! set UC logger level */
	UC_CMD_LOGGER_LEVEL_SET,
	/*! set the multicast pp port id */
	UC_CMD_MCAST_PID,
	/*! set the multicast pp rx queue id (second cycle) */
	UC_CMD_MCAST_QUEUE,
	/*! set the multicast group bitmap */
	UC_CMD_MCAST_DST,
	/*! Set the multicast uc to work in session mirroring mode */
	UC_CMD_MCAST_SESS_MIRRORING,
	/*! IPSeC info information */
	UC_CMD_IPSEC_INFO,
	/*! set the reassembly info */
	UC_CMD_REASSEMBLY_INFO,
	/*! set reassembly context timeout */
	UC_CMD_REASSEMBLY_TIMEOUT_THR,
	/* read UC AUX register value */
	UC_CMD_AUX_REG_RD,
	/* write UC AUX register value */
	UC_CMD_AUX_REG_WR,
	/*! Set LRO configuration */
	UC_CMD_LRO_CONF_SET,
	/*! Get LRO configuration */
	UC_CMD_LRO_CONF_GET,
	/*! Create Tdox record */
	UC_CMD_TDOX_CREATE,
	/*! Remove Tdox record */
	UC_CMD_TDOX_REMOVE,
	/*! Get Tdox record */
	UC_CMD_TDOX_STATS,
	/*! Checker mailbox */
	UC_CMD_CHK_MBX,
	/*! Classifier mailbox */
	UC_CMD_CLS_MBX,
	/* TDOX conf parameters set */
	UC_CMD_TDOX_CONFIG_SET,
	/* TDOX conf parameters get */
	UC_CMD_TDOX_CONFIG_GET,
	/* LLD conf set */
	UC_CMD_LLD_CTX_CFG_SET,
	/* AQM conf set */
	UC_CMD_AQM_CTX_CFG_SET,
	/* LLD allowed AQ set */
	UC_CMD_LLD_ALLOWED_AQ_SET,
	/* AQM LLD hist get */
	UC_CMD_AQM_LLD_HIST_GET,

	UC_CMD_LAST = UC_CMD_AQM_LLD_HIST_GET,

	UC_CMD_CNT,
	UC_CMD_MAX  = U32_MAX,
};

/**
 * @enum mbox_cmd_err
 * @brief mailbox command error code
 */
enum mbox_cmd_err {
	MBOX_CMD_SUCCESS,
	MBOX_CMD_BUSY,
	MBOX_CMD_UNSUPPORTED,
	MBOX_CMD_INVALID_PARAM,
	MBOX_CMD_INVALID_LEN,
	MBOX_CMD_CNT
};

/**
 * @brief UC FAT table Definitions
 * The UC FAT table translate 32 bits addresses to 36 bits addresses
 * The resolution is 0x4000000 (bit 26)
 * The actual translation is replacing bits 31-26 from the 32 bits address
 * with 10 bits from the fat table entry
 */
#define UC_FAT_ENT_CNT               48
#define UC_FAT_BASE_ENT_CNT          8
#define UC_FAT_ALL_ENT_CNT           (UC_FAT_BASE_ENT_CNT+UC_FAT_ENT_CNT)
#define UC_FAT_ENT_WIN_SZ            BIT(26)

/**
 * @enum uc_log_level
 * @brief logger log level
 */
enum uc_log_level {
	UC_LOG_FATAL,
	UC_LOG_ERROR,
	UC_LOG_WARN,
	UC_LOG_INFO,
	UC_LOG_DEBUG,
	UC_LOG_LEVELS_NUM
};

/**
 * @struct uc_log_msg
 * @brief UC log message
 * @note keep the structure aligned to power of 2
 */
struct uc_log_msg {
	u32  val;
	u32  ts;
	u8   level;
	u8   cid:3,
	     tid:5;
	char str[38];
	char func[16];
};

/**
 * @brief Logger database
 */
struct logger_db {
	/*! logs buffer base address */
	struct uc_log_msg *msgs;
	/*! next empty message index */
	u16 next_idx;
	/*! last message index */
	u16 max_idx;
	/*! log level */
	u8  level;
};

/*! Derived directly from number of actual buffer pools */
#define UC_MAX_BMGR_POOLS 16
#ifndef RX_DMA_MAX_POOL_SIZE_TYPE
#define RX_DMA_MAX_POOL_SIZE_TYPE 5
#endif

/**
 * @struct eg_uc_init_info
 * @brief UC init info
 */
struct eg_uc_init_info {
	/*! logger buffer base address  */
	u64 logger_buff;
	/*! logger buffer size in bytes */
	u32 logger_buff_sz;
	/* checker base address */
	u64 chk_base;
	/* classifier base address */
	u64 cls_base;
	/* wred_cfg base address */
	u64 wred_cfg_base;
	/* qos misc base address */
	u64 qos_misc_base;
	struct __packed {
		/*! original pool base address */
		u64 base;
		u32 sz; /* just for verification */
	} pool[UC_MAX_BMGR_POOLS];
	/*! Policy buffer sizes */
	u16 buffer_size[RX_DMA_MAX_POOL_SIZE_TYPE];
} __packed;

/**
 * @struct mbox_aux_reg_wr
 * @brief UC Mailbox command intended to aux register
 */
struct mbox_aux_reg_wr {
	/*! aux register */
	u32 reg;
	/*! register value */
	u32 val;
};

/**
 * @struct uc_chk_cmd
 * @brief UC Mailbox command intended to checker
 */
struct uc_chk_cmd {
	/*! session id */
	u32 id;
	/*! checker mailbox command */
	u32 cmd;
};

/**
 * @brief SI size in 32bit words granularity
 */
#define _PP_SI_WORDS_CNT                 (64)

/**
 * @struct uc_cls_cmd
 * @brief UC Mailbox command intended to classifier
 */
struct uc_cls_cmd {
	/*! classifier mailbox command */
	u32 cmd_id;
	/*! Hash1 */
	u32 h1;
	/*! hash2 */
	u32 h2;
	/*! sig */
	u32 sig;
	/*! session id + bucket offset + hash table + flags */
	u32 cmd_info;
	/*! DSI update info */
	u32 dsi_info;
	/*! SI (optional) */
	u32 si[_PP_SI_WORDS_CNT];
};

/**
 * @struct mcast_stats
 */
struct mcast_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! drop packet counter */
	u64 drop_pkt;
	/*! Mirror TX packet counter */
	u64 mirror_tx_pkt;
	/*! Mirror drop packet counter */
	u64 mirror_drop_pkt;
};

/**
 * @struct ipsec_stats
 */
struct ipsec_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! error packet counter */
	u64 error_pkt;
};

/**
 * @struct lld_sf_stats
 */
struct lld_sf_stats {
	/*! RX total packet counter */
	u64 rx_pkt;
	/*! RX ECT0 packet counter */
	u64 rx_ect0_pkt;
	/*! RX ECT1 packet counter */
	u64 rx_ect1_pkt;
	/*! RX CE packet counter */
	u64 rx_ce_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! mark packet counter */
	u64 mark_pkt;
	/*! saction packet counter */
	u64 sanction_pkt;
	/*! drop packet counter */
	u64 drop_pkt;
};

/**
 * @struct aqm_sw_sf_stats
 */
struct aqm_sw_sf_stats {
	/*! rx packet counter */
	u64 rx_pkt;
	/*! tx packet counter */
	u64 tx_pkt;
	/*! bc drop packet counter */
	u64 bc_drop_pkt;
	/*! aqm drop packet counter */
	u64 aqm_drop_pkt;
};

/**
 * @struct aqm_lld_stats
 */
struct aqm_lld_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! Context error packet counter */
	u64 ctx_error_pkt;
	/*! general error packet counter */
	u64 error_pkt;
	/*! lld stats per sf */
	struct lld_sf_stats lld_sf[MAX_LLD_CONTEXT];
	/*! aqm stats per sf */
	struct aqm_sw_sf_stats aqm_sf[MAX_AQM_CONTEXT];
};

/**
 * @struct smgr_reass_uc_cpu_stats
 * @brief reassembly UC cpu statistics
 */
struct reassembly_stats {
	u64 rx_pkts;
	u64 tx_pkts;
	u64 reassembled;
	u64 accelerated;
	u64 diverted;
	u64 early_diverted;
	u64 matched;
	u64 not_matched;
	struct {
		u64 cntxs_starv;
		u64 cntxs_busy;
		u64 cntx_overflow;
		u64 timedout;
		u64 timeout_err;
		u64 frags_starv;
		u64 duplicates;
		u64 unsupported_frag;
		u64 unsupported_ext;
		u64 unsupported_proto;
		u64 unsupported_pmac;
		u64 unsupported_pre_l2;
		u64 ext_overflow;
		u64 invalid_len;
		u64 dropped;
		u64 bm_null_buff;
	} err;
};

/**
 * @struct frag_stats
 */
struct frag_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! Drop packet counter */
	u64 total_drops;
	/*! bmgr drops */
	u64 bmgr_drops;
	/*! don't frag drops */
	u64 df_drops;
	/*! More frags required than supported */
	u64 max_frags_drops;
};

/**
 * @struct remarking_stats
 */
struct remarking_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! remarked packet counter */
	u64 remarked_pkt;
	/*! error packet counter */
	u64 error_pkt;
};

/**
 * @struct LRO stats
 */
struct lro_stats {
	/*! RX packet counter */
	u64 rx_pkt;
	/*! TX packet counter */
	u64 tx_pkt;
	/*! aggregated packet counter */
	u64 agg_pkt;
	/*! Exception packet counter */
	u64 exp_pkt;
	/*! Drop packet counter */
	u64 drop_pkt;
	/*! Error packet counter */
	u64 error_pkt;
};

#define MAX_STATS_CNT 256
#define STAT_CNT_SIZE sizeof(u64)

/**
 * @struct egress cpu uc statistical 64 bit counters
 */
struct egress_cpu_stats {
	struct reassembly_stats reass;
};

struct egr_glb_stats {
	u64 null_job_null_buf;
	u64 bm_buf_rev_oob;
	u64 bm_buf_oob;
};

/**
 * @struct egress uc statistical 64 bit counters - up to 256 counters in total
 */
struct egress_stats {
	struct egress_cpu_stats cpu[UC_CPUS_REASS_MAX];
	struct ipsec_stats      ipsec;
	struct mcast_stats      mcast;
	struct frag_stats       frag;
	struct remarking_stats  remark;
	struct aqm_lld_stats    aqm_lld;
	struct lro_stats        lro;
	struct egr_glb_stats    glb;
};

/**
 * @brief Reassembly network function parameters for the FW
 */
struct __packed reassembly_info {
	/*! Host port headroom */
	u16 host_port_hr;
	/*! Host port tailroom */
	u16 host_port_tr;
	/*! host base policy */
	u8  host_base_policy;
	/*! host policies bitmap */
	u8  host_policies_bmap;
	/*! host pid */
	u8  host_pid;
	/*! host port flags. To be used with struct si_ud_reass_info.flags */
	u8  host_port_flags;
	/*! host queue to send packets to host upon failure */
	u16 host_q;
	/*! 2nd round queue */
	u16 pp_rx_q;
	/*! si base address, translated according to the FAT by the driver */
	u64 si_base;
};

/**
 * @brief LLD network function info
 */
struct lld_info {
	u16 nf_queue;
} __aligned((4));

/**
 * @brief IPSeC network function info
 */
struct ipsec_info {
	u16 tx_gpid;
	u16 tx_q;
	u16 tx_subif;
	u16 vpn_gpid;
} __aligned((4));

/**
 * @brief IPSeC & LLD common network function info
 */
struct ipsec_lld_info {
	struct ipsec_info ipsec;
	struct lld_info lld;
} __aligned((4));

/**
 * @brief LSO network function info
 */
struct lro_info {
	/* lro egress port */
	u32 pid;
	/*! lro dest host q */
	u16 tx_q;
	/*! max aggregated bytes */
	u16 max_agg_bytes;
	/*! max aggregated packets */
	u16 max_agg_pkts;
	/*! lro port headroom */
	u16 headroom;
	/*! lro port tailroom */
	u16 tailroom;
	/*! lro host subif */
	u16 subif;
	/*! lro pool policy */
	u16 policy;
	/*! lro buffer pool */
	u16 pool;
} __aligned((4));

/**
 * @brief Tdox uc create command
 */
struct tdox_record_create_cmd {
	u16 low_queue;
	u16 high_queue;
	u16 sess_id;
	u16 tdox_id;
	u16 nf_q;
	u16 ts_flag;
	u8 is_docsis;
	u8 lro_flag;
};

/**
 * @brief Tdox conf parameters
 */
struct tdox_cfg {
	/* Timeout in usec */
	u32 timeout;
	/* Max reach target */
	u32 max_reach_target;
	/* Max suppression ratio */
	u32 max_supp_ratio;
	/* Max suppression payload bytes */
	u32 max_supp_bytes;
};

struct tdox_uc_stats {
	u32 dccm_addr;
	u32 stage1;
	u32 stage2;
	u32 stored;
	u32 forward;
	u32 aggressive;
	u32 low_timers;
	u32 high_timers;
	u32 expired;
	u32 create;
	u32 remove;
	u32 recycle;
	u32 recycle_next_op;
	u32 errors;
	u32 max_ratio;
	u32 max_supp;
	u32 max_reach_target;
	u32 timer_cancel;
	u32 s2_fwd1;
	u32 s2_fwd2;
	u32 set_timer;
};

/**
 * @brief Tdox uc remove command
 */
struct tdox_record_remove_cmd {
	u16 tdox_id;
	u16 lro_flag;
};

/**
 * @brief mcast uc session mirroring
 */
struct mcast_sess_mirroring_cmd {
	u16 enable;
	u16 orig_q;
	u16 q;
	u16 gpid;
	u32 ps0;
	u16 ps_valid;
	u16 eg_port_hr;
	u16 eg_port_tr;
	u16 flags;
	u16 base_policy;
	u8  policies_map;
};

#define MAX_BINS    (16)

/**
 * @brief histogram configuration common for both lld and aqm
 */
struct __packed hist_cfg {
	/*! Histogram - Num bins. Set 0 to disable Histogram */
	u32    num_hist_bins;
	/*! Histogram - Bin edges */
	u32    bin_edges[MAX_BINS - 1];
};

/**
 * @brief LLD context parameters configuration
 */
struct __packed lld_cfg {
	/* Valid Context */
	u16    valid;
	/* LLD Context ID */
	u8     lld_ctx_id;
	/* SF ID */
	u8     sf_id;
	/* Max rate (bps). 0 means no limit */
	u64    max_rate;
	/* buffer size */
	u32    buffer_size;
	/* Coupled SF */
	u8     coupled_sf;
	/* IAQM enable */
	u8     iaqm_en;
	/* Queue Protection enable */
	u8     qp_en;
	/* LG Aging */
	u8     lg_aging;
	/* 1 << LG range in ns */
	u32    range_ns;
	/* Min threshold in ns */
	u32    minth_ns;
	/* Max threshold in ns */
	u32    maxth_ns;
	/* Critical QL in ns */
	u32    critical_ql_ns;
	/* Critical QL Product */
	u64    critical_qL_product;
	/* VQ interval */
	u32    vq_interval;
	/* EWMA Alpha */
	u32    vq_ewma_alpha;
};

/**
 * @brief LLD context configuration
 */
struct __packed lld_ctx_cfg {
	struct lld_cfg lld_cfg;
	struct hist_cfg hist_cfg;
};

/**
 * @brief AQM context parameters configuration
 */
struct __packed aqm_cfg {
	/* Valid Context */
	u32 valid;

	/* AQM SF id */
	u32 sf_id;

	/*! AQM Latency Target for this Service Flow (milliseconds) */
	u32 latency_target_ms;

	/*! Service Flow configured Peak Rate, expressed in Bytes/sec */
	u32 peak_rate;

	/*! Service Flow configured MaxSustained Rate expressed in Bytes/sec */
	u32 msr;

	/*! SF buffer size for AQM alog, expressed in bytes*/
	u32 buffer_size;
};

/**
 * @brief AQM context configuration
 */
struct __packed aqm_ctx_cfg {
	struct aqm_cfg aqm_cfg;
	struct hist_cfg hist_cfg;
};

/**
 * @brief LLD Allowed AQ Set command
 */
struct lld_allowed_aq_set_cmd {
	/* Context ID */
	u32    ctx;
	/* Allowed AQ */
	u32    allowed_aq;
};

struct aqm_lld_hist_get_in_cmd {
	/* Context ID */
	u32 ctx;
	/* reset */
	u32 reset;
};

struct aqm_lld_hist_get_out_cmd {
	/* Num histogram bins */
	u32 num_hist_bins;
	/* Histogram counter per bin */
	u32 hist_counter[MAX_BINS];
	/* Number of total updates */
	u32 hist_updates;
	/* maximum latency */
	u32 max_latency;
};

/*************************************/
/** Ingress uC Specific Definitions **/
/*************************************/

#define UC_ING_MAX_HOST_CPU    UC_CPUS_MAX
#define UC_WHITELIST_MAX_RULES 32
/* equal to PP_MAX_CPU_QUEUES define in pp_api.h file */
#define UC_WHITELIST_NUM_PRIO 8
#define UC_WHITELIST_LOWEST_PRIO (UC_WHITELIST_NUM_PRIO - 1)
#define UC_GPID_GRP_CNT 8
#define UC_ETH_ALEN 6
#define UC_IPV6_WLEN 4
/* equal to PP_DPL_HASH_BIT_MAX_ENTRIES define in pp_api.h file */
#define UC_HASH_BIT_MAX_ENTRIES_SUPPORTED (2048)
#define UC_HASH_BIT_MAX_WORDS (UC_HASH_BIT_MAX_ENTRIES_SUPPORTED / 32)

/**
 * @brief Enumeration for DPL white list fields to use
 * for the enable bitmap
 */
enum whitelist_fields {
	UC_WL_FLD_SRC_MAC,
	UC_WL_FLD_DST_MAC,
	UC_WL_FLD_ETH_TYPE,
	UC_WL_FLD_IPV4_SRC_IP,
	UC_WL_FLD_IPV4_DST_IP,
	UC_WL_FLD_IPV6_SRC_IP,
	UC_WL_FLD_IPV6_DST_IP,
	UC_WL_FLD_IP_PROTO,
	UC_WL_FLD_L4_SRC_PORT,
	UC_WL_FLD_L4_DST_PORT,
	UC_WL_FLD_COUNT
};

/**
 * @brief Host CPU Info
 */
struct ing_host_cpu_info {
	/*! all entries MUST be valid always to simplify uC work */
	u16 qid[UC_WHITELIST_NUM_PRIO];
	u8 gpid[UC_WHITELIST_NUM_PRIO];
};

/**
 * @brief Ingress statistics
 */
struct ing_stats {
	u32 rx_checker;
	u32 rx_parser;
	u32 rx_host;
	u32 rx_frag;
	u32 rx_frag_eg;
	u32 rx_wl;
	u32 rx_lro;
};

/**
 * @brief White list Database
 */
struct hash_bit_db {
	u32 bitHashFound;
	u32 bitHashNotFound;
	u32 priorityHigherThenBitHash;
	/* hash bit enable configured by Host */
	u32 hash_bit[UC_HASH_BIT_MAX_WORDS];
};

enum uc_ing_soft_event {
	ING_SOFT_EVENT_MBOX = 8,
	ING_SOFT_EVENT_MAX = 32
};

enum ing_mbox_msg_type {
	ING_MBOX_INVALID,
	ING_MBOX_MSG_1,
	ING_MBOX_WL_ADD_RULE,
	ING_MBOX_WL_DEL_RULE,
	ING_MBOX_WL_RESET_RULES,
	ING_MBOX_WL_TEST_RULES,
	ING_MBOX_WL_GRP_RULE_EN,
	ING_MBOX_WL_GRP_RULE_DIS,
	ING_MBOX_WL_GRP_BHASH_EN,
	ING_MBOX_WL_GRP_BHASH_DIS,
	ING_MBOX_LOGGER_INIT,
	ING_MBOX_LOGGER_RESET,
	ING_MBOX_LOGGER_LEVEL_SET,
	ING_MBOX_LRO_PORT_SET,
	ING_MBOX_MSG_MAX
};

enum ing_wl_rule_type {
	ING_WL_RULE_TYPE_INVALID = 0,
	ING_WL_RULE_TYPE_IP_MAC = 1,
	ING_WL_RULE_TYPE_MAC = 2,
	ING_WL_RULE_TYPE_ETH_TYPE = 3,
	ING_WL_RULE_TYPE_IP_PROTO_PORTS = 4,
	ING_WL_RULE_TYPE_ICMP = 5,
	ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS = 6,
	ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS = 7,
	ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS = 8,
	ING_WL_RULE_TYPE_BITHASH = 9,
	ING_WL_RULE_TYPE_COUNT
};

#define WL_RULE_IP_AND_MAC_MAX_RULES			(8)
#define WL_RULE_MAC_MAX_RULES				(8)
#define WL_RULE_ETH_MAX_RULES				(8)
#define WL_RULE_IP_PROTO_PORTS_MAX_RULES		(8)
#define WL_RULE_ICMP_MAX_RULES				(8)
#define WL_RULE_IPV4_SRC_OR_DST_AND_L4_PORTS_MAX_RULES	(4)
#define WL_RULE_IPV4_SRC_AND_DST_AND_L4_PORTS_MAX_RULES	(2)
#define WL_RULE_IPV6_SRC_OR_DST_AND_L4_PORTS_MAX_RULES	(4)
/* KEEP this as the maximum from all the above max rules type */
#define ING_WL_RULE_MAX_RULES (8)

/* equal to PP_MAX_PORT define in pp_api.h file */
#define UC_WHITELIST_MAX_PORTS (256)

struct wl_rule_common {
	u32 hits;
	u16 priority;
	u16 fields_en;
} __packed_aligned(4);

struct wl_rule_ip_mac {
	struct wl_rule_common common;
	union {
		__be32 v6[UC_IPV6_WLEN]; /*! ipv6 source or dest address */
		__be32 v4; /*! ipv4 source or dest address */
	} ip;
	u8 mac[UC_ETH_ALEN]; /*! source or dest MAC address !*/
} __packed_aligned(4);

struct wl_rule_mac {
	struct wl_rule_common common;
	u8 mac[UC_ETH_ALEN]; /*! source or dest mac address !*/
} __packed_aligned(4);

struct wl_rule_eth {
	struct wl_rule_common common;
	__be16 eth_type; /*! Ethertype !*/
} __packed_aligned(4);

struct wl_rule_ip_proto_ports {
	struct wl_rule_common common;
	__be16 src_port; /*! optional: L4 source port */
	__be16 dst_port; /*! optional: L4 destination port */
	u8 ip_proto; /*! mandatory: next protcol after L3, UDP/TCP etc. */
} __packed_aligned(4);

struct wl_rule_icmp {
	struct wl_rule_common common;
	u8 icmp_type;
	u8 icmp_code;
} __packed_aligned(4);

struct wl_rule_ipv4_ports {
	struct wl_rule_common common;
	__be32 v4; /*! ipv4 source or destination address */
	__be16 src_port; /*! L4 source port */
	__be16 dst_port; /*! L4 destination port */
	u8 ip_proto; /*! next protcol after L3, UDP/TCP etc. */
} __packed_aligned(4);

struct wl_rule_double_ipv4_ports {
	struct wl_rule_common common;
	__be32 src; /*! ipv4 source address */
	__be32 dst; /*! ipv4 destination address */
	__be16 src_port; /*! L4 source port */
	__be16 dst_port; /*! L4 destination port */
	u8 ip_proto; /*! next protcol after L3, UDP/TCP etc. */
} __packed_aligned(4);

struct wl_rule_ipv6_ports {
	struct wl_rule_common common;
	__be32 v6[UC_IPV6_WLEN]; /*! ipv6 source or destination address */
	__be16 src_port; /*! L4 source port */
	__be16 dst_port; /*! L4 destination port */
	u8 ip_proto; /*! next protcol after L3, UDP/TCP etc. */
} __packed_aligned(4);

struct ing_wl_rules {
	u16 wl_rule_type_bit_enabled;
	u16 wl_rule_index_enabled[ING_WL_RULE_TYPE_COUNT];
	struct wl_rule_ip_mac rule_ip_mac[WL_RULE_IP_AND_MAC_MAX_RULES];
	struct wl_rule_mac rule_mac[WL_RULE_MAC_MAX_RULES];
	struct wl_rule_eth rule_eth[WL_RULE_ETH_MAX_RULES];
	struct wl_rule_ip_proto_ports rule_ip_proto_ports[WL_RULE_IP_PROTO_PORTS_MAX_RULES];
	struct wl_rule_icmp rule_icmp[WL_RULE_ICMP_MAX_RULES];
	struct wl_rule_ipv4_ports rule_ipv4_ports[WL_RULE_IPV4_SRC_OR_DST_AND_L4_PORTS_MAX_RULES];
	struct wl_rule_double_ipv4_ports rule_double_ipv4_ports[WL_RULE_IPV4_SRC_AND_DST_AND_L4_PORTS_MAX_RULES];
	struct wl_rule_ipv6_ports rule_ipv6_ports[WL_RULE_IPV6_SRC_OR_DST_AND_L4_PORTS_MAX_RULES];
#ifdef DEBUG_WL_RULES_TIME
	u32 rule_time[ING_WL_RULE_TYPE_COUNT];
#endif
};

union ing_wl_rule_union {
	struct wl_rule_ip_mac rule_ip_mac;
	struct wl_rule_mac rule_mac;
	struct wl_rule_eth rule_eth;
	struct wl_rule_ip_proto_ports rule_ip_proto_ports;
	struct wl_rule_icmp rule_icmp;
	struct wl_rule_ipv4_ports rule_ipv4_ports;
	struct wl_rule_double_ipv4_ports rule_double_ipv4_ports;
	struct wl_rule_ipv6_ports rule_ipv6_ports;
	struct wl_rule_common rule_common;
};

struct ing_host_mbox_stat {
	u32 msg_recv_cnt;
	u32 msg_recv_err;
	u32 msg_recv_type[ING_MBOX_MSG_MAX];
} __packed;

struct ing_cmd_1 {
	u32 param[2]; /*! params */
} __packed;

struct ing_mbox_cmd_add_rule {
	u32 rule_type; /*! rule type  */
	u32 rule_index; /*! rule index within specific type */
	union ing_wl_rule_union rule_data; /*! param2 */
} __packed;

struct ing_mbox_cmd_del_rule {
	u32 rule_type; /*! rule type  */
	u32 rule_index; /*! rule index within specific type */
} __packed;

struct ing_mbox_cmd_logger_init {
	u64 buff; /*! logger buffer base */
	u32 sz;   /*! buffer size */
} __packed;

struct ing_mbox_cmd_logger_level {
	enum uc_log_level level;
} __packed;

struct ing_mbox_cmd_grp_rule_set {
	u32 rule_type; /*! rule type */
	u32 rule_index; /*! rule index within specific type */
	u32 grp_id; /*! gpid group index */
} __packed;

struct ing_mbox_cmd_lro_port_set {
	u32 port;
	u32 policy;
	u32 ssb_pkt_sz;
} __packed;

union uc_ing_cmd_u {
	struct ing_cmd_1 cmd_1;
	struct ing_mbox_cmd_add_rule add_rule;
	struct ing_mbox_cmd_del_rule del_rule;
	struct ing_mbox_cmd_logger_init log_init;
	struct ing_mbox_cmd_logger_level log_lvl;
	struct ing_mbox_cmd_grp_rule_set grp_rule;
	struct ing_mbox_cmd_lro_port_set lro_cfg;
} __packed;

struct uc_ing_cmd {
	u32 rsp_valid;
	u32 msg_type;
	union uc_ing_cmd_u ing_cmd;
} __packed;

struct ingress_uc_gdb {
	/*! fw version - MUST BE THE FIRST MEMBER */
	u32 ver;
	/* ldb local db 1 offset */
	u32 ldb1_offset;
	/* ldb local db 2 offset */
	u32 ldb2_offset;
	/*! Host port */
	u32 host_port;
	/*! Host queue */
	u32 host_q;
	/*! Host base policy */
	u32 host_base_policy;
	/*! Host policies bmap */
	u32 host_policies_bmap;
	/* soft lro port */
	u32 lro_port;
	/* soft lro ssb policy */
	u32 lro_policy;
	/* Max pkt size for ssb buf */
	u32 ssb_buf_pkt_sz;
	/* host mbox statistics */
	struct ing_host_mbox_stat ing_mbox_stat;
	/* ing host command */
	struct uc_ing_cmd ing_cmd;
	/* looger DB */
	struct logger_db logger;
};

struct uc_gpid_group {
	/*! bitmap specifying which rule types are enabled for the group */
	u16 wl_rule_type_bmap_en;
	/*! per rule type bitmap specifying which rule is enable for
	 *  the group
	 */
	u16 wl_rule_index_bmap_en[ING_WL_RULE_TYPE_COUNT];
	/*! per cpu info for the group, all entries MUST be valid to simplify
	 *  uC work
	 */
	struct ing_host_cpu_info cpu[UC_ING_MAX_HOST_CPU];
} __packed_aligned(4);

struct ingress_uc_local_db {
	/* ingress statistics */
	struct ing_stats stats;
	/*! whitelist rules DB */
	struct ing_wl_rules wl_rules;
	/*! GPID groups info */
	struct uc_gpid_group grp[UC_GPID_GRP_CNT];
	/*! checker registers base */
	u32 chk_base;
};

struct ingress_uc_local_db_2 {
	/* lowest priority per port. relevant when the port is in GPID group */
	u8 dflt_port_priority[UC_WHITELIST_MAX_PORTS];
	/* hash bit support */
	struct hash_bit_db hb;
};

#endif /* __PP_UC_HOST_COMMON_H__ */
