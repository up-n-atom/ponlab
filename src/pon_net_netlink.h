/*****************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file contains functions that use NETLINK_ROUTE to configure network
 * interfaces.
 */

#ifndef _PON_NET_NETLINK_H_
#define _PON_NET_NETLINK_H_

#include <stdint.h>
#include <net/if.h>
#include <stdbool.h>
#include <linux/if_ether.h>

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NETLINK PON Netlink Definitions
 *  These are commonly used definitions for the PON Netlink Library.
 *  @{
 */

struct nl_cache;
struct rtnl_link;
struct pon_net_netlink;

#define NETLINK_FILTER_COOKIE_SIZE 16

#define UNTAGGED 0
#define TAGGED 1

/** Generic filter actions */
enum netlink_filter_act {
	/** Pass packet action */
	NETLINK_FILTER_ACT_OK,
	/** Drop packet action */
	NETLINK_FILTER_ACT_DROP,
	/** VLAN action */
	NETLINK_FILTER_ACT_VLAN,
	/** Color marking action */
	NETLINK_FILTER_ACT_COLMARK,
	/** Mirred action */
	NETLINK_FILTER_ACT_MIRRED,
	/** Trap action */
	NETLINK_FILTER_ACT_TRAP,
	/** Police action */
	NETLINK_FILTER_ACT_POLICE,
	/** SKBEDIT action */
	NETLINK_FILTER_ACT_SKBEDIT,
};

/** VLAN actions */
enum netlink_filter_act_vlan {
	/** Remove a VLAN tag action */
	NETLINK_FILTER_ACT_VLAN_POP,
	/** Add a VLAN tag action */
	NETLINK_FILTER_ACT_VLAN_PUSH,
	/** Modify a VLAN tag action */
	NETLINK_FILTER_ACT_VLAN_MODIFY,
	/** Pop a tag and then modify */
	NETLINK_FILTER_ACT_VLAN_POP_AND_MODIFY,
	/** Modify and push */
	NETLINK_FILTER_ACT_VLAN_MODIFY_AND_PUSH,
};

/* Cookie used for Ext. Vlan */
struct netlink_cookie {
	/** Action cookie */
	unsigned char cookie[NETLINK_FILTER_COOKIE_SIZE];
};

/**
 * These are parameters specific to action colmark. This
 * structure should be assigned to netlink_filter.act_data
 * when netlink_filter.act is set to NETLINK_FILTER_ACT_COLMARK
 */
struct netlink_colmark_data {
	/** Color Mode */
	int mode;
	/** Color Marking */
	int drop_precedence;
	/** Meter Type */
	int meter_type;
};

/**
 * These are parameters specific to police / meter action. This
 * structure should be assigned to netlink_filter.act_data
 * when netlink_filter.act is set to NETLINK_FILTER_ACT_POLICE
 */
struct netlink_police_data {
	/* True if this setting is used */
	bool active;
	/* committed information rate, in bytes per second */
	uint32_t cir;
	/* peak information rate, in bytes per second */
	uint32_t pir;
	/* committed burst size, in bytes */
	uint32_t cbs;
	/*  peak burst size, in bytes */
	uint32_t pbs;
	/* Color marking data; */
	struct netlink_colmark_data colmark;
};

/**
 * These are parameters specific to action VLAN. This
 * structure should be assigned to netlink_filter.act_data
 * when netlink_filter.act is set to NETLINK_FILTER_ACT_VLAN
 * TODO: Adding 2 vlan tags at once is not supported. Setting
 *       cvlan fields will not have an effect at the moment
 */
struct netlink_vlan_data {
	/** VLAN ID */
	int vlan_id;
	/** VLAN Priority (PCP) */
	int vlan_prio;
	/** VLAN Protocol (TPID) */
	int vlan_proto;
	/** Second VLAN ID. This is intended for QinQ */
	int svlan_id;
	/** Second VLAN Priority (PCP). This is intended for QinQ */
	int svlan_prio;
	/** Second VLAN Ethernet Type. This is intended for QinQ */
	int svlan_eth_type;
	/** Action to be performed */
	enum netlink_filter_act_vlan act_vlan;
	/** Action cookie */
	struct netlink_cookie cookie;
};

/**
 * These are parameters specific to action MIRRED. This
 * structure should be assigned to netlink_filter.act_data
 * when netlink_filter.act is set to NETLINK_FILTER_ACT_MIRRED
 */
struct netlink_mirred_data {
	/** Device name for net_device. */
	char dev[IF_NAMESIZE];
	/** Mirred action */
	int action;
	/** Mirred policy */
	int policy;
	/** VLAN chained action */
	struct netlink_vlan_data *vlan_act;
};

/**
 * These are parameters specific to action SKBEDIT. This
 * structure should be assigned to netlink_filter.act_data
 * when netlink_filter.act is set to NETLINK_FILTER_ACT_SKBEDIT
 */
struct netlink_skbedit_data {
	/** skbedit action */
	int action;
	/** priority */
	int prio;
	/** Action cookie */
	char cookie[NETLINK_FILTER_COOKIE_SIZE];
	/** VLAN action data for combined VLAN and SKBEDIT action */
	struct netlink_vlan_data *vlan_act;
};

#define NETLINK_QDISC_RED_UNUSED (-1)
/** Structure used by \ref netlink_qdisc_red_create */
struct netlink_qdisc_red {
	int limit;
	int min;
	int max;
	int flags;
	int wlog;
	int plog;
	int scell_log;
	double max_p;
};

#define NETLINK_QDISC_TBF_UNUSED (-1)
/** Structure used by \ref netlink_qdisc_tbf_create */
struct netlink_qdisc_tbf {
	/* Committed information rate [bytes/second] */
	int cir;
	/* Peak information rate [bytes/second] */
	int pir;
	/* Committed burst size [bytes] */
	int cbs;
	/* Peak burst size [bytes] */
	int pbs;
	/*  Queue length */
	int lim;
};

/** Filter directions (the code relies on enum values) */
enum netlink_filter_dir {
	NETLINK_FILTER_DIR_NONE = 0,
	/** Ingress direction only (received into the device) */
	NETLINK_FILTER_DIR_INGRESS = 0x1,
	/** Egress direction only (sent out of the device) */
	NETLINK_FILTER_DIR_EGRESS = 0x2,
	/** Bidirectional (received or sent) */
	NETLINK_FILTER_DIR_BOTH =
		NETLINK_FILTER_DIR_INGRESS | NETLINK_FILTER_DIR_EGRESS
};

enum netlink_offload_flags {
	NETLINK_HW_ONLY = 0,
	NETLINK_SW_ONLY = 0x1,
	NETLINK_HW_SW = 0x2,
};

#define NETLINK_FILTER_UNUSED (-1)
#define IPV6_ALEN 16

/** Structure used by \ref netlink_filter_add */
struct netlink_filter {
	/** Device name */
	char device[IF_NAMESIZE];
	/**
	 *  Handle identifying filter:
	 *  Valid values are: 1-65536 or NETLINK_FILTER_UNUSED
	 */
	uint32_t handle;
	/** Filter priority */
	int prio;
	/** Filter on (outer) VLAN ID (VID).
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int vlan_id;
	/** Filter on (outer) VLAN priority (PCP).
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int vlan_prio;
	/** Filter on (outer) VLAN protocol (TPID value).
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int vlan_proto;
	/** Filter on inner VLAN ID.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int cvlan_id;
	/** Filter on inner VLAN priority.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int cvlan_prio;
	/** Filter on inner VLAN protocol (TPID value).
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int cvlan_proto;
	/** Match protocol. ETH_P_ALL matches all.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED,
	 *  which will actually be equivalent to ETH_P_ALL */
	int proto;
	/** Action to be performed if the filter conditions match. */
	enum netlink_filter_act act;
	/** Action related data */
	void *act_data;
	/** Traffic direction to which the filter/action shall be applied. */
	uint8_t dir;
	/** Device name for indev. Set the first character to \0 if unused. */
	char indev[IF_NAMESIZE];
	/** Class ID major number.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int classid_maj;
	/** Class ID minor number.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int classid_min;
	/** IPv4 type of service.
	 *  Can be omitted by setting it to NETLINK_FILTER_UNUSED.
	 */
	int ip_tos;
	/** Destination MAC enable */
	bool eth_dst;
	/** Destination MAC address */
	uint8_t eth_dst_addr[ETH_ALEN];
	/** Destination MAC address mask - not used if all fields are zero */
	uint8_t eth_dst_mask[ETH_ALEN];
	/** Skip hardware creation legally */
	enum netlink_offload_flags flags;
	/** IP protocol type */
	uint8_t ip_proto;
	/** ICMPV6 type */
	uint8_t icmpv6_type;
	/** Destination IPV4 enable */
	bool ipv4_dst_en;
	/** Destination IPV4 address */
	uint32_t ipv4_dst_addr;
	/** Destination IPV4 mask */
	uint32_t ipv4_dst_mask;
	/** Destination IPV6 enable */
	bool ipv6_dst_en;
	/** Destination IPV6 address */
	uint8_t ipv6_dst_addr[IPV6_ALEN];
	/** Destination IPV6 mask */
	uint8_t ipv6_dst_mask[IPV6_ALEN];
};

/* The NetLink messages the GEM and scheduler configuration for only entry on
 * the Bridge Port.
 * Ethernet (G.989), Ethernet unicast (G.984) */
#define NETLINK_GEM_TRAFFIC_TYPE_ETH 0
/* OMCI (G.984, G.989) */
#define NETLINK_GEM_TRAFFIC_TYPE_OMCI 1
/* Ethernet multicast (G.984 only) */
#define NETLINK_GEM_TRAFFIC_TYPE_ETHM 2
/* Encryption disabled (G.984, G989) */
#define NETLINK_GEM_ENC_DIS 0
/* Unicast payload encryption in both directions (G.989 only) or encrypted
 * (G.984)
 */
#define NETLINK_GEM_ENC_UC 1
/* Broadcast encryption (G.989 only) */
#define NETLINK_GEM_ENC_BC 2
/* Unicast encryption in downstream only (G.984, G.989) */
#define NETLINK_GEM_ENC_DSEN 3
/* Disabled (NETLINK_GEM port not used) */
#define NETLINK_GEM_DIR_DIS 0
/* Upstream (UNI-to-ANI) */
#define NETLINK_GEM_DIR_US 1
/* Downstream (ANI-to-UNI) */
#define NETLINK_GEM_DIR_DS 2
/* Bidirectional */
#define NETLINK_GEM_DIR_BI 3
#define NETLINK_GEM_UNUSED (-1)

/** Structure used by \ref netlink_gem_create and \ref netlink_gem_update */
struct netlink_gem {
	/** T-CONT Managed Entity id */
	int tcont_meid;
	/** Traffic type */
	int traffic_type;
	/** Direction */
	int dir;
	/** Encryption key ring */
	int enc;
	/** Max GEM payload size */
	int max_size;
	/** GEM port id */
	int port_id;
	/** Multicast GEM */
	int mc;
};

/** Queue lookup mode
 *
 *  The enum values are important as they are recognized by the driver
 **/
enum netlink_queue_lookup {
	NETLINK_QUEUE_LOOKUP_FLOW_ID_TC_4BIT = 0,
	NETLINK_QUEUE_LOOKUP_SUBIF_ID = 1,
	NETLINK_QUEUE_LOOKUP_SUBIF_ID_TC_4BIT = 2,
	NETLINK_QUEUE_LOOKUP_SUBIF_ID_TC_3BIT = 3,
};

/** Set queue lookup mode
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  mode                Queue lookup mode. One of QUEUE_LOOKUP_*
 *                                 defines
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_set_queue_lookup_mode(struct pon_net_netlink *nl_ctx,
			      enum netlink_queue_lookup mode);

/** Check if a device is connected to a bridge
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[out] is_slave            Set to 1 if it is connected, set to 0 if not
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_is_slave(struct pon_net_netlink *nl_ctx,
		 const char *ifname,
		 int *is_slave);

/** Destroys a network interface by name
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_netdevice_destroy(struct pon_net_netlink *nl_ctx,
						 const char *ifname);

/** Brings up a network interface by name
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  up                  If 1, then netdevice will be brought up,
 *                                 down otherwise
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_netdevice_state_set(struct pon_net_netlink *nl_ctx,
			    const char *ifname,
			    int up);

/** Mark network interface as multicast by name
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  enabled             If 1, then netdevice will marked as
 *				   multicast, otherwise not
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_multicast_state_set(struct pon_net_netlink *nl_ctx,
			    const char *ifname,
			    int enabled);

/** Checks if a network interface is up
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[out] up                  Netdevice operational state
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_netdevice_operstate_get(struct pon_net_netlink *nl_ctx,
				const char *ifname,
				bool *up);

/** Set mtu of network device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  max_frames_size     Max frame size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_netdevice_mtu_set(struct pon_net_netlink *nl_ctx,
						 const char *ifname,
						 unsigned int max_frames_size);

/** Set mac address of network device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  if_mac              MAC address
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_netdevice_mac_addr_set(struct pon_net_netlink *nl_ctx,
			       const char *ifname,
			       const uint8_t if_mac[ETH_ALEN]);

/** Enslaves slave to master. If master is an empty string, then slave is
 *  removed from its master
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  master              Master interface name
 * \param[in]  slave               Slave interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_netdevice_enslave(struct pon_net_netlink *nl_ctx,
			  const char *master,
			  const char *slave);

/** Returns ifindex for given network device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname		   Interface name
 *
 * \return returns value as follows:
 * - Positive ifindex for device identified by "ifname"
 * - Non-positive value if error has occurred
 */
int netlink_netdevice_ifindex_get(struct pon_net_netlink *nl_ctx,
				  const char *ifname);

/** Destroy qdisc
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  parent_id           Parent Qdisc. 0 if no parent
 * \param[in]  parent_queue        Parent Queue. Unused if parent_id is 0
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_qdisc_destroy(struct pon_net_netlink *nl_ctx,
					     const char *ifname, uint16_t id,
					     uint16_t parent_id,
					     uint16_t parent_queue);

/** Create prio qdisc
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  parent_id           Parent Qdisc. 0 if no parent
 * \param[in]  parent_queue        Parent Queue. Unused if parent_id is 0
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_qdisc_prio_create(struct pon_net_netlink *nl_ctx,
						 const char *ifname,
						 uint16_t id,
						 uint16_t parent_id,
						 uint16_t parent_queue);

/** Assign a GEM port to a queue
 *
 *  This is implemented by adding a tc-flower filter to the device
 *  specified as ifname.
 *
 * \param[in]  nl_ctx        PON Network Netlink Context
 * \param[in]  ifname        Interface name
 * \param[in]  id            Qdisc handle major number
 * \param[in]  queue_number  Queue number
 * \param[in]  indev         Interface which is assigned to queue
 * \param[in]  assignment_id ID of this assignment
 * \param[in]  vlan_prio     PCP value corresponding to this queue number.
 *                           Must be set to NETLINK_FILTER_UNUSED if unspecified
 * \param[in]  tc            Traffic class corresponding to this queue number.
 *                           Must be set to NETLINK_FILTER_UNUSED if unspecified
 * \param[in]  prio          Priority of the filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_queue_assign(struct pon_net_netlink *nl_ctx, const char *ifname,
		     uint32_t id, uint16_t queue_number, const char *indev,
		     uint32_t assignment_id, int vlan_prio, int tc,
		     uint16_t prio);

/** Initialize a netlink_qdisc_red structure with default values
 *
 * \param[in]  red              RED settings to initialize with default values
 */
void netlink_qdisc_red_defaults(struct netlink_qdisc_red *red);

/** Initialize a netlink_qdisc_red structure with default values
 *
 * \param[out] red               RED settings to initialize with default values
 * \param[in]  min               Min threshold
 * \param[in]  max               Max threshold
 * \param[in]  probability       Probability
 * \param[in]  limit             HARD maximal queue length
 * \param[in]  scell_log         Cell size for idle damping
 * \param[in]  wlog              log(W) W is the filter time constant.
 *                               To allow larger bursts, W should be decreased.
 * \param[in]  flags             Flags
 */
void netlink_qdisc_red_p_and_thr_set(struct netlink_qdisc_red *red,
				     int min,
				     int max,
				     double probability,
				     int limit,
				     int scell_log,
				     int wlog,
				     int flags);

/** Create red qdisc
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  parent_id           Parent Qdisc. 0 if no parent
 * \param[in]  parent_queue        Parent Queue. Unused if parent_id is 0
 * \param[in]  params              Qdisc RED data structure
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_qdisc_red_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t parent_id,
			 uint16_t parent_queue,
			 const struct netlink_qdisc_red *params);

/** Initialize a netlink_qdisc_tbf structure with default values
 *
 * \param[in]  tbf              TBF settings to initialize with default values
 */
void netlink_qdisc_tbf_defaults(struct netlink_qdisc_tbf *tbf);

/** Initialize a netlink_qdisc_tbf structure with given values
 *
 * \param[out] tbf                 TBF settings to initialize with given values
 * \param[in]  cir                 Committed information rate
 * \param[in]  pir                 Peak information rate
 * \param[in]  cbs                 Committed burst size
 * \param[in]  pbs                 Peak burst size
 * \param[in]  lim                 Queue length
 */
void netlink_qdisc_tbf_params_set(struct netlink_qdisc_tbf *tbf,
				  int cir, int pir,
				  int cbs, int pbs,
				  int lim);

/** Create tbf qdisc
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  parent_id           Parent Qdisc. 0 if no parent
 * \param[in]  parent_queue        Parent Queue. Unused if parent_id is 0
 * \param[in]  params              Qdisc TBF data structure
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_qdisc_tbf_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t parent_id,
			 uint16_t parent_queue,
			 const struct netlink_qdisc_tbf *params);

/** Adds clsact qdisc to interface, if no qdisc is added does nothing
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_qdisc_clsact_create(struct pon_net_netlink *nl_ctx,
			    const char *ifname);

/** Destroy clsact
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_qdisc_clsact_destroy(struct pon_net_netlink *nl_ctx,
			     const char *ifname);

/** Create drr qdisc to interface
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  parent_id           Parent Qdisc. 0 if no parent
 * \param[in]  parent_queue        Parent Queue. Unused if parent_id is 0
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_qdisc_drr_create(struct pon_net_netlink *nl_ctx,
						const char *ifname, uint16_t id,
						uint16_t parent_id,
						uint16_t parent_queue);

/** Create drr class
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  Qdisc handle major number
 * \param[in]  queue_number        Queue number
 * \param[in]  quantum	           Drr quantum parameter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_class_drr_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t queue_number,
			 uint32_t quantum);

/** Destroy all classes connected to qdisc
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  id                  qdisc handle major number
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_class_clear_all(struct pon_net_netlink *nl_ctx,
			const char *ifname,
			uint16_t id);

/** Destroy one class
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  parent              qdisc handle major number
 * \param[in]  traffic_class       qdisc handle major number
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_class_clear_one(struct pon_net_netlink *nl_ctx,
			const char *ifname,
			uint16_t parent,
			uint16_t traffic_class);

/** Create an IEEE 802.1p mapper device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  pmapper_index       802.1p Mapper Service Profile Index
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_pmapper_create(struct pon_net_netlink *nl_ctx,
					      uint16_t pmapper_index);

/**
 * \param[in]  nl_ctx          PON Network Netlink Context
 * \param[in]  pmapper_index   IEEE 802.1p Mapper Service Profile Index
 * \param[in]  default_pcp     A default PCP value
 * \param[in]  gem_ctp_me_ids  Array of size 8 containing
 *                             GEM CTP Managed Entity IDs
 * \param[in]  dscp_to_pbit    Array of size 64 containing
 *                             DSCP to Pbit mapping
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_pmapper_setup(struct pon_net_netlink *nl_ctx,
					     uint16_t pmapper_index,
					     uint8_t default_pcp,
					     uint16_t *gem_ctp_me_ids,
					     uint8_t *dscp_to_pbit);

/** Sets an IEEE 802.1p mapper port to "up" state
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  pmapper_index       802.1p Mapper Service Profile Index
 * \param[in]  up                  If 1 GEM port will go up, down otherwise
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_pmapper_state_set(struct pon_net_netlink *nl_ctx,
			  uint16_t pmapper_index, int up);

/** Destroy an IEEE 802.1p mapper device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  pmapper_index       802.1p Mapper Service Profile Index
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_pmapper_destroy(struct pon_net_netlink *nl_ctx,
					       uint16_t pmapper_index);

/** Check if pmapper has GEM ports
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname		   Interface name
 * \param[out]  result		   If pmapper has GEM ports
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_pmapper_has_gem_ports(struct pon_net_netlink *nl_ctx,
			      const char *ifname, bool *result);

/** Create a tcont_omci interface
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_pon_net_omci_tcont_init(struct pon_net_netlink *nl_ctx);

/** Destroy a tcont_omci interface
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 */
void netlink_pon_net_omci_tcont_destroy(struct pon_net_netlink *nl_ctx);

/** Create a gem_omci interface
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  if_mac              OMCC interface MAC address
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_pon_net_omci_gem_init(struct pon_net_netlink *nl_ctx,
			      const uint8_t if_mac[ETH_ALEN]);

/** Destroy a gem_omci interface
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 */
void netlink_pon_net_omci_gem_destroy(struct pon_net_netlink *nl_ctx);

/** Initialize a netlink_gem structure with default values
 *
 * \param[in]  gem              GEM settings to initialize with default values
 */
void netlink_gem_defaults(struct netlink_gem *gem);

/** Create a network device representing a GEM port
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  gem_meid            GEM Port Managed Entity ID
 * \param[in]  gem                 GEM Settings
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_gem_create(struct pon_net_netlink *nl_ctx,
					  uint32_t gem_meid,
					  const struct netlink_gem *gem);

/** Assign an Allocation ID to a GEM port
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  gem_meid            GEM Managed Entity ID
 * \param[in]  gem                 GEM Settings
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_gem_update(struct pon_net_netlink *nl_ctx,
					  uint16_t gem_meid,
					  const struct netlink_gem *gem);

/** Set a GEM port into "up" state
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  gem_meid            GEM Port Managed Entity ID
 * \param[in]  up                  If 1 GEM port will go up, down otherwise
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_gem_state_set(struct pon_net_netlink *nl_ctx,
					     uint16_t gem_meid, int up);

/** Destroy a GEM port
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  gem_index           GEM index
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_gem_destroy(struct pon_net_netlink *nl_ctx,
					   uint16_t gem_index);

/** Create a network device representing a T-CONT
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  me_id		   T-CONT Managed Entity ID
 * \param[in]  alloc_id		   Alloc ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_tcont_create(struct pon_net_netlink *nl_ctx, uint16_t me_id,
		     uint16_t alloc_id);

/** Destroy a T-CONT network device
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  me_id		   T-CONT Managed Entity ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_tcont_destroy(struct pon_net_netlink *nl_ctx, uint16_t me_id);

/** Bridge settings */
struct netlink_bridge {
#define NETLINK_BRIDGE_vlan_filtering BIT(0)
#define NETLINK_BRIDGE_vlan_protocol BIT(1)
#define NETLINK_BRIDGE_vlan_stats_enabled BIT(2)
	/** Mask indicating, which field is set */
	uint32_t mask;
	/** Whether to enable VLAN filtering */
	bool vlan_filtering;
	/**
	 * Which vlan protocol to use
	 *
	 * Valind values:
	 * - ETH_P_8021Q
	 * - ETH_P_8021AD
	 */
	uint16_t vlan_protocol;
	/** Whether to enable VLAN stats */
	bool vlan_stats_enabled;
};

/** Create a bridge
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Bridge name
 * \param[in]  params              Struct containing optional parameters or NULL
 *                                 if there are not parameters
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bridge_create(struct pon_net_netlink *nl_ctx, const char *ifname,
		      const struct netlink_bridge *params);

/** Add a filter
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  filter              Filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_filter_add(struct pon_net_netlink *nl_ctx,
					  const struct netlink_filter *filter);

/** Remove filter
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  filter              Filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_filter_del(struct pon_net_netlink *nl_ctx,
					  struct netlink_filter *filter);

/** Remove one filter
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 * \param[in]  ifname              Interface name
 * \param[in]  protocol            Protocol type
 * \param[in]  priority            Priority of the filter
 * \param[in]  handle              Handle of the filter
 * \param[in]  dir                 Direction
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_filter_clear_one(struct pon_net_netlink *nl_ctx,
						const char *ifname,
						uint16_t protocol,
						uint16_t priority,
						uint32_t handle,
						enum netlink_filter_dir dir);

/** Remove all interfaces that were created by the PON Network Library
 *
 * \param[in]  nl_ctx              PON Network Netlink Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_netdevices_clear_all(struct pon_net_netlink *nl_ctx);

/** Initialize a netlink_filter structure with default values
 *
 * \param[in]  filter              Filter to initialize with default values
 */
void netlink_filter_defaults(struct netlink_filter *filter);

/** Initialized netlink_vlan structure with default values
 *
 * \param[in]  data                VLAN data to initialize with default values
 */
void netlink_vlan_defaults(struct netlink_vlan_data *data);

/** Initialized netlink_skbedit structure with default values
 *
 * \param[in]  data                Initialize SKBEDIT data with default values
 */
void netlink_skbedit_defaults(struct netlink_skbedit_data *data);

/** Get an IEEE 802.1p mapper interface name by its index.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifindex            Interface index
 * \param[out] name               Interface name
 * \param[in]  len                Interface name length
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_ifindex_to_name(struct pon_net_netlink *nl_ctx, int ifindex, char *name,
			size_t len);

/** Get an interface index for a given interface name
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  name               Interface name
 *
 * \return returns value as follows:
 * - positive value as interface index
 * - 0 if the given name do not represent any interface index
 */
int netlink_name_to_ifindex(struct pon_net_netlink *nl_ctx, char *name);

/** Set Multicast destination and source lookup
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifidx_bp           Interface index
 * \param[in]  dest_lookup        True to activate multicast destination lookup
 * \param[in]  src_lookup         True to activate multicast source lookup
 * \param[in]  learning           True to activate learning in this port
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bridge_port_cfg(struct pon_net_netlink *nl_ctx, int ifidx_bp,
			bool dest_lookup, bool src_lookup, bool learning);

/** Set Multicast bridge flood traffic configuration
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifidx_bp           bridge network Interface index
 * \param[in]  enable             True to activate multicast flooding
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bridge_mcast_flood(struct pon_net_netlink *nl_ctx, int ifidx_bp,
			   bool enable);

/** Set MAC learning limits for an interface by its name.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  omci_depth_limit   Limit to set (0 for 'unlimited learning')
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bport_learning_limit_set(struct pon_net_netlink *nl_ctx,
				 const char *ifname,
				 uint8_t omci_depth_limit);

/** Create a new vlan filter entry
 *
 * This is the equivalent of 'bridge vlan add' from iproute2.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  vlan               VLAN ID that identifies the vlan
 * \param[in]  self               If 'true' the vlan is configured on the
 *                                specified physical device. Required if the
 *                                device is the bridge device. If 'false' the
 *                                vlan is configured on the software bridge.
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bport_vlan_add(struct pon_net_netlink *nl_ctx,
		       const char *ifname,
		       uint16_t vlan,
		       bool self);

/** Remove an existing vlan filter entry
 *
 * This is the equivalent of 'bridge vlan del' from iproute2.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  vlan               VLAN ID that identifies the vlan
 * \param[in]  self               If 'true' the vlan is configured on the
 *                                specified physical device. Required if the
 *                                device is the bridge device. If 'false' the
 *                                vlan is configured on the software bridge.
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_bport_vlan_del(struct pon_net_netlink *nl_ctx,
		       const char *ifname,
		       uint16_t vlan,
		       bool self);

/** Initialize a netlink_colmark_data structure with default values
 *
 * \param[in]  colmark             colmark settings to initialize with default
 *                                 values
 */
void netlink_colmark_defaults(struct netlink_colmark_data *colmark);

/** Setup color marking on a network interface
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  dir                Direction
 * \param[in]  color_marking      Color marking value
 * \param[in]  handle             Handle of the created filter
 * \param[in]  priority           Priority of the created filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_color_marking_set(struct pon_net_netlink *nl_ctx, const char *ifname,
			  enum netlink_filter_dir dir,
			  uint8_t color_marking,
			  uint32_t handle, uint16_t priority);

/** Setup police / meter on a network interface
 *
 * \param[in]  nl_ctx           PON Network Netlink Context
 * \param[in]  ifname		Interface name
 * \param[in]  dir		Direction
 * \param[in]  handle		Handle of the created filter
 * \param[in]  priority		Priority of the created filter
 * \param[in]  police		Police / meter settings to be applied
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_police_set(struct pon_net_netlink *nl_ctx, const char *ifname,
		   enum netlink_filter_dir dir,
		   uint32_t handle, uint16_t priority,
		   struct netlink_police_data *police);

/** Setup OMCI trap on a network interface
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  dir                Direction
 * \param[in]  handle             Handle of the created filter
 * \param[in]  priority           Priority of the created filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_omci_trap_set(struct pon_net_netlink *nl_ctx, const char *ifname,
		      enum netlink_filter_dir dir,
		      uint32_t handle, uint16_t priority);

/** Setup Trap on a network interface
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  dir                Direction
 * \param[in]  hw_tc              HW traffic class
 * \param[in]  proto              Protocol
 * \param[in]  handle             Handle of the created filter
 * \param[in]  priority           Priority of the created filter
 * \param[in]  vlan_id            Vlan Id
 * \param[in]  eth_dst            Destination MAC Address
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_trap_set(struct pon_net_netlink *nl_ctx,
					const char *ifname, uint8_t dir,
					uint32_t hw_tc, int proto,
					uint32_t handle, int priority,
					int vlan_id, const uint8_t *eth_dst);

/** Create IP Host interface.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 * \param[in]  master             Master interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_iphost_create(struct pon_net_netlink *nl_ctx,
		      const char *ifname,
		      const char *master);

/** Delete IP Host interface.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_iphost_destroy(struct pon_net_netlink *nl_ctx,
					      const char *ifname);

/**
 * Initialize MC ipv4 drop filter
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             MC GEM interface name
 * \param[in]  proto              Protocol
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error
 */
enum pon_adapter_errno
netlink_filter_mc_unspec_ipv4_drop(struct netlink_filter *flt,
				   const char *ifname, int proto);

/**
 * Initialize MC ipv6 drop filter
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             MC GEM interface name
 * \param[in]  proto              Protocol
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error
 */
enum pon_adapter_errno
netlink_filter_mc_unspec_ipv6_drop(struct netlink_filter *flt,
				   const char *ifname, int proto);

/**
 * Initialize LCT ptp trap filter
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             MC GEM interface name
 * \param[in]  tagged             Tagged packets flag
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error
 */
enum pon_adapter_errno netlink_filter_lct_trap_ptp(struct netlink_filter *flt,
						   const char *ifname,
						   bool tagged);

/**
 * Initialize the ARP filter
 *
 * The 'act_data' must have a larger lifetime than 'flt'
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             MC GEM interface name
 * \param[in]  hw_tc              Traffic class
 * \param[in]  tagged             Tagged packets flag
 * \param[in]  act                Action
 * \param[in]  act_data           Action data
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error
 */
enum pon_adapter_errno netlink_filter_arp(struct netlink_filter *flt,
					  const char *ifname, uint32_t hw_tc,
					  bool tagged,
					  enum netlink_filter_act act,
					  void *act_data);

/** Fill filter struct for IGMP trap on PON master device.
 *
 * \param[in]  flt                Filter
 * \param[in]  tagged             tagged packets flag
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_filter_igmp_trap(struct netlink_filter *flt, bool tagged);

/** Fill filter struct with ICMPV6 trap for unique type on PON master device.
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             net device name
 * \param[in]  hw_tc              traffic class
 * \param[in]  type               filter type
 * \param[in]  tagged             tagged packets flag
 * \param[in]  act                action
 * \param[in]  act_data           action data
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_filter_icmpv6(struct netlink_filter *flt,
					     const char *ifname, uint32_t hw_tc,
					     uint8_t type, bool tagged,
					     enum netlink_filter_act act,
					     void *act_data);

/** Fill filter with IP Host trap for VLAN id on IPHOST master device.
 *
 * \param[in]  flt                Netlink filter
 * \param[in]  ifname             Interface name
 * \param[in]  hw_tc              Traffic class
 * \param[in]  vlan_id            IP Host VLAN id
 * \param[in]  act                Action
 * \param[in]  act_data           Action data
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno
netlink_filter_vlan(struct netlink_filter *flt,
		    const char *ifname, uint32_t hw_tc,
		    int vlan_id,
		    enum netlink_filter_act act,
		    void *act_data);

/** Setup filter for MAC address
 *
 * \param[in]  flt                Filter
 * \param[in]  ifname             Interface name
 * \param[in]  dir                Direction
 * \param[in]  hw_tc              Traffic class
 * \param[in]  proto              Protocol
 * \param[in]  vlan_id            Vlan Id
 * \param[in]  eth_dst            Pointer to MAC address
 * \param[in]  act                Action
 * \param[in]  act_data           Action data
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_filter_mac(struct netlink_filter *flt,
					  const char *ifname, uint8_t dir,
					  uint32_t hw_tc, int proto,
					  int vlan_id, const uint8_t *eth_dst,
					  enum netlink_filter_act act,
					  void *act_data);

/** Check if netdevice exists.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 * \param[in]  ifname             Interface name
 *
 * \return returns a boolean value that indicates
 *         whether netdevice exists on the system
 */
bool netlink_netdevice_exists(struct pon_net_netlink *nl_ctx,
			      const char *ifname);

/** Refreshes info about all netdevices, getting the info from kernel.
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An appropriate code in case of error.
 */
enum pon_adapter_errno netlink_cache_refill(struct pon_net_netlink *nl_ctx);

/** Initialize netlink channel.
 *
 * \return returns pointer to netlink context
 */
struct pon_net_netlink *netlink_create(void);

/** Close netlink channel
 *
 * \param[in]  nl_ctx             PON Network Netlink Context
 */
void netlink_destroy(struct pon_net_netlink *nl_ctx);

/** @} */ /* PON_NETLINK */

/** @} */ /* PON_NET_LIB */
#endif
