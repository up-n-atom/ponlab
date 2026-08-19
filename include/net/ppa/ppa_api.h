/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PPA_API_H__
#define __PPA_API_H__
/*******************************************************************************
 **
 ** FILE NAME	: ppa_api.h
 ** DESCRIPTION	: PPA Protocol Stack Hook API Header File
 ** COPYRIGHT	: Copyright (c) 2020-2024 MaxLinear, Inc.
 **		  Copyright (c) 2017 Intel Corporation.
 ** 		  Copyright (c) 2010 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 **
 *******************************************************************************/
/*! \file ppa_api.h
	\brief This file contains es.
	provide PPA API.
 */
/** \defgroup PPA_API PPA Kernel Hook and Userspace Function API
	\brief PPA is a loadable network module. Hence, it exports its API though function pointer hooks. Callers need to check that hooks are non-NULL before invoking them. The hooks are initialized when the PPA is initialized. Certain API which are control / configuration related are also exposed to user space applications through the ioctl API. The PPA Kernel and Userspace API are discussed in the following sections:
 */
/* @{*/
/** \defgroup PPA_IOCTL PPA Userspace API
	\brief The subset of PPA API which is exposed to userspace for control and configuration of the PPA is invoked through
	an ioctls()/system call interface as described in this section.
	The API is defined in the following two source files:
	- ppa_api.h: Header file for PPA API
	- ppa_api.c: C Implementation file for PPA API
 */
/** \defgroup PPA_HOOK_API PPA Hook API
	\brief PPA is a loadable network module. Hence, it exports its API though function pointer hooks. Callers need to check that hooks are non-NULL before invoking them. The hooks are initialized when the PPA is initialized.
	- ppa_hook.h: Header file for PPA API
	- ppa_hook.c: C Implementation file for PPA API
 */
/** \defgroup PPA_PWM_API PPA Power Management API
	\brief PPA Power Management API provide PPA Power Management and IOCTL API
	The API is defined in the following two source files
	- ppa_api_pwm.h: Header file for PPA API
	- ppa_api_pwm.c: C Implementation file for PPA Power management API
	- ppa_api_pwm_logic.c: C impelementation file for Powr management Logic and interface with PPE driver
 */
/** \defgroup PPA_ADAPTATION_LAYER PPA Stack Adaptation API
	\brief PPA module aims for OS and Protocol stack independence, and the
	core PPA logic does not access any OS or Protocol stack implementation
	specific structures directly. The PPA Protocol Stack Adaptation layer
	provides API that allows for straight-forward and structured OS / protocol
	stack porting of the PPA just by porting the Adaptation Layer (AL) API.
	The AL API is defined in the following two source files
	- ppa_stack_al.h: Header file for AL layer
	- ppa_stack_al.c: C Implementation file for AL API
 */
/* @}*/
#include <linux/types.h>
#include <net/ppa/ppa_api_common.h>
#include <uapi/net/ppa_api.h>
#include <net/ppa/ppa_stack_al.h>
/*
 * ####################################
 * Definition
 * ####################################
 */
#ifdef NO_DOXY
#define VLAN_ID_SPLIT(full_id, pri, cfi, vid)	do { pri = ((full_id) >> 13) & 7; cfi = ((full_id) >> 12) & 1; vid = (full_id) & 0xFFF } while (0)
#define VLAN_ID_CONBINE(full_id, pri, cfi, vid) full_id	= (((uint16_t)(pri) & 7) << 13) | (((uint16_t)(cfi) & 1) << 12) | ((uint16_t) (vid) & 0xFFF)
#define PPA_JIFFY_MILLSEC(x, hz) (x * 1000 / (hz))
#define WRAPROUND_32BITS ((uint64_t)0xFFFFFFFF)
#define WRAPROUND_64BITS ((uint64_t)0xFFFFFFFFFFFFFFFF)
#define WRAPROUND_SESSION_MIB ((uint64_t)0x1FFFFFE0) /*note, 0xFFFFFF * 0x20. In PPE FW, 1 means 32 bytes, ie 0x20 this value will be different with GRX500 platform */

#if IS_ENABLED(CONFIG_INTEL_IPQOS_MPE_DS_ACCEL)
/* Flag to extract bit 28 of extmark to check if DS_QOS is enabled for this session*/
#define SESSION_DS_QOS_SET				0x10000000

/* Flag to extract bit 17 of extmark that specifies if Ingress / Egress QoS enabled for sesssion */
#define SESSION_ING_EGR_QOS				0x00020000

/* Flag to extract bit 18 of extmark that specifies if classification is done from PCE or iptables */
#define SESSION_PCE_CLASSIFICATION		0x00040000

/* Flag to extract bits 19-20 of extmark that specifies if TC bits are set for Ingress Qs / Egress Qs / Both (Ingress & Egress Qs) */
#define SESSION_ING_TC_SET				0x00080000
#define SESSION_EGR_TC_SET				0x00100000

/* Flag to extract bits 21-22 of extmark that specifies the Ingress group of the session */
#define SESSION_ING_GROUP				0x00600000

/* Flag to extract bits 23-27 of extmark that are set with TC of Ingress Qs */
#define SESSION_INGRESS_TC				0x0F800000

#define SESSION_FLAG2_DS_MPE_QOS		0x00008000 //Flag for DS_QOS capab

#define ING_TC_SET_POS					19
#define EGR_TC_SET_POS					20
#define ING_GROUP_POS					21
#define INGRESS_TC_BIT_POS				23
#endif /* CONFIG_INTEL_IPQOS_MPE_DS_ACCEL */

#if IS_ENABLED(WMM_QOS_CONFIG)
typedef int (*PPA_QOS_CLASS2PRIO_CB)(int32_t , PPA_NETIF *, uint8_t *);
#endif
typedef enum {
	PPE_HAL = 0,
	PAE_HAL,
	MPE_HAL,
	TMU_HAL,
	LRO_HAL,
	DSL_HAL,
	SWAC_HAL,
	PUMA_HAL,
	PPV4_HAL,
	MAX_HAL
} PPA_HAL_ID;
typedef enum {
	SESS_BRIDG = 1,
	SESS_IPV4,
	SESS_IPV6,
	SESS_MC_DS,
	SESS_MC_DS_VAP,
	SESS_LOCAL_IN,
	SESS_LOCAL_OUT,
	TUNNEL_6RD,
	TUNNEL_DSLITE,
	TUNNEL_L2TP_US,
	TUNNEL_L2TP_DS,
	TUNNEL_ENCRYPT,
	TUNNEL_DECRYPT,
	TUNNEL_GRE_US,
	TUNNEL_GRE_DS,
	TUNNEL_VXLAN,
	TUNNEL_IPSEC_US,
	TUNNEL_IPSEC_DS,
	TUNNEL_IPSEC_MIB,
	QOS_INIT,
	QOS_UNINIT,
	QOS_CLASSIFY,
	QOS_QUEUE,
	QOS_INGGRP,
	Q_SCH_WFQ,
	Q_SCH_SP,
	Q_DROP_DT,
	Q_DROP_RED,
	Q_DROP_WRED,
	Q_DROP_CODEL,
	Q_SHAPER,
	PORT_SHAPER,
	QOS_LAN_CLASSIFY,
	QOS_LAN_QUEUE,
	QOS_WMM_INIT,
	QOS_WMM_UNINIT,
	XDSL_PHY,
	SESS_NAT_LOOPBACK,
	LRO_ENG,
#if IS_ENABLED(CONFIG_INTEL_IPQOS_MPE_DS_ACCEL)
	MPE_DS_QOS,
#endif
	TUNNEL_MAP_E,
	QOS_METER,
	MAX_CAPS
} PPA_API_CAPS;

typedef struct ppa_hsel_cnode{
	PPA_HLIST_NODE	cap_list;
	uint8_t wt;
	PPA_HAL_ID hal_id;
	PPA_API_CAPS cap;
} PPA_HSEL_CAP_NODE;
#define MAX_TUNNEL_ENTRIES 16
#define FLAG_SESSION_HI_PRIO 0x0001
#define FLAG_SESSION_SWAPPED 0x0002
#define FLAG_SESSION_LOCK_FAIL 0x0004
/* tunnel table datastructures*/
/* PPA default values */
#define PPA_IPV4_HDR_LEN 20
#define PPA_IPV6_HDR_LEN 40
#define PPA_VLAN_PROTO_8021Q 0x8100
#define PPA_VLAN_PROTO_8021AD 0x88A8
#endif /*NO_DOXY*/
/*
 * ####################################
 * Data Type
 * ####################################
 */
/* -------------------------------------------------------------------------- */
/* Structure and Enumeration Type Defintions */
/* -------------------------------------------------------------------------- */
/** \addtogroup PPA_HOOK_API */
/*@{*/
/*!
	\brief ppa_ct_counter holds the connection statistics.
 */
typedef struct {
	uint32_t txPackets; /*!< Tx Packets - Packets from LAN to WAN. Currently not available */
	uint64_t txBytes; /*!< Tx bytes - Bytes from LAN to WAN */
	uint32_t rxPackets; /*!< Rx Packets - Packets from WAN to LAN. Currently not available */
	uint64_t rxBytes; /*!< Rx Bytes - Bytes from WAN to LAN */
	uint32_t lastHitTime; /*!< Last hit time in secs. It is approximate
							time in seconds when a packet is seen on this
							connection. Note that seconds calculation is
							based on jiffies */
} PPA_CT_COUNTER;
/*!
	\brief This is the data structure for PPA Interface Info specification.
 */
typedef struct {
	PPA_IFNAME *ifname; /*!< Name of the stack interface */
	uint32_t if_flags; /*!< Flags for Interface. Valid values are below: PPA_F_LAN_IF and PPA_F_WAN_IF */
	uint32_t port; /*!< physical port id for this Interface. Valid values are below: 0 ~ */
	uint32_t force_wanitf_flag; /*!< force_wanitf_flag is used for force change PPE FW's LAN/WAN interface */
	PPA_IFNAME *ifname_lower; /*!< Name of the manually configured its lower interface */
	uint8_t hw_disable; /*!< If this flag is set then only HW acceleration would be disabled for ifname (SW acceleration would still work for ifname) */
} PPA_IFINFO;
/*!
	\brief This is the data structure for PPA Initialization kernel hook function
 */
typedef struct {
	uint32_t num_lanifs; /*!< Number of LAN side interfaces */
	PPA_IFINFO *p_lanifs; /*!< Pointer to array of LAN Interfaces. */
	uint32_t num_wanifs; /*!< Number of WAN side interfaces */
	PPA_IFINFO *p_wanifs; /*!< Pointer to array of WAN Interfaces. */
	uint32_t max_lan_source_entries; /*!< Maximum Number of session entries with LAN source */
	uint32_t max_wan_source_entries; /*!< Maximum Number of session entries with WAN source */
	uint32_t max_mc_entries; /*!< Maximum Number of multicast sessions */
	uint32_t max_bridging_entries; /*!< Maximum Number of bridging entries */
	uint32_t add_requires_min_hits; /*!< Minimum number of calls to ppa_add_session() before session would be added in h/w - calls from the same hook position in stack. Currently, set to 1 */
} PPA_INIT_INFO;
/*!
	\brief This is the data structure for PPA HAL Initialization kernel hook function
 */
typedef struct {
	void (*del_cb)(void *); /*!< Pointer to pass the callback function*/
} PPA_HAL_INIT_CFG;
/*!
	\brief This is the data structure which specifies an interface and its TTL value as applicable for multicast routing.
 */
typedef struct {
	PPA_IFNAME *ifname; /*!< Pointer to interface name. */
	uint8_t ttl; /*!< Time to Live (TTL) value of interface which is used for multicast routing to decide if a packet can be routed onto that interface
					 Note, it is not used at present.
					*/
} PPA_IF_TTL_ENTRY;

enum flg{
	PPA_IF_DEL,
	PPA_IF_ADD,
	PPA_IF_DROP
};

typedef struct {
	uint8_t index;
	enum flg flag;
} PPA_C_OP;

/*!
	\brief This is the data structure for PPA Multicast Group membership. It specifies the interfaces which are members of
	the specified IP Multicast Group address. Please see the discussion on outer and inner VLAN tags in the
	section on PPA_SESSION_EXTRA data structure.
 */
typedef struct {
	IP_ADDR_C ip_mc_group; 		/*!< Multicast IP address group */
	int8_t num_ifs; 		/*!< Number of Interfaces which are member of this Multicast IP group address */
	PPA_IF_TTL_ENTRY array_mem_ifs[PPA_MAX_MC_IFS_NUM]; /*!< Array of interface elements of maximum PPA_MAX_MC_IFS_NUM elements.
														Actual number of entries is specified by num_ifs */
	PPA_C_OP cop;               /*Current Operation done on the Interface  when they Join and leave i.e Add when join and Del when they leave */
	uint16_t if_mask; 		/*!< Mask of Interfaces corresponding to num_ifs interfaces specified in array_mem_ifs. For internaly use only. */
	PPA_IFNAME *src_ifname; 	/*!< the source interface of specified multicast IP address group */
	uint32_t vlan_insert:1; 	/*!< If inner VLAN tag should be inserted into the frame at egress. Valid values are: PPA_ENABLED and PPA_DISABLED */
	uint32_t vlan_remove:1; 	/*!< If inner VLAN untagging should be performed on the received frame. Untagging, if enabled, is
								carried out before any VLAN tag insert. Valid values are:PPA_ENABLED and PPA_DISABLED */
	uint32_t out_vlan_insert:1; 	/*!< If outer VLAN tag should be inserted into the frame at egress. Valid values are: PPA_ENABLED and PPA_DISABLED */
	uint32_t out_vlan_remove:1; 	/*!< If outer VLAN untagging should be performed on the received frame. Untagging, if enabled, is
									carried out before any VLAN tag insert. Valid values are:PPA_ENABLED and PPA_DISABLED */
	uint32_t dslwan_qid_remark:1; 	/*!< not use at present */
	uint32_t reserved1:3;		/*!< valid in A4/A5 */
	uint32_t vlan_prio:3;		/*!< 802.1p VLAN priority configuration. Valid values are 0-7. */
	uint32_t vlan_cfi:1; 		/*!< Always set to 1 for Ethernet frames */
	uint32_t vlan_id:12; 		/*!< VLAN Id to be used to tag the frame. Valid values are 0-4095 */
	uint32_t out_vlan_tag; 		/*!< Outer VLAN tag value including VLAN Id. */
	uint32_t new_dscp_en:1;		/*!< If new dscp value should be set. Valid values are:PPA_ENABLED and PPA_DISABLED */
	uint32_t res:15; 		/*!< reserved */
	uint32_t new_dscp:16; 		/*!< New DSCP code point value for the session.Valid values are 0-63. */
	uint16_t dslwan_qid; 		/*!< not use at present */
	uint32_t bridging_flag; 	/*!< 0 - routing mode/igmp proxy, 1 - bridge mode/igmp snooping. */
	uint8_t mac[PPA_ETH_ALEN]; 	/*!< reserved for future */
	uint8_t SSM_flag; 		/*!< Set the flag if source specific forwarding is required default 0*/
	IP_ADDR_C source_ip; 		/*!< source ip address */
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
	uint8_t RTP_flag; 		/*!< rtp flag */
#endif
	uint16_t group_id;		/*!< Multicast group identifier allocated by the multicast daemon */
	uint8_t src_mac[PPA_ETH_ALEN];	/*!< source mac address for grx5xx. */
} PPA_MC_GROUP;
/*!
	\brief This data structure is an abstraction for unicast and multicast routing sessions.
	Pointer to any kind of PPA session
 */
typedef void PPA_U_SESSION;
/*!
	\brief This is the data structure for standard packet and byte statistics for an interface.
 */
typedef struct {
	uint64_t	tx_pkts;		/*!<	 Number of transmitted packets through the interface */
	uint64_t	rx_pkts;		/*!<	 Number of received packets through the interface */
	uint64_t	tx_discard_pkts;	/*!<	 Number of packets discarded while transmitting through the interface. */
	uint64_t	tx_error_pkts;		/*!<	 Number of transmit errors through the interface. */
	uint64_t	rx_discard_pkts;	/*!<	 Number of received packets through the interface that were discarded */
	uint64_t	rx_error_pkts;		/*!<	 Number of received errors through the interface. */
	uint64_t	tx_bytes;		/*!<	 Number of transmit bytes through the interface */
	uint64_t	rx_bytes;		/*!<	 Number of received bytes through the interface */
	uint64_t	rx_pkts_prev[MAX_HAL];
	uint64_t	tx_pkts_prev[MAX_HAL];
	uint64_t	rx_bytes_prev[MAX_HAL];
	uint64_t	tx_bytes_prev[MAX_HAL];
} PPA_IF_STATS;
/*!
	\brief This is the data structure for PPA accelerated statistics for an interface. Depending on the platform and
	acceleration capabilities, some of the statistics may not be available.
 */
typedef struct {
	uint32_t fast_routed_tcp_pkts; /*!< Fastpath routed TCP unicast packets Tx */
	uint32_t fast_routed_udp_pkts; /*!< Fastpath routed UDP unicast packets Tx */
	uint32_t fast_routed_udp_mcast_pkts; /*!< Fastpath routed UDP multicast packets Tx */
	uint32_t fast_drop_pkts; /*!< Fastpath ingress pkts dropped */
	uint32_t fast_drop_bytes; /*!< Fastpath ingress bytes dropped */
	uint32_t fast_ingress_cpu_pkts; /*!< Fastpath ingress CPU pkts */
	uint32_t fast_ingress_cpu_bytes; /*!< Fastpath ingress CPU bytes */
	uint32_t rx_pkt_errors; /*!< Fastpath packet error */
	uint32_t fast_bridged_ucast_pkts; /*!< Fastpath bridged unicast pkts */
	uint32_t fast_bridged_mcast_pkts; /*!< Fastpath bridged multicast pkts */
	uint32_t fast_bridged_bcast_pkts; /*!< Fastpath bridged broadcast pkts */
	uint32_t fast_bridged_bytes; /*!< Fastpath bridged bytes */
} PPA_ACCEL_STATS;
/*!
	\brief Union of PPA VLAN filter criteria.
 */
typedef union {
	PPA_IFNAME *ifname; /*!< Pointer to interface name on which VLAN filter match is to be performed. */
	IPADDR ip_src; /*!< IP source address of ingress frame for VLAN filter matching. */
	uint32_t eth_protocol; /*!< Ethernet protocol as a match filter for VLAN filter matching */
	uint32_t ingress_vlan_tag; /*!< Ingress frame VLAN tag as match criteria for VLAN filter matching */
} match_criteria_vlan;
/*!
	\brief This data structure specifies the filter or match criteria for applying VLAN transforms based on rules. It is currently supported only for bridging paths.
 */
typedef struct {
	match_criteria_vlan match_field; /*!< Union of VLAN filter criteria */
	uint32_t match_flags; /*!< Indicates which VLAN filter criteria is specified in this VLAN match entry.
							Valid values are one of the following: \n
							PPA_F_VLAN_FILTER_IFNAME \n
							PPA_F_VLAN_FILTER_IP_SRC \n
							PPA_F_VLAN_FILTER_ETH_PROTO \n
							PPA_F_VLAN_FILTER_VLAN_TAG \n
							 */
} PPA_VLAN_MATCH_FIELD;
/*!
	\brief This is the data structure for PPA VLAN configuration ioctl() on a per interface basis from userspace. It is currently
	supported only for bridging paths.
 */
typedef struct {
	uint16_t vlan_vci; /*!< VLAN Information including VLAN Id, 802.1p and CFI bits. */
	uint16_t qid; /*!< queue index */
	uint32_t out_vlan_id; /*!< out vlan id */
	uint32_t inner_vlan_tag_ctrl;/*!< none(0)/remove(1)/insert(2)/replac(3), for vlan tag based only. */
	uint32_t out_vlan_tag_ctrl; /*!< none(0)/remove(1)/insert(2)/replac(3), for vlan tag based only. */
	uint16_t num_ifs; /*!< Number of interfaces in the array of PPA_IFINFO structures. */
	PPA_IFINFO *vlan_if_membership; /*!< Pointer to array of interface info structures for each interface which is a member of this VLAN group. The number of entries is given by num_ifs. */
} PPA_VLAN_INFO;
/*!
	\brief This is the data structure for PPA VLAN filter configuration. It is currently supported only for bridging paths
 */
typedef struct {
	PPA_VLAN_MATCH_FIELD match_field; /*!< VLAN Match field information */
	PPA_VLAN_INFO vlan_info; /*!< VLAN Group and Membership Info */
} PPA_VLAN_FILTER_CONFIG;
/*!
	\brief This is the data structure for PPA hooks list
 */
typedef struct PPA_HOOK_INFO_LIST {
	PPA_HOOK_INFO info; /*!< ppa hook info */
	struct PPA_HOOK_INFO_LIST *next; /*!< point to next ppa hook info */
} PPA_HOOK_INFO_LIST;
/*!
	\brief This is the data structure for changing to FPI address
 */
typedef struct{
	uint32_t addr_orig; /*!< original address */
	uint32_t addr_fpi; /*!< converted FPI address */
	uint32_t flags; /*!< reserved for future */
} PPA_FPI_ADDR;
#if defined(PPA_IF_MIB) && PPA_IF_MIB
/*!
	\brief This is the data structure for PPA accelerated statistics for an interface. Depending on the platform and
	acceleration capabilities, some of the statistics may not be available.
 */
typedef struct {
	PPA_PORT_MIB_INFO port_mib_stats;
	PPA_IF_STATS hw_accel_stats;
	PPA_IF_STATS sw_accel_stats;
} PPA_NETIF_ACCEL_STATS;
#endif
#if defined(L2TP_CONFIG) && L2TP_CONFIG
/*!
	\brief This is the data structure for L2TP INFO
 */
typedef struct {
	uint32_t source_ip;
	uint32_t dest_ip;
	uint16_t tunnel_id;
	uint16_t session_id;
	uint32_t tunnel_idx;
	uint32_t tunnel_type;
	uint16_t ip_chksum;
	uint16_t udp_chksum;
} PPA_L2TP_INFO;
#endif
typedef struct {
	uint32_t vlan_id; /*out vlanid or ctag_vlan_id*/
	uint32_t vlan_entry; 
	uint16_t vlan_type;
	uint8_t ctag_rem;
	uint8_t ctag_ins;
	uint8_t stag_rem;
	uint8_t stag_ins;
	uint32_t stag_vlan_id;
} PPA_OUT_VLAN_INFO;

typedef enum {
	TUNNEL_TYPE_NULL	= 0, /*!< Not Tunnel */
	TUNNEL_TYPE_6RD, /*!< 6rd Tunnel */
	TUNNEL_TYPE_DSLITE, /*!< DSLITE Tunnel */
	TUNNEL_TYPE_L2TP, /*!< L2TP Tunnel */
	TUNNEL_TYPE_EOGRE, /*!< IPv4 Ethernet Over GRE Tunnel */
	TUNNEL_TYPE_6EOGRE, /*!< IPv6 Ethernet Over GRE Tunnel */
	TUNNEL_TYPE_IPOGRE, /*!< IPv4 IP Over GRE Tunnel */
	TUNNEL_TYPE_IP6OGRE, /*!< IPv6 IP Over GRE Tunnel */
	TUNNEL_TYPE_VXLAN,  /*!< VXLAN over UDP Tunnel */
	TUNNEL_TYPE_IPSEC, /*!< IPSEC Tunnel */
	TUNNEL_TYPE_MAX /*!< Not Valid Tunnel type */
} e_Tunnel_Types;
typedef struct {
	uint32_t tunnel_idx;
	uint32_t tunnel_type;
	struct net_device *tx_dev;
} PPA_TUNNEL_INFO;

typedef struct {
	uint8_t session_id;
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	uint8_t lro_type;
	uint16_t dst_q;
#endif /*IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)*/
} PPA_LRO_INFO;

typedef struct {
	void 		*p_item; 	/* pointer to the uc_session_node */
	PPA_BUF 	*skb;		/* skb pointer */
	void		*txif_info;		/* tx physical interface name */
} PPA_SESSMETA_INFO;

typedef struct {
	void 		*p_item; 	/* pointer to the uc_session_node */
	void		*session_meta;	/* pointer to session meta for HAL layers */
	PPA_TUNNEL_INFO tnnl_info;	/* Tunnel table entry info */
	uint8_t		pppoe_mode;	/* PPPoE mode tunnel mode =1 or transport mode =0 */

	uint32_t	entry;		/* session index returned by Hardware */
	uint64_t	bytes;		/*for MIB*/
	uint64_t	packets;	/*for MIB*/
	uint8_t		f_hit;		/*only for test_and_clear_hit_stat*/
	uint8_t		collision_flag; /* 1 mean the entry is in collsion table or no hashed table, like ASE/Danube*/

	uint8_t		nFlowId;		/* FlowId */
	uint8_t		f_tc_remark_enable;	/* Traffic class remarking enabled */
	uint32_t	tc_remark;		/* Traffic class remark value*/
	uint32_t	update_flags;		/*for update_routing_entry and retun from HAL*/

} PPA_ROUTING_INFO;

typedef struct{
	uint32_t f_is_lan;
	uint32_t entry_num;
	uint32_t mc_entry_num;
	uint32_t flags;
} PPA_ROUTING_CFG;
typedef struct{
	uint32_t entry_num;
	uint32_t br_to_src_port_mask;
	uint32_t br_to_src_port_en;
	uint32_t f_dest_vlan_en;
	uint32_t f_src_vlan_en;
	uint32_t f_mac_change_drop;
	uint32_t flags;
} PPA_BRDG_CFG;
typedef struct {
	uint32_t f_is_lan;
	uint32_t f_enable;
	uint32_t flags;
} PPA_ACC_ENABLE;
typedef struct {
	uint32_t max_uc_session;
	uint32_t max_mc_session;
	uint32_t max_uc_ipv4_session;
	uint32_t max_uc_ipv6_session;
	uint32_t curr_uc_ipv4_session;
	uint32_t curr_uc_ipv6_session;
	uint32_t curr_mc_ipv4_session;
	uint32_t curr_mc_ipv6_session;
	uint32_t uc_dropped_sess;
	uint32_t mc_dropped_sess;
} PPA_HAL_STATS;
#if defined(MIB_MODE_ENABLE) && MIB_MODE_ENABLE
typedef struct {
	uint8_t session_mib_unit;
} PPA_MIB_MODE_ENABLE;
#endif
typedef struct {
	uint32_t if_no;
	uint32_t f_eg_vlan_insert;
	uint32_t f_eg_vlan_remove;
	uint32_t f_ig_vlan_aware;
	uint32_t f_ig_src_ip_based;
	uint32_t f_ig_eth_type_based;
	uint32_t f_ig_vlanid_based;
	uint32_t f_ig_port_based;
	uint32_t f_eg_out_vlan_insert;
	uint32_t f_eg_out_vlan_remove;
	uint32_t f_ig_out_vlan_aware;
} PPA_BRDG_VLAN_CFG;
typedef struct {
	uint32_t entry; /*so far it is only for get command*/
	uint32_t ig_criteria_type;
	uint32_t ig_criteria;;
	uint32_t new_vci;
	uint32_t dest_qos;
	PPA_OUT_VLAN_INFO out_vlan_info;
	uint32_t in_out_etag_ctrl;
	uint32_t vlan_port_map;
} PPA_BRDG_VLAN_FILTER_MAP;

typedef struct {

	void 		*p_item; /* struct mc_session_node pointer */

	uint8_t 	pppoe_mode;
	uint8_t 	f_hit; /*only for test_and_clear_hit_stat*/

	uint32_t 	dest_list; 
	uint32_t 	p_entry;
	uint64_t 	bytes;
	uint64_t 	packets;
	uint32_t 	update_flags; 
	PPA_C_OP 	*cop; /*Current Operation i.e Add when join and Del when they leave */
} PPA_MC_INFO;

typedef struct {
	uint32_t	port;
	uint8_t		mac[PPA_ETH_ALEN];
	uint32_t	f_src_mac_drop;
	uint32_t	dslwan_qid;
	uint32_t	dest_list;
	uint32_t	p_entry;
	uint32_t	f_hit; /*only for test_and_clear_bridging_hit_stat*/
} PPA_BR_MAC_INFO;
typedef struct {
	uint16_t 	brid;
	uint32_t 	port;
	uint16_t 	vid;
	uint16_t 	index;
} PPA_BR_PORT_INFO;
struct ppe_itf_mib {
	uint32_t	ig_fast_brg_pkts;
	uint32_t	ig_fast_brg_bytes;

	uint32_t	ig_fast_rt_ipv4_udp_pkts;
	uint32_t	ig_fast_rt_ipv4_tcp_pkts;
	uint32_t	ig_fast_rt_ipv4_mc_pkts;
	uint32_t	ig_fast_rt_ipv4_bytes;

	uint32_t	ig_fast_rt_ipv6_udp_pkts;
	uint32_t	ig_fast_rt_ipv6_tcp_pkts;
	uint32_t	res0;
	uint32_t	ig_fast_rt_ipv6_bytes;

	uint32_t	res1;
	uint32_t	ig_cpu_pkts;
	uint32_t	ig_cpu_bytes;

	uint32_t	ig_drop_pkts;
	uint32_t	ig_drop_bytes;

	uint32_t	eg_fast_pkts;
};
typedef struct {
	uint32_t 	itf ; /*port*/
	struct 		ppe_itf_mib mib;
	uint32_t 	flag;
} PPE_ITF_MIB_INFO;

/* The main device statistics structure */
struct intf_mib {
	 uint64_t	rx_packets;			/* total packets received */
	 uint64_t	tx_packets;			/* total packets transmitted	*/
	 uint64_t	rx_bytes;			/* total bytes received		 */
	 uint64_t	tx_bytes;			/* total bytes transmitted*/
	 uint64_t	rx_errors;			/* bad packets received		 */
	 uint64_t	tx_errors;			/* packet transmit problems	 */
	 uint64_t	rx_dropped;			/* no space in linux buffers	*/
	 uint64_t	tx_dropped;			/* no space available in linux	*/
	 uint64_t	multicast;			/* multicast packets received	 */
	 uint64_t	collisions;

	/* detailed rx_errors: */
	 uint64_t	rx_length_errors;
	 uint64_t	rx_over_errors;		 	/* receiver ring buff overflow	*/
	 uint64_t	rx_crc_errors;			/* recved pkt with crc error	*/
	 uint64_t	rx_frame_errors;		/* recv'd frame alignment error */
	 uint64_t	rx_fifo_errors;		 	/* recv'r fifo overrun	*/
	 uint64_t	rx_missed_errors;		/* receiver missed packet */

	/* detailed tx_errors */
	uint64_t	tx_aborted_errors;
	uint64_t	tx_carrier_errors;
	uint64_t	tx_fifo_errors;
	uint64_t	tx_heartbeat_errors;
	uint64_t	tx_window_errors;

	/* for cslip etc */
	uint64_t	rx_compressed;
	uint64_t	tx_compressed;
};

typedef struct {
	struct 	intf_mib mib;
	struct 	netif_info *ifinfo;
} PPA_ITF_MIB_INFO;

typedef struct {
	int32_t num ;
	uint32_t flags ;
} PPA_COUNT_CFG;
typedef struct {
	uint32_t vfitler_type;
	int32_t num ;
	uint32_t flags ;
} PPA_VFILTER_COUNT_CFG;

typedef struct {
	PPA_IFNAME ifname[PPA_IF_NAME_SIZE]; /*!< Name of the stack interface */
	uint32_t if_flags; /*!< Flags for Interface. Valid values are below: PPA_F_LAN_IF and PPA_F_WAN_IF */
	uint32_t port; /*!< physical port id for this Interface. Valid values are below: 0 ~ */
} PPE_IFINFO;
typedef struct {
	uint8_t lan_flag; /* 1 means lan port, 0 means wan ports*/
	uint32_t physical_port;
	uint32_t old_lan_flag; /* 1 means lan port, 0 means wan ports*/
} PPA_WANITF_CFG;
typedef struct {
	uint8_t f_is_lan; /* input:1 means lan port, 0 means wan ports*/
	uint32_t src_ip; /* input:*/
	uint32_t src_port; /*input:*/
	uint32_t dst_ip; /* input:*/
	uint32_t dst_port; /* input:*/
	uint32_t hash_index; /*output*/
	uint32_t hash_table_id; /*output mainly reserved for future GRX500 since LAN/WAN will share same hash table*/
	uint32_t flag; /*reserved*/
} PPA_SESSION_HASH;
typedef struct {
	uint8_t 	hop_limit;		/*!< PPA ip6hdr hop limit*/
	uint32_t	saddr[4];	/*!< PPA ip6hdr src address*/
	uint32_t 	daddr[4];	/*!< PPA ip6hdr dst address*/
} PPA_IP6HDR;
typedef struct {
	uint32_t saddr;		/*!< PPA ip4hdr src address*/
	uint32_t daddr;		/*!< PPA ip4hdr src address*/
} PPA_IP4HDR;
typedef union {
	PPA_IP4HDR		ip4_hdr;	/*!< PPA ip4 hdr*/
	PPA_IP6HDR		ip6_hdr;	/*!< PPA ip6 hdr*/
#if defined(L2TP_CONFIG) && L2TP_CONFIG
	PPA_L2TP_INFO	l2tp_hdr;	/*!< PPA l2tp hdr*/
#endif
} ppa_tnl_info;

typedef struct {
	int32_t		tunnel_type;	/*!< PPA tunnel type*/
	ppa_tnl_info	tunnel_info;	/*!< PPA tunnel info*/
	void	 *hal_buffer;	/*!< PPA hal buffer*/
} ppa_tunnel_entry;

/*
 * ####################################
 * Declaration
 * ####################################
 */
#ifdef __KERNEL__
#ifdef NO_DOXY
#ifdef CONFIG_LTQ_PORT_MIRROR
extern struct net_device *(*get_mirror_netdev)(void);
extern uint32_t (*is_device_type_wireless)(void);
#define SKB_MIRROR_FLAG 0x2000
#endif
#define ENUM_STRING(x) #x
void ppa_session_delete_by_sgc(int32_t sgc_grp_idx, int32_t sgc_id);
void ppa_delete_all_hw_session(void);
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
int32_t ppa_mc_entry_rtp_set(PPA_MC_GROUP *);
#endif
#if defined(PPA_IF_MIB) && PPA_IF_MIB
int32_t ppa_get_netif_accel_stats(PPA_IFNAME *, PPA_NETIF_ACCEL_STATS *, uint32_t, struct rtnl_link_stats64*);
#endif
int32_t ppa_add_if(PPA_IFINFO *, uint32_t);
int32_t ppa_del_if(PPA_IFINFO *, uint32_t);
int32_t ppa_update_min_hit(uint32_t val);
extern uint32_t g_ppa_ppa_mtu;
extern uint32_t g_ppa_min_hits;
#endif /*NO_DOXY*/
#endif /*end of __KERNEL__*/
#ifdef __KERNEL__
/* this header file uses many of the above structure definitions*/
#include <net/ppa/ppa_hook.h>
#endif
#endif /* __PPA_API_H__ */
