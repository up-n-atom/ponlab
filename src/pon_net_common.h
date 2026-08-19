/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds common definitions.
 */

#ifndef _PON_NET_COMMON_H_
#define _PON_NET_COMMON_H_

#include <stdio.h>
#include <net/if.h>

#include <linux/if_ether.h>
#include <sys/queue.h>

#include <pthread.h>
#include <semaphore.h>

#include <pon_adapter.h>

#include "pon_net_config.h"
#include "pon_net_mapper_table.h"
#include "pon_net_me_list.h"
#include "pon_net_vlan_flow.h"
#include "pon_net_netlink.h"
#include "pon_net_os.h"

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_LIB_MACROS PON Network Library Macro Definitions
 *  These macros are used in the pon_net_lib code to reduce code complexity
 *  and improve readability.
 *  @{
 */

#define NEXT_OR_NULL(var, field) \
	(var) ? (var)->field.sle_next : NULL

#define FOREACH_SAFE(var, next, head, field) \
	for ((var) = (head)->slh_first, (next) = NEXT_OR_NULL(var, field); \
	     (var); \
	     (var) = (next), (next) = NEXT_OR_NULL(var, field))

/** The maximum number of physical UNI ports for PRX300 devices in general is 2.
 *  The maximum number of physical UNI ports for External Switch is 5.
 *  \remark The physical limit for PRX126 is 1.
 */
#define LAN_PORT_MAX 5

/** Maximum number of supported VEIP instances.
 *  \remark This is not a physical limit and can be changed, if needed.
 */
#define VEIP_PORT_MAX 4

/** Maximum number of supported T-CONTs */
#define TCONT_MAX 64

/** Maximum number of supported bridge ports */
#define BRIDGE_PORT_MAX 128

#define PON_MASTER_DEVICE "pon0"
#define IPH_MASTER_DEVICE "ip0"
#define UNUSED(x) (void)(x)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#define BIT(nr) (1ul << (nr))

#ifndef _MKSTR_1
#define _MKSTR_1(x)    #x
#define _MKSTR(x)      _MKSTR_1(x)
#endif

#define DSCP_MAX 64

#define ADMIN_STATE_LOCKED 0
#define ADMIN_STATE_UNLOCKED 1

/* Supported ICMPV6 types */
#ifndef ICMPV6_MGM_QUERY
#define ICMPV6_MGM_QUERY		130
#define ICMPV6_MGM_REPORT		131
#define ICMPV6_MGM_REDUCTION		132
#define ICMPV6_MLD2_REPORT		143
#endif

#define TCI_MASK_VID 0x0FFF
#define TCI_MASK_DEI 0x1000
#define TCI_MASK_PCP 0xE000

#define TCI_VID(tci) ((tci) & TCI_MASK_VID)
#define TCI_PCP(tci) (((tci) & TCI_MASK_PCP) >> 13)

#define OMCI_HW_TC 8
#define IPHOST_MAC_HW_TC 10
#define IPHOST_VLAN_HW_TC 10
#define IPHOST_NDP_HW_TC 12
#define IPHOST_ARP_HW_TC 13
#define LCT_PTP_HW_TC 9
#define LCT_MAC_HW_TC 11
#define LCT_MLD_HW_TC 12
#define LCT_ARP_HW_TC 13
#define IGMP_HW_TC 14

/**
 * This macro can be used to perform assertions at compile time.
 * Example: PON_NET_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes)
 * The example will cause the code not to compile unless sizeof(int) == 4
 */
#define PON_NET_STATIC_ASSERT(condition, msg) \
do { \
	char assert_##msg[(condition) ? 1 : -1]; \
	UNUSED(assert_##msg); \
} while (0)

/**
 *  This macro is used to print a debug message in case when a return value ret
 *  indicates an error or warning.
 *  This error code and the source of the error are printed if debug printouts
 *  are enabled.
 *  Example : FN_ERR_RET(ret, rtnl_flower_set_flags, PON_ADAPTER_ERROR)
 *
 * \param[in] ERR_FN	err, ret variables
 * \param[in] FN	function name which failed
 * \param[in] ERR_OUT	errorcode value
 */
#define FN_ERR_RET(ERR_FN, FN, ERR_OUT) \
	do { \
		dbg_err_fn_ret(FN, ERR_FN); \
		dbg_out_ret("%d", ERR_OUT); \
	} while (0)

/**
 *  This macro is used to print a more detailed debug message in case when a
 *  return value ret of netlink function indicates an error or warning.
 *  The source of the error is printed if debug printouts are enabled.
 *  Example : FN_ERR_NL(ret, link_change_by_name, ifname)
 *
 * \param[in] ERR_FN	err, ret variables
 * \param[in] FN	function name which failed
 * \param[in] IFNAME	interface name
 */
#define FN_ERR_NL(ERR_FN, FN, IFNAME) \
	dbg_err("%s() failed on [%s] with %d (%s)\n", #FN, IFNAME, \
		ERR_FN, nl_geterror(ERR_FN))

/**
 *  This macro is used to print a more detailed debug message in case when a
 *  return value ret of netlink function indicates an error or warning.
 *  This error code and the source of the error are printed if debug printouts
 *  are enabled.
 *  Example : FN_ERR_NL_RET(ret, link_change_by_name, ifname, PON_ADAPTER_ERROR)
 *
 * \param[in] ERR_FN	err, ret variables
 * \param[in] FN	function name which failed
 * \param[in] IFNAME	interface name
 * \param[in] ERR_OUT	errorcode value
 */
#define FN_ERR_NL_RET(ERR_FN, FN, IFNAME, ERR_OUT) \
	do { \
		FN_ERR_NL(ERR_FN, FN, IFNAME); \
		dbg_out_ret("%d", ERR_OUT); \
	} while (0)

/** Use it to indicate that some function call failed on given interface */
#define dbg_err_fn_ifname(fn, ifname) \
	dbg_err("%s() failed on [%s]\n", #fn, ifname)

/** Use it to indicate that some function call failed on given interface
 *  with specific return value
 */
#define dbg_err_fn_ifname_ret(fn, ifname, ret) \
	dbg_err("%s() failed on [%s] with %d\n", #fn, ifname, ret)

/** Network device name of the OMCC (OMCI management interface) */
#define GEM_OMCI_DEVICE "gem-omci"

/* Example: snprintf(buf, sizeof(buf), PON_IFNAME_GEM, number); */

/** Network device name for a GEM port */
#define PON_IFNAME_GEM "gem%d"
/** Network device name for a T-CONT */
#define PON_IFNAME_TCONT "tcont%d"
/** Network device name for a MAC bridge */
#define PON_IFNAME_BRIDGE "sw%d"
/** Network device name for an IEEE 802.1p mapper */
#define PON_IFNAME_PMAPPER "pmapper%d"
/** Network device name for an Ethernet UNI port */
#define PON_IFNAME_PPTP_ETH_UNI "eth0_%d"
/** Network device name for a dedicated multicast bridge */
#define PON_IFNAME_BRIDGE_MULTICAST "sw-multicast"
/** Network device name for the Local Craft Terminal (LCT) */
#define PON_IFNAME_LCT_SUFFIX "_1_lct"

#define PON_IFNAME_PREFIXES { "gem", "tcont", "sw", "pmapper", "sw-multicast" }

/* TC qdisc handles are 16-bit numbers and must be unique
 * (per network interface), because they are used to identify a TC qdisc.
 *
 * handle structure:
 *            1
 *  5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | type  | type specific data    |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * type - is the purpose of the qdisc, see TC_QDISC_* macros
 * type specific data - is some type specific identification data
 */

/* Handle type for first RED qdisc (green) */
#define PON_QDISC_WRED_0 1
/* Handle type for second RED qdisc (yellow) */
#define PON_QDISC_WRED_1 2
/* Handle type for qdisc corresponding to Traffic Descriptor */
#define PON_QDISC_TBF 3
/* Handle type for qdisc corresponding to Traffic Scheduler */
#define PON_QDISC_TS 4
/* Handle type for additional qdiscs corresponding to Traffic Scheduler,
 * created by the partitioning process (see pon_net_qdiscs_partition()) */
#define PON_QDISC_TS_AUX 5
/* Handle type for additional qdiscs corresponding to implicit root Traffic
 * Scheduler on the T-CONT or on the UNI, created by
 * the partitioning process (see pon_net_qdiscs_partition()) */
#define PON_QDISC_ROOT_AUX 7
/* Handle type for qdisc corresponding to implicit root Traffic Scheduler on
 * the T-CONT or on the UNI */
#define PON_QDISC_ROOT 8

/* Builds handle using type and type specific data */
#define PON_QDISC_HANDLE(type, type_specific_data) \
	((uint16_t)((((type) << 12) & 0xF000) |\
		((type_specific_data) & 0x0FFF)))
/* Extracts type from handle */
#define PON_QDISC_HANDLE_TYPE(H) (((H) >> 12) & 0xF)
/* Extracts type specific data from handle */
#define PON_QDISC_HANDLE_TYPE_DATA(H) ((H) & 0xFFF)

/* type specific data structure for qdiscs representing the Traffic Schedulers
 *    1
 *  1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+
 * |S|     TSID      |  N  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * S - 1 if traffic scheduler is from no slot, and 0 if traffic scheduler is
 *     from slot calculated as follows:
 *
 *         { 1, if (traffic_scheduler_me_id >> 16) == 0xFF
 *     S = {
 *         { 0, otherwise
 *
 * TSID - Traffic Scheduler Id calculated as follows:
 *
 *     TSID = traffic_scheduler_me_id & 0xFF
 *
 * N - Qdisc number. One Traffic Scheduler in OMCI can be represented as
 *     multiple qdiscs in TC.
 *     This is because sometimes OMCI wishes to create a traffic scheduler
 *     with for example 64 queues and we support only 8. We handle this
 *     situation by creating additional child traffic schedulers, thus one
 *     Traffic Scheduler in OMCI can be represented as multiple qdiscs in TC.
 *
 * The above bit transformations guarantee unique handle within a T-CONT
 * (T-CONT can have only Traffic Schedulers from the same slot or from no slot
 *  0xFF)
 */
#define PON_QDISC_TS_DATA(S, TSID, N) \
	(uint16_t)((((S) & 0x1U) << 11) | \
		(((TSID) & 0xFFU) << 3) | ((N) & 0x7U))

/* Extracts S from Traffic Scheduler type specific data */
#define PON_QDISC_TS_DATA_S(X) (((X) >> 11) & 0x1)
/* Extracts TSID from Traffic Scheduler type specific data */
#define PON_QDISC_TS_DATA_TSID(X) (((X) >> 3) & 0xFF)
/* Extracts N from Traffic Scheduler type specific data */
#define PON_QDISC_TS_DATA_N(X) (((X) >> 0) & 0x7)

/** Translates Traffic Scheduler ME ID into a qdisc handle.
 *  The ME ID has 0xSSBB, form where:
 *  - 0xSS is slot id or 0xFF if Traffic Scheduler is not associated with a slot
 *  - 0xBB is Traffic Scheduler Id */
#define PON_TS_QDISC(me_id) \
	PON_QDISC_HANDLE(PON_QDISC_TS, \
			 PON_QDISC_TS_DATA((me_id) >> 8 == 0xFF, me_id, 0))

/** Handle of first RED qdisc */
#define PON_WRED_QDISC_0(priority_queue_index) \
	PON_QDISC_HANDLE(PON_QDISC_WRED_0, priority_queue_index)

/** Handle of the second RED qdisc */
#define PON_WRED_QDISC_1(priority_queue_index) \
	PON_QDISC_HANDLE(PON_QDISC_WRED_1, priority_queue_index)

/** Handle of the TBF qdisc */
#define PON_TBF_QDISC_HANDLE(priority_queue_index) \
	PON_QDISC_HANDLE(PON_QDISC_TBF, priority_queue_index)

/** Handle of the root qdisc on a network interface, usually this corresponds
 *  to T-CONT or UNI "built-in/internal" scheduler */
#define PON_ROOT_QDISC \
	PON_QDISC_HANDLE(PON_QDISC_ROOT, PON_QDISC_TS_DATA(0, 0, 0))

/** Handle of the class root qdisc on a network interface */
#define PON_ROOT_CLASS_QDISC(class_id) \
	PON_QDISC_HANDLE(PON_QDISC_ROOT, PON_QDISC_TS_DATA(0, 0, class_id))

/* Handle for additional qdisc corresponding to implicit root Traffic Scheduler,
 * created by partitioning process */
#define PON_AUX_ROOT_QDISC(N) \
	PON_QDISC_HANDLE(PON_QDISC_ROOT_AUX, \
			 PON_QDISC_TS_DATA(0, 0, N))

/** Maximum number of priority classes */
#define PON_8021P_PCP_COUNT 8

/* To distinguish extended VLAN rules for multicast IGMP/MLD software
 * processing for upstream and downstream direction
 */
/** Extended VLAN rule handling for multicast IGMP/MLD packets in upstream
 *  direction
 */
#define DIR_US 0
/** Extended VLAN rule handling for multicast IGMP/MLD packets in downstream
 *  direction
 */
#define DIR_DS 1

/**
 * The operation performed in the downstream direction is the inverse of
 * that performed in the upstream direction. Which treatment and filter
 * fields are used for downstream filtering and the handling of unmatched
 * frames are left to the implementation of the ONU.
 */
#define DS_MODE_REVERSE 0

/*
 * Regardless of the filter rules, no operation is performed in the
 * downstream direction. All downstream frames are forwarded
 * unmodified. See ITU-T G.988 for more detail.
 */
#define DS_MODE_FORWARD 1

/**
 * Filter on VID and P-bit value. On a match, perform the inverse
 * operation on both the VID and P-bit value. If no match is found,
 * forward the frame unmodified.
 */
#define DS_MODE_VID_PBIT_OR_FORWARD 2

/**
 * Filter on VID only. On a match, perform the inverse VID operation
 * only; pass the P bits through. If no match is found, forward the frame
 * unmodified.
 */
#define DS_MODE_VID_OR_FORWARD 3

/**
 * Filter on P-bit only. On a match, perform the inverse P-bit operation
 * only; pass the VID through. If no match is found, forward the frame
 * unmodified.
 */
#define DS_MODE_PBIT_OR_FORWARD 4

/**
 * Filter on VID and P-bit value. On a match, perform the inverse
 * operation on both the VID and P-bit value. If no match is found, discard
 * the frame.
 */
#define DS_MODE_VID_PBIT_OR_DISCARD 5

/**
 * Filter on VID. On a match, perform the inverse operation on the VID
 * only. Pass the P-bits unmodified. If no match is found, discard the frame.
 */
#define DS_MODE_VID_OR_DISCARD 6

/**
 * Filter on P-bit only. On a match, perform the inverse P-bit operation
 * only; pass the VID through. If no match is found, discard the frame.
 */
#define DS_MODE_PBIT_OR_DISCARD 7

/* Regardless of the filter rules, discard all downstream traffic. */
#define DS_MODE_DISCARD 8

/** OMCC Socket thread shutdown wait time [ms]*/
#define SOCKET_THREAD_SHUTDOWN_WAIT	5000

/**
 * Multicast Operations Profile VLAN update structure
 */
struct mc_vlan_data {
	/** LAN port index (0 to 3) */
	uint16_t lan_idx;
	/** Upstream multicast VLAN tag control */
	uint8_t us_igmp_tag_ctrl;
	/** Upstream multicast TCI value */
	uint16_t us_igmp_tci;
	/** Downstream multicast VLAN tag control */
	uint8_t ds_igmp_mc_tag_ctrl;
	/** Downstream multicast TCI value */
	uint16_t ds_igmp_mc_tci;
};

/**
 * PON Network Library configuration, read by the initialization routine
 */
struct pon_net_config {
	/** MAC address of the OMCC network device PON-IP */
	uint8_t omcc_dev_mac[ETH_ALEN];
	/** MAC address of the OMCC network interface */
	uint8_t omcc_if_mac[ETH_ALEN];
	/** Names of the UNI network devices */
	char uni_name[LAN_PORT_MAX][IF_NAMESIZE];
	/** Names of multicast devices corresponding to UNI */
	char uni_mc_name[LAN_PORT_MAX][IF_NAMESIZE];
	/** Names of broadcast devices corresponding to UNI */
	char uni_bc_name[LAN_PORT_MAX][IF_NAMESIZE];
	/** MAC address of the corresponding UNI */
	uint8_t uni_macaddr[LAN_PORT_MAX][ETH_ALEN];
	/** Names of the VEIP network devices */
	char veip_name[VEIP_PORT_MAX][IF_NAMESIZE];
	/** Names of multicast devices corresponding to VEIP */
	char veip_mc_name[VEIP_PORT_MAX][IF_NAMESIZE];
	/** Names of broadcast devices corresponding to VEIP */
	char veip_bc_name[VEIP_PORT_MAX][IF_NAMESIZE];
	/** MAC address of the corresponding VEIP */
	uint8_t veip_macaddr[VEIP_PORT_MAX][ETH_ALEN];
	/** Names of LCT device */
	char lct_name[IF_NAMESIZE];
	/** Maximum number of LAN ports */
	uint32_t max_ports;
	/** PON mode */
	enum pa_pon_op_mode mode;
	/** VLAN forwarding */
	uint32_t vlan_forwarding;
	/** Do not create DS priority queues for VEIP and UNI */
	uint32_t no_ds_prio_queues;
	/** Create CPU queues */
	uint8_t enable_cpu_queues;
	/** Name of external switch device */
	char uni_ext_switch_name[IF_NAMESIZE];
	/** Name of UNI external switch master device */
	char uni_ext_master_name[LAN_PORT_MAX][IF_NAMESIZE];
	/* ID of the UNI that is connected to the external switch device */
	uint8_t uni_ext_id[LAN_PORT_MAX];
};

/**
 * PON Network Library OMCC handle
 */
struct pon_net_omcc_context {
	/** OMCC socket used to receive and send OMCI messages */
	struct pon_net_omcc_socket *omcc_socket;
	/** Socket thread control structure */
	pthread_t socket_thread;
	/** Reference to config.omcc-mac to use this context in the callback
	 *  routine instead of the general pon_net_context
	 */
	const uint8_t *p_mac;
	/** Callback routine to notify the higher layer about a received OMCI
	 *  message
	 */
	enum pon_adapter_errno (*receive_callback)(void *hl_handle,
						   const uint8_t *omci_msg,
						   const uint16_t len,
						   const uint32_t *crc);

	/** Higher level handle for callback (e.g., receive)*/
	void *hl_handle;
};

/* Data that will be stored in ctx->me_list */
struct tcont {
	/* T-CONT Managed Entity Id */
	uint16_t me_id;

	/* Tree builder. It will be called to build a new pon_net_qdiscs, which
	 * will be then applied and saved in the 'tree' field
	 */
	const struct pon_net_tree_builder *tree_builder;

	/* Set of qdiscs that are configured at a given moment */
	struct pon_net_qdiscs *tree;
};

/**
 * PMapper data that will be stored in ctx->me_list
 */
struct pmapper {
	/**
	 * The array contains GEM Port Network CTP
	 * Managed Entity IDs
	 */
	uint16_t gem_ctp_me_ids[PON_8021P_PCP_COUNT];
	/** Whether the Pmapper needs to reconnected to the bridge */
	bool needs_reconnect;
};

/** Traffic Descriptor Manage Entity Data
 * This structure is stored in &ctx->me_list
 */
struct pon_net_traffic_descriptor {
	/** CIR (committed information rate, in bytes/s) */
	uint32_t cir;
	/** PIR (peak information rate, in bytes/s) */
	uint32_t pir;
	/** CBS (committed block size, in bytes) */
	uint32_t cbs;
	/** PBS (peak block size, in bytes) */
	uint32_t pbs;
	/** Color mode */
	uint8_t color_mode;
	/** Ingress color marking */
	uint8_t ingress_color_marking;
	/** Egress color marking */
	uint8_t egress_color_marking;
	/** Meter type */
	uint8_t meter_type;
};

/**
 * Traffic Scheduler Managed Entity Data
 * This structure is stored in &ctx->me_list
 */
struct pon_net_traffic_scheduler {
	/** Managed Entity Id */
	uint16_t me_id;
	/** T-CONT pointer */
	uint16_t tcont_ptr;
	/** Traffic Scheduler pointer */
	uint16_t traffic_scheduler_ptr;
	/** Policy */
	uint8_t policy;
	/** Priority/weight */
	uint8_t priority_weight;
};

#define TS_HIERARCHY_MAX_HEIGHT 5

/**
 * Priority Queue attribute values.
 * This structure is stored in &ctx->me_list
 */
struct pon_net_priority_queue {
	/** Managed Entity Id */
	uint16_t me_id;
	/**
	 * In case of upstream Priority Queue this is
	 * T-CONT Managed Entity ID, and in case of downstream
	 * Priority Queue this is PPTP Eth UNI Managed Entity ID.
	 */
	uint16_t associated_me_id;
	/** The queue number in the driver */
	uint16_t queue;
	/** The traffic class number for this queue */
	uint16_t traffic_class;
	/** Weight of this Priority Queue (needed for WRR) */
	uint8_t weight;
	/** Traffic Scheduler ME ID */
	uint16_t traffic_scheduler_me_id;
	/**
	 * Array of traffic schedulers through which upstream packets will
	 * travel before reaching the T-CONT
	 */
	struct pon_net_traffic_scheduler chain[TS_HIERARCHY_MAX_HEIGHT];
	/** Number of traffic schedulers in 'chain' array */
	unsigned int chain_length;
	/** Index generated by mapper_id_map() */
	uint32_t index;
	/** Packet drop queue thresholds first value */
	uint32_t pkt_drop_q_thr_green_min;
	/** Packet drop queue thresholds second value */
	uint32_t pkt_drop_q_thr_green_max;
	/** Packet drop queue thresholds third value */
	uint32_t pkt_drop_q_thr_yellow_min;
	/** Packet drop queue thresholds fourth value */
	uint32_t pkt_drop_q_thr_yellow_max;
	/** The probability of dropping a green packet */
	uint8_t pkt_drop_probability_green;
	/** The probability of dropping a yellow packet */
	uint8_t pkt_drop_probability_yellow;
	/* Specifies how drop precedence is marked on ingress packets to the
	 * priority queue */
	uint8_t drop_precedence_color_marking;
};

/**
 * Bridge Port Config
 * This structure is stored in &ctx->me_list
 */
struct pon_net_bridge_port_config {
	/** Managed Entity Id */
	uint16_t me_id;
	/** Termination Point type */
	uint8_t tp_type;
	/** Termination Point pointer */
	uint16_t tp_ptr;
	/**
	 * This is ME ID of the actual termination point. For example:
	 * If tp_ptr points to GEM Port Interworking TP, then real_tp_ptr
	 * will point to GEM Port Network CTP associated with it. Same
	 * for Multicast GEM Port Interworking TP. For MEs which are connected
	 * directly the values of tp_ptr and real_tp_ptr are the same.
	 */
	uint16_t real_tp_ptr;
	/** MAC learning depth */
	uint8_t mac_learning_depth;
	/** MAC Bridge ME ID */
	uint16_t bridge_me_id;
	/** MAC Bridge Port ME ID */
	uint32_t bridge_idx;
	/** Outbound Traffic Descriptor pointer */
	uint16_t outbound_td_ptr;
	/** Inbound Traffic Descriptor pointer */
	uint16_t inbound_td_ptr;
	/**
	 * This is the first connection of this tp_type and real_tp_ptr to
	 * any bridge. This is used for connecting a single GEM Port Network
	 * CTP to multiple bridges. In this case one of them will have first
	 * equal to true, and others will have first equal to false.
	 */
	bool first;
	/**
	 * Bridge ifname, that the device is actually connected to. This can
	 * be different from the bridge pointed by bridge_me_id, in case when
	 * this bridge port represents a shared connection. In such case
	 * the bridge_me_id points to the bridge that this bridge port is
	 * connected from the OMCI perspective and br_ifname points to the
	 * "main" bridge - a bridge to which devices like, which model shared
	 * connection (like eth0_0_2, eth0_0_3) are connected.
	 */
	char br_ifname[IF_NAMESIZE];
	/** Interface ifname */
	char ifname[IF_NAMESIZE];
	/* Second interface name used when gem is connected to bridge with two
	 * UNI ports
	 */
	char ifname2[IF_NAMESIZE];
	/** Current downstream police configuration applied to HW. */
	struct netlink_police_data us_police;
	/** Current upstream police configuration applied to HW. */
	struct netlink_police_data ds_police;
	/**
	 * Set to true after a call to tp->connect() and to false after
	 * a call to tp->disconnect().
	 */
	bool connected;
};

/** Structure representing VLAN forwarding configuration parameters */
struct pon_net_vlan_forwarding {
	/** Array of VLAN TCIs */
	uint16_t tci_list[MAX_TCIS];
	/** Number of items in 'tci_list' array */
	uint8_t tci_list_len;
	/** TCI Mask */
	uint16_t tci_mask;
	/** Whether to configure untagged bridging */
	bool untagged_bridging;
};

/**
 * IP Host Config
 * This structure is stored in &ctx->me_list
 */
struct pon_net_ip_host {
	/** Flag this as data for the IPv6 ME */
	bool is_ipv6;
	/** Index to config section */
	uint32_t index;
	/** Name of config section */
	char cfg_name[15]; /* e.g. "ip6hostX" */
	/** "ifname" of used section */
	char ifname[IF_NAMESIZE];
	/** "bp_ifname" of used section */
	char bp_ifname[IF_NAMESIZE];
	/** Identifies netdevice status */
	bool is_up;
	/** Filter handles */
	uint32_t handles[7];
	/** Filter priorities */
	uint32_t prios[7];
	/* queue mapping priority */
	uint32_t queue_prio;
	/* queue mapping handle */
	uint32_t queue_handle;
	/** True if TC filter have been created */
	bool filters_configured;
};

/** Ethernet PMHD maximum number of counters index.
 *  This is equal to maximum number of attributes.
 */
#define ETH_PMHD_CNT_IDX_MAX 16
/** Ethernet PMHD maximum number of counters index */
#define ETH_PMHD_CNT_IDX_UNKNOWN -1

/**
 * Counters indexes.
 * This structure stores counter indexes used to parse ethtool counters.
 */
struct pon_net_cnt_idx {
	/** Counter indexes */
	int indexes[ETH_PMHD_CNT_IDX_MAX];
	/** True if indexes are valid */
	bool parsed;
};

static inline void pon_net_cnt_idx_defaults(struct pon_net_cnt_idx *cnt)
{
	unsigned int i = 0;

	cnt->parsed = false;
	for (i = 0; i < ARRAY_SIZE(cnt->indexes); ++i)
		cnt->indexes[i] = ETH_PMHD_CNT_IDX_UNKNOWN;
}

/** Ethernet Extended PMHD counter indexes */
struct pon_net_eth_ext_pmhd_cnt_idx {
	/** Extended receive counters indexes */
	struct pon_net_cnt_idx rx;
	/** Extended transmit counters indexes */
	struct pon_net_cnt_idx tx;
};

static inline void
pon_net_eth_ext_pmhd_cnt_idx_defaults(struct pon_net_eth_ext_pmhd_cnt_idx *cnt)
{
	pon_net_cnt_idx_defaults(&cnt->rx);
	pon_net_cnt_idx_defaults(&cnt->tx);
}

extern const struct pa_gem_interworking_tp_ops gem_itp_ops;
extern const struct pa_gem_port_net_ctp_ops gem_port_net_ctp_ops;
extern const struct pa_mac_bp_config_data_ops mac_bp_config_data_ops;
extern const struct pa_mac_bridge_service_profile_ops
	mac_bridge_service_profile_ops;
extern const struct pa_mac_bp_filter_preassign_table_ops
	mac_bp_filter_preassign_table_ops;
extern const struct pa_mac_bp_bridge_table_data_ops
	mac_bp_bridge_table_data_ops;
extern const struct pa_mc_gem_itp_ops mc_gem_itp_ops;
extern const struct pa_mc_profile_ops mc_profile_ops;
extern const struct pa_pptp_eth_uni_ops pptp_eth_uni_ops;
extern const struct pa_pptp_lct_uni_ops pptp_lct_uni_ops;
extern const struct pa_tcont_ops tcont_ops;
extern const struct pa_traffic_descriptor_ops traffic_descriptor_ops;
extern const struct pa_virtual_ethernet_interface_point_ops
	virtual_ethernet_interface_point_ops;
extern const struct pa_vlan_tagging_filter_data_ops
	vlan_tagging_filter_data_ops;
extern const struct pa_vlan_tagging_operation_config_data_ops
	vlan_tagging_operation_config_data_ops;
extern const struct pa_dot1p_mapper_ops pon_net_dot1p_mapper_ops;
extern const struct pa_ext_vlan_ops ext_vlan_ops;
extern const struct pon_adapter_vendor_emop_ops vendor_emop_ops;
extern const struct pa_priority_queue_ops priority_queue_ops;
extern const struct pa_traffic_scheduler_ops traffic_scheduler_ops;
extern const struct pa_gal_eth_profile_ops gal_eth_profile_ops;
extern const struct pa_onu2_g_ops pon_net_onu2_g_ops;
extern const struct pa_onu_g_ops pon_net_onu_g_ops;
extern const struct pa_ip_host_ops ip_host_ops;
extern const struct pa_eth_pmhd_ops eth_pmhd_ops;

extern const struct pa_msg_ops msg_ops;

/** PON Network Library context definition */
struct pon_net_context {
	/** PON Network Library specific configuration */
	struct pon_net_config cfg;
	/** Netlink context */
	struct pon_net_netlink *netlink;
	/** Array of mappers (for each ID type) */
	struct mapper *mapper[MAPPER_IDTYPE_MAX];
	/** MCC socket used to listen for IGMP and MLP packets */
	int mcc_s;
	/** Interface index for reinsertion netdev (ins0) */
	int mcc_ins_ifindex;
	/** Indicator that the MCC socket should listen
	 *  - true: MCC is running.
	 *  - false: MCC is stopped.
	 */
	bool mcc_running;
	/** Indicates whether the netifd service is currently running. */
	bool netifd_running;
	/** OMCC handle to support OMCI message send/receive */
	struct pon_net_omcc_context omcc_ctx;
	/** List of attribute structures for each Managed Entity */
	struct pon_net_me_list me_list;
	/** List of VLAN flows */
	struct pon_net_vlan_flow_list vlan_flow_list;
	/** Netlink socket for mcc */
	struct nl_sock *mcc_nl_sock;

#ifdef MCC_DRV_ENABLE
	/** Counter for the Multicast Operations Profile Managed Entity,
	 *  Provides the number of Managed Entity instances.
	 */
	int me_mop_cnt;
	/** IGMP or MLD version as defined
	 *  in the Multicast Operations Profile Managed Entity, values other
	 *  than specified are invalid.
	 *  - 1: IGMPv1 (deprecated version, shall not be used)
	 *  - 2: IGMPv2
	 *  - 3: IGMPv3
	 *  - 16: MLDv1
	 *  - 17: MLDv2
	 */
	int me_mop_igmp_version;
	/** Multicast GEM Interface Index */
	int mc_gem_ifindex;
	/** Multicast bridge Interface Index */
	int mc_br_ifindex;
	/** IGMP/MLD multicasting indication
	 *  - 0: DIS, multicast is disabled.
	 *  - 1: EN, multicast is enabled.
	 */
	int is_multicast_enabled;
	/** Synchronization of multicast thread with multicasting mode */
	sem_t multicast_enabled_event;
#endif

	/** Socket used for ioctl() messages */
	int ioctl_fd;

	/** Event callbacks for each event type set by higher layer */
	const struct pa_eh_ops *event_handlers;

	/** Higher layer context pointer */
	void *hl_handle;

	/** NetLink Route listening thread */
	pthread_t nl_route_thread;

	/** Number of created LCT interfaces */
	int num_lct;

	/** Ethernet Extended PMHD counter indexes
	 *  TODO: to be extended and stored per "class_id"
	 */
	struct pon_net_eth_ext_pmhd_cnt_idx eth_ext_pmhd_cnt_idx;

	/** Ethernet PMHD counter indexes */
	struct pon_net_cnt_idx eth_pmhd_cnt_idx;

	/** Device information database */
	struct pon_net_dev_db *db;

	/** OMCI Trap filter handle */
	uint32_t omci_trap_handle;

	/** OMCI Trap filter prio */
	uint32_t omci_trap_prio;

	/** CPU port filter handles */
	uint32_t pon_queue_handles[16];

	/** CPU port filter prios */
	uint32_t pon_queue_prios[16];

	/** PON net queue handles validity flag */
	bool pon_net_queue_handles_valid;

	/** VLAN Aware bridging configuration */
	struct pon_net_vlan_aware_bridging *vab;

	/** Callback functions provided by OMCI daemon. */
	const struct pa_config *pa_config;

	/** Ignore DS Extended VLAN rules priority matching */
	bool ignore_ds_rules_prio;
};

#ifdef MCC_DRV_ENABLE
/** Enable multicast if disabled;
 *
 * \param[in] ctx PON NET context pointer
 */
void pon_net_mcc_multicast_enable(struct pon_net_context *ctx);

/** Disable multicast if enabled;
 *
 * \param[in] ctx PON NET context pointer
 */
void pon_net_mcc_multicast_disable(struct pon_net_context *ctx);
#endif

/** Get network device name for Managed Entity
 *
 * \param[in]  ctx      PON Network Library context pointer
 * \param[in]  class_id Managed Entity Class ID
 * \param[in]  me_id    Managed Entity ID
 * \param[out] ifname   Network device name
 * \param[out] size     ifname size
 */
enum pon_adapter_errno pon_net_ifname_get(struct pon_net_context *ctx,
					  uint16_t class_id, uint16_t me_id,
					  char *ifname, size_t size);

/** Get network device name for the Termination Point
 *
 * \param[in]  ctx     PON Network Library context pointer
 * \param[in]  tp_type Termination Point Type
 * \param[in]  tp_ptr  Termination Point Pointer
 * \param[out] ifname  Network device name
 * \param[out] size    ifname size
 */
enum pon_adapter_errno pon_net_tp_ifname_get(struct pon_net_context *ctx,
					     uint8_t tp_type,
					     uint16_t tp_ptr,
					     char *ifname,
					     size_t size);

/**
 * Set a termination point network device up or down
 *
 * \param[in]  ctx     PON Network Library context pointer
 * \param[in]  tp_type Termination Point Type
 * \param[in]  tp_ptr  Termination Point Pointer
 * \param[out] state   State, 1 if up, 0 if down
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tp_state_set(struct pon_net_context *ctx,
					    uint8_t tp_type, uint16_t tp_ptr,
					    int state);

/** Get network device name for the Ext Vlan Associated ME
 *
 * \param[in]  ctx PON Network  Library context pointer
 * \param[in]  association_type Termination Point Type
 * \param[in]  associated_ptr   Termination Point Pointer
 * \param[out] ifname           Network interface device name
 * \param[out] size Buffer size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_associated_ifname_get(struct pon_net_context *ctx,
			      uint16_t association_type,
			      uint16_t associated_ptr,
			      char *ifname,
			      size_t size);


/**
 * The below codes are intended for pon_net_uni_ifname_get() function, that
 * returns names of UNI interfaces of interfaces closely related.
 */
/** Empty code */
#define UNI_UNSPEC 0
/** Code for name of VEIP interface */
#define UNI_VEIP 1
/** Code for name of PPTP Ethernet UNI interface */
#define UNI_PPTP_ETH 2
/** Code for name of LCT interface */
#define UNI_PPTP_LCT 3
/** Code for name of multicast interface corresponding to PPTP Ethernet UNI */
#define UNI_PPTP_MC 4
/** Code for name of multicast interface corresponding to VEIP */
#define UNI_VEIP_MC 5
/** Code for name of broadcast interface corresponding to PPTP Ethernet UNI */
#define UNI_PPTP_BC 6
/** Code for name of broadcast interface corresponding to VEIP */
#define UNI_VEIP_BC 7

/** Get network device name for UNI
 *
 * \param[in]  ctx PON Network  Library context pointer
 * \param[in]  uni_type		One of UNI_* codes
 * \param[in]  lport		Termination Point Pointer
 * \param[out] ifname           Network interface device name
 * \param[out] size Buffer size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_ifname_get(struct pon_net_context *ctx,
					      uint8_t uni_type,
					      uint16_t lport,
					      char *ifname, size_t size);

/** Get the network device name for a MAC Bridge Service Profile Managed Entity
 *
 * \param[in]  ctx         PON Network Library context pointer
 * \param[in]  bridge_idx  Bridge interface index
 * \param[out] ifname      Network interface device name
 * \param[out] size        Buffer size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_bridge_ifname_get_by_idx(struct pon_net_context *ctx,
				 unsigned int bridge_idx,
				 char *ifname,
				 size_t size);

/** Get the network device name for a MAC Bridge Service Profile Managed Entity
 *
 * \param[in]  ctx    PON Network Library context pointer
 * \param[in]  me_id  MAC Bridge Service Profile Managed Entity Id
 * \param[out] ifname Network interface device name
 * \param[out] size Buffer size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_bridge_ifname_get(struct pon_net_context *ctx,
						 uint16_t me_id,
						 char *ifname,
						 size_t size);

/** Get the LAN port index for a given UNI
 *
 * \param[in]  me_id       Managed Entity ID of PPTP Eth UNI or VEIP
 * \param[out] lport       LAN port index
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_uni_lport_get(const uint16_t me_id, uint16_t *lport);

/** Get PPTP Ethernet UNI interface name based on LAN port index
 *
 * \param[in] ctx		PON Network Library context pointer
 * \param[in] lport		LAN port index
 * \param[out] ifname		PPTP Ethernet UNI interface name
 * \param[in] size		ifname size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_pptp_eth_uni_name_get(struct pon_net_context *ctx,
			      uint16_t lport,
			      char *ifname,
			      size_t size);

/** Get PPTP LCT interface name based on LAN port index
 *
 * \param[in] ctx		PON Network Library context pointer
 * \param[in] lport		LAN port index
 * \param[out] ifname		PPTP LCT UNI interface name
 * \param[in] size		ifname size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_pptp_lct_uni_name_get(struct pon_net_context *ctx,
			      uint16_t lport,
			      char *ifname,
			      size_t size);

/** Get Virtual Ethernet Interface Point interface name based on LAN port index
 *
 * \param[in] ctx		PON Network Library context pointer
 * \param[in] lport		LAN port index
 * \param[out] ifname		Virtual Ethernet Interface Point interface name
 * \param[in] size		ifname size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_veip_name_get(struct pon_net_context *ctx,
		      uint32_t lport,
		      char *ifname,
		      size_t size);

/** Get the MAC address from UCI configuration for given interface
 *
 * \param[in] ctx		PON Network Library context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] lport		LAN port index
 * \param[out] mac		MAC address
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - PON_ADAPTER_ERR_NO_DATA: If no MAC address was configured
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_macaddr_get(struct pon_net_context *ctx,
					   uint16_t class_id, uint16_t lport,
					   uint8_t mac[ETH_ALEN]);

/**
 * Adds TBF qdiscs related to list of given gem port ctp me id
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] gem_ctp_me_ids	Array of size 'capacity' containing
 *				GEM CTP Managed Entity IDs
 * \param[in] capacity		Array capacity
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_gem_add_gem_tbf(struct pon_net_context *ctx,
					       const uint16_t *gem_ctp_me_ids,
					       unsigned int capacity);

/**
 * Get the LAN index for a specific interface name
 *
 * This will find UNI and VEIP interfaces from the mapper.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] ifname		Interface name
 *
 * \return LAN index or 0xFF error value
 */
uint8_t ifname_to_lan_idx(struct pon_net_context *ctx, const char *ifname);

/**
 * Get the LAN index for a specific interface index
 *
 * This will find UNI and VEIP interfaces from the mapper.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] ifindex		Interface index (Linux)
 *
 * \return LAN index or 0xFF error value
 */
uint8_t ifindex_to_lan_index(struct pon_net_context *ctx, int ifindex);

/**
 * Check if ME Id is in given array
 *
 * \param[in] id		ME Id
 * \param[in] array		Array of me_ids
 * \param[in] capacity		Length of the array
 *
 * \return 1 if id is in array, 0 otherwise
 */
int pon_net_me_id_in_array(uint16_t id, const uint16_t *array,
			   size_t capacity);

/**
 * Setup color marking
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] me_id		Managed Entity ID
 * \param[in] color_marking	Color marking value
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_color_marking_set(struct pon_net_context *ctx,
						 uint16_t class_id,
						 uint16_t me_id,
						 uint8_t color_marking);

/**
 * Setup police / meter action
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] me_id		Managed Entity ID
 * \param[in] dir		Direction of the police
 * \param[in] police		Police / meter settings to apply
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_police_set(struct pon_net_context *ctx,
					uint16_t class_id,
					uint16_t me_id,
					enum netlink_filter_dir dir,
					struct netlink_police_data *police);

/**
 * Delete police / meter action
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] me_id		Managed Entity ID
 * \param[in] dir		Direction of the police
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_police_unset(struct pon_net_context *ctx,
					    uint16_t class_id,
					    uint16_t me_id,
					    enum netlink_filter_dir dir);

/**
 * Unassign GEMs from Managed Entity's queues
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] me_id		Managed Entity ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_queue_unassign(struct pon_net_context *ctx,
					      uint16_t class_id,
					      uint16_t me_id);

/**
 * Update GEMs in Managed Entity's queues
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] class_id		Managed Entity Class ID
 * \param[in] me_id		Managed Entity ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_queue_update(struct pon_net_context *ctx,
					    uint16_t class_id, uint16_t me_id);

/**
 * Get Termination Point data for specified Managed Entity ID
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] me_id		Managed Entity ID
 * \param[out] tp_type		Termination type
 * \param[out] tp_ptr		Termination pointer
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
tp_index_get(struct pon_net_context *ctx,
	     const uint16_t me_id,
	     uint8_t *tp_type,
	     uint16_t *tp_ptr);

/**
 * Get MAC address of netdev pointed by name
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] ifname		Managed Entity ID
 * \param[out] mac_address	MAC address
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_get_mac_by_ifname(struct pon_net_context *ctx, char *ifname,
			  uint8_t mac_address[ETH_ALEN]);

/**
 * Returns the default qdisc tree builder
 *
 * The default qdisc builder is a tree builder, which is
 * used to build qdisc tree before
 *
 * \param[in] ctx		PON NET context pointer
 *
 * \return default qdisc tree builder
 */
const struct pon_net_tree_builder *
pon_net_get_default_qdisc_tree_builder(struct pon_net_context *ctx);

/**
 * Returns the External Switch downstream qdisc tree builder
 *
 * \param[in] ctx		PON NET context pointer
 *
 * \return mixed qdisc tree builder
 */
const struct pon_net_tree_builder *
pon_net_get_ext_uni_ds_qdisc_tree_builder(struct pon_net_context *ctx);

/**
 * Stops given thread using cancellation API
 *
 * \param[in] th_id		pointer to the pthread object
 * \param[in] th_name		thread name
 * \param[in] time		timeout [s] for closing the thread
 *
 * \return default qdisc tree builder
 */
enum pon_adapter_errno
pon_net_thread_stop(const pthread_t *th_id, const char *th_name, int time);

/** Create IP Host interface.
 *
 *  \param[in]  ctx                PON Network Library Context
 *  \param[in]  ifname             Interface name
 *  \param[in]  master             Master interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_iphost_create(struct pon_net_context *ctx,
					     const char *ifname,
					     const char *master);

/** Delete IP Host interface.
 *
 *  \param[in]  ctx                PON Network Library Context
 *  \param[in]  ifname             Interface name
 */
void pon_net_iphost_destroy(struct pon_net_context *ctx, const char *ifname);

/**
 * Setup multicast traps on CPU port
 *
 * \param[in] ctx		PON NET context pointer
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_mcc_cpu_traps_set(struct pon_net_context *ctx);

/**
 * Unset multicast traps on CPU port
 *
 * \param[in] ctx		PON NET context pointer
 */
void
pon_net_mcc_cpu_traps_unset(struct pon_net_context *ctx);

/**
 * Compare two arrays of uint16_t
 *
 * \param[in] array_a		First array
 * \param[in] array_b		Second array
 * \param[in] array_a_len	First array length
 * \param[in] array_b_len	Second array length
 *
 * \return returns value as follows:
 * - true: arrays are equal
 * - false: arrays are not equal
 */
bool pon_net_array_eq_u16(const uint16_t *array_a, unsigned int array_a_len,
			  const uint16_t *array_b, unsigned int array_b_len);

/** Structure used by \ref pon_net_pair_walk */
struct pon_net_pair_walk_args {
	/** Base pointer to the first array */
	void *first_array;
	/** Number of elements in the first array */
	unsigned int first_array_len;
	/** Base pointer to the second array */
	void *second_array;
	/** Number of elements in the second array */
	unsigned int second_array_len;
	/** Element size */
	unsigned int elem_size;
	/** Comparator */
	int (*cmp)(const void *item_a, const void *item_b, void *arg);
	/** User private data */
	void *arg;
	/** Function pointer executed on pair of each matched elements */
	enum pon_adapter_errno (*fn)(void *item_a, void *item_b, void *arg);
};

/**
 * Call 'fn' from \ref pon_net_pair_walk_args structure for pairs
 * of the same array items.
 *
 * The arrays must be sorted according to 'cmp' function from \ref
 * pon_net_pair_walk_args structure
 *
 * If a rule is present in 'second_array' but not in 'first_array', 'fn' will
 * be called with 'item_a' argument set to NULL
 * If a rule is present in 'first_array' but not in 'second_array', 'fn' will
 * be called with 'item_b' argument set to NULL
 * If a rule is present in both sets, 'fn' will be called with 'item_a'
 * from the 'first_array', and 'item_b' from the 'second_array'
 *
 * \param[in]     args	     arguments
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_pair_walk(const struct pon_net_pair_walk_args *args);

/** @} */ /* PON_NET_LIB_MACROS */

/** @} */ /* PON_NET_LIB */

#endif
