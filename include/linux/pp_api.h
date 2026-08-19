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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP APIs and definitions
 */

#ifndef __PP_API_H__
#define __PP_API_H__

#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/pp_buffer_mgr_api.h>
#include <linux/pp_qos_api.h>

#ifdef CONFIG_SOC_LGM
#include <linux/pktprs.h>
#endif

/**
 * @file
 * @brief Packet Processor API
 */

/**
 * @define PP descriptor number of L3 offsets
 */
#define PP_DESC_NUM_OF_L3_OFF      (6)

/**
 * @define PP number of ports
 */
#define PP_MAX_PORT                (256)
#define PP_PORT_INVALID            (U16_MAX)

/**
 * @define Max aggregation SF
 */
#define PP_MAX_ASF  (8)

/**
 * @define number of maximum session group counters assigned to
 *  a session
 */
#define PP_SI_SGC_MAX              (8)
#define PP_SGC_INVALID             (U16_MAX)

/**
 * @brief Definition of maximum number of SGC groups owners
 */
#define PP_SGC_MAX_OWNERS          (PP_SI_SGC_MAX + 1)

/**
 * @brief Definition of invalid owner to use for allocating
 *        SGCs from un-reserved groups
 */
#define PP_SGC_SHARED_OWNER        PP_SI_SGC_MAX

/**
 * @define number of maximum traffic bucket meters assigned to a
 *  session
 */
#define PP_SI_TBM_MAX              (5)
#define PP_TBM_INVALID             (U16_MAX)

/**
 * PP Colors
 */
#define PP_COLOR_INVALID           (0)
#define PP_COLOR_GREEN             (1)
#define PP_COLOR_YELLOW            (2)
#define PP_COLOR_RED               (3)
#define PP_COLOR_MAX               (4)

/**
 * @define number of BM policies supported by PP port
 */
#define PP_POLICY_PER_PORT         (5)

/**
 * @define size of the template UD in bytes
 *         the template UD is being copied from
 *         STW to the buffer per user request
 *         for a specific session
 */
#define PP_TEMPLATE_UD_SZ          32

/**
 * @enum pp_min_tx_pkt_len
 * @brief minimum packet size
 */
enum pp_min_tx_pkt_len {
	PP_MIN_TX_PKT_LEN_NONE,
#define PP_MIN_TX_PKT_LEN_VAL_NONE (0)
	PP_MIN_TX_PKT_LEN_60B,
#define PP_MIN_TX_PKT_LEN_VAL_60B  (60)
	PP_MIN_TX_PKT_LEN_64B,
#define PP_MIN_TX_PKT_LEN_VAL_64B  (64)
	PP_MIN_TX_PKT_LEN_128B,
#define PP_MIN_TX_PKT_LEN_VAL_128B (128)
	PP_NUM_TX_MIN_PKT_LENS
};

/**
 * @enum
 * @brief host interface number of datapath egress
 */
enum {
	PP_HOSTIF_1EG = 1,  /*!< 1 egress --> 1 queue and 1 port */
	PP_HOSTIF_2EG,      /*!< 2 egress --> 2 queues and 2 ports */
	PP_HOSTIF_EG_ERROR, /*!< 3 egress --> not allowed */
	PP_HOSTIF_4EG,      /*!< 4 egress --> 4 queues and 4 ports */
	PP_HOSTIF_EG_MAX = PP_HOSTIF_4EG,
};

/**
 * @define Max Status word port classification field bit
 * @brief defines the last bit that can be extracted from the stw for
 *        port classification
 */
#define PP_STW_CLS_MAX_BIT         (127)

/**
 * @struct pp_version
 */
struct pp_version {
	u32 major;     /*! major version number */
	u32 major_mid; /*! major-mid version number */
	u32 mid;       /*! mid version number */
	u32 minor_mid; /*! minor-mid version number */
	u32 minor;     /*! minor version number */
};

/**
 * @enum pp_version_type
 */
enum pp_version_type {
	PP_VER_TYPE_DRV,
	PP_VER_TYPE_FW,
	PP_VER_TYPE_HW,
};

/* Active queue management engine mode */
enum pp_aqm_engine {
	PP_AQM_SW = 0,
	PP_AQM_HW = 1
};

/**
 * @struct pp_pkt_ud
 * @brief pp decriptor packet user-define host structure
 */
struct pp_pkt_ud {
	u32 sess_id;     /*! session id                   */
	u32 hash_sig;   /*! hash signature result        */
	u32 hash_h1;    /*! hash H1 result               */
	u32 hash_h2;    /*! hash H2 result               */
	u8 is_exc_sess; /*! if exception session         */
	u8 rx_port;     /*! rx port id                   */
};

/**
 * @struct pp_desc
 * @brief pp decriptor host structure
 */
struct pp_desc {
	u64 ps:48,           /*! protocol specific, up to 48bits         */
	    data_off:9,      /*! data_pointer = buff_ptr + data_off      */
	    color:2,         /*! color at egress                         */
	    src_pool:4;      /*! from which pool the buffer was taken    */
	u64 buff_ptr;        /*! buffer pointer                          */
	u8  tx_port;         /*! Egress Port @ RXDMA output              */
	u8  bm_policy;       /*! from which policy the buffer was taken  */
	u16 pkt_len:14,      /*! packet length. Defaults incl preL2 & ts */
	    lsp_pkt:1;       /*! last slow path packet, used for syncq   */
	struct pp_pkt_ud ud; /*! packet user define block                */
};

/**
 * @struct pp_stw_to_fv_copy
 * @brief PP status word to fv copy information, used for port
 * @note only fields from the first 128 bits of the stw can be used
 */
struct pp_stw_to_fv_copy {
	u8 stw_off; /*! source bit in stw (in bits), up to 128 bits */
	u8 copy_size; /*! number of bits to copy, up to 15 bits*/
};

/**
 * @struct pp_port_cls_info
 * @brief port classification information
 * @details The parser has the capability of coping information that
 *          is stored in the first 128 bits of the Status Word (STW)
 *          to the first four bytes of the FV. only bits 16-31 can be
 *          used for user specific classification per port.
 * @note maximum of all fields length cannot exceeds 16 bits
 */
struct pp_port_cls_info {
	u8 n_flds; /*! number of fields to copy into the fv */
	struct pp_stw_to_fv_copy cp[4]; /*! structure with copy information */
};

/**
 * @struct pp_port_cls_data
 * @brief port classification data definition Defines stw fields
 *        data that will be written the a newly created session
 *        field vector for port based classification
 */
struct pp_port_cls_data {
	/*! number of valid fields in the fields array */
	u8 n_flds;

	/*! the actual data that is expected to be on the status */
	/*! word, up to 4 fields allowed */
	u16 fld_data[4];
};

/**
 * @enum pp_port_parsing
 * @brief PP parsing types
 */
enum pp_port_parsing {
	L2_PARSE, /*!< packet parsing start from layer 2 */
	IP_PARSE, /*!< packet parsing start from ip layer (ipv6 or ipv4) */
	NO_PARSE  /*!< no parsing */
};

/**
 * @struct pp_port_rx_info
 */
struct pp_port_rx_info {
	/*! port spacial classification info */
	struct pp_port_cls_info cls;

	/*! if set this port is a memory port (0 for streaming port) */
	u8  mem_port_en;

	/*! if set this port is support flow control */
	u8  flow_ctrl_en;

	/*! port's policies bitmap specifying which policy is enabled (1)
	 *  or disabled (0), valid policies are only bits 0-3,
	 *  zero bit map or if one of the bits 4-7 is set is considered
	 *  an invalid bitmap
	 */
	u8  policies_map;

	/*! if set - no packet parsing */
	enum pp_port_parsing parse_type;
};

/**
 * @struct pp_port_tx_info
 */
struct pp_port_tx_info {
	/*! maximum packet size used for packets MTU at egress stage,
	 *  for getting fragmentation decisions (including L2 header size)
	 */
	u16 max_pkt_size;

	/*! this determines the starting offset for the packet write
	 *  in a new buffer before sending packet to egress peripherals
	 */
	u16 headroom_size;

	/*! when selecting a buffer size RXDMA will make sure there are
	 *  at least Tailroom bytes from the end of the packet
	 *  (incl. timestamp) to the end of the buffer
	 */
	u16 tailroom_size;

	/*! minimum packet length, 1 of 4 options
	 *  defined by enum pp_min_tx_pkt_len
	 */
	enum pp_min_tx_pkt_len min_pkt_len;

	/*! base policy used by the rx dma */
	u16 base_policy;

	/*! port's policies bitmap specifying which policy is enabled (1)
	 *  or disabled (0), valid policies are only bits 0-4,
	 *  zero bit map or if one of the bits 5-7 is set is considered
	 *  an invalid bitmap
	 */
	u8  policies_map;

	/*! if set, the RXDMA will report data offset and packet len to
	 * include the packet only, even if PreL2 and/or timestamp exists
	 */
	bool pkt_only_en;

	/*! if set this port is allowed to use internal buffers
	 *  (used for streaming ports)
	 */
	bool seg_en;

	/*! if set this port can receive prel2 data */
	bool prel2_en;

	/*! if set, the RXDMA will copy the descriptor to the buffer */
	bool wr_desc;
};

/**
 * @struct pp_port_cfg
 * @brief pp port configuration
 */
struct pp_port_cfg {
	struct pp_port_rx_info rx; /*! port RX info */
	struct pp_port_tx_info tx; /*! port TX info */
};

/**
 * @struct pp_stats
 * @brief pp packets and bytes statistics
 */
struct pp_stats {
	u64 packets; /*! number of packets */
	u64 bytes; /*! number of bytes */
	u64 ing_dropped_bytes; /* number of ingress dropped bytes */
	u64 egr_dropped_bytes; /* number of egress dropped bytes */
	u64 ing_dropped_packets; /* number of ingress dropped packets */
	u64 egr_dropped_packets; /* number of egress dropped packets */
};

/**
 * @enum session_stats_op
 * @brief Session's statistics operations
 */
enum pp_stats_op {
	/*! decrement stats */
	PP_STATS_SUB,
	/*! increment stats */
	PP_STATS_ADD,
	/*! reset stats */
	PP_STATS_RESET,
	PP_STATS_OP_CNT
};

#define PP_STATS_OP_STR(op) \
	(op == PP_STATS_SUB   ? "decrement" : \
	(op == PP_STATS_ADD   ? "increment" : \
	(op == PP_STATS_RESET ? "reset"     : \
	("invalid"))))

struct pp_hal_stats {
	u64 rx_inline_packets; /*! pp inline RX packets */
	u64 tx_inline_packets; /*! pp inline TX packets */
	u64 drop_inline_packets; /*! pp inline DROP packets */
};

/**
 * @struct pp_hash
 * @brief pp hash results
 */
struct pp_hash {
	u32 h1;  /*! hash1 function result */
	u32 h2;  /*! hash2 function result */
	u32 sig; /*! signature function result */
};

/**
 * @struct pp_egress
 */
struct pp_egress {
	u16 pid;   /*! pp port id */
	u16 qos_q; /*! pp qos queue (logical) id */
};

/**
 * @struct pp_hif_datapath
 * @brief PP Host interface datapath information
 */
struct pp_hif_datapath {
	/*! color to set on descriptor */
	u8  color;

	/*! sgc optional, sgc id's */
	u16 sgc[PP_SI_SGC_MAX];

	/*! tbm optional, tbm id's */
	u16 tbm[PP_SI_TBM_MAX];

	/*! up to 4 egress (logical queue and pp port), set unused
	 * queues to PP_QOS_INVALID_ID and unused ports to PP_PORT_INVALID
	 */
	struct pp_egress eg[PP_HOSTIF_EG_MAX];
};

/**
 * @struct pp_hif_class
 * @brief PP Host interface classification information
 */
struct pp_hif_class {
	u16 port; /*! PP ingress port ID */
	u8  tc_bitmap; /*! 4bit, each bit represent ingress TC value */
};

/**
 * @struct pp_hostif_cfg
 * @brief PP Host interface information
 */
struct pp_hostif_cfg {
	struct pp_hif_class    cls; /*! classification information */
	struct pp_hif_datapath dp; /*! datapath to CPU information */
};

/**
 * @brief Defines the packet length types to consider for TBM calculations
 *
 */
enum pp_tbm_len_type {
	/*! new packet length, after modification */
	PP_TBM_NEW_LEN,
	/*! original packet length, before modification */
	PP_TBM_ORG_LEN,
	PP_TBM_LEN_TYPES_NUM
};

/**
 * @brief TBM work mode definition
 * @note Color blind mode is where the session's color defined in the SI
 *       is taken into considerations to determine  the final color for
 *       the packets where in color blind it is not.
 * @see RFC4115 and RFC2698 for more details
 */
enum pp_tbm_mode {
	PP_TBM_MODE_FIRST,
	/*! RFC 4115 Color blind mode */
	PP_TBM_MODE_4115_CB = PP_TBM_MODE_FIRST,
	/*! RFC 4115 Color aware mode */
	PP_TBM_MODE_4115_CA,
	/*! RFC 2698 Color blind mode */
	PP_TBM_MODE_2698_CB,
	/*! RFC 2698 Color aware mode */
	PP_TBM_MODE_2698_CA,
	PP_TBM_MODE_LAST = PP_TBM_MODE_2698_CA,
	PP_TBM_MODES_NUM
};

/**
 * @struct pp_dual_tbm
 * @brief pp dual token bucket meter configuration
 *        Roughly, cir and pir supported ranges are:
 *          1. 50MHz-600MHz: 512bps-100Gbps
 *          2. 11MHz       : 512bps-22Gbps
 *          3. 5Mhz        : 512bps-10Gbps
 *          4. 1MHz        : 512bps-2Gbps
 * @note to implement single TBM set the pir and pbs to 0
 */
struct pp_dual_tbm {
	/*! specify if the TBM should be enable or not */
	bool enable;
	/*! packet length type to use for calculations */
	enum pp_tbm_len_type len_type;
	/*! color decision algorithm */
	enum pp_tbm_mode mode;
	/*! committed information rate, bytes per second */
	u64 cir;
	/*! committed burst size, bytes per second */
	u32 cbs;
	/*! peak information rate, bytes per second */
	u64 pir;
	/*! peak burst size, bytes per second */
	u32 pbs;
};

/**
 * @enum pp acceleration mode.
 * @NORMAL acceleration is supported only in known and specific use cases
 *         (IPv4/IPv6+UDP/TCP/ESP).
 * @UNKNOWN_L4 in case L4 is not UDP/TCP/ESP,
 *             create a session with classification up to L3.
 * @UNKNOWN_L3 in case L3 is not IPv4/IPv6,
 *             create a session with classification up to L2.
*/
enum pp_accl_mode {
	PP_ACCL_MODE_FIRST,
	PP_ACCL_MODE_NORMAL = PP_ACCL_MODE_FIRST,
	PP_ACCL_MODE_UNKNOWN_L4,
	PP_ACCL_MODE_UNKNOWN_L3,
	PP_ACCL_MODE_NUM,
};

/**
 * @define the maximum number of supported multicast groups
 */
#define PP_MCAST_GRP_MAX (512)
/**
 * @define the maximum number of supported multicast
 *         destinations
 */
#define PP_MCAST_DST_MAX (8)

/**
 * @struct pp_mcast
 * @brief pp multicast session information
 */
struct pp_mcast {
	u16 grp_idx; /*! multicast group index       */
	u8  dst_idx; /*! multicast destination index */
};

/**
 * @struct pp_mcast_grp_info
 * @brief Used for mapping the multicast groups to pp sessions
 */
struct pp_mcast_grp_info {
	u32 base_sess;                  /*! session base (first cycle)   */
	ulong dst_bmap;                 /*! active clients bitmap        */
	u32 dst_sess[PP_MCAST_DST_MAX]; /*! dst sessions (second cycle)  */
};

/**
 * @define Session flag specifing if sync queue is required
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_SYNCQ_BIT                  (0)

/**
 * @define Session flag specifying if MTU should be checked prior
 *        to modification
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_MTU_CHCK_BIT               (1)

/**
 * @define Session flag specifying if PP should ignore internal
 *        L3 don't frag flag and always fragment big packets in
 *        the session
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_IGNORE_INT_DFRAG_BIT       (2)

/**
 * @define same as 'IGNORE_INT_DFRAG' just for external L3
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_IGNORE_EXT_DFRAG_BIT       (3)

/**
 * @define Specify if PS should be copied from STW to the buffer
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_PS_COPY_BIT                (4)

/**
 * @define Specify if the session is a new multicast group
 *         session (mcast session 1st round)
 * @note to use with (struct pp_sess_create_args).flags, user
 *       must specify the group index in the (struct
 *       pp_sess_create_args).mcast parameter
 */
#define PP_SESS_FLAG_MCAST_GRP_BIT              (5)

/**
 * @define Specify if the session is a new multicast destination
 *         session (mcast session 2nd round)
 * @note to use with (struct pp_sess_create_args).flags, user
 *       must specify the group index and destination index in
 *       the (struct pp_sess_create_args).mcast parameter
 */
#define PP_SESS_FLAG_MCAST_DST_BIT              (6)

/**
 * @define Specify if the LRO info is valid and should be used
 * @note when LRO info is used the session cannot be classified
 *       for TDOX
 */
#define PP_SESS_FLAG_LRO_INFO_BIT               (7)

/**
 * @define Specify if Turbodox prioritization should be used for this session
 * @note dst_q_high must be configured
 */
#define PP_SESS_FLAG_TDOX_PRIO_BIT              (8)
/**
 * @define Specify if Turbodox ack suppression should be used for this session
 * @note dst_q_high must be configured
 */
#define PP_SESS_FLAG_TDOX_SUPP_BIT              (9)

/**
 * @define Specify if hash will be calculated internally
 * @note Step B Only!
 */
#define PP_SESS_FLAG_INTERNAL_HASH_CALC_BIT     (10)

/**
 * @define Specify if PS is valid
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_PS_VALID_BIT                (11)

/**
 * @define Specify the session is ESP transport tunnel
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_ESP_TRANSPORT_BIT           (12)

/**
 * @define Force reassembly support on outer IP header in the session
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_OUTER_REASS_FORCE           (13)

/**
 * @define Specify if dscp remarking should be used for this session
 * @note to use with remark_dscp param
 *       to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_REMARK_BIT                  (14)

/**
 * @define Specify the session is vpn bypass 
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_VPN_BIT                     (15)

/**
 * @define Specify if session is LLD session
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_LLD_BIT                     (16)

/**
 * @define Specify if it's docsis ports. relevant when
 *         PP_SESS_FLAG_TDOX_SUPP_BIT is set
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_DOCSIS_BIT                  (17)

/**
 * @define Specify if the soft LRO info is valid and should be used
 * @note when LRO info is used the session cannot be classified
 *       for TDOX
 */
#define PP_SESS_FLAG_SLRO_INFO_BIT               (18)

/**
 * @define Specify if this session requires SW AQM
 * @note to use with (struct pp_sess_create_args).flags
 */
#define PP_SESS_FLAG_AQM_BIT                     (19)

/**
 * @struct pp_session_args
 * @brief PP session arguments required for creating a new
 *        session
 */
struct pp_sess_create_args {
	/*! PP ingress port index */
	u16 in_port;

	/*! PP egress port index */
	u16 eg_port;

	/* LRO info, this will be copied in the SI tdox field */
	u16 lro_info;

	/*! priority used for allocating buffer from FSQM, */
	/*! applicable only if FSQM_ALLOC flag is set      */
	u8  fsqm_prio;

	/*! Session color */
	u8  color;

	/*! session flags */
	unsigned long flags;

	/*! logical destination queue number where the */
	/*! session's packets should be accelerated to */
	u32 dst_q;

	/*! logical high priority destination queue number where the */
	/*! high priority session's packets should be accelerated to */
	/*! relevant only for Turbodox sessions */
	u32 dst_q_high;

	/*! session group counters indexes, specify the group counters */
	/*! to update for the session, use PP_SGC_INVALID to specify    */
	/*! an invalid entry */
	u16 sgc[PP_SI_SGC_MAX];

	/*! token bucket meters indexes, specify the token bucket meters */
	/*! to use for the session */
	u16 tbm[PP_SI_TBM_MAX];

	/*! protocol specific data to copy to STW PS */
	/*! Set PP_SESS_FLAG_PS_VALID_BIT to indicate the ps is valid */
	u32 ps;

	/*! specify number of bytes to copy from STW template to packet UD */
	u8  tmp_ud_sz;

	/* user can add an extra csum delta change to be done for all
	 * accelerated packets on the ip header where nat is done
	 * NOTE: supported only for session with nat changes
	 */
	__sum16 ip_csum_delta;

	/* classification fields */
	/*! defines the classification data that should be copied to the */
	/*! session SI field vector oob (out of band) region, this must  */
	/*! matched the ingress port classification info                 */
	struct pp_port_cls_data cls;

	/*! rx packet info as received from the packet parse module */
	struct pktprs_hdr *rx;

	/*! tx packet info as received from the packet parse module */
	struct pktprs_hdr *tx;

	/*! hash result to identify the session in HW */
	struct pp_hash hash;

	/*! mcast information, for multicast sessions */
	struct pp_mcast mcast;

	/*! new dscp val, 6 bit field */
	u8 remark_dscp;
};

/**
 * @enum pp_nf_type
 * @brief packet processor network function type
 */
enum pp_nf_type {
	PP_NF_REASSEMBLY,
	PP_NF_TURBODOX,
	PP_NF_FRAGMENTER,
	PP_NF_MULTICAST,
	PP_NF_IPSEC_LLD,
	PP_NF_REMARKING,
	PP_NF_NUM
};

/**
 * @struct pp_nf_info
 * @brief pp network function information
 */
struct pp_nf_info {
	u16 pid;     /*! nf pp port index (GPID)        */
	u16 subif;   /*! DP subif */
	u16 q;       /*! nf QoS (logical) queue         */
	u16 cycl2_q; /*! nf cycle 2 QoS (logical) queue */
};

/**
 * @enum pp_event
 * @brief Defining PP events IDs
 */
enum pp_event {
	PP_SESS_CREATE,    /*!< session create */
	PP_SESS_DELETE,    /*!< session delete */
	PP_SESS_UPDATE,    /*!< session update */
	PP_MC_SESS_CREATE, /*!< multicast session create */
	PP_MC_SESS_UPDATE, /*!< multicast session update */
	PP_PORT_FLUSH,     /*!< flush all port's sessions */
	PP_FLUSH_ALL,      /*!< flush all PP sessions */
	PP_INACTIVE_LIST,  /*!< inactive sessions list */
	PP_EVENTS_NUM      /*!< Number of PP events */
};

/**
 * @define converts event enum to string
 */
#define PP_EVENT_STR(event) \
	(event == PP_SESS_CREATE      ? "Session create"           : \
	(event == PP_SESS_DELETE      ? "Session delete"           : \
	(event == PP_SESS_UPDATE      ? "Session update"           : \
	(event == PP_MC_SESS_CREATE   ? "Multicast session create" : \
	(event == PP_MC_SESS_UPDATE   ? "Multicast session update" : \
	(event == PP_PORT_FLUSH       ? "Port sessions flush"      : \
	(event == PP_FLUSH_ALL        ? "Flush all sessions"       : \
	(event == PP_INACTIVE_LIST    ? "Inactive sessions list"   : \
	("Other")))))))))

/**
 * @struct pp_cb_args
 * @brief PP callback args definition, defines the basic
 *        argument for PP callback for all events, this MUST be
 *        included in all events arguments structures.
 */
struct pp_cb_args {
	/*! Event ID */
	enum pp_event ev;

	/*! return code of the operations required by the user, this will   */
	/*! specify if the operation a user asked for was successful or not */
	/*! common return code are:                                         */
	/*!     0 for success,                                              */
	/*!     -EPERM in case session manager is disabled                  */
	/*!     -EINVAL in case input arguments are invalid                 */
	/*!     every other non-zero value which isn't specified            */
	/*!     here is an error                                            */
	/*! below are return code per request:                              */
	/*! Session create: -EEXIST in case session already exist           */
	/*!                 -EPROTONOSUPPORT in case session isn't          */
	/*!                  supported ENOSPC in case there isn't no        */
	/*!                  space for a new session                        */
	/*! Session delete: -ENOENT in case session doesn't exist           */
	/*! Port Flush    : -EFAULT in case the port is not active          */
	s32 ret;

	/*! the request id, this is value is specified by the user upon     */
	/*! the request and being returned by the PP to the user via        */
	/*! the callback */
	unsigned long req_id;
};

/**
 * @struct pp_sess_create_cb_args
 * @brief PP session create user callback arguments, defines
 *        the arguments list for the callback called when PP
 *        create session has completed
 */
struct pp_sess_create_cb_args {
	/*! basic event arguments */
	struct pp_cb_args base;

	/*! the newly created session ID in case the */
	/*! it was successfully created              */
	u32 sess_id;
};

/**
 * @struct pp_sess_delete_cb_args
 * @brief PP session delete user callback arguments
 */
struct pp_sess_delete_cb_args {
	/*! basic event arguments */
	struct pp_cb_args base;

	/*! the deleted session id */
	u32 sess_id;
};

/**
 * @struct pp_port_flush_cb_args
 * @brief PP session delete user callback arguments
 */
struct pp_port_flush_cb_args {
	/*! basic event arguments */
	struct pp_cb_args base;

	/*! PP port id (GPID) */
	u16 port_id;
};

/**
 * @struct pp_inactive_list_cb_args
 * @brief PP get inactive sessions list user callback
 *        arguments list. defines the arguments list for the
 *        callback called to provide a list of inactive PP
 *        sessions.
 */
struct pp_inactive_list_cb_args {
	/*! basic event arguments */
	struct pp_cb_args base;
	/*! Number of inactive sessions */
	u32 n_sessions;
	/*! Array, provided by the user, of inactive sessions */
	u32 *inact_sess;
};

/**
 * @brief Packet Processor callback definition.
 *        The user of various PP operations need to provide an API of
 *        this type in order to get notified when the operation
 *        completed and if it was successful or not
 *        The user is always provided with the basic event arguments
 *        list, to get the event specific argument list, use
 *        'container_of'
 * @param args callback arguments list
 */
typedef void (*pp_cb)(struct pp_cb_args *args);

/**
 * @struct pp_request
 */
struct pp_request {
	/*! callback for request result, when this is set to a non-null
	 *  value, the context of the request will be changed to work
	 *  queue, AKA, async mode
	 *  When set to null, no context switch is done, AKA, sync mode
	 */
	pp_cb cb;

	/*! request priority for async mode, any non-zero value specify
	 *  an high priority request
	 */
	unsigned long req_prio;

	/*! request id, user specific id to identify the user request */
	unsigned long req_id;
};

/**
 * @define Test whether PP request is async
 */
#define PP_IS_ASYNC_REQUEST(req)                ((req) && (req)->cb)

/**
 * @define Test whether PP request is sync
 */
#define PP_IS_SYNC_REQUEST(req)                 !(PP_IS_ASYNC_REQUEST(req))

/**
 * @define Maximum host cpus the PP supports
 */
#define PP_MAX_HOST_CPUS 4

/**
 * @define Maximum number of queues supported per host CPU the PP supports
 */
#define PP_MAX_CPU_QUEUES 8

/**
 * @define Lowest priority for port in gpid group
 */
#define PP_GPID_GRP_LOWEST_PRIO (PP_MAX_CPU_QUEUES - 1)

struct pp_cpu_queue_info {
	u16 gpid; /* PP global port id */
	u16 id;   /* Logical queue id */
};

/**
 * @struct CPU info PP requires for DoS protection
 */
struct pp_cpu_info {
	/*! queues info, highest priority queue at index 0 */
	struct pp_cpu_queue_info queue[PP_MAX_CPU_QUEUES];
	u8 num_q; /*! number of queues */
};

enum pp_whitelist_field_type {
	PP_FLD_SRC_MAC,
	PP_FLD_DST_MAC,
	PP_FLD_ETH_TYPE,
	PP_FLD_IPV4_SRC_IP,
	PP_FLD_IPV4_DST_IP,
	PP_FLD_IPV6_SRC_IP,
	PP_FLD_IPV6_DST_IP,
	PP_FLD_IP_PROTO,
	PP_FLD_L4_SRC_PORT,
	PP_FLD_L4_DST_PORT,
	PP_FLD_ICMP_TYPE_CODE,
	PP_FLD_COUNT
};

struct pp_whitelist_field {
	enum pp_whitelist_field_type type;
	union {
		u8 ip_proto; /*! ip/ipv6 next protocol */
		u8 dst_mac[ETH_ALEN]; /*! destination MAC address !*/
		u8 src_mac[ETH_ALEN]; /*! source MAC address !*/
		__be16 eth_type; /*! ethernet type */
		__be16 src_port; /*! L4 source port */
		__be16 dst_port; /*! L4 destination port */
		__be16 icmp_type_code; /*! icmp type & code */
		union {
			struct in6_addr v6; /*! ipv6 source address */
			__be32 v4; /*! ipv4 source address */
		} src_ip;
		union {
			struct in6_addr v6; /*! ipv6 destination address */
			__be32 v4; /*! ipv4 destination address */
		} dst_ip;
	};
};

/**
 * @struct pp_lld_stats
 * @brief lld counters, identical to struct aqm_lld_stats in uc host common
 */
struct pp_lld_stats {
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
	/*! error packet counter */
	u64 error_pkt;
	/*! mark packet counter */
	u64 mark_pkt;
	/*! saction packet counter */
	u64 sanction_pkt;
	/*! drop packet counter */
	u64 drop_pkt;
};

/**
 * @struct PP_AQM_SW_stats
 * @brief sw aqm statistics
 */
struct PP_AQM_SW_stats {
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
 * Handling routines are only of interest to the kernel
 */

/* PP Misc */

/**
 * @brief Configure PP with host CPUs specific info to be used
 *        for DoS protection
 * @param cpu cpu info array
 * @param num_cpus number of cpus
 * @return int 0 on success, error code otherwise
 */
s32 pp_host_cpu_info_init(struct pp_cpu_info *cpu, unsigned int num_cpus);

/**
 * @brief get the pp components version number
 * @param ver pp version template
 * @param ver_type pp component type
 * @return s32 return 0 for success
 */
s32 pp_version_get(struct pp_version *ver, enum pp_version_type ver_type);

/**
 * @brief set the packet processor network functions
 * @param type network function type
 * @param nf network function information
 * @param data optional data that relevant to the nf
 * @return s32 return 0 for success
 */
s32 pp_nf_set(enum pp_nf_type type, struct pp_nf_info *nf, void *data);

/**
 * @brief get the packet processor network function information
 * @param type network function type
 * @param nf network function information
 * @return s32 return 0 for success
 */
s32 pp_nf_get(enum pp_nf_type type, struct pp_nf_info *nf);

/**
 * @brief get pp global counters and status buffer
 * @param buf buffer for printing
 * @param size size of allocated buffer, in success, api sets
 *        the size of returned buffer
 * @return s32 return 0 for success
 */
s32 pp_get_global_status(char *buf, u32 *size);

/**
 * @brief PP RX hook, attach PP and PS cookies to an SKB that
 *        was received from PP HW
 * @param skb
 */
void pp_rx_pkt_hook(struct sk_buff *skb);

/**
 * @brief PP TX hook
 * @param skb
 * @param pid PP port id (GPID) for transmission [0..255]
 */
void pp_tx_pkt_hook(struct sk_buff *skb, u16 pid);

/**
 * @brief get the PP descriptor of packet
 * @param skb skb pointer
 * @return struct pp_desc* descriptor cookie pointer, NULL if
 *         not exist
 */
struct pp_desc *pp_pkt_desc_get(struct sk_buff *skb);

/**
 * @brief Set Aqm & LLD service flow
 * @param sf_id service flow id
 * @param sf_cfg service flow configuration
 * @return 0 on success
 */
s32 pp_misc_sf_set(u8 sf_id, struct pp_qos_aqm_lld_sf_config *sf_cfg);

/**
 * @brief Remove Aqm & LLD service flow
 * @param sf_id service flow id
 * @return 0 on success
 */
s32 pp_misc_sf_remove(u8 sf_id);

/**
 * @brief Get Aqm & LLD service flow configuration
 * @param sf_id service flow id
 * @param sf_cfg service flow configuration
 * @return 0 on success
 */
s32 pp_misc_sf_conf_get(u8 sf_id, struct pp_qos_aqm_lld_sf_config *sf_cfg);

/**
 * @brief Get Aqm & LLD service flow histogram
 * @param sf_id service flow id
 * @param hist service flow histogram
 * @param reset reset histogram after retrieving
 * @return 0 on success
 */
s32 pp_misc_sf_hist_get(u8 sf_id, struct pp_sf_hist_stat *hist, bool reset);

/**
 * @brief Get LLD counters per SF
 * @param stats lld stats structure
 * @return 0 on success
 */
s32 pp_misc_sf_lld_counters_get(u8 sf_id, struct pp_lld_stats *stats);

/**
 * @brief Get sw aqm counters per SF
 * @param stats bc stats structure
 * @return 0 on success
 */
s32 pp_misc_aqm_sw_sf_counters_get(u8 sf_id, struct PP_AQM_SW_stats *stats);

/**
 * @brief Set LLD Allowed AQ
 * @param sf_id LLD service flow id
 * @param allowed_aq Allowed AQ (calculated in MAC US)
 * @return 0 on success
 */
s32 pp_lld_allowed_aq_set(u8 sf_id, u32 allowed_aq);

/**
 * @brief check if queue is attached to LLD SF
 * @param dest_q logical destination queue
 * @param lld_sf true if queue is attached to LLD SF, false otherwise
 * @return s32 return 0 for success
 */
s32 pp_misc_check_queue_lld_sf(u16 dst_q, bool *lld_sf);

/**
 * @brief get sf id according to queue
 * @param queue logical destination queue
 * @param sf_indx service flow index
 * @return s32 return 0 for success
 */
s32 pp_misc_get_sf_indx_by_q(u16 queue, u16 *sf_indx);

/**
 * @brief get lld fw context according to sf
 * @param sf_id ervice flow index
 * @param ctx lld fw context
 * @return s32 return 0 for success
 */
s32 pp_misc_fw_lld_ctx_get(u8 sf_id, u8 *ctx);

/**
 * @brief set pci ready state and BAR address of the PCI device
 * @param state true for ready, false for not ready
 * @param bar_addr BAR address of the pci device
 * @return s32 0 on success, error code otherwise
*/
s32 pp_misc_pci_ready_set(bool state, u32 bar_addr);
/**
 * @brief get aqm engine
 * @param aqm_engine set sw or hw aqm
 * @return s32 0 on success, error code otherwise
*/
s32 pp_misc_get_aqm_engine(u8 *aqm_engine);

/* Port manager driver API */
/**
 * @brief adding new pp port
 * @param port_id pp port index (GPID)
 * @param cfg pp port configuration
 * @return s32 return 0 for success
 */
s32 pp_port_add(u16 port_id, struct pp_port_cfg *cfg);

/**
 * @brief updating pp port configuration
 * @param port_id pp port index (GPID)
 * @param cfg pp port configuration
 * @return s32 return 0 for success
 */
s32 pp_port_update(u16 port_id, struct pp_port_cfg *cfg);

/**
 * @brief get pp port configuration
 * @param port_id pp port index (GPID)
 * @param cfg pp port configuration (filled by API)
 * @return s32 return 0 for success
 */
s32 pp_port_get(u16 port_id, struct pp_port_cfg *cfg);

/**
 * @brief deleting pp port
 * @param pid id of pp port
 * @return s32 return 0 for success
 */
s32 pp_port_del(u16 pid);

/**
 * @brief get pp port statistics
 * @param pid pp port number
 * @param stats pp packet and bytes statistics
 * @return s32 return 0 for success
 */
s32 pp_port_stats_get(u16 pid, struct pp_stats *stats);

/**
 * @brief Start port DPL white list protection
 * @param pid pp port number
 * @return s32 return 0 for success
 */
s32 pp_port_protect(u16 pid);

/**
 * @brief Stop port DPL white list protection
 * @param pid pp port number
 * @return s32 return 0 for success
 */
s32 pp_port_unprotect(u16 pid);

/**
 * @brief set default PP to host interface
 * @param dp default hostif datapath param
 * @return s32 return 0 for success
 */
s32 pp_hostif_dflt_set(struct pp_hif_datapath *dp);

/**
 * @brief get default PP to host interface
 * @param dp default hostif datapath param
 * @return s32 return 0 for success
 */
s32 pp_hostif_dflt_get(struct pp_hif_datapath *dp);

/**
 * @brief add pp to host interface
 * @param hif pp to host interface configuration
 * @return s32 return 0 for success
 */
s32 pp_hostif_add(struct pp_hostif_cfg *hif);

/**
 * @brief update pp to host interface with a new datapath
 * @param hif current pp to host interface configuration
 * @param new_dp new pp to host interface datapath to set
 * @return s32 return 0 for success
 */
s32 pp_hostif_update(struct pp_hostif_cfg *hif, struct pp_hif_datapath *new_dp);

/**
 * @brief delete pp to host interface
 * @param hif pp to host interface configuration
 * @return s32 return 0 for success
 */
s32 pp_hostif_del(struct pp_hostif_cfg *hif);

/**
 * @brief Create a new GPID group
 * @param cpu cpu info array
 * @param num_cpus number of cpus
 * @return s32 group id on success, error code value
 * @note return value is to be used for other group operations.
 *       in case of failure, the error code is a negative value
 */
s32 pp_gpid_group_create(const char *name, struct pp_cpu_info *cpu,
			 unsigned int num_cpus);

/**
 * @brief Delete an existing GPID group
 * @param id group's id
 * @return s32 0 on success, error code otherwise
 */
s32 pp_gpid_group_delete(u32 id);

/**
 * @brief Add a new priority rule to a GPID group
 * @param grp_id group id, returned by gpid group create API
 * @param prio priority, used for choosing destination queue for packets
 *             who matched the rule.
 *             valid values are from 0 (highest) to 7 (lowest)
 * @param fields fields array to compare to the packet data
 * @param cnt number of fields
 *
 * GPID grouping rules are used to priorities certain packets over other
 * on specific ports on ingress side.
 * Rules can only be applied on groups of GPIDs.
 * Note that only specific rules types are supported, and only some
 * fields combinations are supported:
 * 1. Source or destination MAC address.
 * 2. Source or destination MAC AND source or destination IP.
 * 3. Last ethertype of the MAC header.
 * 4. TCP/UDP source AND destination ports.
 * 5. ICMPv4/ICMPv6 code AND type.
 * 6. IPv4 source *or* destination AND UDP/TCP source and destination ports.
 * 7. IPv4 source *and* destination AND UDP/TCP source and destination ports.
 * 8. IPv6 source or destination AND UDP/TCP source and destination ports.
 * @return int 0 on success, error code otherwise
 */
s32 pp_gpid_group_rule_add(u32 grp_id, u32 prio,
			      struct pp_whitelist_field *fields, u32 cnt);

/**
 * @brief Delete GPID rule, input parameters here
 *        should be the exact ones that were used when the rule created
 * @param grp_id group id, returned by gpid group create API
 * @param prio priority
 * @param fields fields array
 * @param cnt number of fields
 * @return int 0 on success, error code otherwise
 */
s32 pp_gpid_group_rule_del(u32 grp_id, u32 prio,
			      struct pp_whitelist_field *fields, u32 cnt);

/**
 * @brief Map GPID to a GPID group and set default priority for the port
 * @param grp_id the group to map to
 * @param port_id the GPID to map
 * @param dflt_prio lowest priority for the port
 * @return s32 0 on succuss, error code otherwise
 */
s32 pp_gpid_group_add_port(u32 grp_id, u32 port_id, u32 dflt_prio);

/**
 * @brief Unmap GPID from a GPID group and restore old
 *        GPID mapping
 * @param grp_id the group to unmap from
 * @param port_id the port to unmap
 * @return s32 0 on succuss, error code otherwise
 */
s32 pp_gpid_group_del_port(u32 grp_id, u32 port_id);

/**
 * @brief get group id that this port map to
 * @param port_id pp port index (GPID)
 * @param grp_id group id that contains the port
 * @return s32 0 on succuss, error code otherwise
 */
s32 pp_gpid_group_get_port_map(u32 port_id, u32 *grp_id);

/* Session manager driver API */
/**
 * @brief get the pp max supported sessions number
 * @param max_sessions number of max supported sessions
 * @return s32 return 0 for success
 */
s32 pp_max_sessions_get(u32 *max_sessions);

/**
 * @brief get the pp number of open sessions
 * @param open_sessions number of open sessions
 * @return s32 return 0 for success
 */
s32 pp_open_sessions_get(u32 *open_sessions);

/**
 * @brief get the pp number of created sessions
 * @param created_sessions number of created sessions
 * @return s32 return 0 for success
 */
s32 pp_created_sessions_get(u32 *created_sessions);

/**
 * @brief get session statistics
 * @note Cannot be used from irq context
 * @param id session id
 * @param stats pp packets and bytes statistics
 * @return s32 return 0 for success, error code otherwise
 */
s32 pp_session_stats_get(u32 id, struct pp_stats *stats);

/**
 * @brief Reset session counters
 * @param id session id
 * @param act action to do, decrement, increment or reset
 * @param pkts number of packets to dec/inc, applicable only for
 *             increment and decrement
 * @param bytes number of bytes to dec/inc, applicable only for
 *             increment and decrement
 * @note Both pkts and bytes must be non-zero value when incrementing
 *       or decrementing
 * @return s32 0 on success, error code otherwise
 */
s32 pp_session_stats_mod(u32 id, enum pp_stats_op act, u8 pkts, u32 bytes);

/**
 * @brief get session size in ddr, number of bytes needed to hold 1
 *        session in ddr
 * @param sz buffer to return the result
 * @return s32 return 0 for success, error code otherwise
 */
s32 pp_session_size_get(size_t *sz);

/**
 * @brief get pp hash results for a session
 * @note Cannot be used from irq context
 * @param id session id
 * @param hash pp hash results
 * @return s32 return 0 for success
 */
s32 pp_session_hash_get(u32 id, struct pp_hash *hash);

/**
 * @brief Get last 3 hash results calculated by the HW
 * @param hash 3 hash results
 * @return s32 0 on success, error code otherwise
 */
s32 pp_session_dbg_hash_get(struct pp_hash *hash);

/**
 * @brief get pp session active state, an active session means a
 *        session which had a least 1 packet accelerated since he was
 *        created or from last crawler scan.
 * @note Cannot be used from irq context
 * @param id session id
 * @param active return whether the session is active or not
 * @return s32 return 0 for success
 */
s32 pp_session_active_state_get(u32 id, bool *active);

/**
 * @brief get number of exception pkts
 * @param pkts return number pkts
 * @return s32 return 0 for success
 */
s32 pp_exception_pkts_get(u64 *pkts);

/**
 * @brief Reserve an entire group of SGCs.
 *        The SGCs from the specified group can ONLY
 *        be allocated/freed/modified with the returned
 *        owner id
 * @param grp group id
 * @param owner allocated owner id to use later for
 *              allocated/freed/modified SGCs from the specifeid group
 * @return s32 0 on success, error code otherwise
 */
s32 pp_sgc_group_reserve(u8 grp, u32 *owner);

/**
 * @brief Unreserve SGC group, ONLY the owner of the group can do it
 * @param grp the group
 * @param owner the owner
 * @return s32 0 on success, error code otherwise
 */
s32 pp_sgc_group_unreserve(u8 grp, u32 owner);

/**
 * @brief allocate pp session group counters
 * @param owner the group owner identifier allocated by pp_sgc_group_res
 * @param grp sgc group to alloc the counter from
 * @param counters allocated counters indexes
 * @param count number of counters to alloc
 * @return s32 return 0 for success, error code otherwise
 */
s32 pp_sgc_alloc(u32 owner, u8 grp, u16 *counters, u16 count);

/**
 * @brief free pp session group counter
 * @param owner the group owner identifier allocated by pp_sgc_group_res
 * @param grp pp sgc group
 * @param counters to free
 * @param number of counters to free
 * @return s32 return 0 for success, error code otherwise
 */
s32 pp_sgc_free(u32 owner, u8 grp, u16 *counters, u16 count);

/**
 * @brief get pp session group counter statistics
 * @param pool pp sgc group
 * @param sgc pp sgc counter id
 * @param stats pp packets and bytes statistics
 * @param num_sessions number of active sessions that
 *        use the specified SGC
 * @return s32 return 0 for success
 */
s32 pp_sgc_get(u8 grp, u16 sgc_id, struct pp_stats *stats, u32 *num_sessions);

/**
 * @brief Modify PP session group counter
 * @param owner the group owner identifier allocated by pp_sgc_group_res
 * @param grp sgc group
 * @param sgc_id sgc counter id
 * @param pp_stats_op what to do
 * @param pkts number of packets to modify
 * @param bytes number of bytes to modify
 * @note packets and bytes to modify applicable for add or sub and at least
 *       one of them needs to be non-zero value
 * @return s32
 */
s32 pp_sgc_mod(u32 owner, u8 grp, u16 sgc_id, enum pp_stats_op op, u32 pkts,
	       u32 bytes);

/**
 * @brief Get an array of sessions indexes which uses the specified sgc
 *        The array doesn't have to be big enough to hold all sessions
 * @note it might be that by the time the caller iterate over the array
 *       the sessions doesn't exist anymore, to ensure the caller MUST
 *       lock the session manager, and this isn't possible!
 * @param grp sgc group
 * @param cntr sgc index
 * @param sessions the array
 * @param n_sessions [in/out] array size, will be updated to the actual
 *                   number of sessions that were written to the array
 * @return s32
 */
s32 pp_sgc_sessions_get(u8 grp, u16 cntr, u32 *sessions, u32 *n_sessions);

/**
 * @brief add new pp token bucket meter
 * @param idx the newly allocated tbm index
 * @param tbm optional, tbm configuration, use NULL if configuring the TBM
 *            isn't needed
 * @return s32 return 0 for success
 */
s32 pp_dual_tbm_alloc(u16 *idx, struct pp_dual_tbm *cfg);

/**
 * @brief delete pp token bucket meter
 * @param idx tbm index to free
 * @return s32 return 0 for success
 */
s32 pp_dual_tbm_free(u16 idx);

/**
 * @brief get pp token bucket meter configuration
 * @param idx tbm index
 * @param tbm pp tbm configuration
 * @return s32 return 0 for success
 */
s32 pp_dual_tbm_get(u16 idx, struct pp_dual_tbm *cfg);

/**
 * @brief update pp token bucket meter configuration
 * @param idx tbm index
 * @param tbm pp tbm configuration
 * @return s32 return 0 for success
 */
s32 pp_dual_tbm_set(u16 idx, struct pp_dual_tbm *cfg);

/**
 * @brief update turbodox timer timeout in usec
 * @note Maximum value is 5000 usec (5 msec)
 * @param tout timeout value
 * @return s32 return 0 for success
 */
s32 pp_tdox_tout_set(u32 tout);

/**
 * @brief add/remove a multicast destination to/from a group
 * @param group multicast group index
 * @param dst multicast destination index
 * @param add_op true for add operation, false for remove
 * @return s32 return 0 for success
 */
s32 pp_mcast_dst_set(u16 group, u8 dst, bool add_op);

/**
 * @brief get the multicast group information
 * @param group multicast group index
 * @param info multicast group information
 * @return s32 return 0 for success
 */
s32 pp_mcast_group_info_get(u16 group, struct pp_mcast_grp_info *info);

/**
 * @brief get list of inactive pp sessions, session is considered to
 *        be inactive if one of the following apply:<br>
 *        1. No packets were ever forwarded on the session
 *        2. No packets were forwarded on the session from the last
 *           call to this function
 * @note Cannot be used from irq context
 * @param req pp request
 * @param inact array to return inactive sessions indexes
 * @param n number of entries in 'inact' array
 * @return s32 return 0 for success
 */
s32 pp_inactive_sessions_get(struct pp_request *req, u32 *inact, u32 n);

/**
 * @brief add new pp session
 * @note Cannot be used from irq context
 * @param args session arguments
 * @param sess_id pointer to store session id, this is ignored in case
 *                a callback was provided in the request
 * @param req session create async request, set to NULL to specify
 *            sync request
 * @return s32 return 0 for success, error code otherwise, in async
 *         request, real return code will be returned via the callback
 */
s32 pp_session_create(struct pp_sess_create_args *args, u32 *sess_id,
		      struct pp_request *req);

/**
 * @brief delete pp session
 * @note Cannot be used from irq context
 * @param sess_id session id to delete
 * @param req session delete async request, set to NULL to specify
 *            sync request
 * @return s32 return 0 for success, error code otherwise, in async
 *         request, real return code will be returned via the callback
 */
s32 pp_session_delete(u32 sess_id, struct pp_request *req);

/**
 * @brief flush all PP sessions
 * @note Cannot be used from irq context
 * @param req flush all async request, set to NULL to specify
 *            sync request
 * @return s32 return 0 for success, error code otherwise, in async
 *         request, real return code will be returned via the callback
 */
s32 pp_flush_all(struct pp_request *req);

/**
 * @brief flush all port's sessions, deletes all the sessions
 *        associated with the specified port<br>
 *        Session is associated with a port if the port in used as the
 *        session's ingress port or egress port or both
 * @note Cannot be used from irq context
 * @param req port flush async request, set to NULL to specify
 *            sync request
 * @param port id port id to flush
 * @return s32 return 0 for success, error code otherwise, in async
 *         request, real return code will be returned via the callback
 */
s32 pp_port_flush(u16 port_id, struct pp_request *req);

/**
 * @brief updating destination queue of a pp session
 * @note Cannot be used from irq context
 * @param id session id
 * @param dst_q new egress queue logical id
 * @return s32 return 0 for success
 */
s32 pp_session_dst_queue_update(u32 id, u16 dst_q);

/**
 * @brief Print all PP white rules into seq_file
 * @param f the seq file to print into
 */
void pp_uc_whitelist_rules_show(struct seq_file *f);

/**
 * @brief Add new white list rule to the PP
 * @param prio priority, used for choosing destination queue for packets
 *             who matched the rule.
 *             valid values are from 0 (highest) to 7 (lowest)
 * @param fields fields array to compare to the the packets
 * @param cnt number of fields
 *
 * White list rules are used to protect the host when a DoS attack occurs
 * by prioritizing packets that was matched to one of the rules
 * to queues with high priority
 * @return int 0 on success, error code otherwise
 */
s32 pp_whitelist_rule_add(u32 prio, struct pp_whitelist_field *fields, u32 cnt);

/**
 * @brief Delete an existing white rule from the PP, input parameters here
 *        should be the exact ones that were used in pp_whitelist_rule_add
 * @param prio priority
 * @param fields fields array
 * @param cnt number of fields
 * @return int 0 on success, error code otherwise
 */
s32 pp_whitelist_rule_del(u32 prio, struct pp_whitelist_field *fields, u32 cnt);

/**
 * @define PP max number of bit hash entries
 */
#define PP_DPL_HASH_BIT_MAX_ENTRIES (2048)

/**
 * @brief Hash bit table , enable bit according to index recv from API
 * @param index index in table to enable
 * @return int 0 on success, error code otherwise
 */
s32 pp_dpl_hash_bit_enable(u32 index);

/**
 * @brief Hash bit table , Disable bit according to index recv from API
 * @param index index in table to disable
 * @return int 0 on success, error code otherwise
 */
s32 pp_dpl_hash_bit_disable(u32 index);

/**
 * @brief Hash bit table , init table to zero
 * @return int 0 on success, error code otherwise
 */
s32 pp_dpl_hash_bit_reset(void);

/**
 * @brief get pp hal packets statistics
 * @param stats pp hal packets statistics
 * @return s32 return 0 for success
 */
s32 pp_hal_stats_get(struct pp_hal_stats *stats);

/**
 * @brief Set pp acceleration mode
 * @param acceleration mode (enum pp_accl_mode)
 * @return s32 0 on success, error code otherwise
*/
s32 pp_accl_mode_set(u8 mode);

/**
 * @brief Get pp acceleration mode
 * @param mode pointer to save acceleration mode
 * @return s32 0 on success, error code otherwise
*/
s32 pp_accl_mode_get(u8 *mode);

#endif /* __PP_API_H__ */
