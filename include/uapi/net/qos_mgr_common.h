/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* Copyright (C) 2020~2023 MaxLinear Inc.
*/
#ifndef _UAPI_QOS_MGR_API_H
#define _UAPI_QOS_MGR_API_H
#ifndef __KERNEL__
#include <stdint.h> 
#endif
#include <net/switch_api/gsw_types.h>
#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/lantiq_gsw_flow.h>
/*
 * ####################################
 * Definition
 * ####################################
 */
/*!
	\brief QOS_MGR_MAX_MC_IFS_NUM
 */
#define QOS_MGR_MAX_MC_IFS_NUM 16 /*!< Maximum interface number supported */
/*!
	\brief QOS_MGR_MAX_IFS_NUM
 */
#define QOS_MGR_MAX_IFS_NUM 16 /*!< Maximum interface number supported */
#define MAX_SUBIF	16 /*!< Maximum sub interface supported */
/*!
	\brief QOS_MGR_IOC_MAGIC
 */
#define QOS_MGR_IOC_MAGIC ((uint32_t)'q') /*!< Magic number to differentiate PPA ioctl commands */
/*!
	\brief QOS_MGR_SUCCESS
 */
#define QOS_MGR_SUCCESS	0 /*!< Operation was successful. */
/*!
	\brief QOS_MGR_FAILURE
 */
#define QOS_MGR_FAILURE 	-1 /*!< Operation failed */
/*!
	\brief QOS_MGR_EPERM
 */
#define QOS_MGR_EPERM 	-2 /*!< not permitted */
/*!
	\brief QOS_MGR_EIO
 */
#define QOS_MGR_EIO 	-5 /*!< I/O/Hardware/Firmware error */
/*!
	\brief QOS_MGR_EAGAIN
 */
#define QOS_MGR_EAGAIN 	-11 /*!< try again later */
/*!
	\brief QOS_MGR_ENOMEM
 */
#define QOS_MGR_ENOMEM	-12 /*!< out of memory */
/*!
  \brief QOS_MGR_NETIF_FAIL
 */
#define QOS_MGR_NETIF_FAIL	0 /*!< netif passed is NULL */
/*!
	\brief QOS_MGR_EACCESS
 */
#define QOS_MGR_EACCESS QOS_MGR_EPERM
/*!
	\brief QOS_MGR_EFAULT
 */
#define QOS_MGR_EFAULT 	-14 /*!< bad address */
/*!
	\brief QOS_MGR_EBUSY
 */
#define QOS_MGR_EBUSY 	-16 /*!< busy */
/*!
	\brief QOS_MGR_EINVAL
 */
#define QOS_MGR_EINVAL 	-22 /*!< invalid argument */
/*!
	\brief QOS_MGR_ENOTAVAIL
 */
#define QOS_MGR_ENOTAVAIL 	-97
/*!
	\brief QOS_MGR_ENOTPOSSIBLE
 */
#define QOS_MGR_ENOTPOSSIBLE	-98
/*!
	\brief QOS_MGR_ENOTIMPL
 */
#define QOS_MGR_ENOTIMPL 	-99 /*!< not implemented */
/*!
	\brief QOS_MGR_INDEX_OVERFLOW
 */
#define QOS_MGR_INDEX_OVERFLOW	-100
/*!
	\brief QOS_MGR_ENABLED
 */
#define QOS_MGR_ENABLED	1 /*!< Status enabled / Device was enabled */
/*!
	\brief QOS_MGR_DISABLED
 */
#define QOS_MGR_DISABLED	0 /*!< Status disabled / Device was disabled. */
/*!
	\brief QOS_MGR_MAX_PORT_NUM
 */
#define QOS_MGR_MAX_PORT_NUM 16 /*!< Maximum PPE FW Port number supported in PPA layer. */
/*!
	\brief QOS_MGR_MAX_QOS_QUEUE_NUM
 */
#define QOS_MGR_MAX_QOS_QUEUE_NUM 16 /*!< Maximum PPE FW QoS Queue number supported in PPA layer for one port. */
/*
 * flags
 */
/*!
	\brief QOS_MGR_F_BEFORE_NAT_TRANSFORM
	\note PPA Routing Session add hook is called before NAT transform has taken place. \n
	In Linux OS, this NSFORM corresponds to the netfilter PREROUTING hook
 */
#define QOS_MGR_F_BEFORE_NAT_TRANSFORM 0x00000001
/*!
	\brief QOS_MGR_F_ACCEL_MODE
	\note notify PPA to enable or disable acceleration for one routing session. It is only for Hook/ioctl, not for PPE FW usage
 */
#define QOS_MGR_F_ACCEL_MODE 0x00000002
/*!
	\brief QOS_MGR_F_SESSION_ORG_DIR
	\note Packet in original direction of session i.e. the direction in which the session was established
 */
#define QOS_MGR_F_SESSION_ORG_DIR 0x00000010
/*!
	\brief QOS_MGR_F_SESSION_REPLY_DIR
	\note Packet in reply direction of session i.e. opposite to the direction in which session was initiated.
 */
#define QOS_MGR_F_SESSION_REPLY_DIR 0x00000020
/*!
	\brief QOS_MGR_F_SESSION_BIDIRECTIONAL
	\note For PPA Session add, add a bidirectional session, else unidirection session is assumed.
 */
#define QOS_MGR_F_SESSION_BIDIRECTIONAL (QOS_MGR_F_SESSION_ORG_DIR | QOS_MGR_F_SESSION_REPLY_DIR)
/*!
	\brief QOS_MGR_F_BRIDGED_SESSION
	\note Denotes that the PPA session is bridged
 */
#define QOS_MGR_F_BRIDGED_SESSION 0x00000100
/*!
	\brief QOS_MGR_F_SESSION_LOCAL_IN
	\note Denotes that the PPA session is local in
 */
#define QOS_MGR_F_SESSION_LOCAL_IN 0x00000200
/*!
	\brief QOS_MGR_F_SESSION_LOCAL_OUT
	\note Denotes that the PPA session is local out
 */
#define QOS_MGR_F_SESSION_LOCAL_OUT 0x00000400
/*!
	\brief QOS_MGR_F_SESSION_NEW_DSCP
	\note Denotes that the PPA session has DSCP remarking enabled
 */
#define QOS_MGR_F_SESSION_NEW_DSCP 0x00001000
/*!
	\brief QOS_MGR_F_SESSION_VLAN
	\note Denotes that the PPA session has VLAN tagging enabled.
 */
#define QOS_MGR_F_SESSION_VLAN 0x00002000
/*!
	\brief QOS_MGR_F_MTU
	\note Denotes that the PPA session has a MTU limit specified
 */
#define QOS_MGR_F_MTU 0x00004000
/*!
	\brief QOS_MGR_F_SESSION_OUT_VLAN
	\note Denotes that the PPA session has Outer VLAN tagging enable
 */
#define QOS_MGR_F_SESSION_OUT_VLAN 0x00008000
/*!
	\brief QOS_MGR_F_BRIDGE_LOCAL
	\note Denotes that the PPA bridge session is for a flow terminated at the CPE (i.e. not bridged out). Such an entry will not be accelerated
 */
#define QOS_MGR_F_BRIDGE_LOCAL 0x00010000
/*!
	\brief QOS_MGR_F_PPPOE
	\note Denotes that the PPA session has PPPOE header
 */
#define QOS_MGR_F_PPPOE 0x00020000
/*!
	\brief QOS_MGR_F_LAN_IF
	\note Indicates that the interface is a LAN interface
 */
#define QOS_MGR_F_LAN_IF 0x01000000
/*!
	\brief QOS_MGR_F_LAN_IPSEC
	\note Indicates that the interface is a LAN interface
 */
#define QOS_MGR_F_LAN_IPSEC 0x08000000
/*!
	\brief QOS_MGR_F_STATIC_ENTRY
	\note Indicates that it is a static entry
 */
#define QOS_MGR_F_STATIC_ENTRY 0x20000000
/*!
	\brief QOS_MGR_F_DROP_PACKET
	\note Denotes that the PPA session has a drop action specified. In other words, this acts as a fast path \n
	packet filter drop action
 */
#define QOS_MGR_F_DROP_PACKET 0x40000000
/*!
	\brief QOS_MGR_F_BRIDGE_ACCEL_MODE
	\note Flag denoting that the PPA should accelerate bridging sessions. Reserved currently
 */
#define QOS_MGR_F_BRIDGE_ACCEL_MODE 0x80000000
/*
 * interface flags
 */
/*!
	\brief QOS_MGR_SESSION_NOT_ADDED
 */
#define QOS_MGR_SESSION_NOT_ADDED -1 /*!< PPA Session Add failed. This can happen either because the Session is not yet ready for addition or \n
									 that PPA cannot accelerate the session because the packet is looped back */
/*!
	\brief QOS_MGR_SESSION_NOT_DELETED
 */
#define QOS_MGR_SESSION_NOT_DELETED -1 /*!< PPA Session Add failed. This can happen either because the Session is not yet ready for addition or \n
									 that PPA cannot accelerate the session because the packet is looped back */
/*!
	\brief QOS_MGR_SESSION_ADDED
 */
#define QOS_MGR_SESSION_ADDED 0 /*!< Indicates PPA was able to successfully add the session */
/*!
	\brief QOS_MGR_SESSION_DELETED
 */
#define QOS_MGR_SESSION_DELETED 0 /*!< Indicates PPA was able to successfully add the session */
/*!
	\brief QOS_MGR_SESSION_EXISTS
 */
#define QOS_MGR_SESSION_EXISTS 1 /*!< Indicates PPA already has the session added. This is also a success indication */
/*!
	\brief QOS_MGR_MC_SESSION_VIOLATION
 */
#define QOS_MGR_MC_SESSION_VIOLATION -2 /*!< PPA MC Session lookup failed. This can happen when you search IGMP V2 entry but hit one IGMPv3 entry with same \n
										Multicast Group IP or vise versa*/
/*!
	\brief QOS_MGR_SESSION_FILTED
 */
#define QOS_MGR_SESSION_FILTED -1 /*!< Indicates session is not qualified to be accelerated yet */
/*!
	\brief QOS_MGR_SESSION_NOT_FILTED
 */
#define QOS_MGR_SESSION_NOT_FILTED 0 /*!< Indicates session can be further processed */
/*
 * QOS_MGR_inactivity_status return value
 */
/*!
	\brief QOS_MGR_HIT
	\note PPA Session is active i.e. was hit with packets within the configured inactivity time inter
 */
#define QOS_MGR_HIT 0
/*!
	\brief QOS_MGR_TIMEOUT
	\note PPA Session is inactive and hence has timed out
 */
#define QOS_MGR_TIMEOUT 1
/*!
	\brief QOS_MGR_F_VLAN_FILTER_IFNAME
 */
#define QOS_MGR_F_VLAN_FILTER_IFNAME 0 /*!< Port based VLAN */
/*!
	\brief QOS_MGR_F_VLAN_FILTER_IP_SRC
 */
#define QOS_MGR_F_VLAN_FILTER_IP_SRC 1 /*!< SRC IP based VLAN */
/*!
	\brief QOS_MGR_F_VLAN_FILTER_ETH_PROTO
 */
#define QOS_MGR_F_VLAN_FILTER_ETH_PROTO 2 /*!< Ethernet Type based VLAN */
/*!
	\brief QOS_MGR_F_VLAN_FILTER_VLAN_TAG
 */
#define QOS_MGR_F_VLAN_FILTER_VLAN_TAG 3 /*!< Vlan tag based VLAN */
/*!
	\brief QOS_MGR_F_NETIF_PORT_MIB
 */
#define QOS_MGR_F_NETIF_PORT_MIB 0x00000001 /*!< PPE Port MIB stats */
/*!
	\brief QOS_MGR_F_NETIF_HW_ACCEL
 */
#define QOS_MGR_F_NETIF_HW_ACCEL 0x00000002 /*!< HW accelerated session stats */
/*!
	\brief QOS_MGR_F_NETIF_SW_ACCEL
 */
#define QOS_MGR_F_NETIF_SW_ACCEL 0x00000004 /*!< SW accelerated session stats */
/*!
	\brief QOS_MGR_INVALID_QID
 */
#define QOS_MGR_INVALID_QID 0xFFFF /*!< Invalid VLAN ID. Note, it is used only in IOCTL */
/*!
	\brief QOS_MGR_VLAN_TAG_MASK
 */
#define QOS_MGR_VLAN_TAG_MASK 0xFFFF1FFF /*!< VLAN MASK to remove VLAN priority*/
/*!
	\brief MAX_HOOK_NAME_LEN
 */
#define MAX_HOOK_NAME_LEN 71 /*!< maximum hook name length */
/*!
	\brief QOS_MGR_PORT_MODE_ETH
 */
#define QOS_MGR_PORT_MODE_ETH 1 /*!< Ethernet Port */
/*!
	\brief QOS_MGR_PORT_MODE_DSL
 */
#define QOS_MGR_PORT_MODE_DSL 2 /*!< DSL Port */
/*!
	\brief QOS_MGR_PORT_MODE_EXT
 */
#define QOS_MGR_PORT_MODE_EXT 3 /*!< Extension Port, like USB/WLAN */
/*!
	\brief QOS_MGR_PORT_MODE_CPU
 */
#define QOS_MGR_PORT_MODE_CPU 4 /*!< CPU */
/*!
	\brief MAX_Q_NAME_LEN */
#define MAX_Q_NAME_LEN 128
/*!
	\brief MAX_TC_NUM */
#define MAX_TC_NUM 16
/*!
	\brief MAX_PRIO_NUM */
#define MAX_PRIO_NUM 8

#define TC_START_BIT_POS		0
#define TC_MASK				 0x1f
/*!
	\brief QOS_MGR_OP_F_ADD */
#define QOS_MGR_OP_F_ADD			0x00000001
/*!
	\brief QOS_MGR_OP_F_ADD */
#define QOS_MGR_OP_F_MODIFY			0x00000002
/*!
	\brief QOS_MGR_OP_F_DELETE */
#define QOS_MGR_OP_F_DELETE			0x00000004
/*!
	\brief QOS_MGR_OP_F_INGRESS */
#define QOS_MGR_OP_F_INGRESS			0x00000080
/*!
	\brief QOS_MGR_Q_F_INGRESS */
#define QOS_MGR_Q_F_INGRESS			0x00000010
/*!
	\brief QOS_MGR_Q_F_DEFAULT */
#define QOS_MGR_Q_F_DEFAULT			0x00000020
/*!
	\brief QOS_MGR_Q_F_WLANDP */
#define QOS_MGR_Q_F_WLANDP			0x00000040
/*!
	\brief QOS_MGR_Q_F_INGGRP1 */
#define QOS_MGR_Q_F_INGGRP1			0x00000100
/*!
	\brief QOS_MGR_Q_F_INGGRP2 */
#define QOS_MGR_Q_F_INGGRP2			0x00000200
/*!
	\brief QOS_MGR_Q_F_INGGRP3 */
#define QOS_MGR_Q_F_INGGRP3			0x00000400
/*!
	\brief QOS_MGR_Q_F_INGGRP4 */
#define QOS_MGR_Q_F_INGGRP4			0x00000800
/*!
	\brief QOS_MGR_Q_F_INGGRP5 */
#define QOS_MGR_Q_F_INGGRP5			0x00001000
/*!
	\brief QOS_MGR_Q_F_INGGRP6 */
#define QOS_MGR_Q_F_INGGRP6			0x00002000
/*!
	\brief QOS_MGR_Q_F_INGGRP7 */
#define QOS_MGR_Q_F_INGGRP7			0x00004000
/*!
  \brief QOS_MGR_Q_F_WFQ_UPDATE */
#define QOS_MGR_Q_F_WFQ_UPDATE                  0x00008000
/*!
  \brief QOS_MGR_CL_CPU */
#define QOS_MGR_CL_CPU				0x00010000
/*!
  \brief QOS_MGR_Q_F_SP_WFQ - QoS configured in SP+WFQ mode */
#define QOS_MGR_Q_F_SP_WFQ			0x00020000
/*!
  \brief QOS_MGR_Q_F_SPCL_CONN - QoS configuration for SPCL Connections */
#define QOS_MGR_Q_F_SPCL_CONN                  	0x00040000
/*!
	\brief QOS_MGR_F_PPPOATM */
#define QOS_MGR_F_PPPOATM				0x80000000

/*
 * Queue Threshold Color Drop Flags
 */
/** QOS_MGR_DT_F_GREEN_THRESHOLD:Set Green color threshold drop tail flag */
#define QOS_MGR_DT_F_GREEN_THRESHOLD 0x00000001
/** QOS_MGR_DT_F_YELLOW_THRESHOLD:Set Yellow color threshold drop tail flag*/
#define QOS_MGR_DT_F_YELLOW_THRESHOLD 0x00000002
/** QOS_MGR_DT_F_RED_THRESHOLD:Set Red color threshold drop tail flag*/
#define QOS_MGR_DT_F_RED_THRESHOLD 0x00000004

/** MAX_IF_PER_CPU_INGRESS_GROUP:Maximum no of CPU Ingress Groups */
#define NUM_CPU_INGRESS_GROUPS 			7
/** Maximum Interface Names per CPU Ingress Groups */
#define MAX_IF_PER_CPU_INGRESS_GROUP	20
/** Maximum length of ingress group name*/
#define MAX_INGGROUP_NAME_LENGTH	16
/** Maximum number of fields per rule*/
#define MAX_FIELDS_PER_INGRESS_RULE	8

/*! \def ETH_ALEN
	\brief Macro that specifies the maximum length of an Ethernet MAC address.
 */
#define ETH_ALEN 6
/*! \def IFNAMSIZ
	\brief Macro that specifies the maximum size of an interface NAME
 */
#define IFNAMSIZ 16
/*! \def QOS_MGR_ETH_ALEN
	\brief Macro that specifies the maximum length of an Ethernet MAC address.
 */
#define QOS_MGR_ETH_ALEN ETH_ALEN
/*! \def QOS_MGR_ETH_HLEN
	\brief Macro that specifies the maximum length of an Ethernet MAC header.
 */
#define QOS_MGR_ETH_HLEN ETH_HLEN
/*! \def QOS_MGR_ETH_CRCLEN
	\brief Macro that specifies the maximum length of an Ethernet CRC.
 */
#define QOS_MGR_ETH_CRCLEN 4
/*! \def QOS_MGR_IF_NAME_SIZE
	\brief Macro that specifies the maximum size of one interface name
 */
#define QOS_MGR_IF_NAME_SIZE IFNAMSIZ
/*! \def QOS_MGR_IF_SUB_NAME_MAX_NUM
	\brief Macro that specifies the maximum size of one interface name
 */
#define QOS_MGR_IF_SUB_NAME_MAX_NUM 5
/*! \def QOS_MGR_IPPROTO_TCP
	\brief Macro that specifies TCP flag
 */
#define QOS_MGR_IPPROTO_TCP 6
/*! \def QOS_MGR_IPPROTO_UDP
	\brief Macro that specifies UDP flag
 */
#define QOS_MGR_IPPROTO_UDP 17
/*! \def QOS_MGR_USER
	\brief Macro that specifies the flag for the buffer type from User Space via ioctl
 */
#define QOS_MGR_USER __user
/*! \def NIP6_FMT
	\brief Macro that specifies NIP6_FMT format definition for printing IPV6 address
 */
#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
/*! \def nipquad_fmt
	\brief macro that specifies nipquad_fmt format definition for printing ipv4 address
 */
#define NIPQUAD_FMT "%u.%u.%u.%u"
#define QOS_MGR_VERSION_LEN		64
/*
 * ####################################
 * Data Type
 * ####################################
 */
#if 0 
/*!
	\brief This is the unsigned char 32-bit data type.
 */
typedef unsigned int uint32_t;
/*!
	\brief This is the unsigned char 16-bit data type.
 */
typedef unsigned short uint16_t;
/*!
	\brief This is the unsigned char 8-bit data type.
 */
typedef unsigned char uint8_t;
/*!
	\brief This is the unsigned char 64-bit data type.
 */
typedef unsigned long long uint64_t;
#endif
/* -------------------------------------------------------------------------- */
/* Structure and Enumeration Type Defintions */
/* -------------------------------------------------------------------------- */
/*!
	\brief char to interface name
 */
typedef char QOS_MGR_IFNAME;

/*!
	\brief This is the data structure holding the IP address. It helps to provide future compatibility for IPv6 support.
	Currently it only supports IPv4.
 */
typedef uint32_t IPADDR;
/*@}*/ /* QOS_MGR_HOOK_API */
/*
 * ioctl command structures
 */
/** \addtogroup QOS_MGR_IOCTL */
/*@{*/
/*!
	\brief This is the data structure for basic ppa Versions
 */
typedef struct {
	uint32_t index; /*!< index for PP32 */
	uint32_t family; /*!< ppa version hardware family */
	uint32_t type; /*!< ppa version hardware type */
	uint32_t itf;/*!< ppa version itf */
	uint32_t mode; /*!< ppa version mode */
	uint32_t major; /*!< ppa version major version number */
	uint32_t mid; /*!< ppa version mid version number */
	uint32_t minor; /*!< ppa version minor version number */
	uint32_t tag; /*!< ppa version tag number. Normally for internal usage */
	uint32_t	id;
	char	name[QOS_MGR_VERSION_LEN];
	char	version[QOS_MGR_VERSION_LEN];
} QOS_MGR_VERSION;
/*!
	\brief QoS Shaper Mode Configuration
 */
typedef enum {
	QOS_MGR_SHAPER_NONE = 0,
	QOS_MGR_SHAPER_COLOR_BLIND = 1,
	QOS_MGR_SHAPER_TR_TCM = 2,
	QOS_MGR_SHAPER_TR_TCM_RFC4115 = 3,
	QOS_MGR_SHAPER_LOOSE_COLOR_BLIND = 4
} QOS_MGR_SHAPER_MODE;
/*!
	\brief QoS Shaper configuration structure
 */
typedef struct {
	QOS_MGR_SHAPER_MODE mode; /*!< Mode of Token Bucket shaper */
	uint32_t		enable; /*!< Enable for Shaper */
	uint32_t cir; /*!< Committed Information Rate in bytes/s */
	uint32_t cbs; /*!< Committed Burst Size in bytes */
	uint32_t pir; /*!< Peak Information Rate in bytes/s */
	uint32_t pbs; /*!< Peak Burst Size */
	uint32_t flags; /*!< Flags define which shapers are enabled
						- QOS_SHAPER_F_PIR
						- QOS_SHAPER_F_CIR */
	int32_t			phys_shaperid;
} QOS_MGR_SHAPER_CFG;
/*!
	\brief QoS Meter Mode Configuration
 */
typedef enum {
	QOS_MGR_METER_TOKEN_BUCKET = 1,/*!< Simple Token Bucket */
	QOS_MGR_METER_SR_TCM = 2, /*!< Single Rate Three Color Marker */
	QOS_MGR_METER_TR_TCM = 3 /*!< Two Rate Three Color Marker */
}QOS_MGR_METER_TYPE;
/*!
  \brief QoS Meter configuration structure
 */
typedef struct {
	QOS_MGR_METER_TYPE	type; /*!< Mode of Meter*/
	uint32_t		enable; /*!< Enable for meter */
	uint32_t		cir; /*!< Committed Information Rate in bytes/s */
	uint32_t		cbs; /*!< Committed Burst Size in bytes */
	uint32_t		pir; /*!< Peak Information Rate in bytes/s */
	uint32_t		pbs; /*!< Peak Burst Size */
	uint32_t		meterid; /*!< Meter ID Configured on the system*/
	uint32_t		flags; /*!< Flags define operations on meters enbled*/
}QOS_MGR_METER_INFO;
/*!
	\brief This is the data structure for PPA QOS Internal INFO
 */
typedef struct {
	uint32_t t; /*!< Time Tick */
	uint32_t w; /*!< weight */
	uint32_t s; /*!< burst */
	uint32_t r; /*!< Replenish */
	uint32_t d; /*!< ppe internal variable */
	uint32_t tick_cnt; /*!< ppe internal variable */
	uint32_t b; /*!< ppe internal variable */
	/*For PPA Level only */
	uint32_t reg_addr; /*!< register address */
	uint32_t bit_rate_kbps; /*!< rate shaping in kbps */
	uint32_t weight_level; /*!< internal wfq weight */
} QOS_MGR_INTERVAL;
/*!
	\brief This is the data structure for PPA QOS Internal Debug INFO
 */
typedef struct {
	/*struct wtx_qos_q_desc_cfg*/
	uint32_t threshold; /*!< qos wtx threshold */
	uint32_t length; /*!< qos wtx length */
	uint32_t addr; /*!< qos wtx address */
	uint32_t rd_ptr; /*!< qos wtx read pointer */
	uint32_t wr_ptr; /*!< qos wtx write pointer */
	/*For PPA Level only */
	uint32_t reg_addr; /*!< register address */
} QOS_MGR_DESC_CFG_INTERNAL;
/*!
	\brief This is the data structure for PPA QOS to get the maximum queue number supported for one physical port
 */
typedef struct {
	uint32_t portid; /*!< the phisical port id which support qos queue */
	uint32_t queue_num; /*!< the maximum queue number is supported */
	uint32_t flags; /*!< Reserved currently */
} QOS_MGR_CMD_QUEUE_NUM_INFO;
/*!
	\brief This is the data structure for PPA QOS MIB Counter
 */
typedef struct {
	uint64_t total_rx_pkt; /*!< all packets received by this queue */
	uint64_t total_rx_bytes; /*!< all bytes received by thi queue */
	uint64_t total_tx_pkt; /*!< all packets trasmitted by this queue */
	uint64_t total_tx_bytes; /*!< all bytes trasmitted by thi queue */
	uint64_t cpu_path_small_pkt_drop_cnt; /*!< all small packets dropped in CPU path for lack of TX DMA descriptor in the queue*/
	uint64_t cpu_path_total_pkt_drop_cnt; /*!< all packets dropped in CPU path for lack of TX DMA descriptor in the queue*/
	uint64_t fast_path_small_pkt_drop_cnt; /*!< all small packets dropped in fast path for lack of TX DMA descriptor */
	uint64_t fast_path_total_pkt_drop_cnt; /*!< all packets dropped in fast path for lack of TX DMA descriptor */
	uint64_t tx_diff; /*!< tx bytes since last read */
	uint64_t tx_diff_L1; /*!< tx bytes plus L1 overheader since last read */
	uint64_t tx_diff_jiffy; /*!< jiffy diff since last read */
	uint32_t sys_hz; /*!< system HZ value. For debugging purpose */
	uint32_t q_occ; /*!< instantaneous number of packets present in this queue */
} QOS_MGR_MIB;
typedef struct {
	uint32_t portid;
	uint32_t num ;
	uint32_t flags ;
} QOS_MGR_COUNT_CFG;
typedef struct {
	char *ifname;/*!< Interface name on which Queue is to be added*/
	uint32_t portid;
	uint32_t queueid;
	QOS_MGR_MIB mib;
	uint32_t reg_addr;
	uint32_t flag;
} QOS_MGR_MIB_INFO;
typedef struct {
	uint32_t portid;
	uint32_t queueid;
	uint32_t weight_level;
	uint32_t flag;
} QOS_MGR_WFQ_CFG;
typedef struct {
	uint32_t portid;
	uint32_t f_enable;
	uint32_t flag;
} QOS_MGR_ENABLE_CFG;
typedef struct {
	uint32_t portid;
	uint32_t queueid;
	int32_t shaperid;
	uint32_t rate_in_kbps;
	uint32_t burst;
	uint32_t flag;
} QOS_MGR_RATE_SHAPING_CFG;
/*!
	\brief This is the data structure for PPA QOS to get the maximum queue number supported for one physical port
 */
typedef struct {
	QOS_MGR_IFNAME			ifname[QOS_MGR_IF_NAME_SIZE];/*!< Interface name on which the Queue is modified*/
	uint32_t portid; /*!< the phisical port id which support qos queue */
	int32_t queue_num; /*!< Logical queue number added will be filled up in this field */
	QOS_MGR_MIB mib; /*!< the mib information for the current specified queue */
	uint32_t flags; /*!< Reserved currently */
} QOS_MGR_CMD_MIB_INFO;
/*!
	\brief This is the data structure for PPA QOS to be enabled/disabled
 */
typedef struct {
	uint32_t portid; /*!< which support qos queue. */
	uint32_t enable; /*!< enable/disable flag */
	uint32_t flags; /*!< Reserved currently */
} QOS_MGR_CMD_CTRL_INFO;
/*!
	\brief This is the data structure for PPA Rate Shapping Set/Get/Reset one queue's rate limit
 */
typedef struct {
	QOS_MGR_IFNAME		ifname[QOS_MGR_IF_NAME_SIZE];
	uint32_t		portid; /*!< the phisical port id which support qos queue */
	int32_t			queueid; /*!< the queu id. Now it only support 0 ~ 7 */
	int32_t			shaperid; /*!< the shaper id. Now it only support 0 ~ 7 */
	QOS_MGR_SHAPER_CFG	shaper;
	uint32_t		rate; /*!< rate limit in kbps */
	uint32_t		burst; /*!< rate limit in bytes. Note: it is PPE FW QOS internal value. Normally there is no need to set this value or just set to default value zero */
	uint32_t		flags; /*!< Reserved currently */
} QOS_MGR_CMD_RATE_INFO;
/*!
	\brief This is the data structure for PPA WFQ Set/Get/Reset one queue's weight
 */
typedef struct {
	uint32_t portid; /*!< the phisical port id which support qos queue */
	uint32_t queueid; /*!< the queu id. Now it only support 0 ~ 7 */
	uint32_t weight; /*!< WFQ weight. The value is from 0 ~ 100 */
	uint32_t flags; /*!< Reserved currently */
} QOS_MGR_CMD_WFQ_INFO;
/*!
	\brief This is the data structure for getting all exported PPA hooks.
 */
typedef struct {
	uint8_t hookname[MAX_HOOK_NAME_LEN]; /*!< hook name */
	void *hook_addr; /*!< mid hook address */
	uint8_t hook_flag:1; /*!< hook_flag: 0-disabled, otherwise -enabled */
	uint8_t used_flag:1; /*!< used_flag: 0-not used, otherwise -used */
	uint8_t reserved:6; /*!< reserved for future */
} QOS_MGR_HOOK_INFO;
/*!
	\brief This is the data structure for getting all exported PPA hooks.
 */
typedef struct {
	uint32_t hook_count; /*!< hook counter */
	uint32_t flag; /*!< reserved for future */
	QOS_MGR_HOOK_INFO list[1]; /*!< it is a dummy array. Userspace should apply storage buffer for it. */
} QOS_MGR_CMD_HOOK_LIST_INFO;
/*!
	\brief This is the data structure for enable/disable ppa hook
 */
typedef struct {
	uint8_t hookname[MAX_HOOK_NAME_LEN]; /*!< hook name */
	uint32_t enable; /*!< enable/disable ppa hook */
	uint32_t flag; /*!< reserved for future */
} QOS_MGR_HOOK_ENABLE_INFO;
/*!
	\brief This is the data structure for IOCTL to enable/disable ppa hook
 */
typedef QOS_MGR_HOOK_ENABLE_INFO QOS_MGR_CMD_HOOK_ENABLE_INFO;
/*!
	\brief This is the data structure to get the physical port id For IOCTL.
 */
typedef struct {
	QOS_MGR_IFNAME ifname[QOS_MGR_IF_NAME_SIZE]; /*!< the interface name */
	uint32_t portid; /*!< the port id */
	uint32_t flags; /*!< reserved for future */
} QOS_MGR_CMD_PORTID_INFO;
/*!
	\brief This is the data structure for DSL queue mib
 */
typedef struct {
	uint32_t queue_num; /*!< the queue number filled in the drop mib*/
	uint32_t drop_mib[16]; /*!< the drop mib counter */
	/*later other DSL mib counter will be added here*/
	uint32_t flag; /*!< reserved for futurer */
} QOS_MGR_DSL_QUEUE_MIB;
/*!
	\brief This is the data structure for general ENABLE/DISABLE INFO
 */
typedef struct {
	uint32_t enable; /*!< enable/disable flag */
	uint32_t flags; /*!< reserved for future */
} QOS_MGR_CMD_GENERAL_ENABLE_INFO;
/*!
	\brief This is the data structure to get the DSL queue drop mib for IOCTL.
 */
typedef struct {
	QOS_MGR_DSL_QUEUE_MIB mib; /*!< dsl queue mib coutner. */
	uint32_t flags; /*!< reserved for future */
} QOS_MGR_CMD_DSL_MIB_INFO;
/*!
	\brief This is the data structure for PPA QOS status
 */
typedef struct {
	uint32_t qos_queue_portid; /*!< the port id which support qos. at present, only one port can support QOS at run time */
	/*port's qos status*/
	uint32_t time_tick ; /*!< number of PP32 cycles per basic time tick */
	uint32_t overhd_bytes ; /*!< number of overhead bytes per packet in rate shaping */
	uint32_t eth1_eg_qnum ; /*!< maximum number of egress QoS queues; */
	uint32_t eth1_burst_chk; /*!< always 1, more accurate WFQ */
	uint32_t eth1_qss ; /*!< 1-FW QoS, 0-HW QoS */
	uint32_t shape_en ; /*!< 1-enable rate shaping, 0-disable */
	uint32_t wfq_en ; /*!< 1-WFQ enabled, 0-strict priority enabled */
	uint32_t tx_qos_cfg_addr; /*!< qos cfg address */
	uint32_t pp32_clk; /*!< pp32 clock */
	uint32_t basic_time_tick; /*!< pp32 qos time tick */
	uint32_t wfq_multiple; /*!< qos wfq multipler */
	uint32_t wfq_multiple_addr; /*!< qos wfq multipler address */
	uint32_t wfq_strict_pri_weight; /*!< strict priority's weight value */
	uint32_t wfq_strict_pri_weight_addr; /*!< strict priority's weight address */
	uint32_t wan_port_map; /*!< wan port interface register value */
	uint32_t wan_mix_map; /*!< mixed register value */
	QOS_MGR_INTERVAL qos_port_rate_internal; /*!< internal qos port parameters */
	QOS_MGR_INTERVAL ptm_qos_port_rate_shaping[4]; /*!< internal ptm qos port rate shaping parameters */
	uint32_t max_buffer_size; /*!< must match with below arrays, ie, here set to 8 */
	QOS_MGR_MIB_INFO mib[QOS_MGR_MAX_QOS_QUEUE_NUM]; /*!< Qos mib buffer */
	QOS_MGR_INTERVAL queue_internal[QOS_MGR_MAX_QOS_QUEUE_NUM]; /*!< internal qos queue parameters */
	QOS_MGR_DESC_CFG_INTERNAL desc_cfg_interanl[QOS_MGR_MAX_QOS_QUEUE_NUM];/*!< internal desc cfg parameters */
	uint32_t res; /*!< res flag for qos status succeed or not: possible value is QOS_MGR_SUCCESS OR QOS_MGR_FAILURE */
} QOS_MGR_STATUS;
/*!
	\brief This is the data structure for PPA QOS to get the status
 */
typedef struct {
	QOS_MGR_STATUS qstat; /*!< qos status buffer */
	uint32_t flags; /*!< Reserved currently */
} QOS_MGR_CMD_STATUS_INFO;
/*!
	\brief QoS Queue drop Mode Configuration
 */
typedef enum {
	QOS_MGR_DROP_TAIL = 0,
	QOS_MGR_DROP_RED = 1,
	QOS_MGR_DROP_WRED = 2,
	QOS_MGR_DROP_CODEL = 3
} QOS_MGR_DROP_MODE;
/*!
	\brief QoS WRED Drop configuration structure
	\remark If RED, then this is same as WRED Curve 0 only configuration
 */
typedef struct {
	uint32_t weight; /*!< Weighting factor for QAVG calculation */
	uint32_t min_th0; /*!< Min Threshold for WRED Curve 0 in % of qlen */
	uint32_t max_th0; /*!< Max Threshold for WRED Curve 0 in % of qlen */
	uint32_t max_p0; /*!< Max Drop Probability % at max_th0 for WRED Curve 0 */
	uint32_t min_th1; /*!< Min Threshold for WRED Curve 1 in % of
						qlen*/
	uint32_t max_th1; /*!< Max Threshold for WRED Curve 1 in % of
						qlen*/
	uint32_t max_p1; /*!< Max Drop Probability % at max_th1 for WRED Curve 1 */
	uint32_t drop_th1; /*!< Threshold for red color packet*/
} QOS_MGR_WRED;
/*!
	\brief QoS Queue Drop configuration structure
 */
typedef struct {
	int32_t enable; /*!< Whether shaper is enabled */
	QOS_MGR_DROP_MODE mode; /*!< Mode of Queue Drop - Taildrop, WRED
							 */
	QOS_MGR_WRED wred; /*!< WRED configuration of the queue */
	int32_t flags;  /*!< Drops flags - Drop for red color, yellow color, red color */
} QOS_MGR_DROP_CFG;
/*!
	\brief Engine Selector Capabilities Structure
 */
typedef struct {
	uint32_t q_max; /* Max number of queues supported by the engine */
	uint32_t q_cap; /* Bitmap to indicate various queueing capabilities supported by the engine */
	uint32_t q_len_max; /* Max queue length in Bytes supported by the engine */
	uint32_t cl_max; /* Max number of classifiers supported by the engine */
	uint32_t cl_cap; /* Bitmap to indicate various classifying capabilities supported by the engine */
} QOS_MGR_ENG_CAP;
/*!
	\brief Enum for List of Engines that can perform QoS
 */
typedef enum {
	QOS_MGR_RESOURCE_CPU,
	QOS_MGR_RESOURCE_FW,
	QOS_MGR_RESOURCE_HW,
	QOS_MGR_RESOURCE_PPA
} QOS_MGR_RESOURCE;
/*!
	\brief Enum for Module type to be configured in PPA Engine
 */
typedef enum QOS_MGR_module_type {
	QOS_MGR_MODULE_CLASSIFIER,
	QOS_MGR_MODULE_QUEUE,
	QOS_MGR_MODULE_BOTH
} QOS_MGR_MODULE_TYPE;
/*!
	\brief Enum for QoS Shaper Mode Configuration
 */
typedef enum {
	QOS_MGR_SCHED_SP = 0,
	QOS_MGR_SCHED_WFQ = 1,
	QOS_MGR_SCHED_WRR = 2,
} QOS_MGR_QSCHED_MODE;
/*!
	\brief QoS Modify Queue Configuration structure
 */
typedef struct {
	uint32_t	enable; /*!< Whether Queue is enabled */
	int32_t		weight; /*!< WFQ Weight */
	int32_t		priority; /*!< Queue Priority or Precedence. Start from 1-16, with 1 as highest priority */
	int32_t		qlen; /*!< Length of Queue in bytes */
	QOS_MGR_DROP_CFG	drop; /*!< Queue Drop Properties */
	QOS_MGR_SHAPER_CFG	shaper;/*!< Queue Shaper Properties*/
} QOS_MGR_CMD_MOD_QUEUE_CFG;
/*!
	\brief QoS Queue Add configuration info structure
 */
typedef struct {
	char				*ifname;/*!< Interface name on which Queue is to be added*/
	int32_t				queue_num; /*!< Logical queue number added will be filled up in this field */
	int32_t				weight; /* Weight of the Queue being added */ /*Added: venu*/
	int32_t				priority; /* Priority of the Queue being added */ /*Added: venu*/
	uint32_t			portid;/*!< Portid*/
	QOS_MGR_SHAPER_CFG		shaper;/*!< Queue Shaper Properties*/
	uint32_t			flags;/*!< Flags for future use*/
} QOS_MGR_CMD_ADD_QUEUE_INFO;
/*!
	\brief QoS Queue Modify configuration info structure
 */
typedef struct {
	char				*ifname;/*!< Interface name on which the Queue is modified*/
	QOS_MGR_CMD_MOD_QUEUE_CFG	*q;/*!< Modify Queue config*/
	uint32_t			flags;/*!< Flags for future use*/
} QOS_MGR_CMD_MOD_QUEUE_INFO;
/*!
	\brief QoS Queue Delete configuration info structure
 */
typedef struct {
	char				*ifname;/*!< Interface name on which Queue is being deleted*/
	uint32_t			queueid;/*!< Queue id*/
	uint32_t			portid;/*!< Portid*/
} QOS_MGR_CMD_DEL_QUEUE_INFO;
/*!
	\brief QoS Queue common configuration info structure for ADD/MODIFY/DEL operation
 */
typedef struct {
	QOS_MGR_IFNAME			ifname[QOS_MGR_IF_NAME_SIZE];/*!< Interface name on which the Queue is modified*/
	char				tc_map[MAX_TC_NUM]; /*!< Which all Traffic Class(es) map to this Queue */
	uint8_t				tc_no; /*!< Number of Traffic Class(es) map to this Queue */
	uint8_t				flowId_en; /*!<Enable/Disable for flow Id + tc bits used for VAP's & Vlan interfaces*/
	uint16_t			flowId; /*!<flow Id + tc bits used for VAP's & Vlan interfaces*/
	uint32_t			enable; /*!< Whether Queue is enabled */
	int32_t				weight; /*!< WFQ Weight */
	int32_t				priority; /*!< Queue Priority or Precedence. Start from 1-16, with 1 as highest priority */
	int32_t				qlen; /*!< Length of Queue in bytes */
	int32_t				queue_num; /*!< Logical queue number added will be filled up in this field */
	int32_t				shaper_num; /*!< Logical queue number added will be filled up in this field */
	uint32_t			portid;/*!< Portid*/
	QOS_MGR_DROP_CFG		drop; /*!< Queue Drop Properties */
	QOS_MGR_SHAPER_CFG		shaper;/*!< Queue Shaper Properties*/
	QOS_MGR_QSCHED_MODE		sched;/*!< Queue Scheduler Properties*/
	uint32_t			flags;/*!< Flags for future use*/
} QOS_MGR_CMD_QUEUE_INFO;
/*!
	\brief QoS subif port info structure
 */
typedef struct {
	QOS_MGR_IFNAME ifname[QOS_MGR_IF_NAME_SIZE];/*!< Interface name on which the Queue is modified*/
	int32_t		port_id; /*!< Portid*/
	uint32_t	priority_level; /*!< Queue Priority or Precedence. Start from 1-16, with 1 as highest priority */
	uint32_t	weight; /*!< WFQ Weight */
	uint32_t	flags; /*!< Flags for future use*/
} QOS_MGR_CMD_SUBIF_PORT_INFO;
/*!
	\brief QoS classifier rule fields
 */
enum qos_mgr_rule_field_type {
	QOS_MGR_FLD_SRC_MAC,
	QOS_MGR_FLD_DST_MAC,
	QOS_MGR_FLD_ETH_TYPE,
	QOS_MGR_FLD_IPV4_SRC_IP,
	QOS_MGR_FLD_IPV4_DST_IP,
	QOS_MGR_FLD_IPV6_SRC_IP,
	QOS_MGR_FLD_IPV6_DST_IP,
	QOS_MGR_FLD_IP_PROTO,
	QOS_MGR_FLD_L4_SRC_PORT,
	QOS_MGR_FLD_L4_DST_PORT,
	QOS_MGR_FLD_ICMP_TYPE_CODE,
	QOS_MGR_FLD_IPV4_SRC_MASK,
	QOS_MGR_FLD_IPV4_DST_MASK,
	QOS_MGR_FLD_PPP_PROTO,
	QOS_MGR_FLD_APP_DATA_LSB,
	QOS_MGR_FLD_APP_DATA_MSB,
	QOS_MGR_FLD_COUNT
};
/*!
	\brief QoS classifier rule type
 */
enum rule_type {
	QOS_MGR_CHECKER,
	QOS_MGR_PCE
};
/*!
	\brief QoS classifier rule direction
 */
enum rule_dir {
	QOS_MGR_INGRESS,
	QOS_MGR_EGRESS
};
/*!
	\brief Structure to store the QoS classifier rule fields
 */
struct qos_mgr_rule_fields {
	enum qos_mgr_rule_field_type type;
	union {
		uint8_t ip_proto;
		uint8_t dst_mac[ETH_ALEN];
		uint8_t src_mac[ETH_ALEN];
		uint16_t eth_type;
		uint16_t src_port;
		uint16_t dst_port;
		uint16_t icmp_type_code;

		union {
			uint32_t v4;
		} src_ip;
		uint32_t src_ip_mask;
		union {
			uint32_t v4;
		} dst_ip;
		uint32_t dst_ip_mask;
		uint16_t PPP_proto;
		uint16_t app_data_lsb;
		uint16_t app_data_msb;
	};
};
/*!
	\brief Structure to store the QoS classifier rule
 */
typedef struct {
	struct qos_mgr_rule_fields fields[MAX_FIELDS_PER_INGRESS_RULE];
	int field_count;
} class_rule_t;

typedef struct {
	class_rule_t	rules;
	uint32_t	region;
	uint32_t	eTrafficClassAction;
	uint8_t		nTrafficClassAlternate;
	bool		bEnable;
	bool		bSubIfIdEnable;
	bool		bPortIdEnable;
	bool		bEtherTypeEnable;
	bool		eSrcIP_Select;
	bool		eDstIP_Select;
	uint16_t	nEtherTypeMask;
	uint32_t	nSrcIP_Mask;
	uint32_t	nDstIP_Mask;
	uint8_t		nPortId;
	uint16_t	nSubIfId;
} pcecfg_t;
/*!
	\brief Structure to store the PCE classifier parameters
 */
typedef struct {
	class_rule_t rules;
	char group_name[MAX_INGGROUP_NAME_LENGTH]; /* Only for Ingress */
	int priority;
} chkrcfg_t;
/*!
	\brief This is the data structure for PPA QOS to add classifier
 */
typedef struct {
	enum rule_type type;/* CHECKER/GSW */
	enum rule_dir dir; /* INGRESS/EGRESS */

	union
	{
		pcecfg_t	pcecfg;
		chkrcfg_t	chkrcfg;
	} rulecfg;
} QOS_MGR_CMD_ADD_CLASSIFIER_INFO;
typedef enum{
	QOS_MGR_INGGRP0,
	QOS_MGR_INGGRP1,
	QOS_MGR_INGGRP2,
	QOS_MGR_INGGRP3,
	QOS_MGR_INGGRP4,
	QOS_MGR_INGGRP5,
	QOS_MGR_INGGRP6,
	QOS_MGR_INGGRP_MAX
} QOS_MGR_INGGRP;
/*!
	\brief This enum is for CPU INGRESS GROUP OPTIONS
 */
enum {
	INGRESS_OPTION_0,
	INGRESS_OPTION_1,
	INGRESS_OPTION_MAX
};
/*!
	\brief This is  data structure for CPU INGRESS GROUP
 */
typedef struct cpu_ingress_group {
	char ifnames[MAX_IF_PER_CPU_INGRESS_GROUP][IFNAMSIZ];
	uint8_t if_cnt;
	char grp_name[MAX_INGGROUP_NAME_LENGTH];
	uint8_t grp_id;
	uint8_t ing_op;
} CPU_INGGRP;
/*!
	\brief This is the data structure for PPA QOS to get the status
 */
typedef struct {
	QOS_MGR_IFNAME ifname[QOS_MGR_IF_NAME_SIZE];
	QOS_MGR_INGGRP ingress_group;
	CPU_INGGRP cpu_inggrp[NUM_CPU_INGRESS_GROUPS];
	uint32_t flowId:2;
	uint32_t flowId_en:1;
	uint32_t enc:1;
	uint32_t dec:1;
	uint32_t mpe1:1;
	uint32_t mpe2:1;
	uint32_t tc:4;
	uint32_t ep:4;
	uint32_t res:17;
	uint32_t flags;
} QOS_MGR_CMD_INGGRP_INFO;
typedef struct {
	uint32_t enable; /*!< Whether Queue is enabled */
	uint32_t status; /*!< Operational status of the queue. Only
						 valid on GET */
	char name[MAX_Q_NAME_LEN]; /*!< Name or Alias of the
								 queue, if any */
	uint32_t owner; /*<! Enumerated value of who is doing the operation.
						Possible owners can be: TR-069, Web GUI, CLI, System, BaseRules */
	uint32_t flags; /*<! Following flags can be kept per
						filter structure -
						- QOS_Q_F_MODIFIED: Modified after being originally added
						- QOS_Q_F_MODIFIED_OTHER: Modified by an entity other than the owner
						- QOS_Q_F_HW: Set if this Q is in HW
						- QOS_Q_F_INGRESS: This queue is applied to ingress from the interface */
	uint32_t queue_id; /*!< Queue Id for other operations */
	char	 tc_map[MAX_TC_NUM]; /*!< Which all Traffic Class(es) map to this Queue */
	uint8_t		tc_no; /*!< Number of Traffic Class(es) map to this Queue */
	/* ?? What about additional qselectors possible in GRX350 */
	int32_t qlen; /*!< Length of Queue in bytes */
	int32_t weight; /*!< WFQ Weight */
	int32_t priority; /*!< Queue Priority or Precedence. Start from 1-16, with 1 as highest priority */
	QOS_MGR_DROP_CFG drop; /*!< Queue Drop Properties */
	QOS_MGR_QSCHED_MODE sched; /*!< QoS scheduler mode - Priority, WFQ */
	QOS_MGR_SHAPER_CFG shaper; /*!< Shaper Configuration */
} QOS_MGR_QUEUE_CFG;
/*!
	\brief PPA API Engine Info Configuration structure
 */
typedef struct {
	uint32_t engine_id; /* to be alloted by engine selector, es can create and pass this id to engine on first invoke, Id can be allotted from enum engines_t */
	int32_t (*init) (QOS_MGR_MODULE_TYPE); /* Callback function to initialize the engine*/
	int32_t (*fini) (QOS_MGR_MODULE_TYPE); /* Callback function to un-initialize the engine*/
	int32_t (*queue_add) (char *, QOS_MGR_QUEUE_CFG *); /* Callback function to add a queue for the given interface in the engine*/
	int32_t (*queue_modify) (char *, QOS_MGR_QUEUE_CFG *); /* Callback function to modify a queue for the given interface in the engine*/
	int32_t (*queue_delete) (char *, QOS_MGR_QUEUE_CFG *); /* Callback function to delete a queue for the given interface in the engine*/
	int32_t (*class_add) (QOS_MGR_QUEUE_CFG *); /* Callback function to add a classifier for the given interface in the engine*/
	int32_t (*class_modify) (QOS_MGR_QUEUE_CFG *); /* Callback function to modify a classifier for the given interface in the engine*/
	int32_t (*class_delete) (QOS_MGR_QUEUE_CFG *); /* Callback function to delete a classifier for the given interface in the engine*/
	/*Callback for port shape*/
	QOS_MGR_ENG_CAP engine_capab; /* engine's capabilities */
	QOS_MGR_RESOURCE used_resource; /* Used resource to achieve the functionality. each engine shall provide its capabilities and resources needed for that, so that engine selector can use this as well as some other info like number of sessions handeled by a particular resource at present. eg switch can do queueing in hardware and sw engine can do the same using CPU. */
} QOS_MGR_CMD_ENGINE_INFO;
typedef QOS_MGR_SHAPER_CFG QOS_MGR_CMD_SHAPER_INFO;
/*!
	\brief This is the data structure for IOCTL
 */
typedef union {
	QOS_MGR_CMD_INGGRP_INFO qos_inggrp_info; /*!<PPA engine ingress qos info parameter */
	/* If not required ADD/MOD/DEL Queue info structures can be removed later.*/
	QOS_MGR_CMD_ADD_QUEUE_INFO	 qos_add_queue_info; /*!<PPA engine info parameter */
	QOS_MGR_CMD_MOD_QUEUE_INFO	 qos_mod_queue_info; /*!<PPA engine info parameter */
	QOS_MGR_CMD_DEL_QUEUE_INFO	 qos_del_queue_info; /*!<PPA engine info parameter */
	QOS_MGR_CMD_QUEUE_INFO	 qos_queue_info; /*!<PPA engine info parameter */
	QOS_MGR_CMD_SHAPER_INFO		 qos_shaper_info;/*!<PPA Qos shaper info parameter */
	QOS_MGR_CMD_SUBIF_PORT_INFO	 subif_port_info; /*!<PPA Qos subif port info parameter */
	QOS_MGR_CMD_STATUS_INFO qos_status_info; /*!< PPA qos status parameter */
	QOS_MGR_CMD_QUEUE_NUM_INFO qnum_info; /*!< PPA qos queue parameter */
	QOS_MGR_CMD_CTRL_INFO qos_ctrl_info; /*!< PPA qos control parameter */
	QOS_MGR_CMD_MIB_INFO qos_mib_info; /*!< PPA qos mib parameter */
	QOS_MGR_CMD_RATE_INFO qos_rate_info; /*!< PPA qos rate shapping parameter */
	QOS_MGR_CMD_WFQ_INFO qos_wfq_info; /*!< PPA qos wfq parameter */
	QOS_MGR_CMD_HOOK_LIST_INFO hook_list_info; /*!< PPA all registered hook list */
	QOS_MGR_CMD_HOOK_ENABLE_INFO hook_control_info; /*!< PPA control information: enable/disable */
	QOS_MGR_CMD_DSL_MIB_INFO dsl_mib_info; /*!< PPA DSL queue mib counter. At present only drop counter is supported */
	QOS_MGR_CMD_GENERAL_ENABLE_INFO genernal_enable_info; /*!< General PPA enable/disable info. */
	QOS_MGR_METER_INFO meter_info; /*!< PPA qos meter info. */
	QOS_MGR_CMD_ADD_CLASSIFIER_INFO qos_add_classifier_info; /*!< PPA qos classifier info. */
} QOS_MGR_CMD_DATA;
/*@}*/ /* QOS_MGR_IOCTL */
/* -------------------------------------------------------------------------- */
/* IOCTL Command Definitions */
/* -------------------------------------------------------------------------- */
/** \addtogroup QOS_MGR_IOCTL */
/*@{*/
/** PPA IOCTL NR values
 */
typedef enum {
	QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM */
	QOS_MGR_CMD_SET_QOS_WFQ_NR, /*!< NR for QOS_MGR_CMD_SET_QOS_WFQ */
	QOS_MGR_CMD_GET_QOS_WFQ_NR, /*!< NR for QOS_MGR_CMD_SET_QOS_WFQ */
	QOS_MGR_CMD_RESET_QOS_WFQ_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_WFQ */
	QOS_MGR_CMD_GET_HOOK_COUNT_NR, /*!< NR for QOS_MGR_CMD_GET_HOOK_COUNT */
	QOS_MGR_CMD_GET_HOOK_LIST_NR, /*!< NR for QOS_MGR_CMD_GET_HOOK_LIST */
	QOS_MGR_CMD_SET_HOOK_NR, /*!< NR for QOS_MGR_CMD_SET_HOOK */
	QOS_MGR_CMD_SET_CTRL_QOS_WFQ_NR, /*!< NR for QOS_MGR_CMD_SET_CTRL_QOS_WFQ */
	QOS_MGR_CMD_GET_CTRL_QOS_WFQ_NR, /*!< NR for QOS_MGR_CMD_GET_CTRL_QOS_WFQ */
	QOS_MGR_CMD_SET_CTRL_QOS_RATE_NR, /*!< NR for QOS_MGR_CMD_SET_CTRL_QOS_RATE */
	QOS_MGR_CMD_GET_CTRL_QOS_RATE_NR, /*!< NR for QOS_MGR_CMD_GET_CTRL_QOS_RATE */
	QOS_MGR_CMD_SET_QOS_RATE_NR, /*!< NR for QOS_MGR_CMD_SET_QOS_RATE */
	QOS_MGR_CMD_GET_QOS_RATE_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_RATE */
	QOS_MGR_CMD_RESET_QOS_RATE_NR, /*!< NR for QOS_MGR_CMD_RESET_QOS_RATE */
	QOS_MGR_CMD_SET_QOS_INGGRP_NR, /*NR for QOS_MGR_CMD_SET_QOS_INGGRP*/
	QOS_MGR_CMD_GET_QOS_INGGRP_NR, /*NR for QOS_MGR_CMD_GET_QOS_INGGRP*/
	QOS_MGR_CMD_ADD_QOS_QUEUE_NR, /*! NR for QOS_MGR_CMD_ADD_QOS_QUEUE */
	QOS_MGR_CMD_MOD_QOS_QUEUE_NR, /*! NR for QOS_MGR_CMD_MOD_QOS_QUEUE */
	QOS_MGR_CMD_DEL_QOS_QUEUE_NR, /*! NR for QOS_MGR_CMD_DEL_QOS_QUEUE */
	QOS_MGR_CMD_ENG_QUEUE_INIT_NR, /*! NR for QOS_MGR_CMD_ENG_QUEUE_INIT */
	QOS_MGR_CMD_ENG_QUEUE_UNINIT_NR, /*! NR for QOS_MGR_CMD_ENG_QUEUE_UNINIT */
	QOS_MGR_CMD_MOD_SUBIF_PORT_NR, /*! NR for QOS_MGR_CMD_MOD_SUBIF_PORT */
	QOS_MGR_CMD_SET_QOS_SHAPER_NR, /*!< NR for QOS_MGR_CMD_SET_QOS_SHAPER	*/
	QOS_MGR_CMD_GET_QOS_SHAPER_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_SHAPER	*/
	QOS_MGR_CMD_ADD_WMM_QOS_QUEUE_NR, /*! NR for QOS_MGR_CMD_ADD_WMM_QOS_QUEUE */
	QOS_MGR_CMD_DEL_WMM_QOS_QUEUE_NR, /*! NR for QOS_MGR_CMD_DEL_WMM_QOS_QUEUE */
	QOS_MGR_CMD_GET_QOS_MIB_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_MIB */
	QOS_MGR_CMD_GET_PORTID_NR, /*!< NR for QOS_MGR_GET_CMD_PORTID */
	QOS_MGR_CMD_GET_DSL_MIB_NR, /*!< NR for QOS_MGR_GET_DSL_MIB */
	QOS_MGR_CMD_CLEAR_DSL_MIB_NR, /*!< NR for QOS_MGR_CLEAR_DSL_MIB */
	QOS_MGR_CMD_GET_PORT_MIB_NR, /*!< NR for QOS_MGR_GET_PORT_MIB */
	QOS_MGR_CMD_CLEAR_PORT_MIB_NR, /*!< NR for QOS_MGR_CLEAR_PORT_MIB */
	QOS_MGR_CMD_GET_QOS_STATUS_NR, /*!< NR for QOS_MGR_CMD_GET_QOS_STATUS */
	QOS_MGR_CMD_SET_QOS_METER_NR, /*!< NR for QOS_MGR_CMD_SET_QOS_METER */
	QOS_MGR_CMD_DSCP_CLASS_SET_NR, /*!< NR for QOS_MGR_CMD_DSCP_CLASS_SET */
	QOS_MGR_CMD_DSCP_CLASS_RESET_NR, /*!< NR for QOS_MGR_CMD_DSCP_CLASS_RESET */
	QOS_MGR_CMD_ADD_CLASSIFIER_NR, /*!< NR for QOS_MGR_CMD_ADD_CLASSIFIER */
	/* QOS_MGR_IOC_MAXNR should be the last one in the enumberation */
	QOS_MGR_IOC_MAXNR /*!< NR for QOS_MGR_IOC_MAXNR */
} QOS_MGR_IOC_NR;
/**
 * PPA Get QoS Ingress Group. Value is manipulated by _IOW() macro for final value
 * \param [in,out] QOS_MGR_CMD_INGGRP_INFO The parameter points to a
 * \ref QOS_MGR_CMD_INGGRP_INFO structure
 */
#define QOS_MGR_CMD_GET_QOS_INGGRP _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_INGGRP_NR, QOS_MGR_CMD_INGGRP_INFO)
/**
 * PPA Set QoS Ingress Group. Value is manipulated by _IOW() macro for final value
 * \param [in,out] QOS_MGR_CMD_INGGRP_INFO The parameter points to a
 * \ref QOS_MGR_CMD_INGGRP_INFO structure
 */
#define QOS_MGR_CMD_SET_QOS_INGGRP _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_QOS_INGGRP_NR, QOS_MGR_CMD_INGGRP_INFO)
/** PPA GET QOS status. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_STATUS_INFO The parameter points to a
	\ref QOS_MGR_CMD_STATUS_INFO structure
 */
#define QOS_MGR_CMD_GET_QOS_STATUS _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_STATUS_NR, QOS_MGR_CMD_STATUS_INFO)
/** PPA GET the maximum queue supported for WFQ/RateShapping. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_QUEUE_NUM_INFO The parameter points to a
	\ref QOS_MGR_CMD_QUEUE_NUM_INFO structure
	\note: portid is input parameter, and queue_num is output value .
 */
#define QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM_NR, QOS_MGR_CMD_QUEUE_NUM_INFO)
/** PPA GET the QOS mib counter. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_QUEUE_NUM_INFO The parameter points to a
	\ref QOS_MGR_CMD_QUEUE_NUM_INFO structure
	\note: portid is input parameter, and queue_num is output value .
 */
#define QOS_MGR_CMD_GET_QOS_MIB _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_MIB_NR, QOS_MGR_CMD_MIB_INFO)
/** PPA Enable/Disable QOS WFQ feature. Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_CTRL_INFO The parameter points to a
	\ref QOS_MGR_CMD_CTRL_INFO structure
 */
#define QOS_MGR_CMD_SET_CTRL_QOS_WFQ _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_CTRL_QOS_WFQ_NR, QOS_MGR_CMD_CTRL_INFO)
/** PPA get QOS WFQ feature status: enabled or disabled. Value is manipulated by _IOR() macro for final value
	\param[in] QOS_MGR_CMD_CTRL_INFO The parameter points to a
	\ref QOS_MGR_CMD_CTRL_INFO structure
 */
#define QOS_MGR_CMD_GET_CTRL_QOS_WFQ _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_CTRL_QOS_WFQ_NR, QOS_MGR_CMD_CTRL_INFO)
/** PPA Set WFQ weight. Value is manipulated by _IOW() macro for final value
	\param QOS_MGR_CMD_WFQ_INFO The parameter points to a
	\ref QOS_MGR_CMD_WFQ_INFO structure
 */
#define QOS_MGR_CMD_SET_QOS_WFQ _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_QOS_WFQ_NR, QOS_MGR_CMD_WFQ_INFO)
/** PPA Get WFQ weight. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_WFQ_INFO The parameter points to a
	\ref QOS_MGR_CMD_WFQ_INFO structure
	\note portid, queueid and weight should be set accordingly.
 */
#define QOS_MGR_CMD_GET_QOS_WFQ _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_WFQ_NR, QOS_MGR_CMD_WFQ_INFO)
/** PPA Reset WFQ weight. Value is manipulated by _IOW() macro for final value
	\param[out] QOS_MGR_CMD_WFQ_INFO The parameter points to a
	\ref QOS_MGR_CMD_WFQ_INFO structure
	\note: portid/queueid is input parameter, and weight is output value.
 */
#define QOS_MGR_CMD_RESET_QOS_WFQ _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_RESET_QOS_WFQ_NR, QOS_MGR_CMD_WFQ_INFO)
/** PPA Enable/Disable QOS Rate Shaping feature. Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_CTRL_INFO The parameter points to a
	\ref QOS_MGR_CMD_CTRL_INFO structure
 */
#define QOS_MGR_CMD_SET_CTRL_QOS_RATE _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_CTRL_QOS_RATE_NR, QOS_MGR_CMD_CTRL_INFO)
/** PPA get QOS Rate Shaping feature status: enabled or disabled. Value is manipulated by _IOR() macro for final value
	\param[in] QOS_MGR_CMD_CTRL_INFO The parameter points to a
	\ref QOS_MGR_CMD_CTRL_INFO structure
 */
#define QOS_MGR_CMD_GET_CTRL_QOS_RATE _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_CTRL_QOS_RATE_NR, QOS_MGR_CMD_CTRL_INFO)
/** PPA Set QOS rate shaping. Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_RATE_INFO The parameter points to a
	\ref QOS_MGR_CMD_RATE_INFO structure
 */
#define QOS_MGR_CMD_SET_QOS_RATE _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_QOS_RATE_NR, QOS_MGR_CMD_RATE_INFO)
/** PPA Get QOS Rate shaping configuration. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_RATE_INFO The parameter points to a
	\ref QOS_MGR_CMD_RATE_INFO structure
	\note portid, queueid and weight should be set accordingly.
 */
#define QOS_MGR_CMD_GET_QOS_RATE _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_RATE_NR, QOS_MGR_CMD_RATE_INFO)
/** PPA Reset QOS Rate shaping. Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_RATE_INFO The parameter points to a
	\ref QOS_MGR_CMD_RATE_INFO structure
	\note: portid/queueid is input parameter, and weight is output value.
 */
#define QOS_MGR_CMD_RESET_QOS_RATE _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_RESET_QOS_RATE_NR, QOS_MGR_CMD_RATE_INFO)
/** PPA Add QOS Queue. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_ADD_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_ADD_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_ADD_QOS_QUEUE _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_ADD_QOS_QUEUE_NR, QOS_MGR_CMD_QUEUE_INFO)
/** PPA Modify QOS Queue. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_MOD_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_MOD_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_MOD_QOS_QUEUE _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_MOD_QOS_QUEUE_NR, QOS_MGR_CMD_QUEUE_INFO)
/** PPA Delete QOS Queue. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_DEL_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_DEL_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_DEL_QOS_QUEUE _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_DEL_QOS_QUEUE_NR, QOS_MGR_CMD_QUEUE_INFO)
/** PPA Add WMM QOS Queue. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_ADD_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_ADD_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_ADD_WMM_QOS_QUEUE _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_ADD_WMM_QOS_QUEUE_NR, QOS_MGR_CMD_QUEUE_INFO)
/** PPA Delete WMM QOS Queue. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_DEL_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_DEL_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_DEL_WMM_QOS_QUEUE _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_DEL_WMM_QOS_QUEUE_NR, QOS_MGR_CMD_QUEUE_INFO)
#define QOS_MGR_CMD_ENG_QUEUE_INIT _IO(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_ENG_QUEUE_INIT_NR)
/** PPA to get the QOS port id of one specified interface name. Value is manipulated by _IOR() macro for final value
	\param PPA_CMD_MAX_ENTRY_INFO The parameter points to a
	\ref PPA_CMD_MAX_ENTRY_INFO structure
 */
#define QOS_MGR_CMD_GET_PORTID _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_PORTID_NR, QOS_MGR_CMD_PORTID_INFO)
/** PPA UnInit GSWIP-L Queues. Value is manipulated by _IOW() macro for final value
	\param[in,out] QOS_MGR_CMD_DEL_QUEUE_INFO The parameter points to a
	\ref QOS_MGR_CMD_DEL_QUEUE_INFO structure
 */
#define QOS_MGR_CMD_ENG_QUEUE_UNINIT _IO(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_ENG_QUEUE_UNINIT_NR)
/** PPA Modify QOS Sub interface to Port . Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_SUBIF_PORT_INFO The parameter points to a
	\ref QOS_MGR_CMD_SUBIF_PORT_INFO structure
 */
#define QOS_MGR_CMD_MOD_SUBIF_PORT _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_MOD_SUBIF_PORT_NR, QOS_MGR_CMD_SUBIF_PORT_INFO)
/** PPA Set QOS rate shaper. Value is manipulated by _IOW() macro for final value
	\param[in] QOS_MGR_CMD_RATE_INFO The parameter points to a
	\ref QOS_MGR_CMD_RATE_INFO structure
 */
#define QOS_MGR_CMD_SET_QOS_SHAPER _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_QOS_SHAPER_NR, QOS_MGR_CMD_RATE_INFO)
/** PPA Get QOS Rate shaper configuration. Value is manipulated by _IOR() macro for final value
	\param[out] QOS_MGR_CMD_RATE_INFO The parameter points to a
	\ref QOS_MGR_CMD_RATE_INFO structure
	\note portid, queueid and weight should be set accordingly.
 */
#define QOS_MGR_CMD_GET_QOS_SHAPER _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_QOS_SHAPER_NR, QOS_MGR_CMD_RATE_INFO)
/** PPA Set QOS rate Meter. Value is manipulated by _IOW() macro for final value
 *   \param[in] QOS_MGR_METER_CFG The parameter points to a
 *     \ref QOS_MGR_METER_CFG structure
 *      */
#define QOS_MGR_CMD_SET_QOS_METER _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_QOS_METER_NR, QOS_MGR_METER_INFO)
/* PPA Set DSCP to TC Map */
#define QOS_MGR_CMD_DSCP_CLASS_SET _IO(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_DSCP_CLASS_SET_NR)
/* PPA Reset DSCP to TC Map */
#define QOS_MGR_CMD_DSCP_CLASS_RESET _IO(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_DSCP_CLASS_RESET_NR)
/** PPA get all exported hook count. Value is manipulated by _IOR() macro for final value
	\param QOS_MGR_CMD_COUNT_INFO The parameter points to a
	\ref QOS_MGR_CMD_COUNT_INFO structure
 */
#define QOS_MGR_CMD_GET_HOOK_COUNT _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_HOOK_COUNT_NR, QOS_MGR_CMD_COUNT_INFO)
/** PPA get the exported hook list. Value is manipulated by _IOR() macro for final value
	\param QOS_MGR_CMD_HOOK_LIST_INFO The parameter points to a
	\ref QOS_MGR_CMD_HOOK_LIST_INFO structure
 */
#define QOS_MGR_CMD_GET_HOOK_LIST _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_HOOK_LIST_NR, QOS_MGR_CMD_HOOK_LIST_INFO)
/** PPA to enable/disable the exported hook. Value is manipulated by _IOW() macro for final value
	\param QOS_MGR_CMD_HOOK_ENABLE_INFO The parameter points to a
	\ref QOS_MGR_CMD_HOOK_ENABLE_INFO structure
 */
#define QOS_MGR_CMD_SET_HOOK _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_SET_HOOK_NR, QOS_MGR_CMD_HOOK_ENABLE_INFO)
/** PPA Get DSL MIB info. Value is manipulated by _IOR() macro for final value
	\param QOS_MGR_CMD_GET_DSL_MIB The parameter points to a
	\ref QOS_MGR_CMD_GET_DSL_MIB structure
 */
#define QOS_MGR_CMD_GET_DSL_MIB _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_GET_DSL_MIB_NR, QOS_MGR_CMD_DSL_MIB_INFO)
/** PPA Clear DSL MIB info. Value is manipulated by _IOR() macro for final value
	\param QOS_MGR_CMD_CLEAR_DSL_MIB The parameter points to a
	\ref QOS_MGR_CMD_CLEAR_DSL_MIB structure
 */
#define QOS_MGR_CMD_CLEAR_DSL_MIB _IOR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_CLEAR_DSL_MIB_NR, QOS_MGR_CMD_DSL_MIB_INFO)
/** PPA Clear PORT MIB info. Value is manipulated by _IOR() macro for final value
	\param QOS_MGR_CMD_CLEAR_PORT_MIB The parameter points to a
	\ref QOS_MGR_CMD_CLEAR_PORT_MIB structure
 */
#define QOS_MGR_CMD_CLEAR_PORT_MIB _IOW(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_CLEAR_PORT_MIB_NR, QOS_MGR_CMD_PORT_MIB_INFO)
/*@}*/ /* QOS_MGR_IOCTL */
/**
 * PPA Set QoS add classifier. Value is manipulated by _IOW() macro for final value
 * \param [in,out] QOS_MGR_CMD_ADD_CLASSIFIER The parameter points to a
 * \ref QOS_MGR_CMD_ADD_CLASSIFIER structure
 */
#define QOS_MGR_CMD_ADD_CLASSIFIER _IOWR(QOS_MGR_IOC_MAGIC, QOS_MGR_CMD_ADD_CLASSIFIER_NR, QOS_MGR_CMD_ADD_CLASSIFIER_INFO)
/*!
        \brief Macro that specifies PPA network interface data structure
 */
typedef struct notifier_block   QOS_MGR_NOTIFIER_BLOCK;

#endif /* UAPI_QOS_MGR_API_H*/
